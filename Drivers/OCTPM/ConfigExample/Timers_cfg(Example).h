/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		Timers_cfg.h
\n			header file of Timers_cfg.h
\n
\file		Timers_cfg.h 
\ingroup	[module_name_slave] 
\date		Sep 6, 2010 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
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
/*<belonging to the module, visible to the whole Platform project>*/
#define HAL_TIMER_SUPPORT	

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* Prescaler Value for Timer  */
#define TMR_ID0_PSC_VALUE     2
#define TMR_ID1_PSC_VALUE     4

/* Timer's Tick Resolution [nSec] */
#define TMR_ID0_TICK_RES    400
#define TMR_ID1_TICK_RES    1600

/* Period Value [ticks] */
#define TMR_ID0_PERIOD_VALUE    62500
#define TMR_ID1_PERIOD_VALUE    65000


/* Hardware Timer ID */
#define HW_TMR_ID_0 0
#define HW_TMR_ID_1 1    
#define HW_TMR_ID_2 2
#define HW_TMR_ID_3 3
#define NUM_HW_TMR_DESC 4
/* Timer Channel ID */
#define TMR_CH_ID_0 0
#define TMR_CH_ID_1 1    
#define TMR_CH_ID_2 2
#define TMR_CH_ID_3 3
#define TMR_CH_ID_4 4
#define TMR_CH_ID_5 5    
#define TMR_CH_ID_6 6
#define TMR_CH_ID_7 7
#define NUM_TMR_CH_DESC 8


/* Hardware Counter Clear Source ID */
#define TMR_CLEAR_SOURCE_IS_INTERNAL  0xFF
#define TMR_CLEAR_SOURCE_IS_OC_CH0    0    /* Output Compare Channel 0 */
#define TMR_CLEAR_SOURCE_IS_OC_CH1    1    /* Output Compare Channel 1 */
/* Timer's clock source ID */
#define TMR_CLK_SOURCE_IS_BUS_CLOCK             0
#define TMR_CLK_SOURCE_IS_FIXED_SYSTEM_CLOCK    1
#define TMR_CLK_SOURCE_IS_EXTERNAL_PIN          2
/* Timer's Counting Mode */
#define TMR_COUNT_UP            0
#define TMR_COUNT_DOWN          1
#define TMR_COUNT_UP_AND_DOWN   2
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
#define TMR_CH_OC_OFF           4	/* Software compare only */	
#define TMR_CH_OC_TOGGLE_T1     5	/* Toggle output on compare Threshold 1 */	
#define TMR_CH_OC_TOGGLE_T1_T2  6   /* Toggle output on compare Threshold 1 and 2 */		
#define TMR_CH_OC_PIN_HIGH      7	/* Set output on compare */	
#define TMR_CH_OC_PIN_LOW       8	/* Clear output on compare */	
/* Timer Channel ISR */
#define TMR_CH_ISR_DISABLE      0
#define TMR_CH_ISR_ENABLE       1
/* Timer Channels (mode Input Capture, Output Compare) */
#define TMR_CH_NO_INV           0
#define TMR_CH_INV              1

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



