/**
\defgroup ActivateAlgorithm	Modulo microprogramma Activate Algorithm

Gestisce il microprogramma Activate Algorithm per l'esecuzione di algoritmi e funzioni varie

\par \b PCLint: x Errors, 8 Warnings, x Info 12/06/2008 Valeria Catani


\date    17/03/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file ActivateAlgorithm.c

\file    ActivateAlgorithm.h
\ingroup ActivateAlgorithm
\date    17/03/2008
\author  Valeria Catani
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
\addtogroup ActivateAlgorithm
@{*/

#ifndef __UP_EXECUTEALGORITHM_H
#define __UP_EXECUTEALGORITHM_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* Debug Errori */
#ifdef _CHECKERRORS_
#define _AAL_CHECKERRORS_
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

enum  /* ADF_ALG*/
{

	ALGO_SV_WAIT=		    0,
	ALGO_1LIMIT_POWER,
	ALGO_WAIT_1L,
	ALGO_2LIMIT_POWER,
	ALGO_LOAD_RESTORE,
	ALGO_LOAD_WAIT,
	ALGO_SV_END,
	ALGO_SV_WAIT_END/* ultimo stato possibile con 3 bit*/

};
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */


/* Algoritmi */
void AAL_TurbiditySensorCalibration(uint8_t cmd);
void AAL_LevToTimeRefUpdate(uint8_t cmd);
void AAL_ReadTurbidityContinuous(uint8_t cmd);
void AAL_ReadTurbidityOneShot(uint8_t cmd);
void AAL_ValidateTurbidityCalibration(uint8_t cmd);
void AAL_ReadVirtualSensor(uint8_t cmd);


#endif /* __UP_EXECUTEALGORITHM_H */
/*@}*/

/* *********** */
/* End of file */
/* *********** */
