/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Fixed Speed motor Interface
\n			Interface (macros, prototypes and type definitions) for Fixed Speed motor module.
\n
\file		FixedSpeedMotor.h
\ingroup	PM_FixedSpeedMotor
\date		28-jul-2010
\version	00.00.54
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.54 
\n		28/07/2010 
\n		Roberto Fioravanti 
\n		changes
\n 		- creation
\n
\n
\n etc.
*/

#ifndef __FIXEDSPEEDMOTOR_H_
#define __FIXEDSPEEDMOTOR_H_


/* ******** */
/* Includes */
/* ******** */

#include "StandardDefines.h"
#include "StandardTypes.h"
#include "OutputManagerExp.h"
#include "FixedSpeedMotorExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* #define FSM_DEBUG */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**
Error type
 */

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
void FSM_Init(void);
void FSM_SetParam(uint8_t sor, 
                  uint8_t suppH,
                  uint8_t suppL,
                  uint8_t T2,
                  uint8_t T3
                  );
void FSM_Manager(void);
void FSM_SetPointUpdate(const GIOM_LoadStruct  * const cmd);

#endif /* FIXEDSPEEDMOTOR_H_ */

/* *********** */
/* End of file */
/* *********** */

