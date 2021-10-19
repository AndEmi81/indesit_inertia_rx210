/**
\defgroup Application configuration

Il modulo contiene ...

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\date    24/05/2012
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL
*/

/**
Header file 

\file    Config.h
\ingroup InertiaCfg
\date    27/04/2010
\author  Roberto Fioravanti
\version 00.00.01
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
#include "StandardDefines.h"
#include "StandardTypes.h"
#include "Trace_cfg.h"
/*RF*/
#include "Loads_cfg.h"
#include "ProductSafety_cfg.h"
#include "Setting_Cfg.h"

#if defined (OS_EmbOS)
#include "RTOS.h"
#endif /* defined (OS_EmbOS) */


#include "BoardConfigurations.h"
#include "WatchDogConfig.h"
#include "Timers_cfg.h"
#include "FlashConfig.h"
#include "CLKConfig.h"
#include "AdcConfig.h"
#include "DIO_cfg.h"

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#include "UART_cfg.h"
#include "ExternalOutputManager_cfg.h"
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */

#include "IRQ_cfg.h"
#include "ZCV_cfg.h"

#include "BackUp_cfg.h"
#include "SWalarms_cfg.h"   /* ghost */

#include "SwTimersCounters.h"

#include "TaskMonitor_Cfg.h"
#if defined(CONFIG_PM_LINEARPRESSURE_SENSOR)
#include "LPS_cfg.h"
#endif /*defined(CONFIG_PM_LINEARPRESSURE_SENSOR)*/
#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)
#include "PLP_cfg.h"
#endif /*defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)*/

/*
Platform libraries
*/

/* 
Product's modules configuration. ghost 
*/
#include "ProductModules_Cfg.h"
#include "ZcLoadsCfg.h"

#if defined(CONFIG_LIB_PRODUCTSAFETY_SUPPORT)
#include "LIB_ProductSafetyExp.h"
#endif

#if defined(CONFIG_PM_ENERGY_METER_SW)
#include "PM_EnergyMeterSwExp.h"
#endif /* defined(CONFIG_PM_ENERGY_METER_SW) */

/* ****** */
/* Errors */
/* ****** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined (CONFIG_HAL_TMR_SUPPORT)
#define HAL_TIMER_SUPPORT
#endif /* defined (CONFIG_HAL_TMR_SUPPORT) */

/* stack configuration */

#if defined(__ICCARM__)     /* EWARM */
#pragma location = ".osstack"
#define STACK_SEC
#define STACK_SEC_POST

#elif defined(__GNUC__)       /* GNU */
#define STACK_SEC
#define STACK_SEC_POST		__attribute__ ((section (".osstack")))

#elif defined(__CWCC__)       /* Codewarrior */
#pragma define_section 		STACK_SECTION ".osstack" far_absolute R
#define STACK_SEC 			__declspec (STACK_SECTION)
#define STACK_SEC_POST
#else
#define STACK_SEC
#define STACK_SEC_POST

#endif




/*External Board's*/

/*External Boards params */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#define IOM_NUM_OF_EXTERNAL_LOADS						(2)
#define IOM_EXTERNAL_0_ENABLED							

#define IOM_EXTERNAL_0_COMMANDBUFFER_SIZE				(1 + BLDC_DRAIN_L_CMD_DATA_SZ + BLDC_WASH_L_CMD_DATA_SZ + IOM_EXT_AW_CDM_DATA_SZ)
#define IOM_EXTERNAL_0_RAMBUFFER_SIZE					(32)	/*16 words*/
#define IOM_EXTERNAL_0_INFOBUFFER_SIZE					(12)	/* 6 words*/
#define IOM_EXTERNAL_0_GLOBAL_DATA_BLOCK_SZ				(12)
#if !defined(_LOADS_CFG_)
extern uint8_t ExtMotor_InfoBuffer[IOM_EXTERNAL_0_INFOBUFFER_SIZE];
#endif

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */


/* rom data addresses */
#if defined(NXP_LPC12xx_1227)
#define APPL_PDM_ADDRESS            (0x0001BAF4)
#define APPL_WBOX_ADDRESS           (0x0001BB00)
#define APPL_BCKP_STAT_ADDRESS      (0x0001BC00)
#define APPL_SETTING_ADDRESS        (0x0001C000)


#define APPL_PDM_SIZE_W             (5)
#define APPL_WBOX_SIZE_W            (128)
#define APPL_APPL_BCKP_STAT_SIZE_W  (512)
#define APPL_SETTING_SIZE_W         (0x2000)
#endif /* defined(NXP_LPC12xx_1227) */


#if defined(FREESCALE_MCF51_AG128)
#define APPL_PDM_ADDRESS            (0x0001BAF4)
#define APPL_WBOX_ADDRESS           (0x0001BB00)
#define APPL_BCKP_STAT_ADDRESS      (0x0001BC00)
#define APPL_SETTING_ADDRESS        (0x0001C000)
#define SETTING_ADDRESS_OFFSET	    (0x00010000)
#define SETTING_VIRTUAL_ADDRESS	    (APPL_SETTING_ADDRESS - SETTING_ADDRESS_OFFSET)
#define PDM_OFFSET					(0XFFFF0000)
#define WBOX_OFFSET		            (0x00010000)
#define WBOX_VIRTUAL_ADDRESS	    (APPL_WBOX_ADDRESS - WBOX_OFFSET)
#define BCKP_STAT_OFFSET	        (0x00010000)
#define BCKP_STAT_VIRTUAL_ADDRESS   (APPL_BCKP_STAT_ADDRESS - BCKP_STAT_OFFSET)

#define APPL_PDM_SIZE_W             (5)
#define APPL_WBOX_SIZE_W            (128)
#define APPL_APPL_BCKP_STAT_SIZE_W  (512)
#define APPL_SETTING_SIZE_W         (0x2000)
#endif /* defined(FREESCALE_MCF51_AG128) */


/* rom data addresses */
#if defined(RN_RX200)
#if defined (RN_RX200_R5F5210_8)
 #define APPL_WBOX_ADDRESS          (0x00100800)
 #define WBOX_OFFSET                (0x000FC000)
 #define WBOX_VIRTUAL_ADDRESS       (APPL_WBOX_ADDRESS - WBOX_OFFSET)

 #define APPL_PDM_ADDRESS           (0xFFFC7000)
 #define PDM_OFFSET					(0XFFFBA000)
 #define PDM_VIRTUAL_ADDRESS		(APPL_PDM_ADDRESS - PDM_OFFSET)

 #define APPL_BCKP_STAT_ADDRESS     (0x00100000)
 #define BCKP_STAT_OFFSET			(0x000FC000)
 #define BCKP_STAT_VIRTUAL_ADDRESS	(APPL_BCKP_STAT_ADDRESS - BCKP_STAT_OFFSET)
 #define APPL_APPL_BCKP_STAT_SIZE_W (512)

 #define APPL_SETTING_ADDRESS       (0xFFFC0000)
 #define SETTING_ADDRESS_OFFSET     (0XFFFBA000)

 #define APPL_SETTING_SIZE_W        (0x3800)

#elif defined (RN_RX200_R5F52Z0_6)
 #define APPL_WBOX_ADDRESS          (0x00100800)
 #define WBOX_OFFSET                (0x000FC000)
 #define WBOX_VIRTUAL_ADDRESS       (APPL_WBOX_ADDRESS - WBOX_OFFSET)

 #define APPL_PDM_ADDRESS           (0xFFFC4000)
 #define PDM_OFFSET					(0XFFFBA000)
 #define PDM_VIRTUAL_ADDRESS		(APPL_PDM_ADDRESS - PDM_OFFSET)

 #define APPL_BCKP_STAT_ADDRESS     (0x00100000)
 #define BCKP_STAT_OFFSET			(0x000FC000)
 #define BCKP_STAT_VIRTUAL_ADDRESS	(APPL_BCKP_STAT_ADDRESS - BCKP_STAT_OFFSET)
 #define APPL_APPL_BCKP_STAT_SIZE_W (512)

 #define APPL_SETTING_ADDRESS       (0xFFFC0000)
 #define SETTING_ADDRESS_OFFSET	    (0XFFFBA000)

 #define APPL_SETTING_SIZE_W        (0x2000)

#elif defined (RN_RX200_R5F52Z0_5)
 #define APPL_WBOX_ADDRESS          (0x00100400)
 #define WBOX_OFFSET                (0x000FC000)
 #define WBOX_VIRTUAL_ADDRESS       (APPL_WBOX_ADDRESS - WBOX_OFFSET)

 #define APPL_PDM_ADDRESS           (0xFFFE4000)
 #define PDM_OFFSET					(0XFFFDA000)
 #define PDM_VIRTUAL_ADDRESS		(APPL_PDM_ADDRESS - PDM_OFFSET)

 #define APPL_BCKP_STAT_ADDRESS     (0x00100000)
 #define BCKP_STAT_OFFSET			(0x000FC000)
 #define BCKP_STAT_VIRTUAL_ADDRESS	(APPL_BCKP_STAT_ADDRESS - BCKP_STAT_OFFSET)
 #define APPL_APPL_BCKP_STAT_SIZE_W (512)

 #define APPL_SETTING_ADDRESS       (0xFFFE0000)
 #define SETTING_ADDRESS_OFFSET	    (0XFFFDA000)

 #define APPL_SETTING_SIZE_W        (0x2000)

#else
 #error "this model of RX200-family is not supported yet"
#endif

#define SETTING_VIRTUAL_ADDRESS		(APPL_SETTING_ADDRESS - SETTING_ADDRESS_OFFSET)

#define APPL_PDM_SIZE_W             (5)
#define APPL_WBOX_SIZE_W            (128)
#define APPL_WBOX_SIZE				(APPL_WBOX_SIZE_W*2)

/** ClassB ADC check tolerance redefinition */
#define HAL_RN_RX200_ADC12_TOLERANCE	20

#endif /* defined(RN_RX200) */


#define APPL_WBOX_END_ADDRESS       (APPL_WBOX_ADDRESS + APPL_WBOX_SIZE_W*2 -1)
#define APPL_SETTING_END_ADDRESS    (APPL_SETTING_ADDRESS + APPL_SETTING_SIZE_W*2 -1)



/* Critical sections */
#if         defined (OS_EmbOS)
#define EnterCriticalSection()          OS_IncDI()
#define ExitCriticalSection()           OS_DecRI()

#define FLS_EnterCriticalSection()      OS_IncDI()
#define FLS_ExitCriticalSection()       OS_DecRI()




#else
#error "Config.h: EnterCriticalSection() and ExitCriticalSection() not defined."
#endif  /*  defined (OS_EmbOS)*/




/* System frequency*/
#if         defined(OS_FSYS)
#define SYS_FREQ_Hz             OS_FSYS
#else   /*  defined(OS_FSYS) */

#if         defined(FREESCALE_MCF51_AG128)
#define SYS_FREQ_Hz             (39845888)
#endif  /*  defined(FREESCALE_MCF51_AG128) */

#endif  /*  defined(OS_FSYS) */

#if         defined(FREESCALE_MCF51_AG128)
#define BUS_FREQUENCY_Hz  		(SYS_FREQ_Hz/2)
#endif  /*  defined(FREESCALE_MCF51_AG128) */

#if         defined(NXP_LPC12xx_1227)
#define BUS_FREQUENCY_Hz  		(SYS_FREQ_Hz)
#endif  /*  defined(NXP_LPC12xx_1227) */

#if         defined(RN_RX200)
#define BUS_FREQUENCY_Hz  		(20000000)
#endif  /*  defined(RN_RX200) */

/**************/
/* I2C driver */
/**************/

/****************************/
/* I2C driver configuration */
#define HAL_I2C_SUPPORT
#if defined(FREESCALE_MCF51_AG128)
#define I2C_OCDMA_SUPPORT
#endif

#define I2C_CLOCKRATE_kbps                          100
#define I2C_SLAVEADDRESS                            0x90
#define I2C_SLAVE_SUPPORT                           TRUE
#define I2C_MASTER_SUPPORT                          FALSE


/* ISR */
#define I2C_BUS_MONITOR_SDA                         TRUE
#define I2C_NESTING_ISR_ENABLE                      FALSE

/* Unblock routines param */
#define I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC   TRUE
#define I2CMONITOR_NOACTIVITY_TIMEOUT               (5000)
#define I2CMONITOR_LEV1_LOCAL_OCCURS                (5)             /* local attempts for unlock on block 1 */
#define I2CMONITOR_LEV1_GLOBAL_OCCURS               (3)             /* local attempts for unlock on block 1 */


#define PERIPHERAL_MONITOR_POLL                     (10)            /* block polling in ms */
#define I2CMONITOR_BUSY_BLOCK_TIME_LEV1             (150)           /* level 1 unblock time in ms*/

#if defined(I2C_OCDMA_SUPPORT)
#define I2C_SWDMA_SUPPORT							0
#else
#define I2C_SWDMA_SUPPORT							1
#endif

#if defined(HAL_UART_SUPPORT) && ! defined(UART_OCDMA_SUPPORT)
#define UART_SWDMA_SUPPORT							1
#else
#define UART_SWDMA_SUPPORT							0
#endif


#define HAL_SWDMA_SUPPORT
#define SWDMA_CH_NUM								(UART_SWDMA_SUPPORT + I2C_SWDMA_SUPPORT)
#define SWDMA_SIZE_8BITs_TRANSFER 					TRUE
#define SWDMA_FIXED_SIZE							TRUE

/*****************************/
/* UART driver configuration */
#define UART_AUTOMATIC_CIRCULAR_BUFFER              FALSE

#define OCUART_BUFFER_LSIZE	                        (16)            /* circular buffer*/
#if defined(FREESCALE_MCF51_AG128)
#define UART_OCDMA_SUPPORT
#endif

/**********/
/* dma    */

#if defined(I2C_OCDMA_SUPPORT) || defined(UART_OCDMA_SUPPORT)
#define HAL_OCDMA_SUPPORT      
#endif




#if defined(FREESCALE_MCF51_AG128)
/*uart*/
#if HAL_OPTION_UART == 1
#define UART_RX_DMA_CH                              0
#define UART_RX_DMA_DEV                             13
#define UART_TX_DMA_CH                              1
#define UART_TX_DMA_DEV                             11
extern void OCDMA_CH0_ISR(void);
#define Dma00_Isr_Handler                           OCDMA_CH0_ISR
#elif HAL_OPTION_UART == 2
#define UART_RX_DMA_CH                              3
#define UART_RX_DMA_DEV                             10
#define UART_TX_DMA_CH                              2
#define UART_TX_DMA_DEV                             13
extern void OCDMA_CH3_ISR(void);
#define Dma03_Isr_Handler                           OCDMA_CH3_ISR
#endif

/*I2C*/
#if HAL_OPTION_UART == 1 || !defined(HAL_UART_SUPPORT)
#define I2C_DMA_CH                                  3
#define I2C_DMA_DEV                                 15
extern void OCDMA_CH3_ISR(void);
#define Dma03_Isr_Handler                           OCDMA_CH3_ISR
#elif HAL_OPTION_UART == 2
#define I2C_DMA_CH                                  0
#define I2C_DMA_DEV                                 10
extern void OCDMA_CH0_ISR(void);
#define Dma00_Isr_Handler                           OCDMA_CH0_ISR
#endif


/**/

#endif /* defined(FREESCALE_MCF51_AG128) */


/**********/
/* timers */
#define HAL_TIMER_SUPPORT

/**********/
/* I2C 	  */
void OCI2C_ISR_Slave (void);
extern void I2C_ISR_CALLBACK (void);

/** 
This option is used by:
\li Output manager, 
\li CP module configuration (SetLoad request)
*/
#define __GENERICIOMANAGER_H

/* ************* */
/* Class B Check */
/* ************* */

#if defined (RN_RX200_R5F52Z0_5)
/* RX 2Z0 128K has rodata in Data Flash */
#define NUM_FLASH_CBOARD_TEST_BLOCKS    2
#else
#define NUM_FLASH_CBOARD_TEST_BLOCKS    1
#endif /* defined (RN_RX200_R5F52Z0) */
#define NUM_RAM_CBOARD_TEST_BLOCKS      5

/* *************************** */
/* Custom Fault Priorities     */
/* *************************** */

/* #define LIB_SETTINGFILE_CUSTOM_FAULT_PRIORITY		1 */

#if defined(LIB_SETTINGFILE_CUSTOM_FAULT_PRIORITY)
extern const Loader_Struct CSTM_ApplFaultPriorities;
extern const Loader_Struct CSTM_PltFaultPriorities;
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* HAL timer channel index configuration 
	TmrChMap entry
*/
typedef enum eTIMERS_CH_IDX {

	  TMR0_IDX,

	  TMR1_IDX,

	  TMR2_IDX,

	  TMR3_IDX,

	  TMR4_IDX,

	  TMR5_IDX,

	  TMR6_IDX,

	  TMR7_IDX,

	  TMR8_IDX,

}TimersChType;




/* ****************** */
/* Exported variables */
/* ****************** */
#if 0
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
void SetupHw(void);
/*common prototypes*/
void ICUZC_Isr(void);
void InitSema(void);
void Main_ApplicationTask_Body(void);
void Main_SupervisorBody( SysMode_Type sysMode, bool_t ZC_Ok, int t0);
void Main_OnSupervisorStart (void);

void Main_EnableRemoteTimeout(void);
bool_t Main_IsRemoteEnabled(void);
bool_t Main_IsTimeForColdReset (void);


#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
void UseExternalBoard(void);
void UnuseExternalBoard(void);

void UseExternalInfo(void);
void UnuseExternalInfo(void);
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */

/* Function defined in BusCommPltf_cfg.h */
void CP_UART_EnableComm(void);
void CP_UART_DisableComm(void);

uint16_t CP_GetPPAddr(const void *realAddr);

uint8_t CP_GetExternalBoardCommStatus(void);
void CP_ResetExternalBoardCommStatus(void);

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
void UsePowerProfile(void);
void UnusePowerProfile(void);
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

#endif /* __APL_CFG_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
