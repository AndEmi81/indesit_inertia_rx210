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

FILE NAME  	: 	ramtest_march_x_wom.c

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

/***********************************************************************************
Includes
***********************************************************************************/
#include <stddef.h> /*Provides NULL declaration*/
#include "misratypes.h"
#include "ramtest_march_x_wom.h"

/**********************************************************************************
 #defines
***********************************************************************************/

/*The selected RAM access size changes the followign #defines.*/
#if (RAMTEST_MARCH_X_WOM_ACCESS_SIZE == RAMTEST_MARCH_X_WOM_ACCESS_SIZE_8BIT)
	/*8 bit access */

	/*Data Patterns used*/
	#define DATA_PATTERN_0s 0x00U
	#define DATA_PATTERN_Fs 0xFFU
	#define DATA_PATTERN_As 0xAAU
	#define DATA_PATTERN_5s 0x55U
	
	/*Number of bytes in WORD*/
	#define BYTES_PER_WORD 1U
	
	/*Used to check an address is 8 bit alligned */
	#define ALIGNMENT_BITS 0x00000000UL
	
	/*Set the variable type used for RAM access*/
	typedef uint8_t    TYPE_ACCESS_t;
			
#elif (RAMTEST_MARCH_X_WOM_ACCESS_SIZE == RAMTEST_MARCH_X_WOM_ACCESS_SIZE_16BIT)
	/*16 bit access */
	
	/*Data Patterns used*/
	#define DATA_PATTERN_0s 0x0000U
	#define DATA_PATTERN_Fs 0xFFFFU
	#define DATA_PATTERN_As 0xAAAAU
	#define DATA_PATTERN_5s 0x5555U
	
	/*Number of bytes in WORD*/
	#define BYTES_PER_WORD 2U
	
	/*Used to check an address is 16 bit alligned */
	#define ALIGNMENT_BITS 0x00000001UL
	
	/*Set the variable type used for RAM access*/
	typedef uint16_t    TYPE_ACCESS_t;

#elif (RAMTEST_MARCH_X_WOM_ACCESS_SIZE == RAMTEST_MARCH_X_WOM_ACCESS_SIZE_32BIT)
	/*32 bit access */
	
	/*Data Patterns used*/
	#define DATA_PATTERN_0s 0x00000000UL
	#define DATA_PATTERN_Fs 0xFFFFFFFFUL
	#define DATA_PATTERN_As 0xAAAAAAAAUL
	#define DATA_PATTERN_5s 0x55555555UL
	
	/*Number of bytes in WORD*/
	#define BYTES_PER_WORD 4U
	
	/*Used to check an address is 32 bit alligned */
	#define ALIGNMENT_BITS 0x00000003UL
	
	/*Set the variable type used for RAM access*/
	typedef uint32_t    TYPE_ACCESS_t;
#endif

/*Sets the variable type used to hold the number of WORDS in the test are,
see header file for explanation.*/
#if (RAMTEST_MARCH_X_WOM_MAX_WORDS == RAMTEST_MARCH_X_WOM_MAX_WORDS_8BIT)
	typedef uint8_t    TYPE_WORDS_t;
#elif (RAMTEST_MARCH_X_WOM_MAX_WORDS == RAMTEST_MARCH_X_WOM_MAX_WORDS_16BIT)
	typedef uint16_t   TYPE_WORDS_t;
#elif (RAMTEST_MARCH_X_WOM_MAX_WORDS == RAMTEST_MARCH_X_WOM_MAX_WORDS_32BIT)
	typedef uint32_t   TYPE_WORDS_t;
#endif

/***********************************************************************************
Function Prototypes
***********************************************************************************/					
static bool_t Start_March_X_WOM(volatile TYPE_ACCESS_t* pui_RAM, 
									const TYPE_WORDS_t ui_NumBlocks);

static bool_t March_X_WOM_increment(volatile TYPE_ACCESS_t* pui_RAM,
										const TYPE_WORDS_t ui_NumBlocks,
										const TYPE_ACCESS_t ui_ValueRead,
					 					const TYPE_ACCESS_t ui_ValueWrite);
					
static bool_t March_X_WOM_decrement(volatile TYPE_ACCESS_t* pui_RAM,
						 				const TYPE_WORDS_t ui_NumBlocks,
										const TYPE_ACCESS_t ui_ValueRead,
										const TYPE_ACCESS_t ui_ValueWrite);
										
static void MemoryCopy(const volatile TYPE_ACCESS_t* const pui_Source,
						volatile TYPE_ACCESS_t* const pui_Dest,
						const TYPE_WORDS_t ui_NumBlocks);


/***********************************************************************************
User Program Code
***********************************************************************************/

/***********************************************************************************
Function Name : RamTest_March_X_WOM_Extra
Description   :	RAM memory test using March X algorithm for WOM.
				The test is non-destructive. This is achieved by copying the
				contents of the test are to a supplied buffer (pui_RAMSafe)
				before the test and then restoring the contents after the test.
				Before using the buffer (pui_RAMSafe) a destructive memory test
				is performed on it. If it fails the main test will not be performed
				and the function will return Test Failed.
				
Parameters    : ui32_StartAddr - Start address (Aligned with chosen access width).
				ui32_EndAddr   - End address (Aligned with chosen access width).
				pui_RAMSafe    - Set to the start of a buffer that is the same size
				 				 as the test RAM area. (Aligned with chosen access width)
				
				Note: StartAddr must be less than or equall to the end address.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST or parameter check Failed.
***********************************************************************************/
bool_t RamTest_March_X_WOM_Extra(const uint32_t ui32_StartAddr,
							 	const uint32_t ui32_EndAddr,
								void* const p_RAMSafe)
{
	bool_t b_Result;
	uint32_t ui32_NumBlocks;
	
	/*Convert to the selected access width*/
	TYPE_ACCESS_t* pui_RAMSafe = (TYPE_ACCESS_t*)p_RAMSafe;
	
	/*Perform quick check that pui_RAMSafe is not NULL as this is not allowed
	for this '_Extra' function but is for the non '_Extra' function.*/
	if(NULL == pui_RAMSafe)
	{
		b_Result = FALSE;
	}
	else
	{
		/*Calculate the number of words this covers*/
		ui32_NumBlocks = 1 + ((ui32_EndAddr - ui32_StartAddr) / BYTES_PER_WORD);
		
		/*First test the 'Safe' buffer. This is a destructive test.*/
		/*NOTE: This causes MISRA message 0306 -
		"The size of integer required to hold the result of a pointer cast is implementation-defined."
		This is recognised as unavoidable for certain embedded applications.*/
		b_Result = RamTest_March_X_WOM((uint32_t)&pui_RAMSafe[0],
										 (uint32_t)&pui_RAMSafe[ui32_NumBlocks-1], NULL);

		if(TRUE == b_Result)
		{
			/*Perform the main test.*/	
			b_Result = RamTest_March_X_WOM(ui32_StartAddr, ui32_EndAddr, pui_RAMSafe);
		}
	}
	
	return b_Result;
}
								
/***********************************************************************************
Function Name : RamTest_March_X_WOM
Description   :	RAM memory test using March X algorithm for WOM.
Parameters    : ui32_StartAddr - Start address (Aligned with chosen access width).
				ui32_EndAddr   - end address (Aligned with chosen access width)
				pui_RAMSafe    - For a destructive memory test set to NULL.
								For a non-destructive memory test,
								set to the start of a buffer
								that is large enough to copy the contents
								of the test RAM area into it.
								(Aligned with chosen access width)
				
				Note: StartAddr must be less than or equall to the end address.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST or parameter check Failed.
***********************************************************************************/
bool_t RamTest_March_X_WOM(const uint32_t ui32_StartAddr,
							 	const uint32_t ui32_EndAddr,
								void* const p_RAMSafe)
{	
	bool_t b_Result = TRUE;
	uint32_t ui32_NumBlocks = 0;
	volatile TYPE_ACCESS_t* pui_RAM;
	
	/*Convert to the selected access width*/
	TYPE_ACCESS_t* pui_RAMSafe = (TYPE_ACCESS_t*)p_RAMSafe;
	
	/*Check parameters*/
	{
		/*Check start address is alligned with chosen access width.*/
		if((ui32_StartAddr & ALIGNMENT_BITS) != 0)
		{
			b_Result = FALSE;
		}
		/*Check end address is alligned with chosen access width*/
		else if((ui32_EndAddr & ALIGNMENT_BITS) != 0)
		{
			b_Result = FALSE;
		}
		/*Check end address is greater or equall to start address*/
		else if(ui32_StartAddr > ui32_EndAddr)
		{
			b_Result = FALSE;	
		}
		/*Check buffer is alligned with chosen access width (if it is being used)*/
		/*NOTE: This causes MISRA message 0306 -
		"The size of integer required to hold the result of a pointer cast is implementation-defined."
		This is recognised as unavoidable for certain embedded applications.*/
		else if( (pui_RAMSafe != NULL) &&
				 (((uint32_t)pui_RAMSafe & ALIGNMENT_BITS) != 0) )
		{
			b_Result = FALSE;	
		}	
		else
		{
			/*Calculate number of words this covers*/
			ui32_NumBlocks = 1 + ((ui32_EndAddr - ui32_StartAddr) / BYTES_PER_WORD);
		
			/*Check that the number of words required fits into any user defined
			limits that may have been set.*/
			#if (RAMTEST_MARCH_X_WOM_MAX_WORDS == RAMTEST_MARCH_X_WOM_MAX_WORDS_8BIT)
				if(ui32_NumBlocks > 0xFFuL)
				{
					b_Result = FALSE;
				}
			#elif (RAMTEST_MARCH_X_WOM_MAX_WORDS == RAMTEST_MARCH_X_WOM_MAX_WORDS_16BIT)
				if(ui32_NumBlocks > 0xFFFFuL)
				{
					b_Result = FALSE;
				}
			#endif
			/*Parameters Ok*/
		}
	}
	
	/*Only perform the test if the parameter checking was OK */	
	if(TRUE == b_Result)
	{
		/*Point to start of RAM to test*/
		/*NOTE: This causes MISRA message 0303 -
		"Cast between a pointer to volatile object and an integral type"
		This is recognised as unavoidable for certain embedded applications.*/
		pui_RAM = (volatile TYPE_ACCESS_t*)ui32_StartAddr;
		
		/*Do we need to keep the current RAM contents?*/
		if(pui_RAMSafe != NULL)
		{
			/*Copy Test area contents to safe RAM*/
			/*NOTE: This causes MISRA message 0303 -
			"Cast between a pointer to volatile object and an integral type"
			This is recognised as unavoidable for certain embedded applications.*/
			MemoryCopy((const TYPE_ACCESS_t*)ui32_StartAddr, 
						(volatile TYPE_ACCESS_t*)pui_RAMSafe,
						 (TYPE_WORDS_t)ui32_NumBlocks);
		}
	
		/*Perform the actual test*/
		b_Result = Start_March_X_WOM(pui_RAM, (TYPE_WORDS_t)ui32_NumBlocks);
		
		/*Do we need to restore the original RAM contents?*/
		if(pui_RAMSafe != NULL)
		{
			/*Copy Test area contents to safe RAM*/
			/*NOTE: This causes MISRA message 0303 -
			"Cast between a pointer to volatile object and an integral type"
			This is recognised as unavoidable for certain embedded applications.*/
			MemoryCopy((const volatile TYPE_ACCESS_t*)pui_RAMSafe,
			 			(volatile TYPE_ACCESS_t*)ui32_StartAddr,
						 (TYPE_WORDS_t)ui32_NumBlocks);
		}
	}
	
	return b_Result;
}

/***********************************************************************************
Function Name : Start_March_X_WOM
Description   :	Perform the actual March X WOM test.
				The memory contents of the test area will be lost.
				 
Parameters    : pui_RAM - pointer to start of RAM to test.
				ui_NumBlocks - number of words to test
				
Return Value  : TRUE = TEST Passed. FALSE = TEST Failed.
***********************************************************************************/
static bool_t Start_March_X_WOM(volatile TYPE_ACCESS_t* pui_RAM, 
									const TYPE_WORDS_t ui_NumBlocks)
{
	bool_t b_Result = TRUE;
	TYPE_WORDS_t ui_BlockIndex;

	/* 1) Write all zeros */
	for(ui_BlockIndex = 0; ui_BlockIndex < ui_NumBlocks; ui_BlockIndex++)
	{
		pui_RAM[ui_BlockIndex] = DATA_PATTERN_0s;
	}
	/* 2) Starting at lowest address and incrementing up,
	check read h'0000 then write h'FFFF */
	if(FALSE == March_X_WOM_increment(pui_RAM, ui_NumBlocks,
											 DATA_PATTERN_0s, DATA_PATTERN_Fs))
	{
		b_Result = FALSE;
	}
	/* 3) Starting at highest address and decrementing down,
	check read h'FFFF then write h'0000 */
	else if(FALSE == March_X_WOM_decrement(pui_RAM, ui_NumBlocks,
												 DATA_PATTERN_Fs, DATA_PATTERN_0s))
	{
		b_Result = FALSE;
	}
	/* 4) Starting at lowest address and incrementing up,
	check read h'0000 then write h'AAAA*/
	else if(FALSE == March_X_WOM_increment(pui_RAM, ui_NumBlocks,
												  DATA_PATTERN_0s, DATA_PATTERN_As))
	{
		b_Result = FALSE;
	}
	/* 5) Starting at highest address and decrementing down,
	check read h'AAAA then write h'5555*/
	else if(FALSE == March_X_WOM_decrement(pui_RAM, ui_NumBlocks,
												 DATA_PATTERN_As, DATA_PATTERN_5s))
	{
		b_Result = FALSE;
	}
	else
	{
		/*6) Check read all h'5555*/
		for(ui_BlockIndex = 0; ui_BlockIndex < ui_NumBlocks; ui_BlockIndex++)
		{
			if(pui_RAM[ui_BlockIndex] != DATA_PATTERN_5s)
			{
				b_Result = FALSE;
				break;
			}
		}
	}
	
	return b_Result;
}

/***********************************************************************************
Function Name : March_X_WOM_increment
Description   :	Starting at lowest address and incrementing up, a word at a time,
				check Value equalls ValueRead,
				then write ValueWrite to it.
				 
Parameters    : pui_RAM - pointer to start of RAM to test.
				ui_NumBlocks - number of words to test.
				ui_ValueRead - expected Value to read.
				ui_ValueWrite - value to write.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST Failed.
***********************************************************************************/
static bool_t March_X_WOM_increment(volatile TYPE_ACCESS_t* pui_RAM, 
										const TYPE_WORDS_t ui_NumBlocks,
										const TYPE_ACCESS_t ui_ValueRead,
										const TYPE_ACCESS_t ui_ValueWrite)
{	
	bool_t b_Result = TRUE;
	TYPE_ACCESS_t ui_Value;
	TYPE_WORDS_t ui_BlockIndex;
	
	/*For each word*/	
	for(ui_BlockIndex = 0; ui_BlockIndex < ui_NumBlocks; ui_BlockIndex++)
	{
		/*** Read RAM ***/
		ui_Value = pui_RAM[ui_BlockIndex];
		
		/*Check value is valueRead*/
		if(ui_ValueRead != ui_Value)
		{
			/*Value is not as expected*/
			b_Result = FALSE;
		}
		
		/*** Write RAM ****/
		pui_RAM[ui_BlockIndex] = ui_ValueWrite;
		
		/*If test has already failed then don't continue*/
		if(FALSE == b_Result)
		{
			break;
		}		
	}
	
	return b_Result;
}

/***********************************************************************************
Function Name : March_X_WOM_decrement
Description   :	Starting at highest address and decrementing down, a word at a time,
				check Value equalls ValueRead
				then write ValueWrite to it.
				 
Parameters    : pui_RAM - pointer to start(lowest address) of RAM to test.
				ui_NumBlocks - number of words to test
				ui_ValueRead - expected Value to read.
				ui_ValueWrite - value to write.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST Failed.
***********************************************************************************/
static bool_t March_X_WOM_decrement(volatile TYPE_ACCESS_t* pui_RAM,
										const TYPE_WORDS_t ui_NumBlocks,
										const TYPE_ACCESS_t ui_ValueRead,
										const TYPE_ACCESS_t ui_ValueWrite)
{
	bool_t b_Result = TRUE;
	TYPE_ACCESS_t ui_Value;
	TYPE_WORDS_t ui_BlockIndex;	
	TYPE_WORDS_t ui_BlockLoop;
	
	/*For each word*/	
	for(ui_BlockLoop = ui_NumBlocks; ui_BlockLoop != 0; ui_BlockLoop--)
	{
		ui_BlockIndex = ui_BlockLoop-1;
		
		/*** Read RAM ***/
		ui_Value = pui_RAM[ui_BlockIndex];
		
		/*Check value is valueRead*/
		if(ui_ValueRead != ui_Value)
		{
			/*Value is not as expected*/
			b_Result = FALSE;
		}
		
		/*** Write RAM ****/
		pui_RAM[ui_BlockIndex] = ui_ValueWrite;
		
		/*If test has already failed then don't continue*/
		if(FALSE == b_Result)
		{
			break;
		}		
	}
	
	return b_Result;
}

/***********************************************************************************
Function Name : MemoryCopy
Description   :	Copy memory contents from a specified source to a
				specified destination.
				 
Parameters    : pui_Source - pointer to start of source.
				pui_Dest - pointer to start of destination.
				ui_NumBlocks - Number of words to copy.
				
Return Value  : void
***********************************************************************************/
static void MemoryCopy(const volatile TYPE_ACCESS_t* const pui_Source,
						volatile TYPE_ACCESS_t* const pui_Dest,
						const TYPE_WORDS_t ui_NumBlocks)
{
	TYPE_WORDS_t ui_BlockIndex;
	
	for(ui_BlockIndex = 0; ui_BlockIndex < ui_NumBlocks; ui_BlockIndex++)
	{
		pui_Dest[ui_BlockIndex] = pui_Source[ui_BlockIndex];
	}
}
