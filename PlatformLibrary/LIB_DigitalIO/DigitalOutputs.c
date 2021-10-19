/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		DigitalOutputs.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		DigitalOutputs.c
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
#define MODULE_BODY_LIB_DIGITAL_IO
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "IOSharing.h"
#include "DigitalOutputs.h"
#include "LIB_Error.h"
#include "OCDIO_drv.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void LIB_DIO_SET_LOAD_F(uint8_t value, uint8_t pin);

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Set the pin value according with pinValue and reiterates it's direction as output.

\param  [in]		pinValue  if 1 -> On if 0 -> off
\param  [in]		pin pinIndex of Module IO Map

\author 	Stefano Ughi
\date		18/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_DIO_SET_LOAD(uint8_t pinValue, uint8_t pin)
{
	if ( pin >= IO_MODULE_MAP_SIZE )
	{
		PLT_ERROR_FATAL(PLT_ERROR_DIG_OUT_1);
		return;  /* Serious Error */
	}

	LIB_DIO_SET_LOAD_F(pinValue, pin);
	(void)OCDIO_SetPinDirValueSync(pin,1);
}
/* Local Function */
/**
Set an output.

\param  [in]	value  if 1 -> On if 0 -> off
\param  [in]	pin pinIndex of Module IO Map

\author 	Stefano Ughi
\date		18/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static void LIB_DIO_SET_LOAD_F(uint8_t value, uint8_t pin)
{
	bool_t actvSense;

	actvSense = LIB_IOM_Get_IODir_ActiveSense(pin);
	/*lint -e514 Unusual use of a Boolean */
	if (value^(!actvSense))
	{
		/*lint +e514 */
		/* pin high */
		OCDIO_SetPinValueSync(pin, 1);
	}
	else
	{
		/* pin low */
		OCDIO_SetPinValueSync(pin, 0);
	}
}

/* *********** */
/* End of file */
/* *********** */

