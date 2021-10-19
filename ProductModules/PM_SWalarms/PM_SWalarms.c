/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_SWalarms.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_SWalarms.c
\ingroup	PM_SwAlarms
\date		Feb 15, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Feb 15, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/
#define MODULE_BODY_SWALARMS
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "PM_SWalarms.h"
#include "LIB_Error.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Status's flags */
typedef struct SWalarmsFlagsTag
{
	/** If TRUE Alarm will expire before timer's overflow */
	bool_t ToutBeforeTimerOvfl:1;
	/** If TRUE Alarm is active */
	bool_t AlarmActive:1;
	/** If TRUE Alarm is expired */
	bool_t AlarmExpired:1;
	bool_t Free:5;
} SWalarmsFlags_t;
/** Software alarm config. */
typedef struct SWalarmsTag
{
	/** Time when you call PM_SWalarmsSet() [ticks]*/
	IO_HwTimerType Tstart;
	/** Timeout [ticks] */
	IO_HwTimerType Tout;
	/** Flags */
	SWalarmsFlags_t Flags;
} SWalarms_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/** Vars to manage software alarms */
static SWalarms_t SWalarms[NUM_SW_ALARMS];

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
Converts a time, expressed in nSec, in ticks

\param  [in]		timeNsec time in nSec

\return 	IO_HwTimerType

\retval		time in ticks

\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/
IO_HwTimerType PM_SWalarmsConvNsecInTicks(IO_TimingValueType timeNsec)
{
	IO_TickResType tickResNSec;
	tickResNSec = OCHWCNTR_GetTickResolution(SW_ALARMS_HW_TMR_ID);
	return (IO_HwTimerType)(timeNsec/tickResNSec);
}

/**
\n Sets a software alarm.

\param  [in]		swAlarmId alarm Id.
\param  [in]		alarm timeout in ticks


\author 	stefano.ughi
\date		Feb 16, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void PM_SWalarmsSet( SW_AlarmType swAlarmId, IO_HwTimerType alarm)
{
    Period_ValueType tmpModulus = OCHWCNTR_GetTimerModulus(SW_ALARMS_HW_TMR_ID);

	PLT_ERROR_FATAL_IF(( swAlarmId >= NUM_SW_ALARMS ) || (alarm > tmpModulus), PLT_ERROR_SWAL_INVALID);


	SWalarms[swAlarmId].Tstart = OCHWCNTR_GetTicksSync(SW_ALARMS_HW_TMR_ID);
	SWalarms[swAlarmId].Tout = (IO_HwTimerType) (((uint32_t) SWalarms[swAlarmId].Tstart + alarm) % tmpModulus);

	if ( SWalarms[swAlarmId].Tout > SWalarms[swAlarmId].Tstart )
	{
		/* Timeout will expire before timer's overflow */
		SWalarms[swAlarmId].Flags.ToutBeforeTimerOvfl = TRUE;
	}
	else
	{
		/* Timeout will expire after timer's overflow */
		SWalarms[swAlarmId].Flags.ToutBeforeTimerOvfl = FALSE;
	}
	SWalarms[swAlarmId].Flags.AlarmExpired = FALSE;
	SWalarms[swAlarmId].Flags.AlarmActive = TRUE;
}
/**
Checks if software alarm with swAlarmId is expired.

\param  [in]		swAlarmId alarm Id.


\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void PM_SWalarmsCheck(SW_AlarmType swAlarmId)
{
	IO_HwTimerType t;

	PLT_ERROR_FATAL_IF(( swAlarmId >= NUM_SW_ALARMS ), PLT_ERROR_SWAL_INVALID);

	if ( ( TRUE == SWalarms[swAlarmId].Flags.AlarmActive ) &&
	        ( FALSE == SWalarms[swAlarmId].Flags.AlarmExpired ) )
	{
		t = OCHWCNTR_GetTicksSync(SW_ALARMS_HW_TMR_ID);

		if ( TRUE == SWalarms[swAlarmId].Flags.ToutBeforeTimerOvfl )
		{
			if ( (t >= SWalarms[swAlarmId].Tout) || ( t < SWalarms[swAlarmId].Tstart ) )
			{
				SWalarms[swAlarmId].Flags.AlarmActive = FALSE;
				SWalarms[swAlarmId].Flags.AlarmExpired = TRUE;
			}
		}
		else
		{
			/* Timeout after timer's overflow,
			   condition "t < SWalarms[swAlarmId].Tstart" ensures that timer did a turn  */
			if ( ( t < SWalarms[swAlarmId].Tstart ) && ( t >= SWalarms[swAlarmId].Tout ) )
			{
				SWalarms[swAlarmId].Flags.AlarmActive = FALSE;
				SWalarms[swAlarmId].Flags.AlarmExpired = TRUE;
			}
		}
	}
}
/**
Returns the state of alarm with Id swAlarmId.

\param  [in]		swAlarmId alarm id

\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t PM_SWalarms_IsAlarmExpired(SW_AlarmType swAlarmId)
{
	PLT_ERROR_FATAL_IF(( swAlarmId >= NUM_SW_ALARMS ), PLT_ERROR_SWAL_INVALID);
	return(SWalarms[swAlarmId].Flags.AlarmExpired);
}

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

