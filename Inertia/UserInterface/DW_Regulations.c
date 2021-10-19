/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**

\Par	G
\Header file.
 
\file    DW_Regulations.c
\ingroup DW_Regulations
\date    12/02/2008
\author  Valeria Catani
\version 1.0.0

 Old Versions
\version 1.0.0
\date    24/01/2006
\author  Simone Costantini - Valeria Catani - Marco Quintini
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

/**
Inizializza il vettore BufFunctionsIDs[] con gli ID delle funzioni definite nel file di setting
e utilizzate dall'applicazione main.
 
\param [in] 	none
\param [out]	BufFunctionsIDs[] (global) è il vettore con gli ID delle funzioni della main
\return 		none

\notes:			se ci sono problemi di caricamento BufFunctionsIDs[i] viene forzato a FUNC_ID_GENERIC.

\author			Marco Quintini
\date   		01/06/2006
\version    	0.0.1

*/
void REG_getBufFunctionsIDs(void)
{
uint8_t i;   
Loader_Struct funcHeader;

	for( i = 0; i < FUNC_NUM_TO_MAIN; i++ )	
	{	
		BufFunctionsIDs[i] = FUNC_ID_GENERIC; 	
		if( BasicLoader((uint16_t)PTR_FUNCTION_HEADING, i, &funcHeader ) == S_OK )
		{
		   	BufFunctionsIDs[i] = ( OFFS_B(funcHeader.Data, FUNCTION_ID_OFFSET) & FUNCTION_ID_MASK );
		}
	}	    
}
/**/


/**
Restituisce la posizione della funzione nell'elenco delle funzioni definite nel file di setting e
utilizzate dall'applicazione main, effettuando la ricerca all'interno di BufFunctionsIDs[].
 
\param [in]		BufFunctionsIDs[] (global) è il vettore con gli ID delle funzioni della main
\param [in] 	functionID è il codice identificativo della funzione
\param [out]	*pFuncPos è la posizione della funzione nella lista e nel buffer delle regolazioni.
				E' anche il displacement, all'interno del campo dati individuato da PTR_FUNCTION_HEADING,
				della funzione functionID.
	 
\return 		S_OK se la funzione è stata eseguita correttamente, un codice d'errore altrimenti.

\notes:			questa routine può essere utilizzata solo dopo che BufFunctionsIDs[] è stato
				inizializzato

\author			Marco Quintini
\date   		01/06/2006
\version    	0.0.1

*/
uint8_t REG_getFuncPosition( uint8_t functionID, uint8_t *pFuncPos )
{
uint8_t i;

	for( i = 0; i < FUNC_NUM_TO_MAIN; i++ ){
	
		if( BufFunctionsIDs[i] == functionID )
		{
			*pFuncPos = i;
			
			return S_OK;
		}			 
	}
	
	return REG_ERR_FUNC_NOT_FOUND;
}
/**/

/**
Per la funzione individuata da funcPos, restituisce il valore corrispondente all'indice di 
regolazione contenuto in BufNewReg[], noto l'indice della tabella di regolazione.

\param [in]		funcPos è la posizione della funzione nell'elenco delle funzioni definite nel 
				file di setting
\param [out]	*pFunctionVal è il valore della funzione
\					 
	 
\return 		S_OK se la funzione è stata eseguita correttamente, un codice d'errore altrimenti.
\warning		REG_WRN_OUT_OF_RANGE_INDEX se l'indice di regolazione è >= del numero massimo di
				regolazioni specificato nella tabella di regolazione

\notes:			nel caso si voglia utilizzare la routine per ricavare il valore della funzione
				rispetto ad un indice di regolazione che non sia quello in BufNewReg[], occorre
				passare come parametro il buffer delle regolazioni da considerare.

\author			Marco Quintini
\date   		05/06/2006
\version    	0.0.1

*/
uint8_t REG_getFunctionValueByPos( uint8_t funcPos, uint16_t *pFunctionVal )
{
Loader_Struct funcHeader;
Loader_Struct mainRegTable;
uint8_t regTableDispl; 		/* Displacement tabella regolazione rispetto PTR_UI_MAIN_REG_PARAM_TABLE */
uint8_t dataFormat;
uint8_t stepReg; 
uint8_t valStep;	
uint16_t stepPerReg;

uint8_t indexReg;


	indexReg = (BufNewReg[funcPos] - 1);	/* L'indice di regolazione ha un offset di 1 */
      
	/** Se (BufNewReg[funcPos] == 0) si è in presenza di un caso particolare in cui comunque non
		si deve ricavare il valore della funzione dalla tabella */
	if( indexReg == 0xFF )
	{
		return REG_NULL_INDEX;
	}
 	
 	if( BasicLoader((uint16_t)PTR_FUNCTION_HEADING, funcPos, &funcHeader) != S_OK )
	{
		return REG_ERR_BASIC_LOADER;
	}
	
	regTableDispl = OFFS_B(funcHeader.Data, (uint8_t)((NUM_WORDS_FUNC_DESCR*2) + MAIN_REG_TAB_OFFSET));
	
	if( BasicLoader((uint16_t)PTR_UI_MAIN_REG_PARAM_TABLE, regTableDispl, &mainRegTable) != S_OK )
	{
		return REG_ERR_BASIC_LOADER;	
	}
    
	dataFormat = (OFFS_B(mainRegTable.Data, DATA_FORMAT_OFFSET) & DATA_FORMAT_MASK);
 
	stepReg = (OFFS_B(funcHeader.Data, (uint8_t)(NUM_WORDS_FUNC_DESCR*2)) & STEP_REG_MASK);

	if( stepReg )
	{
		valStep = OFFS_B(mainRegTable.Data, STEP_OFFSET);
		stepPerReg = (valStep * (uint16_t)(indexReg));	/*	Attenzione: se l'ozione -Cni è
															settata è necessario castare uno dei
															due operandia uint16_t,	altrimenti il
															compilatore tronca il prodotto a uint8_t */
		switch( dataFormat )
		{
			case DATA_FORMAT_UCHAR:
				*pFunctionVal = ((*(uint8_t*)(mainRegTable.Data + CHAR_INIT_VAL_OFFSET)) + stepPerReg);
			break;

			case DATA_FORMAT_SCHAR:	
				*pFunctionVal = ((*(int8_t*)(mainRegTable.Data + CHAR_INIT_VAL_OFFSET)) + stepPerReg);
			break;
			
			case DATA_FORMAT_USHORT:
				*pFunctionVal = ((*(uint16_t*)(mainRegTable.Data + WORD_INIT_VAL_OFFSET)) + stepPerReg );
			break;

			case DATA_FORMAT_SSHORT:
				*pFunctionVal = ((*(int16_t*)(mainRegTable.Data + WORD_INIT_VAL_OFFSET)) + stepPerReg );
			break;

			default:
			break;
		}
	}
	else
	{
		switch( dataFormat )
		{
			case DATA_FORMAT_UCHAR:
				 *pFunctionVal = *(uint8_t*)(mainRegTable.Data + INIT_VAL_OFFSET + indexReg);
			break;
			case DATA_FORMAT_SCHAR:
				 *pFunctionVal = *(int8_t*)(mainRegTable.Data + INIT_VAL_OFFSET + indexReg);
			break;
			case DATA_FORMAT_USHORT:
				 *pFunctionVal = *(uint16_t*)(mainRegTable.Data + INIT_VAL_OFFSET + (2*indexReg));	 
			break;
			case DATA_FORMAT_SSHORT:			
				 *pFunctionVal = *(int16_t*)(mainRegTable.Data + INIT_VAL_OFFSET + (2*indexReg));	 
			break;

			default:
			break;
		} 

	}
	
	
	/* Controllo se l'indice di regolazione è >= numero massimo di regolazioni */
	if (indexReg >= OFFS_B(funcHeader.Data, (uint8_t)((NUM_WORDS_FUNC_DESCR*2) + NUM_REG_OFFSET)))
	{
		if(stepReg)
		{
			/* Per regolazioni a step il warning potrebbe anche essere ignorato per alcune funzioni */
			return REG_WRN_OUT_OF_RANGE_INDEX;
		}
		else
		{
			return REG_ERR_OUT_OF_RANGE_INDEX;
		}
	}
	
	return S_OK; 	
}

/**
Per la funzione individuata da functionID, restituisce il valore corrispondente all'indice di 
regolazione contenuto in BufNewReg[].

\param [in]		functionID è il codice identificativo della funzione
\param [out]	*pFunctionVal è il valore della funzione
\					 
	 
\return 		S_OK se la funzione è stata eseguita correttamente, un codice d'errore altrimenti.

\author			Marco Quintini
\date   		05/06/2006
\version    	0.0.1

*/
uint8_t REG_getFunctionValueByID( uint8_t functionID, uint16_t *pFunctionVal )
{
uint8_t funcPos;


	if(REG_getFuncPosition(functionID, &funcPos) != S_OK)
	{
    	return REG_ERR_FUNC_NOT_FOUND;
	}
    
	return ( REG_getFunctionValueByPos(funcPos, pFunctionVal) );
}
/**/

/**
Controlla se l'indice di regolazione della funzione (avente per ID functionID) è diverso in 
BufNewReg[] e BufOldReg[].
 
\param [in] 	functionID è il codice identificativo della funzione
\param [in] 	BufNewReg[] (global) è il buffer delle nuove regolazioni dalla scheda Visual 
\param [in] 	BufOldReg[] (global) è il buffer delle vecchie regolazioni Main
\param [out]	none
\return 		TRUE se l'indice è diverso
				FALSE se l'indice è uguale o se non è stato possibile determinare la posizione
				della funzione nell'elenco delle funzioni.

\notes:

\author			Marco Quintini
\date   		01/06/2006
\version    	0.0.1

*/
uint8_t REG_checkIfChanged(uint8_t functionID)
{
uint8_t regPosition;

	if( REG_getFuncPosition(functionID, &regPosition) == S_OK )
	{
	    if( BufNewReg[regPosition] != BufOldReg[regPosition] )
	    {
	    	return REG_CHANGED;
	    }
	    else
	    {
	    	return REG_UNCHANGED;
	    }
	}
	else
	{
    	return REG_NOT_EXIST;
    }
}
/**/

/**
Pone BufOldReg[] della funzione avente per ID functionID uguale a BufNewReg[].

\param [in] 	functionID è il codice identificativo della funzione
\param [in] 	BufNewReg[] (global) è il buffer delle nuove regolazioni (dalla scheda Visual)
\param [out] 	BufOldReg[] (global) è il buffer delle vecchie regolazioni Main

\return 		none

\notes:

\author			Marco Quintini
\date   		01/06/2006
\version    	0.0.1

*/
void REG_updateOldReg(uint8_t functionID)
{
uint8_t regPosition;

	if( REG_getFuncPosition(functionID, &regPosition) == S_OK )
	{
	    BufOldReg[regPosition] = BufNewReg[regPosition];
	}
}
/**/

/**
Pone BufOldReg[] delle funzioni 'To Main' non esplicitamente processate in SPV_superVisor()
uguale a BufNewReg[].

\param [in] 	BufNewReg[] (global) è il buffer delle nuove regolazioni (dalla scheda Visual)
\param [out] 	BufOldReg[] (global) è il buffer delle vecchie regolazioni Main

\return 		none

\notes:

\author			Marco Quintini
\date   		25/07/2006
\version    	1.0.0

*/
void REG_updateFuncToMainOldReg(void)
{
uint8_t i;   

	for( i = 0; i < FUNC_NUM_TO_MAIN; i++ )	
	{	
		switch(BufFunctionsIDs[i])
		{
			case FUNC_ID_SELECTOR:
			case FUNC_ID_START:
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
			case FUNC_ID_DELAY_HH:
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
			case FUNC_ID_DELAY_MM:
			case FUNC_ID_TEMPERATURE:


			/* Aggiungere le altre funzioni processate esplicitamente in SPV_superVisor §_§ */

			/* Do nothing, processed separately */ 

			break;
			
			default:
			
				BufOldReg[i] = BufNewReg[i];
			
			break;
		}
	}	    


}

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
