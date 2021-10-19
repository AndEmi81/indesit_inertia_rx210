/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		IOSharing.h
\n			Header file of IOSharing.c
\n
\file		IOSharing.h
\ingroup	InputOutputShare
\date		Jun 10, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Jun 10, 2010
\n		stefano.ughi
\n		changes
\n 		- creation
\n
*/

#ifndef __IOSHARING_H
#define __IOSHARING_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
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
void LIB_IOM_Set_IODir_ActiveSense(bool_t actSense,uint8_t pinIndex);
bool_t LIB_IOM_Get_IODir_ActiveSense(uint8_t pinIndex);
void LIB_IOM_IO_Manager_Init(void);
#endif /* __IOSHARING_H */

/* *********** */
/* End of file */
/* *********** */
