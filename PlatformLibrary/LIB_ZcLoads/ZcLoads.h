/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		ZcLoads.h
\n			header file of ZcLoads.c
\n
\file		ZcLoads.h 
\ingroup	LIB_ZcLoads
\date		Nov 26, 2010 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n
\n 		01.00.00 
\n		Nov 26, 2010 
\n		stefano.ughi 
\n		changes 
\n 			Creation
\n
*/

#ifndef __ZC_LOADS_H
#define __ZC_LOADS_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "ZcLoadsExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define PLATFORM2_ZCLOADS

#define LIB_ZCLOAD_NEW_EDGE 1
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
void LIB_ZcLoads_InitSync(void);
void LIB_ZcLoads_Manager( uint8_t newZC );
bool_t LIB_ZcLoadsAreLoadsRegistered(void); 
zcLoadsError_t LIB_ZcLoadsInsertLoadTriac( uint8_t pin );
zcLoadsError_t LIB_ZcLoadsInsertLoadRelay(uint8_t pin);
zcLoadsError_t LIB_ZcLoadsLoadEnable( uint8_t pin, uint8_t profile );
zcLoadsError_t LIB_ZcLoadsLoadDisable(uint8_t pin);

#endif /* __ZC_LOADS_H */

/* *********** */
/* End of file */
/* *********** */



