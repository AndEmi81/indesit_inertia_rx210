/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_ProfilerExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_ProfilerExp.h
\ingroup	PM_Profiler
\date		Nov 24, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 24, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
*/

#ifndef __PROFILER_EXP_H
#define __PROFILER_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
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

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
bool_t PM_IsAtLeastOneProfileActive (void);
bool_t PM_IsProfileActive(uint8_t i);

#endif /* __PROFILER_EXP_H */

/* *********** */
/* End of file */
/* *********** */

