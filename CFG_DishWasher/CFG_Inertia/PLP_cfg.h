/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		[Title]
\n			[Brief description]
\n
\file		PLP_cfg.h 
\ingroup	[module_name_slave] 
\date		Ott 25, 2011 
\version	01.00.00 
\author		Leonardo Sparti 
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

#ifndef __PLPPLTFCFG_H
#define __PLPPLTFCFG_H

/* ********** */
/* Includes */
/* ********* */
#include "timers_cfg.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Canale PWM usato per il PLP */
#define  PLP_CH_ID   					TMR_CH_ID_7


/* Risoluzione del timer: deve essere congruente con
   PLP_CH_ID (stesso timer di cui si sta usando un canale) */
#define  PLP_TMR_TICK_RES   			TMR_ID1_TICK_RES


/* Macro di conversione */
#define US_TO_TICK(x)  					((IO_TickResType)((IO_TimingValueType)(x)))


/* spegnimento carichi in us prima del successivo ZC */
/*#define PLP_TURNOFF_TIMINGus    4000*/


/* spegnimento carichi in tick prima del successivo ZC */
/*#define TIMING_TURNOFFticks     US_TO_TICK(PLP_TURNOFF_TIMINGus)*/


/* Threshold = 260us */
/*#define PLP_ACTIVATION_TH     US_TO_TICK(260)*/
/* Activation = 600us */
#define PLP_DRIVELBOUNDtck    US_TO_TICK(600)


/* activation point for the total turning OFF of the lamp */
#define DIM_OFF   575
/* activation point for the total turning ON of the lamp */
#define DIM_MAX     0


/* activation point for the total turning OFF of the lamp */
/*#define SLID_OFF   570*/
/* activation point for the total turning ON of the lamp */
/*#define SLID_MAX     0*/
/* Step of the slider is obtained as (SLID_OFF/N Regulations) = (570/15) */
/*#define SLID_STEP    38*/


/* Conversion from Freescale GB60 timer tick to us */
#define PLP_TIMER_SCALING(x)	((12800*(x))/1000)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __PLPPLTFCFG_H */

/* *********** */
/* End of file */
/* *********** */
