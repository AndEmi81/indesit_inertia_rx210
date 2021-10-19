/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_ResetExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		LIB_ResetExp.h
\ingroup	LIB_Reset
\date		Mar 7, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Mar 7, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __RESET_EXP_H
#define __RESET_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "IO_driver.h"
#include "Tools.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define		LIB_RESET_FLAG_AFTER_POR		0x01
#define		LIB_RESET_FLAG_APPL_POR			0x02
#define		LIB_RESET_FLAG_IS_COLD_RESET	0x04
#define		LIB_RESET_FLAG_IS_REMOTE_CTRL	0x08

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct {
	/** This variable contains ID reset's source */
	RST_Type ResetSrc;

	/** This variable contains the new SystemMode value */
	SysMode_Type NewSystemMode;

	/** This variable contains the final SystemMode value */
	SysMode_Type SystemMode;

	/** This variable contains the new SystemMode value */
	SysSubMode_Type NewSystemSubMode;
	/** This variable contains the final SystemMode value */
	SysSubMode_Type SystemSubMode;

	/** Flags */
	uint8_t Reset;

#if defined(CONFIG_LIB_RESET_TIMED_REQ)
	/** Timeout for next reset */
	TOOLS_TIMER Reset_timeout;
#endif /*defined(CONFIG_LIB_RESET_TIMED_REQ)*/

} ResetStatus_t;

/* ****************** */
/* Exported variables */
/* ****************** */

#if !defined(MODULE_BODY_LIB_RESET)
extern const ResetStatus_t ResetCtrl;
extern const uint16_t ResetCnt[NUM_RST_SOURCE];
extern const uint8_t BootVersionInfo[4];
#endif
/* ******************* */
/* Function prototypes */
/* ******************* */

bool_t LIB_Reset_GetPOR_flag(void);
SysMode_Type LIB_Reset_GetSystemMode(void);
SysSubMode_Type LIB_Reset_GetSystemSubMode(void);
SysMode_Type LIB_Reset_GetNewSystemMode(void);
bool_t LIB_Reset_GetIsColdReset(void);
void LIB_Reset_ForceColdReset(void);
void LIB_Reset_ForceReset(void);

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
void LIB_Reset_SafetyCheck(void);
#endif
#endif /* __RESET_EXP_H */

/* *********** */
/* End of file */
/* *********** */

