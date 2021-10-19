/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_DamperOnOff
@{
\n  This module manages the Damper On Off, a device used by no frost fridges.
\n
\par Configuration:
\n
\n  none
\n
\par Exported Functions:
\n
\n\li   PM_Damper_ONOFF_Init()
\n\li   PM_Damper_ONOFF_InitParams()
\n\li   PM_Damper_ONOFF_SetPosition()
\n\li   PM_Damper_ONOFF_Manager()
\n\li   PM_Damper_ONOFF_Refresh()
\n\li   PM_Damper_ONOFF_GetPosition()
\n\li   PM_Damper_ONOFF_GetTmaxReachPos()
\n\li   PM_Damper_ONOFF_IsDriven()
\n\li   PM_Damper_ONOFF_GetReedVal()
\n\li   damper_ONOFF_reedIsAcq()
\n\li   damper_ONOFF_PositionReached()
\n\li   DamperONOFF_Positioning()
\n\li   Damper_ONOFF_SetVarsForNewPosition()
\n\li   Damper_ONOFF_SetPylot()
\n
@}
\ingroup   	ProductM
*/
/**
\n
\par		PM_Damper_ONOFF.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_Damper_ONOFF.c
\ingroup	PM_DamperOnOff
\date		Nov 18, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 18, 2010
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
#include "StandardDefines.h"
#include "PM_Damper_ONOFF.h"
#include "InputManagerExp.h"
#include "DigitalInputsExp.h"
#include "FeedbacksExp.h"
#include "DigitalOutputs.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
\n Configuration data for the module.
\n
\n  |   Index             | ReedType            |
\n  ---------------------------------------------
\n  | Extra Time [10msec] | TmaxReachPos [1sec] |
\n
\n Index:  position inside feedback vector or position inside bitmap containig readings.
\n ReedType:   read type of reed linked to damper, feedback or digital.
\n Extra Time: after the device has reached the position, get from reed, continues to be driven for Extra Time (time base 10 msec).
\n             be attention on time to have reed's acquisition
\n TmaxReachPos: max. time to reach position (Used for fault)
*/

/** Position, inside setting file, of Index */
#define POS_INDEX           0
/** Position, inside setting file, of ReedType */
#define POS_READ_TYPE       1
/** Position, inside setting file, of Extra Time */
#define POS_EXTRA_TIME      2
/** Position, inside setting file, of TmaxReachPos parameter */
#define POS_TIME_MAX_R_POS  3


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
damperONOFFStruct_t DamperONOFFVars;

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static bool_t damper_ONOFF_reedIsAcq(void);
static bool_t damper_ONOFF_PositionReached(void);
static bool_t DamperONOFF_Positioning(void);
static void Damper_ONOFF_SetVarsForNewPosition( uint8_t damperPosReq );
static void Damper_ONOFF_SetPylot( uint8_t pylotState );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Module's Initialization. \n
Call this functon at "cold" reset.

\return 	damperONOFFError_t

\retval		DAMPER_ONOFF_E_OK


\author 	stefano.ughi
\date		Nov 16, 2010
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi
*/
damperONOFFError_t PM_Damper_ONOFF_Init(void)
{
	(void)memset( &DamperONOFFVars,0,sizeof(DamperONOFFVars));
	return DAMPER_ONOFF_E_OK;
}
/**
Set up parameters for the module.\n
This function is called by LIB_IOM_OutputInitialization() if reset is "cold".

\param  [in]    pin pin to drive the device, offset vs module IO map.
\param  [in]	pDataComp pointer to the configuration device data.

\return 	damperONOFFError_t

\retval		DAMPER_ONOFF_E_OK

\author 	stefano.ughi
\date		Nov 16, 2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

damperONOFFError_t PM_Damper_ONOFF_InitParams(uint8_t pin,const uint8_t *const pDataComp)
{
	DamperONOFFVars.pDataDamperONOFF = pDataComp;
	DamperONOFFVars.pin = pin;
	DamperONOFFVars.currPos = DAMPER_ONOFF_POS_UNDEF;
	DamperONOFFVars.reqPos = DAMPER_ONOFF_POS_UNDEF;
	/* module initialized */
	DamperONOFFVars.state.bit.init = 1;
	return DAMPER_ONOFF_E_OK;
}

/**
\n This function decides if a new positioning's request can be accepted.
\n If true the request is forwarded to the manager.
\n Called by SetLoad().

\param  [in]	damperPosReq new damper position

\return 	damperONOFFError_t

\retval		DAMPER_ONOFF_E_KO Damper module not initialized or request not valid or Damper is moving.
\retval     DAMPER_ONOFF_E_OK Damper's position reached or positioning's request sent to Manager.

\author 	Stefano Ughi
\date		17/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			if refresh is ongoing, Damper's positioning will be performed at the end of refresh\n
			if SetLoad() is continually called.

*/
damperONOFFError_t PM_Damper_ONOFF_SetPosition(uint8_t damperPosReq)
{

	if ( 0 == DamperONOFFVars.state.bit.init )
	{
		/* Module not initialized */
		return DAMPER_ONOFF_E_KO;
	}
	/* Damper Manager Enabled */
	DamperONOFFVars.state.bit.stopManager = 0;

	if ( DAMPER_ONOFF_STOP_PYLOT == damperPosReq )
	{
		/* Request to suspend Damper handling */
		Damper_ONOFF_SetPylot( (uint8_t)OFF );
		/* Damper Manager Disabled */
		DamperONOFFVars.state.bit.stopManager = 1;
		return DAMPER_ONOFF_E_OK;
	}

	if ( ( damperPosReq != DAMPER_ONOFF_POS_CLOSED ) &&
	        ( damperPosReq != DAMPER_ONOFF_POS_OPEN ) )
	{
		/* Damper position not covered */
		return DAMPER_ONOFF_E_KO;
	}

	if ( DamperONOFF_Positioning() == TRUE )
	{
		/* Damper is moving -> I do not accept requests */
		return DAMPER_ONOFF_E_KO;
	}

	if ( DamperONOFFVars.currPos == damperPosReq )
	{
		/* Requested position equal to current position --> nothing to do  */
	}
	else
	{
		/* Setup variables for new position */
		Damper_ONOFF_SetVarsForNewPosition( damperPosReq );
	}

	return DAMPER_ONOFF_E_OK;

}


/**
Damper On Off Manager.

\return 	damperONOFFError_t

\retval		DAMPER_ONOFF_E_KO Damper module not initialized or first acquisition on the reed\n
            linked to the Damper not ready.

\retval		DAMPER_ONOFF_E_OK Manager is working


\author 	Stefano Ughi
\date		17/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2;
			We assume that Manager is called every 10 msec, cfr DamperONOFFVars.timeAfterPosReached\n
			If this assumption is wrong please adjust DamperONOFFVars.timeAfterPosReached update.
			\par \b Nota 3;
			Add the reed update time to DamperONOFFVars.timeAfterPosReached counter in order \n
			to obtain the real delay between pos reached and stop driving instant.

*/

damperONOFFError_t PM_Damper_ONOFF_Manager(void)
{

	if ( 0 == DamperONOFFVars.state.bit.init)
	{
		/* Module not initialized */
		return DAMPER_ONOFF_E_KO;
	}

	if ( damper_ONOFF_reedIsAcq() == FALSE )
	{
		/* First acquisition on reed, linked to the damper, not ready */
		return DAMPER_ONOFF_E_KO;
	}

	if ( 1 == DamperONOFFVars.state.bit.stopManager )
	{
		/* Manager operations suspended */
		return DAMPER_ONOFF_E_OK;
	}
	if ( DamperONOFF_Positioning() == TRUE )
	{
		/* Damper is moving */
		if ( damper_ONOFF_PositionReached() == TRUE )
		{
			/* Reed's value congruent with damper's position */
			if ( 0 == DamperONOFFVars.timeAfterPosReached )
			{
				/* Extra time elapsed --> stop  */
				Damper_ONOFF_SetPylot( (uint8_t)OFF );
				DamperONOFFVars.currPos = DamperONOFFVars.reqPos;

				/* Check if refresh  */
				if ( 1 == DamperONOFFVars.state.bit.refresh )
				{
					Damper_ONOFF_SetVarsForNewPosition( DamperONOFFVars.returnPos );
					DamperONOFFVars.state.bit.refresh = 0;
				}
			}
			else
			{
				DamperONOFFVars.timeAfterPosReached --;
				Damper_ONOFF_SetPylot( (uint8_t)ON );
			}
		}
		else
		{
			Damper_ONOFF_SetPylot( (uint8_t)ON );
		}
	}
	else
	{
		Damper_ONOFF_SetPylot( (uint8_t)OFF );
	}
	return DAMPER_ONOFF_E_OK;

}
/**
This function verify conditions to submit a refresh request to the manager.
\n The refresh consists in moving the damper in the opposite position respect the current and
\n than moving the damper versus the previous position.
\n If the Damper is moving the refresh request isn't submitted.

\author 	Stefano Ughi
\date		17/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

void PM_Damper_ONOFF_Refresh(void)
{
	if ( 0 == DamperONOFFVars.state.bit.init )
	{
		/* Module not initialized */
		return;
	}

	if ( DamperONOFF_Positioning() == FALSE )
	{
		/* Damper is not moving  */
		if ( DamperONOFFVars.currPos == DAMPER_ONOFF_POS_CLOSED )
		{
			Damper_ONOFF_SetVarsForNewPosition( DAMPER_ONOFF_POS_OPEN );
			/* Store return position */
			DamperONOFFVars.returnPos = DAMPER_ONOFF_POS_CLOSED;
			/* Send to the Manager refresh request */
			DamperONOFFVars.state.bit.refresh = 1;

		}
		else if ( DamperONOFFVars.currPos == DAMPER_ONOFF_POS_OPEN )
		{
			Damper_ONOFF_SetVarsForNewPosition( DAMPER_ONOFF_POS_CLOSED );
			/* store return position */
			DamperONOFFVars.returnPos = DAMPER_ONOFF_POS_OPEN;
			/* Send to the Manager refresh request */
			DamperONOFFVars.state.bit.refresh = 1;
		}
		else
		{
			/* First positioning not done! */
		}

	}
}

/**
This function returns the current position of the device.

\return 	damperONOFFPosition_t

\retval		DAMPER_ONOFF_POS_CLOSED   damper closed
\retval		DAMPER_ONOFF_POS_OPEN   damper open
\retval     DAMPER_ONOFF_POS_UNDEF 	  damper position not defined

\author 	Stefano Ughi
\date		17/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

damperONOFFPosition_t PM_Damper_ONOFF_GetPosition(void)
{
	return (DamperONOFFVars.currPos) ;
}


/**
This function returns the max. time to reach desired position.

\return 	uint8_t

\retval		TimeMaxToReachPos time in sec.

\author 	Stefano Ughi
\date		17/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t PM_Damper_ONOFF_GetTmaxReachPos( void )
{
	return(*(DamperONOFFVars.pDataDamperONOFF + POS_TIME_MAX_R_POS));
}


/**
This function returns TRUE if the damper is driven, otherwise FALSE.

\return 	bool_t

\retval		TRUE Damper is driven
\retval		FALSE Damper is not driven


\author 	Stefano Ughi
\date		17/10/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t PM_Damper_ONOFF_IsDriven( void )
{
	if ( 1 == DamperONOFFVars.state.bit.pylotStateDamper )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
This function detect Damper's position starting from reed value.
\n We assume that an high value on read implies Damper Open and a low value Damper Closed
\n You can change the logic of reading reed in order to invert these assumptions.

\return 	damperONOFFPosition_t

\retval		DAMPER_ONOFF_POS_UNDEF position not defined
\retval		DAMPER_ONOFF_POS_OPEN damper is open
\retval		DAMPER_ONOFF_POS_CLOSED damper is closed


\author 	Stefano Ughi
\date		18/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
\note		\par \b Nota 2:
			Only Digital and Feedback read type are supported for the reed.


*/

damperONOFFPosition_t PM_Damper_ONOFF_GetReedVal(void)
{
	uint8_t reedValue;
	uint8_t reed_readType;
	uint8_t readPos;
	damperONOFFPosition_t damperPosition;

	/* default value for Damper position */
	damperPosition = DAMPER_ONOFF_POS_UNDEF;
	/* how to read reed status linked to the damper */
	reed_readType = *(DamperONOFFVars.pDataDamperONOFF+POS_READ_TYPE);
	/* where i find the value of the read */
	readPos = *(DamperONOFFVars.pDataDamperONOFF+POS_INDEX);

	switch(reed_readType)
	{
	case FEEDBACK_I:
	case FEEDMULT_I:

		reedValue = LIB_FDB_GetFeedbackVal(readPos);

		if ( reedValue == FDB_INPUT_HI )
		{
			damperPosition = DAMPER_ONOFF_POS_OPEN;
		}
		else if ( reedValue == FDB_INPUT_LOW )
		{
			damperPosition = DAMPER_ONOFF_POS_CLOSED;
		}
		else
		{
			/* do nothing default value */
		}
		break;

	case DIGITAL_I:
	case DIGMULT_I:

		reedValue = LIB_DIO_GetDigInputVal(readPos);

		if ( reedValue )
		{
			damperPosition = DAMPER_ONOFF_POS_OPEN;
		}
		else
		{
			damperPosition = DAMPER_ONOFF_POS_CLOSED;
		}
		break;

	default:
		/* tipologia di lettura non prevista per il dispositivo */
		break;
	}

	return (damperPosition);
}

/* Local Function */
/**
This function check if first acquisition on reed linked to the damper is ready.


\return 	bool_t

\retval		TRUE first acquisition done
\retval		FALSE first acquisition not done or read type on reed not supported


\author 	Stefano Ughi
\date		18/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static bool_t damper_ONOFF_reedIsAcq(void)
{
	uint8_t reed_readType;
	uint8_t readPos;
	bool_t retValue;

	/* default value: reed acquisition not ready  */
	retValue = FALSE;
	/* how to read reed status linked to the damper */
	reed_readType = *(DamperONOFFVars.pDataDamperONOFF+POS_READ_TYPE);
	/* where i find the value of the read */
	readPos = *(DamperONOFFVars.pDataDamperONOFF+POS_INDEX);

	switch(reed_readType)
	{
	case FEEDBACK_I:
	case FEEDMULT_I:

		retValue = LIB_FDB_IsFeedbackReady(readPos);
		break;

	case DIGITAL_I:
	case DIGMULT_I:

		retValue = LIB_DIO_IsDigitalReady(readPos);

		break;
	default:
		/* this kind of read is not supported */
		break;
	}
	return(retValue);
}

/**
This function check if Damper reached requested position.


\return 	bool_t

\retval		TRUE position reached
\retval		FALSE position not reached


\author 	Stefano Ughi
\date		18/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static bool_t damper_ONOFF_PositionReached(void)
{
	if ( DamperONOFFVars.reqPos == PM_Damper_ONOFF_GetReedVal() )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
This function checks if Damper is positioning.

\return 	bool_t

\retval		TRUE  Damper is positioning
\retval		FALSE Damper position is stable

\author 	Stefano Ughi
\date		18/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static bool_t DamperONOFF_Positioning(void)
{
	if ( DamperONOFFVars.currPos == DamperONOFFVars.reqPos )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


/**
This function sets variables for a new positioning request.

\param  [in]    damperPosReq requested position

\author 	Stefano Ughi
\date		18/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static void Damper_ONOFF_SetVarsForNewPosition( uint8_t damperPosReq )
{
	DamperONOFFVars.currPos = DAMPER_ONOFF_POS_UNDEF;
	DamperONOFFVars.reqPos = damperPosReq;
	DamperONOFFVars.timeAfterPosReached = *(DamperONOFFVars.pDataDamperONOFF+POS_EXTRA_TIME);
}


/**
\n This Function pylot the device.

\param  [in]    pylotState allowable values ON or OFF


\author 	Stefano Ughi
\date		22/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static void Damper_ONOFF_SetPylot( uint8_t pylotState )
{
	LIB_DIO_SET_LOAD(pylotState, DamperONOFFVars.pin);
	DamperONOFFVars.state.bit.pylotStateDamper = (pylotState?1:0);
}

/* *********** */
/* End of file */
/* *********** */



