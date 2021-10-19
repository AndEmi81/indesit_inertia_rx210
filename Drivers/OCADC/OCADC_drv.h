/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCADC_drv.h
\n			Header file of OCADC_drv.c
\n
\file		OCADC_drv.h
\ingroup	OCADC
\date		March 2, 2010
\version	01.00.03
\author		Jaroslav Musil
\n
\n
\par	History:
\n
\n 		01.00.03
\n		02/03/2010
\n		Jaroslav Musil
\n      - added ADC_ResultValueType that depends on the peripheral result register
\n      - added the ADC_RESULT_REGISTER_SIZE define
\n      - added pResults item into the OCADC_ResultType structure
\n
\n 		01.00.02
\n		19/02/2010
\n		Jaroslav Musil
\n      - Commented
\n      - dma channels set up by the defines; i.e. removed from the function interfaces
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
\n
*/

#ifndef __OCADC_DRV_H
#define __OCADC_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Config.h"

#include "Hal_Interface.h"
#include HAL_CFG
#include HAL


#if (ADC_USE_SW_DMA && ADC_USE_HW_DMA )
#error "Not both ADC_USE_SW_DMA and ADC_USE_HW_DMA can be used"
#endif
#if (ADC_USE_SW_DMA)
#include "SWDMA_drv.h"
#elif (ADC_USE_HW_DMA)
#if defined(_HAL_DMA_CAPABILITY)
#include "OCDMA_drv.h"
#else
#error "HW DMA is not available on this platform. Check the _HAL_DMA_CAPABILITY option."
#endif
#endif

#include "IO_driver.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define ADC_IO_DEVICETYPE	CHANNEL
#define ADC_IO_ERROR_HOOK	FALSE
#define ADC_IO_DRIVERMODUS	SYNCRON

#define ADC_INT_ENABLED     1
#define ADC_INT_DISABLED    0
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
#ifndef ADC_RESOLUTION
#error "ADC_RESOLUTION not defined"
#endif /* ADC_RESOLUTION not defined */
#if (ADC_RESOLUTION > 16)
#error "ADC_RESOLUTION too high, max value is 16"
#endif /* ADC_RESOLUTION > 16 */
#if (ADC_RESOLUTION == 0)
#error "ADC_RESOLUTION can't be 0"
#endif /* ADC_RESOLUTION > 16 */

/* Result value after the resolution correction */
#if (ADC_RESOLUTION <= 8)
typedef uint8_t ADC_ValueType;
#define SIZE_IN_BYTE_OF_ADC_ValueType   1
#else
typedef uint16_t ADC_ValueType;
#define SIZE_IN_BYTE_OF_ADC_ValueType   2
#endif

#if (ADC_USE_SW_DMA)
/* Selects the correct data size with respect to the ADC result register size */
#if (ADC_RESULT_REGISTER_SIZE == 8)
/* Result value from the hardware; ADC hardware data type */
typedef uint8_t ADC_ResultValueType;
#elif (ADC_RESULT_REGISTER_SIZE == 16)
/* Result value from the hardware; ADC hardware data type */
typedef uint16_t ADC_ResultValueType;
#else
#error "Incorrect ADC register size; possible value is 8 or 16!"
#endif
#elif (ADC_USE_HW_DMA)
/* Selects the correct data size with respect to the ADC result register size */
#if (ADC_RESULT_REGISTER_SIZE == 8)
#define ADC_RESULT_DMA_SIZE OCDMA_SIZE_8BITs

/* Result value from the hardware; ADC hardware data type */
typedef uint8_t ADC_ResultValueType;
#elif (ADC_RESULT_REGISTER_SIZE == 16)
#define ADC_RESULT_DMA_SIZE OCDMA_SIZE_16BITs

/* Result value from the hardware; ADC hardware data type */
typedef uint16_t ADC_ResultValueType;
#else
#error "Incorrect ADC register size; possible value is 8 or 16!"
#endif
#endif
#define AD_DUMMY_POS    ((IO_PositionType)0xFF)


/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
IO_ErrorType OCADC_InitSync(void);
IO_ErrorType OCADC_DeInitSync(void);
ADC_ValueType OCADC_GetSync(IO_PositionType position);
IO_ErrorType OCADC_StartSampleSync (IO_ChannelType channel,IO_PositionType position, Comm_Notification callback);
IO_ErrorType OCADC_SampleSync (IO_ChannelType channel,IO_PositionType position);
IO_ErrorType  OCADC_IsConvertionReadySync(IO_ChannelType channel);
void OCADC_StopSampleSync(void);
#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
IO_ErrorType OCADC_StartMultiSampleOneChannelSync(IO_ChannelType channel, uint16_t sampleCount, ADC_ResultValueType *pResults, Comm_Notification callback);
IO_ErrorType OCADC_StartMultiSampleMultiChannelSync(IO_ChannelType *pChannels, uint16_t sampleCount, ADC_ResultValueType *pResults, Comm_Notification callback);
#endif
void OCADC_ISR(void);
IO_ErrorType OCADC_GetADchannelSync(const IO_ModuleIOMapType *const pIOMap,uint8_t *pChannel);

#endif /* __OCADC_DRV_H */

/* *********** */
/* End of file */
/* *********** */

