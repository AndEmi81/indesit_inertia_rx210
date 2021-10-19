/*
** **************************************************************************
** **************************************************************************
*/ 
/** 


Source File

\file    Load.c
\ingroup Load
\date    11/03/2008
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

/**DEBUG*/
/*#define PUMP_CTRL_UP_RUNNING_MSK     1*/  /*E' nel modulo Pump Control*/
/**DEBUG END*/
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void LOA_levTOtimeRef_Update(void);
static void LOA_timeLoad_Init(void);
static uint8_t LOA_timeLoadEnd_Check(void);
static void LOA_waterInDW_defUpdate(void);
static void LOA_disableLoadCounters(void);

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
Gestisce l'esecuzione del microprogramma Load .
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma è verificata (caricamento acqua concluso),
\return S_FALSE se non è ancora verificata(caricamento acqua in corso),
\return codice di errore in altri casi
 
 
\version		1.0.0
\date 			11/03/2008
\author			Valeria Catani

\note	in sospeso pressostato lineare

*/
uint8_t LOA_uP_Load_Exe (uint8_t HighStatement,uint8_t LowStatement)
{

uint8_t loadingWater;

uint8_t res;
int8_t variation;
int16_t delta;
uint8_t loadMode;
bool_t isFullRequested;

	
	if (Interpreter_Data[0].stepInitialized == FALSE )
	{
		/* Prima iterazione del passo */

		uP_Load_Data.status = LOAD_INIT;
		uP_Load_Data.VS_check=FALSE;
	
	}
	if(Interpreter_Data[0].optionalStepInitialized == FALSE)
	{
		uP_Load_Data.VS_check=FALSE;
	}

	/* passaggio per uP Load */
	FltLoadToutData.uP_exe = TRUE;
	
	/*Analisi modalità di caricamento richiesta */
	loadMode = HighStatement & (F1|F0);
	
	if (( loadMode== LOA_PS_CHECK_MODE) && ( HWC_Flags.bit.vs_wash_presence == 0))
	{/* non c'è il sensore virtuale*/
		uP_Load_Data.status = LOAD_TRUE;
		uP_Load_Data.pump_act=FALSE;
		return S_OK;	
	}
	
	if ( (uPDoorStatus == UP_DOOR_WAIT)||
	((loadMode== LOA_PS_CHECK_MODE)&&(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==5)&& uP_Load_Data.VS_check ))
	{/* porta aperta o in attesa di timeout dopo la chiusura o delay MR -> il uP va sospeso */
	 /* sospensione anche con lo stato 5 del sensore virtuale che non ferma la pompa lavaggio ma solo l'eventuale caricamento */
		
		LOA_disableLoadCounters();  /*disabilitazione contatori*/
		(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
		(void)SWTC_Stop((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);

		uP_Load_Data.init = TRUE;
		(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)( OFFS_B(uPLoad_setting_data.Data,5)&0x7F));				
		return S_FALSE;
	}


	

	
	isFullRequested = ( ( loadMode == LOA_PP_CHECK_MODE )? TRUE : FALSE );

	/* ********************************************************* */
	/* Aggiornamento stato pressostato per l'iterazione in corso */
	/* ********************************************************* */

	/* Test validità stato criteri - solo in caso di calcolo portata o caricamento a PP, condizioni in cui serve
	conoscere lo stato del pressostato */			 
	                                       

	if (( (Flag_levTOtimeRef_update == TRUE) || (uP_Load_Data.status == LOAD_EXE_PP) )&& ( loadMode!=LOA_PS_CHECK_MODE ) )
	{		
	  	if ( (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) == 0) || (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_CR_OVFL_IND) ) == 0 ))
	  	{
	  		/*
	  		Uno dei due criteri è in uno stato non valido o non è presente:
	  		stato pressostato indeterminato
	  		*/

	  		LOA_disableLoadCounters();  /*disabilitazione contatori*/
  		
	  		/* Non eseguo il uP, ma abilito avanzamento passo */
	  		return S_OK;
	  	}
	}

	/* Stato valido criteri pressostato */

	/* Criterio Overflow */
	 
	if ( TESTBIT (GICriteria,GI_CR_OVFL_IND) != 0 )
	{
		/* Overflow: interrompe caricamento ma non abilita avanzamento passo */
		
		LOA_disableLoadCounters();  /*disabilitazione contatori*/
		
		return S_FALSE;
	}
	
	
/* ******************************** */
/* Inizializzazione dati da setting */
/* ******************************** */
	
	if (uP_Load_Data.status == LOAD_INIT)
	{
		CLRBIT(Flags_1, JIF_FLAG1_UP_TERMINATED_KO_POS);
		
		/* default : disabilitazione timeout di caricamento */
		FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;

		/* QI - 26.10.09 - init timer per F06 - sub 1 e F05 - sub 3 : (tout caricamento fino a P.P. in caso di turbina KO o funzionante rispettivamente) */

		if ( isFullRequested  )
		{ /* richiesto controllo su pieno pressostato per eventuale F06 : 
			configurazione timer per timeout caricamento */

			FltLoadToutData.state = FLT_LOAD_TOUT_INIT;
		}
		/* END QI - 26.10.09 */

		/* nuovo caricamento: reset var per F05 */
		CLRBIT(FltFlagsKO,WARNING_LOADPP_KO);

		if ( TESTBIT(Flags_1, JIF_FLAG1_REGENERATE_OK_POS)!=0 )
		{/* è stata completata l'operazione di rigenera -> devo resettare la variabile litri caricati*/

			clearRegWaterLoaded();

			CLRBIT(Flags_1, JIF_FLAG1_REGENERATE_OK_POS);  
		}


		/* Inizializzazione timer per calcolo tempo raggiungimento p.p. (portata) */

		/*if (Flag_levTOtimeRef_update == TRUE)*/		 /* IL FLAG VIENE SETTATO DAL UP ATTIVA FUNZIONE CHE DEVE STARE PRIMA DI LOAD!! */
		/* la presenza del uP Attiva funzione garantisce anche il vuoto */
		                  
		/* Config+init timer calcolo portata, senza test su Flag che ancora non � impostato in questa iterazione */
		(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)0x7F);

		/* Caricamento dati dalla flash */
		if( BasicLoader (PTR_UP_LOAD, LowStatement, &uPLoad_setting_data) != S_OK )
		{
			/* Non eseguo il uP, ma abilito avanzamento passo */
			return S_OK;
		}

		loadingWater =  OFFS_B(uPLoad_setting_data.Data,0);

		/* Gestione modificatori */

		res = GetModifierValue (OFFS_B(uPLoad_setting_data.Data,1), &variation);

		if( ( MOD_DONT_CARE == res ) || ( S_FALSE == res ) )
		{/* Se non è attivo nessun modificatore la variazione viene posta a zero */

			variation=0;
		}

		if( variation )
		{/* variazione != 0 va applicata*/

			/* calcolo delta */                      
			delta = GetDeltaFromVariation(loadingWater,variation);

			/* aggiornamento */
			loadingWater += (uint8_t)delta;         
		}

		/* End Gestione modificatori */    

		uP_Load_Data.tgWater = ( loadingWater + WaterInDW );

		/* Init variabili per caricamento a turbina */

		/*uP_Load_Data.waterInDWInit = WaterInDW;*/ /* questa var è utile soprattutto per Debug*/
		/*PulseCounter_SetCount(0);*/               /* init pulse counter per nuovo conteggio */


		PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);          /* Reset di eventuali warning presenti */


		(void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT); /* init funzione controllo turbina bloccata */

		CLRBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);
		
		if(loadMode == LOA_PS_CHECK_MODE )
		{
			if ( OFFS_B(uPLoad_setting_data.Data, 2)==0)
			{/* EA problema rigenere: se la velocità  è 0 non fa il controllo */
			/* EA reinizializza conteggio in caso di rigenere*/

				FltLoadToutData.state = FLT_LOAD_TOUT_INIT;

				SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_INIT);

				uP_Load_Data.status = LOAD_EXE;	
			}else
			{
				uP_Load_Data.VS_check=FALSE;	
				/* se non ho ancora eseguito l'accensione della pompa */
				uP_Load_Data.status = LOAD_WAIT_PUMP;	
			}	
		}else
		{
			uP_Load_Data.status = LOAD_EXE;	
		}
		uP_Load_Data.init = TRUE;		              /* per init della variabile waterInDWInit */
		if (HWC_Flags.bit.vs_wash_presence )		
		{
			CLRBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* reset flag overflow ripristini */	
		}
		/* Nella prima iterazione non vengono pilotati i carichi */
		return S_FALSE;
	}
	/*LOAD_INIT*/

	/* Gestione timer per calcolo portata successiva a LOAD_INIT */
	if ((Flag_levTOtimeRef_update == TRUE)&& ( loadMode!=LOA_PS_CHECK_MODE ))
	{/* Richiesto calcolo portata */

		if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )
		{/* pieno pressostato -> disabilito counter */

			(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER);
		}
		else
		{/* vuoto pressostato -> abilito counter */
			(void)SWTC_Run((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER);
		}
	}
	
	/* QI - 26.10.09 - disabilitazione tout statico durante caricamento dinamico 
	(non ci fidiamo del flag IsFullRequested per identificazione dei caricamenti statici)  */
	
	/* QI 05.11.09 gestione timeout caricamento dinamico */

	if ( TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) != 0 )
	{/* è in esecuzione il uP Pump Control -> si tratta di un caricamento dinamico */
	    /* il timeout relativo al P.P. in caricamento statico viene disabilitato */
  		if ( loadMode!=LOA_PS_CHECK_MODE )
		{
    		FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;		
		}

		 /* QI 05.11.09 gestione timeout caricamento dinamico: vale in Load + Pump Control e in caricamento a P.P. dinamico  */
  		SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_EXE);

  	}

	else
	{/* QI 05.11.09 gestione timeout caricamento dinamico : init al P.P. in statico : vale in EXE e in TIME */
	/* con i sensori virtuali questo non conta */
		if ( loadMode!=LOA_PS_CHECK_MODE )
		{
			SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_INIT);
		}else
		{ /* modifica brevetti: IL timeout di apertura dell'elettriovalvola diventa comunue al caricamento statico e dinamico.*/
			SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_EXE);
				
		}	
	}

	if ( TESTBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS) )
	{
		/* in analogia con la dichiarazione del KO Restore per WaterInDW > maxWater in Load Restore
		non si prosegue nel caricamento e viene abilitato avanzamento passo 
		Il Jump su KO Restore verrà processato nel passo successivo 
		se Load Restore è l'unico uP del passo*/	 

		uP_Load_Data.status = LOAD_TRUE;
	}
	
	
/* *************** */
/* Esecuzione uP   */
/* *************** */
#if (VIRTUAL_SENSOR_ENABLE == 1)
	if (uP_Load_Data.status == LOAD_WAIT_PUMP)
	{
		if ( !uP_Load_Data.VS_check)
		{
			(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)( OFFS_B(uPLoad_setting_data.Data,5)&0x7F));				
			uP_Load_Data.VS_check=TRUE;
		}	
		if ( SWTC_IsExpired((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION))  
		{/* conteggio concluso */
			if ( TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) == 0 )
			{/* si tratta di caricamento statico */
				uP_Load_Data.pump_act=FALSE;
				/* ad ogni caricamento statico azzero questa variabile
				si presuppone che prima di ogni caricamento dinamico ci sia uno statico */
				uP_Load_Data.turbina_KO=FALSE;
				uP_Load_Data.status = LOAD_EXE;
		
				/* EA modifica brevetto_ il calcolo del fault f06 deve essere fatto sia in statico che in dinamico e viene resettato
				solo nello statico , cioè ad ogni fase di lavaggio */

				SFT_Timeout_4Loadrestore_Mgmt(FLT_PUMP_LOAD_INIT);

				if ( (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=0) || (!TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))))
				{
					/* corrisponde ad una prima fase di caricamento statico, se non siamo in vuoto sensore il uP termina*/
					    if (TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH)))
						{
							SETBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS);
#ifdef _STATISTICS_ON_
						
							IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].load_ko);
#endif	
							
						}	
						uP_Load_Data.status = LOAD_TRUE;
						uP_Load_Data.pump_act=FALSE;
				}		
			}else
			{
				uP_Load_Data.pump_act=TRUE;	
				/* sono in grado di determinare il valore del sensore con acqua in vasca
				al termine del caricamento statico invece non attivo + la pompa*/
				
				/* EA : cambuara gestione dell'accension della spia filtro otturato. impostato ad ogni riconoscimento di incongruenza e 
				resettato ad ogni inizio del caricamento dinamico*/
				CLRBIT(Flags_2, FLAG2_CLOGGED_FILTER);			
			
				if ( uP_Load_Data.turbina_KO)
				{
					/*uP_Load_Data.pump_act=FALSE;*/ /* aumento velocità , tramite pump control*/
					uP_Load_Data.status = LOAD_PS_DINAMIC;	
					(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue) (OFFS_B(uPLoad_setting_data.Data,4)&0x7F));	        
					 (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
					 
					 
					if (((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==2) || (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==1))&&(TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))))
					{/* vedo il pieno */
						uP_Load_Data.pump_act=FALSE;	
					}
					 
					 
				}else
				{
					uP_Load_Data.status = LOAD_EXE;
				}
				uP_Load_Data.init = TRUE;	
					
			}
			(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION); 
			 FltLoadToutData.state = FLT_LOAD_TOUT_INIT;
			 uP_Load_Data.time= OFFS_B(Tab_HWCVS_data.Data, 1)& 0x7F;
			 LOA_timeLoad_Init();  /*Inizializzazione caricamento a tempo*/

		}else
		{	
			uP_Load_Data.pump_act=TRUE;
			(void)SWTC_Run((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION);
			
		} 
		return S_FALSE;
	}

#endif /*#if (VIRTUAL_SENSOR_ENABLE == 1)*/

	if (uP_Load_Data.status == LOAD_EXE) 
	{ /* esecuzione caricamento a turbina */
   		
		if ( loadMode==LOA_PS_CHECK_MODE )
		{
			if (TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) == 0 )
			{/* si tratta di caricamento statico */
				(void)SWTC_Run((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER);
				/* il contatore di timeout elettrovavola parte lo stesso, così, se la turbina funziona non 
				viene analizzato , ma se la turbina non funziona non carica più dei dl impostati da setting 
				corrispondenti al timeout 
				considerare anche le tolleranze*/
			}else
			{ 
				/* se la turbina funziona in dinamico si carica la quantità d'acqua voluta a velocità ridotta*/
				uP_Load_Data.pump_act=TRUE;
				if ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0)&&(TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))) )
				{/* vedo il vuoto*/
						
					SETBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS);
#ifdef _STATISTICS_ON_			
					IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].load_ko);
#endif	
			
					uP_Load_Data.status = LOAD_TRUE;
					uP_Load_Data.pump_act=FALSE;
					return S_FALSE;			
				}
			}
		}			
	
	    if ( (PM_PulseCounter_GetWarning() == NO_PULSE_COUNTER_WARNING) && (TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) == 0) )
	    {/* Contatore impulsi turbina OK && NO turbina bloccata */
        
	        /* controllo turbina bloccata */
	        if ( FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_EXE) == TRUE )
	        {
				SETBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS); 
				/* EA modifica per brevetto. al prefault la visual deve segnalare mancanza acqua */
				if (HWC_Flags.bit.vs_wash_presence)
				{
					FltWarning.load=TRUE;
				}
				/* salva nella pila dei last prefault*/
				FLT_warningSave(FLT_CODE_PULSE_COUNTER, FLT_SUBCODE_PULSE_COUNTER, WRN_PREFAULT);

				return S_FALSE; /*alla prox iterazione entra con bit = 1 e cambia modalit� di caricamento*/
         
	        }
        
	        if (uP_Load_Data.init == TRUE)
	        {
	          uP_Load_Data.waterInDWInit = WaterInDW; 
          
	          WaterLoadedReset();
          
	          /* QI 27.10.09 init finestra di buio su controllo turbina. Messo qui per reinizializzare contatore al POR */

	         (void)SWTC_SetSValue((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER, (SWTC_SCounterValue)TOUT_PULSE_COUNT_CHECK_DELAY);

	          uP_Load_Data.init = FALSE;
            
	        }
        
	        /* QI 27.10.09 gestione finestra di buio su check turbina */
        
	        FLT_PulseCounterCheckDelay_Mgmt();
         	
	        WaterInDW = ( uP_Load_Data.waterInDWInit + GIWaterLoaded );		 /* aggiornamento acqua in vasca con la quantit� caricata */
        
			if ( WaterInDW >= uP_Load_Data.tgWater )
			{/* caricamento a turbina concluso - non piloto EV */

				TempCycle_Stats.loaded_water += WaterInDW;		/* Aggiornamento statistica acqua caricata*/
				cycle_loaded_water += GIWaterLoaded;
				/* Aggiornamento portata storica - se richiesto */
				LOA_levTOtimeRef_Update(); 

				(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);

				/* Caricamento a turbina concluso : 
				controllo sul pieno pressostato per F05 se richiesto dal setting */

				if ( isFullRequested )
				{ /* richiesto controllo su pieno pressostato per eventuale F05 */

					if (TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) == 0)
					{/* carico statico (verifica, ma dovrebbe essere implicito nel flag) */

						FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;		 /* disabilito Tout F05 - sub 3 perch� � stato raggiunto il target */
/* da modificare.*/
						if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
						{/* vuoto -> anomalia pressostato : condizione per F05 - sub 1 */
  		      
							SETBIT(FltFlagsKO,WARNING_LOADPP_KO);
      		         
      		    		}
      		  		}
      			}
 
          		uP_Load_Data.status = LOAD_TRUE;
        	}
			else
			{/* else caricamento non concluso */

				
			}
			/* carico pilotato comunque per continuità */
	        LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/ 
	   	} 
		else
		{/* Warning Pulse Counter: il conteggio dell'acqua in ingresso non è affidabile 
        o Turbina bloccata: non leggo impulsi */
      		
	   		if (( loadMode==LOA_PS_CHECK_MODE )&& (TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS)))
			{
				/* modifica per brevetto : in caso di turbina ko ( NO IMPULSI)  rimane in questo stato fino a fault o a ripristino turbina
				Se la turbina è fuori range, va avanti comunque sensore o a tempo */
				uP_Load_Data.init = TRUE;
				LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/ 
				return S_FALSE;		
			}
			if ( TESTBIT (uP_PumpCtrl_Data.flags,PUMP_CTRL_UP_RUNNING_BIT) != 0 )
			{/* è in esecuzione il uP Pump Control -> si tratta di un caricamento dinamico */
			/* il caricamento alternativo alla turbina è a pressostato */
				if ( loadMode==LOA_PS_CHECK_MODE )
				{/* EA qui non ci dovrei mai andare perchè l'ho discriminato prima */
					(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue) (OFFS_B(uPLoad_setting_data.Data,4)&0x7F));	        
					 (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
					uP_Load_Data.status = LOAD_PS_DINAMIC;
				}else
				{	
			      /* config + init Down counter per tempo stabilità pressostato in pieno */
				   (void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)TOUT_FULL_STABILITY);	        

			   		uP_Load_Data.status = LOAD_EXE_PP; 		
				}
			  /*posso proseguire con LOAD_EXE_PP senza effettuare return */
			}
			else
       		{/* NON è in esecuzione il uP Pump Control -> si tratta di un caricamento statico */
	        	/* il caricamento alternativo alla turbina è a tempo, in base alla portata storica */
       
				loadingWater =  (uP_Load_Data.tgWater - WaterInDW);	 /* cos� si tiene conto dell'acqua caricata fino a questo momento */
          
	          /* Calcolo tempo di caricamento residuo */
				if (  loadMode!=LOA_PS_CHECK_MODE )
				{
		          /*loadingWater =  OFFS_B(uPLoad_setting_data.Data,0);*/		/*var locale utile per debug*/
     
				    uP_Load_Data.time = LOA_levTOtime_Conv(loadingWater); /* Calcolo tempo totale di caricamento */	
					LOA_timeLoad_Init();  /*Inizializzazione caricamento a tempo*/	  
					
				}		  	  
	            uP_Load_Data.status = LOAD_EXE_TIME;
          
         		 /*posso proseguire con LOAD_EXE_TIME senza effettuare return*/
        
       		} 
    	}
 	
	}/* LOAD_EXE */
  
	if (uP_Load_Data.status == LOAD_EXE_PP)
	{/*caricamento a pressostato (dinamico)*/

  		
    	if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )
    	{/* pieno pressostato */
    	  
    	  (void)SWTC_Run((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
		  
    	  if ( SWTC_IsExpired((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER) )
    	  { /* caricamento a P.P. concluso (end conteggio per stabilità pressostato in pieno) */
    	   
				(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);

				/* Aggiornamento portata storica - se richiesto */
				LOA_levTOtimeRef_Update(); 

				  /* Aggiornamento acqua in vasca */
				LOA_waterInDW_defUpdate();
	
				/* disabilitazione timeout di caricamento */  /*NON SERVE: Se sono qui nessuno lo ha abilitato*/
		       /*FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;*/
  	    
    	    	uP_Load_Data.status = LOAD_TRUE;  
    	    
    	  }
    	  
    	}
    	else
    	{/* vuoto - continuo caricamento */
    	 
    	 /* re-inizializzo counter per tempo stabilità pressostato in pieno */
		 
		  (void) SWTC_SetSValue ((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER, (SWTC_SCounterValue)TOUT_FULL_STABILITY);
    	 
    	}
		
		/* carico pilotato comunque per continuità */
		LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/
		
  }
  
	if (uP_Load_Data.status == LOAD_EXE_TIME)
	{/* caricamento a tempo (statico)*/
	    if ( LOA_timeLoadEnd_Check() == S_OK )
	    { /* caricamento a tempo concluso */
			uP_Load_Data.turbina_KO=TRUE;
			/* 11.02.10 - controllo sul pieno richiesto */
			if (isFullRequested)
			/* EA, variabile mai impostata a 1 se ci sono i sensori virtuali*/
			{/* pieno richiesto */
			
				if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )	
				{/* pieno raggiunto */
					/* Aggiornamento portata storica - se richiesto */
        			LOA_levTOtimeRef_Update(); 

  					/* Aggiornamento acqua in vasca */
  					LOA_waterInDW_defUpdate();
  			      
        			uP_Load_Data.status = LOAD_TRUE;			
					
				}
				else if (FltLoadToutData.state == FLT_LOAD_TOUT_END)
				{/* pieno richiesto e non raggiunto - esaurito timeout di caricamento 
						(in realtà non dovrebbe verificarsi perchè vado in fault): termina uP e non piloto EV */
        
        			uP_Load_Data.status = LOAD_TRUE;
        
        			return S_OK; 	
				}
				else
				{
					/* Do nothing */
				}
			}
			else
			{/* pieno non richiesto */
				
				/* Aggiornamento portata storica - se richiesto */
        		LOA_levTOtimeRef_Update(); 

  				/* Aggiornamento acqua in vasca */
  				LOA_waterInDW_defUpdate();
  			      
        		uP_Load_Data.status = LOAD_TRUE;				
			}
			
		}
		/* else caricamento a tempo non concluso -> pilotaggio EV*/
	
			/* QI 29.01.10 ELIMINATO */
			/*QI 07.01.10 - quando è in esecuzione il uP Load è attivo il Tout di caricamento (se non scaduto)
			ribadito lo stato di EXE per assicurare la ripresa dopo PAUSE  */

		/* carico pilotato comunque per continuità */
		LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/

	}
  
  
 #if (VIRTUAL_SENSOR_ENABLE == 1) 
	if (uP_Load_Data.status == LOAD_PS_DINAMIC)
	{
		/*uP_Load_Data.pump_act=FALSE;*/
		/* la velocità qui dovrebbe essere quella di lavaggio*/
		/*EA: rivedere la logica dell'ativazione del motore*/
		
		
		if (TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH)) )
		{/* se lo stato del sensore non è valido aspetta che torni tale*/
			if ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0) )
			{/* vedo il vuoto*/
				if ( !uP_Load_Data.pump_act)
				{/* esce dal uP */	
					SETBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS);
#ifdef _STATISTICS_ON_
						
					IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].load_ko);
#endif	

				  	/* Aggiornamento acqua in vasca */
					LOA_waterInDW_defUpdate();
	
					uP_Load_Data.status = LOAD_TRUE;
					return S_FALSE;
					
				}
			}
			if ((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==2) || (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==1))
			{/* vedo il pieno */
				if ( uP_Load_Data.pump_act)
				{
					uP_Load_Data.pump_act=FALSE;
				}else
				{				
					(void)SWTC_Run((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
				}	
			}
			if ( SWTC_IsExpired((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER) )
			{ /* caricamento a P.P. concluso (end conteggio per stabilità pressostato in pieno) */

				(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
				/* Aggiornamento acqua in vasca */
				LOA_waterInDW_defUpdate();
	
				uP_Load_Data.status = LOAD_TRUE;
			}
			
			/* carico pilotato comunque per continuità */
			LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON; /* pilotaggio EV*/
		}else
		{
			 (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
		 }	 
			
		
	}	
#endif /*#if (VIRTUAL_SENSOR_ENABLE == 1)*/			
  if (uP_Load_Data.status == LOAD_TRUE)
  {    
    /*
		Lo stato LOAD_TRUE corrisponde a return S_OK
		(OK avanzamento passo) e impedisce di riprendere il caricamento
		*/
		Flag_levTOtimeRef_update = FALSE;
		
		LOA_disableLoadCounters();  /* disabilitazione contatori */
		
		/*AE TODO Prendere temperatura APP tramite gitemp*/
		WaterTemp_At_CycleStart = GITemp;


		return S_OK;
    
  } /* LOAD_TRUE */

  return S_FALSE;


}
/**/



/* ************** */
/* Local Function */
/* ************** */


/**
Gestisce aggiornamento fattore di conversione tra livello acqua e tempo ("portata") .
 
\note :	Aggiorna direttamente variabili globali. viene chiamata solo una volta in un caso specifico
		Fondamentale inizializzare (WM_Hound?) la var globale uP_Load_Data.levTOtimeRef al LEV_TO_TIME_MIN
 
\version		1.0.0
\date 			12/03/2008
\author			Valeria Catani

*/

static void LOA_levTOtimeRef_Update(void)
{
uint8_t levTOtime_tmp;

	if (Flag_levTOtimeRef_update == TRUE)	
	{
	  
	  	if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )
	  	{/* pieno pressostato */
  	     	(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER);

    		levTOtime_tmp = ( (SWTC_TimeType)SWTC_GetElapsedTime((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER));
		
      		/* QI - 13.10.09 */
      
			if (levTOtime_tmp < LEV_TO_TIME_MIN )
			{/* valore inferiore al minimo -> uso il minimo */

				levTOtime_tmp = LEV_TO_TIME_MIN;
			}
			else if (levTOtime_tmp > LEV_TO_TIME_MAX) 
			{/* valore superiore al max -> uso il max */

				levTOtime_tmp = LEV_TO_TIME_MAX;   
			}
			else
			{
				/* Do nothing */
			}

	    	/* aggiornamento portata storica con il nuovo valore calcolato (o con min/max se out of range)*/
	    	uP_Load_Data.levTOtimeRef = ( (((uint16_t)uP_Load_Data.levTOtimeRef*7) + levTOtime_tmp)/8 );
    	
    	
  		}
  	
  		Flag_levTOtimeRef_update = FALSE;
  	
	}

}
/**/

/**
Gestisce conversione da livello d'acqua in dl a tempo in secondi in base alla portata

\param [in]		level : livello acqua in dl
\return 		tempo in secondi corrispondente ad un caricamento da vuoto a pieno pressostato
				(dato aggiornato o default)
 
\version		1.0.0
\date 			12/03/2008
\author			Valeria Catani
				
*/
uint16_t LOA_levTOtime_Conv(uint8_t level)
{
	
	/* EA mettere qui l'impostazione del tempo in fuznione del nuovo parametro in Editor*/
	
	return ((uint16_t)(((uint32_t)level * uP_Load_Data.levTOtimeRef)/(FULL_REF_LEVEL)));
}
/**/

/**
Gestisce conversione da tempo in secondi a livello d'acqua in dl in base alla portata

\param [in]	  tempo in secondi corrispondente ad un caricamento da vuoto a pieno pressostato
				(dato aggiornato o default)

\return       level : livello acqua in dl	
 
\version		1.0.0
\date 			28/08/2008
\author			Valeria Catani
				
*/
uint8_t LOA_timeTOlev_Conv(uint16_t time)
{
	return ((uint8_t)(((uint32_t)time * (FULL_REF_LEVEL)) / uP_Load_Data.levTOtimeRef));
}
/**/



/**
Inizializza up counter per caricamento a tempo

\param [in]		

\note: devo utilizzare un contatore diverso (timeLoad_counter) da quello 
\       del cacolo portata per la modalità con cui l'aggiornamento della portata viene gestita
\version		1.0.0
\date 			15/02/2006
\author			Valeria Catani

*/
static void LOA_timeLoad_Init(void)
{
	uP_Load_Data.timeAcc = 0;

	/* config + init UP counter per conteggio caricamento a tempo */
	(void)Set_SmallCounter((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)TOUT_MAX_COUNTER);

}
/**/


/**
Valuta se il target per il caricamento a tempo è  stato raggiunto

\param [in]		
\return 		S_OK se il target � stato raggiunto
				S_FALSE se il target non � ancora raggiunto
				 
\version		1.0.0
\date 			14/03/2008
\author			Valeria Catani

*/

static uint8_t LOA_timeLoadEnd_Check(void)
{
uint16_t timeElapsed_tmp;

	/* Caricamento in base alla portata (a tempo) */

	timeElapsed_tmp = (uP_Load_Data.timeAcc + (SWTC_TimeType)SWTC_GetElapsedTime((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER) );

	if (timeElapsed_tmp >= uP_Load_Data.time)
	{
		/* End counting */

		(void)SWTC_Stop((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER);

		return S_OK;
	}
	else
	{
		if ( SWTC_IsExpired((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER) )
		{
			/* Restart counter */

			/* Update timeAcc */
			uP_Load_Data.timeAcc += TOUT_MAX_COUNTER;

			/* Restart timer */

			(void)SWTC_SetSValue ((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER,(SWTC_SCounterValue)TOUT_MAX_COUNTER);
		}
 		(void)SWTC_Run((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER);
		
	}

	return S_FALSE;
}
/**/

/**
Aggiorna la quantità di acqua in vasca con la quantità che è stato
richiesto di caricare nel dato del uP.
Viene fatto anche in caso di caricamento a pressostato perch�, pur commettendo un errore,
è la scelta più prudente in caso di un ripristino successivo.

\param [in]		
\return 		
								 
\version		1.0.0
\date 			17/03/2008
\author			Valeria Catani

*/


static void LOA_waterInDW_defUpdate(void)
{

/* Aggiornamento acqua in vasca con la quantità che avrei dovuto caricare a turbina */

	 WaterInDW =  uP_Load_Data.tgWater; 			/* QI - 13.10.09*/
	 TempCycle_Stats.loaded_water += WaterInDW;
	 cycle_loaded_water += GIWaterLoaded;
  
}
/**/

/**
Disabilita i contatori utilizzati nel uP
\param [in]		
\return 		
								 
\version		1.0.0
\date 			03/04/2008
\author			Valeria Catani

*/
static void LOA_disableLoadCounters(void)
{
   (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER);
   (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
   (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER);
   
   (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_PUMP_ACTIVATION); 
#if 0 /* EA fault */
   (void)SWTC_Stop((SWTC_CountersType)MP_PUMP_LOAD_M_COUNTER);
#endif
}
/**/

/* *********** */
/* End of file */
/* *********** */
