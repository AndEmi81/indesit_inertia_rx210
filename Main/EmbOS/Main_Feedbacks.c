/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			This file contains the EmbOS-based service for the acquisition of feedbacks.
\n			For both types of feedbacks the module makes use of a task with parameter.
			Each type of feedback runs inside its own instance of the same task.
\file		Main_Feedbacks.c
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
#include "FeedBacks.h"
#include "LIB_Trace.h"

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#include "Lib_ClassBVar.h"
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */


/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_MAIN_FEEDBACKS_TASK_TRACE_PIN_ENABLE) && defined(CONFIG_LIB_TRACE_SUPPORT)
#define MAIN_FEEDBACKS_TASK_TRACE_BEGIN()    TRACE_TO_PIN(ON , (CONFIG_MAIN_FEEDBACKS_TASK_TRACE_PIN) )
#define MAIN_FEEDBACKS_TASK_TRACE_END()      TRACE_TO_PIN(OFF , (CONFIG_MAIN_FEEDBACKS_TASK_TRACE_PIN) )
#else
#define MAIN_FEEDBACKS_TASK_TRACE_BEGIN()	{}
#define MAIN_FEEDBACKS_TASK_TRACE_END()		{}
#endif

#if !defined(CONFIG_MAIN_FEEDBACKS_TASK_TASKMONITOR_ENABLE)
/** @name Feedbacks task safety

\par Properties
Not mandatory. If they remain undefined assume by default an empty value.

@{
*/
#define START_FEEDBACKS_TASK_MONITORING(delayed)	{}
/** safety entry */
#define MAIN_FEEDBACKS_TASK__MONITOR_IN(delayed)	{}
/** safety exit */
#define MAIN_FEEDBACKS_TASK__MONITOR_OUT(delayed)	{}
/*@}*/
#endif


#if !defined(START_FEEDBACKS_TASK_MONITORING)
#error "START_FEEDBACKS_TASK_MONITORING() missing definition."
#endif

#if !defined(MAIN_FEEDBACKS_TASK__MONITOR_IN)
#error "MAIN_FEEDBACKS_TASK__MONITOR_IN() missing definition."
#error "Both MAIN_FEEDBACKS_TASK__MONITOR_IN() and MAIN_FEEDBACKS_TASK__MONITOR_OUT() must be defined."
#endif

#if !defined(MAIN_FEEDBACKS_TASK__MONITOR_OUT)
#error "MAIN_FEEDBACKS_TASK__MONITOR_OUT() missing definition."
#error "Both MAIN_FEEDBACKS_TASK__MONITOR_IN() and MAIN_FEEDBACKS_TASK__MONITOR_OUT() must be defined."
#endif



#if !defined(MAIN_FEEDBACKS_TASK_DEBUG)
/**
@name Debug macros
\par Properties
Not mandatory. If not defined they assume the default values
\code
#define MAIN_FEEDBACKS__TASK_DEBUG                  (1)
#define MAIN_FEEDBACKS_TASK_DURATION                (1)
#define MAIN_FEEDBACKS_TASK_DELAY_ON_ZC             (1)
\endcode
@{
*/
/** Debug enable
\li 0: all debug features disabled
\li 1: allows to enable the debug level for MAIN_FEEDBACKS_TASK_DURATION, MAIN_FEEDBACKS_TASK_DELAY_ON_ZC
*/
#define MAIN_FEEDBACKS_TASK_DEBUG                (1)
#endif

#if !defined(MAIN_FEEDBACKS_TASK_DURATION)
/** Measurement of the duration of the task
\li 0: disabled
\li 1: enabled
*/
#define MAIN_FEEDBACKS_TASK_DURATION             (1)
#endif


/*@}*/





/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Feedback modes enum
*/
typedef enum MainFeedbacksMode
{

#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
	/**
	Normal feedbacks mode.
	*/
    MAIN_FEEDBACK_MODE_NORMAL = 0,
#endif
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
    /**
	Delayed feedbacks mode.
	*/
	MAIN_FEEDBACK_MODE_DELAYED
#endif
} MainFeedbacksModeType;

/*
Parameter for delay on ZC
It is the delay on ZC in system cycles (OS_NANOSECS_TO_SYSTEMTIMERCYCLES())
*/
typedef OS_U32 MainFeedbacksDelayOnZCParamType;

/* enum to access Delay on ZC param structure */
typedef enum
{
	/**
	Normal feedbacks mode.
	*/
    MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MIN = 0,
	/**
	Delayed feedbacks mode.
	*/
    MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MAX
} MainFeedbacksDelayOnZCParamIndexType;

/* score (kind of correlation index) to state if delay on ZC measurement is in range and reliable */
typedef struct Score_s
{
	OS_I8	score;
	OS_I8	eScore;
} MainFeedbacks_ScoreType;
/* ********* */
/* Variables */
/* ********* */

/* Global */
/* Static */
/**
@name Service variables
@{
*/
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
/** Task Stack for normal feedbacks */
static STACK_SEC OS_STACKPTR int  Main_FeedbacksTaskStack[CONFIG_MAIN_FEEDBACKS_TASK_STACK_SIZE] STACK_SEC_POST;
/** Task Control Block for normal feedbacks*/
static OS_TASK          Main_FeedbacksTaskTCB;
/** high resolution Alarm for normal feedbacks*/
static OS_ALARM         Main_AlmFeedbacks;
#if defined(CONFIG_MAIN_FEEDBACKS_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_FeedbackTaskStackMonitor;
#endif
#endif /* defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) */


#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
/** Task Stack for delayed feedbacks */
static STACK_SEC OS_STACKPTR int  Main_FeedbacksPTaskStack[CONFIG_MAIN_FEEDBACKS_TASK_STACK_SIZE] STACK_SEC_POST;
/** Task Control Block for delayed feedbacks*/
static OS_TASK          Main_FeedbacksPTaskTCB;
/** high resolution Alarm for delayed feedbacks*/
static OS_ALARM         Main_AlmFeedbacksP;
#if defined(CONFIG_MAIN_FEEDBACKSP_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_FeedbackPTaskStackMonitor;
#endif
#endif /*defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE) */

/*@}*/



#if MAIN_FEEDBACK_BOTH==1

#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
	#define MAIN_FEEDBACK_MODE_TYPE     	0     /*MAIN_FEEDBACK_MODE_NORMAL*/
	#define Main_Feedbacks_TASK_TCB     	Main_FeedbacksTaskTCB
	#define Main_Feedbacks_TASK_STACK   	Main_FeedbacksTaskStack
	#define Main_Feedbacks_StackMonitor   	Main_FeedbackTaskStackMonitor

#elif defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
	#define MAIN_FEEDBACK_MODE_TYPE     	1     /*MAIN_FEEDBACK_MODE_DELAYED*/
	#define Main_Feedbacks_TASK_TCB     	Main_FeedbacksPTaskTCB
	#define Main_Feedbacks_TASK_STACK   	Main_FeedbacksPTaskStack
	#define Main_Feedbacks_StackMonitor   	Main_FeedbackPTaskStackMonitor
#endif

#endif



/**
@name  variables

@{
*/

#if defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC)
/* feedbacks timing for the measurement of the delay to ZC  */
static  TOOLS_TIMING        		MainFeedbacksDelayOnZC[MAIN_FEEDBACK_BOTH];
/* Number of updates for normal feedbacks :
 * free runner counter incremented from -128 up to 127 each task execution
 * */
static	MainFeedbacks_ScoreType		MainFeedbacksDelayOnZC_Score[MAIN_FEEDBACK_BOTH]; /*score*/
/* Max and min delay in ZC in ns*/
static const MainFeedbacksDelayOnZCParamType MainFeedbacksDelayOnZCParam[2]=
{
	OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC_MIN_VALUE*1000),/*min*/
	OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC_MAX_VALUE*1000)	/*max*/
};


#endif  /* MAIN_FEEDBACKS_TASK_DELAY_ON_ZC */


#if     MAIN_FEEDBACKS_TASK_DEBUG
#if     MAIN_FEEDBACKS_TASK_DURATION
/** task duration in [us]
\attention Available only if \ref MAIN_FEEDBACKS_TASK_DEBUG and MAIN_FEEDBACKS_TASK_DURATION are
defined as 1.
\note not used yet
*/
static  TOOLS_TIMING        		MainFeedbacksDuration;
/** statistics about task duration [us]
\attention Available only if \ref MAIN_FEEDBACKS_TASK_DEBUG and MAIN_FEEDBACKS_TASK_DURATION are
defined as 1.
\note not used yet
*/
static  TOOLS_TIMING_STATS  		MainFeedbacksTaskDurationStat;
#endif  /* MAIN_FEEDBACKS_TASK_DURATION */

#endif  /* MAIN_FEEDBACKS_TASK_DEBUG */
/*@}*/

#if		defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK)
/* feedbacks timing for task period measurement */
static  TOOLS_PERIOD        		MainFeedbacksPeriod[MAIN_FEEDBACK_BOTH];
/* Number of updates for feedbacks :
 * free runner counter incremented from -128 up to 127 each task execution
 * */
static	MainFeedbacks_ScoreType		MainFeedbacksPeriod_Score[MAIN_FEEDBACK_BOTH]; /*score*/

/* Max and min delay oin ZC in ns*/
static const MainFeedbacksDelayOnZCParamType MainFeedbacksPeriodParam[2]=
{
	OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_MIN_VALUE*1000),/*min*/
	OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_MAX_VALUE*1000)	/*max*/
};
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */



/* Local Function */
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
static void Main_ActivateFeedbacks ( void );
#endif /* defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) */
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
static void Main_ActivateFeedbacksP ( void );
#endif /* defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE) */


#if MAIN_FEEDBACK_BOTH  == 2
static void Main_FeedbacksTask ( void* pFeedbackType );
#elif MAIN_FEEDBACK_BOTH  == 1
static void Main_FeedbacksTask ( void );
#endif
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
static void AlmFeedbacksCallback ( void );
#endif /* (CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) */

#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
static void AlmFeedbacksPCallback ( void );
#endif /*(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE) */


#ifdef		CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC
static void Main_FeedbacksTask_DelayOnZC_Init ( void );
static void Main_FeedbacksTask_DelayOnZC_Process 	( MainFeedbacksModeType FeedbackType );
static OS_I8 Main_FeedbacksMode_DelayOnZC_IsRangeOK(MainFeedbacksModeType mode);
#else
#define Main_FeedbacksTask_DelayOnZC_Init()
#define Main_FeedbacksTask_DelayOnZC_Process(dummy)
#define Main_FeedbacksMode_DelayOnZC_IsRangeOK(dummy)
#endif /* CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC */

#if     MAIN_FEEDBACKS_TASK_DEBUG   &&   MAIN_FEEDBACKS_TASK_DURATION
static void Main_FeedbacksTask_Duration_Init ( void );
static void Main_FeedbacksTask_Duration_Start ( void );
static void Main_FeedbacksTask_Duration_End ( void );
#else
#define Main_FeedbacksTask_Duration_Init()
#define Main_FeedbacksTask_Duration_Start()
#define Main_FeedbacksTask_Duration_End()
#endif /* MAIN_FEEDBACKS_TASK_DEBUG   &&   MAIN_FEEDBACKS_TASK_DURATION */

#if		defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK)
static void Main_FeedbacksTask_Period_Init ( void );
static void Main_FeedbacksTask_Period_Measure ( MainFeedbacksModeType FeedbackType );
static OS_I8 Main_FeedbacksMode_Period_IsRangeOK(MainFeedbacksModeType mode);
#else
#define Main_FeedbacksTask_Period_Init()
#define Main_FeedbacksTask_Period_Measure(dummy)
#define Main_FeedbacksMode_Period_IsRangeOK(dummy)
#endif /* CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK */




/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Feedbacks service initialization.
It creates:
\li two instances of the same task ref Main_FeedbacksTask (\ref MAIN_FEEDBACK_MODE_NORMAL
and \ref MAIN_FEEDBACK_MODE_DELAYED)
\li and the two alarms \ref Main_AlmFeedbacksP and \ref Main_AlmFeedbacks.
\n If CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK is enabled, it
initializes tools for timing measurement.
*/
void Main_FeedbacksInit ( void )
{
#if MAIN_FEEDBACK_BOTH  == 2
  
	OS_CREATETASK_EX ( &Main_FeedbacksTaskTCB,
						"FeedbackTask",
						Main_FeedbacksTask,
						( OS_U8 ) MAIN_FEEDBACKS_TASK_PRIORITY,
						Main_FeedbacksTaskStack,
						( void* ) MAIN_FEEDBACK_MODE_NORMAL );
	OS_CREATETASK_EX ( &Main_FeedbacksPTaskTCB,
						"FeedbackPTask",
						Main_FeedbacksTask,
						( OS_U8 ) MAIN_FEEDBACKSP_TASK_PRIORITY,
						Main_FeedbacksPTaskStack,
						( void* ) MAIN_FEEDBACK_MODE_DELAYED );
#if defined(CONFIG_MAIN_FEEDBACKS_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_FeedbacksTaskTCB, &Main_FeedbackStackMonitor);
#endif
#if defined(CONFIG_MAIN_FEEDBACKSP_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_FeedbacksPTaskTCB, &Main_FeedbackPStackMonitor);
#endif
    
#elif MAIN_FEEDBACK_BOTH  == 1 /* MAIN_FEEDBACK_BOTH */
    
    OS_CREATETASK    ( &Main_Feedbacks_TASK_TCB,
					   "FeedbackTask",
					   Main_FeedbacksTask,
					   ( OS_U8 ) MAIN_FEEDBACKS_TASK_PRIORITY,
					   Main_Feedbacks_TASK_STACK );
#if defined(CONFIG_MAIN_FEEDBACKS_STACK_MONITOR) || defined(CONFIG_MAIN_FEEDBACKP_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&Main_Feedbacks_TASK_TCB, &Main_Feedbacks_StackMonitor);
#endif
    
#endif /* MAIN_FEEDBACK_BOTH */
    
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
	OS_CreateAlarm ( &Main_AlmFeedbacks, AlmFeedbacksCallback, ( OS_U32 ) 0, ( OS_U32 ) 0 );
#endif /* if CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE    */
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
	OS_CreateAlarm ( &Main_AlmFeedbacksP, AlmFeedbacksPCallback, ( OS_U32 ) 0, ( OS_U32 ) 0 );
#endif /* CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE  */
	Main_FeedbacksTask_DelayOnZC_Init();
    Main_FeedbacksTask_Duration_Init();
    Main_FeedbacksTask_Period_Init();
}

/**
Sets the alarm on high resolution timer for continuous readings of normal feedbacks.
Use it only if zerocrossing is missing.

It could be used for continuous asynchronous readings of normal feedbacks, that is when
zerocrossing signal is missing.

\param [in] usOffset alarm expiration in microseconds

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
void Main_ActivateFeedbacks_AsyncRead ( OS_U32 usOffset )
{
    if(OS_IsTask(&Main_FeedbacksTaskTCB))
    {        
        OS_STARTALARMSF ( &Main_AlmFeedbacks, usOffset, usOffset, -1 );
    }
}
#endif /* CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE */

/**
Sets the alarm on high resolution timer for continuous readings of delayed feedbacks.
Use it only if zerocrossing is missing.

It could be used for continuous asynchronous readings of delayed feedbacks, that is when
zerocrossing signal is missing.

\param [in] usOffset alarm expiration in microseconds

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
void Main_ActivateFeedbacksP_AsyncRead ( OS_U32 usOffset )
{
    if(OS_IsTask(&Main_FeedbacksPTaskTCB))
    {
        OS_STARTALARMSF ( &Main_AlmFeedbacksP, usOffset, usOffset, -1 );
    }
}
#endif /* CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE */
/**
Sets the alarm on high resolution timer for synchronous one-shot readings of normal feedbacks.

It could be used for synchronized readings of normal feedbacks, whenever zerocrossing signal is available.

\param [in] usOffset alarm expiration in microseconds

\attention For sync readings it must be called in sync with ZC.

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
void Main_ScheduleFeedbacks ( OS_U32 usOffset, OS_U32 TickRef, OS_U32 period )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
    if(OS_IsTask(&Main_FeedbacksTaskTCB))
    {
        OS_START_ABSALARM ( &Main_AlmFeedbacks, usOffset, period, -1, &ref );
    }
}
#endif /* CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE */

/**
Sets the Alarm on high resolution timer for synchronous one-shot readings of delayed feedbacks.

It could be used for synchronized readings of normal feedbacks, whenever zerocrossing signal is available.

\param [in] usOffset alarm expiration in microseconds
\attention For sync readings it must bel called in sync with ZC.

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
void Main_ScheduleFeedbacksP ( OS_U32 usOffset, OS_U32 TickRef, OS_U32 period )
{
	OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
	if(OS_IsTask(&Main_FeedbacksPTaskTCB))
	{
		OS_START_ABSALARM ( &Main_AlmFeedbacksP, usOffset, period,  -1, &ref );
	}
}
#endif /* CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE */



/*
 *
*/
#if defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC)
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)

/*
Checks if range is ok.
\return
\retvalue	0 , no update since last time
\retvalue	>0 , in range and reliable (value has been at least once updated since last call
\retvalue	<0 , not in range
*/
OS_I8 Main_Feedbacks_DelayOnZC_IsOK(void)
{
	return Main_FeedbacksMode_DelayOnZC_IsRangeOK(MAIN_FEEDBACK_MODE_NORMAL);
}
#endif /* defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)*/
/*
Checks if range is ok.
\return
\retvalue	0 , no update since last time
\retvalue	>0 , in range and reliable (value has been at least once updated since last call
\retvalue	<0 , not in range
*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
OS_I8 Main_FeedbacksP_DelayOnZC_IsOK(void)
{
	return Main_FeedbacksMode_DelayOnZC_IsRangeOK(MAIN_FEEDBACK_MODE_DELAYED);
}

#endif /*defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)*/
#endif /*defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC)*/



#if defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK)
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
/*
Checks if range is ok.
\return
\retvalue	0 , no update since last time
\retvalue	>0 , in range and reliable (value has been at least once updated since last call
\retvalue	<0 , not in range
*/
OS_I8 Main_Feedbacks_Period_IsOK(void)
{
	return Main_FeedbacksMode_Period_IsRangeOK(MAIN_FEEDBACK_MODE_NORMAL);
}
#endif /* defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
/*
Checks if range is ok.
\return
\retvalue	0 , no update since last time
\retvalue	>0 , in range and reliable (value has been at least once updated since last call
\retvalue	<0 , not in range
*/
OS_I8 Main_FeedbacksP_Period_IsOK(void)
{
	return Main_FeedbacksMode_Period_IsRangeOK(MAIN_FEEDBACK_MODE_DELAYED);
}

#endif /*defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)*/
#endif /*defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC)*/

/* Local Functions */


/**
Preemptable task for Feedback
When its activity has been completed the task suspends itself, waiting for the next alarm expiration
which will resume the task.
\warning	none

\author 	Roberto Fioravanti
\date		25/02/2011
\version	1.0.0

\par Properties (safety)
\li MAIN_FEEDBACKS_TASK__MONITOR_IN()
\li MAIN_FEEDBACKS_TASK__MONITOR_OUT()

\par Properties (system)
\li MAIN_FEEDBACKS_TASK_STACK_SIZE

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
#if MAIN_FEEDBACK_BOTH == 2
static void Main_FeedbacksTask ( void* pFeedbackType )
{
	START_FEEDBACKS_TASK_MONITORING(pFeedbackType);
	while ( 1 )
	{
		OS_Suspend ( (OS_TASK*)NULL );
		MAIN_FEEDBACKS_TASK_TRACE_BEGIN();
		Main_FeedbacksTask_Period_Measure(( MainFeedbacksModeType ) pFeedbackType);
		Main_FeedbacksTask_DelayOnZC_Process ( (MainFeedbacksModeType) pFeedbackType );
        Main_FeedbacksTask_Duration_Start();
		MAIN_FEEDBACKS_TASK__MONITOR_IN(pFeedbackType);
		LIB_FDB_Manager ( ( bool_t ) ((int)pFeedbackType), goodZCSignal() );
		MAIN_FEEDBACKS_TASK__MONITOR_OUT(pFeedbackType);
        Main_FeedbacksTask_Duration_End();
        MAIN_FEEDBACKS_TASK_TRACE_END();
	}
}
#elif MAIN_FEEDBACK_BOTH  == 1 /* MAIN_FEEDBACK_BOTH */
static void Main_FeedbacksTask ( void )
{
	START_FEEDBACKS_TASK_MONITORING(0); /* mode is not applicable */
	while ( 1 )
	{
		OS_Suspend ( NULL );

		TRACE_CPU_IDLE_END();

		/* debug: begin */
		MAIN_FEEDBACKS_TASK_TRACE_BEGIN();
		Main_FeedbacksTask_Period_Measure(( MainFeedbacksModeType ) 0 /*MAIN_FEEDBACK_MODE_TYPE*/);
		Main_FeedbacksTask_DelayOnZC_Process ( ( MainFeedbacksModeType ) 0 );
		Main_FeedbacksTask_Duration_Start();
		MAIN_FEEDBACKS_TASK__MONITOR_IN(0);	/* mode is not applicable */
		LIB_FDB_Manager ( ( bool_t ) MAIN_FEEDBACK_MODE_TYPE, goodZCSignal() );
		MAIN_FEEDBACKS_TASK__MONITOR_OUT(0);	/* mode is not applicable */
        Main_FeedbacksTask_Duration_End();
		MAIN_FEEDBACKS_TASK_TRACE_END();
	}
}
#endif /* MAIN_FEEDBACK_BOTH */

#if		defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
/**
debug only
\attention available if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
*/
static void Main_FeedbacksTask_Period_Init ( void )
{
	SAFETY_REGION_START();
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
	Tools_CreatePeriodMeasure ( &MainFeedbacksPeriod[MAIN_FEEDBACK_MODE_NORMAL] );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod, MAIN_FEEDBACK_MODE_NORMAL );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod_Score, MAIN_FEEDBACK_MODE_NORMAL);
#endif
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
	Tools_CreatePeriodMeasure ( &MainFeedbacksPeriod[MAIN_FEEDBACK_MODE_DELAYED] );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod, MAIN_FEEDBACK_MODE_DELAYED );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod_Score, MAIN_FEEDBACK_MODE_DELAYED);
#endif
	SAFETY_REGION_END();
}

/**
debug only
\attention available if MEASURE_PERIOD_TASK_PERIODIC_DEBUG
*/
static void Main_FeedbacksTask_Period_Measure ( MainFeedbacksModeType FeedbackType )
{
	OS_TIMING timing=0;

	SAFETY_REGION_START();

	/* get period */
	SAFETY_DATA_ELEM_CHECK(MainFeedbacksPeriod, FeedbackType );
	Tools_PeriodMeasure ( &MainFeedbacksPeriod[FeedbackType] );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod, FeedbackType );

	/* store the period in a local variable */
	timing = MainFeedbacksPeriod[FeedbackType].Period_cyc;

	/* check*/
	if(( timing >= MainFeedbacksPeriodParam[MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MIN])
		&& (timing <= MainFeedbacksPeriodParam[MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MAX]))
	{
		/* the value of the delay is in the allowed range */
		SAFETY_DATA_ELEM_CHECK(MainFeedbacksPeriod_Score, FeedbackType);
		MainFeedbacksPeriod_Score[FeedbackType].score++;
		SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod_Score, FeedbackType);
	}
	else
	{
		SAFETY_DATA_ELEM_CHECK(MainFeedbacksPeriod_Score, FeedbackType);
		if(MainFeedbacksPeriod_Score[FeedbackType].score!=MainFeedbacksPeriod_Score[FeedbackType].eScore)
		{
			MainFeedbacksPeriod_Score[FeedbackType].score--;
			SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod_Score, FeedbackType);
		}
	}

	SAFETY_REGION_END();
}
#endif

#if MAIN_FEEDBACKS_TASK_DEBUG && MAIN_FEEDBACKS_TASK_DURATION
static void Main_FeedbacksTask_Duration_Init ( void )
{
	Tools_CreateTimingMeasure ( &MainFeedbacksDuration );
	Tools_CreateTimingStats ( &MainFeedbacksTaskDurationStat );
}

static void Main_FeedbacksTask_Duration_Start ( void )
{
	Tools_TimingSetRef ( &MainFeedbacksDuration );
}

static void Main_FeedbacksTask_Duration_End ( void )
{
	Tools_ProcessValue ( &MainFeedbacksTaskDurationStat, Tools_GetTiming ( &MainFeedbacksDuration ) );
}
#endif /* MAIN_ANALOG_INPUTS_TASK_DEBUG && MAIN_ANALOG_INPUTS_TASK_DURATION */



#if defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
/**
Init Duration  measurement
*/
static void Main_FeedbacksTask_DelayOnZC_Init ( void )
{
	SAFETY_REGION_START();
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
	Tools_CreateTimingMeasure ( &MainFeedbacksDelayOnZC[MAIN_FEEDBACK_MODE_NORMAL] );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC, MAIN_FEEDBACK_MODE_NORMAL );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC_Score, MAIN_FEEDBACK_MODE_NORMAL );
#endif
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
	Tools_CreateTimingMeasure ( &MainFeedbacksDelayOnZC[MAIN_FEEDBACK_MODE_DELAYED] );
	SAFETY_DATA_ELEM_UPDATE( MainFeedbacksDelayOnZC, MAIN_FEEDBACK_MODE_DELAYED );
	SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC_Score, MAIN_FEEDBACK_MODE_DELAYED );
#endif
	SAFETY_REGION_END();
}


/**
Stops the delay measurement
\param [in] FeedbackType
*/
static void Main_FeedbacksTask_DelayOnZC_Process ( MainFeedbacksModeType FeedbackType )
{
	OS_TIMING timing=0;

	SAFETY_REGION_START();
	if(goodZCSignal())
	{
		/* get the delay */
		SAFETY_DATA_ELEM_CHECK( MainFeedbacksDelayOnZC,  FeedbackType);
		Tools_TimingSetRefBySysTime ( &MainFeedbacksDelayOnZC[FeedbackType], Get_ZCSysTime() );
		Tools_GetTiming ( &MainFeedbacksDelayOnZC[FeedbackType] );
		SAFETY_DATA_ELEM_UPDATE( MainFeedbacksDelayOnZC, FeedbackType );


		/* store the delay in a local variable */
		timing = MainFeedbacksDelayOnZC[FeedbackType].Timing_cyc;

		/* check if the delay is in range */
		if(( timing >= MainFeedbacksDelayOnZCParam[MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MIN])
			&& (timing <= MainFeedbacksDelayOnZCParam[MAIN_FEEDBACK_DELAY_ON_ZC_PARAM_MAX]))
		{
			/* the value of the delay is in the allowed range */
			SAFETY_DATA_ELEM_CHECK(MainFeedbacksDelayOnZC_Score, FeedbackType);
			MainFeedbacksDelayOnZC_Score[FeedbackType].score++;
			SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC_Score, FeedbackType);
		}
		else
		{

			SAFETY_DATA_ELEM_CHECK(MainFeedbacksDelayOnZC_Score, FeedbackType);
			if(MainFeedbacksDelayOnZC_Score[FeedbackType].score!=MainFeedbacksDelayOnZC_Score[FeedbackType].eScore)
			{
				MainFeedbacksDelayOnZC_Score[FeedbackType].score--;
				SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC_Score, FeedbackType);
			}

		}
	}
	else
	{
		/* cannot apply delay on ZC*/
		/* do nothing*/

	}
	SAFETY_REGION_END();
}
#endif  /* defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC) */





/**
Alarm callback for normal feedbacks alarm that will be called at alarm expiration
*/
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
static void AlmFeedbacksCallback ( void )
{
	Main_ActivateFeedbacks();
}
#endif /* CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE */

/**
Alarm callback for delayed feedbacks alarm that will be called at alarm expiration
*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
static void AlmFeedbacksPCallback ( void )
{
	Main_ActivateFeedbacksP();
}
#endif /* CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE */


/**
Resumes normal feedbacks task
*/
#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
static void Main_ActivateFeedbacks ( void )
{
	if ( OS_GetSuspendCnt ( &Main_FeedbacksTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_FeedbacksTaskTCB );
	}
}
#endif  /* CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE */

/**
Resumes normal feedbacks task
*/
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
static void Main_ActivateFeedbacksP ( void )
{
	if ( OS_GetSuspendCnt ( &Main_FeedbacksPTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_FeedbacksPTaskTCB );
	}
}
#endif  /* CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE */

#ifdef		CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC
static OS_I8 Main_FeedbacksMode_DelayOnZC_IsRangeOK(MainFeedbacksModeType mode)
{
	OS_I8 result=MainFeedbacksDelayOnZC_Score[mode].score-MainFeedbacksDelayOnZC_Score[mode].eScore;
	if (result > 0)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK(MainFeedbacksDelayOnZC_Score, mode);
		MainFeedbacksDelayOnZC_Score[mode].eScore=MainFeedbacksDelayOnZC_Score[mode].score;
		SAFETY_DATA_ELEM_UPDATE(MainFeedbacksDelayOnZC_Score, mode);
		SAFETY_REGION_END();
		return 1;
	}else if (result == 0)
	{
		return 0;
	}
	result = -1;
	return result;
}



static OS_I8 Main_FeedbacksMode_Period_IsRangeOK(MainFeedbacksModeType mode)
{
	OS_I8 result=MainFeedbacksPeriod_Score[mode].score-MainFeedbacksPeriod_Score[mode].eScore;
	if (result > 0)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK(MainFeedbacksPeriod_Score, mode);
		MainFeedbacksPeriod_Score[mode].eScore=MainFeedbacksPeriod_Score[mode].score;
		SAFETY_DATA_ELEM_UPDATE(MainFeedbacksPeriod_Score, mode);
		SAFETY_REGION_END();
		return 1;
	}else if (result == 0)
	{
		return 0;
	}
	result = -1;
	return result;
}

#endif /*		CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC*/
/* *********** */
/* End of file */
/* *********** */
