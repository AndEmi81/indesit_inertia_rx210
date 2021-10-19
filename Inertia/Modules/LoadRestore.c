/**
Source File 

\file    LoadRestore.c
\ingroup LoadRestore
\date    18/03/2008
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
#include "PM_PulseCounterExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void LOR_init(void);
static void LOR_timeLoad_Init(void);
static uint8_t LOR_timeLoadEnd_Check(void);
static void LOR_waterInDW_timeBased_Update(void);

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
Gestisce l'esecuzione del microprogramma Load Restore .
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma è verificata (caricamento acqua concluso),
\return S_FALSE se non è ancora verificata(caricamento acqua in corso),

 
\version		1.0.0
\date 			18/03/2008
\author			Valeria Catani

\NB: il up con i sensori virtuali non è usato.

\note	in sospeso pressostato lineare

*/
uint8_t LOR_uP_LoadRestore_Exe (uint8_t HighStatement,uint8_t LowStatement)
{
	Loader_Struct uPLoadRestore_setting_data;

	if (Interpreter_Data[0].stepInitialized == FALSE )
	{
		/* Prima iterazione del passo */

		uP_LoadRestore_Data.status = LOAD_RESTORE_INIT;
	}

	if (uPDoorStatus == UP_DOOR_WAIT)
  	{/* porta aperta o in attesa di timeout dopo la chiusura o delay MR-> il uP va sospeso */
   
	    
		(void)SWTC_Stop((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);
	    (void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
#if 0 /* EA FAULT*/
		(void)SWTC_Stop((SWTC_CountersType)MP_PUMP_LOAD_M_COUNTER);
#endif
	    (void)SWTC_Stop((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER);

	    uP_LoadRestore_Data.init = TRUE;

		return S_FALSE;
			    
	}

	
  
  /* Test validità stato criterio pieno/vuoto */			 
	                                       
	if ( (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) == 0) )
	{
		/*
		Il criterio pieno/vuoto è in uno stato non valido o non è presente:
		*/

		/* Non eseguo il uP, ma abilito avanzamento passo */
		return S_OK;
	}


	if (uP_LoadRestore_Data.status == LOAD_RESTORE_INIT)
	{
	 
	  /* Caricamento dati dalla flash */


		if (BasicLoader (PTR_UP_LOAD_RESTORE, LowStatement, &uPLoadRestore_setting_data) != S_OK)
		{

			/* Non eseguo il uP, ma abilito avanzamento passo */
			return S_OK;
		}
		
        TempCycle_Stats.flags.bit.restoreCntEn=1;
		 
        uP_LoadRestore_Data.maxWater = OFFS_B(uPLoadRestore_setting_data.Data,0);	
		
		
		/* Init variabili per caricamento a turbina */
		
		uP_LoadRestore_Data.init = TRUE;

		
		PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);             /*            Reset di eventuali warning presenti*/
		

		uP_LoadRestore_Data.fullPulseIn = 0;                  /* init var per conteggio impulsi entranti in pieno */
						
		LOR_init();                                           /* init per caricamento con fullPulseIn = 0*/
    
		(void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT);     /* init funzione controllo turbina bloccata */
		CLRBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);

		uP_LoadRestore_Data.status = LOAD_RESTORE_EXE;
		
		/* Nella prima iterazione non vengono pilotati i carichi */
		return S_FALSE;
			  
	} /*LOAD_RESTORE_INIT*/
	
	
	/* QI 05.11.09 gestione timeout caricamento dinamico: vale in Load Restore perchè 
	implicitamente si assume che con il Load Restore ci sia sempre attivazione pompa, ovvero sia
	sempre un caricamento dinamico */

	SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_EXE);

	if ( TESTBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS) )
	{
		
		/* in analogia con la dichiarazione del KO Restore per WaterInDW > maxWater
		non si prosegue nel caricamento e viene abilitato avanzamento passo 
		Il Jump su KO Restore verrà processato nel passo successivo 
		se Load Restore è l'unico uP del passo*/
		
		uP_LoadRestore_Data.status = LOAD_RESTORE_TRUE; 
		
	}
	
	
	if (uP_LoadRestore_Data.status == LOAD_RESTORE_EXE)
	{
	
		if ( (PM_PulseCounter_GetWarning() == NO_PULSE_COUNTER_WARNING) && (TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) == 0) )
		{/* Pulse counter OK && non bloccata */

			if ( WaterInDW >= uP_LoadRestore_Data.maxWater )
			{/* raggiunto livello massimo consentito */

				if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
				{/*vuoto*/

					SETBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* set flag overflow ripristini */
          
					  /* incremento contatore KO Restore per statistiche
					  posso farlo qui perchè eseguo istruzione una volta soltanto dopo aver messo ad 1 il flag KO RESTORE
					  e poi cambio stato in LOAD_RESTORE_TRUE */
#ifdef _STATISTICS_ON_                        
					MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.koRestore++;
#endif        
        /*DisableCounter(&PulseCounterStill_timer);viene fatto in LOAD_RESTORE_TRUE*/
								
					uP_LoadRestore_Data.status = LOAD_RESTORE_TRUE;

				}
				else
				{/* pieno -> interrompo il Restore */
				
					/*uP_LoadRestore_Data.init = TRUE;*/

					/* Rimango in questo stato anche se consento avanzamento passo */
					return S_OK;

				}
			}
			else
			{/* ancora possibile caricare acqua - controllo stato pressostato */
      
				if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
				{ /* vuoto */
          
					if (uP_LoadRestore_Data.init == TRUE)
					{/* questa init potrebbe avvenire anche dopo un passaggio per il pieno per cui fullPulseIn != 0*/

							LOR_init();
            
							/* QI 28.10.09 init finestra di buio su controllo turbina. Messo qui per reinizializzare contatore al POR */

							(void)SWTC_SetSValue((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER, (SWTC_SCounterValue)TOUT_PULSE_COUNT_CHECK_DELAY);

            
              
							uP_LoadRestore_Data.init = FALSE;
              
					}
          
					WaterInDW = (uP_LoadRestore_Data.waterInDWInit + GIWaterLoaded);  /* aggiornamento acqua in vasca con quella caricata */

					TempCycle_Stats.loaded_water += GIWaterLoaded;
					cycle_loaded_water += GIWaterLoaded;
					/* QI 27.10.09 gestione finestra di buio su check turbina */
          
					FLT_PulseCounterCheckDelay_Mgmt();
          

#ifdef _STATISTICS_ON_
					if((GIWaterLoaded>5) && (TempCycle_Stats.flags.bit.restoreCntEn==1))
					{
						MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.restore++;
						TempCycle_Stats.flags.bit.restoreCntEn =0 ;
					}
#endif
          
					uP_LoadRestore_Data.emptyPulseIn = (GIWaterLoaded * FlowMeterDivider);	 /* aggiornamento acqua in vasca in impulsi,
                                                                                      rappresenta l'acqua in dl convertita in impulsi   
                                                                                      valore salvato per maggiore precisione quando riparto dal p.p. */
                    				
					LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/
				
					/* controllo turbina bloccata */
					if ( FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_EXE) == TRUE )
					{
            
						/* entro qui una sola volta,poi cambio modalità di ripristino */
						/* salva nella pila dei last prefault*/
						FLT_warningSave(FLT_CODE_PULSE_COUNTER, FLT_SUBCODE_PULSE_COUNTER, WRN_PREFAULT);
            
						SETBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);
            
					}
	  
					return S_FALSE;
	                
				}
				else
				{/* pieno */
         
					/*in pieno non piloto EV ma tengo conto di eventuali impulsi conteggiati dalla turbina */
                    
					/*impulsi contati durante la permanenza in pieno */
					if ( PulseLoaded >= uP_LoadRestore_Data.emptyPulseIn )
					{
						uP_LoadRestore_Data.fullPulseIn = ( PulseLoaded - uP_LoadRestore_Data.emptyPulseIn );
					}
				
					/*METTI GIWaterLoaded = 0, magari nella procedura di Init, qui manca*/
					uP_LoadRestore_Data.init = TRUE;
					
					(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);

					/* Rimango in questo stato anche se consento avanzamento passo */

					return S_OK;
					
				}
					
			}

		}
		else
		{/* Warning Pulse Counter: il conteggio dell'acqua in ingresso non è affidabile ->
			  non eseguo uP ma abilito avanzamento passo 	 */
      
			uP_LoadRestore_Data.status = LOAD_RESTORE_EXE_TIME;
      
			uP_LoadRestore_Data.init = TRUE;  /* uso la stessa var perchè sono in uno stato diverso */
      
		}
        
   }/*LOAD_RESTORE_EXE*/
   
   if (uP_LoadRestore_Data.status == LOAD_RESTORE_EXE_TIME)
   {/* ripristino a tempo */
   
	   if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
	   {/* vuoto -> posso ancora caricare acqua, se ho tempo residuo */

			if (uP_LoadRestore_Data.init == TRUE)
			{/* questa init potrebbe avvenire anche dopo un passaggio per il pieno per cui fullPulseIn != 0*/

				LOR_timeLoad_Init();

				uP_LoadRestore_Data.init = FALSE;

			}

			if ( LOR_timeLoadEnd_Check() == S_OK )
			{ /* Esaurito tempo a disposizione per ripristino ->
			 Non potrò più caricare acqua neanche nei ripristini successivi, a meno che non
			 abbiano un dato di maxWater maggiore */

				  /* Aggiornamento acqua in vasca */
				WaterInDW = uP_LoadRestore_Data.maxWater;

				SETBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* set flag overflow ripristini */

				  /* incremento contatore KO Restore per statistiche
				  posso farlo qui perchè eseguo istruzione una volta soltanto dopo aver messo ad 1 il flag KO RESTORE
				  e poi cambio stato in LOAD_RESTORE_TRUE */
			#ifdef _STATISTICS_ON_
				MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.koRestore++;
			#endif

				uP_LoadRestore_Data.status = LOAD_RESTORE_TRUE;
			}
			/* else tempo di ripristino non esaurito */

			/* carico pilotato comunque per continuità */
			LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/
	   }
		else
		{/* pieno -> interrompo il Restore */

		  if ( uP_LoadRestore_Data.init == FALSE )
		  {/* è stata eseguita init in LOAD_RESTORE_EXE_TIME -> var inizializzate
			  inoltre init == FALSE per un'unica iterazione-> aggiornamento un'unica volta */

				/* aggiornamento acqua caricata in base al tempo trascorso */
				LOR_waterInDW_timeBased_Update();
				
				uP_LoadRestore_Data.init = TRUE;	 /* per eventuale restart con vuoto */
		  }

				/* Rimango in questo stato anche se consento avanzamento passo */

		  return S_OK;
		}

   }/*LOAD_RESTORE_EXE_TIME*/
   
   if (uP_LoadRestore_Data.status == LOAD_RESTORE_TRUE)
   {
	
		(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
	 
		return S_OK;

	}/*LOAD_RESTORE_TRUE*/
  
   return S_FALSE; 
  
}
/**/



/* ************** */
/* Local Function */
/* ************** */

/**
Gestisce l'inizializzazione del caricamento.
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 

 
\version		1.0.0
\date 			18/03/2008
\author			Valeria Catani

\note	in sospeso pressostato lineare

*/
static void LOR_init(void)
{
 
  GIWaterLoaded = 0;
  PulseLoaded = 0;
  uP_LoadRestore_Data.emptyPulseIn = 0;
 
  uP_LoadRestore_Data.waterInDWInit = WaterInDW; 
	         
  PM_PulseCounter_SetCount(uP_LoadRestore_Data.fullPulseIn);/* init pulse counter per nuovo conteggio */

}

/**
Inizializza up counter per caricamento a tempo

\param [in]		

\note: 
\version		1.0.0
\date 			27/08/2008
\author			Valeria Catani

*/
static void LOR_timeLoad_Init(void)
{
uint8_t loadingWater;	
	
	/* Calcolo tempo di caricamento residuo */
  	loadingWater =  ( uP_LoadRestore_Data.maxWater - WaterInDW );	  /* acqua residua che è possibile ripristinare */	
          
	uP_LoadRestore_Data.time = LOA_levTOtime_Conv(loadingWater); /* Calcolo tempo totale di ripristino */
		    		  	
	uP_LoadRestore_Data.timeAcc = 0;

	/* config + init UP counter per conteggio caricamento a tempo */

	(void)Set_SmallCounter((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)TOUT_MAX_COUNTER);

}
/**/


/**
Valuta se il target per il ripristino a tempo è stato raggiunto -


\param [in]		
\return 		S_OK se il target è stato raggiunto
				S_FALSE se il target non è ancora raggiunto
				 
\version		1.0.0
\date 			27/08/2008
\author			Valeria Catani

*/
static uint8_t LOR_timeLoadEnd_Check(void)
{
uint16_t timeElapsed_tmp;

	/* Caricamento in base alla portata (a tempo) */

	timeElapsed_tmp = (uP_LoadRestore_Data.timeAcc + (SWTC_TimeType)SWTC_GetElapsedTime((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER));

	if (timeElapsed_tmp >= uP_LoadRestore_Data.time)
	{
		/* End counting */

		(void)SWTC_Stop((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER);

		return S_OK;
	}
	else
	{
		if (SWTC_IsExpired((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER) )
		{
			/* Restart counter */

			/* Update timeAcc */
			uP_LoadRestore_Data.timeAcc += TOUT_MAX_COUNTER;

			/* Restart timer */

			(void)SWTC_SetSValue ((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER,(SWTC_SCounterValue)TOUT_MAX_COUNTER);

		}

		(void)SWTC_Run((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER);
	}

	return S_FALSE;
}
/**/


/**
Aggiorna la quantità di acqua in vasca in base alla conversione del tempo di caricamento 
appena applicato

\param [in]		
\return 		
								 
\version		1.0.0
\date 			28/08/2008
\author			Valeria Catani

*/

static void LOR_waterInDW_timeBased_Update(void)
{
uint16_t timeElapsed_tmp;
uint8_t loadedWater;

  timeElapsed_tmp = (uP_LoadRestore_Data.timeAcc + (SWTC_TimeType)SWTC_GetElapsedTime((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER));  

  loadedWater =  LOA_timeTOlev_Conv(timeElapsed_tmp); 
  
  WaterInDW += loadedWater;		 /* aggiornamento acqua in vasca con la quantità caricata
                                  in base al tempo trascorso */
  
  TempCycle_Stats.loaded_water += loadedWater; /*AE Statistic */
  cycle_loaded_water += GIWaterLoaded;
}
/**/

/* *********** */
/* End of file */
/* *********** */
