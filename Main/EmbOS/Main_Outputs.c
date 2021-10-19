/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			This module implements the service for loads synchronized with ZC .
            It makes use of a task which is resumed more times at the beginning of each sub-interval
            in which the halfwave is divided.
\n
\file		Main_Outputs.c
\ingroup	Main
\date		12/01/2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.00
\n		12/01/2011
\n		Roberto Fioravanti
\n		changes
\n 		- Creation
\n
\n
*/
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "Main.h"
#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
#include "LIB_TaskMonitoring.h"
#endif
#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_MAIN_OUTPUTS_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_OUTPUTS_TASK_TRACE_BEGIN()		TRACE_TO_PIN(ON, (CONFIG_MAIN_OUTPUTS_TASK_TRACE_PIN) )
#define MAIN_OUTPUTS_TASK_TRACE_END()		TRACE_TO_PIN(OFF , (CONFIG_MAIN_OUTPUTS_TASK_TRACE_PIN) )
#else
#define MAIN_OUTPUTS_TASK_TRACE_BEGIN()		{}
#define MAIN_OUTPUTS_TASK_TRACE_END()		{}
#endif


/**
@name Outputs task safety
Outputs task safety is available if CONFIG_LIB_CLASS_B_PERIODIC_CHECK or CONFIG_PM_TASK_MONITOR_SUPPORT are defined.
Both require that CONFIG_MAIN_OUTPUT_TASK_TASKMONITOR_ENABLE is defined.
\attention:
This section show the default definition that may not reflect the real configuration of your project.
@{
*/

#if !defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK) || !defined(CONFIG_LIB_TM_OUTPUT)
#define START_OUTPUT_TASK_MONITORING()		{}
#define MAIN_OUTPUT_TASK__MONITOR_IN()		{}
#define MAIN_OUTPUT_TASK__MONITOR_OUT()		{}
#endif
/*@}*/



/** @name General macros

\par Properties
Not mandatory.

\par Examples

\li idEvent is unused
\code
#define MAIN_OUTPUTS_TASK__BODY(idEvent)\
{\
    (void)LIB_ZcLoads_Manager(0);\
}
\endcode
@{
*/

#if !defined(MAIN_OUTPUTS_TASK__BODY)
/**
Body of Synchronized loads task.
It can remain undefined if not needed.
\param [in] idEvent it identifies each interval in which the half wave is divided. It assumes values from 0 (@ each ZC) to NLEVEL-1

*/
#define MAIN_OUTPUTS_TASK__BODY(idEvent)
#endif /*MAIN_OUTPUTS__PROFILER*/
/*@}*/

/**
@name Debug macros
\par Properties
Not mandatory. If not defined they assume the default values
\li #define OUTPUTS_TASK_DEBUG                (1)
\li #define OUTPUTS_TASK_DURATION             (1)
\li #define OUTPUTS_TASK_TUNING               (1)
@{
*/

#if !defined(OUTPUTS_TASK_DEBUG)
/** Debug enable
\li 0: all debug features disabled
\li 1: allows to enable the debug level for OUTPUTS_TASK_DURATION, OUTPUTS_TASK_TUNING
*/
#define OUTPUTS_TASK_DEBUG                (1)
#endif

#if !defined(OUTPUTS_TASK_DURATION)
/** Measurement of the duration of the task
\li 0: disabled
\li 1: enabled
*/
#define OUTPUTS_TASK_DURATION             (1)
#endif

#if !defined(OUTPUTS_TASK_TUNING)
/** Dynamic Tuning. It allows to change dynamically the period and the delay of the exection of the task
by changing the value of the variables OutputsTuning_Delay and OutputsTuning_Period.
\li 0: disabled
\li 1: enabled

\attention When OUTPUTS_TASK_TUNING is defined as 1,  OUTPUTS_DELAY_ACTIVATION_TUNING_VAL and OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL
are not used
*/
#define OUTPUTS_TASK_TUNING               (1)
#endif

/*@}*/


/**
@name Tuning macros
\par Properties
Not mandatory. If not defined they assume the default values
\li #define OUTPUTS_DELAY_ACTIVATION_TUNING_VAL         (0)
\li #define OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL         (0)
\attention Used only if \ref OUTPUTS_TASK_DEBUG is defined as 0.
@{
*/
#ifndef OUTPUTS_DELAY_ACTIVATION_TUNING_VAL
#define OUTPUTS_DELAY_ACTIVATION_TUNING_VAL         (0)     /**< First outputs activation delay correction in [us] */
#endif

#ifndef OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL
#define OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL         (0)    /**< Outputs activation period correction in in [us]*/
#endif
/*@}*/

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
#if !defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT )
/** Task Stack */
static STACK_SEC OS_STACKPTR int  Main_OutputsTaskStack[CONFIG_MAIN_OUTPUTS_TASK_STACK_SIZE] STACK_SEC_POST;

/** Task Control Block*/
static OS_TASK          Main_OutputsTaskTCB;

/** high resolution Alarm */
static OS_ALARM         Main_AlmOutputs;
/** current of the sub-interval in which the halfwave is divided */
static uint8_t          OutputCnt;
#endif
/*@}*/


/**
@name Debug variables

@{
*/
#if     OUTPUTS_TASK_DEBUG


#if     OUTPUTS_TASK_DURATION
static  TOOLS_TIMING        OutputsDuration;        /**< task duration in [us]
                                                    \attention Available only if \ref OUTPUTS_TASK_DEBUG and OUTPUTS_TASK_DURATION are
                                                    defined as 1.
                                                    */
static  TOOLS_TIMING_STATS  OutputsTaskDurationStat;/**< statistics about task duration [us]
                                                    \attention Available only if \ref OUTPUTS_TASK_DEBUG and OUTPUTS_TASK_DURATION are
                                                                                                        defined as 1.
                                                    */
#endif  /* OUTPUTS_TASK_DURATION */


#if		OUTPUTS_TASK_TUNING
static  OS_I32  OutputsTuning_Delay;    /**< Current delay correction for first activation of the task in [us]
                                        \attention Available only if \ref OUTPUTS_TASK_DEBUG and OUTPUTS_TASK_TUNING are
                                        defined as 1.
                                        */
static  OS_I32  OutputsTuning_Period;   /**< Current period correction for subsequent activations of the task in [us]
                                        \attention Available only if \ref OUTPUTS_TASK_DEBUG and OUTPUTS_TASK_TUNING are
                                        defined as 1.
                                        */
#endif /* OUTPUTS_TASK_TUNING */

#endif  /* OUTPUTS_TASK_DEBUG */
/*@}*/
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
/* Local Function */
#if !defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT )
static void Main_OutputsTask ( void );
static void AlmOutputsCallback ( void );
#endif



#if     OUTPUTS_TASK_DEBUG   &&   OUTPUTS_TASK_DURATION
void Main_OutputsTask_Duration_Init ( void );
void Main_OutputsTask_Duration_Start ( void );
void Main_OutputsTask_Duration_End ( void );
#else
#define Main_OutputsTask_Duration_Init()
#define Main_OutputsTask_Duration_Start()
#define Main_OutputsTask_Duration_End()
#endif /* OUTPUTS_TASK_DEBUG   &&   OUTPUTS_TASK_DURATION */

#if     OUTPUTS_TASK_DEBUG   &&   OUTPUTS_TASK_TUNING
void Main_OutputsTask_Tuning_Init ( void );
OS_I32 Main_OutputsTask_DelayTuner ( OS_U32 usOffset );
OS_I32 Main_OutputsTask_PeriodTuner ( OS_U32 usOffset );
#else
#define Main_OutputsTask_Tuning_Init()
#define Main_OutputsTask_DelayTuner(dummy)     ((OS_I32)OUTPUTS_DELAY_ACTIVATION_TUNING_VAL)
#define Main_OutputsTask_PeriodTuner(dummy)    ((OS_I32)OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL)
#endif /* OUTPUTS_TASK_DEBUG   &&   OUTPUTS_TASK_TUNING */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Outputs service initialization.
It creates task \ref Main_OutputsTask and \ref Main_AlmOutputs alarm.
If debug option OUTPUTS_TASK_DEBUG and
\li \ref OUTPUTS_TASK_TUNING or
\li \ref OUTPUTS_TASK_DURATION
\n are enabled it initializes tools for timing measurement.
*/
#if !defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT )
void Main_OutputsInit ( void )
{
	OS_CREATETASK ( &Main_OutputsTaskTCB,
					"OutputsTask",
					Main_OutputsTask,
					MAIN_OUTPUTS_TASK_PRIORITY,
					Main_OutputsTaskStack );
	OS_CreateAlarm ( &Main_AlmOutputs, AlmOutputsCallback, ( OS_U32 ) 0, ( OS_U32 ) 0 );
	Main_OutputsTask_Duration_Init();
	Main_OutputsTask_Tuning_Init();
}

/**
Sets the alarm on high resolution timer to resume Outputs Task at the beginning of each sub-interval of the halfwave
It must be called on ZC event.
At each expiration of the alarm the task is resumed.

\param [in] usOffset first alarm expiration in microseconds
\param [in] usPeriod alarm period in microseconds
\param [in] count number of expiration in microseconds (See alarm documentation)

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
void Main_ScheduleOutputs ( OS_U32 usOffset, OS_U32 usPeriod, OS_I8 count, OS_U32 TickRef )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
	usOffset += Main_OutputsTask_DelayTuner ( usOffset );
	usPeriod += Main_OutputsTask_PeriodTuner ( usPeriod );
	if(OS_IsTask(&Main_OutputsTaskTCB))
	{
	    OS_START_ABSALARM ( &Main_AlmOutputs, usOffset, usPeriod, count, &ref );
	}
	OutputCnt = 0;
	
}
#endif /* defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT ) */
/* Local Function */
#if !defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT )
/**
Outputs Alarm callback that resumes Outputs task
*/
static void AlmOutputsCallback ( void )
{
	if ( OS_GetSuspendCnt ( &Main_OutputsTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_OutputsTaskTCB );
	}
}




/**
normal Non-preemptable periodic task for ADC channels readings

\warning	none

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
static void Main_OutputsTask ( void )
{
	START_OUTPUT_TASK_MONITORING();
	while ( 1 )
	{
		OS_Suspend ( NULL );
		
		TRACE_CPU_IDLE_END();

		MAIN_OUTPUTS_TASK_TRACE_BEGIN()
		OS_EnterRegion();
		/* here the scheduler is switched off */
		/* debug: begin */
		Main_OutputsTask_Duration_Start();
		/* debug: end */
		MAIN_OUTPUT_TASK__MONITOR_IN();
		OutputCnt++;
		MAIN_OUTPUTS_TASK__BODY ( OutputCnt );
		MAIN_OUTPUT_TASK__MONITOR_OUT();
		/* debug: begin */
		Main_OutputsTask_Duration_End();
		/* debug: end */
		OS_LeaveRegion();
        MAIN_OUTPUTS_TASK_TRACE_END()
		/* here the scheduler is switched on */
	}
}




#if OUTPUTS_TASK_DEBUG && OUTPUTS_TASK_DURATION
/**
Init Duration  measurement
*/
static void Main_OutputsTask_Duration_Init ( void )
{
	Tools_CreateTimingMeasure ( &OutputsDuration );
	Tools_CreateTimingStats ( &OutputsTaskDurationStat );
}
/**
Starts a duration measurement
*/
static void Main_OutputsTask_Duration_Start ( void )
{
	Tools_TimingSetRef ( &OutputsDuration );
}

/**
Stops the duration measurement
*/
static void Main_OutputsTask_Duration_End ( void )
{
	Tools_ProcessValue ( &OutputsTaskDurationStat, Tools_GetTiming ( &OutputsDuration ) );
}
#endif /* OUTPUTS_TASK_DEBUG && OUTPUTS_TASK_DURATION */


#if     OUTPUTS_TASK_DEBUG && OUTPUTS_TASK_TUNING
/**
Tuning initialization.
Resets OutputsTuning_Delay and OutputsTuning_Period at zero.
*/
static void Main_OutputsTask_Tuning_Init ( void )
{
	OutputsTuning_Delay = 0;
	OutputsTuning_Period = 0;
}

/**
Filters OutputsTuning_Delay.
\param [in] usOffset current delay
\return OS_I32
\retval OutputsTuning_Delay.
\attention range of OutputsTuning_Delay:
from -usOffset to max of OS_I32
*/
static OS_I32 Main_OutputsTask_DelayTuner ( OS_U32 usOffset )
{
	if ( OutputsTuning_Delay < 0 )
	{
		if ( -OutputsTuning_Delay > usOffset )
		{
			OutputsTuning_Delay = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return OutputsTuning_Delay;
}

/**
Filters OutputsTuning_Period.
\param [in] usOffset current period
\return OS_I32
\retval OutputsTuning_Delay.
\attention range of OutputsTuning_Delay:
from -usOffset to max of OS_I32
*/
static OS_I32 Main_OutputsTask_PeriodTuner ( OS_U32 usOffset )
{
	if ( OutputsTuning_Period < 0 )
	{
		if ( -OutputsTuning_Period > usOffset )
		{
			OutputsTuning_Period = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return OutputsTuning_Period;
}
#endif /* OUTPUTS_TASK_DEBUG && OUTPUTS_TASK_TUNING */

#endif /* defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT ) */
/* *********** */
/* End of file */
/* *********** */
