/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/



/**
\n
\Par	On Chip Real Time Clock (RTC)
\n		Implementation of RTC Driver.
\n
\file		OCRTC_drv.c
\ingroup	OCRTC
\date		28-10-2011
\version	Platform2
\author	  Marco Sperandini
\n
\n
\Par\b	History:
\n 
\n
\n
\n 
*/
#define __DRIVER_H

/* ******** */
/* Includes */
/* ******** */




#include "HAL_Interface.h"
/*lint -e537*/
#include "Config.h"
/*lint +e537*/
#include  HAL_CFG
#include  HAL

#include "OCRTC_drv.h"
#include "LIB_TaskMonitoring.h"

#ifdef CONFIG_PL_DRV_OCRTC
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Exported */


/* Static */
static RTC_AlarmCallback_t RTC_AlarmCallback;

/* ******************* */
/* Functions Prototype */
/* ******************* */
void OCRTC_isr( void );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
RTC initialization.
*/
void OCRTC_InitSync(void)
{
    // install ISR
    // ...
    HAL_RTC_ClearInterrupt();
    
    // Init rtc
    HAL_RTC_Init();    
    
    // enable interrupt   
    HAL_RTC_EnableInterrupt();
 
}

uint32_t OCRTC_getTimeSync(void) {
    return HAL_RTC_GetTime();  
}

void OCRTC_setTimeSync(uint32_t value){
    HAL_RTC_SetTime(value);
}

/*void RTC_IRQHandler(void){
    OCRTC_isr();
}*/

int8_t OCRTC_setAlarmSync( uint32_t cmpAlarmValue, RTC_AlarmCallback_t cb ) { 
    // disabilita gli interrupt
    EnterCriticalSection();
    // setta la callback
    RTC_AlarmCallback = cb;  
    
    // setta il match register
    HAL_RTC_SetAlarm(cmpAlarmValue);
    
    HAL_RTC_EnableInterrupt();
    
    // abilita gli interrupt
    ExitCriticalSection();
}

/* local  */

void OCRTC_IsrHandler( void )
{
    uint32_t now;

	IRQ_MONITOR_ENTER();

    HAL_RTC_isr(); // update the rtc hal, clear interrupt ecc...
    
    /* -------------------------- */
    now = HAL_RTC_GetTime();
    
    if( HAL_RTC_IsAlarmExpired() )
    { 
        HAL_RTC_SetAlarm(0); // disable Alarm  
        
        if( RTC_AlarmCallback != 0)
        {      
            RTC_AlarmCallback(now); 
        }
    }

	IRQ_MONITOR_EXIT();

}
/* #include "exceptions.h" */

#endif /* CONFIG_PL_DRV_OCRTC */
/* *********** */
/* End of file */
/* *********** */

