/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		IOSharing.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		IOSharing.c
\ingroup	InputOutputShare
\date		Jun 10, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Jun 10, 2010
\n		stefano.ughi
\n		changes
\n 		- creation
\n
\n 		01.00.01
\n		Apr 01, 2011
\n		stefano.ughi
\n		changes
\n 		- #include "Trash.h" under defines
\n		- added #include "DIO_cfg.h"
\n      - void LIB_IOM_IO_Manager_Init(void) call InitCalibrationPointers_trash(); under defines
\n etc.
*/
#define MODULE_BODY_LIB_IOMANAGER
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include <string.h>
#include "IOSharing.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if (IO_MODULE_MAP_SIZE%BYTE_SIZE)
#define ACT_SENSE_SIZE_VECT (IO_MODULE_MAP_SIZE/BYTE_SIZE+1)
#else
#define ACT_SENSE_SIZE_VECT (IO_MODULE_MAP_SIZE/BYTE_SIZE)
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
/*
In this vector we store the following info:
for each input if the pin's read must be inverted or not (1 inverted)
for each output if the pin is active high or low (1 active high)
*/
uint8_t PinIOActvSense[ACT_SENSE_SIZE_VECT];
/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
PinIOActvSense[] setup.

\param  [in]		actSense if '1' inversion
\param  [in]		pinIndex (pinIndex-1) of M.C.U. Linkage Module


\author 	Stefano Ughi
\date		10/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_IOM_Set_IODir_ActiveSense(bool_t actSense,uint8_t pinIndex)
{
	uint8_t mask;
	uint8_t offset;
    
	mask = (uint8_t)(1u << (pinIndex%BYTE_SIZE));
	offset = (uint8_t)(pinIndex/BYTE_SIZE);

	if (offset < ACT_SENSE_SIZE_VECT)
	{
		if (actSense)
		{
			PinIOActvSense[offset] |= mask;
		}
		else
		{
			PinIOActvSense[offset] &= (uint8_t)(~(uint32_t)mask);
		}
	}
}
/**
Fixed a pinIndex return if the pin must be inverted or not.

\param  [in]		pinIndex (pinIndex-1) of M.C.U. Linkage Module

\return bool_t
\retval TRUE inversion
\retval FALSE no inversion

\author 	Stefano Ughi
\date		11/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_IOM_Get_IODir_ActiveSense(uint8_t pinIndex)
{
	uint8_t mask;
	uint8_t offset;

	mask = (uint8_t)(1u << (pinIndex%BYTE_SIZE));
	offset = (uint8_t)(pinIndex/BYTE_SIZE);

	if (PinIOActvSense[offset]&mask)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
\n Initialization.
\n The function must be called if SystemMode == SYSTEM_NORMAL_MODE regardless reset's type.

\attention \b PinIOActvSense[] used as W

\author 	Stefano Ughi
\date		07/09/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_IOM_IO_Manager_Init(void)
{
	(void)memset(&PinIOActvSense[0], 0, sizeof(PinIOActvSense) );
}

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

