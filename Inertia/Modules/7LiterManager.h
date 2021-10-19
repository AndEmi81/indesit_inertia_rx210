/**
\defgroup 7 litri	Modulo microprogramma 7 litri

Gestisce il microprogramma 7 litri, con tutto quello che ciò comporta.

\par \b PCLint: 0 Errors, 1 Warnings, 7 Info 28/03/2008 roberto Fioravanti


\date    13/02/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file Maintain.c

\file    Maintain.h
\ingroup Maintain
\date    13/02/2008
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
\addtogroup Maintain
@{*/


#ifndef __UP_7LITER_H
#define __UP_7LITER_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define _7LITER_TANK_TIMEOUT_DISPL         0
#define _7LITER_TIME_MUL                   96 /* il contatore piattaforma 7 litri conta in quindicine di minuti; il valore del massimo tempo nel setting è in giorni, quindi va moltiplicato per 24h e poi per 4 (4 volte 15 m in un'ora) */
#define GET_7LITER_MAXTIME_FROM_SETTING()  (OFFS_B(Tab_SFT_7liter.Data, _7LITER_TANK_TIMEOUT_DISPL)*_7LITER_TIME_MUL)


/* stato microprogramma 7 litri */
enum{
  SL_INIT=0,
  SL_EV_OPEN_EXE,		/*1*/
  SL_EV_CLOSE_AND_PAUSE,/*2*/
  SL_END,				/*3*/
  SL_NUM
};

/** time */
#define SL_TIME_MASK			0x7FFF

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t SL_uP_7liter_Exe (uint8_t HighStatement,uint8_t LowStatement);

#endif /* __UP_7LITER_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
