/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par	Gestisce le statistiche.
\Header file.
 
\file    DW_Statistics.c
\ingroup DW_Statistics
\date    11/09/2008
\author  Valeria Catani
\version 1.0.0


\Par		History (titolo)
 	
\n		
*/

/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"
#include "DigitalInputsExp.h"
#include "AnalogExp.h"
#include "PM_AnalogFeedbackExp.h"
#if defined(PLATFORM2_SUPPORT)
#include "BCP_BusCommMng.h"
#endif /* defined(PLATFORM2_SUPPORT) */

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#include "PM_PowerProfileExp.h"
#endif

#ifdef _STATISTICS_ON_			

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
void STC_OnTimeUpdate(void);

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
Salva i dati nella 'Scatola nera' in caso di prefault 
In caso di fault viene semplicemente aggiornato il tipo di evento, ma non i dati
precedentemente salvati nel prefault.

\param [in] 	
\param [out]	
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		18/05/2007
\version    	1.0.0

*/


void STC_saveBlackBoxData(sft_fault_codes_t faultToBeSaved)
{
uint8_t i, analogFb_bitmap;

	/* Il salvataggio avviene in corrispondenza di un prefault */
	MachineStatistics.blackBox_Data[0].event = BB_EVENT_PREFAULT;
	
	/* Codici di guasto nel momento del salvataggio */

	MachineStatistics.blackBox_Data[0].faultCodes = faultToBeSaved;

	/* Ciclo, fase, passo */
	MachineStatistics.blackBox_Data[0].cycle = Interpreter_Data[0].cycle;
	MachineStatistics.blackBox_Data[0].phase = Interpreter_Data[0].phase;
	MachineStatistics.blackBox_Data[0].first_statement = Interpreter_Data[0].first_statement;

	/* Carichi attivi */
	MachineStatistics.blackBox_Data[0].activeLoadsBmp = 0;
	for(i=BB_VALID_LOADS_NUM;i>0;i--)
	{
		if(LoadsStatus[i-1] != 0)
		{
			/*lint -e712  1 <= i <= 16 */
			SETBIT(MachineStatistics.blackBox_Data[0].activeLoadsBmp, (i-1));
			/*lint +e712 */
		}
	}
	
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT

	/* velocit� pompa lavaggio */
	if( ( HWC_Flags.bit.bldc ) || ( HWC_Flags.bit.bldc1 ) )
	{
		MachineStatistics.blackBox_Data[0].pumpSpeed = ((int16_t)(BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VCURR_MSB]<<8) + (int16_t)BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VCURR_LSB]);
	}
	else 
	{
		MachineStatistics.blackBox_Data[0].pumpSpeed = LoadsStatus[LD_WASH_PUMP_IND];
	}
  
#endif	
	 
	/* Stato input analogici + feedback analogici */
	
	analogFb_bitmap = AnalogFeedbackGetMaskAnalogF_Enabled();	
  	 
	for(i=0; i<8; i++)
	{
	  
	  if ( TESTBIT(analogFb_bitmap,i) )
	  {
	    MachineStatistics.blackBox_Data[0].analogfeedbackVal = PM_AnalogFeedbackGetValue (i);
	  }

	  MachineStatistics.blackBox_Data[0].analogInputs[i] = 0xFFFF;
	  
	  if (0 != (LIB_ADC_IsApplAnalogReady(1<<i) ))
	  {
	    MachineStatistics.blackBox_Data[0].analogInputs[i] = LIB_ADC_GetAnalogValue (i);
	  }
	}

	/* Stato input digitali */
	
	MachineStatistics.blackBox_Data[0].digInputs = DigInputs;
		
	/* Generic Input */
	MachineStatistics.blackBox_Data[0].washTemperature = GITemp;
	
	MachineStatistics.blackBox_Data[0].waterLoaded = GIWaterLoaded;
	
	MachineStatistics.blackBox_Data[0].turbidity = TurbidityReads.perc;		 /* GITurbidity; */
  	
	MachineStatistics.blackBox_Data[0].criteria = GICriteria;
	
	MachineStatistics.blackBox_Data[0].giValid = GIValid;
	
	
	/* Reset ed errori I2C */
	MachineStatistics.blackBox_Data[0].coldResetNum = MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].coldResetNum;
	MachineStatistics.blackBox_Data[0].warmResetNum = MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].warmResetNum;
	

	memcpy(&MachineStatistics.blackBox_Data[0].commErr[0][0], &MachineStatistics.testCycleStats.commErr[0][0],sizeof(MachineStatistics.blackBox_Data[0].commErr[0]));


	MachineStatistics.blackBox_Data[0].flags_2copy = Flags_2;
	MachineStatistics.blackBox_Data[0].virtual_Sensor[GI_VIRTUAL_SENSOR_WASH] = Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH];
	MachineStatistics.blackBox_Data[0].virtual_Sensor[GI_VIRTUAL_SENSOR_DRAIN] = Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN];
	MachineStatistics.blackBox_Data[0].State5_algo = State5.max_speed_value;
	MachineStatistics.blackBox_Data[0].failnumVs.VScheckinconsistency=FltFlagsVS.VScheckinconsistency;
	MachineStatistics.blackBox_Data[0].failnumVs.warninglostwater =FltFlagsVS.warninglostwater;
	MachineStatistics.blackBox_Data[0].dirty_foam_state=  *((uint8_t *)(&dirty_foam_Data +1));



}




/**
Processa la variabile ExternalRequest e a seconda del suo valore effettua delle operazioni
di reset sulle statistiche.

\param [in/out] ExternalRequest (global)	
\param [out]	MachineStatistics (global)
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		02/07/2007
\version    	1.0.0

*/
void STC_manageStatResetRequest(void)
{
	if(ExternalRequest == SYSTEM_NORMAL_RESET_LAST_FAULT)
	{
		/* Fa scorrere il buffer dei Last Fault, eliminando il più vecchio
		   e resettando la locazione del più recente */
		FLT_lastFaultUpdate(No_Fault, &MachineStatistics.lastFault[0]);
	}
	else if	(ExternalRequest == SYSTEM_NORMAL_RESET_TO_DEFAULT)
	{
		/* Azzera tutte le statistiche permanenti */
		memset(&MachineStatistics, 0, sizeof(MachineStatistics));
	}
	else
	{
		return;
	}
	
	ExternalRequest = 0;
}

/**
Aggiorna tutte le statistiche che vengono registrate a fine ciclo, 
-statistiche generiche globali
-statistiche specifiche relative agli ultimi n cicli eseguiti 
(eliminando quelle del pi� vecchio e introducendo in testa quelle del pi� recente).

\param [out]	MachineStatistics (global)
	 
\return     	none

\notes:			

\author			Roberto Fioravanti
\date   		24/09/2008
\version    	1.0.0

*/
void STC_updateLastCycleStat_END(uint8_t mode)
{

SWTC_TimeType cycleDuration;

	/**
	Aggiornamento statistiche ultimi cicli
	*/	
    /* ciclo */
	/*MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].cycle = StartedCycle;*/



	cycleDuration = SWTC_GetElapsedTime((SWTC_CountersType) STC_TEMP_CYCLE_STATISTIC_M_COUNTER);
	
	if( cycleDuration > UINT8_T_MAX )
	{
		/* La massima durata memorizzabile è 255 minuti */
		cycleDuration = UINT8_T_MAX;
	}

	/*lint -e734 cycleDuration è limitato a 255 */
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].duration= cycleDuration;
	/*lint +e734 */

	
  	/**
	Aggiornamento statistiche globali 
	(dopo quelle degli ultimi cicli, utilizzano alcune informazioni
	che vengono aggiornate nell'array degli ultimi cicli */  
	
	
	/* Cicli eseguiti */
	if(0 == mode)
	{
		if( StartedCycle < SIZE(MachineStatistics.globalStats.executedCycles_Num) )
		{
			/* Si potrebbe anche memorizzare il ciclo Interpreter_Data.cycle anzichè StartedCycle */
			IncWordWithCheck(&MachineStatistics.globalStats.executedCycles_Num[StartedCycle]);
		}
	}
	
	/* Numero cicli in cui è avvenuta almeno una condizione di KO Restore */
	
  if ( MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].restore_koRestore.b.koRestore != 0 )
  {
	  MachineStatistics.globalStats.KORestoreCycles_Num++;
  }

/* Connectivity stat */
  	  /* UPDATE WATER LOADED/WEIGHT ESTIMATION */
  	  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].waterxcycle = cycle_loaded_water;
  	  /*Turbidity*/
  	  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].turbidity = TurbidityReads.perc;		 /* GITurbidity; */
  	  /* UPDATE ENERGY STATISTCS */
  	  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].energyCycleConsumption = ((b_ui32EnergyCounterWh * 3600) +  b_ui16EnergyCounterWhAcc)- PrevEnergyCount; /* [W/h] */
  	 /* APP Statistics */
  	  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].APPEnergyConsumption = TempCycle_Stats.APPEnergyConsumption;
  	  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].APPTotalDuration = TempCycle_Stats.APPTotalDuration;

  	  /* Update cicli schedulati */
	  

	  if (MachineStatistics.globalStats.water_loaded + cycle_loaded_water < UINT32_T_MAX )
	  {
		  MachineStatistics.globalStats.water_loaded += cycle_loaded_water;
	  }
	  else
	  {
		  MachineStatistics.globalStats.water_loaded = UINT32_T_MAX;
	  }
	  cycle_loaded_water = 0;
/*end connectivity stat*/
			   /**/

  	  /* ON Time : tempo di Run */
  	  STC_OnTimeUpdate();
  
}
/**
Inizializza tutte le variabili temporanee per le statistiche relative all'ultimo ciclo eseguito.

\param [out]	TempCycle_Stats (global)
	 
\return     	none

\notes:			

\author			Roberto Fioravanti
\date   		24/09/2008
\version    	1.0.0

*/
void STC_LastCycleStat_Init(void)
{    

    /* reset var temporanea per statistiche */
    TempCycle_Stats.turbindex=0;
    TempCycle_Stats.flags.flags=0;
    /* reset statistiche last cycle */
  /*  memset(&MachineStatistics.lastCycles, 0, sizeof(MachineStatistics.lastCycles));*/
    /* imposta propriet� timer */
	(void)Set_MediumCounter((SWTC_CountersType)STC_TEMP_CYCLE_STATISTIC_M_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1MIN, 0x7FFF); /* MQ introdurre define */
 /* Saving regulation for hour and minute di quando il ciclo va in run  */	
	
}


/**
Fa scorrere la lista fifo degli ultimi cicli (viene chiamata ad inizio ciclo)

\param [out]	
	 
\return     	none

\notes:			

\author		Valeria Catani
\date   		17/10/2008
\version    	1.0.0

*/
void STC_LastCyclesArray_Init(void)
{
uint8_t i;

  /** 
	Aggiorna lista delle statistiche degli ultimi cicli. Fa scorrere la lista fifo.
	*/
	for(i=(SIZE(MachineStatistics.lastCycles) - 1);i>0;i--)
	{
		MachineStatistics.lastCycles[i] = MachineStatistics.lastCycles[i - 1];
		
	}  
	
  /*_memset_clear_8bitCount( &MachineStatistics.lastCyclesStats[MOST_RECENT_STAT_CYCLE_POS], sizeof(MachineStatistics.lastCyclesStats[MOST_RECENT_STAT_CYCLE_POS]) );*/
	memset(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS], 0, sizeof(MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS]));
	
	/* ciclo */
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].cycle = StartedCycle;
	
}

/**/

/**
Aggiornamento statistiche nel Supervisor
-Abilitazione del contatore della durata di un ciclo e

\param [in]     Interpreter_Data[0].completeLabel
\param [out] 	TempCycle_Stats.duration_timer (global)
	 
\return     	none

\notes:			

\author			Roberto Fioravanti
\date   		24/09/2008
\version    	1.1.0

*/

CM_Err_ClassType PrevCommErrCounters[sizeof(CommErr)/sizeof(CommErr[0])][sizeof(CommErr[0])/sizeof(CommErr[0][0])];

void STC_superVisorUpdateStatistics(void)
{

uint8_t channelCnt, errTypeCnt;
CM_Err_ClassType newCommErrCounters;
CM_Err_ClassType deltaCommErr;
LIB_Fixed_ValueType dummy;



	/* Contatore durata ciclo */
	if(Interpreter_Data[0].completeLabel.s.label == LBL_RUN)
	{
		/* Abilita contatore (solo in RUN) */
		(void)SWTC_Run((SWTC_CountersType)STC_TEMP_CYCLE_STATISTIC_M_COUNTER);
	}
	else
	{
		/* Disabilita contatore nei cicli diversi da RUN */
		(void)SWTC_Stop((SWTC_CountersType)STC_TEMP_CYCLE_STATISTIC_M_COUNTER);
	}
	
	/* Update rejoin counter */
	if (( Join_Leave_Reg != oldJoinValue )  && (Join_Leave_Reg == RE_JOIN_ERC))
	{
		if( SPV_isCycleRunning() == TRUE )
		{
			IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].numRejoin);
		}
		IncWordWithCheck(&MachineStatistics.globalStats.numReJoin);
		
	}
	oldJoinValue = Join_Leave_Reg;	
	
			

	/* Statistica intervento routine di sblocco I2C */
	for(channelCnt = 0; channelCnt < (sizeof(CommErr)/sizeof(CommErr[0])) ; channelCnt++)
	{
			for(errTypeCnt = 0; errTypeCnt < (sizeof(CommErr[0])/sizeof(CommErr[0][0])); errTypeCnt++)
			{
				/* Statistica errori di comunicazione */
				newCommErrCounters = CommErr[channelCnt][errTypeCnt];

				if( SPV_isCycleRunning() == TRUE )
				{
					/* Statistica aggiornata in RUN, DELAY e PAUSE */
					deltaCommErr = newCommErrCounters - PrevCommErrCounters[channelCnt][errTypeCnt];

					if( (UINT16_T_MAX - MachineStatistics.testCycleStats.commErr[channelCnt][errTypeCnt]) >= deltaCommErr )
					{
						/* C'è ancora posto */
						MachineStatistics.testCycleStats.commErr[channelCnt][errTypeCnt] += deltaCommErr;
					}
					else
					{
						/* Non c'è abbastanza posto, la statistica viene limitata a 65535 */
						MachineStatistics.testCycleStats.commErr[channelCnt][errTypeCnt] = UINT16_T_MAX;
					}
		    	}

			    PrevCommErrCounters[channelCnt][errTypeCnt] = newCommErrCounters;
			}
		}

		/*gestisce le statistiche globale energia consumata*/
		if(b_ui32EnergyCounterWh > MachineStatistics.globalStats.energyConsumption)
		{
			MachineStatistics.globalStats.energyConsumption = b_ui32EnergyCounterWh;
		}

		 /*LastCycle vline*/
		  if( MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].Vline == 0 &&  ExtMotor_DataBuffer[1] > 0)
		  {
			  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].Vline = VlineCalcolusFromDSP(&dummy);
		  }
}

void STC_functionValueUpdateStatistics(void) 
{
    uint8_t i;
    const uint8_t *ptrUIFunctionsTab;
    uint16_t bufferStartIndex;
    uint16_t tempWordVar;
    uint8_t funcRegNum;
    uint8_t funcIndex;
    uint16_t funcValue = 0;
    uint16_t funcValueAux=0;
    uint8_t retCode;


    /* Valori funzioni */

	/* Bitmap delle funzioni On-Off attive nell'ultimo ciclo */	
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].on_off_functions = 0;

	ptrUIFunctionsTab = Tab_FunctionsStatistics.Data;
	bufferStartIndex = 0;
	
	for(i=0; i<Tab_FunctionsStatistics.DataLenght;i++)
	/* NOTA: se nel setting non � stata inserita la tabella necessaria, 
	   Tab_FunctionsStatistics.DataLenght viene inizializzata a 0 in HND_wmConfigInit(), per cui
	   il ciclo for non viene eseguito affatto e Tab_FunctionsStatistics.Data non � utilizzato */
	{
		funcRegNum = OFFS_B(ptrUIFunctionsTab, 1);
		/* Controllo se la posizione della funzione � coerente con la dimensione di BufNewReg[] */
		if( (*ptrUIFunctionsTab) < SIZE(BufNewReg) )
		{
			funcIndex = BufNewReg[*ptrUIFunctionsTab] - 1;
			if(funcIndex < funcRegNum)	/* Rende superfluo il controllo funcIndex != 0xFF */
			{
		        if(funcRegNum == ON_OFF_FUNC_REG_NUM) 
		        {/* E ' una funzione On - Off */                    
		    		  
					if ( funcIndex == (INDEX_ON_VALUE-1)  )
					{/* E' una funzione ON - Off attiva */

						if( i< BYTE_SIZE )
						{/* C'� ancora spazio nella bitmap -> aggiorno stat ultimo ciclo */

							SETBIT(MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].on_off_functions, i);
						}

						/* Aggiornamento stat globali funzione on off*/
						IncByteWithCheck(&MachineStatistics.globalStats.functionsValues_Buffer[bufferStartIndex]);

					}

					funcRegNum=1;   /* si incrementa l'indice di una sola unit� per le stat globali */
		        } 
		        else 
		        {
		          tempWordVar = funcIndex + bufferStartIndex;
		                  
		          if( tempWordVar < SIZE(MachineStatistics.globalStats.functionsValues_Buffer) )
		  				{
		  				    /* statistiche globali funzione multi-regolazione*/
		  					/* Numero di volte che una funzione ha assunto un dato valore */
		  					IncByteWithCheck(&MachineStatistics.globalStats.functionsValues_Buffer[tempWordVar]);
		  				}
		        }
			}
		}

		bufferStartIndex += funcRegNum;
		ptrUIFunctionsTab += 2;	
	}

	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].clockHH = TempCycle_Stats.clockHH;
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].clockMM= TempCycle_Stats.clockMM;
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].schedule_mode = Join_Leave_Reg;
		oldJoinValue = Join_Leave_Reg;

	/*  Saving schedule regulation */

	 	  if (( Join_Leave_Reg == ON_ERC_ECO ) ||
	 	      ( Join_Leave_Reg == ON_ERC_SMART ))
	 	  {
	 	  	IncWordWithCheck(&MachineStatistics.globalStats.executedschedulCycles);

			/* Saving time constraint regulation */
	 		  retCode = REG_getFunctionValueByID((uint8_t)(FUNC_ID_END_PROGRAM_HH), &funcValue);
	 		  retCode |= REG_getFunctionValueByID((uint8_t)(FUNC_ID_END_PROGRAM_MM), &funcValueAux);
	 		  if (( retCode == S_OK ) && ((retCode & REG_NOT_EXIST)==0))
	 		  {
	 			  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayConstrHH = (uint8_t)funcValue;
	 			  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayConstrMM = (uint8_t)funcValueAux;
	 		  }
	 		  else
	 		  {
	 			  /* Update clock information with dummy value if the clock regulation are not present*/
	 			  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayConstrHH = DUMMY_CLOCK_VALUE;
	 			  MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayConstrMM = DUMMY_CLOCK_VALUE;
	 		  }
	 	  }
	 	  /* **** Save Initial Water **** */
	 	  	 PrevWaterCount = TempCycle_Stats.loaded_water;
	 	 	 /* **** Saving energy information **** */
	 	   PrevEnergyCount = (b_ui32EnergyCounterWh * 3600) +  b_ui16EnergyCounterWhAcc;
	 	   /* **** Saving APP data **** */
	 	   PM_PowerProfile_TotalProfileEnergy(1, &TempCycle_Stats.APPEnergyConsumption);

	 	   PM_PowerProfile_TotalProfileDuration(1, &TempCycle_Stats.APPTotalDuration);
}

void STC_userDishRack_H2oHardness_Update(void)
{
    MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].userH2oHardness=WaterForRegenerate;

    MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dishRack_regenerate_spaklingAid.b.dishRack=DishRackFunctionValue;
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].washzone_reg=ZoneWashFunctionValue;
}

void STC_regenerate_spaklingAid_Update(void)
{
    if(TESTBIT(ApplicationFlags, AF_NO_SALT_BIT)==0)
    {
        /* Sale presente */
        MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dishRack_regenerate_spaklingAid.b.regenerate_cnt=0;
    } 
    else 
    {        
        /* Sale assente */
        MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dishRack_regenerate_spaklingAid.b.regenerate_cnt=(uP_Regenerate_Data.nReg<=31)?uP_Regenerate_Data.nReg:31;
    }
    MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].dishRack_regenerate_spaklingAid.b.sparkling_aid_state=((Flags_2 & FLAGS2_NO_SPARKL_AID_MSK)?1:0);
	MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].rinse_aid=RinseAidFunctionValue;
}

void STC_Add_LastCycle_turbPerc_turbCalType(void)
{
    if(TempCycle_Stats.turbindex<STAT_SAVED_CYCLES_TURB_READS_NUM) 
    {        
        MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].turbPerc_turbCalType.b[TempCycle_Stats.turbindex].turbidity=TurbidityReads.perc;
        MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].turbPerc_turbCalType.b[TempCycle_Stats.turbindex].turbCalType=(TurbiditySensorCal.val==TURBIDITY_PWM_MIN)?1:0;
        TempCycle_Stats.turbindex++;
    }
}


/* ************** */
/* Local Function */
/* ************** */


/**

Calcola il tempo di ON (Statistiche globali) inteso come tempo di RUN espresso in ore

\param [out]	MachineStatistics.globalStats.OnTime (global)
	 
\return     	none

\notes:			

\author			Valeria Catani
\date   		15/10/2008
\version    1.0.0

*/
void STC_OnTimeUpdate(void)
{
uint32_t time_tmp;


  time_tmp = (uint32_t)( ((uint32_t)(MachineStatistics.globalStats.OnTime * 60)) + 
                (MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].duration) +
                (TempCycle_Stats.ONTime_Minutes) );
  
  MachineStatistics.globalStats.OnTime = (time_tmp / 60);
  
  TempCycle_Stats.ONTime_Minutes = ( time_tmp - (MachineStatistics.globalStats.OnTime * 60) );
  
}
/**/

#endif /*_STATISTICS_ON_*/
/* *********** */
/* End of file */
/* *********** */
