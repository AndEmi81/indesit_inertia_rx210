/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include "ISRcfg.h"


#if defined(EDGE_MODULE_LP1_0258_01) || defined(CASTELLATED_MODULE_LP1_0241_01) || defined(CUSTOM_MODULE)

#elif defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)

#if defined(TMR0_ISR_SUPPORT)
#define TGRA0_IRQ_SUPPORT
#endif
#if defined(TMR1_ISR_SUPPORT)
#define TGRA3_IRQ_SUPPORT
#endif
#if defined(TMR2_ISR_SUPPORT)
#define TGRA2_IRQ_SUPPORT
#endif
#if defined(TMR3_ISR_SUPPORT)
#define TGRC0_IRQ_SUPPORT
#endif
#if defined(TMR4_ISR_SUPPORT)
#define TGRA1_IRQ_SUPPORT
#endif
#if defined(TMR5_ISR_SUPPORT)
#define TGRC3_IRQ_SUPPORT
#endif
#if defined(TMR6_ISR_SUPPORT)
#define TGRC4_IRQ_SUPPORT
#endif
#if defined(TMR7_ISR_SUPPORT)
#define TGRA4_IRQ_SUPPORT
#endif
#if defined(TMR8_ISR_SUPPORT)
#define TGRD3_IRQ_SUPPORT
#endif
#if defined(TMR9_ISR_SUPPORT)
#define TGRB4_IRQ_SUPPORT
#endif

#elif defined(RX110_NOMODULE_CUSTOM1) || defined(RX110_NOMODULE_CUSTOM2)

#if defined(TMR0_ISR_SUPPORT)
	#define TGRA0_IRQ_SUPPORT
#endif

#if defined(TMR1_ISR_SUPPORT)
	#define TGRB0_IRQ_SUPPORT
#endif

#if defined(TMR2_ISR_SUPPORT)
	#define TGRC0_IRQ_SUPPORT
#endif

#if defined(TMR3_ISR_SUPPORT)
	#define TGRD0_IRQ_SUPPORT
#endif

#if defined(TMR4_ISR_SUPPORT)
	#define TGRA1_IRQ_SUPPORT
#endif

#if defined(TMR5_ISR_SUPPORT)
	#define TGRB1_IRQ_SUPPORT
#endif

#if defined(TMR6_ISR_SUPPORT)
	#define TGRA2_IRQ_SUPPORT
#endif

#if defined(TMR7_ISR_SUPPORT)
	#define TGRB2_IRQ_SUPPORT
#endif

#else
#error "MCU MODULE not supported"
#endif



#if defined(TGRA0_IRQ_SUPPORT) || defined(TGRB0_IRQ_SUPPORT) || defined(TGRC0_IRQ_SUPPORT) || defined(TGRD0_IRQ_SUPPORT)
#define TCIV0_IRQ_SUPPORT
#endif

#if defined(TGRA1_IRQ_SUPPORT) || defined(TGRB1_IRQ_SUPPORT)
#define TCIV1_IRQ_SUPPORT
#endif

#if defined(TGRA2_IRQ_SUPPORT) || defined(TGRB2_IRQ_SUPPORT)
#define TCIV2_IRQ_SUPPORT
#endif


int	CNT0_OVF_ExceptionHandler(void);
int	CNT1_OVF_ExceptionHandler(void);
int	CNT2_OVF_ExceptionHandler(void);


int	MTU0_TGIA0_ExceptionHandler(void* pDevice, int status);
int	MTU0_TGIB0_ExceptionHandler(void* pDevice, int status);
int	MTU0_TGIC0_ExceptionHandler(void* pDevice, int status);
int	MTU0_TGID0_ExceptionHandler(void* pDevice, int status);

int	MTU1_TGIA1_ExceptionHandler(void* pDevice, int status);
int	MTU1_TGIB1_ExceptionHandler(void* pDevice, int status);

int	MTU2_TGIA2_ExceptionHandler(void* pDevice, int status);
int MTU2_TGIB2_ExceptionHandler(void* pDevice, int status);


