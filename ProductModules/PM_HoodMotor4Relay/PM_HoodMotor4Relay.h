/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		        Header file di PM_HoodMotor4Relay.c 
\n			        Contiene le definizioni per l'attivazione/disattivazione di un carico
\n
\file           PM_HoodMotor4Relay.h
\ingroup        PM_HoodMotor4Relay
\version        1.0.0
\date           30/09/2011
\author         Leonardo Sparti
\n

*/



#ifndef __HOOD_MOTOR_4RELAY_H
#define __HOOD_MOTOR_4RELAY_H

/* ******** */
/* Includes */
/* ******** */
#include "OutputManagerExp.h"
#include "DigitalOutputs.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** typedef pointer in input a DigOutMultiSetLoad() */
typedef const uint8_t *const pModulesPins_t;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void HoodMotor4Rel_Init(pModulesPins_t pModulePins, uint8_t delay_activation);
void HoodMotor4RelSetLoad( L_PARAM loadParam, pModulesPins_t pModulePins );
void HoodMotor4Rel_Manager( void );



#endif /* __HOOD_MOTOR_4RELAY_H */

/* *********** */
/* End of file */
/* *********** */
