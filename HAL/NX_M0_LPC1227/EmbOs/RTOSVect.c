/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2012  SEGGER Microcontroller GmbH & Co KG         *
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
*       OS version: 3.84c                                            *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : RTOSVect.c
Purpose : Vector table for NXP LPC1227.
--------  END-OF-HEADER  ---------------------------------------------
*/


/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/

#include "RTOS.h"
#include "exceptions.h"    // CMSIS compatible exception handler definition
#include "derivative.h"

#ifdef __cplusplus
  extern "C" {
#endif

/*********************************************************************
*
*       Reference to other moduls
*
**********************************************************************
*/

#pragma language=extended
#pragma section="CSTACK"
extern void __iar_program_start(void);

/**********************************************************************
*
*       Defines
*
***********************************************************************
*/
#define _NVIC_HFSR       *((volatile unsigned long *)(0xE000ED2C))

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _IllegalException()
*
*       Is called from any exception handler which is not implemeted
*       by the application (not overwriiten by user).
*       We implemented and endless loop, so the programm stops when any
*       of the exceptions is called.
*       Using a debugger and setting a breakpoint here allows to analyze
*       which exception / interrupt was called.
*       Normally, the CPU should not arrive here.
*/
static void _IllegalException(void) {
  //
  // In case we received a hard fault because
  // of a breakpoint instruction we return.
  // This may happen with the IAR compiler when using semihosting.
  //
  if (_NVIC_HFSR & 1UL << 31) {
    _NVIC_HFSR |= (1UL << 31);
    return;
  }
  while(1);
}

/*********************************************************************
*
*       Dummy system exception handler
*
*       We implemented a call to _IllegalException(), so the program
*       calls this function when any of the exceptions is called.
*       Using a debugger and setting a breakpoint in _IllegelException()
*       allows to analyze which exception / interrupt was called.
*/
WEAK void NMIException        (void) { _IllegalException(); }
WEAK void HardFaultException  (void) { _IllegalException(); }
WEAK void SVCHandler          (void) { _IllegalException(); }
WEAK void PendSV_Handler      (void) { _IllegalException(); }
WEAK void SysTick_Handler     (void) { _IllegalException(); }

/*********************************************************************
*
*       Default dummy interrupt handler
*
*       We implemented a call to _IllegalException(), so the program
*       calls this function when any of the exceptions is called.
*       Using a debugger and setting a breakpoint in _IllegelException()
*       allows to analyze which exception / interrupt was called.
*/

WEAK void WAKE_UP0_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP1_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP2_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP3_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP4_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP5_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP6_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP7_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP8_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP9_IRQHandler (void) { _IllegalException(); }
WEAK void WAKE_UP10_IRQHandler(void) { _IllegalException(); }
WEAK void WAKE_UP11_IRQHandler(void) { _IllegalException(); }
WEAK void I2C0_IRQHandler     (void) { _IllegalException(); }
WEAK void CT16B0_IRQHandler   (void) { _IllegalException(); }
WEAK void CT16B1_IRQHandler   (void) { _IllegalException(); }
WEAK void CT32B0_IRQHandler   (void) { _IllegalException(); }
WEAK void CT32B1_IRQHandler   (void) { _IllegalException(); }
WEAK void SSP0_IRQHandler     (void) { _IllegalException(); }
WEAK void UART0_IRQHandler    (void) { _IllegalException(); }
WEAK void UART1_IRQHandler    (void) { _IllegalException(); }
WEAK void CMP_IRQHandler      (void) { _IllegalException(); }
WEAK void ADC_IRQHandler      (void) { _IllegalException(); }
WEAK void WDT_IRQHandler      (void) { _IllegalException(); }
WEAK void BOD_IRQHandler      (void) { _IllegalException(); }
WEAK void FLASH_IRQHandler    (void) { _IllegalException(); }
WEAK void PIO0_IRQHandler     (void) { _IllegalException(); }
WEAK void PIO1_IRQHandler     (void) { _IllegalException(); }
WEAK void PIO2_IRQHandler     (void) { _IllegalException(); }
WEAK void PMU_IRQHandler      (void) { _IllegalException(); }
WEAK void DMA_IRQHandler      (void) { _IllegalException(); }
WEAK void RTC_IRQHandler      (void) { _IllegalException(); }

/*********************************************************************
*
*       Global code
*
**********************************************************************
*/

/*********************************************************************
*
*       Exception vector table
*/

/****** NXP LPC122x Cortex M0 Vector Table entries ******************/
#pragma location = ".appentry2"
const intvec_elem __vector_table2[] =
{
  { .__ptr = __sfe( "CSTACK" ) },
  __iar_program_start
};

#pragma location = ".appentry"
const intvec_elem __appentry_table[] =
{
  __iar_program_start,
};



#pragma location = ".intvec"
const intvec_elem __vector_table[] =
{
  { .__ptr = __sfe( "CSTACK" ) },
  __iar_program_start,
  NMIException,
  HardFaultException,
  0,                    /* 4:  Reserved                        */
  0,                    /* 5:  Reserved                        */
  0,                    /* 6:  Reserved                        */
  0,                    /* 7:  Reserved, CRC located here      */
  0,                    /* 8:  Reserved                        */
  0,                    /* 9:  Reserved                        */
  0,                    /* 10: Reserved                        */
  SVCHandler,
  0,                    /* 12: Reserved                        */
  0,                    /* 13: Reserved                        */
  PendSV_Handler,       /* Required for the OS !               */
  SysTick_Handler,      /* Required for the OS !               */

  /**** Peripheral exceptions / interrupts 16-46               */

  WAKE_UP0_IRQHandler,  /* 16: WAKE_UP0 Handler                */
  WAKE_UP1_IRQHandler,  /* 17: WAKE_UP1 Handler                */
  WAKE_UP2_IRQHandler,  /* 18: WAKE_UP2 Handler                */
  WAKE_UP3_IRQHandler,  /* 19: WAKE_UP3 Handler                */
  WAKE_UP4_IRQHandler,  /* 20: WAKE_UP4 Handler                */
  WAKE_UP5_IRQHandler,  /* 21: WAKE_UP5 Handler                */
  WAKE_UP6_IRQHandler,  /* 22: WAKE_UP6 Handler                */
  WAKE_UP7_IRQHandler,  /* 23: WAKE_UP7 Handler                */
  WAKE_UP8_IRQHandler,  /* 24: WAKE_UP8 Handler                */
  WAKE_UP9_IRQHandler,  /* 25: WAKE_UP9 Handler                */
  WAKE_UP10_IRQHandler, /* 26: WAKE_UP10 Handler               */
  WAKE_UP11_IRQHandler, /* 27: WAKE_UP11 Handler               */
  I2C0_IRQHandler,      /* 28: I2C0 Handler                    */
  CT16B0_IRQHandler,    /* 29: CT16B0 Handler                  */
  CT16B1_IRQHandler,    /* 30: CT16B1 Handler                  */
  CT32B0_IRQHandler,    /* 31: CT32B0 Handler                  */
  CT32B1_IRQHandler,    /* 32: CT32B1 Handler                  */
  SSP0_IRQHandler,      /* 33: SSP0 Handler                    */
  UART0_IRQHandler,     /* 34: UART0 Handler                   */
  UART1_IRQHandler,     /* 35: UART0 Handler                   */
  CMP_IRQHandler,       /* 36: CMP Handler                     */
  ADC_IRQHandler,       /* 37: ADC Handler                     */
  WDT_IRQHandler,       /* 38: WDT Handler                     */
  BOD_IRQHandler,       /* 39: BOD Handler                     */
  FLASH_IRQHandler,     /* 40: FLASH Handler                   */
  PIO0_IRQHandler,      /* 41: PIO0 Interupt Handler           */
  PIO1_IRQHandler,      /* 42: PIO1 Interupt Handler           */
  PIO2_IRQHandler,      /* 43: PIO2 Interupt Handler           */
  PMU_IRQHandler,       /* 44: Power management unit Handler   */
  DMA_IRQHandler,       /* 45: DMA Handler                     */
  RTC_IRQHandler        /* 46: RTC Interupt Handler            */
};

#ifdef __cplusplus
  }
#endif

/****** End Of File *************************************************/
