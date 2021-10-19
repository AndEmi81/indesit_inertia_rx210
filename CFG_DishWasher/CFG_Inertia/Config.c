/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	Arcadia2NewCertCfg Drivers configuration
@{
\par		Description
\n			This file contains drivers configuration
@}
\ingroup   	Inertia
*/
/**
\n
\par		Description
\n			[Brief description]
\n
\file		Config.c 
\ingroup	InertiaCfg
\date		Dec 2, 2009 
\version	01.00.00 
\author		Stefano Ughi, Roberto Fioravanti
\n
\n
\par\b	History:
\n 
\n 		01.04.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
*/

/* ******** */
/* Includes */
/* ******** */
#define __DRIVER_H
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL

#include "RTOS.h"
#include "IO_driver.h"
#if defined(CONFIG_PL_DRV_OCCRC)
#include "OCCRC_drv.h"
#else
#include "SWCRC_drv.h"
#endif
#include "LIB_Reset.h"

#include "OCTPM_drv.h"
#include "OCFLS_drv.h"
#include "OCWDT_drv.h"

#include "callback.h"

#include "LIB_ZeroCrossing.h"
#include "LIB_ClassBCheck.h"
#include "LIB_ClassBVar.h"
#include "LIB_trace.h"

#if defined(CONFIG_LIB_CLASS_B_RAM)
#include "IEC60335.h"
#endif

#include "Main.h"
#include "LIB_TaskMonitoring.h"

/* Product modules */



#if defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
#include "PM_AnalogFeedback.h"
#endif
#if defined(CONFIG_PM_CONSISTENCY)
#include "PM_Consistency.h"
#endif /* defined(CONFIG_PM_CONSISTENCY) */
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
#include "PM_FrequencyFeedbacks.h"
#endif /* defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT) */
#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
#include "PM_LinkedLoadsExp.h"
#endif /* defined(CONFIG_PM_LINKED_LOADS_SUPPORT) */
#if defined(CONFIG_PM_DIGITAL_OUTPUT_MULTI)
#include "DigitalOutputMulti.h"
#endif /* defined(CONFIG_PM_DIGITAL_OUTPUT_MULTI) */
#if defined(CONFIG_PM_LINEARPRESSURE_SENSOR)
#include "PM_LinearPressureSensor.h"
#endif /* CONFIG_PM_LINEARPRESSURE_SENSOR */
#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
#include "PM_PowerProfile.h"
#endif /* defined(CONFIG_PM_POWER_PROFILE_SUPPORT) */
#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)
#include "PM_ProfileLoadsPartializationExp.h"
#endif
#if defined(CONFIG_PM_DOORLOCK)
#include "PM_DoorLock.h"
#endif /* defined(CONFIG_PM_DOORLOCK) */
#if defined(CONFIG_PM_DRUMLOCK)
#include "PM_DrumLock.h"
#endif /* defined(CONFIG_PM_DRUMLOCK) */
#if defined(CONFIG_PM_ENERGY_METER_SW)
#include "PM_EnergyMeterSw.h"
#endif /* defined(CONFIG_PM_ENERGY_METER_SW) */

#if defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
#include "PM_TurbiditySensor.h"
#endif /* defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT) */

#if defined(CONFIG_PM_ALTERNATO)
#include "PM_Alternato.h"
#endif /* defined(CONFIG_PM_ALTERNATO) */


#include "Main.h"
#include "SwTimersCounters.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

extern uint32_t normVpeak;

TOOLS_TIMETRIGGER Trigger10ms;
TOOLS_TIMETRIGGER Trigger20ms;
TOOLS_TIMETRIGGER Trigger100ms;
TOOLS_TIMETRIGGER Trigger200ms;
/*!
Synchronization callback list, for the synchronization service
*/
struct SubscriberNode *syncSvcList =0;


/* Trigger to enable Feedback reading when ZC is missing */
static uint8_t		zcFail;

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/*!
Synchronization callback list, for the synchronization service
*/
static OS_RSEMA SemaExternalBoard;    /* Resource semaphore for ExternalBoard */
static OS_RSEMA SemaExternalInfo;    /* Resource semaphore for ExternalBoard */
#endif

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
static OS_RSEMA SemaPowerProfile;           /* Resource semaphore for Power Profile */
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

OS_ALARM_TICK ZcRefTick;





/*******************   EMS Configuration   *******************/
#if defined(CONFIG_PM_ENERGY_METER_SW)

	#if defined(CONFIG_RES_CALIBRATION_ENABLE) && CONFIG_NUM_RES_TO_CALIB
	const sEMS_ResCalWBoxOff_t resCalTab[CONFIG_NUM_RES_TO_CALIB] =
	{
		/* LoadID Res to calibrate */	/* Offset for the calib value in WBOX */
		{0x10, 0x00},
		{0x11, 0x01},


	} ;
	#endif /* defined(CONFIG_RES_CALIBRATION_ENABLE) */
#endif /* defined(CONFIG_PM_ENERGY_METER_SW) */







/*====================== Global Static Flash Configuration ==============================*/
/**
OCFLS configuration
Writable/erasable Flash areas definitions
@{
*/
/**
Areas
*/
BEGIN_MCU_FLASH_MAP(NUM_FLASH_BLOCKS)
{ APPL_WBOX_ADDRESS,      APPL_WBOX_END_ADDRESS },      /* WBox */
{ BACKUPAREA_START_ADDR,  BACKUPAREA_END_ADDR },        /* Backup and statistics */
{ APPL_SETTING_ADDRESS,   APPL_SETTING_END_ADDRESS },   /* Appliance Setting */
END_MCU_FLASH_MAP()
/**
Permissions
*/
BEGIN_MCU_FLASH_MAP_ERASE(NUM_FLASH_BLOCKS)
1,  /* Write Only Operations on WBox */
0,  /* Erase/Write Operations on Backup and statistics */
0   /* Erase/Write Operations on Appliance Setting */
END_MCU_FLASH_MAP_ERASE()
/*@}*/

/*==================== End Global Static Flash Configuration ============================*/

/*====================== Global Static DIO Configuration ==============================*/

#if defined(FREESCALE_MCF51_AG128)

/* 
Additional IO map. For internal use only.
*/
#if defined(ZC_IO_MODULE_MAP_SIZE)
BEGIN_IO_MAP(ZC_IO_MODULE_MAP_NAME, 1)
  PIN(A,0),  /* ZC validation */
END_IO_MAP()
#endif /* defined(ZC_IO_MODULE_MAP_SIZE) */


BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
  PIN_DUMMY, /* 1 - NC */
  PIN_DUMMY, /* 2 - Vcc */
  PIN(F,4),  /* 3 */
  PIN(F,5),  /* 4 */
  PIN(J,7),  /* 5 */
  PIN(F,0),  /* 6 */
  PIN(E,1),  /* 7 */
  PIN(E,0),  /* 8 */
  PIN(C,2),  /* 9 */
  PIN(D,7),  /* 10 */
  PIN(D,3),  /* 11 */
  PIN(F,1),  /* 12 */
  PIN(F,2),  /* 13 */
  PIN(F,3),  /* 14 */
  PIN_DUMMY, /* 15 - GND */
  PIN(A,0),  /* 16 */
  PIN_DUMMY, /* 17 - NC */
  PIN_DUMMY, /* 18 - GNDA */
  PIN(B,6),  /* 19 */
  PIN(D,5),  /* 20 */
  PIN(D,4),  /* 21 */
  PIN(G,3),  /* 22 */
  PIN(B,4),  /* 23 */
  PIN(B,3),  /* 24 */
  PIN(B,2),  /* 25 */
  PIN(A,7),  /* 26 */
  PIN_DUMMY, /* 27 - VccA */
  PIN(J,6),  /* 28 */
  PIN(F,6),  /* 29 */
  PIN(F,7),  /* 30 */
  PIN(C,6),  /* 31 */
  PIN_DUMMY, /* 32 - NC */
  PIN_DUMMY, /* 33 - NC */
  PIN_DUMMY, /* 34 - Vcc */
  PIN(E,3),  /* 35 */
  PIN(E,2),  /* 36 */
  PIN(B,0),  /* 37 */
  PIN(B,1),  /* 38 */
  PIN(C,5),  /* 39 */
  PIN(C,3),  /* 40 */
  PIN(B,5),  /* 41 */
  PIN(B,7),  /* 42 */
  PIN(A,6),  /* 43 */
  PIN(A,1),  /* 44 */
  PIN(D,6),  /* 45 */
  PIN(D,0),  /* 46 */
  PIN_DUMMY, /* 47 - GND */
  PIN_DUMMY, /* 48 - NC */
  PIN_DUMMY, /* 49 - NC */
  PIN_DUMMY, /* 50 - IRQ */
  PIN(A,2),  /* 51 */
  PIN(A,3),  /* 52 */
  PIN(A,4),  /* 53 */
  PIN(A,5),  /* 54 */
  PIN(D,2),  /* 55 */
  PIN(D,1),  /* 56 */
  PIN(C,1),  /* 57 */
  PIN(C,0),  /* 58 */
  PIN(E,7),  /* 59 */
  PIN(E,6),  /* 60 */
  PIN(E,5),  /* 61 */
  PIN(E,4),  /* 62 */
  PIN_DUMMY, /* 63 - RESET* */
  PIN_DUMMY  /* 64 - NC */ 
END_MODULE_IO_MAP()

#endif /* defined(FREESCALE_MCF51_AG128) */



#if defined(NXP_LPC12xx_1227)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
  PIN_DUMMY, /* 1 - NC */
  PIN_DUMMY, /* 2 - Vcc */
  PIN(2,4),  /* 3 */
  PIN(2,5),  /* 4 */
  PIN(2,6),  /* 5 */
  PIN(2,8),  /* 6 */
  PIN(0,1),  /* 7 */
  PIN(0,2),  /* 8 */
  PIN(0,0),  /* 9 */
  PIN(0,3),  /* 10 */
  PIN(0,4),  /* 11 */
  PIN(2,2),  /* 12 */
  PIN(2,3),  /* 13 */
  PIN(2,11),  /* 14 */
  PIN_DUMMY, /* 15 - GND */
  PIN(2,12),  /* 16 */
  PIN_DUMMY, /* 17 - NC */
  PIN_DUMMY, /* 18 - GNDA */
  PIN(1,0),  /* 19 */
  PIN(0,31),  /* 20 */
  PIN(0,30),  /* 21 */
  PIN(1,1),  /* 22 */
  PIN(1,2),  /* 23 */
  PIN(1,3),  /* 24 */
  PIN(1,4),  /* 25 */
  PIN(1,5),  /* 26 */
  PIN_DUMMY, /* 27 - VccA */
  PIN(0,29),  /* 28 */
  PIN(0,19),  /* 29 */
  PIN(2,13),  /* 30 */
  PIN(0,12),  /* 31 */
  PIN_DUMMY, /* 32 - NC */
  PIN_DUMMY, /* 33 - NC */
  PIN_DUMMY, /* 34 - Vcc */
  PIN(2,9),  /* 35 */
  PIN(2,10),  /* 36 */
  PIN(2,1),  /* 37 */
  PIN(2,0),  /* 38 */
  PIN(0,8),  /* 39 */
  PIN(0,9),  /* 40 */
  PIN(0,5),  /* 41 */
  PIN(0,6),  /* 42 */
  PIN(0,7),  /* 43 */
  PIN(0,18),  /* 44  SWCLK pin*/
  PIN(2,7),  /* 45 */
  PIN(0,21),  /* 46 */
  PIN_DUMMY, /* 47 - GND */
  PIN_DUMMY, /* 48 - NC */
  PIN_DUMMY, /* 49 - NC */
  PIN(1,6),  /* 50  */
  PIN(0,23),  /* 51 */
  PIN(0,28),  /* 52 */
  PIN(0,24),  /* 53 */
  PIN(0,20),  /* 54 */
  PIN(0,27),  /* 55 */
  PIN(0,22),  /* 56 */
  PIN(0,11),  /* 57 SDA pin duplicated on two locations*/
  PIN(0,10),  /* 58 SCL pin duplicated on two locations*/
  PIN(0,14),  /* 59 */
  PIN(0,17),  /* 60 */
  PIN(0,16),  /* 61 */
  PIN(0,15),  /* 62 */
  PIN_DUMMY, /* 63 - RESET* */
  PIN_DUMMY  /* 64 - NC */
END_MODULE_IO_MAP()
#endif /* defined(NXP_LPC12xx_1227)*/ 

#if defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)
/**************************************************/
/* mapping tra id timer il timer hw usato internamente dall'hal*/
const TMR_CNTAddressType TmrCntMap[NUM_HW_CNTR]={
		HAL_TMR_MTU0,
		HAL_TMR_MTU1,
		HAL_TMR_MTU2,
		HAL_TMR_MTU3,
		HAL_TMR_MTU4
		/* non gestire TMR_MTU5,*/
};/*end const*/


/*********************/
/* IO Map definitions*/
/*********************/
#if defined(CASTELLATED_MODULE_LP1_0241_01)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	PIN_DUMMY, /* 1 - NC */
	PIN_DUMMY, /* 2 - Vcc */
	PIN(3,2),  /* 3 -  GPIO0/TMR0 */
	PIN(2,6),  /* 4 -  GPIO1/TMR1 */
	PIN(1,4),  /* 5 -  GPIO2/TMR2 */
	PIN(B,3),  /* 6 -  GPIO3/TMR3 */
	PIN(E,2),  /* 7 -  GPIO8/RXD1 */
	PIN(E,1),  /* 8 -  GPIO9/TXD1 */
	PIN(H,2),  /* 9 -  GPIO20/KIB0*/
	PIN(1,5),  /* 10 - GPIO21/KIB1*/
	PIN(A,3),  /* 11 - GPIO22/KIB2*/
	PIN(0,3),  /* 12 - GPIO26 */
	PIN(3,1),  /* 13 - GPIO27 */
	PIN(3,0),  /* 14 - GPIO28 */
	PIN_DUMMY, /* 15 - GND */
	PIN_DUMMY, /* 16 - NC */
	PIN_DUMMY, /* 17 - NC */
	PIN_DUMMY, /* 18 - GNDA */
	PIN(E,5),  /* 19 - GI0/ATD_CH0 */
	PIN(E,0),  /* 20 - GI1/ATD_CH1 */
	PIN(4,6),  /* 21 - GI2/ATD_CH2 */
	PIN(4,4),  /* 22 - GI3/ATD_CH3 */
	PIN(4,3),  /* 23 - GI4/ATD_CH4 */
	PIN(4,2),  /* 24 - GI5/ATD_CH5 */
	PIN(4,1),  /* 25 - GI6/ATD_CH6 */
	PIN(4,0),  /* 26 - GI7/ATD_CH7 */
	PIN_DUMMY, /* 27 - VccA */
	PIN(2,7),  /* 28 - GPIO29 */
	PIN(H,3),  /* 29 - GPIO30 */
	PIN(H,0),  /* 30 - GPIO31 */
	PIN(5,4),  /* 31 - GPIO32 */
	PIN_DUMMY, /* 32 - NC */
	PIN_DUMMY, /* 33 - NC */
	PIN_DUMMY, /* 34 - Vcc */
	PIN(E,4),  /* 35 - GPIO4/TMR4 */
	PIN(C,6),  /* 36 - GPIO5/TMR5 */
	PIN(B,1),  /* 37 - GPIO6/TMR6 */
	PIN(A,0),  /* 38 - GPIO7/TMR7 */
	PIN(C,2),  /* 39 - GPIO10/RXD2 */
	PIN(C,3),  /* 40 - GPIO11/TXD2 */
	PIN(B,5),  /* 41 - GPIO23/KBI3 */
	PIN(B,0),  /* 42 - GPIO24/KBI4 */
	PIN(B,6),  /* 43 - GPIO25/KBI5 */
	PIN(C,4),  /* 44 - GPIO33 */
	PIN(3,5),  /* 45 - GPIO34 */
	PIN(B,7),  /* 46 - GPIO35/C1IN+- */
	PIN_DUMMY, /* 47 - GND */
	PIN_DUMMY, /* 48 - NC */
	PIN_DUMMY, /* 49 - NC */
	PIN(H,1),  /* 50 - HW_IRQ */
	PIN(A,1),  /* 51 - GPIO26/C1IN+-/C2IN+- */
	PIN(5,5),  /* 52 - GPIO37 */
	PIN(0,5),  /* 53 - GPIO38/C2IN+- */
	PIN(3,7),  /* 54 - GPIO39 */
	PIN(3,6),  /* 55 - GPIO40 */
	PIN(E,3),  /* 56 - GPIO41 */
	PIN(1,7),  /* 57 PM Pin*/
	PIN(1,6),  /* 58 PM Pin*/
	PIN(C,5),  /* 59 - GPIO42/SCLK */
	PIN(A,6),  /* 60 - GPIO43/MOSI */
	PIN(C,7),  /* 61 - GPIO44/MISO */
	PIN(A,4),  /* 62 - GPIO45/SS   */
	PIN_DUMMY, /* 63 - RESET* */
	PIN_DUMMY  /* 64 - NC */
END_MODULE_IO_MAP()
#elif defined(EDGE_MODULE_LP1_0258_01)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	PIN_DUMMY, /* 1  - NC     */
	PIN_DUMMY, /* 2  - Vdd    */
	PIN_DUMMY, /* 3  - Reset  */
	PIN_DUMMY, /* 4  - Vdd    */
	PIN(A,3),  /* 5  - HCOUT1 */
	PIN(A,1),  /* 6  - HCOUT0 */
	PIN(A,6),  /* 7  - HCOUT3 */
	PIN(A,4),  /* 8  - HCOUT2 */
	PIN(B,6),  /* 9  - HCOUT5 */
	PIN(B,5),  /* 10 - HCOUT4 */
	PIN(1,5),  /* 11 - GPIO12 */
	PIN(B,7),  /* 12 - HCOUT6 */
	PIN(H,2),  /* 13 - GPIO14 */
	PIN(H,3),  /* 14 - GPIO13 */
	PIN(5,5),  /* 15 - GPIO16 */
	PIN(H,0),  /* 16 - GPIO15 */
	PIN(C,5),  /* 17 - GPIO18 */
	PIN(5,4),  /* 18 - GPIO17 */
	PIN(2,6),  /* 19 - PWM1   */
	PIN(3,2),  /* 20 - PWM0   */
	PIN(B,3),  /* 21 - PWM3   */
	PIN(1,4),  /* 22 - PWM2   */
	PIN(C,6),  /* 23 - PWM5   */
	PIN(E,4),  /* 24 - PWM4   */
	PIN(A,0),  /* 25 - PWM7   */
	PIN(B,1),  /* 26 - PWM6   */
	PIN(E,2),  /* 27 - RXD1   */
	PIN(H,1),  /* 28 - HW_IRQ */
	PIN(E,1),  /* 29 - TXD1   */
	PIN_DUMMY, /* 30 - SDA    */
	PIN(C,2),  /* 31 - RXD2   */
	PIN_DUMMY, /* 32 - SCL    */
	PIN(C,3),  /* 33 - TXD2   */
	PIN_DUMMY, /* 34 - Vcc    */
	PIN_DUMMY, /* 35 - GNDA   */
	PIN_DUMMY, /* 36 - GND    */
	PIN(E,0),  /* 37 - ATD_CH1*/
	PIN(E,5),  /* 38 - ATD_CH0*/
	PIN(4,4),  /* 39 - ATD_CH3*/
	PIN(4,6),  /* 40 - ATD_CH2*/
	PIN(4,2),  /* 41 - ATD_CH5*/
	PIN(4,3),  /* 42 - ATD_CH4*/
	PIN(4,0),  /* 43 - ATD_CH7*/
	PIN(4,1),  /* 44 - ATD_CH6*/
END_MODULE_IO_MAP()
#elif defined(EDGE_MODULE_LP1_0258_02)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	/* format: Module pin no - function */
	PIN_DUMMY, /* 1  - GND    */
	PIN_DUMMY, /* 2  - VDD REG */
	PIN_DUMMY, /* 3  - Reset  */
	PIN_DUMMY, /* 4  - VDD    */
	PIN(1,5),  /* 5  - HCOUT1 */
	PIN(A,1),  /* 6  - HCOUT0 */
	PIN(B,7),  /* 7  - HCOUT3 */
	PIN(C,5),  /* 8  - HCOUT2 */
	PIN(B,6),  /* 9  - HCOUT5 */
	PIN(B,5),  /* 10 - HCOUT4 */
	PIN(A,3),  /* 11 - GPIO12 */
	PIN(E,3),  /* 12 - HCOUT6 */
	PIN(H,2),  /* 13 - GPIO14 */
	PIN(H,3),  /* 14 - GPIO13 */
	PIN(A,4),  /* 15 - GPIO16 */
	PIN(0,5),  /* 16 - GPIO15 */
	PIN(5,4),  /* 17 - GPIO18 */
	PIN(0,3),  /* 18 - GPIO17 */
	PIN(1,4),  /* 19 - PWM1   */
	PIN(B,3),  /* 20 - PWM0   */
	PIN(3,2),  /* 21 - PWM3   */
	PIN(2,6),  /* 22 - PWM2   */
	PIN(C,6),  /* 23 - PWM5   */
	PIN(E,4),  /* 24 - PWM4   */
	PIN(A,0),  /* 25 - PWM7   */
	PIN(B,1),  /* 26 - PWM6   */
	PIN(E,2),  /* 27 - RXD1   */
	PIN(H,1),  /* 28 - HW_IRQ */
	PIN(E,1),  /* 29 - TXD1   */
	PIN_DUMMY, /* 30 - SDA    */
	PIN(C,2),  /* 31 - RXD2   */
	PIN_DUMMY, /* 32 - SCL    */
	PIN(C,3),  /* 33 - TXD2   */
	PIN_DUMMY, /* 34 - Vcc    */
	PIN_DUMMY, /* 35 - GNDA   */
	PIN_DUMMY, /* 36 - GND    */
	PIN(4,1),  /* 37 - ATD_CH1*/
	PIN(E,5),  /* 38 - ATD_CH0*/
	PIN(4,2),  /* 39 - ATD_CH3*/
	PIN(4,6),  /* 40 - ATD_CH2*/
	PIN(E,0),  /* 41 - ATD_CH5*/
	PIN(4,3),  /* 42 - ATD_CH4*/
	PIN(4,0),  /* 43 - ATD_CH7*/
	PIN(4,4),  /* 44 - ATD_CH6*/
END_MODULE_IO_MAP()

#elif defined(EDGE_MODULE_LP1_0258_04)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
	/* format: Module pin no - function */
	PIN_DUMMY, /* 1  - GND    */
	PIN_DUMMY, /* 2  - VDD REG */
	PIN_DUMMY, /* 3  - Reset  */
	PIN_DUMMY, /* 4  - VDD    */
	PIN(A,3),  /* 5  - HCOUT1 */
	PIN(A,6),  /* 6  - HCOUT0 */
	PIN(C,5),  /* 7  - HCOUT3 */
	PIN(H,3),  /* 8  - HCOUT2 */
	PIN(B,6),  /* 9  - HCOUT5 */
	PIN(B,5),  /* 10 - HCOUT4 */
	PIN(B,0),  /* 11 - GPIO12 */
	PIN(B,7),  /* 12 - HCOUT6 */
	PIN(1,5),  /* 13 - GPIO14 */
	PIN(E,3),  /* 14 - GPIO13 */
	PIN(3,0),  /* 15 - GPIO16 */
	PIN(0,3),  /* 16 - GPIO15 */
	PIN(5,4),  /* 17 - GPIO18 */
	PIN(H,0),  /* 18 - GPIO17 */
	PIN(1,4),  /* 19 - PWM1   */
	PIN(B,3),  /* 20 - PWM0   */
	PIN(3,2),  /* 21 - PWM3   */
	PIN(2,6),  /* 22 - PWM2   */
	PIN(C,6),  /* 23 - PWM5   */
	PIN(E,4),  /* 24 - PWM4   */
	PIN(A,0),  /* 25 - PWM7   */
	PIN(B,1),  /* 26 - PWM6   */
	PIN(E,2),  /* 27 - RXD1   */
	PIN(H,1),  /* 28 - HW_IRQ */
	PIN(E,1),  /* 29 - TXD1   */
	PIN_DUMMY, /* 30 - SDA    */
	PIN(C,2),  /* 31 - RXD2   */
	PIN_DUMMY, /* 32 - SCL    */
	PIN(C,3),  /* 33 - TXD2   */
	PIN_DUMMY, /* 34 - Vcc    */
	PIN_DUMMY, /* 35 - GNDA   */
	PIN_DUMMY, /* 36 - GND    */
	PIN(4,2),  /* 37 - ATD_CH1*/
	PIN(E,5),  /* 38 - ATD_CH0*/
	PIN(4,4),  /* 39 - ATD_CH3*/
	PIN(4,3),  /* 40 - ATD_CH2*/
	PIN(4,1),  /* 41 - ATD_CH5*/
	PIN(4,0),  /* 42 - ATD_CH4*/
	PIN(4,6),  /* 43 - ATD_CH7*/
	PIN(E,0),  /* 44 - ATD_CH6*/
END_MODULE_IO_MAP()
#else
#error "IO Map definitions not supported yet."
#endif

/*********************/
/* Timers definitions*/
/*********************/
#if defined(EDGE_MODULE_LP1_0258_01) || defined(CASTELLATED_MODULE_LP1_0241_01)
/* configurazione dei canali dell'HAL usati.*/
const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH] = {
		{ HAL_CH_TGRC0, HAL_TMR_MTU0 }, /* TMR0 */
		{ HAL_CH_TGRA2, HAL_TMR_MTU2 },
		{ HAL_CH_TGRA3, HAL_TMR_MTU3 }, /* . */
		{ HAL_CH_TGRA0, HAL_TMR_MTU0 },	/* . */
		{ HAL_CH_TGRA1, HAL_TMR_MTU1 },	/* . */
		{ HAL_CH_TGRC3, HAL_TMR_MTU3 },
		{ HAL_CH_TGRC4, HAL_TMR_MTU4 },
		{ HAL_CH_TGRA4, HAL_TMR_MTU4 }, /* TMR7 */
		{ HAL_CH_TGRB0, HAL_TMR_MTU0 } /* prova contatore senza pin*/
};/*end const*/

/* Pin relativi alla mappa dei canali TmrChMap, ( corrispondenza sull'indice dell'array )*/
/* se ci sono dei canali che non hanno pin si deve inserire un pin dummy.*/
const TMR_MapType TmrChToPinMap[NUM_HW_CNTR_CH]={
						/*function, 64-pinLQFP, pin name, module */
#if defined(CONFIG_TMR0_ISR_SUPPORT)
        PIN(3,2),   /*MTIOC0C, Pin12, P32, PWM0*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR1_ISR_SUPPORT)
        PIN(2,6),   /*MTIOC2A, Pin16, P26, PWM1*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR2_ISR_SUPPORT)
        PIN(1,4),   /*MTIOC3A, Pin20, P14, PWM2*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR3_ISR_SUPPORT)
        PIN(B,3),   /*MTIOC0A, Pin36, PB3, PWM3*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR4_ISR_SUPPORT)
        PIN(E,4),   /*MTIOC1A, Pin47, PE4, PWM4*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR5_ISR_SUPPORT)
        PIN(C,6),   /*MTIOC3C, Pin28, PC6, PWM5*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR6_ISR_SUPPORT)
        PIN(B,1),   /*MTIOC4C, Pin37, PB1, PWM6*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR7_ISR_SUPPORT)
        PIN(A,0),   /*MTIOC4A, Pin45, PA0, PWM7*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
        PIN_DUMMY   /*Dummy PIN */
};
/** END TMR PIN MAP **/

#elif defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)
/*************************************************/
/* configurazione dei canali dell'HAL usati.*/
const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH] = {
	{ HAL_CH_TGRA0, HAL_TMR_MTU0 }, /* HAL_CH_TGRC0*/
	{ HAL_CH_TGRA3, HAL_TMR_MTU3 },
	{ HAL_CH_TGRA2, HAL_TMR_MTU2 },
	{ HAL_CH_TGRC0, HAL_TMR_MTU0 },
	{ HAL_CH_TGRA1, HAL_TMR_MTU1 },
	{ HAL_CH_TGRC3, HAL_TMR_MTU3 },
	{ HAL_CH_TGRC4, HAL_TMR_MTU4 },
	{ HAL_CH_TGRA4, HAL_TMR_MTU4 },
	{ HAL_CH_TGRB0, HAL_TMR_MTU0 } /* prova contatore senza pin*/
};/*end const*/
/*************************************************/


/*************************************************/
/* Pin relativi alla mappa dei canali TmrChMap, ( corrispondenza sull'indice dell'array )*/
/* se ci sono dei canali che non hanno pin si deve inserire un pin dummy.*/
const TMR_MapType TmrChToPinMap[NUM_HW_CNTR_CH]={

#if defined(CONFIG_TMR0_ISR_SUPPORT)
        PIN(B,3),   /*MTIOC0A, Pin36, PB3*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR1_ISR_SUPPORT)
        PIN(1,4),   /*MTIOC3A, Pin20, P14*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR2_ISR_SUPPORT)
        PIN(2,6),   /*MTIOC2A, Pin16, P26*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR3_ISR_SUPPORT)
		PIN(3,2),   /*MTIOC0C, Pin12, P32*/
#else
		PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR4_ISR_SUPPORT)
        PIN(E,4),   /*MTIOC1A, Pin47, PE4*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR5_ISR_SUPPORT)
        PIN(C,6),   /*MTIOC3C, Pin28, PC6*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR6_ISR_SUPPORT)
        PIN(B,1),   /*MTIOC4C, Pin37, PB1*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR7_ISR_SUPPORT)
        PIN(A,0),   /*MTIOC4A, Pin45, PA0*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
        PIN_DUMMY   /*Dummy PIN */
};
/** END TMR PIN MAP **/
#else
#error "Timers definitions not supported yet."
#endif



#if defined(TEST_MODULE)
BEGIN_MODULE_IO_MAP(IO_MODULE_MAP_SIZE)
  PIN_DUMMY, /* 1 - NC */
  PIN_DUMMY, /* 2 - Vcc */
  PIN(3,2),  /* 3 */
  PIN(2,6),  /* 4 */
  PIN(1,4),  /* 5 */
  PIN(H,1),  /* 6 */
  PIN(E,2),  /* 7 */
  PIN(E,1),  /* 8 */
  PIN(H,2),  /* 9 */
  PIN(B,7),  /* 10*/
  PIN(B,6),  /* 11 */
  PIN_DUMMY, /* 12 - NC */
  PIN_DUMMY, /* 13 - NC */
  PIN_DUMMY, /* 14 - NC*/
  PIN_DUMMY, /* 15 - GND */
  PIN_DUMMY, /* 16 - NC */
  PIN_DUMMY, /* 17 - NC */
  PIN_DUMMY, /* 18 - GNDA */
  PIN(E,5),  /* 19 */
  PIN(E,0),  /* 20 */
  PIN(4,6),  /* 21 */
  PIN(4,4),  /* 22 */
  PIN(4,3),  /* 23 */
  PIN(4,2),  /* 24 */
  PIN(4,1),  /* 25 */
  PIN(4,0),  /* 26 */
  PIN_DUMMY, /* 27 - VccA */
  PIN_DUMMY,  /* 28 - NC */
  PIN_DUMMY,  /* 29 - NC */
  PIN_DUMMY,  /* 30 - NC */
  PIN_DUMMY,  /* 31 - NC */
  PIN_DUMMY, /* 32 - NC */
  PIN_DUMMY, /* 33 - NC */
  PIN_DUMMY, /* 34 - Vcc */
  PIN(5,5),  /* 35 */
  PIN(C,6),  /* 36 */
  PIN(C,5),  /* 37 */
  PIN(A,0),  /* 38 */
  PIN(C,2),  /* 39 */
  PIN(C,3),  /* 40 */
  PIN(B,5),  /* 41 */
  PIN(B,0),  /* 42 */
  PIN(A,3),  /* 43 */
  PIN_DUMMY,  /* 44 - NC*/
  PIN_DUMMY,  /* 45 - NC */
  PIN_DUMMY,  /* 46 - NC */
  PIN_DUMMY, /* 47 - GND */
  PIN_DUMMY, /* 48 - NC */
  PIN_DUMMY, /* 49 - NC */
  PIN(1,5),  /* 50  */
  PIN(A,1),  /* 51 */
  PIN(B,1),  /* 52 */
  PIN(E,4),  /* 53 */
  PIN_DUMMY,  /* 54 - NC */
  PIN(3,6),  /* 55 */
  PIN(0,5),  /* 56 */
  PIN(1,7),  /* 57 PM Pin*/
  PIN(1,6),  /* 58 PM Pin*/
  PIN(C,5),  /* 59 */
  PIN(A,6),  /* 60 */
  PIN(C,7),  /* 61 */
  PIN(A,4),  /* 62 */
  PIN_DUMMY, /* 63 - RESET* */
  PIN_DUMMY  /* 64 - NC */
  END_MODULE_IO_MAP()

/**************************************************
mapping tra id timer il timer hw usato internamente dall'hal*/
const TMR_CNTAddressType TmrCntMap[NUM_HW_CNTR]={
                HAL_TMR_MTU0,
                HAL_TMR_MTU1,
                HAL_TMR_MTU2,
                HAL_TMR_MTU3,
                HAL_TMR_MTU4
                /* non gestire TMR_MTU5,*/
        };/*end const*/
  
/************************************************
 configurazione dei canali dell'HAL usati.*/
const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH] = {
                { HAL_CH_TGRC0, HAL_TMR_MTU0 }, /* HAL_CH_TGRC0*/
                { HAL_CH_TGRA2, HAL_TMR_MTU2 },
                { HAL_CH_TGRA3, HAL_TMR_MTU3 },
                { HAL_CH_TGRA0, HAL_TMR_MTU0 },
                { HAL_CH_TGRA1, HAL_TMR_MTU1 },
                { HAL_CH_TGRC3, HAL_TMR_MTU3 },
                { HAL_CH_TGRC4, HAL_TMR_MTU4 },
                { HAL_CH_TGRA4, HAL_TMR_MTU4 },
                { HAL_CH_TGRB0, HAL_TMR_MTU0 } /* prova contatore senza pin*/
        };/*end const*/



/*************************************************/
      
/************************************************
 Pin relativi alla mappa dei canali TmrChMap, ( corrispondenza sull'indice dell'array )
 se ci sono dei canali che non hanno pin si deve inserire un pin dummy.*/
const TMR_MapType TmrChToPinMap[NUM_HW_CNTR_CH]={
#if defined(CONFIG_TMR0_ISR_SUPPORT)
        PIN(3,2),   /*MTIOC0C, Pin12, P32*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR1_ISR_SUPPORT)
        PIN(2,6),   /*MTIOC2A, Pin16, P26*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR2_ISR_SUPPORT)
        PIN(1,4),   /*MTIOC3A, Pin20, P14*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR3_ISR_SUPPORT)
        PIN(B,3),   /*MTIOC0A, Pin36, PB3*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR4_ISR_SUPPORT)
        PIN(E,4),   /*MTIOC1A, Pin47, PE4*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR5_ISR_SUPPORT)
        PIN(C,6),   /*MTIOC3C, Pin28, PC6*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR6_ISR_SUPPORT)
        PIN(B,1),   /*MTIOC4C, Pin37, PB1*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
#if defined(CONFIG_TMR7_ISR_SUPPORT)
        PIN(A,0),   /*MTIOC4A, Pin45, PA0*/
#else
        PIN_DUMMY,   /*Dummy PIN */
#endif
        PIN_DUMMY   /*MTIOC4A, Pin45, PA0*/
};
/** END TMR PIN MAP **/  
#endif  

#endif /*defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)*/
/*******************   Start TMR Configuration   *******************/
/* Hw Counter id 0 Configuration */
const IO_HwCntrCfgType ZcHwCntr_cfg = {
        HW_TMR_ID_0,
        TCNT_CLEARING_OVERFLOW,                 		/* gestisce il massimo periodo senza occupare altri canali*/
        TMR_PSC_VALUE___64,                             /* / 64  divisore prescaler*/
        TMR_CLK_SOURCE_IS_BUS_CLOCK,    				/* puo' cambiare?*/
        TMR_COUNT_UP,
        50*64,                                          /*  dipende dal clock e dal prescaler*/
        0xFFFF,                                          /* periodo in tick*/
        0
};

/* Hw Counter id 1 Configuration */
const IO_HwCntrCfgType HwCntr1_cfg = {
        HW_TMR_ID_1,
        TCNT_CLEARING_OVERFLOW,
        TMR_PSC_VALUE___64,
        TMR_CLK_SOURCE_IS_BUS_CLOCK,
        TMR_COUNT_UP,
        50*64,
        0xFFFF,
        0
};

/* Hw Counter id 3 Configuration */
const IO_HwCntrCfgType HwCntr3_cfg = {
        HW_TMR_ID_3,
        TCNT_CLEARING_OVERFLOW,
        TMR_PSC_VALUE___64,
        TMR_CLK_SOURCE_IS_BUS_CLOCK,
        TMR_COUNT_UP,
        50*64,
        0xFFFF,
        0
};

/* Hw Counter id 4 Configuration : Turbity sensor*/
const IO_HwCntrCfgType HwCntr4_cfg = {
        HW_TMR_ID_4,
        TCNT_CLEARING_TGRD,
        TMR_PSC_VALUE___64,
        TMR_CLK_SOURCE_IS_BUS_CLOCK,
        TMR_COUNT_UP,
        50*64,
        0x200,
        0
};

/* Hw Counter id 0 Configuration */
/*const IO_HwCntrCfgType HwCntr0_cfg = {
HW_TMR_ID_1,
TMR_CLEAR_SOURCE_IS_INTERNAL,
TMR_ID1_PSC_VALUE,
TMR_CLK_SOURCE_IS_BUS_CLOCK,
TMR_COUNT_UP,
TMR_ID0_TICK_RES,
TMR_ID0_PERIOD_VALUE
};*/
/* Hw Counter id 1 Configuration */
/*const IO_HwCntrCfgType HwCntr1_cfg = {
HW_TMR_ID_1,

#if defined(FREESCALE_MCF51_AG128)
TMR_CLEAR_SOURCE_IS_INTERNAL,
#endif

#if defined(NXP_LPC12xx_1227)
TMR_CLEAR_SOURCE_IS_OC_CH0,
#endif

TMR_ID1_PSC_VALUE,
TMR_CLK_SOURCE_IS_BUS_CLOCK,
TMR_COUNT_UP,
TMR_ID1_TICK_RES,
TMR_ID1_PERIOD_VALUE,
1
};*/

#if defined(LIB_SETTINGFILE_CUSTOM_FAULT_PRIORITY)

static const uint8_t CSTM_ApplFaultPriorities_Data[] =
{
	18,0x01,	9,0xFF,
	 1,0x81,	2,0xFF,
	 1,0x01,	1,0x02,
	 1,0x03,	1,0x04,
	 1,0x82,	1,0x83,
	 1,0x84,	1,0x70,
	 1,0x71,	1,0x31,
	 1,0x32,	5,0x02,
	 5,0x03,	1,0x33,
	 1,0x23,	1,0x63,
	 1,0x41,	1,0x51,
	16,0x01,

	 1,  2,  3,  4,  5,  6,  7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23

};

static const uint8_t CSTM_PltFaultPriorities_Data[] =
{
	 9, 0x83,	 3, 0xFF,
	 5, 0xFF,	 1, 0x14,
	15, 0x02,	 1, 0x04,
	 2, 0xFF,	 8, 0x01,
	11, 0x01,	 1, 0x24,
	13, 0xFF,	15, 0x01,
	 1, 0x14,	 1, 0x51,
	 6, 0xFF,	 1, 0x02,
	 1, 0x01,	 1, 0xFF,
	 9, 0xFF,

	  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
	 11, 12, 13, 14, 15, 16, 17, 18, 19, 0

};

const Loader_Struct CSTM_ApplFaultPriorities =
{
		(uint8_t *) CSTM_ApplFaultPriorities_Data, sizeof(CSTM_ApplFaultPriorities_Data)/2
};

const Loader_Struct CSTM_PltFaultPriorities =
{
		(uint8_t *) CSTM_PltFaultPriorities_Data, sizeof(CSTM_PltFaultPriorities_Data)/2
};

#endif



#if defined(CONFIG_LIB_CLASS_B_FLASH)
extern char FlashCheck1;
extern char FlashCheck1_End;
#if defined (RN_RX200_R5F52Z0_5)
/* RX 2Z0 128K has rodata in Data Flash */
extern char FlashCheck2;
extern char FlashCheck2_End;
#endif /* defined (RN_RX200_R5F52Z0) */

/*      ClassB Flash Check area - it contains all flash blocks (Code + constant area) under check
        NUM_FLASH_CBOARD_TEST_BLOCKS is defined in Config.h! */

const MemSegment AreaFlashCheckTable[NUM_FLASH_CBOARD_TEST_BLOCKS]={
{ (uint8_t*)&FlashCheck1 , (uint8_t*)&FlashCheck1_End },
#if defined (RN_RX200_R5F52Z0_5)
/* RX 2Z0 128K has rodata in Data Flash */
{ (uint8_t*)&FlashCheck2 , (uint8_t*)&FlashCheck2_End },
#endif /* defined (RN_RX200_R5F52Z0) */
};
#endif

#if defined(CONFIG_LIB_CLASS_B_RAM)
#if defined(RN_RX200)

extern char _CLASSB__NOINIT_START__ ,_CLASSB__1_RAM_START__;
extern char _CLASSB__NOINIT_END__,  _CLASSB__1_RAM_END__;
extern char _CLASSB__3_RAM_START__;
extern char _CLASSB__3_RAM_END__;
extern char ustack_start, istack_start;
extern char ustack, istack;




/*	ClassB RAM POST and BIST - This table contains all RAM areas including stacks
	Each stack must be a in single area.
	The last column indicates the API used to tes the ares.
	The first block is always non destructive test.
	The others : POST (Pre-Operation self test) is destructive to save time at startup; BIST (Built-In Self Test) is non destructive.
	NUM_STACK_CBOARD_TEST_BLOCKS is defined in Config.h!
	*/
const ClassB_RamTestDescriptorType	RamAreaTestTable[NUM_RAM_CBOARD_TEST_BLOCKS]=
{
	{(uint32_t)&_CLASSB__NOINIT_START__,(uint32_t)&_CLASSB__NOINIT_END__, 	IEC60335_RamMarchC},
	{(uint32_t)&_CLASSB__1_RAM_START__, (uint32_t)&_CLASSB__1_RAM_END__, 	IEC60335_RamMarchC},
	{(uint32_t)&_CLASSB__3_RAM_START__, (uint32_t)&_CLASSB__3_RAM_END__,    IEC60335_RamMarchC},
	{(uint32_t)&ustack_start, 			(uint32_t)&ustack -1,				IEC60335_RamMarchC_UserStack},
	{(uint32_t)&istack_start, 			(uint32_t)&istack -1,				IEC60335_RamMarchC_IntStack}
};

#endif /* defined(RN_RX200) */
#endif

/* platform SW counters */

SWTC_SCounterVariableType Time2Death_Counter;
SWTC_CounterConfigurationType CfgTime2Death_Counter = { SWTC_CFG_INIT_EX(FALSE, FALSE, 3, SWTC_CFG_SIZE_S) };

SWTC_SCounterVariableType Time_Since_Last_Reset_Counter;
SWTC_CounterConfigurationType CfgTime_Since_Last_Reset_Counter = { SWTC_CFG_INIT_EX(FALSE, TRUE,6,SWTC_CFG_SIZE_S) };

/* Static */



/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Initilializes specific-application drivers in Normal mode.
*/
void SetupHw(void)
{       
    IO_ErrorType Err;

    /* Init Hw Counter */
    OCHWCNTR_DeInitSync(ZcHwCntr_cfg.hwCntrlId);
    Err = OCHWCNTR_InitSync(&ZcHwCntr_cfg);
    PLT_ASSERT(Err == IO_E_OK);

	OCHWCNTR_DeInitSync(HwCntr1_cfg.hwCntrlId);
	Err = OCHWCNTR_InitSync(&HwCntr1_cfg);
	PLT_ASSERT(Err == IO_E_OK);

	OCHWCNTR_DeInitSync(HwCntr3_cfg.hwCntrlId);
    Err = OCHWCNTR_InitSync(&HwCntr3_cfg);
    PLT_ASSERT(Err == IO_E_OK);

	OCHWCNTR_DeInitSync(HwCntr4_cfg.hwCntrlId);
	Err = OCHWCNTR_InitSync(&HwCntr4_cfg);
    PLT_ASSERT(Err == IO_E_OK);

	/* Start Counting */
	Err = OCHWCNTR_Enable(ZcHwCntr_cfg.hwCntrlId, TMR_CNT_ENABLE); 
	PLT_ASSERT(Err == IO_E_OK);

	Err = OCHWCNTR_Enable(HwCntr1_cfg.hwCntrlId, TMR_CNT_ENABLE);
	PLT_ASSERT(Err == IO_E_OK);

	Err = OCHWCNTR_Enable(HwCntr3_cfg.hwCntrlId, TMR_CNT_ENABLE);
	PLT_ASSERT(Err == IO_E_OK);

	Err = OCHWCNTR_Enable(HwCntr4_cfg.hwCntrlId, TMR_CNT_ENABLE);
    PLT_ASSERT(Err == IO_E_OK);

	if ( SYSTEM_NORMAL_MODE == LIB_Reset_GetSystemMode() )
	{
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
		PM_FrequencyFeedbacks_Init();
#endif
#if defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
		PM_AnalogFeedback_Init();
#endif /* defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)*/

		if ( TRUE == LIB_Reset_GetIsColdReset() )
		{
#if defined(CONFIG_PM_CONSISTENCY)
			PM_Consistency_Init();
#endif /* defined(CONFIG_PM_CONSISTENCY) */

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
			PM_LinkedLoads_Init();
#endif /* defined(CONFIG_PM_LINKED_LOADS_SUPPORT) */

#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
			PM_PowerProfile_Init();
#endif /* defined(CONFIG_PM_POWER_PROFILE_SUPPORT) */

#if defined(CONFIG_PM_ENERGY_METER_SW)
			EM_LoadConfigRead();
#endif /* __EM_SOFTWARE_H */

#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)
			PM_PLP_Init();
#endif /* defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)*/
		}
		else
		{
#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)
			PM_PLP_Init_OnWarmReset();
#endif /* defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)*/
		}
	}
}


/* 
Called on Startup of the Main framework
As tasks are not created at this time avoid using OS services here.
*/
void Main_OnSupervisorStart (void)
{
    /* 	Common driver initialization */
#if defined(HAL_OCDMA_SUPPORT)
	( void ) OCDMA_InitSync();      /* DMA driver used by I2C/Uart drivers */
#endif      
	( void ) OCFLS_InitSync();      /* FLS driver could be used by CALLBACK_Main_InitTask to invalidate backup area. */


#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
	LIB_TaskMonitorInit();
#endif

	/*triggers*/
	Tools_CreateTimeTrigger ( &Trigger10ms, 10, TRUE );
	Tools_CreateTimeTrigger ( &Trigger20ms, 20, TRUE );
	Tools_CreateTimeTrigger ( &Trigger100ms, 100, TRUE );
	Tools_CreateTimeTrigger ( &Trigger200ms, 200, TRUE );

	zcFail = TRUE;
}


#if defined(FREESCALE_MCF51_AG128)
/** Zerocrossing on IC */
void ICUZC_Isr(void)
{
    HWTMRCH_ISR((IO_ChannelType)TMR_CH_ID_6);
}

#endif /* defined(FREESCALE_MCF51_AG128) */


#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/* external board */
/*lint -e534 TODO description */
void UseExternalBoard(void) 
{
  OS_Use(&SemaExternalBoard);
}

void UnuseExternalBoard(void)
{
	OS_Unuse(&SemaExternalBoard);
}

void UseExternalInfo(void)
{
	OS_Use(&SemaExternalInfo);
}

void UnuseExternalInfo(void) 
{
	OS_Unuse(&SemaExternalInfo);
}
/*lint +e534 TODO description*/
#endif /*defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)*/
void InitSema(void)
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
    OS_CREATERSEMA(&SemaExternalBoard);
    OS_CREATERSEMA(&SemaExternalInfo);
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
    OS_CREATERSEMA(&SemaPowerProfile);
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
}

/** Main Supervisor body */
void Main_SupervisorBody( SysMode_Type sysMode, bool_t ZC_Ok, int t0)
{

	SupervisorInfoType 	spInfo;
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && 	defined(CONFIG_PM_DOORLOCK)
	static uint8_t		doorLocked = FALSE;
#endif
	spInfo.systemMode	= sysMode;
	spInfo.zcGood		= ZC_Ok;
	spInfo.tick			= t0;

	if (Main_IsTimeForColdReset())
	{
		Backup_SystemBackup(sysMode);
	}

	CALLBACK_Main_GP(spInfo);

	if (sysMode == SYSTEM_NORMAL_MODE)
	{
/* doorlock-based UART management*/
#if defined(CONFIG_PM_DOORLOCK) && defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
		uint8_t doorstatus =goodZCSignal();
		bool_t door = ((doorstatus) || (doorLocked && (PM_DoorLock_GetLockStatusFiltered() != BPGEN_FEED_OPEN)))? TRUE:FALSE;

		if(door ^ doorLocked)
		{
			/*  transition to/from door locked*/
			doorLocked = door;

			if(door)
			{
				CP_UART_EnableComm();
			}
		}

		if(!door)
		{
			CP_UART_DisableComm();
		}
#endif

		LIB_ZC_CheckIfZC_Is_Good();

		if(0 != Tools_IsTriggered(&Trigger10ms))
		{
			Main_AnalogRefMonitor();
			Main_ZcSyncMonitor();

#if defined(CONFIG_PM_DIGITAL_OUTPUT_MULTI)
			DigDelayedMulti_Manager();
#endif /*defined(CONFIG_PM_DIGITAL_OUTPUT_MULTI)*/
#if defined(CONFIG_PM_DOORLOCK)
			PM_DoorLock_Manager(EV_10MS);
#endif
#if defined(CONFIG_PM_DRUMLOCK)
			PM_DrumLock_Manager(EV_10MS);
#endif
#ifdef CONFIG_PM_CONSISTENCY
                PM_Consistency_Manager();
#endif /* CONFIG_PM_CONSISTENCY */
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
			PM_FrequencyFeedbacks_Manager();
#endif /* defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT) */

#if defined(CONFIG_PM_LINEARPRESSURE_SENSOR)
			PM_LPS_Manager();
#endif /* CONFIG_PM_LINEARPRESSURE_SENSOR */

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
			PM_LinkedLoads_Manager();
#endif /*	defined(CONFIG_PM_LINKED_LOADS_SUPPORT) */

#if defined( CONFIG_PM_MOTORGATE )
            {
              extern void PM_UM_MotorManager(void);
              PM_UM_MotorManager();
            }
#endif


#if	defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
            PM_TurbiditySensor_Manager();
#endif	/* defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT) */

#if defined(CONFIG_PM_ALTERNATO)
            PM_ALT_EngineDrv();
#endif /* defined(CONFIG_PM_ALTERNATO) */

			if(FALSE == ZC_Ok)
			{
				/* halfwave in us */
				uint16_t hw =LIB_ZC_GetNominalHalfWave();

				if(hw == 0)
				{
					/* invalid hw or not valid at least once: force a default value.
					 * This is needed to allow feedback reading after power on if ZC is not
					 * available (Eg. unlocked doorlock => no ZC)*/
					hw=10000;
				}

				if(zcFail)
				{
					Main_ActivateFeedbacks_AsyncRead(hw/2);
					Main_ActivateFeedbacksP_AsyncRead(hw/2);
					zcFail = FALSE;
				}
#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
#if defined(CONFIG_LIB_TM_OUTPUT)
				LIB_SetTask(OUTPUT_TASK_ID, 0);
#endif
#if defined(CONFIG_LIB_TM_LIVE)
				LIB_SetTask(LIVE_TASK_ID, 0);
#endif
#if defined(CONFIG_MAIN_SEQUENCER_TASK_TASKMONITOR_ENABLE)
				LIB_SetTask(SEQUENCER_TASK_ID, 0);
#endif
#endif
			}
			else
			{
				zcFail=TRUE;

#if defined(CONFIG_LIB_CLASS_B_PERIODIC_CHECK)
#if defined(CONFIG_LIB_TM_OUTPUT)
				LIB_SetTask(OUTPUT_TASK_ID, 1);
#endif
#if defined(CONFIG_LIB_TM_LIVE)
				LIB_SetTask(LIVE_TASK_ID, 1);
#endif
#if defined(CONFIG_MAIN_SEQUENCER_TASK_TASKMONITOR_ENABLE)
				LIB_SetTask(SEQUENCER_TASK_ID, 1);
#endif
#endif

			}
			/*lint -e522 -e534 */
			(void) Main_Feedbacks_DelayOnZC_IsOK();
			(void) Main_FeedbacksP_DelayOnZC_IsOK();
			(void) Main_Feedbacks_Period_IsOK();
			(void) Main_FeedbacksP_Period_IsOK();

			(void) Main_Live_DelayOnZC_IsRangeOK();
			/*lint +e522 +e534  TODO description*/

#if defined( CONFIG_PL_FOR_ATTILIO )
            if( goodZCSignal()){
              TRACE_TO_PIN(1, LED_YELLOW);
            }else{
              TRACE_TO_PIN(0, LED_YELLOW);
            }
#endif
#if defined(CONFIG_LIB_CLASS_B_CLOCK) && defined(CONFIG_LIB_CLASS_B_CLOCK_NPX)
{
			/* TODO : remove extern*/
			extern void IEC60335_Clocktest_TimerIntHandler();
			IEC60335_Clocktest_TimerIntHandler(); /* agganciato al timer della cpu*/
}
#endif /* defined(CONFIG_LIB_CLASS_B_CLOCK) && defined(LIB_CLASS_B_CLOCK_NPX)  */

#ifdef CONFIG_LIB_CLASS_B_PERIODIC_CHECK
            LIB_TaskMonitor();
#endif /* CONFIG_LIB_CLASS_B_PERIODIC_CHECK*/

#ifdef CONFIG_LIB_CLASS_B_RAM
            if(!goodZCSignal())
			{
            	if((getZCStability() == 0xFF) )
	            { 
					/* con zc assente e stabile altrimenti potrebbe intralciare il driver del motore a collettore */
	            	LIB_ClassB_RAM_BIST();
	            }
			}
#endif /* CONFIG_LIB_CLASS_B_RAM*/



#ifdef CONFIG_LIB_CLASS_B_FLASH
            CheckFlash_Manager(CONFIG_LIB_CLASS_B_FLASH_CHUNKSIZE, FALSE);
#endif /* CONFIG_LIB_CLASS_B_FLASH*/

		}
		if(0 != Tools_IsTriggered(&Trigger20ms))
		{
			LIB_DIO_DigInputManager();
			if(FALSE == ZC_Ok)
			{
				Main_ActivateAnalogInputs();
			}

#if defined(CONFIG_PM_ENERGY_METER_SW)
			PowerMeter_SetNormInst_V(normVpeak);
#endif
		}

		if(0 != Tools_IsTriggered(&Trigger100ms))
		{
#ifdef CONFIG_LIB_CLASS_B_RAM
			if ( 1 == Main_IsRestoreCompleted() )
			{
				LIB_ClassB_Keywords_BIST();
			}
#endif /* CONFIG_LIB_CLASS_B_RAM*/

#if defined(CONFIG_PM_ENERGY_METER_SW)
			PowerMeter_Accum();
#endif /* CONFIG_PM_ENERGY_METER_SW */

		}

        if(0 != Tools_IsTriggered(&Trigger200ms))
        {
#if defined( CONFIG_PL_FOR_ATTILIO )
          static uint8_t toggle_led;
          TRACE_TO_PIN(toggle_led, LED_GREEN);
          toggle_led = !toggle_led;
#endif

#ifdef CONFIG_FEEDBACK_ENABLE
#if defined( CONFIG_PL_FOR_ATTILIO )
			if( LIB_FDB_IsFeedbackReady(0) )
			{
				TRACE_TO_PIN(toggle_led, LED_RED);
			}
#endif	/* (CONFIG_PL_FOR_ATTILIO) */
#endif	/* CONFIG_FEEDBACK_ENABLE */

#if defined(CONFIG_LIB_CLASS_B_CLOCK) && !defined(CONFIG_LIB_CLASS_B_CLOCK_HW)
            {
            	static int cnt=0;
            	cnt++;
                if( cnt > 10 )
				{ /* ogni 2 sec*/
					cnt=0;
					CheckClock_Manager(0);
                }
            }
#endif /* CONFIG_LIB_CLASS_B_CLOCK*/

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
            LIB_Reset_SafetyCheck();
#endif
        }

    }

#if defined(CONFIG_LIB_CLASS_B_IRQ_CHECK)
    LIB_IRQMonitor();
#endif

}

bool_t Main_IsRemoteEnabled(void)
{
	/* Sample code to use a SW Counter*/

	bool_t retVal = FALSE;

	 if ( (!(SWTC_IsStopped((SWTC_CountersType)TIME2DEATH_COUNTER_ID))) &&
		 (!SWTC_IsExpired((SWTC_CountersType)TIME2DEATH_COUNTER_ID)) )
	 {
	 	 SWTC_Run((SWTC_CountersType)TIME2DEATH_COUNTER_ID);
		retVal = TRUE;
	 }

	 return retVal;
}

void Main_EnableRemoteTimeout(void)
{
	/*Sample code to use a SW Counter */

	SWTC_SetSValueAndRun(TIME2DEATH_COUNTER_ID, PROGRAMMING_MODE_TIMEOUT_s);
}

/**
Returns if it's time to generate cold reset
\return bool_t
\author 	Roberto Fioravanti
\date       22/04/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t Main_IsTimeForColdReset ( void )
{
	bool_t retVal = FALSE;
	uint8_t sysMode = LIB_Reset_GetSystemMode();

	if ((sysMode == SYSTEM_PROGRAMMING_MODE)
		|| (TRUE == CALLBACK_EnableCheckColdResetTimeout(sysMode)))
	{
		if ( !SWTC_IsStopped ( (SWTC_CountersType)TIME2DEATH_COUNTER_ID )
				&&  ( SWTC_IsExpired ( (SWTC_CountersType)TIME2DEATH_COUNTER_ID ) ) )
		{
			retVal = TRUE;
		}
	}

	return retVal;
}


/** Application Body */
void Main_ApplicationTask_Body(void)
{
	bool_t applRunning=FALSE;

	if ( CALLBACK_Main_ApplicationTask_Ctrl() )
	{
	  void * pRemoteLoadsControlData = 0;
	  int len=0;

	  applRunning = TRUE;

	  Main_RemoteLoadsControl_GetCommand(&pRemoteLoadsControlData, &len);
	  if(pRemoteLoadsControlData && len)
	  {
		  /* Enter / Refresh Remote Control enabled */
		  LIB_Reset_EnterRemoteControl(0);
		  CALLBACK_RemoteLoadsControl_BeforeProcess(pRemoteLoadsControlData, &len);
		  Main_RemoteLoadsControl_ProcessCommand((uint8_t*)pRemoteLoadsControlData);
		  CALLBACK_RemoteLoadsControl_AfterProcess(pRemoteLoadsControlData, &len);
		  Main_RemoteLoadsControl_PurgeCommand();
	  }
	  CALLBACK_Main_ApplicationTask_Body();
	}

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

	LIB_IOM_LoadsRequestManager();

	if (applRunning)
	{
		CALLBACK_Main_ApplBody_PostLoads();
	}
#endif

}



#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
void UsePowerProfile(void)
{
	OS_Use(&SemaPowerProfile);
}
void UnusePowerProfile(void)
{
	OS_Unuse(&SemaPowerProfile);
}
#endif /*CONFIG_PM_POWER_PROFILE_SUPPORT*/

#ifdef CONFIG_PL_DEBUG
void pltErrorHandler(int err){
	while(1)
	{
		err=err;
	}
}
#endif

/** Zerocrossing callback
\param[in]  edge            edge type, or halfwave type
\param[in]  usHalfWave      halfwave measure in us

It is called on ZC event, if ZC is valid
*/
void OnZc_OK(LIB_ZcEdgeType edge, uint16_t usHalfWave)
{
	/* halfwave in us */
	uint16_t hw =LIB_ZC_GetNominalHalfWave();

    /* Presa unicamente la configurazione ANALOG_ALWAYS = TRUE */
    if ( VPEAK_EDGE == edge )
    {
        /* lancia misura Vpeak a HalfWavePosUsec/2 */
        /*RF depends on      Main_ScheduleMeasureLive(usHalfWave/2, (OS_U32)ZcRefTick);*/

        /* set sequencer profile for Live */
        Main_MeasureLive_SetSequencerProfile ( Main_GetSequencerMiddleEvent());
    }
    else
    {
        /* Disable sequencer profile for Live measurement */
        Main_MeasureLive_SetSequencerProfile ( 0 );

        /* analog channels measurement at HalfWaveNegUsec/4 */
        Main_ScheduleAnalogMeasure((OS_U32)3000, (OS_U32)LIB_ZC_Control_Var.ZcRefTick);
    }

#if 0		/*RF depends on*/
    if ( LIB_ZC_PeriodicAlarm() == TRUE)
    {
        LIB_ZC_InitPeriodicEvents(edge);

        partialWidthUsec = LIB_ZC_GetPartialWidth();
        /* periodic Alarm PartialWidthUsec @ PartialWidthUsec - ANTICIPO_PARZIALIZZATI, */
        Main_ScheduleOutputs(partialWidthUsec, partialWidthUsec, (N_LEVEL-1), (OS_U32)ZcRefTick);
    }
    else
    {
        /* todo
           PLP Init
           Attiva il task Task_Partialization
         End todo*/
    }
#else
    /* launch Sequencer for Activities */
    Main_PeriodicScheduleSequencer(0,                                                   /* initial offset */
                                   LIB_ZC_GetSemiPeriod()/Main_GetSequencerLength(),    /* period */
                                   (OS_U32)LIB_ZC_Control_Var.ZcRefTick);                                  /* absolute ref */
#endif

	if(hw != 0)
	{
	    Main_ScheduleFeedbacks( (OS_U32)(2500), (OS_U32)LIB_ZC_Control_Var.ZcRefTick, hw);
	    Main_ScheduleFeedbacksP((OS_U32)(5800), (OS_U32)LIB_ZC_Control_Var.ZcRefTick, hw);
	    zcFail=TRUE;
	}

#if defined(CONFIG_PM_DOORLOCK)
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
    {
    	ZcEvent.type = EV_ZC;
    	ZcEvent.data = (uint16_t)edge;
		Main_ActivateDispatcher();
    }
#else
    {
    	NotifyDataEvent evt;
    	evt.type = EV_ZC;
    	evt.data = (uint16_t)edge;
    	PM_DoorLock_Manager_OnZc(&evt);
    }

#endif
#endif /* CONFIG_PM_DOORLOCK*/

#if defined( CONFIG_LIB_CLASS_B_RAM )
    if(goodZCSignal())
    { /* Solo con ZC Good altrimenti va in conflitto con il check nel task supervisor */
        LIB_ClassB_RAM_BIST();
    }

#endif

}


/** Zerocrossing callback
\param[in]  edge            edge type, or halfwave type
\param[in]  usHalfWave      halfwave measure in us

It is called on ZC event, if ZC is not valid
*/
void OnZc_KO(LIB_ZcEdgeType edge, uint16_t usHalfWave)
{
    (void) edge;
    (void) usHalfWave;
}



void *const PltBackupAddrTab[] = {
	(void *) &Backup_Source,				/* Deve essere la prima variabile tra quelle da backuppare */
#ifdef CONFIG_PM_ENERGY_METER_SW
	&normVpeak,
	&b_ui32EnergyCounterWh,
  &b_ui16EnergyCounterWhAcc,
#endif /* CONFIG_PM_ENERGY_METER_SW */
  (void *) BCK_ADDR_END_TAB
};

const uint8_t PltBackupDimTab[] = {
	sizeof(Backup_Source),
#ifdef CONFIG_PM_ENERGY_METER_SW
	sizeof(normVpeak),
  sizeof(b_ui32EnergyCounterWh),
  sizeof(b_ui16EnergyCounterWhAcc),
#endif /* CONFIG_PM_ENERGY_METER_SW */
	/*0 */ /**< end table non necessario*/
};
/* *********** */
/* End of file */
/* *********** */
