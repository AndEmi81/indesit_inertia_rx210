/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	LPM 
@{
\par		On Chip LPM Driver
\n			The OCLPM driver offers functions to put the processor into the low power mode with
\n          possible source of wake-up. The sources are:
\n\li		I2C match address. The CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT define must be defined.
\n\li		UART 00 A5 sequence where the 00 byte is corrupted so A5 is received only.
\n			The CONFIG_OCLPM_DRV_UART_TO_CANCEL_LP_MODE_SUPPORT define must be defined.
\n\li 		Digital pin wake-up. The CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT define must be defined.
\n\li		Periodical wake-up followed by ADC or digital pin reading. The _HAL_LPM_PERIOD define must be defined.
\n
\n          The driver reuses the I2C, UART, DIO and ADC drivers' functions and ISR's.
\n 
@}
\ingroup   	Drivers
*/
/**
\n
\par		OCLPM_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCLPM_drv.c 
\ingroup	LPM
\date		July 02, 2012 
\version	01.01.01 
\author		Leonardo Sparti
\n
\n
\par\b	History:
\n
\n 		01.00.00 
\n		17/09/2010 
\n		Jaroslav Musil
\n		- Creation
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */
#include "OCLPM_drv.h"
#include HAL


#if defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
#include "OCI2C_drv.h"
#endif

#if defined(CONFIG_OCLPM_DRV_UART_TO_CANCEL_LP_MODE_SUPPORT)
#include "OCUART_drv.h"
#endif

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT) || defined(_HAL_LPM_PERIOD)
#include "OCDIO_drv.h"
#define LPM_WAKEUP_IO_ENABLE()
#define LPM_WAKEUP_IO_DISABLE()
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)

OCLPM_WakeUpPinType OCLPM_WakeUpPin = {
		CONFIG_LPM_WAKEUP_PIN,
		0, /* basso per il wakup */
		CONFIG_LPM_WAKEUP_FILTER,
};
#endif

#if defined(_HAL_LPM_PERIOD)
OCLPM_WakeUpPeriodType OCLPM_WakeUpPeriod;
#endif



/* Static */
static uint8_t leaveLPM = 0;

#if defined(_HAL_LPM_PERIOD)
static ADC_ValueType centerADCValue;
#endif


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
/*static IO_ErrorType OCLPM_Callback(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error);*/

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n This function initializes the LPM driver.

\return 	IO_ErrorType

\retval		IO_E_OK initialization done		 	 		

\author 	Leonardo Sparti 
\date		Jul 5, 2012 
\version	1.0.1
   
\note		

*/
IO_ErrorType OCLPM_LowPowerModeInit(void)
{
    LPM_SleepModeInit();

    return (IO_ErrorType)IO_E_OK;
}


/**
\n This function manages the low power mode defined by the wakeupCriteria bitmap.
\n If the processor is switched into the low power mode, the input parameter determines
\n which criteria will wake it up.

\param  [in]	wakeupCriteria Peripherals to wake up the processor.
                               LPM_CRITERIA_I2C - I2C wakeup bit
                               LPM_CRITERIA_UART - UART wakeup bit
                               LPM_CRITERIA_DIO - pin wakeup bit
                               LPM_CRITERIA_PERIOD - periodic wakeup bit
                               

\return 	IO_ErrorType

\retval		IO_E_OK processor waken up
    		IO_E_INVALID_VALUE unapplicable criteria for the current platform 	 		

\author 	Leonardo Sparti 
\date		Jul 5, 2012 
\version	1.0.1
   
\note		

*/
IO_ErrorType OCLPM_GoLowPowerMode(uint16_t wakeupCriteria)
{
#if defined(_HAL_LPM_PERIOD)
    ADC_SumType adcSum;
    uint8_t i;
    bool_t centerPointMeasured = 0;
    uint8_t adcChannel;
#endif

    leaveLPM = 1;

    do
    {
        /* All peripherals clock off */
        LPM_AllClockOff();
        
#if defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
        if ((wakeupCriteria & LPM_CRITERIA_I2C) != 0)
        {
        	I2C_ClkEnable();

            /* Low power with wake-up */
            (void)OCI2C_LowPowerMode(OCLPM_Callback);
            
            leaveLPM = 0;    
        }
#else
        if ((wakeupCriteria & LPM_CRITERIA_I2C) != 0)
        {
            return (IO_ErrorType)IO_E_INVALID_VALUE;
        }
#endif

#if defined(CONFIG_OCLPM_DRV_UART_TO_CANCEL_LP_MODE_SUPPORT)
        if ((wakeupCriteria & LPM_CRITERIA_UART) != 0)
        {
            UART_ClockOn(HAL_UART_DEV);    	

            /* Low power with wake-up */
            (void)OCUART_LowPowerMode(OCLPM_Callback);    

            leaveLPM = 0;
        }
#else
        if ((wakeupCriteria & LPM_CRITERIA_UART) != 0)
        {
            return (IO_ErrorType)IO_E_INVALID_VALUE;
        }
#endif

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
        if ((wakeupCriteria & LPM_CRITERIA_DIO) != 0)
        {
            /* Low power with wake-up */
        	LPM_WakupIoEnable(OCLPM_WakeUpPin.modulePin, 1);
        
            leaveLPM = 0;
        }
#else
        if ((wakeupCriteria & LPM_CRITERIA_DIO) != 0)
        {
            return (IO_ErrorType)IO_E_INVALID_VALUE;
        }
#endif

#if defined(_HAL_LPM_PERIOD)
        if ((wakeupCriteria & LPM_CRITERIA_PERIOD) != 0)
        {
            /* if it is a mode where the center point is measured */
            if ((OCLPM_WakeUpPeriod.type & LPM_WAKE_UP_PERIOD_DELTA_MASK) == (LPM_WAKE_UP_PERIOD_TYPE_ANDELTA << LPM_WAKE_UP_PERIOD_TYPE_SHIFT))
            {
                /* the center point hasn't been measured yet */
                if (!centerPointMeasured)
                {
                    /* Clock on ADC */
                    ADC_ClockGateOn();

                    /* ADC Init */
                    ADC_Init();
                            
                    /* ADC channel mapped by the pin number */
                    if (OCADC_GetADchannelSync(&ModuleIOmap[OCLPM_WakeUpPeriod.modulePin & LPM_WAKE_UP_PIN_MODULE_MASK], &adcChannel) == (IO_ErrorType)IO_E_OK)
                    {
                        adcSum = 0;
                        
                        /* Sumes 8 samples */
                        for (i = 0; i < 8; i++)
                        {
                            if (OCADC_SampleSync(adcChannel, (IO_PositionType)0) == (IO_ErrorType)IO_E_OK)
                            {
                                adcSum += OCADC_GetSync((IO_PositionType)0);
                            }
                            else
                            {
                                leaveLPM = 0;
                                    
                                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                            }
                        }
                    
                        /* Avg. value */
                        centerADCValue = (ADC_ValueType)(adcSum >> 3);
                        
                        /* The center point already measured */
                        centerPointMeasured = TRUE;
                    }
                }
            }
            
            
            /* Sets the sleep timer with a sleep period of ms * 10 */
            LPM_SetWakeupTimer(OCLPM_WakeUpPeriod.sleepTime);
            
            leaveLPM = 0;
        }
#else
        if ((wakeupCriteria & LPM_CRITERIA_PERIOD) != 0)
        {
            return (IO_ErrorType)IO_E_INVALID_VALUE;
        }
#endif
        /* Sleep mode procedure */
        LPM_SleepModeProc();
    } while (leaveLPM == 0);

#if defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
    if ((wakeupCriteria & LPM_CRITERIA_I2C) != 0)
    {
        OCI2C_ResetLowPowerMode();
    }
#endif

#if defined(CONFIG_OCLPM_DRV_UART_TO_CANCEL_LP_MODE_SUPPORT)
    if ((wakeupCriteria & LPM_CRITERIA_UART) != 0)
    {
        OCUART_ResetLowPowerMode();
    }
#endif

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
    if ((wakeupCriteria & LPM_CRITERIA_DIO) != 0)
    {
    	LPM_WakupIoEnable(OCLPM_WakeUpPin.modulePin, 0);
    }
#endif

    return (IO_ErrorType)IO_E_OK;
}


/**
\n This function is the callback when a peripheral wakes the processor. The data are transferred via
\n the input parameters. The return value determines if the wake-up is correct.

\param  [in]	notification Transmitted value from the peripheral
\param  [in]	channel      Wake-up source
\param  [in]	error        Unused
                               

\return 	IO_ErrorType

\retval		IO_E_OK Processor wake-up condition occurred
    		IO_E_UNEXPECTED_CONDITION The condition is not to wake up		

\author 	Jaroslav Musil 
\date		Sep 16, 2010 
\version	1.0.0
   
\note		

*/
/*static*/ IO_ErrorType OCLPM_Callback(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error)
{
#if defined (_HAL_LPM_PERIOD)
    bool_t expectedPinValue;
    uint8_t wakeUpPin;
    uint8_t adcChannel;
    ADC_ValueType adcSample;
    ADC_ValueType adcRangeMax, adcRangeMin;
#endif

    (void)error;
    (void)notification;
    (void)channel;

#if defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
    /* if I2C wake-up */
    if (channel == LPM_ID_I2C)
    {
        /* Flag to leave the low power mode */
        leaveLPM = 1;    
    
        return (IO_ErrorType)IO_E_OK;
    }
#endif
    
#if defined(CONFIG_OCLPM_DRV_UART_TO_CANCEL_LP_MODE_SUPPORT)
    /* if I2C wake-up */
    if (channel == LPM_ID_UART)
    {
        /* checks if the received byte is A5 */
        if (notification == 0xA5)
        {
            /* Flag to leave the low power mode */
            leaveLPM = 1;    
        
            return (IO_ErrorType)IO_E_OK;
        }
        else
        {
            return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
        }
    }
#endif

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
    /* if pin wake-up */
    if (channel == LPM_ID_DIO)
    {
        /* filter timer on */
        int filterCnt = OCLPM_WakeUpPin.timeFilter;

        while (filterCnt > 0)
        {
            /* if the pin doesn't keep the expected value */
            if (OCDIO_GetPinValueSync(OCLPM_WakeUpPin.modulePin) != OCLPM_WakeUpPin.expectedPinValue)
            {
                /* Flag to reenter the low power mode */
                leaveLPM = 0;
                
                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
            }
            filterCnt--;
        }

        /* Flag to leave the low power mode */
        leaveLPM = 1;
        
        return (IO_ErrorType)IO_E_OK;
    }
#endif


#if defined(_HAL_LPM_PERIOD)
    /* if period wake-up */
    if (channel == LPM_ID_PERIOD)    
    {
        /* if analog reading with delta */
        if ((OCLPM_WakeUpPeriod.type & LPM_WAKE_UP_PERIOD_DELTA_MASK) == (LPM_WAKE_UP_PERIOD_TYPE_ANDELTA << LPM_WAKE_UP_PERIOD_TYPE_SHIFT))
        {
            /* ADC clock on */
            ADC_ClockGateOn();

            /* ADC init */
            ADC_Init();
                    
            /* Gets the adc channel based on the pins table */
            if (OCADC_GetADchannelSync(OCDIO_GetIOMapItem(OCLPM_WakeUpPeriod.modulePin & LPM_WAKE_UP_PIN_MODULE_MASK), &adcChannel) == (IO_ErrorType)IO_E_OK)
            {
                /* filter timer on */
                LPM_SetFilterTimer(OCLPM_WakeUpPeriod.timeFilter);

                /* upper frontier */
                adcRangeMax = (ADC_ValueType)(centerADCValue + (OCLPM_WakeUpPeriod.adcDelta & LPM_WAKE_UP_PERIOD_DELTA_MASK));
                
                /* lower frontier */
                adcRangeMin = (ADC_ValueType)(centerADCValue - (OCLPM_WakeUpPeriod.adcDelta & LPM_WAKE_UP_PERIOD_DELTA_MASK));

                while (!LPM_IsFilterTimerExpired())
                {
                    /* Sample channel */
                    if (OCADC_SampleSync(adcChannel, 0) == (IO_ErrorType)IO_E_OK)
                    {
                        /* Get sample */
                        adcSample = OCADC_GetSync(0);
                        
                        /* if the sample is inside the range */
                        if ((adcSample <= adcRangeMax) && (adcSample >= adcRangeMin))
                        {
                            /* Flag to reenter the low power mode */
                            leaveLPM = 0;
                                
                            return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                        }
                        
                    }
                }

                /* Flag to leave the low power mode */
                leaveLPM = 1;
                
                return (IO_ErrorType)IO_E_OK;
            }
            else
            {
                /* Flag to reenter the low power mode */
                leaveLPM = 0;
                    
                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
            }
            
            
        }
        /* if analog reading with range */
        else if ((OCLPM_WakeUpPeriod.type & LPM_WAKE_UP_PERIOD_DELTA_MASK) == (LPM_WAKE_UP_PERIOD_TYPE_ANRANGE << LPM_WAKE_UP_PERIOD_TYPE_SHIFT)) 
        {
            /* ADC clock on */
            ADC_ClockGateOn();

            /* ADC init */
            ADC_Init();
                    
            /* Gets the adc channel based on the pins table */
            if (OCADC_GetADchannelSync(OCDIO_GetIOMapItem(OCLPM_WakeUpPeriod.modulePin & LPM_WAKE_UP_PIN_MODULE_MASK), &adcChannel) == (IO_ErrorType)IO_E_OK)
            {
                /* filter timer on */
                LPM_SetFilterTimer(OCLPM_WakeUpPeriod.timeFilter);

                /* upper frontier */
                adcRangeMax = (ADC_ValueType)(OCLPM_WakeUpPeriod.adcSetPoint + (OCLPM_WakeUpPeriod.adcDelta & LPM_WAKE_UP_PERIOD_DELTA_MASK));

                /* lower frontier */
                adcRangeMin = (ADC_ValueType)(OCLPM_WakeUpPeriod.adcSetPoint - (OCLPM_WakeUpPeriod.adcDelta & LPM_WAKE_UP_PERIOD_DELTA_MASK));

                while (!LPM_IsFilterTimerExpired())
                {
                    /* Sample channel */
                    if (OCADC_SampleSync(adcChannel, 0) == (IO_ErrorType)IO_E_OK)
                    {
                        /* Get sample */
                        adcSample = OCADC_GetSync(0);
                        
                        
                        /* Checks the condtion of inside/outside range option */
                        if ((OCLPM_WakeUpPeriod.adcDelta & LPM_WAKE_UP_PERIOD_CONDITION_MASK) == 0)
                        {
                            /* if the sample is outside the range */
                            if ((adcSample > adcRangeMax) || (adcSample < adcRangeMin))
                            {
                                /* Flag to reenter the low power mode */
                                leaveLPM = 0;
                                    
                                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                            }
                        }
                        else
                        {
                            /* if the sample is inside the range */
                            if ((adcSample <= adcRangeMax) && (adcSample >= adcRangeMin))
                            {
                                /* Flag to reenter the low power mode */
                                leaveLPM = 0;
                                    
                                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                            }
                        
                        }
                    }
                    else
                    {
                        /* Flag to reenter the low power mode */
                        leaveLPM = 0;
                            
                        return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                    }
                }

                /* Flag to leave the low power mode */
                leaveLPM = 1;
                
                return (IO_ErrorType)IO_E_OK;
            }
            else
            {
                /* Flag to reenter the low power mode */
                leaveLPM = 0;
                    
                return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
            }
        }
        /* if digital reading */
        else if ((OCLPM_WakeUpPeriod.type & LPM_WAKE_UP_PERIOD_DELTA_MASK) == (LPM_WAKE_UP_PERIOD_TYPE_DIGITAL << LPM_WAKE_UP_PERIOD_TYPE_SHIFT)) 
        {
            /* pin id */
            wakeUpPin = (uint8_t)(OCLPM_WakeUpPeriod.modulePin & LPM_WAKE_UP_PIN_MODULE_MASK);

            /* expected pin value */
            expectedPinValue = (uint8_t)(OCLPM_WakeUpPeriod.modulePin >> 7);
            
            /* filter timer on */
            LPM_SetFilterTimer(OCLPM_WakeUpPeriod.timeFilter);

            while (!LPM_IsFilterTimerExpired())
            {
                /* if the pin doesn't keep the expected value */
                if (OCDIO_GetPinValueSync(wakeUpPin) != expectedPinValue)
                {
                    /* Flag to reenter the low power mode */
                    leaveLPM = 0;
                    
                    return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION; 
                }
            }

            /* Flag to leave the low power mode */
            leaveLPM = 1;
            
            return (IO_ErrorType)IO_E_OK;
        
        }
        else
        {
            return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
        }
    }

#endif

    return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
}

#if defined(CONFIG_OCLPM_DRV_DIO_TO_CANCEL_LP_MODE_SUPPORT)
void OCLPM_WakeUpIsr(void)
{
	if (OCLPM_Callback((IO_NotificationType)0, (IO_ChannelType)LPM_ID_DIO, (IO_ErrorType)IO_E_OK) == IO_E_OK)
	{
        LPM_SleepModeProcExit();
	}
}
#endif

#if defined(_HAL_LPM_PERIOD)
/**
\n This function loads from setting file parameters for periodic WakeUp and analog reading criteria.

\return   IO_ErrorType

\retval   IO_E_OK

\author   Leonardo Sparti 
\date     Jul 5, 2012 
\version  1.0.1
   
\note		

*/
IO_ErrorType OCLPM_SetWakeUpPeriodParam(uint8_t *pStruct)
{
  
  OCLPM_WakeUpPeriod.modulePin =   *(pStruct);
  OCLPM_WakeUpPeriod.timeFilter =  *(++pStruct);
  OCLPM_WakeUpPeriod.type =        *(++pStruct);
  OCLPM_WakeUpPeriod.sleepTime =   *(++pStruct);
  OCLPM_WakeUpPeriod.adcSetPoint = (ADC_ValueType)*(++pStruct);
  OCLPM_WakeUpPeriod.adcDelta =    (ADC_ValueType)*( pStruct + (sizeof(ADC_ValueType)) );
    
  return (IO_ErrorType)IO_E_OK;
  
}



/**
\n Interrupt function of the periodic wake-up. In case of timer interrupt it calls
\n the pre-defined callback to process the wake-up.

\author 	Leonardo Sparti 
\date		Jul 5, 2012 
\version	1.0.1
   
\note		

*/
void OCLPM_PeriodIsr(void)
{
    IO_ErrorType resetCmd;

    /* if timer flag */
    if (LPM_IsTimerExpired())
    {
        /* Disable interrupt of the wake-up timer */
        LPM_WakeupTimerDisableInt();
        
        /* Callback */
        resetCmd = OCLPM_Callback((IO_NotificationType)0, (IO_ChannelType)LPM_ID_PERIOD, (IO_ErrorType)IO_E_OK);
        
        /* Clearing the flag */
        LPM_WakeupTimerClearFlag();
        
        
        if ( resetCmd == (IO_ErrorType)IO_E_OK )
        {
          PM_LPM_Sleep_Reset();
        }
        
    }
}
#endif /* defined(_HAL_LPM_PERIOD) */


/* Exported function */

/* *********** */
/* End of file */
/* *********** */


