/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		AnalogInputs
\n			This file contains the EmbOS-based service for the acquisition of analog channels (ADC).
It makes use of Platform Library modules.
\n
\file		Main_AnalogInputs.c
\ingroup	Main
\date		Feb 7, 2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.00
\n		29/03/2011
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
#include "Main.h"
#include "Analog.h"
#include "LIB_Trace.h"

#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
#include "PM_AnalogFeedback.h"
#endif /* CONFIG_PM_ANALOG_FEEDBACK */

#if defined(CONFIG_PM_TASK_MONITOR_SUPPORT)
#include "PM_TaskMonitoring.h"
#endif

#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
#include "LIB_TaskMonitoring.h"
#endif

#if defined(CONFIG_MAIN_ANALOG_TASK_TRACE_PIN_ENABLE)
#include "LIB_Trace.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */


/** 
@name Trace to pin
If CONFIG_MAIN_ANALOG_TASK_TRACE_PIN_ENABLE and CONFIG_LIB_TRACE_SUPPORT are both defined the pin
CONFIG_MAIN_ANALOG_TASK_TRACE_PIN geneates a pulse during the execution of a task.

\attention:
This section show the default definition that may not reflect the real configuration of your project.
@{
*/
#if defined(CONFIG_MAIN_ANALOG_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_ANALOG_TASK_TRACE_BEGIN()        TRACE_TO_PIN(ON , (CONFIG_MAIN_ANALOG_TASK_TRACE_PIN) )
#define MAIN_ANALOG_TASK_TRACE_END()          TRACE_TO_PIN(OFF , (CONFIG_MAIN_ANALOG_TASK_TRACE_PIN) )
#else
#define MAIN_ANALOG_TASK_TRACE_BEGIN()	{}
#define MAIN_ANALOG_TASK_TRACE_END()	{}
#endif
/*@}*/

#if !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_IN) && \
    !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT) && \
    !defined(START_ANALOG_TASK_MONITORING)
      
/** @name Analog task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.
@{
*/
#define START_ANALOG_TASK_MONITORING()
/** safety entry */
#define MAIN_ANALOG_INPUTS_TASK__MONITOR_IN()
/** safety exit */
#define MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT()
/*@}*/

#else
/* MAIN_ANALOG_INPUTS_TASK__MONITOR_IN or MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT defined */
#if !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_IN)
#error "MAIN_ANALOG_INPUTS_TASK__MONITOR_IN() missing definition."
#error "START_ANALOG_TASK_MONITORING, MAIN_ANALOG_INPUTS_TASK__MONITOR_IN() and MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT() must be defined."
#endif

#if !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT)
#error "MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT() missing definition."
#error "START_ANALOG_TASK_MONITORING, MAIN_ANALOG_INPUTS_TASK__MONITOR_IN() and MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT() must be defined."
#endif
      
#if !defined(START_ANALOG_TASK_MONITORING)
#error "START_ANALOG_TASK_MONITORING() missing definition."
#error "START_ANALOG_TASK_MONITORING, MAIN_ANALOG_INPUTS_TASK__MONITOR_IN() and MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT() must be defined."
#endif
             
#endif /* !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_IN) && !defined(MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT) */


/**
@name Debug Configuration
\par Properties
Not mandatory. If not defined they assume the default values

In order to save ram and flash this options should be disabled.
\code
#define MAIN_ANALOG_INPUTS_TASK_DEBUG                (1)
#define MAIN_ANALOG_INPUTS_TASK_DURATION             (1)
#define MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC          (1)
#define MAIN_ANALOG_INPUTS_TASK_TUNING               (1)
\endcode
@{
*/
#if !defined(MAIN_ANALOG_INPUTS_TASK_DEBUG)

/** Debug enable
\li 0: all debug features disabled
\li 1: allows to enable the debug level for MAIN_ANALOG_INPUTS_TASK_DURATION, MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC
MAIN_ANALOG_INPUTS_TASK_TUNING
*/
#define MAIN_ANALOG_INPUTS_TASK_DEBUG                (1)
#endif
#if !defined(MAIN_ANALOG_INPUTS_TASK_DURATION)
/** Measurement of the duration of the task
\li 0: disabled
\li 1: enabled

If enabled defines:
\li Main_AnalogInputsTask_Duration_Init
\li Main_AnalogInputsTask_Duration_End
\li Main_AnalogInputsTask_Duration_Start
*/
#define MAIN_ANALOG_INPUTS_TASK_DURATION		(1)
#endif

#if !defined(MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC)
/** delay measurement of the activation of the task in [us] 
\li 0: disabled
\li 1: enabled

If enabled defines:
\li Main_AnalogInputsTask_DelayOnZC_Init
\li Main_AnalogInputsTask_DelayOnZC_SetRef
\li Main_AnalogInputsTask_DelayOnZC_GetTiming
*/
#define MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC		(1)
#endif

#if !defined(MAIN_ANALOG_INPUTS_TASK_TUNING)
/** Allows dynamic correction of the delay of the activation of the task in [us]. This options provides the
variable AnalogInputsTuning that allows the correction of delay the task activation.

Once the system is tuned, it is good practice apply the value to the property MAIN_ANALOG_INPUTS_ACTIVATION_TUNING_VAL
in order to save ram.
 
\li 0: disabled
\li 1: enabled

If enabled defines:
\li Main_AnalogInputsTask_Tuning_Init 
\li Main_AnalogInputsTask_Tuning_Tuner
*/
#define MAIN_ANALOG_INPUTS_TASK_TUNING               (1)
#endif
/*@}*/




/**
@name Static Tuning
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define ANALOG_INPUTS_ACTIVATION_TUNING_VAL         (0)
\endcode
\attention Used only if \ref ANALOG_INPUTS_TASK_DEBUG is defined as 0, otherwise 
dynamic correction is available.
@{
*/

#ifndef MAIN_ANALOG_INPUTS_ACTIVATION_TUNING_VAL
/**
Signed static delay correction in [us] analog task activation
\par Type
Type: OS_I32
*/
#define MAIN_ANALOG_INPUTS_ACTIVATION_TUNING_VAL             (0)
#endif  /* ANALOG_INPUTS_OFFSET_ACTIVATION */

/*}@*/
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
static STACK_SEC OS_STACKPTR int  Main_AnalogInputsTaskStack[CONFIG_MAIN_ANALOGINPUTS_TASK_STACK_SIZE] STACK_SEC_POST;

/** Task Control Block*/
static OS_TASK          Main_AnalogInputsTaskTCB;

/** high resolution Alarm */
static OS_ALARM         Main_AlmMeasureAnalog;

/** Global event that signals Ref is good. */
static OS_EVENT         Main_AnalogRef_Event;
/*@}*/

#if defined(CONFIG_MAIN_ANALOG_INPUTS_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_AnalogInputsStackMonitor;
#endif

/**
@name Debug - Delay measurement
@{
*/
#if     MAIN_ANALOG_INPUTS_TASK_DEBUG

#if     MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC
/** task delay in [us]
\attention Available only if \ref MAIN_ANALOG_INPUTS_TASK_DEBUG and MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC are
defined as 1.
*/
static  TOOLS_TIMING    AnalogInputsDelayOnZC;
#endif  /* ANALOG_INPUTS_TASK_DELAY_ON_ZC */
/*@}*/


/**
@name Debug - Duration measurement
@{
*/
#if     MAIN_ANALOG_INPUTS_TASK_DURATION
/** task duration in [us]
\attention Available only if \ref MAIN_ANALOG_INPUTS_TASK_DEBUG and MAIN_ANALOG_INPUTS_TASK_DURATION are
defined as 1.
*/
static  TOOLS_TIMING    AnalogInputsDuration;
/** statistics about task duration [us]
\attention Available only if \ref MAIN_ANALOG_INPUTS_TASK_DEBUG and MAIN_ANALOG_INPUTS_TASK_DURATION are
defined as 1.
*/
static  TOOLS_TIMING_STATS  AnalogInputsTaskDurationStat;
#endif  /* MAIN_ANALOG_INPUTS_TASK_DURATION */
/*@}*/

/**
@name Debug - Dynamic Tuning
@{
*/
#if     MAIN_ANALOG_INPUTS_TASK_TUNING
/** Dynamic delay correction in [us] of the task activation
\attention Available only if \ref MAIN_ANALOG_INPUTS_TASK_DEBUG and MAIN_ANALOG_INPUTS_TASK_TUNING are
defined as 1.
*/
static  OS_I32  AnalogInputsTuning;
#endif /* MAIN_ANALOG_INPUTS_TASK_ACTIVATION_TUNING */

#endif  /* MAIN_ANALOG_INPUTS_TASK_DEBUG */
/*@}*/



/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */

static void AlmAnalogCallback ( void );
static void Main_AnalogInputsTask ( void );


/**
@name Debug - Delay measurement
They are called by framework to measure the delay between the event and the intant of the task activation.
@{
*/

/*lint -idlen(60) */
/*lint -esym(621,Main_AnalogInputsTask_DelayOnZC_GetTiming)*/
/*lint -esym(621,Main_AnalogInputsTask_DelayOnZC_SetRef)*/


#if     MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC
static void Main_AnalogInputsTask_DelayOnZC_Init ( void );
static void Main_AnalogInputsTask_DelayOnZC_SetRef ( void );
static void Main_AnalogInputsTask_DelayOnZC_GetTiming ( void );
#else
#define Main_AnalogInputsTask_DelayOnZC_Init()
#define Main_AnalogInputsTask_DelayOnZC_SetRef()
#define Main_AnalogInputsTask_DelayOnZC_GetTiming()
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC */
/*@}*/


/**
@name Debug - Duration Measurement
They are called by framework to measure the task duration
@{
*/

#if     MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DURATION
void Main_AnalogInputsTask_Duration_Init ( void );
void Main_AnalogInputsTask_Duration_Start ( void );
void Main_AnalogInputsTask_Duration_End ( void );
#else
#define Main_AnalogInputsTask_Duration_Init()
#define Main_AnalogInputsTask_Duration_Start()
#define Main_AnalogInputsTask_Duration_End()
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DURATION */
/*@}*/


/**
@name Debug - Dynamic Tuning
They are called by framework to apply the correction to the delay.
@{
*/
#if     MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_TUNING
void Main_AnalogInputsTask_Tuning_Init ( void );
OS_I32 Main_AnalogInputsTask_Tuner ( OS_U32 usOffset );
#else
#define Main_AnalogInputsTask_Tuning_Init()
#define Main_AnalogInputsTask_Tuner(dummy)     ((OS_I32)MAIN_ANALOG_INPUTS_ACTIVATION_TUNING_VAL)
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_TUNING */
/*@}*/



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
@name APIs
\page APIs APIs

*/

/**
Analog inputs service initialization.
It creates task Main_AnalogInputsTask and the high resolution alarm.
If debug (\ref ANALOG_INPUTS_TASK_DEBUG and \ref ANALOG_INPUTS_TASK_DELAY_ON_ZC or
\ref ANALOG_INPUTS_TASK_DURATION or \ref ANALOG_INPUTS_TASK_TUNING) is enabled
initializes tools for timing measurement.
*/
void Main_AnalogInit ( void )
{
	OS_CREATETASK ( &Main_AnalogInputsTaskTCB,
					"AnalogInputsTask",
					Main_AnalogInputsTask,
					( OS_U8 ) MAIN_ANALOG_TASK_PRIORITY,
					Main_AnalogInputsTaskStack );
	OS_CreateAlarm ( &Main_AlmMeasureAnalog, AlmAnalogCallback, ( OS_U32 ) 0, ( OS_U32 ) 0 );
	OS_EVENT_Create ( &Main_AnalogRef_Event );
	Main_AnalogInputsTask_DelayOnZC_Init();
	Main_AnalogInputsTask_Duration_Init();
	Main_AnalogInputsTask_Tuning_Init();
#if defined(CONFIG_MAIN_ANALOG_INPUTS_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_AnalogInputsTaskTCB, &Main_AnalogInputsStackMonitor);
#endif
}


/**
    If 'Ref' is good sets Main_AnalogRef_Event to signaled state.
    Calls LIB_ADC_Is_Ref_Good.
*/
void Main_AnalogRefMonitor ( void )
{
	if ( LIB_ADC_Is_Ref_Good() )
	{
		OS_EVENT_Set ( &Main_AnalogRef_Event );
	}
	else
	{
		OS_EVENT_Reset ( &Main_AnalogRef_Event );
	}
}


/**
    If 'Ref' is good sets Main_AnalogRef_Event to signaled state.
    Calls LIB_ADC_Is_Ref_Good.
*/
uint8_t Main_Analog_IsRefOk ( void )
{
	return OS_EVENT_Get ( &Main_AnalogRef_Event );
}


/**
Resumes task
*/
void Main_ActivateAnalogInputs ( void )
{
	if ( OS_GetSuspendCnt ( &Main_AnalogInputsTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_AnalogInputsTaskTCB );
	}
}

/**
Sets the Alarm on high resolution timer for analog task

\param [in] usOffset alarm expiration in microseconds
\attention For sync readings it must bel called in sync with ZC.

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
void Main_ScheduleAnalogMeasure ( OS_U32 usOffset, OS_U32 TickRef )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;	
	usOffset += Main_AnalogInputsTask_Tuner ( usOffset );
	if(OS_IsTask(&Main_AnalogInputsTaskTCB))
    {
        OS_START_ABSALARM_ONESHOT ( &Main_AlmMeasureAnalog, usOffset, &ref );
    }
	Main_AnalogInputsTask_DelayOnZC_SetRef();
}


/**
Sets perodic alarm on high resolution timer for analog task

\param [in] usOffset - alarm delay in microseconds
\param [in] usPeriod - alarm period in microseconds

\attention For sync readings it must bel called in sync with ZC.

\author 	Alessandro Sabbatini
\date		26/02/2014
\version	1.0.0
*/
void Main_SchedulePeriodicAnalogMeasure ( OS_U32 usOffset, OS_U32 usPeriod, OS_U32 TickRef )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
	usOffset += Main_AnalogInputsTask_Tuner ( usOffset );
	if(OS_IsTask(&Main_AnalogInputsTaskTCB))
    {
        OS_START_ABSALARM(&Main_AlmMeasureAnalog, usOffset, usPeriod, -1, &ref );
    }
	Main_AnalogInputsTask_DelayOnZC_SetRef();
}

/*@}*/




/* Local Functions */

/**
@name Local functions
*/


/**
normal Non-preemptable periodic task for ADC channels readings.
When its activity has been completed the task suspends itself, waiting for the next alamr expiration to be resumed
\warning	none

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
static void Main_AnalogInputsTask ( void )
{
	START_ANALOG_TASK_MONITORING();
	while ( 1 )
	{
		TRACE_CPU_IDLE_END();

		OS_Suspend ( (OS_TASK*)NULL );
        MAIN_ANALOG_TASK_TRACE_BEGIN();
		/*OS_EnterRegion();*/
		/* here the scheduler is switched off */
		/* debug: begin */
		Main_AnalogInputsTask_DelayOnZC_GetTiming();
		Main_AnalogInputsTask_Duration_Start();
		/* debug: end */
		MAIN_ANALOG_INPUTS_TASK__MONITOR_IN()
#if defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
		PM_AnalogFeedbackManager();
#endif /* CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT*/
		LIB_ADC_Manager();
		ANALOG_INPUT_TASK__BODY ( 1 );
		MAIN_ANALOG_INPUTS_TASK__MONITOR_OUT()
		/* debug: begin */
		Main_AnalogInputsTask_Duration_End();
		/* debug: end */
		/*OS_LeaveRegion();*/
		/* here the scheduler is switched on */
        MAIN_ANALOG_TASK_TRACE_END();
	}
}







#if MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC
/**
Init Duration  measurement
*/
static void Main_AnalogInputsTask_DelayOnZC_Init ( void )
{
	Tools_CreateTimingMeasure ( &AnalogInputsDelayOnZC );
}
/**
Starts the delay measurement
*/
static void Main_AnalogInputsTask_DelayOnZC_SetRef ( void )
{
	Tools_TimingSetRef ( &AnalogInputsDelayOnZC );
}

/**
Stops the delay measurement
*/
static void Main_AnalogInputsTask_DelayOnZC_GetTiming ( void )
{
	( void ) Tools_GetTiming ( &AnalogInputsDelayOnZC );
}
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG   &&   MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC */


#if MAIN_ANALOG_INPUTS_TASK_DEBUG && MAIN_ANALOG_INPUTS_TASK_DURATION
void Main_AnalogInputsTask_Duration_Init ( void )
{
	Tools_CreateTimingMeasure ( &AnalogInputsDuration );
	Tools_CreateTimingStats ( &AnalogInputsTaskDurationStat );
}

void Main_AnalogInputsTask_Duration_Start ( void )
{
	Tools_TimingSetRef ( &AnalogInputsDuration );
}

void Main_AnalogInputsTask_Duration_End ( void )
{
	Tools_ProcessValue ( &AnalogInputsTaskDurationStat, Tools_GetTiming ( &AnalogInputsDuration ) );
}
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG && MAIN_ANALOG_INPUTS_TASK_DURATION */


#if     MAIN_ANALOG_INPUTS_TASK_DEBUG && MAIN_ANALOG_INPUTS_TASK_TUNING
void Main_AnalogInputsTask_Tuning_Init ( void )
{
	AnalogInputsTuning = 0;
}

OS_I32 Main_AnalogInputsTask_Tuner ( OS_U32 usOffset )
{
	if ( AnalogInputsTuning < 0 )
	{
		if ( -AnalogInputsTuning > usOffset )
		{
			AnalogInputsTuning = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return AnalogInputsTuning;
}
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG && MAIN_ANALOG_INPUTS_TASK_TUNING */


/**
High resolution timer Alarm callback is called when Main_AlmMeasureAnalog expires.
It resumes task Main_AnalogInputsTask

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
static void AlmAnalogCallback ( void )
{
	Main_ActivateAnalogInputs();
}


/*@}*/

/* *********** */
/* End of file */
/* *********** */
