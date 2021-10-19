/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\n
\par		PM_Safety_WM_NTCProbe
\n
\n
\file		PM_SFT_WM_NTCProbe.c
\ingroup	PM_SFT_NTCProbe
\date		04/02/2013
\author		Salvatore Fimiani
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SFT_WM_NTCProbe.h"
#include "LIB_Utilities.h"
#include "LoadsRequestExp.h"
#include "SWTimersCounters.h"
#include "LIB_ClassBVar.h"
#include "PM_SFT_WM_washRes.h"

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
#include "LIB_ResetExp.h"
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* Defines related to GI MAP */

#define EEP_GIMAP_TABPTR_OFFSET 		0x01 	/* GIMAP displacement vs PTR_IO_CONFIG_DATA */
#define GIMAP_TABLE_ITEM_SIZE_IN_BYTES 	0x04 	/* Size in bytes of a single item inside the G.I. MAP table */


#define GI_PROBE_ID						0x00
/*#define GI_PROBE_POS_NOT_VALID			255*/
#define BAD_PARAMETERS_SF				255


#define GI_READ_ID(pByte)				*(pByte+1)
#define GI_READ_POSITION(pByte)			*(pByte+3)

/* End of defines related to GI MAP */


#define DSPL_PROBEFLT_WASH_PARAM			0x02u	/* Probe Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_PREF_TIME				0
#define DSPL_OFFSET_FAULT_TIME				3
#define DSPL_OFFSET_RESON_DELTA				7
#define DSPL_OFFSET_PROBE_POS				8
#define DSPL_OFFSET_COOLDOWN_TIMEOUT		9
#define DSPL_OFFSET_SHORTED_FAULT_THR		10
#define DSPL_OFFSET_OPEN_FAULT_THR			12
#define DSPL_OFFSET_MIN_SAFE_THR			14
#define DSPL_OFFSET_MAX_SAFE_THR			16
#define DSPL_OFFSET_RESON_TIMEOUT			18


#if defined (CONFIG_APPLIANCE_WD)
#define DSPL_PROBEFLT_DRY_PARAM				0x0Du	/* Probe Fault Parameters displacement vs SF_PTR_FAULT */
#endif /* CONFIG_APPLIANCE_WD */



#define ADC_VALUE_MASK  0x0FFF
#define LOW_NIBBLE		0x0F



#define PROBE_CH_TIMEOUT	2 	/* Seconds */

#define KEEP_RESON_TIME		10	/* 10 = 1 */


#define ZC_NOTGOOD_TIMEOUT	50

#define LOAD_STATUS_UNDEFINED	0xFF
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/**
 * Info related to the loads that trigger checks for blocked probe
*/






/* ********* */
/* Variables */
/* ********* */

/* Global */




/* To be inserted into Backup */
uint16_t coolDownCnt;			/* Counter for measuring time between door opening and closing */

SafetyBkNTCProbe_t persWashNTCProbe  = {FALSE,FAULTSRC_NTCPROBE_NOFAULT,FALSE,0,0};
SafetyNTCProbe_t washNTCProbe;
SampleNTCProbe_t washProbeSample = {0,0,0xFFFF,0};


uint16_t resOFFTimeoutCnt;

LoadInfos_t prbWashRes = {LOAD_NOT_FOUND,LOAD_STATUS_UNDEFINED,LOAD_STATUS_UNDEFINED}; 	/* Info about the Washing Resistor load 	 */

uint8_t prbDoorLockPos = LOAD_NOT_FOUND;
LoadInfos_t prbWashDoorRef =  {LOAD_NOT_FOUND,LOAD_STATUS_UNDEFINED,LOAD_STATUS_UNDEFINED};
LoadInfos_t prbDryDoorRef =  {LOAD_NOT_FOUND,LOAD_STATUS_UNDEFINED,LOAD_STATUS_UNDEFINED};

uint8_t justTurnedON = TRUE;
/* Static */

/* Counter to count the time the ZCgood signal is not present */
static uint8_t zcNotGoodCnt = 0;

#if defined (CONFIG_APPLIANCE_WD)
SampleNTCProbe_t dryProbeSample = {0,0,0xFFFF,0};
LoadInfos_t prbDryFan =  {LOAD_NOT_FOUND,LOAD_STATUS_UNDEFINED,LOAD_STATUS_UNDEFINED};	/* Info about the Dryer Fan load 			 */
LoadInfos_t prbDryRes =  {LOAD_NOT_FOUND,LOAD_STATUS_UNDEFINED,LOAD_STATUS_UNDEFINED};	/* Info about the Dryer Resistor load 		 */
SafetyNTCProbe_t dryNTCProbe;
SafetyBkNTCProbe_t persDryNTCProbe = {FALSE,FAULTSRC_NTCPROBE_NOFAULT,FALSE,0,0};   /* To be backupped */
#endif /* CONFIG_APPLIANCE_WD */


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

static bool_t PM_SFT_WM_BlockedProbeTriggerCheck(eNTCProbeType prbType, LoadInfos_t doorLockRef, SafetyBkNTCProbe_t *prbSftPers,
		SampleNTCProbe_t *prbSample, bool_t isCntEnabled);
static void PM_SFT_WM_CheckForProbeChReady(SafetyNTCProbe_t *prbNTC);

static void PM_SFT_WM_NTCProbe_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags, SafetyNTCProbe_t *prbNTC,
		SampleNTCProbe_t *prbSample, SafetyBkNTCProbe_t *prbSftPers);


static void PM_SFT_WM_NTCProbe_GeneralInit(sft_module_status_t *probeMod, eNTCProbeType prbType, SafetyNTCProbe_t *prbNTC);
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */



/**
\n This function initializes the status of the wash/dry probe and
\n the state machine related to the safety of ntc module.

\param
\return

\author		Salvatore Fimiani
\date		12/02/2013
\version

\note

*/

static void PM_SFT_WM_NTCProbe_GeneralInit(sft_module_status_t *probeMod, eNTCProbeType prbType, SafetyNTCProbe_t *prbNTC)
{
	/* Pointer to the tab of the Probe Fault Parameters */
	struct Loader prbFltParTabPtr;
	bool_t tempFlag = FALSE;
	uint8_t tempDispl = 0xFF;
	uint8_t err = 0;


	if(prbType == wash)
	{
		coolDownCnt = 0; /* At the beginning it does not need to wait for the cool down */
		resOFFTimeoutCnt = 0;

		prbDoorLockPos = LIB_IOM_GetLoadPos((uint8_t) LD_DOOR_LOCK_POS);

		prbWashRes.pos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_RES_POS);
		tempDispl = DSPL_PROBEFLT_WASH_PARAM;
		tempFlag = (prbWashRes.pos == LOAD_NOT_FOUND) || (prbDoorLockPos == LOAD_NOT_FOUND);
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		prbDryRes.pos = LIB_IOM_GetLoadPos((uint8_t) LD_DRY_RES_POS);
		tempDispl = DSPL_PROBEFLT_DRY_PARAM;
	}
#endif /* CONFIG_APPLIANCE_WD */

	if (tempFlag)
	{
		err = BAD_PARAMETERS_SF;
	}

	SAFETY_REGION_START();

	prbNTC->fltRstTimer =  GEN_FLT_RESET_TIMER_VALUE;
	prbNTC->cnt = 0;
	prbNTC->startDelayCnt = BASETIME_1S_TO_100MS(PROBE_CH_TIMEOUT);
	prbNTC->prbType = prbType;

	/* Get a pointer to the Wash/Dry Probe Fault Parameters Tab */
	if ( 0 != BasicLoader(SF_PTR_FAULT, tempDispl, &prbFltParTabPtr) )
	{
		if(prbType == wash)
		{
			err = BAD_PARAMETERS_SF;
		}
#if defined (CONFIG_APPLIANCE_WD)
		/* This piece of code is useful to prevent of giving error if a SF for WM
		 * is set up on a WD structure. Indeed only if it is there the drying resistor the
		 * drying NTC is surely present */
		else if (prbDryRes.pos != LOAD_NOT_FOUND)
		{
			err = BAD_PARAMETERS_SF;
		}
#endif

	}
	else
	{
		/* Reads probe parameters from the Probe Fault Parameters Tab */
		prbNTC->genFltParams.prefaultTime = ( ((uint16_t) (*prbFltParTabPtr.Data)<<8) + (uint16_t)(*(prbFltParTabPtr.Data+1)));
		if ((*(prbFltParTabPtr.Data+2)& 0xF8) == 0x10)
		{
			prbNTC->genFltParams.faultTime =   (uint16_t)(*(prbFltParTabPtr.Data + DSPL_OFFSET_FAULT_TIME));
		}
		else
		{
			prbNTC->genFltParams.faultTime =  (uint16_t)(*(prbFltParTabPtr.Data + DSPL_OFFSET_FAULT_TIME)) * 10;
		}
		/* Conversion of the faultTime from seconds to multiples of 100ms */
		prbNTC->genFltParams.faultTime = (uint16_t)BASETIME_1S_TO_100MS(prbNTC->genFltParams.faultTime);
		prbNTC->genFltParams.checkTimeout = (uint16_t) BASETIME_10S_TO_100MS(*(prbFltParTabPtr.Data+DSPL_OFFSET_RESON_TIMEOUT));

		prbNTC->prbPos = (uint8_t)(*(prbFltParTabPtr.Data + DSPL_OFFSET_PROBE_POS));

		prbNTC->genFltParams.deltaCheck =  *(prbFltParTabPtr.Data+DSPL_OFFSET_RESON_DELTA);
		prbNTC->genFltParams.shrtFaultADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_SHORTED_FAULT_THR))) & ADC_VALUE_MASK;
		prbNTC->genFltParams.openFltADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OPEN_FAULT_THR))) & ADC_VALUE_MASK;
		prbNTC->genFltParams.minSafeADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_MIN_SAFE_THR))) & ADC_VALUE_MASK;
		prbNTC->genFltParams.maxSafeADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_MAX_SAFE_THR))) & ADC_VALUE_MASK;
		prbNTC->genFltParams.coolDownTimeout = (uint16_t)BASETIME_10S_TO_100MS(*(prbFltParTabPtr.Data+DSPL_OFFSET_COOLDOWN_TIMEOUT));
	}

	if(prbType == wash)
	{
		SAFETY_DATA_UPDATE(washNTCProbe);
		SAFETY_DATA_UPDATE(persWashNTCProbe);
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		SAFETY_DATA_UPDATE(dryNTCProbe);
		SAFETY_DATA_UPDATE(persDryNTCProbe);
	}
#endif
	SAFETY_REGION_END();


	if (err == BAD_PARAMETERS_SF)
	{
		probeMod->status = SFT_MODULE_FAULT;
		probeMod->faultSrc = FAULTSRC_NTCPROBE_BAD_SETTING;
	}
	else
	{
		probeMod->status = SFT_MODULE_IDLE;
		probeMod->faultSrc = SFT_NO_FAULT_SRC;
	}

}

/**
\n This function initializes the status of the wash probe and
\n the state machine related to the safety of wash ntc module.

\param
\return

\author		Salvatore Fimiani
\date		02/02/2013
\version    01.00.01
\n
\par	History:
\n
\n 		01.00.00
\n		12/02/2013
\n		Salvatore Fimiani
\n		changes
\n 			Creation


\n 		01.00.01
\n		07/05/2013
\n		Salvatore Fimiani
\n		changes
\n 			Eliminato il corpo della funzione e
\n			inserita la chiamata a funzione di inizializzazione per sonda generica

\note

*/

void PM_SFT_WM_washNTCProbe_Init(sft_module_status_t *probeMod)
{
	PM_SFT_WM_NTCProbe_GeneralInit(probeMod, wash, &washNTCProbe);
}



/**
\n This function simply call the other function PM_SFT_WM_NTCProbe_GeneralManager to specifically manage the Wash Probe
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] probeMod		Info related to the safety status of this module
\param [in] 	applReqFlags	Flags of requests by the application


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\n\li

\author		Salvatore Fimiani
\date		12/02/2013
\version	0.1



*/

void PM_SFT_WM_washNTCProbe_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(probeMod);
	PM_SFT_WM_NTCProbe_GeneralManager(newLoads,probeMod,applReqFlags, &washNTCProbe, &washProbeSample, &persWashNTCProbe);
	LIB_SFT_LeaveSafetyModule(probeMod);
}

uint16_t PM_SFT_WM_washNTCProbe_GetTime(sft_module_status_t *status)
{
	return washNTCProbe.cnt;
}

#if defined (CONFIG_APPLIANCE_WD)

void PM_SFT_WM_dryNTCProbe_Init(sft_module_status_t *probeMod)
{
	PM_SFT_WM_NTCProbe_GeneralInit(probeMod, dryer, &dryNTCProbe);
}


/**
\n This function implements the state machine which manages the safety of the Wash Probe
\n module.
\n It must be called within a task
\n
\param
\return

\author		Salvatore Fimiani
\date		12/02/2013
\version

\note

*/

void PM_SFT_WM_dryNTCProbe_Monitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(probeMod);
	if (prbDryRes.pos != LOAD_NOT_FOUND)
	{
		PM_SFT_WM_NTCProbe_GeneralManager(newLoads,probeMod,applReqFlags, &dryNTCProbe, &dryProbeSample, &persDryNTCProbe);
	}
	LIB_SFT_LeaveSafetyModule(probeMod);
}

uint16_t PM_SFT_WM_dryNTCProbe_GetTime(sft_module_status_t *status)
{
	return dryNTCProbe.cnt;
}
#endif


/**
\n This function gets the washing probe location in ram.

\param
\return 	uint8_t The position in ram of the value of the NTC probe

\author		Salvatore Fimiani
\date		12/02/2013
\version

\note

*/

uint8_t PM_SFT_WM_NTCProbe_GetWashProbePos(void)
{
	return washNTCProbe.prbPos;
}
/*****************
 * Local Functions
 ******************/

/**
\n This function implements the state machine which manages safety of the Probe
\n module.
\n
\n
\param[in,out] newLoads			Info related to loads status request
\param[in,out] probeMod			Info related to the safety status of this module
\param[in] 	   applReqFlags		Flags of requests by the application
\param[in,out] prbNTC			Safety parameters and counters
\param[in,out] prbSample		Analog values measured by NTC probe
\param[in,out] prbSftPers		Counters and flags to execute the test of blocked probe

\return void
\retval	N.A.

\par Architecture Requirements ID:
\li\ref		SARS_WMMT_MN_SNTC_2
\n\li	SARS_WMMT_MN_SNTC_3
\n\li	SARS_WMMT_MN_SNTC_4
\n\li	SARS_WMMT_MN_SNTC_5
\n\li	SARS_WMMT_MN_SNTC_6
\n\li	SARS_WMMT_MN_SNTC_7


\n\author	Salvatore Fimiani
\n\date		06/05/2013
\n\version	0.1
\n
\n
\n
*/

static void PM_SFT_WM_NTCProbe_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags, SafetyNTCProbe_t *prbNTC,
		SampleNTCProbe_t *prbSample, SafetyBkNTCProbe_t *prbSftPers)
{
	uint8_t prbDoorLockStatus;
	bool_t doReset = FALSE;

	prbDoorLockStatus = LoadsStatus[prbDoorLockPos];
	SAFETY_REGION_START();
	/* Gets the current state of the Wash/Dry Resistor and checks if there is the conditions
	* for requesting the controls of blocked Wash/Dry NTC probe */
	if(prbNTC->prbType == wash)
	{
		SAFETY_DATA_CHECK(washNTCProbe);
		SAFETY_DATA_CHECK(persWashNTCProbe);

		prbWashDoorRef.status = prbDoorLockStatus ;

		prbWashRes.status = LoadsStatus[prbWashRes.pos];
		doReset = PM_SFT_WM_BlockedProbeTriggerCheck(wash,prbWashDoorRef, prbSftPers, prbSample, TRUE);
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		SAFETY_DATA_CHECK(dryNTCProbe);
		SAFETY_DATA_CHECK(persDryNTCProbe);

		prbDryDoorRef.status = prbDoorLockStatus;

		prbDryRes.status = LoadsStatus[prbDryRes.pos];
		(void)PM_SFT_WM_BlockedProbeTriggerCheck(dryer,prbDryDoorRef, prbSftPers, prbSample, FALSE);
	}
#endif /* CONFIG_APPLIANCE_WD */




	/* Checks if the probe's analog channel is ready */
	PM_SFT_WM_CheckForProbeChReady(prbNTC);

	/* Starts controls only if the system is not just been turned on */
	if(prbNTC->startDelayCnt == 0)
	{
		prbSample->currVal = AnalogInput[prbNTC->prbPos];

		/* Detection of the alarm types is made only when the door is blocked */
		if(goodZCSignal() == TRUE)
		{
			zcNotGoodCnt = ZC_NOTGOOD_TIMEOUT;
			/* Shorted NTC probe trigger (beware that there is a pull-down resistor) */
			if (prbSample->currVal > prbNTC->genFltParams.shrtFaultADC_Thr)
			{
				prbNTC->currentAlertType = FAULTSRC_NTCPROBE_SHORTED;
			}

			/* Open NTC probe trigger (beware that there is a pull-down resistor) */
			else if (prbSample->currVal < prbNTC->genFltParams.openFltADC_Thr)
			{
				prbNTC->currentAlertType = FAULTSRC_NTCPROBE_OPEN;
			}

			/* Probe value in the correct range  */
			else /* if ((prbSample->currVal >= prbNTC->genFltParams.openFltADC_Thr)
						&& (prbSample->currVal <= prbNTC->genFltParams.shrtFaultADC_Thr)) */
			{
				/* Checks if a control for blocked probe has been request */
				if(prbSftPers->prbBlockedControlON == TRUE)
				{
					prbSample->maxVal = MAX(prbSample->currVal, prbSample->maxVal);
					prbSample->minVal = MIN(prbSample->currVal, prbSample->minVal);
					/* If true, the test is passed */
					if((prbSample->maxVal - prbSample->minVal) >= prbNTC->genFltParams.deltaCheck)
					{
						prbSftPers->isTestAlreadyExec = TRUE;
						prbSftPers->prbBlockedControlON = FALSE; 	/* Stop the check */
					}
					else
					{
						if (prbSftPers->prbBlockedControlCnt >= prbNTC->genFltParams.checkTimeout)
						{
							prbNTC->currentAlertType = FAULTSRC_NTCPROBE_BLOCKED;
							prbSftPers->prbIsBlocked = TRUE;
							prbSftPers->prbBlockedControlON = FALSE; /* Stop the check */
							prbSftPers->isTestAlreadyExec = TRUE;
						}

					}
					/* The timer runs only if the wash/dry resistor is ON */
					if(prbNTC->prbType == wash)
					{
						if(prbWashRes.status == ON)
						{
							prbSftPers->prbBlockedControlCnt++;
						}
					}
	#if defined (CONFIG_APPLIANCE_WD)
					else
					{
						if(prbDryRes.status == ON)
						{
							prbSftPers->prbBlockedControlCnt++;
						}
					}
	#endif /* CONFIG_APPLIANCE_WD */

					if(prbSftPers->prbBlockedControlON == FALSE)
					{
						prbSftPers->prbBlockedControlCnt = 0; /* Reset the timer */
					}
				} /* End of the test for blocked NTC probe */
				else
				{
					/* NOTHING TO DO */
				}

				/* If everything is ok, be sure that the probe's value is below a max safety threshold */
				if (prbNTC->currentAlertType != FAULTSRC_NTCPROBE_BLOCKED)
				{
					if(prbSample->currVal > prbNTC->genFltParams.maxSafeADC_Thr)
					{
						prbNTC->currentAlertType = FAULTSRC_NTCPROBE_HIGHTEMP;
					}
					else if(prbSample->currVal < prbNTC->genFltParams.minSafeADC_Thr)
					{
						/* Till now everything is ok so reset fault source */
						prbNTC->currentAlertType = FAULTSRC_NTCPROBE_NOFAULT;
					}
				}
			}

			/* This code is required to restore from backup the eventual protection status for blocked probe */
			if(prbSftPers->protFrom == FAULTSRC_NTCPROBE_BLOCKED)
			{
				probeMod->status = SFT_MODULE_PROTECTION;
			}
		}
		else /* If the door is now open */
		{
			if(zcNotGoodCnt == 0)
			{
				/* When the door is open, if the module status is not equals to FAULT, all alarms are reset */
				if(probeMod->status != SFT_MODULE_FAULT)
				{
					prbNTC->currentAlertType = FAULTSRC_NTCPROBE_NOFAULT;
					probeMod->faultSrc = SFT_NO_FAULT_SRC;
					probeMod->status = SFT_MODULE_IDLE;
				}
			}
			else
			{
				zcNotGoodCnt--;
			}

		}

		/* State machine for the probe's faults */
		switch (probeMod->status)
		{
			case SFT_MODULE_RESET:
				prbNTC->currentAlertType = FAULTSRC_NTCPROBE_NOFAULT;
				probeMod->faultSrc = SFT_NO_FAULT_SRC;
				probeMod->status = SFT_MODULE_IDLE;
				break;

			case SFT_MODULE_IDLE:
				if(prbNTC->currentAlertType != FAULTSRC_NTCPROBE_NOFAULT)
				{
					/* Set the timer with timeout to prefault */
					if(prbNTC->currentAlertType == FAULTSRC_NTCPROBE_BLOCKED )
					{

						prbSftPers->protFrom = FAULTSRC_NTCPROBE_BLOCKED;
						probeMod->status = SFT_MODULE_PROTECTION;

					}
					else if ((prbNTC->currentAlertType == FAULTSRC_NTCPROBE_SHORTED) ||
							(prbNTC->currentAlertType == FAULTSRC_NTCPROBE_OPEN ))
					{
						/* Set the timer with the timeout to prefault */
						if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
						{
							/* Only if in auto test mode */
							prbNTC->cnt = SFT_TEST_PREFAULT_TIMEOUT_100MS;
						}
						else
						{
							/* Only in normal mode */
							prbNTC->cnt = prbNTC->genFltParams.prefaultTime;
						}

						probeMod->status = SFT_MODULE_WARNING;
					}
					if(prbNTC->currentAlertType == FAULTSRC_NTCPROBE_HIGHTEMP)
					{
						if(prbNTC->prbType == wash)
						{
							/* This is the time interval during which the washing resistor must be kept turned ON in order to detect any problems
							 * on the feedback  */
							prbNTC->cnt = PM_SFT_WM_washRes_GetFltTimeCAOnHighTemp(applReqFlags & SFT_FLAGS_TEST_TMR_REQ) + KEEP_RESON_TIME;
						}
						probeMod->status = SFT_MODULE_PROTECTION;
					}

				}
				prbSftPers->protFrom = prbNTC->currentAlertType;
				probeMod->faultSrc = prbNTC->currentAlertType;
				/* If on the previous door's unlocking the NTC probe was being blocked, when the door gets locked again
				 * the system instantaneously goes to the protection state */
				if((prbSftPers->prbIsBlocked == TRUE) && (prbDoorLockStatus == LOAD_ON))
				{
					prbSftPers->protFrom = FAULTSRC_NTCPROBE_BLOCKED;
					probeMod->faultSrc = FAULTSRC_NTCPROBE_BLOCKED;
					probeMod->status = SFT_MODULE_PROTECTION;
				}

				break;

			case SFT_MODULE_MONITORING:
			case SFT_MODULE_WARNING:
				/* Reset status if the alert source has changed since the last cycle */
				if(prbNTC->currentAlertType != prbNTC->lastAlertType)
				{
					probeMod->status = SFT_MODULE_IDLE;
					break;
				}

				if((prbNTC->currentAlertType == FAULTSRC_NTCPROBE_SHORTED) ||
						(prbNTC->currentAlertType == FAULTSRC_NTCPROBE_OPEN ))
				{
					if(prbNTC->cnt == 0) /* Timer expired */
					{
						/* Set the timer with timeout to fault */
						if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
						{
							/* Only if in auto test mode */
							prbNTC->cnt = SFT_TEST_FAULT_TIMEOUT_100MS;
						}
						else
						{
							/* Only in normal mode */
							prbNTC->cnt = prbNTC->genFltParams.faultTime;
						}
						probeMod->status = SFT_MODULE_PREFAULT;
					}
				}
				prbNTC->fltRstTimer = GEN_FLT_RESET_TIMER_VALUE;
				break;

			case SFT_MODULE_PROTECTION:
				/* The wash/dry resistor is kept OFF */
				if(prbNTC->prbType == wash)
				{
					if(prbSftPers->protFrom == FAULTSRC_NTCPROBE_HIGHTEMP)
					{
						if(prbNTC->cnt > 0)
						{
							newLoads->loads_ovr[prbWashRes.pos] = LOAD_REG_ON;
						}
						else
						{
							newLoads->loads_ovr[prbWashRes.pos] = LOAD_REG_OFF;
						}
					}
					else
					{
						newLoads->loads_ovr[prbWashRes.pos] = LOAD_REG_OFF;
					}

				}
			#if defined (CONFIG_APPLIANCE_WD)
				else
				{
					newLoads->loads_ovr[prbDryRes.pos] = LOAD_REG_OFF;

				}
			#endif /* CONFIG_APPLIANCE_WD */
				/* If the probe is blocked, it must be assured a protection till to the door's unlock  */
				if(prbSftPers->protFrom == FAULTSRC_NTCPROBE_BLOCKED)
				{
					if(prbDoorLockStatus == OFF)
					{
						prbSftPers->protFrom = FAULTSRC_NTCPROBE_NOFAULT;
						probeMod->status = SFT_MODULE_RESET;
					}
				}
				else
				{
					if(prbNTC->currentAlertType != prbSftPers->protFrom )
					{
						if(prbNTC->prbType == wash)
						{
							if(prbNTC->currentAlertType == FAULTSRC_NTCPROBE_SHORTED)
							{
								prbNTC->cnt = prbNTC->genFltParams.prefaultTime;
								prbSftPers->protFrom = FAULTSRC_NTCPROBE_SHORTED;
								probeMod->faultSrc = FAULTSRC_NTCPROBE_SHORTED;
								probeMod->status = SFT_MODULE_PREFAULT;
							}
							else
							{
								probeMod->status = SFT_MODULE_IDLE;
							}
						}
						else
						{
							probeMod->status = SFT_MODULE_IDLE;
						}

					}
				}
				break;

			case SFT_MODULE_PREFAULT:
				/* Reset status if the alert source has changed since the last cycle */
				if(prbNTC->currentAlertType != prbSftPers->protFrom)
				{
					if(prbNTC->fltRstTimer == 0)
					{
						probeMod->status = SFT_MODULE_IDLE;
					}
					else
					{
						prbNTC->fltRstTimer--;
					}


				}
				else
				{
					prbNTC->fltRstTimer = GEN_FLT_RESET_TIMER_VALUE;
					if(prbNTC->cnt == 0) /* Timer expired */
					{
						probeMod->status = SFT_MODULE_FAULT;
					}
				}

				break;

			case SFT_MODULE_FAULT:
				/* Handle reset fault request */
				if (applReqFlags & SFT_FLAGS_RESET_REQ)
				{
					probeMod->status = SFT_MODULE_RESET;
				}

				break;

			default:
				break;
		}

		/* Internal Timer */
		if(prbNTC->cnt > 0)
		{
			prbNTC->cnt--;
		}

	}/* End if(prbNTC->startDelayCnt == 0) */


	if(prbNTC->prbType == wash)
	{
		prbWashDoorRef.statusOld = prbWashDoorRef.status;
		prbWashRes.statusOld = prbWashRes.status;
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		prbDryDoorRef.statusOld = prbDryDoorRef.status;
		prbDryRes.statusOld = prbDryRes.status;
	}
#endif /* CONFIG_APPLIANCE_WD */

	prbNTC->lastAlertType = prbNTC->currentAlertType;


	if(prbNTC->prbType == wash)
	{
		SAFETY_DATA_UPDATE(washNTCProbe);
		SAFETY_DATA_UPDATE(persWashNTCProbe);
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		SAFETY_DATA_UPDATE(dryNTCProbe);
		SAFETY_DATA_UPDATE(persDryNTCProbe);
	}
#endif
	SAFETY_REGION_END();

#if defined(CONFIG_LIB_CLASS_B_WDTEST)
	if (doReset)
	{
		/* This avoid data care failure */
		LIB_SFT_LeaveSafetyModule(probeMod);
		LIB_Reset_ForceReset();
	}
#endif
}



/**
\n This function checks if the analog channel for the NTC probe is ready to be read
\n
\n
\param
\return

\author		Salvatore Fimiani
\date		26/02/2013
\version

\note

*/

static void PM_SFT_WM_CheckForProbeChReady(SafetyNTCProbe_t *prbNTC)
{
	if(LIB_ADC_IsAnalogReady(prbNTC->prbPos)== TRUE)
	{
		prbNTC->startDelayCnt = 0;
	}
	else if(prbNTC->startDelayCnt > 0)
	{
		prbNTC->startDelayCnt--;
	}
}


/**
\n This function checks if the wash\dry resistor has been turned on the first time after
\n the door lock was closed
\n
\param
\return

\author		Salvatore Fimiani
\date		12/02/2013
\version

\note

*/


static bool_t PM_SFT_WM_BlockedProbeTriggerCheck(eNTCProbeType prbType, LoadInfos_t doorLockRef, SafetyBkNTCProbe_t *prbSftPers,
		SampleNTCProbe_t *prbSample, bool_t isCntEnabled)
{
	bool_t retValue = FALSE;

	/* The door is closing now */
	if( (doorLockRef.status == LOAD_ON) && (doorLockRef.statusOld == LOAD_OFF))
	{
		/* At door's closure the test for blocked NTC probe is enabled if the cool down timeout has expired */
		if ((coolDownCnt == 0) && (prbSftPers->isTestAlreadyExec ==  FALSE))
		{
			prbSftPers->prbBlockedControlON = TRUE; /* Enable the Blocked Probe Check */
			prbSftPers->prbBlockedControlCnt = 0; 	/* Reset the timer for the blocked probe check */
		}
	}
	/* The door is opening now */
	else if( (doorLockRef.status == LOAD_OFF) && (doorLockRef.statusOld == LOAD_ON) )
	{
		coolDownCnt = washNTCProbe.genFltParams.coolDownTimeout;
	}
	else if (doorLockRef.status == LOAD_OFF)/* The door was already open */
	{

		prbSftPers->prbBlockedControlCnt = 0; /* Reset the timer for the blocked probe check */
		/* If the test for blocked probe is in progress, it has to be reset only if the cooldown timeout at door
		 * opening has expired */
		if(coolDownCnt == 1)
		{
			prbSftPers->isTestAlreadyExec = FALSE;
			prbSftPers->prbBlockedControlON = FALSE;
			prbSample->maxVal = 0;
			prbSample->minVal = 0xFFFF;
			/* If the probe was blocked, the system does not forget it till the end of the cycle even if
			* the door gets unlocked */
			prbSftPers->prbIsBlocked = FALSE;
		}

		if(prbType == dryer)
		{
			if((prbSftPers->isTestAlreadyExec == TRUE)
					&&(prbSftPers->prbIsBlocked == TRUE))
			{
				prbSftPers->isTestAlreadyExec = FALSE;
				prbSftPers->prbBlockedControlON = TRUE;
				prbSample->maxVal = 0;
				prbSample->minVal = 0xFFFF;
				prbSftPers->prbIsBlocked = FALSE;
			}
		}

		if((isCntEnabled == TRUE) && (coolDownCnt > 0))
		{
			coolDownCnt--;
			if(coolDownCnt == 0)
			{
				retValue = TRUE;
			}
		}



	}



	return retValue;

}


/**
*\n This function checks if the wash\dry probe has detected an high temperature protection
*\n
*\n
*\param		[in] probe: is the ID of the NTC probe [wash/dryer]
*\return	TRUE/FALSE

*\author		Salvatore Fimiani
*\date			03/03/2014
*\version
*\version
*
 **/
bool_t PM_SFT_WM_NTCProbe_isHighTemp(eNTCProbeType probe)
{
	bool_t retVal = FALSE;

	if(probe == wash)
	{
		retVal = (persWashNTCProbe.protFrom == FAULTSRC_NTCPROBE_HIGHTEMP)?TRUE:FALSE;
	}
#if defined (CONFIG_APPLIANCE_WD)
	else
	{
		retVal = (persDryNTCProbe.protFrom == FAULTSRC_NTCPROBE_HIGHTEMP)?TRUE:FALSE;
	}
#endif /* CONFIG_APPLIANCE_WD */

	return retVal;
}



