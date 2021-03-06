/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup   PM_ConductivitySensor
@{
\n    This module handles a conductivity sensor circuit (e.g. for drying level)
\n    changes on conductivity are detected as a frequency change on signal.
\n    Based on OCPWD Driver.
\n
\par  Configuration:
\li CONFIG_CSN_VALUE_32BIT, CONFIG_CSN_VALUE_16BIT output value type
\li CONFIG_CSN_EDGE_RISING
\li CONFIG_CSN_EDGE_FALLING
\li CONFIG_CSN_ONESHOT_ENABLE_FREQ
\li CONFIG_CSN_MEASURE_PERIOD
\li CONFIG_CSN_DISABLE_MODULE
\li CONFIG_CSN_CONTINUOUS_ENABLE_FREQ
\n
\n  See CSN_cfg.h file for configuration parameters.
\n
\n
\par Exported Functions:
\n
\n  Internal:
\n  \li   void PM_CSN_Init(uint8_t pin,const uint8_t *pCondSensParams);
\n  \li   void PM_CSN_SetChannelStatus(uint8_t status );
\n  \li   void PM_CSN_Manager(void);
\n  TODO
\n  \li   uint8_t PM_CSN_GetFaultBit(void);
\n  \li   void PM_CSN_SetFaultFlag(uint8_t newVal);
\n  \li   void PM_CSN_ResetChannel(void);
\n
\n  Avaliable to application:
\n  \li   bool_t PM_CSN_GetFaultBit(void);
\n  \li   void PM_CSN_UpdatePSValue(void);
\n

@}
\ingroup        ProductM
*/

/**
\n
\par    PM_ConductivitySensor Source file
\n  Contains function implementation of Conductivity sensor module.
\n
\file        PM_ConductivitySensor.c
\ingroup     PM_ConductivitySensor
\date               07/10/2011
\version          00.00.01
\author           Alessandro Sabbatini
\n
\n
\Par\b  History:
\n
\n    00.00.01
\n    07/10/2011
\n    Alessandro Sabbatini
\n    - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */

#include "autoconf.h"
#include "StandardDefines.h"
#include "OCDIO_drv.h"
#include "OCTPM_drv.h"
#include "LIB_Utilities.h"
#include "PM_ConductivitySensor.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifndef CONFIG_CSN_DISABLE_MODULE
#define CONFIG_CSN_DISABLE_MODULE (0)
#endif

#if defined(CONFIG_CSN_CHECK_IOSENSE)
#if defined(CONFIG_CSN_EDGE_RISING)
#define CSN_CHAN_INVERTED LIB_IOM_Get_IODir_ActiveSense(pin)
#elif defined(CONFIG_CSN_EDGE_FALLING)
#define CSN_CHAN_INVERTED !LIB_IOM_Get_IODir_ActiveSense(pin)
#else
#error "CONFIG_CSN_EDGE_RISING and CONFIG_CSN_EDGE_FALLING missing definition"
#endif
#else
#if defined(CONFIG_CSN_EDGE_RISING)
#define CSN_CHAN_INVERTED 0
#elif defined(CONFIG_CSN_EDGE_FALLING)
#define CSN_CHAN_INVERTED 1
#else
#error "CONFIG_CSN_EDGE_RISING and CONFIG_CSN_EDGE_FALLING missing definition"
#endif
#endif


#if (CONFIG_CSN_ONESHOT_ENABLE_FREQ <= CONFIG_CSN_CONTINUOUS_ENABLE_FREQ)
#error CONFIG_CSN_ONESHOT_ENABLE_FREQ must be higher than CONFIG_CSN_CONTINUOUS_ENABLE_FREQ
#endif

#define CSN_ONESHOT_ENABLE_THRESHOLD		(1000000000 / (CONFIG_CSN_ONESHOT_ENABLE_FREQ))
#define CSN_CONTINUOUS_ENABLE_THRESHOLD		(1000000000 / (CONFIG_CSN_CONTINUOUS_ENABLE_FREQ))

#if !defined(CSN_EVAL_VALUE)
#error CSN_EVAL_VALUE must be defined!
#endif

#if !defined(CSN_ACCUMULATE)
#error CSN_ACCUMULATE must be defined!
#endif

#if defined (CONFIG_CSN_MEASURE_PERIOD)
#define		CSN_UPDATE_NUM_SAMPLES(x)	((x) - 1)
#define		CSN_MAXREAD_FINAL(x)			(POSTCONV_CSN_VALUE(x) / CSN_Control_Var.UnitMeasure)
#elif defined (CONFIG_CSN_MEASURE_SEMIPERIOD)
#define		CSN_UPDATE_NUM_SAMPLES(x)	(((x) > 1) ? ((x)-2) : 0)
#define		CSN_MAXREAD_FINAL(x)			(POSTCONV_CSN_VALUE(x) / (2 * CSN_Control_Var.UnitMeasure))
#else
#error "CONFIG_CSN_MEASURE_PERIOD and CONFIG_CSN_MEASURE_SEMIPERIOD missing definition"
#endif

#define		CSN_VALUE_FINAL(x)			(POSTCONV_CSN_VALUE(x) / CSN_Control_Var.UnitMeasure)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Standard type used for Conductivity Sensor accumulators */
typedef uint32_t CSN_AccuType;

/** CSN_Control_Var global variable type definition */
typedef struct
{
    /** PWD channel configuration used as Conductivity sensor */
    IO_PWD_CfgType     PwdCfg;
    /** # samples to count for average */
    uint8_t numSamples;
    /** Signal out of range Timeout Parameter */
    uint8_t Timeout;
    /** Sampling Period Time */
    uint8_t Period;
    /** Output unit of Measure */
    uint8_t UnitMeasure;
    /** Lowest good value for acceptance */
    CSN_AccuType MinValue;
    /** Highest good value for acceptance */
    CSN_AccuType MaxValue;
    /** Highest sample value read (until reset) */
    CSN_AccuType MaxReadValue;
    /** Flags bitmap */
    struct
    {
        /** ON when module is configured */
        uint8_t InitDone        :1;
        /** ON when module is enabled */
        uint8_t Enabled         :1;
        /** ON when continuous mode sampling is enabled */
        uint8_t ContinuousMode  :1;
        /** ON when a new measure is ready */
        uint8_t Ready           :1;
        /** A new evaluated value is available after a CSN_UpdatePSValue call */
        uint8_t EvalUpdated     :1;
        /** Free for future Use */
        uint8_t Free5           :1;
        /** Out of range notification */
        uint8_t Overflow		:1;
        /** ON if no frequency value in range received for a time
        longer than timeout */
        uint8_t FaultFlag       :1;
    }
    Ctrl;
    /** # samples remaining to accumulate */
    uint8_t  SamplesCnt;
    /** Sampling Period Time counter */
    uint8_t PeriodCnt;
    /** 10 msec counter used to check no good signal timeout - fault */
    uint8_t TimerTimeOut;
    /** Accumulator */
    CSN_AccuType Acc;
    /** Last sample measured */
    CSN_AccuType Sample;
}
CSN_Control_var_t;

/* ********* */
/* Variables */
/* ********* */

/* Exported */
extern const IO_ModuleIOMapType ModuleIOmap[];

/* Static */

/* Global */

/** CSN Control variable */
CSN_Control_var_t CSN_Control_Var;

/** Conductivity Sensor value - average value converted to required unit measure */
CSN_ValueType CSNValue;
/** Conductivity Sensor Counter value */
uint8_t CSNSamplesCnt;


/* ******************* */
/* Function Prototypes */
/* ******************* */

/** Callback used to check if it's in range and update average value */
IO_ErrorType PM_CSN_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error );

/* ****************** */
/* External functions */
/* ****************** */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Conductivity Sensor initialization - called in InputInitialization()
regardless of reset type occured.

\param  [in]  pin MCU module pin where Conductivity sensor is attached
\param  [in]  pCondSensParams pointer to related setting file parameters

\attention \b CSN_Control_Var used as I/O

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_CSN_Init ( uint8_t pin, const uint8_t *pCondSensParams )
{
    IO_ChannelType tmpChannel;
    
#if (CONFIG_CSN_DISABLE_MODULE==0)
    
    /* Cleaning of CSN_Control_Var */
    /* (void)memset(&CSN_Control_Var,0,sizeof(CSN_Control_Var)); */

    /* Checks if pin module is a Timer channel */
    if ( OCTPM_GetChannelSync ( &ModuleIOmap[pin], &tmpChannel ) != ( IO_ErrorType ) IO_E_OK )
    {
        /* Some problems occured - leaving CSN unconfigured */
        CSN_Control_Var.PwdCfg.PWD_Id = 0xFF;
        CSN_Control_Var.Ctrl.FaultFlag = ON;
        return;
    }
    else
    {
        /* OK it is; prepare configuration */
        CSN_Control_Var.PwdCfg.PWD_Id = tmpChannel;
        CSN_Control_Var.PwdCfg.inverted = CSN_CHAN_INVERTED;
        
        /* Deinit PWD driver - no matter what happens, if in real trouble it won't be then initialized */
        (void) OCPWD_DeInitSync(tmpChannel);

        /* Try initialization of PWD Driver */
        if ( OCPWD_InitSync ( &CSN_Control_Var.PwdCfg ) == ( IO_ErrorType ) IO_E_OK )
        {

#if defined(CONFIG_CSN_DISABLE_TON_READING)
        	OCPWD_SetTonMeasure(CSN_Control_Var.PwdCfg.PWD_Id, FALSE);
#endif

#if (CONFIG_CSN_PWD_FILTER_TIME)
        	OCPWD_SetFilterTime(CSN_Control_Var.PwdCfg.PWD_Id, CONFIG_CSN_PWD_FILTER_TIME);
#endif

#if defined(CONFIG_CSN_DIGITAL_FILTER_FREQ)
        	OCDIO_SetIODigitalFilterFrequencySync(pin, CONFIG_CSN_DIGITAL_FILTER_FREQ);
        	OCDIO_SetIODigitalFilterPinSync(pin);
#endif

            /* Initialization of setting file parameters */

            CSN_Control_Var.Timeout = pCondSensParams[2];

            CSN_Control_Var.MaxValue = CSN_SETTING_CONV ( (CSN_AccuType) EndianU16_BToN(*((uint16_t *) &pCondSensParams[4] )));
            CSN_Control_Var.MinValue = CSN_SETTING_CONV ( (CSN_AccuType) EndianU16_BToN(*((uint16_t *) &pCondSensParams[6] )));

            CSN_Control_Var.Period = pCondSensParams[8];
            CSN_Control_Var.numSamples = pCondSensParams[9];

            CSN_DECODE_UNIT_MEASURE ( ( pCondSensParams[11]&0x07 ), CSN_Control_Var.UnitMeasure );

            if ( CSN_Control_Var.UnitMeasure == 0 )
            {
                /* Some problems occured - leaving CSN unconfigured */
                OCPWD_DeInitSync ( tmpChannel );
                CSN_Control_Var.PwdCfg.PWD_Id = 0xFF;
                CSN_Control_Var.Ctrl.FaultFlag = ON;
            }
            else
            {
                /* Initialization completed!
                 * Reset all average algorithm - used to inizialize accumulator indexes */
                CSN_Control_Var.Ctrl.InitDone = ON;
                PM_CSN_SetChannelStatus(OFF);
                PM_CSN_RestartReadings();
                PM_CSN_ResetMaxReadValue();
            }


        }

    }
    

#endif /*(CONFIG_CSN_DISABLE_MODULE==0)*/
    
}


/**
Enables / disables channel synchronization. \n
It is called in InputInizialization() regardless of reset occured.

\param  [in]  status flag used to start (!= 0) or stop (==0) the module

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_CSN_SetChannelStatus ( uint8_t status )
{
    IO_ErrorType res;
    
#if (CONFIG_CSN_DISABLE_MODULE==0)
    
    /* If channel is not initialized, do nothing */
    if ( CSN_Control_Var.Ctrl.InitDone == OFF )
    {
        return;
    }
    
    /* Align enabled flag to keep channel status */
    CSN_Control_Var.Ctrl.Enabled = OFF;
    
    if ( status == ON )
    {
        res = OCPWD_StartSampleSync ( CSN_Control_Var.PwdCfg.PWD_Id, PM_CSN_PWDCallback );
        
        /* Enable Frequency meter channel capture */
        if ( res == IO_E_OK )
        {
            /* Align enabled flag to keep channel status */
            CSN_Control_Var.Ctrl.Enabled = ON;
        }
    }
    else
    {
        /* Disable Frequency meter channel capture */
        OCPWD_StopSampleSync ( CSN_Control_Var.PwdCfg.PWD_Id );
    }
    
#endif /* (CONFIG_CSN_DISABLE_MODULE==0) */
    
}

/**
Reset average channel variables.

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
void PM_CSN_ResetChannel()
{
#if (CONFIG_CSN_DISABLE_MODULE==0)

    /* Force all accumulators to zero */
    CSN_Control_Var.Acc = CSN_INITIAL_ACC;
    
    /* Reset flags and mean value */
    CSN_Control_Var.Ctrl.Ready = OFF;
    CSN_Control_Var.Ctrl.ContinuousMode = OFF;
    CSN_Control_Var.Ctrl.Overflow = OFF;
    CSN_Control_Var.PeriodCnt = 0;
    
    CSN_Control_Var.SamplesCnt = CSN_Control_Var.numSamples;
    
#endif /* (CONFIG_CSN_DISABLE_MODULE==0) */
    
}

/**
Set Fault flag status ON or OFF; it initializes Timeout timer too.

\param  [in]  newVal New flag value

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
void PM_CSN_SetFaultFlag ( bool_t newVal )
{
#if (CONFIG_CSN_DISABLE_MODULE==0)

    if ( CSN_Control_Var.Ctrl.InitDone == ON )
    {
        CSN_Control_Var.Ctrl.FaultFlag = (newVal == ON);
        CSN_Control_Var.TimerTimeOut = CSN_Control_Var.Timeout;
    }
    
#endif /* (CONFIG_CSN_DISABLE_MODULE==0) */
}

/**
Restart reading: reset mean value and set fault status to OFF

\param      none

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
void PM_CSN_RestartReadings ( void )
{
    PM_CSN_SetFaultFlag ( OFF );
    PM_CSN_ResetChannel();
    
}

/**
Conductivity sensor manager - updates values taken from ISR callback once
they are ready and checks for timeout fault.
It is called in the Main task.

\attention \b CSN_Control_Var used as I/O
\attention \b CSNValue is NOT updated - application must call CSN_UpdateValue

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_CSN_Manager(void)
{

#if (CONFIG_CSN_DISABLE_MODULE==0)

    if ( CSN_Control_Var.Ctrl.InitDone == OFF )
    {
        /* Reset CSN Status to initial value - when enabled starts from zero */
        PM_CSN_ResetChannel();
        PM_CSN_ResetMaxReadValue();
        PM_CSN_SetFaultFlag ( OFF );
        CSN_Control_Var.PeriodCnt = 0;
        return;
    }
    
    /* Check If ISR callback completed samples collecting */
    if ( CSN_Control_Var.Ctrl.Ready == ON )
    {
        /* Check if period is in range */
        if ( ( CSN_Control_Var.MaxValue == 0 ) || \
                ( ( CSN_Control_Var.Sample >= CSN_Control_Var.MinValue ) \
                  && ( CSN_Control_Var.Sample <= CSN_Control_Var.MaxValue ) ) )
        {
        
            /* Check if new period sampled is higher than continuous threshold */
            if ( CSN_Control_Var.Sample >= CSN_CONTINUOUS_ENABLE_THRESHOLD )
            {
                CSN_Control_Var.Ctrl.ContinuousMode = ON;
            }
            
            if ( CSN_Control_Var.Sample > CSN_Control_Var.MaxReadValue)
            {
            	 CSN_Control_Var.MaxReadValue = CSN_Control_Var.Sample;
            }

            /* All samples collected - mean value available */
            if ( CSN_Control_Var.SamplesCnt == 0 )
            {
                /* Evaluate new mean value, reset accumulator,
                  samples counter and notify new update available */

            	CSN_AccuType tmpValue  = CSN_EVAL_VALUE(CSN_Control_Var.Acc, CSN_Control_Var.numSamples);

                tmpValue = CSN_VALUE_FINAL(tmpValue);

#if defined(CONFIG_CSN_VALUE_16BIT)

                CSN_Control_Var.Ctrl.Overflow = (tmpValue > 0xFFFFU);

                if ( !CSN_Control_Var.Ctrl.Overflow )
                {
                	CSNValue = ( CSN_ValueType ) tmpValue;
                }

#else

                CSNValue = (CSN_ValueType) tmpValue;

#endif /* (CONFIG_CSN_VALUE_16BIT) */

                CSN_Control_Var.Acc = CSN_INITIAL_ACC;
                CSN_Control_Var.SamplesCnt = CSN_Control_Var.numSamples;
                CSN_Control_Var.Ctrl.EvalUpdated = ON;
            }
            
            /* Collecting new sample get */
            CSN_Control_Var.SamplesCnt = CSN_UPDATE_NUM_SAMPLES(CSN_Control_Var.SamplesCnt);
            CSN_Control_Var.Acc = CSN_ACCUMULATE ( CSN_Control_Var.Acc, CSN_Control_Var.numSamples, CSN_Control_Var.Sample );
            
            if ( CSN_Control_Var.Ctrl.ContinuousMode )
            {
                CSN_Control_Var.PeriodCnt = 0;
            }
            else
            {
                /* Set new waiting time if not in continuous mode */
                CSN_Control_Var.PeriodCnt = CSN_Control_Var.Period;
            }
            
            /* Up counter for samples used as backward compatibility */
            CSNSamplesCnt = CSN_Control_Var.numSamples - CSN_Control_Var.SamplesCnt - 1;
            
            /* Reset any fault / Timeout conditions */
            PM_CSN_SetFaultFlag ( OFF );
            
        }
        
        /* Restart data collecting */
        CSN_Control_Var.Ctrl.Ready = OFF;
        
    }

    /* Timeout = 0 disables timeout */
	if ( CSN_Control_Var.Timeout )
	{
		/* Check if timeout occured */
		if ( CSN_Control_Var.TimerTimeOut > 0 )
		{
			/* Update timeout counter */
			CSN_Control_Var.TimerTimeOut--;
		}
		else
		{
			/* Reset all channel average and go in fault condition */
			PM_CSN_ResetChannel();
			PM_CSN_SetFaultFlag(ON);
			PM_CSN_SetChannelStatus(ON);
			CSN_Control_Var.PeriodCnt = 0;
		}
	}
    
    /* If channel was disabled, re-enables it */
    if ( CSN_Control_Var.Ctrl.Enabled == OFF )
    {
        if ( CSN_Control_Var.PeriodCnt )
        {
            CSN_Control_Var.PeriodCnt--;
        }
        else
        {
            PM_CSN_SetChannelStatus(ON);
        }
    }
    else
    {
        CSN_Control_Var.PeriodCnt = 0;
    }
    
#endif /* (CONFIG_CSN_DISABLE_MODULE==0) */
    
}


/**
Returns conductivity sensor running state; this mean module has been initialized

\attention \b LPS_Control_Var used as I

\return     bool_t

\retval     0 not yet ready to run
\retval     !=0  ready to run

\author     Alessandro Sabbatini
\date       29/05/2013
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

bool_t PM_CSN_IsRunning(void)
{

#if (CONFIG_LPS_DISABLE_MODULE==0)

    return CSN_Control_Var.Ctrl.InitDone;

#else
    return 0;

#endif

}


/**
Returns Conductivity fault state.

\attention \b CSN_Control_Var used as I

\return     bool_t

\retval     0 no fault
\retval     !=0  fault

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

bool_t PM_CSN_GetFaultFlag(void)
{

#if (CONFIG_CSN_DISABLE_MODULE==0)

    return CSN_Control_Var.Ctrl.FaultFlag;
    
#else
    
    return 0;
    
#endif
    
}

/**
Reset EvalUpdate flag to allow trigger next CSNValue update

\attention \b CSN_Control_Var used as O

\return     none

\author     Alessandro Sabbatini
\date       29/05/2013
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_CSN_WaitNextUpdate ( void )
{

#if (CONFIG_LPS_DISABLE_MODULE==0)
	CSN_Control_Var.Ctrl.EvalUpdated = OFF;
#endif

}


/**
Get CSN Status

\attention \b CSN_Control_Var used as I

\return     uint8_t

\retval     CSN_UPD_OK  update succeeded
\retval     CSN_UPD_NOTREADY update not yet ready
\retval     CSN_ERR_OVERFLOW update failed - average overflow occurred
\retval     CSN_ERR_FAULT update failed - fault occurred

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

CSN_Update_Result PM_CSN_GetStatus ( void )
{
	CSN_Update_Result retVal = CSN_UPD_OK;

#if (CONFIG_CSN_DISABLE_MODULE==0)
    
    if ( CSN_Control_Var.Ctrl.FaultFlag == ON )
    {
        retVal = CSN_ERR_FAULT;
    }
    else if ( CSN_Control_Var.Ctrl.InitDone == OFF )
    {
    	retVal = CSN_ERR_NOT_INIT;
    }
    else if ( CSN_Control_Var.Ctrl.Overflow == ON )
    {
    	retVal = CSN_ERR_OVERFLOW;
    }
    else if ( CSN_Control_Var.Ctrl.EvalUpdated == OFF )
    {
    	retVal = CSN_UPD_NOTREADY;
    }
    else
    {
    	/* Nothing else to do */
    }
    
#endif  /* (CSN_DISABLE_MODULE==0) */
    
    return retVal;
    
}

/**
Provide CSNValue variable

\return     CSN_ValueType

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

CSN_ValueType PM_CSN_GetValue(void)
{
    return CSNValue;
}

/**
Provide Maximum sample read

\return     CSN_ValueType

\author     Alessandro Sabbatini
\date       26/06/2013
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

CSN_ValueType PM_CSN_GetMaxReadValue(void)
{
    return CSN_MAXREAD_FINAL(CSN_Control_Var.MaxReadValue);
}

/**
Reset Maximum sample read

\return     none

\author     Alessandro Sabbatini
\date       26/06/2013
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_CSN_ResetMaxReadValue(void)
{
    CSN_Control_Var.MaxReadValue = 0;
}

/**
Provide Unit measure set from setting file

\return     CSN_ValueType

\author     Alessandro Sabbatini
\date       26/06/2013
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

CSN_ValueType PM_CSN_GetUnitMeasure_us(void)
{
    return CSN_Control_Var.UnitMeasure;
}

/* Local Function */

/**
OCPWD Callback to update period time

\param  [in]    notifVal Value of notification (IO_N_COMPLETED if all is OK)
\param  [in]    channel timer channel used (it should be same of CSN)
\param  [in]    error Error notification (IO_E_OK if all is OK)

\attention Parameters aligned on standard callbacks
\attention \b CSN_Control_Var used as I/O
\attention \b ModuleIOmap used as I

\author     Alessandro Sabbatini
\date       07/10/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
IO_ErrorType PM_CSN_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error )
{
    IO_PWD_MeasureType tmpMeasure;
    CSN_AccuType tmpTime;
    
    /* Check if notification status is good */
    if ( ( notifVal != IO_N_COMPLETED ) || ( error != IO_E_OK ) )
    {
        return IO_E_UNKNOWN_MODE;
    }
    
    /* IF CSN Update is disabled, do nothing */
    if ( CSN_Control_Var.Ctrl.InitDone != ON )
    {
        return IO_E_INVALID_CHANNEL_ID;
    }
    
    
    /* Check if channel is enabled (it shoud be!) and CSN_Manager
    already processed previous update or an update is in progress */
    if ( ( CSN_Control_Var.Ctrl.Enabled != ON ) || ( CSN_Control_Var.Ctrl.Ready != OFF ) )
    {
        return IO_E_WORKING;
    }
    
    /* Get last measure from OCPWD driver - all in nanoseconds! */
    OCPWD_GetAllSync (channel, &tmpMeasure);
    
    tmpTime = ( CSN_AccuType ) tmpMeasure.period;
    
    /* Update current accumulator with new value */
    CSN_Control_Var.Sample = tmpTime;
    
    CSN_Control_Var.Ctrl.Ready = ON;

    /* Check if new period sampled is lower than one-shot threshold 
     * If so, switch to one shot mode immediately!
     */
    if ( CSN_Control_Var.Sample < CSN_ONESHOT_ENABLE_THRESHOLD )
    {
        CSN_Control_Var.Ctrl.ContinuousMode = OFF;
    }
    
    if ( CSN_Control_Var.Ctrl.ContinuousMode == OFF )
    {
        /* Suspends channel synchronization after data ready */
        PM_CSN_SetChannelStatus ( OFF );
    }
    
    return IO_E_OK;
}


/* *********** */
/* End of file */
/* *********** */
