/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**********************************************************************
* File Name : Test_ADC12.c
* Version : 1.00
* Device : RX110
* Tool Chain :
* H/W Platform : 
* Description : Self Test code.
*               Test ADC12 using ADC12 module diagnostic mode.
**********************************************************************/

/**********************************************************************
Includes
**********************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "RN_RX100_reg.h"
#include "RN_RX100_hal.h"
#include "Test_ADC12.h"


/**********************************************************************
Defines
**********************************************************************/
#define ADC12_INTERRUPT_PRIORITY 0x05

/*Expected results*/
#if 0
#define VALUE_12_BIT_FULL_SCALE 0xFFF
#define VALUE_12_BIT_HALF_SCALE 0x800
#define VALUE_12_BIT_ZERO_SCALE 0x000
#endif
#define VALUE_10_BIT_FULL_SCALE 1023
#define VALUE_10_BIT_HALF_SCALE 540
#define VALUE_10_BIT_ZERO_SCALE 0

#define VALUE_10_BIT_RTN_MIN  468
#define VALUE_10_BIT_RTN_MAX  598

#define VALUE_10_BIT_SONDA_MIN  8
#define VALUE_10_BIT_SONDA_MAX  980

/*The actual result is allowed to be within
a +/- tolerence (ADC12_TOLERANCE) of the expected.
Note: This is initially set as the maximum possible absoulute inaccuracy.
A calibrated system will be able to tighten this tolerence.*/
/* #define ADC12_TOLERANCE 20 */
#define ADC10_TOLERANCE 5

/**********************************************************************
Variables
**********************************************************************/
/*Error Callbacks*/
static ADC12_ERROR_CALL_BACK g_ADC12_ErrorCallBack = NULL;

/********************************************************************
Prototypes
**********************************************************************/

/**********************************************************************
Functions
**********************************************************************/


/***********************************************************************************
Function Name : Test_ADC12_Init

Description   :	Initialise the ADC12 module.

Parameters    : 
Return value  : 
***********************************************************************************/
#if defined( CONFIG_CK2_STRIP )
void Test_ADC12_Init(void)
{
	/*Disable register protection (PRC1)*/
	SYSTEM.PRCR.WORD = 0xA502;	
	/*Enable the ADC12 Module*/
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0;
	/*Re-Enable register protection*/
	SYSTEM.PRCR.WORD = 0xA500;
}
#elif defined( CONFIG_CK2_ENTRY )
/*MS TODO: */
#else
#error "BOARD type not defined"
#endif
/***********************************************************************************
Function Name : Test_ADC12_Wait

Description   :	Suitable as a power on test rather than as a periodic test.
				
Parameters    :
				 
Return value  : TRUE = test passed. FALSE = test failed.
***********************************************************************************/
static uint8_t currTestCh;/*MS TODO: proteggere con datacare */
volatile int dbg_delayCarica=100;

#if defined(CONFIG_CK2_STRIP)
#define ADC_CH_REF_GND	15
#define ADC_CH_REF_VCC2	9
#define ADC_CH_REF_RTN  1
#define ADC_CH_SONDA	8

#define ADC_MIN_10BIT_CHECK_HI	1000
#define ADC_MIN_10BIT_CHECK_LO	10
#define ADC_PINTEST_MAX_TEST	4
#define ADC_PINTEST_MAX_FAIL	2

uint8_t checkPin(int ch, int checkType)
{
	volatile int cntTestFail=0;
	volatile bool_t bResult = TRUE;
	volatile uint16_t value;
	volatile int cntCheck;
	int n;
	DISABLE_PORT_REGISTER_WRITE_PROTECTION();

	uint8_t savePMR  = PORTE.PMR.BYTE;
	uint8_t savePDR  = PORTE.PDR.BYTE;
	uint8_t savePCR  = PORTE.PCR.BYTE;
	uint8_t savePODR = PORTE.PODR.BYTE;

	switch(ch){
	case 8:
		MPC.PE0PFS.BIT.ASEL = 0; /* Pin Function Control Register: 0: Used as other than an analog pin, 1: Used as an analog pin */
		PORTE.PMR.BIT.B0 = 0; /* configura il bit come IO */
		PORTE.PDR.BIT.B0 = 1; /* as output */
		PORTE.PCR.BIT.B0 = 1; /* attiva pulup */
		PORTE.PODR.BIT.B0 =1; /* 1=uscita alta 0=uscita bassa*/
	break;
	case 9:
		MPC.PE1PFS.BIT.ASEL = 0; /* Pin Function Control Register: 0: Used as other than an analog pin, 1: Used as an analog pin */
		PORTE.PMR.BIT.B1 = 0; /* configura il bit come IO */
		PORTE.PDR.BIT.B1 = 1; /* as output */
		PORTE.PCR.BIT.B1 = 1; /* attiva pulup */
		PORTE.PODR.BIT.B1 =1; /* 1=uscita alta 0=uscita bassa*/
	break;
	case 15:
		MPC.PE7PFS.BIT.ASEL = 0; /* Pin Function Control Register: 0: Used as other than an analog pin, 1: Used as an analog pin */
		PORTE.PMR.BIT.B7 = 0; /* configura il bit come IO */
		PORTE.PDR.BIT.B7 = 1; /* as output */
		PORTE.PCR.BIT.B7 = 1; /* attiva pulup */
		PORTE.PODR.BIT.B7 =1; /* 1=uscita alta 0=uscita bassa*/
	break;

	}
	S12AD.ADANSA.WORD = 1<< ch;     /* seleziona il canale */
	if(1 == checkType){ /* check multiplexer */
		for(cntCheck=0; cntCheck<ADC_PINTEST_MAX_TEST; cntCheck++)
		{

			if((cntCheck%2)==0){
				switch(ch){
					case 8:
						PORTE.PODR.BIT.B0 =1; /* 1=uscita alta */
					break;
					case 9:
						PORTE.PODR.BIT.B1 =1; /* 1=uscita alta */
					break;
					case 15:
						PORTE.PODR.BIT.B7 =1; /* 1=uscita alta */
					break;
				}

			}else{
				switch(ch){
				case 8:
					PORTE.PODR.BIT.B0 =0; /* 0=uscita bassa*/
				break;
				case 9:
					PORTE.PODR.BIT.B1 =0; /* 0=uscita bassa*/
				break;
				case 15:
					PORTE.PODR.BIT.B7 =0; /* 0=uscita bassa*/
				break;
				}
			}
			for( n=0; n< dbg_delayCarica; n++){
				value++;
			}

			S12AD.ADCSR.WORD  = 0x8000;   /* start conversion */

			while(S12AD.ADCSR.BIT.ADST != 0) {}

			value = *(((uint16_t*)&(S12AD.ADDR0))+ch);
			value >>= 2;

			if((cntCheck%2)==0){
				if( value < ADC_MIN_10BIT_CHECK_HI){
					cntTestFail++;
				}
			}else{
				if( value > ADC_MIN_10BIT_CHECK_LO){
					cntTestFail++;
				}
			}
		}
	}else{ /* check pin flottante */
		for(cntCheck=0; cntCheck < ADC_PINTEST_MAX_TEST; cntCheck++)
		{
			for( n=0; n< dbg_delayCarica; n++){
				value++;
			}
			S12AD.ADCSR.WORD  = 0x8000;   /* start conversion */

			while(S12AD.ADCSR.BIT.ADST != 0) {}

			value = *(((uint16_t*)&(S12AD.ADDR0))+ch);
			value >>= 2;

			if( value < ADC_MIN_10BIT_CHECK_HI){
				cntTestFail++;
			}
		}
	}

	PORTE.PMR.BYTE  = savePMR;
	PORTE.PDR.BYTE  = savePDR;
	PORTE.PCR.BYTE  = savePCR;
	PORTE.PODR.BYTE = savePODR;

	ENABLE_PORT_REGISTER_WRITE_PROTECTION();

	if(cntTestFail > ADC_PINTEST_MAX_FAIL){
		bResult = FALSE;
	}

	return !bResult;
}

bool_t Test_ADC12_Wait(void)
{	
	bool_t bResult = TRUE;
	uint16_t errMask=0;

	errMask |= checkPin(ADC_CH_SONDA,   0) << ADC_CH_SONDA;
	errMask |= checkPin(ADC_CH_REF_VCC2,0) << ADC_CH_REF_VCC2;
	errMask |= checkPin(ADC_CH_REF_GND, 0) << ADC_CH_REF_GND;
	/*NOTA: check su rn non possibile, non è sulla porta E */

	if( errMask != 0){
		bResult = FALSE;
	}
	return bResult;
}
#elif defined(CONFIG_CK2_ENTRY)

#else
#error " non board type"
#endif
/***********************************************************************************
Function Name : Test_ADC12_Start

Description   :

				
Parameters    : Callback: A function to call if an error is detected. 
Return value  :
***********************************************************************************/
void Test_ADC12_Start(const ADC12_ERROR_CALL_BACK Callback)
{	
	/*Store error callback*/
	g_ADC12_ErrorCallBack = Callback;
	currTestCh = ADC_CH_REF_GND;
}

/***********************************************************************************
Function Name : Test_ADC12_CheckResult

Description   :	Check the ADC diagnostic result is as expected.
				This must be called after Test_ADC12_Start.
				It must be called periodically or whenever an ADC conversion completes.
				This function check the ranges of security ch and multiplexer test.
			NOTA: il test sul ultiplexer potrebbe essere fatto piu raramente.

Parameters    :  bCallErrorHandler: Set True if want error handler to be called
				 					if an error is detected.
									Note it is the users responsibility to ensure
									the error callback has been set correctly using
									Test_ADC12_Start function.
									
Return value  : TRUE = test passed. FALSE = test failed.
***********************************************************************************/
bool_t Test_ADC12_CheckResult(const bool_t bCallErrorHandler)
{
	volatile uint16_t Value;
	volatile uint16_t Expected_Max=0;
	volatile uint16_t Expected_Min=0;
	volatile bool_t bResult = TRUE;
	volatile uint32_t  nextCh=ADC_CH_REF_GND;
	/* Start Convertion */
	ADC_ConfigurePinAsChannel(currTestCh);
	/*lint -e506 */
	ADC_StartConvertionOnChannel(currTestCh,ADC_INT_DISABLED);
	/*lint +e506 */

	ADC_WaitForConvertionOnChannel(currTestCh);

	Value = ADC_GetConvertedValueOnChannel(currTestCh);

	/*The Data Register Diag (ADRD) can be right or left aligned*/
	if(1 == S12AD.ADCER.BIT.ADRFMT)
	{
		/*Left aligned*/
		Value >>= 4;
	}
	
	
	/*Expected result depends upon test voltage used.
	Calculate an allowable range based on the given TOLERANCE.*/
	switch(currTestCh)
	{
		case ADC_CH_REF_GND:
			Expected_Min = VALUE_10_BIT_ZERO_SCALE;
			Expected_Max = VALUE_10_BIT_ZERO_SCALE + ADC10_TOLERANCE;
			bResult = !checkPin(ADC_CH_REF_GND,   1); /*test su multiplexer */
			nextCh = ADC_CH_REF_VCC2;
			break;
		case ADC_CH_REF_VCC2:
			Expected_Min = VALUE_10_BIT_HALF_SCALE - ADC10_TOLERANCE;
			Expected_Max = VALUE_10_BIT_HALF_SCALE + ADC10_TOLERANCE;
			bResult = !checkPin(ADC_CH_REF_VCC2,   1); /*test su multiplexer */
			nextCh = ADC_CH_REF_RTN;
			break;
		case ADC_CH_REF_RTN:
			Expected_Min = VALUE_10_BIT_RTN_MIN - ADC10_TOLERANCE;
			Expected_Max = VALUE_10_BIT_RTN_MAX + ADC10_TOLERANCE;
			/*NOTA: non possibilie checkPin(ADC_CH_REF_RTN,   1); test su multiplexer */
			nextCh = ADC_CH_SONDA;
			break;
		case ADC_CH_SONDA:
			Expected_Min = VALUE_10_BIT_SONDA_MIN - ADC10_TOLERANCE;
			Expected_Max = VALUE_10_BIT_SONDA_MAX + ADC10_TOLERANCE;
			bResult = !checkPin(ADC_CH_SONDA,   1); /*test su multiplexer */
			nextCh = ADC_CH_REF_GND;
			break;
		default:
			bResult = FALSE;
	}
		
	/*Check value is within expected range*/
	if((Value > Expected_Max) || (Value < Expected_Min))
	{
		if( (currTestCh != ADC_CH_REF_VCC2)
			&& (currTestCh != ADC_CH_SONDA)
		){ /*MS NOTA: baord senze segnale di riferimento corretto */
			/*Test Failed*/
			bResult = FALSE;
		}
		
		/*Call error callback?*/
		if(TRUE == bCallErrorHandler)
		{
			g_ADC12_ErrorCallBack();
		}
	}	
	currTestCh = nextCh;
	return bResult;
}
