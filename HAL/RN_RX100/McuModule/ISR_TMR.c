#include "ISR_TMR.h"

#include "HAL_Interface.h"
#include  HAL_CFG
#include  HAL

#include "OCTPM_drv.h"

#if defined(CASTELLATED_MODULE_LP1_0241_01) || defined(EDGE_MODULE_LP1_0258_01) || defined(CUSTOM_MODULE)

#if defined(TMR0_ISR_SUPPORT)
int TMR0_ISR(void);
__attribute__((weak)) int TMR0_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR0_IDX);
		return 1;
	}
    return 0;
}


int MTU0_TGIC0_ExceptionHandler(void* pDevice, int status)
{
    return TMR0_ISR();
}
#endif

#if defined(TMR1_ISR_SUPPORT)
int TMR1_ISR(void);
__attribute__((weak)) int TMR1_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_2);
		return 1;
	}else{
		HWTMRCH_ISR(TMR1_IDX);
		return 1;
	}
    return 0;
}

int MTU2_TGIA2_ExceptionHandler(void* pDevice, int status)
{
    return TMR1_ISR();
}
#endif

#if defined(TMR2_ISR_SUPPORT)
int TMR2_ISR(void);
__attribute__((weak)) int TMR2_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR2_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGIA3_ExceptionHandler(void* pDevice, int status)
{
    return TMR2_ISR();
}
#endif

#if defined(TMR3_ISR_SUPPORT)
int TMR3_ISR(void);
__attribute__((weak)) int TMR3_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR3_IDX);
		return 1;
	}
}

int MTU0_TGIA0_ExceptionHandler(void* pDevice, int status)
{
    return TMR3_ISR();
}
#endif

#if defined(TMR4_ISR_SUPPORT)
int TMR4_ISR(void);
__attribute__((weak)) int TMR4_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_1);
		return 1;
	}else{
		HWTMRCH_ISR(TMR4_IDX);
		return 1;
	}
    return 0;
}

int MTU1_TGIA1_ExceptionHandler(void* pDevice, int status)
{
    return TMR4_ISR();
}
#endif

#if defined(TMR5_ISR_SUPPORT)
int TMR5_ISR(void);
__attribute__((weak)) int TMR5_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR5_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGIC3_ExceptionHandler(void* pDevice, int status)
{
    return TMR5_ISR();
}
#endif

#if defined(TMR6_ISR_SUPPORT)
int TMR6_ISR(void);
__attribute__((weak)) int TMR6_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR6_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIC4_ExceptionHandler(void* pDevice, int status)
{
    return TMR6_ISR();
}
#endif

#if defined(TMR7_ISR_SUPPORT)
int TMR7_ISR(void);
__attribute__((weak)) int TMR7_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR7_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIA4_ExceptionHandler(void* pDevice, int status)
{
    return TMR7_ISR();
}
#endif


#if defined(TMR8_ISR_SUPPORT)
int TMR8_ISR(void);
__attribute__((weak)) int TMR8_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRD ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR8_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGID3_ExceptionHandler(void* pDevice, int status)
{
    return TMR8_ISR();
}
#endif


#if defined(TMR9_ISR_SUPPORT)
int TMR9_ISR(void);
__attribute__((weak)) int TMR9_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR9_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIB4_ExceptionHandler(void* pDevice, int status)
{
    return TMR9_ISR();
}
#endif


#if defined(TMR10_ISR_SUPPORT)
int TMR10_ISR(void);
__attribute__((weak)) int TMR10_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR10_IDX);
		return 1;
	}
    return 0;
}

int MTU0_TGIB0_ExceptionHandler(void* pDevice, int status)
{
    return TMR10_ISR();
}
#endif

#elif defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)


#if defined(TMR0_ISR_SUPPORT)
int TMR0_ISR(void);
__attribute__((weak)) int TMR0_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR0_IDX);
		return 1;
	}
    return 0;
}


int MTU0_TGIA0_ExceptionHandler(void* pDevice, int status)
{
    return TMR0_ISR();
}
#endif

#if defined(TMR1_ISR_SUPPORT)
int TMR1_ISR(void);
__attribute__((weak)) int TMR1_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR1_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGIA3_ExceptionHandler(void* pDevice, int status)
{
    return TMR1_ISR();
}
#endif

#if defined(TMR2_ISR_SUPPORT)
int TMR2_ISR(void);
__attribute__((weak)) int TMR2_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_2);
		return 1;
	}else{
		HWTMRCH_ISR(TMR2_IDX);
		return 1;
	}
    return 0;
}

int MTU2_TGIA2_ExceptionHandler(void* pDevice, int status)
{
    return TMR2_ISR();
}
#endif

#if defined(TMR3_ISR_SUPPORT)
int TMR3_ISR(void);
__attribute__((weak)) int TMR3_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR3_IDX);
		return 1;
	}
}

int MTU0_TGIC0_ExceptionHandler(void* pDevice, int status)
{
    return TMR3_ISR();
}
#endif

#if defined(TMR4_ISR_SUPPORT)
int TMR4_ISR(void);
__attribute__((weak)) int TMR4_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_1);
		return 1;
	}else{
		HWTMRCH_ISR(TMR4_IDX);
		return 1;
	}
    return 0;
}

int MTU1_TGIA1_ExceptionHandler(void* pDevice, int status)
{
    return TMR4_ISR();
}
#endif

#if defined(TMR5_ISR_SUPPORT)
int TMR5_ISR(void);
__attribute__((weak)) int TMR5_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR5_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGIC3_ExceptionHandler(void* pDevice, int status)
{
    return TMR5_ISR();
}
#endif

#if defined(TMR6_ISR_SUPPORT)
int TMR6_ISR(void);
__attribute__((weak)) int TMR6_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR6_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIC4_ExceptionHandler(void* pDevice, int status)
{
    return TMR6_ISR();
}
#endif

#if defined(TMR7_ISR_SUPPORT)
int TMR7_ISR(void);
__attribute__((weak)) int TMR7_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR7_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIA4_ExceptionHandler(void* pDevice, int status)
{
    return TMR7_ISR();
}
#endif


#if defined(TMR8_ISR_SUPPORT)
int TMR8_ISR(void);
__attribute__((weak)) int TMR8_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRD ){
		HWCNTR_ISR(HW_TMR_ID_3);
		return 1;
	}else{
		HWTMRCH_ISR(TMR8_IDX);
		return 1;
	}
    return 0;
}

int MTU3_TGID3_ExceptionHandler(void* pDevice, int status)
{
    return TMR8_ISR();
}
#endif


#if defined(TMR9_ISR_SUPPORT)
int TMR9_ISR(void);
__attribute__((weak)) int TMR9_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_4);
		return 1;
	}else{
		HWTMRCH_ISR(TMR9_IDX);
		return 1;
	}
    return 0;
}

int MTU4_TGIB4_ExceptionHandler(void* pDevice, int status)
{
    return TMR9_ISR();
}
#endif
#elif defined(RX110_NOMODULE_CUSTOM1) || defined(RX110_NOMODULE_CUSTOM2)

#if defined(TMR0_ISR_SUPPORT)
int TMR0_ISR(void);
__attribute__((weak)) int TMR0_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR0_IDX);
		return 1;
	}
    return 0;
}

int MTU0_TGIA0_ExceptionHandler(void* pDevice, int status)
{
    return TMR0_ISR();
}
#endif

#if defined(TMR1_ISR_SUPPORT)
int TMR1_ISR(void);
__attribute__((weak)) int TMR1_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR1_IDX);
		return 1;
	}
    return 0;
}

int MTU0_TGIB0_ExceptionHandler(void* pDevice, int status)
{
    return TMR1_ISR();
}
#endif

#if defined(TMR2_ISR_SUPPORT)
int TMR2_ISR(void);
__attribute__((weak)) int TMR2_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRC ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR2_IDX);
		return 1;
	}
    return 0;
}

int MTU0_TGIC0_ExceptionHandler(void* pDevice, int status)
{
    return TMR2_ISR();
}
#endif

#if defined(TMR3_ISR_SUPPORT)
int TMR3_ISR(void);
__attribute__((weak)) int TMR3_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRD ){
		HWCNTR_ISR(TMR_CH_ID_0);
		return 1;
	}else{
		HWTMRCH_ISR(TMR3_IDX);
		return 1;
	}
}

int MTU0_TGID0_ExceptionHandler(void* pDevice, int status)
{
    return TMR3_ISR();
}
#endif

#if defined(TMR4_ISR_SUPPORT)
/*MS attenzione in conflitto con il canale usato dagli allarmi per OS */
int TMR4_ISR(void);
__attribute__((weak)) int TMR4_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_1);
		return 1;
	}else{
		HWTMRCH_ISR(TMR4_IDX);
		return 1;
	}
    return 0;
}

int MTU1_TGIA1_ExceptionHandler(void* pDevice, int status)
{
    return TMR4_ISR();
}
#endif

#if defined(TMR5_ISR_SUPPORT)
int TMR5_ISR(void);
__attribute__((weak)) int TMR5_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_1);
		return 1;
	}else{
		HWTMRCH_ISR(TMR5_IDX);
		return 1;
	}
    return 0;
}

int MTU1_TGIB1_ExceptionHandler(void* pDevice, int status)
{
    return TMR5_ISR();
}
#endif

#if defined(TMR6_ISR_SUPPORT)
int TMR6_ISR(void);
__attribute__((weak)) int TMR6_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_TGRA ){
		HWCNTR_ISR(HW_TMR_ID_2);
		return 1;
	}else{
		HWTMRCH_ISR(TMR6_IDX);
		return 1;
	}
    return 0;
}

int MTU2_TGIA2_ExceptionHandler(void* pDevice, int status)
{
    return TMR6_ISR();
}
#endif

#if defined(TMR7_ISR_SUPPORT)
int TMR7_ISR(void);
__attribute__((weak)) int TMR7_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_2);
		return 1;
	}else{
		HWTMRCH_ISR(TMR7_IDX);
		return 1;
	}
    return 0;
}

int MTU2_TGIB2_ExceptionHandler(void* pDevice, int status)
{
    return TMR7_ISR();
}
#endif


#if defined(TCIV0_IRQ_SUPPORT)
int TCIV0_ISR(void);
__attribute__((weak)) int TCIV0_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_OVERFLOW ){
		HWCNTR_ISR(HW_TMR_ID_0);
		return 1;
	}
    return 0;
}

int  CNT0_OVF_ExceptionHandler(void)
{
	return TCIV0_ISR();
}
#endif

#if defined(TCIV1_IRQ_SUPPORT)
int TCIV1_ISR(void);
__attribute__((weak)) int TCIV1_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_OVERFLOW ){
		HWCNTR_ISR(HW_TMR_ID_1);
		return 1;
	}
    return 0;
}

int  CNT1_OVF_ExceptionHandler(void)
{
	return TCIV1_ISR();
}
#endif


#if defined(TCIV2_IRQ_SUPPORT)
int TCIV2_ISR(void);
__attribute__((weak)) int TCIV2_ISR(void)
{
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_OVERFLOW ){
		HWCNTR_ISR(HW_TMR_ID_2);
		return 1;
	}
    return 0;
}

int  CNT2_OVF_ExceptionHandler(void)
{
	return TCIV2_ISR();
}
#endif


#endif
