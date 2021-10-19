/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_WM_DrainPump
\n
\n
\file		PM_SFT_WM_DrainPump.c
\ingroup	PM_SFT_DrainPump
\date		04/03/2013
\author		Alessandro Sabbatini
\n
*/

/* ******** */
/* Includes */
/* ******** */

#define PM_SFT_WM_DRAIN_PUMP_MODULE

#include "Config.h"
#include "PM_SFT_WM_DrainPump.h"
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_SafetyActions.h"
#include "OutputManager.h"
#include "PM_ConsistencyExp.h"
#include "PM_DoorLockExp.h"
#include "Analog.h"
#include "LIB_Utilities.h"

#include "LIB_ClassBVar.h"
#include "SwTimersCounters.h"
/* ********* */
/* Defines   */
/* ********* */

#define		SFT_VIRT_TEMP_NORM_COEFF		256
#define		SFT_VIRT_PARAM_NORM_COEFF		16384ul


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

DrainPumpFaultStatus_t	sftDrainPumpStatus;
DrainPumpVirtualTemp_t	sftDrainPumpVirtualTemp;

/* *************** */
/* Function Bodies */
/* *************** */

static uint16_t PM_SFT_WM_Drainpump_SetPrefaultTime(uint8_t fltSrc);
static uint16_t PM_SFT_WM_DrainPump_UpdateTemp(uint16_t curTemp, uint8_t tmpParmOffs);
static bool_t PM_SFT_WM_DrainPump_DigOutTestProgress(void);

#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
static uint8_t PM_SFT_WM_DrainPump_IsDigOutMulti(void);
static void PM_SFT_WM_DrainPump_TestDigOutMulti(iom_loads_struct_t *loadRef, uint8_t sftStatus);
#endif

/* ------------------------------------------------------------*/


void PM_SFT_WM_Drainpump_Init( sft_module_status_t *safetyData )
{  /* initialize safety module */
	Loader_Struct tmpDrainPumpParamData;
	bool_t cfgOK = FALSE;
	uint8_t *tmpFltData = (uint8_t *) NULL;

	SAFETY_REGION_START();

	sftDrainPumpStatus.loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_DRAIN_PUMP_POS);

#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
	sftDrainPumpStatus.pinChk = PM_SFT_WM_DrainPump_IsDigOutMulti();
#endif

	/* Check if Drain Pump Fault displacement is good */
	cfgOK = ((BasicLoader( (settingPntr_t) SF_PTR_FAULT, (uint8_t) SF_DISPL_FAULT_DRAINPUMP, &tmpDrainPumpParamData) == SF_OK )
			&& (sftDrainPumpStatus.loadPos != LOAD_NOT_FOUND) );

	if ( cfgOK )
	{
		tmpFltData = tmpDrainPumpParamData.Data;
	}

	/* Check if Drain Pump Safety displacement is good */
	cfgOK = ((BasicLoader( (settingPntr_t) SF_PTR_SAFETY_PARAMETERS, (uint8_t) SF_DISPL_SFT_DRAINPUMP, &tmpDrainPumpParamData) == SF_OK)
			&& cfgOK );

	/* Check if Drain Pump Safety displacement data size is good (at least 14) */
	cfgOK = ((tmpDrainPumpParamData.DataLenght >= 0x0e) && cfgOK);

	if( cfgOK )
	{
		uint8_t tmpInitVirtTemp = SFT_DRAIN_PARM_VT_RST_TEMP;

		sftDrainPumpStatus.sftParams = tmpDrainPumpParamData.Data;
		sftDrainPumpStatus.fltParams = tmpFltData;

		/* Complete initialization */
		safetyData->status = SFT_MODULE_IDLE;
		safetyData->faultSrc = SFT_NO_FAULT_SRC;

		/* Drain Pump T_On auto is in 10s unit - convert it in seconds */
		sftDrainPumpStatus.autoOffToutCntr = sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_TON_AUTO] * 10;

		if (tmpDrainPumpParamData.DataLenght > 0x0e)
		{
			tmpInitVirtTemp = SFT_DRAIN_PARM_VT_FST_TEMP;
		}

		/* Virtual temps */
		sftDrainPumpVirtualTemp.virTempFreePump = ((uint16_t) sftDrainPumpStatus.sftParams[tmpInitVirtTemp] * SFT_VIRT_TEMP_NORM_COEFF);
		sftDrainPumpVirtualTemp.virTempPump = sftDrainPumpVirtualTemp.virTempFreePump;

		sftDrainPumpStatus.sftSecondTmr = 10;
	}
	else
	{
		/* Setting file error */
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_DRAINPUMP_BAD_SETTING;
	}

	SAFETY_DATA_UPDATE(sftDrainPumpStatus);
	SAFETY_DATA_UPDATE(sftDrainPumpVirtualTemp);
	SAFETY_REGION_END();

}

/**
\n This function implements the state machine which manages safety of the Drain Pump
\n module.
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] safetyData		Info related to the safety status of this module
\param [in] 	applReqFlags	Flags of requests by the application


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\n\li

\author
\date
\version



*/
void PM_SFT_WM_Drainpump_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
	uint8_t newFaultSrc = safetyData->faultSrc;
	uint8_t newStatus = safetyData->status;
	uint8_t curConsistency = PM_CS_NOT_VALID_CODE;
	uint16_t newFltTmrValue = 0;


	uint8_t curDrainStatus = LIB_IOM_GetLoadStatus(sftDrainPumpStatus.loadPos);
	uint8_t curSRCfgLoad = LIB_IOM_FindLoadOnSwitchRelay(sftDrainPumpStatus.loadPos);

	uint16_t tmpMaxTemp = ((uint16_t) sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_VT_MAX_TEMP] * SFT_VIRT_TEMP_NORM_COEFF);
	uint16_t tmpRstTemp = ((uint16_t) sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_VT_RST_TEMP] * SFT_VIRT_TEMP_NORM_COEFF);

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(sftDrainPumpStatus);
	SAFETY_DATA_CHECK(sftDrainPumpVirtualTemp);

	/* This avoid bad setting configuration:
	 * if Max temperature is lower than Restore temperature, discard latter one.
	 */
	if (tmpMaxTemp < tmpRstTemp)
	{
		tmpRstTemp = tmpMaxTemp;
	}

	if ((safetyData->status != SFT_MODULE_FAULT) || (safetyData->faultSrc != FAULTSRC_DRAINPUMP_BAD_SETTING))
	{
		if ( goodZCSignal() )
		{
			if (PM_CS_NO_ERROR == PM_Consistency_IsValueReady(sftDrainPumpStatus.loadPos, &curConsistency))
			{
				/* Check actual consistency */
				switch (curConsistency)
				{
				case DRAIN_CS_OFF_OK:
				{
					/* Diode Mode ON must be filtered since detected only
					 * when load is ON
					 */
					if ((safetyData->faultSrc != FAULTSRC_DRAINPUMP_TRIAC_DM_ON) ||
							/* Single pin check in progress is OK too */
							( (safetyData->faultSrc == FAULTSRC_DRAINPUMP_TRIAC_DM_OFF) &&
							(safetyData->status == SFT_MODULE_MONITORING)))
					{
						newFaultSrc = SFT_NO_FAULT_SRC;
					}
				}
				break;

				case DRAIN_CS_ON_OK:
				{
					/* Open Circuit and Diode Mode ON can be reset  when load is ON */
					if ( (safetyData->faultSrc == FAULTSRC_DRAINPUMP_TRIAC_OC) ||
							(safetyData->faultSrc == FAULTSRC_DRAINPUMP_TRIAC_DM_ON) ||
							(safetyData->faultSrc == FAULTSRC_DRAINPUMP_FB_KO) ||
					 /* Single pin check in progress is OK too */
							( (safetyData->faultSrc == FAULTSRC_DRAINPUMP_TRIAC_DM_OFF) &&
							(safetyData->status == SFT_MODULE_MONITORING)))
					{
						newFaultSrc = SFT_NO_FAULT_SRC;
					}
				}
				break;

				case DRAIN_CS_OFF_ERR_SC:
				{
					/* Short circuit when load is OFF and switch relay is not in
					 * its position can be filtered - it's due to EV
					 */
					if ((curSRCfgLoad & (LOAD_SR_FOUND | LOAD_SR_SAME_POS)) == LOAD_SR_FOUND)
					{
						newFaultSrc = SFT_NO_FAULT_SRC;
					}
					else
					{
						newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_SC;
					}
				}
				break;


				case DRAIN_CS_OFF_ERR_GENERIC:
				case DRAIN_CS_ON_ERR_GENERIC:
				{
					newFaultSrc = FAULTSRC_DRAINPUMP_FB_KO;
				}
				break;

				case DRAIN_CS_OFF_ERR_DM:
				{
					newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_DM_OFF;
				}
				break;

				case DRAIN_CS_ON_ERR_OC:
				{
					newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_OC;
				}
				break;


				case DRAIN_CS_ON_ERR_DM:
				{
					newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_DM_ON;
				}
				break;

				default:
				break;

				}
			}
		}
		else
		{
			/* No ZC signal good - assume everything goes fine */
			newFaultSrc = SFT_NO_FAULT_SRC;
		}

		/* Safety algorithms checked every second */
		if(sftDrainPumpStatus.sftSecondTmr == 0)
		{
			bool_t rfrAutoOff = FALSE;

			if (safetyData->status <= (uint8_t) SFT_MODULE_MONITORING)
			{
				if (sftDrainPumpStatus.autoOffToutCntr == 0)
				{
					/* Start DM Monitoring to find hidden fault cases */
					rfrAutoOff = TRUE;
					if (sftDrainPumpStatus.pinChk == SFT_DRAIN_TEST_COMPLETE)
					{
						sftDrainPumpStatus.pinChk = SFT_DRAIN_TEST_PHASE0;
					}
				}
				else
				{
					/* Countdown is active only when load is ON or FB KO and DM in OFF state case */
					if (curDrainStatus == (uint8_t) LOAD_REG_ON)
					{
							sftDrainPumpStatus.autoOffToutCntr--;
					}
					else
					{
						rfrAutoOff = TRUE;
					}
				}
			}
			else
			{
				rfrAutoOff = TRUE;
			}


			/* Refresh Auto OFF countdown timer */
			if (rfrAutoOff)
			{
				uint16_t tmpRefrCnt = sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_TON_AUTO];

				/* Reduce timer in Autotest mode - only if it's shorter than the original one! */
				if (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
				{
					uint16_t tmpParm = (uint16_t) sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_TON_AUTO_TEST];
					if (tmpParm != 0)
					{
						tmpRefrCnt = MIN(tmpRefrCnt, tmpParm);
					}
				}

				sftDrainPumpStatus.autoOffToutCntr = tmpRefrCnt * 10;
			}

			/* Drain pump is considered ON when:
			 *
			 * - ZC is present (of course!) AND one of
			 * 		- Pump is ON
			 * 		- There is a fault in progress (don't know what's happening to device... for me it's power supplied!)
			 */
			if ( goodZCSignal() && ( (curDrainStatus == (uint8_t) LOAD_REG_ON)
				|| ((safetyData->faultSrc != FAULTSRC_DRAINPUMP_BLOCKED) && (safetyData->status > SFT_MODULE_MONITORING))))
			{
				sftDrainPumpVirtualTemp.virTempFreePump = PM_SFT_WM_DrainPump_UpdateTemp(sftDrainPumpVirtualTemp.virTempFreePump, SFT_DRAIN_PARM_VT_FRE_SLOPE);

				if ( ( PM_SFT_WM_PressSwitch_GetPumpStatus() == PRSWH_PUMP_BLOCKED) || (sftDrainPumpStatus.lastloadStatus == LOAD_REG_OFF) )
				{
					sftDrainPumpVirtualTemp.virTempPump = PM_SFT_WM_DrainPump_UpdateTemp(sftDrainPumpVirtualTemp.virTempPump, SFT_DRAIN_PARM_VT_BLK_SLOPE);
				}
				else
				{
					sftDrainPumpVirtualTemp.virTempPump = sftDrainPumpVirtualTemp.virTempFreePump;
				}

			}
			else
			{
				sftDrainPumpVirtualTemp.virTempFreePump = PM_SFT_WM_DrainPump_UpdateTemp(sftDrainPumpVirtualTemp.virTempFreePump, SFT_DRAIN_PARM_VT_UNP_SLOPE);
				sftDrainPumpVirtualTemp.virTempPump = PM_SFT_WM_DrainPump_UpdateTemp(sftDrainPumpVirtualTemp.virTempPump, SFT_DRAIN_PARM_VT_UNP_SLOPE);
			}

			sftDrainPumpStatus.lastloadStatus = curDrainStatus;
			sftDrainPumpStatus.sftSecondTmr = 10;

		}
		else
		{
			sftDrainPumpStatus.sftSecondTmr--;
		}

		/* if no other fault sources are in place, check if virtual temperature go too high */
		if ( (newFaultSrc == SFT_NO_FAULT_SRC) || (newFaultSrc == FAULTSRC_DRAINPUMP_TRIAC_OC) || (curConsistency == PM_CS_NOT_VALID_CODE) )
		{
			/* At least one virtual temperature is higher than high temp set - go to Blocked rotor protection */
			if (sftDrainPumpVirtualTemp.virTempPump > tmpMaxTemp)
			{
				newFaultSrc = FAULTSRC_DRAINPUMP_BLOCKED;

				if ( (safetyData->status <= SFT_MODULE_MONITORING) || (newFaultSrc == FAULTSRC_DRAINPUMP_TRIAC_OC) )
				{
					newStatus = SFT_MODULE_WARNING;
					newFltTmrValue = BASETIME_1S_TO_100MS((uint16_t) sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_VT_PREFILTER]);
				}
			}

			/* If already in Blocked rotor fault source, wait to go under Restore temperature to clear it */
			if ( (safetyData->faultSrc == FAULTSRC_DRAINPUMP_BLOCKED) && (safetyData->status > SFT_MODULE_WARNING))
			{
				newFaultSrc = FAULTSRC_DRAINPUMP_BLOCKED;
			}

		}

		switch(safetyData->status)
		{
		case SFT_MODULE_IDLE:
		case SFT_MODULE_RESET:
		case SFT_MODULE_MONITORING:
		{
			if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
				if ((sftDrainPumpStatus.cnt == 0) || (safetyData->faultSrc == SFT_NO_FAULT_SRC))
				{
					newStatus = SFT_MODULE_IDLE;
				}
				else
				{
					/* Keep Reset of current Fault source */
					newStatus = SFT_MODULE_RESET;
					newFaultSrc = safetyData->faultSrc;
				}

				/* When single pin check is in progress show it as TRIAC DM Monitoring */
				if ( PM_SFT_WM_DrainPump_DigOutTestProgress())
				{
					newStatus = SFT_MODULE_MONITORING;
					newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_DM_OFF;
				}
			}
			else
			{
				/* If a new fault source is found and no explicit new status is set, assume Warning */
				if (newStatus <= SFT_MODULE_MONITORING)
				{
					newStatus = SFT_MODULE_WARNING;
					newFltTmrValue = PM_SFT_WM_Drainpump_SetPrefaultTime(newFaultSrc);
				}
			}

		}
		break;

		case SFT_MODULE_WARNING:
		{
			newStatus = safetyData->status;

			if (newFaultSrc == SFT_NO_FAULT_SRC)
			{
					/* When single pin check is in progress go back on TRIAC DM Monitoring */
				if ( PM_SFT_WM_DrainPump_DigOutTestProgress())
				{
					newStatus = SFT_MODULE_MONITORING;
					newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_DM_OFF;
				}
				else
				{
					newStatus = SFT_MODULE_IDLE;
				}
			}
			else if (sftDrainPumpStatus.cnt == 0)
			{
				if (newFaultSrc == FAULTSRC_DRAINPUMP_BLOCKED)
				{
					newStatus = SFT_MODULE_PROTECTION;
				}
				else
				{
					uint8_t newPrescaler;

					newStatus = SFT_MODULE_PREFAULT;
					sftDrainPumpStatus.fltrCnt = GEN_FLT_RESET_TIMER_VALUE;

					if (newFaultSrc == FAULTSRC_DRAINPUMP_TRIAC_OC)
					{
						newPrescaler = (sftDrainPumpStatus.fltParams[SFT_DRAIN_FLT_OC_FAULT_PS]);
						newFltTmrValue = (uint16_t) sftDrainPumpStatus.fltParams[SFT_DRAIN_FLT_OC_FAULT_VALUE];
					}
					else
					{
						newPrescaler = (sftDrainPumpStatus.fltParams[SFT_DRAIN_FLT_SC_FAULT_PS]);
						newFltTmrValue = (uint16_t) sftDrainPumpStatus.fltParams[SFT_DRAIN_FLT_SC_FAULT_VALUE];
					}


					if ((newPrescaler >> 3) == 3)
					{
						newFltTmrValue = BASETIME_10S_TO_100MS(newFltTmrValue);
					}
					else
					{
						newFltTmrValue = BASETIME_1S_TO_100MS(newFltTmrValue);
					}
				}
			}
			else if (newFaultSrc != FAULTSRC_DRAINPUMP_BLOCKED)
			{
				if ( (newFaultSrc != safetyData->faultSrc) && (safetyData->faultSrc == FAULTSRC_DRAINPUMP_TRIAC_OC))
				{
					newFltTmrValue = PM_SFT_WM_Drainpump_SetPrefaultTime(newFaultSrc);
				}
			}
			else
			{
				/* Do nothing */
			}

			/* If temperature go below maximum one, reset fault source */
			if ((sftDrainPumpVirtualTemp.virTempPump < tmpMaxTemp) && (newFaultSrc == FAULTSRC_DRAINPUMP_BLOCKED))
			{
				newStatus = SFT_MODULE_IDLE;
			}

		}
		break;

		case SFT_MODULE_PROTECTION:
		{

			/* Protection is used for blocked rotor fault source; other ones are remapped directly to Warning/Monitoring status */
			if (newFaultSrc == FAULTSRC_DRAINPUMP_BLOCKED)
			{
				/* Wait to go under Restore temperature to clear it */
				if (sftDrainPumpVirtualTemp.virTempPump < tmpRstTemp)
				{
					newStatus = SFT_MODULE_RESET;
					newFltTmrValue = SFT_DRAIN_FLTR_TIMER_VALUE;
				}
				else
				{
					sftDrainPumpStatus.cnt = 0;
					newStatus = SFT_MODULE_PROTECTION;
				}
			}
			else
			{
				newStatus = SFT_MODULE_WARNING;
			}

		}
		break;

		case SFT_MODULE_PREFAULT:
		{
			newStatus = safetyData->status;

			if (sftDrainPumpStatus.cnt == 0)
			{
				newStatus = SFT_MODULE_FAULT;
			}
			else if ((newFaultSrc == SFT_NO_FAULT_SRC) || (newFaultSrc == FAULTSRC_DRAINPUMP_BLOCKED))
			{
				if (sftDrainPumpStatus.fltrCnt > 0)
				{
					sftDrainPumpStatus.fltrCnt--;
					newFaultSrc = safetyData->faultSrc;
				}
				/* Virtual temperature must be lower than Restore, otherwise it go back to blocked rotor */
				else if (sftDrainPumpVirtualTemp.virTempPump > tmpRstTemp)
				{
					newFaultSrc = FAULTSRC_DRAINPUMP_BLOCKED;
					newStatus = SFT_MODULE_PROTECTION;
				}
				else
				{
					/* When single pin check is in progress go back on TRIAC DM Monitoring */
					if ((newFaultSrc != FAULTSRC_DRAINPUMP_BLOCKED) && ( PM_SFT_WM_DrainPump_DigOutTestProgress()))
					{
						newStatus = SFT_MODULE_MONITORING;
						newFaultSrc = FAULTSRC_DRAINPUMP_TRIAC_DM_OFF;
					}
					else
					{
						newStatus = SFT_MODULE_RESET;
						newFaultSrc = safetyData->faultSrc;
					}
					newFltTmrValue = SFT_DRAIN_FLTR_TIMER_VALUE;
				}
			}
			else
			{
				newFaultSrc = safetyData->faultSrc;
				sftDrainPumpStatus.fltrCnt = GEN_FLT_RESET_TIMER_VALUE;
			}
		}
		break;

		case SFT_MODULE_FAULT:
		{
			/* Handle reset fault request */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				newStatus = SFT_MODULE_IDLE;
				newFaultSrc = SFT_NO_FAULT_SRC;
			}
			else
			{
				newStatus = safetyData->status;
			}

			/* Keep current fault source until it disappears (but not a different fault source appears!)*/
			newFaultSrc = safetyData->faultSrc;
		}
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

			sftDrainPumpStatus.cnt = newFltTmrValue;

		}
		else
		{
			if (sftDrainPumpStatus.cnt)
			{
				sftDrainPumpStatus.cnt--;
			}
		}

#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
		PM_SFT_WM_DrainPump_TestDigOutMulti(newLoads, newStatus);
#endif

		safetyData->faultSrc = newFaultSrc;
		safetyData->status = newStatus;

	}

	SAFETY_DATA_UPDATE( sftDrainPumpStatus );
	SAFETY_DATA_UPDATE( sftDrainPumpVirtualTemp );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);
}

uint16_t PM_SFT_WM_Drainpump_GetTime(sft_module_status_t *status)
{
	uint16_t retVal = 0;

	if (status->status == SFT_MODULE_IDLE)
	{
		retVal = BASETIME_1S_TO_100MS(sftDrainPumpStatus.autoOffToutCntr);
	}
	else if (status->status == SFT_MODULE_MONITORING)
	{
		retVal = sftDrainPumpStatus.fltrCnt;
	}
	else
	{
		retVal = sftDrainPumpStatus.cnt;
	}

	return retVal;

}

static uint16_t PM_SFT_WM_Drainpump_SetPrefaultTime(uint8_t fltSrc)
{
	uint8_t tmrIdx = SFT_DRAIN_FLT_SC_PREFAULT;

	if (fltSrc == FAULTSRC_DRAINPUMP_TRIAC_OC)
	{
		tmrIdx =SFT_DRAIN_FLT_OC_PREFAULT;
	}

	return EndianU16_BToN(*((uint16_t *) &sftDrainPumpStatus.fltParams[tmrIdx]));

}

static uint16_t PM_SFT_WM_DrainPump_UpdateTemp(uint16_t curTemp, uint8_t tmpParmOffs)
{
	uint16_t tmpNewVal = curTemp;

	if ((tmpParmOffs == SFT_DRAIN_PARM_VT_BLK_SLOPE) || (tmpParmOffs == SFT_DRAIN_PARM_VT_FRE_SLOPE) || (tmpParmOffs == SFT_DRAIN_PARM_VT_UNP_SLOPE))
	{
		uint16_t tmpMinTemp = ((uint16_t) sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_VT_MIN_TEMP] * SFT_VIRT_TEMP_NORM_COEFF);
		uint16_t tmpSlope = EndianU16_BToN(*((uint16_t *) &sftDrainPumpStatus.sftParams[tmpParmOffs]));
		uint16_t tmpIntcp = EndianU16_BToN(*((uint16_t *) &sftDrainPumpStatus.sftParams[tmpParmOffs+2]));

		tmpNewVal = (uint16_t) ((((uint32_t) curTemp * (uint32_t) tmpSlope) + (((uint32_t) tmpIntcp)* (uint32_t) SFT_VIRT_TEMP_NORM_COEFF)) / SFT_VIRT_PARAM_NORM_COEFF);

			/* This avoids stall in same value due to roundup computation */
			if (tmpNewVal == curTemp)
			{
				if (tmpParmOffs == SFT_DRAIN_PARM_VT_UNP_SLOPE)
				{
					tmpNewVal--;
				}
				else
				{
					tmpNewVal++;
				}
			}

		/* Check if go below minimum temp */
		if (tmpNewVal < tmpMinTemp)
		{
			tmpNewVal = tmpMinTemp;
		}
	}

	return tmpNewVal;

}

#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
static uint8_t PM_SFT_WM_DrainPump_IsDigOutMulti(void)
{
	uint8_t retVal = SFT_DRAIN_TEST_DISABLED;
	uint8_t loadPilotType = LIB_IOM_GetLoadPilotType(sftDrainPumpStatus.loadPos);

	if(loadPilotType == DIGITAL_OUTPUT_MULTI_L)
	{
		retVal = SFT_DRAIN_TEST_PHASE0;
	}


	return retVal;

}

/**
 *\n This function drives the Drain Pump with the new Collector Board.
 *\n The Drain Pump is piloted with one or two independent identical circuits.
 *\n If at least one of the two is driven to HIGH, the pump will be switched on and,
 *\n if there is no faults, the feedback will be 1 else you will see the 50 Hz signal.
 *\n If both driving circuits are switched off, the pump will be switched off (50 Hz).
 *\n
 *\n At the switching on, for a given time, a test for each circuit will be executed:
 *\n the two channels will be piloted in alternate way. When the test timeout has expired,
 *\n both channels will be driven HIGH.
 *\n
 *\param  [in,out]	*loadRef:		pointer to the load request array
 *\param  [in]		loadPos:		the position of the drain pump in the Load Status array
 *\param  [in]		faultStatus:	the status of driving of the drain pump
 *\param  [in]		pmpReg:			0: OFF - 1: ON
 *\n
 *\retval SFT_DRAIN_TEST_PHASE0		The pump is OFF - No test required
 *\retval SFT_DRAIN_TEST_PHASE1 	The test phase with pin1 high and pin2 low is in progress
 *\retval SFT_DRAIN_TEST_PHASE2		The test phase with both the pins high is in progress
 *\retval SFT_DRAIN_TEST_PHASE3		The test phase with pin1 low and pin2 high is in progress
 *\retval SFT_DRAIN_TEST_COMPLETE	Test is complete: from now on the drain pump will be piloted with both channels (pin1 high, pin2 high)
 *
 */
static void PM_SFT_WM_DrainPump_TestDigOutMulti(iom_loads_struct_t *loadRef, uint8_t sftStatus)
{
	uint8_t newOvr = LOAD_NOT_FOUND;
	uint8_t newTmrVal =  sftDrainPumpStatus.sftParams[SFT_DRAIN_PARM_TOFF_AUTO] / 3;

	if (newTmrVal < SFT_SINGLEPIN_CHK_TIMEOUT_100MS)
	{
		newTmrVal = SFT_SINGLEPIN_CHK_TIMEOUT_100MS;
	}

	if (sftDrainPumpStatus.pinChk != SFT_DRAIN_TEST_DISABLED)
	{
		if ( (loadRef->loads_req[sftDrainPumpStatus.loadPos] == LOAD_REG_ON) ||
			(	(sftStatus > SFT_MODULE_MONITORING) &&
				(sftDrainPumpStatus.pinChk != SFT_DRAIN_TEST_PHASE0) &&
				(sftDrainPumpStatus.pinChk != SFT_DRAIN_TEST_COMPLETE) ))
		{/* At switching on the test is executed */
			if(sftDrainPumpStatus.pinChk == SFT_DRAIN_TEST_PHASE0)
			{
				sftDrainPumpStatus.pinChk = SFT_DRAIN_TEST_PHASE1;
			}

			switch(sftDrainPumpStatus.pinChk)
			{
				/* Testing the circuit at the pin1 of the digital output channel */
				case SFT_DRAIN_TEST_PHASE1:
					newOvr = LOAD_CFG_MR_MULTI_1 | LOAD_REG_ON;
					break;
				/* Testing the circuit at the pin2 of the digital output channel */
				case SFT_DRAIN_TEST_PHASE2:
					newOvr = LOAD_REG_ON;
					break;
				case SFT_DRAIN_TEST_PHASE3:
					newOvr = LOAD_CFG_MR_MULTI_2 | LOAD_REG_ON;
					break;
					/* Test is complete */
				case SFT_DRAIN_TEST_COMPLETE:
				default:
					break;

			}

			if(sftStatus <= SFT_MODULE_MONITORING)
			{
				if(sftDrainPumpStatus.pinChk != SFT_DRAIN_TEST_COMPLETE)
				{
					sftDrainPumpStatus.fltrCnt++;
				}

				if(sftDrainPumpStatus.fltrCnt >= newTmrVal)
				{
					sftDrainPumpStatus.fltrCnt = 0;
					sftDrainPumpStatus.pinChk++;
				}
			}

		}
		else
		{
			sftDrainPumpStatus.pinChk = SFT_DRAIN_TEST_PHASE0;
			if(sftStatus <= SFT_MODULE_MONITORING)
			{
				sftDrainPumpStatus.fltrCnt = 0;
			}
		}

		if (newOvr != LOAD_NOT_FOUND)
		{
			loadRef->loads_ovr[sftDrainPumpStatus.loadPos] = newOvr;
		}

	}

}

#endif

/**
 *\n This function returns if single pin check is in progress or not.
 *\n
 *\param  none
 *\n
 *\retval TRUE						Single pin check in progress
 *\retval FALSE						Single pin check not active (completed or disabled)
 *
 */
static bool_t PM_SFT_WM_DrainPump_DigOutTestProgress(void)
{
#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_SELECTABLE_PINS)
	return ( (sftDrainPumpStatus.pinChk == SFT_DRAIN_TEST_PHASE1) ||
			(sftDrainPumpStatus.pinChk == SFT_DRAIN_TEST_PHASE2) ||
			(sftDrainPumpStatus.pinChk == SFT_DRAIN_TEST_PHASE3));
#else
	return FALSE;
#endif
}

