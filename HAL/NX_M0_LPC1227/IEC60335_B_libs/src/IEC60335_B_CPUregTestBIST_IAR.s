;/****************************************************************************
; *   $Id:: IEC60335_B_CPUregTestBIST_IAR.s 7461 2011-06-01 13:01:58Z gerrit#$
; *   Project: NXP Cortex-M0 IEC60335 Class B certified library
; *
; *   Description:
; *     Source file for the IEC60335 Class B CPU register testing 
; *	    library for the IAR compiler.
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

	PUBLIC	_CPUregTestLOW
	PUBLIC	_CPUregTestMID
	PUBLIC	_CPUregTestHIGH
	PUBLIC	_CPUregTestSP
	PUBLIC	_CPUregTestSPEC
			
;/* Test pattern definition */ 
pattern1 	EQU		0xAAAAAAAA
pattern2 	EQU		0xAAAAAAA8
pattern3 	EQU		0x55555555
pattern4 	EQU		0x55555554
pattern5 	EQU		0xA0000000
pattern6 	EQU		0x50000000

;------------------------------------------
; memory variables for output of test result
;------------------------------------------
	EXTERN	CPUregTestBIST_struct 

; Test structure offset definitions 
testPassed  EQU 	0x4
testState   EQU		0x0
  
;------------------------------------------
; Start of the CPU register test LOW
;------------------------------------------
_CPUregTestLOW
        push    {r0-r7,r14} ; CortexM0 supports push on R0-R7, R14 (LR)

;------------------------------------------
; LOW REGISTERS: r0
; Since r0 is the first register to be tested and no other registers may be used
; r0 should be tested by only using immediate instructions
; Since instruction cmp can only compare with an immediate 8-bit value, the 
; bits to be tested need to be shifted to the LSB
;------------------------------------------
	movs	r0, #0xAA   			; r0 - stuck at 0 test
	cmp 	r0, #0xAA   			; Test r0[7:0]
	bne     _cpu_low_test_fail
        
	movs	r0, #0xAA
        lsls    r0, r0, #8				
	lsrs    r0, r0, #8
	cmp 	r0, #0xAA   			; Test r0[15:8]
	bne		_cpu_low_test_fail
	
        movs	r0, #0xAA
        lsls    r0, r0, #16
	lsrs	r0, r0, #16
	cmp 	r0, #0xAA   			; Test r0[23:16]
	bne		_cpu_low_test_fail
	
        movs	r0, #0xAA
        lsls    r0, r0, #24
	lsrs	r0, r0, #24
	cmp 	r0, #0xAA   			; Test r0[31:24]
	bne		_cpu_low_test_fail

	movs	r0, #0x55   			; r0 - inverted pattern and neighbour stuck test
	cmp 	r0, #0x55   			; Test r0[7:0]
	bne		_cpu_low_test_fail
	
        movs	r0, #0x55
        lsls    r0, r0, #8
	lsrs    r0, r0, #8
	cmp 	r0, #0x55   			; Test r0[15:8]
	bne		_cpu_low_test_fail
        
        movs	r0, #0x55
        lsls    r0, r0, #16
	lsrs	r0, r0, #16
	cmp 	r0, #0x55   			; Test r0[23:16]
	bne		_cpu_low_test_fail
        
        movs	r0, #0x55
        lsls    r0, r0, #24
	lsrs	r0, r0, #24
	cmp 	r0, #0x55   ; Test r0[31:24]
	bne		_cpu_low_test_fail
	
_cpu_r0_test_pass
 	b       _cpu_r1_r7_test 

_cpu_r0_test_fail
	b       _cpu_low_test_fail

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

	ldr     r0,=pattern1  		; Load pattern1 in r0 
        
	mov     r1,r0         		; Put pattern1 in r1-r7 
	cmp     r1,r0         		; Compare with r0
	bne     _cpu_low_test_fail
        
	mov     r2,r0
	cmp     r2,r0
	bne     _cpu_low_test_fail

	mov     r3,r0
	cmp     r3,r0
	bne     _cpu_low_test_fail

	mov     r4,r0
	cmp     r4,r0
	bne     _cpu_low_test_fail

	mov     r5,r0
	cmp     r5,r0
	bne     _cpu_low_test_fail

	mov     r6,r0
	cmp     r6,r0
	bne     _cpu_low_test_fail

	mov     r7,r0
	cmp     r7,r0
	bne     _cpu_low_test_fail

	ldr     r0,=pattern3  		; Load pattern3 in r0

	mov     r1,r0         		; Put pattern3 in r1
	cmp     r1,r0         		; Compare with r0
	bne     _cpu_low_test_fail

	mov     r2,r0
	cmp     r2,r0
	bne     _cpu_low_test_fail

	mov     r3,r0
	cmp     r3,r0
	bne     _cpu_low_test_fail

	mov     r4,r0
	cmp     r4,r0
	bne     _cpu_low_test_fail

	mov     r5,r0
	cmp     r5,r0
	bne     _cpu_low_test_fail

	mov     r6,r0
	cmp     r6,r0
	bne     _cpu_low_test_fail

	mov     r7,r0
	cmp     r7,r0
	bne     _cpu_low_test_fail

_cpu_low_test_pass
; Indicate in the CPUregTestBIST_struct that test passed
; indicated by 0x1
        movs	r0, #0x1
	ldr     r1, =CPUregTestBIST_struct+testState
	str     r0, [r1]
; Indicate LOW test has passed 
        movs    r0, #0xFF
        ldr     r2, [r1,#testPassed]
        orrs    r0, r0, r2
        str     r0, [r1,#testPassed]
	b       _cpu_low_test_return

_cpu_low_test_fail
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1]

_cpu_low_test_return
        pop     {r0-r7,PC} ; Restore R0 to R7 and jump back, SP is updated

;------------------------------------------
; LOW REGISTERS: r1-r7 Test End 
;------------------------------------------

;------------------------------------------
; MID REGISTERS r4-r10
; Register r4-r10 test:
; The registers under test will be written with pattern1 = 0xAAAA.AAAA
; Each register will be individually compared to r0 for a pass/fail for the
; test. The second part of the test will write the inverted pattern in the 
; register under test and pass/fail with a cmp
;------------------------------------------ 
_CPUregTestMID

        push    {r0-r7,r14} 		; CortexM0 supports push on R0-R7, R14 (LR)
        mov     r3, r8
        mov     r4, r9
        mov     r5, r10
        push    {r3-r5}     		; push the rest on the stack, SP is updated

        movs	r0, #0        		; Clear r0 
	ldr		r0,=pattern1  			; Load pattern1 in r0 

	mov		r4,r0         			; Put pattern1 in r4
	cmp		r4,r0         			; Compare with r0
	bne		_cpu_mid_test_fail

	mov		r5,r0
	cmp		r5,r0
	bne		_cpu_mid_test_fail

	mov		r6,r0
	cmp		r6,r0
	bne		_cpu_mid_test_fail

	mov		r7,r0
	cmp		r7,r0
	bne		_cpu_mid_test_fail

	mov		r8,r0
	cmp		r8,r0
	bne		_cpu_mid_test_fail
	
	mov		r9,r0
	cmp		r9,r0
	bne		_cpu_mid_test_fail
	
	mov		r10,r0
	cmp		r10,r0
	bne		_cpu_mid_test_fail
	
	ldr		r0,=pattern3  			; Load pattern3 in r0 

	mov		r4,r0         			; Put pattern3 in r4 
	cmp		r4,r0         			; Compare with r0
	bne		_cpu_mid_test_fail
	
	mov		r5,r0
	cmp		r5,r0
	bne		_cpu_mid_test_fail

	mov		r6,r0
	cmp		r6,r0
	bne		_cpu_mid_test_fail

	mov		r7,r0
	cmp		r7,r0
	bne		_cpu_mid_test_fail

	mov		r8,r0
	cmp		r8,r0
	bne		_cpu_mid_test_fail

	mov		r9,r0
	cmp		r9,r0
	bne		_cpu_mid_test_fail
	
	mov		r10,r0
	cmp		r10,r0
	bne		_cpu_mid_test_fail

_cpu_mid_test_pass
; Indicate in the CPUregTestBIST_struct that test passed
; indicated by 0x1
        movs	r0, #0x01
	ldr     r1, =CPUregTestBIST_struct+testState
	str     r0, [r1]
; Indicate MID test has passed 
        movs    r0, #0x7F
	lsls	r0, r0, #4 			; flags #0x7F0
        ldr     r2, [r1,#testPassed]
        orrs    r0, r0, r2
        str     r0, [r1,#testPassed]
	b       _cpu_mid_test_return

_cpu_mid_test_fail
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1]

_cpu_mid_test_return
        pop     {r3-r5}     ; take back R8 to R10, SP is updated 
        mov     r10, r5
        mov     r9, r4
        mov     r8, r3
        pop     {r0-r7,PC} ; Restore R0 to R7 and jump back, SP is updated

;------------------------------------------
; MID REGISTERS: r4-r10 Test End 
;------------------------------------------	


;------------------------------------------
; HIGH REGISTERS r8-r12
; Register r8-r12 test:
; The registers under test will be written with pattern1 = 0xAAAA.AAAA
; Each register will be individually compared to r0 for a pass/fail for the
; test. The second part of the test will write the inverted pattern in the 
; register under test and pass/fail with a cmp
;------------------------------------------ 
_CPUregTestHIGH

        push    {r0-r7,r14} 		; CortexM0 supports push on R0-R7, R14 (LR)
        mov     r3, r8
        mov     r4, r9
        mov     r5, r10
        mov     r6, r11
        mov     r7, r12
        push    {r3-r7}     		; Push the rest on the stack, SP is updated

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
; Indicate in the CPUregTestBIST_struct that test passed
; indicated by 0x1
        movs	r0, #0x01
	ldr     r1, =CPUregTestBIST_struct+testState
	str     r0, [r1]
; Indicate HIGH test has passed 
        movs    r0, #0x1F
	lsls	r0, r0, #8      	; flags #0x1F00
        ldr     r2, [r1, #testPassed]
        orrs    r0, r0, r2
        str     r0, [r1,#testPassed]
	b       _cpu_high_test_return

_cpu_high_test_fail
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1]

_cpu_high_test_return
        
	pop     {r3-r7}     ; Take back R8 to R12, SP is updated 
        mov     r12, r7
        mov     r11, r6
        mov     r10, r5
        mov     r9, r4
        mov     r8, r3
        pop     {r0-r7,PC} ; Restore R0 to R7 and jump back, SP is updated

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
_CPUregTestSP
        push    {r0-r7,r14} 		; CortexM0 supports push on R0-R7, R14 (LR)
	mrs	r3, CONTROL   		; Store the current CONTROL value 
	movs 	r6, #0x00      
	msr	CONTROL, r6   		; Select default stack (MSP)
        isb                   		; Ensure change is applied
        mov	r4, r13       		; Store the current SP 

	mrs	r5, MSP       		; Check whether MSP is really selected 
	cmp	r4,r5
	bne	_cpu_MSP_test_fail

	ldr	r0,=pattern2  		; Load pattern2 in r0
	mov	r13, r0       		; Move pattern2 in the MSP 
	cmp	r13, r0       		; Compare  
	bne	_cpu_MSP_test_fail

	ldr	r0,=pattern4  		; Load pattern4 in r0 
	mov	r13, r0       		; Move pattern4 in the MSP 
	cmp	r13, r0       		; Compare
	bne	_cpu_MSP_test_fail


	ldr	r0,=pattern2  		; Load pattern2 in r0 
	msr	MSP, r0       		; Move pattern2 in MSP with special instruction
	mrs 	r1, MSP       		; Copy MSP to r1, since cmp uses general regs
	cmp	r1, r0        		; Compare
	bne	_cpu_MSP_test_fail

	ldr	r0,=pattern4  			; Load pattern4 in r0 
	msr	MSP, r0       		; Move pattern4 in MSP with special instruction
	mrs 	r1, MSP       		; Copy MSP to r1, since cmp uses general regs
	cmp	r1, r0        		; Compare
	bne	_cpu_MSP_test_fail
        b	_cpu_MSP_test_pass      ; Now go to test the other mode

_cpu_MSP_test_fail
	mov 	r13, r4       		; Restore SP
	msr	CONTROL, r3   		; Restore CONTROL
        isb                   		; Ensure change is applied 
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1] 
        pop     {r0-r7,PC} 		; Restore R0 to R7 and jump back, SP is updated
        
_cpu_MSP_test_pass
	mov 	r13, r4       		; Restore SP
	msr	CONTROL, r3   		; Restore CONTROL
        isb                   		; ensure change is applied 

;------------------------------------------
; HIGH REGISTERS: Stack Pointer r13 (MSP) Test End 
;------------------------------------------


cpu_PSP_test
	mrs	r3, CONTROL   		; Store the current CONTROL value 
	movs 	r6, #0x2      
	msr	CONTROL, r6   		; Select stack (PSP)
        isb                   		; ensure change is applied        
	mov	r4, r13       		; Store the current SP 

	mrs	r5, PSP       		; Check whether PSP is really selected 
	cmp	r4,r5
	bne	_cpu_PSP_test_fail

	ldr	r0,=pattern2  		; Load pattern2 in r0
	mov	r13, r0       		; Move pattern2 in the PSP 
	cmp	r13, r0       		; Compare  
	bne	_cpu_PSP_test_fail

	ldr	r0,=pattern4  		; Load pattern4 in r0 
	mov	r13, r0       		; Move pattern4 in the PSP 
	cmp	r13, r0       		; Compare
	bne	_cpu_PSP_test_fail


	ldr	r0,=pattern2  		; Load pattern2 in r0 
	msr	PSP, r0       		; Move pattern2 in PSP with special instruction
	mrs 	r1, PSP       		; Copy PSP to r1, since cmp uses general regs
	cmp	r1, r0        		; Compare
	bne	_cpu_PSP_test_fail

	ldr	r0,=pattern4  		; Load pattern4 in r0 
	msr	PSP, r0       		; Move pattern4 in PSP with special instruction
	mrs 	r1, PSP       		; Copy PSP to r1, since cmp uses general regs
	cmp	r1, r0        		; Compare
	bne	_cpu_PSP_test_fail

_cpu_PSP_test_pass
	mov 	r13, r4       		; Restore SP 
	msr	CONTROL, r3   		; restore CONTROL
        isb                   		; ensure change is applied
        
; Indicate in the CPUregTestBIST_struct that r13(PSP) test passed
; indicated by 0x1
	movs	r0, #0x01
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1]
; Indicate SP test has passed 
        movs    r0, #0x3
	lsls	r0, r0, #12 		; flags #0x6000
        ldr     r2, [r1, #testPassed]
        orrs    r0, r0, r2
        str     r0, [r1,#testPassed]		
        b	_cpu_PSP_test_return
	
_cpu_PSP_test_fail
	mov 	r13, r4       ;	Restore SP 
	msr	CONTROL, r3   ; restore CONTROL
        isb                   ; ensure change is applied
        
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1]
_cpu_PSP_test_return        
        pop     {r0-r7,PC} ; Restore R0 to R7 and jump back, SP is updated

;------------------------------------------
; HIGH REGISTERS: Stack Pointer r13 (PSP) Test End 
;------------------------------------------

;------------------------------------------
; SPECIAL REGISTERS: Link register r14 (LR)
; The link register will be written with pattern1, then compared for a 
; pass/fail. Then the inverse of the pattern will be tested.
;------------------------------------------
_CPUregTestSPEC
        push    {r0-r7,r14} 		; CortexM0 supports push on R0-R7, R14 (LR)
	mov     r3, r14       		; Store the current link register 

        ldr     r0,=pattern1  			; Load pattern1 in r0
	mov     r14, r0       			; Move pattern1 in the LR
	cmp     r14, r0       			; compare
	bne     _cpu_LR_test_fail_res

	ldr     r0,=pattern3  			; Load pattern3 in r0
	mov     r14, r0       			; Move pattern3 in the LR
	cmp     r14, r0       			; Compare
	bne     _cpu_LR_test_fail_res

_cpu_LR_test_pass
	mov 	r14, r3       			; Restore the LR
        b	_cpu_APSR_test

_cpu_LR_test_fail_res
        mov 	r14, r3       			; Restore the LR
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1] 
        pop    {r0-r7,PC} 			; restore and jump back	
		
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
	cmp     r1, r0        		; Compare the pattern with the input pattern 
	bne     _cpu_APSR_test_fail

	ldr     r0,=pattern6  		; Load pattern6 in r0 
	msr     APSR, r0      		; Move pattern6 in the APSR
	mrs     r1, APSR      		; Read pattern6 from the APSR to r1
								; (since cmp uses general regs)
	cmp     r1, r0        		; Compare the pattern with the input pattern 
	bne     _cpu_APSR_test_fail

_cpu_APSR_test_pass
	msr 	APSR, r3      		; Restore the APSR 
        b       _cpu_PRIMASK_test	; Continiue with the other test
        
_cpu_APSR_test_fail
	msr 	APSR, r3      		; Restore the APSR 
	
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
	movs	r0, #0x00
	ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1] 
        pop    {r0-r7,PC} 			; Restore and jump back

;------------------------------------------
; SPECIAL REGISTERS: APSR register Test End 
;------------------------------------------

;------------------------------------------
; SPECIAL REGISTERS: PRIMASK register
; The PRIMASK bit 0 will be tested
; Only PRIMASK[0] is readable and writeable, it will be compared for a pass/fail.
;------------------------------------------
_cpu_PRIMASK_test

	mrs     r3, PRIMASK   			; Store the current PRIMASK 

	movs    r0, #0        			; Load zero pattern in r0 */
	msr     PRIMASK, r0   			; Clear PRIMASK bit 
	mrs     r1, PRIMASK   			; Read back from the PRIMASK 
	cmp     r1, r0        			; Compare
	bne     _cpu_PRIMASK_test_fail

	movs    r0, #1        			; Load 0x1 pattern in r0 
	msr     PRIMASK, r0   			; Set PRIMASK bit 
	mrs     r1, PRIMASK   			; Read back from the PRIMASK 
	cmp     r1, r0        			; Compare the pattern with the input pattern
	bne     _cpu_PRIMASK_test_fail
        
; Indicate in the CPUregTestBIST_struct that test passed
; indicated by 0x1
        movs    r0, #0x01
        ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1] 
		
; Indicate SPEC test has passed 
        movs    r0, #0x3
	lsls	r0, r0, #15 			; flags #0x3800
        ldr     r2, [r1, #testPassed]
        orrs    r0, r0, r2
        str     r0, [r1, #testPassed]		

        b       _cpu_PRIMASK_test_end
        
_cpu_PRIMASK_test_fail
; Indicate in the CPUregTestBIST_struct that test failed
; indicated by 0x0
        movs    r0, #0x00
        ldr     r1, =CPUregTestBIST_struct+testState
        str     r0, [r1] 

_cpu_PRIMASK_test_end
        msr     PRIMASK, r3   	; Restore the PRIMASK 
        pop     {r0-r7,PC} 	; Restore R0 to R7 and jump back, SP is updated

;------------------------------------------
; SPECIAL REGISTERS: PRIMASK register Test End 
;------------------------------------------
;------------------------------------------
; End of the CPU BIST register test 
;------------------------------------------

  END
  