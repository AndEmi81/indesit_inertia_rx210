/*
Copyright (c) 2004-2013, Indesit Company
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	ProductSafety
\n	This module contains Safety Supervisor and Safety Action manager modules
\n
\file		LIB_ProductSafety.c
\ingroup        LIB_ProductSafety
\date	    2013-03-24
\version	01.01
\author		 Alessandro Sabbatini
\n
\n
\par	History:

\n		01.01
\n		2013-03-24
\n		Alessandro Sabbatini
\n		changes
\n		- Add safety execution check flags in Safety Supervisor for safety modules and safety action manager
\n
\n 		01.00
\n		2013-01-17
\n		Alessandro Sabbatini
\n		changes
\n 			Creation
\n
*/

#define MODULE_BODY_PRODUCTSAFETY

/* ******** */
/* Includes */
/* ******** */

#include "StandardTypes.h"
#include "Config.h"
#include "LIB_ProductSafety.h"
#include "LIB_UTilities.h"
#include "LIB_ClassBVar.h"
#include "LIB_TaskMonitoring.h"

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

#define		SFT_ACTIONS_FULLBMP		((1ul << SFT_NUM_SAFETY_ACTIONS) - 1ul)

#if !defined(SFT_CUSTOM_FAULT_PRIO)
#define SFT_CUSTOM_FAULT_PRIO SFT_PRIO_NONE
#endif

/* ******** */
/* typedefs */
/* ******** */
typedef struct sft_safetyAction_status_s {
	sft_safety_action_bmp_t sftActionReq;
	sft_safety_action_bmp_t sftActionExec;
	sft_safety_action_bmp_t sftActionDone;
} sft_safetyAction_status_t;

typedef struct sft_supervisor_status_s
{
	sft_module_status_t moduleStatus[SFT_NUM_SAFETY_MODULES];
	sft_safety_action_bmp_t	sftApplReq;
	uint8_t status;
	uint8_t highFaultSrc;
	uint8_t reqFlags;
} sft_supervisor_status_t;

/* ******** */
/* Globals  */
/* ******** */
/** Safety Action Status */
static sft_safetyAction_status_t SafetyActionStatus;
/** Safety Supervisor Status - contains overall and per-module status */
static sft_supervisor_status_t SafetySupervisorStatus;
/** Safety Module Status Flag - status change notification, last item is used by supervisor
 *  Not necessary for safety */
static uint8_t SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES+1];

uint16_t	SafetyModuleTimerCntr[SFT_NUM_SAFETY_MODULES];

#endif

static uint8_t LIB_SFT_GetSafetyPriorityByFaultSrc(uint8_t faultSrcID, uint8_t *prio);
static uint8_t LIB_SFT_GetFaultSrcByModuleFaultSrc(uint8_t reqModule, uint8_t reqModFaultSrc);
static void LIB_SFT_SafetyActionAllReset(void);



/**
\n Safety Supervisor init module; initializes Safety Supervisor and modules

\param		none

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetySupervisorInit(void)
{
	uint8_t i;
	uint8_t tmpPrio;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	PLT_ASSERT((sizeof(SafetyModuleInitFuncs)/sizeof(SFT_SafetyModuleInit_t)) == SFT_NUM_SAFETY_MODULES )
	PLT_ASSERT((sizeof(SafetyModuleFuncs)/sizeof(SFT_SafetyModuleExec_t)) == SFT_NUM_SAFETY_MODULES )

	SAFETY_REGION_START();

	LIB_SFT_SafetySupervisorCommonCfgInit();

	SafetySupervisorStatus.sftApplReq = 0;

	if (LIB_SFT_GetSafetyPriorityByFaultSrc(SFT_FLT_SUPERVISOR_BAD_SETTING, &tmpPrio) != SF_OK)
	{
		SafetySupervisorStatus.status = SFT_MODULE_FAULT;
		SafetySupervisorStatus.highFaultSrc = SFT_FLT_SUPERVISOR_BAD_SETTING;
		SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES] = TRUE;
	}
	else
	{
		SafetySupervisorStatus.status = SFT_MODULE_IDLE;
		SafetySupervisorStatus.highFaultSrc = SFT_NO_FAULT_SRC;
		SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES] = FALSE;
	}

	for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
	{
		SafetySupervisorStatus.moduleStatus[i].status = SafetySupervisorStatus.status;
		SafetySupervisorStatus.moduleStatus[i].faultSrc = SFT_NO_FAULT_SRC;
		SafetyModuleStatusFlags[i] = SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES];

		if ((SafetySupervisorStatus.status != SFT_MODULE_FAULT) && (SafetyModuleInitFuncs[i] != NULL))
		{
			/* Initialization module - may not be used in all modules! */
			SafetyModuleInitFuncs[i](&SafetySupervisorStatus.moduleStatus[i]);
			SafetyModuleTimerCntr[i] = 0;
		}
	}

	SAFETY_DATA_UPDATE(SafetySupervisorStatus);
	SAFETY_REGION_END();

#endif

}

/**
\n Safety Supervisor core module; it handles call and check of each safety module
\n defined according to priority set.

\param		newLoads	pointer to a structure containing all loads request/override pointers

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetySupervisor(iom_loads_struct_t *newLoads)
{

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t chkSafetyExec = 0;

	LIB_PRODUCTSAFETY_TASK__MONITOR_IN();

	if (!(Main_IsRemoteEnabled() && SAFETY_CHECK_ISDISABLED()))
	{
		uint8_t tmpBkpOvr[N_MAXLOADS];
#if  defined(CONFIG_NUM_EXTDLOADS)
		uint8_t tmpExtBkpOvr[CONFIG_NUM_EXTDLOADS][CONFIG_EXT_LOAD_PARAMDATA_SZ];
#endif
		uint8_t i;
		uint8_t curPrio, newPrio;
		uint8_t  curFaultSrc = SFT_NO_FAULT_SRC, curStatus = SFT_MODULE_IDLE;
		sft_module_status_t tmpStatus;
		sft_safety_action_bmp_t tmpSafetyActionsBmp = SafetySupervisorStatus.sftApplReq;
		sft_safety_action_bmp_t tmpSafetyActionsClrBmp = 0;

		curPrio = SFT_PRIO_NONE;
		SAFETY_DATA_CHECK(SafetySupervisorStatus);

		if ((SafetySupervisorStatus.status != SFT_MODULE_FAULT) ||
			(SafetySupervisorStatus.highFaultSrc != SFT_FLT_SUPERVISOR_BAD_SETTING))
		{
			uint8_t tmpSftReqFlags = SafetySupervisorStatus.reqFlags;

			/* If there are any safety action maked in SFT_ACTION_RESET_CHK_MASK still pending,
			 * filter SFT_FLAGS_RESET_REQ flag - reset action is allowed only at end of all safety procedures.
			 */
			if (LIB_SFT_GetPendingSafetyActionBmp() & SFT_ACTION_RESET_CHK_MASK)
			{
				tmpSftReqFlags &= ~SFT_FLAGS_RESET_REQ;
			}

			/* Execution of all safety modules */
			for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
			{
				uint8_t newFaultSrc;
				uint8_t newStatus;

				SAFETY_REGION_START();

				memcpy(&tmpStatus, &SafetySupervisorStatus.moduleStatus[i], sizeof(tmpStatus));
				memcpy(tmpBkpOvr, newLoads->loads_ovr, sizeof(tmpBkpOvr));
#if  defined(CONFIG_NUM_EXTDLOADS)
				memcpy(tmpExtBkpOvr, newLoads->extdloads_ovr, sizeof(tmpExtBkpOvr));
#endif
				if(SafetyModuleFuncs[i] != 0)
				{
					/* Execute current Safety Module */
					SafetySupervisorStatus.moduleStatus[i].chkExec = (SFT_CHKEXEC_ENTER | SFT_CHKEXEC_EXIT);
					SafetyModuleFuncs[i](newLoads, &SafetySupervisorStatus.moduleStatus[i], tmpSftReqFlags);
				}

				if ( (SafetySupervisorStatus.moduleStatus[i].chkExec == 0) && (chkSafetyExec == i))
				{
					chkSafetyExec++;
				}

				SafetySupervisorStatus.moduleStatus[i].chkExec = 0;

				newStatus = SafetySupervisorStatus.moduleStatus[i].status;
				newFaultSrc = LIB_SFT_GetFaultSrcByModuleFaultSrc(i, SafetySupervisorStatus.moduleStatus[i].faultSrc);

				if (SafetyModuleTimer[i] != 0)
				{
					SafetyModuleTimerCntr[i] = SafetyModuleTimer[i](&SafetySupervisorStatus.moduleStatus[i]);
				}

				/* If module is in idle state, any Loads Override set will be discarded */
				if (newStatus == SFT_MODULE_IDLE)
				{
					SafetySupervisorStatus.moduleStatus[i].faultSrc = SFT_NO_FAULT_SRC;
				}

				/* If priority of a fault is not present...
				 * choose! default or go to Setting file fault state? Depends on SFT_DEFAULT_PRIORITY */
				if (LIB_SFT_GetSafetyPriorityByFaultSrc(newFaultSrc, &newPrio) != SF_OK)
				{
#if defined(SFT_DEFAULT_PRIORITY)
					newPrio = SFT_DEFAULT_PRIORITY;
#else

					LIB_SFT_GetSafetyPriorityByFaultSrc(SFT_FLT_SUPERVISOR_BAD_SETTING, &newPrio);
					curStatus = SFT_MODULE_FAULT;
					curFaultSrc = SFT_FLT_SUPERVISOR_BAD_SETTING;

#endif
				}

				if ((newStatus != SFT_MODULE_IDLE) && (newPrio == SFT_PRIO_NONE))
				{
					/* Revert back all load overrides */
					memcpy(newLoads->loads_ovr, tmpBkpOvr, sizeof(tmpBkpOvr));
	#if  defined(CONFIG_NUM_EXTDLOADS)
					memcpy(newLoads->extdloads_ovr, tmpExtBkpOvr, sizeof(tmpExtBkpOvr));
	#endif

					/* Prevent going to fault state */
					if (newStatus == SFT_MODULE_FAULT)
					{
						newStatus = SFT_MODULE_PREFAULT;
						SafetySupervisorStatus.moduleStatus[i].status = SFT_MODULE_PREFAULT;
					}

				}

				SAFETY_DATA_UPDATE(SafetySupervisorStatus);

				if (newStatus != tmpStatus.status)
				{
					SafetyModuleStatusFlags[i] = TRUE;
				}

				/* New priority lower than current - previous overrides set take precedence than new ones */
				if ((newPrio != SFT_PRIO_NONE) && ((newStatus > curStatus) ||
					((newStatus == curStatus) && (newPrio > curPrio))))
				{

					curPrio = newPrio;
					curStatus = newStatus;
					curFaultSrc = newFaultSrc;
				}

				if (SafetySupervisorStatus.status != curStatus)
				{
					SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES] = TRUE;
				}

				SAFETY_REGION_END();

			}
		}
		else
		{
			curStatus = SFT_MODULE_FAULT;
			curFaultSrc = SFT_FLT_SUPERVISOR_BAD_SETTING;
			chkSafetyExec = SFT_NUM_SAFETY_MODULES;
		}

#if defined(SFT_NUM_STATUS_ACTIONS)

		for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
		{
			uint8_t newStatus;
			uint8_t curModule = i;

			newPrio = SFT_PRIO_NONE;

			if ((SafetySupervisorStatus.status == SFT_MODULE_FAULT) &&
				(SafetySupervisorStatus.highFaultSrc == SFT_FLT_SUPERVISOR_BAD_SETTING))
			{
				newStatus = SFT_MODULE_FAULT;
				curModule = 0xff;
				LIB_SFT_GetSafetyPriorityByFaultSrc(SFT_FLT_SUPERVISOR_BAD_SETTING, &newPrio);
			}
			else
			{
				uint8_t tmpFaultSrc = LIB_SFT_GetFaultSrcByModuleFaultSrc(i, SafetySupervisorStatus.moduleStatus[i].faultSrc);

				newStatus = SafetySupervisorStatus.moduleStatus[i].status;
				LIB_SFT_GetSafetyPriorityByFaultSrc(tmpFaultSrc, &newPrio);
			}

			/* If module is not in IDLE state, it may request some safety actions according to configuration */
			if (newStatus != SFT_MODULE_IDLE)
			{
				uint8_t tmpExecuteItem = SFT_ID_ANY;
				uint8_t j;

				/* Check if a safety action has to be executed */
				for (j = 0; (tmpExecuteItem == SFT_ID_ANY) && (j < SFT_NUM_STATUS_ACTIONS); j++)
				{
					uint8_t tmpModuleID = SafetyModulesStatusActions[j].moduleID;
					uint8_t tmpFaultSrc = SafetyModulesStatusActions[j].faultSrcID;

					if (SafetyModulesStatusActions[j].moduleStatus == newStatus)
					{
						/* SF_ID_ANY wild card case for module ID */
						if (tmpModuleID == SFT_ID_ANY)
						{
							/* SF_ID_ANY wild card case for module fault source */
							if ( (tmpFaultSrc == SFT_ID_ANY) ||
									( (SafetySupervisorStatus.highFaultSrc == SFT_FLT_SUPERVISOR_BAD_SETTING) &&
											(tmpFaultSrc == SFT_FLT_SUPERVISOR_BAD_SETTING) ) )
							{
								/* Overrides application request by any safety action requested here */
								tmpExecuteItem = j;
							}
						}
						else
						{
							if (tmpModuleID == curModule)
							{
								/* SF_ID_ANY wild card case for module fault source or current fault source matches */
								if ((tmpFaultSrc == SFT_ID_ANY) ||
									(tmpFaultSrc == SafetySupervisorStatus.moduleStatus[curModule].faultSrc))
								{
									/* Overrides application request by any safety action requested here */
									tmpExecuteItem = j;
								}
							}
						}
					}

				}

				/* If a safety action is found and priority is not null, melt it with others previously set */
				if ( (tmpExecuteItem != SFT_ID_ANY) && (newPrio != SFT_PRIO_NONE))
				{
					tmpSafetyActionsClrBmp |= SafetyModulesStatusActions[tmpExecuteItem].sfyActionReqClrBmp;
					tmpSafetyActionsBmp |= SafetyModulesStatusActions[tmpExecuteItem].sfyActionReqBmp;
				}


			}
		}
#endif

		tmpSafetyActionsBmp &= ~tmpSafetyActionsClrBmp;

		/* Merge tmpSafetyActionsClrBmp and tmpSafetyActionsBmp
		 * All of these actions must be kept to avoid unwanted resets */
		tmpSafetyActionsClrBmp |= tmpSafetyActionsBmp;

		SAFETY_REGION_START();
		SafetySupervisorStatus.status = curStatus;
		SafetySupervisorStatus.highFaultSrc = curFaultSrc;
		SafetySupervisorStatus.reqFlags = SFT_FLAGS_NONE;
		SAFETY_DATA_UPDATE(SafetySupervisorStatus);
		SAFETY_REGION_END();

		/* Call Safety Action Manager */
		tmpStatus.chkExec = (SFT_CHKEXEC_ENTER | SFT_CHKEXEC_EXIT);
		LIB_SFT_SafetyActionMgr(tmpSafetyActionsBmp, tmpSafetyActionsClrBmp, newLoads, &tmpStatus);
		if (tmpStatus.chkExec  == 0)
		{
			chkSafetyExec++;
		}

	}
	else
	{
		chkSafetyExec = (SFT_NUM_SAFETY_MODULES+1);
	}

	if (chkSafetyExec == (SFT_NUM_SAFETY_MODULES+1))
	{
		LIB_PRODUCTSAFETY_TASK__MONITOR_OUT();
	}

#endif

}

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

/**
\n Safety Actions manager core module; it handles call of safety actions as
\n their request.

\param		newSafetyActionBmp			Bitmap of new safety action requests
\param		keepDoneSafetyActionBmp		Bitmap of safety action not requested but need to be kept (e.g. application requests while safety ones are in action)
\param		newLoads					pointer to a structure containing all loads request/override pointers

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetyActionMgr(sft_safety_action_bmp_t newSafetyActionBmp, sft_safety_action_bmp_t keepDoneSafetyActionBmp, iom_loads_struct_t *newLoads, sft_module_status_t *pChkExec)
{
uint32_t i;
static uint8_t initRequest = _DISABLE_;
static uint8_t localRet;


	/*NTBI Se SW relativo alla sicurezza è nello stato "DISABLE":] */
	/*NTBI	return	*/
	/*NTBI else:	*/
	/*NTBI	niente	*/

	LIB_SFT_EnterSafetyModule(pChkExec);

	/*	la bit-map che indica il set di nuove azioni di sicurezza da fare viene subito	*/
	/*	notificata come "richiesta"														*/
	SafetyActionStatus.sftActionReq = (newSafetyActionBmp & SFT_ACTIONS_FULLBMP);

	/*	Per ciascuno dei bits che indicano il pilotaggio di un carico:	*/
	for (i=0; i<SFT_NUM_SAFETY_ACTIONS; i++)
	{
		sft_safety_action_bmp_t tmpCurrSftAction = ((sft_safety_action_bmp_t)(1) << i);

		/*	Se è stata richiesta almeno una procedura safety di pilotaggio carico:	*/
		if (SafetyActionStatus.sftActionReq & tmpCurrSftAction)
		{
			/*	Se la corrispondente azione di sicurezza è stata inizializzata:	*/
			/*	il relativo bit risulta settato; NON si ripete l'esecuzione		*/
			/*	della init														*/
			if (SafetyActionStatus.sftActionExec & ((sft_safety_action_bmp_t)(1) << i))
			{
				initRequest = _DISABLE_;
			}
			/*	Se la corrispondente azione di sicurezza NON è stata inizializzata:	*/
			/*	il relativo bit risulta nullo; si esegue la init in modo one-shot	*/
			else
			{
				initRequest = _ENABLE_;
				/*	La init della azione di sicurezza corrisponde alla condizione 	*/
				/*	di START per la esecuzione della stessa							*/
				SafetyActionStatus.sftActionExec |= tmpCurrSftAction;
			}

			/*	Esecuzione vera e propria della singola procedura individuata dalla bit-map	*/
			if( 0 != SafetyActionFuncs[i] ){
				localRet = SafetyActionFuncs[i](newLoads, initRequest);
			}else{
				localRet = TRUE;
			}
			/*	Se Check ritorno della procedura NON è OK:	*/
			if (localRet != TRUE)
				/*	Si interrompe immediatamente l'esecuzione del ciclo for:	*/
				break;
			else
			{
				SafetyActionStatus.sftActionDone |= tmpCurrSftAction;
			}
		}
		/*	Se NON è stata richiesta alcuna procedura safety di pilotaggio carico:	*/
		else
		{
			/* Viene azzerata nel caso non sia presente nella keepDoneSafetyActionBmp */
			if ((keepDoneSafetyActionBmp  & tmpCurrSftAction) == 0)
			{
				SafetyActionStatus.sftActionExec &= ~tmpCurrSftAction;
				SafetyActionStatus.sftActionDone &= ~tmpCurrSftAction;
			}
		}

	}

	LIB_SFT_LeaveSafetyModule(pChkExec);

}

void LIB_SFT_EnterSafetyModule(sft_module_status_t *sftData)
{
	sftData->chkExec ^= SFT_CHKEXEC_ENTER;
}


void LIB_SFT_LeaveSafetyModule(sft_module_status_t *sftData)
{
	sftData->chkExec ^= SFT_CHKEXEC_EXIT;
}

#endif	/* defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */

/**
\n Function to get safety action requests from application

\param		newApplSafetyReqBmp	Bitmap of safety actions requested

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetyActionRequest(sft_safety_action_bmp_t newApplSafetyReqBmp)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(SafetySupervisorStatus);
	SafetySupervisorStatus.sftApplReq = (newApplSafetyReqBmp & SFT_ACTIONS_FULLBMP);
	SAFETY_DATA_UPDATE(SafetySupervisorStatus);
	SAFETY_REGION_END();
#endif
}

/**
\n Function to get safety flag requests from application

\param		newApplSafetyFlagsBmp	Bitmap of safety flags requested

\return 	none


\author 	Alessandro Sabbatini
\date		Feb 13, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetyFlagsRequest(uint8_t newApplSafetyFlagsBmp)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(SafetySupervisorStatus);
	SafetySupervisorStatus.reqFlags = newApplSafetyFlagsBmp;
	SAFETY_DATA_UPDATE(SafetySupervisorStatus);
	SAFETY_REGION_END();
#endif
}


/**
\n Function that copies all safety modules status; fault sources is related to SafetyFaultSources
items (visible to Application) instead of internal fault source.

\param		[out] newSafetyStatus	Pointer to array where all safety modules status will be saved

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_GetAllSafetyFaultStatus(sft_module_faults_t *newSafetyStatus)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t i;

	for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
	{
		(void) LIB_SFT_GetSafetyModFaultStatus(i, &newSafetyStatus[i]);
	}
#endif


}


/**
\n Function that copies safety modules status of changed modules each call, and reset changed status of module shown; 
fault sources is related to SafetyFaultSources items (visible to Application) instead of internal fault source.

\param		[out] newSafetyStatus	Pointer to array where all safety modules status will be saved

\return 	FALSE					if there are no changes found
\return 	TRUE					if there is a fault status change - update it


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

bool_t LIB_SFT_GetResetChangedFaultStatus(sft_module_faults_t *newSafetyStatus)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	uint8_t i;
	bool_t foundChange = FALSE;


	for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
	{
		if (SafetyModuleStatusFlags[i])
		{
			(void) LIB_SFT_GetSafetyModFaultStatus(i, newSafetyStatus);
			SafetyModuleStatusFlags[i] = 0;
			foundChange = TRUE;
		}
	}

	if (!foundChange)
	{
		SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES] = FALSE;
	}

	return foundChange;

#else
	return FALSE;
#endif

}


/**
\n Function that resets status change notifications

\param		none

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_ResetStatusChangeNotification(void)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t i;

	for (i=0; i < (SFT_NUM_SAFETY_MODULES+1); i++)
	{
		SafetyModuleStatusFlags[i] = FALSE;
	}
#endif
}


/**
\n Function that copies safety Supervisor status

\param		[out] newSafetyStatus	Pointer to array where all safety modules status will be saved

\return 	none


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_SFT_GetOverallFaultStatus(sft_module_faults_t *newSafetyStatus)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t tmpStatus = SFT_MODULE_IDLE;
	uint8_t tmpFaultSrc = SafetySupervisorStatus.highFaultSrc;
	uint8_t tmpIsChanged = SafetyModuleStatusFlags[SFT_NUM_SAFETY_MODULES];
	uint8_t tmpFaultCode = 0;
	uint8_t tmpFaultSubCode = 0;

	if (LIB_SFT_GetSafetyFaultCode(tmpFaultSrc, &tmpFaultCode, &tmpFaultSubCode) == SF_OK)
	{
		tmpStatus = SafetySupervisorStatus.status;
	}

	newSafetyStatus->status = (tmpStatus & 0x7f) | ((tmpIsChanged != 0) ? 0x80 : 0 );
	newSafetyStatus->fault.code = tmpFaultCode;
	newSafetyStatus->fault.subCode = tmpFaultSubCode;

#else
	newSafetyStatus->status = SFT_MODULE_IDLE;
	newSafetyStatus->fault.code = 0;
	newSafetyStatus->fault.subCode = 0;

#endif

}


/**
\n Function that gives status of a specific fault source

\param		[in] faultSrcItem	Fault source item from array SafetyFaultSources

\return 	status				Status of requested fault source; SFT_MODULE_IDLE if fault source has no errors


\author 	Alessandro Sabbatini
\date		Feb 111, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_SFT_GetFaultSourceStatus(uint8_t faultSrcItem)
{
	uint8_t tmpCurStatus = SFT_MODULE_IDLE;
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	if ((faultSrcItem == SFT_FLT_SV_NO_SETTING) && (faultSrcItem == SafetySupervisorStatus.highFaultSrc))
	{
		tmpCurStatus = SafetySupervisorStatus.status;
	}

	if ((faultSrcItem > SFT_FLT_SV_NO_SETTING) && (faultSrcItem < SFT_NUM_FAULT_SOURCES))
	{
		if (SafetyFaultSources[faultSrcItem].moduleFaultSrcID ==
			SafetySupervisorStatus.moduleStatus[SafetyFaultSources[faultSrcItem].moduleID].faultSrc)
		{
			tmpCurStatus = SafetySupervisorStatus.moduleStatus[SafetyFaultSources[faultSrcItem].moduleID].status;
		}
	}

#endif
	return tmpCurStatus;

}

/**
\n Function that gives status of a specific fault source

\param		 [in] faultSrcItem	Fault source item from array SafetyFaultSources
\param		[out] faultCode		Fault code of fault source requested (if exists)
\param		[out] faultSubCode	Fault sub code of fault source requested (if exists)

\return		Returns if fault source is found or not

\return 	== 0				if fault source is found
\return 	!= 0				if fault source is not found - result codes are unchanged

\author 	Alessandro Sabbatini
\date		Feb 111, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_SFT_GetSafetyFaultCode(uint8_t faultSrcItem, uint8_t *faultCode, uint8_t *faultSubcode)
{
	uint8_t tmpNotFound = TRUE;
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

	if (faultSrcItem < SFT_NUM_FAULT_SOURCES)
	{
#if defined(SFT_NUM_CUSTOM_FAULTCODES)
		uint8_t i;

		for (i = 0; (tmpNotFound && (i< SFT_NUM_CUSTOM_FAULTCODES)) ; i++)
		{
			/* Looks for a custom fault code */
			if ( (SafetyCustomCodeFaultSrc[i] == faultSrcItem) && (SafetyCustomCodeFuncs[i] != 0))
			{
				/* If function returns TRUE, values are updated */
				if (SafetyCustomCodeFuncs[i](faultCode, faultSubcode) == FALSE)
				{
					faultCode = 0;
					faultSubcode = 0;
				}
				tmpNotFound = FALSE;
			}
		}
#endif

		if (tmpNotFound)
		{
			*faultCode = SafetyFaultSources[faultSrcItem].faultCode;
			*faultSubcode = SafetyFaultSources[faultSrcItem].faultSubcode;
			tmpNotFound = FALSE;
		}
	}

#endif
	return tmpNotFound;

}

/**
\n Function that copies fault code and status of a given safety module

\param		[in] safetyModule		Safety module ID to get status
\param		[out] newSafetyStatus	Pointer to array where all modules status will be saved

\return 	== 0 if success
\return 	!= 0 if an error occurred

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_SFT_GetSafetyModFaultStatus(uint8_t safetyModuleID, sft_module_faults_t *newSafetyStatus)
{
	uint8_t retVal = 0xff;

#if defined CONFIG_LIB_PRODUCTSAFETY_SUPPORT
	sft_module_status_t tmpSafetyStatus;

	if (0 == LIB_SFT_GetSafetyModStatus(safetyModuleID, &tmpSafetyStatus))
	{
		uint8_t tmpFaultCode = 0;
		uint8_t tmpFaultSubCode = 0;

		if (LIB_SFT_GetSafetyFaultCode(tmpSafetyStatus.faultSrc, &tmpFaultCode, &tmpFaultSubCode) == SF_OK)
		{
			newSafetyStatus->status = tmpSafetyStatus.status | ((SafetyModuleStatusFlags[safetyModuleID] != 0) ? 0x80 : 0);
			newSafetyStatus->fault.code = tmpFaultCode;
			newSafetyStatus->fault.subCode = tmpFaultSubCode;
			retVal = 0;
		}

	}

#endif

	return retVal;
}

/**
\n Function that copies fault source and status of a given safety module

\param		[in] safetyModule		Safety module ID to get status
\param		[out] newSafetyStatus	Pointer to array where all modules status will be saved;
									FaultSrc field is get from SafetyFaultSources, not from module itself.

\return 	== 0 if success, newSafetyStatus has been updated
\return 	!= 0 if an error occurred

\author 	Alessandro Sabbatini
\date		Mar 08, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

uint8_t LIB_SFT_GetSafetyModStatus(uint8_t safetyModuleID, sft_module_status_t *newSafetyStatus)
{
	uint8_t retVal = 0xff;

#if defined CONFIG_LIB_PRODUCTSAFETY_SUPPORT

	if (SafetySupervisorStatus.highFaultSrc == SFT_FLT_SUPERVISOR_BAD_SETTING)
	{
		newSafetyStatus->status = SFT_MODULE_FAULT;
		newSafetyStatus->faultSrc = SFT_FLT_SUPERVISOR_BAD_SETTING;
		retVal = 0;
	}
	else if (safetyModuleID < SFT_NUM_SAFETY_MODULES)
	{
		newSafetyStatus->status = SafetySupervisorStatus.moduleStatus[safetyModuleID].status;
		newSafetyStatus->faultSrc = LIB_SFT_GetFaultSrcByModuleFaultSrc(safetyModuleID, SafetySupervisorStatus.moduleStatus[safetyModuleID].faultSrc);
		retVal = 0;
	}
	else
	{
		/* Do nothing */
	}
#endif

	return retVal;
}

/**
\n Function that return which safety actions are pending

\param		none

\return 	== 0 		if all safety actions have been completed
\return 	!= 0 		A bitmap of pending safety actions

\author 	Alessandro Sabbatini
\date		Feb 15, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
sft_safety_action_bmp_t LIB_SFT_GetPendingSafetyActionBmp(void)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	return ((SafetyActionStatus.sftActionReq ^ SafetyActionStatus.sftActionDone) & ( SafetyActionStatus.sftActionReq & SFT_ACTIONS_FULLBMP));
#else
	return 0;
#endif
}

/**
\n Function that return which safety actions are under request

\param		none

\return		sft_safety_action_bmp_t		Bitmap of current safety actions requested

\author 	Francesco Barletta
\date		Feb 19, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Francesco Barletta
*/

sft_safety_action_bmp_t LIB_SFT_GetRequestSafetyActionBmp(void)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	return ( SafetyActionStatus.sftActionReq & SFT_ACTIONS_FULLBMP );
#else
	return 0;
#endif
}

/**
\n Function that return which safety actions are pending

\param		none

\return 	== 0 		if all safety actions have been completed
\return 	!= 0 		A bitmap of pending safety actions

\author 	Alessandro Sabbatini
\date		Feb 15, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
sft_safety_action_bmp_t LIB_SFT_GetSafetyActionDoneBmp(void)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	return (SafetyActionStatus.sftActionDone & SFT_ACTIONS_FULLBMP);
#else
	return 0;
#endif
}

/**
\n Function that return which safety actions are executing

\param		none

\return 	== 0 		if no safety actions are executing
\return 	!= 0 		A bitmap of executing safety actions

\author 	Alessandro Sabbatini
\date		May 14, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
sft_safety_action_bmp_t LIB_SFT_GetSafetyActionExecBmp(void)
{
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	return (SafetyActionStatus.sftActionExec & SFT_ACTIONS_FULLBMP);
#else
	return 0;
#endif
}

/**
\n Function that return module's safety priority to itself
\n ** This function must be called within a safety module only ** outside this scope it doesn't make sense!

\param		[in] mySftData		Pointer to module's safety status
\param		[in] newMyFaultSrc	New fault source to get priority; if 0 current fault source is used

\return 	uint8_t 			Priority value of current/new fault source

\author 	Alessandro Sabbatini
\date		May 14, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
uint8_t LIB_SFT_GetMySafetyPriority( sft_module_status_t *mySftData, uint8_t newMyFaultSrc)
{
	uint8_t	tmpRes = 0;
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	uint8_t tmpModFaultSrc = newMyFaultSrc;

	if ((tmpModFaultSrc == 0) && (mySftData->faultSrc != SFT_NO_FAULT_SRC)
			&& (mySftData->status != SFT_MODULE_IDLE))
	{
		tmpModFaultSrc = mySftData->faultSrc;
	}

	if (tmpModFaultSrc != 0)
	{
		uint8_t i;

		for (i = 0; i < SFT_NUM_SAFETY_MODULES; i++)
		{
			if (mySftData == &SafetySupervisorStatus.moduleStatus[i])
			{
				uint8_t tmpFaultSrc = LIB_SFT_GetFaultSrcByModuleFaultSrc(i, tmpModFaultSrc);
				LIB_SFT_GetSafetyPriorityByFaultSrc(tmpFaultSrc, &tmpRes);
			}
		}
	}

#endif

	return tmpRes;
}

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)


/**
\n Function that return Safety Priority of a given Fault source

\param		[in] faultSrcID		Fault source ID
\param		[out] prio			Priority value of current fault source

\return		uint8_t				Error code of requested operation - SF_OK if everything goes fine

\author 	Alessandro Sabbatini
\date		Feb 19, 2013
\version	1.0.0

\attention	Behaviour when SFT_NUM_CUSTOM_FAULTCODES is set:
			When a fault code+subcode is not found, it is considered as non-safety.
			In this case, this fault is searched in Application fault priorities:
			if found and its priority is SFT_PRIO_NONE (fault disabled), this priority is used,
			otherwise (not found or found with any other priority value) fault priority used is SFT_CUSTOM_FAULT_PRIO.


\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

static uint8_t LIB_SFT_GetSafetyPriorityByFaultSrc(uint8_t faultSrcID, uint8_t *prio)
{
	uint8_t resValue = SF_OK;
	uint8_t tmpPrio = SFT_PRIO_NONE;
	uint8_t tmpCode = 0, tmpSubcode = 0;


	/* No return function check is needed - if not found,will return SFT_PRIO_NONE */
	(void) LIB_SFT_GetSafetyFaultCode(faultSrcID, &tmpCode, &tmpSubcode);

	if ((tmpCode != 0) || (tmpSubcode != 0))
	{
		resValue = LIB_SEF_GetFaultPriority(FALSE, tmpCode, tmpSubcode, &tmpPrio);
		if ( SF_OK == resValue)
		{
			*prio = LIB_SEF_FaultPriorityField(tmpPrio);
		}
		else
		{
#if defined(SFT_NUM_CUSTOM_FAULTCODES)
			uint8_t i;

			for (i = 0; ((SF_OK != resValue) && (i< SFT_NUM_CUSTOM_FAULTCODES)) ; i++)
			{
				/* Looks for a custom fault code */
				if (SafetyCustomCodeFaultSrc[i] == faultSrcID)
				{
					tmpPrio = SFT_CUSTOM_FAULT_PRIO;

					/* Find if custom fault is defined in Application fault priorities table
					 * if so, consider it with a predefined priority - it's not safety related! */
					if (SF_OK == LIB_SEF_GetFaultPriority(TRUE, tmpCode, tmpSubcode, &tmpPrio))
					{
						if (LIB_SEF_FaultPriorityField(tmpPrio) != SFT_PRIO_NONE)
						{
							tmpPrio = SFT_CUSTOM_FAULT_PRIO;
						}
					}

					*prio = tmpPrio;
					resValue = SF_OK;
				}
			}
#endif
		}
	}


	return resValue;
}

#endif

/**
\n Function that return global Fault Source from a specific module fault source

\param		[in] reqModule			Module ID
\param		[in] reqModFaultSrc		Module Fault source ID

\return		uint8_t				Global Fault Source ID - SFT_ID_ANY if not found

\author 	Alessandro Sabbatini
\date		Feb 19, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

static uint8_t LIB_SFT_GetFaultSrcByModuleFaultSrc(uint8_t reqModule, uint8_t reqModFaultSrc)
{
	uint8_t i, tmpResVal = SFT_ID_ANY;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
	for (i=0;(tmpResVal == SFT_ID_ANY) && (i < SFT_NUM_FAULT_SOURCES); i++)
	{
		if (SafetyFaultSources[i].moduleFaultSrcID == reqModFaultSrc)
		{
			if ( ((SafetyFaultSources[i].moduleID == SFT_ID_ANY) && (reqModFaultSrc == 0))
					|| (SafetyFaultSources[i].moduleID == reqModule))
			{
				tmpResVal = i;
			}
		}
	}
#endif

	return tmpResVal;
}

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

/**
\n Function that reset Safety Actions status - used to restart

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Feb 19, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

static void LIB_SFT_SafetyActionAllReset(void)
{


	SafetyActionStatus.sftActionExec = 0;
	SafetyActionStatus.sftActionDone = 0;

}


/**
\n Function that reset Safety Actions status - used to restart

\param		none

\return		none

\author 	Alessandro Sabbatini
\date		Feb 19, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_SFT_SafetyActionBmpReset(sft_safety_action_bmp_t reqResetActionBmp)
{


	SafetyActionStatus.sftActionDone &= ~reqResetActionBmp;
	SafetyActionStatus.sftActionExec &= ~reqResetActionBmp;


}

#endif

