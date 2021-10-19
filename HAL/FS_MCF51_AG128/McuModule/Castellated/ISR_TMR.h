/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include "ISRcfg.h"
#include "exceptions.h"                                     // for weak attribute
#include "IsrInterface.h"

#if defined(TMR0_ISR_SUPPORT)
#define FTM2CH00_IRQ_SUPPORT
#endif

#if defined(TMR1_ISR_SUPPORT)
#define FTM2CH01_IRQ_SUPPORT
#endif

#if defined(TMR2_ISR_SUPPORT)
#define FTM2CH02_IRQ_SUPPORT
#endif

#if defined(TMR3_ISR_SUPPORT)
#define FTM1CH02_IRQ_SUPPORT
#endif

#if defined(TMR4_ISR_SUPPORT)
#define FTM1CH01_IRQ_SUPPORT
#endif

#if defined(TMR5_ISR_SUPPORT)
#define FTM1CH00_IRQ_SUPPORT
#endif

#if defined(TMR6_ISR_SUPPORT)
#define TPM3CH00_IRQ_SUPPORT
#endif

#if defined(TMR7_ISR_SUPPORT)
#define TMRCH07_IRQ_SUPPORT
#endif



extern int TMR0_ISR(void);
extern int TMR1_ISR(void);
extern int TMR2_ISR(void);
extern int TMR3_ISR(void);
extern int TMR4_ISR(void);
extern int TMR5_ISR(void);
extern int TMR6_ISR(void);
extern int TMR7_ISR(void);

extern int FTM2CH00_IRQHandler(void* pDevice, int status);
extern int FTM2CH01_IRQHandler(void* pDevice, int status);
extern int FTM2CH02_IRQHandler(void* pDevice, int status);
extern int FTM1CH00_IRQHandler(void* pDevice, int status);
extern int FTM1CH01_IRQHandler(void* pDevice, int status);
extern int FTM1CH02_IRQHandler(void* pDevice, int status);
extern int TPM3CH00_IRQHandler(void* pDevice, int status);
extern int TPM3CH01_IRQHandler(void* pDevice, int status);



