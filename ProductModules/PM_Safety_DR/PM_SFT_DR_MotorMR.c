/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_DR_MasterRelays
\n
\n
\file		PM_SFT_DR_MasterRelays.c
\ingroup	PM_Safety_DR
\date		05/06/2013
\author		Alessandro Sabbatini
\n
*/

/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "OutputManager.h"
#include "LoadsRequest.h"
#include "Analog.h"
#include "LIB_Utilities.h"
#include "LIB_ResetExp.h"
#include "LIB_ProductSafety.h"
#include "LIB_ClassBVar.h"
#include "PM_ConsistencyExp.h"

#include "PM_SFT_DR_MotorMR.h"
#include "PM_SFT_DR_Common.h"


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct sftMotorMRStatus_s
{
	uint8_t		*sftDoorParams;
	uint16_t	preFaultTime;
	uint16_t	faultTime;
	uint16_t 	resetTime;
	uint16_t 	resetCnt;
	uint16_t 	cnt;
	uint8_t 	fltrCnt;
	uint8_t		sftDoorPos;
	uint8_t		sftDoorNumItems;
	uint8_t		loadPos;
	uint8_t 	doorOpened:1;
	uint8_t 	forceDoorOpen:1;
	uint8_t 	chkExec:3;
} sftMotorMRStatus_t;

sftMotorMRStatus_t sftMotorMRStatus;

/* ********* */
/* Variables */
/* ********* */

/* *************** */
/* Function Bodies */
/* *************** */

/* ------------------------------------------------------------*/


void PM_SFT_DR_MotorMR_Init( sft_module_status_t *safetyData )
{  /* initialize safety module */
	Loader_Struct tmpLoaderPtr;
	bool_t cfgOK = FALSE;
	uint8_t *tmpFltData = (uint8_t *) NULL;
	uint16_t tmpResetTime = SFT_RESET_STD_TIMEOUT_100MS;

	SAFETY_REGION_START();

	sftMotorMRStatus.loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_MOTOR_MASTER_RELAY_ITM);

	cfgOK = (sftMotorMRStatus.loadPos != LOAD_NOT_FOUND);

	/* Check if Motor MR (ZC) Fault displacement is good */
	cfgOK = ((BasicLoader( (settingPntr_t) SF_PTR_FAULT, (uint8_t) SF_DISPL_FAULT_ZC, &tmpLoaderPtr) == SF_OK )
			&& cfgOK );

	if ( cfgOK )
	{
		tmpFltData = tmpLoaderPtr.Data;

		/* Reset time parameter is present in setting file */
		if (tmpLoaderPtr.DataLenght >= 3)
		{
			tmpResetTime = BASETIME_1S_TO_100MS((EndianU16_BToN(*((uint16_t *) &tmpFltData[SFT_MR_FLT_RESET_TIME])) & PREF_TIME_MASK));
		}
	}

	/* Check if Master Relay Safety displacement is good */
	cfgOK = ((BasicLoader( (settingPntr_t) SF_PTR_SAFETY_PARAMETERS, (uint8_t) SF_DISPL_SFT_MASTERRELAY, &tmpLoaderPtr) == SF_OK)
			&& cfgOK );

	/* Check if Motor Master Relay Safety displacement data size is good (at least 4)	*/
	cfgOK = ((tmpLoaderPtr.DataLenght > 0x02) && cfgOK);

	if( cfgOK )
	{
		sftMotorMRStatus.preFaultTime = EndianU16_BToN(*((uint16_t *) &tmpFltData[SFT_MR_FLT_PREFAULT_TIME])) & PREF_TIME_MASK;
		sftMotorMRStatus.faultTime = PM_SFT_DR_GetTimeByCntrValue(EndianU16_BToN(*((uint16_t *)(tmpFltData+SFT_MR_FLT_FAULT_TIME_OP))));
		sftMotorMRStatus.resetTime = tmpResetTime;

		sftMotorMRStatus.sftDoorPos = tmpLoaderPtr.Data[SFT_MR_PARM_DOOR_POS];
		sftMotorMRStatus.sftDoorNumItems = tmpLoaderPtr.Data[SFT_MR_PARM_DOOR_N_ITEMS];
		sftMotorMRStatus.sftDoorParams = &tmpLoaderPtr.Data[SFT_MR_PARM_DOOR_N_ITEMS + 1];

		/* Complete initialization */
		safetyData->status = SFT_MODULE_IDLE;
		safetyData->faultSrc = SFT_NO_FAULT_SRC;

	}
	else
	{
		/* Setting file error */
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_MOTORMR_BAD_SETTING;
	}

	SAFETY_DATA_UPDATE(sftMotorMRStatus);
	SAFETY_REGION_END();

}

void PM_SFT_DR_MotorMR_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	uint8_t newFaultSrc = safetyData->faultSrc;
	uint8_t newStatus = safetyData->status;
	uint8_t curConsistency = PM_CS_NOT_VALID_CODE;
	uint16_t newFltTmrValue = 0;
	bool_t tmpDoorClosed = PM_SFT_DR_MotorMR_IsDoorClosed();
	bool_t doReset = FALSE;

	LIB_SFT_EnterSafetyModule(safetyData);
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(sftMotorMRStatus);

	if ((safetyData->status != SFT_MODULE_FAULT) || (safetyData->faultSrc != FAULTSRC_MOTORMR_BAD_SETTING))
	{
		if (PM_CS_NO_ERROR == PM_Consistency_IsValueReady(sftMotorMRStatus.loadPos, &curConsistency))
		{
			/* Check actual consistency */
			switch (curConsistency)
			{
				case MR_CS_OFF_OK:
				case MR_CS_ON_OK:
				{
					newFaultSrc = SFT_NO_FAULT_SRC;
				}
				break;

				case MR_CS_OFF_ERR_SC:
				case MR_CS_OFF_ERR_GENERIC:
				{
					newFaultSrc = FAULTSRC_MOTORMR_STUCK_ON;
				}
				break;

				case MR_CS_ON_ERR_OC:
				case MR_CS_ON_ERR_GENERIC:
				{
					newFaultSrc = FAULTSRC_MOTORMR_STUCK_OFF;
				}
				break;

				default:
					break;
			}

		}

		if ( (!tmpDoorClosed) || (sftMotorMRStatus.forceDoorOpen))
		{
			sftMotorMRStatus.doorOpened = (uint8_t) TRUE;
			if (newFaultSrc != FAULTSRC_MOTORMR_STUCK_ON)
			{
				newFaultSrc = FAULTSRC_MOTORMR_PR_DOOR;
			}
		}

		/* When load request is OFF, reset single-pin check */
		if (newLoads->loads_req[sftMotorMRStatus.loadPos] == LOAD_REG_OFF)
		{
			sftMotorMRStatus.chkExec = 0;

			/* ON - OFF Transition: start reset timeout to refresh ClassB checks */
			if (LIB_IOM_GetLoadStatus(sftMotorMRStatus.loadPos) != LOAD_REG_OFF)
			{
				sftMotorMRStatus.resetCnt = sftMotorMRStatus.resetTime;
			}
		}
		else
		{
			/* When Master Relay request is ON, reset is disabled */
			sftMotorMRStatus.resetCnt = 0;

			if (safetyData->status >= SFT_MODULE_MONITORING)
			{
				if ((LIB_IOM_GetLoadStatus(sftMotorMRStatus.loadPos) == LOAD_REG_OFF)
				   &&(sftMotorMRStatus.chkExec == 0))
				{
					sftMotorMRStatus.chkExec = 4;
				}
			}
		}


		switch(safetyData->status)
		{
		case SFT_MODULE_IDLE:
		case SFT_MODULE_RESET:
		{
			if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
				if ((sftMotorMRStatus.cnt == 0) || (safetyData->faultSrc == SFT_NO_FAULT_SRC))
				{
					newStatus = SFT_MODULE_IDLE;
					sftMotorMRStatus.cnt = 0;
				}
				else
				{
					/* Keep Reset of current Fault source */
					newStatus = SFT_MODULE_RESET;
					newFaultSrc = safetyData->faultSrc;
				}

				/* When load status goes from OFF to ON and pilot type is digital output multi
				 * perform single-pin check
				 */
				if ( (LIB_IOM_GetLoadStatus(sftMotorMRStatus.loadPos) == LOAD_REG_OFF) && (newLoads->loads_req[sftMotorMRStatus.loadPos] == LOAD_REG_ON))
				{
					uint8_t tmpPilotType = LIB_IOM_GetLoadPilotType(sftMotorMRStatus.loadPos);

					if (tmpPilotType == DIGITAL_OUTPUT_MULTI_L)
					{
						newStatus = SFT_MODULE_MONITORING;
						newFaultSrc = FAULTSRC_MOTORMR_STUCK_ON;
						newFltTmrValue = SFT_SINGLEPIN_CHK_TIMEOUT_100MS;
						sftMotorMRStatus.chkExec = 3;
					}

				}

			}
			else
			{
				if (sftMotorMRStatus.forceDoorOpen)
				{
					newStatus = SFT_MODULE_PROTECTION;
					sftMotorMRStatus.forceDoorOpen = FALSE;
				}
				else
				{
					newStatus = SFT_MODULE_WARNING;
					if (newFaultSrc != FAULTSRC_MOTORMR_PR_DOOR)
					{
						newFltTmrValue = sftMotorMRStatus.preFaultTime;
					}
					else
					{
						newFltTmrValue = SFT_MR_FLTR_DOOR_TIMER;
					}
				}

			}

		}
		break;

		case SFT_MODULE_MONITORING:
		{
			newStatus = safetyData->status;

			if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
				if (sftMotorMRStatus.chkExec)
				{
					newFaultSrc = safetyData->faultSrc;
					sftMotorMRStatus.chkExec--;
					newFltTmrValue = SFT_SINGLEPIN_CHK_TIMEOUT_100MS;
				}
				else
				{
					newStatus = SFT_MODULE_IDLE;
				}
			}
			else if (newFaultSrc == FAULTSRC_MOTORMR_PR_DOOR)
			{
				newStatus = SFT_MODULE_WARNING;
				newFltTmrValue = SFT_MR_FLTR_DOOR_TIMER;
			}
			else
			{
				if (sftMotorMRStatus.cnt == 0)
				{
					newStatus = SFT_MODULE_WARNING;
					newFltTmrValue = sftMotorMRStatus.preFaultTime;
				}
			}

		}
		break;

		case SFT_MODULE_WARNING:
		{
			if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
				newStatus = SFT_MODULE_IDLE;
			}
			else
			{
				if (sftMotorMRStatus.cnt == 0)
				{
					if (newFaultSrc == FAULTSRC_MOTORMR_PR_DOOR)
					{
						sftMotorMRStatus.fltrCnt = SFT_MR_FLTR_DOOR_TIMER;
						newStatus = SFT_MODULE_PROTECTION;
					}
					else
					{
						sftMotorMRStatus.fltrCnt = SFT_MR_FLTR_TIMER_VALUE;
						newFltTmrValue = sftMotorMRStatus.faultTime;
						newStatus = SFT_MODULE_PREFAULT;
					}
				}
				else
				{
					/* If fault source changes between warning door and others during warning time, refresh timeout */
					if (safetyData->faultSrc != newFaultSrc)
					{
						if (newFaultSrc == FAULTSRC_MOTORMR_PR_DOOR)
						{
							newFltTmrValue = SFT_MR_FLTR_DOOR_TIMER;
						}
						else
						{
							newFltTmrValue = sftMotorMRStatus.preFaultTime;
						}
					}
				}
			}
		}
		break;

		case SFT_MODULE_PROTECTION:
		{
			if (tmpDoorClosed && (newLoads->loads_req[sftMotorMRStatus.loadPos] == LOAD_REG_OFF))
			{
				if (sftMotorMRStatus.fltrCnt > 0)
				{
					sftMotorMRStatus.fltrCnt--;
					newFaultSrc = safetyData->faultSrc;
					newStatus = safetyData->status;
				}
				else
				{
					newStatus = SFT_MODULE_RESET;
					newFaultSrc = safetyData->faultSrc;
					newFltTmrValue = SFT_MR_RESET_TIMER_VALUE;
					sftMotorMRStatus.doorOpened = (uint8_t) FALSE;
				}
			}
			else
			{
				if (newFaultSrc == FAULTSRC_MOTORMR_STUCK_ON)
				{
					sftMotorMRStatus.fltrCnt = SFT_MR_FLTR_TIMER_VALUE;
					newFltTmrValue = sftMotorMRStatus.faultTime;
					newStatus = SFT_MODULE_PREFAULT;
				}
				else
				{
					sftMotorMRStatus.fltrCnt = SFT_MR_FLTR_DOOR_TIMER;
					newFaultSrc = safetyData->faultSrc;
					newStatus = safetyData->status;
				}
			}
		}
		break;

		case SFT_MODULE_PREFAULT:
		{
			if (sftMotorMRStatus.cnt == 0)
			{
				newStatus = SFT_MODULE_FAULT;
			}
			else if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
				if (sftMotorMRStatus.fltrCnt > 0)
				{
					sftMotorMRStatus.fltrCnt--;
					newStatus = safetyData->status;
				}
				else
				{
					if (sftMotorMRStatus.doorOpened == (uint8_t) FALSE)
					{
						newStatus = SFT_MODULE_RESET;
						newFltTmrValue = SFT_MR_RESET_TIMER_VALUE;
					}
					else
					{
						newFaultSrc = FAULTSRC_MOTORMR_PR_DOOR;
					}
				}
			}
			else
			{
				sftMotorMRStatus.fltrCnt = SFT_MR_FLTR_TIMER_VALUE;
			}

			if (newFaultSrc == FAULTSRC_MOTORMR_PR_DOOR)
			{
				newStatus = SFT_MODULE_PROTECTION;
			}
			else
			{
				newFaultSrc = safetyData->faultSrc;
			}


		}
		break;

		case SFT_MODULE_FAULT:
		{
			/* Handle reset fault request */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				newStatus = SFT_MODULE_RESET;
				newFltTmrValue = SFT_MR_RESET_TIMER_VALUE;
			}

			/* Keep current fault source until it disappears (but not a different fault source appears!)*/
			newFaultSrc = safetyData->faultSrc;
			sftMotorMRStatus.chkExec = 0;
		}
		break;

		default:
			break;



		}

		switch(sftMotorMRStatus.chkExec)
		{
		case 1:
			newLoads->loads_ovr[sftMotorMRStatus.loadPos] = ( LOAD_CFG_MR_FORCE_DELAY | LOAD_CFG_MR_MULTI_2);
			break;

		case 2:
		case 4:
			newLoads->loads_ovr[sftMotorMRStatus.loadPos] = ( LOAD_CFG_MR_FORCE_DELAY |LOAD_REG_OFF);
			break;

		case 3:
			newLoads->loads_ovr[sftMotorMRStatus.loadPos] = ( LOAD_CFG_MR_FORCE_DELAY |LOAD_CFG_MR_MULTI_1);
			break;

		default:
			break;
		}


		/* Set new timer value */
		if (newFltTmrValue != 0)
		{
			if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
			{
#if defined(SFT_TEST_PREFAULT_TIMEOUT_100MS)
				if (newStatus == SFT_MODULE_WARNING)
				{
#if SFT_TEST_PREFAULT_TIMEOUT_100MS == 0
					newFltTmrValue = 0;
#else
					newFltTmrValue = MIN(newFltTmrValue, (uint16_t) SFT_TEST_PREFAULT_TIMEOUT_100MS);
#endif
				}
#endif

#if defined(SFT_TEST_FAULT_TIMEOUT_100MS)
				if (newStatus == SFT_MODULE_PREFAULT)
				{
#if SFT_TEST_FAULT_TIMEOUT_100MS == 0
					newFltTmrValue = 0;
#else
					newFltTmrValue = MIN(newFltTmrValue, (uint16_t) SFT_TEST_FAULT_TIMEOUT_100MS);
#endif
				}
#endif

			}

			sftMotorMRStatus.cnt = newFltTmrValue;

		}
		else
		{
			if (sftMotorMRStatus.cnt)
			{
				sftMotorMRStatus.cnt--;
			}
		}

		if (sftMotorMRStatus.resetCnt != 0)
		{
			if (LIB_IOM_GetLoadStatus(sftMotorMRStatus.loadPos) == LOAD_REG_OFF)
			{
				sftMotorMRStatus.resetCnt--;

				/* Timeout or explicit request from application */
				if ( (sftMotorMRStatus.resetCnt == 0)
				  || (applReqFlags & SFT_FLAGS_ENDCYCLE_REQ))
				{
					doReset = TRUE;
				}

				if (doReset)
				{
					sftMotorMRStatus.resetCnt = 0;
				}
			}

		}

		safetyData->faultSrc = newFaultSrc;
		safetyData->status = newStatus;

	}

	SAFETY_DATA_UPDATE(sftMotorMRStatus);
	SAFETY_REGION_END();
	LIB_SFT_LeaveSafetyModule(safetyData);

	if (doReset)
	{
		LIB_Reset_ForceReset();
	}

}

uint16_t PM_SFT_DR_MotorMR_GetTime(sft_module_status_t *status)
{
	return sftMotorMRStatus.cnt;
}

void PM_SFT_DR_MotorMR_ForceDoorOpen(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(sftMotorMRStatus);

	sftMotorMRStatus.forceDoorOpen = (uint8_t) TRUE;

	SAFETY_DATA_UPDATE(sftMotorMRStatus);
	SAFETY_REGION_END();
}

bool_t PM_SFT_DR_MotorMR_IsDoorClosed(void)
{
	bool_t retVal = FALSE;
	uint8_t	i;
	uint16_t *tmpMRDoorValues = (uint16_t *) sftMotorMRStatus.sftDoorParams;
	uint16_t tmpDoorAD = LIB_ADC_GetAnalogValue(sftMotorMRStatus.sftDoorPos)  & ONE_AD_SAMPLE_MAX_VALUE;

	for (i = 0; (retVal == FALSE) && (i < sftMotorMRStatus.sftDoorNumItems); i++)
	{
		uint16_t tmpMinThr = EndianU16_BToN(tmpMRDoorValues[2*i]) & ONE_AD_SAMPLE_MAX_VALUE;
		uint16_t tmpMaxThr = EndianU16_BToN(tmpMRDoorValues[(2*i)+1]) & ONE_AD_SAMPLE_MAX_VALUE;

		/* If Door ADC value is in range, door is closed properly */
		if ((tmpDoorAD > tmpMinThr) && (tmpDoorAD <= tmpMaxThr))
		{
			retVal = TRUE;
		}
	}

	return retVal;
}
