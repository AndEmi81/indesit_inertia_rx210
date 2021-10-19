/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		        Header file di FreqMeter.c
\n			        Contiene le definizioni per la gestione del Damper ON OFF
\n
\file		        FreqMeter.h
\ingroup	      PM_FreqMeter
\date		        24/06/2009
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
\n                - Aggiornata la documentazione
\n
\n 
\n 		          1.0.1
\n          		14/01/2008
\n          		Alberto Camerin,Roberto Fioravanti,Stefano Ughi
\n          		Terza versione
\n
\n
\n 		          1.4.0
\n          		10/06/2004
\n          		Progettista_1
\n          		Seconda versione
\n
\n
\n 		          1.3.0
\n          		10/04/2004
\n          		Progettista_1
\n          		Prima versione
\n
*/


#ifndef __FREQMETER_H
#define __FREQMETER_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Se '0' abilita le funzioni del modulo */
#define FREQMETER_DISABLE_MODULE 0

/** Numero dei canali complessivi utilizzabili dal modulo */
#define FREQMETER_TPM1_NUMOFCHANNEL MCU_TPM1_NUMOFCHANNEL
#define FREQMETER_TPM2_NUMOFCHANNEL MCU_TPM2_NUMOFCHANNEL
#define FREQMETER_NUMOFCHANNEL      MCU_TOTAL_NUMOFCHANNEL

/** Vettore di abilitazione dei canali all'interno del modulo */
#define FreqMeterChTPMEnable     MCUTPMChEnable
/** Vettore di configurazione dei canali all'interno del modulo */
#define FreqMeterChTPMSCConfig   MCUTPMChConfig

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ****** */
/* Errors */
/* ****** */

/* TPM1 */
#if (FREQMETER_TPM1_CH0_ENABLE==1)
	/* Channel 0 */
	#ifdef TPM1_CH0_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM1_CH0_USED
	#define FREQMETER_ON_TPM1_CH0
	#endif
#endif /* (FREQMETER_TPM1_CH0_ENABLE==1) */

#if (FREQMETER_TPM1_CH1_ENABLE==1)
	/* Channel 1 */
	#ifdef TPM1_CH1_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM1_CH1_USED
	#define FREQMETER_ON_TPM1_CH1
	#endif
#endif /* (FREQMETER_TPM1_CH1_ENABLE==1) */
	
#if (FREQMETER_TPM1_CH2_ENABLE==1)
	/* Channel 2 */
	#ifdef TPM1_CH2_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM1_CH2_USED
	#define FREQMETER_ON_TPM1_CH2
	#endif
#endif /*(FREQMETER_TPM1_CH2_ENABLE==1)*/

//#if defined(BUILD_RENESAS_36079)
#if (FREQMETER_TPM1_CH3_ENABLE==1)
	/* Channel 3 */
	#ifdef TPM1_CH3_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM1_CH3_USED
	#define FREQMETER_ON_TPM1_CH3
	#endif
#endif /*(FREQMETER_TPM1_CH3_ENABLE==1)*/
//#endif



/* TPM2 */
#if (FREQMETER_TPM2_CH0_ENABLE==1)
	/* Channel 0 */
	#ifdef TPM2_CH0_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM2_CH0_USED
	#define FREQMETER_ON_TPM2_CH0
	#endif
#endif /*(FREQMETER_TPM2_CH0_ENABLE==1)*/

#if (FREQMETER_TPM2_CH1_ENABLE==1)
	/* Channel 1 */
	#ifdef TPM2_CH1_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM2_CH1_USED
	#define FREQMETER_ON_TPM2_CH1
	#endif
#endif /*(FREQMETER_TPM2_CH1_ENABLE==1)*/
	
#if (FREQMETER_TPM2_CH2_ENABLE==1)
	/* Channel 2 */
	#ifdef TPM2_CH2_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM2_CH2_USED
	#define FREQMETER_ON_TPM2_CH2
	#endif
#endif /*(FREQMETER_TPM2_CH2_ENABLE==2) */
	
#if (FREQMETER_TPM2_CH3_ENABLE==1)
	/* Channel 3 */
	#ifdef TPM2_CH3_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM2_CH3_USED
	#define FREQMETER_ON_TPM2_CH3
	#endif
#endif /* (FREQMETER_TPM2_CH3_ENABLE==1) */
	
//#if defined(BUILD_FREESCALE_GB60)
#if (FREQMETER_TPM2_CH4_ENABLE==1)
	/* Channel 4 */
	#ifdef TPM2_CH4_USED
	#define FREQMETER_CH_ERROR
	#else 
	#define TPM2_CH4_USED
	#define FREQMETER_ON_TPM2_CH4
	#endif
#endif /* (FREQMETER_TPM2_CH4_ENABLE==1) */
//#endif /* BUILD_FREESCALE_GB60 */

/* ******************* */
/* Function prototypes */
/* ******************* */
void FreqMeter_Init( void);
void FreqMeter_SetChannelStatus(uint8_t,uint8_t);
void FreqMeter_SetIsrInputCapture(uint8_t ,uint8_t );
void FreqMeter_ManageIsrOverflow(uint8_t, uint8_t);
uint8_t FreqMeter_GetOverflow(uint8_t,uint8_t);
void FreqMeter_SetManagerFunctions( uint8_t, 
								    uint8_t(*)(uint8_t,uint8_t),
								    uint8_t(*)(uint8_t, uint16_t, uint8_t) );

void FreqMeter_SetOverflowFlag(uint8_t);
void FreqMeter_ResetOverflowFlag(uint8_t  );
void FreqMeter_ResetOverflow(uint8_t);
uint8_t FreqMeter_GetOverflowChannel(uint8_t);
uint8_t FreqMeter_GetTimerChannelFromPin(uint8_t );
void FreqMeter_ConfigTimerCntrlRegister( uint8_t);
uint8_t FreqMeter_GetChannelStatus(uint8_t timer_num);
void FreqMeter_SetEdge(uint8_t timer_num, uint8_t edge);
uint8_t FreqMeter_GetEdge( uint8_t timer_num );

/* PI begin: aggiunte a FreqMeter.h perchè usate in interrupt.c */
/*static*/ void FreqMeter_ManageISR(uint8_t timer_num,uint16_t value, uint8_t ovfFlag);
/*static*/ void Isr_TPM_common(uint8_t i);
/* PI end: aggiunte a FreqMeter.h perchè usate in interrupt.c */

#if defined(BUILD_RENESAS_36079)
void FreqMeterTimerManager(void);
void TimerHook( void);
#endif



#endif /* __FREQMETER_H */

/* *********** */
/* End of file */
/* *********** */
