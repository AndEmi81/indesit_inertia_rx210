/**
\defgroup	CstmGlobal Modulo variabili globali
\ingroup	ApplGeneral

Il modulo contiene la dichiarazione e, laddove prevista, l'inizializzazione
delle variabili e delle costanti globali dell'applicazione.
*/

/**
Source file del modulo variabili globali

\file		CstmGlobal.c
\ingroup	CstmGlobal
\date		16/07/2012
\version	01.00.00
\author		Marco Quintini

\par		History

\par		PCLint
			Report dd/mm/yyyy: x Errors, y Warnings, z Info
*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


#define __CSTM_GLOBAL

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "BCP_BusCommMng.h"
#include "LIB_ResetExp.h"
#include "LIB_ZeroCrossingExp.h"
#include "SWTC_Exported.h"
#include "CstmHeaders.h"
#include "BackupAddress.h"
#include "PM_PulseCounterExp.h"

/*
Platform Library
*/
#include "AnalogExp.h"
#include "DigitalInputsExp.h"
#include "PM_AnalogFeedbackExp.h"

/*
Platform modules
*/
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#include "ExternalOutputManagerExp.h"
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */

#ifdef CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT
#include "PM_FrequencyFeedbacksExp.h"
#endif /* CONFIG_PM_FREQUENCY_FEEDBACKS_SUPPORT */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* general */
#define NUM_REG_MAX										40
#define NUM_UI_INFO_MAX 								3	/* numer of bytes of the buffer BufUIInfo. TBD */
#define N_UI_BACKUP_DATA								16	/* UI-backup reserved buffer size in bytes */
/**
\name Ident Table
*/
/*@{*/ 
/*Define for RX200 microcontroller family*/
#if defined(RN_RX200)

#if defined(RN_RX200_R5F5210)

/** Setting File Page Size in Words = 512<<NUM_SHIFT_LEFT_OF_512 */
#define NUM_SHIFT_LEFT_OF_512							2
/** N° di pagine riservate al file di setting */
#define NUM_PAGES_SETTING								14
#elif defined(RN_RX200_R5F52Z0_6)
/** Setting File Page Size in Words = 512<<NUM_SHIFT_LEFT_OF_512 */
#define NUM_SHIFT_LEFT_OF_512							2
/** N° di pagine riservate al file di setting */
#define NUM_PAGES_SETTING								8
#elif defined(RN_RX200_R5F52Z0_5)
/** Setting File Page Size in Words = 512<<NUM_SHIFT_LEFT_OF_512 */
#define NUM_SHIFT_LEFT_OF_512							1
/** N° di pagine riservate al file di setting */
#define NUM_PAGES_SETTING								16
#endif

/** Produttore del microcontrollore */
#define IDT_MANUFACTURER								MCU_RENESAS
/** Modello del microcontrollore */
#if defined(RN_RX200_R5F5210)
#define IDT_MODEL										MODEL_R5F52108
#elif defined(RN_RX200_R5F52Z0)
#define IDT_MODEL										MODEL_R5F52Z05
#endif /* Model */
/** Uso del CRC */
#define IDT_CRC											CRC__YES

/** Byte order */
#ifdef __GNUC__
	#ifdef __RX_LITTLE_ENDIAN__
		#define IDT_ENDIAN								LITTLE_ENDIAN
	#else
		#define IDT_ENDIAN								BIG_ENDIAN
	#endif /* __RX_LITTLE_ENDIAN__ */
#else /* __GNUC__ */
	#error "not supported yet."
	#define IDT_ENDIAN									BIG_ENDIAN
#endif /* !__GNUC__ */

/** firmware version RX200: 1st index */
#define IDT_FW_VER_VERSION_IND							0x02	/*encoding: B.C.D. */
/** firmware version RX200: 2xd index */
#define IDT_FW_VER_VERIFICATION_IND						0x11	/*encoding: B.C.D. */
/** firmware version RX200: 3rd index */
#define IDT_FW_VER_MODIFICATION_IND						0x00	/*encoding: B.C.D. */
/** Indirizzo iniziale della RAM destinata alla Piattaforma */
#define IDT_SKELETON_RAM_AREA_START_ADDR		0
/** Indirizzo iniziale della RAM destinata all'Applicazione */
#define IDT_CUSTOM_RAM_AREA_START_ADDR			0

#define _EXTERN_

#endif  /* RN_RX200 */


/** last ram write protected address */
#define END_OF_UNTOUCHABLE_RAM							((uint16_t)PP_END_OF_UNTOUCHABLE_RAM)


/**  Kind of applicance */

#if defined(CONFIG_APPLIANCE_WM)
#define IDT_APPLIANCE					    			APPLIANCE_WM
#elif defined(CONFIG_APPLIANCE_DW)
#define IDT_APPLIANCE					    			APPLIANCE_DW
#elif defined(CONFIG_APPLIANCE_CL)
#define IDT_APPLIANCE					    			APPLIANCE_CL
#elif defined(CONFIG_APPLIANCE_CK)
#define IDT_APPLIANCE					    			APPLIANCE_DK
#elif defined(CONFIG_APPLIANCE_DR)
#define IDT_APPLIANCE					    			APPLIANCE_DR
#elif defined(CONFIG_APPLIANCE_HO)
#define IDT_APPLIANCE					    			APPLIANCE_HO
#elif defined(CONFIG_APPLIANCE_HD)
#define IDT_APPLIANCE					    			APPLIANCE_HD
#elif defined(CONFIG_APPLIANCE_CL)
#define IDT_APPLIANCE					    			NOTAPPLIANCE
#else
#error "Unsupported appliance model"
#endif
#define IDT_BOARD										BOARD_MAIN
/** Evo */
#define IDT_EVO											EVO_PLATFORM__5_0

/** Formato Ident Table */
#define IDT_FORMAT										E_FORMAT
/** Dimensione blocco di scrittura */
#define IDT_BLOCK_SIZE									BLOCK_SIZE__64
/** */
#define IDT_FREE										0

/** Extended address support (32 bit addresses supported in Ident Extended, read/erase/write memory ) */
#if defined(CONFIG_CP_EXTENDED_REQUESTS_ENABLED)
#define IDT_EXTENDED_REQ_SUPPORT				1
#else
#define IDT_EXTENDED_REQ_SUPPORT				0
#endif

/** First readable RAM address  */
#define IDT_APPLICATION_RAM_START_ADDRESS				END_OF_UNTOUCHABLE_RAM
/** Setting file Start address */
#define IDT_EEP_START_ADDR								((uint16_t)(SETTING_VIRTUAL_ADDRESS))

/** Lookup Table address*/
#define IDT_LUT_PTR										((uint16_t)&LookupTablePointers[0])
/** Dimensione in word di una pagina del file di setting (numero di shift a sinistra di 512) */
/** Setting page size (left shift of 512 bytes) */
#define IDT_EEP_PAGE_SIZE_IN_WORDS						NUM_SHIFT_LEFT_OF_512
/** Number of pages  for setting - 1 */
#define IDT_EEP_PAGE_NUM								(NUM_PAGES_SETTING-1)
/**  Lookup Table items number*/
#define IDT_LUT_ITEMS_NUM								sizeTab(LookupTablePointers)
/** backup start address*/
#define IDT_BACKUPAREA_START_ADDR						((uint16_t)BCKP_STAT_VIRTUAL_ADDRESS)
/** statistics start address */
#define IDT_STATISTICSAREA_START_ADDR					((uint16_t) &MachineStatistics)
/** backup size in bytes*/
#define IDT_BACKUPAREA_SIZE								((uint16_t)BACKUPAREA_SIZE_IN_BYTES)
/**  statistics size */
#define IDT_STATISTICSAREA_SIZE							(sizeof(MachineStatistics))

/** Last Fault Address */
#define IDT_LAST_FAULT_ADDRESS							((uint16_t) &MachineStatistics.lastFault[0].faultCodes)
/** Current Fault Address */
#define IDT_CURR_FAULT_ADDRESS							((uint16_t) &CurrentFault)
/** WBox Area Address*/ 
#define IDT_WBOX_ADDRESS								((uint16_t)WBOX_VIRTUAL_ADDRESS)
/** max global statistics area in bytes */
#define IDT_STATISTIC_ABS_DIM							0x0000
/** max daily statistics area size in bytes */
#define IDT_STATISTIC_REL_DIM							0x0000
/** PDM code address */ 
#define IDT_PDM_ADDRESS									((uint16_t)PDM_VIRTUAL_ADDRESS)
/** Product Config Bitmap Values */
#define IDT_PROD_CFG_BITMAP								PRODUCT_CFG_WBOX_PRESENT

/*@}*/


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

/* ****** INIZIALIZZAZIONE VARIABILI APPLICAZIONE ****** */

/** Maintain initialization */
_DW_GLOBAL_ uP_Maintain_Struct uP_Maintain_Data =
{
	(uint8_t)0,						/* uint8_t status; */
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	LKT_INIT_M_COUNTER(-1),					/* SWTC_MCounterVariableType     timer; */
	LKT_INIT_S(0),			/* uint16_t time_target; */
	LKT_INIT_PTR((uint8_t *)NULL)
};

/* Loads control Initialization */
_DW_GLOBAL_ loadsPulse_Struct loadsPulseData1 =
{
	0, /* uint8_t offset; */
	0, /* uint8_t step; */
	LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
};

_DW_GLOBAL_ loadsPulse_Struct loadsPulseData2 =
{
	0, /* uint8_t offset; */
	0, /* uint8_t step; */
	LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
};

_DW_GLOBAL_ uP_Drain_Struct uP_Drain_Data = {
	 0,
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */

};

_DW_GLOBAL_ uP_7liter_Struct uP_7liter_Data = {
	0,
	0,
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	LKT_INIT_M_COUNTER(-1),	/* SWTC_MCounterVariableType     timer; */
	LKT_INIT_S(0),
	LKT_INIT_S(0),
};

_DW_GLOBAL_ uP_AltCtrl_Struct uP_AltCtrl_Data= {
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S), /* SWTC_CounterConfigurationType cfgTimer; */
	((SWTC_SCounterValue)-1),
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S), /* SWTC_CounterConfigurationType cfgTimer; */
	((SWTC_SCounterValue)-1),
	0,
	0,
	0,
	0,
	0,
	0,
};

uP_Load_Struct uP_Load_Data = {
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S),
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S),
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S),
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S),
	((SWTC_SCounterValue)-1),
	((SWTC_SCounterValue)-1),
	((SWTC_SCounterValue)-1),
	((SWTC_SCounterValue)-1),
	LKT_INIT_S(0),
	LKT_INIT_S(0),

};

uP_Regenerate_Struct uP_Regenerate_Data = {
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_200MS, SWTC_CFG_SIZE_S),
	((SWTC_SCounterValue)-1),
	(uint16_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
};

ReadVS_struct ReadVS_Data = {
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(SWTC_CounterConfigurationType)(SWTC_CFG_INIT(FALSE, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)),
	(SWTC_MCounterValue)0,
	(uint16_t)0,
	(uint16_t)0,
};

uP_LoadRestore_Struct uP_LoadRestore_Data = {
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S),
	((SWTC_SCounterValue)-1),
	(uint16_t)0,
	(uint16_t)0,
	(uint16_t)0,
	(uint16_t)0
};   /* SI Backup */ /*LUT*/
uP_SetProfile_Struct uP_SetProfile_Data = {

	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	 LKT_INIT_S(0),
	 LKT_INIT_M_COUNTER(-1),
	 SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */

};

uP_SparklingAid_Struct uP_SparklingAid_Data = {
	(uint8_t) 0,
	(uint8_t )0,
	(uint8_t )0,
	(uint8_t )0,
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M), /* SWTC_CounterConfigurationType cfgTimer; */
	LKT_INIT_M_COUNTER(-1),
	(uint8_t)0,
	(uint8_t)0
};

LoadEVCheck_Struct LoadEVCheck = {
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S),
	((SWTC_SCounterValue)-1)
};
FltTout_Struct FltHeatToutData = {
	(uint8_t)0,
	(uint8_t)0,
	(SWTC_CounterConfigurationType)(SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1MIN, SWTC_CFG_SIZE_M)),
	LKT_INIT_M_COUNTER(-1),
};
/*
Altre strutture con contatori da inizializzare
*/
Pump_Load_Struct Pump_Load_Data = {
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M),
	LKT_INIT_M_COUNTER(-1),
};
FltTout_Struct FltLoadToutData = {
	0,
	0,
	(SWTC_CounterConfigurationType)(SWTC_CFG_INIT(FALSE, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)),
	LKT_INIT_M_COUNTER(-1),

};
SWTC_CounterConfigurationType Bulk_Off_Delay_timer_cfg = {SWTC_CFG_INIT_EX(FALSE,FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Timer_Fan_StartUp_Config= {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Timer_Pump_OverHeating_Drain_config={SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)};
SWTC_CounterConfigurationType DelayTimer_Config  ={SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType DoorClosedEvent_Pause_timer_cfg ={SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Res_Press_Stable_Cnt_Config={SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType DryResDelayActivation_cnt_cfg= {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType time_4_unblock_EV_VS_config= {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Flt_overheating_ResOff_timer_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)};
SWTC_CounterConfigurationType PulseCntStill_timer_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_100MS, SWTC_CFG_SIZE_M)};
#if (VIRTUAL_SENSOR_ENABLE == 1)
SWTC_CounterConfigurationType Aal_DF_end_always3_config = {	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)};
SWTC_CounterConfigurationType uP_7liter_dirtywater_timer_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_15MIN, SWTC_CFG_SIZE_M)};
SWTC_CounterConfigurationType Filter_VS_56_State_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1MIN, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Stability_Filter_VS_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S)};
SWTC_CounterConfigurationType Flt_UnderVoltage_ResOff_timer_cfg = {SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_M)};

#ifdef _STATISTICS_ON_
TempCycleStatistics_t TempCycle_Stats = {
	(uint8_t )0,
	(uint8_t )0,
	(uint8_t )0,
	(SWTC_CounterConfigurationType)(SWTC_CFG_INIT_EX(FALSE, FALSE, COUNTERS2_TBASE_1MIN, SWTC_CFG_SIZE_M)),
	LKT_INIT_M_COUNTER(-1),
	/* Connectivity statistic support */
	  0, /* uint8_t clockHH; */
	  0, /* uint8_t clockMM; */
	  LKT_INIT_S(0),/* UINT16_FIELD( APPEnergyConsumption ); */
	  LKT_INIT_S(0),/* UINT16_FIELD( APPTotalDuration ); */
	  0, /* UINT16_FIELD( loaded_water)*/
	/**/

};
#endif /* _STATISTICS_ON_ */




al_DirtyFoam_Struct dirty_foam_Data = {
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S),
	(SWTC_SCounterValue)0,
	(uint16_t)0,
	(uint16_t)0,
	(uint16_t)0,
	(uint16_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_1SEC, SWTC_CFG_SIZE_S),
	(SWTC_SCounterValue)0,
};

SWTC_SCounterVariableType Stability_Filter_VS_timer =
{
	(SWTC_SCounterValue)-1
};

New_SafetyVs_Struct New_safety_VS = {
	(uint8_t)0,
	(uint8_t)0,
	(uint8_t)0,
	SWTC_CFG_INIT_EX(FALSE, FALSE,COUNTERS2_TBASE_10SEC, SWTC_CFG_SIZE_S),
	(SWTC_SCounterValue)0
};
#endif
SWTC_SCounterVariableType Bulk_Off_Delay_timer =
{
	(SWTC_SCounterValue)-1
};

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

#if defined(CONFIG_LIB_CLASS_B_FLASH)
/** FlashCheckCode - Check code (CRC) for Flash area defined
 */
const uint16_t FlashCheckCode __attribute__ ((section (".ProgramCheckCode"))) = 0x0000;

#endif

/* ****** INIZIALIZZAZIONE COSTANTI DELL'APPLICAZIONE ****** */

const MICROPROG Microprog_array[N_MAX_MICROPROG] = {
	SST_SetState_Exe,		      /* OpCode:	0 */
	LOA_uP_Load_Exe,			  /* OpCode:	1 */
	LOR_uP_LoadRestore_Exe,	      /* OpCode:	2 */
	HEA_uP_Heat_Exe,			  /* OpCode:	3 */
	PMP_uP_PumpControl_Exe,       /* OpCode:	4 */
	ALT_uP_AlternateControl_Exe,  /* OpCode:	5 */
	LDC_uP_LoadsControl_Exe,      /* OpCode:	6 */
	SPA_uP_SparklingAid_Exe,      /* OpCode:	7 */
	DRN_uP_Drain_Exe,             /* OpCode:	8 */
	MTN_uP_Maintain_Exe,		  /* OpCode:	9 */
	AAL_Activate_Algorithm_Exe,   /* OpCode:	10 */
	JIF_JumpIf_Exe,			      /* OpCode:	11 */
	SAL_uP_SetAlarm_Exe,          /* OpCode:    12 */
	RGN_uP_Regenerate_Exe,        /* Opcode:    13 */
	SPF_SetProfile_Exe,			  /* Opcode:    14 */
	SL_uP_7liter_Exe,			  /* Opcode:    15 */

};

/* VAR INIT */
/* Puntatori ai selettori per i diversi vani */
const uint16_t SelectorDataPointers[APP_N_COMP] =
{
	APP_PTR_SELECTOR_TARGET_0,
};


/**
COUNTERS Configuration
Constants for sw timers configuration
*/
/**  Costanti per la determinazione delle basi dei tempi dei timer */

const SWTC_PrescalerTableType SWTC_PrescalerTable[SWTC_PRESCALERS_NUM-1] = {
	10,	/*10*10ms = 100ms*/
	2,	/*2*(10*10)ms = 200ms*/
	5,	/*5*(2*10*10)ms = 1sec*/
	2,	/*2*(5*2*10*10)ms = 2sec*/
	5,	/*5*(2*5*2*10*10)ms = 10sec*/
	6,	/*6*(5*2*5*2*10*10)ms = 1 MIN*/
	15,	/*15*(6*5*2*5*2*10*10)ms = 15 M*/
};

/*
NOTA: ogni volta che viene dichiarato un nuovo contatore con la rispettiva variabile
di configurazione, occorre inserire gli address delle 2 variabili in queste tabelle.
Inoltre va opportunamente configurata l'enum "CountersIndex" per indicizzare 
opportunamente il contatore
*/

/*Tabella variabili di configurazione*/
_EXTERN_ BEGIN__SWTC_CONFIG_TABLE(SWTC_COUNTERS_NUM)
	PLATFORM_SW_COUNTERS_CFG
	APP_PLT_SW_COUNTERS_CFG
	&uP_Maintain_Data.timer_config,				/* +0 */
	&loadsPulseData1.timer_config,				/* +1 */
	&loadsPulseData2.timer_config,				/* +2 */
	&uP_Drain_Data.timer_config,				/* +3 */
	&uP_7liter_dirtywater_timer_cfg,			/* +4 */
	&uP_7liter_Data.timer_config,				/* +5 */
	&uP_AltCtrl_Data.timer_config,				/* +6 */
	&uP_AltCtrl_Data.filter_timer_config,		/* +7 */
	&uP_Load_Data.levTOtime_cnt_config,			/* +8 */
	&uP_Load_Data.fullStability_cnt_config,		/* +9 */
	&uP_Load_Data.timeLoad_cnt_config,			/* +10 */
	&uP_Load_Data.pump_Act_4VS_cfg,				/* +11 */
	&uP_SetProfile_Data.seg_cnt_config,			/* +12 */
	&uP_SparklingAid_Data.timer_config,			/* +13 */
	&Bulk_Off_Delay_timer_cfg,					/* +14 */
	&Timer_Fan_StartUp_Config,					/* +15 */
	&Timer_Pump_OverHeating_Drain_config,		/* +16 */
	&DelayTimer_Config,							/* +17 */
	&uP_LoadRestore_Data.timeLoad_cnt_config,	/* +18 */
	&uP_Regenerate_Data.EVdelay_cnt_config,		/* +19 */
	&DoorClosedEvent_Pause_timer_cfg,			/* +20 */
	&Res_Press_Stable_Cnt_Config,				/* +21 */

#if (VIRTUAL_SENSOR_ENABLE == 1)
	&Stability_Filter_VS_cfg,					/* +22 */
	&Aal_DF_end_always3_config,					/* +23*/
	&ReadVS_Data.timer_config, 					/* +24*/
	&Filter_VS_56_State_cfg,					/* +25 */
	&dirty_foam_Data.aal_Stab_sens_cnt_config,	/* +26 */
	&dirty_foam_Data.aal_stab_stato3_config,	/* +27 */
	&New_safety_VS.timer_config,				/* +28 */
#endif
	&DryResDelayActivation_cnt_cfg,				/* +29 */
	&PulseCntCheckDelay_timer_cfg,				/* +30 */
	&PulseCntStill_timer_cfg,					/* +31 */
	&LoadEVCheck.timer_config,					/* +32 */
	&time_4_unblock_EV_VS_config,				/* +33 */
	&FltHeatToutData.timer_config,				/* +34 */
	&Flt_UnderVoltage_ResOff_timer_cfg,			/* +35 */
	&Flt_overheating_ResOff_timer_cfg,			/* +36 */
	&TempCycle_Stats.duration_timer_cfg,		/* +37 */
	&Pump_Load_Data.timer_config,				/* +38 */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
	&cfgEnPhElapsedTime_Counter,				/* +39 */
	&cfgTimeBtwPhases_Counter,					/* +40 */
	&cfgtte_enphnospecialselapsedtime_Counter   /* +41 */
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
END__SWTC_CONFIG_TABLE()



/*Tabella variabili counters*/
_EXTERN_ BEGIN__SWTC_COUNTERS_TABLE(SWTC_COUNTERS_NUM)
	PLATFORM_SW_COUNTERS
	APP_PLT_SW_COUNTERS
	&uP_Maintain_Data.timer,					/* +0 */
	&loadsPulseData1.timer,						/* +1 */
	&loadsPulseData2.timer,						/* +2 */
	&uP_Drain_Data.timer,						/* +3 */
	&uP_7liter_dirtywater_timer,				/* +4 */
	&uP_7liter_Data.timer,				        /* +5 */
	&uP_AltCtrl_Data.timer,						/* +6 */
	&uP_AltCtrl_Data.filter_timer,				/* +7 */
	&uP_Load_Data.levTOtime_counter,			/* +8 */
	&uP_Load_Data.fullStability_counter,		/* +9 */
	&uP_Load_Data.timeLoad_counter,				/* +10 */
	&uP_Load_Data.pump_Act_4VS_counter,			/* +11 */
	&uP_SetProfile_Data.seg_timer,				/* +12 */
	&uP_SparklingAid_Data.timer,				/* +13 */
	&Bulk_Off_Delay_timer,						/* +14 */
	&Timer_Fan_StartUp,							/* +15 */
	&Timer_Pump_OverHeating_Drain,			    /* +16 */
	&DelayTimer,								/* +17 */
	&uP_LoadRestore_Data.timeLoad_counter,		/* +18 */
	&uP_Regenerate_Data.EVdelay_counter,		/* +19 */
	&DoorClosedEvent_Pause_timer,				/* +20 */
	&Res_Press_Stable_Counter,					/* +21 */
#if (VIRTUAL_SENSOR_ENABLE == 1)
	&Stability_Filter_VS_timer,					/* +22 */
	&Aal_DF_end_always3_timer,					/* +23 */
	&ReadVS_Data.timer, 						/* +24*/
	&Filter_VS_56_State_timer,					/* +25 */
	&dirty_foam_Data.aal_Stab_sens_counter,		/* +26 */
	&dirty_foam_Data.aal_stab_stato3_counter,	/* +27 */
	&New_safety_VS.timer,						/* +28 */
#endif
	&DryResDelayActivation_cnt,					/* +29 */
	&PulseCounterCheckDelay_timer,				/* +30 */
	&PulseCounterStill_timer,					/* +31 */
	&LoadEVCheck.timer,							/* +32 */
	&time_4_unblock_EV_VS_counter,				/* +33 */
	&FltHeatToutData.timer,						/* +34 */
	&Flt_UnderVoltage_ResOff_timer,				/* +35 */
	&Flt_overheating_ResOff_timer,				/* +36 */
	&TempCycle_Stats.duration_timer,			/* +37 */
	&Pump_Load_Data.timer,						/* +38 */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
	&EnPhElapsedTime_Counter,					/* +39 */
	&TimeBtwPhases_Counter,						/* +40 */
	&tte_enphnospecialselapsedtime_Counter,		/* +41*/
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
END__SWTC_COUNTERS_TABLE()


/**
JUMPIF & CONDITIONS
*/

/* GIProbe */
uint8_t *const JIF_GIProbe_Ptr[1] = {

  &GITemp,
};

/* GIPressSensor */
uint16_t *const JIF_GIPressSensor_Ptr[1] = {

	(uint16_t*)&GIWaterLevel,
};

/* GICriterion */
uint8_t *const JIF_GICriterion_Ptr[1] = {

	(uint8_t*)&GICriteria,
};

/* GIWaterLoaded */
uint16_t *const JIF_GIWaterLoaded_Ptr[1] =
{
   (uint16_t*)&GIWaterLoaded,
};

/* GITurbidity */
uint8_t *const JIF_GITurbidity_Ptr[1] =
{
   &GITurbidity,
};

/*Functions*/
uint8_t *const JIF_Functions_Ptr[1] = {

	&BufNewReg[0],
};

/* Booleans */
const uint8_t JIF_JumpAlways_var = 1;

uint8_t *const JIF_Boolean_Ptr[1] = {

	(uint8_t*)(&JIF_JumpAlways_var),
};

/* Flags */

uint8_t *const JIF_Flags_Ptr[2] = {

	(uint8_t*)&Flags_1,
	(uint8_t*)&Flags_3,
};


/* LastFault */
uint16_t *const JIF_LastFault_Ptr[1] = {

	(uint16_t*)&MachineStatistics.lastFault[0].faultCodes,
};

uint8_t *const JIF_WashPumpVS_Ptr[1] = {

	(uint8_t*)&Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_WASH],
}; 		/* Cod 27 - JIF_TAB_WASH_VS */

uint8_t *const JIF_DrainPumpVS_Ptr[1] = {

	(uint8_t*)&Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_DRAIN],
};
/* Cod 19 - Alpha coefficients */
uint16_t *const JIF_Alpha_Coeffients[4] = {

	(uint16_t*)&app_alpha_voltage_on_power,
	(uint16_t*)&app_alpha_voltage_on_durationON,
	(uint16_t*)&app_alpha_voltage_temperature_on_durationON,
	(uint16_t*)&app_alpha_temperature_on_durationOFF
};
/* Cod 28 - JIF_TAB_DRAIN_VS */



/* Condition Table */
void* const JIF_DataTable[NDATATYPES] = {

	(void*)(JIF_GIProbe_Ptr),			  /* Cod 0, GIProbe -	JIF_TAB_GI_PROBE_SEG */
	(void*)(JIF_GIPressSensor_Ptr),	/* Cod 1, GIPressSensor - JIF_TAB_GI_PRESSSENSOR_SEG */
	(void*)(JIF_GICriterion_Ptr),		/* Cod 2, GICriterion -	JIF_TAB_GI_CRITERION_SEG */
	(void*)(JIF_GIWaterLoaded_Ptr),	/* Cod 3  GIWaterLoaded - JIF_TAB_GI_FLOW_METER_SEG*/
	(void*)(JIF_GITurbidity_Ptr),		/* Cod 4  GITurbidity - JIF_TAB_GI_TURBIDITY_SEG */
	NULL,								            /* Cod 5 */
	NULL,								            /* Cod 6 */
	NULL, 								          /* Cod 7 */
	(void*)(JIF_Functions_Ptr), 		/* Cod 8, Functions - JIF_TAB_FUNCTIONS_SEG */
	NULL, 							          /* Cod 9 */
	NULL, 							          	/* Cod 10 */
	NULL, 								          /* Cod 11 */
	NULL, 								          /* Cod 12 */
	NULL, 								          /* Cod 13 */
	NULL, 								          /* Cod 14 */
	NULL, 								          /* Cod 15 */
	(void*)(JIF_Boolean_Ptr),			  /* Cod 16, Booleani		- 	JIF_TAB_BOOLEANS_SEG */
	(void*)(JIF_Flags_Ptr),				  /* Cod 17, Flags		- 	JIF_TAB_FLAGS_SEG */
	(void*)(JIF_LastFault_Ptr),			/* Cod 18, LastFault	-	JIF_TAB_LAST_FAULT */
	(void*)(JIF_Alpha_Coeffients),		/* Cod 19 */
	NULL,								/* Cod 20 */
	NULL,							    /* Cod 21 */
	NULL, 								/* Cod 22 */
	NULL, 								/* Cod 23 */
	(void*)(JIF_WashPumpVS_Ptr), 		/* Cod 24 - JIF_TAB_WASH_VS */
	(void*)(JIF_DrainPumpVS_Ptr), 		/* Cod 25 - JIF_TAB_DRAIN_VS */
	NULL, 								/* Cod 26 */
	NULL, 								/* Cod 27 */
	NULL, 								/* Cod 28 */
	NULL, 								/* Cod 29 */
	NULL, 								/* Cod 30 */
	NULL, 								/* Cod 31 */
};

/**
MACHINE STATE
*/
void *const TabMachineStateAddr[NUM_MACHINE_STATE_VARS] = {

	&ApplicationState,
	&ApplicationFlags,
	&UICycle,				/* variabili in cui viene salvato ciclo/fase main quando sono stabili */
	&UIPhase,
	&DelayInMinutes,
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
    &ApplPowerProfileStatus,
    &EnergyPause_Status,
  	&totalEnergyEst,
  	&TTEEstimation
#endif
};

const uint8_t TabMachineStateDim[NUM_MACHINE_STATE_VARS] = {

	sizeof(ApplicationState),
	sizeof(ApplicationFlags),
	sizeof(UICycle),
	sizeof(UIPhase),
	sizeof(DelayInMinutes),
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
    sizeof(ApplPowerProfileStatus),
    sizeof(EnergyPause_Status),
	sizeof(totalEnergyEst),
	sizeof(TTEEstimation)
#endif
};

/**/

/**
DIGITAL PROTOCOL Configuration
*/
uint8_t* const Write_Status_Addr = (uint8_t*)(&BufNewReg[0]);
uint8_t* const Status_Var = (uint8_t*)(&MachineStateBuf[0]);
uint8_t* const Reg_Var = (uint8_t*)(&BufOldReg[0]);
uint8_t* const UIInfoBuff_Addr = (uint8_t*)(&BufUIInfo[0]);



/*
BACKUP Configuration
*/


/**
LOOKUP TABLE
*/
const uint8_t LookupTableDataSizes[N_LOOKUP_TABLE_ELEMENTS-1] = {

	sizeof(ResetCtrl.SystemMode),			            	/* 0  Modalità di funzionamento */
	sizeof(ResetCtrl.Reset), 								/* 1  Reset flags for PC Control enabled */
	sizeof(ResetCnt),										/* 2  Contatori cause di reset */
	sizeof(CommErr),										/* 3  Contatori errori I2C */
	sizeof(&SWTC_PrescalerTable),							/* 4: tabella dei valori di prescaler usati dall'applicazione */
	sizeof(Time2Death_Counter), 							/* 5  sizeof(Time2Death_Counter),*/					/* 17 Timer su PC Control */
	sizeof(Time_Since_Last_Reset_Counter),					/* 6  : Tempo trascorso dall'ultimo reset */
	sizeof(DigInputs),										/* 7  Ingressi Digitali */
	sizeof(AnalogInput),									/* 8  Ingressi Analogici */
	sizeof(AnalogFeedbackValue),							/* 9: Array di valori dei feedback analogici */
	sizeof(DigInputsDisable),								/* 10  Disable Ingressi Digitali */
	sizeof(AnalogInputDisable),								/* 11 Disable Ingressi Analogici */
	sizeof(AnalogFeedbackDisable),							/* 12 Bitmap disable Analog Feedback */
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
	sizeof(IOM_CommandBufferExternal_0),				/* 13 Buffer comandi DSP */
	sizeof(BLDC_Pump_InfoBuffer),						/* 14 Buffer info DSP */
	sizeof(IOM_ReadRamExternal_0) ,						/* 15 Buffer letture RAM DSP */
#else
	0,													/* 13 Reserved */
	0,													/* 14 Reserved */
	0,													/* 15 Reserved */
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */
	sizeof(LoadsStatus),						/* 16 : Array di stato dei carichi */
	sizeof(GITemp),								/* 17 Reserved */
	sizeof(GICriteria),							/* 18 : Bitmap stato criteri */
	sizeof(Consistency_Current),				/* 19: array consistency per fault res */
	sizeof(Interpreter_Data[0]),				/* 20: Informazioni sul ciclo in esecuzione */
	sizeof(uP_Maintain_Data),					/* 21 : dati uP Maintain */
	sizeof(uP_LoadsControl_Data),				/* 22 : dati uP LoadsControl */
	sizeof(loadsPulseData1),					/* 23 : dati uP LoadsControl - seq1 */
	sizeof(loadsPulseData2),					/* 24 : dati uP LoadsControl - seq2 */
	sizeof(uP_PumpCtrl_Data),					/* 25 : dati uP Pump Control */
	sizeof(uP_Drain_Data),						/* 26 : dati uP Drain */
	sizeof(uP_7liter_Data),				        /* 27 : dati uP 7 litri */
	sizeof(uP_7liter_dirtywater_timer),			/* 28 : contatore tasca 7 litri */
	sizeof(uP_7liter_dirtywater_timer_cfg),		/* 29 :  config contatore tasca 7 litri */
	sizeof(uP_Heat_Data),						/* 30 : dati uP heat  */
	sizeof(uP_AltCtrl_Data),					/* 31 : dati uP ALTERNATO */
	sizeof(Table_JumpIf),						/* 32 : dati uP JumpIF - destinazione */
	sizeof(JmpPhase),							/* 33 : dati uP JumpIF - JIF Phase */
	sizeof(ReturnPoint),						/* 34 : dati uP JumpIF - Return */
	sizeof(Jsr),								/* 35 : dati uP JumpIF  - JSR */
	sizeof(uP_SparklingAid_Data),				/* 36 : dati uP Sparkling Aid */
	sizeof(uP_SetProfile_Data),					/* 37 : dati uP Set Profile */
	sizeof(ActiveAlgorithmBmp),					/* 38 : bitmap Algoritmi */
	sizeof(DelayInMinutes),						/* 39 : valore delay del ciclo */
	sizeof(SelectorFunctionValue),				/* 40 : valore funzione selettore ciclo */
	sizeof(DishRackFunctionValue),				/* 41 : valore funzione selettore cesto */
	sizeof(WaterForRegenerate),					/* 42 : valore funzione acqua per rigenera (da durezza) */
	sizeof (GIValid),							/* 43 :  validità del sensore virtuale*/
	sizeof (uP_Load_Data),						/* 44: dati uP Load */
	sizeof (uP_LoadRestore_Data),				/* 45: dati uP LoadRestore */
	sizeof (uP_Regenerate_Data),				/* 46: dati uP Regenerate */
#if (VIRTUAL_SENSOR_ENABLE == 1)
	sizeof (Virtual_Sensor_Value),				/*
	+ 47 :  Valori dei sensori virtuali*/
	sizeof(dirty_foam_Data),					/* 48 :  algoritmo sporco / schiuma*/
#else
	0,											/* 47 :  Valori dei sensori virtuali*/
	0,											/* 48 :  Valori dei sensori virtuali*/
#endif
	sizeof(ApplCurrentFault),					/* 49 Fault corrente NON di sicurezza */
	sizeof(PltFaultCodes),						/* 50 Fault corrente di sicurezza */
	sizeof(CurrentFault) ,						/* 51: dati preFault */
	sizeof(FaultMgmt_Data),						/* 52  Macchina a stati dei fault */
	sizeof(BufOldReg),							/* 53: buffer regolazioni letto da UI */
	sizeof(BufNewReg),							/* 54 buffer regolazioni scritto da UI */
	sizeof(GITurbidity),						/* 55 : Valore torbidità  */
	sizeof(TurbiditySensorCal),					/* 56 : Struttura per calibrazione torbidità  */
	sizeof(TurbiditySensor),					/* 57 : Struttura per impostazione pwm torbidità  */
	sizeof(TurbidityReads),						/* 58 : Variabile mediata lettura torbidià  */
	sizeof(Reg_WaterLoaded),					/* 59 : acqua per rigenera */
	sizeof(GIWaterLoaded),						/* 60 : Valore acqua caricata in dl */
	sizeof(WaterInDW),							/* 61 : acqua in vasca */
	sizeof(PulseCount),							/* 62: Misura impulsi della turbina */
	sizeof(FltWarning),							/* 63: Flag  Warning ( segnalti in condizione di “prefault” o subito) */
	sizeof(FltFlagsKO),							/* 64: Flag Warning attuali ( legati uP)   */
#ifdef _STATISTICS_ON_
	sizeof(MachineStatistics.globalStats),		/* 65: statistiche globali */
	sizeof(MachineStatistics.lastCycles),		/* 66 : statistiche ultimi cicli eseguiti */
	sizeof(MachineStatistics.lastFault),		/* 67 : ultimi fault */
	sizeof(MachineStatistics.lastPreFault),		/* 68 : ultimi prefault */
	sizeof(MachineStatistics.testCycleStats),	/* 69 : statistiche ciclo di collaudo */
	sizeof(MachineStatistics.blackBox_Data),	/* 70 : black box */
#endif /* _STATISTICS_ON_ */
	sizeof(UIBackupData),						/* 71 : Buffer Backup UI */


};

uint16_t LookupTablePointers[N_LOOKUP_TABLE_ELEMENTS];

const void * const InitLookupTablePointers[N_LOOKUP_TABLE_ELEMENTS]={

	&ResetCtrl.SystemMode,			  					/* 0	: system mode */
	&ResetCtrl.Reset, 									/* 1  Reset flags for PC Control enabled */
	&ResetCnt[0],										/* 2 Contatori cause di reset */
	&CommErr[0][0],										/* 3 Contatori errori I2C */
	&SWTC_PrescalerTable[0],							/* 4: tabella dei valori di prescaler usati dall'applicazione */
	&Time2Death_Counter, 								/* 5 Timer su PC Control */
	&Time_Since_Last_Reset_Counter,						/* 6: Tempo trascorso dall'ultimo reset */
	&DigInputs,											/* 7  Ingressi Digitali */
	&AnalogInput[0],									/* 8  Ingressi Analogici */
	&AnalogFeedbackValue[0],							/* 9: Array di valori dei feedback analogici */
	&DigInputsDisable,									/* 10 Disable Ingressi Digitali */
	&AnalogInputDisable,								/* 11 Disable Ingressi Analogici */
    &AnalogFeedbackDisable,								/* 12: Bitmap disable Analog Feedback */
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
	&IOM_CommandBufferExternal_0[0],					/* 13 Buffer comandi DSP */
	&BLDC_Pump_InfoBuffer[0],							/* 14 Buffer info DSP */
	&IOM_ReadRamExternal_0[0],							/* 15 Buffer letture RAM DSP */
#else
	&DummyVar,											/* 13 Reserved */
	&DummyVar,											/* 14 Reserved */
	&DummyVar,											/* 15 Reserved */
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */
	&LoadsStatus[0],									/* 16 : Array di stato dei carichi */
	&GITemp,											/* 17 Temperature  */
	&GICriteria,										/* 18 Criteri */
	&Consistency_Current[0],							/* 19 : array consistency per fault res */
	&Interpreter_Data[0],								/* 20: Informazioni sul ciclo in esecuzione */
	&uP_Maintain_Data,									/* 21 Dati uP Maintain */
	&uP_LoadsControl_Data,								/* 22 : dati uP LoadsControl */
	&loadsPulseData1,									/* 23 : dati uP LoadsControl - seq1 */
	&loadsPulseData2,									/* 24 : dati uP LoadsControl - seq2 */
	&uP_PumpCtrl_Data,									/* 25 : dati uP Pump Control */
	&uP_Drain_Data,										/* 26 : dati uP Drain */
	&uP_7liter_Data,				        			/* 27 : dati uP 7 litri */
	&uP_7liter_dirtywater_timer,						/* 28 : contatore tasca 7 litri */
	&uP_7liter_dirtywater_timer_cfg,					/* 29 :  config contatore tasca 7 litri */
	&uP_Heat_Data,										/* 30 : dati uP heat  */
	&uP_AltCtrl_Data,									/* 31: dati uP ALTERNATO  */
	&Table_JumpIf[0],									/* 32 : dati uP JumpIF - destinazione */
	&JmpPhase[0],										/* 33 : dati uP JumpIF - JIF Phase */
	&ReturnPoint[0],									/* 34 : dati uP JumpIF - Return */
	&Jsr[0],											/* 35 : dati uP JumpIF  - JSR */
	&uP_SparklingAid_Data,								/* 36 : dati uP Sparkling Aid */
	&uP_SetProfile_Data,								/* 37 : dati uP Set Profile */
	&ActiveAlgorithmBmp[0],								/* 38 : bitmap Algoritmi */
	&DelayInMinutes,									/* 39 : dati DELAY DEL CICLO  */
	&SelectorFunctionValue,								/* 40 : valore funzione selettore ciclo */
	&DishRackFunctionValue,								/* 41 : valore funzione selettore cesto */
	&WaterForRegenerate,								/* 42 : valore funzione acqua per rigenera (da durezza) */
	&GIValid,						 					/* 43 :  validità del sensore virtuale*/
	&uP_Load_Data,										/* 44: dati uP Load */
	&uP_LoadRestore_Data,								/* 45: dati uP LoadRestore */
	&uP_Regenerate_Data,								/* 46: dati uP Regenerate */
#if (VIRTUAL_SENSOR_ENABLE == 1)
	& Virtual_Sensor_Value[0],		 					/* 47 :  Valori dei sensori virtuali*/
	& dirty_foam_Data,				 					/* 48 :  algoritmo sporco / schiuma*/
#else
	&Dummy,												/* 47 :  Valori dei sensori virtuali*/
	&Dummy,												/* 48 :  Valori dei sensori virtuali*/
#endif
	&ApplCurrentFault[0],					/* 49 Fault corrente NON di sicurezza */
	&PltFaultCodes,							/* 50 Fault corrente di sicurezza */
	&CurrentFault[0] ,						/* 51: dati preFault */
	&FaultMgmt_Data[0],						/* 52  Macchina a stati dei fault */
	&BufOldReg[0],							/* 53: buffer regolazioni letto da UI */
	&BufNewReg[0],							/* 54: buffer regolazioni scritto da UI */
	&GITurbidity,							/* 55 : Valore torbidità  */
	&TurbiditySensorCal,					/* 56 : Struttura per calibrazione torbidità  */
	&TurbiditySensor,						/* 57 : Struttura per impostazione pwm torbiditÃ   */
	&TurbidityReads,						/* 58 : Variabile mediata lettura torbiditÃ  */
	&Reg_WaterLoaded,						/* 59 : acqua per rigenera */
	&GIWaterLoaded,							/* 60: Valore acqua caricata in dl */
	&WaterInDW,								/* 61 : acqua in vasca */
	&PulseCount,							/* 62: Misura impulsi della turbina */
	&FltWarning,							/* 63: Flag  Warning ( segnalti in condizione di “prefault” o subito) */
	&FltFlagsKO,							/* 64: Flag Warning attuali ( legati uP)   */
#ifdef _STATISTICS_ON_
	&MachineStatistics.globalStats,				/* 65 : statistiche globali */
	&MachineStatistics.lastCycles[0],			/* 66 : statistiche ultimi cicli eseguiti */
	&MachineStatistics.lastFault[0],			/* 67 : ultimi fault */
	&MachineStatistics.lastPreFault[0],			/* 68 : ultimi prefault */
	&MachineStatistics.testCycleStats,			/* 69 : statistiche ciclo di collaudo */
	&MachineStatistics.blackBox_Data[0],		/* 70 : black box */
#endif /* _STATISTICS_ON_ */
	&UIBackupData[0],							/* 71	Buffer Backup UI */
	(void*)(&LookupTableDataSizes[0]),
};


/**
EXTENDED IDENT TABLE
*/
const STD_ExtendedIdentTableType ExtendedIdentTable =
{
	{
		STD_IDENT32_HDR_MCU(IDT_MODEL, IDT_MANUFACTURER),
		STD_IDENT32_HDR_BOARD_APPLIANCE(IDT_BOARD, IDT_APPLIANCE),
		STD_IDENT32_HDR_INFO(IDT_BLOCK_SIZE,IDT_ENDIAN,IDT_CRC,STD_IDENT32_VER_0),

		STD_IDENT32_BODY_FWVERSION_XX(IDT_FW_VER_VERSION_IND),
		STD_IDENT32_BODY_FWVERSION_YY(IDT_FW_VER_VERIFICATION_IND),
		STD_IDENT32_BODY_FWVERSION_ZZ(IDT_FW_VER_MODIFICATION_IND),

		STD_IDENT32_BODY_PRODUCT_CFG1(),
		STD_IDENT32_BODY_PRODUCT_CFG0(PRODUCT_CFG_RC_CONNECT_CLUSTER, PRODUCT_CFG_WBOX_PRESENT),

		STD_IDENT32_BODY_PRJCODE(IDT_EVO),

		(sizeof(STD_IdentTableStruct32bitType) / sizeof(uint32_t)),
		(sizeof(STD_IdentTableStruct16bitType) / sizeof(uint16_t)),
		(sizeof(STD_IdentTableStruct08bitType) / sizeof(uint8_t)),
	},
	{
		IDT_APPLICATION_RAM_START_ADDRESS,
		RAM_SIZE,
		(uint32_t)&InitLookupTablePointers[0],
		APPL_WBOX_ADDRESS,
		APPL_PDM_ADDRESS,
		APPL_SETTING_ADDRESS,
		APPL_BCKP_STAT_ADDRESS,
		STD_IDENT32_STRINGFILE_ADDRESS_DEFAULT,
	},
	{
		IDT_LUT_ITEMS_NUM,
		APPL_WBOX_SIZE,
		(APPL_SETTING_SIZE_W * 2),
		(APPL_APPL_BCKP_STAT_SIZE_W * 2),
		STD_IDENT32_UCVFW_DEFAULT,
		STD_IDENT32_STRINGFILE_SIZE_DEFAULT,
	},
	{
		(IDT_EEP_PAGE_NUM + 1),  /* IDT_EEP_PAGE_NUM is N-1 for compatibility with the 16bit table */
		STD_IDENT32_STRINGFILE_NUM_OF_PAGES_DEFAULT
	}
};


IdentTableStruct_t IdentTable = {
{
#if defined(__BIT_ORDER_MSB_FIRST__)
	IDT_MODEL,							/* Model */
	IDT_MANUFACTURER,					/* Manufacturer */
	IDT_BOARD,
	IDT_APPLIANCE,  					/* Appliance */

	CONFIG_CP_EXTENDED_REQUESTS_ENABLED,
	IDT_BLOCK_SIZE,
	IDT_FORMAT,
	IDT_ENDIAN,
	IDT_CRC,
	IDT_EVO,							/* Evo */
#else /* __BIT_ORDER_MSB_FIRST__ */
	IDT_MANUFACTURER,					/* Manufacturer */
	IDT_MODEL,							/* Model */

	IDT_APPLIANCE,  					/* Appliance */
	IDT_BOARD,  						/* Board */

	IDT_EVO,							/* Evo */
	IDT_CRC,
	IDT_ENDIAN,
	IDT_FORMAT,
	IDT_BLOCK_SIZE,
	IDT_FREE,
#endif /* !__BIT_ORDER_MSB_FIRST__ */
	},

	{
	IDT_FW_VER_VERSION_IND,				/* Firmware version */
	IDT_FW_VER_VERIFICATION_IND,
	IDT_FW_VER_MODIFICATION_IND,
	},

	IDT_APPLICATION_RAM_START_ADDRESS,	/* Microcontroller RAM Start address */

	IDT_EEP_START_ADDR,					/* Setting start address */

	0,									/* dummy Lookup table, this field must me updated in ram  */
#if defined(__BIT_ORDER_MSB_FIRST__)
    IDT_EEP_PAGE_NUM,
    IDT_EEP_PAGE_SIZE_IN_WORDS,
#else /* __BIT_ORDER_MSB_FIRST__ */
    IDT_EEP_PAGE_SIZE_IN_WORDS,
    IDT_EEP_PAGE_NUM,
#endif /* !__BIT_ORDER_MSB_FIRST__ */
	IDT_LUT_ITEMS_NUM,						/* LUT size (words) */
	IDT_BACKUPAREA_START_ADDR,			/* BackUp Area */
	0,									/* Statistics Area, this field must me updated in ram */
	IDT_BACKUPAREA_SIZE,				/* Size of the backup area */
	IDT_STATISTICSAREA_SIZE,			/* Size of the statistic area */
	IDT_SKELETON_RAM_AREA_START_ADDR,	/* SKELETON RAM Start */
 	IDT_CUSTOM_RAM_AREA_START_ADDR,		/* Application RAM start address */
	0,									/* Last Fault address, this field must me updated in ram */
	0,									/* Current Fault  address, this field must me updated in ram */
	IDT_WBOX_ADDRESS,					/* Factory Test Data Start Address */
	IDT_STATISTIC_ABS_DIM,				/* Size of the absolute statistics area */
	IDT_STATISTIC_REL_DIM,				/* Size of the daily statistics area */
	IDT_PDM_ADDRESS,					/* PDM Code Start Address */
	IDT_PROD_CFG_BITMAP					/* Product Config Bitmap Values */
};

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif

#ifndef _IDENTCONST_ENABLE
/*
this function initializes ident 
-LUTaddress
-StatisticsAddress
-LastFaultAddress
-CurrFaultAddress
*/
void Init_IdentTablePointers(void)
{
	/* Lookup Table start address */
	IdentTable.LUTaddress = CP_GetPPAddr(&LookupTablePointers[0]);
	/* Statistics Area start address */
	IdentTable.StatisticsAddress = CP_GetPPAddr(&MachineStatistics);
	/* Last Fault address */
	IdentTable.LastFaultAddress = CP_GetPPAddr(&MachineStatistics.lastFault[0].faultCodes);
	/* Current Fault address */
	IdentTable.CurrFaultAddress = CP_GetPPAddr(&CurrentFault);
}
#endif
void Init_LookupTablePointers(void)
{
	int i;
	for(i=0; i<N_LOOKUP_TABLE_ELEMENTS;i++)
	{
		LookupTablePointers[i] = CP_GetPPAddr((uint8_t*)InitLookupTablePointers[i]);
	}
}

#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wpointer-to-int-cast"
#endif
#if defined(FREESCALE_MCF51_AG128)
#pragma cplusplus off
#endif


/* ***************** */
/* Exported function */
/* ***************** */

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
