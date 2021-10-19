/*
** **************************************************************************
** **************************************************************************
*/ 
/** 
Gestisce il microprogramma ActivateAlgorithm, che permette di attivare
alcune funzionalità della macchina durante l'esecuzione del ciclo.

Source file
 
\file    ActivateAlgorithm.c
\ingroup ActivateAlgorithm
 
\version 	1.0.0
\date    	18/12/2006
\author		Simone Costantini

*/ 
/*
** **************************************************************************
** **************************************************************************
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version
\date
\author
*/


/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"

#if (AAL_ACTIVATEALGORHYTHM_ENABLE == 1)

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define AAL_GET_ALG_CODE(p)		OFFS_B((p), 0)
#define AAL_GET_ALG_DATA(p)		OFFS_B((p), 1)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* Generale */


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* Algorithm Tables */
extern const ALGORITHM_PTR AAL_algList[N_MAX_ALG];

/**/

/* ***************** */
/* Exported function */
/* ***************** */


/**
Gestisce l'esecuzione del microprogramma ActivateAlgorithm.

\param [in] highStatement specifica l'high statement (opcode, flag F0, F1, N, T) del microprogramma
\param [in] lowStatement specifica il low statement (displacement ai dati) del microprogramma

\return S_OK se la condizione di avanzamento passo per il microprogramma è verificata (istantaneo),
\return S_FALSE se non è ancora verificata,
\return codice di errore in altri casi


\version		1.0.0
\date 			19/01/2006
\author			Simone Costantini
*/

#if (MULTI_COMPARTMENT_APPL == 1)	
uint8_t AAL_Activate_Algorithm_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment)
{
#else
uint8_t AAL_Activate_Algorithm_Exe(uint8_t highStatement, uint8_t lowStatement)
{
uint8_t compartment=0;
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

Loader_Struct uPAAlgSettingData;
uint8_t algCode;
Interpreter_Struct *pItpData = &Interpreter_Data[compartment];

	if (MSettingBasicLoader( Interpreter_Data[0].mSetting, APP_PTR_MP_ACTIVATEALGO_DATA, lowStatement, &uPAAlgSettingData) != S_OK)
	{
#ifdef _ACTIVATEALGORYTHM_CHECKERRORS_				
		DBG_CheckErr(AAL_ERR_LD_DATA_TAB);
#endif
	return S_OK;
	}
	/* Caricati correttamente dati uProg */
	algCode = AAL_GET_ALG_CODE(uPAAlgSettingData.Data);
	if (algCode < N_MAX_ALG)
	{		/* check di sicurezza */
		if ((highStatement & F0) == 0)
		{		/* F0 = 0: Attiva l'algoritmo */
			/*
			Nel caso di tabella di parametri selezionabile da uProg
			viene memorizzato il displacement alla tabella in una
			locazione riservata per lo specifico algoritmo.
			*/
			AADisplParams[algCode][compartment] = AAL_GET_ALG_DATA(uPAAlgSettingData.Data);
			/*lint -save -e912 */
			if ((pItpData->stepInitialized == FALSE) || (pItpData->optionalStepInitialized == FALSE))
			/*lint -restore */
			{
					/*
					Prima iterazione del passo o ripristino da backup:
					viene forzato l'init dell'algoritmo	attivato.
					Poichè le bitmap vengono salvate in backup, se utilizziamo il uProg
					nella modalità one-shot (quindi eseguito solo una volta e in un passo diverso da
					quello in cui si controlla la terminazione dell'algoritmo) al ripristino non 
					verrà mandato il comando di init (cmd = AAL_ALG_INIT) all'algoritmo quindi,
					se anche i dati relativi vengono salvati, l'esecuzione prosegue senza interruzioni.
					Se invece lo utilizziamo nella modalità continua col flag N=1 viene passato
					cmd = AAL_ALG_INIT e quindi viene perso il backup.
					Sfruttando questa caratteristica si può decidere caso per caso, a seconda di come
					si costruisce la macro, il comportamento dell'algoritmo dopo una mancanza rete.
					*/
					CLRBIT(ActiveAlgorithmBmpOld[compartment], algCode);
					SETBIT(ActiveAlgorithmBmp[compartment], algCode);
					CLRBIT(TerminatedAlgorithmBmp[compartment], algCode); /* per attivazioni multiple algoritmo */			
			}
			/* La condizione di uscita viene data solo quando l'algoritmo è terminato */
			if (TESTBIT(TerminatedAlgorithmBmp[compartment], algCode) == 0uL)
			{
				return S_FALSE;
			}
		}
		else
		{			    
			/* F0 = 1: Disattiva l'algoritmo */
			CLRBIT(ActiveAlgorithmBmp[compartment], algCode);				
		}		
	}	
	return S_OK;
}
/**/


/**
Gestisce l'attivazione e la disattivazione degli algoritmi selezionati

\par \b Inizializzazione
AAL_ALG_INIT
\par \b Esecuzione
AAL_ALG_MONITOR
\par \b Disattivazione
AAL_ALG_DISABLE

\version		1.0.0
\date 			19/01/2006
\author			Simone Costantini,Roberto Fioravanti
*/

#if (MULTI_COMPARTMENT_APPL == 1)	
void AAL_algorithmManagement(uint8_t compartment)
{
#else
void AAL_algorithmManagement(void)
{
uint8_t compartment=0;
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

uint8_t i;
uint8_t cmd;

	for (i = 0; i < N_MAX_ALG; i++)
	{
		if (TESTBIT(ActiveAlgorithmBmp[compartment], i))
		{
			/* L'algoritmo i-esimo è attivo */
			cmd = (uint8_t)AAL_ALG_MONITOR;
			if (TESTBIT((((ActiveAlgorithmBmpOld[compartment]) ^ (ActiveAlgorithmBmp[compartment])) & (ActiveAlgorithmBmp[compartment])), i))
			{
				/* 
				Lo stato dell'algoritmo è passato da OFF a ON:
				viene passato all'algoritmo il comando di inizializzazione
				*/
				cmd = (uint8_t)AAL_ALG_INIT;
			}
			/* Esecuzione Algoritmo */
#if (MULTI_COMPARTMENT_APPL == 1)	
 			AAL_algList[i](cmd,compartment);				
#else
			AAL_algList[i](cmd);				
#endif /* (MULTI_COMPARTMENT_APPL == 1) */			
		}
		else
		{
		    if (TESTBIT((((ActiveAlgorithmBmpOld[compartment]) ^ (ActiveAlgorithmBmp[compartment])) & (ActiveAlgorithmBmpOld[compartment])), i))
			{
				/* 
				Lo stato dell'algoritmo è passato da ON a OFF:
				viene passato all'algoritmo il comando di disabilitazione
				*/
				cmd = (uint8_t)AAL_ALG_DISABLE;								
			    /* chiama la funzione di disabilitazione */
#if (MULTI_COMPARTMENT_APPL == 1)	
 				AAL_algList[i](cmd,compartment);				
#else
				AAL_algList[i](cmd);				
#endif /* (MULTI_COMPARTMENT_APPL == 1) */					
				SETBIT(TerminatedAlgorithmBmp[compartment], i);	            
			}
		}
	}
	ActiveAlgorithmBmpOld[compartment] = ActiveAlgorithmBmp[compartment];
}
/**/

/**
resetta le variabili di controllo di ActivateAlgorithm

\version		1.0.0
\date 			27/06/2008
\author			Simone Costantini
*/

#if (MULTI_COMPARTMENT_APPL == 1)	
void AAL_resetAlgorithmCtlVars(uint8_t compartment)
{
#else
void AAL_resetAlgorithmCtlVars(void)
{
uint8_t compartment=0;
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
	ActiveAlgorithmBmp[compartment] = 0;
	ActiveAlgorithmBmpOld[compartment] = 0;
	TerminatedAlgorithmBmp[compartment] = 0;
}


/**/

/* ************** */
/* Local Function */
/* ************** */

/**
Funzione segnaposto che occupa la posizione di algoritmi non 
ancora implementati.
Se viene eseguita significa che si è verificato un errore.

\version		1.0.0
\date 			19/01/2006
\author			Simone Costantini
*/
/*lint -e715
Elimina il warning sul parametro cmd che non viene utilizzato
*/

#if (MULTI_COMPARTMENT_APPL == 1)	
void NullFunc(uint8_t cmd, uint8_t compartment)
{
#else
void NullFunc(uint8_t cmd)
{
uint8_t compartment=0;
#endif /* (MULTI_COMPARTMENT_APPL == 1) */
uint8_t i;
	for (i = 0; i < N_MAX_ALG; i++)
	{
		if (NullFunc == AAL_algList[i])
		{
			SETBIT(TerminatedAlgorithmBmp[compartment], i);
			CLRBIT(ActiveAlgorithmBmp[compartment], i);			
		}
	}
}

/**/
#endif /* (AAL_ACTIVATEALGORHYTHM_ENABLE == 1) */


/* *********** */
/* End of file */
/* *********** */
