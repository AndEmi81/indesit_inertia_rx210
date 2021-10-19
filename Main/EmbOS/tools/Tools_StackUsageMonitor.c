/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    StackMonitor Stack monitoring
@{
\par            Description
\n                EmbOS-based tool that allows the monitor of the stacks usage (system, interrupt and task stack).
				This module is intended only for debug to optimize stack size.
				This module relies on stacks functionalities and APIs provided by embOS. Therefore available functionalities
				are subject to embOS implementation:
				-	due to version.
				-	due to specific implementation

\par			Features:
\n				-	configurable period
\n				-	Interrupt stack monitor
\n				-	System stack monitor
\n				-	Task stack monitor
\n				-	info on stack overflow

\par			General
 	 	 	 	General feature of the module
				Related properties:
				-	CONFIG_TOOLS_STACKUSAGE_PERIOD, period in ms of the acquisition (default 1000 ms)
				-	CONFIG_TOOLS_STACKUSAGE_BASE, option allowing the acquisition of the size of the whole stack
				APIs:
				-	Tools_StackUsageInit

\par			Interrupt stack monitor (available since 3.84)
				Allows monitoring the requirement for interrupt stack, if available in the specific implementation.
				Interrupt monitor can be enabled by defines.
				Related properties:

				APIs:
\par			System stack monitor (available since 3.84)
				Allows monitoring  of the requirement for system stack (available since 3.84)
				System stack monitor can be enabled by defines.
\par			Task stack monitor
				Allows monitoring  of the requirement for task stack.
				APIs:
				Tools_CreateTaskStackMonitor
				It needs the definition of a global variable (TOOLS_STACKUSAGE_MONITOR) associated to the task, that
				collects all the info of the relating stack.
\par			Overflow

				A global variable stores the OS-error related to stacks that embOS makes available trough OS_Error function
				APIs:
				Tools_ProcessOSErrorStack
\par 			Implementation
                The monitor runs on its own task whose period can be configured.

                Task stack monitor must be created manually.
@}
\ingroup        Tools
*/
/**
\n
\par		    Stack monitor
\n
\file		    Tools_StackUsageMonitor.c
\ingroup	    Tools
\date		    30/10/2012
\version	    00.00.00
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		        00.00.00
\n          	24/12/2010
\n          	Roberto Fioravanti
\n            - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */

#include "Tools.h"
#if 	defined(CONFIG_TOOLS_STACKUSAGE)

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if 	!defined(CONFIG_TOOLS_STACKUSAGE_PERIOD)
#define CONFIG_TOOLS_STACKUSAGE_PERIOD						1000
#endif

#if OS_SUPPORT_SYSSTACK_INFO &&(OS_VERSION_GENERIC>38400)
#define OS_SYSSTACK_API_SUPPORT								/* OS Api for Int stack documented since 3.84 */
#endif
#if OS_SUPPORT_INTSTACK_INFO &&(OS_VERSION_GENERIC>38400)
#define OS_INTSTACK_API_SUPPORT								/* OS Api for Int stack documented since 3.84 */
#endif

#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && !defined(OS_SYSSTACK_API_SUPPORT)
#error  "current version of embOS doesn't support System Stack APIs"
#endif
#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && !defined(OS_INTSTACK_API_SUPPORT)
#error  "current version of embOS doesn't support Interrupt Stack APIs"
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */


OS_STACKPTR int Tools_StackUsageMonitorStack[128];          /* Task stacks */
OS_TASK TCB_Tools_StackUsage;                        		/* Task-control-blocks */


/* Static */
/** Global module variable*/
#if defined(__GNUC__)
static TOOLS_STACKUSAGE_GLOBAL Tools_StackUsageGlobal __attribute__((section (".noinit")));
#elif defined (__CWCC__)     
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */

NO_INIT_VAR TOOLS_STACKUSAGE_GLOBAL Tools_StackUsageGlobal;
#else
#error "IDE or compiler not supported"
#endif

/* System stack monitor **/
#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && defined(OS_SYSSTACK_API_SUPPORT)
static TOOLS_STACKUSAGE_MONITOR	SysStack;
#endif
/* Interrupt stack monitor **/
#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && defined(OS_INTSTACK_API_SUPPORT)
static TOOLS_STACKUSAGE_MONITOR	IntStack;
#endif


/* ******************** */
/* Functions Prototypes */
/* ******************** */
static void OS_CleanStackUsageMonitors ( void );

#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && defined(OS_INTSTACK_API_SUPPORT)
static void Tools_CreateIntStackMonitor( void );
#else
#define Tools_CreateIntStackMonitor()
#endif

#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && defined(OS_SYSSTACK_API_SUPPORT)
static void Tools_CreateSysStackMonitor( void );
#else
#define Tools_CreateSysStackMonitor()
#endif

static void Tools_StackUsage_InsertMonitor(TOOLS_STACKUSAGE_MONITOR * pMonitor);
static void Tools_StackUsageManager(void);
static void Tools_TaskStackMonitor(TOOLS_STACKINFO * pInfo, OS_TASK* pTask);

#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && defined(OS_INTSTACK_API_SUPPORT)
static void Tools_IntStackMonitor(TOOLS_STACKINFO * pInfo);
#else
#define Tools_IntStackMonitor(x)
#endif

#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && defined(OS_SYSSTACK_API_SUPPORT)
static void Tools_SysStackMonitor(TOOLS_STACKINFO * pInfo);
#else
#define Tools_SysStackMonitor(x)
#endif
static void Tools_StackUsage(void);

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Starts the tool.

Creates the task that running the monitor
*/
void Tools_StackUsageInit(void)
{
	Tools_StackUsageGlobal.pCurrMonitor=NULL;
	Tools_StackUsageGlobal.count=0;

	OS_CleanStackUsageMonitors();
	Tools_CreateIntStackMonitor();
	Tools_CreateSysStackMonitor();
	OS_CREATETASK(&TCB_Tools_StackUsage, "StackUsage", Tools_StackUsage, 20, Tools_StackUsageMonitorStack);
}

/*
Creates a stack monitor for task
\param [in] pTask pointer to TCB
\param [in] pMonitor pointer to monitor
*/
void Tools_CreateTaskStackMonitor ( OS_TASK* pTask, TOOLS_STACKUSAGE_MONITOR* pMonitor )
{
	pMonitor->id=TOOLS_STACK_TYPE_TASK;
	pMonitor->pTask=pTask;
	Tools_StackUsage_InsertMonitor(pMonitor);
}

/**
Processes OS Stack Error
*/
void Tools_ProcessOSErrorStack(int _OS_Error)
{
	if(Tools_StackUsageGlobal.err_base == 0)
	{
		switch(_OS_Error)
		{
		case OS_ERR_SYS_STACK:		
#if defined(OS_SYSSTACK_API_SUPPORT)
			Tools_StackUsageGlobal.err_base = (void OS_STACKPTR*)OS_GetSysStackBase();
#else
#endif
			break;
		case OS_ERR_INT_STACK:
#if defined(OS_INTSTACK_API_SUPPORT)
			Tools_StackUsageGlobal.err_base = (void OS_STACKPTR*)OS_GetIntStackBase();
#else
#endif
			break;
		case OS_ERR_STACK:
		{
			Tools_StackUsageGlobal.err_base = (void OS_STACKPTR*)OS_GetStackBase(OS_GetpCurrentTask());
		}
			break;
		}
	}
}

/* Local Functions */
/**
Creates monitor for interrupt stack
*/
#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && defined(OS_INTSTACK_API_SUPPORT)
static void Tools_CreateIntStackMonitor( void )
{
	IntStack.id=TOOLS_STACK_TYPE_INT;
	Tools_StackUsage_InsertMonitor(&IntStack);
}
#endif
/**
Creates monitor for system stack
*/
#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && defined(OS_SYSSTACK_API_SUPPORT)
static void Tools_CreateSysStackMonitor( void )
{
	SysStack.id=TOOLS_STACK_TYPE_SYS;
	Tools_StackUsage_InsertMonitor(&SysStack);
}
#endif

/**

 */
static void Tools_StackUsage_InsertMonitor(TOOLS_STACKUSAGE_MONITOR * pStackMonitor)
{
	TOOLS_STACKUSAGE_MONITOR ** pp;
	OS_IncDI();

	for ( pp = &(Tools_StackUsageGlobal.pMonitor); ( *pp ) != NULL; pp = & ( ( *pp )->pNext ) )
	{
		if ( *pp == pStackMonitor )
		{
			return;
		}
	}

	pStackMonitor->pNext = NULL;
	*pp = pStackMonitor;
	Tools_StackUsageGlobal.count++;
	OS_DecRI();
}

/**
Interrupt Stack acquisition
*/
#if defined(CONFIG_TOOLS_INT_STACKUSAGE) && defined(OS_INTSTACK_API_SUPPORT)
static void Tools_IntStackMonitor(TOOLS_STACKINFO * pInfo)
{

#if defined(CONFIG_TOOLS_STACKUSAGE_BASE)
	pInfo->base= OS_GetIntStackBase();
#endif
	pInfo->size= OS_GetIntStackSize();
	pInfo->used= OS_GetIntStackUsed();
}
#endif

/**
System Stack acquisition
 */
#if defined(CONFIG_TOOLS_SYS_STACKUSAGE) && defined(OS_SYSSTACK_API_SUPPORT)
static void Tools_SysStackMonitor(TOOLS_STACKINFO * pInfo)
{

#if defined(CONFIG_TOOLS_STACKUSAGE_BASE)
	pInfo->base= (void OS_STACKPTR*)OS_GetSysStackBase();
#endif
	pInfo->size= OS_GetSysStackSize();
	pInfo->used= OS_GetSysStackUsed();
}
#endif

static void Tools_TaskStackMonitor(TOOLS_STACKINFO * pInfo, OS_TASK* pTask)
{
#if defined(CONFIG_TOOLS_STACKUSAGE_BASE)
	pInfo->base= (void OS_STACKPTR*)OS_GetStackBase(pTask);
#endif
	pInfo->size= OS_GetStackSize(pTask);
	pInfo->used= OS_GetStackUsed(pTask);
}

/**
Acquisition state machine
*/
static void Tools_StackUsageManager(void)
{
	TOOLS_STACKUSAGE_MONITOR * p = NULL;

	if(Tools_StackUsageGlobal.pMonitor == NULL)
	{
		/* empty list*/
		return;
	}

	if(Tools_StackUsageGlobal.pCurrMonitor == NULL)
	{
		Tools_StackUsageGlobal.pCurrMonitor = Tools_StackUsageGlobal.pMonitor;
	}

	p = Tools_StackUsageGlobal.pCurrMonitor;
	Tools_StackUsageGlobal.pCurrMonitor = p->pNext;

	switch(p->id)
	{
#if defined(CONFIG_TOOLS_INT_STACKUSAGE)
	case TOOLS_STACK_TYPE_INT:
		Tools_IntStackMonitor(&(p->info));
		break;
#endif
#if defined(CONFIG_TOOLS_SYS_STACKUSAGE)
	case TOOLS_STACK_TYPE_SYS:
		Tools_SysStackMonitor(&(p->info));
		break;
#endif
	case TOOLS_STACK_TYPE_TASK:
		Tools_TaskStackMonitor(&(p->info), p->pTask);
		break;
	default:
		;
	}
}
/**
Task
*/
static void Tools_StackUsage(void) {
  while (1) {
	  Tools_StackUsageManager();

	  OS_Delay (CONFIG_TOOLS_STACKUSAGE_PERIOD);
  }
}

/**
Cleans all previous created monitor
*/
static void OS_CleanStackUsageMonitors ( void )
{
	TOOLS_STACKUSAGE_MONITOR * p = Tools_StackUsageGlobal.pMonitor;
	TOOLS_STACKUSAGE_MONITOR * pTmp;

	while ( p )
	{
		/*if ( OS_IS_TASK ( p ) )*/
		{
			pTmp = p;
			p = ( TOOLS_STACKUSAGE_MONITOR * ) p->pNext;
			OS_MEMSET ( pTmp, 0, sizeof ( TOOLS_STACKUSAGE_MONITOR ) );
		}
	}

	Tools_StackUsageGlobal.pMonitor=NULL;
}
#endif 	/* !defined(CONFIG_TOOLS_STACKUSAGE) */
/* *********** */
/* End of file */
/* *********** */
