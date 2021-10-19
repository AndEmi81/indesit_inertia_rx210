/**
\addtogroup ActivateAlgorithm
\ingroup Project
*/
/*
** **************************************************************************
** **************************************************************************
*/ 
/** 
Gestisce il microprogramma ActivateAlgorithm, che permette di attivare
alcune funzionalità della macchina durante l'esecuzione del ciclo.

Header file
 
\file    ActivateAlgorithm.h
\ingroup ActivateAlgorithm
 
\version 	1.0.0
\date    	17/01/2006
\author		Simone Costantini
*/ 
/*
** **************************************************************************
** **************************************************************************
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
\addtogroup ActivateAlgorithm
@{*/


#ifndef __UP_ACTIVATEALGORITHM_H
#define __UP_ACTIVATEALGORITHM_H

/* ******** */
/* Includes */
/* ******** */

#include "APP_Config.h"
#if (AAL_ACTIVATEALGORHYTHM_ENABLE == 1)

/* ****************** */
/* Defines and Macros */
/* ****************** */
enum AAL_ALG_CMD
{
    AAL_ALG_MONITOR=0,
    AAL_ALG_INIT,
    AAL_ALG_DISABLE
};


/* Debug Errori */
#if (CHECKERRORS_ENABLE == 1)
	#define _ACTIVATEALGORYTHM_CHECKERRORS_
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


#if (MULTI_COMPARTMENT_APPL == 1)	
uint8_t AAL_Activate_Algorithm_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment);
void AAL_algorithmManagement(uint8_t compartment);
void NullFunc(uint8_t cmd, uint8_t compartment);
void AAL_resetAlgorithmCtlVars(uint8_t compartment);
#else
uint8_t AAL_Activate_Algorithm_Exe(uint8_t highStatement, uint8_t lowStatement);
void AAL_algorithmManagement(void);
void NullFunc(uint8_t cmd);
void AAL_resetAlgorithmCtlVars(void);
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

#endif /* __UP_ACTIVATEALGORITHM_H */
/*@}*/
#endif /* (AAL_ACTIVATEALGORHYTHM_ENABLE == 1) */

/* *********** */
/* End of file */
/* *********** */
