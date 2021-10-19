/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	PM_DoorLock
\n		Contains the skeleton ad main funcions of the module PM_DoorLock
\n
\file		PM_DoorLock.c
\ingroup    PM_DoorLock
\date	    20/09/2011
\version	01.00.00
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

#define MODULE_BODY_DOORLOCK

#include "Config.h"
#if ! defined( CONFIG_PM_DOORLOCK_MR_ENABLE )
#ifdef CONFIG_PM_DOORLOCK
#include <string.h>
#include "LIB_ClassBVar.h"
#include "LIB_Error.h"

#include "OutputManagerExp.h"
#include "DigitalOutputs.h"
#include "LIB_ZeroCrossingExp.h"
#include "Feedbacks.h"
#include "DigitalInputsExp.h"
#ifdef CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT
#include "PM_FrequencyFeedbacks.h"
#endif

#include "FeedbacksExp.h"
#include "InputManagerExp.h"

#include "PM_DoorLock.h"
#include "LIB_Trace.h"

#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
#include "PM_DoorLock_Pulse.h"
/*#include "BP_Omnium.h"*/
#endif /*  CONFIG_PM_DOORLOCK_PULSE_ENABLE*/

#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
#include "PM_DoorLock_Wax.h"
#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE*/

#include "syncSvc.h"

/*	Define
** ************************************************************************************************
*/

#define BP_IMPULSI_MANAGER_PERIOD   2			/**< divider, based to 10ms */

/*Undef position, is not a command*/
#define BPGEN_UNDEF					0x2
/* #define CONFIG_MIN_ZC_STABILITY			(100) 		*/	/**< filter for zc stability x 10ms*/
/* #define CONFIG_CONSISTENCY_FILTER_MAX_CNT 	(20)	*/	/**< filter for consistency stability x 20ms*/
#define ZC_OFF_STABILITY_FILTER		(10)


/*	This struct contains the variables to control the device
***************************************************************************************************
*/

/** CLASS_B_VAR
 *  datacare potected data
 */
struct PM_DoorLock_Cfg   PM_DoorLock_Config_Var;
struct PM_DoorLock_Ctrl PM_DoorLock_Control_Var;
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
struct SubscriberNode PM_DoorLock_ZcNotify;
#endif
/**
 * doorLockStatusSaved used to backup last status reached ok,
 * is not on datacare because used only on reset and there are not safety issue
 */
int doorLockStatusSaved=0xff;

/* local functions declaration*/
uint8_t PM_DoorLock_ConsistencyMngr( void );


/** Configure the pilot type
 *  installing the driver
 *  \param [in] pilot_type
 */
void  PM_DoorLock_Configure_PilotType(uint8_t pilot_type)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Config_Var);
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

	PM_DoorLock_Config_Var.pilot_type = pilot_type;

    /* installa le callback*/

    switch(PM_DoorLock_Config_Var.pilot_type)
    {
#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
    case BLOCCO_PORTA_IMPULSI:
        PM_DoorLock_Pulse_Install_drv( PM_DoorLock_Config_Var.pilot_type );
        break;
#endif /*CONFIG_PM_DOORLOCK_PULSE_ENABLE*/

#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
    case BLOCCO_PORTA_CERA:
    	PM_DoorLock_Wax_Install_drv( PM_DoorLock_Config_Var.pilot_type );
        break;
#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE*/
    default:
    	PLT_ERROR_FATAL(PLT_ERROR_DOORLOCK_CONFIG);
    }

    /* Chiama la reset*/
    PM_DoorLock_Config_Var.func->reset(PM_DoorLock_Config_Var.pilot_type);

    SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var );
    SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var);
    SAFETY_REGION_END();
}

/** Configure the control pin
 * \param [in] pin index of pin
 */
void  PM_DoorLock_Configure_ControlPin(uint8_t pin)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Config_Var);

	PM_DoorLock_Config_Var.control_pin = pin;

	SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var );
	SAFETY_REGION_END();
}

/** Configure the used feedback
 * \param [in]  numFb
 * \param [in]  readType
 * \param [in]  feedbackPos
 *
 * \return 0=OK 1=fail
 */
uint8_t PM_DoorLock_ConfigFeedback(uint8_t numFb, uint8_t readType, uint8_t feedbackPos)
{
	uint8_t retVal = 0;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Config_Var);

	/*Configure feedback for locked door in position 0 and sensing in pos 1*/
	if (readType == ZEROCROSSING_I) {
		numFb = IDX_ZC;
	} else {
		numFb = IDX_FB;
	}
	if (numFb < MAX_NUM_FB) /* max 2 feedbacks*/
	{
		PM_DoorLock_Config_Var.feedback_index[numFb] = feedbackPos;
		PM_DoorLock_Config_Var.feedback_type[numFb] = readType;

		if (!(ZEROCROSSING_I == readType || DIGITAL_I == readType
#if defined(CONFIG_FEEDBACK_ENABLE)
				|| FEEDBACK_I == readType
#endif
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
				|| FREQFEEDBACK_I == readType)
#endif
		) {
			retVal = 1; /* error , input not supported*/
		}
	} else {
		retVal = 1;
	}

	SAFETY_DATA_UPDATE(PM_DoorLock_Config_Var);
	SAFETY_REGION_END();

	PLT_ERROR_FATAL_IF(retVal, PLT_ERROR_DOORLOCK_CONFIG);

	return retVal;
}

/** Configure sensing
 * \param [in]  isSensing true to configure a sensing device
 */
void PM_DoorLock_Configure_Sensing( bool_t isSensing)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_DoorLock_Config_Var );

	PM_DoorLock_Config_Var.isSensing = isSensing;

	SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var );
	SAFETY_REGION_END();
}

/** Init the module
 * \param [in]	isColdReset the reset type
*/
void PM_DoorLock_InitModule( uint8_t isColdReset )
{
	SAFETY_REGION_START();
    if( isColdReset )
    {
    	memset(&PM_DoorLock_Control_Var, 0, sizeof(PM_DoorLock_Control_Var));
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
        PM_DoorLock_ZcNotify.callback = &PM_DoorLock_Manager_OnZc;
        PM_DoorLock_ZcNotify.event.type = EV_ZC;
        SUBSCRIBE_SYNC(&PM_DoorLock_ZcNotify);

#endif

		PM_DoorLock_Config_Var.feedback_index[IDX_ZC]  	= BPGEN_FEEDBACK_NOT_INIT;
		PM_DoorLock_Config_Var.feedback_index[IDX_FB]  	= BPGEN_FEEDBACK_NOT_INIT;
		/* non necessario
		PM_DoorLock_Config_Var.isSensing				= 0;
		PM_DoorLock_Config_Var.isConfigured				= 0;
		*/

		PM_DoorLock_Control_Var.actual_position  		= BPGEN_STATUS_NOT_INIT;
		PM_DoorLock_Control_Var.desired_position 		= BPGEN_STATUS_NOT_INIT;
		PM_DoorLock_Control_Var.last_desired_position 	= BPGEN_STATUS_NOT_INIT;
		PM_DoorLock_Control_Var.stop_status      		= BPGEN_DEVICE_STOPPED_OK;
		PM_DoorLock_Control_Var.pilot_pin_flag			= ON;
		PM_DoorLock_Control_Var.cmdPin					= CMD_NONE;
		PM_DoorLock_Control_Var.consistencyFiltered 	= DOORLOCK_NOTAVAILABLE;
		PM_DoorLock_Control_Var.activated				= 0;

		SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var);
		SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var);

    }else{
    	SAFETY_DATA_CHECK( PM_DoorLock_Config_Var);
    	SAFETY_DATA_CHECK( PM_DoorLock_Control_Var);
    }
	SAFETY_REGION_END();
}

/** Set the end of configuration */
void PM_DoorLock_Configure_Ended(void){
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Config_Var);

	PM_DoorLock_Config_Var.isConfigured = 1;

	SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var);
	SAFETY_REGION_END();
}

/** Get the internal consistency
 *  and manage the warning door,
 *  depends on CONFIG_DOORLOCK_WARNING_DOOR_DISABLE
*/
uint8_t PM_DoorLock_InternalConsistency( void )
{
	/* gestione warning door
	 * stato precedente richiesto = UNLOCK
	 * AND stato attuale richiesto = LOCK
	 * AND stop_status = ERROR
	 * Allora warning door
	 */
	uint8_t retVal = PM_DoorLock_Control_Var.consistencyFiltered;
#if defined(CONFIG_DOORLOCK_WARNING_DOOR_DISABLE)
	/* disable the warning door detection */
#else
	if(         PM_DoorLock_Control_Var.activated
			&& PM_DoorLock_Control_Var.statusFiltered == BPGEN_OPEN /*(PM_DoorLock_Control_Var.last_desired_position  == BPGEN_OPEN   ) */
			&& (PM_DoorLock_Control_Var.desired_position       == BPGEN_CLOSE  )
			&& (PM_DoorLock_Control_Var.stop_status == BPGEN_DEVICE_STOPPED_FOR_ERROR)
	 	 )
	{
		retVal=DOORLOCK_WARNING_DOOR;
	}
#endif
    return retVal;
}

/** Internal function to take a pulse on pin drive
 * if there is the ZC initiates synchronized on the rising edge,
 * otherwise use the pin immediately
 */
void PM_DoorLock_PulseReq(void)
{
    if( goodZCSignal() )
    {
		PM_DoorLock_Control_Var.cmdPin = CMD_ON;
    }
    else
    {
			LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.on_counter = 2; /* pulse len in 10ms*/
			PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
	}
}

/** Get the status of the command
 * \return
 * \retval BPGEN_DEVICE_RUNNING   			Device running
 * \retval BPGEN_DEVICE_STOPPED_OK 		Device stable stopped
 * \retval BPGEN_DEVICE_STOPPED_FOR_ERROR	Device stable stopped for errors
*/
uint8_t PM_DoorLock_GetStatus(void)
{
		return PM_DoorLock_Control_Var.stop_status;
}

/** Internal function that defines the initial status
 *
 */
uint8_t PM_DoorLock_SetInitStatus(void)
{
	uint8_t retVal;

	if(PM_DoorLock_Config_Var.func == NULL){
		return FALSE;
	}

	if(!LIB_FDB_AreFeedbackReady()){ /*NOTA: necessario solo se usa FB normali */
		return FALSE;
	}
	
    retVal = PM_DoorLock_Config_Var.func->setInitStatus();
    if( retVal == TRUE ){
    	PM_DoorLock_Control_Var.isReady = 1;
    }

    return retVal;
}

/** Internal function sets the state required
 * \param [in] door_position BPGEN_OPEN or BPGEN_CLOSE
 * \return TRUE if the request is accepted, FALSE if not
 */
bool_t PM_DoorLock_SetPosition_Internal(uint8_t door_position)
{
    uint8_t feed_val;
    bool_t retVal=FALSE;

	/*Control the cmd, accetta solo OPEN e CLOSE*/
	PLT_ASSERT( (door_position == BPGEN_OPEN) || (door_position == BPGEN_CLOSE))

	if( ! PM_DoorLock_Control_Var.activated ){ /* se non e' stata attivata deve essere prima inizializzata */
		if (PM_DoorLock_SetInitStatus() == FALSE){ /* viene fatto finche non viene attivata  */
			return FALSE; /* non e' stata ancora inizializzata */
		}else{
			/* e' stata inizializzata */
		}
	}else{
		/* e' stata attivata */
	}


	if( PM_DoorLock_Control_Var.activated && PM_DoorLock_Control_Var.desired_position == door_position )
	{   /* se viene richiesta la posizione uguale alla posizione attuale non viene fatto nulla */
		retVal = TRUE;
	}else{

		feed_val = 	PM_DoorLock_GetLockStatus();

		switch ( feed_val )
		{
		case BPGEN_FEED_OPEN:
		{
			/* OPEN -> CLOSE*/
			PM_DoorLock_Control_Var.actual_position = BPGEN_OPEN;
			PM_DoorLock_Control_Var.stop_status	    = BPGEN_DEVICE_RUNNING;
		}
		break;
		case BPGEN_FEED_CLOSE:
		{
			/* CLOSE -> OPEN*/
			PM_DoorLock_Control_Var.actual_position = BPGEN_CLOSE;
			PM_DoorLock_Control_Var.stop_status	    = BPGEN_DEVICE_RUNNING;
		}
		break;
		case BPGEN_FEED_UNDEFINED:
		{
			PM_DoorLock_Control_Var.actual_position = BPGEN_UNDEF;
			PM_DoorLock_Control_Var.stop_status	    = BPGEN_DEVICE_STOPPED_OK;
		}
		break;
		}


		/*Update the desired status*/
		PM_DoorLock_Control_Var.last_desired_position = PM_DoorLock_Control_Var.desired_position;
		PM_DoorLock_Control_Var.desired_position      = door_position;

		retVal = PM_DoorLock_Config_Var.func->setPosition( door_position );
		PM_DoorLock_Control_Var.activated = 1; /* attivazione */
	}

    return retVal;
}

/** Interface function to require the status
 * \param [in] door_position BPGEN_OPEN or BPGEN_CLOSE
 * \return TRUE if the request is accepted, FALSE if not
 */
bool_t PM_DoorLock_SetPosition(uint8_t door_position)
{
	bool_t retVal;
	EnterCriticalSection();
    SAFETY_REGION_START();
    SAFETY_DATA_CHECK( PM_DoorLock_Config_Var );
    SAFETY_DATA_CHECK( PM_DoorLock_Control_Var );

    retVal = PM_DoorLock_SetPosition_Internal(door_position);

    SAFETY_DATA_UPDATE( PM_DoorLock_Config_Var );
	SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
	SAFETY_REGION_END();
	ExitCriticalSection();

	return retVal;
}

/** Interface function to disable/enable the control of the pin lock_pin (only wax 2pin)
 *  is used in WM with collector motor
 * \param [in] onoff, 0 disable the control
 */
void  PM_DoorLock_SetBPPilot(uint8_t onoff)
{
	EnterCriticalSection();
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_DoorLock_Control_Var);

    if(onoff)
    {
        PM_DoorLock_Control_Var.pilot_pin_flag = ON;
    }
    else
    {
        PM_DoorLock_Control_Var.pilot_pin_flag = OFF;
    }

    SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
    SAFETY_REGION_END();
    ExitCriticalSection();
}

/** Internal function to manage the delayed pin commands
 *  used to terminate the impulse, off the pin
 */

void PM_DoorLock_Manager_Time(void)
{
    if( PM_DoorLock_Control_Var.on_counter > 0)
    {
        PM_DoorLock_Control_Var.on_counter--;
	}
    else
	{
        switch( PM_DoorLock_Control_Var.cmdPin )
        {
		case CMD_OFF:		
			LIB_DIO_SET_LOAD( OFF, PM_DoorLock_Config_Var.control_pin);

			PM_DoorLock_Control_Var.cmdPin = CMD_NONE ;
			break;
		}
	}
}

/**
* Internal function gets the feedback regardless of type and convert it to a logical feedback value for the device
* \param [in]   the feedback index
* \return	BPGEN_FEED_OPEN, BPGEN_FEED_CLOSE, BPGEN_FEED_UNDEFINED
*/
uint8_t PM_DoorLock_GetFeedbackVal(uint8_t pos)
{
    uint8_t feedback_value=BPGEN_FEED_UNDEFINED;

    switch(PM_DoorLock_Config_Var.feedback_type[pos])
    {
	case ZEROCROSSING_I:
		if( PM_DoorLock_Control_Var.zcFail == 1){ /* nel caso si  zc fail suppone la porta chiusa */
			feedback_value = BPGEN_FEED_CLOSE;
		}else{
			if(goodZCSignal()){
				feedback_value = BPGEN_FEED_CLOSE;
			}else{
				feedback_value = BPGEN_FEED_OPEN;
			}
		}
	break;
#if defined(CONFIG_FEEDBACK_ENABLE)
	case FEEDBACK_I:
		feedback_value = LIB_FDB_GetFeedbackVal(PM_DoorLock_Config_Var.feedback_index[pos]);
 	break;
#endif
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
	case FREQFEEDBACK_I:
		if( PM_FrequencyFeedbacks_GetValueByPosition(pos, &feedback_value) != FREQFB_OK ){
			feedback_value = BPGEN_FEED_UNDEFINED;
		}
	break;
#endif
 	case DIGITAL_I:
 		feedback_value = LIB_DIO_GetDigInputVal(PM_DoorLock_Config_Var.feedback_index[pos]);
	break;
	}

    return feedback_value;
}


/** Get the feedback value and map it to a lock status
* \return 	BPGEN_FEED_CLOSE, BPGEN_FEED_OPEN
*/
uint8_t PM_DoorLock_GetLockStatus(void)
{
	uint8_t feedback_value;
	uint8_t retVal = BPGEN_FEED_UNDEFINED;
	uint8_t pos=IDX_ZC;

    if( PM_DoorLock_Config_Var.feedback_index[IDX_ZC] == BPGEN_FEEDBACK_NOT_INIT )
    {   /** ZC non presente, quindi considera come sensore di chiusura il FB */
    	if( PM_DoorLock_Config_Var.feedback_index[IDX_FB] != BPGEN_FEEDBACK_NOT_INIT ){
    		pos = IDX_FB;
    	}
    }

    feedback_value = PM_DoorLock_GetFeedbackVal(pos);

    switch(PM_DoorLock_Config_Var.feedback_type[pos])
    {
	case ZEROCROSSING_I:
		if(feedback_value){
			retVal = BPGEN_FEED_CLOSE;
		}else{
			retVal = BPGEN_FEED_OPEN;
		}
	break;
#if defined(CONFIG_FEEDBACK_ENABLE)
	case FEEDBACK_I:
        switch(feedback_value)
        {
			case FDB_INPUT_LOW:
			case FDB_INPUT_HI:
				retVal=BPGEN_FEED_OPEN;
			break;
			case FDB_INPUT_FREQ_OPP_PHASE:
			case FDB_INPUT_FREQ_PHASE:
				retVal = BPGEN_FEED_CLOSE;
			break;
		}
 	break;
#endif
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
	case FREQFEEDBACK_I:
        switch(feedback_value)
        {
			case FDB_INPUT_LOW:
			case FDB_INPUT_HI:
				retVal=BPGEN_FEED_OPEN;
			break;
			case FDB_INPUT_FREQ_OPP_PHASE:
			case FDB_INPUT_FREQ_PHASE:
				retVal = BPGEN_FEED_CLOSE;
			break;
		}
	break;
#endif
 	case DIGITAL_I:
 		if(feedback_value){
 			retVal = BPGEN_FEED_CLOSE;
 		}else{
 		    retVal = BPGEN_FEED_OPEN;
 		}
	break;
	}

    return retVal;
}

/** Get the feedback value and map it to a lock status
 * \return 	BPGEN_FEED_CLOSE, BPGEN_FEED_OPEN
 */
uint8_t PM_DoorLock_GetLockStatusFiltered(void)
{
	uint8_t retVal;

	switch(PM_DoorLock_Control_Var.consistencyFiltered)
    {
	case DOORLOCK_UNLOCKED_OK:
		retVal = BPGEN_FEED_OPEN;
	break;
	case DOORLOCK_LOCKED_OK:
		retVal = BPGEN_FEED_CLOSE;
		break;
	default:
		retVal = PM_DoorLock_GetLockStatus(); /* stato aperto/chiuso rilevato */
		if( !retVal ){
			retVal = PM_DoorLock_Control_Var.statusFiltered;/* ultimo stato raggiunto con successo */
		}
	}

	return retVal;
}


/** Interface function to get the enable status of the control pin of DoorLock
 *  \return 0 = disabled
 */
uint8_t PM_DoorLock_GetPilotEnable(void)
{
    return PM_DoorLock_Control_Var.pilot_pin_flag;
}

/** Get the DoorLock type */
uint8_t PM_DoorLock_GetDoorLockType( void )
{
    return (PM_DoorLock_Config_Var.pilot_type);
}

/** Check the consistency of the zc and door sensing feedback in a specific desired_position
 * \return The calculated status
 *
 * \par Architecture Requirements ID: SARS_WMMT_MN_DLM_1
 */
uint8_t PM_DoorLock_ConsistencyMngr( void )
{
	uint8_t retVal;

	if(PM_DoorLock_Config_Var.func == NULL){
		return DOORLOCK_NOTAVAILABLE;
	}

	if( !PM_DoorLock_Control_Var.isReady ){
		return DOORLOCK_NOTAVAILABLE;
	}

	if( PM_DoorLock_Control_Var.consistencyFilteredCnt < CONFIG_CONSISTENCY_FILTER_MAX_CNT)
	{
		PM_DoorLock_Control_Var.consistencyFilteredCnt++;
	}

	retVal = PM_DoorLock_Config_Var.func->getConsistency(); /* rileva la consistency attuale */

	if(BPGEN_DEVICE_RUNNING == PM_DoorLock_Control_Var.stop_status)
	{ /* consistebcy non disponibile in running */
		retVal = DOORLOCK_NOTAVAILABLE;
	}else{ /* se la consistency attuale non è disponibile usa quella precedente */
    	if(retVal == DOORLOCK_NOTAVAILABLE){
    		retVal = PM_DoorLock_Control_Var.retValOld;
    	}
	}

    if( (PM_DoorLock_Control_Var.retValOld != retVal)  /* se c'e' una variazione di consistency */
    		|| (getZCStability() < CONFIG_MIN_ZC_STABILITY))  /* se la stabilità del segnale ZC è minore del valore di filtro */
    { /* reset del filtro */
    	PM_DoorLock_Control_Var.consistencyFilteredCnt=0;
	}

    PM_DoorLock_Control_Var.retValOld = retVal;

    if( PM_DoorLock_Control_Var.consistencyFilteredCnt == CONFIG_CONSISTENCY_FILTER_MAX_CNT)
    { /* se non stati raggiunti il numero di rilevamenti indicati come filtro */
        PM_DoorLock_Control_Var.consistencyFiltered = retVal;
	}else{ /* se il numero di rilevazioni non e' completo */
		PM_DoorLock_Control_Var.consistencyFiltered = DOORLOCK_NOTAVAILABLE;
	}

    /* Check for accumulation errors */
    if( PM_DoorLock_Control_Var.errCnt >= CONFIG_PM_DOORLOCK_MAX_ERRORS){
    	PM_DoorLock_Control_Var.consistencyFiltered = DOORLOCK_FAULT_FB;
    }

	return retVal;
}

/** Manage the event on ZC
 * used to synchronize the start of impulse requested
 * \param [in] pEvent, type EV_ZC , data ZC_EDGE_FALLING,ZC_EDGE_RISING
 */
void PM_DoorLock_Manager_OnZc(NotifyDataEvent* pEvent)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_DoorLock_Control_Var );

	PLT_ASSERT(pEvent->type == EV_ZC) /* ensure the event type */

	if( ((NotifyDataEvent*)pEvent)->data == ZC_EDGE_FALLING )
	{
		switch( PM_DoorLock_Control_Var.cmdPin )
		{
		case CMD_IMPULSE_ON_NEG:
			LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.cmdPin = CMD_IMPULSE_ON_NEG_OFF;
			break;
		case CMD_IMPULSE_ON_POS_OFF:
			LIB_DIO_SET_LOAD( OFF, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.cmdPin = CMD_NONE ;
			break;
		}
	}
	else
	{
		switch( PM_DoorLock_Control_Var.cmdPin )
		{
		case CMD_OFF:
			if( 0 == PM_DoorLock_Control_Var.on_counter )
			{
				LIB_DIO_SET_LOAD( OFF, PM_DoorLock_Config_Var.control_pin);
				PM_DoorLock_Control_Var.cmdPin = CMD_NONE ;
			}
			break;
		case CMD_ON:
			LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);
			/*PM_DoorLock_Control_Var.cmdPin = CMD_NONE ;*/
			PM_DoorLock_Control_Var.cmdPin = CMD_OFF;
			PM_DoorLock_Control_Var.on_counter = 2;
			break;
		case CMD_ON_CERA:
			LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.cmdPin = CMD_OFF;

			break;
		case CMD_IMPULSE_ON_POS:
			LIB_DIO_SET_LOAD( ON, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.cmdPin = CMD_IMPULSE_ON_POS_OFF;
			break;
		case CMD_IMPULSE_ON_NEG_OFF:
			LIB_DIO_SET_LOAD( OFF, PM_DoorLock_Config_Var.control_pin);
			PM_DoorLock_Control_Var.cmdPin = CMD_NONE ;
			break;
		}
	}

    SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
    SAFETY_REGION_END();
}

/** Manage the state machine to pilot the DoorLock
 *
 * \par Architecture Requirements ID: SARS_WMMT_MN_DLM_2
 */
void PM_DoorLock_Manager( int evt)
{
	EnterCriticalSection();
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_DoorLock_Config_Var );
	SAFETY_DATA_CHECK( PM_DoorLock_Control_Var);

	if(PM_DoorLock_Config_Var.func != NULL)
	{
		PM_DoorLock_Manager_Time(); /* ogni 10ms*/

		PM_DoorLock_Control_Var.bpmanagercnt++;
		PM_DoorLock_Control_Var.bpmanagercnt %= BP_IMPULSI_MANAGER_PERIOD;

		if ( PM_DoorLock_Control_Var.bpmanagercnt != 0 ) /* divisore, potrebbe essere ottimizzata gestendo la chiamata ogni 20ms dal chimante.*/
		{

			PM_DoorLock_ConsistencyMngr();

			/*Controllo che la device sia inizializzata*/
			if ( PM_DoorLock_Config_Var.isConfigured )
			{
				bool_t do_nothing = FALSE;
				/*Control the initialization of the device's status -------------------------------*/
				if ( PM_DoorLock_Control_Var.activated ) /* attende il primo comando */
				{
					/*Transition is on going? ------------------------*/
					if( PM_DoorLock_Control_Var.actual_position != PM_DoorLock_Control_Var.desired_position )
					{
						/*Status stable? (checking the feedback)*/
						if ( PM_DoorLock_Config_Var.func->statusReached() )
						{
							/* Status stable*/
							PM_DoorLock_Control_Var.actual_position = PM_DoorLock_Control_Var.desired_position;

							PM_DoorLock_Config_Var.func->enterStable();

						}
						else
						{
							/* Status NOT stable*/
							PM_DoorLock_Config_Var.func->onGoingNotStable();
						}
					}
					else
					{
						/* Stable position*/
						PM_DoorLock_Config_Var.func->onStable();
						if (PM_DoorLock_Control_Var.stop_status == BPGEN_DEVICE_STOPPED_OK)
						{
							doorLockStatusSaved = PM_DoorLock_Control_Var.actual_position;
							PM_DoorLock_Control_Var.statusFiltered =  PM_DoorLock_Control_Var.actual_position;
						}
					}
				}
			}
		}
	}
	SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
	SAFETY_REGION_END();
	ExitCriticalSection();
}

/** Interface function to detect unstable conditions
 * \return TRUE is unstable conditions
 */
bool_t PM_DoorLock_isInstableCondition(void)
{
	bool_t retVal = FALSE;

	if( 		(PM_DoorLock_Control_Var.stop_status == BPGEN_DEVICE_STOPPED_OK)
			&& 	(PM_DoorLock_Control_Var.consistencyFiltered == DOORLOCK_LOCKED_FAIL_ZC)
			&&  (getZCStability() < CONFIG_MIN_ZC_STABILITY) ){
		retVal = TRUE;
	}
	return retVal;
}

void PM_DoorLock_ForceUnlockDoor(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( PM_DoorLock_Control_Var );

	if( PM_DoorLock_Control_Var.zcFail == 0){
		PM_DoorLock_Control_Var.zcFail = 1;
	}

	SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
	SAFETY_REGION_END();
}

void PM_DoorLock_ResetForceUnlockDoor(void)
{
	if(PM_DoorLock_Control_Var.zcFail != 0 )
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK( PM_DoorLock_Control_Var );

		PM_DoorLock_Control_Var.zcFail = 0;

		SAFETY_DATA_UPDATE( PM_DoorLock_Control_Var );
		SAFETY_REGION_END();
	}
}

void PM_DoorLock_ResetFeedback(void)
{
	uint8_t pos =  PM_DoorLock_Config_Var.feedback_index[IDX_FB];
	switch(PM_DoorLock_Config_Var.feedback_type[IDX_FB])
	    {
		case ZEROCROSSING_I:

		break;
#if defined(CONFIG_FEEDBACK_ENABLE)
		case FEEDBACK_I:
			LIB_FDB_FeedbackInitMeasure(pos);
	 	break;
#endif
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
		case FREQFEEDBACK_I:
#if defined(CONFIG_DOORLOCK_NO_FB)
			/* PM_FrequencyFeedbacks_RestartFeedbackByPosition( pos ); crea una situazione oscillante per il fb del motore */
#else
			PM_FrequencyFeedbacks_RestartFeedbackByPosition( pos );
#endif
		break;
#endif
	 	case DIGITAL_I:

		break;
		}


}

/* Check if the configutation is ok
 *
 *  \return 0 = OK else error
 */
uint8_t PM_DoorLock_CheckConfig(void)
{
	uint8_t retVal=0;
	if(		PM_DoorLock_Config_Var.feedback_index[IDX_ZC] == BPGEN_FEEDBACK_NOT_INIT
		&&  PM_DoorLock_Config_Var.feedback_index[IDX_FB] == BPGEN_FEEDBACK_NOT_INIT)
	{
		retVal=1;
	}
	return retVal;
}

#endif /* CONFIG_PM_DOORLOCK*/
#endif /* #if !defined( CONFIG_PM_DOORLOCK_MR_ENABLE ) */
