/**
\defgroup Drain	Modulo microprogramma Drain

Gestisce il microprogramma Drain.

2 sono le opzioni possibili

-F1|F0 = 0|0 -> scarica a vuoto pressostato + x sec.
-F1|F0 = 0|1 -> scarica per x sec

Prima opzione :
Al termine,  il microprogramma azzera dl_in_vasca.e aggiorna i dl caricati.
La pompa viene attivata per tempo massimo pari al timeout Drain1stTimeoutToEmpty sec.
Se il vuoto prerssostato viene raggiunto, allora la pompa rimane attivata per x sec.
Altrimenti, dopo una pausa di DrainPauseToEmpty secondi in cui la pompa è disattivata.
Altrimenti, dopo una pausa di DrainPauseToEmpty secondi in cui la pompa ? disattivata.
si passa ad un secondo tentativo di Drain2ndTimeoutToEmpty secondi.
Se il vuoto prerssostato viene raggiunto, allora la pompa rimane attivata per x sec, 
altrimenti viene venerato un allarme.

Seconda opzione:
la pompa viene attivata per x secondi.


\par \b PCLint: 0 Errors, 4 Warnings, 8 Info 25/02/2008 roberto Fioravanti


\date    21/02/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file Drain.c

\file    Drain.h
\ingroup Drain
\date    25/02/2008
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
\addtogroup Drain
@{*/

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */



#ifndef __UP_DRAIN_H
#define __UP_DRAIN_H



#define DRN_DRAIN_TYPE_MSK		(F1|F0)


#define DRN_DRAIN_VP_PLUS_TIME		0
#define DRN_DRAIN_TIME				F0
#define DRN_DRAIN_VP_PLUS_TIME_SENSORI_VIRTUALI		F1

#define IS_VIRTUAL_SENSOR_VERSION(x)  (((x) &( DRN_DRAIN_TYPE_MSK)) == DRN_DRAIN_VP_PLUS_TIME_SENSORI_VIRTUALI)

/* dati microprogramma */
#define DRN_DRAINTIME_DISPL     0

#define DRN_DRAINTIME           OFFS_B(uPDrain_setting_data.Data, DRN_DRAINTIME_DISPL)

/* timeout vuoto pressostato */
#define DRN_1ST_TOUT_TO_EMPTY_DISPL   0
#define DRN_PAUSE_TO_EMPTY_DISPL      1
#define DRN_2ND_TOUT_TO_EMPTY_DISPL   2



#define DRN_1ST_TOUT_TO_EMPTY(p)   OFFS_B(p.Data, DRN_1ST_TOUT_TO_EMPTY_DISPL)
#define DRN_PAUSE_TO_EMPTY(p)     OFFS_B(p.Data, DRN_PAUSE_TO_EMPTY_DISPL)
#define DRN_2ND_TOUT_TO_EMPTY(p)   OFFS_B(p.Data, DRN_2ND_TOUT_TO_EMPTY_DISPL)


/* dati microprogramma */
#define DRAINPUMP_CTRL_SPEED_DISPL     2
#define DRAINPUMP_CTRL_TIME_DISPL      3



/* stati up Drain */
enum
{ 
  DRAIN_INIT=0,
  DRAIN_1ST_TOUT,
  DRAIN_TIME,
  DRAIN_PAUSE,
  DRAIN_2ND_TOUT,
  DRAIN_END,
  DRAIN_ERROR,
};

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t DRN_uP_Drain_Exe( uint8_t HighStatement, uint8_t LowStatement);
 
/* *********** */
/* End of file */
/* *********** */
#endif /* __UP_DRAIN_H */

