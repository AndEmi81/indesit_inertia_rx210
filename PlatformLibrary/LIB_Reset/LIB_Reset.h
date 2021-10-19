/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_Reset.h
\n			header file of LIB_Reset.c
\n
\file		LIB_Reset.h
\ingroup	LIB_Reset
\date		Mar 7, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Mar 7, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n 		01.00.02
\n		Mar 31, 2011
\n		stefano.ughi
\n		changes
\n 			- added #include "LIB_ResetExp.h"
*/

#ifndef __RESET_H
#define __RESET_H

/* ********** */
/* Includes */
/* ********* */
#include "OCRST_drv.h"
#include "LIB_ResetExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/** R.A.M. init. called inside _startup */
extern void	__ram_initialization(void);

/*<belonging to the module, visible to the Platform project>*/
void __initialize_hardware(void);
void __lib_reset_manager(void);
bool_t LIB_CheckKeywordsRam(void);
void LIB_SetKeywordsRam(void);
void LIB_KeywordsRamCorruption(void);
void LIB_Reset_SetNewSystemMode(SysMode_Type value);
void LIB_Reset_SetNewSubSystemMode(SysSubMode_Type value);
bool_t LIB_Reset_IsColdReset(void);

bool_t LIB_Reset_SetSystemModeRequest ( SysMode_Type newMode, SysSubMode_Type newSubMode );
#if defined CONFIG_LIB_RESET_TIMED_REQ
void LIB_Reset_SetTimedRequest(uint16_t chkKeyword, uint32_t timeout);
void LIB_Reset_ResetTimedRequest(void);
bool_t LIB_Reset_ChkTimedRequest(uint16_t chkKeyword);
void LIB_Reset_SetResetTimeout(uint32_t timeout);
bool_t LIB_Reset_IsTimeToReset(void);
bool_t LIB_Reset_IsTimeoutSet(void);

void LIB_Reset_EnterRemoteControl(uint32_t timeout);
bool_t LIB_Reset_IsRemoteControl(void);
uint32_t LIB_Reset_GetResetTimeout(void);
#endif

#endif /* __RESET_H */

/* *********** */
/* End of file */
/* *********** */
