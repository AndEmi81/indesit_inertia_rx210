/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name : ramtest_stack.c
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* Description : Self Test code.
*               RAM memory test of stack.
*				The actual memory test is provided by a different module.
*				
*				These functions must only be called in supervisor mode as they use the
*				MVTC instruction which is ignored in user mode.
*				
*				NOTE: These functions are not re-entrant.
*				Only 1 of these functions must be called at any one time.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
03-Feb-2011 VDE (Version 1.0)
09-May-2012 RTE-PJI Ported to GCC (Version 1.2)
***********************************************************************************/
						
/***********************************************************************************
Includes
***********************************************************************************/
#include "ramtest_stack.h"

/**********************************************************************************
 #defines
***********************************************************************************/

/***********************************************************************************
Function Prototypes
***********************************************************************************/					
static void StartUserTestAssembly(void);

static void StartIntTestAssembly(void);

static void StartBothTestAssembly(void);

void StartTest(void);

/***********************************************************************************
User Program Code
***********************************************************************************/
/*Global variables used while stack in not use-able*/
static volatile uint32_t g_ui32_StartAddr;
static volatile uint32_t g_ui32_EndAddr;
static volatile void* g_pRAMSafe;
static volatile TEST_FUNC g_fpTestFunc;

/*Variables referenced from assembly can not be static.*/
volatile uint32_t g_ui32_USP_Old;
volatile uint32_t g_ui32_USP_New;

/*Variables referenced from assembly can not be static.*/
volatile uint32_t g_ui32_ISP_Old;
volatile uint32_t g_ui32_ISP_New;

static volatile bool_t g_bResult;

/***********************************************************************************
Function Name : RamTest_Stack_User
Description   :	Tests an area of memory that includes the User Stack.
				
				Re-locate the user stack to the supplied new
				stack area to allow the test to be run.
				
				The test performed is a non-destructive test.
				However, the contents of the safe buffer (p_RAMSafe)
				will be destroyed as will the new stack area.
				
				The test RAM area must not include the interrupt stack (ISP)
				unless this is called from a non-interrupt and interrupts are disabled.
				
				After the test has completed then the USP and any registers
				used will be restored to their orginal values.
				
				
				This functions must only be called in supervisor mode as it uses the
				MVTC instruction which is ignored in User mode.
				
Parameters    : ui32_StartAddr 	- Start address of test area.(*)
				ui32_EndAddr   	- End address of test area.(*)
				p_RAMSafe 		- Set to the start of a buffer that is the same
				 				size as the test RAM area.(*)
				ui32_NewUSP    	- SP value for new user stack.
								Note: This is the highest address in new stack area.
								If calling from an interrupt this is not required
								and can be NULL.
				fpTest_Func		- Function pointer to the actual memory test to be used.
				
				NOTE: (*) These parameters are passed to the selected actual memory test
				(fpTest_Func) function. It is the specification of the parameter
				in the specified memory test function that must be adhered to.
				For example a certain test may require a buffer of a particular bit alignment.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST or parameter check Failed.
***********************************************************************************/
bool_t RamTest_Stack_User(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewUSP,
						const TEST_FUNC fpTest_Func)
{
	/*Store arguments in global variables so that they
	will be available without access to stack.*/
	g_ui32_StartAddr = ui32_StartAddr;
	g_ui32_EndAddr = ui32_EndAddr;
	g_pRAMSafe = p_RAMSafe;
	g_ui32_USP_New = ui32_NewUSP;
	g_fpTestFunc = fpTest_Func;
		
	/*Perform Test*/
	StartUserTestAssembly();
	
	return g_bResult;
}



/***********************************************************************************
Function Name : RamTest_Stack_Int
Description   :	Tests an area of memory that includes the Interrupt Stack.
				
				This will re-locate the interrupt stack to the supplied new
				interrupt stack area to allow the test to be run.
				This means this test can either be called from an interrupt routine
				itself or if called from the main programme loop and interrupts
				can be left enabled.
				
				The test performed is a non-destructive test.
				However, the contents of the safe buffer (p_RAMSafe)
				will be destroyed as will the new stack area.
				
				The test RAM area must not include the user stack (USP)
				unless this is called from an interrupt.
				
				After the test has completed then the ISP and any registers
				used will be restored to their orginal values.
				
				This functions must only be called in supervisor mode as it uses the
				MVTC instruction which is ignored in User mode.
							
Parameters    : ui32_StartAddr 	- Start address of test area.(*)
				ui32_EndAddr   	- End address of test area.(*)
				p_RAMSafe 		- Set to the start of a buffer that is the same
				 				size as the test RAM area.(*)
				ui32_NewISP    - SP value for new interrupt stack.
								Note: This is the highest address in new stack area.
				fpTest_Func		- Function pointer to the actual memory test to be used.
				
				NOTE: (*) These parameters are passed to the selected actual memory test
				(fpTest_Func) function. It is the specification of the parameter
				in the specified memory test function that must be adhered to.
				For example a certain test may require a buffer of a particular bit alignment.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST or parameter check Failed.
***********************************************************************************/
bool_t RamTest_Stack_Int(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewISP,
						const TEST_FUNC fpTest_Func)
{
	/*Store arguments in global variables so that they
	will be available without access to stack.*/
	g_ui32_StartAddr = ui32_StartAddr;
	g_ui32_EndAddr = ui32_EndAddr;
	g_pRAMSafe = p_RAMSafe;
	g_ui32_ISP_New = ui32_NewISP;
	g_fpTestFunc = fpTest_Func;
		
	/*Perform Test*/
	StartIntTestAssembly();
	
	return g_bResult;	
}

/***********************************************************************************
Function Name : RamTest_Stacks
Description   :	Tests an area of memory that includes both the Interrupt Stack
				and the User Stack.
				
				This will re-locate the interrupt stack to the supplied new
				ISP stack area to allow the test to be run without the need to
				disable interrupts. (or to allow this to be called from an interrupt)
				
				This will re-locate the user stack to the supplied new
				USP stack area to allow the test to be run.
				
				The test performed is a non-destructive test.
				However, the contents of the safe buffer (p_RAMSafe)
				will be destroyed as will the new stack areas.
				
				After the test has completed then the SPs and any registers
				used will be restored to their orginal values.
				
				This functions must only be called in supervisor mode as it uses the
				MVTC instruction which is ignored in User mode.
				
Parameters    : ui32_StartAddr 	- Start address of test area.(*)
				ui32_EndAddr   	- End address of test area.(*)
				p_RAMSafe 		- Set to the start of a buffer that is the same
				 				size as the test RAM area.(*)
				ui32_NewIntSP    - SP value for new interrupt stack (ISP).
								Note: This is the highest address in new stack area.
				ui32_NewUserSP    - SP value for new user stack (USP).
								If calling from an interrupt this is not required
								and can be NULL.
								Note: This is the highest address in new stack area.
				fpTest_Func		- Function pointer to the actual memory test to be used.
				
				NOTE: (*) These parameters are passed to the selected actual memory test
				(fpTest_Func) function. It is the specification of the parameter
				in the specified memory test function that must be adhered to.
				For example a certain test may require a buffer of a particular bit alignment.
				
Return Value  : TRUE = TEST Passed. FALSE = TEST or parameter check Failed.
***********************************************************************************/
bool_t RamTest_Stacks(const uint32_t ui32_StartAddr,
						const uint32_t ui32_EndAddr,
						void* const p_RAMSafe,
						const uint32_t ui32_NewISP,
						const uint32_t ui32_NewUSP,
						const TEST_FUNC fpTest_Func)
{
		/*Store arguments in global variables so that they
	will be available without access to stack.*/
	g_ui32_StartAddr = ui32_StartAddr;
	g_ui32_EndAddr = ui32_EndAddr;
	g_pRAMSafe = p_RAMSafe;
	g_ui32_ISP_New = ui32_NewISP;
	g_ui32_USP_New = ui32_NewUSP;
	g_fpTestFunc = fpTest_Func;
		
	/*Perform Test*/
	StartBothTestAssembly();
	
	return g_bResult;
}

/***********************************************************************************
Function Name : StartUserTestAssembly
Description   :	Assembly function that will:
				1. Relocate User stack.
				2. Run memory test.
				3. Move stack back to original position.
				
Parameters    : none	
Return Value  : none
***********************************************************************************/
static void StartUserTestAssembly(void)
{
	/*Set R1 with address of global variable g_ui32_USP_Old */
	__asm__ ("MOV.L #_g_ui32_USP_Old, R1");

	/*Set R2 with address of global variable g_ui32_USP_New */
	__asm__ ("MOV.L #_g_ui32_USP_New, R2");
	
	/*Preserve current SP value by storing in global variable g_ui32_USP_Old, via R3*/
	__asm__ ("MVFC USP, R3");
	__asm__ ("MOV.L R3, [R1]");

	/*Relocate stack to g_ui32_USP_New via R3*/
	__asm__ ("MOV.L [R2], R3");
	__asm__ ("MVTC R3, USP");

	/*Use C function with no parameters or return value to actually perform the memory test. */
	__asm__ ("MOV.L #_StartTest, R3");
	__asm__ ("JSR R3");
	__asm__ ("NOP");

	/*Restore SP*/
	/*Set R1 with address of global variable g_ui32_USP_Old */
	__asm__ ("MOV.L #_g_ui32_USP_Old, R1");
	/*Set USP with value stored in g_ui32_USP_Old via R3*/
	__asm__ ("MOV.L [R1], R3");
	__asm__ ("MVTC R3, USP");
}


/***********************************************************************************
Function Name : StartIntTestAssembly
Description   :	Assembly function that will:
				1. Relocate Interrupt stack.
				2. Run memory test.
				3. Move stack back to original poistion.
				
Parameters    : none	
Return Value  : none
***********************************************************************************/
static void StartIntTestAssembly(void)
{	
	/*Set R1 with address of global variable g_ui32_ISP_Old */
	__asm__ ("MOV.L #_g_ui32_ISP_Old, R1");

	/*Set R2 with address of global variable g_ui32_ISP_New */
	__asm__ ("MOV.L #_g_ui32_ISP_New, R2");
	
	/*Preserve current SP value by storing in global variable g_ui32_ISP_Old, via R3*/
	__asm__ ("MVFC ISP, R3");
	__asm__ ("MOV.L R3, [R1]");

	/*Relocate stack to g_ui32_ISP_New via R3*/
	__asm__ ("MOV.L [R2], R3");
	__asm__ ("MVTC R3, ISP");

	/*Use C function with no parameters or return value to actually perform the memory test. */
	__asm__ ("MOV.L #_StartTest, R3");
	__asm__ ("JSR R3");
	__asm__ ("NOP");

	/*Restore SP*/
	/*Set R1 with address of global variable g_ui32_ISP_Old */
	__asm__ ("MOV.L #_g_ui32_ISP_Old, R1");
	/*Set ISP with value stored in g_ui32_ISP_Old via R3*/
	__asm__ ("MOV.L [R1], R3");
	__asm__ ("MVTC R3, ISP");
}


/***********************************************************************************
Function Name : StartBothTestAssembly
Description   :	Assembly function that will:
				1. Relocate User and Interrupt stack.
				2. Run memory test.
				3. Move stacks back to original poistions.
				
Parameters    : none	
Return Value  : none
***********************************************************************************/
static void StartBothTestAssembly(void)
{
	/*** RE-LOCATE USP ***/
		/*Set R1 with address of global variable g_ui32_USP_Old */
		__asm__ ("MOV.L #_g_ui32_USP_Old, R1");

		/*Set R2 with address of global variable g_ui32_USP_New */
		__asm__ ("MOV.L #_g_ui32_USP_New, R2");
	
		/*Preserve current SP value by storing in global variable g_ui32_USP_Old, via R3*/
		__asm__ ("MVFC USP, R3");
		__asm__ ("MOV.L R3, [R1]");

		/*Relocate USP to g_ui32_USP_New via R3*/
		__asm__ ("MOV.L [R2], R3");
		__asm__ ("MVTC R3, USP");
	
	/*** RE-LOCATE ISP ***/	
		/*Set R1 with address of global variable g_ui32_ISP_Old */
		__asm__ ("MOV.L #_g_ui32_ISP_Old, R1");

		/*Set R2 with address of global variable g_ui32_ISP_New */
		__asm__ ("MOV.L #_g_ui32_ISP_New, R2");
	
		/*Preserve current SP value by storing in global variable g_ui32_ISP_Old, via R3*/
		__asm__ ("MVFC ISP, R3");
		__asm__ ("MOV.L R3, [R1]");

		/*Relocate stack to g_ui32_ISP_New via R3*/
		__asm__ ("MOV.L [R2], R3");
		__asm__ ("MVTC R3, ISP");

	/*Use C function with no parameters or return value to actually perform the memory test. */
	__asm__ ("MOV.L #_StartTest, R3");
	__asm__ ("JSR R3");
	__asm__ ("NOP");

	/*Restore USP*/
	/*Set R1 with address of global variable g_ui32_USP_Old */
	__asm__ ("MOV.L #_g_ui32_USP_Old, R1");
	/*Set ISP with value stored in g_ui32_ISP_Old via R3*/
	__asm__ ("MOV.L [R1], R3");
	__asm__ ("MVTC R3, USP");
	
	/*Restore ISP*/
	/*Set R1 with address of global variable g_ui32_ISP_Old */
	__asm__ ("MOV.L #_g_ui32_ISP_Old, R1");
	/*Set ISP with value stored in g_ui32_ISP_Old via R3*/
	__asm__ ("MOV.L [R1], R3");
	__asm__ ("MVTC R3, ISP");
}

/***********************************************************************************
Function Name : StartTest
Description   :	Calls the actual memory test routine.
				
				Variables g_ui32_StartAddr, g_ui32_EndAddr, g_pRAMSafe
				and g_fpTestFunc must be set before calling this.
				
				Variable g_bResult will be set with the result of the test.
				
Parameters    : 
				
Return Value  : 
***********************************************************************************/
void StartTest(void)
{
	g_bResult = g_fpTestFunc(g_ui32_StartAddr, g_ui32_EndAddr, (void*)g_pRAMSafe);								 
}
