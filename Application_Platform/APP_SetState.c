/**
\defgroup	APP_SetState Modulo microprogramma SetState
\ingroup	ApplPlatform

Il modulo implementa il microprogramma SetState (varianti NOP e RESET).
*/

/**
Source file del modulo microprogramma SetState

\file		APP_SetState.c
\ingroup	APP_SetState
\date		22/09/2009
\version	01.00.00
\author		Marco Quintini, Simone Costantini, Francesco Corvo

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
Gestisce l'esecuzione del microprogramma SetState.

\param [in]		highStatement specifica l'High Statement (T flag + N flag + 
				modificatori F1-F0 + opcode) del microprogramma.
\param [in]		lowStatement specifica il low statement (indice displacement ai dati)
				del microprogramma.
\param [in]		compartment specifica il vano in cui viene eseguito il microprogramma.

\attention		Interpreter_Data (global, struttura con i dati sullo stato
				corrente dell'interprete) e' usata come input

\return			uint8_t

\retval			S_OK/S_FALSE

\warning		none		

\author			Marco Quintini, Simone Costantini, Francesco Corvo
\date			22/09/2009
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
/*lint -e715 */
#if (MULTI_COMPARTMENT_APPL == 1)
uint8_t SST_SetState_Exe(uint8_t highStatement, uint8_t lowStatement, uint8_t compartment)
#else
uint8_t SST_SetState_Exe(uint8_t highStatement, uint8_t lowStatement)
#endif /* !MULTI_COMPARTMENT_APPL */
{
#if ( (SST_NOPNOP_ENABLE == 1) || (SST_IDROSTOP_ENABLE == 1)  )

#if (MULTI_COMPARTMENT_APPL == 1)
Interpreter_Struct *pItpData = &Interpreter_Data[compartment];
#else
Interpreter_Struct *pItpData = &Interpreter_Data[0];
#endif /* !MULTI_COMPARTMENT_APPL */

#endif /* (SST_NOPNOP_ENABLE == 1) || (SST_IDROSTOP_ENABLE == 1) */

	if(highStatement & F1)
	{
#if ((SST_NOPNOP_ENABLE == 1) || (SST_IDROSTOP_ENABLE == 1) || (SST_COOLSTATE_ENABLE == 1) || (ITP_JIF_RET_ENABLE == 1))
	
	/* Abilitata almeno una tra le varianti NOP-NOP, IDROSTOP o RETURN */

#if (SST_NOPNOP_ENABLE == 1)
		if ( COMPLETE_OPCODE(highStatement) == UP_SETSTATE_NOP_NOP )
		{
			/*lint -save -e912 */
			if (pItpData->stepInitialized == FALSE)
			/*lint -restore */
			{
				/* Prima iterazione del passo */
				return S_FALSE;	
			}
		} 
#endif	/* #if (SST_NOPNOP_ENABLE == 1) */

#if (SST_IDROSTOP_ENABLE == 1)
		/*
		In ARCADIA 1 e 2, la configurazione F0=0 e F1=1 è utilizzata per l'Idrostop.
		In tutte le altre linee prodotto la stessa configurazione di F1/F0 è
		utilizzata per il SetState RETURN
		*/
		if (COMPLETE_OPCODE(highStatement) == UP_SETSTATE_IDROSTOP)
		{
			/*lint -save -e912 */
			if ( pItpData->stepInitialized == FALSE )
			/*lint -restore */
			{
				/* prima iterazione del passo */
				
				Idrostop_Status = IDROSTOP_ON;
				
				return S_FALSE;	
			}
			
			/* iterazioni successive */
		
			if ( Idrostop_Status != IDROSTOP_END )
			{
				/* stato di ON */
				
				Idrostop_Status = IDROSTOP_ON;
				
				return S_FALSE;
			}	
		}

#elif (SST_COOLSTATE_ENABLE == 1)
		/*
		In ARCADIA DRYER, la configurazione F0=0 e F1=1 è utilizzata per indicare
		lo stato di COOLSTATE
		*/
		if (COMPLETE_OPCODE(highStatement) == UP_SETSTATE_COOLSTATE)
		{

			/* stato di ON */

			Cool_Status = COOLSTATE_ON;

			return S_FALSE;

		}

#elif (ITP_JIF_RET_ENABLE == 1)

		/* Insert code here for "SetState RETURN" */

#endif /* (ITP_JIF_RET_ENABLE == 1) */
		

#else /* (SST_NOPNOP_ENABLE == 1) || (SST_IDROSTOP_ENABLE == 1) || (SST_COOLSTATE_ENABLE == 1) || (ITP_JIF_RET_ENABLE == 1) */

		/*
		Nessuna tra le varianti NOP-NOP, IDROSTOP o RETURN  abilitata.
		F1 != 0 è una configurazione non prevista.
		*/

#ifdef _SST_CHECKERRORS_
		DBG_CheckErr(SST_ERROR_WRONG_PARAM);
#endif
		
#endif	/* (SST_NOPNOP_ENABLE == 1) || (SST_IDROSTOP_ENABLE == 1) || (SST_COOLSTATE_ENABLE == 1) || (ITP_JIF_RET_ENABLE == 1) */
	}
	else
	{
		/* F1F0 == 00 -> NOP */
		/* F1F0 == 01 -> RESET */
	}
	
	return S_OK;					
}
/*lint +e715 */

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
