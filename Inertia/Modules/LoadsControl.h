/**
\defgroup LoadsControl	Modulo microprogramma LoadsControl

Gestisce il microprogramma LoadsControl per il pilotaggio dei carichi,
con la possibilit� di attivazione di uno o pi� carichi in continua o ad impulsi (profili)

\par \b PCLint: 0 Errors, 2 Warnings, 5 Info 28/03/2008 roberto Fioravanti


\date    21/02/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file LoadsControl.c

\file    LoadsControl.h
\ingroup LoadsControl
\date    21/02/2008
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
\addtogroup LoadsControl
@{*/




#ifndef __UP_LOADSCONTROL_H
#define __UP_LOADSCONTROL_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifdef _CHECKERRORS_
	#define _LDC_CHECKERRORS_
#endif

/* modalit� di pilotaggio carichi */
#define LDC_DRIVE_TYPE_MSK        (F1|F0)


#define LDC_NOSTOP_DRIVE          0
#define LDC_PULSE_DRIVE           F0

/* tipo sequenza */
#define LDC_SEQ_START_OFFS        3
#define LDC_SEQ_START_MSK         (b7)
#define LDC_SEQ_START_ON          0
#define LDC_SEQ_START_OFF         (b7)

/* TON, TDELAY, TOFF */
#define LDC_TDELAY_MSK            0x7F
#define LDC_TON_OFFS              2
#define LDC_TDELAY_OFFS           3
#define LDC_TOFF_OFFS             4



/** ordine fisso */
enum {
LDC_PULSE_EXE_DELAY=0,
LDC_PULSE_EXE_ON,
LDC_PULSE_EXE_OFF
};

#define SEQ1_OFFSET				        0
#define SEQ2_OFFSET				        5

/*LoadsControlDat.status*/


/* Dati impulsi */
#define LDC_PULSE_START(x)		((OFFS_B((uPLoadsControl_setting_data.Data+x),LDC_SEQ_START_OFFS)) & LDC_SEQ_START_MSK)
#define LDC_PULSE_TDELAY(x)		(OFFS_B((uPLoadsControl_setting_data.Data+x),LDC_TDELAY_OFFS) & LDC_TDELAY_MSK)
#define LDC_PULSE_TON(x)		(OFFS_B((uPLoadsControl_setting_data.Data+x),LDC_TON_OFFS)) 
#define LDC_PULSE_TOFF(x)		(OFFS_B((uPLoadsControl_setting_data.Data+x),LDC_TOFF_OFFS))


/* stato */
#define LOADSCONTROL_INIT		            0
    
#define LOADSCONTROL_SEQ1_DONE_MSK      MASK_B(0)
#define LOADSCONTROL_SEQ2_DONE_MSK      MASK_B(1)

#define LOADSCONTROL_ALL_SEQ_DONE_MSK   (LOADSCONTROL_SEQ1_DONE_MSK|LOADSCONTROL_SEQ2_DONE_MSK)


#if ((SEQ1_OFFSET > 7) || (SEQ2_OFFSET > 7))
#error "\n LOADSCONTROL_TRUE is a 8 bit mask and uses profile's offset as a marker, this connection cannot further be used!"
#endif


#define N_LDC_BITMAP			16

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t LDC_uP_LoadsControl_Exe (uint8_t highStatement,uint8_t lowStatement);

#endif /* __UP_LOADSCONTROL_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
