/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		    OCCLK - On Clock Synthesis Driver Source file
\n			    Contains implementation of the OCCLK driver
\n
\file		    OCCLK_drv.c
\ingroup	    OCCLK
\date		    11/03/2010
\version	    00.00.04
\author		    Jaroslav Musil
\n
\n
\Par\b	History:
\n
\n 		        00.00.04
\n          	11/03/2010
\n          	Stefano Ughi
\n            - Changed uint16_t with CLK_TrimmingType M.C.U. dependent.
\n            - Added comments on properties that the user must setup in CLKconfig.h
\n            - Put in order Doxygen documentation
\n
\n 		        00.00.03
\n          	16/02/2010
\n          	Jaroslav Musil
\n            - The function corps depended on chip moved to hal.
\n
\n 		        00.00.02
\n          	15/02/2010
\n          	Jaroslav Musil
\n            - The OCCLK_EngageClkRefSync lines moved to hal.
\n
\n 		        00.00.01
\n          	3/02/2010
\n          	Jaroslav Musil
\n            - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */


#include "OCCLK_drv.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* ******************** */
/* Functions Prototypes */
/* ******************** */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Engage the selected clock signals. The configuration is set up by
the commands in CLKconfig.h.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			If the crystal is turned on, it requires some init
			time. Use the OCCLK_IsExtRefClockReady function
			to check that the crystal is ready.

*/

IO_ErrorType OCCLK_EngageClkRefSync(void)
{
	CLK_EngageClkRefSync();

	return (IO_ErrorType)IO_E_OK;
}


/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the internal
reference clock in the FLL bypass mode.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE  incorrect value of the desired clock

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
IO_ErrorType OCCLK_SetInternalClockSync(uint32_t desiredClock)
{

	/* If wrong desired frequency */
	if ((desiredClock == 0) || (desiredClock > CLK_MAX_DEVICE_FREQ))
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	if (CLK_SetInternalClockSync(desiredClock))
	{
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}

/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the external
reference clock in the FLL bypass mode.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE incorrect value of the desired clock
\retval     IO_E_UNEXPECTED_CONDITION the external reference signal is not present.

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
IO_ErrorType OCCLK_SetExternalClockSync(uint32_t desiredClock)
{
	/* If wrong desired frequency */
	if ((desiredClock == 0) || (desiredClock > CLK_MAX_DEVICE_FREQ))
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	/* If the crystal is not active */
	if (!CLK_IsOscillatorReady())
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	if (CLK_SetExternalClockSync(desiredClock))
	{
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}

/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with FLL on the external
reference clock.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE  incorrect value of the desired clock
\retval     IO_E_UNEXPECTED_CONDITION the external reference signal is not present or
            the frequency range of the clock for FLL not reached

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
IO_ErrorType OCCLK_SetFLLExternalClockSync(uint32_t desiredClock)
{
	/* If wrong desired frequency */
	if ((desiredClock == 0) || (desiredClock > CLK_MAX_DEVICE_FREQ))
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	/* If the crystal is not active */
	if (!CLK_IsOscillatorReady())
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	if (CLK_SetFLLExternalClockSync(desiredClock))
	{
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}

/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with FLL on the internal
reference clock.

\param  [in]	desiredClock 32-bit value of the desired system clock in Hz.

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE incorrect value of the desired clock
\retval     IO_E_UNEXPECTED_CONDITION  the frequency range of the clock for FLL not reached

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
IO_ErrorType OCCLK_SetFLLInternalClockSync(uint32_t desiredClock)
{
	/* If wrong desired frequency */
	if ((desiredClock == 0) || (desiredClock > CLK_MAX_DEVICE_FREQ))
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	if (CLK_SetFLLInternalClockSync(desiredClock))
	{
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}


/**
Checks if the external reference signal is ready.

\return 	bool_t

\retval		1 - ready, 0 - not ready

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
bool_t OCCLK_IsExtRefClockReady(void)
{
	return (bool_t)(CLK_IsOscillatorReady());
}

/**
Sets up the trimming value.
\n\li AG128 The function feeds both the trimming register and the fine trimming bit.
\n\li GB60  The function feeds the trimming register

\param  [in]	desiredTrim
\n\li   AG128 16-bit value where the trim value is the higher 8 bits
\n      and the fine trim bit is the highest bit of the lower 8 bits.
\n\     i.e. trim bit 8 - 15, fine trim bit 7, unused bits 0 - 6.
\n\li   GB60 8 bit value.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Stefano Ughi
\date		09/03/2010
\version	1.0.1
\n typedef input param changed from uint16_t to CLK_TrimmingType

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
IO_ErrorType OCCLK_SetTrimSync(CLK_TrimmingType desiredTrim)
{
	CLK_SetTrimSync(desiredTrim);

	return (IO_ErrorType)IO_E_OK;
}

/**
\n Gets the trimming value.
\n\li AG128 The function reads both the trimming register and the fine trimming bit.
\n\li GB60  The function reads the trimming register

\return 	CLK_TrimmingType

\retval		trimming value

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
CLK_TrimmingType OCCLK_GetTrimSync(void)
{
	return CLK_GetTrimSync();
}


/* Local Function */

/* *********** */
/* End of file */
/* *********** */
