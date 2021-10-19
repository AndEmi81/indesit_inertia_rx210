/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\n
\par		LIB_ZeroCrossing.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_ZeroCrossing.c
\ingroup	LIB_ZeroCrossing
\date		Dec 20, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Dec 20, 2010
\n		stefano.ughi
\n		changes
\n
\n 		01.00.00
\n		Dec 20, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/

/* ******** */
/* Includes */
/* ******** */

#define __ZERO_CROSSING

#include "Config.h"
#include "LIB_ZeroCrossing.h"
#include "OCIRQ_drv.h"
#include "OCTPM_drv.h"
#include "LIB_GridVoltage.h"
#if defined(CONFIG_PL_CLASS_B_CHECK)
#include "LIB_ClassBCheck.h"
#endif /* defined(CONFIG_PL_CLASS_B_CHECK) */
#if (ENABLE_ZC_LOADS_MODULE	== 1)
#include "ZcLoads.h"
#endif
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
#include "syncSvc.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*#define DEBUG_ZC_VOLTAGE 1*/

#ifndef ZCV_HW_TMR_ID
#error "ZCV_HW_TMR_ID missing definition.."
#endif

/* If no ICU default timer defined, disable it */
#ifndef ZCV_ICU_TMR_ID
#define ZCV_ICU_TMR_ID	NUM_HW_CNTR_CH
#endif

#if defined(CONFIG_LIB_ZC_ON_ICU) && !defined(CONFIG_LIB_ZC_ICU_OVR_SETTING) && (ZCV_ICU_TMR_ID == NUM_HW_CNTR_CH)
#error "ZCV_ICU_TMR_ID must be set with a defined TMR reference!"
#endif

/**
Number of edges to discard, in order to obtain a consisting measure of
grid half period, this value can't be less than 2 because of
                                       1) Old timer measure initialization
                                       2) possible G.I.E. enabled after LIB_ZC_InitSync() call
*/
#define NUM_EDGES_TO_DISCARD    2u

/**
Max. number of times grid frequency found too low in order to set warning
N.B. this parameter is linked to NUM_CALL_CIZCG and FLT_GOOD_ZC_FALSE
*/
#define N_TIMES_FREQ_TOO_LOW    10
/**
Max. number of times grid frequency found too high in order to set warning
N.B. this parameter is linked to NUM_CALL_CIZCG and FLT_GOOD_ZC_FALSE
*/
#define N_TIMES_FREQ_TOO_HIGH   10

/**
LIB_ZC_CheckIfZC_Is_Good(): Filter before check if Zero Crossing Signal is in range
*/
#define NUM_CALL_CIZCG	    4
/**
LIB_ZC_CheckIfZC_Is_Good(): Filter to declare Zero Crossing Signal out of range
*/
#define FLT_GOOD_ZC_FALSE   3
/** LIB_ZC_CheckIfZC_Is_Good(): Upper limit for V line [V] */
#define V_TH_HIGH	280
/** LIB_ZC_CheckIfZC_Is_Good(): Lower limit for V line [V] */
#define V_TH_LOW	 93

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* grid semiperiod range descriptor */
typedef struct
{
    uint16_t nominal;		/* nominal period */
    uint16_t mean;			/* mean given by min and max period */
    uint16_t dev;			/* deviation */
}ZCSemiPeriodRangeType;

/* ********* */
/* Variables */
/* ********* */

/* Global */
/** This struct is used to decide if Zero Crossing Signal is right */
checkIfZC_Is_Good_t IsZC_Good= {0,0,0,0,0};
LIB_ZC_Control LIB_ZC_Control_Var;
#if defined(CONFIG_DISPATCHER_TASK_ENABLE)
NotifyDataEvent ZcEvent= {EV_NONE};
#endif

#if defined (ZERO_CROSSING_CIRCUIT_INVERTING)
#if (ZERO_CROSSING_CIRCUIT_INVERTING == TRUE)
	#define IRQ_MEASURE_VPEAK_EDGE  IRQ_ON_NEGATIVE_EDGE
	#define POS_HALF_WAVE           ICU_EDGE_MODE_RISING
	#define HalfWavePosUsec         LIB_ZC_Control_Var.HalfWaveLengthNegUsec
	#define HalfWaveNegUsec         LIB_ZC_Control_Var.HalfWaveLengthPosUsec
#else
	#define IRQ_MEASURE_VPEAK_EDGE  IRQ_ON_POSITIVE_EDGE
	#define POS_HALF_WAVE           ICU_EDGE_MODE_FALLING
	#define HalfWavePosUsec         LIB_ZC_Control_Var.HalfWaveLengthPosUsec
	#define HalfWaveNegUsec         LIB_ZC_Control_Var.HalfWaveLengthNegUsec
#endif  /* #if (ZERO_CROSSING_CIRCUIT_INVERTING == TRUE) */

#if defined(CONFIG_LIB_ZC_ON_ICU) && (ZERO_CROSSING_CIRCUIT_INVERTING == TRUE)
#define ICU_EDGE_INVERTED		TRUE
#else
#define ICU_EDGE_INVERTED		FALSE
#endif

#else
#error "\n ZERO_CROSSING_CIRCUIT_INVERTING property not defined in IRQ_cfg.h"
#endif  /* #if defined (ZERO_CROSSING_CIRCUIT_INVERTING) */

typedef struct {
	uint8_t ZcTout;
	uint8_t Old_goodZC;
	uint8_t Vline_cnt;
	uint8_t Zc_cnt;
	bool_t	OnGood;
	bool_t	OnNotGood;
} LIB_ZC_Good_Control;

static LIB_ZC_Good_Control LIB_ZC_Good_Control_Var;

/** This vector is used to check, fixed nominal grid frequency, if grid frequency is in range */


static const ZCSemiPeriodRangeType ZcHalfPeriodRangeUsec[2] =
{
	{
	NOM_HALF_PERIOD_60HZ,
	(uint16_t)MEAN_HALF_PERIOD_60HZ,
	(uint16_t)DEV_HALF_PERIOD_60HZ
	},
	{
	NOM_HALF_PERIOD_50HZ,
	(uint16_t)MEAN_HALF_PERIOD_50HZ,
	(uint16_t)DEV_HALF_PERIOD_50HZ
	}
};



/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void LIB_ZC_InitICU(void);

#if defined (CONFIG_LIB_ZC_ON_ICU)
IO_ErrorType callbackICU_Event (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType validate);
#elif !defined(CONFIG_OCIRQ_STATIC_CFG)
IO_ErrorType callbackIRQ_Event (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType validate);
#endif

static bool_t LIB_ZC_CheckNominalFrequency(void);
static void LIB_ZC_ResetGridFreqWarnings(void);
static bool_t LIB_ZC_IsHalfPeriodInRange(uint32_t halfPeriodLengthUsec, uint8_t i);

uint32_t TimingValue2nd(int seconds);

void OnZc_EdgeValidated(void);
static void LIB_ZC_Maskable(bool_t edge, bool_t go_on);

/*static bool_t zcCalculate(bool_t validate, LIB_ZcEdgeType edge, IO_HwTimerType currentHalfPeriodMeasure);*/
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
 * This function enables interrupt on the M.C.U. pin dedicated to Zero Crossing Voltage,
 * this function must be called regardless of reset's type.
*/
void LIB_ZC_InitSync(void)
{
	SAFETY_REGION_START();

	memset(&LIB_ZC_Control_Var, 0, sizeof(LIB_ZC_Control_Var));
	LIB_ZC_Control_Var.NominalGridFrequency = NOM_FREQ_IS_UNDEFINED;
#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
	LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id = ZCV_ICU_TMR_ID;
	LIB_ZC_Control_Var.IcuZC_cfg.inverted = ICU_EDGE_INVERTED;
#endif
	LIB_ZC_ResetGridFreqWarnings();
	memset(&LIB_ZC_Good_Control_Var, 0, sizeof(LIB_ZC_Good_Control_Var));
	IsZC_Good.stability = 0;

	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_DATA_UPDATE(LIB_ZC_Control_Var);
	SAFETY_DATA_UPDATE(LIB_ZC_Good_Control_Var);
	SAFETY_REGION_END();

#if defined(CONFIG_LIB_ZC_ON_ICU)

	/* Input Capture Init. */
	LIB_ZC_InitICU();

#else
#if defined(CONFIG_LIB_ZC_ICU_REFERENCE)
	LIB_ZC_InitICU();
#endif

	OCIRQ_InitSync(callbackIRQ_Event);
	OCIRQ_SetEdgeSync(1); /*RISING*/
	OCIRQ_IntrptEnableSync(TRUE);
#endif /* #if defined(CONFIG_LIB_ZC_ON_ICU) */

}

void LIB_ZC_RegisterICUFromPin(uint8_t pin)
{
#if (defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)) && defined(CONFIG_LIB_ZC_ICU_OVR_SETTING)
	uint8_t tmpIcuCh = OCTMR_GetChannelIDFromPinID(pin);

	if (tmpIcuCh < NUM_HW_CNTR_CH)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(LIB_ZC_Control_Var);

		 /* ZC input capture */
		LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id = OCTMR_GetChannelIDFromPinID(pin);
		LIB_ZC_Control_Var.IcuZC_cfg.inverted = ICU_EDGE_INVERTED;

		SAFETY_DATA_UPDATE(LIB_ZC_Control_Var);
		SAFETY_REGION_END();

		LIB_ZC_InitICU();
	}
#endif
}

/**
This function returns TRUE if Syncronization Phase is completed.

\return 	bool_t

\retval		TRUE -> Syncronization Phase completed.
\retval		FALSE -> Syncronization Phase not completed.


\author 	Stefano Ughi
\date		Dec. 20, 2010
\version	1.0.0

*/
bool_t LIB_ZC_SyncCompleted(void)
{
	return (bool_t)( LIB_ZC_Control_Var.NumZC >= FLT_NUM_ZC );
}

/**
\n This function resets filters for zero crossing voltage syncronization phase.

\author 	stefano.ughi
\date		Dec 22, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_ZC_ResetSyncPhase(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IsZC_Good);
	SAFETY_DATA_CHECK(LIB_ZC_Control_Var);

	LIB_ZC_Control_Var.NumZC       = 0;
	LIB_ZC_Control_Var.GridFreqFlt = 0;
	IsZC_Good.goodZC               = FALSE;

	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_DATA_UPDATE(LIB_ZC_Control_Var);
	SAFETY_REGION_END();
}

static void LIB_ZC_ResetSyncPhaseInt(void)
{
	LIB_ZC_Control_Var.NumZC       = 0;
	LIB_ZC_Control_Var.GridFreqFlt = 0;
	IsZC_Good.goodZC               = FALSE;
}

static void LIB_ZC_InitICU(void)
{
#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
	if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_LIB_ZC_ON_ICU)
		IO_ModeType edge = ICU_EDGE_MODE_RISING;
#else
		IO_ModeType edge = (OCIRQ_GetEdgeSync() ? ICU_EDGE_MODE_RISING : ICU_EDGE_MODE_FALLING);
#endif
	    (void)OCICU_DeInitSync(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id);
	    (void)OCICU_InitSync(&LIB_ZC_Control_Var.IcuZC_cfg);
	    (void)OCICU_SetModeSync(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id,edge);
#if defined(CONFIG_LIB_ZC_ON_ICU)
	    (void)OCICU_EnableISR(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id, TMR_CH_ISR_ENABLE, callbackICU_Event);
#endif
	}
#endif
}

bool_t LIB_ZC_GetCurrentEdge(void)
{
	bool_t retVal = IRQ_ON_NEGATIVE_EDGE;
#if defined(CONFIG_LIB_ZC_ON_ICU)
	ICU_EdgeModeType mode;

    (void)OCICU_GetModeSync(ZCV_CH_ID, &mode);

    if ( ICU_EDGE_MODE_RISING == mode )
    {
    	retVal = IRQ_ON_POSITIVE_EDGE;
    }

#else
    retVal = OCIRQ_GetEdgeSync();

#endif

	return retVal;
}

/**
This function returns the nominal grid frequency..

\return 	uint8_t

\retval		NOM_FREQ_IS_50_HZ
\retval		NOM_FREQ_IS_60_HZ
\retval		NOM_FREQ_IS_UNDEFINED


\author 	Stefano Ughi
\date		Dec 22,2010
\version	1.0.0


*/
uint8_t LIB_ZC_GetNominalFrequency(void)
{
	return (LIB_ZC_Control_Var.NominalGridFrequency);
}

/**
This function returns the nominal halfwave in us.


\return 	uint16_t

\retval		us if valid
\retval     0 if invalid (NOM_FREQ_IS_UNDEFINED)


\author 	Roberto Fioravanti
\date		Apr 14,2011
\version	1.0.0


*/
uint16_t LIB_ZC_GetNominalHalfWave(void)
{
	uint16_t retVal=0;

    if(LIB_ZC_Control_Var.NominalGridFrequency != NOM_FREQ_IS_UNDEFINED)
    {
    	retVal = ZcHalfPeriodRangeUsec[LIB_ZC_Control_Var.NominalGridFrequency].nominal;
    }
    return retVal;
}

/**
Returns TRUE if a periodic alarm with task activation is requested otherwise FALSE..


\return 	bool_t

\retval		TRUE
\retval		FALSE   task activation

\author 	Stefano Ughi
\date		Jan 11,2011
\version	1.0.0

*/
bool_t LIB_ZC_PeriodicAlarm(void)
{
	bool_t retValue=FALSE;


#if (ENABLE_ZC_LOADS_MODULE == 1)
	retValue=TRUE;
#endif  /* ENABLE_ZC_LOADS_MODULE */

#ifdef __PROFILER_H
	retValue=TRUE;
#endif  /* __PROFILER_H */

	return retValue;
}
/**
Function called inside Task MeasureLive.

\param  [in]	isPresentVLine allowable values TRUE or FALSE
\param  [in]    vLine A/D value of V peak

\author 	Stefano Ughi
\date		Jan 12,2011
\version	1.0.1

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			Ex setLineVoltage

*/

void LIB_ZC_SetLineVoltageParams( bool_t isPresentVLine, ADC_ValueType vLine )
{
	EnterCriticalSection();
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IsZC_Good);

	IsZC_Good.vLineIsPresent = isPresentVLine;
	IsZC_Good.lineVoltage = vLine;

	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_REGION_END();
	ExitCriticalSection();
}
bool_t LIB_ZC_IsOnGood(void)
{
	return LIB_ZC_Good_Control_Var.OnGood;
}

bool_t LIB_ZC_IsOnNotGood(void)
{
	return LIB_ZC_Good_Control_Var.OnNotGood;
}

/**
\n Checks the congruence between Zero Crossing and V line (if available).
\n Called inside 10 msec Task.

\param  [in,out] freqValid is set to TRUE by zero crossing ISR
                 if grid frequency found in range for FLT_NUM_ZC times.

\param	[in,out] lineVoltage V peak [A/D]

\param	[in]     vLineIsPresent if TRUE V line measure available
\param	[out]    goodZC if TRUE the zero crossing signal is right


\author 	Stefano Ughi
\date		Jan 13,2011
\version	1.0.1


\note		\par \b Nota 1:
			Ex checkIfZC_Is_Good
*/
void LIB_ZC_CheckIfZC_Is_Good( void )
{
	int16_t vacRMS;
	EnterCriticalSection();
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IsZC_Good);
	SAFETY_DATA_CHECK(LIB_ZC_Good_Control_Var);

	LIB_ZC_Good_Control_Var.ZcTout ++;
	if ( LIB_ZC_Good_Control_Var.ZcTout >= NUM_CALL_CIZCG )
	{
		LIB_ZC_Good_Control_Var.ZcTout = 0;

		if ( TRUE == IsZC_Good.freqValid )
		{
			IsZC_Good.freqValid = FALSE;
			IsZC_Good.goodZC    = TRUE;
			LIB_ZC_Good_Control_Var.Zc_cnt = 0;
		}
		else
		{
			LIB_ZC_Good_Control_Var.Zc_cnt++;
			if ( LIB_ZC_Good_Control_Var.Zc_cnt > FLT_GOOD_ZC_FALSE )
			{
				IsZC_Good.goodZC = FALSE;
				LIB_ZC_Good_Control_Var.Zc_cnt = FLT_GOOD_ZC_FALSE;
			}
		}

		if ( TRUE == IsZC_Good.vLineIsPresent )
		{
			vacRMS = LIB_GV_getRMS_Vac( IsZC_Good.lineVoltage );
			if( ( vacRMS > V_TH_HIGH ) || ( vacRMS < V_TH_LOW ) )
			{
				LIB_ZC_Good_Control_Var.Vline_cnt++;
				if ( LIB_ZC_Good_Control_Var.Vline_cnt > FLT_GOOD_ZC_FALSE )
				{
					IsZC_Good.goodZC = FALSE;
					LIB_ZC_Good_Control_Var.Vline_cnt = FLT_GOOD_ZC_FALSE;
				}
			}
			else
			{
				LIB_ZC_Good_Control_Var.Vline_cnt = 0;
			}
			IsZC_Good.lineVoltage = 0;
		}
		LIB_ZC_Good_Control_Var.OnGood=0;
		LIB_ZC_Good_Control_Var.OnNotGood=0;

		if (IsZC_Good.goodZC != LIB_ZC_Good_Control_Var.Old_goodZC)
		{
			if ( FALSE == IsZC_Good.goodZC )
			{
				/* I lost the Zero Crossing Signal --> restart Zero Crossing Sync Phase */
				SAFETY_DATA_CHECK(LIB_ZC_Control_Var);
				LIB_ZC_ResetSyncPhaseInt();
				SAFETY_DATA_UPDATE(LIB_ZC_Control_Var);
				LIB_ZC_Good_Control_Var.OnNotGood=1;
			}
			else
			{
				/* Now I have a good Zero Crossing signal but before...*/
				LIB_ZC_Good_Control_Var.OnGood=1;
			}
			/* Reset stability */
			IsZC_Good.stability = 0;
		}
		else
		{
			/* Increase stability up to maximum value 0xFF */
			uint16_t tmpNewVal = IsZC_Good.stability + NUM_CALL_CIZCG;
			if (tmpNewVal > 0xFF)
			{
				tmpNewVal = 0xFF;
			}

			IsZC_Good.stability = (uint8_t) tmpNewVal;
		}
		LIB_ZC_Good_Control_Var.Old_goodZC = IsZC_Good.goodZC;
	}

	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_DATA_UPDATE(LIB_ZC_Good_Control_Var);
	SAFETY_REGION_END();
	ExitCriticalSection();
}
/**
Returns the value of IsZC_Good.goodZC var.

\return 	bool_t

\retval		TRUE Zero Crossing Signal is O.K.
\retval		FALSE Zero Crossing Signal is K.O.


\author 	stefano.ughi
\date		Jan 13, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t goodZCSignal( void )
{
	return IsZC_Good.goodZC;
}
/**
Returns the stability measure of the  zero crossing signal
*/
uint8_t getZCStability(void)
{
	return IsZC_Good.stability;
}

/**
Sets the value of IsZC_Good.goodZC var.

\param	[in] value the value assumed by variable


\author 	stefano.ughi
\date		Feb 14, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void SetgoodZCSignalTo(bool_t value)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IsZC_Good);

	IsZC_Good.goodZC = value;

	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_REGION_END();
}


/**
Returns TRUE if LIB_ZC_Control_Var.NumZC is greater than 0, otherwise FALSE.

\return 	bool_t

\retval		TRUE Zero Crossing Signal is O.K.
\retval		FALSE Zero Crossing Signal is K.O.


\author 	stefano.ughi
\date		Jan 13, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t IsZCSyncOngoing( void )
{
	bool_t retVal=FALSE;
	if (LIB_ZC_Control_Var.NumZC > 0)
	{
		retVal = TRUE;
	}
	return retVal;
}
/**
Returns if found Grid Frequency out of range.

\return 	uint8_t

\retval		GRID_FRQ_TOO_LOW
\retval		GRID_FRQ_TOO_HIGH
\retval		GRID_FRQ_IN_RANGE


\author 	stefano.ughi
\date		Jan 14, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint8_t LIB_ZC_GetGridFrequencyWarning( void )
{
	uint8_t retVal = GRID_FRQ_IN_RANGE;
	if ( TRUE == LIB_ZC_Control_Var.FreqTooLowWarning )
	{
		retVal = GRID_FRQ_TOO_LOW;
	}
	else if ( TRUE == LIB_ZC_Control_Var.FreqTooHighWarning )
	{
		retVal = GRID_FRQ_TOO_HIGH;
	}

	return retVal;
}

/* Local Function */

/**
Compares if halfPeriodLengthUsec in  the i-th range
\param  [in]	halfPeriodLengthUsec semiperiod in us
\param  [in]	i range identifier described by a ZCSemiPeriodRangeType

\return 	bool_t
\retval     TRUE, in range
            FALSE, not in range
*/
static bool_t LIB_ZC_IsHalfPeriodInRange(uint32_t _SemiPeriodLengthUsec, uint8_t i)
{
    bool_t  _InRange=FALSE;

    if( i >= (sizeof(ZcHalfPeriodRangeUsec)/sizeof(ZcHalfPeriodRangeUsec[0])))
    {
    	_InRange=FALSE;
    }else{
    	if(labs((int32_t)_SemiPeriodLengthUsec-(int32_t)ZcHalfPeriodRangeUsec[i].mean) < ZcHalfPeriodRangeUsec[i].dev)
    	{
    		_InRange = TRUE;
    	}
    }
    return _InRange;
}

/**
Returns the reference in us.

\return 	uint32_t
\retval     us
*/
uint32_t LIB_ZC_GetReference_us(void)
{
	uint32_t tmpTickRef = OCHWCNTR_GetTickResolution(ZCV_HW_TMR_ID);

#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
	if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
	{
		tmpTickRef = OCHWCNTR_GetTickResolution(OCTPM_GetTimerIDFromChannelID_1(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id));
	}
#endif

    return ((LIB_ZC_Control_Var.OldHalfPeriodMeasure * tmpTickRef) / 1000);
}


/**
 * Update LIB_ZC_Control_Var form timer data,edge and validation
 * \return TRUE if
 */
static bool_t zcCalculate(bool_t validate, LIB_ZcEdgeType edge, IO_HwTimerType currentHalfPeriodMeasure)
{
	bool_t go_on=FALSE;
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(IsZC_Good);
	SAFETY_DATA_CHECK(LIB_ZC_Control_Var);

	if (validate)
	{
	#if defined (DEBUG_ZC_VOLTAGE)
		uint32_t halfPeriodLengthUsec;              /* half period measure in uSec */
	#else
		uint16_t halfPeriodLengthUsec;              /* half period measure in uSec */
	#endif
		IO_HwTimerType halfPeriodLength;            /* half period measure in ticks (type verified) */

		OnZc_EdgeValidated();

#if defined(CONFIG_PM_MOTORGATE)
		PM_UM_OnZC((uint32_t) currentHalfPeriodMeasure);
#endif

		if ( LIB_ZC_Control_Var.CntrEdgesToDisc < NUM_EDGES_TO_DISCARD )
		{
			/* Startup phase */
			LIB_ZC_Control_Var.OldHalfPeriodMeasure = currentHalfPeriodMeasure;
			LIB_ZC_Control_Var.CntrEdgesToDisc ++;
			go_on = FALSE;
		}
		else
		{
			uint32_t tmpLenghtUsec;
			IO_ChannelType curHwTmr = ZCV_HW_TMR_ID;

#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
			if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
			{
				curHwTmr = OCTPM_GetTimerIDFromChannelID_1(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id);
			}
#endif
			halfPeriodLength = (uint16_t)(OCHWCNTR_GetDelta(curHwTmr, LIB_ZC_Control_Var.OldHalfPeriodMeasure, currentHalfPeriodMeasure));
			tmpLenghtUsec = (((IO_TimingValueType)halfPeriodLength*OCHWCNTR_GetTickResolution(curHwTmr))/1000);

#if defined (DEBUG_ZC_VOLTAGE)
			if ( tmpLenghtUsec = 0xFFFF )
			{
				tmpLenghtUsec = 0xFFFF;
				PLT_ERROR_WARNING(PLT_ERROR_LIB_ZC_1);
			}
#endif   /* #if defined (DEBUG_ZC_VOLTAGE) */

			halfPeriodLengthUsec = (uint16_t) tmpLenghtUsec;


			if ( !LIB_ZC_SyncCompleted() )
			{
				/* Sync phase */

				if ( (halfPeriodLengthUsec >= MIN_HALF_PERIOD_SYNC_50HZ) &&
					(halfPeriodLengthUsec <= MAX_HALF_PERIOD_SYNC_50HZ) ) /* 50Hz*/
				{
					LIB_ZC_Control_Var.GridFreqFlt--;
					LIB_ZC_Control_Var.NumZC++;
				}
				else if ( (halfPeriodLengthUsec >= MIN_HALF_PERIOD_SYNC_60HZ) &&
						(halfPeriodLengthUsec <= MAX_HALF_PERIOD_SYNC_60HZ) ) /*60Hz*/
				{
					LIB_ZC_Control_Var.GridFreqFlt++;
					LIB_ZC_Control_Var.NumZC++;
				}
				else
				{
					LIB_ZC_ResetSyncPhaseInt();
				}

				LIB_GV_InitNominalGridVoltageMeasure();
				go_on = FALSE;
			}
			else
			{
				/* Sync phase completed */
				if ( !LIB_ZC_CheckNominalFrequency() )
				{
					/* There are some troubles with grid frequency */
					LIB_ZC_ResetSyncPhaseInt();
					go_on = FALSE;
				}
				else
				{
					if ( LIB_ZC_IsHalfPeriodInRange (halfPeriodLengthUsec, LIB_ZC_Control_Var.NominalGridFrequency) )
					{
						/* Grid Frequency in range */
						LIB_ZC_ResetGridFreqWarnings();
						if ( ZC_EDGE_RISING == edge )
						{
							LIB_ZC_Control_Var.HalfWaveLengthNegUsec = halfPeriodLengthUsec;
						}
						else
						{
							LIB_ZC_Control_Var.HalfWaveLengthPosUsec = halfPeriodLengthUsec;
						}

						/* Flag for LIB_ZC_CheckIfZC_Is_Good() */
						IsZC_Good.freqValid = TRUE;

						if((HalfWavePosUsec) && (HalfWaveNegUsec))
						{
							go_on = TRUE;
						}
					}
					else
					{
						/* Grid Frequency out of range */
						if ( halfPeriodLengthUsec > (ZcHalfPeriodRangeUsec[LIB_ZC_Control_Var.NominalGridFrequency].mean + ZcHalfPeriodRangeUsec[LIB_ZC_Control_Var.NominalGridFrequency].dev) )
						{
							/* half period length too long */
							if ( LIB_ZC_Control_Var.CntrFreqTooLow < N_TIMES_FREQ_TOO_LOW )
							{
								LIB_ZC_Control_Var.CntrFreqTooLow++;
							}
							else
							{
								LIB_ZC_Control_Var.FreqTooLowWarning = TRUE;
							}
						}
						else
						{
							/* half period length too short */
							if ( LIB_ZC_Control_Var.CntrFreqTooHigh < N_TIMES_FREQ_TOO_HIGH )
							{
								LIB_ZC_Control_Var.CntrFreqTooHigh++;
							}
							else
							{
								LIB_ZC_Control_Var.FreqTooHighWarning = TRUE;
							}
						}

						/* Reset both half wave length values */
						LIB_ZC_Control_Var.HalfWaveLengthNegUsec = 0;
						LIB_ZC_Control_Var.HalfWaveLengthPosUsec = 0;

						go_on = FALSE;
					}
				}
			}
			LIB_ZC_Control_Var.OldHalfPeriodMeasure = currentHalfPeriodMeasure;
		}
	}
	else
	{
		/* glitch probably due more to Mr. Brufani.
		1) don't change z.c. sync edge
		2) don't update LIB_ZC_Control_Var.OldHalfPeriodMeasure
		*/
		go_on = FALSE;
	}

	SAFETY_DATA_UPDATE(LIB_ZC_Control_Var);
	SAFETY_DATA_UPDATE(IsZC_Good);
	SAFETY_REGION_END();
	return go_on;
}


/**
\n Zero Crossing Voltage Event.
\n This callback is called by IRQ ISR.
\n for a detailed description please read pdf document inside LIB_Zero_Crossing folder

\param  [in]	notifVal Zero Crossing sync edge, available values are:\n
                IRQ_ON_NEGATIVE_EDGE\n
                IRQ_ON_POSITIVE_EDGE

\param  [in]        chid    not used
\param  [in]        validate   if TRUE valid edge otherwise glitch


\return 	IO_ErrorType

\retval		IO_E_OK


\author 	stefano.ughi
\date		Dec 20, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			removed code under define V_LINE_MEASURE because belonging to backup for V line


*/
/*lint -esym(715,chid) chid not used */
#if !defined (CONFIG_LIB_ZC_ON_ICU)
IO_ErrorType callbackIRQ_Event (IO_NotificationType edge, IO_ChannelType chid, IO_ErrorType validate)
{
	IO_HwTimerType currentHalfPeriodMeasure = OCHWCNTR_GetTicksSync(ZCV_HW_TMR_ID);    /* Current timer's value in ticks (type verified) */
	bool_t go_on;
	(void)chid;

	TRACE_ZC_IRQ_START();

#if defined(CONFIG_LIB_ZC_ICU_REFERENCE)
	if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
	{
		currentHalfPeriodMeasure = OCICU_GetTickSync(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id);
	}
#endif

    if (validate)
	{
		OCIRQ_ChangeEdgeSync((bool_t)edge);
		OCHWCNTR_Trigger(HW_CNTR_TRIG_ZC);

#if defined(CONFIG_LIB_ZC_ICU_REFERENCE)
		if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
		{
			OCOCU_SetTickValByTriggerIndex(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id, HW_CNTR_TRIG_ZC, currentHalfPeriodMeasure);
			OCICU_SetModeSync(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id,
				(edge ? ICU_EDGE_MODE_FALLING : ICU_EDGE_MODE_RISING));
		}
#endif
	}
    go_on = zcCalculate(validate, (LIB_ZcEdgeType)edge, currentHalfPeriodMeasure);



#if !defined(IRQ_IS_UNMASKABLE_INT)
	LIB_ZC_Maskable(edge, go_on);
#else
	/* LIB_ZC_Maskable() will be called by maskable interrupt */
	LIB_ZC_Control_Var.go_on= go_on;
	LIB_ZC_Control_Var.edge=edge;
#endif /* defined(IRQ_IS_UNMASKABLE_INT) */

	TRACE_ZC_IRQ_END();
	return (IO_ErrorType)IO_E_OK;
}
#endif /* #if !defined CONFIG_LIB_ZC_ON_ICU */


/**
\n Zero Crossing Voltage Event.
\n This callback is called by an Input capture ISR.
\n for a detailed description please read pdf document inside LIB_Zero_Crossing folder

\param  [in]		notifVal Zero Crossing sync edge
\param  [in]        chid    channel id (not used)
\param  [in]        validate   not used yet (always IO_E_OK)


\return 	IO_ErrorType

\retval		IO_E_OK


\author 	Roberto Fioravanti
\date		Mar 31, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
			\par \b Note 2:
			removed code under define V_LINE_MEASURE because belonging to backup for V line

\todo:  in case Grid Frequency in range between positive and negative edge must be discerned
\todo: validate must assume the following meaning : if FALSE valid edge, otherwise glitch
*/
/*lint -esym(715,notifVal)*/
#if defined(CONFIG_LIB_ZC_ON_ICU)
IO_ErrorType callbackICU_Event (IO_NotificationType edge, IO_ChannelType chid, IO_ErrorType err)
{
	IO_HwTimerType currentHalfPeriodMeasure;    /* Current timer's value in ticks (type verified) */
	bool_t go_on;
	bool_t validate;
	ICU_EdgeModeType mode;
	/* MS TRACE_TO_PIN(1,24); */

	currentHalfPeriodMeasure        = OCICU_GetTickSync(chid);

   (void)OCICU_GetModeSync(chid, &mode);
    if(mode == POS_HALF_WAVE){
		edge = ZC_EDGE_RISING;
	}else{
		edge = ZC_EDGE_FALLING;
	}

	if(err == 0){
		validate = TRUE;
		(void)OCICU_ToggleModeSync(chid);
	}else{
		validate = FALSE;
	}

	go_on = zcCalculate(validate, edge, currentHalfPeriodMeasure);

#if !defined(IRQ_IS_UNMASKABLE_INT)
	LIB_ZC_Maskable(edge, go_on);
#else
	/* LIB_ZC_Maskable() will be called by maskable interrupt */
	LIB_ZC_Control_Var.go_on= go_on;
	LIB_ZC_Control_Var.edge=edge;
#endif /* defined(IRQ_IS_UNMASKABLE_INT) */

	/* TRACE_TO_PIN(0,24); */
	return (IO_ErrorType)IO_E_OK;
}
#endif /* #if defined CONFIG_LIB_ZC_ON_ICU */

static void LIB_ZC_Maskable(bool_t edge, bool_t go_on)
{
	uint16_t HalfWaveLengthUsec;
	if( ZC_EDGE_RISING == edge )
	{
		HalfWaveLengthUsec = HalfWavePosUsec;
	}
	else
	{
		HalfWaveLengthUsec = HalfWaveNegUsec;
	}

    if ( TRUE == go_on )
	{
		OnZc_OK((LIB_ZcEdgeType)edge, HalfWaveLengthUsec);
	}
	else
	{
		/* Here perform actions if Zero Crossing is not valid */
		OnZc_KO((LIB_ZcEdgeType)edge, HalfWaveLengthUsec);
	}
}

#if defined(IRQ_IS_UNMASKABLE_INT)
void LIB_ZC_MaskableHandler(void)
{
	LIB_ZC_Maskable(LIB_ZC_Control_Var.edge, LIB_ZC_Control_Var.go_on);
}
#endif /* defined(IRQ_IS_UNMASKABLE_INT)*/
/**
Returns the minimum pulse width, in uSec, useful for driving loads of type "ZcLoads".

\param  [in] LIB_ZC_Control_Var.HalfWaveLengthPosUsec uSec
\param  [in] LIB_ZC_Control_Var.HalfWaveLengthNegUsec uSec

\return 	uint16_t

\retval		minimum pulse width in uSec


\author 	Stefano Ughi
\date		Jan 11,2011
\version	1.0.0

*/

uint16_t LIB_ZC_GetPartialWidth(void)
{
	uint16_t minHalfPeriod;

	if( LIB_ZC_Control_Var.HalfWaveLengthPosUsec < LIB_ZC_Control_Var.HalfWaveLengthNegUsec )
	{
		minHalfPeriod = LIB_ZC_Control_Var.HalfWaveLengthPosUsec;
	}
	else
	{
		minHalfPeriod = LIB_ZC_Control_Var.HalfWaveLengthNegUsec;
	}
	return (uint16_t)((minHalfPeriod/N_LEVEL) - OFFSET_PARTIAL_WIDTH);
}

/**
Returns the lenght of positive half wave, in uSec, necessary for driving partialized loads.

\param  [out] LIB_ZC_Control_Var.HalfWaveLengthPosUsec

\return 	uint16_t

\retval		lenght of positive half wave


\author     Leonardo Sparti
\date       Oct 25,2011
\version    1.0.0

*/
uint16_t LIB_ZC_GetHalfWaveLengthPos(void)
{
	return LIB_ZC_Control_Var.HalfWaveLengthPosUsec;
}

/**
Returns the lenght of negative half wave, in uSec, necessary for driving partialized loads.

\param  [out] LIB_ZC_Control_Var.HalfWaveLengthNegUsec

\return 	uint16_t

\retval		lenght of negative half wave


\author     Leonardo Sparti
\date       Oct 25,2011
\version    1.0.0

*/
uint16_t LIB_ZC_GetHalfWaveLengthNeg(void)
{
	return LIB_ZC_Control_Var.HalfWaveLengthNegUsec;
}

/** Returns the mimimum length of the semi-period un us.

\return 	uint16_t

\retval		lenght of negative half wave

\author     Roberto Fioravanti
\date       Feb 22,2012
\version    1.0.0

*/
uint16_t LIB_ZC_GetSemiPeriod(void)
{
	uint16_t minHalfPeriod;

	if( LIB_ZC_Control_Var.HalfWaveLengthPosUsec < LIB_ZC_Control_Var.HalfWaveLengthNegUsec )
	{
		minHalfPeriod = LIB_ZC_Control_Var.HalfWaveLengthPosUsec;
	}
	else
	{
		minHalfPeriod = LIB_ZC_Control_Var.HalfWaveLengthNegUsec;
	}
	return (uint16_t)(minHalfPeriod);
}

/** Returns the period un us.

\return 	uint16_t

\retval		lenght of full wave

\author     Alessandro Sabbatini
\date       Feb 26,2014
\version    1.0.0

*/
uint16_t LIB_ZC_GetPeriod(void)
{
	return (uint16_t) (LIB_ZC_Control_Var.HalfWaveLengthPosUsec +
			LIB_ZC_Control_Var.HalfWaveLengthNegUsec);
}

/**
\n This function initializes modules linked to periodic alarm.

\param  [in]  notifVal Zero Crossing sync edge

\author 	Stefano Ughi
\date		Jan 12, 2011
\version	1.0.0
*/

void LIB_ZC_InitPeriodicEvents(IO_NotificationType notifVal)
{
    (void) notifVal;

#if (ENABLE_ZC_LOADS_MODULE == 1)
	if ( LIB_ZcLoadsAreLoadsRegistered() == TRUE)
	{
		LIB_ZcLoads_Manager( LIB_ZCLOAD_NEW_EDGE );
	}
#endif /* #if (ENABLE_ZC_LOADS_MODULE == 1) */
#ifdef __PROFILER_H
	PM_ProfilesManager(notifVal);
#endif /* __PROFILER_H	*/
}

/**
\n This function checks if nominal grid frequency was correctly acquired and set LIB_ZC_Control_Var.NominalGridFrequency
\n according to.

\return 	bool_t

\retval		TRUE    O.K.
\retval		FALSE   Problem

\author 	Stefano Ughi
\date		Dec 23, 2010
\version	1.0.0


*/
static bool_t LIB_ZC_CheckNominalFrequency(void)
{
	bool_t retVal = FALSE;
	if ( LIB_ZC_Control_Var.GridFreqFlt > (FLT_NUM_ZC-DELTA_FRQ) )
	{
		LIB_ZC_Control_Var.NominalGridFrequency = NOM_FREQ_IS_60_HZ;
		retVal = TRUE;
	}
	else if ( LIB_ZC_Control_Var.GridFreqFlt < (-(FLT_NUM_ZC-DELTA_FRQ)) )
	{
		LIB_ZC_Control_Var.NominalGridFrequency = NOM_FREQ_IS_50_HZ;
		retVal = TRUE;
	}
	else
	{
		LIB_ZC_Control_Var.NominalGridFrequency = NOM_FREQ_IS_UNDEFINED;
		retVal = FALSE;
	}
	return retVal;
}
/**
\n This function resets grid frequency warnings and counters

\author 	Stefano Ughi
\date		Jan 10, 2012
\version	1.0.0


*/
static void LIB_ZC_ResetGridFreqWarnings(void)
{
	LIB_ZC_Control_Var.CntrFreqTooLow = 0;
	LIB_ZC_Control_Var.CntrFreqTooHigh = 0;
	LIB_ZC_Control_Var.FreqTooLowWarning = FALSE;
	LIB_ZC_Control_Var.FreqTooHighWarning = FALSE;

}


#if (NUM_EDGES_TO_DISCARD >= 255)
#error "\n NUM_EDGES_TO_DISCARD must be less of 255"
#endif

#if ((FLT_NUM_ZC > 126) || (FLT_NUM_ZC&0x01) )
#error "\n FLT_NUM_ZC must be less of 127 and even"
#endif

#if (DELTA_FRQ > FLT_NUM_ZC)
#error "\n DELTA_FRQ must be less or equal to FLT_NUM_ZC"
#endif

#if (N_TIMES_FREQ_TOO_LOW > 255)
#error "\n N_TIMES_FREQ_TOO_LOW must be less or equal to 255"
#endif

#if (N_TIMES_FREQ_TOO_HIGH > 255)
#error "\n N_TIMES_FREQ_TOO_HIGH must be less or equal to 255"
#endif

/* Macro per periodo di RETE */
#define PERIODO_RETE_NUM_SAMPLES          		201
#define PERIODO_RETE_OUT_OF_RANGE_PERMILLE		25	/* Permille specifies the tenth of a percent or one part per thousand */
/* Variabili per calcolo della media del periodo di Rete */
uint8_t  avgcnt=PERIODO_RETE_NUM_SAMPLES;
uint32_t accPeriodoRete;
uint32_t lasticval;
uint32_t discardedperiod=0;
uint32_t periodoReteMedio=0;

uint32_t TimingValue2nd(int seconds)
{
	return  BUS_FREQUENCY_Hz * seconds;
}


/* returns nominal grid period in us */
uint32_t LIB_ZC_GetPeriodoReteNominale(void)
{
	uint32_t retVal=0;
	if( LIB_ZC_GetNominalFrequency() == NOM_FREQ_IS_50_HZ )
	{
		retVal = (1000000)/50;
	}else if( LIB_ZC_GetNominalFrequency() == NOM_FREQ_IS_60_HZ )
	{
		retVal = (1000000)/60;
	}
	return retVal;
}

uint32_t LIB_ZC_GetPeriodoMedio(void)
{
	return periodoReteMedio;
}
/**
Procedura determinazione media del periodo di rete.
\author 	Roberto Fioravanti
*/

#if defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD)
void LIB_ZC_PeriodoReteMedioManager(void)
{
	uint8_t  discard  = 0;
	uint8_t curHwTmr = ZCV_HW_TMR_ID;
	uint32_t period   = 0;
	uint32_t refperiod;
	uint32_t th;

#if defined(CONFIG_LIB_ZC_ICU_REFERENCE) || defined(CONFIG_LIB_ZC_ON_ICU)
	if (LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id < NUM_HW_CNTR_CH)
	{
		curHwTmr = OCTPM_GetTimerIDFromChannelID_1(LIB_ZC_Control_Var.IcuZC_cfg.ICU_Id);
	}
#endif

	if(avgcnt == PERIODO_RETE_NUM_SAMPLES)
	{
		accPeriodoRete=0;
		discardedperiod=0;

		if(!periodoReteMedio)
		{
			periodoReteMedio = LIB_ZC_GetPeriodoReteNominale();
		}
	}
	else
	{
		period = OCHWCNTR_GetDelta(curHwTmr, lasticval, LIB_ZC_Control_Var.OldHalfPeriodMeasure);
		period *= OCHWCNTR_GetTickResolution(curHwTmr);
		period /= 1000;

        refperiod =  LIB_ZC_GetPeriodoReteNominale();

        th = (refperiod*PERIODO_RETE_OUT_OF_RANGE_PERMILLE)/1000;

        if(abs(period - refperiod) > th)
        {
            /* discard sample */
            discardedperiod++;
            discard=1;
        }

        if(!discard)
        {
            /* the sample has not been discarded */
            accPeriodoRete+=(period);
        }

	}

    lasticval = LIB_ZC_Control_Var.OldHalfPeriodMeasure;

    avgcnt--;

    if(!avgcnt)
    {
        avgcnt=PERIODO_RETE_NUM_SAMPLES;

        if(discardedperiod != (PERIODO_RETE_NUM_SAMPLES-1)) /* avoid to divide by zero => all samples have been discarded */
        {
            periodoReteMedio = accPeriodoRete/((PERIODO_RETE_NUM_SAMPLES-1)-discardedperiod);
        }
    }
}
#endif /* defined(CONFIG_MAIN_LIVE_AVERAGE_GRID_PERIOD) */

/** Zerocrossing callback
Called at each validated edge
*/
void OnZc_EdgeValidated(void)
{
	/*update tick for alarm*/
	LIB_ZC_Control_Var.ZcRefTick = OS_GetTickRef();
	/*update tick for alarm*/
	OS_DI();
	LIB_ZC_Control_Var.SysTime = OS_GetTime_Cycles();
	OS_RESTORE_I();
}

OS_U32 Get_ZCSysTime(void)
{
	return LIB_ZC_Control_Var.SysTime;
}





/* *********** */
/* End of file */
/* *********** */

