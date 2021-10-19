/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	BackupLibrary  Backup Library
@{
\par Description
Backup Library for \ref MainBackup service.
@}
\ingroup        MainLibrary
*/
/**
\n
\par		    Description
\n			    Backup and Restore based on Platfrom library module.
\n
\file		    Backup.c
\ingroup	    MainLibrary
\date		    24/01/2011
\version	    00.00.00
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		        00.00.00
\n          	24/01/2011
\n          	Roberto Fioravanti
\n            - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */

#include "Main.h"
#include "LIB_Backup.h"

#include "LIB_Reset.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define Backup_EnterCriticalSection()        EnterCriticalSection()


#define Backup_ExitCriticalSection()        ExitCriticalSection()

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */


/* Static */

/* Global */

/* ******************** */
/* Functions Prototypes */
/* ******************** */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
If _SystemMode is SYSTEM_NORMAL_MODE forces a backup.
If _SystemMode is SYSTEM_PROGRAMMING_MODE or SYSTEM_SAVEREG_PROG_MODE invalidates backup area if backup is valid.
If _SystemMode is SYSTEM_SAVEREG_PROG_MODE forces a backup
At the end forces a cold reset.

\param [in] _SystemMode current SystemMode

*/
void Backup_SystemBackup ( uint8_t _SystemMode )
{
	if ( _SystemMode == SYSTEM_NORMAL_MODE )
	{
		LIB_Backup_ForceBackup();
	}
	else
	{
		/* 	_SystemMode == SYSTEM_PROGRAMMING_MODE or
			_SystemMode == SYSTEM_SAVEREG_PROG_MODE */
		Backup_EnterCriticalSection();
		
		if ( LIB_Backup_ChecksumVerify() == TRUE )
		{
			/* Backup area Invalidation */
			(void)LIB_Backup_InvalidateBackupArea();
		}
		
		Backup_ExitCriticalSection();
	}
	
	if((_SystemMode == SYSTEM_PROGRAMMING_MODE) && (LIB_Reset_GetSystemSubMode() == SYSTEM_PROGRAMMING_SAVEREG_MODE))
	{
		LIB_Backup_ForceBackup();
	}

	LIB_Reset_SetNewSystemMode ( (SysMode_Type)SYSTEM_NORMAL_MODE );
	LIB_Reset_ForceColdReset();
}


/**
If _SystemMode is SYSTEM_NORMAL_MODE Forces a backup, else invalidates backup area if backup is valid.
At the end forces a cold reset.

Calls LIB_Backup_RestoreManager().

\return bool_t

\retval		TRUE backup data restored
\retval		FALSE backup data not restored because checksum K.O.

\author 	Roberto Fioravanti
\date		Mar 14, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t DoRestoreSystem ( void )
{
	return LIB_Backup_RestoreManager();
}



/* Local Function */




/* *********** */
/* End of file */
/* *********** */
