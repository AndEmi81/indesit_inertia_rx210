/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		DigitalInputs.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		DigitalInputs.c
\ingroup	DigitalInputs
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		Jun 11, 2010
\n		01.00.00
\n		stefano.ughi
\n		changes
\n 		- creation
\n
\n etc.
*/
#define MODULE_BODY_LIB_DIGITAL_IO
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include <string.h>
#include "IOSharing.h"
#include "DigitalInputs.h"
#include "LIB_Error.h"
#include "OCDIO_drv.h"

#include "LIB_ClassBVar.h"
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
/** bit map, bit i -> result of the input in pos. i */
dig_input_t DigInputs;
/** bit map, bit i=1 -> result of the input in pos. i is ready */
dig_input_t DigInputsReady;
/** bit map, bit i=1 -> read on input in pos.i is disabled */
dig_input_t DigInputsDisable;
/* Static */
/* Input Acq. Configuration */
static digitalInput_t DigInputPin[DIGINPUT_NUM];
/** Number of '1' collected */
static uint8_t DigInput_val[DIGINPUT_NUM];
/** Counter */
static uint8_t DigInput_c[DIGINPUT_NUM];

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
\n Initialization of data structures relating to digital inputs.
\n This function is called inside OnColdReset() if SystemMode == SYSTEM_NORMAL_MODE

\attention \b DigInputs (Global) W
\attention \b DigInputsReady (Global) W
\attention \b DigInputsDisable (Global) W

\attention \b DigInputPin (Global) W
\attention \b DigInput_val (Global) W
\attention \b DigInput_c (Global) W

\author 	stefano.ughi
\date		Jun 16, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_DIO_InitDigInputs(void)
{
	SAFETY_REGION_START();
	DigInputs           = 0;
	DigInputsDisable    = 0;
	SAFETY_DATA_UPDATE( DigInputs );
	SAFETY_DATA_UPDATE( DigInputsDisable );
	SAFETY_REGION_END();

	DigInputsReady      = 0;

	(void)memset(&DigInputPin[0], 0, sizeof(DigInputPin));
	(void)memset(&DigInput_val[0], 0, sizeof(DigInput_val));
	(void)memset(&DigInput_c[0], 0, sizeof(DigInput_c));

}
/**
\n Digital Input setup.


\param  [in] pos Ram Position Conversion's Result.
\param  [in] pin pin linkage module M.C.U. - 1..
\param  [in] readtype '1' -> multiplexed input '0' -> not multiplexed input.
\param  [in] pConfigData pointer to config. params.

\attention \b DigInputPin[] (global) used as R/W


\author 	Stefano Ughi
\date		11/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			Call in LIB_IOM_InputInitialization() only if reset is cold.
*/
void LIB_DIO_InsertDigInput ( uint8_t pos, uint8_t pin, uint8_t readtype, const uint8_t *pConfigData )
{
	digitalInput_t *pDigInputPin;

	if (!((pos < DIGINPUT_NUM) && (pin < IO_MODULE_MAP_SIZE)))
	{
		PLT_ERROR_FATAL(PLT_ERROR_DIG_IN_1);
		return; /* Serious Error */
	}

	pDigInputPin = &DigInputPin[pos];

	pDigInputPin->s.readtype = ((MPX_INPUT == readtype)?1:0);
	pDigInputPin->s.pin = pin;
	/* filtering */
	/*lint -e826 Suspicious pointer-to-pointer conversion (verified area)*/
	pDigInputPin->s.filterPot2 = (((digitalLLIparams_t*)((void*)pConfigData))->bit.nRead);
	/*lint +e826 */

	/* is read enabled ? */
	/*lint -e826 Suspicious pointer-to-pointer conversion (verified area)*/
	pDigInputPin->s.active = (((digitalLLIparams_t*)((void*)pConfigData))->bit.automatic?1:0);
	/*lint +e826 */

	/*
	N.B.: don't care pull-up info because pull-up is already set in LIB_IOM_InputInitialization()
	*/

#if defined __MULTI_H
	/* mpx config. setup */
	pDigInputPin->s.chsel = (((digitalMpxLLIparams_t*)pConfigData)->bit.mpxCfg);
	pDigInputPin->s.inversion = (((digitalMpxLLIparams_t*)pConfigData)->bit.inverted?1:0);
	pDigInputPin->s.pullupEn = (((digitalMpxLLIparams_t*)pConfigData)->bit.pullUp?1:0);
#endif /* __MULTI_H  */

}
/**
\n LIB_DIO_DigInputManager.
\n Manage all digital inputs.

\attention \b  DigInputPin[]  (global) Input Description.
\attention \b  DigInput_c[]   (global) Number of samples collected for the i-th input
\attention \b  DigInput_val[] (global) Number of times i-th input was high (logical value,not real value).

\attention \b DigInputs (global) bit map, bit i -> result of the input in pos. i
\attention \b DigInputsDisable (global) bit map, bit i -> read on input in pos.i is disabled
\attention \b DigInputsReady (global) bit map, bit i -> result of the input in pos. i is ready


\author 	Stefano Ughi
\date		08/10/2010
\version	1.0.2

\todo       check __MULTI_H

\note		\par \b Nota 1:
			This function is called inside Task_10_ms if SystemMode = SYSTEM_NORMAL_MODE,\n
			The call period depends from  DIGITAL_INPUT_SAMPLE_TIME property \n
			Documentation by Stefano Ughi

*/
void LIB_DIO_DigInputManager(void)
{
	uint8_t i;
	uint8_t nSamplesOnPin;
	dig_input_t mask = 1;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(DigInputs);
	SAFETY_DATA_CHECK(DigInputsDisable);

	for ( i = 0; i < DIGINPUT_NUM; i++ )
	{
		if ( DigInputPin[i].s.active == TRUE )
		{
			/* the input is enabled */
#ifdef __MULTI_H
			if ( DigInputPin[i].s.readtype )
			{
				/* the input is multiplexed */

				LIB_IOM_Set_IODir_ActiveSense(DigInputPin[i].s.inversion,DigInputPin[i].s.pin);
				if ( DigInputPin[i].s.pullupEn )
				{
					(void)OCDIO_SetPinPullUpSync(DigInputPin[i].s.pin);
				}
				else
				{
					(void)OCDIO_ResetPinPullUpSync(DigInputPin[i].s.pin);
				}
				/* Mpx Selection */
				PM_setInMux(DigInputPin[i].s.chsel);
			}
#endif /* #ifdef __MULTI_H */

#ifdef DIGITAL_LOCAL_FILTER_ENABLED
			if (LIB_DIO_GET_INPUT_F (DigInputPin[i].s.pin, DIGITAL_LOCAL_FILTER_NUM))
			{
				DigInput_val[i]++;
			}
#else
			if (LIB_DIO_GET_INPUT (DigInputPin[i].s.pin))
			{
				DigInput_val[i]++;
			}
#endif

			DigInput_c[i]++;
			/* Number of readings to do before making the decision */
			nSamplesOnPin = (uint8_t)(1u<<DigInputPin[i].s.filterPot2);

			if (DigInput_c[i] >= nSamplesOnPin)
			{
				DigInput_c[i]=0;

				if ( DigInput_val[i] >= (uint8_t)( nSamplesOnPin-(nSamplesOnPin/4) ) )
				{
					if ((DigInputsDisable & mask) == 0)
					{
						/* The input is enabled so update the final value */
						DigInputs |= (mask);
					}
				}
				else if ( DigInput_val[i] <= (nSamplesOnPin/4) )
				{
					if ((DigInputsDisable & mask) == 0)
					{
						/* The input is enabled so update the final value */
						DigInputs &= (dig_input_t)(~(uint32_t)mask);
					}
				}
				else
				{
					/* stay with the old value */
				}

				DigInputsReady |= (mask);

				DigInput_val[i]=0;
			}
		}
		mask = (dig_input_t)( mask << 1 );

	}
	SAFETY_DATA_UPDATE( DigInputs );
	SAFETY_REGION_END();
}

/**
\n Fixed the pin index inside the Module IO Map return it's value.
\n real value if there isn't inversion, otherwise inverted value.

\param  [in] pin pin index of ModuleIOmap.

\return 	bool_t

\retval		input pin value.

\author 	Stefano Ughi
\date		17/10/2010
\version	1.0.0

*/
bool_t LIB_DIO_GET_INPUT( uint8_t pin )
{
	bool_t pinValue;
	bool_t pinSensing;

	pinValue = OCDIO_GetPinValueSync(pin);

	pinSensing = LIB_IOM_Get_IODir_ActiveSense(pin);

	return ( (bool_t)(pinValue ^ pinSensing) );
}
/**
\n Fixed the pin index inside the Module IO Map return it's value.
\n real value if there isn't inversion, otherwise inverted value
\n Reading is filtered with filter value equal to filterValue [range 0-127]
\n The decision on the fact that the value of the pin is high or low is made by a majority.

\param  [in] pin pin index of ModuleIOmap.
\param  [in] filterValue number of samples to collect.

\return 	bool_t

\retval		input pin value.


\author 	Stefano Ughi
\date		18/10/2010
\version	1.0.0

*/
bool_t LIB_DIO_GET_INPUT_F( uint8_t pin, int8_t filterValue )
{
	int8_t value=0;

	while ( filterValue >= 0 )
	{
		if ( LIB_DIO_GET_INPUT( pin ) )
		{
			value ++;
		}
		else
		{
			value --;
		}
		filterValue --;
	}

	if ( value > 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
\n The function returns TRUE if all active digital inputs are ready otherwise FALSE.

\attention \b DigInputPin[] (global) Input Description.
\attention \b DigInputsReady (global) bit map, bit i -> result of the input in pos. i is ready.

\return 	bool_t

\retval		TRUE
\retval     FALSE

\author 	Stefano Ughi
\date		09/07/2010
\version	1.0.0


*/

bool_t LIB_DIO_AreDigInputsReady(void)
{
	uint8_t i;

	for ( i = 0; i < DIGINPUT_NUM; i++ )
	{
		if ( DigInputPin[i].s.active )
		{
			if ( 0 == ( DigInputsReady & ( ( (dig_input_t)1u ) << i ) ) )
			{
				return (FALSE);
			}
		}
	}

	return (TRUE);
}
/**
Fixed pos return the input's value.

\param  [in] pos Input position described inside Setting File
\attention \b DigInputs (global) bitfield

\return 	uint8_t

\retval		TRUE the input in position pos is high
\retval	    FALSE the input in position pos is low


\author 	Stefano Ughi
\date		09/07/2010
\version	1.0.0

*/

bool_t LIB_DIO_GetDigInputVal(uint8_t pos)
{
	if ( DigInputs & ( (dig_input_t)(1u<<pos) ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
Set the input value identified by its position (position is a setting file parameter).

\param  [in] pos Input position described inside Setting File
\param  [in] value to set
\attention \b DigInputs (global) bitfield


\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

\note		\par \b Nota 1:
			Disable automatic read before use this function.

*/

void LIB_DIO_SetDigInputVal(uint8_t pos, uint8_t value)
{
	SAFETY_REGION_START();
	if(value)
	{
		DigInputs |= (dig_input_t)(1u << pos);
	}
	else
	{
		DigInputs &= (dig_input_t)(~(1u << pos));
	}
	SAFETY_DATA_UPDATE( DigInputs);
	SAFETY_REGION_END();
}

/**
\n The function returns TRUE if the digital input identified by its position is ready otherwise FALSE.

\param  [in] pos Input position described inside Setting File
\attention \b DigInputsReady (global) bitfield

\return 	bool_t

\retval		TRUE
\retval		FALSE

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

*/

bool_t LIB_DIO_IsDigitalReady(uint8_t pos)
{
	return (bool_t) ((DigInputsReady & (dig_input_t)(1u << pos)) ? TRUE : FALSE);
}

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

