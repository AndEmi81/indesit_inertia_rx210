#include "ISR_TMR.h"



#if defined(TMR0_ISR_SUPPORT)
#pragma overload int TMR0_ISR(void); 
int TMR0_ISR(void)
{
    return 0;
}

int FTM2CH00_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR0_ISR();
}


#endif

#if defined(TMR1_ISR_SUPPORT)

#pragma overload int TMR1_ISR(void);
inline int TMR1_ISR(void)
{
    return 0;
}

int FTM2CH01_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR1_ISR();
}


#endif


#if defined(TMR2_ISR_SUPPORT)

#pragma overload int TMR2_ISR(void);
inline int TMR2_ISR(void)
{
    return 0;
}

int FTM2CH02_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR2_ISR();
}

#endif



#if defined(TMR3_ISR_SUPPORT)

#pragma overload int TMR3_ISR(void);
inline int TMR3_ISR(void)
{
    return 0;
}

int FTM1CH02_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR3_ISR();
}

#endif


#if defined(TMR4_ISR_SUPPORT)

#pragma overload int TMR4_ISR(void);
inline int TMR4_ISR(void)
{
    return 0;
}

int FTM1CH01_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR4_ISR();
}

#endif

#if defined(TMR5_ISR_SUPPORT)

#pragma overload int TMR5_ISR(void);
inline int TMR5_ISR(void)
{
    return 0;
}

int FTM1CH00_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR5_ISR();
}

#endif


#if defined(TMR6_ISR_SUPPORT)

#pragma overload int TMR6_ISR(void);
inline int TMR6_ISR(void)
{
    return 0;
}

int TPM3CH00_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR6_ISR();
}

#endif

#if defined(TMR7_ISR_SUPPORT)

#pragma overload int TMR7_ISR(void);
inline int TMR7_ISR(void)
{
    return 0;
}

int TPM3CH01_IRQHandler(void* pDevice, int status)
{
    (void) pDevice;
    (void) status;
    return TMR7_ISR();
}

#endif

