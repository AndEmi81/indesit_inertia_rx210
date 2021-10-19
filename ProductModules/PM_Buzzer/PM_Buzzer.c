/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_Buzzer 
@{
\n	This module Manage the Buzzer function trought a PWM exit pin
\n
\n
\par    Configuration: 
\n
\n\li	  none
\n
\par    Functions:
\n\li   void PM_BuzzerInit ( uint8_t pin )
\n\li   void PM_SetBuzzer (uint8_t beepTime) 
\n\li   void PM_BuzzerManager ( void )
\n\li   ISR (Isr_SCI2_TR_Empty)
\n\li   static void Buzzart( bool_t state )
\n
@}
\ingroup   	ProductM 
*/
/**
\n
\par		PM_Buzzer.c
\n			Implementation of the necessary functions to manage the Buzzer trough a PWM pin 
\n
\file		PM_Buzzer.c 
\ingroup	PM_Buzzer
\date		31/05/2011
\version	01.00.00 
\author		Giuseppe Boccolato 
\n
\n
\par\b	History:
\n 
\n 		01.00.00 
\n		31/05/2011 
\n		Giuseppe Boccolato 
\n		changes 
\n 			Creation
\n
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "PM_Buzzer.h"
#include "Config.h"
#if defined (BUZZER_USE_TIMER)
#include "OCTPM_drv.h"
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */

#if defined ( BUZZER_USE_UART )

/** setup Baud Rate */
#define B_RATE_BUZZ	(uint16_t)8000
#define B_RATE_REG     ((BUS_FREQUENCY/16)/(B_RATE_BUZZ)) 
#define DATA_TO_TX	0x55

#endif /* BUZZER_USE_UART */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

#if defined ( BUZZER_USE_UART ) && defined(BUILD_FREESCALE_GT32_44P)

static void Buzzart( bool_t state );

#endif

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

static uint8_t BeepTimeValue; 
static uint8_t BeepCounter;

#if defined (BUZZER_USE_TIMER)
static uint8_t BuzzerFreqIndex;
#endif

#if defined ( BUZZER_USE_UART )
static uint8_t UartUsed;
#endif

extern  IO_PWM_CfgType Buzzer_cfg;
extern unsigned long tabBuzzerFrequency[];

/* ***************** */
/* Exported function */
/* ***************** */

/**
Inizializzazione del modulo Buzzer, chiamata in  OutputInitialization()
indipendentemente dal tipo di reset

\param  [in] pin, pin di pilotaggio del componente

\attention \b BeepCounter [in,out], tempo buzzer on x 10 msec
\attention \b BeepTimeValue [in] , tempo buzzer on x 10 msec da file di setting
\attention \b UartUsed, [out] se utilizzata la Uart per generare la frequenza
\attention \b ModuleIOmap[][] [in]

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0

\todo   Modificare la gestione del Buzzer quando usa il timer perchè va ad utilizzare
        TPM2SC e TPM2MOD \n
        Stessa Cosa per PWM \n     

\note		\par \b Nota 1: Target dependent

*/
/*lint -esym(715,pin)*/

void PM_BuzzerInit ( uint8_t pin )
{
	BeepCounter=0;
	BeepTimeValue=0;
 
#if defined (BUZZER_USE_TIMER)
	BuzzerFreqIndex = 0;

#elif defined ( BUZZER_USE_UART )
	/* in base al pin e al tipo di micro individua la UART da utilizzare */
	#if defined(BUILD_FREESCALE_GT32_44P)
	if ( ( ModuleIOmap[pin][0] == (uint8_t)&PTCD ) && 
	     ( ModuleIOmap[pin][1] == BIT0_MASK ) )
	{
    	UartUsed = 2;
	}
	else if ( ( ModuleIOmap[pin][0] ==(uint8_t)&PTED ) && 
			  ( ModuleIOmap[pin][1] == BIT0_MASK ) )
	{
		UartUsed = 1;	
	}
	else
	{
		UartUsed = 0; /* No Uart Module Selected */
	}
	#endif /*BUILD_FREESCALE_GT32_44P */  
#endif
     
}
/*lint +esym(715,pin)*/

/**
Setup del tempo di buzzer on.

\param [in] beepTime time buzzer on  (x 10 msec).
\param [in] freqIndex frequency index.	

\attention \b BeepTimeValue [in,out], tempo buzzer on x 10 msec

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0
*/
void PM_SetBuzzer (uint8_t beepTime, uint8_t freqIndex)
{
	BeepTimeValue = beepTime;
#if defined (BUZZER_USE_TIMER)
	BuzzerFreqIndex = freqIndex;
#endif
}

/**
Manager del modulo Buzzer, chiamata nel Task_10_ms se SystemMode = SYSTEM_NORMAL_MODE.


\attention \b BeepCounter [in], tempo buzzer on x 10 msec
\attention \b BeepTimeValue [in] , tempo buzzer on x 10 msec da file di setting

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0

   
\note		\par \b Nota 1: Target dependent

*/
       
void PM_BuzzerManager ( void )
{ 
    
	if ( BeepTimeValue != 0 )
	{
		if ( BeepCounter == 0 )
		{
			#if defined ( BUZZER_USE_TIMER )
			    (void)OCPWM_InitSync(&Buzzer_cfg);
			    OCPWM_SetAllSync(Buzzer_cfg.PWM_Id, (Buzzer_cfg.dfltPeriod/2), (tabBuzzerFrequency[BuzzerFreqIndex]));
			#elif defined ( BUZZER_USE_UART )
				#if defined(BUILD_FREESCALE_GT32_44P)
				Buzzart(TRUE);
				#endif
			#else
			
			#endif
			
			BeepCounter++;       
		}  
		else 
		{
			BeepCounter++;        
			if ( BeepCounter >= BeepTimeValue )
			{
				#if defined (BUZZER_USE_TIMER)
				    (void)OCPWM_DeInitSync(Buzzer_cfg.PWM_Id);
				#elif defined ( BUZZER_USE_UART )
					#if defined(BUILD_FREESCALE_GT32_44P)
					Buzzart(FALSE);			    
				    #endif
				#else
				#endif
				BeepCounter=0;
				BeepTimeValue=0;
			}
		}
	}
}  


/**
Gestione Interrupt Transmit Buffer Empty su SCI2.

\attention \b UartUsed [in], periferica utilizzata

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0

   
\note		\par \b Nota 1: Target dependent

*/
#if defined(SCI2_INTERRUPT_TX)

ISR (Isr_SCI2_TR_Empty)
{
	(void)SCI2S1;	/* clear interrupt pending flag */
	if ( UartUsed == 2 )
	{
		SCI2D = DATA_TO_TX;		 /* load new data */
	}
	else
	{
		/* This is a problem -> Disable Interrupt Enable */
    	SCI2C2_TIE = 0; 					
	}
}

#endif

/**
Gestione Interrupt Transmit Buffer Empty su SCI1.

\attention \b UartUsed [in], periferica utilizzata

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0

   
\note		\par \b Nota 1: Target dependent

*/
#if defined(SCI1_INTERRUPT_TX)

ISR (Isr_SCI1_TR_Empty)
{
	(void)SCI1S1;
	if ( UartUsed == 1 )
	{
		SCI1D = DATA_TO_TX;		 /* load new data */
	}
	else
	{
		/* This is a problem -> Disable Interrupt Enable */
    	SCI1C2_TIE = 0; 					
	}
}

#endif

/* ************** */
/* Local Function */
/* ************** */
/**
Inizializzazione del modulo Uart per il pilotaggio del buzzer.

\param  [in] state, se TRUE -> attiva periferica se FALSE -> disattiva periferica


\attention \b UartUsed [in], periferica utilizzata

\author 	Stefano Ughi
\date		12/10/2007
\version	1.0.0

   
\note		\par \b Nota 1: Target dependent

*/
#if defined ( BUZZER_USE_UART ) && defined(BUILD_FREESCALE_GT32_44P)

static void Buzzart( bool_t state )
{
	if ( UartUsed == 1 )
	{    	
    	if ( state == TRUE )
    	{
    		/*lint -e572 -e778*/
    		SCI1BDH = (B_RATE_REG>>8);  /* Set high divisor register */   
    		/*lint +e572 +e778*/
			
			SCI1BDL = B_RATE_REG;    	/* Set low divisor register  */		
    		SCI1C2_TIE = 1; 			/* Transmit Interrupt Enable */					
			SCI1C2_TE = 1;				/* Transmitter Enable 		 */
		}
		else
		{
			SCI1C2_TE = 0;				/* Transmitter Disable 		 */
		} 	 			
	}
	else if ( UartUsed == 2 )
	{
	    if ( state == TRUE )
	    {
	        /*lint -e572 -e778*/
		    SCI2BDH = (B_RATE_REG>>8);  /* Set high divisor register */   
    		/*lint +e572 +e778*/
			SCI2BDL = B_RATE_REG;    	/* Set low divisor register  */    
	        SCI2C2_TIE = 1; 			/* Transmit Interrupt Enable */
	    	SCI2C2_TE = 1;				/* Transmitter Enable 		 */
	    }
	    else
	    {
	    	SCI2C2_TE = 0;	    		/* Transmitter Disable 		 */
	    } 	 			
	}
	else
	{
		/* do nothing */
	}

}

#endif

/* *********** */
/* End of file */
/* *********** */







