/*
Copyright (c) 2013-2015, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_TurbiditySensor
\n			The aim of the Turbidity Sensor module is to provide an index of the transparency
of the water drain. To obtain that index it makes use of:
 	 	 	 - PWM channel that drives a LED
 	 	 	 - ADC channel driven by a photo-sensitive component coupled with the LED.

 	 	 	 The module provides the following operations:
 	 	 	 -   calibration: the module must find the PWM value so that
 	 	 	 the output from ADC reaches the nearest value to the reference.
 	 	 	 - acquisition: given a PWM, the module acquire the related ADC value.

 	 	 	 The period of the PWM is given as it depends on the features of the hw components.


\n
\n
\file		PM_TurbiditySensor.c
\ingroup	PM_TurbiditySensor
\date		14/10/2013
\author		Roberto Fioravanti
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "PM_TurbiditySensor.h"

#include "Analog.h"

#include "OCTPM_drv.h"
#include "OCDIO_drv.h"

#include "Tools.h"



#if defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**/
#define TURB_SENS_PWM_PERIOD 							1638400 /* given by period [ticks] x tick duration [ns] x prescaler */

/* Turbidity Sensor LLI settings macro*/
#define SF_TURBSENS_GET_MAXDELAY_RAW(_ptr)				MBGET(1, _ptr, delay)
#define SF_TURBSENS_GET_FLAGS(_ptr)						MBGET(1, _ptr, flags)
#define SF_TURBSENS_GET_POLARITY(_ptr)					BF_8_GET(MBGET(1, _ptr, flags), 0, 1)

/* Events bit */
enum ePM_TurbSensInputsBmp
{
	TURB_SENS_TIME_TO_SETUP_AD=0,		/* 0 */
	TURB_SENS_CORRECTION_EMPTY,			/* 1 */
	TURB_SENS_AD_READY,					/* 2 */
	TURB_SENS_INPUTS_NUM/* current maximum: 7 -> PM_TurbSensCalibFsmInType*/
};

/* Actions bit */
enum ePM_TurbSensActionsBmp
{
	TURB_SENS_SETUP_AD_BIT=0,			/* 0 */
	TURB_SENS_DONE_BIT,					/* 1 */
	TURB_SENS_INIT_PWM_BIT,				/* 2 */
	TURB_SENS_EVAL_PWM_BIT,				/* 3 */
	TURB_SENS_BEFORE_SETUP_AD_BIT,		/* 4 */
	TURB_SENS_ACQ_PWM_INIT,				/* 5 */
	TURB_SENS_ACQ_DONE_BIT,				/* 6 */
	TURB_SENS_ACTIONS_NUM/* current maximum: 16 -> PM_TurbSensCalibFsmOutType*/
};

/* Sensor Calibration */
typedef enum ePM_TurbSensCalibState
{
/* Calibration */
	TURB_SENS_CALIB_ENTRY=0,
	TURB_SENS_CALIB_WAIT_FOR_AD_SETUP,
	TURB_SENS_CALIB_WAIT_FOR_AD_READY,
	TURB_SENS_CALIB_DONE,
	TURB_SENS_CALIB_EXIT,

/* Idle */
	TURB_SENS_IDLE_ENTRY,
	TURB_SENS_IDLE,
	TURB_SENS_IDLE_EXIT,


/* Acquisition */
	TURB_SENS_ACQ_ENTRY,
	TURB_SENS_ACQ_WAIT_FOR_AD_READY,
	TURB_SENS_ACQ_DONE,
	TURB_SENS_ACQ_EXIT,

/* End of states */
	TURB_SENS_STATE_NUM
}PM_TurbSensStateType;


/* Sensor Commands */
typedef enum ePM_TurbSensCmd
{
	TURB_SENS_CMD_IDLE = 0,
	TURB_SENS_CMD_CALIBRATION,
	TURB_SENS_CMD_ACQUISITION,
/*End of commands Id */
	TURB_SENS_CMD_NUM,
	TURB_SENS_CMD_UNDEFINED
}PM_TurbSensCmdType;

/* general */




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */



/* general */
/*
Delay between PWM and AD conversion to start in ms.
Its definition is affected by the size of the field 'delay' in PM_TurbSensLLISettingsType:
To define PM_TurbSensDelayType maximum allowed value of the field 'delay'.
*/
typedef	uint16_t PM_TurbSensDelayType;

/*
Flags: see 'flags' field documentation in PM_TurbSensLLISettingsType.
Its definition is affected by the size of the field 'flags' in PM_TurbSensLLISettingsType:
*/
typedef	uint8_t PM_TurbSensFlagsType;


/* flow table */
/* Events bitmap */
typedef 	uint8_t		PM_TurbSensFsmEventsBmpType;
/* Action bitmap*/
typedef 	uint16_t	PM_TurbSensFsmActionsBmpType;



typedef		IO_ErrorType (* pTurbSensCalibFsmStateProcessType)(PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);
typedef		struct sPM_TurbSensCalibFsmConfig
{
	const pTurbSensCalibFsmStateProcessType 	pProcess;
}PM_TurbSensCalibFsmConfigType;


typedef struct sPM_TurbSensFsm
{
	PM_TurbSensStateType						state;				/* state */
	pTurbSensCalibFsmStateProcessType 			pStateProc;			/* state procedure */
}PM_TurbSensFsmType;


typedef		struct sPM_TurbSensCmdToState
{
	PM_TurbSensStateType						state;
}PM_TurbSensCmdToStateType;


/* TurbiditySensor */
typedef struct sPM_TurbSens
{
	/*
	pwm channel
	*/
	IO_ChannelType								pwm;
	/*
	current pwm val [ns], for calibration only
	*/
	IO_TimingValueType							pwmVal;
	/*
	current pwm val [ns], for acquisition only
	*/
	IO_TimingValueType							pwmAcq;
	/*
	current tuning value for pwm val [ns], for calibration only
	*/
	IO_TimingValueType							pwmTuning;
	/*
	AD channel
	 */
	IO_ChannelType								ad;
	/*
	AD reference value for calibration
	*/
	ADC_ValueType								adref;
	/*
	delay before start acquisition
	*/
	PM_TurbSensDelayType						delay;
	/*
	LLI flags
	*/
	PM_TurbSensFlagsType						flags;
	/* current command */
	PM_TurbSensCmdType							cmd;
	/* next command */
	PM_TurbSensCmdType							cmdNext;
	/* fsm */
	PM_TurbSensFsmType							fsm;
	/*
	Interface
	*/
	bool8_t										acqReady;
	/* timer for timings: see delay */
	TOOLS_TIMER									timer;
}PM_TurbSensType;





/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
static IO_ErrorType	PM_TurbSens_InitPWM(IO_ChannelType chPWM, uint8_t options);
static IO_ErrorType	PM_TurbSens_DeInitPWM(IO_ChannelType chPWM);

/* Idle cmd  related state procedures prototypes */
static IO_ErrorType PM_TurbSens_IdleEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);

/* Calibration cmd related state procedures prototypes */
static IO_ErrorType PM_TurbSens_CalibEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);
static IO_ErrorType PM_TurbSens_CalibWait4ADSetup (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);
static IO_ErrorType PM_TurbSens_CalibWait4ADReady (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);
static IO_ErrorType PM_TurbSens_CalibDone (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);

/* Acquisition cmd related state procedures prototypes */
static IO_ErrorType PM_TurbSens_AcqEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);
static IO_ErrorType PM_TurbSens_AcqWait4ADReady (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut);

/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */
static PM_TurbSensType	TurbiditySensors;	/* Turbidity sensor */

/* State procedures
Each row reflects the value of PM_TurbSensStateType */
static const pTurbSensCalibFsmStateProcessType PM_TurbSensCalibFsmConfig[TURB_SENS_STATE_NUM]=
{
		/* Calibration */
		&PM_TurbSens_CalibEntry,			/* Calibration Entry state */
		&PM_TurbSens_CalibWait4ADSetup,		/* waits for AD setup */
		&PM_TurbSens_CalibWait4ADReady,		/* waits for AD ready */
		&PM_TurbSens_CalibDone,				/* Calibration done*/
		NULL,								/* Calibration Exit (unused)*/
		/* Idle */
		&PM_TurbSens_IdleEntry,				/* Idle Entry state */
		NULL,								/* Idle state (unused) */
		NULL,								/* Idle Exit (unused) */
		/* Acquisition */
		&PM_TurbSens_AcqEntry,				/* Acquisition Entry state */
		&PM_TurbSens_AcqWait4ADReady,		/* waits for AD ready */
		NULL,								/* Acquisition Done (unused) */
		NULL,								/* Acquisition Exit (unused)*/

};


/* Entryes by command.
Each row reflects the value of PM_TurbSensCmdType
*/
static const PM_TurbSensCmdToStateType PM_TurbSensCmdToState[TURB_SENS_CMD_NUM]=
{
		{TURB_SENS_IDLE_ENTRY},				/* Idle cmd entry state */
		{TURB_SENS_CALIB_ENTRY},			/* Calibration cmd entry state */
		{TURB_SENS_ACQ_ENTRY}				/* Acquisition cmd entry state */
};



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported Functions */

/**
Input manager initialization for turbidity sensor
\param [in]		chADC	AD channel
\param [in]		chPWM	PWM channel
\param [in]		pParam	pointer to Low Level Input (LLI) turidity sensor params structure PM_TurbSensLLISettingsType
*/
void PM_TurbiditySensor_SetParam( IO_ChannelType chADC, IO_ChannelType pinPWM, PM_TurbSensLLISettingsType* pParam )
{
	PM_TurbSensType* pSensor = &TurbiditySensors;
	IO_ModuleIOMapType* pPinStruct=0;
	IO_ChannelType tmrCh = 0xFF;

	pSensor->ad = chADC;
	/* AD Channel has to be configured Automatic.
	 * Since setting file may have it not configured as is, force it
	 */
	LIB_ADC_ForceAutoRead(LIB_ADC_ADChVsAnalogInputParamIndex(chADC), TRUE);

	pSensor->delay = SF_TURBSENS_GET_MAXDELAY_RAW(pParam)*20;
	pSensor->flags = SF_TURBSENS_GET_FLAGS(pParam);

	pSensor->cmd = TURB_SENS_CMD_UNDEFINED;
	pSensor->cmdNext = TURB_SENS_CMD_IDLE;

	pSensor->fsm.pStateProc = NULL;
	pSensor->fsm.state=0xFF;

	pSensor->acqReady=0;

	pPinStruct =  (IO_ModuleIOMapType*) OCDIO_GetIOMapItem( pinPWM );

	if ( (pPinStruct ) && (OCTPM_GetChannelSync(pPinStruct,&tmrCh) == (IO_ErrorType)IO_E_OK ))
	{
		if ( PM_TurbSens_InitPWM(tmrCh, pSensor->flags & 1) == (IO_ErrorType)IO_E_OK )
		{
			pSensor->acqReady |= 4;
			pSensor->pwm = tmrCh;
		}
	}
}

/**
Starts calibration command
\param [in]		val	AD reference value
*/
void PM_TurbiditySensor_StartCalib(ADC_ValueType val)
{
	PM_TurbSensType* pSensor = &TurbiditySensors;

	pSensor->adref = val;
	pSensor->cmdNext = TURB_SENS_CMD_CALIBRATION;
}

/**
Starts acquisition command
\param [in]		val	PWM value
*/
void PM_TurbiditySensor_StartAcq(IO_TimingValueType val)
{
	PM_TurbSensType* pSensor = &TurbiditySensors;

	pSensor->pwmAcq = val;
	pSensor->cmdNext = TURB_SENS_CMD_ACQUISITION;
}

/**
returns whether acquisition is ready
\param [in]		val	AD reference value
*/
bool8_t PM_TurbiditySensor_AcqIsReady(void)
{
	PM_TurbSensType* pSensor = &TurbiditySensors;

	return (pSensor->acqReady & 1)?1:0;
}

/**
returns the period of the PWM signal in ns
\param [in]		val	AD reference value
*/
IO_TimingValueType PM_TurbiditySensor_GetPWMPeriod(void)
{
	return TURB_SENS_PWM_PERIOD;
}
/**
returns whether acquisition is ready
\param [out]		pVal	pointer to  PWM value, updated if returned value is TRUE

\return TRUE	calibration ready
\return FALSE	calibration not ready
*/
bool8_t PM_TurbiditySensor_GetCalibration(IO_TimingValueType* pVal)
{
	PM_TurbSensType* pSensor = &TurbiditySensors;
	if(pVal)
	{
		*pVal = pSensor->pwmVal;
	}

	return (pSensor->acqReady & 2)? TRUE : FALSE;
}

/**
Turbidity sensor manager
*/
void PM_TurbiditySensor_Manager(void)
{
	PM_TurbSensType* pSensor = &TurbiditySensors;
	PM_TurbSensFsmEventsBmpType in = 0;
	PM_TurbSensFsmActionsBmpType out = 0;
	IO_ErrorType	error = IO_E_FAIL;

	PM_TurbSensStateType	state;

	ADC_ValueType	adVal = 0;

	if(pSensor->acqReady & 4)
	{

		if(pSensor->cmd != pSensor->cmdNext)
		{
			pSensor->cmd = pSensor->cmdNext;
			pSensor->fsm.state = PM_TurbSensCmdToState[pSensor->cmd].state;

			if((pSensor->cmd == TURB_SENS_CMD_ACQUISITION) || pSensor->cmd == TURB_SENS_CMD_CALIBRATION)
			{
				pSensor->acqReady &= 4;
			}
		}

		state = pSensor->fsm.state;
		pSensor->fsm.pStateProc = PM_TurbSensCalibFsmConfig[state];

		/*	evaluating inputs	*/

		if( LIB_ADC_IsAnalogReady( LIB_ADC_ADChVsAnalogInputParamIndex(pSensor->ad) ))
		{
			in |= BFMASK(TURB_SENS_AD_READY, 1);
			adVal = LIB_ADC_GetAnalogValue(LIB_ADC_AnalogChRamPos(pSensor->ad));
		}

		if(Tools_IsTimerElapsed ( &(pSensor->timer) , (OS_TIME)pSensor->delay))
		{
			in |= BFMASK(TURB_SENS_TIME_TO_SETUP_AD, 1);
		}

		if(!pSensor->pwmTuning)
		{
			in |= BFMASK(TURB_SENS_CORRECTION_EMPTY, 1);
		}


		/* state process */
		if(pSensor->fsm.pStateProc)
		{
			error = (pSensor->fsm.pStateProc) (&state, in, &out);
		}

		/*	actions	*/
		if(out & BFMASK(TURB_SENS_INIT_PWM_BIT, 1))
		{
			adVal=0;

			pSensor->pwmVal = 0;


			/* whole range initialization: pwmAcq recycled to optimize demand of RAM */
			pSensor->pwmAcq = TURB_SENS_PWM_PERIOD;
			/* tuning value initialization */
			pSensor->pwmTuning = TURB_SENS_PWM_PERIOD/2 ;
		}

		if(out & BFMASK(TURB_SENS_EVAL_PWM_BIT, 1))
		{
			/* eval PWM calib val */

			if(adVal < pSensor->adref)
			{
				pSensor->pwmVal += pSensor->pwmTuning;
			}
			else
			{
				pSensor->pwmVal -= pSensor->pwmTuning;
			}

			/* eval next tuning value taking into account the remaining range.
			available range is the current one minus current tuning: this takes into account
			the remainder of the division by two */
			pSensor->pwmAcq -= pSensor->pwmTuning;
			/* next tuning is the remaining range divided by 2 */
			pSensor->pwmTuning = pSensor->pwmAcq >> 1;

			/* enable PWM with Ton @ pSensor->pwmVal;  */
			if( PM_TurbSens_InitPWM(pSensor->pwm, pSensor->flags & 1) == (IO_ErrorType)IO_E_OK)
			{
				OCPWM_SetSync(pSensor->pwm, pSensor->pwmVal);
			}

		}


		if(out & BFMASK(TURB_SENS_BEFORE_SETUP_AD_BIT, 1))
		{
			/* start timeout before read AD */
			Tools_StartTimer ( &(pSensor->timer) );
		}

		if(out & BFMASK(TURB_SENS_SETUP_AD_BIT, 1))
		{
			 LIB_ADC_InitMeasureForCh ( pSensor->ad );
		}

		if(out & BFMASK(TURB_SENS_DONE_BIT, 1))
		{
			pSensor->cmd = TURB_SENS_CMD_IDLE;
			OCPWM_DeInitSync(pSensor->pwm);
			/*OCPWM_SetSync(pSensor->pwm, 0);*/
			pSensor->acqReady |= 2;
		}

		if(out & BFMASK(TURB_SENS_ACQ_PWM_INIT, 1))
		{
			/* enable PWM with Ton @ pSensor->pwmAcq;  */

			if( PM_TurbSens_InitPWM(pSensor->pwm, pSensor->flags & 1) == (IO_ErrorType)IO_E_OK)
			{
				OCPWM_SetSync(pSensor->pwm, pSensor->pwmAcq);
			}
		}

		if(out & BFMASK(TURB_SENS_ACQ_DONE_BIT, 1))
		{
			pSensor->cmd = TURB_SENS_CMD_IDLE;
			pSensor->acqReady |= 1;
			OCPWM_DeInitSync(pSensor->pwm);
			/*OCPWM_SetSync(pSensor->pwm, 0);*/
		}

		pSensor->fsm.state = state; /* apply */

	}
}


/* Local Functions */
static IO_ErrorType	PM_TurbSens_InitPWM(IO_ChannelType chPWM, uint8_t options)
{
	IO_ErrorType err = (IO_ErrorType)IO_E_FAIL;
	IO_PWM_CfgType PWM_ChCfg={0};

	PWM_ChCfg.PWM_Id=chPWM;
	PWM_ChCfg.dfltPeriod=TURB_SENS_PWM_PERIOD;  /*[ns] */
	PWM_ChCfg.inverted = options & 1;

	if (OCPWM_InitSync(&PWM_ChCfg) == IO_E_OK)
	{
		err = (IO_ErrorType)IO_E_OK;
	}

	return err;

}


static IO_ErrorType	PM_TurbSens_DeInitPWM(IO_ChannelType chPWM)
{
	IO_ErrorType err = (IO_ErrorType)IO_E_FAIL;

	if (OCPWM_DeInitSync(chPWM) == IO_E_OK)
	{
		err = (IO_ErrorType)IO_E_OK;
	}

	return err;
}

/**
Entry point of idle FSM
It goes immediately into TURB_SENS_IDLE without actions

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_IdleEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_IDLE;
	if(pState)
	{
		*pState = state;
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}

/**
Entry point of Calibration command FSM
It goes immediately into TURB_SENS_CALIB_WAIT_FOR_AD_SETUP.
Actions:
- PWM initialization
- timer started up

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_CalibEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_CALIB_WAIT_FOR_AD_SETUP;
	if(pState)
	{
		/* TODO: PWM Init */
		*pOut |= BFMASK(TURB_SENS_BEFORE_SETUP_AD_BIT, 1)|BFMASK(TURB_SENS_INIT_PWM_BIT, 1)|BFMASK(TURB_SENS_EVAL_PWM_BIT, 1) ;

		*pState = state;
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}

/**
Waiting for AD setup
it it's time to setup AD it goes into TURB_SENS_CALIB_WAIT_FOR_AD_READY to wait for AD acquisition.
Actions:
- setup AD channel

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_CalibWait4ADSetup (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_CALIB_WAIT_FOR_AD_SETUP;

	if(pState)
	{
		if(in & (BFMASK(TURB_SENS_TIME_TO_SETUP_AD, 1)))
		{
			state = TURB_SENS_CALIB_WAIT_FOR_AD_READY;
			*pOut |= BFMASK(TURB_SENS_SETUP_AD_BIT, 1);
		}

		*pState = state;

		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}

/**
Waiting for AD ready.
If not available tuning and AD ready then it goes into TURB_SENS_CALIB_DONE, without doing any action.
If AD ready then it goes into TURB_SENS_CALIB_WAIT_FOR_AD_SETUP
Actions:
- timer started up
- update tuning value

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_CalibWait4ADReady (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_CALIB_WAIT_FOR_AD_READY;
	PM_TurbSensFsmEventsBmpType in2 = in & (BFMASK(TURB_SENS_CORRECTION_EMPTY, 1) | BFMASK(TURB_SENS_AD_READY, 1));

	if(pState)
	{
		if(in2 == (BFMASK(TURB_SENS_CORRECTION_EMPTY, 1) | BFMASK(TURB_SENS_AD_READY, 1)))
		{
			state = TURB_SENS_CALIB_DONE;
		}
		else if (in2 == BFMASK(TURB_SENS_AD_READY, 1))
		{
			*pOut |= BFMASK(TURB_SENS_BEFORE_SETUP_AD_BIT, 1)|BFMASK(TURB_SENS_EVAL_PWM_BIT, 1);
			state = TURB_SENS_CALIB_WAIT_FOR_AD_SETUP;
		}
		*pState = state;
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}

/**
Calibration done
Exit pseudo state
Actions:
- Calibration done

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_CalibDone (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	if(pState)
	{
		*pOut |= BFMASK(TURB_SENS_DONE_BIT, 1);
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}


/**
Acquisition Entry point
it goes into TURB_SENS_ACQ_WAIT_FOR_AD_READY
Actions:
-	timer start up
-	AD setun
-	PWM init

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_AcqEntry (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_ACQ_WAIT_FOR_AD_READY;
	if(pState)
	{
		/* TODO: PWM Init */
		*pOut |= BFMASK(TURB_SENS_BEFORE_SETUP_AD_BIT, 1)|BFMASK(TURB_SENS_SETUP_AD_BIT, 1)|BFMASK(TURB_SENS_ACQ_PWM_INIT, 1);

		*pState = state;
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}

/**
Waiting for AD ready
If AD ready it goes into TURB_SENS_ACQ_DONE
Actions:
-	acquisition done

\param [in,out] pState	pointer to current state
\param [in]		input flags
\param [out]	pointer to actions
*/
static IO_ErrorType PM_TurbSens_AcqWait4ADReady (PM_TurbSensStateType* pState, PM_TurbSensFsmEventsBmpType in, PM_TurbSensFsmActionsBmpType* pOut)
{
	IO_ErrorType error = IO_E_FAIL;
	PM_TurbSensStateType	state = TURB_SENS_ACQ_WAIT_FOR_AD_READY;

	if(pState)
	{
		if (in & BFMASK(TURB_SENS_AD_READY, 1))
		{
			*pOut |= BFMASK(TURB_SENS_ACQ_DONE_BIT, 1);
			state = TURB_SENS_ACQ_DONE;
		}
		*pState = state;
		error= (IO_ErrorType)IO_E_OK;
	}
	return error;
}
#endif /* defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT) */
/* *********** */
/* End of file */
/* *********** */
