/**
\par	ProductSafety
\n	This module contains Safety Supervisor and Safety Action Mgr
\n
\file		LIB_ProductSafety.h
\ingroup        LIB_ProductSafety
\date	    2013-01-17
\version	01.00.00
\author		 Alessandro Sabbatini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		2013-01-17
\n		Alessandro Sabbatini
\n		changes
\n 			Creation
\n
*/

#ifndef _LIB_PRODUCTSAFETY_H_
#define _LIB_PRODUCTSAFETY_H_

#include "Config.h"
#include "LIB_ProductSafetyExp.h"
#include "LoadsRequest.h"

#define		SFT_CHKEXEC_ENTER		0x5C
#define		SFT_CHKEXEC_EXIT		0xA3

/* Pointer to get timer function prototype for each Safety Module */
typedef uint16_t (* SFT_SafetyModuleTimer_t) (sft_module_status_t *status);
/* Pointer to function prototype for each Safety Module */
typedef void (* SFT_SafetyModuleExec_t) (iom_loads_struct_t *newLoads, sft_module_status_t *status, uint8_t applReqFlags);
/* Pointer to function prototype for each Safety Module Init */
typedef void (* SFT_SafetyModuleInit_t) (sft_module_status_t *status);
/* Pointer to function prototype for each Safety Action */
typedef uint8_t (* SFT_SafetyActionExec_t) (iom_loads_struct_t *newLoads, uint8_t status);
/* Pointer to function prototype for each custom Fault code functions
 * This function returns TRUE if faultcode and faultSubcode are updated */
typedef bool_t (* SFT_SafetyCustomCode_t) (uint8_t *faultCode, uint8_t *faultSubcode);

/* Fault Source map code */
typedef struct sft_faultSrc_s {
	uint8_t moduleID;
	uint8_t moduleFaultSrcID;
	uint8_t faultCode;
	uint8_t faultSubcode;
} sft_faultSrc_t;

/* Module and Fault Source safety Actions table.
 * This configuration let safety action to be executed depending on module,
 * fault source and related status.
 *
 * There is a wildcard value (SFT_ID_ANY) for Module ID and eventually Fault Source ID
 * to configure an action valid for all cases - it should be used as last option
 * otherwise subsequent items will be discarded.
 *
 */
typedef struct sft_safetyModulesActions_s {
	uint8_t moduleID;
	uint8_t faultSrcID;
	uint8_t moduleStatus;
	sft_safety_action_bmp_t sfyActionReqBmp;
	sft_safety_action_bmp_t sfyActionReqClrBmp;
} sft_safetyModulesActions_t;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

#if !defined(_PRODUCTSAFETY_CFG_)
/* Function array of Safety Modules configured - each item must respect the
 * enum SafetyModuleID list.
 */
extern const SFT_SafetyModuleInit_t SafetyModuleInitFuncs[SFT_NUM_SAFETY_MODULES];
extern const SFT_SafetyModuleExec_t SafetyModuleFuncs[SFT_NUM_SAFETY_MODULES];
extern const SFT_SafetyModuleTimer_t SafetyModuleTimer[SFT_NUM_SAFETY_MODULES];

/* Function array of Safety Actions configured - each item must respect the
 * enum SafetyActionID list.
 */
extern const sft_faultSrc_t SafetyFaultSources[SFT_NUM_FAULT_SOURCES];
extern const SFT_SafetyActionExec_t SafetyActionFuncs[SFT_NUM_SAFETY_ACTIONS];

#if defined(SFT_NUM_STATUS_ACTIONS)
extern const sft_safetyModulesActions_t SafetyModulesStatusActions[SFT_NUM_STATUS_ACTIONS];
#endif

#if defined(SFT_NUM_CUSTOM_FAULTCODES)
extern const uint8_t SafetyCustomCodeFaultSrc[SFT_NUM_CUSTOM_FAULTCODES];
extern const SFT_SafetyCustomCode_t SafetyCustomCodeFuncs[SFT_NUM_CUSTOM_FAULTCODES];
#endif

#endif


/* Function prototype for a custom common init configuration good for all safety modules
 * This function MUST be defined in configuration file! If not used, it has to be empty. */
void LIB_SFT_SafetySupervisorCommonCfgInit(void);

void LIB_SFT_SafetySupervisorInit(void);
void LIB_SFT_SafetySupervisor(iom_loads_struct_t *newLoads);
void LIB_SFT_SafetyActionMgr(sft_safety_action_bmp_t newSafetyActionBmp, sft_safety_action_bmp_t keepDoneSafetyActionBmp, iom_loads_struct_t *newLoads, sft_module_status_t *pChkExec);

void LIB_SFT_SafetyActionBmpReset(sft_safety_action_bmp_t reqResetActionBmp);
uint8_t LIB_SFT_GetMySafetyPriority( sft_module_status_t *mySftData, uint8_t newMyFaultSrc);

void LIB_SFT_EnterSafetyModule(sft_module_status_t *sftStatus);
void LIB_SFT_LeaveSafetyModule(sft_module_status_t *sftStatus);

#endif	/* defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */

#endif	/* _LIB_PRODUCTSAFETY_H_ */
