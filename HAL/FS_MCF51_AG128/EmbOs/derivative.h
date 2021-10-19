/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include "OS_Config.h"
#include "HAL_Interface.h"
#include HAL_CFG
#include HAL


#define _Stop asm ( mov3q #4,d0; bclr.b d0,SOPT1; stop #0x2000; )
  /*!< Macro to enter stop modes, STOPE bit in SOPT1 register must be set prior to executing this macro */

#define _Wait asm ( mov3q #4,d0; bset.b d0,SOPT1; nop; stop #0x2000; )
  /*!< Macro to enter wait mode */

#define _SetVectorBaseRegister(value)\
  asm { \
    /* VBR: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,ADDRESS=8,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */\
    move value, d0\
    movec d0,VBR\
  }

#if (OS_ISRHANDLERTABLE_REDIRECTION == 0)
#define ISR_REDIRECTION_ADDRESS               (0x00000000)
#else
#define ISR_REDIRECTION_ADDRESS               (0x00800000)
#endif
