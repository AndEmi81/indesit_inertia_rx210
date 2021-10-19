/**
Source File 

\file    Maintain.c
\ingroup Maintain
\date    13/02/2008
\author  Roberto Fioravanti
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
Gestisce l'esecuzione del microprogramma Maintain.
 
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
uint8_t MTN_uP_Maintain_Exe (uint8_t HighStatement,uint8_t LowStatement)
{
uint8_t res;
int8_t perc=0;
int16_t delta;
#if defined(CONFIG_LIB_ADC_CALIBRATION)
struct Loader CalibrationDataPtr_Factory;
#endif /* defined(CONFIG_LIB_ADC_CALIBRATION) */

	if (Interpreter_Data[0].stepInitialized== FALSE)
	{	/*prima iterazione del passo*/

		uP_Maintain_Data.status = MAINTAIN_INIT;
	}
	
  
	if (CurrentFault[0].code == CODE_NO_FAULT)
	{/* non sono in fault - controllo lo stato della porta 
		In caso di Fault il uP può agire anche a porta aperta, non eseguo controllo  */

		if ( uPDoorStatus == UP_DOOR_WAIT ) 
		{ /* porta aperta -> il uP va sospeso */

			(void)SWTC_Stop((SWTC_CountersType)MP_MAINTAIN_M_COUNTER);

			return S_FALSE;
		}
	}
	
	if (uP_Maintain_Data.status == MAINTAIN_INIT) 
	{
		/* caricamento dati dal setting */

		if ( BasicLoader ( PTR_UP_MAINTAIN, LowStatement, &uPMaintain_setting_data ) != S_OK )
		{			
			return S_OK;			
		}

#if defined(CONFIG_LIB_ADC_CALIBRATION)

		LKT_FIELD_S(uP_Maintain_Data.time_target) = ( (*(uint16_t*)uPMaintain_setting_data.Data) & MTN_TIME_MASK );
		

#else /*(CONFIG_LIB_ADC_CALIBRATION )*/

		LKT_FIELD_S(uP_Maintain_Data.time_target) = *(uint16_t*)uPMaintain_setting_data.Data;

#endif /*(CONFIG_LIB_ADC_CALIBRATION )*/		
		if(MTN_MOD_TAB == 0x7F)
		{
			res=S_FALSE;
		}
		else
		{
		    /* Gestione modificatori */
		    res=GetModifierValue(MTN_MOD_TAB, &perc);
		}
		if( ( MOD_DONT_CARE == res ) || ( S_FALSE == res ) )
		{
			perc=0;
		}
		
		/* aggiornamento target conteggio */
		if(perc)
		{		    
			delta = GetDeltaFromVariation(uP_Maintain_Data.time_target,perc);
			LKT_FIELD_S(uP_Maintain_Data.time_target) += delta;
		}
	
		/* Gestione fattore correttivo */ 
#if defined(CONFIG_LIB_ADC_CALIBRATION)

		if ( TESTBIT((*(uint16_t*)uPMaintain_setting_data.Data),MTN_CORR_FLAG_BIT) && (LIB_CAL_GetCalibData((uint8_t)ID_CAL_PNTR_FACTORY, &CalibrationDataPtr_Factory) == BSLDR_OK))
		{	/* richiesta applicazione fattore correttivo */
			/* aggiornamento target conteggio */
			delta = 0;

			if ( ( OFFS_B(uPMaintain_setting_data.Data,3) & MNT_RES_FOR_CORR_MASK ) == 0 )
			{/* indica di quale resistenza fare applicare il fattore di correzione*/
				/* RESISTENZA LAVAGGIO*/
				if((uint8_t)CalibrationDataPtr_Factory.Data[WASH_RESISTOR_CAL_OFFSET] != 0xFF)
				{				
					delta = ((int8_t)CalibrationDataPtr_Factory.Data[WASH_RESISTOR_CAL_OFFSET]*(int32_t)uP_Maintain_Data.time_target)/MNT_CALIBRATION_UP_MANTAIN_CST;
				}
			}
			else
			{	/* RESISTENZA ASCIUGATURA */
				if((uint8_t)CalibrationDataPtr_Factory.Data[DRY_RESISTOR_CAL_OFFSET] != 0xFF)
				{
					delta = ((int8_t)CalibrationDataPtr_Factory.Data[DRY_RESISTOR_CAL_OFFSET]*(int32_t)uP_Maintain_Data.time_target)/MNT_CALIBRATION_UP_MANTAIN_CST;
				}
			}

			LKT_FIELD_S(uP_Maintain_Data.time_target) += delta;
		}
#endif /*(CALIBRATION_ENABLE == 1)*/
		/**/		
		/* Config + init downcounter */
		/*
		Attenzione: a differenza di DEA602 il contatore ? un down counter, quindi
		l'inizializzazione deve essere fatta dopo tutte le potenziali modifiche
		(modificatori e/o calibrazione) a uP_Maintain_Data.time_target
		*/
		(void)Set_MediumCounter((SWTC_CountersType)MP_MAINTAIN_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_MCounterValue)(uP_Maintain_Data.time_target)); 

						
		uP_Maintain_Data.status = MAINTAIN_EXE;

	} /* MAINTAIN_INIT */
	
	if (uP_Maintain_Data.status == MAINTAIN_EXE) 
	{
		/*CHECK COUNTER*/

		if ( (( SWTC_IsExpired((SWTC_CountersType)MP_MAINTAIN_M_COUNTER) ) ))
		{/*End counting*/

			uP_Maintain_Data.status = MAINTAIN_END;	

			SETBIT(Flags_1,JIF_FLAG1_MAINTAIN_TARGET_OK_POS);	
			/*
			conteggio concluso-> flag utile per JumpIF
			Mantieni utilizzato come Timeout per uscita
			dal passo corrente
			*/
		}
		else
		{
			(void)SWTC_Run((SWTC_CountersType)MP_MAINTAIN_M_COUNTER);
		}

	}/*MAINTAIN_EXE*/
	
	if (uP_Maintain_Data.status == MAINTAIN_END)
	{
		(void)SWTC_Stop((SWTC_CountersType)MP_MAINTAIN_M_COUNTER);

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
/* *********** */
