/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par	
\Header file.
 
\file    DW_Supervisor.c
\ingroup DW_Supervisor
\date    08/04/2008
\author  Valeria Catani
\version 1.0.0


\Par		History (titolo)
 		
\version old version
\date    
\author Marco Quintini 
\n		
*/

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "LIB_ResetExp.h"
#include "LIB_BackUpExp.h"
#include "PM_AlternatoExp.h"

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#include "PM_PowerProfileExp.h"
#include "PM_PowerProfile.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Bit definition per updateCycleDependentFunctions */
#define CURRENT_SPV_CALL	0
#define PREV_SPV_CALL		(CURRENT_SPV_CALL + 1)

/** Soluzioni alternative per i cicli di riferimento per le funzioni */
/*#define SOLUTION_1_FOR_REF_CYCLE*/	/* Nei cicli DELAY, PAUSE ed END la fase indica sempre il ciclo
									   avviato. 
									   Il ciclo di riferimento per le tabelle di regolazione rimane
									   sempre il ciclo avviato, anche se interviene un Jump ad un
									   ciclo diverso */
									   
/*SOLUTION 2 E' COME DEA601 CON UNA PRECISAZIONE SULLA GESTIONE DEL CICLO DI RIFERIMENTO IN PAUSA */									   
#define SOLUTION_2_FOR_REF_CYCLE  /* Nei cicli DELAY, PAUSE ed END la fase indica sempre il ciclo
									   avviato. 
									   In caso di Jump ad un ciclo diverso da quello avviato, il 
									   ciclo di riferimento per le funzioni diventa il nuovo ciclo.
 									   Questo vale anche per il ciclo PAUSE, mentre in END il ciclo
 									   di riferimento torna ad essere il ciclo avviato (=> possibile
 									   incoerenza nella transizione RUN->END!!) */
/*#define SOLUTION_3_FOR_REF_CYCLE*/ /* Nei cicli DELAY, PAUSE ed END la fase indica rispettivamente
									   il ciclo che partir�, che � stato interrotto, che � terminato.
									   In PAUSE ed END la fase potrebbe differire dal ciclo avviato
									   nel caso di Jump ad un ciclo diverso.
									   In caso di Jump ad un ciclo diverso da quello avviato, il
 									   il ciclo di riferimento per le funzioni diventa il nuovo
 									   ciclo, e questo vale anche per PAUSE ed END */

#define APP_REGULATIONS_BITMAP_SIZEOF    ((NUM_REG_MAX%8)!=0?1:0)+(NUM_REG_MAX/8)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void SPV_updateCycleLabel(void);
static void SPV_saveIntptState(Interpreter_Struct *pDestinationBuffer);
static void SPV_restoreIntptState(const Interpreter_Struct *pSourceBuffer);
static void SPV_stopMicroprogVars(void);
static bool_t SPV_testCycleLabel(uint8_t cycle, uint8_t completeLabel);
static bool_t SPV_testRefCycleLabel(uint8_t completeLabel);
static bool_t SPV_isCycleWithLabelRunning(uint8_t completeLabel);
static void SPV_manageDelayCounters(void);
static void SPV_resetSensVariablesButDelay(void);
static void SPV_startCycleInit(void);
static void SPV_endCycleResetVars(void);
static void SPV_startCycle(void);
static uint8_t SPV_updateFunctionValue(uint8_t funcID, uint8_t *funcValue);
static void SPV_generateColdReset(void);
static void SPV_faultCompleteReset (void);
bool_t SPV_RegulationsMonitor(uint8_t* pOldRegs, uint8_t* pNewRegs);
void SVP_BuildBmpFunctionsActingAsMofifiers(void);
PM_PowerProfile_ProfileIndexType SPV_getPowerProfileIndex(uint8_t cycle, uint8_t mSetting);

void Set_UpCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode); /*AE Remove when possible*/
static void SPV_updateTimeToEnd(void);
void SPV_updatePowerProfile(uint8_t oldLabel, uint8_t oldPhaseIndex);
/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
static bool_t  bRegsChanged = FALSE; /* If 1, will be calculated new parameter for Power Profile */
static uint8_t regBmp[APP_REGULATIONS_BITMAP_SIZEOF];
static uint8_t maxFuncIndex = 0;
#endif

/* ***************** */
/* Exported function */
/* ***************** */


/**
Gestisce le transizioni di stato della macchina.
 
\param [in]		Interpreter_Data[0] (global) � la struttura che contiene ciclo, fase, passo ecc.
\param [in]		SelectorFunctionValue (global)
\param [out]	
 
\return 		none
 
\notes:		
 
\version		0.0.1
\date 			30/05/2006
\author			Marco Quintini

*/

void SPV_superVisor(void)
{
uint16_t tempFuncValue, tempFuncValue2;
uint8_t interpreterResult;

uint8_t destinationCycle;
uint8_t startFunctionValue;

uint8_t retCode,retCodeAux;
uint16_t tempTotalEnergyEst;	/*Local variable for energy estimation*/
bool_t started_cycle;



#if defined (_ENABLE_SUPERVISOR_SKIP_WD_) && defined (__CLASSBCHECK_H)
uint8_t classBCheckTestWD_EndCycle = FALSE;
#endif

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
uint8_t  oldLabel;
uint8_t  oldPhase;

PM_PowerProfile_ProfileIndexType index = 1;

uint16_t tmpSize;
PM_PowerProfile_EnergyPhaseType* pFirstEnergyPhase;
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */


#if CONFIG_PM_POWER_PROFILE_SUPPORT
	/* Save appliance label of first compartment */
	oldLabel = Interpreter_Data[0].completeLabel.s.label;
	/* Save appliance phase of first compartment */
	oldPhase = Interpreter_Data[0].phase;

	bRegsChanged = SPV_RegulationsMonitor(BufOldReg, BufNewReg);
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
	/**
	======================================================================================  	
		CHECK SELETTORE PROGRAMMI                                                            
	======================================================================================
	*/  	
	if( REG_checkIfChanged((uint8_t)(FUNC_ID_SELECTOR)) == REG_CHANGED )
	{
		retCode = REG_getFunctionValueByID((uint8_t)(FUNC_ID_SELECTOR), &tempFuncValue);
		
		if( retCode == S_OK )
		{
			if( (uint8_t)(tempFuncValue) < PHASES_PROGRAMMING_NUM )
			{
				SelectorFunctionValue = (uint8_t)(tempFuncValue);
				if( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING )
				{
					Interpreter_Data[0].phase = SelectorFunctionValue;
					ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));			    

				}

				REG_updateOldReg((uint8_t)(FUNC_ID_SELECTOR));
			}
			else
			{
				/* Do nothing (keep BufOldReg[i] unchanged) */
			}
		}
		else if( retCode == REG_NULL_INDEX )
		{
			/* Gestire in modo particolare il valore della funzione (es. valore di default)? �_� */

			REG_updateOldReg((uint8_t)(FUNC_ID_SELECTOR));
		}
		else
		{
			/* Do nothing (keep BufOldReg[i] unchanged) */
		}
	}
	/**
	======================================================================================  	
		CHECK START REG
	======================================================================================
	*/
	
	started_cycle = FALSE;
	  	    
	if( REG_checkIfChanged((uint8_t)(FUNC_ID_START)) == REG_CHANGED )
	{
	bool_t rejectedRegulation = FALSE;
	

		retCode = REG_getFunctionValueByID((uint8_t)(FUNC_ID_START), &tempFuncValue);

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
    /* Fixed for validate the last value */
    if ((retCode == S_OK) || (retCode == REG_WRN_OUT_OF_RANGE_INDEX))
#else		/*AE TODO check APP REG_WRN_OUT_OF_RANGE*/
		if( retCode == S_OK)
#endif
		{
			startFunctionValue = (uint8_t)(tempFuncValue);
			
			switch(startFunctionValue)
			{
				case REG_START_CONDITION:
					
					if(Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING )
					{
						/**
						=============
						 SI PARTE !!
						=============
						*/

					  SPV_startCycle();
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
					  Set_UpCounter(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
					  Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
					  (void)SWTC_Run((SWTC_CountersType)PP_ENPHELAPSEDTIME_UPCOUNTER);
					  (void)SWTC_Run((SWTC_CountersType)PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);

#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
					  uiSavedEnPhElapsedTime_Counter = 0;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
					  
					  /*DW: Se il ciclo in partenza � SAT (AUTOTEST) va aggiornato il contatore corrispondente (per statistiche)*/
					  /*upDateNumTimesSAT();*/
					  
					  started_cycle = TRUE;
					  cycle_loaded_water = 0;	/*Reset variable water loaded*/
					  		
					}		
					else if( Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE )
					{
					  /** Ripristina lo stato precedente la pausa */
						SPV_restoreIntptState(&Paused_Intpt_Data[0]);
						ITP_interpreterFlagsInit((uint8_t)(ITP_PARTIAL_INIT));

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
						/* timer continue... */
						SWTC_Run(PP_ENPHELAPSEDTIME_UPCOUNTER);
						SWTC_Run(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
						
						if (ApplPowerProfileStatus & PM_POWER_PROFILE_STATUS__DELAY_PHASE_TRANSITION_MSK)
						{							
							index = SPV_getPowerProfileIndex(Interpreter_Data[0].cycle, Interpreter_Data[0].mSetting);
							ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase,Interpreter_Data[0].mSetting,
																							  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
						}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
					}
					else
					{
						/* do nothing */
					}
									
					break;
				
				case REG_RESET_CONDITION:
				case REG_RESET_FAULT:

					rejectedRegulation = TRUE;	

					if( SPV_searchCycleByLabel(CMPLT_LBL_PROGRAMMING, &destinationCycle) == S_OK )
					{
						if( startFunctionValue == REG_RESET_CONDITION )
						{/* RESET COND : gestita in tutti gli stati (anche in FAULT)*/
						
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
								/* Reset the counter */
							/*	StopCounterToZero(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER); */
								/*
								MQ PA TODO Secondo me è corretto come sotto, dato PP_ENPHELAPSEDTIME_UPCOUNTER è usato come
								se fosse un Up Counter
								*/
								SWTC_Stop(PP_ENPHELAPSEDTIME_UPCOUNTER);								
								SWTC_SetSValue(PP_ENPHELAPSEDTIME_UPCOUNTER, INT8_T_MAX);
								Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
								uiSavedEnPhElapsedTime_Counter = 0;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */
								if ( ! SWTC_IsStopped(PP_TIMEBTWPHASES_COUNTER))
								{
									/* Disabilitare il TimeBtwPhases_Counter */
									Interpreter_Data[0].phase++;
									/* Ripristinare la fase al valore corrente */
									SWTC_Stop(PP_TIMEBTWPHASES_COUNTER);
								}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

							rejectedRegulation = FALSE;	  


							if( Interpreter_Data[0].completeLabel.s.label == CMPLT_LBL_END )
							{
#if defined (_ENABLE_SUPERVISOR_SKIP_WD_) && defined (__CLASSBCHECK_H)
							/* Class B certification */
								/*
								Se con la RESET_CONDITION si passa da END a
								PROGRAMMING viene richiesto il test del Watchdog
								*/
								classBCheckTestWD_EndCycle = TRUE;
#endif
							}


							else{
#ifdef _STATISTICS_ON_
								IncWordWithCheck(&MachineStatistics.globalStats.user_cycle_reset);
#endif
							}	

						}
						
						if( startFunctionValue == REG_RESET_FAULT )
						{/* Richiesto RESET di un fault */

							if( Interpreter_Data[0].completeLabel.s.label == LBL_FAULT )
							{/* Sono nel ciclo di FAULT  - test superfluo ?*/

								rejectedRegulation = FALSE;

								/*
								La macchina a stati dell'applicazione viene comunque resettata: sia che sia presente un fault di applicazione
								(da visualizzare o no), sia che la macchina a stati sia in uno stato precedente. In questo modo si evita che
								rimangano appese eventuali azioni di sicurezza (che magari erano state bloccate dal fault di piattaforma se
								presente).
								*/
								SPV_faultCompleteReset();

								/*
								Aggiornamento codice globale di fault: in questo modo, se è stato resettato un fault di applicazione,
								si può uscire dallo stato di FAULT direttamente in questa iterazione
								*/
								FLT_updateGlobalFaultCode();
							}
						}
						
						if(rejectedRegulation == FALSE)
						{
							Interpreter_Data[0].cycle = destinationCycle;
							Interpreter_Data[0].phase = SelectorFunctionValue;
							ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));

							/* Reset di alcune variabili */
							SPV_resetSensVariablesButDelay();
						}
					}
					else
					{
						/* 
						La presenza del ciclo di PROGRAMMING viene verificata ad ogni reset
						freddo ed in caso di assenza viene generato il fault file di setting.
						Dunque l'unica possibilità di rilevare l'assenza del ciclo durante
						l'esecuzione del Supervisore è che il file di setting si sia corrotto
						dopo l'ultimo reset freddo.Utilizzo la funzione che mi permette 
						di preservare le statistiche al RESET freddo.					
						*/
							
						SPV_generateColdReset();
					}
					
					break;

				case REG_RESTART_CONDITION:
				/* 
				DEA 602 implementa questa regolazione come Arkadia (stesse specifiche) 
				per eventuali nuove interfacce che la potrebbero utilizzare.
				In DEA 601 non era prevista.
				*/

					/* Equivale ad una RESET_CONDITION seguita da una START_CONDITION */

					if( (Interpreter_Data[0].completeLabel.s.label == LBL_RUN) &&
						(SelectorFunctionValue != StartedCycle) )
					{
						/*
						La REG_RESTART_CONDITION � accettata solo se si è in RUN e se il ciclo
						che si vuole avviare è diverso da quello in corso
						*/
						
						/* Operazioni da eseguire al RESET di un ciclo */
						SPV_resetSensVariablesButDelay();

						/** RESTART CICLO */					
					   	
						SPV_startCycle();
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
						Set_UpCounter(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
						Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
						SWTC_Run(PP_ENPHELAPSEDTIME_UPCOUNTER);
						SWTC_Run(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
	#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
						uiSavedEnPhElapsedTime_Counter = 0;
	#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */
#endif
					}
					else
					{
						rejectedRegulation = TRUE;
					}
					
					break;

				case REG_PAUSE_CONDITION:
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
					/* Stop timer */
					SWTC_Stop(PP_ENPHELAPSEDTIME_UPCOUNTER);
					SWTC_Stop(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);

					if ( ! SWTC_IsStopped(PP_TIMEBTWPHASES_COUNTER)) {
						/* Disabilitare il TimeBtwPhases_Counter */
						Interpreter_Data[0].phase++;
						/* Ripristinare la fase al valore corrente */
						SWTC_Stop(PP_TIMEBTWPHASES_COUNTER);
						index = SPV_getPowerProfileIndex(Interpreter_Data[0].cycle, Interpreter_Data[0].mSetting);
						ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																						  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
					}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT*/
#if ( PP_ENERGYPAUSE_STATUS == TRUE ) && ( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )
					if ( TRUE == SPV_goingToSleepManager() )
					{
						/* reset EnergyPause_Status */
						EnergyPause_Status = PP_ENERGYPAUSE_STATUS_NO_PAUSE;
					}
#else /* ( PP_ENERGYPAUSE_STATUS == TRUE ) && ( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE ) */

					/* Se il ciclo attivo è di tipo RUN o DELAY si va in PAUSE */
					(void)SPV_goingToSleepManager();
#endif

					break;
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
				case REG_OVERLOAD_PAUSE_CONDITION:
					 /*Stop timer*/
					SWTC_Stop(PP_ENPHELAPSEDTIME_UPCOUNTER);
					SWTC_Stop(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);

	#if ( PP_ENERGYPAUSE_STATUS == TRUE )
					if ( TRUE == SPV_goingToSleepManager() )
					{
						 /*Lock the door*/
		#if ( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )
						EnergyPause_Status &= PP_ENERGYPAUSE_STATUS_PAUSE_OVERLOAD;
		#else  /*( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )*/
						EnergyPause_Status = PP_ENERGYPAUSE_STATUS_PAUSE_OVERLOAD;
		#endif  /*( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )*/
					}
	#else  /*( PP_ENERGYPAUSE_STATUS == TRUE )*/
					 /*Se il ciclo attivo è di tipo RUN o DELAY si va in PAUSE*/
					(void)SPV_goingToSleepManager();
	#endif  /*( PP_ENERGYPAUSE_STATUS == TRUE )*/
					break;
				case REG_ENERGY_PHASES_PAUSE_CONDITION:
					 /*Stop timer*/
					SWTC_Stop(PP_ENPHELAPSEDTIME_UPCOUNTER);
					SWTC_Stop(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);

					if (( Interpreter_Data[0].completeLabel.s.label == LBL_RUN) || (Interpreter_Data[0].completeLabel.s.label == LBL_DELAY)) {
						 /*La regolazione ENERGY_PHASE_PAUSE non può essere accettata solo se
						   il timeout ENERGY_PHASE_PAUSE_TIMEOUT non è scaduto*/
						if ( ! SWTC_IsStopped(PP_TIMEBTWPHASES_COUNTER)) {
							/* Prima di cambiare stato (ciclo) si deve riportare la fase al valore corrente*/
							Interpreter_Data[0].phase++;
							SWTC_Stop(PP_TIMEBTWPHASES_COUNTER);
#if ( PP_ENERGYPAUSE_STATUS == TRUE )
							if ( TRUE == SPV_goingToSleepManager() )
							{
								/*Lock the door*/
#if ( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )
								EnergyPause_Status &= PP_ENERGYPAUSE_STATUS_PAUSE_EN_PHASES;
#else  /*( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )*/
								EnergyPause_Status = PP_ENERGYPAUSE_STATUS_PAUSE_EN_PHASES;
#endif  /*( PP_MULTIPLE_ENERGYPAUSE_STATUS == TRUE )*/
							}
#else  /*( PP_ENERGYPAUSE_STATUS == TRUE )*/
							 Se il ciclo attivo è di tipo RUN o DELAY si va in PAUSE
          (void)SPV_goingToSleepManager();
#endif  /*( PP_ENERGYPAUSE_STATUS == TRUE )*/
						}
					}
					break;
#endif /*CONFIG_PM_POWER_PROFILE_SUPPORT*/
				case REG_DONT_CARE:
				default:
					break;					
			}

		}		
		else if( retCode == REG_NULL_INDEX )
		{
			/* Gestire in modo particolare il valore della funzione (es. valore di default)? �_� */
		}
		else
		{
			/* Do nothing (keep BufOldReg[i] unchanged) */
			rejectedRegulation = TRUE;
		}

		if(rejectedRegulation == FALSE)
		{
			REG_updateOldReg((uint8_t)(FUNC_ID_START));
		}
	}
		
	
	/**	Aggiorno l'etichetta del ciclo che potrebbe essere cambiata	*/
	SPV_updateCycleLabel();
	

  /**
	======================================================================================  	
		CHECK DELAY TIMER                                                            
	======================================================================================
	*/  	

	retCode = REG_checkIfChanged((uint8_t)FUNC_ID_DELAY_HH);
	retCode |= REG_checkIfChanged((uint8_t)FUNC_ID_DELAY_MM);


	/* Check Delay Hours */
	if( (retCode & REG_CHANGED )&& ( (retCode & REG_NOT_EXIST)==0))
	{

		retCode = REG_getFunctionValueByID((uint8_t)(FUNC_ID_DELAY_HH), &tempFuncValue);
		retCode |= REG_getFunctionValueByID((uint8_t)(FUNC_ID_DELAY_MM), &tempFuncValue2);
		if( (retCode == S_OK )&& ((retCode & REG_NULL_INDEX)==0))
		{

			DelayInMinutes = (uint16_t)(tempFuncValue) * 60 + tempFuncValue2; /* aggiornamento delay in ore */

			/* Sincronizzazione del contatore secondi */
			SPV_setupDelayCounter();
			
			REG_updateOldReg((uint8_t)(FUNC_ID_DELAY_HH));
			REG_updateOldReg((uint8_t)(FUNC_ID_DELAY_MM));

		}
		else if( retCode == REG_NULL_INDEX )
		{
			REG_updateOldReg((uint8_t)(FUNC_ID_DELAY_HH));
			REG_updateOldReg((uint8_t)(FUNC_ID_DELAY_MM));
		}
		else
		{
			/* Do nothing (keep BufOldReg[i] unchanged) */
		}
	
	}
	

	/**
		======================================================================================
			CHECK ORARIO TIMER
		======================================================================================
		*/
	retCode = REG_checkIfChanged((uint8_t)FUNC_ID_CLOCK_HH);
	retCode |= REG_checkIfChanged((uint8_t)FUNC_ID_CLOCK_MM);

	if( (retCode & REG_CHANGED )&& ( (retCode & REG_NOT_EXIST)==0))
	{
		retCode = REG_getFunctionValueByID((uint8_t)(FUNC_ID_CLOCK_HH), &tempFuncValue);
		if( retCode == S_OK )
		{
			TempCycle_Stats.clockHH = tempFuncValue+1;
		}
		else if( retCode == REG_NULL_INDEX )
		{
			REG_updateOldReg((uint8_t)(FUNC_ID_CLOCK_HH));
			TempCycle_Stats.clockHH = DUMMY_CLOCK_VALUE;
		}
		retCodeAux = REG_getFunctionValueByID((uint8_t)(FUNC_ID_CLOCK_MM), &tempFuncValue2);
		if(retCodeAux == S_OK )
		{
			TempCycle_Stats.clockMM = tempFuncValue2+1;
		}
		else if( retCodeAux == REG_NULL_INDEX )
		{
			REG_updateOldReg((uint8_t)(FUNC_ID_CLOCK_MM));
			TempCycle_Stats.clockMM = DUMMY_CLOCK_VALUE;
		}
	}

	/**
	======================================================================================
		CHECK REG JOIN/LEAVE
	======================================================================================
	*/
	if( SPV_updateFunctionValue((uint8_t)(FUNC_ID_ENERGY_REMOTE_CONTROL), &Join_Leave_Reg) == S_FALSE)
	{
		Join_Leave_Reg = JOIN_LEAVE_REG_DFLT_VALUE;
	}


	/**
	======================================================================================  	
		CHECK TRANSIZIONI DELAY->RUN, RUN->DELAY                                                         
	======================================================================================
	*/  	

	if( Interpreter_Data[0].completeLabel.s.label == LBL_RUN ) 
	{
		if( DelayInMinutes != 0)
		{
			/** Vai in DELAY */ 
			if( SPV_searchCycleByLabel(CMPLT_LBL_DELAY, &destinationCycle) == S_OK )
			{
			
				/* Numero di partenze ritardate */
				IncWordWithCheck(&MachineStatistics.globalStats.delayedCycles_Num);


				/* Imposta Opcode e valore iniziale per Delay_Counter */
				SPV_setupDelayCounter();
				
				/** Cambio stato */
				Interpreter_Data[0].cycle = destinationCycle;
				Interpreter_Data[0].phase = StartedCycle;
				ITP_interpreterFlagsInit( (uint8_t)(ITP_FULL_INIT));

			}
			else
			{/* Recovery case */
				DelayInMinutes = 0;
			}			
		}
	}
	else if( Interpreter_Data[0].completeLabel.s.label == LBL_DELAY )
	{		
		if( DelayInMinutes == 0)
		{
			/** 
			Si Parte 
			*/
			retCode = REG_getFunctionValueByID((uint16_t)(FUNC_ID_CLOCK_HH), &tempFuncValue);
			if (retCode == S_OK)
			{
				/* Saving clock at the end of delay */
				MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayHH = tempFuncValue+1;
			}
			else
			{
				/* For safety reasons, clockMMEndDelay and
				 * clockHHEndDelay are assigned the value DUMMY_CLOCK_VALUE even if I
				 * have already brought the value of these variables to this value at
				 * the beginning of the cycle*/
				MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayHH = DUMMY_CLOCK_VALUE;
			}
			retCodeAux = REG_getFunctionValueByID((uint16_t)(FUNC_ID_CLOCK_MM), &tempFuncValue2);
			if( retCodeAux == S_OK )
			{
				MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayMM = tempFuncValue2+1;
			}
			else
			{
				/* For safety reasons, clockMMEndDelay and
				 * clockHHEndDelay are assigned the value DUMMY_CLOCK_VALUE even if I
				 * have already brought the value of these variables to this value at
				 * the beginning of the cycle*/
				MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].delayMM = DUMMY_CLOCK_VALUE;
			}

			Interpreter_Data[0].cycle = Interpreter_Data[0].phase;
			Interpreter_Data[0].phase = 0;
			ITP_interpreterFlagsInit( (uint8_t)(ITP_FULL_INIT));									
		}
	}
	else
	{
		/* do nothing */ 
	}
	

	/**	Aggiorno l'etichetta del ciclo che potrebbe essere cambiata	*/
	SPV_updateCycleLabel();

	/** Conteggio Delay */
	SPV_manageDelayCounters();


	/**
	======================================================================================  	
		IN ARKADIA: GESTIONE INGRESSO IN PAUSE A CAUSA DEL WARNING PORTA APERTA
		IN DEA602 PORTA APERTA GESTITA IN MODO AUTONOMO DAI UP E ALGORITMI,
	  ALCUNI DEVONO CONTINUARE A FUNZIONARE, VEDI SCARICO
	======================================================================================
	*/
	
	/**
	======================================================================================  	
		GESTIONE INGRESSO - PERMANENZA NELLO STATO DI FAULT                                                         
	======================================================================================
	*/  	

	if( CurrentFault[0].code != CODE_NO_FAULT )
	{

		if( Interpreter_Data[0].completeLabel.s.label != LBL_FAULT )
		{ /* sono in uno stato diverso da FAULT : INGRESSO in FAULT */

			/** Ingresso nello stato di FAULT */

			if( SPV_searchCycleByLabel(CMPLT_LBL_FAULT, &destinationCycle) == S_OK )
			{
						
#ifdef _STATISTICS_ON_
				/* Aggiornamento statistiche di fine ciclo */
				STC_updateLastCycleStat_END(1);
#endif 			/* _STATISTICS_ON_ */

				/** Va in FAULT */
				Interpreter_Data[0].cycle = destinationCycle;

				/* Fase: codice di allarme */
				Interpreter_Data[0].phase = CurrentFault[0].code;

				ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));									

				Interpreter_Data[0].completeLabel.b = CMPLT_LBL_FAULT;

				/** All'ingresso in FAULT viene resettato comunque il valore del delay + altre variabili */
				SPV_resetSensVariables();
			}
			else
			{
				/*
				La presenza del ciclo di FAULT viene verificata ad ogni reset
				freddo ed in caso di assenza viene generato il fault file di setting.
				Dunque l'unica possibilità di rilevare l'assenza del ciclo durante
				l'esecuzione del Supervisore è che il file di setting si sia corrotto
				dopo l'ultimo reset freddo, nel qual caso si presume che interverrebbe
				comunque la routine di certificazione a forzare un reset freddo.
				*/
				SPV_generateColdReset();
			}					
		}
		else
		{/* sono già in fault -> controllo se cambia il fault code per cambiare fase*/

			if ( Interpreter_Data[0].phase != CurrentFault[0].code )
			{/* fase diversa dal fault corrente: è cambiato il codice */

				Interpreter_Data[0].phase = CurrentFault[0].code;   /*aggiornamento fase*/

				ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));	

				SPV_resetSensVariables();
			}
		}

	}

	/*
	======================================================================================  	
		END GESTIONE INGRESSO NELLO STATO DI FAULT                                                                                                                  
	======================================================================================
	*/  

/**
Controllo funzioni utilizzate dall'Interprete 
*/  	

	/**
	======================================================================================  	
		CHECK CESTO SELEZIONATO                                                             
	======================================================================================
	*/  	
  
	if( SPV_updateFunctionValue((uint8_t)(FUNC_ID_BASKET), &DishRackFunctionValue) == S_FALSE)
	{
		DishRackFunctionValue = DISH_RACK_DFLT_VALUE;
	}


	/**
	======================================================================================  	
		CHECK DUREZZA ACQUA PER QUANTITA' ACQUA RIGENERE                                                             
	======================================================================================
	*/  	
  
	if( SPV_updateFunctionValue((uint8_t)(FUNC_ID_WATER_HARDNESS), &WaterForRegenerate) == S_FALSE)
	{
		WaterForRegenerate = REGENERATE_WATER_DFLT_VALUE;
	}

	/**
	======================================================================================  	
		CHECK ZONE WASH                                                            
	======================================================================================
	*/  	
  	/* nota : nel caso di voglia gestire il vecchio tipo di mezzocarico con l'alternato a 6 posizioni, si deve comunque definire questa 
	regolazione e inserire le due tabelle corrispondenti al mezzo carico voluto.*/
	
	if( SPV_updateFunctionValue((uint8_t)(FUNC_ID_ZONE_WASH), &ZoneWashFunctionValue) == S_FALSE)
	{
		ZoneWashFunctionValue = ZONE_WASH_DFLT_VALUE;
	}
	
	/**
	======================================================================================  	
		CHECK ZONE RINSE AID                                                             
	======================================================================================
	*/  	
  	/* per rilascio brillantante*/
	
	if( SPV_updateFunctionValue((uint8_t)(FUNC_ID_RINSE_AID), &RinseAidFunctionValue) == S_FALSE)
	{
		RinseAidFunctionValue = RINSE_AID_DFLT_VALUE;
	}

	
	/**
	======================================================================================  	
		CARICAMENTO PUNTATORI PER PARAMETRI FAULT (UTILIZZATI IN UP DRAIN E DW_FAULT)                                                            
	======================================================================================
	*/
	
	/* Start ciclo di Autotest - va processato qui perch� ho aggiornato la Label */
	if ( started_cycle == TRUE )
	{  
	    if ( Interpreter_Data[0].completeLabel.s.subLabel == SUBL_AUTOTEST )
	    {
	      /* Reset var Autotest, devono essere applicate i parametri Fault per Autotest */
	      FLT_faultMgmtDataReset();  
	    }
	}

#if (VIRTUAL_SENSOR_ENABLE == 1)
	if( (uP_AltCtrl_Data.SV_invalid_4alt == TRUE) || !(TESTBIT (GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))) )
	{
		Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_WASH]=0xFF;
	}
	else
	{
		Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_WASH]=Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH];
	}

    
	if(!(TESTBIT (GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))) )
	{
		Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_DRAIN]=0xFF;
	}
	else
	{
		Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_DRAIN]=Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN];
	}
	
#endif
	/**
	=====================
	 CHIAMATA INTERPRETE 
	=====================	
	*/
	interpreterResult = ITP_programFlowInterpreter();
	
	if((interpreterResult & ERR_CODE) == 0)
	{
		/**	Aggiorno l'etichetta del ciclo che potrebbe essere cambiata	(a causa di un JumpIf) */
		SPV_updateCycleLabel();

		#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
/* MT TODO: verificare se vanno bene così */
		if(Interpreter_Data[0].completeLabel.s.label == LBL_RUN) {
			/* MT gives the updated status of the counter to the Platform library */
			PM_PowerProfile_SetDuration(1, (PM_PowerProfile_DurationType)SWTC_GetElapsedTime(PP_ENPHELAPSEDTIME_UPCOUNTER));

			PM_PowerProfile_SetForceReload(1);
		}
		else
		{
			PM_PowerProfile_ResetForceReload(1);
		}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

				
		switch( interpreterResult )
		{

			case END_PHASE:
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT

        index = SPV_getPowerProfileIndex(Interpreter_Data[0].cycle, Interpreter_Data[0].mSetting);

				if(Interpreter_Data[0].completeLabel.s.label == LBL_RUN) {
					/* Modifica per la gestione Delay tra le Fasi Energetiche */
					if ( ! SWTC_IsStopped(PP_TIMEBTWPHASES_COUNTER)) {
						if (SWTC_IsExpired(PP_TIMEBTWPHASES_COUNTER)) {
							/* Timer esaurito e la ENERGY_PHASE_PAUSE non è stata ricevuta
								--> la fase ritorna quella corrente. */
							Interpreter_Data[0].phase++;
							SWTC_Stop(PP_TIMEBTWPHASES_COUNTER);
							ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																							  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
						}
#if (( MAINTAIN_LOADS_AFTER_PHASE == TRUE ) && defined(CONFIG_PM_POWER_PROFILE_SUPPORT))

						/*else*/
						/* Always mantain loads if TimeBtwPhases_Counter is enabled */
						{
							#if (MULTI_COMPARTMENT_APPL == 1)
							ITP_loadsMgmt(compartment);
							#else
							ITP_loadsMgmt();
							#endif
						}
#endif /* #if (( MAINTAIN_LOADS_AFTER_PHASE == TRUE ) && defined(CONFIG_PM_POWER_PROFILE_SUPPORT)) */
					}
					else {
						bool_t newEnergyPhase = FALSE;
						uint16_t maxActivationDelay = 0x0000;

						/* La nuova fase funzionale è l'inizio di una nuova fase energetica?  */
						if (PM_PowerProfile_GetEnergyPhaseMaxActivationDelay
										(/*powerProfileID*/ 1,
										 /*PM_PowerProfile_ProfileIndex*/ index,
										 /*cycle Interpreter_Data[0].cycle,*/
										 /*functionalPhase*/Interpreter_Data[0].phase,
										 /*mSetting*/Interpreter_Data[0].mSetting,
										 /*pNewEnergyPhase*/&newEnergyPhase,
										 /*pMaxActivationDelay*/&maxActivationDelay) != POWER_PROFILE_NO_ERR)
						{
							;/* ERROR */
						}
						else {
							if (newEnergyPhase == TRUE) {
								/* Start del Timer per contare il tempo trascorso nella fase energetica corrente */

								Set_UpCounter(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
								Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
	              				SWTC_Run(PP_ENPHELAPSEDTIME_UPCOUNTER);
	              				SWTC_Run(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
								uiSavedEnPhElapsedTime_Counter = 0;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */


								if (maxActivationDelay != 0) {
									ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																									  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_ON, ApplPowerProfileStatus);
									/* Si forza la fase alla fase precedente per continuare con la visualizzazione
									   precedente e inizializzo timer tra le fasi */
									Interpreter_Data[0].phase--;
									/* Start del Timer per la finestra temporale di accettazione comand EnergyPhasePause */
									Set_UpCounter(SHORT_COUNTER, PP_TIMEBTWPHASES_COUNTER, COUNTERS2_TBASE_1MIN); /*AE TODO verificare WM è Down Counter*/
									SWTC_Run(PP_TIMEBTWPHASES_COUNTER);
								}
								else {
									ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																									  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
								}
							}
						}
					}
				}
				else
#else /* CONFIG_PM_POWER_PROFILE_SUPPORT */
				
				/*In RUN e FAULT non posso reinizializzare la fase. In RUN il ciclo procede con la fase successiva,
				  in FAULT il comportamento viene imposto da setting */
				if( (Interpreter_Data[0].completeLabel.s.label != LBL_RUN) && (Interpreter_Data[0].completeLabel.s.label != LBL_FAULT) )
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
				{

					/** Nei cicli sa
						fase, che va ogni volta reinizialinon di RUN e non di FAULT l'Interprete deve ciclare sempre sulla steszzata */
							
					Interpreter_Data[0].phase --;
					ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));									
				}

			break;

			case END_CYCLE:
			case END_CYCLE_RESET:
				/** Ciclo terminato naturalmente (include anche il caso di ultima fase del
					ciclo terminata) o incontrata una condizione di reset ciclo. */
				if( Interpreter_Data[0].completeLabel.s.label != LBL_RUN )
				{		
				  if (Interpreter_Data[0].completeLabel.s.label != LBL_FAULT)
				  {
  					/** Nei cicli non di RUN e non di FAULT l'Interprete deve ciclare sempre sulla stessa
  						fase, che va ogni volta reinizializzata */
  					ITP_interpreterFlagsInit( (uint8_t)(ITP_FULL_INIT));
				  }
				  
				  /* else : nel caso di FAULT non modifico nulla (non si dovrebbe mai verificare) */ 
				}
				else
				{/* RUN -> END */
					
					if( SPV_searchCycleByLabel(CMPLT_LBL_END, &destinationCycle) == S_OK )
					{
#ifdef _STATISTICS_ON_
						/* Aggiornamento statistiche di fine ciclo */
						STC_updateLastCycleStat_END(0);
#endif /* _STATISTICS_ON_ */
             															 
			            /* cancellazione Last Fault in END Autotest */  
			            if( Interpreter_Data[0].completeLabel.s.subLabel == SUBL_AUTOTEST )
			            {
			                FLT_lastFaultUpdate(No_Fault, &MachineStatistics.lastFault[0]);
			            }

#if defined(SOLUTION_1_FOR_REF_CYCLE) || defined(SOLUTION_2_FOR_REF_CYCLE)

						Interpreter_Data[0].phase = StartedCycle;

#elif defined(SOLUTION_3_FOR_REF_CYCLE)

						Interpreter_Data[0].phase = Interpreter_Data[0].cycle;

#endif

					    Interpreter_Data[0].cycle = destinationCycle;
						ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));										

					    Interpreter_Data[0].completeLabel.b = CMPLT_LBL_END;
							    
			            SPV_endCycleResetVars();

#if defined(SOLUTION_2_FOR_REF_CYCLE)

#endif						
					}
					else
					{
						/* 
						La presenza del ciclo di END viene verificata ad ogni reset
						freddo ed in caso di assenza viene generato il fault file di setting.
						Dunque l'unica possibilità di rilevare l'assenza del ciclo durante
						l'esecuzione del Supervisore è che il file di setting si sia corrotto
						dopo l'ultimo reset freddo. Utilizzo la funzione che mi permette di 
						preservare le statistiche al reset freddo.
						*/
					  
						SPV_generateColdReset(); /*ForceColdReset();*/
					}
						
				}
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
				/* Reset counter */
	      /*  StopCounterToZero(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER); */
	      /*  StopCounterToZero(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER); */
				/*
				MQ PA TODO Secondo me è corretto come sotto, dato PP_ENPHELAPSEDTIME_UPCOUNTER è usato come
				se fosse un Up Counter
				*/
        SWTC_Stop(PP_ENPHELAPSEDTIME_UPCOUNTER);
        SWTC_Stop(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
        SWTC_SetSValue(PP_ENPHELAPSEDTIME_UPCOUNTER, INT8_T_MAX);

#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
				uiSavedEnPhElapsedTime_Counter = 0;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */		
			break;
			
			case PROGRAM_FLOW_BREAK:
			/* JumpIf */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT

				/* Controlla che il JumpIF sia verso un mSetting/Ciclo/Fase
				 * diverso da quello di origine. Nel caso contrario non devono
				 * essere agggiornate le informazioni riguardo l'APP
				 */
				if (( bkp_CompItpDataCycle == Interpreter_Data[0].cycle ) &&
					( bkp_CompItpDataPhase == Interpreter_Data[0].phase ) &&
					( bkp_CompItpDatamSetting == Interpreter_Data[0].mSetting ))
				{
					/* Do nothing */
				}
				else
				{
					index = SPV_getPowerProfileIndex(Interpreter_Data[0].cycle, Interpreter_Data[0].mSetting);

					if(Interpreter_Data[0].completeLabel.s.label == LBL_RUN) {
						/* Modifica per la gestione Delay tra le Fasi Energetiche */
						if ( ! SWTC_IsStopped(PP_TIMEBTWPHASES_COUNTER)) {
							if (SWTC_IsExpired(PP_TIMEBTWPHASES_COUNTER)) {
								/* Timer esaurito e la ENERGY_PHASE_PAUSE non è stata ricevuta
								   --> la fase ritorna quella corrente. */
								Interpreter_Data[0].phase++;
								SWTC_Stop(PP_TIMEBTWPHASES_COUNTER);
								ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																								  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
							}
						}
						else {
							bool_t newEnergyPhase;
							uint16_t maxActivationDelay;

							/* La nuova fase funzionale è l'inizio di una nuova fase energetica?  */
							if (PM_PowerProfile_GetEnergyPhaseMaxActivationDelay
						(/*powerProfileID*/ 1,
						 /*PM_PowerProfile_ProfileIndex*/ index,
						 /*cycle Interpreter_Data[0].cycle,*/
											 /*functionalPhase*/Interpreter_Data[0].phase,
											 /*mSetting*/Interpreter_Data[0].mSetting,
											 /*pNewEnergyPhase*/&newEnergyPhase,
											 /*pMaxActivationDelay*/&maxActivationDelay) != POWER_PROFILE_NO_ERR)
							{
								;/* ERROR */
							}
							else {
								if (newEnergyPhase == TRUE) {
									/* Start del Timer per contare il tempo trascorso nella fase energetica corrente */
									Set_UpCounter(SHORT_COUNTER, PP_ENPHELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
									Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
									SWTC_Run(PP_ENPHELAPSEDTIME_UPCOUNTER);
								    SWTC_Run(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
									uiSavedEnPhElapsedTime_Counter = 0;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */


									if (maxActivationDelay != 0) {
										ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																										  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_ON, ApplPowerProfileStatus);
										/* Si forza la fase alla fase precedente per continuare con la visualizzazione
										   precedente e inizializzo timer tra le fasi */
										Interpreter_Data[0].cycle = bkp_CompItpDataCycle;
										Interpreter_Data[0].phase = bkp_CompItpDataPhase;
										Interpreter_Data[0].mSetting= bkp_CompItpDatamSetting;
										Interpreter_Data[0].first_statement = bkp_CompItpDataFirstStatement;
										/*Set_DownCounter(SHORT_COUNTER, PP_TIMEBTWPHASES_COUNTER, (uint8_t)COUNTERS2_TBASE_1SEC, ENERGY_PHASE_PAUSE_TIMEOUT);*/
										SWTC_Run(PP_TIMEBTWPHASES_COUNTER);
									}
									else {
										ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, index, Interpreter_Data[0].phase, Interpreter_Data[0].mSetting,
																										  FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
									}
								}
							}
						}
					}
				}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

			break;		

			case END_STEP:
			/** Nothing to do */

			case SAME_STEP:
			/* Nothing to do */
			
			default:

			break;		
		}

	}
	else
	{
		/* Resettare o dichiarare il fault file di setting? �_� */
		SPV_generateColdReset();
	}					


#ifdef _STATISTICS_ON_
	/** Aggiornamento statistiche legate allo stato */
	STC_superVisorUpdateStatistics();
#endif


	/*
	======================================================================================  	
		THE LAST BUT NOT THE LEAST
	======================================================================================
	*/

    /* Aggiorna BufOldReg[i] per le regolazioni 'To Main' non esplicitamente processate in precedenza */
    REG_updateFuncToMainOldReg();
    
    /* Aggiorna BufOldReg per le regolazioni non 'To Main' */
	memcpy(&BufOldReg[FUNC_NUM_TO_MAIN], &BufNewReg[FUNC_NUM_TO_MAIN], (uint8_t)(FUNC_NUM_ALL - FUNC_NUM_TO_MAIN));




#if defined (_ENABLE_SUPERVISOR_SKIP_WD_) && defined (__CLASSBCHECK_H)

	if (classBCheckTestWD_EndCycle == TRUE)
	{
		Set_SkipWD();
	}

#endif /* defined (_ENABLE_SUPERVISOR_SKIP_WD_) && defined (__CLASSBCHECK_H) */

#if ( PP_ENERGYPAUSE_STATUS == TRUE)
	if (( Interpreter_Data[0].completeLabel.s.label == LBL_RUN )
	    || ( Interpreter_Data[0].completeLabel.s.label == LBL_DELAY )
	    || ( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING )
	    || ( Interpreter_Data[0].completeLabel.s.label == LBL_FAULT )
	    || ( Interpreter_Data[0].completeLabel.s.label == LBL_END )
	)
	{
		EnergyPause_Status=PP_ENERGYPAUSE_STATUS_NO_PAUSE;
	}
#endif /* ( PP_ENERGYPAUSE_STATUS == TRUE) */

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
		SPV_updatePowerProfile(oldLabel, oldPhase);

		SPV_updateTimeToEnd();
#endif

}/*end supervisor*/




/**
Ricerca l'intestazione del ciclo Interpreter_Data[0].cycle e ne memorizza l'etichetta in
Interpreter_Data[0].completeLabel.
 
\param [in/out] Interpreter_Data[0] (global)
 
\return 		none

\notes:			le Label sono	-LBL_PROGRAMMING
								-LBL_RUN
								-LBL_DELAY
								-LBL_END
								-LBL_PAUSE
								-LBL_FAULT
				In caso di incoerenza nell'intestazione del ciclo viene autogenerato un 
				reset freddo
 
\version		1.2.0
\date 			15/05/2007
\author			Marco Quintini
*/
static void SPV_updateCycleLabel(void)
{
const uint8_t *pCycleHeader;
	
	if( SPV_getCycleHeaderPtr(Interpreter_Data[0].cycle, &pCycleHeader) == S_OK)
	{
		Interpreter_Data[0].completeLabel.b = ( OFFS_B(pCycleHeader, CYCLE_LABEL_OFFSET) & (CYCLE_SUBLABEL_MASK|CYCLE_LABEL_MASK) );
	}
	else
	{
		/* Dati incoerenti nell'intestazione del ciclo, viene autogenerato un reset freddo */
		SPV_generateColdReset();
	}
}

/**
Inizializza un puntatore all'intestazione del ciclo Cycle.
 
\param [in] 	Cycle è il ciclo di cui si ricerca l'intestazione
\param [out] 	pCycleHeading punta all'intestazione del ciclo
 
\return 		S_OK se la funzione è stata eseguita correttamente, un codice d'errore altrimenti..

\notes:

\version		0.0.1
\date 			31/05/2006
\author			Marco Quintini
*/
uint8_t SPV_getCycleHeaderPtr(uint8_t cycle, const uint8_t **pCycleHeading)
{
Loader_Struct prgHeader;
	
		
	if( BasicLoader((uint16_t) PTR_PROGRAM_HEADING, cycle, &prgHeader ) != S_OK )
	{
		return SPV_ERR_BASIC_LOADER; 
	}
	if( prgHeader.DataLenght < PROGRAM_HEADING_ITEM_SIZE )
	/*In dea 700 è != io metto minore tanto i cicli normali hanno due mentre app a 4 come header del ciclo*/
	{
		return SPV_ERR_PRG_HEADING_SIZE; 
	}
	
	*pCycleHeading = prgHeader.Data;

	return S_OK; 
}

/**
Restituisce il primo ciclo nella cui intestazione compare l'etichetta completeLabel.
 
\param [in] 	completeLabel è l'etichetta (label+subLabel) che viene cercata nelle intestazioni
				dei cicli.
\param [out] 	*pCycle è il ciclo che ha la completeLabel specificata.
 
\return 		S_OK se il ciclo viene trovato, SPV_WRN_LABEL_NOT_FOUND se la funzione NON
				è stata eseguita correttamente o se non è presente alcun ciclo con la
				completeLabel specificata.

\notes:			non invertire il ciclo for per ottimizzare, non restituirebbe più il primo
				ciclo ma l'ultimo.

\version		1.1.0
\date 			15/05/2007
\author			Marco Quintini
*/
uint8_t SPV_searchCycleByLabel(uint8_t completeLabel, uint8_t *pCycle)
{
uint8_t i;

	for( i=0; i< CYCLES_NUM; i++ )
	{		
		if(SPV_testCycleLabel(i, completeLabel) == TRUE)
		{
	    	*pCycle = i;
	    	return S_OK; 	
		}
	}
	
	return SPV_WRN_LABEL_NOT_FOUND; 
}

/**
Restituisce il ciclo di riferimento per il calcolo del valore di una funzione.
 
\param [in]		Interpreter_Data[0] (global)
 
\return 		il ciclo di riferimento.

\notes:

\version		0.0.1
\date 			06/06/2006
\author			Marco Quintini
*/
uint8_t SPV_getRefCycleForFuncValue(void)
{		
#if defined(SOLUTION_1_FOR_REF_CYCLE)

	if( Interpreter_Data[0].completeLabel.s.label == LBL_RUN )
	{
		/* Potrebbe differire dal ciclo in caso di Jump ad un ciclo diverso da quello di partenza. */
		return (StartedCycle);
	}
	else if( Interpreter_Data[0].completeLabel.s.label == LBL_FAULT )
	{
		return (Interpreter_Data[0].cycle);
	}
	else
	{
		/* PROGRAMMING, DELAY, PAUSE, END. In DELAY, PAUSE ed END coincide con StartedCycle */
		return (Interpreter_Data[0].phase);
	}

#elif defined(SOLUTION_2_FOR_REF_CYCLE)		/* come DEA601 con precisazione per PAUSE */

	if( ( Interpreter_Data[0].completeLabel.s.label == LBL_RUN ) || ( Interpreter_Data[0].completeLabel.s.label == LBL_FAULT ) )
	{
		/* In RUN potrebbe differire dal ciclo di partenza in caso di Jump ad un ciclo diverso. */
		return (Interpreter_Data[0].cycle);
	}
	else if( Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE )                                                                  
	{/* questa precisazione non è presente in DEA601, ma è corretto farla */
		if(Paused_Intpt_Data[0].completeLabel.s.label == LBL_RUN)
		{
			/* Pausa da RUN */
			/* Potrebbe differire dalla fase se la pausa interviene quando è in esecuzione un ciclo
			   diverso da quello di partenza (a causa di un Jump a ciclo). */
			return (Paused_Intpt_Data[0].cycle);
		}
		else
		{
			/* Pausa da DELAY */
			return (Paused_Intpt_Data[0].phase);
		}
	}
	else
	{
		/* PROGRAMMING, DELAY, END */
		/* In effetti, nel passaggio RUN->END si potrebbe introdurre una discontinuità nel ciclo di
		   riferimento se il ciclo di RUN che termina, a causa di un Jump a ciclo, è diverso da
		   quello di partenza */
		return (Interpreter_Data[0].phase);
	}

#elif defined(SOLUTION_3_FOR_REF_CYCLE)

	if( ( Interpreter_Data[0].completeLabel.s.label == LBL_RUN ) || ( Interpreter_Data[0].completeLabel.s.label == LBL_FAULT ) )
	{
		/* In RUN potrebbe differire dal ciclo di partenza in caso di Jump ad un ciclo diverso. */
		return (Interpreter_Data[0].cycle);
	}
	else
	{
		/* PROGRAMMING, DELAY, PAUSE, END */
		return (Interpreter_Data[0].phase);
	}

#endif	/* SOLUTION_X_FOR_REF_CYCLE */	
}


/* *************** */
/* Local Functions */
/* *************** */

/**
Salva lo stato corrente dell'Interpreter nella struttura specificata.
 
\param [in]		Interpreter_Data[0] (global)
\param [in]		*pDestinationBuffer è la struttura destinazione in cui salvare lo stato
 
\return 		none

\notes:

\version		0.0.1
\date 			19/06/2006
\author			Marco Quintini
*/
static void SPV_saveIntptState(Interpreter_Struct *pDestinationBuffer)
{
	*pDestinationBuffer = Interpreter_Data[0];
	
	/* salva la struttura del JumpIF valido per Fase del ciclo di provenienza */
	Paused_JmpPhase[0] =  JmpPhase[0];
}

/**
Ripristina lo stato corrente dell'Interpreter copiandolo dalla struttura specificata.
 
\param [in]		Interpreter_Data[0] (global)
\param [in]		*pSourceBuffer � la struttura sorgente da cui copiare lo stato
 
\return 		none

\notes:

\version		0.0.1
\date 			19/06/2006
\author			Marco Quintini
*/
static void SPV_restoreIntptState(const Interpreter_Struct *pSourceBuffer)
{
	Interpreter_Data[0] = *pSourceBuffer;
	
	/* ripristina la struttura del JumpIF valido per Fase del ciclo di provenienza */
	JmpPhase[0] = Paused_JmpPhase[0];
	
	/* per preservare il JumpIF valido per fase se presente */
	Interpreter_Data[0].jumpIF_phase_ctrl = 1;
}	
				
/**
Arresta i contatori usati nei microprogrammi in corrispondenza di una PAUSE_CONDITION.
 
\param [in]		
 
\return 		

\notes:

\version		0.0.1
\date 			16/04/2008
\author			Valeria Catani
*/
static void SPV_stopMicroprogVars(void)
{

  StopMicroprogCounters();
  
  /*uP Alternato */
  if (uP_AltCtrl_Data.step == ALT_CTRL_WAIT_ALTPOS_EXE)
  {
    /* posizionamento del motorino alternato in corso re-inizializzare la procedura */
    uP_AltCtrl_Data.step= ALT_CTRL_BEGIN;
  }
  
  /*uP Sparkling Aid*/
  if( (uP_SparklingAid_Data.step== SPRKLING_T1 ) || (uP_SparklingAid_Data.step == SPRKLING_T2) )
  {
    uP_SparklingAid_Data.cmd.RepeatTurbRead = TRUE;
    
  }

}


/**
Imposta il contatore per il delay, distinguendo i casi di DEMO attiva e no.
Il valore del timer viene posto a 0.

\param [out] 	SPV_DELAY_TIMER_S_COUNTER(global)

\return			none 

\author 		Valeria Catani
\date			02/07/2008
\version		1.0.0
*/
void SPV_setupDelayCounter(void)
{
	if (SPV_testRefCycleLabel(CMPLT_LBL_DEMO) == TRUE)
	{
		
		(void)Set_SmallCounter((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)1);
	}
	else
	{
		
		(void)Set_SmallCounter((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)60);
	}
}
  

/**
Gestisce i contatori utilizzati per il Delay e aggiorna le variabili relative 
alla gestione del delay

\param [in]		Delay(global)
\param [in]		SPV_DELAY_TIMER_S_COUNTER (global)
	 
\return 		none
\warning		none

\notes:			

\author			Valeria Catani
\date   		02/07/2008
\version    	1.0.0

*/
static void SPV_manageDelayCounters(void)
{ 
	if( (Interpreter_Data[0].completeLabel.s.label == LBL_DELAY) ||
	((Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE) && (Paused_Intpt_Data[0].completeLabel.s.label == LBL_DELAY)) )
	{
		/* Il delay scorre in DELAY e in PAUSE da DELAY, indipendentemente dallo stato della porta */

		SWTC_Run((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER);
	}
	else
	{
		SWTC_Stop((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER);
	}

	if ( SWTC_IsExpired((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER))
	{
		/* One minute elapsed */

		if( DelayInMinutes )
		{
			DelayInMinutes --;
			SPV_setupDelayCounter();
		}
		else
		{/* Hours = 0; Minutes = 0; */

				(void)SWTC_Stop((SWTC_CountersType)SPV_DELAY_TIMER_S_COUNTER);
		}
	}

}
  
  

/**
Verifica se la label del ciclo cycle coincide con completeLabel.

\param [in] 	cycle è il ciclo di cui si vuole verificare la label.
\param [in] 	completeLabel è la label completa (label+sublabel) con cui confrontare
				quella del ciclo cycle.

\return			TRUE se la label coincide, FALSE se non coincide o in caso di errore nella
				ricerca dell'intestazione del ciclo.

\author 		Marco Quintini
\date			15/05/2007
\version		1.0.0
*/
static bool_t SPV_testCycleLabel(uint8_t cycle, uint8_t completeLabel)
{
const uint8_t *pCycleHeader;

	if( SPV_getCycleHeaderPtr(cycle, &pCycleHeader) == S_OK )
	{
		if( (OFFS_B(pCycleHeader, CYCLE_LABEL_OFFSET) & (CYCLE_SUBLABEL_MASK|CYCLE_LABEL_MASK)) == completeLabel)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**
Verifica se la label del ciclo di riferimento (che in generale non è Interpreter_Data[0].cycle,
ad esempio in PROGRAMMING, DELAY, PAUSE, END è Interpreter_Data[0].phase) coincide con
completeLabel.

\param [in] 	completeLabel è la label completa (label+sublabel) con cui confrontare
				quella del ciclo di riferimento.

\return			TRUE se la label coincide, FALSE se non coincide o in caso di errore nella
				ricerca dell'intestazione del ciclo.

\author 		Marco Quintini
\date			15/05/2007
\version		1.1.0
*/
static bool_t SPV_testRefCycleLabel(uint8_t completeLabel)
{
uint8_t refCycle;
/*const uint8_t *pCycleHeader;*/

	refCycle = SPV_getRefCycleForFuncValue();

	if(SPV_testCycleLabel(refCycle, completeLabel) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

/**
Verifica se c'è un ciclo in corso (ovvero se lo stato è RUN, PAUSE o DELAY).

\param [in] 	Interpreter_Data[0] (global)

\return			TRUE se c'è un ciclo in corso, FALSE altrimenti

\author 		Marco Quintini
\date			  18/06/2007
\version		1.0.0
*/
bool_t SPV_isCycleRunning(void)
{
	if( (Interpreter_Data[0].completeLabel.s.label == LBL_RUN) ||
		(Interpreter_Data[0].completeLabel.s.label == LBL_DELAY) ||
		(Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
Verifica se è in corso (ovvero se è in RUN, PAUSE o DELAY) il ciclo che ha come
etichetta completa completeLabel.

\param [in] 	completeLabel è l'etichetta completa (label+sublabel) del ciclo
				da controllare
\param [in] 	Interpreter_Data[0] (global)

\return			TRUE se c'è il ciclo in corso, FALSE altrimenti

\author 		Marco Quintini
\date			31/07/2007
\version		1.0.0
*/
static bool_t SPV_isCycleWithLabelRunning(uint8_t completeLabel)
{

	if( (SPV_testRefCycleLabel(completeLabel) == TRUE) && 
		(SPV_isCycleRunning() == TRUE ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}




/**
Verifica è in corso il ciclo di Autotest (controllando anche se è in DELAY o in PAUSE).

\param [in] 	Interpreter_Data[0] (global)

\return			TRUE se è in corso il ciclo di Autotest, FALSE altrimenti

\author 		Marco Quintini
\date			16/05/2007
\version		1.0.0
*/
bool_t SPV_isAutotestRunning(void)
{
	return (SPV_isCycleWithLabelRunning(CMPLT_LBL_AUTOTEST));
}

/**
Verifica è in corso il ciclo di Autotest o di Collaudo 
(controllando anche se è in DELAY o in PAUSE -> PER "COMPLETEZZA").

\param [in] 	Interpreter_Data[0] (global)

\return			TRUE se è in corso il ciclo di Autotest (o Collaudo), FALSE altrimenti

\author 		Valeria Catani (Marco Quintini)
\date			  24/09/2008
\version		1.0.0
*/
bool_t SPV_isAutotestOrTestRunning(void)
{
	
	return ( SPV_isCycleWithLabelRunning(CMPLT_LBL_AUTOTEST) || 
	         SPV_isCycleWithLabelRunning(CMPLT_LBL_COLLAUDO) );        
}




/**
Processa i comandi di attivazione dei cicli di COLLAUDO e AUTOTEST da remoto.

\param [in,out]	ExternalRequest (global) è la modalità speciale di funzionamento richiesta
\param [in,out] InterpreterRefData (global)
\param [out] ApplicationState (global)
	 
\return     	none

\notes:			

\author			Marco Quintini,Roberto Fioravanti
\date   		23/09/2008
\version    	1.0.1

*/
void SPV_manageRequestFromRemote( void )
{
uint8_t requestedCycleCompleteLabel;
uint8_t destinationCycle;
    

    /* Aggiornamento ApplicationState */
    if( Interpreter_Data[0].completeLabel.s.subLabel == SUBL_AUTOTEST )
	{		
		SETBIT(ApplicationState, AS_AUTOTEST_ON_BIT);	  
	}
	else
	{
	    CLRBIT(ApplicationState, AS_AUTOTEST_ON_BIT);
	}
	
	if( Interpreter_Data[0].completeLabel.s.subLabel == SUBL_COLLAUDO )
	{
	    SETBIT(ApplicationState, AS_COLLAUDO_ON_BIT);	
	}
	else
	{
		CLRBIT(ApplicationState, AS_COLLAUDO_ON_BIT);		    
	}
    
    /* ExternalRequest processing */
	if( ExternalRequest == SYSTEM_NORMAL_AUTOTEST_MODE )
	{
		requestedCycleCompleteLabel = CMPLT_LBL_AUTOTEST;
	}
	else if( ExternalRequest == SYSTEM_NORMAL_COLLAUDO_MODE )
	{
		requestedCycleCompleteLabel = CMPLT_LBL_COLLAUDO;				
	}
	else
	{	    
		return;
	}

	/** AUTOTEST e COLLAUDO sono avviabili solo da PROGRAMMING e da END. In
		altre condizioni eventuali richieste di attivazione sono annullate */
	if((Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING) || (Interpreter_Data[0].completeLabel.s.label == LBL_END))
	{
		if( SPV_searchCycleByLabel(requestedCycleCompleteLabel, &destinationCycle) == S_OK )
		{
			StartedCycle = destinationCycle;
			Interpreter_Data[0].cycle = destinationCycle;
			Interpreter_Data[0].phase = 0;

			ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));
									  
			SPV_startCycleInit();

			/* Aggiorno la label come ultimo passo, così in caso di reset caldo posso
			   riprocessare la richiesta */
			Interpreter_Data[0].completeLabel.b = requestedCycleCompleteLabel;
		}
		else
		{
			/* Do nothing */
		}
	}
	else
	{
		/* Do nothing */
	}			
	
	ExternalRequest = 0;
}


/**
Gestisce il passaggio DELAY/RUN --> PAUSE.

\param [in,out]	InterpreterRefData (global)
\param [out] 	Paused_Intpt_Data (global)
	 
\return     	TRUE se il ciclo di partenza è RUN o DELAY ed il ciclo PAUSE esiste, FALSE altrimenti

\notes:			

\author			Marco Quintini
\date   		10/10/2006
\version    	1.0.0

*/
bool_t SPV_goingToSleepManager(void)
{
uint8_t destinationCycle;

	if( ( Interpreter_Data[0].completeLabel.s.label == LBL_RUN ) || ( Interpreter_Data[0].completeLabel.s.label == LBL_DELAY ) )
	{
		/* Passa al ciclo PAUSE */

		if( SPV_searchCycleByLabel(CMPLT_LBL_PAUSE, &destinationCycle) == S_OK )
		{   
			SPV_stopMicroprogVars();
			
			/** Salva lo stato corrente */
			SPV_saveIntptState(&Paused_Intpt_Data[0]);

#if defined(SOLUTION_1_FOR_REF_CYCLE) || defined(SOLUTION_2_FOR_REF_CYCLE)

			Interpreter_Data[0].phase = StartedCycle;
		                                        
		  
#elif defined(SOLUTION_3_FOR_REF_CYCLE)

			if( Interpreter_Data[0].completeLabel.s.label == LBL_RUN )
			{	
				/** Interrotto un ciclo RUN */
				Interpreter_Data[0].phase = Interpreter_Data[0].cycle;
			}
			else
			{	
				/** Interrotto un ciclo DELAY */
				/* Non faccio niente, la fase rimane la stessa */
			}
#endif
		    Interpreter_Data[0].cycle = destinationCycle;

			ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));													

		    Interpreter_Data[0].completeLabel.b = CMPLT_LBL_PAUSE;
		
			return (TRUE);
		}

	}
	
	return (FALSE);
}
/**/



/**
Resetta alcune variabili sensibili del Supervisor in coincidenza di una 
RESET CONDITION o di una REG_RESTART_CONDITION.

\param [out] 	Flag_levTOtimeRef_update (global)
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		27/11/2006
\version    	1.0.0

*/

static void SPV_resetSensVariablesButDelay(void)
{
  Flag_levTOtimeRef_update = FALSE;
  
  /* disabilitazione timeout di caricamento */
	FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;

  /* reset warning H2O */
  FltWarning.load = FALSE; 	 
  
  /* Reset variabili ActivateAlgorithm */
	ActiveAlgorithmBmp[0] = 0;
	TerminatedAlgorithmBmp [0]= 0;
	
	CLRBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE);
	
   	/*EA TO DO ?LoadEVCheck.status_sensor=LOAD_EV_PILOT_INIT;	????*/
#if (VIRTUAL_SENSOR_ENABLE == 1)
	if (HWC_Flags.bit.vs_wash_presence != 0) 
	{	
		State5.max_speed_value =0xFFFF;
	}
#endif
   
}


/**/

/**
Oltre alle variabili resettate da SPV_resetSensVariablesButDelay, resetta anche le variabili
relative al DELAY nel caso si ingresso in FAULT.

\param [out] 	Delay
	 
\return     	none

\notes:			

\author			Valeria Catani
\date   		07/06/2008
\version    1.0.0

*/

void SPV_resetSensVariables(void)
{
	
	DelayInMinutes = 0;
	SPV_resetSensVariablesButDelay();
}

/**/


/**
Reset variabili all'avvio di un ciclo.

\return     	none

\notes:			

\author			Marco Quintini
\date   		22/03/2007
\version    	1.0.0

*/
static void SPV_startCycleInit(void)
{
	/* Reset eventuali warning attivi */
	
	/* Reset variabili ActivateAlgorithm */
	ActiveAlgorithmBmp[0] = 0;
	ActiveAlgorithmBmpOld[0] = 0;
	TerminatedAlgorithmBmp[0] = 0;
#if (VIRTUAL_SENSOR_ENABLE == 1)
	ReadVS_Data.ctrl_active = FALSE;
	ReadVS_Data.control_done =FALSE;
	if (HWC_Flags.bit.vs_wash_presence != 0)
	{
		State5.max_speed_value =0xFFFF;
	}
	dirty_foam_Data.filter_foam_on=FALSE;
	CLRBIT(Flags_2, FLAG2_CLOGGED_FILTER);
#endif

	/* Init alternato : pos undef */    
    HIO_SetAltPosition(PM_ALT_getPosNotDef());

    /* QI 29.10.09 gestione warning turbina */
    FltWarning.pulseCounterKO =  FALSE;
	FltFlagsKO &= 0X0007;
	/* AZZERO TUTTI I FLAG TRANNE:WARNING_DRAIN_KO, WARNING_DRAIN_PREKO,WARNING_LOADPP_KO*/

	if ( FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO,FLT_CODE_LOAD_EV_KO))
	{/* forzo il reset del fault*/
		FLT_faultMgmtDataReset();
	}

	FltFlagsVS.VScheckinconsistency=0;
	FltFlagsVS.warninglostwater=0;
	Pump_Load_Data.init = FALSE;
	(void)SWTC_Stop((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
	CLRBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* reset flag overflow ripristini */
	CLRBIT(Flags_1, JIF_FLAG1_UP_TERMINATED_KO_POS);
	CLRBIT(Flags_3, JIF_FLAG3_VIRTUALSENSOR_END_CYCLE);
	CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS);
	CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
	CLRBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE);
	if (( FaultMgmt_Data[0].faultCodes.code==FLT_CODE_VS)|| FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_EV_VIA_VS))
	{/* forzo il reset del fault*/
		FLT_faultMgmtDataReset();
	}

	/* Reset statistiche ciclo */
	STC_LastCycleStat_Init();
	/* Aggiornamento array ultimi cicli*/
	STC_LastCyclesArray_Init();
	/* aggiornamento funzioni */
	STC_functionValueUpdateStatistics();
	/* aggiornamento durezza e cesto */
	STC_userDishRack_H2oHardness_Update();
	/* aggiornamento rigenera e billantante */
	STC_regenerate_spaklingAid_Update();

	/* Reset bitmap fasi eseguite (per UI) */
	ITP_phaseTrackerReset();

}
/**/

/**
Reset variabili all'END ciclo

\return     	none

\notes:			

\author			Valeria Catani
\date   		27/05/2008
\version    	1.0.0

*/
static void SPV_endCycleResetVars(void)
{
  
	/* Reset variabili ActivateAlgorithm */
	ActiveAlgorithmBmp[0] = 0;
	TerminatedAlgorithmBmp[0] = 0;
}


/**
Avvia il ciclo indicato dal selettore a seguito di una START o di una REG_RESTART_CONDITION

\param [in] 	SelectorFunctionValue (global)
\param [out] 	StartedCycle (global)
\param [out] 	Interpreter_Data[0] (global)

\return     	none

\notes:			

\author			Marco Quintini
\date   		13/07/2007
\version    	1.0.0

*/
static void SPV_startCycle(void)
{
	StartedCycle = SelectorFunctionValue;
	Interpreter_Data[0].cycle = StartedCycle;
	Interpreter_Data[0].phase = 0;
	ITP_interpreterFlagsInit((uint8_t)(ITP_FULL_INIT));


	/* Inizializzazioni varie */
	SPV_startCycleInit();
}
/**/

/**
Aggiorna la variabile globale utilizzata dall'applicazione per conoscere il valore della
funzione specificata da funcID, ed eventualmente allinea la regolazione della funzione in
BufOldReg a quella in BufNewReg.

\param [in] 	funcID è l'ID della funzione
\param [in] 	Interpreter_Data[0] (global)
\param [in] 	updateCycleDependentFunctions (global)
\param [out] 	*funcValue è il valore della funzione
	 
\return     	S_FALSE se la funzione non è presente nel file di setting, S_OK altrimenti

\notes:			

\author			Marco Quintini
\date   		23/01/2007
\version    	1.0.0

*/
static uint8_t SPV_updateFunctionValue(uint8_t funcID, uint8_t *funcValue)
{
uint8_t retCode;
uint16_t tempFuncValue;

	retCode = REG_checkIfChanged(funcID);
	if( retCode == REG_NOT_EXIST )
	{
		/* La funzione non è presente nel setting */
		return S_FALSE;
	}
	else if( retCode == REG_CHANGED )
	{		
		retCode = REG_getFunctionValueByID(funcID, &tempFuncValue);

		if( retCode == S_OK )
		{
			*funcValue = (uint8_t)(tempFuncValue);
	    	REG_updateOldReg(funcID);
		}
		else if( retCode == REG_NULL_INDEX )
		{
			REG_updateOldReg(funcID);
			
			/* Mantiene *funcValue unchanged */
			
			/* return S_FALSE; con S_FALSE invece imposterei valore di default */
			
		}
		else
		{
			/* Do nothing (mantiene *funcValue e BufOldReg[i] unchanged) */
		}
	}
	else
	{
		/* Do nothing */
	}
	
	return S_OK;
}
/**/

/**
Autogenera un reset freddo in conseguenza di condizioni anomale riscontrate sul file
di setting. Nel reset freddo vengono comunque preservate le statistiche.


\param [in] 	
\param [out] 	
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		07/05/2007
\version    	1.0.0

*/
static void SPV_generateColdReset(void)
{
	/*Viene richiesto il salvataggio dei dati di backup ed il loro ripristino parziale
	(solo le statistiche) dopo il reset.
	*/
	LIB_Backup_SetBackupSource();
	LIB_Backup_ForceBackup();


	LIB_Reset_ForceColdReset();

}
/**/


#if (SYSTEM_NORMAL_AUTOTEST_MODE == 0) 
	#error "SPV_manageRequestFromRemote: SYSTEM_NORMAL_AUTOTEST_MODE equal 0"
#elif (SYSTEM_NORMAL_COLLAUDO_MODE == 0)
	#error "SPV_manageRequestFromRemote: SYSTEM_NORMAL_COLLAUDO_MODE equal 0"
#endif

/**
Reset completo di un fault

\param [in]
\param [out]

\return     	none

\notes:

\author			Claudio Fontana
\date   		01/10/2010
\version    	1.0.0

*/
static void SPV_faultCompleteReset (void)
{
	FLT_applCurrentFaultReset();
	FLT_faultMgmtDataReset();
	FLT_safetyBitmapReset();
	FltFlagsKO=0;
	/*
	CLRBIT(FltFlagsKO,WARNING_DRAIN_KO);
	CLRBIT(FltFlagsKO,WARNING_DRAIN_PREKO);
	CLRBIT(FltFlagsKO,FLAG_BLDC_UNDVL);
	CLRBIT(FltFlagsKO,FLAG_BLDC_OH);
	CLRBIT(FltFlagsKO,FLAG_BLDC_HEATTOUT);
	CLRBIT(FltFlagsKO,FLAG_BLDC_PRESS_SWITCH);
	CLRBIT(FltFlagsKO,WARNING_LOADPP_KO);
	*/
	FltBLDCComm_Tout=0;
	FltBLDCMotor_Tout = 0;
	FltHeatToutData.state = FLT_HEAT_TOUT_DISABLE;
	FltFlagsVS.warninglostwater=0;
	FltFlagsVS.VScheckinconsistency=0;
	CLRBIT(Flags_1, JIF_FLAG1_UP_TERMINATED_KO_POS);
	CLRBIT(Flags_3, JIF_FLAG3_VIRTUALSENSOR_END_CYCLE);
	CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS);
	CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
}



#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
/**
Initiate a new calculation for Power Profile Parameters

\param [in]  none
\param [out] none

\return      none

\notes:

\author      Pietro Loschiavo
\date        07/02/2012
\version     1.0.0

*/
void SPV_calcReqNewPP(void)
{
	bRegsChanged = TRUE;
}


/**
  Returns if regulations among ones acting as modifiers are changed
  The result must be used to require the computation of power profile
  \return     bool_t
  \retval     0, no changes
              1, changes

  \author     Roberto Fioravanti
  \date       24/11/2010
  \version    1.0.0
*/
bool_t SPV_RegulationsMonitor(uint8_t* pOldRegs, uint8_t* pNewRegs)
{
	uint8_t i_bit, i_byte;
	uint8_t  funcPos = 0;
  static uint8_t regs_filter = 0;
  static uint8_t regs_changed = FALSE;


	if(maxFuncIndex > 0)
	{
		for (i_byte=0; i_byte<APP_REGULATIONS_BITMAP_SIZEOF; i_byte++)
		{
			for (i_bit=0; (i_bit<8) && (funcPos<=maxFuncIndex); i_bit++, funcPos++)
			{
				if (
						(regBmp[i_byte] & (1<<(i_bit)))
						&&(pOldRegs[funcPos] != pNewRegs[funcPos] )
				)
				{
			    regs_filter = REGS_FILTER_INIT;
			    regs_changed = TRUE;


/*					return (bool_t)1; */
				}
			}
		}
	}
	else
	{
		/* status is PM_POWER_PROFILE_STATUS_FLAGS__NO_MODIFIERS */
	}

  if(regs_filter)
  {
    regs_filter --;
  }

  if(regs_changed && !regs_filter)
  {
    regs_changed=FALSE;
    return TRUE;
  }


	return (bool_t)0;
}


/**
  Builds the bitmap relating to regulations that are acting as modifiers
  \return     none

  \author     Roberto Fioravanti
  \date       24/11/2010
  \version    1.0.0
*/
void SVP_BuildBmpFunctionsActingAsMofifiers(void)
{
	uint8_t i;
	Loader_Struct ldVarTable;
	uint8_t dataIndex,dataType;
	uint8_t bl=(!SF_OK);

	(void)memset(regBmp, 0 , sizeof(regBmp));

	/* load table of functions acting as modifiers */
	bl=MSettingBasicLoader(Interpreter_Data[0].mSetting, PTR_HW_CONFIG_DATA, CONFIG_PM_POWER_PROFILE_MODIFIERS_VAR_TABLE_DISPL, &ldVarTable);

	if (bl != SF_OK )
	{
		/* PM_PowerProfile_UpdateGlobalStatus(PM_POWER_PROFILE_STATUS_FLAGS__NO_MODIFIERS); */
		return;
	}

	for(i=0; i<ldVarTable.DataLenght; i++)
	{

		dataType  = ((Condition_SelDataStruct *)(ldVarTable.Data) )->dataType;
		dataIndex = ((Condition_SelDataStruct *)(ldVarTable.Data) )->dataIndex;

		switch(dataType)
		{
			case JIF_TAB_FUNCTIONS_SEG:
				regBmp[dataIndex/8] |= (1<<(dataIndex%8));
				/* update max index */
				if( dataIndex > maxFuncIndex )
				{
					maxFuncIndex=dataIndex;
				}
			break;
		}

		ldVarTable.Data += sizeof(Condition_SelDataStruct);
	}
}


/**
  Returns the Power Profile Index from the current Program Heading
  \return     Power Profile Index
x
  \param [in]  cycle cycle number to search
  \param [in]  mSetting mSetting index

  \author     Michele Tanzi
  \date       16/10/2012
  \version    1.0.0

  \author     Pietro Loschiavo
  \date       08/03/2013
  \version    1.1.0

*/
PM_PowerProfile_ProfileIndexType SPV_getPowerProfileIndex(uint8_t cycle, uint8_t mSetting)
{
	Loader_Struct prgHeader;

	if( MSettingBasicLoader(mSetting,
							(uint8_t) PTR_PROGRAM_HEADING, cycle, &prgHeader ) != S_OK )
	{
		return 0;
	}

	if (prgHeader.DataLenght < PROGRAM_HEADING_ITEM_SIZE_WITH_PP)
	{
		return 0;
	}

	return ( OFFS_B(prgHeader.Data, PP_INDEX_OFFSET));
}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */


/**
Configura il contatore specificato in ingresso per un uso come Up Counter,
lo inizializza e lo disabilita.

\param [in]		counterSize e' la size SHORT/MEDIUM/LARGE_COUNTER del contatore da inizializzare
\param [in]		counterId e' l'identificatore del contatore da inizializzare
\param [in]		opCode e' la base tempi del contatore

\return			void

\retval			none

\warning		none

\author			Michele Tanzi
\date			18/07/2012
\version		1.0.0

\bug			none

\todo			nothing

\note			none
*/
void Set_UpCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode)
{
	SWTC_Stop(counterId);
	SWTC_SetPrescaler(counterId, opCode);
	/* The counter is actually a down-counter and it must start from its maximum value
	 * in order to take the correct elapsed time with the SWTC_GetElapsedTime().
	 * Value 0 is used to force the timer expiring because retrigger works
	 * only on an expired timer.
	 */
	switch (counterSize)
	{
		case SHORT_COUNTER:
			SWTC_SetSValue(counterId, (SWTC_SCounterValue) 0);
			break;
		case MEDIUM_COUNTER:
			SWTC_SetMValue(counterId, (SWTC_MCounterValue) 0);
			break;
#if defined(SWTC_L_ENABLED)
		case LARGE_COUNTER:
			SWTC_SetLValue(counterId, (SWTC_LCounterValue) 0);
			break;
#endif

		default:
			break;
	}

	(void)SWTC_Retrigger(counterId);
}


/**/
/**
Updates the Time-To-End estimation.

\author      Andrea Zoppi
\date        28/05/2014
\version     1.0.0
*/
void SPV_updateTimeToEnd(void)
{
  PM_PowerProfile_ProfileIndexType powerProfileIndex;
  SWTC_TimeType elapsedTime;

		switch ( Interpreter_Data[0].completeLabel.s.label )
		{
			case LBL_PROGRAMMING:
			case LBL_DELAY:
			{
				/* Ensure the counter is reset */
				Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);

				/* TTE always computed from the beginning of the cycle */
				powerProfileIndex = SPV_getPowerProfileIndex(Interpreter_Data[0].phase, Interpreter_Data[0].mSetting);
				TTEEstimation = PM_PowerProfile_ComputeTimeToEnd(Interpreter_Data[0].mSetting, 0, powerProfileIndex, 0, 0);
				break;
			}
			case LBL_RUN:
			{

				/* Ensure the counter is running */
				SWTC_Run(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);

				elapsedTime = SWTC_GetElapsedTime(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);


				/* Update TTE estimation */
				powerProfileIndex = SPV_getPowerProfileIndex(Interpreter_Data[0].cycle, Interpreter_Data[0].mSetting);
				TTEEstimation = PM_PowerProfile_ComputeTimeToEnd(Interpreter_Data[0].mSetting, 0, powerProfileIndex, Interpreter_Data[0].phase, elapsedTime);

				break;
			}
			case LBL_END:
			case LBL_FAULT:
			{
				/* Ensure the counter is reset */
				Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);

				/* Ensure there is no more time left */
				TTEEstimation = 0;
				break;
			}
			case LBL_PAUSE:
			{
				/* Ensure the counter is paused */
				SWTC_Stop(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);


				  if ( LBL_RUN == Paused_Intpt_Data[0].completeLabel.s.label )
				  {
					/* Pause from RUN, recompute TTE estimation */
					powerProfileIndex = SPV_getPowerProfileIndex(Paused_Intpt_Data[0].cycle, Paused_Intpt_Data[0].mSetting);
					elapsedTime = SWTC_GetElapsedTime(PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER);
					TTEEstimation = PM_PowerProfile_ComputeTimeToEnd(Paused_Intpt_Data[0].mSetting, 0, powerProfileIndex, Paused_Intpt_Data[0].phase, elapsedTime);
				  }
				  else
				  {
					/* Pause from DELAY, TTE always computed from the beginning of the cycle */
					powerProfileIndex = SPV_getPowerProfileIndex(Paused_Intpt_Data[0].phase, Paused_Intpt_Data[0].mSetting);
					TTEEstimation = PM_PowerProfile_ComputeTimeToEnd(Paused_Intpt_Data[0].mSetting, 0, powerProfileIndex, 0, 0);

					/* Ensure the counter is reset */
					Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
				  }
				break;
			}
			  default:
			  {
				/* Error state, reset */
				Set_UpCounter(SHORT_COUNTER, PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER, COUNTERS2_TBASE_1MIN);
				TTEEstimation = 0;
				break;
			  }
		}
}


/**
Updates the Power Profile.

\author      Andrea Zoppi
\date        12/06/2014
\version     1.0.0
*/
void SPV_updatePowerProfile(uint8_t oldLabel, uint8_t oldPhaseIndex)
{
  bool_t recomputationRequest = FALSE;
  uint8_t cycleIndex = 0;
  uint8_t phaseIndex = 0;
  uint8_t mSetting = MAIN_SF_INDEX;
  PM_PowerProfile_ProfileIndexType powerProfileIndex;
  PM_PowerProfile_PowerProfileSizeType powerProfileSize;
  PM_PowerProfile_EnergyType totalProfileEnergy;
  PM_PowerProfile_EnergyPhaseType *firstEnergyPhasePtr;
  bool_t regulationsChanged = PM_PowerProfile_IsRecomputationReady(bRegsChanged);

  if ( oldLabel != Interpreter_Data[0].completeLabel.s.label )
  {
    /* Running state changed */
    cycleIndex = Interpreter_Data[0].phase;
    phaseIndex = 0;
    mSetting = Interpreter_Data[0].mSetting;
    powerProfileIndex = SPV_getPowerProfileIndex(cycleIndex, mSetting);

    if ( LBL_END == Interpreter_Data[0].completeLabel.s.label )
    {
      /* Entered END state */
      /* - aggiornare le info del PowerProfile perché se ci sono state richieste di PowerProfile la struttura locale non è attendibile
         - azzerare il counter del power profile e il bit DelayPhaseTransition nel byte di stato */
      APP_AlphaCoefficientsComputation();
      PM_PowerProfile_GetPowerProfile(powerProfileIndex, 1, cycleIndex, mSetting, &firstEnergyPhasePtr, &powerProfileSize);

      ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, powerProfileIndex, phaseIndex, mSetting, TRUE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
      PM_PowerProfile_TotalProfileEnergy(1, &totalProfileEnergy);
      totalEnergyEst = (uint8_t)((totalProfileEnergy + 50) / 100);  /* Con approssimazione a 0.5 */
    }
    else if ( LBL_PROGRAMMING == Interpreter_Data[0].completeLabel.s.label )
    {
      /* Entered PROGRAMMING state */
      ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, powerProfileIndex, phaseIndex, mSetting, FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
    }
    else
    {
      /* Do nothing */
    }
  }
  else if ( (LBL_PROGRAMMING == Interpreter_Data[0].completeLabel.s.label) && (oldPhaseIndex != Interpreter_Data[0].phase) )
  {
    /* Another cycle was chosen in programming mode */
    recomputationRequest = TRUE;
    cycleIndex = Interpreter_Data[0].phase;
    phaseIndex = 0;
    mSetting = Interpreter_Data[0].mSetting;
  }
  else if ( regulationsChanged || APP_PowerUp_flg )
  {
    /* Regulations changed, or the interpreter state was resumed from power loss (RMR) */
    recomputationRequest = TRUE;
    switch ( Interpreter_Data[0].completeLabel.s.label )
    {
    case LBL_PROGRAMMING:
    case LBL_DELAY:
    {
      cycleIndex = Interpreter_Data[0].phase;
      phaseIndex = 0;
      mSetting = Interpreter_Data[0].mSetting;
      break;
    }
    case LBL_PAUSE:
    {

        if ( LBL_RUN == Paused_Intpt_Data[0].completeLabel.s.label )
        {
          /* Pause from RUN */
          cycleIndex = Paused_Intpt_Data[0].cycle;
          phaseIndex = Paused_Intpt_Data[0].phase;
        }
        else
        {
          /* Pause from DELAY */
          cycleIndex = Paused_Intpt_Data[0].phase;
          phaseIndex = 0;
        }
        mSetting = Paused_Intpt_Data[0].mSetting;

      break;
    }
    case LBL_RUN:
    {

        cycleIndex = Interpreter_Data[0].cycle;
        phaseIndex = Interpreter_Data[0].phase;
        mSetting = Interpreter_Data[0].mSetting;

      break;
    }
    default:
    {
      /* Error */
      break;
    }
    }
  }
  else
  {
    /* Do nothing */
  }

  if ( recomputationRequest )
  {
    SVP_BuildBmpFunctionsActingAsMofifiers();

    if ( 0 != cycleIndex )  /* Ignore the dummy cycle */
    {
      /* Do not update the (backupped) alpha coefficients when resuming from power loss (RMR) */
      if ( FALSE == APP_PowerUp_flg )
      {
    	  APP_AlphaCoefficientsComputation();
      }

      powerProfileIndex = SPV_getPowerProfileIndex(cycleIndex, mSetting);
      PM_PowerProfile_GetPowerProfile(powerProfileIndex, 1, cycleIndex, mSetting, &firstEnergyPhasePtr, &powerProfileSize);

      ApplPowerProfileStatus = PM_PowerProfile_UpdatePowerProfileStatus(0, powerProfileIndex, phaseIndex, mSetting, FALSE, POWER_PROFILE_PHASE_DELAY_TRANSITION_OFF, ApplPowerProfileStatus);
      PM_PowerProfile_TotalProfileEnergy(1, &totalProfileEnergy);
      totalEnergyEst = (uint8_t)((totalProfileEnergy + 50) / 100);  /* Con approssimazione a 0.5 */


    }
  }

  /* The SID might be updated */
  PM_PowerProfile_UpdateTemporalFilters(0, recomputationRequest);

  APP_PowerUp_flg = FALSE;
}




/* *********** */
/* End of file */
/* *********** */
