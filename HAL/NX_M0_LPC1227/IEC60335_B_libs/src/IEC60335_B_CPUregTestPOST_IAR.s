;/****************************************************************************
; *   $Id:: IEC60335_B_CPUregTestPOST_IAR.s 7461 2011-06-01 13:01:58Z gerrit#$
; *   Project: NXP Cortex-M0 IEC60335 Class B certified library
; *
; *   Description:
; *     Source file for the IEC60335 Class B CPU register POST testing 
; *	   library for the IAR compiler.
; *
; ****************************************************************************
; * Software that is described herein is for illustrative purposes only
; * which provides customers with programming information regarding the
; * products. This software is supplied "AS IS" without any warranties.
; * NXP Semiconductors assumes no responsibility or liability for the
; * use of the software, conveys no license or title under any patent,
; * copyright, or mask work right to the product. NXP Semiconductors
; * reserves the right to make changes in the software without
; * notification. NXP Semiconductors also make no representation or
; * warranty that such application will be suitable for the specified
; * use without further testing or modification.
;****************************************************************************/

  SECTION .text:CODE

  #include "IEC60335_B_Config.h"
  
  PUBLIC  _CPUregTestPOST	
  EXTERN  Reset_Handler

;------------------------------------------
; memory variable for output of fault / pass condition
;------------------------------------------
  EXTERN  CPUregTestPOSTStatus  

;------------------------------------------
; optional user function to perform specific recovery or signaling actions
; can be overridden by the user / application code
; default implementation just keeps CPU in infinite loop
;------------------------------------------

  PUBWEAK _CPUpostTestFailureHook 

;------------------------------------------
; Test pattern definition 
pattern1 	EQU		0xAAAAAAAA
pattern2 	EQU		0xAAAAAAA8
pattern3 	EQU		0x55555555
pattern4 	EQU		0x55555554
pattern5 	EQU		0xA0000000
pattern6 	EQU		0x50000000
;------------------------------------------

;------------------------------------------
; Start of the CPU register test 
;------------------------------------------
_CPUregTestPOST
        
;------------------------------------------
; LOW REGISTERS: r0
; Since r0 is the first register to be tested and no other registers may be used
; r0 should be tested by only using immediate instructions
; Since instruction cmp can only compare with an immediate 8-bit value, the 
; bits to be tested need to be shifted to the LSB
;------------------------------------------
_cpu_low_test
  movs	r0, #0xAA   			; r0 - stuck at 0 test
  cmp 	r0, #0xAA   			; Test r0[7:0]
  bne     _cpu_low_test_fail
        
  movs  r0, #0xAA
  lsls  r0, r0, #8
  lsrs  r0, r0, #8
  cmp 	r0, #0xAA   			; Test r0[15:8]
  bne	_cpu_low_test_fail
	
  movs	r0, #0xAA
  lsls  r0, r0, #16
  lsrs	r0, r0, #16
  cmp 	r0, #0xAA   			; Test r0[23:16]
  bne	_cpu_low_test_fail
	
  movs	r0, #0xAA
  lsls  r0, r0, #24
  lsrs	r0, r0, #24
  cmp 	r0, #0xAA   			; Test r0[31:24]
  bne	_cpu_low_test_fail

  movs	r0, #0x55   			; r0 - inverted pattern and neighbour stuck test
  cmp 	r0, #0x55   			; Test r0[7:0]
  bne	_cpu_low_test_fail
  	
  movs	r0, #0x55
  lsls  r0, r0, #8
  lsrs  r0, r0, #8
  cmp 	r0, #0x55   			; Test r0[15:8]
  bne	_cpu_low_test_fail
        
  movs	r0, #0x55
  lsls  r0, r0, #16
  lsrs	r0, r0, #16
  cmp 	r0, #0x55   			; Test r0[23:16]
  bne	_cpu_low_test_fail
        
  movs	r0, #0x55
  lsls  r0, r0, #24
  lsrs	r0, r0, #24
  cmp 	r0, #0x55   			; Test r0[31:24]
  bne	_cpu_low_test_fail

_cpu_r0_test_pass
  b     _cpu_r1_r7_test 

_cpu_r0_test_fail
  b     _cpu_low_test_fail

;------------------------------------------
; LOW REGISTERS: r0 Test End 
;------------------------------------------

;------------------------------------------
; LOW REGISTERS r1-r7
; Register r1-r7 test:
; The registers under test will be written with pattern1 = 0xAAAA.AAAA
; Each register will be individually compared to r0 for a pass/fail for the
; test. The second part of the test will write the inverted pattern in the 
; register under test and pass/fail with a cmp
;------------------------------------------ 
_cpu_r1_r7_test

  ldr   r0,=pattern1  		; Load pattern1 in r0 
        
  mov   r1, r0     		; Put pattern1 in r1-r7 
  cmp   r1,r0         		; Compare with r0
  bne   _cpu_low_test_fail
        
  mov   r2,r0
  cmp   r2,r0
  bne   _cpu_low_test_fail

  mov   r3,r0
  cmp   r3,r0
  bne   _cpu_low_test_fail

  mov   r4,r0
  cmp   r4,r0
  bne   _cpu_low_test_fail

  mov   r5,r0
  cmp   r5,r0
  bne   _cpu_low_test_fail

  mov   r6,r0
  cmp   r6,r0
  bne   _cpu_low_test_fail

  mov   r7,r0
  cmp   r7,r0
  bne   _cpu_low_test_fail

  ldr   r0,=pattern3  		; Load pattern3 in r0

  mov   r1,r0         		; Put pattern3 in r1
  cmp   r1,r0         		; Compare with r0
  bne   _cpu_low_test_fail

  mov   r2,r0
  cmp   r2,r0
  bne   _cpu_low_test_fail

  mov   r3,r0
  cmp   r3,r0
  bne   _cpu_low_test_fail

  mov   r4,r0
  cmp   r4,r0
  bne   _cpu_low_test_fail

  mov   r5,r0
  cmp   r5,r0
  bne   _cpu_low_test_fail

  mov   r6,r0
  cmp   r6,r0
  bne   _cpu_low_test_fail

  mov   r7,r0
  cmp   r7,r0
  bne   _cpu_low_test_fail

_cpu_low_test_pass
  b     _cpu_high_test

_cpu_low_test_fail
  b	_cpu_high_test_fail

;------------------------------------------
; LOW REGISTERS: r1-r7 Test End 
;------------------------------------------

;------------------------------------------
; HIGH REGISTERS r8-r12
; Register r8-r12 test:
; The registers under test will be written with pattern1 = 0xAAAA.AAAA
; Each register will be individually compared to r0 for a pass/fail for the
; test. The second part of the test will write the inverted pattern in the 
; register under test and pass/fail with a cmp
;------------------------------------------ 
_cpu_high_test

  movs	r0, #0        		; Clear r0 
  ldr	r0,=pattern1  		; Load pattern1 in r0 

  mov	r8,r0         		; Put pattern1 in r8
  cmp	r8,r0         		; Compare with r0
  bne	_cpu_high_test_fail

  mov	r9,r0
  cmp	r9,r0
  bne	_cpu_high_test_fail

  mov	r10,r0
  cmp	r10,r0
  bne	_cpu_high_test_fail

  mov	r11,r0
  cmp	r11,r0
  bne	_cpu_high_test_fail

  mov	r12,r0
  cmp	r12,r0
  bne	_cpu_high_test_fail

  ldr	r0,=pattern3  		; Load pattern3 in r0 

  mov	r8,r0         		; Put pattern3 in r8 
  cmp	r8,r0         		; Compare with r0
  bne	_cpu_high_test_fail

  mov	r9,r0
  cmp	r9,r0
  bne	_cpu_high_test_fail

  mov	r10,r0
  cmp	r10,r0
  bne	_cpu_high_test_fail

  mov	r11,r0
  cmp	r11,r0
  bne	_cpu_high_test_fail

  mov	r12,r0
  cmp	r12,r0
  bne	_cpu_high_test_fail

_cpu_high_test_pass
  b	cpu_MSP_test

_cpu_high_test_fail
  b       _cpu_MSP_test_fail

;------------------------------------------
; HIGH REGISTERS: r8-r12 Test End 
;------------------------------------------

;------------------------------------------
; HIGH REGISTERS: Stack Pointer r13 (MSP, PSP)
; The test consists of two parts, the first does read/writes to r13 using the
; normal instructions. The second test accesses the register by using the MSR
; and MRS special register read/write instructions.
; For r13 a different pattern is used because bits r13[1:0] are always zero.
;------------------------------------------
cpu_MSP_test

  mrs	r3, CONTROL   			; Store the current CONTROL value 
  movs 	r6, #0x00      
  msr	CONTROL, r6   			; Select default stack (MSP)
  isb                   		; Ensure change is applied
  mov	r4, r13       			; Store the current SP 

  mrs	r5, MSP       			; Check whether MSP is really selected 
  cmp	r4,r5
  bne	_cpu_MSP_test_fail_res

  ldr	r0,=pattern2  			; Load pattern2 in r0
  mov	r13, r0       			; Move pattern2 in the MSP 
  cmp	r13, r0       			; Compare  
  bne	_cpu_MSP_test_fail_res

  ldr	r0,=pattern4  			; Load pattern4 in r0 
  mov	r13, r0       			; Move pattern4 in the MSP 
  cmp	r13, r0       			; Compare
  bne	_cpu_MSP_test_fail_res


  ldr	r0,=pattern2  			; Load pattern2 in r0 
  msr	MSP, r0       			; Move pattern2 in MSP with special instruction
  mrs 	r1, MSP       			; Copy MSP to r1, since cmp uses general regs
  cmp	r1, r0        			; Compare
  bne	_cpu_MSP_test_fail_res

  ldr	r0,=pattern4  			; Load pattern4 in r0 
  msr	MSP, r0       			; Move pattern4 in MSP with special instruction
  mrs 	r1, MSP       			; Copy MSP to r1, since cmp uses general regs
  cmp	r1, r0        			; Compare
  bne	_cpu_MSP_test_fail_res

_cpu_MSP_test_pass
  mov 	r13, r4       			; Restore SP 
  msr	CONTROL, r3   			; Restore CONTROL
  isb                   		; Ensure change is applied       
  b	cpu_PSP_test

_cpu_MSP_test_fail_res
  mov 	r13, r4       			; Restore SP
  msr	CONTROL, r3   			; Restore CONTROL
  isb                   		; Ensure change is applied        
_cpu_MSP_test_fail
  b	_cpu_PSP_test_fail

;------------------------------------------
; HIGH REGISTERS: Stack Pointer r13 (MSP) Test End 
;------------------------------------------
cpu_PSP_test
  mrs	r3, CONTROL   				; Store the current CONTROL value 
  movs 	r6, #0x2      
  msr	CONTROL, r6   				; Select stack (PSP)
  isb                   			; Ensure change is applied        
  mov	r4, r13       				; Store the current SP 

  mrs	r5, PSP       				; Check whether PSP is really selected 
  cmp	r4,r5
  bne	_cpu_PSP_test_fail_res

  ldr	r0,=pattern2  				; Load pattern2 in r0
  mov	r13, r0       				; Move pattern2 in the PSP 
  cmp	r13, r0       				; Compare  
  bne	_cpu_PSP_test_fail_res
  ldr	r0,=pattern4  				; Load pattern4 in r0 
  mov	r13, r0       				; Move pattern4 in the PSP 
  cmp	r13, r0       				; Compare
  bne	_cpu_PSP_test_fail_res


  ldr	r0,=pattern2  				; Load pattern2 in r0 
  msr	PSP, r0       				; Move pattern2 in PSP with special instruction
  mrs 	r1, PSP       				; Copy PSP to r1, since cmp uses general regs
  cmp	r1, r0        				; Compare
  bne	_cpu_PSP_test_fail_res

  ldr	r0,=pattern4  				; Load pattern4 in r0 
  msr	PSP, r0       				; Move pattern4 in PSP with special instruction
  mrs 	r1, PSP       				; Copy PSP to r1, since cmp uses general regs
  cmp	r1, r0        				; Compare
  bne	_cpu_PSP_test_fail_res

_cpu_PSP_test_pass
  mov 	r13, r4       				; Restore SP 
  msr	CONTROL, r3  	 			; Restore CONTROL
  isb                   			; Ensure change is applied
  b	_cpu_LR_test

_cpu_PSP_test_fail_res
  mov 	r13, r4       				; Restore SP 
  msr	CONTROL, r3   				; Restore CONTROL
  isb                   			; Ensure change is applied
_cpu_PSP_test_fail
  b     _cpu_LR_test_fail

;------------------------------------------
; HIGH REGISTERS: Stack Pointer r13 (PSP) Test End 
;------------------------------------------

;------------------------------------------
; SPECIAL REGISTERS: Link register r14 (LR)
; The link register will be written with pattern1, then compared for a 
; pass/fail. Then the inverse of the pattern will be tested.
;------------------------------------------
_cpu_LR_test
  mov     r3, r14       			; Store the current link register 

  ldr     r0,=pattern1  			; Load pattern1 in r0
  mov     r14, r0       			; Move pattern1 in the LR
  cmp     r14, r0       			; Compare
  bne     _cpu_LR_test_fail_res

  ldr     r0,=pattern3  			; Load pattern3 in r0
  mov     r14, r0       			; Move pattern3 in the LR
  cmp     r14, r0       			; Compare
  bne     _cpu_LR_test_fail_res

_cpu_LR_test_pass
  mov 	r14, r3       ;	Restore the LR
  b	_cpu_APSR_test

_cpu_LR_test_fail_res
  mov 	r14, r3       ;	Restore the LR

_cpu_LR_test_fail
  b       _cpu_APSR_test_fail

;------------------------------------------
; SPECIAL REGISTERS: Link register r14 (LR) Test End 
;------------------------------------------

;------------------------------------------
; SPECIAL REGISTERS: APSR register
; Only the APSR register can be test, the other PSR registers, IPSR and EPSR,
; are read-only. The APSR register will be written with pattern5, because 
; only APSR[31:28] are readable and writeable, then compared for a pass/fail.
; Then the inverse of the pattern will be tested.
;------------------------------------------
_cpu_APSR_test

  mrs     r3, APSR      		; Store the current APSR register 

  ldr     r0,=pattern5  		; Load pattern5 in r0 
  msr     APSR, r0      		; Move pattern5 in the APSR
  mrs     r1, APSR      		; Read pattern6 from the APSR to r1 
					; (since cmp uses general regs)
  cmp     r1, r0        		; Compare with the input pattern 
  bne     _cpu_APSR_test_fail_res

  ldr     r0,=pattern6  		; Load pattern6 in r0 
  msr     APSR, r0      		; Move pattern6 in the APSR
  mrs     r1, APSR      		; Read pattern6 from the APSR to r1
					; (since cmp uses general regs)
  cmp     r1, r0        		; Compare with the input pattern 
  bne     _cpu_APSR_test_fail_res

_cpu_APSR_test_pass
  msr 	APSR, r3      ;	Restore the APSR 
  b	_cpu_PRIMASK_test

_cpu_APSR_test_fail_res
  msr 	APSR, r3      ;	Restore the APSR 
_cpu_APSR_test_fail
  b 	_cpu_test_fail ; Jump to the end

;------------------------------------------
; SPECIAL REGISTERS: APSR register Test End 
;------------------------------------------

;------------------------------------------
; SPECIAL REGISTERS: PRIMASK register
; The PRIMASK bit 0 will be tested
; Only PRIMASK[0] is readable and writeable,it will be compared for a pass/fail
;------------------------------------------
_cpu_PRIMASK_test

  mrs   r3, PRIMASK   			; Store the current PRIMASK 
  movs  r0, #0        			; Load zero pattern in r0 
  msr   PRIMASK, r0   			; Clear PRIMASK bit 
  mrs   r1, PRIMASK   			; Read back from the PRIMASK 
  cmp   r1, r0        			; Compare
  bne   _cpu_PRIMASK_test_fail_res

  movs  r0, #1        			; Load 0x1 pattern in r0 
  msr   PRIMASK, r0   			; Set PRIMASK bit 
  mrs   r1, PRIMASK   			; Read back from the PRIMASK 
  cmp   r1, r0        			; Compare with the input pattern
  bne   _cpu_PRIMASK_test_fail_res

_cpu_PRIMASK_test_pass
  msr 	PRIMASK, r3   ;	Restore the PRIMASK 
  b	_cpu_test_pass

_cpu_PRIMASK_test_fail_res
  msr 	PRIMASK, r3   ;	Restore the PRIMASK 
  b 	_cpu_test_fail

;------------------------------------------
; SPECIAL REGISTERS: PRIMASK register Test End 
;------------------------------------------

_cpu_test_pass
;------------------------------------------
; End of the CPU POST register test 
;------------------------------------------
; Indicate in the CPUregTestPOST_struct that the CPU reg test passed
; indicated by 0x1
  movs	r0, #0x1
  ldr   r1, =CPUregTestPOSTStatus
  str   r0, [r1]
  bx    LR              ; test successfully finished
                        ; return
        
_cpu_test_fail
;------------------------------------------
; Try to write a feedback (fault status) in a specific RAM location
;------------------------------------------
; Indicate in the CPUregTestPOST_struct that the CPU reg test failed
; indicated by 0x0
  movs	r0, #0x0
  ldr   r1, =CPUregTestPOSTStatus
  str   r0, [r1]
  b     _CPUpostTestFailureHook


;------------------------------------------
; Keep the CPU in a loop (default) or call a user hook function if overridden
;------------------------------------------
_CPUpostTestFailureHook:
  b _CPUpostTestFailureHook  ; do nothing by default
  

  END



