/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_TaskMonitoring
@{
\n
\n          Monitors Tasks's activation, in TaskMonitor_Cfg.h you must define
\n          numbers of tasks to monitor, task ID and task timeout in terms of
\n          multiple of periodic task.
\n
\par    Configuration:
\n
\n\li   \b NUM_TASKS_TO_MONITOR
\n      numbers of tasks or functions to monitor
\n\li   \b <>_TASK_ID
\n      task or function id
\n\li   \b TOUT_<>_TASK
\n      task's activation timeout expressed in GP periods (typically 10 msec)
\n
\par    Exported Functions:
\n
\n\li   PM_TM_TaskMonitor()
\n\li   PM_TM_HelloTaskMonitor()
\n\li   PM_TM_SetTask()
\n\li   CheckActTask()
\n
@}
\ingroup   	ProductM
*/
/**
\n
\par		PM_TaskMonitoring.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_TaskMonitoring.c
\ingroup	PM_TaskMonitoring
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

/* ******** */
/* Includes */
/* ******** */
#include "StandardDefines.h"
#include "PM_TaskMonitoring.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** define on checkActTaskError_t variables: task is not activated within the timeout */
#define TASK_NOT_ACTIVATED          0
/** define on checkActTaskError_t variables: task activated within the timeout */
#define TASK_ACTIVATED              1
/** define on checkActTaskError_t variables: control on task suspended */
#define TASK_SUSPENDED              2
/** define on checkActTaskError_t variables: task activated but timeout not expired */
#define TASK_TIMEOUT_NOT_ELAPSED    3

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** typedef returns values of CheckActTask function */
typedef uint8_t checkActTaskError_t;

/** structure for the control variables of the tasks subject to monitoring */
struct checkTasks_s
{
	/** timeout counter */
	uint8_t	toutTask:8;
	/** flag if '1' task executed before timeout elapsed  */
	uint8_t	taskExecuted:1;
	/** flag if '1' the task is active otherwise suspended */
	uint8_t taskActive:1;

};

/** word access or bit access on checkTasks_s's structures */
union checkTasks_u
{
	/** word access */
	uint16_t all;
	/** bit access */
	struct checkTasks_s bit;
};

typedef union checkTasks_u checkTasks_t;

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static checkActTaskError_t CheckActTask( uint8_t taskID );

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/** modul's variables */
static checkTasks_t CheckTasks[ NUM_TASKS_TO_MONITOR ];
/** timeout for each task submitted to the monitoring */
extern const uint8_t TaskTout[ NUM_TASKS_TO_MONITOR ];

/* ***************** */
/* Exported function */
/* ***************** */

/**
\n Monitorings the activation of selected tasks,

\return 	bool_t

\retval		TRUE all tasks submitted to the monitoring were activated within the prescribed time-out.
\retval		FALSE a task submitted to the monitoring was not activated within the prescribed time-out.


\author 	Stefano Ughi
\date		Mar 28, 2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

bool_t PM_TM_TaskMonitor( void )
{
	uint8_t taskSToCheck;

	for ( taskSToCheck = 0; taskSToCheck < NUM_TASKS_TO_MONITOR; taskSToCheck ++ )
	{
		if ( CheckActTask( taskSToCheck ) == TASK_NOT_ACTIVATED )
		{
			return FALSE;
		}
	}
	return TRUE;
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

void PM_TM_HelloTaskMonitor( uint8_t taskID )
{
	CheckTasks[taskID].bit.taskExecuted = 1;
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
void PM_TM_SetTask( uint8_t taskID, bool_t state )
{
	if ( TRUE == state )
	{
		CheckTasks[taskID].bit.taskActive = 1;
	}
	else
	{
		CheckTasks[taskID].bit.taskActive = 0;
	}
}

/* ************** */
/* Local Function */
/* ************** */

/**
Checks task's activation.

\param  	[in], taskID ID del task

\return 	checkActTaskError_t

\retval		TASK_ACTIVATED              the task was performed within the timeout
\retval		TASK_SUSPENDED              the task is suspended
\retval		TASK_TIMEOUT_NOT_ELAPSED    the task was not performed but the timeout isn't expired
\retval		TASK_NOT_ACTIVATED          the task wasn't performed within timeout

\author 	Stefano Ughi
\date		Mar 28,2011
\version	1.0.2

\note       \par \b Nota 1:
			documentation by Stefano Ughi

*/

static checkActTaskError_t CheckActTask( uint8_t taskID )
{
	checkActTaskError_t result;

	if ( CheckTasks[taskID].bit.taskActive )
	{
		if ( CheckTasks[taskID].bit.taskExecuted )
		{
			/* Task activated before timeout */
			CheckTasks[taskID].bit.toutTask = 0;
			CheckTasks[taskID].bit.taskExecuted = 0;
			result = TASK_ACTIVATED;
		}
		else
		{
			if ( CheckTasks[taskID].bit.toutTask < TaskTout[taskID] )
			{
				/* Task not done but timeout not expired */
				CheckTasks[taskID].bit.toutTask ++;
				result = TASK_TIMEOUT_NOT_ELAPSED;
			}
			else
			{
				/* The task not done within the timeout */
				result = TASK_NOT_ACTIVATED;
			}
		}
	}
	else
	{
		CheckTasks[taskID].bit.toutTask = 0;
		result = TASK_SUSPENDED;
	}
	return(result);
}


/* *********** */
/* End of file */
/* *********** */
