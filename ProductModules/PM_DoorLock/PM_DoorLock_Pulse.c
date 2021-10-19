/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	DoorLock
\n	Pulse doorlock functions
\n
\file		PM_DoorLock_Pulse.c
\ingroup    DoorLockPulse
\date	    20/09/2011
\version	01.00
\author		Alberto Camerin,  Marco Sperandini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		20/09/2011
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/

#include "Config.h"
/*lint -e537*/
#include "SwTimersCounters.h"
#include "OutputManagerExp.h"
#include "DigitalOutputs.h"
/*lint +e537*/

#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"
#include "PM_DoorLock.h"
#include "PM_DoorLock_Pulse.h"
#include "FeedbacksExp.h"

#if defined( CONFIG_ZC_ALWAYS_ON )
static uint8_t Pulses_DoorLockedTab[2][4] =
{
/*zc == 0 */    {DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL, DOORLOCK_FAULT_FB, DOORLOCK_LOCKED_OK},
/*zc == 1 */    {DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL, DOORLOCK_FAULT_FB, DOORLOCK_LOCKED_OK}
};
static uint8_t Pulses_DoorUnlockedTab[2][4] =
{
    {DOORLOCK_FAULT_FB, DOORLOCK_UNLOCKED_OK, DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL},
    {DOORLOCK_FAULT_FB, DOORLOCK_UNLOCKED_OK, DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL}
};

#elif  defined(CONFIG_DOORLOCK_NO_FB)
/* no table */
#else
static uint8_t Pulses_DoorLockedTab[2][3] =
{
    {DOORLOCK_FAULT_FB, DOORLOCK_LOCKED_FAIL_ZC, DOORLOCK_LOCKED_GENERIC_FAIL},
    {DOORLOCK_FAULT_FB, DOORLOCK_FAULT_FB,       DOORLOCK_LOCKED_OK}
};
static uint8_t Pulses_DoorUnlockedTab[2][3] =
{
    {DOORLOCK_FAULT_FB, DOORLOCK_OPENDOOR, DOORLOCK_UNLOCKED_OK},
    {DOORLOCK_FAULT_FB, DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL}
};
#endif

const static DoorLockFuncTab pulseFuncTab={
		PM_DoorLock_Pulse_Reset_PilotType,
		PM_DoorLock_Pulse_SetPosition,
		PM_DoorLock_Pulse_InitStatus,
		PM_DoorLock_Pulse_StatusReached,
		PM_DoorLock_Pulse_EnterStableStatus,
		PM_DoorLock_Pulse_OngoingNotStable,
		PM_DoorLock_Pulse_OnStable,
		PM_DoorLock_Pulse_Consistency,
};

/** Reset status
 * \param [in] doorlock type, unused
 */
void PM_DoorLock_Pulse_Reset_PilotType( uint8_t type )
{
    PM_DoorLock_Control_Var.bp.bpi.hist_counter    = 0;
    PM_DoorLock_Control_Var.bp.bpi.impulseCnt      = 0;
    PM_DoorLock_Control_Var.bp.bpi.Max_Num_Impulsi = 0;
}


/** Configure impulse properties
 * \param [in] impulse_properties, from settings file
*/
void PM_DoorLock_Pulse_Configure_Impulse_Properties(uint16_t impulse_properties)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Config_Var);
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

    PM_DoorLock_Control_Var.bp.bpi.Delta_Timing    = (uint8_t)(impulse_properties      & 0x00FF);
    PM_DoorLock_Control_Var.bp.bpi.Max_Num_Impulsi = (uint8_t)((impulse_properties>>8) & 0x000F);

    PM_DoorLock_Control_Var.actual_position  	= BPGEN_STATUS_NOT_INIT;
    PM_DoorLock_Control_Var.desired_position 	= BPGEN_STATUS_NOT_INIT;

    SAFETY_DATA_UPDATE(PM_DoorLock_Config_Var);
    SAFETY_DATA_UPDATE(PM_DoorLock_Control_Var);
    SAFETY_REGION_END();
}

/** Specific on cheange stato requested for impulsive doorlock, reset counters
 * \param [in]	door_position
 * return always TRUE
*/
bool_t PM_DoorLock_Pulse_SetPosition(uint8_t door_position)
{
    PM_DoorLock_Control_Var.bp.bpi.hist_counter  = 0;
    PM_DoorLock_Control_Var.bp.bpi.impulseCnt    = 0;
    return TRUE;
}

/** Install specific functions for impulsive doorlock
 * \param [in]	pt	doorlock type
*/
void PM_DoorLock_Pulse_Install_drv( uint8_t pt )
{
    PM_DoorLock_Config_Var.pilot_type 		= pt;
    PM_DoorLock_Config_Var.func = (DoorLockFuncTab *) &pulseFuncTab;
}

/** Initializes status after reset
 *  use the doorLockStatusSaved backup var
 * \param [in]	feedback_val, unused
 * return always TRUE
*/
bool_t PM_DoorLock_Pulse_InitStatus(void) {

	PM_DoorLock_Control_Var.actual_position = doorLockStatusSaved;
	PM_DoorLock_Control_Var.desired_position = doorLockStatusSaved;

	PM_DoorLock_Control_Var.statusFiltered = PM_DoorLock_Control_Var.actual_position;

	PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_OK;

#if defined(CONFIG_DOORLOCK_NO_FB) || defined( CONFIG_ZC_ALWAYS_ON ) || defined(CONFIG_DL_PULSE_ZC_STABILITY_FILTER_ENABLED)
	if( getZCStability() < CONFIG_PM_DOORLOCK_PULSE_ZC_STABILITY_FILTER){
		return FALSE;
	}
#endif
	return TRUE;
}

/** Function executed entering on stable status
 *
*/
void PM_DoorLock_Pulse_EnterStableStatus( void )
{
	PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
}
/** Function executed in non stable status, RUUNING status
 *
*/
void PM_DoorLock_Pulse_OngoingNotStable( void )
{
    if( PM_DoorLock_Control_Var.stop_status != BPGEN_DEVICE_RUNNING )
        return;

	if (PM_DoorLock_Control_Var.bp.bpi.hist_counter < ( PM_DoorLock_Control_Var.bp.bpi.Delta_Timing * PM_DoorLock_Control_Var.bp.bpi.Max_Num_Impulsi))
	{
		if ((PM_DoorLock_Control_Var.bp.bpi.hist_counter % PM_DoorLock_Control_Var.bp.bpi.Delta_Timing) == 0)
		{
#if !defined( CONFIG_ZC_ALWAYS_ON )
			if(PM_DoorLock_Control_Var.zcFail){ /* nel caso zc fail da solo 2 impulsi allo scopo di sbloccare la porta */
				if(goodZCSignal()){/* lo Zc e' tornato */
					PM_DoorLock_Control_Var.zcFail = 0;
				}else{
					if(PM_DoorLock_Control_Var.bp.bpi.impulseCnt == 2){
						PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
						PM_DoorLock_Control_Var.bp.bpi.hist_counter = 0;
						PM_DoorLock_Control_Var.zcFail = 2;
						return;
					}
				}
			}
#endif
			if( PM_DoorLock_Control_Var.desired_position == BPGEN_OPEN )
			{
               	PM_DoorLock_PulseReq();
			}
			else if( PM_DoorLock_Control_Var.desired_position == BPGEN_CLOSE )
			{
                PM_DoorLock_PulseReq();
			}
			PM_DoorLock_Control_Var.bp.bpi.impulseCnt++;
		}
		PM_DoorLock_Control_Var.bp.bpi.hist_counter++;
	}
	else
	{
		PM_DoorLock_Control_Var.cmdPin = CMD_OFF;

		PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
		PM_DoorLock_Control_Var.bp.bpi.hist_counter = 0;
	}
}

/** Function execute in stable status
*	depends on CONFIG_DL_PULSE_RETRY_ON_STABLE
*/
void PM_DoorLock_Pulse_OnStable( void )
{
	if (PM_DoorLock_Control_Var.stop_status != BPGEN_DEVICE_STOPPED_FOR_ERROR)
	{
		if ((PM_DoorLock_Control_Var.bp.bpi.hist_counter % PM_DoorLock_Control_Var.bp.bpi.Delta_Timing) == 0)
		{
			PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_OK;
			PM_DoorLock_Control_Var.bp.bpi.hist_counter = 0;
			PM_DoorLock_Control_Var.errCnt = 0;
		}
		if (PM_DoorLock_Control_Var.stop_status == BPGEN_DEVICE_RUNNING)
		{
			PM_DoorLock_Control_Var.bp.bpi.hist_counter++;
		}
	}
	else{ /* se entra in stable con stato di errore e poi la consistency risulta coerente resetta lo stato di errore */
		if( BPGEN_CLOSE == PM_DoorLock_Control_Var.desired_position){
			if( PM_DoorLock_Control_Var.consistencyFiltered == DOORLOCK_LOCKED_OK ){
				PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_OK;
				PM_DoorLock_Control_Var.errCnt=0;
			}
		}else if( BPGEN_OPEN == PM_DoorLock_Control_Var.desired_position){
			if(    PM_DoorLock_Control_Var.consistencyFiltered == DOORLOCK_UNLOCKED_OK
				|| PM_DoorLock_Control_Var.consistencyFiltered == DOORLOCK_OPENDOOR
			){
				PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_OK;
				PM_DoorLock_Control_Var.errCnt=0;
			}
		}
	}

#if defined(CONFIG_DL_PULSE_RETRY_ON_STABLE)
	if(    PM_DoorLock_Control_Var.activated
		&& BPGEN_DEVICE_STOPPED_OK == PM_DoorLock_Control_Var.stop_status
		)
    {
		if(!PM_DoorLock_Control_Var.bp.bpi.retryDisabled)
		{
			uint8_t nowConsistency = PM_DoorLock_Control_Var.consistencyFiltered;
			uint8_t locked = PM_DoorLock_GetLockStatus();

#if defined(CONFIG_DOORLOCK_NO_FB) || defined( CONFIG_ZC_ALWAYS_ON ) || defined(CONFIG_DL_PULSE_ZC_STABILITY_FILTER_ENABLED)
			if( getZCStability() < CONFIG_PM_DOORLOCK_PULSE_ZC_STABILITY_FILTER ){
				nowConsistency = DOORLOCK_NOTAVAILABLE;
			}
#endif
			/* check internal consistency*/
			if( BPGEN_CLOSE == PM_DoorLock_Control_Var.desired_position )
			{
				if(    (locked != BPGEN_CLOSE)
					&& (nowConsistency != DOORLOCK_LOCKED_FAIL_ZC)
					&& (nowConsistency != DOORLOCK_LOCKED_OK     )
					&& (nowConsistency != DOORLOCK_NOTAVAILABLE)
					&& (nowConsistency != DOORLOCK_FAULT_FB)  /* non fa il riblocco se c'e' il FB errato */
				)
				{
					if( PM_DoorLock_Control_Var.errCnt < CONFIG_PM_DOORLOCK_MAX_ERRORS )
					{
						PM_DoorLock_Control_Var.errCnt++;
						PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
						PM_DoorLock_Control_Var.desired_position = BPGEN_FEED_UNDEFINED; /* per forzare il cambio di stato */

						PM_DoorLock_SetPosition_Internal(BPGEN_CLOSE);

						PM_DoorLock_Control_Var.last_desired_position = BPGEN_CLOSE;
					}
				}
			}
			else  if( BPGEN_OPEN == PM_DoorLock_Control_Var.desired_position )
			{
				if( (locked != BPGEN_OPEN)
						&& (nowConsistency != DOORLOCK_NOTAVAILABLE ) )
				{
					if(PM_DoorLock_Control_Var.errCnt < CONFIG_PM_DOORLOCK_MAX_ERRORS)
					{
						PM_DoorLock_Control_Var.errCnt++;
						PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
						PM_DoorLock_Control_Var.desired_position = BPGEN_FEED_UNDEFINED; /* per forzare il cambio di stato */

						PM_DoorLock_SetPosition_Internal(BPGEN_OPEN);

						PM_DoorLock_Control_Var.last_desired_position = BPGEN_OPEN;
					}
				}
			}
		}/*end retryDisabled*/
	}
#endif
	PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
}
/** This function calculates the consistency for pulse doorlock
 * depends on CONFIG_ZC_ALWAYS_ON, CONFIG_DOORLOCK_NO_FB
 * it is different for different HW configuration
 * CONFIG_ZC_ALWAYS_ON used on Hoven
 * CONFIG_DOORLOCK_NO_FB used on WM with Collector Motor
 * return the calculated consistency
*/
#if defined( CONFIG_ZC_ALWAYS_ON )
uint8_t PM_DoorLock_Pulse_Consistency( void )
{
    uint8_t retVal=DOORLOCK_NOTAVAILABLE;
    bool_t zc = goodZCSignal();
    uint8_t fb  = LIB_FDB_GetFeedbackVal(PM_DoorLock_Config_Var.feedback_index[IDX_FB]);

    switch(PM_DoorLock_Control_Var.desired_position )
    {
    case BPGEN_OPEN: /*DOOR_UNLOCKED*/
    		retVal = Pulses_DoorUnlockedTab[zc][fb];
        break;
    case BPGEN_CLOSE: /*DOOR_LOCKED*/
        retVal= Pulses_DoorLockedTab[zc][fb];
        break;

    default: /* BPGEN_FEED_UNDEFINED*/
        break;
    }

    return retVal;
}
#elif defined(CONFIG_DOORLOCK_NO_FB)
uint8_t PM_DoorLock_Pulse_Consistency(void )
{
    uint8_t retVal=DOORLOCK_NOTAVAILABLE;
    bool_t zc = goodZCSignal();

    switch(PM_DoorLock_Control_Var.desired_position )
    {
    case BPGEN_OPEN: /*DOOR_UNLOCKED*/
    	if(zc){
    		retVal = DOORLOCK_GENERIC_FAIL;
    	}else{
    		retVal = DOORLOCK_UNLOCKED_OK;

			if (PM_DoorLock_Config_Var.isSensing)
			{
				if (PM_DoorLock_Config_Var.feedback_index[IDX_FB]!= BPGEN_FEEDBACK_NOT_INIT)
				{ /*DOOR SENSING*/
					uint8_t fb = PM_DoorLock_GetFeedbackVal((uint8_t) IDX_FB);

					if (fb == FDB_INPUT_HI) {
						retVal = DOORLOCK_OPENDOOR;
					}
				}
			}
    	}
        break;
    case BPGEN_CLOSE: /*DOOR_LOCKED*/
		if (zc) {
			retVal = DOORLOCK_LOCKED_OK;
		} else {
			if( getZCStability() < CONFIG_MIN_ZC_STABILITY ){
				retVal = DOORLOCK_LOCKED_FAIL_ZC;
			}else{
				retVal = DOORLOCK_LOCKED_GENERIC_FAIL;
			}
		}
        break;

    default: /* BPGEN_FEED_UNDEFINED*/
        break;
    }

    return retVal;
}
#else
uint8_t PM_DoorLock_Pulse_Consistency(void)
{
    uint8_t retVal=DOORLOCK_NOTAVAILABLE;
    uint8_t fb;
    bool_t zc = goodZCSignal();

    if (PM_DoorLock_Config_Var.feedback_index[IDX_FB]!= BPGEN_FEEDBACK_NOT_INIT)
    {
    	fb = PM_DoorLock_GetFeedbackVal((uint8_t) IDX_FB);
    }else{
    	fb=FDB_INPUT_FREQ_OPP_PHASE; /* if fb is not configtated */
    }

    if( fb > FDB_INPUT_FREQ_OPP_PHASE){
        fb = FDB_INPUT_FREQ_OPP_PHASE;
    }

    switch(PM_DoorLock_Control_Var.desired_position )
    {

    case BPGEN_OPEN: /*DOOR_UNLOCKED*/
    	if( !PM_DoorLock_Config_Var.isSensing && (zc == 0) && (fb == FDB_INPUT_HI)){
    		retVal = DOORLOCK_FAULT_FB;
    	}else{
    		retVal = Pulses_DoorUnlockedTab[zc][fb];
    	}
        break;
    case BPGEN_CLOSE: /*DOOR_LOCKED*/
        retVal= Pulses_DoorLockedTab[zc][fb];
        break;

    default: /* BPGEN_FEED_UNDEFINED*/
        break;
    }

    return retVal;
}
#endif


uint8_t PM_DoorLock_Pulse_StatusReached(void)
{
	uint8_t locked;
	    uint8_t retVal = FALSE;

	    locked = PM_DoorLock_GetLockStatus();

	    if( PM_DoorLock_Control_Var.desired_position == BPGEN_CLOSE)
	    {
	        /*OPEN->CLOSE*/
	        if(BPGEN_FEED_CLOSE == locked )
	        {
	            retVal = TRUE;
	        }
	    }
	    else
	    {
	        /*CLOSE->OPEN*/
	        if(BPGEN_FEED_OPEN == locked)
	        {
	            retVal = TRUE;
	        }
	    }
	    return retVal;
}

void PM_DoorLock_Pulse_DisableRetry(bool_t disable)
{
	EnterCriticalSection();
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

	PM_DoorLock_Control_Var.bp.bpi.retryDisabled = disable;

	SAFETY_DATA_UPDATE(PM_DoorLock_Control_Var);
	SAFETY_REGION_END();
	ExitCriticalSection();

}

uint8_t PM_DoorLock_Pulse_GetCmdPulse(void)
{
	return PM_DoorLock_Control_Var.bp.bpi.impulseCnt;
}

#endif /* CONFIG_PM_DOORLOCK_PULSE_ENABLE */
