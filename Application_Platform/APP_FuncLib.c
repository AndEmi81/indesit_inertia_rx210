/**
\defgroup	APP_FuncLib Modulo Application Platform Library Functions
\ingroup	ApplPlatform

Il modulo e' una libreria di macro e funzioni generiche condivise da
tutte le applicazioni.
*/

/**
Source file del modulo Application Platform Library Functions

\file		APP_FuncLib.c
\ingroup	APP_FuncLib
\date		08/07/2009
\version	01.00.00
\author		Marco Quintini

\par		History

\par		PCLint
			Report 08/07/2009: 0 Errors, 0 Warnings, 0 Info

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */
#if (SW_TIMER_NORM_MODE	== FALSE) 
/**
Configura il contatore SMALL specificato in ingresso senza abilitare la partenza

\param [in]		idCounter e' l'indice al contatore che si vuole impostare
\param [in]		prescalerValue e' la base tempi del contatore
\param [in]		value e' il valore di inizializzazione del contatore ti tipo SMALL

\return		 none

\retval			none

\warning		none		

\author			Marco Quintini
\date			01/08/2011
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Set_SmallCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_SCounterValue value)
{	
	/* Configura il PRESCALER */
	SWTC_SetPrescaler (idCounter, prescalerValue);
	
	
	/* Imposta il valore */
	SWTC_SetSValue (idCounter, value);

	/* Stop counter */
	SWTC_Stop(idCounter);
				
}


/**
Configura il contatore MEDIUM specificato in ingresso senza abilitare la partenza

\param [in]		idCounter e' l'indice al contatore che si vuole impostare
\param [in]		prescalerValue e' la base tempi del contatore
\param [in]		value e' il valore di inizializzazione del contatore ti tipo MEDIUM

\return		none

\retval			none

\warning		none		

\author			Marco Quintini
\date			19/07/2011
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Set_MediumCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_MCounterValue value)
{	
	/* Configura il PRESCALER */
	SWTC_SetPrescaler (idCounter, prescalerValue);
	
	/* Imposta il valore */
	SWTC_SetMValue (idCounter, value);

	/* Stop counter */
	SWTC_Stop(idCounter);
				
}


#if defined(SWTC_L_ENABLED)
/**
Configura il contatore LARGE specificato in ingresso senza abilitare la partenza

\param [in]		idCounter e' l'indice al contatore che si vuole impostare
\param [in]		prescalerValue e' la base tempi del contatore
\param [in]		value e' il valore di inizializzazione del contatore ti tipo LARGE

\return			ritorna S_OK se tutto è andato a buon fine, altrimenti ritorna S_FALSE

\retval			none

\warning		none		

\author			Marco Quintini
\date			19/07/2011
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Set_LargeCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_LCounterValue value)
{	
	/* Configura il PRESCALER */
	SWTC_SetPrescaler (idCounter, prescalerValue);
	
	/* Imposta il valore */
	SWTC_SetLValue (idCounter, value);
	
	/* Stop counter */
	SWTC_Stop(idCounter);
	
}
#endif	/*#if defined(SWTC_L_ENABLED)*/


/**
Arresta il contatore SMALL specificato in ingresso e lo azzera.

\param [in]		idCounter e' l'indice al contatore che si vuole impostare

\return			bool_t

\retval		  none

\warning		none		

\author			Marco Quintini
\date			03/08/2011
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Reset_SmallCounter(SWTC_CountersType idCounter)
{	
	SWTC_Stop(idCounter);

	
	SWTC_SetSValue(idCounter, (SWTC_SCounterValue)0);

}


/**
Arresta il contatore MEDIUM specificato in ingresso e lo azzera.

\param [in]		idCounter e' l'indice al contatore che si vuole impostare

\return			bool_t

\retval			none

\warning		none		

\author			Marco Quintini
\date			19/07/2011
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Reset_MediumCounter(SWTC_CountersType idCounter)
{	
	SWTC_Stop(idCounter);

	SWTC_SetMValue(idCounter, (SWTC_MCounterValue)0);
}


#if defined(SWTC_L_ENABLED)
/**
Arresta il contatore LARGE specificato in ingresso e lo azzera.

\param [in]		idCounter è l'indice al contatore LARGE che si vuole impostare

\return		 none

\retval			none

\warning		none		

\author			Francesco Corvo
\date			16/09/2010
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/

void Reset_LargeCounter(SWTC_CountersType idCounter)
{
	SWTC_Stop(idCounter);
	
	SWTC_SetLValue(idCounter, (SWTC_LCounterValue)0);

}
#endif	/*#if defined(SWTC_L_ENABLED)*/

#else /* SW_TIMER_NORM_MODE	== FALSE */
/**
Configura il contatore specificato in ingresso per un uso come Up Counter,
lo inizializza e lo disabilita.

\param [in]		counterSize e' la size SHORT/MEDIUM/LARGE_COUNTER del contatore da inizializzare
\param [in]		counterId e' l'identificatore del contatore da inizializzare
\param [in]		opCode e' la base tempi del contatore

\return			void

\retval			none

\warning		none		

\author			Michele Tanzi
\date			18/07/2012
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Set_UpCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode)
{	
	SWTC_Stop(counterId);
	SWTC_SetPrescaler(counterId, opCode);
	/* The counter is actually a down-counter and it must start from its maximum value
	 * in order to take the correct elapsed time with the SWTC_GetElapsedTime().
	 * Value 0 is used to force the timer expiring because retrigger works
	 * only on an expired timer.
	 */
	switch (counterSize)
	{
		case SHORT_COUNTER:
			SWTC_SetSValue(counterId, (SWTC_SCounterValue) 0);
			break;
		case MEDIUM_COUNTER:
			SWTC_SetMValue(counterId, (SWTC_MCounterValue) 0);
			break;
#if defined(SWTC_L_ENABLED)
		case LARGE_COUNTER:
			SWTC_SetLValue(counterId, (SWTC_LCounterValue) 0);
			break;
#endif

		default:
			break;
	}

	(void)SWTC_Retrigger(counterId);
}


/**
Configura il contatore specificato in ingresso come Down Counter,
lo inizializza e lo disabilita.

\param [in]		counterId e' l'identificatore del contatore da inizializzare
\param [in]		opCode e' la base tempi del contatore
\param [in]		value e' il valore di inizializzazione del contatore [1..127]

\return			void

\retval			none

\warning		none		

\author			Michele Tanzi
\date			18/07/2012
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void Set_DownCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode, SWTC_TimeType value)
{	
	SWTC_Stop(counterId);
	SWTC_SetPrescaler(counterId, opCode);
	switch (counterSize)
	{
		case SHORT_COUNTER:
			SWTC_SetSValue(counterId, (SWTC_SCounterValue)value);
			break;
		case MEDIUM_COUNTER:
			SWTC_SetMValue(counterId, (SWTC_MCounterValue)value);
			break;
#if defined(SWTC_L_ENABLED)
		case LARGE_COUNTER:
			SWTC_SetLValue(counterId, (SWTC_LCounterValue) value);
			break;
#endif

		default:
			break;
	}
}


/**
Disabilita il contatore specificato in ingresso e azzera il suo valore.

\param [in]		pCounter e' il puntatore al contatore da resettare

\return			void

\retval			none

\warning		attenzione, per contatori usati come se fossero Up Counter non è equivalente al ResetCounter
				di Platform 1.0

\author			Simone Costantini
\date			26/01/2007
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void StopCounterToZero(uint8_t counterSize, SWTC_CountersType counterId)
{
	SWTC_Stop(counterId);
	switch (counterSize)
	{
		case SHORT_COUNTER:
			SWTC_SetSValue(counterId, (SWTC_SCounterValue) 0);
			break;
		case MEDIUM_COUNTER:
			SWTC_SetMValue(counterId, (SWTC_MCounterValue) 0);
			break;
#if defined(SWTC_L_ENABLED)
		case LARGE_COUNTER:
			SWTC_SetLValue(counterId, (SWTC_LCounterValue) 0);
			break;
#endif

		default:
			break;
	}
}
#endif /* SW_TIMER_NORM_MODE	== FALSE */


/**
Incrementa la variabile *pAccumulator se questa risulta inferiore a 0xFF, altrimenti
la lascia invariata.

\param [in,out]	pAccumulator e' il puntatore alla variabile da incrementare.

\return			void

\retval			none

\warning		none		

\author			Marco Quintini
\date   		30/11/2006
\version    	1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void IncByteWithCheck(uint8_t *pAccumulator)
{
	if((*pAccumulator) < UINT8_T_MAX)
	{
		(*pAccumulator) ++;
	}
}


#if (CHECKERRORS_ENABLE == 1)
/**
Memorizza in un buffer circolare tutti i codici di errore generati dai moduli
dell'applicazione. \n
Il funzionamento corretto prevede che non ci sia nessun elemento all'interno del buffer.
 
\param [in]		codeToVerify e' il codice (ad esempio restituito da una routine)
				su cui viene effettuata la verifica. Se e' un codice di errore
				e se non e' gia' presente in Debug_Error[], viene aggiunto in coda.
\attention		Debug_Error (global, buffer che memorizza i codici di errore) e'
				usata come input/output

\return			void

\retval			none

\warning		none		

\author  		Simone Costantini, Valeria Catani, Lucio Bianchi, Marco Quintini	
\date 			14/02/2006
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void DBG_CheckErr(uint8_t codeToVerify)
{
static uint8_t iDbgErr = 0;
uint8_t i;
	
	if(codeToVerify & ERROR_WARNING_CODE_MASK)
	{
		for(i = 0; i < iDbgErr; i++)
		{
			if(codeToVerify == Debug_Error[i])
			{
				return;
			}
		}
	
		/*lint -save -e912 */
		if(iDbgErr >= SIZE(Debug_Error))
		/*lint -restore */
		{
			iDbgErr = 0;
		}
		
		Debug_Error[iDbgErr++] = codeToVerify;
	}
}
#endif /* (CHECKERRORS_ENABLE == 1) */


/* *************** */
/* Local Functions */
/* *************** */


/* *********** */
/* End of file */
/* *********** */
