/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Watchdog configuration
\n			Watchdog configuration
\n
\file		WatchDogConfig.h 
\ingroup	WizardPrj 
\date		Nov 3, 2009 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\par\b	History:
\n
\n 		01.04.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

#ifndef __WDTPLTFCFG_H
#define __WDTPLTFCFG_H

/* ********** */
/* Includes */
/* ********* */
#if !defined(AUTOCONF_INCLUDED)
#error "autoconf.h must be incuded"
#endif
#include "StandardDefines.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/*************************** Common Settings *************************/
#define HAL_WDT_SUPPORT	
/** Enable/Disable WatchDog, possible values TRUE/FALSE */
#if defined(CONFIG_WDT_ENABLED)
	#define WDT_ENABLED  TRUE
#else
	#define WDT_ENABLED  FALSE
#endif


/*************************** QE128 Settings *************************/
#if defined (FREESCALE_MC9S08_QE128)
#define WDT_ALTERNATE_CLOCK TRUE
#endif         
/*************************** QE128 Settings *************************/
/** 
\n This is the reload time for watchdog.
\n this parameter is hardware depending:
\n -GB/GT: 0->Short timeout period selected (2el(13) cycles of BUSCLK).
\n 		   1->Long timeout period selected (2el(18) cycles of BUSCLK).
\n -QE128: 0->Short timeout period selected.
\n 		   1->Long timeout period selected.
\n-H83607x: possible values [0-0xFE] 1 = 1,5625 msec typ, min. 0,78125 msec.
\n        Typical WatchDog Timeout = (0xFF-WDT_RELOAD_TIME)*1,5625 msec.
*/
#if defined(FREESCALE_MC9S08_QE128) || defined(FREESCALE_S08_GB60)
#define WDT_TIMEOUT	1
#endif
/** 
\n -QE128 select clock source for watchdog 
\n      COPCLKS            COPT		         SOURCE
\n			0               0				 1KHz	   2el(5)
\n          0               1				 1KHz	   2el(8)
\n          1               0				 BUSCLK	   2el(13)
\n          1               1				 BUSCLK	   2el(18)
*/



/*************************** AG128 Settings *************************/

#if defined(FREESCALE_MCF51_AG128)
/* Enables or disables further update of configuration, if 0, write only once */
#define WDT_UPDATE_AFTER_WRITE  1

/* Enables or disables the windowed mode of WDT */
#define WDT_WINDOWED_MODE       1

/* Enables or disables interrupt before the reset of WDT */
#define WDT_INT_ENABLED         0

/* Sets LPO oscillator or alternate clock (1 -> Alternate)*/
#define WDT_ALTERNATE_CLOCK     0

/* Enables or disables WDT in the following states */
#define WDT_ENABLED_IN_STOP     0
#define WDT_ENABLED_IN_DEBUG    0
#define WDT_ENABLED_IN_WAIT     0

/* Watchdog timeout period ( It is defined in terms of cycles of the watchdog clock.)*/
#define WDT_TIMEOUT         100 

/* 
Watchdog window period (It is defined in terms of cycles of the watchdog clock.) 
Window width must take into account the durtation needed the erase which takes about 20-23 ms. 
Suggested value is 40 ms.
*/
#define WDT_WINDOW          60

/* Allows the WDOG's functional test mode to be disabled permanently. */
#define WDT_DISABLE_TEST         0
#endif  /* #if defined(FREESCALE_MCF51_AG128) */


/*************************** LPC1227 Settings *************************/
#if defined(NXP_LPC12xx_1227)

/** Enable/Disable WatchDog, possible values TRUE/FALSE */
#define WDT_ENABLED  1

/* Enables or disables further update of configuration, if 0, write only once */
#define WDT_UPDATE_AFTER_WRITE  1

/* Enables or disables the windowed mode of WDT */
#define WDT_WINDOWED_MODE       1

/* Enables or disables interrupt *before* the reset of WDT */
/* at watchdog timeout user can only select interrupt OR reset */
/* this feature is related to the "warning" interrupt on LPC1227 */
/* set zero to disable pre-warning interrupt */
#define WDT_INT_ENABLED         0

/* Sets default IRC oscillator or watchdog oscillator (alternate) clock (1 -> WD clock)*/
#define WDT_ALTERNATE_CLOCK     1

/* Enables or disables WDT in the following states */
#define WDT_ENABLED_IN_DEBUG    WDT_ENABLED

/* Watchdog timeout period ( It is defined in terms of cycles of the watchdog clock.)*/
/* by default LPC1227 runs with WD timeout value set to 0xFFFF */
/* there is a prescaler by 4 on the clock */
/* default timeout is long (2^18) = 0x3FFFF (18 bit) */
#define WDT_TIMEOUT (100)//ms

/* Watchdog window period (It is defined in terms of cycles of the watchdog clock.) */
/* set to 0xFFFFFF to disable */
#define WDT_WINDOW  (34) /*(worst case (-40%) 20.4ms )0xFFFFFF*/

#endif /* defined(NXP_LPC12xx_1227) */

/*************************** RX210 Settings *************************/
#if defined(RN_RX200)

/* Enables or disables the windowed mode of WDT */
#define WDT_WINDOWED_MODE       1

/* Enables or disables interrupt *before* the reset of WDT */
/* at watchdog timeout user can only select interrupt OR reset */
/* this feature is related to the "warning" interrupt on LPC1227 */
/* set zero to disable pre-warning interrupt */
#define WDT_INT_ENABLED         FALSE

/* WTD This macro setting the parameter of WATCH DOG in a static way.
The setting macros are defined in *hal.h*/
#define WDT_TIMEOUT 			RX210_WDT_TIMEOUT_4096
#define WDT_PRESCALER 			RX210_WDT_DIVIDER_16			/*< This setting divide the value of PCLK clock*/
#define WDT_WINDOW_START  		START_WINDOW_50
#define WDT_WINDOW_END			END_WINDOW_0

#define WDT_ENABLED_AT_POWERON

#endif /* defined (RN_RX200) */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/

#endif /* __WDTPLTFCFG_H */

/* *********** */
/* End of file */
/* *********** */
