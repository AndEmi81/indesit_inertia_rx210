/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
			Implementation of all functions necessary to achieve the aim.
\n
\file		InputManager.c
\ingroup	InputsManager
\date		Jun 8, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Jun 8, 2010
\n		Stefano Ughi
\n		changes
\n 	    - Creation
\n
\n
\n 		01.00.01
\n		Nov 15, 2010
\n		Stefano Ughi
\n		changes
\n 		- void LIB_IOM_InputInitialization( uint8_t resetType )
\n		  added error handling in input pin setup
\n        added new call to feedbacks's init. if defined PLATFORM2_FEEDBACK
\n
\n 		01.00.02
\n		Mar 30, 2011
\n		Stefano Ughi
\n		changes
\n 		- added #include "ProductModules_Cfg.h"
\n      - added define WILD_CARD to use with PLATFORM2_SUPPORT define
\n
\n etc.
*/
#define MODULE_BODY_LIB_IOMANAGER
/* ******** */
/* Includes */
/* ******** */

#include "Config.h"

#include "LIB_Utilities.h"
#include "InputManager.h"
#include "LIB_Error.h"
/*#include "InputManagerExp.h"*/

#include "IOSharing.h"
#include "DigitalInputs.h"
#include "Analog.h"
#include "OCDIO_drv.h"
#include "LIB_ZeroCrossing.h"
#if defined(PLATFORM1_MODULES_USED)
#include "Feedback.h"
#else
#include "Feedbacks.h"
#endif
/*lint -e537*/
#include "ProductModules_Cfg.h"
/*lint +e537*/
#ifdef CONFIG_PM_CONDUCTIVITY_SENSOR
#include "PM_ConductivitySensor.h"
#endif /* CONFIG_PM_CONDUCTIVITY_SENSOR */

#ifdef CONFIG_PM_LINEARPRESSURE_SENSOR
#include "PM_LinearPressureSensor.h"
#endif /* CONFIG_PM_LINEARPRESSURESENSOR */

#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
#include "PM_AnalogFeedback.h"
#endif /* CONFIG_PM_ANALOG_FEEDBACK */

#ifdef CONFIG_PM_MOTORGATE
#include "PM_UM_UniversalMotor.h"
#include "PM_UM_Tachometer.h"
#endif

#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
#include "PM_FrequencyFeedbacks.h"
#endif /* defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT) */

#if defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
#include "PM_TurbiditySensor.h"
#endif /* defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT) */
#if defined(CONFIG_PM_PULSECOUNTER)
#include "PM_PulseCounter.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define EEP_LOWLEVELINPUT_TABPTR_OFFSET		0		/* L.L.I. displacement vs PTR_IO_CONFIG_DATA */
#define EEP_PIN_TABPTR_OFFSET    			4		/* Pins displacement vs PTR_IO_CONFIG_DATA */

#define LLI_TABLE_ITEM_SIZE_IN_BYTES 		4		/* Size in bytes of a single item inside L.L.I. table */
/* Input Maps Field Access */
#define READ_TYPE(pByte)                    (*(pByte))      /* Input Read Type */
#define INPUT_PIN_TAB_OFFSET(pByte)         (*(pByte+1))    /* Pin Tab Offset Field vs PTR_IO_DEVICE_DATA */
#define INPUT_TYPE_PARAMS_OFFSET(pByte)     (*(pByte+2))    /* Input Type Params Offset vs PTR_IO_DEVICE_DATA */
#define INPUT_RESULT_POS(pByte)             (*(pByte+3))    /* Where Input Result is stored */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */


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
\n Inputs Initialization.
\n This function is called in ResetModuleManager() if SystemMode==SYSTEM_NORMAL_MODE
\n regardless cold or warm reset.

\param  [in] resetType if !=0 -> cold reset otherwise warm reset.

\author 	Stefano Ughi
\date		10/06/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void LIB_IOM_InputInitialization( uint8_t resetType )
{
	struct Loader lowLevelInputTabPtr;
	struct Loader pinTabPtr;
	struct Loader lowLevelParametersPtr;
	IO_ErrorType err;
	uint8_t *pByte;          /* local pointer used to scan inputs */
	uint8_t pin;
	uint8_t readType;
	uint8_t activeSense;

	uint8_t channelAD;


	if ( 0 != BasicLoader(SF_PTR_IO_CONFIG, EEP_LOWLEVELINPUT_TABPTR_OFFSET, &lowLevelInputTabPtr) )
	{
		PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_1);
		return; /* Serious Error */
	}
	if ( 0 != BasicLoader(SF_PTR_IO_CONFIG, EEP_PIN_TABPTR_OFFSET, &pinTabPtr) )
	{
		PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_2);
		return; /* Serious Error */
	}
	LIB_ADC_UnregisterAllADch();
	/* For All Inputs in Setting File */
	for ( pByte = lowLevelInputTabPtr.Data;
	        pByte < (lowLevelInputTabPtr.Data + (lowLevelInputTabPtr.DataLenght*2));
	        pByte += LLI_TABLE_ITEM_SIZE_IN_BYTES )
	{
		/* pin (refer to Linkage M.C.U. Board) linked with Input
		pin = value inserted in Editor minus 1 */
		pin = *(pinTabPtr.Data + INPUT_PIN_TAB_OFFSET(pByte));
		if ( pin >= IO_MODULE_MAP_SIZE )
		{
			PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_3);
			return;  /* Serious Error */
		}


		/* Set the pin as input */
		err = OCDIO_SetPinDirValueSync(pin,0);
		if ( (IO_ErrorType)IO_E_UNKNOWN_MODE == err )
		{
			/* Serious Error */
			PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_6);
		}
		else if ( (IO_ErrorType)IO_E_INVALID_VALUE == err )
		{
			/* Serious Error */
			PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_7);
		}
		else
		{
			/* do nothing */
		}
		/* get pointer to low level input parameters */
		if ( 0 != BasicLoader( SF_PTR_IO_DEVICE, INPUT_TYPE_PARAMS_OFFSET(pByte), &lowLevelParametersPtr) )
		{
			PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_4);
			return;  /* Serious Error */
		}


		readType = READ_TYPE(pByte);
		switch (readType)
		{

		case I2C_I:

			/* Nothing to do up to now ..... */

			break;


		case ZEROCROSSING_I:
#if defined(CONFIG_LIB_ZC_ICU_OVR_SETTING)
		{
			LIB_ZC_RegisterICUFromPin(pin);
		}
#endif

			break;
	
#ifdef __FREQFEEDBACKCHK_H
		case FREQFEEDBACK_I:
			if ( ((freqFeedbackLLIparams_t*)lowLevelParametersPtr.Data)->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}
			activeSense = ((((freqFeedbackLLIparams_t*)lowLevelParametersPtr.Data)->bit.inverted)?1:0);
			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);
			FFC_Enable(pin,(*lowLevelParametersPtr.Data & 0x7F), INPUT_RESULT_POS(pByte),30);

			break;
#elif defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
		case FREQFEEDBACK_I:

			if( IO_E_OK != PM_FrequencyFeedbacks_Configure(pin,(freqFeedbackLLIparams_t*)((void*)lowLevelParametersPtr.Data), INPUT_RESULT_POS(pByte))){
				PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_8);
			}

			break;
#endif /*CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT */

#if defined(CONFIG_PM_MOTORGATE) /* ex __TACHIMETRICA_H */
		case TACHO_I:
			if ( ((tachoLLIparams_t*)lowLevelParametersPtr.Data)->bit.pullUp)
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}
			LIB_IOM_Set_IODir_ActiveSense(0,pin);

			PM_UM_Tacho_Init(pin);

			break;
#endif /*defined(CONFIG_PM_MOTORGATE) ex __TACHIMETRICA_H */

/* Linear Pressure Sensor on Plaform 1 */
#ifdef __PRESSOSTATOLINEARE_H
		case PRESSLIN_I:
			if ( ((linearPressureSensLLIparams_t*)lowLevelParametersPtr.Data)->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}

			LIB_IOM_Set_IODir_ActiveSense(0,pin);

			PSL_Init(pin,*(lowLevelParametersPtr.Data+2));
			PSL_Enable(lowLevelParametersPtr.Data,1);

			break;
#elif defined(CONFIG_PM_LINEARPRESSURE_SENSOR)
		case PRESSLIN_I:
			if ( ((linearPressureSensLLIparams_t*)((void*)lowLevelParametersPtr.Data))->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}

			LIB_IOM_Set_IODir_ActiveSense(0,pin);

			PM_LPS_Init(pin, lowLevelParametersPtr.Data);
			break;
#endif /*CONFIG_PM_LINEARPRESSURESENSOR */

#ifdef __PM_PULSECOUNTER_H
		case PULSECNTR_I:
			if ( ((pulseCounterLLIparams_t*)lowLevelParametersPtr.Data)->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}
			LIB_IOM_Set_IODir_ActiveSense(0,pin);
			(void)PM_PulseCounter_Init(pin,lowLevelParametersPtr.Data);
			(void)PM_PulseCounter_SetChannelStatus ( PULSECOUNTER_ENABLE );
			break;
#endif /* __PM_PULSECOUNTER_H */

		case ANALOG_I:
#if defined(__TURBIDITY_SENSOR_H) || defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
		case TURBIDITY_I:
#endif
#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
		case ANALOGFEEDBACK_I:
#endif /* CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT */

			/* fixed the linkage's pin return the AD channel linked with that pin */

			if ( OCADC_GetADchannelSync(OCDIO_GetIOMapItem(pin),&channelAD) != (IO_ErrorType)IO_E_OK )
			{
				PLT_ERROR_FATAL(PLT_ERROR_INPUT_MNGR_5);
				return; /* Serious Error !*/
			}

			/* Function Parameters: Param 1: channelAD -> A/D channel
								    Param 2: Ram Position Conversion Result
								    Param 3: lowLevelParametersPtr.Data -> pointer to Setting File Params
			*/

			LIB_ADC_RegisterADchannel( channelAD, INPUT_RESULT_POS(pByte), lowLevelParametersPtr.Data );

#ifdef __TURBIDITY_SENSOR_H

			if ( readType == TURBIDITY_I )
			{
				uint8_t pinPWMCh = *(pinTabPtr.Data + INPUT_PIN_TAB_OFFSET(pByte) + 1 );
				(void)TurbiditySensor_Init( channelAD, pinPWMCh, lowLevelParametersPtr.Data );
			}

#endif /* __TURBIDITY_SENSOR_H */
#if defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT)
			if ( readType == TURBIDITY_I )
			{
				uint8_t pinPWMCh = *(pinTabPtr.Data + INPUT_PIN_TAB_OFFSET(pByte) + 1 );
				PM_TurbiditySensor_SetParam( channelAD, pinPWMCh, (PM_TurbSensLLISettingsType*)lowLevelParametersPtr.Data );
			}
#endif /* defined(CONFIG_PM_TURBIDITY_SENSOR_SUPPORT) */

#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT

			if ( readType == ANALOGFEEDBACK_I )
			{
#if defined (PLATFORM2_SUPPORT) && ( defined(DR_DRYER) || defined (DW_DEA700) || defined(WILD_CARD) )
				(void)PM_AnalogFeedback_Setup( channelAD, lowLevelParametersPtr.Data );
#else
				(void)AnalogFeedback_Setup( channelAD, lowLevelParametersPtr.Data );
#endif
			}

#endif /* CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT */

			break;

#ifdef __TIME_COUNTER_H
		case TIME_I:
			/* If not to add to TimeInputs[] Array */
			if( !(((timeLLIparams_t *)lowLevelParametersPtr.Data) -> bit.automatic) )
			{
				break;
			}

			if (resetType)
			{
				/*                     POS                  NSAMPLES                          DELTA                  MULCONF  ENABLED  READTYPE  */
				if (InsertTimeInput( INPUT_RESULT_POS(pByte),(*lowLevelParametersPtr.Data & 0x7F),(*(lowLevelParametersPtr.Data+1) & 0x78) >> 3,0,1,0))
				{
					/* errore !!! */
				}
			}

			break;



		case TIMEMULT_I:
			/* If not to add to TimeInputs[] Array */
			if( !(((timeMpxLLIparams_t *)lowLevelParametersPtr.Data) -> bit.automatic) )
			{
				break;
			}
			if (resetType)
			{
				/* POS       NSAMPLES              DELTA                        MULCONF     ENABLED  READTYPE	  */
				if (InsertTimeInput( INPUT_RESULT_POS(pByte),(*lowLevelParametersPtr.Data & 0x7F),(*(lowLevelParametersPtr.Data+1) & 0x78) >> 3,(*(lowLevelParametersPtr.Data+1) & 0x07),1,1))
				{

					/* errore !!! */
				}
			}
			/* per il TIME semplice togliere il selettore mulsel */
			break;

#endif /*__TIME_COUNTER_H */

#ifdef __PUSH_H

		case KEY_I:

			/* No pullup */
			activeSense = ((((keyLLIparams_t*)lowLevelParametersPtr.Data)->bit.inverted)?1:0);
			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);

			if (PUSH_InsertDigInput( INPUT_RESULT_POS(pByte), pin, 0, 0, (*lowLevelParametersPtr.Data & 0x7F)))
			{
				/* errore !!! */
			}

			break;

#endif /* __PUSH_H */

		case DIGITAL_I:

			if ( ((digitalLLIparams_t*)((void*)lowLevelParametersPtr.Data))->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}
			activeSense = (uint8_t) ((((digitalLLIparams_t*)((void*)lowLevelParametersPtr.Data))->bit.inverted) ? 1 : 0);
			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);


			if (resetType)		/* if reset is cold */
			{
				/* param 1: Ram Position Conversion's Result */
				/* param 2: pin -> pin linkage module M.C.U. - 1 */
				/* param 3: multiplexed or not multiplexed input */
				/* param 4: pointer to config. params  */

				LIB_DIO_InsertDigInput( INPUT_RESULT_POS(pByte), pin, NO_MPX_INPUT, lowLevelParametersPtr.Data);

			}

			break;
#if defined( __CONDUCTIVITY_SENSOR_H_ )

		case CONDUCTIVITYSENSOR_I:
			/*longfill conductivity sensor and conductivity sensor*/
			if ( ((conductivitySensorLLI_t*)lowLevelParametersPtr.Data)->bit.pullUp)
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}

			activeSense = ((((conductivitySensorLLI_t*)lowLevelParametersPtr.Data)->bit.inverted)?1:0);

			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);
			/*Tout, Tmin, Tmax non used*/
			CSN_Init(pin,  *(lowLevelParametersPtr.Data+9), *(lowLevelParametersPtr.Data+8), *(lowLevelParametersPtr.Data+11) & 0x07, (((uint16_t)(*((uint16_t*)(lowLevelParametersPtr.Data) + (2))))&0x7FFF)  );

			break;

#elif defined( CONFIG_PM_CONDUCTIVITY_SENSOR )

		case CONDUCTIVITYSENSOR_I:
			/*longfill conductivity sensor and conductivity sensor*/
			if ( ((conductivitySensorLLI_t*)lowLevelParametersPtr.Data)->bit.pullUp)
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}

			activeSense = ((((conductivitySensorLLI_t*)lowLevelParametersPtr.Data)->bit.inverted)?1:0);

			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);
			/*Tout, Tmin, Tmax non used*/		
			PM_CSN_Init(pin,  lowLevelParametersPtr.Data );

			break;

#endif /*__CONDUCTIVITY_SENSOR_2_H_*/

#ifdef __MULTI_H

		case DIGMULT_I:

			if (resetType)
			{
				/* param 1: Ram Position Conversion's Result */
				/* param 2: pin -> pin linkage module M.C.U. - 1 */
				/* param 3: multiplexed or not multiplexed input */
				/* param 4: pointer to config. params  */

				LIB_DIO_InsertDigInput( INPUT_RESULT_POS(pByte), pin, MPX_INPUT, lowLevelParametersPtr.Data);
			}

			break;
#endif

#ifdef CONFIG_FEEDBACK_ENABLE

		case FEEDBACK_I:

			if ( ((feedbackLLIparams_t*)((void*)lowLevelParametersPtr.Data))->bit.pullUp)
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}
			activeSense = (uint8_t) ((((feedbackLLIparams_t*)((void*)lowLevelParametersPtr.Data))->bit.inverted) ? 1 : 0);
			LIB_IOM_Set_IODir_ActiveSense(activeSense,pin);

			if (resetType)
			{
				/* param 1: Ram Position Conversion Result */
				/* param 2: pin -> pin del linkage - 1 */
				/* param 3: '0' -> ingresso semplice, '1' multiplexato */
				/* param 4: puntatore alla word descrittiva del feedback */
#if defined (CONFIG_PLATFORM2_FEEDBACK)
				LIB_FDB_RegisterFeedback( INPUT_RESULT_POS(pByte), pin, 0, lowLevelParametersPtr.Data );
#else
				InsertFeedback( INPUT_RESULT_POS(pByte), pin, 0, lowLevelParametersPtr.Data );
#endif /* defined (CONFIG_PLATFORM2_FEEDBACK) */
			}
			break;

#ifdef __MULTI_H

		case FEEDMULT_I:

			if (resetType)
			{

				/* param 1: Ram Position Conversion Result */
				/* param 2: pin -> pin del linkage - 1 */
				/* param 3: '0' -> ingresso semplice, '1' multiplexato */
				/* param 4: puntatore alla word descrittiva del feedback */
#if defined (CONFIG_PLATFORM2_FEEDBACK)
				LIB_FDB_RegisterFeedback( INPUT_RESULT_POS(pByte), pin, 1, lowLevelParametersPtr.Data );
#else
				InsertFeedback( INPUT_RESULT_POS(pByte), pin, 1, lowLevelParametersPtr.Data );
#endif /* #if defined (CONFIG_PLATFORM2_FEEDBACK) */
			}

			break;

#endif /* __MULTI_H */


#endif /* CONFIG_FEEDBACK_ENABLE */

#ifdef __RLTEC_DDC_H
		case DDC_I:
			if ( ((ddcLLIparams_t*)lowLevelParametersPtr.Data)->bit.pullUp )
			{
				(void)OCDIO_SetPinPullUpSync(pin);
			}

			LIB_IOM_Set_IODir_ActiveSense(0,pin);
			/** pin è il pin del modulo linkage
			    lowLevelParametersPtr.Data è il puntatore ai parametri Low Level Input */
			DDC_Init( pin, lowLevelParametersPtr.Data );

			break;
#endif  /* __RLTEC_DDC_H */
		default:
			/* CODICE NON CONTEMPLATO */
			break;


		} /* end switch */
	}
}


/* Local Function */

/* *********** */
/* End of file */
/* *********** */

