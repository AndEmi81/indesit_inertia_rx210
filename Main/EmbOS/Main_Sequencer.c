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

#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)

#include "TaskMonitor_Cfg.h"
#include "Main.h"
#include "Main_ActivityList.h"
#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_MAIN_SEQUENCER_LENGTH_2)
#define MAIN_SEQUENCER_LENGTH                   (2)
#elif defined(CONFIG_MAIN_SEQUENCER_LENGTH_4)
#define MAIN_SEQUENCER_LENGTH                   (4)
#else
#error "CONFIG_MAIN_SEQUENCER_LENGTH_2 or CONFIG_MAIN_SEQUENCER_LENGTH_4 missing definition."
#endif


#if defined(CONFIG_MAIN_SEQUENCER_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_SEQUENCER_TASK_TRACE_BEGIN()        TRACE_TO_PIN(ON, (CONFIG_MAIN_SEQUENCER_TASK_TRACE_PIN) )
#define MAIN_SEQUENCER_TASK_TRACE_END()          TRACE_TO_PIN(OFF , (CONFIG_MAIN_SEQUENCER_TASK_TRACE_PIN) )
#else
#define MAIN_SEQUENCER_TASK_TRACE_BEGIN()
#define MAIN_SEQUENCER_TASK_TRACE_END()
#endif

#if !defined(CONFIG_LIB_TM_MAIN)
/** @name Synchronized loads task safety

\par Properties
Not mandatory. They can remain undefined.
The configuration is defined in the file TaskMonitor_Cgf.h
@{
*/
#define START_SEQUENCER_TASK_MONITORING()
#define MAIN_SEQUENCER_TASK__MONITOR_IN()          /**< safety entry */
#define MAIN_SEQUENCER_TASK__MONITOR_OUT()         /**< safety exit */
/*@}*/
#endif

#if !defined(MAIN_SEQUENCER_TASK__MONITOR_IN) && !defined(MAIN_SEQUENCER_TASK__MONITOR_OUT) && !defined(START_SEQUENCER_TASK_MONITORING)
/** @name Sequencer task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

\par Examples
\code
#define MAIN_SEQUENCER_TASK__MONITOR_IN()     helloTaskMonitor( APPLICATION_TASK_ID );
#define MAIN_SEQUENCER_TASK__MONITOR_OUT()
\endcode
@{
*/

/** safety config */
#define START_SEQUENCER_TASK_MONITORING()
/** safety entry */
#define MAIN_SEQUENCER_TASK__MONITOR_IN()
/** safety exit */
#define MAIN_SEQUENCER_TASK__MONITOR_OUT()
/*@}*/

#else
#if !defined(START_SEQUENCER_TASK_MONITORING)
#error "START_SEQUENCER_TASK_MONITORING not defined"
#endif

#if !defined(MAIN_SEQUENCER_TASK__MONITOR_IN)
#error "MAIN_SEQUENCER_TASK__MONITOR_IN not defined"
#endif

#if !defined(MAIN_SEQUENCER_TASK__MONITOR_OUT)
#error "MAIN_SEQUENCER_TASK__MONITOR_OUT not defined"
#endif

#endif

/** @name General macros

\par Properties
Not mandatory.

\par Examples

\li idEvent is unused
\code
#define MAIN_SEQUENCER_TASK__BODY(idEvent)\
{\
    (void)LIB_ZcLoads_Manager(0);\
}
\endcode
@{
*/


/*@}*/

/**
@name Debug macros
\par Properties
Not mandatory. If not defined they assume the default values
\li #define SEQUENCER_TASK_DEBUG                (1)
\li #define SEQUENCER_TASK_DURATION             (1)
\li #define SEQUENCER_TASK_TUNING               (1)
@{
*/

#if !defined(SEQUENCER_TASK_DEBUG)
/** Debug enable
\li 0: all debug features disabled
\li 1: allows to enable the debug level for SEQUENCER_TASK_DURATION, SEQUENCER_TASK_TUNING
*/
#define SEQUENCER_TASK_DEBUG                (1)
#endif

#if !defined(SEQUENCER_TASK_DURATION)
/** Measurement of the duration of the task
\li 0: disabled
\li 1: enabled
*/
#define SEQUENCER_TASK_DURATION             (1)
#endif

#if !defined(SEQUENCER_TASK_TUNING)
/** Dynamic Tuning. It allows to change dynamically the period and the delay of the exection of the task
by changing the value of the variables OutputsTuning_Delay and OutputsTuning_Period.
\li 0: disabled
\li 1: enabled

\attention When SEQUENCER_TASK_TUNING is defined as 1,  SEQUENCER_DELAY_ACTIVATION_TUNING_VAL and SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL
are not used
*/
#define SEQUENCER_TASK_TUNING               (1)
#endif

/*@}*/


/**
@name Tuning macros
\par Properties
Not mandatory. If not defined they assume the default values
\li #define SEQUENCER_DELAY_ACTIVATION_TUNING_VAL         (0)
\li #define SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL         (0)
\attention Used only if \ref SEQUENCER_TASK_DEBUG is defined as 0.
@{
*/
#ifndef SEQUENCER_DELAY_ACTIVATION_TUNING_VAL
#define SEQUENCER_DELAY_ACTIVATION_TUNING_VAL           (0)     /**< First outputs activation delay correction in [us] */
#endif

#ifndef SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL
#define SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL          (0)    /**< Outputs activation period correction in in [us]*/
#endif
/*@}*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct Main_SequencerCBStruct Main_SequencerCB_Type;
struct Main_SequencerCBStruct
{
    Main_SequencerProfileType           SequencerID;            /* ID of the sequence */
    Main_ActivityList_Type              SeqActivityList;        /* Standard Activity list manager */
};

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
static STACK_SEC OS_STACKPTR int  Main_SequencerTaskStack[CONFIG_MAIN_SEQUENCER_TASK_STACK_SIZE] STACK_SEC_POST;

/** Task Control Block*/
static OS_TASK                  Main_SequencerTaskTCB;

/** high resolution Alarm */
static OS_ALARM                 Main_AlmSequencer;

/* first object */
static Main_SequencerCB_Type    Main_Sequencer;
/*@}*/

#if defined(CONFIG_MAIN_SEQUENCER_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_SequencerStackMonitor;
#endif

/**
@name Debug variables

@{
*/
#if     SEQUENCER_TASK_DEBUG


#if     SEQUENCER_TASK_DURATION
static  TOOLS_TIMING            SequencerDuration;        /**< task duration in [us]
                                                    \attention Available only if \ref SEQUENCER_TASK_DEBUG and SEQUENCER_TASK_DURATION are
                                                    defined as 1.
                                                    */
static  TOOLS_TIMING_STATS      SequencerTaskDurationStat;/**< statistics about task duration [us]
                                                    \attention Available only if \ref SEQUENCER_TASK_DEBUG and SEQUENCER_TASK_DURATION are
                                                                                                        defined as 1.
                                                    */
#endif  /* SEQUENCER_TASK_DURATION */


#if		SEQUENCER_TASK_TUNING
static  OS_I32                  SequencerTuning_Delay;    /**< Current delay correction for first activation of the task in [us]
                                        \attention Available only if \ref SEQUENCER_TASK_DEBUG and SEQUENCER_TASK_TUNING are
                                        defined as 1.
                                        */
static  OS_I32                  SequencerTuning_Period;   /**< Current period correction for subsequent activations of the task in [us]
                                        \attention Available only if \ref SEQUENCER_TASK_DEBUG and SEQUENCER_TASK_TUNING are
                                        defined as 1.
                                        */
#endif /* SEQUENCER_TASK_TUNING */

#endif  /* SEQUENCER_TASK_DEBUG */
/*@}*/
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
/* Local Function */
static void Main_SequencerTask ( void );
static void AlmSequencerCallback ( void );
static void Main_SequencerCallback(void);


#if     SEQUENCER_TASK_DEBUG   &&   SEQUENCER_TASK_DURATION
static void Main_SequencerTask_Duration_Init ( void );
static void Main_SequencerTask_Duration_Start ( void );
static void Main_SequencerTask_Duration_End ( void );
#else
#define Main_SequencerTask_Duration_Init()
#define Main_SequencerTask_Duration_Start()
#define Main_SequencerTask_Duration_End()
#endif /* SEQUENCER_TASK_DEBUG   &&   SEQUENCER_TASK_DURATION */

#if     SEQUENCER_TASK_DEBUG   &&   SEQUENCER_TASK_TUNING
static void Main_SequencerTask_Tuning_Init ( void );
static OS_I32 Main_SequencerTask_DelayTuner ( OS_U32 usOffset );
static OS_I32 Main_SequencerTask_PeriodTuner ( OS_U32 usOffset );
#else
#define Main_SequencerTask_Tuning_Init()
#define Main_SequencerTask_DelayTuner(dummy)     ((OS_I32)SEQUENCER_DELAY_ACTIVATION_TUNING_VAL)
#define Main_SequencerTask_PeriodTuner(dummy)    ((OS_I32)SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL)
#endif /* SEQUENCER_TASK_DEBUG   &&   SEQUENCER_TASK_TUNING */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Sequencer service initialization.
It creates task \ref Main_SequencerTask and \ref Main_AlmSequencer alarm.
If debug option OUTPUTS_TASK_DEBUG and
\li \ref OUTPUTS_TASK_TUNING or
\li \ref OUTPUTS_TASK_DURATION
\n are enabled it initializes tools for timing measurement.
*/
#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)
void Main_SequencerInit ( void )
{
    /* first Activity points to NULL*/
    
    Main_ActivityList_Init(&(Main_Sequencer.SeqActivityList));
    
    /* create the task */
	OS_CREATETASK ( &Main_SequencerTaskTCB,
					"SequencerTask",
					Main_SequencerTask,
					MAIN_SEQUENCER_TASK_PRIORITY,
					Main_SequencerTaskStack );
#if defined(CONFIG_MAIN_SEQUENCER_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_SequencerTaskTCB, &Main_SequencerStackMonitor);
#endif
    /* create the alarm */
	OS_CreateAlarm ( &Main_AlmSequencer, AlmSequencerCallback, ( OS_U32 ) 0, ( OS_U32 ) 0 );
    
    /* initialize debug utilities */
	Main_SequencerTask_Duration_Init();
	Main_SequencerTask_Tuning_Init();
    
    
}
#endif
/**
Sets the alarm on high resolution timer to resume Sequencer Task at the beginning of each sub-interval of the halfwave
It must be called on ZC event.
At each expiration of the alarm the task is resumed.

\param [in] usOffset first alarm expiration in microseconds
\param [in] usPeriod alarm period in microseconds
\param [in] count number of expiration in microseconds (See alarm documentation)

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)
void Main_PeriodicScheduleSequencer ( OS_U32 usOffset, OS_U32 usPeriod, OS_U32 TickRef )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
	usOffset += Main_SequencerTask_DelayTuner ( usOffset );
	usPeriod += Main_SequencerTask_PeriodTuner ( usPeriod );
	if(OS_IsTask(&Main_SequencerTaskTCB))
	{
	    OS_START_ABSALARM ( &Main_AlmSequencer, usOffset, usPeriod, MAIN_SEQUENCER_LENGTH, &ref );
	}
	Main_Sequencer.SequencerID = 1;
}
#endif

#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)
void Main_AddSequencerActivity ( Main_SequencerActivityType* pSeqActivity, Main_ActivityCallbackType*  pSeqCallback )
{
	Main_ActivityType * pActivity;
    
    /* Set the hook routine */
    pSeqActivity->pSeqCallback=pSeqCallback;
    pSeqActivity->profile=0;
    
    /* Process the standard Activity */
    pActivity= &(pSeqActivity->activity);
    Main_CreateActivity ( &(Main_Sequencer.SeqActivityList), pActivity, Main_SequencerCallback , NULL);
}
#endif




int Main_GetSequencerLength(void)
{
    return (int) (MAIN_SEQUENCER_LENGTH);
}


Main_SequencerProfileType Main_GetSequencerMiddleEvent(void)
{
    return (Main_SequencerProfileType) (1<<(MAIN_SEQUENCER_LENGTH/2));
}

/* Local Function */

static void Main_SequencerCallback(void)
{
    Main_SequencerActivityType * p = (Main_SequencerActivityType *) (Main_Sequencer.SeqActivityList.pCurrentActivity);
    if(p->pSeqCallback)
    {
      if ( p->profile & Main_Sequencer.SequencerID )
      {
        (p->pSeqCallback)();
      }
    }
}
/**
*/
static void Main_SequencerManager(void)
{
    Main_ActivityProcess(&(Main_Sequencer.SeqActivityList));
    
    Main_Sequencer.SequencerID <<= 1U;
}

/**
Sequencer Alarm callback that resumes Outputs task
*/
static void AlmSequencerCallback ( void )
{
	if ( OS_GetSuspendCnt ( &Main_SequencerTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_SequencerTaskTCB );
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
static void Main_SequencerTask ( void )
{
	START_SEQUENCER_TASK_MONITORING();
	while ( 1 )
	{
		OS_Suspend ( NULL );

		TRACE_CPU_IDLE_END();

		MAIN_SEQUENCER_TASK_TRACE_BEGIN()
		OS_EnterRegion();
		/* here the scheduler is switched off */
		/* debug: begin */
		Main_SequencerTask_Duration_Start();
		/* debug: end */
		MAIN_SEQUENCER_TASK__MONITOR_IN();
		
        Main_SequencerManager();

		MAIN_SEQUENCER_TASK__MONITOR_OUT();
		/* debug: begin */
		Main_SequencerTask_Duration_End();
		/* debug: end */
		OS_LeaveRegion();
        MAIN_SEQUENCER_TASK_TRACE_END()
		/* here the scheduler is switched on */
	}
}
/* Local Function */

#if SEQUENCER_TASK_DEBUG && SEQUENCER_TASK_DURATION
/**
Init Duration  measurement
*/
static void Main_SequencerTask_Duration_Init ( void )
{
	Tools_CreateTimingMeasure ( &SequencerDuration );
	Tools_CreateTimingStats ( &SequencerTaskDurationStat );
}
/**
Starts a duration measurement
*/
static void Main_SequencerTask_Duration_Start ( void )
{
	Tools_TimingSetRef ( &SequencerDuration );
}

/**
Stops the duration measurement
*/
static void Main_SequencerTask_Duration_End ( void )
{
	Tools_ProcessValue ( &SequencerTaskDurationStat, Tools_GetTiming ( &SequencerDuration ) );
}
#endif /* SEQUENCER_TASK_DEBUG && SEQUENCER_TASK_DURATION */


#if     SEQUENCER_TASK_DEBUG && SEQUENCER_TASK_TUNING
/**
Tuning initialization.
Resets OutputsTuning_Delay and OutputsTuning_Period at zero.
*/
static void Main_SequencerTask_Tuning_Init ( void )
{
	SequencerTuning_Delay = 0;
	SequencerTuning_Period = 0;
}

/**
Filters OutputsTuning_Delay.
\param [in] usOffset current delay
\return OS_I32
\retval OutputsTuning_Delay.
\attention range of OutputsTuning_Delay:
from -usOffset to max of OS_I32
*/
static OS_I32 Main_SequencerTask_DelayTuner ( OS_U32 usOffset )
{
	if ( SequencerTuning_Delay < 0 )
	{
		if ( -SequencerTuning_Delay > (int)usOffset )
		{
			SequencerTuning_Delay = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return SequencerTuning_Delay;
}

/**
Filters OutputsTuning_Period.
\param [in] usOffset current period
\return OS_I32
\retval OutputsTuning_Delay.
\attention range of OutputsTuning_Delay:
from -usOffset to max of OS_I32
*/
static OS_I32 Main_SequencerTask_PeriodTuner ( OS_U32 usOffset )
{
	if ( SequencerTuning_Period < 0 )
	{
		if ( abs(SequencerTuning_Period) > usOffset )
		{
			SequencerTuning_Period = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return SequencerTuning_Period;
}
#endif /* SEQUENCER_TASK_DEBUG && SEQUENCER_TASK_TUNING */
#endif /* defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) */
/* *********** */
/* End of file */
/* *********** */
