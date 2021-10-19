/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_BackUpExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		LIB_BackUpExp.h
\ingroup	LIB_BackUp
\date		Feb 07, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Feb 07, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __BACK_UP_EXP_H
#define __BACK_UP_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/**
\name Indicatori per individuare le variabili da non ripristinare dopo un PC Control
*/
/*@{*/
#define PC_CTRL_NOT_RESTORED_END		((void *) 0)/**< Marcatore che, nella lista degli indirizzi,
 													 segna l'inizio delle variabili da
 													 ripristinare anche dopo un PC Control. */
#define SIZEOF_PC_CTRL_MARKER			0		/**< Corrisponde a PC_CTRL_NOT_RESTORED_END
													 nella lista delle dimensioni, fa si' che
													 per l'indirizzo PC_CTRL_NOT_RESTORED_END
													 non sia ripristinato nulla. */

/** marker for end backp list*/
#define BCK_ADDR_END_TAB	((void*)0xFFFFFFFF)

/** Keyword used to decide if partial or full backup restore must be performed
tipically after a P.C. Control command or an Application cold reset request */
#define PARTIAL_BACKUP_KEYWORD		0x5A
/** defines used for LIB_Backup_SetBackupControlMonitor() */
/** Disables backup's monitoring */
#define BACKUP_NO_SOURCE    		((uint8_t) 0 )
/** Enables backup's monitoring on RTN */
#define BACKUP_ON_RTN       		((uint8_t) 1 )
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

#if !defined(MODULE_BODY_LIB_BACKUP)
extern const uint8_t Backup_Source;
#endif

extern void *const AppBackupAddrTab[];
extern uint8_t const AppBackupDimTab[];
/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
uint8_t LIB_Backup_GetBackupSource(void);
void LIB_Backup_SetBackupSource(void);
void LIB_Backup_ResetBackupSource(void);
void LIB_Backup_ForceBackup(void);
void LIB_Backup_SetBackupControlMonitor(uint8_t backupSource);
uint8_t LIB_Backup_GetBackupControlMonitor(void);
bool_t LIB_Backup_InvalidateBackupArea(void);
uint16_t LIB_Backup_GetBackupLen(void);

#endif /* __BACK_UP_EXP_H */

/* *********** */
/* End of file */
/* *********** */

