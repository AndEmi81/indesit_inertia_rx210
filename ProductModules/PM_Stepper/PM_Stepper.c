/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_Stepper
@{
\n
\n      This module implements all functions necessaries to drive a bipolar stepper.
\n      The device is used by no frost fridges.
\n
\par    Configuration:
\n
\n\li   \b T_CALL_STEPPER_MANAGER
\n      used to define the step chanching time
\n\li   \b DEAD_TIME_DELAY
\n      bridge dead time
\n
\par    Exported Functions:
\n
\n\li   PM_Stepper_Init()
\n\li   PM_Stepper_InitParams()
\n\li   PM_Stepper_SetLogicalPosition()
\n\li   PM_Stepper_Refresh()
\n\li   PM_StepperIsMoving()
\n\li   PM_Stepper_Manager()
\n\li   PM_Stepper_GetLastSeqApplied()
\n\li   Stepper_GetPinsConf()
\n\li   Stepper_Chk_If_Init_Device()
\n\li   StepperGetFreqStepVariation()
\n\li   StepperIsTimeToControl()
\n\li   StepperSetVarsForInit()
\n\li   StepperGetNStepsForInit()
\n\li   StepperApplySeq()
\n\li   StepperApplySmallDelay()
\n\li   StepperMantainPositionWithoutPower()
\n\li   StepperGetNumberOfStepsToReachNewPosition()
\n\li   StepperGetReg()
\n\li   StepperGetInfoStepsTab()
\n
@}
\ingroup   	ProductM
*/
/**
\n
\par		PM_Stepper.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_Stepper.c
\ingroup	PM_Stepper
\date		Nov 23, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Nov 23, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include <stdlib.h>
#include "StandardTypes.h"
#include "StandardDefines.h"
#include "PM_Stepper.h"
#include "PM_StepperExp.h"
#include "DigitalOutputs.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** control's sequence: phase A position*/
#define A_MASK      BIT3_MASK
/** control's sequence: phase _A position*/
#define _A_MASK     BIT2_MASK
/** control's sequence: phase B position*/
#define B_MASK      BIT1_MASK
/** control's sequence: phase _B position*/
#define _B_MASK     BIT0_MASK

/** Time function PM_Stepper_Manager() call [msec] used to define the step chanching time.*/
#define T_CALL_STEPPER_MANAGER  10u
/** Dead Time (HCS08 machine cycles = 19+9xDEAD_TIME_DELAY) */
#define DEAD_TIME_DELAY         (uint8_t)1u
/** Offset in bytes of the field within is stored the number of sequences, inside Pylot Type Params table,*/
#define OFFSET_N_SEQ        5u
/** Number of sequences bits asigned */
#define MASK_FIELD_N_SEQ    0x0F
/** Offset in bytes of the field within is stored Seq0-Seq1, inside Pylot Type Params table,*/
#define OFFSET_SEQ          6u
/** Offset in bytes of the field within is stored I flag (cfr. Setting File Doc), inside Pylot Type Params table,*/
#define OFFSET_I            5u
/** I bit mask */
#define STEPPER_I_MASK      BIT4_MASK
/** Offset in bytes of the field within is stored Steps Adjust (cfr. Setting File Doc), inside Pylot Type Params table,*/
#define STEPPER_OFFSET_ADJ  4u
/** Offset in bytes of the field within is stored Stepper Speed in p.p.s.(cfr. Setting File Doc), inside Pylot Type Params table,*/
#define STEPPER_VEL_OFFSET  2u
/** Offset in bytes of the field within is stored P flag (cfr. Setting File Doc), inside Pylot Type Params table,*/
#define STEPPER_P           5u
/** P bit mask */
#define STEPPER_P_MASK      BIT5_MASK
/** n° of fixed words, inside Pylot Type Params table (cfr. Setting File Doc) */
#define STEPPER_N_FIX_WORDS_PYLOT_PARAMS    3u
/** Error code for regulation not found, admissibles regulations are from 0 to 15 */
#define STEPPER_REG_NOT_FOUNDED (uint8_t)0xFFu
/** Regulation's value used to disconnect H bridge */
#define STEPPER_STOP_PYLOT  0u


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Struct in order to define starting pointer to Step_Pos0 and n° of
Step_PosX, (cfr. Setting File Doc)  */
typedef struct
{
	/** Step_Pos0 pointer*/
	uint16_t *pStep_Pos0;
	/** number of Step_PosX = number of admissibles regulations */
	uint8_t nStep_Posx;
} stepPosX_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */
/** Variable used to manage device */
stepperLoadStruct_t StepperLoad;

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static uint8_t Stepper_GetPinsConf( uint16_t step );
static bool_t Stepper_Chk_If_Init_Device(void);
static void StepperSetVarsForInit(void);
static uint8_t StepperGetFreqStepVariation(void);
static bool_t StepperIsTimeToControl(void);
static void StepperApplySeq( uint8_t seq );
static void StepperApplySmallDelay(uint8_t delay);
static bool_t StepperMantainPositionWithoutPower(void);
static uint16_t StepperGetNumberOfStepsToReachNewPosition( uint8_t reg );
static stepPosX_t StepperGetInfoStepsTab(void);
static uint16_t StepperGetNStepsForInit(void);
static uint8_t StepperGetReg( uint16_t stepCurrent );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Module Initialization, call this function at "cold" reset if SystemMode=SYSTEM_NORMAL_MODE.

\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void PM_Stepper_Init(void)
{
	(void)memset(&StepperLoad, 0, sizeof(StepperLoad));
}
/**
This function provides data initialization for the device,
\n Called inside LIB_IOM_OutputInitialization() at "cold" reset

\param  [in]	pPinsComponent pointer to the pins used to drive the device.
\param  [in]	pylotParams pointer to the device configuration table inside setting file plus data lenght


\return 	stepperError_t

\retval		STEPPER_E_OK initialization done.

\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
stepperError_t PM_Stepper_InitParams( uint8_t *pPinsComponent,struct Loader pylotParams )
{

	StepperLoad.pylotParams = pylotParams;
	StepperLoad.pPinsDevice = pPinsComponent;
	StepperLoad.data.divisor = StepperGetFreqStepVariation();
	StepperLoad.data.numberOfSeq = (uint8_t)(*(StepperLoad.pylotParams.Data + OFFSET_N_SEQ)&MASK_FIELD_N_SEQ);

	if (Stepper_Chk_If_Init_Device())
	{
		/* Setup Variables for device Init. */
		StepperSetVarsForInit();
	}
	/* N.B.: If the device don't needs initialization
	StepperLoad.stepCurr = StepperLoad.stepTo = 0 */

	/* Initialization done */
	StepperLoad.data.state.flag.initModule = 1;
	return STEPPER_E_OK;
}

/**
This function checks the conditions to apply the new regulation.
\n This function is called by LIB_IOM_SetLoad().

\param  [in]	regNew new regulation's value.

\return 	stepperError_t

\retval		STEPPER_E_OK no problem
\retval		STEPPER_E_KO  problem


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\todo       con comando di Stop
            da esaminare il caso di dispositivi che debbono essere
            permanentemente alimentati per tenere la posizione.
            Hanno una molla di ritorno che se disalimentati vanno a
            finire in una posizione nota ?

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

stepperError_t PM_Stepper_SetLogicalPosition( uint8_t regNew )
{
	uint8_t regNow;

	if ( !StepperLoad.data.state.flag.initModule )
	{
		/* Module not initialized */
		return STEPPER_E_KO;
	}
	/* Manager enabled */
	StepperLoad.data.state.flag.stopManager = 0;
	if ( regNew == STEPPER_STOP_PYLOT )
	{
		/* disalimento il ponte, per i dispositivi che mantengono
		   la posizione anche se disalimentati non ci sono problemi
		   per quelli che non mantengono la posizione ipotizzo che tornino
		   ad una posizione nota */
		StepperApplySeq((uint8_t)0);
		if ( !StepperMantainPositionWithoutPower())
		{
			StepperLoad.data.stepCurr = 0;
		}

		/* Disable Manager Activities */
		StepperLoad.data.state.flag.stopManager = 1;
		/* Flag Device Not Moving */
		StepperLoad.data.state.flag.deviceIsMoving = 0;
		return STEPPER_E_OK;
	}
	if ( StepperLoad.data.state.flag.deviceIsMoving )
	{
		/* Device in movement */
		return STEPPER_E_KO;
	}
	/* Device steady */
	if ( Stepper_Chk_If_Init_Device() == TRUE )
	{
		/* Device need initialization */
		if ( StepperLoad.data.stepCurr == StepperGetNStepsForInit() )
		{
			/* Exit from initialization procedure */
			/* Apply the requested regulation */
			StepperLoad.data.stepCurr = 0;
			/* regNew consistency is checked by LIB_IOM_SetLoad() */
			StepperLoad.data.stepTo = StepperGetNumberOfStepsToReachNewPosition( regNew );
			return STEPPER_E_OK;
		}
		/* Dispositivo fermo e non sono uscito dalla procedura d'inizializzazione,
		   sono quindi nel funzionamento a regime */
		regNow = StepperGetReg(StepperLoad.data.stepTo);
		if ( regNow == STEPPER_REG_NOT_FOUNDED )
		{
			/* Non ho trovato nessuna regolazione corrispondente al passo
			corrente del dispositivo, possono essere accadute 2 cose:
			a) mi sono perso il passo.
			b) sono stato stoppato mentre eseguivo l'inizializzazione.
			N.B.: C'è il vincolo che nessuno degli Step_PosX deve essere
			uguale alla somma di Numero Steps Totali + N° Steps Adjust
			Reinizializzo il dispositivo */
			StepperSetVarsForInit();
			return STEPPER_E_KO;
		}
		else
		{
			/* N.B.: se regNow == regNew allora
			StepperLoad.stepTo == StepperLoad.stepCurr e il Manager()
			non farà niente */
			StepperLoad.data.stepTo = StepperGetNumberOfStepsToReachNewPosition( regNew );
			return STEPPER_E_OK;
		}
	}
	else
	{
		/* Il dispositivo non doveva essere inizializzato
		   dispositivo con molla di ritorno ?  */
		StepperLoad.data.stepTo = StepperGetNumberOfStepsToReachNewPosition( regNew );
		return STEPPER_E_OK;
	}
}
/**
This function initializes variables for refresh procedure.
\n The Refresh procedure consist in apply the initialization procedure.

\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void PM_Stepper_Refresh(void)
{

	if ( !StepperLoad.data.state.flag.initModule )
	{
		/* modulo non inizializzato */
		return;
	}
	/* Non mi interessa se il dispositivo si sta muovendo
	forzo l'inizializzazione del dispositivo, ci penserà la
	LIB_IOM_SetLoad() a portarlo, al termine dell'inizializzazione, alla posizione voluta */
	StepperSetVarsForInit();

}

/**
This function returns TRUE if device is moving otherwise FALSE.

\return 	bool_t

\retval		TRUE -> device is moving
\retval		FALSE -> device steady


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_StepperIsMoving(void)
{
	if (StepperLoad.data.state.flag.deviceIsMoving)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
This is the Manager of the device.
\n We suppose that this function is called every 10 msec. If you change time's call please adjust
\n T_CALL_STEPPER_MANAGER parameter.

\return 	stepperError_t

\retval		STEPPER_E_OK no problem
\retval		STEPPER_E_KO problem encountered


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

stepperError_t PM_Stepper_Manager(void)
{
	uint8_t seq;
	uint8_t old_seq;

	/* Controlla se le inizializzazioni sono state eseguite */
	if ( !StepperLoad.data.state.flag.initModule )
	{
		return STEPPER_E_KO;
	}
	/* Controlla se è arrivato un comando di Stop */
	if ( StepperLoad.data.state.flag.stopManager == 1 )
	{
		/* segnalo dispositivo non in movimento */
		StepperLoad.data.state.flag.deviceIsMoving = 0;
		return STEPPER_E_OK;
	}
	/* Controlla se c'è qualcosa da fare */
	if ( StepperLoad.data.stepCurr == StepperLoad.data.stepTo )
	{
		/* niente da fare */
		if (StepperMantainPositionWithoutPower())
		{
			/* disalimento il ponte */
			StepperApplySeq((uint8_t)0);
		}
		/* Flag dispositivo fermo */
		StepperLoad.data.state.flag.deviceIsMoving = 0;
		return STEPPER_E_OK;
	}

	/* Flag dispositivo in fase di posizionamento */
	StepperLoad.data.state.flag.deviceIsMoving = 1;
	if ( StepperIsTimeToControl() == FALSE )
	{
		return STEPPER_E_OK;
	}

	/* E' tempo di applicare il nuovo step */


	/* In base al passo ricavo la sequenza da applicare ai pins
	   di controllo del dispositivo */
	seq = Stepper_GetPinsConf(StepperLoad.data.stepCurr);

	if ( StepperLoad.data.stepCurr < StepperLoad.data.stepTo )
	{
		/* Vecchia sequenza applicata */
		old_seq = Stepper_GetPinsConf((uint16_t)(StepperLoad.data.stepCurr-1u));
		/* Preparazione nuovo passo*/
		StepperLoad.data.stepCurr++;
	}
	else
	{
		/* Vecchia sequenza applicata */
		old_seq = Stepper_GetPinsConf((uint16_t)(StepperLoad.data.stepCurr+1u));
		/* Preparazione nuovo passo*/
		StepperLoad.data.stepCurr--;
	}
	/* Controllo se c'è inversione di fase: corrente che prima fluiva da A a _A
	   ora deve fluire da _A a A o viceversa */
	if ( ( ( seq & (A_MASK|_A_MASK) ) ^ ( old_seq & (A_MASK|_A_MASK) ) ) == (A_MASK|_A_MASK) )
	{
		/* Disalimento la fase A _A altrimenti rischio il corto circuito */
		/*lint -e{917} prototype coercion int to unsigned char */
		StepperApplySeq((uint8_t)(old_seq & (B_MASK|_B_MASK)));
		/* Dead time: tutti i transistor del ponte debbono essere \n
		effettivamente spenti */
		StepperApplySmallDelay(DEAD_TIME_DELAY);
	}
	/* Controllo se c'è inversione di fase: corrente che prima fluiva da B a _B
	   ora deve fluire da _B a B o viceversa  */
	if ( ( ( seq & (B_MASK|_B_MASK) ) ^ ( old_seq & (B_MASK|_B_MASK) ) ) == (B_MASK|_B_MASK) )
	{
		/* Disalimento la fase B _B altrimenti rischio il corto circuito */
		/*lint -e{917} prototype coercion int to unsigned char */
		StepperApplySeq((uint8_t)(old_seq & (A_MASK|_A_MASK)));
		/* Dead time: tutti i transistor del ponte debbono essere \n
		effettivamente spenti */
		StepperApplySmallDelay(DEAD_TIME_DELAY);
	}

	/* Applico la sequenza di controllo */
	StepperApplySeq(seq);
	return STEPPER_E_OK;
}
/**
This function returns the last sequence applied to the device.
\n The sequence's pattern is [0,0,0,0,A,_A.B,_B].

\return 	uint8_t

\retval		StepperLoad.lastSeqApplied


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint8_t PM_Stepper_GetLastSeqApplied( void )
{
	return ( StepperLoad.data.lastSeqApplied );
}

/* Local Function */
/**
Starting from step's value decides the values to apply to the Stepper's pins.

\param  [in]    step step to perform

\return 	uint8_t

\retval		seq values to apply to the Stepper's pins


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static uint8_t Stepper_GetPinsConf( uint16_t step )
{
#if !defined (DEBUG_STEPPER)
	uint8_t indexSeq;
	uint8_t seq;
#else
	static uint8_t indexSeq;
	static uint8_t seq;
#endif

	/*lint -e{912} */
	indexSeq = (uint8_t)(step%StepperLoad.data.numberOfSeq);

	seq = *(StepperLoad.pylotParams.Data + OFFSET_SEQ + (indexSeq/2));

	if (indexSeq%2)
	{
		seq&=0x0F;
	}
	else
	{
		seq>>=4;
	}
	/* check sequence's coerency  */
	if ( ( ( seq&(A_MASK|_A_MASK) ) == (A_MASK|_A_MASK) ) ||
	        ( ( seq&(B_MASK|_B_MASK) ) == (B_MASK|_B_MASK) ) )
	{
		/* configuration is not admissible -> safe mode */
		seq = 0;
	}
	return seq;
}

/**
This function checks if the device must be initialized.
\n Device initialization consists in performing a numbers of steps equal to
\n number of total steps plus number of Steps Adjust (Cfr. Setting File Doc.)
\n In this manner the device reach a fixed position dependent from applied sequence..

\return 	bool_t

\retval		FALSE No Initialization requested
\retval		TRUE Device must be initialized


\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static bool_t Stepper_Chk_If_Init_Device(void)
{
	if (*(StepperLoad.pylotParams.Data + OFFSET_I)&STEPPER_I_MASK)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
This function determines the rate of change of the step according to the time of the call of
\n PM_Stepper_Manager () and speed required in the settings file.


\return 	uint8_t


\author 	Stefano Ughi
\date		24/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			We suppose stepperVel > 0 (check performed by Editor)

*/

static uint8_t StepperGetFreqStepVariation(void)
{
#if !defined (DEBUG_STEPPER)
	/* velocità di pilotaggio dello stepper in passi per secondo */
	uint16_t stepperVel;
	/* indica ogni quante volte debbo chiamare il Manger prima di cambiare passo */
	uint16_t freqVarStep;
#else
	static uint16_t stepperVel;
	static uint16_t freqVarStep;
#endif

#if !defined (DEBUG_STEPPER)
	/*lint -e{826} pointer to pointer conversion verified */
	stepperVel = *(uint16_t*)(StepperLoad.pylotParams.Data + STEPPER_VEL_OFFSET);
#endif

	freqVarStep = (1000u/(stepperVel*T_CALL_STEPPER_MANAGER));

	if ( !freqVarStep )
	{
		/* velocità di pilotaggio dello stepper troppo alta rispetto alla frequenza di chiamata del Manager
		   -> fisso la massima possibile */
		freqVarStep = 1;
	}
	else if ( freqVarStep > 255u )
	{
		/* velocità di pilotaggio dello stepper troppo bassa -> fisso la minima possibile */
		freqVarStep = 255u;
	}
	else
	{
		/* do nothing */
	}
	return ((uint8_t)freqVarStep);
}
/**
This function decides if time to change the step.

\return 	bool_t

\retval		TRUE It's time to change the step
\retval		FALSE Keep the current configuration

\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static bool_t StepperIsTimeToControl(void)
{

	StepperLoad.data.cntrControl++;

	if ( StepperLoad.data.cntrControl >= StepperLoad.data.divisor )
	{
		StepperLoad.data.cntrControl = 0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/**
Variables setup for device initialization in order to bring the stepper in a known position.

\author 	Stefano Ughi
\date		23/11/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			This note, for me, is too difficult to translate
			si osservi che ponendo StepperLoad.stepCurr = (StepperLoad.stepTo*2)\n
		    e ipotizzando che le sequenze di pilotaggio siano state poste \n
		    in modo tale che a StepperLoad.stepCurr crescenti lo stepper si\n
		    muova in senso antiorario mi vado a muovere in senso orario.
*/

static void StepperSetVarsForInit(void)
{
	StepperLoad.data.stepTo = StepperGetNStepsForInit();
	StepperLoad.data.stepCurr = (uint16_t)(StepperLoad.data.stepTo*2);

	StepperLoad.data.state.flag.deviceIsMoving = 1;
}
/**
This function returns number of steps necessaries for device initialization.

\return 	uint16_t

\retval		stepsForInit


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static uint16_t StepperGetNStepsForInit(void)
{
	uint16_t stepsForInit;

	/*lint -e912 -e826 il controllo che la somma entri in un uint_16 è fatta da Editor */
	stepsForInit = (uint16_t)(*(uint16_t*)StepperLoad.pylotParams.Data + *(StepperLoad.pylotParams.Data+STEPPER_OFFSET_ADJ));
	/* lint +e912 +e826 */
	return (stepsForInit);
}
/**
This function applies to H bridge the input sequence.

\param  [in]    seq sequence to apply

\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.1


\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			*(StepperLoad.pPinsDevice+0) ->  A \n
            *(StepperLoad.pPinsDevice+1) -> _A \n
            *(StepperLoad.pPinsDevice+2) ->  B \n
            *(StepperLoad.pPinsDevice+3) -> _B \n
            \par \b Nota 3:
            StepperLoad.lastSeqApplied used by Consistency Module

*/
static void StepperApplySeq( uint8_t seq )
{
#if !defined(DEBUG_STEPPER)
	uint8_t i;
	uint8_t mask;
	uint8_t pinState;
#else
	static uint8_t i;
	static uint8_t mask;
	static uint8_t pinState;
#endif

	/* applico la sequenza
	seq = [0,0,0,0,A,_A.B,_B] */

	mask = 0x08;
	for ( i = 0; i < 4; i++ )
	{
		if (mask&seq)
		{
			pinState = (uint8_t)ON;
		}
		else
		{
			pinState = (uint8_t)OFF;
		}
		LIB_DIO_SET_LOAD(pinState, *(StepperLoad.pPinsDevice+i));
		mask>>=1;
	}
	StepperLoad.data.lastSeqApplied = seq;
}

/**
This function applies the dead time in order to avoid short circuit.

\param  [in]	delay to apply


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0

\todo       This delay is target dependent because of bus frequency and machine cycles of this routine.\n
            It's high reccomended to implement an assembly routine.

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static void StepperApplySmallDelay(uint8_t delay)
{
	uint8_t i;

	for ( i = 0; i < delay; i++ )
	{

	}
}

/**
This function verifies if the device is able to mantain position without power.

\return 	bool_t

\retval		TRUE -> device is able to mantain position without power.
\retval		FALSE -> device must be powered in order to mantain it's position.


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static bool_t StepperMantainPositionWithoutPower(void)
{
	if (*(StepperLoad.pylotParams.Data + STEPPER_P)&STEPPER_P_MASK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
This function,starting from regulation, returns the numbers of step to perform in order to reach\n
the desired position.

\param  [in]	reg regulation's value


\return 	uint16_t

\retval		number of steps


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static uint16_t StepperGetNumberOfStepsToReachNewPosition( uint8_t reg )
{
	stepPosX_t stepsTab;

	stepsTab = StepperGetInfoStepsTab();
	return(*(stepsTab.pStep_Pos0 + reg));
}

/**
This function, starting from stepCurrent, returns the associated regulation.

\param  [in]	stepCurrent current step.

\return 	uint8_t

\retval		reg if regulation found.
\retval     STEPPER_REG_NOT_FOUNDED if regulation not found.


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static uint8_t StepperGetReg( uint16_t stepCurrent )
{
	uint8_t reg;
	stepPosX_t stepsTab;

	stepsTab = StepperGetInfoStepsTab();
	for ( reg = 0; reg < stepsTab.nStep_Posx; reg ++ )
	{
		if ( *(stepsTab.pStep_Pos0 + reg) == stepCurrent )
		{
			return reg;
		}
	}
	return (STEPPER_REG_NOT_FOUNDED);
}

/**
This function return the pointer to Step_Pos0 and how many Step_PosX are inside configuration table\n
(Cfr. Setting File Doc.)


\return 	nFieldsSteps typedef stepPosX_t


\author 	Stefano Ughi
\date		24/10/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static stepPosX_t StepperGetInfoStepsTab(void)
{
	div_t divResult;
	stepPosX_t nFieldsSteps;

	/* ricavo il n° di words all'interno delle quali sono memorizzate le sequenze */
	/*lint -e{917} StepperLoad.numberOfSeq is not int */
	divResult = div(StepperLoad.data.numberOfSeq,4);
	if (divResult.rem)
	{
		divResult.quot++;
	}
	/* puntatore a Step-Pos0 cfr. setting file */
	nFieldsSteps.pStep_Pos0 = ((uint16_t*)StepperLoad.pylotParams.Data + STEPPER_N_FIX_WORDS_PYLOT_PARAMS + divResult.quot);
	/* n° di regolazioni */
	nFieldsSteps.nStep_Posx = (uint8_t)(StepperLoad.pylotParams.DataLenght - (STEPPER_N_FIX_WORDS_PYLOT_PARAMS + (uint16_t)divResult.quot));
	return (nFieldsSteps);
}

/* *********** */
/* End of file */
/* *********** */






