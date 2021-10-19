/*
 * LIB_Error.c
 *
 *  Created on: 18/mar/2013
 *      Author: marco
 */
#include "LIB_Error.h"
#include "Main.h"
#include "LIB_ResetExp.h"

#if defined (__CWCC__)
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */
NO_INIT_VAR PltError_t  LIB_Error_lastWarning;
NO_INIT_VAR PltError_t  LIB_Error_lastFatal ;

#elif defined(__GNUC__)
PltError_t  LIB_Error_lastWarning __attribute__ ((section (".noinit")));
PltError_t  LIB_Error_lastFatal __attribute__ ((section (".noinit")));
#else
PltError_t  LIB_Error_lastWarning ;
PltError_t  LIB_Error_lastFatal ;
#endif



void LIB_Error_Warning_Handler( PltError_t err)
{
	LIB_Error_lastWarning = err;
}

void LIB_Error_Fatal_Handler(PltError_t err)
{
	LIB_Error_lastFatal = err;

	if (LIB_Backup_GetBackupControlMonitor() != BACKUP_NO_SOURCE)
	{
#if !defined(CONFIG_LIB_BACKUP_DATACARE_SAVE)
		/* Force a cold reset with restore
		 * partial restore only if Data corruption occurs */
		if (err == PLT_ERROR_DATACORRUPTION_1)
		{
			LIB_Backup_SetBackupSource();
		}
#endif
		Backup_SystemBackup(LIB_Reset_GetSystemMode());
	}
	else
	{
		LIB_Reset_ForceColdReset();
	}
}
