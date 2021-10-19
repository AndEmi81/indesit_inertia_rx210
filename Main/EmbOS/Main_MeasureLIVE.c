/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Main_MeasureLive
\n			This file contains the EmbOS-based service for the live peak value.
\n
\file		Main_MeasureLive.c
\ingroup	Main
\date		Feb 7, 2011
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		00.00.00
\n		29/03/2011
\n		Roberto Fioravanti
\n		changes
\n 		- file created
\n
\n
\n etc.
*/
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "Analog.h"
#include "Main.h"
#include "LIB_Utilities.h"
#include "LIB_GridVoltage.h"
#include "LIB_ZeroCrossing.h"

#if defined(CONFIG_PM_ENERGY_METER_SW)
#include "PM_EnergyMeterSw.h"
#endif /* CONFIG_PM_ENERGY_METER_SW*/

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
#include "Lib_ClassBVar.h"
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */
/* ****************** */
/* Defines and Macros */
/* ****************** */

#if (ADC_RESOLUTION == 10)
#define EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_10BIT(peakvoltage)	((unsigned short)( (((peakvoltage*1.4142+12)*1800/221800)/3.3)*1024))
#endif
#if (ADC_RESOLUTION == 8)
#define EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_8BIT(peakvoltage)	(VLINE_THEORETIC_AD_10BIT(peakvoltage)>>2)
#endif





/** 
@name Live task safety
If not needed, they can remain undefined.
The configuration definition is in TaskMonitor_Cfg.h
@{
*/
#if  !defined(MAIN_LIVE_TASK_TASKMONITOR_ENABLE) && !defined(CONFIG_MAIN_LIVE_TASK_TASKMONITOR_ENABLE) && !defined(CONFIG_LIB_TM_LIVE)
#define START_LIVE_TASK_MONITORING()		{}
#define MAIN_LIVE_TASK__MONITOR_IN()		{}
#define MAIN_LIVE_TASK__MONITOR_OUT()		{}
#endif
/*@}*/


/*  Check for the security entry definitions*/
#if !defined(START_LIVE_TASK_MONITORING)
#error "START_LIVE_TASK_MONITORING() missing definition."
#endif

#if !defined(MAIN_LIVE_TASK__MONITOR_IN)
#error "MAIN_LIVE_TASK__MONITOR_IN() missing definition."
#endif

#if !defined(MAIN_LIVE_TASK__MONITOR_OUT)
#error "MAIN_LIVE_TASK__MONITOR_OUT() missing definition."
#endif

/**
@name Debug macros
\par Properties
Not mandatory. If not defined they assume the default values
\code
#define MAIN_LIVE_TASK_DEBUG                (1)
#define MAIN_LIVE_TASK_DURATION             (1)
#define MAIN_LIVE_TASK_DELAY_ON_ZC          (1)
#define MAIN_LIVE_TASK_TUNING               (1)
\endcode
@{
*/

#if !defined(MAIN_LIVE_TASK_DEBUG)
/** Debug enable
\li 0: all debug features disabled
\li 1: allows to enable the debug level for MAIN_LIVE_TASK_DURATION, MAIN_LIVE_TASK_DELAY_ON_ZC
MAIN_LIVE_TASK_TUNING
*/
#define MAIN_LIVE_TASK_DEBUG                (1)
#endif

#if !defined(MAIN_LIVE_TASK_DURATION)
/** Measurement of the duration of the task
\li 0: disabled
\li 1: enabled
*/
#define MAIN_LIVE_TASK_DURATION             (1)
#endif

#if !defined(MAIN_LIVE_TASK_DELAY_ON_ZC)
/** delay of the activation of the task in [us] */
#define MAIN_LIVE_TASK_DELAY_ON_ZC          (1)
#endif

#if !defined(MAIN_LIVE_TASK_TUNING)
/** correction of the delay of the activation of the task in [us] */
#define MAIN_LIVE_TASK_TUNING               (1)
#endif
/*@}*/


/**
@name Tuning macros
\par Properties
Not mandatory. If not defined it assumes the default value
\code
#define LIVE_OFFSET_ACTIVATION         (0)
\endcode
\attention Used only if \ref MAIN_LIVE_TASK_DEBUG is defined as 0.
@{
*/
#ifndef LIVE_OFFSET_ACTIVATION
/** Live task activation delay correction in [us] */
#define LIVE_OFFSET_ACTIVATION             (0)
#endif  /* LIVE_OFFSET_DEFAULT */
/*@}*/


#define LIVE_TASK__LIVE_AWG_SAMPLES         (1<<LIVE_TASK__LIVE_AWG_SAMPLES_POF2)

#define V_LINE_UNDER_TH_FILTER              5


#if V_LINE_UNDER_TH_FILTER >= 255
#error "V_LINE_UNDER_TH_FILTER >= 255"
#endif /* V_LINE_UNDER_TH_FILTER >= 255*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef struct
{
	Main_LivePeakMeasureADCType LastSample;
	uint8_t LiveUnderThCnt;
} Main_LiveFilterType;


typedef struct
{
#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)
	Main_LiveRMSType   Vrms;
#endif /* defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION) */
	uint16_t  AverageValue;
	int16_t   AverageIndex;
	uint8_t   AverageSamplesCount;
} Main_LiveType;


/*
Parameter for delay on ZC
It is the delay on ZC in system cycles (OS_NANOSECS_TO_SYSTEMTIMERCYCLES())
*/
typedef struct
{
	OS_U32 min;
	OS_U32 max;
}MainLiveDelayOnZCParamType;


/* score (kind of correlation index) to state if delay on ZC measurement is in range and reliable */
typedef struct Score_s
{
	OS_I8	score;
	OS_I8	eScore;
} MainLive_ScoreType;
/* **************************** */
/* Prototype of local functions */
/* **************************** */





/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
/**
@name Service variables
@{
*/

#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
/** Task Stack */
static STACK_SEC OS_STACKPTR int      Main_MeasureLiveTaskStack[CONFIG_MAIN_MEASURELIVE_TASK_STACK_SIZE] STACK_SEC_POST;

/** Task Control Block*/
static OS_TASK              Main_MeasureLiveTaskTCB;


/** high resolution Alarm */
static OS_ALARM             Main_AlmMeasureLive;
#endif  /* defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */


#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)  
/** Sema */
static OS_RSEMA             LiveSema;
#endif


static Main_LiveFilterType  Main_LiveFilter = {0};
static Main_LiveType        Main_Live = {0};
/*@}*/



#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC)
/*  timing for the measurement of the delay to ZC  */
static  TOOLS_TIMING        			LiveDelayOnZC;
/* Number of updates for normal feedbacks :
 * free runner counter incremented from -128 up to 127 each task execution
 * */
static	MainLive_ScoreType				MainLiveDelayOnZC_Score; /*score*/
/* Max and min delay in ZC in ns*/
static const MainLiveDelayOnZCParamType MainLiveDelayOnZCParam =
{
		OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_LIVE_DELAY_ZC_ADV_MIN_VALUE*1000),
		OS_NANOSECS_TO_SYSTEMTIMERCYCLES(CONFIG_MAIN_LIVE_DELAY_ZC_ADV_MAX_VALUE*1000)
};


#endif  /* MAIN_FEEDBACKS_TASK_DELAY_ON_ZC */

/**
@name Debug variables
@{
*/
#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
#if     MAIN_LIVE_TASK_DEBUG

#if     MAIN_LIVE_TASK_DELAY_ON_ZC
/** task delay in [us]
\attention Available only if \ref MAIN_LIVE_TASK_DEBUG and \ref MAIN_LIVE_TASK_DELAY_ON_ZC are
defined as 1.
*/
static  TOOLS_TIMING        MainLiveTaskDelayOnZC;
static  TOOLS_TIMING_STATS  MainLiveTaskDelayOnZCStat;
#endif  /* MAIN_LIVE_TASK_DELAY_ON_ZC */

#if     MAIN_LIVE_TASK_DURATION
/** task duration in [us]
\attention Available only if \ref MAIN_LIVE_TASK_DEBUG and \ref MAIN_LIVE_TASK_DURATION are
defined as 1.
*/
static  TOOLS_TIMING        MainLiveTaskDuration;
/** statistics about task duration [us]
\attention Available only if \ref MAIN_LIVE_TASK_DEBUG and \ref MAIN_LIVE_TASK_DURATION are
defined as 1.
*/
static  TOOLS_TIMING_STATS  MainLiveTaskDurationStat;
#endif  /* MAIN_LIVE_TASK_DURATION */


#if     MAIN_LIVE_TASK_TUNING
/** task delay correction in [us]
\attention Available only if \ref MAIN_LIVE_TASK_DEBUG and \ref MAIN_LIVE_TASK_TUNING are
defined as 1.
*/
static  OS_I32              MainLiveTaskTuning;
#endif /* MAIN_LIVE_TASK_TUNING */

#endif  /* MAIN_LIVE_TASK_DEBUG */
#endif  /* !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */

/*@}*/
#if defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
static Main_SequencerActivityType LiveActivity;
#endif /* defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Function */
#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
static void Main_LiveTask ( void );
static OS_ALARMROUTINE AlmMeasureLiveCallback;


#if     MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_DELAY_ON_ZC
static void Main_LiveTask_DelayOnZC_Init ( void );
static void Main_LiveTask_DelayOnZC_SetRef ( void );
static void Main_LiveTask_DelayOnZC_GetTiming ( void );
#else
#define Main_LiveTask_DelayOnZC_Init()
#define Main_LiveTask_DelayOnZC_SetRef()
#define Main_LiveTask_DelayOnZC_GetTiming()
#endif /* MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_DELAY_ON_ZC */


#if     MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_DURATION
static void Main_LiveTask_Duration_Init ( void );
static void Main_LiveTask_Duration_Start ( void );
static void Main_LiveTask_Duration_End ( void );
#else
#define Main_LiveTask_Duration_Init()
#define Main_LiveTask_Duration_Start()
#define Main_LiveTask_Duration_End()
#endif /* MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_DURATION */


#if     MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_TUNING
static void Main_LiveTask_Tuning_Init ( void );
static OS_I32 Main_LiveTask_Tuner ( OS_U32 usOffset );
#else
#define Main_LiveTask_Tuning_Init()
#define Main_LiveTask_Tuner(dummy)      ((OS_I32)LIVE_OFFSET_ACTIVATION)
#endif /* MAIN_LIVE_TASK_DEBUG   &&   MAIN_LIVE_TASK_TUNING */
#endif /* !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */

#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
static void Main_Live_DelayOnZCProcess( void );
#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
static void Main_Live_DelayOnZC_Init ( void );
#endif
#else
#define Main_Live_DelayOnZCProcess()
#define Main_Live_DelayOnZC_Init()
#endif

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Live initialization. It creates the task Main_LiveTask and alarm Main_AlmMeasureLive.
*/
void Main_LiveInit ( void )
{
#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)  
	OS_CREATERSEMA(&LiveSema);
#endif
    
#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
    OS_CREATETASK ( &Main_MeasureLiveTaskTCB,
					"LiveTask",
					Main_LiveTask,
					( OS_U8 ) MAIN_LIVE_TASK_PRIORITY,
					Main_MeasureLiveTaskStack );
	
	OS_CreateAlarm ( &Main_AlmMeasureLive, AlmMeasureLiveCallback, 0, 0 );
    Main_LiveTask_DelayOnZC_Init();
	Main_LiveTask_Duration_Init();
	Main_LiveTask_Tuning_Init();
#else
    Main_AddSequencerActivity ( &LiveActivity, Main_LiveMeasure );
#endif	

#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
    SAFETY_REGION_START();
   	SAFETY_DATA_UPDATE(LiveDelayOnZC );
   	SAFETY_DATA_UPDATE(MainLiveDelayOnZC_Score);
   	SAFETY_REGION_END();
#endif
}
/**
Returns Live rms value
*/
#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)
Main_LiveRMSType Main_GetLiveValue_Rms(void)
{
    Main_LiveRMSType tmp;
    
    OS_Use(&LiveSema);
    tmp = Main_Live.Vrms;
    OS_Unuse(&LiveSema);
    return tmp;
}
#endif  /* defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION) */
/**
Sets the Alarm on high resolution timer for Live Task

\param [in] usOffset alarm expiration in microseconds
\param [in] TickRef raw time reference in ticks


\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
void Main_ScheduleMeasureLive ( OS_U32 usOffset, OS_U32 TickRef )
{
    OS_ALARM_TICK ref = (OS_ALARM_TICK)TickRef;
	usOffset += Main_LiveTask_Tuner ( usOffset );
	OS_START_ABSALARM_ONESHOT ( &Main_AlmMeasureLive, usOffset, &ref );
	Main_LiveTask_DelayOnZC_SetRef();
}
#endif /* !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */

/**
Sets the Alarm on high resolution timer for Live Task

\param [in] usOffset alarm expiration in microseconds
\param [in] TickRef raw time reference in ticks


\author 	Roberto Fioravanti
\date		29/03/2011
\version	1.0.0
*/
#if defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
void Main_MeasureLive_SetSequencerProfile ( Main_SequencerProfileType profile  )
{
    LiveActivity.profile = profile;
}
#endif
/**
Returns the ADC live peak value
\return ADC_ValueType
\retval ADC sample of live peak.
*/
ADC_ValueType Main_GetLivePeakValueADC ( void )
{
	return Main_LiveFilter.LastSample;
}

/**
Live peak acquisition
*/
void Main_LiveMeasure(void)
{
    Main_LivePeakMeasureADCType LivePeakADC;
#if defined(CONFIG_LIB_ADC_CALIBRATION)
	struct Loader CalibrationDataPtr_ElectSupp;
#endif

    Main_Live_DelayOnZCProcess();
    if ( LIB_ADC_VLineMeasure ( &LivePeakADC ) == TRUE )
    {
        /* LIVE channel not present: suspend task */
#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
        LIB_ZC_SetLineVoltageParams(FALSE, LivePeakADC);
#endif
    }
    else
    {
#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
        LIB_ZC_SetLineVoltageParams(TRUE, LivePeakADC);
#endif

#if defined(CONFIG_LIB_ADC_VLINE_CALIBRATION)
	if (LIB_CAL_GetCalibData(ID_CAL_PNTR_ELECT_SUPP, &CalibrationDataPtr_ElectSupp) == BSLDR_OK)
	{
		if (CalibrationDataPtr_ElectSupp.Data[CONFIG_LIB_CAL_VLINE_MEASR_OFFS] != 0)
		{
			/* Data available for calibration */
			LivePeakADC = (ADC_ValueType)(((uint32_t)LivePeakADC * CalibrationDataPtr_ElectSupp.Data[CONFIG_LIB_CAL_VLINE_THEOR_OFFS])/CalibrationDataPtr_ElectSupp.Data[CONFIG_LIB_CAL_VLINE_MEASR_OFFS]);
		}
	}
#endif /* defined(CONFIG_LIB_ADC_VLINE_CALIBRATION) */

#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)
        OS_Use(&LiveSema);
        Main_Live.Vrms = LIB_GV_getRMS_Vac ( LivePeakADC );
        OS_Unuse(&LiveSema);
#endif /* defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION) */
        Main_LiveFilter.LastSample = LivePeakADC;
        if ( LIB_GV_CheckNominalVoltage ( LivePeakADC ) == TRUE )
        {
            {
                Main_LiveFilter.LiveUnderThCnt = 0;
            }
        }
#if defined(CONFIG_PM_ENERGY_METER_SW)
        PowerMeter_SetInstant_V((uint32_t)LivePeakADC);
#endif /* CONFIG_PM_ENERGY_METER_SW*/

#if defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD)
        LIB_ZC_PeriodoReteMedioManager();
#endif /* defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD) */

        LIVE_TASK__LIVE_PROCESSING()
    }
}


/**
Computes average index of Live peak with LIVE_TASK__LIVE_AWG_SAMPLES samples.
\param [in] ADC_LivePeakValue ADC peak value
*/
void Main_AvgIndexManager ( ADC_ValueType ADC_LivePeakValue )
{
	if ( Main_Live.AverageSamplesCount++ < LIVE_TASK__LIVE_AWG_SAMPLES )
	{
		Main_Live.AverageValue += ADC_LivePeakValue;
	}
	else
	{
#if ADC_RESOLUTION == 10
	
		if ( LIB_GV_GetNominalGridVoltage() )
		{
			Main_Live.AverageIndex = (int16_t) (Main_Live.AverageValue / EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_10BIT (110) - LIVE_TASK__LIVE_AWG_SAMPLES);
		}
		else
		{
			Main_Live.AverageIndex = (int16_t) ((Main_Live.AverageValue / EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_10BIT (230)) - LIVE_TASK__LIVE_AWG_SAMPLES);
		}
		
#endif
#if ADC_RESOLUTION == 8
		
		if ( LIB_GV_GetNominalGridVoltage() )
		{
			Main_Live.AverageIndex = Main_Live.AverageValue / EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_8BIT ( 110 ) - LIVE_TASK__LIVE_AWG_SAMPLES;
		}
		else
		{
			Main_Live.AverageIndex = Main_Live.AverageValue / EXPECTED_LIVE_PEAK_VOLTAGE_TO_ADC_8BIT ( 230 ) - LIVE_TASK__LIVE_AWG_SAMPLES;
		}
		
#endif
		Main_Live.AverageSamplesCount = 0;
		Main_Live.AverageValue = 0;
	}
}

/*
Checks if range is ok.
\return
\retvalue	0 , no update since last time
\retvalue	>0 , in range and reliable (value has been at least once updated since last call
\retvalue	<0 , not in range
*/
#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
OS_I8 Main_Live_DelayOnZC_IsRangeOK(void)
{
	OS_I8 result=MainLiveDelayOnZC_Score.score-MainLiveDelayOnZC_Score.eScore;
	if (result > 0)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(MainLiveDelayOnZC_Score);
		/*updates correlation index value*/
		MainLiveDelayOnZC_Score.eScore=MainLiveDelayOnZC_Score.score;
		SAFETY_DATA_UPDATE(MainLiveDelayOnZC_Score);
		SAFETY_REGION_END();
	}

	return result;
}
#endif

/* Local Functions */

#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)
/**
Not-preemptable cyclic task for Live measurement.
At each cycle the task is suspended.
It can be resumed with delay by calling Main_ActivateMeasureLiveWithOffset() function.

\warning	none

\author 	Roberto Fioravanti
\date		  13/01/2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
static void Main_LiveTask ( void )
{
	Main_LivePeakMeasureADCType LivePeakADC;
	START_LIVE_TASK_MONITORING();
	while ( 1 )
	{
		OS_Suspend ( NULL );
		OS_EnterRegion();
		/* here the scheduler is switched off */
		
		/* debug: begin */
		Main_LiveTask_DelayOnZC_GetTiming();
		Main_LiveTask_Duration_Start();
		/* debug: end */
		MAIN_LIVE_TASK__MONITOR_IN();
		
		if ( LIB_ADC_VLineMeasure ( &LivePeakADC ) == TRUE )
		{
	        /* LIVE channel not present: suspend task */
#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
	        LIB_ZC_SetLineVoltageParams(FALSE, LivePeakADC);
#endif
		}
		else
		{
#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
        LIB_ZC_SetLineVoltageParams(TRUE, LivePeakADC);
#endif

#if defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION)
            OS_Use(&LiveSema);
			Main_Live.Vrms = LIB_GV_getRMS_Vac ( LivePeakADC );
			OS_Unuse(&LiveSema);
#endif /* defined(LIVE_TASK__ADC_LIVE_TO_RMS_CONVERSION) */
			
			if ( LIB_GV_CheckNominalVoltage ( LivePeakADC ) == TRUE )
			{
				{
					Main_LiveFilter.LiveUnderThCnt = 0;
				}
			}
			
			LIVE_TASK__LIVE_PROCESSING()
		}
		
		MAIN_LIVE_TASK__MONITOR_OUT();
		/* debug: begin */
		Main_LiveTask_Duration_End();
		/* debug: end */
		/* here the scheduler is switched on */
		OS_LeaveRegion();
	}
}


static void AlmMeasureLiveCallback ( void )
{
	if ( OS_GetSuspendCnt ( &Main_MeasureLiveTaskTCB ) > 0 )
	{
		OS_Resume ( &Main_MeasureLiveTaskTCB );
	}
}

#if MAIN_LIVE_TASK_DEBUG && MAIN_LIVE_TASK_DELAY_ON_ZC
static void Main_LiveTask_DelayOnZC_Init ( void )
{
	Tools_CreateTimingMeasure ( &MainLiveTaskDelayOnZC );
	Tools_CreateTimingStats ( &MainLiveTaskDelayOnZCStat );
}

static void Main_LiveTask_DelayOnZC_SetRef ( void )
{
	Tools_TimingSetRef ( &MainLiveTaskDelayOnZC );
}

static void Main_LiveTask_DelayOnZC_GetTiming ( void )
{
	Tools_ProcessValue ( &MainLiveTaskDelayOnZCStat, Tools_GetTiming ( &MainLiveTaskDelayOnZC ) );
	if(MainLiveTaskDelayOnZC.Timing_cyc >14000)
	{
	    MainLiveTaskDelayOnZC.Timing_cyc++;
	}
}
#endif


#if MAIN_LIVE_TASK_DEBUG && MAIN_LIVE_TASK_DURATION
static void Main_LiveTask_Duration_Init ( void )
{
	Tools_CreateTimingMeasure ( &MainLiveTaskDuration );
	Tools_CreateTimingStats ( &MainLiveTaskDurationStat );
}

static void Main_LiveTask_Duration_Start ( void )
{
	Tools_TimingSetRef ( &MainLiveTaskDuration );
}

static void Main_LiveTask_Duration_End ( void )
{
	Tools_ProcessValue ( &MainLiveTaskDurationStat, Tools_GetTiming ( &MainLiveTaskDuration ) );
}
#endif /* MAIN_LIVE_TASK_DEBUG && MAIN_LIVE_TASK_DURATION */


#if     MAIN_LIVE_TASK_DEBUG && MAIN_LIVE_TASK_TUNING
static void Main_LiveTask_Tuning_Init ( void )
{
	MainLiveTaskTuning = 0;
}

static OS_I32 Main_LiveTask_Tuner ( OS_U32 usOffset )
{
	if ( MainLiveTaskTuning < 0 )
	{
		if ( -MainLiveTaskTuning > usOffset )
		{
			MainLiveTaskTuning = - ( ( OS_I32 ) usOffset );
		}
	}
	
	return MainLiveTaskTuning;
}
#endif /* MAIN_LIVE_TASK_DEBUG && MAIN_LIVE_TASK_TUNING */
#endif  /* !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY) */


#if defined(CONFIG_MAIN_LIVE_DELAY_ON_ZC) && defined(CONFIG_CLASS_B_DATA_INTEGRITY)
/**
Init Duration  measurement
*/

#if !defined(CONFIG_MAIN_LIVE_MEASURE_AS_ACTIVITY)

static void Main_Live_DelayOnZC_Init ( void )
{
	SAFETY_REGION_START();
	Tools_CreateTimingMeasure ( &LiveDelayOnZC );
	SAFETY_DATA_UPDATE(LiveDelayOnZC );
	SAFETY_REGION_END();
}
#endif

static void Main_Live_DelayOnZCProcess(void)
{
	OS_TIMING timing=0;
	OS_U32 peakTime = 0;


	if(goodZCSignal())
	{
		peakTime = LIB_ZC_GetNominalHalfWave();
		if(peakTime)
		{
			peakTime = OS_Convert_us2Cycles(peakTime);
			peakTime /= 2;
		}

		SAFETY_REGION_START();
		SAFETY_DATA_CHECK( LiveDelayOnZC);
		Tools_TimingSetRefBySysTime ( &LiveDelayOnZC, Get_ZCSysTime());
		Tools_GetTiming ( &LiveDelayOnZC );
		SAFETY_DATA_UPDATE( LiveDelayOnZC);

		/* store the delay in a local variable */
		timing = LiveDelayOnZC.Timing_cyc;

		/* check if the delay is in range */
		if( (timing<peakTime-MainLiveDelayOnZCParam.min) && (timing>peakTime+MainLiveDelayOnZCParam.max))
		{
			/* the value of the delay is in the allowed range */
			SAFETY_DATA_CHECK(MainLiveDelayOnZC_Score);
			MainLiveDelayOnZC_Score.score++;
			SAFETY_DATA_UPDATE(MainLiveDelayOnZC_Score);
		}
		else
		{
			SAFETY_DATA_CHECK(MainLiveDelayOnZC_Score);
			if(MainLiveDelayOnZC_Score.score!=MainLiveDelayOnZC_Score.eScore)
			{
				MainLiveDelayOnZC_Score.score--;
				SAFETY_DATA_UPDATE(MainLiveDelayOnZC_Score);
			}
		}
		SAFETY_REGION_END();
	}
}
#endif

/* *********** */
/* End of file */
/* *********** */
