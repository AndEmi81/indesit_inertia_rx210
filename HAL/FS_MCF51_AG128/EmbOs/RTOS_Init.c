/**
\defgroup EMBOS_FS_MCF51_AG128 EMBOS for AG128
@{                                      
This module contains 
Purpose : Initializes and handles the hardware for the OS as far
          as required by the OS.


\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy name lastname


\date    12/01/2011
\author  Roberto Fioravanti
\version 1.0.0
@}
\ingroup HAL_FS_MCF51_AG128
*/

/**


\todo 

\date    12/01/2011
\author  Roberto Fioravanti
\version 1.0.0

*/

/**
Header file of the FS_MCF51_AG128_hal.c file

\file    RTOS_Init.c
\ingroup HAL
\date    12/01/2011
\author  Roberto Fioravanti
\version 0.0.84

\par		History
\n 		00.00.84
\n		12/01/2011 
\n		Roberto Fioravanti
\n		changes
\n      Creation
\n		Support for OS version: 3.80i 
*/

/*
Copyright (c) 2009-2010, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/


/* ******** */
/* Includes */
/* ******** */
#include "config.h"

#define __DRIVER_H
#include "derivative.h"

#include "ISR_TMR.h"
/* ****** */
/* Errors */
/* ****** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifndef    OS_ISRHANDLERTABLE_STATIC
  #define  OS_ISRHANDLERTABLE_STATIC          (0)
#endif                    

#ifndef    OS_ISRHANDLERTABLE_REDIRECTION
  #define  OS_ISRHANDLERTABLE_REDIRECTION     (0)
#endif

/****** Begin of configuration settings *******************************/

/*        System Configuration  */

#define ALLOW_NESTED_INTERRUPTS  (0)  // Caution: Nesting interrupts will cause higher stack load on system stack CSTACK




/*       Clock frequency settings */

#ifndef   OS_TICK_FREQ
  #define OS_TICK_FREQ (1000)         /* <<< System tick */
#endif
#define     OS_TICK_MSEC              (1000/OS_TICK_FREQ)




/*       Configuration of communication to OSView */
#ifndef   OS_VIEW_ENABLE            // Global enable of communication
  #define OS_VIEW_ENABLE    (0)     // Default: off, uart is not tested because DEMOEM eval board
#endif                              // has no dsub9 connector


/*       System timer definitions */


#ifndef OS_SYSTEM_TIMER_FREERUNNING
#define OS_SYSTEM_TIMER_FREERUNNING (1)
#endif




/*********************************************************************
*
*       System Configuration
*/




#if defined(OS_DEFAULT_SYSTIMER) || defined (OS_SYSTIMER_3)
#define OS_TPMCLK_EN                   SCGC1_TPM3
#define OS_TPMSC                       TPM3SC   /*  Timer status and Control register*/
#define OS_TPMCNT                      TPM3CNT  /*  Timer counter register */
#define OS_TPMMOD                      TPM3MOD  /*  Timer counter modulo register*/
#define OS_TPMCxSC                     TPM3C0SC /*  Timer channel 0 status and control register */
#define OS_TPMCxV                      TPM3C0V  /*  Timer channel 0 value register*/

#define OS_ALMTPMCxSC                  TPM3C1SC /*  Timer channel 0 status and control register */
#define OS_ALMTPMCxV                   TPM3C1V  /*  Timer channel 0 value register*/

#define SystemIrqVectorNumber          VectorNumber_Vtpm3ch0
#define SystemAlarmVectorNumber        VectorNumber_Vtpm3ch1
#define _OS_IrqHandler88               _OS_SystemIrqhandler
#define _OS_IrqHandler89               _OS_SystemAlarmHandler



#elif   defined (OS_SYSTIMER_1)        /*OS_DEFAULT_SYSTIMER*/

    #define OS_TPMCLK_EN                   SCGC1_FTM1
    #define OS_TPMSC                       FTM1SC   /*  Timer status and Control register */
    #define OS_TPMCNT                      FTM1CNT  /*  Timer counter register */
    #define OS_TPMMOD                      FTM1MOD  /*  Timer counter modulo register */



    #if     defined(OS_SYSTIMER_CH0)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch0
    #define _OS_IrqHandler74               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C0SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C0V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH1)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch1
    #define _OS_IrqHandler75               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C1SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C1V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH2)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch2
    #define _OS_IrqHandler76               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C2SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C2V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH3)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch3
    #define _OS_IrqHandler77               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C3SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C3V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH4)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch4
    #define _OS_IrqHandler78               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C4SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C4V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH5)
    #define SystemIrqVectorNumber          VectorNumber_Vftm1ch5
    #define _OS_IrqHandler79               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM1C5SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM1C5V  /*  Timer channel 0 value register */


    #else
    #error "OS_SYSTIMER_CHx options unsupported for OS_SYSTIMER_1"
    #endif


    #if     defined(OS_ALMTIMER_CH0)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch0
    #define OS_ALMTPMCxSC                  FTM1C0SC /*  Timer channel 0 status and control register */
    #define OS_ALMTPMCxV                   FTM1C0V  /*  Timer channel 0 value register*/
    #define _OS_IrqHandler74               _OS_SystemAlarmHandler


    #elif   defined(OS_ALMTIMER_CH1)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch1
    #define OS_ALMTPMCxSC                  FTM1C1SC /*  Timer channel 1 status and control register */
    #define OS_ALMTPMCxV                   FTM1C1V  /*  Timer channel 1 value register*/
    #define _OS_IrqHandler75               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH2)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch2
    #define OS_ALMTPMCxSC                  FTM1C2SC /*  Timer channel 2 status and control register */
    #define OS_ALMTPMCxV                   FTM1C2V  /*  Timer channel 2 value register*/
    #define _OS_IrqHandler76               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH3)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch3
    #define OS_ALMTPMCxSC                  FTM1C3SC /*  Timer channel 3 status and control register */
    #define OS_ALMTPMCxV                   FTM1C3V  /*  Timer channel 3 value register*/
    #define _OS_IrqHandler77               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH4)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch4
    #define OS_ALMTPMCxSC                  FTM1C4SC /*  Timer channel 4 status and control register */
    #define OS_ALMTPMCxV                   FTM1C4V  /*  Timer channel 4 value register*/
    #define _OS_IrqHandler78               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH5)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm1ch5
    #define OS_ALMTPMCxSC                  FTM1C5SC /*  Timer channel 5 status and control register */
    #define OS_ALMTPMCxV                   FTM1C5V  /*  Timer channel 5 value register*/
    #define _OS_IrqHandler79               _OS_SystemAlarmHandler

    #else
    #error "OS_ALMTIMER_CHx options unsupported for OS_SYSTIMER_1"
    #endif

    #if (SystemAlarmVectorNumber==SystemIrqVectorNumber)
    #error "OS_ALMTIMER_CHx and OS_SYSTIMER_CHx must not be the same"
    #endif


#elif   defined (OS_SYSTIMER_2)        /*OS_DEFAULT_SYSTIMER*/

    #define OS_TPMCLK_EN                   SCGC1_FTM2
    #define OS_TPMSC                       FTM2SC   /*  Timer status and Control register */
    #define OS_TPMCNT                      FTM2CNT  /*  Timer counter register */
    #define OS_TPMMOD                      FTM2MOD  /*  Timer counter modulo register */



    #if     defined(OS_SYSTIMER_CH0)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch0
    #define _OS_IrqHandler81               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C0SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C0V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH1)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch1
    #define _OS_IrqHandler82               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C1SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C1V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH2)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch2
    #define _OS_IrqHandler83               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C2SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C2V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH3)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch3
    #define _OS_IrqHandler84               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C3SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C3V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH4)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch4
    #define _OS_IrqHandler85               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C4SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C4V  /*  Timer channel 0 value register */


    #elif   defined(OS_SYSTIMER_CH5)
    #define SystemIrqVectorNumber          VectorNumber_Vftm2ch5
    #define _OS_IrqHandler85               _OS_SystemIrqhandler
    #define OS_TPMCxSC                     FTM2C5SC /*  Timer channel 0 status and control register */
    #define OS_TPMCxV                      FTM2C5V  /*  Timer channel 0 value register */


    #else
    #error "OS_SYSTIMER_CHx options unsupported for OS_SYSTIMER_1"
    #endif


    #if     defined(OS_ALMTIMER_CH0)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch0
    #define OS_ALMTPMCxSC                  FTM2C0SC /*  Timer channel 0 status and control register */
    #define OS_ALMTPMCxV                   FTM2C0V  /*  Timer channel 0 value register*/
    #define _OS_IrqHandler81               _OS_SystemAlarmHandler


    #elif   defined(OS_ALMTIMER_CH1)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch1
    #define OS_ALMTPMCxSC                  FTM2SC /*  Timer channel 1 status and control register */
    #define OS_ALMTPMCxV                   FTM2C1V  /*  Timer channel 1 value register*/
    #define _OS_IrqHandler82               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH2)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch2
    #define OS_ALMTPMCxSC                  FTM2C2SC /*  Timer channel 2 status and control register */
    #define OS_ALMTPMCxV                   FTM2C2V  /*  Timer channel 2 value register*/
    #define _OS_IrqHandler83               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH3)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch3
    #define OS_ALMTPMCxSC                  FTM2C3SC /*  Timer channel 3 status and control register */
    #define OS_ALMTPMCxV                   FTM2C3V  /*  Timer channel 3 value register*/
    #define _OS_IrqHandler84               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH4)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch4
    #define OS_ALMTPMCxSC                  FTM2C4SC /*  Timer channel 4 status and control register */
    #define OS_ALMTPMCxV                   FTM2C4V  /*  Timer channel 4 value register*/
    #define _OS_IrqHandler85               _OS_SystemAlarmHandler

    #elif   defined(OS_ALMTIMER_CH5)
    #define SystemAlarmVectorNumber        VectorNumber_Vftm2ch5
    #define OS_ALMTPMCxSC                  FTM2C5SC /*  Timer channel 5 status and control register */
    #define OS_ALMTPMCxV                   FTM2C5V  /*  Timer channel 5 value register*/
    #define _OS_IrqHandler86               _OS_SystemAlarmHandler

    #else
    #error "OS_ALMTIMER_CHx options unsupported for OS_SYSTIMER_1"
    #endif

#if (SystemAlarmVectorNumber==SystemIrqVectorNumber)
#error "OS_ALMTIMER_CHx and OS_SYSTIMER_CHx must not be the same"
#endif




#else
#error "invalid or missing OS_SYSTIMER_x option on OS_Config.h."
#endif /*OS_DEFAULT_SYSTIMER*/








/*********************************************************************
*
*       UART settings for OSView
*       If you do not want (or can not due to hardware limitations)
*       to dedicate a UART to OSView, please define it to be -1
*       Currently UART0 and UART1 are supported and the standard
*       setup enables UART 0 per default
*/
#ifndef   OS_UART
  #define OS_UART (1)
#endif

#ifndef   OS_BAUDRATE
  #define OS_BAUDRATE (38400)
#endif

#ifndef   OS_PCLK_UART
  #define OS_PCLK_UART  (OS_FSYS/2)
#endif




/****** End of configuration settings *******************************/


/* Unified Isr Handler Names */

/* handler for vector 64 */


#ifndef Irq_Unmaskable_Handler
#define Irq_Isr_Handler   _OS_IrqDefaultHandler
#endif

/* handler for vector 68 */
#ifndef Dma00_Isr_Handler
#define Dma00_Isr_Handler _OS_IrqDefaultHandler
#endif

/* handler for vector 69 */
#ifndef Dma01_Isr_Handler
#define Dma01_Isr_Handler _OS_IrqDefaultHandler
#endif

/* handler for vector 70 */
#ifndef Dma02_Isr_Handler
#define Dma02_Isr_Handler _OS_IrqDefaultHandler
#endif

/* handler for vector 71 */
#ifndef Dma03_Isr_Handler
#define Dma03_Isr_Handler _OS_IrqDefaultHandler
#endif

/* handler for vector 99 */
#ifndef Iic00_Isr_Handler
#define Iic00_Isr_Handler _OS_IrqDefaultHandler
#endif


#ifndef _OS_IrqHandler96
#define _OS_IrqHandler96  _OS_IrqDefaultHandler
#endif


#ifndef _OS_IrqHandler97
#define _OS_IrqHandler97  _OS_IrqDefaultHandler
#endif


#ifndef _OS_IrqHandler98
#define _OS_IrqHandler98  _OS_IrqDefaultHandler
#endif

#ifndef _OS_IrqHandler101
#define _OS_IrqHandler101  _OS_IrqDefaultHandler
#endif

#ifndef _OS_IrqHandler102
#define _OS_IrqHandler102  _OS_IrqDefaultHandler
#endif

#ifndef _OS_IrqHandler110
#define _OS_IrqHandler110  _OS_IrqDefaultHandler
#endif

#ifndef Irq_MaskableHandler
#define _OS_IrqHandlerSwi6  _OS_IrqDefaultHandler
#endif

/*********************************************************************
*
*       UART settings for OSView
*
*       Automatically generated from configuration settings
*/
#define OS_UART_USED   (OS_VIEW_ENABLE && ((OS_UART == 0) || (OS_UART == 1)))


/*********************************************************************
*
*       Local defines (sfrs used in RTOSInit.c)
*
**********************************************************************
*/

/* INT sources supported by hw */
#define _NUM_INT_SOURCES                51
#define _MAX_INT_OFFSET_                Vieventch3


/* System timer definitions */

#define TPMCxSC_CHxF                    (1 << 7)


/* system uart */
#if (OS_UART == 0) 
  #define SCIxEN     SCGC1_SCI1
  #define SCIxBDH    SCI1BDH 
  #define SCIxBDL    SCI1BDL 
  #define SCIxC1     SCI1C1
  #define SCIxC2     SCI1C2      
  #define SCIxS1     SCI1S1 
  #define SCIxS2     SCI1S2
  #define SCIxC3     SCI1C3
  #define SCIxD      SCI1D 
  #define SCx_RX_VECTORNUMBER           OS_VECTOR(VectorNumber_Vsci1rx)
  #define SCx_TX_VECTORNUMBER           OS_VECTOR(VectorNumber_Vsci1tx)
#elif (OS_UART == 1) 
  #define SCIxEN     SCGC1_SCI2
  #define SCIxBDH    SCI2BDH 
  #define SCIxBDL    SCI2BDL 
  #define SCIxC1     SCI2C1
  #define SCIxC2     SCI2C2      
  #define SCIxS1     SCI2S1 
  #define SCIxS2     SCI2S2
  #define SCIxC3     SCI2C3
  #define SCIxD      SCI2D 
  #define SCx_RX_VECTORNUMBER           OS_VECTOR(VectorNumber_Vsci2rx)
  #define SCx_TX_VECTORNUMBER           OS_VECTOR(VectorNumber_Vsci2tx)
#endif

        
#define SCIxS1_ERRORS_MASK       0x0F  
#define SCIxS1_RDRF              (1 << 5)
#define SCIxS1_TDRE              (1 << 7)
#define SCIxC2_TE                (1 << 3)
#define SCIxC2_RE                (1 << 2)
#define SCIxC2_RIE               (1 << 5)
#define SCIxC2_TIE               (1 << 6)


/* interrupt controller : NMI interrupt to mask*/
#define OS_NMI_MASK             1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */





/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void _OS_IrqDefaultHandler(void);
static void _OS_SystemIrqhandler(void);
static void _OS_SystemAlarmHandler(void);


#ifdef  Irq_MaskableHandler
static void    _OS_IrqHandlerSwi6(void);
#endif

#ifdef  Irq_Unmaskable_Handler
static void    Irq_Isr_Handler(void);
#endif

OS_BOOL OS_IsNMIEnabled(void);


#if OS_UART_USED
void OS_COM_ISR_Rx(void);
void OS_COM_ISR_Tx(void);
#endif /* OS_UART_USED */
  
OS_U32 OS_ConvertCycles2_10us(OS_U32 Cycles);





#if defined(FTM2CH00_IRQ_SUPPORT)
void    _OS_IrqHandler81(void);

#if (defined(OS_SYSTIMER_2) && (defined(OS_ALMTIMER_CH0) || defined(OS_SYSTIMER_CH0))) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_2 is busy. FTM2CH00_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler81(void)
{
    int i=0;
    int status = FTM2C0SC;
    
    i= FTM2CH00_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH00_IRQHandler(void* pDevice, int status);
inline  int FTM2CH00_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH0))  */
#else
#ifndef _OS_IrqHandler81
#define _OS_IrqHandler81  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH00_IRQ_SUPPORT) */


#if defined(FTM2CH01_IRQ_SUPPORT)
void    _OS_IrqHandler82(void);

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH1) || defined(OS_SYSTIMER_CH1)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_2 is busy. FTM2CH01_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler82(void)
{
    int i=0;
    int status = FTM2C1SC;
    
    i= FTM2CH01_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH01_IRQHandler(void* pDevice, int status);
inline  int FTM2CH01_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH1))  */
#else
#ifndef _OS_IrqHandler82
#define _OS_IrqHandler82  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH01_IRQ_SUPPORT) */



#if defined(FTM2CH02_IRQ_SUPPORT)
void    _OS_IrqHandler83(void);

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH2) || defined(OS_SYSTIMER_CH2)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_2 is busy. FTM2CH02_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler83(void)
{
    int i=0;
    int status = FTM2C2SC;
    
    i= FTM2CH02_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH02_IRQHandler(void* pDevice, int status);
inline  int FTM2CH02_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH2))  */
#else
#ifndef _OS_IrqHandler83
#define _OS_IrqHandler83  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH02_IRQ_SUPPORT) */


#if defined(FTM2CH03_IRQ_SUPPORT)
void    _OS_IrqHandler84(void);

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH3) || defined(OS_SYSTIMER_CH3)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_2 is busy. FTM2CH03_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler84(void)
{
    int i=0;
    int status = FTM2C3SC;
    
    i= FTM2CH03_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH03_IRQHandler(void* pDevice, int status);
inline  int FTM2CH03_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}
#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH3))  */

#else
#ifndef _OS_IrqHandler84
#define _OS_IrqHandler84  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH03_IRQ_SUPPORT) */



#if defined(FTM2CH04_IRQ_SUPPORT)
void    _OS_IrqHandler85(void);

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH4) || defined(OS_SYSTIMER_CH4)) 
#error "OS_ALMTIMER_CH4 on OS_SYSTIMER_2 is busy. FTM2CH03_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler85(void)
{
    int i=0;
    int status = FTM2C4SC;
    
    i= FTM2CH04_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH04_IRQHandler(void* pDevice, int status);
inline  int FTM2CH04_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}
#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH4))  */

#else
#ifndef _OS_IrqHandler85
#define _OS_IrqHandler85  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH04_IRQ_SUPPORT) */


#if defined(FTM2CH05_IRQ_SUPPORT)
void    _OS_IrqHandler86(void);

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH5) || defined(OS_SYSTIMER_CH5)) 
#error "OS_ALMTIMER_CH4 on OS_SYSTIMER_2 is busy. FTM2CH03_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler86(void)
{
    int i=0;
    int status = FTM2C5SC;
    
    i= FTM2CH05_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM2CH05_IRQHandler(void* pDevice, int status);
inline  int FTM2CH05_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}
#endif  /* (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH5))  */

#else
#ifndef _OS_IrqHandler86
#define _OS_IrqHandler86  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM2CH04_IRQ_SUPPORT) */








#if defined(FTM1CH00_IRQ_SUPPORT)
void    _OS_IrqHandler74(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0) || defined(OS_SYSTIMER_CH0)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_1 is busy. FTM1CH00_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler74(void)
{
    int i=0;
    int status = FTM1C0SC;
    
    i= FTM1CH00_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM1CH00_IRQHandler(void* pDevice, int status);
inline  int FTM1CH00_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0))  */
#else
#ifndef _OS_IrqHandler74
#define _OS_IrqHandler74  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH00_IRQ_SUPPORT) */


#if defined(FTM1CH01_IRQ_SUPPORT)
void    _OS_IrqHandler75(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH1) || defined(OS_SYSTIMER_CH1)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_1 is busy. FTM1CH01_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler75(void)
{
    int i=0;
    int status = FTM1C1SC;
    
    i= FTM1CH01_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM1CH01_IRQHandler(void* pDevice, int status);
inline  int FTM1CH01_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH1))  */
#else
#ifndef _OS_IrqHandler75
#define _OS_IrqHandler75  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH01_IRQ_SUPPORT) */


#if defined(FTM1CH02_IRQ_SUPPORT)
void    _OS_IrqHandler76(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH2) || defined(OS_SYSTIMER_CH2)) 
#error "OS_ALMTIMER_CH2 on OS_SYSTIMER_1 is busy. FTM1CH02_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler76(void)
{
    int i=0;
    int status = FTM1C2SC;
    
    i= FTM1CH02_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM1CH02_IRQHandler(void* pDevice, int status);
inline  int FTM1CH02_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH2))  */
#else
#ifndef _OS_IrqHandler76
#define _OS_IrqHandler76  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH02_IRQ_SUPPORT) */



#if defined(FTM1CH03_IRQ_SUPPORT)
void    _OS_IrqHandler77(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH3) || defined(OS_SYSTIMER_CH3)) 
#error "OS_ALMTIMER_CH3 on OS_SYSTIMER_1 is busy. FTM1CH03_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler77(void)
{
    int i=0;
    int status = FTM1C3SC;
    
    i= FTM1CH03_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload int FTM1CH03_IRQHandler(void* pDevice, int status);
inline  int FTM1CH03_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH3))  */
#else
#ifndef _OS_IrqHandler77
#define _OS_IrqHandler77  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH03_IRQ_SUPPORT) */



#if defined(FTM1CH04_IRQ_SUPPORT)
void    _OS_IrqHandler78(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH4) || defined(OS_SYSTIMER_CH4)) 
#error "OS_ALMTIMER_CH4 on OS_SYSTIMER_1 is busy. FTM1CH04_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler78(void)
{
    int i=0;
    int status = FTM1C4SC;
    
    i= FTM1CH04_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM1CH04_IRQHandler(void* pDevice, int status);
inline  int FTM1CH04_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH4))  */
#else
#ifndef _OS_IrqHandler78
#define _OS_IrqHandler78  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH04_IRQ_SUPPORT) */



#if defined(FTM1CH05_IRQ_SUPPORT)
void    _OS_IrqHandler79(void);

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH5) || defined(OS_SYSTIMER_CH5)) 
#error "OS_ALMTIMER_CH4 on OS_SYSTIMER_1 is busy. FTM1CH04_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler79(void)
{
    int i=0;
    int status = FTM1C5SC;
    
    i= FTM1CH05_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void FTM1CH05_IRQHandler(void* pDevice, int status);
inline  int FTM1CH05_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH5))  */
#else
#ifndef _OS_IrqHandler79
#define _OS_IrqHandler79  _OS_IrqDefaultHandler
#endif
#endif  /* defined(FTM1CH05_IRQ_SUPPORT) */



#if defined(TPM3CH00_IRQ_SUPPORT)
void    _OS_IrqHandler88(void);

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH0) || defined(OS_SYSTIMER_CH0)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_3 is busy. TPM3CH00_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler88(void)
{
    int i=0;
    int status = TPM3C0SC;
    
    i= TPM3CH00_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void TPM3CH00_IRQHandler(void* pDevice, int status);
inline  int TPM3CH00_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH0))  */
#else
#ifndef _OS_IrqHandler88
#define _OS_IrqHandler88  _OS_IrqDefaultHandler
#endif
#endif  /* defined(TPM3CH00_IRQ_SUPPORT) */


#if defined(TPM3CH01_IRQ_SUPPORT)
void    _OS_IrqHandler89(void);

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH1) || defined(OS_SYSTIMER_CH1)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_3 is busy. TPM3CH00_IRQ_SUPPORT can't be enabled."
#else

void _OS_IrqHandler89(void)
{
    int i=0;
    int status = TPM3C1SC;
    
    i= TPM3CH01_IRQHandler(NULL, status)<<0;
    
    if(!i)
    {
        _OS_IrqDefaultHandler();
    }
}


#pragma overload void TPM3CH01_IRQHandler(void* pDevice, int status);
inline  int TPM3CH01_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
    return 0;
}

#endif  /* (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH0))  */
#else
#ifndef _OS_IrqHandler89
#define _OS_IrqHandler89   _OS_IrqDefaultHandler
#endif
#endif  /* defined(TPM3CH01_IRQ_SUPPORT) */









/* ********* */
/* Variables */
/* ********* */

/* Global */
//extern unsigned long far _SP_INIT[];  

/* Static */                               





#if (OS_ISRHANDLERTABLE_STATIC != 0)
static OS_ISR_HANDLER* const _papISR[_NUM_INT_SOURCES] = 
{
  Irq_Isr_Handler,                               // Handler for interrupt 64 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 65 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 66  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 67  
  Dma00_Isr_Handler,                              // Handler for interrupt 68  
  Dma01_Isr_Handler,                              // Handler for interrupt 69  
  Dma02_Isr_Handler,                              // Handler for interrupt 70  
  Dma03_Isr_Handler,                              // Handler for interrupt 71  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 72  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 73  
  _OS_IrqHandler74,                                 // Handler for interrupt 74  
  _OS_IrqHandler75,                                 // Handler for interrupt 75  
  _OS_IrqHandler76,                                 // Handler for interrupt 76  
  _OS_IrqHandler77,                                 // Handler for interrupt 77  
  _OS_IrqHandler78,                                 // Handler for interrupt 78  
  _OS_IrqHandler79,                                 // Handler for interrupt 79  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 80  
  _OS_IrqHandler81,                                 // Handler for interrupt 81  
  _OS_IrqHandler82,                                 // Handler for interrupt 82  
  _OS_IrqHandler83,                                 // Handler for interrupt 83  
  _OS_IrqHandler84,                                 // Handler for interrupt 84  
  _OS_IrqHandler85,                                 // Handler for interrupt 85  
  _OS_IrqHandler86,                                 // Handler for interrupt 86  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 87  
  _OS_IrqHandler88,                                 // Handler for interrupt 88  
  _OS_IrqHandler89,                                 // Handler for interrupt 89  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 90  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 91  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 92  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 93  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 94  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 95  
  _OS_IrqHandler96,                              // Handler for interrupt 96  

#if (OS_UART_USED && (OS_UART == 0))
  OS_COM_ISR_Rx,                                 // Handler for interrupt 97 UART0 Rx
  OS_COM_ISR_Tx,                                 // Handler for interrupt 98 UART0 Tx 
#else 
  _OS_IrqHandler97,                              // Handler for interrupt 97 UART0 Rx
  _OS_IrqHandler98,                              // Handler for interrupt 98 UART0 Tx 
#endif
  Iic00_Isr_Handler,                              // Handler for interrupt 99  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 100  
  _OS_IrqHandler101,                             // Handler for interrupt 101  
#if (OS_UART_USED && (OS_UART == 1))
  OS_COM_ISR_Rx,                                 // Handler for interrupt 102 UART1 Rx
#else
  _OS_IrqHandler102,                         // Handler for interrupt 102 UART1 Rx
#endif  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 103  
  _OS_IrqHandlerSwi6,                         // Handler for interrupt 104  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 105 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 106  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 107  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 108 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 109  
#if (OS_UART_USED && (OS_UART == 1))
  OS_COM_ISR_Tx,                                 // Handler for interrupt 110 UART1 Tx
#else
  _OS_IrqHandler110,                         // Handler for interrupt 110 UART1 Tx
#endif  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 111
  _OS_IrqDefaultHandler,                         // Handler for interrupt 112
  _OS_IrqDefaultHandler,                         // Handler for interrupt 113
  _OS_IrqDefaultHandler,                         // Handler for interrupt 114
};



#else /*(OS_ISRHANDLERTABLE_STATIC == 0)*/
static OS_ISR_HANDLER* _papISR[_NUM_INT_SOURCES] = 
{
  Irq_Isr_Handler,                               // Handler for interrupt 64 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 65 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 66  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 67  
  Dma00_Isr_Handler,                              // Handler for interrupt 68  
  Dma01_Isr_Handler,                              // Handler for interrupt 69  
  Dma02_Isr_Handler,                              // Handler for interrupt 70  
  Dma03_Isr_Handler,                              // Handler for interrupt 71    
  _OS_IrqDefaultHandler,                         // Handler for interrupt 72  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 73  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 74  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 75  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 76  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 77  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 78  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 79  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 80  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 81  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 82  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 83  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 84  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 85  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 86  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 87  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 88  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 89  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 90  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 91  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 92  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 93  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 94  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 95  
  _OS_IrqHandler96,                              // Handler for interrupt 96

#if (OS_UART_USED && (OS_UART == 0))
  OS_COM_ISR_Rx,                                 // Handler for interrupt 97 UART0 Rx
  OS_COM_ISR_Tx,                                 // Handler for interrupt 98 UART0 Tx 
#else
  _OS_IrqHandler97,                              // Handler for interrupt 97 UART0 Rx
  _OS_IrqHandler98,                              // Handler for interrupt 98 UART0 Tx 
#endif
  Iic00_Isr_Handler,                              // Handler for interrupt 99  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 100  
  _OS_IrqHandler101,                         // Handler for interrupt 101  
#if (OS_UART_USED && (OS_UART == 1))
  OS_COM_ISR_Rx,                                 // Handler for interrupt 102 UART1 Rx
#else
  _OS_IrqHandler102,                         // Handler for interrupt 102 UART1 Rx
#endif  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 103  
  _OS_IrqHandlerSwi6,                             // Handler for interrupt 104  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 105 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 106  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 107  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 108 
  _OS_IrqDefaultHandler,                         // Handler for interrupt 109  
#if (OS_UART_USED && (OS_UART == 1))
  OS_COM_ISR_Tx,                                 // Handler for interrupt 110 UART1 Tx
#else
  _OS_IrqHandler110,                         // Handler for interrupt 110 UART1 Tx
#endif  
  _OS_IrqDefaultHandler,                         // Handler for interrupt 111
  _OS_IrqDefaultHandler,                         // Handler for interrupt 112
  _OS_IrqDefaultHandler,                         // Handler for interrupt 113
  _OS_IrqDefaultHandler,                         // Handler for interrupt 114
};
#endif /*(OS_ISRHANDLERTABLE_STATIC != 0)*/





/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */





/**
Initializzation of the hardware:
\li (timer) required for the OS to run
 
\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Nota 1:
						documentation by Roberto Fioravanti
*/
#define OS_TIMER_RELOAD (OS_PCLK_TIMER/OS_TICK_FREQ)
#if (OS_TIMER_RELOAD >= 0x10000)
  #error "TPM1 timer can not be used, please check configuration"
#endif

void OS_InitHW(void) {
  int i;
  OS_IncDI();
  
  
  
  //
  // Intiliaze interrupt vector table in RAM if required
  //
#if (OS_ISRHANDLERTABLE_STATIC == 0)
  for (i = 0; i < _NUM_INT_SOURCES; +i++) {
    _papISR[i] = (OS_ISR_HANDLER*)_OS_IrqDefaultHandler;
  }
#endif  
  
#if (OS_ISRHANDLERTABLE_REDIRECTION != 0)  
  for (i = 0; i <= _MAX_INT_OFFSET_; i+=4)
  {
    *((unsigned long*)(0x800000 +i)) = (unsigned long)OS_ISR_Handler;
  }
  /* vector base register */
  _SetVectorBaseRegister(#0x800000);
#endif /*(OS_ISRHANDLERTABLE_REDIRECTION != 0)*/
  


  //
  // Do not remove the following code line!
  // Freescale V1 has no configurable int priorities, so we must disable all interrupts
  //
  OS_SetFastIntPriorityLimit(7);
  
    
  //
  // Initialize embOS timer
  //

  OS_EnableISR(OS_VECTOR(SystemIrqVectorNumber), _OS_SystemIrqhandler);   
  OS_EnableISR(OS_VECTOR(SystemAlarmVectorNumber), _OS_SystemAlarmHandler);   
  
#if defined (OS_TICK_HANDLE_EX)  
  OS_TICK_Config(OS_TICK_MSEC, 1);
#endif  
  
  
  OS_TPMCLK_EN = 1; 
  
  OS_TPMSC    = 0x00;               // Stop HW; disable overflow interrupt and set prescaler to 0 
  OS_TPMCxSC  = 0x50;               // Set output compare mode and enable compare interrupt 
  OS_TPMCxV   = (OS_TIMER_RELOAD);  // Initialize appropriate value to the compare/modulo/reload register      
#if OS_SYSTEM_TIMER_FREERUNNING == 0
  OS_TPMMOD   = (OS_TIMER_RELOAD);
#endif 
  OS_TPMCNT   = 0x00;               // Reset HW Counter  
  OS_TPMSC    = 0x08 | OS_PSSYS;          // prescaler, select BusClk and run timer
  
  
  /**/
  INTC_PL6P7 = 0x39;

  OS_COM_Init();
  OS_DecRI();
}


/**
  Idle loop
  
\attention
  This is basically the "core" of the idle loop.
  This core loop can be changed, but:
  The idle loop does not have a stack of its own, therefore no
  functionality should be implemented that relies on the stack
  to be preserved. However, a simple program loop can be programmed
  (like toggeling an output or incrementing a counter)

 
\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Nota 1:
						documentation by Roberto Fioravanti
*/
void OS_Idle(void) {           // Idle loop: No task is ready to exec 
  while (1) {
    //
    // Some NOPs may be necessary for bdm debugger
    // to prevent bdm debugger lock-up
    //
    #ifdef __IAR_SYSTEMS_ICC__
      __no_operation();
      __no_operation();
    #else
      asm(NOP);
      asm(NOP);
    #endif
  }
}
  
/**
System Irq handler 

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
        \par \b Note 2:			
  affected by property OS_SYSTEM_TIMER_FREERUNNING
*/
void _OS_SystemIrqhandler(void) {
  OS_TPMCxSC &= ~TPMCxSC_CHxF;      // Reset compare interrupt request flag, TPM1C0SC: CH0F=0  
#if (OS_SYSTEM_TIMER_FREERUNNING == 1)
  OS_TPMCxV   += (OS_TIMER_RELOAD);
#endif
#if defined (OS_TICK_HANDLE_EX)
    OS_TICK_HandleEx();
#else
    OS_TICK_Handle();
#endif
}


/**
OS system alarm handler
*/
void _OS_SystemAlarmHandler(void) 
{
  OS_ALMTPMCxSC &= ~TPMCxSC_CHxF;   // Reset compare interrupt request flag
  OS_ALARM_Handler();
}

/**
\retval system alarm hw counter value
*/
OS_ALARM_TICK OS_GetHwTime(void)
{
  return OS_ALARM_HW_CNT;
}

/**
\retval last system alarm target value
*/
OS_ALARM_TICK OS_GetHwLastTime(void)
{
  return OS_ALARM_HW_CNT_TARGET_VAL;
}

/**
\retval if system alarm is just raised
*/
int OS_AlarmRaised(void)
{
  return (OS_ALMTPMCxSC & TPMCxSC_CHxF);
}


/**
Disables alla alarms
*/
void OS_DisableAlarms(void)
{
  OS_ALMTPMCxSC &= ~0x40;
}

/**
Arms system alarm
*/
void OS_AlarmCounterArm(OS_ALARM_TICK Tick)
{
  OS_ALMTPMCxSC &= ~TPMCxSC_CHxF;  /* Reset compare interrupt request flag*/
  OS_ALMTPMCxSC = 0x50;
  OS_ALARM_HW_CNT_TARGET_VAL = Tick;  
}



/**
Returns the system time in timer clock cycles.

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
        \par \b Note 2:			
      This routine is required for task-info via OSView or high
      resolution time maesurement functions.
*/
OS_U32 OS_GetTime_Cycles(void) {
  OS_U32 Time, Cycles;
  OS_U16 Cnt;  
  
  
  Time = (OS_U32)OS_GetTime32();
  Cnt  = OS_TPMCNT;
  //  
  // Check if timer interrupt pending ...
  //
  
 
  
  if (OS_TPMCxSC & TPMCxSC_CHxF) {  // Adjust result
    Cnt = OS_TPMCNT;
    Time++;
  }
  else
  {
    Cnt += OS_TIMER_RELOAD;
  }
  Cnt-=OS_TPMCxV;
  Cycles= (OS_TIMER_RELOAD * Time) + Cnt;
    
#if (OS_SYSTEM_TIMER_FREERUNNING == 0)  
  return (OS_TIMER_RELOAD * Time) + Cnt;
#else
  return Cycles;
#endif  
}

                        


/**
Convert cycles into micro-seconds.

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
        \par \b Note 2:			
      If your clock frequency is not a multiple of 1 MHz,
      you may have to modify this routine in order to get proper
      diagonstics.
        \par \b Note 3:      
      This routine is required for profiling or high resolution time
      measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles) {
  //return Cycles/(OS_PCLK_TIMER/1000000);    /*  */
  OS_U32 us;
  
  //OS_ASSERT_NOTOORCYCLES(Cycles);
  
  us = Cycles*0x20*0x20;
  us = (unsigned int)us/(unsigned int)(OS_PCLK_TIMER/0x20/0x20/OS_SYSTEM_PS);
  us *= 1000000/0x20/0x20/0x20/0x20/OS_SYSTEM_PS;
  return Cycles;                           
}

/**
Convert micro-seconds into system cycles.

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
        \par \b Note 2:			
      If your clock frequency is not a multiple of 1 MHz,
      you may have to modify this routine in order to get proper
      diagonstics.
        \par \b Note 3:      
      This routine is required for profiling or high resolution time
      measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_Convert_us2Cycles(OS_U32 us) {
  //return Cycles/(OS_PCLK_TIMER/1000000);    /*  */
  OS_U32 Cycles;

  Cycles = us * (0x20*OS_SYSTEM_PS);
  Cycles = (unsigned int)Cycles/(unsigned int)(1000000/(OS_PCLK_TIMER/0x20/OS_SYSTEM_PS));
  Cycles = Cycles;
  return Cycles;                           
}


/**
Convert Cycles into tens micro seconds.

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
        \par \b Note 2:			
      If your clock frequency is not a multiple of 1 MHz,
      you may have to modify this routine in order to get proper
      diagonstics.
        \par \b Note 3:      
      This routine is required for profiling or high resolution time
      measurement only. It does not affect operation of the OS.
*/

OS_U32 OS_ConvertCycles2_10us(OS_U32 Cycles) {
  Cycles *= (0x20);
  Cycles = (unsigned int)Cycles/(unsigned int)(OS_PCLK_TIMER/0x20);
  Cycles = Cycles * 100000/0x20/0x20;
  return Cycles;                           
}


/*********************************************************************
*
*       Communication for OSView via UART (optional)
*
**********************************************************************
*/

#if OS_UART_USED

#define BAUDDIVIDE (OS_PCLK_UART / (16 * OS_BAUDRATE))


/**
OS USART Rx interrupt handler 

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void OS_COM_ISR_Rx(void) {
  OS_U8 c;
  OS_U8 SciStat;
 // OS_EnterInterrupt(); Segger from 3.82p
  SciStat = SCIxS1;  // Read current status
  c = SCIxD;
  if ((SciStat & SCIxS1_ERRORS_MASK) == 0)  {
    if (SciStat & SCIxS1_RDRF) {  // No Error occured and Rx buffer full?
      OS_OnRx(c);
    }
  }
//  OS_LeaveInterrupt(); Segger from 3.82p
}

/**
OS USART Tx interrupt handler 

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void OS_COM_ISR_Tx(void) {
  if (OS_OnTx()) {             // No more characters to send?  
    SCIxC2 &= ~SCIxC2_TIE;     // Disable Transmission Complete Interrupt Enable (for TC)     
  }
}



/**


\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
			  \par \b Note 2:
			Never call this function directly from your application
*/
void OS_COM_Send1(OS_U8 c) {
  while ((SCIxS1 & SCIxS1_TDRE) == 0){
  } // Is transmit buffer empty?
  
  SCIxC2 |= SCIxC2_TIE; 
  SCIxD   = c;  
}


/**
Initialize UART for OSView

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void OS_COM_Init(void) {       // Initialize UART, enable UART interrupts 
  OS_EnableISR(SCx_RX_VECTORNUMBER, OS_COM_ISR_Rx);
  OS_EnableISR(SCx_TX_VECTORNUMBER, OS_COM_ISR_Tx);
  
  SCIxEN  = 1;					// CLK gate enable
  
  SCIxC1  = 0x00;              // SCI Control Register 1
  SCIxBDH = BAUDDIVIDE >> 8;   // Set baudrate
  SCIxBDL = BAUDDIVIDE & 0xFF;
  SCIxC2  =   SCIxC2_RIE       // Receive Complete Interrupt Enable (for TC)
            | SCIxC2_TE        // Transmitter Enable
            | SCIxC2_RE;       // Receiver Enable
}

#else 

void OS_COM_Init(void) {}
void OS_COM_Send1(OS_U8 c) {
  (void) c;                    // Avoid compiler warning 
  OS_COM_ClearTxActive();      // Let the OS know that Tx is not busy 
}

#endif /*  OS_UART_USED  */

/****** Final check of configuration ********************************/

#ifndef OS_UART_USED
  #error "OS_UART_USED has to be defined"
#endif




/*********************************************************************
*
*       OS interrupt handler and ISR specific functions
*
**********************************************************************
*/
/** 
Disables NMI interrupt via interrupt controller 
*/
void OS_DisableNMI(void)
{
    INTC_IMRL |= OS_NMI_MASK;
}
/** 
Enables NMI interrupt via interrupt controller 
*/
void OS_EnableNMI(void)
{
    INTC_IMRL &= ~OS_NMI_MASK;
}

/** 
Returns if NMI interrupt is enabled
*/
OS_BOOL OS_IsNMIEnabled(void)
{
    return (OS_BOOL)((INTC_IMRL & OS_NMI_MASK)==0);
}

/**
Default

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void _OS_IrqDefaultHandler(void) {
#if OS_DEBUG
  uint8_t error=1;
  OS_DI();
  while (error==1) {
  //
  // The NOPs are necessary for bdm debugger,
  // otherwise the bdm debugger locks up
  //
#ifdef __IAR_SYSTEMS_ICC__
		__no_operation();
		__no_operation();
#else
		asm(NOP);
		asm(NOP);
#endif
  }
#endif
}



/**
Detect reason for IRQ and call correspondig service routine.
OS_irq_handler is called from OS_IRQ_SERVICE function
found in RTOS.s

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static char n;
#if (OS_VERSION_GENERIC == 36203 /*3.62c*/ )

void OS_irq_handler(int ISRIndex) {
  OS_ISR_HANDLER* pISR;

  #if OS_DEBUG
    if (ISRIndex < 64) {
      while(1) {
        // OS_irq_Handler handles peripheral interrupts only
        __no_operation();
        __no_operation();
      }
    }
  #endif
  pISR = *_papISR[ISRIndex - 64];
  pISR();
}
#else /* (OS_VERSION_GENERIC == 36203) */
void OS_irq_handler(int Status) {
  int ISRIndex;
  bool_t nmi = OS_IsNMIEnabled();/* save interrupt controller status */
  ISRIndex = ((Status >> 18) & 0xFF) - 64;  // Extract ISR Index

#if OS_DEBUG
  if ((ISRIndex >= _NUM_INT_SOURCES) || (ISRIndex < 0)) {

    OS_Error(OS_ERR_ISR_INDEX);
    return;
  }
#endif
#if ALLOW_NESTED_INTERRUPTS
  OS_EnterNestableInterrupt();         // Now interrupts may be reenabled. If nesting should be allowed
#else
  OS_EnterInterrupt();                 // Inform OS that interrupt handler is running
#endif
  _papISR[ISRIndex]();                 // Call installed handler
#if ALLOW_NESTED_INTERRUPTS
  OS_LeaveNestableInterrupt();         // Leave nestable interrupt, perform task switch if required
#else
  OS_LeaveInterrupt();                 // Leave interrupt, perform task switch if required  
#endif

  /* restore the saved interrupt controller status */
  if(nmi)
  {
    OS_EnableNMI();
  }  
}

#endif /* (OS_VERSION_GENERIC == 36203) */


/**
Enable ISR

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/

OS_ISR_HANDLER* OS_EnableISR(int ISRIndex, OS_ISR_HANDLER* pISRHandler)
{
    OS_ISR_HANDLER*  pOldHandler;
#if (OS_ISRHANDLERTABLE_STATIC != 0)
    (void) pISRHandler;
#endif
  
#if OS_DEBUG
  if ((ISRIndex >= _NUM_INT_SOURCES) || (ISRIndex < 0)) {
    OS_Error(OS_ERR_ISR_INDEX);
    return NULL;
  }
#endif

  OS_DI();
  pOldHandler = _papISR[ISRIndex];       // Get old handler
#if (OS_ISRHANDLERTABLE_STATIC == 0)
  _papISR[ISRIndex] = pISRHandler;       // Install new handler
#endif
  OS_RestoreI();
  return pOldHandler;
}


/**
Disable ISR

\author 	Roberto Fioravanti
\date		  12/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/

void OS_DisableISR(int ISRIndex) {
#if OS_DEBUG
  if ((ISRIndex >= _NUM_INT_SOURCES) || (ISRIndex < 0)) {
    OS_Error(OS_ERR_ISR_INDEX);
    return;
  }
#endif
  OS_DI();
#if (OS_ISRHANDLERTABLE_STATIC == 0)
  _papISR[ISRIndex] = _OS_IrqDefaultHandler;   // Set default handler
#endif
  OS_RestoreI();
}


/* Local Function */


#ifdef Irq_MaskableHandler
static void _OS_IrqHandlerSwi6(void)
{
    INTC_CFRC = 0x39;
    Irq_MaskableHandler();
}

#endif

#ifdef Irq_Unmaskable_Handler
static void Irq_Isr_Handler(void)
{
    Irq_Unmaskable_Handler();
#ifdef Irq_MaskableHandler    
    INTC_SFRC = 0x39;
#endif    
}
#endif


/* *********** */
/* End of file */
/* *********** */