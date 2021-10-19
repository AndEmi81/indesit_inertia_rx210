/**
\defgroup Application configuration

Il modulo contiene ...

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\date    12/03/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL
*/

/**
Header file 

\file    .h
\ingroup Config
\date    09/11/2009
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup Config
@{*/



#ifndef __APL_CFG_H
#define __APL_CFG_H
/* ******** */
/* Includes */
/* ******** */
#include "autoconf.h"
#include "SWTCcfg.h"
#include "ProductConfig.h"
#include "AdcConfig.h"
#include "BoardConfigurations.h"
#include "DIO_cfg.h"

#include "SkeletonFuncLib.h"

#include "ExternalOutputManager_cfg.h"
/* ****** */
/* Errors */
/* ****** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/*Setting*/ /*PLT2.5*/
#define 		APPL_SETTING_SIZE_W		SETTING_FILE_SIZE_IN_WORDS


#define __GENERICIOMANAGER_H	/* Extended SetLoad */

#define MCU_LINKAGE	/* present on DEA700 platform */	
#if 0 /*PLT2.5*/
#define CONFIG_BLDC_DRAIN_L_SUPPORT
#define CONFIG_BLDC_WASH_L_SUPPORT
#endif
/* IOM --------------------------------------------------------------------*/
#define EXTERNAL_MOTOR_CONTROL
#define IOM_NUM_OF_EXTERNAL_LOADS						2
#define IOM_EXTERNAL_0_ENABLED							
#define IOM_EXTERNAL_0_COMMANDBUFFER_SIZE				(1 + BLDC_DRAIN_L_CMD_DATA_SZ + BLDC_WASH_L_CMD_DATA_SZ + IOM_EXT_AW_CDM_DATA_SZ)
#define IOM_EXTERNAL_0_RAMBUFFER_SIZE					32
#define IOM_EXTERNAL_0_INFOBUFFER_SIZE					12

/* OSEK --------------------------------------------------------------------*/
#if defined(OSEK_SUPPORT)
#if defined(BUILD_RENESAS_36079)
#include "osek.h"
#endif
#define EnterCriticalSection()      SuspendAllInterrupts()
#define ExitCriticalSection()       ResumeAllInterrupts()
#define CriticalSectionBegin(res)   GetResource(res)
#define CriticalSectionEnd(res)     ReleaseResource(res)


#define UseExternalBoard()			CriticalSectionBegin(Engine_Res)
#define	UnuseExternalBoard() 		CriticalSectionEnd(Engine_Res)

#define UseExternalInfo() 			CriticalSectionBegin(ExtBoardInfo_Res)
#define UnuseExternalInfo() 		CriticalSectionEnd(ExtBoardInfo_Res)
#elif defined(_lint)
#define EnterCriticalSection()      
#define ExitCriticalSection()       
#define CriticalSectionBegin(res)   
#define CriticalSectionEnd(res)     
#else
#define EnterCriticalSection()      EnterCritical()
#define ExitCriticalSection()       ExitCritical()
#define CriticalSectionBegin(res)   EnterCritical()
#define CriticalSectionEnd(res)     ExitCritical()
#endif

#pragma abs16(OCUART_Buffer)
#pragma abs16(OCUART)
#pragma abs16(OCI2C)

#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#define BUS_FREQUENCY_Hz  	10000000
#endif

#if defined(BUILD_RENESAS_36079)
#define BUS_FREQUENCY_Hz  	16000000
#endif

#define HAL_I2C_SUPPORT
#define I2C_CLOCKRATE_kbps  200
#define I2C_SLAVEADDRESS    0x90
#define I2C_SLAVE_SUPPORT   TRUE
#define I2C_MASTER_SUPPORT  FALSE

#define I2C_ISR_CALLBACK    I2C_IsrCallback
/* ISR */
#define I2C_BUS_MONITOR_SDA TRUE
#define I2C_NESTING_ISR_ENABLE  FALSE
/* Unblock */
#define I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC TRUE
#define I2CMONITOR_NOACTIVITY_TIMEOUT           5000
#define I2CMONITOR_LEV1_LOCAL_OCCURS            5           /* num tentativi locali blocco 1 */
#define I2CMONITOR_LEV1_GLOBAL_OCCURS           3


#define PERIPHERAL_MONITOR_POLL                 10          /* unità di tempo in ms */
#define I2CMONITOR_BUSY_BLOCK_TIME_LEV1         150


extern void IlopCmd(void);
#define SW_RESET_MCU IlopCmd
#define HAL_SWDMA_SUPPORT
#define SWDMA_CH_NUM      2
#define SWDMA_SIZE_8BITs_TRANSFER TRUE
#define SWDMA_FIXED_SIZE	TRUE

#define OCUART_BUFFER_SIZE	20



/*Uart*/
#define HAL_UART_SUPPORT
#define HAL_OPTION_UART								1
#define HAL_OPTION_UART_ASYN_MODE					TRUE
#define HAL_OPTION_UART_DATALEN						8
//#define HAL_OPTION_UART1_PARITY					UART_PARITY_NONE
#define HAL_OPTION_UART_USE_PARITY					FALSE
#define HAL_OPTION_UART_STOP						1

#define HAL_OPTION_UART_BAUDRATE_DIVISOR			0
#define HAL_OPTION_UART_BAUDRATE					9600


/* Platform SW counters*/


#define PLATFORM_SW_COUNTERS	\
	&Time2Death_Counter,				/** Contatore reset PC Control */	\
	&Time_Since_Last_Reset_Counter,	/** Contatore dall'ultimo reset */

#define PLATFORM_SW_COUNTERS_CFG	\
	&CfgTime2Death_Counter,				/** Contatore reset PC Control */	\
	&CfgTime_Since_Last_Reset_Counter,	/** Contatore dall'ultimo reset */

#define TIME2DEATH_COUNTER_ID			0
#define TIME_SINCE_LAST_RESET_COUNTER_ID			1

void OCI2C_ISR_Slave (void);
extern void I2C_ISR_CALLBACK (void);




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */


extern unsigned char StatusLen;
/* ******************* */
/* Function prototypes */
/* ******************* */

void TimerHook( void);



#endif /* __APL_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */