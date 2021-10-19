/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008 IAR Systems. All rights reserved.
 *
 * $Revision: 36645 $
 *
 * Modified by Stefano Ughi May 11,2011
 * Coldfire V1 Compliant 
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup       
	PUBLIC	__iar_program_start
	EXTERN  __vector_table
        EXTERN  main
        EXTERN  exit
        EXTWEAK __safety_IEC60335_B_TestWD
        EXTWEAK __safety_IEC60335_B_TestCPU
        EXTWEAK __safety_IEC60335_B_TestRAM
        EXTWEAK __initialize_hardware
        EXTWEAK __lib_reset_manager
        EXTWEAK __initialize_system
 
        SECTION .text:CODE:REORDER(1)
        THUMB
__iar_program_start:	

        ;BL      __safety_IEC60335_B_TestWD
        BL      __initialize_hardware
        ;BL      __safety_IEC60335_B_TestCPU
        ;BL      __safety_IEC60335_B_TestRAM
        BL      __lib_reset_manager
        BL      __initialize_system


        MOVS    r0,#0   ;  No parameters
        BL      main

        BL      exit
	REQUIRE __vector_table
	

        END
