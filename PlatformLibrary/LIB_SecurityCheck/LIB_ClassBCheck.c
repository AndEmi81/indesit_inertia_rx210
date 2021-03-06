/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n	Il file contiene le funzioni per il drive del bloccoporta ad impulsi
\n
\file		LIB_ClassBCheck.c
\ingroup    LIB_ClassB
\date	    20/09/2011
\version	01.00.00
\author		 Marco Sperandini
\n
\n
\par	History:
\n
\n 		01.00.00
\n		20/09/2011
\n		marco.sperandini
\n		changes
\n 			Creation
\n
*/
#define MODULE_BODY_CLASSBCHECK
/* ******** */
/* Includes */
/* ******** */

#include "Config.h"
#include "LIB_ClassBCheck.h"
#include "LIB_Error.h"
#include "LIB_Trace.h"
#include "LIB_Setting.h"


#if   defined(NXP_LPC12xx_1227)
#include "IEC60335_B_CPUregTests.h"
#include "IEC60335_B_RAMTestBIST.h"
#include "IEC60335_B_WDTest.h"

#elif defined(FREESCALE_MCF51_AG128)
#include "IEC60335.h"
#include "IEC60335_B_CPUregTests.h"
#include "IEC60335_B_RAMTestBIST.h"
#include "IEC60335_B_WDTest.h"

#elif defined(RN_RX200) || defined(RN_RX100)
	#include "IEC60335.h"
	/*#include "iwdt.h"*/
#endif

#if defined(CONFIG_LIB_CLASS_B_USE_OCCRC)
#include "OCCRC_drv.h"
#define LIB_ClassBChk_CRC_calculate OCCRC_Calculate
#else
#include "SWCRC_drv.h"
#define LIB_ClassBChk_CRC_calculate SWCRC_Calculate
#endif
#include "OCWDT_drv.h"
#include "LIB_Reset.h"
#include "LIB_ClassBVar.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
#define CHECKRAM_AREA_NUM		(sizeof(RamAreaTestTable)/sizeof(ClassB_RamTestDescriptorType))

#if defined(CONFIG_ERROR_HANDLING)
#define CLASSB_FATAL(err)		PLT_ERROR_FATAL(err)
#define CLASSB_WARNING(err)		PLT_ERROR_WARNING(err)
#else
#define CLASSB_FATAL(err) 		LIB_Reset_ForceColdReset()
#define CLASSB_WARNING(err)		{}
#endif

typedef struct ClassB_CheckRamTestState_s
{
	uint8_t		AreaIndex;
	uint32_t	StartAddress;
	uint32_t	RemainingBytes;
}ClassB_CheckRamTestStateType;
/* **************************** */
/* Prototype of local functions */
/* **************************** */
#if defined (CONFIG_LIB_CLASS_B_FLASH)
static void CheckFlash_SetBlock(uint8_t newBlock);
#endif

#if defined(CONFIG_LIB_CLASS_B_RAM)
static uint8_t CheckRAM_Manager(bool_t useCriticalSection);
static void CheckRAM_Init(void);
#endif


/* ****************** */
/* Exported variables */
/* ****************** */


/* **************** */
/* Global variables */
/* **************** */
/*
.CLASS_B_temp must be an area with no destructive test
*/
static volatile ClassB_CheckRamTestStateType ClassB_CheckRamTestState __attribute__ ((section (".CLASS_B_temp")));

#ifdef CONFIG_LIB_CLASS_B_FLASH
uint16_t flashChkCodeEval;
#endif

#ifdef CONFIG_LIB_CLASS_B_ADC
/** AD Check failure filter counter */
uint16_t	ADC_CheckFilter;
#endif

/* ***************** */
/* Exported function */
/* ***************** */
#if defined(CONFIG_LIB_CLASS_B_CPU) && !defined(BOOTLOADER_IEC60335)
void RegCheck(void)
{
	TRACE_CPU_CHECK_START();
	IEC60335_CPUregTest_BIST();
	TRACE_CPU_CHECK_END();
}
#endif /* CONFIG_LIB_CLASS_B_CPU*/

#ifdef CONFIG_LIB_CLASS_B_FLASH
typedef struct
{
    uint8_t* curAddr;
    uint8_t* endAddr;
    uint16_t partialChecksum;
    uint8_t  currBankIdx;
} FlashTestStatus_t;

static FlashTestStatus_t flashTestStatus;
typedef enum {  FLASH_TEST_NONE,
                FLASH_TEST_SETTING_RUNNING,
                FLASH_TEST_COMPLETE
}
FlashTestStatusStep_t;


static void CheckFlash_SetBlock(uint8_t newBlock)
{

	uint16_t len;
	if (newBlock == FLASH_BLOCK_SETTING_FILE)
	{
		LIB_SettingBIST_StartSync(&len, &(flashTestStatus.curAddr),&(flashTestStatus.partialChecksum));
		flashTestStatus.endAddr = flashTestStatus.curAddr+len;
	}
	else
	{
		if ((flashTestStatus.currBankIdx == FLASH_BLOCK_SETTING_FILE) ||
		(newBlock != (flashTestStatus.currBankIdx + 1)) ||
		( newBlock >= NUM_FLASH_CBOARD_TEST_BLOCKS ))
		{
			newBlock = 0;
			flashTestStatus.partialChecksum = 0xffff;
		}

		flashTestStatus.curAddr = (uint8_t *)AreaFlashCheckTable[newBlock].startAddr;
		flashTestStatus.endAddr = (uint8_t *)AreaFlashCheckTable[newBlock].endAddr;
	}

	flashTestStatus.currBankIdx = newBlock;
}

void CheckFlash_Init(bool_t isBIST)
{
	if (isBIST)
	{
	    SAFETY_REGION_START();
	}

    CheckFlash_SetBlock(0);
    SAFETY_DATA_UPDATE(flashTestStatus);

    if (isBIST)
	{
    	SAFETY_REGION_END();
	}
}

uint8_t CheckFlash_Manager(uint16_t byte_to_check, bool_t skipSettingFile)
{
    uint8_t* tmpEndAddr;
    uint32_t lenght;
    uint8_t retValue=CHECK_RUNNING;

    TRACE_FLASH_CHECK_START();

    if (!skipSettingFile)
    {
        SAFETY_REGION_START();
    }

    SAFETY_DATA_CHECK(flashTestStatus);

    tmpEndAddr = flashTestStatus.curAddr + byte_to_check;

    if( tmpEndAddr > flashTestStatus.endAddr)
    {
        tmpEndAddr = flashTestStatus.endAddr;
    }

    lenght = tmpEndAddr - flashTestStatus.curAddr;

    if( flashTestStatus.currBankIdx == FLASH_BLOCK_SETTING_FILE )  /* per il segmento del file dei settings */
    {
        flashTestStatus.partialChecksum = LIB_SettingBIST_StepSync( lenght, flashTestStatus.curAddr, flashTestStatus.partialChecksum );
    }
    else
    {
        flashTestStatus.partialChecksum = LIB_ClassBChk_CRC_calculate( lenght, flashTestStatus.curAddr, flashTestStatus.partialChecksum );
    }

    flashTestStatus.curAddr = tmpEndAddr;

    if (tmpEndAddr >= flashTestStatus.endAddr)
    {
        if (flashTestStatus.currBankIdx < (NUM_FLASH_CBOARD_TEST_BLOCKS-1))
        {
            CheckFlash_SetBlock(flashTestStatus.currBankIdx+1);
        }
        else
        {
            if( flashTestStatus.currBankIdx == FLASH_BLOCK_SETTING_FILE )  /* per il segmento del file dei settings */
            {
                if ( LIB_SettingBIST_EndSync(flashTestStatus.partialChecksum)  )
                {
                    retValue = CHECK_PASSED;
                }
                else
                {
                	retValue = CHECK_FAILED;
                }
            }
            else {
            	flashChkCodeEval = flashTestStatus.partialChecksum;

            	/* FlashCheckCode can be == 0 to disable test; production firwmare cannot have FlashCheckCode == 0! */
                if (FlashCheckCode && (flashTestStatus.partialChecksum != FlashCheckCode))
                {
                	retValue = CHECK_FAILED;
                }else if (skipSettingFile)
                {
                    retValue = CHECK_PASSED;
                }
                else
                {
                    CheckFlash_SetBlock(FLASH_BLOCK_SETTING_FILE);
                }

            }
        }
    }

    SAFETY_DATA_UPDATE(flashTestStatus);

    if (!skipSettingFile)
    {
    	SAFETY_REGION_END();
    }

    TRACE_FLASH_CHECK_END();

    switch( retValue )
	{
	case CHECK_FAILED:
		CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_FLASH);
		break;
	case CHECK_PASSED:
		CheckFlash_Init(!skipSettingFile);
		break;
	}

    return retValue;
}
#endif /* CONFIG_LIB_CLASS_B_FLASH*/

#ifdef CONFIG_LIB_CLASS_B_RAM
#if defined(__ICCARM__)     /* EWARM */
#pragma location = ".noinit"
static uint8_t tempRam[CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE];
#elif defined(__CWCC__)       /* Codewarrior */

#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */

NO_INIT_VAR uint8_t tempRam[CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE];
#endif


/**
This API implements keywords BIST (Built-In Self Test)
*/
void LIB_ClassB_Keywords_BIST(void)
{
	if( TRUE != LIB_CheckKeywordsRam())
	{
		LIB_Reset_ForceColdReset();
	}
}

/**
 This API implements the state machine of a non destructive RAM BIST (Built-In Self Test) to be performed
 on an area of RAM or stack (nIterrupt or User).
 */
void LIB_ClassB_RAM_BIST(void)
{
	switch (CheckRAM_Manager(TRUE))
	{
	case CHECK_FAILED:
		CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_RAM);
		break;
	case CHECK_PASSED:
	default:
		;
	}
}

#endif /* CONFIG_LIB_CLASS_B_RAM*/

#ifdef CONFIG_LIB_CLASS_B_CLOCK

#if defined(CONFIG_LIB_CLASS_B_CLOCK_MS)
#include "OCRTC_drv.h"
/**
LA sorgente esterna deve essere piu lenta,
ad ogni interrupt della sorgente esterna
va verificato che
*/
typedef struct
{
    int32_t beginSysCnt;
    int32_t endSysCnt;
    int32_t beginExtCnt;
    int32_t endExtCnt;
    int8_t nFailedCheck;
    int8_t failed;
} ClockTestStatus_t;

ClockTestStatus_t clockTestStatus;
//! Fattore di scala tra rtc e tempo di sistema (1ms)
#if defined(FREESCALE_MCF51_AG128)
#define EXT_CLOCK_SCALE	1
#else if   defined(NXP_LPC12xx_1227)
//MS TODO: fix the RTC HW problem
#define EXT_CLOCK_SCALE	10
#endif

//! Tolleranza in percentuale
#define MAX_CLOCK_TOLLERANCE_PERC 10
//! Numero di fallimenti consecutivi per dichiare test fallito
#define MAX_CLOCK_FAILURE 3
void check( uint32_t time );

/**
Inizializza le risorse necessareper il test sulla frequenza della cpu.
Confronta la sorgente di clock da cui dipende quella della cpu e una
sorgente esterna, come RTC.
*/
void CheckClock_Init(void)
{
    clockTestStatus.nFailedCheck = 0;
    clockTestStatus.endExtCnt=0;

    clockTestStatus.beginSysCnt = OS_GetTime32();
    clockTestStatus.beginExtCnt = OCRTC_getTimeSync();

    OCRTC_setAlarmSync(clockTestStatus.beginExtCnt+100, check);
}

void check( uint32_t time )
{
    //clockTestStatus.failed   = 1;
    uint32_t beginSysCnt;
    uint32_t beginExtCnt;

    beginSysCnt = OS_GetTime32();
    beginExtCnt = OCRTC_getTimeSync();

    OCRTC_setAlarmSync(clockTestStatus.beginExtCnt+100, check);
}

void CheckClock_Manager(uint16_t cmd)
{
    int32_t sysLen;
    int32_t sysDiff;
    int32_t extDiff;
    int32_t bound;

    TRACE_CLOCK_CHECK_START();

    clockTestStatus.endSysCnt = OS_GetTime32();
    clockTestStatus.endExtCnt = OCRTC_getTimeSync();

    sysDiff = clockTestStatus.endSysCnt - clockTestStatus.beginSysCnt;
    extDiff = (clockTestStatus.endExtCnt - clockTestStatus.beginExtCnt)* EXT_CLOCK_SCALE;

    clockTestStatus.beginSysCnt = clockTestStatus.endSysCnt;
    clockTestStatus.beginExtCnt = clockTestStatus.endExtCnt;

    bound = (sysDiff/100)*MAX_CLOCK_TOLLERANCE_PERC;
    if(   ((sysDiff - extDiff) > bound)
            || ((extDiff - sysDiff) > bound) )
    {
        clockTestStatus.nFailedCheck++;
        if( clockTestStatus.nFailedCheck > MAX_CLOCK_FAILURE )
        {
        	CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_CLOCK);
        }
    }
    clockTestStatus.nFailedCheck = 0;
    TRACE_CLOCK_CHECK_END();
}

#elif  defined(LIB_CLASS_B_CLOCK_NPX)

#include "OCRTC_drv.h"
/**
NPX Implementation
*/
#include "IEC60335_B_ClockTest.h"
void check( uint32_t time );
void check( uint32_t time )
{

    IEC60335_Clocktest_RTCHandler();  /* agganciato al timer dell'rtc, indipendente da quello di cpu.*/

    OCRTC_setAlarmSync( OCRTC_getTimeSync()+1, check);
}

/**
Inizializza le risorse necessareper il test sulla frequenza della cpu.
Confronta la sorgente di clock da cui dipende quella della cpu e una
sorgente esterna, come RTC.
*/
void CheckClock_Init(void)
{
    IEC60335_initClockTest(  1 // timerOccThreshold
                             , 1 // rtcOccThreshold
                             , 0 // timerLowerBound
                             , 1 // timerUpperBound
                          );
    OCRTC_setAlarmSync( OCRTC_getTimeSync()+1, check);
}
/**
Verifica che le due sorgenti di clock utilzzate per il test non
si discostino troppo.
*/

void CheckClock_Manager(uint16_t cmd)
{
    type_testResult res;

    res = IEC60335_Clocktest_MainLoopHandler();

    if ( IEC60335_testPassed != res )
    {
    	CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_CLOCK);
    }
}
#elif (defined(RN_RX200) || defined(RN_RX100)) && defined(CONFIG_LIB_CLASS_B_CLOCK_HW)

void CheckClock_Init(void)
{
	IEC60335_initClockTestHW();
}

#endif /* else CONFIG_LIB_CLASS_B_CLOCK_MS*/
#endif /* CONFIG_LIB_CLASS_B_CLOCK*/

#if defined(CONFIG_LIB_CLASS_B_WDTEST) && defined(CONFIG_PL_DRV_OCWDT)
typedef enum {
	WDTCHECK_START=0,			/* WDCheck init state, starts WDT and waits for timeout to occurr  */
	WDTCHECK_1,					/* if catches a reset by timeout,
									- Windowed: then starts WDT and issues an untimely refresh
									- normal:  end of check reached */
	WDTCHECK_2,					/* Windowed only:  if catches a reset by untimely refresh, end of check reached*/
	WDTCHECK_END,				/* not used */
	WDTCHECK_ERROR, 			/* for debug purpose only*/
	WDTCHECK_RESTART,			/* init state of WDCheck once end of check has been reached  */
}
WDTCheckStepType;

#define ISTEST_KEY	0x01
static volatile WDTCheckStepType WDTCheckStep;
static void WDCheck(void);

/**
	Initializes WDCheck state machine.
*/
void LIB_ClassB_WDTCheckInit(void)
{
	WDTCheckStep = WDTCHECK_START;
	WDT_ClearTimeoutExpired();
	WDT_ClearUntimelyResetOccurred();
}

/**
	WDT POST tests.
	WDT allows recovering from error situations such as runaway or deadlocked code.

	Test is performed on the same configuration of the final application,
	no custom configuration are adopted  (because IWDT register can be written
	once after reset).

	Preconditions:
	-	Windowed WDT whose timer is clocked independently from the CPU (IWDT);
	-	WDT configured with a predefined timeout and window (if required).
	-	Refresh-permitted range (or window) is end-aligned.
	-	bootloader doesn't affect WDT registers


	It's better if WDT counter not started, but start is issued only if Counter is not running
	to avoid reset by untimely refresh. (In some MCUs it could happen that test is started but WDT
	is already running, like RX200 after sw reset )

	The self-diagnostic routine basically makes sure that:
	-	system reset is issued after the occurrence of a WDT timeout
	-	in window mode only, device is reset upon untimely WDT refresh
	-	in window mode only, ensure that the window is not wider than expected

	he self-diagnostic routine should ensure that WDT counter can be refreshed, but this check can be
	avoided thanks to the architecture of the check and appropriate preconditions.

	At each reset caused by WDT, state machine increments its state. Any other reset
	reinitializes the state machine (WDTCHECK_START).

	The device is reset a number of times during the WDT test, original cause of reset
	is overwritten. Therefore, SRAM variables WDTCheckStep and bkpResetSrc are used
	by the self-diagnostic routine to keep track of the test phase and original reset source
	before the test.

	states:
	WDTCHECK_START: this step starts the WDT counter and waits for the timeout to check whether WDT
	is able to issue a reset by timeout.
	WDTCHECK_1: In windowed mode, if reset by WDT timeout occurred, the state machine starts the WDT
	counter again and refreshes the WDT untimely, that is Refresh-prohibited period (Refresh-permitted
	window is end-aligned).
	In Normal mode, if reset by WDT timeout occurred check is passed.
	WDTCHECK_2: (windowed mode only)if reset by WDT untimely refresh occurred it means that untimely
	refresh is able to cause a reset. Then the state machine is re-initilized WDTCHECK_RESTART.

	In window mode both tests must be passed to return IEC60335_testPassed.

	TODO: avoid WDT reset is a wanted reset occurs? Does WDTCheckStep need to be protected by DataCare?
*/
static void WDCheck(void)
{
	static volatile resetSource_t bkpResetSrc;
	bool_t ret = IEC60335_testFailed;
	RST_Type resetSrc;

	TRACE_WD_CHECK_START();

	/* get the reset source */
	resetSrc = OCRST_GetResetSourceSync();

	/* catch any source of reset except one by WDT and reset the WDCheck state machine*/
	if((RST_BY_WATCHDOG != resetSrc))
	{
		LIB_ClassB_WDTCheckInit();
	}

	/* test whether WDT timeout is able to issue system reset */
	if( !WDT_IsRunning() )
	{
		OCWDT_StartSync();	/*start WDT without refreshing*/
	}

	switch(WDTCheckStep)
	{
		case WDTCHECK_RESTART:
		case WDTCHECK_START:
			/*save original reset source*/
			bkpResetSrc = OCRST_SaveResetValue();

			while(1)
			{
				WDTCheckStep=WDTCHECK_1;
			}
			/*break;*/ /*  lint -e527 (Unreachable code at token 'break') keep break */


		case WDTCHECK_1:

			if(WDT_TimeoutExpired())
			{
				/*OK, WDT is able to issue system reset by timeout*/

				/* reset WDT Status */
				WDT_ClearTimeoutExpired();

#if (WDT_WINDOWED_MODE == 1)
				/* Now it's time to test whether untimely refresh causes a reset */
				WDTCheckStep=WDTCHECK_2;

				/* wait for counter is near the refresh permitted window */
				while( !WDT_IsJustBeforePermittedWindow() )
				{
					/* nothing to to,  wait a moment please */
				}

				/*
				one-shot refresh and then...
				*/
				WDT_Refresh(); /* issue the untimely refresh */

				/*...deadlock!!!
				Deadlock to prevent execution whether untimely refresh didn't cause a reset*/
				while(1)
				{
					/*
					 - as T1 passed, WDT timeout will cause a reset:
					 - As Next state will be WDTCHECK_2, it must catch the error
					*/
				}
				/* Please note:
				'one-shot refresh and then ... deadlock'
				Reasons:
					Due to a fault, it could happen that the refresh permitted window is not the right one
					(compared to desired one):
					a) upper boundary is shifted;
					b) lower boundary is shifted: window will is wider than the desired one
					c) is a) + b), that is a) and b) at the same time (in the worst case no window has been set).

					In the case a) there is not any sort of problem because the window is shorter than the
					desired one (because Refresh-permitted range (or window) is end-aligned).
					In the case b) window is wider than the desired one: the fault will be caught because
					the refresh	is issued in the refresh permitted window and the deadlock prevents any other
					execution of the code.

					In the case c), if the refresh is not able to reset the WD counter (not tested at this time),
					please note that a strategy like

					while(1)
					{
						WDT_Refresh();
					}

					will not catch the fault, because sooner or later, except in the worst case,
					the untimely refresh might be able to cause a reset ('might' because it depends on the
					architecture). If the 'real' window is very wide (e.g lower boundary @ start and upper
					boundary is almost at the end of the WDT period) specifications are not met ( because
					window is wider than expected , usually 50% of the period).

					Conclusion:
					Untimely refresh must be done in a precise instant of time just before the refresh
					permitted window) and must not be repeated (in this way if the refresh is not able to
					reset the WD counter, a reset by timeout will occur (just tested).

					In this way the check is not required to ensure that the WDT counter can be refreshed.
				*/


#else
				/* End of Check sequence */
				OCRST_RestoreResetValue(bkpResetSrc);

				/*start WDT*/
				OCWDT_StartSync();
				ret=IEC60335_testPassed;

				/* state for the reset by WDT timeout */
				WDTCheckStep=WDTCHECK_RESTART;
#endif
			}
			else
			{
				/* error */
				WDTCheckStep=WDTCHECK_ERROR; /* this assignment is for debug purpose only */
			}

			break;

#if (WDT_WINDOWED_MODE == 1)
		case WDTCHECK_2:

			if(WDT_UntimelyResetOccurred())
			{
				/*OK, WDT is able to issue system reset by untimely refresh*/
				/* reset WDT Status */
				WDT_ClearUntimelyResetOccurred();

				/* End of Check sequence */
				OCRST_RestoreResetValue(bkpResetSrc);

				ret=IEC60335_testPassed;

				/* state for the reset by WDT timeout */
				WDTCheckStep=WDTCHECK_RESTART;
			}
			else
			{
				/* error */
				WDTCheckStep=WDTCHECK_ERROR; /* this assignment is for debug purpose only */
			}
			break;
#endif
		default:
			break;

	}

	TRACE_WD_CHECK_END();
	if (ret == IEC60335_testFailed)
	{
		LIB_ClassB_WDTCheckInit();
		CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_WDTEST);
	}
}
#endif

#if !defined(BOOTLOADER_IEC60335)
void InitCheck(void)
{

#ifdef CONFIG_LIB_CLASS_B_WDTEST
	WDCheck();
#endif /* CONFIG_LIB_CLASS_B_WDTEST*/

#ifdef CONFIG_LIB_CLASS_B_CLOCK
    CheckClock_Init();
#endif /* CONFIG_LIB_CLASS_B_CLOCK*/

}
#endif /* !defined(BOOTLOADER_IEC60335) */

void InitMemoryCheck(void)
{
#if defined(BOOTLOADER_IEC60335)
	CheckRAM_Init();
	CheckFlash_Init(FALSE);
#else /* defined(BOOTLOADER_IEC60335) */
	uint8_t checkRes=CHECK_FAILED;
	uint16_t CheckRAM_bytesChecked = 0;

#ifdef CONFIG_LIB_CLASS_B_RAM

	/* RAM POST - Pre-Operation Self Test*/
	CheckRAM_Init();
	checkRes = CHECK_FAILED;
	CheckRAM_bytesChecked = 0;

	do
	{
		checkRes = CheckRAM_Manager(FALSE);

		if(checkRes == CHECK_FAILED)
		{
			CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_RAM);
		}

		CheckRAM_bytesChecked += CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE;
		if (CheckRAM_bytesChecked >= CONFIG_LIB_CLASS_B_RAM_REFRESH_BYTES)
		{
			OCWDT_RefreshSync();
			CheckRAM_bytesChecked = 0;
		}

	}while(checkRes == CHECK_RUNNING);

#endif /* CONFIG_LIB_CLASS_B_RAM*/

#ifdef CONFIG_LIB_CLASS_B_FLASH
	/* ROM POST - Pre-Operation Self Test */
	CheckFlash_Init(FALSE);

	do
	{
		  checkRes = CheckFlash_Manager(CONFIG_LIB_CLASS_B_FLASH_CHUNKSIZE, TRUE);
		  CheckRAM_bytesChecked += CONFIG_LIB_CLASS_B_FLASH_CHUNKSIZE;
		  if( checkRes == CHECK_FAILED )
		  {
			  CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_FLASH);
		  }
		  if (CheckRAM_bytesChecked >= CONFIG_LIB_CLASS_B_FLASH_REFRESH_BYTES)
		  {
			  OCWDT_RefreshSync();
			  CheckRAM_bytesChecked = 0;
		  }
	}
	while( checkRes == CHECK_RUNNING );
#endif
#endif /* defined(BOOTLOADER_IE60335) */

}

#ifdef CONFIG_LIB_CLASS_B_ADC


/**
\n	ADC Check Initialization routine
\n	This function initializes ADC_CheckFilter and its Data care check
\n
\param none

\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/
void ADC_CheckInit(void)
{
	if(IEC60335_testFailed == IEC60335_ADCtest_POST())
	{
		CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_ADC);
	}

	IEC60335_ADCtest_BITS_start();

	ADC_CheckFilter = 0;
	SAFETY_DATA_UPDATE(ADC_CheckFilter);
}

/**
\n	ADC Check Routine
\n	This function calls IEC60335_ADCtest_BITS_check and updates ADC_CheckFilter
	depending of test result; on success decrement its value up to 0, otherwise
	it will be incremented by CONFIG_LIB_CLASS_B_ADC_FLTR_INCR, and once
	CONFIG_LIB_CLASS_B_ADC_FLTR_MAXTH value is reached it will raise a
	Platform exception.
\n
\param none

\return none
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/
void ADC_Check(void)
{
	bool_t retVal;
	TRACE_ADC_CHECK_START();

	retVal = (IEC60335_ADCtest_BITS_check() != IEC60335_testPassed);

	TRACE_ADC_CHECK_END();

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(ADC_CheckFilter);

	if(retVal)
	{
		ADC_CheckFilter += CONFIG_LIB_CLASS_B_ADC_FLTR_INCR;
		if(ADC_CheckFilter > CONFIG_LIB_CLASS_B_ADC_FLTR_MAXTH)
		{
			CLASSB_FATAL(PLT_ERROR_CLASSBCHECK_ADC);
		}
	}
	else
	{
		if (ADC_CheckFilter > 0)
		{
			ADC_CheckFilter--;
		}
	}

	SAFETY_DATA_UPDATE(ADC_CheckFilter);
	SAFETY_REGION_END();

}
#endif

/* ************** */
/* local function */
/* ************** */

#if defined (CONFIG_LIB_CLASS_B_RAM)

/**
\n
\n
\n
\param [in]
\param [in,out]
\param [in]


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/
static void CheckRAM_Init(void)
{
	/* Test Check RAM status area */
	IEC60335_RamMarchC((const uint32_t) &ClassB_CheckRamTestState, sizeof(ClassB_CheckRamTestState), FALSE);

	/* Init Check RAM Manager */
	ClassB_CheckRamTestState.AreaIndex=CHECKRAM_AREA_NUM;
	ClassB_CheckRamTestState.RemainingBytes=0;

    SAFETY_DATA_UPDATE(ClassB_CheckRamTestState);
}
/**
\n BIST is always non destructive
\n POST is always destructive except for the first block in  RamAreaTestTable
\n
\n
\n
\param [in]
\param [in,out]
\param [in]


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\li


*/
static uint8_t CheckRAM_Manager(bool_t isBIST)
{
    uint32_t length;
    uint8_t retValue = CHECK_NOT_RUNNING;
    CLASSB_TEST_FUNC pTestFunc=0;
    volatile ClassB_CheckRamTestStateType* pState= &ClassB_CheckRamTestState;

    TRACE_RAM_CHECK_START();

    if(isBIST)
    {
    	EnterCriticalSection();
    }

    SAFETY_DATA_CHECK(ClassB_CheckRamTestState);

    if(pState->AreaIndex >= CHECKRAM_AREA_NUM)
    {
    	/*	well-initialized state machine start here:
    		it's time to apply destructive test to safe ram
    	*/
    	if(IEC60335_testFailed ==IEC60335_RamMarchC_SafeRam())
    	{
    		retValue = CHECK_FAILED;
    	}
    	else
    	{
    		retValue = CHECK_RUNNING;

			/* update next index */
    		pState->AreaIndex=0;
    		pState->StartAddress = RamAreaTestTable[pState->AreaIndex].startAddr;
    		pState->RemainingBytes = (RamAreaTestTable[pState->AreaIndex].endAddr - RamAreaTestTable[pState->AreaIndex].startAddr) +1;
		}
    }
    else
    {
    	/* test */
    	if(pState->RemainingBytes >= CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE)
		{
			length  = CONFIG_LIB_CLASS_B_RAM_CHUNKSIZE;
		}
		else
		{
			length = pState->RemainingBytes;
		}
    	pState->RemainingBytes -= length;

    	pTestFunc = RamAreaTestTable[pState->AreaIndex].fpTest_Func;
    	if(pTestFunc && (length))
    	{
    		/* by default the first block is non-destructive test like in BIST */
    		if(IEC60335_testFailed == pTestFunc(pState->StartAddress, length, (pState->AreaIndex?isBIST:TRUE)))
    		{
    			retValue = CHECK_FAILED;
    		}
    		else
    		{
    			retValue = CHECK_RUNNING;
    		}
    	}

    	if(retValue == CHECK_RUNNING)
    	{
    		pState->StartAddress += length;

			if(!pState->RemainingBytes)
			{
				pState->AreaIndex++;

				if(pState->AreaIndex < CHECKRAM_AREA_NUM)
				{
					/* configure state machine to go to the next area */
					pState->StartAddress = RamAreaTestTable[pState->AreaIndex].startAddr;
					pState->RemainingBytes = (RamAreaTestTable[pState->AreaIndex].endAddr - RamAreaTestTable[pState->AreaIndex].startAddr) +1;
				}
				else if(pState->AreaIndex == CHECKRAM_AREA_NUM)
				{
					retValue = CHECK_PASSED;
					/* in this branch state machine is already initialized */
				}
				else
				{
					/* error !!! */
					retValue = CHECK_FAILED;
				}
			}
    	}
    	else
    	{
    		/* error !!! */
    		retValue = CHECK_FAILED;
    	}
    }

    SAFETY_DATA_UPDATE(ClassB_CheckRamTestState);
    if(isBIST)
    {
    	ExitCriticalSection();
    }

    TRACE_RAM_CHECK_END();
    return retValue;
}

#endif

/* *********** */
/* End of file */
/* *********** */
