/**
Source File 

\file    7literManager.c
\ingroup 7literManager
\date    14/07/2011
\author  Leonardo Ripoli
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


/** 
\todo :  gestione modificatori da implementare e provare		
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


/* ******************* */
/* External variables  */
/* ******************* */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Gestisce l'esecuzione del microprogramma 7 litri.
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (termine conteggio),
\retvalue S_FALSE se non è ancora verificata(conteggio in corso),
 
\version		1.0.0
\date 			1/2/2008
\author			Roberto Fioravanti

\versione 1.1.0
\date	27/9/2010
\author Emanuela Antognoli
\note: cambiata la gestione dei timers per la piattaforma 2.0: se il timer è a 15 bit la gestione dell'accumulatore 
\non serve più quindi il conteggio massimo è 0x3FFF secondi.

*/
uint8_t SL_uP_7liter_Exe (uint8_t HighStatement,uint8_t LowStatement)
{

	if(EV_Pos[LD_7LITER_EV_IND] == NO_LD_POS)
	{ /* condizione di errore */
	    return S_OK;
	}
	
	if (Interpreter_Data[0].stepInitialized== FALSE)
	{	/*prima iterazione del passo*/

		uP_7liter_Data.status = SL_INIT;
	}
	  
	if (CurrentFault[0].code == CODE_NO_FAULT)
	{/* non sono in fault - controllo lo stato della porta 
		In caso di Fault il uP può agire anche a porta aperta, non eseguo controllo  */

		if ( uPDoorStatus == UP_DOOR_WAIT ) 
		{ /* porta aperta -> il uP va sospeso */

			(void)SWTC_Stop((SWTC_CountersType)MP_7LITER_M_COUNTER);

			return S_FALSE;
		}
	}
	
	if (uP_7liter_Data.status == SL_INIT) 
	{
		/* caricamento dati dal setting */

		if ( BasicLoader ( PTR_UP_7LITER, LowStatement, &uP7liter_setting_data ) != S_OK )
		{			
			return S_OK;			
		}

		uP_7liter_Data.timeON_ev = (OFFS_W(uP7liter_setting_data.Data,0)) & SL_TIME_MASK;

		uP_7liter_Data.timeOFF_ev = (OFFS_W(uP7liter_setting_data.Data,1)) & SL_TIME_MASK;

		(void)Set_MediumCounter((SWTC_CountersType)MP_7LITER_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_MCounterValue)(uP_7liter_Data.timeON_ev)); 
						
		uP_7liter_Data.status = SL_EV_OPEN_EXE;

	} /* SL_INIT */
	
	if (uP_7liter_Data.status == SL_EV_OPEN_EXE) 
	{
		/*CHECK COUNTER*/

		if ( (( SWTC_IsExpired((SWTC_CountersType)MP_7LITER_M_COUNTER) ) ))
		{/*End counting*/

		    (void)Set_MediumCounter((SWTC_CountersType)MP_7LITER_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_MCounterValue)(uP_7liter_Data.timeOFF_ev)); 
			uP_7liter_Data.status = SL_EV_CLOSE_AND_PAUSE;	
		}
		else
		{
			/* velocità pompa lavaggio richiesta in questa iterazione */

			if( TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) )
			{
				SWTC_MCounterValue _7liter_timeout_local;
				_7liter_timeout_local=(GET_7LITER_MAXTIME_FROM_SETTING());
		        (void)Set_MediumCounter((SWTC_CountersType)SLITER_DIRTYWATER_TIMEOUT_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_15MIN, (SWTC_MCounterValue)(_7liter_timeout_local));
			    (void)SWTC_Run((SWTC_CountersType)SLITER_DIRTYWATER_TIMEOUT_M_COUNTER);
				uP_7liter_Data.Sir_I_HaveLoadedSomeWater=1;		
			}
			else
			{
				uP_7liter_Data.Sir_I_HaveLoadedSomeWater=0;		
			}
			
			LoadsReq_New[EV_Pos[LD_7LITER_EV_IND]].b = EV_ON; /* pilotaggio EV 7 litri */ 
			(void)SWTC_Run((SWTC_CountersType)MP_7LITER_M_COUNTER);			
		}

	} /* SL_EV_OPEN_EXE */
		
	if (uP_7liter_Data.status == SL_EV_CLOSE_AND_PAUSE) 
	{
		/*CHECK COUNTER*/

		if ( (( SWTC_IsExpired((SWTC_CountersType)MP_7LITER_M_COUNTER) ) ))
		{/*End counting*/

			uP_7liter_Data.status = SL_END;	

		}
		else
		{
			LoadsReq_New[EV_Pos[LD_7LITER_EV_IND]].b = EV_OFF; /* pilotaggio EV 7 litri */ 
			(void)SWTC_Run((SWTC_CountersType)MP_7LITER_M_COUNTER);
		}

	} /* SL_EV_CLOSE_AND_PAUSE */

	if (uP_7liter_Data.status == SL_END)
	{
		if(uP_7liter_Data.Sir_I_HaveLoadedSomeWater==0)
		{
			SWTC_MCounterValue _7liter_timeout_local;
			_7liter_timeout_local=(GET_7LITER_MAXTIME_FROM_SETTING());
            /* LR 7/9/2011: carico il contatore con il valore del setting (in realtà basterebbe un valore qualsiasi superiore a -1) */
			(void)Set_MediumCounter((SWTC_CountersType)SLITER_DIRTYWATER_TIMEOUT_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_15MIN, (SWTC_MCounterValue)(_7liter_timeout_local));
		    (void)SWTC_Stop((SWTC_CountersType)SLITER_DIRTYWATER_TIMEOUT_M_COUNTER);
	    }
		LoadsReq_New[EV_Pos[LD_7LITER_EV_IND]].b = EV_OFF; /* pilotaggio EV 7 litri */ 
		(void)SWTC_Stop((SWTC_CountersType)MP_7LITER_M_COUNTER);

		return S_OK;

	}/*MAINTAIN_TRUE*/

	return S_FALSE;
}
/**/

/* ************** */
/* Local Function */
/* ************** */


/* *********** */
/* End of file */
/* *********** */
