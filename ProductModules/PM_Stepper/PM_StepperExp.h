/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_StepperExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_StepperExp.h
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

#ifndef __STEPPER_EXP_H
#define __STEPPER_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "ProductModules_Properties.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
/** State flags structure */
typedef struct
{
	/** if '1' module is init. */
	uint8_t initModule      :1;
	/** if '1' device is moving */
	uint8_t deviceIsMoving  :1;
	/** if '1' Manager is blocked */
	uint8_t stopManager     :1;

} stateFlags_s;
/** Byte and bit access to state's flags  */
typedef union
{
	/** byte access */
	uint8_t all;
	/** bit access */
	stateFlags_s flag;
} stateFlags_t;
/** This is a piece of Variable used to control the Stepper */
typedef struct
{
	/** free used for padding (Renesas) */
	uint8_t free;
	/** State's flags */
	stateFlags_t state;
	/** Numbers of sequences */
	uint8_t numberOfSeq;
	/** When change step */
	uint8_t divisor;
	/**  Step variation counter */
	uint8_t cntrControl;
	/** Last sequence applied to the stepper */
	uint8_t lastSeqApplied;
	/** Current step */
	uint16_t stepCurr;
	/** Where to go */
	uint16_t stepTo;

} stepperData_t;

/** Variable used to control the Stepper has this type */
typedef struct
{
	stepperData_t data;
	/** Pointer to the device's pin table */
	uint8_t *pPinsDevice;
	/** Pointer and lenght of Pylot Type Params */
	Loader_Struct pylotParams;

} stepperLoadStruct_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
extern stepperLoadStruct_t StepperLoad;

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
/* N.B.: LIB_IOM_SetLoad() must be called continuosly by Application */

/* Refresh request of the device, perform this call only if the device is steady */
void PM_Stepper_Refresh(void);
/* Returns TRUE if the device is moving otherwise FALSE */
bool_t PM_StepperIsMoving(void);

#endif /* __STEPPER_EXP_H */

/* *********** */
/* End of file */
/* *********** */






