/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			Backup service implementation.
\n          The following functionalities are available:
\li         Initialization
\li         backup activation
\li         backup data restoring
\n
\n          Backup runs on a high priority non-preempatble task
\n
\file		Main_Backup.c
\ingroup	Main
\date		Feb 7, 2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "callback.h"
#include "Main.h"
#include "LIB_BackupExp.h"
#include "LIB_Reset.h"
#if defined(CONFIG_LIB_TM_BACKUP)
#include "LIB_TaskMonitoringExp.h"
#endif /*defined(CONFIG_LIB_TM_BACKUP)*/
#include "LIB_Trace.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#if !defined(CONFIG_LIB_TM_BACKUP)
/** @name Backup task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

@{
*/
/** */
#define START_BACKUP_TASK_MONITORING()		{}
#define MAIN_BACKUP_TASK__MONITOR_IN()		{}
#define MAIN_BACKUP_TASK__MONITOR_OUT()		{}
/*@}*/
#endif

/* Check for definition od security check */
#if !defined(START_BACKUP_TASK_MONITORING)
#error "START_BACKUP_TASK_MONITORING() missing definition."
#endif

#if !defined(MAIN_BACKUP_TASK__MONITOR_IN)
#error "MAIN_BACKUP_TASK__MONITOR_IN() missing definition."
#endif

#if !defined(MAIN_BACKUP_TASK__MONITOR_OUT)
#error "MAIN_BACKUP_TASK__MONITOR_OUT() missing definition."
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* ********* */
/* Variables */
/* ********* */

/* Global */
/* Static */
/**
@name Service variables
@{
*/
/** Task Stack */
static STACK_SEC OS_STACKPTR int Main_BackupTaskStack[MAIN_BACKUP_TASK_STACK_SIZE] STACK_SEC_POST;
/**
Backup task control block
*/
static OS_TASK Main_BackupTaskTCB;

/*@}*/

#if defined(CONFIG_MAIN_BACKUP_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_BackupStackMonitor;
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */
static void Main_BackupTask ( void );


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Backup service initialization.
It creates task Main_BackupTask
*/
void Main_BackupInit ( void )
{
	OS_CREATETASK ( &Main_BackupTaskTCB,
					"BackupTask",
					Main_BackupTask,
					( OS_U8 ) MAIN_BACKUP_TASK_PRIORITY,
					Main_BackupTaskStack );
#if defined(CONFIG_MAIN_BACKUP_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_BackupTaskTCB, &Main_BackupStackMonitor);
#endif
}

/**
Backup activation
Resumes task Main_BackupTask
*/
void Main_ActivateBackup ( void )
{
	if ( OS_GetSuspendCnt ( &Main_BackupTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_BackupTaskTCB );
	}
}


/*
Restores Application data


Calls the following callbacks:
\li CALLBACK_Main_ColdReset_BeforeRestore
\li CALLBACK_Main_ColdReset_OnRestoredData
\li CALLBACK_Main_ColdReset_OnNotRestoredData
\li CALLBACK_Main_ColdReset_AfterRestore
\li CALLBACK_Main_WarmReset
\li CALLBACK_Main_BackupAreaInvalidationRequired

\warning	none

\author 	Roberto Fioravanti
\date		30/03/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void Main_Backup_DoRestore ( void )
{
	if ( LIB_CheckKeywordsRam() == FALSE )
	{
		CALLBACK_Main_ColdReset_BeforeRestore();
		
		if ( DoRestoreSystem() )
		{
			CALLBACK_Main_ColdReset_OnRestoredData();
		}
		else
		{
			CALLBACK_Main_ColdReset_OnNotRestoredData();
		}
		
		CALLBACK_Main_ColdReset_AfterRestore();
		LIB_Backup_ResetBackupSource();
		LIB_SetKeywordsRam();
	}
	else
	{
		CALLBACK_Main_WarmReset();
	}
	
	if ( CALLBACK_Main_BackupAreaInvalidationRequired() == TRUE )
	{
		/* Backup area Invalidation */
		( void ) LIB_Backup_InvalidateBackupArea();
	}
}

/* Local Functions */

/*
No-preemptable task for backup. When it starts if goes in 'suspended mode'.
I can can be resumed calling Main_ActivateBackup() function

\warning	none

\author 	Roberto Fioravanti
\date		30/03/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void Main_BackupTask ( void )
{
  	START_BACKUP_TASK_MONITORING();
	while ( 1 )
	{
		OS_Suspend ( NULL );

		TRACE_CPU_IDLE_END();

		OS_EnterRegion();
		/* here the scheduler is switched off */
		MAIN_BACKUP_TASK__MONITOR_IN();
		Task_Backup();
		MAIN_BACKUP_TASK__MONITOR_OUT();
		OS_LeaveRegion();
		/* here the scheduler is switched on */
	}
}



/* *********** */
/* End of file */
/* *********** */
