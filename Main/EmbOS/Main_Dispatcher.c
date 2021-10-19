/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			Dispatch Events
\n
\n          Runs on a high priority non-preempatble task
\n
\file		Main_Dispatcher.c
\ingroup	Main
\date		27/02/2013
\version	01.00.00
\author		Marco Sperandini
\n
\n
*/
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
#include "callback.h"
#include "Main.h"
#include "LIB_TaskMonitoringExp.h"
#include "syncSvc.h"
#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if !defined(CONFIG_LIB_TM_DISPATCHER)
/** @name Dispatch task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

@{
*/
/** */
#define START_DISPATCHER_TASK_MONITORING()
#define MAIN_DISPATCHER_TASK__MONITOR_IN()
#define MAIN_DISPATCHER_TASK__MONITOR_OUT()
/*@}*/
#endif

/* Check for definition od security check */
#if !defined(START_DISPATCHER_TASK_MONITORING)
#error "START_DISPATCHER_TASK_MONITORING() missing definition."
#endif

#if !defined(MAIN_DISPATCHER_TASK__MONITOR_IN)
#error "MAIN_DISPATCHER_TASK__MONITOR_IN() missing definition."
#endif

#if !defined(MAIN_DISPATCHER_TASK__MONITOR_OUT)
#error "MAIN_DISPATCHER_TASK__MONITOR_OUT() missing definition."
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
static STACK_SEC OS_STACKPTR int Main_DispatcherTaskStack[CONFIG_MAIN_DISPATCHER_TASK_STACK_SIZE] STACK_SEC_POST;
/**
Dispatcher task control block
*/
static OS_TASK Main_DispatcherTaskTCB;

/*@}*/

#if defined(CONFIG_MAIN_DISPATCHER_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_DisparcherStackMonitor;
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */
static void Main_DispatcherTask ( void );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Service initialization.
It creates task.
*/
void Main_DispatcherInit ( void )
{
	OS_CREATETASK ( &Main_DispatcherTaskTCB,
					"DispatcherTask",
					Main_DispatcherTask,
					( OS_U8 ) MAIN_DISPATCHER_TASK_PRIORITY,
					Main_DispatcherTaskStack );
#if defined(CONFIG_MAIN_DISPATCHER_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_DispatcherTaskTCB, &Main_DispatcherStackMonitor);
#endif
}

/**
Dispatcher activation
Resumes task
*/
void Main_ActivateDispatcher ( void )
{
	if ( OS_GetSuspendCnt ( &Main_DispatcherTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_DispatcherTaskTCB );
	}
}



/* Local Functions */

static void Main_DispatcherTask ( void )
{
  	START_DISPATCHER_TASK_MONITORING()
	while ( 1 )
	{
		OS_Suspend ( NULL );

		TRACE_CPU_IDLE_END();

		OS_EnterRegion();
		/* here the scheduler is switched off */
		MAIN_DISPATCHER_TASK__MONITOR_IN()

		if(ZcEvent.type){
			notifyEvent( &syncSvcList, (NotifyEvent*)&ZcEvent);
			ZcEvent.type = EV_NONE;
		}

		MAIN_DISPATCHER_TASK__MONITOR_OUT()
		OS_LeaveRegion();
		/* here the scheduler is switched on */
	}
}

#endif /* defined(CONFIG_DISPATCHER_TASK_ENABLE) */
/* *********** */
/* End of file */
/* *********** */
