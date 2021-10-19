/**
\defgroup PumpControl	Modulo microprogramma PumpControl

Permette di attivare sia la pompa sincrona che BLDC

\par \b PCLint: 0 Errors, 2 Warnings, 5 Info 28/03/2008 roberto Fioravanti


\date    04/03/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file PumpControl.c

\file    PumpControl.h
\ingroup PumpControl
\date    04/03/2008
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author  
*/



/**
\addtogroup PumpControl
@{*/

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */



#ifndef __UP_PUMP_CONTROL_H
#define __UP_PUMP_CONTROL_H


/* coefficienti rappesentazione dati */
#define PUMP_CTRL_BLDC_SPEED_MUL            25
#define PUMP_CTRL_BLDC_RAMP_TIME_MUL        5

/* dati microprogramma */
#define PUMP_CTRL_SPEED_DISPL     0
#define PUMP_CTRL_TIME_DISPL      1

/* Stati di PumpControl */
enum
{ 
  PUMP_CTRL_INIT=0,
  PUMP_CTRL_ERROR,
};

/* uP_PumpCtrl_Flags */
#define PUMP_CTRL_UP_RUNNING_BIT    1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t PMP_uP_PumpControl_Exe( uint8_t HighStatement, uint8_t LowStatement);
 
#endif /* __UP_PUMP_CONTROL_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */

