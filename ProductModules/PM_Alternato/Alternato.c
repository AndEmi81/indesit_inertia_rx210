/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    PM_Alternato 
@{
\n              Il modulo gestisce il servomeccanismo alternato pilotando
\n 				il carico con opportuni tempi di on e di off
\n
\n
\par Configuration:
\n
\n
\par Exportated Functions: 
\n
@}
\ingroup        ProductM
*/
/**
\n
\Par		    Alternato Source file
\n			    Contiene le funzioni per la gestione del servomeccanismo alternato
\n
\file		    Alternato.c
\ingroup	    PM_Alternato 
\date		    24/06/2009
\version	    00.00.01
\author		    Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n              00.00.01
\n              24/06/2009
\n              Pierluigi Ianni
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n 		        1.0.0
\n          	28/01/2008
\n          	Stefano Ughi
\n          	Terza versione
\n
\n
\n				1.4.0
\n				10/06/2004
\n				Progettista_1
\n				Seconda versione
\n
\n
\n				1.3.0
\n				10/04/2004
\n				Progettista_1
\n				Prima versione
\n
*/


/* ******** */
/* Includes */
/* ******** */

#ifndef DIO_DEBUG
#include "IO_driver.h"
#include "debug.h"
#endif 

#include "OCDIO_drv.h"

#ifndef DIO_DEBUG
/*PI begin: Rimossi solo per permettere la compilazione */
#include "SkeletonGlobal.h"
#include "IO_Manager.h"
/*PI end: Rimossi solo per permettere la compilazione */
#else
/*PI begin: Aggiunti solo per permettere la compilazione */
/*#include "StandardTypes.h"*/
#include "StandardDefines.h"
#include "Alternato.h"
/*PI end: Aggiunti solo per permettere la compilazione */
#endif /* DIO_DEBUG */

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

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
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
/*lint -e960 i campi hanno la medesima dim. */
typedef union {
    /** accesso a byte dei flags */
    uint8_t all;
    /** accesso a bit dei flags */
    FlagsStruct_t bit;    
}AlternatoFlags_t;
/*lint +e960 */

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

    AlternatoFlags_t flags;
}AlternatoStruct_t;

typedef uint8_t command_t;
typedef uint8_t ALT_ManagerResult_t;


/* ********* */
/* Variables */
/* ********* */

/* Exported */
AlternatoStruct_t AlternatoVars;


/* Static */

/* ******************** */
/* Functions Prototypes */
/* ******************** */
static void ALT_SetCommand( command_t );
static ALT_ManagerResult_t ALT_Manager ( void );


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Inizializzazione delle variabili necessarie al modulo. 
Chiamata in OutputInitialization() indipendentemente dal tipo di reset.

\param  [in]		pAlternatoData pointer to a struct type Loader
\param  [in]		pin 

\attention \b AlternatoVars used as I/O

\return 	AlternatoError_t

\retval		ALTERNATO_E_OK, ALTERNATO_E_KO 

\author 	Stefano Ughi
\date	  29/01/2008
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

AlternatoError_t ALT_Init( pAlternatoData_t pAlternatoData, uint8_t pin )
{
AlternatoError_t error = ALTERNATO_E_OK;
    
    AlternatoVars.flags.all = 0;
    
    if ( (pAlternatoData->DataLenght == 3) || (pAlternatoData->DataLenght == 5)  )
    {   
        AlternatoVars.data = *pAlternatoData;
        AlternatoVars.pin = pin;
        AlternatoVars.altEnginePosReq = POS_ALT_NOT_DEF;
        AlternatoVars.altEnginePosCurr = POS_ALT_NOT_DEF;
        AlternatoVars.flags.bit.altEngineIsPresent = 1;
    }
    else
    {
        error = ALTERNATO_E_KO;
    } 
    return(error);
}


/**
Fissa la posizione da raggiungere per il servomeccanismo dell'alternato, eventualmente
viene richiesta la sincronizzazione. 
Chiamata nella SetLoad().

\param  [in] 		loadParam typedef L_PARAM

\attention \b AlternatoVars used as I/O

\author 	Stefano Ughi
\date	  29/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			not portable cfr. L_PARAM (big Endian little Endian ? )

*/

void ALT_SetPosition(L_PARAM loadParam)
{
/*PI begin modifica per la compilazione*/
/* DIO_DEBUG begin: righe rimosse solo per debug */
#ifndef DIO_DEBUG
    AlternatoVars.flags.bit.altEngineReqSync = (( loadParam.b & BIT4_MASK )?1:0);
#endif /* DIO_DEBUG */
/* DIO_DEBUG end: righe rimosse solo per debug */
/*PI end modifica per la compilazione*/

    AlternatoVars.altEnginePosReq = loadParam.s.reg;
}


/**
Driver che gestisce il funzionamento del servomeccanismo dell'alternato.
La procedura è chiamata nel task dei 10 msec  se SystemMode == SYSTEM_NORMAL_MODE.

\attention\b  AlternatoVars used as I/O

\author 	Pierluigi Ianni
\date		03/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni	
*/

void ALT_EngineDrv( void )
{
    #if defined (DEBUG_ALTERNATO) && defined(DEBUG_HCS08)


      /* Sostituito con chiamata al driver  03/06/2009 Pierluigi Ianni
      PTADD_PTADD7 = 1;
      PTAD_PTAD7^=1;
			*/
			
			
			InOutDriverParamType IOdp;
			
			IOdp.PinNumber=10;
			IOdp.PinValue=1;
      
      		OCDIO_SetPortDirSync(&IOdp);
      		
			if (OCDIO_GetPortSync(&IOdp) == 1)
			{
			   IOdp.PinValue=0;
			}
			
      OCDIO_SetPortSync(&IOdp);
    #endif
	if ( AlternatoVars.flags.bit.altEngineIsPresent == 0 )
	{
		return;
	}
	/* La condizione di sincronizzazione è prioritaria */
	if ( AlternatoVars.flags.bit.altEngineReqSync )
	{
		ALT_SetCommand((command_t)ALT_DRIV_TO_REF);
		AlternatoVars.flags.bit.waitForSyncPos = 1;
		AlternatoVars.altEnginePosCurr = POS_ALT_NOT_DEF; /* flag posizione corrente non nota */
		AlternatoVars.flags.bit.altEngineReqSync = 0;
    }
    if ( AlternatoVars.flags.bit.waitForSyncPos == 1 )
    {
		if ( ALT_Manager() == LAV_ALT_POS_REACHED )
		{
        	AlternatoVars.altEnginePosCurr = 0;	  /* flag posizione corrente 0 */
        	AlternatoVars.flags.bit.initIncPos = 0;
        	AlternatoVars.flags.bit.waitForSyncPos = 0;	
		}    
    }
    else
    {
        if ( ( AlternatoVars.altEnginePosReq != AlternatoVars.altEnginePosCurr ) && 
             ( AlternatoVars.altEnginePosReq < NUM_ALT_POS ) )
        {
        	if ( AlternatoVars.altEnginePosCurr >= NUM_ALT_POS )
        	{
        		/* recovery procedure */
				AlternatoVars.flags.bit.altEngineReqSync = 1;    
        	}
        	else
        	{
    			if ( AlternatoVars.flags.bit.initIncPos == 0 )
			    {
					ALT_SetCommand((command_t)ALT_DRIV_INC);
					AlternatoVars.flags.bit.initIncPos = 1;				
			    }
			    else
			    {
			    	if ( ALT_Manager() == LAV_ALT_POS_REACHED )
			    	{
			    		if( AlternatoVars.altEnginePosCurr < (NUM_ALT_POS-1) )
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

bool_t ALT_posReached( void )
{
	if( ( AlternatoVars.altEnginePosReq == AlternatoVars.altEnginePosCurr ) &&
	    ( AlternatoVars.altEnginePosCurr < NUM_ALT_POS ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}


/**
Ritorna la posizione corrente dell'alternato. 

\attention \b AlternatoVars used as I/O

\return 	uint8_t

\retval		valori ammissibili tra 0 e NUM_ALT_POS - 1 	 

\author 	Stefano Ughi
\date		30/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint8_t ALT_getPos( void )
{
	return( AlternatoVars.altEnginePosCurr ); 
}

/* Local Function */

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

static void ALT_SetCommand( command_t command )
{	
		
	switch (command)
	{				
		case ALT_DRIV_INC:		/* Target position: Next */	
			
			if( AlternatoVars.data.DataLenght == 3 )
			{
				AlternatoVars.stepCurrent = 4;
				AlternatoVars.stepTarget  = 5;			
			}
			else 
			{
				AlternatoVars.stepCurrent = 7;
				AlternatoVars.stepTarget  = 8;						
			}
			
			break;
		
		case ALT_DRIV_TO_REF:	/* Target position: Ref */
			
			AlternatoVars.stepCurrent = 0;

			if( AlternatoVars.data.DataLenght == 3 )
			{			
				AlternatoVars.stepTarget = 3;
		    }
			else 
			{
				AlternatoVars.stepTarget = 6;						
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

static ALT_ManagerResult_t ALT_Manager (void)
{	
ALT_ManagerResult_t posReached = LAV_ALT_POS_NOT_REACHED;
uint8_t Output = 0;	
	
	if ( AlternatoVars.stepCurrent > AlternatoVars.stepTarget )
	{
		/* final position has been reached */
						
		posReached = LAV_ALT_POS_REACHED;					
	
	}
    else
    {
		AlternatoVars.cntr100msec++;
		
		if ( AlternatoVars.cntr100msec >= 10 )
		{
			AlternatoVars.cntrOutputHighLow++;
			AlternatoVars.cntr100msec = 0;	
		}
	    if ( AlternatoVars.cntrOutputHighLow < *(AlternatoVars.data.Data + AlternatoVars.stepCurrent) )
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
		else
		{
			/* time is elapsed -> increment step */		
			AlternatoVars.stepCurrent++;			
			/* Reset counter */
			AlternatoVars.cntrOutputHighLow = 0;		
		}
    }
    
    SET_LOAD(Output, AlternatoVars.pin);

	return (posReached);			
}

/* *********** */
/* End of file */
/* *********** */
