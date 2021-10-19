/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			This header file contains all exported objects
\n
\file		MainInterface.h
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

#ifndef __MAIN_INTERFACE_H
#define __MAIN_INTERFACE_H


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "Tools.h"


#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) || defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)
#include "Main_ActivityList.h"
#endif



/* ****************** */
/* Defines and Macros */
/* ****************** */
#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) || defined(CONFIG_MAIN_SEQUENCER_LENGTH_4)
#if defined(CONFIG_MAIN_SEQUENCER_LENGTH_2) 
#define MAIN_SEQUENCER_EVENT_FIRST              (1<<0)
#define MAIN_SEQUENCER_EVENT_SECOND             (1<<1)
#endif
#if defined(CONFIG_MAIN_SEQUENCER_LENGTH_4)
#define MAIN_SEQUENCER_EVENT_THIRD              (1<<2)
#define MAIN_SEQUENCER_EVENT_FOURTH             (1<<3)
#endif
#endif /* defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* Remote Loads Control */
typedef enum
{
	REMOTE_LOADS_CONTROL_BASIC = 1,         /**< Basic */
	REMOTE_LOADS_CONTROL_EXTENDED	        /**< Extended */
} RemoteLoadsControlType;

/**
Remote Loads control Errors
*/
typedef  enum
{
	REMOTELOADSCONTROL_NO_ERROR = 0,                          /**< no error */
	REMOTELOADSCONTROL_INVALID_DATA_ON_PUTCOMMAND,            /**< data cannot be stored in the queue.*/
	REMOTELOADSCONTROL_INVALID_DATA_POINTER_ON_GETCOMMAND,    /**< data cannot be retrieved from the queue, because pointer are invalid */
	REMOTELOADSCONTROL_FULLQUEUE_ON_PUTCOMMAND,               /**< queue is full or not enough space */
	REMOTELOADSCONTROL_UNKNOWN_OBJ,                            /**< uninitilized module */
	REMOTELOADSCONTROL_UNKNOWN_COMMAND                        /**< unknown command: command is not of type RemoteLoadsControlType */
} Main_RemoteLoadsControlErrorType;

/* Sequencer */

#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)

/**
Sequencer profile it's a bitmap that describes the events on which the callback must be activated
*/
typedef int8_t Main_SequencerProfileType;
typedef void Main_SequencerCallbackType (void);
    
typedef struct Main_SequencerActivityStruct Main_SequencerActivityType;
struct Main_SequencerActivityStruct{
    Main_ActivityType               activity;
    Main_SequencerProfileType       profile;
    Main_ActivityCallbackType*      pSeqCallback;
};
#endif /* defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) */
/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/*Supervisor*/
#if defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT)
Main_ActivityList_Type* Main_SupervisorGetActivityInstance(int id);
#endif /* defined(CONFIG_MAIN_SERVICE_ACTIVITY_SUPPORT) */
void Main_ZcSyncMonitor ( void );
/* Remote Loads control */
Main_RemoteLoadsControlErrorType Main_RemoteLoadsControl_PutCommand ( RemoteLoadsControlType type, uint8_t len, void* pData );
/* Live */
void Main_ScheduleMeasureLive ( OS_U32 usOffset, OS_U32 TickRef );

#if defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
void Main_MeasureLive_SetSequencerProfile ( Main_SequencerProfileType profile  );
#else
#define Main_MeasureLive_SetSequencerProfile(profile)
#endif /* defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT) */


#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
OS_I8 Main_Live_DelayOnZC_IsRangeOK(void);
#else
#define Main_Live_DelayOnZC_IsRangeOK()		(OS_I8)(TRUE)
#endif

void Main_LiveMeasure(void);
/* Analog */
void Main_ScheduleAnalogMeasure ( OS_U32 usOffset, OS_U32 TickRef );
void Main_SchedulePeriodicAnalogMeasure ( OS_U32 usOffset, OS_U32 usPeriod, OS_U32 TickRef );
void Main_ActivateAnalogInputs ( void );
void Main_AnalogRefMonitor ( void );
/* Output */
void Main_ScheduleOutputs ( OS_U32 usOffset, OS_U32 usPeriod, OS_I8 count, OS_U32 TickRef );
/* Feedbacks */

#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
void Main_ScheduleFeedbacks ( OS_U32 usOffset, OS_U32 TickRef, OS_U32 period );
void Main_ActivateFeedbacks_AsyncRead ( OS_U32 usOffset );
#else
#define Main_ScheduleFeedbacks( x,y,z)
#define Main_ActivateFeedbacks_AsyncRead(usOffset)
#endif
#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
void Main_ScheduleFeedbacksP ( OS_U32 usOffset, OS_U32 TickRef, OS_U32 period );
void Main_ActivateFeedbacksP_AsyncRead ( OS_U32 usOffset );
#else
#define Main_ScheduleFeedbacksP( x,y,z)
#define Main_ActivateFeedbacksP_AsyncRead(usOffset)
#endif

#if defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC) && defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
OS_I8 Main_Feedbacks_DelayOnZC_IsOK(void);
#else
#define Main_Feedbacks_DelayOnZC_IsOK()		(OS_I8)(TRUE)
#endif

#if defined(CONFIG_MAIN_FEEDBACKS_TASK_DELAY_ON_ZC) && defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
OS_I8 Main_FeedbacksP_DelayOnZC_IsOK(void);
#else
#define Main_FeedbacksP_DelayOnZC_IsOK()	(OS_I8)(TRUE)
#endif

#if defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK) && defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
OS_I8 Main_Feedbacks_Period_IsOK(void);
#else
#define Main_Feedbacks_Period_IsOK()		(OS_I8)(TRUE)
#endif

#if defined(CONFIG_MAIN_FEEDBACKS_TASK_PERIOD_CHECK) && defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
OS_I8 Main_FeedbacksP_Period_IsOK(void);
#else
#define Main_FeedbacksP_Period_IsOK()		(OS_I8)(TRUE)
#endif

/* Sequencer */
#if defined(CONFIG_MAIN_SEQUENCER_TASK_SUPPORT)
void		Main_SequencerInit(void);
void		Main_AddSequencerActivity ( Main_SequencerActivityType* pSeqActivity, Main_ActivityCallbackType*  pCallback );
void    	Main_PeriodicScheduleSequencer ( OS_U32 usOffset, OS_U32 usPeriod, OS_U32 TickRef );
int         Main_GetSequencerLength(void);
Main_SequencerProfileType
			Main_GetSequencerMiddleEvent(void);
#else
#define     Main_PeriodicScheduleSequencer(x,y,z)
#define     Main_SequencerInit()
#define     Main_AddSequencerActivity               ( x )
#define     Main_GetSequencerLength()               (-1)
#define     Main_GetSequencerMiddleEvent()          (0)
#endif


#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
void Main_DispatcherInit(void);
void Main_ActivateDispatcher ( void );
#endif

/* Local Function */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/* Local Function */


#endif /* __MAIN_INTERFACE_H */
/* *********** */
/* End of file */
/* *********** */

