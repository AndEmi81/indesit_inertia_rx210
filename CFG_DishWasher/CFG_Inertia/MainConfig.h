/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup   	MainFrameworkCfg Main framework configuration
@{
\par		Description
\n			Wizard Main framework configuration.
\n          Wizard is a general purpose project.
@}
\ingroup	WizardPrj
*/

/**
\par		Description
\n			Main framework configuration file for Wizard application
\n          The definitions in this file override default ones of the Main framework and can be used 
as starting point for a new final application.
\file		MainConfig.h 
\ingroup	MainFrameworkCfg
\date		Feb 15, 2011 
\version	01.00.00 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:
\n
\n 		00.00.00 
\n		Feb 15, 2011 
\n		Roberto Fioravanti
\n		changes 
\n 			Creation
\n
*/


#ifndef __MAIN_CONFIG_H
#define __MAIN_CONFIG_H

/* ********** */
/* Includes */
/* ********* */

/* general */
#include "Config.h"
#include "callback.h"



/* platform libraries */

#include "DigitalInputs.h"
#include "LIB_Backup.h"
#include "LIB_ZeroCrossing.h"
#include "ZcLoads.h"
#include "LIB_BackupExp.h"
#include "LoadsRequest.h"
#include "LIB_ProductSafety.h"




/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined(OS_TICK_FREQ)
/* conversion from tick freq to period [ms]*/
#define OS_TICK_ms_PERIOD                                   ((1000/(OS_TICK_FREQ)))
#endif  /* !defined(OS_TICK_FREQ) */



/***********/
/* General */

/**
@name Task Priorities definitions.

@{
*/
#define MAIN_SUPERVISOR_TASK_PRIORITY                       (128)   /**< */
#define MAIN_ANALOG_TASK_PRIORITY                           (70)   /**< */
#define MAIN_APPLICATION_TASK_PRIORITY                      (60)   /**< */
#define MAIN_LIVE_TASK_PRIORITY                             (130)    /**< */
#define MAIN_BACKUP_TASK_PRIORITY                           (160)   /**< */
#define MAIN_OUTPUTS_TASK_PRIORITY                          (140)   /**< */
#define MAIN_FEEDBACKS_TASK_PRIORITY                        (91)    /**< */
#define MAIN_FEEDBACKSP_TASK_PRIORITY                       (90)   /**< */
#define MAIN_DISPATCHER_TASK_PRIORITY                       (150)   /**< */

#define CP_COMM_I2C_PRIORITY								(80)
#define CP_COMM_UART_PRIORITY								(68)

#define PM_UM_TASK_PRIORITY 	  							(95)

/*@}*/

#if defined(NXP_LPC12xx_1227)
#define MAIN_ANALOGINPUTS_TASK_STACK_SIZE                   (64)
#define MAIN_BACKUP_TASK_STACK_SIZE                         (64)
#define MAIN_FEEDBACKS_TASK_STACK_SIZE                      (64)
#define MAIN_FEEDBACKS_P_TASK_STACK_SIZE                    (64)
#define MAIN_MEASURELIVE_TASK_STACK_SIZE                    (50)
#define MAIN_OUTPUTS_TASK_STACK_SIZE                        (64)
#endif

#if defined(FREESCALE_MCF51_AG128)
#define MAIN_BACKUP_TASK_STACK_SIZE                         (256)
#define MAIN_OUTPUTS_TASK_STACK_SIZE                        (192)
#define MAIN_SUPERVISOR_TASK_STACK_SIZE                     (256)
#define MAIN_APP_PERIODICASYN_TASK_STACK_SIZE               (384)
#endif


/*********/
/* Debug */


/**
 @name Analog Inputs debug options
 @{
*/
#define MAIN_ANALOG_INPUTS_TASK_DEBUG                       (0)     /**< General 
                                                                        \li 0: all debug features disabled
                                                                        \li 1: allows to enable
                                                                    */

#define MAIN_ANALOG_INPUTS_TASK_DELAY_ON_ZC                 (1)     /**< Measurement of the delay between ZC and the task
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */
#define MAIN_ANALOG_INPUTS_TASK_DURATION                    (0)     /**< Measurement of the duration of the task
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */
#define MAIN_ANALOG_INPUTS_TASK_TUNING                      (0)     /**< tuning for the task activation
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */
#define MAIN_ANALOG_INPUTS_ACTIVATION_TUNING_VAL            (0)
/*@}*/



/**
 @name Live debug options
 @{
*/
#define MAIN_LIVE_TASK_DEBUG                                (1)     /**< General 
                                                                        \li 0: all debug features disabled
                                                                        \li 1: allows to enable
                                                                    */
#define MAIN_LIVE_TASK_DURATION                             (0)     /**< Measurement of the duration of the task
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */

#define MAIN_LIVE_TASK_TUNING                               (1)     /**< tuning for the task activation
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */
#define MAIN_LIVE_TASK_DELAY_ON_ZC                          (1)     /**< Measurement of the delay between ZC and the task
                                                                        \li 0: disabled
                                                                        \li 1: enabled
                                                                    */
                                                            
#define LIVE_OFFSET_ACTIVATION                              (0)
/*@}*/




/**
 @name Synchronized loads debug options
 @{
*/
#define OUTPUTS_TASK_DEBUG                                  (0)     /**< General 
                                                                    \li 0: all debug features disabled
                                                                    \li 1: allows to enable
                                                                    */
#define OUTPUTS_TASK_DURATION                               (0)     /**< Measurement of the duration of the task 
                                                                    \li 0: disabled
                                                                    \li 1: enabled
                                                                    */
#define OUTPUTS_TASK_TUNING                                 (0)     /**< Tuning 
                                                                    \li 0: disabled
                                                                    \li 1: enabled
                                                                    */
#define OUTPUTS_DELAY_ACTIVATION_TUNING_VAL                 (0)
#define OUTPUTS_PERIOD_ACTIVATION_TUNING_VAL                (0)
/*@}*/

/**
 @name Feedbacks debug options
 @{
*/
#define MAIN_FEEDBACKS_TASK_DEBUG                           (1)
#define MAIN_FEEDBACKS_TASK_DELAY_ON_ZC                     (1)
/*@*/


/**
@name Supervisor debug options
 @{ 
*/
#define MEASURE_PERIOD_TASK_PERIODIC_DEBUG                  (0)

/*@}*/


/**
@name Remote Loads Configuration
@{
*/
/** Remote Loads Control configuration. It's the number of bytes of the buffer. */
#define PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE			        (20)
/*@}*/


/**
@name Live service Configuration
@{
*/

/**
number of samples to build live average value in power of two 
*/
#define LIVE_TASK__LIVE_AWG_SAMPLES_POF2                    (6)

/**
if defined enables ADC to RMS conversion of the live channel
*/
#define LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION

/**
Defines how the measure must be threated
*/
#define LIVE_TASK__LIVE_PROCESSING()                

/*@}*/


/**
@name Application service configuration
@{
*/

/**
Application Task period in ms
*/
#define MAIN_APPLICATION_TASK_PERIOD                        (100)

/**
Defines the condition to allow the execution of application task.
The condition is evaluated as stated by MAIN_APPLICATION_TASK_PERIOD.
\return bool_t 
\retval TRUE application task execution will continue
\retval FALSE execution will be stopped

\par Example: ready if 'Ref' is good
In this example task execution will be continued if 'Ref' is good
\code
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       Main_Analog_IsRefOk()
\endcode
        
\par Example: ready if 'Ref' is good and ZC is synchronized
In this example task execution will be continued if 'Ref' is good
\code
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       (Main_Analog_IsRefOk() && Main_IsZcSync())
\endcode        
        

*/
#define MAIN_APP_PERIODICASYN_TASK__IS_READY()       Main_Analog_IsRefOk()


/**
Application task body.
it calls 
\li CALLBACK_Main_ApplicationTask_Ctrl
\li CALLBACK_RemoteLoadsControl_BeforeProcess
\li CALLBACK_RemoteLoadsControl_AfterProcess
\li CALLBACK_Main_ApplicationTask_Body

\par Example
\code
#define MAIN_APP_PERIODICASYN_TASK__BODY()\
if ( CALLBACK_Main_ApplicationTask_Ctrl() )\
{\
  void * pRemoteLoadsControlData = 0;\
  int len=0;\
  \
  Main_RemoteLoadsControl_GetCommand(&pRemoteLoadsControlData, &len);\
  if(pRemoteLoadsControlData && len)\
  {\
    CALLBACK_RemoteLoadsControl_BeforeProcess(pRemoteLoadsControlData, &len);\
    Main_RemoteLoadsControl_ProcessCommand((uint8_t*)pRemoteLoadsControlData);\
    CALLBACK_RemoteLoadsControl_AfterProcess(pRemoteLoadsControlData, &len);\
    Main_RemoteLoadsControl_PurgeCommand();\
  }\
  CALLBACK_Main_ApplicationTask_Body();\
}  
\endcode 
*/
#define MAIN_APP_PERIODICASYN_TASK__BODY()	Main_ApplicationTask_Body()

/**
Defines the application task startup behavior 	
Calls the following callbacks:
\li CALLBACK_Main_StartUp()

\par Example
\code
#define MAIN_APP_PERIODICASYN_TASK__STARTUP()\
    Main_RemoteLoadsControl_Init();\
    CALLBACK_Main_StartUp();
\endcode
*/
#define MAIN_APP_PERIODICASYN_TASK__STARTUP() {\
    Main_RemoteLoadsControl_Init();\
    CALLBACK_Main_StartUp();\
	}
/*@}*/



/**
@name Supervisor service configuration
@{
*/

/**
Supervisor Task period in ms
*/
#define MAIN_SUPERVISOR_TASK_PERIOD                             (10)


#if (MAIN_SUPERVISOR_TASK_PERIOD % OS_TICK_ms_PERIOD) != 0
/*
Period must be a multiple of OS tick defined by OS_TICK_FREQ
*/
#error "MAIN_SUPERVISOR_TASK_PERIOD must be multiple of OS tick (defined by OS_TICK_FREQ).  \
Change OS_TICK_ms_PERIOD or MAIN_SUPERVISOR_TASK_PERIOD"
#else
/*
Defines the Supervisor task period in unit of OS tick.
*/
#define MAIN_SUPERVISOR_TASK__OSTICK_PERIOD             		(MAIN_SUPERVISOR_TASK_PERIOD/OS_TICK_RESOLUTION)
#endif

/**
Body of the period in ms of the periodic task
Calls 
\li CALLBACK_Main_GP()
\li CALLBACK_EnableCheckColdResetTimeout()
*/
#define MAIN_SUPERVISOR_TASK__BODY(_SystemMode, _zc_ok) \
		t0 += MAIN_SUPERVISOR_TASK__OSTICK_PERIOD;		\
		Main_SupervisorBody(_SystemMode, _zc_ok, t0);
    

/*@}*/



/**
@name Analog inputs service configuration
@{
*/

/** Product modules manager 
\code
#define ANALOG_INPUT_TASK__PRODUCT_MODULES()\
    TurbiditySensorManager();\
    PM_AnalogFeedbackManager();
\endcode
*/
#define ANALOG_INPUT_TASK__PRODUCT_MODULES()

#define ANALOG_INPUT_TASK__BODY(_SystemMode)\
	if ( TRUE == LIB_Backup_CheckIfIsTimeToBackup(_SystemMode) )\
	{\
	    Main_ActivateBackup();\
	}

/*@}*/



/**
@name Synchronized loads service configuration
@{
*/

#if defined(CONFIG_PM_PROFILER)
#define PROFILER(idEvent)\
{\
	switch(idEvent)\
	{\
		case (N_LEVEL/2):\
			PM_ProfilesManager(0);\
			break;\
		case (N_LEVEL-1):\
			PM_CheckIfSwitchOffProfile();\
			break;\
		default:\
			break;\
	}\
}
#else
#define PROFILER(idEvent)   {}
#endif /* defined(CONFIG_PM_PROFILER) */


#if ((ENABLE_ZC_LOADS_MODULE == TRUE) && defined(PLATFORM2_ZCLOADS))
#define ZC_LOADS(idEvent)\
(void)LIB_ZcLoads_Manager(0);
#else
#define ZC_LOADS(idEvent)
#define LIB_ZcLoads_InitSync()
#endif /* defined(PLATFORM2_ZCLOADS) */

/**
Output task body.
It can remain undefined if not needed
*/
#define MAIN_OUTPUTS_TASK__BODY(idEvent)\
{\
    PROFILER(idEvent);\
    ZC_LOADS(idEvent);\
}


/*@}*/



/** */
#define EVERY_TIME()                                        (1)

#define PROGRAMMING_MODE_TIMEOUT_s              			(120)            /**< Default timeout in programming mode */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
#endif /*__MAIN_CONFIG_H*/

/* *********** */
/* End of file */
/* *********** */
