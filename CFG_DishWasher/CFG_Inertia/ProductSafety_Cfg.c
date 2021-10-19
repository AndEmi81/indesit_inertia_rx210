/*
 * ProductSafety_Cfg.c
 *
 *  Created on: Jan 14, 2013
 *      Author: alessandro.sabbatini
 */

#ifndef _PRODUCTSAFETY_CFG_
#define _PRODUCTSAFETY_CFG_

#include <stddef.h>
#include "Config.h"

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)

#include "LIB_ProductSafety.h"

/* Safety Modules to include */
#include "include filename"




/* Safety module init functions array defined */
const SFT_SafetyModuleInit_t SafetyModuleInitFuncs[SFT_NUM_SAFETY_MODULES] =
{
		(SFT_SafetyModuleInit_t) &FunctionPointerToInitMethod
};

/* Safety modules array defined */
const SFT_SafetyModuleExec_t SafetyModuleFuncs[SFT_NUM_SAFETY_MODULES] =
{
		(SFT_SafetyModuleExec_t) &FunctionPointerToMonitorMethod,
};

const SFT_SafetyModuleTimer_t SafetyModuleTimer[SFT_NUM_SAFETY_MODULES] =
{
		(SFT_SafetyModuleTimer_t) &FunctionPointerToGetTimeMethod,
};

const sft_faultSrc_t SafetyFaultSources[SFT_NUM_FAULT_SOURCES] =
{
	/* 		ModuleID, 			FaultSrcID , 					FaultCode,	FaultSubCode */
	/* Standard and Supervisor */
		{ SFT_ID_ANY, 				SFT_NO_FAULT_SRC, 				0, 			0},
		{ SFT_ID_ANY, 				SFT_FLT_SUPERVISOR_BAD_SETTING, 0x09, 		0x01},

};

const SFT_SafetyActionExec_t SafetyActionFuncs[SFT_NUM_SAFETY_ACTIONS] =
{
		(SFT_SafetyActionExec_t)&FunctionPointerTosafetyActionMethod,

};

#if defined(SFT_NUM_STATUS_ACTIONS)
const sft_safetyModulesActions_t SafetyModulesStatusActions[SFT_NUM_STATUS_ACTIONS] =
{
		/* ModuleID, 			FaultSrcID, 					Module Status, 			SafetyActionBmp, 				SafetyActionClrBmp */

		{ SFT_ID_ANY, 			SFT_ID_ANY, 					SFT_MODULE_FAULT, 		SFT_ACTION_DEFAULT_MASK, 0 },
};
#endif

#if defined(SFT_NUM_CUSTOM_FAULTCODES)

const uint8_t SafetyCustomCodeFaultSrc[SFT_NUM_CUSTOM_FAULTCODES] =
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
		SFT_FLT_EXT_MOTOR_KO,
#elif defined(CONFIG_PM_MOTORGATE)
		SFT_FLT_WM_ACMOTOR_KO,
#else
		NULL
#endif
};
const SFT_SafetyCustomCode_t SafetyCustomCodeFuncs[SFT_NUM_CUSTOM_FAULTCODES] =
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
		&PM_SFT_WM_MotorGetFault,
#elif defined(CONFIG_PM_MOTORGATE)
		&PM_SFT_WM_MotorGetFault,
#else
		NULL
#endif
};

#endif

void LIB_SFT_SafetySupervisorCommonCfgInit()
{
	PM_SFT_WM_Init();
}

#endif /* defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) */

#endif /* _PRODUCTSAFETY_CFG_ */
