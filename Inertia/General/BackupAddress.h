/**
Header file del modulo variabili da backuppare

\file		BackupAddress.h
\ingroup	BackupAddress
\date		17/07/2012
\version	01.00.00
\author		Marco Quintini

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __BACKUP_ADDRESS_H
#define __BACKUP_ADDRESS_H


/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "LIB_BackUpExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*@}*/ 

/**
\name Standard backup
*/
/*@{*/ 
/*
 01 Interpreter_Data
 02	Paused_Intpt_Data
 03 uP_Maintain_Data,
 04 uP_PumpCtrl_Data,
 05 uP_LoadsControl_Data,
 06 loadsPulseData1,
 07 loadsPulseData2,
 08 uP_Drain_Data,
 09 uP_7liter_Data,
 10 uP_7liter_dirtywater_timer,
 11 uP_7liter_dirtywater_timer_cfg,
 12 uP_Heat_Data,
 13	uP_SparklingAid_Data,
 14	uP_SetProfile_Data,
 15	uP_AltCtrl_Data,
 16	uP_SparklingAid_Data,
 17	uPSparklingAid_setting_data,
 18	ReadVS_Data,
 29	dirty_foam_setting_data,
 20	Flags_1,
 21 Flags_2,
 22	Flags_3,
 23	GICriteria,
 24 uP_Load_Data,
 25 uP_LoadRestore_Data,
 26 uP_Regenerate_Data,
 27 BufOldReg[0],
 28 DelayInMinutes,
 29	TurbiditySensorCal.val,
 30	TurbiditySensorCal.adH2oRef,
 31 Reg_WaterLoaded,
 32 UIBackupData,
 33 StartedCycle,
 34 WaterInDW,
 35 loadedWater
 36 TTEEstimation
*/
/** Numero di variabili */
#define BACKUP_STD_NUM		(34 + 2 + 1) /* Il +2  è per PC_CTRL_NOT_RESTORED_END e BCK_ADDR_END_TAB + loadedwater*/
/*@}*/ 


/**
\name JumpIf backup
*/
/*@{*/ 
#if (ITP_JIF_PHASE_ENABLE == 1)

/*
 01	JmpPhase
 02	Paused_JmpPhase
*/
#if ( ITP_JIF_RET_ENABLE == 1 )
/*
 01 JmpPhase_Return
 02 ReturnPoint
 03 Jsr
*/
/** Numero di variabili */
#define BACKUP_JUMPIF_NUM		5
#else
#define BACKUP_JUMPIF_NUM		2
#endif

#else /*#if (ITP_JIF_PHASE_ENABLE == 1)*/
#if ( ITP_JIF_RET_ENABLE == 1 )
/*
 02 ReturnPoint
 03 Jsr
*/
#define BACKUP_JUMPIF_NUM		2
#else
#define BACKUP_JUMPIF_NUM		0
#endif

#endif	/* (ITP_JIF_PHASE_ENABLE == 1) */
/*@}*/ 


#if (VIRTUAL_SENSOR_ENABLE == 1)
/* 01 Aal_DF_end_always3_timer,
 * 02 &Aal_DF_end_always3_config,
 * 03 state 5
 * 04 dirty_foam_Data,
 */
#define BACKUP_VS_DATA			4
#else
#define BACKUP_VS_DATA			0
#endif	/* (ITP_JIF_PHASE_ENABLE == 1) */
/*@}*/
/*
 01 sizeofSftPulseWarn
 02 FltWarning,
 03 FltHeatToutData,
 04 Flt_UnderVoltage_ResOff_timer,
 05 Flt_overheating_ResOff_timer,
 06	Pump_Load_Data
*/
/** Numero di variabili */
#define BACKUP_SAFETY_NUM 	6


/**
\name Statistics backup
*/
/*@{*/ 
#ifdef _STATISTICS_ON_

/*
 01	MachineStatistics.globalStats
 02	MachineStatistics.lastCycle
 03	MachineStatistics.lastFault
 04	MachineStatistics.lastPreFault
 05	MachineStatistics.testCycleStats
 06	MachineStatistics.blackBox_Data
 07	TempCycle_Stats
*/

/** Numero di variabili */
#define BACKUP_STAT_NUM			7

#else	/* !_STATISTICS_ON_ */

/*
 01	MachineStatistics.lastFault
 02 MachineStatistics.lastPreFault[0],
*/

#define BACKUP_STAT_NUM			2

#endif/* _STATISTICS_ON_ */
/*@}*/ 


/**
\name APP backup
*/
/*@{*/ 
#if defined( CONFIG_PM_POWER_PROFILE_SUPPORT ) && ( PP_ENERGYPAUSE_STATUS == TRUE ) && ( APP_PAUSE_STATE_BACKUP == TRUE )

/*
 01	EnergyPause_Status
*/

/** Numero di variabili */
#define BACKUP_A_POWER_PROFILE_NUM 1


#else /* defined( CONFIG_PM_POWER_PROFILE_SUPPORT ) && ( PP_ENERGYPAUSE_STATUS == TRUE ) && ( APP_PAUSE_STATE_BACKUP == TRUE ) */

#define BACKUP_A_POWER_PROFILE_NUM 0

#endif /* defined( CONFIG_PM_POWER_PROFILE_SUPPORT ) && ( PP_ENERGYPAUSE_STATUS == TRUE ) && ( APP_PAUSE_STATE_BACKUP == TRUE ) */
/*@}*/ 


/**
\name Cycle Download Data backup
*/
/*@{*/ 

/*
 01	StartedMSettingIndex
*/

/** Numero di variabili */
#define BACKUP_CYCLE_DOWNLOAD_NUM              1


/**
\name Standby backup
*/
/*@{*/ 
#if (SBY_STANDBY_ENABLE == 1)

/*
 01	HarakiriLowPower
*/

/** Numero di variabili */
#define BACKUP_STANDBY_NUM	1

#else /* !(SBY_STANDBY_ENABLE == 1) */

#define BACKUP_STANDBY_NUM		0

#endif /* (SBY_STANDBY_ENABLE == 1) */
/*@}*/ 

/**
\name Activate Algorithm backup
*/
/*@{*/
#if (AAL_ACTIVATEALGORHYTHM_ENABLE == 1)

/*
 01	ActiveAlgorithmBmp
 02	ActiveAlgorithmBmpOld
 03	AADisplParams
*/

/** Numero di variabili */
#define BACKUP_ACTIVATE_ALGORHYTHM_NUM		3

#else /* !(AAL_ACTIVATEALGORHYTHM_ENABLE == 1) */

#define BACKUP_ACTIVATE_ALGORHYTHM_NUM		0

#endif /* (AAL_ACTIVATEALGORHYTHM_ENABLE == 1) */

#define BACKUP_APP 2	/*TTE + TTE Counter*/
/*@}*/


/**
\name Overall backup
*/
/*@{*/ 

/** Numero totale di variabili */
#define BACKUP_NUM		(	BACKUP_STD_NUM + \
							BACKUP_JUMPIF_NUM + \
							BACKUP_STAT_NUM + \
							BACKUP_VS_DATA + \
							BACKUP_STANDBY_NUM + \
							BACKUP_CYCLE_DOWNLOAD_NUM + \
							BACKUP_SAFETY_NUM +\
							BACKUP_ACTIVATE_ALGORHYTHM_NUM+\
							BACKUP_APP)
/*@}*/ 

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/** Inidirizzi delle variabili da salvare in backup */
/*extern void *const AppBackupAddrTab[BACKUP_NUM];*/

/** Dimensione delle variabili da salvare in backup */
/*extern const uint8_t AppBackupDimTab[BACKUP_NUM-1];*/

#endif /* __BACKUP_ADDRESS_H */

/* *********** */
/* End of file */
/* *********** */
