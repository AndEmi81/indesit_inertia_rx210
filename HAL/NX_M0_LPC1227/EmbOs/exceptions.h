/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2011  SEGGER Microcontroller GmbH & Co KG         *
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
*       OS version: 3.82u                                            *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : exceptions.h
Purpose : CMSIS compatible definition of exception handler for LPC1227
          To be included in all sources which implement
          or reference exception/irq handler
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Define WEAK attribute                                            */
/* All default exception handlers are implemented weak,             */
/* so they can be replaced by the application.                      */
#if defined   ( __CC_ARM   )
  #define WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
  #define WEAK __weak
#elif defined (  __GNUC__  )
  #define WEAK __attribute__ ((weak))
#endif

/********************************************************************/

/****** Cortex M0 generic exceptions ********************************/

extern WEAK void NMIException        (void);
extern WEAK void HardFaultException  (void);
extern WEAK void SVCHandler          (void);
extern WEAK void PendSV_Handler      (void);
extern WEAK void SysTick_Handler     (void);

/****** Controller specific peripheral interrupts *******************/

extern WEAK void WAKE_UP0_IRQHandler (void);
extern WEAK void WAKE_UP1_IRQHandler (void);
extern WEAK void WAKE_UP2_IRQHandler (void);
extern WEAK void WAKE_UP3_IRQHandler (void);
extern WEAK void WAKE_UP4_IRQHandler (void);
extern WEAK void WAKE_UP5_IRQHandler (void);
extern WEAK void WAKE_UP6_IRQHandler (void);
extern WEAK void WAKE_UP7_IRQHandler (void);
extern WEAK void WAKE_UP8_IRQHandler (void);
extern WEAK void WAKE_UP9_IRQHandler (void);
extern WEAK void WAKE_UP10_IRQHandler(void);
extern WEAK void WAKE_UP11_IRQHandler(void);
extern WEAK void I2C0_IRQHandler     (void);
extern WEAK void CT16B1_IRQHandler   (void);
extern WEAK void CT16B0_IRQHandler   (void);
extern WEAK void CT32B0_IRQHandler   (void);
extern WEAK void CT32B1_IRQHandler   (void);
extern WEAK void SSP0_IRQHandler     (void);
extern WEAK void UART0_IRQHandler    (void);
extern WEAK void UART1_IRQHandler    (void);
extern WEAK void CMP_IRQHandler      (void);
extern WEAK void ADC_IRQHandler      (void);
extern WEAK void WDT_IRQHandler      (void);
extern WEAK void BOD_IRQHandler      (void);
extern WEAK void FLASH_IRQHandler    (void);
extern WEAK void PIO0_IRQHandler     (void);
extern WEAK void PIO1_IRQHandler     (void);
extern WEAK void PIO2_IRQHandler     (void);
extern WEAK void PMU_IRQHandler      (void);
extern WEAK void DMA_IRQHandler      (void);
extern WEAK void RTC_IRQHandler      (void);

#ifdef __cplusplus
  }
#endif

/********************************************************************/

#endif                                  /* Avoid multiple inclusion */

/*************************** End of file ****************************/
