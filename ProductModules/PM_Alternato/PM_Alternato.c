/*
 * PM_Alternato.c
 *
 *  Created on: 06/nov/2013
 *      Author: consul.fimiani
 */




/* ******** */
/* Includes */
/* ******** */
#include "PM_Alternato.h"
#include "DigitalOutputs.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Valori possibili per command_t: Command per procedura ricerca posizione default */
#define ALT_DRIV_TO_REF	0
/** Valori possibili per command_t: Command per procedura incremento posizione rispetto l'attuale */
#define ALT_DRIV_INC	1

/** Valori ammissibili per ALT_ManagerResult_t: posizione raggiunta  */
#define LAV_ALT_POS_REACHED		TRUE
/** Valori ammissibili per ALT_ManagerResult_t: posizione non raggiunta  */
#define LAV_ALT_POS_NOT_REACHED	FALSE


#define IS_ALT_TYPE1(pAltData)	(pAltData->DataLenght == 3)
#define IS_ALT_TYPE2(pAltData)	(pAltData->DataLenght == 5)
#define IS_ALT_TYPE3(pAltData)	(pAltData->DataLenght == 8)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t command_t;
typedef uint8_t ALT_ManagerResult_t;


/** struttura flags */
typedef struct {
    /** se '1' servomeccanismo alternato presente */
    uint8_t altEngineIsPresent      :1;
    /** se '1' richiesta procedura di sincronizzazione */
    uint8_t altEngineReqSync        :1;
    /** se '1' driver in attesa di fine procedura sincronizzazione */
    uint8_t waitForSyncPos          :1;
    /** se '0' inizializzare procedura incremento posizione rispetto all'attuale */
    uint8_t initIncPos              :1;
}FlagsStruct_t;

/** union flags */
typedef union {
    /** accesso a byte dei flags */
    uint8_t all;
    /** accesso a bit dei flags */
    FlagsStruct_t bit;
}AlternatoFlags_t;


/** struttura variabili utilizzate dal modulo */
typedef struct {
    /** struttura dati contente lunghezza dati
    e puntatore all'inizio della struttura dati stessa */
    struct Loader data;
    /** pin cui è connesso il modulo */
    uint8_t pin;
    /** posizione richiesta per il servomeccanismo */
    uint8_t altEnginePosReq;
    /** posizione corrente per il servomeccanismo dell'alternato */
    uint8_t altEnginePosCurr;
    /** offset vs tabella temporizzazioni output Off/On */
    uint8_t stepCurrent;
    /** offset vs tabella temporizzazioni output Off/On */
    uint8_t stepTarget;
    /** contatore per ottenere i 100 msec */
    uint8_t cntr100msec;
    /** contatore che indica se debbo tenere l'uscita alta o bassa */
    uint8_t cntrOutputHighLow;
    /** numero di posizioni del servomeccanismo da gestire, 3 o 6 */
    uint8_t numAltPositions;
    /** Posizione servomeccanismo alternato NON definita */
    uint8_t altPositionNotDef;
    /** numero di ripetizioni necessarie per riportare la camma in posizione
    di riferimento. Sono usate solo per il servomeccanismo a 6 posizioni */
    uint8_t altRepetitions;

    AlternatoFlags_t flags;
}AlternatoStruct_t;


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */


AlternatoStruct_t AlternatoVars;



/* **************************** */
/* Prototype of local functions */
/* **************************** */

static void PM_ALT_SetCommand(command_t command);
static ALT_ManagerResult_t PM_ALT_Manager(void);


/**
Inizializzazione delle variabili necessarie al modulo.
Chiamata in OutputInitialization() indipendentemente dal tipo di reset.

\param  [in] pAlternatoData pointer to a struct type Loader

\attention \b AlternatoVars used as I/O

\return 	AlternatoError_t

\retval		ALTERNATO_E_OK, ALTERNATO_E_KO


\author 	Stefano Ughi
\date		29/01/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

AlternatoError_t PM_ALT_Init(pAlternatoData_t pAlternatoData, uint8_t pin)
{
	AlternatoError_t err = ALTERNATO_E_OK;

	AlternatoVars.flags.all = 0;

	if(IS_ALT_TYPE1(pAlternatoData) || IS_ALT_TYPE2(pAlternatoData) || IS_ALT_TYPE3(pAlternatoData))
	{
		AlternatoVars.data = *pAlternatoData;
		AlternatoVars.pin = pin;
		AlternatoVars.flags.bit.altEngineIsPresent = 1;

		/* The alternate of type 3 has 6 positions, the others have 3 */
		if (IS_ALT_TYPE3(pAlternatoData))
		{
		  AlternatoVars.numAltPositions = 6;
		}
		else
		{
		  AlternatoVars.numAltPositions = 3;
		}
		AlternatoVars.altPositionNotDef = AlternatoVars.numAltPositions; /* please don't touch */


		AlternatoVars.altEnginePosReq = AlternatoVars.altPositionNotDef;
		AlternatoVars.altEnginePosCurr = AlternatoVars.altPositionNotDef;
	}
	else
	{
		err = ALTERNATO_E_KO;
	}

	#if defined (CONFIG_ALT_PULSE_PARTIALIZATION)
	  PM_PLP_InsertLoad(pin,DIM_LAMP_L);
	#endif

	return err;
}




/**
Fissa la posizione da raggiungere per il servomeccanismo dell'alternato, eventualmente
viene richiesta la sincronizzazione.
Chiamata nella SetLoad().

\param  [in] loadParam typedef L_PARAM

\attention \b AlternatoVars used as I/O


\author 	Stefano Ughi
\date		29/01/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			not portable cfr. L_PARAM (big Endian little Endian ? )

*/
void PM_ALT_SetPosition(L_PARAM loadParam)
{
    AlternatoVars.flags.bit.altEngineReqSync = ((loadParam.b & BIT4_MASK )?1:0);
    AlternatoVars.altEnginePosReq = loadParam.s.reg;
}


/**
 *\n Driver for management of the servomechansim.
 *\n This procedure is called from the task 10ms when SystemMode == SYSTEM_NORMAL_MODE
 *\n
 *
 *\attention\b  AlternatoVars used as I/O
 *
 *\author 	Stefano Ughi
 *\date		03/08/2006
 *\version	1.0.0
 *
 */
void PM_ALT_EngineDrv( void )
{
  	if (AlternatoVars.flags.bit.altEngineIsPresent == 0)
	{
		return;
	}
	/* La condizione di sincronizzazione è prioritaria */
	if (AlternatoVars.flags.bit.altEngineReqSync)
	{
		PM_ALT_SetCommand((command_t)ALT_DRIV_TO_REF);
		AlternatoVars.flags.bit.waitForSyncPos = 1;
		AlternatoVars.altEnginePosCurr = AlternatoVars.altPositionNotDef; /* flag posizione corrente non nota */
		AlternatoVars.flags.bit.altEngineReqSync = 0;
    }
    if (AlternatoVars.flags.bit.waitForSyncPos == 1)
    {
		if (PM_ALT_Manager() == LAV_ALT_POS_REACHED)
		{
        	AlternatoVars.altEnginePosCurr = 0;	  /* flag posizione corrente 0 */
        	AlternatoVars.flags.bit.initIncPos = 0;
        	AlternatoVars.flags.bit.waitForSyncPos = 0;
		}
    }
    else
    {
        if ( ( AlternatoVars.altEnginePosReq != AlternatoVars.altEnginePosCurr ) &&
             ( AlternatoVars.altEnginePosReq < AlternatoVars.numAltPositions ) )
        {
        	if ( AlternatoVars.altEnginePosCurr >= AlternatoVars.numAltPositions )
        	{
        		/* recovery procedure */
				AlternatoVars.flags.bit.altEngineReqSync = 1;
        	}
        	else
        	{
    			if ( AlternatoVars.flags.bit.initIncPos == 0 )
			    {
					PM_ALT_SetCommand((command_t)ALT_DRIV_INC);
					AlternatoVars.flags.bit.initIncPos = 1;
			    }
			    else
			    {
			    	if ( PM_ALT_Manager() == LAV_ALT_POS_REACHED )
			    	{
			    		if( AlternatoVars.altEnginePosCurr < (AlternatoVars.numAltPositions-1) )
			    		{
			    			AlternatoVars.altEnginePosCurr ++;
			    		}
			    		else
			    		{
			    			AlternatoVars.altEnginePosCurr = 0;
			    		}
    					AlternatoVars.flags.bit.initIncPos = 0;
					}
			    }
        	}
        }
    }
}



/**
Inizializzazioni per driver alternato.

\param  [in] command valori ammissibili \n
            ALT_DRIV_INC -> incrementa di una posizione rispetto la corrente \n
            ALT_DRIVE_TO_REF -> esegui il posizionamento assoluto per trovare la posizione di default

\attention \b AlternatoVars used as I/O

\author 	Stefano Ughi
\date		29/01/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

static void PM_ALT_SetCommand(command_t command)
{

  AlternatoVars.altRepetitions = 0;

	switch (command)
	{
		case ALT_DRIV_INC:		/* Target position: Next */

			if( AlternatoVars.data.DataLenght == 3 )
			{/* For alternate type 1 */
				AlternatoVars.stepCurrent = 4;
				AlternatoVars.stepTarget  = 5;
			}
			else if( AlternatoVars.data.DataLenght == 5 )
			{/* For alternate type 2 */
				AlternatoVars.stepCurrent = 7;
				AlternatoVars.stepTarget  = 8;
			}
			else
			{/* For alternate type 3 */
				AlternatoVars.stepCurrent = 14;
				AlternatoVars.stepTarget  = 15;
			}

			break;

		case ALT_DRIV_TO_REF:	/* Target position: Ref */

			AlternatoVars.stepCurrent = 0;

			if( AlternatoVars.data.DataLenght == 3 )
			{/* For alternate type 1 */
				AlternatoVars.stepTarget = 3;
			}
			else if( AlternatoVars.data.DataLenght == 5 )
			{/* For alternate type 2 */
				AlternatoVars.stepTarget = 6;
			}
			else
			{/* For alternate type 3 */
			  AlternatoVars.stepTarget = 13;
			  AlternatoVars.altRepetitions = 5;
			}

			break;

		default:

		    break;
	}
	AlternatoVars.cntr100msec = 0;
	AlternatoVars.cntrOutputHighLow = 0;

}
/**
Applica i tempi di On e Off al pin di output del driver per raggiungere la posizione
corretta del servomeccanismo.

\attention \b AlternatoVars used as I/O

\return 	ALT_ManagerResult_t

\retval		LAV_ALT_POS_NOT_REACHED -> posizione non raggiunta \n
            LAV_ALT_POS_REACHED -> posizione raggiunta


\author 	Stefano Ughi
\date		29/01/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static ALT_ManagerResult_t PM_ALT_Manager (void)
{
ALT_ManagerResult_t posReached = LAV_ALT_POS_NOT_REACHED;
uint8_t Output = 0;

	if (AlternatoVars.stepCurrent > AlternatoVars.stepTarget)
	{
		/* final position has been reached */

		posReached = LAV_ALT_POS_REACHED;

	}
    else
    {
		AlternatoVars.cntr100msec++;

		if (AlternatoVars.cntr100msec >= 10)
		{
			AlternatoVars.cntrOutputHighLow++;
			AlternatoVars.cntr100msec = 0;
		}
	    if (AlternatoVars.cntrOutputHighLow < *(AlternatoVars.data.Data + AlternatoVars.stepCurrent) )
		{

			/* Per il servomeccanismo a 6 posizioni il primo parametro deve essere un ON */
			if(AlternatoVars.numAltPositions == 6)
			{
				/*
				 time is not elapsed
				 controlla se lo step corrente è pari
				*/
				if ( (AlternatoVars.stepCurrent & BIT0_MASK) == 0 )
				{
				    /* drive output */
				    Output = 1;
				}
			}
			else
			{
				/*
				 time is not elapsed
				 controlla se lo step corrente è dispari
				*/
				if ( AlternatoVars.stepCurrent & BIT0_MASK )
				{
				    /* drive output */
				    Output = 1;
				}
			}


		}
		else
		{
			/* time is elapsed -> increment step */
			AlternatoVars.stepCurrent++;

			/* Per il servomeccanismo a 6 posizioni in caso di riposizionamento */
			if( AlternatoVars.altRepetitions )
			{
			  if( AlternatoVars.stepCurrent == 14)
			  {
			    AlternatoVars.altRepetitions--;
			    AlternatoVars.stepCurrent = 2;
			  }
			}

			/* Reset counter */
			AlternatoVars.cntrOutputHighLow = 0;
		}
    }


    #if defined(CONFIG_ALT_PULSE_PARTIALIZATION)
        /* Per 300 ms l'impulso positivo viene parzializzato */
        /* 370 è il valore di parzializzazione al 35% di ON  */
      	if( Output && (AlternatoVars.cntrOutputHighLow < 3) )
      	{
      	  DIM_Const_SetLoad( Output, 0, AlternatoVars.pin, ((uint16_t)370), DIM_OFF );
      	}
      	else
      	{
      		DIM_Const_SetLoad( Output, 0, AlternatoVars.pin, DIM_MAX, DIM_OFF );
      	}
    #else
        LIB_DIO_SET_LOAD(Output, AlternatoVars.pin);
    #endif /* defined(CONFIG_ALT_PULSE_PARTIALIZATION) */


	return (posReached);
}




/**
Ritorna il numero di posizioni del corrente alternato.

\attention \b AlternatoVars used as I/O

\return 	uint8_t

\retval		AlternatoVars.numAltPositions

\author 	Leonardo Sparti
\date		21/02/2012
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/

uint8_t PM_ALT_getNumPos( void )
{
	return( AlternatoVars.numAltPositions );
}


/**
Ritorna il valore "Posizione non definita".

\attention \b AlternatoVars used as I/O

\return 	uint8_t

\retval		AlternatoVars.altPositionNotDef

\author 	Leonardo Sparti
\date		21/02/2012
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/

uint8_t PM_ALT_getPosNotDef( void )
{
	return( AlternatoVars.altPositionNotDef );
}



/**
Ritorna la posizione corrente dell'alternato.

\attention \b AlternatoVars used as I/O

\return 	uint8_t

\retval		valori ammissibili tra 0 e (AlternatoVars.numAltPositions - 1)

\author 	Stefano Ughi
\date		30/01/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint8_t PM_ALT_getPos( void )
{
	return( AlternatoVars.altEnginePosCurr );
}


/**
Posizione servomeccanismo raggiunta o meno.

\attention \b AlternatoVars used as I/O

\return 	bool_t

\retval		TRUE -> posizione corrente = posizione richiesta e \n
            posizione corrente compresa tra quelle possibili \n
            FALSE -> altrimenti


\author 	Stefano Ughi
\date		30/01/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

bool_t PM_ALT_posReached( void )
{
	if( ( AlternatoVars.altEnginePosReq == AlternatoVars.altEnginePosCurr ) &&
	    ( AlternatoVars.altEnginePosCurr < AlternatoVars.numAltPositions ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
