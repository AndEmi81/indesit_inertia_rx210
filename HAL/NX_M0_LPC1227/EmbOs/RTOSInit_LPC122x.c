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
File    : RTOSInit_LPC122x.c
Purpose : RTOSInit for LPC122x (LPC122x evaluation board). Initializes and
          handles the hardware for the OS as far as required by the OS.
--------  END-OF-HEADER  ---------------------------------------------
*/

#define __DRIVER_H
#include "StandardTypes.h"
#include "RTOS.h"
#ifdef __ICCARM__  // IAR
  #include "exceptions.h"           /* CMSIS compatible irq interface */
#endif
#include "derivative.h"
#include "NX_M0_LPC1227_reg.h"

#include "ISR_TMR.h"

#pragma section=".intvec"
#pragma section="RamVector"


/*********************************************************************
*
*       Configuration
*
*********************************************************************/

/*********************************************************************
*
*       Clock frequency settings
*/
#ifndef   OS_FSYS                   /* CPU main clock frequency     */
  #define OS_FSYS 33000000uL        /* Maximum is 45MHz for LPC122x */
#endif

#ifndef   OS_XTAL_FREQUENCY         /* Frequency of external crystal*/
  #define OS_XTAL_FREQUENCY (12000000uL)
#endif



#ifndef   OS_PCLK_UART              /* Peripheral clock for uart    */
  #define OS_PCLK_UART OS_FSYS      /* May vary from CPU clock      */
#endif                              /* depending on CPU             */

#ifndef   OS_TICK_FREQ
  #define OS_TICK_FREQ (1000)
#endif

#define OS_Systick SysTick_Handler  /* Map OS systick handler to CMSIS compatible handler name */

/*********************************************************************
*
*       Configuration of communication to OSView
*/
#ifndef   OS_VIEW_ENABLE            // Global enable of communication
  #define OS_VIEW_ENABLE    (0)     // Default: on
#endif

#ifndef   OS_VIEW_USE_UART          // If set, UART will be used for communication
  #define OS_VIEW_USE_UART  (0)     // Default: 0 => memory access is used
#endif                              // if OS_VIEW_ENABLE is on

/*********************************************************************
*
*       UART settings for OSView
*       If you do not want (or can not due to hardware limitations)
*       to dedicate a UART to embOSView, please define it to be -1
*       Currently the standard code enables OS_UART 0 per default
*       when OS_VIEW_USE_UART is set
*/
#ifndef   OS_UART
  #define OS_UART (0)
#endif

#ifndef   OS_BAUDRATE
  #define OS_BAUDRATE (38400)
#endif

/*********************************************************************
*
*       Other configuration settings
*/
#ifndef   OS_DISABLE_WATCHDOG
  #define OS_DISABLE_WATCHDOG  (1)
#endif

/****** End of configuration settings *******************************/

#ifndef   DEBUG
  #define DEBUG (0)
#endif

/****** Calculate PLL settings **************************************/

#define PSEL     (1)   // Fixed, no additional Multiplier in the loop
#define AHB_DIV  (4)   // Divider for PLL output -> CPU, fixed
#define PLL_MUL ((OS_FSYS * AHB_DIV) / OS_XTAL_FREQUENCY)

#if (PLL_MUL > 32)
  #error "PLL can not be initialized with current settings."
#endif

/*********************************************************************
*
*       JLINKMEM and UART settings for OSView
*
*       Automatically generated from configuration settings
*/
#define OS_USE_JLINKMEM   (OS_VIEW_ENABLE && (OS_VIEW_USE_UART == 0))

#define OS_UART_USED      (OS_VIEW_ENABLE && (OS_VIEW_USE_UART != 0) && (OS_UART == 0))

#if OS_USE_JLINKMEM
  #include "JLINKMEM.h"
#endif

/*********************************************************************
*
*       Local defines (sfrs used in RTOSInit.c)
*
**********************************************************************
*/



#define    SYSTICK_VECTOR_ID                  15     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC12xx Specific Interrupt Numbers *******************************************************/
#define    WAKEUP0_VECTOR_ID                  16
#define    WAKEUP1_VECTOR_ID                  17
#define    WAKEUP2_VECTOR_ID                  18
#define    WAKEUP3_VECTOR_ID                  19
#define    WAKEUP4_VECTOR_ID                  20
#define    WAKEUP5_VECTOR_ID                  21        
#define    WAKEUP6_VECTOR_ID                  22        
#define    WAKEUP7_VECTOR_ID                  23        
#define    WAKEUP8_VECTOR_ID                  24        
#define    WAKEUP9_VECTOR_ID                  25        
#define    WAKEUP10_VECTOR_ID                 26       
#define    WAKEUP11_VECTOR_ID                 27              
#define    I2C_VECTOR_ID                      28       /*!< I2C Interrupt                                    */
#define    TIMER_16_0_VECTOR_ID               29       /*!< 16-bit Timer0 Interrupt                          */
#define    TIMER_16_1_VECTOR_ID               30       /*!< 16-bit Timer1 Interrupt                          */
#define    TIMER_32_0_VECTOR_ID               31       /*!< 32-bit Timer0 Interrupt                          */
#define    TIMER_32_1_VECTOR_ID               32       /*!< 32-bit Timer1 Interrupt                          */
#define    SSP_VECTOR_ID                      33       /*!< SSP Interrupt                                    */
#define    UART0_VECTOR_ID                    34       /*!< UART0 Interrupt                                  */
#define    UART1_VECTOR_ID                    35       /*!< UART1 Interrupt                                  */
#define    CMP_VECTOR_ID                      36       /*!< Comparator Interrupt                             */
#define    ADC_VECTOR_ID                      37       /*!< A/D Converter Interrupt                          */
#define    WDT_VECTOR_ID                      38       /*!< Watchdog timer Interrupt                         */  
#define    BOD_VECTOR_ID                      39       /*!< Brown Out Detect(BOD) Interrupt                  */
#define    FLASH_VECTOR_ID                    40       /*!< Flash Interrupt                                  */
#define    EINT0_VECTOR_ID                    41       /*!< External Interrupt 0 Interrupt                   */
#define    EINT1_VECTOR_ID                    42       /*!< External Interrupt 1 Interrupt                   */
#define    EINT2_VECTOR_ID                    43       /*!< External Interrupt 2 Interrupt                   */
#define    PMU_VECTOR_ID                      44       /*!< PMU Interrupt                                    */
#define    DMA_VECTOR_ID                      45       /*!< DMA Interrupt                                    */
#define    RTC_VECTOR_ID                      46       /*!< RTC Interrupt                                    */
#define    EDM_VECTOR_ID                      47       /*!< EDT Interrupt                                    */


typedef enum VectorIDs
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  SYSTICK_VECTOR                  = 15,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC12xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_VECTOR                  ,
  WAKEUP1_VECTOR                  ,
  WAKEUP2_VECTOR                  ,
  WAKEUP3_VECTOR                  ,
  WAKEUP4_VECTOR                  ,
  WAKEUP5_VECTOR                  ,        
  WAKEUP6_VECTOR                  ,        
  WAKEUP7_VECTOR                  ,        
  WAKEUP8_VECTOR                  ,        
  WAKEUP9_VECTOR                  ,        
  WAKEUP10_VECTOR                 ,       
  WAKEUP11_VECTOR                 ,              
  I2C_VECTOR                      ,       /*!< I2C Interrupt                                    */
  TIMER_16_0_VECTOR               ,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_VECTOR               ,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_VECTOR               ,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_VECTOR               ,       /*!< 32-bit Timer1 Interrupt                          */
  SSP_VECTOR                      ,       /*!< SSP Interrupt                                    */
  UART0_VECTOR                    ,       /*!< UART0 Interrupt                                  */
  UART1_VECTOR                    ,       /*!< UART1 Interrupt                                  */
  CMP_VECTOR                      ,       /*!< Comparator Interrupt                             */
  ADC_VECTOR                      ,       /*!< A/D Converter Interrupt                          */
  WDT_VECTOR                      ,       /*!< Watchdog timer Interrupt                         */  
  BOD_VECTOR                      ,       /*!< Brown Out Detect(BOD) Interrupt                  */
  FLASH_VECTOR                    ,       /*!< Flash Interrupt                                  */
  EINT0_VECTOR                    ,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_VECTOR                    ,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_VECTOR                    ,       /*!< External Interrupt 2 Interrupt                   */
  PMU_VECTOR                      ,       /*!< PMU Interrupt                                    */
  DMA_VECTOR                      ,       /*!< DMA Interrupt                                    */
  RTC_VECTOR                      ,       /*!< RTC Interrupt                                    */
  EDM_VECTOR                      ,       /*!< EDT Interrupt                                    */
} VectorID_Type;

/****** Watchdog ****************************************************/

#define WDT_BASE_ADDR (0x40004000)
#define WDT_MOD       (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x000))
#define WDT_FEED      (*(volatile OS_U32*)(WDT_BASE_ADDR + 0x008))

/****** Alarms ******************************************************/

#if defined (OS_SYSTIMER_0)
/* 16_0 specific */

#define ALM_ISR_ID                  (TIMER_16_0_VECTOR_ID)

#if     defined (OS_ALMTIMER_CH0)
#define ALM_IR_MSK                  (1U<<0)
#define ALM_IE_MSK                  (1U<<(3*0))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B0->MR0
#elif   defined (OS_ALMTIMER_CH1)
#define ALM_IR_MSK                  (1U<<(1))
#define ALM_IE_MSK                  (1U<<(3*1))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B0->MR1
#elif   defined (OS_ALMTIMER_CH2)
#define ALM_IR_MSK                  (1U<<(2))
#define ALM_IE_MSK                  (1U<<(3*2))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B0->MR2
#elif   defined (OS_ALMTIMER_CH3)
#define ALM_IR_MSK                  (1U<<(3))
#define ALM_IE_MSK                  (1U<<(3*3))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B0->MR3
#else
#error "Currently supported values for OS_SYSTIMER_0 are OS_ALMTIMER_CH0, OS_ALMTIMER_CH1
OS_ALMTIMER_CH2, OS_ALMTIMER_CH3"
#endif


#define ALM_CLK_BIT                 (7)

#define OS_ALMCT_IR                 LPC_TMR16B0->IR     // Interrupt Register.
#define OS_ALARM_HW_CNT             LPC_TMR16B0->TC     // Counter Register.

#define OS_ALARM_MATCH_CTRL         LPC_TMR16B0->MCR    // Match Control Register.
#define OS_ALARM_MATCH_CTRL_MSK     ALM_IE_MSK

#define CT_PR                       LPC_TMR16B0->PR     // Prescale Register.
#define CT_PC                       LPC_TMR16B0->PC     // Prescale Counter.
#define CT_TCR                      LPC_TMR16B0->TCR    // Timer Control Register.

#elif defined (OS_SYSTIMER_1)

/* 16_1 specific */
#define ALM_ISR_ID                  (TIMER_16_1_VECTOR_ID)


#if     defined (OS_ALMTIMER_CH0)
#define ALM_IR_MSK                  (1U<<0)
#define ALM_IE_MSK                  (1U<<(3*0))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B1->MR0
#elif   defined (OS_ALMTIMER_CH1)
#define ALM_IR_MSK                  (1U<<(1))
#define ALM_IE_MSK                  (1U<<(3*1))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B1->MR1
#elif   defined (OS_ALMTIMER_CH2)
#define ALM_IR_MSK                  (1U<<(2))
#define ALM_IE_MSK                  (1U<<(3*2))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B1->MR2
#elif   defined (OS_ALMTIMER_CH3)
#define ALM_IR_MSK                  (1U<<(3))
#define ALM_IE_MSK                  (1U<<(3*3))
#define OS_ALARM_HW_CNT_MATCH_VAL   LPC_TMR16B1->MR3
#else
#error "Currently supported values for OS_SYSTIMER_1 are OS_ALMTIMER_CH0, OS_ALMTIMER_CH1
OS_ALMTIMER_CH2, OS_ALMTIMER_CH3"
#endif

#define ALM_CLK_BIT                 (8)
#define OS_ALMCT_IR                 LPC_TMR16B1->IR     // Interrupt Register.
#define OS_ALARM_HW_CNT             LPC_TMR16B1->TC     // Counter Register.

#define OS_ALARM_MATCH_CTRL         LPC_TMR16B1->MCR    // Match Control Register.
#define OS_ALARM_MATCH_CTRL_MSK     ALM_IE_MSK

#define CT_PR                       LPC_TMR16B1->PR     // Prescale Register.
#define CT_PC                       LPC_TMR16B1->PC     // Prescale Counter.
#define CT_TCR                      LPC_TMR16B1->TCR    // Timer Control Register.
#else
#error "Alarm is supported values for OS_SYSTIMER_0 and OS_SYSTIMER_1"
#endif


/*general*/
#define ALM_CT_CLK                  (1<<ALM_CLK_BIT)    // Mask for clock gate


/****** System control **********************************************/

#define SYS_BASE_ADDR (0x40048000)
#define SYSPLLCTRL    (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x008))  // System PLL control
#define SYSPLLSTAT    (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x00C))  // System PLL status
#define SYSPLLCLKSEL  (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x040))  // System PLL clock source select
#define SYSPLLCLKUEN  (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x044))  // System PLL clock source update enable
#define MAINCLKSEL    (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x070))  // Main clock source select
#define MAINCLKUEN    (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x074))  // Main clock source update enable
#define SYSAHBCLKDIV  (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x078))  // AHB clock divider

#define SYSOSCCTRL    (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x020))  // System oscillator control
#define SYSAHBCLKCTRL (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x080))
#define PDRUNCFG      (*(volatile OS_U32*)(SYS_BASE_ADDR + 0x238))  // Power-down control register

#define SYS_CLK_WDT_BIT (12)

/****** NVIC, interrupt control *************************************/

#define NVIC_SYS_HND_CTRL_MEM         (0x00010000uL) // Mem manage fault enable
#define NVIC_SYS_HND_CTRL_BUS         (0x00020000uL) // Bus fault enable
#define NVIC_SYS_HND_CTRL_USAGE       (0x00040000uL) // Usage fault enable

#define NVIC_PRIOBASE_ADDR            (0xE000E400)
#define NVIC_ENABLE_ADDR              (0xE000E100)
#define NVIC_DISABLE_ADDR             (0xE000E180)
#define NVIC_VTOREG_ADDR              (0xE000ED08)

#define NUM_INTERRUPTS                (16+31)

/*****  Interrupt ID numbers ****************************************/

#define ISR_ID_SYSTICK                (15)               // System Tick

#define OS_ISR_ID_TICK                ISR_ID_SYSTICK     // We use Sys-Timer
#define OS_ISR_TICK_PRIO              (2uL << 6)         // One priority level higher (2) than PendSV priority (3)

/****** SystemTimer *************************************************/

#define SYS_TICK_BASE_ADDR            (0xE000E010)
#define SYS_TICK_CONTROL              (*(volatile OS_U32*)(SYS_TICK_BASE_ADDR + 0x00))
#define SYS_TICK_RELOAD               (*(volatile OS_U32*)(SYS_TICK_BASE_ADDR + 0x04))
#define SYS_TICK_VALUE                (*(volatile OS_U32*)(SYS_TICK_BASE_ADDR + 0x08))
#define SYS_TICK_CALIBRATION          (*(volatile OS_U32*)(SYS_TICK_BASE_ADDR + 0x0C))
#define SYS_TICK_ENABLE_BIT           (0)
#define SYS_TICK_INT_ENABLE_BIT       (1)
#define SYS_TICK_CLK_SOURCE_BIT       (2)

/****** System interrupt control ************************************/

#define SYSPRI1_ADDR                  (0xE000ED18)
#define SYS_INT_CTRL_STATE            (*(volatile OS_U32*)(0xE000ED04))
#define SYS_PENDSTSET                 26

/****** UART ********************************************************/

#if OS_UART_USED
#define IOCON_BASE_ADDR               (0x40044000)
#define IOCON_PIO0_1                  (*(volatile OS_U32*)(IOCON_BASE_ADDR + 0x048))
#define IOCON_PIO0_2                  (*(volatile OS_U32*)(IOCON_BASE_ADDR + 0x04C))

#define USART_BASE_ADDR               (0x40008000)
#define U0RBR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x000)) // Receiver Buffer Register. Contains the next received character to be read.
#define U0THR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x000)) // Transmit Holding Register. The next character to be transmitted is written here.
#define U0DLL                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x000)) // Divisor Latch LSB. Least significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider.
#define U0DLM                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x004)) // Divisor Latch MSB. Most significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider.
#define U0IER                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x004)) // Interrupt Enable Register. Contains individual interrupt enable bits for the 7 potential UART interrupts.
#define U0FCR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x008)) // FIFO Control Register. Controls UART FIFO usage and modes.
#define U0IIR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x008)) // Interrupt ID Register. Identifies which interrupt(s) are pending. 0x01 -
#define U0LCR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x00C)) // Line Control Register. Contains controls for frame formatting and break generation.
#define U0LSR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x014)) // Line Status Register. Contains flags for transmit and receive status, including line errors.
#define U0MSR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x018)) // Modem status register 0x00 -
#define U0FDR                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x028)) // Fractional divider
#define U0TER                         (*(volatile OS_U32*)(USART_BASE_ADDR + 0x030)) // Transmit Enable Register. Turns off UART transmitter for use with software flow control.
#define IOCON_RXD                     IOCON_PIO0_1
#define IOCON_TXD                     IOCON_PIO0_2
#define UARTCLKDIV                    (*(volatile OS_U32*)(0x40048098))

#define OS_ISR_ID_UART                (18 + 16)          // Uart0 interrupt ID
#define OS_ISR_UART_PRIO              (2uL << 6)         // Uart interrupt  priority 2
#define OS_UART_CLK_BIT               (12)               // AHB clock control bit for UART0
#endif

/****** OS timer configuration **************************************/
#define OS_TICK_MSEC              (1000/OS_TICK_FREQ)
#define OS_TIMER_RELOAD           (OS_PCLK_TIMER / OS_TICK_FREQ - 1)
#if (OS_TIMER_RELOAD >= 0x100000000)
  #error "Systick can not be used, please check configuration"
#endif

/****** MAP UART initialization function ****************************/

#if  (OS_UART_USED || OS_USE_JLINKMEM)
  #define OS_COM_INIT() OS_COM_Init()
#else
  #define OS_COM_INIT()
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

#if OS_USE_JLINKMEM
  // Size of the communication buffer for JLINKMEM
const OS_U32 OS_JLINKMEM_BufferSize = 32;
#else
const OS_U32 OS_JLINKMEM_BufferSize = 0;     // Communication not used
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

#if defined(Iic00_Isr_Handler)
void I2C0_IRQHandler (void)
{
  OS_EnterNestableInterrupt();
  Iic00_Isr_Handler();
  OS_LeaveNestableInterrupt();
}
#endif


void PIO1_IRQHandler(void)
{
  OS_EnterNestableInterrupt();

#if defined(Irq_Isr_Handler)
    Irq_Isr_Handler();
#endif
  
  OS_LeaveNestableInterrupt();
}



/*********************************************************************
*
*       CT32B0
*
**********************************************************************
*/

#if defined(CT32B0CH0_IRQ_SUPPORT)


#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_2 is busy. CT32B0CH0_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B0CH0_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH0) && defined(OS_SYSTIMER_2)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B0CH1_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_2 is busy. CT32B0CH1_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B0CH1_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH1) && defined(OS_SYSTIMER_2)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B0CH2_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH2)) 
#error "OS_ALMTIMER_CH2 on OS_SYSTIMER_2 is busy. CT32B0CH2_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B0CH2_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH2) && defined(OS_SYSTIMER_2)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B0CH3_IRQ_SUPPORT)


#if (defined(OS_SYSTIMER_2) && defined(OS_ALMTIMER_CH3)) 
#error "OS_ALMTIMER_CH3 on OS_SYSTIMER_2 is busy. CT32B0CH3_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B0CH3_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH3) && defined(OS_SYSTIMER_2)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}


#if defined(CT32B0CH0_IRQ_SUPPORT) || defined(CT32B0CH1_IRQ_SUPPORT) || \
    defined(CT32B0CH2_IRQ_SUPPORT) || defined(CT32B0CH3_IRQ_SUPPORT) || \
    (defined(OS_SYSTIMER_2) && (defined(OS_ALMTIMER_CH0)    \
                                ||defined(OS_ALMTIMER_CH1)  \
                                ||defined(OS_ALMTIMER_CH2)  \
                                ||defined(OS_ALMTIMER_CH3)))


//#if defined(TMR0_ISR_ENABLE) || defined(TMR1_ISR_ENABLE) || defined(TMR2_ISR_ENABLE)
void CT32B0_IRQHandler(void)
{
    int i=0;
    int status = LPC_TMR32B0->IR;
    
    OS_EnterInterrupt();

    i |= CT32B0CH0_IRQHandler(NULL, status)<<0;
    i |= CT32B0CH1_IRQHandler(NULL, status)<<1;
    i |= CT32B0CH2_IRQHandler(NULL, status)<<2;
    i |= CT32B0CH3_IRQHandler(NULL, status)<<3;
    
    if(!i)
      while(1);
    
    OS_LeaveInterrupt();
}
#endif


/*********************************************************************
*
*       CT32B1
*
**********************************************************************
*/
#if defined(CT32B1CH0_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_3 is busy. TMR12_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B1CH0_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH0) && defined(OS_SYSTIMER_3)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B1CH1_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_3 is busy. TMR13_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B1CH1_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH1) && defined(OS_SYSTIMER_3)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B1CH2_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH2)) 
#error "OS_ALMTIMER_CH2 on OS_SYSTIMER_3 is busy. CT32B1CH2_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B1CH2_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH2) && defined(OS_SYSTIMER_3)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT32B1CH3_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_3) && defined(OS_ALMTIMER_CH3)) 
#error "OS_ALMTIMER_CH3 on OS_SYSTIMER_3 is busy. CT32B1CH3_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT32B1CH3_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH3) && defined(OS_SYSTIMER_3)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}


#if defined(CT32B1CH0_IRQ_SUPPORT) || defined(CT32B1CH1_IRQ_SUPPORT) || \
    defined(CT32B1CH2_IRQ_SUPPORT) || defined(CT32B1CH3_IRQ_SUPPORT) || \
    (defined(OS_SYSTIMER_3) && (defined(OS_ALMTIMER_CH0)    \
                                ||defined(OS_ALMTIMER_CH1)  \
                                ||defined(OS_ALMTIMER_CH2)  \
                                ||defined(OS_ALMTIMER_CH3)))

//#if defined(TMR3_ISR_ENABLE) || defined(TMR4_ISR_ENABLE) || defined(TMR5_ISR_ENABLE)
void CT32B1_IRQHandler(void)
{
    int i=0;
    int status = LPC_TMR32B1->IR;
    
    OS_EnterInterrupt();

    i |= CT32B1CH0_IRQHandler(NULL, status)<<0;
    i |= CT32B1CH1_IRQHandler(NULL, status)<<1;
    i |= CT32B1CH2_IRQHandler(NULL, status)<<2;
    i |= CT32B1CH3_IRQHandler(NULL, status)<<3;
    
    if(!i)
      while(1);
    
    OS_LeaveInterrupt();
}
#endif


#define     UART_IsRDA(status)      (((status)&4UL))
#define     UART_IsTDC(status)      (((status)&0x02))
#define     U0_IIR                  (*(volatile OS_U32*)(0x40008008)) // Interrupt ID Register. Identifies which interrupt(s) are pending. 0x01 -
#define     U1_IIR                  (*(volatile OS_U32*)(0x4000C008)) // Interrupt ID Register. Identifies which interrupt(s) are pending. 0x01 -
/* Receive Data Available */
extern WEAK int UART00_IRQ_RDA_Handler(void);
WEAK int UART00_IRQ_RDA_Handler(void)      
{ 
    return 0; 
}
/* Transmit Data Completed */
extern WEAK int UART00_IRQ_TDC_Handler(void);
WEAK int UART00_IRQ_TDC_Handler(void)      
{ 
    return 0; 
}


#if defined(UART00_ISR_ENABLE)
void UART0_IRQHandler    (void)
{
    int i=0;
    uint32_t status = 0;
    
    OS_EnterInterrupt();
    status = U0_IIR;
    if(UART_IsRDA(status))
    {
        i |= UART00_IRQ_RDA_Handler()<<0;
    }
    if(UART_IsTDC(status))
    {
        i |= UART00_IRQ_TDC_Handler()<<1;
    }
    
    if(!i)
      while(1);
    
    OS_LeaveInterrupt();
}
#endif /* defined(UART00_ISR_ENABLE) */

extern WEAK int UART01_IRQHandler(uint32_t status);
WEAK int UART01_IRQHandler(uint32_t status)
{ 
    (void) status;
    return 0; 
}

#if defined(UART01_ISR_ENABLE)
void UART1_IRQHandler    (void)
{
    int i=0;
    uint32_t status = 0;
    
    OS_EnterInterrupt();
    status = U1_IIR;
    if(UART_IsRDA(status))
    {
        i |= UART01_IRQ_RDA_Handler()<<0;
    }
    if(UART_IsTDC(status))
    {
        i |= UART01_IRQ_TDC_Handler()<<1;
    }
    
    if(!i)
      while(1);
    
    OS_LeaveInterrupt();
}
#endif /* defined(UART01_ISR_ENABLE) */


/*********************************************************************
*
*       CT16B0
*
**********************************************************************
*/

#if defined(CT16B0CH0_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH0 on OS_SYSTIMER_0 is busy. CT16B0CH0_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B0CH0_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH0) && defined(OS_SYSTIMER_0)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B0CH1_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_0 is busy. CT16B0CH1_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B0CH1_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH1) && defined(OS_SYSTIMER_0)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B0CH2_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH2)) 
#error "OS_ALMTIMER_CH2 on OS_SYSTIMER_0 is busy. CT16B0CH2_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B0CH2_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH2) && defined(OS_SYSTIMER_0)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B0CH3_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH3)) 
#error "OS_ALMTIMER_CH3 on OS_SYSTIMER_0 is busy. CT16B0CH3_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B0CH3_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH3) && defined(OS_SYSTIMER_0)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}


#if defined(CT16B0CH0_IRQ_SUPPORT) || defined(CT16B0CH1_IRQ_SUPPORT) || \
    defined(CT16B0CH2_IRQ_SUPPORT) || defined(CT16B0CH3_IRQ_SUPPORT) || \
    (defined(OS_SYSTIMER_0) && (defined(OS_ALMTIMER_CH0)    \
                                ||defined(OS_ALMTIMER_CH1)  \
                                ||defined(OS_ALMTIMER_CH2)  \
                                ||defined(OS_ALMTIMER_CH3)))
void CT16B0_IRQHandler(void)
{
    int i=0;
    int status = LPC_TMR16B0->IR;
    
    OS_EnterNestableInterrupt();
    
    i |= CT16B0CH0_IRQHandler(NULL, status)<<0;   /* CH0 */
    i |= CT16B0CH1_IRQHandler(NULL, status)<<1;   /* CH1 */
    i |= CT16B0CH2_IRQHandler(NULL, status)<<2;   /* CH2 */
    i |= CT16B0CH3_IRQHandler(NULL, status)<<3;   /* CH3 */
    
    if(!i)
    {
        while(1);
    }
    
    OS_LeaveNestableInterrupt();
}
#endif




/*********************************************************************
*
*       CT16B1
*
**********************************************************************
*/

#if defined(CT16B1CH0_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_1 is busy. CT16B1CH0_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B1CH0_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH0) && defined(OS_SYSTIMER_1)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B1CH1_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH0)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_1 is busy. CT16B1CH1_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B1CH1_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH1) && defined(OS_SYSTIMER_1)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B1CH2_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH2)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_1 is busy. CT16B1CH1_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B1CH2_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH2) && defined(OS_SYSTIMER_1)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}


#if defined(CT16B1CH3_IRQ_SUPPORT)

#if (defined(OS_SYSTIMER_1) && defined(OS_ALMTIMER_CH3)) 
#error "OS_ALMTIMER_CH1 on OS_SYSTIMER_1 is busy. CT16B1CH1_IRQ_SUPPORT can't be enabled."
#endif
#endif

inline WEAK int CT16B1CH3_IRQHandler(void* pDevice, int status)
{ 
    (void) pDevice;
    (void) status;
#if defined(OS_ALMTIMER_CH3) && defined(OS_SYSTIMER_1)
    return _OS_SystemAlarmHandler();
#else
    return 0;
#endif
}

#if defined(CT16B1CH0_IRQ_SUPPORT) || defined(CT16B1CH1_IRQ_SUPPORT) || \
   defined(CT16B1CH2_IRQ_SUPPORT) || defined(CT16B1CH3_IRQ_SUPPORT) || \
    (defined(OS_SYSTIMER_1) && ( defined(OS_ALMTIMER_CH0)   \
                                ||defined(OS_ALMTIMER_CH1)  \
                                ||defined(OS_ALMTIMER_CH2)  \
                                ||defined(OS_ALMTIMER_CH3)))
void CT16B1_IRQHandler(void)
{
    int i=0;
    int status = LPC_TMR16B1->IR;
    
    OS_EnterNestableInterrupt();

    i |= CT16B1CH0_IRQHandler(NULL, status)<<0;  /* CH0 */
    i |= CT16B1CH1_IRQHandler(NULL, status)<<1;  /* CH1 */
    i |= CT16B1CH2_IRQHandler(NULL, status)<<2;  /* CH2 */
    i |= CT16B1CH3_IRQHandler(NULL, status)<<3;  /* CH3 */
    
    if(!i)
    {
        while(1);
    }
    
    OS_LeaveNestableInterrupt();
}
#endif






#if defined(OCRTC_IsrHandler)
//extern WEAK void RTC_IsrHandler        (void);
void RTC_IRQHandler(void)
{
    OS_EnterNestableInterrupt();
    OCRTC_IsrHandler();
    OS_LeaveNestableInterrupt();
}
#endif

#pragma diag_suppress=Pe177
static void _InitPLL(void) {
  //
  // Enable main oscillator
  //
  if (OS_XTAL_FREQUENCY > 20000000uL) { // Frequency > 20MHz
    SYSOSCCTRL = 0x02;                  // Select high frequency range
  } else {
    SYSOSCCTRL = 0x00;
  }
  //
  // Ensure Sys-Oscillator and internal RC oscillator are powered up
  //
  PDRUNCFG &= ~((1uL << 1) | (1uL << 5));  // Clear IRC_PD and SYSOSC_PD to power oscillators
  //
  // Switch to internal RC oscillator
  //
  MAINCLKUEN = 0;   // Enable update of clock source
  MAINCLKSEL = 0;   // Select IRC oscillator as clock source
  MAINCLKUEN = 1;   // Update clock source
  //
  // Configure PLL
  //
  PDRUNCFG    |= (1uL << 7);  // Power down PLL
  SYSPLLCLKUEN = 0;           // Enable update of PLL clock source
  SYSPLLCLKSEL = 1;           // Select system oscillator as clock source
  SYSPLLCLKUEN = 1;           // Update PLL clock source
  SYSPLLCTRL   = (PLL_MUL-1 << 0)  // b4..0: Multiplier
               | (PSEL << 5)       // b6..5: Post divider
               ;
  PDRUNCFG    &= ~(1uL << 7); // Power up PLL
  SYSAHBCLKDIV = AHB_DIV;     // AHB clock divider = 4;
  while (SYSPLLSTAT == 0x00) {
    // Wait until PLL is locked
  }
  //
  // Switch to PLL clock
  //
  MAINCLKUEN = 0;   // Enable update of clock source
  MAINCLKSEL = 3;   // Select PLL as clock source
  MAINCLKUEN = 1;   // Update clock source
}
#pragma diag_remark=Pe177

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_Systick
*
* Function description
*   This is the code that gets called when the processor receives a
*   _SysTick exception. SysTick is used as OS timer tick.
*
* NOTES:
*   (1) It has to be inserted in the interrupt vector table, if RAM
*       vectors are not used. Therefore is is declared public
*/
void OS_Systick(void) {
  OS_EnterNestableInterrupt();
  OS_TICK_HandleEx();
#if OS_USE_JLINKMEM
  JLINKMEM_Process();
#endif
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       OS_InitHW()
*
*       Initialize the hardware (timer) required for the OS to run.
*       May be modified, if an other timer should be used
*/
void OS_InitHW(void) {
  OS_IncDI();

#if OS_DISABLE_WATCHDOG
  //
  // Initially disable Watchdog
  //
  WDT_MOD  = 0;
  WDT_FEED = 0xAA;
  WDT_FEED = 0x55;
  SYSAHBCLKCTRL &= ~(1uL << SYS_CLK_WDT_BIT);
#endif
  
  /* copy vector table in ram 
  NB it copies only __section_size("RamVector") bytes as defined in .icf file altough .intvec section were greater or smaller
  */
  OS_MEMCPY((void*)__sfb("RamVector"), (void*)__sfb(".intvec"), __section_size("RamVector"));
  /* enable vector table in ram */
  
  (*(volatile OS_U32*)(0x40048000))=1;
  __DSB();
  //
  // Initialize OS timer, clock soure = core clock
  //
  SYS_TICK_RELOAD  = OS_TIMER_RELOAD;
  SYS_TICK_CONTROL = (1uL << SYS_TICK_ENABLE_BIT) | (1uL << SYS_TICK_CLK_SOURCE_BIT);
  
  OS_TICK_Config(OS_TICK_MSEC, 1);
  //
  // Install Systick Timer Handler and enable timer interrupt
  //
  OS_ARM_ISRSetPrio(OS_ISR_ID_TICK, OS_ISR_TICK_PRIO);
  OS_ARM_EnableISR(OS_ISR_ID_TICK);


  SYSAHBCLKCTRL |= ALM_CT_CLK;
  CT_PR = OS_PSSYSALM-1; // maximum value for the Prescale Counter (about 394 ns)
  CT_TCR =1; // The Timer Counter and Prescale Counter are enabled for counting
  OS_ARM_ISRSetPrio(ALM_ISR_ID, OS_ISR_TICK_PRIO);
  OS_ARM_EnableISR(ALM_ISR_ID);

  OS_COM_INIT();
  OS_DecRI();
}

/*********************************************************************
*
*       Idle loop  (OS_Idle)
*
*       Please note:
*       This is basically the "core" of the idle loop.
*       This core loop can be changed, but:
*       The idle loop does not have a stack of its own, therefore no
*       functionality should be implemented that relies on the stack
*       to be preserved. However, a simple program loop can be programmed
*       (like toggeling an output or incrementing a counter)
*/
void OS_Idle(void) {     // Idle loop: No task is ready to execute
  while (1) {
  }
}
/**
OS system alarm handler
*/
int _OS_SystemAlarmHandler(void) 
{ 
	if(OS_ALMCT_IR & ALM_IR_MSK)
	{
		OS_ALMCT_IR |= ALM_IR_MSK;
  		OS_ALARM_Handler();
        return 1;
	}
    return 0;
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
  return OS_ALARM_HW_CNT_MATCH_VAL;
}

/**
\retval if system alarm is just raised
*/
int OS_AlarmRaised(void)
{
  return (OS_ALMCT_IR & ALM_IR_MSK);
}


/**
Disables all alarms
*/
void OS_DisableAlarms(void)
{
  OS_ALARM_MATCH_CTRL &= ~OS_ALARM_MATCH_CTRL_MSK;
}

/**
Arms system alarm
*/
void OS_AlarmCounterArm(OS_ALARM_TICK Tick)
{
  OS_ALMCT_IR |= ALM_IR_MSK;  /* Reset compare interrupt request flag*/
  OS_ALARM_MATCH_CTRL |= OS_ALARM_MATCH_CTRL_MSK;
  OS_ALARM_HW_CNT_MATCH_VAL = Tick;  
}

/*********************************************************************
*
*       OS_GetTime_Cycles()
*
*       This routine is required for high
*       resolution time measurement functions.
*       It returns the system time in timer clock cycles.
*/
OS_U32 OS_GetTime_Cycles(void) {
    unsigned int t_cnt;
    OS_U32 time;
    time  = OS_Time;
    t_cnt = (OS_PCLK_TIMER/1000) - SYS_TICK_VALUE;
    if (SYS_INT_CTRL_STATE & (1uL << SYS_PENDSTSET)) {       // Missed a counter interrupt?
        t_cnt = (OS_PCLK_TIMER/1000) - SYS_TICK_VALUE;       // Adjust result
        time++;
    }
    return (OS_PCLK_TIMER/1000) * time + t_cnt;
}

/*********************************************************************
*
*       OS_ConvertCycles2us
*
*       Convert Cycles into micro seconds.
*
*       If your clock frequency is not a multiple of 1 MHz,
*       you may have to modify this routine in order to get proper
*       diagonstics.
*
*       This routine is required for profiling or high resolution time
*       measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles) {
  return Cycles/(OS_PCLK_TIMER/1000000);
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
  //return  us*(OS_PCLK_ALM/1000000);
  
  OS_U32 Cycles;

  Cycles = us * (0x20*OS_PSSYSALM);
  Cycles = (unsigned int)Cycles/(unsigned int)(1000000/(OS_PCLK_ALM/0x20/OS_PSSYSALM));
  Cycles = Cycles;
  return Cycles;
  
}
#if OS_UART_USED
/*********************************************************************
*
*       Communication for OSView via UART (optional)
*
**********************************************************************
*/
#define BAUDDIVIDE ((OS_PCLK_UART + (8 * OS_BAUDRATE)) / (16 * OS_BAUDRATE))

/*********************************************************************
*
*       OS_COM_IsrHandler
*
* Function description
*   The communication interrupt handler for UART communication
*   to embOSView.
*/
void UART0_IRQHandler(void) {
  OS_U8 IntID;
  OS_U8 c;
  OS_EnterNestableInterrupt();
  IntID = (U0IIR >> 1) & 0x07;
  if (IntID == 0x02) {           // Is Rx interrupt pending?
    if ((U0LSR & 0x0E) == 0) {
      c = U0RBR;
      OS_OnRx(c);
    }
  } else {
    if (IntID == 0x01) {         // Is Tx interrupt pending?
      if (OS_OnTx()) {
        U0IER &= ~0x02uL;        // Disable Tx interrupt
      }
    }
  }
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       OS_COM_Send1()
*
*       Sends one character via UART.
*       Never call this function from your application
*/
void OS_COM_Send1(OS_U8 c) {
  U0THR  = c;
  U0IER |= 0x02; // Enable tx interrupt
}

/*********************************************************************
*
*       OS_COM_Init()
*
*       Initialize the selected UART
*/
void OS_COM_Init(void) {
  SYSAHBCLKCTRL  |=  ((1uL << OS_UART_CLK_BIT) | (1uL << 16));  // Enable clock for Uart and IOCON
  IOCON_RXD       = 0x82;                          // Select function Rxd
  IOCON_TXD       = 0x82;                          // Select function Txd
  UARTCLKDIV      = 4;
  U0FDR           = 0x10;                          // Reset fractional divider, not used
  U0TER           = 0x00;                          // Tx disable
  U0LCR           = 0x03;                          // 8N1
  U0LCR          |= 0x80;                          // Enable access to divisor latches
  U0DLL           = BAUDDIVIDE & 0xFF;             // Select baudrate
  U0DLM           = (BAUDDIVIDE >> 8) & 0xFF;
  U0LCR          &= ~0x80uL;                       // Disable access to divisor latches
  U0FCR           = 0x01;                          // Enable FIFOs
  U0IER          |= 0x01;                          // Enable Receive interrupt
  U0TER           = 0x80;                          // Tx enable
  OS_ARM_ISRSetPrio(OS_ISR_ID_UART, OS_ISR_UART_PRIO);
  OS_ARM_EnableISR(OS_ISR_ID_UART);
}

#elif OS_USE_JLINKMEM

/*********************************************************************
*
*       _JLINKMEM_OnRx()
*/
static void _JLINKMEM_OnRx(OS_U8 Data) {
  OS_OnRx(Data);
}

/*********************************************************************
*
*       _JLINKMEM_OnTx()
*/
static void _JLINKMEM_OnTx(void) {
  OS_OnTx();
}

/*********************************************************************
*
*       _JLINKMEM_GetNextChar()
*/
static OS_INT _JLINKMEM_GetNextChar(void) {
  return OS_COM_GetNextChar();
}

/*********************************************************************
*
*       OS_COM_Init()
*       Initialize memory access for OSView
*/
void OS_COM_Init(void) {
  JLINKMEM_SetpfOnRx(_JLINKMEM_OnRx);
  JLINKMEM_SetpfOnTx(_JLINKMEM_OnTx);
  JLINKMEM_SetpfGetNextChar(_JLINKMEM_GetNextChar);
}

/*********************************************************************
*
*       OS_COM_Send1()
*       Send 1 character via memory
*/
void OS_COM_Send1(OS_U8 c) {
  JLINKMEM_SendChar(c);
}


#else // No UART communication for OS enabled, define dummy functions

/*********************************************************************
*
*       Communication for embOSView not selected
*
**********************************************************************
*/
void OS_COM_Init(void) {}
void OS_COM_Send1(OS_U8 c) {
  OS_USEPARA(c);
  OS_COM_ClearTxActive();  /* Free OS transmitter buffer */
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

/*********************************************************************
*
*       OS_ARM_EnableISR
*/
void OS_ARM_EnableISR(int ISRIndex) {
  OS_DI();
  if (ISRIndex < NUM_INTERRUPTS) {
    if (ISRIndex >= 16) {
      //
      // Enable standard "external" interrupts, starting at index 16
      //
      ISRIndex -= 16;
      *(((OS_U32*) NVIC_ENABLE_ADDR) + (ISRIndex >> 5)) = (1uL << (ISRIndex & 0x1F));
    } else if (ISRIndex == ISR_ID_SYSTICK) {
      //
      // Enable the System Tick interrupt.
      //
      SYS_TICK_CONTROL |= (1uL << SYS_TICK_INT_ENABLE_BIT);
    }
  }
  OS_RestoreI();
}

/*********************************************************************
*
*       OS_ARM_DisableISR
*/
void OS_ARM_DisableISR(int ISRIndex) {
  OS_DI();
  if (ISRIndex < NUM_INTERRUPTS) {
    if (ISRIndex >= 16) {
      //
      // Disable standard "external" interrupts
      //
      ISRIndex -= 16;
      *(((OS_U32*) NVIC_DISABLE_ADDR) + (ISRIndex >> 5)) = (1uL << (ISRIndex & 0x1F));
    } else if (ISRIndex == ISR_ID_SYSTICK) {
      //
      // Enable the System Tick interrupt.
      //
      SYS_TICK_CONTROL &= ~(1uL << SYS_TICK_INT_ENABLE_BIT);
    }
  }
  OS_RestoreI();
}

/*********************************************************************
*
*       OS_ARM_ISRSetPrio
*
*   Notes:
*     (1) Some priorities of system handler are reserved
*         0..3 : Priority can not be set
*         7..10: Reserved
*         13   : Reserved
*     (2) System handler use different control register. This affects
*         ISRIndex 0..15
*/
int OS_ARM_ISRSetPrio(int ISRIndex, int Prio) {
  OS_U32* pPrio;
  int     OldPrio;
  OS_U8   Reg;
  OS_U8   Byte;

  OldPrio = 0;
  if (ISRIndex < NUM_INTERRUPTS) {
    if (ISRIndex >= 16) {
      //
      // Handle standard "external" interrupts
      // With CM0 we have only 32bit access to IPR registers
      //
      ISRIndex -= 16;                   // Adjust index
      OS_DI();
      pPrio   = (OS_U32*)(NVIC_PRIOBASE_ADDR);
      Reg     = ISRIndex / 4;
      Byte    = ISRIndex % 4;
      OldPrio = pPrio[Reg];
      OldPrio = OldPrio >> (Byte * 8);
      OldPrio &= 0xFF;
      pPrio[Reg] &= ~(0xFFuL << (Byte * 8));  // Clear old Prio
      pPrio[Reg] |= (Prio    << (Byte * 8));  // Set new Prio
      OS_RestoreI();
    } else {
      //
      // Handle System Interrupt controller
      //
      if ((ISRIndex <= 10) || (ISRIndex == 12) || (ISRIndex == 13)) {
        //
        // Reserved ISR channel, do nothing
        //
      } else {
        //
        // Set priority in system interrupt priority control register
        // With CM0 we only have 32bit access to SYSPRI registers!
        //
        OS_DI();
        pPrio = (OS_U32*)(SYSPRI1_ADDR);
        ISRIndex -= 4;                  // Adjust Index
        Reg  = ISRIndex / 4;
        Byte = ISRIndex % 4;
        OldPrio = pPrio[Reg];           // Get old Prio
        OldPrio = OldPrio >> (Byte * 8);
        OldPrio &= 0xFF;
        pPrio[Reg] &= ~(0xFFuL << (Byte * 8));  // Clear old Prio
        pPrio[Reg] |= (Prio    << (Byte * 8));  // Set new Prio
        OS_RestoreI();
      }
    }
  }
  return OldPrio;
}

/********************************************************************/

#ifdef __ICCARM__  // IAR

#ifdef __cplusplus
  extern "C" {
#endif
  int __low_level_init(void);
#ifdef __cplusplus
#endif
#ifdef __cplusplus
  }
#endif

/*********************************************************************
*
*       __low_level_init()
*
*/
int __low_level_init(void) {
#if OS_DISABLE_WATCHDOG  //
  // Initially disable Watchdog
  //
  WDT_MOD  = 0;
  WDT_FEED = 0xAA;
  WDT_FEED = 0x55;
  SYSAHBCLKCTRL &= ~(1uL << SYS_CLK_WDT_BIT);
#endif
  //
  // Insert other initialization code here, if required
  //

  return 1;  // Always return 1 to force segment initialization
}
#endif

/*****  End Of File  ************************************************/
