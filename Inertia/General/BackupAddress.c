/**
\defgroup	BackupAddress Modulo variabili da backuppare
\ingroup	ApplGeneral

Il modulo contiene gli indirizzi e le dimensioni delle variabili da salvare in backup.
*/

/**
Source file del modulo variabili da backuppare

\file		BackupAddress.c
\ingroup	BackupAddress
\date		17/07/2012
\version	01.00.00
\author		Marco Quintini

\par		PCLint
			Report dd/mm/yyyy: x Errors, y Warnings, z Info

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* ******** */
/* Includes */
/* ******** */
#include "BackupAddress.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */

/* **************** */
/* Global variables */
/* **************** */

/** Indirizzi delle variabili da salvare in backup */
void *const AppBackupAddrTab[BACKUP_NUM] = {
		&Interpreter_Data[0],
		&Paused_Intpt_Data[0],
		&JmpPhase[0],
		&Paused_JmpPhase[0],
		&JmpPhase_Return,
		&ReturnPoint,
		&Jsr,
		&uP_Maintain_Data,
		&uP_PumpCtrl_Data,
		&uP_LoadsControl_Data,
		&loadsPulseData1,
		&loadsPulseData2,
		&uP_Drain_Data,
		&uP_Load_Data,
		&uP_LoadRestore_Data,
		&uP_Regenerate_Data,
		&uP_7liter_Data,
		&uP_7liter_dirtywater_timer,
		&uP_7liter_dirtywater_timer_cfg,
		&uP_Heat_Data,
		&uP_SparklingAid_Data,
		&uP_SetProfile_Data,
		&AADisplParams[0][0],
		&ActiveAlgorithmBmp[0],
		&ActiveAlgorithmBmpOld[0],
		&uP_AltCtrl_Data,
		&uP_SparklingAid_Data,
		&uPSparklingAid_setting_data,
		&dirty_foam_setting_data,
		&BufOldReg[0],
		&DelayInMinutes,
		&Flags_1,
		&Flags_2,
		&Flags_3,
		&GICriteria,
		&(TurbiditySensorCal.val),
		&(TurbiditySensorCal.adH2oRef),
		&SftPulseWarn,
		&Reg_WaterLoaded,
		&FltWarning,
		&FltHeatToutData,
		&Flt_UnderVoltage_ResOff_timer,
		&Flt_overheating_ResOff_timer,
		&UIBackupData[0],
		&StartedCycle,
		&Pump_Load_Data,
		&WaterInDW,
		&cycle_loaded_water,
#if (VIRTUAL_SENSOR_ENABLE == 1)
		&ReadVS_Data,
		&dirty_foam_Data,
		&Aal_DF_end_always3_config,
		&Aal_DF_end_always3_timer,
		&State5,
#endif
#if (SBY_STANDBY_ENABLE == 1)		/* SbyLowPow 22.01.10 */
		&HarakiriLowPower,
#endif
		PC_CTRL_NOT_RESTORED_END,	/* In BackupDimTab[] gli corrisponde 0 per cui non è utilizzato per il salvataggio e ripristino */
#ifdef _STATISTICS_ON_
		&MachineStatistics.globalStats,
		&MachineStatistics.lastCycles[0],
		&MachineStatistics.lastFault[0],
		&MachineStatistics.lastPreFault[0],
		&MachineStatistics.testCycleStats,
		&MachineStatistics.blackBox_Data,
		&TempCycle_Stats,
#else		/* !_STATISTICS_ON_ */
		&MachineStatistics.lastFault[0],
		&MachineStatistics.lastPreFault[0],
#endif  /* _STATISTICS_ON_ */
		&TTEEstimation,
		&tte_enphnospecialselapsedtime_Counter,
		BCK_ADDR_END_TAB
};

/** Dimensione delle variabili da salvare in backup */
/* 
N.B.A.: verificare che la sizeof() di ogni singolo campo sia effettivamente
<= 255 bytes, altrimenti non può essere rappresentata in BackupDimTab[],essendo
questo un buffer di uint8_t.
*/
const uint8_t AppBackupDimTab[BACKUP_NUM-1] = {
		sizeof(Interpreter_Data),
		sizeof(Paused_Intpt_Data),
		sizeof(JmpPhase),
		sizeof(Paused_JmpPhase),	
		sizeof(JmpPhase_Return),
		sizeof(ReturnPoint),
		sizeof(Jsr),
		sizeof(uP_Maintain_Data),
		sizeof(uP_PumpCtrl_Data),
		sizeof(uP_LoadsControl_Data),
		sizeof(loadsPulseData1),
		sizeof(loadsPulseData2),
		sizeof(uP_Drain_Data),
		sizeof(uP_Load_Data),
		sizeof(uP_LoadRestore_Data),
		sizeof(uP_Regenerate_Data),
		sizeof(uP_7liter_Data),
		sizeof(uP_7liter_dirtywater_timer),
		sizeof(uP_7liter_dirtywater_timer_cfg),
		sizeof(uP_Heat_Data),
		sizeof(uP_SparklingAid_Data),
		sizeof(uP_SetProfile_Data),
		sizeof(AADisplParams),
		sizeof(ActiveAlgorithmBmp),
		sizeof(ActiveAlgorithmBmpOld),
		sizeof(uP_AltCtrl_Data),
		sizeof(uP_SparklingAid_Data),
		sizeof(uPSparklingAid_setting_data),
		sizeof(dirty_foam_setting_data),
		sizeof(BufOldReg),
		sizeof(DelayInMinutes),
		sizeof(Flags_1),
		sizeof(Flags_2),
		sizeof(Flags_3),
		sizeof(GICriteria),
		sizeof(TurbiditySensorCal.val),
		sizeof(TurbiditySensorCal.adH2oRef),
		sizeof(SftPulseWarn),
		sizeof(Reg_WaterLoaded),
		sizeof(FltWarning),
		sizeof(FltHeatToutData),
		sizeof(Flt_UnderVoltage_ResOff_timer),
		sizeof(Flt_overheating_ResOff_timer),
		sizeof(UIBackupData),
		sizeof(StartedCycle),
		sizeof(Pump_Load_Data),
		sizeof(WaterInDW),
		sizeof(cycle_loaded_water),
#if (VIRTUAL_SENSOR_ENABLE == 1)
		sizeof(ReadVS_Data),
		sizeof(dirty_foam_Data),
		sizeof(Aal_DF_end_always3_config),
		sizeof(Aal_DF_end_always3_timer),
		sizeof(State5),
#endif
#if (SBY_STANDBY_ENABLE == 1)		/* SbyLowPow 22.01.10 */
		sizeof(HarakiriLowPower),
#endif
		SIZEOF_PC_CTRL_MARKER,	/* Corrisponde a PC_CTRL_NOT_RESTORED_END in BackupAddrTab[] (non va salvato nè ripristinato niente) */
#ifdef _STATISTICS_ON_
		sizeof(MachineStatistics.globalStats),
		sizeof(MachineStatistics.lastCycles),
		sizeof(MachineStatistics.lastFault),
		sizeof(MachineStatistics.lastPreFault),
		sizeof(MachineStatistics.testCycleStats),
		sizeof(MachineStatistics.blackBox_Data),
		sizeof(TempCycle_Stats),
#else		/* !_STATISTICS_ON_ */
		&MachineStatistics.lastFault[0],
		&MachineStatistics.lastPreFault[0],
#endif  /* _STATISTICS_ON_ */
		sizeof(TTEEstimation),
		sizeof(tte_enphnospecialselapsedtime_Counter)
};

/* ***************** */
/* Exported function */
/* ***************** */

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
