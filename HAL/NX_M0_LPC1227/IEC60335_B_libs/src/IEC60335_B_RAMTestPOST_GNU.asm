///****************************************************************************
// *   $Id:: IEC60335.h 7460 2011-06-01 11:53:27Z gerritdewaard               $
// *   Project: NXP Cortex-M0 IEC60335 Class B certified library
// *
// *   Description:
// *     Source file for the IEC60335 Class B RAM (variable memory) POST testing 
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
.global  _RAMTestPOST
.extern  Reset_Handler
.extern  RamPostTestStatus
.extern  IEC60335_BOTTOM_RAM_POST_MARCH
.extern  IEC60335_TOP_RAM_POST_MARCH

//------------------------------------------
// Test pattern definition
.equ pattern,	0x55555555
//------------------------------------------

//------------------------------------------
// optional user function to perform specific recovery or signaling actions
// can be overridden by the user / application code
// default implementation just keeps CPU in infinite loop
//------------------------------------------

.weak _ramPostTestFailureHook
  
//------------------------------------------
_RAMTestPOST:
//------------------------------------------
// start loading the variable values for all tests
  ldr   r0, =IEC60335_BOTTOM_RAM_POST_MARCH   // Load start address in r0
  ldr   r1, =IEC60335_TOP_RAM_POST_MARCH      // Load end address in r1
        
//------------------------------------------
_march0_start:
//------------------------------------------
// overwrites the complete memory with the pattern
  mov   r2, r0              // Copy start address in r2 (pointer)
  ldr   r3,=pattern         // Load test pattern in r3
_march0_loop:
  cmp   r2, r1              // check if loop is done
  beq   _march1_even        // if loop is done move to next step
  str   r3, [r2]            // write pattern to memory
  adds  r2, #0x4            // update the pointer by one location
  b     _march0_loop
        
//------------------------------------------
_march1_even:
//------------------------------------------
// prime the loop with relevant variables
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // backup the counter value for read
  mov   r9,r5         // backup the counter value for write

  mov   r2, r0            // reload pointer with start address
  ldr   r3,=pattern//      // load test pattern in r3
  ldr   r6,=_march1_odd   // get the next label for branching
  mov   r10, r6           // backup the next branch within r10
  b     _march_incr_krnl  // call the march increment kernel

//------------------------------------------
_march1_odd:
//------------------------------------------
// prime the loop with relevant variables
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  adds  r2, r0, #0x4      // reload pointer with start address (odd)
  ldr   r3,=pattern//      // load test pattern in r3
  ldr   r6,=_march2_even  // get the next label for branching
  mov   r10, r6           // backup the next branch into r10
  b     _march_incr_krnl  // call the march increment kernel

//------------------------------------------
_march2_even:
//------------------------------------------
// prime the loop with relevant variables
  movs  r4,#0x5       // load read counter (5)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

// check if size is even or odd, then adjust the starting address
  mov   r2, r1        // reload pointer with start address
  subs  r6, r1, r0    // calculate the size from start to end address
                      // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2    // divide by 4 to get number of elements
  movs  r7, #0x1      
  ands  r6, r7        // verify if the size is odd
  bne   _m2e_odd_size // if not equal (1) size is odd
  subs  r2, #0x4      // if size is even, need to skip 1 more location
                      // to get to the even locations
_m2e_odd_size:
  subs  r2, #0x4          // if size is odd, get to the last ram location
  ldr   r3,=pattern//      // load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march2_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl  // call the march decrement kernel

//------------------------------------------
_march2_odd:
//------------------------------------------
  movs  r4,#0x5       // load read counter (5)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  // check if size is even or odd, then calculate the starting address
  mov   r2, r1          // reload pointer with start address
  subs  r6, r1, r0      // calculate the size
                        // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2      // divide by 4 to get number of elements
  movs  r7, #0x1
  ands  r6, r7          // see if the size is odd
  beq   _m2o_even_size  // if equal (0) size is even
  subs  r2, #0x4        // if size is odd, need to sub 1 more location
                        // to get to the odd locations
_m2o_even_size:
  subs  r2, #0x4
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march3_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl  // call the march decrement kernel

//------------------------------------------
_march3_even:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  // check if size is even or odd, then calculate the starting address
  mov   r2, r1        // reload pointer with start address
  subs  r6, r1, r0    // calculate the size
                      // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2    // divide by 4 to get number of elements
  movs  r7, #0x1
  ands  r6, r7        // see if the size is odd
  bne   _m3e_odd_size // if not equal (1) size is odd
  subs  r2, #0x4      // if size is even, need to sub 1 more location
                      // to get to the even location
_m3e_odd_size:
  subs  r2, #0x4
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march3_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl  // call the march decrement kernel

//------------------------------------------
_march3_odd:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  // check if size is even or odd, then calculate the starting address
  mov   r2, r1        // reload pointer with start address
  subs  r6, r1, r0    // calculate the size
                      // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2    // divide by 4 to get number of elements
  movs  r7, #0x1
  ands  r6, r7          // see if the size is odd
  beq   _m3o_even_size  // if equal (0) size is even
  subs  r2, #0x4        // if size is odd, need to sub 1 more location
                        // to get to the odd location
_m3o_even_size:
  subs  r2, #0x4
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march4_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl  // call the march decrement kernel

//------------------------------------------
_march4_even:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  mov   r2, r0            // reload pointer with start address
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march4_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl  // call the march increment kernel

//------------------------------------------
_march4_odd:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  adds  r2, r0, #0x4      // reload pointer with start address (odd)
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march5_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl  // call the march increment kernel

//------------------------------------------
_march5_even:
//------------------------------------------
  movs  r4,#0x5       // load read counter (5)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  mov   r2, r0            // reload pointer with start address
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march5_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl  // call the march increment kernel

//------------------------------------------
_march5_odd:
//------------------------------------------
  movs  r4,#0x5       // load read counter (5)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  adds  r2, r0, #0x4      // reload pointer with start address (odd)
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march6_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl

//------------------------------------------
_march6_even:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x1       // load write counter (1)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  mov   r2, r0            // reload pointer with start address
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march6_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl

//------------------------------------------
_march6_odd:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x1       // load write counter (1)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  adds  r2, r0, #0x4      // reload pointer with start address (odd)
  ldr   r3,=pattern//      // Load test pattern in r3
  ldr   r6,=_march7_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl
        
//------------------------------------------
_march7_even:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  // check if size is even or odd, then calculate the starting address
  mov   r2, r1        // reload pointer with start address
  subs  r6, r1, r0    // calculate the size
                      // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2    // divide by 4 to get number of elements
  movs  r7, #0x1
  ands  r6, r7            // see if the size is odd
  bne   _m7e_odd_size     // if not equal (1) size is odd
  subs  r2, #0x4          // if size is even, need to sub 1 more location
_m7e_odd_size:
  subs  r2, #0x4
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march7_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl
        
//------------------------------------------
_march7_odd:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x0       // load write counter (0)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  // check if size is even or odd, then calculate the starting address
  mov   r2, r1        // reload pointer with start address
  subs  r6, r1, r0    // calculate the size
                      // r1 is the next pointer after end of valid ram
  lsrs  r6, r6, #2    // divide by 4 to get number of elements
  movs  r7, #0x1
  ands  r6, r7          // see if the size is odd
  beq   _m7o_even_size  // if equal (0) size is even
  subs  r2, #0x4        // if size is odd, need to sub 1 more location
_m7o_even_size:
  subs  r2, #0x4
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march8_even  // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_decr_krnl

//------------------------------------------
_march8_even:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x1       // load write counter (1)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  mov   r2, r0            // reload pointer with start address
  ldr   r3,=pattern//      // Load test pattern in r3
  mvns  r3,r3             // negate the pattern
  ldr   r6,=_march8_odd   // store the next label for branching
  mov   r10, r6           // backup the load register into r10
  b     _march_incr_krnl
        
//------------------------------------------
_march8_odd:
//------------------------------------------
  movs  r4,#0x0       // load read counter (0)
  movs  r5,#0x1       // load write counter (1)
  mov   r8,r4         // store the counter value for read
  mov   r9,r5         // store the counter value for write

  adds  r2, r0, #0x4          // reload pointer with start address (odd)
  ldr   r3,=pattern//          // Load test pattern in r3
  mvns  r3,r3                 // negate the pattern
  ldr   r6,=_exitRamTestPost  // store the next label for branching
  mov   r10, r6               // backup the load register into r10
  b     _march_incr_krnl
        
//------------------------------------------
_march_incr_krnl:
//------------------------------------------
  mov   r5, r9                // load the counter values
  mov   r4, r8         
_write_loop_inc:
  cmp   r5, #0x0              // need to perform multiple writes?
  beq   _march_increment_test // if not go to march kernel
  str   r3, [r2]              // write the pattern to the location
  subs  r5, #1                // subtract one
  b     _write_loop_inc
_march_increment_test:
  ldr   r6, [r2]              // read back the value
  cmp   r6, r3                // compare with the pattern
  bne   _march_test_failed    // if not equal, test failed
  mvns  r6,r6
  str   r6, [r2]              // write back the negated pattern
_read_loop_inc:
  cmp   r4,#0x0               // need to perform multiple reads?
  beq   _march_incr_loop
  ldr   r7, [r2]              // load the negated pattern from the location
  cmp   r6, r7                // compare the negated pattern
  bne   _march_test_failed    // if not equal, test failed
  subs  r4, #1                // subtract one
  b     _read_loop_inc
_march_incr_loop:
  adds  r2,#0x8               // update the pointer by two locations
  cmp   r2, r1
  blt   _march_incr_krnl
_march_incr_exit:
  bx    r10                   // branch return value is stored in r10



//------------------------------------------
_march_decr_krnl:
//------------------------------------------
  mov   r5, r9                // load the counter values
  mov   r4, r8         
_write_loop_dec:
  cmp   r5, #0x0              // need to perform multiple writes?
  beq   _march_decrement_test // if not go to march kernel
  str   r3, [r2]              // write the pattern to the location
  subs  r5, #1                // subtract one
  b     _write_loop_dec
_march_decrement_test:
  ldr   r6, [r2]              // read back the value
  cmp   r6, r3                // compare with the pattern
  bne   _march_test_failed    // if not equal, test failed
  mvns  r6, r6                //
  str   r6, [r2]              // write back the negated pattern
_read_loop_dec:
  cmp   r4,#0x0               // need to perform multiple reads?
  beq   _march_decr_loop
  ldr   r7, [r2]              // load the negated pattern from the location
  cmp   r6, r7                // compare the negated pattern
  bne   _march_test_failed    // if not equal, test failed
  subs  r4, #1                // subtract one
  b     _read_loop_dec
_march_decr_loop:
  subs  r2,#0x8               // update the pointer by two locations
  
  cmp   r2, r0
  bge   _march_decr_krnl
_march_decr_exit:
  bx    r10                   // branch return value is stored in r10


//------------------------------------------
// End of the POST RAM test
//------------------------------------------
_exitRamTestPost:
// Indicate in the RamPostTestStatus variable that the ram test succeeded
// indicated by 0x1
  movs	r0, #0x1
  ldr   r1, =RamPostTestStatus
  strb  r0, [r1]		  // enum is smallest possible size
  bx    LR                        // test successfully finished, return
								
                    
_march_test_failed:
//------------------------------------------
// Try to write a fault status in a specific RAM location
//------------------------------------------
// Indicate in the RamPostTestStatus variable that the ram test failed
// indicated by 0x0
  movs	r0, #0x0
  ldr   r1, =RamPostTestStatus
  strb  r0, [r1]			// enum is smallest possible size
  b     _ramPostTestFailureHook


//------------------------------------------
// Keep the CPU in a loop (default) or call a user hook function if overridden
//------------------------------------------
_ramPostTestFailureHook:
  b _ramPostTestFailureHook  // do nothing by default


.end


