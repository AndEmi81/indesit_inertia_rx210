/*
 * File:    exceptions.c
 * Purpose: Generic exception handling for ColdFire processors
 *
 */
#define __DRIVER_H

#include "derivative.h"
#include "exceptions.h"
#include "startcf.h"
#include "rtos.h"
#define REGISTER_ABI __REGABI__


/***********************************************************************/
/*
 *  Set NO_PRINTF to 0 in order the exceptions.c interrupt handler
 *  to output messages to the standard io. 
 * 
 */
#define NO_PRINTF    1

#if NO_PRINTF
#define VECTORDISPLAY(MESSAGE)                    asm { nop; };
#define VECTORDISPLAY2(MESSAGE,MESSAGE2)          asm { nop; };
#define VECTORDISPLAY3(MESSAGE,MESSAGE2,MESSAGE3) asm { nop; };
#else
#include <stdio.h>
#define VECTORDISPLAY(MESSAGE1)                    printf(MESSAGE1);
#define VECTORDISPLAY2(MESSAGE1,MESSAGE2)          printf(MESSAGE1,MESSAGE2);
#define VECTORDISPLAY3(MESSAGE1,MESSAGE2,MESSAGE3) printf(MESSAGE1,MESSAGE2,MESSAGE3);
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned long far _SP_INIT[];

/***********************************************************************/
/*
 * Handling of the TRK ColdFire libs (printf support in Debugger Terminal) 
 * 
 * To enable this support:  
 * - Set CONSOLE_IO_SUPPORT  1 in this file; this will enable 
 *   TrapHandler_printf for the trap #14 exception.
 *
 * - Make sure the file console_io_cf.c is in your project. 
 *
 * - In the debugger make sure that in the Connection "Setup" dialog, 
 *   "Debug Options" property page, the check box 
 *   "Enable Terminal printf support" is set.
 *   
 *
 * 
 */
#define CONSOLE_IO_SUPPORT  0 

#if CONSOLE_IO_SUPPORT
asm __declspec(register_abi) void TrapHandler_printf(void) {
   HALT
   RTE
}
#endif                                                   

/***********************************************************************/
/*
 * This is the handler for all exceptions which are not common to all 
 * ColdFire Chips.  
 *
 * Called by mcf_exception_handler
 * 
 */
void derivative_interrupt(unsigned long vector)
{
   if (vector < 64 || vector > 192) {
      VECTORDISPLAY2("User Defined Vector #%d\n",vector);
   }
}

/***********************************************************************
 *
 * This is the exception handler for all  exceptions common to all 
 * chips ColdFire.  Most exceptions do nothing, but some of the more 
 * important ones are handled to some extent.
 *
 * Called by asm_exception_handler 
 *
 * The ColdFire family of processors has a simplified exception stack
 * frame that looks like the following:
 *
 *              3322222222221111 111111
 *              1098765432109876 5432109876543210
 *           8 +----------------+----------------+
 *             |         Program Counter         |
 *           4 +----------------+----------------+
 *             |FS/Fmt/Vector/FS|      SR        |
 *   SP -->  0 +----------------+----------------+
 *
 * The stack self-aligns to a 4-byte boundary at an exception, with
 * the FS/Fmt/Vector/FS field indicating the size of the adjustment
 * (SP += 0,1,2,3 bytes).
 *             31     28 27      26 25    18 17      16 15                                  0
 *           4 +---------------------------------------+------------------------------------+
 *             | Format | FS[3..2] | Vector | FS[1..0] |                 SR                 |
 *   SP -->  0 +---------------------------------------+------------------------------------+
 */ 
#define MCF5XXX_RD_SF_FORMAT(PTR)   \
   ((*((unsigned short *)(PTR)) >> 12) & 0x00FF)

#define MCF5XXX_RD_SF_VECTOR(PTR)   \
   ((*((unsigned short *)(PTR)) >>  2) & 0x00FF)

#define MCF5XXX_RD_SF_FS(PTR)    \
   ( ((*((unsigned short *)(PTR)) & 0x0C00) >> 8) | (*((unsigned short *)(PTR)) & 0x0003) )

#define MCF5XXX_SF_SR(PTR)    *(((unsigned short *)(PTR))+1)

#define MCF5XXX_SF_PC(PTR)    *((unsigned long *)(PTR)+1)

#define MCF5XXX_EXCEPTFMT     "%s -- PC = %#08X\n"


void mcf_exception_handler(void *framepointer) 
{
   volatile unsigned long exceptionStackFrame = (*(unsigned long *)(framepointer)); 
   volatile unsigned short stackFrameSR       = MCF5XXX_SF_SR(framepointer);  
   volatile unsigned short stackFrameWord     = (*(unsigned short *)(framepointer));  
   volatile unsigned long  stackFrameFormat   = (unsigned long)MCF5XXX_RD_SF_FORMAT(&stackFrameWord);
   volatile unsigned long  stackFrameFS       = (unsigned long)MCF5XXX_RD_SF_FS(&stackFrameWord);
   volatile unsigned long  stackFrameVector   = (unsigned long)MCF5XXX_RD_SF_VECTOR(&stackFrameWord);
   volatile unsigned long  stackFramePC       = MCF5XXX_SF_PC(framepointer);

   switch (stackFrameFormat)
   {
      case 4:
      case 5:
      case 6:
      case 7:
         break;
      default:
         VECTORDISPLAY3(MCF5XXX_EXCEPTFMT,"Illegal stack type", stackFramePC);
         break;
   }

   switch (stackFrameVector)
   {
   case 2:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Access Error", stackFramePC);
      switch (stackFrameFS)
      {
         case 4:
            VECTORDISPLAY("Error on instruction fetch\n");
            break;
         case 8:
            VECTORDISPLAY("Error on operand write\n");
            break;
         case 9:
            VECTORDISPLAY("Attempted write to write-protected space\n");
            break;
         case 12:
            VECTORDISPLAY("Error on operand read\n");
            break;
         default:
            VECTORDISPLAY("Reserved Fault Status Encoding\n");
            break;
      }
      break;
   case 3:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Address Error", stackFramePC);
      switch (stackFrameFS)
      {
         case 4:
            VECTORDISPLAY("Error on instruction fetch\n");
            break;
         case 8:
            VECTORDISPLAY("Error on operand write\n");
            break;
         case 9:
            VECTORDISPLAY("Attempted write to write-protected space\n");
            break;
         case 12:
            VECTORDISPLAY("Error on operand read\n");
            break;
         default:
            VECTORDISPLAY("Reserved Fault Status Encoding\n");
            break;
      }
      break;
   case 4:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Illegal instruction", stackFramePC);
      break;
   case 8:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Privilege violation", stackFramePC);
      break;
   case 9:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Trace Exception", stackFramePC);
      break;
   case 10:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Unimplemented A-Line Instruction", 	stackFramePC);
      break;
   case 11:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Unimplemented F-Line Instruction", 	stackFramePC);
      break;
   case 12:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Debug Interrupt", stackFramePC);
      break;
   case 14:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Format Error", stackFramePC);
      break;
   case 15:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Unitialized Interrupt", stackFramePC);
      break;
   case 24:
      VECTORDISPLAY3(MCF5XXX_EXCEPTFMT, "Spurious Interrupt", stackFramePC);
      break;
   case 25:
   case 26:
   case 27:
   case 28:
   case 29:
   case 30:
   case 31:
      VECTORDISPLAY2("Autovector interrupt level %d\n", stackFrameVector - 24);
      break;
   case 32:
   case 33:
   case 34:
   case 35:
   case 36:
   case 37:
   case 38:
   case 39:
   case 40:
   case 41:
   case 42:
   case 43:
   case 44:
   case 45:
   case 46:
   case 47:
      VECTORDISPLAY2("TRAP #%d\n", stackFrameVector - 32);
      break;
   case 5:
   case 6:
   case 7:
   case 13:
   case 16:
   case 17:
   case 18:
   case 19:
   case 20:
   case 21:
   case 22:
   case 23:
   case 48:
   case 49:
   case 50:
   case 51:
   case 52:
   case 53:
   case 54:
   case 55:
   case 56:
   case 57:
   case 58:
   case 59:
   case 60:
   case 61:
   case 62:
   case 63:
      VECTORDISPLAY2("Reserved: #%d\n", stackFrameVector);
      break;
   default:
      derivative_interrupt(stackFrameVector);
   break;
   }
}

#if REGISTER_ABI
asm __declspec(register_abi)  void asm_exception_handler(void)
{
   link     a6,#0 
   lea     -20(sp), sp
   movem.l d0-d2/a0-a1, (sp)
   lea     24(sp),a0   /* A0 point to exception stack frame on the stack */
   jsr     mcf_exception_handler
   movem.l (sp), d0-d2/a0-a1
   lea     20(sp), sp
   unlk a6
   rte
}
#else
asm void asm_exception_handler(void)
{
   link     a6,#0 
   lea     -20(sp), sp
   movem.l d0-d2/a0-a1, (sp)
   pea     24(sp)   /* push exception frame address */
   jsr     mcf_exception_handler
   movem.l 4(sp), d0-d2/a0-a1
   lea     24(sp), sp
   unlk a6
   rte
}
#endif

typedef void (* vectorTableEntryType)(void);





/* Bootloader */

#pragma define_section STARTUP_SECTION ".redirectedvectors" far_absolute R
#define APPL_STARTUP __declspec (STARTUP_SECTION)
APPL_STARTUP vectorTableEntryType const RedirectedStartup[1]=
{
  (vectorTableEntryType)&_startup,		/* !!! Bootloader jumps here !!! */
};





#if CONSOLE_IO_SUPPORT
vectorTableEntryType vector_printf @(Vtrap14+ISR_REDIRECTION_ADDRESS)     = TrapHandler_printf;
#endif

/*
 *  MCF51AG128 vector table
 *  CF V1 has 114 vector + SP_INIT in the vector table (115 entries)
 */

//__declspec(weak) vectorTableEntryType vector_0   @(INITSP) = (vectorTableEntryType)&_SP_INIT;

__declspec(weak) vectorTableEntryType vector_1   @(INITPC) = (vectorTableEntryType)&_startup;

#if (OS_ISRHANDLERTABLE_REDIRECTION == 0)

__declspec(weak) vectorTableEntryType vector_0   @(INITSP+ISR_REDIRECTION_ADDRESS) = (vectorTableEntryType)&_SP_INIT;
__declspec(weak) vectorTableEntryType vector_2   @(Vaccerr+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_3   @(Vadderr+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_4   @(Viinstr+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_5   @(VReserved5+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_6   @(VReserved6+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_7   @(VReserved7+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_8   @(Vprviol+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_9   @(Vtrace+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_10  @(Vunilaop+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_11  @(Vunilfop+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_12  @(Vdbgi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_13  @(VReserved13+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_14  @(Vferror+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_15  @(VReserved15+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_16  @(VReserved16+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_17  @(VReserved17+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_18  @(VReserved18+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_19  @(VReserved19+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_20  @(VReserved20+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_21  @(VReserved21+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_22  @(VReserved22+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_23  @(VReserved23+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_24  @(Vspuri+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_25  @(VReserved25+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_26  @(VReserved26+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_27  @(VReserved27+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_28  @(VReserved28+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_29  @(VReserved29+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_30  @(VReserved30+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_31  @(VReserved31+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_32  @(Vtrap0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_33  @(Vtrap1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_34  @(Vtrap2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_35  @(Vtrap3+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_36  @(Vtrap4+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_37  @(Vtrap5+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_38  @(Vtrap6+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_39  @(Vtrap7+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_40  @(Vtrap8+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_41  @(Vtrap9+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_42  @(Vtrap10+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_43  @(Vtrap11+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_44  @(Vtrap12+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_45  @(Vtrap13+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
#if CONSOLE_IO_SUPPORT == 0
__declspec(weak) vectorTableEntryType vector_46  @(Vtrap14+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
#endif
__declspec(weak) vectorTableEntryType vector_47  @(Vtrap15+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_48  @(VReserved48+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_49  @(VReserved49+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_50  @(VReserved50+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_51  @(VReserved51+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_52  @(VReserved52+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_53  @(VReserved53+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_54  @(VReserved54+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_55  @(VReserved55+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_56  @(VReserved56+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_57  @(VReserved57+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_58  @(VReserved58+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_59  @(VReserved59+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_60  @(VReserved60+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_61  @(Vunsinstr+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_62  @(VReserved62+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_63  @(VReserved63+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_64  @(Virq+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_65  @(Vlvd+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_66  @(VReserved66+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_67  @(VReserved67+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_68  @(Vdmach0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_69  @(Vdmach1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_70  @(Vdmach2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_71  @(Vdmach3+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_72  @(Vieventch0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_73  @(Vftm1fault_ovf+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_74  @(Vftm1ch0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_75  @(Vftm1ch1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_76  @(Vftm1ch2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_77  @(Vftm1ch3+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_78  @(Vftm1ch4+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_79  @(Vftm1ch5+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_80  @(Vftm2fault_ovf+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_81  @(Vftm2ch0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_82  @(Vftm2ch1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_83  @(Vftm2ch2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_84  @(Vftm2ch3+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_85  @(Vftm2ch4+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_86  @(Vftm2ch5+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_87  @(Vtpm3ovf+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_88  @(Vtpm3ch0+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_89  @(Vtpm3ch1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_90  @(Vadc+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_91  @(Vhscmp1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_92  @(Vhscmp2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_93  @(Vieventch1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_94  @(Vspi1+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_95  @(Vspi2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_96  @(Vsci1err+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_97  @(Vsci1rx+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_98  @(Vsci1tx+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_99  @(Viic+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_100 @(Vieventch2+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_101 @(Vsci2err+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_102 @(Vsci2rx+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_103 @(VL7swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_104 @(VL6swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_105 @(VL5swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_106 @(VL4swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_107 @(VL3swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_108 @(VL2swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_109 @(VL1swi+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_110 @(Vsci2tx+ISR_REDIRECTION_ADDRESS)   = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_111 @(Vportae+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_112 @(Vportfj+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_113 @(Vrtc_wdg+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;
__declspec(weak) vectorTableEntryType vector_114 @(Vieventch3+ISR_REDIRECTION_ADDRESS) = OS_ISR_Handler;

#endif /*(OS_ISRHANDLERTABLE_REDIRECTION == 0)*/

#ifdef __cplusplus
}
#endif
