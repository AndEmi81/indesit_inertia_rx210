/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_AnalogFeedback.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		PM_AnalogFeedback.c
\ingroup	PM_AnalogFeedbacks
\date		Jul 19, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Jul 19, 2010
\n		stefano.ughi
\n		changes
\n 		- creation
\n
*/

#define	MODULE_BODY_PM_ANALOGFEEDBACK
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if  defined(CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT)
#include "Analog.h"
#include "PM_AnalogFeedback.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** maschera relativa alla soglia A/D sulla word contenente soglia \n
e valore dell'analog feedback corrispondente */
#define ANALOG_FEEDBACK_FIELD_THRESHOLD_MASK    0x03FFu
/** maschera relativa al valore dell'analog feedback sulla word contenente soglia \n
e valore dell'analog feedback corrispondente */
#define ANALOG_FEEDBACK_FIELD_VALUE_MASK        (0xFFFFu^ANALOG_FEEDBACK_FIELD_THRESHOLD_MASK)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Analog Feedback channel config. Setting file params */
typedef union analogFeedbackLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic:  1;  /* Automatic Readings */
		uint8_t unused_0:   4;
		uint8_t nRead:      3;  /* Real number of samples is 2^nRead */
		/* Low Byte */
		uint8_t unused_1:   2;  /* the channel is tagged as V line */
		uint8_t comp:       1;  /* if '1' measure must be compesated with V ref measure */
		uint8_t chDisch:    1;  /* if '1' read V line channel before reading the channel */
		uint8_t fal:        4;  /* Filter validation of reading (real number is 2^fal) */
#else
		/* High Byte */
		uint8_t nRead:      3;  /* Real number of samples is 2^nRead */
		uint8_t unused_0:   4;
		uint8_t automatic:  1;  /* Automatic Readings */
		/* Low Byte */
		uint8_t fal:        4;  /* Filter validation of reading (real number is 2^fal) */
		uint8_t chDisch:    1;  /* if '1' read V line channel before reading the channel */
		uint8_t comp:       1;  /* if '1' measure must be compesated with V ref measure */
		uint8_t unused_1:   2;
#endif
	} bit;

} analogFeedbackLLIparams_t;

/** dim counter for new analog read */
typedef uint8_t AnalogFeedbackCntr_t;

/** struttura a bit per gestire il feedback analogico i-esimo */
typedef struct
{
	/** se '1' feedback analogico abilitato */
	uint8_t analogFeedbackEn:1;
	/** se '1' reinizializzare misura analog feedback per transizione stato carico */
	uint8_t restartMeasure:1;

} AnalogFeedbackStruct_t;
/** accesso a byte o a bit per variabili di tipo AnalogFeedbackStruct_t  */
typedef union
{
	uint8_t all;
	AnalogFeedbackStruct_t bit;

} AnalogFeedbackUnion_t;

/** struttura complessiva per gestire il feedback analogico i-esimo */
typedef struct
{
	/** data pointer to analog feedback */
	pAnalogFeedbackData_t pAnalogFeedbackData;
	/** flags */
	AnalogFeedbackUnion_t flags;
	/** counter delay for a new read when load change state */
	AnalogFeedbackCntr_t analogFeedbackCntr;
	/** ADC channel linked to Analog Feedback */
	uint8_t adCh;
} AnalogFeedback_t;


/* ********* */
/* Variables */
/* ********* */

/* Global */
/** Valore dell'analog feedback */
analogFeedbackValue_t AnalogFeedbackValue[CONFIG_PM_ANALOG_FEEDBACKS_NUM];
/** If '1' Analog Feedback in position pos is ready */
analogFeedbackInfoCmd_t AnalogFeedbackReady;
/** If '1' Analog Feedback in position pos is disabled */
analogFeedbackInfoCmd_t AnalogFeedbackDisable;

/** Vettore gestione modulo */
static AnalogFeedback_t AnalogFeedbackVars[CONFIG_PM_ANALOG_FEEDBACKS_NUM];
/** bitmap indicante gli analog feedback inizializzati (position dependent) */
static analogFeedbackInfoCmd_t AnalogFeedbackInitializedMask;
/** bitmap stato dei feedback */
analogFeedbackInfoCmd_t InternalAnalogFeedbackReady;
/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static analogFeedbackValue_t AnalogFeedbackSearchValue( uint8_t posAnalogFeedback, ADC_ValueType analogMeasure );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n Module's initialization, call this function regardless reset type
\n if SystemMode = SYSTEM_NORMAL_MODE.

\attention \b AnalogFeedbackValue[] used as I/O
\attention \b AnalogFeedbackReady used as I/O
\attention \b AnalogFeedbackDisable used as I/O by External Tools


\author 	Stefano Ughi
\date		03/03/2008
\version	1.0.1

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			13/10/2008 added init. of AnalogFeedbackInitializedMask var.

*/

void PM_AnalogFeedback_Init( void )
{
	uint8_t i;

	for ( i = 0; i < N_ANALOG_FEEDBACK; i++ )
	{
		AnalogFeedbackValue[i] = (analogFeedbackValue_t)ANALOG_FEEDBACK_INC;
		AnalogFeedbackVars[i].flags.all = 0;
		AnalogFeedbackVars[i].analogFeedbackCntr = 0;
		AnalogFeedbackVars[i].adCh = ANALOG_FEEDBACK_NO_ADC_DEFINED;
		AnalogFeedbackVars[i].pAnalogFeedbackData = NULL;
	}
	AnalogFeedbackReady = (analogFeedbackInfoCmd_t)ANALOG_FEEDBACK_NOT_READY;
	InternalAnalogFeedbackReady = AnalogFeedbackReady;
	AnalogFeedbackDisable = (analogFeedbackInfoCmd_t)ANALOG_FEEDBACK_ENABLED;
	AnalogFeedbackInitializedMask = (analogFeedbackInfoCmd_t)ALL_ANALOG_FEEDBACK_NOT_INIT;
}

/**
Analog feedbacks setup, called inside LIB_IOM_InputInitialization() regardless \n
reset's type.

\param  [in]	chADC_AF A/D channel linked to the analog feedback.
\param  [in]	pAnalogFeedbackData pointer to Low Level Parameters inside Setting File

\attention \b AnalogFeedbackVars used as W

\return 	error enum AnalogFeedbackErr_Tag

\retval		AN_FEEDBACK_E_OK
\retval		AN_FEEDBACK_CH_ADC_NOT_COMPLIANT
\retval     AN_FEEDBACK_POS_NOT_COMPLIANT


\author 	Stefano Ughi
\date		19/07/2010
\version	1.0.2


\note		\par \b Nota 1:
			documentation by Stefano Ughi
		    \par \b Nota 2:
		    13/10/2008 added management of AnalogFeedbackInitializedMask var.
		    \par \b Nota 3:
		    19/07/2010 modified data structures to support new micro.

*/

enum AnalogFeedbackErr_Tag PM_AnalogFeedback_Setup( uint8_t chADC_AF, pAnalogFeedbackData_t pAnalogFeedbackData )
{
	AnalogFeedback_t *pAnalogFeedbackVars;
	enum AnalogFeedbackErr_Tag error = AN_FEEDBACK_E_OK;
	uint8_t posAnalogFeedback;

		/* posizione all'interno del vettore AnalogInput[] su cui trover� il valore
		   A/D linkato all'analog feedback */
		posAnalogFeedback = LIB_ADC_AnalogChRamPos(chADC_AF);
		if (posAnalogFeedback < N_ANALOG_FEEDBACK)
		{
			pAnalogFeedbackVars = &AnalogFeedbackVars[posAnalogFeedback];
			/* Save pointer to the configuration params */
			pAnalogFeedbackVars->pAnalogFeedbackData = pAnalogFeedbackData;
			/* Save ADC channel linked to Analog Feedback */
			pAnalogFeedbackVars->adCh = chADC_AF;
			/* check se lettura auto */
			if ( ((analogFeedbackLLIparams_t*)pAnalogFeedbackData)->bit.automatic)
			{
				pAnalogFeedbackVars->flags.bit.analogFeedbackEn = 1;
				/* Inizializzazione della misura per l'analogico linkato */
				LIB_ADC_InitMeasureForCh ( chADC_AF );
			}
			AnalogFeedbackInitializedMask |= (analogFeedbackInfoCmd_t)(1u<<posAnalogFeedback);
		}
		else
		{
			error = AN_FEEDBACK_POS_NOT_COMPLIANT;
		}
	
	return error;
}
/**
Per tutti gli analogfeedback abilitati controlla se la misura analogica corrispondente \n
� pronta ed in caso affermativo esegue la conversione in base ad una tabella presente \n
nel file di setting.\n
Chiamata in Task_AnalogChannels.
For all analogfeedbacks enabled, controls whether analog measurement corresponding \ n
is ready and if so, does the conversion on the basis of a table in the settings file. \ n
Called inside Task AnalogChannels.


\attention \b AnalogFeedbackVars used as I
\attention \b AnalogFeedbackDisable used as I
\attention \b AnalogFeedbackValue[] used as I/O
\attention \b AnalogFeedbackReady used as I/O
\attention \b InternalAnalogFeedbackReady used as I/O


\author 	Stefano Ughi
\date		18/09/2008
\version	1.0.1


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void PM_AnalogFeedbackManager( void )
{
	uint8_t posAnalogFeedback;
	ADC_ValueType analogMeasure;
	AnalogFeedback_t *pAnalogFeedbackVars = &AnalogFeedbackVars[0];

	for ( posAnalogFeedback = 0; posAnalogFeedback < N_ANALOG_FEEDBACK; posAnalogFeedback ++ )
	{
		if ( pAnalogFeedbackVars->flags.bit.analogFeedbackEn )
		{
			/* feedback analogico abilitato */
			if ( pAnalogFeedbackVars->analogFeedbackCntr != 0 )
			{
				/* c'� stata una richiesta di restart della misura dopo lo scadere
				   di analogFeedbackCntr */
				pAnalogFeedbackVars->analogFeedbackCntr--;
				pAnalogFeedbackVars->flags.bit.restartMeasure = 1;
			}
			else
			{
				if ( pAnalogFeedbackVars->flags.bit.restartMeasure == 1 )
				{
					/* Reinizializza misura sul canale ADC linkato */
					LIB_ADC_InitMeasureForCh ( pAnalogFeedbackVars->adCh );
					pAnalogFeedbackVars->flags.bit.restartMeasure = 0;
				}
				if ( LIB_ADC_IsNoAutoMeasureReady( pAnalogFeedbackVars->adCh ) )
				{
					if ( !( AnalogFeedbackDisable & (1u<<(posAnalogFeedback)) ) )
					{
						/* aggiorna valore solo se abilitato da tools esterni */
						analogMeasure = LIB_ADC_GetAnalogValue(posAnalogFeedback);
						AnalogFeedbackValue[posAnalogFeedback] = AnalogFeedbackSearchValue(posAnalogFeedback,analogMeasure);
						AnalogFeedbackReady |= (analogFeedbackInfoCmd_t)(1u<<posAnalogFeedback);
					}
					InternalAnalogFeedbackReady |= (analogFeedbackInfoCmd_t)(1u<<posAnalogFeedback);

					LIB_ADC_InitMeasureForCh ( pAnalogFeedbackVars->adCh );
				}
			}
		}
		pAnalogFeedbackVars++;
	}
}
/**
Returns TRUE if the analog feedback in position "posAnalogFeedback" was acquired
at least once.

\param  [in]		posAnalogFeedback analog feedback position

\attention \b AnalogFeedbackReady used as I

\return 	bool_t

\retval		TRUE -> analog feedback ready
\retval		FALSE -> analog feedback not ready


\author 	Stefano Ughi
\date		04/03/2008
\version	1.0.0



\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
		    This function is for the exclusive use of the application

*/

bool_t PM_AnalogFeedbackValueReady( uint8_t posAnalogFeedback )
{
	return(bool_t)( ( ( AnalogFeedbackReady & ( 1u << posAnalogFeedback ) )?TRUE:FALSE) );
}
/**
Returns TRUE if the analog feedback in position "posAnalogFeedback" was acquired.

\param  [in]	posAnalogFeedback analog feedback position

\attention \b InternalAnalogFeedbackReady used as I

\return 	bool_t

\retval		TRUE -> analog feedback ready
\retval		FALSE -> analog feedback not ready

\author 	Stefano Ughi
\date		07/03/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_AnalogFeedbackIsReady( uint8_t posAnalogFeedback )
{
	return(bool_t)( ( ( InternalAnalogFeedbackReady & ( 1 << posAnalogFeedback ) )?TRUE:FALSE) );
}
/**
Returns TRUE if all analog feedbacks enabled were acquired at least once.

\attention \b AnalogFeedbackVars[] used as I

\return 	bool_t

\retval		TRUE -> all analog feedback ready
\retval		FALSE -> at least one analog feedback is not ready


\author 	Stefano Ughi
\date		07/03/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi


*/
bool_t PM_AnalogFeedbackAllValuesReady ( void )
{
	uint8_t posAnalogFeedback;

	for ( posAnalogFeedback = 0; posAnalogFeedback < N_ANALOG_FEEDBACK; posAnalogFeedback ++ )
	{
		if ( AnalogFeedbackVars[posAnalogFeedback].flags.bit.analogFeedbackEn )
		{
			if ( PM_AnalogFeedbackValueReady(posAnalogFeedback) == FALSE )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
/**
Returns the analog feedback value in position "posAnalogFeedback".

\param  [in]	posAnalogFeedback analog feedback position

\attention \b AnalogFeedbackValue[] used as I

\return 	analogFeedbackValue_t


\author 	Stefano Ughi
\date		04/03/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
analogFeedbackValue_t PM_AnalogFeedbackGetValue( uint8_t posAnalogFeedback )
{
	return(AnalogFeedbackValue[posAnalogFeedback]);
}
/**
The function is called by the module consistency  each time the load state, \n
which is linked to the analog feedback, varies. \n
Is sought, within the parameters associated with the module, the timeout value
before retry consistency. \n

\param  [in]	posAnalogFeedback positionl of the analog feedback linked to the load

\attention \b AnalogFeedbackVars[] used as I/O


\author 	Stefano Ughi
\date		10/03/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void PM_AnalogFeedbackResetMeasure( uint8_t posAnalogFeedback )
{
	if ( posAnalogFeedback < N_ANALOG_FEEDBACK )
	{
		AnalogFeedbackVars[posAnalogFeedback].analogFeedbackCntr = *((uint8_t*)(AnalogFeedbackVars[posAnalogFeedback].pAnalogFeedbackData) + 2);
		/* Inibisce a Consistency di processare il feedback */
		InternalAnalogFeedbackReady &= (analogFeedbackInfoCmd_t)(~( 1u << posAnalogFeedback ));
	}
}
/**
Returns, as bit mask, the used elements inside AnalogFeedbackValue[] vector\n
E.g.:\n
Bit Position                     7,6,5,4,3,2,1,0 \n
AnalogFeedbackInitializedMask = [0,0,1,1,0,1,0,0]\n
AnalogFeedbackValue[2],AnalogFeedbackValue[4],AnalogFeedbackValue[5] used.

\attention \b AnalogFeedbackInitializedMask used as R

\return 	analogFeedbackInfoCmd_t

\retval		AnalogFeedbackInitializedMask


\author 	Stefano Ughi
\date		13/10/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

analogFeedbackInfoCmd_t AnalogFeedbackGetMaskAnalogF_Enabled( void )
{
	return (AnalogFeedbackInitializedMask);
}

/* Local Function */
/**
Fixed L.L.I parameters and the A/D measure of the channel linked to the feedback\n
decides the analog feedback value.

\param  [in]		ch A/D channel linked to the feedback
\param  [in]		analogMeasure A/D measure

\attention \b AnalogFeedbackVars used as I

\return 	analogFeedbackValue_t

\retval		dipendente da tabella


\author 	Stefano Ughi
\date		04/03/2008
\version	1.0.0

\warning    vale solo per risoluzione dell'A/D converter pari a 10

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static analogFeedbackValue_t AnalogFeedbackSearchValue( uint8_t posAnalogFeedback, ADC_ValueType analogMeasure )
{
	static uint16_t *pSoglia;
	analogFeedbackValue_t analogFeedbackValue;

	pSoglia = (uint16_t*)AnalogFeedbackVars[posAnalogFeedback].pAnalogFeedbackData+1;

	do
	{
		pSoglia++;

	}
	while( analogMeasure > (*pSoglia&ANALOG_FEEDBACK_FIELD_THRESHOLD_MASK) );

	analogFeedbackValue = (analogFeedbackValue_t)((*(pSoglia)&ANALOG_FEEDBACK_FIELD_VALUE_MASK)>>10);
	return ( analogFeedbackValue );
}

#endif /* defined (CONFIG_PM_ANALOG_FEEDBACKS_SUPPORT) */
/* *********** */
/* End of file */
/* *********** */

