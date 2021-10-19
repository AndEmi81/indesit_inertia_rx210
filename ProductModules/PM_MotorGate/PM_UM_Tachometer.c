/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\file       PM_UM_Tacheometer.c
\brief      Tachometer Driver for Universal Motor
\ingroup    PM_MotorGate
\author     Marco Sperandini
*/
/**
\addtogroup PM_MotorGate
*@{*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_PM_MOTORGATE)
#include "String.h"
#include "OCTPM_drv.h"
#include "OCDIO_drv.h"
#include "PM_UM_Tachometer.h"
#include "LIB_Trace.h"
#include "LIB_ClassBVar.h"


#include "PM_UM_UniversalMotorExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define TACH_WAIT_FIRST_SAMPLE	0
#define TACH_VAL_READY		1
#define TACH_ALREADY_READ	2

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct
{
    uint8_t isPresent:    1;				/* TRUE/FALSE */
    uint8_t enabled:      1;
    uint8_t faultCode:    2;
    uint8_t tackState:    2;				/* State of Capture */
    uint8_t typeMeasure:  1;				/* tipologia misura in continua o one shot */
    
    IO_PWD_CfgType		PwdCfg;         	/* Channel of the tach ...*/
    IO_TimingValueType 	TachTimer_lastCapt;	/* # of tks last capture */
    uint32_t	        TachT;				/* tacho value in us */
} TachCtrlVar_t;

   
/* ***************** */
/* Imported variables */
/* ***************** */
 
/* **************** */
/* Local variables */
/* **************** */
static TachCtrlVar_t TachCtrlVar;


/* ***************** */
/* Exported function */
/* ***************** */

/**
 * PM_UM_Tacho_GetValue
 * returns the period of the signal of the tachometer [us]
 * */
uint32_t PM_UM_Tacho_GetValue(void)
{
	uint32_t retval = 0;
	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);

	retval= TachCtrlVar.TachT;

    SAFETY_DATA_UPDATE(TachCtrlVar);
    ExitCriticalSection();

    return retval;
}

/**
 * PM_UM_Tacho_SetNewValue
 * \brief Set value
 * \param [in]  value of the measure
 */
void PM_UM_Tacho_SetNewValue(uint32_t val)
{
	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);
    TachCtrlVar.TachT = val;
    SAFETY_DATA_UPDATE(TachCtrlVar);
    ExitCriticalSection();
}

/**
 * PM_UM_Tacho_GetNewValue
 * \brief Set value
 * \param [out]  tacho pointer to the value of the measure in us
 * \param [in]  update  TRUE: if measure is ready, make it as read. FALSE: keep status
 * \note its use in ISR is allowed
 */
uint8_t PM_UM_Tacho_GetNewValue(uint32_t *tacho, bool_t update)
{
	uint8_t retval = TACH_VALUE_KO;

	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);

    *tacho = TachCtrlVar.TachT;
    
    if( TachCtrlVar.tackState == TACH_VAL_READY )
    {
        if( update )
        {
        	TachCtrlVar.tackState = TACH_ALREADY_READ;
        }

        SAFETY_DATA_UPDATE(TachCtrlVar);
        retval = TACH_VALUE_OK;
    }
    
    ExitCriticalSection();
    return retval;
}

/** ISR driver's Callback*/
IO_ErrorType PM_UM_Tacho_InputCaptureManager(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{    
	IO_PWD_MeasureType measure;
    /* Check if notification status is good */

	if ((notifVal != IO_N_COMPLETED) || (error != IO_E_OK))
    {    
		return IO_E_UNKNOWN_MODE;
    }
	SAFETY_DATA_CHECK(TachCtrlVar);
	/* update capture value in ns */
	OCPWD_GetAllSync(TachCtrlVar.PwdCfg.PWD_Id, &measure);

	if( measure.period < 3000000)
	{
		TachCtrlVar.typeMeasure = PM_UM_TACHO_ONE_SHOT;

		if( (measure.period > (7*TachCtrlVar.TachTimer_lastCapt)/4) && (TachCtrlVar.TachTimer_lastCapt))
		{
			/* missed at least one pulse , don't update */
		}
		else
		{
			OCPWD_StopSampleSync(TachCtrlVar.PwdCfg.PWD_Id);
			TachCtrlVar.enabled = FALSE;

			TachCtrlVar.TachTimer_lastCapt = measure.period;
		}
	}
	else
	{
		TachCtrlVar.TachTimer_lastCapt = measure.period;
	}

	TachCtrlVar.TachT = TachCtrlVar.TachTimer_lastCapt/1000;
	TachCtrlVar.tackState = TACH_VAL_READY;

	SAFETY_DATA_UPDATE(TachCtrlVar);

	return IO_E_OK;
}


void PM_UM_Tacho_Reset(void)
{
	SAFETY_REGION_START();
	memset(&TachCtrlVar,0,sizeof(TachCtrlVar));

	TachCtrlVar.isPresent = FALSE;
	TachCtrlVar.PwdCfg.PWD_Id    = 0xFF;
	TachCtrlVar.faultCode = 1;  /* TODO: codificare errori */

	SAFETY_DATA_UPDATE(TachCtrlVar);
	SAFETY_REGION_END();
}

/**
Inizializzazione Modulo Tachimetrica.
Chiamata al rese freddo e caldo in InputInitialization( unsigned char ResetType)

\param  [in] pin, pin cui � connessa la tachimetrica
\param  [in,out] TachCtrlVar, type TachCtrlVar_t global
\return 	none
\note its use in ISR is NOT allowed */
void PM_UM_Tacho_Init(uint8_t pin)
{
	IO_ChannelType tmpChannel;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(TachCtrlVar);
	/* Checks if pin module is a Timer channel */

	if (OCTPM_GetChannelSync(OCDIO_GetIOMapItem(pin), &tmpChannel) == (IO_ErrorType) IO_E_OK )
	{
		/* OK it is; prepare configuration */
		TachCtrlVar.PwdCfg.PWD_Id = tmpChannel;
		TachCtrlVar.PwdCfg.inverted = 0; /* rising edge */

		/* Try initialization of PWD Driver */
		if ( OCPWD_InitSync(&TachCtrlVar.PwdCfg) == (IO_ErrorType) IO_E_OK)
		{
			TachCtrlVar.isPresent = TRUE;
			TachCtrlVar.faultCode = 0;
			TachCtrlVar.typeMeasure = PM_UM_TACHO_ONE_SHOT;

			OCPWD_SetTonMeasure(TachCtrlVar.PwdCfg.PWD_Id, FALSE); /* disable Ton Measure*/

#if defined(CONFIG_PM_UM_TACHO_PWD_FILTER_TIME)
			OCPWD_SetFilterTime(TachCtrlVar.PwdCfg.PWD_Id, CONFIG_PM_UM_TACHO_PWD_FILTER_TIME);
#endif

#if defined(CONFIG_PM_UM_TACHO_DIGITAL_FILTER_FREQ)
			OCDIO_SetIODigitalFilterFrequencySync(pin, CONFIG_PM_UM_TACHO_DIGITAL_FILTER_FREQ);
			OCDIO_SetIODigitalFilterPinSync(pin);
#endif

			TachCtrlVar.tackState = TACH_WAIT_FIRST_SAMPLE;
			TachCtrlVar.enabled = TRUE;
			OCPWD_StartSampleSync(TachCtrlVar.PwdCfg.PWD_Id, PM_UM_Tacho_InputCaptureManager);
		}
    }
    SAFETY_DATA_UPDATE(TachCtrlVar);
    SAFETY_REGION_END();
}

/**  Enables Tach reading
\note its use in ISR is allowed */
void PM_UM_Tacho_Enable(void)
{
	IO_ErrorType res= IO_E_OK;

	/* Start */
	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);

	if( TachCtrlVar.isPresent == TRUE )
	{
		if((TachCtrlVar.enabled == FALSE) || (TachCtrlVar.typeMeasure == PM_UM_TACHO_ONE_SHOT))
		{
			TachCtrlVar.tackState = TACH_WAIT_FIRST_SAMPLE;
			res = OCPWD_StartSampleSync(TachCtrlVar.PwdCfg.PWD_Id, PM_UM_Tacho_InputCaptureManager);
		}

		/* Enable Frequency meter channel capture */
		if (res == IO_E_OK)
		{
			TachCtrlVar.enabled = TRUE;
		}
	}

	SAFETY_DATA_UPDATE(TachCtrlVar);
	ExitCriticalSection();
}

/**  Disables Tach reading
\note its use in ISR is allowed */
void PM_UM_Tacho_Disable(void) 
{
	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);
	OCPWD_StopSampleSync(TachCtrlVar.PwdCfg.PWD_Id);
	TachCtrlVar.enabled = FALSE;
	SAFETY_DATA_UPDATE(TachCtrlVar);
	ExitCriticalSection();
}

/**  Sets Tach reading mode
\note its use in ISR is allowed */
void PM_UM_Tacho_SetMode( PM_UM_Tachometer_SetAcqMode howMeasure )
{
	EnterCriticalSection();
	SAFETY_DATA_CHECK(TachCtrlVar);

	TachCtrlVar.typeMeasure = howMeasure;

	SAFETY_DATA_UPDATE(TachCtrlVar);
	ExitCriticalSection();
}

PM_UM_Tachometer_SetAcqMode PM_UM_Tacho_GetMode(void)
{
	return TachCtrlVar.typeMeasure;
}
#endif /* CONFIG_PM_MOTORGATE */

/* *********** */
/* End of file */
/* *********** */
/**@}*/
