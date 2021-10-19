/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
 * \page pageLPS
 * \ingroup PM_LinearPressureSensor
 */

/**
\n
\n  Contains function implementation of Linear Pressure sensor module.
\n
\file        PM_LinearPressureSensor.c
\ingroup     PM_LinearPressureSensor
\date               23/08/2011
\version          00.00.01
\author           Alessandro Sabbatini
\n
\n
\Par\b  History:
\n
\n    00.00.01
\n    23/08/2011
\n    Alessandro Sabbatini
\n    - Creation
\n
*/


/* ******** */
/* Includes */
/* ******** */
#define __PM_LINEARPRESSURESENSOR

#include "Config.h"

#include "OCDIO_drv.h"
#include "OCTPM_drv.h"
#include "LIB_Utilities.h"
#include "PM_LinearPressureSensor.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifndef CONFIG_LPS_DISABLE_MODULE
#define CONFIG_LPS_DISABLE_MODULE (0)
#endif

#ifdef CONFIG_LPS_EDGE_RISING
#define LPS_CHAN_INVERTED 0
#elif defined(CONFIG_LPS_EDGE_FALLING)
#define LPS_CHAN_INVERTED 1
#else
#error "CONFIG_LPS_EDGE_RISING and CONFIG_LPS_EDGE_FALLING missing definition"
#endif

#if LPS_BIT_ACCUMULATORS > 7
#error Number of accumulators is too high! Maximum value is 7 (128 items)
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** Standard type used for Liner Pressure Sensor accumulators */
#if (LPS_ACCU_32BIT == ON)
typedef uint32_t LPS_AccuType;
#else
typedef uint16_t LPS_AccuType;
#endif

/** LPS_Control_Var global variable type definition */
typedef struct
{
    /** PWD channel configuration used as linear pressure sensor */
    IO_PWD_CfgType   PwdCfg;
    /** # bit samples to count for average */
    uint8_t bitSamples;
    /** Signal out of range Timeout Parameter */
    uint8_t Timeout;
    /** Actual accumulator index used */
    uint8_t curAccIdx;
    /** number of stored samples */
    uint8_t  curSamplesIndex;
    /** 10 msec counted used to check no good signal timeout - fault */
    uint8_t TimerTimeOut;
    /** Flags bitmap */
    struct
    {
        /** ON when module is configured */
        uint8_t InitDone        :1;
        /** ON when module is enabled */
        uint8_t Enabled         :1;
        /** ON when loop completed at least once */
        uint8_t FirstTimeDone   :1;
        /** ON when loop completed the first time */
        uint8_t Started         :1;
        /** ON when a new measure is ready */
        uint8_t Ready           :1;
        /** A new mean value is available after a LPS_UpdatePSValue call */
        uint8_t MeanUpdated     :1;
        /** Values out of range */
        uint8_t OutOfRange      :1;
        /** ON if no frequency value in range received for a time
        longer than timeout */
        uint8_t FaultFlag       :1;
    }
    Ctrl;
    /** Lowest good value for acceptance */
    LPS_AccuType MinValue;
    /** Highest good value for acceptance */
    LPS_AccuType MaxValue;
    /** Accumulator array */
    LPS_AccuType LPS_Acc[ ( 1u << LPS_BIT_ACCUMULATORS )];
    
}
LPS_Control_var_t;

/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* Global */

/** LPS Control variable */
static LPS_Control_var_t LPS_Control_Var;

/** LPS Mean value available for Application */
LPS_ValueType PSValue;

/** Flag used to disable LPS update - for debugging purposes only */
uint8_t PSDisable;

/* ******************* */
/* Function Prototypes */
/* ******************* */

/** Callback used to check if it's in range and update average value */
static IO_ErrorType PM_LPS_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error );

/* ****************** */
/* External functions */
/* ****************** */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
Linear Pressure Sensor initialization - called in InputInitialization()
regardless of reset type occured.

\param  [in]  pin MCU module pin where linear pressure sensor is attached
\param  [in]  pLinearPressureParams pointer to related setting file parameters

\attention \b LPS_Control_Var used as I/O

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_LPS_Init ( uint8_t pin, const uint8_t *pLinearPressureParams )
{
    IO_ChannelType tmpChannel;
    
#if (CONFIG_LPS_DISABLE_MODULE==0)
    
    /* Cleaning of LPS_Control_Var */
    /* (void)memset(&LPS_Control_Var,0,sizeof(LPS_Control_Var)); */
    
    /* Checks if pin module is a Timer channel */
    if ( OCTPM_GetChannelSync ( OCDIO_GetIOMapItem(pin), &tmpChannel ) == ( IO_ErrorType ) IO_E_OK )
    {
        /* OK it is; prepare configuration */
        LPS_Control_Var.PwdCfg.PWD_Id = tmpChannel;
        LPS_Control_Var.PwdCfg.inverted = LPS_CHAN_INVERTED;
        
        /* Deinit PWD driver - no matter what happens, if in real trouble it won't be then initialized */
        (void) OCPWD_DeInitSync(tmpChannel);

        /* Try initialization of PWD Driver */
        if ( OCPWD_InitSync ( &LPS_Control_Var.PwdCfg ) == ( IO_ErrorType ) IO_E_OK )
        {
        
#if defined(CONFIG_LPS_PWD_FILTER_TIME)
        	OCPWD_SetFilterTime(LPS_Control_Var.PwdCfg, CONFIG_LPS_PWD_FILTER_TIME);
#endif

#if defined(CONFIG_LPS_DIGITAL_FILTER_FREQ)
        	OCDIO_SetIODigitalFilterFrequencySync(pin, CONFIG_LPS_DIGITAL_FILTER_FREQ);
        	OCDIO_SetIODigitalFilterPinSync(pin);
#endif

            /* Initialization of setting file parameters */
            
            LPS_Control_Var.Timeout = pLinearPressureParams[2];
            LPS_Control_Var.bitSamples = ( uint8_t ) ( pLinearPressureParams[0]&0x07 );
            LPS_Control_Var.MaxValue = LPS_SETTING_CONV ((LPS_AccuType) EndianU16_BToN(*((uint16_t *) &pLinearPressureParams[4])));
            LPS_Control_Var.MinValue = LPS_SETTING_CONV ((LPS_AccuType) EndianU16_BToN(*((uint16_t *) &pLinearPressureParams[6])));
            LPS_Control_Var.Ctrl.InitDone = ON;
            
            /* Reset all average algorithm - used to inizialize accumulator indexes */
            PM_LPS_ResetChannel();
            PM_LPS_SetFaultFlag ( OFF );
            PM_LPS_SetChannelStatus( OFF );
        }
    }
    else
    {
        /* Some problems occured - leaving LPS unconfigured */
        LPS_Control_Var.PwdCfg.PWD_Id = 0xFF;
        LPS_Control_Var.Ctrl.FaultFlag = ON;
    }
    
#endif /*(CONFIG_LPS_DISABLE_MODULE==0)*/
    
}


/**
Enables / disables channel synchronization. \n
It is called in InputInizialization() regardless of reset occured.

\param  [in]  status flag used to start (!= 0) or stop (==0) the module

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_LPS_SetChannelStatus ( uint8_t status )
{
    IO_ErrorType res;
    
#if (CONFIG_LPS_DISABLE_MODULE==0)
    
    /* If channel is not initialized, do nothing */
    if ( LPS_Control_Var.Ctrl.InitDone == OFF )
    {
        return;
    }
    
    /* Align enabled flag to keep channel status */
    LPS_Control_Var.Ctrl.Enabled = OFF;
    
    if ( status == ON )
    {
        res = OCPWD_StartSampleSync ( LPS_Control_Var.PwdCfg.PWD_Id, PM_LPS_PWDCallback );
        
        /* Enable Frequency meter channel capture */
        if ( res == IO_E_OK )
        {
            /* Align enabled flag to keep channel status */
            LPS_Control_Var.Ctrl.Enabled = ON;
        }
    }
    else
    {
        /* Disable Frequency meter channel capture */
        OCPWD_StopSampleSync ( LPS_Control_Var.PwdCfg.PWD_Id );
    }
    
#endif /* (CONFIG_LPS_DISABLE_MODULE==0) */
    
}

/**
Reset average channel variables.

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
void PM_LPS_ResetChannel(void)
{
    uint8_t i;
    
#if (CONFIG_LPS_DISABLE_MODULE==0)
    
    if ( LPS_Control_Var.Ctrl.InitDone == OFF )
    {
        return;
    }
    
    
    /* Force all accumulators to zero */
    for ( i= ( 1u << LPS_BIT_ACCUMULATORS ); i > 0 ; )
    {
        i--;
        LPS_Control_Var.LPS_Acc[i] = 0;
    }
    
    /* Reset flags and mean value */
    LPS_Control_Var.Ctrl.Started = OFF;
    LPS_Control_Var.Ctrl.Ready = OFF;
    
    /* Initializes accumulator indexes */
    if ( LPS_Control_Var.bitSamples < LPS_BIT_ACCUMULATORS )
    {
        LPS_Control_Var.curAccIdx = ( (uint8_t) 1u << LPS_Control_Var.bitSamples ) - 1;
        LPS_Control_Var.curSamplesIndex = 1;
    }
    else
    {
        LPS_Control_Var.curAccIdx = ( 1u << LPS_BIT_ACCUMULATORS ) - 1;
        LPS_Control_Var.curSamplesIndex = ( (uint8_t) 1u << ( LPS_Control_Var.bitSamples - LPS_BIT_ACCUMULATORS ) );
    }
    
#endif /* (CONFIG_LPS_DISABLE_MODULE==0) */
    
}

/**
Set Fault flag status ON or OFF; it initializes Timeout timer too.

\param  [in]  newVal New flag value

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/
void PM_LPS_SetFaultFlag ( bool_t newVal )
{
#if (CONFIG_LPS_DISABLE_MODULE==0)

    if ( LPS_Control_Var.Ctrl.InitDone == ON )
    {
        LPS_Control_Var.Ctrl.FaultFlag = (newVal == ON);
        LPS_Control_Var.TimerTimeOut = LPS_Control_Var.Timeout;
    }
    
#endif
}

/**
Linear pressure sensor manager - updates values taken from ISR callback once
they are ready and checks for timeout fault.
It is called in the Main task.

\attention \b LPS_Control_Var used as I/O
\attention \b PSValue is NOT updated - application must call LPS_UpdatePSValue
\attention \b PSDisable used as I - update enabled if value is '0'

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_LPS_Manager ( void )
{

#if (CONFIG_LPS_DISABLE_MODULE==0)

    if ( LPS_Control_Var.Ctrl.InitDone == OFF )
    {
        return;
    }
    
    /* Checks if Linear Pressure is enabled */
    if ( PSDisable == ON )
    {
        /* Reset LPS Status to initial value - when enabled starts from zero */
        PM_LPS_ResetChannel();
        PM_LPS_SetFaultFlag ( OFF );
        
        /* If PSDisable is set, exit function */
        return;
    }
    
    /* If channel was disabled, re-enables it */
    if ( LPS_Control_Var.Ctrl.Enabled == OFF )
    {
        PM_LPS_SetChannelStatus ( ON );
    }
    
    
    /* Check If ISR callback completed samples collecting */
    if ( LPS_Control_Var.Ctrl.Ready == OFF )
    {
#if 0
        /* Check if already in fault condition */
        if ( LPS_Control_Var.Ctrl.FaultFlag == ON )
        {
            /* If in fault and no new sample available, exit function */
            return;
        }
#endif
        
        /* Timeout = 0 disables timeout */
        if ( LPS_Control_Var.Timeout )
        {
            /* Check if timeout occured */
            if ( LPS_Control_Var.TimerTimeOut > 0 )
            {
                /* Update timeout counter */
                LPS_Control_Var.TimerTimeOut--;
            }
            else
            {
                /* Reset all channel average and go in fault condition */
                PM_LPS_ResetChannel();
                PM_LPS_SetFaultFlag ( ON );
                PM_LPS_SetChannelStatus ( ON );
            }
        }
        
        /* If value not yet ready, exit function */
        return;
    }
    
    
    /* Reset samples index for next collect */
    if ( LPS_Control_Var.bitSamples < LPS_BIT_ACCUMULATORS )
    {
        LPS_Control_Var.curSamplesIndex = 1;
    }
    else
    {
        LPS_Control_Var.curSamplesIndex = (uint8_t) 1u << ( LPS_Control_Var.bitSamples - LPS_BIT_ACCUMULATORS );
    }
    
    /* Update accumulator index for next collect - if round completed starts again */
    if ( LPS_Control_Var.curAccIdx == 0 )
    {
        if ( LPS_Control_Var.bitSamples < LPS_BIT_ACCUMULATORS )
        {
            LPS_Control_Var.curAccIdx = ( (uint8_t) 1u << LPS_Control_Var.bitSamples );
        }
        else
        {
            LPS_Control_Var.curAccIdx = ( 1u << LPS_BIT_ACCUMULATORS );
        }
        
        LPS_Control_Var.Ctrl.FirstTimeDone = ON;
        LPS_Control_Var.Ctrl.Started = ON;
    }
    
    LPS_Control_Var.curAccIdx--;
    
    /* Algorithm aleady running - updates immediately, after completing an accumulator */
    if ( LPS_Control_Var.Ctrl.Started )
    {
        uint8_t i;
        LPS_AccuType tmpAcc = 0;
        
        for ( i= ( 1u << LPS_BIT_ACCUMULATORS ); i > 0 ; )
        {
            i--;
#if (LPS_DIVIDE_ACCUMULATOR_AFTER_SUM == ON)
            tmpAcc += LPS_Control_Var.LPS_Acc[i];
#else
            tmpAcc += ( LPS_Control_Var.LPS_Acc[i] >> LPS_Control_Var.bitSamples );
#endif
        }
        
        /* Mean value update - it is the base for PSValue update when requested */
#if (LPS_DIVIDE_ACCUMULATOR_AFTER_SUM == ON)
        tmpAcc = ( tmpAcc >> LPS_Control_Var.bitSamples );
#endif

        tmpAcc = POSTCONV_LPS_VALUE ( tmpAcc );

#if defined(CONFIG_LPS_VALUE_16BIT)

        LPS_Control_Var.Ctrl.OutOfRange = ( tmpAcc > 0xFFFF );

        if (!LPS_Control_Var.Ctrl.OutOfRange)
        {
            PSValue = ( LPS_ValueType ) tmpAcc;
        }

#else

        PSValue = ( LPS_ValueType ) tmpAcc;

#endif

        /* Notify a new update available to application */
        LPS_Control_Var.Ctrl.MeanUpdated = ON;
        
    }
    
    /* Reset next accumulator */
    LPS_Control_Var.LPS_Acc[LPS_Control_Var.curAccIdx] = 0;
    
    /* Restart data collecting */
    LPS_Control_Var.Ctrl.Ready = OFF;
    
    /* Reset any fault / Timeout conditions */
    PM_LPS_SetFaultFlag ( OFF );
    
#endif /* (CONFIG_LPS_DISABLE_MODULE==0) */
    
}


/**
Returns linear pressure fault state.

\attention \b LPS_Control_Var used as I

\return     bool_t

\retval     0 no fault
\retval     !=0  fault

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

bool_t PM_LPS_GetFaultFlag ( void )
{

#if (CONFIG_LPS_DISABLE_MODULE==0)

    return LPS_Control_Var.Ctrl.FaultFlag;
    
#else
    return 0;
    
#endif
    
}

/**
Reset MeanUpdate flag to allow trigger next PSValue update

\attention \b LPS_Control_Var used as O

\return     none

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

void PM_LPS_WaitNextUpdate ( void )
{

#if (CONFIG_LPS_DISABLE_MODULE==0)
    LPS_Control_Var.Ctrl.MeanUpdated = OFF;
#endif

}

/**
Returns linear pressure running state; this mean
1) module has been initialized
2) first sample has been generated OR a fault occurred

\attention \b LPS_Control_Var used as I

\return     bool_t

\retval     0 not yet ready to run
\retval     !=0  ready to run

\author     Alessandro Sabbatini
\date       09/10/2012
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

bool_t PM_LPS_IsRunning ( void )
{

#if (CONFIG_LPS_DISABLE_MODULE==0)

    return (LPS_Control_Var.Ctrl.InitDone && LPS_Control_Var.Ctrl.FirstTimeDone);

#else
    return 0;

#endif

}


/**
Get LPS Status

\attention \b LPS_Control_Var used as I

\return     uint8_t

\retval     LPS_UPD_OK  update succeeded
\retval     LPS_UPD_NOTREADY update failed - not ready
\retval     LPS_ERR_OUTOFRANGE update failed - average is out of range
\retval     LPS_ERR_FAULT update failed - fault occurred

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

LPS_Update_Result PM_LPS_GetStatus ( void )
{
    LPS_Update_Result retVal = LPS_UPD_OK;

#if (CONFIG_LPS_DISABLE_MODULE==0)
    
    if ( PSDisable == OFF )
    {
        if ( LPS_Control_Var.Ctrl.InitDone == OFF )
        {
        	retVal = LPS_ERR_NOT_INIT;
        }
        else if ( LPS_Control_Var.Ctrl.FaultFlag == ON )
		{
			retVal = LPS_ERR_FAULT;
		}
		else if ( LPS_Control_Var.Ctrl.OutOfRange == ON )
        {
            retVal = LPS_ERR_OUTOFRANGE;
        }
		else if ( ( LPS_Control_Var.Ctrl.Started == OFF ) ||
			( LPS_Control_Var.Ctrl.MeanUpdated == OFF ) )
		{
			retVal = LPS_UPD_NOTREADY;
		}
		else
		{
			/* Do nothing */
		}
    }
    
#endif
    
    return retVal;
    
}

/**
Provide PSValue variable

\return     LPS_ValueType

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/

LPS_ValueType PM_LPS_GetPSValue ( void )
{
    return PSValue;
}

/* Local Function */

/**
OCPWD Callback to update period time

\param  [in]    notifVal Value of notification (IO_N_COMPLETED if all is OK)
\param  [in]    channel timer channel used (it should be same of LPS)
\param  [in]    error Error notification (IO_E_OK if all is OK)

\attention Parameters aligned on standard callbacks
\attention \b LPS_Control_Var used as I/O
\attention \b ModuleIOmap used as I

\author     Alessandro Sabbatini
\date       23/08/2011
\version    00.00.01

\note       \par \b Note 1:
            documentation by Alessandro Sabbatini

*/


static IO_ErrorType PM_LPS_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error )
{
    IO_PWD_MeasureType tmpMeasure;
    LPS_AccuType tmpPeriod;
    
    /* Check if notification status is good */
    if ( ( notifVal != IO_N_COMPLETED ) || ( error != IO_E_OK ) )
    {
        return IO_E_UNKNOWN_MODE;
    }
    
    /* IF LPS Update is disabled, do nothing */
    if ( LPS_Control_Var.Ctrl.InitDone == OFF )
    {
        return IO_E_INVALID_CHANNEL_ID;
    }
    
    /* If PDSisable is OFF, tell everything goes OK */
    if ( PSDisable == ON )
    {
        return IO_E_OK;
    }
    
    /* Skip if it called before LPS_Manager processed last sample get
       Channel has to be enabled at this time!  */
    if ( ( LPS_Control_Var.Ctrl.Enabled == OFF ) || ( LPS_Control_Var.Ctrl.Ready == ON ) )
    {
        return IO_E_WORKING;
    }
    
    /* Get last measure from OCPWD driver - all in nanoseconds! */
    OCPWD_GetAllSync( channel, &tmpMeasure );

    
#if (LPS_ACCU_32BIT != TRUE)
    
    /* Check if new period sample fits into 16 bit accumulator */
    if ( PRECONV_LPS_VALUE ( tmpMeasure.period ) > 0xFFFF )
    {
        /* Ouch, it's too high! Gives an error */
        tmpMeasure.period = 0;
        return IO_E_FAIL;
    }
    
#endif
    
    /* Pre-conversion of period sample */
    tmpPeriod = ( LPS_AccuType ) PRECONV_LPS_VALUE(tmpMeasure.period);

    /* Check if period is in range */
    if ( ( tmpPeriod >= LPS_Control_Var.MinValue ) && ( tmpPeriod <= LPS_Control_Var.MaxValue ) )
    {
        /* Update current accumulator with noew value */
        LPS_Control_Var.LPS_Acc[LPS_Control_Var.curAccIdx] += tmpPeriod;
        LPS_Control_Var.curSamplesIndex--;
        
        /* All samples are got - data ready ! */
        if (  LPS_Control_Var.curSamplesIndex == 0 )
        {
            LPS_Control_Var.Ctrl.Ready = ON;
#if (LPS_SUSPEND_CHANNEL_AFTER_DATA_READY == ON)
            /* Suspends channel synchronization after data ready */
            PM_LPS_SetChannelStatus ( OFF );
#endif
            
        }
    }
    
    return IO_E_OK;
}


/* *********** */
/* End of file */
/* *********** */
