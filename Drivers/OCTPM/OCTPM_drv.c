/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		OCTPM_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCTPM_drv.c
\ingroup	OCTPM
\date		Oct 22, 2010
\version	01.00.00
\author		Jaroslav Musil,Stefano Ughi
\n
\n
\par	History:
\n
\n 		01.00.03
\n		Apr 4, 2011
\n		Stefano Ughi
\n      - added IO_ErrorType OCTPM_GetChannelSync(const IO_ModuleIOMapType *const pIOMap,uint8_t *pChannel)
\n      Roberto Fioravanti
\n      - added ICU_GetModeSync(IO_ChannelType channel, ICU_EdgeModeType *pMode)
\n      - added IO_ErrorType ICU_ToggleModeSync(IO_ChannelType channel)
\n 		01.00.02
\n		Mar 28, 2011
\n		Stefano Ughi
\n      - changed EnterCritical() with EnterCriticalSection()
\n      - changed ExitCritical() with ExitCriticalSection()
\n 		01.00.01
\n		Dec 12, 2010
\n		Stefano Ughi
\n      - functions review plus linting
\n      - added IO_HwTimerType OCHWCNTR_GetTicksSync(IO_ChannelType channel)
\n      - added Period_ValueType OCHWCNTR_GetTimerModulus(IO_ChannelType channel)
\n 		01.00.00
\n		Oct 22, 2010
\n		Jaroslav Musil
\n      - creation on base of Stefano Ughi's specifications
*/
#define MODULE_BODY_OCTPM
/* ******** */
/* Includes */
/* ******** */
#include "OCTPM_drv.h"
#include "OCDIO_drv.h"
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"
#include "LIB_Error.h"
#include "LIB_TaskMonitoring.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/** OCTPM_PWD_TIMING_DEBUG can be defined to enable debugging information */

/* Capture Event (State Machine) */
#define PWD_FIRST_CAPT_EVENT    0
#define PWD_SECOND_CAPT_EVENT   1
#define PWD_THIRD_CAPT_EVENT    2

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Extern */
extern const TMR_MapType 	TmrChToPinMap[NUM_HW_CNTR_CH];
extern const HAL_TimerId_t	TmrCntMap[NUM_HW_CNTR];
extern const TMR_ChMapType	TmrChMap[NUM_HW_CNTR_CH];

/* Static */
/** Array of pointers to hardware counters config.*/
static const IO_HwCntrCfgType *pHwCntrCfgTable[NUM_HW_CNTR] = {(IO_HwCntrCfgType *)NULL};
/** Stores Input Capture Channel Config. */
static IO_ICU_CfgType TMR_IcuDefinition[NUM_HW_CNTR_CH];
/** Stores Output Compare Channel Config. */
static IO_OCU_CfgType TMR_OcuDefinition[NUM_HW_CNTR_CH];
/** Stores the channel's Pwd Config.*/
static IO_PWD_CfgType TMR_PwdDefinition[NUM_HW_CNTR_CH];
typedef struct {

IO_PWD_CfgType        TMR_Definition;	/** Stores the channel's Pwd Config.*/
uint8_t               TMR_Status;		/** Capture Event (State Machine) */
uint8_t               HwTmrTurns;		/** Hardware Counter Overflow */
bool8_t               DecHwTmrTurns;	/** Flags, not used bitfield to be faster inside interrupt */
Comm_Notification     callback;		/** Stores the end PWD Event callbacks */
IO_HwTimerType        TMR_Start;		/** Stores the First Timer's Capture */
IO_HwTimerType        TMR_Filter;		/** Filter timer capture: minimum time accepted at each trigger */
IO_PWD_RawMeasureType Measurement;		/** Stores the PWD measures */

} PWD_CtrlType;

volatile PWD_CtrlType PWD_CtrlVar[NUM_HW_CNTR_CH];

static const IO_PWM_CfgType*          pPwmCfgTable[NUM_HW_CNTR_CH] = {(IO_PWM_CfgType *)NULL};

#if defined(OCTPM_PWD_TIMING_DEBUG)

uint8_t minTonCntr[NUM_HW_CNTR_CH];
uint8_t maxTonCntr[NUM_HW_CNTR_CH];
uint8_t minPeriodCntr[NUM_HW_CNTR_CH];
uint8_t maxPeriodCntr[NUM_HW_CNTR_CH];
IO_PWD_RawMeasureType PWD_DBG_MinMeasure[NUM_HW_CNTR_CH];
IO_PWD_RawMeasureType PWD_DBG_MaxMeasure[NUM_HW_CNTR_CH];
uint32_t PWD_Timestamp[NUM_HW_CNTR_CH];

#endif



/** ISR callback */
static Comm_Notification        TMR_Callback[NUM_HW_CNTR];
static Comm_Notification        TMR_CH_Callback[NUM_HW_CNTR_CH];

/** Stores hw counter values
first index hw counter ID, the same of TmrCntMap[] table.
the second index is the value.
*/
 volatile IO_HwTimerType TMR_CntTrigVal[NUM_HW_CNTR][NUM_HW_CNTR_COLLECTIONS];

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
/** Structure defining that the timer channel is busy */
static TMR_ChResourceType TMR_ChResource = 0;
/** Structure defining that the timer channel is busy due to Pwd config. */
static TMR_ChResourceType TMR_PwdChResource = 0;
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
static IO_ErrorType OCPWD_EdgeTime(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error);
static IO_ErrorType OCPWD_HwTmrTurns(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error);
static uint32_t OCPWD_GetIntervalInTicks(IO_ChannelType channel, IO_HwTimerType time);
/* *************** */
/* Function Bodies */
/* *************** */

/****************************************** Timer counter - begin ****************************************/

/**
Driver global state initialization initialization.
 
\notes:			Call before using any other method

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
void OCTPM_InitSync(void)
{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
    TMR_ChResource = 0;
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
}

/* Exported function */
/**
\n This function initialises the hardware counter driver. This functions marks the counter as busy.
\n The following parameters are set: prescaler, count direction and modulo.

\param  [in]	The pointer pHwCntrCfg specifies the init structure for the counter.


\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator \n
            or invalid channel as clearing resource.
\retval		IO_E_BUSY counter in use
\retval     IO_E_UNKNOWN_MODE Clearing Source unknown


\note	    After Setting Up an Hardware Counter the function mark that Hardware Counter as busy.\n
            A new Initialization can be performed only after a OCHWCNTR_DeInitSync call.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_ErrorType OCHWCNTR_InitSync(const IO_HwCntrCfgType *pHwCntrCfg)
{
	TMR_CNTAddressType tmdId;
	TMR_CHAddressType  clrSrcChId;

	/* Check if Hardware Counter ID is valid */
	if (pHwCntrCfg->hwCntrlId >= NUM_HW_CNTR  )
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* the timer pointer is assigned */
	tmdId = TmrCntMap[pHwCntrCfg->hwCntrlId];

	EnterCriticalSection();

	/* Checks if the timer module resource is busy */
	if (pHwCntrCfgTable[pHwCntrCfg->hwCntrlId] != NULL)
	{
		ExitCriticalSection();

		/* the resource is busy, but it has been already initialized (eg. OS system timer )*/
		if(pHwCntrCfg->options.skipHWcntrConf)
		{
			return (IO_ErrorType)IO_E_OK;
		}

		return (IO_ErrorType)IO_E_BUSY;
	}
	
	/* Check clearingSourceId */
	if (TMR_CheckClearSourceIdFailed(tmdId, pHwCntrCfg->clearingSourceId))
	{
		ExitCriticalSection();
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;

	}
	
#ifdef TMR_GetClearSourceCh
	clrSrcChId = TMR_GetClearSourceCh(tmdId, pHwCntrCfg->clearingSourceId);
	if(clrSrcChId != NO_CHANNEL_ID)
	{
		int i;
		TMR_CHAddressType ch=NO_CHANNEL_ID;
		/* cerca la risorsa tra quelle configurabili*/
		for(i=0; i<NUM_HW_CNTR_CH ; i++){
			if( TmrChMap[i].Channel == clrSrcChId ){
				ch = i ;
				break;	
			}		
		}
		clrSrcChId = ch;
	}
#else
	clrSrcChId = pHwCntrCfg->clearingSourceId;
#endif

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	if( clrSrcChId != NO_CHANNEL_ID ){
		if(TMR_ChResource & (1 << clrSrcChId)){
			ExitCriticalSection();
			return (IO_ErrorType)IO_E_BUSY;
		}
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */


	SAFETY_REGION_START();
	/* Assigns the timer resource as busy filling pointer */
	pHwCntrCfgTable[pHwCntrCfg->hwCntrlId] = pHwCntrCfg;

	SAFETY_DATA_ELEM_UPDATE(pHwCntrCfgTable, pHwCntrCfg->hwCntrlId);
	SAFETY_REGION_END() ;
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	/* Assigns the timer clearing resource as busy  */
	if( clrSrcChId != NO_CHANNEL_ID){
		TMR_ChResource |= (TMR_ChResourceType)(1<<clrSrcChId);		
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	ExitCriticalSection();
	/* If config. performed outside Eg. OS */
	if( pHwCntrCfg->options.skipHWcntrConf )
	{
		return (IO_ErrorType)IO_E_OK;
	}

	/* Clock Gating */
	TMR_ClockGate(pHwCntrCfg->hwCntrlId, 1);

	/* Sets the prescaler */
	TMR_SetPrescaler(tmdId, pHwCntrCfg->prescaler);

	/* Sets counting direction */
	TMR_SetCountDir(tmdId, pHwCntrCfg->countingMode);

	/* Sets the modulo period */
	TMR_SetModulo(tmdId, pHwCntrCfg->period, pHwCntrCfg->clearingSourceId);
	
#ifdef TMR_GetTickResolution
	/* verifica che il tick resolution sia giusto */
	if( pHwCntrCfg->tickResolution != TMR_GetTickResolution(tmdId))
	{
		return IO_E_INVALID_VALUE;
	}
#endif	
	return (IO_ErrorType)IO_E_OK;
}

/**
\n The function deinitilizes the counter driver. Its setting is returned to its default values.
\n If the counter is not in use, the function won't do anything.

\param  [in]	channel - the identificator of the counter

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator

\todo       deinit case clearing source id different from internal resource
*/
void OCHWCNTR_DeInitSync(IO_ChannelType tmrId)
{
	/* pointer to the timer module */
	TMR_CNTAddressType halTmrId;
	TMR_CHAddressType  clrSrcChId;
	
	PLT_ASSERT(tmrId < NUM_HW_CNTR )

	EnterCriticalSection();

	/* Checks the timer module resource */
	if (pHwCntrCfgTable[tmrId] != NULL)
	{
		ExitCriticalSection();

		/* the timer pointer is assigned */
		halTmrId = TmrCntMap[tmrId];

		/* No input clock */
		TMR_StopClockSource(halTmrId);

		/* Sets the prescaler to M.C.U. reset value */
		TMR_SetPrescalerDfltValue(halTmrId);

		/* Sets counting direction to M.C.U. reset value */
		TMR_SetCountDirDfltValue(halTmrId);

		/* Disable Overflow Interrupt */
		TMR_DisableTI(halTmrId, pHwCntrCfgTable[tmrId]->clearingSourceId);

		/* Clears interrupt flag */
		TMR_ClearTOF(halTmrId, pHwCntrCfgTable[tmrId]->clearingSourceId);

		/* Sets the modulo period to M.C.U. reset value */
		TMR_SetModuloDfltValue(halTmrId, pHwCntrCfgTable[tmrId]->clearingSourceId);

		/* Sets the Timer Counter to M.C.U. reset value */
		TMR_SetCounterDfltValue(halTmrId);

		/* Clock Gating */
		TMR_ClockGate((uint8_t)halTmrId,0);
#ifdef TMR_GetClearSourceCh
		clrSrcChId = TMR_GetClearSourceCh(tmrId, pHwCntrCfgTable[tmrId]->clearingSourceId);
		if(clrSrcChId != NO_CHANNEL_ID)
		{
			int i;
			TMR_CHAddressType ch=NO_CHANNEL_ID;
			/* cerca la risorsa tra quelle configurabili*/
			for(i=0; i<NUM_HW_CNTR_CH ; i++){
				if( TmrChMap[i].Channel == clrSrcChId ){
					ch = i ;
					break;	
				}		
			}
			clrSrcChId = ch;
		}
#else
		clrSrcChId = pHwCntrCfgTable[tmrId]->clearingSourceId;		
#endif
		
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		/* clear resource channel */
		if(clrSrcChId != NO_CHANNEL_ID){
			TMR_ChResource &= ~(1u << clrSrcChId);
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
		/* Release the timer resource */
		pHwCntrCfgTable[tmrId] = (IO_HwCntrCfgType *)NULL;
		
	}else{
		ExitCriticalSection();
	}

	return; /* (IO_ErrorType)IO_E_OK; */
}

/**
\n The function returns the counter register value of the timer. This value is converted into nSec.

\param  [in]	channel - the identificator of the counter

\attention\b Correct tick resolution must be filled in the init structure

\return 	IO_TimingValueType

\retval		counter value

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_TimingValueType OCHWCNTR_GetSync(IO_ChannelType channel)
{
	TMR_CNTAddressType pTmr_cnt;
	IO_TimingValueType time_nSec;
	/* pointer to the timer module */
	pTmr_cnt = TmrCntMap[channel];
	time_nSec = ((IO_TimingValueType)TMR_GetCounter(pTmr_cnt) * pHwCntrCfgTable[channel]->tickResolution);
	return time_nSec;
}
/**
\n The function returns the counter register value of the timer. This value is in ticks.

\param  [in]	channel - the identificator of the counter

\return 	IO_TimingValueType
\retval		counter value

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_HwTimerType OCHWCNTR_GetTicksSync(IO_ChannelType timerId)
{
	TMR_CNTAddressType pTmr_cnt;
	IO_HwTimerType timerValueInTicks;
	/* pointer to the timer module */
	pTmr_cnt = TmrCntMap[timerId];
	timerValueInTicks = TMR_GetCounter(pTmr_cnt);
	return timerValueInTicks;
}

/**
\n The function sets the counter value register of the timer. The input value is in nSec.

\param  [in]	channel - the identificator of the counter
\param  [in]	timerValue - the counter value in nSec

\attention\b Correct tick resolution must be filled in the init structure

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\warning	If tick resolution is 0, the application will crash by div by zero

\todo		not supported in HCS08 e AG128

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_ErrorType OCHWCNTR_SetSync(IO_ChannelType timerId, IO_TimingValueType timerValue)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_cnt;

	if (timerId >= NUM_HW_CNTR)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Checks the timer module resource */
	if (pHwCntrCfgTable[timerId] == NULL)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	/* the timer pointer is assigned */
	pTmr_cnt = TmrCntMap[timerId];

	TMR_SetCounter(pTmr_cnt, (IO_HwTimerType)(timerValue / pHwCntrCfgTable[timerId]->tickResolution));

	return (IO_ErrorType)IO_E_OK;
}


void OCHWCNTR_EnableMulti(uint32_t uBmp)
{
	TMR_StopClockSourceMulti(uBmp);
	TMR_SetClockSourceMulti(uBmp);
}
/**
\n The function enables and set the Timer Clock Source or disables the counter.

\param  [in]	channel - the identificator of the counter
\param  [in]	enable - true enables the counter and set the clock source, false disables the counter


\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_ErrorType OCHWCNTR_Enable(IO_ChannelType tmrId, bool_t enable)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_cnt;

	if (tmrId >= NUM_HW_CNTR)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Check if hardware counter setup done */
	if (pHwCntrCfgTable[tmrId] == NULL)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	/* the timer pointer is assigned */
	pTmr_cnt = TmrCntMap[tmrId];

	if (enable)
	{
		TMR_SetClockSource(pTmr_cnt, pHwCntrCfgTable[tmrId]->clockSource);
	}
	else
	{
		TMR_StopClockSource(pTmr_cnt);
	}

	return (IO_ErrorType)IO_E_OK;
}
/**
The function enables or disables the ISR of the counter.

\param  [in]	channel - the identificator of the counter
\param  [in]	enableISR - true enables the ISR, false disables the ISR
\param  [in]	callback - pointer to the callback function

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION the channel is not configured

\note       when passing enableISR=FALSE it's reccomanded to pass callbak=NULL

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_ErrorType OCHWCNTR_EnableISR(IO_ChannelType tmrId, bool_t enableISR, Comm_Notification callback)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_cnt;

	if (tmrId >= NUM_HW_CNTR)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Check if hardware counter setup done */
	if (pHwCntrCfgTable[tmrId] == NULL)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	/* the timer pointer is assigned */
	pTmr_cnt = TmrCntMap[tmrId];

	if (!enableISR)
	{
        	TMR_DisableTI(pTmr_cnt, pHwCntrCfgTable[tmrId]->clearingSourceId);
	}

	TMR_Callback[tmrId] = callback;

	if (enableISR)
	{
        	TMR_ClearTOF(pTmr_cnt, pHwCntrCfgTable[tmrId]->clearingSourceId);
        	TMR_EnableTI(pTmr_cnt, pHwCntrCfgTable[tmrId]->clearingSourceId);
	}

	return (IO_ErrorType)IO_E_OK;
}

/**
The function returns the tick resolution relating to a channel

\param  [in]	ch - the identifier of the channel

\return 	IO_TickResType
\retval		tick resolution in nSec

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_TickResType OCHWCNTR_GetResolutionByTimerChannel(IO_ChannelType ch)
{
	return OCHWCNTR_GetTickResolution(TmrChMap[ch].hwCntrlId);
}

/**
The function returns the tick resolution.

\param  [in]	channel - the identifier of the counter

\return 	IO_TickResType
\retval		tick resolution in nSec

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_TickResType OCHWCNTR_GetTickResolution(IO_ChannelType tmrId)
{
#if defined(TMR_GetTickResolution)
	return TMR_GetTickResolution( pHwCntrCfgTable[tmrId]->hwCntrlId );	
#else
	return ( pHwCntrCfgTable[tmrId]->tickResolution);
#endif
}

/**
The function returns the timer's modulus.

\param  [in]	channel - the identificator of the counter

\return 	Period_ValueType
\retval		period in ticks

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
Period_ValueType OCHWCNTR_GetTimerModulus(IO_ChannelType tmrId)
{
	return (pHwCntrCfgTable[tmrId]->period);
}


/**
The function returns the timer period in nanoseconds
\param  [in]	channel - the identificator of the counter

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_TimingValueType OCHWCNTR_GetPeriod(IO_ChannelType tmrId)
{
	return (pHwCntrCfgTable[tmrId]->period * pHwCntrCfgTable[tmrId]->tickResolution );
}
/**
The function returns the maximum timer period in nanoseconds
\param  [in]	channel - the identificator of the counter

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
IO_TimingValueType OCHWCNTR_GetMaxPeriod(IO_ChannelType tmrId)
{
	 return( (IO_TimingValueType)pHwCntrCfgTable[tmrId]->tickResolution * 0xFFFF ); /*MS TODO: definire un funzione nell'HAL per prendere il massimo valore del periodo del timer*/
}

/**
The function returns the timer's modulus.

\param  [in]	channel - the identificator of the counter

\return 	Period_ValueType
\retval		period in ticks
*/
Period_ValueType OCHWCNTR_GetDelta(IO_ChannelType tmrId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR)
{
	return TMR_GetDelta(TmrCntMap[tmrId], CntrVal_OLD, CntrVal_CURR);
}

/**
ISR of the counter with id channel. This function calls the callback function defined by the user.
\param		[in] tmrID

\note		This function must call inside interrupt vector

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_2
*/
void HWCNTR_ISR(IO_ChannelType tmrID)
{
	IRQ_MONITOR_ENTER();

	if( tmrID < NUM_HW_CNTR )
	{
		/* pointer to the timer module */
		TMR_CNTAddressType pTmr_cnt = TmrCntMap[tmrID];

		if(TMR_Callback[tmrID] != 0)
		{
			/* Calls the user callback */
			(void)(TMR_Callback)[tmrID]((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)tmrID, (IO_ErrorType)IO_E_OK);
		}

		/* Clears the interrupt flag */
    	TMR_ClearTOF(pTmr_cnt, pHwCntrCfgTable[tmrID]->clearingSourceId);
	}

	IRQ_MONITOR_EXIT();

#if 0 /* NOTA: problemi di compatibilita? perchè è stato inserita questa modifica, ho ripristinato l'handler perch altrimenti non funziona nulla*/
	/* callback index */
    int callbackIndex = -1;
    /* pointer to the timer module */
	TMR_CNTAddressType pTmr_cnt = 0;
	
	if( tmrID < NUM_HW_CNTR )
	{
    /* pointer to the timer module */
    	if(pHwCntrCfgTable[TmrChMap[tmrID].hwCntrlId]->clearingSourceId == 0xFF)
    	{
        	callbackIndex=tmrID;
			pTmr_cnt = TmrCntMap[tmrID];
		}
		else
		{
        	callbackIndex = TmrChMap[tmrID].hwCntrlId;
        	pTmr_cnt = TmrCntMap[TmrChMap[tmrID].hwCntrlId];
		}
    
		if(TMR_Callback[callbackIndex] != 0)
	    {
			/* Calls the user callback */
			(void)(TMR_Callback)[callbackIndex]((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)callbackIndex, (IO_ErrorType)IO_E_OK);
	    }
	
		/* Clears the interrupt flag */
    	TMR_ClearTOF(pTmr_cnt, pHwCntrCfgTable[tmrID]->clearingSourceId);
	}
#endif
}
/**
 * Set the base counter for trigger management
 \param [in]  triggID
 */
void OCHWCNTR_Trigger(OCTPM_TriggerIndexType triggID )
{
	int i=0;
	PLT_ERROR_FATAL_IF((triggID>=NUM_HW_CNTR_COLLECTIONS), PLT_ERROR_OCTPM_INVALID_PARAMS);

	for(i=0; i<(int)(sizeof(TmrCntMap)/sizeof(TmrCntMap[0]));i++)
	{
		TMR_CntTrigVal[i][triggID]=OCHWCNTR_GetTicksSync(i);
	}
}


/******************************************* Timer counter - end ******************************************/

/*************************************** Timer input capture - begin **************************************/

/**
This function initializes the hardware timer channel driver for the function of input capture.
This functions marks the channel as busy.

\param  [in]	The pointer pICU_Cfg specifies the init structure for the channel.

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identifier
\retval		IO_E_UNEXPECTED_CONDITION HW Timer channel is not yet configured
\retval		IO_E_BUSY counter in use

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/

IO_ErrorType OCICU_InitSync(const IO_ICU_CfgType *pICU_Cfg)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_ch;

	/* Check if Timer ID is in range */
	if (pICU_Cfg -> ICU_Id >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Check if HW Timer has already been initialized */
	if ( pHwCntrCfgTable[TmrChMap[pICU_Cfg -> ICU_Id].hwCntrlId] == NULL )
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (TMR_ChResource & (1 << pICU_Cfg->ICU_Id))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_BUSY;
	}

	/* Assigns the timer resource as busy */
	TMR_ChResource |= (1 << pICU_Cfg->ICU_Id);

	ExitCriticalSection();
#endif

	SAFETY_REGION_START();
	/* Copy the timer structure */
	TMR_IcuDefinition[pICU_Cfg->ICU_Id] = *pICU_Cfg;
	SAFETY_DATA_ELEM_UPDATE(TMR_IcuDefinition, pICU_Cfg->ICU_Id );
	SAFETY_REGION_END();
	/* the timer pointer is assigned */
	pTmr_ch = TmrChMap[pICU_Cfg->ICU_Id].Channel;

	/* Sets to default input capture */
	TMR_SetChannelInputChapture(pTmr_ch);

	return (IO_ErrorType)IO_E_OK;
}

/**
\n The function dispose of the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]	channel - the identifier of the channel

\return 	IO_ErrorType
\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
*/
IO_ErrorType OCICU_DeInitSync(IO_ChannelType channel)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_ch;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer channel resource */
	if (TMR_ChResource & (1 << channel))
	{
		ExitCriticalSection();
#endif

		SAFETY_REGION_START();
		/* Clears the timer structure */
		(void)memset (&TMR_IcuDefinition[channel], 0, sizeof(IO_ICU_CfgType));
		SAFETY_DATA_ELEM_UPDATE(TMR_IcuDefinition, channel);
		SAFETY_REGION_END();
		/* the timer pointer is assigned */
		pTmr_ch = TmrChMap[channel].Channel;

		TMR_DeInitChannel(pTmr_ch);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		/* Release the timer resource */
		TMR_ChResource &= (~(1u << channel));
	}
	else
	{
		ExitCriticalSection();
	}
#endif
	return (IO_ErrorType)IO_E_OK;
}

/**
The function returns the captured value register of the channel converted in nSec.

\param  [in]	channel - the identifier of the channel
\return 	IO_TimingValueType
\retval		captured value converted in nsec

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
IO_TimingValueType OCICU_GetSync(IO_ChannelType channel)
{
	TMR_CHAddressType pTmr_ch;

	/* pointer to the timer channel */
	pTmr_ch = TmrChMap[channel].Channel;

	return (IO_TimingValueType)TMR_GetChValue(pTmr_ch) * OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId);
}

/**
The function get the raw counter value of the captured value of the channel

\param [in] channel

\return  IO_HwTimerType
\retval captured value

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
 */
IO_HwTimerType OCICU_GetTickSync(IO_ChannelType channel)
{
	TMR_CHAddressType pTmr_ch;

	/* pointer to the timer channel */
	pTmr_ch = TmrChMap[channel].Channel;

	return TMR_GetChValue( pTmr_ch );
}

/**
\n The function sets the edge type which triggers the capture event.
\n The edge can be rising, falling or both.

\param  [in]	channel - the identificator of the channel
\param  [in]	mode - 1 rising, 2 falling, 3 both

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
IO_ErrorType OCICU_SetModeSync(IO_ChannelType channel, IO_ModeType mode)
{
	/* pointer to the timer module */
	TMR_CNTAddressType pTmr_ch;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	SAFETY_DATA_ELEM_CHECK(TMR_IcuDefinition, channel);

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	pTmr_ch = TmrChMap[channel].Channel;

	TMR_SetChannelEdgeMode(pTmr_ch, mode, TMR_IcuDefinition[channel].inverted);

	return (IO_ErrorType)IO_E_OK;
}

/**
Returns mode is rising or falling.
If edge is set to both configuration remains unchanged.

\param  [in]	channel - the identificator of the channel
\param  [out]	pMode mode:
\li		0   invalid
\li		1   rising
\li		2   falling
\li		3   both

\return 	IO_ErrorType

\retval		IO_E_OK returned value is valid
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
IO_ErrorType OCICU_GetModeSync(IO_ChannelType channel, ICU_EdgeModeType *pMode)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;
	uint8_t tmpRes;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	ExitCriticalSection();
#endif
	pTmr_ch = TmrChMap[channel].Channel;

	tmpRes = TMR_GetChannelEdgeMode(pTmr_ch);
	*pMode = (ICU_EdgeModeType)tmpRes;
	return (IO_ErrorType)IO_E_OK;
}

/**
Toggles edge type which triggers the capture event if mode is rising or falling.
If edge is set to both configuration remains unchanged.

\param  [in]	channel - the identificator of the channel

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
IO_ErrorType OCICU_ToggleModeSync(IO_ChannelType channel)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	pTmr_ch = TmrChMap[channel].Channel;

	TMR_ToggleChannelEdgeMode(pTmr_ch);

	return (IO_ErrorType)IO_E_OK;
}

/**
The function enables or disables the ISR of the channel for input capture event.

\param  [in]	channel - the identificator of the channel
\param  [in]	enableISR - true enables the ISR, false disables the ISR
\param  [in]	callback - pointer to the callback function

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_3
*/
IO_ErrorType OCICU_EnableISR(IO_ChannelType channel, bool_t enableISR, Comm_Notification callback)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

	/* the timer pointer is assigned */
	pTmr_ch = TmrChMap[channel].Channel;

	if (!enableISR)
	{
		TMR_DisableCHI(pTmr_ch);
	}

	TMR_CH_Callback[channel] = callback;

	if (enableISR)
	{
		TMR_ClearCHF(pTmr_ch);
		TMR_EnableCHI(pTmr_ch);
	}

	return (IO_ErrorType)IO_E_OK;
}
/*************************************** Timer input capture - end *********************************/

/************************************** Timer output compare - begin *******************************/

/**
\n This function initialises the hardware timer channel driver for the function of output compare.
\n This functions marks the channel as busy.
\n The following parameters are set: Output compare mode.

\param  [in]	The pointer pOCU_Cfg specifies the init structure for the channel.

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION HW Timer channel is not yet configured
\retval		IO_E_BUSY counter in use

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
IO_ErrorType OCOCU_InitSync(IO_OCU_CfgType *pOCU_Cfg)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;

	/* Check if Timer ID is in range */
	if (pOCU_Cfg -> OCU_Id >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Check if HW Timer has already been initialized */
	if ( pHwCntrCfgTable[TmrChMap[pOCU_Cfg -> OCU_Id].hwCntrlId] == NULL )
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();
	/* Checks the timer module resource */
	if (TMR_ChResource & (1 << pOCU_Cfg->OCU_Id))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_BUSY;
	}

	/* Assigns the timer resource as busy */
	TMR_ChResource |= (1 << pOCU_Cfg->OCU_Id);
	ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

	/* Copy the timer structure */
	TMR_OcuDefinition[pOCU_Cfg->OCU_Id] = *pOCU_Cfg;

	/* the timer pointer is assigned */
	pTmr_ch = TmrChMap[pOCU_Cfg->OCU_Id].Channel;

	/* Sets to default output compare */
	TMR_SetChannelOutputCompare(pTmr_ch);

	return (IO_ErrorType)IO_E_OK;
}

/**
\n The function deinitilizes the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]	channel - the identifier of the channel

\return 	IO_ErrorType
\retval		IO_E_OK de-initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid channel identifier

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
IO_ErrorType OCOCU_DeInitSync(IO_ChannelType channel)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;


	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();
	/* Checks the timer module resource */
	if (TMR_ChResource & (1 << channel))
	{
		ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

		/* Clears the timer structure */
		(void)memset (&TMR_OcuDefinition[channel], 0, sizeof(IO_OCU_CfgType));

		/* the timer pointer is assigned */
		pTmr_ch = TmrChMap[channel].Channel;

		TMR_DeInitChannel(pTmr_ch);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		/* Release the timer resource */
		TMR_ChResource &= ~(1u << channel);
	}
	else
	{
		ExitCriticalSection();
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	return (IO_ErrorType)IO_E_OK;
}

/**
\n The function returns the counter value. This value is converted into nSec.

\param  [in]	channel - the identificator of the channel

\return 	IO_TimingValueType
\retval		counter value

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
IO_TimingValueType OCOCU_GetSync(IO_ChannelType channel)
{
	return (IO_TimingValueType)OCHWCNTR_GetSync(TmrChMap[channel].hwCntrlId);
}

/**
\n The function returns the counter value. This value is raw in ticks.

\param  [in]	channel - the identifier of the channel

\return 	IO_HwTimerType
\retval		counter value in ticks

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
IO_HwTimerType OCOCU_GetSyncTickResolution(IO_ChannelType channel)
{
	return (IO_HwTimerType)OCHWCNTR_GetTicksSync(TmrChMap[channel].hwCntrlId);
}

/**
\n The function sets the threshold 1 for the output compare event of the channel. The value is in nSec.

\param  [in]	channel - the identifier of the channel
\param  [in]	threshold1 - the threshold value 1 in nSec

\warning	If tick resolution is 0, the application will crash by div by zero

\note		Correct tick resolution must be filled in the init structure.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetThreshold1Sync(IO_ChannelType channel, IO_TimingValueType threshold1)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;
	uint32_t tickRes;
	uint32_t tmrPeriod;
	uint32_t threshold;

	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			pTmr_ch = TmrChMap[channel].Channel;

			tickRes = OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId);
			tmrPeriod = TMR_GetModulo( TmrChMap[channel].hwCntrlId, channel);
			threshold = threshold1 / tickRes;
			
			if( threshold > tmrPeriod )
			{
				threshold -=tmrPeriod;
			}
			TMR_SetThreshold1(pTmr_ch, (IO_HwTimerType)threshold);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}

/**
\n The function sets the treshold 1 for the output compare event of the channel. The value is raw in ticks.

\param  [in]	channel - the identificator of the channel
\param  [in]	threshold1 - the treshold value 1

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetThreshold1TickResolutionSync(IO_ChannelType channel, IO_HwTimerType threshold1)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;
	Period_ValueType tmrPeriod;

	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			pTmr_ch = TmrChMap[channel].Channel;
			tmrPeriod = TMR_GetModulo( TmrChMap[channel].hwCntrlId, channel);
			if( threshold1 > tmrPeriod )
			{
				threshold1 -=tmrPeriod;
			}
			TMR_SetThreshold1(pTmr_ch, threshold1);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}

bool8_t OCOCU_IsTickElapsed(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, IO_TickResType tick)
{
	bool8_t res = FALSE;
	uint8_t tmrID = 0xFF;
	IO_HwTimerType	ref, curr;
	IO_HwTimerType diff1, diff2;
	PLT_ERROR_FATAL_IF((trigRefIndex>=NUM_HW_CNTR_COLLECTIONS), PLT_ERROR_OCTPM_INVALID_PARAMS);
	PLT_ERROR_FATAL_IF((channel>=NUM_HW_CNTR_CH), PLT_ERROR_OCTPM_INVALID_INDEX);

	tmrID = OCTPM_GetTimerIDFromChannelID_1(channel);
	PLT_ERROR_FATAL_IF((tmrID>=(sizeof(TmrCntMap)/sizeof(TmrCntMap[0]))), PLT_ERROR_OCTPM_INVALID_INDEX);

	curr=OCHWCNTR_GetTicksSync(tmrID);
	ref = (IO_HwTimerType)(TMR_CntTrigVal[tmrID][trigRefIndex]);
	diff1 = curr-ref;
	diff2 = tick-ref;

	if((diff1)>=(diff2))
	{
		res = TRUE;
	}

	return res;
}

/*
Returns hw counter val in tick corresponding to an event specified by trigger ID index relating to a channel
*/
IO_HwTimerType OCOCU_GetTickValByTriggerIndex(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex)
{
	uint8_t tmrID = 0xFF;
	IO_HwTimerType	ref;

	PLT_ERROR_FATAL_IF((trigRefIndex>=NUM_HW_CNTR_COLLECTIONS), PLT_ERROR_OCTPM_INVALID_PARAMS);
	PLT_ERROR_FATAL_IF((channel>=NUM_HW_CNTR_CH), PLT_ERROR_OCTPM_INVALID_INDEX);

	tmrID = OCTPM_GetTimerIDFromChannelID_1(channel);
	PLT_ERROR_FATAL_IF((tmrID>=(sizeof(TmrCntMap)/sizeof(TmrCntMap[0]))), PLT_ERROR_OCTPM_INVALID_INDEX);

	ref = (TMR_CntTrigVal[tmrID][trigRefIndex]);

	return ref;
}

/*
Sets hw counter val relating to a channel by specifying trigegr ID index
*/
void OCOCU_SetTickValByTriggerIndex(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, IO_HwTimerType val)
{
	uint8_t tmrID = 0xFF;

	PLT_ERROR_FATAL_IF((trigRefIndex>=NUM_HW_CNTR_COLLECTIONS), PLT_ERROR_OCTPM_INVALID_PARAMS);
	PLT_ERROR_FATAL_IF((channel>=NUM_HW_CNTR_CH), PLT_ERROR_OCTPM_INVALID_INDEX);

	tmrID = OCTPM_GetTimerIDFromChannelID_1(channel);
	PLT_ERROR_FATAL_IF((tmrID>=(sizeof(TmrCntMap)/sizeof(TmrCntMap[0]))), PLT_ERROR_OCTPM_INVALID_INDEX);

	TMR_CntTrigVal[tmrID][trigRefIndex] = val;

}
/**
\n Sets an event with a delay [ns] respect to trigger

\param  [in]	channel - the identifier of the channel
\param  [in]	trigRefIndex - trigger ID
\param  [in]	nsDelay - delay respect to the trigger in ns

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetEventSync(IO_ChannelType channel, OCTPM_TriggerIndexType trigRefIndex, uint32_t nsDelay)
{
	/* resolution */
	IO_TickResType timerRes = 0;
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch = 0xFF;
	IO_HwTimerType ticks=0;

	uint8_t tmrID = 0xFF;
	Period_ValueType tmrPeriod;


	PLT_ERROR_FATAL_IF((trigRefIndex>=NUM_HW_CNTR_COLLECTIONS), PLT_ERROR_OCTPM_INVALID_PARAMS);
	PLT_ERROR_FATAL_IF((channel>=NUM_HW_CNTR_CH), PLT_ERROR_OCTPM_INVALID_INDEX);


	tmrID = OCTPM_GetTimerIDFromChannelID_1(channel);

	PLT_ERROR_FATAL_IF((tmrID>=(sizeof(TmrCntMap)/sizeof(TmrCntMap[0]))), PLT_ERROR_OCTPM_INVALID_INDEX);

	/* tick resolution of the counter */
	timerRes = OCHWCNTR_GetTickResolution(tmrID);

	PLT_ERROR_FATAL_IF((timerRes==0), PLT_ERROR_OCTPM_INVALID_PARAMS);

	ticks = (IO_HwTimerType)(TMR_CntTrigVal[tmrID][trigRefIndex] + nsDelay/timerRes);


	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			pTmr_ch = TmrChMap[channel].Channel;
			tmrPeriod = TMR_GetModulo( tmrID, channel);
			if( ticks > tmrPeriod )
			{
				ticks -=tmrPeriod;
			}
			TMR_SetThreshold1(pTmr_ch, ticks);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}

/**
\n Sets an event with a delay [ns] respect to current compare match

\param  [in]	channel - the identifier of the channel
\param  [in]	nsDelay - delay respect to the trigger in ns

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetRelativeEventSync(IO_ChannelType channel, uint32_t nsDelay)
{
	/* resolution */
	IO_TickResType timerRes = 0;
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch = 0xFF;
	IO_HwTimerType ticks=0;

	uint8_t tmrID = 0xFF;
	Period_ValueType tmrPeriod;

	PLT_ERROR_FATAL_IF((channel >= NUM_HW_CNTR_CH), PLT_ERROR_OCTPM_INVALID_INDEX);

	tmrID = OCTPM_GetTimerIDFromChannelID_1(channel);

	PLT_ERROR_FATAL_IF((tmrID >= (sizeof(TmrCntMap)/sizeof(TmrCntMap[0]))), PLT_ERROR_OCTPM_INVALID_INDEX);

	/* tick resolution of the counter */
	timerRes = OCHWCNTR_GetTickResolution(tmrID);

	PLT_ERROR_FATAL_IF((timerRes==0), PLT_ERROR_OCTPM_INVALID_PARAMS);

	ticks = (IO_HwTimerType)(OCICU_GetTickSync(channel) + nsDelay/timerRes);


	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			pTmr_ch = TmrChMap[channel].Channel;
			tmrPeriod = TMR_GetModulo( tmrID, channel);
			if( ticks > tmrPeriod )
			{
				ticks -=tmrPeriod;
			}
			TMR_SetThreshold1(pTmr_ch, ticks);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}


/**
\n The function sets the threshold 2 for the output compare event of the channel. The value is in nSec.

\param  [in]	channel - the identificator of the channel
\param  [in]	threshold2 - the treshold value 2 in nSec

\warning	If tick resolution is 0, the application will crash by div by zero

\note		Correct tick resolution must be filled in the init structure

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetThreshold2Sync(IO_ChannelType channel, IO_TimingValueType threshold2)
{
	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			if(FALSE == Threshold2SetUpUnsupported(channel))
			{
				TMR_SetThreshold2(TmrChMap[channel].Channel, threshold2 / OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId));
			}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}

/**
\n The function sets the threshold 2 for the output compare event of the channel. The value is raw in ticks.

\param  [in]	channel - the identificator of the channel
\param  [in]	threshold2 - the threshold value 2

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
void OCOCU_SetThreshold2TickResolutionSync(IO_ChannelType channel, IO_HwTimerType threshold2)
{
	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			if(FALSE == Threshold2SetUpUnsupported(channel))
			{
				TMR_SetThreshold2(TmrChMap[channel].Channel, threshold2);
			}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}
}

/**
\n The function sets the edge type which will be generated on the compare event.
\n The level can be high, low or toggle with respect to the 2 possible threshold.

\param  [in]	channel - the identifier of the channel
\param  [in]	mode - 4 no edge event, 5 toggle on T1, 6 toggle on T2, 7 set high, 8 set low

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned
\retval     IO_E_UNKNOWN_MODE mode not supported

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_4
*/
IO_ErrorType OCOCU_SetModeSync(IO_ChannelType channel, IO_ModeType mode)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;


	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	if (OutputCompareModeUnsupported(mode,TMR_OcuDefinition[channel].inverted))
	{
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();
	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

	pTmr_ch = TmrChMap[channel].Channel;

	TMR_SetChannelOutputMode(pTmr_ch, mode,TMR_OcuDefinition[channel].inverted);

	return (IO_ErrorType)IO_E_OK;
}

/************************************** Timer output compare - end ***********************************/


/************** Timer PWM - begin **************/

/**
\n This function initializes the hardware timer channel driver for the function of PWM.
\n This functions marks the channel as busy.
\n The following parameters are set: PWM mode.

\param  [in]	The pointer pPWM_Cfg specifies the init structure for the channel.

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION HW Timer channel is not yet configured
\retval		IO_E_BUSY counter in use

*/
IO_ErrorType OCPWM_InitSync(IO_PWM_CfgType *pPWM_Cfg)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;

	/* Check if Timer ID is in range */
	if (pPWM_Cfg -> PWM_Id >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	/* Check if HW Timer has already been initialized */
	if ( pHwCntrCfgTable[TmrChMap[pPWM_Cfg -> PWM_Id].hwCntrlId] == NULL )
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (TMR_ChResource & (1 << pPWM_Cfg -> PWM_Id))
	{
		ExitCriticalSection();

		return (IO_ErrorType)IO_E_BUSY;
	}

	/* Assigns the timer resource as busy */
	TMR_ChResource |= (1 << pPWM_Cfg -> PWM_Id);

	ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

	/* Assigns Config. Pointer */
	pPwmCfgTable[pPWM_Cfg -> PWM_Id] = pPWM_Cfg;


	/* the timer pointer is assigned */
	pTmr_ch = TmrChMap[pPWM_Cfg -> PWM_Id].Channel;

	/* PWM channel config. no activation! */
	TMR_SetChannelPWM(pTmr_ch);

	return (IO_ErrorType)IO_E_OK;
}
/**
\n The function dispose of the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]	channel - the identifier of the channel

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
*/
IO_ErrorType OCPWM_DeInitSync(IO_ChannelType channel)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	EnterCriticalSection();

	/* Checks the timer module resource */
	if (TMR_ChResource & (1 << channel))
	{
		ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
		/* Clears the timer structure */
		pPwmCfgTable[channel]= (IO_PWM_CfgType *)NULL;

		/* the timer pointer is assigned */
		pTmr_ch = TmrChMap[channel].Channel;

		TMR_DeInitChannel(pTmr_ch);
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		/* Release the timer resource */
		TMR_ChResource &= ~(1u << channel);
	}
	else
	{
		ExitCriticalSection();
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	return (IO_ErrorType)IO_E_OK;
}

/**
\n The function sets the on-time of the PWM of the channel. The value is in nSec.

\param  [in]	channel - the identificator of the channel
\param  [in]	tOn - the on-time value in nSec

\warning	If tick resolution is 0, the application will crash by div by zero
*/
IO_ErrorType OCPWM_SetSync(IO_ChannelType channel, IO_TimingValueType tOn)
{
	/* pointer to the timer module */
	TMR_CHAddressType pTmr_ch;
	IO_ErrorType retVal = IO_E_OK;

	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
			pTmr_ch = TmrChMap[channel].Channel;

			/* verifica che il onTime non superi il periodo del timer*/
			if( tOn >= OCHWCNTR_GetPeriod(TmrChMap[channel].hwCntrlId) ){
				retVal = IO_E_INVALID_VALUE;
			}else{
				/* Turns on the PWM mode */
				TMR_SetChannelPulses(pTmr_ch, pPwmCfgTable[channel]->inverted);

				/* Sets the on time */
				TMR_SetTimeOn(pTmr_ch, (IO_HwTimerType)(tOn / OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId)));
			}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}
		else
		{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}else{
		return retVal = IO_E_INVALID_CHANNEL_ID;
	}
	return retVal;
}

/**
The function returns the on-time value in nSec.

\param  [in]	channel - the identificator of the channel

\return 	IO_TimingValueType
\retval		on-time value
*/
IO_TimingValueType OCPWM_GetSync(IO_ChannelType channel)
{
	TMR_CHAddressType pTmr_ch;
	Period_ValueType tOn;
	
	if( channel >= NUM_HW_CNTR_CH )
		return 0; /*TODO: gestion errore*/

	/* pointer to the timer module */
	pTmr_ch = TmrChMap[channel].Channel;

	TMR_GetTimeOn( pTmr_ch, &tOn); 
	
	return (IO_TimingValueType) tOn * OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId);
}

/**
The function returns the period value. This value is converted into nSec.

\param  [in]	channel - the identificator of the channel

\attention\b Correct tick resolution must be filled in the init structure of the counter

\return 	IO_TimingValueType
\retval		period value
*/
IO_TimingValueType OCPWM_GetPeriodSync(IO_ChannelType channel)
{
	TMR_CHAddressType pTmr_cnt;
	uint8_t hwCntr;

	if( channel >= NUM_HW_CNTR_CH )
		return 0; /*TODO: gestion errore*/
		
	hwCntr = TmrChMap[channel].hwCntrlId;
	/* pointer to the timer module */
	pTmr_cnt = TmrCntMap[hwCntr];

    return (IO_TimingValueType)TMR_GetModulo(pTmr_cnt, channel) * OCHWCNTR_GetTickResolution(hwCntr);
}

/**
\n The function sets the period of the PWM of the channel. The value is in nSec.

\param  [in]	channel - the identifier of the channel
\param  [in]	period - the period value in nSec

\attention\b Correct tick resolution must be filled in the init structure\n
\attention\b Timer Modulo affected\n

\warning	If tick resolution is 0, the application will crash by div by zero

*/
IO_ErrorType OCPWM_SetPeriodSync(IO_ChannelType channel, IO_TimingValueType period)
{
	TMR_CNTAddressType pTmr_cnt;
	uint8_t hwCntrlId;
	IO_ErrorType retVal = IO_E_OK;

	if (channel < NUM_HW_CNTR_CH)
	{
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		EnterCriticalSection();

		/* Checks the timer module resource */
		if ((TMR_ChResource & (1 << channel)))
		{
			ExitCriticalSection();
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */

			hwCntrlId = TmrChMap[channel].hwCntrlId;
			EnterCriticalSection();

			if ( pHwCntrCfgTable[hwCntrlId] != NULL )
			{
				ExitCriticalSection();

				/* check for overflow*/
				if( period >= OCHWCNTR_GetMaxPeriod(TmrChMap[channel].hwCntrlId) ){
						retVal = IO_E_INVALID_VALUE;
				}else{
					/* pointer to the timer module */
					pTmr_cnt = TmrCntMap[hwCntrlId];

					/* Sets the modulo */
					TMR_SetModulo(pTmr_cnt, (period / OCHWCNTR_GetTickResolution(hwCntrlId)), pHwCntrCfgTable[hwCntrlId]->clearingSourceId);						
				}
			}else{
					ExitCriticalSection();
			}
 #if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		}else{
			ExitCriticalSection();
		}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}else{
		retVal = IO_E_INVALID_CHANNEL_ID;
	}
	return retVal;
}


/**
\n The function sets the on-time and the period of the PWM of the channel. The value is in nSec.

\param  [in]	channel - the identificator of the channel
\param  [in]	tOn - the on-time value in nSec
\param  [in]	period - the period value in nSec


\attention\b Correct tick resolution must be filled in the init structure\n
\attention\b Timer Modulo affected\n

\warning	If tick resolution is 0, the application will crash by div by zero
*/
void OCPWM_SetAllSync(IO_ChannelType channel, IO_TimingValueType tOn, IO_TimingValueType period)
{
	OCPWM_SetSync(channel, tOn);

	OCPWM_SetPeriodSync(channel, period);
}
/**
\n The function enables or disables the ISR of the channel for PWM at the beginning of the new period.

\param  [in]	channel - the identificator of the channel
\param  [in]	enableISR - true enables the ISR, false disables the ISR
\param  [in]	callback - pointer to the callback function

\attention\b    the functions overwrites previous callback on hardware counter ISR

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned
*/
IO_ErrorType OCPWM_EnableT1ISR(IO_ChannelType channel, bool_t enableISR, Comm_Notification callback)
{

	if (channel >= NUM_HW_CNTR_CH)
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
	/* Checks the timer module resource */
	if (!(TMR_ChResource & (1 << channel)))
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	return (IO_ErrorType)OCHWCNTR_EnableISR(TmrChMap[channel].hwCntrlId, enableISR, callback);
}

/************************************************ Timer PWM - end **********************************************/


/*********************************************** Timer PWD - begin *********************************************/

/**
\n This function initialises the hardware timer channel driver for the function of PWD. It uses the input capture functions.
\n This functions marks the channel as busy.
\n The following parameters are set: PWD mode (input capture).

\param  [in]	The pointer pPWM_Cfg specifies the init structure for the channel.

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_BUSY counter in use

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
IO_ErrorType OCPWD_InitSync(IO_PWD_CfgType *pPWD_Cfg)
{
	/* pointer to the timer module */
	IO_ICU_CfgType icuCfg;
	IO_ErrorType returnInit;


	icuCfg.ICU_Id   = pPWD_Cfg->PWD_Id;
	icuCfg.inverted = pPWD_Cfg->inverted;

	returnInit = OCICU_InitSync(&icuCfg);

	SAFETY_REGION_START();

	if (returnInit == (IO_ErrorType)IO_E_OK)
	{
		/* Copy the timer structure cfg. */
		TMR_PwdDefinition[pPWD_Cfg->PWD_Id] = *pPWD_Cfg;
		TMR_PwdDefinition[pPWD_Cfg->PWD_Id].in_use = 1;
		TMR_PwdDefinition[pPWD_Cfg->PWD_Id].noTon = 0;		/* by default , measure of  Ton is enabled*/

#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		TMR_PwdChResource |= (1u<<pPWD_Cfg -> PWD_Id);
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
	}

	SAFETY_DATA_ELEM_UPDATE( TMR_PwdDefinition, pPWD_Cfg->PWD_Id );
	SAFETY_DATA_ELEM_UPDATE( PWD_CtrlVar, pPWD_Cfg->PWD_Id );

 	SAFETY_REGION_END();

	return returnInit;
}

/**
\n The function dispose of the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]	channel - the identifier of the channel

\return 	IO_ErrorType

\retval		IO_E_OK dispose of done
\retval		IO_E_INVALID_CHANNEL_ID
\retval     IO_E_UNEXPECTED_CONDITION

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
IO_ErrorType OCPWD_DeInitSync(IO_ChannelType channel)
{
	uint8_t hardwareTimerLinked;
	IO_ErrorType returnInit;
	uint8_t i;
	bool_t disbleHwISR;

	returnInit = OCICU_DeInitSync(channel);

	if (returnInit == (IO_ErrorType)IO_E_OK)
	{
		SAFETY_REGION_START();
		/* Copy the timer structure */
		(void)memset (&TMR_PwdDefinition[channel], 0, sizeof(IO_PWD_CfgType));

		SAFETY_DATA_ELEM_UPDATE( TMR_PwdDefinition, channel );
		SAFETY_REGION_END();
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
		TMR_PwdChResource &= (~(1u<<channel));
#endif /* #if defined(CONFIG_OCTPM_RUNTIME_CHECK) */
		/* Check if all pwd channels linked to an hardware timer are deinitialized */

		/* Detect Hardware timer linked to the channel */
		hardwareTimerLinked = TmrChMap[channel].hwCntrlId;

		disbleHwISR = TRUE;
		i=0;
		while( i < NUM_HW_CNTR_CH )
		{
			/* check if all pwd channels linked to the hardware timer are free */
			if (
#if defined(CONFIG_OCTPM_RUNTIME_CHECK)
					(TMR_PwdChResource & (1u<<i)) &&
#endif
					(TmrChMap[i].hwCntrlId == hardwareTimerLinked))
			{
				disbleHwISR = FALSE;
				i = NUM_HW_CNTR_CH; 
			}
			else
			{
				i++;
			}
		}
		if (disbleHwISR)
		{
			/*lint -e619 (Loss of precision arg. 3*/
			(void)OCHWCNTR_EnableISR(hardwareTimerLinked, 0, (Comm_Notification)NULL);
			/*lint +e619 */
		}

	}

	return returnInit;
}

/**
\n The function starts the pulse-width demodulation.
\n It uses the interrupts to read the times of the edges.
\n It uses the Hardware Timer Overflow/Underflow Interrupt linked to the channel.

\param  [in]	channel - the identificator of the channel
\param  [in]	callback - pointer to the callback function

\attention\b    OCHWCNTR_EnableISR() overwrite TMR_Callback[channel]

\return 	IO_ErrorType

\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned

\warning	If the the processor is slow and the duty cycle approches 0 % or 100 % then
            the pulses are narrow and it losses edges. An additional check should
            be put into a periodical time interrupt to see that the edges come. If the edges
            don't come (0 or 100%) than the driver doesn't recognize the state.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
IO_ErrorType OCPWD_StartSampleSync(IO_ChannelType channel, Comm_Notification callback)
{
	IO_ErrorType returnInit;

	/* Sets the rising  edge,
	   in case of inversion OCICU_SetModeSync() will provide falling edge setting */
	returnInit = OCICU_SetModeSync(channel, TMR_CH_IC_POS_EDGE);

	if (returnInit != (IO_ErrorType)IO_E_OK)
	{
		return returnInit;
	}

	/*SAFETY_REGION_START(); i dati sono modificata da isr */
	EnterCriticalSection();

	SAFETY_DATA_ELEM_CHECK( PWD_CtrlVar, channel );

	/* Init State of Capture */
	PWD_CtrlVar[channel].TMR_Status = PWD_FIRST_CAPT_EVENT;

	/* Saves the callback address */
	PWD_CtrlVar[channel].callback = callback;

	SAFETY_DATA_ELEM_UPDATE( PWD_CtrlVar, channel );

	ExitCriticalSection();
	/* SAFETY_REGION_END(); i dati sono modificata da isr */

	/* Programs the interrupt on linked HwCntr overflow/underflow */
	returnInit = OCHWCNTR_EnableISR(TmrChMap[channel].hwCntrlId, 1, OCPWD_HwTmrTurns);

	if (returnInit != (IO_ErrorType)IO_E_OK)
	{
		return returnInit;
	}

	/* Programs the interrupt on capture */
	returnInit = OCICU_EnableISR(channel, 1, OCPWD_EdgeTime);

	return returnInit;
}

/**
\n The function dispose of the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]		channel	- the identifier of the channel
\param  [in]  newFilterTime	- new desired filter time (in ns)

\return 	IO_ErrorType

\retval		IO_E_OK dispose of done
\retval		IO_E_INVALID_CHANNEL_ID

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
\note Do not use in interrupt
*/
IO_ErrorType OCPWD_SetFilterTime(IO_ChannelType channel, IO_TimingValueType newFilterTime)
{
	IO_ErrorType returnVal = (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;

	if (channel < NUM_HW_CNTR_CH)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( PWD_CtrlVar, channel );
		IO_TickResType timerRes = OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId);
		PWD_CtrlVar[channel].TMR_Filter = (newFilterTime / timerRes);
		returnVal = (IO_ErrorType) IO_E_OK;
		SAFETY_DATA_ELEM_UPDATE( PWD_CtrlVar, channel );
		SAFETY_REGION_END();
	}

	return returnVal;
}


/**
\n The function dispose of the channel of the timer. Its setting is returned to its default values.
\n If the channel is not in use, the function won't do anything.

\param  [in]		channel	- the identifier of the channel
\param  [in]  		bTon	- TRUE if Ton measure is enabled.

\return 	IO_ErrorType

\retval		IO_E_OK dispose of done
\retval		IO_E_INVALID_CHANNEL_ID

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
\note Do not use in interrupt
*/
IO_ErrorType OCPWD_SetTonMeasure(IO_ChannelType channel, bool8_t bTon)
{
	IO_ErrorType returnVal = (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;

	if (channel < NUM_HW_CNTR_CH)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( TMR_PwdDefinition, channel );
		TMR_PwdDefinition[channel].noTon = !(bTon==TRUE);
		returnVal = (IO_ErrorType) IO_E_OK;
		SAFETY_DATA_ELEM_UPDATE( TMR_PwdDefinition, channel );
		SAFETY_REGION_END();
	}

	return returnVal;
}


/**
\n The function is a local ISR that is called when an edge comes. This function calculates the
\n period of the signal (if non-inverted the period is between two raising edges, if inverted
\n than between two falling edges) and the lenght of the on-pulse (if non-inverted from the raising
\n to falling edge, if inverted from falling to rising).
\n The function generate a callback always when a new period edge comes.

\param  [in]    notification
\param  [in]	channel - the identifier of the channel
\param  [in]	error


\attention\b    the functions return's values are not tested to be faster, we are inside interrupt!!

\return 	IO_ErrorType

\retval		IO_E_OK

\warning	If the the processor is slow and the duty cycle approches 0 % or 100 % then
            the pulses are narrow and it losses edges. An additional check should
            be put into a periodical time interrupt to see that the edges come. If the edges
            don't come (0 or 100%) than the driver doesn't recognize the state.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
static IO_ErrorType OCPWD_EdgeTime(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error)
{
	uint8_t hwTmrID = TmrChMap[channel].hwCntrlId;
	/* Time of the edge */
	IO_HwTimerType time;
	bool_t turnPendingFlag;
	uint8_t pinIOlevel=0;

    (void) notification;
    (void) error;

	time = OCICU_GetTickSync(channel); 

	/* Check if HwTmr (linked to the channel) Overflow/Underflow pending flag active */
	
	/* 1) Get the pending flag */
    turnPendingFlag = TMR_GetTOF(TmrCntMap[hwTmrID], channel);

	SAFETY_DATA_ELEM_CHECK( pHwCntrCfgTable, TmrChMap[channel].hwCntrlId);

	SAFETY_DATA_ELEM_CHECK( TMR_PwdDefinition, channel );
	SAFETY_DATA_ELEM_CHECK( PWD_CtrlVar, channel );

#if !defined(CONFIG_OCPWD_DISABLE_PINCHK)

	/* PIN check: gets 5 I/O samples to validate level */
	if (!TMR_PwdDefinition[channel].noTon)
	{
		uint8_t i;

		for (i=0; i < 5; i++)
		{
			pinIOlevel += OCDIO_GetIOMapPinValueSync(&TmrChToPinMap[0], channel);
		}

		/* If set to falling edge, inverts detection */
		if (TMR_PwdDefinition[channel].inverted)
		{
			pinIOlevel = 5 - pinIOlevel;
		}
	}
	else
#endif
	/* PIN check disabled: it will always give expected value */
	if (PWD_CtrlVar[channel].TMR_Status != PWD_SECOND_CAPT_EVENT)
	{
		pinIOlevel = 5;
	}

	/* Which came first, the chicken or the egg? */
	if (turnPendingFlag && ( time < pHwCntrCfgTable[TmrChMap[channel].hwCntrlId]->period/2 ) )
	{
		/* We assume the overlow/underflow came first so PW_HwTmrTurns[] is not update
		   it will be update exing from this interrupt */
		PWD_CtrlVar[channel].HwTmrTurns++;
		/* Next Interrupt remember to dec. PW_HwTmrTurns[] */
		PWD_CtrlVar[channel].DecHwTmrTurns = TRUE;
	}
	else
	{
		/* No overlow/underflow or
		   overlow/underflow but time >= pHwCntrCfgTable[TmrChMap[channel].hwCntrlId]->period/2
		   and we assume that overlow/underflow came after edge */
		if ( TRUE == PWD_CtrlVar[channel].DecHwTmrTurns )
		{
			PWD_CtrlVar[channel].HwTmrTurns--;
			PWD_CtrlVar[channel].DecHwTmrTurns = FALSE;
		}
	}

	switch(PWD_CtrlVar[channel].TMR_Status)
	{
	case PWD_FIRST_CAPT_EVENT:
	{
		PWD_CtrlVar[channel].TMR_Start = time;
		/* Sets the falling  edge, in case of inversion ICU_SetModeSync() will provide rising. edge setting */

		if(TMR_PwdDefinition[channel].noTon)
		{
			PWD_CtrlVar[channel].TMR_Status = PWD_THIRD_CAPT_EVENT;
		}
		else
		{
			if (pinIOlevel > 2)
			{
				(void)OCICU_SetModeSync(channel, TMR_CH_IC_NEG_EDGE);
				PWD_CtrlVar[channel].TMR_Status = PWD_SECOND_CAPT_EVENT;
			}
		}

		PWD_CtrlVar[channel].HwTmrTurns = 0;

#if defined(OCTPM_PWD_TIMING_DEBUG)
		maxTonCntr[channel] = 0;
		minTonCntr[channel] = 0;
		maxPeriodCntr[channel] = 0;
		minPeriodCntr[channel] = 0;
#endif
	}
		break;

	case PWD_SECOND_CAPT_EVENT:
	{
		uint32_t tmpPeriod = OCPWD_GetIntervalInTicks(channel,time);

#if defined(OCTPM_PWD_TIMING_DEBUG)
		if ((PWD_DBG_MaxMeasure[channel].tOn < tmpPeriod) || (maxTonCntr[channel] == 0))
		{
			PWD_DBG_MaxMeasure[channel].tOn = tmpPeriod;
			maxTonCntr[channel] = 100;
		}
		else
		{
			maxTonCntr[channel]--;
		}

		if ((PWD_DBG_MinMeasure[channel].tOn > tmpPeriod) || (minTonCntr[channel] == 0))
		{
			PWD_DBG_MinMeasure[channel].tOn = tmpPeriod;
			minTonCntr[channel] = 100;
		}
		else
		{
			minTonCntr[channel]--;
		}
#endif

		if ( ( tmpPeriod >= PWD_CtrlVar[channel].TMR_Filter) && (pinIOlevel <= 2))
		{
			/* Check How many turns HwCounter did between first capture and the current capture */
			PWD_CtrlVar[channel].Measurement.tOn = tmpPeriod;
			/* Sets the rising  edge, in case of inversion ICU_SetModeSync() will provide falling edge setting */
			(void)OCICU_SetModeSync(channel, TMR_CH_IC_POS_EDGE);

			PWD_CtrlVar[channel].TMR_Status = PWD_THIRD_CAPT_EVENT;
		}
	}
		break;

	case PWD_THIRD_CAPT_EVENT:
	{
		uint32_t tmpPeriod = OCPWD_GetIntervalInTicks(channel,time);
		uint32_t tmpRef = PWD_CtrlVar[channel].TMR_Filter;

		if (!TMR_PwdDefinition[channel].noTon)
		{
			tmpRef += PWD_CtrlVar[channel].Measurement.tOn;
		}

#if defined(OCTPM_PWD_TIMING_DEBUG)
		if ((PWD_DBG_MaxMeasure[channel].period < tmpPeriod) || (maxPeriodCntr[channel] == 0))
		{
			PWD_DBG_MaxMeasure[channel].period = tmpPeriod;
			maxPeriodCntr[channel] = 100;
		}
		else
		{
			maxPeriodCntr[channel]--;
		}

		if ((PWD_DBG_MinMeasure[channel].period > tmpPeriod) || (minPeriodCntr[channel] == 0))
		{
			PWD_DBG_MinMeasure[channel].period = tmpPeriod;
			minPeriodCntr[channel] = 100;
		}
		else
		{
			minPeriodCntr[channel]--;
		}
#endif

		if ((pinIOlevel > 2) && (tmpPeriod >= tmpRef))
		{
			/* period calculation */
			PWD_CtrlVar[channel].Measurement.period = tmpPeriod;

			PWD_CtrlVar[channel].TMR_Start = time;

			/* Reset  # overflows/underflows */
			PWD_CtrlVar[channel].HwTmrTurns = 0;

			if(TMR_PwdDefinition[channel].noTon)
			{
				/* Dummy value set as half period only for compatibility */
				PWD_CtrlVar[channel].Measurement.tOn = tmpPeriod/2;
			}
			else
			{
				/* Sets the falling  edge, in case of inversion ICU_SetModeSync() will provide rising. edge setting */
				(void)OCICU_SetModeSync(channel, TMR_CH_IC_NEG_EDGE);

				PWD_CtrlVar[channel].TMR_Status = PWD_SECOND_CAPT_EVENT;
			}

			/* Informs the user that a new measure is ready */
			if( PWD_CtrlVar[channel].callback != NULL )
			{
				(void)(PWD_CtrlVar[channel].callback)((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)channel, (IO_ErrorType)IO_E_OK);
			}

		}
	}

		break;

	default:
		PWD_CtrlVar[channel].TMR_Status = PWD_FIRST_CAPT_EVENT;
		break;
	}

	SAFETY_DATA_ELEM_UPDATE( PWD_CtrlVar, channel );

	return (IO_ErrorType)IO_E_OK;
}
/**
\n The function is a local ISR that is called when the hardware timer overflow/underflow comes.
\n This function update number of overflow/underflow.

\param  [in]		notification unused
\param  [in]		channel Hardware Timer Id
\param  [in]	    error unused

\attention \b       PWD_HwTmrTurns[i] is a free running counter

\return 	IO_ErrorType

\retval		IO_E_INVALID_CHANNEL_ID
\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
static IO_ErrorType OCPWD_HwTmrTurns(IO_NotificationType notification, IO_ChannelType tmdId, IO_ErrorType error)
{
	uint8_t i;

    (void) notification;
    (void) error;
	
	for(i=0; i < NUM_HW_CNTR_CH; i++ ) /* per tutti i canali*/
	{
		/* channel belongs to the timer and is in use */
		if (TmrChMap[i].hwCntrlId == tmdId && TMR_PwdDefinition[i].in_use)
		{
			SAFETY_DATA_ELEM_CHECK(PWD_CtrlVar, i);

			/* increment PWD_HwTmrTurns */
			PWD_CtrlVar[i].HwTmrTurns++;
			/* Check if PWD_HwTmrTurns goes overflow */
			if (PWD_CtrlVar[i].HwTmrTurns == 0)
			{
				/* Invalid measure - Reset PWD state */
				PWD_CtrlVar[i].TMR_Status = PWD_FIRST_CAPT_EVENT;
				OCICU_SetModeSync(i, TMR_CH_IC_POS_EDGE);
			}

			SAFETY_DATA_ELEM_UPDATE(PWD_CtrlVar, i);
		}
	}
	return (IO_ErrorType)IO_E_OK;
}

/**
\n Calculates the time interval between two edge in accordance with the formula:
\n timeInt = lastCapt + nHwTmrTurns*HwTmrModulo - firstCapt
\n where:
\n lastCapt is the last capture in ticks
\n nHwTmrTurn is the number of overflows/underflows of the hardware timer
\n HwTmrModulo is the modulo of the hardware timer
\n firstCapt is the first capture in ticks

\param  [in]		time last capture in ticks
\param  [in]		channel id of the PWD channel

\attention \b  possible overflow in calculation

\return 	uint32_t
\retval		interval in ticks

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
static uint32_t OCPWD_GetIntervalInTicks(IO_ChannelType channel, IO_HwTimerType time)
{
	uint32_t intervalInTicks=0;

	if( PWD_CtrlVar[channel].HwTmrTurns > 0 ){
		uint32_t periodo = pHwCntrCfgTable[TmrChMap[channel].hwCntrlId]->period;

		intervalInTicks  = (periodo - PWD_CtrlVar[channel].TMR_Start) + time ;
		intervalInTicks += (periodo * (PWD_CtrlVar[channel].HwTmrTurns-1) );
	}else{
		intervalInTicks = time - PWD_CtrlVar[channel].TMR_Start;
	}
	return(intervalInTicks);
}

/**
\n The function returns the measured PW data (on-time and period). These values are converted into nSec.

\param  [in]	channel - the identifier of the channel
\param [out]	measurement		(IO_PWD_MeasureType) PWD structure - period and on-time value

\attention\b Correct tick resolution must be filled in the init structure of the counter

\return 	none

\note	    probably it's necessary to introduce a critical sectione entry and exit\n
            to ensure the integrity of the information.
            No if the functions is called inside notification's callback

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
void OCPWD_GetAllSync(IO_ChannelType channel, IO_PWD_MeasureType *measurement)
{
	/* Resolution of the timer */
	IO_TickResType timerRes;

	/* tick resolution of the counter */
	timerRes = OCHWCNTR_GetTickResolution(TmrChMap[channel].hwCntrlId);

	/* Converts the data into nSec */
	measurement->period = (IO_TimingValueType)PWD_CtrlVar[channel].Measurement.period * (IO_TimingValueType)timerRes;
	measurement->tOn = (IO_TimingValueType)PWD_CtrlVar[channel].Measurement.tOn * (IO_TimingValueType)timerRes;

}

/**
\n The function returns the measured PW data (on-time and period). These values are in ticks.

\param  [in]	channel - the identificator of the channel
\param	[out]	measurement - (IO_PWD_RawMeasureType) period and on-time value in ticks

\return 	IO_PWD_RawMeasureType

\retval		measurement (period and on-time value)

\note	    probably it's necessary to introduce a critical section entry and exit\n
            to ensure the integrity of the information.
            No if the functions is called inside notification's callback

\par Architecture Requirements ID: SARS_WMMT_MN_DrvTMR_5
*/
void OCPWD_GetAllSyncInTicks(IO_ChannelType channel, IO_PWD_RawMeasureType *measurement)
{
	measurement->period = PWD_CtrlVar[channel].Measurement.period;
	measurement->tOn    = PWD_CtrlVar[channel].Measurement.tOn;
}

/**
\n The function stops the pulse-width demodulation.

\param  [in]	channel - the identifier of the channel

\return IO_ErrorType
\retval		IO_E_OK initialization done
\retval		IO_E_INVALID_CHANNEL_ID invalid counter identificator
\retval		IO_E_UNEXPECTED_CONDITION channel is not assigned
*/
IO_ErrorType OCPWD_StopSampleSync(IO_ChannelType channel)
{
	/* Stops the interrupt */

	/*lint -e619 (Loss of precision arg. 3*/
	return OCICU_EnableISR(channel, 0, (Comm_Notification)NULL);
	/*lint +e619 */
}

/********************************************** Timer PWD - end ************************************************/

/**
\n ISR of the channel, This function calls the callback function defined by the user.

\param  [in]	channel - the identifier of the channel

\attention\b    This function must be called inside interrupt routine

\todo       For MCU with events on both thresholds: identify the source
*/
void HWTMRCH_ISR(IO_ChannelType channel)
{
	IRQ_MONITOR_ENTER();

	if( channel < NUM_HW_CNTR_CH )
	{	
	/* pointer to the timer module */
		TMR_CNTAddressType pTmr_ch = TmrChMap[channel].Channel;

		if( TMR_CH_Callback[channel] != 0 )
		{
			/* Calls the user callback */
			(void)(TMR_CH_Callback)[channel]((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)channel, (IO_ErrorType)IO_E_OK);
		}

		/* Clears the interrupt flag */
		TMR_ClearCHF(pTmr_ch);

	}

	IRQ_MONITOR_EXIT();
}

/**
\n The function returns the timer channel linked to a specified pin.

\param  [in]	pIOMap fixed a pin index specifies the pointer to the Module IO Map.
\param  [in]	pChannel pointer where is stored the timer channel

\return 	IO_ErrorType
\retval		IO_E_OK
\retval     IO_E_UNEXPECTED_CONDITION
*/
IO_ErrorType OCTPM_GetChannelSync(const IO_ModuleIOMapType *const pIOMap,IO_ChannelType *pChannel)
{
	uint8_t ch;

	if(!pIOMap)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}

	if ( OCTMR_GetChannel(pIOMap->PortAddress,pIOMap->PinMask,&ch) )
	{
		*pChannel = ch;
		return (IO_ErrorType)IO_E_OK;
	}
	return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
}

/*
This function get the timer id from channel id
\param [in]  chID
\param [out] pTmrId

\return IO_ErrorType
\retval IO_E_OK if found
\retval IO_E_UNEXPECTED_CONDITION if not found
 */
IO_ErrorType OCTPM_GetTimerIDFromChannelID(int chID, uint8_t* pTmrId)
{
	if( chID >= NUM_HW_CNTR_CH ){
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
	
	*pTmrId = TmrChMap[chID].hwCntrlId;
	
	return (IO_ErrorType)IO_E_OK;
}

uint8_t OCTPM_GetTimerIDFromChannelID_1(uint8_t chID )
{
	return  TmrChMap[chID].hwCntrlId;
}


/*
The function search for TMR channel starting from portAddress and pinMask (read pin)
\param [in]  portAddress
\param [in]  pinMask
\param [out]  pTMRch  the channel ID found

\return bool_t	TRUE if found
 */
bool_t OCTMR_GetChannel(DIO_ModuleMapPortAddressType portAddress,	DIO_ModuleMapMaskType pinMask, uint8_t *pTMRch)
{
	uint8_t j;
	bool_t channelfound = FALSE;

	for (j = 0; j < NUM_HW_CNTR_CH; j++) 
	{
		if ( ModuleMapPortAddressMatch(TmrChToPinMap[j].PortAddress,portAddress) && ModuleMapPortAddressIsValid(TmrChToPinMap[j].PortAddress))
		{
			if (TmrChToPinMap[j].PinMask == pinMask) 
			{
				channelfound = TRUE;
				*pTMRch = j;
				break;
			}/*end if*/
		}/*end if*/
	}/*end for*/
	
	return channelfound;
} /*end TMR_GetChannel*/

/**
The function get the channel id prom pin id
\param [in] pinID

\return   channel id
\retval   0xFF if not found
 */
uint8_t OCTMR_GetChannelIDFromPinID(uint8_t pinID)
{
	uint8_t chId=0xFF;
	const IO_ModuleIOMapType *pinMap = OCDIO_GetIOMapItem(pinID);
	uint8_t j;

	for(j=0; j < NUM_HW_CNTR_CH; j++)
	{
		if ( ModuleMapPortAddressMatch(TmrChToPinMap[j].PortAddress,pinMap->PortAddress) && ModuleMapPortAddressIsValid(TmrChToPinMap[j].PortAddress))
		{
			if(  TmrChToPinMap[j].PinMask == pinMap->PinMask )
			{
				chId = j;
				break;
			}
		}
	}
	return chId;
}

/*
The function get the pin id from channel id

\param [in] chID
\return   pin id
\retval   0xFF if not found

\note please avoid calling this method in ISR because the for statement could
 */
uint8_t OCTPM_GetPinIDFromChannelID(int chID )
{
	const TMR_MapType * pinPtr;
	int i;

	if( chID < NUM_HW_CNTR_CH )
	{
		pinPtr = &TmrChToPinMap[chID];

		if (ModuleMapPortAddressIsValid(pinPtr->PortAddress))
		{
			for( i=0; i<IO_MODULE_MAP_SIZE; i++)
			{
				const IO_ModuleIOMapType *pinMap = OCDIO_GetIOMapItem(i);

				if( (ModuleMapPortAddressMatch(pinMap->PortAddress, pinPtr->PortAddress))
				&& (pinMap->PinMask == pinPtr->PinMask) )
				{
					return i;
				}
			}
		}
	}
	return 0xFF; /* invalid pin*/
}
/* Local Function */

/* *********** */
/* End of file */
/* *********** */


