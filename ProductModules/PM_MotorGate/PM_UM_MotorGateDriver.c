/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\file    PM_UM_MotorGateDriver.c
\brief   Implementazione del pilotaggio dell motore con parzializzazione di fase.
         Pilota solo il pin di carico e non i rele.
\ingroup PM_MotorGate
\author  Roberto Fioravanti, Marco Sperandini
*/

/**
\addtogroup PM_MotorGate
*@{*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

#ifdef CONFIG_PM_MOTORGATE

#include "PM_UM_MotorGateDriver.h"
#include "PM_UM_UniversalMotor.h"
#include "IO_driver.h"
#include "OCDIO_drv.h"
#include "OCTPM_drv.h"
#include "OCIRQ_drv.h"
#include "LIB_ZeroCrossingExp.h"
#include "LIB_Trace.h"

#include "DigitalOutputs.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define CICLO_SUPERVISORE   	(4)			 /* durata ciclo supervisore controllo motore (x10 msec) */

#define HALF_PERIOD_50HZ_US     (10000)
#define HALF_PERIOD_60HZ_US     (8333)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Hw Counter id 1 Configuration */

typedef struct
{
    uint32_t zcRefCounter; /** in uSec */
    uint32_t zcRefCounterOld;
    uint32_t startImpulseCounter;
    uint16_t pulseLen;
    uint16_t halflinePeriod;
    uint16_t alpha;
    uint16_t alpha_new;
    uint8_t  gatedrv_cnt;

    uint8_t pintTriac;
    unsigned int enabled: 1;
    IO_ModeType mode;
} PM_UM_GateDrvCtrl_t;
/* **************************** */
/* Prototype of local functions */
/* **************************** */
void            PM_UM_GateDrv_SetCompareReg_uSec( uint32_t  val);
static IO_ErrorType PM_UM_GateDrvManagerISR(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);
/* ****************** */
/* Imported variables */
/* ****************** */

extern const TMR_ChMapType TmrChMap[NUM_HW_CNTR_CH];

/* **************** */
/* Global variables */
/* **************** */
static IO_OCU_CfgType      PM_UM_OcuTmrCh_cfg;
static PM_UM_GateDrvCtrl_t PM_UM_GateDrvCtrlVar;
/* ***************** */
/* Exported function */
/* ***************** */

void PM_UM_OnZC(uint32_t cur_measure)
{
	OCOCU_SetTickValByTriggerIndex(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, cur_measure);
}


/**
Procedura configurazione Canale Timer per la gestione del triac controllo motore.
*/
void PM_UM_GateDrv_Init( uint8_t pin )
{
    IO_ErrorType Err;
    IO_ChannelType tmpChannel;

    /* La configurazione HW del timer controllare l'impulso del pin di pilotaggio ??? fatta nella SetupHw() , OCHWCNTR_InitSync(&HwCntr_cfg) */

    PM_UM_GateDrvCtrlVar.pintTriac = pin;
    PM_UM_GateDrvCtrlVar.enabled = 0;
    
    if (OCTPM_GetChannelSync(OCDIO_GetIOMapItem(PM_UM_GateDrvCtrlVar.pintTriac), &tmpChannel) == (IO_ErrorType) IO_E_OK )
    {
        PM_UM_OcuTmrCh_cfg.OCU_Id = tmpChannel;
        PM_UM_OcuTmrCh_cfg.inverted  = 0; /* not inverted */

        /* Output Compare Init. for */
        Err = OCOCU_InitSync(&PM_UM_OcuTmrCh_cfg);
        PLT_ASSERT(Err == IO_E_OK);

        /* Toggle Pin On Compare */
        Err = OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_OC_OFF);
        PLT_ASSERT(Err == IO_E_OK);

        Err = OCOCU_EnableISR(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_ISR_DISABLE, NULL);
        PLT_ASSERT(Err == IO_E_OK);

        (void) Err;
    }
    PLT_ASSERT(PM_UM_OcuTmrCh_cfg.OCU_Id != 0xFF);

    if( LIB_ZC_GetNominalFrequency() == NOM_FREQ_IS_50_HZ )
    {
    	PM_UM_GateDrvCtrlVar.halflinePeriod = HALF_PERIOD_50HZ_US; /* 10 ms in uSec */
    }else
    {
    	PM_UM_GateDrvCtrlVar.halflinePeriod = HALF_PERIOD_60HZ_US; /* 8.333 ms in uSec */
    }
    
}

void PM_UM_GateDrv_SetImpulseLen( uint16_t val )
{
    PM_UM_GateDrvCtrlVar.pulseLen = val;
}

void PM_UM_GateDrv_SetAlpha(uint16_t alpha_n, bool8_t bHigh)
{

	uint32_t	u32nsDelay;

	PM_UM_GateDrvCtrlVar.halflinePeriod = LIB_ZC_GetHalfWaveLengthPos();
    
    if( alpha_n > PM_UM_GateDrvCtrlVar.halflinePeriod ) 
    {
        alpha_n = PM_UM_GateDrvCtrlVar.halflinePeriod;
    }


    PM_UM_GateDrvCtrlVar.alpha_new = alpha_n;

    /* init alpha is motor is not driven */
    if( PM_UM_GateDrvCtrlVar.alpha == 0 )
    {
        PM_UM_GateDrvCtrlVar.alpha = alpha_n;
    }
        

    if( !PM_UM_GateDrvCtrlVar.enabled && PM_UM_GateDrvCtrlVar.alpha_new)
    {
    	u32nsDelay=2*PM_UM_GateDrvCtrlVar.halflinePeriod;

    	if( LIB_ZC_GetCurrentEdge() == VPEAK_EDGE )
    	{
    		u32nsDelay += PM_UM_GateDrvCtrlVar.halflinePeriod;
    	}

        PM_UM_GateDrvCtrlVar.gatedrv_cnt=CICLO_SUPERVISORE*2-2;
        PM_UM_GateDrvCtrlVar.enabled = 1;

        OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_OC_PIN_LOW);
        OCOCU_SetEventSync(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, (PM_UM_GateDrvCtrlVar.alpha_new+u32nsDelay)*1000UL);
        OCOCU_EnableISR(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_ISR_ENABLE, &PM_UM_GateDrvManagerISR);

        PM_UM_GateDrvCtrlVar.zcRefCounter = OCOCU_GetTickValByTriggerIndex(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC);
        PM_UM_GateDrvCtrlVar.zcRefCounterOld = PM_UM_GateDrvCtrlVar.zcRefCounter +1;
    }
    else
    {
    	if((PM_UM_GateDrvCtrlVar.mode == TMR_CH_OC_PIN_HIGH) ^ (bHigh))
    	{
			PM_UM_GateDrvCtrlVar.mode = (bHigh)?TMR_CH_OC_PIN_HIGH:TMR_CH_OC_PIN_LOW;
			/*OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, PM_UM_GateDrvCtrlVar.mode);*/
    	}
    }



}

void PM_UM_GateDrv_SetCfg( bool8_t bHigh)
{
	if((PM_UM_GateDrvCtrlVar.mode == TMR_CH_OC_PIN_HIGH) ^ (bHigh))
	{
		PM_UM_GateDrvCtrlVar.mode = (bHigh)?TMR_CH_OC_PIN_HIGH:TMR_CH_OC_PIN_LOW;
	}
}

Period_ValueType currThreshold;
/** Imposta il valore dell'output compare per l'accensione del triac */
void PM_UM_GateDrv_SetCompareReg_uSec( uint32_t  val)
{
    /* Set Threshold */
    OCOCU_SetThreshold1Sync(PM_UM_OcuTmrCh_cfg.OCU_Id, val * 1000);
    currThreshold = val * 1000;
}

void PM_UM_GateDrv_check(void)
{
	Period_ValueType now;
	TMR_CHAddressType pTmr_ch;
	uint32_t tmrPeriod;

	uint8_t channel = PM_UM_OcuTmrCh_cfg.OCU_Id;
	pTmr_ch = TmrChMap[channel].Channel;

	now = OCOCU_GetSync( channel );

	tmrPeriod = OCPWM_GetPeriodSync( channel );

	if( currThreshold > tmrPeriod){
		currThreshold -=tmrPeriod;
	}

	if( now > tmrPeriod ){
		now -=tmrPeriod;
	}
	if( TMR_ReadCHF(pTmr_ch)){
		TMR_ClearCHF(pTmr_ch);
		PM_UM_GateDrvManagerISR(IO_N_COMPLETED, 0, IO_E_OK);
	}
}


void PM_UM_GateDrv_Enable ( void )
{
    PM_UM_GateDrvCtrlVar.enabled = 1;
    OCOCU_EnableISR(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_ISR_ENABLE, &PM_UM_GateDrvManagerISR);
}

void PM_UM_GateDrv_Disable ( void )
{
    OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_OC_OFF);
    OCOCU_EnableISR(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_ISR_DISABLE, NULL);
    PM_UM_GateDrvCtrlVar.enabled = 0;
    LIB_DIO_SET_LOAD(OFF, PM_UM_GateDrvCtrlVar.pintTriac);
}

uint8_t PM_UM_GateDrvIsEnabled ( void )
{
    return PM_UM_GateDrvCtrlVar.enabled;
}


/**
ISR: driver di pilotaggio del gate del triac motore, interrupt routine.

\param  [in] alpha_n             angolo di attivazione del triac	calcolato dall'algoritmo di controllo
\param  [in] motorzcref          rifermento ultimo zero crossing voltage
\param  [in] TIME_MIN_FIRE_TRIAC tempo minimo di attivazione del triac parametro da setting

\return none

Per CICLO_SUPERVISORE (4) semionde applica lo stesso angolo di parzializzazione.
Il supervisore (algoritmo di controllo) viene attivato quando manca una semionda
al completamento del ciclo.

L'interrupt si disattiva automaticamente, spegnendo il triac, quando alpha_n vale 0.

\author Roberto Fioravanti

\date 	30/11/2005

*/
static IO_ErrorType PM_UM_GateDrvManagerISR(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{

	uint8_t	bOff=0;
	uint32_t delay;

    /* Check if notification status is good */
    if ((notifVal != IO_N_COMPLETED) || (error != IO_E_OK))
    {
        return IO_E_UNKNOWN_MODE;
    }


    PM_UM_GateDrvCtrlVar.gatedrv_cnt++;
    PM_UM_GateDrvCtrlVar.gatedrv_cnt%=CICLO_SUPERVISORE*2;

    PM_UM_GateDrvCtrlVar.zcRefCounter = OCOCU_GetTickValByTriggerIndex(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC);

    if(PM_UM_GateDrvCtrlVar.gatedrv_cnt & 0x01)
    {
    	/* gate triac is high */
    	uint32_t endImpulseCounter;
    	IO_TickResType resolution;

    	/* triac just turned on  */
    	resolution = OCHWCNTR_GetResolutionByTimerChannel(PM_UM_OcuTmrCh_cfg.OCU_Id);

    	delay=(resolution*(uint32_t)(0xFFFF&(OCOCU_GetSyncTickResolution(PM_UM_OcuTmrCh_cfg.OCU_Id) - OCICU_GetTickSync(PM_UM_OcuTmrCh_cfg.OCU_Id))));

    	if( (PM_UM_GateDrvCtrlVar.alpha > LIB_ZC_GetHalfWaveLengthPos()/2) )
    	{
    		endImpulseCounter = (PM_UM_GateDrvCtrlVar.alpha + PM_UM_GateDrvCtrlVar.pulseLen)*1000UL + delay;
    	}
    	else
    	{
    		endImpulseCounter = (LIB_ZC_GetHalfWaveLengthPos()/2+PM_UM_GateDrvCtrlVar.pulseLen)*1000UL + delay;
    	}



		/* set next OC event */
		OCOCU_SetEventSync(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, endImpulseCounter);

		/* force LOW at compare match */
		OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_OC_PIN_LOW);

		if(OCOCU_IsTickElapsed(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, OCICU_GetTickSync(PM_UM_OcuTmrCh_cfg.OCU_Id)))
		{
			/* pointer to the timer module */
			TMR_CNTAddressType pTmr_ch = TmrChMap[channel].Channel;

			delay = 20000; /*ns*/
			do
			{
				if(OCOCU_IsTickElapsed(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, OCICU_GetTickSync(PM_UM_OcuTmrCh_cfg.OCU_Id)))
				{
					/* Clears the interrupt flag */
					TMR_ClearCHF(pTmr_ch);

					/* set relative event sync */
					OCOCU_SetRelativeEventSync(PM_UM_OcuTmrCh_cfg.OCU_Id, delay);
					OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, TMR_CH_OC_PIN_LOW);
					delay *= 2;
				}
			}while(OCDIO_GetPinValueSync(PM_UM_GateDrvCtrlVar.pintTriac));

			PM_UM_GateDrvCtrlVar.gatedrv_cnt++;

			bOff=1;
		}
    }
    else
    {
    	bOff=1;
    }

    if(bOff)
    {
    	/* triac just turned off and must be reactivated */
    	PM_UM_GateDrvCtrlVar.alpha = PM_UM_GateDrvCtrlVar.alpha_new;

    	if( (PM_UM_GateDrvCtrlVar.gatedrv_cnt == (CICLO_SUPERVISORE*2-2)) )
		{
			PM_UM_ActivateTask();
		}

    	if(PM_UM_GateDrvCtrlVar.alpha)
    	{
    		uint16_t hwp =LIB_ZC_GetHalfWaveLengthPos();
    		uint16_t hwn =LIB_ZC_GetHalfWaveLengthNeg();

    		if(!hwn || !hwp)
    		{
    			hwn = LIB_ZC_GetNominalHalfWave();
    		}
    		if(hwp>hwn)
    		{
    			hwn=hwp;
    		}

    		delay = PM_UM_GateDrvCtrlVar.alpha;

    		if(abs((hwn+hwp)/2 - LIB_ZC_GetNominalHalfWave())>LIB_ZC_GetNominalHalfWave()/10)
    		{
    			PM_UM_OnZC_KO();
    		}

    		if((PM_UM_GateDrvCtrlVar.zcRefCounterOld != PM_UM_GateDrvCtrlVar.zcRefCounter)
    				&& !( PM_UM_IsSuspendedTriac() || PM_UM_IsSuspendedTriacBySafety()))
    		{
    			delay += hwn;
    		}
    		else
    		{
    			/* nothing to do */


    			delay += LIB_ZC_GetNominalHalfWave();
    			PM_UM_GateDrvCtrlVar.mode = TMR_CH_OC_PIN_LOW;
    			PM_UM_OnZC_KO();
    		}

    		OCOCU_SetModeSync(PM_UM_OcuTmrCh_cfg.OCU_Id, PM_UM_GateDrvCtrlVar.mode);
    		OCOCU_SetEventSync(PM_UM_OcuTmrCh_cfg.OCU_Id, HW_CNTR_TRIG_ZC, (delay)*1000UL);

    		PM_UM_GateDrvCtrlVar.zcRefCounterOld = PM_UM_GateDrvCtrlVar.zcRefCounter;
    	}
    	else
    	{
    		/* disable gate driver*/
    		PM_UM_GateDrv_Disable();
    		PM_UM_OnZC_KO();
    	}
    }
    return IO_E_OK;
}
#endif /* CONFIG_PM_MOTORGATE */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
