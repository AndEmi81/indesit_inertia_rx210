/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
 *\page pageLibBackup
 *\ingroup LIB_BackUp
 */
/**
\n
\par		LIB_BackUp.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_BackUp.c
\ingroup	LIB_BackUp
\date		Feb 07, 2011
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n 		01.00.01
\n		Apr 14, 2011
\n		stefano.ughi
\n		changes
\n 		    - added  LIB_Backup_SetBackupControlMonitor(BACKUP_NO_SOURCE) inside void Task_Backup(void)
\n 		01.00.00
\n		Feb 07, 2011
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/
#define MODULE_BODY_LIB_BACKUP
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "callback.h"
#include "LIB_BackUp.h"
#include "LIB_Error.h"
/*lint -e537 Repeated include file*/
#include "LIB_BackupExp.h"
/*lint +e537*/
#include "Analog.h"
#include "OCRST_drv.h"
#include "OCDIO_drv.h"
#include "OCFLS_drv.h"
#include "OCIRQ_drv.h"

#include "OCTPM_drv.h"
#if defined (CONFIG_PL_DRV_OCCRC)
#include "OCCRC_drv.h"
#else
#include "SWCRC_drv.h"
#endif
#include "OutputManager.h"
#include "LIB_ZeroCrossing.h"
#if defined (RTN_CHECK_WITH_SW_ALARM)
#include "PM_SWalarms.h"
#endif
#if defined(CONFIG_PM_MOTORGATE) /* ex __UNIVERSAL_MOTOR_H */
#include "PM_UM_UniversalMotor.h"
#endif
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)
#include "LIB_ClassBVarBck.h"
#endif
#include "LIB_Trace.h"
#if !defined(CONFIG_TRACE_BACKUP)
#define TRACE_BACKUP_START()	{}
#define TRACE_BACKUP_END()		{}

#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */


#if defined(CONFIG_LIB_BACKUP_CHECK_BOTH_TH)
#define RTN_RANGE(x)	(((x) < REF_MIN_ADC_UNITS) || ((x) > REF_MAX_ADC_UNITS))

#else
#define RTN_RANGE(x)	((x) < REF_MIN_ADC_UNITS)
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
/** This variable must be the first data of backup block */
uint8_t Backup_Source;
/* Static */

/** Used to check if backup monitoring is active */
static uint8_t BackupCntrlSource;
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static bool_t LIB_Backup_IsTimeToBackup(void);
static void DeinitAllPeripherals(void);
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
#if defined( CONFIG_DEBUG_BACKUP )
#include "backup_trace.h"
#else
#define DEBUG_BACKUP_INIT()
#define DEBUG_BACKUP_START(x)
#define DEBUG_BACKUP_END(x,y)
#define DEBUG_BACKUP_VERIFY(x)
#define DEBUG_BACKUP_GOBACKUP()
#endif



/**
Performs all actions necessaries to backup.

\author 	stefano.ughi
\date		Feb 14, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void Task_Backup(void)
{
	uint8_t retVal;
	/* Application's service */
	CBF_notifyBackupAlarm();
	/* Switch off loads to save power */
	LIB_IOM_SetAllLoadsOff ( SOURCE_SALO_PLATFORM );
	/* Checks if is time to backup  */
	if ( LIB_Backup_IsTimeToBackup() == TRUE )
	{
		/* Interrupts are harmful in this phase */
		EnterCriticalSection();

		TRACE_BACKUP_START();

		/* To save M.C.U. current */
		DeinitAllPeripherals();
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)
	Sec_Backup_SecDataSave();
#endif
		DEBUG_BACKUP_START(BACKUP_BY_REFERENCE);

		retVal = LIB_Backup_DataSave();

		DEBUG_BACKUP_END(BACKUP_BY_REFERENCE, retVal);

		TRACE_BACKUP_END();

		LIB_ZC_ResetSyncPhase();
		SetgoodZCSignalTo(FALSE);
		/* Disables further backups */
		LIB_Backup_SetBackupControlMonitor(BACKUP_NO_SOURCE);
		/* bye, bye */
		OCRST_GenerateUserResetSync();
	}
}
/**
Checks conditions for backup.

\param  [in]		systemMode system mode

\return 	bool_t

\retval		TRUE it's time to call backup task
\retval		FALSE there is at least one condition not satisfied to call backup task


\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
volatile int dbg_gobackup=0;
bool_t LIB_Backup_CheckIfIsTimeToBackup(uint8_t systemMode)
{
	bool_t isTime;

	isTime = FALSE;
	if ( SYSTEM_NORMAL_MODE == systemMode )
	{
		DEBUG_BACKUP_GOBACKUP();

		if ( LIB_ADC_CheckRef() == TRUE )
		{
			if ( BACKUP_ON_RTN == LIB_Backup_GetBackupControlMonitor() )
			{
				if ( FALSE == LIB_ADC_Is_Ref_Good() )
				{
					isTime = TRUE;
				}
			}
		}
	}
	return (isTime);
}

/**
Forces backup.

\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_Backup_ForceBackup(void)
{
	uint8_t retVal;
	EnterCriticalSection();
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)
	Sec_Backup_SecDataSave();
#endif
	DEBUG_BACKUP_START(BACKUP_BY_FORCED);

	retVal = LIB_Backup_DataSave();

	DEBUG_BACKUP_END(BACKUP_BY_FORCED, retVal);
	ExitCriticalSection();
}



/**
\n Checks if backup checksum is O.K.
\n if checksum is O.K. performs backup data restore and returns TRUE otherwise FALSE.


\return 	bool_t

\retval		TRUE backup data restored
\retval		FALSE backup data not restored because checksum K.O.

\author 	stefano.ughi
\date		Feb 14, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t LIB_Backup_RestoreManager(void)
{
	if ( LIB_Backup_ChecksumVerify() == TRUE )
	{
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)
		Sec_Backup_SecDataRestore();
#endif
		LIB_Backup_DataRestore();

		DEBUG_BACKUP_VERIFY(TRUE);
		return TRUE;
	}
	else
	{
		DEBUG_BACKUP_VERIFY(FALSE);
		return FALSE;
	}
}



#if defined (RTN_CHECK_WITH_COUNTER)
uint8_t LIB_Backup_RefCheck(void)
{
	static uint8_t NumOfReadings;
	static struct
	{
		bool_t NumOfReadingsOutOfRangeSetup:1;
		bool_t NumOfReadingsInRangeSetup:1;
		bool_t Free:6;
	} Flags= {0,0,0};
#if defined (DEBUG_BACKUP_MANAGER)
	static ADC_ValueType rtnADvalue;
#else
	ADC_ValueType rtnADvalue;
#endif
	uint8_t posAnalogInputParam;


	posAnalogInputParam = LIB_ADC_AnalogChRamPos(LIB_ADC_GetChCompPos());

	if ( AD_POS_NOT_FOUND == posAnalogInputParam )
	{
		/* Serious error Ch_Comp not defined */
		PLT_ERROR_WARNING(PLT_ERROR_BACKUP_1);

		return BACKUP_RTN_CH_NOT_FOUND;
	}
	/* Performs read on RTN */
	rtnADvalue = LIB_ADC_Conv_F (posAnalogInputParam, AD_LOCAL_SAMPLES_POWER_OF_TWO, AD_LOCAL_SAMPLES_DISCARDED);

	if (RTN_RANGE(rtnADvalue))
	{
		/* Measure lower than minimum  */
		Flags.NumOfReadingsInRangeSetup = 0;
		if ( 0 == Flags.NumOfReadingsOutOfRangeSetup )
		{
			NumOfReadings = NUM_READ_RTN_OUT_OF_RANGE;
			Flags.NumOfReadingsOutOfRangeSetup = 1;
		}
		NumOfReadings --;
		if ( 0 == NumOfReadings )
		{
			/* For the next time but perhaps not necessary if after backup a cold reset is performed */
			Flags.NumOfReadingsOutOfRangeSetup = 0;
			/* It's time to perform backup ! */
			return BACKUP_NOW;
		}
		else
		{
			return BACKUP_WAIT;
		}
	}
	else
	{
		/* Measure in range */
		Flags.NumOfReadingsOutOfRangeSetup = 0;
		if ( 0 == Flags.NumOfReadingsInRangeSetup )
		{
			NumOfReadings = NUM_READ_RTN_IN_RANGE;
			Flags.NumOfReadingsInRangeSetup = 1;
		}
		NumOfReadings --;
		if ( 0 == NumOfReadings )
		{
			/* For the next time */
			Flags.NumOfReadingsInRangeSetup = 0;
			/* False Alarm Exit From Task Backup  */
			return BACKUP_EXIT;
		}
		else
		{
			return BACKUP_WAIT;
		}
	}
}
/**
\n This function monitors the RTN in order to establish if a backup must be performed.
\n If RTN value is out of range for RTN_OUT_OF_RANGE_TOUT_NSEC
\n we assume that a backup must be performed.
\n If RTN value is in range for RTN_IN_RANGE_TOUT_NSEC
\n we assume that it was a false backup alarm.

\attention \b   Ch_Comp is A/D channel where we read RTN value, used as Input.

\return 	uint8_t

\retval		BACKUP_NOW  perform backup
\retval		BACKUP_WAIT  decision pending
\retval     BACKUP_EXIT no backup
\retval     BACKUP_RTN_CH_NOT_FOUND rtn A/D channel not found, check setting file.


\author 	stefano.ughi
\date		Feb 8, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
#elif defined(RTN_CHECK_WITH_SW_ALARM)
uint8_t LIB_Backup_RefCheck(void)
{
	static struct
	{
		bool_t AlarmOutOfRangeSetup:1;
		bool_t AlarmInRangeSetup:1;
		bool_t Free:6;
	} Flags= {0,0,0};
	IO_HwTimerType alarmInTicks;
#if defined (DEBUG_BACKUP_MANAGER)
	static ADC_ValueType rtnADvalue;
#else
	ADC_ValueType rtnADvalue;
#endif
	uint8_t posAnalogInputParam;


	posAnalogInputParam = LIB_ADC_ADChVsAnalogInputParamIndex(LIB_ADC_GetChCompPos());

	if ( AD_POS_NOT_FOUND == posAnalogInputParam )
	{
		/* Serious error Ch_Comp not defined */
		PLT_ERROR_WARNING(PLT_ERROR_BACKUP_2);

		return BACKUP_RTN_CH_NOT_FOUND;
	}
	/* Performs read on RTN */
	rtnADvalue = LIB_ADC_Conv_F (posAnalogInputParam, AD_LOCAL_SAMPLES_POWER_OF_TWO, AD_LOCAL_SAMPLES_DISCARDED);

	if ( RTN_RANGE(rtnADvalue) )
	{
		/* Measure lower than minimum  */
		Flags.AlarmInRangeSetup = 0;
		if ( 0 == Flags.AlarmOutOfRangeSetup )
		{
			alarmInTicks = PM_SWalarmsConvNsecInTicks(RTN_OUT_OF_RANGE_TOUT_NSEC);
			PM_SWalarmsSet(ID_SW_ALARM, alarmInTicks);

			Flags.AlarmOutOfRangeSetup = 1;
		}

		PM_SWalarmsCheck(ID_SW_ALARM);
		if ( PM_SWalarms_IsAlarmExpired(ID_SW_ALARM) == TRUE )
		{
			/* For the next time but perhaps not necessary if after backup a cold reset is performed */
			Flags.AlarmOutOfRangeSetup = 0;

			/* It's time to perform backup ! */
			return BACKUP_NOW;
		}
		else
		{
			return BACKUP_WAIT;
		}
	}
	else
	{
		/* Measure in range */
		Flags.AlarmOutOfRangeSetup = 0;
		if ( 0 == Flags.AlarmInRangeSetup )
		{
			alarmInTicks = PM_SWalarmsConvNsecInTicks(RTN_IN_RANGE_TOUT_NSEC);
			PM_SWalarmsSet(ID_SW_ALARM, alarmInTicks);
			Flags.AlarmInRangeSetup = 1;
		}

		PM_SWalarmsCheck(ID_SW_ALARM);
		if ( PM_SWalarms_IsAlarmExpired(ID_SW_ALARM) == TRUE )
		{
			/* For the next time */
			Flags.AlarmInRangeSetup = 0;
			/* False Alarm Exit From Task Backup  */
			return BACKUP_EXIT;
		}
		else
		{
			return BACKUP_WAIT;
		}
	}
}
#else
#error "Backup RTN Check Mode not defined"
#endif
/**
Enables/Disables Backup Control on...

\param  [in]    backupSource event to monitor in order to enter in backup

\author 	stefano.ughi
\date		Feb 9, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_Backup_SetBackupControlMonitor(uint8_t backupSource)
{
	BackupCntrlSource = backupSource;
}
/**
Returns Backup Control Monitor source.

\return 	uint8_t

\retval		BACKUP_NO_SOURCE    Backup'monitoring off so no backup is performed.
\retval     BACKUP_ON_RTN       Backup'monitoring on RTN active.

\author 	stefano.ughi
\date		Feb 9, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint8_t LIB_Backup_GetBackupControlMonitor(void)
{
	return BackupCntrlSource;
}
/**
Returns the value of Backup_Source that must be the first variable of backup data.

\return 	uint8_t

\retval		PARTIAL_BACKUP_KEYWORD partial backup data restore was performed
\retval		not PARTIAL_BACKUP_KEYWORD full backup data restore was performed

\author 	stefano.ughi
\date		Feb 01, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			This routine must be called after LIB_Backup_DataRestore()

*/
uint8_t LIB_Backup_GetBackupSource(void)
{
	return Backup_Source;
}
/**
Sets Backup_Source variable with the keyword for partial backup.

\author 	stefano.ughi
\date		Feb 01, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			This routine must be invoked before performing backup if you want\n
			partial restore.

*/
void LIB_Backup_SetBackupSource(void)
{
	Backup_Source = PARTIAL_BACKUP_KEYWORD;
}

/**
Resets Backup_Source variable.

\author 	stefano.ughi
\date		Feb 01, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			This routine must be invoked before performing backup if you want\n
			full restore.

*/
void LIB_Backup_ResetBackupSource(void)
{
	Backup_Source = 0;
}

/* Local Function */
/**
Checks if it's time to perform backup.

\return 	bool_t

\retval		TRUE it's time to perform backup
\retval		FALSE it was a false alarm.

\author 	stefano.ughi
\date		Feb 14, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/
static bool_t LIB_Backup_IsTimeToBackup(void)
{
	uint8_t refCheckResult;
	bool_t endLessLoop;
	/*lint -esym(644,backup) backup is always initialized */
	bool_t backup;

	endLessLoop = TRUE;
	while ( TRUE == endLessLoop )
	{
		DEBUG_BACKUP_GOBACKUP();

		refCheckResult = LIB_Backup_RefCheck();
		if ( BACKUP_NOW == refCheckResult )
		{
			backup = TRUE;
			endLessLoop = FALSE;
		}
		else if ( BACKUP_EXIT == refCheckResult )
		{
			backup = FALSE;
			endLessLoop = FALSE;
		}
		else
		{
			/* Wait for response */
		}
	}
	return backup;
}
/**
Peripherals's deinitialization.

\author 	stefano.ughi
\date		Feb 15, 2011
\version	1.0.0


\todo		OCDMA_DeinitSync()
\todo       OCI2C_DeInitSync()
\todo       OCUART_DeInitSync()

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
static void DeinitAllPeripherals(void)
{
	IO_ChannelType tmrId;

	(void)OCDIO_DeInitSync();
	(void)OCIRQ_DeInitSync();
	(void)OCADC_DeInitSync();
#if defined(CONFIG_PL_DRV_OCCRC)
	(void)OCCRC_DeInitSync();
#endif
	for ( tmrId = 0; tmrId < NUM_HW_CNTR; tmrId++ )
	{
		(void)OCHWCNTR_DeInitSync(tmrId);
	}
}


/* *********** */
/* End of file */
/* *********** */

