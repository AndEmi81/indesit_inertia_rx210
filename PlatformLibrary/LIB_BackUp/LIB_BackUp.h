/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		LIB_BackUp.h
\n			header file of LIB_BackUp.c
\n
\file		LIB_BackUp.h
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

#ifndef __BACK_UP_H
#define __BACK_UP_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "LIB_BackupExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
/** Size of temporary buffer used to store backup data */
#define BLOCK_FLASH_SIZE            64

/* Return values of LIB_Backup_DataSave() */
/** Too many elements inside BackupDimTab */
#define BACKUP_E_TOO_MANY_ELEMENTS  3
/** Too many backup bytes */
#define BACKUP_E_TOO_MANY_BYTES     2
/** Error on write/verify block operation */
#define BACKUP_E_BLOCK_WRITE        1
/** Success */
#define BACKUP_E_OK                 0

/* Return values of LIB_Backup_RefCheck() */
/** Wait don't perform action */
#define	BACKUP_WAIT 0
/** It's time to perform backup */
#define	BACKUP_NOW  1
/** False Alarm don't perform backup */
#define BACKUP_EXIT 2
/** rtn A/D channel not found, check setting file. */
#define BACKUP_RTN_CH_NOT_FOUND 3


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/** lenght of checksum used for backup data */
typedef uint8_t backupChecksum_t;
/** each element of BackupDim must have a size...*/
typedef uint8_t indexBackupDimTab_t;
/** return types for LIB_Backup_DataSave()*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

#if defined(MODULE_BODY_LIB_BACKUP)
extern uint8_t Backup_Source;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void    Task_Backup(void);
bool_t  LIB_Backup_CheckIfIsTimeToBackup(uint8_t systemMode);
uint8_t LIB_Backup_DataSave (void);
bool_t  LIB_Backup_ChecksumVerify(void);
void    LIB_Backup_DataRestore(void);
/**
\n This function monitors the RTN in order to establish if a backup must be performed.
\n If RTN value is lower than REF_MIN_ADC_UNITS for NUM_READ_RTN_OUT_OF_RANGE consecutives times
\n we assume that a backup must be performed.
\n If RTN value is higher or equal REF_MIN_ADC_UNITS for NUM_READ_RTN_IN_RANGE consecutives times
\n we assume that it was a false backup alarm.

\attention \b   Ch_Comp is A/D channel where we read RTN value, used as Input.

\return 	uint8_t

\retval		BACKUP_NOW  perform backup
\retval		BACKUP_WAIT  decision pending
\retval     BACKUP_EXIT no backup
\retval     BACKUP_RTN_CH_NOT_FOUND rtn A/D channel not found, check setting file.


\author 	stefano.ughi
\date		Feb 8, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint8_t LIB_Backup_RefCheck(void);
bool_t  LIB_Backup_RestoreManager(void);
extern void *const   PltBackupAddrTab[];
extern const uint8_t PltBackupDimTab[];
#if defined(CONFIG_LIB_BACKUP_DATACARE_SAVE)
extern const uint8_t PltBackupSafetyIdx[];
#endif
#endif /* __BACK_UP_H */

/* *********** */
/* End of file */
/* *********** */
