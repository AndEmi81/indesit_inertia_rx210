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
*       OS version: 3.86d                                            *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : RTOSInit_RX210.c for RENESAS RX210 and RPBRX210 eval board
          for HEW, IAR and GNU compiler.          
Purpose : Initializes and handles the hardware for embOS as far
          as required by the OS
          Feel free to modify this file acc. to your target system.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"
#include "LIB_Trace.h"
#include "LIB_TaskMonitoring.h"

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/

/*********************************************************************
*
*       Clock frequency settings
*/
#ifndef		OS_FSYS                   /* Main clock frequency         */
  #define	OS_FSYS 20000000uL        /* Assume 20MHz                 */
#endif



#ifndef		OS_INIT_PLL
	#define 	OS_INIT_PLL (0)
#endif

#ifndef		OS_TICK_FREQ
	#define 	OS_TICK_FREQ (1000)         /* <<< System tick */
#endif
#define		OS_TICK_MSEC              (1000/OS_TICK_FREQ)


/*********************************************************************
*
*       Configuration of communication to OSView
*/
#ifndef   OS_VIEW_ENABLE            /* Global enable of communication*/
  #define OS_VIEW_ENABLE    (1)     /* Default: on*/
#endif

#ifndef   OS_VIEW_USE_UART          /* If set, UART will be used*/
  #define OS_VIEW_USE_UART  (1)     /* Default: 1 => Uart is used*/
#endif                              /* if OS_VIEW_ENABLE is on*/

/*********************************************************************
*
*       UART settings for OSView
*       If you do not want (or can not due to hardware limitations)
*       to dedicate a UART to OSView, please define it to be -1
*       Currently UART1 is supported and the standard
*       setup enables UART1 per default
*/
#ifndef   OS_UART
  #define OS_UART (-1)
#endif

#ifndef   OS_BAUDRATE
  #define OS_BAUDRATE (38400)
#endif

#ifndef   OS_PCLK_UART
  #define OS_PCLK_UART  (OS_FSYS)
#endif

/*********************************************************************
*
*       Configuration of Option Bytes and ID Code
*
*       We set all bytes explicitely to 0xFF which is the default value
*       IAR handles these bytes in IAR linker files
*/
#ifdef __RX
  #pragma address _MDEreg=0xFFFFFF80            /* MDE register (Single Chip Mode)*/
  #ifdef __BIG
    const unsigned long _MDEreg = 0xFFFFFFF8;   /* Big endian*/
  #else
    const unsigned long _MDEreg = 0xFFFFFFFF;   /* Little endian*/
  #endif
  #pragma address _Dummy=0xFFFFFF84             /* Dummy value*/
  const unsigned long _Dummy  = 0xFFFFFFFF;
  #pragma address _OFS1=0xFFFFFF88              /* Option function select register 1 (OFS1)*/
  const unsigned long _OFS1   = 0xFFFFFFFF;
  #pragma address _OFS2=0xFFFFFF8C              /* Option function select register 2 (OFS2)*/
  const unsigned long _OFS2   = 0xFFFFFFFF;
  #pragma address _IDCode0=0xFFFFFFA0           /* ID Code 0*/
  const unsigned long _IDCode0 = 0xFFFFFFFF;
  #pragma address _IDCode1=0xFFFFFFA4           /* ID Code 1*/
  const unsigned long _IDCode1 = 0xFFFFFFFF;
  #pragma address _IDCode2=0xFFFFFFA8           /* ID Code 2*/
  const unsigned long _IDCode2 = 0xFFFFFFFF;
  #pragma address _IDCode3=0xFFFFFFAC           /* ID Code 3*/
  const unsigned long _IDCode3 = 0xFFFFFFFF;           
#endif
#ifdef __GNUC__
  /*
  // Please ensure that sections ".options" and ".idcode" are defined in linker at fixed addresses 0xFFFFFF80 and 0xFFFFFFA0.
  */
  #ifdef __RX_BIG_ENDIAN__                                                            /* MDE register (Single Chip Mode)*/
  const unsigned long _MDEreg	__attribute__ ((section (".options"))) = 0xFFFFFFF8;  /* Big endian*/
  #else
  const unsigned long _MDEreg	__attribute__ ((section (".options"))) = 0xFFFFFFFF;  /* Little endian*/
  #endif
  const unsigned long _Dummy    __attribute__ ((section (".options"))) = 0xFFFFFFFF;  /* Dummy value*/
  const unsigned long _OFS1     __attribute__ ((section (".options"))) = 0xFFFFFFFF;  /* Option function select register 1 (OFS1)*/
  const unsigned long _OFS0     __attribute__ ((section (".options"))) = 0xFFFFFFFF;  /* Option function select register 2 (OFS0)*/
  const unsigned long _IDCode0  __attribute__ ((section (".idcode")))  = 0xFFFFFFFF;  /* ID Code 0*/
  const unsigned long _IDCode1  __attribute__ ((section (".idcode")))  = 0xFFFFFFFF;  /* ID Code 1*/
  const unsigned long _IDCode2  __attribute__ ((section (".idcode")))  = 0xFFFFFFFF;  /* ID Code 2*/
  const unsigned long _IDCode3  __attribute__ ((section (".idcode")))  = 0xFFFFFFFF;  /* ID Code 3*/
#endif

/****** End of configurable options *********************************/

#define OS_UART_USED  ((OS_VIEW_ENABLE && (OS_VIEW_USE_UART != 0)) && (OS_UART == 0))

#if OS_UART_USED
  #define OS_COM_INIT() OS_COM_Init()
#else
  #define OS_COM_INIT()
#endif

/*********************************************************************
*
*       Check configuration
*
**********************************************************************
*/

#ifndef   DEBUG     /* Should normally be defined as project option */
  #define DEBUG  (0)
#endif

/*********************************************************************
*
*       Compiler dependent defines
*
**********************************************************************
*/
#ifdef __ICCRX__
  #define EVENACCESS
  #define WAIT_FOR_INTERRUPT()   __wait_for_interrupt()
#endif

#ifdef __RX
  #define EVENACCESS             __evenaccess
  #define WAIT_FOR_INTERRUPT()   wait()
#endif

#ifdef __GNUC__
  #define EVENACCESS             
  #define WAIT_FOR_INTERRUPT()   __builtin_rx_wait()
#endif

#ifndef	EVENACCESS
#define	EVENACCESS
#endif

/*********************************************************************
*
*   Sfrs used in RTOSInit
*
**********************************************************************
*/

/****** Protect register  ********************************************/
#define PRCR                 (*(volatile EVENACCESS OS_U16*) (0x000803FE))           

/****** Interrupt controller       **********************************/
#define IRR_BASE_ADDR        (0x00087000)
#define ISEL_BASE_ADDR       (0x00087100)

#define IER03                (*(volatile EVENACCESS OS_U8*)  (0x00087203))
#define IER1A                (*(volatile EVENACCESS OS_U8*)  (0x0008721A))
#define IER1B                (*(volatile EVENACCESS OS_U8*)  (0x0008721B))
#define IER15                (*(volatile EVENACCESS OS_U8*)  (0x00087215))
#define IER16                (*(volatile EVENACCESS OS_U8*)  (0x00087216))
#define IPR04                (*(volatile EVENACCESS OS_U8*)  (0x00087304))
#define IPR214               (*(volatile EVENACCESS OS_U8*)  (0x000873D6))
#define IPR218               (*(volatile EVENACCESS OS_U8*)  (0x000873DA))
#define IPR174               (*(volatile EVENACCESS OS_U8*)  (0x000873AE))
#define IPR175               (*(volatile EVENACCESS OS_U8*)  (0x000873AF))
#define IPR180               (*(volatile EVENACCESS OS_U8*)  (0x000873B4))

/****** SYSTEM module  **********************************************/
#define SYSTEM_BASE_ADDR     (0x00080000)
#define MSTPCRA              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x010))  /* Module stop register A*/
#define MSTPCRB              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x014))  /* Module stop register B*/
#define MSTPCRC              (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x018))  /* Module stop register C*/
#define SCKCR                (*(volatile EVENACCESS OS_U32*) (SYSTEM_BASE_ADDR + 0x020))  /* System clock control register*/
#define SCKCR3               (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x026))  /* System clock control register 3*/
#define PLLCR                (*(volatile EVENACCESS OS_U16*) (SYSTEM_BASE_ADDR + 0x028))  /* PLL control register */
#define PLLCR2               (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x02A))  /* PLL control register 2*/
#define BCKCR                (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x030))  /* External bus clock control register*/
#define MOSCCR               (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x032))  /* Main clock oscillator control register*/
#define MOSCWTCR             (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x0A2))  /* Main clock oscillator wait control register*/
#define PLLWTCR              (*(volatile EVENACCESS OS_U8*)  (SYSTEM_BASE_ADDR + 0x0A6))  /* PLL wait control register*/

/****** Assign timer used for OS ************************************/
#if defined(OS_SYSTIMER_TMR01_A)

#define SYS_TMR_BASE_ADDR      	(0x00088200)
#define SYS_TMR_TCR            	(*(volatile EVENACCESS OS_U8*) (SYS_TMR_BASE_ADDR + 0x00))
#define SYS_TMR_TCORA          	(*(volatile EVENACCESS OS_U16*)(SYS_TMR_BASE_ADDR + 0x04))
#define SYS_TMR_TCORB          	(*(volatile EVENACCESS OS_U16*)(SYS_TMR_BASE_ADDR + 0x06))
#define SYS_TMR_TCNT           	(*(volatile EVENACCESS OS_U16*)(SYS_TMR_BASE_ADDR + 0x08))
#define SYS_TMR0_TCCR           (*(volatile EVENACCESS OS_U8*) (SYS_TMR_BASE_ADDR + 0x0A))
#define SYS_TMR1_TCCR           (*(volatile EVENACCESS OS_U8*) (SYS_TMR_BASE_ADDR + 0x0B))
#define SYS_TMR_TCSTR          	(*(volatile EVENACCESS OS_U8*) (SYS_TMR_BASE_ADDR + 0x0C))
#define SYS_TMR_MSTPCR         	(MSTPCRA)
#define SYS_TMR_MSTPCR_BIT     	(5)
#define SYS_TMR_VECT           	(174)
#define SYS_TMR_IPR            	(IPR174)
#define SYS_TMR_IER            	(IER15)
#define SYS_TMR_IER_BIT        	(6)
#define OS_TIMER_INT_PRIO    	(0x01)     /* Lowest priority*/
#define OS_TIMER_VECT        	SYS_TMR_VECT
#define SYS_TMR_MATCH_VAL		SYS_TMR_TCORA
#else
	#error	"Unsupported system timer."
#endif

#if defined(OS_SYSALARM_TMR01_B) /*|| defined(OS_SYSALARM_TMR01_A)*/
#define ALM_TMR_BASE_ADDR      		(0x00088200)
#define ALM_TMR_TCR            		(*(volatile EVENACCESS OS_U8*) (ALM_TMR_BASE_ADDR + 0x00))
#define ALM_TMR_TCORA          		(*(volatile EVENACCESS OS_U16*)(ALM_TMR_BASE_ADDR + 0x04))
#define ALM_TMR_TCORB          		(*(volatile EVENACCESS OS_U16*)(ALM_TMR_BASE_ADDR + 0x06))
#define ALM_TMR_TCNT           		(*(volatile EVENACCESS OS_U16*)(ALM_TMR_BASE_ADDR + 0x08))
#define ALM_TMR0_TCCR            	(*(volatile EVENACCESS OS_U8*) (ALM_TMR_BASE_ADDR + 0x0A))
#define ALM_TMR1_TCCR            	(*(volatile EVENACCESS OS_U8*) (ALM_TMR_BASE_ADDR + 0x0B))
#define ALM_TMR_TCSTR          		(*(volatile EVENACCESS OS_U8*) (ALM_TMR_BASE_ADDR + 0x0C))
#define OS_ALARM_HW_CNT           	ALM_TMR_TCNT
#define OS_ALARM_HW_EN_CTRL			ALM_TMR_MSTPCR
#define OS_ALARM_HW_EN_BIT			ALM_TMR_MSTPCR_BIT
#define OS_ALARM_CNT_START			ALM_TMR_TCSTR


#define OS_ALM_TIMER_INT_PRIO		(0x01)
#define OS_ALARM_PRIO_INT_REG     	(IPR175)
#define OS_ALARM_MATCH_ICU       	(IER15)
#define OS_ALARM_MATCH_ICU_MSK   	(1uL << 7)
#define OS_ALM_TIMER_PRESCALE   	(OS_PSSYS)
#define OS_ALM_TIMER_VECT       	(175)
#define OS_ALARM_HW_CNT_MATCH_VAL 	ALM_TMR_TCORB

#define OS_ALMCT_IR               	(*(OS_U8*)(IRR_BASE_ADDR + OS_ALM_TIMER_VECT))
#else
	#error	"Unsupported option for system alarm."
#endif


#define OS_TIMER_RELOAD		((OS_PCLK_TIMER/OS_PSSYS)/OS_TICK_FREQ)





/****** UART ********************************************************/
#if OS_UART_USED

#define SCI0_BASE_ADDR       (0x0008A000)

#define PWPR                 (*(volatile EVENACCESS OS_U8*)(0x0008C11F))
#define P20PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C150))
#define P21PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C151))
#define P15PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C14D))
#define P16PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C14E))
#define P26PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C156))
#define P30PFS               (*(volatile EVENACCESS OS_U8*)(0x0008C158))
#define P2PMR                (*(volatile EVENACCESS OS_U8*)(0x0008C062))
#define P3PMR                (*(volatile EVENACCESS OS_U8*)(0x0008C063))

#if (OS_UART == 0)
  #define OS_UART_MSTPCR     (MSTPCRB)
  #define OS_UART_MSTPCR_BIT (31)
  
  #define OS_UART_BASE_ADDR  (SCI0_BASE_ADDR)
  #define OS_UART_IPR        (IPR214)
  #define OS_UART_IER_ER     (IER1A)
  #define OS_UART_IER_RX     (IER1A)
  #define OS_UART_IER_TX     (IER1B)
  #define OS_UART_IER_ER_BIT (6)
  #define OS_UART_IER_RX_BIT (7)
  #define OS_UART_IER_TX_BIT (0)
  #define OS_UART_ICR        (P2_ICR)
  #define OS_UART_ICR_BIT    (1)

  #define OS_UART_ERROR_VECT 214
  #define OS_UART_RX_VECT    215
  #define OS_UART_TX_VECT    216
#else
  #error "Selected UART not supported, OS_UART_* definitions have to be modified."
#endif  

#define OS_UART_INT_PRIO     (0x01)     /* Lowest priority*/

#define OS_UART_SMR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x00))
#define OS_UART_BRR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x01))
#define OS_UART_SCR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x02))
#define OS_UART_TDR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x03))
#define OS_UART_SSR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x04))
#define OS_UART_RDR          (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x05))
#define OS_UART_SCMR         (*(volatile EVENACCESS OS_U8*) (OS_UART_BASE_ADDR + 0x06))

#define UART_ERR_PER_BIT     (3)
#define UART_ERR_FER_BIT     (4)
#define UART_ERR_ORER_BIT    (5)
#define UART_ERR_PER_MASK    (1uL << UART_ERR_PER_BIT)
#define UART_ERR_FER_MASK    (1uL << UART_ERR_FER_BIT)
#define UART_ERR_ORER_MASK   (1uL << UART_ERR_ORER_BIT)
#define UART_ERR_MASK_ALL    (UART_ERR_PER_MASK | UART_ERR_FER_MASK | UART_ERR_ORER_MASK)
#endif  /* OS_UART_USED*/

/*********************************************************************
*
*       OS_InitHW()
*
*       Initialize the hardware (timer) required for the OS to run.
*       May be modified, if an other timer should be used
*/
void OS_InitHW(void) {
  OS_IncDI();
  /*
  // Initialize timer for embOS, assuming PLL is already initialized
  */

#if defined (OS_TICK_HANDLE_EX)
  OS_TICK_Config(OS_TICK_MSEC, OS_TICK_RESOLUTION);
#endif

  PRCR = 0xA502;                                           		/* Protect off	- 210*/
  /*PRCR = 0xA503;*/                                           /* Protect off	- 220*/

  SYS_TMR_MSTPCR &= ~(1uL << SYS_TMR_MSTPCR_BIT);              	/* Activate TMR01 unit*/
  SYS_TMR_TCSTR   = 0x00;                                    	/* Stop counter*/
  SYS_TMR_TCNT    = 0x0;                                     	/* Set count value to 0*/
  SYS_TMR_TCR     = 0x40;                                    	/* Compare match interrupt enable*/
  SYS_TMR_MATCH_VAL   = (OS_U16)OS_TIMER_RELOAD;              	/* Compare value for resolution (OS_TICK_FREQ)*/
  SYS_TMR0_TCCR 	= 0x18|OS_SYSTEM_PS;                        /* Count on TMR1 overflow -> 16bit cascade mode ps=8 (see TIMER_PRESCALE)*/
  SYS_TMR1_TCCR 	= 0x08|OS_SYSTEM_PS;                        /* Set Clock input, ps=8 (see TIMER_PRESCALE)*/
  /*
  // Enable TMR01 interrupt in interrupt controller
  */
  SYS_TMR_IPR  = OS_TIMER_INT_PRIO;                          	/* Set interrupt priority, lowest priority*/
  SYS_TMR_IER |= (1uL << SYS_TMR_IER_BIT);                     	/* Enable TMR01 compare interrupt*/
  *(OS_U8*)(IRR_BASE_ADDR + OS_TIMER_VECT)  = 0;           		/* Clear interrupt request*/

	/*
	// Alarms cfg
	*/
  OS_ALARM_HW_CNT_MATCH_VAL	=	(OS_U16)0;              	/* Compare value*/

  OS_ALARM_PRIO_INT_REG = OS_ALM_TIMER_INT_PRIO;
  OS_ALARM_MATCH_ICU |= OS_ALARM_MATCH_ICU_MSK;
  OS_ALMCT_IR = 0;

  /*
  // Start timer
  */
  SYS_TMR_TCSTR 				= 0x01;						/* Start counter*/

  /*
  // Initialize the optional UART for OS viewer
  */
  OS_COM_INIT();
  PRCR = 0xA500;                                           /* Protect on*/
  OS_DecRI();
}

/*********************************************************************
*
*       OS_Idle()
*
*       Please note:
*       This is basically the "core" of the idle loop.
*       This core loop can be changed, but:
*       The idle loop does not have a stack of its own, therefore no
*       functionality should be implemented that relies on the stack
*       to be preserved. However, a simple program loop can be programmed
*       (like toggeling an output or incrementing a counter)
*/
void OS_Idle(void){ /* Idle loop: No task is ready to exec*/
	for (;;)
	{ /* Nothing to do ... wait for interrupt*/
#if (DEBUG == 0)
		WAIT_FOR_INTERRUPT(); /* Switch CPU into sleep mode*/
#endif

		TRACE_CPU_IDLE_START();
	}
}





/**
OS system alarm handler
*/
#ifdef __ICCRX__
  #pragma vector = (OS_ALM_TIMER_VECT)
  static __interrupt void _OS_SystemAlarmHandler (void) {
#elif __RX
  #pragma interrupt (OS_ISR_Tick(vect=OS_ALM_TIMER_VECT))
  void OS_ISR_Tick (void) {
#elif __GNUC__
void __attribute__ ((interrupt)) _OS_SystemAlarmHandler(void)
#else
void _OS_SystemAlarmHandler(void)
#endif
{ 
	OS_EnterInterrupt();
	IRQ_MONITOR_ENTER();
	OS_ALMCT_IR = 0;
	OS_ALARM_Handler();
	IRQ_MONITOR_EXIT();
	OS_LeaveInterrupt();
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
  return (OS_ALMCT_IR != 0);
}


/**
Disables all alarms
*/
void OS_DisableAlarms(void)
{
	ALM_TMR_TCR  &= ~0x80;                               /* Compare match A interrupt disabled*/
}

/**
Arms system alarm
*/
void OS_AlarmCounterArm(OS_ALARM_TICK Tick)
{
	OS_ALMCT_IR = 0;  /* Reset compare interrupt request flag*/
	OS_ALARM_HW_CNT_MATCH_VAL = Tick;
	ALM_TMR_TCR  |= 0x80;
}




/*********************************************************************
*
*       OS_GetTime_Cycles()
*
*       This routine is required for task-info via OSView or high
*       resolution time measurement functions.
*       It returns the system time in timer clock cycles.
*/
OS_U32 OS_GetTime_Cycles(void) {
  OS_U32 Time;
  OS_U16 Cnt;

  Time = OS_GetTime32();
  Cnt  = SYS_TMR_TCNT;

  if (*(volatile EVENACCESS OS_U8*)(IRR_BASE_ADDR + OS_TIMER_VECT) != 0) {  /* Adjust result*/
      Cnt = SYS_TMR_TCNT;
      Time++;
    }
    else
    {
      Cnt += OS_TIMER_RELOAD;
    }
    Cnt-=SYS_TMR_MATCH_VAL;
    return ((OS_TIMER_RELOAD*OS_TICK_RESOLUTION/OS_TICK_MSEC) * Time)+ Cnt;
}

/*********************************************************************
*
*       OS_ConvertCycles2us()
*
*       Convert Cycles into micro seconds.
*
*       If your clock frequency is not a multiple of 1 MHz,
*       you may have to modify this routine in order to get proper
*       diagnostics.
*
*       This routine is required for profiling or high resolution time
*       measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles) {
  return (Cycles * OS_PSSYS * 1000)/(OS_PCLK_TIMER/1000);
}

OS_U32 OS_Convert_us2Cycles(OS_U32 us) {
	return (us * (OS_PCLK_TIMER / 1000))/(OS_PSSYS * 1000);
}
/*********************************************************************
*
*       OS_ISR_Tick()
*       The OS system timer interrupt
*/
#ifdef __ICCRX__
  #pragma vector = (OS_TIMER_VECT)
  static __interrupt void OS_ISR_Tick (void) {
#elif __RX
  #pragma interrupt (OS_ISR_Tick(vect=OS_TIMER_VECT))
  void OS_ISR_Tick (void) {
#elif __GNUC__
  void OS_ISR_Tick(void) __attribute__ ((interrupt));

  void __attribute__ ((interrupt)) OS_ISR_Tick (void) {
#else
  void OS_ISR_Tick (void) {
#endif
  OS_EnterInterrupt();
  IRQ_MONITOR_ENTER();

  SYS_TMR_MATCH_VAL += OS_TIMER_RELOAD;
  *(OS_U8*)(IRR_BASE_ADDR + OS_TIMER_VECT) = 0;            /* Clear interrupt request*/
	
#if defined (OS_TICK_HANDLE_EX)
  OS_HandleTickEx();
#else
  OS_HandleTick();
#endif
  IRQ_MONITOR_EXIT();
  OS_LeaveInterrupt();
}

/*********************************************************************
*
*   Communication for embOSView
*
**********************************************************************
*/
#if OS_UART_USED
#define OS_BAUDDIVIDE ((OS_PCLK_UART+(OS_BAUDRATE*16L))/(OS_BAUDRATE*32L))-1
//#define OS_BAUDDIVIDE ( ((OS_PCLK_UART / OS_BAUDRATE) / 32u) - 1)
/*********************************************************************
*
*       OS_COM_Init()
*       Initialize UART 1
*/
void OS_COM_Init(void) {
  volatile int i;
  //
  // Enable UART Module
  //
  OS_UART_MSTPCR &= ~(1uL << OS_UART_MSTPCR_BIT);
  //
  // Set uart port pins
  //
  PWPR   = 0x00;  // Disable write protect
  PWPR   = 0x40;  // Enable write access
  P20PFS = 0x0A;  // Set pin function register to uart function
  P21PFS = 0x0A;  // Set pin function register to uart function
 /*2Z0 
  P26PFS = 0x0A;
  P30PFS = 0x0A;*/
  PWPR   = 0x80;  // Enable write protect
  P2PMR  = 0x03;  // Set pin to peripheral function
/*
  P2PMR  = 0x40;  // Set pin to peripheral function
  P3PMR  = 0x01;  // Set pin to peripheral function
*/
  //
  // Reset, disable Reception and transmission
  //
  OS_UART_SCR = 0;
  //
  // Set transmission mode to 8N1, use PCLK
  //
  OS_UART_SMR = 0
              | (0 << 7)
              | (0 << 6)
              | (0 << 5)
              | (0 << 4)
              | (0 << 3)
              | (0 << 2)
              | (0 << 1)
              | (0 << 0);
  //
  // Set transfer direction, LSB first
  //
  OS_UART_SCMR = 0x00;
  //
  // Set baudrate
  //
  OS_UART_BRR = OS_BAUDDIVIDE;
  //
  // Wait for SCI Settling time (1 bit cycle)
  //
  for(i = 0; i < 3000 ; i++);
  //
  // Clear status
  //
  OS_UART_SSR = 0x00;
  //
  // Setup interrupt control register for UART
  //
  OS_UART_IPR = OS_UART_INT_PRIO;     // Lowest priority
  //
  // Enable UART interrupts
  //
  OS_UART_IER_ER    |= (1uL << OS_UART_IER_ER_BIT);     // Enable Rx error interrupt
  OS_UART_IER_RX    |= (1uL << OS_UART_IER_RX_BIT);     // Enable Rx interrupt
  OS_UART_IER_TX    |= (1uL << OS_UART_IER_TX_BIT);     // Enable Tx empty interrupt
  //
  // Clear interrupt request flags
  //
  *(OS_U8*)(IRR_BASE_ADDR + OS_UART_ERROR_VECT) = 0;
  *(OS_U8*)(IRR_BASE_ADDR + OS_UART_RX_VECT)    = 0;
  *(OS_U8*)(IRR_BASE_ADDR + OS_UART_TX_VECT)    = 0;
  //
  // Finally enable Rx and Tx interrupts and enable uart Rx and Tx
  //
  OS_UART_SCR |= (1uL << 7)       // Tx interrupt enable
               | (1uL << 6);      // Rx interrupt enable
  OS_UART_SCR |= (1uL << 5)       // Tx enable
               | (1uL << 4);      // Rx enable
}

/*********************************************************************
*
*       OS_COM_Send1()
*       Send one character via UART.
*       Never call this function from your application
*/
void OS_COM_Send1(OS_U8 c) {
  OS_UART_TDR = c;
}

/*********************************************************************
*
*       _OS_ISR_RxErr()
*       Interrupt handler for UART reception error
*/
#ifdef __ICCRX__
  #pragma vector = (OS_UART_ERROR_VECT)  // RxError interrupt
  static __interrupt void _OS_ISR_RxErr (void) {
#endif
#ifdef __RX
  #pragma interrupt (_OS_ISR_RxErr(vect=OS_UART_ERROR_VECT))  // RxError interrupt
  static void _OS_ISR_RxErr (void) {
#endif
#ifdef __GNUC__
  void __attribute__ ((interrupt)) OS_ISR_RxErr (void) {
#endif
  OS_U8 Error;

  OS_EnterNestableInterrupt();
  //
  // Reset Error flags
  //
  Error       = OS_UART_SSR;            // Get current error state
  Error      &= ~(UART_ERR_MASK_ALL);   // Clear all errors
  OS_UART_SSR = Error;                  // Reset errors
  Error       = OS_UART_SSR;            // Dummy read, required according to hardware manual
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       _OS_ISR_Rx()
*       Interrupt handler for UART reception
*/
#ifdef __ICCRX__
  #pragma vector = (OS_UART_RX_VECT) // Rx data interrupt
  static __interrupt void _OS_ISR_Rx (void) {
#endif
#ifdef __RX
  #pragma interrupt (_OS_ISR_Rx(vect=OS_UART_RX_VECT)) // Rx data interrupt
  static void _OS_ISR_Rx (void) {
#endif
#ifdef __GNUC__
  void __attribute__ ((interrupt)) OS_ISR_Rx (void) {
#endif
  OS_U8 Data;

  OS_EnterNestableInterrupt();  //  We will enable interrupts
  Data = OS_UART_RDR;
  OS_OnRx(Data);
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       _OS_ISR_Tx()
*       Interrupt handler for UART transmission
*/
#ifdef __ICCRX__
  #pragma vector = (OS_UART_TX_VECT)  // TX data empty interrupt
  static __interrupt void _OS_ISR_Tx (void) {
#endif
#ifdef __RX
  #pragma interrupt (_OS_ISR_Tx(vect=OS_UART_TX_VECT))  // TX data empty interrupt
  static void _OS_ISR_Tx (void) {
#endif
#ifdef __GNUC__
  void __attribute__ ((interrupt)) OS_ISR_Tx (void) {
#endif
  OS_EnterNestableInterrupt();
  OS_OnTx();
  OS_LeaveNestableInterrupt();
}

#else  /* UART for communication not used, define dummy functions */

void OS_COM_Send1(OS_U8 c) {
  OS_USEPARA(c);           /* Avoid compiler warning */
  OS_COM_ClearTxActive();  /* Let the OS know that Tx is not busy */
}
#endif

/*********************************************************************
*
*       HardwareSetup()
*
*       Initialize clock generation and pll
*
*       Has to be modified, if another CPU clock frequency should be
*       used. This function is called during startup and
*       has to return 1 to perform segment initialization.
*
*       Because variables are not initialized before __low_level_init() is called,
*       no access or call which relies on any variable can be used here.
*
*       This function is called from HEW startup code.
*/
void HardwareSetup(void);
void HardwareSetup(void) {
#if OS_INIT_PLL
  unsigned int i;
  //
  // Enable PLL
  // We assume a 20 MHz external crystal
  //
  PRCR     = 0xA503;                   // Protect off 
  MOSCWTCR = 0x0D;                     // 131072 state, wait over 10ms  
  PLLWTCR  = 0x0E;                     // 2097152 state, wait over 12ms  @PLL=100MHz(20MHz/2*10) 
  PLLCR    = 0x0901;                   // x10 /2 @PLL 
  MOSCCR   = 0x02;                     // External oscillation input selection 
  PLLCR2   = 0x00;                     // Enable PLL 
  for(i = 0 ; i < 0x168 ; i++);        // wait over 12ms 
  SCKCR    = 0x21022222;               // ICK=PLL/2,FCK,PCK,BCL=PLL/4 
  SCKCR3   = 0x0400;                   // LOCO -> PLL 
  PRCR     = 0xA500;                   // Protect on 
#endif
}

#ifdef __ICCRX__
/*********************************************************************
*
*       __low_level_init()
*
*/
int __low_level_init(void) {
  HardwareSetup();
  return 1;
}
#endif

/****** End Of File *************************************************/
