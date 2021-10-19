#include "ISR_TMR.h"


#if defined(TMR0_ISR_SUPPORT)
inline WEAK int TMR0_ISR(void)
{
    return 0;
}

int CT32B0CH0_IRQHandler(void* pDevice, int status)
{
    return TMR0_ISR();
}
#endif

#if defined(TMR1_ISR_SUPPORT)
inline WEAK int TMR1_ISR(void)
{
    return 0;
}

int CT32B0CH1_IRQHandler(void* pDevice, int status)
{
    return TMR1_ISR();
}
#endif

#if defined(TMR2_ISR_SUPPORT)
inline WEAK int TMR2_ISR(void)
{
    return 0;
}

int CT32B0CH2_IRQHandler(void* pDevice, int status)
{
    return TMR2_ISR();
}
#endif

#if defined(TMR3_ISR_SUPPORT)
inline WEAK int TMR3_ISR(void)
{
    return 0;
}

int CT32B1CH2_IRQHandler(void* pDevice, int status)
{
    return TMR3_ISR();
}
#endif


#if defined(TMR4_ISR_SUPPORT)
inline WEAK int TMR4_ISR(void)
{
    return 0;
}

int CT32B1CH1_IRQHandler(void* pDevice, int status)
{
    return TMR4_ISR();
}
#endif


#if defined(TMR5_ISR_SUPPORT)
inline WEAK int TMR5_ISR(void)
{
    return 0;
}

int CT32B1CH0_IRQHandler(void* pDevice, int status)
{
    return TMR5_ISR();
}
#endif


#if defined(TMR6_ISR_SUPPORT)
inline WEAK int TMR6_ISR(void)
{
    return 0;
}

int CT16B1CH0_IRQHandler(void* pDevice, int status)
{
    return TMR6_ISR();
}
#endif


#if defined(TMR7_ISR_SUPPORT)
inline WEAK int TMR7_ISR(void)
{
    return 0;
}

int CT16B1CH1_IRQHandler(void* pDevice, int status)
{
    return TMR7_ISR();
}
#endif