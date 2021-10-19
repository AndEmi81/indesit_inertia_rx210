/*
Copyright (c) 2004-2013, Indesit Company
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	ProductSafety
\n	This module contains Safety Supervisor and Safety Action Mgr
\n
\file		LIB_ProductSafetyExp.h
\ingroup        LIB_ProductSafety
\date	    2013-01-22
\version	01.00.00
\author		 Alessandro Sabbatini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		2013-01-22
\n		Alessandro Sabbatini
\n		changes
\n 			Creation
\n
*/

/* ******** */
/* Includes */
/* ******** */

#ifndef _LIB_PRODUCTSAFETY_EXP_H_
#define _LIB_PRODUCTSAFETY_EXP_H_

#define		SFT_ID_ANY				0xFF

/* No fault source value - all module must not use this value for safety source! */
#define		SFT_NO_FAULT_SRC		0

/* No priority value */
#define		SFT_PRIO_NONE			0

/* Safety flag requests */
#define		SFT_FLAGS_NONE			0x00
#define		SFT_FLAGS_RESET_REQ		0x01
#define		SFT_FLAGS_TEST_TMR_REQ	0x02
#define		SFT_FLAGS_ENDCYCLE_REQ	0x04

enum sft_safetymodule_status {
	SFT_MODULE_IDLE,		/* 0 */
	SFT_MODULE_RESET,		/* 1 */
	SFT_MODULE_MONITORING,	/* 2 */
	SFT_MODULE_WARNING,		/* 3 */
	SFT_MODULE_PROTECTION,	/* 4 */
	SFT_MODULE_PREFAULT,	/* 5 */
	SFT_MODULE_FAULT		/* 6 */
};

/* Safety module status */
typedef struct sft_module_status_s
{
	uint8_t	status;
	uint8_t chkExec;
	uint8_t faultSrc;
} sft_module_status_t;

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT) && !defined(MODULE_BODY_PRODUCTSAFETY)
extern const uint16_t	SafetyModuleTimerCntr[SFT_NUM_SAFETY_MODULES];
#endif

void LIB_SFT_SafetyActionRequest(sft_safety_action_bmp_t newApplSafetyReqBmp);
void LIB_SFT_SafetyFlagsRequest(uint8_t newApplSafetyFlagsBmp);

sft_safety_action_bmp_t LIB_SFT_GetPendingSafetyActionBmp(void);
sft_safety_action_bmp_t LIB_SFT_GetRequestSafetyActionBmp(void);
sft_safety_action_bmp_t LIB_SFT_GetSafetyActionExecBmp(void);
sft_safety_action_bmp_t LIB_SFT_GetSafetyActionDoneBmp(void);

void LIB_SFT_ResetStatusChangeNotification(void);
uint8_t LIB_SFT_GetSafetyModFaultStatus(uint8_t safetyModuleID, sft_module_faults_t *newSafetyStatus);
void LIB_SFT_GetAllSafetyFaultStatus(sft_module_faults_t *newSafetyStatus);
void LIB_SFT_GetOverallFaultStatus(sft_module_faults_t *newSafetyStatus);
bool_t LIB_SFT_GetResetChangedFaultStatus(sft_module_faults_t *newSafetyStatus);

uint8_t LIB_SFT_GetSafetyModStatus(uint8_t safetyModuleID, sft_module_status_t *newSafetyStatus);

uint8_t LIB_SFT_GetSafetyFaultCode(uint8_t faultSrcItem, uint8_t *faultCode, uint8_t *faultSubcode);
uint8_t LIB_SFT_GetFaultSourceStatus(uint8_t faultSrcItem);



#endif
