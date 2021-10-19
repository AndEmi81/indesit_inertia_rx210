/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Timer configuration
\n			The files contains defines for timer's driver configuration
\n
\note		\par \b Note 1:
\n			This Must NOT access HAL or HAL_CFG files
\file		Timers_cfg.h 
\ingroup	[module_name_slave] 
\date		Sep 6, 2010 
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

#ifndef __TIMERS_CFG_H
#define __TIMERS_CFG_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */



/* Timer's clock source ID */
#define TMR_CLK_SOURCE_IS_BUS_CLOCK             0x00




/* ************************************************************************** */
/* FREESCALE_MCF51_AG128 */
/* ************************************************************************** */
#ifdef FREESCALE_MCF51_AG128
/* Hardware Timer ID entry nella TmrCntMap*/
#define HW_TMR_ID_0 0
#define HW_TMR_ID_1 1
#define HW_TMR_ID_2 2
#define HW_TMR_ID_3 3
#define HW_TMR_ID_4 4

/* Timer Channel ID */
#define TMR_CH_ID_0 0
#define TMR_CH_ID_1 1
#define TMR_CH_ID_2 2
#define TMR_CH_ID_3 3
#define TMR_CH_ID_4 4
#define TMR_CH_ID_5 5
#define TMR_CH_ID_6 6
#define TMR_CH_ID_7 7
#define TMR_CH_ID_8 8
#define TMR_CH_ID_9 9
#define TMR_CH_ID_10 10
#define TMR_CH_ID_11 11
#define TMR_CH_ID_12 12
#define TMR_CH_ID_13 13
#define TMR_CH_ID_14 14
#define TMR_CH_ID_15 15

/* Hardware Counter Clear Source ID */
#define TMR_CLEAR_SOURCE_IS_INTERNAL  0xFF

#define NUM_HW_TMR_DESC 4
#define NUM_TMR_CH_DESC 12

/* Prescaler Value for Timer  */
#define TMR_ID0_PSC_VALUE       2
#define TMR_ID1_PSC_VALUE       3
#define TMR_ID2_PSC_VALUE       3               /**< for AG128:  this value must be the same of OS_SYS counter */
#define TMR_ID3_PSC_VALUE       3

/* Timer's Tick Resolution [nSec] */
#define TMR_ID0_TICK_RES        400
#define TMR_ID1_TICK_RES        402
#define TMR_ID2_TICK_RES        402

/* Period Value [ticks] */
#define TMR_ID0_PERIOD_VALUE    62500
#define TMR_ID1_PERIOD_VALUE    0
#define TMR_ID2_PERIOD_VALUE    0
#endif /* defined(FREESCALE_MCF51_AG128) */


/* ************************************************************************** */
/* NXP_LPC12xx_1227 */
/* ************************************************************************** */
#ifdef NXP_LPC12xx_1227  

#define NUM_HW_TMR_DESC 4
#define NUM_TMR_CH_DESC 16


#define TMR_ID0_PSC_VALUE     1
#define TMR_ID1_PSC_VALUE     13
#define TMR_ID2_PSC_VALUE     3
#define TMR_ID3_PSC_VALUE     4

/* Timer's Tick Resolution [nSec] */
#define TMR_ID0_TICK_RES    400
#define TMR_ID1_TICK_RES    485
#define TMR_ID2_TICK_RES    1600
#define TMR_ID3_TICK_RES    1600

/* Period Value [ticks] */
#define TMR_ID0_PERIOD_VALUE    62500
#define TMR_ID1_PERIOD_VALUE    0
#define TMR_ID2_PERIOD_VALUE    128000
#define TMR_ID3_PERIOD_VALUE    128000


  /* fixed system clock is not applicable to LPC */
  #undef TMR_CLK_SOURCE_IS_FIXED_SYSTEM_CLOCK

/* Note: TMR_CLEAR_SOURCE_IS_INTERNAL is used for the modulo register on the freescale */
/* not applicable for the LPC */

/* there are two additional channels which can be used for modulo operation */
/* these are physical channels associated with the timer */
  #define TMR_CLEAR_SOURCE_IS_OC_CH2    2    /* Output Compare (match) Channel 2 */
  #define TMR_CLEAR_SOURCE_IS_OC_CH3    3    /* Output Compare (match) Channel 3 */


  /* these define the options for input clock signal selection (which pin) */
  #define CAP0_IN       (0u << 2)     // capture 0 pin is clk input
  #define CAP1_IN       (1u << 2)     // capture 1 is clk input
  #define COMP_LEVEL    (2u << 2)     // comparator level is sampled for clocking (16-bit timer only)
  #define COMP_EDGE     (3u << 2)     // comparator edge is sampled for clocking (16-bit timer only)
  #define CAP2_IN       (COMP_LEVEL)  // capture 2 is clk input (32-bit timer only)
  #define CAP3_IN       (COMP_EDGE)   // capture 3 is clk input (32-bit timer only)

  /* these define the setup for the external clock, if used */
  #define TMR_ID0_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // cap0in is external clock pin of LPC_CT16B0
  #define TMR_ID1_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // cap0in is external clock pin of LPC_CT16B0
  #define TMR_ID2_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // cap0in is external clock pin of LPC_CT16B0
  #define TMR_ID3_CLK_SOURCE_INPUT_SEL  (CAP0_IN) // cap0in is external clock pin of LPC_CT16B0

#endif

/* Timer up/down direction counter is defined in MTU2S->TSR Register*/
#define TMR_COUNT_DOWN          0
#define TMR_COUNT_UP            1
#define TMR_COUNT_UP_AND_DOWN   2


#ifdef NXP_LPC12xx_1227
  /* LPC timers support only count up */
  #undef TMR_COUNT_DOWN
  #undef TMR_COUNT_UP_AND_DOWN
#endif
/* ************************************************************************** */
/* RN_RX200 */
/* ************************************************************************** */
/* TIMER DEFINE FOR RENESAS RX200 FAMILY MICROCONTROLLER */

#if defined(RN_RX200)
/* RX210 Prescaler value **/
#define TMR_DEFAULT_PSC_VALUE 	0xF8				/* Default prescaler value from DS*/
#define TMR_PSC_VALUE____1		0					/* Internal Clock: counts on PCLK/1,    M0-M1-M2-M3-M4*/
#define TMR_PSC_VALUE____4		1					/* Internal Clock: counts on PCLK/4,    M0-M1-M2-M3-M4*/
#define TMR_PSC_VALUE___16		2					/* Internal Clock: counts on PCLK/16,   M0-M1-M2-M3-M4*/
#define TMR_PSC_VALUE___64		3					/* Internal Clock: counts on PCLK/64,   M0-M1-M2-M3-M4*/
#define TMR_PSC_VALUE__256		4					/* Internal Clock: counts on PCLK/256,  M3-M4*/
#define TMR_PSC_VALUE_1024		5					/* Internal Clock: counts on PCLK/1024, M3-M4*/
/*
#define TMR_PSC_VALUE_1024	  6					Internal Clock: counts on PCLK/1024, M1
#define TMR_PSC_VALUE_1024	  7					Internal Clock: counts on PCLK/1024, M2
*/


/* common define for each MODULEs  */
#define NUM_HW_CNTR     5			/* Available Timers*/
/* Hardware Timer ID: entry in TmrCntMap table */
#define HW_TMR_ID_0 0
#define HW_TMR_ID_1 1
#define HW_TMR_ID_2 2
#define HW_TMR_ID_3 3
#define HW_TMR_ID_4 4

#if defined(EDGE_MODULE_LP1_0258_01) || defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)

	#define NUM_HW_CNTR_CH  9		/* number of available channels */

	/* 
	Timer Channel ID: TmrChMap table entry. 
	0xFF invalid
	*/
	#define TMR_CH_ID_0 3
	#define TMR_CH_ID_1 8

	#define TMR_CH_ID_2 0

	#define TMR_CH_ID_3  0xFF
	#define TMR_CH_ID_4  0xFF
	#define TMR_CH_ID_5  0xFF 
	#define TMR_CH_ID_6  0xFF

	#define TMR_CH_ID_7  4

	#define TMR_CH_ID_8  0xFF
	#define TMR_CH_ID_9  0xFF
	#define TMR_CH_ID_10 0xFF

	#define TMR_CH_ID_11 1

	#define TMR_CH_ID_12 0xFF
	#define TMR_CH_ID_13 0xFF
	#define TMR_CH_ID_14 0xFF

	#define TMR_CH_ID_15 2

	#define TMR_CH_ID_16 0xFF

	#define TMR_CH_ID_17 5

	#define TMR_CH_ID_18 0xFF
	#define TMR_CH_ID_19 0xFF

	#define TMR_CH_ID_20 7

	#define TMR_CH_ID_21 0xFF

	#define TMR_CH_ID_22 6

	#define TMR_CH_ID_23 0xFF
	#define TMR_CH_ID_24 0xFF
	#define TMR_CH_ID_25 0xFF
	#define TMR_CH_ID_26 0xFF
	#define TMR_CH_ID_27 0xFF
	#define MAX_TMR_CH_ID 28

#elif defined(CASTELLATED_MODULE_LP1_0241_01)
	#define NUM_HW_CNTR_CH  9		/* number of available channels */

	/* 
	Timer Channel ID: TmrChMap table entry. 
	0xFF invalid
	*/

	#define TMR_CH_ID_0 3 /* entry in TmrChMap*/
	#define TMR_CH_ID_1 8

	#define TMR_CH_ID_2 0

	#define TMR_CH_ID_3  0xFF
	#define TMR_CH_ID_4  0xFF
	#define TMR_CH_ID_5  0xFF 
	#define TMR_CH_ID_6  0xFF

	#define TMR_CH_ID_7  4

	#define TMR_CH_ID_8  0xFF
	#define TMR_CH_ID_9  0xFF
	#define TMR_CH_ID_10 0xFF

	#define TMR_CH_ID_11 1

	#define TMR_CH_ID_12 0xFF
	#define TMR_CH_ID_13 0xFF
	#define TMR_CH_ID_14 0xFF

	#define TMR_CH_ID_15 2

	#define TMR_CH_ID_16 0xFF

	#define TMR_CH_ID_17 5

	#define TMR_CH_ID_18 0xFF
	#define TMR_CH_ID_19 0xFF

	#define TMR_CH_ID_20 7

	#define TMR_CH_ID_21 0xFF

	#define TMR_CH_ID_22 6

	#define TMR_CH_ID_23 0xFF
	#define TMR_CH_ID_24 0xFF
	#define TMR_CH_ID_25 0xFF
	#define TMR_CH_ID_26 0xFF
	#define TMR_CH_ID_27 0xFF
	
	#define MAX_TMR_CH_ID 28

#else
#error "Module not yet supported or check you pre-processor settings."
#endif /*defined(CASTELLATED_MODULE_LP1_0241_01)*/

/* Timer's Tick Resolution [nSec] */
#define TMR_ID0_TICK_RES    	50

/* Period Value [ticks] */
#define TMR_ID0_PERIOD_VALUE    0xF000


#endif /*defined(RN_RX200)*/

/* Timer's Counting Enable/Disable */
#define TMR_CNT_DISABLE         0
#define TMR_CNT_ENABLE          1

/* Timer Overflow/Underflow ISR */
#define TMR_ISR_DISABLE         0
#define TMR_ISR_ENABLE          1


/* Input Capture Sensitive Edge */
#define TMR_CH_IC_POS_EDGE      1
#define TMR_CH_IC_NEG_EDGE      2
#define TMR_CH_IC_BOTH_EDGE     3

/* Output Compare behaviour */
#define TMR_CH_OC_OFF           4   /* Software compare only */	
#define TMR_CH_OC_TOGGLE_T1     5   /* Toggle output on compare Threshold 1 */	
#define TMR_CH_OC_TOGGLE_T1_T2  6   /* Toggle output on compare Threshold 1 and 2 */		
#define TMR_CH_OC_PIN_HIGH      7   /* Set output on compare */	
#define TMR_CH_OC_PIN_LOW       8   /* Clear output on compare */	

#ifdef NXP_LPC12xx_1227
  /* LPC compare just on one threshold */
  #undef TMR_CH_OC_TOGGLE_T1_T2
#endif

/* Timer Channel ISR */
#define TMR_CH_ISR_DISABLE      0
#define TMR_CH_ISR_ENABLE       1
/* Timer Channels (mode Input Capture, Output Compare, PWM) */
#define TMR_CH_NO_INV           0
#define TMR_CH_INV              1

/* Hw Counter Options */
#define TMR_DONT_TOUCH_HW_TMR_REG_POS   0

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/

#endif /* __TIMERS_CFG_H */

/* *********** */
/* End of file */
/* *********** */



