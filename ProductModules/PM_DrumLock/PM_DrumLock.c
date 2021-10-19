/*
Copyright (c) 2004-2013, Indesit Company.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_DrumLock main source file
\n			Contains the skeleton ad main functions of the module PM_DrumLock
\n
\file		PM_DrumLock.c
\ingroup	PM_DrumLock
\date	    05/03/2013
\version	01.00.00
\author		michele.tanzi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		05/03/2013
\n		michele.tanzi
\n		changes
\n 			Creation
\n
*/

/*!	Include
** ************************************************************************************************
*/
#include "Config.h"

#ifdef CONFIG_PM_DRUMLOCK
#include "PM_DrumLock.h"
#include "InputManagerExp.h"
#include "FeedbacksExp.h"
#include "DigitalInputsExp.h"
#include "LIB_ZeroCrossingExp.h"
#include "DigitalOutputs.h"


/*!	Define
** ************************************************************************************************
*/

#define UINT8_TMAX	0xFF

/* Value to init the feedback field of the control structure */
#define DMLGEN_FEEDBACK_NOT_INIT    	0xFF

/* Defines for LockStatusFiltered */
#define DRUMLOCK_LOCKED_OK 				0
#define DRUMLOCK_UNLOCKED_OK 			1
#define DRUMLOCK_LOCKED_GENERIC_FAIL	2
#define DRUMLOCK_NOTAVAILABLE  			0xFF

/** Gestione temporizzazione di chiamata */
#define DML_MANAGER_PERIOD       2

/*Undef position, is not a command*/
#define DMLGEN_UNDEF		0x2
#define MIN_ZC_STABILITY	100

#define SECONDS_TO_TICKS			(100 / DML_MANAGER_PERIOD)
#define T_OFF_MOTOR_LOCK_DEFAULT	(15 /* seconds */ * SECONDS_TO_TICKS)
#define T_OFF_MOTOR_UNLOCK_DEFAULT	(60 /* seconds */ * SECONDS_TO_TICKS)

/*!	This struct contains the variables to control the device
***************************************************************************************************
*/
static struct PM_DrumLock_Cfg
{
	/* configuration */
	uint8_t  lock_pin;
	uint8_t  feedback_index;		/*Feedback index for the _GLOBAL_ unsigned char Feedback[N_FEEDBACK] var in SkeletonGLobal.h*/
	uint16_t t_off_motor_lock;   /*Pilot type parameter*/
	uint16_t t_off_motor_unlock; /*Pilot type parameter*/
} PM_DrumLock_Config_Var =
{
	UINT8_TMAX,
	DMLGEN_FEEDBACK_NOT_INIT,
	T_OFF_MOTOR_LOCK_DEFAULT,
	T_OFF_MOTOR_UNLOCK_DEFAULT
};

static struct PM_DrumLock_Ctrl
{
    /* status */
	uint8_t  actual_position;       /*Actual status*/
	uint8_t  desired_position;      /*Desired status*/
	uint8_t  stop_status;           /*To control the stop status*/
	uint8_t  dmlmanagercnt;         /*Divisor cycle*/
	uint8_t  lockStatusFiltered;
	uint16_t t_off_wait_time;
} PM_DrumLock_Control_Var;

static void    PM_DrumLock_GoingNotStable( void );
static void    PM_DrumLock_Stable( void );
static uint8_t PM_DrumLock_Consistency( uint8_t fdb );
static void    PM_DrumLock_ConsistencyMngr( void );
static bool_t  PM_DrumLock_StatusReached( void );
static bool_t  PM_DrumLock_SetInitStatus( void );
static uint8_t PM_DrumLock_GetLockStatus( void );


/**
* Configures the lock pin
* \param [in]	configures the lock pin
*/
void  PM_DrumLock_Configure_LockPin( uint8_t pin )
{
	PM_DrumLock_Config_Var.lock_pin = pin;
}

/**
 * Set feedback configuration
 * \param [in]  feedback index
*/
void PM_DrumLock_ConfigFeedback( uint8_t feedbackPos )
{
	PM_DrumLock_Config_Var.feedback_index = feedbackPos;
}


/**
Init the module
\param [in]	the reset type
*/
void PM_DrumLock_InitModule( void )
{
    memset(&PM_DrumLock_Control_Var, 0, sizeof(PM_DrumLock_Control_Var));

    PM_DrumLock_Config_Var.feedback_index	  		= DMLGEN_FEEDBACK_NOT_INIT;
    PM_DrumLock_Config_Var.lock_pin 				= UINT8_TMAX;
	PM_DrumLock_Config_Var.t_off_motor_lock			= T_OFF_MOTOR_LOCK_DEFAULT;
	PM_DrumLock_Config_Var.t_off_motor_unlock		= T_OFF_MOTOR_UNLOCK_DEFAULT;

    PM_DrumLock_Control_Var.actual_position  		= DMLGEN_STATUS_NOT_INIT;
    PM_DrumLock_Control_Var.desired_position 		= DMLGEN_STATUS_NOT_INIT;
    PM_DrumLock_Control_Var.stop_status      		= DMLGEN_DEVICE_STOPPED_OK;
    PM_DrumLock_Control_Var.lockStatusFiltered 		= DRUMLOCK_NOTAVAILABLE;
}

/*!
Chk if the device is busy,if not return the code.
\return DMLGEN_DEVICE_RUNNING: busy,
		DMLGEN_DEVICE_STOPPED_OK: stop OK,
		DMLGEN_DEVICE_STOPPED_FOR_ERROR: stop with error
*/
uint8_t PM_DrumLock_DriverStatus( void )
{
	return PM_DrumLock_Control_Var.stop_status;
}

/**
* Set the initial status, reading the feedback.
* \return 	TRUE, FALSE if reading the feedback fails.
*/
static bool_t PM_DrumLock_SetInitStatus( void )
{
	uint8_t feedback_val = PM_DrumLock_GetLockStatus();
	uint8_t stop_status;
	bool_t  ret_val = FALSE;

	if (LIB_FDB_AreFeedbackReady() &&
		(feedback_val != DMLGEN_FEED_UNDEFINED))
	{
		if (DMLGEN_FEED_UNLOCK == feedback_val)
		{
			feedback_val = DMLGEN_UNLOCK;
			stop_status  = DMLGEN_DEVICE_RUNNING;
		}
		else
		{
			feedback_val = DMLGEN_LOCK;
			stop_status  = DMLGEN_DEVICE_STOPPED_OK;
		}

		PM_DrumLock_Control_Var.actual_position  = feedback_val;
		PM_DrumLock_Control_Var.desired_position = DMLGEN_LOCK;
		PM_DrumLock_Control_Var.stop_status      = stop_status;
		PM_DrumLock_Control_Var.t_off_wait_time  = 0;

		ret_val = TRUE;
	}

    return ret_val;
}


/**
* Check if the desired status id stable
* \return 	TRUE desired status , FALSE undesired status
*/
static bool_t PM_DrumLock_StatusReached( void )
{
	uint8_t feed_val; /* Feedback value */
	bool_t  ret_val = FALSE;

    /* Reads the feedback status */
    feed_val = PM_DrumLock_GetLockStatus();

    if ((DMLGEN_FEED_LOCK == feed_val) &&
    	(DMLGEN_LOCK == PM_DrumLock_Control_Var.desired_position))
    {
        /* OPEN->CLOSE */
    	ret_val = TRUE;
    }
    else
    if ((DMLGEN_FEED_UNLOCK == feed_val) &&
    	(DMLGEN_UNLOCK == PM_DrumLock_Control_Var.desired_position))
    {
        /* CLOSE->OPEN */
    	ret_val = TRUE;
    }

    return ret_val;
}


/**
* Issue a command to the device.
* The commands accepted are only DMLGEN_LOCK and DMLGEN_UNLOCK
* The command could not be accepted if the device is not in the correct state,
* the device must be initialized.
* \param [in]	the command DMLGEN_LOCK or DMLGEN_UNLOCK
* \return 	TRUE if the commans is accepted.
*/
bool_t PM_DrumLock_SetPosition( uint8_t drum_position )
{
	uint8_t feed_val;
    bool_t retVal = FALSE;

    /* Checks the initialization of the device status */
    if ((DMLGEN_STATUS_NOT_INIT == PM_DrumLock_Control_Var.actual_position ) ||
        (DMLGEN_STATUS_NOT_INIT == PM_DrumLock_Control_Var.desired_position) )
    {
    	PM_DrumLock_SetInitStatus();
    	retVal = FALSE;
    }
    else
    {
		/* Checks the command, accepts only UNLOCK e LOCK */
		if ((drum_position != DMLGEN_UNLOCK) && (drum_position != DMLGEN_LOCK))
		{
			retVal = FALSE;
		}
		else
		{
			if (PM_DrumLock_Control_Var.desired_position == drum_position)
			{
				retVal = TRUE;
			}
			else
			{
				feed_val = 	PM_DrumLock_GetLockStatus();

				switch (feed_val)
				{
					case DMLGEN_FEED_UNLOCK:
					{
						/* OPEN -> CLOSE*/
						PM_DrumLock_Control_Var.actual_position = DMLGEN_UNLOCK;
						PM_DrumLock_Control_Var.stop_status	    = DMLGEN_DEVICE_RUNNING;

						if (DMLGEN_UNLOCK == drum_position)
						{
							/* Unlock has been commanded from unlocked condition,
							 * maybe the drum is not aligned, therefore charge the t_OFF_Motor_Unlocked counter */
							PM_DrumLock_Control_Var.t_off_wait_time = PM_DrumLock_Config_Var.t_off_motor_unlock;
						}
						else
						{
						    PM_DrumLock_Control_Var.t_off_wait_time = 0;
						}
					}
						break;
					case DMLGEN_FEED_LOCK:
					{
						/* CLOSE -> OPEN*/
						PM_DrumLock_Control_Var.actual_position = DMLGEN_LOCK;
						PM_DrumLock_Control_Var.stop_status	    = DMLGEN_DEVICE_RUNNING;
						/* Unlock has been commanded, therefore charge the t_OFF_Motor_Locked counter */
					    PM_DrumLock_Control_Var.t_off_wait_time = PM_DrumLock_Config_Var.t_off_motor_lock;
					}
						break;
					default:
					{
						PM_DrumLock_Control_Var.actual_position  = DMLGEN_UNDEF;
						PM_DrumLock_Control_Var.stop_status	     = DMLGEN_DEVICE_STOPPED_OK;
					    PM_DrumLock_Control_Var.t_off_wait_time  = 0;
					}
						break;
				}

				/*Update the desired status*/
				PM_DrumLock_Control_Var.desired_position = drum_position;

			    retVal = TRUE;
			}
		}
    }

    return retVal;
}


/*!
* Get the feedback value and map it to a lock status
* \return 	DMLGEN_FEED_LOCK, DMLGEN_FEED_UNLOCK
*/
static uint8_t PM_DrumLock_GetLockStatus( void )
{
	uint8_t feedback_value;
	uint8_t retVal = DMLGEN_FEED_UNDEFINED;

	if (PM_DrumLock_Config_Var.feedback_index != DMLGEN_FEEDBACK_NOT_INIT)
	{
		feedback_value = LIB_DIO_GetDigInputVal(PM_DrumLock_Config_Var.feedback_index);

		if (feedback_value)
		{
			retVal = DMLGEN_FEED_LOCK;
		}
		else
		{
			retVal = DMLGEN_FEED_UNLOCK;
		}
	}

    return retVal;
}

/*!
* Get the feedback value and map it to a lock status.
* \return 	DMLGEN_FEED_LOCK, DMLGEN_FEED_UNLOCK, DMLGEN_FEED_UNDEFINED
* filtered version
*/
uint8_t PM_DrumLock_GetLockStatusFiltered( void )
{
	uint8_t retVal = DMLGEN_FEED_UNDEFINED;

    switch (PM_DrumLock_Control_Var.lockStatusFiltered)
    {
		case DRUMLOCK_UNLOCKED_OK:
			retVal = DMLGEN_FEED_UNLOCK;
			break;
		case DRUMLOCK_LOCKED_OK:
			retVal = DMLGEN_FEED_LOCK;
			break;
	}

    /* Hides the actual lock status if the tOFF is not expired yet */
	if (PM_DrumLock_Control_Var.t_off_wait_time > 0)
	{
		retVal = DMLGEN_FEED_UNDEFINED;
	}

    return retVal;
}


/*!
 * Check the consistency of the zc and drum sensing feedback in a specific desired_position
 */
static void PM_DrumLock_ConsistencyMngr( void )
{
	uint8_t lockSensor;
	uint8_t retVal;

	/* Get the lock position from the feedback */
	lockSensor = PM_DrumLock_GetLockStatus();

	/* Check if the actual position is the desired one */
    retVal = PM_DrumLock_Consistency(lockSensor);

   	PM_DrumLock_Control_Var.lockStatusFiltered = retVal;
}


/**
* Manages the device state machine
*/
void PM_DrumLock_Manager( int evt )
{
	EnterCriticalSection();

	PM_DrumLock_Control_Var.dmlmanagercnt += 1;
	PM_DrumLock_Control_Var.dmlmanagercnt %= DML_MANAGER_PERIOD;

	if (PM_DrumLock_Control_Var.dmlmanagercnt != 0) /* divisore, potrebbe essere ottimizzata gestendo la chiamata ogni 20ms dal chiamante */
	{
		PM_DrumLock_ConsistencyMngr();

		/* Checks the device is initialized */
		if ((PM_DrumLock_Config_Var.lock_pin != 0xFF) &&
			(PM_DrumLock_Config_Var.feedback_index != DMLGEN_FEEDBACK_NOT_INIT))
		{
			bool_t do_nothing = FALSE;
			/* Checks the initialization of the device status ------------------------------- */
			if ((DMLGEN_STATUS_NOT_INIT == PM_DrumLock_Control_Var.actual_position ) ||
				(DMLGEN_STATUS_NOT_INIT == PM_DrumLock_Control_Var.desired_position) )
			{
				/* Device NOT Initialized */
				if (FALSE == PM_DrumLock_SetInitStatus())
				{
					do_nothing = TRUE;
				}
			}
			if ( ! do_nothing)
			{
				/* Is transition on going? ------------------------ */
				if (PM_DrumLock_Control_Var.actual_position != PM_DrumLock_Control_Var.desired_position)
				{
					/* Status stable? (checking the feedback) */
					if (PM_DrumLock_StatusReached())
					{
						/* Status stable */
						PM_DrumLock_Control_Var.actual_position = PM_DrumLock_Control_Var.desired_position;
					}
					else
					{
						/* Status NOT stable */
						PM_DrumLock_GoingNotStable();
					}
				}
				else
				{
					/* Stable position */
					PM_DrumLock_Stable();
				}
			}
		}
	}
	ExitCriticalSection();
}

static void PM_DrumLock_Stable( void )
{
    /* Update the tOFF counter */
	if (PM_DrumLock_Control_Var.t_off_wait_time > 0)
	{
		/* This is decremented every EV_10MS * DML_MANAGER_PERIOD */
		PM_DrumLock_Control_Var.t_off_wait_time -= 1;
	}

    if (DMLGEN_UNLOCK == PM_DrumLock_Control_Var.desired_position)
    {
       	LIB_DIO_SET_LOAD(ON, PM_DrumLock_Config_Var.lock_pin);
    }
    else
    {
        /* The device is locked */
		LIB_DIO_SET_LOAD(OFF, PM_DrumLock_Config_Var.lock_pin);
    }

    if (DMLGEN_DEVICE_RUNNING == PM_DrumLock_Control_Var.stop_status)
	{
		PM_DrumLock_Control_Var.stop_status = DMLGEN_DEVICE_STOPPED_OK;
	}
}

/*!
* Detect unstable condition related to the drum lock.
*/
static void PM_DrumLock_GoingNotStable( void )
{
    if (DMLGEN_LOCK == PM_DrumLock_Control_Var.desired_position)
    {
        LIB_DIO_SET_LOAD(OFF, PM_DrumLock_Config_Var.lock_pin);
    }
    else
    {
        LIB_DIO_SET_LOAD(ON, PM_DrumLock_Config_Var.lock_pin);
    }
}

/*!
	Configure the device timing before declaring lock position
	\param t_off_motor_lock  	Motor hold-off time, after the nominal unlock command and when unlocked condition is detected
	\param t_off_motor_unlock	Motor hold-off time, after the unlock command and under unlocked condition
*/
void  PM_DrumLock_Configure_MotorOffTiming( uint8_t t_off_motor_lock, uint8_t t_off_motor_unlock )
{
	PM_DrumLock_Config_Var.t_off_motor_lock   = (uint16_t)t_off_motor_lock   * SECONDS_TO_TICKS;
	PM_DrumLock_Config_Var.t_off_motor_unlock = (uint16_t)t_off_motor_unlock * SECONDS_TO_TICKS;
}

/**
TODO
*/
static uint8_t PM_DrumLock_Consistency( uint8_t fdb )
{
    uint8_t retVal = DRUMLOCK_NOTAVAILABLE;

    if (DMLGEN_DEVICE_STOPPED_OK == PM_DrumLock_Control_Var.stop_status)
    {
		switch (PM_DrumLock_Control_Var.desired_position)
		{
			case DMLGEN_UNLOCK: /* Drum unlocked */
			{
				if (DMLGEN_FEED_UNLOCK == fdb)
				{
					retVal = DRUMLOCK_UNLOCKED_OK;
				}
				else
				{
					retVal = DRUMLOCK_LOCKED_GENERIC_FAIL;
				}
			}
				break;
			case DMLGEN_LOCK: /* Drum locked */
			{
				if (DMLGEN_FEED_LOCK == fdb)
				{
					retVal = DRUMLOCK_LOCKED_OK;
				}
				else
				{
					retVal = DRUMLOCK_LOCKED_GENERIC_FAIL;
				}
			}
				break;

			default: /* DMLGEN_FEED_UNDEFINED */
				break;
		}
    }

	return retVal;

}

#endif /* CONFIG_PM_DRUMLOCK*/
