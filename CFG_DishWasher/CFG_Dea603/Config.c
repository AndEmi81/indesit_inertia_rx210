/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	CFG_DEA700
@{
\Par		CFG_DEA700
\n			This module contains the platform2 configuration parameters for DEA700 project.
@}

*/
/**
\n
\Par		Content
\n			This file contains the platform2 configuration parameters for PlatformNodeZED project.
\n
\file		Config.c
\ingroup	CFG_DEA700
\date		22/09/2010
\version	00.00.63 
\author		Roberto Fioravanti
\n
\n
\Par\b	History:
\n 
\n 		00.00.30 
\n		28/02/2010
\n		Roberto Fioravanti
\n		changes
\n 		- creation
\n
\n
\n etc.
*/


#include "HAL_Interface.h"
#include HAL_CFG
#include HAL

/* ******** */
/* Includes */
/* ******** */
#include "io_driver.h"
#include "config.h"


#include "SkeletonGlobal.h"
#include "DigitalProtocolExp.h" 
#include "callback.h"  

#if defined(SLEEP_MODE_ENABLE)
 #include "platform.h"
#endif /* SLEEP_MODE_ENABLE */

#include "SWTimersCounters.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

#if defined (DEBUG_RENESAS_36079) && defined (EDK3687)
#undef MCU_LINKAGE 
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
  PIN(B,6),		/* PIN 1 */
  PIN(B,7), 
  PIN_DUMMY,	/* AVcc */
  PIN_DUMMY,	/* X2 */ 
  PIN_DUMMY,	/* X1 */ 
  PIN_DUMMY,	/* VCL */ 
  PIN_DUMMY,	/* RES */ 
  PIN_DUMMY,	/* TEST */
  PIN_DUMMY,	/* Vss */
  PIN(C,1),		/* PIN 10 */
  PIN(C,0),
  PIN_DUMMY,	/* Vcc */
  PIN(5,0),		/* PIN 13 TOGGLE TASK 10 MSEC */
  PIN(5,1), 	
  PIN(3,4),		/* PIN 15 TOGGLE TASK 100 MSEC */
  PIN(3,5),		/* PIN 16 */
  PIN(3,6),		/* PIN 17 TOGGLE TASK ANALOG */
  PIN(3,7),
  PIN(5,2),
  PIN(5,3),		
  PIN(5,4),
  PIN(5,5),
  PIN(1,0),		
  PIN(1,1),		
  PIN(1,2),
  PIN(5,6),
  PIN(5,7),
  PIN(7,4),		
  PIN(7,5),
  PIN(7,6),
  PIN(2,4),
  PIN(6,3),	/* PIN 32 */		
  PIN(6,2),	/* PIN 33 TOGGLE TASK FEEDBACK */
  PIN(6,1),
  PIN_DUMMY,	/* NMI */
  PIN(6,0),		
  PIN(6,4),		/* PIN 37 ULED1 */
  PIN(6,5),		/* PIN 38 ULED2 */
  PIN(6,6),		/* PIN 39 Z.C. Gen. */
  PIN(6,7),	
  PIN(8,5),	
  PIN(8,6), 
  PIN(8,7),	
  PIN(2,0),		
  PIN(2,1),
  PIN(2,2),
  PIN(2,3),
  PIN(7,0),	/* PIN 48 */		
  PIN(7,1),
  PIN(7,2),
  PIN(1,4),	/* PIN 51 IRQ0 (Z.C. Input) */
  PIN(1,5),		
  PIN(1,6),
  PIN(1,7),
  PIN(3,3),
  PIN(3,2),		
  PIN(3,1),
  PIN(3,0),
  PIN(B,3),
  PIN(B,2),		
  PIN(B,1),
  PIN(B,0),
  PIN(B,4),
  PIN(B,5)	/* PIN 64 */		   
	
END_MODULE_IO_MAP()
#endif  /* defined (DEBUG_RENESAS_36079) && defined (EDK3687) */

/* Modulino castellated orizzontale */
#if defined (RENESAS_300H_39079) && defined (MCU_CASTELLATED) 
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	PIN(3,7),	/* Debug 	- T001 */
//  PIN_DUMMY,	/* NC1 		- T001 */
	PIN_DUMMY,	/* Vcc 		- T002 */
	PIN(6,0),	/* FTIOA0 	- T003 */
	PIN(6,1),	/* FTIOB0 	- T004 */
	PIN(6,3),	/* FTIOD0 	- T005 */	
	PIN(6,2),	/* FTIOC0 	- T006 */
	PIN(2,1),	/* RXD		- T007 */
	PIN(2,2),	/* TXD		- T008 */
	PIN(5,5),	/* WKP5		- T009 */	
	PIN(5,4),	/* WKP4		- T010 */	
	PIN(5,3),	/* WKP3		- T011 */
	PIN(2,0),	/* SCK3		- T012 */
	PIN(2,3),	/* 			- T013 */
	PIN(3,0),	/*			- T014 */
	PIN_DUMMY,	/* Gnd 		- T015 */
	PIN(8,5),	/* Debugger - T016 */	
//	PIN_DUMMY,	/* NC2 		- T016 */
	PIN(8,6),   /* Debugger - T017 */
//	PIN_DUMMY,	/* NC3 		- T017 */
	PIN_DUMMY,	/* AGnd 	- T018 */
	PIN(B,3),	/* AN3		- T019 */
	PIN(B,2),	/* AN2		- T020 */	
	PIN(B,1),	/* AN1		- T021 */
	PIN(B,0),	/* AN0		- T022 */	
	PIN(B,4),	/* AN4		- T023 */	
	PIN(B,5),	/* AN5		- T024 */	
	PIN(B,7),	/* AN7		- T025 */	
	PIN(B,6),	/* AN6		- T026 */	
	PIN_DUMMY,	/* AVcc 	- T027 */
	PIN(3,1),	/*			- T028 */
	PIN(3,2),	/*			- T029 */	
	PIN(3,3),	/*			- T030 */
	PIN(3,5),	/*			- T031 */
	PIN(8,7),	/* Debugger - T032 */	
//	PIN_DUMMY,	/* NC4 		- T032 */	
	PIN_DUMMY,	/* NC5 		- T033 */	
	PIN_DUMMY,	/* Vcc 		- T034 */
	PIN(6,4),	/* FTIOA1	- T035 */
	PIN(6,7),	/* FTIOD1	- T036 */	
	PIN(6,6),	/* FTIOC1	- T037 */
	PIN(6,5),	/* FTIOB1	- T038 */
	PIN(7,1),	/* RXD_2	- T039 */
	PIN(7,2),	/* TXD_2	- T040 */
	PIN(5,0),	/* WKP0		- T041 */
	PIN(5,1),	/* WKP1		- T042 */
	PIN(5,2),	/* WKP2		- T043 */
	PIN(C,0),	/* OSC1		- T044 */
	PIN(C,1),	/* OSC2/CLKOUT - T045 */
	PIN(7,0),	/* SCK3_2	- T046 */
	PIN_DUMMY,	/* Gnd 		- T047 */
	PIN_DUMMY,	/* NC6 		- T048 */
	PIN_DUMMY,	/* NC7 		- T049 */
	PIN_DUMMY,	/* IRQ0		- T050 */
	PIN(3,6),	/* 			- T051 */		
	PIN(1,0),	/* TMOV		- T052 */
	PIN(1,1),	/* PWM		- T053 */
	PIN(1,2),	/*			- T054 */
	PIN(1,7),	/* IRQ3/TRGV - T055 */
	PIN(7,4),	/* TMRIV	- T056 */
	PIN(5,6),	/* SDA		- T057 */	
	PIN(5,7),	/* SCL		- T058 */
	PIN(7,5),	/* TMCIV	- T059 */	
	PIN(7,6),	/* TMOV		- T060 */
	PIN(2,4),	/*			- T061 */
	PIN(3,4),	/*			- T062 */
	PIN_DUMMY,	/* Reset	- T063 */
	PIN_DUMMY,	/* NC8 		- T064 */				
END_MODULE_IO_MAP()
#endif

/* Modulino M.C.U. verticale */
#if defined (RENESAS_300H_39079) && defined (MCU_LINKAGE)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	PIN_DUMMY, 		/*Versione LINKAGE*/
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
    PIN(3,1), 		/* pin 5 */
    PIN(3,0),
    PIN(3,3),
    PIN(3,2),
    PIN(3,5),
    PIN(3,4),
    PIN(3,7),
    PIN(3,6), /* pin 12 */
    PIN(5,1),
    PIN(5,0),
    PIN(5,3),
    PIN(5,2),
    PIN(5,5),
    PIN(5,4), /*pin 18 */
    PIN(6,1), 
    PIN(6,0),
    PIN(6,3),
    PIN(6,2),
    PIN(6,5),
    PIN(6,4),
    PIN(6,7),
    PIN(6,6), /* pin 26 */
    PIN(2,1),
    PIN(1,4), /* IRQ */
    PIN(2,2),    
    PIN(5,6), /* SDA */
    PIN(7,1), 
    PIN(5,7), /* SCL */
    PIN(7,2), /* pin 33 */
    PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
    PIN(B,1), /* pin 37 */
    PIN(B,0),  
    PIN(B,3),
    PIN(B,2),
    PIN(B,5),
    PIN(B,4),
    PIN(B,7),  
    PIN(B,6), /* pin 44 */
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY, 
	PIN_DUMMY, 
	PIN_DUMMY,
	PIN_DUMMY,	/* pin 64 */ 

								
END_MODULE_IO_MAP()
#endif /* #if defined (RENESAS_300H_39079) && defined (MCU_LINKAGE) */


/* platform SW counters */

SWTC_SCounterVariableType Time2Death_Counter;
SWTC_CounterConfigurationType CfgTime2Death_Counter = {SWTC_CFG_INIT(FALSE,3,SWTC_CFG_SIZE_S)};

SWTC_SCounterVariableType Time_Since_Last_Reset_Counter;
SWTC_CounterConfigurationType CfgTime_Since_Last_Reset_Counter = {SWTC_CFG_INIT(FALSE,6,SWTC_CFG_SIZE_S)};

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/**
OCI2C interrupt callback for stanby
*/
void I2C_IsrCallback(void)
{
/* Gestione del wakeup per ricezione pacchetto su I2C */
#if defined(SLEEP_MODE_ENABLE)
  if (SystemMode == SYSTEM_SLEEP_MODE)
  {
  	/* IICD disponibile -> possiamo distinguere il primo byte di un qualsiasi pacchetto */
    /* if(IICD == 0xA5 ) */
  	
    NewSystemMode=SYSTEM_NORMAL_MODE;


#if defined(BUILD_RENESAS_36079)	
	/* Restart watchdog timer */
	WDT.TMWD.BYTE = 0xF;
	WDT.TCSRWD.BYTE = 0x50;
	WDT.TCSRWD.BYTE = 0x55;
	WDT.TCWD = 0xCC;
	WDT.TCSRWD.BYTE = 0xFF;
	WDT.TCSRWD.BYTE = 0xFF;
#endif      /* BUILD_RENESAS_36079 */	

	
    /* Viene forzato un reset freddo */
    ForceColdReset();
  }
#endif /* SLEEP_MODE_ENABLE */

}

/* Exported function */

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

