/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		ExternalOutputManager.h
\n			Header file of ExternalOutputManager.c
\n
\file		ExternalOutputManager.h 
\ingroup	ExtOutputsManager 
\date		Sep 11, 2010 
\version	01.00.00 
\author		Roberto fioravanti
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

#ifndef __EXTERNAL_OUTPUT_MANAGER_H
#define __EXTERNAL_OUTPUT_MANAGER_H

/* ********** */
/* Includes */
/* ********* */
#include "OutputManagerExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */
LIB_IOM_ErrorType LIB_IOM_External_SetCommand(uint8_t pilotType, const GIOM_LoadStruct *const pLoad);
bool_t	LIB_IOM_External_GetLoadStatus(uint8_t pilotType, const GIOM_LoadStruct *const pLoad);
#endif	/* __EXTERNAL_OUTPUT_MANAGER_H */

/* *********** */
/* End of file */
/* *********** */
