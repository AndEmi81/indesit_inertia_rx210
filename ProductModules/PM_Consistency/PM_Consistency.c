/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
 * \page pageConsistency
 * \ingroup	PM_Consistency
 */

/**
\par		PM_Consistency.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_Consistency.c
\ingroup	PM_Consistency
\date		July 13, 2005
\version	01.00.00
\author		Camerin ALberto
\n
\n
\par	History:
\n
\n 		01.00.00
\n		July 13, 2005
\n		Camerin ALberto
\n		changes
\n 			Creation
\n
\n 		01.00.01
\n		Dec 15, 2010
\n		Stefano Ughi
\n		changes
\n 			heavy modifications  check on V.C.S.
*/

/* */
#define MODULE_BODY_CONSISTENCY
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "LIB_Error.h"
#if defined(CONFIG_PM_CONSISTENCY)

#include <string.h>
#include "PM_Consistency.h"

#include "Lib_utilities.h"
#include "IO_driver.h"

#include "OutputManager.h"
#include "InputManagerExp.h"

#include "Feedbacks.h"
#include "DigitalInputsExp.h"

#if defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
#include "PM_AnalogFeedback.h"
#endif  /* CONFIG_PM_ANALOG_FEEDBACK*/

#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
#include "PM_FrequencyFeedbacks.h"
#endif

#if defined(CONFIG_PM_FIXEDSPEEDMOTOR)
#include "FixedSpeedMotor.h"
#endif

#if  defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
#include "PM_LinkedLoads.h"
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
#include "PM_LinkedLoads_RelayLoadExp.h"
#endif /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)*/
#endif

#if defined(CONFIG_PM_MOTORGATE)
#include "PM_UM_UniversalMotorExp.h"
#endif

/*#include "ProductModules_Cfg.h"*/
#include "LIB_ZeroCrossingExp.h"

#ifdef  CONFIG_PM_DOORLOCK
#include "PM_DoorLockExp.h"
#endif  /* CONFIG_PM_DOORLOCK*/

#ifdef  CONFIG_PM_DRUMLOCK
#include "PM_DrumLockExp.h"
#endif  /* CONFIG_PM_DRUMLOCK*/

#ifdef __MULTI_H
#include "Trash1000.h"  /* remove a.s.a.p.*/
#endif

#include "LIB_ClassBVar.h"
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#include "LoadsRequestExp.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Load's state modified for consistency, state off  */
#define CS_LOAD_NOT_ACTIVE					0
/** Load's state modified for consistency, state on  */
#define CS_LOAD_ACTIVE						1
/** Load's state modified for consistency, only relay switched to the load but Triac is off */
#define CS_LOAD_FFC_ONLY_RELAY_ACTIVE		2
/** Load's state modified for consistency, relay not switched to the load
This define is used also to distinguish the OFF state of fan */
#define	CS_LOAD_FFC_NOT_ACTIVE				3
/** Load's state modified for consistency, High Speed fan */
#define	CS_LOAD_FAN_HIGH_SPEED				4


/** Diplacement vs consistency's table: load's state on */
#define CS_LOAD_STATUS_DISPLACEMENT_PILOT			0
/** Diplacement vs consistency's table: load's state off */
#define CS_LOAD_STATUS_DISPLACEMENT_NOT_PILOT		4
/** Diplacement vs consistency's table: relay not switched to the load */
#define CS_LOAD_STATUS_DISPLACEMENT_FFC_NOT_PILOT   8

/** Set CONFIG_CS_LINKEDLOADS_EXTRA_DELAY to original default 25 if not defined */
#if !defined(CONFIG_CS_LINKEDLOADS_EXTRA_DELAY)
#define CONFIG_CS_LINKEDLOADS_EXTRA_DELAY       25
#endif

/** Set CONFIG_CS_DOORLOCK_POWERON_DELAY to original default 500 if not defined */
#if !defined(CONFIG_CS_DOORLOCK_POWERON_DELAY)
#define CONFIG_CS_DOORLOCK_POWERON_DELAY       500
#endif


#ifdef __DAMPER_ONOFF_H
/** Non ci sono problemi sul pilotaggio del Damper On Off */
#define DAMPER_ONOFF_FEEDBACK_IS_OK   0
/** Ci sono problemi sul pilotaggio del Damper On Off */
#define DAMPER_ONOFF_FEEDBACK_IS_KO   1
#endif /* __DAMPER_ONOFF_H */

#ifdef __STEPPER_H
/** Non ci sono problemi sullo Stepper */
#define STEPPER_FEEDBACK_IS_OK   0
/** Ci sono problemi sullo Stepper */
#define STEPPER_FEEDBACK_IS_KO   1
#endif /* __STEPPER_H */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Struttura di ritorno da CS_ManageLoad e in input a CS_ManageRes */
typedef struct LoadStatusRes
{
	/** status of the load */
	uint8_t load_status;
	/** code for the status */
	uint8_t cs_code;
} LoadStatusRes_t;

#ifdef __DAMPER_ONOFF_H
/** Struttura di stato per il Damper ON OFF accesso a bit */
typedef struct
{
	/** se '0' e' la prima volta che entro nel case Damper Pilotato */
	uint8_t firstTimeDamperPylot    :1;
	/** se '0' e' la prima volta che entro nel case Damper Non Pilotato */
	uint8_t firstTimeDamperNotPylot :1;
	/** se '1' posizione damper e feedback O.K.  */
	uint8_t cameFromInputOK         :1;
	/** se '1' stop monitoring su damper pilotato */
	uint8_t stopMonitoring          :1;
} DamperOnOffState_s;

/** Struttura di stato per il Damper ON OFF accesso a byte e bit */
/*lint -esym(754,all)*/
typedef union
{
	uint8_t all;
	DamperOnOffState_s bit;
} DamperOnOffState_t;

#endif /* #ifdef __DAMPER_ONOFF_H */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

/**  Variables used to manage the module */
static struct
{
#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)
	/** Pointer to the buffer, passed by Application, where the module will inserts consistency values.*/
	uint8_t  *pCS_Resource;
	/** Status of the Consistency module */
	uint8_t  Status;
#endif
	/** vettore contatori con base tempi pari alla chiamata del manager */
	uint8_t  CS_LoadCnt[N_MAXLOADS];
	/** Time to wait until next evaluation of the feedback for Digital Delayed and Triac+Rele' */
	uint8_t  CS_MaxDelay;
#ifdef __DAMPER_ONOFF_H
	/** variabile utilizzata per l'inizializzazione dei timeouts \n
	che servono a controllare il corretto comportamento del dispositivo */
	DamperOnOffState_t damperOnOffState;
	/** contatore per ottenere 1 sec */
	uint8_t damperOnOffcntrOneSec;
	damperONOFFPosition_t damperCurrentPos;
	uint8_t damperFeedbackValue;
#endif
} CS_Status_Var;


/* Pointer used just to support Data Care operation */
static uint8_t PLT_Consistency[N_MAXLOADS];

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static uint8_t CS_GetValue(uint8_t *pFeedbackData);

static void CS_ManageLoad( uint8_t load_status, uint8_t feedback_val, LoadStatusRes_t *res);

static void CS_ManageRes( LoadStatusRes_t res, uint8_t *pFeedBackData, uint8_t loadPos );

static void CS_ElemNewConsistency( uint8_t array_ind, uint8_t new_CS_value );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n The function is called from the application and performs initialization of a vector
\n containing the output of the module.

\param  [in]	pLLConsistency pointer to the array where will be inserted the outputs of the module.

\attention \b We assume that vector's size passed through pLLConsistency is equal to N_MAXLOADS.

\author 	Stefano Ughi
\date		18/02/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			the function has been written in this way to maintain compatibility with the past

*/

void PM_Consistency_ConfigureOutputArray(uint8_t *pLLConsistency)
{
#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)
	CS_Status_Var.pCS_Resource = pLLConsistency;
	CS_Status_Var.Status |= (PM_CS_STATUS_INIT);
#endif
}

/**
Module's initialization, called if reset is cold and if SystemMode == SYSTEM_NORMAL_MODE.

\attention \b CS_Status_Var used as I/O
\attention \b CS_Status_Var.OutputTablePtr used as I/O type Loader


\author 	Stefano Ughi
\date		18/02/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi


*/

void PM_Consistency_Init(void)
{
	uint8_t i;

	SAFETY_REGION_START();
	
	(void)memset(&CS_Status_Var,0,sizeof(CS_Status_Var));
	(void)memset(PLT_Consistency, PM_CS_NOT_VALID_CODE, N_MAXLOADS);

	for ( i=0; i<N_MAXLOADS; i++)
	{
		SAFETY_DATA_ELEM_UPDATE( PLT_Consistency, i );
	}
	
	SAFETY_REGION_END();
#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)
	CS_Status_Var.Status |= (PM_CS_STATUS_CONFIGURED);
#endif

}

/**
Called inside periodic task if SystemMode == SYSTEM_NORMAL_MODE. \n
For all the loads present in the Output Map checks if there are feedbacks associated \n
with, if so check if there are data for Consistency and processes them.

\attention \b CS_Status_Var used as I/O


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.1


\todo		CS_Status_Var.CS_MaxDelay deve essere un vettore !!
\todo       il valore di ritorno di CS_GetValue() dovrebbe essere testato e sollevata \n
            un'eccezione se maggiore di 3

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			version 1.0.1 extension to loads with more feedbacks linked

*/
void PM_Consistency_Manager(void)
{
	struct Loader feedbackData;
	uint8_t *pByte;
	LoadStatusRes_t res;
	uint8_t loadPos;
	uint8_t nLoads = LIB_IOM_NumberOfLoads();
	uint8_t pilotType;
	uint8_t readType = 0;
	uint8_t feedbackPos;
	uint8_t load_status;
	uint8_t feedback_value;
	uint8_t numOfFeedbacksLinkedToLoad;
	bool_t feedbacksReady;
	uint8_t i;
	uint8_t tmpNewMaxDelay = 0;

#if defined(CONFIG_PM_DOORLOCK)
        if( PM_DoorLock_isInstableCondition() )
        {
          for(i=0; i<N_MAXLOADS; i++) /* invalida tutti i consistency*/
          {
            CS_ElemNewConsistency( i, PM_CS_NOT_VALID_CODE );
          }
          return;
        }
#endif /* defined(CONFIG_PM_DOORLOCK) */

	/* per tutti i carichi presenti nell'output map */
	for ( loadPos = 0; loadPos < nLoads; loadPos++)
	{
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT) || defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
		uint8_t TRL_Status = 0; /*Variable to Chk the status of the R+T module*/
#endif
#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
		uint8_t plpStatus = 0;  /*Variable to Chk the status of the PLP module*/
#endif
#if defined(CONFIG_PM_DOORLOCK)
        static uint16_t PowON_Delay = CONFIG_CS_DOORLOCK_POWERON_DELAY;
#endif /* CONFIG_PM_DOORLOCK */

		(void) LIB_IOM_FindLoad(loadPos, &pByte);

		/* Tipo di pilotaggio del carico */
		pilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(pByte));

		/* Per il carico in oggetto punto al campo iniziale dei parametri feedback linkati al carico stesso */
		if ( BSLDR_OK != BasicLoader( SF_PTR_IO_DEVICE, LOAD_FEEDBACKS_OFFSET(pByte), &feedbackData))
		{
			/* Parametri feedback non presenti */
			continue;
		}
		/*lint +e826 Suspicious pointer-to-pointer */


		numOfFeedbacksLinkedToLoad = NUM_FEEDBACKS_LINKED(feedbackData.Data);


		if ( feedbackData.DataLenght <= ( 1+numOfFeedbacksLinkedToLoad ) )
		{
			/* There's no data for consistency chk !!! -> Skip this load !!! */
			continue;
		}

		feedbacksReady = TRUE;
		/*lint -e960 side effects on right hand of logical operator || */
		for ( i = 0; (feedbacksReady == TRUE) && (i < numOfFeedbacksLinkedToLoad); i++ )
		{
			/* Tipologia del feedback */
			readType = FEEDBACK_READ_TYPE_VALUE(feedbackData.Data,i);
			/* Posizione all'interno della variabile o vettore che ne contiene il risultato */
			feedbackPos = FEEDBACK_POSITION_VALUE(feedbackData.Data,i);
			/* Controlla se tutti i feedbacks linkati al carico sono pronti */
			switch(readType)
			{
			case FEEDBACK_I:
			case FEEDMULT_I:
			{
				if ( FALSE == LIB_FDB_IsFeedbackWritableReady( feedbackPos ) )
				{
					feedbacksReady = FALSE;
				}
			}
			break;

			case DIGITAL_I:
			case DIGMULT_I:
			{
				if (FALSE == LIB_DIO_IsDigitalReady( feedbackPos ))
				{
					feedbacksReady = FALSE;
				}
			}
			break;

#if (defined(CONFIG_CS_ZEROCROSSING_DELAY) && (CONFIG_CS_ZEROCROSSING_DELAY > 0))
			case ZEROCROSSING_I:
			{
				tmpNewMaxDelay = CONFIG_CS_ZEROCROSSING_DELAY;
			}
			break;
#endif

#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
			case FREQFEEDBACK_I:
			{
				if ( FALSE == PM_FrequencyFeedbacks_GetReadyByPosition(feedbackPos) )
				{
					feedbacksReady = FALSE;
				}
			}
			break;
#endif

#if defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
			case ANALOGFEEDBACK_I:
			{
				if (FALSE == PM_AnalogFeedbackIsReady( feedbackPos ))
				{
					feedbacksReady = FALSE;
				}
			}
			break;
#endif

			default:
			break;

			}

		}

		if ( FALSE == feedbacksReady )
		{
			/* per il carico in oggetto i feedbacks ad esso linkati non sono pronti */
			CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
			continue; /* Process next load */
		}

		/* Stato del carico (solo reg no config)
		   attenzione questa variabile indica solo il valore delle regolazione
		   applicata al carico ma non dice se il carico e' pilotato o meno in questo istante */

		load_status = (uint8_t)(LIB_IOM_GetLoadStatus(loadPos)&LOAD_REG_MASK);

#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
		if (pilotType == PLP_L)
		{
			/* Store the state of PLP loads */
			plpStatus = load_status;
		}
#endif /* #if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT ) */

		/* Time to wait until next evaluation of the feedback */
		CS_Status_Var.CS_MaxDelay = tmpNewMaxDelay;

		/* A seconda del tipo di pilotaggio del carico */
		switch (pilotType)
		{
			/*========================================*/
			/* Carichi con un solo feedback associato */
			/*========================================*/
#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)

		case DIGITALDELAYED_L:
		case TRIACRELAY_L:

			CS_Status_Var.CS_MaxDelay = PM_LinkedLoads_GetTiming(loadPos, load_status) + (uint8_t)CONFIG_CS_LINKEDLOADS_EXTRA_DELAY;
			/* -fallthrough */
		case RELAY_TRIAC_L:
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
		case RELAY_LOAD_L:
#endif  /* #if (CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT == 1) */

#endif /* __TRIAC_RELAY_MULTI_H */

		case DIGITAL_L:

#if defined(__TRIAC_RELAY_H)
		case COMPTR_L:
#endif /* __TRIAC_RELAY_H */

		case ZC_L:
		case RELEZC_L:

#if defined(CONFIG_PM_MOTORGATE)
		case COLLECTORMOTOR_L:
			if ((pilotType == COLLECTORMOTOR_L) && (load_status != PM_UM_RelaysInRunningState()))
			{
				PM_Consistency_LoadChangedStatus(loadPos, load_status, !load_status, pByte, pilotType);
			}
			/* -fallthrough */
#endif

#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
		case PLP_L:
#endif /*#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT ) */

#if defined(CONFIG_PM_DIGITAL_OUTPUT_MULTI)
		case DIGITAL_OUTPUT_MULTI_L:
		case DIGITALDELAYED_MULTI_L:
#endif

#ifdef __HOOD_MOTOR_4RELAY_H
		case HOODMOTOR_4RELAY_L:
#endif

#if defined(CONFIG_PM_FIXEDSPEEDMOTOR)
            case FIXED_SPEED_MOTOR_L:
            
                if ( pilotType == FIXED_SPEED_MOTOR_L )
                {
	              	/* Delay before next evaluation */
	              	CS_Status_Var.CS_MaxDelay = CONFIG_CS_FIXEDSPEEDMOTOR_DELAY;
                }
#endif

			/* Valore del feedback */
			feedback_value = CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0));

			if ( CS_Status_Var.CS_LoadCnt[loadPos] < CS_Status_Var.CS_MaxDelay )
			{
				CS_Status_Var.CS_LoadCnt[loadPos]++;
				continue;
			}

			if ( (load_status & LOAD_REG_MASK) != LOAD_REG_OFF )
			{
				load_status = CS_LOAD_ACTIVE;
			}

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_SR_LOADS)

			/* Load is OFF and consistency table is for 3 load states */
			if ( ((load_status & LOAD_REG_MASK) == LOAD_REG_OFF) &&
					(feedbackData.DataLenght == (1 + 4*numOfFeedbacksLinkedToLoad)) )
			{
				uint8_t tmpFindLoadOnSR = LIB_IOM_FindLoadOnSwitchRelay(loadPos);
				/* Check if load is on a switch relay */
				if (tmpFindLoadOnSR & LOAD_SR_FOUND)
				{
					if (tmpFindLoadOnSR & LOAD_SR_SAME_POS)
					{
						 /* If switch relay status is selecting it, treat like it's a Relay-only relay+triac linked load. */
						load_status = CS_LOAD_FFC_ONLY_RELAY_ACTIVE;
					}
					else
					{
						 /* If switch relay status is not selecting it, treat like it's an OFF relay+triac linked load. */
						load_status = CS_LOAD_FFC_NOT_ACTIVE;
					}
				}
			}

#endif


#if defined(CONFIG_PM_FIXEDSPEEDMOTOR)

			if ( pilotType == FIXED_SPEED_MOTOR_L )
                {
                    /* During relays transition, Consistency shall not be considered valid */
                    if( FSM_GetCntStatus() )
                    {
                       CS_Status_Var.CS_LoadCnt[loadPos]=0;
                       CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
                       continue;
                    }
                }
#endif  /* defined(CONFIG_PM_FIXEDSPEEDMOTOR) */


#if defined(CONFIG_PM_LINKED_LOADS__RELAY_TRIAC_SUPPORT) || defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
			if ( ( pilotType == RELAY_TRIAC_L )
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
			        || ( pilotType == RELAY_LOAD_L )
#endif  /* defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT) */
			   )
			{

				/*Chk if i'm piloting the Relay or Triac or both o none */
				uint8_t tmpLLState = PM_LinkedLoads_GetLoadState(loadPos);

				if (tmpLLState == LL_1ON)
				{
					TRL_Status = CS_LOAD_FFC_ONLY_RELAY_ACTIVE;
				}
				else if (tmpLLState == (LL_1ON | LL_2ON))
				{
					TRL_Status = CS_LOAD_ACTIVE;
				}

				SAFETY_REGION_START();
				SAFETY_DATA_ELEM_CHECK( PLT_Consistency, loadPos );
				SAFETY_REGION_END();

				if ((PLT_Consistency[loadPos] != PM_CS_NOT_VALID_CODE) && (TRL_Status != ((PLT_Consistency[loadPos]>>3)&0x03)))
				{
#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)

					if ( FEEDBACK_READ_TYPE_VALUE(feedbackData.Data,0) == FREQFEEDBACK_I )
					{
					    PM_FrequencyFeedbacks_RestartFeedbackByPosition(FEEDBACK_POSITION_VALUE(feedbackData.Data,0));
					}
					else
					{
#endif /*CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT */

						LIB_FDB_FeedbackInitMeasure(FEEDBACK_POSITION_VALUE(feedbackData.Data,0));

#if defined(CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT)
					}
#endif/* CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT */

					CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
					continue;
				}

				if (TRL_Status == LL_2ON) /*Relay ON*/
				{
					load_status = CS_LOAD_FFC_ONLY_RELAY_ACTIVE; /*Assert that only the relay is ON */
				}
				else if (TRL_Status == LL_ALL_OFF)
				{
					load_status = CS_LOAD_FFC_NOT_ACTIVE;
				}
				else if (TRL_Status & LL_1ON)
				{
					load_status = CS_LOAD_ACTIVE;
				}
				else
				{
					/*All other cases*/
				}
			}
#endif /* __TRIAC_RELAY_MULTI_H */


#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
			if ( pilotType == PLP_L )
			{
				/* During ZC synchronization, Consistency shall not be considered valid */
				if( !goodZCSignal() && IsZCSyncOngoing() )
				{
					CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
					continue;
				}

				if (plpStatus == 0) /* OFF State */
				{
					load_status = CS_LOAD_FFC_NOT_ACTIVE;
				}
				else if (plpStatus == 3) /* High Speed */
				{
					load_status = CS_LOAD_FAN_HIGH_SPEED;
				}
				else /* Low Speed */
				{
					load_status = CS_LOAD_ACTIVE;
				}

			}
#endif /* #if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT ) */


			CS_ManageLoad(load_status,feedback_value, &res);

			CS_ManageRes(res,feedbackData.Data,loadPos);

			break;


#ifdef  __STEPPER_H

		case STEPPER_L:
			/*===============================================*/
			/* Dai feedbacks reali ricavo il feedback logico */
			/*===============================================*/
			if ( TRUE == PM_StepperIsMoving() )
			{
				/* Stepper in movimento non posso prendere decisioni */
				CS_Status_Var.CS_LoadCnt[loadPos] = 0;
				CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
			}
#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT

			else if( FEEDBACK_READ_TYPE_VALUE(feedbackData.Data,0) == ANALOGFEEDBACK_I )
			{
				if ( CS_Status_Var.CS_LoadCnt[loadPos] >= 100 )
				{
					load_status = CS_LOAD_NOT_ACTIVE;
					feedback_value = STEPPER_FEEDBACK_IS_OK;

					/* L'unico caso distinto e' quello di carico scablato, che viene    */
					/* identificato col valore 0 nella tabella L.L.I. del Setting File */

					if( CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0)) == 0 )
					{
						feedback_value = STEPPER_FEEDBACK_IS_KO;
					}

					CS_ManageLoad(load_status, feedback_value, &res);
					CS_ManageRes(res, feedbackData.Data, loadPos);
				}
				else
				{
					/* attendo che il feedback sia consistente */
					CS_Status_Var.CS_LoadCnt[loadPos]++;
					CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
				}
			}
#endif /* CONFIG_PM_ANALOG_FEEDBACK */
			else /* Digital Feedback */
			{
				if ( numOfFeedbacksLinkedToLoad == 4 )
				{
					if ( CS_Status_Var.CS_LoadCnt[loadPos] > (2*FEEDBACK_SAMPLES) )
					{
						load_status = CS_LOAD_NOT_ACTIVE;
						feedback_value = 0;
						for ( i = 0; i < 4; i++ )
						{
							feedback_value <<= 1;

							feedback_value |= (CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,i))?1:0);
						}
						if ( PM_Stepper_GetLastSeqApplied() == feedback_value )
						{
							feedback_value = STEPPER_FEEDBACK_IS_OK;
						}
						else
						{
							feedback_value = STEPPER_FEEDBACK_IS_KO;
						}
						CS_ManageLoad(load_status, feedback_value, &res);
						CS_ManageRes(res, feedbackData.Data, loadPos);
					}
					else
					{
						/* attendo che il feedback sia consistente */
						CS_Status_Var.CS_LoadCnt[loadPos]++;
						CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
					}
				}
				else if(numOfFeedbacksLinkedToLoad == 1)
				{
					if ( CS_Status_Var.CS_LoadCnt[loadPos] > (2*FEEDBACK_SAMPLES) )
					{
						load_status = CS_LOAD_NOT_ACTIVE;
						feedback_value = STEPPER_FEEDBACK_IS_OK;

						/* L'unico caso distinto e' quello di carico scablato */
						/* che fornisce un valore di feedback digitale alto  */

						if( CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0)) )
						{
							feedback_value = STEPPER_FEEDBACK_IS_KO;
						}

						CS_ManageLoad(load_status, feedback_value, &res);
						CS_ManageRes(res, feedbackData.Data, loadPos);
					}
					else
					{
						/* attendo che il feedback sia consistente */
						CS_Status_Var.CS_LoadCnt[loadPos]++;
						CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
					}
				}
				else
				{
					/* Non e' previsto un numero di feedback digitali diverso da 1 e 4 */
				}
			}
			break;

#endif /*__STEPPER_H */


#ifdef __PROFILER_H
			/*========================================*/
			/* Carichi con un solo feedback associato */
			/*========================================*/

		case PROFILE_L:

			/* Valore del feedback */
			feedback_value = CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0));

			/* Per il carico contraddistinto da loadPos
			   controlla se e' un profilo in esecuzione.
			   N.B.: la durata massima di un profilo e' di 320 msec a 50 Hz
			   Cfr. Loads Profile Data in Platform Setting doc
			   quindi avro' il primo dato utile per consistency dopo
			   320 + 2*FEEDBACK_SAMPLES = 520 msec con FEEDBACK_SAMPLES=20 */

			if ( TRUE == PM_IsProfileActiveInPos(loadPos) )
			{
				/* profilo in esecuzione non posso prendere decisioni */
				CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
			}
			else
			{
				/* profilo non in esecuzione.
				 N.B.:CS_Status_Var.CS_LoadCnt[loadPos] viene azzerato ogni
				 volta che c'e' un cambiamento di profilo  */
				if ( CS_Status_Var.CS_LoadCnt[loadPos] > (2*FEEDBACK_SAMPLES) )
				{
					/* il feedback e' consistente */
					CS_ManageLoad(CS_LOAD_NOT_ACTIVE, feedback_value, &res);

					CS_ManageRes(res, feedbackData.Data, loadPos);

				}
				else
				{
					/* attendo che il feedback sia consistente */
					CS_Status_Var.CS_LoadCnt[loadPos]++;
					CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
				}
			}
			break;

#endif /*__PROFILER_H*/

#ifdef __BUZZER_H
		case BUZZER_L:

			/* To be complete ... */
			continue;
#endif /* __BUZZER_H */

#ifdef __MULTI_H
		case CHIPSEL_L:

			/* [To be complete ...] */
			continue;


#endif /*__MULTI_H*/


#ifdef __MULTI2_H
		case CHIPSEL2_L:

			/* [To be complete ...] */
			continue;


#endif /*__MULTI2_H*/

#ifdef __MULTI_OUT_H
        case CHIPSEL_OUT_L:

            /* [To be completed ...] */
            continue;


#endif /*__MULTI_OUT_H*/

		case ACQUACARE_L:

			/* To be complete ...*/
			continue;

#ifdef __DAMPER_ONOFF_H

		case DUMPEROO_L:
			/*=========================*/
			/* Feedback di tipo logico */
			/*=========================*/
			if ( TRUE == PM_Damper_ONOFF_IsDriven() )
			{
				/*=============================*/
				/* caso damper on off pilotato */
				/*=============================*/
				/* per quando il Damper risultera' non pilotato */
				CS_Status_Var.damperOnOffState.bit.firstTimeDamperNotPylot = 0;
				load_status = CS_LOAD_ACTIVE;

				if ( CS_Status_Var.damperOnOffState.bit.firstTimeDamperPylot == 0 )
				{
					/* first time -> init */
					CS_Status_Var.CS_LoadCnt[loadPos] = 0;
					CS_Status_Var.damperOnOffcntrOneSec = 0;
					CS_Status_Var.damperOnOffState.bit.stopMonitoring = 0;
					CS_Status_Var.damperOnOffState.bit.firstTimeDamperPylot = 1;
				}
				if ( CS_Status_Var.damperOnOffState.bit.stopMonitoring == 1 )
				{
					/* Posizionamento non raggiunto entro il timeout */
					feedback_value = DAMPER_ONOFF_FEEDBACK_IS_KO;
				}
				else
				{
					feedback_value = DAMPER_ONOFF_FEEDBACK_IS_OK;
					CS_Status_Var.CS_LoadCnt[loadPos] ++;
					if ( CS_Status_Var.CS_LoadCnt[loadPos] >= 100 )
					{
						/* one second elapsed */
						CS_Status_Var.CS_LoadCnt[loadPos] = 0;

						if ( CS_Status_Var.damperOnOffcntrOneSec >= PM_Damper_ONOFF_GetTmaxReachPos() )
						{
							CS_Status_Var.damperOnOffState.bit.stopMonitoring = 1;
						}
						else
						{
							CS_Status_Var.damperOnOffcntrOneSec ++;
						}
					}
				}
			}
			else
			{
				/*=================================*/
				/* caso damper on off NON pilotato */
				/*=================================*/
				/* per quando il Damper risultera' pilotato */
				CS_Status_Var.damperOnOffState.bit.firstTimeDamperPylot = 0;
				load_status = CS_LOAD_NOT_ACTIVE;
				if ( CS_Status_Var.damperOnOffState.bit.firstTimeDamperNotPylot == 0 )
				{
					/* first time -> init */

					/* Campiona la posizione corrente del damper */
					CS_Status_Var.damperCurrentPos = PM_Damper_ONOFF_GetReedVal();
					/* Clear filter for monitoring */
					CS_Status_Var.CS_LoadCnt[loadPos] = 0;
					/* Flag condizione corretta */
					CS_Status_Var.damperOnOffState.bit.cameFromInputOK = 1;
					/* Stato del feedback O.K. fino a quando non avra' eseguito il nuovo campionamento */
					CS_Status_Var.damperFeedbackValue = DAMPER_ONOFF_FEEDBACK_IS_OK;
					/* Inizializzazione eseguita */
					CS_Status_Var.damperOnOffState.bit.firstTimeDamperNotPylot = 1;
				}

				if ( CS_Status_Var.damperCurrentPos != PM_Damper_ONOFF_GetReedVal() )
				{
					/* Controlla se la lettura precedente era O.K. */
					if ( CS_Status_Var.damperOnOffState.bit.cameFromInputOK == 1 )
					{
						/* No -> Clear filter for monitoring */
						CS_Status_Var.CS_LoadCnt[loadPos] = 0;
					}
					/* Flag condizione non corretta */
					CS_Status_Var.damperOnOffState.bit.cameFromInputOK = 0;

					/* filtro per eventuali disturbi */
					if ( CS_Status_Var.CS_LoadCnt[loadPos] < 50 )
					{
						/* non prendo decisioni sul valore del feedback -> rimango con il vecchio valore */
						CS_Status_Var.CS_LoadCnt[loadPos] ++;
					}
					else
					{
						/* il damper NON è pilotato e la posizione e' cambiata */
						CS_Status_Var.damperFeedbackValue = DAMPER_ONOFF_FEEDBACK_IS_KO;
					}
				}
				else
				{
					/* Controlla se la lettura precedente era O.K. */
					if ( CS_Status_Var.damperOnOffState.bit.cameFromInputOK == 0 )
					{
						/* Clear filter for monitoring */
						CS_Status_Var.CS_LoadCnt[loadPos] = 0;
					}
					/* Flag condizione corretta */
					CS_Status_Var.damperOnOffState.bit.cameFromInputOK = 1;

					/* filtro per eventuali disturbi */
					if ( CS_Status_Var.CS_LoadCnt[loadPos] < 50 )
					{
						/* non prendo decisioni sul valore del feedback -> rimango con il vecchio valore */
						CS_Status_Var.CS_LoadCnt[loadPos] ++;
					}
					else
					{
						CS_Status_Var.damperFeedbackValue = DAMPER_ONOFF_FEEDBACK_IS_OK;
					}
				}
				feedback_value = CS_Status_Var.damperFeedbackValue;
			}

			CS_ManageLoad(load_status, feedback_value, &res);

			CS_ManageRes(res, feedbackData.Data, loadPos);

			break;

#endif /*__DAMPER_ONOFF_H*/

#ifdef CONFIG_PM_DOORLOCK

		case BLOCCO_PORTA_IMPULSI:
		case BLOCCO_PORTA_CERA:
		case BLOCCO_PORTA_CERA_2PIN:

			if ( PM_DoorLock_GetStatus() == BPGEN_DEVICE_RUNNING )
			{
				CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
				continue; /* Exit From switch(pilotType) process next Load */
			}

			/* Filtraggio degli impulsi spuri e del Fault F6.01: allo   */
			/* startup si aspettano 5s prima di valutare il Consistency */
			if ( PowON_Delay )
			{
				PowON_Delay--;
				CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
				continue;
			}

			/*The mask to avoid the control of the config field*/

			/*NB:
			  When the define BLOCCOPORTA_ENABLE_ZC_FEEDBACK_CONTROL == 1 the
			  the fields of the Consistency table that must be filled are
			  0 (NOT SYNC) 1 (SYNC)
			*/
			/* Valore del feedback */
			feedback_value = CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0));

			CS_ManageLoad(load_status, feedback_value, &res);

			/*gets consistency value from PM_DoorLock module */
#if defined(CONFIG_PM_CS_RES_PLUS_STATUS)
                CS_ElemNewConsistency( loadPos, (uint8_t)(PM_DoorLock_InternalConsistency() | (uint8_t)(res.load_status<<3)) );
#else
                CS_ElemNewConsistency( loadPos, PM_DoorLock_InternalConsistency() );
#endif
			break;

#endif /* CONFIG_PM_DOORLOCK */

#ifdef CONFIG_PM_DRUMLOCK

		case DRUM_LOCK_L:

			if ( PM_DrumLock_DriverStatus() == DMLGEN_DEVICE_RUNNING )
			{
				CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );
				continue; /* Exit From switch(pilotType) process next Load */
			}

			/* Valore del feedback */
			feedback_value = CS_GetValue(FEEDBACK_READ_TYPE_PNTR(feedbackData.Data,0));

			CS_ManageLoad(load_status, feedback_value, &res);

			CS_ManageRes(res,feedbackData.Data,loadPos);

			break;

#endif /* CONFIG_PM_DRUMLOCK */

#if defined(PWM_LOAD_ENABLE) && (PWM_LOAD_ENABLE == 1)
		case PWM_L:

			/* [To be complete ...] */
			continue;

#endif /*(PWM_LOAD_ENABLE == 1)*/

		case I2C_L:

			continue;

		default:
			PLT_ERROR_WARNING(PLT_ERROR_CONSISTENCY_NOT_SUPPORTED);
			continue;
		}

	}

#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)
	if ( CS_Status_Var.pCS_Resource != NULL )
	{
		PM_Consistency_GetAllCsValues(CS_Status_Var.pCS_Resource);
	}
#endif
}

/**
Enables/Disables Consistency's work.

\param  [in]		status

\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0



\note		\par \b Nota 1:
			documentation by Stefano Ughi


*/
void PM_Consistency_SetModuleStatus(uint8_t status)
{
#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)

	if (status == PM_CS_STATUS_ENABLE)
	{
		CS_Status_Var.Status |= PM_CS_STATUS_ENABLE;
	}
	else
	{
		CS_Status_Var.Status &= ~(PM_CS_STATUS_ENABLE);
	}
#endif
}

/**
Returns CS_Status_Var.Status.


\attention \b CS_Status_Var.Status used as I

\return 	uint8_t


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t PM_Consistency_GetModuleStatus(void)
{
#if defined (CONFIG_PM_CONSISTENCY_APPL_BUFFER)
	return (CS_Status_Var.Status);
#else
	return 0;
#endif
}
/**
Returns consistency's value for the load identified by loadID.

\param  [in]	loadPos load's position inside Output Map
\param  [out]	*pCS_Value consistency's value

\attention \b CS_Status_Var used as I

\return 	uint8_t

\retval	    PM_CS_ERROR load not found
\retval     PM_CS_NO_ERROR load found and consistency's value in *pCS_Value

\author 	Alberto Camerin
\date		gg/mm/aaaa
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			*pCS_Value=PM_CS_NOT_VALID_CODE -> consistency not ready or load not found


*/
uint8_t PM_Consistency_IsValueReady( uint8_t loadPos, uint8_t *pCS_Value )
{

	uint8_t loadIndex;
	uint8_t *pByte;

	/* defaul value */
	*pCS_Value = PM_CS_NOT_VALID_CODE;

	loadIndex = LIB_IOM_FindLoad(loadPos,&pByte);
	if ( loadIndex == 0xFF )
	{
		return PM_CS_ERROR;
	}
	
	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( PLT_Consistency, loadIndex );
	*pCS_Value = PLT_Consistency[loadIndex];
	SAFETY_REGION_END();	

	return PM_CS_NO_ERROR;

}
/**
The function when find a change in the state of the load, resets reading of \n
feedback coupled to the load and raises the value of consistency to undefined.
Not for all types of loads!.

\param  [in]	loadPos load's position inside Output Map
\param  [in]    oldloadstatus current load's state.
\param  [in]    newloadstatus new load's state
\param  [in]	pWord pointer to the beginning of the field that describes the load
\param  [in]	pilottype type of pilot load

\attention \b CS_Status_Var used as I/O


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0


\todo		questa funzione ha troppi parametri in input, parametri che potrebbero essere\n
            ricavati internamente.


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
            Non e' stato possibile rinominare questa funzione perche' utilizzata da prodotti ibridi\n
            come Dryer e DEA700

*/
void PM_Consistency_LoadChangedStatus( uint8_t loadPos, uint8_t oldloadstatus, uint8_t newloadstatus, uint8_t* pByte, uint8_t pilottype )
{


	if (oldloadstatus != newloadstatus)
	{

		struct Loader RamData;

		CS_Status_Var.CS_LoadCnt[loadPos] = 0;

		switch (pilottype)
		{

#if defined(CONFIG_PM_LINKED_LOADS_SUPPORT)
		case RELAY_TRIAC_L:
#if defined(CONFIG_PM_LINKED_LOADS__RELAY_LOAD_SUPPORT)
		case RELAY_LOAD_L:
#endif
			break;
		case TRIACRELAY_L:
		case DIGITALDELAYED_L:

#endif /* __TRIAC_RELAY_MULTI_H */
		case DIGITAL_L:

#ifdef __TRIAC_RELAY_H
		case COMPTR_L:
#endif /*__TRIAC_RELAY_H */

		case ZC_L:
		case RELEZC_L:

#if defined(CONFIG_PM_MOTORGATE) /* ex  __UNIVERSAL_MOTOR_H */
		case COLLECTORMOTOR_L:
#endif
#ifdef CONFIG_PM_DIGITAL_OUTPUT_MULTI
		case DIGITAL_OUTPUT_MULTI_L:
#endif /* CONFIG_PM_DIGITAL_OUTPUT_MULTI */
#ifdef __PROFILER_H
		case PROFILE_L:
#endif /* __PROFILER_H */

#if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT )
		case PLP_L:
#endif /* #if defined( CONFIG_PM_PROFILE_LOADS_PARTIALIZATION_SUPPORT ) */


#if defined(CONFIG_PM_FIXEDSPEEDMOTOR)
		case FIXED_SPEED_MOTOR_L:
#endif
			/*
			    get puntatore ai parametri consistency
			    -*(RamData.Data+2) = Feedback ReadType
			    -*(RamData.Data+3) = Feedback Position all'interno del proprio vettore d'appartenenza
			*/

			if ( BSLDR_OK != BasicLoader( SF_PTR_IO_DEVICE, LOAD_FEEDBACKS_OFFSET(pByte), &RamData) )
			{
				/* No Consistency configuration present */
				return;
			}

			switch ( FEEDBACK_READ_TYPE_VALUE(RamData.Data,0) )
			{
			case FEEDBACK_I:
			case FEEDMULT_I:
				LIB_FDB_FeedbackInitMeasure( FEEDBACK_POSITION_VALUE(RamData.Data,0) );
				break;

#ifdef CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT
			case FREQFEEDBACK_I:
				PM_FrequencyFeedbacks_RestartFeedbackByPosition( FEEDBACK_POSITION_VALUE(RamData.Data,0) );
				break;
#endif
#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
			case ANALOGFEEDBACK_I:
				PM_AnalogFeedbackResetMeasure( FEEDBACK_POSITION_VALUE(RamData.Data,0) );
				break;
#endif /* CONFIG_PM_ANALOG_FEEDBACK */
			default:
				break;
			}

			CS_ElemNewConsistency( loadPos, PM_CS_NOT_VALID_CODE );

			break;
		default:
			break;
		}
	}
}



/* Local Function */
/**
Depending on the type of feedback and its position within the variables
that contain its value returns the value of the feedback itself.

\param  [in]    pFeedbackData pointer to the word that describes the Feedback's ReadType \n
                and its position within the variables that contain the read value. \n

\return 	uint8_t

\retval		feedback's value

\author 	Stefano Ughi
\date		28/02/2008
\version	1.0.0


\todo		nel caso di feedback non contemplato la funzione dovrebbe ritornare un valore \n
            superiore al valore massimo consentito per il feedback che, per come e' \n
            strutturata la tabella del consistency e' 3 e tale valore deve essere testato dalla \n
            chiamante per individuare eventuali condizioni errate.

\note		\par \b Nota 1:
			documentation by Stefano Ughi


*/

static uint8_t CS_GetValue(uint8_t *pFeedbackData)
{
	uint8_t feedbackType;
	uint8_t feedbackPos;
	uint8_t retVal = PM_CS_NOT_VALID_CODE;

	feedbackType = *pFeedbackData;
	feedbackPos = *(pFeedbackData + 1);

	switch (feedbackType){
	case DIGITAL_I:
	case DIGMULT_I:
		retVal = (uint8_t) ((DigInputs & ((dig_input_t) 1 << feedbackPos)) ? 1 : 0);
		break;

	case ANALOG_I:
		/*TODO: da implementare !! */
		retVal = PM_CS_NOT_VALID_CODE;
		break;

	case FEEDBACK_I:
	case FEEDMULT_I:
		retVal = (LIB_FDB_GetFeedbackVal(feedbackPos));
		break;

#ifdef CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT
	case FREQFEEDBACK_I:
	{
		PM_FrequencyFeedbacks_ValueType feedbackVal;
		if (PM_FrequencyFeedbacks_GetValueByPosition(feedbackPos, &feedbackVal) == FREQFB_OK)
		{
			return feedbackVal;
		}
	}
		break;
#endif /* CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT */

#ifdef CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT
		case ANALOGFEEDBACK_I:
		retVal = ( PM_AnalogFeedbackGetValue( feedbackPos ) );
		break;
#endif /* CONFIG_PM_ANALOG_FEEDBACK */

	case ZEROCROSSING_I:
		retVal = goodZCSignal();
		break;

	default:
		PLT_ERROR_FATAL(PLT_ERROR_CONSISTENCY_NOT_SUPPORTED);
		break;
	}

	return retVal;
}


/**
Depending on the load status and feedback's value prepares the "res" variable \n
in order to access to consistency's table.

\param  [in]		load_status modified load's status
\param  [in]		feedback_val feedback's value (max. value is 3)


\return 	LoadStatusRes_t


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi


*/

static void CS_ManageLoad( uint8_t load_status, uint8_t feedback_val, LoadStatusRes_t *res )
{

	res->load_status = load_status;

	if (load_status == CS_LOAD_ACTIVE)
	{
		res->cs_code = (uint8_t)(CS_LOAD_STATUS_DISPLACEMENT_PILOT+feedback_val);
	}
	else if ( ( load_status == CS_LOAD_NOT_ACTIVE ) || ( load_status == CS_LOAD_FFC_ONLY_RELAY_ACTIVE) || ( load_status == CS_LOAD_FAN_HIGH_SPEED) )
	{
		res->cs_code = (uint8_t)(CS_LOAD_STATUS_DISPLACEMENT_NOT_PILOT+feedback_val);
	}
	else
	{
		res->cs_code = (uint8_t)(CS_LOAD_STATUS_DISPLACEMENT_FFC_NOT_PILOT+feedback_val);
	}

}

/**
Depending from load's status and feedback's value returns a code inside consistency's table.
\n\n\n

case CONFIG_PM_CS_RES_PLUS_STATUS = TRUE
<TABLE>

	<TR>
		<th> 15  	</th>
		<th> 14-12  </th>
		<th> 11   	</th>
		<th> 10-8 	</th>
		<th> 7  	</th>
		<th> 6-4 	</th>
		<th> 3 		</th>
		<th> 2-0 	</th>
	</TR>

	<TR>
		<TD> 0 			</TD>
		<TD> P_CODE_1	</TD>
		<td> 0  		</td>
		<td> P_CODE_2	</td>
		<td> 0  		</td>
		<td> P_CODE_3 	</td>
		<td> 0  		</td>
		<td> P_CODE_4   </td>
	</TR>

	<TR>
		<td> 0 			</td>
		<td> NP_CODE_1 	</td>
		<td> 0 			</td>
		<td> NP_CODE_2	</td>
		<td> 0 			</td>
		<td> NP_CODE_3	</td>
		<td> 0 			</td>
		<td> NP_CODE_4	</td>
	</TR>
	<tr>
		<td> -	</td>
		<td> 0 	</td>
		<td> -	</td>
		<td> 1	</td>
		<td> -	</td>
		<td> 2 	</td>
		<td> -	</td>
		<td> 3 	</td>
	</tr>
</TABLE>
\n\n\n\n
case CONFIG_PM_CS_RES_PLUS_STATUS undefined

<TABLE>
	<tr>
		<th> 15-12 </th>
		<th> 11-8 </th>
		<th> 7-4 </th>
		<th> 3-0 </th>
	</tr>
	<tr>
		<td> P_CODE_1 </td>
		<td> P_CODE_2 </td>
		<td> P_CODE_3 </td>
		<td> P_CODE_4 </td>
	</tr>
	<tr>
		<td> NP_CODE_1 </td>
		<td> NP_CODE_2 </td>
		<td> NP_CODE_3 </td>
		<td> NP_CODE_4 </td>
	</tr>
	<tr>
		<td> 0	</td>
		<td> 1	</td>
		<td> 2	</td>
		<td> 3	</td>
	</tr>
</TABLE>


\n P_CODE_X:  table for the load piloted
\n NP_CODE_X: table for the load not piloted

\param  [in]		res structure of LoadStatusRes_t type
\param  [in]		pFeedBackData pointer to consistency's table
\param  [in]	    loadPos load's position inside output map

\attention \b CS_Status_Var.pCS_Resource used as I/O


*/

static void CS_ManageRes( LoadStatusRes_t res, uint8_t *pFeedBackData, uint8_t loadPos )
{
	uint8_t retcode;
	uint8_t *auxptr;

	/* Punto all'inizio della struttura NP_CODE_X */
	auxptr = (uint8_t*)((uint16_t*)((void*)pFeedBackData) + *(pFeedBackData+1) + 1);
	/* A seconda dello stato carico: CS_LOAD_ACTIVE,CS_LOAD_NOT_ACTIVE or CS_LOAD_FFC_NOT_ACTIVE
	   punto alla word contenente i codici di ritorno per i diversi valori di feedback */
	auxptr += 2*((res.cs_code>>2)&0x03);
	/* A seconda del valore del feedback 0,1,2,3 punto al byte alto o al byte basso */
	auxptr += ((res.cs_code>>1) & 0x01);
	/* A seconda del valore del feedback 0,1,2,3 ricavo il nibble contenente il codice operativo
	   da ritornare */
	/*lint -e504 shift verificato */
	retcode = (uint8_t)(((*(auxptr)) >> (4*((res.cs_code&0x01)^1))) & 0x0F);
	/*lint +e504 */

#if defined(CONFIG_PM_CS_RES_PLUS_STATUS)

	if (res.load_status == CS_LOAD_FFC_NOT_ACTIVE)
	{
		res.load_status = CS_LOAD_NOT_ACTIVE;
	}
	CS_ElemNewConsistency( loadPos, (uint8_t)(retcode | (uint8_t)(res.load_status<<3)) );

#else

	CS_ElemNewConsistency( loadPos, retcode );

#endif

}

/**
Store the new consistency value of the selected load in the related element \n
of pCS_Resource array

\param  [in]		array_ind: index of element into pCS_Resource array
\param  [in]		new_CS_value: new consistency value of array_ind-th element


\return 	none


\author 	xyz
\date		gg/mm/aaaa
\version	1.0.0

\note		\par \b Nota 1:
			documentation by xyz


*/

static void CS_ElemNewConsistency( uint8_t array_ind, uint8_t new_CS_value )
{
  SAFETY_REGION_START();
  SAFETY_DATA_ELEM_CHECK( PLT_Consistency, array_ind );

  PLT_Consistency[array_ind] = new_CS_value;

  SAFETY_DATA_ELEM_UPDATE( PLT_Consistency, array_ind );
  SAFETY_REGION_END();
}

/**
Returns all consistency values in pConsistency array

\param  [out]		pConsistency: array to store consistency value, its size has to be N_MAXLOADS bytes.

\return 	none


\author 	Alessandro Sabbatini
\date		07/03/2013
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini

*/

void PM_Consistency_GetAllCsValues(uint8_t *pConsistency)
{
	memcpy(pConsistency, PLT_Consistency, sizeof(PLT_Consistency));

}


#endif /* CONFIG_PM_CONSISTENCY */
/* *********** */
/* End of file */
/* *********** */

