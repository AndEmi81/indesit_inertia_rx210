/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_Profiler
@{
\n
\n			This module performs pin activations/deactivations following a predetermined profile
\n          described inside Setting File.
\n          Pin activations/deactivations are syncronized with Zero Crossing Voltage.
\n
\image html Profiler.JPG
\n
\par    \b Use:
\n
\n\li   1 call PM_ProfilesManager()
\n\li   2 call PM_ProfilesManager()
\n\li   3 call PM_CheckIfSwitchOffProfile()
\n
\n      so module in addition to the zero crossing ISR also needs others events for
\n      step 2 and 3.
\n      N.B. step 2 is the center of half wave.
\n
\n      The module is used for bi/tri state valve driving, inside fridges
\n
\par    \b Configuration:
\n
\n\li   \b MAX_LOADWITHPROFILE_NUM
\n      Max. number of loads driven by the module
\n
\par    Exported Functions:
\n
\n\li   PM_Profiler_Init()
\n\li   PM_InsertLoadWithProfile()
\n\li   PM_GetLoadWithProfileNumber()
\n\li   PM_ProfileEnable()
\n\li   PM_ProfileDisable()
\n\li   PM_CheckIfSwitchOffProfile()
\n\li   PM_ProfilesManager()
\n\li   PM_IsAtLeastOneProfileActive()
\n\li   PM_IsProfileActive()
\n\li   PM_IsProfileActiveInPos()
\n\li   ProfilerBitToApply()
\n
@}
\ingroup   	ProductM
*/
/*
           -----------            -----------
          |           |          |           |           |
          |-----|---|-|----|---|-|-----|---|-|-----|---|-|
          1     2   3 1    2   3 1     2   3 1     2   3 1
         Z.C.        Z.C.       Z.C.        Z.C.        Z.C.
*/
/**
\n
\par		PM_Profiler.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_Profiler.c
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
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "StandardTypes.h"
#include "Config.h"
#include "PM_Profiler.h"
#include "PM_ProfilerExp.h"
#include "DigitalOutputs.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*lint -esym(754,v1,v2,pin,Reserved) */

/** Control Variable structure */
typedef union profilerModule_Tag
{
	/** First access type */
	struct
	{
		/** pointer to the next profile */
		uint8_t *pNewProfile;
		/** pointer to the current profile */
		uint8_t *pCurrProfile;

		/** pin linked to the load */
		uint8_t pin;
		/** Load Position */
		uint8_t loadID;

		/** if '1' there is an active profile */
		uint8_t Enable     :1;
		/** if '1' profile must be repeated continuously*/
		uint8_t Reload     :1;

		/** Index used to access to Profile Table */
		uint8_t bitIndex   :3;
		/** Index used to access to Profile Table */
		uint8_t byteIndex  :3;

	} bit;
	/** Second access type */
	struct
	{
		/** a vector of pointer's size */
		uint8_t v1[sizeof(void*)];
		/** a vector of pointer's size */
		uint8_t v2[sizeof(void*)];

		/** pin linked to the load */
		uint8_t pin;
		/** Load Position */
		uint8_t loadID;

		/** reserved */
		uint8_t Reserved :2;
		/** Index used to access to Profile Table */
		uint8_t Index    :6;


	} ind;

} profilerModule_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/** Number of loads managed by Module */
static uint8_t ProfilerNumLoads;
/** Vars. to manage a "Profile" load */
static profilerModule_t LoadwithProfile[CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static uint8_t ProfilerBitToApply ( uint8_t i );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Module initialization, \n
Call this function at "cold" reset if SystemMode = NORMAL_MODE.


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void PM_Profiler_Init( void )
{
	(void)memset( &LoadwithProfile[0],0,sizeof(LoadwithProfile));

	ProfilerNumLoads=0;
}
/**
This function is called by LIB_IOM_OutputInitialization() at "cold" reset\n
It performs variables's initialization starting from Setting File.

\param  [in]	pinLinkedToLoad pin to drive the load
\param  [in]	loadID load's position vs Output Map

\return 	profilerError_t

\retval		PROFILER_E_OK no problem
\retval		PROFILER_E_KO too many loads with profile


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
profilerError_t PM_InsertLoadWithProfile( uint8_t pinLinkedToLoad, uint8_t loadID )
{
	if ( ProfilerNumLoads < CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM )
	{
		LoadwithProfile[ProfilerNumLoads].bit.pin = pinLinkedToLoad;
		LoadwithProfile[ProfilerNumLoads].bit.loadID = loadID;
		ProfilerNumLoads++;
		return PROFILER_E_OK;
	}
	else
	{
		return PROFILER_E_KO;
	}
}

/**
This function returns the number of load driven by this module.

\return 	uint8_t

\retval		ProfilerNumLoads

\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t PM_GetLoadWithProfileNumber( void )
{
	return ProfilerNumLoads;
}

/**
This function enables selected profile for a fixed load.\n
The function is called by LIB_IOM_SetLoad().

\param  [in]	pProfile pointer to the profile that must be executed.
\param  [in]	pinLinkedToLoad pin used to manage the load.
\param  [in]	rep if > 0 profile must be continuously executed.


\return 	profilerError_t

\retval		PROFILER_E_KO Problems
\retval		PROFILER_E_OK  All is O.K.


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
profilerError_t PM_ProfileEnable( uint8_t *pProfile, uint8_t pinLinkedToLoad, uint8_t rep )
{
	uint8_t i;

	i=0;
	while ( ( i < CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM ) &&
	        ( LoadwithProfile[i].bit.pin != pinLinkedToLoad ) )
	{
		i++;
	}
	if ( i >= CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM )
	{
		/* è stato passato un pin non "registrato" */
		return PROFILER_E_KO;
	}
	if ( LoadwithProfile[i].bit.Enable )
	{
		/* c'è già un profilo in esecuzione -> metto la richiesta in coda */
		LoadwithProfile[i].bit.pNewProfile=pProfile;
		return PROFILER_E_OK;
	}
	LoadwithProfile[i].bit.pCurrProfile=pProfile;
	LoadwithProfile[i].ind.Index=0;
	if ( rep )
	{
		LoadwithProfile[i].bit.Reload=1;
	}
	LoadwithProfile[i].bit.Enable=1;

	return PROFILER_E_OK;
}

/**
This function disables selected profile for a fixed load.\n
The function is called by LIB_IOM_SetLoad() and by PM_ProfilesManager().

\param  [in] pinLinkedToLoad pin used to manage the load

\return 	profilerError_t

\retval		PROFILER_E_KO Problem
\retval		PROFILER_E_OK No Problem


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
profilerError_t PM_ProfileDisable( uint8_t pinLinkedToLoad )
{
	uint8_t i;
	uint8_t loadIDlocal;

	i=0;
	while ( ( i < CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM ) &&
	        ( LoadwithProfile[i].bit.pin != pinLinkedToLoad ) )
	{
		i++;
	}

	if ( i >= CONFIG_PM_PROFILER_MAX_LOADWITHPROFILE_NUM )
	{
		return PROFILER_E_KO;
	}
	loadIDlocal = LoadwithProfile[i].bit.loadID;
	/* azzera la struttura i-esima agganciata al pin */
	(void)memset( &LoadwithProfile[i],0,sizeof(LoadwithProfile[i]));
	/* ripristina il pin linkato nella struttura */
	LoadwithProfile[i].bit.pin = pinLinkedToLoad;
	LoadwithProfile[i].bit.loadID = loadIDlocal;
	/* disattiva carico */
	LIB_DIO_SET_LOAD((uint8_t)OFF, pinLinkedToLoad);

	return PROFILER_E_OK;
}

/**
This function decide if switch off the load.\n
It must be called before the next Zero Crossing Voltage and acts on bit to apply\n
at next Zero Crossing.\n
In Platform 1 the function was called inside Task_Partialization.


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void PM_CheckIfSwitchOffProfile ( void )
{
	uint8_t i;

	for ( i = 0; i < ProfilerNumLoads; i++ )
	{
		if ( LoadwithProfile[i].bit.Enable )
		{
			if ( ProfilerBitToApply (i) == 0 )
			{
				/* disattiva carico */
				LIB_DIO_SET_LOAD((uint8_t)OFF, LoadwithProfile[i].bit.pin);
			}
		}
	}
}
/**
This function is the Manager of the module.\n
It must be called inside Zero Crossing Voltage ISR and at 1/4 of grid period.

\param  [in]	zcPos '1' if inside Z.C.V. ISR a positive edge was captured \n
                      '0' if a negative edge was captured

\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void PM_ProfilesManager( bool_t zcPos )
{
	uint8_t i;


	for ( i = 0; i < ProfilerNumLoads; i++ )
	{
		if ( ( LoadwithProfile[i].ind.Index || zcPos )
		        && LoadwithProfile[i].bit.Enable )
		{
			LIB_DIO_SET_LOAD(ProfilerBitToApply (i), LoadwithProfile[i].bit.pin);

			if ( LoadwithProfile[i].ind.Index >= 63 )
			{
				/* è stata scansionato tutto l'array di 8 bytes */
				LoadwithProfile[i].ind.Index=0;
				/* verifica se c'è un profilo in coda */
				if ( LoadwithProfile[i].bit.pNewProfile != NULL )
				{
					LoadwithProfile[i].bit.pCurrProfile=LoadwithProfile[i].bit.pNewProfile;
					LoadwithProfile[i].bit.pNewProfile=NULL;
				}
				/* ripetere il profilo ? */
				else if ( LoadwithProfile[i].bit.Reload == 0 )
				{
					/* No -> disabilita il profilo */
					(void)PM_ProfileDisable(LoadwithProfile[i].bit.pin);
				}
				else
				{
				}
			}
			else
			{
				LoadwithProfile[i].ind.Index++;
			}
		}
	}
}

/**
This function returns TRUE if, at least, one profile is active otherwise FALSE\n.
Used by Application.

\return 	bool_t

\retval		TRUE at least one profile is active
\retval		FALSE all profiles are disabled


\author 	Stefano Ughi
\date		24/11/2011
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_IsAtLeastOneProfileActive (void)
{
	uint8_t i;

	for ( i = 0; i < ProfilerNumLoads; i++ )
	{
		if ( LoadwithProfile[i].bit.Enable )
		{
			return (TRUE);
		}
	}
	return (FALSE);

}
/**
This function returns TRUE if Profile i is active, otherwise FALSE\n
Used by Application.

\param  [in]    i profile's index .

\return 	bool_t

\retval		FALSE -> profile i is disabled
\retval		TRUE -> profile i is active


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_IsProfileActive(uint8_t i)
{

	if (LoadwithProfile[i].bit.Enable)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
This function returns TRUE if profile linked to the load in position loadId is active\n
otherwise FALSE.

\param  [in]	loadId load position in Output Map


\return 	bool_t

\retval		TRUE -> profile is active
\retval		FALSE -> profile is disabled


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_IsProfileActiveInPos( uint8_t loadId )
{
	uint8_t i;

	for ( i = 0; i < ProfilerNumLoads; i++ )
	{
		if( ( LoadwithProfile[i].bit.loadID == loadId  ) &&
		        LoadwithProfile[i].bit.Enable )
		{
			return (TRUE);
		}
	}
	return (FALSE);

}

/* Local Function */
/**
This function decides, fixed profile i, if activate or deactivate the pin linked to the load. \n
Below the profile's structure inside setting file.
The bits are scanned from 0 to 7 \n
The bytes are scanned from 0 to 7.\n
\n
\n       7 6 5 4 3 2 1 0
\n +0   | | | | | | | | |	 <-- LoadwithProfile[i].bit.pCurrProfile
\n       - - - - - - - -
\n +1   | | | | | | | | |
\n       - - - - - - - -
\n +2   | | | | | | | | |
\n       - - - - - - - -
\n +3   | | | | | | | | |
\n       - - - - - - - -
\n +4   | | | | | | | | |
\n       - - - - - - - -
\n +5   | | | | | | | | |
\n       - - - - - - - -
\n +6   | | | | | | | | |
\n       - - - - - - - -
\n +7   | | | | | | | | |
\n       - - - - - - - -

\param  [in] i this is the index profile


\return 	uint8_t

\retval		1 -> output On
\retval		0 -> output Off


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static uint8_t ProfilerBitToApply ( uint8_t i )
{
	if ( *(LoadwithProfile[i].bit.pCurrProfile+LoadwithProfile[i].bit.byteIndex)
	        & (0x01 << LoadwithProfile[i].bit.bitIndex) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* *********** */
/* End of file */
/* *********** */




