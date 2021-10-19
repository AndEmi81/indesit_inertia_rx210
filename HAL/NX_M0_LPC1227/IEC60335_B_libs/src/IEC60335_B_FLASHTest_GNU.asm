///****************************************************************************
// *   $Id:: IEC60335.h 7460 2011-06-01 11:53:27Z gerritdewaard               $
// *   Project: NXP Cortex-M0 IEC60335 Class B certified library
// *
// *   Description:
// *     Source file for the IEC60335 Class B FLASH (invariable memory) testing 
// *	    library for the GNU compiler.
// *
// ****************************************************************************
// * Software that is described herein is for illustrative purposes only
// * which provides customers with programming information regarding the
// * products. This software is supplied "AS IS" without any warranties.
// * NXP Semiconductors assumes no responsibility or liability for the
// * use of the software, conveys no license or title under any patent,
// * copyright, or mask work right to the product. NXP Semiconductors
// * reserves the right to make changes in the software without
// * notification. NXP Semiconductors also make no representation or
// * warranty that such application will be suitable for the specified
// * use without further testing or modification.
//****************************************************************************/
.text
.syntax unified
.global _FLASHTestPOST
.global _FLASHTestBIST

.extern  FlashPostTestStatus
.extern  FlashBistTestStatus

.type _FLASHTestPOST, function
.type _FLASHTestBIST, function

.thumb
.thumb_func
  
#include "IEC60335_B_Config.h"

//------------------------------------------------------------------------------
// optional user function to perform specific recovery or signaling actions
// can be overridden by the user / application code
// default implementation just keeps CPU in infinite loop
//------------------------------------------------------------------------------

.weak _flashPostTestFailureHook
.weak _flashBistTestFailureHook
  
//------------------------------------------------------------------------------
// inclusion of register addresses for CRC module and MISR module
// the macros define the register addresses of the hw block
//------------------------------------------------------------------------------
CRC_MODE_ADDR     
CRC_SEED_ADDR
CRC_SUM_ADDR
CRC_WR_DATA_ADDR
FMSSTART_ADDR  
FMSSTOP_ADDR  
FMSW0_ADDR     
FMSW1_ADDR     
FMSW2_ADDR     
FMSW3_ADDR     
FMSTAT_ADDR    
FMSTATCLR_ADDR 

//------------------------------------------------------------------------------
// definition of the hw configurations for the supported CRC types on LPC1227
//------------------------------------------------------------------------------
.equ CCITT_SEED, 0x0000FFFF
.equ CCITT_MODE, 0x00000000

.equ CRC16_SEED, 0x00000000
.equ CRC16_MODE, 0x00000015

.equ CRC32_SEED, 0xFFFFFFFF
.equ CRC32_MODE, 0x00000036

//------------------------------------------------------------------------------
// definition of mode
//------------------------------------------------------------------------------
.equ FLASHTESTPOST, 0xAA
.equ FLASHTESTBIST, 0x55

//------------------------------------------------------------------------------
_FLASHTestBIST:
//------------------------------------------------------------------------------
  push    {r0-r7,r14}                  // CortexM0 supports push on R0-R7, R14 (LR)
  ldr     r0,=FLASHTESTBIST            // Store the mode the test is in R0
  b       _FLASHTest                   // Branch to the actual Flash Test

//------------------------------------------------------------------------------
_FLASHTestPOST:
//------------------------------------------------------------------------------
  ldr   r0,=FLASHTESTPOST              // Store the mode the test is in
  mov   r12, r14                       // backup link register
  
_FLASHTest:
  mov     r11, r0                      // Copy the MODE to R11
// start loading the variable values for all tests
  ldr   r0,=IEC60335_BOTTOM_ROM_POST   // Load start address in r0
  ldr   r1,=IEC60335_TOP_ROM_POST      // Load end address in r1
  ldr   r2,=SELECTED_CRC_TYPE          //
  
//------------------------------------------------------------------------------
// determine which algo needs to be used
// the following are supported: CRC_16, CRC_32, CCITT (on LPC1227)
// MISR_HW on LPC11x (with signature generator)
//------------------------------------------------------------------------------
  ldr   r4, = CRC16_ALGO 
  cmp   r4, r2            
  beq   _crc16
  
  ldr   r4, = CRC32_ALGO 
  cmp   r4, r2            
  beq   _crc32

  ldr   r4, = CCITT_ALGO 
  cmp   r4, r2            
  beq   _ccitt

  ldr   r4, = MISRHW_ALGO 
  cmp   r4, r2            
  beq   _misr_hw

_unsupported:
  b     _crc_failed

//------------------------------------------------------------------------------
// CCITT algorithm
//------------------------------------------------------------------------------
_ccitt:
  bl    _crc_load_regs
  ldr   r3, = CCITT_MODE        // load the mode configuration
  str   r3, [r5]                // program the mode register
  ldr   r3, = CCITT_SEED        // load the seed configuration
  str   r3, [r6]                // program the seed register
  bl    _crc_start              // jump to the computation loop
  b     _crc16_verify           // jump to signature verification
  
//------------------------------------------------------------------------------
// CRC16 algorithm
//------------------------------------------------------------------------------
_crc16:
  bl    _crc_load_regs
  ldr   r3, = CRC16_MODE        // load the mode configuration
  str   r3, [r5]                // program the mode register
  ldr   r3, = CRC16_SEED        // load the seed configuration
  str   r3, [r6]                // program the seed register
  bl    _crc_start              // jump to the computation loop
  b     _crc16_verify           // jump to signature verification
  
//------------------------------------------------------------------------------
// CRC32 algorithm
//------------------------------------------------------------------------------
_crc32:
  bl    _crc_load_regs
  ldr   r3, = CRC32_MODE        // load the mode configuration
  str   r3, [r5]                // program the mode register
  ldr   r3, = CRC32_SEED        // load the seed configuration
  str   r3, [r6]                // program the seed register
  bl    _crc_start              // jump to the computation loop
  b     _crc32_verify           // jump to signature verification

  
//------------------------------------------------------------------------------
// MISR hw algorithm
//------------------------------------------------------------------------------
_misr_hw:
  lsrs  r0, r0, #4                    // align start address to 128 bit
  lsrs  r1, r1, #4                    // align stop address to 128 bit
  
  ldr   r3,=TARGET_FMSSTART           // load address of MISR hw start reg
  
  movs  r6, #0x1                      // prepare a mask for sigdone check
  lsls  r6, r6, #2                    // shift by two
  mov   r7, r6                        // keep in r7 sigdone mask

  ldr   r4,=TARGET_FMSSTOP            // load address of MISR hw stop reg

  ldr   r5,=TARGET_FMSTAT             // keep address of MISR hw stat reg

  movs  r6, #0x1                      // prepare the mask for start
  lsls  r6, r6, #17                   // left shift to bit 17 (start)
  orrs  r6, r1                        // now stop mask is ready (stop addr + bit)
  str   r0, [r3]                      // copy the start address in the hw
  str   r6, [r4]                      // start the peripheral

_loop_label:
  ldr   r6, [r5]                      // read sigdone flag in a loop
  ands  r6, r7                        // mask out the bit
  cmp   r6, r7
  bne  _loop_label                   // wait for flag done

  ldr   r3,=TARGET_FMSTATCLR          // load address of MISR hw stat clear reg
  str   r7, [r3]                      // clear the flag into stat clr reg, done
  
  movs  r4, #0                        // signature computation is finished
  mov   r8, r4                        // initialize counter for checks
  movs  r4, #4

  ldr   r3,= TARGET_FMSW0         // get addr. of first word of computed signature
  ldr   r5,= MISR_SIGNATURE_ADDR  // get addr. of first word of reference signature

_misr_hw_sigcheck:
  ldr   r6, [r3]                // load first word of computed signature
  ldr   r7, [r5]                // load first word of reference signature
  cmp   r6, r7                  // compare the two
  bne  _misr_hw_failed         // if different, test failed
  subs  r4, #1
  cmp   r4, r8                  
  beq   _misr_hw_success        // after 4 words, comparison finished w/o errors
  adds  r3, #4                  // point to the next word
  adds  r5, #4
  b     _misr_hw_sigcheck       // loop

_misr_hw_failed:
  ldr   r4,= _crc_failed
  bx    r4
_misr_hw_success:
  ldr   r4,= _crc_ok
  bx    r4


//------------------------------------------------------------------------------
// common loops for computation routines
//------------------------------------------------------------------------------

 
//------------------------------------------------------------------------------
// CRC algorithm loop
//------------------------------------------------------------------------------
_crc_start:
  mov   r4, r0                      // load the start address into r4
  ldr   r3, = TARGET_WD_DATA_ADDR   // load the data register address into r3

_crc_kernel_loop:
  cmp   r4, r1              // compare with the end address
  bge   _signature_check    // loop finished, check signature
  ldr   r7, [r4]            // load the value from flash
  str   r7, [r3]            // write the value to the CRC engine
  adds  r4, #0x4            // update the pointer by one location
  b     _crc_kernel_loop    //

_signature_check:
  ldr   r3, = TARGET_CRC_SUM_ADDR   // load the crc sum register address into r3
  ldr   r5, = CRC_SIGNATURE_ADDR    // load the address of the signature into r5
  bx    LR                          // return to caller
  
_crc_load_regs:
  ldr   r5,=TARGET_CRC_MODE_ADDR  // load the address for mode reg
  ldr   r6,=TARGET_CRC_SEED_ADDR  // load the address for seed reg
  bx    LR                        // get back


//------------------------------------------------------------------------------
// End of test, verifying results
//------------------------------------------------------------------------------
_crc16_verify:
  ldrh  r6, [r3]                // read the computed signature
  ldrh  r7, [r5]                // load the reference signature
  cmp   r6, r7                  // compare the two
  bne  _crc_failed
  b     _crc_ok
  
_crc32_verify:
  ldr   r6, [r3]                // read the computed signature
  ldr   r7, [r5]                // load the reference signature
  cmp   r6, r7                  // compare the two
  bne  _crc_failed              //

_crc_ok:
// Indicate in the FlashPostTestStatus that the flash test succeeded
// indicated by 0x1
  movs	r0, #0x1
  ldr   r1, =FlashPostTestStatus
  strb  r0, [r1]                // enum type is smallest possible size
  mov   r0, r11                 // Fetch the test MODE
  ldr   r1, =FLASHTESTBIST      //
  cmp   r0, r1                  //
  beq   _return_from_bist       //
  mov   r14, r12                // restore link register
  bx    LR                      // crc ok, return

_return_from_bist:
  pop    {r0-r7,PC} 	        // restore and jump back

_crc_failed:
//------------------------------------------------------------------------------
// Try to write a fault status in a specific RAM location
//------------------------------------------------------------------------------
// Indicate in the FlashPostTestStatus that the CPU reg test failed
// indicated by 0x0
  movs	r0, #0x0
  ldr   r1, =FlashPostTestStatus
  strb  r0, [r1]                  // enum type is smallest possible size
  
  mov   r0,r11                    // Copy the MODE back to R0
  ldr   r1, =FLASHTESTBIST        //
  cmp   r0, r1                    //
  beq   _hook_to_bist_failure     // If MODE is FLASHTESTBIST, hook to BIST hook
  b     _flashPostTestFailureHook // Jump to POST test failure hook

_hook_to_bist_failure:
  b     _flashBistTestFailureHook // Jump to BIST test failure hook
  
  
//------------------------------------------------------------------------------
// Keep the CPU in a loop (default) or call a user hook function if overridden
//------------------------------------------------------------------------------
_flashPostTestFailureHook:
  b _flashPostTestFailureHook  // do nothing by default

_flashBistTestFailureHook:
  b _flashBistTestFailureHook  // do nothing by default

//------------------------------------------------------------------------------
// end of test
//------------------------------------------------------------------------------
.align
.end

