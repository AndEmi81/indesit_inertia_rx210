/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		OCADC_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCADC_drv.c
\ingroup	OCADC
\date		March 2, 2010
\version	01.00.03
\author		Jaroslav Musil
\n
\n
\par	History:
\n
\n      01.00.07
\n      Feb 02,2011
\n      Stefano Ughi
\n      fixed bug on OCADC_StartMultiSampleMultiChannelSync()
\n
\n      01.00.06
\n      16/06/2010
\n      Stefano Ughi
\n      added function IO_ErrorType OCADC_GetADchannelSync(IO_ModuleIOMapType *pIOMap,uint8_t *pChannel)
\n
\n		01.00.05
\n		31/05/2010
\n		Stefano Ughi
\n		added channel parameter on ADC_pResultRegister for compatibility with others uc
\n
\n 		01.00.04
\n		16/04/2010
\n		Stefano Ughi
\n      - In OCADC_StartMultiSampleMultiChannelSync()
\n      (void)SWDMA_StartTransferSync(ADC_RESULTS_DMA_CHANNEL, ADC_Callback);
\n      changed in
\n      (void)SWDMA_StartTransferSync(ADC_RESULTS_DMA_CHANNEL, NULL);
\n
\n      (void)SWDMA_StartTransferSync(ADC_CHANNELS_DMA_CHANNEL, NULL);
\n      changed in
\n      (void)SWDMA_StartTransferSync(ADC_CHANNELS_DMA_CHANNEL, ADC_Callback);
\n      because of new DMA SW Implementation
\n
\n 		01.00.03
\n		02/03/2010
\n		Jaroslav Musil
\n      - added ADC_ResultValueType that depends on the peripheral result register
\n      - added the ADC_RESULT_REGISTER_SIZE define
\n      - added pResults item into the OCADC_ResultType structure
\n
\n 		01.00.02
\n		18/02/2010
\n		Jaroslav Musil
\n      - Commented
\n      - dma channels set up by the defines; i.e. removed from the function interfaces
\n      - dma structure setting checks the type of the results and sets the transfer either 8-bit or 16-bit
\n      - used defines ADC_USE_SW_DMA and ADC_USE_HW_DMA
\n
\n 		01.00.01
\n		18/02/2010
\n		Jaroslav Musil
\n      - added the DMA multi sample functions
\n
\n 		01.00.00
\n		16/02/2010
\n		Stefano Ughi
\n		- Creation
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "OCADC_drv.h"
#include "LIB_ClassBVar.h"
#include "LIB_Trace.h"
#include "LIB_TaskMonitoring.h"
#if defined (CHECK_POSSIBLE_RESET_CAUSES)
#include "ResetModule.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/**

*/
typedef struct OCADC_ResultType_tag
{
	Comm_Notification callback;     /* callback function that will be called after the last sample is taken */
	ADC_ValueType *pResult;         /* pointer to the result after resolution correction, i.e. data type corresponds to the resolution size; used for single-shot sampling */
#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
	ADC_ResultValueType *pResults;  /* pointer to the result correction where the data type is the same as the hardware data type; used for multi-sampling*/
#endif
	IO_ChannelType channel;			/*usato per l'acquisizione singola*/
	IO_ChannelType *pChannel;       /* pointer to the channels */
	uint16_t sampleCount;           /* defines the number of samples that should be taken. In single shot mode, it must be 0 */
	IO_ChannelType dmaChannel;      /* dma channel for the results */
	IO_ChannelType dmaChannelCh;    /* dma channel for the adc channels */
} OCADC_ResultType;



/* ********* */
/* Variables */
/* ********* */


/* Global */

/* Static */
/** If TRUE ADC is Busy */
static bool_t ADC_Resource;

/** Here driver store convertion results
 * \par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_3
 * */
static ADC_ValueType ADC_Convertions[ADC_NUM_CHANNELLS_USED];
static OCADC_ResultType ADC_DataResult;

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Local Function */
#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
static IO_ErrorType ADC_Callback(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error);
#endif

#if defined( CONFIG_DEBUG_ADC_CHANNELS )
#include "OCADC_trace.h"
#else
#define DEBUG_ADC_TRACE_MODULE(x)
#define DEBUG_ADC_TRACE_INIT(x)
#define DEBUG_ADC_TRACE_CH( i, value )
#endif

DEBUG_ADC_TRACE_MODULE( ADC_NUM_CHANNELLS_USED )
/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n This function initialises the ADC driver. It uses the configurations parameters defined in
\n AdcConfig.h file that must be included in Config.h file.
\return 	IO_ErrorType
\retval		IO_E_OK initialization done

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_InitSync(void)
{
	int i;
	ADC_Init();
	ADC_Resource = FALSE;

	DEBUG_ADC_TRACE_INIT(ADC_NUM_CHANNELLS_USED)

	SAFETY_REGION_START();
	for(i=0; i<ADC_NUM_CHANNELLS_USED ; i++){
		SAFETY_DATA_ELEM_UPDATE( ADC_Convertions, i );
	}
	SAFETY_REGION_END();
	return (IO_ErrorType)IO_E_OK;
}
/**
\n This function deinitialises the ADC driver and
\n sets all ADC-channels back to their controller default configuration.
\return 	IO_ErrorType
\retval		IO_E_OK deinitialization done

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_DeInitSync(void)
{
	ADC_DeInit();
	ADC_Resource = FALSE;
	return (IO_ErrorType)IO_E_OK;
}
/**
\n The function returns the analogue channel linked to a specified pin.
\param  [in]	pIOMap fixed a pin index specifies the pointer to the Module IO Map.
\param  [in]	pChannel pointer where is stored the ADC channel
\return 	IO_ErrorType
\retval		IO_E_OK
\retval     IO_E_UNEXPECTED_CONDITION

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_GetADchannelSync(const IO_ModuleIOMapType *const pIOMap,uint8_t *pChannel)
{
	uint8_t ch;

	if ( ADC_GetChannel(pIOMap->PortAddress,pIOMap->PinMask,&ch))
	{
		*pChannel = ch;
		return (IO_ErrorType)IO_E_OK;
	}
	return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
}
/**
\n The function returns the analogue value of the specified position in the configured resolution.
\n Cfr. ADC_RESOLUTION param in AdcConfig.h.\n
\n The user is responsible to call the OCADC_StartSampleSync or OCADC_SampleSync functions before.
\param  [in]	position specifies a position inside ADC_Convertions[] buffer.
\return 	ADC_ValueType
\retval		analogue value

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
ADC_ValueType OCADC_GetSync(IO_PositionType position)
{
	return (ADC_Convertions[position]);
}

/**
\n This function starts the measurement of analogue value of the specified ADC channel and
\n stores the values into ADC_Convertions[] buffer. The function OCADC_GetSync  returns the stored
\n values. A measurement can be stopped using the function OCADC_StopSampleSync.

\param  [in]	The parameter channel specifies the ADC channel.
\param  [in]	The parameter position specifies where conversion must be stored inside ADC_Convertions[] buffer.
\param  [in]	callback  This parameter points to a callback function to inform the caller \n
 				about sample status of the specified channel: \n
				IO_E_OK: Measurement successfully finished. \n
				IO_E_FCN_SUSPENDED: Measurement was cancelled with function OCADC_StopSampleSync. \n
				This pointer can be set to NULL if not needed, in this case the caller is responsable to \n
				call OCADC_IsConvertionReadySync() in order to test if the resul is ready.\n
				The callback will be called when measurement is finished.\n
				Starting a new measurement from callback function is possible.

\return 	IO_ErrorType
\retval		IO_E_OK Measurement started successfully.
\retval		IO_E_BUSY Function is blocked because a measurement is currently running.
\retval		IO_E_INVALID_CHANNEL_ID The channel descriptor is invalid.
\retval		IO_E_INVALID_POSITION The position descriptor is invalid.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_StartSampleSync (IO_ChannelType channel,IO_PositionType position, Comm_Notification callback)
{

	if ( position >= ADC_NUM_CHANNELLS_USED )
	{
		return (IO_ErrorType)IO_E_INVALID_POSITION;
	}
	if (ADC_CheckChannel(channel))
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}


	EnterCriticalSection();
	if ( ADC_Resource )
	{
		ExitCriticalSection();
		return (IO_ErrorType)IO_E_BUSY;
	}
	/* Get ADC Resource */
	ADC_Resource = TRUE;
	ExitCriticalSection();

	/* The sampleCount is zero to determine that it is not multisample mode */
	ADC_DataResult.sampleCount = 0;
	/* Stores the pointer for the callback function */
	ADC_DataResult.callback = callback;
	/* Stores the pointer for the result */

	ADC_DataResult.pResult = &ADC_Convertions[position];
	/* Start Convertion */
	ADC_ConfigurePinAsChannel(channel);
	ADC_DataResult.channel = channel;
	
	/*lint -e506 */
	/*lint -e610 pointers have the same size */
	if ( NULL != callback )
		/*lint +e610 */
	{
		ADC_StartConvertionOnChannel(channel,ADC_INT_ENABLED);
	}
	else
	{
		ADC_StartConvertionOnChannel(channel,ADC_INT_DISABLED);
	}
	/*lint +e506 */

	return (IO_ErrorType)IO_E_OK;

}
/**
\n This function checks if the measurement of analogue value is finished. If it is finished, the value is
\n stored on the position into ADC_Convertions[] buffer which was specified by the function OCADC_StartSampleSync.
\n The function OCADC_GetSync  returns the stored values.
\n A measurement can be stopped using the function OCADC_StopSampleSync.

\return 	IO_ErrorType
\retval		IO_E_OK Measurement finished successfully.
\retval		IO_E_BUSY Function is blocked because a measurement is currently running.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_IsConvertionReadySync(IO_ChannelType channel)
{
    (void)channel;
	if (ADC_EndConvertion())
	{
        	*ADC_DataResult.pResult = (ADC_ValueType) ADC_GetConvertedValueOnChannel(channel);
		/* Release ADC Resource */
		ADC_Resource = FALSE;
		return (IO_ErrorType)IO_E_OK;
	}
	else
	{
		return (IO_ErrorType)IO_E_BUSY;
	}
}

/**
\n This function starts the measurement of analogue value of the specified ADC channel,
\n wait for end convertion, and stores the values into ADC_Convertions[] buffer.
\n The function OCADC_GetSync  returns the stored values.
\n The function is always avaible also if is not defined ADC_HW_DMA nor ADC_SW_DMA.

\param  [in]	The parameter channel specifies the ADC channel.
\param  [in]	The parameter position specifies where conversion must be stored inside ADC_Convertions[] buffer.

\return 	IO_ErrorType

\retval		IO_E_OK Measurement finished successfully.
\retval		IO_E_BUSY Function is blocked because a measurement is currently running.
\retval		IO_E_INVALID_CHANNEL_ID The channel descriptor is invalid.
\retval		IO_E_INVALID_POSITION The position descriptor is invalid.

\note		\par \b Note 1:
			-if position = AD_DUMMY_POS will be performed a sample on channel but the read not stored.
			-added for debugging functions Reset_SetResetCode(RESET_CODE_ATD);

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_SampleSync (IO_ChannelType channel,IO_PositionType position)
{
	ADC_ValueType adcValue;

	if ( (position >= ADC_NUM_CHANNELLS_USED) && (position != AD_DUMMY_POS))
	{
		return (IO_ErrorType)IO_E_INVALID_POSITION;
	}
	if (ADC_CheckChannel(channel))
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	EnterCriticalSection();
	if ( ADC_Resource )
	{
		ExitCriticalSection();
		return (IO_ErrorType)IO_E_BUSY;
	}
	/* Get ADC Resource */
	ADC_Resource = TRUE;
	ExitCriticalSection();
	/* Start Convertion */
	ADC_ConfigurePinAsChannel(channel);
	/*lint -e506 */
	ADC_StartConvertionOnChannel(channel,ADC_INT_DISABLED);
	/*lint +e506 */
#if defined (CHECK_POSSIBLE_RESET_CAUSES)
	Reset_SetResetCode(RESET_CODE_ATD);
#endif /* #if defined (CHECK_POSSIBLE_RESET_CAUSES) */
	ADC_WaitForConvertionOnChannel(channel);

	adcValue = (ADC_ValueType) ADC_GetConvertedValueOnChannel(channel);
	if (position != AD_DUMMY_POS)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( ADC_Convertions, position );

		ADC_Convertions[position] = adcValue;

		DEBUG_ADC_TRACE_CH(position, adcValue)

		SAFETY_DATA_ELEM_UPDATE( ADC_Convertions, position );
		SAFETY_REGION_END();
	}
	/* Release ADC Resource */
	ADC_Resource = FALSE;
	return (IO_ErrorType)IO_E_OK;
}
/**
\n This function stop a measurement.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
void OCADC_StopSampleSync(void)
{
	EnterCriticalSection();

	if (ADC_Resource)
	{
		/* Stops ADC conversion */
		ADC_StopConversion();

		/* Release ADC Resource */
		ADC_Resource = FALSE;

	}

	ExitCriticalSection();
}

#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
/**
\n This function starts the measurement of analogue value of the specified ADC channel.
\n It uses DMA and converts from the same channel the defined number of results.
\n At the end the function generates the callback.

\param  [in]	channel specifies the ADC channel.
\param  [in]	sampleCount specifies the number of samples to be taken.
\param  [in]	pResults specifies the position in memory where to store the first sample after conversion.
\param  [in]	callback  This parameter points to a callback function to inform the caller \n
 				about the conversion complete: \n
				if callback return IO_E_OK -> measurement successfully finished. \n

\return 	IO_ErrorType

\retval		IO_E_OK Measurement finished successfully.
\retval		IO_E_BUSY Function is blocked because a measurement is currently running.
\retval		IO_E_INVALID_CHANNEL_ID The channel descriptor is invalid.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_StartMultiSampleOneChannelSync(IO_ChannelType channel, uint16_t sampleCount, ADC_ResultValueType *pResults, Comm_Notification callback)
{
#if (ADC_USE_SW_DMA)
	SWDMA_TransferType  transfer = {0};
#else
	OCDMA_TransferType  transfer = {0};
#endif

	/* Checks if all the channel is valid */
	if (ADC_CheckChannel(channel))
	{
		return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
	}

	EnterCriticalSection();

	if (ADC_Resource)
	{
		ExitCriticalSection();
		return (IO_ErrorType)IO_E_BUSY;
	}

	/* Get ADC Resource */
	ADC_Resource = TRUE;

	ExitCriticalSection();

	/* Configures the DMA for the results transmission */

#if (ADC_USE_SW_DMA)

#if (ADC_RESULT_REGISTER_SIZE == 8)

	transfer.options.optionbyte |= (SWDMA_OPTION_SSIZE_BYTE|SWDMA_OPTION_DSIZE_BYTE);
	transfer.bcr = sampleCount;

#endif

#if (ADC_RESULT_REGISTER_SIZE == 16)

	transfer.options.optionbyte |= (SWDMA_OPTION_SSIZE_WORD|SWDMA_OPTION_DSIZE_WORD);
	transfer.bcr = (sampleCount << 1);

#endif

	transfer.sar = (SWDMA_SarType)ADC_pResultRegister(channel);
	transfer.dar = (SWDMA_DarType)pResults;
	transfer.channel = ADC_RESULTS_DMA_CHANNEL;

	transfer.options.optionbyte |= (SWDMA_OPTION_DINC|SWDMA_OPTION_CS);

	(void)SWDMA_ChannelEnableSync(ADC_RESULTS_DMA_CHANNEL);
	(void)SWDMA_SetTransferSync(&transfer);
	(void)SWDMA_StartTransferSync(ADC_RESULTS_DMA_CHANNEL, ADC_Callback);
#elif (ADC_USE_HW_DMA)
	/* in case of 8-bit ADC */
	if (ADC_RESULT_DMA_SIZE == OCDMA_SIZE_8BITs)
	{
		transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;

		transfer.bcr = sampleCount;
	}
	/* in case of 16-bit ADC */
	else if (ADC_RESULT_DMA_SIZE == OCDMA_SIZE_16BITs)
	{
		transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_WORD | OCDMA_ADDR_DSIZE_WORD | OCDMA_ADDR_CS;

		transfer.bcr = (sampleCount << 1);
	}

	transfer.sar = (OCDMA_SarType)ADC_pResultRegister();
	transfer.dar = (OCDMA_DarType)pResults;

	transfer.channel = OCDMA_CHANNEL_ID(ADC_RESULTS_DMA_CHANNEL) | OCDMA_CHANNEL_DEVICE(ADC_RESULTS_DMA_DEVICE);

	transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;


	/* Enables the DMA on ADC */
	ADC_EnableDMA();

	(void)OCDMA_SetChannelDeviceSync(&transfer);
	(void)OCDMA_SetTransferParamSync(&transfer);
	(void)OCDMA_SetTransferSync(&transfer);
	(void)OCDMA_StartTransferSync(0, &transfer, ADC_Callback);
#endif /* #if (ADC_USE_SW_DMA) */


	/* Stores sampleCount information */
	ADC_DataResult.sampleCount = sampleCount;

	/* Stores the pointer for the callback function */
	ADC_DataResult.callback = callback;

	/* Stores the pointer for the result */
	ADC_DataResult.pResults = pResults;

	/* No source table with channels */
	ADC_DataResult.pChannel = NULL;

	/* Saves the dma channels */
	ADC_DataResult.dmaChannel = ADC_RESULTS_DMA_CHANNEL;

	/* Configures the pin as ADC */
	ADC_ConfigurePinAsChannel(channel);

	/* Start Conversion */
	ADC_StartMultiSampleConvertionOnChannel(channel);

	return (IO_ErrorType)IO_E_OK;

}
#endif /* #if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)*/
#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
/**
\n This function, fixed a channels table pointed by pChannels, performs an A/D reading on each channel
\described inside the table.
\n One sample for each channel.
\n It uses DMA to transfer the channels to the ADC peripher and to transfer the converted results.
\n At the end the function generates the callback.
\n This function use two DMA Channels one for A/D channel table and one for results.

\param  [in]	pChannels points to the position in memory where the table of ADC Channels that must be sampled is.
\param  [in]	sampleCount specifies the number of samples to be taken equal to the numbers of channels decribed in the table.
\param  [in]    pResults points to the position in memory where the samples will be stored.
\param  [in]	callback  This parameter points to a callback function to inform the caller \n
 				about the conversion complete: \n
				IO_E_OK: Measurement successfully finished. \n

\return 	IO_ErrorType

\retval		IO_E_OK Measurement finished successfully.
\retval		IO_E_BUSY Function is blocked because a measurement is currently running.
\retval		IO_E_INVALID_CHANNEL_ID The channel descriptor is invalid.


\warning 	this function is not supported by H839079 because A/D result register depends from \n
			selected channel.

\note		\par \b Note 1:
			documentation by Jaroslav Musil
			\par \b Note 2:
			On MCF51AG128 the table of channels is modified by the funtion in the way that the channel is ordered with the IE mask
			because the channel register is shared with additional cofiguration like DMA and interrupt bit. This bit
			is set in each item of the channels' table and so the table must be R/W.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCADC_StartMultiSampleMultiChannelSync(IO_ChannelType *pChannels, uint16_t sampleCount, ADC_ResultValueType *pResults, Comm_Notification callback)
{
	IO_ChannelType *pChannel;
	uint32_t i;

#if (ADC_USE_SW_DMA)
	SWDMA_TransferType  transfer = {0};
	SWDMA_TransferType  transferCh = {0};
#elif (ADC_USE_HW_DMA)
	OCDMA_TransferType  transfer = {0};
	OCDMA_TransferType  transferCh = {0};
#endif

	/* The working pointer to the channels' table */
	pChannel = pChannels;

	/* Checks if all the channels are valid */
	for (i = 0; i < sampleCount; i++)
	{
		/*lint -e666, -e960 side effects don't cause problem */
		if (ADC_CheckChannel(*pChannel))
			/*lint +e666, +e960 */
		{
			return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
		}
		pChannel++;
	}


	EnterCriticalSection();

	if (ADC_Resource)
	{
		ExitCriticalSection();
		return (IO_ErrorType)IO_E_BUSY;
	}

	/* Get ADC Resource */
	ADC_Resource = TRUE;

	ExitCriticalSection();

	/* Configures the DMA for the results transmission */

#if (ADC_USE_SW_DMA)

#if (ADC_RESULT_REGISTER_SIZE == 8)

	transfer.options.optionbyte |= (SWDMA_OPTION_SSIZE_BYTE|SWDMA_OPTION_DSIZE_BYTE);
	transfer.bcr = sampleCount;

#endif

#if (ADC_RESULT_REGISTER_SIZE == 16)

	transfer.options.optionbyte |= (SWDMA_OPTION_SSIZE_WORD|SWDMA_OPTION_DSIZE_WORD);
	transfer.bcr = (sampleCount << 1);

#endif
	/* Source register is the A/D result register */
	transfer.sar = (SWDMA_SarType)ADC_pResultRegister(*pChannels);
	transfer.dar = (SWDMA_DarType)pResults;
	/* Select DMA Channel */
	transfer.channel = ADC_RESULTS_DMA_CHANNEL;
	/* Increment destination pointer after a sample, no source pointer incrementation  */
	transfer.options.optionbyte |= (SWDMA_OPTION_DINC|SWDMA_OPTION_CS);


	(void)SWDMA_ChannelEnableSync(ADC_RESULTS_DMA_CHANNEL);
	(void)SWDMA_SetTransferSync(&transfer);
	(void)SWDMA_StartTransferSync(ADC_RESULTS_DMA_CHANNEL, NULL);
#elif (ADC_USE_HW_DMA)

	/* in case of 8-bit ADC */
	if (ADC_RESULT_DMA_SIZE == OCDMA_SIZE_8BITs)
	{
		transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;

		transfer.bcr = sampleCount;
	}
	/* in case of 16-bit ADC */
	else if (ADC_RESULT_DMA_SIZE == OCDMA_SIZE_16BITs)
	{
		transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_SSIZE_WORD | OCDMA_ADDR_DSIZE_WORD | OCDMA_ADDR_CS;

		transfer.bcr = (sampleCount << 1);
	}

	transfer.sar = (OCDMA_SarType)ADC_pResultRegister();
	transfer.dar = (OCDMA_DarType)pResults;
	transfer.channel = OCDMA_CHANNEL_ID(ADC_RESULTS_DMA_CHANNEL) | OCDMA_CHANNEL_DEVICE(ADC_RESULTS_DMA_DEVICE);

	transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ | OCDMA_PERLINK_LINKCC_LCH1CS | OCDMA_PERLINK_LCH1(ADC_CHANNELS_DMA_CHANNEL);

	/* Enables the DMA on ADC */
	ADC_EnableDMA();

	(void)OCDMA_SetChannelDeviceSync(&transfer);
	(void)OCDMA_SetTransferParamSync(&transfer);
	(void)OCDMA_SetTransferSync(&transfer);
	(void)OCDMA_StartTransferSync(0, &transfer, ADC_Callback);
#endif


	/* Stores sampleCount information */
	ADC_DataResult.sampleCount = sampleCount;

	/* Stores the pointer for the callback function */
	ADC_DataResult.callback = callback;

	/* Stores the pointer for the result */
	ADC_DataResult.pResults = pResults;

	/* No source table with channels */
	ADC_DataResult.pChannel = pChannels;

	/* Saves the dma channels */
	ADC_DataResult.dmaChannel = ADC_RESULTS_DMA_CHANNEL;
	ADC_DataResult.dmaChannelCh = ADC_CHANNELS_DMA_CHANNEL;


	/* The working pointer to the channels' table */
	pChannel = pChannels;



	/* Configures all the channels as ADC and sets the IE flag of the channel (important for the DMA) */
	for (i = 0; i < sampleCount; i++)
	{
		ADC_ConfigurePinAsChannel(*pChannel);

		*pChannel = ADC_CorrectChannel(*pChannel);

		pChannel++;
	}

	/* Configures the ADC for the conversion */
	ADC_StartMultiSampleConvertionMultiChannel();

	/* Configures the DMA for channels; the ADC starts with the first channel transmission */

#if (ADC_USE_SW_DMA)

	transferCh.bcr = sampleCount;
	transferCh.sar = (SWDMA_SarType)pChannels;
	transferCh.dar = (SWDMA_DarType)ADC_pChannelRegister(*pChannels);
	transferCh.channel = ADC_CHANNELS_DMA_CHANNEL;

	transferCh.options.optionbyte |= (SWDMA_OPTION_CS|SWDMA_OPTION_START|SWDMA_OPTION_SINC|SWDMA_OPTION_SSIZE_BYTE|SWDMA_OPTION_DSIZE_BYTE);

	(void)SWDMA_ChannelEnableSync(ADC_CHANNELS_DMA_CHANNEL);
	(void)SWDMA_SetTransferSync(&transferCh);
	/*lint -e619 pointers have the same size */
	(void)SWDMA_StartTransferSync(ADC_CHANNELS_DMA_CHANNEL, ADC_Callback);
	/*lint +e619 */
#elif (ADC_USE_HW_DMA)

	/* bcr = sampleCounte * 2 because it is 16-bit */
	transferCh.bcr = sampleCount;
	transferCh.sar = (OCDMA_SarType)pChannels;
	transferCh.dar = (OCDMA_DarType)ADC_pChannelRegister();

	transferCh.channel = OCDMA_CHANNEL_ID(ADC_CHANNELS_DMA_CHANNEL) | OCDMA_CHANNEL_DEVICE(ADC_CHANNELS_DMA_DEVICE);

	transferCh.addr = OCDMA_ADDR_SINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;

	transferCh.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;

	(void)OCDMA_SetChannelDeviceSync(&transferCh);
	(void)OCDMA_SetTransferParamSync(&transferCh);
	(void)OCDMA_SetTransferSync(&transferCh);
	(void)OCDMA_StartTransferSync(1, &transferCh, NULL);

#endif

	return (IO_ErrorType)IO_E_OK;

}
#endif /* #if (ADC_USE_SW_DMA | ADC_USE_HW_DMA) */
/**
\n ADC Driver event.
\n Call this method inside the ADC interrupt.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
void OCADC_ISR(void)
{
	IRQ_MONITOR_ENTER();

	/* if sampleCount is 0, it means the single shot mode is used */
	if (ADC_DataResult.sampleCount == 0)
	{
		IO_ChannelType channel;
		channel = ADC_DataResult.channel;
		/* Saves the result into the table */
		*ADC_DataResult.pResult
				= (ADC_ValueType) ADC_GetConvertedValueOnChannel(channel);
		/* Release ADC Resource */
		ADC_Resource = FALSE;

		/* Conversion complete callback */
		(void) (ADC_DataResult.callback)((IO_NotificationType) IO_N_COMPLETED,
				(IO_ChannelType) ADC_GetActiveChannel(), (IO_ErrorType) IO_E_OK);
	}

	IRQ_MONITOR_EXIT();

#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
	else
	{
#if (ADC_USE_SW_DMA)
		/* executes the result DMA transfer */
		if (SWDMA_CheckChannelEnableSync(ADC_DataResult.dmaChannel) == (IO_ErrorType)IO_STATE_ACTIVE)
		{
			(void)SWDMA_ExeTransferMngrSync(ADC_DataResult.dmaChannel);
#if defined(ADC_NO_CONTINUOS_MODE_SUPPORTED)
			/* if in single channel mode */
			if (ADC_DataResult.pChannel == NULL)
			{
				ADC_EmulateContinuousConversion();
			}
#endif /* #if defined(ADC_NO_CONTINUOS_MODE_SUPPORTED) */
		}

		/* if in multi channel mode */
		if (ADC_DataResult.pChannel != NULL)
		{
			/* executes the channelt DMA transfer */
			if (SWDMA_CheckChannelEnableSync(ADC_DataResult.dmaChannelCh) == (IO_ErrorType)IO_STATE_ACTIVE)
			{
				(void)SWDMA_ExeTransferMngrSync(ADC_DataResult.dmaChannelCh);
			}
		}

#endif /* (ADC_USE_SW_DMA) */
	}
#endif /* #if (ADC_USE_SW_DMA | ADC_USE_HW_DMA) */
}


/* Local Function */

#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
/**
\n DMA Driver event.
\n Callback of the DMA driver.
\n The results are converted to have the correct resulution and the ADC user callback is called.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
static IO_ErrorType ADC_Callback(IO_NotificationType notification, IO_ChannelType channel, IO_ErrorType error)
{
	/* Stops conversion  */
	ADC_StopConversion();

#if (ADC_USE_HW_DMA)
	/* Deinitializes and release the DMA channel */
	OCDMA_DeInitSync(ADC_DataResult.dmaChannel);

	/* if in multichannel mode */
	if (ADC_DataResult.pChannel != NULL)
	{
		/* Deinitializes and release the DMA channel */
		OCDMA_DeInitSync(ADC_DataResult.dmaChannelCh);
	}

	/* Disables the DMA on ADC */
	ADC_DisableDMA();
#endif
	/* Converts all the results to have the correct resolution */
	while (ADC_DataResult.sampleCount > 0)
	{
		/*lint -e506 */

		*ADC_DataResult.pResults = ADC_GetCorrectResolutionResult(*ADC_DataResult.pResults);
		/*lint +e506 */
		ADC_DataResult.pResults++;

		ADC_DataResult.sampleCount--;
	}
	/* Release ADC Resource */
	ADC_Resource = FALSE;

	/* The conversion complete callback */
	(void)(ADC_DataResult.callback)((IO_NotificationType)IO_N_COMPLETED, (IO_ChannelType)ADC_DataResult.dmaChannel, (IO_ErrorType)IO_E_OK);
	return (IO_ErrorType)IO_E_OK;

}
#endif /* (ADC_USE_SW_DMA | ADC_USE_HW_DMA) */

/* *********** */
/* End of file */
/* *********** */


