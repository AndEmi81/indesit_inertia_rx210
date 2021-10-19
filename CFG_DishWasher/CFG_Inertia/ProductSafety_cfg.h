/*
 * ProductSafety_Cfg.h
 *
 *  Created on: Jan 22, 2013
 *      Author: alessandro.sabbatini
 */

#ifndef _PRODUCTSAFETY_CFG_H_
#define _PRODUCTSAFETY_CFG_H_

/* Macro defines with number of array items - must be aligned with arrays! */
#if defined(CONFIG_APPLIANCE_WD)
#define SFT_NUM_WD_ACTIONS			2
#else
#define SFT_NUM_WD_ACTIONS			0
#endif
#if	defined(CONFIG_PM_MOTORGATE)
#define SFT_NUM_MOTORGATE_ACTIONS	2
#else
#define SFT_NUM_MOTORGATE_ACTIONS	0
#endif
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#define SFT_NUM_EXTBOARD_ACTIONS		1
#else
#define SFT_NUM_EXTBOARD_ACTIONS		0
#endif
#define SFT_NUM_STATUS_ACTIONS		(13 + SFT_NUM_WD_ACTIONS + SFT_NUM_EXTBOARD_ACTIONS + SFT_NUM_MOTORGATE_ACTIONS)

#define SFT_NUM_CUSTOM_FAULTCODES	1
#define SFT_CUSTOM_FAULT_PRIO		1

/* #define SFT_DEFAULT_PRIORITY	0 */

#define SFT_FLT_SUPERVISOR_BAD_SETTING	1

#define SFT_PREFAULT_TIMEOUT_100MS		(50) 	/* default 5 secondi */
#define SFT_FAULT_TIMEOUT_100MS			(10*30) /* default 30 secondi*/

/* Time reduction in Test/AutoTest mode - time unit is 100ms */
#define SFT_TEST_PREFAULT_TIMEOUT_100MS		100
#define SFT_TEST_FAULT_TIMEOUT_100MS		0

/* Safety modules array definition */
enum SafetyModuleID {
	SFT_MOD_DOORLOCK,
	SFT_MOD_NTCPROBE_WASH,
	SFT_MOD_MOTOR,
	SFT_MOD_DRAINPUMP,
	SFT_MOD_WASH_RESISTOR,
	SFT_MOD_PRESS_SWITCH,
#if defined(CONFIG_APPLIANCE_WD)
	SFT_MOD_DRYFAN,
	SFT_MOD_NTCPROBE_DRY,
	SFT_MOD_DRY_HEATER,
#endif

	SFT_NUM_SAFETY_MODULES

};

/** Fault Sources array definition */
enum SafetyFaultSrcID {
	/* Standard and Supervisor */
	SFT_FLT_NO_FAULT,
	SFT_FLT_SV_NO_SETTING,
	/* Door Lock */
	SFT_FLT_DRLCK_CANNOT_CLOSE,
	SFT_FLT_DRLCK_CANNOT_OPEN,
	SFT_FLT_DRLCK_SETTING,
	/* NTC Wash Probe */
	SFT_FLT_NTCWASH_OPEN,
	SFT_FLT_NTCWASH_SHORTED,
	SFT_FLT_NTCWASH_BLOCKED,
	SFT_FLT_NTCWASH_HIGHTEMP,
	SFT_FLT_NTCWASH_SETTING,
#if defined(CONFIG_APPLIANCE_WD)
	/* NTC Drying Probe */
	SFT_FLT_NTCDRY_OPEN,
	SFT_FLT_NTCDRY_SHORTED,
	SFT_FLT_NTCDRY_BLOCKED,
	SFT_FLT_NTCDRY_HIGHTEMP,
	SFT_FLT_NTCDRY_SETTING,
	/* Drying Heater */
	SFT_FLT_DR_GENERR,
	SFT_FLT_DR_OFFSC,
	SFT_FLT_DR_ONOC,
	SFT_FLT_DR_DISP,
	/* Drying Fan */
	SFT_FLT_DF_SETTING,
	SFT_FLT_DF_FB_KO,
	SFT_FLT_DF_TRIAC_SC,
	SFT_FLT_DF_INTERR,
	SFT_FLT_DF_TRIAC_DM,
#endif
	/* Motor */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
	SFT_FLT_EXT_MOTOR_KO,
#endif
#if defined(CONFIG_PM_MOTORGATE)
	SFT_FLT_WM_ACMOTOR_KO,
	SFT_FLT_WM_ACMOTOR_REL_KO,
#endif /* defined(CONFIG_PM_MOTORGATE) */
	/* Drain Pump */
	SFT_FLT_DRAIN_TRIAC_SC,
	SFT_FLT_DRAIN_TRIAC_DM_ON,
	SFT_FLT_DRAIN_TRIAC_DM_OFF,
	SFT_FLT_DRAIN_TRIAC_DM_TRN,
	SFT_FLT_DRAIN_FB_KO_ON,
	SFT_FLT_DRAIN_FB_KO_OFF,
	SFT_FLT_DRAIN_FB_KO,
	SFT_FLT_DRAIN_TRIAC_OC,
	SFT_FLT_DRAIN_LOCKED,
	SFT_FLT_DRAIN_SETTING,
	/* Washing Resistor */
	SFT_FLT_WR_GENERR,
	SFT_FLT_WR_OFFCC,
	SFT_FLT_WR_ONCA,
	SFT_FLT_WR_SETTING,
	/* Press Switch */
	SFT_FLT_PRSWH_OUTOFRANGE,
	SFT_FLT_PRSWH_BAD_EMPTYLEV,
	SFT_FLT_PRSWH_LITTLE_WATER,
	SFT_FLT_PRSWH_FILL_BLOCKED,
	SFT_FLT_PRSWH_DRAIN_BLOCKED,
	SFT_FLT_PRSWH_NOT_CALIBRATED,
	SFT_FLT_PRSWH_DRAIN_BLKD_AT_ZEROREF,
	SFT_FLT_PRSWH_SETTING,
	/* This must be last item! */
	SFT_NUM_FAULT_SOURCES
};

/** Safety actions array definition */
enum SafetyActionID {
	SFT_ACTION_ELECTROVALVES_OFF,				/* 0 */
	SFT_ACTION_RES_OFF,							/* 1 */
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
	SFT_ACTION_RESTORE_EXT_COMM,				/* 2 restores communication with external board forcing uart peripheral */
	SFT_ACTION_SUSPEND_EXT_COMM_TMP,			/* 3 disables communication for a moment */
#endif
	SFT_ACTION_DRY_FAN_ON,						/* 4 */
#if defined(CONFIG_APPLIANCE_WD)
	SFT_ACTION_SWRELOFF,						/* 5 */
#endif
	SFT_ACTION_LOADSOFF,						/* 6 */
	SFT_ACTION_FORCE_DRAIN_OFF,					/* 7 */
#if defined(CONFIG_PM_MOTORGATE)
	SFT_ACTION_AC_MOTOR_TURNOFF,				/* 8*/
#endif /* defined (CONFIG_PM_MOTORGATE) */
	SFT_ACTION_DRAIN,							/* 9 */
	SFT_ACTION_UNLOCK_DOOR,						/* 10 */
	SFT_ACTION_LOADSOFF_TOTAL,					/* 11 */
	SFT_NUM_SAFETY_ACTIONS
};

/* Default safety action */
#define 	SFT_ACTION_DEFAULT_MASK		(BFMASK(SFT_ACTION_DRY_FAN_ON, 1) | BFMASK(SFT_ACTION_LOADSOFF, 1) | BFMASK(SFT_ACTION_DRAIN, 1) | BFMASK(SFT_ACTION_UNLOCK_DOOR, 1))
#define 	SFT_ACTION_RESET_CHK_MASK	(SFT_ACTION_DEFAULT_MASK | BFMASK(SFT_ACTION_RES_OFF, 1))


#endif /* _PRODUCTSAFETY_CFG_H_ */