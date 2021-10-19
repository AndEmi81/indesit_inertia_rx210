/****************************************************************************
 *   $Id:: IEC60335_B_ClockTest.c 7461 2011-06-01 13:01:58Z gerritdewaard   $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B Clock system testing 
 *	   library.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/

#include "IEC60335.h"

typedef struct tag_ClkTest
{		
	UINT32 timerTestThreshold;		/*!< Used in the mainloop function, defines the number of calls to start occurrence test*/
	UINT32 rtcTestThreshold;		/*!< Used in the mainloop function, defines the number of calls to start occurrence test*/
	UINT32 rtcOccCounter;			/*!< Counter variable for the mainloop, if value reached the defined threshold, the occurrence test is started*/
	UINT32 timerOccCounter;			/*!< Counter variable for the mainloop, if value reached the defined threshold, the occurrence test is started*/
	BOOL timerOccured;				/*!< This bool will be set in the timer function, and is reset durring occurrence test*/
	BOOL rtcOccured;				/*!< This bool will be set in the rtc function, and is reset durring occurrence test*/
	UINT32 timerCounter;			/*!< The counter Variable, to test the timer to be within its bounds*/
	UINT32 timerBoundLower;			/*!< The estimated minimum count of cycle occurencies (Threshold for timer test). */
	UINT32 timerBoundUpper;			/*!< The estimated maximum count of cycle occurencies (Threshold for timer test). */
	BOOL timerOutOfBounds;			/*!< within this bool, the rtc timer test signals the error state to the main function*/
	BOOL timerCounterOverflow;		/*!< Reflects, if the TimerCounter was flew over due an error */
} type_ClockTest;

type_ClockTest clkTst = 
{
	0,
	0,
	0,
	0,
	FALSE,
	FALSE,
	0,
	0,
	0,
	FALSE,
	FALSE
};

/***************************************************************************//**
** @brief 		The IEC60335_resetClockTest function clears and resets all used Clock Test variables 
** @param[in]	NONE
** @return		NONE
*****************************************************************************/
void IEC60335_resetClockTest(void);
void IEC60335_resetClockTest(void)
{
	// general
	clkTst.rtcOccCounter = 0;			/*!< The counter Variable*/
	clkTst.timerOccCounter = 0;			/*!< The counter Variable*/

	clkTst.timerCounter = 0;

	clkTst.timerOccured = FALSE;
	clkTst.rtcOccured = FALSE;
	clkTst.timerOutOfBounds = FALSE;

	clkTst.timerCounterOverflow = FALSE;
}

/***************************************************************************//**
** @brief 		This function initiates the various variables used during the Clock Test. 
** @param[in]	timerOccThreshold	Initiates the threshold value that defines the number of calls that started the timer occurrence test.
** @param[in]	rtcOccThreshold		Initiates the threshold value that defines the number of calls that started the RTC occurrence test.
** @param[in]	timerLowerBound		This variable sets the lower bound value of the number of timer or RTC test occurrences.
** @param[in]	timerUpperBound		This variable sets the upper bound value of the number of timer or RTC test occurrences.
** @return		NONE
*****************************************************************************/
void IEC60335_initClockTest(UINT32 timerOccThreshold, UINT32 rtcOccThreshold, UINT32 timerLowerBound, UINT32 timerUpperBound)
{
	// general
	clkTst.timerTestThreshold = timerOccThreshold;		/*!< The counter Variable*/
	clkTst.rtcTestThreshold = rtcOccThreshold;			/*!< The counter Variable*/

	clkTst.timerBoundLower = timerLowerBound;			/*!< The estimated minimum count of cycle occurencies (Threshold for MainLoop). */
	clkTst.timerBoundUpper = timerUpperBound;			/*!< The estimated maximum count of cycle occurencies (Threshold for MainLoop). */	

	IEC60335_resetClockTest();
}

/***************************************************************************//**
** @brief 		This function represents the part of the IEC60335 Class B clock test which has to be executed within the main loop.
**				It checks several thing:
**					- If the clock test timer interrupts are triggered
**					- If the clock test rtc interrupt is triggered
**					- or, If in any of the two interrupts an error was detected...
**				This function must be called once inside the main loop.
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
** @attention 	For this function, it is necessary to estimate the count how often this function could be called. This is important to find 
**				valid threshold values, which are used to test timer and rtc interrupt occurrence.
*****************************************************************************/
type_testResult IEC60335_Clocktest_MainLoopHandler(void)
{
	type_testResult Result = IEC60335_testPassed;

	clkTst.rtcOccCounter = clkTst.rtcOccCounter + 1;
	clkTst.timerOccCounter = clkTst.timerOccCounter + 1;

	if(clkTst.rtcOccCounter == 0)
	{
		Result = IEC60335_testFailed;
	}

	if(clkTst.timerOccCounter == 0)
	{
		Result = IEC60335_testFailed;
	}

	if(clkTst.rtcOccCounter >= clkTst.rtcTestThreshold)
	{
		clkTst.rtcOccCounter = 0;

		if(clkTst.rtcOccured == FALSE)
		{
			Result = IEC60335_testFailed;
		}
		else
		{
			clkTst.rtcOccured = FALSE;

			if(clkTst.timerOutOfBounds == TRUE)
			{
				Result = IEC60335_testFailed;
				clkTst.timerOutOfBounds = FALSE;
			}
		}
	}

	if(clkTst.timerOccCounter >= clkTst.timerTestThreshold)
	{
		clkTst.timerOccCounter = 0;

		if(clkTst.timerOccured == FALSE)
		{
			Result = IEC60335_testFailed;
		}
		else
		{
			clkTst.timerOccured = FALSE;
		}
	}
	
	if(Result == IEC60335_testFailed)
	{
		IEC60335_resetClockTest();
	}

	return Result;
}

/***************************************************************************//**
** @brief 		This function is intended to use as timer interrupt service handler 
**				or to be called once inside the timer interrupt service handler.
** @param[in]	NONE
** @return		NONE
*****************************************************************************/
void IEC60335_Clocktest_TimerIntHandler(void)
{
	clkTst.timerCounter = clkTst.timerCounter + 1;

	if(clkTst.timerCounter == 0)
	{
		clkTst.timerCounter = 0xFFFFFFFF;
		clkTst.timerCounterOverflow = TRUE;
	}

	clkTst.timerOccured = TRUE;
}


 /***************************************************************************//**
** @brief 		This function is intended to be called once inside the custom rtc interrupt service handler. It can't be used as service handler by itself, because of the 
**				return value that has to be evaluated after the call.
**				This function tests the timer time frame and in this case the CPU frequency.
**				Also, this function checks if the main loop function was called.
** @return		NONE
*****************************************************************************/
void IEC60335_Clocktest_RTCHandler(void)
{
	clkTst.rtcOccured = TRUE;

/* check if timer is still in frame */
	if((clkTst.timerCounter < clkTst.timerBoundLower) || (clkTst.timerCounter > clkTst.timerBoundUpper))
	{
		clkTst.timerOutOfBounds = TRUE;
	}
	else
	{
		clkTst.timerOutOfBounds = FALSE;
	}

	clkTst.timerCounter = 0;
}
