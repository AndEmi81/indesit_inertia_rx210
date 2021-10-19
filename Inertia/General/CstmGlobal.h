/**
Header file del modulo variabili globali

\file		CstmGlobal.h
\ingroup	CstmGlobal
\date		11/07/2012
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2012-2003, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/
#ifndef __CSTMGLOBAL_H
#define __CSTMGLOBAL_H


/* ******** */
/* Includes */
/* ******** */
#ifndef AUTOCONF_INCLUDED
#include "autoconf.h"
#endif /* !AUTOCONF_INCLUDED */
#include "Config.h"
#include "APP_Config.h"
#include "CstmTypes.h"
#include "CstmDefines.h"
#include "OutputManagerExp.h"
#include "APP_Global.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifndef __CSTM_GLOBAL
 #define	_DW_GLOBAL_ extern
 #define	_DW_INIT_(initVal)
#else
 #define	_DW_GLOBAL_
 #define	_DW_INIT_(initVal) = { initVal }
#endif
/*
Ogni Appliance deve definire la label opportuna
Es. _CK_GLOBAL_, _DW_GLOBAL_, etc...
*/
enum CountersIndex{
/* Questo enum serve per indicizzare i contatori memorizzati in SWTC_CounterConfigurationTable e SWTC_CountersTable */

	MP_MAINTAIN_M_COUNTER = SWTC_APP_PLT_COUNTERS_NUM,/* + SWTC_PLATFORM_COUNTERS_NUM,*/
	MP_LOADCONTROL_M_COUNTER1,				/* +1 */
	MP_LOADCONTROL_M_COUNTER2,				/* +2 */
	MP_DRAIN_M_COUNTER,						/* +3 */
	SLITER_DIRTYWATER_TIMEOUT_M_COUNTER,	/* +4 */
	MP_7LITER_M_COUNTER,					/* +5 */
	MP_ALTCTRL_S_COUNTER,					/* +6 */
	MP_ALTERNATE_UP_VS_FILTER_S_TIMER,		/* +7 */
	MP_LOAD_LEVTOTIME_S_COUNTER,			/* +8 */
	MP_LOAD_FULLSTABILITY_S_COUNTER,		/* +9 */
	MP_LOAD_TIMELOAD_S_COUNTER,				/* +10 */
	MP_LOAD_PUMP_ACTIVATION,				/* +11 */
	MP_SETDRYPROFILE_M_COUNTER,				/* +12 */
	MP_SPARKLINGAID_M_COUNTER,				/* +13 */
	HIO_BULK_OFF_DELAY_S_COUNTER,			/* +14 */
	SFT_FAN_STARTUP_S_COUNTER,				/* +15 */
	SFT_DRAINPUMP_OVER_HEATING_M_COUNTER,	/* +16 */
	SPV_DELAY_TIMER_S_COUNTER,				/* +17 */
	MP_LOADRESTORE_S_COUNTER,				/* +18 */
	MP_REGENERATE_EVDELAY_S_COUNTER,		/* +19 */
	SFT_DOOR_CLOSE_PAUSE_S_COUNTER,			/* +20 */
	SFT_RES_PRES_STABLE_S_COUNTER,			/* +21 */
#if (VIRTUAL_SENSOR_ENABLE == 1)
	HIO_STABILITY_FILTER_VS,				/* +22*/
	AAL_END_ALWAYS3_M_TIMER,				/* +23 */
	READ_VS_M_TIMER,		                /* +24 */
	SFT_FILTER_VS56_STATE_W,				/* +25 */
	AAL_STAB_SENS_S_TIMER,					/* +26 */
	AAL_STAB_STATO3_S_TIMER,				/* +27 */
	SFT_EVOORDRAIN_COUNTER,					/* +28*/
#endif
	SFT_DRY_RES_DELAYACT_S_COUNTER,			/* +29 */
	PULSE_CHECK_DELAY_S_COUNTER,			/* +30 */
	PULSE_STILL_M_COUNTER,					/* +31 */
	SFT_EVCHECK_S_COUNTER,					/* +32 */
	UNBLOCK_4EV_VS_TIMER,					/* +33 */
	FLT_HEATTOUT_M_COUNTER,					/* +34 */
	FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER,	/* +35 */
	FLT_OVERHEATING_RES_OFF_M_COUNTER,		/* +36 */
	STC_TEMP_CYCLE_STATISTIC_M_COUNTER,		/* +37 */
	MP_LOADTIMEOUT_REST_M_COUNTER,			/* +38  ex MP_PUMP_LOAD_M_COUNTER*/
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
	PP_ENPHELAPSEDTIME_UPCOUNTER,			/* + 39 */
	PP_TIMEBTWPHASES_COUNTER,				/* + 40 */
	PP_ENPHNOSPECIALSELAPSEDTIME_UPCOUNTER,	/* + 41*/
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

};
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/*
******************************************************
Variables/Costants not requiring initialization
******************************************************
*/

/*
******************************************************
Variables/Costants requiring initialization
******************************************************
*/
_DW_GLOBAL_ SWTC_InternalCounterType SWTC_InternalCounter[SWTC_PRESCALERS_NUM];		/* No Backup */

/**
VARIABLES
*/
_DW_GLOBAL_ IdentTableStruct_t IdentTable;
_DW_GLOBAL_ const STD_ExtendedIdentTableType ExtendedIdentTable;
_DW_GLOBAL_ uint8_t ApplicationFlags;				/* No Backup */
_DW_GLOBAL_ uint32_t cycle_loaded_water;
/**
CONSTANTS
*/
_DW_GLOBAL_ const SWTC_PrescalerTableType SWTC_PrescalerTable[SWTC_PRESCALERS_NUM-1];
_DW_GLOBAL_	SWTC_CounterConfigurationType* const SWTC_CounterConfigurationTable[SWTC_COUNTERS_NUM];
_DW_GLOBAL_	void* const SWTC_CountersTable[SWTC_COUNTERS_NUM];

_DW_GLOBAL_ uint8_t DishRackFunctionValue;
/* Valore della funzione Zone Wash */
_DW_GLOBAL_ uint8_t	ZoneWashFunctionValue;			/* No Backup */

/* Valore della funzione Zone Wash */
_DW_GLOBAL_ uint8_t	RinseAidFunctionValue;			/* No Backup */

/** Relevant functions values */
/* Valore della funzione selettore */
_DW_GLOBAL_ uint8_t SelectorFunctionValue;			/* No Backup */	/*LUT*/
/* Valore della funzione Durezza Acqua = Litri per rigenera */
_DW_GLOBAL_ uint8_t WaterForRegenerate;				/* No Backup */
/* Valore della funzione join-leave */
_DW_GLOBAL_ uint8_t Join_Leave_Reg;		/* No Backup */

_DW_GLOBAL_ uint8_t NoSalt_Reg;						/* Si Backup */
/* Ciclo avviato dalla START_CONDITION in PROGRAMMING */
_DW_GLOBAL_ uint8_t StartedCycle;					/* Si Backup*/

_DW_GLOBAL_ uint16_t DelayInMinutes;						/* Si Backup */
_DW_GLOBAL_ SWTC_SCounterVariableType DelayTimer;	/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType DelayTimer_Config;
_DW_GLOBAL_ Reg_WaterLoaded_Struct Reg_WaterLoaded; /* contatore acqua accumulata ai fini della rigenera */
/**/
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
/**
BLDC COMMUNICATION
*/
_DW_GLOBAL_ GIOM_LoadStruct BLDC_PumpReq_New[NR_OF_BLDC_PUMPS];			/* No Backup */
_DW_GLOBAL_ GIOM_LoadStruct BLDC_PumpReq_Old[NR_OF_BLDC_PUMPS];			/* No Backup */
_DW_GLOBAL_ uint8_t BLDC_Pump_InfoBuffer[NR_OF_BLDC_PUMPS][IOM_EXTERNAL_0_INFOBUFFER_SIZE];	/* No Backup */	/*LUT*/
#ifdef IOM_EXTERNAL_0_ENABLED
_DW_GLOBAL_ uint8_t ExternalLoad1Demo_InfoBuffer[IOM_EXTERNAL_0_INFOBUFFER_SIZE];		/* No Backup */	/*LUT*/
#endif
#ifdef IOM_EXTERNAL_1_ENABLED
_DW_GLOBAL_ uint8_t ExternalLoad2Demo_InfoBuffer[IOM_EXTERNAL_1_INFOBUFFER_SIZE];
#endif

#endif

/**
INPUTS
*/	
/*Generic Inputs*/
_DW_GLOBAL_ uint8_t   GICriteria;	  /* stato criteri */	/*No Backup*/	/*LUT*/
_DW_GLOBAL_ uint8_t	  GITemp;			  /* Array stato ingressi temperatura */	  /*No Backup*/	/*LUT*/
_DW_GLOBAL_ uint16_t  GIWaterLevel; /* ingresso pressostato lineare (dl)*/	  /*No Backup*/	/*LUT*/
_DW_GLOBAL_ uint16_t  GIWaterLoaded;  /* ingresso turbina (dl)*/	            /*No Backup*/	/*LUT*/
_DW_GLOBAL_ uint16_t  GIValid;		  /* No Backup */
_DW_GLOBAL_ uint16_t  PulseLoaded;  /* ingresso turbina espresso in impulsi */  /*No Backup*/
_DW_GLOBAL_ uint8_t   GICriteriaOld;  /* stato criteri old : per sale*/					/*No Backup ??*/
_DW_GLOBAL_ uint8_t   GITurbidity;  /* torbidità*/
_DW_GLOBAL_ uint8_t   Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_NUM];
_DW_GLOBAL_ uint8_t   Virtual_Sensor_Value_jumpifcopy[GI_VIRTUAL_SENSOR_NUM];
_DW_GLOBAL_ uint8_t   FlowMeterDivider;

_DW_GLOBAL_ uint8_t   WaterTemp_At_CycleStart;			/*APP is used for store water temperature after load*/


_DW_GLOBAL_ uint8_t Pump_Pos[N_MAX_LD_PUMP];			/* No Backup */
_DW_GLOBAL_ uint8_t Res_Pos[N_MAX_LD_RES];				/* No Backup */
_DW_GLOBAL_ uint8_t EV_Pos[N_MAX_LD_EV];				/* No Backup */
_DW_GLOBAL_ uint8_t Disp_Pos;							/* No Backup */
_DW_GLOBAL_ uint8_t Alt_Pos;							/* No Backup */
_DW_GLOBAL_ uint8_t RegDev_Pos[N_MAX_LD_REG_DEV];		/* No Backup */
_DW_GLOBAL_ uint8_t Fan_Pos[N_MAX_LD_FAN];
_DW_GLOBAL_ uint8_t Ozonator_Pos[N_MAX_LD_OZONE];
_DW_GLOBAL_ uint8_t Lamp_Pos;

_DW_GLOBAL_ L_PARAM LoadsReq_New[N_MAXLOADS];		/* No Backup */							/*ZERO PAGE??*/
_DW_GLOBAL_ L_PARAM LoadsReq_Old[N_MAXLOADS];		/* Backup solo per alcuni carichi */	/*ZERO PAGE??*/

_DW_GLOBAL_ L_PARAM DrainSftReq_New;					/* No Backup */
_DW_GLOBAL_ L_PARAM LoadEVFltReq_New;					/* No Backup */
_DW_GLOBAL_ uint8_t Bulk_DSP_Off_Delay;		/* No Backup */
_DW_GLOBAL_ Surge_Fan_Params_t Surge_Fan_Params;
_DW_GLOBAL_ uint8_t Dry_SfyTime_Res_On;
_DW_GLOBAL_ LoadEVCheck_Struct LoadEVCheck;       /* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType time_4_unblock_EV_VS_config;
_DW_GLOBAL_ SWTC_SCounterVariableType time_4_unblock_EV_VS_counter;


_DW_GLOBAL_ SWTC_CounterConfigurationType Bulk_Off_Delay_timer_cfg;
_DW_GLOBAL_ SWTC_SCounterVariableType Bulk_Off_Delay_timer;			/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType Timer_Fan_StartUp_Config;
_DW_GLOBAL_ SWTC_SCounterVariableType Timer_Fan_StartUp;/* No Backup */
_DW_GLOBAL_ uint8_t Salt_Filtro_Stab;
_DW_GLOBAL_ uint8_t Salt_Time_Of_Stability;
_DW_GLOBAL_ SWTC_MCounterVariableType Timer_Pump_OverHeating_Drain;/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType Timer_Pump_OverHeating_Drain_config;




/**
PUNTATORI SETTING FILE
*/
_DW_GLOBAL_ Loader_Struct Tab_OutputData;	/* Puntatore e Lunghezza campo dati Output Map */     /*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_GIData;					/* Puntatore e Lunghezza campo dati GI Map */			    /*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_SFT_7liter;
_DW_GLOBAL_ Loader_Struct uP7liter_setting_data;	    /** puntatore al campo dati uP 7 liter */	      /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct Tab_HWCTurbidity;    			/* Puntatore e Lunghezza campo dati Turbidity *//*No Backup*/
_DW_GLOBAL_ Loader_Struct uPDrain_setting_data;	       	/** puntatore al campo dati uP Drain */	          /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct uPLoadsControl_setting_data;  /** puntatore al campo dati uP LoadsControl */    /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct uPMaintain_setting_data;	    /** puntatore al campo dati uP Maintain */	      /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct uPPumpCtrl_setting_data;      /** puntatore al campo dati uP PumpControl */	    /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct Tab_SFT_MotorWash; 			/* Puntatore e Lunghezza campo dati Safety parametri per la pompa lavaggio*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct uPAlternateCtrl_setting_data; /** puntatore al campo dati uP AlternateControl*/	/* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct uPHeat_setting_data; 	      	/** puntatore al campo dati uP Heat */	            /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct uPLoad_setting_data;          /** puntatore al campo dati uP Load */	            /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct Tab_HWCPressSensorData; 		/* Puntatore e Lunghezza campo dati Pressure Sensor */			    /*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_HWCSPR_TimeBase; 			/* Puntatore e Lunghezza campo dati Asciugatura */
_DW_GLOBAL_ Loader_Struct uPSparklingAid_setting_data;  /** puntatore al campo dati uP SparklingAid */	  /* Si Backup (Data)*/
_DW_GLOBAL_ Loader_Struct Tab_SFT_EV;  					/* Puntatore e Lunghezza campo dati Safety Drain */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_SFT_MotorDrain;			/* Puntatore e Lunghezza campo dati Safety parametri per la pompa scarico*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_HWCRegenerateData;  		/* Puntatore e Lunghezza campo dati Regenerate */			    /*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_HWCFuncNum;   			/* Puntatore e Lunghezza campo dati Numero Funzioni*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_HWCCyclesNum;				 /* Puntatore e Lunghezza campo dati Cycles Data */				/*No Backup*/

_DW_GLOBAL_ Loader_Struct Tab_SFT_Door;    				 /* Puntatore e Lunghezza campo dati Safety Door */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_SFT_PressRes;				 /* Puntatore e Lunghezza campo dati Safety Press/Res */	/*No Backup*/

_DW_GLOBAL_ Loader_Struct Tab_LoadFault;  				/* Puntatore e Lunghezza campo dati Fault caricamento*/	/*No Backup*/

_DW_GLOBAL_ Loader_Struct Tab_HeatFault;  				/* Puntatore e Lunghezza campo dati Fault riscaldamento*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_MainBoardFault; 			 /* Puntatore e Lunghezza campo dati Fault main board*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_ExtDevFault;  			/* Puntatore e Lunghezza campo dati Fault ext dev (BLDC)*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_ExtDevFaultDrain; 		 /* Puntatore e Lunghezza campo dati Fault ext dev drain (BLDC drain)*/	/*No Backup*/

_DW_GLOBAL_ Loader_Struct Tab_HeatFault_Special;		/* Puntatore e Lunghezza campo dati Fault riscaldamento Special Cycles*/	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_MainBoardFault_Special;  	/* Puntatore e Lunghezza campo dati Fault main board Special Cycles */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_LoadFault_Special;  		/* Puntatore e Lunghezza campo dati Fault caricamento Special Cycles */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_DrainFault;				 /* pnt Drain Fault Special Cycles */
_DW_GLOBAL_ Loader_Struct Tab_DrainFault_Special; 		/* pnt Drain Fault Special Cycles */
_DW_GLOBAL_ Loader_Struct Tab_ExtDevFault_Special;  	/* Puntatore e Lunghezza campo dati Fault ext dev (BLDC) Special Cycles */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_ExtDevFaultDrainSpecial;  /* Puntatore e Lunghezza campo dati Fault ext dev drain (BLDC) Special Cycles */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_VSFaultSpecial; 			 /* Puntatore e Lunghezza campo dati Fault virtual sensor */	/*No Backup*/
_DW_GLOBAL_ Loader_Struct Tab_VSFault;  				/* Puntatore e Lunghezza campo dati Fault virtual sensor */	/*No Backup*/

/**
ALGORITMI
*/

_DW_GLOBAL_ Loader_Struct dirty_foam_setting_data;
_DW_GLOBAL_ TurbiditySensorCalibration_Struct TurbiditySensorCal; /* Backup solo campo .val*/
_DW_GLOBAL_ TurbiditySensor_t TurbiditySensor;
_DW_GLOBAL_ bool_t Flag_levTOtimeRef_update;		/* SI Backup */

#if (VIRTUAL_SENSOR_ENABLE == 1)
_DW_GLOBAL_ Loader_Struct Tab_HWCVS_data;
_DW_GLOBAL_ ReadVS_struct ReadVS_Data;
_DW_GLOBAL_ al_DirtyFoam_Struct dirty_foam_Data;	/* SI Backup */ /*LUT*/
_DW_GLOBAL_ state5_struct State5;
_DW_GLOBAL_ SWTC_SCounterVariableType Filter_VS_56_State_timer;
_DW_GLOBAL_ SWTC_CounterConfigurationType Filter_VS_56_State_cfg;
_DW_GLOBAL_ SWTC_CounterConfigurationType Stability_Filter_VS_cfg;
_DW_GLOBAL_ SWTC_SCounterVariableType Stability_Filter_VS_timer;
_DW_GLOBAL_ SWTC_CounterConfigurationType Aal_DF_end_always3_config;
_DW_GLOBAL_ SWTC_MCounterVariableType Aal_DF_end_always3_timer;/* Si Backup */
_DW_GLOBAL_ New_SafetyVs_Struct New_safety_VS;
_DW_GLOBAL_ SftPulseWarn_Struct SftPulseWarn;     /*No Backup*/
#endif

_DW_GLOBAL_ SparklingReads_t SparklingReads;		/* lettura brillantante */		  /*No Backup*/
/**
INFO HW CONFIG
*/
_DW_GLOBAL_ HWC_Flags_Struct HWC_Flags;


/**
CONSISTENCIES
*/
/** array stato corrente coerenza feedback-carichi */	 
_DW_GLOBAL_ uint8_t Consistency_Current[N_MAXLOADS];		/* No Backup */
/**/
/*SAFETY*/
_DW_GLOBAL_ SWTC_CounterConfigurationType DoorClosedEvent_Pause_timer_cfg;
_DW_GLOBAL_ SWTC_SCounterVariableType DoorClosedEvent_Pause_timer;/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType Res_Press_Stable_Cnt_Config;
_DW_GLOBAL_ SWTC_SCounterVariableType Res_Press_Stable_Counter;		/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType DryResDelayActivation_cnt_cfg;
_DW_GLOBAL_ SWTC_SCounterVariableType DryResDelayActivation_cnt;/* No Backup */

_DW_GLOBAL_ UINT16_FIELD (Sft_WashPumpSpeedLow);
_DW_GLOBAL_ UINT16_FIELD (Sft_WashPumpSpeedHigh);
_DW_GLOBAL_ Sft_DrainPP_Struct Sft_DrainPP; /* No Backup?? */
/**
FAULT
*/
_DW_GLOBAL_ sft_module_faults_t FltSafetyModuleStatus[SFT_NUM_SAFETY_MODULES];	/* No Backup - LUT  EA_ controllare perchè qui non ci sono fault.*/

_DW_GLOBAL_ uint16_t FltFlagsKO;     /* Si Backup */
_DW_GLOBAL_ FltFlagsVS_Struct FltFlagsVS;	  /* SI Backup */
_DW_GLOBAL_ SWTC_MCounterVariableType PulseCounterStill_timer;		/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType PulseCntStill_timer_cfg ;
_DW_GLOBAL_ FltWarning_Struct FltWarning;       /*Si Backup */
_DW_GLOBAL_ FltBLDCUnblock_Struct FltBLDCUnblock;
_DW_GLOBAL_ FltTout_Struct FltHeatToutData;     /* Si Backup */
_DW_GLOBAL_ FltTout_Struct FltLoadToutData;     /* Si Backup */
_DW_GLOBAL_ uint8_t FltGICriteria;			        /* NO Backup */
_DW_GLOBAL_ uint8_t FltGIPulseCount;	          /* NO Backup */
_DW_GLOBAL_ uint8_t FltGITemp;	                /* NO Backup */
_DW_GLOBAL_ uint8_t FltGITurbidity;	            /* NO Backup */
_DW_GLOBAL_ uint8_t FltBLDCComm_Tout;
_DW_GLOBAL_ uint8_t FltBLDCMotor_Tout;
_DW_GLOBAL_ uint8_t FltLoadEVKOStatus;

_DW_GLOBAL_ SWTC_CounterConfigurationType PulseCntCheckDelay_timer_cfg;
_DW_GLOBAL_ SWTC_SCounterVariableType PulseCounterCheckDelay_timer;	/* No Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType Flt_UnderVoltage_ResOff_timer_cfg;
_DW_GLOBAL_ SWTC_MCounterVariableType Flt_UnderVoltage_ResOff_timer;/* Sì Backup */
_DW_GLOBAL_ SWTC_CounterConfigurationType Flt_overheating_ResOff_timer_cfg;
_DW_GLOBAL_ SWTC_MCounterVariableType Flt_overheating_ResOff_timer;/* Sì Backup */
_DW_GLOBAL_ Pump_Load_Struct Pump_Load_Data; /* Si Backup */



/**
MICROPROGRAMMI
*/
/* Maintain */
_DW_GLOBAL_ uP_Maintain_Struct uP_Maintain_Data;	/* SI Backup */	/*LUT - SAT Header */
/** LoadsControl */
_DW_GLOBAL_ uP_LoadsControl_Struct uP_LoadsControl_Data;		    /* Si Backup */	/*LUT*/
_DW_GLOBAL_ loadsPulse_Struct loadsPulseData1; /* Si Backup */ /*LUT*/
_DW_GLOBAL_ loadsPulse_Struct loadsPulseData2; /* Si Backup */ /*LUT*/
/** PumpControl */
_DW_GLOBAL_ uP_PumpControl_Struct uP_PumpCtrl_Data;			/* SI Backup */
/* Drain */
_DW_GLOBAL_ uP_Drain_Struct uP_Drain_Data;				/* Si Backup */ /*LUT*/
/* 7 litri */
_DW_GLOBAL_ uP_7liter_Struct uP_7liter_Data;		/* Sì Backup */ /*LUT */
_DW_GLOBAL_ SWTC_CounterConfigurationType uP_7liter_dirtywater_timer_cfg;
_DW_GLOBAL_ SWTC_MCounterVariableType uP_7liter_dirtywater_timer;/* Sì Backup */
/** AlternateControl */
_DW_GLOBAL_ uP_AltCtrl_Struct uP_AltCtrl_Data;			/* SI Backup */ /* LUT */
/*Heat LUT*/
_DW_GLOBAL_ uP_Heat_Struct uP_Heat_Data;			/* SI Backup */ /*LUT*/
/** Load */
_DW_GLOBAL_ uP_Load_Struct uP_Load_Data;			/* SI Backup */
/** Load Restore */
_DW_GLOBAL_ uP_LoadRestore_Struct uP_LoadRestore_Data;	/* SI Backup */ /*LUT*/
/* Regenerate */
_DW_GLOBAL_ uP_Regenerate_Struct uP_Regenerate_Data;	/* SI Backup */ /* LUT */
/* Set Profile */
_DW_GLOBAL_ uP_SetProfile_Struct uP_SetProfile_Data;	/* SI Backup */ /* LUT */
/** SparklingAid */
_DW_GLOBAL_ uP_SparklingAid_Struct uP_SparklingAid_Data;/* Si Backup */ /*LUT*/

_DW_GLOBAL_ uint8_t WaterInDW;						/* SI Backup */	 /*LUT*/

_DW_GLOBAL_ void* const JIF_DataTable[NDATATYPES];
/*
INIT & RESTORE
*/


_DW_GLOBAL_ uint8_t BufOldReg[NUM_REG_MAX];			/* Si Backup *//* Buffer delle vecchie regolazioni Main */
_DW_GLOBAL_ uint8_t BufNewReg[NUM_REG_MAX];			/* No Backup */ /* Buffer delle nuove regolazioni da modulo Visual */

/* Buffer delle informazione display (pacchetto WriteUIInfo) */
_DW_GLOBAL_ uint8_t BufUIInfo[NUM_UI_INFO_MAX];		/* No Backup */

/* Sincronizzazione UI-Main mediante la sequenza READ_STATUS - READ_REGS - WRITE_REGS */
_DW_GLOBAL_ uint8_t recoveryComProc;				/* No Backup */
_DW_GLOBAL_ uint8_t ConfigInitResult;				/* No Backup */
_DW_GLOBAL_ Interpreter_Struct Paused_Intpt_Data[APP_N_COMP]; /* Si Backup */
_DW_GLOBAL_ uint8_t UIBackupData[N_UI_BACKUP_DATA];	/* SI Backup */
_DW_GLOBAL_ TurbidityReads_t   TurbidityReads;	/* lettura torbidità */         /*No Backup */	/*LUT*/
_DW_GLOBAL_ uint8_t   RestoreType;
/*Buffer degli ID delle funzioni presenti nel setting file*/
_DW_GLOBAL_ uint8_t BufFunctionsIDs[NUM_REG_MAX];	/* No Backup */
_DW_GLOBAL_ uint8_t ApplicationState;				/* No Backup */

/**
STATISTICS
*/
/* Statistiche generali accessibili dall'esterno. Se _STATISTICS_ON_ non è definita
   contiene solo i LastFault */
_DW_GLOBAL_ TempCycleStatistics_t TempCycle_Stats;         /* Struttura temporanea per statistiche ultimo ciclo */ /* SI backup */
_DW_GLOBAL_ uint8_t Statistics_update_flag;/** Byte di Flags vari,per statistiche */	/* Si Backup */
#ifdef _STATISTICS_ON_
_DW_GLOBAL_ Loader_Struct Tab_FunctionsStatistics;	       /* Puntatore e Lunghezza campo dati statistiche funzioni */		/*No Backup*/
_DW_GLOBAL_ uint8_t oldJoinValue;
_DW_GLOBAL_ uint32_t PrevEnergyCount;                         /* SI Backup */
_DW_GLOBAL_ uint16_t PrevWaterCount;                         /* SI Backup */
_DW_GLOBAL_ uint8_t PrevUnblockCountersSum;					/* SI Backup */
#endif /* _STATISTICS_ON_*/

/*
Buffer dello stato dell'applicazione.
E' stato definito qui perchè deve vedere tutte
le variabili definite sopra.
*/
_DW_GLOBAL_ uint8_t MachineStateBuf[DIM_MACHINE_STATE_BUF];	/* No Backup */
_DW_GLOBAL_ uint8_t ExternalRequest;				/* ModalitÃ  speciale di funzionamento (AUTOTEST, COLLAUDO) richiesta dall'esterno*/ /* No Backup */
/** Relevant functions values */
/* Valore della funzione selettore */
_DW_GLOBAL_ uint8_t uPDoorStatus;   /* No Backup */
_DW_GLOBAL_ uint8_t Flags_1;	/** Byte di Flags vari, accessibili con JumpIF */	/* Si Backup */
_DW_GLOBAL_ uint8_t Flags_2;	/** Byte di Flags vari,  */	/* Si Backup */
_DW_GLOBAL_ uint8_t Flags_3;	/** Byte di Flags vari, accessibili con JumpIF */	/* Si Backup */
_DW_GLOBAL_ uint8_t UI_Info_Ctrl; /* byte di controllo di validità del pacchetto inviato via i2c */
_DW_GLOBAL_ void *const TabMachineStateAddr[NUM_MACHINE_STATE_VARS];
_DW_GLOBAL_ const uint8_t TabMachineStateDim[NUM_MACHINE_STATE_VARS];


/**
DSP COMMUNICATION
*/
_DW_GLOBAL_ uint8_t ExtMotor_DataBuffer[IOM_EXTERNAL_0_GLOBAL_DATA_BLOCK_SZ];
_DW_GLOBAL_ EngineData3PH_Struct DSPInfoInterpreterBuffer;					/* No Backup */

/**
APP
*/
_DW_GLOBAL_ LIB_Fixed_ValueType app_alpha_voltage_on_power;
_DW_GLOBAL_ LIB_Fixed_ValueType app_alpha_voltage_on_durationON;
_DW_GLOBAL_ LIB_Fixed_ValueType app_alpha_voltage_temperature_on_durationON;
_DW_GLOBAL_ LIB_Fixed_ValueType app_alpha_temperature_on_durationOFF;

_DW_GLOBAL_ LIB_Fixed_ValueType Vref;
_DW_GLOBAL_ LIB_Fixed_ValueType Tref;



/**
DUMMY & DEBUG
*/
_DW_GLOBAL_ uint8_t Dummy;

/* ******************* */
/* Function prototypes */
/* ******************* */
#ifndef _IDENTCONST_ENABLE
void Init_IdentTablePointers(void);
#endif /* !_IDENTCONST_ENABLE */
void Init_LookupTablePointers(void);

#endif /* __CSTMGLOBAL_H */
/* *********** */
/* End of file */
/* *********** */

