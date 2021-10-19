/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		OCLVD_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCLVD_drv.c
\ingroup	OCLVD
\date		May 6, 2010
\version	00.00.01
\author		Jaroslav Musil
\n
\n
\Par\b	History:
\n
\n 		00.00.01
\n		06/05/2010
\n		Jaroslav Musil
\n		First Insertion
\n
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "OCLVD_drv.h"
#include "LIB_TaskMonitoring.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Commands and status */
typedef struct LvdStateTag
{
	/** if '1' first setup done */
	bool_t setupDone:1;
	/** free for future use */
	bool_t free:7;
} LvdState_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
static Comm_Notification callback;
static LvdState_t LvdState;
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
Sets the low voltage detection level to generate LV reset. Turns on the LV detection
mode and turns on the reset on LV feature.

\param  [in]	tripPointVR - low voltage trip in volt * 10

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE - trip > than conventional Vdd

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

\note		\par \b Note 1:
            Feb 25,2011
		    Rewritten because of several mistakes \n
            on AG128 SPMSC1 and SPMSC2 are one time writable on some bits (Stefano Ughi).\n
            added flag LvdState.setupDone

*/
IO_ErrorType OCLVDR_SetSync(LVDR_TripPointType tripPointVR)
{
	if ( tripPointVR > UC_VDD_X_10 )
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	/* Sets the trip for detection to reset */
	if ( LvdState.setupDone )
	{
		if ( LVD_AllowUpDate() )
		{
			LVDR_SetUp(tripPointVR);
			return (IO_ErrorType)IO_E_OK;
		}
		else
		{
			return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
		}
	}
	else
	{
		LVDR_SetUp(tripPointVR);
		LvdState.setupDone = 1;
		return (IO_ErrorType)IO_E_OK;
	}
}
/**
Disable reset on Low Voltage Detection.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Stefano Ughi
\date		Dec 7, 2010
\version	1.0.0

\note		\par \b Note 1:
            Feb 25,2011
            added flag LvdState.setupDone

*/
IO_ErrorType OCLVDNR_SetSync(void)
{
	if ( LvdState.setupDone )
	{
		if ( LVD_AllowUpDate() )
		{
			LVDR_Disable();
			return (IO_ErrorType)IO_E_OK;
		}
		else
		{
			return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
		}
	}
	else
	{
		LVDR_Disable();
		LvdState.setupDone = 1;
		return (IO_ErrorType)IO_E_OK;
	}
}

/**
Sets the low voltage warning level to generate LV warning.
If the callback parameter is set, the interrupt is enabled.

\param  [in]		tripPointVW - low voltage warning in volt * 10
\param  [in]        lowVoltageWarning - pointer to the callback function

\return 	IO_ErrorType

\retval		IO_E_OK
\retval     IO_E_INVALID_VALUE - trip > than conventional Vdd

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

*/
IO_ErrorType OCLVDW_SetSync(LVDW_TripPointType tripPointVW, Comm_Notification lowVoltageWarning)
{
	if (tripPointVW > UC_VDD_X_10)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}

	/* Sets the low voltage warning */
	LVDW_SetTrip(tripPointVW);

	/* if callback function is defined, the interrupt is activated */
	if (lowVoltageWarning != NULL)
	{
		LVDWarningIntOn();
		callback = lowVoltageWarning;
	}
	else
	{
		LVDWarningIntOff();
	}

	return (IO_ErrorType)IO_E_OK;
}
/**
This function checks if low warning voltage is present.

\return 	bool_t

\retval		TRUE low voltage warning
\retval		FALSE no low voltage warning

\author 	Stefano Ughi
\date		Dec 6, 2010
\version	1.0.0

*/
bool_t OCLVDW_IsWarningOn(void)
{
	if(LVDGetWarningFlag())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/**
The interrupt function that is to be called from the interrupt vector table when
the LV warning is triggered. The function calls the predefined callback and clears
the warning flag

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

\note

*/

void OCLVDW_ISR(void)
{
	IRQ_MONITOR_ENTER();

	(void)(callback)((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)0, (IO_ErrorType)IO_E_OK);

	/* Acknoledges the flag */
	LVDWarningAcknoledge();

	IRQ_MONITOR_EXIT();
}

/**
Turns on or off the LVD in stop mode.

\param  [in]	enable
\li             !0 turn on LVD in Stop Mode
\li             0  turn off LVD in Stop Mode

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

\note

*/
IO_ErrorType OCLVD_SetStop(bool_t enable)
{
	if (enable)
	{
		LVDSetStopOn();
	}
	else
	{
		LVDSetStopOff();
	}

	return (IO_ErrorType)IO_E_OK;
}

/**
Reads the preset low voltage detection level to generate LV reset.

\return 	LVDR_TripPointType

\retval		low voltage detection trip in volts * 10

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

\note

*/
LVDR_TripPointType OCLVDR_GetSync(void)
{
	return LVDR_GetTrip();
}

/**
Reads the preset low voltage warning level to generate LV warning.

\return 	LVDW_TripPointType

\retval		low voltage warning trip in volts * 10

\author 	Jaroslav Musil
\date		May 6, 2010
\version	1.0.0

\note

*/
LVDW_TripPointType OCLVDW_GetSync(void)
{
	return LVDW_GetTrip();
}


/* Local Function */

/* *********** */
/* End of file */
/* *********** */

