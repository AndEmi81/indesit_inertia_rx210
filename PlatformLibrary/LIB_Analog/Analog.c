/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		Analog.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		Analog.c
\ingroup	LIB_Analog
\date		Jun 16, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par\b	History:
\n
\n 		01.00.00
\n		Jun 16, 2010
\n		stefano.ughi
\n		changes
\n 		- Creation
\n
\n 		01.00.01
\n		Feb 3, 2011
\n		stefano.ughi
\n		changes
\n 		- Removed RTN_BACKUP define
\n
\n 		01.00.02
\n		Mar 9, 2011
\n		stefano.ughi
\n		changes
\n 		- Added ADC_ValueType LIB_ADC_GetRefInput(void)
\n      - Added bool_t LIB_ADC_IsRefInputReady(void)
\n      - Added SFC_VlineMeasureOnNegativeLevel(Ch_Vline) under AD_CONVERTER_CHECK
\n
\n 		01.00.03
\n		Apr 1, 2011
\n		stefano.ughi
\n		changes
\n 		- #include "Trash.h" and #include "TrashExp.h" under defines
\n      - #include "LIB_GridVoltage.h" under defines
*/
#define MODULE_BODY_LIB_ANALOG
/* ******** */
/* Includes */
/* ******** */
#include <stdlib.h>
#include <string.h>
#include "Config.h"
#include "Analog.h"
#include "callback.h"
#include "LIB_Error.h"
#include "LIB_ClassBVar.h"

#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
#include "LIB_ZeroCrossing.h"
#endif

#if defined(PLATFORM2_SUPPORT) && defined(WILD_CARD)
#include "LIB_GridVoltage.h"
#endif

#if defined(CONFIG_LIB_CLASS_B_ADC)
#include "LIB_ClassBCheck.h"
#endif

#include "LIB_Trace.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* A/D channel config. Setting file params */
typedef union analogLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic:  1;  /* Automatic Readings */
		uint8_t unused_0:   4;
		uint8_t nRead:      3;  /* Real number of samples is 2^nRead */
		/* Low Byte */
		uint8_t vLine:      1;  /* the channel is tagged as V line */
		uint8_t vRef:       1;  /* the channel is tagged as V ref */
		uint8_t comp:       1;  /* if '1' measure must be compesated with V ref measure */
		uint8_t chDisch:    1;  /* if '1' read V line channel before reading the channel */										
		uint8_t fal:        4;  /* Filter validation of reading (real number is 2^fal) */
#else
		/* High Byte */
		uint8_t nRead:      3;  /* Real number of samples is 2^nRead */
		uint8_t unused_0:   4;
		uint8_t automatic:  1;  /* Automatic Readings */
		/* Low Byte */
		uint8_t fal:        4;  /* Filter validation of reading (real number is 2^fal) */
		uint8_t chDisch:    1;  /* if '1' read V line channel before reading the channel */
		uint8_t comp:       1;  /* if '1' measure must be compesated with V ref measure */
		uint8_t vRef:       1;  /* the channel is tagged as V ref */
		uint8_t vLine:      1;  /* the channel is tagged as V line */

#endif								
	} bit;

} analogLLIparams_t;

typedef struct analog_tag
{
	analogLLIparams_t analogConfigData;     /* Low Level Input Params */
	uint8_t channel;                        /* A/D channel */
	uint8_t analogInputRamPos;              /* Position where result is stored */
	uint8_t analogInputIndex;				/* Number of Samples collected */
	uint8_t analogInputCnt;                 /* Counter used for max. variation between two samples */
	ad_sum_t analogInputSum;   			    /* A/D samples's sum */
	ADC_ValueType analogInputOldValue;      /* A/D old sample */

} analog_t;

/* V Line measure */
typedef struct V_Line_tag
{
	/* V Line new measure A/D value */
	ADC_ValueType newVPeak;
	/* V Line current measure A/D value */
	ADC_ValueType vPeak;
	/* samples */
	uint8_t samples;

} V_Line_t;

#if (defined(CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES) && (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 1))

/* Check if average computation can fit into an uint16_t accumulator */
#if ((ONE_AD_SAMPLE_MAX_VALUE * CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES) <= 0xFFFF)
typedef uint16_t ADC_SumValueType;

#else

/* uin32_t is necessary for accumulator */
typedef uint32_t ADC_SumValueType;

#if ((ONE_AD_SAMPLE_MAX_VALUE * CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES) > 0xFFFFFFFF)
#error "Too many A/D Average samples used - sum computation may not fit into a 32 bit integer"
#endif

#endif

#endif


typedef struct RefInput_s
{
	/** Main Voltage A/D Channel */
	uint8_t Ch_Vline;
	/** RTN/Compensation A/D Channel */
	uint8_t RefCh;
	/** RTN A/D value */
	ADC_ValueType RefVal;
#if defined(CONFIG_VLINE_ON_NEG_LEVEL)
	ADC_ValueType VlineOnNegLev;
#endif
#if (defined(CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES) && (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 1))
	ADC_SumValueType SumVlineOnNegLev;
	#if (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 0xFFFFFFFF)
	#error "Too many A/D Average samples!"
	#elif (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 0xFFFF)
	uint32_t SumVlineSamples;
	#elif (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 0xFF)
	uint16_t SumVlineSamples;
	#else
	uint8_t SumVlineSamples;
	#endif
#endif
	/** If TRUE RTN measure is ready */
	bool8_t RefReady;
	/** Number of registered channels */
	uint8_t ChannelRegistered;
} RefInput_t;

/* ********* */
/* Variables */
/* ********* */

/* Global */

/* bitmap if '0' update AnalogInput[] */
analogInput_t AnalogInputDisable;
/* bitmap, this variable can only be set never clear except on Cold Reset */
analogInput_t AnalogInputReady;
/* bitmap, this variable can only be set from Platform when A/D readings is ready and
can only be cleared by Application  */
analogInput_t ApplianceAnalogInputReady;

#if (MANAGE_AD_CHANNEL_READ==TRUE)
/* bitmap */
static analogInput_t AD_ChInputReady;
#endif

/** Array with the acquired analog values */
ADC_ValueType AnalogInput[ADC_NUM_CHANNELLS_USED];

/* Static */
static analog_t AnalogInputParam[ADC_NUM_CHANNELLS_USED];
static RefInput_t RefInput;

#if defined (ATD_INTERRUPT_ENABLED)
static uint8_t AD_Index;
#elif defined (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS)
#if (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS == 0)
static uint8_t AD_Index;
#endif
#else
/* do nothing */
#endif

/** If TRUE RTN value is in acceptable (good) */
static bool_t GoodRef = FALSE;
/** If TRUE RTN value is in range */
static bool_t RtnOutRange = FALSE;

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static void LIB_ADC_AvgFilter ( uint8_t index, ADC_ValueType conversion );
static void LIB_ADC_DeltaCtrl( uint8_t index, ADC_ValueType newADvalue );
#if defined(CONFIG_VLINE_ON_NEG_LEVEL)
static void LIB_ADC_VlineMeasOnNegLevel(uint8_t ch_Vline);
#endif

#if defined (ATD_INTERRUPT_ENABLED)
static IO_ErrorType EndConvCallback (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
#endif

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
\n Initialization of variables relating to analog inputs.
\n This function is called inside OnColdReset() if SystemMode == SYSTEM_NORMAL_MODE

\attention \b AnalogInputParam[] (global) used as W
\attention \b AD_Index (global) used as W
\attention \b AnalogInputDisable (global) used as W
\attention \b AnalogInputReady (global) used as W
\attention \b RefInput.RefReady (global) used as W
\attention \b RefInput.Ch_Vline (global) used as W
\attention \b RefInput.RefCh (global) used as W
\attention \b AD_ChInputReady (global) used as W
\attention \b ApplianceAnalogInputReady (global) used as W

\author 	stefano.ughi
\date		Jun 28, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			tested
*/
void LIB_ADC_InitSync (void)
{
	uint8_t i;
#if defined (ATD_INTERRUPT_ENABLED)

	AD_Index = 0;

#elif defined (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS)

#if (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS == 0)

	AD_Index = 0;

#endif

#else
	/* do nothing */
#endif

	AnalogInputDisable = 0;	    /* Sample on Analog Inputs Enabled */
	AnalogInputReady = 0;	    /* A/D Reading not ready */
	ApplianceAnalogInputReady = 0;
#if (MANAGE_AD_CHANNEL_READ==TRUE)
	AD_ChInputReady = 0;
#endif

	SAFETY_REGION_START();

	/* V ref. not ready */
	RefInput.RefReady = FALSE;
	RefInput.RefVal = 0;
	/* Channel for Vline Measure not defined */
	RefInput.Ch_Vline = CH_VLINE_IS_NOT_DEFINED;
	/* Channel for Compensation Measure not defined */
	RefInput.RefCh = CH_FOR_COMPENSATION_IS_NOT_DEFINED;

	SAFETY_DATA_UPDATE( RefInput );
	SAFETY_DATA_UPDATE( AnalogInputDisable );

	memset(AnalogInputParam, 0, sizeof(AnalogInputParam));
	for ( i=0; i<ADC_NUM_CHANNELLS_USED; i++)
	{
		SAFETY_DATA_ELEM_UPDATE( AnalogInputParam, i );
		SAFETY_DATA_ELEM_UPDATE( AnalogInput, i );
	}

#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
	LIB_ZC_SetLineVoltageParams(FALSE, 0);
#endif

	SAFETY_REGION_END();

}

/**
\n Setup of a single A/D channel.
\n This function is called inside LIB_IOM_InputInitialization()  regardless cold or warm reset.

\param  [in] channel A/D Converter channel
\param  [in] posResult  position vs AnalogInput[] vector where A/D conv. is stored
\param  [in] *pADlowLevelParams pointer to the Setting File Structure that describes the channel.

\attention \b RefInput.Ch_Vline (global) W A/D channel where Vpeak is measured.
\attention \b RefInput.RefCh (global) W A/D channel where V ref is measured..
\attention \b AnalogInputParam[channel] (global) W A/D channel config. structure.

\author 	stefano.ughi
\date		Jun 28, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			tested

*/

void LIB_ADC_RegisterADchannel( uint8_t channel, uint8_t posResult, const uint8_t *pADlowLevelParams )
{
	analog_t *pAnalogInputParam;

	/*MS errato check PLT_ASSERT( channel < ADC_NUM_CHANNELLS_USED ) */

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(RefInput);
	SAFETY_DATA_ELEM_CHECK(AnalogInputParam, RefInput.ChannelRegistered);

	pAnalogInputParam = &AnalogInputParam[RefInput.ChannelRegistered];


	(void)memset(pAnalogInputParam,0,sizeof(AnalogInputParam[0]));

	pAnalogInputParam->channel = channel;
	/*lint -e826 pointer-to-pointer conversion area too small */
	/*TODO: plausibility on pADlowLevelParams , verificare se � un parametro di sicurezza nel settings*/
	pAnalogInputParam->analogConfigData = *(analogLLIparams_t*)pADlowLevelParams;
	/*lint +e826 */
	pAnalogInputParam->analogInputRamPos = posResult;
	SAFETY_DATA_ELEM_UPDATE(AnalogInputParam, RefInput.ChannelRegistered);

	if (pAnalogInputParam->analogConfigData.bit.vLine)
	{
		RefInput.Ch_Vline = pAnalogInputParam->channel;
#if defined (CONFIG_MAIN_LIVE_UPDATE_ZC_LINE_VOLTAGE)
		LIB_ZC_SetLineVoltageParams(TRUE, 0);
#endif
	}
	else if (pAnalogInputParam->analogConfigData.bit.vRef)
	{
		RefInput.RefCh = pAnalogInputParam->channel;
	}
	else
	{
		/* do nothing */
	}

	RefInput.ChannelRegistered++;

	SAFETY_DATA_UPDATE(RefInput);
	SAFETY_REGION_END();

}
/**
\n Unregister A/D channel structure.
\n This function is called inside LIB_IOM_InputInitialization()  regardless cold or warm reset.

\attention \b ChannelRegistered (global) used as W

\author 	stefano.ughi
\date		Jun 28, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_ADC_UnregisterAllADch(void)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(RefInput);

	RefInput.ChannelRegistered = 0;      /* No channel registered */

	SAFETY_DATA_UPDATE(RefInput);
	SAFETY_REGION_END();

	/* Start ADC Check is performed here
	 * LIB_ADC_InitSync() is called only on cold reset
	 */

#ifdef CONFIG_LIB_CLASS_B_ADC
	ADC_CheckInit();
#endif /* CONFIG_LIB_CLASS_B_ADC */

}


/**
\n A/D channels manager, called inside Task Main_AnalogInputTask.
\n if ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS = 1
\n all channels defined inside setting file, except for the Vline channel,
\n are sampled in the negative half-wave voltage
\n if ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS = 0
\n all channels defined inside setting file, except for the Vline channel,
\n are sampled in the negative half-wave voltage one for each half-wave
\n if defined ATD_INTERRUPT_ENABLED
\n all channels defined inside setting file
\n are sampled in interrupt (Not Tested)

\attention \b AnalogInputParam[i].analogConfigData.AnalogInputEnable (global) used as input
\attention \b RefInput.Ch_Vline = 0xFF -> Vline non presente (global) used as input
\attention \b AnalogInputParam[i].analogConfigData.AnalogInputMustBeDischBeforeRead (global) used as input
\attention \b RefInput.RefCh (global) used as input
\attention \b AD_Index (global) used as input

\attention \b   RefInput.RefReady (global) used as output
\attention \b   RefInput (global) used as output


\author 	x
\date		gg/mm/aaaa
\version	x.y.z


\note		\par \b Nota 1:
			Se per un certo canale AnalogInputParam[i].analogConfigData.AnalogInputMustBeDischBeforeRead
			vale '1' viene eseguita prima una lettura dummy della Vline per scaricare il canale
			si e' infatti notato che se il canale ? flottante tende a "caricarsi"
			\par \b Nota 2:
			Se ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS vale 0 si potrebbe implementare un metodo diverso\n
			di gestione delle letture per mantenere costante il periodo di campionamento, del tipo\n
			se il canale e' abilitato eseguo lettura altrimenti aspetto la successiva chiamata del Manager.
*/
void LIB_ADC_Manager(void)
{
	uint8_t i;
#ifndef ATD_INTERRUPT_ENABLED
	ADC_ValueType res;
#endif

#if defined (ATD_INTERRUPT_ENABLED)

	i=0;
	/*  Read all enabled channel, using end convertion interrupt, except V line Channel if present.*/
	while ( ( ( AnalogInputParam[AD_Index].analogConfigData.bit.automatic == 0 ) || ( AnalogInputParam[AD_Index].channel == RefInput.Ch_Vline ) ) &&
	        ( i < ADC_NUM_CHANNELLS_USED ) &&
	        ( AD_Index < ADC_NUM_CHANNELLS_USED ) )
	{
		i++;
		AD_Index++;
		if ( AD_Index >= ADC_NUM_CHANNELLS_USED )
		{
			AD_Index=0;
		}
	}


	if ( i >= ADC_NUM_CHANNELLS_USED )
	{
		/* No automatic channels found !!*/
		return;
	}

	/* if Ch_Vline is defined and the channel must be discharged before read it, we discharge it on Ch_Vline
	with a dummy read */
	if ( ( RefInput.Ch_Vline != CH_VLINE_IS_NOT_DEFINED ) &&
	        ( AnalogInputParam[AD_Index].analogConfigData.bit.chDisch ) )
	{
		(void)OCADC_SampleSync (RefInput.Ch_Vline,AD_DUMMY_POS);
	}

	if ((IO_ErrorType)IO_E_BUSY != OCADC_StartSampleSync (AnalogInputParam[AD_Index].channel,AnalogInputParam[AD_Index].analogInputRamPos, EndConvCallback))
	{
		AD_Index++;
		if (AD_Index >= ADC_NUM_CHANNELLS_USED)
		{
			AD_Index=0;
		}
	}

#elif defined (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS)

	/* No interrupts to manage A/D conversions */
#if (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS == 1)

	/*  Read all enabled channel "one shot reading" except V line Channel if present.*/

	for ( i=0; i<ADC_NUM_CHANNELLS_USED; i++)
	{
		SAFETY_REGION_START();
		SAFETY_DATA_ELEM_CHECK( AnalogInputParam, i );
		/* AnalogInput position is not directly related to AnalogInputParam; anyway this check is useful
		 * to check all AnalogInput channels
		 */
		SAFETY_DATA_ELEM_CHECK( AnalogInput, i);
		SAFETY_REGION_END();
		/* N.B.: if Ch_Vline not defined Ch_Vline = 0xFF and ADC_NUM_CHANNELLS_USED is <= 32 */
		if ( ( AnalogInputParam[i].analogConfigData.bit.automatic ) &&
		        ( AnalogInputParam[i].channel != RefInput.Ch_Vline ) )
		{
			/* Ch_Vline is defined and the channel must be discharged before read it, we discharge it on Ch_Vline
			   with a dummy read */
			if ( ( RefInput.Ch_Vline != CH_VLINE_IS_NOT_DEFINED ) &&
			        ( AnalogInputParam[i].analogConfigData.bit.chDisch ) )
			{
				(void)OCADC_SampleSync (RefInput.Ch_Vline, AD_DUMMY_POS);
			}

			/* consecutive readings performed 2^AD_LOCAL_SAMPLES_POWER_OF_TWO */
			res = LIB_ADC_Conv_F( i, AD_LOCAL_SAMPLES_POWER_OF_TWO, AD_LOCAL_SAMPLES_DISCARDED);

			if ( AnalogInputParam[i].channel == RefInput.RefCh )
			{
				SAFETY_REGION_START();
				SAFETY_DATA_CHECK( RefInput );
				/* V ref Ready */
				RefInput.RefReady = TRUE;
				/* I'm reading compensation channel */
				RefInput.RefVal = res;
				SAFETY_DATA_UPDATE( RefInput );
				SAFETY_REGION_END();
			}

			CBF_AnalogInputIntManager_Polling();
		}
	}
	
#else

	i=0;
	/*  Read all enabled channel "one reading per call" except V line Channel if present.*/
	/* N.B.: con questo metodo il periodo di campionamento di un canale non e' costante ma dipende da quanti
	   canali di tipo automatic sono presenti */

	while ( ( ( AnalogInputParam[AD_Index].analogConfigData.bit.automatic == 0 ) || ( AnalogInputParam[AD_Index].channel == RefInput.Ch_Vline ) ) &&
	        ( i < ADC_NUM_CHANNELLS_USED ) &&
	        ( AD_Index < ADC_NUM_CHANNELLS_USED ) )
	{
		i++;
		AD_Index++;
		if ( AD_Index >= ADC_NUM_CHANNELLS_USED )
		{
			AD_Index=0;
		}
	}


	if ( i >= ADC_NUM_CHANNELLS_USED )
	{
		/* No automatic channels found !!*/
		return;
	}

	/* if Ch_Vline is defined and the channel must be discharged before read it, we discharge it on Ch_Vline
	   with a dummy read */
	if ( ( RefInput.Ch_Vline != CH_VLINE_IS_NOT_DEFINED ) &&
	        ( AnalogInputParam[AD_Index].analogConfigData.bit.chDisch ) )
	{
		(void)OCADC_SampleSync (RefInput.Ch_Vline,AD_DUMMY_POS);
	}

	/* consecutive readings performed 2^AD_LOCAL_SAMPLES_POWER_OF_TWO */
	res = LIB_ADC_Conv_F  ( AD_Index, AD_LOCAL_SAMPLES_POWER_OF_TWO, AD_LOCAL_SAMPLES_DISCARDED );

	/* I'm reading compensation channel */
	if ( AnalogInputParam[AD_Index].channel == RefInput.RefCh )
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK( RefInput );
		
		/* V ref Ready */
		RefInput.RefReady = TRUE;
		/* I'm reading compensation channel */
		RefInput.RefVal = res;
		
		SAFETY_DATA_UPDATE( RefInput );
		SAFETY_REGION_END();
	}

	CBF_AnalogInputIntManager_Polling();

	AD_Index++;
	if (AD_Index >= ADC_NUM_CHANNELLS_USED)
	{
		AD_Index=0;
	}

#endif /*#if (ANALOG_INPUT_MANAGE_POLLING_CONTINUOUS == 1)*/

#else
#error "How have I to manage A/D readings?"     
#endif  /* ATD_INTERRUPT_ENABLED */

#if defined(CONFIG_VLINE_ON_NEG_LEVEL)
	LIB_ADC_VlineMeasOnNegLevel(RefInput.Ch_Vline);
#endif

#ifdef CONFIG_LIB_CLASS_B_ADC
	ADC_Check();
#endif /* CONFIG_LIB_CLASS_B_ADC */

}

/**
\n This function performs, fixed index inside AnalogInputParam[] vector,
\n 2^ad_sample_num_pot2 samples on the channel linked to AnalogInputParam[index] and returns the average.
\n The average is also passed to the functions LIB_ADC_AvgFilter or LIB_ADC_DeltaCtrl.

\param  [in] index index of AnalogInputParam[].
\param  [in] ad_sample_num_pot2 2^ad_sample_num_pot2 = numbers of reading to do.
\param  [in] samples_to_discard readings to discharge.

\attention \b AnalogInputParam[channel] (global) R.

\return 	ADC_ValueType

\retval		average average of readings

\author 	Stefano ughi
\date		29/06/2010
\version	1.0.2


\note	    \par \b Nota 1:
            added parameter samples_to_discard, Number of readings to be discharged\n
            due to floating channels problem.

*/

ADC_ValueType LIB_ADC_Conv_F  ( uint8_t _index, uint8_t ad_sample_num_pot2, uint8_t samples_to_discard)
{
	analog_t *pAnalogInputParam;
	ADC_ValueType adValue;
	lowFilterADCsum_t lf_sum;
#if (VLINEMEASURE_COMP_FACTOR != 0)
	ADC_ValueType tmp_adValue;
#endif
	uint8_t i;
	uint8_t localSamples;

	pAnalogInputParam = &AnalogInputParam[_index];
	localSamples = (uint8_t)(1u<<ad_sample_num_pot2);
	lf_sum = 0;

	for ( i = 0; i < localSamples; i++ )
	{
		(void)OCADC_SampleSync(pAnalogInputParam->channel,pAnalogInputParam->analogInputRamPos);
		adValue = OCADC_GetSync(pAnalogInputParam->analogInputRamPos);

		if ( i >= samples_to_discard )
		{
			lf_sum += (lowFilterADCsum_t)adValue;
		}
	}

	if (localSamples>samples_to_discard)
	{
		adValue = (ADC_ValueType)(lf_sum/(uint8_t)(localSamples-samples_to_discard));
	}
	else
	{
		/* Serious RunTime Error */
		adValue = ONE_AD_SAMPLE_MAX_VALUE;
		PLT_ERROR_WARNING(PLT_ERROR_ANALOG_1);
	}

#if (VLINEMEASURE_COMP_FACTOR != 0)
	/* V line compensation */
	if ( pAnalogInputParam->channel == RefInput.Ch_Vline )
	{
		tmp_adValue = (ADC_ValueType) (adValue+VLINEMEASURE_COMP_FACTOR);
		if ( tmp_adValue > ONE_AD_SAMPLE_MAX_VALUE )
		{
			adValue = ONE_AD_SAMPLE_MAX_VALUE;
		}
		else
		{
			adValue=tmp_adValue;
		}
	}
#endif /* #if (VLINEMEASURE_COMP_FACTOR != 0) */
	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( AnalogInputParam,      _index );

	if ( pAnalogInputParam->analogConfigData.bit.fal == 0 )
	{
		/* Don't care that between current value and previous there is a difference of more than
		   a fixed value.*/
		LIB_ADC_AvgFilter(_index,adValue);
	}
	else
	{
		/* Between current value and previous must be a difference less than a fixed value.*/
		LIB_ADC_DeltaCtrl(_index,adValue);
	}
	SAFETY_DATA_ELEM_UPDATE( AnalogInputParam,      _index );
	SAFETY_REGION_END();

	return adValue;
}

/**
This function returns TRUE if all A/D channels labeled as automatic were acquired, otherwise FALSE.

\attention \b AnalogInputParam[i] (global) R.
\attention \b RefInput.Ch_Vline (global) R.
\attention \b AnalogInputReady (global) R.
\attention \b AnalogInputParam[i].AnalogInputRamPos (global) R.

\return 	bool_t

\retval		TRUE
\retval     FALSE

\author 	Stefano Ughi
\date		28/06/2010
\version	1.0.0

*/
bool_t LIB_ADC_AreAnalogInputsReady(void)
{
	uint8_t i;
	bool_t retVal=TRUE;

	for ( i=0; (i<ADC_NUM_CHANNELLS_USED) && (retVal == TRUE); i++)
	{
		if ( AnalogInputParam[i].analogConfigData.bit.automatic
		        && (AnalogInputParam[i].channel != RefInput.Ch_Vline)
		   )
		{
			if ( (AnalogInputReady & (analogInput_t)(1u << AnalogInputParam[i].analogInputRamPos)) == 0)
			{
				retVal = FALSE;
			}
		}
	}
	
	return retVal;
}
/**
This function returns TRUE if A/D convertion of the channel linked to position "pos" is ready\n
otherwise FALSE.

\param  [in] pos A/D channel position vs AnalogInput[] vector.
\attention \b AnalogInputReady bitmap (global) R.
\attention \b AnalogInputParam[pos].AnalogInputRamPos (global) R.

\return 	bool_t

\retval		TRUE    conversion ready
\retval		FALSE   conversion not ready

\author 	Stefano Ughi
\date		01/07/2010
\version	1.0.0

*/

bool_t LIB_ADC_IsAnalogReady(uint8_t pos)
{
	return (bool_t) (((((AnalogInputReady & (1u << AnalogInputParam[pos].analogInputRamPos)) == 0))?(FALSE):(TRUE)));
}
/**
This function returns TRUE if A/D convertion of the channel linked to position "pos" is ready\n
otherwise FALSE.

\param  [in] pos A/D channel position vs AnalogInput[] vector.
\attention \b ApplianceAnalogInputReady bitmap (global) R.
\attention \b AnalogInputParam[pos].AnalogInputRamPos (global) R.

\return 	bool_t

\retval		TRUE    conversion ready
\retval		FALSE   conversion not ready

\warning    The function is reserved to the Appliance.

\author 	Stefano Ughi
\date		06/07/2010
\version	1.0.0

*/

bool_t LIB_ADC_IsApplAnalogReady(uint8_t pos)
{
	return (bool_t) (((((ApplianceAnalogInputReady & (1u << pos)) == 0))?(FALSE):(TRUE)));
}
/**
This function given pos, position of Analog inside AnalogInput[], reset the correspondent bit\n
inside  ApplianceAnalogInputReady var.

\param  [in] pos A/D channel position vs AnalogInput[] vector.
\attention \b ApplianceAnalogInputReady bitmap (global) W.

\return 	bool_t

\retval		TRUE    conversion ready
\retval		FALSE   conversion not ready

\warning    The function is reserved to the Appliance.

\author 	Stefano Ughi
\date		06/07/2010
\version	1.0.0

*/

void LIB_ADC_ResetApplAnalogReady(uint8_t pos)
{
	PLT_ASSERT(pos < ADC_NUM_CHANNELLS_USED)

	ApplianceAnalogInputReady&=(~(1u << pos));
}

/**
This function fills the global A/D result (AnalogInput[]) in position pos with value.

\param  [in] pos offset vs AnalogInput[] where to store the value.
\param  [in] value value to store.
\attention \b AnalogInput[]	 (global) W.

\author 	Stefano Ughi
\date		28/06/2010
\version	1.0.0

*/
void LIB_ADC_SetAnalogValue( uint8_t pos, ADC_ValueType value )
{
	PLT_ASSERT(pos < ADC_NUM_CHANNELLS_USED)

	SAFETY_REGION_START();
	AnalogInput[pos] = value;
	SAFETY_DATA_ELEM_UPDATE( AnalogInput, pos );
	SAFETY_REGION_END();

}
/**
This function returns the A/D result in position pos of AnalogInput[].

\param  [in] pos offset vs AnalogInput[].
\attention \b AnalogInput[]	 (global) R.
\return 	ADC_ValueType

\retval		A/D coversion result

\author 	Stefano Ughi
\date		29/06/2010
\version	1.0.0

*/
ADC_ValueType LIB_ADC_GetAnalogValue(uint8_t pos)
{
	PLT_ASSERT(pos < ADC_NUM_CHANNELLS_USED)
	return AnalogInput[pos];
}

/**
This function disables measure on A/D channel, init all variables related to the measure\n
on that channel and restart readings.

\param  [in]	channel A/D channel

\attention \b AnalogInputParam[] (global) R/W
\attention \b AD_ChInputReady used as W

\author 	Stefano Ughi
\date		11/02/2008
\version	1.0.0

\author 	Stefano Ughi
\date		01/07/2010
\version	1.0.1

\author 	Alessandro Sabbatini
\date		13/06/2014
\version	1.0.2

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			Ver 1.0.1 added call to LIB_ADC_ADChVsAnalogInputParamIndex
			\par \b Nota 3:
			Ver 1.0.2 removed constraint of Automatic flag set after calling this function: previous state will be restored

*/
#if (MANAGE_AD_CHANNEL_READ==TRUE)
void LIB_ADC_InitMeasureForCh ( uint8_t channel )
{
	analog_t *pAnalogInputParam;
	uint8_t _index;
	bool_t tmpAuto;

	_index = LIB_ADC_ADChVsAnalogInputParamIndex(channel);
	if (AD_INDEX_NOT_FOUND == _index)
	{
		return;
	}

	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( AnalogInputParam,      _index );

	pAnalogInputParam = &AnalogInputParam[_index];

	tmpAuto = pAnalogInputParam->analogConfigData.bit.automatic;
	pAnalogInputParam->analogConfigData.bit.automatic = 0;
	LIB_ADC_ResetAnalogMeasure(_index);
	AD_ChInputReady &= (analogInput_t)(~(1u<<pAnalogInputParam->analogInputRamPos));
	if (tmpAuto)
	{
		pAnalogInputParam->analogConfigData.bit.automatic = 1;
	}

	SAFETY_DATA_ELEM_UPDATE( AnalogInputParam,      _index );
	SAFETY_REGION_END();

}
#endif /* #if (MANAGE_AD_CHANNEL_READ==TRUE) */
/**
The function returns TRUE if reading on A/D channel "channel" is ready otherwise FALSE.

\param  [in]	channel A/D channel

\attention \b AD_ChInputReady (global) used as R
\attention \b AnalogInputParam[index].analogInputRamPos (global) used as R


\author 	Stefano Ughi
\date		11/02/2008
\version	1.0.0

\author 	Stefano Ughi
\date		01/07/2010
\version	1.0.1

\warning    index not tested

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			Ver 1.0.1 added call to LIB_ADC_ADChVsAnalogInputParamIndex

*/
#if (MANAGE_AD_CHANNEL_READ==TRUE)

bool_t LIB_ADC_IsNoAutoMeasureReady( uint8_t channel )
{
	uint8_t _index;

	_index = LIB_ADC_ADChVsAnalogInputParamIndex(channel);

	if ( AD_ChInputReady & (1u<<AnalogInputParam[_index].analogInputRamPos) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#endif  /* #if (MANAGE_AD_CHANNEL_READ==TRUE) */

/**
This function disables measure on A/D channel linked to pos, init all variables related to the measure\n
on that channel and restart readings.

\param  [in]    pos position where the A/D measure is stored

\attention \b AnalogInputParam[] (global) R/W
\attention \b AD_ChInputReady used as W


\author 	Stefano Ughi
\date		05/07/2010
\version	1.0.0

\author 	Alessandro Sabbatini
\date		13/06/2014
\version	1.0.1

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			Ver 1.0.1 removed constraint of Automatic flag set after calling this function: previous state will be restored

*/
#if (MANAGE_AD_CHANNEL_READ==TRUE)
void LIB_ADC_InitMeasureForChInPos ( uint8_t pos )
{
	analog_t *pAnalogInputParam;
	uint8_t _index;
	bool_t tmpAuto;

	_index = LIB_ADC_AnalogInputParamIndexVsPos(pos);
	if (AD_INDEX_NOT_FOUND == _index)
	{
		return;
	}
	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( AnalogInputParam,      _index );

	pAnalogInputParam = &AnalogInputParam[_index];
	tmpAuto = pAnalogInputParam->analogConfigData.bit.automatic;
	pAnalogInputParam->analogConfigData.bit.automatic = 0;
	LIB_ADC_ResetAnalogMeasure(_index);
	AD_ChInputReady &= (analogInput_t)(~(1u<<pAnalogInputParam->analogInputRamPos));
	if (tmpAuto)
	{
		pAnalogInputParam->analogConfigData.bit.automatic = 1;
	}

	SAFETY_DATA_ELEM_UPDATE( AnalogInputParam,      _index );
	SAFETY_REGION_END();

}
#endif /* #if (MANAGE_AD_CHANNEL_READ==TRUE) */
/**
This function returns if measure on A/D channel linked to pos is ready.

\param  [in]    pos position where the A/D measure is stored

\attention \b AnalogInputParam[] (global) R/W
\attention \b AD_ChInputReady used as W


\return 	bool_t

\retval		TRUE measure ready
\retval		FALSE measure not ready

\author 	Stefano Ughi
\date		05/07/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
#if (MANAGE_AD_CHANNEL_READ==TRUE)
bool_t LIB_ADC_IsNoAutoMeasureForChInPosReady ( uint8_t pos )
{
	uint8_t _index;

	_index = LIB_ADC_AnalogInputParamIndexVsPos(pos);
	if ( AD_ChInputReady & (1u<<AnalogInputParam[_index].analogInputRamPos) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
#endif /* #if (MANAGE_AD_CHANNEL_READ==TRUE) */

/* Search functions */

/**
This function, given index of AnalogInputParam[], \n
returns the position where is stored the A/D measure.

\param  [in] index index of AnalogInputParam[]

\return 	uint8_t

\retval		position inside AnalogInput[] if success
\retval     AD_POS_NOT_FOUND

\author 	Stefano Ughi
\date		05/07/2010
\version	1.0.0

*/
uint8_t LIB_ADC_ADposVsIndex(uint8_t _index)
{
	if (_index < ADC_NUM_CHANNELLS_USED)
	{
		return(AnalogInputParam[_index].analogInputRamPos);
	}
	else
	{
		return(AD_POS_NOT_FOUND);
	}
}
/**
This function, given an A/D channel, returns result position inside AnalogInputParam[].

\param  [in] channel A/D channel

\return 	uint8_t

\retval		position inside AnalogInputParam[] if success
\retval     AD_POS_NOT_FOUND

\author 	Stefano Ughi
\date		28/06/2010
\version	1.0.0

*/
uint8_t LIB_ADC_AnalogChRamPos(uint8_t channel)
{
	uint8_t i;
	for ( i=0; i<ADC_NUM_CHANNELLS_USED; i++)
	{
		if ( AnalogInputParam[i].channel == channel )
		{
			return(AnalogInputParam[i].analogInputRamPos);
		}
	}
	return AD_POS_NOT_FOUND;
}
/**
This function, given an A/D channel, returns the index of AnalogInputParam[] related to that channel. .

\param  [in] channel A/D channel

\return 	uint8_t

\retval		index inside AnalogInputParam[] if success
\retval     AD_INDEX_NOT_FOUND

\author 	Stefano Ughi
\date		01/07/2010
\version	1.0.0

*/
uint8_t LIB_ADC_ADChVsAnalogInputParamIndex(uint8_t channel)
{
	uint8_t i;
	for ( i=0; i<ADC_NUM_CHANNELLS_USED; i++)
	{
		if ( AnalogInputParam[i].channel == channel )
		{
			return(i);
		}
	}

	return AD_INDEX_NOT_FOUND;

}
/**
This function, given pos,position vs AnalogInput[] where the A/D result is stored,\n
returns the index of AnalogInputParam[] related to.

\param  [in] pos ram position where is stored A/D result

\return 	uint8_t

\retval		index inside AnalogInputParam[] if success
\retval     AD_INDEX_NOT_FOUND

\author 	Stefano Ughi
\date		05/07/2010
\version	1.0.0

*/
uint8_t LIB_ADC_AnalogInputParamIndexVsPos(uint8_t pos)
{
	uint8_t i;
	for ( i=0; i<ADC_NUM_CHANNELLS_USED; i++)
	{
		if ( AnalogInputParam[i].analogInputRamPos == pos )
		{
			return(i);
		}
	}
	return AD_INDEX_NOT_FOUND;
}
/**
Fixed index, displacement vs AnalogInputParam[],this functions reset the A/D measure.

\param  [in] index index of AnalogInputParam[].

\attention \b AnalogInputParam[index] (global) W


\author 	Stefano Ughi
\date		02/07/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
void LIB_ADC_ResetAnalogMeasure(uint8_t _index)
{
	analog_t *pAnalogInputParam;
	
	SAFETY_REGION_START();
	
	pAnalogInputParam = &AnalogInputParam[_index];
	pAnalogInputParam->analogInputIndex = 0;
	pAnalogInputParam->analogInputSum = 0;
	pAnalogInputParam->analogInputOldValue = 0;
	pAnalogInputParam->analogInputCnt = 0;
	
	SAFETY_DATA_ELEM_UPDATE( AnalogInputParam, _index );
	SAFETY_REGION_END();
}

/**
This function, given index of AnalogInputParam[] and new value to set,
overrides automatic flag in setting file

\param  [in] _index index of AnalogInputParam[].
\param  [in] newVal New value to be set

\attention \b AnalogInputParam[index] (global) W


\author 	Alessandro Sabbatini
\date		18/06/2014
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Alessandro Sabbatini

*/
void LIB_ADC_ForceAutoRead(uint8_t _index, bool_t newVal)
{
	SAFETY_REGION_START();
	SAFETY_DATA_ELEM_CHECK( AnalogInputParam, _index );

	AnalogInputParam[_index].analogConfigData.bit.automatic = newVal;

	SAFETY_DATA_ELEM_UPDATE( AnalogInputParam, _index );
	SAFETY_REGION_END();
}

/**
This function check if Vref is in range.

\attention \b RefInput.RefCh (global) R
\attention \b RefInput.RefReady (global) R
\attention \b RefInput A/D channel readings on Vref channel (global) R.
\attention \b GoodRef (global) W

\return 	bool_t

\retval		FALSE Vref channel not present or GoodRef not significant.
\retval     TRUE GoodRef significant.

\author 	Stefano Ughi
\date		07/07/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_ADC_CheckRef(void)
{
	static ADC_ValueType PreviousVref;
	static bool_t FirstSample = FALSE;

	ADC_ValueType avg;
	bool_t retValue = FALSE;

	if ( ( RefInput.RefCh != CH_FOR_COMPENSATION_IS_NOT_DEFINED ) && (TRUE == RefInput.RefReady) )
	{
		if ( FirstSample == TRUE )
		{
			/*lint -esym(727,PreviousVref) previousVref is init before
			  enter here !! cfr. FirstSample */
#if (ADC_RESOLUTION==16)
			avg = (ADC_ValueType)(((uint32_t)PreviousVref + RefInput.RefVal)/2);
#else
			avg = (ADC_ValueType)((PreviousVref + RefInput.RefVal)/2);
#endif
			/*lint +esym(727,PreviousVref) */

			if ( ( avg < REF_MIN_ADC_UNITS ) || ( avg > REF_MAX_ADC_UNITS ) )
			{
				GoodRef = FALSE;
			}
			else
			{
				GoodRef = TRUE;
			}

			if ( ( avg < BOTTOM_BOUND_RTN_ADC_UNITS ) || ( avg > UPPER_BOUND_RTN_ADC_UNITS ) )
			{
				RtnOutRange = TRUE;
			}
			else
			{
				RtnOutRange = FALSE;
			}

			retValue = TRUE;
		}

		PreviousVref = RefInput.RefVal;
		FirstSample = TRUE;
	}

	return retValue;

}
/**
This functions returns TRUE if Vref in range otherwise FALSE.

\attention \b GoodRef (global) R.

\return 	bool_t

\retval		TRUE/FALSE


\author 	Stefano Ughi
\date		07/07/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
bool_t LIB_ADC_Is_Ref_Good(void)
{
	return GoodRef;
}


/**
Returns TRUE if Vref measure, taking into account tolerances of hardware components, is out of range.

\return 	bool_t

\retval		TRUE/FALSE


\author 	Roberto Fioravanti
\date		08/11/2012
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Roberto Fioravanti

*/
bool_t LIB_ADC_Is_RTN_Out_Range(void)
{
    return RtnOutRange;
}

/**
This function performs V peak measure.

\param  [out]    pVlinePeak

\return 	bool_t

\retval		TRUE  V Line Channel not defined
\retval		FALSE V peak ready


\author 	Stefano Ughi
\date		07/07/2010
\version	1.0.0


\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
#if (N_SAMPLE_V_LINE<0)
#error "N_SAMPLE_V_LINE must be >= 0"
#endif
#if (N_SAMPLE_V_LINE>255)
#error "N_SAMPLE_V_LINE must be <= 255"
#endif
bool_t LIB_ADC_VLineMeasure  (ADC_ValueType *pVlinePeak)
{
	V_Line_t vLine;
	uint8_t pos;
	uint8_t _index;

	(void)memset(&vLine,0,sizeof(vLine));

	if (CH_VLINE_IS_NOT_DEFINED==RefInput.Ch_Vline)
	{
		return TRUE;
	}

#if defined(CONFIG_LIB_TRACE_SUPPORT)
	TRACE_LINE_PEAK_START();
#endif

	_index = LIB_ADC_ADChVsAnalogInputParamIndex(RefInput.Ch_Vline);

	do
	{
		vLine.newVPeak = LIB_ADC_Conv_F (_index, (uint8_t)0, (uint8_t)0);

		if ( vLine.newVPeak > vLine.vPeak )
		{
			vLine.vPeak = vLine.newVPeak;
		}
		vLine.samples ++;
	}
	while ( vLine.samples < N_SAMPLE_V_LINE );

	/*
	   Vline adjustment is performed inside LIB_ADC_Conv_F (index, 0, 0) and in any case
	   saturated to ONE_AD_SAMPLE_MAX_VALUE
	   check VLINEMEASURE_COMP_FACTOR
	*/

	/* Updates AnalogInput[] where we store VLine */
	pos = LIB_ADC_AnalogChRamPos(RefInput.Ch_Vline);

	if ( AD_POS_NOT_FOUND != pos ){
		LIB_ADC_SetAnalogValue( pos, vLine.vPeak );
	}

#if (CHECK_VLINE_CROSSING == 1)
	/* Only for statistics */
	ManageVLineRefCrossing(vLine.vPeak);
#endif

	*pVlinePeak= vLine.vPeak;

#if defined(CONFIG_LIB_TRACE_SUPPORT)
	TRACE_LINE_PEAK_END();
#endif

	return FALSE;
}

/**
Returns RefInput (A/D).

\return 	ADC_ValueType

\retval		RTN A/D value

\author 	stefano.ughi
\date		Mar 9, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
ADC_ValueType LIB_ADC_GetRefInput(void)
{
	return(RefInput.RefVal);
}
/**
Returns if RefInput is ready.

\return 	bool_t

\retval		TRUE RefInput.RefReady is ready
\retval		FALSE RefInput.RefReady is not ready

\author 	stefano.ughi
\date		Mar 9, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t LIB_ADC_IsRefInputReady(void)
{
	return (RefInput.RefReady);
}

/**
Return Compensation Channel position

\return 	uint8_t		Compensation Channel position

\author 	Alessandro Sabbatini
\date		Feb 20, 2014
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
uint8_t LIB_ADC_GetChCompPos(void)
{
	return RefInput.RefCh;
}

/* Local Function */

/**
Fixed index, displacement vs AnalogInputParam[],this functions performs the average over n samples\n
and insert the value in the global variable AnalogInput[].

\param  [in] index index of AnalogInputParam[].
\param  [in] conversion A/D measure.

\attention \b AnalogInputParam[index] (global) R/W
\attention \b RefInput.RefCh A/D compensation channel  R
\attention \b AnalogInputDisable (global) R
\attention \b AnalogInput[adChPos] (global) A/D output W
\attention \b AnalogInputReady (global) W


\author 	Stefano Ughi
\date		02/07/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Nota 2:
			added  pAnalogInputParam->analogConfigData.bit.automatic = 0;\n
			under condition  #if (MANAGE_AD_CHANNEL_READ == TRUE)

*/

static void LIB_ADC_AvgFilter ( uint8_t _index, ADC_ValueType conversion )
{
	analog_t *pAnalogInputParam = &AnalogInputParam[_index];
	ADC_ValueType adAverage;
	uint8_t nSamplesToCollect;
	uint8_t adChCompPos;
	uint8_t adChPos = pAnalogInputParam->analogInputRamPos;
	
	/* updates the average */
	pAnalogInputParam->analogInputSum += (ad_sum_t)conversion;
	/* updates the readings's  counter */
	pAnalogInputParam->analogInputIndex++;
	
	SAFETY_DATA_CHECK(AnalogInputDisable);
	
	nSamplesToCollect = (uint8_t)(1u<<pAnalogInputParam->analogConfigData.bit.nRead);

	if (pAnalogInputParam->analogInputIndex >= nSamplesToCollect)
	{
		/* if channel is enabled */
		if ((AnalogInputDisable & (1u<<adChPos)) == 0)
		{

			adAverage = (ADC_ValueType) (pAnalogInputParam->analogInputSum/nSamplesToCollect);

			if (pAnalogInputParam->analogConfigData.bit.comp)
			{
				/* channel must be compesated with RTN reference */
				/* Get position where RefCh is stored */
				adChCompPos = LIB_ADC_AnalogChRamPos(RefInput.RefCh);
				if ( AD_POS_NOT_FOUND != adChCompPos )
				{
					ADC_ValueType tmpAdCompVal = AnalogInput[adChCompPos];
					uint32_t adCompesatedValue;

					/* Check if RefCh A/D measure is ready and has valid value */
					if ( (  ( AnalogInputReady & (1u<<adChCompPos) ) ) &&
							( tmpAdCompVal > COMP_MIN_ADC_UNITS) &&
							( tmpAdCompVal < COMP_MAX_ADC_UNITS) )
					{
						adCompesatedValue = (((uint32_t)adAverage*AD_COMP_VAL)/tmpAdCompVal);

						/* Get rid an overflow value */
						if (adCompesatedValue > ONE_AD_SAMPLE_MAX_VALUE)
						{
							adCompesatedValue = ONE_AD_SAMPLE_MAX_VALUE;
						}

						/* Override new Analog average value with compensated one */
						adAverage = (ADC_ValueType)adCompesatedValue;
					}
				}
				else
				{
					/* Compensation's channel not found ! Serious error */
					PLT_ERROR_WARNING(PLT_ERROR_ANALOG_2);
				}
			}

			/* Set value as is - good if no compensation needed or it cannot be applied */
			AnalogInput[adChPos] = adAverage;

			SAFETY_DATA_ELEM_UPDATE(AnalogInput, adChPos);

			AnalogInputReady |= (analogInput_t)(1u<<adChPos);
			/* Flag for Appliance */
			ApplianceAnalogInputReady |= (analogInput_t)(1u<<adChPos);
			
#if (MANAGE_AD_CHANNEL_READ == TRUE)
			AD_ChInputReady |= (analogInput_t)(1u<<adChPos);
#endif
		}
		
		LIB_ADC_ResetAnalogMeasure(_index);
	}
}


/**
Fixed index, displacement vs AnalogInputParam[],this functions performs a control between\n
current ADC reading and old ADC reading, if the distance between the measures is is less than or equal\n
to a certain setting parameter the new measure is validated.

\param  [in] index index of AnalogInputParam[].
\param  [in] newADvalue A/D measure.

\attention \b AnalogInputParam[index] (global) R/W

\author 	Stefano Ughi
\date		05/07/2010
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

static void LIB_ADC_DeltaCtrl( uint8_t _index, ADC_ValueType newADvalue)
{
	analog_t *pAnalogInputParam;
	ADC_ValueType delta;
	ADC_ValueType maxVar;

	pAnalogInputParam = &AnalogInputParam[_index];

	delta = (ADC_ValueType)labs((int32_t)newADvalue-(int32_t)pAnalogInputParam->analogInputOldValue);
	maxVar = (ADC_ValueType)(1u<<pAnalogInputParam->analogConfigData.bit.fal);

	if (pAnalogInputParam->analogInputCnt < ANALOG_SYNC_THR )
	{
		/* synchronization phase */
		if (0 == pAnalogInputParam->analogInputCnt)
		{
			pAnalogInputParam->analogInputOldValue = newADvalue;
			pAnalogInputParam->analogInputCnt++;
		}
		else
		{
			if (delta <= maxVar)
			{
				pAnalogInputParam->analogInputOldValue = newADvalue;
				pAnalogInputParam->analogInputCnt++;
			}
			else
			{
				/* restart synch. */
				pAnalogInputParam->analogInputCnt = 0;
			}
		}
	}
	else
	{
		if (delta <= maxVar)
		{
			/* Valid data */
			LIB_ADC_AvgFilter (_index, newADvalue);
		}
		else
		{
			/* Restart acquisition */
			pAnalogInputParam->analogInputSum = 0;
			pAnalogInputParam->analogInputIndex = 0;
		}
		pAnalogInputParam->analogInputOldValue = newADvalue;
	}
}
#if defined (ATD_INTERRUPT_ENABLED)
/*lint -esym(715,error)*/
/*lint -esym(715,notifVal)*/
static IO_ErrorType EndConvCallback (IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
	uint8_t position;
	uint8_t index;
	ADC_ValueType adValue;
#if (VLINEMEASURE_COMP_FACTOR != 0)
	ADC_ValueType tmp_adValue;
#endif

	position = LIB_ADC_AnalogChRamPos(chid);
	if (AD_POS_NOT_FOUND == position)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
	adValue = OCADC_GetSync(position);
	index = LIB_ADC_ADChVsAnalogInputParamIndex(chid);
	if (AD_INDEX_NOT_FOUND == index)
	{
		return (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
#if (VLINEMEASURE_COMP_FACTOR != 0)
	/* Vline compensation */
	if(AnalogInputParam[index].channel==RefInput.Ch_Vline)
	{
		tmp_adValue = (adValue+VLINEMEASURE_COMP_FACTOR);
		if ( tmp_adValue > ONE_AD_SAMPLE_MAX_VALUE )
		{
			adValue = ONE_AD_SAMPLE_MAX_VALUE;
		}
		else
		{
			adValue=tmp_adValue;
		}
	}
#endif /*(VLINEMEASURE_COMP_FACTOR != 0)*/
	/* Read compensation channel ? */
	if ( AnalogInputParam[index].channel == RefInput.RefCh )
	{
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(RefInput);

		RefInput.RefVal = adValue;
		RefInput.RefReady = TRUE;  	/* V ref Ready */

		SAFETY_DATA_UPDATE(RefInput);
		SAFETY_REGION_END();

	}

	/* Introduced the control of delta between samples */
	if ( AnalogInputParam[index].analogConfigData.bit.fal == 0 )
	{
		LIB_ADC_AvgFilter( index, adValue );
	}
	else
	{
		LIB_ADC_DeltaCtrl( index, adValue );
	}

	return (IO_ErrorType)IO_E_OK;
}
#endif /* #if defined (ATD_INTERRUPT_ENABLED) */

#if defined(CONFIG_VLINE_ON_NEG_LEVEL)

static void LIB_ADC_VlineMeasOnNegLevel(uint8_t ch_Vline)
{
	uint8_t position;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(RefInput);

	position = LIB_ADC_AnalogChRamPos(RefInput.Ch_Vline);

	if ( AD_POS_NOT_FOUND != position )
	{
          (void)OCADC_SampleSync(RefInput.Ch_Vline,position);

#if (defined(CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES) && (CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES > 1))
          /* Collect new samples */
          RefInput.SumVlineOnNegLev += (ADC_SumValueType) OCADC_GetSync(position);
          RefInput.SumVlineSamples++;

          /* When all samples are collected, give its average in VlineOnNegLev */
          if (RefInput.SumVlineSamples >= CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES)
            {
        	  RefInput.SumVlineOnNegLev = (RefInput.SumVlineOnNegLev / CONFIG_VLINE_ON_NEG_LEVEL_AVGSAMPLES);

        	  /* Check against overflow */
              if (RefInput.SumVlineOnNegLev > ONE_AD_SAMPLE_MAX_VALUE)
              {
                  /* Ch_Vline average value out of range: Serious Error */
            	  RefInput.SumVlineOnNegLev = ONE_AD_SAMPLE_MAX_VALUE;
              }

              /* Set new value and restart collecting samples */
              RefInput.VlineOnNegLev = (ADC_ValueType) RefInput.SumVlineOnNegLev;
              RefInput.SumVlineOnNegLev = 0;
              RefInput.SumVlineSamples = 0;
            }
#else
          RefInput.VlineOnNegLev = OCADC_GetSync(position);
#endif

	}
	else
	{
	    /* Ch_Vline channel not defined: Serious Error */
		RefInput.VlineOnNegLev = ONE_AD_SAMPLE_MAX_VALUE;
	}

	SAFETY_DATA_UPDATE(RefInput);
	SAFETY_REGION_END();

}

ADC_ValueType LIB_ADC_GetVlineOnNegLevel(void)
{
    return (RefInput.VlineOnNegLev);
}

#endif /* defined(CONFIG_VLINE_ON_NEG_LEVEL) */

/* *********** */
/* End of file */
/* *********** */

