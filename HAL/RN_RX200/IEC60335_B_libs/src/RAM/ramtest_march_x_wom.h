/***********************************************************************************
 DISCLAIMER	: We (Renesas Electronics Europe Limited) do NOT warrant that the Software
			  is free from claims by a third party of copyright, patent, trademark,
			  trade secret or any other intellectual property infringement.

			  Under no circumstances are we liable for any of the following:

			  1. third-party claims against you for losses or damages;
			  2. loss of, or damage to, your records or data; or
			  3. economic consequential damages (including lost profits or savings)
			  	 or incidental damages, even if we are informed of their possibility.

			  We do not warrant uninterrupted or error free operation of the Software.
			  We have no obligation to provide service, defect, correction, or any
			  maintenance for the Software. We have no obligation to supply any
			  Software updates or enhancements to you even if such are or later become
			  available.

			  IF YOU DOWNLOAD OR USE THIS SOFTWARE YOU AGREE TO THESE TERMS.

			  THERE ARE NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE IMPLIED
			  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

FILE NAME  	: 	ramtest_march_x_wom.h

DESCRIPTION	: 	RAM memory test based on March X algorithm.
				The standard March X algorithm has been converted for 
				Word Oriented Memories to decrease the run time.
				An extra 2 March elements have been added for detection
				of intra word faults.
				The memory access width is defined in the header,
				see RAMTEST_MARCH_X_WOM_ACCESS_SIZE.
				The maximum ram test area is defined in the header,
				see RAMTEST_MARCH_X_WOM_MAX_WORDS.

				The 6 steps of the algorithm:-
				(Note: The data values written depend on the defined access width,
				below is an example if using 16 bits.)				
				STEP1 - Write all zeros to RAM.
				STEP2 - Starting at lowest address,
						 check read zeros, write ones(0xFFFF).
				STEP3 - Starting at highest address,
						 read ones(0xFFFF), write zeros(0x0000).
				STEP4 - Starting at lowest address,
						 check read zeros, write 0xAAAA.
				STEP5 - Starting at highest address,
						 read 0xAAAA, write 0x5555.
				STEP6 - Check read all 0x5555 from RAM.
***********************************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
24-Feb-2009 RTE-PJI First Release
28-May-2009 VDE (Version 1.0)
***********************************************************************************/

#ifndef _RAMTEST_MARCH_X_WOM_H_INCLUDED_
#define _RAMTEST_MARCH_X_WOM_H_INCLUDED_


/***********************************************************************************
 User Includes
***********************************************************************************/
#include "misratypes.h"


/**********************************************************************************
 #defines
***********************************************************************************/
/*Set the RAM access size.
Define RAMTEST_MARCH_X_WOM_ACCESS_SIZE to be one of the following:*/
#define RAMTEST_MARCH_X_WOM_ACCESS_SIZE_8BIT 	1
#define RAMTEST_MARCH_X_WOM_ACCESS_SIZE_16BIT	2
#define RAMTEST_MARCH_X_WOM_ACCESS_SIZE_32BIT	3

#define RAMTEST_MARCH_X_WOM_ACCESS_SIZE 	RAMTEST_MARCH_X_WOM_ACCESS_SIZE_8BIT

/*Limiting the maximum size of the test area may decrease the run time of the test.
This limiting is done by reducing the maximum number of words that the test area
can consist of so that a variable of a smaller type can be used.
The size of a word is defined by the RAM_ACCESS_SIZE.
Define RAMTEST_MARCH_X_WOM_MAX_WORDS to be one of the following:*/
#define RAMTEST_MARCH_X_WOM_MAX_WORDS_8BIT  	1 /*(h'FF words)*/
#define RAMTEST_MARCH_X_WOM_MAX_WORDS_16BIT 	2 /*(h'FFFF words)*/
#define RAMTEST_MARCH_X_WOM_MAX_WORDS_32BIT 	3 /*(h'FFFFFFFF words).*/

#define RAMTEST_MARCH_X_WOM_MAX_WORDS 		RAMTEST_MARCH_X_WOM_MAX_WORDS_32BIT

/***********************************************************************************
 Function Prototypes
***********************************************************************************/
bool_t RamTest_March_X_WOM(const uint32_t ui32_StartAddr,
							 	const uint32_t ui32_EndAddr,
								void* const p_RAMSafe);
								
bool_t RamTest_March_X_WOM_Extra(const uint32_t ui32_StartAddr,
							 	const uint32_t ui32_EndAddr,
								void* const p_RAMSafe);

#endif
