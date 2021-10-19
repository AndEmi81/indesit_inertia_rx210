/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\par	DoorLockWax
\n	This file contains function for doorlock device
\n
\file		PM_DoorLock_Wax.c
\ingroup    DoorLockWax
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

#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE

#include "SwTimersCounters.h"
#include "OutputManagerExp.h"
#include "DigitalOutputs.h"
#include "OCDIO_drv.h"

#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"

#include "PM_DoorLock.h"
#include "PM_DoorLock_Wax.h"
#include "FeedbacksExp.h"
#define  FAIL_FILTER	10
#define MIN_ZC_STABILITY						(250) 		/**< filter for zc stability x 10ms*/
/* #define CONFIG_PM_DOORLOCK_WAX_MAX_BDR_TIME		(500)*/		/* massima lunghezza buco di rete */
const static DoorLockFuncTab waxFuncTab={
		PM_DoorLock_Wax_Reset_PilotType,
		PM_DoorLock_Wax_SetPosition,
		PM_DoorLock_Wax_InitStatus,
		PM_DoorLock_Wax_StatusReached,
		PM_DoorLock_Wax_EnterStableStatus,
		PM_DoorLock_Wax_OnGoingNotStable,
		PM_DoorLock_Wax_OnStable,
		PM_DoorLock_Wax_Consistency,
};

#if defined( CONFIG_ZC_ALWAYS_ON )
static uint8_t Wax_DoorLockedTab[4]   = {DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL, DOORLOCK_FAULT_FB, DOORLOCK_LOCKED_OK};
static uint8_t Wax_DoorUnlockedTab[4] = {DOORLOCK_FAULT_FB, DOORLOCK_UNLOCKED_OK,  DOORLOCK_FAULT_FB, DOORLOCK_GENERIC_FAIL};
#endif

/** Function executed on stable status of wax doorlock
 *  depends on CONFIG_DL_WAX_RETRY_ON_STABLE
 *  manage the load partializazion as defined in the settings
 *
 */
void PM_DoorLock_Wax_OnStable( void )
{
    /* Reset fault counter */
    PM_DoorLock_Control_Var.bp.bpc.fault_wait_time  = 0;

    if (PM_DoorLock_Control_Var.desired_position == BPGEN_OPEN)
    {
        /* The device is open --> don't pilot the device */
        LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Config_Var.control_pin);

        if (PM_DoorLock_Control_Var.bp.bpc.lock_pin != 0xFF)
        {
            if (PM_DoorLock_Control_Var.pilot_pin_flag)
                LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Control_Var.bp.bpc.lock_pin);
        }
    }
    else
    {
        /* The device is close */
        if (PM_DoorLock_Control_Var.bp.bpc.lock_pin != 0xFF)
        {
        	if( PM_DoorLock_Control_Var.bp.bpc.periodPartialized != 0)
        	{
				if( PM_DoorLock_Control_Var.bp.bpc.cntCycle <= PM_DoorLock_Control_Var.bp.bpc.tOn )
				{
					if(PM_DoorLock_Control_Var.on_counter == 0)
					{
						PM_DoorLock_Control_Var.cmdPin = CMD_ON_CERA;
						PM_DoorLock_Control_Var.on_counter = PM_DoorLock_Control_Var.bp.bpc.tOn;
						PM_DoorLock_ResetFeedback();
					}
				}
				else if( PM_DoorLock_Control_Var.bp.bpc.cntCycle < PM_DoorLock_Control_Var.bp.bpc.periodPartialized )
				{
					PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
					if(PM_DoorLock_Control_Var.bp.bpc.cntCycle == (PM_DoorLock_Control_Var.bp.bpc.tOn+1)){
						PM_DoorLock_ResetFeedback();
					}
				}
				else
					PM_DoorLock_Control_Var.bp.bpc.cntCycle = 0;

				PM_DoorLock_Control_Var.bp.bpc.cntCycle++;
        	}else{
				LIB_DIO_SET_LOAD(ON, PM_DoorLock_Config_Var.control_pin);
        	}

            if (PM_DoorLock_Control_Var.pilot_pin_flag)
                LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Control_Var.bp.bpc.lock_pin);
        }
        else
        {
        	if(PM_DoorLock_Control_Var.bp.bpc.periodPartialized != 0 )
        	{
				if( PM_DoorLock_Control_Var.bp.bpc.cntCycle <= PM_DoorLock_Control_Var.bp.bpc.tOn )
				{
					if(PM_DoorLock_Control_Var.on_counter == 0)
					{
						PM_DoorLock_Control_Var.cmdPin = CMD_ON_CERA;

						if(!goodZCSignal())
						{
							LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);

							PM_DoorLock_Control_Var.on_counter = PM_DoorLock_Control_Var.bp.bpc.tOn;
							PM_DoorLock_Control_Var.bp.bpc.onFail=0;
							PM_DoorLock_Control_Var.bp.bpc.offFail=0;
							PM_DoorLock_Control_Var.bp.bpc.zcFail=0;
							PM_DoorLock_Control_Var.bp.bpc.cntCycle=0;
						}
						PM_DoorLock_ResetFeedback();

						PM_DoorLock_Control_Var.on_counter = PM_DoorLock_Control_Var.bp.bpc.tOn;
						PM_DoorLock_Control_Var.bp.bpc.onFail=0;
						PM_DoorLock_Control_Var.bp.bpc.offFail=0;
						PM_DoorLock_Control_Var.bp.bpc.zcFail=0;
					}
				}
				else if( PM_DoorLock_Control_Var.bp.bpc.cntCycle < PM_DoorLock_Control_Var.bp.bpc.periodPartialized )
				{
					PM_DoorLock_Control_Var.cmdPin = CMD_OFF;

					LIB_DIO_SET_LOAD( OFF, PM_DoorLock_Config_Var.control_pin);

					if(PM_DoorLock_Control_Var.bp.bpc.cntCycle == (PM_DoorLock_Control_Var.bp.bpc.tOn+1)){
						PM_DoorLock_ResetFeedback();
					}
				}
				else{
					PM_DoorLock_Control_Var.bp.bpc.cntCycle = 0;
#if !defined( CONFIG_ZC_ALWAYS_ON )	&& 	!defined(CONFIG_DOORLOCK_NO_FB)

					/* decide lo stato dell consistency guardando il risultato del periodo precedente */
					if(  PM_DoorLock_Control_Var.bp.bpc.periodPartialized != 0
							&& PM_DoorLock_Control_Var.desired_position == BPGEN_CLOSE)
					{
						if(!PM_DoorLock_Control_Var.bp.bpc.zcFail){ /* ZC OK */
							if( PM_DoorLock_Control_Var.bp.bpc.onFail  < FAIL_FILTER
								&& PM_DoorLock_Control_Var.bp.bpc.offFail < FAIL_FILTER )
							{
								PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_LOCKED_OK;
							}else{
								PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_FAULT_FB;
							}

						}else{
							if( PM_DoorLock_Control_Var.bp.bpc.onFail     < FAIL_FILTER
								&& PM_DoorLock_Control_Var.bp.bpc.offFail < FAIL_FILTER )
							{
								if(!goodZCSignal() && getZCStability() > MIN_ZC_STABILITY )
								{
									PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_LOCKED_GENERIC_FAIL;
								}else{
									PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_LOCKED_OK;
								}
							} else
							if( PM_DoorLock_Control_Var.bp.bpc.onFail  < FAIL_FILTER
									&& PM_DoorLock_Control_Var.bp.bpc.offFail >= FAIL_FILTER)
							{ /* buco di rete */
								PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_LOCKED_FAIL_ZC;
							}else{
								PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_LOCKED_GENERIC_FAIL;
							}
						}
					}
#endif
				}

				PM_DoorLock_Control_Var.bp.bpc.cntCycle++;
        	}else{
        		LIB_DIO_SET_LOAD(ON, PM_DoorLock_Config_Var.control_pin);
        	}
        }
    }


    if (PM_DoorLock_Control_Var.stop_status == BPGEN_DEVICE_RUNNING)
    {
        PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_OK;
    }

}
/** Funcion executed on not stable status, RUNNING, of WAX doorlock
 *
 */
void PM_DoorLock_Wax_OnGoingNotStable( void )
{
    if (PM_DoorLock_Control_Var.desired_position == BPGEN_CLOSE)
    {
        if (PM_DoorLock_Control_Var.bp.bpc.lock_pin != 0xFF)
        {
            if (PM_DoorLock_Control_Var.pilot_pin_flag)
                LIB_DIO_SET_LOAD(ON, PM_DoorLock_Control_Var.bp.bpc.lock_pin);
        }
        else
        {
                LIB_DIO_SET_LOAD(ON, PM_DoorLock_Config_Var.control_pin);
        }

        if (PM_DoorLock_Control_Var.bp.bpc.fault_wait_time >= PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_close)
        {
            PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
            PM_DoorLock_Control_Var.errCnt=1;
        }
    }
    else /* OPEN */
    {
    	LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Config_Var.control_pin);

        if (PM_DoorLock_Control_Var.bp.bpc.fault_wait_time >= PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_open)
        {
            PM_DoorLock_Control_Var.stop_status = BPGEN_DEVICE_STOPPED_FOR_ERROR;
            PM_DoorLock_Control_Var.errCnt=1;
        }

        if(!goodZCSignal() && (PM_DoorLock_Control_Var.bp.bpc.zc_lost_time < 0xFFFF)){
        	PM_DoorLock_Control_Var.bp.bpc.zc_lost_time++;
        }else{
        	PM_DoorLock_Control_Var.bp.bpc.zc_lost_time=0;
        }
    }

    /* Update the fault counter */
    PM_DoorLock_Control_Var.bp.bpc.fault_wait_time++;
    PM_DoorLock_Control_Var.bp.bpc.lastConsistency=DOORLOCK_NOTAVAILABLE;

}

/** Install specific functions for WAX doorlock
 * \param [in]	pt	doorlock type
 */
void PM_DoorLock_Wax_Install_drv( uint8_t pt )
{
	PM_DoorLock_Config_Var.pilot_type 		= pt;
	PM_DoorLock_Config_Var.func = (DoorLockFuncTab *) &waxFuncTab;
}

/** Reset counters of WAX doorlock
 *
*/
void PM_DoorLock_Wax_Reset_PilotType( uint8_t type )
{
    PM_DoorLock_Control_Var.bp.bpc.fault_wait_time      		  = 0;
    PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_open   = 0;
    PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_close	= 0;
    PM_DoorLock_Control_Var.bp.bpc.lock_pin 			            = 0xFF;
	PM_DoorLock_Control_Var.bp.bpc.periodPartialized = 0;
	PM_DoorLock_Control_Var.bp.bpc.tOn = 0;
	PM_DoorLock_Control_Var.bp.bpc.zc_lost_time=0;

}

/**	Configure the device maximum time to change status
 * \param [in] wt_fault_open , max counter for unlock, x 20ms
 * \param [in] wt_fault_close , max counter for lock, x 20ms
 *
*/
void  PM_DoorLock_Wax_Configure_FaultTiming(uint16_t wt_fault_open, uint16_t wt_fault_close)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

	PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_open 	= wt_fault_open;
    PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_close 	= wt_fault_close;

    SAFETY_DATA_UPDATE(PM_DoorLock_Control_Var);
    SAFETY_REGION_END();
}

/** Configure the aux pin for Wax2Pin
 * \param auxPin  pin to lock
*/
void  PM_DoorLock_Wax_Configure_AuxPin(uint8_t auxPin)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

    PM_DoorLock_Control_Var.bp.bpc.lock_pin 		= auxPin;

    SAFETY_DATA_UPDATE(PM_DoorLock_Control_Var);
    SAFETY_REGION_END();
}

/** Execute on change status requested OPEN/CLOSE
 * used in setPositionInternal
 * \param [in] door_position OPEN/CLOSE
 * return always TRUE
 */
bool_t PM_DoorLock_Wax_SetPosition(uint8_t door_position)
{
    PM_DoorLock_Control_Var.bp.bpc.fault_wait_time  	= 0;
    PM_DoorLock_Control_Var.bp.bpc.cntCycle 			= 0;
    PM_DoorLock_Control_Var.errCnt						= 0;
    return TRUE;
}

/** Wax doorlock status initialization
 * define the initial status, not use the backup status
 * \param [in] feedback_val
 * return always TRUE
*/
bool_t PM_DoorLock_Wax_InitStatus( void  )
{
	uint8_t feedback_val = PM_DoorLock_GetLockStatus();
    PM_DoorLock_Control_Var.actual_position  = feedback_val;
    PM_DoorLock_Control_Var.desired_position = feedback_val;
    PM_DoorLock_Control_Var.stop_status      = BPGEN_DEVICE_STOPPED_OK;
    return TRUE;
}

/** Function executed on enter on stable status
 *
 */
void PM_DoorLock_Wax_EnterStableStatus( void )
{
    PM_DoorLock_Control_Var.stop_status            = BPGEN_DEVICE_STOPPED_OK;
    PM_DoorLock_Control_Var.bp.bpc.fault_wait_time = 0;


    if (PM_DoorLock_Control_Var.bp.bpc.lock_pin != 0xFF)
    {
        if (PM_DoorLock_Control_Var.pilot_pin_flag)
            LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Control_Var.bp.bpc.lock_pin);
    }

    if (PM_DoorLock_Control_Var.actual_position == BPGEN_OPEN)
    {
        /*The device is open --> don't pilot the device*/
        LIB_DIO_SET_LOAD(OFF, PM_DoorLock_Config_Var.control_pin);

        PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
    }
    else
    {
        /*The device is close --> pilot the device*/
        LIB_DIO_SET_LOAD(ON, PM_DoorLock_Config_Var.control_pin);
    }

    PM_DoorLock_Control_Var.bp.bpc.cntCycle = 0;
}

/** Thi function calulate the consistency for WAX doorlock
 * depends on  CONFIG_ZC_ALWAYS_ON,CONFIG_DOORLOCK_NO_FB
 * it is different for different HW configuration
 * CONFIG_ZC_ALWAYS_ON used on Hoven
 * CONFIG_DOORLOCK_NO_FB used on WM with Collector Motor
 * return the calculated consistency
 */
#if defined( CONFIG_ZC_ALWAYS_ON ) /*FORNO */
uint8_t PM_DoorLock_Wax_Consistency(void)
{
    uint8_t retVal=DOORLOCK_NOTAVAILABLE;

    uint8_t fb = LIB_FDB_GetFeedbackVal(PM_DoorLock_Config_Var.feedback_index[IDX_FB]);

    if( BPGEN_DEVICE_RUNNING == PM_DoorLock_Control_Var.stop_status ){
        retVal = DOORLOCK_NOTAVAILABLE;
    }else{
    	switch (PM_DoorLock_Control_Var.desired_position)
		{
		case BPGEN_OPEN: /* DOOR_UNLOCKED */
			retVal = Wax_DoorUnlockedTab[fb];
		break;
		case BPGEN_CLOSE: /* DOOR_LOCKED */
			retVal = Wax_DoorLockedTab[fb];
			break;
		default: /* BPGEN_FEED_UNDEFINED */
			break;
		}
    }
    return retVal;
}

#elif defined(CONFIG_DOORLOCK_NO_FB) /* motore a collettore */
uint8_t PM_DoorLock_Wax_Consistency(void)
{
    uint8_t retVal=BPGEN_FEED_UNDEFINED;
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
				if (PM_DoorLock_Config_Var.feedback_index[IDX_FB] != BPGEN_FEEDBACK_NOT_INIT)
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
			retVal = DOORLOCK_LOCKED_GENERIC_FAIL;
		}
        break;

    default: /* BPGEN_FEED_UNDEFINED*/
        break;
    }

    return retVal;
}
#else
uint8_t PM_DoorLock_Wax_Consistency(void)
{
    uint8_t retVal=DOORLOCK_NOTAVAILABLE;
    bool_t zc = goodZCSignal();
    uint8_t zc_stability = getZCStability();
    uint8_t fb=0;

    if( BPGEN_DEVICE_RUNNING == PM_DoorLock_Control_Var.stop_status ){
        retVal = DOORLOCK_NOTAVAILABLE;
    }else{
    	if (PM_DoorLock_Config_Var.feedback_index[IDX_FB]!= BPGEN_FEEDBACK_NOT_INIT)
		{
			fb = PM_DoorLock_GetFeedbackVal((uint8_t) IDX_FB);
		}

		if( fb > FDB_INPUT_FREQ_OPP_PHASE){
			fb = FDB_INPUT_FREQ_OPP_PHASE;
		}
		switch(PM_DoorLock_Control_Var.desired_position )
		{
			case BPGEN_OPEN: /* DOOR_UNLOCKED *******************************************************/

			if( zc == 0 )
			{
				if( fb == FDB_INPUT_FREQ_OPP_PHASE )
				{
					retVal = DOORLOCK_UNLOCKED_OK;
				}
				else if( fb == FDB_INPUT_HI)
				{
					if( PM_DoorLock_Config_Var.isSensing ){
						retVal = DOORLOCK_OPENDOOR;
					}else{
						retVal = DOORLOCK_LOCKED_GENERIC_FAIL;
					}
				}
				else
				{
					retVal = DOORLOCK_FAULT_FB;
				}
			}
			else
			{
				retVal = DOORLOCK_LOCKED_GENERIC_FAIL;
			}
			PM_DoorLock_Control_Var.bp.bpc.lastConsistency = retVal;
			break;
			case BPGEN_CLOSE: /* DOOR_LOCKED ********************************************************/

			if (fb == FDB_INPUT_LOW) { /* da subito errore */
				PM_DoorLock_Control_Var.bp.bpc.lastConsistency = DOORLOCK_LOCKED_GENERIC_FAIL;
			}else{

				if( PM_DoorLock_Control_Var.bp.bpc.periodPartialized != 0)
				{
					uint32_t control_pin_status =  OCDIO_GetPinValueSync(PM_DoorLock_Config_Var.control_pin);
					if(PM_DoorLock_Control_Var.bp.bpc.cntCycle < (PM_DoorLock_Control_Var.bp.bpc.tOn))
					{ /* triac acceso */
						if (PM_DoorLock_Control_Var.bp.bpc.cntCycle < PM_DoorLock_Control_Var.bp.bpc.filterTime) {
							retVal = DOORLOCK_NOTAVAILABLE;
						}else{
							if (!zc) {
								PM_DoorLock_Control_Var.bp.bpc.zcFail = 1;
							}

							if (control_pin_status) {
								if (fb != FDB_INPUT_HI) {
									PM_DoorLock_Control_Var.bp.bpc.onFail++;
								}
							} else {
								if (fb != FDB_INPUT_FREQ_OPP_PHASE) {
									PM_DoorLock_Control_Var.bp.bpc.offFail++;
								}
							}

						}
					}
					else /* triac spento */
					{
						if (PM_DoorLock_Control_Var.bp.bpc.cntCycle
								< (PM_DoorLock_Control_Var.bp.bpc.tOn + PM_DoorLock_Control_Var.bp.bpc.filterTime))
						{
							retVal = DOORLOCK_NOTAVAILABLE;
						}else{
							if(!zc){
								PM_DoorLock_Control_Var.bp.bpc.zcFail=1;
							}

							if (control_pin_status) {
								if (fb != FDB_INPUT_HI) {
									PM_DoorLock_Control_Var.bp.bpc.onFail++;
								}
							} else {
								if (fb != FDB_INPUT_FREQ_OPP_PHASE) {
									PM_DoorLock_Control_Var.bp.bpc.offFail++;
								}
							}

						}
					}
				}else{
					if( zc == 1 )
					{
						if( fb == FDB_INPUT_HI )
						{
							retVal = DOORLOCK_LOCKED_OK;
						}
						else
						{
							retVal = DOORLOCK_FAULT_FB;
						}
					}
					else
					{
						if (fb == FDB_INPUT_HI) {
							retVal = DOORLOCK_LOCKED_FAIL_ZC; /* buco di rete */
						}else{
							retVal = DOORLOCK_LOCKED_GENERIC_FAIL;
						}
					}
					PM_DoorLock_Control_Var.bp.bpc.lastConsistency = retVal;
				}
			}
			break;

	default: /* BPGEN_FEED_UNDEFINED */

			break;
		}
    }

	return PM_DoorLock_Control_Var.bp.bpc.lastConsistency;
}
#endif

/** Used to configure the partialization of the load
 *  as defined in the settings file
 *  depends on CONFIG_PM_DOORLOCK_WAX_PARTIALIZE_LOAD,
 *  that specify to ignore the partialization settings in the settings file
 *  and use CONFIG_PM_DOORLOCK_WAX_PARTIALIZELOAD_LEN, CONFIG_PM_DOORLOCK_WAX_PARTIALIZE_LOAD_N_ON
 *
 */
void PM_DoorLock_Wax_Configure_Parzilize( uint16_t period, uint16_t tOn, uint16_t filterTime)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_DoorLock_Control_Var );

#if defined(CONFIG_PM_DOORLOCK_WAX_PARTIALIZE_LOAD)
	PM_DoorLock_Control_Var.bp.bpc.periodPartialized 	= CONFIG_PM_DOORLOCK_WAX_PARTIALIZELOAD_LEN;
	PM_DoorLock_Control_Var.bp.bpc.tOn 					= CONFIG_PM_DOORLOCK_WAX_PARTIALIZE_LOAD_N_ON;
	PM_DoorLock_Control_Var.bp.bpc.filterTime 			= 20;
#else
	PM_DoorLock_Control_Var.bp.bpc.periodPartialized 	= period;
	PM_DoorLock_Control_Var.bp.bpc.tOn 					= tOn;
	PM_DoorLock_Control_Var.bp.bpc.filterTime 			= filterTime;
	/*NOTA: filter time deve essere < tOn e di TOff */
#endif
	SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
	SAFETY_REGION_END();
}


uint8_t PM_DoorLock_Wax_StatusReached(void)
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
#if !defined( CONFIG_ZC_ALWAYS_ON ) && !defined(CONFIG_DOORLOCK_NO_FB) /* WM/WD */
	    	if(!goodZCSignal() && (PM_DoorLock_Control_Var.bp.bpc.zc_lost_time > CONFIG_PM_DOORLOCK_WAX_MAX_BDR_TIME))
	    	{
	    		retVal = TRUE;
	    	}
#else
	        if(BPGEN_FEED_OPEN == locked)
	        {
	            retVal = TRUE;
	        }
#endif
	    }
	    return retVal;
}

/* TODO:  */
uint16_t PM_DoorLock_Wax_GetPrefaultTimeout(void){
	return PM_DoorLock_Control_Var.bp.bpc.max_fault_wait_time_open;
}

#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE */
