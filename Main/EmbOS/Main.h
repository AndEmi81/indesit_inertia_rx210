/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n			EmbOS-based Main header file
\n
\file		Main.h
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

#ifndef MAIN_EMBOS_H_
#define MAIN_EMBOS_H_


/* ******** */
/* Includes */
/* ******** */
#include "MainConfig.h"
#include "MainInterface.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */





#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) && defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
#define MAIN_FEEDBACK_BOTH  2
#elif defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) && !defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
#define MAIN_FEEDBACK_BOTH  1
#elif !defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE) && defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
#define MAIN_FEEDBACK_BOTH  1
#endif












/**
@name Default EmbOS stack definitions
@{
*/

#ifndef MAIN_ANALOG_INPUTS_TASK_STACK_SIZE
/** Analog task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_ANALOGINPUTS_TASK_STACK_SIZE               (128)
\endcode
*/
#define MAIN_ANALOG_INPUTS_TASK_STACK_SIZE               (128)
#endif

#ifndef MAIN_SUPERVISOR_TASK_STACK_SIZE
/** General purpose task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_SUPERVISOR_TASK_STACK_SIZE                 128
\endcode
*/
#define MAIN_SUPERVISOR_TASK_STACK_SIZE                 (128)
#endif
#ifndef MAIN_BACKUP_TASK_STACK_SIZE
/** Backup task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_BACKUP_TASK_STACK_SIZE                     128
\endcode
*/
#define MAIN_BACKUP_TASK_STACK_SIZE                     (128)
#endif
#ifndef MAIN_OUTPUTS_TASK_STACK_SIZE
/** Synchronized loads task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_OUTPUTS_TASK_STACK_SIZE                    128
\endcode
*/
#define MAIN_OUTPUTS_TASK_STACK_SIZE                    128
#endif
#ifndef MAIN_MEASURELIVE_TASK_STACK_SIZE
/** Feedback task stack
This property is shared between all instances of the task.

\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_MEASURELIVE_TASK_STACK_SIZE                128
\endcode
*/
#define MAIN_MEASURELIVE_TASK_STACK_SIZE                128
#endif
#ifndef MAIN_FEEDBACKS_TASK_STACK_SIZE
/** Normal feedbacks task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_FEEDBACKS_TASK_STACK_SIZE                  128
\endcode
*/
#define MAIN_FEEDBACKS_TASK_STACK_SIZE                  128
#endif
#ifndef MAIN_FEEDBACKS_P_TASK_STACK_SIZE
/** Delayed feedbacks task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_FEEDBACKS_P_TASK_STACK_SIZE                128
\endcode
*/
#define MAIN_FEEDBACKS_P_TASK_STACK_SIZE                128
#endif
#ifndef MAIN_APP_PERIODICASYN_TASK_STACK_SIZE
/** Application task stack
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define MAIN_APP_PERIODICASYN_TASK_STACK_SIZE           128
\endcode
*/
#define MAIN_APP_PERIODICASYN_TASK_STACK_SIZE           128
#endif

#ifndef CONFIG_MAIN_DISPATCHER_TASK_STACK_SIZE
#define CONFIG_MAIN_DISPATCHER_TASK_STACK_SIZE             64
#endif


/*  @} */

/**
@name Default EmbOS task priority
@{
*/
#ifndef MAIN_BACKUP_TASK_PRIORITY
#define MAIN_BACKUP_TASK_PRIORITY               (160)   /**< */
#endif
#ifndef MAIN_OUTPUTS_TASK_PRIORITY
#define MAIN_OUTPUTS_TASK_PRIORITY              (140)   /**< */
#endif
#ifndef MAIN_SUPERVISOR_TASK_PRIORITY
#define MAIN_SUPERVISOR_TASK_PRIORITY           (128) /**< */
#endif
#ifndef MAIN_FEEDBACKS_TASK_PRIORITY
#define MAIN_FEEDBACKS_TASK_PRIORITY            (91)    /**< */
#endif
#ifndef MAIN_FEEDBACKSP_TASK_PRIORITY
#define MAIN_FEEDBACKSP_TASK_PRIORITY            (90)    /**< */
#endif
#ifndef MAIN_ANALOG_TASK_PRIORITY
#define MAIN_ANALOG_TASK_PRIORITY                (70)   /**< */
#endif
#ifndef MAIN_APPLICATION_TASK_PRIORITY
#define MAIN_APPLICATION_TASK_PRIORITY           (60)   /**< */
#endif
#ifndef MAIN_LIVE_TASK_PRIORITY
#define MAIN_LIVE_TASK_PRIORITY                 (40)   /**< */
#endif

#ifndef MAIN_SEQUENCER_TASK_PRIORITY
#define MAIN_SEQUENCER_TASK_PRIORITY            (150)   /**< */
#endif

/*  @} */

/**
@name General options
@{
*/
#ifndef PROGRAMMING_MODE_TIMEOUT_s
#define PROGRAMMING_MODE_TIMEOUT_s              (60)            /**< Default timeout in programming mode */
#endif /* PROGRAMMING_MODE_TIMEOUT_s */


#define SKELETON_TRACE 0
#if OS_TRACE && SKELETON_TRACE
enum
{
	SKELETON_TRACE_ID_GP_HP_TASK = 101,
};
#endif

/*@}*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


typedef ADC_ValueType Main_LivePeakMeasureADCType;
typedef int16_t Main_LiveRMSType;

/**
Main global struct
*/





/* ********* */
/* Variables */
/* ********* */

/* Global */
/**
@name General options
@{
*/
/**
Main global variable
*/


/*@}*/



/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/** @name Remote loads control
@{
*/
void Main_RemoteLoadsControl_Init ( void );
void Main_RemoteLoadsControl_GetCommand ( void** ppData, int* pLen );
void Main_RemoteLoadsControl_ProcessCommand ( void* pData );
void Main_RemoteLoadsControl_PurgeCommand ( void );
/*@}*/

/**
@name Live
@{
*/
void Main_AvgIndexManager ( ADC_ValueType ADC_LivePeakValue );
ADC_ValueType Main_GetLivePeakValueADC ( void );
#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)
Main_LiveRMSType Main_GetLiveValue_Rms(void);
#endif  /* defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION) */

void Main_LiveInit ( void );
/*@}*/

/**
@name Application
@{
*/
void Main_ApplicationInit ( void );
/*@}*/


/**
 @name Backup library
 @{
*/

void Backup_SystemBackup ( uint8_t _SystemMode );
bool_t DoRestoreSystem ( void );

/*@}*/

/**
 @name Backup
 @{
*/
void Main_BackupInit ( void );
void Main_ActivateBackup ( void );
uint8_t Main_IsRestoreCompleted ( void );
void Main_CompleteRestore ( void );
void Main_Backup_DoRestore ( void );
/*@}*/


/**
 @name Analog
 @{
*/
void Main_AnalogInit ( void );
uint8_t Main_Analog_IsRefOk ( void );
void Main_ActivateAnalogMeasureWithOffset ( OS_U32 usOffset );
/*@*/

/**
 @name Synchronized loads
 @{
*/
#if defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT )
#define Main_OutputsInit()
#else
void Main_OutputsInit ( void );
#endif /* defined( CONFIG_MAIN_SEQUENCER_TASK_SUPPORT ) */
/*@}*/


/**
 @name Feedbacks
 @{
*/

#if defined(CONFIG_MAIN_NORMAL_FEEDBACKS_READ_ENABLE)
#else
#define Main_ActivateFeedbacks_AsyncRead(usOffset)
#endif

#if defined(CONFIG_MAIN_DELAYED_FEEDBACKS_READ_ENABLE)
#else
#define Main_ActivateFeedbacksP_AsyncRead(usOffset)
#endif

void Main_FeedbacksInit ( void );


/*@}*/


/**
 @name Supervisor
 @{
*/
uint8_t Main_IsZcSync ( void );
/*@}*/

/* Local Function */



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Local Function */


#endif /* MAIN_EMBOS_H_ */
/* *********** */
/* End of file */
/* *********** */


