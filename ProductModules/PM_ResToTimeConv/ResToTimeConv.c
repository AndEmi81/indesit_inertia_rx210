/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    PM_ResToTimeConv
@{
\todo Inserire descrizione
\n
\par Configuration:
\todo
\n
\par Exported Functions:
\todo
\n     
         
@}
\ingroup        ProductM
*/
/**
\n
\Par		        ResToTimeConv Source file
\n			        Contiene le funzioni per la gestione degli overflow e degli interrupt sul timer
\n
\file		        ResToTimeConv.c
\ingroup	      PM_ResToTimeConv
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
\n              - Questo file rinominato e aggiornato sostituisce il vecchio TimeCounter.c
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n				      1.0.0
\n				      25/03/2005
\n				      Lucio Bianchi
\n				      Prima versione
\n
*/

/* TimeCounter 1.1.0 */
/* SelectNextChannel() come funzione per eesere chiamata anche da routine applicazione*/


/*
BUS_FREQ 	= 9.99771  MHz
BUS_PERIOD	= 0.100023 us


			PS2		PS1		PS0		PRESCALER	TICK TIMER		MAX TIMER COUNT

TPM1SC 		1		1		1		128			12.8 us			839 ms

TPM2SC 		1		0		0		16			1.6 us			104 ms
*/



/* ******** */
/* Includes */
/* ******** */
#include "debug.h"

#include "SkeletonGlobal.h"
/* PI begin: righe rimosse solo per permettere la compilazione */
#ifndef DIO_DEBUG
#include "ProductConfig.h"
#else
/*#include "StandardTypes.h"*/
#include "IO_driver.h"
#endif /* DIO_DEBUG */
#include "ResToTimeConv.h"
/* PI end: righe rimosse solo per permettere la compilazione */


/*#include "string.h"*/
/*LLBB*/

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*Bus Frequency [kHz]*/

/*PIZZOLATO 25/09/09 la variabile sottostante è effettivamente inutilizzata*/
/*#define BUS_FREQUENCY_kHz      (BUS_FREQUENCY/1000)*/


/* PI begin: define non più necessarie con l'uso del driver */
/*
/// ***	Registers
#if (TIME_COUNTER_TPM_CH==0)
//TPM 2 Channel 0 value
#define TIMECNT_C_V			TPM2C0V			/// register
//TPM 2 Channel 0 Status & Control Register
#define TIMECNT_C_SC			TPM2C0SC		/// register
#define TIMECNT_C_SC_IE		TPM2C0SC_CHxIE	///	ch 0 Interrupt Enable
#define TIMECNT_C_SC_F			TPM2C0SC_CHxF	/// ch 0 Flag
//Input Capture pin
#define TIME_COUNTER_IC_PIN 	PTDD_PTDD3  /// 


#elif (TIME_COUNTER_TPM_CH==1)
//TPM 2 Channel 1 value
#define TIMECNT_C_V			TPM2C1V			/// register
//TPM 2 Channel 1 Status & Control Register
#define TIMECNT_C_SC			TPM2C1SC		/// register
#define TIMECNT_C_SC_IE		TPM2C1SC_CHxIE	///	ch 1 Interrupt Enable
#define TIMECNT_C_SC_F			TPM2C1SC_CHxF	/// ch 1 Flag
//Input Capture pin
#define TIME_COUNTER_IC_PIN 	PTDD_PTDD4


#elif (TIME_COUNTER_TPM_CH==2)
//TPM 2 Channel 2 value
#define TIMECNT_C_V			TPM2C2V			/// register
//TPM 2 Channel 2 Status & Control Register
#define TIMECNT_C_SC			TPM2C2SC		/// register
#define TIMECNT_C_SC_IE		TPM2C2SC_CHxIE	///	ch 2 Interrupt Enable
#define TIMECNT_C_SC_F			TPM2C2SC_CHxF	/// ch 2 Flag
//Input Capture pin
#define TIME_COUNTER_IC_PIN 	PTDD_PTDD5


#elif (TIME_COUNTER_TPM_CH==3)
//TPM 2 Channel 3 value
#define TIMECNT_C_V			TPM2C3V			/// register
//TPM 2 Channel 3 Status & Control Register
#define TIMECNT_C_SC			TPM2C3SC		/// register
#define TIMECNT_C_SC_IE		TPM2C3SC_CHxIE	///	ch 3 Interrupt Enable
#define TIMECNT_C_SC_F			TPM2C3SC_CHxF	/// ch 3 Flag
//Input Capture pin
#define TIME_COUNTER_IC_PIN 	PTDD_PTDD6


#elif (TIME_COUNTER_TPM_CH==4)
//TPM 2 Channel 4 value
#define TIMECNT_C_V			TPM2C4V			/// register
//TPM 2 Channel 4 Status & Control Register
#define TIMECNT_C_SC			TPM2C4SC		/// register
#define TIMECNT_C_SC_IE		TPM2C4SC_CHxIE	///	ch 4 Interrupt Enable
#define TIMECNT_C_SC_F			TPM2C4SC_CHxF	/// ch 4 Flag
//Input Capture pin
#define TIME_COUNTER_IC_PIN 	PTDD_PTDD7

#endif
*/
/* PI end: define non più necessarie con l'uso del driver */



/*!
**	*******************************************************************
**	ERRORS 											TPM2 Configuration
**	*******************************************************************
*/

/*PIZZOLATO: errori eliminati per permeteere la compilazione*/
#if !defined(DIO_DEBUG) 
#ifndef TPM2_PS
	#error "ResToTimeConv module Error:\n This module requires TPM2 timer to be configured.\
 	Define TPM2_PS in ProductConfig.h file" 
#endif
#else
#define N_TIME_INPUTS 10
#define MAX_TIME_NSAMPLES 1000
#endif /*!defined(DIO_DEBUG) */

/*!
**	*******************************************************************
**	ERRORS 							Pulse Counter module configuration
**	*******************************************************************
*/

#if (TIME_COUNTER_TPM_CH==0)
///
	#ifdef TIME_COUNTER_CH_ERROR
	#error "\n ResToTimeConv module error:\nCH0 on TPM 2 timer is already used"
	#endif

#elif (TIME_COUNTER_TPM_CH==1)

	#ifdef TIME_COUNTER_CH_ERROR
	#error "\n ResToTimeConv module error:\nCH1 on TPM 2 timer is already used"
	#endif

#elif (TIME_COUNTER_TPM_CH==2)
	#ifdef TIME_COUNTER_CH_ERROR
	#error "\n ResToTimeConv module error:\nCH2 on TPM 2 timer is already used"
	#endif

#elif (TIME_COUNTER_TPM_CH==3)
	#ifdef TIME_COUNTER_CH_ERROR
	#error "\n ResToTimeConv module error:\nCH3 on TPM 2 timer is already used"
	#endif

#elif (TIME_COUNTER_TPM_CH==4)
	#ifdef TIME_COUNTER_CH_ERROR
	#error "\n ResToTimeConv module error:\nCH4 on TPM 2 timer is already used"
	#endif
#endif

/*
#if ( MAX_OVERFLOW_VALUE > 0xFFFF )
	#error "\n PulseCounter module Error:\n MAX_OVERFLOW_VALUE out of range:\n\
	modify TMP_PS property in CstmDefines.h file or PULSECNT_SW_DIVISOR"
#endif

#if (MAX_NUM_LETTURE < 1)
	#error "\n PulseCounter Module error: MAX_NUM_LETTURE value not valid"
#endif
#if (MAX_NUM_LETTURE > 255)
	#error "\n PulseCounter Module error: MAX_NUM_LETTURE value not valid"
#endif
#if (DELTA_NUM_LETTURE > 255)
	#error "\n PulseCounter Module error: DELTA_NUM_LETTURE value not valid"
#endif

#if (DELTA_NUM_LETTURE >= MAX_NUM_LETTURE )
	#error "\n PulseCounter Module error: DELTA_NUM_LETTURE value not valid"
#endif

*/

/*lint -e553 PIZZOLATO 25/09/09 PULSE_COUNTER_TPM_CH non definita nel progetto HAL*/
#if (PULSE_COUNTER_TPM_CH>4)
/*lint +e553*/
#error "ResToTimeConv module Error:\nTIME_COUNTER_TPM_CH value is wrong because\nTPM 2 timer has only 5 channels"
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* Global */

/*PIZZOLATO 25/09/09 la variabile TCM_OldTmrValue non è usata*/
/*static WORD  TCM_OldTmrValue    = 0;*/    /* Last Input Capture Value */
static BYTE  TCM_OverflowCount  = 0;    /* Number of timer overflows */

/*lint -esym(551,Tmr_ticks_sum) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static DWORD Tmr_ticks_sum		= 0;    /* N_SAMPLES Sum */

/*lint -esym(528,Tmr_Ref) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static WORD  Tmr_Ref 			= 0;    /* Reference value for filter before media calculation  */

/*lint -esym(551,N_read) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static BYTE  N_read 			= 0;
static BYTE  index 				= N_TIME_INPUTS - 1;    /*  Time Input Index*/
static BYTE  error_count 		= 0;    /* exit from endless loop */
/* 10 */
/*lint -esym(528,Tmr_ticks) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static WORD Tmr_ticks; 
/* 11*/

/*lint -esym(551, Time_Input_Enabled) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static BYTE Time_Input_Enabled = FALSE; /* TRUE with at least one time input enabled */
										
static TIME_INPUT_STRUCT TimeInputsParams[N_TIME_INPUTS];

static BYTE  TimeInputsAlarms	 = 0;
static BYTE  HL_TimeInputsAlarms = 0;
/*lint -esym(551, Reset_N_Read) PIZZOLATO 25/09/09 pclint segnala che non c'è accesso alla variabile sottostante, ma questo 
è dovuto quasi sicuramente alla variabile DIO_DEBUG. Una volta eliminata tale variabile (utilizzata al solo fine di
aiutare lo sviluppo dell'HAL) l'errore dovrebbe scomparire. */ 
static BYTE  Reset_N_Read = 0;


#ifndef OVERFLOW_CONTROL
	static unsigned short count_max = 0;
	static unsigned char  NSamplesMax = 0;
	unsigned short 		  count 	= 0; /* time (s) = */
	unsigned char         old_index = 0;
#endif


/*PIZZOLATO: variabile aggiunta per permettere la compilazione*/
int TimeInputDisable =0;
int TimeInputs[10];
int TimeInputReady = 0;
/*PIZZOLATO end*/


/* ******************* */
/* Function Prototypes */
/* ******************* */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Local Function */

/* PI begin: righe rimosse tramite DIO_DEBUG solo per permettere la compilazione */
#ifndef DIO_DEBUG
/**
TimeCounter_InputCapture_ISR

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

// about  70us@10MHz for positive edge (min calculation)
// about 150us@10MHz for negative edge (medium calculation)
// about 250us@10MHz for negative edge with average calculation, each N_Samples (max calculation)


ISR(Isr_TimeCounter_InputCapture)
{
  WORD TCM_TmrValue;
	BYTE ContinueTest = FALSE;
	BYTE InputCount = 0;
	BYTE InputSum = 0;
	
	TPMDriverParam_t TPMdp;
	
	TPMdp.TimerNum = TIME_COUNTER_TPM_CH;
	
	/* PI begin: vecchia versione senza driver */	
	/*if(TIMECNT_C_SC_F)*/
	/* PI end: vecchia versione senza driver */
	if (OCTPM_GetChEventFlag(&TPMdp))
	{
		
		/*Always reset pending*/
	  /* PI begin: vecchia versione senza driver */			
		/*TIMECNT_C_SC_F = 0; */
		/* PI end: vecchia versione senza driver */	
		
		OCTPM_ResetChEventFlag(&TPMdp);
		
		/*At least one time input enabled*/
				
		if(Time_Input_Enabled)
		{
		
			/* Store timer input capture value*/
		  
		  /* PI begin: vecchia versione senza driver */
			/*TCM_TmrValue = (unsigned int)(TIMECNT_C_V); */
		  /* PI end: vecchia versione senza driver */
		  
		  TCM_TmrValue = OCTPM_GetChannelValue(&TPMdp); 
		
		    /* Signal acquisition (70us@10Mhz bus freq with TCM_MAX_NUM_LETTURE = 20)*/
	  	
	  	while(InputCount++ < TCM_MAX_NUM_LETTURE)
	  	{
			   InputSum += TIME_COUNTER_IC_PIN;
			}	
	    
	    /* Local filter for signal stability
		   Rising edge + high level stable*/
		  
		  /* PI begin: vecchia versione senza driver */
		  /*if((TIMECNT_C_SC & INP_CAPTURE_POS_EDGE) && (InputSum >= (TCM_MAX_NUM_LETTURE - TCM_DELTA_NUM_LETTURE )))*/
			/* PI end: vecchia versione senza driver */
			 	
			if((OCTMP_GetEdge(&TPMdp) & INPUT_CAPTURE_ON_RISING_EDGE) && (InputSum >= (TCM_MAX_NUM_LETTURE - TCM_DELTA_NUM_LETTURE )))
			{
			 	ContinueTest = TRUE;
			}
		
			// Falling edge + low level stable
		
		  /* PI begin: vecchia versione senza driver */
			//if((TIMECNT_C_SC & INP_CAPTURE_NEG_EDGE) && (InputSum <= TCM_DELTA_NUM_LETTURE ))
		  /* PI end: vecchia versione senza driver */
			
			if((OCTMP_GetEdge(&TPMdp) & INPUT_CAPTURE_ON_FALLING_EDGE) && (InputSum <= TCM_DELTA_NUM_LETTURE ))
			{
			 	ContinueTest = TRUE;
		  }
			
			if(ContinueTest)
			{
			    
			  TCM_OverflowCount = 0;
		    
		    // Calculate signal high level duration
		 	
		 		/* PI begin: vecchia versione senza driver */
		 		//if(TIMECNT_C_SC & INP_CAPTURE_NEG_EDGE)
		 		/* PI end: vecchia versione senza driver */
		 		
		 		if(OCTMP_GetEdge(&TPMdp) & INPUT_CAPTURE_ON_FALLING_EDGE)
		 		{	 
			
			 		 //Set the reference for DELTA
			
					 Tmr_Ref = Tmr_ticks;			 	    
			
					 //Timer ticks 
			
					 Tmr_ticks 	=  TCM_TmrValue - TCM_OldTmrValue;	
			
					 // 1. Wait for first 3 samples to calculate DELTA
			
					 if(N_read > 2)
					 {
					 
					 	  //2. Check for DELTA between samples  
			
					 	  if(ABS(Tmr_ticks,Tmr_Ref)> (0x01 << TimeInputsParams[index].s.Delta))
					 	  {
						
								Tmr_ticks_sum = 0;  
			
								N_read = 0;
			
								Reset_N_Read++;
			
						  }
							else
							{
			
							  //Add sample to sum
			
							  Tmr_ticks_sum += Tmr_ticks;  
							
							}
			
					 }
			
					 if(Reset_N_Read >= MAX_SUM_REPETITION)
					 {
			
					  	//if sum not possible, change channel
			
					 	  ResetVar();
			
					 	  //Set Read Alarms
			
					 	  TimeInputsAlarms |= (0x01 << index + 4 );
			
					 	
						  if(SelectNextChannel() == FALSE)
						  {
							
								// // gestione errore nessun canale enabled
			
							  Time_Input_Enabled = FALSE; 
							}
								
					 }
					 
					 //Average value calculation	
					
					 if(N_read++ > ((0x01 << TimeInputsParams[index].s.NSamples) + 1))
					 {
					 					 	
					 	  //Reset Read Alarms
					
					 	  TimeInputsAlarms &= ((0x01 << index + 4 ) ^ 0xFF);
					
					 	  //Reset Channel Alarms
					 	
					 	  HL_TimeInputsAlarms &= ((0x01 << index) ^ 0xFF);
					 	
					 	  //Average
	
						  if((TimeInputDisable & ((BYTE)1 << index)) == 0)
						  {
					 	
					 		  TimeInputs[index]= (WORD)(Tmr_ticks_sum >> TimeInputsParams[index].s.NSamples) ;
						
						  }
	
						  //Update TimeInputReady
					
						  TimeInputReady |= (0x01 << index);
					
						  //
					
						  ResetVar();
					
						  //Select next channel on multiplex
	
						  if(SelectNextChannel() == FALSE)
						  {
					
							  // gestione errore nessun canale enabled
							
							  index = 0;
					
							  ResetVar();
						  }
			 			
					 }
			 	
			 	}	 
				
				/* PI begin: vecchia versione senza driver */
				//TIMECNT_C_SC ^= TOGGLE_EDGE;   //PI: Quanto vale TOGGLE_EDGE?? 0x0C??
			 	/* PI end: vecchia versione senza driver */
			 	
			 	TPMdp.Config = TOGGLE_EDGE;
			 	
			 	OCTPM_ChannelConfig(&TPMdp);
			 	
			 	// New time reference
			    
			  TCM_OldTmrValue = TCM_TmrValue;   
			   	
			}	// End if(ContinueTest)
			
		} // End if(Time_Input_Enabled) 	

	}
	
}
#endif /* DIO_DEBUG */
/* PI end: righe rimosse tramite DIO_DEBUG solo per permettere la compilazione */

	 		 
/**
Enables or disables the module

\param [in]		IntEnable

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void TimeCounter_Enable(BYTE IntEnable)
{   /* 0 disabilita, 1 abilita*/
 	 	
 	/* PI begin: vecchia versione senza driver */
 	
    /*TIMECNT_C_SC_IE = IntEnable & 0x01; */    	 /* Abilita/Disabilita interrupt Input Capture*/
 
 	/*TPM2SC_TOIE     = IntEnable & 0x01; */   		 /* Abilita/Disabilita interrupt overflow */
	
	/* PI end: vecchia versione senza driver */
	
	TPMDriverParam_t TPMdp;
	
	/*TPMdp.State = ENABLE;*/
	
	TPMdp.State = IntEnable;
	
  OCTPM_EnableChInterrupt(&TPMdp);	 /* Abilita/Disabilita interrupt Input Capture*/
  OCTPM_EnableOFInterrupt(&TPMdp);	 /* Abilita/Disabilita interrupt overflow*/
  	
}


/**
Setta i parametri per il timer

\param [in]		pos
\param [in]		NSamples
\param [in]		Delta
\param [in]		MulConf
\param [in]		Enable
\param [in]		ReadType

\return BYTE

\retval TOOMANY_TIME_NSAMPLES, NO_TIME_INPUT_ERROR, TOOMANY_TIME_INPUT_ERROR
												 
\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

BYTE InsertTimeInput (BYTE pos,BYTE NSamples, BYTE Delta, BYTE MulConf ,BYTE Enabled ,BYTE ReadType )
{

TIME_INPUT_STRUCT *p_timeinputs_params = &TimeInputsParams[pos];

	 /*
	 pos		= 2; // terza pos vettore
	 NSamples   = 4; // 16 campioni
	 Delta		= 7; // 128 ticks per delta	
	 MulConf	= 1; // A = 1, B = 0
	 ReadType	= 1; // ?	
	 
	 */
	 
	 /*lint -e650 MAX_TIME_SAMPLES non definita nel progetto HAL, per questo causa una segnalazione da pclint */
	 if (NSamples > MAX_TIME_NSAMPLES)
	 /*lint +e650 */
	 { 

     	return TOOMANY_TIME_NSAMPLES;

     }

	 if (pos < N_TIME_INPUTS)
	 {

        p_timeinputs_params->s.Enabled 	= Enabled     & 0x01; /* 1 bit*/

		p_timeinputs_params->s.ReadType 	= ReadType    & 0x01; /* 1 bit */

       	p_timeinputs_params->s.NSamples 	= NSamples    & 0x7F; /* 7 bits */

        p_timeinputs_params->s.Delta 		= Delta  	  & 0x0F; /* 4 bits  */

        p_timeinputs_params->s.MulConf 	= MulConf     & 0x07; /* 3 bits  */

        /* Find the first enabled position*/

        if((p_timeinputs_params->s.Enabled == TRUE) && (pos <= index)){

        	index = pos;

        	/*At least one time input enabled*/

        	Time_Input_Enabled = TRUE;
        }
       
        /*28 JUNE*/

        #ifndef OVERFLOW_CONTROL
        
        if(p_timeinputs_params->s.NSamples > NSamplesMax)
        {

        	/* il tempo max di permanenza in un canale è dato dal caso peggiore
        	 di MAX_SUM_REPETITION volte ricalcolare la somma di NSamples campioni
        	 (+ 2 = margine di sicurezza)*/
        	
        	NSamplesMax = p_timeinputs_params->s.NSamples;
        	
        	count_max = (0x01 << NSamplesMax) * (MAX_SUM_REPETITION + 2);
       
        }
        
        #endif
              
        /*END 28 JUNE*/
        
        return NO_TIME_INPUT_ERROR;

     }  

     else
     {

         return TOOMANY_TIME_INPUT_ERROR;

     }       
     
} 


/**
Configura il timer selezionato in Input Capture con l'interrupt disabilitato

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void TimeCounter_InputCaptureConfig(void)
{
 	/* Configure Input Capture, interrupt not enabled*/

    /* PI begin: vecchia versione senza driver */
    /*TIMECNT_C_SC = INP_CAPTURE_NEG_EDGE;*/
    /* PI end: vecchia versione senza driver */

    TPMDriverParam_t TPMdp;
    
    TPMdp.Config = IC_ON_FALLING_EDGE;
    TPMdp.State = DISABLE;
    OCTPM_EnableChInterrupt(&TPMdp);
    OCTPM_ChannelConfig(&TPMdp);
        
    /*Input_Edge_expected = POSITIVE;*/
}


/**
Inizializza il MultiConfig

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

BYTE GetInitialMultiConfig(void){

	return(TimeInputsParams[index].s.MulConf);

}


/**
Cerca il prossimo canale

\return uint8_t

\retval TRUE, FALSE

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

uint8_t SelectNextChannel(void)
{ 

	uint8_t esito = TRUE;	
	
	if (++index >= N_TIME_INPUTS)
	{

	  	index = 0;          

	}

	while(TimeInputsParams[index].s.Enabled == 0)
	{

		/* ACHTUNG
		 Endless loop if ISR enabled
		 and no position of array is 
		 initialized with Enabled = TRUE; */

		if (++index >= N_TIME_INPUTS)
		{

	  		index = 0;

	  		if(error_count++ > EXIT_LOOP_FOR_ERROR)
	  		{

	  			esito = FALSE;

	  			break; /* EXIT from loop*/

	  		}	  

	    }
	  	        
	}

	/*  reset error index*/

	error_count = 0;
	
	/* PI begin: vecchia versione con multi e multi2 */
	/*
	#ifdef __MULTI2_H                 

		// multiplex selection  

        if(esito == TRUE){		

			if (TimeInputsParams[index].s.ReadType){

				// if not multiplexed this function is not called

			    MulSelect2 (TimeInputsParams[index].s.MulConf);
			    
			
			}	

		}	

	#endif
	*/
	/* PI end: vecchia versione senza driver */
	
	#ifdef __MULTIPLEXER_H                 

		// multiplex selection  

    if(esito == TRUE)
    {		

			if (TimeInputsParams[index].s.ReadType)
			{

				// if not multiplexed this function is not called

			   MulSelect (TimeInputsParams[index].s.MulConf, 1);  // 1 PI:ATTENZIONE!!! MuxId=1 è arbitrario. Il MuxId corretto è da selezionare
					
			}	

		}	

	#endif
	
		/* 28 JUNE
	 Setta ad 1 il bit per fault sonda staccata, resettato alla prima lettura in interrupt
	 Nel task 100ms verificare che siano tutti 0 , altrimenti dopo 200 volte (20 sec) dare l'allarme
	 CheckTimeProbe(index);
	END 28 JUNE */
	
	  
	/*
	if(esito == TRUE){
			//Reset channel alarm , corretto farlo qui? Spostato in SelectNextChannel
			TimeInputsAlarms &= (0x01 << index) ^ 0xFF;
			
	} 
	*/  
	            
	return(esito);
}


/**
Resetta le variabili

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void ResetVar(void)
{

	Reset_N_Read = 0; /*1.1.0*/

	N_read = 0;

	Tmr_ticks_sum = 0;
}

	
/**
Ritorna gli allarmi sull'ingresso

\return uint8_t

\retval TimeInputsAlarms

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

uint8_t GetTimeProbeReadAlarms(void)
{

	return(TimeInputsAlarms);

}	


/**
Ritorna gli allarmi sul canale

\return uint8_t

\retval HL_TimeInputsAlarms

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

uint8_t GetTimeProbeChannelAlarms(void)
{

	return(HL_TimeInputsAlarms);

}	


/**
Ritorna i Probe abilitati

\return uint8_t

\retval enabled_bitmap

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

uint8_t GetEnabledProbe(void){

	 uint8_t i,  enabled_bitmap = 0;

	 for (i=0;i<N_TIME_INPUTS;i++){

	/*lint -e734 PIZZOLATO 25/09/09 segnalata in questo punto una possibile perdita di informazioni dovuta al cast implicito */	
	    enabled_bitmap |= (TimeInputsParams[i].s.Enabled & 0x01) << i;
	/*lint +e734 */    

	 }	

	return(enabled_bitmap);
}	


/**
Verifica i Probe

\param  [in]		array_index

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

/*HIGH NIBBLE  read alarm*/
/*LOW NIBBLE   channel alarm  */

void CheckTimeProbe(unsigned char array_index)
{

	/*Set channel alarm 
	28 JUNE */

	TimeInputsAlarms |= ((0x01 << array_index) & 0x0F);

	/*END 28 JUNE*/

}


/**
Verifica se gli ingressi time sono pronti

\return uint8_t

\retval TRUE, FALSE

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

/* Ritorna TRUE se tutti gli ingressi time attivi letti in automatico 
 sono stati acquisiti, FALSE altrimenti*/
uint8_t  AreTimeInputsReady(void)
{

uint8_t i;

	for (i=0; i<N_TIME_INPUTS; i++)
	{

		if(TimeInputsParams[i].s.Enabled)
		{

			if((TimeInputReady & (0x01 << i)) == 0)
			{

				return (FALSE);

			}

		}

	}

	return (TRUE);	
}

	

/* PI begin: righe rimosse solo per permettere la compilazione */
#ifndef DIO_DEBUG
/**
ISR per Overflow sul timer

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

ISR (Isr_Tpm2Overflow)
{
	
	/* PI begin: vecchia versione senza driver */
	/*
	if(TPM2SC_TOF){						  //§§
	
	     TPM2SC_TOF = 0;  // reset pending
	
	     TCM_OverflowCount++;
	
	 }
	*/
	/* PI end: vecchia versione senza driver */
	
	TPMDriverParam_t TPMdp;
	
	TPMdp.TimerNum = TIME_COUNTER_TPM_CH;
	
	if(OCTPM_GetOFFlag(&TPMdp))
	{
	
	   OCTPM_ResetOFFlag(&TPMdp);  // reset pending
	
	   TCM_OverflowCount++;
	
	 }
 
} 
#endif /* DIO_DEBUG */
/* PI end: righe rimosse solo per permettere la compilazione */

 

/**
Inizializza gl ingressi per il timer

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void InitTimeInput (void)
{
#if 0

	uint8_t i;
	TIME_INPUT_STRUCT *p_timeinputs_params;


	for (i=0; i<N_TIME_INPUTS; i++) 
  {
		
		TimeInputs[i]=0;
		
		p_timeinputs_params = &TimeInputsParams[i];
		
		p_timeinputs_params->s.Enabled 	= 0;

		p_timeinputs_params->s.ReadType 	= 0;

    p_timeinputs_params->s.NSamples 	= 0;

    p_timeinputs_params->s.Delta 		= 0;

    p_timeinputs_params->s.MulConf 	= 0;
		
	}
	
	TimeInputReady		= 0;
	TimeInputDisable 	= 0;
#endif /* 0*/
	
	memset(&TimeInputs[0],0,sizeof(TimeInputs[0])*N_TIME_INPUTS);
	memset(&TimeInputsParams[0],0,sizeof(TimeInputsParams[0])*N_TIME_INPUTS);
	
}


/**
Manager per il contatore

\author 	Pierluigi Ianni
\date	  21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void TimeCounter_Manager (void)
{

#ifdef OVERFLOW_CONTROL
	
	if(TCM_OverflowCount > MAX_NUM_OVERFLOW)
	{ 
	
		HL_TimeInputsAlarms |= ((0x01 << index) & 0x0F);
		
		ResetVar();
			
		/*lint -e534 PIZZOLATO 25/09/09 ignorato il valore di ritorno di SelectNextChannel() */	
		SelectNextChannel();
		/*lint +e534 */
		TCM_OverflowCount = 0;
	}
			
#else
	
	if(index == old_index)
	{
	
		if(count++ >= count_max)
		{

			HL_TimeInputsAlarms |= ((0x01 << index) & 0x0F);
			
			ResetVar();
			
			SelectNextChannel();
			
			count = 0;

		}

	}

	old_index = index;

#endif
	
}

/*END 28 JUNE*/


/* *********** */
/* End of file */
/* *********** */


/* FAULT SONDE

b4,b5,b6,b7 of TimeInputsAlarms = 1 for x seconds --> instable reading 

b4,b5,b6,b7 set to 1 and reset to 0 both by Timecounter ISR

b0,b1,b2,b3   reset to 0 by Timecounter ISR 
b0,b1,b2,b3   MUST BE SETTLED to 1 by CheckTimeProbe

*/

