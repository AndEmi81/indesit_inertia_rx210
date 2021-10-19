/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**

\defgroup	MainSyncOutputs  Synhronized Loads
@{
\par		Desciptions
\n			This module implements the service for loads synchronized with ZC:
\li         ZCLoads
\li         Profiler


\par        Properties
The module can be configured in mainconfig.h file by defining the following properties:
\li         \ref MAIN_SEQUENCER_TASK__MONITOR_IN(), \ref MAIN_SEQUENCER_TASK__MONITOR_OUT(), not mandatory
\li         \ref MAIN_SEQUENCER_TASK__BODY(), mandatory
\li         \ref SEQUENCER_DELAY_ACTIVATION_TUNING_VAL, \ref SEQUENCER_PERIOD_ACTIVATION_TUNING_VAL
\li         \ref MAIN_SEQUENCER_TASK_STACK_SIZE
\li         \ref MAIN_SEQUENCER_COUNT

\par        APIs
\li         Main_ActivateOutputs()

\par        Debug support
            This module offer several debug features (\ref OUTPUTS_TASK_DEBUG):
\li         task duration measurement based on high resolution timer (\ref SEQUENCER_TASK_DURATION).
\li         fine-tuning task activation: the initial delay and the period of the task can be dinamically tuned
in order to otpimize the final application behavior (\ref SEQUENCER_TASK_TUNING).
@}
\ingroup   	Main
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
#include <string.h>
#include "Main_ActivityList.h"

#if defined(CONFIG_MAIN_SEQUENCER_TASK_TRACE_PIN_ENABLE)
#include "LIB_Trace.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */











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
Sequencer service initialization.
It creates task \ref Main_SequencerTask and \ref Main_AlmSequencer alarm.
If debug option OUTPUTS_TASK_DEBUG and
\li \ref OUTPUTS_TASK_TUNING or
\li \ref OUTPUTS_TASK_DURATION
\n are enabled it initializes tools for timing measurement.
*/

void Main_ActivityList_Init ( Main_ActivityList_Type* pActivityCB )
{
    pActivityCB -> pActivityList = NULL;
    pActivityCB -> pCurrentActivity = NULL;
#if defined(CONFIG_MAIN_ACTIVITY_LIST_EXECUTE_COUNT)
    pActivityCB -> count=0;
#endif
}



void Main_CreateActivity ( Main_ActivityList_Type* pActivityCB, 
                          Main_ActivityType* pActivity, 
                          Main_ActivityCallbackType*  pCallback,
                          Main_ActivityCallbackType*  pInit)
{
    Main_ActivityType ** pp;

	for ( pp = &(pActivityCB->pActivityList); ( *pp ) != NULL; pp = & ( ( *pp )->pNext ) )
	{
		if ( *pp == pActivity )
		{
            /* Activity already present in the list: don't add it!!*/
			return;
		}
	}
    
    /* force new Activity to point to NULL */
    pActivity->pCallback = pCallback;
    pActivity->pNext=NULL;
#if defined(CONFIG_MAIN_ACTIVITY_EXECUTE_COUNT)
    pActivity->count=0;
#endif    
    /* force last Activity to point to the new */
    *pp = pActivity;
    
    /* execute init */
    if(pInit)
    {
      pInit();
    }
}


void Main_ActivityProcess(Main_ActivityList_Type* pActivityCB)
{
    Main_ActivityType ** pp;
    
    if(!pActivityCB)
    {
        return;
    }
    
#if defined(CONFIG_MAIN_ACTIVITY_LIST_EXECUTE_COUNT)
    pActivityCB -> count ++;
#endif
    
    for ( pp = &(pActivityCB->pActivityList); ( *pp ) != NULL; pp = & ( ( *pp )->pNext ) )
	{
        if((*pp)->pCallback)
        {
#if defined(CONFIG_MAIN_ACTIVITY_EXECUTE_COUNT)
            (*pp) -> count=0;
#endif
            pActivityCB->pCurrentActivity=(*pp);
            ((*pp) -> pCallback)();

        }
	}
}


/* Local Function */



/* *********** */
/* End of file */
/* *********** */
