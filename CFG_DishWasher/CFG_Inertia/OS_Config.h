/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\par		Purpose
\n			RTOS configuration

\file		OS_Config.h 
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
/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2010  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 3.82p                                            *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : OS_Config.h
Purpose : Configuration settings for the OS build and OS_VIEW
--------  END-OF-HEADER  ---------------------------------------------
*/






#ifndef OS_CONFIG_H                     /* Avoid multiple inclusion */
#define OS_CONFIG_H
/* ******** */
/* Includes */
/* ******** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

#define OS_DISABLE_WATCHDOG                             (0)
#if defined(FREESCALE_MCF51_AG128)
/**
@name Freescale AG128 System Timer configuration
@{
*/
#define OS_SYSTIMER_1
#define OS_SYSTIMER_CH4
#define OS_ALMTIMER_CH5

#define OS_TICK_HANDLE_EX
/*@}*/
#endif

#if defined(NXP_LPC12xx_1227)

/**
@name NXP LPC1227 System Timer configuration
@{
*/
#define OS_SYSTIMER_1
#define OS_ALMTIMER_CH0
/*@}*/
#endif


#if defined(RN_RX200)
/**
@name RX200 Series System Timer configuration
@{
*/
#define OS_TICK_HANDLE_EX
#define OS_SYSALARM_TMR01_B
/*@}*/


/**
@name RX200 Series Alarm Timer configuration
@{
*/
#define OS_SYSTIMER_TMR01_A
/*@}*/
#endif /* defined(RN_RX200) */

#define OS_SYSTEM_TIMER_FREERUNNING               	    (1)         /**< System timer as freerunning */
#define OS_TICK_FREQ								    (100)       /**< System tick frequecy [Hz]) */
#define OS_TICK_RESOLUTION  						    (1)         /**< tick resolution [ms]) */


#if !defined(OS_TICK_RESOLUTION)
#define OS_TICK_RESOLUTION                              ((1000/(OS_TICK_FREQ)))
#endif  /* !defined(OS_TICK_RESOLUTION) */

/**
@name Vector
@{
*/
#define OS_ISRHANDLERTABLE_REDIRECTION            	    (1)
#define OS_ISRHANDLERTABLE_STATIC                 	    (1)
/*@}*/



/* Initial ISRs value  */
/**
@name ISR table configuration
@{
*/
/************************* I2C DMA **************************************/
#if defined(FREESCALE_MCF51_AG128)
extern void OCI2C_ISR(void);
#endif /* defined(FREESCALE_MCF51_AG128) */


#define Iic00_Isr_Handler                           OCI2C_ISR

#define I2C00_ISR_ENABLE
/************************* ZC *******************************************/
#if defined(NXP_LPC12xx_1227)
#define Irq_Isr_Handler                             IRQ_ISR
#endif /* defined(NXP_LPC12xx_1227) */



#if defined(FREESCALE_MCF51_AG128)
extern void IRQ_ISR(void);
#define Irq_Unmaskable_Handler                      IRQ_ISR

extern void LIB_ZC_Maskable(void);
#define Irq_MaskableHandler                         LIB_ZC_Maskable
#endif /* defined(FREESCALE_MCF51_AG128) */


/*
#if defined(FREESCALE_MCF51_AG128)
extern void ICUZC_Isr(void);
#define _OS_IrqHandler76                            ICUZC_Isr
#endif
*/
/************************* EXTERNAL MOTOR **************************************/
#if defined(FREESCALE_MCF51_AG128)
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)

extern void OCDMA_CH0_ISR(void);
#define Dma00_Isr_Handler                           OCDMA_CH0_ISR

extern void OCUART_ErrMngr(void);
extern void OCUART_TxDataMngr(void);
extern void OCUART_RxDataMngr(void);

#define _OS_IrqHandler96                            OCUART_ErrMngr
#define _OS_IrqHandler97                            OCUART_RxDataMngr
#define _OS_IrqHandler98                            OCUART_TxDataMngr
#endif  /* defined(EXTERNAL_MOTOR_CONTROL)*/
#endif  /* defined(FREESCALE_MCF51_AG128) */

/*@}*/

/*********************************************************************
*
*       Default configuration
*
**********************************************************************
*/

#ifndef   DEBUG           /* Should be overwritten by project settings*/
  #define DEBUG     (0)   /* in debug builds*/
#endif

/*********************************************************************
*
*       Configuration for RTOS build and UART
*
*  One of the following builds needs to be selected for both DEBUG and Release builds:
*
*  OS_LIBMODE_XR    Extremly small release build without Round robin
*  OS_LIBMODE_R     Release build
*  OS_LIBMODE_S     Release build with stack check
*  OS_LIBMODE_SP    Release build with stack check and profiling
*  OS_LIBMODE_D     Debug build
*  OS_LIBMODE_DP    Debug build with profiling
*  OS_LIBMODE_DT    Debug build with trace
*/
#if defined(DEBUG)

#if   DEBUG == 2
  #define OS_LIBMODE_DT
#endif  /*DEBUG == 2*/

#if   DEBUG == 1
  #define OS_LIBMODE_DP
#endif  /*DEBUG == 1*/
  
#if   DEBUG == 0
  #define OS_LIBMODE_D
#endif  /*DEBUG == 0*/  
  

#if   DEBUG == (-1)
  #define OS_LIBMODE_SP
#endif  /*DEBUG == -1*/

#if   DEBUG == (-2)
  #define OS_LIBMODE_S
#endif  /*DEBUG == -2*/

#else /*defined(DEBUG)*/
  #define OS_LIBMODE_R
  #define OS_VIEW_ENABLE   0
#endif /*defined(DEBUG)*/

/********************************************************************/

#endif  /*OS_CONFIG_H*/                 /* Avoid multiple inclusion */

/*************************** End of file ****************************/

