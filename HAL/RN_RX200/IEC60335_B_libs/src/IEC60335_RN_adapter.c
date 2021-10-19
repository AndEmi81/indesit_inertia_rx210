/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		[Title]
\n			[Brief description]
\n
\file		[File Name] <file's name, use tag ${file} if you are Eclipse's user>
\ingroup	[module_name_slave] <Symbolic name used by Doxygen to group files belonging to the same module>
\date		[Date] <Creation date, format dd/mm/yyyy, use tag ${date} if you are Eclipse's user >
\version	01.00.00 <Start Project's BaseLine>
\author		[Developer] <Name and Surname of Developer, use tag ${user} if you are Eclipse's user >
\n
\n
\Par\b	History:
\n
\n 		01.04.00 <Current Project's BaseLine>
\n		[Date] <Date of Current Project's BaseLine, format dd/mm/yyyy>
\n		[Developer] <Name and Surname of Developer>
\n		changes: <Changes's Description>
\n 		- change 1 <Description of change 1>
\n		- change 2 <Description of change 2>\n
\n
\n 		01.03.00 <Previous Project's BaseLine>
\n		[Date] <Date of Previous Project's BaseLine, format dd/mm/yyyy >
\n		[Developer] <Name and Surname of Developer>
\n		changes: <Changes's Description>
\n 		- change 1 <Description of change 1>
\n		- change 2 <Description of change 2>
\n
\n
\n etc.
*/

#define MODULE_BODY_CLASSBCHECK

/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "IEC60335.h"
#include "interrupt_handlers.h"
#include "LIB_Error.h"
#include "OCRST_drv.h"
#include "Common/misratypes.h"
#include "CPU/cpu_test.h"
#include "RAM/ramtest_march_c.h"
#include "RAM/ramtest_stack.h"
#include "Clock/clock_monitor.h"
#include "ADC12/test_adc12.h"
#include "IWDT/iwdt.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

#if defined(CONFIG_LIB_CLASS_B_RAM)
/* safe ram must be outside the block to be tested, tested by ad hoc function*/
static uint8_t SafeRam[CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE]  			__attribute__ ((section (".CLASS_B_temp")));
static int SafeStack[12];
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */

/*CPU TEST*/
void CPU_Test_ErrorHandler(void)
{
#if defined(CONFIG_PL_DEBUG)
	PLT_ERROR_FATAL(PLT_ERROR_CLASSBCHECK_CPU);
#else
	OCRST_GenerateUserResetSync();
#endif
}

void IEC60335_CPUregTest_BIST(void)
{
	CPU_Test_All();
}

/*RAM TEST*/

#if defined(CONFIG_LIB_CLASS_B_RAM)

/*
Generic test
\param [in]	startAddrs	start address
\param [in]	length in bytes
\param [in]	isBIST boolean indicating whether BIST or POST test
*/
bool_t IEC60335_RamMarchC (const uint32_t startAddrs, const uint32_t length, bool_t isBIST)
{
	void* p_RAMSafe=0;
	if(0 != isBIST)
	{
		p_RAMSafe=SafeRam;
	}
	return RamTest_March_C(startAddrs, startAddrs+(length-1), p_RAMSafe);
}

/*
safe ram destructive test : POST - Pre-Operation Self test and BIST - built-in self test
*/
bool_t IEC60335_RamMarchC_SafeRam (void)
{
	return RamTest_March_C((uint32_t)SafeRam, (uint32_t)(SafeRam+sizeof(SafeRam)-1), (void* const)0);
}

/*
User Stack test
\param [in]	startAddrs	start address
\param [in]	length in bytes
\param [in]	isBIST boolean indicating whether BIST or POST test
*/
bool_t IEC60335_RamMarchC_UserStack (const uint32_t startAddrs, const uint32_t length, bool_t isBIST)
{
	void* p_RAMSafe=0;
	if(0 != isBIST)
	{
		p_RAMSafe=SafeRam;
	}
	return RamTest_Stack_User(startAddrs, startAddrs+(length-1), p_RAMSafe,(uint32_t) SafeStack + sizeof(SafeStack)-1, RamTest_March_C);
}
/*
Int Stack test
\param [in]	startAddrs	start address
\param [in]	length in bytes
\param [in]	isBIST boolean indicating whether BIST or POST test
*/
bool_t IEC60335_RamMarchC_IntStack (const uint32_t startAddrs, const uint32_t length, bool_t isBIST)
{
	/*in POST (isBIST is FALSE): isBIST is ignored because Int stack is used */
	return RamTest_Stack_Int(startAddrs, startAddrs+(length-1), SafeRam,(uint32_t) SafeStack + sizeof(SafeStack)-1, RamTest_March_C);
}

#endif

/*Clock Error Management ------------------------------*/

#if defined(CONFIG_LIB_CLASS_B_CLOCK_HW)

void IEC60335_initClockTestHW(void)
{
#if defined(CONFIG_WDT_ENABLED) /* funziona solo se l'IWDT e' fatto partire*/
	ClockMonitor_Init(NULL);
#endif
}

/* Clock test main loop handler is not necessary
 * Failure is already performed by peripheral exception
 */

type_testResult IEC60335_Clocktest_MainLoopHandler(void)
{
	return IEC60335_testPassed;
}

void INT_Excep_FERRF(void)
{
	PLT_ERROR_FATAL(PLT_ERROR_CLASSBCHECK_CLOCK);
}

void INT_Excep_OVFF(void)
{
	PLT_ERROR_FATAL(PLT_ERROR_CLASSBCHECK_CLOCK);
}

#endif

/* --- WDT TEST -------------------------------------------------------*/
/*!
 * @brief IEC60335 WD Test
 */

type_testResult IEC60335_WDTest(void)
{
	IWDT_Init(0,0,0,0,0);
	return IEC60335_testPassed;
}

type_testResult IEC60335_ADCtest_POST(void)
{
	type_testResult retVal=IEC60335_testPassed;

	Test_ADC12_Init();

	if(!Test_ADC12_Wait()){
		retVal = IEC60335_testFailed;
	}
	return retVal;
}

void IEC60335_ADCtest_BITS_start(void)
{
	Test_ADC12_Start(0);
}

type_testResult IEC60335_ADCtest_BITS_check(void)
{
	type_testResult retVal=IEC60335_testPassed;

	if(!Test_ADC12_CheckResult(FALSE))
		retVal = IEC60335_testFailed;

	return retVal;
}

/* *********** */
/* End of file */
/* *********** */


