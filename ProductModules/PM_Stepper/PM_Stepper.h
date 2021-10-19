/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_Stepper.h
\n			header file of PM_Stepper.c
\n
\file		PM_Stepper.h
\ingroup	PM_Stepper
\date		Nov 23, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 23, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __STEPPER_H
#define __STEPPER_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "PM_StepperExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
/** define on data stepperError_t type no errors */
#define STEPPER_E_OK    0
/** define on data stepperError_t type errors */
#define STEPPER_E_KO    1

/** */
#define PLATFORM2_STEPPER

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/** typedef errors */
typedef uint8_t stepperError_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void PM_Stepper_Init(void);
stepperError_t PM_Stepper_InitParams( uint8_t *pPinsComponent,struct Loader pylotParams );
stepperError_t PM_Stepper_SetLogicalPosition( uint8_t regNew );
stepperError_t PM_Stepper_Manager(void);
uint8_t PM_Stepper_GetLastSeqApplied( void );

#endif /* __STEPPER_H */

/* *********** */
/* End of file */
/* *********** */

