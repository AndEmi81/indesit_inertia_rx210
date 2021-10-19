/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		OutputManager.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OutputManager.c
\ingroup	OutputsManager
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n		00.00.93
\n		05/04/2011
\n		Stefano Ughi
\n		changes
\n 		- changed LIB_IOM_OutputInitialization() check __PWM_CTRL_H
\n		- changed LIB_IOM_SetLoad() check __PWM_CTRL_H
\n
\n		00.00.92
\n		09/03/2011
\n		Stefano Ughi
\n		changes
\n 		- fixed bug on LIB_IOM_OutputInitialization()
\n
\n		00.00.69
\n		05/10/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of check for params for FIXED_SPEED_MOTOR_L
\n
\n 		00.00.64
\n		22/09/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of LIB_IOM_External_SetCommand in default of SetLoad
\n
\n 		00.00.55
\n		28/07/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of support for FIXED_SPEED_MOTOR_L in LIB_IOM_GIOM_LoadPilot
\n
\n
\n 		00.00.64
\n		14/09/2010
\n		Stefano Ughi
\n		OUTPUT_TABLE_ITEM_SIZE_IN_WORDS moved from OutputManager.c to OutputManagerExp.h
\n 		00.00.65
\n		28/09/2010
\n		manage loads only with position, removed all code under condition
\n		#if(IOMANAGER_OUTPUT_MAP_MANAGE_POSITION==0)
\n      30/09/2010
\n      added #include ZcLoads.h and ExternalOutputManager.h"
\n
\n 		00.00.66
\n		01/04/2011
\n      Stefano Ughi
\n		#include "Trash.h" under defines
\n
*/
#define MODULE_BODY_LIB_IOMANAGER
#define _OUTPUT_MANAGER_

/* ******** */
/* Includes */
/* ******** */

#include "LIB_Utilities.h"
#include "OutputManager.h"
#include "InputManagerExp.h"
#include "LoadsRequestExp.h"
#include "LIB_Error.h"
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& (defined(IOM_EXTERNAL_0_ENABLED) || defined(IOM_EXTERNAL_1_ENABLED))
/*lint -e537*/
#include "ExternalOutputManager.h"
/*lint +e537*/
#endif
#include "IOSharing.h"
#include "DigitalOutputs.h"
#include "OCDIO_drv.h"
#include "callback.h"
#if (ENABLE_ZC_LOADS_MODULE == 1)
#include "ZcLoads.h"
#endif


#include "LIB_ClassBVar.h"

#ifdef CONFIG_PM_DOORLOCK
#include "PM_DoorLock.h"
#if defined(CONFIG_PM_DOORLOCK_PULSE_ENABLE)
#include "PM_DoorLock_Pulse.h"
#endif
#endif

#ifdef CONFIG_PM_DRUMLOCK
#include "PM_DrumLock.h"
#endif

#ifdef CONFIG_PM_BUZZER
#include "PM_Buzzer.h"
#endif

#ifdef CONFIG_PM_DIGITAL_OUTPUT_MULTI
#include "DigitalOutputMulti.h"
#endif

#if defined(CONFIG_PM_FIXEDSPEEDMOTOR)
#include  "FixedSpeedMotor.h"
#endif /* PM_FIXEDSPEEDMOTOR */

#if defined(CONFIG_PM_PWM_LOADSCONTROL)
#include  "PM_PWM_LoadsControl.h"
#endif /* CONFIG_PM_PWM_LOADSCONTROL */

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
#include "PM_LinkedLoads_DigitalDelayedExp.h"
#include "PM_LinkedLoads_RelayLoadExp.h"
#include "PM_LinkedLoads_RelayTriacExp.h"
#include "PM_LinkedLoads_TriacRelayExp.h"
#endif  /* defined(CONFIG_PM_LINKED_LOADS_SUPPORT) */

#ifdef CONFIG_PM_PROFILER
#include "PM_Profiler.h"
#endif /* CONFIG_PM_PROFILER */

#ifdef CONFIG_PM_STEPPER
#include "PM_Stepper.h"
#endif

#ifdef CONFIG_PM_DAMPER_ONOFF
#include "PM_Damper_ONOFF.h"
#endif
#if defined(CONFIG_PM_CONSISTENCY) && defined(CONFIG_PLATFORM2_CONSISTENCY)
#include "PM_Consistency.h"
#endif

#if defined(CONFIG_PM_MOTORGATE) /* ex __UNIVERSAL_MOTOR_H */
#include "PM_UM_UniversalMotor.h"
#endif


#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT) && defined(CONFIG_PM_PLP_PLATFORM2)
#include "PM_ProfileLoadsPartialization.h"
#endif

#if defined (CONFIG_PM_ALTERNATO)
#include "PM_Alternato.h"
#endif

/*lint -esym(750,PTR_LOADS_PROFILE_DATA) */
/*lint -esym(750,FEEDBACK_PARAMS_OFFSET) */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define EEP_PIN_TABPTR_OFFSET    			4		/* Pins displacement vs PTR_IO_CONFIG_DATA */

/* Output Maps Field Access */
#define PIN_TAB_OFFSET_MASK                 0x3F
#define LOAD_PIN_TAB_OFFSET(pByte)          (*(pByte+3) & PIN_TAB_OFFSET_MASK)     /* Pin Tab Offset Field vs PTR_IO_DEVICE_DATA */
#define FEEDBACK_PARAMS_OFFSET(pByte)       (*(pByte+1))            /* Feedback Params Offset vs PTR_IO_DEVICE_DATA */
#define PILOT_TYPE_PARAMS_OFFSET(pByte)     (*(pByte+4))            /* Pilot Type Params Offset vs PTR_IO_DEVICE_DATA */
#define LOAD_REGS(pByte)                    (*(pByte+6) & (0x0F))   /* Load's regulations */
#define LOADTYPE_PARAMS_OFFSET(pByte)       (*(pByte+7)) /* Load Type Params Offset vs PTR_IO_DEVICE_DATA */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
/** Load state config+reg.
this variable does not indicate if load is driven or not */
uint8_t LoadsStatus[N_MAXLOADS];
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
uint8_t ExtdLoadsStatus[CONFIG_NUM_EXTDLOADS][CONFIG_EXT_LOAD_PARAMDATA_SZ];
#endif

#if defined(EXTERNAL_MOTOR_CONTROL)	&& !defined(IOM_EXTERNAL_0_ENABLED) && !defined(IOM_EXTERNAL_1_ENABLED)
extern uint8_t* const DSP_CommandBuffer_Addr;
#endif /*defined(EXTERNAL_MOTOR_CONTROL) && !defined(IOM_EXTERNAL_0_ENABLED) && !defined(IOM_EXTERNAL_1_ENABLED)*/

/* Static */


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

static uint8_t LIB_IOM_GIOM_LoadPilot(const GIOM_LoadStruct *const pLoad);

static void LIB_IOM_ElemNewLoadStatus( uint8_t array_ind, uint8_t new_LS_value );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n Outputs Initialization.
\n This function is called in ResetModuleManager() if SystemMode==SYSTEM_NORMAL_MODE
\n regardless cold or warm reset.

\param  [in] resetType if !=0 -> cold reset otherwise warm reset.


\author		Stefano Ughi

\date		22/11/2010

\version    1.0.1


\todo		 -case  PWM_L 	    check struct setting file
\n   		data format not compatible with IO_MANAGER_VERSION == 0
\n          direct access to the hardware.

\note       \par \b Nota1:
\n			only loadID=position inside Output Map is supported
            documentation by Stefano Ughi
            \par \b History:
            \li Nov 20,2010 if OCDIO_SetPinDirValueSync(pin,1) return error exit from\n
            for cycle.
            \li Mar 8,2011 added control\n
             if ( LOAD_PIN_TAB_OFFSET(pByte) < PIN_TAB_OFFSET_MASK )

*/

void LIB_IOM_OutputInitialization( uint8_t resetType )
{
	struct Loader outputTabPtr;
	struct Loader pinTabPtr;
	struct Loader pilotTypeParametersPtr;
	uint8_t *pByte;
	uint8_t pilotType;
	uint8_t pin = PIN_TAB_OFFSET_MASK;
	uint8_t nPinsLinkedToOutput = 0;
	/*lint -esym(550,loadPosition) */
	uint8_t loadPosition;
	uint8_t j;
	uint8_t loadID;
	bool_t noPilot;
	uint8_t pinActivation;
	bool_t actSense;

	if (!resetType){
		uint8_t i;
		SAFETY_REGION_START();

		for(i=0;i<N_MAXLOADS; i++)
		{
			SAFETY_DATA_ELEM_CHECK( LoadsStatus, i );
		}
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
		for(i=0;i<CONFIG_NUM_EXTDLOADS; i++)
		{
			SAFETY_DATA_ELEM_CHECK( ExtdLoadsStatus, i );
		}
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
		SAFETY_REGION_END();
	}

	if ( SF_OK != BasicLoader(SF_PTR_IO_CONFIG, EEP_OUTPUT_TABPTR_OFFSET, &outputTabPtr) )
	{
		PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_1);
		return; /* Serious Error */
	}
	if ( SF_OK != BasicLoader(SF_PTR_IO_CONFIG, EEP_PIN_TABPTR_OFFSET, &pinTabPtr) )
	{
		PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_2);
		return; /* Serious Error */
	}

	/*lint -e826 */
	for ( pByte = outputTabPtr.Data, loadPosition = 0;
	        pByte < (outputTabPtr.Data + (outputTabPtr.DataLenght*2));
	        pByte += OUTPUT_TABLE_ITEM_SIZE_IN_BYTES, loadPosition++ )
	{
		/*lint +e826 */
		loadID = loadPosition;


		pilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(pByte));

		/* Check if there are pins linked to the load */
		if ( LOAD_PIN_TAB_OFFSET(pByte) < PIN_TAB_OFFSET_MASK )
		{
			/* Get the pin of ModuleIOMap[]  linked with the load */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

			if ( pin >= IO_MODULE_MAP_SIZE )
			{
				PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_3);
				return;  /* Serious Error */
			}
		}
		else
		{
			/* No physical pin linked to the load */
		}

		/* get pointer to pylot type parameters */
		if (0xFF != PILOT_TYPE_PARAMS_OFFSET(pByte))
		{
			/* Pilot Parameters Available */
			if ( 0 != BasicLoader( SF_PTR_IO_DEVICE,PILOT_TYPE_PARAMS_OFFSET(pByte), &pilotTypeParametersPtr) )
			{
				PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_LOAD_DATA);
				return;  /* Serious Error */
			}
		}


		switch (pilotType)
		{

		case DIGITAL_L:

			nPinsLinkedToOutput=1;

			break;

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT)

		case RELAY_TRIAC_L:

			nPinsLinkedToOutput=2;

			if (resetType)
			{
				{
					uint8_t *pPinsComponent = (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
					PM_LinkedLoads_RelayTriac_NewLoad(  *pPinsComponent,
					                                    *(pPinsComponent+1),
					                                    loadID,
					                                    *pilotTypeParametersPtr.Data,
					                                    *(pilotTypeParametersPtr.Data+1));
				}
			}

			break;
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT) */

#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
		case RELAY_LOAD_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
				PM_LinkedLoads_RelayLoad_NewLoad(  pin,
													*(pilotTypeParametersPtr.Data+2),
				                                   loadID,
				                                   *(pilotTypeParametersPtr.Data),
				                                   *(pilotTypeParametersPtr.Data+1));

			}

			break;

#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */

#if defined(CONFIG_PM_LINKED__DIGITAL_DELAYED_SUPPORT)
		case DIGITALDELAYED_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
				PM_LinkedLoads_DigitalDelayed_NewLoad(  pin,
				                                        loadID,
				                                        *pilotTypeParametersPtr.Data,
				                                        *(pilotTypeParametersPtr.Data+1));
			}


			break;
#endif /* defined(CONFIG_PM_LINKED__DIGITAL_DELAYED_SUPPORT) */

#if defined(CONFIG_PM_LINKED_LOADS__TRIAC_RELAY_SUPPORT)

		case TRIACRELAY_L:

			nPinsLinkedToOutput=2;

			if (resetType)
			{
				{
					uint8_t *pPinsComponent = (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

					PM_LinkedLoads_TriacRelay_NewLoad(  *pPinsComponent,
					                                    *(pPinsComponent+1),
					                                    loadID,
					                                    *pilotTypeParametersPtr.Data,
					                                    *(pilotTypeParametersPtr.Data+1));
				}
			}

			break;
#endif /* defined(CONFIG_PM_LINKED_LOADS__TRIAC_RELAY_SUPPORT)*/
#endif /* defined(CONFIG_PM_LINKED_LOADS_SUPPORT) */

#ifdef __TRIAC_RELAY_H

		case COMPTR_L:

			nPinsLinkedToOutput=2;

			if (resetType)
			{
				{
					/* first pin:  Triac drive
					   second pin: Relay drive
					   pilotTypeParametersPtr.Data: low level drive parameters  */
					const uint8_t *const pPinsComponent = (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
					(void)TriacRelay_Init( pilotTypeParametersPtr.Data, pPinsComponent );
				}
			}

			break;

#endif

#ifdef  CONFIG_PM_STEPPER

		case STEPPER_L:

			nPinsLinkedToOutput=4;
			if ( resetType )
			{
				/*lint -esym(644,pilotTypeParametersPtr) the pointer is initialized */
				/* pointer to the pins used to drive the device */
				uint8_t *pPinsComponent = (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
#if defined(PLATFORM2_STEPPER)
				/* pilotTypeParametersPtr: pointer to the device configuration table inside setting file plus data lenght */
				(void)PM_Stepper_InitParams(pPinsComponent,pilotTypeParametersPtr);
#else
				/* pilotTypeParametersPtr puntatore alla tabella di pilotaggio del carico + lunghezza tabella*/
				(void)Stepper_Insert(pPinsComponent,pilotTypeParametersPtr);
#endif
				/*lint +esym(644,pilotTypeParametersPtr) the pointer is initialized */
			}

			break;

#endif /* CONFIG_PM_STEPPER */


#ifdef CONFIG_PM_PROFILER

		case PROFILE_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
				/*lint -esym(644,pin) pin is initialized */
#if defined(PLATFORM2_PROFILER)
				(void)PM_InsertLoadWithProfile( pin,loadID );
#else
				(void)InsertLoadwithProfile( pin,loadID );
#endif
				/*lint +esym(644,pin) pin is initialized */

			}
			break;

#endif /* CONFIG_PM_PROFILER */

#if (ENABLE_ZC_LOADS_MODULE == 1)

		case ZC_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
#if defined(PLATFORM2_ZCLOADS)
				(void)LIB_ZcLoadsInsertLoadTriac( pin );
#else
				(void)ZcLoads_NewLoad_Triac( pin );
#endif
			}

			break;

		case RELEZC_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
#if defined(PLATFORM2_ZCLOADS)
				(void)LIB_ZcLoadsInsertLoadRelay( pin );
#else
				(void)ZcLoads_NewLoad_Relay( pin );
#endif
			}

			break;

#endif /* (ENABLE_ZC_LOADS_MODULE == 1) */

#ifdef CONFIG_PM_BUZZER

		case BUZZER_L:

			nPinsLinkedToOutput=1;

			PM_BuzzerInit(pin);

			break;

#endif /*CONFIG_PM_BUZZER */

#if defined(CONFIG_PM_PWM_LOADSCONTROL) || defined(__PWM_CTRL_H)

		case DIM_LED_L:
		case PWM_PROF_L:

			nPinsLinkedToOutput=1;

			PWMLoadInit(pin,pilotType);

			break;

#endif /* CONFIG_PM_PWM_LOADSCONTROL */

#ifdef __MULTI_H

		case CHIPSEL_L:

			nPinsLinkedToOutput=3;

			break;

#endif


#ifdef __MULTI2_H

		case CHIPSEL2_L:

			nPinsLinkedToOutput=2;

			break;

#endif

#ifdef __MULTI_OUT_H

		case CHIPSEL_OUT_L:

			nPinsLinkedToOutput=3;
			PM_InsMulChOut(loadID);
			
		break;

#endif

		case ACQUACARE_L:

			nPinsLinkedToOutput=1;

			break;

#ifdef CONFIG_PM_DAMPER_ONOFF

		case DUMPEROO_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
#if defined(PLATFORM2_DAMPER_ONOFF)
				(void)PM_Damper_ONOFF_InitParams(pin,pilotTypeParametersPtr.Data);
#else
				(void)Damper_ONOFF_InitParams(pin,pilotTypeParametersPtr.Data);
#endif
			}

			break;

#endif	/* CONFIG_PM_DAMPER_ONOFF */

#if defined( CONFIG_PM_DOORLOCK_MR_ENABLE )
		case BLOCCO_PORTA_MR:
		{
			struct Loader feedbacksPtr;
			DoorLockConfigParams_t params;
			params.pilot_type = BLOCCO_PORTA_IMPULSI;
			uint16_t wparams = *((uint16_t *)pilotTypeParametersPtr.Data);
			params.Delta_Timing    = (uint8_t)(wparams      & 0x00FF);
			params.Max_Num_Impulsi = (uint8_t)((wparams>>8) & 0x000F);
			params.isSensing = *((uint16_t *)pilotTypeParametersPtr.Data)>>12 & 0x0001;

			/* Get pointer to feedbacks linked to the load */
			if(!BasicLoader( SF_PTR_IO_DEVICE, FEEDBACK_PARAMS_OFFSET(pByte), &feedbacksPtr))
			{

				uint32_t numOfFeedbacksLinkedToLoad = NUM_FEEDBACKS_LINKED(feedbacksPtr.Data);

				if(numOfFeedbacksLinkedToLoad > 1 )
				{
					uint8_t readType;
					/* Tipologia del feedback */
					readType = FEEDBACK_READ_TYPE_VALUE(feedbacksPtr.Data,0);
					/*TODO: check sul tipo del FB */
					/* Posizione all'interno della variabile o vettore che ne contiene il risultato */
					params.dl_fb_lock_idx = FEEDBACK_POSITION_VALUE(feedbacksPtr.Data,0);

					readType = FEEDBACK_READ_TYPE_VALUE(feedbacksPtr.Data,1);
					/*TODO: check sul tipo del FB */
					params.mr_fb_idx = FEEDBACK_POSITION_VALUE(feedbacksPtr.Data,1);
				}
			}

			PM_DoorLock_Configure(&params);
		}
			break;
#endif
#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
		case BLOCCO_PORTA_IMPULSI:
		{
			struct Loader feedbacksPtr;
			nPinsLinkedToOutput=1;
			if(resetType)
			{

				PM_DoorLock_Configure_PilotType(BLOCCO_PORTA_IMPULSI);
				PM_DoorLock_Configure_ControlPin(pin);

				/*lint -e826 */
				PM_DoorLock_Pulse_Configure_Impulse_Properties(*((uint16_t *)pilotTypeParametersPtr.Data));
				/*lint +e826 */

				/*lint -e826 */
				PM_DoorLock_Configure_Sensing(*((uint16_t *)pilotTypeParametersPtr.Data)>>12 & 0x0001 );
				/*lint +e826 */

				/* Get pointer to feedbacks linked to the load */
				if(!BasicLoader( SF_PTR_IO_DEVICE, FEEDBACK_PARAMS_OFFSET(pByte), &feedbacksPtr))
				{
					int i,readType,feedbackPos,numOfFeedbacksLinkedToLoad;
					numOfFeedbacksLinkedToLoad = NUM_FEEDBACKS_LINKED(feedbacksPtr.Data);

					/*lint -e960 side effects on right hand of logical operator || */
					for ( i = 0; i < numOfFeedbacksLinkedToLoad; i++ )
					{
						/* Tipologia del feedback */
						readType = FEEDBACK_READ_TYPE_VALUE(feedbacksPtr.Data,i);
						/* Posizione all'interno della variabile o vettore che ne contiene il risultato */
						feedbackPos = FEEDBACK_POSITION_VALUE(feedbacksPtr.Data,i);

						 PM_DoorLock_ConfigFeedback(i,readType,feedbackPos);
					}
					/*lint +e960 side effects on right hand of logical operator || */
				}else{
					PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_LOAD_DATA);
				}
				PM_DoorLock_Configure_Ended();
			}
		}
		break;

#endif /* CONFIG_PM_DOORLOCK_PULSE_ENABLE */

#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
		case BLOCCO_PORTA_CERA:
		case BLOCCO_PORTA_CERA_2PIN:
		{
			struct Loader feedbacksPtr;
			nPinsLinkedToOutput=1;
			if(resetType)
			{

				PM_DoorLock_Configure_PilotType(BLOCCO_PORTA_CERA);
				PM_DoorLock_Configure_ControlPin(pin);
				/*lint -e826 */
				PM_DoorLock_Wax_Configure_FaultTiming( (*(uint16_t*)pilotTypeParametersPtr.Data) / 2 , (*((uint16_t *)pilotTypeParametersPtr.Data+1)) / 2); /* param in 10ms to 20ms base */

				PM_DoorLock_Configure_Sensing( *((uint16_t*)pilotTypeParametersPtr.Data+3) & 0x0001 );

				/*lint +e826 */

				/* Get pointer to feedbacks linked to the load */
				if(!BasicLoader( SF_PTR_IO_DEVICE, FEEDBACK_PARAMS_OFFSET(pByte), &feedbacksPtr))
				{
					int i,readType,feedbackPos,numOfFeedbacksLinkedToLoad;
					numOfFeedbacksLinkedToLoad = NUM_FEEDBACKS_LINKED(feedbacksPtr.Data);

					/*lint -e960 side effects on right hand of logical operator || */
					for ( i = 0; i < numOfFeedbacksLinkedToLoad; i++ )
					{
						/* Tipologia del feedback */
						readType = FEEDBACK_READ_TYPE_VALUE(feedbacksPtr.Data,i);
						/* Posizione all'interno della variabile o vettore che ne contiene il risultato */
						feedbackPos = FEEDBACK_POSITION_VALUE(feedbacksPtr.Data,i);

						PM_DoorLock_ConfigFeedback(i,readType,feedbackPos);
					}
					/*lint +e960 side effects on right hand of logical operator || */
				}else{
					PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_LOAD_DATA);
				}


				if (pilotType == BLOCCO_PORTA_CERA_2PIN)
				{
					nPinsLinkedToOutput = 2;
					PM_DoorLock_Wax_Configure_AuxPin(*(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte) + 1));
				}

				/* Parzialized load of doorlock & fault fb */
				{	uint16_t tOn, tOff, period, filterTime;

						filterTime  = *(uint8_t*)((uint8_t*)pilotTypeParametersPtr.Data + DL_PRAM_FilterTimeOffset) * 5;
						tOn  		= *(uint8_t*)((uint8_t*)pilotTypeParametersPtr.Data + DL_PRAM_DoorLockOnTimeOffset) * 5;
						tOff 		= *(uint8_t*)((uint8_t*)pilotTypeParametersPtr.Data + DL_PRAM_DoorLockOffTimeOffset) * 5;
						period = tOn+ tOff;
						PM_DoorLock_Wax_Configure_Parzilize(period, tOn, filterTime);
				}
				PM_DoorLock_Configure_Ended();
			}
		}
		break;
#endif /* CONFIG_PM_DOORLOCK_WAX_ENAB */

#ifdef CONFIG_PM_DRUMLOCK
		case DRUM_LOCK_L:
		{
			struct Loader feedbacksPtr;
			nPinsLinkedToOutput = 1;
			if (resetType)
			{
				PM_DrumLock_InitModule();
				PM_DrumLock_Configure_LockPin(pin);
				/*lint -e826 */
				PM_DrumLock_Configure_MotorOffTiming( *pilotTypeParametersPtr.Data ,
				                                     *(pilotTypeParametersPtr.Data+1) );
				/*lint +e826 */

				/* Get pointer to feedbacks linked to the load */
				if ( ! BasicLoader( SF_PTR_IO_DEVICE, FEEDBACK_PARAMS_OFFSET(pByte), &feedbacksPtr))
				{
					int readType, feedbackPos, numOfFeedbacksLinkedToLoad;
					numOfFeedbacksLinkedToLoad = NUM_FEEDBACKS_LINKED(feedbacksPtr.Data);

					/* Only 1 feedback is expected */
					PLT_ERROR_FATAL_IF( numOfFeedbacksLinkedToLoad != 1, PLT_ERROR_OUTPUT_MNGR_16)

					/* Read Feedback type */
					readType = FEEDBACK_READ_TYPE_VALUE(feedbacksPtr.Data, 0);

					/* Only digital feedback is expected */
					PLT_ERROR_FATAL_IF( readType != DIGITAL_I, PLT_ERROR_OUTPUT_MNGR_17)

					/* Posizione all'interno della variabile o vettore che ne contiene il risultato */
					feedbackPos = FEEDBACK_POSITION_VALUE(feedbacksPtr.Data, 0);

					PM_DrumLock_ConfigFeedback(feedbackPos);
				}else{
					PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_LOAD_DATA);
				}
			}
		}
		break;
#endif /* CONFIG_PM_DRUMLOCK */

#if defined(PWM_LOAD_ENABLE) && (PWM_LOAD_ENABLE == 1)
		case PWM_L:

			nPinsLinkedToOutput=0;

			PWMinit ();

			break;
#endif /*(PWM_LOAD_ENABLE == 1) */

		case I2C_L:

			nPinsLinkedToOutput=0;

			break;

#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)

		case PLP_L:
		case DIM_LAMP_L:

			nPinsLinkedToOutput=1;

			if (resetType)
			{
#if defined(CONFIG_PM_PLP_PLATFORM2)
				PM_PLP_InsertLoad(pin,pilotType);
#else
				PLP_InsertLoad(pin,pilotType);
#endif
			}

			break;

#endif /*defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT) */

#if defined(CONFIG_PM_MOTORGATE) /* ex __UNIVERSAL_MOTOR_H */

		case COLLECTORMOTOR_L:

			nPinsLinkedToOutput=4;
			{
				uint8_t *ptr_pin_tab;
				ptr_pin_tab = (uint8_t *)(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

				PM_UM_InitParams(ptr_pin_tab);
			}

			break;


#endif /* CONFIG_PM_MOTORGATE  ex __UNIVERSAL_MOTOR_H */
#ifdef CONFIG_PM_ALTERNATO
		case ALTERNATO_L:
			nPinsLinkedToOutput = 1;
			(void)PM_ALT_Init(&pilotTypeParametersPtr,pin);
			break;
#endif /* CONFIG_PM_ALTERNATO */
#ifdef CONFIG_PM_DIGITAL_OUTPUT_MULTI
		case DIGITAL_OUTPUT_MULTI_L:
			nPinsLinkedToOutput = 2;
			break;

		case DIGITAL_OUTPUT_MULTI_3P_L:
			nPinsLinkedToOutput = 3;
			break;
                        
        case DIGITALDELAYED_MULTI_L:
			nPinsLinkedToOutput = 2;
                        
			if (resetType)
			{
				DigDelayedMulti_Init( (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte) ) ,
									   *pilotTypeParametersPtr.Data ,
									   *(pilotTypeParametersPtr.Data+1));
			}
			break;
			                            
#endif /* __DIGITAL_OUTPUT_MULTI_H */
#ifdef __HOOD_MOTOR_4RELAY_H
		case HOODMOTOR_4RELAY_L:
			nPinsLinkedToOutput = 4;
                        
			if (resetType)
			{
			  HoodMotor4Rel_Init( (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte) ), *pilotTypeParametersPtr.Data);
			}
			break;
#endif /* __HOOD_MOTOR_4RELAY_H */
#ifdef  CONFIG_PM_FIXEDSPEEDMOTOR
		case FIXED_SPEED_MOTOR_L:
		{
			uint8_t * pPinsComponent = (uint8_t *)(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

			nPinsLinkedToOutput = 3;

			FSM_SetParam( *pPinsComponent,
						  *(pPinsComponent+1),
						  *(pPinsComponent+2),
						  *pilotTypeParametersPtr.Data,
						  *(pilotTypeParametersPtr.Data+1)
						);

		}
		break;
#endif  /* CONFIG_PM_FIXEDSPEEDMOTOR */
		default:
			nPinsLinkedToOutput=0;
			break;

		}/* end switch on PylotType */

		/* Hi/Low activation, only high nibble */
		pinActivation = *(pByte+6);

		/* For all the pins linked to the load */
		for ( j = 0; j < nPinsLinkedToOutput; j++ )
		{
			/* pin linked to the load */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte) + j);

			if ( pin >= IO_MODULE_MAP_SIZE )
			{
				PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_5);
				return;  /* Serious Error */
			}

			/* Set the pin as output */
			if ((IO_ErrorType)IO_E_OK != OCDIO_SetPinDirValueSync(pin,1))
			{
				PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_11);
				/* Fatal Error -> Exit from for cycle */
				break;
			}

			if (resetType)
			{
#ifdef __MULTI_H
				if (pilotType == CHIPSEL_L)
				{
					(void)PM_InsMulChSelInPin(pin);
				}
#endif  /* #ifdef __MULTI_H */

#ifdef __MULTI2_H

				if (pilotType == CHIPSEL2_L)
				{
					InsMulChSelPin2(pin);
				}
#endif  /* #ifdef __MULTI2_H */

#ifdef __MULTI_OUT_H
				if (pilotType == CHIPSEL_OUT_L)
				{
					(void)PM_InsMulChSelOutPin (loadID, pin);
				}
#endif  /* __MULTI_OUT_H */
			}


			/* Ask for driving the pin to the application if TRUE no */
			noPilot = CBF_IOManager_IOVal_Ctrl(loadID);
			/* The pin is active high or low ? */
            actSense = (bool_t) ((pinActivation & (((uint8_t) 0x10) << j)) ? 1 : 0);
			LIB_IOM_Set_IODir_ActiveSense(actSense,pin);

			if (noPilot == TRUE)
			{
				/* switch off the pin */
				/*lint -e730 Boolean argument to function */
                OCDIO_SetPinValueSync(pin, (uint8_t) !actSense);
				/*lint +e730 */
			}

		} /* end for */
	}

	/* Init data safety */
	{
		uint8_t i;
		SAFETY_REGION_START();

		for(i=0;i<N_MAXLOADS; i++)
		{
			SAFETY_DATA_ELEM_UPDATE( LoadsStatus, i );
		}
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
		for(i=0;i<CONFIG_NUM_EXTDLOADS; i++)
		{
			SAFETY_DATA_ELEM_UPDATE( ExtdLoadsStatus, i );
		}
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0) */
		SAFETY_REGION_END();
	}
}

/**
This function searchs the position of a load identified by loadPos inside the OutputMap.

\param  [in]	loadPos	load's position

\param  [out]	ppByte pointer to pointer \n
                    points to the star addr. of the structure that describes the load.\n

\return     uint8_t
\retval		LOAD_NOT_FOUND if load position is out of range \n
            otherwise load's position inside OutputMap.

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

\note       \par \b Nota 1:
			documentation by Alessandro Sabbatini

\todo		1) il ritorno della funzione dovrebbe essere una struct \n
            costituita da un puntatore, a NULL se carico non	 \n
            trovato e da un byte loadIndex \n
            2) non  necessario passare il puntatore iniziale all'Output Map.\n
            e questo riporta al punto 1).


*/
uint8_t LIB_IOM_FindLoad(uint8_t loadPos,uint8_t **ppByte)
{
	Loader_Struct outputTabPtr;
	uint8_t loadIndex = LOAD_NOT_FOUND;


	if ( 0 == BasicLoader(SF_PTR_IO_CONFIG, EEP_OUTPUT_TABPTR_OFFSET, &outputTabPtr) )
	{
		if (loadPos < (outputTabPtr.DataLenght / OUTPUT_TABLE_ITEM_SIZE_IN_WORDS)) {
			*ppByte = (&outputTabPtr.Data[loadPos * OUTPUT_TABLE_ITEM_SIZE_IN_BYTES]);
			loadIndex = loadPos;
		}

	}
	return loadIndex; /*OK LOAD IN THE TABLE*/

}
/**
Load act./disact.

\param  [in]	loadID load's ID or Load's position inside Output Map
\param  [in]	loadParam  struct typedef L_PARAM

\attention \b  LoadsStatus[] typedef uint8_t used as W

\return 	uint8_t

\retval		SET_LOAD_NO_ERROR
\retval		SET_LOAD_ERROR


\author 	Stefano Ughi
\date		21/06/2010
\version	1.0.1


\todo
			-case  PWM_L 	    check struct setting file \n
   			formato dati NON compatibile con  IO_MANAGER_VERSION == 0
   			-case  PWM_L direct access to hardware


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
/*lint -esym(529,ramData) ramData
  used under some options of compilation */
uint8_t LIB_IOM_SetLoad( uint8_t loadID, L_PARAM loadParam)
{
	struct Loader pinTabPtr;
	struct Loader ramData;

	bool_t skipSetLoad = FALSE;
	uint8_t *pByte;
	uint8_t loadIndex;
	uint8_t pilotType;
	uint8_t pin;

	if ( 0 != BasicLoader(SF_PTR_IO_CONFIG, EEP_PIN_TABPTR_OFFSET, &pinTabPtr) )
	{
		PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_8);
		return SET_LOAD_ERROR; /* Serious Error */
	}

	loadIndex = LIB_IOM_FindLoad(loadID,&pByte);
	/* warning: pWord is modified by FinLoad and now point to the start addr. of load's descr.*/
	if ( loadIndex == LOAD_NOT_FOUND )
	{
		PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_LOAD_NOT_FOUND);
		return SET_LOAD_ERROR;
	}

    (void) &ramData;
    

	if (loadParam.s.reg > LOAD_REGS(pByte))
	{
		/* load's regulation requested not valid */
		return SET_LOAD_ERROR;
	}

	pilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(pByte));

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

#if defined(CONFIG_NUM_SR_LOADS)
	{
		uint8_t tmpCfgSR = LIB_IOM_FindLoadOnSwitchRelay(loadIndex);
		if ((tmpCfgSR & (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE | LOAD_SR_SAME_POS)) == (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE))
		{
			loadParam.b = LOAD_REG_OFF;
			if ( (pilotType != DIGITALDELAYED_L)
			        && (pilotType != TRIACRELAY_L)
			        && (pilotType != RELAY_TRIAC_L)
			        && (pilotType != RELAY_LOAD_L)
			   )
			{
				skipSetLoad = TRUE;
			}
		}

		if (((tmpCfgSR & (LOAD_SR_FOUND | LOAD_SR_ITSELF)) == (LOAD_SR_FOUND | LOAD_SR_ITSELF))
				&& (LoadsStatus[loadIndex] != loadParam.b) )
				{
					uint8_t i;
					uint8_t *tmpPByte;
					uint8_t nMaxLoads = LIB_IOM_NumberOfLoads();

					for (i = 0; i < nMaxLoads; i++)
					{
						if (LIB_IOM_GetSRLoadPosByRelLdPos(i) == loadIndex)
						{
							uint8_t tmpPilotType = LIB_IOM_GetLoadPilotType(i);

							(void) LIB_IOM_FindLoad(i,&tmpPByte);
#if defined(CONFIG_PM_CONSISTENCY)
							PM_Consistency_LoadChangedStatus(i, ~LoadsStatus[i], LoadsStatus[i], tmpPByte, tmpPilotType);
#endif /*defined(CONFIG_PM_CONSISTENCY)*/
						}
					}
				}
	}
#endif /* defined(CONFIG_NUM_SR_LOADS) */

#if defined(CONFIG_NUM_MR_LOADS)
	{
		uint8_t tmpCfgMR = LIB_IOM_FindLoadOnMasterRelay(loadIndex);

		if (((tmpCfgMR & (LOAD_MR_FOUND | LOAD_MR_ITSELF)) == (LOAD_MR_FOUND | LOAD_MR_ITSELF))
				&& (LoadsStatus[loadIndex] != loadParam.b) )
				{
					uint8_t i;
					uint8_t *tmpPByte;
					uint8_t nMaxLoads = LIB_IOM_NumberOfLoads();

					for (i = 0; i < nMaxLoads; i++)
					{
						if (LIB_IOM_GetMRLoadPosByRelLdPos(i) == loadIndex)
						{
							uint8_t tmpPilotType = LIB_IOM_GetLoadPilotType(i);

							(void) LIB_IOM_FindLoad(i,&tmpPByte);
							PM_Consistency_LoadChangedStatus(i, ~LoadsStatus[i], LoadsStatus[i], tmpPByte, tmpPilotType);
						}
					}
				}
	}
#endif /* defined(CONFIG_NUM_MR_LOADS) */
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */

#ifdef CONFIG_PM_CONSISTENCY
#if defined (CONFIG_PLATFORM2_CONSISTENCY)
	PM_Consistency_LoadChangedStatus(loadIndex,LoadsStatus[loadIndex],loadParam.b,pByte,pilotType);
#else
	CS_LoadChangedStatus(loadIndex,LoadsStatus[loadIndex],loadParam.b,(uint16_t *) pByte,pilotType);
#endif
#endif /* CONFIG_PM_CONSISTENCY */

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)

#endif

	LIB_IOM_ElemNewLoadStatus( loadIndex, loadParam.b );

	/* if defined LOAD_DONT_CARE property and the M.S.B. of config is '1'  load is ignored */
#ifdef LOAD_DONT_CARE
	if (loadParam.s.config & 0x08)
	{
		if ( (pilotType != DIGITALDELAYED_L)
		        && (pilotType != TRIACRELAY_L)
		        && (pilotType != RELAY_TRIAC_L)
		        && (pilotType != RELAY_LOAD_L)
		   )
		{
			skipSeskipSetLoad = TRUE;
		}
	}
#endif /* #ifdef LOAD_DONT_CARE */

	if (skipSetLoad == FALSE)
	{
		switch (pilotType)
		{
	#if defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT)
	#if defined(CONFIG_PM_PLP_NORMAL_SUPPORT)

		case PLP_L:

		{
			uint8_t rpt = 0;   /* repetitions */

			/* Get pin */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

			/* Pylot Type Param */
			(void)BasicLoader(SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);


			if (loadParam.s.reg > *ramData.Data)
			{
				return SET_LOAD_ERROR;
			}
			if (loadParam.s.reg)
			{
				/* get repetitions */
				rpt=*(ramData.Data+2*loadParam.s.reg +1);

				/* get load profile data */
				(void)BasicLoader(SF_PTR_LOADS_PROFILE, *(ramData.Data+2*loadParam.s.reg), &ramData);
			}
			/*lint -esym(644,rpt) rpt ha senso solo se loadParam.s.reg != 0 */



			/*lint -e826 */
			/* Set load cmd */
			PM_PLP_SetLoad(loadParam.s.reg, pin, (uint16_t *)ramData.Data+1,(uint8_t)((*ramData.Data)+1), rpt, *(ramData.Data+1));
			/*lint +e826 */

		}
		break;

	#endif /* defined(CONFIG_PM_PLP_NORMAL_SUPPORT) */

	#if defined(PM_PLP_DIMMER_SUPPORT)

		case DIM_LAMP_L:

		{
			/* Get pin */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
			/* Pylot Type Param */
			(void)BasicLoader(SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);

			/*lint -e826 */
			/* Set load cmd */
			PM_DIM_SetLoad(loadParam.s.reg, loadParam.s.config, pin, (uint8_t *)(ramData.Data));
			/*lint +e826 */

		}
		break;

	#endif /* definded(PM_PLP_DIMMER_SUPPORT)*/
	#endif /* defined(CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT) */

	#if defined(CONFIG_PM_PWM_LOADSCONTROL) || defined(__PWM_CTRL_H)

		case DIM_LED_L:

		{
			/* Get pin */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
			/* Pylot Type Params  */
			(void)BasicLoader( SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);

			/*lint -e826 */
			/* Set load cmd */
			PWMSetLoad(loadParam.s.reg, loadParam.s.config, pin, (uint8_t *)(ramData.Data));
			/*lint +e826 */

		}
		break;


		case PWM_PROF_L:

		{
			/* Get pin */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
			/* Pylot Type Params  */
			(void)BasicLoader( SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);

			if (loadParam.s.reg > *ramData.Data)
			{
				return SET_LOAD_ERROR;
			}
			if (loadParam.s.reg)
			{
				/* get load profile data */
				(void)BasicLoader( SF_PTR_LOADS_PROFILE, *(ramData.Data+loadParam.s.reg), &ramData);
			}

			/*lint -e826 */
			/* Set load cmd */
			PWMSetLoad(loadParam.s.reg, loadParam.s.config, pin, (uint8_t *)(ramData.Data));
			/*lint +e826 */

		}
		break;

	#endif /* CONFIG_PM_PWM_LOADSCONTROL */

		case DIGITAL_L:

			/* switch on/off the digital load */
			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
			LIB_DIO_SET_LOAD( loadParam.s.reg, pin );

			break;

	#ifdef __TRIAC_RELAY_H

		case COMPTR_L:

			(void)TriacRelay_SetLoad(loadParam.s.reg);

			break;

	#endif /* __TRIAC_RELAY_H */

	#if defined (CONFIG_PM_LINKED_LOADS_SUPPORT)
	#if defined(CONFIG_PM_LINKED__DIGITAL_DELAYED_SUPPORT)

		case DIGITALDELAYED_L:
	#endif /* defined(CONFIG_PM_LINKED__DIGITAL_DELAYED_SUPPORT) */
	#if defined(CONFIG_PM_LINKED_LOADS__TRIAC_RELAY_SUPPORT)
		case TRIACRELAY_L:
	#endif /* defined(CONFIG_PM_LINKED_LOADS__TRIAC_RELAY_SUPPORT) */
	#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) || defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT)
		case RELAY_TRIAC_L:
		case RELAY_LOAD_L:
	#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) || defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT) */

			PM_LinkedLoads_SetLoad(loadParam.b,loadID);

			break;
	#endif

	#ifdef __DAMPER_ONOFF_H

		case DUMPEROO_L:

	#if defined (PLATFORM2_DAMPER_ONOFF)
			(void)PM_Damper_ONOFF_SetPosition(loadParam.s.reg);
	#else
			(void)Damper_ONOFF_SetPosition(loadParam.s.reg);
	#endif

			break;

	#endif  /* __DAMPER_ONOFF_H */

	#ifdef CONFIG_PM_DOORLOCK_PULSE_ENABLE
		case BLOCCO_PORTA_IMPULSI:
			if (loadParam.s.config == 0)
			{
				PM_DoorLock_SetPosition(loadParam.s.reg);
			}

			LIB_IOM_ElemNewLoadStatus( loadIndex, loadParam.s.reg );

			break;
	#endif /* CONFIG_PM_DOORLOCK_PULSE_ENABLE*/
	#if defined(CONFIG_PM_DOORLOCK_SINGLEPULSE_ENABLE)
		case BLOCCO_PORTA_MONO_IMPULSO:

			if (loadParam.s.config == 0)
			{
				PM_DoorLock_SetPosition(loadParam.s.reg);
			}

			LIB_IOM_ElemNewLoadStatus( loadIndex, loadParam.s.reg );

			break;
	#endif


	#ifdef CONFIG_PM_DOORLOCK_WAX_ENABLE
		case BLOCCO_PORTA_CERA:
		case BLOCCO_PORTA_CERA_2PIN:
			PM_DoorLock_SetPosition(loadParam.s.reg);
			LIB_IOM_ElemNewLoadStatus( loadIndex, loadParam.s.reg );
			break;
	#endif /* CONFIG_PM_DOORLOCK_WAX_ENABLE */

	#ifdef CONFIG_PM_DRUMLOCK
		case DRUM_LOCK_L:

			PM_DrumLock_SetPosition(loadParam.s.reg);
			LIB_IOM_ElemNewLoadStatus( loadIndex, loadParam.s.reg );

			break;
	#endif /* CONFIG_PM_DRUMLOCK */

	#ifdef CONFIG_PM_STEPPER

		case STEPPER_L:

	#if defined (PLATFORM2_STEPPER)
			(void)PM_Stepper_SetLogicalPosition(loadParam.s.reg);
	#else
			(void)Stepper_SetLogicalPosition(loadParam.s.reg);
	#endif

			break;

	#endif	/* CONFIG_PM_STEPPER */

	#ifdef CONFIG_PM_PROFILER

		case PROFILE_L:

			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

			/* abilita o disabilita un profilo associato al carico */
			if (loadParam.s.reg)
			{
				/* Pylot Type Params get displ. profilo da applicare vs pointer Platform_Loads_Profile_Data_Addr */
				(void)BasicLoader( SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);
				/* *(ramData.Data+loadParam.s.reg-1) displacement vs Platform_Loads_Profile_Data_Addr in base
				   al quale vado a ricavare il puntatore al byte iniziale del profilo da
				   applicare */
				(void)BasicLoader( SF_PTR_LOADS_PROFILE, *(ramData.Data+loadParam.s.reg-1), &ramData);
				/* puntatore al profilo da applicare, pin, repetion */
	#if defined(PLATFORM2_PROFILER)
				(void)PM_ProfileEnable( ramData.Data, pin, loadParam.s.config );
	#else
				(void)ProfileEnable( ramData.Data, pin, loadParam.s.config );
	#endif
			}
			else
			{
	#if defined(PLATFORM2_PROFILER)
				(void)PM_ProfileDisable( pin );
	#else
				(void)ProfileDisable( pin );
	#endif
			}

			break;

	#endif  /* CONFIG_PM_PROFILER */


	#if (ENABLE_ZC_LOADS_MODULE == 1)

		case RELEZC_L:
		case ZC_L:

			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));

			/* abilita o disabilita un carico parzializzato */
			if (loadParam.s.reg)
			{
				/* Pylot Type Params */
				(void)BasicLoader( SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);
				/* Profilo attivazione/disattivazione */
	#if defined(PLATFORM2_ZCLOADS)
				(void)LIB_ZcLoadsLoadEnable( pin, *(ramData.Data + loadParam.s.reg-1));
	#else
				(void)ZcLoads_LoadEnable( pin, *(ramData.Data + loadParam.s.reg-1));
	#endif
			}
			else
			{
	#if defined(PLATFORM2_ZCLOADS)
				(void)LIB_ZcLoadsLoadDisable( pin );
	#else
				(void)ZcLoads_LoadDisable( pin );
	#endif
			}
			break;

	#endif /* (ENABLE_ZC_LOADS_MODULE == 1) */

	#ifdef CONFIG_PM_BUZZER

		case BUZZER_L:

			if (loadParam.s.reg)
			{
				/* Pylot Type Params */
				(void)BasicLoader( SF_PTR_IO_DEVICE, PILOT_TYPE_PARAMS_OFFSET(pByte), &ramData);
				PM_SetBuzzer(*(ramData.Data + loadParam.s.reg-1), loadParam.s.config);
			}

			break;

	#endif  /* CONFIG_PM_BUZZER */

	#if defined(PWM_LOAD_ENABLE) && (PWM_LOAD_ENABLE == 1)

		case PWM_L:

			pin = *(pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte));
			if (loadParam.s.reg >= 5)  /* 100% */
			{

				/* disable PWM */
	#if defined (BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
				*((uint8_t far*)(FIRST_PWM_CHANNEL_ADR + (pin-FIRST_PWM_CHANNEL_PIN)*3)) = 0;
	#endif
				LIB_DIO_SET_LOAD(1, pin);
			}
			else if (loadParam.s.reg)
			{
				/* set duty  */
	#if defined (BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
				*((uint16_t far*)(FIRST_PWM_CHANNEL_ADR+1 + (pin-FIRST_PWM_CHANNEL_PIN)*3)) = PWM_STEP*loadParam.s.reg;
	#endif
				if ( LIB_IOM_Get_IODir_ActiveSense(pin) )
				{
					/* PWM , high-true pulses */
	#if defined  (BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
					*((uint8_t far*)(FIRST_PWM_CHANNEL_ADR + (pin-FIRST_PWM_CHANNEL_PIN)*3)) = 0x28;
	#endif
				}
				else
				{
					/* PWM , low-true pulses */
	#if defined (BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
					*((uint8_t far*)(FIRST_PWM_CHANNEL_ADR + (pin-FIRST_PWM_CHANNEL_PIN)*3)) = 0x2C;
	#endif
				}
			}
			else
			{
				/* disable PWM */
	#if defined (BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
				*((uint8_t far*)(FIRST_PWM_CHANNEL_ADR + (pin-FIRST_PWM_CHANNEL_PIN)*3)) = 0;
	#endif
				LIB_DIO_SET_LOAD(0, pin);
			}

			break;

	#endif /*(PWM_LOAD_ENABLE == 1) */
	#if defined(CONFIG_PM_ALTERNATO)

		case ALTERNATO_L:
			PM_ALT_SetPosition(loadParam);
			break;

	#endif /* CONFIG_PM_ALTERNATO */
	#ifdef CONFIG_PM_DIGITAL_OUTPUT_MULTI
		case DIGITAL_OUTPUT_MULTI_L:
			/*  Nuovo stato per il carico - Puntatore ai pin linkati al carico */
			DigOutMultiSetLoad( loadParam, (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte)) );
			break;

		case DIGITAL_OUTPUT_MULTI_3P_L:
			/*  Nuovo stato per il carico - Puntatore ai pin linkati al carico */
			DigOutMul3PSetLoad( loadParam, (pinTabPtr.Data + LOAD_PIN_TAB_OFFSET(pByte)) );
			break;

		case DIGITALDELAYED_MULTI_L:
			DigDelayedMultiSetLoad(loadParam, (pinTabPtr.Data +  LOAD_PIN_TAB_OFFSET(pByte)) );
			break;

	#endif /* __DIGITAL_OUTPUT_MULTI_H */
	#ifdef __MULTI_H

		case CHIPSEL_L:
		break;

	#endif	/* __MULTI_H */

	#ifdef __MULTI2_H

		case CHIPSEL2_L:
		break;

	#endif	/* __MULTI2_H */

	#ifdef __MULTI_OUT_H

		case CHIPSEL_OUT_L:
			/* [S.S.] Nota: è necessario chiamare due volte la funzione PM_setOutMux, poichè
			 * (per il comportamento interno della stessa) l'enable al singolo mux viene
			 * attivato solo se il parametro di ingresso si mantiene costante in due
			 * iterazioni succesive. */
			PM_setOutMux(loadID, loadParam.s.reg);
			PM_setOutMux(loadID, loadParam.s.reg);
		break;

	#endif	/* __MULTI_OUT_H */

	#ifdef __HOOD_MOTOR_4RELAY_H
		case HOODMOTOR_4RELAY_L:
			HoodMotor4RelSetLoad(loadParam, (pinTabPtr.Data +  LOAD_PIN_TAB_OFFSET(pByte)) );
			break;
	#endif /* __HOOD_MOTOR_4RELAY_H */

		default:
			PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_UNMANAGED_LOAD);
			return SET_LOAD_ERROR;


		}  /* end switch on PylotType */
	}

	return SET_LOAD_NO_ERROR;
}

/**
This function switch off all the loads.

\param  [in]    source Id of caller, possible values are\b
                - SOURCE_SALO_PCCONTROL caller is P.C. control cmd.\b
                  no Application's callback is called.\b
                - SOURCE_SALO_PLATFORM caller is Platform\b
                  and Application's callback is called.

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

\bug        non tutti i carichi con param.b si spengono 22/08/2008

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_IOM_SetAllLoadsOff ( uint8_t source )
{
	bool_t hasToSet;
	uint8_t loadPos;
	uint8_t numLoads = LIB_IOM_NumberOfLoads();


#ifdef __GENERICIOMANAGER_H

	GIOM_LoadStruct load;
	(void)memset( &load,0,sizeof(load));

#else

	L_PARAM param;
	param.b=0;

#endif /* #ifdef __GENERICIOMANAGER_H */

	for ( loadPos = 0; loadPos < numLoads; loadPos++ )
	{

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
		(void) source;
		hasToSet = LIB_IOM_CFG_SetAllLoadsFilter(loadPos);
#else
		hasToSet = ( CBF_SetAllLoaddOff(loadPos) && ( source == SOURCE_SALO_PLATFORM ) ) ||
		        ( source != SOURCE_SALO_PLATFORM );
#endif
		/* Check the caller */
		if ( hasToSet )
		{
#ifdef __GENERICIOMANAGER_H
			load.loadID = loadPos;
			(void)LIB_IOM_GIOM_SetLoad (&load);
#else
			(void)LIB_IOM_SetLoad( loadID, param);

#endif /* #ifdef __GENERICIOMANAGER_H */
		}
	}
}

/**
This function call the LIB_IOM_SetLoad or the LIB_IOM_GIOM_LoadPilot according to the return of LIB_IOM_GIOM_LoadPilot.

\param  [in]  pLoad pointer type GIOM_LoadStruct

\return     bool_t
\retval     TRUE -> O.K.
\retval	    FALSE -> problem
\warning

\author 	Stefano Ughi
\date		21/06/2010
\version	1.0.1

*/
bool_t LIB_IOM_GIOM_SetLoad (const GIOM_LoadStruct *const pLoad)
{
	bool_t retVal = FALSE;
	uint8_t result;

	result = LIB_IOM_GIOM_LoadPilot(pLoad);

	if( result == GIOM_USE_STANDARD_SET_LOAD )
	{

#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
		if (PM_LinkedLoads_RelayLoad_NotDrive(pLoad->loadID,  pLoad->loadParam.s.reg))
		{
			retVal = TRUE;
		}
		else
#endif  /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
		if( LIB_IOM_SetLoad(pLoad->loadID, pLoad->loadParam) == SET_LOAD_NO_ERROR )
		{
			retVal = TRUE;
		}
		else
		{
			PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_SET_LOAD_FAIL);
		}
	}
	else
	{
		if( result == GIOM_SET_LOAD_NO_ERROR )
		{
			retVal = TRUE;
		}
		else
		{
			PLT_ERROR_WARNING(PLT_ERROR_OUTPUT_MNGR_SET_LOAD_FAIL);
		}
	}

	return retVal;
}


/**
This function, given pilot type field (cfr. Setting File), returns the real pilot type.

\param  [in]	pilotTypeField pilot type field


\return 	uint8_t

\retval		real pilot type


\author 	stefano.ughi
\date		July 14, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint8_t LIB_IOM_GetPilotType(uint8_t pilotTypeField)
{
	uint8_t pilotType;
	uint8_t pilotTypeExtra;
	uint8_t realPiloType;

    pilotTypeExtra  = (uint8_t) ((pilotTypeField & 0x03) << 5);
    pilotType       = (uint8_t) ((pilotTypeField & 0x7C) >> 2);
    realPiloType    = (uint8_t) (pilotType | pilotTypeExtra);
    return realPiloType;
}

/**
This function, given position, returns the state of the load.

\param  [in]	position   load's position

\return 	uint8_t

\retval		load's state - LOAD_REG_OFF if out of range

\author 	stefano.ughi
\date		Dec 14, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint8_t LIB_IOM_GetLoadStatus(uint8_t position)
{
	uint8_t retVal = LOAD_REG_OFF;

	if (position < N_MAXLOADS)
	{
		retVal = LoadsStatus[position];
	}
  
	return retVal;
}

/**
This function, given position, returns the state of the load.

\param  [in]	extLoadItm		extended load item
\param	[out]	pExtdStatus		pointer to Extended Status array to copy data

\return 	uint8_t

\retval		load's state - LOAD_REG_OFF if out of range

\author 	Alessandro Sabbatini
\date		Jan 28, 2014
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
uint8_t LIB_IOM_GetExtdLoadStatus(uint8_t extLoadItm, uint8_t pExtdStatus[CONFIG_EXT_LOAD_PARAMDATA_SZ])
{
	uint8_t retVal = LOAD_REG_OFF;

	if (extLoadItm < CONFIG_NUM_EXTDLOADS)
	{
		uint8_t tmpPos = LIB_IOM_GetLoadPosByExtdLoadItem(extLoadItm);

		retVal = LIB_IOM_GetLoadStatus(tmpPos);
		memcpy(pExtdStatus, ExtdLoadsStatus[extLoadItm], sizeof(ExtdLoadsStatus[0]));
	}
	return retVal;
}
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0) */
/**
\n Function that provides all Loads Status to application

\param		[out] newLoadStatus		Array of all Loads request (max index is N_MAXLOADS)
\param		[out] newExtdLoadStatus	Array of all Extended Loads request (max index is IOM_NUM_OF_EXTERNAL_LOADS)

\return		none

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini
*/
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
void LIB_IOM_GetAllLoadsStatus(uint8_t *pNewLoadStatus, uint8_t (*pNewExtdLoadStatus)[CONFIG_EXT_LOAD_PARAMDATA_SZ])
{
	uint8_t i;

	SAFETY_REGION_START();
	for(i=0;i<N_MAXLOADS;i++)
	{
		/*TODO MS verificare se necessario, se c'è gia un controllo periodico */
		SAFETY_DATA_ELEM_CHECK( LoadsStatus,i );
	}
	SAFETY_REGION_END();
	
	if(pNewLoadStatus)
	{
		memcpy(pNewLoadStatus, LoadsStatus, N_MAXLOADS);
	}

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
	SAFETY_REGION_START();
	for(i=0;i<CONFIG_NUM_EXTDLOADS;i++)
	{
		SAFETY_DATA_ELEM_CHECK( ExtdLoadsStatus, i );
	}
	SAFETY_REGION_END();

	if(pNewExtdLoadStatus)
	{
		memcpy(pNewExtdLoadStatus, ExtdLoadsStatus, CONFIG_NUM_EXTDLOADS*CONFIG_EXT_LOAD_PARAMDATA_SZ);
	}
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0) */
}
#endif

/**
This function returns number of loads defined in setting file.

\param  [in]	position   load's position


\return 	uint8_t

\retval		number of loads defined in setting file.


\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
uint8_t LIB_IOM_NumberOfLoads(void)
{
	Loader_Struct outputTabPtr;
	uint8_t tmpOutputLen = 0;

	if ( SF_OK == BasicLoader(SF_PTR_IO_CONFIG, EEP_OUTPUT_TABPTR_OFFSET, &outputTabPtr) )
	{
		tmpOutputLen = (uint8_t) (outputTabPtr.DataLenght / ( OUTPUT_TABLE_ITEM_SIZE_IN_WORDS));
	}
	return tmpOutputLen;
}

/**
This function, given load position, returns its real pilot type.

\param  [in]	loadPos   load's position


\return 	uint8_t

\retval		real pilot type

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/

uint8_t LIB_IOM_GetLoadPilotType(uint8_t loadPos) {
	uint8_t *tmpLoad;
	uint8_t tmpPilotType = LOAD_NOT_FOUND;

	if (( loadPos == LIB_IOM_FindLoad(loadPos, &tmpLoad)) && (loadPos != LOAD_NOT_FOUND))
	{
		tmpPilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(tmpLoad));
	}

	return tmpPilotType;

}

/**
This function, given load position, returns Pin tab pointer.

\param  [in]	loadPos			load's position
\param  [out]	PinTabPtr	Pointer to related Pin tab table \n
                    This argumend is set only if load is found.


\return 	uint8_t

\retval		== LOAD_NOT_FOUND	if load not found or pilot parameters table is not available
\retval		!= LOAD_NOT_FOUND	Pilot type parameters offset (as found in setting file)

\author 	Alessandro Sabbatini
\date		Jan 30, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/

uint8_t LIB_IOM_GetLoadPinTabTable(uint8_t loadPos, uint8_t *PinTabPtr)
{
	uint8_t tmpPinTabDispl = LOAD_NOT_FOUND;
	uint8_t *tmpLoad;

	if (( loadPos == LIB_IOM_FindLoad(loadPos, &tmpLoad)) && (loadPos != LOAD_NOT_FOUND))
	{
		Loader_Struct tmpPinTab;

		if ( SF_OK == BasicLoader(SF_PTR_IO_CONFIG, EEP_PIN_TABPTR_OFFSET, &tmpPinTab))
		{
			tmpPinTabDispl = PILOT_TYPE_PARAMS_OFFSET(tmpLoad);

			if (tmpPinTabDispl != LOAD_NOT_FOUND)
			{
				PinTabPtr = &tmpPinTab.Data[tmpPinTabDispl];
			}
		}
	}

	return tmpPinTabDispl;

}


/**
This function, given load position, returns Pilot parameters table pointer.

\param  [in]	loadPos			load's position
\param  [out]	ParmTablePtr	Pointer to Pilot parameters table \n
                    This argumend is set only if load is found.


\return 	uint8_t

\retval		== LOAD_NOT_FOUND	if load not found or pilot parameters table is not available
\retval		!= LOAD_NOT_FOUND	Pilot type parameters offset (as found in setting file)

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/

uint8_t LIB_IOM_GetLoadPilotParamTable(uint8_t loadPos, Loader_Struct *ParmTableLdrPtr) {
	uint8_t resPilotParmDispl = LOAD_NOT_FOUND;
	uint8_t *tmpLoad;

	if (( loadPos == LIB_IOM_FindLoad(loadPos, &tmpLoad)) && (loadPos != LOAD_NOT_FOUND))
	{
		Loader_Struct tmpPilotParm;
		uint8_t tmpPilotParmDispl = LOADTYPE_PARAMS_OFFSET(tmpLoad);

		if ( SF_OK == BasicLoader(SF_PTR_IO_DEVICE, tmpPilotParmDispl, &tmpPilotParm))
		{
			resPilotParmDispl = tmpPilotParmDispl;
			ParmTableLdrPtr->Data = tmpPilotParm.Data;
			ParmTableLdrPtr->DataLenght = tmpPilotParm.DataLenght;
		}
	}

	return resPilotParmDispl;

}

/**
This function, given load position, returns related Priority bitmap field

\param  [in]	loadPos			load's position

\return 	uint8_t		Priority bitmap field, 0 is default value if load is not found

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/

uint8_t LIB_IOM_GetLoadPriorityBitmap(uint8_t loadPos) {
	uint8_t *tmpLoad;
	uint8_t tmpPrioBitmap = 0;

	if (( loadPos == LIB_IOM_FindLoad(loadPos, &tmpLoad)) && (loadPos != LOAD_NOT_FOUND))
	{
		tmpPrioBitmap = PRIORITY_BITMAP(tmpLoad);
	}

	return tmpPrioBitmap;


}

/**
This function, given load position, returns related Max Regulations field

\param  [in]	loadPos			load's position

\return 	uint8_t		 Max Regulations field, 0 is default value if load is not found

\author 	Alessandro Sabbatini
\date		Jan 22, 2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/

uint8_t LIB_IOM_GetLoadMaxRegValue(uint8_t loadPos) {
	uint8_t *tmpLoad;
	uint8_t tmpMaxReg = 0;

	if (( loadPos == LIB_IOM_FindLoad(loadPos, &tmpLoad)) && (loadPos != LOAD_NOT_FOUND))
	{
		tmpMaxReg = (uint8_t) LOAD_REGS(tmpLoad);
	}

	return tmpMaxReg;

}

/* Local Function */
/**
This function ask to the application for what kind of pilot must be used.
If basic pilot return, otherwise extended pilot applied.

\param  [in]		pLoad pointer to a struct


\return 	uint8_t

\retval		GIOM_USE_STANDARD_SET_LOAD
\retval		GIOM_SET_LOAD_NO_ERROR
\retval		GIOM_ERROR_LOAD_NOT_PRESENT


\author 	stefano.ughi
\date		Jun 22, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/

static uint8_t LIB_IOM_GIOM_LoadPilot(const GIOM_LoadStruct *const pLoad)
{
	uint8_t *pByte = NULL;
	bool_t isStdLoad = TRUE;
	uint8_t pilotType;
	uint8_t loadIndex;

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#if defined(CONFIG_NUM_EXTDLOADS)
	isStdLoad = ( LIB_IOM_GetExtdLoadPosByLoadPos(pLoad->loadID) == LOAD_NOT_FOUND);
#endif /* defined(CONFIG_NUM_EXTDLOADS)*/
#else
	isStdLoad = ( CBF_GIOM_LoadPilot(pLoad->loadID) == FALSE );
#endif

	if (isStdLoad)
	{
		/* Application says "Pilot the load using the standard LIB_IOM_SetLoad" */
		return GIOM_USE_STANDARD_SET_LOAD;
	}
	loadIndex = LIB_IOM_FindLoad(pLoad->loadID,&pByte);
	/* warning: pInt is modified by FinLoad and now point to the start addr. of load's descr.*/

	if ( LOAD_NOT_FOUND == loadIndex )
	{
		PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_13);
		return GIOM_ERROR_LOAD_NOT_PRESENT;
	}

	pilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(pByte));

	switch (pilotType)
	{
#if defined(CONFIG_PM_MOTORGATE) /* ex __UNIVERSAL_MOTOR_H */
	case COLLECTORMOTOR_L:
	{
		uint8_t newStatus = (PM_UM_RelaysInRunningState() ? LOAD_ON : LOAD_OFF);

#ifdef CONFIG_PM_CONSISTENCY
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( LoadsStatus, loadIndex );
		SAFETY_REGION_END();
#if defined (CONFIG_PLATFORM2_CONSISTENCY)
		PM_Consistency_LoadChangedStatus(loadIndex,LoadsStatus[loadIndex],newStatus,pByte,pilotType);
#else
		CS_LoadChangedStatus(loadIndex,LoadsStatus[loadIndex],newStatus,(uint16_t *) pByte,pilotType);
#endif
#endif /* CONFIG_PM_CONSISTENCY */

		LIB_IOM_ElemNewLoadStatus( loadIndex, newStatus );

		PM_UM_SetTargetSync(pLoad);

	}

		break;
#endif  /* CONFIG_PM_MOTORGATE  ex __UNIVERSAL_MOTOR_H */
/* This case was kept for compatibility with old platform */
#if defined(EXTERNAL_MOTOR_CONTROL) && !defined(IOM_EXTERNAL_0_ENABLED) && !defined(IOM_EXTERNAL_1_ENABLED) && !defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	case EXTERN_MODULE_L:
		/* if velocity target != 0 load status = 1 */
		LIB_IOM_ElemNewLoadStatus( loadIndex, ((pLoad->data[0] || pLoad->data[1])?(1):(0)) );
		CriticalSectionBegin(Engine_Res);
		{
			/* copy the data in the command buffer */
			(void)memcpy(DSP_CommandBuffer_Addr,&pLoad->data[0],DSP_CMD_DATA_SIZE);
		}
		CriticalSectionEnd(Engine_Res);

		break;
#endif /* defined(EXTERNAL_MOTOR_CONTROL) && !defined(IOM_EXTERNAL_0_ENABLED) && !defined(IOM_EXTERNAL_1_ENABLED)*/

#ifdef  CONFIG_PM_FIXEDSPEEDMOTOR
	case FIXED_SPEED_MOTOR_L:
		/* if speed target != 0 load status = 1 */
		LIB_IOM_ElemNewLoadStatus( loadIndex, ((pLoad->data[0] || pLoad->data[1])?(1):(0)) );
		FSM_SetPointUpdate(pLoad);
		break;
#endif  /* CONFIG_PM_FIXEDSPEEDMOTOR */

	default:

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && (defined(IOM_EXTERNAL_0_ENABLED) || defined(IOM_EXTERNAL_1_ENABLED))
		if(LIB_IOM_External_SetCommand(pilotType, pLoad) != LIB_IOM_NO_ERROR)
		{
			PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_14);
			return GIOM_ERROR_LOAD_NOT_PRESENT;
		}
		LIB_IOM_ElemNewLoadStatus( loadIndex, LIB_IOM_External_GetLoadStatus(pilotType, pLoad) );
#else
		PLT_ERROR_FATAL(PLT_ERROR_OUTPUT_MNGR_14);
		return GIOM_ERROR_LOAD_NOT_PRESENT;
#endif
	}
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
	loadIndex = LIB_IOM_GetExtdLoadPosByLoadPos(pLoad->loadID);
	if (loadIndex != LOAD_NOT_FOUND) {

		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( ExtdLoadsStatus, loadIndex );
		memcpy(ExtdLoadsStatus[loadIndex], pLoad->data, sizeof(pLoad->data));
		SAFETY_DATA_ELEM_UPDATE( ExtdLoadsStatus, loadIndex );
		SAFETY_REGION_END();

	}
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0) */

	/*lint -e527 depends from defines */
	return GIOM_SET_LOAD_NO_ERROR;
	/*lint +e527 depends from defines */

}


/**
This function searches the position of a load identified by loadID inside the OutputMap.

\param  [in]		load's position

\param  [out]	ppByte pointer to pointer \n
                    points to the star addr. of the structure that describes the load.\n
                    This argumend is set only if load is found.

\return     uint8_t
\retval		LOAD_NOT_FOUND if load not found  \n
            otherwise load's position inside OutputMap.

\author 	Platform Group
\date		gg/mm/aaaa
\version	1.0.0

\note       \par \b Nota 1:
			documentation by Alessandro Sabbatini

*/
uint8_t LIB_IOM_FindLoadByID(uint8_t loadID,uint8_t **ppByte)
{
	Loader_Struct outputTabPtr;
	uint8_t loadIndex = LOAD_NOT_FOUND;


	if ( SF_OK == BasicLoader(SF_PTR_IO_CONFIG, EEP_OUTPUT_TABPTR_OFFSET, &outputTabPtr) )
	{
		uint8_t i;
		uint8_t mapLen = (uint8_t) (outputTabPtr.DataLenght / OUTPUT_TABLE_ITEM_SIZE_IN_WORDS);
		uint8_t *tmpItem;

		for (i = 0; i < mapLen; i++)
		{
			tmpItem = (outputTabPtr.Data + (i * OUTPUT_TABLE_ITEM_SIZE_IN_BYTES));

			if (loadID == LOAD_ID(tmpItem)) {
				loadIndex = i;
				if (ppByte != NULL) {
					*ppByte = tmpItem;
				}
				break;
			}
		}
	}
	return loadIndex; /*OK LOAD IN THE TABLE*/

}


/**
Store the new Load Status value of the selected load in the related element \n
of LoadsStatus array

\param  [in]		array_ind: index of element into LoadsStatus array
\param  [in]		new_LS_value: new Load Status value of array_ind-th element


\return 	none


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0

\note		\par \b Nota 1:
			documentation by xyz


*/

static void LIB_IOM_ElemNewLoadStatus( uint8_t array_ind, uint8_t new_LS_value )
{
  SAFETY_REGION_START();

  if (array_ind < N_MAXLOADS)
  {
	  SAFETY_DATA_ELEM_CHECK( LoadsStatus, array_ind );
	  LoadsStatus[array_ind] = new_LS_value;
	  SAFETY_DATA_ELEM_UPDATE( LoadsStatus, array_ind );

  }

  SAFETY_REGION_END();
}

/* *********** */
/* End of file */
/* *********** */

