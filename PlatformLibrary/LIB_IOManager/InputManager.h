/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
 			Header file of InputManager.c
\n
\file		InputManager.h
\ingroup	InputsManager
\date		Jun 8, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Jun 8, 2010
\n		stefano.ughi
\n		changes
\n 		- creation
\n
*/

#ifndef __INPUT_MANAGER_H
#define __INPUT_MANAGER_H

/* ********** */
/* Includes */
/* ********* */
/*lint -e537*/
#include "StandardTypes.h"
#include "InputManagerExp.h"
/*lint +e537*/
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
/*<belonging to the module, visible to the Platform project>*/
void LIB_IOM_InputInitialization( uint8_t resetType );
#endif /* __INPUT_MANAGER_H */

/* *********** */
/* End of file */
/* *********** */
