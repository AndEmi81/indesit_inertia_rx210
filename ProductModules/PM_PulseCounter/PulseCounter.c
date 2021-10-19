/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	      PM_PulseCounter
@{
\n     Il modulo ha lo scopo di intercettare impulsi all'interno di un determinato range di frequenze
\n 	   definite sul file di setting.
\n
\par Configuration:
\todo
\n
\par Exported Functions:
\todo
\n   
@}
\todo la #define ONE_TIMER_TURN presuppone che il timer sia free running \n
      ma se non lo fosse bisognerebbe sostituirlo con il valore del TPMxMOD \n
\ingroup        ProductM
*/
/**
\n
\Par		        PulseCounter Source file
\n			        Contiene le funzioni per la gestione dispositivo flowmeter.
\n
\file		        PulseCounter.c
\ingroup	      PM_PulseCounter
\date		        21/07/2009
\version	      00.00.01
\author		      Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n              00.00.01
\n              21/07/2009
\n              Pierluigi Ianni
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n				      1.0.0
\n				      17/01/2008
\n				      Stefano Ughi
\n				      Terza versione
\n
\n
\n				      1.4.0
\n				      10/06/2004
\n				      Progettista_1
\n				      Seconda versione
\n
\n
\n				      1.3.0
\n				      10/04/2004
\n				      Progettista_1
\n				      Prima versione
\n
*/


/* ******** */
/* Includes */
/* ******** */
#include "debug.h"

#include "SkeletonGlobal.h"
#include "IO_Manager.h"
#include "PulseCounter.h"

/*#ifdef DIO_DEBUG*/
/*#include "StandardTypes.h"*/
/*#include "StandardDefines.h"*/
/*#endif */
/* DIO_DEBUG */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Questo è l'offset vs inizio registri porte per HCS08 e per H836079 */
#if defined(BUILD_FREESCALE_GB60)
#define OFFSET  0x00000000UL
#endif
#if defined(BUILD_RENESAS_36079)
#define OFFSET  0x00FF0000UL
#endif

/** Questo è il software divisor per la scalatura dei periodi calcolati */
#define PULSECOUNTER_SW_DIV             (1+((PULSECOUNTER_MAX_PERIOD_MSEC*1000000UL)/(PULSECOUNTER_TICK_DURATION*65535U)))
/** questi sono gli overflows massimi ammissibili tra una cattura e la successiva */
#define PULSECOUNTER_NMAX_TIMER_OVFL    (PULSECOUNTER_SW_DIV-1)

/** Input funzione PulseCounter_UpdateCntrsFreqNotInRange() frequenza troppo alta */
#define PULSECOUNTER_HIGH_FREQ_WARN   (uint8_t)1
/** Input funzione PulseCounter_UpdateCntrsFreqNotInRange() frequenza troppo bassa */
#define PULSECOUNTER_LOW_FREQ_WARN    (uint8_t)0
    
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** struttura flags */
typedef struct {
    /** se '1' segnala frequenza troppo alta */
    uint8_t pulseCounterWarningFreqTooHigh      :1;
    /** se '1' segnala frequenza troppo bassa */
    uint8_t pulseCounterWarningFreqTooLow       :1;
    /** se '1' decrementa il numero di overflows al prossimo ISR di cattura */
    uint8_t pulseCounterDecOverflowsNextOne     :1;
    /** questi bits sono liberi */
    uint8_t freeBit :5;

}PulseCounterFlagsStruct_t;
/*lint -e960 i membri della union hanno la stessa dim. */
/** struttura flags */
typedef union {

    /** accesso a byte dei flags */
    uint8_t all;
    /** accesso a bit dei flags */
    PulseCounterFlagsStruct_t flags;
    
} PulseCounterFlags_t;
/*lint +e960*/
/** struttura variabili globali modulo */
typedef struct {
    /** questo è il valore massimo accettabile per il periodo, normalizzato per la quantità \n
    1000000ul/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV) */
    uint16_t maxTnorm;          
    /** questo è il valore minimo accettabile per il periodo, normalizzato per la quantità \n
    1000000ul/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV) */
    uint16_t minTnorm;			
     /** questo è il limite per procedere alla correzione degli impulsi, normalizzato per la quantità \n
    1000000ul/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV) */   
    uint16_t corrTnorm;		    
    /** fattore di correzione */
    uint8_t corrFactor;			
    /** pin agganciato al canale */
    PulseCounterPin_t pin; 
    /** contatore n° volte segnale con frequenza troppo alta */
    uint8_t highFreqCntr;       
    /** contatore n° volte segnale con frequenza troppo bassa */
    uint8_t lowFreqCntr;
    /** contatore n° volte impulsi all'interno del range di compensazione */		
    uint8_t pulseCorrRangeCntr;
    /** canale agganciato al modulo */
    uint8_t channelLinkedToModule;
    /** flags vari */ 
    PulseCounterFlags_t flags;

}PulseCounterStruct_t;
/** periodo tra 2 fronti normalizzato per PULSECOUNTER_SW_DIV */
typedef uint16_t pulseCounterTickPeriod_norm_t;


/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* Global */

/** Tutte le variabili globali utilizzate dal modulo */
PulseCounterStruct_t PulseCounterVars;
#if defined (PULSECOUNTER_DEBUG)
pulseCounterTickPeriod_norm_t pulseCounterTickPeriod_norm;
uint8_t numOfTimerOverflows;
#endif


uint16_t PulseCount;

/* ******************* */
/* Function Prototypes */
/* ******************* */
static uint8_t PulseCounter_Isr_InputCaptureManager(uint8_t channel,uint16_t captTimerTick,uint8_t ovfFlag);
static bool_t PulseCounter_InputIsCoherent( uint8_t channel );
static void PulseCounter_UpdateCntrsFreqNotInRange( uint8_t high_low_freq );
static uint8_t PulseCounter_GetInputPinValue( PulseCounterPin_t pin );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
La funzione provvede all'inizializzazione del modulo PulseCounter, \n
la chiamata è effettuata in nel modulo IO_Manager \n 
funzione InputInitialization() indipendentemente dal tipo di reset. 
La struttura dati in input è la seguente: \n
   | byte high   |   byte low  |			 \n
   | corrFactor  |    unused   |			 \n
   | valore massimo periodo    |  [msec]	 \n
   | valore minimo periodo     |  [msec]	 \n
   | limite per la correzione  |  [msec]	 \n
     
\param  [in]		pin è il pin cui è collegato il canale di Input Capture
\param  [in]		pDataPulseCounter puntatore ai dati di configurazione del modulo


\attention \b PulseCounterVars used as I/O

\return 	PulseCounterError_t

\retval		PULSECOUNTER_E_OK \n
         	PULSECOUNTER_E_KO 	 

\author 	Stefano Ughi
\date		17/01/2008
\version	1.0.1

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			il parametro corrFactor viene prelevato dalla posizione +8 invece che dalla posizione +0 \n
			della tabella di configurazione (06/06/2008).	

*/
PulseCounterError_t PulseCounter_Init(PulseCounterPin_t pin, pPulseCounterData_t pDataPulseCounter)
{
PulseCounterError_t error = PULSECOUNTER_E_OK;

    /* richiede il canale del timer associato al pin */
    PulseCounterVars.channelLinkedToModule = FreqMeter_GetTimerChannelFromPin(pin);
    if ( ( PulseCounterVars.channelLinkedToModule == 0xFF ) || ( pDataPulseCounter == NULL) )
    {
        error = PULSECOUNTER_E_KO;    
    }
    else
    {   
        PulseCounterVars.pin = pin;

        PulseCounterVars.corrFactor = *((uint8_t*)pDataPulseCounter+8);
        /* massimo periodo di accettazione del segnale -> minima frequenza */
        /*lint -e912 */
        PulseCounterVars.maxTnorm = (uint16_t)((*((uint16_t*)pDataPulseCounter+1)*1000000UL)/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV));
        /* minimo periodo di accettazione del segnale -> minima frequenza */		
		PulseCounterVars.minTnorm = (uint16_t)((*((uint16_t*)pDataPulseCounter+2)*1000000UL)/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV));
        /* periodo per la correzione degli impulsi -> minima frequenza */				
		PulseCounterVars.corrTnorm = (uint16_t)((*((uint16_t*)pDataPulseCounter+3)*1000000UL)/(PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV));
		/*lint +e912 */

        
        PulseCounterVars.flags.all = 0;	

        
        /* carica per il canale selezionato le funzioni per gestire gli overflows
           e per la gestione dell'input capture */
        FreqMeter_SetManagerFunctions(PulseCounterVars.channelLinkedToModule,NULL,PulseCounter_Isr_InputCaptureManager);
		
        /* disabilita la gestione della chiamate alla funzione 
        PulseCounter_Isr_InputCaptureManager in corrispondenza 
        degli interrupts relativi 
        */
        FreqMeter_SetChannelStatus(PulseCounterVars.channelLinkedToModule,(uint8_t)OFF);

        /* disabilita il capture interrupt */
        FreqMeter_SetIsrInputCapture(PulseCounterVars.channelLinkedToModule,(uint8_t)OFF);        
    
    }
    return error;
}
    	

/**
Reset warnings frequenza troppo alta, frequenza troppo bassa. 

\param  [in]		whichWarning indica quale warning resettare

\attention \b PulseCounterVars


\author 	Stefano Ughi
\date		24/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/    	
void PulseCounter_Reset_Warning( uint8_t whichWarning )
{
    switch ( whichWarning )
    {
        case PULSECOUNTER_RESET_LOW_WARNING:
            PulseCounterVars.lowFreqCntr = 0;
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooLow = FALSE;
            break;
        case PULSECOUNTER_RESET_HIGH_WARNING:
            PulseCounterVars.highFreqCntr = 0;
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooHigh = FALSE;
            break;
        case PULSECOUNTER_RESET_BOTH_WARNING:
            PulseCounterVars.highFreqCntr = 0;
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooHigh = FALSE;
            PulseCounterVars.lowFreqCntr = 0;
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooLow = FALSE;
            break;
        default:
            break;
    }
}


/**
Ritorna il o i warnings presenti. 

\attention \b PulseCounterVars used

\return 	uint8_t

\retval		  0 -> No warnings 
            1 -> High Frequency warning 
            2 -> Low Frequency warning
            3 -> High and Low Frequency warning 

\author 	Stefano Ughi
\date		24/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t PulseCounter_GetWarning( void )
{
uint8_t pulseCounterWarnings = 0;

    if ( PulseCounterVars.flags.flags.pulseCounterWarningFreqTooHigh )
    {
        pulseCounterWarnings |= BIT0_MASK;    
    }
    if ( PulseCounterVars.flags.flags.pulseCounterWarningFreqTooLow )
    {
        pulseCounterWarnings |= BIT1_MASK;            
    }
    
    return ( pulseCounterWarnings );
	
}


/**
Setta la variabile globale PulseCount al valore prefissato.

\param  [in]		newPulseCount valore per PulseCount

\attention \b PulseCount used as I/O


\author 	Stefano Ughi
\date		24/01/2008
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi	

*/
/* PIZZOLATO 25/09/09 versione originale (commentata) non chiara, si ritiene che la versione corretta
   sia quella modificata sotto*/
void PulseCounter_SetCount( uint16_t newPulseCount )
{
	/*uint16_t PulseCount = newPulseCount;*/
	PulseCount = newPulseCount;
}


/**
Ritorna il valore della variabile PulseCount. 

\attention \b PulseCount used as I/O

\return 	uint16_t

\author 	Stefano Ughi
\date		24/01/2008
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint16_t PulseCounter_GetCount( void )
{
uint16_t copyOfPulseCount;

	SuspendAllInterrupts();
	copyOfPulseCount = PulseCount;
	ResumeAllInterrupts();
  return (copyOfPulseCount);

}


/**
Abilita/Disabilita il modulo. 

\param  [in] moduleState abilitazione/disabilitazione 

\attention \b PulseCounterVars used as I/O

\return 	PulseCounterError_t

\retval		PULSECOUNTER_E_OK -> l'operazione ha avuto successo \n
          PULSECOUNTER_E_KO 	 

\author 	Stefano Ughi
\date		25/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

PulseCounterError_t PulseCounter_SetState ( PulseCounterState_t moduleState )
{
PulseCounterError_t error = PULSECOUNTER_E_OK;
    
    if ( PulseCounterVars.channelLinkedToModule != 0xFF )
    {
        switch ( moduleState )
        {
            case PULSECOUNTER_ENABLE:
                /* solo se il canale non è già abilitato */
                if ( !FreqMeter_GetChannelStatus(PulseCounterVars.channelLinkedToModule) )
                {

            		    /* selezione del fronte di sensibilità all'interrupt */
                    FreqMeter_SetEdge(PulseCounterVars.channelLinkedToModule, (uint8_t)INPUT_CAPTURE_ON_RISING_EDGE);
                    /* resetta il n° di overflow */    		
            		    FreqMeter_ResetOverflow(PulseCounterVars.channelLinkedToModule);
            		    /* abilita il capture interrupt */
                    FreqMeter_SetIsrInputCapture(PulseCounterVars.channelLinkedToModule,(uint8_t)ON);        
                    /* abilita la gestione della chiamate alla funzione 
            		    PulseCounter_Isr_InputCaptureManager in corrispondenza 
            		    degli interrupts relativi	*/
            		    FreqMeter_SetChannelStatus(PulseCounterVars.channelLinkedToModule,(uint8_t)ON);

                }
                break;
            
            case PULSECOUNTER_DISABLE:
                
                /* disabilita la gestione della chiamate alla funzione 
        		    PulseCounter_Isr_InputCaptureManager in corrispondenza 
        		    degli interrupts relativi */
        		    FreqMeter_SetChannelStatus(PulseCounterVars.channelLinkedToModule,(uint8_t)OFF);

        		    /* disabilita il capture interrupt */
                FreqMeter_SetIsrInputCapture(PulseCounterVars.channelLinkedToModule,(uint8_t)OFF);        
            
                break;
            
            default:
                error = PULSECOUNTER_E_KO;
                break;
        }
    }
    else
    {
        error = PULSECOUNTER_E_KO;
    }
    return ( error );
}


/* Local Function */


/**
Funzione per il managing dell'input capture. 

\param  [in]		channel canale cui è agganciato il modulo 
\param  [in]		captTimerTick valore della cattura in tick del timer
\param  [in]	  ovfFlag indica se nell'isr è stato intercettato un overflow del timer	 

\attention \b PulseCounterVars used as I/O

\return 	uint8_t

\retval		dummy 	 

\author 	Stefano Ughi
\date		24/01/2008
\version	1.0.0
   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static uint8_t PulseCounter_Isr_InputCaptureManager(uint8_t channel,uint16_t captTimerTick,uint8_t ovfFlag)
{
  uint8_t filter = 0;
  uint8_t numPinSamples = 0;
  bool_t checkPulsePeriod = FALSE;
  #if !defined (PULSECOUNTER_DEBUG)
  pulseCounterTickPeriod_norm_t pulseCounterTickPeriod_norm;
  uint8_t numOfTimerOverflows;
  #endif

  static uint16_t Old_captTimerTick;
    	
	filter = 0;
  numPinSamples = 0;	   
	/* filtro locale */
	/* PI begin: modificare o è solo per debug? */
	#if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_HCS08)
	PTADD_PTADD4 = 1;
	PTAD_PTAD4 = 1;
  #endif
  /* PI end: modificare o è solo per debug? */

	while ( filter < PULSECOUNTER_LOCALFILTER )
	{
	   if ( PulseCounter_InputIsCoherent(channel) )
	   {
	       numPinSamples ++;
	   }
	   filter ++;
	}
	/* PI begin: modificare o è solo per debug? */
	#if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_HCS08)
	PTADD_PTADD4 = 1;
	PTAD_PTAD4 = 0;
  #endif
	/* PI end: modificare o è solo per debug? */
	
	if ( numPinSamples == PULSECOUNTER_LOCALFILTER )
	{ 
		if ( FreqMeter_GetEdge( channel ) == INPUT_CAPTURE_ON_RISING_EDGE )
		{
	      /* PI begin: modificare o è solo per debug? */
	      #if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_HCS08)
    		PTADD_PTADD7 = 1;
    		PTAD_PTAD7 = 1;
    		#endif
        
        #if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_RENESAS_36079)
        PDR2_Mask |= BIT3_MASK;
        PORT.PDR2.BYTE = PDR2_Mask;
        #endif
			  /* PI end: modificare o è solo per debug? */
			/* Sono sincronizzato sul fronte positivo ed il fronte è valido */
			   numOfTimerOverflows = FreqMeter_GetOverflow(channel,(uint8_t)1);
			   if ( ovfFlag && ( captTimerTick < (ONE_TIMER_TURN/2) ) )
			   {
			    /* intercettato un overflow del timer nell'ISR di cattura e  
			       il valore della cattura è minore di mezzo periodo del timer -> 
			       l'overflow è precedente alla cattura  quindi debbo conteggiarlo 
			       e scartarlo alla successiva cattura perchè all'uscita da questo ISR
			       sarà conteggiato nell'interrupt dell'overflow del timer 
			       N.B.: PulseCounterVars.flags.flags.pulseCounterDecOverflowsNextOne vale TRUE 
			       si è verificato il caso in cui 2 fronti successivi si sono presentati 
			       subito dopo l'overflow del timer in tal caso NON devo incrementare 
			       numOfTimerOverflows */
			      if ( PulseCounterVars.flags.flags.pulseCounterDecOverflowsNextOne == FALSE )
			      {
			        if ( numOfTimerOverflows < 0xFF )
			        {
			            numOfTimerOverflows++;
			            PulseCounterVars.flags.flags.pulseCounterDecOverflowsNextOne = TRUE;
			        }			    
			      }		
			   }
			   else
			   {
			      if ( PulseCounterVars.flags.flags.pulseCounterDecOverflowsNextOne == TRUE )
			      {
			        numOfTimerOverflows--;
			        PulseCounterVars.flags.flags.pulseCounterDecOverflowsNextOne = FALSE;
			      }
			   }
				
			/* è tempo di determinare il periodo del segnale */
			checkPulsePeriod = TRUE;
			/* cambio fronte */
			FreqMeter_SetEdge(channel, (uint8_t)INPUT_CAPTURE_ON_FALLING_EDGE);
			
		}
		else
		{
        /* PI begin: modificare o è solo per debug? */
        #if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_HCS08)
    		PTADD_PTADD7 = 1;
    		PTAD_PTAD7 = 0;
    		#endif
        
        #if defined (PULSECOUNTER_DEBUG) && defined (DEBUG_RENESAS_36079)
        PDR2_Mask &= ~BIT3_MASK;
        PORT.PDR2.BYTE = PDR2_Mask;
        #endif
				/* PI end: modificare o è solo per debug? */
				
			/* Sono sincronizzato sul fronte negativo ed il fronte è valido 
		     --> cambio fronte */
         FreqMeter_SetEdge(channel, (uint8_t)INPUT_CAPTURE_ON_RISING_EDGE);
		}
	}
		
	if( checkPulsePeriod )
	{	
	    /*lint -esym(644,numOfTimerOverflows) numOfTimerOverflows è sempre inizializzato quando arrivo qui */					  
	    if ( numOfTimerOverflows < 0xFF )
	    {
	    /*lint +esym(644,numOfTimerOverflows) numOfTimerOverflows è sempre inizializzato quando arrivo qui */					  

    	    if ( numOfTimerOverflows > PULSECOUNTER_NMAX_TIMER_OVFL )
    	    {
    	        /* segnale non in range ( frequenza troppo bassa ) */
                PulseCounter_UpdateCntrsFreqNotInRange( PULSECOUNTER_LOW_FREQ_WARN );    	        
    	    }
    	    else
    	    {
    	        /* determinazione del periodo normalizzato del segnale */
    	        /*lint -e727 la mancata inizializzazione di Old_captTimerTick 
    	          provoca la prima misura non corretta ma ciò non rappresenta un problema */
    	        pulseCounterTickPeriod_norm = (pulseCounterTickPeriod_norm_t)
    	        ( 
    	        ((uint32_t)numOfTimerOverflows*ONE_TIMER_TURN+captTimerTick-Old_captTimerTick)/PULSECOUNTER_SW_DIV
    	        ); 

    	        /*lint +e727*/
    	        /* verifica se segnale in range */ 
    	        if ( pulseCounterTickPeriod_norm < PulseCounterVars.minTnorm )
    	        {
    	            /* segnale non in range (frequenza troppo alta) */
                    PulseCounter_UpdateCntrsFreqNotInRange( PULSECOUNTER_HIGH_FREQ_WARN );
    	        }
    	        else if ( pulseCounterTickPeriod_norm > PulseCounterVars.maxTnorm )
    	        {
    	            /* segnale non in range (frequenza troppo bassa ) */
                    PulseCounter_UpdateCntrsFreqNotInRange( PULSECOUNTER_LOW_FREQ_WARN );
    	        }
    	        else
    	        {
    	            /* segnale in range */
    	            if ( ( PulseCounterVars.highFreqCntr == 0 ) && ( PulseCounterVars.lowFreqCntr == 0) )
    	            {
    	                /* per incrementare PulseCount devo avere una condizione precedente 
					       priva di warning (burst!!) */
					    PulseCount++;
    	            }
    	            PulseCounterVars.highFreqCntr = 0;
    	            PulseCounterVars.lowFreqCntr = 0;
    	            if ( pulseCounterTickPeriod_norm >= PulseCounterVars.corrTnorm )
    	            {
        				/*
        				Correggo di un'unità ogni PulseCounterVars.corrFactor impulsi consecutivi 
        				all'interno del range di correzione
        				*/
    	                PulseCounterVars.pulseCorrRangeCntr ++;
    	                if ( PulseCounterVars.pulseCorrRangeCntr >= PulseCounterVars.corrFactor )
    	                {
    	                    PulseCounterVars.pulseCorrRangeCntr = 0;
    	                    PulseCount++;
    	                } 
    	            }
    	            else
    	            {
    	                PulseCounterVars.pulseCorrRangeCntr = 0;
    	            }
    	        }
    	           	    
    	    }
	    
	    }
	    else
	    {
	        /* segnale non in range (frequenza troppo bassa) */
	        PulseCounter_UpdateCntrsFreqNotInRange( PULSECOUNTER_LOW_FREQ_WARN );
	    }
		
		Old_captTimerTick = captTimerTick;
	}
	
	return 0;
}


/**
Verifica la coerenza tra il fronte su cui è sincronizzato l'interrupt e il valore campionato sul pin,
se sono sincronizzato sul fronte positivo debbo leggere un valore alto, se sono sincronizzato
sul fronte negativo debbo leggere un valore basso. 

\param  [in]		channel canale cui è agganciato il modulo pulse counter

\attention \b PulseCounterVars used as input

\return 	bool_t

\retval		TRUE coerenza
          FALSE incoerenza 	 


\author 	Stefano Ughi
\date		26/01/2008
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static bool_t PulseCounter_InputIsCoherent( uint8_t channel )
{

    /*lint -e960 no side effects on right hand of operator & and or*/
    if ( ( ( PulseCounter_GetInputPinValue( PulseCounterVars.pin ) == 1 ) &&
         ( FreqMeter_GetEdge( channel ) == INPUT_CAPTURE_ON_RISING_EDGE ) ) ||
         ( ( PulseCounter_GetInputPinValue( PulseCounterVars.pin ) == 0 ) &&
         ( FreqMeter_GetEdge( channel ) == INPUT_CAPTURE_ON_FALLING_EDGE ) ) )
    /*lint +e960  */
    {
        return TRUE;   
    }
    else
    {
        return FALSE;
    }
    
}


/**
Ritorna il valore dell'ingresso individuato da pin. 

\param  [in]		pin pin del modulo secondo ModuleIOMap

\attention \b ModuleIOmap[] used as I

\return 	bool_t

\retval		1   il pin è alto
\retval		0   il pin è basso
 	 
\author 	Stefano Ughi
\date		21/07/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			

*/

static bool_t PulseCounter_GetInputPinValue( PulseCounterPin_t pin )
{
    
 		InOutDriverParamType IOdp;
    
    /* PI begin: vecchia versione senza driver */
    //bool_t pinValue;
    /*lint -e511 size compatibility verified */
    //pinValue = ((*((uint8_t*)(ModuleIOmap[pin][0] + OFFSET)) & ModuleIOmap[pin][1]));
    /*lint +e511 size compatibility verified */
    //return (pinValue?1:0);
    /* PI end: vecchia versione senza driver */
    
    IOdp.PinNumber = pin;
    return OCDIO_GetPortSync(&IOdp);

}


/**
Aggiorna i contatori di frequenza troppo alta o troppo bassa ed eventualmente vengono settati
i relativi warnings. 

\param  [in]		high_low_freq indica se intercettata frequenza troppo alta o troppo bassa

\attention \b PulseCounterVars used as I/O


\author 	Stefano Ughi
\date		26/01/2008
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
static void PulseCounter_UpdateCntrsFreqNotInRange( uint8_t high_low_freq )
{
    if ( high_low_freq == PULSECOUNTER_HIGH_FREQ_WARN )
    {
        if ( PulseCounterVars.highFreqCntr < PULSECOUNTER_FREQ_TOO_HIGH_FILTER )
        {
            PulseCounterVars.highFreqCntr++;
        }
        else
        {
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooHigh = TRUE;
        }
    }
    else
    {
        if ( PulseCounterVars.lowFreqCntr < PULSECOUNTER_FREQ_TOO_LOW_FILTER )
        {
            PulseCounterVars.lowFreqCntr++;
        }
        else
        {
            PulseCounterVars.flags.flags.pulseCounterWarningFreqTooLow = TRUE;
        }    
    }
}


/* *********** */
/* User Errors */
/* *********** */

#if (PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV > 65535 )

	#error "\n PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV too large, max value 65535 "

#endif

#if (PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV == 0 )

	#error "\n PULSECOUNTER_TICK_DURATION*PULSECOUNTER_SW_DIV is '0' "

#endif
#if ( PULSECOUNTER_NMAX_TIMER_OVFL >= 0xFF )
    /* il valore massimo sostenibile dovrebbe essere 
    ( PULSECOUNTER_NMAX_TIMER_OVFL >= ((0xFFFFFFFF-0xFFFF)/ONE_TIMER_TURN) )
    ma la funzione FreqMeter_GetOverflow() ritorna un uint8_t */
	#error "\n PULSECOUNTER_NMAX_TIMER_OVFL is TOO LARGE "

#endif
/* *********** */
/* End of file */
/* *********** */
