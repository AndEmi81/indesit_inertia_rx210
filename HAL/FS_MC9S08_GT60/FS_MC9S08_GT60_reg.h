/* Based on CPU DB MC9S08GT60A_48, version 2.87.109 (RegistersPrg V2.23) */
/*
** ###################################################################
**     Filename  : mc9s08gt60a.h
**     Processor : MC9S08GT60ACFD
**     FileFormat: V2.23
**     DataSheet : MC9S08GB60A Rev. 1.00 08/2005
**     Compiler  : CodeWarrior compiler
**     Date/Time : 26.2.2008, 11:52
**     Abstract  :
**         This header implements the mapping of I/O devices.
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2008
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
**
**     CPU Registers Revisions:
**      - 23.02.2006, V2.87.082:
**              - Renamed bits IICA_ADDR0..6 ==> IICA_ADDR1..7 REASON: Bug-fix (#3047 from UNIS issue manager)
**      - 16.03.2006, V2.87.085:
**              - Added register DBGCBL. REASON: Bug-fix (#3123 from UNIS issue manager)
**      - 11.04.2006, V2.87.089:
**              - Renamed bits IIC1A_ADDR0..6 ==> IIC1A_ADDR1..7 REASON: Bug-fix (#3047 from UNIS issue manager)
**
**     File-Format-Revisions:
**      - 14.11.2005, V2.00 :
**               - Deprecated symbols added for backward compatibility (section at the end of this file)
**      - 15.11.2005, V2.01 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 17.12.2005, V2.02 :
**               - Arrays (symbols xx_ARR) are defined as pointer to volatile, see issue #2778
**      - 16.01.2006, V2.03 :
**               - Fixed declaration of non volatile registers. Now it does not require (but allows) their initialization, see issue #2920.
**               - "volatile" modifier removed from declaration of non volatile registers (that contain modifier "const")
**      - 08.03.2006, V2.04 :
**               - Support for bit(s) names duplicated with any register name in .h header files
**      - 24.03.2006, V2.05 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 26.04.2006, V2.06 :
**               - Absolute assembly supported (depreciated symbols are not defined)
**      - 27.04.2006, V2.07 :
**               - Fixed macro __RESET_WATCHDOG for HCS12, HCS12X ,HCS08 DZ and HCS08 EN derivatives (write 0x55,0xAA).
**      - 07.06.2006, V2.08 :
**               - For .inc files added constants "RAMStart" and "RAMEnd" even there is only Z_RAM.
**      - 03.07.2006, V2.09 :
**               - Flash commands constants supported
**      - 27.10.2006, V2.10 :
**               - __RESET_WATCHDOG improved formating and re-definition
**      - 23.11.2006, V2.11 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 22.01.2007, V2.12 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 01.03.2007, V2.13 :
**               - Flash commands constants values converted to HEX format
**      - 02.03.2007, V2.14 :
**               - Interrupt vector numbers added into .H, see VectorNumber_*
**      - 26.03.2007, V2.15 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 10.05.2007, V2.16 :
**               - Fixed flash commands definition for ColdFireV1 assembler (equ -> .equ)
**      - 05.06.2007, V2.17 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 19.07.2007, V2.18 :
**               - Improved number of blanked lines inside register structures
**      - 06.08.2007, V2.19 :
**               - CPUDB revisions generated ahead of the file-format revisions.
**      - 11.09.2007, V2.20 :
**               - Added comment about initialization of unbonded pins.
**      - 02.01.2008, V2.21 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 13.02.2008, V2.22 :
**               -  Changes have not affected this file (because they are related to another family)
**      - 20.02.2008, V2.23 :
**               -  Changes have not affected this file (because they are related to another family)
**
**     Not all general-purpose I/O pins are available on all packages or on all mask sets of a specific
**     derivative device. To avoid extra current drain from floating input pins, the user�s reset
**     initialization routine in the application program must either enable on-chip pull-up devices
**     or change the direction of unconnected pins to outputs so the pins do not float.
** ###################################################################
*/

#ifndef _MC9S08GT60A_H
#define _MC9S08GT60A_H

/* Types definition */
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];

/* Watchdog reset macro */
#ifndef __RESET_WATCHDOG
#ifdef _lint
  #define __RESET_WATCHDOG()  /* empty */
#else
  #define __RESET_WATCHDOG() {asm sta SRS;}
#endif
#endif /* __RESET_WATCHDOG */

#define REG_BASE 0x0000                /* Base address for the I/O register block */


#pragma MESSAGE DISABLE C1106 /* WARNING C1106: Non-standard bitfield type */

/**************** interrupt vector numbers ****************/
#define VectorNumber_Vrti               25
#define VectorNumber_Viic1              24
#define VectorNumber_Vatd1              23
#define VectorNumber_Vkeyboard1         22
#define VectorNumber_Vsci2tx            21
#define VectorNumber_Vsci2rx            20
#define VectorNumber_Vsci2err           19
#define VectorNumber_Vsci1tx            18
#define VectorNumber_Vsci1rx            17
#define VectorNumber_Vsci1err           16
#define VectorNumber_Vspi1              15
#define VectorNumber_Vtpm2ovf           14
#define VectorNumber_Reserved12         13
#define VectorNumber_Reserved13         12
#define VectorNumber_Reserved14         11
#define VectorNumber_Vtpm2ch1           10
#define VectorNumber_Vtpm2ch0           9
#define VectorNumber_Vtpm1ovf           8
#define VectorNumber_Vtpm1ch2           7
#define VectorNumber_Vtpm1ch1           6
#define VectorNumber_Vtpm1ch0           5
#define VectorNumber_Vicg               4
#define VectorNumber_Vlvd               3
#define VectorNumber_Virq               2
#define VectorNumber_Vswi               1
#define VectorNumber_Vreset             0

/**************** interrupt vector table ****************/
#define Vrti                            0x0000FFCC
#define Viic1                           0x0000FFCE
#define Vatd1                           0x0000FFD0
#define Vkeyboard1                      0x0000FFD2
#define Vsci2tx                         0x0000FFD4
#define Vsci2rx                         0x0000FFD6
#define Vsci2err                        0x0000FFD8
#define Vsci1tx                         0x0000FFDA
#define Vsci1rx                         0x0000FFDC
#define Vsci1err                        0x0000FFDE
#define Vspi1                           0x0000FFE0
#define Vtpm2ovf                        0x0000FFE2
#define Reserved12                      0x0000FFE4
#define Reserved13                      0x0000FFE6
#define Reserved14                      0x0000FFE8
#define Vtpm2ch1                        0x0000FFEA
#define Vtpm2ch0                        0x0000FFEC
#define Vtpm1ovf                        0x0000FFEE
#define Vtpm1ch2                        0x0000FFF0
#define Vtpm1ch1                        0x0000FFF2
#define Vtpm1ch0                        0x0000FFF4
#define Vicg                            0x0000FFF6
#define Vlvd                            0x0000FFF8
#define Virq                            0x0000FFFA
#define Vswi                            0x0000FFFC
#define Vreset                          0x0000FFFE

/**************** registers I/O map ****************/

/*** PTAD - Port A Data Register; 0x00000000 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAD0       :1;                                       /* Port A Data Register Bit 0 */
    byte PTAD1       :1;                                       /* Port A Data Register Bit 1 */
    byte PTAD2       :1;                                       /* Port A Data Register Bit 2 */
    byte PTAD3       :1;                                       /* Port A Data Register Bit 3 */
    byte PTAD4       :1;                                       /* Port A Data Register Bit 4 */
    byte PTAD5       :1;                                       /* Port A Data Register Bit 5 */
    byte PTAD6       :1;                                       /* Port A Data Register Bit 6 */
    byte PTAD7       :1;                                       /* Port A Data Register Bit 7 */
  } Bits;
} PTADSTR;
extern volatile PTADSTR _PTAD @0x00000000;
#define PTAD                            _PTAD.Byte
#define PTAD_PTAD0                      _PTAD.Bits.PTAD0
#define PTAD_PTAD1                      _PTAD.Bits.PTAD1
#define PTAD_PTAD2                      _PTAD.Bits.PTAD2
#define PTAD_PTAD3                      _PTAD.Bits.PTAD3
#define PTAD_PTAD4                      _PTAD.Bits.PTAD4
#define PTAD_PTAD5                      _PTAD.Bits.PTAD5
#define PTAD_PTAD6                      _PTAD.Bits.PTAD6
#define PTAD_PTAD7                      _PTAD.Bits.PTAD7

#define PTAD_PTAD0_MASK                 1
#define PTAD_PTAD1_MASK                 2
#define PTAD_PTAD2_MASK                 4
#define PTAD_PTAD3_MASK                 8
#define PTAD_PTAD4_MASK                 16
#define PTAD_PTAD5_MASK                 32
#define PTAD_PTAD6_MASK                 64
#define PTAD_PTAD7_MASK                 128


/*** PTAPE - Pullup Enable for Port A; 0x00000001 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPE0      :1;                                       /* Pullup Enable for Port A Bit 0 */
    byte PTAPE1      :1;                                       /* Pullup Enable for Port A Bit 1 */
    byte PTAPE2      :1;                                       /* Pullup Enable for Port A Bit 2 */
    byte PTAPE3      :1;                                       /* Pullup Enable for Port A Bit 3 */
    byte PTAPE4      :1;                                       /* Pullup Enable for Port A Bit 4 */
    byte PTAPE5      :1;                                       /* Pullup Enable for Port A Bit 5 */
    byte PTAPE6      :1;                                       /* Pullup Enable for Port A Bit 6 */
    byte PTAPE7      :1;                                       /* Pullup Enable for Port A Bit 7 */
  } Bits;
} PTAPESTR;
extern volatile PTAPESTR _PTAPE @0x00000001;
#define PTAPE                           _PTAPE.Byte
#define PTAPE_PTAPE0                    _PTAPE.Bits.PTAPE0
#define PTAPE_PTAPE1                    _PTAPE.Bits.PTAPE1
#define PTAPE_PTAPE2                    _PTAPE.Bits.PTAPE2
#define PTAPE_PTAPE3                    _PTAPE.Bits.PTAPE3
#define PTAPE_PTAPE4                    _PTAPE.Bits.PTAPE4
#define PTAPE_PTAPE5                    _PTAPE.Bits.PTAPE5
#define PTAPE_PTAPE6                    _PTAPE.Bits.PTAPE6
#define PTAPE_PTAPE7                    _PTAPE.Bits.PTAPE7

#define PTAPE_PTAPE0_MASK               1
#define PTAPE_PTAPE1_MASK               2
#define PTAPE_PTAPE2_MASK               4
#define PTAPE_PTAPE3_MASK               8
#define PTAPE_PTAPE4_MASK               16
#define PTAPE_PTAPE5_MASK               32
#define PTAPE_PTAPE6_MASK               64
#define PTAPE_PTAPE7_MASK               128


/*** PTASE - Slew Rate Control Enable for Port A; 0x00000002 ***/
typedef union {
  byte Byte;
  struct {
    byte PTASE0      :1;                                       /* Slew Rate Control Enable for Port A Bit 0 */
    byte PTASE1      :1;                                       /* Slew Rate Control Enable for Port A Bit 1 */
    byte PTASE2      :1;                                       /* Slew Rate Control Enable for Port A Bit 2 */
    byte PTASE3      :1;                                       /* Slew Rate Control Enable for Port A Bit 3 */
    byte PTASE4      :1;                                       /* Slew Rate Control Enable for Port A Bit 4 */
    byte PTASE5      :1;                                       /* Slew Rate Control Enable for Port A Bit 5 */
    byte PTASE6      :1;                                       /* Slew Rate Control Enable for Port A Bit 6 */
    byte PTASE7      :1;                                       /* Slew Rate Control Enable for Port A Bit 7 */
  } Bits;
} PTASESTR;
extern volatile PTASESTR _PTASE @0x00000002;
#define PTASE                           _PTASE.Byte
#define PTASE_PTASE0                    _PTASE.Bits.PTASE0
#define PTASE_PTASE1                    _PTASE.Bits.PTASE1
#define PTASE_PTASE2                    _PTASE.Bits.PTASE2
#define PTASE_PTASE3                    _PTASE.Bits.PTASE3
#define PTASE_PTASE4                    _PTASE.Bits.PTASE4
#define PTASE_PTASE5                    _PTASE.Bits.PTASE5
#define PTASE_PTASE6                    _PTASE.Bits.PTASE6
#define PTASE_PTASE7                    _PTASE.Bits.PTASE7

#define PTASE_PTASE0_MASK               1
#define PTASE_PTASE1_MASK               2
#define PTASE_PTASE2_MASK               4
#define PTASE_PTASE3_MASK               8
#define PTASE_PTASE4_MASK               16
#define PTASE_PTASE5_MASK               32
#define PTASE_PTASE6_MASK               64
#define PTASE_PTASE7_MASK               128


/*** PTADD - Data Direction Register A; 0x00000003 ***/
typedef union {
  byte Byte;
  struct {
    byte PTADD0      :1;                                       /* Data Direction for Port A Bit 0 */
    byte PTADD1      :1;                                       /* Data Direction for Port A Bit 1 */
    byte PTADD2      :1;                                       /* Data Direction for Port A Bit 2 */
    byte PTADD3      :1;                                       /* Data Direction for Port A Bit 3 */
    byte PTADD4      :1;                                       /* Data Direction for Port A Bit 4 */
    byte PTADD5      :1;                                       /* Data Direction for Port A Bit 5 */
    byte PTADD6      :1;                                       /* Data Direction for Port A Bit 6 */
    byte PTADD7      :1;                                       /* Data Direction for Port A Bit 7 */
  } Bits;
} PTADDSTR;
extern volatile PTADDSTR _PTADD @0x00000003;
#define PTADD                           _PTADD.Byte
#define PTADD_PTADD0                    _PTADD.Bits.PTADD0
#define PTADD_PTADD1                    _PTADD.Bits.PTADD1
#define PTADD_PTADD2                    _PTADD.Bits.PTADD2
#define PTADD_PTADD3                    _PTADD.Bits.PTADD3
#define PTADD_PTADD4                    _PTADD.Bits.PTADD4
#define PTADD_PTADD5                    _PTADD.Bits.PTADD5
#define PTADD_PTADD6                    _PTADD.Bits.PTADD6
#define PTADD_PTADD7                    _PTADD.Bits.PTADD7

#define PTADD_PTADD0_MASK               1
#define PTADD_PTADD1_MASK               2
#define PTADD_PTADD2_MASK               4
#define PTADD_PTADD3_MASK               8
#define PTADD_PTADD4_MASK               16
#define PTADD_PTADD5_MASK               32
#define PTADD_PTADD6_MASK               64
#define PTADD_PTADD7_MASK               128


/*** PTBD - Port B Data Register; 0x00000004 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBD0       :1;                                       /* Port B Data Register Bit 0 */
    byte PTBD1       :1;                                       /* Port B Data Register Bit 1 */
    byte PTBD2       :1;                                       /* Port B Data Register Bit 2 */
    byte PTBD3       :1;                                       /* Port B Data Register Bit 3 */
    byte PTBD4       :1;                                       /* Port B Data Register Bit 4 */
    byte PTBD5       :1;                                       /* Port B Data Register Bit 5 */
    byte PTBD6       :1;                                       /* Port B Data Register Bit 6 */
    byte PTBD7       :1;                                       /* Port B Data Register Bit 7 */
  } Bits;
} PTBDSTR;
extern volatile PTBDSTR _PTBD @0x00000004;
#define PTBD                            _PTBD.Byte
#define PTBD_PTBD0                      _PTBD.Bits.PTBD0
#define PTBD_PTBD1                      _PTBD.Bits.PTBD1
#define PTBD_PTBD2                      _PTBD.Bits.PTBD2
#define PTBD_PTBD3                      _PTBD.Bits.PTBD3
#define PTBD_PTBD4                      _PTBD.Bits.PTBD4
#define PTBD_PTBD5                      _PTBD.Bits.PTBD5
#define PTBD_PTBD6                      _PTBD.Bits.PTBD6
#define PTBD_PTBD7                      _PTBD.Bits.PTBD7

#define PTBD_PTBD0_MASK                 1
#define PTBD_PTBD1_MASK                 2
#define PTBD_PTBD2_MASK                 4
#define PTBD_PTBD3_MASK                 8
#define PTBD_PTBD4_MASK                 16
#define PTBD_PTBD5_MASK                 32
#define PTBD_PTBD6_MASK                 64
#define PTBD_PTBD7_MASK                 128


/*** PTBPE - Pullup Enable for Port B; 0x00000005 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPE0      :1;                                       /* Pullup Enable for Port B Bit 0 */
    byte PTBPE1      :1;                                       /* Pullup Enable for Port B Bit 1 */
    byte PTBPE2      :1;                                       /* Pullup Enable for Port B Bit 2 */
    byte PTBPE3      :1;                                       /* Pullup Enable for Port B Bit 3 */
    byte PTBPE4      :1;                                       /* Pullup Enable for Port B Bit 4 */
    byte PTBPE5      :1;                                       /* Pullup Enable for Port B Bit 5 */
    byte PTBPE6      :1;                                       /* Pullup Enable for Port B Bit 6 */
    byte PTBPE7      :1;                                       /* Pullup Enable for Port B Bit 7 */
  } Bits;
} PTBPESTR;
extern volatile PTBPESTR _PTBPE @0x00000005;
#define PTBPE                           _PTBPE.Byte
#define PTBPE_PTBPE0                    _PTBPE.Bits.PTBPE0
#define PTBPE_PTBPE1                    _PTBPE.Bits.PTBPE1
#define PTBPE_PTBPE2                    _PTBPE.Bits.PTBPE2
#define PTBPE_PTBPE3                    _PTBPE.Bits.PTBPE3
#define PTBPE_PTBPE4                    _PTBPE.Bits.PTBPE4
#define PTBPE_PTBPE5                    _PTBPE.Bits.PTBPE5
#define PTBPE_PTBPE6                    _PTBPE.Bits.PTBPE6
#define PTBPE_PTBPE7                    _PTBPE.Bits.PTBPE7

#define PTBPE_PTBPE0_MASK               1
#define PTBPE_PTBPE1_MASK               2
#define PTBPE_PTBPE2_MASK               4
#define PTBPE_PTBPE3_MASK               8
#define PTBPE_PTBPE4_MASK               16
#define PTBPE_PTBPE5_MASK               32
#define PTBPE_PTBPE6_MASK               64
#define PTBPE_PTBPE7_MASK               128


/*** PTBSE - Slew Rate Control Enable for Port B; 0x00000006 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBSE0      :1;                                       /* Slew Rate Control Enable for Port B Bit 0 */
    byte PTBSE1      :1;                                       /* Slew Rate Control Enable for Port B Bit 1 */
    byte PTBSE2      :1;                                       /* Slew Rate Control Enable for Port B Bit 2 */
    byte PTBSE3      :1;                                       /* Slew Rate Control Enable for Port B Bit 3 */
    byte PTBSE4      :1;                                       /* Slew Rate Control Enable for Port B Bit 4 */
    byte PTBSE5      :1;                                       /* Slew Rate Control Enable for Port B Bit 5 */
    byte PTBSE6      :1;                                       /* Slew Rate Control Enable for Port B Bit 6 */
    byte PTBSE7      :1;                                       /* Slew Rate Control Enable for Port B Bit 7 */
  } Bits;
} PTBSESTR;
extern volatile PTBSESTR _PTBSE @0x00000006;
#define PTBSE                           _PTBSE.Byte
#define PTBSE_PTBSE0                    _PTBSE.Bits.PTBSE0
#define PTBSE_PTBSE1                    _PTBSE.Bits.PTBSE1
#define PTBSE_PTBSE2                    _PTBSE.Bits.PTBSE2
#define PTBSE_PTBSE3                    _PTBSE.Bits.PTBSE3
#define PTBSE_PTBSE4                    _PTBSE.Bits.PTBSE4
#define PTBSE_PTBSE5                    _PTBSE.Bits.PTBSE5
#define PTBSE_PTBSE6                    _PTBSE.Bits.PTBSE6
#define PTBSE_PTBSE7                    _PTBSE.Bits.PTBSE7

#define PTBSE_PTBSE0_MASK               1
#define PTBSE_PTBSE1_MASK               2
#define PTBSE_PTBSE2_MASK               4
#define PTBSE_PTBSE3_MASK               8
#define PTBSE_PTBSE4_MASK               16
#define PTBSE_PTBSE5_MASK               32
#define PTBSE_PTBSE6_MASK               64
#define PTBSE_PTBSE7_MASK               128


/*** PTBDD - Data Direction Register B; 0x00000007 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBDD0      :1;                                       /* Data Direction for Port B Bit 0 */
    byte PTBDD1      :1;                                       /* Data Direction for Port B Bit 1 */
    byte PTBDD2      :1;                                       /* Data Direction for Port B Bit 2 */
    byte PTBDD3      :1;                                       /* Data Direction for Port B Bit 3 */
    byte PTBDD4      :1;                                       /* Data Direction for Port B Bit 4 */
    byte PTBDD5      :1;                                       /* Data Direction for Port B Bit 5 */
    byte PTBDD6      :1;                                       /* Data Direction for Port B Bit 6 */
    byte PTBDD7      :1;                                       /* Data Direction for Port B Bit 7 */
  } Bits;
} PTBDDSTR;
extern volatile PTBDDSTR _PTBDD @0x00000007;
#define PTBDD                           _PTBDD.Byte
#define PTBDD_PTBDD0                    _PTBDD.Bits.PTBDD0
#define PTBDD_PTBDD1                    _PTBDD.Bits.PTBDD1
#define PTBDD_PTBDD2                    _PTBDD.Bits.PTBDD2
#define PTBDD_PTBDD3                    _PTBDD.Bits.PTBDD3
#define PTBDD_PTBDD4                    _PTBDD.Bits.PTBDD4
#define PTBDD_PTBDD5                    _PTBDD.Bits.PTBDD5
#define PTBDD_PTBDD6                    _PTBDD.Bits.PTBDD6
#define PTBDD_PTBDD7                    _PTBDD.Bits.PTBDD7

#define PTBDD_PTBDD0_MASK               1
#define PTBDD_PTBDD1_MASK               2
#define PTBDD_PTBDD2_MASK               4
#define PTBDD_PTBDD3_MASK               8
#define PTBDD_PTBDD4_MASK               16
#define PTBDD_PTBDD5_MASK               32
#define PTBDD_PTBDD6_MASK               64
#define PTBDD_PTBDD7_MASK               128


/*** PTCD - Port C Data Register; 0x00000008 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCD0       :1;                                       /* Port C Data Register Bit 0 */
    byte PTCD1       :1;                                       /* Port C Data Register Bit 1 */
    byte PTCD2       :1;                                       /* Port C Data Register Bit 2 */
    byte PTCD3       :1;                                       /* Port C Data Register Bit 3 */
    byte PTCD4       :1;                                       /* Port C Data Register Bit 4 */
    byte PTCD5       :1;                                       /* Port C Data Register Bit 5 */
    byte PTCD6       :1;                                       /* Port C Data Register Bit 6 */
    byte PTCD7       :1;                                       /* Port C Data Register Bit 7 */
  } Bits;
} PTCDSTR;
extern volatile PTCDSTR _PTCD @0x00000008;
#define PTCD                            _PTCD.Byte
#define PTCD_PTCD0                      _PTCD.Bits.PTCD0
#define PTCD_PTCD1                      _PTCD.Bits.PTCD1
#define PTCD_PTCD2                      _PTCD.Bits.PTCD2
#define PTCD_PTCD3                      _PTCD.Bits.PTCD3
#define PTCD_PTCD4                      _PTCD.Bits.PTCD4
#define PTCD_PTCD5                      _PTCD.Bits.PTCD5
#define PTCD_PTCD6                      _PTCD.Bits.PTCD6
#define PTCD_PTCD7                      _PTCD.Bits.PTCD7

#define PTCD_PTCD0_MASK                 1
#define PTCD_PTCD1_MASK                 2
#define PTCD_PTCD2_MASK                 4
#define PTCD_PTCD3_MASK                 8
#define PTCD_PTCD4_MASK                 16
#define PTCD_PTCD5_MASK                 32
#define PTCD_PTCD6_MASK                 64
#define PTCD_PTCD7_MASK                 128


/*** PTCPE - Pullup Enable for Port C; 0x00000009 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPE0      :1;                                       /* Pullup Enable for Port C Bit 0 */
    byte PTCPE1      :1;                                       /* Pullup Enable for Port C Bit 1 */
    byte PTCPE2      :1;                                       /* Pullup Enable for Port C Bit 2 */
    byte PTCPE3      :1;                                       /* Pullup Enable for Port C Bit 3 */
    byte PTCPE4      :1;                                       /* Pullup Enable for Port C Bit 4 */
    byte PTCPE5      :1;                                       /* Pullup Enable for Port C Bit 5 */
    byte PTCPE6      :1;                                       /* Pullup Enable for Port C Bit 6 */
    byte PTCPE7      :1;                                       /* Pullup Enable for Port C Bit 7 */
  } Bits;
} PTCPESTR;
extern volatile PTCPESTR _PTCPE @0x00000009;
#define PTCPE                           _PTCPE.Byte
#define PTCPE_PTCPE0                    _PTCPE.Bits.PTCPE0
#define PTCPE_PTCPE1                    _PTCPE.Bits.PTCPE1
#define PTCPE_PTCPE2                    _PTCPE.Bits.PTCPE2
#define PTCPE_PTCPE3                    _PTCPE.Bits.PTCPE3
#define PTCPE_PTCPE4                    _PTCPE.Bits.PTCPE4
#define PTCPE_PTCPE5                    _PTCPE.Bits.PTCPE5
#define PTCPE_PTCPE6                    _PTCPE.Bits.PTCPE6
#define PTCPE_PTCPE7                    _PTCPE.Bits.PTCPE7

#define PTCPE_PTCPE0_MASK               1
#define PTCPE_PTCPE1_MASK               2
#define PTCPE_PTCPE2_MASK               4
#define PTCPE_PTCPE3_MASK               8
#define PTCPE_PTCPE4_MASK               16
#define PTCPE_PTCPE5_MASK               32
#define PTCPE_PTCPE6_MASK               64
#define PTCPE_PTCPE7_MASK               128


/*** PTCSE - Slew Rate Control Enable for Port C; 0x0000000A ***/
typedef union {
  byte Byte;
  struct {
    byte PTCSE0      :1;                                       /* Slew Rate Control Enable for Port C Bit 0 */
    byte PTCSE1      :1;                                       /* Slew Rate Control Enable for Port C Bit 1 */
    byte PTCSE2      :1;                                       /* Slew Rate Control Enable for Port C Bit 2 */
    byte PTCSE3      :1;                                       /* Slew Rate Control Enable for Port C Bit 3 */
    byte PTCSE4      :1;                                       /* Slew Rate Control Enable for Port C Bit 4 */
    byte PTCSE5      :1;                                       /* Slew Rate Control Enable for Port C Bit 5 */
    byte PTCSE6      :1;                                       /* Slew Rate Control Enable for Port C Bit 6 */
    byte PTCSE7      :1;                                       /* Slew Rate Control Enable for Port C Bit 7 */
  } Bits;
} PTCSESTR;
extern volatile PTCSESTR _PTCSE @0x0000000A;
#define PTCSE                           _PTCSE.Byte
#define PTCSE_PTCSE0                    _PTCSE.Bits.PTCSE0
#define PTCSE_PTCSE1                    _PTCSE.Bits.PTCSE1
#define PTCSE_PTCSE2                    _PTCSE.Bits.PTCSE2
#define PTCSE_PTCSE3                    _PTCSE.Bits.PTCSE3
#define PTCSE_PTCSE4                    _PTCSE.Bits.PTCSE4
#define PTCSE_PTCSE5                    _PTCSE.Bits.PTCSE5
#define PTCSE_PTCSE6                    _PTCSE.Bits.PTCSE6
#define PTCSE_PTCSE7                    _PTCSE.Bits.PTCSE7

#define PTCSE_PTCSE0_MASK               1
#define PTCSE_PTCSE1_MASK               2
#define PTCSE_PTCSE2_MASK               4
#define PTCSE_PTCSE3_MASK               8
#define PTCSE_PTCSE4_MASK               16
#define PTCSE_PTCSE5_MASK               32
#define PTCSE_PTCSE6_MASK               64
#define PTCSE_PTCSE7_MASK               128


/*** PTCDD - Data Direction Register C; 0x0000000B ***/
typedef union {
  byte Byte;
  struct {
    byte PTCDD0      :1;                                       /* Data Direction for Port C Bit 0 */
    byte PTCDD1      :1;                                       /* Data Direction for Port C Bit 1 */
    byte PTCDD2      :1;                                       /* Data Direction for Port C Bit 2 */
    byte PTCDD3      :1;                                       /* Data Direction for Port C Bit 3 */
    byte PTCDD4      :1;                                       /* Data Direction for Port C Bit 4 */
    byte PTCDD5      :1;                                       /* Data Direction for Port C Bit 5 */
    byte PTCDD6      :1;                                       /* Data Direction for Port C Bit 6 */
    byte PTCDD7      :1;                                       /* Data Direction for Port C Bit 7 */
  } Bits;
} PTCDDSTR;
extern volatile PTCDDSTR _PTCDD @0x0000000B;
#define PTCDD                           _PTCDD.Byte
#define PTCDD_PTCDD0                    _PTCDD.Bits.PTCDD0
#define PTCDD_PTCDD1                    _PTCDD.Bits.PTCDD1
#define PTCDD_PTCDD2                    _PTCDD.Bits.PTCDD2
#define PTCDD_PTCDD3                    _PTCDD.Bits.PTCDD3
#define PTCDD_PTCDD4                    _PTCDD.Bits.PTCDD4
#define PTCDD_PTCDD5                    _PTCDD.Bits.PTCDD5
#define PTCDD_PTCDD6                    _PTCDD.Bits.PTCDD6
#define PTCDD_PTCDD7                    _PTCDD.Bits.PTCDD7

#define PTCDD_PTCDD0_MASK               1
#define PTCDD_PTCDD1_MASK               2
#define PTCDD_PTCDD2_MASK               4
#define PTCDD_PTCDD3_MASK               8
#define PTCDD_PTCDD4_MASK               16
#define PTCDD_PTCDD5_MASK               32
#define PTCDD_PTCDD6_MASK               64
#define PTCDD_PTCDD7_MASK               128


/*** PTDD - Port D Data Register; 0x0000000C ***/
typedef union {
  byte Byte;
  struct {
    byte PTDD0       :1;                                       /* Port D Data Register Bit 0 */
    byte PTDD1       :1;                                       /* Port D Data Register Bit 1 */
    byte PTDD2       :1;                                       /* Port D Data Register Bit 2 */
    byte PTDD3       :1;                                       /* Port D Data Register Bit 3 */
    byte PTDD4       :1;                                       /* Port D Data Register Bit 4 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTDD :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTDDSTR;
extern volatile PTDDSTR _PTDD @0x0000000C;
#define PTDD                            _PTDD.Byte
#define PTDD_PTDD0                      _PTDD.Bits.PTDD0
#define PTDD_PTDD1                      _PTDD.Bits.PTDD1
#define PTDD_PTDD2                      _PTDD.Bits.PTDD2
#define PTDD_PTDD3                      _PTDD.Bits.PTDD3
#define PTDD_PTDD4                      _PTDD.Bits.PTDD4
#define PTDD_PTDD                       _PTDD.MergedBits.grpPTDD

#define PTDD_PTDD0_MASK                 1
#define PTDD_PTDD1_MASK                 2
#define PTDD_PTDD2_MASK                 4
#define PTDD_PTDD3_MASK                 8
#define PTDD_PTDD4_MASK                 16
#define PTDD_PTDD_MASK                  31
#define PTDD_PTDD_BITNUM                0


/*** PTDPE - Pullup Enable for Port D; 0x0000000D ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPE0      :1;                                       /* Pullup Enable for Port D Bit 0 */
    byte PTDPE1      :1;                                       /* Pullup Enable for Port D Bit 1 */
    byte PTDPE2      :1;                                       /* Pullup Enable for Port D Bit 2 */
    byte PTDPE3      :1;                                       /* Pullup Enable for Port D Bit 3 */
    byte PTDPE4      :1;                                       /* Pullup Enable for Port D Bit 4 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTDPE :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTDPESTR;
extern volatile PTDPESTR _PTDPE @0x0000000D;
#define PTDPE                           _PTDPE.Byte
#define PTDPE_PTDPE0                    _PTDPE.Bits.PTDPE0
#define PTDPE_PTDPE1                    _PTDPE.Bits.PTDPE1
#define PTDPE_PTDPE2                    _PTDPE.Bits.PTDPE2
#define PTDPE_PTDPE3                    _PTDPE.Bits.PTDPE3
#define PTDPE_PTDPE4                    _PTDPE.Bits.PTDPE4
#define PTDPE_PTDPE                     _PTDPE.MergedBits.grpPTDPE

#define PTDPE_PTDPE0_MASK               1
#define PTDPE_PTDPE1_MASK               2
#define PTDPE_PTDPE2_MASK               4
#define PTDPE_PTDPE3_MASK               8
#define PTDPE_PTDPE4_MASK               16
#define PTDPE_PTDPE_MASK                31
#define PTDPE_PTDPE_BITNUM              0


/*** PTDSE - Slew Rate Control Enable for Port D; 0x0000000E ***/
typedef union {
  byte Byte;
  struct {
    byte PTDSE0      :1;                                       /* Slew Rate Control Enable for Port D Bit 0 */
    byte PTDSE1      :1;                                       /* Slew Rate Control Enable for Port D Bit 1 */
    byte PTDSE2      :1;                                       /* Slew Rate Control Enable for Port D Bit 2 */
    byte PTDSE3      :1;                                       /* Slew Rate Control Enable for Port D Bit 3 */
    byte PTDSE4      :1;                                       /* Slew Rate Control Enable for Port D Bit 4 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTDSE :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTDSESTR;
extern volatile PTDSESTR _PTDSE @0x0000000E;
#define PTDSE                           _PTDSE.Byte
#define PTDSE_PTDSE0                    _PTDSE.Bits.PTDSE0
#define PTDSE_PTDSE1                    _PTDSE.Bits.PTDSE1
#define PTDSE_PTDSE2                    _PTDSE.Bits.PTDSE2
#define PTDSE_PTDSE3                    _PTDSE.Bits.PTDSE3
#define PTDSE_PTDSE4                    _PTDSE.Bits.PTDSE4
#define PTDSE_PTDSE                     _PTDSE.MergedBits.grpPTDSE

#define PTDSE_PTDSE0_MASK               1
#define PTDSE_PTDSE1_MASK               2
#define PTDSE_PTDSE2_MASK               4
#define PTDSE_PTDSE3_MASK               8
#define PTDSE_PTDSE4_MASK               16
#define PTDSE_PTDSE_MASK                31
#define PTDSE_PTDSE_BITNUM              0


/*** PTDDD - Data Direction Register D; 0x0000000F ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDD0      :1;                                       /* Data Direction for Port D Bit 0 */
    byte PTDDD1      :1;                                       /* Data Direction for Port D Bit 1 */
    byte PTDDD2      :1;                                       /* Data Direction for Port D Bit 2 */
    byte PTDDD3      :1;                                       /* Data Direction for Port D Bit 3 */
    byte PTDDD4      :1;                                       /* Data Direction for Port D Bit 4 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTDDD :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTDDDSTR;
extern volatile PTDDDSTR _PTDDD @0x0000000F;
#define PTDDD                           _PTDDD.Byte
#define PTDDD_PTDDD0                    _PTDDD.Bits.PTDDD0
#define PTDDD_PTDDD1                    _PTDDD.Bits.PTDDD1
#define PTDDD_PTDDD2                    _PTDDD.Bits.PTDDD2
#define PTDDD_PTDDD3                    _PTDDD.Bits.PTDDD3
#define PTDDD_PTDDD4                    _PTDDD.Bits.PTDDD4
#define PTDDD_PTDDD                     _PTDDD.MergedBits.grpPTDDD

#define PTDDD_PTDDD0_MASK               1
#define PTDDD_PTDDD1_MASK               2
#define PTDDD_PTDDD2_MASK               4
#define PTDDD_PTDDD3_MASK               8
#define PTDDD_PTDDD4_MASK               16
#define PTDDD_PTDDD_MASK                31
#define PTDDD_PTDDD_BITNUM              0


/*** PTED - Port E Data Register; 0x00000010 ***/
typedef union {
  byte Byte;
  struct {
    byte PTED0       :1;                                       /* Port E Data Register Bit 0 */
    byte PTED1       :1;                                       /* Port E Data Register Bit 1 */
    byte PTED2       :1;                                       /* Port E Data Register Bit 2 */
    byte PTED3       :1;                                       /* Port E Data Register Bit 3 */
    byte PTED4       :1;                                       /* Port E Data Register Bit 4 */
    byte PTED5       :1;                                       /* Port E Data Register Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTED :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTEDSTR;
extern volatile PTEDSTR _PTED @0x00000010;
#define PTED                            _PTED.Byte
#define PTED_PTED0                      _PTED.Bits.PTED0
#define PTED_PTED1                      _PTED.Bits.PTED1
#define PTED_PTED2                      _PTED.Bits.PTED2
#define PTED_PTED3                      _PTED.Bits.PTED3
#define PTED_PTED4                      _PTED.Bits.PTED4
#define PTED_PTED5                      _PTED.Bits.PTED5
#define PTED_PTED                       _PTED.MergedBits.grpPTED

#define PTED_PTED0_MASK                 1
#define PTED_PTED1_MASK                 2
#define PTED_PTED2_MASK                 4
#define PTED_PTED3_MASK                 8
#define PTED_PTED4_MASK                 16
#define PTED_PTED5_MASK                 32
#define PTED_PTED_MASK                  63
#define PTED_PTED_BITNUM                0


/*** PTEPE - Pullup Enable for Port E; 0x00000011 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPE0      :1;                                       /* Pullup Enable for Port E Bit 0 */
    byte PTEPE1      :1;                                       /* Pullup Enable for Port E Bit 1 */
    byte PTEPE2      :1;                                       /* Pullup Enable for Port E Bit 2 */
    byte PTEPE3      :1;                                       /* Pullup Enable for Port E Bit 3 */
    byte PTEPE4      :1;                                       /* Pullup Enable for Port E Bit 4 */
    byte PTEPE5      :1;                                       /* Pullup Enable for Port E Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTEPE :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTEPESTR;
extern volatile PTEPESTR _PTEPE @0x00000011;
#define PTEPE                           _PTEPE.Byte
#define PTEPE_PTEPE0                    _PTEPE.Bits.PTEPE0
#define PTEPE_PTEPE1                    _PTEPE.Bits.PTEPE1
#define PTEPE_PTEPE2                    _PTEPE.Bits.PTEPE2
#define PTEPE_PTEPE3                    _PTEPE.Bits.PTEPE3
#define PTEPE_PTEPE4                    _PTEPE.Bits.PTEPE4
#define PTEPE_PTEPE5                    _PTEPE.Bits.PTEPE5
#define PTEPE_PTEPE                     _PTEPE.MergedBits.grpPTEPE

#define PTEPE_PTEPE0_MASK               1
#define PTEPE_PTEPE1_MASK               2
#define PTEPE_PTEPE2_MASK               4
#define PTEPE_PTEPE3_MASK               8
#define PTEPE_PTEPE4_MASK               16
#define PTEPE_PTEPE5_MASK               32
#define PTEPE_PTEPE_MASK                63
#define PTEPE_PTEPE_BITNUM              0


/*** PTESE - Slew Rate Control Enable for Port E; 0x00000012 ***/
typedef union {
  byte Byte;
  struct {
    byte PTESE0      :1;                                       /* Slew Rate Control Enable for Port E Bit 0 */
    byte PTESE1      :1;                                       /* Slew Rate Control Enable for Port E Bit 1 */
    byte PTESE2      :1;                                       /* Slew Rate Control Enable for Port E Bit 2 */
    byte PTESE3      :1;                                       /* Slew Rate Control Enable for Port E Bit 3 */
    byte PTESE4      :1;                                       /* Slew Rate Control Enable for Port E Bit 4 */
    byte PTESE5      :1;                                       /* Slew Rate Control Enable for Port E Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTESE :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTESESTR;
extern volatile PTESESTR _PTESE @0x00000012;
#define PTESE                           _PTESE.Byte
#define PTESE_PTESE0                    _PTESE.Bits.PTESE0
#define PTESE_PTESE1                    _PTESE.Bits.PTESE1
#define PTESE_PTESE2                    _PTESE.Bits.PTESE2
#define PTESE_PTESE3                    _PTESE.Bits.PTESE3
#define PTESE_PTESE4                    _PTESE.Bits.PTESE4
#define PTESE_PTESE5                    _PTESE.Bits.PTESE5
#define PTESE_PTESE                     _PTESE.MergedBits.grpPTESE

#define PTESE_PTESE0_MASK               1
#define PTESE_PTESE1_MASK               2
#define PTESE_PTESE2_MASK               4
#define PTESE_PTESE3_MASK               8
#define PTESE_PTESE4_MASK               16
#define PTESE_PTESE5_MASK               32
#define PTESE_PTESE_MASK                63
#define PTESE_PTESE_BITNUM              0


/*** PTEDD - Data Direction Register E; 0x00000013 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDD0      :1;                                       /* Data Direction for Port E Bit 0 */
    byte PTEDD1      :1;                                       /* Data Direction for Port E Bit 1 */
    byte PTEDD2      :1;                                       /* Data Direction for Port E Bit 2 */
    byte PTEDD3      :1;                                       /* Data Direction for Port E Bit 3 */
    byte PTEDD4      :1;                                       /* Data Direction for Port E Bit 4 */
    byte PTEDD5      :1;                                       /* Data Direction for Port E Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTEDD :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTEDDSTR;
extern volatile PTEDDSTR _PTEDD @0x00000013;
#define PTEDD                           _PTEDD.Byte
#define PTEDD_PTEDD0                    _PTEDD.Bits.PTEDD0
#define PTEDD_PTEDD1                    _PTEDD.Bits.PTEDD1
#define PTEDD_PTEDD2                    _PTEDD.Bits.PTEDD2
#define PTEDD_PTEDD3                    _PTEDD.Bits.PTEDD3
#define PTEDD_PTEDD4                    _PTEDD.Bits.PTEDD4
#define PTEDD_PTEDD5                    _PTEDD.Bits.PTEDD5
#define PTEDD_PTEDD                     _PTEDD.MergedBits.grpPTEDD

#define PTEDD_PTEDD0_MASK               1
#define PTEDD_PTEDD1_MASK               2
#define PTEDD_PTEDD2_MASK               4
#define PTEDD_PTEDD3_MASK               8
#define PTEDD_PTEDD4_MASK               16
#define PTEDD_PTEDD5_MASK               32
#define PTEDD_PTEDD_MASK                63
#define PTEDD_PTEDD_BITNUM              0


/*** IRQSC - Interrupt Request Status and Control Register; 0x00000014 ***/
typedef union {
  byte Byte;
  struct {
    byte IRQMOD      :1;                                       /* IRQ Detection Mode */
    byte IRQIE       :1;                                       /* IRQ Interrupt Enable */
    byte IRQACK      :1;                                       /* IRQ Acknowledge */
    byte IRQF        :1;                                       /* IRQ Flag */
    byte IRQPE       :1;                                       /* IRQ Pin Enable */
    byte IRQEDG      :1;                                       /* Interrupt Request (IRQ) Edge Select */
    byte             :1; 
    byte             :1; 
  } Bits;
} IRQSCSTR;
extern volatile IRQSCSTR _IRQSC @0x00000014;
#define IRQSC                           _IRQSC.Byte
#define IRQSC_IRQMOD                    _IRQSC.Bits.IRQMOD
#define IRQSC_IRQIE                     _IRQSC.Bits.IRQIE
#define IRQSC_IRQACK                    _IRQSC.Bits.IRQACK
#define IRQSC_IRQF                      _IRQSC.Bits.IRQF
#define IRQSC_IRQPE                     _IRQSC.Bits.IRQPE
#define IRQSC_IRQEDG                    _IRQSC.Bits.IRQEDG

#define IRQSC_IRQMOD_MASK               1
#define IRQSC_IRQIE_MASK                2
#define IRQSC_IRQACK_MASK               4
#define IRQSC_IRQF_MASK                 8
#define IRQSC_IRQPE_MASK                16
#define IRQSC_IRQEDG_MASK               32


/*** KBI1SC - KBI1 Status and Control; 0x00000016 ***/
typedef union {
  byte Byte;
  struct {
    byte KBIMOD      :1;                                       /* Keyboard Detection Mode */
    byte KBIE        :1;                                       /* Keyboard Interrupt Enable */
    byte KBACK       :1;                                       /* Keyboard Interrupt Acknowledge */
    byte KBF         :1;                                       /* Keyboard Interrupt Flag */
    byte KBEDG4      :1;                                       /* Keyboard Edge Select for Port A Bit 4 */
    byte KBEDG5      :1;                                       /* Keyboard Edge Select for Port A Bit 5 */
    byte KBEDG6      :1;                                       /* Keyboard Edge Select for Port A Bit 6 */
    byte KBEDG7      :1;                                       /* Keyboard Edge Select for Port A Bit 7 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpKBEDG_4 :4;
  } MergedBits;
} KBI1SCSTR;
extern volatile KBI1SCSTR _KBI1SC @0x00000016;
#define KBI1SC                          _KBI1SC.Byte
#define KBI1SC_KBIMOD                   _KBI1SC.Bits.KBIMOD
#define KBI1SC_KBIE                     _KBI1SC.Bits.KBIE
#define KBI1SC_KBACK                    _KBI1SC.Bits.KBACK
#define KBI1SC_KBF                      _KBI1SC.Bits.KBF
#define KBI1SC_KBEDG4                   _KBI1SC.Bits.KBEDG4
#define KBI1SC_KBEDG5                   _KBI1SC.Bits.KBEDG5
#define KBI1SC_KBEDG6                   _KBI1SC.Bits.KBEDG6
#define KBI1SC_KBEDG7                   _KBI1SC.Bits.KBEDG7
#define KBI1SC_KBEDG_4                  _KBI1SC.MergedBits.grpKBEDG_4
#define KBI1SC_KBEDG                    KBI1SC_KBEDG_4

#define KBI1SC_KBIMOD_MASK              1
#define KBI1SC_KBIE_MASK                2
#define KBI1SC_KBACK_MASK               4
#define KBI1SC_KBF_MASK                 8
#define KBI1SC_KBEDG4_MASK              16
#define KBI1SC_KBEDG5_MASK              32
#define KBI1SC_KBEDG6_MASK              64
#define KBI1SC_KBEDG7_MASK              128
#define KBI1SC_KBEDG_4_MASK             240
#define KBI1SC_KBEDG_4_BITNUM           4


/*** KBI1PE - KBI1 Pin Enable Register; 0x00000017 ***/
typedef union {
  byte Byte;
  struct {
    byte KBIPE0      :1;                                       /* Keyboard Pin Enable for Port A Bit 0 */
    byte KBIPE1      :1;                                       /* Keyboard Pin Enable for Port A Bit 1 */
    byte KBIPE2      :1;                                       /* Keyboard Pin Enable for Port A Bit 2 */
    byte KBIPE3      :1;                                       /* Keyboard Pin Enable for Port A Bit 3 */
    byte KBIPE4      :1;                                       /* Keyboard Pin Enable for Port A Bit 4 */
    byte KBIPE5      :1;                                       /* Keyboard Pin Enable for Port A Bit 5 */
    byte KBIPE6      :1;                                       /* Keyboard Pin Enable for Port A Bit 6 */
    byte KBIPE7      :1;                                       /* Keyboard Pin Enable for Port A Bit 7 */
  } Bits;
} KBI1PESTR;
extern volatile KBI1PESTR _KBI1PE @0x00000017;
#define KBI1PE                          _KBI1PE.Byte
#define KBI1PE_KBIPE0                   _KBI1PE.Bits.KBIPE0
#define KBI1PE_KBIPE1                   _KBI1PE.Bits.KBIPE1
#define KBI1PE_KBIPE2                   _KBI1PE.Bits.KBIPE2
#define KBI1PE_KBIPE3                   _KBI1PE.Bits.KBIPE3
#define KBI1PE_KBIPE4                   _KBI1PE.Bits.KBIPE4
#define KBI1PE_KBIPE5                   _KBI1PE.Bits.KBIPE5
#define KBI1PE_KBIPE6                   _KBI1PE.Bits.KBIPE6
#define KBI1PE_KBIPE7                   _KBI1PE.Bits.KBIPE7

#define KBI1PE_KBIPE0_MASK              1
#define KBI1PE_KBIPE1_MASK              2
#define KBI1PE_KBIPE2_MASK              4
#define KBI1PE_KBIPE3_MASK              8
#define KBI1PE_KBIPE4_MASK              16
#define KBI1PE_KBIPE5_MASK              32
#define KBI1PE_KBIPE6_MASK              64
#define KBI1PE_KBIPE7_MASK              128


/*** SCI1BD - SCI1 Baud Rate Register; 0x00000018 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI1BDH - SCI1 Baud Rate Register High; 0x00000018 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpSBR_8 :5;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SCI1BDHSTR;
    #define SCI1BDH                     _SCI1BD.Overlap_STR.SCI1BDHSTR.Byte
    #define SCI1BDH_SBR8                _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR8
    #define SCI1BDH_SBR9                _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR9
    #define SCI1BDH_SBR10               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR10
    #define SCI1BDH_SBR11               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR11
    #define SCI1BDH_SBR12               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR12
    #define SCI1BDH_SBR_8               _SCI1BD.Overlap_STR.SCI1BDHSTR.MergedBits.grpSBR_8
    #define SCI1BDH_SBR                 SCI1BDH_SBR_8
    
    #define SCI1BDH_SBR8_MASK           1
    #define SCI1BDH_SBR9_MASK           2
    #define SCI1BDH_SBR10_MASK          4
    #define SCI1BDH_SBR11_MASK          8
    #define SCI1BDH_SBR12_MASK          16
    #define SCI1BDH_SBR_8_MASK          31
    #define SCI1BDH_SBR_8_BITNUM        0
    

    /*** SCI1BDL - SCI1 Baud Rate Register Low; 0x00000019 ***/
    union {
      byte Byte;
      struct {
        byte SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
        byte SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
        byte SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
        byte SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
        byte SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
        byte SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
        byte SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
        byte SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
      } Bits;
    } SCI1BDLSTR;
    #define SCI1BDL                     _SCI1BD.Overlap_STR.SCI1BDLSTR.Byte
    #define SCI1BDL_SBR0                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR0
    #define SCI1BDL_SBR1                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR1
    #define SCI1BDL_SBR2                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR2
    #define SCI1BDL_SBR3                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR3
    #define SCI1BDL_SBR4                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR4
    #define SCI1BDL_SBR5                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR5
    #define SCI1BDL_SBR6                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR6
    #define SCI1BDL_SBR7                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR7
    
    #define SCI1BDL_SBR0_MASK           1
    #define SCI1BDL_SBR1_MASK           2
    #define SCI1BDL_SBR2_MASK           4
    #define SCI1BDL_SBR3_MASK           8
    #define SCI1BDL_SBR4_MASK           16
    #define SCI1BDL_SBR5_MASK           32
    #define SCI1BDL_SBR6_MASK           64
    #define SCI1BDL_SBR7_MASK           128
    
  } Overlap_STR;

} SCI1BDSTR;
extern volatile SCI1BDSTR _SCI1BD @0x00000018;
#define SCI1BD                          _SCI1BD.Word


/*** SCI1C1 - SCI1 Control Register 1; 0x0000001A ***/
typedef union {
  byte Byte;
  struct {
    byte PT          :1;                                       /* Parity Type */
    byte PE          :1;                                       /* Parity Enable */
    byte ILT         :1;                                       /* Idle Line Type Select */
    byte WAKE        :1;                                       /* Receiver Wakeup Method Select */
    byte M           :1;                                       /* 9-Bit or 8-Bit Mode Select */
    byte RSRC        :1;                                       /* Receiver Source Select */
    byte SCISWAI     :1;                                       /* SCI Stops in Wait Mode */
    byte LOOPS       :1;                                       /* Loop Mode Select */
  } Bits;
} SCI1C1STR;
extern volatile SCI1C1STR _SCI1C1 @0x0000001A;
#define SCI1C1                          _SCI1C1.Byte
#define SCI1C1_PT                       _SCI1C1.Bits.PT
#define SCI1C1_PE                       _SCI1C1.Bits.PE
#define SCI1C1_ILT                      _SCI1C1.Bits.ILT
#define SCI1C1_WAKE                     _SCI1C1.Bits.WAKE
#define SCI1C1_M                        _SCI1C1.Bits.M
#define SCI1C1_RSRC                     _SCI1C1.Bits.RSRC
#define SCI1C1_SCISWAI                  _SCI1C1.Bits.SCISWAI
#define SCI1C1_LOOPS                    _SCI1C1.Bits.LOOPS

#define SCI1C1_PT_MASK                  1
#define SCI1C1_PE_MASK                  2
#define SCI1C1_ILT_MASK                 4
#define SCI1C1_WAKE_MASK                8
#define SCI1C1_M_MASK                   16
#define SCI1C1_RSRC_MASK                32
#define SCI1C1_SCISWAI_MASK             64
#define SCI1C1_LOOPS_MASK               128


/*** SCI1C2 - SCI1 Control Register 2; 0x0000001B ***/
typedef union {
  byte Byte;
  struct {
    byte SBK         :1;                                       /* Send Break */
    byte RWU         :1;                                       /* Receiver Wakeup Control */
    byte RE          :1;                                       /* Receiver Enable */
    byte TE          :1;                                       /* Transmitter Enable */
    byte ILIE        :1;                                       /* Idle Line Interrupt Enable (for IDLE) */
    byte RIE         :1;                                       /* Receiver Interrupt Enable (for RDRF) */
    byte TCIE        :1;                                       /* Transmission Complete Interrupt Enable (for TC) */
    byte TIE         :1;                                       /* Transmit Interrupt Enable (for TDRE) */
  } Bits;
} SCI1C2STR;
extern volatile SCI1C2STR _SCI1C2 @0x0000001B;
#define SCI1C2                          _SCI1C2.Byte
#define SCI1C2_SBK                      _SCI1C2.Bits.SBK
#define SCI1C2_RWU                      _SCI1C2.Bits.RWU
#define SCI1C2_RE                       _SCI1C2.Bits.RE
#define SCI1C2_TE                       _SCI1C2.Bits.TE
#define SCI1C2_ILIE                     _SCI1C2.Bits.ILIE
#define SCI1C2_RIE                      _SCI1C2.Bits.RIE
#define SCI1C2_TCIE                     _SCI1C2.Bits.TCIE
#define SCI1C2_TIE                      _SCI1C2.Bits.TIE

#define SCI1C2_SBK_MASK                 1
#define SCI1C2_RWU_MASK                 2
#define SCI1C2_RE_MASK                  4
#define SCI1C2_TE_MASK                  8
#define SCI1C2_ILIE_MASK                16
#define SCI1C2_RIE_MASK                 32
#define SCI1C2_TCIE_MASK                64
#define SCI1C2_TIE_MASK                 128


/*** SCI1S1 - SCI1 Status Register 1; 0x0000001C ***/
typedef union {
  byte Byte;
  struct {
    byte PF          :1;                                       /* Parity Error Flag */
    byte FE          :1;                                       /* Framing Error Flag */
    byte NF          :1;                                       /* Noise Flag */
    byte OR          :1;                                       /* Receiver Overrun Flag */
    byte IDLE        :1;                                       /* Idle Line Flag */
    byte RDRF        :1;                                       /* Receive Data Register Full Flag */
    byte TC          :1;                                       /* Transmission Complete Flag */
    byte TDRE        :1;                                       /* Transmit Data Register Empty Flag */
  } Bits;
} SCI1S1STR;
extern volatile SCI1S1STR _SCI1S1 @0x0000001C;
#define SCI1S1                          _SCI1S1.Byte
#define SCI1S1_PF                       _SCI1S1.Bits.PF
#define SCI1S1_FE                       _SCI1S1.Bits.FE
#define SCI1S1_NF                       _SCI1S1.Bits.NF
#define SCI1S1_OR                       _SCI1S1.Bits.OR
#define SCI1S1_IDLE                     _SCI1S1.Bits.IDLE
#define SCI1S1_RDRF                     _SCI1S1.Bits.RDRF
#define SCI1S1_TC                       _SCI1S1.Bits.TC
#define SCI1S1_TDRE                     _SCI1S1.Bits.TDRE

#define SCI1S1_PF_MASK                  1
#define SCI1S1_FE_MASK                  2
#define SCI1S1_NF_MASK                  4
#define SCI1S1_OR_MASK                  8
#define SCI1S1_IDLE_MASK                16
#define SCI1S1_RDRF_MASK                32
#define SCI1S1_TC_MASK                  64
#define SCI1S1_TDRE_MASK                128


/*** SCI1S2 - SCI1 Status Register 2; 0x0000001D ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SCI1S2STR;
extern volatile SCI1S2STR _SCI1S2 @0x0000001D;
#define SCI1S2                          _SCI1S2.Byte
#define SCI1S2_RAF                      _SCI1S2.Bits.RAF

#define SCI1S2_RAF_MASK                 1


/*** SCI1C3 - SCI1 Control Register 3; 0x0000001E ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte             :1; 
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI1C3STR;
extern volatile SCI1C3STR _SCI1C3 @0x0000001E;
#define SCI1C3                          _SCI1C3.Byte
#define SCI1C3_PEIE                     _SCI1C3.Bits.PEIE
#define SCI1C3_FEIE                     _SCI1C3.Bits.FEIE
#define SCI1C3_NEIE                     _SCI1C3.Bits.NEIE
#define SCI1C3_ORIE                     _SCI1C3.Bits.ORIE
#define SCI1C3_TXDIR                    _SCI1C3.Bits.TXDIR
#define SCI1C3_T8                       _SCI1C3.Bits.T8
#define SCI1C3_R8                       _SCI1C3.Bits.R8

#define SCI1C3_PEIE_MASK                1
#define SCI1C3_FEIE_MASK                2
#define SCI1C3_NEIE_MASK                4
#define SCI1C3_ORIE_MASK                8
#define SCI1C3_TXDIR_MASK               32
#define SCI1C3_T8_MASK                  64
#define SCI1C3_R8_MASK                  128


/*** SCI1D - SCI1 Data Register; 0x0000001F ***/
typedef union {
  byte Byte;
  struct {
    byte R0_T0       :1;                                       /* Receive/Transmit Data Bit 0 */
    byte R1_T1       :1;                                       /* Receive/Transmit Data Bit 1 */
    byte R2_T2       :1;                                       /* Receive/Transmit Data Bit 2 */
    byte R3_T3       :1;                                       /* Receive/Transmit Data Bit 3 */
    byte R4_T4       :1;                                       /* Receive/Transmit Data Bit 4 */
    byte R5_T5       :1;                                       /* Receive/Transmit Data Bit 5 */
    byte R6_T6       :1;                                       /* Receive/Transmit Data Bit 6 */
    byte R7_T7       :1;                                       /* Receive/Transmit Data Bit 7 */
  } Bits;
} SCI1DSTR;
extern volatile SCI1DSTR _SCI1D @0x0000001F;
#define SCI1D                           _SCI1D.Byte
#define SCI1D_R0_T0                     _SCI1D.Bits.R0_T0
#define SCI1D_R1_T1                     _SCI1D.Bits.R1_T1
#define SCI1D_R2_T2                     _SCI1D.Bits.R2_T2
#define SCI1D_R3_T3                     _SCI1D.Bits.R3_T3
#define SCI1D_R4_T4                     _SCI1D.Bits.R4_T4
#define SCI1D_R5_T5                     _SCI1D.Bits.R5_T5
#define SCI1D_R6_T6                     _SCI1D.Bits.R6_T6
#define SCI1D_R7_T7                     _SCI1D.Bits.R7_T7

#define SCI1D_R0_T0_MASK                1
#define SCI1D_R1_T1_MASK                2
#define SCI1D_R2_T2_MASK                4
#define SCI1D_R3_T3_MASK                8
#define SCI1D_R4_T4_MASK                16
#define SCI1D_R5_T5_MASK                32
#define SCI1D_R6_T6_MASK                64
#define SCI1D_R7_T7_MASK                128


/*** SCI2BD - SCI2 Baud Rate Register; 0x00000020 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI2BDH - SCI2 Baud Rate Register High; 0x00000020 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpSBR_8 :5;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SCI2BDHSTR;
    #define SCI2BDH                     _SCI2BD.Overlap_STR.SCI2BDHSTR.Byte
    #define SCI2BDH_SBR8                _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR8
    #define SCI2BDH_SBR9                _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR9
    #define SCI2BDH_SBR10               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR10
    #define SCI2BDH_SBR11               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR11
    #define SCI2BDH_SBR12               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR12
    #define SCI2BDH_SBR_8               _SCI2BD.Overlap_STR.SCI2BDHSTR.MergedBits.grpSBR_8
    #define SCI2BDH_SBR                 SCI2BDH_SBR_8
    
    #define SCI2BDH_SBR8_MASK           1
    #define SCI2BDH_SBR9_MASK           2
    #define SCI2BDH_SBR10_MASK          4
    #define SCI2BDH_SBR11_MASK          8
    #define SCI2BDH_SBR12_MASK          16
    #define SCI2BDH_SBR_8_MASK          31
    #define SCI2BDH_SBR_8_BITNUM        0
    

    /*** SCI2BDL - SCI2 Baud Rate Register Low; 0x00000021 ***/
    union {
      byte Byte;
      struct {
        byte SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
        byte SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
        byte SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
        byte SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
        byte SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
        byte SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
        byte SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
        byte SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
      } Bits;
    } SCI2BDLSTR;
    #define SCI2BDL                     _SCI2BD.Overlap_STR.SCI2BDLSTR.Byte
    #define SCI2BDL_SBR0                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR0
    #define SCI2BDL_SBR1                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR1
    #define SCI2BDL_SBR2                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR2
    #define SCI2BDL_SBR3                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR3
    #define SCI2BDL_SBR4                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR4
    #define SCI2BDL_SBR5                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR5
    #define SCI2BDL_SBR6                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR6
    #define SCI2BDL_SBR7                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR7
    
    #define SCI2BDL_SBR0_MASK           1
    #define SCI2BDL_SBR1_MASK           2
    #define SCI2BDL_SBR2_MASK           4
    #define SCI2BDL_SBR3_MASK           8
    #define SCI2BDL_SBR4_MASK           16
    #define SCI2BDL_SBR5_MASK           32
    #define SCI2BDL_SBR6_MASK           64
    #define SCI2BDL_SBR7_MASK           128
    
  } Overlap_STR;

} SCI2BDSTR;
extern volatile SCI2BDSTR _SCI2BD @0x00000020;
#define SCI2BD                          _SCI2BD.Word


/*** SCI2C1 - SCI1 Control Register 1; 0x00000022 ***/
typedef union {
  byte Byte;
  struct {
    byte PT          :1;                                       /* Parity Type */
    byte PE          :1;                                       /* Parity Enable */
    byte ILT         :1;                                       /* Idle Line Type Select */
    byte WAKE        :1;                                       /* Receiver Wakeup Method Select */
    byte M           :1;                                       /* 9-Bit or 8-Bit Mode Select */
    byte RSRC        :1;                                       /* Receiver Source Select */
    byte SCISWAI     :1;                                       /* SCI Stops in Wait Mode */
    byte LOOPS       :1;                                       /* Loop Mode Select */
  } Bits;
} SCI2C1STR;
extern volatile SCI2C1STR _SCI2C1 @0x00000022;
#define SCI2C1                          _SCI2C1.Byte
#define SCI2C1_PT                       _SCI2C1.Bits.PT
#define SCI2C1_PE                       _SCI2C1.Bits.PE
#define SCI2C1_ILT                      _SCI2C1.Bits.ILT
#define SCI2C1_WAKE                     _SCI2C1.Bits.WAKE
#define SCI2C1_M                        _SCI2C1.Bits.M
#define SCI2C1_RSRC                     _SCI2C1.Bits.RSRC
#define SCI2C1_SCISWAI                  _SCI2C1.Bits.SCISWAI
#define SCI2C1_LOOPS                    _SCI2C1.Bits.LOOPS

#define SCI2C1_PT_MASK                  1
#define SCI2C1_PE_MASK                  2
#define SCI2C1_ILT_MASK                 4
#define SCI2C1_WAKE_MASK                8
#define SCI2C1_M_MASK                   16
#define SCI2C1_RSRC_MASK                32
#define SCI2C1_SCISWAI_MASK             64
#define SCI2C1_LOOPS_MASK               128


/*** SCI2C2 - SCI2 Control Register 2; 0x00000023 ***/
typedef union {
  byte Byte;
  struct {
    byte SBK         :1;                                       /* Send Break */
    byte RWU         :1;                                       /* Receiver Wakeup Control */
    byte RE          :1;                                       /* Receiver Enable */
    byte TE          :1;                                       /* Transmitter Enable */
    byte ILIE        :1;                                       /* Idle Line Interrupt Enable (for IDLE) */
    byte RIE         :1;                                       /* Receiver Interrupt Enable (for RDRF) */
    byte TCIE        :1;                                       /* Transmission Complete Interrupt Enable (for TC) */
    byte TIE         :1;                                       /* Transmit Interrupt Enable (for TDRE) */
  } Bits;
} SCI2C2STR;
extern volatile SCI2C2STR _SCI2C2 @0x00000023;
#define SCI2C2                          _SCI2C2.Byte
#define SCI2C2_SBK                      _SCI2C2.Bits.SBK
#define SCI2C2_RWU                      _SCI2C2.Bits.RWU
#define SCI2C2_RE                       _SCI2C2.Bits.RE
#define SCI2C2_TE                       _SCI2C2.Bits.TE
#define SCI2C2_ILIE                     _SCI2C2.Bits.ILIE
#define SCI2C2_RIE                      _SCI2C2.Bits.RIE
#define SCI2C2_TCIE                     _SCI2C2.Bits.TCIE
#define SCI2C2_TIE                      _SCI2C2.Bits.TIE

#define SCI2C2_SBK_MASK                 1
#define SCI2C2_RWU_MASK                 2
#define SCI2C2_RE_MASK                  4
#define SCI2C2_TE_MASK                  8
#define SCI2C2_ILIE_MASK                16
#define SCI2C2_RIE_MASK                 32
#define SCI2C2_TCIE_MASK                64
#define SCI2C2_TIE_MASK                 128


/*** SCI2S1 - SCI2 Status Register 1; 0x00000024 ***/
typedef union {
  byte Byte;
  struct {
    byte PF          :1;                                       /* Parity Error Flag */
    byte FE          :1;                                       /* Framing Error Flag */
    byte NF          :1;                                       /* Noise Flag */
    byte OR          :1;                                       /* Receiver Overrun Flag */
    byte IDLE        :1;                                       /* Idle Line Flag */
    byte RDRF        :1;                                       /* Receive Data Register Full Flag */
    byte TC          :1;                                       /* Transmission Complete Flag */
    byte TDRE        :1;                                       /* Transmit Data Register Empty Flag */
  } Bits;
} SCI2S1STR;
extern volatile SCI2S1STR _SCI2S1 @0x00000024;
#define SCI2S1                          _SCI2S1.Byte
#define SCI2S1_PF                       _SCI2S1.Bits.PF
#define SCI2S1_FE                       _SCI2S1.Bits.FE
#define SCI2S1_NF                       _SCI2S1.Bits.NF
#define SCI2S1_OR                       _SCI2S1.Bits.OR
#define SCI2S1_IDLE                     _SCI2S1.Bits.IDLE
#define SCI2S1_RDRF                     _SCI2S1.Bits.RDRF
#define SCI2S1_TC                       _SCI2S1.Bits.TC
#define SCI2S1_TDRE                     _SCI2S1.Bits.TDRE

#define SCI2S1_PF_MASK                  1
#define SCI2S1_FE_MASK                  2
#define SCI2S1_NF_MASK                  4
#define SCI2S1_OR_MASK                  8
#define SCI2S1_IDLE_MASK                16
#define SCI2S1_RDRF_MASK                32
#define SCI2S1_TC_MASK                  64
#define SCI2S1_TDRE_MASK                128


/*** SCI2S2 - SCI2 Status Register 2; 0x00000025 ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SCI2S2STR;
extern volatile SCI2S2STR _SCI2S2 @0x00000025;
#define SCI2S2                          _SCI2S2.Byte
#define SCI2S2_RAF                      _SCI2S2.Bits.RAF

#define SCI2S2_RAF_MASK                 1


/*** SCI2C3 - SCI2 Control Register 3; 0x00000026 ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte             :1; 
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI2C3STR;
extern volatile SCI2C3STR _SCI2C3 @0x00000026;
#define SCI2C3                          _SCI2C3.Byte
#define SCI2C3_PEIE                     _SCI2C3.Bits.PEIE
#define SCI2C3_FEIE                     _SCI2C3.Bits.FEIE
#define SCI2C3_NEIE                     _SCI2C3.Bits.NEIE
#define SCI2C3_ORIE                     _SCI2C3.Bits.ORIE
#define SCI2C3_TXDIR                    _SCI2C3.Bits.TXDIR
#define SCI2C3_T8                       _SCI2C3.Bits.T8
#define SCI2C3_R8                       _SCI2C3.Bits.R8

#define SCI2C3_PEIE_MASK                1
#define SCI2C3_FEIE_MASK                2
#define SCI2C3_NEIE_MASK                4
#define SCI2C3_ORIE_MASK                8
#define SCI2C3_TXDIR_MASK               32
#define SCI2C3_T8_MASK                  64
#define SCI2C3_R8_MASK                  128


/*** SCI2D - SCI2 Data Register; 0x00000027 ***/
typedef union {
  byte Byte;
  struct {
    byte R0_T0       :1;                                       /* Receive/Transmit Data Bit 0 */
    byte R1_T1       :1;                                       /* Receive/Transmit Data Bit 1 */
    byte R2_T2       :1;                                       /* Receive/Transmit Data Bit 2 */
    byte R3_T3       :1;                                       /* Receive/Transmit Data Bit 3 */
    byte R4_T4       :1;                                       /* Receive/Transmit Data Bit 4 */
    byte R5_T5       :1;                                       /* Receive/Transmit Data Bit 5 */
    byte R6_T6       :1;                                       /* Receive/Transmit Data Bit 6 */
    byte R7_T7       :1;                                       /* Receive/Transmit Data Bit 7 */
  } Bits;
} SCI2DSTR;
extern volatile SCI2DSTR _SCI2D @0x00000027;
#define SCI2D                           _SCI2D.Byte
#define SCI2D_R0_T0                     _SCI2D.Bits.R0_T0
#define SCI2D_R1_T1                     _SCI2D.Bits.R1_T1
#define SCI2D_R2_T2                     _SCI2D.Bits.R2_T2
#define SCI2D_R3_T3                     _SCI2D.Bits.R3_T3
#define SCI2D_R4_T4                     _SCI2D.Bits.R4_T4
#define SCI2D_R5_T5                     _SCI2D.Bits.R5_T5
#define SCI2D_R6_T6                     _SCI2D.Bits.R6_T6
#define SCI2D_R7_T7                     _SCI2D.Bits.R7_T7

#define SCI2D_R0_T0_MASK                1
#define SCI2D_R1_T1_MASK                2
#define SCI2D_R2_T2_MASK                4
#define SCI2D_R3_T3_MASK                8
#define SCI2D_R4_T4_MASK                16
#define SCI2D_R5_T5_MASK                32
#define SCI2D_R6_T6_MASK                64
#define SCI2D_R7_T7_MASK                128


/*** SPI1C1 - SPI1 Control Register 1; 0x00000028 ***/
typedef union {
  byte Byte;
  struct {
    byte LSBFE       :1;                                       /* LSB First (shifter direction) */
    byte SSOE        :1;                                       /* Slave Select Output Enable */
    byte CPHA        :1;                                       /* Clock Phase */
    byte CPOL        :1;                                       /* Clock Polarity */
    byte MSTR        :1;                                       /* Master/Slave Mode Select */
    byte SPTIE       :1;                                       /* SPI1 Transmit Interrupt Enable */
    byte SPE         :1;                                       /* SPI1 System Enable */
    byte SPIE        :1;                                       /* SPI1 Interrupt Enable */
  } Bits;
} SPI1C1STR;
extern volatile SPI1C1STR _SPI1C1 @0x00000028;
#define SPI1C1                          _SPI1C1.Byte
#define SPI1C1_LSBFE                    _SPI1C1.Bits.LSBFE
#define SPI1C1_SSOE                     _SPI1C1.Bits.SSOE
#define SPI1C1_CPHA                     _SPI1C1.Bits.CPHA
#define SPI1C1_CPOL                     _SPI1C1.Bits.CPOL
#define SPI1C1_MSTR                     _SPI1C1.Bits.MSTR
#define SPI1C1_SPTIE                    _SPI1C1.Bits.SPTIE
#define SPI1C1_SPE                      _SPI1C1.Bits.SPE
#define SPI1C1_SPIE                     _SPI1C1.Bits.SPIE

#define SPI1C1_LSBFE_MASK               1
#define SPI1C1_SSOE_MASK                2
#define SPI1C1_CPHA_MASK                4
#define SPI1C1_CPOL_MASK                8
#define SPI1C1_MSTR_MASK                16
#define SPI1C1_SPTIE_MASK               32
#define SPI1C1_SPE_MASK                 64
#define SPI1C1_SPIE_MASK                128


/*** SPI1C2 - SPI1 Control Register 2; 0x00000029 ***/
typedef union {
  byte Byte;
  struct {
    byte SPC0        :1;                                       /* SPI1 Pin Control 0 */
    byte SPISWAI     :1;                                       /* SPI1 Stop in Wait Mode */
    byte             :1; 
    byte BIDIROE     :1;                                       /* Bidirectional Mode Output Enable */
    byte MODFEN      :1;                                       /* Master Mode-Fault Function Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SPI1C2STR;
extern volatile SPI1C2STR _SPI1C2 @0x00000029;
#define SPI1C2                          _SPI1C2.Byte
#define SPI1C2_SPC0                     _SPI1C2.Bits.SPC0
#define SPI1C2_SPISWAI                  _SPI1C2.Bits.SPISWAI
#define SPI1C2_BIDIROE                  _SPI1C2.Bits.BIDIROE
#define SPI1C2_MODFEN                   _SPI1C2.Bits.MODFEN

#define SPI1C2_SPC0_MASK                1
#define SPI1C2_SPISWAI_MASK             2
#define SPI1C2_BIDIROE_MASK             8
#define SPI1C2_MODFEN_MASK              16


/*** SPI1BR - SPI1 Baud Rate Register; 0x0000002A ***/
typedef union {
  byte Byte;
  struct {
    byte SPR0        :1;                                       /* SPI1 Baud Rate Divisor Bit 0 */
    byte SPR1        :1;                                       /* SPI1 Baud Rate Divisor Bit 1 */
    byte SPR2        :1;                                       /* SPI1 Baud Rate Divisor Bit 2 */
    byte             :1; 
    byte SPPR0       :1;                                       /* SPI1 Baud Rate Prescale Divisor Bit 0 */
    byte SPPR1       :1;                                       /* SPI1 Baud Rate Prescale Divisor Bit 1 */
    byte SPPR2       :1;                                       /* SPI1 Baud Rate Prescale Divisor Bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpSPR  :3;
    byte         :1;
    byte grpSPPR :3;
    byte         :1;
  } MergedBits;
} SPI1BRSTR;
extern volatile SPI1BRSTR _SPI1BR @0x0000002A;
#define SPI1BR                          _SPI1BR.Byte
#define SPI1BR_SPR0                     _SPI1BR.Bits.SPR0
#define SPI1BR_SPR1                     _SPI1BR.Bits.SPR1
#define SPI1BR_SPR2                     _SPI1BR.Bits.SPR2
#define SPI1BR_SPPR0                    _SPI1BR.Bits.SPPR0
#define SPI1BR_SPPR1                    _SPI1BR.Bits.SPPR1
#define SPI1BR_SPPR2                    _SPI1BR.Bits.SPPR2
#define SPI1BR_SPR                      _SPI1BR.MergedBits.grpSPR
#define SPI1BR_SPPR                     _SPI1BR.MergedBits.grpSPPR

#define SPI1BR_SPR0_MASK                1
#define SPI1BR_SPR1_MASK                2
#define SPI1BR_SPR2_MASK                4
#define SPI1BR_SPPR0_MASK               16
#define SPI1BR_SPPR1_MASK               32
#define SPI1BR_SPPR2_MASK               64
#define SPI1BR_SPR_MASK                 7
#define SPI1BR_SPR_BITNUM               0
#define SPI1BR_SPPR_MASK                112
#define SPI1BR_SPPR_BITNUM              4


/*** SPI1S - SPI1 Status Register; 0x0000002B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte MODF        :1;                                       /* Master Mode Fault Flag */
    byte SPTEF       :1;                                       /* SPI1 Transmit Buffer Empty Flag */
    byte             :1; 
    byte SPRF        :1;                                       /* SPI1 Read Buffer Full Flag */
  } Bits;
} SPI1SSTR;
extern volatile SPI1SSTR _SPI1S @0x0000002B;
#define SPI1S                           _SPI1S.Byte
#define SPI1S_MODF                      _SPI1S.Bits.MODF
#define SPI1S_SPTEF                     _SPI1S.Bits.SPTEF
#define SPI1S_SPRF                      _SPI1S.Bits.SPRF

#define SPI1S_MODF_MASK                 16
#define SPI1S_SPTEF_MASK                32
#define SPI1S_SPRF_MASK                 128


/*** SPI1D - SPI1 Data Register; 0x0000002D ***/
typedef union {
  byte Byte;
} SPI1DSTR;
extern volatile SPI1DSTR _SPI1D @0x0000002D;
#define SPI1D                           _SPI1D.Byte


/*** TPM1SC - TPM 1 Status and Control Register; 0x00000030 ***/
typedef union {
  byte Byte;
  struct {
    byte PS0         :1;                                       /* Prescale Divisor Select Bit 0 */
    byte PS1         :1;                                       /* Prescale Divisor Select Bit 1 */
    byte PS2         :1;                                       /* Prescale Divisor Select Bit 2 */
    byte CLKSA       :1;                                       /* Clock Source Select A */
    byte CLKSB       :1;                                       /* Clock Source Select B */
    byte CPWMS       :1;                                       /* Center-Aligned PWM Select */
    byte TOIE        :1;                                       /* Timer Overflow Interrupt Enable */
    byte TOF         :1;                                       /* Timer Overflow Flag */
  } Bits;
  struct {
    byte grpPS   :3;
    byte grpCLKSx :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1SCSTR;
extern volatile TPM1SCSTR _TPM1SC @0x00000030;
#define TPM1SC                          _TPM1SC.Byte
#define TPM1SC_PS0                      _TPM1SC.Bits.PS0
#define TPM1SC_PS1                      _TPM1SC.Bits.PS1
#define TPM1SC_PS2                      _TPM1SC.Bits.PS2
#define TPM1SC_CLKSA                    _TPM1SC.Bits.CLKSA
#define TPM1SC_CLKSB                    _TPM1SC.Bits.CLKSB
#define TPM1SC_CPWMS                    _TPM1SC.Bits.CPWMS
#define TPM1SC_TOIE                     _TPM1SC.Bits.TOIE
#define TPM1SC_TOF                      _TPM1SC.Bits.TOF
#define TPM1SC_PS                       _TPM1SC.MergedBits.grpPS
#define TPM1SC_CLKSx                    _TPM1SC.MergedBits.grpCLKSx

#define TPM1SC_PS0_MASK                 1
#define TPM1SC_PS1_MASK                 2
#define TPM1SC_PS2_MASK                 4
#define TPM1SC_CLKSA_MASK               8
#define TPM1SC_CLKSB_MASK               16
#define TPM1SC_CPWMS_MASK               32
#define TPM1SC_TOIE_MASK                64
#define TPM1SC_TOF_MASK                 128
#define TPM1SC_PS_MASK                  7
#define TPM1SC_PS_BITNUM                0
#define TPM1SC_CLKSx_MASK               24
#define TPM1SC_CLKSx_BITNUM             3


/*** TPM1CNT - TPM 1 Counter Register; 0x00000031 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1CNTH - TPM 1 Counter Register High; 0x00000031 ***/
    union {
      byte Byte;
    } TPM1CNTHSTR;
    #define TPM1CNTH                    _TPM1CNT.Overlap_STR.TPM1CNTHSTR.Byte
    

    /*** TPM1CNTL - TPM 1 Counter Register Low; 0x00000032 ***/
    union {
      byte Byte;
    } TPM1CNTLSTR;
    #define TPM1CNTL                    _TPM1CNT.Overlap_STR.TPM1CNTLSTR.Byte
    
  } Overlap_STR;

} TPM1CNTSTR;
extern volatile TPM1CNTSTR _TPM1CNT @0x00000031;
#define TPM1CNT                         _TPM1CNT.Word


/*** TPM1MOD - TPM 1 Timer Counter Modulo Register; 0x00000033 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1MODH - TPM 1 Timer Counter Modulo Register High; 0x00000033 ***/
    union {
      byte Byte;
    } TPM1MODHSTR;
    #define TPM1MODH                    _TPM1MOD.Overlap_STR.TPM1MODHSTR.Byte
    

    /*** TPM1MODL - TPM 1 Timer Counter Modulo Register Low; 0x00000034 ***/
    union {
      byte Byte;
    } TPM1MODLSTR;
    #define TPM1MODL                    _TPM1MOD.Overlap_STR.TPM1MODLSTR.Byte
    
  } Overlap_STR;

} TPM1MODSTR;
extern volatile TPM1MODSTR _TPM1MOD @0x00000033;
#define TPM1MOD                         _TPM1MOD.Word


/*** TPM1C0SC - TPM 1 Timer Channel 0 Status and Control Register; 0x00000035 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for TPM Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for TPM Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C0SCSTR;
extern volatile TPM1C0SCSTR _TPM1C0SC @0x00000035;
#define TPM1C0SC                        _TPM1C0SC.Byte
#define TPM1C0SC_ELS0A                  _TPM1C0SC.Bits.ELS0A
#define TPM1C0SC_ELS0B                  _TPM1C0SC.Bits.ELS0B
#define TPM1C0SC_MS0A                   _TPM1C0SC.Bits.MS0A
#define TPM1C0SC_MS0B                   _TPM1C0SC.Bits.MS0B
#define TPM1C0SC_CH0IE                  _TPM1C0SC.Bits.CH0IE
#define TPM1C0SC_CH0F                   _TPM1C0SC.Bits.CH0F
#define TPM1C0SC_ELS0x                  _TPM1C0SC.MergedBits.grpELS0x
#define TPM1C0SC_MS0x                   _TPM1C0SC.MergedBits.grpMS0x

#define TPM1C0SC_ELS0A_MASK             4
#define TPM1C0SC_ELS0B_MASK             8
#define TPM1C0SC_MS0A_MASK              16
#define TPM1C0SC_MS0B_MASK              32
#define TPM1C0SC_CH0IE_MASK             64
#define TPM1C0SC_CH0F_MASK              128
#define TPM1C0SC_ELS0x_MASK             12
#define TPM1C0SC_ELS0x_BITNUM           2
#define TPM1C0SC_MS0x_MASK              48
#define TPM1C0SC_MS0x_BITNUM            4


/*** TPM1C0V - TPM 1 Timer Channel 0 Value Register; 0x00000036 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C0VH - TPM 1 Timer Channel 0 Value Register High; 0x00000036 ***/
    union {
      byte Byte;
    } TPM1C0VHSTR;
    #define TPM1C0VH                    _TPM1C0V.Overlap_STR.TPM1C0VHSTR.Byte
    

    /*** TPM1C0VL - TPM 1 Timer Channel 0 Value Register Low; 0x00000037 ***/
    union {
      byte Byte;
    } TPM1C0VLSTR;
    #define TPM1C0VL                    _TPM1C0V.Overlap_STR.TPM1C0VLSTR.Byte
    
  } Overlap_STR;

} TPM1C0VSTR;
extern volatile TPM1C0VSTR _TPM1C0V @0x00000036;
#define TPM1C0V                         _TPM1C0V.Word


/*** TPM1C1SC - TPM 1 Timer Channel 1 Status and Control Register; 0x00000038 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for TPM Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for TPM Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C1SCSTR;
extern volatile TPM1C1SCSTR _TPM1C1SC @0x00000038;
#define TPM1C1SC                        _TPM1C1SC.Byte
#define TPM1C1SC_ELS1A                  _TPM1C1SC.Bits.ELS1A
#define TPM1C1SC_ELS1B                  _TPM1C1SC.Bits.ELS1B
#define TPM1C1SC_MS1A                   _TPM1C1SC.Bits.MS1A
#define TPM1C1SC_MS1B                   _TPM1C1SC.Bits.MS1B
#define TPM1C1SC_CH1IE                  _TPM1C1SC.Bits.CH1IE
#define TPM1C1SC_CH1F                   _TPM1C1SC.Bits.CH1F
#define TPM1C1SC_ELS1x                  _TPM1C1SC.MergedBits.grpELS1x
#define TPM1C1SC_MS1x                   _TPM1C1SC.MergedBits.grpMS1x

#define TPM1C1SC_ELS1A_MASK             4
#define TPM1C1SC_ELS1B_MASK             8
#define TPM1C1SC_MS1A_MASK              16
#define TPM1C1SC_MS1B_MASK              32
#define TPM1C1SC_CH1IE_MASK             64
#define TPM1C1SC_CH1F_MASK              128
#define TPM1C1SC_ELS1x_MASK             12
#define TPM1C1SC_ELS1x_BITNUM           2
#define TPM1C1SC_MS1x_MASK              48
#define TPM1C1SC_MS1x_BITNUM            4


/*** TPM1C1V - TPM 1 Timer Channel 1 Value Register; 0x00000039 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C1VH - TPM 1 Timer Channel 1 Value Register High; 0x00000039 ***/
    union {
      byte Byte;
    } TPM1C1VHSTR;
    #define TPM1C1VH                    _TPM1C1V.Overlap_STR.TPM1C1VHSTR.Byte
    

    /*** TPM1C1VL - TPM 1 Timer Channel 1 Value Register Low; 0x0000003A ***/
    union {
      byte Byte;
    } TPM1C1VLSTR;
    #define TPM1C1VL                    _TPM1C1V.Overlap_STR.TPM1C1VLSTR.Byte
    
  } Overlap_STR;

} TPM1C1VSTR;
extern volatile TPM1C1VSTR _TPM1C1V @0x00000039;
#define TPM1C1V                         _TPM1C1V.Word


/*** TPM1C2SC - TPM 1 Timer Channel 2 Status and Control Register; 0x0000003B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS2A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS2B       :1;                                       /* Edge/Level Select Bit B */
    byte MS2A        :1;                                       /* Mode Select A for TPM Channel 2 */
    byte MS2B        :1;                                       /* Mode Select B for TPM Channel 2 */
    byte CH2IE       :1;                                       /* Channel 2 Interrupt Enable */
    byte CH2F        :1;                                       /* Channel 2 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS2x :2;
    byte grpMS2x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C2SCSTR;
extern volatile TPM1C2SCSTR _TPM1C2SC @0x0000003B;
#define TPM1C2SC                        _TPM1C2SC.Byte
#define TPM1C2SC_ELS2A                  _TPM1C2SC.Bits.ELS2A
#define TPM1C2SC_ELS2B                  _TPM1C2SC.Bits.ELS2B
#define TPM1C2SC_MS2A                   _TPM1C2SC.Bits.MS2A
#define TPM1C2SC_MS2B                   _TPM1C2SC.Bits.MS2B
#define TPM1C2SC_CH2IE                  _TPM1C2SC.Bits.CH2IE
#define TPM1C2SC_CH2F                   _TPM1C2SC.Bits.CH2F
#define TPM1C2SC_ELS2x                  _TPM1C2SC.MergedBits.grpELS2x
#define TPM1C2SC_MS2x                   _TPM1C2SC.MergedBits.grpMS2x

#define TPM1C2SC_ELS2A_MASK             4
#define TPM1C2SC_ELS2B_MASK             8
#define TPM1C2SC_MS2A_MASK              16
#define TPM1C2SC_MS2B_MASK              32
#define TPM1C2SC_CH2IE_MASK             64
#define TPM1C2SC_CH2F_MASK              128
#define TPM1C2SC_ELS2x_MASK             12
#define TPM1C2SC_ELS2x_BITNUM           2
#define TPM1C2SC_MS2x_MASK              48
#define TPM1C2SC_MS2x_BITNUM            4


/*** TPM1C2V - TPM 1 Timer Channel 2 Value Register; 0x0000003C ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C2VH - TPM 1 Timer Channel 2 Value Register High; 0x0000003C ***/
    union {
      byte Byte;
    } TPM1C2VHSTR;
    #define TPM1C2VH                    _TPM1C2V.Overlap_STR.TPM1C2VHSTR.Byte
    

    /*** TPM1C2VL - TPM 1 Timer Channel 2 Value Register Low; 0x0000003D ***/
    union {
      byte Byte;
    } TPM1C2VLSTR;
    #define TPM1C2VL                    _TPM1C2V.Overlap_STR.TPM1C2VLSTR.Byte
    
  } Overlap_STR;

} TPM1C2VSTR;
extern volatile TPM1C2VSTR _TPM1C2V @0x0000003C;
#define TPM1C2V                         _TPM1C2V.Word


/*** PTGD - Port G Data Register; 0x00000044 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGD0       :1;                                       /* Port G Data Register Bit 0 */
    byte PTGD1       :1;                                       /* Port G Data Register Bit 1 */
    byte PTGD2       :1;                                       /* Port G Data Register Bit 2 */
    byte PTGD3       :1;                                       /* Port G Data Register Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGD :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDSTR;
extern volatile PTGDSTR _PTGD @0x00000044;
#define PTGD                            _PTGD.Byte
#define PTGD_PTGD0                      _PTGD.Bits.PTGD0
#define PTGD_PTGD1                      _PTGD.Bits.PTGD1
#define PTGD_PTGD2                      _PTGD.Bits.PTGD2
#define PTGD_PTGD3                      _PTGD.Bits.PTGD3
#define PTGD_PTGD                       _PTGD.MergedBits.grpPTGD

#define PTGD_PTGD0_MASK                 1
#define PTGD_PTGD1_MASK                 2
#define PTGD_PTGD2_MASK                 4
#define PTGD_PTGD3_MASK                 8
#define PTGD_PTGD_MASK                  15
#define PTGD_PTGD_BITNUM                0


/*** PTGPE - Pullup Enable for Port G; 0x00000045 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPE0      :1;                                       /* Pullup Enable for Port G Bit 0 */
    byte PTGPE1      :1;                                       /* Pullup Enable for Port G Bit 1 */
    byte PTGPE2      :1;                                       /* Pullup Enable for Port G Bit 2 */
    byte PTGPE3      :1;                                       /* Pullup Enable for Port G Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPE :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGPESTR;
extern volatile PTGPESTR _PTGPE @0x00000045;
#define PTGPE                           _PTGPE.Byte
#define PTGPE_PTGPE0                    _PTGPE.Bits.PTGPE0
#define PTGPE_PTGPE1                    _PTGPE.Bits.PTGPE1
#define PTGPE_PTGPE2                    _PTGPE.Bits.PTGPE2
#define PTGPE_PTGPE3                    _PTGPE.Bits.PTGPE3
#define PTGPE_PTGPE                     _PTGPE.MergedBits.grpPTGPE

#define PTGPE_PTGPE0_MASK               1
#define PTGPE_PTGPE1_MASK               2
#define PTGPE_PTGPE2_MASK               4
#define PTGPE_PTGPE3_MASK               8
#define PTGPE_PTGPE_MASK                15
#define PTGPE_PTGPE_BITNUM              0


/*** PTGSE - Slew Rate Control Enable for Port G; 0x00000046 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGSE0      :1;                                       /* Slew Rate Control Enable for Port G Bit 0 */
    byte PTGSE1      :1;                                       /* Slew Rate Control Enable for Port G Bit 1 */
    byte PTGSE2      :1;                                       /* Slew Rate Control Enable for Port G Bit 2 */
    byte PTGSE3      :1;                                       /* Slew Rate Control Enable for Port G Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGSE :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGSESTR;
extern volatile PTGSESTR _PTGSE @0x00000046;
#define PTGSE                           _PTGSE.Byte
#define PTGSE_PTGSE0                    _PTGSE.Bits.PTGSE0
#define PTGSE_PTGSE1                    _PTGSE.Bits.PTGSE1
#define PTGSE_PTGSE2                    _PTGSE.Bits.PTGSE2
#define PTGSE_PTGSE3                    _PTGSE.Bits.PTGSE3
#define PTGSE_PTGSE                     _PTGSE.MergedBits.grpPTGSE

#define PTGSE_PTGSE0_MASK               1
#define PTGSE_PTGSE1_MASK               2
#define PTGSE_PTGSE2_MASK               4
#define PTGSE_PTGSE3_MASK               8
#define PTGSE_PTGSE_MASK                15
#define PTGSE_PTGSE_BITNUM              0


/*** PTGDD - Data Direction Register G; 0x00000047 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDD0      :1;                                       /* Data Direction for Port G Bit 0 */
    byte PTGDD1      :1;                                       /* Data Direction for Port G Bit 1 */
    byte PTGDD2      :1;                                       /* Data Direction for Port G Bit 2 */
    byte PTGDD3      :1;                                       /* Data Direction for Port G Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDD :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDDSTR;
extern volatile PTGDDSTR _PTGDD @0x00000047;
#define PTGDD                           _PTGDD.Byte
#define PTGDD_PTGDD0                    _PTGDD.Bits.PTGDD0
#define PTGDD_PTGDD1                    _PTGDD.Bits.PTGDD1
#define PTGDD_PTGDD2                    _PTGDD.Bits.PTGDD2
#define PTGDD_PTGDD3                    _PTGDD.Bits.PTGDD3
#define PTGDD_PTGDD                     _PTGDD.MergedBits.grpPTGDD

#define PTGDD_PTGDD0_MASK               1
#define PTGDD_PTGDD1_MASK               2
#define PTGDD_PTGDD2_MASK               4
#define PTGDD_PTGDD3_MASK               8
#define PTGDD_PTGDD_MASK                15
#define PTGDD_PTGDD_BITNUM              0


/*** ICGC1 - ICG Control Register 1; 0x00000048 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte LOCD        :1;                                       /* Loss of Clock Disable */
    byte OSCSTEN     :1;                                       /* Enable Oscillator in Off Mode */
    byte CLKS0       :1;                                       /* Clock Mode Select Bit 0 */
    byte CLKS1       :1;                                       /* Clock Mode Select Bit 1 */
    byte REFS        :1;                                       /* External Reference Select */
    byte RANGE       :1;                                       /* Frequency Range Select */
    byte HGO         :1;                                       /* High Gain Oscillator Select */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpCLKS :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ICGC1STR;
extern volatile ICGC1STR _ICGC1 @0x00000048;
#define ICGC1                           _ICGC1.Byte
#define ICGC1_LOCD                      _ICGC1.Bits.LOCD
#define ICGC1_OSCSTEN                   _ICGC1.Bits.OSCSTEN
#define ICGC1_CLKS0                     _ICGC1.Bits.CLKS0
#define ICGC1_CLKS1                     _ICGC1.Bits.CLKS1
#define ICGC1_REFS                      _ICGC1.Bits.REFS
#define ICGC1_RANGE                     _ICGC1.Bits.RANGE
#define ICGC1_HGO                       _ICGC1.Bits.HGO
#define ICGC1_CLKS                      _ICGC1.MergedBits.grpCLKS

#define ICGC1_LOCD_MASK                 2
#define ICGC1_OSCSTEN_MASK              4
#define ICGC1_CLKS0_MASK                8
#define ICGC1_CLKS1_MASK                16
#define ICGC1_REFS_MASK                 32
#define ICGC1_RANGE_MASK                64
#define ICGC1_HGO_MASK                  128
#define ICGC1_CLKS_MASK                 24
#define ICGC1_CLKS_BITNUM               3


/*** ICGC2 - ICG Control Register 2; 0x00000049 ***/
typedef union {
  byte Byte;
  struct {
    byte RFD0        :1;                                       /* Reduced Frequency Divider Bit 0 */
    byte RFD1        :1;                                       /* Reduced Frequency Divider Bit 1 */
    byte RFD2        :1;                                       /* Reduced Frequency Divider Bit 2 */
    byte LOCRE       :1;                                       /* Loss of Clock Reset Enable */
    byte MFD0        :1;                                       /* Multiplication Factor Bit 0 */
    byte MFD1        :1;                                       /* Multiplication Factor Bit 1 */
    byte MFD2        :1;                                       /* Multiplication Factor Bit 2 */
    byte LOLRE       :1;                                       /* Loss of Lock Reset Enable */
  } Bits;
  struct {
    byte grpRFD  :3;
    byte         :1;
    byte grpMFD  :3;
    byte         :1;
  } MergedBits;
} ICGC2STR;
extern volatile ICGC2STR _ICGC2 @0x00000049;
#define ICGC2                           _ICGC2.Byte
#define ICGC2_RFD0                      _ICGC2.Bits.RFD0
#define ICGC2_RFD1                      _ICGC2.Bits.RFD1
#define ICGC2_RFD2                      _ICGC2.Bits.RFD2
#define ICGC2_LOCRE                     _ICGC2.Bits.LOCRE
#define ICGC2_MFD0                      _ICGC2.Bits.MFD0
#define ICGC2_MFD1                      _ICGC2.Bits.MFD1
#define ICGC2_MFD2                      _ICGC2.Bits.MFD2
#define ICGC2_LOLRE                     _ICGC2.Bits.LOLRE
#define ICGC2_RFD                       _ICGC2.MergedBits.grpRFD
#define ICGC2_MFD                       _ICGC2.MergedBits.grpMFD

#define ICGC2_RFD0_MASK                 1
#define ICGC2_RFD1_MASK                 2
#define ICGC2_RFD2_MASK                 4
#define ICGC2_LOCRE_MASK                8
#define ICGC2_MFD0_MASK                 16
#define ICGC2_MFD1_MASK                 32
#define ICGC2_MFD2_MASK                 64
#define ICGC2_LOLRE_MASK                128
#define ICGC2_RFD_MASK                  7
#define ICGC2_RFD_BITNUM                0
#define ICGC2_MFD_MASK                  112
#define ICGC2_MFD_BITNUM                4


/*** ICGS1 - ICG Status Register 1; 0x0000004A ***/
typedef union {
  byte Byte;
  struct {
    byte ICGIF       :1;                                       /* ICG Interrupt Flag */
    byte ERCS        :1;                                       /* External Reference Clock Status */
    byte LOCS        :1;                                       /* Loss Of Clock Status */
    byte LOCK        :1;                                       /* FLL Lock Status */
    byte LOLS        :1;                                       /* FLL Loss of Lock Status */
    byte REFST       :1;                                       /* Reference Clock Status */
    byte CLKST0      :1;                                       /* Clock Mode Status Bit 0 */
    byte CLKST1      :1;                                       /* Clock Mode Status Bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpCLKST :2;
  } MergedBits;
} ICGS1STR;
extern volatile ICGS1STR _ICGS1 @0x0000004A;
#define ICGS1                           _ICGS1.Byte
#define ICGS1_ICGIF                     _ICGS1.Bits.ICGIF
#define ICGS1_ERCS                      _ICGS1.Bits.ERCS
#define ICGS1_LOCS                      _ICGS1.Bits.LOCS
#define ICGS1_LOCK                      _ICGS1.Bits.LOCK
#define ICGS1_LOLS                      _ICGS1.Bits.LOLS
#define ICGS1_REFST                     _ICGS1.Bits.REFST
#define ICGS1_CLKST0                    _ICGS1.Bits.CLKST0
#define ICGS1_CLKST1                    _ICGS1.Bits.CLKST1
#define ICGS1_CLKST                     _ICGS1.MergedBits.grpCLKST

#define ICGS1_ICGIF_MASK                1
#define ICGS1_ERCS_MASK                 2
#define ICGS1_LOCS_MASK                 4
#define ICGS1_LOCK_MASK                 8
#define ICGS1_LOLS_MASK                 16
#define ICGS1_REFST_MASK                32
#define ICGS1_CLKST0_MASK               64
#define ICGS1_CLKST1_MASK               128
#define ICGS1_CLKST_MASK                192
#define ICGS1_CLKST_BITNUM              6


/*** ICGS2 - ICG Status Register 2; 0x0000004B ***/
typedef union {
  byte Byte;
  struct {
    byte DCOS        :1;                                       /* DCO Clock Stable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} ICGS2STR;
extern volatile ICGS2STR _ICGS2 @0x0000004B;
#define ICGS2                           _ICGS2.Byte
#define ICGS2_DCOS                      _ICGS2.Bits.DCOS

#define ICGS2_DCOS_MASK                 1


/*** ICGFLT - ICG Filter Register; 0x0000004C ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** ICGFLTU - ICG Upper Filter Register; 0x0000004C ***/
    union {
      byte Byte;
      struct {
        byte FILT8       :1;                                       /* ICG Filter Bit 8 */
        byte FILT9       :1;                                       /* ICG Filter Bit 9 */
        byte FILT10      :1;                                       /* ICG Filter Bit 10 */
        byte FILT11      :1;                                       /* ICG Filter Bit 11 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpFILT_8 :4;
        byte     :1;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } ICGFLTUSTR;
    #define ICGFLTU                     _ICGFLT.Overlap_STR.ICGFLTUSTR.Byte
    #define ICGFLTU_FILT8               _ICGFLT.Overlap_STR.ICGFLTUSTR.Bits.FILT8
    #define ICGFLTU_FILT9               _ICGFLT.Overlap_STR.ICGFLTUSTR.Bits.FILT9
    #define ICGFLTU_FILT10              _ICGFLT.Overlap_STR.ICGFLTUSTR.Bits.FILT10
    #define ICGFLTU_FILT11              _ICGFLT.Overlap_STR.ICGFLTUSTR.Bits.FILT11
    #define ICGFLTU_FILT_8              _ICGFLT.Overlap_STR.ICGFLTUSTR.MergedBits.grpFILT_8
    #define ICGFLTU_FILT                ICGFLTU_FILT_8
    
    #define ICGFLTU_FILT8_MASK          1
    #define ICGFLTU_FILT9_MASK          2
    #define ICGFLTU_FILT10_MASK         4
    #define ICGFLTU_FILT11_MASK         8
    #define ICGFLTU_FILT_8_MASK         15
    #define ICGFLTU_FILT_8_BITNUM       0
    

    /*** ICGFLTL - ICG Lower Filter Register; 0x0000004D ***/
    union {
      byte Byte;
      struct {
        byte FILT0       :1;                                       /* ICG Filter Bit 0 */
        byte FILT1       :1;                                       /* ICG Filter Bit 1 */
        byte FILT2       :1;                                       /* ICG Filter Bit 2 */
        byte FILT3       :1;                                       /* ICG Filter Bit 3 */
        byte FILT4       :1;                                       /* ICG Filter Bit 4 */
        byte FILT5       :1;                                       /* ICG Filter Bit 5 */
        byte FILT6       :1;                                       /* ICG Filter Bit 6 */
        byte FILT7       :1;                                       /* ICG Filter Bit 7 */
      } Bits;
    } ICGFLTLSTR;
    #define ICGFLTL                     _ICGFLT.Overlap_STR.ICGFLTLSTR.Byte
    #define ICGFLTL_FILT0               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT0
    #define ICGFLTL_FILT1               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT1
    #define ICGFLTL_FILT2               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT2
    #define ICGFLTL_FILT3               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT3
    #define ICGFLTL_FILT4               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT4
    #define ICGFLTL_FILT5               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT5
    #define ICGFLTL_FILT6               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT6
    #define ICGFLTL_FILT7               _ICGFLT.Overlap_STR.ICGFLTLSTR.Bits.FILT7
    
    #define ICGFLTL_FILT0_MASK          1
    #define ICGFLTL_FILT1_MASK          2
    #define ICGFLTL_FILT2_MASK          4
    #define ICGFLTL_FILT3_MASK          8
    #define ICGFLTL_FILT4_MASK          16
    #define ICGFLTL_FILT5_MASK          32
    #define ICGFLTL_FILT6_MASK          64
    #define ICGFLTL_FILT7_MASK          128
    
  } Overlap_STR;

} ICGFLTSTR;
extern volatile ICGFLTSTR _ICGFLT @0x0000004C;
#define ICGFLT                          _ICGFLT.Word


/*** ICGTRM - ICG Trim Register; 0x0000004E ***/
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* ICG Trim Bit 0 */
    byte TRIM1       :1;                                       /* ICG Trim Bit 1 */
    byte TRIM2       :1;                                       /* ICG Trim Bit 2 */
    byte TRIM3       :1;                                       /* ICG Trim Bit 3 */
    byte TRIM4       :1;                                       /* ICG Trim Bit 4 */
    byte TRIM5       :1;                                       /* ICG Trim Bit 5 */
    byte TRIM6       :1;                                       /* ICG Trim Bit 6 */
    byte TRIM7       :1;                                       /* ICG Trim Bit 7 */
  } Bits;
} ICGTRMSTR;
extern volatile ICGTRMSTR _ICGTRM @0x0000004E;
#define ICGTRM                          _ICGTRM.Byte
#define ICGTRM_TRIM0                    _ICGTRM.Bits.TRIM0
#define ICGTRM_TRIM1                    _ICGTRM.Bits.TRIM1
#define ICGTRM_TRIM2                    _ICGTRM.Bits.TRIM2
#define ICGTRM_TRIM3                    _ICGTRM.Bits.TRIM3
#define ICGTRM_TRIM4                    _ICGTRM.Bits.TRIM4
#define ICGTRM_TRIM5                    _ICGTRM.Bits.TRIM5
#define ICGTRM_TRIM6                    _ICGTRM.Bits.TRIM6
#define ICGTRM_TRIM7                    _ICGTRM.Bits.TRIM7

#define ICGTRM_TRIM0_MASK               1
#define ICGTRM_TRIM1_MASK               2
#define ICGTRM_TRIM2_MASK               4
#define ICGTRM_TRIM3_MASK               8
#define ICGTRM_TRIM4_MASK               16
#define ICGTRM_TRIM5_MASK               32
#define ICGTRM_TRIM6_MASK               64
#define ICGTRM_TRIM7_MASK               128


/*** ATD1C - ATD1 Control Register; 0x00000050 ***/
typedef union {
  byte Byte;
  struct {
    byte PRS0        :1;                                       /* Prescaler Rate Select Bit 0 */
    byte PRS1        :1;                                       /* Prescaler Rate Select Bit 1 */
    byte PRS2        :1;                                       /* Prescaler Rate Select Bit 2 */
    byte PRS3        :1;                                       /* Prescaler Rate Select Bit 3 */
    byte SGN         :1;                                       /* Signed Result Select */
    byte RES8        :1;                                       /* ATD Resolution Select */
    byte DJM         :1;                                       /* Data Justification Mode */
    byte ATDPU       :1;                                       /* ATD Power Up */
  } Bits;
  struct {
    byte grpPRS  :4;
    byte         :1;
    byte grpRES_8 :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ATD1CSTR;
extern volatile ATD1CSTR _ATD1C @0x00000050;
#define ATD1C                           _ATD1C.Byte
#define ATD1C_PRS0                      _ATD1C.Bits.PRS0
#define ATD1C_PRS1                      _ATD1C.Bits.PRS1
#define ATD1C_PRS2                      _ATD1C.Bits.PRS2
#define ATD1C_PRS3                      _ATD1C.Bits.PRS3
#define ATD1C_SGN                       _ATD1C.Bits.SGN
#define ATD1C_RES8                      _ATD1C.Bits.RES8
#define ATD1C_DJM                       _ATD1C.Bits.DJM
#define ATD1C_ATDPU                     _ATD1C.Bits.ATDPU
#define ATD1C_PRS                       _ATD1C.MergedBits.grpPRS

#define ATD1C_PRS0_MASK                 1
#define ATD1C_PRS1_MASK                 2
#define ATD1C_PRS2_MASK                 4
#define ATD1C_PRS3_MASK                 8
#define ATD1C_SGN_MASK                  16
#define ATD1C_RES8_MASK                 32
#define ATD1C_DJM_MASK                  64
#define ATD1C_ATDPU_MASK                128
#define ATD1C_PRS_MASK                  15
#define ATD1C_PRS_BITNUM                0


/*** ATD1SC - ATD1 Status and Control Register; 0x00000051 ***/
typedef union {
  byte Byte;
  struct {
    byte ATDCH0      :1;                                       /* Analog Input Channel Select Bit 0 */
    byte ATDCH1      :1;                                       /* Analog Input Channel Select Bit 1 */
    byte ATDCH2      :1;                                       /* Analog Input Channel Select Bit 2 */
    byte ATDCH3      :1;                                       /* Analog Input Channel Select Bit 3 */
    byte ATDCH4      :1;                                       /* Analog Input Channel Select Bit 4 */
    byte ATDCO       :1;                                       /* ATD Continuous Conversion */
    byte ATDIE       :1;                                       /* ATD Interrupt Enabled */
    byte CCF         :1;                                       /* Conversion Complete Flag */
  } Bits;
  struct {
    byte grpATDCH :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ATD1SCSTR;
extern volatile ATD1SCSTR _ATD1SC @0x00000051;
#define ATD1SC                          _ATD1SC.Byte
#define ATD1SC_ATDCH0                   _ATD1SC.Bits.ATDCH0
#define ATD1SC_ATDCH1                   _ATD1SC.Bits.ATDCH1
#define ATD1SC_ATDCH2                   _ATD1SC.Bits.ATDCH2
#define ATD1SC_ATDCH3                   _ATD1SC.Bits.ATDCH3
#define ATD1SC_ATDCH4                   _ATD1SC.Bits.ATDCH4
#define ATD1SC_ATDCO                    _ATD1SC.Bits.ATDCO
#define ATD1SC_ATDIE                    _ATD1SC.Bits.ATDIE
#define ATD1SC_CCF                      _ATD1SC.Bits.CCF
#define ATD1SC_ATDCH                    _ATD1SC.MergedBits.grpATDCH

#define ATD1SC_ATDCH0_MASK              1
#define ATD1SC_ATDCH1_MASK              2
#define ATD1SC_ATDCH2_MASK              4
#define ATD1SC_ATDCH3_MASK              8
#define ATD1SC_ATDCH4_MASK              16
#define ATD1SC_ATDCO_MASK               32
#define ATD1SC_ATDIE_MASK               64
#define ATD1SC_CCF_MASK                 128
#define ATD1SC_ATDCH_MASK               31
#define ATD1SC_ATDCH_BITNUM             0


/*** ATD1R - ATD1 Result Data; 0x00000052 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** ATD1RH - ATD1 Result Data High Right Justified; 0x00000052 ***/
    union {
      byte Byte;
      struct {
        byte BIT8        :1;                                       /* ATD Result Data Bit 8 */
        byte BIT9        :1;                                       /* ATD Result Data Bit 9 */
        byte BIT10       :1;                                       /* ATD Result Data Bit 10 */
        byte BIT11       :1;                                       /* ATD Result Data Bit 11 */
        byte BIT12       :1;                                       /* ATD Result Data Bit 12 */
        byte BIT13       :1;                                       /* ATD Result Data Bit 13 */
        byte BIT14       :1;                                       /* ATD Result Data Bit 14 */
        byte BIT15       :1;                                       /* ATD Result Data Bit 15 */
      } Bits;
    } ATD1RHSTR;
    #define ATD1RH                      _ATD1R.Overlap_STR.ATD1RHSTR.Byte
    #define ATD1RH_BIT8                 _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT8
    #define ATD1RH_BIT9                 _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT9
    #define ATD1RH_BIT10                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT10
    #define ATD1RH_BIT11                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT11
    #define ATD1RH_BIT12                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT12
    #define ATD1RH_BIT13                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT13
    #define ATD1RH_BIT14                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT14
    #define ATD1RH_BIT15                _ATD1R.Overlap_STR.ATD1RHSTR.Bits.BIT15
    
    #define ATD1RH_BIT8_MASK            1
    #define ATD1RH_BIT9_MASK            2
    #define ATD1RH_BIT10_MASK           4
    #define ATD1RH_BIT11_MASK           8
    #define ATD1RH_BIT12_MASK           16
    #define ATD1RH_BIT13_MASK           32
    #define ATD1RH_BIT14_MASK           64
    #define ATD1RH_BIT15_MASK           128
    

    /*** ATD1RL - ATD1 Result Data Low Right Justified; 0x00000053 ***/
    union {
      byte Byte;
      struct {
        byte BIT0        :1;                                       /* ATD Result Data Bit 0 */
        byte BIT1        :1;                                       /* ATD Result Data Bit 1 */
        byte BIT2        :1;                                       /* ATD Result Data Bit 2 */
        byte BIT3        :1;                                       /* ATD Result Data Bit 3 */
        byte BIT4        :1;                                       /* ATD Result Data Bit 4 */
        byte BIT5        :1;                                       /* ATD Result Data Bit 5 */
        byte BIT6        :1;                                       /* ATD Result Data Bit 6 */
        byte BIT7        :1;                                       /* ATD Result Data Bit 7 */
      } Bits;
    } ATD1RLSTR;
    #define ATD1RL                      _ATD1R.Overlap_STR.ATD1RLSTR.Byte
    #define ATD1RL_BIT0                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT0
    #define ATD1RL_BIT1                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT1
    #define ATD1RL_BIT2                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT2
    #define ATD1RL_BIT3                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT3
    #define ATD1RL_BIT4                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT4
    #define ATD1RL_BIT5                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT5
    #define ATD1RL_BIT6                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT6
    #define ATD1RL_BIT7                 _ATD1R.Overlap_STR.ATD1RLSTR.Bits.BIT7
    
    #define ATD1RL_BIT0_MASK            1
    #define ATD1RL_BIT1_MASK            2
    #define ATD1RL_BIT2_MASK            4
    #define ATD1RL_BIT3_MASK            8
    #define ATD1RL_BIT4_MASK            16
    #define ATD1RL_BIT5_MASK            32
    #define ATD1RL_BIT6_MASK            64
    #define ATD1RL_BIT7_MASK            128
    
  } Overlap_STR;

} ATD1RSTR;
extern volatile ATD1RSTR _ATD1R @0x00000052;
#define ATD1R                           _ATD1R.Word


/*** ATD1PE - ATD1 Pin Enable; 0x00000054 ***/
typedef union {
  byte Byte;
  struct {
    byte ATDPE0      :1;                                       /* ATD Pin Enable Bit 0 */
    byte ATDPE1      :1;                                       /* ATD Pin Enable Bit 1 */
    byte ATDPE2      :1;                                       /* ATD Pin Enable Bit 2 */
    byte ATDPE3      :1;                                       /* ATD Pin Enable Bit 3 */
    byte ATDPE4      :1;                                       /* ATD Pin Enable Bit 4 */
    byte ATDPE5      :1;                                       /* ATD Pin Enable Bit 5 */
    byte ATDPE6      :1;                                       /* ATD Pin Enable Bit 6 */
    byte ATDPE7      :1;                                       /* ATD Pin Enable Bit 7 */
  } Bits;
} ATD1PESTR;
extern volatile ATD1PESTR _ATD1PE @0x00000054;
#define ATD1PE                          _ATD1PE.Byte
#define ATD1PE_ATDPE0                   _ATD1PE.Bits.ATDPE0
#define ATD1PE_ATDPE1                   _ATD1PE.Bits.ATDPE1
#define ATD1PE_ATDPE2                   _ATD1PE.Bits.ATDPE2
#define ATD1PE_ATDPE3                   _ATD1PE.Bits.ATDPE3
#define ATD1PE_ATDPE4                   _ATD1PE.Bits.ATDPE4
#define ATD1PE_ATDPE5                   _ATD1PE.Bits.ATDPE5
#define ATD1PE_ATDPE6                   _ATD1PE.Bits.ATDPE6
#define ATD1PE_ATDPE7                   _ATD1PE.Bits.ATDPE7

#define ATD1PE_ATDPE0_MASK              1
#define ATD1PE_ATDPE1_MASK              2
#define ATD1PE_ATDPE2_MASK              4
#define ATD1PE_ATDPE3_MASK              8
#define ATD1PE_ATDPE4_MASK              16
#define ATD1PE_ATDPE5_MASK              32
#define ATD1PE_ATDPE6_MASK              64
#define ATD1PE_ATDPE7_MASK              128


/*** IIC1A - IIC1 Address Register; 0x00000058 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte ADDR1       :1;                                       /* IIC Address Bit 1 */
    byte ADDR2       :1;                                       /* IIC Address Bit 2 */
    byte ADDR3       :1;                                       /* IIC Address Bit 3 */
    byte ADDR4       :1;                                       /* IIC Address Bit 4 */
    byte ADDR5       :1;                                       /* IIC Address Bit 5 */
    byte ADDR6       :1;                                       /* IIC Address Bit 6 */
    byte ADDR7       :1;                                       /* IIC Address Bit 7 */
  } Bits;
  struct {
    byte         :1;
    byte grpADDR_1 :7;
  } MergedBits;
} IIC1ASTR;
extern volatile IIC1ASTR _IIC1A @0x00000058;
#define IIC1A                           _IIC1A.Byte
#define IIC1A_ADDR1                     _IIC1A.Bits.ADDR1
#define IIC1A_ADDR2                     _IIC1A.Bits.ADDR2
#define IIC1A_ADDR3                     _IIC1A.Bits.ADDR3
#define IIC1A_ADDR4                     _IIC1A.Bits.ADDR4
#define IIC1A_ADDR5                     _IIC1A.Bits.ADDR5
#define IIC1A_ADDR6                     _IIC1A.Bits.ADDR6
#define IIC1A_ADDR7                     _IIC1A.Bits.ADDR7
#define IIC1A_ADDR_1                    _IIC1A.MergedBits.grpADDR_1
#define IIC1A_ADDR                      IIC1A_ADDR_1

#define IIC1A_ADDR1_MASK                2
#define IIC1A_ADDR2_MASK                4
#define IIC1A_ADDR3_MASK                8
#define IIC1A_ADDR4_MASK                16
#define IIC1A_ADDR5_MASK                32
#define IIC1A_ADDR6_MASK                64
#define IIC1A_ADDR7_MASK                128
#define IIC1A_ADDR_1_MASK               254
#define IIC1A_ADDR_1_BITNUM             1


/*** IIC1F - IIC1 Frequency Divider Register; 0x00000059 ***/
typedef union {
  byte Byte;
  struct {
    byte ICR0        :1;                                       /* IIC Clock Rate Bit 0 */
    byte ICR1        :1;                                       /* IIC Clock Rate Bit 1 */
    byte ICR2        :1;                                       /* IIC Clock Rate Bit 2 */
    byte ICR3        :1;                                       /* IIC Clock Rate Bit 3 */
    byte ICR4        :1;                                       /* IIC Clock Rate Bit 4 */
    byte ICR5        :1;                                       /* IIC Clock Rate Bit 5 */
    byte MULT0       :1;                                       /* Multiplier Factor Bit 0 */
    byte MULT1       :1;                                       /* Multiplier Factor Bit 1 */
  } Bits;
  struct {
    byte grpICR  :6;
    byte grpMULT :2;
  } MergedBits;
} IIC1FSTR;
extern volatile IIC1FSTR _IIC1F @0x00000059;
#define IIC1F                           _IIC1F.Byte
#define IIC1F_ICR0                      _IIC1F.Bits.ICR0
#define IIC1F_ICR1                      _IIC1F.Bits.ICR1
#define IIC1F_ICR2                      _IIC1F.Bits.ICR2
#define IIC1F_ICR3                      _IIC1F.Bits.ICR3
#define IIC1F_ICR4                      _IIC1F.Bits.ICR4
#define IIC1F_ICR5                      _IIC1F.Bits.ICR5
#define IIC1F_MULT0                     _IIC1F.Bits.MULT0
#define IIC1F_MULT1                     _IIC1F.Bits.MULT1
#define IIC1F_ICR                       _IIC1F.MergedBits.grpICR
#define IIC1F_MULT                      _IIC1F.MergedBits.grpMULT

#define IIC1F_ICR0_MASK                 1
#define IIC1F_ICR1_MASK                 2
#define IIC1F_ICR2_MASK                 4
#define IIC1F_ICR3_MASK                 8
#define IIC1F_ICR4_MASK                 16
#define IIC1F_ICR5_MASK                 32
#define IIC1F_MULT0_MASK                64
#define IIC1F_MULT1_MASK                128
#define IIC1F_ICR_MASK                  63
#define IIC1F_ICR_BITNUM                0
#define IIC1F_MULT_MASK                 192
#define IIC1F_MULT_BITNUM               6


/*** IIC1C - IIC1 Control Register; 0x0000005A ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte RSTA        :1;                                       /* Repeat START Bit */
    byte TXAK        :1;                                       /* Transmit Acknowledge Enable Bit */
    byte TX          :1;                                       /* Transmit Mode Select Bit */
    byte MST         :1;                                       /* Master Mode Select Bit */
    byte IICIE       :1;                                       /* IIC Interrupt Enable Bit */
    byte IICEN       :1;                                       /* IIC Enable Bit */
  } Bits;
} IIC1CSTR;
extern volatile IIC1CSTR _IIC1C @0x0000005A;
#define IIC1C                           _IIC1C.Byte
#define IIC1C_RSTA                      _IIC1C.Bits.RSTA
#define IIC1C_TXAK                      _IIC1C.Bits.TXAK
#define IIC1C_TX                        _IIC1C.Bits.TX
#define IIC1C_MST                       _IIC1C.Bits.MST
#define IIC1C_IICIE                     _IIC1C.Bits.IICIE
#define IIC1C_IICEN                     _IIC1C.Bits.IICEN

#define IIC1C_RSTA_MASK                 4
#define IIC1C_TXAK_MASK                 8
#define IIC1C_TX_MASK                   16
#define IIC1C_MST_MASK                  32
#define IIC1C_IICIE_MASK                64
#define IIC1C_IICEN_MASK                128


/*** IIC1S - IIC1 Status Register; 0x0000005B ***/
typedef union {
  byte Byte;
  struct {
    byte RXAK        :1;                                       /* Receive Acknowledge */
    byte IICIF       :1;                                       /* IIC Interrupt Flag */
    byte SRW         :1;                                       /* Slave Read/Write */
    byte             :1; 
    byte ARBL        :1;                                       /* Arbitration Lost */
    byte BUSY        :1;                                       /* Bus Busy bit */
    byte IAAS        :1;                                       /* Addressed as a Slave Bit */
    byte TCF         :1;                                       /* Transfer Complete Flag */
  } Bits;
} IIC1SSTR;
extern volatile IIC1SSTR _IIC1S @0x0000005B;
#define IIC1S                           _IIC1S.Byte
#define IIC1S_RXAK                      _IIC1S.Bits.RXAK
#define IIC1S_IICIF                     _IIC1S.Bits.IICIF
#define IIC1S_SRW                       _IIC1S.Bits.SRW
#define IIC1S_ARBL                      _IIC1S.Bits.ARBL
#define IIC1S_BUSY                      _IIC1S.Bits.BUSY
#define IIC1S_IAAS                      _IIC1S.Bits.IAAS
#define IIC1S_TCF                       _IIC1S.Bits.TCF

#define IIC1S_RXAK_MASK                 1
#define IIC1S_IICIF_MASK                2
#define IIC1S_SRW_MASK                  4
#define IIC1S_ARBL_MASK                 16
#define IIC1S_BUSY_MASK                 32
#define IIC1S_IAAS_MASK                 64
#define IIC1S_TCF_MASK                  128


/*** IIC1D - IIC1 Data I/O Register; 0x0000005C ***/
typedef union {
  byte Byte;
  struct {
    byte DATA0       :1;                                       /* IIC Data Bit 0 */
    byte DATA1       :1;                                       /* IIC Data Bit 1 */
    byte DATA2       :1;                                       /* IIC Data Bit 2 */
    byte DATA3       :1;                                       /* IIC Data Bit 3 */
    byte DATA4       :1;                                       /* IIC Data Bit 4 */
    byte DATA5       :1;                                       /* IIC Data Bit 5 */
    byte DATA6       :1;                                       /* IIC Data Bit 6 */
    byte DATA7       :1;                                       /* IIC Data Bit 7 */
  } Bits;
} IIC1DSTR;
extern volatile IIC1DSTR _IIC1D @0x0000005C;
#define IIC1D                           _IIC1D.Byte
#define IIC1D_DATA0                     _IIC1D.Bits.DATA0
#define IIC1D_DATA1                     _IIC1D.Bits.DATA1
#define IIC1D_DATA2                     _IIC1D.Bits.DATA2
#define IIC1D_DATA3                     _IIC1D.Bits.DATA3
#define IIC1D_DATA4                     _IIC1D.Bits.DATA4
#define IIC1D_DATA5                     _IIC1D.Bits.DATA5
#define IIC1D_DATA6                     _IIC1D.Bits.DATA6
#define IIC1D_DATA7                     _IIC1D.Bits.DATA7

#define IIC1D_DATA0_MASK                1
#define IIC1D_DATA1_MASK                2
#define IIC1D_DATA2_MASK                4
#define IIC1D_DATA3_MASK                8
#define IIC1D_DATA4_MASK                16
#define IIC1D_DATA5_MASK                32
#define IIC1D_DATA6_MASK                64
#define IIC1D_DATA7_MASK                128


/*** TPM2SC - TPM 2 Status and Control Register; 0x00000060 ***/
typedef union {
  byte Byte;
  struct {
    byte PS0         :1;                                       /* Prescale Divisor Select Bit 0 */
    byte PS1         :1;                                       /* Prescale Divisor Select Bit 1 */
    byte PS2         :1;                                       /* Prescale Divisor Select Bit 2 */
    byte CLKSA       :1;                                       /* Clock Source Select A */
    byte CLKSB       :1;                                       /* Clock Source Select B */
    byte CPWMS       :1;                                       /* Center-Aligned PWM Select */
    byte TOIE        :1;                                       /* Timer Overflow Interrupt Enable */
    byte TOF         :1;                                       /* Timer Overflow Flag */
  } Bits;
  struct {
    byte grpPS   :3;
    byte grpCLKSx :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2SCSTR;
extern volatile TPM2SCSTR _TPM2SC @0x00000060;
#define TPM2SC                          _TPM2SC.Byte
#define TPM2SC_PS0                      _TPM2SC.Bits.PS0
#define TPM2SC_PS1                      _TPM2SC.Bits.PS1
#define TPM2SC_PS2                      _TPM2SC.Bits.PS2
#define TPM2SC_CLKSA                    _TPM2SC.Bits.CLKSA
#define TPM2SC_CLKSB                    _TPM2SC.Bits.CLKSB
#define TPM2SC_CPWMS                    _TPM2SC.Bits.CPWMS
#define TPM2SC_TOIE                     _TPM2SC.Bits.TOIE
#define TPM2SC_TOF                      _TPM2SC.Bits.TOF
#define TPM2SC_PS                       _TPM2SC.MergedBits.grpPS
#define TPM2SC_CLKSx                    _TPM2SC.MergedBits.grpCLKSx

#define TPM2SC_PS0_MASK                 1
#define TPM2SC_PS1_MASK                 2
#define TPM2SC_PS2_MASK                 4
#define TPM2SC_CLKSA_MASK               8
#define TPM2SC_CLKSB_MASK               16
#define TPM2SC_CPWMS_MASK               32
#define TPM2SC_TOIE_MASK                64
#define TPM2SC_TOF_MASK                 128
#define TPM2SC_PS_MASK                  7
#define TPM2SC_PS_BITNUM                0
#define TPM2SC_CLKSx_MASK               24
#define TPM2SC_CLKSx_BITNUM             3


/*** TPM2CNT - TPM 2 Counter Register; 0x00000061 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2CNTH - TPM 2 Counter Register High; 0x00000061 ***/
    union {
      byte Byte;
    } TPM2CNTHSTR;
    #define TPM2CNTH                    _TPM2CNT.Overlap_STR.TPM2CNTHSTR.Byte
    

    /*** TPM2CNTL - TPM 2 Counter Register Low; 0x00000062 ***/
    union {
      byte Byte;
    } TPM2CNTLSTR;
    #define TPM2CNTL                    _TPM2CNT.Overlap_STR.TPM2CNTLSTR.Byte
    
  } Overlap_STR;

} TPM2CNTSTR;
extern volatile TPM2CNTSTR _TPM2CNT @0x00000061;
#define TPM2CNT                         _TPM2CNT.Word


/*** TPM2MOD - TPM 2 Timer Counter Modulo Register; 0x00000063 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2MODH - TPM 2 Timer Counter Modulo Register High; 0x00000063 ***/
    union {
      byte Byte;
    } TPM2MODHSTR;
    #define TPM2MODH                    _TPM2MOD.Overlap_STR.TPM2MODHSTR.Byte
    

    /*** TPM2MODL - TPM 2 Timer Counter Modulo Register Low; 0x00000064 ***/
    union {
      byte Byte;
    } TPM2MODLSTR;
    #define TPM2MODL                    _TPM2MOD.Overlap_STR.TPM2MODLSTR.Byte
    
  } Overlap_STR;

} TPM2MODSTR;
extern volatile TPM2MODSTR _TPM2MOD @0x00000063;
#define TPM2MOD                         _TPM2MOD.Word


/*** TPM2C0SC - TPM 2 Timer Channel 0 Status and Control Register; 0x00000065 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for TPM Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for TPM Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2C0SCSTR;
extern volatile TPM2C0SCSTR _TPM2C0SC @0x00000065;
#define TPM2C0SC                        _TPM2C0SC.Byte
#define TPM2C0SC_ELS0A                  _TPM2C0SC.Bits.ELS0A
#define TPM2C0SC_ELS0B                  _TPM2C0SC.Bits.ELS0B
#define TPM2C0SC_MS0A                   _TPM2C0SC.Bits.MS0A
#define TPM2C0SC_MS0B                   _TPM2C0SC.Bits.MS0B
#define TPM2C0SC_CH0IE                  _TPM2C0SC.Bits.CH0IE
#define TPM2C0SC_CH0F                   _TPM2C0SC.Bits.CH0F
#define TPM2C0SC_ELS0x                  _TPM2C0SC.MergedBits.grpELS0x
#define TPM2C0SC_MS0x                   _TPM2C0SC.MergedBits.grpMS0x

#define TPM2C0SC_ELS0A_MASK             4
#define TPM2C0SC_ELS0B_MASK             8
#define TPM2C0SC_MS0A_MASK              16
#define TPM2C0SC_MS0B_MASK              32
#define TPM2C0SC_CH0IE_MASK             64
#define TPM2C0SC_CH0F_MASK              128
#define TPM2C0SC_ELS0x_MASK             12
#define TPM2C0SC_ELS0x_BITNUM           2
#define TPM2C0SC_MS0x_MASK              48
#define TPM2C0SC_MS0x_BITNUM            4


/*** TPM2C0V - TPM 2 Timer Channel 0 Value Register; 0x00000066 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2C0VH - TPM 2 Timer Channel 0 Value Register High; 0x00000066 ***/
    union {
      byte Byte;
    } TPM2C0VHSTR;
    #define TPM2C0VH                    _TPM2C0V.Overlap_STR.TPM2C0VHSTR.Byte
    

    /*** TPM2C0VL - TPM 2 Timer Channel 0 Value Register Low; 0x00000067 ***/
    union {
      byte Byte;
    } TPM2C0VLSTR;
    #define TPM2C0VL                    _TPM2C0V.Overlap_STR.TPM2C0VLSTR.Byte
    
  } Overlap_STR;

} TPM2C0VSTR;
extern volatile TPM2C0VSTR _TPM2C0V @0x00000066;
#define TPM2C0V                         _TPM2C0V.Word


/*** TPM2C1SC - TPM 2 Timer Channel 1 Status and Control Register; 0x00000068 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for TPM Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for TPM Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2C1SCSTR;
extern volatile TPM2C1SCSTR _TPM2C1SC @0x00000068;
#define TPM2C1SC                        _TPM2C1SC.Byte
#define TPM2C1SC_ELS1A                  _TPM2C1SC.Bits.ELS1A
#define TPM2C1SC_ELS1B                  _TPM2C1SC.Bits.ELS1B
#define TPM2C1SC_MS1A                   _TPM2C1SC.Bits.MS1A
#define TPM2C1SC_MS1B                   _TPM2C1SC.Bits.MS1B
#define TPM2C1SC_CH1IE                  _TPM2C1SC.Bits.CH1IE
#define TPM2C1SC_CH1F                   _TPM2C1SC.Bits.CH1F
#define TPM2C1SC_ELS1x                  _TPM2C1SC.MergedBits.grpELS1x
#define TPM2C1SC_MS1x                   _TPM2C1SC.MergedBits.grpMS1x

#define TPM2C1SC_ELS1A_MASK             4
#define TPM2C1SC_ELS1B_MASK             8
#define TPM2C1SC_MS1A_MASK              16
#define TPM2C1SC_MS1B_MASK              32
#define TPM2C1SC_CH1IE_MASK             64
#define TPM2C1SC_CH1F_MASK              128
#define TPM2C1SC_ELS1x_MASK             12
#define TPM2C1SC_ELS1x_BITNUM           2
#define TPM2C1SC_MS1x_MASK              48
#define TPM2C1SC_MS1x_BITNUM            4


/*** TPM2C1V - TPM 2 Timer Channel 1 Value Register; 0x00000069 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2C1VH - TPM 2 Timer Channel 1 Value Register High; 0x00000069 ***/
    union {
      byte Byte;
    } TPM2C1VHSTR;
    #define TPM2C1VH                    _TPM2C1V.Overlap_STR.TPM2C1VHSTR.Byte
    

    /*** TPM2C1VL - TPM 2 Timer Channel 1 Value Register Low; 0x0000006A ***/
    union {
      byte Byte;
    } TPM2C1VLSTR;
    #define TPM2C1VL                    _TPM2C1V.Overlap_STR.TPM2C1VLSTR.Byte
    
  } Overlap_STR;

} TPM2C1VSTR;
extern volatile TPM2C1VSTR _TPM2C1V @0x00000069;
#define TPM2C1V                         _TPM2C1V.Word


/*** SRS - System Reset Status; 0x00001800 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte LVD         :1;                                       /* Low Voltage Detect */
    byte ICG         :1;                                       /* Internal Clock Generation Module Reset */
    byte             :1; 
    byte ILOP        :1;                                       /* Illegal Opcode */
    byte COP         :1;                                       /* Computer Operating Properly (COP) Watchdog */
    byte PIN         :1;                                       /* External Reset Pin */
    byte POR         :1;                                       /* Power-On Reset */
  } Bits;
} SRSSTR;
extern volatile SRSSTR _SRS @0x00001800;
#define SRS                             _SRS.Byte
#define SRS_LVD                         _SRS.Bits.LVD
#define SRS_ICG                         _SRS.Bits.ICG
#define SRS_ILOP                        _SRS.Bits.ILOP
#define SRS_COP                         _SRS.Bits.COP
#define SRS_PIN                         _SRS.Bits.PIN
#define SRS_POR                         _SRS.Bits.POR

#define SRS_LVD_MASK                    2
#define SRS_ICG_MASK                    4
#define SRS_ILOP_MASK                   16
#define SRS_COP_MASK                    32
#define SRS_PIN_MASK                    64
#define SRS_POR_MASK                    128


/*** SBDFR - System Background Debug Force Reset Register; 0x00001801 ***/
typedef union {
  byte Byte;
  struct {
    byte BDFR        :1;                                       /* Background Debug Force Reset */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SBDFRSTR;
extern volatile SBDFRSTR _SBDFR @0x00001801;
#define SBDFR                           _SBDFR.Byte
#define SBDFR_BDFR                      _SBDFR.Bits.BDFR

#define SBDFR_BDFR_MASK                 1


/*** SOPT - System Options Register; 0x00001802 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte BKGDPE      :1;                                       /* Background Debug Mode Pin Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte STOPE       :1;                                       /* Stop Mode Enable */
    byte COPT        :1;                                       /* COP Watchdog Timeout */
    byte COPE        :1;                                       /* COP Watchdog Enable */
  } Bits;
} SOPTSTR;
extern volatile SOPTSTR _SOPT @0x00001802;
#define SOPT                            _SOPT.Byte
#define SOPT_BKGDPE                     _SOPT.Bits.BKGDPE
#define SOPT_STOPE                      _SOPT.Bits.STOPE
#define SOPT_COPT                       _SOPT.Bits.COPT
#define SOPT_COPE                       _SOPT.Bits.COPE

#define SOPT_BKGDPE_MASK                2
#define SOPT_STOPE_MASK                 32
#define SOPT_COPT_MASK                  64
#define SOPT_COPE_MASK                  128


/*** SDID - System Integration Module Part ID Register; 0x00001806 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SDIDH - System Integration Module Part ID Register High; 0x00001806 ***/
    union {
      byte Byte;
      struct {
        byte ID8         :1;                                       /* Part Identification Number 8 */
        byte ID9         :1;                                       /* Part Identification Number 9 */
        byte ID10        :1;                                       /* Part Identification Number 10 */
        byte ID11        :1;                                       /* Part Identification Number 11 */
        byte REV0        :1;                                       /* Revision Number 0 */
        byte REV1        :1;                                       /* Revision Number 1 */
        byte REV2        :1;                                       /* Revision Number 2 */
        byte REV3        :1;                                       /* Revision Number 3 */
      } Bits;
      struct {
        byte grpID_8 :4;
        byte grpREV :4;
      } MergedBits;
    } SDIDHSTR;
    #define SDIDH                       _SDID.Overlap_STR.SDIDHSTR.Byte
    #define SDIDH_ID8                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID8
    #define SDIDH_ID9                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID9
    #define SDIDH_ID10                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID10
    #define SDIDH_ID11                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID11
    #define SDIDH_REV0                  _SDID.Overlap_STR.SDIDHSTR.Bits.REV0
    #define SDIDH_REV1                  _SDID.Overlap_STR.SDIDHSTR.Bits.REV1
    #define SDIDH_REV2                  _SDID.Overlap_STR.SDIDHSTR.Bits.REV2
    #define SDIDH_REV3                  _SDID.Overlap_STR.SDIDHSTR.Bits.REV3
    #define SDIDH_ID_8                  _SDID.Overlap_STR.SDIDHSTR.MergedBits.grpID_8
    #define SDIDH_REV                   _SDID.Overlap_STR.SDIDHSTR.MergedBits.grpREV
    #define SDIDH_ID                    SDIDH_ID_8
    
    #define SDIDH_ID8_MASK              1
    #define SDIDH_ID9_MASK              2
    #define SDIDH_ID10_MASK             4
    #define SDIDH_ID11_MASK             8
    #define SDIDH_REV0_MASK             16
    #define SDIDH_REV1_MASK             32
    #define SDIDH_REV2_MASK             64
    #define SDIDH_REV3_MASK             128
    #define SDIDH_ID_8_MASK             15
    #define SDIDH_ID_8_BITNUM           0
    #define SDIDH_REV_MASK              240
    #define SDIDH_REV_BITNUM            4
    

    /*** SDIDL - System Integration Module Part ID Register Low; 0x00001807 ***/
    union {
      byte Byte;
      struct {
        byte ID0         :1;                                       /* Part Identification Number 0 */
        byte ID1         :1;                                       /* Part Identification Number 1 */
        byte ID2         :1;                                       /* Part Identification Number 2 */
        byte ID3         :1;                                       /* Part Identification Number 3 */
        byte ID4         :1;                                       /* Part Identification Number 4 */
        byte ID5         :1;                                       /* Part Identification Number 5 */
        byte ID6         :1;                                       /* Part Identification Number 6 */
        byte ID7         :1;                                       /* Part Identification Number 7 */
      } Bits;
    } SDIDLSTR;
    #define SDIDL                       _SDID.Overlap_STR.SDIDLSTR.Byte
    #define SDIDL_ID0                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID0
    #define SDIDL_ID1                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID1
    #define SDIDL_ID2                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID2
    #define SDIDL_ID3                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID3
    #define SDIDL_ID4                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID4
    #define SDIDL_ID5                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID5
    #define SDIDL_ID6                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID6
    #define SDIDL_ID7                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID7
    
    #define SDIDL_ID0_MASK              1
    #define SDIDL_ID1_MASK              2
    #define SDIDL_ID2_MASK              4
    #define SDIDL_ID3_MASK              8
    #define SDIDL_ID4_MASK              16
    #define SDIDL_ID5_MASK              32
    #define SDIDL_ID6_MASK              64
    #define SDIDL_ID7_MASK              128
    
  } Overlap_STR;

} SDIDSTR;
extern volatile SDIDSTR _SDID @0x00001806;
#define SDID                            _SDID.Word


/*** SRTISC - System RTI Status and Control Register; 0x00001808 ***/
typedef union {
  byte Byte;
  struct {
    byte RTIS0       :1;                                       /* Real-Time Interrupt Delay Select Bit 0 */
    byte RTIS1       :1;                                       /* Real-Time Interrupt Delay Select Bit 1 */
    byte RTIS2       :1;                                       /* Real-Time Interrupt Delay Select Bit 2 */
    byte             :1; 
    byte RTIE        :1;                                       /* Real-Time Interrupt Enable */
    byte RTICLKS     :1;                                       /* Real-Time Interrupt Clock Select */
    byte RTIACK      :1;                                       /* Real-Time Interrupt Acknowledge */
    byte RTIF        :1;                                       /* Real-Time Interrupt Flag */
  } Bits;
  struct {
    byte grpRTIS :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SRTISCSTR;
extern volatile SRTISCSTR _SRTISC @0x00001808;
#define SRTISC                          _SRTISC.Byte
#define SRTISC_RTIS0                    _SRTISC.Bits.RTIS0
#define SRTISC_RTIS1                    _SRTISC.Bits.RTIS1
#define SRTISC_RTIS2                    _SRTISC.Bits.RTIS2
#define SRTISC_RTIE                     _SRTISC.Bits.RTIE
#define SRTISC_RTICLKS                  _SRTISC.Bits.RTICLKS
#define SRTISC_RTIACK                   _SRTISC.Bits.RTIACK
#define SRTISC_RTIF                     _SRTISC.Bits.RTIF
#define SRTISC_RTIS                     _SRTISC.MergedBits.grpRTIS

#define SRTISC_RTIS0_MASK               1
#define SRTISC_RTIS1_MASK               2
#define SRTISC_RTIS2_MASK               4
#define SRTISC_RTIE_MASK                16
#define SRTISC_RTICLKS_MASK             32
#define SRTISC_RTIACK_MASK              64
#define SRTISC_RTIF_MASK                128
#define SRTISC_RTIS_MASK                7
#define SRTISC_RTIS_BITNUM              0


/*** SPMSC1 - PM Status and Control 1 Register; 0x00001809 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte LVDE        :1;                                       /* Low-Voltage Detect Enable */
    byte LVDSE       :1;                                       /* Low-Voltage Detect Stop Enable */
    byte LVDRE       :1;                                       /* Low-Voltage Detect Reset Enable */
    byte LVDIE       :1;                                       /* Low-Voltage Detect Interrrupt Enable */
    byte LVDACK      :1;                                       /* Low-Voltage Detect Acknowledge */
    byte LVDF        :1;                                       /* Low-Voltage Detect Flag */
  } Bits;
} SPMSC1STR;
extern volatile SPMSC1STR _SPMSC1 @0x00001809;
#define SPMSC1                          _SPMSC1.Byte
#define SPMSC1_LVDE                     _SPMSC1.Bits.LVDE
#define SPMSC1_LVDSE                    _SPMSC1.Bits.LVDSE
#define SPMSC1_LVDRE                    _SPMSC1.Bits.LVDRE
#define SPMSC1_LVDIE                    _SPMSC1.Bits.LVDIE
#define SPMSC1_LVDACK                   _SPMSC1.Bits.LVDACK
#define SPMSC1_LVDF                     _SPMSC1.Bits.LVDF

#define SPMSC1_LVDE_MASK                4
#define SPMSC1_LVDSE_MASK               8
#define SPMSC1_LVDRE_MASK               16
#define SPMSC1_LVDIE_MASK               32
#define SPMSC1_LVDACK_MASK              64
#define SPMSC1_LVDF_MASK                128


/*** SPMSC2 - PM Status and Control 2 Register; 0x0000180A ***/
typedef union {
  byte Byte;
  struct {
    byte PPDC        :1;                                       /* Partial Power Down Control */
    byte PDC         :1;                                       /* Power Down Control */
    byte PPDACK      :1;                                       /* Partial Power Down Acknowlege */
    byte PPDF        :1;                                       /* Partial Power Down Flag */
    byte LVWV        :1;                                       /* Low-Voltage Warning Voltage Select */
    byte LVDV        :1;                                       /* Low-Voltage Detect Voltage Select */
    byte LVWACK      :1;                                       /* Low-Voltage Warning Acknowlege */
    byte LVWF        :1;                                       /* Low-Voltage Warning Flag */
  } Bits;
} SPMSC2STR;
extern volatile SPMSC2STR _SPMSC2 @0x0000180A;
#define SPMSC2                          _SPMSC2.Byte
#define SPMSC2_PPDC                     _SPMSC2.Bits.PPDC
#define SPMSC2_PDC                      _SPMSC2.Bits.PDC
#define SPMSC2_PPDACK                   _SPMSC2.Bits.PPDACK
#define SPMSC2_PPDF                     _SPMSC2.Bits.PPDF
#define SPMSC2_LVWV                     _SPMSC2.Bits.LVWV
#define SPMSC2_LVDV                     _SPMSC2.Bits.LVDV
#define SPMSC2_LVWACK                   _SPMSC2.Bits.LVWACK
#define SPMSC2_LVWF                     _SPMSC2.Bits.LVWF

#define SPMSC2_PPDC_MASK                1
#define SPMSC2_PDC_MASK                 2
#define SPMSC2_PPDACK_MASK              4
#define SPMSC2_PPDF_MASK                8
#define SPMSC2_LVWV_MASK                16
#define SPMSC2_LVDV_MASK                32
#define SPMSC2_LVWACK_MASK              64
#define SPMSC2_LVWF_MASK                128


/*** DBGCAH - Debug Comparator A High Register; 0x00001810 ***/
typedef union {
  byte Byte;
  struct {
    byte Bit8        :1;                                       /* Debug Comparator A Bit 8 */
    byte Bit9        :1;                                       /* Debug Comparator A Bit 9 */
    byte Bit10       :1;                                       /* Debug Comparator A Bit 10 */
    byte Bit11       :1;                                       /* Debug Comparator A Bit 11 */
    byte Bit12       :1;                                       /* Debug Comparator A Bit 12 */
    byte Bit13       :1;                                       /* Debug Comparator A Bit 13 */
    byte Bit14       :1;                                       /* Debug Comparator A Bit 14 */
    byte Bit15       :1;                                       /* Debug Comparator A Bit 15 */
  } Bits;
} DBGCAHSTR;
extern volatile DBGCAHSTR _DBGCAH @0x00001810;
#define DBGCAH                          _DBGCAH.Byte
#define DBGCAH_Bit8                     _DBGCAH.Bits.Bit8
#define DBGCAH_Bit9                     _DBGCAH.Bits.Bit9
#define DBGCAH_Bit10                    _DBGCAH.Bits.Bit10
#define DBGCAH_Bit11                    _DBGCAH.Bits.Bit11
#define DBGCAH_Bit12                    _DBGCAH.Bits.Bit12
#define DBGCAH_Bit13                    _DBGCAH.Bits.Bit13
#define DBGCAH_Bit14                    _DBGCAH.Bits.Bit14
#define DBGCAH_Bit15                    _DBGCAH.Bits.Bit15

#define DBGCAH_Bit8_MASK                1
#define DBGCAH_Bit9_MASK                2
#define DBGCAH_Bit10_MASK               4
#define DBGCAH_Bit11_MASK               8
#define DBGCAH_Bit12_MASK               16
#define DBGCAH_Bit13_MASK               32
#define DBGCAH_Bit14_MASK               64
#define DBGCAH_Bit15_MASK               128


/*** DBGCAL - Debug Comparator A Low Register; 0x00001811 ***/
typedef union {
  byte Byte;
  struct {
    byte Bit0        :1;                                       /* Debug Comparator A Bit 0 */
    byte Bit1        :1;                                       /* Debug Comparator A Bit 1 */
    byte Bit2        :1;                                       /* Debug Comparator A Bit 2 */
    byte Bit3        :1;                                       /* Debug Comparator A Bit 3 */
    byte Bit4        :1;                                       /* Debug Comparator A Bit 4 */
    byte Bit5        :1;                                       /* Debug Comparator A Bit 5 */
    byte Bit6        :1;                                       /* Debug Comparator A Bit 6 */
    byte Bit7        :1;                                       /* Debug Comparator A Bit 7 */
  } Bits;
} DBGCALSTR;
extern volatile DBGCALSTR _DBGCAL @0x00001811;
#define DBGCAL                          _DBGCAL.Byte
#define DBGCAL_Bit0                     _DBGCAL.Bits.Bit0
#define DBGCAL_Bit1                     _DBGCAL.Bits.Bit1
#define DBGCAL_Bit2                     _DBGCAL.Bits.Bit2
#define DBGCAL_Bit3                     _DBGCAL.Bits.Bit3
#define DBGCAL_Bit4                     _DBGCAL.Bits.Bit4
#define DBGCAL_Bit5                     _DBGCAL.Bits.Bit5
#define DBGCAL_Bit6                     _DBGCAL.Bits.Bit6
#define DBGCAL_Bit7                     _DBGCAL.Bits.Bit7

#define DBGCAL_Bit0_MASK                1
#define DBGCAL_Bit1_MASK                2
#define DBGCAL_Bit2_MASK                4
#define DBGCAL_Bit3_MASK                8
#define DBGCAL_Bit4_MASK                16
#define DBGCAL_Bit5_MASK                32
#define DBGCAL_Bit6_MASK                64
#define DBGCAL_Bit7_MASK                128


/*** DBGCBH - Debug Comparator B High Register; 0x00001812 ***/
typedef union {
  byte Byte;
  struct {
    byte Bit8        :1;                                       /* Debug Comparator B Bit 8 */
    byte Bit9        :1;                                       /* Debug Comparator B Bit 9 */
    byte Bit10       :1;                                       /* Debug Comparator B Bit 10 */
    byte Bit11       :1;                                       /* Debug Comparator B Bit 11 */
    byte Bit12       :1;                                       /* Debug Comparator B Bit 12 */
    byte Bit13       :1;                                       /* Debug Comparator B Bit 13 */
    byte Bit14       :1;                                       /* Debug Comparator B Bit 14 */
    byte Bit15       :1;                                       /* Debug Comparator B Bit 15 */
  } Bits;
} DBGCBHSTR;
extern volatile DBGCBHSTR _DBGCBH @0x00001812;
#define DBGCBH                          _DBGCBH.Byte
#define DBGCBH_Bit8                     _DBGCBH.Bits.Bit8
#define DBGCBH_Bit9                     _DBGCBH.Bits.Bit9
#define DBGCBH_Bit10                    _DBGCBH.Bits.Bit10
#define DBGCBH_Bit11                    _DBGCBH.Bits.Bit11
#define DBGCBH_Bit12                    _DBGCBH.Bits.Bit12
#define DBGCBH_Bit13                    _DBGCBH.Bits.Bit13
#define DBGCBH_Bit14                    _DBGCBH.Bits.Bit14
#define DBGCBH_Bit15                    _DBGCBH.Bits.Bit15

#define DBGCBH_Bit8_MASK                1
#define DBGCBH_Bit9_MASK                2
#define DBGCBH_Bit10_MASK               4
#define DBGCBH_Bit11_MASK               8
#define DBGCBH_Bit12_MASK               16
#define DBGCBH_Bit13_MASK               32
#define DBGCBH_Bit14_MASK               64
#define DBGCBH_Bit15_MASK               128


/*** DBGCBL - Debug Comparator B Low Register; 0x00001813 ***/
typedef union {
  byte Byte;
  struct {
    byte Bit0        :1;                                       /* Debug Comparator B Bit 0 */
    byte Bit1        :1;                                       /* Debug Comparator B Bit 1 */
    byte Bit2        :1;                                       /* Debug Comparator B Bit 2 */
    byte Bit3        :1;                                       /* Debug Comparator B Bit 3 */
    byte Bit4        :1;                                       /* Debug Comparator B Bit 4 */
    byte Bit5        :1;                                       /* Debug Comparator B Bit 5 */
    byte Bit6        :1;                                       /* Debug Comparator B Bit 6 */
    byte Bit7        :1;                                       /* Debug Comparator B Bit 7 */
  } Bits;
} DBGCBLSTR;
extern volatile DBGCBLSTR _DBGCBL @0x00001813;
#define DBGCBL                          _DBGCBL.Byte
#define DBGCBL_Bit0                     _DBGCBL.Bits.Bit0
#define DBGCBL_Bit1                     _DBGCBL.Bits.Bit1
#define DBGCBL_Bit2                     _DBGCBL.Bits.Bit2
#define DBGCBL_Bit3                     _DBGCBL.Bits.Bit3
#define DBGCBL_Bit4                     _DBGCBL.Bits.Bit4
#define DBGCBL_Bit5                     _DBGCBL.Bits.Bit5
#define DBGCBL_Bit6                     _DBGCBL.Bits.Bit6
#define DBGCBL_Bit7                     _DBGCBL.Bits.Bit7

#define DBGCBL_Bit0_MASK                1
#define DBGCBL_Bit1_MASK                2
#define DBGCBL_Bit2_MASK                4
#define DBGCBL_Bit3_MASK                8
#define DBGCBL_Bit4_MASK                16
#define DBGCBL_Bit5_MASK                32
#define DBGCBL_Bit6_MASK                64
#define DBGCBL_Bit7_MASK                128


/*** DBGF - Debug FIFO Register; 0x00001814 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** DBGFH - Debug FIFO High Register; 0x00001814 ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* Debug FIFO Bit 8 */
        byte Bit9        :1;                                       /* Debug FIFO Bit 9 */
        byte Bit10       :1;                                       /* Debug FIFO Bit 10 */
        byte Bit11       :1;                                       /* Debug FIFO Bit 11 */
        byte Bit12       :1;                                       /* Debug FIFO Bit 12 */
        byte Bit13       :1;                                       /* Debug FIFO Bit 13 */
        byte Bit14       :1;                                       /* Debug FIFO Bit 14 */
        byte Bit15       :1;                                       /* Debug FIFO Bit 15 */
      } Bits;
    } DBGFHSTR;
    #define DBGFH                       _DBGF.Overlap_STR.DBGFHSTR.Byte
    #define DBGFH_Bit8                  _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit8
    #define DBGFH_Bit9                  _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit9
    #define DBGFH_Bit10                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit10
    #define DBGFH_Bit11                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit11
    #define DBGFH_Bit12                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit12
    #define DBGFH_Bit13                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit13
    #define DBGFH_Bit14                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit14
    #define DBGFH_Bit15                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit15
    
    #define DBGFH_Bit8_MASK             1
    #define DBGFH_Bit9_MASK             2
    #define DBGFH_Bit10_MASK            4
    #define DBGFH_Bit11_MASK            8
    #define DBGFH_Bit12_MASK            16
    #define DBGFH_Bit13_MASK            32
    #define DBGFH_Bit14_MASK            64
    #define DBGFH_Bit15_MASK            128
    

    /*** DBGFL - Debug FIFO Low Register; 0x00001815 ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* Debug FIFO Bit 0 */
        byte Bit1        :1;                                       /* Debug FIFO Bit 1 */
        byte Bit2        :1;                                       /* Debug FIFO Bit 2 */
        byte Bit3        :1;                                       /* Debug FIFO Bit 3 */
        byte Bit4        :1;                                       /* Debug FIFO Bit 4 */
        byte Bit5        :1;                                       /* Debug FIFO Bit 5 */
        byte Bit6        :1;                                       /* Debug FIFO Bit 6 */
        byte Bit7        :1;                                       /* Debug FIFO Bit 7 */
      } Bits;
    } DBGFLSTR;
    #define DBGFL                       _DBGF.Overlap_STR.DBGFLSTR.Byte
    #define DBGFL_Bit0                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit0
    #define DBGFL_Bit1                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit1
    #define DBGFL_Bit2                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit2
    #define DBGFL_Bit3                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit3
    #define DBGFL_Bit4                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit4
    #define DBGFL_Bit5                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit5
    #define DBGFL_Bit6                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit6
    #define DBGFL_Bit7                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit7
    
    #define DBGFL_Bit0_MASK             1
    #define DBGFL_Bit1_MASK             2
    #define DBGFL_Bit2_MASK             4
    #define DBGFL_Bit3_MASK             8
    #define DBGFL_Bit4_MASK             16
    #define DBGFL_Bit5_MASK             32
    #define DBGFL_Bit6_MASK             64
    #define DBGFL_Bit7_MASK             128
    
  } Overlap_STR;

} DBGFSTR;
extern volatile DBGFSTR _DBGF @0x00001814;
#define DBGF                            _DBGF.Word


/*** DBGC - Debug Control Register; 0x00001816 ***/
typedef union {
  byte Byte;
  struct {
    byte RWBEN       :1;                                       /* Enable R/W for Comparator B */
    byte RWB         :1;                                       /* R/W Comparison Value for Comparator B */
    byte RWAEN       :1;                                       /* Enable R/W for Comparator A */
    byte RWA         :1;                                       /* R/W Comparison Value for Comparator A */
    byte BRKEN       :1;                                       /*  Break Enable */
    byte TAG         :1;                                       /* Tag/Force Select */
    byte ARM         :1;                                       /* Arm Control */
    byte DBGEN       :1;                                       /* Debug Module Enable */
  } Bits;
} DBGCSTR;
extern volatile DBGCSTR _DBGC @0x00001816;
#define DBGC                            _DBGC.Byte
#define DBGC_RWBEN                      _DBGC.Bits.RWBEN
#define DBGC_RWB                        _DBGC.Bits.RWB
#define DBGC_RWAEN                      _DBGC.Bits.RWAEN
#define DBGC_RWA                        _DBGC.Bits.RWA
#define DBGC_BRKEN                      _DBGC.Bits.BRKEN
#define DBGC_TAG                        _DBGC.Bits.TAG
#define DBGC_ARM                        _DBGC.Bits.ARM
#define DBGC_DBGEN                      _DBGC.Bits.DBGEN

#define DBGC_RWBEN_MASK                 1
#define DBGC_RWB_MASK                   2
#define DBGC_RWAEN_MASK                 4
#define DBGC_RWA_MASK                   8
#define DBGC_BRKEN_MASK                 16
#define DBGC_TAG_MASK                   32
#define DBGC_ARM_MASK                   64
#define DBGC_DBGEN_MASK                 128


/*** DBGT - Debug Trigger Register; 0x00001817 ***/
typedef union {
  byte Byte;
  struct {
    byte TRG0        :1;                                       /* Select Trigger Mode Bit 0 */
    byte TRG1        :1;                                       /* Select Trigger Mode Bit 1 */
    byte TRG2        :1;                                       /* Select Trigger Mode Bit 2 */
    byte TRG3        :1;                                       /* Select Trigger Mode Bit 3 */
    byte             :1; 
    byte             :1; 
    byte BEGIN       :1;                                       /* Begin/End Trigger Select */
    byte TRGSEL      :1;                                       /* Trigger Type */
  } Bits;
  struct {
    byte grpTRG  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DBGTSTR;
extern volatile DBGTSTR _DBGT @0x00001817;
#define DBGT                            _DBGT.Byte
#define DBGT_TRG0                       _DBGT.Bits.TRG0
#define DBGT_TRG1                       _DBGT.Bits.TRG1
#define DBGT_TRG2                       _DBGT.Bits.TRG2
#define DBGT_TRG3                       _DBGT.Bits.TRG3
#define DBGT_BEGIN                      _DBGT.Bits.BEGIN
#define DBGT_TRGSEL                     _DBGT.Bits.TRGSEL
#define DBGT_TRG                        _DBGT.MergedBits.grpTRG

#define DBGT_TRG0_MASK                  1
#define DBGT_TRG1_MASK                  2
#define DBGT_TRG2_MASK                  4
#define DBGT_TRG3_MASK                  8
#define DBGT_BEGIN_MASK                 64
#define DBGT_TRGSEL_MASK                128
#define DBGT_TRG_MASK                   15
#define DBGT_TRG_BITNUM                 0


/*** DBGS - Debug Status Register; 0x00001818 ***/
typedef union {
  byte Byte;
  struct {
    byte CNT0        :1;                                       /* FIFO Valid Count Bit 0 */
    byte CNT1        :1;                                       /* FIFO Valid Count Bit 1 */
    byte CNT2        :1;                                       /* FIFO Valid Count Bit 2 */
    byte CNT3        :1;                                       /* FIFO Valid Count Bit 3 */
    byte             :1; 
    byte ARMF        :1;                                       /* Arm Flag */
    byte BF          :1;                                       /* Trigger Match B Flag */
    byte AF          :1;                                       /* Trigger Match A Flag */
  } Bits;
  struct {
    byte grpCNT  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DBGSSTR;
extern volatile DBGSSTR _DBGS @0x00001818;
#define DBGS                            _DBGS.Byte
#define DBGS_CNT0                       _DBGS.Bits.CNT0
#define DBGS_CNT1                       _DBGS.Bits.CNT1
#define DBGS_CNT2                       _DBGS.Bits.CNT2
#define DBGS_CNT3                       _DBGS.Bits.CNT3
#define DBGS_ARMF                       _DBGS.Bits.ARMF
#define DBGS_BF                         _DBGS.Bits.BF
#define DBGS_AF                         _DBGS.Bits.AF
#define DBGS_CNT                        _DBGS.MergedBits.grpCNT

#define DBGS_CNT0_MASK                  1
#define DBGS_CNT1_MASK                  2
#define DBGS_CNT2_MASK                  4
#define DBGS_CNT3_MASK                  8
#define DBGS_ARMF_MASK                  32
#define DBGS_BF_MASK                    64
#define DBGS_AF_MASK                    128
#define DBGS_CNT_MASK                   15
#define DBGS_CNT_BITNUM                 0


/*** FCDIV - FLASH Clock Divider Register; 0x00001820 ***/
typedef union {
  byte Byte;
  struct {
    byte DIV0        :1;                                       /* Divisor for FLASH Clock Divider Bit 0 */
    byte DIV1        :1;                                       /* Divisor for FLASH Clock Divider Bit 1 */
    byte DIV2        :1;                                       /* Divisor for FLASH Clock Divider Bit 2 */
    byte DIV3        :1;                                       /* Divisor for FLASH Clock Divider Bit 3 */
    byte DIV4        :1;                                       /* Divisor for FLASH Clock Divider Bit 4 */
    byte DIV5        :1;                                       /* Divisor for FLASH Clock Divider Bit 5 */
    byte PRDIV8      :1;                                       /* Prescale (Divide) FLASH Clock by 8 */
    byte DIVLD       :1;                                       /* Divisor Loaded Status Flag */
  } Bits;
  struct {
    byte grpDIV  :6;
    byte grpPRDIV_8 :1;
    byte         :1;
  } MergedBits;
} FCDIVSTR;
extern volatile FCDIVSTR _FCDIV @0x00001820;
#define FCDIV                           _FCDIV.Byte
#define FCDIV_DIV0                      _FCDIV.Bits.DIV0
#define FCDIV_DIV1                      _FCDIV.Bits.DIV1
#define FCDIV_DIV2                      _FCDIV.Bits.DIV2
#define FCDIV_DIV3                      _FCDIV.Bits.DIV3
#define FCDIV_DIV4                      _FCDIV.Bits.DIV4
#define FCDIV_DIV5                      _FCDIV.Bits.DIV5
#define FCDIV_PRDIV8                    _FCDIV.Bits.PRDIV8
#define FCDIV_DIVLD                     _FCDIV.Bits.DIVLD
#define FCDIV_DIV                       _FCDIV.MergedBits.grpDIV

#define FCDIV_DIV0_MASK                 1
#define FCDIV_DIV1_MASK                 2
#define FCDIV_DIV2_MASK                 4
#define FCDIV_DIV3_MASK                 8
#define FCDIV_DIV4_MASK                 16
#define FCDIV_DIV5_MASK                 32
#define FCDIV_PRDIV8_MASK               64
#define FCDIV_DIVLD_MASK                128
#define FCDIV_DIV_MASK                  63
#define FCDIV_DIV_BITNUM                0


/*** FOPT - FLASH Options Register; 0x00001821 ***/
typedef union {
  byte Byte;
  struct {
    byte SEC00       :1;                                       /* Security State Code Bit 0 */
    byte SEC01       :1;                                       /* Security State Code Bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte FNORED      :1;                                       /* Vector Redirection Disable */
    byte KEYEN       :1;                                       /* Backdoor Key Mechanism Enable */
  } Bits;
  struct {
    byte grpSEC0 :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FOPTSTR;
extern volatile FOPTSTR _FOPT @0x00001821;
#define FOPT                            _FOPT.Byte
#define FOPT_SEC00                      _FOPT.Bits.SEC00
#define FOPT_SEC01                      _FOPT.Bits.SEC01
#define FOPT_FNORED                     _FOPT.Bits.FNORED
#define FOPT_KEYEN                      _FOPT.Bits.KEYEN
#define FOPT_SEC0                       _FOPT.MergedBits.grpSEC0

#define FOPT_SEC00_MASK                 1
#define FOPT_SEC01_MASK                 2
#define FOPT_FNORED_MASK                64
#define FOPT_KEYEN_MASK                 128
#define FOPT_SEC0_MASK                  3
#define FOPT_SEC0_BITNUM                0


/*** FCNFG - FLASH Configuration Register; 0x00001823 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte KEYACC      :1;                                       /* Enable Writing of Access Key */
    byte             :1; 
    byte             :1; 
  } Bits;
} FCNFGSTR;
extern volatile FCNFGSTR _FCNFG @0x00001823;
#define FCNFG                           _FCNFG.Byte
#define FCNFG_KEYACC                    _FCNFG.Bits.KEYACC

#define FCNFG_KEYACC_MASK               32


/*** FPROT - FLASH Protection Register; 0x00001824 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte FPS0        :1;                                       /* FLASH Protect Size Select Bit 0 */
    byte FPS1        :1;                                       /* FLASH Protect Size Select Bit 1 */
    byte FPS2        :1;                                       /* FLASH Protect Size Select Bit 2 */
    byte FPDIS       :1;                                       /* FLASH Protection Disable */
    byte FPOPEN      :1;                                       /* Open Unprotected FLASH for Program/Erase */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpFPS  :3;
    byte         :1;
    byte         :1;
  } MergedBits;
} FPROTSTR;
extern volatile FPROTSTR _FPROT @0x00001824;
#define FPROT                           _FPROT.Byte
#define FPROT_FPS0                      _FPROT.Bits.FPS0
#define FPROT_FPS1                      _FPROT.Bits.FPS1
#define FPROT_FPS2                      _FPROT.Bits.FPS2
#define FPROT_FPDIS                     _FPROT.Bits.FPDIS
#define FPROT_FPOPEN                    _FPROT.Bits.FPOPEN
#define FPROT_FPS                       _FPROT.MergedBits.grpFPS

#define FPROT_FPS0_MASK                 8
#define FPROT_FPS1_MASK                 16
#define FPROT_FPS2_MASK                 32
#define FPROT_FPDIS_MASK                64
#define FPROT_FPOPEN_MASK               128
#define FPROT_FPS_MASK                  56
#define FPROT_FPS_BITNUM                3


/*** FSTAT - FLASH Status Register; 0x00001825 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte FBLANK      :1;                                       /* FLASH Verified as All Blank (erased) Flag */
    byte             :1; 
    byte FACCERR     :1;                                       /* Access Error Flag */
    byte FPVIOL      :1;                                       /* Protection Violation Flag */
    byte FCCF        :1;                                       /* FLASH Command Complete Flag */
    byte FCBEF       :1;                                       /* FLASH Command Buffer Empty Flag */
  } Bits;
} FSTATSTR;
extern volatile FSTATSTR _FSTAT @0x00001825;
#define FSTAT                           _FSTAT.Byte
#define FSTAT_FBLANK                    _FSTAT.Bits.FBLANK
#define FSTAT_FACCERR                   _FSTAT.Bits.FACCERR
#define FSTAT_FPVIOL                    _FSTAT.Bits.FPVIOL
#define FSTAT_FCCF                      _FSTAT.Bits.FCCF
#define FSTAT_FCBEF                     _FSTAT.Bits.FCBEF

#define FSTAT_FBLANK_MASK               4
#define FSTAT_FACCERR_MASK              16
#define FSTAT_FPVIOL_MASK               32
#define FSTAT_FCCF_MASK                 64
#define FSTAT_FCBEF_MASK                128


/*** FCMD - FLASH Command Register; 0x00001826 ***/
typedef union {
  byte Byte;
  struct {
    byte FCMD0       :1;                                       /* FLASH Command Bit 0 */
    byte FCMD1       :1;                                       /* FLASH Command Bit 1 */
    byte FCMD2       :1;                                       /* FLASH Command Bit 2 */
    byte FCMD3       :1;                                       /* FLASH Command Bit 3 */
    byte FCMD4       :1;                                       /* FLASH Command Bit 4 */
    byte FCMD5       :1;                                       /* FLASH Command Bit 5 */
    byte FCMD6       :1;                                       /* FLASH Command Bit 6 */
    byte FCMD7       :1;                                       /* FLASH Command Bit 7 */
  } Bits;
} FCMDSTR;
extern volatile FCMDSTR _FCMD @0x00001826;
#define FCMD                            _FCMD.Byte
#define FCMD_FCMD0                      _FCMD.Bits.FCMD0
#define FCMD_FCMD1                      _FCMD.Bits.FCMD1
#define FCMD_FCMD2                      _FCMD.Bits.FCMD2
#define FCMD_FCMD3                      _FCMD.Bits.FCMD3
#define FCMD_FCMD4                      _FCMD.Bits.FCMD4
#define FCMD_FCMD5                      _FCMD.Bits.FCMD5
#define FCMD_FCMD6                      _FCMD.Bits.FCMD6
#define FCMD_FCMD7                      _FCMD.Bits.FCMD7

#define FCMD_FCMD0_MASK                 1
#define FCMD_FCMD1_MASK                 2
#define FCMD_FCMD2_MASK                 4
#define FCMD_FCMD3_MASK                 8
#define FCMD_FCMD4_MASK                 16
#define FCMD_FCMD5_MASK                 32
#define FCMD_FCMD6_MASK                 64
#define FCMD_FCMD7_MASK                 128


/*** NVBACKKEY0 - Backdoor Comparison Key 0; 0x0000FFB0 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY0STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY0_INIT @0x0000FFB0 = <NVBACKKEY0_INITVAL>; */
#define _NVBACKKEY0 (*(const NVBACKKEY0STR * __far)0x0000FFB0)
#define NVBACKKEY0                      _NVBACKKEY0.Byte
#define NVBACKKEY0_KEY0                 _NVBACKKEY0.Bits.KEY0
#define NVBACKKEY0_KEY1                 _NVBACKKEY0.Bits.KEY1
#define NVBACKKEY0_KEY2                 _NVBACKKEY0.Bits.KEY2
#define NVBACKKEY0_KEY3                 _NVBACKKEY0.Bits.KEY3
#define NVBACKKEY0_KEY4                 _NVBACKKEY0.Bits.KEY4
#define NVBACKKEY0_KEY5                 _NVBACKKEY0.Bits.KEY5
#define NVBACKKEY0_KEY6                 _NVBACKKEY0.Bits.KEY6
#define NVBACKKEY0_KEY7                 _NVBACKKEY0.Bits.KEY7
/* NVBACKKEY_ARR: Access 8 NVBACKKEYx registers in an array */
#define NVBACKKEY_ARR                   ((volatile byte * __far) &NVBACKKEY0)

#define NVBACKKEY0_KEY0_MASK            1
#define NVBACKKEY0_KEY1_MASK            2
#define NVBACKKEY0_KEY2_MASK            4
#define NVBACKKEY0_KEY3_MASK            8
#define NVBACKKEY0_KEY4_MASK            16
#define NVBACKKEY0_KEY5_MASK            32
#define NVBACKKEY0_KEY6_MASK            64
#define NVBACKKEY0_KEY7_MASK            128


/*** NVBACKKEY1 - Backdoor Comparison Key 1; 0x0000FFB1 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY1STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY1_INIT @0x0000FFB1 = <NVBACKKEY1_INITVAL>; */
#define _NVBACKKEY1 (*(const NVBACKKEY1STR * __far)0x0000FFB1)
#define NVBACKKEY1                      _NVBACKKEY1.Byte
#define NVBACKKEY1_KEY0                 _NVBACKKEY1.Bits.KEY0
#define NVBACKKEY1_KEY1                 _NVBACKKEY1.Bits.KEY1
#define NVBACKKEY1_KEY2                 _NVBACKKEY1.Bits.KEY2
#define NVBACKKEY1_KEY3                 _NVBACKKEY1.Bits.KEY3
#define NVBACKKEY1_KEY4                 _NVBACKKEY1.Bits.KEY4
#define NVBACKKEY1_KEY5                 _NVBACKKEY1.Bits.KEY5
#define NVBACKKEY1_KEY6                 _NVBACKKEY1.Bits.KEY6
#define NVBACKKEY1_KEY7                 _NVBACKKEY1.Bits.KEY7

#define NVBACKKEY1_KEY0_MASK            1
#define NVBACKKEY1_KEY1_MASK            2
#define NVBACKKEY1_KEY2_MASK            4
#define NVBACKKEY1_KEY3_MASK            8
#define NVBACKKEY1_KEY4_MASK            16
#define NVBACKKEY1_KEY5_MASK            32
#define NVBACKKEY1_KEY6_MASK            64
#define NVBACKKEY1_KEY7_MASK            128


/*** NVBACKKEY2 - Backdoor Comparison Key 2; 0x0000FFB2 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY2STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY2_INIT @0x0000FFB2 = <NVBACKKEY2_INITVAL>; */
#define _NVBACKKEY2 (*(const NVBACKKEY2STR * __far)0x0000FFB2)
#define NVBACKKEY2                      _NVBACKKEY2.Byte
#define NVBACKKEY2_KEY0                 _NVBACKKEY2.Bits.KEY0
#define NVBACKKEY2_KEY1                 _NVBACKKEY2.Bits.KEY1
#define NVBACKKEY2_KEY2                 _NVBACKKEY2.Bits.KEY2
#define NVBACKKEY2_KEY3                 _NVBACKKEY2.Bits.KEY3
#define NVBACKKEY2_KEY4                 _NVBACKKEY2.Bits.KEY4
#define NVBACKKEY2_KEY5                 _NVBACKKEY2.Bits.KEY5
#define NVBACKKEY2_KEY6                 _NVBACKKEY2.Bits.KEY6
#define NVBACKKEY2_KEY7                 _NVBACKKEY2.Bits.KEY7

#define NVBACKKEY2_KEY0_MASK            1
#define NVBACKKEY2_KEY1_MASK            2
#define NVBACKKEY2_KEY2_MASK            4
#define NVBACKKEY2_KEY3_MASK            8
#define NVBACKKEY2_KEY4_MASK            16
#define NVBACKKEY2_KEY5_MASK            32
#define NVBACKKEY2_KEY6_MASK            64
#define NVBACKKEY2_KEY7_MASK            128


/*** NVBACKKEY3 - Backdoor Comparison Key 3; 0x0000FFB3 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY3STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY3_INIT @0x0000FFB3 = <NVBACKKEY3_INITVAL>; */
#define _NVBACKKEY3 (*(const NVBACKKEY3STR * __far)0x0000FFB3)
#define NVBACKKEY3                      _NVBACKKEY3.Byte
#define NVBACKKEY3_KEY0                 _NVBACKKEY3.Bits.KEY0
#define NVBACKKEY3_KEY1                 _NVBACKKEY3.Bits.KEY1
#define NVBACKKEY3_KEY2                 _NVBACKKEY3.Bits.KEY2
#define NVBACKKEY3_KEY3                 _NVBACKKEY3.Bits.KEY3
#define NVBACKKEY3_KEY4                 _NVBACKKEY3.Bits.KEY4
#define NVBACKKEY3_KEY5                 _NVBACKKEY3.Bits.KEY5
#define NVBACKKEY3_KEY6                 _NVBACKKEY3.Bits.KEY6
#define NVBACKKEY3_KEY7                 _NVBACKKEY3.Bits.KEY7

#define NVBACKKEY3_KEY0_MASK            1
#define NVBACKKEY3_KEY1_MASK            2
#define NVBACKKEY3_KEY2_MASK            4
#define NVBACKKEY3_KEY3_MASK            8
#define NVBACKKEY3_KEY4_MASK            16
#define NVBACKKEY3_KEY5_MASK            32
#define NVBACKKEY3_KEY6_MASK            64
#define NVBACKKEY3_KEY7_MASK            128


/*** NVBACKKEY4 - Backdoor Comparison Key 4; 0x0000FFB4 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY4STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY4_INIT @0x0000FFB4 = <NVBACKKEY4_INITVAL>; */
#define _NVBACKKEY4 (*(const NVBACKKEY4STR * __far)0x0000FFB4)
#define NVBACKKEY4                      _NVBACKKEY4.Byte
#define NVBACKKEY4_KEY0                 _NVBACKKEY4.Bits.KEY0
#define NVBACKKEY4_KEY1                 _NVBACKKEY4.Bits.KEY1
#define NVBACKKEY4_KEY2                 _NVBACKKEY4.Bits.KEY2
#define NVBACKKEY4_KEY3                 _NVBACKKEY4.Bits.KEY3
#define NVBACKKEY4_KEY4                 _NVBACKKEY4.Bits.KEY4
#define NVBACKKEY4_KEY5                 _NVBACKKEY4.Bits.KEY5
#define NVBACKKEY4_KEY6                 _NVBACKKEY4.Bits.KEY6
#define NVBACKKEY4_KEY7                 _NVBACKKEY4.Bits.KEY7

#define NVBACKKEY4_KEY0_MASK            1
#define NVBACKKEY4_KEY1_MASK            2
#define NVBACKKEY4_KEY2_MASK            4
#define NVBACKKEY4_KEY3_MASK            8
#define NVBACKKEY4_KEY4_MASK            16
#define NVBACKKEY4_KEY5_MASK            32
#define NVBACKKEY4_KEY6_MASK            64
#define NVBACKKEY4_KEY7_MASK            128


/*** NVBACKKEY5 - Backdoor Comparison Key 5; 0x0000FFB5 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY5STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY5_INIT @0x0000FFB5 = <NVBACKKEY5_INITVAL>; */
#define _NVBACKKEY5 (*(const NVBACKKEY5STR * __far)0x0000FFB5)
#define NVBACKKEY5                      _NVBACKKEY5.Byte
#define NVBACKKEY5_KEY0                 _NVBACKKEY5.Bits.KEY0
#define NVBACKKEY5_KEY1                 _NVBACKKEY5.Bits.KEY1
#define NVBACKKEY5_KEY2                 _NVBACKKEY5.Bits.KEY2
#define NVBACKKEY5_KEY3                 _NVBACKKEY5.Bits.KEY3
#define NVBACKKEY5_KEY4                 _NVBACKKEY5.Bits.KEY4
#define NVBACKKEY5_KEY5                 _NVBACKKEY5.Bits.KEY5
#define NVBACKKEY5_KEY6                 _NVBACKKEY5.Bits.KEY6
#define NVBACKKEY5_KEY7                 _NVBACKKEY5.Bits.KEY7

#define NVBACKKEY5_KEY0_MASK            1
#define NVBACKKEY5_KEY1_MASK            2
#define NVBACKKEY5_KEY2_MASK            4
#define NVBACKKEY5_KEY3_MASK            8
#define NVBACKKEY5_KEY4_MASK            16
#define NVBACKKEY5_KEY5_MASK            32
#define NVBACKKEY5_KEY6_MASK            64
#define NVBACKKEY5_KEY7_MASK            128


/*** NVBACKKEY6 - Backdoor Comparison Key 6; 0x0000FFB6 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY6STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY6_INIT @0x0000FFB6 = <NVBACKKEY6_INITVAL>; */
#define _NVBACKKEY6 (*(const NVBACKKEY6STR * __far)0x0000FFB6)
#define NVBACKKEY6                      _NVBACKKEY6.Byte
#define NVBACKKEY6_KEY0                 _NVBACKKEY6.Bits.KEY0
#define NVBACKKEY6_KEY1                 _NVBACKKEY6.Bits.KEY1
#define NVBACKKEY6_KEY2                 _NVBACKKEY6.Bits.KEY2
#define NVBACKKEY6_KEY3                 _NVBACKKEY6.Bits.KEY3
#define NVBACKKEY6_KEY4                 _NVBACKKEY6.Bits.KEY4
#define NVBACKKEY6_KEY5                 _NVBACKKEY6.Bits.KEY5
#define NVBACKKEY6_KEY6                 _NVBACKKEY6.Bits.KEY6
#define NVBACKKEY6_KEY7                 _NVBACKKEY6.Bits.KEY7

#define NVBACKKEY6_KEY0_MASK            1
#define NVBACKKEY6_KEY1_MASK            2
#define NVBACKKEY6_KEY2_MASK            4
#define NVBACKKEY6_KEY3_MASK            8
#define NVBACKKEY6_KEY4_MASK            16
#define NVBACKKEY6_KEY5_MASK            32
#define NVBACKKEY6_KEY6_MASK            64
#define NVBACKKEY6_KEY7_MASK            128


/*** NVBACKKEY7 - Backdoor Comparison Key 7; 0x0000FFB7 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key bits, bit 7 */
  } Bits;
} NVBACKKEY7STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY7_INIT @0x0000FFB7 = <NVBACKKEY7_INITVAL>; */
#define _NVBACKKEY7 (*(const NVBACKKEY7STR * __far)0x0000FFB7)
#define NVBACKKEY7                      _NVBACKKEY7.Byte
#define NVBACKKEY7_KEY0                 _NVBACKKEY7.Bits.KEY0
#define NVBACKKEY7_KEY1                 _NVBACKKEY7.Bits.KEY1
#define NVBACKKEY7_KEY2                 _NVBACKKEY7.Bits.KEY2
#define NVBACKKEY7_KEY3                 _NVBACKKEY7.Bits.KEY3
#define NVBACKKEY7_KEY4                 _NVBACKKEY7.Bits.KEY4
#define NVBACKKEY7_KEY5                 _NVBACKKEY7.Bits.KEY5
#define NVBACKKEY7_KEY6                 _NVBACKKEY7.Bits.KEY6
#define NVBACKKEY7_KEY7                 _NVBACKKEY7.Bits.KEY7

#define NVBACKKEY7_KEY0_MASK            1
#define NVBACKKEY7_KEY1_MASK            2
#define NVBACKKEY7_KEY2_MASK            4
#define NVBACKKEY7_KEY3_MASK            8
#define NVBACKKEY7_KEY4_MASK            16
#define NVBACKKEY7_KEY5_MASK            32
#define NVBACKKEY7_KEY6_MASK            64
#define NVBACKKEY7_KEY7_MASK            128


/*** NVPROT - Nonvolatile FLASH Protection Register; 0x0000FFBD ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte FPS0        :1;                                       /* FLASH Protect Size Select Bit 0 */
    byte FPS1        :1;                                       /* FLASH Protect Size Select Bit 1 */
    byte FPS2        :1;                                       /* FLASH Protect Size Select Bit 2 */
    byte FPDIS       :1;                                       /* FLASH Protection Disable */
    byte FPOPEN      :1;                                       /* Open Unprotected FLASH for Program/Erase */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpFPS  :3;
    byte         :1;
    byte         :1;
  } MergedBits;
} NVPROTSTR;
/* Tip for register initialization in the user code:  const byte NVPROT_INIT @0x0000FFBD = <NVPROT_INITVAL>; */
#define _NVPROT (*(const NVPROTSTR * __far)0x0000FFBD)
#define NVPROT                          _NVPROT.Byte
#define NVPROT_FPS0                     _NVPROT.Bits.FPS0
#define NVPROT_FPS1                     _NVPROT.Bits.FPS1
#define NVPROT_FPS2                     _NVPROT.Bits.FPS2
#define NVPROT_FPDIS                    _NVPROT.Bits.FPDIS
#define NVPROT_FPOPEN                   _NVPROT.Bits.FPOPEN
#define NVPROT_FPS                      _NVPROT.MergedBits.grpFPS

#define NVPROT_FPS0_MASK                8
#define NVPROT_FPS1_MASK                16
#define NVPROT_FPS2_MASK                32
#define NVPROT_FPDIS_MASK               64
#define NVPROT_FPOPEN_MASK              128
#define NVPROT_FPS_MASK                 56
#define NVPROT_FPS_BITNUM               3


/*** NVOPT - Nonvolatile FLASH Options Register; 0x0000FFBF ***/
typedef union {
  byte Byte;
  struct {
    byte SEC00       :1;                                       /* Security State Code Bit 0 */
    byte SEC01       :1;                                       /* Security State Code Bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte FNORED      :1;                                       /* Vector Redirection Disable */
    byte KEYEN       :1;                                       /* Backdoor Key Mechanism Enable */
  } Bits;
  struct {
    byte grpSEC0 :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} NVOPTSTR;
/* Tip for register initialization in the user code:  const byte NVOPT_INIT @0x0000FFBF = <NVOPT_INITVAL>; */
#define _NVOPT (*(const NVOPTSTR * __far)0x0000FFBF)
#define NVOPT                           _NVOPT.Byte
#define NVOPT_SEC00                     _NVOPT.Bits.SEC00
#define NVOPT_SEC01                     _NVOPT.Bits.SEC01
#define NVOPT_FNORED                    _NVOPT.Bits.FNORED
#define NVOPT_KEYEN                     _NVOPT.Bits.KEYEN
#define NVOPT_SEC0                      _NVOPT.MergedBits.grpSEC0

#define NVOPT_SEC00_MASK                1
#define NVOPT_SEC01_MASK                2
#define NVOPT_FNORED_MASK               64
#define NVOPT_KEYEN_MASK                128
#define NVOPT_SEC0_MASK                 3
#define NVOPT_SEC0_BITNUM               0



/* Flash commands */
#define mBlank                          0x05
#define mByteProg                       0x20
#define mBurstProg                      0x25
#define mMassErase                      0x41
#define mPageErase                      0x40


/***********************************************/
/**   D E P R E C I A T E D   S Y M B O L S   **/
/***********************************************/
#endif
