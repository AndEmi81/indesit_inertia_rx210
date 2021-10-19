/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_TaskMonitoring.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_TaskMonitoring.c
\ingroup	LIB_TaskMonitor
\date		Mar 28, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Mar 28, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/
#define LIB_TASKMONITORING_C
#define MODULE_BODY_CLASSBCHECK
/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "LIB_Error.h"
#include "MainConfig.h"
#include "LIB_TaskMonitoring.h"
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */

/** define on checkActTaskError_t variables: task is not activated within the timeout */
#define TASK_NOT_ACTIVATED          0
/** define on checkActTaskError_t variables: control on task suspended */
#define TASK_SUSPENDED              1
/** define on checkActTaskError_t variables: task activated but timeout not expired */
#define TASK_TIMEOUT_NOT_ELAPSED    2

#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF)
#define SET_TASK_TIMEOUT(taskID, value)
#define TASK_TIMEOUT(taskID) CheckTasksMaxTimeout[taskID]
#else
#define SET_TASK_TIMEOUT(taskID, value) CheckTasks[taskID].maxTimeout = (uint8_t)value
#define TASK_TIMEOUT(taskID) CheckTasks[taskID].maxTimeout
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** typedef returns values of CheckActTask function */
typedef uint8_t checkActTaskError_t;

/** structure for the control variables of the tasks subject to monitoring */
struct checkTasks_s
{
	/** timeout counter */
	uint8_t	timeoutCounter;
#if defined (CONFIG_PL_DEBUG)
	uint8_t maxCounter;
#endif
#if !defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF)
	uint8_t	maxTimeout;
#endif /* #if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF) */
	/** flag if '1' the task has been configured */
	uint8_t configDone:1;
	/** flag if '1' the task is active  */
	uint8_t taskActive:1;
	/** flag if '1' task executed before timeout elapsed  */
	uint8_t taskEntered:1;
	/** flag if '1' task executed before timeout elapsed  */
	/* uint8_t	taskExecuted:1; */
	/** flag if '1' task Configured  */
	uint8_t taskConfigured:1;
};

typedef struct checkTasks_s checkTasks_t;

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void LIB_TaskMonitorErrorHandler( uint8_t err );
static checkActTaskError_t CheckActTask( uint8_t taskID );

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF)
extern const uint8_t CheckTasksMaxTimeout[NUM_TASKS_TO_MONITOR];
#endif

/** modul's variables */
static checkTasks_t CheckTasks[ NUM_TASKS_TO_MONITOR ];
static int LIB_TaskMonitorError;

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK)

typedef struct irqMonitorStatus_s
{
	uint16_t EnterCntr;
	uint16_t ExitCntr;
	uint16_t fltCntr;
	uint16_t maxIntrCntr;
} irqMonitorStatus_t;

irqMonitorStatus_t IRQMonitorStatus;

#endif

/* DEBUG */
#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK_DEBUG)
uint16_t LIB_IRQMonitorMin;
uint16_t LIB_IRQMonitorZero;
uint16_t LIB_IRQMonitorMismatch;
OS_TIME LIB_IRQMonitorTimestamp;
#endif

/* ***************** */
/* Exported function */
/* ***************** */

void LIB_TaskMonitorInit( void )
{
	SAFETY_REGION_START();
	memset(CheckTasks, 0, sizeof(CheckTasks));
	LIB_TaskMonitorError = 0;

	SAFETY_DATA_UPDATE(CheckTasks);
	SAFETY_DATA_UPDATE(LIB_TaskMonitorError);
	SAFETY_REGION_END();

}

/**
\n Monitorings the activation of selected tasks. This function must be called periodically
at Supervisor Task period time - timeouts are set depending on this value.

\return 	bool_t

\retval		TRUE all tasks submitted to the monitoring were activated within the prescribed time-out.
\retval		FALSE a task submitted to the monitoring was not activated within the prescribed time-out.


\author 	Stefano Ughi
\date		Mar 28, 2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
#ifdef CONFIG_PL_DEBUG 
uint8_t DBG_LIB_TM_idFailedTask=0;  
#endif /* CONFIG_PL_DEBUG*/
void LIB_TaskMonitor( void )
{
	bool_t retVal = TRUE;
	uint8_t taskToCheck;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(LIB_TaskMonitorError);
	SAFETY_DATA_CHECK(CheckTasks);

	if(LIB_TaskMonitorError != 0 )
	{
		retVal = FALSE;
	}
	else {

		for ( taskToCheck = 0; taskToCheck < NUM_TASKS_TO_MONITOR; taskToCheck ++ )
		{
			if ( CheckActTask( taskToCheck ) == TASK_NOT_ACTIVATED )
			{
	#ifdef CONFIG_PL_DEBUG
			  DBG_LIB_TM_idFailedTask = taskToCheck;
	#endif
			  retVal = FALSE;
			  break;
			}
		}
	}
	SAFETY_DATA_UPDATE(CheckTasks);
	SAFETY_REGION_END();

	if(retVal == FALSE){
		PLT_ERROR_FATAL(PLT_ERROR_CLASSBCHECK_TASKMONITORING);
	}

	/* return retVal; */
}

/**
The function is called by the Tasks subject to monitoring.

\param  	[in], taskID ID calling task


\author 	Stefano Ughi
\date		25/06/2007
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

void LIB_HelloTaskMonitor( uint8_t taskID )
{
  LIB_TaskStartActivity(taskID);
  LIB_TaskEndActivity(taskID);
}

/**
Sets the check configuration  for the specific taskID.

\param  	[in] taskID task's ID (defined in enum  tm_id_enum)
\param  	[in] timeout (in ms)
\param  	[in] state  - TRUE/FALSE set task monitoring active/not active
*/
void LIB_TaskMonitoringConfig( uint8_t taskID, uint16_t timeout, bool_t state )
{
  if( taskID < NUM_TASKS_TO_MONITOR )
  {
#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF)
	 if(CheckTasks[taskID].taskConfigured)
		  return;
#endif /* defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK_STATIC_CONF) */

      SAFETY_REGION_START();
      SAFETY_DATA_CHECK(CheckTasks);
      SET_TASK_TIMEOUT(taskID, timeout);
      CheckTasks[taskID].timeoutCounter = 0;
      CheckTasks[taskID].configDone = 1;
    
      CheckTasks[taskID].taskActive = ( TRUE == state );

      CheckTasks[taskID].taskConfigured = 1;

      SAFETY_DATA_UPDATE(CheckTasks);
      SAFETY_REGION_END();
  }
  else{
#ifdef CONFIG_PL_DEBUG 
	  DBG_LIB_TM_idFailedTask = taskID;
#endif
	  LIB_TaskMonitorErrorHandler(3);
  }
}

void LIB_TaskStartActivity(  uint8_t taskID )
{

    /* Check if task is configured and activated */
  if ((CheckTasks[taskID].configDone) && (CheckTasks[taskID].taskActive)
		  && (!CheckTasks[taskID].taskEntered))
  {
      SAFETY_REGION_START();
      SAFETY_DATA_CHECK(CheckTasks);

      CheckTasks[taskID].taskEntered = 1;

      SAFETY_DATA_UPDATE(CheckTasks);
      SAFETY_REGION_END();
  }
  else {
	/* If called not active, raise an error condition */
#ifdef CONFIG_PL_DEBUG
	DBG_LIB_TM_idFailedTask = taskID;
#endif	
        LIB_TaskMonitorErrorHandler(1);
  }
}

void LIB_TaskEndActivity(  uint8_t taskID )
{

    /* Check if task is configured and activated */
	if ((CheckTasks[taskID].configDone) && (CheckTasks[taskID].taskEntered))
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(CheckTasks);
		CheckTasks[taskID].taskEntered = 0;

#if defined (CONFIG_PL_DEBUG)
		if (CheckTasks[taskID].timeoutCounter > CheckTasks[taskID].maxCounter) {
				CheckTasks[taskID].maxCounter = CheckTasks[taskID].timeoutCounter;
		}
#endif

	/* Task activated before timeout */
	/* Reset timeout counter - everything was going fine! */
      CheckTasks[taskID].timeoutCounter = 0;

      SAFETY_DATA_UPDATE(CheckTasks);
      SAFETY_REGION_END();
    }
	else
	{
        /* If called not active or before LIB_TaskStartActivity, raise an error condition */
#ifdef CONFIG_PL_DEBUG 		
	  DBG_LIB_TM_idFailedTask = taskID;
#endif
          LIB_TaskMonitorErrorHandler(2);
	}

}


/**
Sets the state of Task with ID taskID.

\param  	[in] taskID task's ID
\param  	[in] state if TRUE Task is active otherwise suspended


\author 	Stefano Ughi
\date		Mar 28,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
void LIB_SetTask( uint8_t taskID, bool_t state )
{

  if (CheckTasks[taskID].configDone)
  {
	  SAFETY_REGION_START();
	  SAFETY_DATA_CHECK(CheckTasks);

      CheckTasks[taskID].taskActive = (state == TRUE);

      SAFETY_DATA_UPDATE(CheckTasks);
      SAFETY_REGION_END();
  }

}

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK)

/**
Function called to mark entering an Interrupt routine.
This function checks if EnterCnt go higher than threshold value set
(CONFIG_LIB_CLASS_B_IRQ_CHECK_MAXVALUE): in case of interrupt overload it
may become impossible to check it in periodic monitor.

\param  	none

\return 	none

\author 	Alessandro Sabbatini
\date		17/07/2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_IRQMonitor_Enter(void)
{
	SAFETY_DATA_CHECK(IRQMonitorStatus);

#if !defined(CONFIG_LIB_CLASS_B_IRQ_CHECK_DEBUG)

	/* Check if EnterCnt is lower than minimum threshold - some of expected interrupts was broken */
	PLT_ERROR_FATAL_IF(IRQMonitorStatus.EnterCntr > CONFIG_LIB_CLASS_B_IRQ_CHECK_MAXVALUE, PLT_ERROR_IRQMON_TOO_MANY)

#endif

	IRQMonitorStatus.EnterCntr++;
	SAFETY_DATA_UPDATE(IRQMonitorStatus);
}

/**
Function called to mark leaving an Interrupt routine

\param  	none

\return 	none

\author 	Alessandro Sabbatini
\date		17/07/2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
void LIB_IRQMonitor_Exit(void)
{
	SAFETY_DATA_CHECK(IRQMonitorStatus);
	IRQMonitorStatus.ExitCntr++;
	SAFETY_DATA_UPDATE(IRQMonitorStatus);

}

/**
IRQ Monitoring initialization routine - it should be called on Supervisor initialization.
This function initializes Enter and Exit counter to minimum value acceptable
(CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE) because first time monitor is called
no interrupts could be raised so monitor may fail.

\param  	none

\return 	none

\author 	Alessandro Sabbatini
\date		17/07/2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_IRQMonitor_Init(void)
{
	SAFETY_REGION_START();
	IRQMonitorStatus.EnterCntr = CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE;
	IRQMonitorStatus.ExitCntr = CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE;
	IRQMonitorStatus.fltCntr = 0;
	IRQMonitorStatus.maxIntrCntr = 0;
	SAFETY_DATA_UPDATE(IRQMonitorStatus);
	SAFETY_REGION_END();
}

/**
IRQ Monitoring function - it should be called periodically on Supervisor
This function checks if EnterCnt and ExitCnt are equal - all interrupt
entered an left properly - and EnterCnt is higher (or at least equal)
than minimum threshold set (CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE).

After that, EnterCnt and ExitCnt will be reset to restar monitor in next window
time.

\param  	none

\return 	none

\author 	Alessandro Sabbatini
\date		17/07/2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/

void LIB_IRQMonitor(void)
{
	EnterCriticalSection();
	SAFETY_DATA_CHECK(IRQMonitorStatus);

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK_DEBUG)

	if (IRQMonitorStatus.EnterCntr != IRQMonitorStatus.ExitCntr)
	{
		LIB_IRQMonitorMismatch++;
		LIB_IRQMonitorTimestamp = OS_GetTime();
	}

	if (IRQMonitorStatus.EnterCntr == 0)
	{
		LIB_IRQMonitorZero++;
		LIB_IRQMonitorTimestamp = OS_GetTime();
	}
	else if ((IRQMonitorStatus.EnterCntr < LIB_IRQMonitorMin) || (LIB_IRQMonitorMin == 0))
	{
		LIB_IRQMonitorMin = IRQMonitorStatus.EnterCntr;
	}

#else

	/* Check if EnterCnt equals ExitCnt - all interrupts entered and exited properly, no flow mistakes occurred */
	PLT_ERROR_FATAL_IF(IRQMonitorStatus.EnterCntr != IRQMonitorStatus.ExitCntr, PLT_ERROR_IRQMON_EXEC_FAIL);

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE) && (CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE > 0)
	/* Check if EnterCnt is lower than minimum threshold - if so, some of expected interrupts were broken,
	 * increment filter counter to go toward failure state                                                   */
	if (IRQMonitorStatus.EnterCntr < CONFIG_LIB_CLASS_B_IRQ_CHECK_MINVALUE)
	{
		IRQMonitorStatus.fltCntr += CONFIG_LIB_CLASS_B_IRQ_FLTR_INCR;
	}
	else if (IRQMonitorStatus.fltCntr > 0)
	{
		IRQMonitorStatus.fltCntr--;
	}
	else
	{
		/* Nothing else to do */
	}

	/* If maximum value raised...  give PLT Error! */
	PLT_ERROR_FATAL_IF(IRQMonitorStatus.fltCntr > CONFIG_LIB_CLASS_B_IRQ_FLTR_MAXTH, PLT_ERROR_IRQMON_TOO_FEW);
#endif

#endif

	if (IRQMonitorStatus.EnterCntr > IRQMonitorStatus.maxIntrCntr)
	{
		IRQMonitorStatus.maxIntrCntr = IRQMonitorStatus.EnterCntr;
	}

	/* Reset counters to check a new monitor window */
	IRQMonitorStatus.EnterCntr = 0;
	IRQMonitorStatus.ExitCntr = 0;

	SAFETY_DATA_UPDATE(IRQMonitorStatus);
	ExitCriticalSection();

}

#endif

/* ************** */
/* Local Function */
/* ************** */

/**
Task monitor error handler

\param  	[in], err	Error identificator raised

\return 	none

\author 	Stefano Ughi
\date		Mar 28,2011
\version	1.0.2

\note       \par \b Nota 1:
			documentation by Stefano Ughi

*/
static void LIB_TaskMonitorErrorHandler( uint8_t err )
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(LIB_TaskMonitorError);
	LIB_TaskMonitorError = err;
	SAFETY_DATA_UPDATE(LIB_TaskMonitorError);
	SAFETY_REGION_END();

}

/**
Checks task's activation.

\param  	[in], taskID ID del task

\return 	checkActTaskError_t

\retval		TASK_SUSPENDED              task is suspended - no check is done
\retval		TASK_TIMEOUT_NOT_ELAPSED    task may be running but timeout isn't expired
\retval		TASK_NOT_ACTIVATED          task wasn't performed within timeout

\author 	Stefano Ughi
\date		Mar 28,2011
\version	1.0.2

\note       \par \b Nota 1:
			documentation by Stefano Ughi

*/

static checkActTaskError_t CheckActTask( uint8_t taskID )
{
	checkActTaskError_t retValue = TASK_SUSPENDED;

	if (CheckTasks[taskID].configDone)
	{

		if ( CheckTasks[taskID].taskActive ) /* activity started*/
		{
			if ( CheckTasks[taskID].timeoutCounter < TASK_TIMEOUT(taskID) )
			{
				/* Task not done but timeout not expired */
				CheckTasks[taskID].timeoutCounter ++;
				retValue = TASK_TIMEOUT_NOT_ELAPSED;
			}
			else {
				retValue = TASK_NOT_ACTIVATED;
			}
		}
		else
		{
			/* No action required */
		}

	}
	else
	{
		/* No action required */
	}

	/* If waiting for timeout, skip reset flags */
	if (retValue != TASK_TIMEOUT_NOT_ELAPSED) {
		CheckTasks[taskID].taskEntered = 0;
	}

	return retValue;
}


/* *********** */
/* End of file */
/* *********** */
