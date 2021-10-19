/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\ar		Description
\n			This file contains the EmbOS-based service for the final application.
\n
\file		Main_Application.c
\ingroup	Main
\date		Feb 7, 2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.00
\n		Feb 7, 2011
\n		Roberto Fioravanti
\n		changes
\n 		- file created
\n
\n
\n etc.
*/
/*lint -idlen(60)*/

/* ******** */
/* Includes */
/* ******** */
#include "callback.h"
#include "Main.h"
#include "LIB_BackupExp.h"
#include "LIB_ResetExp.h"
#include "LIB_Trace.h"

/* Product modules*/

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined(CONFIG_MAIN_APPLICATION_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_APPLICATION_TASK_TRACE_BEGIN()        TRACE_TO_PIN(ON , (CONFIG_MAIN_APPLICATION_TASK_TRACE_PIN) )
#define MAIN_APPLICATION_TASK_TRACE_END()          TRACE_TO_PIN(OFF , (CONFIG_MAIN_APPLICATION_TASK_TRACE_PIN) )
#else
#define MAIN_APPLICATION_TASK_TRACE_BEGIN()			{}
#define MAIN_APPLICATION_TASK_TRACE_END()			{}
#endif




#if defined(OS_TICK_ms_PERIOD)

#if (MAIN_APPLICATION_TASK_PERIOD < OS_TICK_ms_PERIOD)
/*
Lowest period is defined by OS tick defined by OS_TICK_FREQ
*/
#error "OS tick (defined by OS_TICK_FREQ) cannot generate the period set in MAIN_APPLICATION_TASK_PERIOD."
#error "Change OS_TICK_FREQ or MAIN_APPLICATION_TASK_PERIOD"
#endif

#if (MAIN_APPLICATION_TASK_PERIOD % OS_TICK_ms_PERIOD) != 0
/*
Period must be a multiple of OS tick defined by OS_TICK_FREQ
*/
#error "MAIN_APPLICATION_TASK_PERIOD must be multiple of OS tick (defined by OS_TICK_FREQ). "
#error "Change OS_TICK_ms_PERIOD or MAIN_APPLICATION_TASK_PERIOD."
#else
/*
Defines the Supervisor task period as expected by OS.
*/
#define MAIN_APPLICATION_TASK__OSTICK_PERIOD             (MAIN_APPLICATION_TASK_PERIOD / OS_TICK_RESOLUTION)
#endif

#endif




#if !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_IN) && !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT) && !defined(MAIN_APP_PERIODICASYN_TASK__MONITORING)
/** @name Analog task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

\par Examples
\code
#define MAIN_APP_PERIODICASYN_TASK__MONITOR_IN()     helloTaskMonitor( APPLICATION_TASK_ID );
#define MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT()
\endcode
@{
*/
/** safety config */
#define MAIN_APP_PERIODICASYN_TASK__MONITORING()
/** safety entry */
#define MAIN_APP_PERIODICASYN_TASK__MONITOR_IN()
/** safety exit */
#define MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT()
/*@}*/

#else
/* MAIN_APP_PERIODICASYN_TASK__MONITOR_IN or MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT defined */
#if !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_IN)
#error "MAIN_APP_PERIODICASYN_TASK__MONITOR_IN() missing definition."
#error "Both MAIN_APP_PERIODICASYN_TASK__MONITOR_IN() and MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT() must be defined."
#endif

#if !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT)
#error "MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT() missing definition."
#error "Both MAIN_APP_PERIODICASYN_TASK__MONITOR_IN() and MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT() must be defined."
#endif

#if !defined(MAIN_APP_PERIODICASYN_TASK__MONITORING)
#error "MAIN_APP_PERIODICASYN_TASK__MONITORING() missing definition - must be defined."
#endif

#endif /* !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_IN) && !defined(MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT) */


#ifndef MAIN_APP_PERIODICASYN_TASK__IS_READY

/**
Defines the condition to allow the execution of application task.

\par Properties
Not mandatory. If they remain undefined assume by default 1.

\par Example #1: only if Res is good.
\code
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       Main_Analog_IsRefOk()
\endcode

\par Example #2: always
\code
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       (1)
\endcode
*/
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       (1)
#endif /* MAIN_APP_PERIODICASYN_TASK__IS_READY */


/**
Defines the startup behavior of the application task.

\par Properties
Not mandatory. If they remain undefined assume by default an empy value.
*/
#ifndef MAIN_APP_PERIODICASYN_TASK__STARTUP
#define MAIN_APP_PERIODICASYN_TASK__STARTUP()
#endif


#ifndef MAIN_APP_PERIODICASYN_TASK__BODY
/**
Defines the application task body.

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

\par Example: with Remote Loads management
\code
#define MAIN_APP_PERIODICASYN_TASK__BODY()\
if ( CALLBACK_Main_ApplicationTask_Ctrl() )\
{\
  void * pRemoteLoadsControlData = 0;\
  int len=0;\
  \
  Main_RemoteLoadsControl_GetCommand(&pRemoteLoadsControlData, &len);\
  if(pRemoteLoadsControlData && len)\
  {\
    CALLBACK_RemoteLoadsControl_BeforeProcess(pRemoteLoadsControlData, &len);\
    Main_RemoteLoadsControl_ProcessCommand((uint8_t*)pRemoteLoadsControlData);\
    CALLBACK_RemoteLoadsControl_AfterProcess(pRemoteLoadsControlData, &len);\
    Main_RemoteLoadsControl_PurgeCommand();\
  }\
  CALLBACK_Main_ApplicationTask_Body();\
}
\endcode
*/
#define MAIN_APP_PERIODICASYN_TASK__BODY()       {}
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
static STACK_SEC OS_STACKPTR int Main_ApplicationPeriodicAsynTaskStack[MAIN_APP_PERIODICASYN_TASK_STACK_SIZE] STACK_SEC_POST;

/** Task Control Block*/
static OS_TASK Main_ApplicationPeriodicAsynTaskTCB;
/*@}*/

#if defined(CONFIG_MAIN_APPLICATION_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_ApplicationStackMonitor;
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */
static void Main_ApplicationPeriodicAsynTask ( void );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Application service initialization.
It creates the task \ref Main_ApplicationPeriodicAsynTask.
*/
void Main_ApplicationInit ( void )
{
	OS_CREATETASK ( &Main_ApplicationPeriodicAsynTaskTCB,
					"ApplicationPeriodicAsynTask",
					Main_ApplicationPeriodicAsynTask,
					MAIN_APPLICATION_TASK_PRIORITY,
					Main_ApplicationPeriodicAsynTaskStack );
#if defined(CONFIG_MAIN_APPLICATION_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_ApplicationPeriodicAsynTaskTCB, &Main_ApplicationStackMonitor);
#endif
}

/* Local Functions */

/**
preemptable task for Feedback

\warning	none

\author 	Roberto Fioravanti
\date		  gg/mm/aaaa
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
static void Main_ApplicationPeriodicAsynTask ( void )
{
	int t0 = OS_GetTime();
    static bool_t Main_ApplicationPeriodicAsynRunning = FALSE;


	MAIN_APP_PERIODICASYN_TASK__MONITORING();
    CONFIG_LIB_LOADSREQUEST_MONITORING();
    CONFIG_LIB_PRODUCTSAFETY_TASK_MONITORING();
#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
    if (LIB_Reset_GetIsColdReset())
    {
    	LIB_SFT_SafetySupervisorInit();
    }
#endif
    MAIN_APP_PERIODICASYN_TASK__STARTUP();

	while ( 1 )
	{
		TRACE_CPU_IDLE_END();

	    MAIN_APP_PERIODICASYN_TASK__MONITOR_IN();
	    MAIN_APPLICATION_TASK_TRACE_BEGIN();

	    t0 += MAIN_APPLICATION_TASK__OSTICK_PERIOD;


	    /* waiting for condition in MAIN_APP_PERIODICASYN_TASK__IS_READY() is satisfied */
	    Main_ApplicationPeriodicAsynRunning = MAIN_APP_PERIODICASYN_TASK__IS_READY();
		
		if(Main_ApplicationPeriodicAsynRunning)
		{
			if ( ! ( Main_IsRestoreCompleted() ) )
			{
			    OS_EnterRegion();
				Main_Backup_DoRestore();
				Main_CompleteRestore();

				OS_LeaveRegion();
				LIB_Backup_SetBackupControlMonitor(BACKUP_ON_RTN);
				START_LIB_LOADSREQUEST_MONITORING();
			}

			MAIN_APP_PERIODICASYN_TASK__BODY();
		}

		MAIN_APPLICATION_TASK_TRACE_END();
		MAIN_APP_PERIODICASYN_TASK__MONITOR_OUT();
		OS_DelayUntil ( t0 );
	}
}

/* *********** */
/* End of file */
/* *********** */
