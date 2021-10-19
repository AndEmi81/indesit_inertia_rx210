/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**

\addtogroup Main
@{
\par Description
This module is the EmbOS-based framework for main board applications.
\n EmbOS is a multitasking system that is used as an embedded OS for realtime applications.

This framework consists of services as specified in the Modules section.
Each service is configurable.

\par Preprocessing options
As this framework is EmbOS-based you need this preprocessing option:
 \b OS_EmbOS
\par Configuration file
The configuration file is "MainConfig.h". This file will contain all custom project properties
for services that will overwrite default value. some of these properties relates to Task Priority,
debug options, task safety, service-specific configuration.


@}
*/
/**
\n
\par		Description.c
\n			EmbOS  Main startup and supervisor EmbOS-based .
\n
\file		Main_Init.c
\ingroup	Main
\date		Jan , 2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n
\n 		00.00.95
\n		dd/mm/yyyy
\n		Roberto Fioravanti
\n		changes
\n 		- file created
\n
\n
\n etc.
*/
/* ******** */
/* Includes */
/* ******** */

/* configuration */
#include "Config.h"

#include "BCP_BusCommMng.h"
#include "Main.h"

/* drivers */
#include "OCDIO_drv.h"
#include "OCWDT_drv.h"
#include "OCTPM_drv.h"

/* platform libraries */
#include "Analog.h"
#include "Feedbacks.h"
#include "LIB_Reset.h"
#include "LIB_Zerocrossing.h"
#if defined(CONFIG_PL_CLASS_B_CHECK)
#include "LIB_ClassBCheck.h"
#endif
#include "InputManager.h"
#include "IOSharing.h"
#include "OutputManager.h"
#if defined (CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS)
#include "DigitalOutputMulti.h"
#endif
#include "ZcLoads.h"

#include "callback.h"

#if defined(CONFIG_PL_DEBUG) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#include "LIB_ClassBVar.h"
#endif

#if defined(CONFIG_MAIN_SUPERVISOR_TASK_TRACE_PIN_ENABLE)
#include "LIB_Trace.h"
#endif
#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)

#elif defined(CONFIG_PM_TASK_MONITOR_SUPPORT)
#include "PM_TaskMonitoring.h"
#endif
#if defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)
#include "Main_ActivityList.h"
#endif
#include "LIB_Trace.h"
#include "LIB_TaskMonitoring.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_MAIN_SUPERVISOR_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_SUPERVISOR_TASK_TRACE_BEGIN()        TRACE_TO_PIN(ON , (CONFIG_MAIN_SUPERVISOR_TASK_TRACE_PIN-1) )
#define MAIN_SUPERVISOR_TASK_TRACE_END()          TRACE_TO_PIN(OFF , (CONFIG_MAIN_SUPERVISOR_TASK_TRACE_PIN-1) )
#else
#define MAIN_SUPERVISOR_TASK_TRACE_BEGIN()
#define MAIN_SUPERVISOR_TASK_TRACE_END()
#endif



#if (MAIN_SUPERVISOR_TASK_PERIOD < OS_TICK_ms_PERIOD)
/*
Lowest period is defined by OS tick defined by OS_TICK_FREQ
*/
#error "OS tick (defined by OS_TICK_FREQ) cannot generate the period set in MAIN_SUPERVISOR_TASK_PERIOD.\
Change OS_TICK_FREQ or MAIN_SUPERVISOR_TASK_PERIOD"
#endif





/** @name Supervisor task safety
\par Example
\code
#define MAIN_SUPERVISOR_TASK__MONITOR_IN()                  helloTaskMonitor(PERIODIC_TASK_ID);
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
\endcode
@{
*/
#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
#define MAIN_SUPERVISOR_TASK__MONITOR_IN()
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(x)             /*To be defined */
#elif defined(CONFIG_PM_TASK_MONITOR_SUPPORT)

#define MAIN_SUPERVISOR_TASK__MONITOR_IN()
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(_SystemMode)\
if (_SystemMode == SYSTEM_NORMAL_MODE)\
{\
    if ( PM_TM_TaskMonitor() == FALSE )\
    {\
        LIB_Reset_ForceColdReset();\
    }\
}
#else
#define MAIN_SUPERVISOR_TASK__MONITOR_IN()
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(x)
#endif
/*@}*/

/** @name Supervisor task safety

\par Properties
Not mandatory. If undefined, they assume by default an empty value.

\par Examples
\code
#define MAIN_SUPERVISOR_TASK__MONITOR_IN()     helloTaskMonitor( SUPERVISOR_TASK_ID );
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
\endcode
@{
*/
#if !defined(MAIN_SUPERVISOR_TASK__MONITOR_IN) && \
    !defined(MAIN_SUPERVISOR_TASK__MONITOR_OUT) && \
    !defined(MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR)

/** safety entry */
#define MAIN_SUPERVISOR_TASK__MONITOR_IN()
/** safety exit */
#define MAIN_SUPERVISOR_TASK__MONITOR_OUT()
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(x)
/*@}*/

#else
/* MAIN_SUPERVISOR_TASK__MONITOR_IN or MAIN_SUPERVISOR_TASK__MONITOR_OUT defined */
#if !defined(MAIN_SUPERVISOR_TASK__MONITOR_IN)
#error "MAIN_SUPERVISOR_TASK__MONITOR_IN() missing definition."
#error "Both MAIN_SUPERVISOR_TASK__MONITOR_IN() and MAIN_SUPERVISOR_TASK__MONITOR_OUT() must be defined."
#endif

#if !defined(MAIN_SUPERVISOR_TASK__MONITOR_OUT)
#error "MAIN_SUPERVISOR_TASK__MONITOR_OUT() missing definition."
#error "Both MAIN_SUPERVISOR_TASK__MONITOR_IN() and MAIN_SUPERVISOR_TASK__MONITOR_OUT() must be defined."
#endif


#if !defined(MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR)
#error "MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR() missing definition."
#error "MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR, MAIN_SUPERVISOR_TASK__MONITOR_IN() and MAIN_SUPERVISOR_TASK__MONITOR_OUT() must be defined."
#endif

#endif /* !defined(MAIN_SUPERVISOR_TASK__MONITOR_IN) && !defined(MAIN_SUPERVISOR_TASK__MONITOR_OUT) */

#ifndef MAIN_SUPERVISOR_TASK_SET_PERIOD
/**
Period of the supervisor task.
Not mandatory.
Must be defined as a value or a value returned by a function.
If undefined, they assume by default
\code
#define MAIN_SUPERVISOR_TASK_SET_PERIOD()   (10)
\endcode
*/
#define MAIN_SUPERVISOR_TASK_SET_PERIOD()   (10)
#endif

#ifndef MAIN_SUPERVISOR_TASK__BODY
/**
Application specific code in supervisor task.
\param [in] _SystemMode system mode
\param [in] _timestamp time

Not mandatory. If undefined, it assumes by default an empty value.

\par Example #1: backup
\code
#define MAIN_SUPERVISOR_TASK__BODY(_SystemMode, _timestamp)\
    if(TRUE == Main_IsTimeForColdReset())\
    {\
        Backup_SystemBackup(_SystemMode);\
    }
\endcode

\par Example #2: calling a callback every 10 ms
\code
#define MAIN_SUPERVISOR_TASK__BODY(_SystemMode, _timestamp)\
{\
    uint8_t t10;\
    t10=_timestamp % 10; \ //check if timestamp is a multiple of 10
    if((t10) == 0)\
    {\
        CALLBACK_Main_GP(_SystemMode);\
    }\
}
\endcode

\par Example #3: doing something every 20 ms
\code
#define MAIN_SUPERVISOR_TASK__BODY(_SystemMode, _timestamp)\
{\
    uint8_t t20;\
    t20=_timestamp % 20; \ //check if timestamp is a multiple of 20
    if((t20) == 0)\
    {\
        .....\ //plece here your code
    }\
}
\endcode
*/
#define MAIN_SUPERVISOR_TASK__BODY(_SystemMode, _timestamp)
#endif

#ifndef MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR
/**
Called at each execution of the task.
\param [in] _SystemMode system mode

Not mandatory. Must be defined as a value or a value returned by a function.
If undefined, they assume by default an empty value
\code
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(_SystemMode)       {}
\endcode
*/
#define MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR(_SystemMode)
#endif
/*@}*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Startup mode structure.
\sa ResetManager
*/
typedef struct Main_StartupModeInfoStruct
{
	SysMode_Type    sysmode;            /**< System Mode*/
	bool_t          coldreset;          /**< cold reset */
} Main_StartupModeInfoType, *pMain_StartupModeInfoType;


/* **************************** */
/* Prototype of local functions */
/* **************************** */



#if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
void Main_Framework_PeriodMeasureInit ( void );
void Main_Framework_PeriodMeasure ( void );

#else
#define Main_Framework_PeriodMeasureInit()
#define Main_Framework_PeriodMeasure()
#endif


/* local */
static void Main_NormalMode ( bool_t bIsColdReset );

static void Main_SupervisorTask ( void );

/* ********* */
/* Variables */
/* ********* */

/* Global */


/* Static */
#if defined(CONFIG_MAIN_SUPERVISOR_TASK_STACK_MONITOR)
static TOOLS_STACKUSAGE_MONITOR	Main_SupervisorStackMonitor;
#endif

#if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
/**
debug
*/
static TOOLS_PERIOD GP_Period;
#endif  /* MEASURE_PERIOD_TASK_PERIODIC_DEBUG*/


/**
@name Service variables
@{
*/
/** Supervisor Task Stack */
static STACK_SEC OS_STACKPTR int  Main_SupervisorTaskStack[CONFIG_MAIN_SUPERVISOR_TASK_STACK_SIZE] STACK_SEC_POST;


/** Init Task Task control block*/
static OS_TASK          Main_SupervisorTCB;


/** Global event for restore event. */
static OS_EVENT         Restore_Event;

/** Global event that signals Ref is good. */
static OS_EVENT         ZcSync_Event;


/*@}*/


#if defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)

typedef struct Main_SupervisorActivityStruct{
    Main_ActivityList_Type    Activity[2];
}Main_SupervisorActivitiesType;

Main_SupervisorActivitiesType Main_SupervisorActivities;
#endif




/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Main EmbOS framework entry point.
It's aim is to start OS creating InitTask.


\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
int main ( void )
{
	uint32_t startTime = OS_Time;

	OS_Clean();                     /* Extension to clean all OS objects */
	OS_IncDI();                      /* Initially disable interrupts  */

	/* If a warm reset occurred, restart with last time stamp
	 * otherwise all time-stamp based checks will fail!
	*/
	if (!LIB_Reset_IsColdReset())
	{
		OS_Time = startTime;
	}

	OS_InitKern();                   /* initialize OS                 */
	OS_InitHW();                     /* initialize Hardware for OS    */
	/* You need to create at least one task before calling OS_Start() */
	/*OS_SendString("Creating Init Task...");*/
	OS_CREATETASK ( &Main_SupervisorTCB,
					"Supervisor Task",
					Main_SupervisorTask,
					( OS_U8 ) MAIN_SUPERVISOR_TASK_PRIORITY,
					Main_SupervisorTaskStack );
	/*OS_SendString ( "created.\n" );
	//OS_CREATETASK(&TCBHP, "HP Task", HPTask, 150, StackHP);
	//OS_CREATETASK(&TCBLP, "LP Task", LPTask,  50, StackLP);*/
#if OS_TRACE   && 0
	OS_TraceDisableAll();
	OS_TraceEnableAll();
#endif
	Tools_StackUsageInit();

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK)
	LIB_IRQMonitor_Init();
#endif

	OS_Start();                      /* Start multitasking            */
	return 0;
}


#if defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)
Main_ActivityList_Type* Main_SupervisorGetActivityInstance(int id)
{
    /*TODO check index*/
    return &(Main_SupervisorActivities.Activity[id]);
}
#endif /* defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)	*/

/* Local Functions */


/**
Init Task: normal preemptable, task.

\author 	Roberto Fioravanti
\date		  13/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void Main_SupervisorTask ( void )
{
	int t0 = OS_GetTime();
	static SysMode_Type sysMode;
	static bool_t coldreset;
	static bool_t ZC_Ok = FALSE;

	OS_EnterRegion();
	/* here the scheduler is switched off */

	sysMode = LIB_Reset_GetSystemMode();
	coldreset = LIB_Reset_GetIsColdReset();

#if defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)
    Main_ActivityList_Init ( &(Main_SupervisorActivities.Activity[0]) );
#endif

    Main_OnSupervisorStart ();

	CALLBACK_Main_InitTask ( sysMode, coldreset );


#if defined(CONFIG_MAIN_SUPERVISOR_TASK_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_SupervisorTCB, &Main_SupervisorStackMonitor);
#endif

	switch ( sysMode )
	{
	case SYSTEM_PROGRAMMING_MODE:
		Main_EnableRemoteTimeout();
#if defined(CONFIG_PL_DEBUG) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
    	Sec_InitSafety();
#endif
		break;
	case SYSTEM_NORMAL_MODE:
		/* Create all Task ...*/
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
		Main_DispatcherInit();
#endif
		Main_SequencerInit();
		Main_LiveInit();
		Main_AnalogInit();
		Main_OutputsInit();
		Main_FeedbacksInit();
		Main_BackupInit();
		Main_ApplicationInit();
		OS_EVENT_Create ( &Restore_Event );
		OS_EVENT_Set ( &Restore_Event );
		/* ... then initialize hardware */

#if 	defined(CONFIG_LIB_CLASS_B_FLASH)
		CheckFlash_Init(TRUE);
#endif
		Main_NormalMode ( coldreset );
		break;
	case SYSTEM_SLEEP_MODE:
		break;
	default:;
	}

	OS_EVENT_Create ( &ZcSync_Event );

	CP_I2CCommunicationPlatformInit();

	OS_LeaveRegion();
	/* here the scheduler is switched on */


	Main_Framework_PeriodMeasureInit();

	while ( 1 )
	{
		TRACE_CPU_IDLE_END();

		/*debug begin */
		Main_Framework_PeriodMeasure();
		/*debug end*/

		MAIN_SUPERVISOR_TASK__MONITOR_IN()
        MAIN_SUPERVISOR_TASK_TRACE_BEGIN()

		( void ) OCWDT_RefreshSync();

		sysMode =   LIB_Reset_GetSystemMode();
		ZC_Ok   =   goodZCSignal();

#if OS_TRACE && SKELETON_TRACE
		OS_TraceData ( SKELETON_TRACE_ID_GP_HP_TASK, v );
#endif

		MAIN_SUPERVISOR_TASK__BODY ( sysMode,  ZC_Ok )

		/* Communication protocol monitoring */
		BCP_BusMonitor(); 
		MAIN_SUPERVISOR_TASK__SYSTEM_MONITOR ( sysMode )
        MAIN_SUPERVISOR_TASK_TRACE_END()
		MAIN_SUPERVISOR_TASK__MONITOR_OUT()

		OS_DelayUntil( t0 );
	}
}


/**
    If 'Zc' is synchronized sets ZcSync_Event to signaled state.
    Calls LIB_ZC_SyncCompleted.
*/
void Main_ZcSyncMonitor ( void )
{
	if ( LIB_ZC_SyncCompleted() )
	{
		OS_EVENT_Set ( &ZcSync_Event );
	}
	else
	{
		OS_EVENT_Reset ( &ZcSync_Event );
	}
}


/**
    If 'Zc' is synchronized sets Main_AnalogRef_Event to signaled state.
    Calls LIB_ADC_Is_Ref_Good.
*/
uint8_t Main_IsZcSync ( void )
{
	return OS_EVENT_Get ( &ZcSync_Event );
}

/**
Returns if restore is completed
\return uint8_t
\retval TRUE, completed
\retval FALSE non completed
\author 	Roberto Fioravanti
\date       22/04/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
uint8_t Main_IsRestoreCompleted ( void )
{
	return (uint8_t) !(OS_EVENT_Get (&Restore_Event));
}

/**
Signals restoring activity as completed
\author 	Roberto Fioravanti
\date       22/04/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void Main_CompleteRestore ( void )
{
	OS_EVENT_Reset ( &Restore_Event );
}


/**
*/
static void Main_NormalMode ( bool_t bIsColdReset )
{
	OCTPM_InitSync();
    ( void ) OCDIO_InitSync();
	( void ) OCADC_DeInitSync();
	( void ) OCADC_InitSync();

	SetupHw();

#if defined(CONFIG_PM_DOORLOCK)
	PM_DoorLock_InitModule(bIsColdReset);
#endif

	LIB_ZC_InitSync();
	LIB_IOM_IO_Manager_Init();
	
#if defined(Func_Main_PM_Init)
    Main_PM_Init();
#endif

    if ( TRUE == bIsColdReset )
    {
    	LIB_DIO_InitDigInputs();
        LIB_ADC_InitSync();
        LIB_ZcLoads_InitSync();
        LIB_FDB_InitSync();
#if defined (CONFIG_LIB_LOADSREQUEST_SUPPORT)
        LIB_IOM_InitLoadsPos();
#endif
    }
    else
    {
#if defined (CONFIG_LIB_LOADSREQUEST_SUPPORT)
    	LIB_IOM_SRPos_Init();
    	LIB_IOM_MRPos_Init();
#endif
    }
    
#if defined(CONFIG_DIGITAL_OUTPUT_MULTI_NUM_OF_LOADS)
    PM_DigDelayedMulti_InitSync();
#endif

    LIB_IOM_OutputInitialization( bIsColdReset );
    LIB_IOM_InputInitialization( bIsColdReset );
}

#if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
/**
debug only
\attention available if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
*/
void Main_Framework_PeriodMeasureInit ( void )
{
	Tools_CreatePeriodMeasure ( &GP_Period );
}
/**
debug only
\attention available if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
*/
void Main_Framework_PeriodMeasure ( void )
{
	Tools_PeriodMeasure ( &GP_Period );
}
#endif


/* *********** */
/* End of file */
/* *********** */
