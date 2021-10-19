/*
Copyright (c) 2004-2012, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/*
\defgroup	Tests Tests
@{
\par		Description

\par		Compilers
			Currently supported compilers:
			\li GNU (__GNUC__)
\n			Framework tests

\par        Properties
\n TODO

\n Trace to pin
\n  not supported

\par        Callbacks
Callbacks are
\li         Test_SetUp()
\li			Test_Step()
\li			Test_SetStepPeriod()
\par        Debug support
            TODO
@}
\ingroup   	Tests
*/


/* ******** */
/* Includes */
/* ******** */

/* configuration */
#include "Main.h"
#include "callback.h"
#include "syncSvc.h"
#include "LIB_Trace.h"

//#include "dbg_uart.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

#define _TEST_DEFAULT_

#if defined(__GNUC__)
#undef _TEST_DEFAULT_
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef  enum { INIT_TEST, WAIT_INPUT, END_TEST } Main_Test_StepSample_t;


/* ***************************** */
/* Prototypes of local functions */
/* ***************************** */


/**
@name embOS tasks
@{
*/
static void Main_TestSupervisorTask ( void );
/*@}*/

/* ********* */
/* Variables */
/* ********* */

/* Global */
/*!
Synchronization callback list, for the synchronization service
*/
/*struct SubscriberNode *syncSvcList=0;*/


/* Static */
static Main_Test_StepSample_t Main_Test_Step;

/**
@name embOS variables
@{
*/
/** Supervisor Task Stack */
static STACK_SEC OS_STACKPTR int  	Main_TestSupervisorTaskStack[200] STACK_SEC_POST;
/** Init Task Task control block*/
static OS_TASK          			Main_TestSupervisorTCB;
/*@}*/

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
#ifdef CONFIG_PL_DEBUG
#if defined(__GNUC__)       /* GNU */
__attribute__((weak))		void Test_SetUp( int param );
__attribute__((weak))		void Test_Step( void );
__attribute__((weak))		int Test_SetStepPeriod(void);
#endif

extern void Test_MainLoop( int par );
#endif // CONFIG_PL_DEBUG



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Main EmbOS framework entry point.
Its aim is to start OS creating Main_TestSupervisorTask.

Actions:
-reset embOS global variables
-embOS kernel initialization
-embOS hw initialization
-init task creation
-start OS

\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0

\note		\par \b Note 1:
documentation by Roberto Fioravanti

*/
int main ( void )
{
	OS_Clean();                     /* Extension to clean all OS objects */
    OS_IncDI();                      /* Initially disable interrupts  */
    OS_InitKern();                   /* initialize OS                 */
    OS_InitHW();                     /* initialize Hardware for OS    */

#ifdef STARTUP_FOR_TEST
	OCCLK_SetInternalClockSync(CLK_INT_CLOCK_FREQ);
#endif


    /* You need to create at least one task before calling OS_Start() */
    /*OS_SendString("Creating Init Task...");*/
    OS_CREATETASK ( &Main_TestSupervisorTCB,
                    "TestSupervisor",
                    Main_TestSupervisorTask,
                    ( OS_U8 ) 128,
                    Main_TestSupervisorTaskStack );

    OS_Start();                      /* Start multitasking            */
    return 0;
}


/* Local Functions */
/**
overridable, called once at startup
\param	[in]	param , always 0

\return none

\author 	Roberto Fioravanti

\note: documentation by Roberto Fioravanti
\date		18/05/2012
\version	1.0.0
*/
#if defined(__GNUC__)   /* GNU */
__attribute__((weak)) void Test_SetUp(int param)
#elif defined(_TEST_DEFAULT_)
void Test_SetUp(int param)
#endif
{
	TestUart_Enable();
	TestUart_PrintStr("\r\n");
	TestUart_PrintLine("TEST Sample **************************************");


	Main_Test_Step = INIT_TEST;
}


/**
overridable, called periodically

\return int
\retval	period in ms. Zero is invalid value;

\author 	Roberto Fioravanti

\note: documentation by Roberto Fioravanti
\date		18/05/2012
\version	1.0.0
*/
#if defined(__GNUC__)       /* GNU */
__attribute__((weak)) int Test_SetStepPeriod(void)
#elif defined(_TEST_DEFAULT_)
int Test_SetStepPeriod(void)
#endif
{
	return 0;
}


/**
overridable, called periodically
\return none

\author 	Roberto Fioravanti

\note: documentation by Roberto Fioravanti
\date		18/05/2012
\version	1.0.0
*/
#if defined(__GNUC__)       /* GNU */
__attribute__((weak)) void Test_Step(void)
#elif defined(_TEST_DEFAULT_)
void Test_Step(void)
#endif
{
	switch( Main_Test_Step )
	{
		case INIT_TEST:
			Main_Test_Step = WAIT_INPUT;
		break;
		case WAIT_INPUT:
			Main_Test_Step = END_TEST;
		break;
		case END_TEST:
			TestUart_PrintLine("END TEST **************************************");
			Main_Test_Step = END_TEST+1;
		break;
		default:
			TestUart_PrintLine("UNMANAGED step");
	}
}

/**
Task for test framework.
It calls Test_SetUp() and then, Test_Step() periodically.
Period of the call to Test_Step() is fixed at 50 ms.

\author 	Roberto Fioravanti
\date		  18/05/2012
\version	1.0.0

\note		\par \b Note 1:
documentation by Roberto Fioravanti
*/

static void Main_TestSupervisorTask ( void )
{
	int period = 10;
	int t0 = OS_GetTime();
#ifdef CONFIG_PL_DEBUG
	Test_SetUp( 0 );
	period = Test_SetStepPeriod();
	if(! period)
	{
		period = 50;
	}
#endif // CONFIG_PL_DEBUG

    while ( 1 )
    {
        Test_Step();
    	t0 += period;
    	OS_DelayUntil (t0);
    }
}

/* *********** */
/* End of file */
/* *********** */
