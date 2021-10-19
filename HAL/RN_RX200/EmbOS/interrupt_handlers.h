/***********************************************************************/
/*  													               */
/*      PROJECT NAME :  FW_PORTHALRX200                                */
/*      FILE         :  interrupt_handlers.h                           */
/*      DESCRIPTION  :  Interrupt Handler Declarations                 */
/*      CPU SERIES   :  RX200                                          */
/*      CPU TYPE     :  Other                                          */
/*  													               */
/*  													               */
/***********************************************************************/


#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H

/* Exception(Supervisor Instruction)*/
void INT_Excep_SuperVisorInst(void) __attribute__ ((interrupt));

/* Exception(Undefined Instruction)*/
void INT_Excep_UndefinedInst(void) __attribute__ ((interrupt));

/* Exception(Floating Point)*/
void INT_Excep_FloatingPoint(void) __attribute__ ((interrupt));

/* NMI*/
void INT_NonMaskableInterrupt(void) __attribute__ ((interrupt));

/* Dummy*/
void DummyFunc (void) __attribute__ ((interrupt));

/* BRK*/
void INT_Excep_BRK(void) __attribute__ ((interrupt));

/* vector  1 reserved
 vector  2 reserved
 vector  3 reserved
 vector  4 reserved
 vector  5 reserved
 vector  6 reserved
 vector  7 reserved
 vector  8 reserved
 vector  9 reserved
 vector 10 reserved
 vector 11 reserved
 vector 12 reserved
 vector 13 reserved
 vector 14 reserved
 vector 15 reserved
 */
/* BUSERR*/

void INT_Excep_BUSERR(void) __attribute__ ((interrupt));

/* vector 17 reserved
 vector 18 reserved
 vector 19 reserved
 vector 20 reserved
*/
/* FCU_FCUERR*/
void INT_Excep_FCU_FCUERR(void) __attribute__ ((interrupt));

/* vector 22 reserved*/

/* FCU_FRDYI*/
void INT_Excep_FCU_FRDYI(void)		__attribute__ ((interrupt));
void INT_Excep_SWINT(void)			__attribute__ ((interrupt));
/* vector 24 reserved
 vector 25 reserved
 vector 26 reserved
 vector 27 reserved
*/
/* CMTU0_CMT0*/
void INT_Excep_CMTU0_CMT0(void)		__attribute__ ((interrupt));

/* CMTU0_CMT1*/
void INT_Excep_CMTU0_CMT1(void) __attribute__ ((interrupt));

/* CMTU1_CMT2*/
void INT_Excep_CMTU1_CMT2(void) __attribute__ ((interrupt));

/* CMTU1_CMT3*/
void INT_Excep_CMTU1_CMT3(void) __attribute__ ((interrupt));


void INT_Excep_FERRF(void)		__attribute__ ((interrupt));
void INT_Excep_MENDF(void)		__attribute__ ((interrupt));

/*
 vector 32 reserved
 vector 33 reserved
 vector 34 reserved
 vector 35 reserved
 vector 36 reserved
 vector 37 reserved
 vector 38 reserved
 vector 39 reserved
 vector 40 reserved
 vector 41 reserved
 vector 42 reserved
 vector 43 reserved
 vector 44 reserved
 vector 45 reserved
 vector 46 reserved
 vector 47 reserved
 vector 48 reserved
 vector 49 reserved
 vector 50 reserved
 vector 51 reserved
 vector 52 reserved
 vector 53 reserved
 vector 54 reserved
 vector 55 reserved
 vector 56 reserved
 vector 57 reserved
 vector 58 reserved
 vector 59 reserved
 vector 60 reserved
 vector 61 reserved
 vector 62 reserved
 vector 63 reserved
*/
void INT_Excep_OVFF(void)		__attribute__ ((interrupt));
void INT_Excep_SPEI0(void)		__attribute__ ((interrupt));

void INT_Excep_SPII0(void)		__attribute__ ((interrupt));

void INT_Excep_SPRI0(void)		__attribute__ ((interrupt));

void INT_Excep_SPTI0(void)		__attribute__ ((interrupt));

void INT_Excep_DOPCF(void)		__attribute__ ((interrupt));

void INT_Excep_CMPB0(void)		__attribute__ ((interrupt));

void INT_Excep_CMPB1(void)		__attribute__ ((interrupt));

void INT_Excep_COUNTUP(void)	__attribute__ ((interrupt));


/* IRQ0*/

void INT_Excep_IRQ0(void) __attribute__ ((interrupt));

/* IRQ1*/

void INT_Excep_IRQ1(void) __attribute__ ((interrupt));

/* IRQ2*/

void INT_Excep_IRQ2(void) __attribute__ ((interrupt));

/* IRQ3*/

void INT_Excep_IRQ3(void) __attribute__ ((interrupt));

/* IRQ4*/

void INT_Excep_IRQ4(void) __attribute__ ((interrupt));

/* IRQ5*/

void INT_Excep_IRQ5(void) __attribute__ ((interrupt));

/* IRQ6*/

void INT_Excep_IRQ6(void) __attribute__ ((interrupt));

/* IRQ7*/

void INT_Excep_IRQ7(void) __attribute__ ((interrupt));


void INT_Excep_ALM(void) __attribute__ ((interrupt));
/*
 vector 80 reserved
 vector 81 reserved
 vector 82 reserved
 vector 83 reserved
 vector 84 reserved
 vector 85 reserved
 vector 86 reserved
 vector 87 reserved
 vector 88 reserved
*/
void INT_Excep_PRD(void)	__attribute__ ((interrupt));
void INT_Excep_LVD1(void)	__attribute__ ((interrupt));
/* vector 89 reserved*/
void INT_Excep_LVD2(void)	__attribute__ ((interrupt));
/*
 vector 90 reserved
 vector 91 reserved
 vector 92 reserved
 vector 93 reserved
 vector 94 reserved
 vector 95 reserved
*/
/* vector 96 WDT_WOVI*/
void INT_Excep_WDT_WOVI(void) __attribute__ ((interrupt));

/* vector 97 reserved*/

/* vector 98 AD0_ADI0*/
void INT_Excep_AD0_ADI0(void) __attribute__ ((interrupt));

/* vector 99 AD1_ADI1*/
void INT_Excep_AD1_ADI1(void) __attribute__ ((interrupt));

/* vector 100 AD2_ADI2*/
void INT_Excep_AD2_ADI2(void) __attribute__ ((interrupt));

/* vector 101 AD3_ADI3*/
void INT_Excep_AD3_ADI3(void) __attribute__ ((interrupt));

/* vector 102 S12AD0_S12ADI0*/
void INT_Excep_S12AD0_S12ADI0(void) __attribute__ ((interrupt));

/* vector 103 S12AD0_GBADI*/
void INT_Excep_S12AD0_GBADI(void)	__attribute__ ((interrupt));

void INT_Excep_ELSR18I(void)		__attribute__ ((interrupt));

void INT_Excep_ELSR19I(void)		__attribute__ ((interrupt));


/* TPU0_TGI0A*/

void INT_Excep_TPU0_TGI0A(void) __attribute__ ((interrupt));

/* TPU0_TGI0B*/

void INT_Excep_TPU0_TGI0B(void) __attribute__ ((interrupt));

/* TPU0_TGI0C*/

void INT_Excep_TPU0_TGI0C(void) __attribute__ ((interrupt));

/* TPU0_TGI0D*/

void INT_Excep_TPU0_TGI0D(void) __attribute__ ((interrupt));

/* TPU0_TCI0V*/

void INT_Excep_TPU0_TCI0V(void) __attribute__ ((interrupt));

/* vector 109 reserved*/
/* vector 110 reserved*/

/* TPU1_TGI1A*/

void INT_Excep_TPU1_TGI1A(void) __attribute__ ((interrupt));

/* TPU1_TGI1B*/

void INT_Excep_TPU1_TGI1B(void) __attribute__ ((interrupt));

/* vector 113 */
/*MTU0 ------------------------------------------------------*/
/* vector 114 */
void  INT_Excep_MTU0_TGIA0(void) __attribute__ ((interrupt));
/* vector 115 */
void  INT_Excep_MTU0_TGIB0(void) __attribute__ ((interrupt));
/* vector 116 */
void  INT_Excep_MTU0_TGIC0(void) __attribute__ ((interrupt));
/* vector 117 */
void  INT_Excep_MTU0_TGID0(void) __attribute__ ((interrupt));
/*vector 118*/
void  INT_Excep_MTU0_TCIV0(void) __attribute__ ((interrupt));
/* vector 119 */
void  INT_Excep_MTU0_TGIE0(void) __attribute__ ((interrupt));
/* vector 120 */
void  INT_Excep_MTU0_TGIF0(void) __attribute__ ((interrupt));
/*MTU1 ------------------------------------------------------*/
/* vector 121 */
void INT_Excep_MTU1_TGIA1(void) __attribute__ ((interrupt));
/* vector 122 */
void INT_Excep_MTU1_TGIB1(void) __attribute__ ((interrupt));
/* vector 123*/
void INT_Excep_MTU1_TCIV1(void) __attribute__ ((interrupt));
/* vector 124*/
void INT_Excep_MTU1_TCIU1(void) __attribute__ ((interrupt));
/*MTU2 ------------------------------------------------------*/
/* vector 125*/
void INT_Excep_MTU2_TGIA2(void) __attribute__ ((interrupt));
/* vector 126*/
void INT_Excep_MTU2_TGIB2(void) __attribute__ ((interrupt));
/* vector 127*/
void INT_Excep_MTU2_TCIV2(void) __attribute__ ((interrupt));
/* vector 128*/
void INT_Excep_MTU2_TCIU2(void) __attribute__ ((interrupt));
/*MTU3 ------------------------------------------------------*/
/* vector 129 */
void INT_Excep_MTU3_TGIA3(void) __attribute__ ((interrupt));
/* vector 130 */
void INT_Excep_MTU3_TGIB3(void) __attribute__ ((interrupt));
/* vector 131*/
void INT_Excep_MTU3_TGIC3(void) __attribute__ ((interrupt));
/* vector 132*/
void INT_Excep_MTU3_TGID3(void) __attribute__ ((interrupt));
/* vector 133*/
void INT_Excep_MTU3_TCIV3(void) __attribute__ ((interrupt));
/*MTU4 ------------------------------------------------------*/
/* vector 134*/
void INT_Excep_MTU4_TGIA4(void) __attribute__ ((interrupt));
/* vector 135 */
void INT_Excep_MTU4_TGIB4(void) __attribute__ ((interrupt));
/* vector 136*/
void INT_Excep_MTU4_TGIC4(void) __attribute__ ((interrupt));
/* vector 137*/
void INT_Excep_MTU4_TGID4(void) __attribute__ ((interrupt));
/* vector 138*/
void INT_Excep_MTU4_TCIV4(void) __attribute__ ((interrupt));
/*MTU5 ------------------------------------------------------*/
/* vector 139*/
void INT_Excep_MTU5_TGIU5(void) __attribute__ ((interrupt));
/* vector 140*/
void INT_Excep_MTU5_TGIV5(void) __attribute__ ((interrupt));
/* vector 140*/
void INT_Excep_MTU5_TGIW5(void) __attribute__ ((interrupt));
/*----- ------------------------------------------------------*/

/* TPU6_TCI6V*/
void INT_Excep_TPU6_TCI6V(void) __attribute__ ((interrupt));


void INT_Excep_OEI1(void)		__attribute__ ((interrupt));

void INT_Excep_OEI2(void)		__attribute__ ((interrupt));


/* vector 143 reserved*/
/* vector 144 reserved*/

/* TPU7_TGI7A*/

void INT_Excep_TPU7_TGI7A(void) __attribute__ ((interrupt));

/* TPU7_TGI7B*/

void INT_Excep_TPU7_TGI7B(void) __attribute__ ((interrupt));

/* vector 147 reserved*/
/* vector 148 reserved*/

/* TPU7_TCI7V*/

void INT_Excep_TPU7_TCI7V(void) __attribute__ ((interrupt));

/* TPU7_TCI7U*/

void INT_Excep_TPU7_TCI7U(void) __attribute__ ((interrupt));

/* TPU8_TGI8A*/

void INT_Excep_TPU8_TGI8A(void) __attribute__ ((interrupt));

/* TPU8_TGI8B*/

void INT_Excep_TPU8_TGI8B(void) __attribute__ ((interrupt));

/* vector 153 reserved*/

/* TPU8_TCI8V*/

void INT_Excep_TPU8_TCI8V(void) __attribute__ ((interrupt));

/* TPU8_TCI8U*/

void INT_Excep_TPU8_TCI8U(void) __attribute__ ((interrupt));

/* TPU9_TGI9A*/

void INT_Excep_TPU9_TGI9A(void) __attribute__ ((interrupt));

/* TPU9_TGI9B*/

void INT_Excep_TPU9_TGI9B(void) __attribute__ ((interrupt));

/* TPU9_TGI9C*/

void INT_Excep_TPU9_TGI9C(void) __attribute__ ((interrupt));

/* TPU9_TGI9D*/

void INT_Excep_TPU9_TGI9D(void) __attribute__ ((interrupt));

/* TPU9_TCI9V*/

void INT_Excep_TPU9_TCI9V(void) __attribute__ ((interrupt));

/* TPU10_TGI10A*/

void INT_Excep_TPU10_TGI10A(void) __attribute__ ((interrupt));

/* TPU10_TGI10B*/

void INT_Excep_TPU10_TGI10B(void) __attribute__ ((interrupt));

/* vector 163 reserved*/
/* vector 164 reserved*/

/* TPU10_TCI10V*/

void INT_Excep_TPU10_TCI10V(void) __attribute__ ((interrupt));

/* TPU10_TCI10U*/

void INT_Excep_TPU10_TCI10U(void) __attribute__ ((interrupt));

/* TPU11_TGI11A*/

void INT_Excep_TPU11_TGI11A(void) __attribute__ ((interrupt));

/* TPU11_TGI11B*/

void INT_Excep_TPU11_TGI11B(void) __attribute__ ((interrupt));

/* vector 169 reserved*/

/* TPU11_TCI11V*/

void INT_Excep_TPU11_TCI11V(void) __attribute__ ((interrupt));

/* TPU11_TCI11U*/

void INT_Excep_TPU11_TCI11U(void) __attribute__ ((interrupt));

/* vector 172 reserved*/
/* vector 173 reserved*/

/* TMR0_CMI0A*/

void INT_Excep_TMR0_CMI0A(void) __attribute__ ((interrupt));

/* TMR0_CMI0B*/

void INT_Excep_TMR0_CMI0B(void) __attribute__ ((interrupt));

/* TMR0_OV0I*/

void INT_Excep_TMR0_OV0I(void) __attribute__ ((interrupt));

/* TMR1_CMI1A*/

void INT_Excep_TMR1_CMI1A(void) __attribute__ ((interrupt));

/* TMR1_CMI1B*/

void INT_Excep_TMR1_CMI1B(void) __attribute__ ((interrupt));

/* TMR1_OV1I*/

void INT_Excep_TMR1_OV1I(void) __attribute__ ((interrupt));

/* TMR2_CMI2A*/

void INT_Excep_TMR2_CMI2A(void) __attribute__ ((interrupt));

/* TMR2_CMI2B*/

void INT_Excep_TMR2_CMI2B(void) __attribute__ ((interrupt));

/* TMR2_OV2I*/

void INT_Excep_TMR2_OV2I(void) __attribute__ ((interrupt));

/* TMR3_CMI3A*/

void INT_Excep_TMR3_CMI3A(void) __attribute__ ((interrupt));

/* TMR3_CMI3B*/

void INT_Excep_TMR3_CMI3B(void) __attribute__ ((interrupt));

/* TMR3_OV3I*/

void INT_Excep_TMR3_OV3I(void) __attribute__ ((interrupt));

/*
 vector 186 reserved
 vector 187 reserved
 vector 188 reserved
 vector 189 reserved
 vector 190 reserved
vector 191 reserved
 vector 192 reserved
 vector 193 reserved
 vector 194 reserved
 vector 195 reserved
 vector 196 reserved
 vector 197 reserved
*/
/* DMAC_DMTEND0*/

void INT_Excep_DMAC_DMTEND0(void)	__attribute__ ((interrupt));

/* DMAC_DMTEND1*/

void INT_Excep_DMAC_DMTEND1(void) __attribute__ ((interrupt));

/* DMAC_DMTEND2*/

void INT_Excep_DMAC_DMTEND2(void) __attribute__ ((interrupt));

/* DMAC_DMTEND3*/

void INT_Excep_DMAC_DMTEND3(void) __attribute__ ((interrupt));
/*
 vector 202 reserved
 vector 203 reserved
 vector 204 reserved
 vector 205 reserved
 vector 206 reserved
 vector 207 reserved
 vector 208 reserved
 vector 209 reserved
 vector 210 reserved
 vector 211 reserved
 vector 212 reserved
 vector 213 reserved
*/
 /*SCI0_ERI0 vector 214*/
void INT_Excep_SCI0_ERI0(void) __attribute__ ((interrupt));

/*  SCI0_RXI0 vector 215*/
void INT_Excep_SCI0_RXI0(void) __attribute__ ((interrupt));

/* SCI0_TXI0  vector 216*/
void INT_Excep_SCI0_TXI0(void) __attribute__ ((interrupt));

/* SCI0_TEI0 vector 217*/
void INT_Excep_SCI0_TEI0(void) __attribute__ ((interrupt));

/* SCI1_ERI1 vector 218*/
void INT_Excep_SCI1_ERI1(void) __attribute__ ((interrupt));

/* SCI1_RXI1  vector 219*/
void INT_Excep_SCI1_RXI1(void) __attribute__ ((interrupt));

/* SCI1_TXI1 vector 220*/
void INT_Excep_SCI1_TXI1(void) __attribute__ ((interrupt));

/* SCI1_TEI1 vector 221*/
void INT_Excep_SCI1_TEI1(void) __attribute__ ((interrupt));

/* SCI5_ERI5 vector 222*/
void INT_Excep_SCI5_ERI5(void) __attribute__ ((interrupt));

/* SCI5_RXI5 vector 223*/
void INT_Excep_SCI5_RXI5(void) __attribute__ ((interrupt));

/* SCI5_TXI5 vector 224*/
void INT_Excep_SCI5_TXI5(void) __attribute__ ((interrupt));

/*SCI5_TEI5 vector 225*/
void INT_Excep_SCI5_TEI5(void) __attribute__ ((interrupt));

/* SCI6_ERI6 vector 226*/
void INT_Excep_SCI6_ERI6(void) __attribute__ ((interrupt));

/* SCI6_RXI6 vector 227*/
void INT_Excep_SCI6_RXI6(void) __attribute__ ((interrupt));

/*SCI6_TXI6 vector 228*/
void INT_Excep_SCI6_TXI6(void) __attribute__ ((interrupt));

/*SCI6_TEI6 vector 229*/
void INT_Excep_SCI6_TEI6(void) __attribute__ ((interrupt));

/*SCI8_ERI8 vector 230*/
void INT_Excep_SCI8_ERI8(void) __attribute__ ((interrupt));

/*SCI8_RXI8 vector 231*/
void INT_Excep_SCI8_RXI8(void) __attribute__ ((interrupt));

/*SCI8_TXI8 vector 232*/
void INT_Excep_SCI8_TXI8(void) __attribute__ ((interrupt));

/* SCI8_TEI8 vector 233*/
void INT_Excep_SCI8_TEI8(void) __attribute__ ((interrupt));

/* SCI9_ERI9 vector 234*/
void INT_Excep_SCI9_ERI9(void) __attribute__ ((interrupt));

/* SCI9_RXI9 vector 235*/
void INT_Excep_SCI9_RXI9(void) __attribute__ ((interrupt));

/* SCI9_TXI9 vector 236*/
void INT_Excep_SCI9_TXI9(void) __attribute__ ((interrupt));

/* SCI9_TEI9 vector 237*/
void INT_Excep_SCI9_TEI9(void) __attribute__ ((interrupt));

/* SCI12_ERI12 vector 238*/
void INT_Excep_SCI12_ERI12(void) __attribute__ ((interrupt));

/*SCI12_RXI12 vector 239*/
void INT_Excep_SCI12_RXI12(void) __attribute__ ((interrupt));

/* SCI12_TXI12 vector 240*/
void INT_Excep_SCI12_TXI12(void) __attribute__ ((interrupt));

/* SCI12_TEI12 vector 241*/
void INT_Excep_SCI12_TEI12(void) __attribute__ ((interrupt));

/* SCI12_SCIX0 vector 242*/
void INT_Excep_SCI12_SCIX0(void) __attribute__ ((interrupt));


void INT_Excep_SCIX0(void)		__attribute__ ((interrupt));

void INT_Excep_SCIX1(void)		__attribute__ ((interrupt));

void INT_Excep_SCIX2(void)		__attribute__ ((interrupt));
void INT_Excep_SCIX3(void)		__attribute__ ((interrupt));


/* RIIC0_EEI0*/
void IIC0_Error_ISR(void)__attribute__ ((interrupt)) ;

/* RIIC0_RXI0*/
void IIC0_RX_ISR(void) __attribute__ ((interrupt));

/* RIIC0_TXI0*/
void IIC0_TX_ISR(void) __attribute__ ((interrupt));

/* RIIC0_TEI0*/
void IIC0_TX_END_ISR(void) __attribute__ ((interrupt));

/* vector 254 reserved*/
/* vector 255 reserved*/

/*;<<VECTOR DATA START (POWER ON RESET)>>*/
/*;Power On Reset PC*/
extern void PowerON_Reset_PC(void) __attribute__ ((interrupt));                                                                                                                
/*;<<VECTOR DATA END (POWER ON RESET)>>*/

#endif
