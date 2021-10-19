/**
\defgroup	APP_Loader Modulo Loader
\ingroup	ApplPlatform

Il modulo fornisce funzionalita' di caricamento dati dal file di setting.
*/

/**
Source file del modulo Loader

\file		APP_Loader.c
\ingroup	APP_Loader
\date		23/07/2008
\version	01.00.00
\author		Valeria Catani, Simone Costantini, Stefano Ughi, Marco Quintini

\par		History

\par		PCLint
			Report 16/01/2009: 0 Errors, 0 Warnings, 0 Info

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
/**
\name Valori particolari assunti dai puntatori nel file di setting
*/
/*@{*/ 
#define NULL_POINTER				0x0000U	/**< Puntatore nullo */
#define INVALID_POINTER				0xFFFFU	/**< Puntatori non valido */
/*@}*/ 



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
/** Puntatori ai selettori per i diversi vani */
extern const uint16_t SelectorDataPointers[APP_N_COMP];

/* ***************** */
/* Exported function */
/* ***************** */


/**
Carica i dati relativi alla fase indicata in ingresso: numero di fasi nel ciclo
cui la fase appartiene, numero di statement che la compongono, puntatore agli High
Statement, puntatore ai Low Statement.

\param [in]		compartment e' il vano in cui si trova il ciclo a cui appartiene
				la fase che si vuole caricare
\param [in]		cycle e' il ciclo a cui appartiene la fase che si vuole caricare
\param [in]		phase e' la fase che si vuole caricare
\param [out]	pDataOut punta ai dati relativi alla fase caricata
\attention		SelectorDataPointers (global, buffer puntatori ai selettori per
				i diversi vani) e' usata come input

\return			S_OK se la funzione e' stata eseguita correttamente,
				un codice d'errore altrimenti.

\warning		none		

\version		1.0.0
\date 			04/01/2006
\author			Simone Costantini, Stefano Ughi
*/
#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t LDR_phaseLoader(uint8_t compartment, uint8_t cycle, uint8_t phase, uint8_t mSetting, PhaseLoader_Struct *pDataOut)
#else /* (MULTI_COMPARTMENT_APPL == 1) */


uint8_t LDR_phaseLoader(uint8_t cycle, uint8_t phase, uint8_t mSetting, PhaseLoader_Struct *pDataOut)

#endif /* (MULTI_COMPARTMENT_APPL != 1) */
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif /* (MULTI_COMPARTMENT_APPL == 0) */  	

Loader_Struct ldData;
uint8_t result;
uint8_t hiStatRefMacro, loStatRefMacro;

#if (MULTI_COMPARTMENT_APPL == 1)
	if( compartment >= SIZE(SelectorDataPointers) )
	{
		return LDR_ERR_OVFL_SELECTORS;
	}
#endif /* (MULTI_COMPARTMENT_APPL == 1) */

	/**	cycle indica il numero del ciclo partendo da 0 
	per questo motivo nel test va usato >= */
	if( cycle >= N_MAX_CYCLES_PC )
	{
		return LDR_ERR_OVFL_CYCLES;
	}
		
	if( phase >= N_MAX_PHASES )
	{
		return LDR_ERR_OVFL_PHASES;
	}
	result = MSettingBasicLoader( mSetting, SelectorDataPointers[compartment], cycle, &ldData );

	if( result != S_OK )
	{
		return result;
	}
	
	if( phase >= ldData.DataLenght )
	{
		return LDR_ERR_INVALID_PHASE;
	}
		
	pDataOut->nCyclePhases = ldData.DataLenght; 	
	
	ldData.Data += 2*phase;
	hiStatRefMacro = OFFS_B(ldData.Data, 0);
	loStatRefMacro = OFFS_B(ldData.Data, 1);
	
	if( ( hiStatRefMacro == 0 ) || ( loStatRefMacro == 0 ) )
	{
		return LDR_ERR_MACRO_NULL;
	}

	result = MSettingBasicLoader( mSetting, APP_PTR_HI_STATEMENTS, (uint8_t)(hiStatRefMacro - 1), &ldData);

	if( result != S_OK )
	{
		return result;
	}
		
	/*
	Se gli statements reali sono dispari l'editor deve provvedere
	ad aggiungere un ultimo statement a '0' (NOP).
	*/
	pDataOut->nPhaseStatements = (uint8_t)(2 * ldData.DataLenght);
	pDataOut->pHiStat = ldData.Data;

	result = MSettingBasicLoader( mSetting, APP_PTR_LO_STATEMENTS, (uint8_t)(loStatRefMacro - 1), &ldData);

	if( result != S_OK )
	{
		return result;
	}

	if (pDataOut->nPhaseStatements != (2 * ldData.DataLenght))
	{
		return LDR_ERR_STATEMENT_MISMATCH;
	}

	pDataOut->pLoStat = ldData.Data;
	
	return S_OK;
} /* END: LDR_phaseLoader */


/**
Questa funzione carica un dato (uint16_t) del setting evitando problemi di incompatibilità
setting file <--> firmware grazie alla funzione MissedOnlyCheck, in seguito all'introduzione 
di nuovi parametri setting;

in pratica viene controllato se il dato che si vuole caricare esiste veramente nella tabella,
se sì viene caricato, altrimenti viene caricato al suo posto il valore 0;
può succedere quando viene introdotta una nuova funzionalità che richiede un nuovo parametro (uint16_t)
setting, quando il dato aggiunto comporta l'aggiunta di una word nella tabella, perchè in questo caso
la tabella si allunga. Se un nuovo firmware che va a cercare questo dato viene fatto girare con un vecchio
setting dove la tabella è più corta (il dato non esiste) senza questo controllo si caricherebbe
un dato random, in questo caso invece si verifica che la lunghezza della tabella comprenda il dato selezionato,
se non è così viene forzato il caricamento del dato a 0

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		position e' lo spiazzamento relativo al puntatore pointer
				del dato da caricare (uint8_t)

\return			il dato caricato

\warning		none

\use			Questa funzione va usata tutte le volte che una tabella setting si allunga con nuovi dati,
				mentre non è necessario usarla quando viene aggiunto un nuovo byte ma la lunghezza della
				tabella (in word) rimane la stessa.

\version		1.0.0
\date 			15/10/2013
\author			Claudio Fontana
*/

uint16_t MissedLoadShortCheck(Loader_Struct* pointer, uint8_t position)
{

Loader_Struct* pointerToCheck = pointer;
uint8_t positionToCheck = position;

uint16_t value;

	if (MissedOnlyCheck(pointerToCheck,positionToCheck))
	{
		value = (OFFS_B(pointer->Data,position)) * 0x100;
	}
	else
	{
		return 0;
	}

	if (MissedOnlyCheck(pointerToCheck,positionToCheck+1))
	{
		value += (OFFS_B(pointer->Data,position+1));
	}
	else
	{
		return 0;
	}

	return value;

}



/**
Questa funzione carica un dato (uint8_t) del setting evitando problemi di incompatibilità
setting file <--> firmware grazie alla funzione MissedOnlyCheck, in seguito all'introduzione 
di nuovi parametri setting;

in pratica viene controllato se il dato che si vuole caricare esiste veramente nella tabella,
se sì viene caricato, altrimenti viene caricato al suo posto il valore 0;
può succedere quando viene introdotta una nuova funzionalità che richiede un nuovo parametro (uint8_t)
setting, quando il dato aggiunto comporta l'aggiunta di una word nella tabella, perchè in questo caso
la tabella si allunga. Se un nuovo firmware che va a cercare questo dato viene fatto girare con un vecchio
setting dove la tabella è più corta (il dato non esiste) senza questo controllo si caricherebbe
un dato random, in questo caso invece si verifica che la lunghezza della tabella comprenda il dato selezionato,
se non è così viene forzato il caricamento del dato a 0

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		position e' lo spiazzamento relativo al puntatore pointer
				del dato da caricare (uint8_t)

\return			il dato caricato

\warning		none

\use			Questa funzione va usata tutte le volte che una tabella setting si allunga con nuovi dati,
				mentre non è necessario usarla quando viene aggiunto un nuovo byte ma la lunghezza della
				tabella (in word) rimane la stessa.

\version		1.0.0
\date 			28/09/2010
\author			Claudio Fontana
*/

uint8_t MissedLoadCheck(Loader_Struct* pointer, uint8_t position)
{

Loader_Struct* pointerToCheck = pointer;
uint8_t positionToCheck = position;

	if (MissedOnlyCheck(pointerToCheck,positionToCheck))
	{
		return (OFFS_B(pointer->Data,position));	
	}
	else
	{
		return 0;	
	}
}


/**
Questa funzione serve per evitare problemi di incompatibilità setting file <--> firmware
in seguito all'introduzione di nuovi parametri setting;
in pratica viene controllato se il dato che si vuole caricare esiste veramente nella tabella,
può succedere quando viene introdotta una nuova funzionalità che richiede un nuovo parametro (uint8_t)
setting, quando il dato aggiunto comporta l'aggiunta di una word nella tabella, perchè in questo caso
la tabella si allunga. Se un nuovo firmware che va a cercare questo dato viene fatto girare con un vecchio
setting dove la tabella è più corta (il dato non esiste) grazie a questo controllo si può evitare
di caricare un dato random

la funzionalità completa viene espletata dalla funzione MissedLoadCheck che però dà per scontato
che il valore 0 del dato da caricare sia quello che disabilita la funzionalità, ma non è sempre così,
per i casi che questa condizione non è rispettata usare questa funzione prima di caricare il dato

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		position e' lo spiazzamento relativo al puntatore pointer
				del dato da caricare (uint8_t)

\return			esito del check (TRUE significa che il dato esiste)

\warning		none

\use			Questa funzione va usata tutte le volte che una tabella setting si allunga con nuovi dati,
				mentre non è necessario usarla quando viene aggiunto un nuovo byte ma la lunghezza della
				tabella (in word) rimane la stessa.

\version		1.0.0
\date 			28/09/2010
\author			Claudio Fontana
*/


bool_t MissedOnlyCheck(const Loader_Struct* pointer, uint8_t position)
{

	if (pointer->DataLenght <= (position/2))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


/**
Questa funzione serve per verificare se una variabile ha raggiunto un limite specificato in setting
tenendo conto che se il limite in setting è 0 significa che il controllo è disabilitato
questo al fine di gestire la compatibilità, difatti se la funzionalità a cui si riferisce il
test è una nuova funzionalità non prevista nei vecchi setting, succede che il riferimento in setting sarà 0
perchè il dato era fisicamente presente ma non era usato (vale 0) oppure non era proprio presente
in questo caso diventa 0 grazie alla funzione precedente MissedLoadCheck

in sintesi se il riferimento (in setting) è presente (cioè > 0) viene fatto il controllo tra la variabile
da testare ed il riferimento stesso, ritornando TRUE o FALSE a seconda dell'esito del test,
mentre se il riferimento non è presente (cioè = 0) verrà sempre ritornato FALSE

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		position e' lo spiazzamento relativo al puntatore pointer
				del dato da caricare (uint8_t)

\return			esito del test

\warning		none

\use			Questa funzione va usata tutte le volte che si può avere un problema di compatibilità
				in seguito all'introduzione di un nuovo dato in setting

\version		1.0.0
\date 			30/09/2010
\author			Claudio Fontana
*/
bool_t SettingDataCompare (uint16_t check, uint16_t reference)
{
	if ((reference > 0) && (check >= reference))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
Complementare alla precedente funzione 'SettingDtaCompare'
in pratica il test viene fatto al contarario, sempre tenendo conto che se reference=0 la condizione è falsa

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		position e' lo spiazzamento relativo al puntatore pointer
				del dato da caricare (uint8_t)

\return			esito del test

\warning		none

\use			Questa funzione va usata tutte le volte che si può avere un problema di compatibilità
				in seguito all'introduzione di un nuovo dato in setting

\version		1.0.0
\date 			22/03/2012
\author			Claudio Fontana
*/
bool_t SettingDataReverseCompare (uint16_t check, uint16_t reference)
{
	if ((reference > 0) && (check < reference))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
