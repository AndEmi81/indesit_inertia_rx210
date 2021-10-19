/*
** **************************************************************************
** **************************************************************************
*/ 
/** 
Gestisce il microprogramma Heat, 
Source File

\file    Heat.c
\ingroup Heat
\date    07/02/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, IDD.
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

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**DEBUG*/
/**DEBUG END*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

static void HEA_tgTemp_Update(void);

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
Gestisce l'esecuzione del microprogramma Heat.
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma è verificata (raggiunta temperatura),
\return S_FALSE se non è ancora verificata(temperatura non ancora raggiunta),
\return codice di errore in altri casi
 
\version		1.0.0
\date 			07/02/2008
\author			Valeria Catani

*/
uint8_t HEA_uP_Heat_Exe (uint8_t HighStatement,uint8_t LowStatement)
{

	if (Interpreter_Data[0].stepInitialized == FALSE)
	{	/* Prima iterazione del passo */
		
		uP_Heat_Data.status = HEAT_INIT;
		
	}
		
	if ( Res_Pos[LD_WASH_RES_IND]== NO_LD_POS )
	{/* Res lavaggio non presente */

		return S_OK;
	}
	
	
	/* passaggio per uP Heat */
	FltHeatToutData.uP_exe = TRUE;

	if ( uPDoorStatus == UP_DOOR_WAIT )
	{/* porta aperta o in attesa di timeout dopo la chiusura -> il uP va sospeso */

		return S_FALSE;
	}

	
	/* Gestione Warning Res OPEN :  Res ON - No Delta */
	
	
/*Init uP*/	  

	if (uP_Heat_Data.status == HEAT_INIT)
	{/* caricamento dati dalla flash */
		
	
		if ( BasicLoader( PTR_UP_HEAT, LowStatement, &uPHeat_setting_data ) != S_OK )
		{		
			return S_OK;	/*non eseguo il uP, ma abilito avanzamento passo*/
		}
		
		/* Init Tout riscaldamento in fault */
		FltHeatToutData.state = FLT_HEAT_TOUT_INIT;
	  
		/* Init flag temperatura raggiunta = FALSE */		
		CLRBIT(Flags_1,JIF_FLAG1_TARGET_TEMP_OK_POS);		
			
		/*init target temp (stacco)*/
		uP_Heat_Data.tgTemp =  OFFS_B(uPHeat_setting_data.Data,0) ;	/*uP_Heat_Data.tgTemp = ( OFFS_B(uPHeat_setting_data.Data,0) & TGTEMP_MASK );*/
		
		/*aggiornamento temperatura target in base a modificatori. 
		Viene fatto solo in Init, poi non più modificabile */
		HEA_tgTemp_Update();
		
		uP_Heat_Data.status = HEAT_EXE;
		
		return S_FALSE;	/*nella prima iterazione non vengono pilotati i carichi */
		
	}/*END HEAT_INIT*/
	
	if (uP_Heat_Data.status == HEAT_EXE)
	{
		/*controllo raggiungimento temperatura*/	
		if( GITemp < uP_Heat_Data.tgTemp )
		{/* temperatura target NON raggiunta */

			if ( TESTBIT(Flags_1,JIF_FLAG1_TARGET_TEMP_OK_POS)!= 0 )
			{/*temperatura stacco già raggiunta - abilitazione a procedere nel passo*/

				return S_OK;
			}																						 
			/*else temperatura non ancora raggiunta*/

			if (	FltHeatToutData.state== FLT_HEAT_TOUT_END )
			{/*timeout riscaldamento raggiunto*/

				return S_OK; 
			}


			LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_ON;
		}	
		else
		{/* temperatura target raggiunta - resistenza OFF*/

			/* 
			questa istruzione garantisce continuità tra le fasi se usata con routine continuità carichi in interprete
			perchè comunque in Init i carichi non vengono pilotati */

			if ( (TESTBIT(Flags_1,JIF_FLAG1_TARGET_TEMP_OK_POS) == 0) && (LoadsReq_Old[Res_Pos[LD_WASH_RES_IND]].b == RES_ON) )
			{
				LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_ON;
			}

			/* disabilito gestione fault timeout riscaldamento */
			FltHeatToutData.state = FLT_HEAT_TOUT_DISABLE;

			/* flag temperatura target raggiunta */
			SETBIT(Flags_1,JIF_FLAG1_TARGET_TEMP_OK_POS);	

			return S_OK;
		}

										
	}/*END HEAT_EXE*/

	return S_FALSE;	
}


/**
Gestisce aggiornamento della temperatura target per il uP in base alla tabella di modificatori

\param [in]		*p : puntatore al campo dati del uP
\return 		void (la temperatura target (temperatura di stacco) viene aggiornata come var globale)
 
\version		1.0.0
\date 			07/02/2008
\author			Valeria Catani

\note			in sospeso gestione modificatori
				  
*/

static void HEA_tgTemp_Update(void)
{		
uint8_t temp;
int8_t variation;
int16_t delta;
uint8_t res;
	
	temp = OFFS_B(uPHeat_setting_data.Data,0);	

	/* Gestione modificatori */

	res = GetModifierValue (OFFS_B(uPHeat_setting_data.Data,1), &variation);

	if(MOD_DONT_CARE == res || S_FALSE == res)
	{/* Se non è attivo nessun modificatore la variazione viene posta a zero */

		variation=0;
	}

	if(variation)
	{/* variazione != 0 va applicata*/
	 
		/* calcolo delta */                      
		delta = GetDeltaFromVariation(temp,variation);

		/* aggiornamento */     
		temp += (uint8_t)delta;         
	}

	uP_Heat_Data.tgTemp = temp;
}


/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
