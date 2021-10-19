/***********************************************************************/
/*                                                                     */
/*  FILE        :vector_table.c                                        */
/*  DATE        :Thu, Sep 08, 2011                                     */
/*  DESCRIPTION :Vector Table                                          */
/*  CPU TYPE    :Other                                                 */
/*                                                                     */
/*  This file is generated by KPIT GNU Project Generator.              */
/*                                                                     */
/***********************************************************************/
                          
#include "RTOS.h"
#include "StandardTypes.h"
#include "interrupt_handlers.h"
#include "OS_Config.h"
#include "LIB_Error.h"


typedef void (*fp) (void);
extern void PowerON_Reset (void);
extern void stack (void);
void OS_ISR_Tick  				(void) __attribute__ ((interrupt));



#define REAL_FVECT_SECT          	__attribute__ ((section (".EndianSelect")))
#define FVECT_SECT          		__attribute__ ((section (".fvectors")))

int IllegalException=0;

/*********************************************************************
*
*       _IllegalException()
*
*       Is called from any exception handler which is not implemeted
*       by the application (not overwriiten by user).
*       It's implemented as endless loop, so the programm stops when any
*       of the exceptions is called.
*       Using a debugger and setting a breakpoint here allows to analyze
*       which exception / interrupt was called.
*       Normally, the CPU should not arrive here.
*/
static void _IllegalException(void)
{
#if 0
	int	endless = 1;
	IllegalException = endless;
	do
	{

	}while(IllegalException);
#endif
	LIB_Error_Fatal_Handler(PLT_ERROR_EXCEPT_ILLEGAL);

}



/*********************************************************************
*
*       Default dummy interrupt handler
*
*       All handlers call _IllegalException(), so the program
*       calls this function when any of the exceptions is called.
*       Using a debugger and setting a breakpoint in _IllegelException()
*       allows to analyze which exception / interrupt was called.
*/


/* Exception(Floating Point)*/
__attribute__((weak)) void INT_Excep_FloatingPoint(void)
{
	_IllegalException();
}

/** Hardware vectors remapped to use with Bootloader */

const voidPtr HardwareVectors[] FVECT_SECT  = {
/*;0xffffff80  Reserved*/
#ifdef __RX_BIG_ENDIAN__
    (fp)0xfffffff8,
#else
    (fp)0xffffffff,
#endif
/*;0xffffff84  Reserved*/
    (fp)0xffffffff,
/*;0xffffff88  Reserved*/
    (fp)0xffffffff,
/*;0xffffff8C  Reserved*/
    (fp)0xffffffff,
/*;0xffffff90  Reserved*/
    (fp)0xffffffff,
/*;0xffffff94  Reserved*/
    (fp)0xffffffff,
/*;0xffffff98  Reserved*/
    (fp)0xffffffff,
/*;0xffffff9C  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffAC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffBC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffCC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffd0  Exception(Supervisor Instruction)*/
    INT_Excep_SuperVisorInst,
/*;0xffffffd4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffd8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffdc  Exception(Undefined Instruction)*/
    INT_Excep_UndefinedInst,
/*;0xffffffe0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffe4  Exception(Floating Point)*/
    INT_Excep_FloatingPoint,
/*;0xffffffe8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffec  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff0  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff4  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff8  NMI*/
    INT_NonMaskableInterrupt,
/*;0xfffffffc  RESET*/
/*;<<VECTOR DATA START (POWER ON RESET)>>*/
/*;Power On Reset PC*/
    PowerON_Reset
/*;<<VECTOR DATA END (POWER ON RESET)>>*/
};

/** Hardware vectors on real address - used when Bootloader is missing e.g. on debug*/

const voidPtr RealHardwareVectors[] REAL_FVECT_SECT  = {
/*;0xffffff80  Reserved*/
#ifdef __RX_BIG_ENDIAN__
    (fp)0xfffffff8,
#else
    (fp)0xffffffff,
#endif
/*;0xffffff84  Reserved*/
    (fp)0xffffffff,
/*;0xffffff88  Reserved*/
    (fp)0xffffffff,
/*;0xffffff8C  Reserved*/
    (fp)0xffffffff,
/*;0xffffff90  Reserved*/
    (fp)0xffffffff,
/*;0xffffff94  Reserved*/
    (fp)0xffffffff,
/*;0xffffff98  Reserved*/
    (fp)0xffffffff,
/*;0xffffff9C  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffA8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffAC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffB8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffBC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffC8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffCC  Reserved*/
    (fp)0xffffffff,
/*;0xffffffd0  Exception(Supervisor Instruction)*/
    INT_Excep_SuperVisorInst,
/*;0xffffffd4  Reserved*/
    (fp)0xffffffff,
/*;0xffffffd8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffdc  Exception(Undefined Instruction)*/
    INT_Excep_UndefinedInst,
/*;0xffffffe0  Reserved*/
    (fp)0xffffffff,
/*;0xffffffe4  Exception(Floating Point)*/
    INT_Excep_FloatingPoint,
/*;0xffffffe8  Reserved*/
    (fp)0xffffffff,
/*;0xffffffec  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff0  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff4  Reserved*/
    (fp)0xffffffff,
/*;0xfffffff8  NMI*/
    INT_NonMaskableInterrupt,
/*;0xfffffffc  RESET*/
/*;<<VECTOR DATA START (POWER ON RESET)>>*/
/*;Power On Reset PC*/
    PowerON_Reset
/*;<<VECTOR DATA END (POWER ON RESET)>>*/
};

#define RVECT_SECT          __attribute__ ((section (".rvectors")))


__attribute__((weak)) void INT_Excep_BUSERR(void)		{ 	_IllegalException();}
/*;0x0054  FCU_FCUERR*/
__attribute__((weak)) void INT_Excep_FCU_FCUERR(void)	{ 	_IllegalException();}
/*;0x005C  FRDYI*/
__attribute__((weak)) void INT_Excep_FCU_FRDYI(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SWINT(void)		{	_IllegalException();}
/*;0x0070  CMTU0_CMT0*/
__attribute__((weak)) void INT_Excep_CMTU0_CMT0(void)	{	_IllegalException();}
/*;0x0074  CMTU0_CMT1*/
__attribute__((weak)) void INT_Excep_CMTU0_CMT1(void)	{	_IllegalException();}
/*;0x0078  CMTU1_CMT2*/
__attribute__((weak)) void INT_Excep_CMTU1_CMT2(void)	{	_IllegalException();}
/*;0x007C  CMTU1_CMT3*/
__attribute__((weak)) void INT_Excep_CMTU1_CMT3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_FERRF(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MENDF(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_OVFF(void)			{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SPEI0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SPII0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SPRI0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SPTI0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_DOPCF(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_CMPB0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_CMPB1(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_COUNTUP(void)		{	_IllegalException();}
/*;0x0100  IRQ0*/
__attribute__((weak)) void INT_Excep_IRQ0(void)			{	_IllegalException();}
/*;0x0104 IRQ1*/
__attribute__((weak)) void INT_Excep_IRQ1(void)			{	_IllegalException();}
/*;0x0108 IRQ2*/
__attribute__((weak)) void INT_Excep_IRQ2(void)			{	_IllegalException();}
/*;0x010C IRQ3*/
__attribute__((weak)) void INT_Excep_IRQ3(void)			{	_IllegalException();}
/*;0x0110 IRQ4*/
__attribute__((weak)) void INT_Excep_IRQ4(void)			{	_IllegalException();}
/*;0x0114 IRQ5*/
__attribute__((weak)) void INT_Excep_IRQ5(void)			{	_IllegalException();}
/*;0x0118 IRQ6*/
__attribute__((weak)) void INT_Excep_IRQ6(void)			{	_IllegalException();}
/*;0x011C IRQ7*/
__attribute__((weak)) void INT_Excep_IRQ7(void)			{	_IllegalException();}

__attribute__((weak))  void INT_Excep_ALM(void)			{	_IllegalException();}

__attribute__((weak))  void INT_Excep_PRD(void)			{	_IllegalException();}
/*;0x0160  LVD1/COMPA1*/
__attribute__((weak)) void INT_Excep_LVD1(void)			{	_IllegalException();}
/*;0x0164  LVD2/COMPA2*/
__attribute__((weak)) void INT_Excep_LVD2(void)			{	_IllegalException();}
/*;0x0180  WDT_WOVI*/
__attribute__((weak)) void INT_Excep_WDT_WOVI(void)		{	_IllegalException();}
/*;0x0188  AD0_ADI0*/
__attribute__((weak)) void INT_Excep_AD0_ADI0(void)		{	_IllegalException();}
/*;0x018C  AD1_ADI1*/
__attribute__((weak)) void INT_Excep_AD1_ADI1(void)		{	_IllegalException();}
/*;0x0190  AD2_ADI2*/
__attribute__((weak)) void INT_Excep_AD2_ADI2(void)		{	_IllegalException();}
/*;0x0194  AD3_ADI3*/
__attribute__((weak)) void INT_Excep_AD3_ADI3(void)		{	_IllegalException();}
/*;0x0198 S12AD0_S12ADI0*/
__attribute__((weak)) void INT_Excep_S12AD0_S12ADI0(void)	{	_IllegalException();}
/*;0x019C   S12AD0_GBADI*/
__attribute__((weak)) void INT_Excep_S12AD0_GBADI(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_ELSR18I(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_ELSR19I(void)		{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGIA0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGIB0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGIC0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGID0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TCIV0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGIE0(void)	{	_IllegalException();}

__attribute__((weak)) void  INT_Excep_MTU0_TGIF0(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU1_TGIA1(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU1_TGIB1(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU1_TCIV1(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU1_TCIU1(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU2_TGIA2(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU2_TGIB2(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU2_TCIV2(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU2_TCIU2(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU3_TGIA3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU3_TGIB3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU3_TGIC3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU3_TGID3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU3_TCIV3(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU4_TGIA4(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU4_TGIB4(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU4_TGIC4(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU4_TGID4(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU4_TCIV4(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU5_TGIU5(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU5_TGIV5(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_OEI1(void)			{	_IllegalException();}

__attribute__((weak)) void INT_Excep_OEI2(void)			{	_IllegalException();}

__attribute__((weak)) void INT_Excep_MTU5_TGIW5(void)	{	_IllegalException();}
/*;0x02BC  TMR0_CMI0B*/
__attribute__((weak)) void INT_Excep_TMR0_CMI0B(void)	{	_IllegalException(); }
/*;0x02C0  TMR0_OV0I*/
__attribute__((weak)) void INT_Excep_TMR0_OV0I(void)	{	_IllegalException(); }
/*;0x02C4  TMR1_CMI1A*/
__attribute__((weak)) void INT_Excep_TMR1_CMI1A(void)	{	_IllegalException(); }
/*;0x02C8  TMR1_CMI1B*/
__attribute__((weak)) void INT_Excep_TMR1_CMI1B(void)	{	_IllegalException();}
/*;0x02CC  TMR1_OV1I*/
__attribute__((weak)) void INT_Excep_TMR1_OV1I(void)	{	_IllegalException();}
/*;0x02D0 TMR2_CMI2A*/
__attribute__((weak)) void INT_Excep_TMR2_CMI2A(void)	{	_IllegalException();}
/*;0x02D4  TMR2_CMI2B*/
__attribute__((weak)) void INT_Excep_TMR2_CMI2B(void)	{	_IllegalException();}
/*;0x02D8  TMR2_OV2I*/
__attribute__((weak)) void INT_Excep_TMR2_OV2I(void)	{	_IllegalException();}
/*;0x02DC  TMR3_CMI3A*/
__attribute__((weak)) void INT_Excep_TMR3_CMI3A(void)	{	_IllegalException();}
/*;0x02E0 TMR3_CMI3B*/
__attribute__((weak)) void INT_Excep_TMR3_CMI3B(void)	{	_IllegalException();}
/*;0x02E4  TMR3_OV3I*/
__attribute__((weak)) void INT_Excep_TMR3_OV3I(void)	{	_IllegalException();}
/*;0x0318  DMAC_DMTEND0*/
__attribute__((weak)) void INT_Excep_DMAC_DMTEND0(void)	{	_IllegalException();}
/*;0x031C  DMAC_DMTEND1*/
__attribute__((weak)) void INT_Excep_DMAC_DMTEND1(void)	{	_IllegalException();}
/*;0x0320  DMAC_DMTEND2*/
__attribute__((weak)) void INT_Excep_DMAC_DMTEND2(void)	{	_IllegalException();}
/*;0x0324  DMAC_DMTEND3*/
__attribute__((weak)) void INT_Excep_DMAC_DMTEND3(void)	{	_IllegalException();}
/*;0x0358  SCI0_ERI0*/
__attribute__((weak)) void INT_Excep_SCI0_ERI0(void)	{	_IllegalException();}
/*;0x035C  SCI0_RXI0*/
__attribute__((weak)) void INT_Excep_SCI0_RXI0(void)	{	_IllegalException();}
/*;0x0360  SCI0_TXI0*/
__attribute__((weak)) void INT_Excep_SCI0_TXI0(void)	{	_IllegalException();}
/*;0x0364  SCI0_TEI0*/
__attribute__((weak)) void INT_Excep_SCI0_TEI0(void)	{	_IllegalException();}
/*;0x0368  SCI1_ERI1*/
__attribute__((weak)) void INT_Excep_SCI1_ERI1(void)	{	_IllegalException();}
/*;0x036C  SCI1_RXI1*/
__attribute__((weak)) void INT_Excep_SCI1_RXI1(void)	{	_IllegalException();}
/*;0x0370  SCI1_TXI1*/
__attribute__((weak)) void INT_Excep_SCI1_TXI1(void)	{	_IllegalException();}
/*;0x0374  SCI1_TEI1*/
__attribute__((weak)) void INT_Excep_SCI1_TEI1(void)	{	_IllegalException();}
/*;0x0378  SCI2_ERI2*/
__attribute__((weak)) void INT_Excep_SCI5_ERI5(void)	{	_IllegalException();}
/*;0x037C  SCI2_RXI2*/
__attribute__((weak)) void INT_Excep_SCI5_RXI5(void)	{	_IllegalException();}
/*;0x0380  SCI2_TXI2*/
__attribute__((weak)) void INT_Excep_SCI5_TXI5(void)	{	_IllegalException();}
/*;0x0384  SCI2_TEI2*/
__attribute__((weak)) void INT_Excep_SCI5_TEI5(void)	{	_IllegalException();}
/*;0x0388  SCI3_ERI3*/
__attribute__((weak)) void INT_Excep_SCI6_ERI6(void)	{	_IllegalException();}
/*;0x038C  SCI3_RXI3*/
__attribute__((weak)) void INT_Excep_SCI6_RXI6(void)	{	_IllegalException();}
/*;0x0390  SCI3_TXI3*/
__attribute__((weak)) void INT_Excep_SCI6_TXI6(void)	{	_IllegalException();}
/*;0x0394  SCI3_TEI3*/
__attribute__((weak)) void INT_Excep_SCI6_TEI6(void)	{	_IllegalException();}
/*;0x0398  SCI4_ERI4*/
__attribute__((weak)) void INT_Excep_SCI8_ERI8(void)	{	_IllegalException();}
/*;0x039C  SCI4_RXI4*/
__attribute__((weak)) void INT_Excep_SCI8_RXI8(void)	{	_IllegalException();}
/*;0x03A0  SCI4_TXI4*/
__attribute__((weak)) void INT_Excep_SCI8_TXI8(void)	{	_IllegalException();}
/*;0x03A4  SCI4_TEI4*/
__attribute__((weak)) void INT_Excep_SCI8_TEI8(void)	{	_IllegalException();}
/*;0x03A8  SCI5_ERI5*/
__attribute__((weak)) void INT_Excep_SCI9_ERI9(void)	{	_IllegalException();}
/*;0x03AC  SCI5_RXI5*/
__attribute__((weak)) void INT_Excep_SCI9_RXI9(void)	{	_IllegalException();}
/*;0x03B0  SCI5_TXI5*/
__attribute__((weak)) void INT_Excep_SCI9_TXI9(void)	{	_IllegalException();}
/*;0x03B4  SCI5_TEI5*/
__attribute__((weak)) void INT_Excep_SCI9_TEI9(void)	{	_IllegalException();}
/*;0x03B8  SCI6_ERI6*/
__attribute__((weak)) void INT_Excep_SCI12_ERI12(void)	{	_IllegalException();}
/*;0x03BC  SCI6_RXI6*/
__attribute__((weak)) void INT_Excep_SCI12_RXI12(void)	{	_IllegalException();}
/*;0x03C0  SCI6_TXI6*/
__attribute__((weak)) void INT_Excep_SCI12_TXI12(void)	{	_IllegalException();}
/*;0x03C4  SCI6_TEI6*/
__attribute__((weak)) void INT_Excep_SCI12_TEI12(void)	{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SCIX0(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SCIX1(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SCIX2(void)		{	_IllegalException();}

__attribute__((weak)) void INT_Excep_SCIX3(void)		{	_IllegalException();}

/*;0x03D8  RIIC0_EEI0*/
__attribute__((weak))  void IIC0_Error_ISR(void)		{	_IllegalException();}
/*;0x03DC  RIIC0_RXI0*/
__attribute__((weak))  void IIC0_RX_ISR(void)			{	_IllegalException();}
/*;0x03E0  RIIC0_TXI0*/
__attribute__((weak))  void IIC0_TX_ISR(void)			{	_IllegalException();}
/*;0x03E4  RIIC0_TEI0*/
__attribute__((weak))  void IIC0_TX_END_ISR(void)		{	_IllegalException();}




const fp RelocatableVectors[] RVECT_SECT  = {
/*;0x0000  Reserved*/
    (fp)0xffffffff,
/*;0x0004  Reserved*/
    (fp)0xffffffff,
/*;0x0008  Reserved*/
    (fp)0xffffffff,
/*;0x000C  Reserved*/
    (fp)0xffffffff,
/*;0x0010  Reserved*/
    (fp)0xffffffff,
/*;0x0014  Reserved*/
    (fp)0xffffffff,
/*;0x0018  Reserved*/
    (fp)0xffffffff,
/*;0x001C  Reserved*/
    (fp)0xffffffff,
/*;0x0020  Reserved*/
    (fp)0xffffffff,
/*;0x0024  Reserved*/
    (fp)0xffffffff,
/*;0x0028  Reserved*/
    (fp)0xffffffff,
/*;0x002C  Reserved*/
    (fp)0xffffffff,
/*;0x0030  Reserved*/
    (fp)0xffffffff,
/*;0x0034  Reserved*/
    (fp)0xffffffff,
/*;0x0038  Reserved*/
    (fp)0xffffffff,
/*;0x003C  Reserved*/
    (fp)0xffffffff,
/*;0x0040  BUSERR*/
	(fp)INT_Excep_BUSERR,
/*;0x0044  Reserved*/
    (fp)0xffffffff,
/*;0x0048  Reserved*/
    (fp)0xffffffff,
/*;0x004C  Reserved*/
    (fp)0xffffffff,
/*;0x0050  Reserved*/
    (fp)0xffffffff,
/*;0x0054 FCUERR*/
	(fp)INT_Excep_FCU_FCUERR,
/*;0x0058  Reserved*/
    (fp)0xffffffff,
/*;0x005C  FRDYI*/
	(fp)INT_Excep_FCU_FRDYI,
/*;0x0060  Reserved*/
    (fp)0xffffffff,
/*;0x0064  Reserved*/
    (fp)0xffffffff,
/*;0x0068  Reserved*/
    (fp)0xffffffff,
/*;0x006C  Reserved*/
    (fp)INT_Excep_SWINT,
/*;0x0070  CMTU0_CMT0*/
	(fp)INT_Excep_CMTU0_CMT0,/*NA*/
/*;0x0074  CMTU0_CMT1*/
	(fp)INT_Excep_CMTU0_CMT1,/*NA*/
/*;0x0078  CMTU1_CMT2*/
	(fp)INT_Excep_CMTU1_CMT2,/*NA*/
/*;0x007C  CMTU1_CMT3*/
	(fp)INT_Excep_CMTU1_CMT3,/*NA*/
/*;0x0080  Reserved*/
    (fp)INT_Excep_FERRF,/*	???*/
/*;0x0084  Reserved*/
    (fp)INT_Excep_MENDF,/*	???*/
/*;0x0088  Reserved*/
    (fp)INT_Excep_OVFF,	/*???*/
/*;0x008C  Reserved*/
    (fp)0xffffffff,
/*;0x0090  Reserved*/
    (fp)0xffffffff,
/*;0x0094  Reserved*/
    (fp)0xffffffff,
/*;0x0098  Reserved*/
    (fp)0xffffffff,
/*;0x009C  Reserved*/
    (fp)0xffffffff,
/*;0x00A0  Reserved*/
    (fp)0xffffffff,
/*;0x00A4  Reserved*/
    (fp)0xffffffff,
/*;0x00A8  Reserved*/
    (fp)0xffffffff,
/*;0x00AC  Reserved*/
    (fp)0xffffffff,
/*;0x00B0  Reserved*/
    (fp)INT_Excep_SPEI0,
/*;0x00B4  Reserved*/
    (fp)INT_Excep_SPRI0,
/*;0x00B8  Reserved*/
    (fp)INT_Excep_SPTI0,
/*;0x00BC  Reserved*/
    (fp)INT_Excep_SPII0,
/*;0x00C0  Reserved*/
    (fp)0xffffffff,
/*;0x00C4  Reserved*/
    (fp)0xffffffff,
/*;0x00C8  Reserved*/
    (fp)0xffffffff,
/*;0x00CC  Reserved*/
    (fp)0xffffffff,
/*;0x00D0  Reserved*/
    (fp)0xffffffff,
/*;0x00D4  Reserved*/
    (fp)0xffffffff,
/*;0x00D8  Reserved*/
    (fp)0xffffffff,
/*;0x00DC  Reserved*/
    (fp)0xffffffff,
/*;0x00E0  Reserved*/
    (fp)0xffffffff,
/*;0x00E4  Reserved*/
    (fp)INT_Excep_DOPCF,
/*;0x00E8  Reserved*/
    (fp)INT_Excep_CMPB0, /*NA*/
/*;0x00EC  Reserved*/
    (fp)INT_Excep_CMPB1,/*NA*/
/*;0x00F0  Reserved*/
    (fp)0xffffffff,
/*;0x00F4  Reserved*/
    (fp)0xffffffff,
/*;0x00F8  Reserved*/
    (fp)0xffffffff,
/*;0x00FC  Reserved*/
    (fp)INT_Excep_COUNTUP,
/*;0x0100  IRQ0*/
    (fp)INT_Excep_IRQ0,
/*;0x0104 IRQ1*/
    (fp)INT_Excep_IRQ1,
/*;0x0108 IRQ2*/
    (fp)INT_Excep_IRQ2,
/*;0x010C IRQ3*/
    (fp)INT_Excep_IRQ3,	/*NA*/
/*;0x0110 IRQ4*/
    (fp)INT_Excep_IRQ4,
/*;0x0114 IRQ5*/
    (fp)INT_Excep_IRQ5,
/*;0x0118 IRQ6*/
    (fp)INT_Excep_IRQ6,
/*;0x011C IRQ7*/
    (fp)INT_Excep_IRQ7,
/*;0x0120 IRQ8*/
    (fp)0xffffffff,/*NA*/
/*;0x0124 IRQ9*/
    (fp)0xffffffff,/*NA*/
/*;0x0128 IRQ10*/
    (fp)0xffffffff,/*NA*/
/*;0x012C IRQ11*/
    (fp)0xffffffff,/*NA*/
/*;0x0130 IRQ12*/
    (fp)0xffffffff,/*NA*/
/*;0x0134 IRQ13*/
    (fp)0xffffffff,/*NA*/
/*;0x0138 IRQ14*/
    (fp)0xffffffff,/*NA*/
/*;0x013C IRQ15*/
    (fp)0xffffffff,/*NA*/
/*;0x0140  Reserved*/
    (fp)0xffffffff,
/*;0x0144  Reserved*/
    (fp)0xffffffff,
/*;0x0148  Reserved*/
    (fp)0xffffffff,
/*;0x014C  Reserved*/
    (fp)0xffffffff,
/*;0x0150  Reserved*/
    (fp)0xffffffff,
/*;0x0154  Reserved*/
    (fp)0xffffffff,
/*;0x0158  Reserved*/
    (fp)0xffffffff,
/*;0x015C  Reserved*/
    (fp)0xffffffff,
/*;0x0160  LVD1/COMPA1*/
    (fp)INT_Excep_LVD1,
/*;0x0164  LVD2/COMPA2*/
    (fp)INT_Excep_LVD2,
/*;0x0168  Reserved*/
    (fp)0xffffffff,
/*;0x016C  Reserved*/
    (fp)0xffffffff,
/*;0x0170  Reserved*/
    (fp)INT_Excep_ALM,
/*;0x0174  Reserved*/
    (fp)INT_Excep_PRD,
/*;0x0178  Reserved*/
    (fp)0xffffffff,
/*;0x017C  Reserved*/
    (fp)0xffffffff,
/*;0x0180  WDT_WOVI*/
    (fp)INT_Excep_WDT_WOVI,/*NA*/
/*;0x0184  Reserved*/
    (fp)0xffffffff,
/*;0x0188  Reserved*/
    (fp)0xffffffff,
/*;0x018C  Reserved*/
    (fp)0xffffffff,
/*;0x0190  Reserved*/
    (fp)0xffffffff,
/*;0x0194  Reserved*/
    (fp)0xffffffff,
/*;0x0198 S12AD0_S12ADI0*/
    (fp)INT_Excep_S12AD0_S12ADI0,
/*;0x019C   S12AD0_GBADI	*/
    (fp)INT_Excep_S12AD0_GBADI,
/*;0x01A0  NA*/
	(fp)0xffffffff,
/*;0x01A4  NA*/
    (fp)0xffffffff,
/*;0x01A8  ELSR18I*/
    (fp)INT_Excep_ELSR18I,
/*;0x01AC  ELSR19I*/
    (fp)INT_Excep_ELSR19I,/*NA*/
/*;0x01B0  NA*/
    (fp)0xffffffff,
/*;0x01B4  Reserved*/
    (fp)0xffffffff,
/*;0x01B8  Reserved*/
    (fp)0xffffffff,
/*;0x01BC  NA*/
	(fp)0xffffffff,
/*;0x01C0  NA*/
    (fp)0xffffffff,
/*;0x01C4  Reserved*/
    (fp)0xffffffff,
/* MTU0 -----------------------------*/
/*;0x01C8  114*/
    (fp)INT_Excep_MTU0_TGIA0,
/*;0x01CC  115*/
    (fp)INT_Excep_MTU0_TGIB0,
/*;0x01D0  116*/
    (fp)INT_Excep_MTU0_TGIC0,
/*;0x01D4  117*/
	(fp)INT_Excep_MTU0_TGID0,
/*;0x01D8  118*/
    (fp)INT_Excep_MTU0_TCIV0,
/*;0x01DC  119*/
    (fp)INT_Excep_MTU0_TGIE0,
/*;0x01E0  120*/
    (fp)INT_Excep_MTU0_TGIF0,
/* MTU1  -----------------------------*/
/*;0x01E4  121*/
	(fp)INT_Excep_MTU1_TGIA1,
/*;0x01E8  122*/
	(fp)INT_Excep_MTU1_TGIB1,
/*;0x01EC  123*/
	(fp)INT_Excep_MTU1_TCIV1,
/*;0x01F0  124*/
	(fp)INT_Excep_MTU1_TCIU1,
/* MTU2  -----------------------------*/
/*;0x01F4  125*/
	(fp)INT_Excep_MTU2_TGIA2,
/*;0x01F8  126*/
	(fp)INT_Excep_MTU2_TGIB2,
/*;0x01FC  127*/
	(fp)INT_Excep_MTU2_TCIV2,
/*;0x0200 128 */
	(fp)INT_Excep_MTU2_TCIU2,
/* MTU3  -----------------------------*/
/*;0x0204  129*/
	(fp)INT_Excep_MTU3_TGIA3,
/*;0x0208  130*/
	(fp)INT_Excep_MTU3_TGIB3,
/*;0x020C 131*/
	(fp)INT_Excep_MTU3_TGIC3,
/*;0x0210 132*/
	(fp)INT_Excep_MTU3_TGID3,
/*;0x0214  133*/
	(fp)INT_Excep_MTU3_TCIV3,
/* MTU4  -----------------------------*/
/*;0x0218  134*/
    (fp)INT_Excep_MTU4_TGIA4,
/*;0x021C  135*/
	(fp)INT_Excep_MTU4_TGIB4,
/*;0x0220 136 */
	(fp)INT_Excep_MTU4_TGIC4,
/*;0x0224 137 */
	(fp)INT_Excep_MTU4_TGID4,
/*;0x0228 138 */
	(fp)INT_Excep_MTU4_TCIV4,
/* MTU5  -----------------------------*/
/*;0x022C  139*/
	(fp)INT_Excep_MTU5_TGIU5,
/*;0x0230  140*/
	(fp)INT_Excep_MTU5_TGIV5,
/*;0x0234  141*/
	(fp)INT_Excep_MTU5_TGIW5,
/*------------------------------------------*/
/*;0x0238  NA*/
    (fp)0xffffffff,
/*;0x023C  Reserved*/
    (fp)0xffffffff,
/*;0x0240  Reserved*/
    (fp)0xffffffff,
/*;0x0244  NA*/
    (fp)0xffffffff,
/*;0x0248  NA*/
    (fp)0xffffffff,
/*;0x024C  Reserved*/
    (fp)0xffffffff,
/*;0x0250  Reserved*/
    (fp)0xffffffff,
/*;0x0254  	NA*/
    (fp)0xffffffff,
/*;0x0258  	NA*/
    (fp)0xffffffff,
/*;0x025C  	NA*/
    (fp)0xffffffff,
/*;0x0260*/
    (fp)0xffffffff,
/*;0x0264  Reserved*/
    (fp)0xffffffff,
/*;0x0268  	NA*/
    (fp)0xffffffff,
/*;0x026C  	NA*/
    (fp)0xffffffff,
/*;0x0270  	NA*/
    (fp)0xffffffff,
/*;0x0274  	NA*/
    (fp)0xffffffff,
/*;0x0278  	NA*/
    (fp)0xffffffff,
/*;0x027C  	NA*/
    (fp)0xffffffff,
/*;0x0280  	NA*/
    (fp)0xffffffff,
/*;0x0284  	NA*/
    (fp)0xffffffff,
/*;0x0288  	NA*/
    (fp)0xffffffff,
/*;0x028C  Reserved*/
    (fp)0xffffffff,
/*;0x0290  Reserved*/
    (fp)0xffffffff,
/*;0x0294  	NA*/
    (fp)0xffffffff,
/*;0x0298  	NA*/
    (fp)0xffffffff,
/*;0x029C  	NA*/
    (fp)0xffffffff,
/*;0x02A0  	NA*/
    (fp)0xffffffff,
/*;0x02A4  Reserved*/
    (fp)0xffffffff,
/*;0x02A8  OEI1*/
    (fp)INT_Excep_OEI1,
/*;0x02AC  OEI2*/
    (fp)INT_Excep_OEI2,
/*;0x02B0  Reserved*/
    (fp)0xffffffff,
/*;0x02B4  Reserved*/
    (fp)0xffffffff,
/*;0x02B8  TMR0_CMI0A*/
    (fp)OS_ISR_Tick, /*INT_Excep_TMR0_CMI0A,*/
/*;0x02BC  TMR0_CMI0B*/
    /*(fp)INT_Excep_TMR0_CMI0B,*/
    (fp)_OS_SystemAlarmHandler,
/*;0x02C0  TMR0_OV0I*/
    (fp)INT_Excep_TMR0_OV0I,
/*;0x02C4  TMR1_CMI1A*/
    (fp)INT_Excep_TMR1_CMI1A,
/*;0x02C8  TMR1_CMI1B*/
    (fp)INT_Excep_TMR1_CMI1B,
/*;0x02CC  TMR1_OV1I*/
    (fp)INT_Excep_TMR1_OV1I,
/*;0x02D0 TMR2_CMI2A*/
    (fp)INT_Excep_TMR2_CMI2A,
/*;0x02D4  TMR2_CMI2B*/
    (fp)INT_Excep_TMR2_CMI2B,
/*;0x02D8  TMR2_OV2I*/
    (fp)INT_Excep_TMR2_OV2I,
/*;0x02DC  TMR3_CMI3A*/
    (fp)INT_Excep_TMR3_CMI3A,
/*;0x02E0 TMR3_CMI3B*/
    (fp)INT_Excep_TMR3_CMI3B,
/*;0x02E4  TMR3_OV3I*/
    (fp)INT_Excep_TMR3_OV3I,
/*;0x02E8  Reserved*/
    (fp)0xffffffff,
/*;0x02EC  Reserved*/
    (fp)0xffffffff,
/*;0x02F0  Reserved*/
    (fp)0xffffffff,
/*;0x02F4  Reserved*/
    (fp)0xffffffff,
/*;0x02F8  Reserved*/
    (fp)0xffffffff,
/*;0x02FC  Reserved*/
    (fp)0xffffffff,
/*;0x0300  Reserved*/
    (fp)0xffffffff,
/*;0x0304  Reserved*/
    (fp)0xffffffff,
/*;0x0308  Reserved*/
    (fp)0xffffffff,
/*;0x030C  Reserved*/
    (fp)0xffffffff,
/*;0x0310  Reserved*/
    (fp)0xffffffff,
/*;0x0314  Reserved*/
    (fp)0xffffffff,
/*;0x0318  DMAC_DMTEND0*/
    (fp)INT_Excep_DMAC_DMTEND0,
/*;0x031C  DMAC_DMTEND1*/
    (fp)INT_Excep_DMAC_DMTEND1,
/*;0x0320  DMAC_DMTEND2*/
    (fp)INT_Excep_DMAC_DMTEND2,
/*;0x0324  DMAC_DMTEND3*/
    (fp)INT_Excep_DMAC_DMTEND3,
/*;0x0328  Reserved*/
    (fp)0xffffffff,
/*;0x032C  Reserved*/
    (fp)0xffffffff,
/*;0x0330  Reserved*/
    (fp)0xffffffff,
/*;0x0334  Reserved*/
    (fp)0xffffffff,
/*;0x0338  Reserved*/
    (fp)0xffffffff,
/*;0x033C  Reserved*/
    (fp)0xffffffff,
/*;0x0340  Reserved*/
    (fp)0xffffffff,
/*;0x0344  Reserved*/
    (fp)0xffffffff,
/*;0x0348  Reserved*/
    (fp)0xffffffff,
/*;0x034C  Reserved*/
    (fp)0xffffffff,
/*;0x0350  Reserved*/
    (fp)0xffffffff,
/*;0x0354  Reserved*/
    (fp)0xffffffff,
/*-------    SCI0*/
/*;0x0358  SCI0_ERI0*/
    (fp)INT_Excep_SCI0_ERI0,	/*NA*/
/*;0x035C  SCI0_RXI0*/
    (fp)INT_Excep_SCI0_RXI0,	/*NA*/
/*;0x0360  SCI0_TXI0*/
    (fp)INT_Excep_SCI0_TXI0,/*NA*/
/*;0x0364  SCI0_TEI0*/
    (fp)INT_Excep_SCI0_TEI0,/*NA*/
    /*-------    SCI1*/
/*;0x0368  SCI1_ERI1*/
    (fp)INT_Excep_SCI1_ERI1,
/*;0x036C  SCI1_RXI1*/
    (fp)INT_Excep_SCI1_RXI1,
/*;0x0370  SCI1_TXI1*/
    (fp)INT_Excep_SCI1_TXI1,
/*;0x0374  SCI1_TEI1*/
    (fp)INT_Excep_SCI1_TEI1,
/*-------    SCI5*/
/*;0x0378  SCI2_ERI2*/
    (fp)INT_Excep_SCI5_ERI5,
/*;0x037C  SCI2_RXI2*/
    (fp)INT_Excep_SCI5_RXI5,
/*;0x0380  SCI2_TXI2*/
    (fp)INT_Excep_SCI5_TXI5,
/*;0x0384  SCI2_TEI2*/
    (fp)INT_Excep_SCI5_TEI5,
    /*-------    SCI6*/
/*;0x0388  SCI3_ERI3*/
    (fp)INT_Excep_SCI6_ERI6,
/*;0x038C  SCI3_RXI3*/
    (fp)INT_Excep_SCI6_RXI6,
/*;0x0390  SCI3_TXI3*/
    (fp)INT_Excep_SCI6_TXI6,
/*;0x0394  SCI3_TEI3*/
    (fp)INT_Excep_SCI6_TEI6,
/*-------    SCI8*/
/*;0x0398  SCI4_ERI4*/
    (fp)INT_Excep_SCI8_ERI8,
/*;0x039C  SCI4_RXI4*/
    (fp)INT_Excep_SCI8_RXI8,
/*;0x03A0  SCI4_TXI4*/
    (fp)INT_Excep_SCI8_TXI8,
/*;0x03A4  SCI4_TEI4*/
    (fp)INT_Excep_SCI8_TEI8,
/*-------    SCI9*/
/*;0x03A8  SCI5_ERI5*/
    (fp)INT_Excep_SCI9_ERI9,
/*;0x03AC  SCI5_RXI5*/
    (fp)INT_Excep_SCI9_RXI9,
/*;0x03B0  SCI5_TXI5*/
    (fp)INT_Excep_SCI9_TXI9,
/*;0x03B4  SCI5_TEI5*/
    (fp)INT_Excep_SCI9_TEI9,
/*-------    SCI12*/
/*;0x03B8  SCI6_ERI6*/
    (fp)INT_Excep_SCI12_ERI12,
/*;0x03BC  SCI6_RXI6*/
    (fp)INT_Excep_SCI12_RXI12,
/*;0x03C0  SCI6_TXI6*/
    (fp)INT_Excep_SCI12_TXI12,
/*;0x03C4  SCI6_TEI6*/
    (fp)INT_Excep_SCI12_TEI12,
/*;0x03C8  Reserved*/
    (fp)INT_Excep_SCIX0,
/*;0x03CC  Reserved*/
    (fp)INT_Excep_SCIX1,
/*;0x03D0  Reserved*/
    (fp)INT_Excep_SCIX2,
/*;0x03D4  Reserved*/
    (fp)INT_Excep_SCIX3,
/*;0x03D8  RIIC0_EEI0*/
    (fp)IIC0_Error_ISR,
/*;0x03DC  RIIC0_RXI0*/
    (fp)IIC0_RX_ISR,
/*;0x03E0  RIIC0_TXI0*/
    (fp)IIC0_TX_ISR,
/*;0x03E4  RIIC0_TEI0*/
    (fp)IIC0_TX_END_ISR,
/*;0x03E8  RIIC1_EEI1*/
    (fp)0xffffffff,
/*;0x03EC  RIIC1_RXI1*/
    (fp)0xffffffff,
/*;0x03F0  RIIC1_TXI1*/
    (fp)0xffffffff,
/*;0x03F4  RIIC1_TEI1*/
    (fp)0xffffffff,
/*;0x03F8  Reserved*/
    (fp)0xffffffff,
/*;0x03FC  Reserved*/
    (fp)0xffffffff,
};


