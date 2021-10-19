/**
Source File 

\file    Drain.c
\ingroup Drain
\date    25/02/2008
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
\author Roberto Fioravanti
*/


/** 
\note : 
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
static void DRN_uP_Drain_Init(void);
static uint8_t DRN_StepMgmt(uint8_t highStatement, uint8_t lowStatement);
static uint8_t Check_Sensor (uint8_t highStatement);
static Loader_Struct Drn_get_time( void);
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
Gestisce l'esecuzione del microprogramma Drain.

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -gestione della sospensione del micorprogramma in presenza di faults
La condizione sulla porta viene testata solo se non ci sono faults
            
*/


uint8_t DRN_uP_Drain_Exe( uint8_t highStatement, uint8_t lowStatement)
{
uint8_t nRet=S_FALSE;


	if ( (FALSE == Interpreter_Data[0].stepInitialized) || (FALSE == Interpreter_Data[0].optionalStepInitialized) ) 
	{
		/* Init passo o restore from backup */

		DRN_uP_Drain_Init();
	} 
	else 
	{
	/* TODO1: condizioni particolari in cui deve essere sospesa l'attività del microprogramma */

		/*Pompa Scarico non sottoposta a MR, neanche alla DOOR ma garantiamo spegnimento di tutti
		i carichi a porta aperta*/


		if(HWC_Flags.bit.bldc_drain == 0)
		{ /* nel caso vecchio (pompa scarico non bldc), con la porta aperta bisognava interrompere lo scarico */
		   /*IN FAULT CONSENTIRE LO SCARICO A PORTA APERTA!!!!!*/
			if (CurrentFault[0].code == CODE_NO_FAULT)
			{/* non sono in fault - controllo lo stato della porta 
			In caso di Fault il uP può agire anche a porta aperta, non eseguo controllo  */

				if ( uPDoorStatus == UP_DOOR_WAIT )
				{  /* porta aperta -> il uP va sospeso */

					(void)SWTC_Stop((SWTC_CountersType)MP_DRAIN_M_COUNTER);

					return S_FALSE;
				}
			} 
		}
		/*		
		}
		*/
		nRet= DRN_StepMgmt(highStatement, lowStatement);
	}

	return nRet;   
}

/* ************** */
/* Local Function */
/* ************** */

/**
Inizializza la macchina a stati del microprogramma Drain.

\param nesssuno
\return nessuno

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

*/
static void DRN_uP_Drain_Init(void)
{
	uP_Drain_Data.step = DRAIN_INIT;
}


/**
Gestisce l'avanzamento della macchina a stati del microprogramma Drain.
Due sono le opzioni possibili.
2 sono le opzioni possibili

-F1|F0 = 0|0 -> scarica a vuoto pressostato + x sec.
-F1|F0 = 0|1 -> scarica per x sec

Prima opzione :
Al termine,  il microprogramma azzera WaterInDW.e aggiorna i dl caricati.
La pompa viene attivata per tempo massimo pari al timeout Drain1stTimeoutToEmpty sec.
Se il vuoto prerssostato viene raggiunto, allora la pompa rimane attivata per x sec.
Altrimenti, dopo una pausa di DrainPauseToEmpty secondi in cui la pompa è disattivata.
si passa ad un secondo tentativo di Drain2ndTimeoutToEmpty secondi.
Se il vuoto prerssostato viene raggiunto, allora la pompa rimane attivata per x sec, 
altrimenti viene venerato un allarme.

Seconda opzione:
la pompa viene attivata per x secondi.
terza opzione :
viene controllato il  valore del sensore virtuale e non il pressostato


\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/
static uint8_t DRN_StepMgmt(uint8_t highStatement, uint8_t lowStatement)
{
uint8_t time, nRet=S_FALSE;

    if((HWC_Flags.bit.vs_drain_presence) && (IS_VIRTUAL_SENSOR_VERSION(highStatement)))
	{
		if(HWC_Flags.bit.bldc_drain == 0)
		{ /* condizione di errore */
		    return S_OK;
		}
	}

	switch(uP_Drain_Data.step) 
	{
		case DRAIN_INIT:

			CLRBIT(FltFlagsKO,WARNING_DRAIN_KO);
			CLRBIT(FltFlagsKO,WARNING_DRAIN_PREKO);

			FLT_ResetFaultDetectProc( FLT_CODE_DRAIN, FLT_SUBCODE_DRAIN,(uint16_t)TOUT_RESET_PREFAULT );

			/* QI 01.12.09 reset Prefault : la procedura di Fault � confinata 
			nell'esecuazione del uP*/	
			/*FLT_preFaultReset(FLT_CODE_DRAIN,FLT_SUBCODE_DRAIN); */

			if (BasicLoader(PTR_UP_DRAIN, lowStatement, &uPDrain_setting_data) != S_OK ) 
			{
				uP_Drain_Data.step = DRAIN_ERROR;
				return S_FALSE;
			}

			switch( (highStatement) & (DRN_DRAIN_TYPE_MSK ))
			{
                case DRN_DRAIN_VP_PLUS_TIME_SENSORI_VIRTUALI:
			   /* fall through to next case */
				case DRN_DRAIN_VP_PLUS_TIME:

					/* 1° timeout pompa scarico */
					time=DRN_1ST_TOUT_TO_EMPTY(Drn_get_time());
					uP_Drain_Data.step = DRAIN_1ST_TOUT;
					break;

				case DRN_DRAIN_TIME:
					time=DRN_DRAINTIME;
					uP_Drain_Data.step = DRAIN_TIME;
					break;

				default:
					uP_Drain_Data.step = DRAIN_ERROR;
					return S_FALSE;
			}

			(void)SWTC_SetMValue((SWTC_CountersType)MP_DRAIN_M_COUNTER, (SWTC_MCounterValue)time);

			break;

		case DRAIN_1ST_TOUT:
			/* gestione 1° timeout pompa scarico */

			/* attiva pompa scarico */
			SET_PUMP(LD_DRAIN_PUMP_IND,LOAD_ON);
			FLT_ResetFaultDetectProc( FLT_CODE_DRAIN, FLT_SUBCODE_DRAIN,(uint16_t)TOUT_RESET_PREFAULT );

			if( Check_Sensor(highStatement))
			{
				(void)SWTC_Run((SWTC_CountersType)MP_DRAIN_M_COUNTER);

				if ( SWTC_IsExpired((SWTC_CountersType)MP_DRAIN_M_COUNTER)) 
				{
					/* 1° timeout scaduto: pompa scarico in pausa */
					SETBIT(FltFlagsKO,WARNING_DRAIN_PREKO);

					(void)SWTC_SetMValue((SWTC_CountersType)MP_DRAIN_M_COUNTER, DRN_PAUSE_TO_EMPTY(Drn_get_time()));

					uP_Drain_Data.step=DRAIN_PAUSE;
				}
			} 
			else
			{
				(void)SWTC_SetMValue((SWTC_CountersType)MP_DRAIN_M_COUNTER,DRN_DRAINTIME);

				uP_Drain_Data.step = DRAIN_TIME;
			}          
			break;

		case DRAIN_PAUSE:
		/* gestione pausa pompa scarico */

			(void)SWTC_Run((SWTC_CountersType)MP_DRAIN_M_COUNTER);

			if ( SWTC_IsExpired((SWTC_CountersType)MP_DRAIN_M_COUNTER)) 
			{
				/* pausa terminata: 2° timeout pompa scarico */


				(void)SWTC_SetMValue((SWTC_CountersType)MP_DRAIN_M_COUNTER,DRN_2ND_TOUT_TO_EMPTY(Drn_get_time()));

				uP_Drain_Data.step=DRAIN_2ND_TOUT;
			}

			break;

		case DRAIN_2ND_TOUT:
			/* attiva pompa scarico */
			SET_PUMP(LD_DRAIN_PUMP_IND,LOAD_ON);

            if( Check_Sensor(highStatement))

			{
				(void)SWTC_Run((SWTC_CountersType)MP_DRAIN_M_COUNTER);

				if (SWTC_IsExpired((SWTC_CountersType)MP_DRAIN_M_COUNTER)) 
				{
					/* 2° timeout scaduto: notifica allarme */
					SETBIT(FltFlagsKO,WARNING_DRAIN_KO);
					nRet=S_OK;
					uP_Drain_Data.step = DRAIN_END;
				}
			} 
			else
			{
				(void)SWTC_SetMValue((SWTC_CountersType)MP_DRAIN_M_COUNTER,DRN_DRAINTIME);


				uP_Drain_Data.step = DRAIN_TIME;
			}

			break;
      
		case DRAIN_TIME:

			/* attiva pompa scarico */
			SET_PUMP(LD_DRAIN_PUMP_IND,LOAD_ON);

			(void)SWTC_Run((SWTC_CountersType)MP_DRAIN_M_COUNTER);

			if ( SWTC_IsExpired((SWTC_CountersType)MP_DRAIN_M_COUNTER)) 
			{
				/* 2° DrainTime trascorso */
				nRet=S_OK;

				if( ((highStatement) &( DRN_DRAIN_TYPE_MSK)) != DRN_DRAIN_TIME )
				{			
					updateRegWaterLoaded();

					WaterInDW = 0;

					/* restart dati turbina */
					WaterLoadedReset();

					/* QI 06.11.09 - reset variabili legate alla gestione del timeout dinamico */
					Pump_Load_Data.init = FALSE;
					CLRBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* reset flag overflow ripristini */
					(void)SWTC_Stop((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
				}

				uP_Drain_Data.step = DRAIN_END;
			}
			FLT_ResetFaultDetectProc( FLT_CODE_DRAIN, FLT_SUBCODE_DRAIN,(uint16_t)TOUT_RESET_PREFAULT );

			break;
    
		case DRAIN_END:                                           
		case DRAIN_ERROR:

			(void)SWTC_Stop((SWTC_CountersType)MP_DRAIN_M_COUNTER);

			nRet=S_OK;
			break;

		default:
			uP_Drain_Data.step=DRAIN_INIT;
			nRet=S_FALSE;
	}

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
	if(HWC_Flags.bit.bldc_drain != 0)
	{
	    if(LoadsReq_New[Pump_Pos[LD_DRAIN_PUMP_IND]].b==LOAD_ON)
		{
			SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],OFFS_B(uPDrain_setting_data.Data, DRAINPUMP_CTRL_SPEED_DISPL)*DRAINPUMP_CTRL_BLDC_SPEED_MUL);
			SET_BLDC_TIME(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],OFFS_B(uPDrain_setting_data.Data, DRAINPUMP_CTRL_TIME_DISPL)*DRAINPUMP_CTRL_BLDC_RAMP_TIME_MUL);
		}
		else
		{
			SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],0);
			SET_BLDC_TIME(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],0);
		}
    }
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/

	return nRet;
}
/**/


/**
Gestisce il riconoscimento dello stato del pressostato o dei sensori visrtuali se sono presenti 

\param [in] none
 
\retvalue S_OK se il pressostato è in pieno o se il valore del sensore virtuale da il pieno.
\retvalue S_FALSE altrimenti

\version		1.0.0
\date 			28/11/2011
\author			Emanuela Antognoli

            
*/
static uint8_t Check_Sensor (uint8_t highStatement)
{
	
	if(IS_VIRTUAL_SENSOR_VERSION(highStatement))
	{
	   if(TESTBIT(GIValid, GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))
	   {
			   
		   if(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN])
		   {/* NB: 28/11/11 il valore del sensore virtuale della pompa scarico ha valori:0-2-3-5*/
			     return TRUE;					   
		   }
	   }else
	   {/* se il sensore vituale non è ready si comincia a scaricare a sensore, c'è comunque il timeout.  */
		   return TRUE;
		   
	   }   
	}
	else
	{/* vecchia gestione col pressostato( anche con la doppia pompa BLAC */
		return (TESTBIT(GICriteria, GI_PRESS_SENS_CR_FULL_IND) != 0);
	}
	return FALSE;
}

static Loader_Struct Drn_get_time( void )
{
	if( SPV_isAutotestOrTestRunning() )
	{
		return Tab_DrainFault_Special;
	}else
	{
		return Tab_DrainFault;
	}
}
/* *********** */
/* End of file */
/* *********** */
