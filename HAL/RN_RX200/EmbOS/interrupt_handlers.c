/***********************************************************************/
/*                                                                     */
/*  FILE        :interrupt_handlers.c                                  */
/*  DATE        :Thu, Sep 08, 2011                                     */
/*  DESCRIPTION :Interrupt Handler                                     */
/*  CPU TYPE    :RX200                                                 */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
                



#include "interrupt_handlers.h"
#include "OS_Config.h"
#include "OCI2C_drv.h"
#include "OCUART_drv.h"
#include "OCLVD_drv.h"
#include "OCADC_drv.h"
#include "OCIRQ_drv.h"
#include "OCTPM_drv.h"

#include "ISR_TMR.h"
#include "LIB_Error.h"

#define	 USER_EXCEP_SUPERVISOR_ADDR	(FLASH_BASE_ADDRESS + (20 * 4) )
#define	 USER_EXCEP_UND_INST_ADDR	(FLASH_BASE_ADDRESS + (23 * 4) )
#define	 USER_EXCEP_NMI_ADDR		(FLASH_BASE_ADDRESS + (30 * 4) )

typedef void (*fp) (void);


/* Exception(Supervisor Instruction)*/
void INT_Excep_SuperVisorInst(void){
#if 0
	fp f;
	uint32_t *p;

	p = (uint32_t *)USER_EXCEP_SUPERVISOR_ADDR;
	f = (fp)(*p);
	f();
#endif
	LIB_Error_Fatal_Handler(PLT_ERROR_EXCEPT_SVR_INSTR);
}
/* Exception(Undefined Instruction)*/
void INT_Excep_UndefinedInst(void){
#if 0
	fp f;
	uint32_t *p;

	p = (uint32_t *)USER_EXCEP_UND_INST_ADDR;
	f = (fp)(*p);
	f();
#endif
	LIB_Error_Fatal_Handler(PLT_ERROR_EXCEPT_UNDEF_INSTR);
}
/*
Exception(Floating Point)
void INT_Excep_FloatingPoint(void){ brk(); }
*/
/* NMI*/
void INT_NonMaskableInterrupt(void){
#if 0
		fp f;
		uint32_t *p;

		p = (uint32_t *)USER_EXCEP_NMI_ADDR;
		f = (fp)(*p);
		f();
#endif
		LIB_Error_Fatal_Handler(PLT_ERROR_EXCEPT_NMI);
}
/*Dummy*/
void DummyFunc(void){/* brk(); */}

/* BRK*/
/*void INT_Excep_BRK(void){ wait(); }*/


/* IRQ0*/
void INT_Excep_IRQ0(void){
	OS_EnterInterrupt();
	IRQ_ISR();	
	OS_LeaveInterrupt();
}

/* IRQ1*/
void INT_Excep_IRQ1(void){
	OS_EnterInterrupt();
	IRQ_ISR();
	OS_LeaveInterrupt();
}


/* IRQ5*/
void INT_Excep_IRQ5(void){ 
	OS_EnterInterrupt();
	IRQ_ISR();
	OS_LeaveInterrupt();
}

/* IRQ6*/
void INT_Excep_IRQ6(void){ 
	OS_EnterInterrupt();
	IRQ_ISR();
	OS_LeaveInterrupt();
}

/* LVD1*/
void INT_Excep_LVD1(void)
{
	OCLVDW_ISR();
}

/* LVD2*/
void INT_Excep_LVD2(void)
{
	OCLVDW_ISR();
}

#ifdef TGRA0_IRQ_SUPPORT
void  INT_Excep_MTU0_TGIA0(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU0_TGIA0_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


#ifdef TGRB0_IRQ_SUPPORT
void  INT_Excep_MTU0_TGIB0(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU0_TGIB0_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif

#ifdef TGRC0_IRQ_SUPPORT
void  INT_Excep_MTU0_TGIC0(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU0_TGIC0_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


void  INT_Excep_MTU0_TGID0(void){  
	if( TMR_GetClearSourceId(HAL_TMR_MTU0) == TCNT_CLEARING_TGRD ){
		HWCNTR_ISR(HW_TMR_ID_0);
	}else{
		HWTMRCH_ISR(TMR_CH_ID_3);
	}
}


#ifdef TCIV0_IRQ_SUPPORT
void  INT_Excep_MTU0_TCIV0(void){
	int i=0;
	OS_EnterInterrupt();

	i |= CNT0_OVF_ExceptionHandler();
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif
void  INT_Excep_MTU0_TGIE0(void){
	HWTMRCH_ISR(TMR_CH_ID_5);
}

void  INT_Excep_MTU0_TGIF0(void){
	HWTMRCH_ISR(TMR_CH_ID_6);
}


#ifdef TGRA1_IRQ_SUPPORT
void  INT_Excep_MTU1_TGIA1(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU1_TGIA1_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


void INT_Excep_MTU1_TGIB1(void){
	if( TMR_GetClearSourceId(HAL_TMR_MTU1) == TCNT_CLEARING_TGRB ){		
		HWCNTR_ISR(HW_TMR_ID_1);
	}else{
		HWTMRCH_ISR(TMR_CH_ID_8);
	}
}

#ifdef TCIV1_IRQ_SUPPORT
void INT_Excep_MTU1_TCIV1(void){
	int i=0;
	OS_EnterInterrupt();

	i |= CNT1_OVF_ExceptionHandler();
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif
void INT_Excep_MTU1_TCIU1(void){ /* MTU1 Underflow*/
	HWTMRCH_ISR(TMR_CH_ID_10);
}


#ifdef TGRA2_IRQ_SUPPORT
void  INT_Excep_MTU2_TGIA2(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU2_TGIA2_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


void INT_Excep_MTU2_TGIB2(void){
	if( TMR_GetClearSourceId(HAL_TMR_MTU2) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_2);
	}else{
		HWTMRCH_ISR(TMR_CH_ID_12);
	}
}

#ifdef TCIV2_IRQ_SUPPORT
void INT_Excep_MTU2_TCIV2(void){
	int i=0;
	OS_EnterInterrupt();

	i |= CNT2_OVF_ExceptionHandler();
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif /*TCIV2_IRQ_SUPPORT */

void INT_Excep_MTU2_TCIU2(void){ /* MTU2 Underflow*/
	HWTMRCH_ISR(TMR_CH_ID_14);
}


#ifdef TGRA3_IRQ_SUPPORT
void  INT_Excep_MTU3_TGIA3(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU3_TGIA3_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


void INT_Excep_MTU3_TGIB3(void){
	if( TMR_GetClearSourceId(HAL_TMR_MTU3) == TCNT_CLEARING_TGRB ){
		HWCNTR_ISR(HW_TMR_ID_3);
	}else{
		HWTMRCH_ISR(TMR_CH_ID_16);
	}
}


#ifdef TGRC3_IRQ_SUPPORT
void  INT_Excep_MTU3_TGIC3(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU3_TGIC3_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


#ifdef TGRD3_IRQ_SUPPORT
void  INT_Excep_MTU3_TGID3(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU3_TGID3_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


#ifdef TCIV3_IRQ_SUPPORT
void INT_Excep_MTU3_TCIV3(void){
	int i=0;
	OS_EnterInterrupt();

	i |= CNT3_OVF_ExceptionHandler();
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif /* TCIV3_IRQ_SUPPORT */

#ifdef TGRA4_IRQ_SUPPORT
void  INT_Excep_MTU4_TGIA4(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU4_TGIA4_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif


#ifdef TGRB4_IRQ_SUPPORT
void  INT_Excep_MTU4_TGIB4(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU4_TGIB4_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif



#ifdef TGRC4_IRQ_SUPPORT
void  INT_Excep_MTU4_TGIC4(void){
	int i=0;
	OS_EnterInterrupt();

	i |= MTU4_TGIC4_ExceptionHandler(NULL, 0);
	if(!i)
	{
		while(1);
	}
	OS_LeaveInterrupt();
}
#endif


void INT_Excep_MTU4_TGID4(void){
	if( TMR_GetClearSourceId(HAL_TMR_MTU4) == TCNT_CLEARING_TGRD ){
		HWCNTR_ISR(HW_TMR_ID_4);
	}else{
		HWTMRCH_ISR(TMR_CH_ID_23);
	}
}

#ifdef TCIV4_IRQ_SUPPORT
void INT_Excep_MTU4_TCIV4(void){
	int i=0;
	OS_EnterInterrupt();

	i |= CNT4_OVF_ExceptionHandler();
	if(!i)
	{
		while(1);
	}

	OS_LeaveInterrupt();
}
#endif /* TCIV4_IRQ_SUPPORT */

void INT_Excep_MTU5_TGIU5(void){
	HWTMRCH_ISR(TMR_CH_ID_25);
}

void INT_Excep_MTU5_TGIV5(void){
	HWTMRCH_ISR(TMR_CH_ID_26);
}

void INT_Excep_MTU5_TGIW5(void){
	HWTMRCH_ISR(TMR_CH_ID_27);
}
/*-----------------------------------------------------------*/


/* SCI0_RXI0 vector 215*/
#if defined(HAL_UART_SUPPORT)
void INT_Excep_SCI0_RXI0(void)
{
	OCUART_RxDataMngr();
}
#endif

/* SCI1_RXI1*/
#if defined(HAL_UART_SUPPORT)
void INT_Excep_SCI1_RXI1(void){ 
	OCUART_RxDataMngr();
}
#endif

#if defined(HAL_UART_SUPPORT)
/* SCI5_RXI5  vector 223 */
void INT_Excep_SCI5_RXI5(void){ 
	OCUART_RxDataMngr();
}
#endif

#if defined(HAL_UART_SUPPORT)
/* SCI5_TEI5  vector 225 */
void INT_Excep_SCI5_TEI5(void){
	OCUART_TxDataMngr();
}
#endif

#if defined(HAL_UART_SUPPORT)
/* SCI12_RXI12 vector 239 */
void INT_Excep_SCI12_RXI12(void){
	OCUART_RxDataMngr();
}
#endif

#if defined(HAL_UART_SUPPORT)
/* SCI12_TEI12  vector 241 */
void INT_Excep_SCI12_TEI12(void){
	OCUART_TxDataMngr();
}
#endif



#if defined(I2C00_ISR_ENABLE)
/* RIIC0_RXI0*/
void IIC0_RX_ISR(void){
	OS_EnterInterrupt();
	OCI2C_ISR();
	OS_LeaveInterrupt();
}

void IIC0_TX_ISR(void){
	OS_EnterInterrupt();
	OCI2C_ISR();
	OS_LeaveInterrupt();
}

/* RIIC0_TEI0*/
void IIC0_TX_END_ISR(void){
	OS_EnterInterrupt();
	OCI2C_ISR();
	OS_LeaveInterrupt();
}
#endif /*  defined(I2C00_ISR_ENABLE) */

/* vector 102 S12AD0_S12ADI0*/
void INT_Excep_S12AD0_S12ADI0(void){
	OCADC_ISR();
}

#if defined(CONFIG_HAL_SPI)

void INT_Excep_SPEI0(void){
	OS_EnterInterrupt();
	/* SPI_ISR1(); */
	OS_LeaveInterrupt();
}
void INT_Excep_SPRI0(void){
	OS_EnterInterrupt();
	/* SPI_ISR1(); */
	OS_LeaveInterrupt();
}
void INT_Excep_SPTI0(void){
	OS_EnterInterrupt();
	SPI_ISR1();
	OS_LeaveInterrupt();
}
void INT_Excep_SPII0(void){
	OS_EnterInterrupt();
	/* SPI_ISR1(); */
	OS_LeaveInterrupt();
}
#endif /* defined(CONFIG_HAL_SPI) */
