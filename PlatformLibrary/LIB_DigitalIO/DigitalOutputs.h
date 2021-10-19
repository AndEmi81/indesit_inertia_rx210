/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		DigitalOutputs.h
\n			header file of DigitalOutputs.c
\n
\file		DigitalOutputs.h
\ingroup	DigitalOutput
\date		Jun 18, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
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

#ifndef __DIGITAL_OUTPUTS_H
#define __DIGITAL_OUTPUTS_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "DigitalOutputsExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
void LIB_DIO_SET_LOAD(uint8_t pinValue, uint8_t pin);
#endif /* __DIGITAL_OUTPUTS_H */

/* *********** */
/* End of file */
/* *********** */
