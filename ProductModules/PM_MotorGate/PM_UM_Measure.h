/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Measure.h
\brief
\addtogroup MotorGate
@{*/

#ifndef __MOTOR_FORMULAS_H
#define __MOTOR_FORMULAS_H
#include "PM_UniversalMotorMeasure.h"
#include "PM_UM_UniversalMotor.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* motore universale
   stato della misura della funzione che calcola la media dell'angolo di fase
   per l'attivazione dell'algoritmo antischiuma e della fase media  */

#define ENGINE_MEASURE_IS_ON				(uint8_t)0
#define ENGINE_MEASURE_IS_OFF				(uint8_t)(!ENGINE_MEASURE_IS_ON)
#define NUM_SAMPLES_ALPHA_PHASE				(uint8_t)16
#define NUM_SAMPLES_ALPHA_ANTI_FOAM			(uint8_t)16
#define NUM_SAMPLES_SPEED_LIM				(uint8_t)16




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/*
motore universale
Inizializza la misura per l'algoritmo antischiuma e taglio della movimentazione
per eccessivo sforzo del motore.

0 -> enginePhase
1 -> antiFoam
*/
#ifdef CONFIG_PM_UM_MOTOR_MEASURE
void engineMeasureInit( PM_UM_Meas_t );
void engineMeasures( PM_UM_Meas_t );
void switchOffEngineMeasure( PM_UM_Meas_t whichMeasure );
#endif

#endif /* __MOTOR_FORMULAS_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
