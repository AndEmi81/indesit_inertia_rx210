/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	    PM_FreqMeter 
@{																																					  
\n		        Il modulo fornisce i metodi per la gestione degli interrupt 
\n				di input capture e di overflow dei Timer 1 e 2
\n
\par Configuration:
\todo
\par Exported Function:
\todo
@}
\ingroup        ProductM
*/
/**
\n
\Par		    Source file gestione FreqMeter
\n			    Contiene le funzioni di gestione degli interrupt \n
di input capture e di overflow dei Timer 1 e 2
\n
\file		    FreqMeter.c
\ingroup	    PM_FreqMeter
\date		    21/07/2009
\version	    00.00.01
\author		    Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n 		        00.00.01
\n          	21/07/2009
\n          	Pierluigi Ianni
\n              - Modificate le funzioni che avevano accesso diretto all'HW dei microcontrollori \n
con le chiamate alle funzioni del driver
\n              - Aggiornata la documentazione
\n
\n
\n 		        1.0.0
\n          	14/01/2008
\n          	Alberto Camerin,Roberto Fioravanti,Stefano Ughi
\n          	Seconda versione
\n
\n
\n 		        1.0.0
\n          	20/12/2005
\n          	Alberto Camerin
\n          	Prima versione
\n
*/


/* ******** */
/* Includes */
/* ******** */

#include "debug.h"

#include "OCTPM_drv.h"
#include "FreqMeter.h"
/* PI begin: rimosso solo per permettere la compilazione */
#ifndef DIO_DEBUG
#include "ProductConfig.h"
/*#else
#include "StandardDefines.h" */
/* PI: aggiunto solo per permettere la compilazione */
#endif /* DIO_DEBUG */
/* PI end: rimosso solo per permettere la compilazione */
#include "SkeletonGlobal.h"

/*#include "string.h"*/


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* PI begin: rimosso per permettere la compilazione */
/*#ifndef TPM2_PS
	#error "FreqMeter module Error:\n This module requires TPM2 timer to be configured.\
 	Define TPM2_PS in ProductConfig.h file" 
#endif
*/
/* PI end: rimosso per permettere la compilazione */


/* The Prescaler for Timer 1 is imposed by OSEK  tick timer  --> 12.804us  */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct {
    /** Ctrl byte for the channel */	
	union {						   
  		uint8_t all_Data;
  		struct {
		    uint8_t Enable    	:1;
		    uint8_t OverflowPendingDuringFirstISRCapture:1;
		    uint8_t BIT_2   	:1;
		    uint8_t BIT_3   	:1;
		    uint8_t BIT_4   	:1;
		    uint8_t BIT_5   	:1;
		    uint8_t BIT_6   	:1;
		    uint8_t BIT_7   	:1;
	  	} Ctrl;
	} CHCtrl;
/** Hook routine for the ISR of the channel */
							 /* channel, valore della cattura, ovfFlag */
	uint8_t (*ChannelManager) ( uint8_t, uint16_t, uint8_t );  
	
	uint8_t NumberOfOverflows;	 	/* Number of overflow */

/** Hook routine to manage overflow */
							 /* channel, n° overflows */
	uint8_t (*OverflowManager)( uint8_t, uint8_t );  
	
} FreqMeter_ChConf_t;

/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* Global */

/** Vettore di abilitazione dei canali all'interno del modulo */
/*const uint8_t FreqMeterChTPMEnable = MCUTPMChEnable; diventata una define in FreqMeter.h*/

/** Vettore di configurazione dei canali all'interno del modulo */
/*const uint8_t FreqMeterChTPMSCConfig = MCUTPMChConfig; diventata una define in FreqMeter.h*/

/* PI begin: dove lo mettiamo? */
/*
#if defined(BUILD_FREESCALE_GB60)

#if (FREQMETER_CTRL_VAR_ON_ZERO_PAGE == 1)
    #pragma DATA_SEG __SHORT_SEG _DATA_ZEROPAGE
#endif*/ /*(FREQMETER_CTRL_VAR_ON_ZERO_PAGE == 1)*/
/*#endif*/ /* BUILD_FREESCALE_GB60 */
/*
 PI end: dove lo mettiamo? */

static FreqMeter_ChConf_t FreqMeterCtrlVarTPM[FREQMETER_NUMOFCHANNEL];

/* PI begin: dove lo mettiamo? */
/*
#if defined(BUILD_FREESCALE_GB60)

#if (FREQMETER_CTRL_VAR_ON_ZERO_PAGE == 1)
    #pragma DATA_SEG DEFAULT_RAM
#endif*/ /* (FREQMETER_CTRL_VAR_ON_ZERO_PAGE == 1) */
/*#endif*/ /* BUILD_FREESCALE_GB60 */

/* PI end: dove lo mettiamo? */


/* ******************* */
/* Function Prototypes */
/* ******************* */
/* PI begin: aggiunte a FreqMeter.h e rimosse da qui perchè usate in interrupt.c */
/*
static void FreqMeter_ManageISR(uint8_t timer_num,uint16_t value, uint8_t ovfFlag);
static void Isr_TPM_common(uint8_t i); 
*/
/* PI end: aggiunte a FreqMeter.h perchè usate in interrupt.c */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Ritorna timer_# dato un pin 

\param [in] pin, pin del modulino

\return uint8_t 
\retval timer_# (contiene l'id del timer), 0xFF on *timer if errors

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/

uint8_t FreqMeter_GetTimerChannelFromPin(uint8_t pin)
{

#if (FREQMETER_DISABLE_MODULE == 0)

	uint8_t channel;

	for(channel = 0; channel < FREQMETER_NUMOFCHANNEL ; channel++)
	{
		if(pin == (timersMap[channel].McuPin))
		{
			return channel;
		}
	}

	return 0xFF;


#endif /*(FREQMETER_DISABLE_MODULE == 0)*/

}


/**
La funzione viene chiamata indipendentemente dal tipo di reset prima della InputInitialization()
Inizializzazione dei canali relativi al timer 2 e al timer 1 in input capture. \n
Per tutti i canali abilitati ->  FreqMeterChTPMEnable[i] != 0  \n
viene eseguito il setup della struttura FreqMeterCtrlVarTPM[]
 
\attention \b FreqMeterCtrlVarTPM[] used as I/O

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/

void FreqMeter_Init( void )
{

#if (FREQMETER_DISABLE_MODULE == 0)

	uint8_t channel;

	TPMDriverParam_t TPMdp;


	for(channel=0; channel < FREQMETER_NUMOFCHANNEL; channel++)
	{
			

	  FreqMeterCtrlVarTPM[channel].CHCtrl.Ctrl.Enable = 0;
	
		if(FreqMeterChTPMEnable[channel])
		{
			FreqMeterCtrlVarTPM[channel].NumberOfOverflows 	= 0;
			FreqMeterCtrlVarTPM[channel].OverflowManager 		= NULL;
			FreqMeterCtrlVarTPM[channel].ChannelManager 		= NULL;


      		/* !!!ATTENZIONE!!!
		    La versione senza driver inizializza:
       		- GB60 tutto il registro TPMyCxSC, cioè input capture/output compare
	        e sensibilità sull'Edge, IRQ e IRQFlag.
    	    - H8 solo input capture/output compare e sensibilità sull'Edge.
       
	        Qui inizializzo solo input capture/output compare e sensibilità sull'Edge */

      		TPMdp.TimerNum = channel;

      		TPMdp.Config = MCUTPMChConfig[channel];
			
			OCTPM_ChannelConfig(&TPMdp);


/* PI begin: vecchia versione no driver */
/*
#if defined(BUILD_FREESCALE_GB60)

			*(uint8_t * const)FreqMeter_Channel_SCReg[i] = (FreqMeterChTPMSCConfig[i]);
#endif
#if defined(BUILD_RENESAS_36079)
			if(!(i&1))				
			{
				(*(TIORxSTR * const)(FreqMeter_IO_CtrlReg[i]+FREQMETER_REGS_ADDR)).Bit.IOL = FreqMeterChTPMSCConfig[i];
			}
			else
			{
				(*(TIORxSTR * const)(FreqMeter_IO_CtrlReg[i]+FREQMETER_REGS_ADDR)).Bit.IOH = FreqMeterChTPMSCConfig[i];
			}
#endif			
*/
/* PI end: vecchia versione no driver */
		}

	}

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/	
	
}


/**
Abilita/disabilita l'utilizzo del canale timer_num.  

\param [in] timer_num canale del timer
\param [in] status  '1' -> abilitazione '0' -> disabilitazione
\attention \b FreqMeterCtrlVarTPM[] used as I/O

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/

void FreqMeter_SetChannelStatus(uint8_t timer_num, uint8_t status)
{

#if (FREQMETER_DISABLE_MODULE == 0)


	FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.Enable = status;

	
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/	
	
}


/**
Dato il canale del timer ritorna se è abilitato o meno. 

\param  [in]		timer_num canale del timer

\attention \b FreqMeterCtrlVarTPM[] used as Output

\return 	uint8_t
\retval		'1' -> canale abilitato '0' canale disabilitato 	 

\author 	Ignoto
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
uint8_t FreqMeter_GetChannelStatus(uint8_t timer_num)
{
	
#if (FREQMETER_DISABLE_MODULE == 0)

	return(FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.Enable);
	
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/	
	
}

/**
Consente di abilitare o disabilitare l'ISR in input capture

\param [in] timer_num, numero del timer selezionato
\param [in] state, abilita/disabilita

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/

void FreqMeter_SetIsrInputCapture(uint8_t timer_num, uint8_t state) 
{

#if (FREQMETER_DISABLE_MODULE == 0)

/* PI begin: nuova versione con driver */
  
  TPMDriverParam_t TPMdp;

  TPMdp.TimerNum = timer_num;
  TPMdp.State = state;

  OCTPM_ResetChEventFlag(&TPMdp);
  OCTPM_EnableChInterrupt(&TPMdp);
/* PI end: nuova versione con driver */

/* PI begin: vecchia versione senza driver */
/*
#if defined(BUILD_FREESCALE_GB60)
  (*(TPMxCxSCSTR *)(FreqMeter_Channel_SCReg[timer_num])).Bits.CHxF = 0;
  (*(TPMxCxSCSTR *)(FreqMeter_Channel_SCReg[timer_num])).Bits.CHxIE = state;
#endif
*/
/* PI end: vecchia versione senza driver */

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/

}


/**
Per il canale selezionato esegue il setup del fronte di sensibilità all'interrupt. 

\param  [in]		timer_num canale selezionato
\param  [in]		channelConfig valori ammissibili \n
                    INPUT_CAPTURE_ON_RISING_EDGE \n
                    INPUT_CAPTURE_ON_FALLING_EDGE \n
                    INPUT_CAPTURE_ON_BOTH_EDGE 

\attention \b FreqMeter_Channel_SCReg[] used as I/O

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/
void FreqMeter_SetEdge(uint8_t timer_num, uint8_t channelConfig)
{

#if (FREQMETER_DISABLE_MODULE == 0)

  TPMDriverParam_t TPMdp;

  TPMdp.TimerNum = timer_num;

  TPMdp.Config = channelConfig;
	
	OCTPM_ChannelConfig(&TPMdp);

/* PI begin: vecchia versione senza driver */
/*
#if defined(BUILD_FREESCALE_GB60)
	
uint8_t aux_byte = 0;

	aux_byte = (*(TPMxCxSCSTR *)(FreqMeter_Channel_SCReg[timer_num])).all_Data & 0xC0;
    (*(TPMxCxSCSTR *)(FreqMeter_Channel_SCReg[timer_num])).all_Data = (aux_byte | edge );
#endif
#if defined(BUILD_RENESAS_36079)	
	SuspendAllInterrupts();
	if(!(timer_num&1))				
	{  
		(*(TIORxSTR * const)(FreqMeter_IO_CtrlReg[timer_num]+FREQMETER_REGS_ADDR)).Bit.IOL = edge;
	}
	else
	{
		(*(TIORxSTR * const)(FreqMeter_IO_CtrlReg[timer_num]+FREQMETER_REGS_ADDR)).Bit.IOH = edge;
	}
	ResumeAllInterrupts();
#endif
*/
/* PI end: vecchia versione senza driver */	
	
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/
	
}


/**
Ritorna su quale fronte è settato l'interrupt per il canale del timer specificato. 

\param  [in]		timer_num canale del timer

\attention \b FreqMeter_Channel_SCReg[] used as output 	 build HCS08
\attention \b FreqMeter_IO_CtrlReg[] used as output 	 build H83607x

\return 	uint8_t 

\retval		0
            INPUT_CAPTURE_ON_RISING_EDGE	
            INPUT_CAPTURE_ON_FALLING_EDGE       
            INPUT_CAPTURE_ON_BOTH_EDGE		    
 	 
\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
*/

uint8_t FreqMeter_GetEdge( uint8_t timer_num )
{

  TPMDriverParam_t TPMdp;
  
  TPMdp.TimerNum = timer_num;
	
	return OCTMP_GetEdge(&TPMdp);

/* PI begin: vecchia versione senza driver */
/*
#if defined(BUILD_FREESCALE_GB60)
    return ( (*(TPMxCxSCSTR *)(FreqMeter_Channel_SCReg[timer_num])).all_Data & 0x0C );
#endif
*/
/* PI end: vecchia versione senza driver */

}


/**
Abilita/disabilita gli interrupt overflows dei Timer. 

\param  [in]		timer_num  canale del timer
\param  [in]		state 
                      
\attention \b MCU Registers utilizzati

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			  documentation by Pierluigi Ianni
  			\par \b Nota 4:
	  		define on state \n
		  	All target BIT7 = 1 abilita BIT7 = 0 disabilita \n
			  HCS08   BIT4-BIT0 don't care \n
			  H836079 BIT4=OVIE,BIT0=IMIEA
*/

/* PI begin: questo lo mettiamo? */
/*
#if defined(BUILD_FREESCALE_GB60)
#pragma INLINE
#endif
*/
/* PI end: questo lo mettiamo? */

/* PI begin: questo dove lo mettiamo? */
/*
#if defined(BUILD_RENESAS_36079)
#pragma inline(FreqMeter_ManageIsrOverflow)
#endif*/
/* PI end: questo dove lo mettiamo? */

void FreqMeter_ManageIsrOverflow(uint8_t timer_num,uint8_t state)
{

#if (FREQMETER_DISABLE_MODULE == 0)

  TPMDriverParam_t TPMdp;
  
  TPMdp.TimerNum = timer_num;
  TPMdp.State = (state&BIT7_MASK);
    
  OCTPM_ResetOFFlag(&TPMdp);
  OCTPM_EnableOFInterrupt(&TPMdp);

/* PI begin: vecchia versione senza driver */

/*!!!ATTENZIONE!!! Manca la dummy read per azzerate il TOF??? */

/*#if defined(BUILD_FREESCALE_GB60)

    if ( timer_num > (FREQMETER_TPM1_NUMOFCHANNEL-1) )
    {
        TPM2SC_TOF  = 0;
        TPM2SC_TOIE = ((state&BIT7_MASK)?1:0);
    }
    else
    {
        TPM1SC_TOF  = 0;
        TPM1SC_TOIE = ((state&BIT7_MASK)?1:0);
    }
#endif */ /* BUILD_FREESCALE_GB60 is defined */

/* PI end: vecchia versione senza driver */
  
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/
  
}


/**
Ritorna gli overflows del counter associato al timer (1 o 2). 

\param  [in]		timer_num canale selezionato
\param  [in]		reset se !=0 resetta il contatore degli overflows relativi a timer_num

\attention \b FreqMeterCtrlVarTPM[] used as I/O

\return 	uint8_t
\retval		n

\author 	Ignoto
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/

uint8_t FreqMeter_GetOverflow(uint8_t timer_num,uint8_t reset)
{

#if (FREQMETER_DISABLE_MODULE == 0)

uint8_t aux;

	if( FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.Enable == 0 )
	{	
		FreqMeterCtrlVarTPM[timer_num].NumberOfOverflows = 0;
		return 0;
	}

	aux = FreqMeterCtrlVarTPM[timer_num].NumberOfOverflows;

	if(reset)
	{
		FreqMeterCtrlVarTPM[timer_num].NumberOfOverflows = 0;
	}

	return aux;

#endif /* (FREQMETER_DISABLE_MODULE == 0) */

}


/**
Setup del modulo per un determinato timer con l'inserzione della funzione
che ne gestirà l'interrupt di cattura e la funzione che ne gestirà l'interrupt 
dell'overflow counter. 

\param  [in]	timer_num canale del timer
\param  [in]    fOverflow puntatore alla funzione gestione isr Overflow Counter
\param  [in]    fManager puntatore alla funzione gestione isr input capture

\attention \b UTILIZZARE PER L'EVENTUALE UTILIZZO DI VARIABILI GLOBALI

\author 	Alberto Camerin
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void FreqMeter_SetManagerFunctions( uint8_t timer_num, 
								    uint8_t(*fOverflow)(uint8_t,uint8_t),
								    uint8_t(*fManager)(uint8_t, uint16_t, uint8_t) )
{

#if (FREQMETER_DISABLE_MODULE == 0)

	FreqMeterCtrlVarTPM[timer_num].OverflowManager = fOverflow;
			
	FreqMeterCtrlVarTPM[timer_num].ChannelManager = fManager;

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/

}


/**
Setta flag OverflowPendingDuringFirstISRCapture

\param [in] timer_num canale timer 

\attention \b FreqMeterCtrlVarTPM[] used as I/O

\author 	Stefano Ughi
\date		01/02/2006
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void FreqMeter_SetOverflowFlag(uint8_t timer_num )
{

#if (FREQMETER_DISABLE_MODULE == 0)

  FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.OverflowPendingDuringFirstISRCapture = 1;
    
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/ 
    
}


/**
Resetta flag OverflowPendingDuringFirstISRCapture

\param [in]  timer_num  canale su cui si deve fare il clear del flag

\attention \b FreqMeterCtrlVarTPM[] used as I/O

\author 	Alberto Camerin
\date		01/02/2006
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void FreqMeter_ResetOverflowFlag(uint8_t timer_num)
{

#if (FREQMETER_DISABLE_MODULE == 0)

	FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.OverflowPendingDuringFirstISRCapture = 0;
    
#endif /*(FREQMETER_DISABLE_MODULE == 0)*/
    
}


/**
Resetta il contatore degli overflows relativi al canale selezionato.

\param [in]  timer_num canale selezionato

\attention \b FreqMeterCtrlVarTPM[] used as I/O

\author 	Stefano Ughi
\date		02/02/2006
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void FreqMeter_ResetOverflow( uint8_t timer_num)
{

#if (FREQMETER_DISABLE_MODULE == 0)

	FreqMeterCtrlVarTPM[timer_num].NumberOfOverflows = 0;

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/

}


/**
Ritorna gli overflow relativi al canale selezionato.

\param [in] timer_num canale selezionato

\attention \b FreqMeterCtrlVarTPM[] used as I

\author 	Stefano Ughi
\date		02/02/2006
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
*/
uint8_t FreqMeter_GetOverflowChannel(uint8_t timer_num)
{

#if (FREQMETER_DISABLE_MODULE == 0)

	return(FreqMeterCtrlVarTPM[timer_num].NumberOfOverflows);

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/

}


/**
Configura il registro di stato e controllo corrispondente al canale del timer selezionato \n
in base alla proprietà FREQMETER_SC_INIT_VAL_TPMxCHy 
cfr. FreqMeter.h

\param [in] timer_num canale del timer

\attention \b FreqMeter_Channel_SCReg[] used as I/O

\author 	Pierluigi Ianni
\date		21/07/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void FreqMeter_ConfigTimerCntrlRegister( uint8_t timer_num )
{

#if (FREQMETER_DISABLE_MODULE == 0)
	
  TPMDriverParam_t TPMdp;
  
  TPMdp.TimerNum = timer_num;
  TPMdp.Config = FreqMeterChTPMSCConfig[timer_num];

	OCTPM_ChannelConfig(&TPMdp);

/* PI begin: vecchia versione senza driver */
/*
#if defined(BUILD_FREESCALE_GB60)
	  *(uint8_t * const)FreqMeter_Channel_SCReg[timer_num] = (FreqMeterChTPMSCConfig[timer_num]);
	Istruzione uguale a riga sopra
	*(uint8_t * const)FreqMeter_Channel_SCReg[i]         = (FreqMeterChTPMSCConfig[i]);
#endif	
*/
/* PI end: vecchia versione senza driver */

#endif /*(FREQMETER_DISABLE_MODULE == 0)*/
	
}


/* Local Function */


/**
Chiamata della funzione agganciata al canale del timer selezionato, la chiamata
è effettuata dall'interrupt.  

\param  [in]		timer_num canale su cui c'è il focus
\param  [in]		value valore della cattura in tick del timer
\param  [in]		ovfFlag indica se è stato intercettato un'oveflow del counter.

\attention \b FreqMeterCtrlVarTPM[] used

\author 	Alberto Camerin
\date		gg/mm/aaaa
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
/*PI rivedere tutto*/
/*static*/ void FreqMeter_ManageISR(uint8_t timer_num,uint16_t value, uint8_t ovfFlag)
{
#ifdef  INPUT_FAILURE_DETECTION

	uint8_t pin;
	pin=FreqMeter_Pin[timer_num];

	if (InputFailureDetectionBitmap[pin/BYTE_SIZE]&(0x01 << (pin%BYTE_SIZE))) 
	{	    
		return;
	}
#endif

	if ( ( FreqMeterCtrlVarTPM[timer_num].CHCtrl.Ctrl.Enable ) && 
	     ( FreqMeterCtrlVarTPM[timer_num].ChannelManager != NULL ) )
	{
		(void)(FreqMeterCtrlVarTPM[timer_num].ChannelManager)(timer_num,value,ovfFlag);		
	}	

}
  
/**
Codice comune per la gestione ISR Overflow dei timer 1 e 2, chiamata negli ISR degli overflow counter

\param [in]     i canale 

\author 	Alberto Camerin
\date		30/10/2006
\version	1.0.0

\note non c'è controllo su channel perchè solo lettura.  
*/
/*PI rivedere tutto*/
/*static*/ void Isr_TPM_common(uint8_t i) 
{
    if(FreqMeterCtrlVarTPM[i].CHCtrl.Ctrl.Enable)
    {			
        if(FreqMeterCtrlVarTPM[i].CHCtrl.Ctrl.OverflowPendingDuringFirstISRCapture == 0)
        {
            if ( FreqMeterCtrlVarTPM[i].NumberOfOverflows < 0xFF )
            {
                FreqMeterCtrlVarTPM[i].NumberOfOverflows++;
            }
        }
        FreqMeterCtrlVarTPM[i].CHCtrl.Ctrl.OverflowPendingDuringFirstISRCapture = 0;

        if(FreqMeterCtrlVarTPM[i].OverflowManager != NULL)
        {				
            (void)(*FreqMeterCtrlVarTPM[i].OverflowManager)(i,FreqMeterCtrlVarTPM[i].NumberOfOverflows);
        }
    }	
}


/**
Gestione degli overflows sul Timer 2
*/  
#if !defined(DIO_DEBUG)
#if defined(BUILD_FREESCALE_GB60)
ISR (Isr_Tpm2Overflow)
#endif	
#if defined(BUILD_RENESAS_36079)
void FreqMeterTimerManager(void)
#endif
{
uint8_t i;
#if defined(BUILD_RENESAS_36079)
uint16_t value;
uint8_t  OvfFlag = 0;
#endif

#if defined(BUILD_FREESCALE_GB60)
	OCTPM_ResetOFFlag(timer_num);   /* reset pending */
	//PI rivedere
	//TPM2SC_TOF = 0;  /* reset pending */
#endif		
#if defined(BUILD_RENESAS_36079)
/*Renesas: per ogni canale viene ritestato il flag di overflow (nel caso si setti mentre 
  si è già nell'interrupt per altri motivi); se individuato si setta un flag che segnala, 
  anche nelle operazioni successive, l'avvenuto overflow; l'overflow viene poi gestito solo 
  al termine (compatibilità Freescale - evita problemi con interrupt contemporanei)*/
	if( TMRZ1.TSR1.BIT.OVF )
	{	
		TMRZ1.TSR1.BIT.OVF=0;
		OvfFlag = 1;
	}
    #if (FREQMETER_TPM2_CH0_ENABLE)
    if (TMRZ1.TSR1.BIT.IMFA)
    {	
    	value = TMRZ1.GRA1;
    	TMRZ1.TSR1.BIT.IMFA =0;
    	if( TMRZ1.TSR1.BIT.OVF )
		{				
			TMRZ1.TSR1.BIT.OVF=0;
			OvfFlag = 1;
		}
    	FreqMeter_ManageISR(2,value,OvfFlag);
    }
    #elif defined ( OVFL_ON_GRA1 )
    if (TMRZ1.TSR1.BIT.IMFA)
    {	
    	
    	TMRZ1.TSR1.BIT.IMFA =0;
    	/* insert code for overflow's managing */
      	#if defined (DEBUG_RENESAS_36079) && defined (TIMERZ1_DEBUG)
	    PDR2_Mask ^= BIT0_MASK;
        PORT.PDR2.BYTE = PDR2_Mask;
        #endif			

    }    
    #endif /*(FREQMETER_TPM2_CH0_ENABLE == 1)*/

    #if (FREQMETER_TPM2_CH1_ENABLE)
    if (TMRZ1.TSR1.BIT.IMFB)
    {	
    	value = TMRZ1.GRB1;
    		    
    	TMRZ1.TSR1.BIT.IMFB =0;
    	if( TMRZ1.TSR1.BIT.OVF )
		{				
			TMRZ1.TSR1.BIT.OVF=0;
			OvfFlag = 1;
		}
    	
    	FreqMeter_ManageISR(3,value,OvfFlag);
    }
    #endif /*(FREQMETER_TPM2_CH1_ENABLE == 1)*/

    #if (FREQMETER_TPM2_CH2_ENABLE)
    if (TMRZ1.TSR1.BIT.IMFC)
    {	
    	value = TMRZ1.GRC1;
    		    
    	TMRZ1.TSR1.BIT.IMFC =0;
    	
    	if( TMRZ1.TSR1.BIT.OVF )
		{				
			TMRZ1.TSR1.BIT.OVF=0;
			OvfFlag = 1;
		}
    	
    	FreqMeter_ManageISR(4,value,OvfFlag);
    }
    #endif /*(FREQMETER_TPM2_CH2_ENABLE == 1)*/

    #if (FREQMETER_TPM2_CH3_ENABLE)
    if (TMRZ1.TSR1.BIT.IMFD)
    {	
    	value = TMRZ1.GRD1;
    		    
    	TMRZ1.TSR1.BIT.IMFD =0;
    	
    	 if( TMRZ1.TSR1.BIT.OVF )
		{				
			TMRZ1.TSR1.BIT.OVF=0;
			OvfFlag = 1;
		}
    	FreqMeter_ManageISR(5,value,OvfFlag);
    }
    #endif /*(FREQMETER_TPM2_CH3_ENABLE == 1)*/
    
    if (OvfFlag == 1)
    {
 
#endif /* BUILD_RENESAS_36079 is defined */
		for( i=(FREQMETER_TPM1_NUMOFCHANNEL+1); i<(FREQMETER_TPM1_NUMOFCHANNEL+FREQMETER_TPM2_NUMOFCHANNEL); i++)
		{
	    	Isr_TPM_common(i); 
		}
#if defined(BUILD_RENESAS_36079)
	}
#endif /* BUILD_RENESAS_36079 is defined */
} 
/**
Gestione Overflows sul Timer 1
*/
#if osdTimerHook
#if defined(BUILD_RENESAS_36079)
void TimerHook( void)
#endif
#endif 
#if defined(BUILD_FREESCALE_GB60)
/*lint -esym(533, OSISRIsr_Tpm1Overflow) */	  
ISR (Isr_Tpm1Overflow)
#endif 
{
uint8_t i;

#if defined(BUILD_RENESAS_36079)
uint16_t value;
uint8_t  OvfFlag = 0; 
#endif /* BUILD_RENESAS_36079 is defined */
	

#if defined(BUILD_FREESCALE_GB60)
		OCTPM_ResetOFFlag(timer_num);   /* reset pending */
		//PI rivedere
    //TPM1SC_TOF = 0;  /* reset pending */
#endif 

#if defined(BUILD_RENESAS_36079)
/*Renesas: per ogni canale viene ritestato il flag di overflow (nel caso si setti mentre 
  si è già nell'interrupt per altri motivi); se individuato si setta un flag che segnala, 
  anche nelle operazioni successive, l'avvenuto overflow; l'overflow viene poi gestito solo 
  al termine (compatibilità Freescale - evita problemi con interrupt contemporanei)*/

	if(TMRZ0.TSR0.BIT.OVF)		
    {
        OvfFlag = 1;
        TMRZ0.TSR0.BIT.OVF=0;
    }
    
#if (FREQMETER_TPM1_CH2_ENABLE)
    if (TMRZ0.TSR0.BIT.IMFC)
    {	
    	value = TMRZ0.GRC0;
    	TMRZ0.TSR0.BIT.IMFC =0;
		if(TMRZ0.TSR0.BIT.OVF)		
	    {
	        OvfFlag = 1;
	        TMRZ0.TSR0.BIT.OVF=0;
	    }
    	FreqMeter_ManageISR(0,value,OvfFlag);
    }
#endif /*(FREQMETER_TPM1_CH2_ENABLE == 1)*/

#if (FREQMETER_TPM1_CH3_ENABLE)
    if (TMRZ0.TSR0.BIT.IMFD)
    {	
    	value = TMRZ0.GRD0;
    	TMRZ0.TSR0.BIT.IMFD =0;
		if(TMRZ0.TSR0.BIT.OVF)		
	    {
	        OvfFlag = 1;
	        TMRZ0.TSR0.BIT.OVF=0;
	    }
    	FreqMeter_ManageISR(1,value,OvfFlag);
    }
#endif /*(FREQMETER_TPM1_CH3_ENABLE == 1)*/

	if (OvfFlag == 1)
	{
#endif /* BUILD_RENESAS_36079 is defined */

	     
    	for ( i=0; i<FREQMETER_TPM1_NUMOFCHANNEL; i++ )
    	{
        	Isr_TPM_common(i); 
    	}
#if defined(BUILD_RENESAS_36079)
	}
#endif /* BUILD_RENESAS_36079 is defined */


} 


/* HCS08 Timer 1 */
#if defined(BUILD_FREESCALE_GB60)

#if (FREQMETER_TPM1_CH0_ENABLE == 1)
ISR (Isr_FM_TPM1CH0_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM1SC_TOF;
	
	value = TPM1C0V;	/* valore della cattura */
    /* sequenza per il clear del pending */
	TPM1C0SC;	        /* dummy read */
	TPM1C0SC_CHxF = 0;  /* clear pending */
	FreqMeter_ManageISR(0,value,ovfFlag);
}
#endif /*(FREQMETER_TPM1_CH0_ENABLE == 1)*/

#if (FREQMETER_TPM1_CH1_ENABLE == 1)
ISR (Isr_FM_TPM1CH1_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM1SC_TOF;
	
	value = TPM1C1V;   /* valore della cattura */
    /* sequenza per il clear del pending */
	TPM1C1SC;		   /* dummy read */
	TPM1C1SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(0,value,ovfFlag);
}
#endif /*(FREQMETER_TPM1_CH1_ENABLE == 1)*/

#if (FREQMETER_TPM1_CH2_ENABLE == 1)
/*lint -esym(533,OSISRIsr_FM_TPM1CH2_IC)*/
ISR (Isr_FM_TPM1CH2_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM1SC_TOF;
	
	value = TPM1C2V;  /* valore della cattura */
    /* sequenza per il clear del pending */
	(void)TPM1C2SC;		   /* dummy read */
	TPM1C2SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(1,value,ovfFlag);
}
#endif /*(FREQMETER_TPM1_CH2_ENABLE == 1)*/

/* HCS08 Timer 2 */

#if (FREQMETER_TPM2_CH0_ENABLE == 1)
/*lint -esym(533,OSISRIsr_FM_TPM2CH0_IC)*/
ISR (Isr_FM_TPM2CH0_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM2SC_TOF;
	
	value = TPM2C0V;  /* valore della cattura */
    /* sequenza per il clear del pending */		
    (void)TPM2C0SC;		   /* dummy read */
	TPM2C0SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(2,value,ovfFlag);
}
#endif /*(FREQMETER_TPM2_CH0_ENABLE == 1)*/


#if (FREQMETER_TPM2_CH1_ENABLE == 1)
ISR (Isr_FM_TPM2CH1_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM2SC_TOF;
	
	value = TPM2C1V;  /* valore della cattura */
    /* sequenza per il clear del pending */		
    TPM2C1SC;		   /* dummy read */
	TPM2C1SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(3,value,ovfFlag);
}
#endif /*(FREQMETER_TPM2_CH1_ENABLE == 1)*/

#if (FREQMETER_TPM2_CH2_ENABLE == 1)
ISR (Isr_FM_TPM2CH2_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM2SC_TOF;
	
	value = TPM2C2V;  /* valore della cattura */
    /* sequenza per il clear del pending */		
    TPM2C2SC;		   /* dummy read */
	TPM2C2SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(4,value,ovfFlag);
}
#endif /*(FREQMETER_TPM2_CH2_ENABLE == 1)*/

#if (FREQMETER_TPM2_CH3_ENABLE == 1)
ISR (Isr_FM_TPM2CH3_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM2SC_TOF;
	
	value = TPM2C3V;  /* valore della cattura */
    /* sequenza per il clear del pending */		
    TPM2C3SC;		   /* dummy read */
	TPM2C3SC_CHxF = 0; /* clear pending */
	FreqMeter_ManageISR(5,value,ovfFlag);
}
#endif /*(FREQMETER_TPM2_CH3_ENABLE == 1) */

#if (FREQMETER_TPM2_CH4_ENABLE == 1)
ISR (Isr_FM_TPM2CH4_IC)
{
uint16_t value;
uint8_t  ovfFlag = TPM2SC_TOF;
	
	
	value = TPM2C4V;  /* valore della cattura */
    /* sequenza per il clear del pending */			
	TPM2C4SC;		   /* dummy read */
	TPM2C4SC_CHxF = 0; /* clear pending */	
	FreqMeter_ManageISR(6,value,ovfFlag);
}

#endif /* (FREQMETER_TPM2_CH4_ENABLE == 1) */

#endif /* BUILD_FREESCALE_GB60 is defined */

#endif /* DIO_DEBUG is defined */

/* *********** */
/* End of file */
/* *********** */
