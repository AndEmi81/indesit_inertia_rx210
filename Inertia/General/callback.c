/**
\defgroup	callback Modulo Callback
\ingroup	ApplGeneral

Il modulo definisce le callback utilizzate nel progetto.
*/

/**
Source file del modulo Callback

\file		callback.c
\ingroup	callback
\date    	01/08/2012
\version	01.00.00
\author  	Francesco Corvo , Marco Quintini
*/

/*
Copyright (c) 2004-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Suppresses "unused symbol" and "pointer should be const", because callback bodies could
 * not use/need all the specified parameters */
/*lint -save -e715 -e818 */

/* ******** */
/* Includes */
/* ******** */
#include "AnalogExp.h"
#include "DigitalInputsExp.h"
#include "StandardTypes.h"
#include "StandardDefines.h"
#include "callback.h"
#include "CstmHeaders.h"
#include "Main.h"
#include "Lib_ResetExp.h"
#include "BusCommPltf_cfg.h"

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#include "PM_PowerProfileExp.h"
#endif
/*#include "SwTimersCounters.h"*/
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* Defines on Special Mode */ /* Sicuramente da spostare MQ PA */
#define SPECIAL_MODE_ENTER			0xFF
#define SPECIAL_MODE_EXIT			0x00


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
/* External function */
/* ***************** */

/* ***************** */
/* Exported function */
/* ***************** */

#if !defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
/**
Viene chiamata all'interno della routine SetAllLoadsOff() per escludere
dallo spegnimento alcuni carichi particolari.

\param [in]		loadID e' l'ID del carico da spegnere

\return 		TRUE se il carico deve essere spento (sempre su Cooling 2008), 0 altrimenti.

\warning		none

\version		1.0.0
\date
\author
*/
/*lint -e715 */
uint8_t CBF_SetAllLoaddOff(uint8_t loadID)
{

	return TRUE;

}

/**
Restituisce un valore che indica se l'applicazione pilota i carichi con la
SetLoad() standard o con la GIOM_LoadPilot().

\param [in]		loadPos e' la posizione o l'ID del carico (noto all'applicazione)

\return 		FALSE -> utilizzare SetLoad() (se TRUE -> utilizzare GIOM_LoadPilot(),
				ma non e' il caso dell'applicazione Cooling 2008)

\warning		none

\version		1.0.0
\date			19/05/2006
\author			Stefano Ughi
*/
uint8_t CBF_GIOM_LoadPilot(uint8_t loadPos)
{

  	if( (loadPos == Pump_Pos[LD_WASH_PUMP_IND] )
  		 &&
  		 ( ( HWC_Flags.bit.bldc ) || ( HWC_Flags.bit.bldc1 ) )
  	  )
	{
		return TRUE;
	}
	if ((loadPos == Pump_Pos[LD_DRAIN_PUMP_IND] ) &&  HWC_Flags.bit.bldc_drain)
	{
		return TRUE;
	}

	return FALSE;
}

#endif /*#if defined CONFIG_LIB_LOADSREQUEST_SUPPORT*/
/*lint +e715 */


/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), in caso di
reset caldo.

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_WarmReset(void)
{
#ifdef _STATISTICS_ON_
	/* Contatore dei reset caldi avvenuti durante un ciclo */
	if( SPV_isCycleRunning() == TRUE )
	{
		IncByteWithCheck(&MachineStatistics.testCycleStats.warmResetNum);
		IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].warmResetNum);
	}

#endif /* _STATISTICS_ON_ */

	if( HWC_Flags.bit.alternate != 0 )
	{

		/* QI 04.12.09 - WARM RESET - Old status Alternato = UNDEF per farein modo che il comando venga ribadito al driver
		(poichè¨ con il reset caldo il carico, come tutti si stacca */

		/*LoadsReq_Old[Alt_Pos].b = ALT_getPosNotDef();*/
		/*LoadsReq_New[Alt_Pos].b = ALT_getPosNotDef();*/

	}

} /*end CALLBACK_Main_WarmReset*/


/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), in caso di
reset freddo e prima che vengano ripristinati i dati di backup. \n
Al suo interno viene chiamata la routine che esegue la configurazione
iniziale dell'applicazione, caricando i dati dal file di setting.

\attention 		ConfigInit_result (global, risultato della chiamata alla
				routine HND_wmConfigInit()) e' usata come output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			25/07/2012
\author			Marco Quintini

\date 			13/11/2013
\author			Andrea Emili
\Note			Porting in Inertia Project

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_ColdReset_BeforeRestore(void)
{
	/* Deve essere chiamata prima di ogni altra funzione di applicazione */
	ConfigInitResult =  HND_dwConfigInit();

	if( ConfigInitResult != S_OK )
	{
		FLT_declareSettingFault();
	}

	/* QI 12.11.09 reset Criteria bit ad eccezione dello stato sale (utile per vedere transizioni durante power off */
	CLRBIT(GICriteria,GI_PRESS_SENS_CR_FULL_IND);
	CLRBIT(GICriteria,GI_CR_OVFL_IND);
	CLRBIT(GICriteria,GI_CR_DOOR_IND);
	CLRBIT(GICriteria,GI_SPARKLING_AID_IND);
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
	PM_PowerProfile_SetParamModifierFunc((CALLBACK_PowerProfile_ModifierValueType)GetModifierValueAPP2_0);
	PM_PowerProfile_SetAlphaFunc((CALLBACK_PowerProfile_AlphaValueType)GetAlphaModifierValueAPP2_0);
	APP_PowerUp_flg = TRUE;
	APP_GetReferenceParameters();
#endif
}/**/


/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), in caso
di reset freddo e se i dati di backup sono stati ripristinati con successo.

\attention 		RestoreType (global, tipo di ripristino dei dati di backup)
				e' usata come output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_ColdReset_OnRestoredData(void)
{
	RestoreType = CHECK_RESTORE;

	if( LIB_Backup_GetBackupSource() == PARTIAL_BACKUP_KEYWORD )
	{
		/*
		Restore parziale dei dati di backup (es. dopo un PC Control o dopo un reset freddo
		autogenerato dal Supervisore): solo le statistiche vengono ripristinate, si riparte
		da OFF logico.
		*/
		RestoreType = DEFAULT_RESTORE;
	}
}/*end CALLBACK_Main_ColdReset_OnRestoredData*/


/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), in caso di
reset freddo e se i dati di backup non sono stati ripristinati.

\attention 		RestoreType (global, tipo di ripristino dei dati di backup)
				e' usata come output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_ColdReset_OnNotRestoredData(void)
{
	RestoreType = DEFAULT_RESTORE;
}/*end CALLBACK_Main_ColdReset_OnNotRestoreData*/

/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), in caso di
reset freddo e dopo che e' stata eseguita la fase di ripristino dei dati (che
sia stata eseguita con successo o meno).

\attention 		ConfigInit_result (global, risultato della chiamata alla
				routine HND_wmConfigInit()) e' usata come input
\attention 		MachineStatistics (global, statistiche) e' usata come output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_ColdReset_AfterRestore(void)
{
#ifdef _STATISTICS_ON_

	/* Contatori dei reset freddi avvenuti durante un ciclo */
	if( SPV_isCycleRunning() == TRUE )
	{
		IncByteWithCheck(&MachineStatistics.testCycleStats.coldResetNum);
		IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].coldResetNum);

	}

	IncWordWithCheck(&MachineStatistics.globalStats.backup_num);
#endif /* _STATISTICS_ON_ */


	/*DEA602 FA BACKUP DELLO STATO DI FAULT E NON LO RESETTA AL POR*/
	/** Inizializzazione CurrentFault. Non posso farla in HND_wmConfigInit() perchÃ¨
		CurrentFault sarebbe poi sovrascritto dal restore dei dati di backup. */
	/*FLT_currentFaultReset();*/

	/** Verifica se dichiarare un fault sul file di setting in base al risultato di
		HND_wmConfigInit(). Non Ã¨ possibile farlo direttamente alla chiamata di HND_wmConfigInit()
		perchÃ¨ CurrentFault sarebbe poi sovrascritto dal restore dei dati di backup. */
	if( ConfigInitResult != S_OK )  /*AE TODO Questo parte va inserita o non ha più senso*/
	{
		FLT_declareSettingFault();
	}
}/*CALLBACK_Main_ColdReset_AfterRestore*/


/**
Viene chiamata all'interno della routine Main_Backup_DoRestore(), restituisce un
valore che indica se l'applicazione richiede l'invalidazione dell'area di backup
al reset e dopo l'eventuale restore dei dati. \n
Per la posizione della sua chiamata, viene al momento sfruttata anche per
inizializzare le strutture utilizzate dalla piattaforma per gestire i pacchetti
di lettura e scrittura regolazioni.

\attention 		BufOldReg (global, buffer delle regolazioni main) e' usata come input
\attention 		BufNewReg (global, buffer delle regolazioni scritte dalla UI) e'
				usata come input

\return			bool_t

\retval 		TRUE (si richiede l'invalidazione)

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
bool_t CALLBACK_Main_BackupAreaInvalidationRequired(void)
{
	CPApplianceDataTableType applDataItem;

	/*
	Init strutture per la gestione dei pacchetti lettura e scrittura regolazioni
	e scrittura UI Info.
	N.B.: per questa operazione sarebbe esteticamente più bello	creare una nuova
	callback con un nome diverso, chiamata nello stesso punto, ma alla fine la sostanza
	non cambia.	Oppure si potrebbe inizializzare le strutture solo al reset freddo.
	*/


	applDataItem.pData = (void*)&BufOldReg[0];
	applDataItem.szMaxLength = FUNC_NUM_ALL;

	Set_CPApplianceDataItem(CP_APPLDATA_R_REGS, &applDataItem);

	applDataItem.pData = (void*)&BufNewReg[0];
	applDataItem.szMaxLength = FUNC_NUM_ALL;

	Set_CPApplianceDataItem(CP_APPLDATA_W_REGS, &applDataItem);

	applDataItem.pData = (void*)&BufUIInfo[0];
	applDataItem.szMaxLength = sizeof(BufUIInfo);

	Set_CPApplianceDataItem(CP_APPLDATA_W_UIINFO, &applDataItem);

	return TRUE;
}


/**
Viene chiamata all'interno della macro MAIN_APP_PERIODICASYN_TASK__STARTUP.
Attiva il contatore che misura il tempo dall'ultimo reset.

\attention 		Time_Since_Last_Reset_Counter (global, contatore tempo
				dall'ultimo reset) e' usata come output


\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note
	- AE Modificato il valore di inizializzazione del contatore.
*/
void CALLBACK_Main_StartUp(void)
{
	/* Essendo usato come se fosse un Up Counter, è inizializzato al valore massimo */
		SWTC_SetSValueAndRun ((SWTC_CountersType)TIME_SINCE_LAST_RESET_COUNTER_ID, (SWTC_SCounterValue)INT8_T_MAX);
}


/**
Viene eseguita in MAIN_APP_PERIODICASYN_TASK__BODY. \n
Verifica che i Low Level Input analogici e digitali siano pronti, esegue
l'aggiornamento dei Generic Input e la copia dei Consistency nel buffer
utilizzato dall'applicazione, ...


\return			bool_t

\retval 		FALSE se gli ingressi analogici o digitali non sono ancora
				pronti (in questo caso il Task Periodico di Applicazione non
				esegue nessun'altra operazione), TRUE altrimenti

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			AE Portata CBF_Task_Main_Application_Body0 di DEA 700
*/
bool_t CALLBACK_Main_ApplicationTask_Ctrl(void)
{
static uint8_t BackupDataAnalysed = FALSE;

#ifdef CONFIG_PM_LINEARPRESSURE_SENSOR
	/* Stato PL */
	LPS_configUpdate();
#endif /* CONFIG_PM_LINEARPRESSURE_SENSOR */

	#ifdef __CLASSBCHECK_H
	Set_WDFlag_In(WD_TASK_MAIN_BODY0_FLAG);
	#endif
	/*ElectSupplDataPntr = GetCalibrationPointers(ID_CAL_PNTR_ELECT_SUPP);*/
	/*FactoryDataPntr = GetCalibrationPointers(ID_CAL_PNTR_FACTORY);*/

	#if	(SBY_STANDBY_ENABLE == 1)
	/** Controllo Reset per StandBy*/
	STB_CheckStandByCounter();
	#endif

	/** Filtro su ingressi analogici e digitali */
	if((LIB_ADC_AreAnalogInputsReady() == FALSE) || (LIB_DIO_AreDigInputsReady() == FALSE))
	{
		return FALSE;
	}

	/* Aggiornamento variabile GICriteriaOld */
	/*da fare qui o al termine del FaultMgmt dove viene utilizzata la var per analisi transizioni */
	HIO_gICriteriaOld_Update();

	/** Aggiornamento ingressi */
	HIO_inputManagement();

	/* Gestione criterio sale presente / assente */
	HIO_saltCriterionRegMgmt();

	HIO_consistencyManagement();

	/*Aggiorna dati dal motore*/
	HIO_updateEngineData();

	/** Analisi dati di backup (solo la prima volta dopo un reset freddo) */

	if( BackupDataAnalysed == FALSE )
	{
		if(HND_backupDataAnalysis() != S_OK)
		{
			/* Rilevato un fault del file di setting*/
			FLT_declareSettingFault();
		}
		BackupDataAnalysed = TRUE;
	}


	/**  Gestione calibrazione sensore di torbidità */
	if(HWC_Flags.bit.turbsens)
	{
		 /*Richiesta calibrazione dall'esterno*/
		if(TurbiditySensorCal.pwd==ACTIVATE_TURBIDITY_CALIBRATION_PWD)
		{
			TurbidityCalibrationCmd();
		}

		TurbidityCalibrationManager();
		TurbidityPwmValManager();
	}

	/*VEDI SE RIPORTARE QUESTA GESTIONE IN DEA602*/
	/*
	Aggiornamento del flag che indica se ci sono le condizioni per l'esecuzione dei u-programmi
	e per l'aggiornamento degli ingressi
	*/
	#if defined (__CLASSBCHECK_H)
	/* Segnala al WD_Manager() l'uscita dall'esecuzione di body0 */
	Set_WDFlag_Out(WD_TASK_MAIN_BODY0_FLAG);
	#endif

	return TRUE;
}/* end CALLBACK_Main_ApplicationTask_Ctrl*/


/**
Viene chiamata in MAIN_APP_PERIODICASYN_TASK__BODY quando si riceve un
pacchetto PC Control e prima di richiedere il pilotaggio dei carichi. \n
Attiva il contatore che gestisce il timeout dello stato di PC Control.

\param [in]		pRemoteLoadsControlData punta ai dati del pacchetto PC Control
\param [in]		len punta alla lunghezza del pacchetto PC Control?

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CALLBACK_RemoteLoadsControl_BeforeProcess(void * pRemoteLoadsControlData, int *len)
{
	(void)Set_SmallCounter(TIME2DEATH_COUNTER_ID, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)(60)); /*AE TODO sostituito COLD_RESET_TIMEOUT con 60*/
	(void)SWTC_Run(TIME2DEATH_COUNTER_ID);
}


/**
Viene chiamata in MAIN_APP_PERIODICASYN_TASK__BODY quando si riceve un
pacchetto PC Control e dopo la richiesta di pilotaggio dei carichi.

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			
\author			

\bug			none

\todo			nothing
   
\note			none
*/
void CALLBACK_RemoteLoadsControl_AfterProcess(void * pRemoteLoadsControlData, int *len)
{

}


/**
Viene eseguita in MAIN_APP_PERIODICASYN_TASK__BODY. \n
Esegue le chiamate alle routine che costituiscono il corpo dell'applicazione:
\par
	- Supervisore;
	- gestione sicurezze e fault;
	- imposizione dello stato dei carichi;
	- aggiornamento delle statistiche;
	- aggiornamento del buffer di stato.

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing

\note			none
*/
void CALLBACK_Main_ApplicationTask_Body(void)
{

	/* Gestione Timer PC Control */
	if ( Main_IsRemoteEnabled() )
	{
		/*
		NOTA: la condizione !SWTC_IsStopped() è necessaria perché al reset freddo
		Time2Death_Counter non viene inizializzato, quindi vale 0 e !SWTC_IsExpired()
		risulta verificata, per cui verrebbe vista una condizione di PC Control.
		*/
		/* PC Control On */
		LIB_Backup_SetBackupSource();

		/** Aggiornamento bit PC Control */
		SETBIT(ApplicationState, AS_REMOTE_CTRL_ON_BIT); /* PC Control */

		/** Aggiornamento buffer di stato */
		UpdateMachineState();

		return;
	}
	/* SuperVisor */
	/** Reset stato richiesto dai carichi*/
	HIO_loadsReset();

	/* Reset richieste flag di sicurezza */
	/*FLT_resetSafetyFlagsRequest();*/	/*AE TODO da inserire? */

	if( FLT_checkForSettingFault() == FALSE )
	{
		/* Se è attivo il fault file di setting la gestione degli stati è inibita */

		/* Gestione attivazione cicli speciali da remoto */
		SPV_manageRequestFromRemote();

		/* Gestione stato porta per uP */
		SFT_uPdoorStatus_Mgmt();

		/* Supervisore stati macchina */
		SPV_superVisor();

		/*Set pointer to DSP Buffer for Energy Meter*/
		UpdateInfoBufferToPowerMeterModule();

		/* Gestione attivazione algoritmi tramite Editor */
		AAL_algorithmManagement();

		/* Gestione sicurezze */
		SFT_safetyManagement();	

		/* Gestione fault (dopo le sicurezze perchè hanno l'ultima parola) */
		FLT_faultManagement();

		/* Gestisce lo stato globale di fault della macchina */
		FLT_manageGlobalFaultStatus();

	}

	HIO_loadsManagement();

	/** Processamento richieste reset statistiche */
	#ifdef _STATISTICS_ON_
	STC_manageStatResetRequest();
	#endif /* _STATISTICS_ON_ */

	/** Aggiornamento bitmap da copiare nel buffer di stato */
	UpdateStateBitmaps();

	/** Aggiornamento buffer di stato */
	UpdateMachineState();

	#if	(SBY_STANDBY_ENABLE == 1)
	/* Standby */
	STB_HarakiriManager();
	#endif /* (SBY_STANDBY_ENABLE == 1) */

	#ifdef __CLASSBCHECK_H
	Set_WDFlag_Out(WD_TASK_MAIN_BODY1_FLAG);
	#endif /* __CLASSBCHECK_H */

}

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)/*AE Non previsto in quanto Inertia è un progetto non normativo*/

void CALLBACK_Main_ApplBody_PostLoads(void)
{}

#endif /*  defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */

/** 
Chiamata all'interno della macro MAIN_SUPERVISOR_TASK__BODY ogni 
MAIN_SUPERVISOR_TASK_PERIOD.

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CALLBACK_Main_GP(SupervisorInfoType spInfo)
{
	SWTC_Manager();
}


/**
Viene chiamata nel MAIN_SUPERVISOR_TASK__BODY, verifica se l'applicazione richiede
che sia controllato il timeout del contatore Time2Death_Counter (in pratica verifica
se il prodotto e' in PC Control).

\attention 		ApplicationState (global, bitmap stato applicazione) e' usata
				come input

\return			bool_t

\retval 		TRUE se il prodotto e' in PC-Control, FALSE altrimenti

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
bool_t CALLBACK_EnableCheckColdResetTimeout(uint8_t systemMode)
{
	if (TESTBIT(ApplicationState, AS_REMOTE_CTRL_ON_BIT))
	{
		return TRUE;
	}

	return FALSE;
}


/**
Chiamata all'interno della routine Main_SupervisorTask().
Esegue delle inizializzazioni dipendenti da SystemMode e dal
tipo di reset (caldo/freddo), ciò che prima veniva fatto in
CBF_ProgrammingMode_InitTask(), OnColdReset(), OnWarmReset(),
OutputInitialization(), CBF_ProgrammingMode_InitTaskStartUp0().

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			03/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note
	- AE Nothing to do
*/
void CALLBACK_Main_InitTask(SysMode_Type systemMode, bool_t coldreset)
{
	CPApplianceDataTableType applDataItem;

	if(systemMode == SYSTEM_PROGRAMMING_MODE)
	{
		/* Backup area Invalidation */
		(void)LIB_Backup_InvalidateBackupArea();
	}

	if(systemMode == SYSTEM_NORMAL_MODE)
	{

	}

	/* Common product modules initialization */
	if ( coldreset )
	{

		SWTC_InitManager();
		SWTC_InitTables(&SWTC_PrescalerTable[0],
				&SWTC_InternalCounter[0],
				(uint8_t)SWTC_PRESCALERS_NUM,
				SWTC_CounterConfigurationTable,
				SWTC_CountersTable,
				(uint8_t)SWTC_COUNTERS_NUM
				);

	}

#ifndef _IDENTCONST_ENABLE
	/* Ident Table initialization */
	Init_IdentTablePointers();
#endif /* !_IDENTCONST_ENABLE */

	/* Lookup Table */
	Init_LookupTablePointers();

	applDataItem.pData = (void*)&IdentTable;
	applDataItem.szMaxLength = sizeof(IdentTable);

	Set_CPApplianceDataItem(CP_APPLDATA_IDENT, &applDataItem);

	applDataItem.pData = (void*)&ExtendedIdentTable;
	applDataItem.szMaxLength = sizeof(ExtendedIdentTable);

	Set_CPApplianceDataItem(CP_APPLDATA_EXTENDED_IDENT, &applDataItem);

	applDataItem.pData = (void*)&MachineStateBuf[0];
	applDataItem.szMaxLength = sizeof(MachineStateBuf);

	Set_CPApplianceDataItem(CP_APPLDATA_R_STATUS, &applDataItem);
}


/**
Viene eseguita in CMS_EE_FUNC_EE_SystemMode. \n
In base al contenuto del pacchetto SYSTEM_MODE imposta la variabile
ExternalRequest che viene poi interpretata dall'applicazione per
eseguire la corrispondente azione richiesta (es: attivazione/disattivazione
cicli speciali, reset delle statistiche, scorrimento della pila dei Last Fault).
 
\param [in]		submode indica l'azione richiesta
\attention 		ExternalRequest (global, memorizza per l'applicazione l'azione
				richiesta) e' usata come output
 
\return			uint8_t

\retval 		0

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
uint8_t CALLBACK_Protocol_SubModeManage(uint8_t submode)
{
	switch (submode)
	{
		/* Attivazione ciclo di Autotest */
		case SYSTEM_NORMAL_AUTOTEST_MODE:
		/* Attivazione ciclo di Collaudo */
		case SYSTEM_NORMAL_COLLAUDO_MODE:
		/* Scorrimento buffer Last Fault */
		case SYSTEM_NORMAL_RESET_LAST_FAULT:
		/* Reset di tutte le statistiche */
		case SYSTEM_NORMAL_RESET_TO_DEFAULT:
		/* Attivazione Standby */
		case SYSTEM_NORMAL_STANDBY_MODE:
		/* Disattivazione Standby */
		case SYSTEM_NORMAL_RESET_STANDBY_MODE:
		/* Attivazione Fast Standby */
		case SYSTEM_NORMAL_FAST_STANDBY_MODE:

			ExternalRequest = submode;

			break;

		/* Attivazione ciclo Demo */
		case SYSTEM_NORMAL_DEMO_MODE:

			/* Se prevista deve essere processata in SPV_manageRequestFromRemote() */

			break;

		default:
			break;

	}

	return 0;
}


/**
Viene chiamata all'interno della routine CMS_EE_FUNC_EE_SystemMode.
Attiva o disattiva la modalita' di Test per l'eventuale UI On Board.
Setta/Resetta il bit AS_SPECIAL_MODE_ON_BIT in ApplicationState.

\param [in]		submode indica la modalità di funzionamento richiesta.

\return 		void

\retval 		none

\warning		none		

\author			Marco Quintini
\date 			31/08/2012
\version		1.0.0

\bug			none

\todo			nothing
   
\note			none
*/
void CBF_Task_Protocol_SetSpecialModeManager(uint8_t submode)
{
}


/**
Viene chiamata all'interno della routine CMS_EE_FUNC_EE_SystemMode.
Usata solo se presente una UI On Board.
 
\param [in]		submode indica la modalità di funzionamento richiesta

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CALLBACK_Protocol_UIRemoteControlManage(uint8_t subMode)
{
/*AE è vuota anche in Arcadia Normative*/
}


/**
Viene eseguita in Task_Backup. \n
Notifica l'allarme pre-Backup.

\attention 		ApplicationState (global, bitmap stato applicazione) e' usata
				come output
\attention 		recoveryComProc (global, variabile utilizzata per la procedura
				di sincronizzazione UI-Main) e' usata come output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CBF_notifyBackupAlarm(void)
{
	/* notifica al display probabile mancanza rete */
	/*CLRBIT(ApplicationState, AS_BACKUP_ALM_OFF_BIT);
	MachineStateBuf[POS_APPL_STATE] &= (PRE_BACKUP_BIT_MASK ^ 0xFF);

	recoveryComProc = 0;*/			  /* blocca write reg. */
}


/*#if ENABLE_SYNC_UI_MAIN == 1*/
/**
Viene chiamata all'interno della routine PP_SlaveNotification(). \n
E' utilizzata nella procedura iniziale di sincronizzazione UI-Main mediante
la sequenza READ_STATUS - READ_REGS - WRITE_REGS.

\attention 		recoveryComProc (global, variabile utilizzata per la procedura
				di sincronizzazione UI-Main) e' usata come input/output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CBF_OnReadStatusCompleted(void)
{
	if(recoveryComProc == 0)
	{
		recoveryComProc++;
	}
}


/**
Viene chiamata all'interno della routine CMS_EE_CTRL_Ctrl_ReadRegsRequest(). \n
E' utilizzata nella procedura iniziale di sincronizzazione UI-Main mediante
la sequenza READ_STATUS - READ_REGS - WRITE_REGS.

\attention 		recoveryComProc (global, variabile utilizzata per la procedura
				di sincronizzazione UI-Main) e' usata come input

\return			uint8_t

\retval 		TRUE/FALSE

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
uint8_t CBF_OnReadRegulations(void)
{
	if(recoveryComProc >= 1)
	{
		return TRUE;
	}

	return FALSE;
}


/**
Viene chiamata all'interno della routine PP_SlaveNotification().
\n
E' utilizzata nella procedura iniziale di sincronizzazione UI-Main mediante
la sequenza READ_STATUS - READ_REGS - WRITE_REGS.

\attention 		ApplicationState (global, bitmap stato applicazione) e' usata
				come output
\attention 		recoveryComProc (global, variabile utilizzata per la procedura
				di sincronizzazione UI-Main) e' usata come input/output

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
void CBF_OnReadRegulationsCompleted(void)
{
	if(recoveryComProc >= 1)
	{
		if(recoveryComProc == 1)
		{
			recoveryComProc++;
		}
		/* Notifica alla UI esterna tutto O.K. */
		SETBIT(ApplicationState, AS_BACKUP_ALM_OFF_BIT);
		MachineStateBuf[POS_APPL_STATE] |= PRE_BACKUP_BIT_MASK;
	}	
}

/**
Viene chiamata all'interno della routine PP_SlaveNotification().
\n
E' utilizzata per la notifica dei dati UI Info

\return			void

\retval 		none

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Roberto Fioravanti

\bug			none

\todo			nothing

\note			none
*/
void CBF_OnWriteUIInfoCompleted(void)
{
	UI_Info_Ctrl=TRUE;
}


/**
Viene chiamata all'interno della routine CMS_EE_CTRL_Ctrl_WriteRegsRequest(). \n
E' utilizzata nella procedura iniziale di sincronizzazione UI-Main mediante
la sequenza READ_STATUS - READ_REGS - WRITE_REGS.

\attention 		ApplicationState (global, bitmap stato applicazione) e' usata
				come output
\attention 		recoveryComProc (global, variabile utilizzata per la procedura
				di sincronizzazione UI-Main) e' usata come input

\return			uint8_t

\retval 		TRUE se è abilitata la scrittura delle regolazioni, FALSE altrimenti

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
uint8_t CBF_OnWriteRegulations(void)
{
	if( recoveryComProc > 1 )
	{   /* cancella uscita da cold reset, al reset freddo il bit Ã¨ a 0, qui viene scritto ad 1 */
		SETBIT(ApplicationState, AS_REGS_WRITTEN_BIT);

		return TRUE;		
	}

	return FALSE;
}
/*#endif*/ /* ENABLE_SYNC_UI_MAIN == 1 */


/**
Viene chiamata in LIB_IOM_OutputInitialization() per discriminare
i carichi da spegnere all'inizializzazione.
Se TRUE il carico viene spento.
 
\param [in]		loadPos e' la posizione nell'Output Map del carico da spegnere

\return			uint8_t

\retval 		TRUE (si richiede lo spegnimento)

\warning		none

\version		1.0.0
\date			31/08/2012
\author			Marco Quintini

\bug			none

\todo			nothing
   
\note			none
*/
bool_t CBF_IOManager_IOVal_Ctrl(uint8_t loadPos)
{
	return TRUE;
}

/**
Viene chiamata nel modulo Analog.
Legge il canale del DS per evitare influenza reciproca dei canali
 
\return			void

\retval 		none

\warning		none		

\version		1.0.0
\date			
\author			Stefano Ughi	

\bug			none

\todo			nothing
   
\note			none
*/
void CBF_AnalogInputIntManager_Polling(void)
{
	
}

/* *************** */
/* Local Functions */
/* *************** */

/*lint -restore */ /* Restore e715 and e818 previously disabled */

/* *********** */
/* End of file */
/* *********** */


