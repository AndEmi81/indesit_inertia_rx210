/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		        Header file di ResToTimeConv
\n			        Contiene le funzioni per la gestione degli overflow e degli interrupt sul timer
\n
\file		        ResToTimeConv.h
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
\n              - Questo file rinominato e aggiornato sostituisce il vecchio TimeCounter.h
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


#ifndef __RES_TO_TIME_CONV_H
#define __RES_TO_TIME_CONV_H

/* ******** */
/* Includes */
/* ******** */

#include "OCTPM_drv.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
///*** Timer Prescaler
/*
#ifdef PULSECNT_SW_DIVISOR
	#define TIMECNT_SW_DIVISOR 				PULSECNT_SW_DIVISOR
	#else 
	#define PULSECNT_SW_DIVISOR				7
	
#endif
*/


///*** Sw divider


///*** Input Capture channels
//	TPM2 has 5 channels (0-4)
//  - GB60: TPM2CH4 = TIMERCHANNEL7
//          Fissato TPM2CH4 il pin associato è il 7 della porta D PTDD_PTDD7
//          --> va fissato il pin del modulo linkage corrispondente
#define TIME_COUNTER_TPM_CH 				TIMERCHANNEL7
#define TIME_COUNTER_IC_PIN					TIMERCHANNEL7_PIN

/* PI begin: define non più necessarie con l'uso del driver */
/*
///*** Errors
#if (TIME_COUNTER_TPM_CH==0)
	// Channel 0
	#ifdef TPM2_CH0_USED
	#define TIME_COUNTER_CH_ERROR
	#else 
	#define TPM2_CH0_USED
	#define TIME_COUNTER_ON_TPM2_CH0
	#endif

#elif (TIME_COUNTER_TPM_CH==1)
	// Channel 1
	#ifdef TPM2_CH1_USED
	#define TIME_COUNTER_CH_ERROR
	#else 
	#define TPM2_CH1_USED
	#define TIME_COUNTER_ON_TPM2_CH1
	#endif
	
#elif (TIME_COUNTER_TPM_CH==2)
	//Channel 2
	#ifdef TPM2_CH2_USED
	#define TIME_COUNTER_CH_ERROR
	#else 
	#define TPM2_CH2_USED
	#define TIME_COUNTER_ON_TPM2_CH2
	#endif
	
#elif (TIME_COUNTER_TPM_CH==3)
	// Channel 3
	#ifdef TPM2_CH3_USED
	#define TIME_COUNTER_CH_ERROR
	#else 
	#define TPM2_CH3_USED
	#define TIME_COUNTER_ON_TPM2_CH3
	#endif
	
#elif (TIME_COUNTER_TPM_CH==4)
	// Channel 4
	#ifdef TPM2_CH4_USED
	#define TIME_COUNTER_CH_ERROR
	#else 
	#define TPM2_CH4_USED
	#define TIME_COUNTER_ON_TPM2_CH4
	#endif
#endif
*/
/* PI end: define non più necessarie con l'uso del driver */


//#define TIMEINPUT_NUM 4N_TIME_INPUTS
///*** Filters
#define MAX_SUM_REPETITION      10   // max number of repetition for sum 
#define TCM_MAX_NUM_LETTURE		20   // number of input measures
#define TCM_DELTA_NUM_LETTURE   2    // number of input measures different from expected
#define NEGATIVE				0
#define POSITIVE				1
#define MAX_NUM_OVERFLOW		5	 // Limit ofthe overflows number	

#define OVERFLOW_CONTROL


#define NO_TIME_INPUT_ERROR      0
#define TOOMANY_TIME_INPUT_ERROR 1
#define TOOMANY_TIME_NSAMPLES    1 
#define EXIT_LOOP_FOR_ERROR      10


// Parametri da Editor, cambiare le seguenti #define

//#define N_SAMPLES				4     //samples to read before average (2^N_SAMPLES)  
//#define REF_CHANNEL             3	  //# reference channel 	

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/**
**	*******************************************************************
** 	Method:					TimeCounter_Enable
**				
**	Description:			Enables or disables the module
**							
**	Inputs:					BYTE Enable
**	Return:					-
**	
** 
**	*******************************************************************
*/
void TimeCounter_Enable(BYTE IntEnable);

/**
**	*******************************************************************
** 	Method:					TimeCounter_Init
**				
**	Description:			Module initialization
**							
**	Inputs:					BYTE * P
**	Return:					-
**	
** 
**	*******************************************************************
*/
BYTE InsertTimeInput(BYTE,BYTE, BYTE, BYTE,BYTE,BYTE);


/**
**	*******************************************************************
** 	Method:					TimeCounter_InputCaptureConfig
**				
**	Description:			Input Capture configuration on TPM2
**							Channel is specified by the define
**							TIME_COUNTER_TPM_CH
**							
**	Inputs:					-
**	Return:					-
**	
** 
**	*******************************************************************
*/
void TimeCounter_InputCaptureConfig(void);

/**
**	*******************************************************************
** 	Method:					SearchNextChannel
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

uint8_t SelectNextChannel(void);



/**
**	*******************************************************************
** 	Method:					ResetVar
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

void ResetVar(void);

/**
**	*******************************************************************
** 	Method:					GetTimeProbeReadAlarms
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

uint8_t GetTimeProbeReadAlarms(void);

/**
**	*******************************************************************
** 	Method:					GetTimeProbeChannelAlarms
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

uint8_t GetTimeProbeChannelAlarms(void);

/**
**	*******************************************************************
** 	Method:					GetEnabledProbe
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

uint8_t GetEnabledProbe(void); 
	
/**
**	*******************************************************************
** 	Method:					CheckTimeProbe
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

void CheckTimeProbe(unsigned char array_index);


/**
**	*******************************************************************
** 	Method:					AreTimeInputsReady
**				
**	Description:			Ritorna TRUE se tutti gli ingressi time attivi letti
**							in automatico sono stati acquisiti, FALSE altrimenti
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/
uint8_t  AreTimeInputsReady(void);

/**	*******************************************************************
** 	Method:					InitTimeInput
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

void InitTimeInput (void);	

//28 JUNE
/**	*******************************************************************
** 	Method:					TimeCounter_Manager
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/

void TimeCounter_Manager (void);	


//END 28 JUNE

/**
**	*******************************************************************
** 	Method:					GetInitialMultiConfig
**				
**	Description:			
**							
**	Inputs:					
**	Return:					-
**	
** 
**	*******************************************************************
*/
BYTE GetInitialMultiConfig(void);




#endif /* __RES_TO_TIME_CONV_H */

/* *********** */
/* End of file */
/* *********** */
