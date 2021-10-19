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
* File Name : cpu_test.c
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* H/W Platform : 
* Description : Self Test code.
*               CPU Tests.
*				This file doesn't include the implementation of the
*				CPU General registers coupling tests
*				 - see cpu_test_coupling.c
*				 
*				 This file should be built with optimisation off.
**********************************************************************/


/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
03-Feb-2011 VDE (Version 1.0) (RX62T)
02-SEP-2011 RTE-PJI Adapted for RX210. RX210 doesn't have a FPSW register.
15-Nov-2011 VDE (Version 1.1)
09-May-2012 RTE-PJI Ported to GCC (Version 1.2)
***********************************************************************************/

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"
#include "cpu_test.h"

/**********************************************************************************
#defines
***********************************************************************************/
/*Comment this out if you want to use the Non-Coupling General Purpose
registers tests in the CPU_Test_ALL function.*/
/* #define USE_TestGPRsCoupling */

/*The RX Group share the same CPU core but with a few differences that
are catered for by these #defines:*/
/*Define RX610 if using with an RX610*/
/*#define RX610*/
/*Define RX210 if using with an RX210*/
#define RX210

/***********************************************************************************
Function Prototypes
***********************************************************************************/					

/*C Function called from CPU_Test_PC*/
static uint32_t TestPC_TestFunction(uint32_t ui32Value) __attribute__ ((section (".PCTEST_TESTFUNCTION")));
void ForImport_CPU_Test(void);

/***********************************************************************************
User Program Code
***********************************************************************************/

/***********************************************************************************
Function Name : CPU_Test_All
Description   :	Performs all of the CPU tests.

				Use #define USE_TestGPRsCoupling to select which General Purpose
				Register test to perform.
				
				NOTE: The processor mode must be supevisor mode.
				In user mode this test will fail as some of the control registers
				can not be written to in user mode.
				If calling this test from within an interrupt handler then the
				processor mode will automatically be supervisor mode.

				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.

				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				See individual tests functions for full description.
				
				
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_All(void)
{
	#ifdef USE_TestGPRsCoupling
		/*General Purpose Registers Coupling tests. (R0 to R15) */
		/*NOTE THese functions are implemented in file CPU_Test_Coupling.c*/
		CPU_Test_GPRsCouplingPartA();
		CPU_Test_GPRsCouplingPartB();
	#else
		/*Test General Purpose Registers that don't need preserving first*/
		CPU_Test_GeneralA();
		/*Test General Purpose Registers that require preserving */
		CPU_Test_GeneralB();
	#endif
	
	/*Control Registers*/
	CPU_Test_Control();
	
	/*Accumulator Register*/
	CPU_Test_Accumulator();
	
	/*Program Counter*/
	/* This test has been disabled because already done in PowerON_Reset */
	/* CPU_Test_PC(); */
}

/***********************************************************************************
Function Name : CPU_Test_GeneralA
Description   :	Test general purpose registers R1,R2,R3,R4,R5,R14 and R15.
				These are the general registers that don't need to be Preserved - see below.
				
				Registers are tested in pairs.
				For each pair of registers:-
				1. Write h'55555555 to both.
				2. Read both and check they are equal.
				3. Write h'AAAAAAAA to both.
				4. Read both and check they are equal.
				
				Note:
				The rules for register preservation state that
				registers R1,R2,R3,R4,R5,R14 and R15 are free for a function to use
				without preserving their values.
				Hence this function does not Preserve the values of
				these registers.
				
				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_GeneralA(void)
{
	/*** Test R1 and R2 ****/
	__asm__ ("MOV.L #055555555h, R1");
	__asm__ ("MOV.L #055555555h, R2");
	__asm__ ("CMP R1,R2");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralA");

	__asm__ ("MOV.L #0AAAAAAAAh, R1");
	__asm__ ("MOV.L #0AAAAAAAAh, R2");
	__asm__ ("CMP R1,R2");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralA");
	
	/*** Test R3 and R4 ****/
	__asm__ ("MOV.L #055555555h, R3");
	__asm__ ("MOV.L #055555555h, R4");
	__asm__ ("CMP R3,R4");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralA");

	__asm__ ("MOV.L #0AAAAAAAAh, R3");
	__asm__ ("MOV.L #0AAAAAAAAh, R4");
	__asm__ ("CMP R3,R4");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralA");
	
	/*** Test R5 and R14 ****/
	__asm__ ("MOV.L #055555555h, R5");
	__asm__ ("MOV.L #055555555h, R14");
	__asm__ ("CMP R5,R14");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralA");

	__asm__ ("MOV.L #0AAAAAAAAh, R5");
	__asm__ ("MOV.L #0AAAAAAAAh, R14");
	__asm__ ("CMP R5,R14");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralA");
	
	/*** Test R15, use already tested R1 ****/
	__asm__ ("MOV.L #055555555h, R15");
	__asm__ ("MOV.L #055555555h, R1");
	__asm__ ("CMP R15,R1");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralA");

	__asm__ ("MOV.L #0AAAAAAAAh, R15");
	__asm__ ("MOV.L #0AAAAAAAAh, R1");
	__asm__ ("CMP R15,R1");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralA");
	
	/*All Tests Passed - so skip the call to the error handler.*/
	__asm__ ("BRA LABEL_PASSED_Test_GeneralA");
	
	__asm__ ("LABEL_FAILED_Test_GeneralA:");
	/*Test failed - branch to the Error handler subroutine*/
	__asm__ ("BSR _CPU_Test_ErrorHandler");

	__asm__ ("LABEL_PASSED_Test_GeneralA:");
	/*Passed - return from this function.*/
}

/***********************************************************************************
Function Name : CPU_Test_GeneralB
Description   :	Test general purpose registers R0,R6,R7,R8,R9,R10,R11,R12 and R13.
				These are the general registers that need to be Preserved - see below.
				
				Registers are tested in pairs.
				For each pair of registers:-
				1. Write h'55555555 to both.
				2. Read both and check they are equal.
				3. Write h'AAAAAAAA to both.
				4. Read both and check they are equal.
				
				Note:
				The rules for register preservation state that
				registers R0,R6,R7,R8,R9,R10,R11,R12 and R13 must be Preserved by a function.
				Hence this function does Preserve the values of these registers.
				
				This test assumes registers R1 to R5, R14and R15 are working.
				
				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_GeneralB(void)
{
	/*Preseve registers to be tested by saving to other registers*/
	__asm__ ("MOV.L R0,R1");
	__asm__ ("MOV.L R6,R2");
	__asm__ ("MOV.L R7,R3");
	__asm__ ("MOV.L R8,R4");
	__asm__ ("MOV.L R9,R5");
	__asm__ ("MOV.L R10,R14");
	__asm__ ("MOV.L R11,R15");
	/*No more spare registers to Preserve R12 and R13 at this point - done later*/
	
	/*** Test R0 and R6 ****/
	__asm__ ("MOV.L #055555555h, R0");
	__asm__ ("MOV.L #055555555h, R6");
	__asm__ ("CMP R0,R6");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralB");

	__asm__ ("MOV.L #0AAAAAAAAh, R0");
	__asm__ ("MOV.L #0AAAAAAAAh, R6");
	__asm__ ("CMP R0,R6");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralB");
	
	/*Restore R0 and R6 now so that R12 and R13 can be saved*/
	__asm__ ("MOV.L R1,R0");
	__asm__ ("MOV.L R2,R6");
	/*Save R12 and R13 so they can be restored later*/
	__asm__ ("MOV.L R12,R1");
	__asm__ ("MOV.L R13,R2");
	
	/*** Test R7 and R8 ****/
	__asm__ ("MOV.L #055555555h, R7");
	__asm__ ("MOV.L #055555555h, R8");
	__asm__ ("CMP R7,R8");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralB");

	__asm__ ("MOV.L #0AAAAAAAAh, R7");
	__asm__ ("MOV.L #0AAAAAAAAh, R8");
	__asm__ ("CMP R7,R8");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralB");
	
	/*** Test R9 and R10 ****/
	__asm__ ("MOV.L #055555555h, R9");
	__asm__ ("MOV.L #055555555h, R10");
	__asm__ ("CMP R9,R10");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralB");

	__asm__ ("MOV.L #0AAAAAAAAh, R9");
	__asm__ ("MOV.L #0AAAAAAAAh, R10");
	__asm__ ("CMP R9,R10");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralB");
	
	/*** Test R11 and R12 ****/
	__asm__ ("MOV.L #055555555h, R11");
	__asm__ ("MOV.L #055555555h, R12");
	__asm__ ("CMP R11,R12");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralB");

	__asm__ ("MOV.L #0AAAAAAAAh, R11");
	__asm__ ("MOV.L #0AAAAAAAAh, R12");
	__asm__ ("CMP R11,R12");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralB");
	
	/*** Test R13, use R12 ****/
	__asm__ ("MOV.L #055555555h, R13");
	__asm__ ("MOV.L #055555555h, R12");
	__asm__ ("CMP R13,R12");
	__asm__ ("BNE LABEL_FAILED_Test_GeneralB");

	__asm__ ("MOV.L #0AAAAAAAAh, R13");
	__asm__ ("MOV.L #0AAAAAAAAh, R12");
	__asm__ ("CMP R13,R12");
	__asm__ ("BNE  LABEL_FAILED_Test_GeneralB");
	
	/*Restore original register values.
	(NOTE: R0 and R6 have already been restored)*/
	__asm__ ("MOV.L R3,R7");
	__asm__ ("MOV.L R4,R8");
	__asm__ ("MOV.L R5,R9");
	__asm__ ("MOV.L R14,R10");
	__asm__ ("MOV.L R15,R11");
	__asm__ ("MOV.L R1,R12");
	__asm__ ("MOV.L R2,R13");
	
	/*All Tests Passed - so skip the call to the error handler.*/
	__asm__ ("BRA LABEL_PASSED_Test_GeneralB");
	
	__asm__ ("LABEL_FAILED_Test_GeneralB:");
	/*Test failed - branch to the Error handler subroutine*/
	__asm__ ("BSR _CPU_Test_ErrorHandler");

	__asm__ ("LABEL_PASSED_Test_GeneralB:");
	/*Passed - return from this function.*/		
}

/***********************************************************************************
Function Name : CPU_Test_Control
Description   :	Test control registers:
				
				ISP,USP,INTB,PSW,BPC,BPSW,FINTV and FPSW.
				
				NOTE: RX210 does not have a FPSW register.
				FPSW is not tested if RX210 is #defined.
				
				The PC is not tested in this function.
				Use function CPU_Test_PC.
				
				NOTE: The processor mode must be supevisor mode.
				In user mode this test will fail as some of the registers
				can not be written to in user mode.

				Generally the test procedure for each register is as follows:
				For each register:-
				1. Write h'55555555 to.
				2. Read back and check value equals h'55555555.
				3. Write h'AAAAAAAA to.
				4. Read back and check value equals h'AAAAAAAA.
				
				Note however that there are some cases where restrictions on
				certain bits within a register mean this can not be can followed exactly.
				Details will be given where this occurs.
				
				This test assumes registers R1 to R5 are working.
				
				This function follows the rules for register preservation.
				
				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_Control(void)
{
	/*Values to write to test registers*/
	__asm__ ("MOV.L #055555555h,R1"); /*Test Value 1*/
	__asm__ ("MOV.L #0AAAAAAAAh,R2"); /*Test Value 2*/

	/* USP and ISP test are disabled at this point
	 * They're already done in PowerON_Reset */
#if 0
	/*** Start Test ISP ***/
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC ISP, R5");
		
		__asm__ ("MVTC R1, ISP"); 	/*Write test value 1*/
		__asm__ ("MVFC ISP, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, ISP"); 	/*Write test value 2*/
		__asm__ ("MVFC ISP, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, ISP;");
	/*** End Test ISP ***/
	
	/*** Start Test USP ***/
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC USP, R5");
		
		__asm__ ("MVTC R1, USP"); 	/*Write test value 1*/
		__asm__ ("MVFC USP, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, USP"); 	/*Write test value 2*/
		__asm__ ("MVFC USP, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, USP");
	/*** End Test USP ***/
#endif

	/*** Start Test INTB ***/
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC INTB, R5");
		
		__asm__ ("MVTC R1, INTB"); 	/*Write test value 1*/
		__asm__ ("MVFC INTB, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, INTB"); 	/*Write test value 2*/
		__asm__ ("MVFC INTB, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, INTB");
	/*** End Test INTB ***/
	
	/*** Start Test BPC ***/
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC BPC, R5");
		
		__asm__ ("MVTC R1, BPC"); 	/*Write test value 1*/
		__asm__ ("MVFC BPC, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, BPC"); 	/*Write test value 2*/
		__asm__ ("MVFC BPC, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, BPC");
	/*** End Test BPC ***/
	
	/*** Start Test FINTV ***/
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC FINTV, R5");
		
		__asm__ ("MVTC R1, FINTV"); /*Write test value 1*/
		__asm__ ("MVFC FINTV, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, FINTV"); /*Write test value 2*/
		__asm__ ("MVFC FINTV, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, FINTV");
	/*** End Test FINTV ***/

	
	/*** Start Test PSW ***/
		/*Values to write to test registers.*/
		/*Can't be h'55555555 and h'AAAAAAAA as not all bits are r/w.*/
		/*The I bit is not tested to avoid potentially enabling interrupts*/
		
		/*The Rx610 requires different test data patterns as it has only 3 IPL bits,*/
		/*rather than the 4 bits that other Rx devices have.*/
		#ifdef RX610
			__asm__ ("MOV.L #005000005h,R1"); /*Test Value 1, as close to h'55555555 as possible.*/
			__asm__ ("MOV.L #00202000Ah,R2"); /*Test Value 2, as close to h'AAAAAAAA as possible.*/
		#else
			__asm__ ("MOV.L #005000005h,R1"); /*Test Value 1, as close to h'55555555 as possible.*/
			__asm__ ("MOV.L #00A02000Ah,R2"); /*Test Value 2, as close to h'AAAAAAAA as possible.*/
		#endif
		
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC PSW, R5");
		
		__asm__ ("MVTC R1, PSW"); 	/*Write test value 1*/
		__asm__ ("MVFC PSW, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, PSW"); 	/*Write test value 2*/
		__asm__ ("MVFC PSW, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, PSW");
	/*** End Test PSW ***/
	
	/*** Start Test BPSW ***/
		/*Values to write to test registers.*/
		/*Can't be h'55555555 and h'AAAAAAAA as not all bits are r/w.*/
		__asm__ ("MOV.L #005010005h,R1"); /*Test Value 1, as close to h'55555555 as possible.*/
		__asm__ ("MOV.L #00202000Ah,R2"); /*Test Value 2, as close to h'AAAAAAAA as possible.*/
		
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC BPSW, R5");
		
		__asm__ ("MVTC R1, BPSW"); 	/*Write test value 1*/
		__asm__ ("MVFC BPSW, R3");	/*Read value back*/
		__asm__ ("CMP R1,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, BPSW"); 	/*Write test value 2*/
		__asm__ ("MVFC BPSW, R3");	/*Read value back*/
		__asm__ ("CMP R2,R3");		/*Compare with value written*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, BPSW");
	/*** End Test BPSW ***/

/*The RX210 doesn't have a FPSW register*/	
#ifndef RX210
	/*** Start Test FPSW ***/
		/*Values to write to test registers.*/
		/*Can't be h'55555555 and h'AAAAAAAA as not all bits are r/w.*/
		__asm__ ("MOV.L #0540055FDh,R1"); /*Test Value 1, as close to h'55555555 as possible.*/
		__asm__ ("MOV.L #0A80028FEh,R2"); /*Test Value 2, as close to h'AAAAAAAA as possible.*/
		/*Also when reading this register must mask some bits before comparing with
		the value written as we don't know the value the bits will read as.*/
		__asm__ ("MOV.L #07FFFFF03h,R4"); /*Mask value (0 bits will not be ignored)*/
		/*Set R14 with the value we expect to read after writing R1*/
		__asm__ ("AND R1,R4,R14");
		/*Set R15 with the value we expect to read after writing R2*/
		__asm__ ("AND R2,R4,R15");
		
		/*Preseve register to be tested by saving to another register*/
		__asm__ ("MVFC FPSW, R5");
		
		__asm__ ("MVTC R1, FPSW"); 	/*Write test value 1*/
		__asm__ ("MVFC FPSW, R3	"); /*Read value back*/
		__asm__ ("AND R4, R3");		/*Mask value read*/
		__asm__ ("CMP R14,R3");		/*Compare with value expected*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
	
		__asm__ ("MVTC R2, FPSW"); 	/*Write test value 2*/
		__asm__ ("MVFC FPSW, R3");	/*Read value back*/
		__asm__ ("AND R4, R3");		/*Mask value read*/
		__asm__ ("CMP R15,R3");		/*Compare with value expected*/
		__asm__ ("BNE LABEL_FAILED_Test_Control"); /*If not equall then branch to Failed label.*/
		
		/*Restore original value*/
		__asm__ ("MVTC R5, FPSW");
	/*** End Test FPSW ***/
#endif /*NOT RX210*/
	
	/*All Tests Passed - so skip the call to the error handler.*/
	__asm__ ("BRA LABEL_PASSED_Test_Control");
	
	__asm__ ("LABEL_FAILED_Test_Control:");
	/*Test failed - branch to the Error handler subroutine*/
	__asm__ ("BSR _CPU_Test_ErrorHandler");

	__asm__ ("LABEL_PASSED_Test_Control:");
	/*Passed - return from this function.*/
	
}

/***********************************************************************************
Function Name : CPU_Test_Accumulator
Description   :	Test ACC register:

				NOTE1: Bits 0-15 can not be read and are therefore not tested.
							
				The test procedure is as follows:
				1. Write h'55555555 to high order 32 bits.
				2. Write h'55555555 to low order 32 bits.
				3. Read back high order and check value equals h'55555555.
				4. Read back middle order(bits 47 to 16) and check value
						equals h'55555555.	
				5. Write h'AAAAAAAA to high order 32 bits.
				6. Write h'AAAAAAAA to low order 32 bits.
				7. Read back high order and check value equals h'AAAAAAAA.
				8. Read back middle order(bits 47 to 16) and check value
						 equals h'AAAAAAAA.
				
				This test assumes registers R1 to R5 are working.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_Accumulator(void)
{
	/*Note ACC is a 64 bit register.
	It is read and written to using two 32bit accesses.*/
	
	/*Preseve register to be tested by saving to other registers*/
	__asm__ ("MVFACHI R14"); /*High order*/
	__asm__ ("MVFACMI R15"); /*Middle order*/
	
	/*Values to write to test registers*/
	__asm__ ("MOV.L #055555555h,R1"); /*Test Value 1*/
	__asm__ ("MOV.L #0AAAAAAAAh,R2"); /*Test Value 2*/
	
	/*Write test value 1*/
	__asm__ ("MVTACHI R1"); /*High order*/
	__asm__ ("MVTACLO R1"); /*Low order*/
	
	/*Read value back*/
	__asm__ ("MVFACHI R3"); /*High order*/
	__asm__ ("MVFACMI R4"); /*Middle order*/
		
	/*Compare with value expected*/	
	__asm__ ("CMP R1,R3");
	/*If not equall then branch to Failed label.*/		
	__asm__ ("BNE LABEL_FAILED_Test_Accumulator");
	/*Compare with value expected*/	
	__asm__ ("CMP R1,R4");
	/*If not equall then branch to Failed label.*/		
	__asm__ ("BNE LABEL_FAILED_Test_Accumulator");
	
	
	/*Write test value 2*/
	__asm__ ("MVTACHI R2"); /*High order*/
	__asm__ ("MVTACLO R2"); /*Low order*/
	
	/*Read value back*/
	__asm__ ("MVFACHI R3"); /*High order*/
	__asm__ ("MVFACMI R4"); /*Middle order*/
	
	/*Compare with value expected*/	
	__asm__ ("CMP R2,R3");
	/*If not equall then branch to Failed label.*/		
	__asm__ ("BNE LABEL_FAILED_Test_Accumulator");
	/*Compare with value expected*/	
	__asm__ ("CMP R2,R4");
	/*If not equall then branch to Failed label.*/		
	__asm__ ("BNE LABEL_FAILED_Test_Accumulator");
	
	/*Restore register*/
	__asm__ ("MVTACHI R14"); /*High order*/
	/*R15 contains middle order - so shift left 16 bits as have to write to low order.*/
	/*NOTE: It us safe to always set b0-b15 to zero.*/
	__asm__ ("SHLL #16, R15");
	__asm__ ("MVTACLO R15"); /*Low order*/
	
	/*All Tests Passed - so skip the call to the error handler.*/
	__asm__ ("BRA LABEL_PASSED_Test_Accumulator");
	
	__asm__ ("LABEL_FAILED_Test_Accumulator:");
	/*Test failed - branch to the Error handler subroutine*/
	__asm__ ("BSR _CPU_Test_ErrorHandler");

	__asm__ ("LABEL_PASSED_Test_Accumulator:");
	/*Passed - return from this function.*/
}

/***********************************************************************************
Function Name : CPU_Test_PC
Description   :	Program Counter (PC) register test.
				This provides a confidence check that the PC is working.
				This tests that the PC is working by calling a function.
				The function is located in its own section so that it can
				be located away from this function - so that when it is called
				more of the PC Register bits are required for it to work.
				So that this function can be sure that the function
				has actually been executed it returns the inverse of
				the supplied parameter.
				
Parameters    : none
Return value  : void
***********************************************************************************/
void CPU_Test_PC(void)
{	
	uint32_t ui32value = 0xAAAAAAAAUL;
	if(~ui32value != TestPC_TestFunction(ui32value))
	{
		CPU_Test_ErrorHandler();
	}
}

/***********************************************************************************
Function Name : TestPC_TestFunction
Description   :	This function is called as part of the PC Test routine.
				It is located in its own section so that it can be located
				away from the PC Test routine - so that when it is called
				more of the PC Register bits are required for it to work.
				So that the PC Test routine is confident that this function
				has actually been executed it returns the inverse of
				the supplied parameter.
Parameters    : uint32_t ui32Value - value to be inversed.
Return value  : Inverse of ui32Value.
***********************************************************************************/
static uint32_t TestPC_TestFunction(uint32_t ui32Value)
{
	ui32Value = (~ui32Value);
	return ui32Value;
}

/***********************************************************************************
Function Name : ForImport_CPU_Test
Description   :	This function purely just provides the assembler with
				the import of the error handler so that other inline
				assembley functions in this file can call it.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void ForImport_CPU_Test(void)
{
	CPU_Test_ErrorHandler();
}
