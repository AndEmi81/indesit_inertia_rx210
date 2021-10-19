/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		DigitalInputsExp.h
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		DigitalInputsExp.h
\ingroup	DigitalInputs
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n 		01.03.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

#ifndef __DIGITAL_INPUTS_EXP_H
#define __DIGITAL_INPUTS_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */




/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
#if !defined(MODULE_BODY_LIB_DIGITAL_IO)
extern dig_input_t DigInputs;          /* bit map */
extern dig_input_t DigInputsReady;     /* bit map */
extern dig_input_t DigInputsDisable;   /* bit map */
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
bool_t LIB_DIO_AreDigInputsReady(void);
bool_t LIB_DIO_GetDigInputVal(uint8_t pos);
void LIB_DIO_SetDigInputVal(uint8_t pos, uint8_t value);
bool_t LIB_DIO_IsDigitalReady(uint8_t pos);
#endif /* __DIGITAL_INPUTS_EXP_H */

/* *********** */
/* End of file */
/* *********** */

