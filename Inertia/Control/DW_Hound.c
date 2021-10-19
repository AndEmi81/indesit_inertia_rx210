/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup     CK_Hound.c	  

\Par	
\Header file.
 
\ingroup      CK_Hound.c

\file    DW_Hound.c
\ingroup DW_Hound
\date    31/01/2008
\author  Valeria Catani
\version 1.0.0

\Par		History (titolo)
 		
*/


/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "PM_TurbiditySensor.h"
#include "PM_Alternato.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

static void HND_defaultInterpreterInit(uint8_t programmingCyclePosition);
static void HND_defaultVarInit(void);
static void HND_initFunctionValue(uint8_t funcID, uint8_t *funcValue);

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
Effettua inizializzazioni e configurazioni sulla base del setting file di variabili da utilizzare
nella gestione a regime.

La funzione viene chiamata al Reset freddo, in Setup nella routine 
CBF_NormalMode_InitTask_ColdReset_PreRestore
 
\return 		S_OK se non ci sono errori di caricamento o nel contenuto dei dati, un codice 
				d'errore altrimenti.
 
\version		1.0.0
\date 			31/01/2008
\author
*/
uint8_t HND_dwConfigInit(void)
{
	Loader_Struct HND_App_Load;
	/* init puntatore Output Map */
	
	if( BasicLoader( APP_PTR_IO_CONFIG_DATA, DSPL_IO_OUTPUT_MAP, &Tab_OutputData ) != S_OK )
	{
		return HND_ERR_OUTPUT_DATA; 	
	}
		
	/* init puntatore GI Map */
	if( BasicLoader( APP_PTR_IO_CONFIG_DATA, DSPL_IO_GENERIC_INPUT_MAP, &Tab_GIData ) != S_OK )
	{
		return HND_ERR_GI_DATA; 	
	}
	
	
	/**init puntatore hw config Regenerate Data */
	if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_REGENERATE_DATA, &Tab_HWCRegenerateData ) != S_OK )
	{
		return HND_ERR_HW_CONF_DATA;
	}

	/**init puntatore hw config Flags */          
	if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_FLAGS_DATA, &HND_App_Load ) != S_OK )
	{
		return HND_ERR_HW_CONF_DATA;
	}
		 
	HWC_Flags = (*(HWC_Flags_Struct*)HND_App_Load.Data);
	
		/* Funzione di inizializzazione variabili con posizione carichi*/
	if ( HIO_ldPosInit() != S_OK)
	{
		return HND_ERR_OUTPUT_DATA;
	}
	
	if ( HWC_Flags.bit.vs_wash_presence==0)
	{
		/**init puntatore hw config Press Sensor Data */
		if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_PRESS_SENSOR_DATA, &Tab_HWCPressSensorData ) != S_OK )
		{
			return HND_ERR_HW_CONF_DATA;
		}
	}
#if (VIRTUAL_SENSOR_ENABLE == 1)
	else
	{
		if( BasicLoader (PTR_HW_CONFIG_DATA, DSPL_HWC_VS_DATA, &Tab_HWCVS_data) != S_OK )
		{
			return HND_ERR_HW_CONF_DATA;
		}
 	
	}
#endif
	/* Init torbidità */
	if(HWC_Flags.bit.turbsens)
	{    
		if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_TURBIDITY_DATA, &Tab_HWCTurbidity ) != S_OK )
		{
			return HND_ERR_HW_CONF_DATA;
		}
	}

    if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_SALT_DATA, &HND_App_Load ) == S_OK )
	{
		Salt_Filtro_Stab =  OFFS_B(HND_App_Load.Data,1);
		/* se non c'è è 0, quindi non applica nessun filtro*/
	}
	
	if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_DRYER_DATA, &Tab_HWCSPR_TimeBase ) != S_OK )
	{
		return HND_ERR_HW_CONF_DATA;
	} 

    /* Init alternato : pos undef  */
    HIO_SetAltPosition(PM_ALT_getPosNotDef());


	/**init puntatore numero di funzioni */
	if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_FUNC_NUM, &Tab_HWCFuncNum) != S_OK )
	{
		return HND_ERR_HWC_FUNC_NUM; 	
	}

	if( (FUNC_NUM_TO_MAIN > NUM_REG_MAX) || (FUNC_NUM_ALL > NUM_REG_MAX) )
	{
		return HND_ERR_OVFL_FUNC_NUM; 	
	}
		
	/* init puntatore numero cicli */
	if( BasicLoader( PTR_HW_CONFIG_DATA, DSPL_HWC_CYCLES_NUM, &Tab_HWCCyclesNum) != S_OK )
	{
		return HND_ERR_CYCLES_DATA; 	
	}

	if(CYCLES_NUM > N_MAX_CYCLES_PC)
	{
		return HND_ERR_OVFL_CYCLES_NUM; 	
	}

  /* init buffer ID funzioni */
	REG_getBufFunctionsIDs();


   /* init puntatore Safety Door Parameters */
	if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_DOOR_LOAD_ON, &Tab_SFT_Door ) != S_OK )
	{
		return HND_ERR_SAFETY_DATA; 	
	}

  /* init puntatore Safety Resistor Parameters */
	
	if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_PRESS_RES, &Tab_SFT_PressRes ) != S_OK )
	{
		return HND_ERR_SAFETY_DATA; 	
	}

	
	/* init puntatore Safety EV Parameters */
	if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_EV, &Tab_SFT_EV ) != S_OK )
	{
		return HND_ERR_SAFETY_DATA; 	
	}
	


	if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_DRY, &HND_App_Load ) != S_OK )
	{
		return HND_ERR_SAFETY_DATA; 	
	}

	Dry_SfyTime_Res_On = (OFFS_B(HND_App_Load.Data,0) & 0x7F);
	Surge_Fan_Params.fan_startup_speed = OFFS_B(HND_App_Load.Data,1) & SPEED_MASK;
	Surge_Fan_Params.fan_startup_timer = OFFS_B(HND_App_Load.Data,2) & TIME_STARTUP_MASK;	

	if( HWC_Flags.bit.vs_wash_presence != 0)
	{
		if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_MOTOR_WASH, &Tab_SFT_MotorWash ) != S_OK )
		{
			return HND_ERR_SAFETY_DATA; 	
		}
	}

	if (HWC_Flags.bit.bldc_drain != 0)
	{
		if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_MOTOR_PARAMS_DRAINPUMP_, &Tab_SFT_MotorDrain ) != S_OK )
		{
			return HND_ERR_SAFETY_DATA;
		}
	}


#if	(SBY_STANDBY_ENABLE == 1)
  	/* init puntatore Safety Standby Parameters */
  	if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_STANDBY, &Tab_SFTStandByData ) != S_OK )
  	{
  		Tab_SFTStandByData.Data = NULL;
  	}


#endif


	if( ( ( HWC_Flags.bit.bldc1 != 0 ) || ( HWC_Flags.bit.bldc_drain != 0 ) )
		&&
 	 	( RegDev_Pos[LD_KILL_DSP_REG_IND] != NO_LD_POS )
 	  )
	{
		/* Presenti pompa lavaggio e/o pompa scarico BLAC e DSP */
		
		if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_BULK_DSP, &HND_App_Load ) != S_OK )
		{
			return HND_ERR_SAFETY_DATA; 	
		}
		
		Bulk_DSP_Off_Delay = (OFFS_B(HND_App_Load.Data,0) & 0x7F);
	}


	if( ( HWC_Flags.bit.bldc1 != 0 ) && ( Res_Pos[LD_WASH_RES_IND] != NO_LD_POS ) )
	{
		/* Presenti pompa lavaggio e resistenza lavaggio */
		
		if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_RESISTORWASH_PUMP_, &HND_App_Load ) != S_OK )
		{
			return HND_ERR_SAFETY_DATA; 	
		}
		
		Sft_WashPumpSpeedLow = OFFS_W(HND_App_Load.Data, 0) & 0x7FFF;	/* Define MQ */
		Sft_WashPumpSpeedHigh = OFFS_W(HND_App_Load.Data, 1) & 0x7FFF;	/* Define MQ */

	}

	/* init puntatore Heat Fault Params */

	if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_HEAT, &Tab_HeatFault ) != S_OK )
	{
		return HND_ERR_FAULT;
	}
	if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_MAIN_BOARD, &Tab_MainBoardFault ) != S_OK )
	{
		return HND_ERR_FAULT;
	}

	/* cicli speciali*/

	if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_HEAT, &Tab_HeatFault_Special ) != S_OK )
	{
		return HND_ERR_FAULT;
	}
	/* init puntatore Drain Fault Params */

	if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_DRAIN, &Tab_DrainFault ) != S_OK )
	{
		return HND_ERR_FAULT;
	}
	if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_LOAD, &Tab_LoadFault ) != S_OK )
	{
		return HND_ERR_FAULT;
	}
	if( HWC_Flags.bit.vs_wash_presence != 0)
	{	/* BLDC/BLAC presente */

		if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_VS, &Tab_VSFault ) != S_OK )
		{
			return HND_ERR_FAULT;
		}
	}

#if	defined (CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	if( ( HWC_Flags.bit.bldc1 ) )
		{	/* BLDC/BLAC presente */

			if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_EXT_DEV, &Tab_ExtDevFault ) != S_OK )
			{
				return HND_ERR_FAULT;
			}
			if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_EXT_DEV, &Tab_ExtDevFault_Special ) != S_OK )
			{
				return HND_ERR_FAULT;
			}
		}
		/* init puntatore Main Board Fault Params */

			/* init puntatore External Device Fault Params pompa drain */
	if( HWC_Flags.bit.bldc_drain != 0)
	{	/* BLDC/BLAC presente */

		if( BasicLoader( PTR_FAULT_DATA, DSPL_FLT_EXT_DEV_DRAINPUMP, &Tab_ExtDevFaultDrain ) != S_OK )
		{
			return HND_ERR_FAULT;
		}
		if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_SPEC_DEV_DRAINPUMP, &Tab_ExtDevFaultDrainSpecial ) != S_OK )
		{
			return HND_ERR_FAULT;
		}
	}
#endif
	/* init puntatore Main Board Fault Params Special Cycles  */

	if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_MAIN_BOARD, &Tab_MainBoardFault_Special ) != S_OK )
	{
		return HND_ERR_FAULT;
	}
	/* init puntatore Drain Fault Params - Special Cycles */

	if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_DRAIN, &Tab_DrainFault_Special ) != S_OK )
	{
		return HND_ERR_FAULT;
	}

	if( HWC_Flags.bit.vs_wash_presence != 0)
	{	/* BLDC/BLAC presente */

		if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_VS_SPEC, &Tab_VSFaultSpecial ) != S_OK )
		{
			return HND_ERR_FAULT;
		}
	}
	/* init puntatore Load Fault Params */


	if( BasicLoader( PTR_FAULT_DATA_SPECIAL, DSPL_FLT_LOAD, &Tab_LoadFault_Special ) != S_OK )
	{
		return HND_ERR_FAULT;
	}


	if(EV_Pos[LD_7LITER_EV_IND] != NO_LD_POS)
	{ /* Carichiamo dati 7 litri solo se esiste l'output 7 litri */
		if( BasicLoader( PTR_SAFETY_PARAM_DATA, DSPL_SFT_MOTOR_PARAMS_7LITER_TANK_, &Tab_SFT_7liter ) != S_OK )
		{
			return HND_ERR_SAFETY_DATA;
		}
	}


#ifdef _STATISTICS_ON_

    {        
    	uint8_t ret = BasicLoader(PTR_STATISTICS_DATA, DSPL_FUNCTIONS_STATISTICS_DATA, &Tab_FunctionsStatistics);
    	if (ret != S_OK)
    	{
    		Tab_FunctionsStatistics.Data = NULL;
    		Tab_FunctionsStatistics.DataLenght = 0;

/*    		if( ret != LDR_WRN_LENGHT )
    		{
    			 Il campo dati nullo � una possibilit� contemplata
    			return HND_ERR_FUNCTIONS_STATISTICS_DATA;
    		}*/ /*AE FIXME Configurazione errori */
    	}
    }

#endif	/* _STATISTICS_ON_ */
	
   
	
	return S_OK;

}
/**/

/**
Analizza i dati in base al risultato del restore degli stessi.

\param [in] 	none
\param [out]	none

\return 		S_OK se la funzione � stata eseguita correttamente, un codice d'errore altrimenti.

\notes:			deve essere eseguita solo al reset freddo e dopo che gli ingressi analogici sono 
				stati acquisiti (FLT_probeResWarningDataReset() fa riferimento alla temperatura
				della sonda lavaggio).

\author			Roberto Fioravanti
\date   		18/04/2008
\version    	0.0.1
*/
uint8_t HND_backupDataAnalysis(void)
{
const uint8_t *pCycleHeader;
uint8_t dummy;
uint8_t programmingCycle;
uint16_t tempFuncValue;
uint8_t cycleRestoreSpecification;

  /* Disabilitazione counter applicazione */
  
  /* se ho salvato contatori abilitati in backup li ritrovo abilitati 
  questa routine li disabilita in attesa di verificare le condizioni di abilitazione
  nell routine/uP opportuni */
  
  StopMicroprogCounters();
  (void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT); /* init funzione controllo turbina bloccata */
  uP_Load_Data.init = TRUE;			/* per uP Load */
  uP_LoadRestore_Data.init = TRUE;   /* per uP Load Restore */

  /* N.B.: DEA601 : alla partenza del collaudo automatico Portata.Storica vale '0'  */
    /* Non è un caso realistico in DEA 602: non viene effettuato collaudo automatico sulla base dei dati di backup
    ma la portata storica viene inizializzata la prima volta che il Chk dei dati di backup è KO (es. quando viene
    scaricato il fw) */

    /* Gestione rigenera dopo reset */
    if (uP_Regenerate_Data.status > REGENERATE_EXE_1)
    {/* concludi rigenera : ma in questo caso non viene aggiornato il numero di rigenere eseguite... */

      uP_Regenerate_Data.status = REGENERATE_TRUE;  /*??*/

      /*oppure: per contare sempre e comunque la rig eseguita*/
      /*uP_Regenerate_Data.status = REGENERATE_EXE_2;*/
      /*SetCounterVal(&uP_Regenerate_Data.EVdelay_counter) = 0;  */
    }
    else
    {/* restart rigenera */
      uP_Regenerate_Data.status = REGENERATE_INIT;
    }

  /* Gestione Sale/Rigenere per prima accensione macchina */

    if ( uP_Regenerate_Data.nReg == 0)
    {/* var = 0 non viene mai impostata dall'algoritmo (init =1) pu� indicare prima accensione macchina
        -> deve essere forzata accensione spia sale */

      uP_Regenerate_Data.nReg = OFFS_B(Tab_HWCRegenerateData.Data,2);
    }

/* Activate Algorithm : ALG_INIT per algoritmi legati a calibrazione/validazione/lettura torbidit� */
  
  CLRBIT(ActiveAlgorithmBmpOld[0], AAL_TURBIDITY_SENSOR_CAL_ALG);
  CLRBIT(ActiveAlgorithmBmpOld[0], AAL_TURBIDITY_READ_CONTINUOUS);
  CLRBIT(ActiveAlgorithmBmpOld[0], AAL_TURBIDITY_READ_ONESHOT);
  CLRBIT(ActiveAlgorithmBmpOld[0], AAL_TURBIDITY_VALIDATE_CALIB);
  		
  


/* Gestione Regolazioni/Supervisor */

  /* Copia in BufNewReg le regolazioni di BufOldReg */
	memcpy(&BufNewReg[0], &BufOldReg[0], (uint8_t)(sizeof(BufNewReg)));
	
	
	/* Verifica presenza cicli END, FAULT, PROGRAMMING: devono essere necessariamente presenti */
	/*La ricerca fatta qui è utile per controllare l'eventuale fault sul file di setting solo nelle
	operazioni iniziali e non a regime. Nel supercisore le eventuali incoerenze/errori di caericamento 
	vengono gestiti con un Reset Freddo che poi fa ricadere qui */
	
	if( SPV_searchCycleByLabel(CMPLT_LBL_END, &dummy) != S_OK )
	{
		return HND_ERR_BACKUP_ANALYSIS;
	}

	if( SPV_searchCycleByLabel(CMPLT_LBL_FAULT, &dummy) != S_OK )
	{
		return HND_ERR_BACKUP_ANALYSIS;
	}

	if( SPV_searchCycleByLabel(CMPLT_LBL_PROGRAMMING, &programmingCycle) != S_OK )
	{
		return HND_ERR_BACKUP_ANALYSIS;
	}

  /** Aggiornamento valore selettore */
  
	if( REG_getFunctionValueByID((uint8_t)(FUNC_ID_SELECTOR), &tempFuncValue) == S_OK)
	{
		SelectorFunctionValue = (uint8_t)(tempFuncValue);
	}
	else
	{
		/* 
		Se non posso ricavare il valore della funzione SELECTOR dalla regolazione (ad esempio, in
		caso di Backup KO, avrei indice di regolazione nullo) assegno a SelectorFunctionValue il
		valore che corrisponde alla fase del PROGRAMMING che indica l'OFF logico.
		*/
		SelectorFunctionValue = programmingCycle;
	}
	

	if(RestoreType == CHECK_RESTORE)  
	{ 
		/*****************************/
		/* Checksum Backup Data O.K. */
		/*****************************/

		/* Caricamento puntatore all'intestazione del ciclo salvato */
		if( SPV_getCycleHeaderPtr(Interpreter_Data[0].cycle, &pCycleHeader) != S_OK )
		{
			return HND_ERR_BACKUP_ANALYSIS;
		}
		
		
		/* DEA602: piuttosto che confrontare la label salvata con quella corrispondente al ciclo salvato
		viene preso come riferimento certo il ciclo salvato e da questo si ricava la label */
		
		/* Viene ricavata la Label del ciclo dall'intestazione */
		Interpreter_Data[0].completeLabel.b = ( OFFS_B(pCycleHeader, CYCLE_LABEL_OFFSET) & (CYCLE_SUBLABEL_MASK|CYCLE_LABEL_MASK) );
		
		/*In ARKADIA invece : */
  		/* Controllo di coerenza tra label e sublabel salvate e quelle corrispondenti al ciclo
  			salvato (potrebbero non essere coerenti se il backup � intervenuto prima che fossero
  			aggiornate) */
  		
  		
		if( ( Interpreter_Data[0].completeLabel.b == CMPLT_LBL_PAUSE) &&
			( Paused_Intpt_Data[0].completeLabel.s.label == LBL_RUN ) )
		{	
			/** Se il ciclo salvato è PAUSE ed il ciclo interrotto da PAUSE è RUN (non
				DELAY) per il restore si considera l'impostazione del ciclo interrotto.
				NOTA: se SPV_getCycleHeaderPtr() non va a buon fine pCycleHeader rimane
				invariato */
			(void)SPV_getCycleHeaderPtr(Paused_Intpt_Data[0].cycle, &pCycleHeader);
		}
		else
		{
			/* In tutti gli altri casi vale l'impostazione in Editor (in particolare per
			   il DELAY e per la PAUSE in DELAY � RESTORE_FROM_POWER_FAIL_IN_ANY_CASE) */
		}

		cycleRestoreSpecification = (OFFS_B(pCycleHeader, CYCLE_BC_OFFSET) & CYCLE_BC_MASK);
		

		switch( cycleRestoreSpecification )
		{
		
			case NEVER_RESTORE_FROM_POWER_FAIL:
				/* Riparte da PROGRAMMING */

				/* 
				In generale, se nel NEVER_RESTORE mantengo tutte le regolazioni, è
				giusto conservare anche DelayInMinutes (in particolare se sono in PROGRAMMING
				ed ho impostato alcune opzioni ed il delay, così come mantengo le opzioni è
				giusto che mantenga anche il delay). Il problema sorgerebbe nel caso in cui il
				ciclo di DELAY (o quello PAUSE, in cui DelayInMinutes può essere diverso da 0)
				fosse etichettato come NEVER_RESTORE, in quel caso ripartirei in PROGRAMMING
				con DelayInMinutes != 0, per cui allo START la macchina andrebbe in DELAY,
				che non sembra corretto. Per lo stato di FAULT invece non ci sono problemi in
				quanto all'ingresso in FAULT DelayInMinutes viene azzerato.
				*/
				
				Interpreter_Data[0].completeLabel.b = CMPLT_LBL_PROGRAMMING;
				Interpreter_Data[0].cycle = programmingCycle;
				Interpreter_Data[0].phase = SelectorFunctionValue;

  				ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));
  				
  				break;
  			
  		
  				case RESTORE_FROM_POWER_FAIL_IN_ANY_CASE:

				if( ( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING ) && (Interpreter_Data[0].phase != SelectorFunctionValue) )
				{
					/*
					Se lo stato da ripristinare è PROGRAMMING la fase deve essere
					allineata alla posizione del selettore. Se questa è diversa dalla
					fase salvata, l'interprete deve essere completamente reinizializzato.
					*/
					Interpreter_Data[0].phase = SelectorFunctionValue;
					ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));
					break;
				}		

				ITP_interpreterFlagsInit( (uint8_t)(ITP_PARTIAL_INIT));
				
				
				Interpreter_Data[0].jumpIF_phase_ctrl = 1;	/* per preservare il JumpIF valido per fase se presente */
				
			
						
			break;
			
			default:
			break;
		}

	}
	else if(RestoreType == DEFAULT_RESTORE)
	{
		/****************************************************************************************/
		/* Checksum Backup Data K.O. o fault file di setting individuato prima del reset freddo */
		/****************************************************************************************/

		/*	Per default mi porto nello stato di OFF logico */
		HND_defaultInterpreterInit(programmingCycle);
		
		/* 
		In questo caso, essendo l'indice di regolazione della funzione SELECTOR uguale a 0
		(perchè i dati di backup non sono buoni o perchè è stato appositamente scritto nel caso
		fosse stato salvato un fault file di setting), SelectorFunctionValue è stata inizializzata
		a programmingCycle, per cui Interpreter_Data[0].phase e SelectorFunctionValue sono coerenti.
		*/
		
		
		/* Deafult init altre variabili */
		HND_defaultVarInit();
		
	}
	else
	{
		/* do nothing */
	}

	/* Aggiornamento altri valori di regolazione */
	/** Stabilito con quale ciclo ripartirà la macchina, si può calcolare il valore delle
		funzioni la cui tabella di regolazione può dipendere dal ciclo stesso. Anche se in DEA602
		per il momento non sono previste funzioni con regolazioni dipendenti dal ciclo.
	 
		I valori di default sono utilizzati nel caso non sia possibile ricavare il valore dalla
		regolazione, in particolare se Backup KO (indice di regolazione nullo) o se la funzione
		non � presente nel setting	*/
		
	DishRackFunctionValue = DISH_RACK_DFLT_VALUE;
	HND_initFunctionValue((uint8_t)(FUNC_ID_BASKET), &DishRackFunctionValue);

	WaterForRegenerate = REGENERATE_WATER_DFLT_VALUE; 
	HND_initFunctionValue((uint8_t)(FUNC_ID_WATER_HARDNESS), &WaterForRegenerate);

	ZoneWashFunctionValue = ZONE_WASH_DFLT_VALUE; 
	HND_initFunctionValue((uint8_t)(FUNC_ID_ZONE_WASH), &ZoneWashFunctionValue);
	
	
	RinseAidFunctionValue = RINSE_AID_DFLT_VALUE;
	HND_initFunctionValue((uint8_t)(FUNC_ID_RINSE_AID), &RinseAidFunctionValue) ;

	/* Imposta il timer per il Delay (che non viene salvato in backup) */
	SPV_setupDelayCounter();
	
	/*SparklingAid criterion init */
    if((Flags_2 & (1<<FLAGS2_REED_NO_SPARKL_BIT)) ==0)
    {
		SETBIT(GICriteria, GI_SPARKLING_AID_IND);
    }
	
	/* delta : 1% (TurbiditySensorCal.adH2oRefDelta può contenere il valore di default)*/
    TurbiditySensorCal.adH2oRefDelta=TurbiditySensorCal.adH2oRef/100;

	#if	(SBY_STANDBY_ENABLE == 1)
		/* Init StandBy */
		STB_InitStandBy_AfterBackUp();
	#endif

	return S_OK;
}
/**/

/**
Porta l'Interprete nello stato di OFF logico, che corrisponde alla fase del ciclo di PROGRAMMING
corrispondente alla posizione del ciclo stesso (di norma Ciclo = 0, Fase = 0).

\param [in] 	programmingCyclePosition è la posizione del ciclo di PROGRAMMING.
\param [out]	Interpreter_Data[0] (global)

\return			none

\author 		Marco Quintini
\date			21/07/2006
\version		1.0.0
*/

static void HND_defaultInterpreterInit(uint8_t programmingCyclePosition)
{
	Interpreter_Data[0].completeLabel.b = CMPLT_LBL_PROGRAMMING;
	Interpreter_Data[0].cycle = programmingCyclePosition;
	Interpreter_Data[0].phase = programmingCyclePosition; 

	ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));
				
			
}
/**/


/**
Inizializza le variabili che hanno un particolare valore di default in caso di 
chksum dei dati di backup KO


\return			none

\author 		Marco Quintini
\date			13/05/2008
\version		1.0.0
*/
static void HND_defaultVarInit(void)
{

#if (VIRTUAL_SENSOR_ENABLE == 1)
	if (HWC_Flags.bit.vs_wash_presence != 0)
	{
		if ( State5.max_speed_value==0)
		{/* deve essere messa nel backup, così in caso di mancanza rete viene ripristinata la condizione*/
			State5.max_speed_value =0xFFFF;
		}
	}
#endif


/* Torbidità : valore di calibrazione (pwm defalut) */
    TurbiditySensorCal.val = TURBIDITY_PWM_MIN;
    TurbiditySensorCal.adH2oRef=TURBIDITY_H2O_DEFAULT_ADVAL;
	
	if(!HWC_Flags.bit.sparklaidreed)
	  {
	    SETBIT(Flags_2,FLAGS2_TURB_NO_SPARKL_BIT);
	  }
	  else
	  {
	    SETBIT(Flags_2,FLAGS2_REED_NO_SPARKL_BIT);
	  }
  
  /* Rigenera */
	WaterForRegenerate = REGENERATE_WATER_DFLT_VALUE;
	
	/* Init portata storica: va fatto qui e non in HND_dwConfigInit 
	che viene chiamato ad ogni Cold Reset. Invece il valore di portata storica viene continuamente 
	aggiornato e salvato in backup */

	if ( HWC_Flags.bit.vs_wash_presence==0)
	{
		uP_Load_Data.levTOtimeRef = LEV_TO_TIME_HIST;
 	}
  
  /*init contatore rigenere forzate (può essere scritto in RAM per fidatezza) */
	uP_Regenerate_Data.forceReg_cnt = 0;

  /* Gestione Sale / Rigenere alla prima accensione macchina (non ho backup del dato -> nReg = 0)*/
  	
	uP_Regenerate_Data.nReg = OFFS_B(Tab_HWCRegenerateData.Data,2);
	

    
  /* stato brillantante */  

}

/**
Inizializza la variabile globale utilizzata dall'applicazione per conoscere il valore della
funzione specificata da funcID.

\param [in] 	funcID è l'ID della funzione
\param [in] 	Interpreter_Data[0] (global)
\param [out] 	*funcValue è il valore della funzione
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		24/01/2007
\version    	1.0.0

*/

static void HND_initFunctionValue(uint8_t funcID, uint8_t *funcValue)
{
uint16_t tempFuncValue;

	if( REG_getFunctionValueByID(funcID, &tempFuncValue) == S_OK )
	{
		*funcValue = (uint8_t)(tempFuncValue);
	}
}

/**/

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */





