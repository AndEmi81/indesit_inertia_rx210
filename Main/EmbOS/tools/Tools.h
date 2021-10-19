/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			Implementation of all
\n
\file		Tools.h
\ingroup	MainDebug
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
#ifndef _MAIN_TOOLS_H
#define _MAIN_TOOLS_H
/* ******** */
/* Includes */
/* ******** */
#include "autoconf.h"
#include "RTOS.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum {
	TOOLS_STACK_TYPE_UNKNOWN=0,
	TOOLS_STACK_TYPE_INT,
	TOOLS_STACK_TYPE_SYS,
	TOOLS_STACK_TYPE_TASK
}TOOLS_STACKTYPE_ID;

typedef struct
{
	unsigned int size;						/* size in bytes */
	unsigned int used;						/* used portion in bytes */
#if defined(CONFIG_TOOLS_STACKUSAGE_BASE)
	void OS_STACKPTR* base;					/* address */
#endif
} TOOLS_STACKINFO;

typedef struct TOOLS_STACKUSAGE_MONITOR_STRUCT TOOLS_STACKUSAGE_MONITOR;
struct TOOLS_STACKUSAGE_MONITOR_STRUCT
{
	TOOLS_STACKINFO 			info;
	TOOLS_STACKTYPE_ID			id;
	OS_TASK* 					pTask;	/*pointer to TCB */
	TOOLS_STACKUSAGE_MONITOR* 	pNext;	/*next monitor*/
};








/* Stack global */
typedef struct
{
	void OS_STACKPTR* 			err_base;		/* cleaned only at cold reset */
	TOOLS_STACKUSAGE_MONITOR* 	pMonitor;		/* list */
	TOOLS_STACKUSAGE_MONITOR* 	pCurrMonitor;	/* Current monitor*/
	OS_U8						count;

} TOOLS_STACKUSAGE_GLOBAL;


/* Period Measure */

typedef struct
{
	OS_TIMING  In;
	OS_TIMING  Overhead;
	OS_TIMING  Period_cyc;
	char       state;
} TOOLS_PERIOD;


/* Timing measure */
typedef struct
{
	OS_TIMING  StartRef;
	OS_TIMING  Overhead;
	OS_TIMING  Timing_cyc;
	char       state;
} TOOLS_TIMING;

typedef struct
{
	OS_TIMING  Current;
	OS_TIMING  Min;
	OS_TIMING  Max;
} TOOLS_TIMING_STATS;



/* */
typedef struct
{
	OS_TIME		timeRef;
	OS_TIME		period;
	OS_BOOL		enable;
} TOOLS_TIMETRIGGER;


typedef OS_TIME		 TOOLS_TIMER;
/* ********* */
/* Variables */
/* ********* */

/* Global */


/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
#if 	defined(CONFIG_TOOLS_STACKUSAGE)
void Tools_StackUsageInit(void);
void Tools_CreateTaskStackMonitor ( OS_TASK* pTask, TOOLS_STACKUSAGE_MONITOR* pMonitor );
void Tools_ProcessOSErrorStack(int _OS_Error);
#else
#define Tools_StackUsageInit()
#define Tools_CreateTaskStackMonitor ( x, y)
#define Tools_ProcessOSErrorStack(x)
#endif


void Tools_CreatePeriodMeasure ( TOOLS_PERIOD* pPeriodMeasure );
void Tools_PeriodMeasure ( TOOLS_PERIOD* pPeriodMeasure );
OS_TIMING Tools_GetPeriodCycles ( TOOLS_PERIOD* pPeriodMeasure );


void Tools_CreateTimingMeasure ( TOOLS_TIMING* pMeasure );
void Tools_TimingSetRef ( TOOLS_TIMING* pMeasure );
void Tools_TimingSetRefBySysTime ( TOOLS_TIMING* pMeasure, OS_U32 sysTime);
OS_TIMING Tools_GetTiming ( TOOLS_TIMING* pMeasure );

void Tools_CreateTimingStats ( TOOLS_TIMING_STATS* pStats );
void Tools_ProcessValue ( TOOLS_TIMING_STATS* pStats, OS_TIMING cycles );


void Tools_CreateTimeTrigger ( TOOLS_TIMETRIGGER* pTimeTrigger, OS_TIME period, OS_BOOL start );
void Tools_SetTimeTriggerStatus ( TOOLS_TIMETRIGGER* pTimeTrigger, OS_BOOL start );
OS_BOOL Tools_IsTriggered( TOOLS_TIMETRIGGER* pTimeTrigger);
OS_BOOL Tools_IsTimeTriggerActive( TOOLS_TIMETRIGGER* pTimeTrigger);

void Tools_StartTimer ( TOOLS_TIMER* pTimeTrigger );
OS_BOOL Tools_IsTimerElapsed ( TOOLS_TIMER* pTimer, OS_TIME time );
void Tools_SetTimeout ( TOOLS_TIMER* pTimer, OS_TIME timeout );
void Tools_DisableTimeout ( TOOLS_TIMER* pTimer);
OS_BOOL	Tools_IsTimeoutDisabled( TOOLS_TIMER* pTimer);
OS_BOOL Tools_IsTimeoutExpired ( TOOLS_TIMER* pTimer);
/* Local Function */




/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Local Function */
#endif /* _MAIN_TOOLS_H */
/* *********** */
/* End of file */
/* *********** */

