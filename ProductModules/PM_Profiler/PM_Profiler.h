/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_Profiler.h
\n			header file of PM_Profiler.c
\n
\file		PM_Profiler.h
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

#ifndef __PROFILER_H
#define __PROFILER_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "PM_ProfilerExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
/** Return value for variables of profilerError_t type  */
#define PROFILER_E_OK   0
/** Return value for variables of profilerError_t type  */
#define PROFILER_E_KO   1

#define PLATFORM2_PROFILER

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/** For Vars of profilerError_t type */
typedef uint8_t profilerError_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void PM_Profiler_Init( void);
profilerError_t PM_InsertLoadWithProfile( uint8_t pinLinkedToLoad, uint8_t loadID );
profilerError_t PM_ProfileEnable( uint8_t *pProfile, uint8_t pinLinkedToLoad, uint8_t rep );
profilerError_t PM_ProfileDisable( uint8_t pinLinkedToLoad );
void PM_CheckIfSwitchOffProfile (void);
void PM_ProfilesManager( bool_t zcPos );
uint8_t PM_GetLoadWithProfileNumber( void );
bool_t PM_IsProfileActiveInPos( uint8_t loadId );

#endif /* __PROFILER_H */

/* *********** */
/* End of file */
/* *********** */


