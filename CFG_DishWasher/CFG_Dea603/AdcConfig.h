#ifndef __ADCPLTFCFG_H
#define __ADCPLTFCFG_H

#include "StandardTypes.h"

#define HAL_ADC_SUPPORT	

/* How manage ADC reading */
#define ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS 1

/* If defined ADC End Convertion Interrupt is Enabled */
#define ADC_USE_END_CONV_INTRPT
/*
if '1' will be used Hardware DMA for ADC 
*/
#define ADC_USE_HW_DMA   0
/*
if '1' will be used Software DMA for ADC 
*/
#define ADC_USE_SW_DMA   0
/*
These define the DMA channels used for the ADC results transfer, and for the channels transfer
*/
#define ADC_RESULTS_DMA_CHANNEL     0
#define ADC_CHANNELS_DMA_CHANNEL    1
/* 
Specifies the resolution of the converted analogue value. This is not the real resolution 
of ADC.
The ADC_RESOLUTION has a range from 1 bit to 16 bit. 
Resolutions better than highest AD resolution will be obtained using 
a shift left operation on ADC result.
Resolutions lower than highest ADC resolution will be obtained using 
a shift right operation on ADC result. 
*/
#define ADC_RESOLUTION	10

/* Specifies the ADC register size, 8-bit or 16-bit Used by DMA */
#define ADC_RESULT_REGISTER_SIZE    16
/*
Specifies the convertion frequency of ADC in Hz.
*/
#define ADC_CONV_FREQUENCY_Hz	228571
/*
Specifies the number of channels to read. This is also the number of elements of the internal buffer 
inside driver used to store AD convertions.  
*/
#define ADC_NUM_CHANNELLS_USED	8
/*
With this define the ADC will perform ADC_NUM_SAMPLES on the specified channel 
and update a buffer with the result.To Verify !!
*/
#define ADC_NUM_SAMPLES	1

/* Setting File Power of 2 Max Number of Samples */
#define AD_N_READ_POWER_2   6

/*------------ Local Filtering ----------*/
/* Number of local readings made consecutively on the selected channel,actual readings = 2^AD_LOCAL_SAMPLES_POWER_OF_TWO */
#define AD_LOCAL_SAMPLES_POWER_OF_TWO   2  
/* Number of local readings discarded on those made consecutively on the selected channel */
#define AD_LOCAL_SAMPLES_DISCARDED      0  
/*------------ V line adjustment and V line N sample --------*/
#define VLINEMEASURE_COMP_FACTOR        0
#define N_SAMPLE_V_LINE 				5

/*---------------- Analog Feedback ---------------------*/
#define N_ANALOG_FEEDBACK   8  /* Numbers of analog feedbacks */
/*----------------------- Don't touch below ----------------*/
#if (ADC_USE_HW_DMA==1)
#define ADC_RESULTS_DMA_DEVICE  7
#define ADC_CHANNELS_DMA_DEVICE 7
#endif

#define SF_AD_MAX_SAMPLES           (1<<AD_N_READ_POWER_2)
#define ONE_AD_SAMPLE_MAX_VALUE     ((1<<ADC_RESOLUTION)-1)
#define LOC_AD_SAMPLES              (1<<AD_LOCAL_SAMPLES_POWER_OF_TWO)

#if (ADC_NUM_CHANNELLS_USED > 32)
#error "ADC_NUM_CHANNELLS_USED must be less or equal 32"
#endif

#if ( AD_LOCAL_SAMPLES_DISCARDED >= (2*AD_LOCAL_SAMPLES_POWER_OF_TWO) )
	#error "\n AD_LOCAL_SAMPLES_DISCARDED property: AD_LOCAL_SAMPLES_DISCARDED must be less than (2*AD_LOCAL_SAMPLES_POWER_OF_TWO) "
#endif
/* define local filtering variable size */
#if ((ONE_AD_SAMPLE_MAX_VALUE*(LOC_AD_SAMPLES-AD_LOCAL_SAMPLES_DISCARDED)) <= ((1<<8)-1))
typedef uint8_t lowFilterADCsum_t;
#elif ((ONE_AD_SAMPLE_MAX_VALUE*(LOC_AD_SAMPLES-AD_LOCAL_SAMPLES_DISCARDED)) <= ((1<<16)-1))
typedef uint16_t lowFilterADCsum_t;
#elif ((ONE_AD_SAMPLE_MAX_VALUE*(LOC_AD_SAMPLES-AD_LOCAL_SAMPLES_DISCARDED)) <= ((1<<32)-1))
typedef uint32_t lowFilterADCsum_t;
#else
#error " ADC resolution or AD_LOCAL_SAMPLES_POWER_OF_TWO exceed limits "
#endif
    

#if ((SF_AD_MAX_SAMPLES*ONE_AD_SAMPLE_MAX_VALUE) <= ((1<<8)-1))
typedef uint8_t ad_sum_t;     	
#elif ((SF_AD_MAX_SAMPLES*ONE_AD_SAMPLE_MAX_VALUE) <= ((1<<16)-1))
typedef uint16_t ad_sum_t;   
#else
typedef uint32_t ad_sum_t;   
#endif

#if (ADC_NUM_CHANNELLS_USED <= 8)
typedef uint8_t analogInput_t;
#elif (ADC_NUM_CHANNELLS_USED <= 16)
typedef uint16_t analogInput_t;
#else
typedef uint32_t analogInput_t;
#endif

#if (N_ANALOG_FEEDBACK > 16)
#error "What do you do with all this Analog Feedback?" 
#endif

#endif /*__ADCPLTFCFG_H*/
