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
* File Name : cpu_test_coupling.c
* Version : 1.00
* Device : RX210
* Tool Chain : GNU Rx
* H/W Platform : 
* Description : Self Test code.
*               CPU General Purpose Register Coupling Tests for Rx.
*				
*				NOTE: This file is not required if only using the
*				non-coupling general purpose register tests.
*				 
*				 This file should be built with optimisation off.
**********************************************************************/

/**********************************************************************************
Revision History
DD-MMM-YYYY OSO-UID Description
20-Jan-2011 RTE-PJI First Release
03-Feb-2011 VDE (Version 1.0) (RX62T)
15-Nov-2011 VDE (Version 1.1) RX210
09-May-2012 RTE-PJI Ported to GCC (Version 1.2)
***********************************************************************************/

/***********************************************************************************
Includes
***********************************************************************************/
#include "misratypes.h"
#include "cpu_test.h"

#ifndef __STRICT_ANSI__
/*
     -ansi  turns off certain features of GCC that are incompatible with ISO C90
     (when compiling C code), or of standard C++ (when compiling C++ code), such as
     the asm and typeof keywords, and predefined macros such as unix and vax that
     identify the type of system you are using. It also enables the undesirable
     and rarely used ISO trigraph feature. For the C compiler, it disables recognition
     of C++ style `//' comments as well as the inline keyword.

     see 3.4 Options Controlling C Dialect
 */
#define __inline__ inline
#endif

/***********************************************************************************
Function Prototypes
***********************************************************************************/
					


/*Inline Assembley functions*/
__inline__ static void TestGPRsCouplingStart_A(void) __attribute__((always_inline));
__inline__ static void TestGPRsCouplingR2_R4_A(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR5_R7_A(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR8_R10_A(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR11_R14_A(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR0andR1_A(void)__attribute__((always_inline));

__inline__ static void TestGPRsCouplingStart_B(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR2_R4_B(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR5_R7_B(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR8_R10_B(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR11_R14_B(void)__attribute__((always_inline));
__inline__ static void TestGPRsCouplingR0andR1_B(void)__attribute__((always_inline));

__inline__ static void TestGPRsCouplingEnd(void)__attribute__((always_inline));

void ForImport_CPU_Test_Coupling(void);

/***********************************************************************************
Program Code
***********************************************************************************/

/***********************************************************************************
Function Name : CPU_Test_GPRsCouplingPartA
Description   :	Tests general purpose registers R0 to R15.
				Coupling faults between the registers are detected.
				This is PartA of a complete GPR test, use function
				CPU_Test_GPRsCouplingPartB to complete the test.
						
				Registers are tested by 
				1. Setting all to H'AAAAAAAA
				2. For each register in turn  H'55555555 is written and then
				other registers are read to check they haven't changed.
				
				Initially R2 to R15 are tested using R1 as a utility register and preserving R0(SP).
				Then R0 and R15 are tested seperately against all other registers.
				
				Note:
				The rules for register preservation state that
				registers R0,R6,R7,R8,R9,R10,R11,R12 and R13 must be preserved by a function.
				Hence this function does preserve the values of these registers.
				The stack is used for this purpose.
				
				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_GPRsCouplingPartA(void)
{
	/*NOTE These functions must all be inline so that there is no use of the
	stack for returns from functions.*/
	TestGPRsCouplingStart_A();
	TestGPRsCouplingR2_R4_A();
	TestGPRsCouplingR5_R7_A();
	TestGPRsCouplingR8_R10_A();
	TestGPRsCouplingR11_R14_A();
	TestGPRsCouplingR0andR1_A();
	TestGPRsCouplingEnd();
}


/***********************************************************************************
Function Name : CPU_Test_GPRsCouplingPartB
Description   :	Tests general purpose registers R0 to R15.
				Coupling faults between the registers are detected.
				This is PartB of a complete GPR test, use function
				CPU_Test_GPRsCouplingPartA to complete the test.
						
				Registers are tested by 
				1. Setting all to H'55555555
				2. For each register in turn  H'AAAAAAAA is written and then
				other registers are read to check they haven't changed.
				
				Initially R2 to R15 are tested using R1 as a utility register and preserving R0(SP).
				Then R0 and R15 are tested seperately against all other registers.
				
				Note:
				The rules for register preservation state that
				registers R0,R6,R7,R8,R9,R10,R11,R12 and R13 must be preserved by a function.
				Hence this function does preserve the values of these registers.
				The stack is used for this purpose.
				
				AN INTERRUPT MUST NOT OCCUR DURING THIS FUNCTION.
				IT IS THE CALLING FUNCTIONS RESPONSIBILITY TO ENSURE THIS.
				
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void CPU_Test_GPRsCouplingPartB(void)
{	
	TestGPRsCouplingStart_B();
	TestGPRsCouplingR2_R4_B();
	TestGPRsCouplingR5_R7_B();
	TestGPRsCouplingR8_R10_B();
	TestGPRsCouplingR11_R14_B();
	TestGPRsCouplingR0andR1_B();
	TestGPRsCouplingEnd();
}

/***********************************************************************************
Function Name : TestGPRsCouplingStart_A
Description   :	Starts Part A of the GPRs Coupling Test.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingStart_A(void)
{
	/*Preserve R6 to R13 on the stack */
	__asm__ ("PUSH.L R6");
	__asm__ ("PUSH.L R7");
	__asm__ ("PUSH.L R8");
	__asm__ ("PUSH.L R9");
	__asm__ ("PUSH.L R10");
	__asm__ ("PUSH.L R11");
	__asm__ ("PUSH.L R12");
	__asm__ ("PUSH.L R13");
	
	/*Set all registers to h'AAAAAAAA*/
	__asm__ ("MOV.L #0AAAAAAAAh, R1");
	__asm__ ("MOV.L R1, R2");
	__asm__ ("MOV.L R1, R3");
	__asm__ ("MOV.L R1, R4");
	__asm__ ("MOV.L R1, R5");
	__asm__ ("MOV.L R1, R6");
	__asm__ ("MOV.L R1, R7");
	__asm__ ("MOV.L R1, R8");
	__asm__ ("MOV.L R1, R9");
	__asm__ ("MOV.L R1, R10");
	__asm__ ("MOV.L R1, R11");
	__asm__ ("MOV.L R1, R12");
	__asm__ ("MOV.L R1, R13");
	__asm__ ("MOV.L R1, R14");
	__asm__ ("MOV.L R1, R15");
	
	/*Check all read back as h'AAAAAAAA*/
	__asm__ ("CMP R1,R2");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R3");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R4");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R5");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R6");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R7");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R8");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R9");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R10");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R11");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R12");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R13");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R14");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R15");
	__asm__ ("BNE LABEL_FAILED");
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR2_R4_A
Description   :	Part A of the GPRs Coupling Test - Registers R2 to R4.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR2_R4_A(void)
{
	
	/*** Start R2 h'5555555 Test ***/
		/*Set just R2 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R2");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R2 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R2");
	/*** End R2 h'55555555 Test ***/
	
	/*** Start R3 h'5555555 Test ***/
		/*Set just R3 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R3");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R3 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R3");
	/*** End R3 h'55555555 Test ***/
	
	/*** Start R4 h'5555555 Test ***/
		/*Set just R4 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R4");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R4 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R4");
	
	/*** End R4 h'55555555 Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR5_R7_A
Description   :	Part A of the GPRs Coupling Test - Registers R5 to R7.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR5_R7_A(void)
{
	
	/*** Start R5 h'5555555 Test ***/
		/*Set just R5 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R5");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R5 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R5");
	/*** End R5 h'55555555 Test ***/
	
	/*** Start R6 h'5555555 Test ***/
		/*Set just R6 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R6");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R6 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R6");
	/*** End R6 h'55555555 Test ***/
	
	/*** Start R7 h'5555555 Test ***/
		/*Set just R7 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R7");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R7 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R7");
	/*** End R7 h'55555555 Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR8_R10_A
Description   :	Part A of the GPRs Coupling Test - Registers R8 to R10.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR8_R10_A(void)
{
	/*** Start R8 h'5555555 Test ***/
		/*Set just R8 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R8");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R8 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R8");
	/*** End R8 h'55555555 Test ***/
	
	/*** Start R9 h'5555555 Test ***/
		/*Set just R9 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R9");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R9 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R9	");
	/*** End R9 h'55555555 Test ***/
	
	/*** Start R10 h'5555555 Test ***/
		/*Set just R10 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R10");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R10 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R10");
	/*** End R10 h'55555555 Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR11_R14_A
Description   :	Part A of the GPRs Coupling Test - Registers R11 to R14.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR11_R14_A(void)
{
	/*** Start R11 h'5555555 Test ***/
		/*Set just R11 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R11");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R11 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R11");
	/*** End R11 h'55555555 Test ***/
	
	/*** Start R12 h'5555555 Test ***/
		/*Set just R12 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R12");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R12 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R12	");
	/*** End R12 h'55555555 Test ***/
	
	/*** Start R13 h'5555555 Test ***/
		/*Set just R13 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R13");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R13 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R13");
	/*** End R13 h'55555555 Test ***/
	
	/*** Start R14 h'5555555 Test ***/
		/*Set just R14 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R1");
		__asm__ ("MOV.L R1, R14");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R14 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R1, R14");
	/*** End R14 h'55555555 Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR0andR1_A
Description   :	Part A of the GPRs Coupling Test - Registers R0 and R1.
				Part A uses H'AAAAAAAA as the background pattern and h'55555555
				as the test pattern.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR0andR1_A(void)
{
	/*NOTE: Initially this function uses R2 and R3 as utility registers
	;- hence they are excluded from the initial tests. Then R4 and R5 are
	;used as utility registers allowing R2 and R3 to be used in test.*/
	
	/*Preserve R0 by saving to R3*/
	__asm__ ("MOV.L R0, R3");
	
	/*Set R1 to h'AAAAAAAA so all registers in test are h'AAAAAAAA*/
	__asm__ ("MOV.L #0AAAAAAAAh, R1");
	
	/*** Start R0 h'5555555 Test ***/
		/*Set just R0 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R2");
		__asm__ ("MOV.L R2, R0");
		/* Check value written is read back*/
		__asm__ ("CMP R2,R0");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R2");

		__asm__ ("CMP R2,R1");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R0 to h'AAAAAAAA*/
		__asm__ ("MOV.L R2, R0");
	/*** End R0 h'55555555 Test ***/
	
	
	/*** Start R1 h'5555555 Test ***/
		/*Set just R1 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R2");
		__asm__ ("MOV.L R2, R1");
		/* Check value written is read back*/
		__asm__ ("CMP R2,R1");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R2");

		__asm__ ("CMP R2,R0");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R1 back to h'AAAAAAAA*/
		__asm__ ("MOV.L R2, R1");
	/*** End R1 h'55555555 Test ***/
	
	/*Restore R0*/
	__asm__ ("MOV.L R3, R0");
	
	/* NOW CHANGE UTILITY REGISTERS (to R4 and R5)
	AND TEST WITH OLD UTILTY REGISTERS (R2 and R3) */
	
	/*Preserve R0 by saving to R5*/
	__asm__ ("MOV.L R0, R5");
	
	/*Set R1,R2 and R3 to h'AAAAAAAA so all registers in test are h'AAAAAAAA*/
	__asm__ ("MOV.L #0AAAAAAAAh, R1");
	__asm__ ("MOV.L R1, R2");
	__asm__ ("MOV.L R1, R3");
	
	/*** Start R0 h'5555555 Test against old utility registers***/
		/*Set just R0 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R4");
		__asm__ ("MOV.L R4, R0");
		/* Check value written is read back*/
		__asm__ ("CMP R4,R0");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check R2 and R3 haven't been changed and are therefore h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R4");

		__asm__ ("CMP R4,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R4,R3");
		__asm__ ("BNE LABEL_FAILED");
		
		/*Set R0 to h'AAAAAAAA*/
		__asm__ ("MOV.L R4, R0");
	/*** End R0 h'55555555 Test ***/
	
	/*** Start R1 h'5555555 Test against old utility registers***/
		/*Set just R1 to h'5555555*/
		__asm__ ("MOV.L #055555555h, R4");
		__asm__ ("MOV.L R4, R1");
		/* Check value written is read back*/
		__asm__ ("CMP R4,R1");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check R2 and R3 haven't been changed and are therefore h'AAAAAAAA.*/
		__asm__ ("MOV.L #0AAAAAAAAh, R4");

		__asm__ ("CMP R4,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R4,R3");
		__asm__ ("BNE LABEL_FAILED");
		
	/*** End R0 h'55555555 Test ***/
	
	/*Restore R0*/
	__asm__ ("MOV.L R5, R0");
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingStart_B
Description   :	Starts Part B of the GPRs Coupling Test.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingStart_B(void)
{
	/*Preserve R6 to R13 on the stack */
	__asm__ ("PUSH.L R6");
	__asm__ ("PUSH.L R7");
	__asm__ ("PUSH.L R8");
	__asm__ ("PUSH.L R9");
	__asm__ ("PUSH.L R10");
	__asm__ ("PUSH.L R11");
	__asm__ ("PUSH.L R12");
	__asm__ ("PUSH.L R13");
	
	/*Set all registers to h'55555555*/
	__asm__ ("MOV.L #055555555h, R1");
	__asm__ ("MOV.L R1, R2");
	__asm__ ("MOV.L R1, R3");
	__asm__ ("MOV.L R1, R4");
	__asm__ ("MOV.L R1, R5");
	__asm__ ("MOV.L R1, R6");
	__asm__ ("MOV.L R1, R7");
	__asm__ ("MOV.L R1, R8");
	__asm__ ("MOV.L R1, R9");
	__asm__ ("MOV.L R1, R10");
	__asm__ ("MOV.L R1, R11");
	__asm__ ("MOV.L R1, R12");
	__asm__ ("MOV.L R1, R13");
	__asm__ ("MOV.L R1, R14");
	__asm__ ("MOV.L R1, R15");
	
	/*Check all read back as h'55555555*/
	__asm__ ("CMP R1,R2");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R3");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R4");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R5");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R6");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R7");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R8");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R9");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R10");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R11");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R12");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R13");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R14");
	__asm__ ("BNE LABEL_FAILED");
	__asm__ ("CMP R1,R15");
	__asm__ ("BNE LABEL_FAILED");
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR2_R4_B
Description   :	Part B of the GPRs Coupling Test - Registers R2 to R4.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR2_R4_B(void)
{
	
	/*** Start R2 h'5555555 Test ***/
		/*Set just R2 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R2");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R2 back to h'55555555*/
		__asm__ ("MOV.L R1, R2");
	/*** End R2 h'AAAAAAAA Test ***/
	
	/*** Start R3 h'5555555 Test ***/
		/*Set just R3 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R3");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R3 back to h'55555555*/
		__asm__ ("MOV.L R1, R3	");
	/*** End R3 h'AAAAAAAA Test ***/
	
	/*** Start R4 h'5555555 Test ***/
		/*Set just R4 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R4");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R4 back to h'55555555*/
		__asm__ ("MOV.L R1, R4");
	
	/*** End R4 h'AAAAAAAA Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR5_R7_B
Description   :	Part B of the GPRs Coupling Test - Registers R5 to R7.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR5_R7_B(void)
{
	
	/*** Start R5 h'5555555 Test ***/
		/*Set just R5 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R5");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R5 back to h'55555555*/
		__asm__ ("MOV.L R1, R5");
	/*** End R5 h'AAAAAAAA Test ***/
	
	/*** Start R6 h'5555555 Test ***/
		/*Set just R6 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R6");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R6 back to h'55555555*/
		__asm__ ("MOV.L R1, R6	");
	/*** End R6 h'AAAAAAAA Test ***/
	
	/*** Start R7 h'5555555 Test ***/
		/*Set just R7 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R7");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R7 back to h'55555555*/
		__asm__ ("MOV.L R1, R7");
	/*** End R7 h'AAAAAAAA Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR8_R10_B
Description   :	Part B of the GPRs Coupling Test - Registers R8 to R10.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR8_R10_B(void)
{
	/*** Start R8 h'5555555 Test ***/
		/*Set just R8 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R8");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R8 back to h'55555555*/
		__asm__ ("MOV.L R1, R8");
	/*** End R8 h'AAAAAAAA Test ***/
	
	/*** Start R9 h'5555555 Test ***/
		/*Set just R9 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R9");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R9 back to h'55555555*/
		__asm__ ("MOV.L R1, R9	");
	/*** End R9 h'AAAAAAAA Test ***/
	
	/*** Start R10 h'5555555 Test ***/
		/*Set just R10 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R10");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R10 back to h'55555555*/
		__asm__ ("MOV.L R1, R10");
	/*** End R10 h'AAAAAAAA Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR11_R14_B
Description   :	Part B of the GPRs Coupling Test - Registers R11 to R14.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				R1 is used as a utility register.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR11_R14_B(void)
{
	/*** Start R11 h'5555555 Test ***/
		/*Set just R11 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R11");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R11 back to h'55555555*/
		__asm__ ("MOV.L R1, R11");
	/*** End R11 h'AAAAAAAA Test ***/
	
	/*** Start R12 h'5555555 Test ***/
		/*Set just R12 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R12");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R12 back to h'55555555*/
		__asm__ ("MOV.L R1, R12	");
	/*** End R12 h'AAAAAAAA Test ***/
	
	/*** Start R13 h'5555555 Test ***/
		/*Set just R13 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R13");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R13 back to h'55555555*/
		__asm__ ("MOV.L R1, R13");
	/*** End R13 h'AAAAAAAA Test ***/
	
	/*** Start R14 h'5555555 Test ***/
		/*Set just R14 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R1");
		__asm__ ("MOV.L R1, R14");
		/* Check value written is read back*/
		__asm__ ("CMP R1,R14");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R1");

		__asm__ ("CMP R1,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R3");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R1,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R14 back to h'55555555*/
		__asm__ ("MOV.L R1, R14");
	/*** End R14 h'AAAAAAAA Test ***/
	
	/*Passed*/
}

/***********************************************************************************
Function Name : TestGPRsCouplingR0andR1_B
Description   :	Part B of the GPRs Coupling Test - Registers R0 and R1.
				Part B uses H'55555555 as the background pattern and h'AAAAAAAA
				as the test pattern.
				If an error is detected then external function
				CPU_Test_ErrorHandler will be called.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingR0andR1_B(void)
{
	/*NOTE: Initially this function uses R2 and R3 as utility registers
	;- hence they are excluded from the initial tests. Then R4 and R5 are
	;used as utility registers allowing R2 and R3 to be used in test.*/
	
	/*Preserve R0 by saving to R3*/
	__asm__ ("MOV.L R0, R3");
	
	/*Set R1 to h'55555555 so all registers in test are h'55555555*/
	__asm__ ("MOV.L #055555555h, R1");
	
	/*** Start R0 h'5555555 Test ***/
		/*Set just R0 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R2");
		__asm__ ("MOV.L R2, R0");
		/* Check value written is read back*/
		__asm__ ("CMP R2,R0");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R2");

		__asm__ ("CMP R2,R1");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R0 to h'55555555*/
		__asm__ ("MOV.L R2, R0");
	/*** End R0 h'AAAAAAAA Test ***/
	
	
	/*** Start R1 h'5555555 Test ***/
		/*Set just R1 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R2");
		__asm__ ("MOV.L R2, R1");
		/* Check value written is read back*/
		__asm__ ("CMP R2,R1");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check other registers haven't been changed and are therefore*/
		/*are all h'55555555.*/
		__asm__ ("MOV.L #055555555h, R2");

		__asm__ ("CMP R2,R0");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R4");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R5");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R6");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R7");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R8");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R9");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R10");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R11");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R12");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R13");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R2,R15");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Set R1 back to h'55555555*/
		__asm__ ("MOV.L R2, R1");
	/*** End R1 h'AAAAAAAA Test ***/
	
	/*Restore R0*/
	__asm__ ("MOV.L R3, R0");
	
	/* NOW CHANGE UTILITY REGISTERS (to R4 and R5)
	AND TEST WITH OLD UTILTY REGISTERS (R2 and R3) */
	
	/*Preserve R0 by saving to R5*/
	__asm__ ("MOV.L R0, R5");
	
	/*Set R1,R2 and R3 to h'55555555 so all registers in test are h'55555555*/
	__asm__ ("MOV.L #055555555h, R1");
	__asm__ ("MOV.L R1, R2");
	__asm__ ("MOV.L R1, R3");
	
	/*** Start R0 h'5555555 Test against old utility registers***/
		/*Set just R0 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R4");
		__asm__ ("MOV.L R4, R0");
		/* Check value written is read back*/
		__asm__ ("CMP R4,R0");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check R2 and R3 haven't been changed and are therefore h'55555555.*/
		__asm__ ("MOV.L #055555555h, R4");

		__asm__ ("CMP R4,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R4,R3");
		__asm__ ("BNE LABEL_FAILED");
		
		/*Set R0 to h'55555555*/
		__asm__ ("MOV.L R4, R0");
	/*** End R0 h'AAAAAAAA Test ***/
	
	/*** Start R1 h'5555555 Test against old utility registers***/
		/*Set just R1 to h'5555555*/
		__asm__ ("MOV.L #0AAAAAAAAh, R4");
		__asm__ ("MOV.L R4, R1");
		/* Check value written is read back*/
		__asm__ ("CMP R4,R1");
		__asm__ ("BNE LABEL_FAILED");
	
		/*Check R2 and R3 haven't been changed and are therefore h'55555555.*/
		__asm__ ("MOV.L #055555555h, R4");

		__asm__ ("CMP R4,R2");
		__asm__ ("BNE LABEL_FAILED");
		__asm__ ("CMP R4,R3");
		__asm__ ("BNE LABEL_FAILED");
		
	/*** End R0 h'AAAAAAAA Test ***/
	
	/*Restore R0*/
	__asm__ ("MOV.L R5, R0");
	
	/*Passed*/
}


/***********************************************************************************
Function Name : Failed
Description   :	Provides LABEL_FAILED.
				Branches to the external error handler function.
Parameters    : none
Return value  : none
***********************************************************************************/
static void Failed(void)
{
	__asm__ ("LABEL_FAILED:");
	/*Test failed - branch to the Error handler subroutine*/
	__asm__ ("BSR _CPU_Test_ErrorHandler");
}


/***********************************************************************************
Function Name : ForImport_CPU_Test_Coupling
Description   :	This function purely just provides the assembler with
				the import of the error handler so that other inline
				assembley functions in this file can call it.
				
Parameters    : none				
Return Value  : void
***********************************************************************************/
void ForImport_CPU_Test_Coupling(void)
{
	CPU_Test_ErrorHandler();
	
	/*Call Failed function so it is called from somewhere so that
	 the compiler doesn't remove it.*/
	Failed();
}

/***********************************************************************************
Function Name : TestGPRsCouplingEnd
Description   :	Ends Part A or Part B of the GPRs Coupling Test.
				Restores registers from stack.
Parameters    : none
Return value  : none
***********************************************************************************/ 
static void TestGPRsCouplingEnd(void)
{
	/*Restore original register values R0, R6 to R13 from stack */
	__asm__ ("POP R13");
	__asm__ ("POP R12");
	__asm__ ("POP R11");
	__asm__ ("POP R10");
	__asm__ ("POP R9");
	__asm__ ("POP R8");
	__asm__ ("POP R7");
	__asm__ ("POP R6");
}
