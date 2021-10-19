/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n			Header file of OutputManager.c
\n
\file		OutputManager.h
\ingroup	OutputsManager
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		00.00.64
\n		23/09/2010
\n		Roberto Fioravanti
\n		changes
\n		-	addition of LIB_IOM_GetPilotType prototype
\n		-	addition of PTR_IO_CONFIG_DATA , EEP_OUTPUT_TABPTR_OFFSET, PILOT_TYPE_PARAMS_OFFSET, PILOT_TYPE_FIELD
\n
\n
*/

#ifndef __OUTPUT_MANAGER_H
#define __OUTPUT_MANAGER_H

/* ********** */
/* Includes */
/* ********* */
#include "OutputManagerExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* setting displacements */

/* These are the inputs for void LIB_IOM_SetAllLoadsOff  ( uint8_t source ) routine
SALO stay for SetAllLoadsOff  */
#define SOURCE_SALO_PLATFORM 	1   /* the function was called by Platform's routines */
#define SOURCE_SALO_PCCONTROL	0   /* the function was called by P.C. Control's routines */

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
uint8_t LIB_IOM_GetPilotType(uint8_t pilotTypeField);
void LIB_IOM_OutputInitialization( uint8_t ResetType );
uint8_t LIB_IOM_FindLoad(uint8_t loadPos,uint8_t **ppByte);
uint8_t LIB_IOM_FindLoadByID(uint8_t loadID,uint8_t **ppByte);
void LIB_IOM_SetAllLoadsOff  ( uint8_t source );

#endif /* __OUTPUT_MANAGER_H */

/* *********** */
/* End of file */
/* *********** */
