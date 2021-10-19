/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_Safety_DR_Heaters
\n
\n
\file		PM_SFT_DR_Heaters.c
\ingroup	PM_Safety_DR
\date		05/06/2013
\author		Salvatore Fimiani
\n
*/

/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "Analog.h"
#include "LoadsRequest.h"
#include "OutputManager.h"
#include "LIB_Utilities.h"
#include "LIB_ClassBVar.h"
#include "LIB_ProductSafety.h"

#include "PM_SFT_DR_Common.h"
#include "PM_SFT_DR_Heaters.h"
#include "PM_ConsistencyExp.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define DSPL_HEATERFLT_DRYER_STA_PARAMS			0x02u	/* Static Heater Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_HEATERFLT_DRYER_DYN_PARAMS			0x03u	/* Dynamic Heater Fault Parameters displacement vs SF_PTR_FAULT */
#define DSPL_OFFSET_DRYER_PREF_TIME_CA		0
#define DSPL_OFFSET_DRYER_FAULT_TIME_CA		2
#define DSPL_OFFSET_DRYER_PREF_TIME_CC		4
#define DSPL_OFFSET_DRYER_FAULT_TIME_CC		6





#define BAD_PARAMETERS_SF				0x80
#define HEATER_PARAMS_NO_ERROR				0x00
#define HEATER_PARAMS_POS_NOT_DEFINED		0x01


#define LOAD_UNDEF			0xFFu

#if (SFT_MRR_CHECK_TIMEOUT < 3*SFT_SINGLEPIN_CHK_TIMEOUT_100MS)
#error	"MRR_CHECK_TIMEOUT shoult be at least 3 times SFT_SINGLEPIN_CHK_TIMEOUT_100MS!"
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */
static SafetyMRHtr_t genMRHeatr;
static SafetyHeater_t staHeaterRes, dynHeaterRes;

/* Global */


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void PM_SFT_DR_Heaters_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *pModInfo, uint8_t applReqFlags,
		SafetyHeater_t *pHeaterRes);
static void PM_SFT_DR_Heater_GeneralInit(sft_module_status_t *pModInfo, eHeaterResType resType, SafetyHeater_t *pHeaterRes);
static bool_t PM_SFT_DR_Heater_CheckMRR(void);
static void PM_SFT_DR_Heater_ResetCheckMRR(void);

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

void PM_SFT_DR_Heater_Common_Init(void)
{
	SAFETY_REGION_START();
	memset(&genMRHeatr, 0, sizeof(genMRHeatr));

	genMRHeatr.resMrRele.pos =  LIB_IOM_GetLoadPos((uint8_t) LD_RES_MASTER_RELAY_ITM);
	genMRHeatr.resMrRele.curStatus = LOAD_NOT_FOUND;
	genMRHeatr.resMrRele.lastStatus = LOAD_NOT_FOUND;
	genMRHeatr.motMrRele.pos =  LIB_IOM_GetLoadPos((uint8_t) LD_MOTOR_MASTER_RELAY_ITM);
	genMRHeatr.motMrRele.curStatus = LOAD_NOT_FOUND;
	genMRHeatr.motMrRele.lastStatus = LOAD_NOT_FOUND;
	SAFETY_DATA_UPDATE(genMRHeatr);
	SAFETY_REGION_END();
}


/**
 *\n Initialization of parameters for the Static Heater
 *\n
 *\param[in,out] pModInfo		Info related to the safety status of this module
 *
 */
void PM_SFT_DR_StaHeater_Init( sft_module_status_t *pModInfo )
{
	SAFETY_REGION_START();
	PM_SFT_DR_Heater_GeneralInit(pModInfo, STATIC_HEATER, &staHeaterRes);
	SAFETY_DATA_UPDATE(staHeaterRes);
	SAFETY_REGION_END();
}


/**
 *\n Initialization of parameters for the Dynamic Heater
 *\n
 *\param[in,out] pModInfo			Info related to the safety status of this module
 *\n
 */
void PM_SFT_DR_DynHeater_Init( sft_module_status_t *pModInfo )
{
	SAFETY_REGION_START();
	PM_SFT_DR_Heater_GeneralInit(pModInfo, DYNAMIC_HEATER, &dynHeaterRes);
	SAFETY_DATA_UPDATE(dynHeaterRes);
	SAFETY_REGION_END();
}


void PM_SFT_DR_StaHeater_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *pModInfo, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(pModInfo);
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(staHeaterRes);
	PM_SFT_DR_Heaters_GeneralManager(newLoads, pModInfo, applReqFlags, &staHeaterRes);
	SAFETY_DATA_UPDATE(staHeaterRes);
	SAFETY_REGION_END();
	LIB_SFT_LeaveSafetyModule(pModInfo);
}

void PM_SFT_DR_DynHeater_Monitor (iom_loads_struct_t *newLoads, sft_module_status_t *pModInfo, uint8_t applReqFlags)
{
	LIB_SFT_EnterSafetyModule(pModInfo);
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(dynHeaterRes);
	PM_SFT_DR_Heaters_GeneralManager(newLoads, pModInfo, applReqFlags, &dynHeaterRes);
	SAFETY_DATA_UPDATE(dynHeaterRes);
	SAFETY_REGION_END();
	LIB_SFT_LeaveSafetyModule(pModInfo);
}

uint16_t PM_SFT_DR_StaHeater_GetTime(sft_module_status_t *status)
{
	uint16_t resTmr = staHeaterRes.cnt;

	if (PM_SFT_DR_Heater_isResMRCheckInProgr() && (status->status < SFT_MODULE_WARNING))
	{
		resTmr = genMRHeatr.checkCnt;
	}

	return resTmr;
}

uint16_t PM_SFT_DR_DynHeater_GetTime(sft_module_status_t *status)
{
	uint16_t resTmr = dynHeaterRes.cnt;

	if (PM_SFT_DR_Heater_isResMRCheckInProgr() && (status->status < SFT_MODULE_WARNING))
	{
		resTmr = genMRHeatr.checkCnt;
	}

	return resTmr;

}


/* Local Functions */

/**
 *\n General initialization function
 *\n
 *\param[in,out] pModInfo			Info related to the safety status of this module
 *\param[in] resType				Resistor ID (Static/Dynamic)
 *\param[in,out] pHeaterRes			Safety parameters and counters
 *\n
 *\n
 */

static void PM_SFT_DR_Heater_GeneralInit(sft_module_status_t *pModInfo, eHeaterResType resType, SafetyHeater_t *pHeaterRes)
{
	/* Pointer to the tab of the Heater Fault Parameters */
	struct Loader resFltParTabPtr;
	uint8_t tempDispl = 0xFF;
	uint8_t err = HEATER_PARAMS_NO_ERROR;

	SAFETY_DATA_CHECK(genMRHeatr);

	if(resType == STATIC_HEATER)
	{
		pHeaterRes->info.pos = LIB_IOM_GetLoadPos((uint8_t) LD_STATIC_RES_ITM);
		tempDispl = DSPL_HEATERFLT_DRYER_STA_PARAMS;
	}
	else
	{
		pHeaterRes->info.pos = LIB_IOM_GetLoadPos((uint8_t) LD_DYNAMIC_RES_ITM);
		tempDispl = DSPL_HEATERFLT_DRYER_DYN_PARAMS;
	}

	if (pHeaterRes->info.pos != LOAD_NOT_FOUND)
	{
		pHeaterRes->resType = resType;
		pHeaterRes->cnt = 0;
		pHeaterRes->monitorAlertType = FAULTSRC_HEATER_NOFAULT;	/* Da valutare se può essere eliminata inizializzazione */
		pHeaterRes->info.curStatus = LOAD_UNDEF;	/* Da valutare se può essere eliminata inizializzazione */
		pHeaterRes->info.lastStatus = LOAD_UNDEF;	/* Da valutare se può essere eliminata inizializzazione */

		/* Get a pointer to the Static/Dynamic Heater Fault Parameters Tab
		 * Check if MRR and MRM are available */
		if (( 0 != BasicLoader(SF_PTR_FAULT, tempDispl, &resFltParTabPtr) )
				&& (genMRHeatr.resMrRele.pos  != LOAD_NOT_FOUND)
				&& (genMRHeatr.motMrRele.pos  != LOAD_NOT_FOUND) )
		{
			err = BAD_PARAMETERS_SF;
		}
		else
		{
			/* Reads parameters from the Static/Dynamic Heater Fault Parameters Tab */
			pHeaterRes->genFltParams.prefaultTimeCA = EndianU16_BToN(*((uint16_t *) (resFltParTabPtr.Data+DSPL_OFFSET_DRYER_PREF_TIME_CA))) & PREF_TIME_MASK;
			pHeaterRes->genFltParams.faultTimeCA = PM_SFT_DR_GetTimeByCntrValue(EndianU16_BToN(*((uint16_t *)(resFltParTabPtr.Data + DSPL_OFFSET_DRYER_FAULT_TIME_CA))));

			pHeaterRes->genFltParams.prefaultTimeCC = EndianU16_BToN(*((uint16_t *) (resFltParTabPtr.Data+DSPL_OFFSET_DRYER_PREF_TIME_CC))) & PREF_TIME_MASK;
			pHeaterRes->genFltParams.faultTimeCC = PM_SFT_DR_GetTimeByCntrValue(EndianU16_BToN(*((uint16_t *)(resFltParTabPtr.Data+DSPL_OFFSET_DRYER_FAULT_TIME_CC))));

			/* Add heater to MRR check */
			if(resType == STATIC_HEATER)
			{
				genMRHeatr.bmpHtrCfg |= (1 << STATIC_HEATER);
			}
			else
			{
				genMRHeatr.bmpHtrCfg |= (1 << DYNAMIC_HEATER);
			}
		}
	}


	if (err != HEATER_PARAMS_NO_ERROR)
	{
		pModInfo->status = SFT_MODULE_FAULT;
		pModInfo->faultSrc = BAD_PARAMETERS_SF;
	}
	else
	{
		pModInfo->status = SFT_MODULE_IDLE;
		pModInfo->faultSrc = SFT_NO_FAULT_SRC;
	}

	SAFETY_DATA_UPDATE(genMRHeatr);

}

/**
 *\n General manager for monitoring the safety status of the two drying resistors:
 *\n Static Heater and Dynamic Heater.
 *\n The control algorithm is build on the consistency table below:
 *\n
 *\n						Low (0)	|	High (1)|	UnPhase (2)	| Phase (3)
 *\n 		UNPILOT			  2		|	  3		|	  	1		|	0
 *\n		PILOT			  2		|	  0		|	  	1		|	4
 *\n
 *\n Consistency = 2 (*, Low)			-> Feedback KO
 *\n Consistency = 3 (Unpilot, High)	-> RMR stuck/Resistor shorted
 *\n Consistency = 0 (Pilot, High) 		-> The resistor is correctly ON
 *\n Consistency = 0 (Unpilot, Phase)	-> The resistor is correctly OFF only is RMR is ON
 *\n Consistency = 1 (*,UnPhase)		-> Generic fault
 *\n Consistency = 4 (Pilot, Phase)		-> RMR shorted/leakage
 *\n
 *\param[in,out] newLoads			Info related to loads status request
 *\param[in,out] pModInfo			Info related to the safety status of this module
 *\param[in] 	 applReqFlags		Flags of requests by the application
 *\param[in,out] pHeaterRes			Safety parameters and counters
 *\n
 *\return void
 *\retval	N.A.
 *
 *\par Architecture Requirements ID:
 *\n\li	SARS_
 *\n\li	SARS_
 *\n\li	SARS_
 *\n\li	SARS_
 *\n\li	SARS_
 */

static void PM_SFT_DR_Heaters_GeneralManager(iom_loads_struct_t *newLoads, sft_module_status_t *pModInfo, uint8_t applReqFlags,
		SafetyHeater_t *pHeaterRes)
{
	uint8_t curAlertType = FAULTSRC_HEATER_NOFAULT;
	uint8_t curConsistency = PM_CS_NOT_VALID_CODE;
	uint8_t resMRCheckInProgr = FALSE;
	uint8_t newHtrStatus = LOAD_NOT_FOUND;

	SAFETY_DATA_CHECK(genMRHeatr);

	if (pHeaterRes->info.pos != LOAD_NOT_FOUND)
	{
		/* Acquiring motor and resistor master relays' status */
		genMRHeatr.resMrRele.curStatus = (LIB_IOM_GetLoadStatus(genMRHeatr.resMrRele.pos) & LOAD_REG_MASK);
		genMRHeatr.motMrRele.curStatus = (LIB_IOM_GetLoadStatus(genMRHeatr.motMrRele.pos) & LOAD_REG_MASK);

		/* Checks if a test for checking the Resistors Master Relay is in progress */
		resMRCheckInProgr = PM_SFT_DR_Heater_CheckMRR();

		/* Controls are made only when the Motor Master Relay and the door are closed */
		if(goodZCSignal() == TRUE)
		{
			if(PM_CS_NO_ERROR == PM_Consistency_IsValueReady(pHeaterRes->info.pos,&curConsistency))
			{

				switch(curConsistency)
				{
					case HEATER_CS_ON_OK:
						curAlertType = FAULTSRC_HEATER_NOFAULT;
						break;
					case HEATER_CS_OFF_OK:
						if(genMRHeatr.resMrRele.curStatus == LOAD_ON)
						{
							curAlertType = FAULTSRC_HEATER_NOFAULT;
						}
	/* Resistors Master Relay not piloted and Feedback = 3 -> Fault Code.SubCode: 08.03 (Static and Dynamic) */
						else
						{
							curAlertType = FAULTSRC_HEATER_MRR_STUCK;
						}

						break;
					/* Generic Fault on the Heater Resistor Circuit(Feedback = 2) -> Fault Code.SubCode: 01.62 (Static) - 01.66 (Dynamic)*/
					case HEATER_CS_TRANS_ERR_GENERIC:
					case HEATER_CS_OFF_ERR_GENERIC:
					case HEATER_CS_ON_ERR_GENERIC:
						curAlertType = FAULTSRC_HEATER_CIRCUIT_KO;

						break;
					/* Heater Resistor Feedback = 0 (Feedback KO) -> Fault Code.SubCode: 01.61 (Static) - 01.65 (Dynamic)*/
					case HEATER_CS_TRANS_ERR_FEEDBACK_KO:
					case HEATER_CS_OFF_ERR_FEEDBACK_KO:
					case HEATER_CS_ON_ERR_FEEDBACK_KO:
						curAlertType = FAULTSRC_HEATER_FDB_KO;
						break;

					/* Heater Resistor not piloted but Feedback = 1 -> Fault Code.SubCode: 08.01 (Static) - 08.11 (Dynamic)*/
					case HEATER_CS_OFF_ERR_SC:
						/* If the Resistors Master Relay is closed, a consistency = 3 means
						* that the resistor relay is shorted or the resistor has leakage */
						if(genMRHeatr.resMrRele.curStatus == LOAD_ON)
						{
							curAlertType = FAULTSRC_HEATER_SHORTED;
						}
						else
						{
							curAlertType = FAULTSRC_HEATER_NOFAULT;
						}

						break;
					/* Heater Resistor is piloted but is not closed -> Fault Code.SubCode: 08.02 (Static) - 08.12 (Dynamic)
					 * Even if this fault is not hazardous it is managed on platform
					 * */
					case HEATER_CS_ON_ERR_OC:
						if(genMRHeatr.resMrRele.curStatus == LOAD_ON)
						{
							curAlertType = FAULTSRC_HEATER_OPEN;
						}

						break;

					default:
						break;
				}
			}
		}
		/* If Zero Crossing is not good */
		else
		{
			if((pModInfo->status != SFT_MODULE_FAULT) && (pModInfo->status != SFT_MODULE_IDLE) )
			{
				pModInfo->status = SFT_MODULE_RESET;
			}
		}

		/* State machine for the heater resistor faults */
		switch (pModInfo->status)
		{
			case SFT_MODULE_RESET:
				curAlertType = FAULTSRC_HEATER_NOFAULT;
				pModInfo->faultSrc = SFT_NO_FAULT_SRC;
				pModInfo->status = SFT_MODULE_IDLE;
				pHeaterRes->cnt = 0;
				break;

			case SFT_MODULE_IDLE:
			case SFT_MODULE_PROTECTION:
			case SFT_MODULE_MONITORING:
				if(curAlertType != FAULTSRC_HEATER_NOFAULT)
				{
					/* Set the timer with the timeout to the prefault */
					if((curAlertType == FAULTSRC_HEATER_SHORTED) ||(curAlertType == FAULTSRC_HEATER_MRR_STUCK))
					{
						pHeaterRes->cnt = pHeaterRes->genFltParams.prefaultTimeCC;
					}
					else
					{
						pHeaterRes->cnt = pHeaterRes->genFltParams.prefaultTimeCA;
					}
					/* If in auto-test mode, the counter is limited to the test timeout value */
					if ( (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
							&& (pHeaterRes->cnt > SFT_TEST_PREFAULT_TIMEOUT_100MS))
					{
						pHeaterRes->cnt = SFT_TEST_PREFAULT_TIMEOUT_100MS;
					}

					pModInfo->status = SFT_MODULE_WARNING;
				}
				else
				{
					genMRHeatr.bmpHtrChk |= (1 << pHeaterRes->resType);

					/* Checks if a test for checking the Resistors Master Relay is in progress */
					if(resMRCheckInProgr == TRUE)
					{
						pModInfo->status = SFT_MODULE_MONITORING;
						curAlertType = FAULTSRC_HEATER_MRR_STUCK;

					}
					else
					{
						pModInfo->status = SFT_MODULE_IDLE;
					}

				}
				pHeaterRes->monitorAlertType = curAlertType;
				pModInfo->faultSrc = curAlertType;

				break;

			case SFT_MODULE_WARNING:
				/* Reset status if the alert source has changed since the last cycle */
				if(curAlertType != pHeaterRes->monitorAlertType)
				{
					pModInfo->status = SFT_MODULE_IDLE;
					pHeaterRes->cnt = 0;
				}
				else
				{
					if(pHeaterRes->cnt == 0) /* Timer expired */
					{
						if((curAlertType == FAULTSRC_HEATER_SHORTED) ||(curAlertType == FAULTSRC_HEATER_MRR_STUCK))
						{
						  pHeaterRes->cnt = pHeaterRes->genFltParams.faultTimeCC;
						}
						else
						{
						  pHeaterRes->cnt = pHeaterRes->genFltParams.faultTimeCA;
						}

						/* Set the timer with timeout to fault */
						if ( (applReqFlags & SFT_FLAGS_TEST_TMR_REQ)
							&& (pHeaterRes->cnt > SFT_TEST_FAULT_TIMEOUT_100MS) )
						{
						/* Only if in auto test mode */
							pHeaterRes->cnt = SFT_TEST_FAULT_TIMEOUT_100MS;
						}

						pModInfo->status = SFT_MODULE_PREFAULT;
						pHeaterRes->fltRstTimer = GEN_FLT_RESET_TIMER_VALUE;
					}
				}


				break;

			case SFT_MODULE_PREFAULT:
				/* Reset status if the alert source has changed since the last cycle */
				if(curAlertType != pHeaterRes->monitorAlertType)
				{
					if(pHeaterRes->fltRstTimer == 0)
					{
						pModInfo->status = SFT_MODULE_IDLE;
					}
					else
					{
						pHeaterRes->fltRstTimer--;
					}
				}
				else
				{
					pHeaterRes->fltRstTimer = GEN_FLT_RESET_TIMER_VALUE;
				}

				if(pHeaterRes->cnt == 0) /* Timer expired */
				{
					pModInfo->status = SFT_MODULE_FAULT;
					PM_SFT_DR_Heater_ResetCheckMRR();
				}


				break;

			case SFT_MODULE_FAULT:
				/* Handle reset fault request */
				if (applReqFlags & SFT_FLAGS_RESET_REQ)
				{
					pModInfo->status = SFT_MODULE_RESET;
				}

				break;

			default:
				break;
		}

		if(pHeaterRes->cnt > 0)
		{
			pHeaterRes->cnt--;
		}

		/* If Motor MR is OFF, force HeatersMR OFF too */
		if (genMRHeatr.motMrRele.curStatus == LOAD_REG_OFF)
		{
			newHtrStatus = LOAD_REG_OFF;
		}
		else
		{
			/* Check if single pin is in place */
			switch(genMRHeatr.chkExec)
			{
			case 1:
				newHtrStatus = ( LOAD_CFG_MR_FORCE_DELAY | LOAD_CFG_MR_MULTI_2);
				break;

			case 2:
			case 4:
				newHtrStatus = ( LOAD_CFG_MR_FORCE_DELAY | LOAD_REG_OFF);
				break;

			case 3:
				newHtrStatus = ( LOAD_CFG_MR_FORCE_DELAY | LOAD_CFG_MR_MULTI_1);
				break;

			default:
				break;
			}
		}

		if (newHtrStatus != LOAD_NOT_FOUND)
		{
			LIB_IOM_UpdateLoadRequestByItem(LD_RES_MASTER_RELAY_ITM, newHtrStatus, newLoads->loads_ovr);
		}

		genMRHeatr.motMrRele.lastStatus = genMRHeatr.motMrRele.curStatus;

		SAFETY_DATA_UPDATE(genMRHeatr);

	}


}

/**
 *\n Controls if the Resistors Master Relay is to be checked.
 *\n The check is enabled at each closure of the Motor's Master Relay
 *\n
 *\return bool_t
 *\n
 *\retval FALSE		The check is NOT in progress
 *\retval TRUE		The check is in progress
 *
 *
 *
 */
static bool_t PM_SFT_DR_Heater_CheckMRR(void)
{
	/* As soon as the Motor Master Relay is closed, the check for the RMR comes enabled */
	if((genMRHeatr.motMrRele.lastStatus == LOAD_OFF) && (genMRHeatr.motMrRele.curStatus == LOAD_ON))
	{
		uint8_t tmpPilotType = LIB_IOM_GetLoadPilotType(genMRHeatr.motMrRele.pos);

		if (tmpPilotType == DIGITAL_OUTPUT_MULTI_L)
		{
			genMRHeatr.chkExec = 4;
			genMRHeatr.checkCnt = (SFT_MRR_CHECK_TIMEOUT - 3*SFT_SINGLEPIN_CHK_TIMEOUT_100MS);
		}
	}
	else if(genMRHeatr.motMrRele.curStatus == LOAD_OFF)
	{
		PM_SFT_DR_Heater_ResetCheckMRR();
	}


	if ( (genMRHeatr.bmpHtrCfg & genMRHeatr.bmpHtrChk) == genMRHeatr.bmpHtrCfg )
	{
		if(genMRHeatr.chkExec > 0)
		{
			if(genMRHeatr.checkCnt > 0)
			{
				genMRHeatr.checkCnt--;
			}
			else
			{
				genMRHeatr.chkExec--;
				if (genMRHeatr.chkExec > 0)
				{
					genMRHeatr.checkCnt = SFT_SINGLEPIN_CHK_TIMEOUT_100MS;
				}
				else
				{
					genMRHeatr.checkCnt = 0;
				}
			}
		}
		else
		{
			genMRHeatr.checkCnt = 0;
		}


		genMRHeatr.bmpHtrChk = 0;
	}

	return (genMRHeatr.chkExec > 0);
}

static void PM_SFT_DR_Heater_ResetCheckMRR(void)
{
	genMRHeatr.chkExec = 0;
	genMRHeatr.checkCnt = 0;
}

bool_t PM_SFT_DR_Heater_isResMRCheckInProgr(void)
{
	return (genMRHeatr.chkExec > 0);
}
