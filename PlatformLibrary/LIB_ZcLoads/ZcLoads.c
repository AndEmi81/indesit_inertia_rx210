/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		ZcLoads.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		ZcLoads.c
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
\n
*/
/*
    0 1 2 3 4 5 0 1 2 3 4 5 0
    |-|-|-|-|-|-|-|-|-|-|-|-|     case N_LEVEL = 6
     ___________
 ___|           |___________|

   Z.C.         Z.C.       Z.C.

    0 1 2 3 4 5 0 1 2 3 4 5 0
    |-|-|-|-|-|-|-|-|-|-|-|-|     case N_LEVEL = 6
     ___         ___
 ___|   |_______|   |_______|     triac att = 0; disatt = 1

   Z.C.         Z.C.       Z.C.

    0 1 2 3 4 5 0 1 2 3 4 5 0
    |-|-|-|-|-|-|-|-|-|-|-|-|     case N_LEVEL = 6
     _____________
 ___|             |__________     relay att = 0; disatt = 1

   Z.C.         Z.C.       Z.C.
*/

#define MODULE_BODY_LIB_ZCLOADS
/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "ZcLoadsExp.h"
#include "ZcLoads.h"
#include "DigitalOutputs.h"
#include "LIB_Error.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Intervals of activation and deactivation of the load */
typedef struct profileAttDis_s_Tag
{
	/** load deactivation */
	uint8_t dis	 :4;
	/** load activation  */
	uint8_t att	 :4;

} profileAttDis_s;
/** Access to the fields of activation and deactivation intervals of the load with byte or with bit */
typedef union profileAttDis_u_Tag
{
	/** byte access */
	uint8_t byt;
	/** bit access */
	profileAttDis_s field;

} profileAttDis_u;
/*lint -esym(754,free,byt) */
/** Configuration tree for each load type ZcLoad */
typedef struct zcLoads_t_Tag
{

	union
	{
		/** byte access */
		uint8_t byt;
		struct
		{
			/** state for a load "relay" type */
			uint8_t relay_state	:1;
			/** free */
			uint8_t free		:1;
			/** pin linked to the load */
			uint8_t pin			:6;
		} field;

	} Config;

	profileAttDis_u Profile;

} zcLoads_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */
/** Bitfield used to enable/disable loads of this type */
zcLoads_output_t ZcLoads_Enabled;
/** Indicates which interval is acting LIB_ZcLoads_Manager() */
uint8_t ZcLoads_Index;

/* Static */
/** Number of loads managed by this module and driven with a triac */
static uint8_t ZcLoads_TriacsRegistered;
/** Number of loads managed by this module and driven with a relay */
static uint8_t ZcLoads_RelaysRegistered;
/** Configuration's variable for a load managed by this module */
static zcLoads_t ZcLoads_[ZC_LOADS_NUM];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static zcLoadsError_t ZcLoadsTooManyLoads( void );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Module initialization, \n
Call this function at "cold" reset if SystemMode = NORMAL_MODE.


\author 	Stefano Ughi
\date		26/10/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void LIB_ZcLoads_InitSync(void)
{
	ZcLoads_Enabled=0;
	ZcLoads_TriacsRegistered=0;
	ZcLoads_RelaysRegistered=0;
	(void)memset(&ZcLoads_[0],0,sizeof(ZcLoads_));
}

/**
This function performs activation/deactivation af a "ZcLoad" load.
\n The function is called inside Zero Crossing Voltage ISR and every HalfWaveLength/N_LEVEL event,
\n where N_LEVEL is the number of intervals in which we divide the half period.

\param  [in]	newZC if '1' the function is called by Zero Crossing Voltage ISR\n
                      if '0' the function is called by HalfWaveLength/N_LEVEL event.


\author 	Stefano Ughi
\date		26/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_ZcLoads_Manager( uint8_t newZC )
{
	zcLoads_output_t zcLoadsMask;
	uint8_t i;
	uint8_t loadState;


	if ( LIB_ZCLOAD_NEW_EDGE == newZC )
	{
		/* la chiamata � avvenuta da parte dell'ISR dello zero crossing */
		ZcLoads_Index=0;
	}
	else
	{
		/* la chiamata � avvenuta nell'evento HalfWaveLength/N_LEVEL */
		ZcLoads_Index++;
	}

	/* Check carichi pilotati dai Triacs */
	zcLoadsMask = 1;
	for ( i = 0; i < ZcLoads_TriacsRegistered; i++ )
	{
		loadState = OFF;
		if ( zcLoadsMask & ZcLoads_Enabled )
		{
			if ( ( ZcLoads_Index >= ZcLoads_[i].Profile.field.att )
			        && ( ZcLoads_Index <= ZcLoads_[i].Profile.field.dis ) )
			{
				loadState=ON;
			}
		}
		zcLoadsMask<<=1;

		LIB_DIO_SET_LOAD(loadState,ZcLoads_[i].Config.field.pin);

	}

	/* Check carichi pilotati dai Relays */
	zcLoadsMask = (1<<(ZC_LOADS_NUM-1));


	for ( i = 0; i < ZcLoads_RelaysRegistered; i++ )
	{
		loadState=OFF;

		if ( zcLoadsMask & ZcLoads_Enabled )
		{
			/* richiesta attivazione */
			if ( ZcLoads_[(ZC_LOADS_NUM-1)-i].Config.field.relay_state )
			{
				/* carico gi� attivo -> ribadisco l'ON del carico */
				loadState=ON;
			}
			else
			{
				/* il carico � off:
				   ZcLoads_Index == ZcLoads_[(ZC_LOADS_NUM-1)-i].Profile.field.att deve essere compreso
				   tra 0 e (N_LEVEL-1) */
				if ( ZcLoads_Index == ZcLoads_[(ZC_LOADS_NUM-1)-i].Profile.field.att )
				{
					loadState=ON;
					/* flag carico attivo */
					ZcLoads_[(ZC_LOADS_NUM-1)-i].Config.field.relay_state=1;
				}
			}
		}
		else
		{
			/* richiesta disattivazione */
			if ( ZcLoads_[(ZC_LOADS_NUM-1)-i].Config.field.relay_state )
			{
				/* il carico � attivo:
				   ZcLoads_[(ZC_LOADS_NUM-1)-i].Profile.field.dis deve essere compreso
				   tra 0 e (N_LEVEL-1) */
				if ( ZcLoads_Index != ZcLoads_[(ZC_LOADS_NUM-1)-i].Profile.field.dis )
				{
					loadState=ON;
				}
				else
				{
					ZcLoads_[(ZC_LOADS_NUM-1)-i].Config.field.relay_state=0;
				}
			}
		}

		zcLoadsMask>>=1;

		LIB_DIO_SET_LOAD(loadState,ZcLoads_[(ZC_LOADS_NUM-1)-i].Config.field.pin);

	}

}

/**
This function returns TRUE if there are loads managed by this module otherwise FALSE.

\return 	bool_t

\retval		TRUE -> there are loads managed by this module
\retval		FALSE -> there aren't loads managed by this module


\author 	Stefano Ughi
\date		26/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t LIB_ZcLoadsAreLoadsRegistered(void)
{
	if ( ( ZcLoads_RelaysRegistered != 0 ) || ( ZcLoads_TriacsRegistered != 0 ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/**
This function initialize loads of "triac" type.
\n The function is called by LIB_IOM_OutputInitialization() at "cold" reset.

\param  [in]	pin pin linked to the load (cfr. IO Module Map)

\retval		ZC_LOADS_E_KO too many zc loads
\retval		ZC_LOADS_E_OK no Problems

\author 	Stefano Ughi
\date		26/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
zcLoadsError_t LIB_ZcLoadsInsertLoadTriac( uint8_t pin )
{
	if ( ZcLoadsTooManyLoads() == ZC_LOADS_E_KO )
	{
		return ZC_LOADS_E_KO;
	}
	ZcLoads_[ZcLoads_TriacsRegistered].Config.field.pin=pin;
	ZcLoads_TriacsRegistered++;
	return ZC_LOADS_E_OK;
}


/**
This function initialize loads of "relay" type.
\n The function is called by LIB_IOM_OutputInitialization() at "cold" reset.

\param  [in]	pin pin linked to the load (cfr. IO Module Map)

\retval		ZC_LOADS_E_KO too many zc loads
\retval		ZC_LOADS_E_OK no Problems

\author 	Stefano Ughi
\date		26/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
zcLoadsError_t LIB_ZcLoadsInsertLoadRelay(uint8_t pin)
{
	if ( ZcLoadsTooManyLoads() == ZC_LOADS_E_KO )
	{
		return ZC_LOADS_E_KO;
	}

	ZcLoads_RelaysRegistered++;
	ZcLoads_[ZC_LOADS_NUM-ZcLoads_RelaysRegistered].Config.field.pin=pin;
	return ZC_LOADS_E_OK;
}

/**
This function sets, for the load identified by the pin linked with it,
\n the activation and deactivation profile.
\n
\n    0 1 2 3 4 5 0
\n    |-|-|-|-|-|-|
\n     ___________
\n ___|			  |__________
\n
\n The function is called inside LIB_IOM_SetLoad()

\param  [in]	pin pin linked to the load (cfr. IO Map)
\param  [in]	profile high nibble indicates in which segment the load must be activated \n
                        low nibble indicates in which segment the load must be deaactivated .


\return 	zcLoadsError_t

\retval		ZC_LOADS_E_KO problems
\retval		ZC_LOADS_E_OK no problems


\author 	Stefano Ughi
\date		26/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
zcLoadsError_t LIB_ZcLoadsLoadEnable( uint8_t pin, uint8_t profile )
{
	profileAttDis_u profileAttDis;
	zcLoads_output_t zcLoadsMask;
	uint8_t i;

	profileAttDis.byt = profile;
	zcLoadsMask = 1;

	if ( ( profileAttDis.field.att >= N_LEVEL ) || ( profileAttDis.field.dis >= N_LEVEL ) )
	{
		return ZC_LOADS_E_KO;
	}

	for ( i=0; i < ZC_LOADS_NUM; i++ )
	{
		if ( pin == ZcLoads_[i].Config.field.pin )
		{
			/* per i carichi zc loads pilotati da triac */
			if ( ( i < ZcLoads_TriacsRegistered ) && ( profileAttDis.field.dis <= profileAttDis.field.att ) )
			{
				return ZC_LOADS_E_KO;
			}
			ZcLoads_[i].Profile.byt=profileAttDis.byt;
			ZcLoads_Enabled|=zcLoadsMask;

			return ZC_LOADS_E_OK;
		}
		zcLoadsMask<<=1;
	}

	return ZC_LOADS_E_KO;
}
/**
This function deactivates, for the load identified by the pin linked with it,
\n the profile.
\n The function is called inside LIB_IOM_SetLoad()

\param  [in]	pin pin linked to the load (cfr. IO Map)

\return 	zcLoadsError_t

\retval		ZC_LOADS_E_KO problems
\retval		ZC_LOADS_E_OK no problems


\author 	Stefano Ughi
\date		26/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

zcLoadsError_t LIB_ZcLoadsLoadDisable(uint8_t pin)
{
	zcLoads_output_t zcLoadsMask;
	uint8_t i;

	zcLoadsMask = 1;
	for ( i = 0; i < ZC_LOADS_NUM; i++)
	{
		if ( pin == ZcLoads_[i].Config.field.pin )
		{
			ZcLoads_Enabled &= (zcLoads_output_t)(~(uint32_t)zcLoadsMask);
			return ZC_LOADS_E_OK;
		}
		zcLoadsMask<<=1;
	}
	return ZC_LOADS_E_KO;
}

/* Local Function */

/**
This function checks if there are too many loads of "ZcLoad" type.


\return 	zcLoadsError_t

\retval		ZC_LOADS_E_KO too many loads
\retval		ZC_LOADS_E_OK no problems

\author 	Stefano Ughi
\date		26/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

static zcLoadsError_t ZcLoadsTooManyLoads( void )
{
	if ( ( ZcLoads_TriacsRegistered+ZcLoads_RelaysRegistered ) > ZC_LOADS_NUM )
	{
		PLT_ERROR_WARNING(PLT_ERROR_ZC_LOADS_1);
		return ZC_LOADS_E_KO;
	}
	return ZC_LOADS_E_OK;
}

/* *********** */
/* End of file */
/* *********** */



