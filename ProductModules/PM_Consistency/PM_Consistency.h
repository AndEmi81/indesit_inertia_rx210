/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n			header file of PM_Consistency.c
\n
\file		PM_Consistency.h
\ingroup	PM_Consistency
\date		May 31, 2005
\version	01.00.00
\author		Alberto Camerin
\n
\n
\par	History:
\n
\n 		01.00.00
\n		May 31, 2005
\n		Alberto Camerin
\n		changes
\n 			Creation
\n 		01.00.01
\n
\n		Dec 15, 2010
\n		stefano.ughi
\n		changes
\n 			Some modifications cfr. V.C.S.
\n
*/

#ifndef __CONSISTENCY_H
#define __CONSISTENCY_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "PM_ConsistencyExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

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
/** Function used to initialize the the module  */
void PM_Consistency_Init(void);

/** Function to Manage the consistency status of the Loads */
void PM_Consistency_Manager(void);

/** The load changed its status */
void PM_Consistency_LoadChangedStatus( uint8_t loadidx, uint8_t oldloadstatus, uint8_t newloadstatus, uint8_t* pByte, uint8_t pilottype );

#endif /* __CONSISTENCY_H */

/* *********** */
/* End of file */
/* *********** */

