/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
 * \page pageFreqFeedb
 *\ingroup	PM_FrequencyFeedbacks
 */
/**
\n
\par		Frequency Feedbacks
\n			TODO
\n
\file		PM_FrequencyFeedbacks.c
\ingroup	PM_FrequencyFeedbacks
\date		12/06/2012
\author		Roberto Fioravanti
\n
\n
\n
\n etc.
*/
#define MODULE_BODY_FREQFB
/* ******** */
/* Includes */
/* ******** */
#define __PM_FREQUENCY_FEEDBACKS

#include "string.h"
#include "OCDIO_drv.h"
#include "OCTPM_drv.h"
#include "PM_FrequencyFeedbacks.h"
#include "LIB_ZeroCrossing.h"
#include "IOSharing.h"
#include "DigitalInputs.h"
#include "LIB_Trace.h"
#include "Lib_ClassBVar.h"
#include "LIB_Error.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#ifndef CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE
#define CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE 		(0)
#endif

#ifndef CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER
#define CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER		(1)
#endif

/** Key for global static structure of the module.*/
#define PM_FREQUENCY_FEEDBACKS_KEY						( ((uint8_t)CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER<<8)|((uint8_t)~CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER&0xFF) )
/** Type ID for frequency feedback local structure. use to validate the type of the structure*/
#define PM_FREQUENCY_FEEDBACK_ID						((uint8_t)(CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER))


#define PM_FREQUENCY_FEEDBACKS_ASSERT_LOCAL(Exp, ErrVar, ErrCode)		\
															if (!(Exp)) \
																{ErrVar = (ErrCode); }

/** Frequency feedback sampling flag */
#define PM_FF_ITEM_CTRL_SAMPLE_ENABLED					(1<<0)
/** Frequency feedback availability flag*/
#define PM_FF_ITEM_CTRL_UNAVAILABLE						(1<<1)

/** If set the result of the sampling is available */
#define PM_FF_ITEM_STATUS_SAMPLES_READY					(1<<0)
/** If set the last polling value is high (1) */
#define PM_FF_ITEM_STATUS_SAMPLE_HIGHVAL				(1<<1)
/** If set end of frequency detection reached */
#define PM_FF_ITEM_STATUS_END_OF_FREQ					(1<<2)
/** If set the result of the frequency detection is available */
#define PM_FF_ITEM_STATUS_FREQ_READY					(1<<3)
/** If set frequency detection is on going */
#define PM_FF_ITEM_STATUS_PULSE							(1<<4)


/**/
#define PM_FF_50HZ_MAX_PULSES_NUM            			(9)
#define PM_FF_60HZ_MAX_PULSES_NUM            			(11)
#define PM_FF_HILO_FREQ_MAX_PULSES_NUM       			(8)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
@name General types
@{
*/

/**
Processing status
*/
typedef enum ePM_FrequencyFeedbackProcessStatus
{
	/** Processing terminated */
	PM_FREQUENCY_FEEDBACK_PROCESS_END = 0,
	/** Processing ongoing */
	PM_FREQUENCY_FEEDBACK_PROCESS_WORKING,
	/** Error */
	PM_FREQUENCY_FEEDBACK_PROCESS_ERROR
}PM_FrequencyFeedbacks_ProcessStatusType;



/**
 *	mask
 */
typedef PM_FrequencyFeedbacks_BitmapType	PM_FrequencyFeedbacks_MaskType;

/**
 *	index
 */
typedef int8_t	PM_FrequencyFeedbacks_IndexType;
/* @} */


/**
@name Module descriptor types used for variables common to the whole module
@{
*/
/**
 * Number of items
 */
typedef uint8_t PM_FrequencyFeedbacks_NumberType;
/**
 * Grid Frequency
 */
typedef uint8_t PM_FrequencyFeedbacks_GridFrequencyType;
/**
 * Key
 */
typedef uint16_t PM_FrequencyFeedbacks_InitKeyType;
/* @} */


/**
@name Feedback descriptor
Variables common to the whole module
@{
*/

/**
pin
 */
typedef uint8_t 		PM_FreqFeedback_PinType;
/**
channel
 */
typedef IO_ChannelType	PM_FreqFeedback_TpmChannelType;
/**
Number of samples
 */
typedef uint8_t 		PM_FreqFeedback_SamplesNumberType;
/**
 * Number of samples
 */
typedef int16_t			PM_FreqFeedback_SamplingRestartTimerType;
/**
 * Polling value
 */
typedef uint8_t			PM_FreqFeedback_PollingValueType;
/**
 * window
 */
typedef uint8_t			PM_FreqFeedback_ProgressType;

/**
 *	control
 */
typedef uint8_t			PM_FreqFeedback_ControlType;
/**
 *	status
 */
typedef uint8_t			PM_FreqFeedback_StatusType;
/**
 *	status
 */
typedef uint8_t			PM_FreqFeedback_MaxPeriodCounterType;
/**
 * Structure ID
 */
typedef uint8_t 		PM_FreqFeedback_StructIDType;
/* @} */



/**
 * Item's descriptor
 */
typedef struct PM_FrequencyFeedback_s
{
	/** */
	PM_FreqFeedback_PinType						pin;
	/** */
	PM_FreqFeedback_TpmChannelType				tpmchannel;
	/** number of requested samples */
	PM_FreqFeedback_SamplesNumberType			requested_samples;
	/** number of current samples */
	PM_FreqFeedback_SamplesNumberType			samples;
	/** number of samples @high level*/
	PM_FreqFeedback_SamplesNumberType			hlsamples;
	/** */
	PM_FreqFeedback_ProgressType				progress;
	/** */
	PM_FreqFeedback_SamplingRestartTimerType	timetorestart;
	/** position in ram */
	PM_FrequencyFeedback_PositionType			position;
	/** control */
	PM_FreqFeedback_ControlType					control;
	/** control */
	PM_FreqFeedback_StatusType					status;
	/** frequency event pending flag */
	PM_FreqFeedback_StatusType					freq_evt;
	/** */
	PM_FreqFeedback_MaxPeriodCounterType		maxperiod;
	/** */
	PM_FreqFeedback_StructIDType				ID;
}PM_FrequencyFeedbackType;

/**
Global module descriptor
This structure is not an interface as it could be changed without notice.
DO NOT export variables with this structure.
*/
typedef struct PM_FrequencyFeedbacks_s
{
	/** currently configured items */
	PM_FrequencyFeedbacks_NumberType			number;
	/** current error */
	/** key */
	PM_FrequencyFeedbacks_InitKeyType			key;
	/** */
	PM_FrequencyFeedbacks_GridFrequencyType		gridfrequency;
	/** Bitmap indicating if ready. Each bit corresponds to the position */
	PM_FrequencyFeedbacks_BitmapType			ready;
	/** Bitmap indicating if all values are reliable that is all feedback has been acquired at least once */
	PM_FrequencyFeedbacks_BitmapType			reliable;
	/** Bitmap indicating all existing frequency feedbacks */
	PM_FrequencyFeedbacks_BitmapType			positionmask;
	/** */
	PM_FrequencyFeedbackType 					item[CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER];
}PM_FrequencyFeedbacksType;

/**
Period in ns
*/
typedef unsigned long PM_FrequencyFeedbacks_PeriodFilterType;
/**
T-On in ns
*/
typedef unsigned long PM_FrequencyFeedbacks_TOnFilterType;

/* ********* */
/* Variables */
/* ********* */

/* Global */
/* global variable for the application*/
PM_FrequencyFeedbacks_InterfaceType	PM_FrequencyFeedbacks_Interface;
/* Static */
static PM_FrequencyFeedbacksType PM_FrequencyFeedbacks;
#if defined(CONFIG_FREQUENCY_FEEDBACKS_NOFREQ_MONITOR)
uint16_t	PM_FreqFeeback_NoFreqMonitor[CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER];
#endif
/**
Period for supported grid frequencies [ns]
 */
static const  PM_FrequencyFeedbacks_PeriodFilterType PM_FrequencyFeedbacks_PeriodFilter[2]=
{
	2*MIN_HALF_PERIOD_SYNC_60HZ*1000,
	2*MIN_HALF_PERIOD_SYNC_50HZ*1000
};
/**
T-On for supported grid frequencies [ns].
 */
static const  PM_FrequencyFeedbacks_TOnFilterType PM_FrequencyFeedbacks_TOnFilter[2]=
{
	((MIN_HALF_PERIOD_SYNC_60HZ*CONFIG_FREQUENCY_FEEDBACKS_TON_TH)/100)*1000,
	((MIN_HALF_PERIOD_SYNC_50HZ*CONFIG_FREQUENCY_FEEDBACKS_TON_TH)/100)*1000
};
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
static void PM_FrequencyFeedbacks_Populate(PM_FrequencyFeedbackType* _pFrequencyFeedback, PM_FreqFeedback_PinType _Pin, IO_ChannelType _TpmCh, PM_FreqFeedback_SamplesNumberType _Samples, PM_FrequencyFeedback_PositionType _Position);
static IO_ErrorType PM_FrequencyFeedbacks_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error );
static PM_FrequencyFeedbacks_IndexType PM_FrequencyFeedbacks_GetIndexByTimerCh(IO_ChannelType _TpmCh);
static void PM_FrequencyFeedbacks_SamplesCollector(PM_FrequencyFeedbackType* _pItem);
static void PM_FrequencyFeedbacks_PulsesCollector(PM_FrequencyFeedbackType* _pItem);
static void  PM_FrequencyFeedbacks_Process(PM_FrequencyFeedbackType* _pItem, bool_t _Disabled);
static PM_FrequencyFeedbacks_TOnFilterType PM_FrequencyFeedbacks_GetMaxTOnFilterValue (void);
static PM_FrequencyFeedbacks_PeriodFilterType PM_FrequencyFeedbacks_GetMaxPeriodFilterValue (void);
static PM_FrequencyFeedbacks_IndexType PM_FrequencyFeedbacks_GetIndexByPosition(PM_FrequencyFeedback_PositionType _Position);
static void PM_FrequencyFeedbacks_RestartFeedbackSampling(PM_FrequencyFeedbackType * _pItem);
static void PM_FrequencyFeedbacks_RestartFeedbackPulse(PM_FrequencyFeedbackType * _pItem);
static void PM_FrequencyFeedbacks_RestartFeedback(PM_FrequencyFeedbackType * _pItem);
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Initialization.
Called Once

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void PM_FrequencyFeedbacks_Init(void)
{
	int i = 0;
#if (CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE)
	/* do nothing */
#else
	SAFETY_REGION_START();

	memset(&PM_FrequencyFeedbacks, 0, sizeof(PM_FrequencyFeedbacks));

	/* init key */
	PM_FrequencyFeedbacks.key = PM_FREQUENCY_FEEDBACKS_KEY;
	/* init current grid frequency */
	PM_FrequencyFeedbacks.gridfrequency = NOM_FREQ_IS_UNDEFINED;
	/* init items */
	for(i=0; i<CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER; i++)
	{
		memset(&(PM_FrequencyFeedbacks.item[i]), 0, sizeof(PM_FrequencyFeedbackType));
		/* force plausibility check on 'ID' to fail */
		PM_FrequencyFeedbacks.item[i].ID = ~PM_FREQUENCY_FEEDBACK_ID;
	}

	/* Variable interface  initialization */
	memset(&PM_FrequencyFeedbacks_Interface, 0, sizeof(PM_FrequencyFeedbacks_Interface));

	for(i=0; i<CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER; i++)
	{
		/* init feedback to 'unreliable value' */
		PM_FrequencyFeedbacks_Interface.feedback[i] = (PM_FrequencyFeedbacks_ValueType)PM_FREQUENCY_FEEDBACKS_UNRELIABLE_VALUE;
	}
	SAFETY_DATA_UPDATE( PM_FrequencyFeedbacks_Interface );
	SAFETY_REGION_END();
#endif /* CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE) */
}

/**
Configuration
\param [in]	_Position position from 0 to sizeof(PM_FrequencyFeedbacks_BitmapType)*8-1


\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
IO_ErrorType PM_FrequencyFeedbacks_Configure(uint8_t _Pin, const freqFeedbackLLIparams_t* _CfgParams, PM_FrequencyFeedback_PositionType _Position)
{
	IO_ErrorType 	result 	= IO_E_INVALID_CHANNEL_ID;
	IO_ChannelType 	tpmCh	= 0xFF /* invalid */;
	IO_PWD_CfgType  pwdcfg	= {0,0};
	uint8_t 		activeSense = 0;
	PM_FreqFeedback_SamplesNumberType samples = 0;

#if (CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE)
	return IO_E_PARAM_IGNORED;
#else
	/**/

	/*plausibility check on 'key' */
	/*lint -e514*/
	PLT_ERROR_FATAL_IF((PM_FrequencyFeedbacks.key != (PM_FrequencyFeedbacks_InitKeyType)PM_FREQUENCY_FEEDBACKS_KEY), PM_FREQUENCY_FEEDBACKS_NOINIT_ERR);
	/*lint +e514*/

	/*plausibility check on 'number'*/
	PLT_ERROR_FATAL_IF(!(PM_FrequencyFeedbacks.number < CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER), PM_FREQUENCY_FEEDBACKS_TOOMANY_ERR)

	/* validate pin */
	if(OCDIO_IsValidPin(_Pin) != ( IO_ErrorType )IO_E_OK)
	{
		return IO_E_FAIL;
	}
	/* settings */
	if (_CfgParams->bit.pullUp )
	{
		(void)OCDIO_SetPinPullUpSync(_Pin);
	}
	activeSense = ((_CfgParams->bit.inverted)?1:0);
	LIB_IOM_Set_IODir_ActiveSense(activeSense,_Pin);

	/* retrieving PWD channel*/
	if ( OCTPM_GetChannelSync ( OCDIO_GetIOMapItem(_Pin), &tpmCh ) != ( IO_ErrorType ) IO_E_OK )
	{
		PLT_ERROR_FATAL(PM_FREQUENCY_FEEDBACKS_CANNOTRETRIEVETPMCH_ERR);
		return IO_E_FAIL;
	}

	/* get number of samples samples */
	samples = 1<<(*((uint8_t*)_CfgParams) & 0x7F);
	if ( (samples > 16) || (samples == 0))
	{
		PLT_ERROR_FATAL(PM_FREQUENCY_FEEDBACKS_EXCEEDINGMAXSAMPLE_ERR);
		return IO_E_FAIL;
	}

	/* check position range */
	PLT_ERROR_FATAL_IF(!(_Position <= (sizeof(PM_FrequencyFeedbacks_BitmapType)*8-1)), PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR)

	SAFETY_REGION_START();

	pwdcfg.PWD_Id = tpmCh;

	/* hardware initializations */
	/* PWD*/
	if ( OCPWD_InitSync ( &pwdcfg ) == ( IO_ErrorType ) IO_E_OK )
	{
#if defined(CONFIG_FREQUENCY_FEEDBACKS_PWD_FILTER_TIME)
		OCPWD_SetFilterTime(pwdcfg.PWD_Id, CONFIG_FREQUENCY_FEEDBACKS_PWD_FILTER_TIME);
#endif
#if defined(CONFIG_FREQUENCY_FEEDBACKS_DIGITAL_FILTER_FREQ)
		OCDIO_SetIODigitalFilterFrequencySync(_Pin, CONFIG_FREQUENCY_FEEDBACKS_DIGITAL_FILTER_FREQ);
		OCDIO_SetIODigitalFilterPinSync(_Pin);
#endif

		/*Populate and start feedback*/
		PM_FrequencyFeedbacks_Populate(&PM_FrequencyFeedbacks.item[PM_FrequencyFeedbacks.number], _Pin, tpmCh, samples, _Position);
		PM_FrequencyFeedbacks_RestartFeedback(&PM_FrequencyFeedbacks.item[PM_FrequencyFeedbacks.number]);
		PM_FrequencyFeedbacks.positionmask |= (PM_FrequencyFeedbacks_BitmapType)(1<<_Position);
		PM_FrequencyFeedbacks.number++;

		result = IO_E_OK;
	}

	SAFETY_REGION_END();
	return result;
#endif /* CONFIG_FREQUENCY_FEEDBACKS_DISABLE_MODULE) */
}

/**
Resets all feedback variables.
To be called inside a safety region
\code
SAFETY_REGION_START();
SAFETY_REGION_END();
\endcode
\param [in] _Position position in ram of the feedback

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void PM_FrequencyFeedbacks_ResetAllFeedbacks(void)
{
	PM_FrequencyFeedbacks_IndexType inputindex = -1;

	PM_FrequencyFeedbacks.ready = 0;

	for(inputindex = 0; inputindex < PM_FrequencyFeedbacks.number; inputindex ++)
	{
		PM_FrequencyFeedbacks_RestartFeedback(&PM_FrequencyFeedbacks.item[inputindex]);
	}
}


/**
Resets feedback given its ram position

\param [in] _Position position in ram of the feedback

\return IO_ErrorType
\retval	IO_E_INVALID_POSITION invalid value for argument _Position
\retval IO_E_UNEXPECTED_CONDITION internal error : invalid index
\retval IO_E_FAIL driver error
\retval IO_E_OK	no error

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
IO_ErrorType PM_FrequencyFeedbacks_RestartFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position)
{
	IO_ErrorType retVal=IO_E_OK;
	PM_FrequencyFeedbacks_IndexType findex = -1;

	if(_Position >= CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER)
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR);
		return IO_E_INVALID_POSITION;
	}

	findex = PM_FrequencyFeedbacks_GetIndexByPosition(_Position);

	if(!((findex >= 0) && (findex < PM_FrequencyFeedbacks.number)))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDINDEXRETRIEVED_ERR);
		return IO_E_UNEXPECTED_CONDITION;
	}

	SAFETY_REGION_START();

	/* update */
	PM_FrequencyFeedbacks.item[findex].control |= (PM_FF_ITEM_CTRL_UNAVAILABLE);

	/* forces not ready */
	PM_FrequencyFeedbacks.ready &= ~ ((PM_FrequencyFeedbacks_BitmapType)(1u << _Position));
	PM_FrequencyFeedbacks_RestartFeedback(&PM_FrequencyFeedbacks.item[findex]);

	PM_FrequencyFeedbacks.item[findex].control &= ~(PM_FF_ITEM_CTRL_UNAVAILABLE);

	SAFETY_REGION_END();
	return  retVal;
}


/**
The feedback will be disabled and not updated

\param [in] _Position position in ram of the feedback

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
IO_ErrorType PM_FrequencyFeedbacks_DisableFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position)
{
	PM_FrequencyFeedbacks_IndexType findex = -1;

	if(!((_Position < CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER)))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR);
		return IO_E_INVALID_POSITION;
	}

	findex = PM_FrequencyFeedbacks_GetIndexByPosition(_Position);

	if(!(findex >= 0) && (findex < PM_FrequencyFeedbacks.number))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDINDEXRETRIEVED_ERR);
		return IO_E_UNEXPECTED_CONDITION;
	}

	SAFETY_REGION_START();

	PM_FrequencyFeedbacks_Interface.disabled |= ((PM_FrequencyFeedbacks_BitmapType)(1u << PM_FrequencyFeedbacks.item[findex].position));

	SAFETY_REGION_END();
	return IO_E_OK;
}


/**
The feedback will be enabled

\param [in] _Position position in ram of the feedback

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
IO_ErrorType PM_FrequencyFeedbacks_EnableFeedbackByPosition(PM_FrequencyFeedback_PositionType _Position)
{
	PM_FrequencyFeedbacks_IndexType findex = -1;

	if(!((_Position < CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER)))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR);
		return IO_E_INVALID_POSITION;
	}

	findex = PM_FrequencyFeedbacks_GetIndexByPosition(_Position);

	if(!(findex >= 0) && (findex < PM_FrequencyFeedbacks.number))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDINDEXRETRIEVED_ERR);
		return IO_E_UNEXPECTED_CONDITION;
	}

	SAFETY_REGION_START();

	PM_FrequencyFeedbacks_Interface.disabled &= ~((PM_FrequencyFeedbacks_BitmapType)(1u << PM_FrequencyFeedbacks.item[findex].position));

	SAFETY_REGION_END();
	return IO_E_OK;
}

/**
Given the frequency feedbacks position returns if its value is ready.
\param [in]	_Position	position of the feedback
\retvalue 	TRUE ready
			FALSE not ready or _Position is out of range
\return		bool_t
\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t PM_FrequencyFeedbacks_GetReadyByPosition(PM_FrequencyFeedback_PositionType _Position)
{
	bool_t retVal = FALSE;

	if(!((_Position < CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER)))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR);
	}
	else
	{
		retVal = ((PM_FrequencyFeedbacks.ready & (PM_FrequencyFeedbacks_BitmapType)(1<<_Position)) != 0) &&
				(PM_FrequencyFeedbacks_Interface.feedback[_Position] != PM_FREQUENCY_FEEDBACKS_UNRELIABLE_VALUE);
	}

	return retVal;
}

/**
Returns the frequency feedbacks bitmap to evaluate the complete state.

\return		PM_FrequencyFeedbacks_BitmapType
\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
PM_FrequencyFeedbacks_BitmapType PM_FrequencyFeedbacks_GetReadyRaw(void)
{
	return PM_FrequencyFeedbacks.ready;
}

/**
Returns if Frequency feedbacks are reliable.
\retvalue 	TRUE reliable
			FALSE not reliable
\return		bool_t
\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
bool_t PM_FrequencyFeedbacks_Reliable(void)
{
	return (bool_t)( !(PM_FrequencyFeedbacks.reliable ^ PM_FrequencyFeedbacks.positionmask))?TRUE:FALSE;
}


/**
Retrieves the feedback value by position

\param [in]		_Position			position (zero based)
\param [out]	_pValue				value
\return	IO_ErrorType
\retval	FREQFB_E_INVALID_POSITION position is out of range
 	 	FREQFB_E_FAIL	 invalid pointer _pValue
 	 	FREQFB_OK	Successful

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
PM_FrequencyFeedback_Result PM_FrequencyFeedbacks_GetValueByPosition(PM_FrequencyFeedback_PositionType _Position, PM_FrequencyFeedbacks_ValueType* _pValue)
{
	if(!(_Position < CONFIG_FREQUENCY_FEEDBACKS_MAX_ITEMS_NUMBER))
	{
		PLT_ERROR_WARNING(PM_FREQUENCY_FEEDBACKS_INVALIDPOSITION_ERR);
		return FREQFB_E_INVALID_POSITION;
	}

	if(_pValue)
	{
		*_pValue = PM_FrequencyFeedbacks_Interface.feedback[_Position];
	}
	else
	{
		return FREQFB_E_FAIL;
	}
	return FREQFB_OK;
}



/**
Frequency feedback manager.
Must be run every 10 ms.
Ensure periodical call to this function for safety.

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void PM_FrequencyFeedbacks_Manager(void)
{
	uint8_t frequency = NOM_FREQ_IS_UNDEFINED;
	PM_FrequencyFeedbacks_IndexType inputindex = -1;
	PM_FrequencyFeedbacks_MaskType	mask = 0;

	/* Plausibility check on PM_FrequencyFeedbacks*/
	PLT_ERROR_FATAL_IF((PM_FrequencyFeedbacks.key != (PM_FrequencyFeedbacks_InitKeyType)PM_FREQUENCY_FEEDBACKS_KEY), PM_FREQUENCY_FEEDBACKS_NOINIT_ERR);

	if(IsZCSyncOngoing())
	{
		frequency = LIB_ZC_GetNominalFrequency();
	}

	SAFETY_REGION_START();

	PM_FrequencyFeedbacks.gridfrequency = frequency;

	if( LIB_ZC_IsOnGood() )
	{
		PM_FrequencyFeedbacks_ResetAllFeedbacks();
	}
	SAFETY_REGION_END();

	for(inputindex = 0; inputindex < PM_FrequencyFeedbacks.number; inputindex ++)
	{
		/* Plausibility check on 'item'*/
		PLT_ERROR_FATAL_IF(((PM_FrequencyFeedbacks.item[inputindex].ID != PM_FREQUENCY_FEEDBACK_ID)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR);

		SAFETY_REGION_START();

		if (!(PM_FrequencyFeedbacks.item[inputindex].control & (PM_FF_ITEM_CTRL_UNAVAILABLE)))
		{
			mask = (PM_FrequencyFeedbacks_MaskType)(1u << PM_FrequencyFeedbacks.item[inputindex].position);

			PM_FrequencyFeedbacks_PulsesCollector(&PM_FrequencyFeedbacks.item[inputindex]);
			PM_FrequencyFeedbacks_SamplesCollector(&PM_FrequencyFeedbacks.item[inputindex]);

			SAFETY_DATA_CHECK( PM_FrequencyFeedbacks_Interface );
			PM_FrequencyFeedbacks_Process(&PM_FrequencyFeedbacks.item[inputindex], PM_FrequencyFeedbacks_Interface.disabled & mask);
			SAFETY_DATA_UPDATE( PM_FrequencyFeedbacks_Interface );

			PM_FrequencyFeedbacks.item[inputindex].status &= ~PM_FF_ITEM_STATUS_PULSE;
		}

		SAFETY_REGION_END();
	}
}

/* Local Function */


/**
Resets feedback variables of the state machine in order to restart the sampling.

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_RestartFeedback(PM_FrequencyFeedbackType * _pItem)
{
	PLT_ERROR_FATAL_IF(!((_pItem != NULL) && (_pItem->ID == PM_FREQUENCY_FEEDBACK_ID)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	PM_FrequencyFeedbacks_RestartFeedbackPulse(_pItem);
	PM_FrequencyFeedbacks_RestartFeedbackSampling(_pItem);
}

/**
Restarts feedback sampling manager.

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_RestartFeedbackPulse(PM_FrequencyFeedbackType * _pItem)
{
	PLT_ERROR_FATAL_IF(((_pItem == NULL)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	/* No need to check it again */
	(void) OCPWD_StopSampleSync ( _pItem->tpmchannel );

	_pItem->progress = 0;
	_pItem->status &= ~(PM_FF_ITEM_STATUS_FREQ_READY);
	_pItem->maxperiod = 0;

	/* No need to check it again */
	(void) OCPWD_StartSampleSync ( _pItem->tpmchannel, PM_FrequencyFeedbacks_PWDCallback);

}

/**
Restarts feedback sampling manager.

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_RestartFeedbackSampling(PM_FrequencyFeedbackType * _pItem)
{
	PLT_ERROR_FATAL_IF(((_pItem == NULL)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	_pItem->status &= ~(PM_FF_ITEM_STATUS_SAMPLES_READY);
	_pItem->hlsamples = 0;
	_pItem->samples = 0;
	_pItem->control |= PM_FF_ITEM_CTRL_SAMPLE_ENABLED;
	_pItem->timetorestart = CONFIG_FREQUENCY_FEEDBACKS_MAX_DELAY_TO_RESTART_SAMPLING;
}

/**
Collects samples

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_SamplesCollector(PM_FrequencyFeedbackType* _pItem)
{
	PLT_ERROR_FATAL_IF(((_pItem == NULL)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	/* sampling policy */
	if( _pItem->status & PM_FF_ITEM_STATUS_PULSE )
	{
		if(_pItem->progress <= 2)
		{
			/* keep active sampling */
			_pItem->control |=  PM_FF_ITEM_CTRL_SAMPLE_ENABLED;
		}
		else
		{
			_pItem->timetorestart = CONFIG_FREQUENCY_FEEDBACKS_MAX_DELAY_TO_RESTART_SAMPLING;
		}
	}

	if (_pItem->control &  PM_FF_ITEM_CTRL_SAMPLE_ENABLED )
	{
		if(++_pItem->samples < _pItem->requested_samples)
		{
			if(LIB_DIO_GET_INPUT_F(_pItem->pin, 20))
			{
				_pItem->hlsamples++;
			}
		}
		else
		{
			_pItem->status |= PM_FF_ITEM_STATUS_SAMPLES_READY;
			_pItem->control &=  ~PM_FF_ITEM_CTRL_SAMPLE_ENABLED;

			if ((_pItem->hlsamples) >= (_pItem->requested_samples - (_pItem->requested_samples/4)))
			{
				_pItem->status |= PM_FF_ITEM_STATUS_SAMPLE_HIGHVAL;
			}
			else if ((_pItem->hlsamples) <= (_pItem->requested_samples/4))
			{
				_pItem->status &= ~PM_FF_ITEM_STATUS_SAMPLE_HIGHVAL;
			}
			else
			{
				/* Keep last value set */
			}
		}
	}

}

/**
Collects pulses

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_PulsesCollector(PM_FrequencyFeedbackType* _pItem)
{
	PLT_ERROR_FATAL_IF(((_pItem == NULL)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	if( _pItem->progress < 20 )
	{
		/* frequency detection in progress */
		_pItem->progress++;
		/* check PWD notification */
		EnterCriticalSection();
		if( _pItem->freq_evt)
		{
			_pItem->freq_evt = 0;
			_pItem->status |= PM_FF_ITEM_STATUS_PULSE;
			_pItem->control &= ~PM_FF_ITEM_CTRL_SAMPLE_ENABLED;
		}
		_pItem->status &= ~PM_FF_ITEM_STATUS_FREQ_READY;
		ExitCriticalSection();
	}
	else
	{
		/* end of pulse detection */
		_pItem->progress=0;
		_pItem->status |= PM_FF_ITEM_STATUS_END_OF_FREQ;
	}

}

/**
Processes the acquisitions.

\param [in] _pItem pointer to feedback descriptor

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_Process(PM_FrequencyFeedbackType* _pItem, bool_t _Disabled)
{
	PM_FrequencyFeedbacks_ValueType newFFBValue = PM_FREQUENCY_FEEDBACKS_UNRELIABLE_VALUE;
	PM_FrequencyFeedbacks_BitmapType mask = (PM_FrequencyFeedbacks_BitmapType)(1u<< _pItem->position);

	PLT_ERROR_FATAL_IF(((_pItem == NULL)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	if ( (_pItem->status & PM_FF_ITEM_STATUS_END_OF_FREQ))
	{
		if(!(_pItem->control & PM_FF_ITEM_CTRL_SAMPLE_ENABLED))
		{
			_pItem->status |= PM_FF_ITEM_STATUS_FREQ_READY;
		}

		_pItem->status &= ~PM_FF_ITEM_STATUS_END_OF_FREQ;
	}

	if( !(_pItem->status & PM_FF_ITEM_STATUS_FREQ_READY) )
	{
		if( _pItem->status & PM_FF_ITEM_STATUS_SAMPLES_READY )
		{
			newFFBValue = (PM_FrequencyFeedbacks_ValueType) ((_pItem->status & PM_FF_ITEM_STATUS_SAMPLE_HIGHVAL) ? PM_FREQUENCY_FEEDBACK_HIGH : PM_FREQUENCY_FEEDBACK_LOW);
#if defined(CONFIG_FREQUENCY_FEEDBACKS_NOFREQ_MONITOR)
			if ((PM_FrequencyFeedbacks_Interface.feedback[_pItem->position] <= PM_FREQUENCY_FEEDBACK_HIGH)
					&& (PM_FrequencyFeedbacks_Interface.feedback[_pItem->position] != newFFBValue)
					&& (PM_FreqFeeback_NoFreqMonitor[_pItem->position] < 0xffff))
			{
			    PM_FreqFeeback_NoFreqMonitor[_pItem->position]++;
			}
#endif
		}
	}
	else
	{
		PM_FreqFeedback_MaxPeriodCounterType maxPeriodThres = PM_FF_HILO_FREQ_MAX_PULSES_NUM;

		/* Check grid frequency range to select threshold */
		if (PM_FrequencyFeedbacks.gridfrequency == NOM_FREQ_IS_50_HZ)
		{
			maxPeriodThres = PM_FF_50HZ_MAX_PULSES_NUM;
		}

		if (PM_FrequencyFeedbacks.gridfrequency == NOM_FREQ_IS_60_HZ)
		{
			maxPeriodThres = PM_FF_60HZ_MAX_PULSES_NUM;
		}

		/* Set FFB value depending of threshold */
		if(_pItem->maxperiod < maxPeriodThres)
		{
			newFFBValue = (PM_FrequencyFeedbacks_ValueType) PM_FREQUENCY_FEEDBACK_HIGH_FREQ;
		}
		else
		{
			newFFBValue = (PM_FrequencyFeedbacks_ValueType) PM_FREQUENCY_FEEDBACK_LOW_FREQ;
		}

	}


	if(newFFBValue != PM_FREQUENCY_FEEDBACKS_UNRELIABLE_VALUE)
	{
		if(!_Disabled)
		{
			PM_FrequencyFeedbacks_Interface.feedback[_pItem->position] = newFFBValue;
		}
		PM_FrequencyFeedbacks.ready |= mask;
		PM_FrequencyFeedbacks.reliable |= mask;
		PM_FrequencyFeedbacks_RestartFeedback(_pItem);
	}

	if (!(_pItem->control &  PM_FF_ITEM_CTRL_SAMPLE_ENABLED ))
	{
		if(_pItem->timetorestart > 0)
		{
			_pItem->timetorestart--;
		}
		else
		{
			_pItem->timetorestart = CONFIG_FREQUENCY_FEEDBACKS_MAX_DELAY_TO_RESTART_SAMPLING;
			PM_FrequencyFeedbacks_RestartFeedbackSampling(_pItem);
		}
	}

}

/**
Populates feedback descriptor.

\param [in]	_pFrequencyFeedback	pointer to feedback descriptor
\param [in]	_Pin				pin of the module
\param [in]	_TpmCh				tpm channel
\param [in]	_Samples			samples
\param [in]	_Position			position in ram

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static void PM_FrequencyFeedbacks_Populate(PM_FrequencyFeedbackType* _pFrequencyFeedback, PM_FreqFeedback_PinType _Pin, IO_ChannelType _TpmCh, PM_FreqFeedback_SamplesNumberType _Samples, PM_FrequencyFeedback_PositionType _Position)
{
	PLT_ERROR_FATAL_IF((_pFrequencyFeedback == NULL), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

	_pFrequencyFeedback->tpmchannel= _TpmCh;
	_pFrequencyFeedback->requested_samples = _Samples;
	_pFrequencyFeedback->pin = _Pin;
	_pFrequencyFeedback->timetorestart = CONFIG_FREQUENCY_FEEDBACKS_MAX_DELAY_TO_RESTART_SAMPLING;
	_pFrequencyFeedback->position = _Position;
	_pFrequencyFeedback->ID=PM_FREQUENCY_FEEDBACK_ID;
}

/**
Retrieves index by tpm channel

\param [in]	_TpmCh				tpm channel
\return	PM_FrequencyFeedbacks_IndexType
\retval	-1 invalid index
 	 	 value greater than 0 valid value

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static PM_FrequencyFeedbacks_IndexType PM_FrequencyFeedbacks_GetIndexByTimerCh(IO_ChannelType _TpmCh)
{
	PM_FrequencyFeedbacks_IndexType inputindex = -1;
	bool_t found = 0;
	for(inputindex = 0; inputindex < PM_FrequencyFeedbacks.number; inputindex ++)
	{
		/* Plausibility check on 'item'*/
		PLT_ERROR_FATAL_IF(((PM_FrequencyFeedbacks.item[inputindex].ID != PM_FREQUENCY_FEEDBACK_ID)), PM_FREQUENCY_FEEDBACKS_INVALIDFEEDBACKPNT_ERR)

		if((PM_FrequencyFeedbacks.item[inputindex].tpmchannel == _TpmCh))
		{
			found =1;
			break;
		}
	}
	if(!found)
	{
		inputindex = -1;
	}
	return inputindex;
}

/**
Retrieves index by position

\param [in]	_Position				position
\return	PM_FrequencyFeedbacks_IndexType
\retval	-1 invalid index
 	 	 value greater than 0 valid value

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static PM_FrequencyFeedbacks_IndexType PM_FrequencyFeedbacks_GetIndexByPosition(PM_FrequencyFeedback_PositionType _Position)
{
	PM_FrequencyFeedbacks_IndexType inputindex = -1;
	bool_t found = 0;
	for(inputindex = 0; inputindex < PM_FrequencyFeedbacks.number; inputindex ++)
	{
		if(!(PM_FrequencyFeedbacks.item[inputindex].ID == PM_FREQUENCY_FEEDBACK_ID))
		{
			/* something is going wrong :PM_FrequencyFeedbacks.item has been corrupted,  then FAULT*/
			return IO_E_UNEXPECTED_CONDITION;
		}

		if((PM_FrequencyFeedbacks.item[inputindex].position == _Position))
		{
			found =1;
			break;
		}
	}
	if(!found)
	{
		inputindex = -1;
	}
	return inputindex;
}

/**
PWD callback

\param [in]	notifVal
\param [in]	channel
\param [in]	error
\return	IO_ErrorType
\retval	IO_E_UNEXPECTED_CONDITION if invalid notification or missin initialization


\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static IO_ErrorType PM_FrequencyFeedbacks_PWDCallback ( IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error )
{
	IO_PWD_MeasureType tmpMeasure;
	PM_FrequencyFeedbacks_IndexType feedback=PM_FrequencyFeedbacks_GetIndexByTimerCh(channel);

    /* Assertion: notification status must be good */
    PLT_ERROR_FATAL_IF(!(( notifVal == IO_N_COMPLETED ) && ( error == IO_E_OK )), PM_FREQUENCY_FEEDBACKS_INVALIDNOTIFICATION_ERR);

    /*lint -e514*/
    PLT_ERROR_FATAL_IF((PM_FrequencyFeedbacks.key != (PM_FrequencyFeedbacks_InitKeyType)PM_FREQUENCY_FEEDBACKS_KEY), PM_FREQUENCY_FEEDBACKS_NOINIT_ERR)
    /*lint +e514*/

    if (feedback < PM_FrequencyFeedbacks.number)
    {
        /* Get last measure from OCPWD driver - all in nanoseconds! */
        OCPWD_GetAllSync( channel , &tmpMeasure);

        if(tmpMeasure.tOn >= PM_FrequencyFeedbacks_GetMaxTOnFilterValue())
    	{
        	if(tmpMeasure.period >= PM_FrequencyFeedbacks_GetMaxPeriodFilterValue ())
    		{
        		PM_FrequencyFeedbacks.item[feedback].maxperiod++;
    		}
    	}

        PM_FrequencyFeedbacks.item[feedback].freq_evt = 1;
    }

    return IO_E_OK;
}

/**
Returns maximum Ton value

\return	PM_FrequencyFeedbacks_TOnFilterType

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static PM_FrequencyFeedbacks_TOnFilterType PM_FrequencyFeedbacks_GetMaxTOnFilterValue (void)
{
	uint8_t findex = PM_FrequencyFeedbacks.gridfrequency;

	if(findex > (sizeof(PM_FrequencyFeedbacks_TOnFilter)/sizeof(PM_FrequencyFeedbacks_TOnFilter[0])))
	{
		findex = 0;
	}

	return  PM_FrequencyFeedbacks_TOnFilter[findex];

}
/**
Returns maximum Period value

\return	PM_FrequencyFeedbacks_PeriodFilterType

\author 	Roberto Fioravanti
\date		12/06/2012
\version	1.0.0



\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
static PM_FrequencyFeedbacks_PeriodFilterType PM_FrequencyFeedbacks_GetMaxPeriodFilterValue (void)
{
	uint8_t findex = PM_FrequencyFeedbacks.gridfrequency;

	if(findex > (sizeof(PM_FrequencyFeedbacks_PeriodFilter)/sizeof(PM_FrequencyFeedbacks_PeriodFilter[0])))
	{
		findex = 0;
	}

	return  PM_FrequencyFeedbacks_PeriodFilter[findex];

}
/* *********** */
/* End of file */
/* *********** */

