/**
\defgroup Maintain	Modulo microprogramma Maintain

Gestisce il microprogramma Maintain, consentendo il blocco dell'esecuzione
del ciclo nel passo corrente per un determinato tempo.

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


#ifndef __UP_MAINTAIN_H
#define __UP_MAINTAIN_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifdef _CHECKERRORS_
	#define _MTN_CHECKERRORS_
#endif

/* stato microprogramma Maintain */
enum{
  MAINTAIN_INIT=0,
  MAINTAIN_EXE,/*1*/
  MAINTAIN_END,/*2*/
  MAINTAIN_NUM
};

/** CALIBRATION */
/*Position for calibration enable*/
#define MTN_CORR_FLAG_BIT		15
#define MNT_RES_FOR_CORR_MASK	0x80		

/** time */
#define MTN_TIME_MASK			0x7FFF

/** Modificatori */
#define MTN_MOD_TAB_MSK         0x7F
#define MTN_MOD_TAB 			(OFFS_B(uPMaintain_setting_data.Data,3) & MTN_MOD_TAB_MSK)
#define UNDEF_MOD_TAB			0xFF	/*puntatore tabella modificatori non definito*/
#define	MNT_CALIBRATION_UPMAN_HALF_SCALE 	128
#define MNT_CALIBRATION_UP_MANTAIN_CST		1280


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t MTN_uP_Maintain_Exe (uint8_t HighStatement,uint8_t LowStatement);

#endif /* __UP_MAINTAIN_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
