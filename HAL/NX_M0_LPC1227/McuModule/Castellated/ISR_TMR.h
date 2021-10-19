/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include "ISRcfg.h"
#include "exceptions.h"                                     // for weak attribute
#include "Castellated_IsrInterface.h"

#if defined(TMR0_ISR_SUPPORT)
#define CT32B0CH0_IRQ_SUPPORT
#endif
#if defined(TMR1_ISR_SUPPORT)
#define CT32B0CH1_IRQ_SUPPORT
#endif
#if defined(TMR2_ISR_SUPPORT)
#define CT32B0CH2_IRQ_SUPPORT
#endif
#if defined(TMR3_ISR_SUPPORT)
#define CT32B1CH2_IRQ_SUPPORT
#endif
#if defined(TMR4_ISR_SUPPORT)
#define CT32B1CH1_IRQ_SUPPORT
#endif
#if defined(TMR5_ISR_SUPPORT)
#define CT32B1CH0_IRQ_SUPPORT
#endif
#if defined(TMR6_ISR_SUPPORT)
#define CT16B1CH0_IRQ_SUPPORT
#endif
#if defined(TMR7_ISR_SUPPORT)
#define CT16B1CH1_IRQ_SUPPORT
#endif


extern int CT32B0CH0_IRQHandler(void* pDevice, int status);
extern int CT32B0CH1_IRQHandler(void* pDevice, int status);
extern int CT32B0CH2_IRQHandler(void* pDevice, int status);

extern int CT32B1CH0_IRQHandler(void* pDevice, int status);
extern int CT32B1CH1_IRQHandler(void* pDevice, int status);
extern int CT32B1CH2_IRQHandler(void* pDevice, int status);

extern int CT16B1CH0_IRQHandler(void* pDevice, int status);
extern int CT16B1CH1_IRQHandler(void* pDevice, int status);
