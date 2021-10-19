/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		PM_PulseCounter
\n
\file		PM_PulseCounter.c
\ingroup	PM_PulseCounter
\date		24/10/2013
\author		Salvatore Fimiani
\n
*/

/* ******** */
/* Includes */
/* ******** */


#include "Config.h"
#if defined (CONFIG_PM_PULSECOUNTER)
#include "PM_PulseCounter.h"

#include "OCTPM_drv.h"

#include "OCDIO_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define PCNT_CH_RISING_EDGE	0



    
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
 *\n Structure with configuration and control data
 *\n for the Pulse Counter
 */
typedef struct {

	/**  Configuration info for the PWD channel used as a pulse counter */
	IO_PWD_CfgType   pwdCfg;

	struct {

		/** If '1' the initialization was done */
		uint8_t initDone :1;
		/** If '1' the module is currently active */
		uint8_t Enabled	:1;
	    /** If '1' the measured frequency is too high */
	    uint8_t pulseCounterWarningFreqTooHigh      :1;
	    /** If '1' the measured frequency is too low */
	    uint8_t pulseCounterWarningFreqTooLow       :1;
	    /** Reserved for future uses */
	    uint8_t freeBit :4;

	} Ctrl;

	/** Max permitted period normalized in nsec */
    uint32_t maxTnorm;
    /** Min permitted period normalized in nsec */
    uint32_t minTnorm;
    /** Period value normalized in nsec such that if corrTnorm <= Tmeas <= maxTnorm must be applied a correction  */
    uint32_t corrTnorm;

    /** Correction factor */
    uint8_t corrFactor;			
    /** Pin linked to the Input Capture channel */
    PulseCounterPin_t pin; 
    /** Counter for counting the times number the frequency is too high */
    uint8_t highFreqCntr;       
    /** Counter for counting the times number the frequency is too low */
    uint8_t lowFreqCntr;
    /** Counter for counting the times number the frequency is in the compensation range */
    uint8_t pulseCorrRangeCntr;


}PulseCounterStruct_t;


/* **************************** */
/* Prototype of local functions */
/* **************************** */

/**
 * Callback to be called by ISR of the PWD.
 * It updates the number of counted pulses
 */
static IO_ErrorType PM_PulseCounter_PWDCallback(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);



/* **************************** */
/* External functions           */
/* **************************** */



/* ****************** */
/* Exported variables */
/* ****************** */
extern const IO_ModuleIOMapType ModuleIOmap[];


/** Number of the counted pulses  */
uint16_t PulseCount = 0;

/* **************** */
/* Global variables */
/* **************** */

/**
 * Variable of configuration and control
 */
static PulseCounterStruct_t PulseCounterVars;


/* ***************** */
/* Exported function */
/* ***************** */

/**
\n This function provides the initialization of the PulseCounter module.
\n So, it has to be called by the InputManger module.
\n The input data are:

\n   | BYTE HIGH  			|   BYTE LOW  		 |
\n   | corrFactor 			| 	unused		  	 |
\n   | 			Max Period Value 	[msec]	  	 |
\n   |			Min Period Value 	[msec]	 	 |
\n   | 	Compensation Period Value 	[msec] 		 |
     
\param  [in]		pin Input Capture pin
\param  [in]		pDataPulseCounter pointer to the module configuration data


\attention \b PulseCounterVars used as I/O

\return 	PulseCounterError_t

\retval		PULSECOUNTER_E_OK \n
            PULSECOUNTER_E_KO 	 
*/
PulseCounterError_t PM_PulseCounter_Init(PulseCounterPin_t pin, pPulseCounterData_t pDataPulseCounter)
{

	PulseCounterError_t error = PULSECOUNTER_E_OK;

	IO_ChannelType tmpChannel;

	(void)memset(&PulseCounterVars, 0, sizeof(PulseCounterVars));
    /* richiede il canale del timer associato al pin */
	if ( OCTPM_GetChannelSync ( &ModuleIOmap[pin], &tmpChannel ) == ( IO_ErrorType ) IO_E_OK )
	{
		PulseCounterVars.pwdCfg.PWD_Id = tmpChannel;
		PulseCounterVars.pwdCfg.inverted = PCNT_CH_RISING_EDGE;
	}
	else
	{
		PulseCounterVars.pwdCfg.PWD_Id = 0xFF;
	}


    if ( ( PulseCounterVars.pwdCfg.PWD_Id == 0xFF ) || ( pDataPulseCounter == NULL) )
    {
        error = PULSECOUNTER_E_KO;    
    }
    else
    {   
    	/* Configuration of the PWD channel */
    	if ( OCPWD_InitSync ( &PulseCounterVars.pwdCfg ) != ( IO_ErrorType ) IO_E_OK )
		{
    		PulseCounterVars.pwdCfg.PWD_Id = 0xFF;
    		error = PULSECOUNTER_E_KO;
		}
    	else
    	{
    		PulseCounterVars.pin = pin;

    		PulseCounterVars.corrFactor = *((uint8_t*)pDataPulseCounter+8);

			/* Max Period expressed in nsec -> Min frequency */
			PulseCounterVars.maxTnorm = (uint32_t)(*((uint16_t*)pDataPulseCounter+1)*1000000UL);
			/* Min Period expressed in nsec -> Max frequency */
			PulseCounterVars.minTnorm = (uint32_t)(*((uint16_t*)pDataPulseCounter+2)*1000000UL);
			/* Period to correct the error -> Min frequency */
			PulseCounterVars.corrTnorm = (uint32_t)(*((uint16_t*)pDataPulseCounter+3)*1000000UL);

			/* At this point the module initialization is done */
			PulseCounterVars.Ctrl.initDone = TRUE;

			PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);

			PM_PulseCounter_SetChannelStatus(PULSECOUNTER_DISABLE);
    	}

    }
    return error;
}



/**
Enable/disable the Pulse Counter Module
(Enabled by default)

\param  [in] status 1: enabled / 0: disabled

\attention \b PulseCounterVars used as I/O

*/
void PM_PulseCounter_SetChannelStatus ( uint8_t status)
{
	IO_ErrorType res;

	/* Do things only if the module is already initialized else do nothing */
    if ( PulseCounterVars.Ctrl.initDone == TRUE )
    {
		if(status == PULSECOUNTER_ENABLE)
		{
			/* Enable it only if the module is not yet enabled */
			if(PulseCounterVars.Ctrl.Enabled == FALSE)
			{
				res = OCPWD_StartSampleSync (PulseCounterVars.pwdCfg.PWD_Id, PM_PulseCounter_PWDCallback );
				if(res == IO_E_OK)
				{
					PulseCounterVars.Ctrl.Enabled = TRUE;
				}
			}
		}
		else
		{
			(void)OCPWD_StopSampleSync(PulseCounterVars.pwdCfg.PWD_Id);
		}
    }
}





/**
Reset warnings

\param  [in]		whichWarning PULSECOUNTER_RESET_LOW_WARNING: 	reset low frequency warning  \n
								 PULSECOUNTER_RESET_HIGH_WARNING: 	reset high frequency warning \n
								 PULSECOUNTER_RESET_BOTH_WARNING:	reset both frequency worning \n

\attention \b PulseCounterVars used as I/O

*/    	
void PM_PulseCounter_Reset_Warning( uint8_t whichWarning )
{
    switch ( whichWarning )
    {
        case PULSECOUNTER_RESET_LOW_WARNING:
            PulseCounterVars.lowFreqCntr = 0;
            PulseCounterVars.Ctrl.pulseCounterWarningFreqTooLow = FALSE;
            break;
        case PULSECOUNTER_RESET_HIGH_WARNING:
            PulseCounterVars.highFreqCntr = 0;
            PulseCounterVars.Ctrl.pulseCounterWarningFreqTooHigh = FALSE;
            break;
        case PULSECOUNTER_RESET_BOTH_WARNING:
            PulseCounterVars.highFreqCntr = 0;
            PulseCounterVars.Ctrl.pulseCounterWarningFreqTooHigh = FALSE;
            PulseCounterVars.lowFreqCntr = 0;
            PulseCounterVars.Ctrl.pulseCounterWarningFreqTooLow = FALSE;
            break;
        default:
            break;
    }
}
/**
Returns the current warnings

\attention \b PulseCounterVars used

\return 	uint8_t

\retval		0 -> No warnings 
            1 -> High Frequency warning 
            2 -> Low Frequency warning
            3 -> High and Low Frequency warning 


*/
uint8_t PM_PulseCounter_GetWarning( void )
{
uint8_t pulseCounterWarnings = 0;

    if ( PulseCounterVars.Ctrl.pulseCounterWarningFreqTooHigh )
    {
        pulseCounterWarnings |= BIT0_MASK;    
    }
    if ( PulseCounterVars.Ctrl.pulseCounterWarningFreqTooLow )
    {
        pulseCounterWarnings |= BIT1_MASK;            
    }
    
    return ( pulseCounterWarnings );
	
}

/**
Sets the PulseCount variable to the value of the input parameter

\param  [in]		newPulseCount  value for PulseCount

\attention \b PulseCount used as I/O


*/
void PM_PulseCounter_SetCount( uint16_t newPulseCount )
{
	PulseCount = newPulseCount;
}

/**
Returns the PulseCount value

\attention \b PulseCount used as I/O

\return 	uint16_t

*/
uint16_t PM_PulseCounter_GetCount( void )
{
uint16_t copyOfPulseCount;

	EnterCriticalSection();
	copyOfPulseCount = PulseCount;
	ExitCriticalSection();

    return (copyOfPulseCount);

}


/* ************** */
/* Local Function */
/* ************** */



static IO_ErrorType PM_PulseCounter_PWDCallback(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error )
{
	 IO_PWD_MeasureType tmpMeasure;
	 IO_ErrorType res = IO_E_OK;

	/* Check if the notification status is good */
	if ( ( notifVal != IO_N_COMPLETED ) || ( error != IO_E_OK ) )
	{
		res = IO_E_UNKNOWN_MODE;
	}
	else if ( PulseCounterVars.Ctrl.initDone == FALSE )
	{
		res = IO_E_INVALID_CHANNEL_ID;
	}
	else
	{
		/* Get the PWD Data Period and Ton in nsec */
		OCPWD_GetAllSync( channel, &tmpMeasure );

		if(tmpMeasure.period < PulseCounterVars.minTnorm)
		{/* The measured frequency is too high */
			if(PulseCounterVars.highFreqCntr < PULSECOUNTER_FREQ_TOO_HIGH_FILTER)
			{
				PulseCounterVars.highFreqCntr++;
			}
			else
			{
				PulseCounterVars.Ctrl.pulseCounterWarningFreqTooHigh = TRUE;
			}
		}
	    else if(tmpMeasure.period > PulseCounterVars.maxTnorm)
	    {/* The measured frequency is too low */
			if(PulseCounterVars.lowFreqCntr < PULSECOUNTER_FREQ_TOO_LOW_FILTER)
			{
			   PulseCounterVars.lowFreqCntr++;
			}
			else
			{
			   PulseCounterVars.Ctrl.pulseCounterWarningFreqTooLow = TRUE;
			}
	    }
	    else
	    {
			PulseCount++;

			/* As soon as the frequency is in the correct range, the error counters are reset */
			PulseCounterVars.highFreqCntr = 0;
			PulseCounterVars.lowFreqCntr = 0;

			/* Checks if it is necessary to correct the PulseCount value */
			if(tmpMeasure.period >= PulseCounterVars.corrTnorm)
			{
			   PulseCounterVars.pulseCorrRangeCntr++;
			   if(PulseCounterVars.pulseCorrRangeCntr >= PulseCounterVars.corrFactor)
			   {
				   PulseCounterVars.pulseCorrRangeCntr = 0;
				   PulseCount++;
			   }
			}
			else
			{
			   PulseCounterVars.pulseCorrRangeCntr = 0;
			}
	    }
	}

	return res;
}

#endif  /*defined(CONFIG_PM_PULSECOUNTER)*/

/* *********** */
/* End of file */
/* *********** */
