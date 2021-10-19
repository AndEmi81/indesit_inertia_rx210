/**
Header file del modulo Application Platform Variables

\file		APP_Global.h
\ingroup	APP_Global
\date		18/06/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_GLOBAL_H
#define __APP_GLOBAL_H


/* ******** */
/* Includes */
/* ******** */
#include "APP_Config.h"
#include "APP_Defines.h"
#include "APP_Types.h"
#include "SwTimersCounters.h"
#include "CstmTypes.h" /*AE Only local check with Ema*/

/* ****************** */
/* Defines and Macros */
/* ****************** */
#ifndef __APPL_GLOBAL
/**
Fa si' che in APP_Global.c, in cui e' visibile la define __APPL_GLOBAL, le variabili
vengano definite, mentre negli altri file che includono APP_Global.h, in cui
__APPL_GLOBAL non e' visibile, le stesse variabili vengano viste come 'extern'.
*/
#define	_AP_GLOBAL_ extern
#define	_AP_INIT_(initVal)
#else
#define	_AP_GLOBAL_
#define	_AP_INIT_(initVal) = { initVal }
#endif /* __APPL_GLOBAL */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
/**
\name Power Profile
*/
/*@{*/ 
_AP_GLOBAL_ bool_t APP_PowerUp_flg;
_AP_GLOBAL_ uint8_t ApplPowerProfileStatus;        /*APPS*/
_AP_GLOBAL_ uint8_t totalEnergyEst;
_AP_GLOBAL_ uint16_t TTEEstimation;

#if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE )
_AP_GLOBAL_ uint8_t uiSavedEnPhElapsedTime_Counter;
#endif /* #if ( PP_SAVE_ELAPSED_POWERPHASE_COUNTER == TRUE ) */

/* Backup */
_AP_GLOBAL_ uint8_t bkp_CompItpDataCycle;
_AP_GLOBAL_ uint8_t bkp_CompItpDataPhase;
_AP_GLOBAL_ uint8_t bkp_CompItpDatamSetting;
_AP_GLOBAL_ uint8_t bkp_CompItpDataFirstStatement;

/* EnergyPause_Status */
#if ( PP_ENERGYPAUSE_STATUS == TRUE )
_AP_GLOBAL_ uint8_t EnergyPause_Status;
#endif /* ( PP_ENERGYPAUSE_STATUS == TRUE ) */

/* Contatori */
_AP_GLOBAL_ SWTC_SCounterVariableType EnPhElapsedTime_Counter        _AP_INIT_(-1);
_AP_GLOBAL_ SWTC_CounterConfigurationType cfgEnPhElapsedTime_Counter _AP_INIT_(SWTC_CFG_INIT_EX(FALSE,  TRUE, COUNTERS2_TBASE_1MIN, SWTC_CFG_SIZE_S));
_AP_GLOBAL_ SWTC_SCounterVariableType TimeBtwPhases_Counter          _AP_INIT_(-1);
_AP_GLOBAL_ SWTC_CounterConfigurationType cfgTimeBtwPhases_Counter   _AP_INIT_(SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S));
_AP_GLOBAL_ SWTC_SCounterVariableType tte_enphnospecialselapsedtime_Counter          _AP_INIT_(-1);
_AP_GLOBAL_ SWTC_CounterConfigurationType cfgtte_enphnospecialselapsedtime_Counter   _AP_INIT_(SWTC_CFG_INIT_EX(FALSE, TRUE, COUNTERS2_TBASE_1MIN, SWTC_CFG_SIZE_S));
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
/*@}*/ 

/**
\name Interprete
*/
/*@{*/ 
/**
Buffer strutture con i dati sullo stato corrente dell'Interprete (per le
applicazioni mono-cavita' e' un buffer monodimensionale)
*/
_AP_GLOBAL_ Interpreter_Struct Interpreter_Data[APP_N_COMP];	/* S� Backup - LUT */
/*@}*/ 

/**
\name Microprogramma Jump If
*/
/*@{*/ 
/** Buffer tabella destinazione Jump If (per le applicazioni mono-cavita'
e' un buffer monodimensionale)
*/
_AP_GLOBAL_ JumpIF_Struct Table_JumpIf[APP_N_COMP];			/* No Backup */

#if (ITP_JIF_PHASE_ENABLE == 1)
/** */
_AP_GLOBAL_ JumpIfPhase_Struct JmpPhase[APP_N_COMP];		/* SI Backup - LUT*/
_AP_GLOBAL_ JumpIfPhase_Struct Paused_JmpPhase[APP_N_COMP];
#endif  /* (ITP_JIF_PHASE_ENABLE == 1) */
#if ( ITP_JIF_RET_ENABLE == 1 )
#if ( ITP_JIF_PHASE_ENABLE == 1 )
/** */
_AP_GLOBAL_ JumpIfPhase_Struct JmpPhase_Return[APP_N_COMP];	/* SI Backup - LUT*/
#endif /* ( ITP_JIF_PHASE_ENABLE == 1 ) */
/** */
_AP_GLOBAL_ RetPoint_Struct ReturnPoint[APP_N_COMP];		/* SI Backup - LUT*/
/** */
_AP_GLOBAL_	JsrStack_Struct Jsr[APP_N_COMP];				/* SI Backup - LUT*/
#endif /* ( ITP_JIF_RET_ENABLE == 1 ) */
/*@}*/ 

#if (ITP_UI_PHASE_TRAKER_ENABLE == 1)
/**
\name Sincronizzazione con UI
*/
/*@{*/ 
/** Ciclo da comunicare alla UI */
_AP_GLOBAL_ uint8_t UICycle;
/** Fase da comunicare alla UI */
_AP_GLOBAL_ uint8_t UIPhase;
/*@}*/ 
#endif /* (ITP_UI_PHASE_TRAKER_ENABLE == 1) */



/*@}*/ 
/**
\name Variabili per la gestione del modulo ActivateAlgorhythm
*/
/*@{*/ 
#if (AAL_ACTIVATEALGORHYTHM_ENABLE == 1)
#if  (AAL_16_ALGO == 0)

_AP_GLOBAL_ uint8_t ActiveAlgorithmBmp[APP_N_COMP];
_AP_GLOBAL_ uint8_t TerminatedAlgorithmBmp[APP_N_COMP];
_AP_GLOBAL_ uint8_t ActiveAlgorithmBmpOld[APP_N_COMP];

#else

_AP_GLOBAL_ uint16_t ActiveAlgorithmBmp[APP_N_COMP];
_AP_GLOBAL_ uint16_t TerminatedAlgorithmBmp[APP_N_COMP];
_AP_GLOBAL_ uint16_t ActiveAlgorithmBmpOld[APP_N_COMP];

#endif
_AP_GLOBAL_ uint8_t AADisplParams[N_MAX_ALG][APP_N_COMP];
#endif /* (AAL_ACTIVATEALGORHYTHM_ENABLE == 1) */
/*@}*/ 




/**
\name Variabili per la gestione del modulo StandBy
*/
/*@{*/ 
#if (SBY_STANDBY_ENABLE == 1)
/** Contatore per lo standby */
_AP_GLOBAL_ SWTC_MCounterVariableType     StbHarakiri_MCounter    _AP_INIT_(-1); /* No Backup */ 
_AP_GLOBAL_ SWTC_CounterConfigurationType StbHarakiri_ConfigCounter _AP_INIT_(SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_M)); /* No Backup */ 
/** stato standby */
_AP_GLOBAL_ uint8_t HarakiriRequired;				/* No Backup */
/** Puntatore e Lunghezza campo dati Safety - StandBy */
_AP_GLOBAL_ Loader_Struct Tab_SFTStandByData;		/*No Backup*/

_AP_GLOBAL_ Sby_LowPower_Struct HarakiriLowPower;		/* SI Backup */ /* Sby Low Power 21.01.10 */

#endif /* (SBY_STANDBY_ENABLE == 1) */
/*@}*/ 

#if (APP_NEW_FAULT_STATE_MACHINE == 1)

_AP_GLOBAL_ Fault_Compl_Struct ApplCurrentFault[APP_N_COMP];	/*  LUT */
_AP_GLOBAL_ sft_fault_codes_t PltFaultCodes;
_AP_GLOBAL_ sft_fault_codes_t CurrentFault[APP_N_COMP];

_AP_GLOBAL_  FaultMgmt_Data_Struct FaultMgmt_Data [APP_N_COMP] ;
_AP_GLOBAL_  const sft_fault_codes_t No_Fault;

#endif

#if (CHECKERRORS_ENABLE == 1)
/**
\name Debug buffer
*/
/*@{*/ 
/** Buffer che memorizza i codici di errore genrati dall'Applicazione */
_AP_GLOBAL_ uint8_t Debug_Error[DEBUG_BUFFER_LEN];		/* No Backup - LUT */


_AP_GLOBAL_ Statistics_Struct MachineStatistics;					/* SI backup */
/*@}*/ 
#endif /* (CHECKERRORS_ENABLE == 1) */


/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __APP_GLOBAL_H */

/* *********** */
/* End of file */
/* *********** */
