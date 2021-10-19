/**
\defgroup	APP_JumpIf Modulo microprogramma JumpIf
\ingroup	ApplPlatform

Il modulo implementa il microprogramma JumpIf per i salti condizionati
*/

/**
Source file del modulo microprogramma JumpIf

\file		APP_JumpIf.c
\ingroup	APP_JumpIf
\date		22/09/2009
\version	01.00.00
\author		Marco Quintini

\par		PCLint
			Report 16/01/2009: 0 Errors, 0 Warnings, 1 Info

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
#if (JIF_AND_OR_ENABLE == 1)
/**
\name Valutazione condizioni multiple
*/
/*@{*/ 
#define COND_IN_AND				0	/**< Condizioni da valutare in AND logico */
#define COND_IN_OR				1	/**< Condizioni da valutare in OR logico */
/*@}*/ 
#endif /* (JIF_AND_OR_ENABLE == 1) */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static bool_t JIF_Equal(int32_t a, int32_t b);
static bool_t JIF_Different(int32_t a, int32_t b);
static bool_t JIF_Greater(int32_t a, int32_t b);
static bool_t JIF_Smaller(int32_t a, int32_t b);

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
/** Array dei possibili operatori di confronto tra gli operandi di una condizione */
static const FUNCOP Operator_array[N_OPERATORS] =
{
	JIF_Equal,
	JIF_Different,
	JIF_Greater,
	JIF_Smaller,
};

/* ***************** */
/* Exported function */
/* ***************** */

#if (MULTI_COMPARTMENT_APPL == 1)
/**
Implementa il microprogramma JumpIF.

\param [in]		highStatement e' l'High Statement del microprogramma (T flag +
				N flag + modificatori F1-F0 + opcode).
\param [in]		lowStatement e' il Low Statement del microprogramma (indice
				dello spiazzamento della tabella dati associata)
\param [in]		compartment e' il vano in cui viene eseguito il microprogramma

\attention		Interpreter_Data (global, struttura con i dati sullo stato
				corrente dell'interprete) e' usata come input
\attention		Table_JumpIf (global, buffer tabelle destinazione Jump If per i
				diversi vani) e' usata come output

\return			uint8_t

\retval			S_OK se la condizione di avanzamento passo per il microprogramma e'
				verificata (condizione JumpIf vera), S_FALSE se non e' ancora verificata
				(condizione JumpIF falsa) o in caso di errore.

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
/*lint -e715 */
uint8_t JIF_JumpIf_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment)

#else /* (MULTI_COMPARTMENT_APPL == 1) */

/**
Implementa il microprogramma JumpIF.

\param [in]		highStatement e' l'High Statement del microprogramma (T flag +
				N flag + modificatori F1-F0 + opcode).
\param [in]		lowStatement e' il Low Statement del microprogramma (indice
				dello spiazzamento della tabella dati associata)

\attention		Interpreter_Data (global, struttura con i dati sullo stato
				corrente dell'interprete) e' usata come input
\attention		Table_JumpIf (global, tabella destinazione Jump If) e' usata come output

\return			uint8_t

\retval			S_OK se la condizione di avanzamento passo per il microprogramma e'
				verificata (condizione JumpIf vera), S_FALSE se non e' ancora verificata
				(condizione JumpIF falsa) o in caso di errore.

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
/*lint -e715 */
uint8_t JIF_JumpIf_Exe(uint8_t highStatement, uint8_t lowStatement)

#endif /* (MULTI_COMPARTMENT_APPL != 1) */
{
#if (MULTI_COMPARTMENT_APPL == 0)
uint8_t compartment = 0;
#endif /* (MULTI_COMPARTMENT_APPL == 0) */  	

JumpIF_Struct *pCompTable_JumpIf = &Table_JumpIf[compartment];
Interpreter_Struct *pCompItpData = &Interpreter_Data[compartment];
Loader_Struct uPJumpIF_setting_data, cond_setting_data;

	/* Caricamento tabella dati JumpIF */
	if( MSettingBasicLoader( Interpreter_Data[0].mSetting, PTR_MP_JUMPIF_DATA, lowStatement, &uPJumpIF_setting_data) != S_OK )
	{
#ifdef _JIF_CHECKERRORS_
		DBG_CheckErr(JIF_ERR_LD_DATA_TAB);
#endif

		return S_FALSE;	/* return S_FALSE perchè altrimenti (con S_OK) non saprei dove saltare! */
	}

	/* Caricamento tabella condizioni JumpIF */	
	/*lint -save -e826 */
	if ( MSettingBasicLoader( Interpreter_Data[0].mSetting, PTR_COND_TABLE_DATA, ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->conditionIndex, &cond_setting_data) != S_OK )
	/*lint -restore */
	{
#ifdef _JIF_CHECKERRORS_			
		DBG_CheckErr(JIF_ERROR_LD_COND_DATA);
#endif

		return S_FALSE;	/* return S_FALSE perchè altrimenti (con S_OK) non saprei dove saltare! */
	}

	/*lint -save -e826 */
	switch(JIF_CheckCondition(&cond_setting_data))
	/*lint -restore */
	{
		case S_OK:
		{
			/* Condizione vera, effettua il salto */

			/* cycle */
			/*lint -save -e826 -e912 */
			if( ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->sameCycle != 0 )
			/*lint -restore */
			{
				/* same cycle */
				pCompTable_JumpIf->cycle = pCompItpData->cycle;
			}
			else
			{
				/* different cycle */
				/*lint -save -e826 */
				pCompTable_JumpIf->cycle = ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->destinationCycle;
				/*lint -restore */
			}	
			
			/* phase */
			/*lint -save -e826 -e912 */
			if( ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->samePhase != 0 )
			/*lint -restore */
			{
				#if (ITP_JIF_RELATIVE_PHASE_ENABLE == 1)
				/*
				inizializzo "newPhase" con il valore di "Destination Phase" contenuto
				nel file di setting. Questa variabile indica l'offset per il salto relativo
				*/
				/*lint -save -e826 */
				int8_t	newPhase = (int8_t)(((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->destinationPhase);
				/*lint -restore */

				/*lint -save -e826 */
				if (((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->signOffset)
				/*lint -restore */
				{	/* salto relativo con offset negativo */
					newPhase = -newPhase;	/* inverto il segno dell'offset */
				}

				/*lint -save -e826 */
				if ( (((int8_t)(pCompItpData->phase) + newPhase) < 0) || (((int8_t)(pCompItpData->phase) + newPhase) >= N_MAX_PHASES) )
				/*lint -restore */
				{
					/* wrong phase: rimango all'interno della fase corrente */
					pCompTable_JumpIf->phase = pCompItpData->phase;
				}
				else
				{
					/*lint -save -e826 */
					pCompTable_JumpIf->phase = (uint8_t)((int8_t)(pCompItpData->phase) + newPhase);
					/*lint -restore */
				}
				#else	/* (ITP_JIF_RELATIVE_PHASE_ENABLE != 1) */
				/* same phase */
				pCompTable_JumpIf->phase = pCompItpData->phase;
				#endif	/* (ITP_JIF_RELATIVE_PHASE_ENABLE != 1) */
			}
			else
			{
				/* different phase */
				/*lint -save -e826 */
				pCompTable_JumpIf->phase = ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->destinationPhase;
				/*lint -restore */
			}

			/*lint -save -e826 */
			pCompTable_JumpIf->first_statement = ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->destinationStatement;
			/*lint -restore */
		
#if (ITP_JIF_RET_ENABLE == 1)
			/* Gestione JumpIf con ritorno */
			/*lint -save -e826 */
			if( ((JumpIfTableData_Struct *)uPJumpIF_setting_data.Data)->ret != 0 )
			/*lint -restore */
			{
				pCompTable_JumpIf->ret = TRUE;
			}
#endif /* (ITP_JIF_RET_ENABLE == 1) */

			return S_OK;
		}
			/*break;*/
		
		case S_FALSE:
		{
			/* Condizione falsa, non effettua il salto */
			return S_FALSE;
		}
			/*break;*/

		default:
		{
			/* Possibile gestione degli errori, se necessaria */
			return S_FALSE;
		}
			/*break;*/
	}
}
/*lint +e715 */


/**
Dato il puntatore pDataCond ad un'area dati che codifica una condizione,
valuta il valore booleano della condizione stessa. \n
La condizione puo' anche essere una combinazione logica di piu' condizioni
semplici: se in APP_Config.h (JIF_AND_OR_ENABLE == 1), si possono combinare
più condizioni semplici in OR e/o in AND, altrimenti le condizioni multiple
sono valutate in AND.

\param [in]		pDataCond punta alla struttura della condizione.

\return			uint8_t

\retval			S_OK/S_FALSE se la condizione e' corretta ed e' verificata/non verificata,
				un codice di errore se i dati della condizione non sono corretti e quindi
				la stessa non può essere valutata.

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
uint8_t JIF_CheckCondition(Loader_Struct const* pDataCond)
{
uint8_t i;
Condition_Struct *pCurrentCondition;
int32_t data, operand;	/* Il tipo deve essere int32_t, per poter gestire sia int16_t
						   che uint16_t */
uint8_t partialCondValue;

	/*lint -save -e912 */
	if( pDataCond->DataLenght < CONDITION_SIZE_IN_W )
	/*lint -restore */
	{
		/*
		Lunghezza del campo dati minore della lunghezza di una condizione:
		la condizione non può essere valutata.
		*/
#ifdef _JIF_CHECKERRORS_			
		DBG_CheckErr(JIF_ERR_COND_DATA_LEN);
#endif
		return ERR_CODE;
	}
	else
	{
		i = 0;
		/* Puntatore alla condizione corrente da valutare */
		/*lint -save -e826 */
		pCurrentCondition = (Condition_Struct *)pDataCond->Data;
		/*lint -restore */
		
		/*lint -save -e912 */
		while( i < (pDataCond->DataLenght / CONDITION_SIZE_IN_W) )
		/*lint -restore */
		{
			/* Caricamento del dato e dell'operando */
			if( SelectOperands(pCurrentCondition, &data, &operand) != S_OK )
			{
				/*
				Problemi nella selezione del dato e/o dell'operando: la
				condizione non può essere valutata.
				*/
#ifdef _JIF_CHECKERRORS_
				DBG_CheckErr(JIF_ERR_OP_OR_DATA_SEL);
#endif
				return ERR_CODE;
			}

			/*
			N.B.: è superfluo introdurre il controllo
				pCurrentCondition->dataSel.operatorCode < N_OPERATORS,
			dato che è un campo a 2 bit e dunque sicuramente < 4.
			*/
			
			if( (Operator_array[pCurrentCondition->dataSel.operatorCode](data, operand)) == FALSE )
#if (JIF_AND_OR_ENABLE == 1)
			{
				/* Condizione corrente FALSE */
				if(i == 0)
				{
					/*
					Si sta valutando la prima condizione, deve essere inizializzato
					il risultato parziale.
					*/
					partialCondValue = FALSE;
				}
				else
				{
					if(pCurrentCondition->dataSel.andOr == COND_IN_AND)
					{
						/* Condizioni in AND: se una è FALSE la combinazione è fin qui falsa */
						partialCondValue = FALSE;
					}
					else
					{
						/*
						Do nothing, una condizione FALSE in OR non altera il
						risultato parziale.
						*/
					}
				}
			}
			else
			{ 
				/* Condizione corrente TRUE */
				if(i == 0)
				{
					/*
					Si sta valutando la prima condizione, deve essere inizializzato
					il risultato parziale.
					*/
					partialCondValue = TRUE;
				}
				else
				{
					if(pCurrentCondition->dataSel.andOr == COND_IN_OR)
					{
						/* Condizioni in OR: se una è TRUE la combinazione è fin qui vera */
						partialCondValue = TRUE;
					}
					else
					{
						/*
						Do nothing, una condizione TRUE in AND non altera
						il risultato parziale.
						*/
					}
				}
			}
#else /* (JIF_AND_OR_ENABLE == 1) */
			{
				/* Condizione corrente FALSE */
				
				/*
				Le condizioni sono valutate in AND: se una è FALSE la combinazione
				è sicuramente falsa, è inutile continuare a valutare le successive.
				*/
				return S_FALSE;
			}
			else
			{
				/*
				Le condizioni sono valutate in AND: finchè sono tutte TRUE, il
				risultato parziale è vero.
				*/
				partialCondValue = TRUE;
			}
#endif /* (JIF_AND_OR_ENABLE != 1) */
			i ++;
			pCurrentCondition ++;
		}
					
		/*
		Se (JIF_AND_OR_ENABLE == 0) PCLint segnala una possibile mancata inizializzazione
		di partialCondValue: in realtà, per come sono costruite i controlli in precedenza,
		il ciclo while viene sicuramente eseguito almeno una volta, per cui in questo
		punto partialCondValue è sicuramente inizializzata (ma PCLint non se ne
		accorge).
		*/
		/*lint -e644 -e771 */
		if(partialCondValue == TRUE)
		/*lint +e771 +e644 */
		{
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	
	}
}


/* ************** */
/* Local Function */
/* ************** */

/**
Valuta se a e' uguale a b.

\param [in]		a e' il primo operando
\param [in]		b e' il secondo operando

\return			bool_t

\retval 		TRUE se a == b, FALSE altrimenti

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
static bool_t JIF_Equal(int32_t a, int32_t b)
{
	return(a == b);
}


/**
Valuta se a e' diverso da b.

\param [in]		a e' il primo operando
\param [in]		b e' il secondo operando

\return			bool_t

\retval 		TRUE se a != b, FALSE altrimenti

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
static bool_t JIF_Different(int32_t a, int32_t b)
{
	return(a != b);
}


/**
Valuta se a e' maggiore di b.

\param [in]		a e' il primo operando
\param [in]		b e' il secondo operando

\return			bool_t

\retval 		TRUE se a > b, FALSE altrimenti

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
static bool_t JIF_Greater(int32_t a, int32_t b)
{
	return(a > b);
}


/**
Valuta se a e' minore di b.

\param [in]		a e' il primo operando
\param [in]		b e' il secondo operando

\return			bool_t

\retval 		TRUE se a < b, FALSE altrimenti

\warning		none		

\author			Marco Quintini
\date 			06/07/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
static bool_t JIF_Smaller(int32_t a, int32_t b)
{
	return(a < b);
}

/* *********** */
/* End of file */
/* *********** */
