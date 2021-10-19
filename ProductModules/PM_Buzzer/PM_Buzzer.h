/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_Buzzer.h
\n			header file of PM_Buzzer.c
\n
\file		PM_Buzzer.h 
\ingroup	PM_Buzzer
\date		31/05/2011 
\version	01.00.00 
\author		Giuseppe Boccolato
\n
\n
\par	History:
\n
\n 		01.00.00 
\n 		Creation
\n		31/05/2011 
\n		Giuseppe Boccolato 
\n		changes 
\n
*/


#ifndef __BUZZER_H
#define __BUZZER_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "PM_BuzzerExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_BuzzerInit (uint8_t pin);
void PM_SetBuzzer (uint8_t beepTime, uint8_t freqIndex);
void PM_BuzzerManager ( void );

/*@}*/

#endif /* __BUZZER_H */

/* *********** */
/* End of file */
/* *********** */








