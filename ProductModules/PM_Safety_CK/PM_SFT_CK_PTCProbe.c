/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/



/*
 * PM_SafetyPTCProbe.c
 *
 *  Created on: 04/feb/2013
 *  Author: consul.fimiani
 */

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SFT_CK_PTCProbe.h"
#include "LIB_Utilities.h"
#include "LoadsRequestExp.h"
#include "LIB_ClassBVar.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define DSPL_PROBEFLT_OVEN_GEN_TIMEOUT			0x01u	/* Probe Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_OVEN_GEN_PREF_TIME				0
#define DSPL_OFFSET_OVEN_GEN_FAULT_TIME				2

#define DSPL_RELLOAD_OVEN						0x10u	/* Probe Related Loads displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_OVEN_LOAD_BITMAP_TOP			0
#define DSPL_OFFSET_OVEN_LOAD_BITMAP_BTM			2

#define DSPL_PROBEFLT_OVEN_BOTTOM_PARAM			0x07u	/* Probe Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_PROBEFLT_OVEN_TOP_PARAM			0x08u	/* Probe Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_OVEN_TEST_TIMEOUT			1		/* Offset for Test Timeout in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_TOHIGH_PREF_TIME		6		/* Offset for Too High Prefault Time in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_TOHIGH_FAULT_TIME		8		/* Offset for Too High Fault Time in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_SHORTED_FAULT_THR		14		/* Offset for Shorted Probe Threshold in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_OPEN_FAULT_THR			12		/* Offset for Open Probe Threshold in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_MIN_SAFE_THR			16		/* Offset for Min Safe Threshold in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_MAX_SAFE_THR			18		/* Offset for Max Safe Threshold in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_DELTA_FOR_TEST			20		/* Offset for Delta Temperature in the Probe Fault Parameters Table */
#define DSPL_OFFSET_OVEN_PROBE_POS				21
#define DSPL_OFFSET_OVEN_MAX_OVER_TEMP_CNT		22
#define DSPL_OFFSET_OVEN_FLTR_TIMEOUT			24



#define DSPL_OFFSET_OUTMAP	0x03

#define POSMASK_RES_SOLE	0x01		/* Position of the resistor Sole in the loads bitmap 		*/
#define POSMASK_RES_CIRC	0x02		/* Position of the resistor Circular in the loads bitmap 	*/
#define POSMASK_RES_GRL1	0x04		/* Position of the resistor Grill1 in the loads bitmap 		*/
#define POSMASK_RES_GRL2	0x08		/* Position of the resistor Grill2 in the loads bitmap 		*/
#define POSMASK_RES_CEIL	0x10		/* Position of the resistor Ceiling in the loads bitmap 	*/
#define POSMASK_FAN_CIRC	0x20		/* Position of the fan Circular in the loads bitmap 		*/


#define PTC_ERR_PARAMS_BAD_SF				0x02
#define PTC_ERR_PARAMS_OK					0x00
#define PTC_ERR_PARAMS_POS_NOT_DEFINED		0x01
#define PTC_ERR_PROBE_ID_NOT_VALID			0x10



#define PTC_CYCLE_UNDEF 	0xFF

#define ADC_VALUE_MASK  0x0FFF
#define LOW_NIBBLE		0x0F



#define PROBE_CH_TIMEOUT	2 	/* Seconds */
#define TEST_DELAY_TIMEOUT	4	/* Seconds */

#define COOLDOWN_TIMER_ON		1
#define COOLDOWN_TIMER_OFF		0


#define CKPTC_NUM_OF_LOADS		6


/* Defines related to GI MAP */

#define EEP_GIMAP_TABPTR_OFFSET 		0x01 	/* GIMAP displacement vs PTR_IO_CONFIG_DATA */
#define GIMAP_TABLE_ITEM_SIZE_IN_BYTES 	0x04 	/* Size in bytes of a single item inside the G.I. MAP table */


#define GI_BOTTOM_PROBE_ID				0x00
#define BASICLOADER_ERR					0xFF


#define GI_READ_ID(pByte)				*(pByte+1)
#define GI_READ_POSITION(pByte)			*(pByte+3)

/* End of defines related to GI MAP */

#define APP_READY_TIMEOUT 	50


/* *******************************************************************************
 * The macros below are for debug's tests. To be commented for normal operations
 * *******************************************************************************/

/* #define DEBUG_WITHOUT_EDITOR

 #define DEBUG_DISABLE_PROBE
 #define DEBUG_DISABLE_DCARE_CHECK
 #define DEBUG_WITHOUT_LOADS
 #define DEBUG_WITHOUT_APP
*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

/**
 * Control var related to the Top PTC Probe
 */
SafetyPTCProbe_t topPTCProbe;

/**
 * Control var related to the Bottom PTC Probe
 */
SafetyPTCProbe_t bottomPTCProbe;

/**
 * Test control var related to the Top PTC Probe
 */
 PTC_SftCntrlData_t persTopPTCProbe  = {PTC_CYCLE_UNDEF,PTC_CYCLE_UNDEF,PTC_CYCLE_UNDEF,
		 	 	 	 	 	 	 	 	 FAULTSRC_PTCPROBE_NOFAULT,FALSE, PTC_TEST_STOPPED, FALSE,0,
		 	 	 	 	 	 	 	 	 SFT_MODULE_IDLE,SFT_MODULE_IDLE,0,0};

/**
 * Test control var related to the Bottom PTC Probe
 */
 PTC_SftCntrlData_t persBottomPTCProbe  = {PTC_CYCLE_UNDEF,PTC_CYCLE_UNDEF,PTC_CYCLE_UNDEF,
	 	 	 	 	 	 	 	 	 	 	 FAULTSRC_PTCPROBE_NOFAULT,FALSE, PTC_TEST_STOPPED, FALSE,0,
	 	 	 	 	 	 	 	 	 	 	 SFT_MODULE_IDLE,SFT_MODULE_IDLE,0,0};

/**
 * Values' var acquired from the Top PTC Probe
 */
static SamplePTCProbe_t topProbeSample = {0,0,0xFFFF,0};

/**
 * Values' var acquired from the Bottom PTC Probe
 */
SamplePTCProbe_t bottomProbeSample = {0,0,0xFFFF,0};

static LoadInfos_t prbMasterReleRes; 	/* Info about the Resistors' Master Relé  load 	 */

/**
 * Table of loads to be registered with the PTC probes
 */
static LoadInfos_t prbLoad[CKPTC_NUM_OF_LOADS];


static uint8_t prbBtmIsPresent = FALSE;

static uint16_t limitFaultHighTempCnt;	/* FB DEBUG */




static sft_module_status_t *pSftPTC_Top, *pSftPTC_Bottom;


/**
 *
 */
typedef struct
{
	 uint8_t bufMain;
	 uint8_t bufTop;
	 uint8_t bufXL;
} BuffVsApp_t;

static BuffVsApp_t prbBuffVsApp = {PTC_CYCLE_UNDEF, PTC_CYCLE_UNDEF, PTC_CYCLE_UNDEF};


#if defined(DEBUG_WITHOUT_LOADS)
uint8_t LoadStatusEmulate[N_MAXLOADS];
#endif


/* ******************* */
/* Functions Prototype */
/* ******************* */



/* Local Function */
static void PM_SFT_CK_CheckForProbeChReady(SafetyPTCProbe_t *prbPTC);
static void PM_SFT_CK_PTCProbe_GeneralInit(sft_module_status_t *probeMod, SafetyPTCProbe_t *prbPTC, ePTCProbeType ePrbType);
static void PM_SFT_CK_PTCProbe_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags,
		SafetyPTCProbe_t *prbPTC, SamplePTCProbe_t *prbSample, PTC_SftCntrlData_t *prbSftPers);
static uint8_t PM_SFT_CK_PTCProbe_InitFltParams(SafetyPTCProbe_t *pPTCProbe,ePTCProbeType ePrbType);
static void PM_SFT_CK_BlockedProbeTriggerCheck(sft_module_status_t *probeMod,
		SafetyPTCProbe_t *prbPTC, PTC_SftCntrlData_t *prbSftPers, SamplePTCProbe_t *prbSample);

static uint8_t PM_SFT_CK_PTCProbe_isGenInputPres(uint8_t inputID);
static uint8_t PM_SFT_CK_PTCProbe_InitGenericFltParams(void);
static uint8_t PM_SFT_CK_PTCProbe_ConfigRelatedLoads(ePTCProbeType prbType);

static uint8_t PM_SFT_CK_PTCProbe_GetCycleStatus(BuffVsApp_t bufValue, ePTCProbeType prbType);


/* Exported function */


/**
 *\n This function calls the PM_SFT_CK_PTCProbe_GeneralInit function to initialize
 *\n the control variables related to the Top PTC probe
 *\n
 *\param 	[in,out]  *probeMod:	pointer to the safety info of the module
 *
 */
void PM_SFT_CK_PTCProbe_TopInit(sft_module_status_t *probeMod)
{
	PM_SFT_CK_PTCProbe_GeneralInit(probeMod, &topPTCProbe, prbTOP);
}

/**
 *\n This function calls the PM_SFT_CK_PTCProbe_GeneralInit function to initialize
 *\n the control variables related to the Bottom PTC probe.
 *\n This is done only after a test to check if the probe is present
 *\n
 *\param 	[in,out]  *probeMod:	pointer to the safety info of the module
 *
 */
void PM_SFT_CK_PTCProbe_BottomInit(sft_module_status_t *probeMod)
{
	/* Checks on run-time if the bottom probe is present */
	prbBtmIsPresent = PM_SFT_CK_PTCProbe_isGenInputPres((uint8_t)GI_BOTTOM_PROBE_ID);
	if(prbBtmIsPresent == TRUE)
	{
		PM_SFT_CK_PTCProbe_GeneralInit(probeMod, &bottomPTCProbe, prbBOTTOM);
	}

}



/**
\n This function initializes the status of the top/bottom probe and
\n the state machine related to the safety of PTC module.

\param
\return

\author		Salvatore Fimiani
\date		04/06/2013
\version

\note

*/
static void PM_SFT_CK_PTCProbe_GeneralInit(sft_module_status_t *probeMod, SafetyPTCProbe_t *pPTCProbe, ePTCProbeType ePrbType)
{
	uint8_t err = PTC_ERR_PARAMS_OK;
	static uint8_t isArrayInit = 0;
	if(isArrayInit == 0)
	{
		memset(prbLoad,0x00,CKPTC_NUM_OF_LOADS*sizeof(prbLoad[0]));
		isArrayInit = 1;
	}


	prbMasterReleRes.pos = LIB_IOM_GetLoadPos((uint8_t) LD_MASTER_RELAY_RES_POS);

	SAFETY_REGION_START();

	/* Gets Safety Parameters for the PTC Probe and configures loads related to the probe */

	err = PM_SFT_CK_PTCProbe_InitFltParams(pPTCProbe,ePrbType);
#if defined(DEBUG_WITHOUT_EDITOR)
	(void)PM_SFT_CK_PTCProbe_ConfigRelatedLoads(ePrbType);
#else
	err |= PM_SFT_CK_PTCProbe_ConfigRelatedLoads(ePrbType);
#endif


	pPTCProbe->startDelayCnt = BASETIME_1S_TO_100MS(PROBE_CH_TIMEOUT);
	pPTCProbe->fltRstTimer =  FLT_RESET_TIMEOUT_VALUE;
	pPTCProbe->cnt = 0;
	pPTCProbe->testDelayCnt = BASETIME_1S_TO_100MS(TEST_DELAY_TIMEOUT);

	if(pPTCProbe->prbType == prbTOP)
	{
		SAFETY_DATA_UPDATE(topPTCProbe);
		SAFETY_DATA_UPDATE(persTopPTCProbe);
	}
	else
	{
		SAFETY_DATA_UPDATE(bottomPTCProbe);
		SAFETY_DATA_UPDATE(persBottomPTCProbe);
	}

	SAFETY_REGION_END();


	if (err != PTC_ERR_PARAMS_OK)
	{
		probeMod->status = SFT_MODULE_FAULT;
		probeMod->faultSrc = FAULTSRC_PTCPROBE_BAD_SETTING;
	}
	else
	{
		probeMod->status = SFT_MODULE_IDLE;
		probeMod->faultSrc = SFT_NO_FAULT_SRC;
	}

}


/**
 * This function is the manager for the safety of the Top PTC probe
 *\n
 *\param  [out]	newLoads: pointer to the loads request
 *\param  [out] probeMod: pointer to the Module Status
 *\param  [in]  applReqFlags:	request flags from application
 */
void PM_SFT_CK_PTCProbe_TopMonitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(probeMod);
	PM_SFT_CK_PTCProbe_GeneralManager(newLoads,probeMod,applReqFlags, &topPTCProbe, &topProbeSample, &persTopPTCProbe);
	pSftPTC_Top = probeMod;
	LIB_SFT_LeaveSafetyModule(probeMod);
}




/**
 * This function is the manager for the safety of the Bottom PTC probe
 *\n
 *\param  [out]	newLoads: 		pointer to the loads request
 *\param  [out] probeMod: 		pointer to the Module Status
 *\param  [in]  applReqFlags:	request flags from application
 */
void PM_SFT_CK_PTCProbe_BottomMonitor(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(probeMod);
	if(prbBtmIsPresent == TRUE)
	{
		PM_SFT_CK_PTCProbe_GeneralManager(newLoads,probeMod,applReqFlags, &bottomPTCProbe, &bottomProbeSample, &persBottomPTCProbe);
		pSftPTC_Bottom = probeMod;
	}
	LIB_SFT_LeaveSafetyModule(probeMod);
}



/**
\n This function implements the state machine which manages the safety of the Probe
\n module.
\n
\n
\param	[in,out]	newLoads: 		pointer to the loads request
\param	[in,out]	probeMod: 		pointer to the Module Status
\param	[in]		applReqFlags:	request flags from application
\param	[in,out]	prbPTC:			pointer to the control var of the PTC probe
\param	[in,out]	prbSample:		pointer to the var with the values acquired on the PTC probe
\param	[in,out]	prbSftPers:		pointer to the control var related to the test for locked probe


\author		Salvatore Fimiani
\date		04/06/2013
\version

*/
static void PM_SFT_CK_PTCProbe_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *probeMod, uint8_t applReqFlags,
		SafetyPTCProbe_t *prbPTC, SamplePTCProbe_t *prbSample, PTC_SftCntrlData_t *prbSftPers)
{

	uint8_t i;

	SAFETY_REGION_START();

#if !defined(DEBUG_DISABLE_DCARE_CHECK)
	if(prbPTC->prbType == prbTOP)
	{
		SAFETY_DATA_CHECK(topPTCProbe);
		SAFETY_DATA_CHECK(persTopPTCProbe);
	}
	else
	{
		SAFETY_DATA_CHECK(bottomPTCProbe);
		SAFETY_DATA_CHECK(persBottomPTCProbe);
	}
#endif


	/* Updates the current state of the Master Relé only during the cycle for the TOP probe */
	if(prbPTC->prbType == prbTOP )
	{
#if defined(DEBUG_WITHOUT_LOADS)
		prbMasterReleRes.currStatus = LoadStatusEmulate[prbMasterReleRes.pos];
#else
		prbMasterReleRes.currStatus = LoadsStatus[prbMasterReleRes.pos];
#endif
	}
	else
	{ /* This statement is necessary because the fltTimeout parameter is present only on the data faults table
	 	 of the TOP probe. This avoid you from imposing to call the initialization function for the TOP probe before
	 	 than the initialization function of the BOTTOM probe, when present */
		prbPTC->fltParams.fltTimeout = topPTCProbe.fltParams.fltTimeout;
	}

#if !defined(DEBUG_WITHOUT_APP)
	prbSftPers->currCycleStatus = PM_SFT_CK_PTCProbe_GetCycleStatus(prbBuffVsApp, prbPTC->prbType);
#endif /* DEBUG_WITHOUT_APP */

	/* Reads the status of all loads related to the specific probe type */
	for(i = 0; i < CKPTC_NUM_OF_LOADS; i++)
	{
		if(prbLoad[i].probeID == prbPTC->prbType)
		{
#if defined(DEBUG_WITHOUT_LOADS)
			prbLoad[i].currStatus = LoadStatusEmulate[prbLoad[i].pos];
#else
			prbLoad[i].currStatus = LoadsStatus[prbLoad[i].pos];
#endif
		}

	}


	/* Checks if the probe's analog channel is ready */
	PM_SFT_CK_CheckForProbeChReady(prbPTC);

	/* Starts controls only if the system is not just been turned on */
	if(prbPTC->startDelayCnt == 0)
	{

		/* Checks if there are conditions for requesting controls on blocked PTC probe */
		PM_SFT_CK_BlockedProbeTriggerCheck(probeMod, prbPTC, prbSftPers, prbSample);

#if !defined(DEBUG_DISABLE_PROBE)
		prbSample->currVal = AnalogInput[prbPTC->prbPos];
		/* At the Power ON it starts to acquires Max and Min only after a 10*TEST_DELAY_TIMEOUT delay */
		if(prbPTC->testDelayCnt  == 0)
		{
			prbSample->maxVal = MAX(prbSample->currVal, prbSample->maxVal);
			prbSample->minVal = MIN(prbSample->currVal, prbSample->minVal);
		}

#endif
		if(prbPTC->currentAlertType != FAULTSRC_PTCPROBE_MISMATCH)
		{
			/* Shorted PTC probe trigger  */
			if (prbSample->currVal < prbPTC->fltParams.shrtFaultADC_Thr)
			{
				prbPTC->currentAlertType = FAULTSRC_PTCPROBE_SHORTED;
			}
			/* Open PTC probe trigger */
			else if (prbSample->currVal > prbPTC->fltParams.openFltADC_Thr)
			{
				prbPTC->currentAlertType = FAULTSRC_PTCPROBE_OPEN;
			}
			/* Probe value in the correct range  */
			else
			{
				/* Checks if a test for blocked probe is in progress */
				if((prbSftPers->prbTestResult == PTC_TEST_IN_PROGR ) && (prbPTC->testDelayCnt == 0))
				{

					/* If true, the test is passed */
					if((prbSample->maxVal - prbSample->minVal) >= prbPTC->fltParams.deltaCheck)
					{
						prbPTC->prbBlockedTestCnt = 0; 		/* Reset the timer */
						if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_BLOCKED)
						{
							prbPTC->currentAlertType = FAULTSRC_PTCPROBE_NOFAULT;
						}
						prbSftPers->prbTestResult = PTC_TEST_OK;
					}
					else
					{
						if (prbPTC->prbBlockedTestCnt == 0)
						{
							prbSftPers->prbTestResult = PTC_TEST_FAILED;
							prbPTC->currentAlertType = FAULTSRC_PTCPROBE_BLOCKED;
						}
						else
						{
							if(prbPTC->prbBlockedTestCnt > 0)
							{
								prbPTC->prbBlockedTestCnt--;
							}

						}
					}
				}

				/* If everything is ok, be sure that the probe's value is below a max safety threshold */
				if (prbPTC->currentAlertType != FAULTSRC_PTCPROBE_BLOCKED)
				{
					if(prbSample->currVal > prbPTC->fltParams.minSafeADC_Thr)
					{
						prbPTC->currentAlertType = FAULTSRC_PTCPROBE_HIGHTEMP;
					}
					else if(prbSample->currVal < prbPTC->fltParams.maxSafeADC_Thr)
					{
						/* Till now everything is ok so reset fault source */
						prbPTC->currentAlertType = FAULTSRC_PTCPROBE_NOFAULT;
					}
				}
			}
		} /* FAULTSRC_PTCPROBE_MISMATCH */
		/* State machine for the probe's faults */
		switch (probeMod->status)
		{
			case SFT_MODULE_RESET:
				prbPTC->currentAlertType = FAULTSRC_PTCPROBE_NOFAULT;
				prbSftPers->protFrom = FAULTSRC_PTCPROBE_NOFAULT;
				prbSftPers->wasInFault = FALSE;
				probeMod->faultSrc = SFT_NO_FAULT_SRC;
				probeMod->status = SFT_MODULE_IDLE;
				break;

			case SFT_MODULE_IDLE:
				if(prbPTC->currentAlertType != FAULTSRC_PTCPROBE_NOFAULT)
				{
					probeMod->faultSrc = prbPTC->currentAlertType;
					probeMod->status = SFT_MODULE_MONITORING;
				}
				else if((prbSftPers->prbTestResult == PTC_TEST_FAILED) &&
						(prbSftPers->currCycleStatus == PTC_CYCLE_RUN))
				{
					prbSftPers->prbTestResult = PTC_TEST_IN_PROGR;
				}
				break;

			case SFT_MODULE_MONITORING:
				if(prbPTC->currentAlertType != FAULTSRC_PTCPROBE_NOFAULT)
				{
					if(prbPTC->currentAlertType != FAULTSRC_PTCPROBE_HIGHTEMP)
					{
						if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_MISMATCH)
						{
							probeMod->status = SFT_MODULE_FAULT;
						}
						else
						{
							/* Set the timer with timeout to prefault */
							if(applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
							{
								/* Only if in auto test mode */
								prbPTC->cnt = SFT_TEST_PREFAULT_TIMEOUT_100MS;
							}
							else
							{
								/* Only in normal mode */
								prbPTC->cnt = genFltParams.prefTime;
							}
							probeMod->status = SFT_MODULE_WARNING;
						}
					}
					else /* Only in the case of high temperature go into protection status */
					{
						probeMod->status = SFT_MODULE_PROTECTION;
					}
					prbSftPers->protFrom = prbPTC->currentAlertType;
					probeMod->faultSrc = prbPTC->currentAlertType;
				}
				else
				{
					probeMod->status = SFT_MODULE_RESET;
				}
				break;

			case SFT_MODULE_WARNING:
				/* Reset status if the alert source has changed since the last cycle */
				if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_NOFAULT)
				{
					probeMod->status = SFT_MODULE_RESET;
				}
				else
				{
					if(prbPTC->currentAlertType != prbSftPers->protFrom)
					{
						probeMod->status = SFT_MODULE_MONITORING;
					}
					else
					{
						if(prbPTC->cnt == 0) /* Timer expired */
						{
							/* Set the timer with timeout to fault */
							if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
							{
								/* Only if in auto test mode */
								prbPTC->cnt = SFT_TEST_FAULT_TIMEOUT_100MS;
							}
							else
							{
								/* Only in normal mode */
								prbPTC->cnt = genFltParams.fltTime;

							}
							prbPTC->fltRstTimer = FLT_RESET_TIMEOUT_VALUE;
							probeMod->status = SFT_MODULE_PREFAULT;
						}
					}
				}

				break;

			case SFT_MODULE_PROTECTION:
				if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_NOFAULT)
				{
					probeMod->status = SFT_MODULE_RESET;
				}
				else if(prbPTC->currentAlertType != prbSftPers->protFrom)
				{
					probeMod->status = SFT_MODULE_MONITORING;
				}
				break;

			case SFT_MODULE_PREFAULT:
				/* Reset the status if the alert source is not the same that has triggered the prefault */
				if(prbPTC->currentAlertType != prbSftPers->protFrom )
				{
					if(prbPTC->fltRstTimer == 0)
					{
						if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_NOFAULT)
						{
							probeMod->status = SFT_MODULE_RESET;
						}
						else
						{
							probeMod->status = SFT_MODULE_MONITORING;
						}
					}
					else
					{
						prbPTC->fltRstTimer--;
					}
				}
				else
				{
					prbPTC->fltRstTimer = FLT_RESET_TIMEOUT_VALUE;
					if(prbPTC->cnt == 0) /* Timer expired */
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
				/* If in protection from FAULTSRC_PTCPROBE_MISMATCH the fault is not reversible */
				else if(prbSftPers->protFrom != FAULTSRC_PTCPROBE_MISMATCH)
				{
					if(prbPTC->currentAlertType == FAULTSRC_PTCPROBE_NOFAULT)
					{
						if(prbPTC->fltRstTimer == 0)
						{
							probeMod->status = SFT_MODULE_RESET;
						}
						else
						{
							prbPTC->fltRstTimer--;
						}

					}
					else
					{
						prbPTC->fltRstTimer = FLT_RESET_TIMEOUT_VALUE;
						if(prbSftPers->protFrom == FAULTSRC_PTCPROBE_BLOCKED)
						{
							prbSftPers->wasInFault = TRUE;
							if((prbSample->maxVal - prbSample->minVal) >= prbPTC->fltParams.deltaCheck)
							{
								prbSftPers->prbTestResult = PTC_TEST_OK;
								probeMod->status = SFT_MODULE_RESET;
							}
						}
					}

				}


				break;

			default:
				break;
		}

		/* Internal Timer */
		if(prbPTC->cnt > 0)
		{
			prbPTC->cnt--;
		}

	}/* End if(prbPTC->startDelayCnt == 0) */

	if(prbPTC->prbType == prbTOP )
	{
		prbMasterReleRes.oldStatus = prbMasterReleRes.currStatus;
	}
	for(i = 0; i < CKPTC_NUM_OF_LOADS; i++)
	{
		if(prbLoad[i].probeID == prbPTC->prbType)
		{
			prbLoad[i].oldStatus = prbLoad[i].currStatus;
		}

	}

	prbPTC->lastAlertType = prbPTC->currentAlertType;
	prbSftPers->lastCycleStatus = prbSftPers->currCycleStatus;




	/* ************************************************************* */

	prbSftPers->lastStatus = prbSftPers->status;
	prbSftPers->status = probeMod->status;

	if ( (persTopPTCProbe.faultHightTempCounter + persBottomPTCProbe.faultHightTempCounter) > persTopPTCProbe.limitFaultHightTempCounter)
	{ /* se si è superato il limite si entra subito in fault */
		probeMod->faultSrc = FAULTSRC_PTCPROBE_HIGHTEMP_OL;
		probeMod->status = SFT_MODULE_FAULT;
	} else {
		if (SFT_MODULE_PROTECTION == prbSftPers->status
				&& FAULTSRC_PTCPROBE_HIGHTEMP == probeMod->faultSrc)
		{
			/* entra in fault la prima volta */
			if (prbSftPers->lastStatus != prbSftPers->status)
			{
				/* si deve evitare il caso in cui F06 counter possa incrementare perchè una sonda rileva il superamento della
				 * soglia di temperatura in una cavità, dopo che l'altra ha fatto la stessa cosa, essendo quindi causa della
				 * apertura del Master Relay Resistenze */
				if(prbMasterReleRes.currStatus == LOAD_ON)
				{
					prbSftPers->faultHightTempCounter++; /* contatore backuppato */
					if (prbSftPers->faultHightTempCounter
							> prbSftPers->limitFaultHightTempCounter)
					{ /* non ripristinabile */
						probeMod->faultSrc = FAULTSRC_PTCPROBE_HIGHTEMP_OL;
					}/* else { ripristinabile probeMod->faultSrc = FAULTSRC_PTCPROBE_HIGHTEMP; */
				}
			}
		}
	}


	if(prbPTC->prbType == prbTOP)
	{
		SAFETY_DATA_UPDATE(topPTCProbe);
		SAFETY_DATA_UPDATE(persTopPTCProbe);
	}
	else
	{
		SAFETY_DATA_UPDATE(bottomPTCProbe);
		SAFETY_DATA_UPDATE(persBottomPTCProbe);
	}

	/* ************************************************************* */
	SAFETY_REGION_END();
}


/**
 * Checks for analog channels getting ready
 */
static void PM_SFT_CK_CheckForProbeChReady(SafetyPTCProbe_t *prbPTC)
{
	if(LIB_ADC_IsAnalogReady(prbPTC->prbPos) == TRUE)
	{
		prbPTC->startDelayCnt = 0;
		if(prbPTC->testDelayCnt > 0)
		{
			prbPTC->testDelayCnt--;
		}

	}
	else if(prbPTC->startDelayCnt > 0)
	{
		prbPTC->startDelayCnt--;
	}
}



/**
\n This function return the adc value of temperature from the top/bottom PTC probe
\n

\param [out] alertType The current alert type from safety module

\return		The adc value of temperature


\author		Salvatore Fimiani
\date		04/06/2013
\version

\note

*/
uint16_t PM_SFT_CK_PTCProbe_GetRawTemperature( uint8_t *alertType)
{
	uint16_t retVal = 0;


	if(prbBtmIsPresent == TRUE)
	{
		/* If the TOP probe is ok then set the alert type for the BOTTOM probe */
		if(topPTCProbe.currentAlertType == FAULTSRC_PTCPROBE_NOFAULT)
		{
			*alertType = bottomPTCProbe.currentAlertType;
		}
		else
		{
			*alertType = topPTCProbe.currentAlertType;
		}
		retVal = MAX(topProbeSample.currVal,bottomProbeSample.currVal);
	}
	else
	{
		*alertType = topPTCProbe.currentAlertType;
		retVal = topProbeSample.currVal;
	}

	if( (*alertType == FAULTSRC_PTCPROBE_HIGHTEMP) || (*alertType == FAULTSRC_PTCPROBE_HIGHTEMP_OL))
		*alertType = 0;

	return retVal;
}



/**
\n This function initializes the fault parameters for the top/bottom probe
\n

\param [out] The values of the fault parameters
\param [in]  The type of PTC probe (top/bottom)

\return		The result of the operation
\retval 	PRB_PARAMS_NO_ERROR
\retval 	BAD_PARAMETERS_SF
\retval 	PRB_PARAMS_POS_NOT_DEFINED


\author		Salvatore Fimiani
\date		04/06/2013
\version

\note

*/
static uint8_t PM_SFT_CK_PTCProbe_InitFltParams(SafetyPTCProbe_t *pPTCProbe, ePTCProbeType ePrbType)
{
	/* Pointer to the tab of the Probe Fault Parameters */
	struct Loader prbFltParTabPtr;
	uint8_t tempDispl = 0xFF;
	uint8_t err = PTC_ERR_PARAMS_OK;

	if(ePrbType == prbTOP)
	{
		tempDispl = DSPL_PROBEFLT_OVEN_TOP_PARAM;
		err |= PM_SFT_CK_PTCProbe_InitGenericFltParams();
	}
	else
	{
		tempDispl = DSPL_PROBEFLT_OVEN_BOTTOM_PARAM;
	}
	pPTCProbe->prbType = ePrbType;

	/* Get a pointer to the High Temperature Probe Fault Parameters Tab */
	if ( 0 != BasicLoader(SF_PTR_FAULT, tempDispl, &prbFltParTabPtr) )
	{
		err |= PTC_ERR_PARAMS_BAD_SF;
	}
	else
	{
		/* Reads probe parameters from the Probe Fault Parameters Tab */
		pPTCProbe->fltParams.ovrtPrefTime = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_TOHIGH_PREF_TIME)));
		/* Conversion of the faultTime from seconds to multiples of 100ms */
		pPTCProbe->fltParams.ovrtFltTime = (uint16_t)BASETIME_1S_TO_100MS((*(prbFltParTabPtr.Data + DSPL_OFFSET_OVEN_TOHIGH_FAULT_TIME)));
		pPTCProbe->fltParams.testTimeout = (uint16_t)600*(*(prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_TEST_TIMEOUT));
#if defined(DEBUG_WITHOUT_EDITOR)
		pPTCProbe->fltParams.fltTimeout = 20; /* 10 = 1 sec */
#else
		/* This parameter is unique for both the PTC probes */
		if(ePrbType == prbTOP)
		{
			pPTCProbe->fltParams.fltTimeout = BASETIME_1S_TO_100MS(EndianU16_BToN(*((uint16_t *)(prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_FLTR_TIMEOUT))));
		}
#endif
		pPTCProbe->prbPos = (uint8_t)(*(prbFltParTabPtr.Data + DSPL_OFFSET_OVEN_PROBE_POS));
		if((pPTCProbe->prbPos > ADC_NUM_CHANNELLS_USED))
		{
			 err |= PTC_ERR_PARAMS_POS_NOT_DEFINED;
		}
		pPTCProbe->fltParams.deltaCheck =  *(prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_DELTA_FOR_TEST);
		pPTCProbe->fltParams.shrtFaultADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_SHORTED_FAULT_THR))) & ADC_VALUE_MASK;
		pPTCProbe->fltParams.openFltADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_OPEN_FAULT_THR))) & ADC_VALUE_MASK;
		pPTCProbe->fltParams.minSafeADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_MIN_SAFE_THR))) & ADC_VALUE_MASK;
		pPTCProbe->fltParams.maxSafeADC_Thr = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_MAX_SAFE_THR))) & ADC_VALUE_MASK;
	}


	tempDispl = DSPL_PROBEFLT_OVEN_TOP_PARAM;
	if ( 0 != BasicLoader(SF_PTR_FAULT, tempDispl, &prbFltParTabPtr) )
	{
		err |= PTC_ERR_PARAMS_BAD_SF;
	}

	if(ePrbType == prbTOP)
	{
		persTopPTCProbe.limitFaultHightTempCounter = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_MAX_OVER_TEMP_CNT)));

		limitFaultHighTempCnt = persTopPTCProbe.limitFaultHightTempCounter;
	}
	else
	{
		persBottomPTCProbe.limitFaultHightTempCounter = EndianU16_BToN(*((uint16_t *) (prbFltParTabPtr.Data+DSPL_OFFSET_OVEN_MAX_OVER_TEMP_CNT)));

	}

	return err;
}


/**
\n This function initializes the generic fault parameters for the top/bottom PTC probe
\n

\param


\return		The result of the operation
\retval 	PRB_PARAMS_NO_ERROR
\retval 	BAD_PARAMETERS_SF
\retval 	PRB_PARAMS_POS_NOT_DEFINED


\author		Salvatore Fimiani
\date		04/06/2013
\version

\note

*/
static uint8_t PM_SFT_CK_PTCProbe_InitGenericFltParams(void)
{
	/* Pointer to the tab of the Probe Fault Parameters */
	struct Loader prbFltParTabPtr;
	uint8_t err = PTC_ERR_PARAMS_OK;

	/* Get a pointer to the Oven Probe Fault Parameters Tab */
	if ( 0 != BasicLoader(SF_PTR_FAULT, DSPL_PROBEFLT_OVEN_GEN_TIMEOUT, &prbFltParTabPtr) )
	{
		err = PTC_ERR_PARAMS_BAD_SF;
	}
	else
	{
		genFltParams.prefTime = ( ((uint16_t) (*prbFltParTabPtr.Data)<<8) + (uint16_t)(*(prbFltParTabPtr.Data+1)));
		genFltParams.fltTime = (uint16_t)BASETIME_1S_TO_100MS((*(prbFltParTabPtr.Data + DSPL_OFFSET_OVEN_GEN_FAULT_TIME)));
	}

	return err;

}



/**
*\n This function checks if there are the conditions for enabling the test to detect
*\n the blocked probe
*\n
*\param	[out]		probeMod			pointer to the safety info of the module
*\param	[in,out]	prbPTC				pointer to the control var of the PTC probe
*\param	[in,out]	prbSftPers			pointer to the control var related to the test for locked probe
*\param	[out]		prbSample			pointer to the var with the values acquired on the PTC probe

*\return

*\author	Salvatore Fimiani
*\date		05/06/2013
*\version
*/
static void PM_SFT_CK_BlockedProbeTriggerCheck(sft_module_status_t *probeMod, SafetyPTCProbe_t *prbPTC, PTC_SftCntrlData_t *prbSftPers, SamplePTCProbe_t *prbSample)
{

	/* The cavity has just came in RUN */
	if((prbSftPers->currCycleStatus == PTC_CYCLE_RUN)&&
			(prbSftPers->lastCycleStatus == PTC_CYCLE_STOP))
	{
		prbSftPers->prbBlockedTestON = TRUE;	/* It is a request for test of blocked probe */
		prbSample->maxVal = 0;					/* Initializes the max val for temperature for any new cycle */
		prbSample->minVal = 0xFFFF;				/* Initializes the min val for temperature for any new cycle */
		prbPTC->prbBlockedTestCnt = prbPTC->fltParams.testTimeout;
	}
	else if((prbSftPers->lastCycleStatus == PTC_CYCLE_RUN)&&
			(prbSftPers->currCycleStatus != PTC_CYCLE_RUN))
	{
		/* The cavity has just came STOPPED */
		prbSftPers->filterCnt = prbPTC->fltParams.fltTimeout;
	}
	if(prbSftPers->currCycleStatus != PTC_CYCLE_RUN)
	{
		if(prbSftPers->filterCnt == 0)
		{
			/* If there is a fault in progress, the test request and the test status must not be changed */
			if(prbSftPers->currCycleStatus == PTC_CYCLE_STOP)
			{
				prbSftPers->prbBlockedTestON = FALSE;			/* The request is canceled */
				prbSftPers->prbTestResult = PTC_TEST_STOPPED;	/* The test is stopped */
			}

			/* If there is at least a load switched on then there is a problem otherwise everything is ok */
			if(prbPTC->numLoadON > 0)
			{
				prbPTC->currentAlertType = FAULTSRC_PTCPROBE_MISMATCH;
			}
		}
		else
		{
			prbSftPers->filterCnt--;
		}
	}

	/* The master relay is closed */
	if(prbMasterReleRes.currStatus == LOAD_ON)
	{
		/* This routine checks if there are loads activated */
		uint8_t i;
		for(i = 0; i < CKPTC_NUM_OF_LOADS; i++)
		{
			if(prbLoad[i].probeID == prbPTC->prbType)
			{
				if((prbLoad[i].currStatus == LOAD_ON) && (prbLoad[i].oldStatus == LOAD_OFF))
				{
					prbPTC->numLoadON++;
				}
				else if((prbLoad[i].currStatus == LOAD_OFF) && (prbLoad[i].oldStatus == LOAD_ON))
				{
					prbPTC->numLoadON--;
				}
			}
		}
		/* If the RMR is closed, the request for test is executed */
		if(prbSftPers->prbBlockedTestON == TRUE)
		{
			prbSftPers->prbTestResult = PTC_TEST_IN_PROGR;
			prbSftPers->prbBlockedTestON = FALSE;		/* The request has been served */
		}
	}
	/* The master relay is open */
	else
	{
		prbPTC->numLoadON = 0;
		prbSftPers->filterCnt = 0;
		prbPTC->prbBlockedTestCnt = prbPTC->fltParams.testTimeout;
	}
}




/**
\n This function searches for the analog input location in ram.

\param 		[in] inputID Generic Input ID

\return 	uint8_t The result of research
\retval 	TRUE
\retval 	FALSE
\retval 	BASICLOADER_ERR

\author		Salvatore Fimiani
\date		05/06/2013
\version

\note
*/

static uint8_t PM_SFT_CK_PTCProbe_isGenInputPres(uint8_t inputID)
{
	struct Loader giMapTabPtr;
	uint8_t *pByte;  /* Local pointer used to scan inputs */

	uint8_t retVal = FALSE;

	if ( 0 != BasicLoader(SF_PTR_IO_CONFIG, EEP_GIMAP_TABPTR_OFFSET, &giMapTabPtr) )
	{
		retVal = BASICLOADER_ERR; /* Serious Error */
	}
	else
	{
		/* For all generic inputs in the setting file */
		for ( pByte = giMapTabPtr.Data;
					pByte < (giMapTabPtr.Data + (giMapTabPtr.DataLenght*2));
					pByte += GIMAP_TABLE_ITEM_SIZE_IN_BYTES )
		{

			if( GI_READ_ID(pByte) == inputID )
			{
				if(GI_READ_POSITION(pByte) < ADC_NUM_CHANNELLS_USED)
				{
					retVal = TRUE;
					break;
				}
			}
		}
	}
	return retVal;
}



uint16_t PM_SFT_CK_PTCProbe_TopGetTime(sft_module_status_t *status)
{
	return topPTCProbe.cnt;
}

uint16_t PM_SFT_CK_PTCProbe_BottomGetTime(sft_module_status_t *status)
{
	uint16_t retVal = 0;
	if(prbBtmIsPresent == TRUE)
	{
		retVal = bottomPTCProbe.cnt;
	}
	return retVal;
}




/**
 * Called by the application, this function returns the result of the test of blocked probe
 *
 *\param [in] 	prbType is the ID of the PTC probe (prbTOP/prbBOTTOM)
 *
 *\return uint8_t is the result of the test
 *\retval PTC_TEST_OK
 *\retval PTC_TEST_FAILED
 *\retval PTC_TEST_IN_PROGR
 *\retval PTC_TEST_STOPPED
 *\retval PTC_ID_NOT_VALID
 */

uint8_t PM_SFT_CK_PTCProbe_GetProbeTestResult(ePTCProbeType prbType)
{
	uint8_t retVal;
	if(prbType == prbTOP)
	{
		retVal = persTopPTCProbe.prbTestResult;
	}
	else if(prbType == prbBOTTOM)
	{
		retVal = persBottomPTCProbe.prbTestResult;
	}
	else
	{
		retVal = PTC_ERR_PROBE_ID_NOT_VALID;
	}
	return retVal;
}

uint16_t PM_SFT_CK_gettHighTempFaulCounter(void)
{
	uint16_t retVal = persTopPTCProbe.faultHightTempCounter;
	if( persBottomPTCProbe.faultHightTempCounter > retVal)
	{
		retVal = persBottomPTCProbe.faultHightTempCounter;
	}
	return retVal;
}

/**
 * This function registers each load with the related PTC probe
 *\n
 *\param [in]	prbType is the ID of the PTC probe (prbTOP/prbBOTTOM)
 *\n
 *\return uint8_t is the result of the operation
 *\retval 	PRB_PARAMS_NO_ERROR
 *\retval 	BAD_PARAMETERS_SF
 *\retval 	PRB_PARAMS_POS_NOT_DEFINED
 *
 */
static uint8_t PM_SFT_CK_PTCProbe_ConfigRelatedLoads(ePTCProbeType ePrbType)
{
	/* Pointer to the tab of the Probe Fault Parameters */
	struct Loader prbLoadsVsProbeTabPtr;
	uint8_t tempPos = 0xFF;

	uint16_t loadBitmap = 0x00;
	uint8_t err = PTC_ERR_PARAMS_OK;
	uint8_t i;




#if defined(DEBUG_WITHOUT_EDITOR)
	if(ePrbType == prbTOP)
	{
		loadBitmap = 0x07;
	}
	else
	{
		loadBitmap = 0x18;
	}

#else

	if ( 0 != BasicLoader(SF_PTR_FAULT, DSPL_RELLOAD_OVEN, &prbLoadsVsProbeTabPtr) )
	{
		err |= PTC_ERR_PARAMS_BAD_SF;
	}
	else
	{
		if(ePrbType == prbTOP)
		{
			loadBitmap = EndianU16_BToN(*((uint16_t *)(prbLoadsVsProbeTabPtr.Data + DSPL_OFFSET_OVEN_LOAD_BITMAP_TOP)));
		}
		else
		{
			loadBitmap = EndianU16_BToN(*((uint16_t *)(prbLoadsVsProbeTabPtr.Data + DSPL_OFFSET_OVEN_LOAD_BITMAP_BTM)));
		}

	}
#endif



	for(i = 0; i < CKPTC_NUM_OF_LOADS; i++)
	{

		switch((1u<<i))
		{
			case POSMASK_RES_SOLE:
				prbLoad[i].loadID = LD_SOLE_RES;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_SOLE_RES_POS);
			break;

			case POSMASK_RES_CIRC:
				prbLoad[i].loadID = LD_CIRCULAR_RES;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_CIRCULAR_RES_POS);
			break;

			case POSMASK_RES_GRL1:
				prbLoad[i].loadID = LD_GRILL1_RES;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_GRILL1_RES_POS);
			break;

			case POSMASK_RES_GRL2:
				prbLoad[i].loadID = LD_GRILL2_RES;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_GRILL2_RES_POS);
			break;

			case POSMASK_RES_CEIL:
				prbLoad[i].loadID = LD_CEILING;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_CEILING_POS);
			break;

			case POSMASK_FAN_CIRC:
				prbLoad[i].loadID = LD_CIRCULAR_FAN;
				tempPos = LIB_IOM_GetLoadPos((uint8_t) LD_CIRCULAR_FAN_POS);
			break;

			default:
			break;
		}/* End switch */

		if(tempPos < 16)
		{
			if((loadBitmap & (1u<<tempPos)))
			{
				prbLoad[i].probeID = ePrbType;
				prbLoad[i].pos = tempPos;
			}
		}
		else
		{
			prbLoad[i].probeID = 0xFF;
			prbLoad[i].pos = 0xFF;
		}



	}

	return err;
}

/**
 * Called by the application, this function sets the status of the cycle
 *\n
 *\param [in]   stMain  is the status of the cycle for the Main Cavity (probe TOP or BOTTOM)
 *\param [in]	stTop	is the status of the cycle for the Top Cavity (probe TOP)
 *\param [in] 	stXL	is the status of the cycle for the XL Cavity (probe TOP and BOTTOM)
 *\n
 *\n
 *\return uint8_t is the result of the operation
 *\retval	PTC_CYCLE_RUN_BOTH
 *\retval
 *\n
 */
uint8_t PM_SFT_CK_PTCProbe_SetCycleStatus(uint8_t stMain, uint8_t stTop, uint8_t stXL)
{
	uint8_t retVal = PTC_CYCLE_NOT_VALID;
	uint8_t err1 = ((stMain != PTC_CYCLE_RUN) && (stMain != PTC_CYCLE_STOP) && (stMain != PTC_CYCLE_FAULT))?PTC_CYCLE_NOT_VALID:0;
	uint8_t err2 = ((stTop != PTC_CYCLE_RUN) && (stTop != PTC_CYCLE_STOP)&& (stTop!= PTC_CYCLE_FAULT))?PTC_CYCLE_NOT_VALID:0;
	uint8_t err3 = ((stXL != PTC_CYCLE_RUN) && (stXL != PTC_CYCLE_STOP) && (stXL != PTC_CYCLE_FAULT))?PTC_CYCLE_NOT_VALID:0;
	if(!(err1 || err2 || err3))
	{
		prbBuffVsApp.bufMain = stMain;
		prbBuffVsApp.bufTop = stTop;
		prbBuffVsApp.bufXL = stXL;
		retVal = 0;
	}
	else
	{
		prbBuffVsApp.bufMain = PTC_CYCLE_UNDEF;
		prbBuffVsApp.bufTop = PTC_CYCLE_UNDEF;
		prbBuffVsApp.bufXL = PTC_CYCLE_UNDEF;
	}

	return retVal;
}



/**
 *
 */
static uint8_t PM_SFT_CK_PTCProbe_GetCycleStatus(BuffVsApp_t bufValue, ePTCProbeType prbType)
{
	uint8_t retVal = PTC_CYCLE_NOT_VALID;
	uint8_t topStatus = PTC_CYCLE_UNDEF, botStatus = PTC_CYCLE_UNDEF;

	if(prbBtmIsPresent == TRUE)
	{
		topStatus = bufValue.bufTop;
		botStatus = bufValue.bufMain;
		if(bufValue.bufXL == PTC_CYCLE_RUN)
		{
			topStatus = PTC_CYCLE_RUN;
			botStatus = PTC_CYCLE_RUN;
		}
		else if(bufValue.bufXL == PTC_CYCLE_FAULT)
		{
			topStatus = PTC_CYCLE_FAULT;
			botStatus = PTC_CYCLE_FAULT;
		}
	}
	else
	{
		topStatus = bufValue.bufMain;
	}



	if(prbType == prbTOP)
	{
		retVal = topStatus;
	}
	else if(prbType == prbBOTTOM)
	{
		retVal = botStatus;
	}


	return retVal;
}
