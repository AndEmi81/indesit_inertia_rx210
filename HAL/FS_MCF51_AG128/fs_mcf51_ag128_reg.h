/* Based on CPU DB MCF51AG128_80, version 3.00.011 (RegistersPrg V2.31) */
/*
** ###################################################################
**     Filename  : mcf51ag128.h
**     Processor : MCF51AG128VLK
**     FileFormat: V2.31
**     DataSheet : MCF51AG128RM Rev. 3 5/2009
**     Compiler  : CodeWarrior compiler
**     Date/Time : 13.11.2009, 15:07
**     Abstract  :
**         This header implements the mapping of I/O devices.
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
**
**     CPU Registers Revisions:
**      - 13.11.2009, V3.00.0:
**              - Removed empty Bits structures from definitions of EWM and WDOG registers.
**
**     File-Format-Revisions:
**      - 10.9.2009, V2.30 :
**               - Fixed generation of registers arrays.
**      - 15.10.2009, V2.31 :
**               - Changes have not affected this file (because they are related to another family)
**
**     Not all general-purpose I/O pins are available on all packages or on all mask sets of a specific
**     derivative device. To avoid extra current drain from floating input pins, the user’s reset
**     initialization routine in the application program must either enable on-chip pull-up devices
**     or change the direction of unconnected pins to outputs so the pins do not float.
** ###################################################################
*/

#ifndef _MCF51AG128_H
#define _MCF51AG128_H

/* Types definition */
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

/* Watchdog reset macro */
#ifndef __RESET_WATCHDOG
#ifdef _lint
  #define __RESET_WATCHDOG()  /* empty */
#else
  #define __RESET_WATCHDOG() (void)(WDOG_REFRESH = 0xA602, WDOG_REFRESH = 0xB480)
#endif
#endif /* __RESET_WATCHDOG */

#pragma options align=packed

/**************** interrupt vector numbers ****************/
#define VectorNumber_INITSP             0
#define VectorNumber_INITPC             1
#define VectorNumber_Vaccerr            2
#define VectorNumber_Vadderr            3
#define VectorNumber_Viinstr            4
#define VectorNumber_VReserved5         5
#define VectorNumber_VReserved6         6
#define VectorNumber_VReserved7         7
#define VectorNumber_Vprviol            8
#define VectorNumber_Vtrace             9
#define VectorNumber_Vunilaop           10
#define VectorNumber_Vunilfop           11
#define VectorNumber_Vdbgi              12
#define VectorNumber_VReserved13        13
#define VectorNumber_Vferror            14
#define VectorNumber_VReserved15        15
#define VectorNumber_VReserved16        16
#define VectorNumber_VReserved17        17
#define VectorNumber_VReserved18        18
#define VectorNumber_VReserved19        19
#define VectorNumber_VReserved20        20
#define VectorNumber_VReserved21        21
#define VectorNumber_VReserved22        22
#define VectorNumber_VReserved23        23
#define VectorNumber_Vspuri             24
#define VectorNumber_VReserved25        25
#define VectorNumber_VReserved26        26
#define VectorNumber_VReserved27        27
#define VectorNumber_VReserved28        28
#define VectorNumber_VReserved29        29
#define VectorNumber_VReserved30        30
#define VectorNumber_VReserved31        31
#define VectorNumber_Vtrap0             32
#define VectorNumber_Vtrap1             33
#define VectorNumber_Vtrap2             34
#define VectorNumber_Vtrap3             35
#define VectorNumber_Vtrap4             36
#define VectorNumber_Vtrap5             37
#define VectorNumber_Vtrap6             38
#define VectorNumber_Vtrap7             39
#define VectorNumber_Vtrap8             40
#define VectorNumber_Vtrap9             41
#define VectorNumber_Vtrap10            42
#define VectorNumber_Vtrap11            43
#define VectorNumber_Vtrap12            44
#define VectorNumber_Vtrap13            45
#define VectorNumber_Vtrap14            46
#define VectorNumber_Vtrap15            47
#define VectorNumber_VReserved48        48
#define VectorNumber_VReserved49        49
#define VectorNumber_VReserved50        50
#define VectorNumber_VReserved51        51
#define VectorNumber_VReserved52        52
#define VectorNumber_VReserved53        53
#define VectorNumber_VReserved54        54
#define VectorNumber_VReserved55        55
#define VectorNumber_VReserved56        56
#define VectorNumber_VReserved57        57
#define VectorNumber_VReserved58        58
#define VectorNumber_VReserved59        59
#define VectorNumber_VReserved60        60
#define VectorNumber_Vunsinstr          61
#define VectorNumber_VReserved62        62
#define VectorNumber_VReserved63        63
#define VectorNumber_Virq               64
#define VectorNumber_Vlvd               65
#define VectorNumber_VReserved66        66
#define VectorNumber_VReserved67        67
#define VectorNumber_Vdmach0            68
#define VectorNumber_Vdmach1            69
#define VectorNumber_Vdmach2            70
#define VectorNumber_Vdmach3            71
#define VectorNumber_Vieventch0         72
#define VectorNumber_Vftm1fault_ovf     73
#define VectorNumber_Vftm1ch0           74
#define VectorNumber_Vftm1ch1           75
#define VectorNumber_Vftm1ch2           76
#define VectorNumber_Vftm1ch3           77
#define VectorNumber_Vftm1ch4           78
#define VectorNumber_Vftm1ch5           79
#define VectorNumber_Vftm2fault_ovf     80
#define VectorNumber_Vftm2ch0           81
#define VectorNumber_Vftm2ch1           82
#define VectorNumber_Vftm2ch2           83
#define VectorNumber_Vftm2ch3           84
#define VectorNumber_Vftm2ch4           85
#define VectorNumber_Vftm2ch5           86
#define VectorNumber_Vtpm3ovf           87
#define VectorNumber_Vtpm3ch0           88
#define VectorNumber_Vtpm3ch1           89
#define VectorNumber_Vadc               90
#define VectorNumber_Vhscmp1            91
#define VectorNumber_Vhscmp2            92
#define VectorNumber_Vieventch1         93
#define VectorNumber_Vspi1              94
#define VectorNumber_Vspi2              95
#define VectorNumber_Vsci1err           96
#define VectorNumber_Vsci1rx            97
#define VectorNumber_Vsci1tx            98
#define VectorNumber_Viic               99
#define VectorNumber_Vieventch2         100
#define VectorNumber_Vsci2err           101
#define VectorNumber_Vsci2rx            102
#define VectorNumber_VL7swi             103
#define VectorNumber_VL6swi             104
#define VectorNumber_VL5swi             105
#define VectorNumber_VL4swi             106
#define VectorNumber_VL3swi             107
#define VectorNumber_VL2swi             108
#define VectorNumber_VL1swi             109
#define VectorNumber_Vsci2tx            110
#define VectorNumber_Vportae            111
#define VectorNumber_Vportfj            112
#define VectorNumber_Vrtc_wdg           113
#define VectorNumber_Vieventch3         114

/**************** interrupt vector table ****************/
#define INITSP                          0x00000000
#define INITPC                          0x00000004
#define Vaccerr                         0x00000008
#define Vadderr                         0x0000000C
#define Viinstr                         0x00000010
#define VReserved5                      0x00000014
#define VReserved6                      0x00000018
#define VReserved7                      0x0000001C
#define Vprviol                         0x00000020
#define Vtrace                          0x00000024
#define Vunilaop                        0x00000028
#define Vunilfop                        0x0000002C
#define Vdbgi                           0x00000030
#define VReserved13                     0x00000034
#define Vferror                         0x00000038
#define VReserved15                     0x0000003C
#define VReserved16                     0x00000040
#define VReserved17                     0x00000044
#define VReserved18                     0x00000048
#define VReserved19                     0x0000004C
#define VReserved20                     0x00000050
#define VReserved21                     0x00000054
#define VReserved22                     0x00000058
#define VReserved23                     0x0000005C
#define Vspuri                          0x00000060
#define VReserved25                     0x00000064
#define VReserved26                     0x00000068
#define VReserved27                     0x0000006C
#define VReserved28                     0x00000070
#define VReserved29                     0x00000074
#define VReserved30                     0x00000078
#define VReserved31                     0x0000007C
#define Vtrap0                          0x00000080
#define Vtrap1                          0x00000084
#define Vtrap2                          0x00000088
#define Vtrap3                          0x0000008C
#define Vtrap4                          0x00000090
#define Vtrap5                          0x00000094
#define Vtrap6                          0x00000098
#define Vtrap7                          0x0000009C
#define Vtrap8                          0x000000A0
#define Vtrap9                          0x000000A4
#define Vtrap10                         0x000000A8
#define Vtrap11                         0x000000AC
#define Vtrap12                         0x000000B0
#define Vtrap13                         0x000000B4
#define Vtrap14                         0x000000B8
#define Vtrap15                         0x000000BC
#define VReserved48                     0x000000C0
#define VReserved49                     0x000000C4
#define VReserved50                     0x000000C8
#define VReserved51                     0x000000CC
#define VReserved52                     0x000000D0
#define VReserved53                     0x000000D4
#define VReserved54                     0x000000D8
#define VReserved55                     0x000000DC
#define VReserved56                     0x000000E0
#define VReserved57                     0x000000E4
#define VReserved58                     0x000000E8
#define VReserved59                     0x000000EC
#define VReserved60                     0x000000F0
#define Vunsinstr                       0x000000F4
#define VReserved62                     0x000000F8
#define VReserved63                     0x000000FC
#define Virq                            0x00000100
#define Vlvd                            0x00000104
#define VReserved66                     0x00000108
#define VReserved67                     0x0000010C
#define Vdmach0                         0x00000110
#define Vdmach1                         0x00000114
#define Vdmach2                         0x00000118
#define Vdmach3                         0x0000011C
#define Vieventch0                      0x00000120
#define Vftm1fault_ovf                  0x00000124
#define Vftm1ch0                        0x00000128
#define Vftm1ch1                        0x0000012C
#define Vftm1ch2                        0x00000130
#define Vftm1ch3                        0x00000134
#define Vftm1ch4                        0x00000138
#define Vftm1ch5                        0x0000013C
#define Vftm2fault_ovf                  0x00000140
#define Vftm2ch0                        0x00000144
#define Vftm2ch1                        0x00000148
#define Vftm2ch2                        0x0000014C
#define Vftm2ch3                        0x00000150
#define Vftm2ch4                        0x00000154
#define Vftm2ch5                        0x00000158
#define Vtpm3ovf                        0x0000015C
#define Vtpm3ch0                        0x00000160
#define Vtpm3ch1                        0x00000164
#define Vadc                            0x00000168
#define Vhscmp1                         0x0000016C
#define Vhscmp2                         0x00000170
#define Vieventch1                      0x00000174
#define Vspi1                           0x00000178
#define Vspi2                           0x0000017C
#define Vsci1err                        0x00000180
#define Vsci1rx                         0x00000184
#define Vsci1tx                         0x00000188
#define Viic                            0x0000018C
#define Vieventch2                      0x00000190
#define Vsci2err                        0x00000194
#define Vsci2rx                         0x00000198
#define VL7swi                          0x0000019C
#define VL6swi                          0x000001A0
#define VL5swi                          0x000001A4
#define VL4swi                          0x000001A8
#define VL3swi                          0x000001AC
#define VL2swi                          0x000001B0
#define VL1swi                          0x000001B4
#define Vsci2tx                         0x000001B8
#define Vportae                         0x000001BC
#define Vportfj                         0x000001C0
#define Vrtc_wdg                        0x000001C4
#define Vieventch3                      0x000001C8

/**************** registers I/O map ****************/

/*** NVFTRIM - Nonvolatile ICS Fine Trim; 0x000003FE ***/
typedef union {
  byte Byte;
  struct {
    byte FTRIM       :1;                                       /* ICS Fine Trim */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} NVFTRIMSTR;
/* Tip for register initialization in the user code:  const byte NVFTRIM_INIT @0x000003FE = <NVFTRIM_INITVAL>; */
#define _NVFTRIM (*(const NVFTRIMSTR *)0x000003FE)
#define NVFTRIM                         _NVFTRIM.Byte
#define NVFTRIM_FTRIM                   _NVFTRIM.Bits.FTRIM

#define NVFTRIM_FTRIM_MASK              1


/*** NVICSTRM - Nonvolatile ICS Trim Register; 0x000003FF ***/
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* ICS Trim Setting, bit 0 */
    byte TRIM1       :1;                                       /* ICS Trim Setting, bit 1 */
    byte TRIM2       :1;                                       /* ICS Trim Setting, bit 2 */
    byte TRIM3       :1;                                       /* ICS Trim Setting, bit 3 */
    byte TRIM4       :1;                                       /* ICS Trim Setting, bit 4 */
    byte TRIM5       :1;                                       /* ICS Trim Setting, bit 5 */
    byte TRIM6       :1;                                       /* ICS Trim Setting, bit 6 */
    byte TRIM7       :1;                                       /* ICS Trim Setting, bit 7 */
  } Bits;
} NVICSTRMSTR;
/* Tip for register initialization in the user code:  const byte NVICSTRM_INIT @0x000003FF = <NVICSTRM_INITVAL>; */
#define _NVICSTRM (*(const NVICSTRMSTR *)0x000003FF)
#define NVICSTRM                        _NVICSTRM.Byte
#define NVICSTRM_TRIM0                  _NVICSTRM.Bits.TRIM0
#define NVICSTRM_TRIM1                  _NVICSTRM.Bits.TRIM1
#define NVICSTRM_TRIM2                  _NVICSTRM.Bits.TRIM2
#define NVICSTRM_TRIM3                  _NVICSTRM.Bits.TRIM3
#define NVICSTRM_TRIM4                  _NVICSTRM.Bits.TRIM4
#define NVICSTRM_TRIM5                  _NVICSTRM.Bits.TRIM5
#define NVICSTRM_TRIM6                  _NVICSTRM.Bits.TRIM6
#define NVICSTRM_TRIM7                  _NVICSTRM.Bits.TRIM7

#define NVICSTRM_TRIM0_MASK             1
#define NVICSTRM_TRIM1_MASK             2
#define NVICSTRM_TRIM2_MASK             4
#define NVICSTRM_TRIM3_MASK             8
#define NVICSTRM_TRIM4_MASK             16
#define NVICSTRM_TRIM5_MASK             32
#define NVICSTRM_TRIM6_MASK             64
#define NVICSTRM_TRIM7_MASK             128


/*** NVBACKKEY0 - Backdoor Comparison Key 0; 0x00000400 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 7 */
  } Bits;
} NVBACKKEY0STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY0_INIT @0x00000400 = <NVBACKKEY0_INITVAL>; */
#define _NVBACKKEY0 (*(const NVBACKKEY0STR *)0x00000400)
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
#define NVBACKKEY_ARR                   ((volatile byte *) &NVBACKKEY0)

#define NVBACKKEY0_KEY0_MASK            1
#define NVBACKKEY0_KEY1_MASK            2
#define NVBACKKEY0_KEY2_MASK            4
#define NVBACKKEY0_KEY3_MASK            8
#define NVBACKKEY0_KEY4_MASK            16
#define NVBACKKEY0_KEY5_MASK            32
#define NVBACKKEY0_KEY6_MASK            64
#define NVBACKKEY0_KEY7_MASK            128


/*** NVBACKKEY1 - Backdoor Comparison Key 1; 0x00000401 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 7 */
  } Bits;
} NVBACKKEY1STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY1_INIT @0x00000401 = <NVBACKKEY1_INITVAL>; */
#define _NVBACKKEY1 (*(const NVBACKKEY1STR *)0x00000401)
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


/*** NVBACKKEY2 - Backdoor Comparison Key 2; 0x00000402 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 7 */
  } Bits;
} NVBACKKEY2STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY2_INIT @0x00000402 = <NVBACKKEY2_INITVAL>; */
#define _NVBACKKEY2 (*(const NVBACKKEY2STR *)0x00000402)
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


/*** NVBACKKEY3 - Backdoor Comparison Key 3; 0x00000403 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 7 */
  } Bits;
} NVBACKKEY3STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY3_INIT @0x00000403 = <NVBACKKEY3_INITVAL>; */
#define _NVBACKKEY3 (*(const NVBACKKEY3STR *)0x00000403)
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


/*** NVBACKKEY4 - Backdoor Comparison Key 4; 0x00000404 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 7 */
  } Bits;
} NVBACKKEY4STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY4_INIT @0x00000404 = <NVBACKKEY4_INITVAL>; */
#define _NVBACKKEY4 (*(const NVBACKKEY4STR *)0x00000404)
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


/*** NVBACKKEY5 - Backdoor Comparison Key 5; 0x00000405 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 7 */
  } Bits;
} NVBACKKEY5STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY5_INIT @0x00000405 = <NVBACKKEY5_INITVAL>; */
#define _NVBACKKEY5 (*(const NVBACKKEY5STR *)0x00000405)
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


/*** NVBACKKEY6 - Backdoor Comparison Key 6; 0x00000406 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 7 */
  } Bits;
} NVBACKKEY6STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY6_INIT @0x00000406 = <NVBACKKEY6_INITVAL>; */
#define _NVBACKKEY6 (*(const NVBACKKEY6STR *)0x00000406)
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


/*** NVBACKKEY7 - Backdoor Comparison Key 7; 0x00000407 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 7 */
  } Bits;
} NVBACKKEY7STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY7_INIT @0x00000407 = <NVBACKKEY7_INITVAL>; */
#define _NVBACKKEY7 (*(const NVBACKKEY7STR *)0x00000407)
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


/*** NVPROT - Nonvolatile Flash Protection Register; 0x0000040D ***/
typedef union {
  byte Byte;
  struct {
    byte FPOPEN      :1;                                       /* Flash Protection Open */
    byte FPS0        :1;                                       /* Flash Protection Size, bit 0 */
    byte FPS1        :1;                                       /* Flash Protection Size, bit 1 */
    byte FPS2        :1;                                       /* Flash Protection Size, bit 2 */
    byte FPS3        :1;                                       /* Flash Protection Size, bit 3 */
    byte FPS4        :1;                                       /* Flash Protection Size, bit 4 */
    byte FPS5        :1;                                       /* Flash Protection Size, bit 5 */
    byte FPS6        :1;                                       /* Flash Protection Size, bit 6 */
  } Bits;
  struct {
    byte         :1;
    byte grpFPS  :7;
  } MergedBits;
} NVPROTSTR;
/* Tip for register initialization in the user code:  const byte NVPROT_INIT @0x0000040D = <NVPROT_INITVAL>; */
#define _NVPROT (*(const NVPROTSTR *)0x0000040D)
#define NVPROT                          _NVPROT.Byte
#define NVPROT_FPOPEN                   _NVPROT.Bits.FPOPEN
#define NVPROT_FPS0                     _NVPROT.Bits.FPS0
#define NVPROT_FPS1                     _NVPROT.Bits.FPS1
#define NVPROT_FPS2                     _NVPROT.Bits.FPS2
#define NVPROT_FPS3                     _NVPROT.Bits.FPS3
#define NVPROT_FPS4                     _NVPROT.Bits.FPS4
#define NVPROT_FPS5                     _NVPROT.Bits.FPS5
#define NVPROT_FPS6                     _NVPROT.Bits.FPS6
#define NVPROT_FPS                      _NVPROT.MergedBits.grpFPS

#define NVPROT_FPOPEN_MASK              1
#define NVPROT_FPS0_MASK                2
#define NVPROT_FPS1_MASK                4
#define NVPROT_FPS2_MASK                8
#define NVPROT_FPS3_MASK                16
#define NVPROT_FPS4_MASK                32
#define NVPROT_FPS5_MASK                64
#define NVPROT_FPS6_MASK                128
#define NVPROT_FPS_MASK                 254
#define NVPROT_FPS_BITNUM               1


/*** NVOPT - Nonvolatile Flash Options Register; 0x0000040F ***/
typedef union {
  byte Byte;
  struct {
    byte SEC0        :1;                                       /* Flash Security Bit 0 */
    byte SEC1        :1;                                       /* Flash Security Bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte KEYEN0      :1;                                       /* Backdoor Key Security Enable Bit 0 */
    byte KEYEN1      :1;                                       /* Backdoor Key Security Enable Bit 1 */
  } Bits;
  struct {
    byte grpSEC  :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpKEYEN :2;
  } MergedBits;
} NVOPTSTR;
/* Tip for register initialization in the user code:  const byte NVOPT_INIT @0x0000040F = <NVOPT_INITVAL>; */
#define _NVOPT (*(const NVOPTSTR *)0x0000040F)
#define NVOPT                           _NVOPT.Byte
#define NVOPT_SEC0                      _NVOPT.Bits.SEC0
#define NVOPT_SEC1                      _NVOPT.Bits.SEC1
#define NVOPT_KEYEN0                    _NVOPT.Bits.KEYEN0
#define NVOPT_KEYEN1                    _NVOPT.Bits.KEYEN1
#define NVOPT_SEC                       _NVOPT.MergedBits.grpSEC
#define NVOPT_KEYEN                     _NVOPT.MergedBits.grpKEYEN

#define NVOPT_SEC0_MASK                 1
#define NVOPT_SEC1_MASK                 2
#define NVOPT_KEYEN0_MASK               64
#define NVOPT_KEYEN1_MASK               128
#define NVOPT_SEC_MASK                  3
#define NVOPT_SEC_BITNUM                0
#define NVOPT_KEYEN_MASK                192
#define NVOPT_KEYEN_BITNUM              6


/*** RGPIO_DIR - RGPIO Data Direction Register; 0x00C00000 ***/
typedef union {
  word Word;
  struct {
    word DIR0        :1;                                       /* RGPIO data direction bit 0 */
    word DIR1        :1;                                       /* RGPIO data direction bit 1 */
    word DIR2        :1;                                       /* RGPIO data direction bit 2 */
    word DIR3        :1;                                       /* RGPIO data direction bit 3 */
    word DIR4        :1;                                       /* RGPIO data direction bit 4 */
    word DIR5        :1;                                       /* RGPIO data direction bit 5 */
    word DIR6        :1;                                       /* RGPIO data direction bit 6 */
    word DIR7        :1;                                       /* RGPIO data direction bit 7 */
    word DIR8        :1;                                       /* RGPIO data direction bit 8 */
    word DIR9        :1;                                       /* RGPIO data direction bit 9 */
    word DIR10       :1;                                       /* RGPIO data direction bit 10 */
    word DIR11       :1;                                       /* RGPIO data direction bit 11 */
    word DIR12       :1;                                       /* RGPIO data direction bit 12 */
    word DIR13       :1;                                       /* RGPIO data direction bit 13 */
    word DIR14       :1;                                       /* RGPIO data direction bit 14 */
    word DIR15       :1;                                       /* RGPIO data direction bit 15 */
  } Bits;
} RGPIO_DIRSTR;
extern volatile RGPIO_DIRSTR _RGPIO_DIR @0x00C00000;
#define RGPIO_DIR                       _RGPIO_DIR.Word
#define RGPIO_DIR_DIR0                  _RGPIO_DIR.Bits.DIR0
#define RGPIO_DIR_DIR1                  _RGPIO_DIR.Bits.DIR1
#define RGPIO_DIR_DIR2                  _RGPIO_DIR.Bits.DIR2
#define RGPIO_DIR_DIR3                  _RGPIO_DIR.Bits.DIR3
#define RGPIO_DIR_DIR4                  _RGPIO_DIR.Bits.DIR4
#define RGPIO_DIR_DIR5                  _RGPIO_DIR.Bits.DIR5
#define RGPIO_DIR_DIR6                  _RGPIO_DIR.Bits.DIR6
#define RGPIO_DIR_DIR7                  _RGPIO_DIR.Bits.DIR7
#define RGPIO_DIR_DIR8                  _RGPIO_DIR.Bits.DIR8
#define RGPIO_DIR_DIR9                  _RGPIO_DIR.Bits.DIR9
#define RGPIO_DIR_DIR10                 _RGPIO_DIR.Bits.DIR10
#define RGPIO_DIR_DIR11                 _RGPIO_DIR.Bits.DIR11
#define RGPIO_DIR_DIR12                 _RGPIO_DIR.Bits.DIR12
#define RGPIO_DIR_DIR13                 _RGPIO_DIR.Bits.DIR13
#define RGPIO_DIR_DIR14                 _RGPIO_DIR.Bits.DIR14
#define RGPIO_DIR_DIR15                 _RGPIO_DIR.Bits.DIR15

#define RGPIO_DIR_DIR0_MASK             1
#define RGPIO_DIR_DIR1_MASK             2
#define RGPIO_DIR_DIR2_MASK             4
#define RGPIO_DIR_DIR3_MASK             8
#define RGPIO_DIR_DIR4_MASK             16
#define RGPIO_DIR_DIR5_MASK             32
#define RGPIO_DIR_DIR6_MASK             64
#define RGPIO_DIR_DIR7_MASK             128
#define RGPIO_DIR_DIR8_MASK             256
#define RGPIO_DIR_DIR9_MASK             512
#define RGPIO_DIR_DIR10_MASK            1024
#define RGPIO_DIR_DIR11_MASK            2048
#define RGPIO_DIR_DIR12_MASK            4096
#define RGPIO_DIR_DIR13_MASK            8192
#define RGPIO_DIR_DIR14_MASK            16384
#define RGPIO_DIR_DIR15_MASK            32768


/*** RGPIO_DATA - RGPIO Data Register; 0x00C00002 ***/
typedef union {
  word Word;
  struct {
    word DATA0       :1;                                       /* RGPIO data bit 0 */
    word DATA1       :1;                                       /* RGPIO data bit 1 */
    word DATA2       :1;                                       /* RGPIO data bit 2 */
    word DATA3       :1;                                       /* RGPIO data bit 3 */
    word DATA4       :1;                                       /* RGPIO data bit 4 */
    word DATA5       :1;                                       /* RGPIO data bit 5 */
    word DATA6       :1;                                       /* RGPIO data bit 6 */
    word DATA7       :1;                                       /* RGPIO data bit 7 */
    word DATA8       :1;                                       /* RGPIO data bit 8 */
    word DATA9       :1;                                       /* RGPIO data bit 9 */
    word DATA10      :1;                                       /* RGPIO data bit 10 */
    word DATA11      :1;                                       /* RGPIO data bit 11 */
    word DATA12      :1;                                       /* RGPIO data bit 12 */
    word DATA13      :1;                                       /* RGPIO data bit 13 */
    word DATA14      :1;                                       /* RGPIO data bit 14 */
    word DATA15      :1;                                       /* RGPIO data bit 15 */
  } Bits;
} RGPIO_DATASTR;
extern volatile RGPIO_DATASTR _RGPIO_DATA @0x00C00002;
#define RGPIO_DATA                      _RGPIO_DATA.Word
#define RGPIO_DATA_DATA0                _RGPIO_DATA.Bits.DATA0
#define RGPIO_DATA_DATA1                _RGPIO_DATA.Bits.DATA1
#define RGPIO_DATA_DATA2                _RGPIO_DATA.Bits.DATA2
#define RGPIO_DATA_DATA3                _RGPIO_DATA.Bits.DATA3
#define RGPIO_DATA_DATA4                _RGPIO_DATA.Bits.DATA4
#define RGPIO_DATA_DATA5                _RGPIO_DATA.Bits.DATA5
#define RGPIO_DATA_DATA6                _RGPIO_DATA.Bits.DATA6
#define RGPIO_DATA_DATA7                _RGPIO_DATA.Bits.DATA7
#define RGPIO_DATA_DATA8                _RGPIO_DATA.Bits.DATA8
#define RGPIO_DATA_DATA9                _RGPIO_DATA.Bits.DATA9
#define RGPIO_DATA_DATA10               _RGPIO_DATA.Bits.DATA10
#define RGPIO_DATA_DATA11               _RGPIO_DATA.Bits.DATA11
#define RGPIO_DATA_DATA12               _RGPIO_DATA.Bits.DATA12
#define RGPIO_DATA_DATA13               _RGPIO_DATA.Bits.DATA13
#define RGPIO_DATA_DATA14               _RGPIO_DATA.Bits.DATA14
#define RGPIO_DATA_DATA15               _RGPIO_DATA.Bits.DATA15

#define RGPIO_DATA_DATA0_MASK           1
#define RGPIO_DATA_DATA1_MASK           2
#define RGPIO_DATA_DATA2_MASK           4
#define RGPIO_DATA_DATA3_MASK           8
#define RGPIO_DATA_DATA4_MASK           16
#define RGPIO_DATA_DATA5_MASK           32
#define RGPIO_DATA_DATA6_MASK           64
#define RGPIO_DATA_DATA7_MASK           128
#define RGPIO_DATA_DATA8_MASK           256
#define RGPIO_DATA_DATA9_MASK           512
#define RGPIO_DATA_DATA10_MASK          1024
#define RGPIO_DATA_DATA11_MASK          2048
#define RGPIO_DATA_DATA12_MASK          4096
#define RGPIO_DATA_DATA13_MASK          8192
#define RGPIO_DATA_DATA14_MASK          16384
#define RGPIO_DATA_DATA15_MASK          32768


/*** RGPIO_ENB - RGPIO Pin Enable Register; 0x00C00004 ***/
typedef union {
  word Word;
  struct {
    word ENB0        :1;                                       /* RGPIO enable bit 0 */
    word ENB1        :1;                                       /* RGPIO enable bit 1 */
    word ENB2        :1;                                       /* RGPIO enable bit 2 */
    word ENB3        :1;                                       /* RGPIO enable bit 3 */
    word ENB4        :1;                                       /* RGPIO enable bit 4 */
    word ENB5        :1;                                       /* RGPIO enable bit 5 */
    word ENB6        :1;                                       /* RGPIO enable bit 6 */
    word ENB7        :1;                                       /* RGPIO enable bit 7 */
    word ENB8        :1;                                       /* RGPIO enable bit 8 */
    word ENB9        :1;                                       /* RGPIO enable bit 9 */
    word ENB10       :1;                                       /* RGPIO enable bit 10 */
    word ENB11       :1;                                       /* RGPIO enable bit 11 */
    word ENB12       :1;                                       /* RGPIO enable bit 12 */
    word ENB13       :1;                                       /* RGPIO enable bit 13 */
    word ENB14       :1;                                       /* RGPIO enable bit 14 */
    word ENB15       :1;                                       /* RGPIO enable bit 15 */
  } Bits;
} RGPIO_ENBSTR;
extern volatile RGPIO_ENBSTR _RGPIO_ENB @0x00C00004;
#define RGPIO_ENB                       _RGPIO_ENB.Word
#define RGPIO_ENB_ENB0                  _RGPIO_ENB.Bits.ENB0
#define RGPIO_ENB_ENB1                  _RGPIO_ENB.Bits.ENB1
#define RGPIO_ENB_ENB2                  _RGPIO_ENB.Bits.ENB2
#define RGPIO_ENB_ENB3                  _RGPIO_ENB.Bits.ENB3
#define RGPIO_ENB_ENB4                  _RGPIO_ENB.Bits.ENB4
#define RGPIO_ENB_ENB5                  _RGPIO_ENB.Bits.ENB5
#define RGPIO_ENB_ENB6                  _RGPIO_ENB.Bits.ENB6
#define RGPIO_ENB_ENB7                  _RGPIO_ENB.Bits.ENB7
#define RGPIO_ENB_ENB8                  _RGPIO_ENB.Bits.ENB8
#define RGPIO_ENB_ENB9                  _RGPIO_ENB.Bits.ENB9
#define RGPIO_ENB_ENB10                 _RGPIO_ENB.Bits.ENB10
#define RGPIO_ENB_ENB11                 _RGPIO_ENB.Bits.ENB11
#define RGPIO_ENB_ENB12                 _RGPIO_ENB.Bits.ENB12
#define RGPIO_ENB_ENB13                 _RGPIO_ENB.Bits.ENB13
#define RGPIO_ENB_ENB14                 _RGPIO_ENB.Bits.ENB14
#define RGPIO_ENB_ENB15                 _RGPIO_ENB.Bits.ENB15

#define RGPIO_ENB_ENB0_MASK             1
#define RGPIO_ENB_ENB1_MASK             2
#define RGPIO_ENB_ENB2_MASK             4
#define RGPIO_ENB_ENB3_MASK             8
#define RGPIO_ENB_ENB4_MASK             16
#define RGPIO_ENB_ENB5_MASK             32
#define RGPIO_ENB_ENB6_MASK             64
#define RGPIO_ENB_ENB7_MASK             128
#define RGPIO_ENB_ENB8_MASK             256
#define RGPIO_ENB_ENB9_MASK             512
#define RGPIO_ENB_ENB10_MASK            1024
#define RGPIO_ENB_ENB11_MASK            2048
#define RGPIO_ENB_ENB12_MASK            4096
#define RGPIO_ENB_ENB13_MASK            8192
#define RGPIO_ENB_ENB14_MASK            16384
#define RGPIO_ENB_ENB15_MASK            32768


/*** RGPIO_CLR - RGPIO Clear Data Register; 0x00C00006 ***/
typedef union {
  word Word;
  struct {
    word CLR0        :1;                                       /* RGPIO clear data bit 0 */
    word CLR1        :1;                                       /* RGPIO clear data bit 1 */
    word CLR2        :1;                                       /* RGPIO clear data bit 2 */
    word CLR3        :1;                                       /* RGPIO clear data bit 3 */
    word CLR4        :1;                                       /* RGPIO clear data bit 4 */
    word CLR5        :1;                                       /* RGPIO clear data bit 5 */
    word CLR6        :1;                                       /* RGPIO clear data bit 6 */
    word CLR7        :1;                                       /* RGPIO clear data bit 7 */
    word CLR8        :1;                                       /* RGPIO clear data bit 8 */
    word CLR9        :1;                                       /* RGPIO clear data bit 9 */
    word CLR10       :1;                                       /* RGPIO clear data bit 10 */
    word CLR11       :1;                                       /* RGPIO clear data bit 11 */
    word CLR12       :1;                                       /* RGPIO clear data bit 12 */
    word CLR13       :1;                                       /* RGPIO clear data bit 13 */
    word CLR14       :1;                                       /* RGPIO clear data bit 14 */
    word CLR15       :1;                                       /* RGPIO clear data bit 15 */
  } Bits;
} RGPIO_CLRSTR;
extern volatile RGPIO_CLRSTR _RGPIO_CLR @0x00C00006;
#define RGPIO_CLR                       _RGPIO_CLR.Word
#define RGPIO_CLR_CLR0                  _RGPIO_CLR.Bits.CLR0
#define RGPIO_CLR_CLR1                  _RGPIO_CLR.Bits.CLR1
#define RGPIO_CLR_CLR2                  _RGPIO_CLR.Bits.CLR2
#define RGPIO_CLR_CLR3                  _RGPIO_CLR.Bits.CLR3
#define RGPIO_CLR_CLR4                  _RGPIO_CLR.Bits.CLR4
#define RGPIO_CLR_CLR5                  _RGPIO_CLR.Bits.CLR5
#define RGPIO_CLR_CLR6                  _RGPIO_CLR.Bits.CLR6
#define RGPIO_CLR_CLR7                  _RGPIO_CLR.Bits.CLR7
#define RGPIO_CLR_CLR8                  _RGPIO_CLR.Bits.CLR8
#define RGPIO_CLR_CLR9                  _RGPIO_CLR.Bits.CLR9
#define RGPIO_CLR_CLR10                 _RGPIO_CLR.Bits.CLR10
#define RGPIO_CLR_CLR11                 _RGPIO_CLR.Bits.CLR11
#define RGPIO_CLR_CLR12                 _RGPIO_CLR.Bits.CLR12
#define RGPIO_CLR_CLR13                 _RGPIO_CLR.Bits.CLR13
#define RGPIO_CLR_CLR14                 _RGPIO_CLR.Bits.CLR14
#define RGPIO_CLR_CLR15                 _RGPIO_CLR.Bits.CLR15

#define RGPIO_CLR_CLR0_MASK             1
#define RGPIO_CLR_CLR1_MASK             2
#define RGPIO_CLR_CLR2_MASK             4
#define RGPIO_CLR_CLR3_MASK             8
#define RGPIO_CLR_CLR4_MASK             16
#define RGPIO_CLR_CLR5_MASK             32
#define RGPIO_CLR_CLR6_MASK             64
#define RGPIO_CLR_CLR7_MASK             128
#define RGPIO_CLR_CLR8_MASK             256
#define RGPIO_CLR_CLR9_MASK             512
#define RGPIO_CLR_CLR10_MASK            1024
#define RGPIO_CLR_CLR11_MASK            2048
#define RGPIO_CLR_CLR12_MASK            4096
#define RGPIO_CLR_CLR13_MASK            8192
#define RGPIO_CLR_CLR14_MASK            16384
#define RGPIO_CLR_CLR15_MASK            32768


/*** RGPIO_SET - RGPIO Set Data Register; 0x00C0000A ***/
typedef union {
  word Word;
  struct {
    word SET0        :1;                                       /* RGPIO set data bit 0 */
    word SET1        :1;                                       /* RGPIO set data bit 1 */
    word SET2        :1;                                       /* RGPIO set data bit 2 */
    word SET3        :1;                                       /* RGPIO set data bit 3 */
    word SET4        :1;                                       /* RGPIO set data bit 4 */
    word SET5        :1;                                       /* RGPIO set data bit 5 */
    word SET6        :1;                                       /* RGPIO set data bit 6 */
    word SET7        :1;                                       /* RGPIO set data bit 7 */
    word SET8        :1;                                       /* RGPIO set data bit 8 */
    word SET9        :1;                                       /* RGPIO set data bit 9 */
    word SET10       :1;                                       /* RGPIO set data bit 10 */
    word SET11       :1;                                       /* RGPIO set data bit 11 */
    word SET12       :1;                                       /* RGPIO set data bit 12 */
    word SET13       :1;                                       /* RGPIO set data bit 13 */
    word SET14       :1;                                       /* RGPIO set data bit 14 */
    word SET15       :1;                                       /* RGPIO set data bit 15 */
  } Bits;
} RGPIO_SETSTR;
extern volatile RGPIO_SETSTR _RGPIO_SET @0x00C0000A;
#define RGPIO_SET                       _RGPIO_SET.Word
#define RGPIO_SET_SET0                  _RGPIO_SET.Bits.SET0
#define RGPIO_SET_SET1                  _RGPIO_SET.Bits.SET1
#define RGPIO_SET_SET2                  _RGPIO_SET.Bits.SET2
#define RGPIO_SET_SET3                  _RGPIO_SET.Bits.SET3
#define RGPIO_SET_SET4                  _RGPIO_SET.Bits.SET4
#define RGPIO_SET_SET5                  _RGPIO_SET.Bits.SET5
#define RGPIO_SET_SET6                  _RGPIO_SET.Bits.SET6
#define RGPIO_SET_SET7                  _RGPIO_SET.Bits.SET7
#define RGPIO_SET_SET8                  _RGPIO_SET.Bits.SET8
#define RGPIO_SET_SET9                  _RGPIO_SET.Bits.SET9
#define RGPIO_SET_SET10                 _RGPIO_SET.Bits.SET10
#define RGPIO_SET_SET11                 _RGPIO_SET.Bits.SET11
#define RGPIO_SET_SET12                 _RGPIO_SET.Bits.SET12
#define RGPIO_SET_SET13                 _RGPIO_SET.Bits.SET13
#define RGPIO_SET_SET14                 _RGPIO_SET.Bits.SET14
#define RGPIO_SET_SET15                 _RGPIO_SET.Bits.SET15

#define RGPIO_SET_SET0_MASK             1
#define RGPIO_SET_SET1_MASK             2
#define RGPIO_SET_SET2_MASK             4
#define RGPIO_SET_SET3_MASK             8
#define RGPIO_SET_SET4_MASK             16
#define RGPIO_SET_SET5_MASK             32
#define RGPIO_SET_SET6_MASK             64
#define RGPIO_SET_SET7_MASK             128
#define RGPIO_SET_SET8_MASK             256
#define RGPIO_SET_SET9_MASK             512
#define RGPIO_SET_SET10_MASK            1024
#define RGPIO_SET_SET11_MASK            2048
#define RGPIO_SET_SET12_MASK            4096
#define RGPIO_SET_SET13_MASK            8192
#define RGPIO_SET_SET14_MASK            16384
#define RGPIO_SET_SET15_MASK            32768


/*** RGPIO_TOG - RGPIO Toggle Data Register; 0x00C0000E ***/
typedef union {
  word Word;
  struct {
    word TOG0        :1;                                       /* RGPIO toggle data bit 0 */
    word TOG1        :1;                                       /* RGPIO toggle data bit 1 */
    word TOG2        :1;                                       /* RGPIO toggle data bit 2 */
    word TOG3        :1;                                       /* RGPIO toggle data bit 3 */
    word TOG4        :1;                                       /* RGPIO toggle data bit 4 */
    word TOG5        :1;                                       /* RGPIO toggle data bit 5 */
    word TOG6        :1;                                       /* RGPIO toggle data bit 6 */
    word TOG7        :1;                                       /* RGPIO toggle data bit 7 */
    word TOG8        :1;                                       /* RGPIO toggle data bit 8 */
    word TOG9        :1;                                       /* RGPIO toggle data bit 9 */
    word TOG10       :1;                                       /* RGPIO toggle data bit 10 */
    word TOG11       :1;                                       /* RGPIO toggle data bit 11 */
    word TOG12       :1;                                       /* RGPIO toggle data bit 12 */
    word TOG13       :1;                                       /* RGPIO toggle data bit 13 */
    word TOG14       :1;                                       /* RGPIO toggle data bit 14 */
    word TOG15       :1;                                       /* RGPIO toggle data bit 15 */
  } Bits;
} RGPIO_TOGSTR;
extern volatile RGPIO_TOGSTR _RGPIO_TOG @0x00C0000E;
#define RGPIO_TOG                       _RGPIO_TOG.Word
#define RGPIO_TOG_TOG0                  _RGPIO_TOG.Bits.TOG0
#define RGPIO_TOG_TOG1                  _RGPIO_TOG.Bits.TOG1
#define RGPIO_TOG_TOG2                  _RGPIO_TOG.Bits.TOG2
#define RGPIO_TOG_TOG3                  _RGPIO_TOG.Bits.TOG3
#define RGPIO_TOG_TOG4                  _RGPIO_TOG.Bits.TOG4
#define RGPIO_TOG_TOG5                  _RGPIO_TOG.Bits.TOG5
#define RGPIO_TOG_TOG6                  _RGPIO_TOG.Bits.TOG6
#define RGPIO_TOG_TOG7                  _RGPIO_TOG.Bits.TOG7
#define RGPIO_TOG_TOG8                  _RGPIO_TOG.Bits.TOG8
#define RGPIO_TOG_TOG9                  _RGPIO_TOG.Bits.TOG9
#define RGPIO_TOG_TOG10                 _RGPIO_TOG.Bits.TOG10
#define RGPIO_TOG_TOG11                 _RGPIO_TOG.Bits.TOG11
#define RGPIO_TOG_TOG12                 _RGPIO_TOG.Bits.TOG12
#define RGPIO_TOG_TOG13                 _RGPIO_TOG.Bits.TOG13
#define RGPIO_TOG_TOG14                 _RGPIO_TOG.Bits.TOG14
#define RGPIO_TOG_TOG15                 _RGPIO_TOG.Bits.TOG15

#define RGPIO_TOG_TOG0_MASK             1
#define RGPIO_TOG_TOG1_MASK             2
#define RGPIO_TOG_TOG2_MASK             4
#define RGPIO_TOG_TOG3_MASK             8
#define RGPIO_TOG_TOG4_MASK             16
#define RGPIO_TOG_TOG5_MASK             32
#define RGPIO_TOG_TOG6_MASK             64
#define RGPIO_TOG_TOG7_MASK             128
#define RGPIO_TOG_TOG8_MASK             256
#define RGPIO_TOG_TOG9_MASK             512
#define RGPIO_TOG_TOG10_MASK            1024
#define RGPIO_TOG_TOG11_MASK            2048
#define RGPIO_TOG_TOG12_MASK            4096
#define RGPIO_TOG_TOG13_MASK            8192
#define RGPIO_TOG_TOG14_MASK            16384
#define RGPIO_TOG_TOG15_MASK            32768


/*** PTAD - Port A Data Register; 0xFFFF8000 ***/
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
extern volatile PTADSTR _PTAD @0xFFFF8000;
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


/*** PTADD - Port A Data Direction Register; 0xFFFF8001 ***/
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
extern volatile PTADDSTR _PTADD @0xFFFF8001;
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


/*** PTAPV - Port A Pin Value Register; 0xFFFF8002 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTAPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTAPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTAPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTAPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTAPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTAPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTAPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTAPVSTR;
extern volatile PTAPVSTR _PTAPV @0xFFFF8002;
#define PTAPV                           _PTAPV.Byte
#define PTAPV_PTAPV0                    _PTAPV.Bits.PTAPV0
#define PTAPV_PTAPV1                    _PTAPV.Bits.PTAPV1
#define PTAPV_PTAPV2                    _PTAPV.Bits.PTAPV2
#define PTAPV_PTAPV3                    _PTAPV.Bits.PTAPV3
#define PTAPV_PTAPV4                    _PTAPV.Bits.PTAPV4
#define PTAPV_PTAPV5                    _PTAPV.Bits.PTAPV5
#define PTAPV_PTAPV6                    _PTAPV.Bits.PTAPV6
#define PTAPV_PTAPV7                    _PTAPV.Bits.PTAPV7

#define PTAPV_PTAPV0_MASK               1
#define PTAPV_PTAPV1_MASK               2
#define PTAPV_PTAPV2_MASK               4
#define PTAPV_PTAPV3_MASK               8
#define PTAPV_PTAPV4_MASK               16
#define PTAPV_PTAPV5_MASK               32
#define PTAPV_PTAPV6_MASK               64
#define PTAPV_PTAPV7_MASK               128


/*** PTBD - Port B Data Register; 0xFFFF8004 ***/
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
extern volatile PTBDSTR _PTBD @0xFFFF8004;
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


/*** PTBDD - Port B Data Direction Register; 0xFFFF8005 ***/
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
extern volatile PTBDDSTR _PTBDD @0xFFFF8005;
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


/*** PTBPV - Port B Pin Value Register; 0xFFFF8006 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTBPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTBPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTBPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTBPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTBPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTBPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTBPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTBPVSTR;
extern volatile PTBPVSTR _PTBPV @0xFFFF8006;
#define PTBPV                           _PTBPV.Byte
#define PTBPV_PTBPV0                    _PTBPV.Bits.PTBPV0
#define PTBPV_PTBPV1                    _PTBPV.Bits.PTBPV1
#define PTBPV_PTBPV2                    _PTBPV.Bits.PTBPV2
#define PTBPV_PTBPV3                    _PTBPV.Bits.PTBPV3
#define PTBPV_PTBPV4                    _PTBPV.Bits.PTBPV4
#define PTBPV_PTBPV5                    _PTBPV.Bits.PTBPV5
#define PTBPV_PTBPV6                    _PTBPV.Bits.PTBPV6
#define PTBPV_PTBPV7                    _PTBPV.Bits.PTBPV7

#define PTBPV_PTBPV0_MASK               1
#define PTBPV_PTBPV1_MASK               2
#define PTBPV_PTBPV2_MASK               4
#define PTBPV_PTBPV3_MASK               8
#define PTBPV_PTBPV4_MASK               16
#define PTBPV_PTBPV5_MASK               32
#define PTBPV_PTBPV6_MASK               64
#define PTBPV_PTBPV7_MASK               128


/*** PTCD - Port C Data Register; 0xFFFF8008 ***/
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
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCD :7;
    byte         :1;
  } MergedBits;
} PTCDSTR;
extern volatile PTCDSTR _PTCD @0xFFFF8008;
#define PTCD                            _PTCD.Byte
#define PTCD_PTCD0                      _PTCD.Bits.PTCD0
#define PTCD_PTCD1                      _PTCD.Bits.PTCD1
#define PTCD_PTCD2                      _PTCD.Bits.PTCD2
#define PTCD_PTCD3                      _PTCD.Bits.PTCD3
#define PTCD_PTCD4                      _PTCD.Bits.PTCD4
#define PTCD_PTCD5                      _PTCD.Bits.PTCD5
#define PTCD_PTCD6                      _PTCD.Bits.PTCD6
#define PTCD_PTCD                       _PTCD.MergedBits.grpPTCD

#define PTCD_PTCD0_MASK                 1
#define PTCD_PTCD1_MASK                 2
#define PTCD_PTCD2_MASK                 4
#define PTCD_PTCD3_MASK                 8
#define PTCD_PTCD4_MASK                 16
#define PTCD_PTCD5_MASK                 32
#define PTCD_PTCD6_MASK                 64
#define PTCD_PTCD_MASK                  127
#define PTCD_PTCD_BITNUM                0


/*** PTCDD - Port C Data Direction Register; 0xFFFF8009 ***/
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
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCDD :7;
    byte         :1;
  } MergedBits;
} PTCDDSTR;
extern volatile PTCDDSTR _PTCDD @0xFFFF8009;
#define PTCDD                           _PTCDD.Byte
#define PTCDD_PTCDD0                    _PTCDD.Bits.PTCDD0
#define PTCDD_PTCDD1                    _PTCDD.Bits.PTCDD1
#define PTCDD_PTCDD2                    _PTCDD.Bits.PTCDD2
#define PTCDD_PTCDD3                    _PTCDD.Bits.PTCDD3
#define PTCDD_PTCDD4                    _PTCDD.Bits.PTCDD4
#define PTCDD_PTCDD5                    _PTCDD.Bits.PTCDD5
#define PTCDD_PTCDD6                    _PTCDD.Bits.PTCDD6
#define PTCDD_PTCDD                     _PTCDD.MergedBits.grpPTCDD

#define PTCDD_PTCDD0_MASK               1
#define PTCDD_PTCDD1_MASK               2
#define PTCDD_PTCDD2_MASK               4
#define PTCDD_PTCDD3_MASK               8
#define PTCDD_PTCDD4_MASK               16
#define PTCDD_PTCDD5_MASK               32
#define PTCDD_PTCDD6_MASK               64
#define PTCDD_PTCDD_MASK                127
#define PTCDD_PTCDD_BITNUM              0


/*** PTCPV - Port C Pin Value Register; 0xFFFF800A ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTCPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTCPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTCPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTCPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTCPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTCPV6      :1;                                       /* Pin Value Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCPV :7;
    byte         :1;
  } MergedBits;
} PTCPVSTR;
extern volatile PTCPVSTR _PTCPV @0xFFFF800A;
#define PTCPV                           _PTCPV.Byte
#define PTCPV_PTCPV0                    _PTCPV.Bits.PTCPV0
#define PTCPV_PTCPV1                    _PTCPV.Bits.PTCPV1
#define PTCPV_PTCPV2                    _PTCPV.Bits.PTCPV2
#define PTCPV_PTCPV3                    _PTCPV.Bits.PTCPV3
#define PTCPV_PTCPV4                    _PTCPV.Bits.PTCPV4
#define PTCPV_PTCPV5                    _PTCPV.Bits.PTCPV5
#define PTCPV_PTCPV6                    _PTCPV.Bits.PTCPV6
#define PTCPV_PTCPV                     _PTCPV.MergedBits.grpPTCPV

#define PTCPV_PTCPV0_MASK               1
#define PTCPV_PTCPV1_MASK               2
#define PTCPV_PTCPV2_MASK               4
#define PTCPV_PTCPV3_MASK               8
#define PTCPV_PTCPV4_MASK               16
#define PTCPV_PTCPV5_MASK               32
#define PTCPV_PTCPV6_MASK               64
#define PTCPV_PTCPV_MASK                127
#define PTCPV_PTCPV_BITNUM              0


/*** PTDD - Port D Data Register; 0xFFFF800C ***/
typedef union {
  byte Byte;
  struct {
    byte PTDD0       :1;                                       /* Port D Data Register Bit 0 */
    byte PTDD1       :1;                                       /* Port D Data Register Bit 1 */
    byte PTDD2       :1;                                       /* Port D Data Register Bit 2 */
    byte PTDD3       :1;                                       /* Port D Data Register Bit 3 */
    byte PTDD4       :1;                                       /* Port D Data Register Bit 4 */
    byte PTDD5       :1;                                       /* Port D Data Register Bit 5 */
    byte PTDD6       :1;                                       /* Port D Data Register Bit 6 */
    byte PTDD7       :1;                                       /* Port D Data Register Bit 7 */
  } Bits;
} PTDDSTR;
extern volatile PTDDSTR _PTDD @0xFFFF800C;
#define PTDD                            _PTDD.Byte
#define PTDD_PTDD0                      _PTDD.Bits.PTDD0
#define PTDD_PTDD1                      _PTDD.Bits.PTDD1
#define PTDD_PTDD2                      _PTDD.Bits.PTDD2
#define PTDD_PTDD3                      _PTDD.Bits.PTDD3
#define PTDD_PTDD4                      _PTDD.Bits.PTDD4
#define PTDD_PTDD5                      _PTDD.Bits.PTDD5
#define PTDD_PTDD6                      _PTDD.Bits.PTDD6
#define PTDD_PTDD7                      _PTDD.Bits.PTDD7

#define PTDD_PTDD0_MASK                 1
#define PTDD_PTDD1_MASK                 2
#define PTDD_PTDD2_MASK                 4
#define PTDD_PTDD3_MASK                 8
#define PTDD_PTDD4_MASK                 16
#define PTDD_PTDD5_MASK                 32
#define PTDD_PTDD6_MASK                 64
#define PTDD_PTDD7_MASK                 128


/*** PTDDD - Port D Data Direction Register; 0xFFFF800D ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDD0      :1;                                       /* Data Direction for Port D Bit 0 */
    byte PTDDD1      :1;                                       /* Data Direction for Port D Bit 1 */
    byte PTDDD2      :1;                                       /* Data Direction for Port D Bit 2 */
    byte PTDDD3      :1;                                       /* Data Direction for Port D Bit 3 */
    byte PTDDD4      :1;                                       /* Data Direction for Port D Bit 4 */
    byte PTDDD5      :1;                                       /* Data Direction for Port D Bit 5 */
    byte PTDDD6      :1;                                       /* Data Direction for Port D Bit 6 */
    byte PTDDD7      :1;                                       /* Data Direction for Port D Bit 7 */
  } Bits;
} PTDDDSTR;
extern volatile PTDDDSTR _PTDDD @0xFFFF800D;
#define PTDDD                           _PTDDD.Byte
#define PTDDD_PTDDD0                    _PTDDD.Bits.PTDDD0
#define PTDDD_PTDDD1                    _PTDDD.Bits.PTDDD1
#define PTDDD_PTDDD2                    _PTDDD.Bits.PTDDD2
#define PTDDD_PTDDD3                    _PTDDD.Bits.PTDDD3
#define PTDDD_PTDDD4                    _PTDDD.Bits.PTDDD4
#define PTDDD_PTDDD5                    _PTDDD.Bits.PTDDD5
#define PTDDD_PTDDD6                    _PTDDD.Bits.PTDDD6
#define PTDDD_PTDDD7                    _PTDDD.Bits.PTDDD7

#define PTDDD_PTDDD0_MASK               1
#define PTDDD_PTDDD1_MASK               2
#define PTDDD_PTDDD2_MASK               4
#define PTDDD_PTDDD3_MASK               8
#define PTDDD_PTDDD4_MASK               16
#define PTDDD_PTDDD5_MASK               32
#define PTDDD_PTDDD6_MASK               64
#define PTDDD_PTDDD7_MASK               128


/*** PTDPV - Port D Pin Value Register; 0xFFFF800E ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTDPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTDPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTDPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTDPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTDPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTDPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTDPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTDPVSTR;
extern volatile PTDPVSTR _PTDPV @0xFFFF800E;
#define PTDPV                           _PTDPV.Byte
#define PTDPV_PTDPV0                    _PTDPV.Bits.PTDPV0
#define PTDPV_PTDPV1                    _PTDPV.Bits.PTDPV1
#define PTDPV_PTDPV2                    _PTDPV.Bits.PTDPV2
#define PTDPV_PTDPV3                    _PTDPV.Bits.PTDPV3
#define PTDPV_PTDPV4                    _PTDPV.Bits.PTDPV4
#define PTDPV_PTDPV5                    _PTDPV.Bits.PTDPV5
#define PTDPV_PTDPV6                    _PTDPV.Bits.PTDPV6
#define PTDPV_PTDPV7                    _PTDPV.Bits.PTDPV7

#define PTDPV_PTDPV0_MASK               1
#define PTDPV_PTDPV1_MASK               2
#define PTDPV_PTDPV2_MASK               4
#define PTDPV_PTDPV3_MASK               8
#define PTDPV_PTDPV4_MASK               16
#define PTDPV_PTDPV5_MASK               32
#define PTDPV_PTDPV6_MASK               64
#define PTDPV_PTDPV7_MASK               128


/*** PTED - Port E Data Register; 0xFFFF8010 ***/
typedef union {
  byte Byte;
  struct {
    byte PTED0       :1;                                       /* Port E Data Register Bit 0 */
    byte PTED1       :1;                                       /* Port E Data Register Bit 1 */
    byte PTED2       :1;                                       /* Port E Data Register Bit 2 */
    byte PTED3       :1;                                       /* Port E Data Register Bit 3 */
    byte PTED4       :1;                                       /* Port E Data Register Bit 4 */
    byte PTED5       :1;                                       /* Port E Data Register Bit 5 */
    byte PTED6       :1;                                       /* Port E Data Register Bit 6 */
    byte PTED7       :1;                                       /* Port E Data Register Bit 7 */
  } Bits;
} PTEDSTR;
extern volatile PTEDSTR _PTED @0xFFFF8010;
#define PTED                            _PTED.Byte
#define PTED_PTED0                      _PTED.Bits.PTED0
#define PTED_PTED1                      _PTED.Bits.PTED1
#define PTED_PTED2                      _PTED.Bits.PTED2
#define PTED_PTED3                      _PTED.Bits.PTED3
#define PTED_PTED4                      _PTED.Bits.PTED4
#define PTED_PTED5                      _PTED.Bits.PTED5
#define PTED_PTED6                      _PTED.Bits.PTED6
#define PTED_PTED7                      _PTED.Bits.PTED7

#define PTED_PTED0_MASK                 1
#define PTED_PTED1_MASK                 2
#define PTED_PTED2_MASK                 4
#define PTED_PTED3_MASK                 8
#define PTED_PTED4_MASK                 16
#define PTED_PTED5_MASK                 32
#define PTED_PTED6_MASK                 64
#define PTED_PTED7_MASK                 128


/*** PTEDD - Port E Data Direction Register; 0xFFFF8011 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDD0      :1;                                       /* Data Direction for Port E Bit 0 */
    byte PTEDD1      :1;                                       /* Data Direction for Port E Bit 1 */
    byte PTEDD2      :1;                                       /* Data Direction for Port E Bit 2 */
    byte PTEDD3      :1;                                       /* Data Direction for Port E Bit 3 */
    byte PTEDD4      :1;                                       /* Data Direction for Port E Bit 4 */
    byte PTEDD5      :1;                                       /* Data Direction for Port E Bit 5 */
    byte PTEDD6      :1;                                       /* Data Direction for Port E Bit 6 */
    byte PTEDD7      :1;                                       /* Data Direction for Port E Bit 7 */
  } Bits;
} PTEDDSTR;
extern volatile PTEDDSTR _PTEDD @0xFFFF8011;
#define PTEDD                           _PTEDD.Byte
#define PTEDD_PTEDD0                    _PTEDD.Bits.PTEDD0
#define PTEDD_PTEDD1                    _PTEDD.Bits.PTEDD1
#define PTEDD_PTEDD2                    _PTEDD.Bits.PTEDD2
#define PTEDD_PTEDD3                    _PTEDD.Bits.PTEDD3
#define PTEDD_PTEDD4                    _PTEDD.Bits.PTEDD4
#define PTEDD_PTEDD5                    _PTEDD.Bits.PTEDD5
#define PTEDD_PTEDD6                    _PTEDD.Bits.PTEDD6
#define PTEDD_PTEDD7                    _PTEDD.Bits.PTEDD7

#define PTEDD_PTEDD0_MASK               1
#define PTEDD_PTEDD1_MASK               2
#define PTEDD_PTEDD2_MASK               4
#define PTEDD_PTEDD3_MASK               8
#define PTEDD_PTEDD4_MASK               16
#define PTEDD_PTEDD5_MASK               32
#define PTEDD_PTEDD6_MASK               64
#define PTEDD_PTEDD7_MASK               128


/*** PTEPV - Port E Pin Value Register; 0xFFFF8012 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTEPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTEPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTEPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTEPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTEPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTEPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTEPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTEPVSTR;
extern volatile PTEPVSTR _PTEPV @0xFFFF8012;
#define PTEPV                           _PTEPV.Byte
#define PTEPV_PTEPV0                    _PTEPV.Bits.PTEPV0
#define PTEPV_PTEPV1                    _PTEPV.Bits.PTEPV1
#define PTEPV_PTEPV2                    _PTEPV.Bits.PTEPV2
#define PTEPV_PTEPV3                    _PTEPV.Bits.PTEPV3
#define PTEPV_PTEPV4                    _PTEPV.Bits.PTEPV4
#define PTEPV_PTEPV5                    _PTEPV.Bits.PTEPV5
#define PTEPV_PTEPV6                    _PTEPV.Bits.PTEPV6
#define PTEPV_PTEPV7                    _PTEPV.Bits.PTEPV7

#define PTEPV_PTEPV0_MASK               1
#define PTEPV_PTEPV1_MASK               2
#define PTEPV_PTEPV2_MASK               4
#define PTEPV_PTEPV3_MASK               8
#define PTEPV_PTEPV4_MASK               16
#define PTEPV_PTEPV5_MASK               32
#define PTEPV_PTEPV6_MASK               64
#define PTEPV_PTEPV7_MASK               128


/*** PTFD - Port F Data Register; 0xFFFF8014 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFD0       :1;                                       /* Port F Data Register Bit 0 */
    byte PTFD1       :1;                                       /* Port F Data Register Bit 1 */
    byte PTFD2       :1;                                       /* Port F Data Register Bit 2 */
    byte PTFD3       :1;                                       /* Port F Data Register Bit 3 */
    byte PTFD4       :1;                                       /* Port F Data Register Bit 4 */
    byte PTFD5       :1;                                       /* Port F Data Register Bit 5 */
    byte PTFD6       :1;                                       /* Port F Data Register Bit 6 */
    byte PTFD7       :1;                                       /* Port F Data Register Bit 7 */
  } Bits;
} PTFDSTR;
extern volatile PTFDSTR _PTFD @0xFFFF8014;
#define PTFD                            _PTFD.Byte
#define PTFD_PTFD0                      _PTFD.Bits.PTFD0
#define PTFD_PTFD1                      _PTFD.Bits.PTFD1
#define PTFD_PTFD2                      _PTFD.Bits.PTFD2
#define PTFD_PTFD3                      _PTFD.Bits.PTFD3
#define PTFD_PTFD4                      _PTFD.Bits.PTFD4
#define PTFD_PTFD5                      _PTFD.Bits.PTFD5
#define PTFD_PTFD6                      _PTFD.Bits.PTFD6
#define PTFD_PTFD7                      _PTFD.Bits.PTFD7

#define PTFD_PTFD0_MASK                 1
#define PTFD_PTFD1_MASK                 2
#define PTFD_PTFD2_MASK                 4
#define PTFD_PTFD3_MASK                 8
#define PTFD_PTFD4_MASK                 16
#define PTFD_PTFD5_MASK                 32
#define PTFD_PTFD6_MASK                 64
#define PTFD_PTFD7_MASK                 128


/*** PTFDD - Port F Data Direction Register; 0xFFFF8015 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFDD0      :1;                                       /* Data Direction for Port F Bit 0 */
    byte PTFDD1      :1;                                       /* Data Direction for Port F Bit 1 */
    byte PTFDD2      :1;                                       /* Data Direction for Port F Bit 2 */
    byte PTFDD3      :1;                                       /* Data Direction for Port F Bit 3 */
    byte PTFDD4      :1;                                       /* Data Direction for Port F Bit 4 */
    byte PTFDD5      :1;                                       /* Data Direction for Port F Bit 5 */
    byte PTFDD6      :1;                                       /* Data Direction for Port F Bit 6 */
    byte PTFDD7      :1;                                       /* Data Direction for Port F Bit 7 */
  } Bits;
} PTFDDSTR;
extern volatile PTFDDSTR _PTFDD @0xFFFF8015;
#define PTFDD                           _PTFDD.Byte
#define PTFDD_PTFDD0                    _PTFDD.Bits.PTFDD0
#define PTFDD_PTFDD1                    _PTFDD.Bits.PTFDD1
#define PTFDD_PTFDD2                    _PTFDD.Bits.PTFDD2
#define PTFDD_PTFDD3                    _PTFDD.Bits.PTFDD3
#define PTFDD_PTFDD4                    _PTFDD.Bits.PTFDD4
#define PTFDD_PTFDD5                    _PTFDD.Bits.PTFDD5
#define PTFDD_PTFDD6                    _PTFDD.Bits.PTFDD6
#define PTFDD_PTFDD7                    _PTFDD.Bits.PTFDD7

#define PTFDD_PTFDD0_MASK               1
#define PTFDD_PTFDD1_MASK               2
#define PTFDD_PTFDD2_MASK               4
#define PTFDD_PTFDD3_MASK               8
#define PTFDD_PTFDD4_MASK               16
#define PTFDD_PTFDD5_MASK               32
#define PTFDD_PTFDD6_MASK               64
#define PTFDD_PTFDD7_MASK               128


/*** PTFPV - Port F Pin Value Register; 0xFFFF8016 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTFPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTFPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTFPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTFPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTFPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTFPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTFPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTFPVSTR;
extern volatile PTFPVSTR _PTFPV @0xFFFF8016;
#define PTFPV                           _PTFPV.Byte
#define PTFPV_PTFPV0                    _PTFPV.Bits.PTFPV0
#define PTFPV_PTFPV1                    _PTFPV.Bits.PTFPV1
#define PTFPV_PTFPV2                    _PTFPV.Bits.PTFPV2
#define PTFPV_PTFPV3                    _PTFPV.Bits.PTFPV3
#define PTFPV_PTFPV4                    _PTFPV.Bits.PTFPV4
#define PTFPV_PTFPV5                    _PTFPV.Bits.PTFPV5
#define PTFPV_PTFPV6                    _PTFPV.Bits.PTFPV6
#define PTFPV_PTFPV7                    _PTFPV.Bits.PTFPV7

#define PTFPV_PTFPV0_MASK               1
#define PTFPV_PTFPV1_MASK               2
#define PTFPV_PTFPV2_MASK               4
#define PTFPV_PTFPV3_MASK               8
#define PTFPV_PTFPV4_MASK               16
#define PTFPV_PTFPV5_MASK               32
#define PTFPV_PTFPV6_MASK               64
#define PTFPV_PTFPV7_MASK               128


/*** PTGD - Port G Data Register; 0xFFFF8018 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGD0       :1;                                       /* Port G Data Register Bit 0 */
    byte PTGD1       :1;                                       /* Port G Data Register Bit 1 */
    byte PTGD2       :1;                                       /* Port G Data Register Bit 2 */
    byte PTGD3       :1;                                       /* Port G Data Register Bit 3 */
    byte PTGD4       :1;                                       /* Port G Data Register Bit 4 */
    byte PTGD5       :1;                                       /* Port G Data Register Bit 5 */
    byte PTGD6       :1;                                       /* Port G Data Register Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGD :7;
    byte         :1;
  } MergedBits;
} PTGDSTR;
extern volatile PTGDSTR _PTGD @0xFFFF8018;
#define PTGD                            _PTGD.Byte
#define PTGD_PTGD0                      _PTGD.Bits.PTGD0
#define PTGD_PTGD1                      _PTGD.Bits.PTGD1
#define PTGD_PTGD2                      _PTGD.Bits.PTGD2
#define PTGD_PTGD3                      _PTGD.Bits.PTGD3
#define PTGD_PTGD4                      _PTGD.Bits.PTGD4
#define PTGD_PTGD5                      _PTGD.Bits.PTGD5
#define PTGD_PTGD6                      _PTGD.Bits.PTGD6
#define PTGD_PTGD                       _PTGD.MergedBits.grpPTGD

#define PTGD_PTGD0_MASK                 1
#define PTGD_PTGD1_MASK                 2
#define PTGD_PTGD2_MASK                 4
#define PTGD_PTGD3_MASK                 8
#define PTGD_PTGD4_MASK                 16
#define PTGD_PTGD5_MASK                 32
#define PTGD_PTGD6_MASK                 64
#define PTGD_PTGD_MASK                  127
#define PTGD_PTGD_BITNUM                0


/*** PTGDD - Port G Data Direction Register; 0xFFFF8019 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDD0      :1;                                       /* Data Direction for Port G Bit 0 */
    byte PTGDD1      :1;                                       /* Data Direction for Port G Bit 1 */
    byte PTGDD2      :1;                                       /* Data Direction for Port G Bit 2 */
    byte PTGDD3      :1;                                       /* Data Direction for Port G Bit 3 */
    byte PTGDD4      :1;                                       /* Data Direction for Port G Bit 4 */
    byte PTGDD5      :1;                                       /* Data Direction for Port G Bit 5 */
    byte PTGDD6      :1;                                       /* Data Direction for Port G Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDD :7;
    byte         :1;
  } MergedBits;
} PTGDDSTR;
extern volatile PTGDDSTR _PTGDD @0xFFFF8019;
#define PTGDD                           _PTGDD.Byte
#define PTGDD_PTGDD0                    _PTGDD.Bits.PTGDD0
#define PTGDD_PTGDD1                    _PTGDD.Bits.PTGDD1
#define PTGDD_PTGDD2                    _PTGDD.Bits.PTGDD2
#define PTGDD_PTGDD3                    _PTGDD.Bits.PTGDD3
#define PTGDD_PTGDD4                    _PTGDD.Bits.PTGDD4
#define PTGDD_PTGDD5                    _PTGDD.Bits.PTGDD5
#define PTGDD_PTGDD6                    _PTGDD.Bits.PTGDD6
#define PTGDD_PTGDD                     _PTGDD.MergedBits.grpPTGDD

#define PTGDD_PTGDD0_MASK               1
#define PTGDD_PTGDD1_MASK               2
#define PTGDD_PTGDD2_MASK               4
#define PTGDD_PTGDD3_MASK               8
#define PTGDD_PTGDD4_MASK               16
#define PTGDD_PTGDD5_MASK               32
#define PTGDD_PTGDD6_MASK               64
#define PTGDD_PTGDD_MASK                127
#define PTGDD_PTGDD_BITNUM              0


/*** PTGPV - Port G Pin Value Register; 0xFFFF801A ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTGPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTGPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTGPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTGPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTGPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTGPV6      :1;                                       /* Pin Value Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPV :7;
    byte         :1;
  } MergedBits;
} PTGPVSTR;
extern volatile PTGPVSTR _PTGPV @0xFFFF801A;
#define PTGPV                           _PTGPV.Byte
#define PTGPV_PTGPV0                    _PTGPV.Bits.PTGPV0
#define PTGPV_PTGPV1                    _PTGPV.Bits.PTGPV1
#define PTGPV_PTGPV2                    _PTGPV.Bits.PTGPV2
#define PTGPV_PTGPV3                    _PTGPV.Bits.PTGPV3
#define PTGPV_PTGPV4                    _PTGPV.Bits.PTGPV4
#define PTGPV_PTGPV5                    _PTGPV.Bits.PTGPV5
#define PTGPV_PTGPV6                    _PTGPV.Bits.PTGPV6
#define PTGPV_PTGPV                     _PTGPV.MergedBits.grpPTGPV

#define PTGPV_PTGPV0_MASK               1
#define PTGPV_PTGPV1_MASK               2
#define PTGPV_PTGPV2_MASK               4
#define PTGPV_PTGPV3_MASK               8
#define PTGPV_PTGPV4_MASK               16
#define PTGPV_PTGPV5_MASK               32
#define PTGPV_PTGPV6_MASK               64
#define PTGPV_PTGPV_MASK                127
#define PTGPV_PTGPV_BITNUM              0


/*** PTHD - Port H Data Register; 0xFFFF801C ***/
typedef union {
  byte Byte;
  struct {
    byte PTHD0       :1;                                       /* Port H Data Register Bit 0 */
    byte PTHD1       :1;                                       /* Port H Data Register Bit 1 */
    byte PTHD2       :1;                                       /* Port H Data Register Bit 2 */
    byte PTHD3       :1;                                       /* Port H Data Register Bit 3 */
    byte PTHD4       :1;                                       /* Port H Data Register Bit 4 */
    byte PTHD5       :1;                                       /* Port H Data Register Bit 5 */
    byte PTHD6       :1;                                       /* Port H Data Register Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHD :7;
    byte         :1;
  } MergedBits;
} PTHDSTR;
extern volatile PTHDSTR _PTHD @0xFFFF801C;
#define PTHD                            _PTHD.Byte
#define PTHD_PTHD0                      _PTHD.Bits.PTHD0
#define PTHD_PTHD1                      _PTHD.Bits.PTHD1
#define PTHD_PTHD2                      _PTHD.Bits.PTHD2
#define PTHD_PTHD3                      _PTHD.Bits.PTHD3
#define PTHD_PTHD4                      _PTHD.Bits.PTHD4
#define PTHD_PTHD5                      _PTHD.Bits.PTHD5
#define PTHD_PTHD6                      _PTHD.Bits.PTHD6
#define PTHD_PTHD                       _PTHD.MergedBits.grpPTHD

#define PTHD_PTHD0_MASK                 1
#define PTHD_PTHD1_MASK                 2
#define PTHD_PTHD2_MASK                 4
#define PTHD_PTHD3_MASK                 8
#define PTHD_PTHD4_MASK                 16
#define PTHD_PTHD5_MASK                 32
#define PTHD_PTHD6_MASK                 64
#define PTHD_PTHD_MASK                  127
#define PTHD_PTHD_BITNUM                0


/*** PTHDD - Port H Data Direction Register; 0xFFFF801D ***/
typedef union {
  byte Byte;
  struct {
    byte PTHDD0      :1;                                       /* Data Direction for Port H Bit 0 */
    byte PTHDD1      :1;                                       /* Data Direction for Port H Bit 1 */
    byte PTHDD2      :1;                                       /* Data Direction for Port H Bit 2 */
    byte PTHDD3      :1;                                       /* Data Direction for Port H Bit 3 */
    byte PTHDD4      :1;                                       /* Data Direction for Port H Bit 4 */
    byte PTHDD5      :1;                                       /* Data Direction for Port H Bit 5 */
    byte PTHDD6      :1;                                       /* Data Direction for Port H Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHDD :7;
    byte         :1;
  } MergedBits;
} PTHDDSTR;
extern volatile PTHDDSTR _PTHDD @0xFFFF801D;
#define PTHDD                           _PTHDD.Byte
#define PTHDD_PTHDD0                    _PTHDD.Bits.PTHDD0
#define PTHDD_PTHDD1                    _PTHDD.Bits.PTHDD1
#define PTHDD_PTHDD2                    _PTHDD.Bits.PTHDD2
#define PTHDD_PTHDD3                    _PTHDD.Bits.PTHDD3
#define PTHDD_PTHDD4                    _PTHDD.Bits.PTHDD4
#define PTHDD_PTHDD5                    _PTHDD.Bits.PTHDD5
#define PTHDD_PTHDD6                    _PTHDD.Bits.PTHDD6
#define PTHDD_PTHDD                     _PTHDD.MergedBits.grpPTHDD

#define PTHDD_PTHDD0_MASK               1
#define PTHDD_PTHDD1_MASK               2
#define PTHDD_PTHDD2_MASK               4
#define PTHDD_PTHDD3_MASK               8
#define PTHDD_PTHDD4_MASK               16
#define PTHDD_PTHDD5_MASK               32
#define PTHDD_PTHDD6_MASK               64
#define PTHDD_PTHDD_MASK                127
#define PTHDD_PTHDD_BITNUM              0


/*** PTHPV - Port H Pin Value Register; 0xFFFF801E ***/
typedef union {
  byte Byte;
  struct {
    byte PTHPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTHPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTHPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTHPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTHPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTHPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTHPV6      :1;                                       /* Pin Value Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHPV :7;
    byte         :1;
  } MergedBits;
} PTHPVSTR;
extern volatile PTHPVSTR _PTHPV @0xFFFF801E;
#define PTHPV                           _PTHPV.Byte
#define PTHPV_PTHPV0                    _PTHPV.Bits.PTHPV0
#define PTHPV_PTHPV1                    _PTHPV.Bits.PTHPV1
#define PTHPV_PTHPV2                    _PTHPV.Bits.PTHPV2
#define PTHPV_PTHPV3                    _PTHPV.Bits.PTHPV3
#define PTHPV_PTHPV4                    _PTHPV.Bits.PTHPV4
#define PTHPV_PTHPV5                    _PTHPV.Bits.PTHPV5
#define PTHPV_PTHPV6                    _PTHPV.Bits.PTHPV6
#define PTHPV_PTHPV                     _PTHPV.MergedBits.grpPTHPV

#define PTHPV_PTHPV0_MASK               1
#define PTHPV_PTHPV1_MASK               2
#define PTHPV_PTHPV2_MASK               4
#define PTHPV_PTHPV3_MASK               8
#define PTHPV_PTHPV4_MASK               16
#define PTHPV_PTHPV5_MASK               32
#define PTHPV_PTHPV6_MASK               64
#define PTHPV_PTHPV_MASK                127
#define PTHPV_PTHPV_BITNUM              0


/*** PTJD - Port J Data Register; 0xFFFF8020 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJD0       :1;                                       /* Port J Data Register Bit 0 */
    byte PTJD1       :1;                                       /* Port J Data Register Bit 1 */
    byte PTJD2       :1;                                       /* Port J Data Register Bit 2 */
    byte PTJD3       :1;                                       /* Port J Data Register Bit 3 */
    byte PTJD4       :1;                                       /* Port J Data Register Bit 4 */
    byte PTJD5       :1;                                       /* Port J Data Register Bit 5 */
    byte PTJD6       :1;                                       /* Port J Data Register Bit 6 */
    byte PTJD7       :1;                                       /* Port J Data Register Bit 7 */
  } Bits;
} PTJDSTR;
extern volatile PTJDSTR _PTJD @0xFFFF8020;
#define PTJD                            _PTJD.Byte
#define PTJD_PTJD0                      _PTJD.Bits.PTJD0
#define PTJD_PTJD1                      _PTJD.Bits.PTJD1
#define PTJD_PTJD2                      _PTJD.Bits.PTJD2
#define PTJD_PTJD3                      _PTJD.Bits.PTJD3
#define PTJD_PTJD4                      _PTJD.Bits.PTJD4
#define PTJD_PTJD5                      _PTJD.Bits.PTJD5
#define PTJD_PTJD6                      _PTJD.Bits.PTJD6
#define PTJD_PTJD7                      _PTJD.Bits.PTJD7

#define PTJD_PTJD0_MASK                 1
#define PTJD_PTJD1_MASK                 2
#define PTJD_PTJD2_MASK                 4
#define PTJD_PTJD3_MASK                 8
#define PTJD_PTJD4_MASK                 16
#define PTJD_PTJD5_MASK                 32
#define PTJD_PTJD6_MASK                 64
#define PTJD_PTJD7_MASK                 128


/*** PTJDD - Port J Data Direction Register; 0xFFFF8021 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJDD0      :1;                                       /* Data Direction for Port J Bit 0 */
    byte PTJDD1      :1;                                       /* Data Direction for Port J Bit 1 */
    byte PTJDD2      :1;                                       /* Data Direction for Port J Bit 2 */
    byte PTJDD3      :1;                                       /* Data Direction for Port J Bit 3 */
    byte PTJDD4      :1;                                       /* Data Direction for Port J Bit 4 */
    byte PTJDD5      :1;                                       /* Data Direction for Port J Bit 5 */
    byte PTJDD6      :1;                                       /* Data Direction for Port J Bit 6 */
    byte PTJDD7      :1;                                       /* Data Direction for Port J Bit 7 */
  } Bits;
} PTJDDSTR;
extern volatile PTJDDSTR _PTJDD @0xFFFF8021;
#define PTJDD                           _PTJDD.Byte
#define PTJDD_PTJDD0                    _PTJDD.Bits.PTJDD0
#define PTJDD_PTJDD1                    _PTJDD.Bits.PTJDD1
#define PTJDD_PTJDD2                    _PTJDD.Bits.PTJDD2
#define PTJDD_PTJDD3                    _PTJDD.Bits.PTJDD3
#define PTJDD_PTJDD4                    _PTJDD.Bits.PTJDD4
#define PTJDD_PTJDD5                    _PTJDD.Bits.PTJDD5
#define PTJDD_PTJDD6                    _PTJDD.Bits.PTJDD6
#define PTJDD_PTJDD7                    _PTJDD.Bits.PTJDD7

#define PTJDD_PTJDD0_MASK               1
#define PTJDD_PTJDD1_MASK               2
#define PTJDD_PTJDD2_MASK               4
#define PTJDD_PTJDD3_MASK               8
#define PTJDD_PTJDD4_MASK               16
#define PTJDD_PTJDD5_MASK               32
#define PTJDD_PTJDD6_MASK               64
#define PTJDD_PTJDD7_MASK               128


/*** PTJPV - Port J Pin Value Register; 0xFFFF8022 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJPV0      :1;                                       /* Pin Value Bit 0 */
    byte PTJPV1      :1;                                       /* Pin Value Bit 1 */
    byte PTJPV2      :1;                                       /* Pin Value Bit 2 */
    byte PTJPV3      :1;                                       /* Pin Value Bit 3 */
    byte PTJPV4      :1;                                       /* Pin Value Bit 4 */
    byte PTJPV5      :1;                                       /* Pin Value Bit 5 */
    byte PTJPV6      :1;                                       /* Pin Value Bit 6 */
    byte PTJPV7      :1;                                       /* Pin Value Bit 7 */
  } Bits;
} PTJPVSTR;
extern volatile PTJPVSTR _PTJPV @0xFFFF8022;
#define PTJPV                           _PTJPV.Byte
#define PTJPV_PTJPV0                    _PTJPV.Bits.PTJPV0
#define PTJPV_PTJPV1                    _PTJPV.Bits.PTJPV1
#define PTJPV_PTJPV2                    _PTJPV.Bits.PTJPV2
#define PTJPV_PTJPV3                    _PTJPV.Bits.PTJPV3
#define PTJPV_PTJPV4                    _PTJPV.Bits.PTJPV4
#define PTJPV_PTJPV5                    _PTJPV.Bits.PTJPV5
#define PTJPV_PTJPV6                    _PTJPV.Bits.PTJPV6
#define PTJPV_PTJPV7                    _PTJPV.Bits.PTJPV7

#define PTJPV_PTJPV0_MASK               1
#define PTJPV_PTJPV1_MASK               2
#define PTJPV_PTJPV2_MASK               4
#define PTJPV_PTJPV3_MASK               8
#define PTJPV_PTJPV4_MASK               16
#define PTJPV_PTJPV5_MASK               32
#define PTJPV_PTJPV6_MASK               64
#define PTJPV_PTJPV7_MASK               128


/*** ICSC1 - ICS Control Register 1; 0xFFFF8024 ***/
typedef union {
  byte Byte;
  struct {
    byte IREFSTEN    :1;                                       /* Internal Reference Stop Enable */
    byte IRCLKEN     :1;                                       /* Internal Reference Clock Enable */
    byte IREFS       :1;                                       /* Internal Reference Select */
    byte RDIV0       :1;                                       /* Reference Divider, bit 0 */
    byte RDIV1       :1;                                       /* Reference Divider, bit 1 */
    byte RDIV2       :1;                                       /* Reference Divider, bit 2 */
    byte CLKS0       :1;                                       /* Clock Source Select, bit 0 */
    byte CLKS1       :1;                                       /* Clock Source Select, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpRDIV :3;
    byte grpCLKS :2;
  } MergedBits;
} ICSC1STR;
extern volatile ICSC1STR _ICSC1 @0xFFFF8024;
#define ICSC1                           _ICSC1.Byte
#define ICSC1_IREFSTEN                  _ICSC1.Bits.IREFSTEN
#define ICSC1_IRCLKEN                   _ICSC1.Bits.IRCLKEN
#define ICSC1_IREFS                     _ICSC1.Bits.IREFS
#define ICSC1_RDIV0                     _ICSC1.Bits.RDIV0
#define ICSC1_RDIV1                     _ICSC1.Bits.RDIV1
#define ICSC1_RDIV2                     _ICSC1.Bits.RDIV2
#define ICSC1_CLKS0                     _ICSC1.Bits.CLKS0
#define ICSC1_CLKS1                     _ICSC1.Bits.CLKS1
#define ICSC1_RDIV                      _ICSC1.MergedBits.grpRDIV
#define ICSC1_CLKS                      _ICSC1.MergedBits.grpCLKS

#define ICSC1_IREFSTEN_MASK             1
#define ICSC1_IRCLKEN_MASK              2
#define ICSC1_IREFS_MASK                4
#define ICSC1_RDIV0_MASK                8
#define ICSC1_RDIV1_MASK                16
#define ICSC1_RDIV2_MASK                32
#define ICSC1_CLKS0_MASK                64
#define ICSC1_CLKS1_MASK                128
#define ICSC1_RDIV_MASK                 56
#define ICSC1_RDIV_BITNUM               3
#define ICSC1_CLKS_MASK                 192
#define ICSC1_CLKS_BITNUM               6


/*** ICSC2 - ICS Control Register 2; 0xFFFF8025 ***/
typedef union {
  byte Byte;
  struct {
    byte EREFSTEN    :1;                                       /* External Reference Stop Enable */
    byte ERCLKEN     :1;                                       /* External Reference Enable */
    byte EREFS       :1;                                       /* External Reference Select */
    byte LP          :1;                                       /* Low Power Select */
    byte HGO         :1;                                       /* High Gain Oscillator Select */
    byte RANGE       :1;                                       /* Frequency Range Select */
    byte BDIV0       :1;                                       /* Bus Frequency Divider, bit 0 */
    byte BDIV1       :1;                                       /* Bus Frequency Divider, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpBDIV :2;
  } MergedBits;
} ICSC2STR;
extern volatile ICSC2STR _ICSC2 @0xFFFF8025;
#define ICSC2                           _ICSC2.Byte
#define ICSC2_EREFSTEN                  _ICSC2.Bits.EREFSTEN
#define ICSC2_ERCLKEN                   _ICSC2.Bits.ERCLKEN
#define ICSC2_EREFS                     _ICSC2.Bits.EREFS
#define ICSC2_LP                        _ICSC2.Bits.LP
#define ICSC2_HGO                       _ICSC2.Bits.HGO
#define ICSC2_RANGE                     _ICSC2.Bits.RANGE
#define ICSC2_BDIV0                     _ICSC2.Bits.BDIV0
#define ICSC2_BDIV1                     _ICSC2.Bits.BDIV1
#define ICSC2_BDIV                      _ICSC2.MergedBits.grpBDIV

#define ICSC2_EREFSTEN_MASK             1
#define ICSC2_ERCLKEN_MASK              2
#define ICSC2_EREFS_MASK                4
#define ICSC2_LP_MASK                   8
#define ICSC2_HGO_MASK                  16
#define ICSC2_RANGE_MASK                32
#define ICSC2_BDIV0_MASK                64
#define ICSC2_BDIV1_MASK                128
#define ICSC2_BDIV_MASK                 192
#define ICSC2_BDIV_BITNUM               6


/*** ICSTRM - ICS Trim Register; 0xFFFF8026 ***/
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* ICS Trim Setting, bit 0 */
    byte TRIM1       :1;                                       /* ICS Trim Setting, bit 1 */
    byte TRIM2       :1;                                       /* ICS Trim Setting, bit 2 */
    byte TRIM3       :1;                                       /* ICS Trim Setting, bit 3 */
    byte TRIM4       :1;                                       /* ICS Trim Setting, bit 4 */
    byte TRIM5       :1;                                       /* ICS Trim Setting, bit 5 */
    byte TRIM6       :1;                                       /* ICS Trim Setting, bit 6 */
    byte TRIM7       :1;                                       /* ICS Trim Setting, bit 7 */
  } Bits;
} ICSTRMSTR;
extern volatile ICSTRMSTR _ICSTRM @0xFFFF8026;
#define ICSTRM                          _ICSTRM.Byte
#define ICSTRM_TRIM0                    _ICSTRM.Bits.TRIM0
#define ICSTRM_TRIM1                    _ICSTRM.Bits.TRIM1
#define ICSTRM_TRIM2                    _ICSTRM.Bits.TRIM2
#define ICSTRM_TRIM3                    _ICSTRM.Bits.TRIM3
#define ICSTRM_TRIM4                    _ICSTRM.Bits.TRIM4
#define ICSTRM_TRIM5                    _ICSTRM.Bits.TRIM5
#define ICSTRM_TRIM6                    _ICSTRM.Bits.TRIM6
#define ICSTRM_TRIM7                    _ICSTRM.Bits.TRIM7

#define ICSTRM_TRIM0_MASK               1
#define ICSTRM_TRIM1_MASK               2
#define ICSTRM_TRIM2_MASK               4
#define ICSTRM_TRIM3_MASK               8
#define ICSTRM_TRIM4_MASK               16
#define ICSTRM_TRIM5_MASK               32
#define ICSTRM_TRIM6_MASK               64
#define ICSTRM_TRIM7_MASK               128


/*** ICSSC - ICS Status and Control Register; 0xFFFF8027 ***/
typedef union {
  byte Byte;
  struct {
    byte FTRIM       :1;                                       /* ICS Fine Trim */
    byte OSCINIT     :1;                                       /* OSC Initialization */
    byte CLKST0      :1;                                       /* Clock Mode Status, bit 0 */
    byte CLKST1      :1;                                       /* Clock Mode Status, bit 1 */
    byte IREFST      :1;                                       /* Internal Reference Status */
    byte DMX32       :1;                                       /* DCO Maximum frequency with 32.768 kHz reference */
    byte DRST_DRS0   :1;                                       /* DCO Range Status/Range Select, bit 0 */
    byte DRST_DRS1   :1;                                       /* DCO Range Status/Range Select, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpCLKST :2;
    byte         :1;
    byte grpDMX_32 :1;
    byte grpDRST_DRS :2;
  } MergedBits;
} ICSSCSTR;
extern volatile ICSSCSTR _ICSSC @0xFFFF8027;
#define ICSSC                           _ICSSC.Byte
#define ICSSC_FTRIM                     _ICSSC.Bits.FTRIM
#define ICSSC_OSCINIT                   _ICSSC.Bits.OSCINIT
#define ICSSC_CLKST0                    _ICSSC.Bits.CLKST0
#define ICSSC_CLKST1                    _ICSSC.Bits.CLKST1
#define ICSSC_IREFST                    _ICSSC.Bits.IREFST
#define ICSSC_DMX32                     _ICSSC.Bits.DMX32
#define ICSSC_DRST_DRS0                 _ICSSC.Bits.DRST_DRS0
#define ICSSC_DRST_DRS1                 _ICSSC.Bits.DRST_DRS1
#define ICSSC_CLKST                     _ICSSC.MergedBits.grpCLKST
#define ICSSC_DRST_DRS                  _ICSSC.MergedBits.grpDRST_DRS

#define ICSSC_FTRIM_MASK                1
#define ICSSC_OSCINIT_MASK              2
#define ICSSC_CLKST0_MASK               4
#define ICSSC_CLKST1_MASK               8
#define ICSSC_IREFST_MASK               16
#define ICSSC_DMX32_MASK                32
#define ICSSC_DRST_DRS0_MASK            64
#define ICSSC_DRST_DRS1_MASK            128
#define ICSSC_CLKST_MASK                12
#define ICSSC_CLKST_BITNUM              2
#define ICSSC_DRST_DRS_MASK             192
#define ICSSC_DRST_DRS_BITNUM           6


/*** RTCSC - RTC Status and Control Register; 0xFFFF8028 ***/
typedef union {
  byte Byte;
  struct {
    byte RTCPS0      :1;                                       /* Real-Time Clock Prescaler Select, bit 0 */
    byte RTCPS1      :1;                                       /* Real-Time Clock Prescaler Select, bit 1 */
    byte RTCPS2      :1;                                       /* Real-Time Clock Prescaler Select, bit 2 */
    byte RTCPS3      :1;                                       /* Real-Time Clock Prescaler Select, bit 3 */
    byte RTIE        :1;                                       /* Real-Time Interrupt Enable */
    byte RTCLKS0     :1;                                       /* Real-Time Clock Source Select, bit 0 */
    byte RTCLKS1     :1;                                       /* Real-Time Clock Source Select, bit 1 */
    byte RTIF        :1;                                       /* Real-Time Interrupt Flag */
  } Bits;
  struct {
    byte grpRTCPS :4;
    byte         :1;
    byte grpRTCLKS :2;
    byte         :1;
  } MergedBits;
} RTCSCSTR;
extern volatile RTCSCSTR _RTCSC @0xFFFF8028;
#define RTCSC                           _RTCSC.Byte
#define RTCSC_RTCPS0                    _RTCSC.Bits.RTCPS0
#define RTCSC_RTCPS1                    _RTCSC.Bits.RTCPS1
#define RTCSC_RTCPS2                    _RTCSC.Bits.RTCPS2
#define RTCSC_RTCPS3                    _RTCSC.Bits.RTCPS3
#define RTCSC_RTIE                      _RTCSC.Bits.RTIE
#define RTCSC_RTCLKS0                   _RTCSC.Bits.RTCLKS0
#define RTCSC_RTCLKS1                   _RTCSC.Bits.RTCLKS1
#define RTCSC_RTIF                      _RTCSC.Bits.RTIF
#define RTCSC_RTCPS                     _RTCSC.MergedBits.grpRTCPS
#define RTCSC_RTCLKS                    _RTCSC.MergedBits.grpRTCLKS

#define RTCSC_RTCPS0_MASK               1
#define RTCSC_RTCPS1_MASK               2
#define RTCSC_RTCPS2_MASK               4
#define RTCSC_RTCPS3_MASK               8
#define RTCSC_RTIE_MASK                 16
#define RTCSC_RTCLKS0_MASK              32
#define RTCSC_RTCLKS1_MASK              64
#define RTCSC_RTIF_MASK                 128
#define RTCSC_RTCPS_MASK                15
#define RTCSC_RTCPS_BITNUM              0
#define RTCSC_RTCLKS_MASK               96
#define RTCSC_RTCLKS_BITNUM             5


/*** RTCCNT - RTC Counter Register; 0xFFFF8029 ***/
typedef union {
  byte Byte;
} RTCCNTSTR;
extern volatile RTCCNTSTR _RTCCNT @0xFFFF8029;
#define RTCCNT                          _RTCCNT.Byte


/*** RTCMOD - RTC Modulo Register; 0xFFFF802A ***/
typedef union {
  byte Byte;
} RTCMODSTR;
extern volatile RTCMODSTR _RTCMOD @0xFFFF802A;
#define RTCMOD                          _RTCMOD.Byte


/*** RTCSC1 - RTC DMA Enable Register; 0xFFFF802B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte DMAEN       :1;                                       /* RTC DMA Enable */
  } Bits;
} RTCSC1STR;
extern volatile RTCSC1STR _RTCSC1 @0xFFFF802B;
#define RTCSC1                          _RTCSC1.Byte
#define RTCSC1_DMAEN                    _RTCSC1.Bits.DMAEN

#define RTCSC1_DMAEN_MASK               128


/*** DAC1CTRL - DAC Control Register; 0xFFFF802D ***/
typedef union {
  byte Byte;
  struct {
    byte VOSEL0      :1;                                       /* DAC output voltage select, bit 0 */
    byte VOSEL1      :1;                                       /* DAC output voltage select, bit 1 */
    byte VOSEL2      :1;                                       /* DAC output voltage select, bit 2 */
    byte VOSEL3      :1;                                       /* DAC output voltage select, bit 3 */
    byte VOSEL4      :1;                                       /* DAC output voltage select, bit 4 */
    byte             :1; 
    byte VRSEL       :1;                                       /* Supply voltage reference source select. This bit selects the supply reference source */
    byte DACEN       :1;                                       /* DAC enable. When the module is disabled, the DAC is powered down to conserve power */
  } Bits;
  struct {
    byte grpVOSEL :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DAC1CTRLSTR;
extern volatile DAC1CTRLSTR _DAC1CTRL @0xFFFF802D;
#define DAC1CTRL                        _DAC1CTRL.Byte
#define DAC1CTRL_VOSEL0                 _DAC1CTRL.Bits.VOSEL0
#define DAC1CTRL_VOSEL1                 _DAC1CTRL.Bits.VOSEL1
#define DAC1CTRL_VOSEL2                 _DAC1CTRL.Bits.VOSEL2
#define DAC1CTRL_VOSEL3                 _DAC1CTRL.Bits.VOSEL3
#define DAC1CTRL_VOSEL4                 _DAC1CTRL.Bits.VOSEL4
#define DAC1CTRL_VRSEL                  _DAC1CTRL.Bits.VRSEL
#define DAC1CTRL_DACEN                  _DAC1CTRL.Bits.DACEN
#define DAC1CTRL_VOSEL                  _DAC1CTRL.MergedBits.grpVOSEL

#define DAC1CTRL_VOSEL0_MASK            1
#define DAC1CTRL_VOSEL1_MASK            2
#define DAC1CTRL_VOSEL2_MASK            4
#define DAC1CTRL_VOSEL3_MASK            8
#define DAC1CTRL_VOSEL4_MASK            16
#define DAC1CTRL_VRSEL_MASK             64
#define DAC1CTRL_DACEN_MASK             128
#define DAC1CTRL_VOSEL_MASK             31
#define DAC1CTRL_VOSEL_BITNUM           0


/*** DAC2CTRL - DAC Control Register; 0xFFFF802E ***/
typedef union {
  byte Byte;
  struct {
    byte VOSEL0      :1;                                       /* DAC output voltage select, bit 0 */
    byte VOSEL1      :1;                                       /* DAC output voltage select, bit 1 */
    byte VOSEL2      :1;                                       /* DAC output voltage select, bit 2 */
    byte VOSEL3      :1;                                       /* DAC output voltage select, bit 3 */
    byte VOSEL4      :1;                                       /* DAC output voltage select, bit 4 */
    byte             :1; 
    byte VRSEL       :1;                                       /* Supply voltage reference source select. This bit selects the supply reference source */
    byte DACEN       :1;                                       /* DAC enable. When the module is disabled, the DAC is powered down to conserve power */
  } Bits;
  struct {
    byte grpVOSEL :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DAC2CTRLSTR;
extern volatile DAC2CTRLSTR _DAC2CTRL @0xFFFF802E;
#define DAC2CTRL                        _DAC2CTRL.Byte
#define DAC2CTRL_VOSEL0                 _DAC2CTRL.Bits.VOSEL0
#define DAC2CTRL_VOSEL1                 _DAC2CTRL.Bits.VOSEL1
#define DAC2CTRL_VOSEL2                 _DAC2CTRL.Bits.VOSEL2
#define DAC2CTRL_VOSEL3                 _DAC2CTRL.Bits.VOSEL3
#define DAC2CTRL_VOSEL4                 _DAC2CTRL.Bits.VOSEL4
#define DAC2CTRL_VRSEL                  _DAC2CTRL.Bits.VRSEL
#define DAC2CTRL_DACEN                  _DAC2CTRL.Bits.DACEN
#define DAC2CTRL_VOSEL                  _DAC2CTRL.MergedBits.grpVOSEL

#define DAC2CTRL_VOSEL0_MASK            1
#define DAC2CTRL_VOSEL1_MASK            2
#define DAC2CTRL_VOSEL2_MASK            4
#define DAC2CTRL_VOSEL3_MASK            8
#define DAC2CTRL_VOSEL4_MASK            16
#define DAC2CTRL_VRSEL_MASK             64
#define DAC2CTRL_DACEN_MASK             128
#define DAC2CTRL_VOSEL_MASK             31
#define DAC2CTRL_VOSEL_BITNUM           0


/*** ADCSC1A - ADC Status and control register 1A; 0xFFFF8030 ***/
typedef union {
  word Word;
  struct {
    word ADCH0       :1;                                       /* Input Channel Select, bit 0 */
    word ADCH1       :1;                                       /* Input Channel Select, bit 1 */
    word ADCH2       :1;                                       /* Input Channel Select, bit 2 */
    word ADCH3       :1;                                       /* Input Channel Select, bit 3 */
    word ADCH4       :1;                                       /* Input Channel Select, bit 4 */
    word ADCO        :1;                                       /* Continuous Conversion Enable */
    word AIEN        :1;                                       /* Interrupt Enable */
    word COCO        :1;                                       /* Conversion Complete Flag */
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
  } Bits;
  struct {
    word grpADCH :5;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} ADCSC1ASTR;
extern volatile ADCSC1ASTR _ADCSC1A @0xFFFF8030;
#define ADCSC1A                         _ADCSC1A.Word
#define ADCSC1A_ADCH0                   _ADCSC1A.Bits.ADCH0
#define ADCSC1A_ADCH1                   _ADCSC1A.Bits.ADCH1
#define ADCSC1A_ADCH2                   _ADCSC1A.Bits.ADCH2
#define ADCSC1A_ADCH3                   _ADCSC1A.Bits.ADCH3
#define ADCSC1A_ADCH4                   _ADCSC1A.Bits.ADCH4
#define ADCSC1A_ADCO                    _ADCSC1A.Bits.ADCO
#define ADCSC1A_AIEN                    _ADCSC1A.Bits.AIEN
#define ADCSC1A_COCO                    _ADCSC1A.Bits.COCO
#define ADCSC1A_ADCH                    _ADCSC1A.MergedBits.grpADCH

#define ADCSC1A_ADCH0_MASK              1
#define ADCSC1A_ADCH1_MASK              2
#define ADCSC1A_ADCH2_MASK              4
#define ADCSC1A_ADCH3_MASK              8
#define ADCSC1A_ADCH4_MASK              16
#define ADCSC1A_ADCO_MASK               32
#define ADCSC1A_AIEN_MASK               64
#define ADCSC1A_COCO_MASK               128
#define ADCSC1A_ADCH_MASK               31
#define ADCSC1A_ADCH_BITNUM             0


/*** ADCSC2 - ADC Status and control register 2; 0xFFFF8032 ***/
typedef union {
  word Word;
  struct {
    word REFSEL0     :1;                                       /* REFSEL, bit 0 */
    word REFSEL1     :1;                                       /* REFSEL, bit 1 */
    word             :1; 
    word DMAEN       :1;                                       /* DMA Enable */
    word BB          :1;                                       /* Back-to-back Conversion Enable */
    word             :1; 
    word ADTRG       :1;                                       /* Conversion Trigger Select */
    word ADACT       :1;                                       /* Conversion Active */
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
  } Bits;
  struct {
    word grpREFSEL :2;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} ADCSC2STR;
extern volatile ADCSC2STR _ADCSC2 @0xFFFF8032;
#define ADCSC2                          _ADCSC2.Word
#define ADCSC2_REFSEL0                  _ADCSC2.Bits.REFSEL0
#define ADCSC2_REFSEL1                  _ADCSC2.Bits.REFSEL1
#define ADCSC2_DMAEN                    _ADCSC2.Bits.DMAEN
#define ADCSC2_BB                       _ADCSC2.Bits.BB
#define ADCSC2_ADTRG                    _ADCSC2.Bits.ADTRG
#define ADCSC2_ADACT                    _ADCSC2.Bits.ADACT
#define ADCSC2_REFSEL                   _ADCSC2.MergedBits.grpREFSEL

#define ADCSC2_REFSEL0_MASK             1
#define ADCSC2_REFSEL1_MASK             2
#define ADCSC2_DMAEN_MASK               8
#define ADCSC2_BB_MASK                  16
#define ADCSC2_ADTRG_MASK               64
#define ADCSC2_ADACT_MASK               128
#define ADCSC2_REFSEL_MASK              3
#define ADCSC2_REFSEL_BITNUM            0


/*** ADCCFG - ADC Configuration register; 0xFFFF803C ***/
typedef union {
  word Word;
  struct {
    word ADICLK0     :1;                                       /* Input Clock Select, bit 0 */
    word ADICLK1     :1;                                       /* Input Clock Select, bit 1 */
    word MODE0       :1;                                       /* Conversion Mode Selection, bit 0 */
    word MODE1       :1;                                       /* Conversion Mode Selection, bit 1 */
    word ADLSMP      :1;                                       /* Long Sample Time Configuration */
    word ADIV0       :1;                                       /* Clock Divide Select, bit 0 */
    word ADIV1       :1;                                       /* Clock Divide Select, bit 1 */
    word ADLPC       :1;                                       /* Low Power Configuration */
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
  } Bits;
  struct {
    word grpADICLK :2;
    word grpMODE :2;
    word         :1;
    word grpADIV :2;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} ADCCFGSTR;
extern volatile ADCCFGSTR _ADCCFG @0xFFFF803C;
#define ADCCFG                          _ADCCFG.Word
#define ADCCFG_ADICLK0                  _ADCCFG.Bits.ADICLK0
#define ADCCFG_ADICLK1                  _ADCCFG.Bits.ADICLK1
#define ADCCFG_MODE0                    _ADCCFG.Bits.MODE0
#define ADCCFG_MODE1                    _ADCCFG.Bits.MODE1
#define ADCCFG_ADLSMP                   _ADCCFG.Bits.ADLSMP
#define ADCCFG_ADIV0                    _ADCCFG.Bits.ADIV0
#define ADCCFG_ADIV1                    _ADCCFG.Bits.ADIV1
#define ADCCFG_ADLPC                    _ADCCFG.Bits.ADLPC
#define ADCCFG_ADICLK                   _ADCCFG.MergedBits.grpADICLK
#define ADCCFG_MODE                     _ADCCFG.MergedBits.grpMODE
#define ADCCFG_ADIV                     _ADCCFG.MergedBits.grpADIV

#define ADCCFG_ADICLK0_MASK             1
#define ADCCFG_ADICLK1_MASK             2
#define ADCCFG_MODE0_MASK               4
#define ADCCFG_MODE1_MASK               8
#define ADCCFG_ADLSMP_MASK              16
#define ADCCFG_ADIV0_MASK               32
#define ADCCFG_ADIV1_MASK               64
#define ADCCFG_ADLPC_MASK               128
#define ADCCFG_ADICLK_MASK              3
#define ADCCFG_ADICLK_BITNUM            0
#define ADCCFG_MODE_MASK                12
#define ADCCFG_MODE_BITNUM              2
#define ADCCFG_ADIV_MASK                96
#define ADCCFG_ADIV_BITNUM              5


/*** ADCSC1B - ADC Status and control register 1B; 0xFFFF8044 ***/
typedef union {
  word Word;
  struct {
    word ADCH0       :1;                                       /* Input Channel Select, bit 0 */
    word ADCH1       :1;                                       /* Input Channel Select, bit 1 */
    word ADCH2       :1;                                       /* Input Channel Select, bit 2 */
    word ADCH3       :1;                                       /* Input Channel Select, bit 3 */
    word ADCH4       :1;                                       /* Input Channel Select, bit 4 */
    word ADCO        :1;                                       /* Continuous Conversion Enable */
    word AIEN        :1;                                       /* Interrupt Enable */
    word COCO        :1;                                       /* Conversion Complete Flag */
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
  } Bits;
  struct {
    word grpADCH :5;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} ADCSC1BSTR;
extern volatile ADCSC1BSTR _ADCSC1B @0xFFFF8044;
#define ADCSC1B                         _ADCSC1B.Word
#define ADCSC1B_ADCH0                   _ADCSC1B.Bits.ADCH0
#define ADCSC1B_ADCH1                   _ADCSC1B.Bits.ADCH1
#define ADCSC1B_ADCH2                   _ADCSC1B.Bits.ADCH2
#define ADCSC1B_ADCH3                   _ADCSC1B.Bits.ADCH3
#define ADCSC1B_ADCH4                   _ADCSC1B.Bits.ADCH4
#define ADCSC1B_ADCO                    _ADCSC1B.Bits.ADCO
#define ADCSC1B_AIEN                    _ADCSC1B.Bits.AIEN
#define ADCSC1B_COCO                    _ADCSC1B.Bits.COCO
#define ADCSC1B_ADCH                    _ADCSC1B.MergedBits.grpADCH

#define ADCSC1B_ADCH0_MASK              1
#define ADCSC1B_ADCH1_MASK              2
#define ADCSC1B_ADCH2_MASK              4
#define ADCSC1B_ADCH3_MASK              8
#define ADCSC1B_ADCH4_MASK              16
#define ADCSC1B_ADCO_MASK               32
#define ADCSC1B_AIEN_MASK               64
#define ADCSC1B_COCO_MASK               128
#define ADCSC1B_ADCH_MASK               31
#define ADCSC1B_ADCH_BITNUM             0


/*** ADCRA - ADC Data Result Registers A; 0xFFFF8046 ***/
typedef union {
  word Word;
  struct {
    word             :1; 
    word             :1; 
    word             :1; 
    word ADR0        :1;                                       /* Digital Result of the Conversion, bit 0 */
    word ADR1        :1;                                       /* Digital Result of the Conversion, bit 1 */
    word ADR2        :1;                                       /* Digital Result of the Conversion, bit 2 */
    word ADR3        :1;                                       /* Digital Result of the Conversion, bit 3 */
    word ADR4        :1;                                       /* Digital Result of the Conversion, bit 4 */
    word ADR5        :1;                                       /* Digital Result of the Conversion, bit 5 */
    word ADR6        :1;                                       /* Digital Result of the Conversion, bit 6 */
    word ADR7        :1;                                       /* Digital Result of the Conversion, bit 7 */
    word ADR8        :1;                                       /* Digital Result of the Conversion, bit 8 */
    word ADR9        :1;                                       /* Digital Result of the Conversion, bit 9 */
    word ADR10       :1;                                       /* Digital Result of the Conversion, bit 10 */
    word ADR11       :1;                                       /* Digital Result of the Conversion, bit 11 */
    word             :1; 
  } Bits;
  struct {
    word         :1;
    word         :1;
    word         :1;
    word grpADR  :12;
    word         :1;
  } MergedBits;
} ADCRASTR;
extern volatile ADCRASTR _ADCRA @0xFFFF8046;
#define ADCRA                           _ADCRA.Word
#define ADCRA_ADR0                      _ADCRA.Bits.ADR0
#define ADCRA_ADR1                      _ADCRA.Bits.ADR1
#define ADCRA_ADR2                      _ADCRA.Bits.ADR2
#define ADCRA_ADR3                      _ADCRA.Bits.ADR3
#define ADCRA_ADR4                      _ADCRA.Bits.ADR4
#define ADCRA_ADR5                      _ADCRA.Bits.ADR5
#define ADCRA_ADR6                      _ADCRA.Bits.ADR6
#define ADCRA_ADR7                      _ADCRA.Bits.ADR7
#define ADCRA_ADR8                      _ADCRA.Bits.ADR8
#define ADCRA_ADR9                      _ADCRA.Bits.ADR9
#define ADCRA_ADR10                     _ADCRA.Bits.ADR10
#define ADCRA_ADR11                     _ADCRA.Bits.ADR11
#define ADCRA_ADR                       _ADCRA.MergedBits.grpADR

#define ADCRA_ADR0_MASK                 8
#define ADCRA_ADR1_MASK                 16
#define ADCRA_ADR2_MASK                 32
#define ADCRA_ADR3_MASK                 64
#define ADCRA_ADR4_MASK                 128
#define ADCRA_ADR5_MASK                 256
#define ADCRA_ADR6_MASK                 512
#define ADCRA_ADR7_MASK                 1024
#define ADCRA_ADR8_MASK                 2048
#define ADCRA_ADR9_MASK                 4096
#define ADCRA_ADR10_MASK                8192
#define ADCRA_ADR11_MASK                16384
#define ADCRA_ADR_MASK                  32760
#define ADCRA_ADR_BITNUM                3


/*** ADCRB - ADC Data Result Registers B; 0xFFFF8048 ***/
typedef union {
  word Word;
  struct {
    word             :1; 
    word             :1; 
    word             :1; 
    word ADR0        :1;                                       /* Digital Result of the Conversion, bit 0 */
    word ADR1        :1;                                       /* Digital Result of the Conversion, bit 1 */
    word ADR2        :1;                                       /* Digital Result of the Conversion, bit 2 */
    word ADR3        :1;                                       /* Digital Result of the Conversion, bit 3 */
    word ADR4        :1;                                       /* Digital Result of the Conversion, bit 4 */
    word ADR5        :1;                                       /* Digital Result of the Conversion, bit 5 */
    word ADR6        :1;                                       /* Digital Result of the Conversion, bit 6 */
    word ADR7        :1;                                       /* Digital Result of the Conversion, bit 7 */
    word ADR8        :1;                                       /* Digital Result of the Conversion, bit 8 */
    word ADR9        :1;                                       /* Digital Result of the Conversion, bit 9 */
    word ADR10       :1;                                       /* Digital Result of the Conversion, bit 10 */
    word ADR11       :1;                                       /* Digital Result of the Conversion, bit 11 */
    word             :1; 
  } Bits;
  struct {
    word         :1;
    word         :1;
    word         :1;
    word grpADR  :12;
    word         :1;
  } MergedBits;
} ADCRBSTR;
extern volatile ADCRBSTR _ADCRB @0xFFFF8048;
#define ADCRB                           _ADCRB.Word
#define ADCRB_ADR0                      _ADCRB.Bits.ADR0
#define ADCRB_ADR1                      _ADCRB.Bits.ADR1
#define ADCRB_ADR2                      _ADCRB.Bits.ADR2
#define ADCRB_ADR3                      _ADCRB.Bits.ADR3
#define ADCRB_ADR4                      _ADCRB.Bits.ADR4
#define ADCRB_ADR5                      _ADCRB.Bits.ADR5
#define ADCRB_ADR6                      _ADCRB.Bits.ADR6
#define ADCRB_ADR7                      _ADCRB.Bits.ADR7
#define ADCRB_ADR8                      _ADCRB.Bits.ADR8
#define ADCRB_ADR9                      _ADCRB.Bits.ADR9
#define ADCRB_ADR10                     _ADCRB.Bits.ADR10
#define ADCRB_ADR11                     _ADCRB.Bits.ADR11
#define ADCRB_ADR                       _ADCRB.MergedBits.grpADR

#define ADCRB_ADR0_MASK                 8
#define ADCRB_ADR1_MASK                 16
#define ADCRB_ADR2_MASK                 32
#define ADCRB_ADR3_MASK                 64
#define ADCRB_ADR4_MASK                 128
#define ADCRB_ADR5_MASK                 256
#define ADCRB_ADR6_MASK                 512
#define ADCRB_ADR7_MASK                 1024
#define ADCRB_ADR8_MASK                 2048
#define ADCRB_ADR9_MASK                 4096
#define ADCRB_ADR10_MASK                8192
#define ADCRB_ADR11_MASK                16384
#define ADCRB_ADR_MASK                  32760
#define ADCRB_ADR_BITNUM                3


/*** SCI1BD - SCI1 Baud Rate Register; 0xFFFF8050 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI1BDH - SCI1 Baud Rate Register High; 0xFFFF8050 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
        byte LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
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
    #define SCI1BDH_RXEDGIE             _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.RXEDGIE
    #define SCI1BDH_LBKDIE              _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.LBKDIE
    #define SCI1BDH_SBR_8               _SCI1BD.Overlap_STR.SCI1BDHSTR.MergedBits.grpSBR_8
    #define SCI1BDH_SBR                 SCI1BDH_SBR_8
    
    #define SCI1BDH_SBR8_MASK           1
    #define SCI1BDH_SBR9_MASK           2
    #define SCI1BDH_SBR10_MASK          4
    #define SCI1BDH_SBR11_MASK          8
    #define SCI1BDH_SBR12_MASK          16
    #define SCI1BDH_RXEDGIE_MASK        64
    #define SCI1BDH_LBKDIE_MASK         128
    #define SCI1BDH_SBR_8_MASK          31
    #define SCI1BDH_SBR_8_BITNUM        0
    

    /*** SCI1BDL - SCI1 Baud Rate Register Low; 0xFFFF8051 ***/
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

  struct {
    word SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
    word SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
    word SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
    word SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
    word SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
    word SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
    word SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
    word SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
    word SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
    word SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
    word SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
    word SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
    word SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
    word             :1; 
    word RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
    word LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
  } Bits;
  struct {
    word grpSBR  :13;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} SCI1BDSTR;
extern volatile SCI1BDSTR _SCI1BD @0xFFFF8050;
#define SCI1BD                          _SCI1BD.Word
#define SCI1BD_SBR0                     _SCI1BD.Bits.SBR0
#define SCI1BD_SBR1                     _SCI1BD.Bits.SBR1
#define SCI1BD_SBR2                     _SCI1BD.Bits.SBR2
#define SCI1BD_SBR3                     _SCI1BD.Bits.SBR3
#define SCI1BD_SBR4                     _SCI1BD.Bits.SBR4
#define SCI1BD_SBR5                     _SCI1BD.Bits.SBR5
#define SCI1BD_SBR6                     _SCI1BD.Bits.SBR6
#define SCI1BD_SBR7                     _SCI1BD.Bits.SBR7
#define SCI1BD_SBR8                     _SCI1BD.Bits.SBR8
#define SCI1BD_SBR9                     _SCI1BD.Bits.SBR9
#define SCI1BD_SBR10                    _SCI1BD.Bits.SBR10
#define SCI1BD_SBR11                    _SCI1BD.Bits.SBR11
#define SCI1BD_SBR12                    _SCI1BD.Bits.SBR12
#define SCI1BD_RXEDGIE                  _SCI1BD.Bits.RXEDGIE
#define SCI1BD_LBKDIE                   _SCI1BD.Bits.LBKDIE
#define SCI1BD_SBR                      _SCI1BD.MergedBits.grpSBR

#define SCI1BD_SBR0_MASK                1
#define SCI1BD_SBR1_MASK                2
#define SCI1BD_SBR2_MASK                4
#define SCI1BD_SBR3_MASK                8
#define SCI1BD_SBR4_MASK                16
#define SCI1BD_SBR5_MASK                32
#define SCI1BD_SBR6_MASK                64
#define SCI1BD_SBR7_MASK                128
#define SCI1BD_SBR8_MASK                256
#define SCI1BD_SBR9_MASK                512
#define SCI1BD_SBR10_MASK               1024
#define SCI1BD_SBR11_MASK               2048
#define SCI1BD_SBR12_MASK               4096
#define SCI1BD_RXEDGIE_MASK             16384
#define SCI1BD_LBKDIE_MASK              32768
#define SCI1BD_SBR_MASK                 8191
#define SCI1BD_SBR_BITNUM               0


/*** SCI1C1 - SCI1 Control Register 1; 0xFFFF8052 ***/
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
extern volatile SCI1C1STR _SCI1C1 @0xFFFF8052;
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


/*** SCI1C2 - SCI1 Control Register 2; 0xFFFF8053 ***/
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
extern volatile SCI1C2STR _SCI1C2 @0xFFFF8053;
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


/*** SCI1S1 - SCI1 Status Register 1; 0xFFFF8054 ***/
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
extern volatile SCI1S1STR _SCI1S1 @0xFFFF8054;
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


/*** SCI1S2 - SCI1 Status Register 2; 0xFFFF8055 ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte LBKDE       :1;                                       /* LIN Break Detection Enable */
    byte BRK13       :1;                                       /* Break Character Generation Length */
    byte RWUID       :1;                                       /* Receive Wake Up Idle Detect */
    byte RXINV       :1;                                       /* Receive Data Inversion */
    byte             :1; 
    byte RXEDGIF     :1;                                       /* RxD Pin Active Edge Interrupt Flag */
    byte LBKDIF      :1;                                       /* LIN Break Detect Interrupt Flag */
  } Bits;
} SCI1S2STR;
extern volatile SCI1S2STR _SCI1S2 @0xFFFF8055;
#define SCI1S2                          _SCI1S2.Byte
#define SCI1S2_RAF                      _SCI1S2.Bits.RAF
#define SCI1S2_LBKDE                    _SCI1S2.Bits.LBKDE
#define SCI1S2_BRK13                    _SCI1S2.Bits.BRK13
#define SCI1S2_RWUID                    _SCI1S2.Bits.RWUID
#define SCI1S2_RXINV                    _SCI1S2.Bits.RXINV
#define SCI1S2_RXEDGIF                  _SCI1S2.Bits.RXEDGIF
#define SCI1S2_LBKDIF                   _SCI1S2.Bits.LBKDIF

#define SCI1S2_RAF_MASK                 1
#define SCI1S2_LBKDE_MASK               2
#define SCI1S2_BRK13_MASK               4
#define SCI1S2_RWUID_MASK               8
#define SCI1S2_RXINV_MASK               16
#define SCI1S2_RXEDGIF_MASK             64
#define SCI1S2_LBKDIF_MASK              128


/*** SCI1C3 - SCI1 Control Register 3; 0xFFFF8056 ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte TXINV       :1;                                       /* Transmit Data Inversion */
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI1C3STR;
extern volatile SCI1C3STR _SCI1C3 @0xFFFF8056;
#define SCI1C3                          _SCI1C3.Byte
#define SCI1C3_PEIE                     _SCI1C3.Bits.PEIE
#define SCI1C3_FEIE                     _SCI1C3.Bits.FEIE
#define SCI1C3_NEIE                     _SCI1C3.Bits.NEIE
#define SCI1C3_ORIE                     _SCI1C3.Bits.ORIE
#define SCI1C3_TXINV                    _SCI1C3.Bits.TXINV
#define SCI1C3_TXDIR                    _SCI1C3.Bits.TXDIR
#define SCI1C3_T8                       _SCI1C3.Bits.T8
#define SCI1C3_R8                       _SCI1C3.Bits.R8

#define SCI1C3_PEIE_MASK                1
#define SCI1C3_FEIE_MASK                2
#define SCI1C3_NEIE_MASK                4
#define SCI1C3_ORIE_MASK                8
#define SCI1C3_TXINV_MASK               16
#define SCI1C3_TXDIR_MASK               32
#define SCI1C3_T8_MASK                  64
#define SCI1C3_R8_MASK                  128


/*** SCI1D - SCI1 Data Register; 0xFFFF8057 ***/
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
extern volatile SCI1DSTR _SCI1D @0xFFFF8057;
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


/*** SCI1MA1 - SCI1 Match Address Registers 1; 0xFFFF8058 ***/
typedef union {
  byte Byte;
  struct {
    byte MA0         :1;                                       /* Match address value, bit 0 */
    byte MA1         :1;                                       /* Match address value, bit 1 */
    byte MA2         :1;                                       /* Match address value, bit 2 */
    byte MA3         :1;                                       /* Match address value, bit 3 */
    byte MA4         :1;                                       /* Match address value, bit 4 */
    byte MA5         :1;                                       /* Match address value, bit 5 */
    byte MA6         :1;                                       /* Match address value, bit 6 */
    byte MA7         :1;                                       /* Match address value, bit 7 */
  } Bits;
} SCI1MA1STR;
extern volatile SCI1MA1STR _SCI1MA1 @0xFFFF8058;
#define SCI1MA1                         _SCI1MA1.Byte
#define SCI1MA1_MA0                     _SCI1MA1.Bits.MA0
#define SCI1MA1_MA1                     _SCI1MA1.Bits.MA1
#define SCI1MA1_MA2                     _SCI1MA1.Bits.MA2
#define SCI1MA1_MA3                     _SCI1MA1.Bits.MA3
#define SCI1MA1_MA4                     _SCI1MA1.Bits.MA4
#define SCI1MA1_MA5                     _SCI1MA1.Bits.MA5
#define SCI1MA1_MA6                     _SCI1MA1.Bits.MA6
#define SCI1MA1_MA7                     _SCI1MA1.Bits.MA7

#define SCI1MA1_MA0_MASK                1
#define SCI1MA1_MA1_MASK                2
#define SCI1MA1_MA2_MASK                4
#define SCI1MA1_MA3_MASK                8
#define SCI1MA1_MA4_MASK                16
#define SCI1MA1_MA5_MASK                32
#define SCI1MA1_MA6_MASK                64
#define SCI1MA1_MA7_MASK                128


/*** SCI1MA2 - SCI1 Match Address Registers 2; 0xFFFF8059 ***/
typedef union {
  byte Byte;
  struct {
    byte MA0         :1;                                       /* Match address value, bit 0 */
    byte MA1         :1;                                       /* Match address value, bit 1 */
    byte MA2         :1;                                       /* Match address value, bit 2 */
    byte MA3         :1;                                       /* Match address value, bit 3 */
    byte MA4         :1;                                       /* Match address value, bit 4 */
    byte MA5         :1;                                       /* Match address value, bit 5 */
    byte MA6         :1;                                       /* Match address value, bit 6 */
    byte MA7         :1;                                       /* Match address value, bit 7 */
  } Bits;
} SCI1MA2STR;
extern volatile SCI1MA2STR _SCI1MA2 @0xFFFF8059;
#define SCI1MA2                         _SCI1MA2.Byte
#define SCI1MA2_MA0                     _SCI1MA2.Bits.MA0
#define SCI1MA2_MA1                     _SCI1MA2.Bits.MA1
#define SCI1MA2_MA2                     _SCI1MA2.Bits.MA2
#define SCI1MA2_MA3                     _SCI1MA2.Bits.MA3
#define SCI1MA2_MA4                     _SCI1MA2.Bits.MA4
#define SCI1MA2_MA5                     _SCI1MA2.Bits.MA5
#define SCI1MA2_MA6                     _SCI1MA2.Bits.MA6
#define SCI1MA2_MA7                     _SCI1MA2.Bits.MA7

#define SCI1MA2_MA0_MASK                1
#define SCI1MA2_MA1_MASK                2
#define SCI1MA2_MA2_MASK                4
#define SCI1MA2_MA3_MASK                8
#define SCI1MA2_MA4_MASK                16
#define SCI1MA2_MA5_MASK                32
#define SCI1MA2_MA6_MASK                64
#define SCI1MA2_MA7_MASK                128


/*** SCI1C4 - SCI1 Control Register 4; 0xFFFF805A ***/
typedef union {
  byte Byte;
  struct {
    byte BRFA0       :1;                                       /* Baud rate fine adjust, bit 0 */
    byte BRFA1       :1;                                       /* Baud rate fine adjust, bit 1 */
    byte BRFA2       :1;                                       /* Baud rate fine adjust, bit 2 */
    byte BRFA3       :1;                                       /* Baud rate fine adjust, bit 3 */
    byte BRFA4       :1;                                       /* Baud rate fine adjust, bit 4 */
    byte             :1; 
    byte MAEN2       :1;                                       /* Match Address Mode Enable 2 */
    byte MAEN1       :1;                                       /* Match Address Mode Enable 1 */
  } Bits;
  struct {
    byte grpBRFA :5;
    byte         :1;
    byte grpMAEN_2 :1;
    byte grpMAEN_1 :1;
  } MergedBits;
} SCI1C4STR;
extern volatile SCI1C4STR _SCI1C4 @0xFFFF805A;
#define SCI1C4                          _SCI1C4.Byte
#define SCI1C4_BRFA0                    _SCI1C4.Bits.BRFA0
#define SCI1C4_BRFA1                    _SCI1C4.Bits.BRFA1
#define SCI1C4_BRFA2                    _SCI1C4.Bits.BRFA2
#define SCI1C4_BRFA3                    _SCI1C4.Bits.BRFA3
#define SCI1C4_BRFA4                    _SCI1C4.Bits.BRFA4
#define SCI1C4_MAEN2                    _SCI1C4.Bits.MAEN2
#define SCI1C4_MAEN1                    _SCI1C4.Bits.MAEN1
#define SCI1C4_BRFA                     _SCI1C4.MergedBits.grpBRFA

#define SCI1C4_BRFA0_MASK               1
#define SCI1C4_BRFA1_MASK               2
#define SCI1C4_BRFA2_MASK               4
#define SCI1C4_BRFA3_MASK               8
#define SCI1C4_BRFA4_MASK               16
#define SCI1C4_MAEN2_MASK               64
#define SCI1C4_MAEN1_MASK               128
#define SCI1C4_BRFA_MASK                31
#define SCI1C4_BRFA_BITNUM              0


/*** SCI1C5 - SCI1 Control Register 5; 0xFFFF805B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RDMAS       :1;                                       /* Receiver Full DMA Select */
    byte             :1; 
    byte TDMAS       :1;                                       /* Transmitter DMA Select */
  } Bits;
} SCI1C5STR;
extern volatile SCI1C5STR _SCI1C5 @0xFFFF805B;
#define SCI1C5                          _SCI1C5.Byte
#define SCI1C5_RDMAS                    _SCI1C5.Bits.RDMAS
#define SCI1C5_TDMAS                    _SCI1C5.Bits.TDMAS

#define SCI1C5_RDMAS_MASK               32
#define SCI1C5_TDMAS_MASK               128


/*** SCI2BD - SCI2 Baud Rate Register; 0xFFFF8060 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI2BDH - SCI2 Baud Rate Register High; 0xFFFF8060 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
        byte LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
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
    #define SCI2BDH_RXEDGIE             _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.RXEDGIE
    #define SCI2BDH_LBKDIE              _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.LBKDIE
    #define SCI2BDH_SBR_8               _SCI2BD.Overlap_STR.SCI2BDHSTR.MergedBits.grpSBR_8
    #define SCI2BDH_SBR                 SCI2BDH_SBR_8
    
    #define SCI2BDH_SBR8_MASK           1
    #define SCI2BDH_SBR9_MASK           2
    #define SCI2BDH_SBR10_MASK          4
    #define SCI2BDH_SBR11_MASK          8
    #define SCI2BDH_SBR12_MASK          16
    #define SCI2BDH_RXEDGIE_MASK        64
    #define SCI2BDH_LBKDIE_MASK         128
    #define SCI2BDH_SBR_8_MASK          31
    #define SCI2BDH_SBR_8_BITNUM        0
    

    /*** SCI2BDL - SCI2 Baud Rate Register Low; 0xFFFF8061 ***/
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

  struct {
    word SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
    word SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
    word SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
    word SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
    word SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
    word SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
    word SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
    word SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
    word SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
    word SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
    word SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
    word SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
    word SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
    word             :1; 
    word RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
    word LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
  } Bits;
  struct {
    word grpSBR  :13;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} SCI2BDSTR;
extern volatile SCI2BDSTR _SCI2BD @0xFFFF8060;
#define SCI2BD                          _SCI2BD.Word
#define SCI2BD_SBR0                     _SCI2BD.Bits.SBR0
#define SCI2BD_SBR1                     _SCI2BD.Bits.SBR1
#define SCI2BD_SBR2                     _SCI2BD.Bits.SBR2
#define SCI2BD_SBR3                     _SCI2BD.Bits.SBR3
#define SCI2BD_SBR4                     _SCI2BD.Bits.SBR4
#define SCI2BD_SBR5                     _SCI2BD.Bits.SBR5
#define SCI2BD_SBR6                     _SCI2BD.Bits.SBR6
#define SCI2BD_SBR7                     _SCI2BD.Bits.SBR7
#define SCI2BD_SBR8                     _SCI2BD.Bits.SBR8
#define SCI2BD_SBR9                     _SCI2BD.Bits.SBR9
#define SCI2BD_SBR10                    _SCI2BD.Bits.SBR10
#define SCI2BD_SBR11                    _SCI2BD.Bits.SBR11
#define SCI2BD_SBR12                    _SCI2BD.Bits.SBR12
#define SCI2BD_RXEDGIE                  _SCI2BD.Bits.RXEDGIE
#define SCI2BD_LBKDIE                   _SCI2BD.Bits.LBKDIE
#define SCI2BD_SBR                      _SCI2BD.MergedBits.grpSBR

#define SCI2BD_SBR0_MASK                1
#define SCI2BD_SBR1_MASK                2
#define SCI2BD_SBR2_MASK                4
#define SCI2BD_SBR3_MASK                8
#define SCI2BD_SBR4_MASK                16
#define SCI2BD_SBR5_MASK                32
#define SCI2BD_SBR6_MASK                64
#define SCI2BD_SBR7_MASK                128
#define SCI2BD_SBR8_MASK                256
#define SCI2BD_SBR9_MASK                512
#define SCI2BD_SBR10_MASK               1024
#define SCI2BD_SBR11_MASK               2048
#define SCI2BD_SBR12_MASK               4096
#define SCI2BD_RXEDGIE_MASK             16384
#define SCI2BD_LBKDIE_MASK              32768
#define SCI2BD_SBR_MASK                 8191
#define SCI2BD_SBR_BITNUM               0


/*** SCI2C1 - SCI2 Control Register 1; 0xFFFF8062 ***/
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
extern volatile SCI2C1STR _SCI2C1 @0xFFFF8062;
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


/*** SCI2C2 - SCI2 Control Register 2; 0xFFFF8063 ***/
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
extern volatile SCI2C2STR _SCI2C2 @0xFFFF8063;
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


/*** SCI2S1 - SCI2 Status Register 1; 0xFFFF8064 ***/
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
extern volatile SCI2S1STR _SCI2S1 @0xFFFF8064;
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


/*** SCI2S2 - SCI2 Status Register 2; 0xFFFF8065 ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte LBKDE       :1;                                       /* LIN Break Detection Enable */
    byte BRK13       :1;                                       /* Break Character Generation Length */
    byte RWUID       :1;                                       /* Receive Wake Up Idle Detect */
    byte RXINV       :1;                                       /* Receive Data Inversion */
    byte             :1; 
    byte RXEDGIF     :1;                                       /* RxD Pin Active Edge Interrupt Flag */
    byte LBKDIF      :1;                                       /* LIN Break Detect Interrupt Flag */
  } Bits;
} SCI2S2STR;
extern volatile SCI2S2STR _SCI2S2 @0xFFFF8065;
#define SCI2S2                          _SCI2S2.Byte
#define SCI2S2_RAF                      _SCI2S2.Bits.RAF
#define SCI2S2_LBKDE                    _SCI2S2.Bits.LBKDE
#define SCI2S2_BRK13                    _SCI2S2.Bits.BRK13
#define SCI2S2_RWUID                    _SCI2S2.Bits.RWUID
#define SCI2S2_RXINV                    _SCI2S2.Bits.RXINV
#define SCI2S2_RXEDGIF                  _SCI2S2.Bits.RXEDGIF
#define SCI2S2_LBKDIF                   _SCI2S2.Bits.LBKDIF

#define SCI2S2_RAF_MASK                 1
#define SCI2S2_LBKDE_MASK               2
#define SCI2S2_BRK13_MASK               4
#define SCI2S2_RWUID_MASK               8
#define SCI2S2_RXINV_MASK               16
#define SCI2S2_RXEDGIF_MASK             64
#define SCI2S2_LBKDIF_MASK              128


/*** SCI2C3 - SCI2 Control Register 3; 0xFFFF8066 ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte TXINV       :1;                                       /* Transmit Data Inversion */
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI2C3STR;
extern volatile SCI2C3STR _SCI2C3 @0xFFFF8066;
#define SCI2C3                          _SCI2C3.Byte
#define SCI2C3_PEIE                     _SCI2C3.Bits.PEIE
#define SCI2C3_FEIE                     _SCI2C3.Bits.FEIE
#define SCI2C3_NEIE                     _SCI2C3.Bits.NEIE
#define SCI2C3_ORIE                     _SCI2C3.Bits.ORIE
#define SCI2C3_TXINV                    _SCI2C3.Bits.TXINV
#define SCI2C3_TXDIR                    _SCI2C3.Bits.TXDIR
#define SCI2C3_T8                       _SCI2C3.Bits.T8
#define SCI2C3_R8                       _SCI2C3.Bits.R8

#define SCI2C3_PEIE_MASK                1
#define SCI2C3_FEIE_MASK                2
#define SCI2C3_NEIE_MASK                4
#define SCI2C3_ORIE_MASK                8
#define SCI2C3_TXINV_MASK               16
#define SCI2C3_TXDIR_MASK               32
#define SCI2C3_T8_MASK                  64
#define SCI2C3_R8_MASK                  128


/*** SCI2D - SCI2 Data Register; 0xFFFF8067 ***/
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
extern volatile SCI2DSTR _SCI2D @0xFFFF8067;
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


/*** SCI2MA1 - SCI2 Match Address Registers 1; 0xFFFF8068 ***/
typedef union {
  byte Byte;
  struct {
    byte MA0         :1;                                       /* Match address value, bit 0 */
    byte MA1         :1;                                       /* Match address value, bit 1 */
    byte MA2         :1;                                       /* Match address value, bit 2 */
    byte MA3         :1;                                       /* Match address value, bit 3 */
    byte MA4         :1;                                       /* Match address value, bit 4 */
    byte MA5         :1;                                       /* Match address value, bit 5 */
    byte MA6         :1;                                       /* Match address value, bit 6 */
    byte MA7         :1;                                       /* Match address value, bit 7 */
  } Bits;
} SCI2MA1STR;
extern volatile SCI2MA1STR _SCI2MA1 @0xFFFF8068;
#define SCI2MA1                         _SCI2MA1.Byte
#define SCI2MA1_MA0                     _SCI2MA1.Bits.MA0
#define SCI2MA1_MA1                     _SCI2MA1.Bits.MA1
#define SCI2MA1_MA2                     _SCI2MA1.Bits.MA2
#define SCI2MA1_MA3                     _SCI2MA1.Bits.MA3
#define SCI2MA1_MA4                     _SCI2MA1.Bits.MA4
#define SCI2MA1_MA5                     _SCI2MA1.Bits.MA5
#define SCI2MA1_MA6                     _SCI2MA1.Bits.MA6
#define SCI2MA1_MA7                     _SCI2MA1.Bits.MA7

#define SCI2MA1_MA0_MASK                1
#define SCI2MA1_MA1_MASK                2
#define SCI2MA1_MA2_MASK                4
#define SCI2MA1_MA3_MASK                8
#define SCI2MA1_MA4_MASK                16
#define SCI2MA1_MA5_MASK                32
#define SCI2MA1_MA6_MASK                64
#define SCI2MA1_MA7_MASK                128


/*** SCI2MA2 - SCI2 Match Address Registers 2; 0xFFFF8069 ***/
typedef union {
  byte Byte;
  struct {
    byte MA0         :1;                                       /* Match address value, bit 0 */
    byte MA1         :1;                                       /* Match address value, bit 1 */
    byte MA2         :1;                                       /* Match address value, bit 2 */
    byte MA3         :1;                                       /* Match address value, bit 3 */
    byte MA4         :1;                                       /* Match address value, bit 4 */
    byte MA5         :1;                                       /* Match address value, bit 5 */
    byte MA6         :1;                                       /* Match address value, bit 6 */
    byte MA7         :1;                                       /* Match address value, bit 7 */
  } Bits;
} SCI2MA2STR;
extern volatile SCI2MA2STR _SCI2MA2 @0xFFFF8069;
#define SCI2MA2                         _SCI2MA2.Byte
#define SCI2MA2_MA0                     _SCI2MA2.Bits.MA0
#define SCI2MA2_MA1                     _SCI2MA2.Bits.MA1
#define SCI2MA2_MA2                     _SCI2MA2.Bits.MA2
#define SCI2MA2_MA3                     _SCI2MA2.Bits.MA3
#define SCI2MA2_MA4                     _SCI2MA2.Bits.MA4
#define SCI2MA2_MA5                     _SCI2MA2.Bits.MA5
#define SCI2MA2_MA6                     _SCI2MA2.Bits.MA6
#define SCI2MA2_MA7                     _SCI2MA2.Bits.MA7

#define SCI2MA2_MA0_MASK                1
#define SCI2MA2_MA1_MASK                2
#define SCI2MA2_MA2_MASK                4
#define SCI2MA2_MA3_MASK                8
#define SCI2MA2_MA4_MASK                16
#define SCI2MA2_MA5_MASK                32
#define SCI2MA2_MA6_MASK                64
#define SCI2MA2_MA7_MASK                128


/*** SCI2C4 - SCI2 Control Register 4; 0xFFFF806A ***/
typedef union {
  byte Byte;
  struct {
    byte BRFA0       :1;                                       /* Baud rate fine adjust, bit 0 */
    byte BRFA1       :1;                                       /* Baud rate fine adjust, bit 1 */
    byte BRFA2       :1;                                       /* Baud rate fine adjust, bit 2 */
    byte BRFA3       :1;                                       /* Baud rate fine adjust, bit 3 */
    byte BRFA4       :1;                                       /* Baud rate fine adjust, bit 4 */
    byte             :1; 
    byte MAEN2       :1;                                       /* Match Address Mode Enable 2 */
    byte MAEN1       :1;                                       /* Match Address Mode Enable 1 */
  } Bits;
  struct {
    byte grpBRFA :5;
    byte         :1;
    byte grpMAEN_2 :1;
    byte grpMAEN_1 :1;
  } MergedBits;
} SCI2C4STR;
extern volatile SCI2C4STR _SCI2C4 @0xFFFF806A;
#define SCI2C4                          _SCI2C4.Byte
#define SCI2C4_BRFA0                    _SCI2C4.Bits.BRFA0
#define SCI2C4_BRFA1                    _SCI2C4.Bits.BRFA1
#define SCI2C4_BRFA2                    _SCI2C4.Bits.BRFA2
#define SCI2C4_BRFA3                    _SCI2C4.Bits.BRFA3
#define SCI2C4_BRFA4                    _SCI2C4.Bits.BRFA4
#define SCI2C4_MAEN2                    _SCI2C4.Bits.MAEN2
#define SCI2C4_MAEN1                    _SCI2C4.Bits.MAEN1
#define SCI2C4_BRFA                     _SCI2C4.MergedBits.grpBRFA

#define SCI2C4_BRFA0_MASK               1
#define SCI2C4_BRFA1_MASK               2
#define SCI2C4_BRFA2_MASK               4
#define SCI2C4_BRFA3_MASK               8
#define SCI2C4_BRFA4_MASK               16
#define SCI2C4_MAEN2_MASK               64
#define SCI2C4_MAEN1_MASK               128
#define SCI2C4_BRFA_MASK                31
#define SCI2C4_BRFA_BITNUM              0


/*** SCI2C5 - SCI2 Control Register 5; 0xFFFF806B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RDMAS       :1;                                       /* Receiver Full DMA Select */
    byte             :1; 
    byte TDMAS       :1;                                       /* Transmitter DMA Select */
  } Bits;
} SCI2C5STR;
extern volatile SCI2C5STR _SCI2C5 @0xFFFF806B;
#define SCI2C5                          _SCI2C5.Byte
#define SCI2C5_RDMAS                    _SCI2C5.Bits.RDMAS
#define SCI2C5_TDMAS                    _SCI2C5.Bits.TDMAS

#define SCI2C5_RDMAS_MASK               32
#define SCI2C5_TDMAS_MASK               128


/*** TPM3SC - TPM3 Status and Control Register; 0xFFFF8070 ***/
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
} TPM3SCSTR;
extern volatile TPM3SCSTR _TPM3SC @0xFFFF8070;
#define TPM3SC                          _TPM3SC.Byte
#define TPM3SC_PS0                      _TPM3SC.Bits.PS0
#define TPM3SC_PS1                      _TPM3SC.Bits.PS1
#define TPM3SC_PS2                      _TPM3SC.Bits.PS2
#define TPM3SC_CLKSA                    _TPM3SC.Bits.CLKSA
#define TPM3SC_CLKSB                    _TPM3SC.Bits.CLKSB
#define TPM3SC_CPWMS                    _TPM3SC.Bits.CPWMS
#define TPM3SC_TOIE                     _TPM3SC.Bits.TOIE
#define TPM3SC_TOF                      _TPM3SC.Bits.TOF
#define TPM3SC_PS                       _TPM3SC.MergedBits.grpPS
#define TPM3SC_CLKSx                    _TPM3SC.MergedBits.grpCLKSx

#define TPM3SC_PS0_MASK                 1
#define TPM3SC_PS1_MASK                 2
#define TPM3SC_PS2_MASK                 4
#define TPM3SC_CLKSA_MASK               8
#define TPM3SC_CLKSB_MASK               16
#define TPM3SC_CPWMS_MASK               32
#define TPM3SC_TOIE_MASK                64
#define TPM3SC_TOF_MASK                 128
#define TPM3SC_PS_MASK                  7
#define TPM3SC_PS_BITNUM                0
#define TPM3SC_CLKSx_MASK               24
#define TPM3SC_CLKSx_BITNUM             3


/*** TPM3CNT - TPM3 Timer Counter Register; 0xFFFF8071 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM3CNTH - TPM3 Timer Counter Register High; 0xFFFF8071 ***/
    union {
      byte Byte;
    } TPM3CNTHSTR;
    #define TPM3CNTH                    _TPM3CNT.Overlap_STR.TPM3CNTHSTR.Byte
    

    /*** TPM3CNTL - TPM3 Timer Counter Register Low; 0xFFFF8072 ***/
    union {
      byte Byte;
    } TPM3CNTLSTR;
    #define TPM3CNTL                    _TPM3CNT.Overlap_STR.TPM3CNTLSTR.Byte
    
  } Overlap_STR;

} TPM3CNTSTR;
extern volatile TPM3CNTSTR _TPM3CNT @0xFFFF8071;
#define TPM3CNT                         _TPM3CNT.Word


/*** TPM3MOD - TPM3 Timer Counter Modulo Register; 0xFFFF8073 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM3MODH - TPM3 Timer Counter Modulo Register High; 0xFFFF8073 ***/
    union {
      byte Byte;
    } TPM3MODHSTR;
    #define TPM3MODH                    _TPM3MOD.Overlap_STR.TPM3MODHSTR.Byte
    

    /*** TPM3MODL - TPM3 Timer Counter Modulo Register Low; 0xFFFF8074 ***/
    union {
      byte Byte;
    } TPM3MODLSTR;
    #define TPM3MODL                    _TPM3MOD.Overlap_STR.TPM3MODLSTR.Byte
    
  } Overlap_STR;

} TPM3MODSTR;
extern volatile TPM3MODSTR _TPM3MOD @0xFFFF8073;
#define TPM3MOD                         _TPM3MOD.Word


/*** TPM3C0SC - TPM3 Timer Channel 0 Status and Control Register; 0xFFFF8075 ***/
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
} TPM3C0SCSTR;
extern volatile TPM3C0SCSTR _TPM3C0SC @0xFFFF8075;
#define TPM3C0SC                        _TPM3C0SC.Byte
#define TPM3C0SC_ELS0A                  _TPM3C0SC.Bits.ELS0A
#define TPM3C0SC_ELS0B                  _TPM3C0SC.Bits.ELS0B
#define TPM3C0SC_MS0A                   _TPM3C0SC.Bits.MS0A
#define TPM3C0SC_MS0B                   _TPM3C0SC.Bits.MS0B
#define TPM3C0SC_CH0IE                  _TPM3C0SC.Bits.CH0IE
#define TPM3C0SC_CH0F                   _TPM3C0SC.Bits.CH0F
#define TPM3C0SC_ELS0x                  _TPM3C0SC.MergedBits.grpELS0x
#define TPM3C0SC_MS0x                   _TPM3C0SC.MergedBits.grpMS0x

#define TPM3C0SC_ELS0A_MASK             4
#define TPM3C0SC_ELS0B_MASK             8
#define TPM3C0SC_MS0A_MASK              16
#define TPM3C0SC_MS0B_MASK              32
#define TPM3C0SC_CH0IE_MASK             64
#define TPM3C0SC_CH0F_MASK              128
#define TPM3C0SC_ELS0x_MASK             12
#define TPM3C0SC_ELS0x_BITNUM           2
#define TPM3C0SC_MS0x_MASK              48
#define TPM3C0SC_MS0x_BITNUM            4


/*** TPM3C0V - TPM3 Timer Channel 0 Value Register; 0xFFFF8076 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM3C0VH - TPM3 Timer Channel 0 Value Register High; 0xFFFF8076 ***/
    union {
      byte Byte;
    } TPM3C0VHSTR;
    #define TPM3C0VH                    _TPM3C0V.Overlap_STR.TPM3C0VHSTR.Byte
    

    /*** TPM3C0VL - TPM3 Timer Channel 0 Value Register Low; 0xFFFF8077 ***/
    union {
      byte Byte;
    } TPM3C0VLSTR;
    #define TPM3C0VL                    _TPM3C0V.Overlap_STR.TPM3C0VLSTR.Byte
    
  } Overlap_STR;

} TPM3C0VSTR;
extern volatile TPM3C0VSTR _TPM3C0V @0xFFFF8076;
#define TPM3C0V                         _TPM3C0V.Word


/*** TPM3C1SC - TPM3 Timer Channel 1 Status and Control Register; 0xFFFF8078 ***/
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
} TPM3C1SCSTR;
extern volatile TPM3C1SCSTR _TPM3C1SC @0xFFFF8078;
#define TPM3C1SC                        _TPM3C1SC.Byte
#define TPM3C1SC_ELS1A                  _TPM3C1SC.Bits.ELS1A
#define TPM3C1SC_ELS1B                  _TPM3C1SC.Bits.ELS1B
#define TPM3C1SC_MS1A                   _TPM3C1SC.Bits.MS1A
#define TPM3C1SC_MS1B                   _TPM3C1SC.Bits.MS1B
#define TPM3C1SC_CH1IE                  _TPM3C1SC.Bits.CH1IE
#define TPM3C1SC_CH1F                   _TPM3C1SC.Bits.CH1F
#define TPM3C1SC_ELS1x                  _TPM3C1SC.MergedBits.grpELS1x
#define TPM3C1SC_MS1x                   _TPM3C1SC.MergedBits.grpMS1x

#define TPM3C1SC_ELS1A_MASK             4
#define TPM3C1SC_ELS1B_MASK             8
#define TPM3C1SC_MS1A_MASK              16
#define TPM3C1SC_MS1B_MASK              32
#define TPM3C1SC_CH1IE_MASK             64
#define TPM3C1SC_CH1F_MASK              128
#define TPM3C1SC_ELS1x_MASK             12
#define TPM3C1SC_ELS1x_BITNUM           2
#define TPM3C1SC_MS1x_MASK              48
#define TPM3C1SC_MS1x_BITNUM            4


/*** TPM3C1V - TPM3 Timer Channel 1 Value Register; 0xFFFF8079 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM3C1VH - TPM3 Timer Channel 1 Value Register High; 0xFFFF8079 ***/
    union {
      byte Byte;
    } TPM3C1VHSTR;
    #define TPM3C1VH                    _TPM3C1V.Overlap_STR.TPM3C1VHSTR.Byte
    

    /*** TPM3C1VL - TPM3 Timer Channel 1 Value Register Low; 0xFFFF807A ***/
    union {
      byte Byte;
    } TPM3C1VLSTR;
    #define TPM3C1VL                    _TPM3C1V.Overlap_STR.TPM3C1VLSTR.Byte
    
  } Overlap_STR;

} TPM3C1VSTR;
extern volatile TPM3C1VSTR _TPM3C1V @0xFFFF8079;
#define TPM3C1V                         _TPM3C1V.Word


/*** FTM1SC - FTM1 Status and Control Register; 0xFFFF8080 ***/
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
} FTM1SCSTR;
extern volatile FTM1SCSTR _FTM1SC @0xFFFF8080;
#define FTM1SC                          _FTM1SC.Byte
#define FTM1SC_PS0                      _FTM1SC.Bits.PS0
#define FTM1SC_PS1                      _FTM1SC.Bits.PS1
#define FTM1SC_PS2                      _FTM1SC.Bits.PS2
#define FTM1SC_CLKSA                    _FTM1SC.Bits.CLKSA
#define FTM1SC_CLKSB                    _FTM1SC.Bits.CLKSB
#define FTM1SC_CPWMS                    _FTM1SC.Bits.CPWMS
#define FTM1SC_TOIE                     _FTM1SC.Bits.TOIE
#define FTM1SC_TOF                      _FTM1SC.Bits.TOF
#define FTM1SC_PS                       _FTM1SC.MergedBits.grpPS
#define FTM1SC_CLKSx                    _FTM1SC.MergedBits.grpCLKSx

#define FTM1SC_PS0_MASK                 1
#define FTM1SC_PS1_MASK                 2
#define FTM1SC_PS2_MASK                 4
#define FTM1SC_CLKSA_MASK               8
#define FTM1SC_CLKSB_MASK               16
#define FTM1SC_CPWMS_MASK               32
#define FTM1SC_TOIE_MASK                64
#define FTM1SC_TOF_MASK                 128
#define FTM1SC_PS_MASK                  7
#define FTM1SC_PS_BITNUM                0
#define FTM1SC_CLKSx_MASK               24
#define FTM1SC_CLKSx_BITNUM             3


/*** FTM1CNT - FTM1 Timer Counter Register; 0xFFFF8081 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1CNTH - FTM1 Timer Counter Register High; 0xFFFF8081 ***/
    union {
      byte Byte;
    } FTM1CNTHSTR;
    #define FTM1CNTH                    _FTM1CNT.Overlap_STR.FTM1CNTHSTR.Byte
    

    /*** FTM1CNTL - FTM1 Timer Counter Register Low; 0xFFFF8082 ***/
    union {
      byte Byte;
    } FTM1CNTLSTR;
    #define FTM1CNTL                    _FTM1CNT.Overlap_STR.FTM1CNTLSTR.Byte
    
  } Overlap_STR;

} FTM1CNTSTR;
extern volatile FTM1CNTSTR _FTM1CNT @0xFFFF8081;
#define FTM1CNT                         _FTM1CNT.Word


/*** FTM1MOD - FTM1 Timer Counter Modulo Register; 0xFFFF8083 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1MODH - FTM1 Timer Counter Modulo Register High; 0xFFFF8083 ***/
    union {
      byte Byte;
    } FTM1MODHSTR;
    #define FTM1MODH                    _FTM1MOD.Overlap_STR.FTM1MODHSTR.Byte
    

    /*** FTM1MODL - FTM1 Timer Counter Modulo Register Low; 0xFFFF8084 ***/
    union {
      byte Byte;
    } FTM1MODLSTR;
    #define FTM1MODL                    _FTM1MOD.Overlap_STR.FTM1MODLSTR.Byte
    
  } Overlap_STR;

} FTM1MODSTR;
extern volatile FTM1MODSTR _FTM1MOD @0xFFFF8083;
#define FTM1MOD                         _FTM1MOD.Word


/*** FTM1C0SC - FTM1 Timer Channel 0 Status and Control Register; 0xFFFF8085 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH0F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for FTM1 Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for FTM1 Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C0SCSTR;
extern volatile FTM1C0SCSTR _FTM1C0SC @0xFFFF8085;
#define FTM1C0SC                        _FTM1C0SC.Byte
#define FTM1C0SC_DMA                    _FTM1C0SC.Bits.DMA
#define FTM1C0SC_ELS0A                  _FTM1C0SC.Bits.ELS0A
#define FTM1C0SC_ELS0B                  _FTM1C0SC.Bits.ELS0B
#define FTM1C0SC_MS0A                   _FTM1C0SC.Bits.MS0A
#define FTM1C0SC_MS0B                   _FTM1C0SC.Bits.MS0B
#define FTM1C0SC_CH0IE                  _FTM1C0SC.Bits.CH0IE
#define FTM1C0SC_CH0F                   _FTM1C0SC.Bits.CH0F
#define FTM1C0SC_ELS0x                  _FTM1C0SC.MergedBits.grpELS0x
#define FTM1C0SC_MS0x                   _FTM1C0SC.MergedBits.grpMS0x

#define FTM1C0SC_DMA_MASK               1
#define FTM1C0SC_ELS0A_MASK             4
#define FTM1C0SC_ELS0B_MASK             8
#define FTM1C0SC_MS0A_MASK              16
#define FTM1C0SC_MS0B_MASK              32
#define FTM1C0SC_CH0IE_MASK             64
#define FTM1C0SC_CH0F_MASK              128
#define FTM1C0SC_ELS0x_MASK             12
#define FTM1C0SC_ELS0x_BITNUM           2
#define FTM1C0SC_MS0x_MASK              48
#define FTM1C0SC_MS0x_BITNUM            4


/*** FTM1C0V - FTM1 Timer Channel 0 Value Register; 0xFFFF8086 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C0VH - FTM1 Timer Channel 0 Value Register High; 0xFFFF8086 ***/
    union {
      byte Byte;
    } FTM1C0VHSTR;
    #define FTM1C0VH                    _FTM1C0V.Overlap_STR.FTM1C0VHSTR.Byte
    

    /*** FTM1C0VL - FTM1 Timer Channel 0 Value Register Low; 0xFFFF8087 ***/
    union {
      byte Byte;
    } FTM1C0VLSTR;
    #define FTM1C0VL                    _FTM1C0V.Overlap_STR.FTM1C0VLSTR.Byte
    
  } Overlap_STR;

} FTM1C0VSTR;
extern volatile FTM1C0VSTR _FTM1C0V @0xFFFF8086;
#define FTM1C0V                         _FTM1C0V.Word


/*** FTM1C1SC - FTM1 Timer Channel 1 Status and Control Register; 0xFFFF8088 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH1F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for FTM1 Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for FTM1 Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C1SCSTR;
extern volatile FTM1C1SCSTR _FTM1C1SC @0xFFFF8088;
#define FTM1C1SC                        _FTM1C1SC.Byte
#define FTM1C1SC_DMA                    _FTM1C1SC.Bits.DMA
#define FTM1C1SC_ELS1A                  _FTM1C1SC.Bits.ELS1A
#define FTM1C1SC_ELS1B                  _FTM1C1SC.Bits.ELS1B
#define FTM1C1SC_MS1A                   _FTM1C1SC.Bits.MS1A
#define FTM1C1SC_MS1B                   _FTM1C1SC.Bits.MS1B
#define FTM1C1SC_CH1IE                  _FTM1C1SC.Bits.CH1IE
#define FTM1C1SC_CH1F                   _FTM1C1SC.Bits.CH1F
#define FTM1C1SC_ELS1x                  _FTM1C1SC.MergedBits.grpELS1x
#define FTM1C1SC_MS1x                   _FTM1C1SC.MergedBits.grpMS1x

#define FTM1C1SC_DMA_MASK               1
#define FTM1C1SC_ELS1A_MASK             4
#define FTM1C1SC_ELS1B_MASK             8
#define FTM1C1SC_MS1A_MASK              16
#define FTM1C1SC_MS1B_MASK              32
#define FTM1C1SC_CH1IE_MASK             64
#define FTM1C1SC_CH1F_MASK              128
#define FTM1C1SC_ELS1x_MASK             12
#define FTM1C1SC_ELS1x_BITNUM           2
#define FTM1C1SC_MS1x_MASK              48
#define FTM1C1SC_MS1x_BITNUM            4


/*** FTM1C1V - FTM1 Timer Channel 1 Value Register; 0xFFFF8089 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C1VH - FTM1 Timer Channel 1 Value Register High; 0xFFFF8089 ***/
    union {
      byte Byte;
    } FTM1C1VHSTR;
    #define FTM1C1VH                    _FTM1C1V.Overlap_STR.FTM1C1VHSTR.Byte
    

    /*** FTM1C1VL - FTM1 Timer Channel 1 Value Register Low; 0xFFFF808A ***/
    union {
      byte Byte;
    } FTM1C1VLSTR;
    #define FTM1C1VL                    _FTM1C1V.Overlap_STR.FTM1C1VLSTR.Byte
    
  } Overlap_STR;

} FTM1C1VSTR;
extern volatile FTM1C1VSTR _FTM1C1V @0xFFFF8089;
#define FTM1C1V                         _FTM1C1V.Word


/*** FTM1C2SC - FTM1 Timer Channel 2 Status and Control Register; 0xFFFF808B ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH2F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS2A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS2B       :1;                                       /* Edge/Level Select Bit B */
    byte MS2A        :1;                                       /* Mode Select A for FTM1 Channel 2 */
    byte MS2B        :1;                                       /* Mode Select B for FTM1 Channel 2 */
    byte CH2IE       :1;                                       /* Channel 2 Interrupt Enable */
    byte CH2F        :1;                                       /* Channel 2 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS2x :2;
    byte grpMS2x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C2SCSTR;
extern volatile FTM1C2SCSTR _FTM1C2SC @0xFFFF808B;
#define FTM1C2SC                        _FTM1C2SC.Byte
#define FTM1C2SC_DMA                    _FTM1C2SC.Bits.DMA
#define FTM1C2SC_ELS2A                  _FTM1C2SC.Bits.ELS2A
#define FTM1C2SC_ELS2B                  _FTM1C2SC.Bits.ELS2B
#define FTM1C2SC_MS2A                   _FTM1C2SC.Bits.MS2A
#define FTM1C2SC_MS2B                   _FTM1C2SC.Bits.MS2B
#define FTM1C2SC_CH2IE                  _FTM1C2SC.Bits.CH2IE
#define FTM1C2SC_CH2F                   _FTM1C2SC.Bits.CH2F
#define FTM1C2SC_ELS2x                  _FTM1C2SC.MergedBits.grpELS2x
#define FTM1C2SC_MS2x                   _FTM1C2SC.MergedBits.grpMS2x

#define FTM1C2SC_DMA_MASK               1
#define FTM1C2SC_ELS2A_MASK             4
#define FTM1C2SC_ELS2B_MASK             8
#define FTM1C2SC_MS2A_MASK              16
#define FTM1C2SC_MS2B_MASK              32
#define FTM1C2SC_CH2IE_MASK             64
#define FTM1C2SC_CH2F_MASK              128
#define FTM1C2SC_ELS2x_MASK             12
#define FTM1C2SC_ELS2x_BITNUM           2
#define FTM1C2SC_MS2x_MASK              48
#define FTM1C2SC_MS2x_BITNUM            4


/*** FTM1C2V - FTM1 Timer Channel 2 Value Register; 0xFFFF808C ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C2VH - FTM1 Timer Channel 2 Value Register High; 0xFFFF808C ***/
    union {
      byte Byte;
    } FTM1C2VHSTR;
    #define FTM1C2VH                    _FTM1C2V.Overlap_STR.FTM1C2VHSTR.Byte
    

    /*** FTM1C2VL - FTM1 Timer Channel 2 Value Register Low; 0xFFFF808D ***/
    union {
      byte Byte;
    } FTM1C2VLSTR;
    #define FTM1C2VL                    _FTM1C2V.Overlap_STR.FTM1C2VLSTR.Byte
    
  } Overlap_STR;

} FTM1C2VSTR;
extern volatile FTM1C2VSTR _FTM1C2V @0xFFFF808C;
#define FTM1C2V                         _FTM1C2V.Word


/*** FTM1C3SC - FTM1 Timer Channel 3 Status and Control Register; 0xFFFF808E ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH3F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS3A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS3B       :1;                                       /* Edge/Level Select Bit B */
    byte MS3A        :1;                                       /* Mode Select A for FTM1 Channel 3 */
    byte MS3B        :1;                                       /* Mode Select B for FTM1 Channel 3 */
    byte CH3IE       :1;                                       /* Channel 3 Interrupt Enable */
    byte CH3F        :1;                                       /* Channel 3 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS3x :2;
    byte grpMS3x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C3SCSTR;
extern volatile FTM1C3SCSTR _FTM1C3SC @0xFFFF808E;
#define FTM1C3SC                        _FTM1C3SC.Byte
#define FTM1C3SC_DMA                    _FTM1C3SC.Bits.DMA
#define FTM1C3SC_ELS3A                  _FTM1C3SC.Bits.ELS3A
#define FTM1C3SC_ELS3B                  _FTM1C3SC.Bits.ELS3B
#define FTM1C3SC_MS3A                   _FTM1C3SC.Bits.MS3A
#define FTM1C3SC_MS3B                   _FTM1C3SC.Bits.MS3B
#define FTM1C3SC_CH3IE                  _FTM1C3SC.Bits.CH3IE
#define FTM1C3SC_CH3F                   _FTM1C3SC.Bits.CH3F
#define FTM1C3SC_ELS3x                  _FTM1C3SC.MergedBits.grpELS3x
#define FTM1C3SC_MS3x                   _FTM1C3SC.MergedBits.grpMS3x

#define FTM1C3SC_DMA_MASK               1
#define FTM1C3SC_ELS3A_MASK             4
#define FTM1C3SC_ELS3B_MASK             8
#define FTM1C3SC_MS3A_MASK              16
#define FTM1C3SC_MS3B_MASK              32
#define FTM1C3SC_CH3IE_MASK             64
#define FTM1C3SC_CH3F_MASK              128
#define FTM1C3SC_ELS3x_MASK             12
#define FTM1C3SC_ELS3x_BITNUM           2
#define FTM1C3SC_MS3x_MASK              48
#define FTM1C3SC_MS3x_BITNUM            4


/*** FTM1C3V - FTM1 Timer Channel 3 Value Register; 0xFFFF808F ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C3VH - FTM1 Timer Channel 3 Value Register High; 0xFFFF808F ***/
    union {
      byte Byte;
    } FTM1C3VHSTR;
    #define FTM1C3VH                    _FTM1C3V.Overlap_STR.FTM1C3VHSTR.Byte
    

    /*** FTM1C3VL - FTM1 Timer Channel 3 Value Register Low; 0xFFFF8090 ***/
    union {
      byte Byte;
    } FTM1C3VLSTR;
    #define FTM1C3VL                    _FTM1C3V.Overlap_STR.FTM1C3VLSTR.Byte
    
  } Overlap_STR;

} FTM1C3VSTR;
extern volatile FTM1C3VSTR _FTM1C3V @0xFFFF808F;
#define FTM1C3V                         _FTM1C3V.Word


/*** FTM1C4SC - FTM1 Timer Channel 4 Status and Control Register; 0xFFFF8091 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH4F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS4A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS4B       :1;                                       /* Edge/Level Select Bit B */
    byte MS4A        :1;                                       /* Mode Select A for FTM1 Channel 4 */
    byte MS4B        :1;                                       /* Mode Select B for FTM1 Channel 4 */
    byte CH4IE       :1;                                       /* Channel 4 Interrupt Enable */
    byte CH4F        :1;                                       /* Channel 4 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS4x :2;
    byte grpMS4x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C4SCSTR;
extern volatile FTM1C4SCSTR _FTM1C4SC @0xFFFF8091;
#define FTM1C4SC                        _FTM1C4SC.Byte
#define FTM1C4SC_DMA                    _FTM1C4SC.Bits.DMA
#define FTM1C4SC_ELS4A                  _FTM1C4SC.Bits.ELS4A
#define FTM1C4SC_ELS4B                  _FTM1C4SC.Bits.ELS4B
#define FTM1C4SC_MS4A                   _FTM1C4SC.Bits.MS4A
#define FTM1C4SC_MS4B                   _FTM1C4SC.Bits.MS4B
#define FTM1C4SC_CH4IE                  _FTM1C4SC.Bits.CH4IE
#define FTM1C4SC_CH4F                   _FTM1C4SC.Bits.CH4F
#define FTM1C4SC_ELS4x                  _FTM1C4SC.MergedBits.grpELS4x
#define FTM1C4SC_MS4x                   _FTM1C4SC.MergedBits.grpMS4x

#define FTM1C4SC_DMA_MASK               1
#define FTM1C4SC_ELS4A_MASK             4
#define FTM1C4SC_ELS4B_MASK             8
#define FTM1C4SC_MS4A_MASK              16
#define FTM1C4SC_MS4B_MASK              32
#define FTM1C4SC_CH4IE_MASK             64
#define FTM1C4SC_CH4F_MASK              128
#define FTM1C4SC_ELS4x_MASK             12
#define FTM1C4SC_ELS4x_BITNUM           2
#define FTM1C4SC_MS4x_MASK              48
#define FTM1C4SC_MS4x_BITNUM            4


/*** FTM1C4V - FTM1 Timer Channel 4 Value Register; 0xFFFF8092 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C4VH - FTM1 Timer Channel 4 Value Register High; 0xFFFF8092 ***/
    union {
      byte Byte;
    } FTM1C4VHSTR;
    #define FTM1C4VH                    _FTM1C4V.Overlap_STR.FTM1C4VHSTR.Byte
    

    /*** FTM1C4VL - FTM1 Timer Channel 4 Value Register Low; 0xFFFF8093 ***/
    union {
      byte Byte;
    } FTM1C4VLSTR;
    #define FTM1C4VL                    _FTM1C4V.Overlap_STR.FTM1C4VLSTR.Byte
    
  } Overlap_STR;

} FTM1C4VSTR;
extern volatile FTM1C4VSTR _FTM1C4V @0xFFFF8092;
#define FTM1C4V                         _FTM1C4V.Word


/*** FTM1C5SC - FTM1 Timer Channel 5 Status and Control Register; 0xFFFF8094 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH5F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS5A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS5B       :1;                                       /* Edge/Level Select Bit B */
    byte MS5A        :1;                                       /* Mode Select A for FTM1 Channel 5 */
    byte MS5B        :1;                                       /* Mode Select B for FTM1 Channel 5 */
    byte CH5IE       :1;                                       /* Channel 5 Interrupt Enable */
    byte CH5F        :1;                                       /* Channel 5 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS5x :2;
    byte grpMS5x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1C5SCSTR;
extern volatile FTM1C5SCSTR _FTM1C5SC @0xFFFF8094;
#define FTM1C5SC                        _FTM1C5SC.Byte
#define FTM1C5SC_DMA                    _FTM1C5SC.Bits.DMA
#define FTM1C5SC_ELS5A                  _FTM1C5SC.Bits.ELS5A
#define FTM1C5SC_ELS5B                  _FTM1C5SC.Bits.ELS5B
#define FTM1C5SC_MS5A                   _FTM1C5SC.Bits.MS5A
#define FTM1C5SC_MS5B                   _FTM1C5SC.Bits.MS5B
#define FTM1C5SC_CH5IE                  _FTM1C5SC.Bits.CH5IE
#define FTM1C5SC_CH5F                   _FTM1C5SC.Bits.CH5F
#define FTM1C5SC_ELS5x                  _FTM1C5SC.MergedBits.grpELS5x
#define FTM1C5SC_MS5x                   _FTM1C5SC.MergedBits.grpMS5x

#define FTM1C5SC_DMA_MASK               1
#define FTM1C5SC_ELS5A_MASK             4
#define FTM1C5SC_ELS5B_MASK             8
#define FTM1C5SC_MS5A_MASK              16
#define FTM1C5SC_MS5B_MASK              32
#define FTM1C5SC_CH5IE_MASK             64
#define FTM1C5SC_CH5F_MASK              128
#define FTM1C5SC_ELS5x_MASK             12
#define FTM1C5SC_ELS5x_BITNUM           2
#define FTM1C5SC_MS5x_MASK              48
#define FTM1C5SC_MS5x_BITNUM            4


/*** FTM1C5V - FTM1 Timer Channel 5 Value Register; 0xFFFF8095 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1C5VH - FTM1 Timer Channel 5 Value Register High; 0xFFFF8095 ***/
    union {
      byte Byte;
    } FTM1C5VHSTR;
    #define FTM1C5VH                    _FTM1C5V.Overlap_STR.FTM1C5VHSTR.Byte
    

    /*** FTM1C5VL - FTM1 Timer Channel 5 Value Register Low; 0xFFFF8096 ***/
    union {
      byte Byte;
    } FTM1C5VLSTR;
    #define FTM1C5VL                    _FTM1C5V.Overlap_STR.FTM1C5VLSTR.Byte
    
  } Overlap_STR;

} FTM1C5VSTR;
extern volatile FTM1C5VSTR _FTM1C5V @0xFFFF8095;
#define FTM1C5V                         _FTM1C5V.Word


/*** FTM1CNTIN - FTM1 Counter Initial Value Registers; 0xFFFF80A0 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM1CNTINH - FTM1 Counter Initial Value Registers High; 0xFFFF80A0 ***/
    union {
      byte Byte;
      struct {
        byte BIT8        :1;                                       /* FTM1 Counter Initial Value Bit 8 */
        byte BIT9        :1;                                       /* FTM1 Counter Initial Value Bit 9 */
        byte BIT10       :1;                                       /* FTM1 Counter Initial Value Bit 10 */
        byte BIT11       :1;                                       /* FTM1 Counter Initial Value Bit 11 */
        byte BIT12       :1;                                       /* FTM1 Counter Initial Value Bit 12 */
        byte BIT13       :1;                                       /* FTM1 Counter Initial Value Bit 13 */
        byte BIT14       :1;                                       /* FTM1 Counter Initial Value Bit 14 */
        byte BIT15       :1;                                       /* FTM1 Counter Initial Value Bit 15 */
      } Bits;
    } FTM1CNTINHSTR;
    #define FTM1CNTINH                  _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Byte
    #define FTM1CNTINH_BIT8             _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT8
    #define FTM1CNTINH_BIT9             _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT9
    #define FTM1CNTINH_BIT10            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT10
    #define FTM1CNTINH_BIT11            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT11
    #define FTM1CNTINH_BIT12            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT12
    #define FTM1CNTINH_BIT13            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT13
    #define FTM1CNTINH_BIT14            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT14
    #define FTM1CNTINH_BIT15            _FTM1CNTIN.Overlap_STR.FTM1CNTINHSTR.Bits.BIT15
    
    #define FTM1CNTINH_BIT8_MASK        1
    #define FTM1CNTINH_BIT9_MASK        2
    #define FTM1CNTINH_BIT10_MASK       4
    #define FTM1CNTINH_BIT11_MASK       8
    #define FTM1CNTINH_BIT12_MASK       16
    #define FTM1CNTINH_BIT13_MASK       32
    #define FTM1CNTINH_BIT14_MASK       64
    #define FTM1CNTINH_BIT15_MASK       128
    

    /*** FTM1CNTINL - FTM1 Counter Initial Value Registers Low; 0xFFFF80A1 ***/
    union {
      byte Byte;
      struct {
        byte BIT0        :1;                                       /* FTM1 Counter Initial Value Bit 0 */
        byte BIT1        :1;                                       /* FTM1 Counter Initial Value Bit 1 */
        byte BIT2        :1;                                       /* FTM1 Counter Initial Value Bit 2 */
        byte BIT3        :1;                                       /* FTM1 Counter Initial Value Bit 3 */
        byte BIT4        :1;                                       /* FTM1 Counter Initial Value Bit 4 */
        byte BIT5        :1;                                       /* FTM1 Counter Initial Value Bit 5 */
        byte BIT6        :1;                                       /* FTM1 Counter Initial Value Bit 6 */
        byte BIT7        :1;                                       /* FTM1 Counter Initial Value Bit 7 */
      } Bits;
    } FTM1CNTINLSTR;
    #define FTM1CNTINL                  _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Byte
    #define FTM1CNTINL_BIT0             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT0
    #define FTM1CNTINL_BIT1             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT1
    #define FTM1CNTINL_BIT2             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT2
    #define FTM1CNTINL_BIT3             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT3
    #define FTM1CNTINL_BIT4             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT4
    #define FTM1CNTINL_BIT5             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT5
    #define FTM1CNTINL_BIT6             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT6
    #define FTM1CNTINL_BIT7             _FTM1CNTIN.Overlap_STR.FTM1CNTINLSTR.Bits.BIT7
    
    #define FTM1CNTINL_BIT0_MASK        1
    #define FTM1CNTINL_BIT1_MASK        2
    #define FTM1CNTINL_BIT2_MASK        4
    #define FTM1CNTINL_BIT3_MASK        8
    #define FTM1CNTINL_BIT4_MASK        16
    #define FTM1CNTINL_BIT5_MASK        32
    #define FTM1CNTINL_BIT6_MASK        64
    #define FTM1CNTINL_BIT7_MASK        128
    
  } Overlap_STR;

  struct {
    word BIT0        :1;                                       /* FTM1 Counter Initial Value Bit 0 */
    word BIT1        :1;                                       /* FTM1 Counter Initial Value Bit 1 */
    word BIT2        :1;                                       /* FTM1 Counter Initial Value Bit 2 */
    word BIT3        :1;                                       /* FTM1 Counter Initial Value Bit 3 */
    word BIT4        :1;                                       /* FTM1 Counter Initial Value Bit 4 */
    word BIT5        :1;                                       /* FTM1 Counter Initial Value Bit 5 */
    word BIT6        :1;                                       /* FTM1 Counter Initial Value Bit 6 */
    word BIT7        :1;                                       /* FTM1 Counter Initial Value Bit 7 */
    word BIT8        :1;                                       /* FTM1 Counter Initial Value Bit 8 */
    word BIT9        :1;                                       /* FTM1 Counter Initial Value Bit 9 */
    word BIT10       :1;                                       /* FTM1 Counter Initial Value Bit 10 */
    word BIT11       :1;                                       /* FTM1 Counter Initial Value Bit 11 */
    word BIT12       :1;                                       /* FTM1 Counter Initial Value Bit 12 */
    word BIT13       :1;                                       /* FTM1 Counter Initial Value Bit 13 */
    word BIT14       :1;                                       /* FTM1 Counter Initial Value Bit 14 */
    word BIT15       :1;                                       /* FTM1 Counter Initial Value Bit 15 */
  } Bits;
} FTM1CNTINSTR;
extern volatile FTM1CNTINSTR _FTM1CNTIN @0xFFFF80A0;
#define FTM1CNTIN                       _FTM1CNTIN.Word
#define FTM1CNTIN_BIT0                  _FTM1CNTIN.Bits.BIT0
#define FTM1CNTIN_BIT1                  _FTM1CNTIN.Bits.BIT1
#define FTM1CNTIN_BIT2                  _FTM1CNTIN.Bits.BIT2
#define FTM1CNTIN_BIT3                  _FTM1CNTIN.Bits.BIT3
#define FTM1CNTIN_BIT4                  _FTM1CNTIN.Bits.BIT4
#define FTM1CNTIN_BIT5                  _FTM1CNTIN.Bits.BIT5
#define FTM1CNTIN_BIT6                  _FTM1CNTIN.Bits.BIT6
#define FTM1CNTIN_BIT7                  _FTM1CNTIN.Bits.BIT7
#define FTM1CNTIN_BIT8                  _FTM1CNTIN.Bits.BIT8
#define FTM1CNTIN_BIT9                  _FTM1CNTIN.Bits.BIT9
#define FTM1CNTIN_BIT10                 _FTM1CNTIN.Bits.BIT10
#define FTM1CNTIN_BIT11                 _FTM1CNTIN.Bits.BIT11
#define FTM1CNTIN_BIT12                 _FTM1CNTIN.Bits.BIT12
#define FTM1CNTIN_BIT13                 _FTM1CNTIN.Bits.BIT13
#define FTM1CNTIN_BIT14                 _FTM1CNTIN.Bits.BIT14
#define FTM1CNTIN_BIT15                 _FTM1CNTIN.Bits.BIT15

#define FTM1CNTIN_BIT0_MASK             1
#define FTM1CNTIN_BIT1_MASK             2
#define FTM1CNTIN_BIT2_MASK             4
#define FTM1CNTIN_BIT3_MASK             8
#define FTM1CNTIN_BIT4_MASK             16
#define FTM1CNTIN_BIT5_MASK             32
#define FTM1CNTIN_BIT6_MASK             64
#define FTM1CNTIN_BIT7_MASK             128
#define FTM1CNTIN_BIT8_MASK             256
#define FTM1CNTIN_BIT9_MASK             512
#define FTM1CNTIN_BIT10_MASK            1024
#define FTM1CNTIN_BIT11_MASK            2048
#define FTM1CNTIN_BIT12_MASK            4096
#define FTM1CNTIN_BIT13_MASK            8192
#define FTM1CNTIN_BIT14_MASK            16384
#define FTM1CNTIN_BIT15_MASK            32768


/*** FTM1STATUS - FTM1 Capture and Compare Status Register; 0xFFFF80A2 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0F        :1;                                       /* Channel 0 Flag */
    byte CH1F        :1;                                       /* Channel 1 Flag */
    byte CH2F        :1;                                       /* Channel 2 Flag */
    byte CH3F        :1;                                       /* Channel 3 Flag */
    byte CH4F        :1;                                       /* Channel 4 Flag */
    byte CH5F        :1;                                       /* Channel 5 Flag */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM1STATUSSTR;
extern volatile FTM1STATUSSTR _FTM1STATUS @0xFFFF80A2;
#define FTM1STATUS                      _FTM1STATUS.Byte
#define FTM1STATUS_CH0F                 _FTM1STATUS.Bits.CH0F
#define FTM1STATUS_CH1F                 _FTM1STATUS.Bits.CH1F
#define FTM1STATUS_CH2F                 _FTM1STATUS.Bits.CH2F
#define FTM1STATUS_CH3F                 _FTM1STATUS.Bits.CH3F
#define FTM1STATUS_CH4F                 _FTM1STATUS.Bits.CH4F
#define FTM1STATUS_CH5F                 _FTM1STATUS.Bits.CH5F

#define FTM1STATUS_CH0F_MASK            1
#define FTM1STATUS_CH1F_MASK            2
#define FTM1STATUS_CH2F_MASK            4
#define FTM1STATUS_CH3F_MASK            8
#define FTM1STATUS_CH4F_MASK            16
#define FTM1STATUS_CH5F_MASK            32


/*** FTM1MODE - FTM1 Features Mode Selection Register; 0xFFFF80A3 ***/
typedef union {
  byte Byte;
  struct {
    byte FTMEN       :1;                                       /* FTM1 Enable */
    byte INIT        :1;                                       /* Initialize the Channels Output */
    byte WPDIS       :1;                                       /* Write Protected Disable */
    byte PWMSYNC     :1;                                       /* PWM synchronization mode */
    byte CAPTTEST    :1;                                       /* Capture test mode enable */
    byte FAULTM0     :1;                                       /* Fault Control Mode Bits, bit 0 */
    byte FAULTM1     :1;                                       /* Fault Control Mode Bits, bit 1 */
    byte FAULTIE     :1;                                       /* Fault Interrupt Enable */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpFAULTM :2;
    byte         :1;
  } MergedBits;
} FTM1MODESTR;
extern volatile FTM1MODESTR _FTM1MODE @0xFFFF80A3;
#define FTM1MODE                        _FTM1MODE.Byte
#define FTM1MODE_FTMEN                  _FTM1MODE.Bits.FTMEN
#define FTM1MODE_INIT                   _FTM1MODE.Bits.INIT
#define FTM1MODE_WPDIS                  _FTM1MODE.Bits.WPDIS
#define FTM1MODE_PWMSYNC                _FTM1MODE.Bits.PWMSYNC
#define FTM1MODE_CAPTTEST               _FTM1MODE.Bits.CAPTTEST
#define FTM1MODE_FAULTM0                _FTM1MODE.Bits.FAULTM0
#define FTM1MODE_FAULTM1                _FTM1MODE.Bits.FAULTM1
#define FTM1MODE_FAULTIE                _FTM1MODE.Bits.FAULTIE
#define FTM1MODE_FAULTM                 _FTM1MODE.MergedBits.grpFAULTM

#define FTM1MODE_FTMEN_MASK             1
#define FTM1MODE_INIT_MASK              2
#define FTM1MODE_WPDIS_MASK             4
#define FTM1MODE_PWMSYNC_MASK           8
#define FTM1MODE_CAPTTEST_MASK          16
#define FTM1MODE_FAULTM0_MASK           32
#define FTM1MODE_FAULTM1_MASK           64
#define FTM1MODE_FAULTIE_MASK           128
#define FTM1MODE_FAULTM_MASK            96
#define FTM1MODE_FAULTM_BITNUM          5


/*** FTM1SYNC - FTM1 Synchronization Register; 0xFFFF80A4 ***/
typedef union {
  byte Byte;
  struct {
    byte CNTMIN      :1;                                       /* FTM1 Counter is Minimum */
    byte CNTMAX      :1;                                       /* FTM1 Counter is Maximum */
    byte REINIT      :1;                                       /* Reinitialization of FTM1 Counter by Synchronization */
    byte SYNCHOM     :1;                                       /* Output Mask by Synchronization */
    byte TRIG0       :1;                                       /* External Trigger Bit 0 for Synchronization */
    byte TRIG1       :1;                                       /* External Trigger Bit 1 for Synchronization */
    byte TRIG2       :1;                                       /* External Trigger Bit 2 for Synchronization */
    byte SWSYNC      :1;                                       /* Software Trigger for Synchronization */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpTRIG :3;
    byte         :1;
  } MergedBits;
} FTM1SYNCSTR;
extern volatile FTM1SYNCSTR _FTM1SYNC @0xFFFF80A4;
#define FTM1SYNC                        _FTM1SYNC.Byte
#define FTM1SYNC_CNTMIN                 _FTM1SYNC.Bits.CNTMIN
#define FTM1SYNC_CNTMAX                 _FTM1SYNC.Bits.CNTMAX
#define FTM1SYNC_REINIT                 _FTM1SYNC.Bits.REINIT
#define FTM1SYNC_SYNCHOM                _FTM1SYNC.Bits.SYNCHOM
#define FTM1SYNC_TRIG0                  _FTM1SYNC.Bits.TRIG0
#define FTM1SYNC_TRIG1                  _FTM1SYNC.Bits.TRIG1
#define FTM1SYNC_TRIG2                  _FTM1SYNC.Bits.TRIG2
#define FTM1SYNC_SWSYNC                 _FTM1SYNC.Bits.SWSYNC
#define FTM1SYNC_TRIG                   _FTM1SYNC.MergedBits.grpTRIG

#define FTM1SYNC_CNTMIN_MASK            1
#define FTM1SYNC_CNTMAX_MASK            2
#define FTM1SYNC_REINIT_MASK            4
#define FTM1SYNC_SYNCHOM_MASK           8
#define FTM1SYNC_TRIG0_MASK             16
#define FTM1SYNC_TRIG1_MASK             32
#define FTM1SYNC_TRIG2_MASK             64
#define FTM1SYNC_SWSYNC_MASK            128
#define FTM1SYNC_TRIG_MASK              112
#define FTM1SYNC_TRIG_BITNUM            4


/*** FTM1OUTINIT - FTM1 Initial State for Channels Output Register; 0xFFFF80A5 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0OI       :1;                                       /* Initial State for Channel 0 Output */
    byte CH1OI       :1;                                       /* Initial State for Channel 1 Output */
    byte CH2OI       :1;                                       /* Initial State for Channel 2 Output */
    byte CH3OI       :1;                                       /* Initial State for Channel 3 Output */
    byte CH4OI       :1;                                       /* Initial State for Channel 4 Output */
    byte CH5OI       :1;                                       /* Initial State for Channel 5 Output */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM1OUTINITSTR;
extern volatile FTM1OUTINITSTR _FTM1OUTINIT @0xFFFF80A5;
#define FTM1OUTINIT                     _FTM1OUTINIT.Byte
#define FTM1OUTINIT_CH0OI               _FTM1OUTINIT.Bits.CH0OI
#define FTM1OUTINIT_CH1OI               _FTM1OUTINIT.Bits.CH1OI
#define FTM1OUTINIT_CH2OI               _FTM1OUTINIT.Bits.CH2OI
#define FTM1OUTINIT_CH3OI               _FTM1OUTINIT.Bits.CH3OI
#define FTM1OUTINIT_CH4OI               _FTM1OUTINIT.Bits.CH4OI
#define FTM1OUTINIT_CH5OI               _FTM1OUTINIT.Bits.CH5OI

#define FTM1OUTINIT_CH0OI_MASK          1
#define FTM1OUTINIT_CH1OI_MASK          2
#define FTM1OUTINIT_CH2OI_MASK          4
#define FTM1OUTINIT_CH3OI_MASK          8
#define FTM1OUTINIT_CH4OI_MASK          16
#define FTM1OUTINIT_CH5OI_MASK          32


/*** FTM1OUTMASK - FTM1 Output Mask Register; 0xFFFF80A6 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0OM       :1;                                       /* Channel 0 Output Mask */
    byte CH1OM       :1;                                       /* Channel 1 Output Mask */
    byte CH2OM       :1;                                       /* Channel 2 Output Mask */
    byte CH3OM       :1;                                       /* Channel 3 Output Mask */
    byte CH4OM       :1;                                       /* Channel 4 Output Mask */
    byte CH5OM       :1;                                       /* Channel 5 Output Mask */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM1OUTMASKSTR;
extern volatile FTM1OUTMASKSTR _FTM1OUTMASK @0xFFFF80A6;
#define FTM1OUTMASK                     _FTM1OUTMASK.Byte
#define FTM1OUTMASK_CH0OM               _FTM1OUTMASK.Bits.CH0OM
#define FTM1OUTMASK_CH1OM               _FTM1OUTMASK.Bits.CH1OM
#define FTM1OUTMASK_CH2OM               _FTM1OUTMASK.Bits.CH2OM
#define FTM1OUTMASK_CH3OM               _FTM1OUTMASK.Bits.CH3OM
#define FTM1OUTMASK_CH4OM               _FTM1OUTMASK.Bits.CH4OM
#define FTM1OUTMASK_CH5OM               _FTM1OUTMASK.Bits.CH5OM

#define FTM1OUTMASK_CH0OM_MASK          1
#define FTM1OUTMASK_CH1OM_MASK          2
#define FTM1OUTMASK_CH2OM_MASK          4
#define FTM1OUTMASK_CH3OM_MASK          8
#define FTM1OUTMASK_CH4OM_MASK          16
#define FTM1OUTMASK_CH5OM_MASK          32


/*** FTM1COMBINE0 - FTM1 Function For Linked Channel 0 Register; 0xFFFF80A7 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 0 and 1 */
    byte COMP        :1;                                       /* Complementary of Channel 0 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM1COMBINE0STR;
extern volatile FTM1COMBINE0STR _FTM1COMBINE0 @0xFFFF80A7;
#define FTM1COMBINE0                    _FTM1COMBINE0.Byte
#define FTM1COMBINE0_COMBINE            _FTM1COMBINE0.Bits.COMBINE
#define FTM1COMBINE0_COMP               _FTM1COMBINE0.Bits.COMP
#define FTM1COMBINE0_DTEN               _FTM1COMBINE0.Bits.DTEN
#define FTM1COMBINE0_SYNCEN             _FTM1COMBINE0.Bits.SYNCEN
#define FTM1COMBINE0_FAULTEN            _FTM1COMBINE0.Bits.FAULTEN
/* FTM1COMBINE_ARR: Access 3 FTM1COMBINEx registers in an array */
#define FTM1COMBINE_ARR                 ((volatile byte *) &FTM1COMBINE0)

#define FTM1COMBINE0_COMBINE_MASK       1
#define FTM1COMBINE0_COMP_MASK          2
#define FTM1COMBINE0_DTEN_MASK          16
#define FTM1COMBINE0_SYNCEN_MASK        32
#define FTM1COMBINE0_FAULTEN_MASK       64


/*** FTM1COMBINE1 - FTM1 Function For Linked Channel 1 Register; 0xFFFF80A8 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 2 and 3 */
    byte COMP        :1;                                       /* Complementary of Channel 2 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM1COMBINE1STR;
extern volatile FTM1COMBINE1STR _FTM1COMBINE1 @0xFFFF80A8;
#define FTM1COMBINE1                    _FTM1COMBINE1.Byte
#define FTM1COMBINE1_COMBINE            _FTM1COMBINE1.Bits.COMBINE
#define FTM1COMBINE1_COMP               _FTM1COMBINE1.Bits.COMP
#define FTM1COMBINE1_DTEN               _FTM1COMBINE1.Bits.DTEN
#define FTM1COMBINE1_SYNCEN             _FTM1COMBINE1.Bits.SYNCEN
#define FTM1COMBINE1_FAULTEN            _FTM1COMBINE1.Bits.FAULTEN

#define FTM1COMBINE1_COMBINE_MASK       1
#define FTM1COMBINE1_COMP_MASK          2
#define FTM1COMBINE1_DTEN_MASK          16
#define FTM1COMBINE1_SYNCEN_MASK        32
#define FTM1COMBINE1_FAULTEN_MASK       64


/*** FTM1COMBINE2 - FTM1 Function For Linked Channel 2 Register; 0xFFFF80A9 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 4 and 5 */
    byte COMP        :1;                                       /* Complementary of Channel 4 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM1COMBINE2STR;
extern volatile FTM1COMBINE2STR _FTM1COMBINE2 @0xFFFF80A9;
#define FTM1COMBINE2                    _FTM1COMBINE2.Byte
#define FTM1COMBINE2_COMBINE            _FTM1COMBINE2.Bits.COMBINE
#define FTM1COMBINE2_COMP               _FTM1COMBINE2.Bits.COMP
#define FTM1COMBINE2_DTEN               _FTM1COMBINE2.Bits.DTEN
#define FTM1COMBINE2_SYNCEN             _FTM1COMBINE2.Bits.SYNCEN
#define FTM1COMBINE2_FAULTEN            _FTM1COMBINE2.Bits.FAULTEN

#define FTM1COMBINE2_COMBINE_MASK       1
#define FTM1COMBINE2_COMP_MASK          2
#define FTM1COMBINE2_DTEN_MASK          16
#define FTM1COMBINE2_SYNCEN_MASK        32
#define FTM1COMBINE2_FAULTEN_MASK       64


/*** FTM1DEADTIME - FTM1 Deadtime Insertion Control Register; 0xFFFF80AB ***/
typedef union {
  byte Byte;
  struct {
    byte DTVAL0      :1;                                       /* Deadtime Value Bits, bit 0 */
    byte DTVAL1      :1;                                       /* Deadtime Value Bits, bit 1 */
    byte DTVAL2      :1;                                       /* Deadtime Value Bits, bit 2 */
    byte DTVAL3      :1;                                       /* Deadtime Value Bits, bit 3 */
    byte DTVAL4      :1;                                       /* Deadtime Value Bits, bit 4 */
    byte DTVAL5      :1;                                       /* Deadtime Value Bits, bit 5 */
    byte DTPS0       :1;                                       /* Deadtime Prescaler Value Bits, bit 0 */
    byte DTPS1       :1;                                       /* Deadtime Prescaler Value Bits, bit 1 */
  } Bits;
  struct {
    byte grpDTVAL :6;
    byte grpDTPS :2;
  } MergedBits;
} FTM1DEADTIMESTR;
extern volatile FTM1DEADTIMESTR _FTM1DEADTIME @0xFFFF80AB;
#define FTM1DEADTIME                    _FTM1DEADTIME.Byte
#define FTM1DEADTIME_DTVAL0             _FTM1DEADTIME.Bits.DTVAL0
#define FTM1DEADTIME_DTVAL1             _FTM1DEADTIME.Bits.DTVAL1
#define FTM1DEADTIME_DTVAL2             _FTM1DEADTIME.Bits.DTVAL2
#define FTM1DEADTIME_DTVAL3             _FTM1DEADTIME.Bits.DTVAL3
#define FTM1DEADTIME_DTVAL4             _FTM1DEADTIME.Bits.DTVAL4
#define FTM1DEADTIME_DTVAL5             _FTM1DEADTIME.Bits.DTVAL5
#define FTM1DEADTIME_DTPS0              _FTM1DEADTIME.Bits.DTPS0
#define FTM1DEADTIME_DTPS1              _FTM1DEADTIME.Bits.DTPS1
#define FTM1DEADTIME_DTVAL              _FTM1DEADTIME.MergedBits.grpDTVAL
#define FTM1DEADTIME_DTPS               _FTM1DEADTIME.MergedBits.grpDTPS

#define FTM1DEADTIME_DTVAL0_MASK        1
#define FTM1DEADTIME_DTVAL1_MASK        2
#define FTM1DEADTIME_DTVAL2_MASK        4
#define FTM1DEADTIME_DTVAL3_MASK        8
#define FTM1DEADTIME_DTVAL4_MASK        16
#define FTM1DEADTIME_DTVAL5_MASK        32
#define FTM1DEADTIME_DTPS0_MASK         64
#define FTM1DEADTIME_DTPS1_MASK         128
#define FTM1DEADTIME_DTVAL_MASK         63
#define FTM1DEADTIME_DTVAL_BITNUM       0
#define FTM1DEADTIME_DTPS_MASK          192
#define FTM1DEADTIME_DTPS_BITNUM        6


/*** FTM1EXTTRIG - FTM1 ExternalTrigger Register; 0xFFFF80AC ***/
typedef union {
  byte Byte;
  struct {
    byte CH2TRIG     :1;                                       /* Channel 2 trigger enable */
    byte CH3TRIG     :1;                                       /* Channel 3 trigger enable */
    byte CH4TRIG     :1;                                       /* Channel 4 trigger enable */
    byte CH5TRIG     :1;                                       /* Channel 5 trigger enable */
    byte CH0TRIG     :1;                                       /* Channel 0 trigger enable */
    byte CH1TRIG     :1;                                       /* Channel 1 trigger enable */
    byte INITTRIGEN  :1;                                       /* Enables the Generation of a Trigger Signal */
    byte TRIGF       :1;                                       /* Channel trigger flag */
  } Bits;
} FTM1EXTTRIGSTR;
extern volatile FTM1EXTTRIGSTR _FTM1EXTTRIG @0xFFFF80AC;
#define FTM1EXTTRIG                     _FTM1EXTTRIG.Byte
#define FTM1EXTTRIG_CH2TRIG             _FTM1EXTTRIG.Bits.CH2TRIG
#define FTM1EXTTRIG_CH3TRIG             _FTM1EXTTRIG.Bits.CH3TRIG
#define FTM1EXTTRIG_CH4TRIG             _FTM1EXTTRIG.Bits.CH4TRIG
#define FTM1EXTTRIG_CH5TRIG             _FTM1EXTTRIG.Bits.CH5TRIG
#define FTM1EXTTRIG_CH0TRIG             _FTM1EXTTRIG.Bits.CH0TRIG
#define FTM1EXTTRIG_CH1TRIG             _FTM1EXTTRIG.Bits.CH1TRIG
#define FTM1EXTTRIG_INITTRIGEN          _FTM1EXTTRIG.Bits.INITTRIGEN
#define FTM1EXTTRIG_TRIGF               _FTM1EXTTRIG.Bits.TRIGF

#define FTM1EXTTRIG_CH2TRIG_MASK        1
#define FTM1EXTTRIG_CH3TRIG_MASK        2
#define FTM1EXTTRIG_CH4TRIG_MASK        4
#define FTM1EXTTRIG_CH5TRIG_MASK        8
#define FTM1EXTTRIG_CH0TRIG_MASK        16
#define FTM1EXTTRIG_CH1TRIG_MASK        32
#define FTM1EXTTRIG_INITTRIGEN_MASK     64
#define FTM1EXTTRIG_TRIGF_MASK          128


/*** FTM1POL - FTM1 Channels Polarity Register; 0xFFFF80AD ***/
typedef union {
  byte Byte;
  struct {
    byte POL0        :1;                                       /* Channel 0 Polarity */
    byte POL1        :1;                                       /* Channel 1 Polarity */
    byte POL2        :1;                                       /* Channel 2 Polarity */
    byte POL3        :1;                                       /* Channel 3 Polarity */
    byte POL4        :1;                                       /* Channel 4 Polarity */
    byte POL5        :1;                                       /* Channel 5 Polarity */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPOL  :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1POLSTR;
extern volatile FTM1POLSTR _FTM1POL @0xFFFF80AD;
#define FTM1POL                         _FTM1POL.Byte
#define FTM1POL_POL0                    _FTM1POL.Bits.POL0
#define FTM1POL_POL1                    _FTM1POL.Bits.POL1
#define FTM1POL_POL2                    _FTM1POL.Bits.POL2
#define FTM1POL_POL3                    _FTM1POL.Bits.POL3
#define FTM1POL_POL4                    _FTM1POL.Bits.POL4
#define FTM1POL_POL5                    _FTM1POL.Bits.POL5
#define FTM1POL_POL                     _FTM1POL.MergedBits.grpPOL

#define FTM1POL_POL0_MASK               1
#define FTM1POL_POL1_MASK               2
#define FTM1POL_POL2_MASK               4
#define FTM1POL_POL3_MASK               8
#define FTM1POL_POL4_MASK               16
#define FTM1POL_POL5_MASK               32
#define FTM1POL_POL_MASK                63
#define FTM1POL_POL_BITNUM              0


/*** FTM1FMS - FTM1 Fault Mode Status Register; 0xFFFF80AE ***/
typedef union {
  byte Byte;
  struct {
    byte FAULTF0     :1;                                       /* Fault detection flag 0 */
    byte FAULTF1     :1;                                       /* Fault detection flag 1 */
    byte FAULTF2     :1;                                       /* Fault detection flag 2 */
    byte FAULTF3     :1;                                       /* Fault detection flag 3 */
    byte             :1; 
    byte FAULTIN     :1;                                       /* Fault Input */
    byte WPEN        :1;                                       /* Write Protected Enable */
    byte FAULTF      :1;                                       /* Fault Detection Flag */
  } Bits;
  struct {
    byte         :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1FMSSTR;
extern volatile FTM1FMSSTR _FTM1FMS @0xFFFF80AE;
#define FTM1FMS                         _FTM1FMS.Byte
#define FTM1FMS_FAULTF0                 _FTM1FMS.Bits.FAULTF0
#define FTM1FMS_FAULTF1                 _FTM1FMS.Bits.FAULTF1
#define FTM1FMS_FAULTF2                 _FTM1FMS.Bits.FAULTF2
#define FTM1FMS_FAULTF3                 _FTM1FMS.Bits.FAULTF3
#define FTM1FMS_FAULTIN                 _FTM1FMS.Bits.FAULTIN
#define FTM1FMS_WPEN                    _FTM1FMS.Bits.WPEN
#define FTM1FMS_FAULTF                  _FTM1FMS.Bits.FAULTF

#define FTM1FMS_FAULTF0_MASK            1
#define FTM1FMS_FAULTF1_MASK            2
#define FTM1FMS_FAULTF2_MASK            4
#define FTM1FMS_FAULTF3_MASK            8
#define FTM1FMS_FAULTIN_MASK            32
#define FTM1FMS_WPEN_MASK               64
#define FTM1FMS_FAULTF_MASK             128


/*** FTM1FILTER0 - FTM1 Input Capture Filter Control Register 0; 0xFFFF80AF ***/
typedef union {
  byte Byte;
  struct {
    byte CH0FVAL0    :1;                                       /* Channel 0 Input Filter Bits, bit 0 */
    byte CH0FVAL1    :1;                                       /* Channel 0 Input Filter Bits, bit 1 */
    byte CH0FVAL2    :1;                                       /* Channel 0 Input Filter Bits, bit 2 */
    byte CH0FVAL3    :1;                                       /* Channel 0 Input Filter Bits, bit 3 */
    byte CH1FVAL0    :1;                                       /* Channel 1 Input Filter Bits, bit 0 */
    byte CH1FVAL1    :1;                                       /* Channel 1 Input Filter Bits, bit 1 */
    byte CH1FVAL2    :1;                                       /* Channel 1 Input Filter Bits, bit 2 */
    byte CH1FVAL3    :1;                                       /* Channel 1 Input Filter Bits, bit 3 */
  } Bits;
  struct {
    byte grpCH0FVAL :4;
    byte grpCH1FVAL :4;
  } MergedBits;
} FTM1FILTER0STR;
extern volatile FTM1FILTER0STR _FTM1FILTER0 @0xFFFF80AF;
#define FTM1FILTER0                     _FTM1FILTER0.Byte
#define FTM1FILTER0_CH0FVAL0            _FTM1FILTER0.Bits.CH0FVAL0
#define FTM1FILTER0_CH0FVAL1            _FTM1FILTER0.Bits.CH0FVAL1
#define FTM1FILTER0_CH0FVAL2            _FTM1FILTER0.Bits.CH0FVAL2
#define FTM1FILTER0_CH0FVAL3            _FTM1FILTER0.Bits.CH0FVAL3
#define FTM1FILTER0_CH1FVAL0            _FTM1FILTER0.Bits.CH1FVAL0
#define FTM1FILTER0_CH1FVAL1            _FTM1FILTER0.Bits.CH1FVAL1
#define FTM1FILTER0_CH1FVAL2            _FTM1FILTER0.Bits.CH1FVAL2
#define FTM1FILTER0_CH1FVAL3            _FTM1FILTER0.Bits.CH1FVAL3
/* FTM1FILTER_ARR: Access 2 FTM1FILTERx registers in an array */
#define FTM1FILTER_ARR                  ((volatile byte *) &FTM1FILTER0)
#define FTM1FILTER0_CH0FVAL             _FTM1FILTER0.MergedBits.grpCH0FVAL
#define FTM1FILTER0_CH1FVAL             _FTM1FILTER0.MergedBits.grpCH1FVAL

#define FTM1FILTER0_CH0FVAL0_MASK       1
#define FTM1FILTER0_CH0FVAL1_MASK       2
#define FTM1FILTER0_CH0FVAL2_MASK       4
#define FTM1FILTER0_CH0FVAL3_MASK       8
#define FTM1FILTER0_CH1FVAL0_MASK       16
#define FTM1FILTER0_CH1FVAL1_MASK       32
#define FTM1FILTER0_CH1FVAL2_MASK       64
#define FTM1FILTER0_CH1FVAL3_MASK       128
#define FTM1FILTER0_CH0FVAL_MASK        15
#define FTM1FILTER0_CH0FVAL_BITNUM      0
#define FTM1FILTER0_CH1FVAL_MASK        240
#define FTM1FILTER0_CH1FVAL_BITNUM      4


/*** FTM1FILTER1 - FTM1 Input Capture Filter Control Register 1; 0xFFFF80B0 ***/
typedef union {
  byte Byte;
  struct {
    byte CH2FVAL0    :1;                                       /* Channel 2 Input Filter Bits, bit 0 */
    byte CH2FVAL1    :1;                                       /* Channel 2 Input Filter Bits, bit 1 */
    byte CH2FVAL2    :1;                                       /* Channel 2 Input Filter Bits, bit 2 */
    byte CH2FVAL3    :1;                                       /* Channel 2 Input Filter Bits, bit 3 */
    byte CH3FVAL0    :1;                                       /* Channel 3 Input Filter Bits, bit 0 */
    byte CH3FVAL1    :1;                                       /* Channel 3 Input Filter Bits, bit 1 */
    byte CH3FVAL2    :1;                                       /* Channel 3 Input Filter Bits, bit 2 */
    byte CH3FVAL3    :1;                                       /* Channel 3 Input Filter Bits, bit 3 */
  } Bits;
  struct {
    byte grpCH2FVAL :4;
    byte grpCH3FVAL :4;
  } MergedBits;
} FTM1FILTER1STR;
extern volatile FTM1FILTER1STR _FTM1FILTER1 @0xFFFF80B0;
#define FTM1FILTER1                     _FTM1FILTER1.Byte
#define FTM1FILTER1_CH2FVAL0            _FTM1FILTER1.Bits.CH2FVAL0
#define FTM1FILTER1_CH2FVAL1            _FTM1FILTER1.Bits.CH2FVAL1
#define FTM1FILTER1_CH2FVAL2            _FTM1FILTER1.Bits.CH2FVAL2
#define FTM1FILTER1_CH2FVAL3            _FTM1FILTER1.Bits.CH2FVAL3
#define FTM1FILTER1_CH3FVAL0            _FTM1FILTER1.Bits.CH3FVAL0
#define FTM1FILTER1_CH3FVAL1            _FTM1FILTER1.Bits.CH3FVAL1
#define FTM1FILTER1_CH3FVAL2            _FTM1FILTER1.Bits.CH3FVAL2
#define FTM1FILTER1_CH3FVAL3            _FTM1FILTER1.Bits.CH3FVAL3
#define FTM1FILTER1_CH2FVAL             _FTM1FILTER1.MergedBits.grpCH2FVAL
#define FTM1FILTER1_CH3FVAL             _FTM1FILTER1.MergedBits.grpCH3FVAL

#define FTM1FILTER1_CH2FVAL0_MASK       1
#define FTM1FILTER1_CH2FVAL1_MASK       2
#define FTM1FILTER1_CH2FVAL2_MASK       4
#define FTM1FILTER1_CH2FVAL3_MASK       8
#define FTM1FILTER1_CH3FVAL0_MASK       16
#define FTM1FILTER1_CH3FVAL1_MASK       32
#define FTM1FILTER1_CH3FVAL2_MASK       64
#define FTM1FILTER1_CH3FVAL3_MASK       128
#define FTM1FILTER1_CH2FVAL_MASK        15
#define FTM1FILTER1_CH2FVAL_BITNUM      0
#define FTM1FILTER1_CH3FVAL_MASK        240
#define FTM1FILTER1_CH3FVAL_BITNUM      4


/*** FTM1FLTFILTER - FTM1 Fault Input Filter Control Register; 0xFFFF80B1 ***/
typedef union {
  byte Byte;
  struct {
    byte FFVAL0      :1;                                       /* Fault Input Filter Bits, bit 0 */
    byte FFVAL1      :1;                                       /* Fault Input Filter Bits, bit 1 */
    byte FFVAL2      :1;                                       /* Fault Input Filter Bits, bit 2 */
    byte FFVAL3      :1;                                       /* Fault Input Filter Bits, bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpFFVAL :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM1FLTFILTERSTR;
extern volatile FTM1FLTFILTERSTR _FTM1FLTFILTER @0xFFFF80B1;
#define FTM1FLTFILTER                   _FTM1FLTFILTER.Byte
#define FTM1FLTFILTER_FFVAL0            _FTM1FLTFILTER.Bits.FFVAL0
#define FTM1FLTFILTER_FFVAL1            _FTM1FLTFILTER.Bits.FFVAL1
#define FTM1FLTFILTER_FFVAL2            _FTM1FLTFILTER.Bits.FFVAL2
#define FTM1FLTFILTER_FFVAL3            _FTM1FLTFILTER.Bits.FFVAL3
#define FTM1FLTFILTER_FFVAL             _FTM1FLTFILTER.MergedBits.grpFFVAL

#define FTM1FLTFILTER_FFVAL0_MASK       1
#define FTM1FLTFILTER_FFVAL1_MASK       2
#define FTM1FLTFILTER_FFVAL2_MASK       4
#define FTM1FLTFILTER_FFVAL3_MASK       8
#define FTM1FLTFILTER_FFVAL_MASK        15
#define FTM1FLTFILTER_FFVAL_BITNUM      0


/*** FTM1FLTCTRL - FTM1 Fault Control Register; 0xFFFF80B2 ***/
typedef union {
  byte Byte;
  struct {
    byte FAULT0EN    :1;                                       /* Fault input 0 enable */
    byte FAULT1EN    :1;                                       /* Fault input 1 enable */
    byte             :1; 
    byte             :1; 
    byte FFLTR0EN    :1;                                       /* Fault input 0 filter enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM1FLTCTRLSTR;
extern volatile FTM1FLTCTRLSTR _FTM1FLTCTRL @0xFFFF80B2;
#define FTM1FLTCTRL                     _FTM1FLTCTRL.Byte
#define FTM1FLTCTRL_FAULT0EN            _FTM1FLTCTRL.Bits.FAULT0EN
#define FTM1FLTCTRL_FAULT1EN            _FTM1FLTCTRL.Bits.FAULT1EN
#define FTM1FLTCTRL_FFLTR0EN            _FTM1FLTCTRL.Bits.FFLTR0EN

#define FTM1FLTCTRL_FAULT0EN_MASK       1
#define FTM1FLTCTRL_FAULT1EN_MASK       2
#define FTM1FLTCTRL_FFLTR0EN_MASK       16


/*** FTM2SC - FTM2 Status and Control Register; 0xFFFF80C0 ***/
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
} FTM2SCSTR;
extern volatile FTM2SCSTR _FTM2SC @0xFFFF80C0;
#define FTM2SC                          _FTM2SC.Byte
#define FTM2SC_PS0                      _FTM2SC.Bits.PS0
#define FTM2SC_PS1                      _FTM2SC.Bits.PS1
#define FTM2SC_PS2                      _FTM2SC.Bits.PS2
#define FTM2SC_CLKSA                    _FTM2SC.Bits.CLKSA
#define FTM2SC_CLKSB                    _FTM2SC.Bits.CLKSB
#define FTM2SC_CPWMS                    _FTM2SC.Bits.CPWMS
#define FTM2SC_TOIE                     _FTM2SC.Bits.TOIE
#define FTM2SC_TOF                      _FTM2SC.Bits.TOF
#define FTM2SC_PS                       _FTM2SC.MergedBits.grpPS
#define FTM2SC_CLKSx                    _FTM2SC.MergedBits.grpCLKSx

#define FTM2SC_PS0_MASK                 1
#define FTM2SC_PS1_MASK                 2
#define FTM2SC_PS2_MASK                 4
#define FTM2SC_CLKSA_MASK               8
#define FTM2SC_CLKSB_MASK               16
#define FTM2SC_CPWMS_MASK               32
#define FTM2SC_TOIE_MASK                64
#define FTM2SC_TOF_MASK                 128
#define FTM2SC_PS_MASK                  7
#define FTM2SC_PS_BITNUM                0
#define FTM2SC_CLKSx_MASK               24
#define FTM2SC_CLKSx_BITNUM             3


/*** FTM2CNT - FTM2 Timer Counter Register; 0xFFFF80C1 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2CNTH - FTM2 Timer Counter Register High; 0xFFFF80C1 ***/
    union {
      byte Byte;
    } FTM2CNTHSTR;
    #define FTM2CNTH                    _FTM2CNT.Overlap_STR.FTM2CNTHSTR.Byte
    

    /*** FTM2CNTL - FTM2 Timer Counter Register Low; 0xFFFF80C2 ***/
    union {
      byte Byte;
    } FTM2CNTLSTR;
    #define FTM2CNTL                    _FTM2CNT.Overlap_STR.FTM2CNTLSTR.Byte
    
  } Overlap_STR;

} FTM2CNTSTR;
extern volatile FTM2CNTSTR _FTM2CNT @0xFFFF80C1;
#define FTM2CNT                         _FTM2CNT.Word


/*** FTM2MOD - FTM2 Timer Counter Modulo Register; 0xFFFF80C3 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2MODH - FTM2 Timer Counter Modulo Register High; 0xFFFF80C3 ***/
    union {
      byte Byte;
    } FTM2MODHSTR;
    #define FTM2MODH                    _FTM2MOD.Overlap_STR.FTM2MODHSTR.Byte
    

    /*** FTM2MODL - FTM2 Timer Counter Modulo Register Low; 0xFFFF80C4 ***/
    union {
      byte Byte;
    } FTM2MODLSTR;
    #define FTM2MODL                    _FTM2MOD.Overlap_STR.FTM2MODLSTR.Byte
    
  } Overlap_STR;

} FTM2MODSTR;
extern volatile FTM2MODSTR _FTM2MOD @0xFFFF80C3;
#define FTM2MOD                         _FTM2MOD.Word


/*** FTM2C0SC - FTM2 Timer Channel 0 Status and Control Register; 0xFFFF80C5 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH0F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for FTM2 Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for FTM2 Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C0SCSTR;
extern volatile FTM2C0SCSTR _FTM2C0SC @0xFFFF80C5;
#define FTM2C0SC                        _FTM2C0SC.Byte
#define FTM2C0SC_DMA                    _FTM2C0SC.Bits.DMA
#define FTM2C0SC_ELS0A                  _FTM2C0SC.Bits.ELS0A
#define FTM2C0SC_ELS0B                  _FTM2C0SC.Bits.ELS0B
#define FTM2C0SC_MS0A                   _FTM2C0SC.Bits.MS0A
#define FTM2C0SC_MS0B                   _FTM2C0SC.Bits.MS0B
#define FTM2C0SC_CH0IE                  _FTM2C0SC.Bits.CH0IE
#define FTM2C0SC_CH0F                   _FTM2C0SC.Bits.CH0F
#define FTM2C0SC_ELS0x                  _FTM2C0SC.MergedBits.grpELS0x
#define FTM2C0SC_MS0x                   _FTM2C0SC.MergedBits.grpMS0x

#define FTM2C0SC_DMA_MASK               1
#define FTM2C0SC_ELS0A_MASK             4
#define FTM2C0SC_ELS0B_MASK             8
#define FTM2C0SC_MS0A_MASK              16
#define FTM2C0SC_MS0B_MASK              32
#define FTM2C0SC_CH0IE_MASK             64
#define FTM2C0SC_CH0F_MASK              128
#define FTM2C0SC_ELS0x_MASK             12
#define FTM2C0SC_ELS0x_BITNUM           2
#define FTM2C0SC_MS0x_MASK              48
#define FTM2C0SC_MS0x_BITNUM            4


/*** FTM2C0V - FTM2 Timer Channel 0 Value Register; 0xFFFF80C6 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C0VH - FTM2 Timer Channel 0 Value Register High; 0xFFFF80C6 ***/
    union {
      byte Byte;
    } FTM2C0VHSTR;
    #define FTM2C0VH                    _FTM2C0V.Overlap_STR.FTM2C0VHSTR.Byte
    

    /*** FTM2C0VL - FTM2 Timer Channel 0 Value Register Low; 0xFFFF80C7 ***/
    union {
      byte Byte;
    } FTM2C0VLSTR;
    #define FTM2C0VL                    _FTM2C0V.Overlap_STR.FTM2C0VLSTR.Byte
    
  } Overlap_STR;

} FTM2C0VSTR;
extern volatile FTM2C0VSTR _FTM2C0V @0xFFFF80C6;
#define FTM2C0V                         _FTM2C0V.Word


/*** FTM2C1SC - FTM2 Timer Channel 1 Status and Control Register; 0xFFFF80C8 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH1F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for FTM2 Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for FTM2 Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C1SCSTR;
extern volatile FTM2C1SCSTR _FTM2C1SC @0xFFFF80C8;
#define FTM2C1SC                        _FTM2C1SC.Byte
#define FTM2C1SC_DMA                    _FTM2C1SC.Bits.DMA
#define FTM2C1SC_ELS1A                  _FTM2C1SC.Bits.ELS1A
#define FTM2C1SC_ELS1B                  _FTM2C1SC.Bits.ELS1B
#define FTM2C1SC_MS1A                   _FTM2C1SC.Bits.MS1A
#define FTM2C1SC_MS1B                   _FTM2C1SC.Bits.MS1B
#define FTM2C1SC_CH1IE                  _FTM2C1SC.Bits.CH1IE
#define FTM2C1SC_CH1F                   _FTM2C1SC.Bits.CH1F
#define FTM2C1SC_ELS1x                  _FTM2C1SC.MergedBits.grpELS1x
#define FTM2C1SC_MS1x                   _FTM2C1SC.MergedBits.grpMS1x

#define FTM2C1SC_DMA_MASK               1
#define FTM2C1SC_ELS1A_MASK             4
#define FTM2C1SC_ELS1B_MASK             8
#define FTM2C1SC_MS1A_MASK              16
#define FTM2C1SC_MS1B_MASK              32
#define FTM2C1SC_CH1IE_MASK             64
#define FTM2C1SC_CH1F_MASK              128
#define FTM2C1SC_ELS1x_MASK             12
#define FTM2C1SC_ELS1x_BITNUM           2
#define FTM2C1SC_MS1x_MASK              48
#define FTM2C1SC_MS1x_BITNUM            4


/*** FTM2C1V - FTM2 Timer Channel 1 Value Register; 0xFFFF80C9 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C1VH - FTM2 Timer Channel 1 Value Register High; 0xFFFF80C9 ***/
    union {
      byte Byte;
    } FTM2C1VHSTR;
    #define FTM2C1VH                    _FTM2C1V.Overlap_STR.FTM2C1VHSTR.Byte
    

    /*** FTM2C1VL - FTM2 Timer Channel 1 Value Register Low; 0xFFFF80CA ***/
    union {
      byte Byte;
    } FTM2C1VLSTR;
    #define FTM2C1VL                    _FTM2C1V.Overlap_STR.FTM2C1VLSTR.Byte
    
  } Overlap_STR;

} FTM2C1VSTR;
extern volatile FTM2C1VSTR _FTM2C1V @0xFFFF80C9;
#define FTM2C1V                         _FTM2C1V.Word


/*** FTM2C2SC - FTM2 Timer Channel 2 Status and Control Register; 0xFFFF80CB ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH2F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS2A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS2B       :1;                                       /* Edge/Level Select Bit B */
    byte MS2A        :1;                                       /* Mode Select A for FTM2 Channel 2 */
    byte MS2B        :1;                                       /* Mode Select B for FTM2 Channel 2 */
    byte CH2IE       :1;                                       /* Channel 2 Interrupt Enable */
    byte CH2F        :1;                                       /* Channel 2 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS2x :2;
    byte grpMS2x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C2SCSTR;
extern volatile FTM2C2SCSTR _FTM2C2SC @0xFFFF80CB;
#define FTM2C2SC                        _FTM2C2SC.Byte
#define FTM2C2SC_DMA                    _FTM2C2SC.Bits.DMA
#define FTM2C2SC_ELS2A                  _FTM2C2SC.Bits.ELS2A
#define FTM2C2SC_ELS2B                  _FTM2C2SC.Bits.ELS2B
#define FTM2C2SC_MS2A                   _FTM2C2SC.Bits.MS2A
#define FTM2C2SC_MS2B                   _FTM2C2SC.Bits.MS2B
#define FTM2C2SC_CH2IE                  _FTM2C2SC.Bits.CH2IE
#define FTM2C2SC_CH2F                   _FTM2C2SC.Bits.CH2F
#define FTM2C2SC_ELS2x                  _FTM2C2SC.MergedBits.grpELS2x
#define FTM2C2SC_MS2x                   _FTM2C2SC.MergedBits.grpMS2x

#define FTM2C2SC_DMA_MASK               1
#define FTM2C2SC_ELS2A_MASK             4
#define FTM2C2SC_ELS2B_MASK             8
#define FTM2C2SC_MS2A_MASK              16
#define FTM2C2SC_MS2B_MASK              32
#define FTM2C2SC_CH2IE_MASK             64
#define FTM2C2SC_CH2F_MASK              128
#define FTM2C2SC_ELS2x_MASK             12
#define FTM2C2SC_ELS2x_BITNUM           2
#define FTM2C2SC_MS2x_MASK              48
#define FTM2C2SC_MS2x_BITNUM            4


/*** FTM2C2V - FTM2 Timer Channel 2 Value Register; 0xFFFF80CC ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C2VH - FTM2 Timer Channel 2 Value Register High; 0xFFFF80CC ***/
    union {
      byte Byte;
    } FTM2C2VHSTR;
    #define FTM2C2VH                    _FTM2C2V.Overlap_STR.FTM2C2VHSTR.Byte
    

    /*** FTM2C2VL - FTM2 Timer Channel 2 Value Register Low; 0xFFFF80CD ***/
    union {
      byte Byte;
    } FTM2C2VLSTR;
    #define FTM2C2VL                    _FTM2C2V.Overlap_STR.FTM2C2VLSTR.Byte
    
  } Overlap_STR;

} FTM2C2VSTR;
extern volatile FTM2C2VSTR _FTM2C2V @0xFFFF80CC;
#define FTM2C2V                         _FTM2C2V.Word


/*** FTM2C3SC - FTM2 Timer Channel 3 Status and Control Register; 0xFFFF80CE ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH3F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS3A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS3B       :1;                                       /* Edge/Level Select Bit B */
    byte MS3A        :1;                                       /* Mode Select A for FTM2 Channel 3 */
    byte MS3B        :1;                                       /* Mode Select B for FTM2 Channel 3 */
    byte CH3IE       :1;                                       /* Channel 3 Interrupt Enable */
    byte CH3F        :1;                                       /* Channel 3 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS3x :2;
    byte grpMS3x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C3SCSTR;
extern volatile FTM2C3SCSTR _FTM2C3SC @0xFFFF80CE;
#define FTM2C3SC                        _FTM2C3SC.Byte
#define FTM2C3SC_DMA                    _FTM2C3SC.Bits.DMA
#define FTM2C3SC_ELS3A                  _FTM2C3SC.Bits.ELS3A
#define FTM2C3SC_ELS3B                  _FTM2C3SC.Bits.ELS3B
#define FTM2C3SC_MS3A                   _FTM2C3SC.Bits.MS3A
#define FTM2C3SC_MS3B                   _FTM2C3SC.Bits.MS3B
#define FTM2C3SC_CH3IE                  _FTM2C3SC.Bits.CH3IE
#define FTM2C3SC_CH3F                   _FTM2C3SC.Bits.CH3F
#define FTM2C3SC_ELS3x                  _FTM2C3SC.MergedBits.grpELS3x
#define FTM2C3SC_MS3x                   _FTM2C3SC.MergedBits.grpMS3x

#define FTM2C3SC_DMA_MASK               1
#define FTM2C3SC_ELS3A_MASK             4
#define FTM2C3SC_ELS3B_MASK             8
#define FTM2C3SC_MS3A_MASK              16
#define FTM2C3SC_MS3B_MASK              32
#define FTM2C3SC_CH3IE_MASK             64
#define FTM2C3SC_CH3F_MASK              128
#define FTM2C3SC_ELS3x_MASK             12
#define FTM2C3SC_ELS3x_BITNUM           2
#define FTM2C3SC_MS3x_MASK              48
#define FTM2C3SC_MS3x_BITNUM            4


/*** FTM2C3V - FTM2 Timer Channel 3 Value Register; 0xFFFF80CF ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C3VH - FTM2 Timer Channel 3 Value Register High; 0xFFFF80CF ***/
    union {
      byte Byte;
    } FTM2C3VHSTR;
    #define FTM2C3VH                    _FTM2C3V.Overlap_STR.FTM2C3VHSTR.Byte
    

    /*** FTM2C3VL - FTM2 Timer Channel 3 Value Register Low; 0xFFFF80D0 ***/
    union {
      byte Byte;
    } FTM2C3VLSTR;
    #define FTM2C3VL                    _FTM2C3V.Overlap_STR.FTM2C3VLSTR.Byte
    
  } Overlap_STR;

} FTM2C3VSTR;
extern volatile FTM2C3VSTR _FTM2C3V @0xFFFF80CF;
#define FTM2C3V                         _FTM2C3V.Word


/*** FTM2C4SC - FTM2 Timer Channel 4 Status and Control Register; 0xFFFF80D1 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH4F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS4A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS4B       :1;                                       /* Edge/Level Select Bit B */
    byte MS4A        :1;                                       /* Mode Select A for FTM2 Channel 4 */
    byte MS4B        :1;                                       /* Mode Select B for FTM2 Channel 4 */
    byte CH4IE       :1;                                       /* Channel 4 Interrupt Enable */
    byte CH4F        :1;                                       /* Channel 4 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS4x :2;
    byte grpMS4x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C4SCSTR;
extern volatile FTM2C4SCSTR _FTM2C4SC @0xFFFF80D1;
#define FTM2C4SC                        _FTM2C4SC.Byte
#define FTM2C4SC_DMA                    _FTM2C4SC.Bits.DMA
#define FTM2C4SC_ELS4A                  _FTM2C4SC.Bits.ELS4A
#define FTM2C4SC_ELS4B                  _FTM2C4SC.Bits.ELS4B
#define FTM2C4SC_MS4A                   _FTM2C4SC.Bits.MS4A
#define FTM2C4SC_MS4B                   _FTM2C4SC.Bits.MS4B
#define FTM2C4SC_CH4IE                  _FTM2C4SC.Bits.CH4IE
#define FTM2C4SC_CH4F                   _FTM2C4SC.Bits.CH4F
#define FTM2C4SC_ELS4x                  _FTM2C4SC.MergedBits.grpELS4x
#define FTM2C4SC_MS4x                   _FTM2C4SC.MergedBits.grpMS4x

#define FTM2C4SC_DMA_MASK               1
#define FTM2C4SC_ELS4A_MASK             4
#define FTM2C4SC_ELS4B_MASK             8
#define FTM2C4SC_MS4A_MASK              16
#define FTM2C4SC_MS4B_MASK              32
#define FTM2C4SC_CH4IE_MASK             64
#define FTM2C4SC_CH4F_MASK              128
#define FTM2C4SC_ELS4x_MASK             12
#define FTM2C4SC_ELS4x_BITNUM           2
#define FTM2C4SC_MS4x_MASK              48
#define FTM2C4SC_MS4x_BITNUM            4


/*** FTM2C4V - FTM2 Timer Channel 4 Value Register; 0xFFFF80D2 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C4VH - FTM2 Timer Channel 4 Value Register High; 0xFFFF80D2 ***/
    union {
      byte Byte;
    } FTM2C4VHSTR;
    #define FTM2C4VH                    _FTM2C4V.Overlap_STR.FTM2C4VHSTR.Byte
    

    /*** FTM2C4VL - FTM2 Timer Channel 4 Value Register Low; 0xFFFF80D3 ***/
    union {
      byte Byte;
    } FTM2C4VLSTR;
    #define FTM2C4VL                    _FTM2C4V.Overlap_STR.FTM2C4VLSTR.Byte
    
  } Overlap_STR;

} FTM2C4VSTR;
extern volatile FTM2C4VSTR _FTM2C4V @0xFFFF80D2;
#define FTM2C4V                         _FTM2C4V.Word


/*** FTM2C5SC - FTM2 Timer Channel 5 Status and Control Register; 0xFFFF80D4 ***/
typedef union {
  byte Byte;
  struct {
    byte DMA         :1;                                       /* The DMA bit enables DMA requests on CH5F flag being set rather than issuing a CPU interrupt request */
    byte             :1; 
    byte ELS5A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS5B       :1;                                       /* Edge/Level Select Bit B */
    byte MS5A        :1;                                       /* Mode Select A for FTM2 Channel 5 */
    byte MS5B        :1;                                       /* Mode Select B for FTM2 Channel 5 */
    byte CH5IE       :1;                                       /* Channel 5 Interrupt Enable */
    byte CH5F        :1;                                       /* Channel 5 Flag */
  } Bits;
  struct {
    byte grpDMx  :1;
    byte         :1;
    byte grpELS5x :2;
    byte grpMS5x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2C5SCSTR;
extern volatile FTM2C5SCSTR _FTM2C5SC @0xFFFF80D4;
#define FTM2C5SC                        _FTM2C5SC.Byte
#define FTM2C5SC_DMA                    _FTM2C5SC.Bits.DMA
#define FTM2C5SC_ELS5A                  _FTM2C5SC.Bits.ELS5A
#define FTM2C5SC_ELS5B                  _FTM2C5SC.Bits.ELS5B
#define FTM2C5SC_MS5A                   _FTM2C5SC.Bits.MS5A
#define FTM2C5SC_MS5B                   _FTM2C5SC.Bits.MS5B
#define FTM2C5SC_CH5IE                  _FTM2C5SC.Bits.CH5IE
#define FTM2C5SC_CH5F                   _FTM2C5SC.Bits.CH5F
#define FTM2C5SC_ELS5x                  _FTM2C5SC.MergedBits.grpELS5x
#define FTM2C5SC_MS5x                   _FTM2C5SC.MergedBits.grpMS5x

#define FTM2C5SC_DMA_MASK               1
#define FTM2C5SC_ELS5A_MASK             4
#define FTM2C5SC_ELS5B_MASK             8
#define FTM2C5SC_MS5A_MASK              16
#define FTM2C5SC_MS5B_MASK              32
#define FTM2C5SC_CH5IE_MASK             64
#define FTM2C5SC_CH5F_MASK              128
#define FTM2C5SC_ELS5x_MASK             12
#define FTM2C5SC_ELS5x_BITNUM           2
#define FTM2C5SC_MS5x_MASK              48
#define FTM2C5SC_MS5x_BITNUM            4


/*** FTM2C5V - FTM2 Timer Channel 5 Value Register; 0xFFFF80D5 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2C5VH - FTM2 Timer Channel 5 Value Register High; 0xFFFF80D5 ***/
    union {
      byte Byte;
    } FTM2C5VHSTR;
    #define FTM2C5VH                    _FTM2C5V.Overlap_STR.FTM2C5VHSTR.Byte
    

    /*** FTM2C5VL - FTM2 Timer Channel 5 Value Register Low; 0xFFFF80D6 ***/
    union {
      byte Byte;
    } FTM2C5VLSTR;
    #define FTM2C5VL                    _FTM2C5V.Overlap_STR.FTM2C5VLSTR.Byte
    
  } Overlap_STR;

} FTM2C5VSTR;
extern volatile FTM2C5VSTR _FTM2C5V @0xFFFF80D5;
#define FTM2C5V                         _FTM2C5V.Word


/*** FTM2CNTIN - FTM2 Counter Initial Value Registers; 0xFFFF80E0 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** FTM2CNTINH - FTM2 Counter Initial Value Registers High; 0xFFFF80E0 ***/
    union {
      byte Byte;
      struct {
        byte BIT8        :1;                                       /* FTM2 Counter Initial Value Bit 8 */
        byte BIT9        :1;                                       /* FTM2 Counter Initial Value Bit 9 */
        byte BIT10       :1;                                       /* FTM2 Counter Initial Value Bit 10 */
        byte BIT11       :1;                                       /* FTM2 Counter Initial Value Bit 11 */
        byte BIT12       :1;                                       /* FTM2 Counter Initial Value Bit 12 */
        byte BIT13       :1;                                       /* FTM2 Counter Initial Value Bit 13 */
        byte BIT14       :1;                                       /* FTM2 Counter Initial Value Bit 14 */
        byte BIT15       :1;                                       /* FTM2 Counter Initial Value Bit 15 */
      } Bits;
    } FTM2CNTINHSTR;
    #define FTM2CNTINH                  _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Byte
    #define FTM2CNTINH_BIT8             _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT8
    #define FTM2CNTINH_BIT9             _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT9
    #define FTM2CNTINH_BIT10            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT10
    #define FTM2CNTINH_BIT11            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT11
    #define FTM2CNTINH_BIT12            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT12
    #define FTM2CNTINH_BIT13            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT13
    #define FTM2CNTINH_BIT14            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT14
    #define FTM2CNTINH_BIT15            _FTM2CNTIN.Overlap_STR.FTM2CNTINHSTR.Bits.BIT15
    
    #define FTM2CNTINH_BIT8_MASK        1
    #define FTM2CNTINH_BIT9_MASK        2
    #define FTM2CNTINH_BIT10_MASK       4
    #define FTM2CNTINH_BIT11_MASK       8
    #define FTM2CNTINH_BIT12_MASK       16
    #define FTM2CNTINH_BIT13_MASK       32
    #define FTM2CNTINH_BIT14_MASK       64
    #define FTM2CNTINH_BIT15_MASK       128
    

    /*** FTM2CNTINL - FTM2 Counter Initial Value Registers Low; 0xFFFF80E1 ***/
    union {
      byte Byte;
      struct {
        byte BIT0        :1;                                       /* FTM2 Counter Initial Value Bit 0 */
        byte BIT1        :1;                                       /* FTM2 Counter Initial Value Bit 1 */
        byte BIT2        :1;                                       /* FTM2 Counter Initial Value Bit 2 */
        byte BIT3        :1;                                       /* FTM2 Counter Initial Value Bit 3 */
        byte BIT4        :1;                                       /* FTM2 Counter Initial Value Bit 4 */
        byte BIT5        :1;                                       /* FTM2 Counter Initial Value Bit 5 */
        byte BIT6        :1;                                       /* FTM2 Counter Initial Value Bit 6 */
        byte BIT7        :1;                                       /* FTM2 Counter Initial Value Bit 7 */
      } Bits;
    } FTM2CNTINLSTR;
    #define FTM2CNTINL                  _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Byte
    #define FTM2CNTINL_BIT0             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT0
    #define FTM2CNTINL_BIT1             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT1
    #define FTM2CNTINL_BIT2             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT2
    #define FTM2CNTINL_BIT3             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT3
    #define FTM2CNTINL_BIT4             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT4
    #define FTM2CNTINL_BIT5             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT5
    #define FTM2CNTINL_BIT6             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT6
    #define FTM2CNTINL_BIT7             _FTM2CNTIN.Overlap_STR.FTM2CNTINLSTR.Bits.BIT7
    
    #define FTM2CNTINL_BIT0_MASK        1
    #define FTM2CNTINL_BIT1_MASK        2
    #define FTM2CNTINL_BIT2_MASK        4
    #define FTM2CNTINL_BIT3_MASK        8
    #define FTM2CNTINL_BIT4_MASK        16
    #define FTM2CNTINL_BIT5_MASK        32
    #define FTM2CNTINL_BIT6_MASK        64
    #define FTM2CNTINL_BIT7_MASK        128
    
  } Overlap_STR;

  struct {
    word BIT0        :1;                                       /* FTM2 Counter Initial Value Bit 0 */
    word BIT1        :1;                                       /* FTM2 Counter Initial Value Bit 1 */
    word BIT2        :1;                                       /* FTM2 Counter Initial Value Bit 2 */
    word BIT3        :1;                                       /* FTM2 Counter Initial Value Bit 3 */
    word BIT4        :1;                                       /* FTM2 Counter Initial Value Bit 4 */
    word BIT5        :1;                                       /* FTM2 Counter Initial Value Bit 5 */
    word BIT6        :1;                                       /* FTM2 Counter Initial Value Bit 6 */
    word BIT7        :1;                                       /* FTM2 Counter Initial Value Bit 7 */
    word BIT8        :1;                                       /* FTM2 Counter Initial Value Bit 8 */
    word BIT9        :1;                                       /* FTM2 Counter Initial Value Bit 9 */
    word BIT10       :1;                                       /* FTM2 Counter Initial Value Bit 10 */
    word BIT11       :1;                                       /* FTM2 Counter Initial Value Bit 11 */
    word BIT12       :1;                                       /* FTM2 Counter Initial Value Bit 12 */
    word BIT13       :1;                                       /* FTM2 Counter Initial Value Bit 13 */
    word BIT14       :1;                                       /* FTM2 Counter Initial Value Bit 14 */
    word BIT15       :1;                                       /* FTM2 Counter Initial Value Bit 15 */
  } Bits;
} FTM2CNTINSTR;
extern volatile FTM2CNTINSTR _FTM2CNTIN @0xFFFF80E0;
#define FTM2CNTIN                       _FTM2CNTIN.Word
#define FTM2CNTIN_BIT0                  _FTM2CNTIN.Bits.BIT0
#define FTM2CNTIN_BIT1                  _FTM2CNTIN.Bits.BIT1
#define FTM2CNTIN_BIT2                  _FTM2CNTIN.Bits.BIT2
#define FTM2CNTIN_BIT3                  _FTM2CNTIN.Bits.BIT3
#define FTM2CNTIN_BIT4                  _FTM2CNTIN.Bits.BIT4
#define FTM2CNTIN_BIT5                  _FTM2CNTIN.Bits.BIT5
#define FTM2CNTIN_BIT6                  _FTM2CNTIN.Bits.BIT6
#define FTM2CNTIN_BIT7                  _FTM2CNTIN.Bits.BIT7
#define FTM2CNTIN_BIT8                  _FTM2CNTIN.Bits.BIT8
#define FTM2CNTIN_BIT9                  _FTM2CNTIN.Bits.BIT9
#define FTM2CNTIN_BIT10                 _FTM2CNTIN.Bits.BIT10
#define FTM2CNTIN_BIT11                 _FTM2CNTIN.Bits.BIT11
#define FTM2CNTIN_BIT12                 _FTM2CNTIN.Bits.BIT12
#define FTM2CNTIN_BIT13                 _FTM2CNTIN.Bits.BIT13
#define FTM2CNTIN_BIT14                 _FTM2CNTIN.Bits.BIT14
#define FTM2CNTIN_BIT15                 _FTM2CNTIN.Bits.BIT15

#define FTM2CNTIN_BIT0_MASK             1
#define FTM2CNTIN_BIT1_MASK             2
#define FTM2CNTIN_BIT2_MASK             4
#define FTM2CNTIN_BIT3_MASK             8
#define FTM2CNTIN_BIT4_MASK             16
#define FTM2CNTIN_BIT5_MASK             32
#define FTM2CNTIN_BIT6_MASK             64
#define FTM2CNTIN_BIT7_MASK             128
#define FTM2CNTIN_BIT8_MASK             256
#define FTM2CNTIN_BIT9_MASK             512
#define FTM2CNTIN_BIT10_MASK            1024
#define FTM2CNTIN_BIT11_MASK            2048
#define FTM2CNTIN_BIT12_MASK            4096
#define FTM2CNTIN_BIT13_MASK            8192
#define FTM2CNTIN_BIT14_MASK            16384
#define FTM2CNTIN_BIT15_MASK            32768


/*** FTM2STATUS - FTM2 Capture and Compare Status Register; 0xFFFF80E2 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0F        :1;                                       /* Channel 0 Flag */
    byte CH1F        :1;                                       /* Channel 1 Flag */
    byte CH2F        :1;                                       /* Channel 2 Flag */
    byte CH3F        :1;                                       /* Channel 3 Flag */
    byte CH4F        :1;                                       /* Channel 4 Flag */
    byte CH5F        :1;                                       /* Channel 5 Flag */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM2STATUSSTR;
extern volatile FTM2STATUSSTR _FTM2STATUS @0xFFFF80E2;
#define FTM2STATUS                      _FTM2STATUS.Byte
#define FTM2STATUS_CH0F                 _FTM2STATUS.Bits.CH0F
#define FTM2STATUS_CH1F                 _FTM2STATUS.Bits.CH1F
#define FTM2STATUS_CH2F                 _FTM2STATUS.Bits.CH2F
#define FTM2STATUS_CH3F                 _FTM2STATUS.Bits.CH3F
#define FTM2STATUS_CH4F                 _FTM2STATUS.Bits.CH4F
#define FTM2STATUS_CH5F                 _FTM2STATUS.Bits.CH5F

#define FTM2STATUS_CH0F_MASK            1
#define FTM2STATUS_CH1F_MASK            2
#define FTM2STATUS_CH2F_MASK            4
#define FTM2STATUS_CH3F_MASK            8
#define FTM2STATUS_CH4F_MASK            16
#define FTM2STATUS_CH5F_MASK            32


/*** FTM2MODE - FTM2 Features Mode Selection Register; 0xFFFF80E3 ***/
typedef union {
  byte Byte;
  struct {
    byte FTMEN       :1;                                       /* FTM2 Enable */
    byte INIT        :1;                                       /* Initialize the Channels Output */
    byte WPDIS       :1;                                       /* Write Protected Disable */
    byte PWMSYNC     :1;                                       /* PWM synchronization mode */
    byte CAPTTEST    :1;                                       /* Capture test mode enable */
    byte FAULTM0     :1;                                       /* Fault Control Mode Bits, bit 0 */
    byte FAULTM1     :1;                                       /* Fault Control Mode Bits, bit 1 */
    byte FAULTIE     :1;                                       /* Fault Interrupt Enable */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpFAULTM :2;
    byte         :1;
  } MergedBits;
} FTM2MODESTR;
extern volatile FTM2MODESTR _FTM2MODE @0xFFFF80E3;
#define FTM2MODE                        _FTM2MODE.Byte
#define FTM2MODE_FTMEN                  _FTM2MODE.Bits.FTMEN
#define FTM2MODE_INIT                   _FTM2MODE.Bits.INIT
#define FTM2MODE_WPDIS                  _FTM2MODE.Bits.WPDIS
#define FTM2MODE_PWMSYNC                _FTM2MODE.Bits.PWMSYNC
#define FTM2MODE_CAPTTEST               _FTM2MODE.Bits.CAPTTEST
#define FTM2MODE_FAULTM0                _FTM2MODE.Bits.FAULTM0
#define FTM2MODE_FAULTM1                _FTM2MODE.Bits.FAULTM1
#define FTM2MODE_FAULTIE                _FTM2MODE.Bits.FAULTIE
#define FTM2MODE_FAULTM                 _FTM2MODE.MergedBits.grpFAULTM

#define FTM2MODE_FTMEN_MASK             1
#define FTM2MODE_INIT_MASK              2
#define FTM2MODE_WPDIS_MASK             4
#define FTM2MODE_PWMSYNC_MASK           8
#define FTM2MODE_CAPTTEST_MASK          16
#define FTM2MODE_FAULTM0_MASK           32
#define FTM2MODE_FAULTM1_MASK           64
#define FTM2MODE_FAULTIE_MASK           128
#define FTM2MODE_FAULTM_MASK            96
#define FTM2MODE_FAULTM_BITNUM          5


/*** FTM2SYNC - FTM2 Synchronization Register; 0xFFFF80E4 ***/
typedef union {
  byte Byte;
  struct {
    byte CNTMIN      :1;                                       /* FTM2 Counter is Minimum */
    byte CNTMAX      :1;                                       /* FTM2 Counter is Maximum */
    byte REINIT      :1;                                       /* Reinitialization of FTM2 Counter by Synchronization */
    byte SYNCHOM     :1;                                       /* Output Mask by Synchronization */
    byte TRIG0       :1;                                       /* External Trigger Bit 0 for Synchronization */
    byte TRIG1       :1;                                       /* External Trigger Bit 1 for Synchronization */
    byte TRIG2       :1;                                       /* External Trigger Bit 2 for Synchronization */
    byte SWSYNC      :1;                                       /* Software Trigger for Synchronization */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpTRIG :3;
    byte         :1;
  } MergedBits;
} FTM2SYNCSTR;
extern volatile FTM2SYNCSTR _FTM2SYNC @0xFFFF80E4;
#define FTM2SYNC                        _FTM2SYNC.Byte
#define FTM2SYNC_CNTMIN                 _FTM2SYNC.Bits.CNTMIN
#define FTM2SYNC_CNTMAX                 _FTM2SYNC.Bits.CNTMAX
#define FTM2SYNC_REINIT                 _FTM2SYNC.Bits.REINIT
#define FTM2SYNC_SYNCHOM                _FTM2SYNC.Bits.SYNCHOM
#define FTM2SYNC_TRIG0                  _FTM2SYNC.Bits.TRIG0
#define FTM2SYNC_TRIG1                  _FTM2SYNC.Bits.TRIG1
#define FTM2SYNC_TRIG2                  _FTM2SYNC.Bits.TRIG2
#define FTM2SYNC_SWSYNC                 _FTM2SYNC.Bits.SWSYNC
#define FTM2SYNC_TRIG                   _FTM2SYNC.MergedBits.grpTRIG

#define FTM2SYNC_CNTMIN_MASK            1
#define FTM2SYNC_CNTMAX_MASK            2
#define FTM2SYNC_REINIT_MASK            4
#define FTM2SYNC_SYNCHOM_MASK           8
#define FTM2SYNC_TRIG0_MASK             16
#define FTM2SYNC_TRIG1_MASK             32
#define FTM2SYNC_TRIG2_MASK             64
#define FTM2SYNC_SWSYNC_MASK            128
#define FTM2SYNC_TRIG_MASK              112
#define FTM2SYNC_TRIG_BITNUM            4


/*** FTM2OUTINIT - FTM2 Initial State for Channels Output Register; 0xFFFF80E5 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0OI       :1;                                       /* Initial State for Channel 0 Output */
    byte CH1OI       :1;                                       /* Initial State for Channel 1 Output */
    byte CH2OI       :1;                                       /* Initial State for Channel 2 Output */
    byte CH3OI       :1;                                       /* Initial State for Channel 3 Output */
    byte CH4OI       :1;                                       /* Initial State for Channel 4 Output */
    byte CH5OI       :1;                                       /* Initial State for Channel 5 Output */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM2OUTINITSTR;
extern volatile FTM2OUTINITSTR _FTM2OUTINIT @0xFFFF80E5;
#define FTM2OUTINIT                     _FTM2OUTINIT.Byte
#define FTM2OUTINIT_CH0OI               _FTM2OUTINIT.Bits.CH0OI
#define FTM2OUTINIT_CH1OI               _FTM2OUTINIT.Bits.CH1OI
#define FTM2OUTINIT_CH2OI               _FTM2OUTINIT.Bits.CH2OI
#define FTM2OUTINIT_CH3OI               _FTM2OUTINIT.Bits.CH3OI
#define FTM2OUTINIT_CH4OI               _FTM2OUTINIT.Bits.CH4OI
#define FTM2OUTINIT_CH5OI               _FTM2OUTINIT.Bits.CH5OI

#define FTM2OUTINIT_CH0OI_MASK          1
#define FTM2OUTINIT_CH1OI_MASK          2
#define FTM2OUTINIT_CH2OI_MASK          4
#define FTM2OUTINIT_CH3OI_MASK          8
#define FTM2OUTINIT_CH4OI_MASK          16
#define FTM2OUTINIT_CH5OI_MASK          32


/*** FTM2OUTMASK - FTM2 Output Mask Register; 0xFFFF80E6 ***/
typedef union {
  byte Byte;
  struct {
    byte CH0OM       :1;                                       /* Channel 0 Output Mask */
    byte CH1OM       :1;                                       /* Channel 1 Output Mask */
    byte CH2OM       :1;                                       /* Channel 2 Output Mask */
    byte CH3OM       :1;                                       /* Channel 3 Output Mask */
    byte CH4OM       :1;                                       /* Channel 4 Output Mask */
    byte CH5OM       :1;                                       /* Channel 5 Output Mask */
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM2OUTMASKSTR;
extern volatile FTM2OUTMASKSTR _FTM2OUTMASK @0xFFFF80E6;
#define FTM2OUTMASK                     _FTM2OUTMASK.Byte
#define FTM2OUTMASK_CH0OM               _FTM2OUTMASK.Bits.CH0OM
#define FTM2OUTMASK_CH1OM               _FTM2OUTMASK.Bits.CH1OM
#define FTM2OUTMASK_CH2OM               _FTM2OUTMASK.Bits.CH2OM
#define FTM2OUTMASK_CH3OM               _FTM2OUTMASK.Bits.CH3OM
#define FTM2OUTMASK_CH4OM               _FTM2OUTMASK.Bits.CH4OM
#define FTM2OUTMASK_CH5OM               _FTM2OUTMASK.Bits.CH5OM

#define FTM2OUTMASK_CH0OM_MASK          1
#define FTM2OUTMASK_CH1OM_MASK          2
#define FTM2OUTMASK_CH2OM_MASK          4
#define FTM2OUTMASK_CH3OM_MASK          8
#define FTM2OUTMASK_CH4OM_MASK          16
#define FTM2OUTMASK_CH5OM_MASK          32


/*** FTM2COMBINE0 - FTM2 Function For Linked Channel 0 Register; 0xFFFF80E7 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 0 and 1 */
    byte COMP        :1;                                       /* Complementary of Channel 0 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM2COMBINE0STR;
extern volatile FTM2COMBINE0STR _FTM2COMBINE0 @0xFFFF80E7;
#define FTM2COMBINE0                    _FTM2COMBINE0.Byte
#define FTM2COMBINE0_COMBINE            _FTM2COMBINE0.Bits.COMBINE
#define FTM2COMBINE0_COMP               _FTM2COMBINE0.Bits.COMP
#define FTM2COMBINE0_DTEN               _FTM2COMBINE0.Bits.DTEN
#define FTM2COMBINE0_SYNCEN             _FTM2COMBINE0.Bits.SYNCEN
#define FTM2COMBINE0_FAULTEN            _FTM2COMBINE0.Bits.FAULTEN
/* FTM2COMBINE_ARR: Access 3 FTM2COMBINEx registers in an array */
#define FTM2COMBINE_ARR                 ((volatile byte *) &FTM2COMBINE0)

#define FTM2COMBINE0_COMBINE_MASK       1
#define FTM2COMBINE0_COMP_MASK          2
#define FTM2COMBINE0_DTEN_MASK          16
#define FTM2COMBINE0_SYNCEN_MASK        32
#define FTM2COMBINE0_FAULTEN_MASK       64


/*** FTM2COMBINE1 - FTM2 Function For Linked Channel 1 Register; 0xFFFF80E8 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 2 and 3 */
    byte COMP        :1;                                       /* Complementary of Channel 2 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM2COMBINE1STR;
extern volatile FTM2COMBINE1STR _FTM2COMBINE1 @0xFFFF80E8;
#define FTM2COMBINE1                    _FTM2COMBINE1.Byte
#define FTM2COMBINE1_COMBINE            _FTM2COMBINE1.Bits.COMBINE
#define FTM2COMBINE1_COMP               _FTM2COMBINE1.Bits.COMP
#define FTM2COMBINE1_DTEN               _FTM2COMBINE1.Bits.DTEN
#define FTM2COMBINE1_SYNCEN             _FTM2COMBINE1.Bits.SYNCEN
#define FTM2COMBINE1_FAULTEN            _FTM2COMBINE1.Bits.FAULTEN

#define FTM2COMBINE1_COMBINE_MASK       1
#define FTM2COMBINE1_COMP_MASK          2
#define FTM2COMBINE1_DTEN_MASK          16
#define FTM2COMBINE1_SYNCEN_MASK        32
#define FTM2COMBINE1_FAULTEN_MASK       64


/*** FTM2COMBINE2 - FTM2 Function For Linked Channel 2 Register; 0xFFFF80E9 ***/
typedef union {
  byte Byte;
  struct {
    byte COMBINE     :1;                                       /* Enables the Combine of the Channels 4 and 5 */
    byte COMP        :1;                                       /* Complementary of Channel 4 */
    byte             :1; 
    byte             :1; 
    byte DTEN        :1;                                       /* Deadtime Enable */
    byte SYNCEN      :1;                                       /* Synchronization Enable */
    byte FAULTEN     :1;                                       /* Fault Control Enable */
    byte             :1; 
  } Bits;
} FTM2COMBINE2STR;
extern volatile FTM2COMBINE2STR _FTM2COMBINE2 @0xFFFF80E9;
#define FTM2COMBINE2                    _FTM2COMBINE2.Byte
#define FTM2COMBINE2_COMBINE            _FTM2COMBINE2.Bits.COMBINE
#define FTM2COMBINE2_COMP               _FTM2COMBINE2.Bits.COMP
#define FTM2COMBINE2_DTEN               _FTM2COMBINE2.Bits.DTEN
#define FTM2COMBINE2_SYNCEN             _FTM2COMBINE2.Bits.SYNCEN
#define FTM2COMBINE2_FAULTEN            _FTM2COMBINE2.Bits.FAULTEN

#define FTM2COMBINE2_COMBINE_MASK       1
#define FTM2COMBINE2_COMP_MASK          2
#define FTM2COMBINE2_DTEN_MASK          16
#define FTM2COMBINE2_SYNCEN_MASK        32
#define FTM2COMBINE2_FAULTEN_MASK       64


/*** FTM2DEADTIME - FTM2 Deadtime Insertion Control Register; 0xFFFF80EB ***/
typedef union {
  byte Byte;
  struct {
    byte DTVAL0      :1;                                       /* Deadtime Value Bits, bit 0 */
    byte DTVAL1      :1;                                       /* Deadtime Value Bits, bit 1 */
    byte DTVAL2      :1;                                       /* Deadtime Value Bits, bit 2 */
    byte DTVAL3      :1;                                       /* Deadtime Value Bits, bit 3 */
    byte DTVAL4      :1;                                       /* Deadtime Value Bits, bit 4 */
    byte DTVAL5      :1;                                       /* Deadtime Value Bits, bit 5 */
    byte DTPS0       :1;                                       /* Deadtime Prescaler Value Bits, bit 0 */
    byte DTPS1       :1;                                       /* Deadtime Prescaler Value Bits, bit 1 */
  } Bits;
  struct {
    byte grpDTVAL :6;
    byte grpDTPS :2;
  } MergedBits;
} FTM2DEADTIMESTR;
extern volatile FTM2DEADTIMESTR _FTM2DEADTIME @0xFFFF80EB;
#define FTM2DEADTIME                    _FTM2DEADTIME.Byte
#define FTM2DEADTIME_DTVAL0             _FTM2DEADTIME.Bits.DTVAL0
#define FTM2DEADTIME_DTVAL1             _FTM2DEADTIME.Bits.DTVAL1
#define FTM2DEADTIME_DTVAL2             _FTM2DEADTIME.Bits.DTVAL2
#define FTM2DEADTIME_DTVAL3             _FTM2DEADTIME.Bits.DTVAL3
#define FTM2DEADTIME_DTVAL4             _FTM2DEADTIME.Bits.DTVAL4
#define FTM2DEADTIME_DTVAL5             _FTM2DEADTIME.Bits.DTVAL5
#define FTM2DEADTIME_DTPS0              _FTM2DEADTIME.Bits.DTPS0
#define FTM2DEADTIME_DTPS1              _FTM2DEADTIME.Bits.DTPS1
#define FTM2DEADTIME_DTVAL              _FTM2DEADTIME.MergedBits.grpDTVAL
#define FTM2DEADTIME_DTPS               _FTM2DEADTIME.MergedBits.grpDTPS

#define FTM2DEADTIME_DTVAL0_MASK        1
#define FTM2DEADTIME_DTVAL1_MASK        2
#define FTM2DEADTIME_DTVAL2_MASK        4
#define FTM2DEADTIME_DTVAL3_MASK        8
#define FTM2DEADTIME_DTVAL4_MASK        16
#define FTM2DEADTIME_DTVAL5_MASK        32
#define FTM2DEADTIME_DTPS0_MASK         64
#define FTM2DEADTIME_DTPS1_MASK         128
#define FTM2DEADTIME_DTVAL_MASK         63
#define FTM2DEADTIME_DTVAL_BITNUM       0
#define FTM2DEADTIME_DTPS_MASK          192
#define FTM2DEADTIME_DTPS_BITNUM        6


/*** FTM2EXTTRIG - FTM2 ExternalTrigger Register; 0xFFFF80EC ***/
typedef union {
  byte Byte;
  struct {
    byte CH2TRIG     :1;                                       /* Channel 2 trigger enable */
    byte CH3TRIG     :1;                                       /* Channel 3 trigger enable */
    byte CH4TRIG     :1;                                       /* Channel 4 trigger enable */
    byte CH5TRIG     :1;                                       /* Channel 5 trigger enable */
    byte CH0TRIG     :1;                                       /* Channel 0 trigger enable */
    byte CH1TRIG     :1;                                       /* Channel 1 trigger enable */
    byte INITTRIGEN  :1;                                       /* Enables the Generation of a Trigger Signal */
    byte TRIGF       :1;                                       /* Channel trigger flag */
  } Bits;
} FTM2EXTTRIGSTR;
extern volatile FTM2EXTTRIGSTR _FTM2EXTTRIG @0xFFFF80EC;
#define FTM2EXTTRIG                     _FTM2EXTTRIG.Byte
#define FTM2EXTTRIG_CH2TRIG             _FTM2EXTTRIG.Bits.CH2TRIG
#define FTM2EXTTRIG_CH3TRIG             _FTM2EXTTRIG.Bits.CH3TRIG
#define FTM2EXTTRIG_CH4TRIG             _FTM2EXTTRIG.Bits.CH4TRIG
#define FTM2EXTTRIG_CH5TRIG             _FTM2EXTTRIG.Bits.CH5TRIG
#define FTM2EXTTRIG_CH0TRIG             _FTM2EXTTRIG.Bits.CH0TRIG
#define FTM2EXTTRIG_CH1TRIG             _FTM2EXTTRIG.Bits.CH1TRIG
#define FTM2EXTTRIG_INITTRIGEN          _FTM2EXTTRIG.Bits.INITTRIGEN
#define FTM2EXTTRIG_TRIGF               _FTM2EXTTRIG.Bits.TRIGF

#define FTM2EXTTRIG_CH2TRIG_MASK        1
#define FTM2EXTTRIG_CH3TRIG_MASK        2
#define FTM2EXTTRIG_CH4TRIG_MASK        4
#define FTM2EXTTRIG_CH5TRIG_MASK        8
#define FTM2EXTTRIG_CH0TRIG_MASK        16
#define FTM2EXTTRIG_CH1TRIG_MASK        32
#define FTM2EXTTRIG_INITTRIGEN_MASK     64
#define FTM2EXTTRIG_TRIGF_MASK          128


/*** FTM2POL - FTM2 Channels Polarity Register; 0xFFFF80ED ***/
typedef union {
  byte Byte;
  struct {
    byte POL0        :1;                                       /* Channel 0 Polarity */
    byte POL1        :1;                                       /* Channel 1 Polarity */
    byte POL2        :1;                                       /* Channel 2 Polarity */
    byte POL3        :1;                                       /* Channel 3 Polarity */
    byte POL4        :1;                                       /* Channel 4 Polarity */
    byte POL5        :1;                                       /* Channel 5 Polarity */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPOL  :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2POLSTR;
extern volatile FTM2POLSTR _FTM2POL @0xFFFF80ED;
#define FTM2POL                         _FTM2POL.Byte
#define FTM2POL_POL0                    _FTM2POL.Bits.POL0
#define FTM2POL_POL1                    _FTM2POL.Bits.POL1
#define FTM2POL_POL2                    _FTM2POL.Bits.POL2
#define FTM2POL_POL3                    _FTM2POL.Bits.POL3
#define FTM2POL_POL4                    _FTM2POL.Bits.POL4
#define FTM2POL_POL5                    _FTM2POL.Bits.POL5
#define FTM2POL_POL                     _FTM2POL.MergedBits.grpPOL

#define FTM2POL_POL0_MASK               1
#define FTM2POL_POL1_MASK               2
#define FTM2POL_POL2_MASK               4
#define FTM2POL_POL3_MASK               8
#define FTM2POL_POL4_MASK               16
#define FTM2POL_POL5_MASK               32
#define FTM2POL_POL_MASK                63
#define FTM2POL_POL_BITNUM              0


/*** FTM2FMS - FTM2 Fault Mode Status Register; 0xFFFF80EE ***/
typedef union {
  byte Byte;
  struct {
    byte FAULTF0     :1;                                       /* Fault detection flag 0 */
    byte FAULTF1     :1;                                       /* Fault detection flag 1 */
    byte FAULTF2     :1;                                       /* Fault detection flag 2 */
    byte FAULTF3     :1;                                       /* Fault detection flag 3 */
    byte             :1; 
    byte FAULTIN     :1;                                       /* Fault Input */
    byte WPEN        :1;                                       /* Write Protected Enable */
    byte FAULTF      :1;                                       /* Fault Detection Flag */
  } Bits;
  struct {
    byte         :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2FMSSTR;
extern volatile FTM2FMSSTR _FTM2FMS @0xFFFF80EE;
#define FTM2FMS                         _FTM2FMS.Byte
#define FTM2FMS_FAULTF0                 _FTM2FMS.Bits.FAULTF0
#define FTM2FMS_FAULTF1                 _FTM2FMS.Bits.FAULTF1
#define FTM2FMS_FAULTF2                 _FTM2FMS.Bits.FAULTF2
#define FTM2FMS_FAULTF3                 _FTM2FMS.Bits.FAULTF3
#define FTM2FMS_FAULTIN                 _FTM2FMS.Bits.FAULTIN
#define FTM2FMS_WPEN                    _FTM2FMS.Bits.WPEN
#define FTM2FMS_FAULTF                  _FTM2FMS.Bits.FAULTF

#define FTM2FMS_FAULTF0_MASK            1
#define FTM2FMS_FAULTF1_MASK            2
#define FTM2FMS_FAULTF2_MASK            4
#define FTM2FMS_FAULTF3_MASK            8
#define FTM2FMS_FAULTIN_MASK            32
#define FTM2FMS_WPEN_MASK               64
#define FTM2FMS_FAULTF_MASK             128


/*** FTM2FILTER0 - FTM2 Input Capture Filter Control Register 0; 0xFFFF80EF ***/
typedef union {
  byte Byte;
  struct {
    byte CH0FVAL0    :1;                                       /* Channel 0 Input Filter Bits, bit 0 */
    byte CH0FVAL1    :1;                                       /* Channel 0 Input Filter Bits, bit 1 */
    byte CH0FVAL2    :1;                                       /* Channel 0 Input Filter Bits, bit 2 */
    byte CH0FVAL3    :1;                                       /* Channel 0 Input Filter Bits, bit 3 */
    byte CH1FVAL0    :1;                                       /* Channel 1 Input Filter Bits, bit 0 */
    byte CH1FVAL1    :1;                                       /* Channel 1 Input Filter Bits, bit 1 */
    byte CH1FVAL2    :1;                                       /* Channel 1 Input Filter Bits, bit 2 */
    byte CH1FVAL3    :1;                                       /* Channel 1 Input Filter Bits, bit 3 */
  } Bits;
  struct {
    byte grpCH0FVAL :4;
    byte grpCH1FVAL :4;
  } MergedBits;
} FTM2FILTER0STR;
extern volatile FTM2FILTER0STR _FTM2FILTER0 @0xFFFF80EF;
#define FTM2FILTER0                     _FTM2FILTER0.Byte
#define FTM2FILTER0_CH0FVAL0            _FTM2FILTER0.Bits.CH0FVAL0
#define FTM2FILTER0_CH0FVAL1            _FTM2FILTER0.Bits.CH0FVAL1
#define FTM2FILTER0_CH0FVAL2            _FTM2FILTER0.Bits.CH0FVAL2
#define FTM2FILTER0_CH0FVAL3            _FTM2FILTER0.Bits.CH0FVAL3
#define FTM2FILTER0_CH1FVAL0            _FTM2FILTER0.Bits.CH1FVAL0
#define FTM2FILTER0_CH1FVAL1            _FTM2FILTER0.Bits.CH1FVAL1
#define FTM2FILTER0_CH1FVAL2            _FTM2FILTER0.Bits.CH1FVAL2
#define FTM2FILTER0_CH1FVAL3            _FTM2FILTER0.Bits.CH1FVAL3
/* FTM2FILTER_ARR: Access 2 FTM2FILTERx registers in an array */
#define FTM2FILTER_ARR                  ((volatile byte *) &FTM2FILTER0)
#define FTM2FILTER0_CH0FVAL             _FTM2FILTER0.MergedBits.grpCH0FVAL
#define FTM2FILTER0_CH1FVAL             _FTM2FILTER0.MergedBits.grpCH1FVAL

#define FTM2FILTER0_CH0FVAL0_MASK       1
#define FTM2FILTER0_CH0FVAL1_MASK       2
#define FTM2FILTER0_CH0FVAL2_MASK       4
#define FTM2FILTER0_CH0FVAL3_MASK       8
#define FTM2FILTER0_CH1FVAL0_MASK       16
#define FTM2FILTER0_CH1FVAL1_MASK       32
#define FTM2FILTER0_CH1FVAL2_MASK       64
#define FTM2FILTER0_CH1FVAL3_MASK       128
#define FTM2FILTER0_CH0FVAL_MASK        15
#define FTM2FILTER0_CH0FVAL_BITNUM      0
#define FTM2FILTER0_CH1FVAL_MASK        240
#define FTM2FILTER0_CH1FVAL_BITNUM      4


/*** FTM2FILTER1 - FTM2 Input Capture Filter Control Register 1; 0xFFFF80F0 ***/
typedef union {
  byte Byte;
  struct {
    byte CH2FVAL0    :1;                                       /* Channel 2 Input Filter Bits, bit 0 */
    byte CH2FVAL1    :1;                                       /* Channel 2 Input Filter Bits, bit 1 */
    byte CH2FVAL2    :1;                                       /* Channel 2 Input Filter Bits, bit 2 */
    byte CH2FVAL3    :1;                                       /* Channel 2 Input Filter Bits, bit 3 */
    byte CH3FVAL0    :1;                                       /* Channel 3 Input Filter Bits, bit 0 */
    byte CH3FVAL1    :1;                                       /* Channel 3 Input Filter Bits, bit 1 */
    byte CH3FVAL2    :1;                                       /* Channel 3 Input Filter Bits, bit 2 */
    byte CH3FVAL3    :1;                                       /* Channel 3 Input Filter Bits, bit 3 */
  } Bits;
  struct {
    byte grpCH2FVAL :4;
    byte grpCH3FVAL :4;
  } MergedBits;
} FTM2FILTER1STR;
extern volatile FTM2FILTER1STR _FTM2FILTER1 @0xFFFF80F0;
#define FTM2FILTER1                     _FTM2FILTER1.Byte
#define FTM2FILTER1_CH2FVAL0            _FTM2FILTER1.Bits.CH2FVAL0
#define FTM2FILTER1_CH2FVAL1            _FTM2FILTER1.Bits.CH2FVAL1
#define FTM2FILTER1_CH2FVAL2            _FTM2FILTER1.Bits.CH2FVAL2
#define FTM2FILTER1_CH2FVAL3            _FTM2FILTER1.Bits.CH2FVAL3
#define FTM2FILTER1_CH3FVAL0            _FTM2FILTER1.Bits.CH3FVAL0
#define FTM2FILTER1_CH3FVAL1            _FTM2FILTER1.Bits.CH3FVAL1
#define FTM2FILTER1_CH3FVAL2            _FTM2FILTER1.Bits.CH3FVAL2
#define FTM2FILTER1_CH3FVAL3            _FTM2FILTER1.Bits.CH3FVAL3
#define FTM2FILTER1_CH2FVAL             _FTM2FILTER1.MergedBits.grpCH2FVAL
#define FTM2FILTER1_CH3FVAL             _FTM2FILTER1.MergedBits.grpCH3FVAL

#define FTM2FILTER1_CH2FVAL0_MASK       1
#define FTM2FILTER1_CH2FVAL1_MASK       2
#define FTM2FILTER1_CH2FVAL2_MASK       4
#define FTM2FILTER1_CH2FVAL3_MASK       8
#define FTM2FILTER1_CH3FVAL0_MASK       16
#define FTM2FILTER1_CH3FVAL1_MASK       32
#define FTM2FILTER1_CH3FVAL2_MASK       64
#define FTM2FILTER1_CH3FVAL3_MASK       128
#define FTM2FILTER1_CH2FVAL_MASK        15
#define FTM2FILTER1_CH2FVAL_BITNUM      0
#define FTM2FILTER1_CH3FVAL_MASK        240
#define FTM2FILTER1_CH3FVAL_BITNUM      4


/*** FTM2FLTFILTER - FTM2 Fault Input Filter Control Register; 0xFFFF80F1 ***/
typedef union {
  byte Byte;
  struct {
    byte FFVAL0      :1;                                       /* Fault Input Filter Bits, bit 0 */
    byte FFVAL1      :1;                                       /* Fault Input Filter Bits, bit 1 */
    byte FFVAL2      :1;                                       /* Fault Input Filter Bits, bit 2 */
    byte FFVAL3      :1;                                       /* Fault Input Filter Bits, bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpFFVAL :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FTM2FLTFILTERSTR;
extern volatile FTM2FLTFILTERSTR _FTM2FLTFILTER @0xFFFF80F1;
#define FTM2FLTFILTER                   _FTM2FLTFILTER.Byte
#define FTM2FLTFILTER_FFVAL0            _FTM2FLTFILTER.Bits.FFVAL0
#define FTM2FLTFILTER_FFVAL1            _FTM2FLTFILTER.Bits.FFVAL1
#define FTM2FLTFILTER_FFVAL2            _FTM2FLTFILTER.Bits.FFVAL2
#define FTM2FLTFILTER_FFVAL3            _FTM2FLTFILTER.Bits.FFVAL3
#define FTM2FLTFILTER_FFVAL             _FTM2FLTFILTER.MergedBits.grpFFVAL

#define FTM2FLTFILTER_FFVAL0_MASK       1
#define FTM2FLTFILTER_FFVAL1_MASK       2
#define FTM2FLTFILTER_FFVAL2_MASK       4
#define FTM2FLTFILTER_FFVAL3_MASK       8
#define FTM2FLTFILTER_FFVAL_MASK        15
#define FTM2FLTFILTER_FFVAL_BITNUM      0


/*** FTM2FLTCTRL - FTM2 Fault Control Register; 0xFFFF80F2 ***/
typedef union {
  byte Byte;
  struct {
    byte FAULT0EN    :1;                                       /* Fault input 0 enable */
    byte FAULT1EN    :1;                                       /* Fault input 1 enable */
    byte             :1; 
    byte             :1; 
    byte FFLTR0EN    :1;                                       /* Fault input 0 filter enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} FTM2FLTCTRLSTR;
extern volatile FTM2FLTCTRLSTR _FTM2FLTCTRL @0xFFFF80F2;
#define FTM2FLTCTRL                     _FTM2FLTCTRL.Byte
#define FTM2FLTCTRL_FAULT0EN            _FTM2FLTCTRL.Bits.FAULT0EN
#define FTM2FLTCTRL_FAULT1EN            _FTM2FLTCTRL.Bits.FAULT1EN
#define FTM2FLTCTRL_FFLTR0EN            _FTM2FLTCTRL.Bits.FFLTR0EN

#define FTM2FLTCTRL_FAULT0EN_MASK       1
#define FTM2FLTCTRL_FAULT1EN_MASK       2
#define FTM2FLTCTRL_FFLTR0EN_MASK       16


/*** PTAPUE - Port A Pulling Enable Register; 0xFFFF8100 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPUE0     :1;                                       /* Internal Pulling Enable for Port A Bit 0 */
    byte PTAPUE1     :1;                                       /* Internal Pulling Enable for Port A Bit 1 */
    byte PTAPUE2     :1;                                       /* Internal Pulling Enable for Port A Bit 2 */
    byte PTAPUE3     :1;                                       /* Internal Pulling Enable for Port A Bit 3 */
    byte PTAPUE4     :1;                                       /* Internal Pulling Enable for Port A Bit 4 */
    byte PTAPUE5     :1;                                       /* Internal Pulling Enable for Port A Bit 5 */
    byte PTAPUE6     :1;                                       /* Internal Pulling Enable for Port A Bit 6 */
    byte PTAPUE7     :1;                                       /* Internal Pulling Enable for Port A Bit 7 */
  } Bits;
} PTAPUESTR;
extern volatile PTAPUESTR _PTAPUE @0xFFFF8100;
#define PTAPUE                          _PTAPUE.Byte
#define PTAPUE_PTAPUE0                  _PTAPUE.Bits.PTAPUE0
#define PTAPUE_PTAPUE1                  _PTAPUE.Bits.PTAPUE1
#define PTAPUE_PTAPUE2                  _PTAPUE.Bits.PTAPUE2
#define PTAPUE_PTAPUE3                  _PTAPUE.Bits.PTAPUE3
#define PTAPUE_PTAPUE4                  _PTAPUE.Bits.PTAPUE4
#define PTAPUE_PTAPUE5                  _PTAPUE.Bits.PTAPUE5
#define PTAPUE_PTAPUE6                  _PTAPUE.Bits.PTAPUE6
#define PTAPUE_PTAPUE7                  _PTAPUE.Bits.PTAPUE7

#define PTAPUE_PTAPUE0_MASK             1
#define PTAPUE_PTAPUE1_MASK             2
#define PTAPUE_PTAPUE2_MASK             4
#define PTAPUE_PTAPUE3_MASK             8
#define PTAPUE_PTAPUE4_MASK             16
#define PTAPUE_PTAPUE5_MASK             32
#define PTAPUE_PTAPUE6_MASK             64
#define PTAPUE_PTAPUE7_MASK             128


/*** PTAPUS - Port A Pullup/Pulldown Select Register; 0xFFFF8101 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTAPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTAPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTAPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTAPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTAPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTAPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTAPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTAPUSSTR;
extern volatile PTAPUSSTR _PTAPUS @0xFFFF8101;
#define PTAPUS                          _PTAPUS.Byte
#define PTAPUS_PTAPUS0                  _PTAPUS.Bits.PTAPUS0
#define PTAPUS_PTAPUS1                  _PTAPUS.Bits.PTAPUS1
#define PTAPUS_PTAPUS2                  _PTAPUS.Bits.PTAPUS2
#define PTAPUS_PTAPUS3                  _PTAPUS.Bits.PTAPUS3
#define PTAPUS_PTAPUS4                  _PTAPUS.Bits.PTAPUS4
#define PTAPUS_PTAPUS5                  _PTAPUS.Bits.PTAPUS5
#define PTAPUS_PTAPUS6                  _PTAPUS.Bits.PTAPUS6
#define PTAPUS_PTAPUS7                  _PTAPUS.Bits.PTAPUS7

#define PTAPUS_PTAPUS0_MASK             1
#define PTAPUS_PTAPUS1_MASK             2
#define PTAPUS_PTAPUS2_MASK             4
#define PTAPUS_PTAPUS3_MASK             8
#define PTAPUS_PTAPUS4_MASK             16
#define PTAPUS_PTAPUS5_MASK             32
#define PTAPUS_PTAPUS6_MASK             64
#define PTAPUS_PTAPUS7_MASK             128


/*** PTADS - Port A Drive Strength Selection Register; 0xFFFF8102 ***/
typedef union {
  byte Byte;
  struct {
    byte PTADS0      :1;                                       /* Output Drive Strength Selection for Port A Bit 0 */
    byte PTADS1      :1;                                       /* Output Drive Strength Selection for Port A Bit 1 */
    byte PTADS2      :1;                                       /* Output Drive Strength Selection for Port A Bit 2 */
    byte PTADS3      :1;                                       /* Output Drive Strength Selection for Port A Bit 3 */
    byte PTADS4      :1;                                       /* Output Drive Strength Selection for Port A Bit 4 */
    byte PTADS5      :1;                                       /* Output Drive Strength Selection for Port A Bit 5 */
    byte PTADS6      :1;                                       /* Output Drive Strength Selection for Port A Bit 6 */
    byte PTADS7      :1;                                       /* Output Drive Strength Selection for Port A Bit 7 */
  } Bits;
} PTADSSTR;
extern volatile PTADSSTR _PTADS @0xFFFF8102;
#define PTADS                           _PTADS.Byte
#define PTADS_PTADS0                    _PTADS.Bits.PTADS0
#define PTADS_PTADS1                    _PTADS.Bits.PTADS1
#define PTADS_PTADS2                    _PTADS.Bits.PTADS2
#define PTADS_PTADS3                    _PTADS.Bits.PTADS3
#define PTADS_PTADS4                    _PTADS.Bits.PTADS4
#define PTADS_PTADS5                    _PTADS.Bits.PTADS5
#define PTADS_PTADS6                    _PTADS.Bits.PTADS6
#define PTADS_PTADS7                    _PTADS.Bits.PTADS7

#define PTADS_PTADS0_MASK               1
#define PTADS_PTADS1_MASK               2
#define PTADS_PTADS2_MASK               4
#define PTADS_PTADS3_MASK               8
#define PTADS_PTADS4_MASK               16
#define PTADS_PTADS5_MASK               32
#define PTADS_PTADS6_MASK               64
#define PTADS_PTADS7_MASK               128


/*** PTASRE - Port A Slew Rate Enable Register; 0xFFFF8103 ***/
typedef union {
  byte Byte;
  struct {
    byte PTASRE0     :1;                                       /* Output Slew Rate Enable for Port A Bit 0 */
    byte PTASRE1     :1;                                       /* Output Slew Rate Enable for Port A Bit 1 */
    byte PTASRE2     :1;                                       /* Output Slew Rate Enable for Port A Bit 2 */
    byte PTASRE3     :1;                                       /* Output Slew Rate Enable for Port A Bit 3 */
    byte PTASRE4     :1;                                       /* Output Slew Rate Enable for Port A Bit 4 */
    byte PTASRE5     :1;                                       /* Output Slew Rate Enable for Port A Bit 5 */
    byte PTASRE6     :1;                                       /* Output Slew Rate Enable for Port A Bit 6 */
    byte PTASRE7     :1;                                       /* Output Slew Rate Enable for Port A Bit 7 */
  } Bits;
} PTASRESTR;
extern volatile PTASRESTR _PTASRE @0xFFFF8103;
#define PTASRE                          _PTASRE.Byte
#define PTASRE_PTASRE0                  _PTASRE.Bits.PTASRE0
#define PTASRE_PTASRE1                  _PTASRE.Bits.PTASRE1
#define PTASRE_PTASRE2                  _PTASRE.Bits.PTASRE2
#define PTASRE_PTASRE3                  _PTASRE.Bits.PTASRE3
#define PTASRE_PTASRE4                  _PTASRE.Bits.PTASRE4
#define PTASRE_PTASRE5                  _PTASRE.Bits.PTASRE5
#define PTASRE_PTASRE6                  _PTASRE.Bits.PTASRE6
#define PTASRE_PTASRE7                  _PTASRE.Bits.PTASRE7

#define PTASRE_PTASRE0_MASK             1
#define PTASRE_PTASRE1_MASK             2
#define PTASRE_PTASRE2_MASK             4
#define PTASRE_PTASRE3_MASK             8
#define PTASRE_PTASRE4_MASK             16
#define PTASRE_PTASRE5_MASK             32
#define PTASRE_PTASRE6_MASK             64
#define PTASRE_PTASRE7_MASK             128


/*** PTAPFE - Port A Passive Filter Enable Register; 0xFFFF8104 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTAPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTAPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTAPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTAPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTAPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTAPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTAPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTAPFESTR;
extern volatile PTAPFESTR _PTAPFE @0xFFFF8104;
#define PTAPFE                          _PTAPFE.Byte
#define PTAPFE_PTAPFE0                  _PTAPFE.Bits.PTAPFE0
#define PTAPFE_PTAPFE1                  _PTAPFE.Bits.PTAPFE1
#define PTAPFE_PTAPFE2                  _PTAPFE.Bits.PTAPFE2
#define PTAPFE_PTAPFE3                  _PTAPFE.Bits.PTAPFE3
#define PTAPFE_PTAPFE4                  _PTAPFE.Bits.PTAPFE4
#define PTAPFE_PTAPFE5                  _PTAPFE.Bits.PTAPFE5
#define PTAPFE_PTAPFE6                  _PTAPFE.Bits.PTAPFE6
#define PTAPFE_PTAPFE7                  _PTAPFE.Bits.PTAPFE7

#define PTAPFE_PTAPFE0_MASK             1
#define PTAPFE_PTAPFE1_MASK             2
#define PTAPFE_PTAPFE2_MASK             4
#define PTAPFE_PTAPFE3_MASK             8
#define PTAPFE_PTAPFE4_MASK             16
#define PTAPFE_PTAPFE5_MASK             32
#define PTAPFE_PTAPFE6_MASK             64
#define PTAPFE_PTAPFE7_MASK             128


/*** PTAIC - Port A Interrupt Control Register; 0xFFFF8105 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTAIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTADMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTAICSTR;
extern volatile PTAICSTR _PTAIC @0xFFFF8105;
#define PTAIC                           _PTAIC.Byte
#define PTAIC_PTAMOD                    _PTAIC.Bits.PTAMOD
#define PTAIC_PTAIE                     _PTAIC.Bits.PTAIE
#define PTAIC_PTADMAEN                  _PTAIC.Bits.PTADMAEN

#define PTAIC_PTAMOD_MASK               1
#define PTAIC_PTAIE_MASK                2
#define PTAIC_PTADMAEN_MASK             128


/*** PTAIPE - Port A Interrupt Pin Enable Register; 0xFFFF8106 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTAIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTAIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTAIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTAIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTAIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTAIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTAIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTAIPESTR;
extern volatile PTAIPESTR _PTAIPE @0xFFFF8106;
#define PTAIPE                          _PTAIPE.Byte
#define PTAIPE_PTAIPE0                  _PTAIPE.Bits.PTAIPE0
#define PTAIPE_PTAIPE1                  _PTAIPE.Bits.PTAIPE1
#define PTAIPE_PTAIPE2                  _PTAIPE.Bits.PTAIPE2
#define PTAIPE_PTAIPE3                  _PTAIPE.Bits.PTAIPE3
#define PTAIPE_PTAIPE4                  _PTAIPE.Bits.PTAIPE4
#define PTAIPE_PTAIPE5                  _PTAIPE.Bits.PTAIPE5
#define PTAIPE_PTAIPE6                  _PTAIPE.Bits.PTAIPE6
#define PTAIPE_PTAIPE7                  _PTAIPE.Bits.PTAIPE7

#define PTAIPE_PTAIPE0_MASK             1
#define PTAIPE_PTAIPE1_MASK             2
#define PTAIPE_PTAIPE2_MASK             4
#define PTAIPE_PTAIPE3_MASK             8
#define PTAIPE_PTAIPE4_MASK             16
#define PTAIPE_PTAIPE5_MASK             32
#define PTAIPE_PTAIPE6_MASK             64
#define PTAIPE_PTAIPE7_MASK             128


/*** PTAIF - Port A Interrupt Flag Register; 0xFFFF8107 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTAIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTAIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTAIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTAIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTAIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTAIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTAIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTAIFSTR;
extern volatile PTAIFSTR _PTAIF @0xFFFF8107;
#define PTAIF                           _PTAIF.Byte
#define PTAIF_PTAIF0                    _PTAIF.Bits.PTAIF0
#define PTAIF_PTAIF1                    _PTAIF.Bits.PTAIF1
#define PTAIF_PTAIF2                    _PTAIF.Bits.PTAIF2
#define PTAIF_PTAIF3                    _PTAIF.Bits.PTAIF3
#define PTAIF_PTAIF4                    _PTAIF.Bits.PTAIF4
#define PTAIF_PTAIF5                    _PTAIF.Bits.PTAIF5
#define PTAIF_PTAIF6                    _PTAIF.Bits.PTAIF6
#define PTAIF_PTAIF7                    _PTAIF.Bits.PTAIF7

#define PTAIF_PTAIF0_MASK               1
#define PTAIF_PTAIF1_MASK               2
#define PTAIF_PTAIF2_MASK               4
#define PTAIF_PTAIF3_MASK               8
#define PTAIF_PTAIF4_MASK               16
#define PTAIF_PTAIF5_MASK               32
#define PTAIF_PTAIF6_MASK               64
#define PTAIF_PTAIF7_MASK               128


/*** PTAIES - Port A Interrupt Edge Select Register; 0xFFFF8108 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTAEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTAEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTAEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTAEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTAEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTAEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTAEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTAIESSTR;
extern volatile PTAIESSTR _PTAIES @0xFFFF8108;
#define PTAIES                          _PTAIES.Byte
#define PTAIES_PTAEDG0                  _PTAIES.Bits.PTAEDG0
#define PTAIES_PTAEDG1                  _PTAIES.Bits.PTAEDG1
#define PTAIES_PTAEDG2                  _PTAIES.Bits.PTAEDG2
#define PTAIES_PTAEDG3                  _PTAIES.Bits.PTAEDG3
#define PTAIES_PTAEDG4                  _PTAIES.Bits.PTAEDG4
#define PTAIES_PTAEDG5                  _PTAIES.Bits.PTAEDG5
#define PTAIES_PTAEDG6                  _PTAIES.Bits.PTAEDG6
#define PTAIES_PTAEDG7                  _PTAIES.Bits.PTAEDG7

#define PTAIES_PTAEDG0_MASK             1
#define PTAIES_PTAEDG1_MASK             2
#define PTAIES_PTAEDG2_MASK             4
#define PTAIES_PTAEDG3_MASK             8
#define PTAIES_PTAEDG4_MASK             16
#define PTAIES_PTAEDG5_MASK             32
#define PTAIES_PTAEDG6_MASK             64
#define PTAIES_PTAEDG7_MASK             128


/*** PTADFE - Port A Digital Filter Enable Register; 0xFFFF8109 ***/
typedef union {
  byte Byte;
  struct {
    byte PTADFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTADFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTADFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTADFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTADFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTADFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTADFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTADFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTADFESTR;
extern volatile PTADFESTR _PTADFE @0xFFFF8109;
#define PTADFE                          _PTADFE.Byte
#define PTADFE_PTADFE0                  _PTADFE.Bits.PTADFE0
#define PTADFE_PTADFE1                  _PTADFE.Bits.PTADFE1
#define PTADFE_PTADFE2                  _PTADFE.Bits.PTADFE2
#define PTADFE_PTADFE3                  _PTADFE.Bits.PTADFE3
#define PTADFE_PTADFE4                  _PTADFE.Bits.PTADFE4
#define PTADFE_PTADFE5                  _PTADFE.Bits.PTADFE5
#define PTADFE_PTADFE6                  _PTADFE.Bits.PTADFE6
#define PTADFE_PTADFE7                  _PTADFE.Bits.PTADFE7

#define PTADFE_PTADFE0_MASK             1
#define PTADFE_PTADFE1_MASK             2
#define PTADFE_PTADFE2_MASK             4
#define PTADFE_PTADFE3_MASK             8
#define PTADFE_PTADFE4_MASK             16
#define PTADFE_PTADFE5_MASK             32
#define PTADFE_PTADFE6_MASK             64
#define PTADFE_PTADFE7_MASK             128


/*** PTADFC - Port A Digital Filter Control Register; 0xFFFF810A ***/
typedef union {
  byte Byte;
  struct {
    byte PTAFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTAFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTAFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTAFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTAFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTACLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTAFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTADFCSTR;
extern volatile PTADFCSTR _PTADFC @0xFFFF810A;
#define PTADFC                          _PTADFC.Byte
#define PTADFC_PTAFF0                   _PTADFC.Bits.PTAFF0
#define PTADFC_PTAFF1                   _PTADFC.Bits.PTAFF1
#define PTADFC_PTAFF2                   _PTADFC.Bits.PTAFF2
#define PTADFC_PTAFF3                   _PTADFC.Bits.PTAFF3
#define PTADFC_PTAFF4                   _PTADFC.Bits.PTAFF4
#define PTADFC_PTACLKS                  _PTADFC.Bits.PTACLKS
#define PTADFC_PTAFF                    _PTADFC.MergedBits.grpPTAFF

#define PTADFC_PTAFF0_MASK              1
#define PTADFC_PTAFF1_MASK              2
#define PTADFC_PTAFF2_MASK              4
#define PTADFC_PTAFF3_MASK              8
#define PTADFC_PTAFF4_MASK              16
#define PTADFC_PTACLKS_MASK             128
#define PTADFC_PTAFF_MASK               31
#define PTADFC_PTAFF_BITNUM             0


/*** PTBPUE - Port B Pulling Enable Register; 0xFFFF8110 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPUE0     :1;                                       /* Internal Pulling Enable for Port B Bit 0 */
    byte PTBPUE1     :1;                                       /* Internal Pulling Enable for Port B Bit 1 */
    byte PTBPUE2     :1;                                       /* Internal Pulling Enable for Port B Bit 2 */
    byte PTBPUE3     :1;                                       /* Internal Pulling Enable for Port B Bit 3 */
    byte PTBPUE4     :1;                                       /* Internal Pulling Enable for Port B Bit 4 */
    byte PTBPUE5     :1;                                       /* Internal Pulling Enable for Port B Bit 5 */
    byte PTBPUE6     :1;                                       /* Internal Pulling Enable for Port B Bit 6 */
    byte PTBPUE7     :1;                                       /* Internal Pulling Enable for Port B Bit 7 */
  } Bits;
} PTBPUESTR;
extern volatile PTBPUESTR _PTBPUE @0xFFFF8110;
#define PTBPUE                          _PTBPUE.Byte
#define PTBPUE_PTBPUE0                  _PTBPUE.Bits.PTBPUE0
#define PTBPUE_PTBPUE1                  _PTBPUE.Bits.PTBPUE1
#define PTBPUE_PTBPUE2                  _PTBPUE.Bits.PTBPUE2
#define PTBPUE_PTBPUE3                  _PTBPUE.Bits.PTBPUE3
#define PTBPUE_PTBPUE4                  _PTBPUE.Bits.PTBPUE4
#define PTBPUE_PTBPUE5                  _PTBPUE.Bits.PTBPUE5
#define PTBPUE_PTBPUE6                  _PTBPUE.Bits.PTBPUE6
#define PTBPUE_PTBPUE7                  _PTBPUE.Bits.PTBPUE7

#define PTBPUE_PTBPUE0_MASK             1
#define PTBPUE_PTBPUE1_MASK             2
#define PTBPUE_PTBPUE2_MASK             4
#define PTBPUE_PTBPUE3_MASK             8
#define PTBPUE_PTBPUE4_MASK             16
#define PTBPUE_PTBPUE5_MASK             32
#define PTBPUE_PTBPUE6_MASK             64
#define PTBPUE_PTBPUE7_MASK             128


/*** PTBPUS - Port B Pullup/Pulldown Select Register; 0xFFFF8111 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTBPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTBPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTBPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTBPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTBPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTBPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTBPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTBPUSSTR;
extern volatile PTBPUSSTR _PTBPUS @0xFFFF8111;
#define PTBPUS                          _PTBPUS.Byte
#define PTBPUS_PTBPUS0                  _PTBPUS.Bits.PTBPUS0
#define PTBPUS_PTBPUS1                  _PTBPUS.Bits.PTBPUS1
#define PTBPUS_PTBPUS2                  _PTBPUS.Bits.PTBPUS2
#define PTBPUS_PTBPUS3                  _PTBPUS.Bits.PTBPUS3
#define PTBPUS_PTBPUS4                  _PTBPUS.Bits.PTBPUS4
#define PTBPUS_PTBPUS5                  _PTBPUS.Bits.PTBPUS5
#define PTBPUS_PTBPUS6                  _PTBPUS.Bits.PTBPUS6
#define PTBPUS_PTBPUS7                  _PTBPUS.Bits.PTBPUS7

#define PTBPUS_PTBPUS0_MASK             1
#define PTBPUS_PTBPUS1_MASK             2
#define PTBPUS_PTBPUS2_MASK             4
#define PTBPUS_PTBPUS3_MASK             8
#define PTBPUS_PTBPUS4_MASK             16
#define PTBPUS_PTBPUS5_MASK             32
#define PTBPUS_PTBPUS6_MASK             64
#define PTBPUS_PTBPUS7_MASK             128


/*** PTBDS - Port B Drive Strength Selection Register; 0xFFFF8112 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBDS0      :1;                                       /* Output Drive Strength Selection for Port B Bit 0 */
    byte PTBDS1      :1;                                       /* Output Drive Strength Selection for Port B Bit 1 */
    byte PTBDS2      :1;                                       /* Output Drive Strength Selection for Port B Bit 2 */
    byte PTBDS3      :1;                                       /* Output Drive Strength Selection for Port B Bit 3 */
    byte PTBDS4      :1;                                       /* Output Drive Strength Selection for Port B Bit 4 */
    byte PTBDS5      :1;                                       /* Output Drive Strength Selection for Port B Bit 5 */
    byte PTBDS6      :1;                                       /* Output Drive Strength Selection for Port B Bit 6 */
    byte PTBDS7      :1;                                       /* Output Drive Strength Selection for Port B Bit 7 */
  } Bits;
} PTBDSSTR;
extern volatile PTBDSSTR _PTBDS @0xFFFF8112;
#define PTBDS                           _PTBDS.Byte
#define PTBDS_PTBDS0                    _PTBDS.Bits.PTBDS0
#define PTBDS_PTBDS1                    _PTBDS.Bits.PTBDS1
#define PTBDS_PTBDS2                    _PTBDS.Bits.PTBDS2
#define PTBDS_PTBDS3                    _PTBDS.Bits.PTBDS3
#define PTBDS_PTBDS4                    _PTBDS.Bits.PTBDS4
#define PTBDS_PTBDS5                    _PTBDS.Bits.PTBDS5
#define PTBDS_PTBDS6                    _PTBDS.Bits.PTBDS6
#define PTBDS_PTBDS7                    _PTBDS.Bits.PTBDS7

#define PTBDS_PTBDS0_MASK               1
#define PTBDS_PTBDS1_MASK               2
#define PTBDS_PTBDS2_MASK               4
#define PTBDS_PTBDS3_MASK               8
#define PTBDS_PTBDS4_MASK               16
#define PTBDS_PTBDS5_MASK               32
#define PTBDS_PTBDS6_MASK               64
#define PTBDS_PTBDS7_MASK               128


/*** PTBSRE - Port B Slew Rate Enable Register; 0xFFFF8113 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBSRE0     :1;                                       /* Output Slew Rate Enable for Port B Bit 0 */
    byte PTBSRE1     :1;                                       /* Output Slew Rate Enable for Port B Bit 1 */
    byte PTBSRE2     :1;                                       /* Output Slew Rate Enable for Port B Bit 2 */
    byte PTBSRE3     :1;                                       /* Output Slew Rate Enable for Port B Bit 3 */
    byte PTBSRE4     :1;                                       /* Output Slew Rate Enable for Port B Bit 4 */
    byte PTBSRE5     :1;                                       /* Output Slew Rate Enable for Port B Bit 5 */
    byte PTBSRE6     :1;                                       /* Output Slew Rate Enable for Port B Bit 6 */
    byte PTBSRE7     :1;                                       /* Output Slew Rate Enable for Port B Bit 7 */
  } Bits;
} PTBSRESTR;
extern volatile PTBSRESTR _PTBSRE @0xFFFF8113;
#define PTBSRE                          _PTBSRE.Byte
#define PTBSRE_PTBSRE0                  _PTBSRE.Bits.PTBSRE0
#define PTBSRE_PTBSRE1                  _PTBSRE.Bits.PTBSRE1
#define PTBSRE_PTBSRE2                  _PTBSRE.Bits.PTBSRE2
#define PTBSRE_PTBSRE3                  _PTBSRE.Bits.PTBSRE3
#define PTBSRE_PTBSRE4                  _PTBSRE.Bits.PTBSRE4
#define PTBSRE_PTBSRE5                  _PTBSRE.Bits.PTBSRE5
#define PTBSRE_PTBSRE6                  _PTBSRE.Bits.PTBSRE6
#define PTBSRE_PTBSRE7                  _PTBSRE.Bits.PTBSRE7

#define PTBSRE_PTBSRE0_MASK             1
#define PTBSRE_PTBSRE1_MASK             2
#define PTBSRE_PTBSRE2_MASK             4
#define PTBSRE_PTBSRE3_MASK             8
#define PTBSRE_PTBSRE4_MASK             16
#define PTBSRE_PTBSRE5_MASK             32
#define PTBSRE_PTBSRE6_MASK             64
#define PTBSRE_PTBSRE7_MASK             128


/*** PTBPFE - Port B Passive Filter Enable Register; 0xFFFF8114 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTBPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTBPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTBPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTBPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTBPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTBPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTBPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTBPFESTR;
extern volatile PTBPFESTR _PTBPFE @0xFFFF8114;
#define PTBPFE                          _PTBPFE.Byte
#define PTBPFE_PTBPFE0                  _PTBPFE.Bits.PTBPFE0
#define PTBPFE_PTBPFE1                  _PTBPFE.Bits.PTBPFE1
#define PTBPFE_PTBPFE2                  _PTBPFE.Bits.PTBPFE2
#define PTBPFE_PTBPFE3                  _PTBPFE.Bits.PTBPFE3
#define PTBPFE_PTBPFE4                  _PTBPFE.Bits.PTBPFE4
#define PTBPFE_PTBPFE5                  _PTBPFE.Bits.PTBPFE5
#define PTBPFE_PTBPFE6                  _PTBPFE.Bits.PTBPFE6
#define PTBPFE_PTBPFE7                  _PTBPFE.Bits.PTBPFE7

#define PTBPFE_PTBPFE0_MASK             1
#define PTBPFE_PTBPFE1_MASK             2
#define PTBPFE_PTBPFE2_MASK             4
#define PTBPFE_PTBPFE3_MASK             8
#define PTBPFE_PTBPFE4_MASK             16
#define PTBPFE_PTBPFE5_MASK             32
#define PTBPFE_PTBPFE6_MASK             64
#define PTBPFE_PTBPFE7_MASK             128


/*** PTBIC - Port B Interrupt Control Register; 0xFFFF8115 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTBIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTBDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTBICSTR;
extern volatile PTBICSTR _PTBIC @0xFFFF8115;
#define PTBIC                           _PTBIC.Byte
#define PTBIC_PTBMOD                    _PTBIC.Bits.PTBMOD
#define PTBIC_PTBIE                     _PTBIC.Bits.PTBIE
#define PTBIC_PTBDMAEN                  _PTBIC.Bits.PTBDMAEN

#define PTBIC_PTBMOD_MASK               1
#define PTBIC_PTBIE_MASK                2
#define PTBIC_PTBDMAEN_MASK             128


/*** PTBIPE - Port B Interrupt Pin Enable Register; 0xFFFF8116 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTBIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTBIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTBIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTBIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTBIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTBIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTBIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTBIPESTR;
extern volatile PTBIPESTR _PTBIPE @0xFFFF8116;
#define PTBIPE                          _PTBIPE.Byte
#define PTBIPE_PTBIPE0                  _PTBIPE.Bits.PTBIPE0
#define PTBIPE_PTBIPE1                  _PTBIPE.Bits.PTBIPE1
#define PTBIPE_PTBIPE2                  _PTBIPE.Bits.PTBIPE2
#define PTBIPE_PTBIPE3                  _PTBIPE.Bits.PTBIPE3
#define PTBIPE_PTBIPE4                  _PTBIPE.Bits.PTBIPE4
#define PTBIPE_PTBIPE5                  _PTBIPE.Bits.PTBIPE5
#define PTBIPE_PTBIPE6                  _PTBIPE.Bits.PTBIPE6
#define PTBIPE_PTBIPE7                  _PTBIPE.Bits.PTBIPE7

#define PTBIPE_PTBIPE0_MASK             1
#define PTBIPE_PTBIPE1_MASK             2
#define PTBIPE_PTBIPE2_MASK             4
#define PTBIPE_PTBIPE3_MASK             8
#define PTBIPE_PTBIPE4_MASK             16
#define PTBIPE_PTBIPE5_MASK             32
#define PTBIPE_PTBIPE6_MASK             64
#define PTBIPE_PTBIPE7_MASK             128


/*** PTBIF - Port B Interrupt Flag Register; 0xFFFF8117 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTBIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTBIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTBIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTBIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTBIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTBIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTBIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTBIFSTR;
extern volatile PTBIFSTR _PTBIF @0xFFFF8117;
#define PTBIF                           _PTBIF.Byte
#define PTBIF_PTBIF0                    _PTBIF.Bits.PTBIF0
#define PTBIF_PTBIF1                    _PTBIF.Bits.PTBIF1
#define PTBIF_PTBIF2                    _PTBIF.Bits.PTBIF2
#define PTBIF_PTBIF3                    _PTBIF.Bits.PTBIF3
#define PTBIF_PTBIF4                    _PTBIF.Bits.PTBIF4
#define PTBIF_PTBIF5                    _PTBIF.Bits.PTBIF5
#define PTBIF_PTBIF6                    _PTBIF.Bits.PTBIF6
#define PTBIF_PTBIF7                    _PTBIF.Bits.PTBIF7

#define PTBIF_PTBIF0_MASK               1
#define PTBIF_PTBIF1_MASK               2
#define PTBIF_PTBIF2_MASK               4
#define PTBIF_PTBIF3_MASK               8
#define PTBIF_PTBIF4_MASK               16
#define PTBIF_PTBIF5_MASK               32
#define PTBIF_PTBIF6_MASK               64
#define PTBIF_PTBIF7_MASK               128


/*** PTBIES - Port B Interrupt Edge Select Register; 0xFFFF8118 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTBEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTBEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTBEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTBEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTBEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTBEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTBEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTBIESSTR;
extern volatile PTBIESSTR _PTBIES @0xFFFF8118;
#define PTBIES                          _PTBIES.Byte
#define PTBIES_PTBEDG0                  _PTBIES.Bits.PTBEDG0
#define PTBIES_PTBEDG1                  _PTBIES.Bits.PTBEDG1
#define PTBIES_PTBEDG2                  _PTBIES.Bits.PTBEDG2
#define PTBIES_PTBEDG3                  _PTBIES.Bits.PTBEDG3
#define PTBIES_PTBEDG4                  _PTBIES.Bits.PTBEDG4
#define PTBIES_PTBEDG5                  _PTBIES.Bits.PTBEDG5
#define PTBIES_PTBEDG6                  _PTBIES.Bits.PTBEDG6
#define PTBIES_PTBEDG7                  _PTBIES.Bits.PTBEDG7

#define PTBIES_PTBEDG0_MASK             1
#define PTBIES_PTBEDG1_MASK             2
#define PTBIES_PTBEDG2_MASK             4
#define PTBIES_PTBEDG3_MASK             8
#define PTBIES_PTBEDG4_MASK             16
#define PTBIES_PTBEDG5_MASK             32
#define PTBIES_PTBEDG6_MASK             64
#define PTBIES_PTBEDG7_MASK             128


/*** PTBDFE - Port B Digital Filter Enable Register; 0xFFFF8119 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTBDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTBDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTBDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTBDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTBDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTBDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTBDFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTBDFESTR;
extern volatile PTBDFESTR _PTBDFE @0xFFFF8119;
#define PTBDFE                          _PTBDFE.Byte
#define PTBDFE_PTBDFE0                  _PTBDFE.Bits.PTBDFE0
#define PTBDFE_PTBDFE1                  _PTBDFE.Bits.PTBDFE1
#define PTBDFE_PTBDFE2                  _PTBDFE.Bits.PTBDFE2
#define PTBDFE_PTBDFE3                  _PTBDFE.Bits.PTBDFE3
#define PTBDFE_PTBDFE4                  _PTBDFE.Bits.PTBDFE4
#define PTBDFE_PTBDFE5                  _PTBDFE.Bits.PTBDFE5
#define PTBDFE_PTBDFE6                  _PTBDFE.Bits.PTBDFE6
#define PTBDFE_PTBDFE7                  _PTBDFE.Bits.PTBDFE7

#define PTBDFE_PTBDFE0_MASK             1
#define PTBDFE_PTBDFE1_MASK             2
#define PTBDFE_PTBDFE2_MASK             4
#define PTBDFE_PTBDFE3_MASK             8
#define PTBDFE_PTBDFE4_MASK             16
#define PTBDFE_PTBDFE5_MASK             32
#define PTBDFE_PTBDFE6_MASK             64
#define PTBDFE_PTBDFE7_MASK             128


/*** PTBDFC - Port B Digital Filter Control Register; 0xFFFF811A ***/
typedef union {
  byte Byte;
  struct {
    byte PTBFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTBFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTBFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTBFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTBFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTBCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTBFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTBDFCSTR;
extern volatile PTBDFCSTR _PTBDFC @0xFFFF811A;
#define PTBDFC                          _PTBDFC.Byte
#define PTBDFC_PTBFF0                   _PTBDFC.Bits.PTBFF0
#define PTBDFC_PTBFF1                   _PTBDFC.Bits.PTBFF1
#define PTBDFC_PTBFF2                   _PTBDFC.Bits.PTBFF2
#define PTBDFC_PTBFF3                   _PTBDFC.Bits.PTBFF3
#define PTBDFC_PTBFF4                   _PTBDFC.Bits.PTBFF4
#define PTBDFC_PTBCLKS                  _PTBDFC.Bits.PTBCLKS
#define PTBDFC_PTBFF                    _PTBDFC.MergedBits.grpPTBFF

#define PTBDFC_PTBFF0_MASK              1
#define PTBDFC_PTBFF1_MASK              2
#define PTBDFC_PTBFF2_MASK              4
#define PTBDFC_PTBFF3_MASK              8
#define PTBDFC_PTBFF4_MASK              16
#define PTBDFC_PTBCLKS_MASK             128
#define PTBDFC_PTBFF_MASK               31
#define PTBDFC_PTBFF_BITNUM             0


/*** PTCPUE - Port C Pulling Enable Register; 0xFFFF8120 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPUE0     :1;                                       /* Internal Pulling Enable for Port C Bit 0 */
    byte PTCPUE1     :1;                                       /* Internal Pulling Enable for Port C Bit 1 */
    byte PTCPUE2     :1;                                       /* Internal Pulling Enable for Port C Bit 2 */
    byte PTCPUE3     :1;                                       /* Internal Pulling Enable for Port C Bit 3 */
    byte PTCPUE4     :1;                                       /* Internal Pulling Enable for Port C Bit 4 */
    byte PTCPUE5     :1;                                       /* Internal Pulling Enable for Port C Bit 5 */
    byte PTCPUE6     :1;                                       /* Internal Pulling Enable for Port C Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCPUE :7;
    byte         :1;
  } MergedBits;
} PTCPUESTR;
extern volatile PTCPUESTR _PTCPUE @0xFFFF8120;
#define PTCPUE                          _PTCPUE.Byte
#define PTCPUE_PTCPUE0                  _PTCPUE.Bits.PTCPUE0
#define PTCPUE_PTCPUE1                  _PTCPUE.Bits.PTCPUE1
#define PTCPUE_PTCPUE2                  _PTCPUE.Bits.PTCPUE2
#define PTCPUE_PTCPUE3                  _PTCPUE.Bits.PTCPUE3
#define PTCPUE_PTCPUE4                  _PTCPUE.Bits.PTCPUE4
#define PTCPUE_PTCPUE5                  _PTCPUE.Bits.PTCPUE5
#define PTCPUE_PTCPUE6                  _PTCPUE.Bits.PTCPUE6
#define PTCPUE_PTCPUE                   _PTCPUE.MergedBits.grpPTCPUE

#define PTCPUE_PTCPUE0_MASK             1
#define PTCPUE_PTCPUE1_MASK             2
#define PTCPUE_PTCPUE2_MASK             4
#define PTCPUE_PTCPUE3_MASK             8
#define PTCPUE_PTCPUE4_MASK             16
#define PTCPUE_PTCPUE5_MASK             32
#define PTCPUE_PTCPUE6_MASK             64
#define PTCPUE_PTCPUE_MASK              127
#define PTCPUE_PTCPUE_BITNUM            0


/*** PTCPUS - Port C Pullup/Pulldown Select Register; 0xFFFF8121 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTCPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTCPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTCPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTCPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTCPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTCPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCPUS :7;
    byte         :1;
  } MergedBits;
} PTCPUSSTR;
extern volatile PTCPUSSTR _PTCPUS @0xFFFF8121;
#define PTCPUS                          _PTCPUS.Byte
#define PTCPUS_PTCPUS0                  _PTCPUS.Bits.PTCPUS0
#define PTCPUS_PTCPUS1                  _PTCPUS.Bits.PTCPUS1
#define PTCPUS_PTCPUS2                  _PTCPUS.Bits.PTCPUS2
#define PTCPUS_PTCPUS3                  _PTCPUS.Bits.PTCPUS3
#define PTCPUS_PTCPUS4                  _PTCPUS.Bits.PTCPUS4
#define PTCPUS_PTCPUS5                  _PTCPUS.Bits.PTCPUS5
#define PTCPUS_PTCPUS6                  _PTCPUS.Bits.PTCPUS6
#define PTCPUS_PTCPUS                   _PTCPUS.MergedBits.grpPTCPUS

#define PTCPUS_PTCPUS0_MASK             1
#define PTCPUS_PTCPUS1_MASK             2
#define PTCPUS_PTCPUS2_MASK             4
#define PTCPUS_PTCPUS3_MASK             8
#define PTCPUS_PTCPUS4_MASK             16
#define PTCPUS_PTCPUS5_MASK             32
#define PTCPUS_PTCPUS6_MASK             64
#define PTCPUS_PTCPUS_MASK              127
#define PTCPUS_PTCPUS_BITNUM            0


/*** PTCDS - Port C Drive Strength Selection Register; 0xFFFF8122 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCDS0      :1;                                       /* Output Drive Strength Selection for Port C Bit 0 */
    byte PTCDS1      :1;                                       /* Output Drive Strength Selection for Port C Bit 1 */
    byte PTCDS2      :1;                                       /* Output Drive Strength Selection for Port C Bit 2 */
    byte PTCDS3      :1;                                       /* Output Drive Strength Selection for Port C Bit 3 */
    byte PTCDS4      :1;                                       /* Output Drive Strength Selection for Port C Bit 4 */
    byte PTCDS5      :1;                                       /* Output Drive Strength Selection for Port C Bit 5 */
    byte PTCDS6      :1;                                       /* Output Drive Strength Selection for Port C Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCDS :7;
    byte         :1;
  } MergedBits;
} PTCDSSTR;
extern volatile PTCDSSTR _PTCDS @0xFFFF8122;
#define PTCDS                           _PTCDS.Byte
#define PTCDS_PTCDS0                    _PTCDS.Bits.PTCDS0
#define PTCDS_PTCDS1                    _PTCDS.Bits.PTCDS1
#define PTCDS_PTCDS2                    _PTCDS.Bits.PTCDS2
#define PTCDS_PTCDS3                    _PTCDS.Bits.PTCDS3
#define PTCDS_PTCDS4                    _PTCDS.Bits.PTCDS4
#define PTCDS_PTCDS5                    _PTCDS.Bits.PTCDS5
#define PTCDS_PTCDS6                    _PTCDS.Bits.PTCDS6
#define PTCDS_PTCDS                     _PTCDS.MergedBits.grpPTCDS

#define PTCDS_PTCDS0_MASK               1
#define PTCDS_PTCDS1_MASK               2
#define PTCDS_PTCDS2_MASK               4
#define PTCDS_PTCDS3_MASK               8
#define PTCDS_PTCDS4_MASK               16
#define PTCDS_PTCDS5_MASK               32
#define PTCDS_PTCDS6_MASK               64
#define PTCDS_PTCDS_MASK                127
#define PTCDS_PTCDS_BITNUM              0


/*** PTCSRE - Port C Slew Rate Enable Register; 0xFFFF8123 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCSRE0     :1;                                       /* Output Slew Rate Enable for Port C Bit 0 */
    byte PTCSRE1     :1;                                       /* Output Slew Rate Enable for Port C Bit 1 */
    byte PTCSRE2     :1;                                       /* Output Slew Rate Enable for Port C Bit 2 */
    byte PTCSRE3     :1;                                       /* Output Slew Rate Enable for Port C Bit 3 */
    byte PTCSRE4     :1;                                       /* Output Slew Rate Enable for Port C Bit 4 */
    byte PTCSRE5     :1;                                       /* Output Slew Rate Enable for Port C Bit 5 */
    byte PTCSRE6     :1;                                       /* Output Slew Rate Enable for Port C Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCSRE :7;
    byte         :1;
  } MergedBits;
} PTCSRESTR;
extern volatile PTCSRESTR _PTCSRE @0xFFFF8123;
#define PTCSRE                          _PTCSRE.Byte
#define PTCSRE_PTCSRE0                  _PTCSRE.Bits.PTCSRE0
#define PTCSRE_PTCSRE1                  _PTCSRE.Bits.PTCSRE1
#define PTCSRE_PTCSRE2                  _PTCSRE.Bits.PTCSRE2
#define PTCSRE_PTCSRE3                  _PTCSRE.Bits.PTCSRE3
#define PTCSRE_PTCSRE4                  _PTCSRE.Bits.PTCSRE4
#define PTCSRE_PTCSRE5                  _PTCSRE.Bits.PTCSRE5
#define PTCSRE_PTCSRE6                  _PTCSRE.Bits.PTCSRE6
#define PTCSRE_PTCSRE                   _PTCSRE.MergedBits.grpPTCSRE

#define PTCSRE_PTCSRE0_MASK             1
#define PTCSRE_PTCSRE1_MASK             2
#define PTCSRE_PTCSRE2_MASK             4
#define PTCSRE_PTCSRE3_MASK             8
#define PTCSRE_PTCSRE4_MASK             16
#define PTCSRE_PTCSRE5_MASK             32
#define PTCSRE_PTCSRE6_MASK             64
#define PTCSRE_PTCSRE_MASK              127
#define PTCSRE_PTCSRE_BITNUM            0


/*** PTCPFE - Port C Passive Filter Enable Register; 0xFFFF8124 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTCPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTCPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTCPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTCPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTCPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTCPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCPFE :7;
    byte         :1;
  } MergedBits;
} PTCPFESTR;
extern volatile PTCPFESTR _PTCPFE @0xFFFF8124;
#define PTCPFE                          _PTCPFE.Byte
#define PTCPFE_PTCPFE0                  _PTCPFE.Bits.PTCPFE0
#define PTCPFE_PTCPFE1                  _PTCPFE.Bits.PTCPFE1
#define PTCPFE_PTCPFE2                  _PTCPFE.Bits.PTCPFE2
#define PTCPFE_PTCPFE3                  _PTCPFE.Bits.PTCPFE3
#define PTCPFE_PTCPFE4                  _PTCPFE.Bits.PTCPFE4
#define PTCPFE_PTCPFE5                  _PTCPFE.Bits.PTCPFE5
#define PTCPFE_PTCPFE6                  _PTCPFE.Bits.PTCPFE6
#define PTCPFE_PTCPFE                   _PTCPFE.MergedBits.grpPTCPFE

#define PTCPFE_PTCPFE0_MASK             1
#define PTCPFE_PTCPFE1_MASK             2
#define PTCPFE_PTCPFE2_MASK             4
#define PTCPFE_PTCPFE3_MASK             8
#define PTCPFE_PTCPFE4_MASK             16
#define PTCPFE_PTCPFE5_MASK             32
#define PTCPFE_PTCPFE6_MASK             64
#define PTCPFE_PTCPFE_MASK              127
#define PTCPFE_PTCPFE_BITNUM            0


/*** PTCIC - Port C Interrupt Control Register; 0xFFFF8125 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTCIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTCDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTCICSTR;
extern volatile PTCICSTR _PTCIC @0xFFFF8125;
#define PTCIC                           _PTCIC.Byte
#define PTCIC_PTCMOD                    _PTCIC.Bits.PTCMOD
#define PTCIC_PTCIE                     _PTCIC.Bits.PTCIE
#define PTCIC_PTCDMAEN                  _PTCIC.Bits.PTCDMAEN

#define PTCIC_PTCMOD_MASK               1
#define PTCIC_PTCIE_MASK                2
#define PTCIC_PTCDMAEN_MASK             128


/*** PTCIPE - Port C Interrupt Pin Enable Register; 0xFFFF8126 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTCIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTCIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTCIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTCIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTCIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTCIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCIPE :7;
    byte         :1;
  } MergedBits;
} PTCIPESTR;
extern volatile PTCIPESTR _PTCIPE @0xFFFF8126;
#define PTCIPE                          _PTCIPE.Byte
#define PTCIPE_PTCIPE0                  _PTCIPE.Bits.PTCIPE0
#define PTCIPE_PTCIPE1                  _PTCIPE.Bits.PTCIPE1
#define PTCIPE_PTCIPE2                  _PTCIPE.Bits.PTCIPE2
#define PTCIPE_PTCIPE3                  _PTCIPE.Bits.PTCIPE3
#define PTCIPE_PTCIPE4                  _PTCIPE.Bits.PTCIPE4
#define PTCIPE_PTCIPE5                  _PTCIPE.Bits.PTCIPE5
#define PTCIPE_PTCIPE6                  _PTCIPE.Bits.PTCIPE6
#define PTCIPE_PTCIPE                   _PTCIPE.MergedBits.grpPTCIPE

#define PTCIPE_PTCIPE0_MASK             1
#define PTCIPE_PTCIPE1_MASK             2
#define PTCIPE_PTCIPE2_MASK             4
#define PTCIPE_PTCIPE3_MASK             8
#define PTCIPE_PTCIPE4_MASK             16
#define PTCIPE_PTCIPE5_MASK             32
#define PTCIPE_PTCIPE6_MASK             64
#define PTCIPE_PTCIPE_MASK              127
#define PTCIPE_PTCIPE_BITNUM            0


/*** PTCIF - Port C Interrupt Flag Register; 0xFFFF8127 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTCIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTCIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTCIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTCIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTCIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTCIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCIF :7;
    byte         :1;
  } MergedBits;
} PTCIFSTR;
extern volatile PTCIFSTR _PTCIF @0xFFFF8127;
#define PTCIF                           _PTCIF.Byte
#define PTCIF_PTCIF0                    _PTCIF.Bits.PTCIF0
#define PTCIF_PTCIF1                    _PTCIF.Bits.PTCIF1
#define PTCIF_PTCIF2                    _PTCIF.Bits.PTCIF2
#define PTCIF_PTCIF3                    _PTCIF.Bits.PTCIF3
#define PTCIF_PTCIF4                    _PTCIF.Bits.PTCIF4
#define PTCIF_PTCIF5                    _PTCIF.Bits.PTCIF5
#define PTCIF_PTCIF6                    _PTCIF.Bits.PTCIF6
#define PTCIF_PTCIF                     _PTCIF.MergedBits.grpPTCIF

#define PTCIF_PTCIF0_MASK               1
#define PTCIF_PTCIF1_MASK               2
#define PTCIF_PTCIF2_MASK               4
#define PTCIF_PTCIF3_MASK               8
#define PTCIF_PTCIF4_MASK               16
#define PTCIF_PTCIF5_MASK               32
#define PTCIF_PTCIF6_MASK               64
#define PTCIF_PTCIF_MASK                127
#define PTCIF_PTCIF_BITNUM              0


/*** PTCIES - Port C Interrupt Edge Select Register; 0xFFFF8128 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTCEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTCEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTCEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTCEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTCEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTCEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCEDG :7;
    byte         :1;
  } MergedBits;
} PTCIESSTR;
extern volatile PTCIESSTR _PTCIES @0xFFFF8128;
#define PTCIES                          _PTCIES.Byte
#define PTCIES_PTCEDG0                  _PTCIES.Bits.PTCEDG0
#define PTCIES_PTCEDG1                  _PTCIES.Bits.PTCEDG1
#define PTCIES_PTCEDG2                  _PTCIES.Bits.PTCEDG2
#define PTCIES_PTCEDG3                  _PTCIES.Bits.PTCEDG3
#define PTCIES_PTCEDG4                  _PTCIES.Bits.PTCEDG4
#define PTCIES_PTCEDG5                  _PTCIES.Bits.PTCEDG5
#define PTCIES_PTCEDG6                  _PTCIES.Bits.PTCEDG6
#define PTCIES_PTCEDG                   _PTCIES.MergedBits.grpPTCEDG

#define PTCIES_PTCEDG0_MASK             1
#define PTCIES_PTCEDG1_MASK             2
#define PTCIES_PTCEDG2_MASK             4
#define PTCIES_PTCEDG3_MASK             8
#define PTCIES_PTCEDG4_MASK             16
#define PTCIES_PTCEDG5_MASK             32
#define PTCIES_PTCEDG6_MASK             64
#define PTCIES_PTCEDG_MASK              127
#define PTCIES_PTCEDG_BITNUM            0


/*** PTCDFE - Port C Digital Filter Enable Register; 0xFFFF8129 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTCDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTCDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTCDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTCDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTCDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTCDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTCDFE :7;
    byte         :1;
  } MergedBits;
} PTCDFESTR;
extern volatile PTCDFESTR _PTCDFE @0xFFFF8129;
#define PTCDFE                          _PTCDFE.Byte
#define PTCDFE_PTCDFE0                  _PTCDFE.Bits.PTCDFE0
#define PTCDFE_PTCDFE1                  _PTCDFE.Bits.PTCDFE1
#define PTCDFE_PTCDFE2                  _PTCDFE.Bits.PTCDFE2
#define PTCDFE_PTCDFE3                  _PTCDFE.Bits.PTCDFE3
#define PTCDFE_PTCDFE4                  _PTCDFE.Bits.PTCDFE4
#define PTCDFE_PTCDFE5                  _PTCDFE.Bits.PTCDFE5
#define PTCDFE_PTCDFE6                  _PTCDFE.Bits.PTCDFE6
#define PTCDFE_PTCDFE                   _PTCDFE.MergedBits.grpPTCDFE

#define PTCDFE_PTCDFE0_MASK             1
#define PTCDFE_PTCDFE1_MASK             2
#define PTCDFE_PTCDFE2_MASK             4
#define PTCDFE_PTCDFE3_MASK             8
#define PTCDFE_PTCDFE4_MASK             16
#define PTCDFE_PTCDFE5_MASK             32
#define PTCDFE_PTCDFE6_MASK             64
#define PTCDFE_PTCDFE_MASK              127
#define PTCDFE_PTCDFE_BITNUM            0


/*** PTCDFC - Port C Digital Filter Control Register; 0xFFFF812A ***/
typedef union {
  byte Byte;
  struct {
    byte PTCFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTCFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTCFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTCFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTCFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTCCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTCFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTCDFCSTR;
extern volatile PTCDFCSTR _PTCDFC @0xFFFF812A;
#define PTCDFC                          _PTCDFC.Byte
#define PTCDFC_PTCFF0                   _PTCDFC.Bits.PTCFF0
#define PTCDFC_PTCFF1                   _PTCDFC.Bits.PTCFF1
#define PTCDFC_PTCFF2                   _PTCDFC.Bits.PTCFF2
#define PTCDFC_PTCFF3                   _PTCDFC.Bits.PTCFF3
#define PTCDFC_PTCFF4                   _PTCDFC.Bits.PTCFF4
#define PTCDFC_PTCCLKS                  _PTCDFC.Bits.PTCCLKS
#define PTCDFC_PTCFF                    _PTCDFC.MergedBits.grpPTCFF

#define PTCDFC_PTCFF0_MASK              1
#define PTCDFC_PTCFF1_MASK              2
#define PTCDFC_PTCFF2_MASK              4
#define PTCDFC_PTCFF3_MASK              8
#define PTCDFC_PTCFF4_MASK              16
#define PTCDFC_PTCCLKS_MASK             128
#define PTCDFC_PTCFF_MASK               31
#define PTCDFC_PTCFF_BITNUM             0


/*** PTDPUE - Port D Pulling Enable Register; 0xFFFF8130 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPUE0     :1;                                       /* Internal Pulling Enable for Port D Bit 0 */
    byte PTDPUE1     :1;                                       /* Internal Pulling Enable for Port D Bit 1 */
    byte PTDPUE2     :1;                                       /* Internal Pulling Enable for Port D Bit 2 */
    byte PTDPUE3     :1;                                       /* Internal Pulling Enable for Port D Bit 3 */
    byte PTDPUE4     :1;                                       /* Internal Pulling Enable for Port D Bit 4 */
    byte PTDPUE5     :1;                                       /* Internal Pulling Enable for Port D Bit 5 */
    byte PTDPUE6     :1;                                       /* Internal Pulling Enable for Port D Bit 6 */
    byte PTDPUE7     :1;                                       /* Internal Pulling Enable for Port D Bit 7 */
  } Bits;
} PTDPUESTR;
extern volatile PTDPUESTR _PTDPUE @0xFFFF8130;
#define PTDPUE                          _PTDPUE.Byte
#define PTDPUE_PTDPUE0                  _PTDPUE.Bits.PTDPUE0
#define PTDPUE_PTDPUE1                  _PTDPUE.Bits.PTDPUE1
#define PTDPUE_PTDPUE2                  _PTDPUE.Bits.PTDPUE2
#define PTDPUE_PTDPUE3                  _PTDPUE.Bits.PTDPUE3
#define PTDPUE_PTDPUE4                  _PTDPUE.Bits.PTDPUE4
#define PTDPUE_PTDPUE5                  _PTDPUE.Bits.PTDPUE5
#define PTDPUE_PTDPUE6                  _PTDPUE.Bits.PTDPUE6
#define PTDPUE_PTDPUE7                  _PTDPUE.Bits.PTDPUE7

#define PTDPUE_PTDPUE0_MASK             1
#define PTDPUE_PTDPUE1_MASK             2
#define PTDPUE_PTDPUE2_MASK             4
#define PTDPUE_PTDPUE3_MASK             8
#define PTDPUE_PTDPUE4_MASK             16
#define PTDPUE_PTDPUE5_MASK             32
#define PTDPUE_PTDPUE6_MASK             64
#define PTDPUE_PTDPUE7_MASK             128


/*** PTDPUS - Port D Pullup/Pulldown Select Register; 0xFFFF8131 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTDPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTDPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTDPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTDPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTDPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTDPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTDPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTDPUSSTR;
extern volatile PTDPUSSTR _PTDPUS @0xFFFF8131;
#define PTDPUS                          _PTDPUS.Byte
#define PTDPUS_PTDPUS0                  _PTDPUS.Bits.PTDPUS0
#define PTDPUS_PTDPUS1                  _PTDPUS.Bits.PTDPUS1
#define PTDPUS_PTDPUS2                  _PTDPUS.Bits.PTDPUS2
#define PTDPUS_PTDPUS3                  _PTDPUS.Bits.PTDPUS3
#define PTDPUS_PTDPUS4                  _PTDPUS.Bits.PTDPUS4
#define PTDPUS_PTDPUS5                  _PTDPUS.Bits.PTDPUS5
#define PTDPUS_PTDPUS6                  _PTDPUS.Bits.PTDPUS6
#define PTDPUS_PTDPUS7                  _PTDPUS.Bits.PTDPUS7

#define PTDPUS_PTDPUS0_MASK             1
#define PTDPUS_PTDPUS1_MASK             2
#define PTDPUS_PTDPUS2_MASK             4
#define PTDPUS_PTDPUS3_MASK             8
#define PTDPUS_PTDPUS4_MASK             16
#define PTDPUS_PTDPUS5_MASK             32
#define PTDPUS_PTDPUS6_MASK             64
#define PTDPUS_PTDPUS7_MASK             128


/*** PTDDS - Port D Drive Strength Selection Register; 0xFFFF8132 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDS0      :1;                                       /* Output Drive Strength Selection for Port D Bit 0 */
    byte PTDDS1      :1;                                       /* Output Drive Strength Selection for Port D Bit 1 */
    byte PTDDS2      :1;                                       /* Output Drive Strength Selection for Port D Bit 2 */
    byte PTDDS3      :1;                                       /* Output Drive Strength Selection for Port D Bit 3 */
    byte PTDDS4      :1;                                       /* Output Drive Strength Selection for Port D Bit 4 */
    byte PTDDS5      :1;                                       /* Output Drive Strength Selection for Port D Bit 5 */
    byte PTDDS6      :1;                                       /* Output Drive Strength Selection for Port D Bit 6 */
    byte PTDDS7      :1;                                       /* Output Drive Strength Selection for Port D Bit 7 */
  } Bits;
} PTDDSSTR;
extern volatile PTDDSSTR _PTDDS @0xFFFF8132;
#define PTDDS                           _PTDDS.Byte
#define PTDDS_PTDDS0                    _PTDDS.Bits.PTDDS0
#define PTDDS_PTDDS1                    _PTDDS.Bits.PTDDS1
#define PTDDS_PTDDS2                    _PTDDS.Bits.PTDDS2
#define PTDDS_PTDDS3                    _PTDDS.Bits.PTDDS3
#define PTDDS_PTDDS4                    _PTDDS.Bits.PTDDS4
#define PTDDS_PTDDS5                    _PTDDS.Bits.PTDDS5
#define PTDDS_PTDDS6                    _PTDDS.Bits.PTDDS6
#define PTDDS_PTDDS7                    _PTDDS.Bits.PTDDS7

#define PTDDS_PTDDS0_MASK               1
#define PTDDS_PTDDS1_MASK               2
#define PTDDS_PTDDS2_MASK               4
#define PTDDS_PTDDS3_MASK               8
#define PTDDS_PTDDS4_MASK               16
#define PTDDS_PTDDS5_MASK               32
#define PTDDS_PTDDS6_MASK               64
#define PTDDS_PTDDS7_MASK               128


/*** PTDSRE - Port D Slew Rate Enable Register; 0xFFFF8133 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDSRE0     :1;                                       /* Output Slew Rate Enable for Port D Bit 0 */
    byte PTDSRE1     :1;                                       /* Output Slew Rate Enable for Port D Bit 1 */
    byte PTDSRE2     :1;                                       /* Output Slew Rate Enable for Port D Bit 2 */
    byte PTDSRE3     :1;                                       /* Output Slew Rate Enable for Port D Bit 3 */
    byte PTDSRE4     :1;                                       /* Output Slew Rate Enable for Port D Bit 4 */
    byte PTDSRE5     :1;                                       /* Output Slew Rate Enable for Port D Bit 5 */
    byte PTDSRE6     :1;                                       /* Output Slew Rate Enable for Port D Bit 6 */
    byte PTDSRE7     :1;                                       /* Output Slew Rate Enable for Port D Bit 7 */
  } Bits;
} PTDSRESTR;
extern volatile PTDSRESTR _PTDSRE @0xFFFF8133;
#define PTDSRE                          _PTDSRE.Byte
#define PTDSRE_PTDSRE0                  _PTDSRE.Bits.PTDSRE0
#define PTDSRE_PTDSRE1                  _PTDSRE.Bits.PTDSRE1
#define PTDSRE_PTDSRE2                  _PTDSRE.Bits.PTDSRE2
#define PTDSRE_PTDSRE3                  _PTDSRE.Bits.PTDSRE3
#define PTDSRE_PTDSRE4                  _PTDSRE.Bits.PTDSRE4
#define PTDSRE_PTDSRE5                  _PTDSRE.Bits.PTDSRE5
#define PTDSRE_PTDSRE6                  _PTDSRE.Bits.PTDSRE6
#define PTDSRE_PTDSRE7                  _PTDSRE.Bits.PTDSRE7

#define PTDSRE_PTDSRE0_MASK             1
#define PTDSRE_PTDSRE1_MASK             2
#define PTDSRE_PTDSRE2_MASK             4
#define PTDSRE_PTDSRE3_MASK             8
#define PTDSRE_PTDSRE4_MASK             16
#define PTDSRE_PTDSRE5_MASK             32
#define PTDSRE_PTDSRE6_MASK             64
#define PTDSRE_PTDSRE7_MASK             128


/*** PTDPFE - Port D Passive Filter Enable Register; 0xFFFF8134 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTDPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTDPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTDPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTDPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTDPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTDPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTDPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTDPFESTR;
extern volatile PTDPFESTR _PTDPFE @0xFFFF8134;
#define PTDPFE                          _PTDPFE.Byte
#define PTDPFE_PTDPFE0                  _PTDPFE.Bits.PTDPFE0
#define PTDPFE_PTDPFE1                  _PTDPFE.Bits.PTDPFE1
#define PTDPFE_PTDPFE2                  _PTDPFE.Bits.PTDPFE2
#define PTDPFE_PTDPFE3                  _PTDPFE.Bits.PTDPFE3
#define PTDPFE_PTDPFE4                  _PTDPFE.Bits.PTDPFE4
#define PTDPFE_PTDPFE5                  _PTDPFE.Bits.PTDPFE5
#define PTDPFE_PTDPFE6                  _PTDPFE.Bits.PTDPFE6
#define PTDPFE_PTDPFE7                  _PTDPFE.Bits.PTDPFE7

#define PTDPFE_PTDPFE0_MASK             1
#define PTDPFE_PTDPFE1_MASK             2
#define PTDPFE_PTDPFE2_MASK             4
#define PTDPFE_PTDPFE3_MASK             8
#define PTDPFE_PTDPFE4_MASK             16
#define PTDPFE_PTDPFE5_MASK             32
#define PTDPFE_PTDPFE6_MASK             64
#define PTDPFE_PTDPFE7_MASK             128


/*** PTDIC - Port D Interrupt Control Register; 0xFFFF8135 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTDIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTDDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTDICSTR;
extern volatile PTDICSTR _PTDIC @0xFFFF8135;
#define PTDIC                           _PTDIC.Byte
#define PTDIC_PTDMOD                    _PTDIC.Bits.PTDMOD
#define PTDIC_PTDIE                     _PTDIC.Bits.PTDIE
#define PTDIC_PTDDMAEN                  _PTDIC.Bits.PTDDMAEN

#define PTDIC_PTDMOD_MASK               1
#define PTDIC_PTDIE_MASK                2
#define PTDIC_PTDDMAEN_MASK             128


/*** PTDIPE - Port D Interrupt Pin Enable Register; 0xFFFF8136 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTDIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTDIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTDIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTDIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTDIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTDIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTDIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTDIPESTR;
extern volatile PTDIPESTR _PTDIPE @0xFFFF8136;
#define PTDIPE                          _PTDIPE.Byte
#define PTDIPE_PTDIPE0                  _PTDIPE.Bits.PTDIPE0
#define PTDIPE_PTDIPE1                  _PTDIPE.Bits.PTDIPE1
#define PTDIPE_PTDIPE2                  _PTDIPE.Bits.PTDIPE2
#define PTDIPE_PTDIPE3                  _PTDIPE.Bits.PTDIPE3
#define PTDIPE_PTDIPE4                  _PTDIPE.Bits.PTDIPE4
#define PTDIPE_PTDIPE5                  _PTDIPE.Bits.PTDIPE5
#define PTDIPE_PTDIPE6                  _PTDIPE.Bits.PTDIPE6
#define PTDIPE_PTDIPE7                  _PTDIPE.Bits.PTDIPE7

#define PTDIPE_PTDIPE0_MASK             1
#define PTDIPE_PTDIPE1_MASK             2
#define PTDIPE_PTDIPE2_MASK             4
#define PTDIPE_PTDIPE3_MASK             8
#define PTDIPE_PTDIPE4_MASK             16
#define PTDIPE_PTDIPE5_MASK             32
#define PTDIPE_PTDIPE6_MASK             64
#define PTDIPE_PTDIPE7_MASK             128


/*** PTDIF - Port D Interrupt Flag Register; 0xFFFF8137 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTDIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTDIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTDIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTDIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTDIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTDIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTDIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTDIFSTR;
extern volatile PTDIFSTR _PTDIF @0xFFFF8137;
#define PTDIF                           _PTDIF.Byte
#define PTDIF_PTDIF0                    _PTDIF.Bits.PTDIF0
#define PTDIF_PTDIF1                    _PTDIF.Bits.PTDIF1
#define PTDIF_PTDIF2                    _PTDIF.Bits.PTDIF2
#define PTDIF_PTDIF3                    _PTDIF.Bits.PTDIF3
#define PTDIF_PTDIF4                    _PTDIF.Bits.PTDIF4
#define PTDIF_PTDIF5                    _PTDIF.Bits.PTDIF5
#define PTDIF_PTDIF6                    _PTDIF.Bits.PTDIF6
#define PTDIF_PTDIF7                    _PTDIF.Bits.PTDIF7

#define PTDIF_PTDIF0_MASK               1
#define PTDIF_PTDIF1_MASK               2
#define PTDIF_PTDIF2_MASK               4
#define PTDIF_PTDIF3_MASK               8
#define PTDIF_PTDIF4_MASK               16
#define PTDIF_PTDIF5_MASK               32
#define PTDIF_PTDIF6_MASK               64
#define PTDIF_PTDIF7_MASK               128


/*** PTDIES - Port D Interrupt Edge Select Register; 0xFFFF8138 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTDEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTDEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTDEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTDEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTDEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTDEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTDEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTDIESSTR;
extern volatile PTDIESSTR _PTDIES @0xFFFF8138;
#define PTDIES                          _PTDIES.Byte
#define PTDIES_PTDEDG0                  _PTDIES.Bits.PTDEDG0
#define PTDIES_PTDEDG1                  _PTDIES.Bits.PTDEDG1
#define PTDIES_PTDEDG2                  _PTDIES.Bits.PTDEDG2
#define PTDIES_PTDEDG3                  _PTDIES.Bits.PTDEDG3
#define PTDIES_PTDEDG4                  _PTDIES.Bits.PTDEDG4
#define PTDIES_PTDEDG5                  _PTDIES.Bits.PTDEDG5
#define PTDIES_PTDEDG6                  _PTDIES.Bits.PTDEDG6
#define PTDIES_PTDEDG7                  _PTDIES.Bits.PTDEDG7

#define PTDIES_PTDEDG0_MASK             1
#define PTDIES_PTDEDG1_MASK             2
#define PTDIES_PTDEDG2_MASK             4
#define PTDIES_PTDEDG3_MASK             8
#define PTDIES_PTDEDG4_MASK             16
#define PTDIES_PTDEDG5_MASK             32
#define PTDIES_PTDEDG6_MASK             64
#define PTDIES_PTDEDG7_MASK             128


/*** PTDDFE - Port D Digital Filter Enable Register; 0xFFFF8139 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTDDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTDDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTDDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTDDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTDDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTDDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTDDFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTDDFESTR;
extern volatile PTDDFESTR _PTDDFE @0xFFFF8139;
#define PTDDFE                          _PTDDFE.Byte
#define PTDDFE_PTDDFE0                  _PTDDFE.Bits.PTDDFE0
#define PTDDFE_PTDDFE1                  _PTDDFE.Bits.PTDDFE1
#define PTDDFE_PTDDFE2                  _PTDDFE.Bits.PTDDFE2
#define PTDDFE_PTDDFE3                  _PTDDFE.Bits.PTDDFE3
#define PTDDFE_PTDDFE4                  _PTDDFE.Bits.PTDDFE4
#define PTDDFE_PTDDFE5                  _PTDDFE.Bits.PTDDFE5
#define PTDDFE_PTDDFE6                  _PTDDFE.Bits.PTDDFE6
#define PTDDFE_PTDDFE7                  _PTDDFE.Bits.PTDDFE7

#define PTDDFE_PTDDFE0_MASK             1
#define PTDDFE_PTDDFE1_MASK             2
#define PTDDFE_PTDDFE2_MASK             4
#define PTDDFE_PTDDFE3_MASK             8
#define PTDDFE_PTDDFE4_MASK             16
#define PTDDFE_PTDDFE5_MASK             32
#define PTDDFE_PTDDFE6_MASK             64
#define PTDDFE_PTDDFE7_MASK             128


/*** PTDDFC - Port D Digital Filter Control Register; 0xFFFF813A ***/
typedef union {
  byte Byte;
  struct {
    byte PTDFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTDFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTDFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTDFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTDFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTDCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTDFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTDDFCSTR;
extern volatile PTDDFCSTR _PTDDFC @0xFFFF813A;
#define PTDDFC                          _PTDDFC.Byte
#define PTDDFC_PTDFF0                   _PTDDFC.Bits.PTDFF0
#define PTDDFC_PTDFF1                   _PTDDFC.Bits.PTDFF1
#define PTDDFC_PTDFF2                   _PTDDFC.Bits.PTDFF2
#define PTDDFC_PTDFF3                   _PTDDFC.Bits.PTDFF3
#define PTDDFC_PTDFF4                   _PTDDFC.Bits.PTDFF4
#define PTDDFC_PTDCLKS                  _PTDDFC.Bits.PTDCLKS
#define PTDDFC_PTDFF                    _PTDDFC.MergedBits.grpPTDFF

#define PTDDFC_PTDFF0_MASK              1
#define PTDDFC_PTDFF1_MASK              2
#define PTDDFC_PTDFF2_MASK              4
#define PTDDFC_PTDFF3_MASK              8
#define PTDDFC_PTDFF4_MASK              16
#define PTDDFC_PTDCLKS_MASK             128
#define PTDDFC_PTDFF_MASK               31
#define PTDDFC_PTDFF_BITNUM             0


/*** PTEPUE - Port E Pulling Enable Register; 0xFFFF8140 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPUE0     :1;                                       /* Internal Pulling Enable for Port E Bit 0 */
    byte PTEPUE1     :1;                                       /* Internal Pulling Enable for Port E Bit 1 */
    byte PTEPUE2     :1;                                       /* Internal Pulling Enable for Port E Bit 2 */
    byte PTEPUE3     :1;                                       /* Internal Pulling Enable for Port E Bit 3 */
    byte PTEPUE4     :1;                                       /* Internal Pulling Enable for Port E Bit 4 */
    byte PTEPUE5     :1;                                       /* Internal Pulling Enable for Port E Bit 5 */
    byte PTEPUE6     :1;                                       /* Internal Pulling Enable for Port E Bit 6 */
    byte PTEPUE7     :1;                                       /* Internal Pulling Enable for Port E Bit 7 */
  } Bits;
} PTEPUESTR;
extern volatile PTEPUESTR _PTEPUE @0xFFFF8140;
#define PTEPUE                          _PTEPUE.Byte
#define PTEPUE_PTEPUE0                  _PTEPUE.Bits.PTEPUE0
#define PTEPUE_PTEPUE1                  _PTEPUE.Bits.PTEPUE1
#define PTEPUE_PTEPUE2                  _PTEPUE.Bits.PTEPUE2
#define PTEPUE_PTEPUE3                  _PTEPUE.Bits.PTEPUE3
#define PTEPUE_PTEPUE4                  _PTEPUE.Bits.PTEPUE4
#define PTEPUE_PTEPUE5                  _PTEPUE.Bits.PTEPUE5
#define PTEPUE_PTEPUE6                  _PTEPUE.Bits.PTEPUE6
#define PTEPUE_PTEPUE7                  _PTEPUE.Bits.PTEPUE7

#define PTEPUE_PTEPUE0_MASK             1
#define PTEPUE_PTEPUE1_MASK             2
#define PTEPUE_PTEPUE2_MASK             4
#define PTEPUE_PTEPUE3_MASK             8
#define PTEPUE_PTEPUE4_MASK             16
#define PTEPUE_PTEPUE5_MASK             32
#define PTEPUE_PTEPUE6_MASK             64
#define PTEPUE_PTEPUE7_MASK             128


/*** PTEPUS - Port E Pullup/Pulldown Select Register; 0xFFFF8141 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTEPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTEPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTEPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTEPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTEPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTEPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTEPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTEPUSSTR;
extern volatile PTEPUSSTR _PTEPUS @0xFFFF8141;
#define PTEPUS                          _PTEPUS.Byte
#define PTEPUS_PTEPUS0                  _PTEPUS.Bits.PTEPUS0
#define PTEPUS_PTEPUS1                  _PTEPUS.Bits.PTEPUS1
#define PTEPUS_PTEPUS2                  _PTEPUS.Bits.PTEPUS2
#define PTEPUS_PTEPUS3                  _PTEPUS.Bits.PTEPUS3
#define PTEPUS_PTEPUS4                  _PTEPUS.Bits.PTEPUS4
#define PTEPUS_PTEPUS5                  _PTEPUS.Bits.PTEPUS5
#define PTEPUS_PTEPUS6                  _PTEPUS.Bits.PTEPUS6
#define PTEPUS_PTEPUS7                  _PTEPUS.Bits.PTEPUS7

#define PTEPUS_PTEPUS0_MASK             1
#define PTEPUS_PTEPUS1_MASK             2
#define PTEPUS_PTEPUS2_MASK             4
#define PTEPUS_PTEPUS3_MASK             8
#define PTEPUS_PTEPUS4_MASK             16
#define PTEPUS_PTEPUS5_MASK             32
#define PTEPUS_PTEPUS6_MASK             64
#define PTEPUS_PTEPUS7_MASK             128


/*** PTEDS - Port E Drive Strength Selection Register; 0xFFFF8142 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDS0      :1;                                       /* Output Drive Strength Selection for Port E Bit 0 */
    byte PTEDS1      :1;                                       /* Output Drive Strength Selection for Port E Bit 1 */
    byte PTEDS2      :1;                                       /* Output Drive Strength Selection for Port E Bit 2 */
    byte PTEDS3      :1;                                       /* Output Drive Strength Selection for Port E Bit 3 */
    byte PTEDS4      :1;                                       /* Output Drive Strength Selection for Port E Bit 4 */
    byte PTEDS5      :1;                                       /* Output Drive Strength Selection for Port E Bit 5 */
    byte PTEDS6      :1;                                       /* Output Drive Strength Selection for Port E Bit 6 */
    byte PTEDS7      :1;                                       /* Output Drive Strength Selection for Port E Bit 7 */
  } Bits;
} PTEDSSTR;
extern volatile PTEDSSTR _PTEDS @0xFFFF8142;
#define PTEDS                           _PTEDS.Byte
#define PTEDS_PTEDS0                    _PTEDS.Bits.PTEDS0
#define PTEDS_PTEDS1                    _PTEDS.Bits.PTEDS1
#define PTEDS_PTEDS2                    _PTEDS.Bits.PTEDS2
#define PTEDS_PTEDS3                    _PTEDS.Bits.PTEDS3
#define PTEDS_PTEDS4                    _PTEDS.Bits.PTEDS4
#define PTEDS_PTEDS5                    _PTEDS.Bits.PTEDS5
#define PTEDS_PTEDS6                    _PTEDS.Bits.PTEDS6
#define PTEDS_PTEDS7                    _PTEDS.Bits.PTEDS7

#define PTEDS_PTEDS0_MASK               1
#define PTEDS_PTEDS1_MASK               2
#define PTEDS_PTEDS2_MASK               4
#define PTEDS_PTEDS3_MASK               8
#define PTEDS_PTEDS4_MASK               16
#define PTEDS_PTEDS5_MASK               32
#define PTEDS_PTEDS6_MASK               64
#define PTEDS_PTEDS7_MASK               128


/*** PTESRE - Port E Slew Rate Enable Register; 0xFFFF8143 ***/
typedef union {
  byte Byte;
  struct {
    byte PTESRE0     :1;                                       /* Output Slew Rate Enable for Port E Bit 0 */
    byte PTESRE1     :1;                                       /* Output Slew Rate Enable for Port E Bit 1 */
    byte PTESRE2     :1;                                       /* Output Slew Rate Enable for Port E Bit 2 */
    byte PTESRE3     :1;                                       /* Output Slew Rate Enable for Port E Bit 3 */
    byte PTESRE4     :1;                                       /* Output Slew Rate Enable for Port E Bit 4 */
    byte PTESRE5     :1;                                       /* Output Slew Rate Enable for Port E Bit 5 */
    byte PTESRE6     :1;                                       /* Output Slew Rate Enable for Port E Bit 6 */
    byte PTESRE7     :1;                                       /* Output Slew Rate Enable for Port E Bit 7 */
  } Bits;
} PTESRESTR;
extern volatile PTESRESTR _PTESRE @0xFFFF8143;
#define PTESRE                          _PTESRE.Byte
#define PTESRE_PTESRE0                  _PTESRE.Bits.PTESRE0
#define PTESRE_PTESRE1                  _PTESRE.Bits.PTESRE1
#define PTESRE_PTESRE2                  _PTESRE.Bits.PTESRE2
#define PTESRE_PTESRE3                  _PTESRE.Bits.PTESRE3
#define PTESRE_PTESRE4                  _PTESRE.Bits.PTESRE4
#define PTESRE_PTESRE5                  _PTESRE.Bits.PTESRE5
#define PTESRE_PTESRE6                  _PTESRE.Bits.PTESRE6
#define PTESRE_PTESRE7                  _PTESRE.Bits.PTESRE7

#define PTESRE_PTESRE0_MASK             1
#define PTESRE_PTESRE1_MASK             2
#define PTESRE_PTESRE2_MASK             4
#define PTESRE_PTESRE3_MASK             8
#define PTESRE_PTESRE4_MASK             16
#define PTESRE_PTESRE5_MASK             32
#define PTESRE_PTESRE6_MASK             64
#define PTESRE_PTESRE7_MASK             128


/*** PTEPFE - Port E Passive Filter Enable Register; 0xFFFF8144 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTEPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTEPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTEPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTEPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTEPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTEPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTEPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTEPFESTR;
extern volatile PTEPFESTR _PTEPFE @0xFFFF8144;
#define PTEPFE                          _PTEPFE.Byte
#define PTEPFE_PTEPFE0                  _PTEPFE.Bits.PTEPFE0
#define PTEPFE_PTEPFE1                  _PTEPFE.Bits.PTEPFE1
#define PTEPFE_PTEPFE2                  _PTEPFE.Bits.PTEPFE2
#define PTEPFE_PTEPFE3                  _PTEPFE.Bits.PTEPFE3
#define PTEPFE_PTEPFE4                  _PTEPFE.Bits.PTEPFE4
#define PTEPFE_PTEPFE5                  _PTEPFE.Bits.PTEPFE5
#define PTEPFE_PTEPFE6                  _PTEPFE.Bits.PTEPFE6
#define PTEPFE_PTEPFE7                  _PTEPFE.Bits.PTEPFE7

#define PTEPFE_PTEPFE0_MASK             1
#define PTEPFE_PTEPFE1_MASK             2
#define PTEPFE_PTEPFE2_MASK             4
#define PTEPFE_PTEPFE3_MASK             8
#define PTEPFE_PTEPFE4_MASK             16
#define PTEPFE_PTEPFE5_MASK             32
#define PTEPFE_PTEPFE6_MASK             64
#define PTEPFE_PTEPFE7_MASK             128


/*** PTEIC - Port E Interrupt Control Register; 0xFFFF8145 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTEIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTEDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTEICSTR;
extern volatile PTEICSTR _PTEIC @0xFFFF8145;
#define PTEIC                           _PTEIC.Byte
#define PTEIC_PTEMOD                    _PTEIC.Bits.PTEMOD
#define PTEIC_PTEIE                     _PTEIC.Bits.PTEIE
#define PTEIC_PTEDMAEN                  _PTEIC.Bits.PTEDMAEN

#define PTEIC_PTEMOD_MASK               1
#define PTEIC_PTEIE_MASK                2
#define PTEIC_PTEDMAEN_MASK             128


/*** PTEIPE - Port E Interrupt Pin Enable Register; 0xFFFF8146 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTEIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTEIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTEIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTEIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTEIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTEIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTEIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTEIPESTR;
extern volatile PTEIPESTR _PTEIPE @0xFFFF8146;
#define PTEIPE                          _PTEIPE.Byte
#define PTEIPE_PTEIPE0                  _PTEIPE.Bits.PTEIPE0
#define PTEIPE_PTEIPE1                  _PTEIPE.Bits.PTEIPE1
#define PTEIPE_PTEIPE2                  _PTEIPE.Bits.PTEIPE2
#define PTEIPE_PTEIPE3                  _PTEIPE.Bits.PTEIPE3
#define PTEIPE_PTEIPE4                  _PTEIPE.Bits.PTEIPE4
#define PTEIPE_PTEIPE5                  _PTEIPE.Bits.PTEIPE5
#define PTEIPE_PTEIPE6                  _PTEIPE.Bits.PTEIPE6
#define PTEIPE_PTEIPE7                  _PTEIPE.Bits.PTEIPE7

#define PTEIPE_PTEIPE0_MASK             1
#define PTEIPE_PTEIPE1_MASK             2
#define PTEIPE_PTEIPE2_MASK             4
#define PTEIPE_PTEIPE3_MASK             8
#define PTEIPE_PTEIPE4_MASK             16
#define PTEIPE_PTEIPE5_MASK             32
#define PTEIPE_PTEIPE6_MASK             64
#define PTEIPE_PTEIPE7_MASK             128


/*** PTEIF - Port E Interrupt Flag Register; 0xFFFF8147 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTEIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTEIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTEIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTEIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTEIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTEIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTEIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTEIFSTR;
extern volatile PTEIFSTR _PTEIF @0xFFFF8147;
#define PTEIF                           _PTEIF.Byte
#define PTEIF_PTEIF0                    _PTEIF.Bits.PTEIF0
#define PTEIF_PTEIF1                    _PTEIF.Bits.PTEIF1
#define PTEIF_PTEIF2                    _PTEIF.Bits.PTEIF2
#define PTEIF_PTEIF3                    _PTEIF.Bits.PTEIF3
#define PTEIF_PTEIF4                    _PTEIF.Bits.PTEIF4
#define PTEIF_PTEIF5                    _PTEIF.Bits.PTEIF5
#define PTEIF_PTEIF6                    _PTEIF.Bits.PTEIF6
#define PTEIF_PTEIF7                    _PTEIF.Bits.PTEIF7

#define PTEIF_PTEIF0_MASK               1
#define PTEIF_PTEIF1_MASK               2
#define PTEIF_PTEIF2_MASK               4
#define PTEIF_PTEIF3_MASK               8
#define PTEIF_PTEIF4_MASK               16
#define PTEIF_PTEIF5_MASK               32
#define PTEIF_PTEIF6_MASK               64
#define PTEIF_PTEIF7_MASK               128


/*** PTEIES - Port E Interrupt Edge Select Register; 0xFFFF8148 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTEEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTEEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTEEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTEEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTEEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTEEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTEEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTEIESSTR;
extern volatile PTEIESSTR _PTEIES @0xFFFF8148;
#define PTEIES                          _PTEIES.Byte
#define PTEIES_PTEEDG0                  _PTEIES.Bits.PTEEDG0
#define PTEIES_PTEEDG1                  _PTEIES.Bits.PTEEDG1
#define PTEIES_PTEEDG2                  _PTEIES.Bits.PTEEDG2
#define PTEIES_PTEEDG3                  _PTEIES.Bits.PTEEDG3
#define PTEIES_PTEEDG4                  _PTEIES.Bits.PTEEDG4
#define PTEIES_PTEEDG5                  _PTEIES.Bits.PTEEDG5
#define PTEIES_PTEEDG6                  _PTEIES.Bits.PTEEDG6
#define PTEIES_PTEEDG7                  _PTEIES.Bits.PTEEDG7

#define PTEIES_PTEEDG0_MASK             1
#define PTEIES_PTEEDG1_MASK             2
#define PTEIES_PTEEDG2_MASK             4
#define PTEIES_PTEEDG3_MASK             8
#define PTEIES_PTEEDG4_MASK             16
#define PTEIES_PTEEDG5_MASK             32
#define PTEIES_PTEEDG6_MASK             64
#define PTEIES_PTEEDG7_MASK             128


/*** PTEDFE - Port E Digital Filter Enable Register; 0xFFFF8149 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTEDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTEDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTEDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTEDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTEDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTEDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTEDFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTEDFESTR;
extern volatile PTEDFESTR _PTEDFE @0xFFFF8149;
#define PTEDFE                          _PTEDFE.Byte
#define PTEDFE_PTEDFE0                  _PTEDFE.Bits.PTEDFE0
#define PTEDFE_PTEDFE1                  _PTEDFE.Bits.PTEDFE1
#define PTEDFE_PTEDFE2                  _PTEDFE.Bits.PTEDFE2
#define PTEDFE_PTEDFE3                  _PTEDFE.Bits.PTEDFE3
#define PTEDFE_PTEDFE4                  _PTEDFE.Bits.PTEDFE4
#define PTEDFE_PTEDFE5                  _PTEDFE.Bits.PTEDFE5
#define PTEDFE_PTEDFE6                  _PTEDFE.Bits.PTEDFE6
#define PTEDFE_PTEDFE7                  _PTEDFE.Bits.PTEDFE7

#define PTEDFE_PTEDFE0_MASK             1
#define PTEDFE_PTEDFE1_MASK             2
#define PTEDFE_PTEDFE2_MASK             4
#define PTEDFE_PTEDFE3_MASK             8
#define PTEDFE_PTEDFE4_MASK             16
#define PTEDFE_PTEDFE5_MASK             32
#define PTEDFE_PTEDFE6_MASK             64
#define PTEDFE_PTEDFE7_MASK             128


/*** PTEDFC - Port E Digital Filter Control Register; 0xFFFF814A ***/
typedef union {
  byte Byte;
  struct {
    byte PTEFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTEFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTEFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTEFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTEFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTECLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTEFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTEDFCSTR;
extern volatile PTEDFCSTR _PTEDFC @0xFFFF814A;
#define PTEDFC                          _PTEDFC.Byte
#define PTEDFC_PTEFF0                   _PTEDFC.Bits.PTEFF0
#define PTEDFC_PTEFF1                   _PTEDFC.Bits.PTEFF1
#define PTEDFC_PTEFF2                   _PTEDFC.Bits.PTEFF2
#define PTEDFC_PTEFF3                   _PTEDFC.Bits.PTEFF3
#define PTEDFC_PTEFF4                   _PTEDFC.Bits.PTEFF4
#define PTEDFC_PTECLKS                  _PTEDFC.Bits.PTECLKS
#define PTEDFC_PTEFF                    _PTEDFC.MergedBits.grpPTEFF

#define PTEDFC_PTEFF0_MASK              1
#define PTEDFC_PTEFF1_MASK              2
#define PTEDFC_PTEFF2_MASK              4
#define PTEDFC_PTEFF3_MASK              8
#define PTEDFC_PTEFF4_MASK              16
#define PTEDFC_PTECLKS_MASK             128
#define PTEDFC_PTEFF_MASK               31
#define PTEDFC_PTEFF_BITNUM             0


/*** PTFPUE - Port F Pulling Enable Register; 0xFFFF8150 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFPUE0     :1;                                       /* Internal Pulling Enable for Port F Bit 0 */
    byte PTFPUE1     :1;                                       /* Internal Pulling Enable for Port F Bit 1 */
    byte PTFPUE2     :1;                                       /* Internal Pulling Enable for Port F Bit 2 */
    byte PTFPUE3     :1;                                       /* Internal Pulling Enable for Port F Bit 3 */
    byte PTFPUE4     :1;                                       /* Internal Pulling Enable for Port F Bit 4 */
    byte PTFPUE5     :1;                                       /* Internal Pulling Enable for Port F Bit 5 */
    byte PTFPUE6     :1;                                       /* Internal Pulling Enable for Port F Bit 6 */
    byte PTFPUE7     :1;                                       /* Internal Pulling Enable for Port F Bit 7 */
  } Bits;
} PTFPUESTR;
extern volatile PTFPUESTR _PTFPUE @0xFFFF8150;
#define PTFPUE                          _PTFPUE.Byte
#define PTFPUE_PTFPUE0                  _PTFPUE.Bits.PTFPUE0
#define PTFPUE_PTFPUE1                  _PTFPUE.Bits.PTFPUE1
#define PTFPUE_PTFPUE2                  _PTFPUE.Bits.PTFPUE2
#define PTFPUE_PTFPUE3                  _PTFPUE.Bits.PTFPUE3
#define PTFPUE_PTFPUE4                  _PTFPUE.Bits.PTFPUE4
#define PTFPUE_PTFPUE5                  _PTFPUE.Bits.PTFPUE5
#define PTFPUE_PTFPUE6                  _PTFPUE.Bits.PTFPUE6
#define PTFPUE_PTFPUE7                  _PTFPUE.Bits.PTFPUE7

#define PTFPUE_PTFPUE0_MASK             1
#define PTFPUE_PTFPUE1_MASK             2
#define PTFPUE_PTFPUE2_MASK             4
#define PTFPUE_PTFPUE3_MASK             8
#define PTFPUE_PTFPUE4_MASK             16
#define PTFPUE_PTFPUE5_MASK             32
#define PTFPUE_PTFPUE6_MASK             64
#define PTFPUE_PTFPUE7_MASK             128


/*** PTFPUS - Port F Pullup/Pulldown Select Register; 0xFFFF8151 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTFPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTFPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTFPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTFPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTFPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTFPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTFPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTFPUSSTR;
extern volatile PTFPUSSTR _PTFPUS @0xFFFF8151;
#define PTFPUS                          _PTFPUS.Byte
#define PTFPUS_PTFPUS0                  _PTFPUS.Bits.PTFPUS0
#define PTFPUS_PTFPUS1                  _PTFPUS.Bits.PTFPUS1
#define PTFPUS_PTFPUS2                  _PTFPUS.Bits.PTFPUS2
#define PTFPUS_PTFPUS3                  _PTFPUS.Bits.PTFPUS3
#define PTFPUS_PTFPUS4                  _PTFPUS.Bits.PTFPUS4
#define PTFPUS_PTFPUS5                  _PTFPUS.Bits.PTFPUS5
#define PTFPUS_PTFPUS6                  _PTFPUS.Bits.PTFPUS6
#define PTFPUS_PTFPUS7                  _PTFPUS.Bits.PTFPUS7

#define PTFPUS_PTFPUS0_MASK             1
#define PTFPUS_PTFPUS1_MASK             2
#define PTFPUS_PTFPUS2_MASK             4
#define PTFPUS_PTFPUS3_MASK             8
#define PTFPUS_PTFPUS4_MASK             16
#define PTFPUS_PTFPUS5_MASK             32
#define PTFPUS_PTFPUS6_MASK             64
#define PTFPUS_PTFPUS7_MASK             128


/*** PTFDS - Port F Drive Strength Selection Register; 0xFFFF8152 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFDS0      :1;                                       /* Output Drive Strength Selection for Port F Bit 0 */
    byte PTFDS1      :1;                                       /* Output Drive Strength Selection for Port F Bit 1 */
    byte PTFDS2      :1;                                       /* Output Drive Strength Selection for Port F Bit 2 */
    byte PTFDS3      :1;                                       /* Output Drive Strength Selection for Port F Bit 3 */
    byte PTFDS4      :1;                                       /* Output Drive Strength Selection for Port F Bit 4 */
    byte PTFDS5      :1;                                       /* Output Drive Strength Selection for Port F Bit 5 */
    byte PTFDS6      :1;                                       /* Output Drive Strength Selection for Port F Bit 6 */
    byte PTFDS7      :1;                                       /* Output Drive Strength Selection for Port F Bit 7 */
  } Bits;
} PTFDSSTR;
extern volatile PTFDSSTR _PTFDS @0xFFFF8152;
#define PTFDS                           _PTFDS.Byte
#define PTFDS_PTFDS0                    _PTFDS.Bits.PTFDS0
#define PTFDS_PTFDS1                    _PTFDS.Bits.PTFDS1
#define PTFDS_PTFDS2                    _PTFDS.Bits.PTFDS2
#define PTFDS_PTFDS3                    _PTFDS.Bits.PTFDS3
#define PTFDS_PTFDS4                    _PTFDS.Bits.PTFDS4
#define PTFDS_PTFDS5                    _PTFDS.Bits.PTFDS5
#define PTFDS_PTFDS6                    _PTFDS.Bits.PTFDS6
#define PTFDS_PTFDS7                    _PTFDS.Bits.PTFDS7

#define PTFDS_PTFDS0_MASK               1
#define PTFDS_PTFDS1_MASK               2
#define PTFDS_PTFDS2_MASK               4
#define PTFDS_PTFDS3_MASK               8
#define PTFDS_PTFDS4_MASK               16
#define PTFDS_PTFDS5_MASK               32
#define PTFDS_PTFDS6_MASK               64
#define PTFDS_PTFDS7_MASK               128


/*** PTFSRE - Port F Slew Rate Enable Register; 0xFFFF8153 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFSRE0     :1;                                       /* Output Slew Rate Enable for Port F Bit 0 */
    byte PTFSRE1     :1;                                       /* Output Slew Rate Enable for Port F Bit 1 */
    byte PTFSRE2     :1;                                       /* Output Slew Rate Enable for Port F Bit 2 */
    byte PTFSRE3     :1;                                       /* Output Slew Rate Enable for Port F Bit 3 */
    byte PTFSRE4     :1;                                       /* Output Slew Rate Enable for Port F Bit 4 */
    byte PTFSRE5     :1;                                       /* Output Slew Rate Enable for Port F Bit 5 */
    byte PTFSRE6     :1;                                       /* Output Slew Rate Enable for Port F Bit 6 */
    byte PTFSRE7     :1;                                       /* Output Slew Rate Enable for Port F Bit 7 */
  } Bits;
} PTFSRESTR;
extern volatile PTFSRESTR _PTFSRE @0xFFFF8153;
#define PTFSRE                          _PTFSRE.Byte
#define PTFSRE_PTFSRE0                  _PTFSRE.Bits.PTFSRE0
#define PTFSRE_PTFSRE1                  _PTFSRE.Bits.PTFSRE1
#define PTFSRE_PTFSRE2                  _PTFSRE.Bits.PTFSRE2
#define PTFSRE_PTFSRE3                  _PTFSRE.Bits.PTFSRE3
#define PTFSRE_PTFSRE4                  _PTFSRE.Bits.PTFSRE4
#define PTFSRE_PTFSRE5                  _PTFSRE.Bits.PTFSRE5
#define PTFSRE_PTFSRE6                  _PTFSRE.Bits.PTFSRE6
#define PTFSRE_PTFSRE7                  _PTFSRE.Bits.PTFSRE7

#define PTFSRE_PTFSRE0_MASK             1
#define PTFSRE_PTFSRE1_MASK             2
#define PTFSRE_PTFSRE2_MASK             4
#define PTFSRE_PTFSRE3_MASK             8
#define PTFSRE_PTFSRE4_MASK             16
#define PTFSRE_PTFSRE5_MASK             32
#define PTFSRE_PTFSRE6_MASK             64
#define PTFSRE_PTFSRE7_MASK             128


/*** PTFPFE - Port F Passive Filter Enable Register; 0xFFFF8154 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTFPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTFPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTFPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTFPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTFPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTFPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTFPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTFPFESTR;
extern volatile PTFPFESTR _PTFPFE @0xFFFF8154;
#define PTFPFE                          _PTFPFE.Byte
#define PTFPFE_PTFPFE0                  _PTFPFE.Bits.PTFPFE0
#define PTFPFE_PTFPFE1                  _PTFPFE.Bits.PTFPFE1
#define PTFPFE_PTFPFE2                  _PTFPFE.Bits.PTFPFE2
#define PTFPFE_PTFPFE3                  _PTFPFE.Bits.PTFPFE3
#define PTFPFE_PTFPFE4                  _PTFPFE.Bits.PTFPFE4
#define PTFPFE_PTFPFE5                  _PTFPFE.Bits.PTFPFE5
#define PTFPFE_PTFPFE6                  _PTFPFE.Bits.PTFPFE6
#define PTFPFE_PTFPFE7                  _PTFPFE.Bits.PTFPFE7

#define PTFPFE_PTFPFE0_MASK             1
#define PTFPFE_PTFPFE1_MASK             2
#define PTFPFE_PTFPFE2_MASK             4
#define PTFPFE_PTFPFE3_MASK             8
#define PTFPFE_PTFPFE4_MASK             16
#define PTFPFE_PTFPFE5_MASK             32
#define PTFPFE_PTFPFE6_MASK             64
#define PTFPFE_PTFPFE7_MASK             128


/*** PTFIC - Port F Interrupt Control Register; 0xFFFF8155 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTFIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTFDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTFICSTR;
extern volatile PTFICSTR _PTFIC @0xFFFF8155;
#define PTFIC                           _PTFIC.Byte
#define PTFIC_PTFMOD                    _PTFIC.Bits.PTFMOD
#define PTFIC_PTFIE                     _PTFIC.Bits.PTFIE
#define PTFIC_PTFDMAEN                  _PTFIC.Bits.PTFDMAEN

#define PTFIC_PTFMOD_MASK               1
#define PTFIC_PTFIE_MASK                2
#define PTFIC_PTFDMAEN_MASK             128


/*** PTFIPE - Port F Interrupt Pin Enable Register; 0xFFFF8156 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTFIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTFIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTFIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTFIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTFIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTFIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTFIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTFIPESTR;
extern volatile PTFIPESTR _PTFIPE @0xFFFF8156;
#define PTFIPE                          _PTFIPE.Byte
#define PTFIPE_PTFIPE0                  _PTFIPE.Bits.PTFIPE0
#define PTFIPE_PTFIPE1                  _PTFIPE.Bits.PTFIPE1
#define PTFIPE_PTFIPE2                  _PTFIPE.Bits.PTFIPE2
#define PTFIPE_PTFIPE3                  _PTFIPE.Bits.PTFIPE3
#define PTFIPE_PTFIPE4                  _PTFIPE.Bits.PTFIPE4
#define PTFIPE_PTFIPE5                  _PTFIPE.Bits.PTFIPE5
#define PTFIPE_PTFIPE6                  _PTFIPE.Bits.PTFIPE6
#define PTFIPE_PTFIPE7                  _PTFIPE.Bits.PTFIPE7

#define PTFIPE_PTFIPE0_MASK             1
#define PTFIPE_PTFIPE1_MASK             2
#define PTFIPE_PTFIPE2_MASK             4
#define PTFIPE_PTFIPE3_MASK             8
#define PTFIPE_PTFIPE4_MASK             16
#define PTFIPE_PTFIPE5_MASK             32
#define PTFIPE_PTFIPE6_MASK             64
#define PTFIPE_PTFIPE7_MASK             128


/*** PTFIF - Port F Interrupt Flag Register; 0xFFFF8157 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTFIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTFIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTFIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTFIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTFIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTFIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTFIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTFIFSTR;
extern volatile PTFIFSTR _PTFIF @0xFFFF8157;
#define PTFIF                           _PTFIF.Byte
#define PTFIF_PTFIF0                    _PTFIF.Bits.PTFIF0
#define PTFIF_PTFIF1                    _PTFIF.Bits.PTFIF1
#define PTFIF_PTFIF2                    _PTFIF.Bits.PTFIF2
#define PTFIF_PTFIF3                    _PTFIF.Bits.PTFIF3
#define PTFIF_PTFIF4                    _PTFIF.Bits.PTFIF4
#define PTFIF_PTFIF5                    _PTFIF.Bits.PTFIF5
#define PTFIF_PTFIF6                    _PTFIF.Bits.PTFIF6
#define PTFIF_PTFIF7                    _PTFIF.Bits.PTFIF7

#define PTFIF_PTFIF0_MASK               1
#define PTFIF_PTFIF1_MASK               2
#define PTFIF_PTFIF2_MASK               4
#define PTFIF_PTFIF3_MASK               8
#define PTFIF_PTFIF4_MASK               16
#define PTFIF_PTFIF5_MASK               32
#define PTFIF_PTFIF6_MASK               64
#define PTFIF_PTFIF7_MASK               128


/*** PTFIES - Port F Interrupt Edge Select Register; 0xFFFF8158 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTFEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTFEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTFEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTFEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTFEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTFEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTFEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTFIESSTR;
extern volatile PTFIESSTR _PTFIES @0xFFFF8158;
#define PTFIES                          _PTFIES.Byte
#define PTFIES_PTFEDG0                  _PTFIES.Bits.PTFEDG0
#define PTFIES_PTFEDG1                  _PTFIES.Bits.PTFEDG1
#define PTFIES_PTFEDG2                  _PTFIES.Bits.PTFEDG2
#define PTFIES_PTFEDG3                  _PTFIES.Bits.PTFEDG3
#define PTFIES_PTFEDG4                  _PTFIES.Bits.PTFEDG4
#define PTFIES_PTFEDG5                  _PTFIES.Bits.PTFEDG5
#define PTFIES_PTFEDG6                  _PTFIES.Bits.PTFEDG6
#define PTFIES_PTFEDG7                  _PTFIES.Bits.PTFEDG7

#define PTFIES_PTFEDG0_MASK             1
#define PTFIES_PTFEDG1_MASK             2
#define PTFIES_PTFEDG2_MASK             4
#define PTFIES_PTFEDG3_MASK             8
#define PTFIES_PTFEDG4_MASK             16
#define PTFIES_PTFEDG5_MASK             32
#define PTFIES_PTFEDG6_MASK             64
#define PTFIES_PTFEDG7_MASK             128


/*** PTFDFE - Port F Digital Filter Enable Register; 0xFFFF8159 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTFDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTFDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTFDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTFDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTFDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTFDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTFDFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTFDFESTR;
extern volatile PTFDFESTR _PTFDFE @0xFFFF8159;
#define PTFDFE                          _PTFDFE.Byte
#define PTFDFE_PTFDFE0                  _PTFDFE.Bits.PTFDFE0
#define PTFDFE_PTFDFE1                  _PTFDFE.Bits.PTFDFE1
#define PTFDFE_PTFDFE2                  _PTFDFE.Bits.PTFDFE2
#define PTFDFE_PTFDFE3                  _PTFDFE.Bits.PTFDFE3
#define PTFDFE_PTFDFE4                  _PTFDFE.Bits.PTFDFE4
#define PTFDFE_PTFDFE5                  _PTFDFE.Bits.PTFDFE5
#define PTFDFE_PTFDFE6                  _PTFDFE.Bits.PTFDFE6
#define PTFDFE_PTFDFE7                  _PTFDFE.Bits.PTFDFE7

#define PTFDFE_PTFDFE0_MASK             1
#define PTFDFE_PTFDFE1_MASK             2
#define PTFDFE_PTFDFE2_MASK             4
#define PTFDFE_PTFDFE3_MASK             8
#define PTFDFE_PTFDFE4_MASK             16
#define PTFDFE_PTFDFE5_MASK             32
#define PTFDFE_PTFDFE6_MASK             64
#define PTFDFE_PTFDFE7_MASK             128


/*** PTFDFC - Port F Digital Filter Control Register; 0xFFFF815A ***/
typedef union {
  byte Byte;
  struct {
    byte PTFFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTFFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTFFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTFFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTFFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTFCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTFFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTFDFCSTR;
extern volatile PTFDFCSTR _PTFDFC @0xFFFF815A;
#define PTFDFC                          _PTFDFC.Byte
#define PTFDFC_PTFFF0                   _PTFDFC.Bits.PTFFF0
#define PTFDFC_PTFFF1                   _PTFDFC.Bits.PTFFF1
#define PTFDFC_PTFFF2                   _PTFDFC.Bits.PTFFF2
#define PTFDFC_PTFFF3                   _PTFDFC.Bits.PTFFF3
#define PTFDFC_PTFFF4                   _PTFDFC.Bits.PTFFF4
#define PTFDFC_PTFCLKS                  _PTFDFC.Bits.PTFCLKS
#define PTFDFC_PTFFF                    _PTFDFC.MergedBits.grpPTFFF

#define PTFDFC_PTFFF0_MASK              1
#define PTFDFC_PTFFF1_MASK              2
#define PTFDFC_PTFFF2_MASK              4
#define PTFDFC_PTFFF3_MASK              8
#define PTFDFC_PTFFF4_MASK              16
#define PTFDFC_PTFCLKS_MASK             128
#define PTFDFC_PTFFF_MASK               31
#define PTFDFC_PTFFF_BITNUM             0


/*** PTGPUE - Port G Pulling Enable Register; 0xFFFF8160 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPUE0     :1;                                       /* Internal Pulling Enable for Port G Bit 0 */
    byte PTGPUE1     :1;                                       /* Internal Pulling Enable for Port G Bit 1 */
    byte PTGPUE2     :1;                                       /* Internal Pulling Enable for Port G Bit 2 */
    byte PTGPUE3     :1;                                       /* Internal Pulling Enable for Port G Bit 3 */
    byte PTGPUE4     :1;                                       /* Internal Pulling Enable for Port G Bit 4 */
    byte PTGPUE5     :1;                                       /* Internal Pulling Enable for Port G Bit 5 */
    byte PTGPUE6     :1;                                       /* Internal Pulling Enable for Port G Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPUE :7;
    byte         :1;
  } MergedBits;
} PTGPUESTR;
extern volatile PTGPUESTR _PTGPUE @0xFFFF8160;
#define PTGPUE                          _PTGPUE.Byte
#define PTGPUE_PTGPUE0                  _PTGPUE.Bits.PTGPUE0
#define PTGPUE_PTGPUE1                  _PTGPUE.Bits.PTGPUE1
#define PTGPUE_PTGPUE2                  _PTGPUE.Bits.PTGPUE2
#define PTGPUE_PTGPUE3                  _PTGPUE.Bits.PTGPUE3
#define PTGPUE_PTGPUE4                  _PTGPUE.Bits.PTGPUE4
#define PTGPUE_PTGPUE5                  _PTGPUE.Bits.PTGPUE5
#define PTGPUE_PTGPUE6                  _PTGPUE.Bits.PTGPUE6
#define PTGPUE_PTGPUE                   _PTGPUE.MergedBits.grpPTGPUE

#define PTGPUE_PTGPUE0_MASK             1
#define PTGPUE_PTGPUE1_MASK             2
#define PTGPUE_PTGPUE2_MASK             4
#define PTGPUE_PTGPUE3_MASK             8
#define PTGPUE_PTGPUE4_MASK             16
#define PTGPUE_PTGPUE5_MASK             32
#define PTGPUE_PTGPUE6_MASK             64
#define PTGPUE_PTGPUE_MASK              127
#define PTGPUE_PTGPUE_BITNUM            0


/*** PTGPUS - Port G Pullup/Pulldown Select Register; 0xFFFF8161 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTGPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTGPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTGPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTGPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTGPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTGPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPUS :7;
    byte         :1;
  } MergedBits;
} PTGPUSSTR;
extern volatile PTGPUSSTR _PTGPUS @0xFFFF8161;
#define PTGPUS                          _PTGPUS.Byte
#define PTGPUS_PTGPUS0                  _PTGPUS.Bits.PTGPUS0
#define PTGPUS_PTGPUS1                  _PTGPUS.Bits.PTGPUS1
#define PTGPUS_PTGPUS2                  _PTGPUS.Bits.PTGPUS2
#define PTGPUS_PTGPUS3                  _PTGPUS.Bits.PTGPUS3
#define PTGPUS_PTGPUS4                  _PTGPUS.Bits.PTGPUS4
#define PTGPUS_PTGPUS5                  _PTGPUS.Bits.PTGPUS5
#define PTGPUS_PTGPUS6                  _PTGPUS.Bits.PTGPUS6
#define PTGPUS_PTGPUS                   _PTGPUS.MergedBits.grpPTGPUS

#define PTGPUS_PTGPUS0_MASK             1
#define PTGPUS_PTGPUS1_MASK             2
#define PTGPUS_PTGPUS2_MASK             4
#define PTGPUS_PTGPUS3_MASK             8
#define PTGPUS_PTGPUS4_MASK             16
#define PTGPUS_PTGPUS5_MASK             32
#define PTGPUS_PTGPUS6_MASK             64
#define PTGPUS_PTGPUS_MASK              127
#define PTGPUS_PTGPUS_BITNUM            0


/*** PTGDS - Port G Drive Strength Selection Register; 0xFFFF8162 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDS0      :1;                                       /* Output Drive Strength Selection for Port G Bit 0 */
    byte PTGDS1      :1;                                       /* Output Drive Strength Selection for Port G Bit 1 */
    byte PTGDS2      :1;                                       /* Output Drive Strength Selection for Port G Bit 2 */
    byte PTGDS3      :1;                                       /* Output Drive Strength Selection for Port G Bit 3 */
    byte PTGDS4      :1;                                       /* Output Drive Strength Selection for Port G Bit 4 */
    byte PTGDS5      :1;                                       /* Output Drive Strength Selection for Port G Bit 5 */
    byte PTGDS6      :1;                                       /* Output Drive Strength Selection for Port G Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDS :7;
    byte         :1;
  } MergedBits;
} PTGDSSTR;
extern volatile PTGDSSTR _PTGDS @0xFFFF8162;
#define PTGDS                           _PTGDS.Byte
#define PTGDS_PTGDS0                    _PTGDS.Bits.PTGDS0
#define PTGDS_PTGDS1                    _PTGDS.Bits.PTGDS1
#define PTGDS_PTGDS2                    _PTGDS.Bits.PTGDS2
#define PTGDS_PTGDS3                    _PTGDS.Bits.PTGDS3
#define PTGDS_PTGDS4                    _PTGDS.Bits.PTGDS4
#define PTGDS_PTGDS5                    _PTGDS.Bits.PTGDS5
#define PTGDS_PTGDS6                    _PTGDS.Bits.PTGDS6
#define PTGDS_PTGDS                     _PTGDS.MergedBits.grpPTGDS

#define PTGDS_PTGDS0_MASK               1
#define PTGDS_PTGDS1_MASK               2
#define PTGDS_PTGDS2_MASK               4
#define PTGDS_PTGDS3_MASK               8
#define PTGDS_PTGDS4_MASK               16
#define PTGDS_PTGDS5_MASK               32
#define PTGDS_PTGDS6_MASK               64
#define PTGDS_PTGDS_MASK                127
#define PTGDS_PTGDS_BITNUM              0


/*** PTGSRE - Port G Slew Rate Enable Register; 0xFFFF8163 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGSRE0     :1;                                       /* Output Slew Rate Enable for Port G Bit 0 */
    byte PTGSRE1     :1;                                       /* Output Slew Rate Enable for Port G Bit 1 */
    byte PTGSRE2     :1;                                       /* Output Slew Rate Enable for Port G Bit 2 */
    byte PTGSRE3     :1;                                       /* Output Slew Rate Enable for Port G Bit 3 */
    byte PTGSRE4     :1;                                       /* Output Slew Rate Enable for Port G Bit 4 */
    byte PTGSRE5     :1;                                       /* Output Slew Rate Enable for Port G Bit 5 */
    byte PTGSRE6     :1;                                       /* Output Slew Rate Enable for Port G Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGSRE :7;
    byte         :1;
  } MergedBits;
} PTGSRESTR;
extern volatile PTGSRESTR _PTGSRE @0xFFFF8163;
#define PTGSRE                          _PTGSRE.Byte
#define PTGSRE_PTGSRE0                  _PTGSRE.Bits.PTGSRE0
#define PTGSRE_PTGSRE1                  _PTGSRE.Bits.PTGSRE1
#define PTGSRE_PTGSRE2                  _PTGSRE.Bits.PTGSRE2
#define PTGSRE_PTGSRE3                  _PTGSRE.Bits.PTGSRE3
#define PTGSRE_PTGSRE4                  _PTGSRE.Bits.PTGSRE4
#define PTGSRE_PTGSRE5                  _PTGSRE.Bits.PTGSRE5
#define PTGSRE_PTGSRE6                  _PTGSRE.Bits.PTGSRE6
#define PTGSRE_PTGSRE                   _PTGSRE.MergedBits.grpPTGSRE

#define PTGSRE_PTGSRE0_MASK             1
#define PTGSRE_PTGSRE1_MASK             2
#define PTGSRE_PTGSRE2_MASK             4
#define PTGSRE_PTGSRE3_MASK             8
#define PTGSRE_PTGSRE4_MASK             16
#define PTGSRE_PTGSRE5_MASK             32
#define PTGSRE_PTGSRE6_MASK             64
#define PTGSRE_PTGSRE_MASK              127
#define PTGSRE_PTGSRE_BITNUM            0


/*** PTGPFE - Port G Passive Filter Enable Register; 0xFFFF8164 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTGPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTGPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTGPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTGPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTGPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTGPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPFE :7;
    byte         :1;
  } MergedBits;
} PTGPFESTR;
extern volatile PTGPFESTR _PTGPFE @0xFFFF8164;
#define PTGPFE                          _PTGPFE.Byte
#define PTGPFE_PTGPFE0                  _PTGPFE.Bits.PTGPFE0
#define PTGPFE_PTGPFE1                  _PTGPFE.Bits.PTGPFE1
#define PTGPFE_PTGPFE2                  _PTGPFE.Bits.PTGPFE2
#define PTGPFE_PTGPFE3                  _PTGPFE.Bits.PTGPFE3
#define PTGPFE_PTGPFE4                  _PTGPFE.Bits.PTGPFE4
#define PTGPFE_PTGPFE5                  _PTGPFE.Bits.PTGPFE5
#define PTGPFE_PTGPFE6                  _PTGPFE.Bits.PTGPFE6
#define PTGPFE_PTGPFE                   _PTGPFE.MergedBits.grpPTGPFE

#define PTGPFE_PTGPFE0_MASK             1
#define PTGPFE_PTGPFE1_MASK             2
#define PTGPFE_PTGPFE2_MASK             4
#define PTGPFE_PTGPFE3_MASK             8
#define PTGPFE_PTGPFE4_MASK             16
#define PTGPFE_PTGPFE5_MASK             32
#define PTGPFE_PTGPFE6_MASK             64
#define PTGPFE_PTGPFE_MASK              127
#define PTGPFE_PTGPFE_BITNUM            0


/*** PTGIC - Port G Interrupt Control Register; 0xFFFF8165 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTGIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTGDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTGICSTR;
extern volatile PTGICSTR _PTGIC @0xFFFF8165;
#define PTGIC                           _PTGIC.Byte
#define PTGIC_PTGMOD                    _PTGIC.Bits.PTGMOD
#define PTGIC_PTGIE                     _PTGIC.Bits.PTGIE
#define PTGIC_PTGDMAEN                  _PTGIC.Bits.PTGDMAEN

#define PTGIC_PTGMOD_MASK               1
#define PTGIC_PTGIE_MASK                2
#define PTGIC_PTGDMAEN_MASK             128


/*** PTGIPE - Port G Interrupt Pin Enable Register; 0xFFFF8166 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTGIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTGIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTGIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTGIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTGIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTGIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGIPE :7;
    byte         :1;
  } MergedBits;
} PTGIPESTR;
extern volatile PTGIPESTR _PTGIPE @0xFFFF8166;
#define PTGIPE                          _PTGIPE.Byte
#define PTGIPE_PTGIPE0                  _PTGIPE.Bits.PTGIPE0
#define PTGIPE_PTGIPE1                  _PTGIPE.Bits.PTGIPE1
#define PTGIPE_PTGIPE2                  _PTGIPE.Bits.PTGIPE2
#define PTGIPE_PTGIPE3                  _PTGIPE.Bits.PTGIPE3
#define PTGIPE_PTGIPE4                  _PTGIPE.Bits.PTGIPE4
#define PTGIPE_PTGIPE5                  _PTGIPE.Bits.PTGIPE5
#define PTGIPE_PTGIPE6                  _PTGIPE.Bits.PTGIPE6
#define PTGIPE_PTGIPE                   _PTGIPE.MergedBits.grpPTGIPE

#define PTGIPE_PTGIPE0_MASK             1
#define PTGIPE_PTGIPE1_MASK             2
#define PTGIPE_PTGIPE2_MASK             4
#define PTGIPE_PTGIPE3_MASK             8
#define PTGIPE_PTGIPE4_MASK             16
#define PTGIPE_PTGIPE5_MASK             32
#define PTGIPE_PTGIPE6_MASK             64
#define PTGIPE_PTGIPE_MASK              127
#define PTGIPE_PTGIPE_BITNUM            0


/*** PTGIF - Port G Interrupt Flag Register; 0xFFFF8167 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTGIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTGIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTGIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTGIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTGIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTGIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGIF :7;
    byte         :1;
  } MergedBits;
} PTGIFSTR;
extern volatile PTGIFSTR _PTGIF @0xFFFF8167;
#define PTGIF                           _PTGIF.Byte
#define PTGIF_PTGIF0                    _PTGIF.Bits.PTGIF0
#define PTGIF_PTGIF1                    _PTGIF.Bits.PTGIF1
#define PTGIF_PTGIF2                    _PTGIF.Bits.PTGIF2
#define PTGIF_PTGIF3                    _PTGIF.Bits.PTGIF3
#define PTGIF_PTGIF4                    _PTGIF.Bits.PTGIF4
#define PTGIF_PTGIF5                    _PTGIF.Bits.PTGIF5
#define PTGIF_PTGIF6                    _PTGIF.Bits.PTGIF6
#define PTGIF_PTGIF                     _PTGIF.MergedBits.grpPTGIF

#define PTGIF_PTGIF0_MASK               1
#define PTGIF_PTGIF1_MASK               2
#define PTGIF_PTGIF2_MASK               4
#define PTGIF_PTGIF3_MASK               8
#define PTGIF_PTGIF4_MASK               16
#define PTGIF_PTGIF5_MASK               32
#define PTGIF_PTGIF6_MASK               64
#define PTGIF_PTGIF_MASK                127
#define PTGIF_PTGIF_BITNUM              0


/*** PTGIES - Port G Interrupt Edge Select Register; 0xFFFF8168 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTGEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTGEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTGEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTGEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTGEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTGEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGEDG :7;
    byte         :1;
  } MergedBits;
} PTGIESSTR;
extern volatile PTGIESSTR _PTGIES @0xFFFF8168;
#define PTGIES                          _PTGIES.Byte
#define PTGIES_PTGEDG0                  _PTGIES.Bits.PTGEDG0
#define PTGIES_PTGEDG1                  _PTGIES.Bits.PTGEDG1
#define PTGIES_PTGEDG2                  _PTGIES.Bits.PTGEDG2
#define PTGIES_PTGEDG3                  _PTGIES.Bits.PTGEDG3
#define PTGIES_PTGEDG4                  _PTGIES.Bits.PTGEDG4
#define PTGIES_PTGEDG5                  _PTGIES.Bits.PTGEDG5
#define PTGIES_PTGEDG6                  _PTGIES.Bits.PTGEDG6
#define PTGIES_PTGEDG                   _PTGIES.MergedBits.grpPTGEDG

#define PTGIES_PTGEDG0_MASK             1
#define PTGIES_PTGEDG1_MASK             2
#define PTGIES_PTGEDG2_MASK             4
#define PTGIES_PTGEDG3_MASK             8
#define PTGIES_PTGEDG4_MASK             16
#define PTGIES_PTGEDG5_MASK             32
#define PTGIES_PTGEDG6_MASK             64
#define PTGIES_PTGEDG_MASK              127
#define PTGIES_PTGEDG_BITNUM            0


/*** PTGDFE - Port G Digital Filter Enable Register; 0xFFFF8169 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTGDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTGDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTGDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTGDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTGDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTGDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDFE :7;
    byte         :1;
  } MergedBits;
} PTGDFESTR;
extern volatile PTGDFESTR _PTGDFE @0xFFFF8169;
#define PTGDFE                          _PTGDFE.Byte
#define PTGDFE_PTGDFE0                  _PTGDFE.Bits.PTGDFE0
#define PTGDFE_PTGDFE1                  _PTGDFE.Bits.PTGDFE1
#define PTGDFE_PTGDFE2                  _PTGDFE.Bits.PTGDFE2
#define PTGDFE_PTGDFE3                  _PTGDFE.Bits.PTGDFE3
#define PTGDFE_PTGDFE4                  _PTGDFE.Bits.PTGDFE4
#define PTGDFE_PTGDFE5                  _PTGDFE.Bits.PTGDFE5
#define PTGDFE_PTGDFE6                  _PTGDFE.Bits.PTGDFE6
#define PTGDFE_PTGDFE                   _PTGDFE.MergedBits.grpPTGDFE

#define PTGDFE_PTGDFE0_MASK             1
#define PTGDFE_PTGDFE1_MASK             2
#define PTGDFE_PTGDFE2_MASK             4
#define PTGDFE_PTGDFE3_MASK             8
#define PTGDFE_PTGDFE4_MASK             16
#define PTGDFE_PTGDFE5_MASK             32
#define PTGDFE_PTGDFE6_MASK             64
#define PTGDFE_PTGDFE_MASK              127
#define PTGDFE_PTGDFE_BITNUM            0


/*** PTGDFC - Port G Digital Filter Control Register; 0xFFFF816A ***/
typedef union {
  byte Byte;
  struct {
    byte PTGFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTGFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTGFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTGFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTGFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTGCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTGFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDFCSTR;
extern volatile PTGDFCSTR _PTGDFC @0xFFFF816A;
#define PTGDFC                          _PTGDFC.Byte
#define PTGDFC_PTGFF0                   _PTGDFC.Bits.PTGFF0
#define PTGDFC_PTGFF1                   _PTGDFC.Bits.PTGFF1
#define PTGDFC_PTGFF2                   _PTGDFC.Bits.PTGFF2
#define PTGDFC_PTGFF3                   _PTGDFC.Bits.PTGFF3
#define PTGDFC_PTGFF4                   _PTGDFC.Bits.PTGFF4
#define PTGDFC_PTGCLKS                  _PTGDFC.Bits.PTGCLKS
#define PTGDFC_PTGFF                    _PTGDFC.MergedBits.grpPTGFF

#define PTGDFC_PTGFF0_MASK              1
#define PTGDFC_PTGFF1_MASK              2
#define PTGDFC_PTGFF2_MASK              4
#define PTGDFC_PTGFF3_MASK              8
#define PTGDFC_PTGFF4_MASK              16
#define PTGDFC_PTGCLKS_MASK             128
#define PTGDFC_PTGFF_MASK               31
#define PTGDFC_PTGFF_BITNUM             0


/*** PTHPUE - Port H Pulling Enable Register; 0xFFFF8170 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHPUE0     :1;                                       /* Internal Pulling Enable for Port H Bit 0 */
    byte PTHPUE1     :1;                                       /* Internal Pulling Enable for Port H Bit 1 */
    byte PTHPUE2     :1;                                       /* Internal Pulling Enable for Port H Bit 2 */
    byte PTHPUE3     :1;                                       /* Internal Pulling Enable for Port H Bit 3 */
    byte PTHPUE4     :1;                                       /* Internal Pulling Enable for Port H Bit 4 */
    byte PTHPUE5     :1;                                       /* Internal Pulling Enable for Port H Bit 5 */
    byte PTHPUE6     :1;                                       /* Internal Pulling Enable for Port H Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHPUE :7;
    byte         :1;
  } MergedBits;
} PTHPUESTR;
extern volatile PTHPUESTR _PTHPUE @0xFFFF8170;
#define PTHPUE                          _PTHPUE.Byte
#define PTHPUE_PTHPUE0                  _PTHPUE.Bits.PTHPUE0
#define PTHPUE_PTHPUE1                  _PTHPUE.Bits.PTHPUE1
#define PTHPUE_PTHPUE2                  _PTHPUE.Bits.PTHPUE2
#define PTHPUE_PTHPUE3                  _PTHPUE.Bits.PTHPUE3
#define PTHPUE_PTHPUE4                  _PTHPUE.Bits.PTHPUE4
#define PTHPUE_PTHPUE5                  _PTHPUE.Bits.PTHPUE5
#define PTHPUE_PTHPUE6                  _PTHPUE.Bits.PTHPUE6
#define PTHPUE_PTHPUE                   _PTHPUE.MergedBits.grpPTHPUE

#define PTHPUE_PTHPUE0_MASK             1
#define PTHPUE_PTHPUE1_MASK             2
#define PTHPUE_PTHPUE2_MASK             4
#define PTHPUE_PTHPUE3_MASK             8
#define PTHPUE_PTHPUE4_MASK             16
#define PTHPUE_PTHPUE5_MASK             32
#define PTHPUE_PTHPUE6_MASK             64
#define PTHPUE_PTHPUE_MASK              127
#define PTHPUE_PTHPUE_BITNUM            0


/*** PTHPUS - Port H Pullup/Pulldown Select Register; 0xFFFF8171 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTHPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTHPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTHPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTHPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTHPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTHPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHPUS :7;
    byte         :1;
  } MergedBits;
} PTHPUSSTR;
extern volatile PTHPUSSTR _PTHPUS @0xFFFF8171;
#define PTHPUS                          _PTHPUS.Byte
#define PTHPUS_PTHPUS0                  _PTHPUS.Bits.PTHPUS0
#define PTHPUS_PTHPUS1                  _PTHPUS.Bits.PTHPUS1
#define PTHPUS_PTHPUS2                  _PTHPUS.Bits.PTHPUS2
#define PTHPUS_PTHPUS3                  _PTHPUS.Bits.PTHPUS3
#define PTHPUS_PTHPUS4                  _PTHPUS.Bits.PTHPUS4
#define PTHPUS_PTHPUS5                  _PTHPUS.Bits.PTHPUS5
#define PTHPUS_PTHPUS6                  _PTHPUS.Bits.PTHPUS6
#define PTHPUS_PTHPUS                   _PTHPUS.MergedBits.grpPTHPUS

#define PTHPUS_PTHPUS0_MASK             1
#define PTHPUS_PTHPUS1_MASK             2
#define PTHPUS_PTHPUS2_MASK             4
#define PTHPUS_PTHPUS3_MASK             8
#define PTHPUS_PTHPUS4_MASK             16
#define PTHPUS_PTHPUS5_MASK             32
#define PTHPUS_PTHPUS6_MASK             64
#define PTHPUS_PTHPUS_MASK              127
#define PTHPUS_PTHPUS_BITNUM            0


/*** PTHDS - Port H Drive Strength Selection Register; 0xFFFF8172 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHDS0      :1;                                       /* Output Drive Strength Selection for Port H Bit 0 */
    byte PTHDS1      :1;                                       /* Output Drive Strength Selection for Port H Bit 1 */
    byte PTHDS2      :1;                                       /* Output Drive Strength Selection for Port H Bit 2 */
    byte PTHDS3      :1;                                       /* Output Drive Strength Selection for Port H Bit 3 */
    byte PTHDS4      :1;                                       /* Output Drive Strength Selection for Port H Bit 4 */
    byte PTHDS5      :1;                                       /* Output Drive Strength Selection for Port H Bit 5 */
    byte PTHDS6      :1;                                       /* Output Drive Strength Selection for Port H Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHDS :7;
    byte         :1;
  } MergedBits;
} PTHDSSTR;
extern volatile PTHDSSTR _PTHDS @0xFFFF8172;
#define PTHDS                           _PTHDS.Byte
#define PTHDS_PTHDS0                    _PTHDS.Bits.PTHDS0
#define PTHDS_PTHDS1                    _PTHDS.Bits.PTHDS1
#define PTHDS_PTHDS2                    _PTHDS.Bits.PTHDS2
#define PTHDS_PTHDS3                    _PTHDS.Bits.PTHDS3
#define PTHDS_PTHDS4                    _PTHDS.Bits.PTHDS4
#define PTHDS_PTHDS5                    _PTHDS.Bits.PTHDS5
#define PTHDS_PTHDS6                    _PTHDS.Bits.PTHDS6
#define PTHDS_PTHDS                     _PTHDS.MergedBits.grpPTHDS

#define PTHDS_PTHDS0_MASK               1
#define PTHDS_PTHDS1_MASK               2
#define PTHDS_PTHDS2_MASK               4
#define PTHDS_PTHDS3_MASK               8
#define PTHDS_PTHDS4_MASK               16
#define PTHDS_PTHDS5_MASK               32
#define PTHDS_PTHDS6_MASK               64
#define PTHDS_PTHDS_MASK                127
#define PTHDS_PTHDS_BITNUM              0


/*** PTHSRE - Port H Slew Rate Enable Register; 0xFFFF8173 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHSRE0     :1;                                       /* Output Slew Rate Enable for Port H Bit 0 */
    byte PTHSRE1     :1;                                       /* Output Slew Rate Enable for Port H Bit 1 */
    byte PTHSRE2     :1;                                       /* Output Slew Rate Enable for Port H Bit 2 */
    byte PTHSRE3     :1;                                       /* Output Slew Rate Enable for Port H Bit 3 */
    byte PTHSRE4     :1;                                       /* Output Slew Rate Enable for Port H Bit 4 */
    byte PTHSRE5     :1;                                       /* Output Slew Rate Enable for Port H Bit 5 */
    byte PTHSRE6     :1;                                       /* Output Slew Rate Enable for Port H Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHSRE :7;
    byte         :1;
  } MergedBits;
} PTHSRESTR;
extern volatile PTHSRESTR _PTHSRE @0xFFFF8173;
#define PTHSRE                          _PTHSRE.Byte
#define PTHSRE_PTHSRE0                  _PTHSRE.Bits.PTHSRE0
#define PTHSRE_PTHSRE1                  _PTHSRE.Bits.PTHSRE1
#define PTHSRE_PTHSRE2                  _PTHSRE.Bits.PTHSRE2
#define PTHSRE_PTHSRE3                  _PTHSRE.Bits.PTHSRE3
#define PTHSRE_PTHSRE4                  _PTHSRE.Bits.PTHSRE4
#define PTHSRE_PTHSRE5                  _PTHSRE.Bits.PTHSRE5
#define PTHSRE_PTHSRE6                  _PTHSRE.Bits.PTHSRE6
#define PTHSRE_PTHSRE                   _PTHSRE.MergedBits.grpPTHSRE

#define PTHSRE_PTHSRE0_MASK             1
#define PTHSRE_PTHSRE1_MASK             2
#define PTHSRE_PTHSRE2_MASK             4
#define PTHSRE_PTHSRE3_MASK             8
#define PTHSRE_PTHSRE4_MASK             16
#define PTHSRE_PTHSRE5_MASK             32
#define PTHSRE_PTHSRE6_MASK             64
#define PTHSRE_PTHSRE_MASK              127
#define PTHSRE_PTHSRE_BITNUM            0


/*** PTHPFE - Port H Passive Filter Enable Register; 0xFFFF8174 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTHPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTHPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTHPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTHPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTHPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTHPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHPFE :7;
    byte         :1;
  } MergedBits;
} PTHPFESTR;
extern volatile PTHPFESTR _PTHPFE @0xFFFF8174;
#define PTHPFE                          _PTHPFE.Byte
#define PTHPFE_PTHPFE0                  _PTHPFE.Bits.PTHPFE0
#define PTHPFE_PTHPFE1                  _PTHPFE.Bits.PTHPFE1
#define PTHPFE_PTHPFE2                  _PTHPFE.Bits.PTHPFE2
#define PTHPFE_PTHPFE3                  _PTHPFE.Bits.PTHPFE3
#define PTHPFE_PTHPFE4                  _PTHPFE.Bits.PTHPFE4
#define PTHPFE_PTHPFE5                  _PTHPFE.Bits.PTHPFE5
#define PTHPFE_PTHPFE6                  _PTHPFE.Bits.PTHPFE6
#define PTHPFE_PTHPFE                   _PTHPFE.MergedBits.grpPTHPFE

#define PTHPFE_PTHPFE0_MASK             1
#define PTHPFE_PTHPFE1_MASK             2
#define PTHPFE_PTHPFE2_MASK             4
#define PTHPFE_PTHPFE3_MASK             8
#define PTHPFE_PTHPFE4_MASK             16
#define PTHPFE_PTHPFE5_MASK             32
#define PTHPFE_PTHPFE6_MASK             64
#define PTHPFE_PTHPFE_MASK              127
#define PTHPFE_PTHPFE_BITNUM            0


/*** PTHIC - Port H Interrupt Control Register; 0xFFFF8175 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTHIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTHDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTHICSTR;
extern volatile PTHICSTR _PTHIC @0xFFFF8175;
#define PTHIC                           _PTHIC.Byte
#define PTHIC_PTHMOD                    _PTHIC.Bits.PTHMOD
#define PTHIC_PTHIE                     _PTHIC.Bits.PTHIE
#define PTHIC_PTHDMAEN                  _PTHIC.Bits.PTHDMAEN

#define PTHIC_PTHMOD_MASK               1
#define PTHIC_PTHIE_MASK                2
#define PTHIC_PTHDMAEN_MASK             128


/*** PTHIPE - Port H Interrupt Pin Enable Register; 0xFFFF8176 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTHIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTHIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTHIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTHIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTHIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTHIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHIPE :7;
    byte         :1;
  } MergedBits;
} PTHIPESTR;
extern volatile PTHIPESTR _PTHIPE @0xFFFF8176;
#define PTHIPE                          _PTHIPE.Byte
#define PTHIPE_PTHIPE0                  _PTHIPE.Bits.PTHIPE0
#define PTHIPE_PTHIPE1                  _PTHIPE.Bits.PTHIPE1
#define PTHIPE_PTHIPE2                  _PTHIPE.Bits.PTHIPE2
#define PTHIPE_PTHIPE3                  _PTHIPE.Bits.PTHIPE3
#define PTHIPE_PTHIPE4                  _PTHIPE.Bits.PTHIPE4
#define PTHIPE_PTHIPE5                  _PTHIPE.Bits.PTHIPE5
#define PTHIPE_PTHIPE6                  _PTHIPE.Bits.PTHIPE6
#define PTHIPE_PTHIPE                   _PTHIPE.MergedBits.grpPTHIPE

#define PTHIPE_PTHIPE0_MASK             1
#define PTHIPE_PTHIPE1_MASK             2
#define PTHIPE_PTHIPE2_MASK             4
#define PTHIPE_PTHIPE3_MASK             8
#define PTHIPE_PTHIPE4_MASK             16
#define PTHIPE_PTHIPE5_MASK             32
#define PTHIPE_PTHIPE6_MASK             64
#define PTHIPE_PTHIPE_MASK              127
#define PTHIPE_PTHIPE_BITNUM            0


/*** PTHIF - Port H Interrupt Flag Register; 0xFFFF8177 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTHIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTHIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTHIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTHIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTHIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTHIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHIF :7;
    byte         :1;
  } MergedBits;
} PTHIFSTR;
extern volatile PTHIFSTR _PTHIF @0xFFFF8177;
#define PTHIF                           _PTHIF.Byte
#define PTHIF_PTHIF0                    _PTHIF.Bits.PTHIF0
#define PTHIF_PTHIF1                    _PTHIF.Bits.PTHIF1
#define PTHIF_PTHIF2                    _PTHIF.Bits.PTHIF2
#define PTHIF_PTHIF3                    _PTHIF.Bits.PTHIF3
#define PTHIF_PTHIF4                    _PTHIF.Bits.PTHIF4
#define PTHIF_PTHIF5                    _PTHIF.Bits.PTHIF5
#define PTHIF_PTHIF6                    _PTHIF.Bits.PTHIF6
#define PTHIF_PTHIF                     _PTHIF.MergedBits.grpPTHIF

#define PTHIF_PTHIF0_MASK               1
#define PTHIF_PTHIF1_MASK               2
#define PTHIF_PTHIF2_MASK               4
#define PTHIF_PTHIF3_MASK               8
#define PTHIF_PTHIF4_MASK               16
#define PTHIF_PTHIF5_MASK               32
#define PTHIF_PTHIF6_MASK               64
#define PTHIF_PTHIF_MASK                127
#define PTHIF_PTHIF_BITNUM              0


/*** PTHIES - Port H Interrupt Edge Select Register; 0xFFFF8178 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTHEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTHEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTHEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTHEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTHEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTHEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHEDG :7;
    byte         :1;
  } MergedBits;
} PTHIESSTR;
extern volatile PTHIESSTR _PTHIES @0xFFFF8178;
#define PTHIES                          _PTHIES.Byte
#define PTHIES_PTHEDG0                  _PTHIES.Bits.PTHEDG0
#define PTHIES_PTHEDG1                  _PTHIES.Bits.PTHEDG1
#define PTHIES_PTHEDG2                  _PTHIES.Bits.PTHEDG2
#define PTHIES_PTHEDG3                  _PTHIES.Bits.PTHEDG3
#define PTHIES_PTHEDG4                  _PTHIES.Bits.PTHEDG4
#define PTHIES_PTHEDG5                  _PTHIES.Bits.PTHEDG5
#define PTHIES_PTHEDG6                  _PTHIES.Bits.PTHEDG6
#define PTHIES_PTHEDG                   _PTHIES.MergedBits.grpPTHEDG

#define PTHIES_PTHEDG0_MASK             1
#define PTHIES_PTHEDG1_MASK             2
#define PTHIES_PTHEDG2_MASK             4
#define PTHIES_PTHEDG3_MASK             8
#define PTHIES_PTHEDG4_MASK             16
#define PTHIES_PTHEDG5_MASK             32
#define PTHIES_PTHEDG6_MASK             64
#define PTHIES_PTHEDG_MASK              127
#define PTHIES_PTHEDG_BITNUM            0


/*** PTHDFE - Port H Digital Filter Enable Register; 0xFFFF8179 ***/
typedef union {
  byte Byte;
  struct {
    byte PTHDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTHDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTHDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTHDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTHDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTHDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTHDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpPTHDFE :7;
    byte         :1;
  } MergedBits;
} PTHDFESTR;
extern volatile PTHDFESTR _PTHDFE @0xFFFF8179;
#define PTHDFE                          _PTHDFE.Byte
#define PTHDFE_PTHDFE0                  _PTHDFE.Bits.PTHDFE0
#define PTHDFE_PTHDFE1                  _PTHDFE.Bits.PTHDFE1
#define PTHDFE_PTHDFE2                  _PTHDFE.Bits.PTHDFE2
#define PTHDFE_PTHDFE3                  _PTHDFE.Bits.PTHDFE3
#define PTHDFE_PTHDFE4                  _PTHDFE.Bits.PTHDFE4
#define PTHDFE_PTHDFE5                  _PTHDFE.Bits.PTHDFE5
#define PTHDFE_PTHDFE6                  _PTHDFE.Bits.PTHDFE6
#define PTHDFE_PTHDFE                   _PTHDFE.MergedBits.grpPTHDFE

#define PTHDFE_PTHDFE0_MASK             1
#define PTHDFE_PTHDFE1_MASK             2
#define PTHDFE_PTHDFE2_MASK             4
#define PTHDFE_PTHDFE3_MASK             8
#define PTHDFE_PTHDFE4_MASK             16
#define PTHDFE_PTHDFE5_MASK             32
#define PTHDFE_PTHDFE6_MASK             64
#define PTHDFE_PTHDFE_MASK              127
#define PTHDFE_PTHDFE_BITNUM            0


/*** PTHDFC - Port H Digital Filter Control Register; 0xFFFF817A ***/
typedef union {
  byte Byte;
  struct {
    byte PTHFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTHFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTHFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTHFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTHFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTHCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTHFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTHDFCSTR;
extern volatile PTHDFCSTR _PTHDFC @0xFFFF817A;
#define PTHDFC                          _PTHDFC.Byte
#define PTHDFC_PTHFF0                   _PTHDFC.Bits.PTHFF0
#define PTHDFC_PTHFF1                   _PTHDFC.Bits.PTHFF1
#define PTHDFC_PTHFF2                   _PTHDFC.Bits.PTHFF2
#define PTHDFC_PTHFF3                   _PTHDFC.Bits.PTHFF3
#define PTHDFC_PTHFF4                   _PTHDFC.Bits.PTHFF4
#define PTHDFC_PTHCLKS                  _PTHDFC.Bits.PTHCLKS
#define PTHDFC_PTHFF                    _PTHDFC.MergedBits.grpPTHFF

#define PTHDFC_PTHFF0_MASK              1
#define PTHDFC_PTHFF1_MASK              2
#define PTHDFC_PTHFF2_MASK              4
#define PTHDFC_PTHFF3_MASK              8
#define PTHDFC_PTHFF4_MASK              16
#define PTHDFC_PTHCLKS_MASK             128
#define PTHDFC_PTHFF_MASK               31
#define PTHDFC_PTHFF_BITNUM             0


/*** PTJPUE - Port J Pulling Enable Register; 0xFFFF8180 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJPUE0     :1;                                       /* Internal Pulling Enable for Port J Bit 0 */
    byte PTJPUE1     :1;                                       /* Internal Pulling Enable for Port J Bit 1 */
    byte PTJPUE2     :1;                                       /* Internal Pulling Enable for Port J Bit 2 */
    byte PTJPUE3     :1;                                       /* Internal Pulling Enable for Port J Bit 3 */
    byte PTJPUE4     :1;                                       /* Internal Pulling Enable for Port J Bit 4 */
    byte PTJPUE5     :1;                                       /* Internal Pulling Enable for Port J Bit 5 */
    byte PTJPUE6     :1;                                       /* Internal Pulling Enable for Port J Bit 6 */
    byte PTJPUE7     :1;                                       /* Internal Pulling Enable for Port J Bit 7 */
  } Bits;
} PTJPUESTR;
extern volatile PTJPUESTR _PTJPUE @0xFFFF8180;
#define PTJPUE                          _PTJPUE.Byte
#define PTJPUE_PTJPUE0                  _PTJPUE.Bits.PTJPUE0
#define PTJPUE_PTJPUE1                  _PTJPUE.Bits.PTJPUE1
#define PTJPUE_PTJPUE2                  _PTJPUE.Bits.PTJPUE2
#define PTJPUE_PTJPUE3                  _PTJPUE.Bits.PTJPUE3
#define PTJPUE_PTJPUE4                  _PTJPUE.Bits.PTJPUE4
#define PTJPUE_PTJPUE5                  _PTJPUE.Bits.PTJPUE5
#define PTJPUE_PTJPUE6                  _PTJPUE.Bits.PTJPUE6
#define PTJPUE_PTJPUE7                  _PTJPUE.Bits.PTJPUE7

#define PTJPUE_PTJPUE0_MASK             1
#define PTJPUE_PTJPUE1_MASK             2
#define PTJPUE_PTJPUE2_MASK             4
#define PTJPUE_PTJPUE3_MASK             8
#define PTJPUE_PTJPUE4_MASK             16
#define PTJPUE_PTJPUE5_MASK             32
#define PTJPUE_PTJPUE6_MASK             64
#define PTJPUE_PTJPUE7_MASK             128


/*** PTJPUS - Port J Pullup/Pulldown Select Register; 0xFFFF8181 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJPUS0     :1;                                       /* Pullup/Pulldown Select Bit 0 */
    byte PTJPUS1     :1;                                       /* Pullup/Pulldown Select Bit 1 */
    byte PTJPUS2     :1;                                       /* Pullup/Pulldown Select Bit 2 */
    byte PTJPUS3     :1;                                       /* Pullup/Pulldown Select Bit 3 */
    byte PTJPUS4     :1;                                       /* Pullup/Pulldown Select Bit 4 */
    byte PTJPUS5     :1;                                       /* Pullup/Pulldown Select Bit 5 */
    byte PTJPUS6     :1;                                       /* Pullup/Pulldown Select Bit 6 */
    byte PTJPUS7     :1;                                       /* Pullup/Pulldown Select Bit 7 */
  } Bits;
} PTJPUSSTR;
extern volatile PTJPUSSTR _PTJPUS @0xFFFF8181;
#define PTJPUS                          _PTJPUS.Byte
#define PTJPUS_PTJPUS0                  _PTJPUS.Bits.PTJPUS0
#define PTJPUS_PTJPUS1                  _PTJPUS.Bits.PTJPUS1
#define PTJPUS_PTJPUS2                  _PTJPUS.Bits.PTJPUS2
#define PTJPUS_PTJPUS3                  _PTJPUS.Bits.PTJPUS3
#define PTJPUS_PTJPUS4                  _PTJPUS.Bits.PTJPUS4
#define PTJPUS_PTJPUS5                  _PTJPUS.Bits.PTJPUS5
#define PTJPUS_PTJPUS6                  _PTJPUS.Bits.PTJPUS6
#define PTJPUS_PTJPUS7                  _PTJPUS.Bits.PTJPUS7

#define PTJPUS_PTJPUS0_MASK             1
#define PTJPUS_PTJPUS1_MASK             2
#define PTJPUS_PTJPUS2_MASK             4
#define PTJPUS_PTJPUS3_MASK             8
#define PTJPUS_PTJPUS4_MASK             16
#define PTJPUS_PTJPUS5_MASK             32
#define PTJPUS_PTJPUS6_MASK             64
#define PTJPUS_PTJPUS7_MASK             128


/*** PTJDS - Port J Drive Strength Selection Register; 0xFFFF8182 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJDS0      :1;                                       /* Output Drive Strength Selection for Port J Bit 0 */
    byte PTJDS1      :1;                                       /* Output Drive Strength Selection for Port J Bit 1 */
    byte PTJDS2      :1;                                       /* Output Drive Strength Selection for Port J Bit 2 */
    byte PTJDS3      :1;                                       /* Output Drive Strength Selection for Port J Bit 3 */
    byte PTJDS4      :1;                                       /* Output Drive Strength Selection for Port J Bit 4 */
    byte PTJDS5      :1;                                       /* Output Drive Strength Selection for Port J Bit 5 */
    byte PTJDS6      :1;                                       /* Output Drive Strength Selection for Port J Bit 6 */
    byte PTJDS7      :1;                                       /* Output Drive Strength Selection for Port J Bit 7 */
  } Bits;
} PTJDSSTR;
extern volatile PTJDSSTR _PTJDS @0xFFFF8182;
#define PTJDS                           _PTJDS.Byte
#define PTJDS_PTJDS0                    _PTJDS.Bits.PTJDS0
#define PTJDS_PTJDS1                    _PTJDS.Bits.PTJDS1
#define PTJDS_PTJDS2                    _PTJDS.Bits.PTJDS2
#define PTJDS_PTJDS3                    _PTJDS.Bits.PTJDS3
#define PTJDS_PTJDS4                    _PTJDS.Bits.PTJDS4
#define PTJDS_PTJDS5                    _PTJDS.Bits.PTJDS5
#define PTJDS_PTJDS6                    _PTJDS.Bits.PTJDS6
#define PTJDS_PTJDS7                    _PTJDS.Bits.PTJDS7

#define PTJDS_PTJDS0_MASK               1
#define PTJDS_PTJDS1_MASK               2
#define PTJDS_PTJDS2_MASK               4
#define PTJDS_PTJDS3_MASK               8
#define PTJDS_PTJDS4_MASK               16
#define PTJDS_PTJDS5_MASK               32
#define PTJDS_PTJDS6_MASK               64
#define PTJDS_PTJDS7_MASK               128


/*** PTJSRE - Port J Slew Rate Enable Register; 0xFFFF8183 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJSRE0     :1;                                       /* Output Slew Rate Enable for Port J Bit 0 */
    byte PTJSRE1     :1;                                       /* Output Slew Rate Enable for Port J Bit 1 */
    byte PTJSRE2     :1;                                       /* Output Slew Rate Enable for Port J Bit 2 */
    byte PTJSRE3     :1;                                       /* Output Slew Rate Enable for Port J Bit 3 */
    byte PTJSRE4     :1;                                       /* Output Slew Rate Enable for Port J Bit 4 */
    byte PTJSRE5     :1;                                       /* Output Slew Rate Enable for Port J Bit 5 */
    byte PTJSRE6     :1;                                       /* Output Slew Rate Enable for Port J Bit 6 */
    byte PTJSRE7     :1;                                       /* Output Slew Rate Enable for Port J Bit 7 */
  } Bits;
} PTJSRESTR;
extern volatile PTJSRESTR _PTJSRE @0xFFFF8183;
#define PTJSRE                          _PTJSRE.Byte
#define PTJSRE_PTJSRE0                  _PTJSRE.Bits.PTJSRE0
#define PTJSRE_PTJSRE1                  _PTJSRE.Bits.PTJSRE1
#define PTJSRE_PTJSRE2                  _PTJSRE.Bits.PTJSRE2
#define PTJSRE_PTJSRE3                  _PTJSRE.Bits.PTJSRE3
#define PTJSRE_PTJSRE4                  _PTJSRE.Bits.PTJSRE4
#define PTJSRE_PTJSRE5                  _PTJSRE.Bits.PTJSRE5
#define PTJSRE_PTJSRE6                  _PTJSRE.Bits.PTJSRE6
#define PTJSRE_PTJSRE7                  _PTJSRE.Bits.PTJSRE7

#define PTJSRE_PTJSRE0_MASK             1
#define PTJSRE_PTJSRE1_MASK             2
#define PTJSRE_PTJSRE2_MASK             4
#define PTJSRE_PTJSRE3_MASK             8
#define PTJSRE_PTJSRE4_MASK             16
#define PTJSRE_PTJSRE5_MASK             32
#define PTJSRE_PTJSRE6_MASK             64
#define PTJSRE_PTJSRE7_MASK             128


/*** PTJPFE - Port J Passive Filter Enable Register; 0xFFFF8184 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJPFE0     :1;                                       /* Port A Input Filter Enable Bit 0 */
    byte PTJPFE1     :1;                                       /* Port A Input Filter Enable Bit 1 */
    byte PTJPFE2     :1;                                       /* Port A Input Filter Enable Bit 2 */
    byte PTJPFE3     :1;                                       /* Port A Input Filter Enable Bit 3 */
    byte PTJPFE4     :1;                                       /* Port A Input Filter Enable Bit 4 */
    byte PTJPFE5     :1;                                       /* Port A Input Filter Enable Bit 5 */
    byte PTJPFE6     :1;                                       /* Port A Input Filter Enable Bit 6 */
    byte PTJPFE7     :1;                                       /* Port A Input Filter Enable Bit 7 */
  } Bits;
} PTJPFESTR;
extern volatile PTJPFESTR _PTJPFE @0xFFFF8184;
#define PTJPFE                          _PTJPFE.Byte
#define PTJPFE_PTJPFE0                  _PTJPFE.Bits.PTJPFE0
#define PTJPFE_PTJPFE1                  _PTJPFE.Bits.PTJPFE1
#define PTJPFE_PTJPFE2                  _PTJPFE.Bits.PTJPFE2
#define PTJPFE_PTJPFE3                  _PTJPFE.Bits.PTJPFE3
#define PTJPFE_PTJPFE4                  _PTJPFE.Bits.PTJPFE4
#define PTJPFE_PTJPFE5                  _PTJPFE.Bits.PTJPFE5
#define PTJPFE_PTJPFE6                  _PTJPFE.Bits.PTJPFE6
#define PTJPFE_PTJPFE7                  _PTJPFE.Bits.PTJPFE7

#define PTJPFE_PTJPFE0_MASK             1
#define PTJPFE_PTJPFE1_MASK             2
#define PTJPFE_PTJPFE2_MASK             4
#define PTJPFE_PTJPFE3_MASK             8
#define PTJPFE_PTJPFE4_MASK             16
#define PTJPFE_PTJPFE5_MASK             32
#define PTJPFE_PTJPFE6_MASK             64
#define PTJPFE_PTJPFE7_MASK             128


/*** PTJIC - Port J Interrupt Control Register; 0xFFFF8185 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJMOD      :1;                                       /* Detection Mode for Pin Interrupt */
    byte PTJIE       :1;                                       /* Interrupt Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte PTJDMAEN    :1;                                       /* DMA Enable bit */
  } Bits;
} PTJICSTR;
extern volatile PTJICSTR _PTJIC @0xFFFF8185;
#define PTJIC                           _PTJIC.Byte
#define PTJIC_PTJMOD                    _PTJIC.Bits.PTJMOD
#define PTJIC_PTJIE                     _PTJIC.Bits.PTJIE
#define PTJIC_PTJDMAEN                  _PTJIC.Bits.PTJDMAEN

#define PTJIC_PTJMOD_MASK               1
#define PTJIC_PTJIE_MASK                2
#define PTJIC_PTJDMAEN_MASK             128


/*** PTJIPE - Port J Interrupt Pin Enable Register; 0xFFFF8186 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJIPE0     :1;                                       /* Interrupt Pin Enable Bit 0 */
    byte PTJIPE1     :1;                                       /* Interrupt Pin Enable Bit 1 */
    byte PTJIPE2     :1;                                       /* Interrupt Pin Enable Bit 2 */
    byte PTJIPE3     :1;                                       /* Interrupt Pin Enable Bit 3 */
    byte PTJIPE4     :1;                                       /* Interrupt Pin Enable Bit 4 */
    byte PTJIPE5     :1;                                       /* Interrupt Pin Enable Bit 5 */
    byte PTJIPE6     :1;                                       /* Interrupt Pin Enable Bit 6 */
    byte PTJIPE7     :1;                                       /* Interrupt Pin Enable Bit 7 */
  } Bits;
} PTJIPESTR;
extern volatile PTJIPESTR _PTJIPE @0xFFFF8186;
#define PTJIPE                          _PTJIPE.Byte
#define PTJIPE_PTJIPE0                  _PTJIPE.Bits.PTJIPE0
#define PTJIPE_PTJIPE1                  _PTJIPE.Bits.PTJIPE1
#define PTJIPE_PTJIPE2                  _PTJIPE.Bits.PTJIPE2
#define PTJIPE_PTJIPE3                  _PTJIPE.Bits.PTJIPE3
#define PTJIPE_PTJIPE4                  _PTJIPE.Bits.PTJIPE4
#define PTJIPE_PTJIPE5                  _PTJIPE.Bits.PTJIPE5
#define PTJIPE_PTJIPE6                  _PTJIPE.Bits.PTJIPE6
#define PTJIPE_PTJIPE7                  _PTJIPE.Bits.PTJIPE7

#define PTJIPE_PTJIPE0_MASK             1
#define PTJIPE_PTJIPE1_MASK             2
#define PTJIPE_PTJIPE2_MASK             4
#define PTJIPE_PTJIPE3_MASK             8
#define PTJIPE_PTJIPE4_MASK             16
#define PTJIPE_PTJIPE5_MASK             32
#define PTJIPE_PTJIPE6_MASK             64
#define PTJIPE_PTJIPE7_MASK             128


/*** PTJIF - Port J Interrupt Flag Register; 0xFFFF8187 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJIF0      :1;                                       /* Interrupt Flag Bit 0 */
    byte PTJIF1      :1;                                       /* Interrupt Flag Bit 1 */
    byte PTJIF2      :1;                                       /* Interrupt Flag Bit 2 */
    byte PTJIF3      :1;                                       /* Interrupt Flag Bit 3 */
    byte PTJIF4      :1;                                       /* Interrupt Flag Bit 4 */
    byte PTJIF5      :1;                                       /* Interrupt Flag Bit 5 */
    byte PTJIF6      :1;                                       /* Interrupt Flag Bit 6 */
    byte PTJIF7      :1;                                       /* Interrupt Flag Bit 7 */
  } Bits;
} PTJIFSTR;
extern volatile PTJIFSTR _PTJIF @0xFFFF8187;
#define PTJIF                           _PTJIF.Byte
#define PTJIF_PTJIF0                    _PTJIF.Bits.PTJIF0
#define PTJIF_PTJIF1                    _PTJIF.Bits.PTJIF1
#define PTJIF_PTJIF2                    _PTJIF.Bits.PTJIF2
#define PTJIF_PTJIF3                    _PTJIF.Bits.PTJIF3
#define PTJIF_PTJIF4                    _PTJIF.Bits.PTJIF4
#define PTJIF_PTJIF5                    _PTJIF.Bits.PTJIF5
#define PTJIF_PTJIF6                    _PTJIF.Bits.PTJIF6
#define PTJIF_PTJIF7                    _PTJIF.Bits.PTJIF7

#define PTJIF_PTJIF0_MASK               1
#define PTJIF_PTJIF1_MASK               2
#define PTJIF_PTJIF2_MASK               4
#define PTJIF_PTJIF3_MASK               8
#define PTJIF_PTJIF4_MASK               16
#define PTJIF_PTJIF5_MASK               32
#define PTJIF_PTJIF6_MASK               64
#define PTJIF_PTJIF7_MASK               128


/*** PTJIES - Port J Interrupt Edge Select Register; 0xFFFF8188 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJEDG0     :1;                                       /* Edge Selects of Pin Interrupt Bit 0 */
    byte PTJEDG1     :1;                                       /* Edge Selects of Pin Interrupt Bit 1 */
    byte PTJEDG2     :1;                                       /* Edge Selects of Pin Interrupt Bit 2 */
    byte PTJEDG3     :1;                                       /* Edge Selects of Pin Interrupt Bit 3 */
    byte PTJEDG4     :1;                                       /* Edge Selects of Pin Interrupt Bit 4 */
    byte PTJEDG5     :1;                                       /* Edge Selects of Pin Interrupt Bit 5 */
    byte PTJEDG6     :1;                                       /* Edge Selects of Pin Interrupt Bit 6 */
    byte PTJEDG7     :1;                                       /* Edge Selects of Pin Interrupt Bit 7 */
  } Bits;
} PTJIESSTR;
extern volatile PTJIESSTR _PTJIES @0xFFFF8188;
#define PTJIES                          _PTJIES.Byte
#define PTJIES_PTJEDG0                  _PTJIES.Bits.PTJEDG0
#define PTJIES_PTJEDG1                  _PTJIES.Bits.PTJEDG1
#define PTJIES_PTJEDG2                  _PTJIES.Bits.PTJEDG2
#define PTJIES_PTJEDG3                  _PTJIES.Bits.PTJEDG3
#define PTJIES_PTJEDG4                  _PTJIES.Bits.PTJEDG4
#define PTJIES_PTJEDG5                  _PTJIES.Bits.PTJEDG5
#define PTJIES_PTJEDG6                  _PTJIES.Bits.PTJEDG6
#define PTJIES_PTJEDG7                  _PTJIES.Bits.PTJEDG7

#define PTJIES_PTJEDG0_MASK             1
#define PTJIES_PTJEDG1_MASK             2
#define PTJIES_PTJEDG2_MASK             4
#define PTJIES_PTJEDG3_MASK             8
#define PTJIES_PTJEDG4_MASK             16
#define PTJIES_PTJEDG5_MASK             32
#define PTJIES_PTJEDG6_MASK             64
#define PTJIES_PTJEDG7_MASK             128


/*** PTJDFE - Port J Digital Filter Enable Register; 0xFFFF8189 ***/
typedef union {
  byte Byte;
  struct {
    byte PTJDFE0     :1;                                       /* Digital Filter Enable Bit 0 */
    byte PTJDFE1     :1;                                       /* Digital Filter Enable Bit 1 */
    byte PTJDFE2     :1;                                       /* Digital Filter Enable Bit 2 */
    byte PTJDFE3     :1;                                       /* Digital Filter Enable Bit 3 */
    byte PTJDFE4     :1;                                       /* Digital Filter Enable Bit 4 */
    byte PTJDFE5     :1;                                       /* Digital Filter Enable Bit 5 */
    byte PTJDFE6     :1;                                       /* Digital Filter Enable Bit 6 */
    byte PTJDFE7     :1;                                       /* Digital Filter Enable Bit 7 */
  } Bits;
} PTJDFESTR;
extern volatile PTJDFESTR _PTJDFE @0xFFFF8189;
#define PTJDFE                          _PTJDFE.Byte
#define PTJDFE_PTJDFE0                  _PTJDFE.Bits.PTJDFE0
#define PTJDFE_PTJDFE1                  _PTJDFE.Bits.PTJDFE1
#define PTJDFE_PTJDFE2                  _PTJDFE.Bits.PTJDFE2
#define PTJDFE_PTJDFE3                  _PTJDFE.Bits.PTJDFE3
#define PTJDFE_PTJDFE4                  _PTJDFE.Bits.PTJDFE4
#define PTJDFE_PTJDFE5                  _PTJDFE.Bits.PTJDFE5
#define PTJDFE_PTJDFE6                  _PTJDFE.Bits.PTJDFE6
#define PTJDFE_PTJDFE7                  _PTJDFE.Bits.PTJDFE7

#define PTJDFE_PTJDFE0_MASK             1
#define PTJDFE_PTJDFE1_MASK             2
#define PTJDFE_PTJDFE2_MASK             4
#define PTJDFE_PTJDFE3_MASK             8
#define PTJDFE_PTJDFE4_MASK             16
#define PTJDFE_PTJDFE5_MASK             32
#define PTJDFE_PTJDFE6_MASK             64
#define PTJDFE_PTJDFE7_MASK             128


/*** PTJDFC - Port J Digital Filter Control Register; 0xFFFF818A ***/
typedef union {
  byte Byte;
  struct {
    byte PTJFF0      :1;                                       /* Filter Factor Bits, bit 0 */
    byte PTJFF1      :1;                                       /* Filter Factor Bits, bit 1 */
    byte PTJFF2      :1;                                       /* Filter Factor Bits, bit 2 */
    byte PTJFF3      :1;                                       /* Filter Factor Bits, bit 3 */
    byte PTJFF4      :1;                                       /* Filter Factor Bits, bit 4 */
    byte             :1; 
    byte             :1; 
    byte PTJCLKS     :1;                                       /* Clok Select Bit */
  } Bits;
  struct {
    byte grpPTJFF :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTJDFCSTR;
extern volatile PTJDFCSTR _PTJDFC @0xFFFF818A;
#define PTJDFC                          _PTJDFC.Byte
#define PTJDFC_PTJFF0                   _PTJDFC.Bits.PTJFF0
#define PTJDFC_PTJFF1                   _PTJDFC.Bits.PTJFF1
#define PTJDFC_PTJFF2                   _PTJDFC.Bits.PTJFF2
#define PTJDFC_PTJFF3                   _PTJDFC.Bits.PTJFF3
#define PTJDFC_PTJFF4                   _PTJDFC.Bits.PTJFF4
#define PTJDFC_PTJCLKS                  _PTJDFC.Bits.PTJCLKS
#define PTJDFC_PTJFF                    _PTJDFC.MergedBits.grpPTJFF

#define PTJDFC_PTJFF0_MASK              1
#define PTJDFC_PTJFF1_MASK              2
#define PTJDFC_PTJFF2_MASK              4
#define PTJDFC_PTJFF3_MASK              8
#define PTJDFC_PTJFF4_MASK              16
#define PTJDFC_PTJCLKS_MASK             128
#define PTJDFC_PTJFF_MASK               31
#define PTJDFC_PTJFF_BITNUM             0


/*** IICA1 - IIC Address Register; 0xFFFF8190 ***/
typedef union {
  byte Byte;
  union { /* Several registers at the same address */
    /*** IICA1 - IIC Address Register; Several registers at the same address ***/
    union {
      struct {
        byte             :1; 
        byte AD1         :1;                                       /* Slave Address Bit 1 */
        byte AD2         :1;                                       /* Slave Address Bit 2 */
        byte AD3         :1;                                       /* Slave Address Bit 3 */
        byte AD4         :1;                                       /* Slave Address Bit 4 */
        byte AD5         :1;                                       /* Slave Address Bit 5 */
        byte AD6         :1;                                       /* Slave Address Bit 6 */
        byte AD7         :1;                                       /* Slave Address Bit 7 */
      } Bits;
      struct {
        byte     :1;
        byte grpAD_1 :7;
      } MergedBits;
    } IICA1STR;
    #define IICA1                       _IICA1.Byte
    #define IICA1_AD1                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD1
    #define IICA1_AD2                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD2
    #define IICA1_AD3                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD3
    #define IICA1_AD4                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD4
    #define IICA1_AD5                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD5
    #define IICA1_AD6                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD6
    #define IICA1_AD7                   _IICA1.SameAddr_STR.IICA1STR.Bits.AD7
    #define IICA1_AD_1                  _IICA1.SameAddr_STR.IICA1STR.MergedBits.grpAD_1
    #define IICA1_AD                    IICA1_AD_1
    
    #define IICA1_AD1_MASK              2
    #define IICA1_AD2_MASK              4
    #define IICA1_AD3_MASK              8
    #define IICA1_AD4_MASK              16
    #define IICA1_AD5_MASK              32
    #define IICA1_AD6_MASK              64
    #define IICA1_AD7_MASK              128
    #define IICA1_AD_1_MASK             254
    #define IICA1_AD_1_BITNUM           1
    
    /*** IICA - IIC Address Register; Several registers at the same address ***/
    union {
      struct {
        byte             :1; 
        byte AD1         :1;                                       /* Slave Address Bit 1 */
        byte AD2         :1;                                       /* Slave Address Bit 2 */
        byte AD3         :1;                                       /* Slave Address Bit 3 */
        byte AD4         :1;                                       /* Slave Address Bit 4 */
        byte AD5         :1;                                       /* Slave Address Bit 5 */
        byte AD6         :1;                                       /* Slave Address Bit 6 */
        byte AD7         :1;                                       /* Slave Address Bit 7 */
      } Bits;
      struct {
        byte     :1;
        byte grpAD_1 :7;
      } MergedBits;
    } IICASTR;
    #define IICA                        _IICA1.Byte
    #define IICA_AD1                    _IICA1.SameAddr_STR.IICASTR.Bits.AD1
    #define IICA_AD2                    _IICA1.SameAddr_STR.IICASTR.Bits.AD2
    #define IICA_AD3                    _IICA1.SameAddr_STR.IICASTR.Bits.AD3
    #define IICA_AD4                    _IICA1.SameAddr_STR.IICASTR.Bits.AD4
    #define IICA_AD5                    _IICA1.SameAddr_STR.IICASTR.Bits.AD5
    #define IICA_AD6                    _IICA1.SameAddr_STR.IICASTR.Bits.AD6
    #define IICA_AD7                    _IICA1.SameAddr_STR.IICASTR.Bits.AD7
    #define IICA_AD_1                   _IICA1.SameAddr_STR.IICASTR.MergedBits.grpAD_1
    #define IICA_AD                     IICA_AD_1
    
    #define IICA_AD1_MASK               2
    #define IICA_AD2_MASK               4
    #define IICA_AD3_MASK               8
    #define IICA_AD4_MASK               16
    #define IICA_AD5_MASK               32
    #define IICA_AD6_MASK               64
    #define IICA_AD7_MASK               128
    #define IICA_AD_1_MASK              254
    #define IICA_AD_1_BITNUM            1
    
  } SameAddr_STR; /*Several registers at the same address */

} IICA1STR;
extern volatile IICA1STR _IICA1 @0xFFFF8190;


/*** IICF - IIC Frequency Divider Register; 0xFFFF8191 ***/
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
} IICFSTR;
extern volatile IICFSTR _IICF @0xFFFF8191;
#define IICF                            _IICF.Byte
#define IICF_ICR0                       _IICF.Bits.ICR0
#define IICF_ICR1                       _IICF.Bits.ICR1
#define IICF_ICR2                       _IICF.Bits.ICR2
#define IICF_ICR3                       _IICF.Bits.ICR3
#define IICF_ICR4                       _IICF.Bits.ICR4
#define IICF_ICR5                       _IICF.Bits.ICR5
#define IICF_MULT0                      _IICF.Bits.MULT0
#define IICF_MULT1                      _IICF.Bits.MULT1
#define IICF_ICR                        _IICF.MergedBits.grpICR
#define IICF_MULT                       _IICF.MergedBits.grpMULT

#define IICF_ICR0_MASK                  1
#define IICF_ICR1_MASK                  2
#define IICF_ICR2_MASK                  4
#define IICF_ICR3_MASK                  8
#define IICF_ICR4_MASK                  16
#define IICF_ICR5_MASK                  32
#define IICF_MULT0_MASK                 64
#define IICF_MULT1_MASK                 128
#define IICF_ICR_MASK                   63
#define IICF_ICR_BITNUM                 0
#define IICF_MULT_MASK                  192
#define IICF_MULT_BITNUM                6


/*** IICC1 - IIC Control Register 1; 0xFFFF8192 ***/
typedef union {
  byte Byte;
  union { /* Several registers at the same address */
    /*** IICC1 - IIC Control Register 1; Several registers at the same address ***/
    union {
      struct {
        byte DMAEN       :1;                                       /* DMA Enable */
        byte WUEN        :1;                                       /* Wake-up Enable */
        byte RSTA        :1;                                       /* Repeat START */
        byte TXAK        :1;                                       /* Transmit Acknowledge Enable */
        byte TX          :1;                                       /* Transmit Mode Select */
        byte MST         :1;                                       /* Master Mode Select */
        byte IICIE       :1;                                       /* IIC Interrupt Enable */
        byte IICEN       :1;                                       /* IIC Enable */
      } Bits;
    } IICC1STR;
    #define IICC1                       _IICC1.Byte
    #define IICC1_DMAEN                 _IICC1.SameAddr_STR.IICC1STR.Bits.DMAEN
    #define IICC1_WUEN                  _IICC1.SameAddr_STR.IICC1STR.Bits.WUEN
    #define IICC1_RSTA                  _IICC1.SameAddr_STR.IICC1STR.Bits.RSTA
    #define IICC1_TXAK                  _IICC1.SameAddr_STR.IICC1STR.Bits.TXAK
    #define IICC1_TX                    _IICC1.SameAddr_STR.IICC1STR.Bits.TX
    #define IICC1_MST                   _IICC1.SameAddr_STR.IICC1STR.Bits.MST
    #define IICC1_IICIE                 _IICC1.SameAddr_STR.IICC1STR.Bits.IICIE
    #define IICC1_IICEN                 _IICC1.SameAddr_STR.IICC1STR.Bits.IICEN
    
    #define IICC1_DMAEN_MASK            1
    #define IICC1_WUEN_MASK             2
    #define IICC1_RSTA_MASK             4
    #define IICC1_TXAK_MASK             8
    #define IICC1_TX_MASK               16
    #define IICC1_MST_MASK              32
    #define IICC1_IICIE_MASK            64
    #define IICC1_IICEN_MASK            128
    
    /*** IICC - IIC Control Register; Several registers at the same address ***/
    union {
      struct {
        byte DMAEN       :1;                                       /* DMA Enable */
        byte WUEN        :1;                                       /* Wake-up Enable */
        byte RSTA        :1;                                       /* Repeat START */
        byte TXAK        :1;                                       /* Transmit Acknowledge Enable */
        byte TX          :1;                                       /* Transmit Mode Select */
        byte MST         :1;                                       /* Master Mode Select */
        byte IICIE       :1;                                       /* IIC Interrupt Enable */
        byte IICEN       :1;                                       /* IIC Enable */
      } Bits;
    } IICCSTR;
    #define IICC                        _IICC1.Byte
    #define IICC_DMAEN                  _IICC1.SameAddr_STR.IICCSTR.Bits.DMAEN
    #define IICC_WUEN                   _IICC1.SameAddr_STR.IICCSTR.Bits.WUEN
    #define IICC_RSTA                   _IICC1.SameAddr_STR.IICCSTR.Bits.RSTA
    #define IICC_TXAK                   _IICC1.SameAddr_STR.IICCSTR.Bits.TXAK
    #define IICC_TX                     _IICC1.SameAddr_STR.IICCSTR.Bits.TX
    #define IICC_MST                    _IICC1.SameAddr_STR.IICCSTR.Bits.MST
    #define IICC_IICIE                  _IICC1.SameAddr_STR.IICCSTR.Bits.IICIE
    #define IICC_IICEN                  _IICC1.SameAddr_STR.IICCSTR.Bits.IICEN
    
    #define IICC_DMAEN_MASK             1
    #define IICC_WUEN_MASK              2
    #define IICC_RSTA_MASK              4
    #define IICC_TXAK_MASK              8
    #define IICC_TX_MASK                16
    #define IICC_MST_MASK               32
    #define IICC_IICIE_MASK             64
    #define IICC_IICEN_MASK             128
    
  } SameAddr_STR; /*Several registers at the same address */

} IICC1STR;
extern volatile IICC1STR _IICC1 @0xFFFF8192;


/*** IICS - IIC Status Register; 0xFFFF8193 ***/
typedef union {
  byte Byte;
  struct {
    byte RXAK        :1;                                       /* Receive Acknowledge */
    byte IICIF       :1;                                       /* IIC Interrupt Flag */
    byte SRW         :1;                                       /* Slave Read/Write */
    byte             :1; 
    byte ARBL        :1;                                       /* Arbitration Lost */
    byte BUSY        :1;                                       /* Bus Busy */
    byte IAAS        :1;                                       /* Addressed as a Slave */
    byte TCF         :1;                                       /* Transfer Complete Flag */
  } Bits;
} IICSSTR;
extern volatile IICSSTR _IICS @0xFFFF8193;
#define IICS                            _IICS.Byte
#define IICS_RXAK                       _IICS.Bits.RXAK
#define IICS_IICIF                      _IICS.Bits.IICIF
#define IICS_SRW                        _IICS.Bits.SRW
#define IICS_ARBL                       _IICS.Bits.ARBL
#define IICS_BUSY                       _IICS.Bits.BUSY
#define IICS_IAAS                       _IICS.Bits.IAAS
#define IICS_TCF                        _IICS.Bits.TCF

#define IICS_RXAK_MASK                  1
#define IICS_IICIF_MASK                 2
#define IICS_SRW_MASK                   4
#define IICS_ARBL_MASK                  16
#define IICS_BUSY_MASK                  32
#define IICS_IAAS_MASK                  64
#define IICS_TCF_MASK                   128


/*** IICD - IIC Data I/O Register; 0xFFFF8194 ***/
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
} IICDSTR;
extern volatile IICDSTR _IICD @0xFFFF8194;
#define IICD                            _IICD.Byte
#define IICD_DATA0                      _IICD.Bits.DATA0
#define IICD_DATA1                      _IICD.Bits.DATA1
#define IICD_DATA2                      _IICD.Bits.DATA2
#define IICD_DATA3                      _IICD.Bits.DATA3
#define IICD_DATA4                      _IICD.Bits.DATA4
#define IICD_DATA5                      _IICD.Bits.DATA5
#define IICD_DATA6                      _IICD.Bits.DATA6
#define IICD_DATA7                      _IICD.Bits.DATA7

#define IICD_DATA0_MASK                 1
#define IICD_DATA1_MASK                 2
#define IICD_DATA2_MASK                 4
#define IICD_DATA3_MASK                 8
#define IICD_DATA4_MASK                 16
#define IICD_DATA5_MASK                 32
#define IICD_DATA6_MASK                 64
#define IICD_DATA7_MASK                 128


/*** IICC2 - IIC Control Register 2; 0xFFFF8195 ***/
typedef union {
  byte Byte;
  struct {
    byte AD8         :1;                                       /* Slave Address Bit 8 */
    byte AD9         :1;                                       /* Slave Address Bit 9 */
    byte AD10        :1;                                       /* Slave Address Bit 10 */
    byte             :1; 
    byte SBRC        :1;                                       /* Slave Baud Rate Control */
    byte HDRS        :1;                                       /* High drive select */
    byte ADEXT       :1;                                       /* Address Extension */
    byte GCAEN       :1;                                       /* General Call Address Enable */
  } Bits;
  struct {
    byte grpAD_8 :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IICC2STR;
extern volatile IICC2STR _IICC2 @0xFFFF8195;
#define IICC2                           _IICC2.Byte
#define IICC2_AD8                       _IICC2.Bits.AD8
#define IICC2_AD9                       _IICC2.Bits.AD9
#define IICC2_AD10                      _IICC2.Bits.AD10
#define IICC2_SBRC                      _IICC2.Bits.SBRC
#define IICC2_HDRS                      _IICC2.Bits.HDRS
#define IICC2_ADEXT                     _IICC2.Bits.ADEXT
#define IICC2_GCAEN                     _IICC2.Bits.GCAEN
#define IICC2_AD_8                      _IICC2.MergedBits.grpAD_8
#define IICC2_AD                        IICC2_AD_8

#define IICC2_AD8_MASK                  1
#define IICC2_AD9_MASK                  2
#define IICC2_AD10_MASK                 4
#define IICC2_SBRC_MASK                 16
#define IICC2_HDRS_MASK                 32
#define IICC2_ADEXT_MASK                64
#define IICC2_GCAEN_MASK                128
#define IICC2_AD_8_MASK                 7
#define IICC2_AD_8_BITNUM               0


/*** IICFLT - IIC Filter register; 0xFFFF8196 ***/
typedef union {
  byte Byte;
  struct {
    byte FLT0        :1;                                       /* Filter value bit 0 */
    byte FLT1        :1;                                       /* Filter value bit 1 */
    byte FLT2        :1;                                       /* Filter value bit 2 */
    byte FLT3        :1;                                       /* Filter value bit 3 */
    byte FLT4        :1;                                       /* Filter value bit 4 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpFLT  :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IICFLTSTR;
extern volatile IICFLTSTR _IICFLT @0xFFFF8196;
#define IICFLT                          _IICFLT.Byte
#define IICFLT_FLT0                     _IICFLT.Bits.FLT0
#define IICFLT_FLT1                     _IICFLT.Bits.FLT1
#define IICFLT_FLT2                     _IICFLT.Bits.FLT2
#define IICFLT_FLT3                     _IICFLT.Bits.FLT3
#define IICFLT_FLT4                     _IICFLT.Bits.FLT4
#define IICFLT_FLT                      _IICFLT.MergedBits.grpFLT

#define IICFLT_FLT0_MASK                1
#define IICFLT_FLT1_MASK                2
#define IICFLT_FLT2_MASK                4
#define IICFLT_FLT3_MASK                8
#define IICFLT_FLT4_MASK                16
#define IICFLT_FLT_MASK                 31
#define IICFLT_FLT_BITNUM               0


/*** IICSMB - SMBus Control and Status Register; 0xFFFF8197 ***/
typedef union {
  byte Byte;
  struct {
    byte SHTF2IE     :1;                                       /* SHTF2 Interrupt Enable */
    byte SHTF2       :1;                                       /* SCL High Timeout Flag 2 */
    byte SHTF1       :1;                                       /* SCL High Timeout Flag 1 */
    byte SLTF        :1;                                       /* SCL Low Timeout Flag */
    byte TCKSEL      :1;                                       /* Time Out Counter Clock Select */
    byte SIICAEN     :1;                                       /* Second IIC Address Enable */
    byte ALERTEN     :1;                                       /* SMBus Alert Response Address Enable */
    byte FACK        :1;                                       /* Fast NACK/ACK enable */
  } Bits;
} IICSMBSTR;
extern volatile IICSMBSTR _IICSMB @0xFFFF8197;
#define IICSMB                          _IICSMB.Byte
#define IICSMB_SHTF2IE                  _IICSMB.Bits.SHTF2IE
#define IICSMB_SHTF2                    _IICSMB.Bits.SHTF2
#define IICSMB_SHTF1                    _IICSMB.Bits.SHTF1
#define IICSMB_SLTF                     _IICSMB.Bits.SLTF
#define IICSMB_TCKSEL                   _IICSMB.Bits.TCKSEL
#define IICSMB_SIICAEN                  _IICSMB.Bits.SIICAEN
#define IICSMB_ALERTEN                  _IICSMB.Bits.ALERTEN
#define IICSMB_FACK                     _IICSMB.Bits.FACK

#define IICSMB_SHTF2IE_MASK             1
#define IICSMB_SHTF2_MASK               2
#define IICSMB_SHTF1_MASK               4
#define IICSMB_SLTF_MASK                8
#define IICSMB_TCKSEL_MASK              16
#define IICSMB_SIICAEN_MASK             32
#define IICSMB_ALERTEN_MASK             64
#define IICSMB_FACK_MASK                128


/*** IICA2 - IIC Address Register 2; 0xFFFF8198 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte SAD1        :1;                                       /* SMBus Address Bit 1 */
    byte SAD2        :1;                                       /* SMBus Address Bit 2 */
    byte SAD3        :1;                                       /* SMBus Address Bit 3 */
    byte SAD4        :1;                                       /* SMBus Address Bit 4 */
    byte SAD5        :1;                                       /* SMBus Address Bit 5 */
    byte SAD6        :1;                                       /* SMBus Address Bit 6 */
    byte SAD7        :1;                                       /* SMBus Address Bit 7 */
  } Bits;
  struct {
    byte         :1;
    byte grpSAD_1 :7;
  } MergedBits;
} IICA2STR;
extern volatile IICA2STR _IICA2 @0xFFFF8198;
#define IICA2                           _IICA2.Byte
#define IICA2_SAD1                      _IICA2.Bits.SAD1
#define IICA2_SAD2                      _IICA2.Bits.SAD2
#define IICA2_SAD3                      _IICA2.Bits.SAD3
#define IICA2_SAD4                      _IICA2.Bits.SAD4
#define IICA2_SAD5                      _IICA2.Bits.SAD5
#define IICA2_SAD6                      _IICA2.Bits.SAD6
#define IICA2_SAD7                      _IICA2.Bits.SAD7
#define IICA2_SAD_1                     _IICA2.MergedBits.grpSAD_1
#define IICA2_SAD                       IICA2_SAD_1

#define IICA2_SAD1_MASK                 2
#define IICA2_SAD2_MASK                 4
#define IICA2_SAD3_MASK                 8
#define IICA2_SAD4_MASK                 16
#define IICA2_SAD5_MASK                 32
#define IICA2_SAD6_MASK                 64
#define IICA2_SAD7_MASK                 128
#define IICA2_SAD_1_MASK                254
#define IICA2_SAD_1_BITNUM              1


/*** IICSLT - IIC SCL Low Time Out register; 0xFFFF8199 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** IICSLTH - IIC SCL Low Time Out register - High byte; 0xFFFF8199 ***/
    union {
      byte Byte;
      struct {
        byte SSLT8       :1;                                       /* SCL Low Time Out Bit 8 */
        byte SSLT9       :1;                                       /* SCL Low Time Out Bit 9 */
        byte SSLT10      :1;                                       /* SCL Low Time Out Bit 10 */
        byte SSLT11      :1;                                       /* SCL Low Time Out Bit 11 */
        byte SSLT12      :1;                                       /* SCL Low Time Out Bit 12 */
        byte SSLT13      :1;                                       /* SCL Low Time Out Bit 13 */
        byte SSLT14      :1;                                       /* SCL Low Time Out Bit 14 */
        byte SSLT15      :1;                                       /* SCL Low Time Out Bit 15 */
      } Bits;
    } IICSLTHSTR;
    #define IICSLTH                     _IICSLT.Overlap_STR.IICSLTHSTR.Byte
    #define IICSLTH_SSLT8               _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT8
    #define IICSLTH_SSLT9               _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT9
    #define IICSLTH_SSLT10              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT10
    #define IICSLTH_SSLT11              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT11
    #define IICSLTH_SSLT12              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT12
    #define IICSLTH_SSLT13              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT13
    #define IICSLTH_SSLT14              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT14
    #define IICSLTH_SSLT15              _IICSLT.Overlap_STR.IICSLTHSTR.Bits.SSLT15
    
    #define IICSLTH_SSLT8_MASK          1
    #define IICSLTH_SSLT9_MASK          2
    #define IICSLTH_SSLT10_MASK         4
    #define IICSLTH_SSLT11_MASK         8
    #define IICSLTH_SSLT12_MASK         16
    #define IICSLTH_SSLT13_MASK         32
    #define IICSLTH_SSLT14_MASK         64
    #define IICSLTH_SSLT15_MASK         128
    

    /*** IICSLTL - IIC SCL Low Time Out register - Low byte; 0xFFFF819A ***/
    union {
      byte Byte;
      struct {
        byte SSLT0       :1;                                       /* SCL Low Time Out Bits, bit 0 */
        byte SSLT1       :1;                                       /* SCL Low Time Out Bits, bit 1 */
        byte SSLT2       :1;                                       /* SCL Low Time Out Bits, bit 2 */
        byte SSLT3       :1;                                       /* SCL Low Time Out Bits, bit 3 */
        byte SSLT4       :1;                                       /* SCL Low Time Out Bits, bit 4 */
        byte SSLT5       :1;                                       /* SCL Low Time Out Bits, bit 5 */
        byte SSLT6       :1;                                       /* SCL Low Time Out Bits, bit 6 */
        byte SSLT7       :1;                                       /* SCL Low Time Out Bits, bit 7 */
      } Bits;
    } IICSLTLSTR;
    #define IICSLTL                     _IICSLT.Overlap_STR.IICSLTLSTR.Byte
    #define IICSLTL_SSLT0               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT0
    #define IICSLTL_SSLT1               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT1
    #define IICSLTL_SSLT2               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT2
    #define IICSLTL_SSLT3               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT3
    #define IICSLTL_SSLT4               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT4
    #define IICSLTL_SSLT5               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT5
    #define IICSLTL_SSLT6               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT6
    #define IICSLTL_SSLT7               _IICSLT.Overlap_STR.IICSLTLSTR.Bits.SSLT7
    
    #define IICSLTL_SSLT0_MASK          1
    #define IICSLTL_SSLT1_MASK          2
    #define IICSLTL_SSLT2_MASK          4
    #define IICSLTL_SSLT3_MASK          8
    #define IICSLTL_SSLT4_MASK          16
    #define IICSLTL_SSLT5_MASK          32
    #define IICSLTL_SSLT6_MASK          64
    #define IICSLTL_SSLT7_MASK          128
    
  } Overlap_STR;

  struct {
    word SSLT0       :1;                                       /* SCL Low Time Out Bits, bit 0 */
    word SSLT1       :1;                                       /* SCL Low Time Out Bits, bit 1 */
    word SSLT2       :1;                                       /* SCL Low Time Out Bits, bit 2 */
    word SSLT3       :1;                                       /* SCL Low Time Out Bits, bit 3 */
    word SSLT4       :1;                                       /* SCL Low Time Out Bits, bit 4 */
    word SSLT5       :1;                                       /* SCL Low Time Out Bits, bit 5 */
    word SSLT6       :1;                                       /* SCL Low Time Out Bits, bit 6 */
    word SSLT7       :1;                                       /* SCL Low Time Out Bits, bit 7 */
    word SSLT8       :1;                                       /* SCL Low Time Out Bits, bit 8 */
    word SSLT9       :1;                                       /* SCL Low Time Out Bits, bit 9 */
    word SSLT10      :1;                                       /* SCL Low Time Out Bits, bit 10 */
    word SSLT11      :1;                                       /* SCL Low Time Out Bits, bit 11 */
    word SSLT12      :1;                                       /* SCL Low Time Out Bits, bit 12 */
    word SSLT13      :1;                                       /* SCL Low Time Out Bits, bit 13 */
    word SSLT14      :1;                                       /* SCL Low Time Out Bits, bit 14 */
    word SSLT15      :1;                                       /* SCL Low Time Out Bits, bit 15 */
  } Bits;
} IICSLTSTR;
extern volatile IICSLTSTR _IICSLT @0xFFFF8199;
#define IICSLT                          _IICSLT.Word
#define IICSLT_SSLT0                    _IICSLT.Bits.SSLT0
#define IICSLT_SSLT1                    _IICSLT.Bits.SSLT1
#define IICSLT_SSLT2                    _IICSLT.Bits.SSLT2
#define IICSLT_SSLT3                    _IICSLT.Bits.SSLT3
#define IICSLT_SSLT4                    _IICSLT.Bits.SSLT4
#define IICSLT_SSLT5                    _IICSLT.Bits.SSLT5
#define IICSLT_SSLT6                    _IICSLT.Bits.SSLT6
#define IICSLT_SSLT7                    _IICSLT.Bits.SSLT7
#define IICSLT_SSLT8                    _IICSLT.Bits.SSLT8
#define IICSLT_SSLT9                    _IICSLT.Bits.SSLT9
#define IICSLT_SSLT10                   _IICSLT.Bits.SSLT10
#define IICSLT_SSLT11                   _IICSLT.Bits.SSLT11
#define IICSLT_SSLT12                   _IICSLT.Bits.SSLT12
#define IICSLT_SSLT13                   _IICSLT.Bits.SSLT13
#define IICSLT_SSLT14                   _IICSLT.Bits.SSLT14
#define IICSLT_SSLT15                   _IICSLT.Bits.SSLT15

#define IICSLT_SSLT0_MASK               1
#define IICSLT_SSLT1_MASK               2
#define IICSLT_SSLT2_MASK               4
#define IICSLT_SSLT3_MASK               8
#define IICSLT_SSLT4_MASK               16
#define IICSLT_SSLT5_MASK               32
#define IICSLT_SSLT6_MASK               64
#define IICSLT_SSLT7_MASK               128
#define IICSLT_SSLT8_MASK               256
#define IICSLT_SSLT9_MASK               512
#define IICSLT_SSLT10_MASK              1024
#define IICSLT_SSLT11_MASK              2048
#define IICSLT_SSLT12_MASK              4096
#define IICSLT_SSLT13_MASK              8192
#define IICSLT_SSLT14_MASK              16384
#define IICSLT_SSLT15_MASK              32768


/*** SPI1C1 - SPI1 Control Register 1; 0xFFFF81A0 ***/
typedef union {
  byte Byte;
  struct {
    byte LSBFE       :1;                                       /* LSB First (Shifter Direction) */
    byte SSOE        :1;                                       /* Slave Select Output Enable */
    byte CPHA        :1;                                       /* Clock Phase */
    byte CPOL        :1;                                       /* Clock Polarity */
    byte MSTR        :1;                                       /* Master/Slave Mode Select */
    byte SPTIE       :1;                                       /* SPI Transmit Interrupt Enable */
    byte SPE         :1;                                       /* SPI System Enable */
    byte SPIE        :1;                                       /* SPI Interrupt Enable (for SPRF and MODF) */
  } Bits;
} SPI1C1STR;
extern volatile SPI1C1STR _SPI1C1 @0xFFFF81A0;
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


/*** SPI1C2 - SPI1 Control Register 2; 0xFFFF81A1 ***/
typedef union {
  byte Byte;
  struct {
    byte SPC0        :1;                                       /* SPI Pin Control 0 */
    byte SPISWAI     :1;                                       /* SPI Stop in Wait Mode */
    byte RXDMAE      :1;                                       /* Receive DMA Enable */
    byte BIDIROE     :1;                                       /* Bidirectional Mode Output Enable */
    byte MODFEN      :1;                                       /* Master Mode-Fault Function Enable */
    byte TXDMAE      :1;                                       /* Transmit DMA Enable */
    byte SPIMODE     :1;                                       /* SPI 8- or 16-bit Mode Select */
    byte SPMIE       :1;                                       /* SPI Match Interrupt Enable */
  } Bits;
} SPI1C2STR;
extern volatile SPI1C2STR _SPI1C2 @0xFFFF81A1;
#define SPI1C2                          _SPI1C2.Byte
#define SPI1C2_SPC0                     _SPI1C2.Bits.SPC0
#define SPI1C2_SPISWAI                  _SPI1C2.Bits.SPISWAI
#define SPI1C2_RXDMAE                   _SPI1C2.Bits.RXDMAE
#define SPI1C2_BIDIROE                  _SPI1C2.Bits.BIDIROE
#define SPI1C2_MODFEN                   _SPI1C2.Bits.MODFEN
#define SPI1C2_TXDMAE                   _SPI1C2.Bits.TXDMAE
#define SPI1C2_SPIMODE                  _SPI1C2.Bits.SPIMODE
#define SPI1C2_SPMIE                    _SPI1C2.Bits.SPMIE

#define SPI1C2_SPC0_MASK                1
#define SPI1C2_SPISWAI_MASK             2
#define SPI1C2_RXDMAE_MASK              4
#define SPI1C2_BIDIROE_MASK             8
#define SPI1C2_MODFEN_MASK              16
#define SPI1C2_TXDMAE_MASK              32
#define SPI1C2_SPIMODE_MASK             64
#define SPI1C2_SPMIE_MASK               128


/*** SPI1BR - SPI1 Baud Rate Register; 0xFFFF81A2 ***/
typedef union {
  byte Byte;
  struct {
    byte SPR0        :1;                                       /* SPI Baud Rate Divisor Bit 0 */
    byte SPR1        :1;                                       /* SPI Baud Rate Divisor Bit 1 */
    byte SPR2        :1;                                       /* SPI Baud Rate Divisor Bit 2 */
    byte SPR3        :1;                                       /* SPI Baud Rate Divisor Bit 3 */
    byte SPPR0       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 0 */
    byte SPPR1       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 1 */
    byte SPPR2       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpSPR  :4;
    byte grpSPPR :3;
    byte         :1;
  } MergedBits;
} SPI1BRSTR;
extern volatile SPI1BRSTR _SPI1BR @0xFFFF81A2;
#define SPI1BR                          _SPI1BR.Byte
#define SPI1BR_SPR0                     _SPI1BR.Bits.SPR0
#define SPI1BR_SPR1                     _SPI1BR.Bits.SPR1
#define SPI1BR_SPR2                     _SPI1BR.Bits.SPR2
#define SPI1BR_SPR3                     _SPI1BR.Bits.SPR3
#define SPI1BR_SPPR0                    _SPI1BR.Bits.SPPR0
#define SPI1BR_SPPR1                    _SPI1BR.Bits.SPPR1
#define SPI1BR_SPPR2                    _SPI1BR.Bits.SPPR2
#define SPI1BR_SPR                      _SPI1BR.MergedBits.grpSPR
#define SPI1BR_SPPR                     _SPI1BR.MergedBits.grpSPPR

#define SPI1BR_SPR0_MASK                1
#define SPI1BR_SPR1_MASK                2
#define SPI1BR_SPR2_MASK                4
#define SPI1BR_SPR3_MASK                8
#define SPI1BR_SPPR0_MASK               16
#define SPI1BR_SPPR1_MASK               32
#define SPI1BR_SPPR2_MASK               64
#define SPI1BR_SPR_MASK                 15
#define SPI1BR_SPR_BITNUM               0
#define SPI1BR_SPPR_MASK                112
#define SPI1BR_SPPR_BITNUM              4


/*** SPI1S - SPI1 Status Register; 0xFFFF81A3 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte MODF        :1;                                       /* Master Mode Fault Flag */
    byte SPTEF       :1;                                       /* SPI Transmit Buffer Empty Flag */
    byte SPMF        :1;                                       /* SPI Match Flag */
    byte SPRF        :1;                                       /* SPI Read Buffer Full Flag */
  } Bits;
} SPI1SSTR;
extern volatile SPI1SSTR _SPI1S @0xFFFF81A3;
#define SPI1S                           _SPI1S.Byte
#define SPI1S_MODF                      _SPI1S.Bits.MODF
#define SPI1S_SPTEF                     _SPI1S.Bits.SPTEF
#define SPI1S_SPMF                      _SPI1S.Bits.SPMF
#define SPI1S_SPRF                      _SPI1S.Bits.SPRF

#define SPI1S_MODF_MASK                 16
#define SPI1S_SPTEF_MASK                32
#define SPI1S_SPMF_MASK                 64
#define SPI1S_SPRF_MASK                 128


/*** SPI1D16 - SPI1 Data Register; 0xFFFF81A4 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SPI1DH - SPI1 Data Register High; 0xFFFF81A4 ***/
    union {
      byte Byte;
    } SPI1DHSTR;
    #define SPI1DH                      _SPI1D16.Overlap_STR.SPI1DHSTR.Byte
    

    /*** SPI1DL - SPI1 Data Register Low; 0xFFFF81A5 ***/
    union {
      byte Byte;
      union { /* Several registers at the same address */
        /*** SPI1DL - SPI1 Data Register Low; Several registers at the same address ***/
        #define SPI1DL                  _SPI1D16.Overlap_STR.SPI1DLSTR.Byte
        
        /*** SPI1D - SPI1 Data Register Low; Several registers at the same address ***/
        union {
          struct {
            byte Bit0        :1;                                       /* SPI Data Bit 0 */
            byte Bit1        :1;                                       /* SPI Data Bit 1 */
            byte Bit2        :1;                                       /* SPI Data Bit 2 */
            byte Bit3        :1;                                       /* SPI Data Bit 3 */
            byte Bit4        :1;                                       /* SPI Data Bit 4 */
            byte Bit5        :1;                                       /* SPI Data Bit 5 */
            byte Bit6        :1;                                       /* SPI Data Bit 6 */
            byte Bit7        :1;                                       /* SPI Data Bit 7 */
          } Bits;
        } SPI1DSTR;
        #define SPI1D                   _SPI1D16.Overlap_STR.SPI1DLSTR.Byte
        #define SPI1D_Bit0              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit0
        #define SPI1D_Bit1              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit1
        #define SPI1D_Bit2              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit2
        #define SPI1D_Bit3              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit3
        #define SPI1D_Bit4              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit4
        #define SPI1D_Bit5              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit5
        #define SPI1D_Bit6              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit6
        #define SPI1D_Bit7              _SPI1D16.Overlap_STR.SPI1DLSTR.SameAddr_STR.SPI1DSTR.Bits.Bit7
        
        #define SPI1D_Bit0_MASK         1
        #define SPI1D_Bit1_MASK         2
        #define SPI1D_Bit2_MASK         4
        #define SPI1D_Bit3_MASK         8
        #define SPI1D_Bit4_MASK         16
        #define SPI1D_Bit5_MASK         32
        #define SPI1D_Bit6_MASK         64
        #define SPI1D_Bit7_MASK         128
        
      } SameAddr_STR; /*Several registers at the same address */
    
    } SPI1DLSTR;
    
  } Overlap_STR;

} SPI1D16STR;
extern volatile SPI1D16STR _SPI1D16 @0xFFFF81A4;
#define SPI1D16                         _SPI1D16.Word


/*** SPI1M - SPI1 Match Register; 0xFFFF81A6 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SPI1MH - SPI1 Match Register High; 0xFFFF81A6 ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* SPI Match Value Bit 8 */
        byte Bit9        :1;                                       /* SPI Match Value Bit 9 */
        byte Bit10       :1;                                       /* SPI Match Value Bit 10 */
        byte Bit11       :1;                                       /* SPI Match Value Bit 11 */
        byte Bit12       :1;                                       /* SPI Match Value Bit 12 */
        byte Bit13       :1;                                       /* SPI Match Value Bit 13 */
        byte Bit14       :1;                                       /* SPI Match Value Bit 14 */
        byte Bit15       :1;                                       /* SPI Match Value Bit 15 */
      } Bits;
    } SPI1MHSTR;
    #define SPI1MH                      _SPI1M.Overlap_STR.SPI1MHSTR.Byte
    #define SPI1MH_Bit8                 _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit8
    #define SPI1MH_Bit9                 _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit9
    #define SPI1MH_Bit10                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit10
    #define SPI1MH_Bit11                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit11
    #define SPI1MH_Bit12                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit12
    #define SPI1MH_Bit13                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit13
    #define SPI1MH_Bit14                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit14
    #define SPI1MH_Bit15                _SPI1M.Overlap_STR.SPI1MHSTR.Bits.Bit15
    
    #define SPI1MH_Bit8_MASK            1
    #define SPI1MH_Bit9_MASK            2
    #define SPI1MH_Bit10_MASK           4
    #define SPI1MH_Bit11_MASK           8
    #define SPI1MH_Bit12_MASK           16
    #define SPI1MH_Bit13_MASK           32
    #define SPI1MH_Bit14_MASK           64
    #define SPI1MH_Bit15_MASK           128
    

    /*** SPI1ML - SPI1 Match Register Low; 0xFFFF81A7 ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* SPI Match Value Bit 0 */
        byte Bit1        :1;                                       /* SPI Match Value Bit 1 */
        byte Bit2        :1;                                       /* SPI Match Value Bit 2 */
        byte Bit3        :1;                                       /* SPI Match Value Bit 3 */
        byte Bit4        :1;                                       /* SPI Match Value Bit 4 */
        byte Bit5        :1;                                       /* SPI Match Value Bit 5 */
        byte Bit6        :1;                                       /* SPI Match Value Bit 6 */
        byte Bit7        :1;                                       /* SPI Match Value Bit 7 */
      } Bits;
    } SPI1MLSTR;
    #define SPI1ML                      _SPI1M.Overlap_STR.SPI1MLSTR.Byte
    #define SPI1ML_Bit0                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit0
    #define SPI1ML_Bit1                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit1
    #define SPI1ML_Bit2                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit2
    #define SPI1ML_Bit3                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit3
    #define SPI1ML_Bit4                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit4
    #define SPI1ML_Bit5                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit5
    #define SPI1ML_Bit6                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit6
    #define SPI1ML_Bit7                 _SPI1M.Overlap_STR.SPI1MLSTR.Bits.Bit7
    
    #define SPI1ML_Bit0_MASK            1
    #define SPI1ML_Bit1_MASK            2
    #define SPI1ML_Bit2_MASK            4
    #define SPI1ML_Bit3_MASK            8
    #define SPI1ML_Bit4_MASK            16
    #define SPI1ML_Bit5_MASK            32
    #define SPI1ML_Bit6_MASK            64
    #define SPI1ML_Bit7_MASK            128
    
  } Overlap_STR;

  struct {
    word Bit0        :1;                                       /* SPI Match Value Bit 0 */
    word Bit1        :1;                                       /* SPI Match Value Bit 1 */
    word Bit2        :1;                                       /* SPI Match Value Bit 2 */
    word Bit3        :1;                                       /* SPI Match Value Bit 3 */
    word Bit4        :1;                                       /* SPI Match Value Bit 4 */
    word Bit5        :1;                                       /* SPI Match Value Bit 5 */
    word Bit6        :1;                                       /* SPI Match Value Bit 6 */
    word Bit7        :1;                                       /* SPI Match Value Bit 7 */
    word Bit8        :1;                                       /* SPI Match Value Bit 8 */
    word Bit9        :1;                                       /* SPI Match Value Bit 9 */
    word Bit10       :1;                                       /* SPI Match Value Bit 10 */
    word Bit11       :1;                                       /* SPI Match Value Bit 11 */
    word Bit12       :1;                                       /* SPI Match Value Bit 12 */
    word Bit13       :1;                                       /* SPI Match Value Bit 13 */
    word Bit14       :1;                                       /* SPI Match Value Bit 14 */
    word Bit15       :1;                                       /* SPI Match Value Bit 15 */
  } Bits;
} SPI1MSTR;
extern volatile SPI1MSTR _SPI1M @0xFFFF81A6;
#define SPI1M                           _SPI1M.Word
#define SPI1M_Bit0                      _SPI1M.Bits.Bit0
#define SPI1M_Bit1                      _SPI1M.Bits.Bit1
#define SPI1M_Bit2                      _SPI1M.Bits.Bit2
#define SPI1M_Bit3                      _SPI1M.Bits.Bit3
#define SPI1M_Bit4                      _SPI1M.Bits.Bit4
#define SPI1M_Bit5                      _SPI1M.Bits.Bit5
#define SPI1M_Bit6                      _SPI1M.Bits.Bit6
#define SPI1M_Bit7                      _SPI1M.Bits.Bit7
#define SPI1M_Bit8                      _SPI1M.Bits.Bit8
#define SPI1M_Bit9                      _SPI1M.Bits.Bit9
#define SPI1M_Bit10                     _SPI1M.Bits.Bit10
#define SPI1M_Bit11                     _SPI1M.Bits.Bit11
#define SPI1M_Bit12                     _SPI1M.Bits.Bit12
#define SPI1M_Bit13                     _SPI1M.Bits.Bit13
#define SPI1M_Bit14                     _SPI1M.Bits.Bit14
#define SPI1M_Bit15                     _SPI1M.Bits.Bit15

#define SPI1M_Bit0_MASK                 1
#define SPI1M_Bit1_MASK                 2
#define SPI1M_Bit2_MASK                 4
#define SPI1M_Bit3_MASK                 8
#define SPI1M_Bit4_MASK                 16
#define SPI1M_Bit5_MASK                 32
#define SPI1M_Bit6_MASK                 64
#define SPI1M_Bit7_MASK                 128
#define SPI1M_Bit8_MASK                 256
#define SPI1M_Bit9_MASK                 512
#define SPI1M_Bit10_MASK                1024
#define SPI1M_Bit11_MASK                2048
#define SPI1M_Bit12_MASK                4096
#define SPI1M_Bit13_MASK                8192
#define SPI1M_Bit14_MASK                16384
#define SPI1M_Bit15_MASK                32768


/*** SPI2C1 - SPI2 Control Register 1; 0xFFFF81A8 ***/
typedef union {
  byte Byte;
  struct {
    byte LSBFE       :1;                                       /* LSB First (Shifter Direction) */
    byte SSOE        :1;                                       /* Slave Select Output Enable */
    byte CPHA        :1;                                       /* Clock Phase */
    byte CPOL        :1;                                       /* Clock Polarity */
    byte MSTR        :1;                                       /* Master/Slave Mode Select */
    byte SPTIE       :1;                                       /* SPI Transmit Interrupt Enable */
    byte SPE         :1;                                       /* SPI System Enable */
    byte SPIE        :1;                                       /* SPI Interrupt Enable (for SPRF and MODF) */
  } Bits;
} SPI2C1STR;
extern volatile SPI2C1STR _SPI2C1 @0xFFFF81A8;
#define SPI2C1                          _SPI2C1.Byte
#define SPI2C1_LSBFE                    _SPI2C1.Bits.LSBFE
#define SPI2C1_SSOE                     _SPI2C1.Bits.SSOE
#define SPI2C1_CPHA                     _SPI2C1.Bits.CPHA
#define SPI2C1_CPOL                     _SPI2C1.Bits.CPOL
#define SPI2C1_MSTR                     _SPI2C1.Bits.MSTR
#define SPI2C1_SPTIE                    _SPI2C1.Bits.SPTIE
#define SPI2C1_SPE                      _SPI2C1.Bits.SPE
#define SPI2C1_SPIE                     _SPI2C1.Bits.SPIE

#define SPI2C1_LSBFE_MASK               1
#define SPI2C1_SSOE_MASK                2
#define SPI2C1_CPHA_MASK                4
#define SPI2C1_CPOL_MASK                8
#define SPI2C1_MSTR_MASK                16
#define SPI2C1_SPTIE_MASK               32
#define SPI2C1_SPE_MASK                 64
#define SPI2C1_SPIE_MASK                128


/*** SPI2C2 - SPI2 Control Register 2; 0xFFFF81A9 ***/
typedef union {
  byte Byte;
  struct {
    byte SPC0        :1;                                       /* SPI Pin Control 0 */
    byte SPISWAI     :1;                                       /* SPI Stop in Wait Mode */
    byte RXDMAE      :1;                                       /* Receive DMA Enable */
    byte BIDIROE     :1;                                       /* Bidirectional Mode Output Enable */
    byte MODFEN      :1;                                       /* Master Mode-Fault Function Enable */
    byte TXDMAE      :1;                                       /* Transmit DMA Enable */
    byte SPIMODE     :1;                                       /* SPI 8- or 16-bit Mode Select */
    byte SPMIE       :1;                                       /* SPI Match Interrupt Enable */
  } Bits;
} SPI2C2STR;
extern volatile SPI2C2STR _SPI2C2 @0xFFFF81A9;
#define SPI2C2                          _SPI2C2.Byte
#define SPI2C2_SPC0                     _SPI2C2.Bits.SPC0
#define SPI2C2_SPISWAI                  _SPI2C2.Bits.SPISWAI
#define SPI2C2_RXDMAE                   _SPI2C2.Bits.RXDMAE
#define SPI2C2_BIDIROE                  _SPI2C2.Bits.BIDIROE
#define SPI2C2_MODFEN                   _SPI2C2.Bits.MODFEN
#define SPI2C2_TXDMAE                   _SPI2C2.Bits.TXDMAE
#define SPI2C2_SPIMODE                  _SPI2C2.Bits.SPIMODE
#define SPI2C2_SPMIE                    _SPI2C2.Bits.SPMIE

#define SPI2C2_SPC0_MASK                1
#define SPI2C2_SPISWAI_MASK             2
#define SPI2C2_RXDMAE_MASK              4
#define SPI2C2_BIDIROE_MASK             8
#define SPI2C2_MODFEN_MASK              16
#define SPI2C2_TXDMAE_MASK              32
#define SPI2C2_SPIMODE_MASK             64
#define SPI2C2_SPMIE_MASK               128


/*** SPI2BR - SPI2 Baud Rate Register; 0xFFFF81AA ***/
typedef union {
  byte Byte;
  struct {
    byte SPR0        :1;                                       /* SPI Baud Rate Divisor Bit 0 */
    byte SPR1        :1;                                       /* SPI Baud Rate Divisor Bit 1 */
    byte SPR2        :1;                                       /* SPI Baud Rate Divisor Bit 2 */
    byte SPR3        :1;                                       /* SPI Baud Rate Divisor Bit 3 */
    byte SPPR0       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 0 */
    byte SPPR1       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 1 */
    byte SPPR2       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpSPR  :4;
    byte grpSPPR :3;
    byte         :1;
  } MergedBits;
} SPI2BRSTR;
extern volatile SPI2BRSTR _SPI2BR @0xFFFF81AA;
#define SPI2BR                          _SPI2BR.Byte
#define SPI2BR_SPR0                     _SPI2BR.Bits.SPR0
#define SPI2BR_SPR1                     _SPI2BR.Bits.SPR1
#define SPI2BR_SPR2                     _SPI2BR.Bits.SPR2
#define SPI2BR_SPR3                     _SPI2BR.Bits.SPR3
#define SPI2BR_SPPR0                    _SPI2BR.Bits.SPPR0
#define SPI2BR_SPPR1                    _SPI2BR.Bits.SPPR1
#define SPI2BR_SPPR2                    _SPI2BR.Bits.SPPR2
#define SPI2BR_SPR                      _SPI2BR.MergedBits.grpSPR
#define SPI2BR_SPPR                     _SPI2BR.MergedBits.grpSPPR

#define SPI2BR_SPR0_MASK                1
#define SPI2BR_SPR1_MASK                2
#define SPI2BR_SPR2_MASK                4
#define SPI2BR_SPR3_MASK                8
#define SPI2BR_SPPR0_MASK               16
#define SPI2BR_SPPR1_MASK               32
#define SPI2BR_SPPR2_MASK               64
#define SPI2BR_SPR_MASK                 15
#define SPI2BR_SPR_BITNUM               0
#define SPI2BR_SPPR_MASK                112
#define SPI2BR_SPPR_BITNUM              4


/*** SPI2S - SPI2 Status Register; 0xFFFF81AB ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte MODF        :1;                                       /* Master Mode Fault Flag */
    byte SPTEF       :1;                                       /* SPI Transmit Buffer Empty Flag */
    byte SPMF        :1;                                       /* SPI Match Flag */
    byte SPRF        :1;                                       /* SPI Read Buffer Full Flag */
  } Bits;
} SPI2SSTR;
extern volatile SPI2SSTR _SPI2S @0xFFFF81AB;
#define SPI2S                           _SPI2S.Byte
#define SPI2S_MODF                      _SPI2S.Bits.MODF
#define SPI2S_SPTEF                     _SPI2S.Bits.SPTEF
#define SPI2S_SPMF                      _SPI2S.Bits.SPMF
#define SPI2S_SPRF                      _SPI2S.Bits.SPRF

#define SPI2S_MODF_MASK                 16
#define SPI2S_SPTEF_MASK                32
#define SPI2S_SPMF_MASK                 64
#define SPI2S_SPRF_MASK                 128


/*** SPI2D16 - SPI2 Data Register; 0xFFFF81AC ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SPI2DH - SPI2 Data Register High; 0xFFFF81AC ***/
    union {
      byte Byte;
    } SPI2DHSTR;
    #define SPI2DH                      _SPI2D16.Overlap_STR.SPI2DHSTR.Byte
    

    /*** SPI2DL - SPI2 Data Register Low; 0xFFFF81AD ***/
    union {
      byte Byte;
      union { /* Several registers at the same address */
        /*** SPI2DL - SPI2 Data Register Low; Several registers at the same address ***/
        #define SPI2DL                  _SPI2D16.Overlap_STR.SPI2DLSTR.Byte
        
        /*** SPI2D - SPI2 Data Register Low; Several registers at the same address ***/
        union {
          struct {
            byte Bit0        :1;                                       /* SPI Data Bit 0 */
            byte Bit1        :1;                                       /* SPI Data Bit 1 */
            byte Bit2        :1;                                       /* SPI Data Bit 2 */
            byte Bit3        :1;                                       /* SPI Data Bit 3 */
            byte Bit4        :1;                                       /* SPI Data Bit 4 */
            byte Bit5        :1;                                       /* SPI Data Bit 5 */
            byte Bit6        :1;                                       /* SPI Data Bit 6 */
            byte Bit7        :1;                                       /* SPI Data Bit 7 */
          } Bits;
        } SPI2DSTR;
        #define SPI2D                   _SPI2D16.Overlap_STR.SPI2DLSTR.Byte
        #define SPI2D_Bit0              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit0
        #define SPI2D_Bit1              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit1
        #define SPI2D_Bit2              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit2
        #define SPI2D_Bit3              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit3
        #define SPI2D_Bit4              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit4
        #define SPI2D_Bit5              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit5
        #define SPI2D_Bit6              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit6
        #define SPI2D_Bit7              _SPI2D16.Overlap_STR.SPI2DLSTR.SameAddr_STR.SPI2DSTR.Bits.Bit7
        
        #define SPI2D_Bit0_MASK         1
        #define SPI2D_Bit1_MASK         2
        #define SPI2D_Bit2_MASK         4
        #define SPI2D_Bit3_MASK         8
        #define SPI2D_Bit4_MASK         16
        #define SPI2D_Bit5_MASK         32
        #define SPI2D_Bit6_MASK         64
        #define SPI2D_Bit7_MASK         128
        
      } SameAddr_STR; /*Several registers at the same address */
    
    } SPI2DLSTR;
    
  } Overlap_STR;

} SPI2D16STR;
extern volatile SPI2D16STR _SPI2D16 @0xFFFF81AC;
#define SPI2D16                         _SPI2D16.Word


/*** SPI2M - SPI2 Match Register; 0xFFFF81AE ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SPI2MH - SPI2 Match Register High; 0xFFFF81AE ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* SPI Match Value Bit 8 */
        byte Bit9        :1;                                       /* SPI Match Value Bit 9 */
        byte Bit10       :1;                                       /* SPI Match Value Bit 10 */
        byte Bit11       :1;                                       /* SPI Match Value Bit 11 */
        byte Bit12       :1;                                       /* SPI Match Value Bit 12 */
        byte Bit13       :1;                                       /* SPI Match Value Bit 13 */
        byte Bit14       :1;                                       /* SPI Match Value Bit 14 */
        byte Bit15       :1;                                       /* SPI Match Value Bit 15 */
      } Bits;
    } SPI2MHSTR;
    #define SPI2MH                      _SPI2M.Overlap_STR.SPI2MHSTR.Byte
    #define SPI2MH_Bit8                 _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit8
    #define SPI2MH_Bit9                 _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit9
    #define SPI2MH_Bit10                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit10
    #define SPI2MH_Bit11                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit11
    #define SPI2MH_Bit12                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit12
    #define SPI2MH_Bit13                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit13
    #define SPI2MH_Bit14                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit14
    #define SPI2MH_Bit15                _SPI2M.Overlap_STR.SPI2MHSTR.Bits.Bit15
    
    #define SPI2MH_Bit8_MASK            1
    #define SPI2MH_Bit9_MASK            2
    #define SPI2MH_Bit10_MASK           4
    #define SPI2MH_Bit11_MASK           8
    #define SPI2MH_Bit12_MASK           16
    #define SPI2MH_Bit13_MASK           32
    #define SPI2MH_Bit14_MASK           64
    #define SPI2MH_Bit15_MASK           128
    

    /*** SPI2ML - SPI2 Match Register Low; 0xFFFF81AF ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* SPI Match Value Bit 0 */
        byte Bit1        :1;                                       /* SPI Match Value Bit 1 */
        byte Bit2        :1;                                       /* SPI Match Value Bit 2 */
        byte Bit3        :1;                                       /* SPI Match Value Bit 3 */
        byte Bit4        :1;                                       /* SPI Match Value Bit 4 */
        byte Bit5        :1;                                       /* SPI Match Value Bit 5 */
        byte Bit6        :1;                                       /* SPI Match Value Bit 6 */
        byte Bit7        :1;                                       /* SPI Match Value Bit 7 */
      } Bits;
    } SPI2MLSTR;
    #define SPI2ML                      _SPI2M.Overlap_STR.SPI2MLSTR.Byte
    #define SPI2ML_Bit0                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit0
    #define SPI2ML_Bit1                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit1
    #define SPI2ML_Bit2                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit2
    #define SPI2ML_Bit3                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit3
    #define SPI2ML_Bit4                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit4
    #define SPI2ML_Bit5                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit5
    #define SPI2ML_Bit6                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit6
    #define SPI2ML_Bit7                 _SPI2M.Overlap_STR.SPI2MLSTR.Bits.Bit7
    
    #define SPI2ML_Bit0_MASK            1
    #define SPI2ML_Bit1_MASK            2
    #define SPI2ML_Bit2_MASK            4
    #define SPI2ML_Bit3_MASK            8
    #define SPI2ML_Bit4_MASK            16
    #define SPI2ML_Bit5_MASK            32
    #define SPI2ML_Bit6_MASK            64
    #define SPI2ML_Bit7_MASK            128
    
  } Overlap_STR;

  struct {
    word Bit0        :1;                                       /* SPI Match Value Bit 0 */
    word Bit1        :1;                                       /* SPI Match Value Bit 1 */
    word Bit2        :1;                                       /* SPI Match Value Bit 2 */
    word Bit3        :1;                                       /* SPI Match Value Bit 3 */
    word Bit4        :1;                                       /* SPI Match Value Bit 4 */
    word Bit5        :1;                                       /* SPI Match Value Bit 5 */
    word Bit6        :1;                                       /* SPI Match Value Bit 6 */
    word Bit7        :1;                                       /* SPI Match Value Bit 7 */
    word Bit8        :1;                                       /* SPI Match Value Bit 8 */
    word Bit9        :1;                                       /* SPI Match Value Bit 9 */
    word Bit10       :1;                                       /* SPI Match Value Bit 10 */
    word Bit11       :1;                                       /* SPI Match Value Bit 11 */
    word Bit12       :1;                                       /* SPI Match Value Bit 12 */
    word Bit13       :1;                                       /* SPI Match Value Bit 13 */
    word Bit14       :1;                                       /* SPI Match Value Bit 14 */
    word Bit15       :1;                                       /* SPI Match Value Bit 15 */
  } Bits;
} SPI2MSTR;
extern volatile SPI2MSTR _SPI2M @0xFFFF81AE;
#define SPI2M                           _SPI2M.Word
#define SPI2M_Bit0                      _SPI2M.Bits.Bit0
#define SPI2M_Bit1                      _SPI2M.Bits.Bit1
#define SPI2M_Bit2                      _SPI2M.Bits.Bit2
#define SPI2M_Bit3                      _SPI2M.Bits.Bit3
#define SPI2M_Bit4                      _SPI2M.Bits.Bit4
#define SPI2M_Bit5                      _SPI2M.Bits.Bit5
#define SPI2M_Bit6                      _SPI2M.Bits.Bit6
#define SPI2M_Bit7                      _SPI2M.Bits.Bit7
#define SPI2M_Bit8                      _SPI2M.Bits.Bit8
#define SPI2M_Bit9                      _SPI2M.Bits.Bit9
#define SPI2M_Bit10                     _SPI2M.Bits.Bit10
#define SPI2M_Bit11                     _SPI2M.Bits.Bit11
#define SPI2M_Bit12                     _SPI2M.Bits.Bit12
#define SPI2M_Bit13                     _SPI2M.Bits.Bit13
#define SPI2M_Bit14                     _SPI2M.Bits.Bit14
#define SPI2M_Bit15                     _SPI2M.Bits.Bit15

#define SPI2M_Bit0_MASK                 1
#define SPI2M_Bit1_MASK                 2
#define SPI2M_Bit2_MASK                 4
#define SPI2M_Bit3_MASK                 8
#define SPI2M_Bit4_MASK                 16
#define SPI2M_Bit5_MASK                 32
#define SPI2M_Bit6_MASK                 64
#define SPI2M_Bit7_MASK                 128
#define SPI2M_Bit8_MASK                 256
#define SPI2M_Bit9_MASK                 512
#define SPI2M_Bit10_MASK                1024
#define SPI2M_Bit11_MASK                2048
#define SPI2M_Bit12_MASK                4096
#define SPI2M_Bit13_MASK                8192
#define SPI2M_Bit14_MASK                16384
#define SPI2M_Bit15_MASK                32768


/*** HSCMP1CR0 - HSCMP1 Control Register 0; 0xFFFF81B0 ***/
typedef union {
  byte Byte;
  struct {
    byte MMC0        :1;                                       /* Minus Input Mux Control, bit 0 */
    byte MMC1        :1;                                       /* Minus Input Mux Control, bit 1 */
    byte PMC0        :1;                                       /* Positive Input Mux Control, bit 0 */
    byte PMC1        :1;                                       /* Positive Input Mux Control, bit 1 */
    byte FILTER_CNT0 :1;                                       /* Filter Sample Count, bit 0 */
    byte FILTER_CNT1 :1;                                       /* Filter Sample Count, bit 1 */
    byte FILTER_CNT2 :1;                                       /* Filter Sample Count, bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpMMC  :2;
    byte grpPMC  :2;
    byte grpFILTER_CNT :3;
    byte         :1;
  } MergedBits;
} HSCMP1CR0STR;
extern volatile HSCMP1CR0STR _HSCMP1CR0 @0xFFFF81B0;
#define HSCMP1CR0                       _HSCMP1CR0.Byte
#define HSCMP1CR0_MMC0                  _HSCMP1CR0.Bits.MMC0
#define HSCMP1CR0_MMC1                  _HSCMP1CR0.Bits.MMC1
#define HSCMP1CR0_PMC0                  _HSCMP1CR0.Bits.PMC0
#define HSCMP1CR0_PMC1                  _HSCMP1CR0.Bits.PMC1
#define HSCMP1CR0_FILTER_CNT0           _HSCMP1CR0.Bits.FILTER_CNT0
#define HSCMP1CR0_FILTER_CNT1           _HSCMP1CR0.Bits.FILTER_CNT1
#define HSCMP1CR0_FILTER_CNT2           _HSCMP1CR0.Bits.FILTER_CNT2
/* HSCMP1CR_ARR: Access 2 HSCMP1CRx registers in an array */
#define HSCMP1CR_ARR                    ((volatile byte *) &HSCMP1CR0)
#define HSCMP1CR0_MMC                   _HSCMP1CR0.MergedBits.grpMMC
#define HSCMP1CR0_PMC                   _HSCMP1CR0.MergedBits.grpPMC
#define HSCMP1CR0_FILTER_CNT            _HSCMP1CR0.MergedBits.grpFILTER_CNT

#define HSCMP1CR0_MMC0_MASK             1
#define HSCMP1CR0_MMC1_MASK             2
#define HSCMP1CR0_PMC0_MASK             4
#define HSCMP1CR0_PMC1_MASK             8
#define HSCMP1CR0_FILTER_CNT0_MASK      16
#define HSCMP1CR0_FILTER_CNT1_MASK      32
#define HSCMP1CR0_FILTER_CNT2_MASK      64
#define HSCMP1CR0_MMC_MASK              3
#define HSCMP1CR0_MMC_BITNUM            0
#define HSCMP1CR0_PMC_MASK              12
#define HSCMP1CR0_PMC_BITNUM            2
#define HSCMP1CR0_FILTER_CNT_MASK       112
#define HSCMP1CR0_FILTER_CNT_BITNUM     4


/*** HSCMP1CR1 - HSCMP1 Control Register 1; 0xFFFF81B1 ***/
typedef union {
  byte Byte;
  struct {
    byte EN          :1;                                       /* Comparator Module Enable */
    byte OPE         :1;                                       /* Comparator Output Pin Enable */
    byte COS         :1;                                       /* Comparator Output Select */
    byte INV         :1;                                       /* Comparator INVERT */
    byte PMODE       :1;                                       /* Power Mode Select */
    byte             :1; 
    byte WE          :1;                                       /* Windowing Enable */
    byte SE          :1;                                       /* Sample Enable */
  } Bits;
} HSCMP1CR1STR;
extern volatile HSCMP1CR1STR _HSCMP1CR1 @0xFFFF81B1;
#define HSCMP1CR1                       _HSCMP1CR1.Byte
#define HSCMP1CR1_EN                    _HSCMP1CR1.Bits.EN
#define HSCMP1CR1_OPE                   _HSCMP1CR1.Bits.OPE
#define HSCMP1CR1_COS                   _HSCMP1CR1.Bits.COS
#define HSCMP1CR1_INV                   _HSCMP1CR1.Bits.INV
#define HSCMP1CR1_PMODE                 _HSCMP1CR1.Bits.PMODE
#define HSCMP1CR1_WE                    _HSCMP1CR1.Bits.WE
#define HSCMP1CR1_SE                    _HSCMP1CR1.Bits.SE

#define HSCMP1CR1_EN_MASK               1
#define HSCMP1CR1_OPE_MASK              2
#define HSCMP1CR1_COS_MASK              4
#define HSCMP1CR1_INV_MASK              8
#define HSCMP1CR1_PMODE_MASK            16
#define HSCMP1CR1_WE_MASK               64
#define HSCMP1CR1_SE_MASK               128


/*** HSCMP1FPR - HSCMP1 Filter Period Register; 0xFFFF81B2 ***/
typedef union {
  byte Byte;
  struct {
    byte FILT_PER0   :1;                                       /* Filter Period, bit 0 */
    byte FILT_PER1   :1;                                       /* Filter Period, bit 1 */
    byte FILT_PER2   :1;                                       /* Filter Period, bit 2 */
    byte FILT_PER3   :1;                                       /* Filter Period, bit 3 */
    byte FILT_PER4   :1;                                       /* Filter Period, bit 4 */
    byte FILT_PER5   :1;                                       /* Filter Period, bit 5 */
    byte FILT_PER6   :1;                                       /* Filter Period, bit 6 */
    byte FILT_PER7   :1;                                       /* Filter Period, bit 7 */
  } Bits;
} HSCMP1FPRSTR;
extern volatile HSCMP1FPRSTR _HSCMP1FPR @0xFFFF81B2;
#define HSCMP1FPR                       _HSCMP1FPR.Byte
#define HSCMP1FPR_FILT_PER0             _HSCMP1FPR.Bits.FILT_PER0
#define HSCMP1FPR_FILT_PER1             _HSCMP1FPR.Bits.FILT_PER1
#define HSCMP1FPR_FILT_PER2             _HSCMP1FPR.Bits.FILT_PER2
#define HSCMP1FPR_FILT_PER3             _HSCMP1FPR.Bits.FILT_PER3
#define HSCMP1FPR_FILT_PER4             _HSCMP1FPR.Bits.FILT_PER4
#define HSCMP1FPR_FILT_PER5             _HSCMP1FPR.Bits.FILT_PER5
#define HSCMP1FPR_FILT_PER6             _HSCMP1FPR.Bits.FILT_PER6
#define HSCMP1FPR_FILT_PER7             _HSCMP1FPR.Bits.FILT_PER7

#define HSCMP1FPR_FILT_PER0_MASK        1
#define HSCMP1FPR_FILT_PER1_MASK        2
#define HSCMP1FPR_FILT_PER2_MASK        4
#define HSCMP1FPR_FILT_PER3_MASK        8
#define HSCMP1FPR_FILT_PER4_MASK        16
#define HSCMP1FPR_FILT_PER5_MASK        32
#define HSCMP1FPR_FILT_PER6_MASK        64
#define HSCMP1FPR_FILT_PER7_MASK        128


/*** HSCMP1SCR - HSCMP1 Status & Control Register; 0xFFFF81B3 ***/
typedef union {
  byte Byte;
  struct {
    byte COUT        :1;                                       /* Analog Comparator Output */
    byte CFF         :1;                                       /* Analog Comparator Flag Falling */
    byte CFR         :1;                                       /* Analog Comparator Flag Rising */
    byte IEF         :1;                                       /* Comparator Interrupt Enable Falling */
    byte IER         :1;                                       /* Comparator Interrupt Enable Rising */
    byte SMLEB       :1;                                       /* Stop Mode Edge / Level Interrupt Control */
    byte DMAEN       :1;                                       /* DMA enable control */
    byte             :1; 
  } Bits;
} HSCMP1SCRSTR;
extern volatile HSCMP1SCRSTR _HSCMP1SCR @0xFFFF81B3;
#define HSCMP1SCR                       _HSCMP1SCR.Byte
#define HSCMP1SCR_COUT                  _HSCMP1SCR.Bits.COUT
#define HSCMP1SCR_CFF                   _HSCMP1SCR.Bits.CFF
#define HSCMP1SCR_CFR                   _HSCMP1SCR.Bits.CFR
#define HSCMP1SCR_IEF                   _HSCMP1SCR.Bits.IEF
#define HSCMP1SCR_IER                   _HSCMP1SCR.Bits.IER
#define HSCMP1SCR_SMLEB                 _HSCMP1SCR.Bits.SMLEB
#define HSCMP1SCR_DMAEN                 _HSCMP1SCR.Bits.DMAEN

#define HSCMP1SCR_COUT_MASK             1
#define HSCMP1SCR_CFF_MASK              2
#define HSCMP1SCR_CFR_MASK              4
#define HSCMP1SCR_IEF_MASK              8
#define HSCMP1SCR_IER_MASK              16
#define HSCMP1SCR_SMLEB_MASK            32
#define HSCMP1SCR_DMAEN_MASK            64


/*** HSCMP1PCR - Pin Control Register; 0xFFFF81B4 ***/
typedef union {
  byte Byte;
  struct {
    byte INMPE1      :1;                                       /* Minus Input Pin 1 Enable */
    byte INMPE2      :1;                                       /* Minus Input Pin 2 Enable */
    byte INMPE3      :1;                                       /* Minus Input Pin 3 Enable */
    byte INMPE4      :1;                                       /* Minus Input Pin 4 Enable */
    byte INPPE1      :1;                                       /* Positive Input Pin 1 Enable */
    byte INPPE2      :1;                                       /* Positive Input Pin 2 Enable */
    byte INPPE3      :1;                                       /* Positive Input Pin 3 Enable */
    byte INPPE4      :1;                                       /* Positive Input Pin 4 Enable */
  } Bits;
  struct {
    byte grpINMPE_1 :4;
    byte grpINPPE_1 :4;
  } MergedBits;
} HSCMP1PCRSTR;
extern volatile HSCMP1PCRSTR _HSCMP1PCR @0xFFFF81B4;
#define HSCMP1PCR                       _HSCMP1PCR.Byte
#define HSCMP1PCR_INMPE1                _HSCMP1PCR.Bits.INMPE1
#define HSCMP1PCR_INMPE2                _HSCMP1PCR.Bits.INMPE2
#define HSCMP1PCR_INMPE3                _HSCMP1PCR.Bits.INMPE3
#define HSCMP1PCR_INMPE4                _HSCMP1PCR.Bits.INMPE4
#define HSCMP1PCR_INPPE1                _HSCMP1PCR.Bits.INPPE1
#define HSCMP1PCR_INPPE2                _HSCMP1PCR.Bits.INPPE2
#define HSCMP1PCR_INPPE3                _HSCMP1PCR.Bits.INPPE3
#define HSCMP1PCR_INPPE4                _HSCMP1PCR.Bits.INPPE4
#define HSCMP1PCR_INMPE_1               _HSCMP1PCR.MergedBits.grpINMPE_1
#define HSCMP1PCR_INPPE_1               _HSCMP1PCR.MergedBits.grpINPPE_1
#define HSCMP1PCR_INMPE                 HSCMP1PCR_INMPE_1
#define HSCMP1PCR_INPPE                 HSCMP1PCR_INPPE_1

#define HSCMP1PCR_INMPE1_MASK           1
#define HSCMP1PCR_INMPE2_MASK           2
#define HSCMP1PCR_INMPE3_MASK           4
#define HSCMP1PCR_INMPE4_MASK           8
#define HSCMP1PCR_INPPE1_MASK           16
#define HSCMP1PCR_INPPE2_MASK           32
#define HSCMP1PCR_INPPE3_MASK           64
#define HSCMP1PCR_INPPE4_MASK           128
#define HSCMP1PCR_INMPE_1_MASK          15
#define HSCMP1PCR_INMPE_1_BITNUM        0
#define HSCMP1PCR_INPPE_1_MASK          240
#define HSCMP1PCR_INPPE_1_BITNUM        4


/*** HSCMP2CR0 - HSCMP2 Control Register 0; 0xFFFF81B8 ***/
typedef union {
  byte Byte;
  struct {
    byte MMC0        :1;                                       /* Minus Input Mux Control, bit 0 */
    byte MMC1        :1;                                       /* Minus Input Mux Control, bit 1 */
    byte PMC0        :1;                                       /* Positive Input Mux Control, bit 0 */
    byte PMC1        :1;                                       /* Positive Input Mux Control, bit 1 */
    byte FILTER_CNT0 :1;                                       /* Filter Sample Count, bit 0 */
    byte FILTER_CNT1 :1;                                       /* Filter Sample Count, bit 1 */
    byte FILTER_CNT2 :1;                                       /* Filter Sample Count, bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpMMC  :2;
    byte grpPMC  :2;
    byte grpFILTER_CNT :3;
    byte         :1;
  } MergedBits;
} HSCMP2CR0STR;
extern volatile HSCMP2CR0STR _HSCMP2CR0 @0xFFFF81B8;
#define HSCMP2CR0                       _HSCMP2CR0.Byte
#define HSCMP2CR0_MMC0                  _HSCMP2CR0.Bits.MMC0
#define HSCMP2CR0_MMC1                  _HSCMP2CR0.Bits.MMC1
#define HSCMP2CR0_PMC0                  _HSCMP2CR0.Bits.PMC0
#define HSCMP2CR0_PMC1                  _HSCMP2CR0.Bits.PMC1
#define HSCMP2CR0_FILTER_CNT0           _HSCMP2CR0.Bits.FILTER_CNT0
#define HSCMP2CR0_FILTER_CNT1           _HSCMP2CR0.Bits.FILTER_CNT1
#define HSCMP2CR0_FILTER_CNT2           _HSCMP2CR0.Bits.FILTER_CNT2
/* HSCMP2CR_ARR: Access 2 HSCMP2CRx registers in an array */
#define HSCMP2CR_ARR                    ((volatile byte *) &HSCMP2CR0)
#define HSCMP2CR0_MMC                   _HSCMP2CR0.MergedBits.grpMMC
#define HSCMP2CR0_PMC                   _HSCMP2CR0.MergedBits.grpPMC
#define HSCMP2CR0_FILTER_CNT            _HSCMP2CR0.MergedBits.grpFILTER_CNT

#define HSCMP2CR0_MMC0_MASK             1
#define HSCMP2CR0_MMC1_MASK             2
#define HSCMP2CR0_PMC0_MASK             4
#define HSCMP2CR0_PMC1_MASK             8
#define HSCMP2CR0_FILTER_CNT0_MASK      16
#define HSCMP2CR0_FILTER_CNT1_MASK      32
#define HSCMP2CR0_FILTER_CNT2_MASK      64
#define HSCMP2CR0_MMC_MASK              3
#define HSCMP2CR0_MMC_BITNUM            0
#define HSCMP2CR0_PMC_MASK              12
#define HSCMP2CR0_PMC_BITNUM            2
#define HSCMP2CR0_FILTER_CNT_MASK       112
#define HSCMP2CR0_FILTER_CNT_BITNUM     4


/*** HSCMP2CR1 - HSCMP2 Control Register 1; 0xFFFF81B9 ***/
typedef union {
  byte Byte;
  struct {
    byte EN          :1;                                       /* Comparator Module Enable */
    byte OPE         :1;                                       /* Comparator Output Pin Enable */
    byte COS         :1;                                       /* Comparator Output Select */
    byte INV         :1;                                       /* Comparator INVERT */
    byte PMODE       :1;                                       /* Power Mode Select */
    byte             :1; 
    byte WE          :1;                                       /* Windowing Enable */
    byte SE          :1;                                       /* Sample Enable */
  } Bits;
} HSCMP2CR1STR;
extern volatile HSCMP2CR1STR _HSCMP2CR1 @0xFFFF81B9;
#define HSCMP2CR1                       _HSCMP2CR1.Byte
#define HSCMP2CR1_EN                    _HSCMP2CR1.Bits.EN
#define HSCMP2CR1_OPE                   _HSCMP2CR1.Bits.OPE
#define HSCMP2CR1_COS                   _HSCMP2CR1.Bits.COS
#define HSCMP2CR1_INV                   _HSCMP2CR1.Bits.INV
#define HSCMP2CR1_PMODE                 _HSCMP2CR1.Bits.PMODE
#define HSCMP2CR1_WE                    _HSCMP2CR1.Bits.WE
#define HSCMP2CR1_SE                    _HSCMP2CR1.Bits.SE

#define HSCMP2CR1_EN_MASK               1
#define HSCMP2CR1_OPE_MASK              2
#define HSCMP2CR1_COS_MASK              4
#define HSCMP2CR1_INV_MASK              8
#define HSCMP2CR1_PMODE_MASK            16
#define HSCMP2CR1_WE_MASK               64
#define HSCMP2CR1_SE_MASK               128


/*** HSCMP2FPR - HSCMP2 Filter Period Register; 0xFFFF81BA ***/
typedef union {
  byte Byte;
  struct {
    byte FILT_PER0   :1;                                       /* Filter Period, bit 0 */
    byte FILT_PER1   :1;                                       /* Filter Period, bit 1 */
    byte FILT_PER2   :1;                                       /* Filter Period, bit 2 */
    byte FILT_PER3   :1;                                       /* Filter Period, bit 3 */
    byte FILT_PER4   :1;                                       /* Filter Period, bit 4 */
    byte FILT_PER5   :1;                                       /* Filter Period, bit 5 */
    byte FILT_PER6   :1;                                       /* Filter Period, bit 6 */
    byte FILT_PER7   :1;                                       /* Filter Period, bit 7 */
  } Bits;
} HSCMP2FPRSTR;
extern volatile HSCMP2FPRSTR _HSCMP2FPR @0xFFFF81BA;
#define HSCMP2FPR                       _HSCMP2FPR.Byte
#define HSCMP2FPR_FILT_PER0             _HSCMP2FPR.Bits.FILT_PER0
#define HSCMP2FPR_FILT_PER1             _HSCMP2FPR.Bits.FILT_PER1
#define HSCMP2FPR_FILT_PER2             _HSCMP2FPR.Bits.FILT_PER2
#define HSCMP2FPR_FILT_PER3             _HSCMP2FPR.Bits.FILT_PER3
#define HSCMP2FPR_FILT_PER4             _HSCMP2FPR.Bits.FILT_PER4
#define HSCMP2FPR_FILT_PER5             _HSCMP2FPR.Bits.FILT_PER5
#define HSCMP2FPR_FILT_PER6             _HSCMP2FPR.Bits.FILT_PER6
#define HSCMP2FPR_FILT_PER7             _HSCMP2FPR.Bits.FILT_PER7

#define HSCMP2FPR_FILT_PER0_MASK        1
#define HSCMP2FPR_FILT_PER1_MASK        2
#define HSCMP2FPR_FILT_PER2_MASK        4
#define HSCMP2FPR_FILT_PER3_MASK        8
#define HSCMP2FPR_FILT_PER4_MASK        16
#define HSCMP2FPR_FILT_PER5_MASK        32
#define HSCMP2FPR_FILT_PER6_MASK        64
#define HSCMP2FPR_FILT_PER7_MASK        128


/*** HSCMP2SCR - HSCMP2 Status & Control Register; 0xFFFF81BB ***/
typedef union {
  byte Byte;
  struct {
    byte COUT        :1;                                       /* Analog Comparator Output */
    byte CFF         :1;                                       /* Analog Comparator Flag Falling */
    byte CFR         :1;                                       /* Analog Comparator Flag Rising */
    byte IEF         :1;                                       /* Comparator Interrupt Enable Falling */
    byte IER         :1;                                       /* Comparator Interrupt Enable Rising */
    byte SMLEB       :1;                                       /* Stop Mode Edge / Level Interrupt Control */
    byte DMAEN       :1;                                       /* DMA enable control */
    byte             :1; 
  } Bits;
} HSCMP2SCRSTR;
extern volatile HSCMP2SCRSTR _HSCMP2SCR @0xFFFF81BB;
#define HSCMP2SCR                       _HSCMP2SCR.Byte
#define HSCMP2SCR_COUT                  _HSCMP2SCR.Bits.COUT
#define HSCMP2SCR_CFF                   _HSCMP2SCR.Bits.CFF
#define HSCMP2SCR_CFR                   _HSCMP2SCR.Bits.CFR
#define HSCMP2SCR_IEF                   _HSCMP2SCR.Bits.IEF
#define HSCMP2SCR_IER                   _HSCMP2SCR.Bits.IER
#define HSCMP2SCR_SMLEB                 _HSCMP2SCR.Bits.SMLEB
#define HSCMP2SCR_DMAEN                 _HSCMP2SCR.Bits.DMAEN

#define HSCMP2SCR_COUT_MASK             1
#define HSCMP2SCR_CFF_MASK              2
#define HSCMP2SCR_CFR_MASK              4
#define HSCMP2SCR_IEF_MASK              8
#define HSCMP2SCR_IER_MASK              16
#define HSCMP2SCR_SMLEB_MASK            32
#define HSCMP2SCR_DMAEN_MASK            64


/*** HSCMP2PCR - Pin Control Register; 0xFFFF81BC ***/
typedef union {
  byte Byte;
  struct {
    byte INMPE1      :1;                                       /* Minus Input Pin 1 Enable */
    byte INMPE2      :1;                                       /* Minus Input Pin 2 Enable */
    byte INMPE3      :1;                                       /* Minus Input Pin 3 Enable */
    byte INMPE4      :1;                                       /* Minus Input Pin 4 Enable */
    byte INPPE1      :1;                                       /* Positive Input Pin 1 Enable */
    byte INPPE2      :1;                                       /* Positive Input Pin 2 Enable */
    byte INPPE3      :1;                                       /* Positive Input Pin 3 Enable */
    byte INPPE4      :1;                                       /* Positive Input Pin 4 Enable */
  } Bits;
  struct {
    byte grpINMPE_1 :4;
    byte grpINPPE_1 :4;
  } MergedBits;
} HSCMP2PCRSTR;
extern volatile HSCMP2PCRSTR _HSCMP2PCR @0xFFFF81BC;
#define HSCMP2PCR                       _HSCMP2PCR.Byte
#define HSCMP2PCR_INMPE1                _HSCMP2PCR.Bits.INMPE1
#define HSCMP2PCR_INMPE2                _HSCMP2PCR.Bits.INMPE2
#define HSCMP2PCR_INMPE3                _HSCMP2PCR.Bits.INMPE3
#define HSCMP2PCR_INMPE4                _HSCMP2PCR.Bits.INMPE4
#define HSCMP2PCR_INPPE1                _HSCMP2PCR.Bits.INPPE1
#define HSCMP2PCR_INPPE2                _HSCMP2PCR.Bits.INPPE2
#define HSCMP2PCR_INPPE3                _HSCMP2PCR.Bits.INPPE3
#define HSCMP2PCR_INPPE4                _HSCMP2PCR.Bits.INPPE4
#define HSCMP2PCR_INMPE_1               _HSCMP2PCR.MergedBits.grpINMPE_1
#define HSCMP2PCR_INPPE_1               _HSCMP2PCR.MergedBits.grpINPPE_1
#define HSCMP2PCR_INMPE                 HSCMP2PCR_INMPE_1
#define HSCMP2PCR_INPPE                 HSCMP2PCR_INPPE_1

#define HSCMP2PCR_INMPE1_MASK           1
#define HSCMP2PCR_INMPE2_MASK           2
#define HSCMP2PCR_INMPE3_MASK           4
#define HSCMP2PCR_INMPE4_MASK           8
#define HSCMP2PCR_INPPE1_MASK           16
#define HSCMP2PCR_INPPE2_MASK           32
#define HSCMP2PCR_INPPE3_MASK           64
#define HSCMP2PCR_INPPE4_MASK           128
#define HSCMP2PCR_INMPE_1_MASK          15
#define HSCMP2PCR_INMPE_1_BITNUM        0
#define HSCMP2PCR_INPPE_1_MASK          240
#define HSCMP2PCR_INPPE_1_BITNUM        4


/*** SRS - System Reset Status Register; 0xFFFF9800 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte LVD         :1;                                       /* Low Voltage Detect */
    byte WDOG        :1;                                       /* Watchdog timer reset */
    byte ILAD        :1;                                       /* Illegal Address */
    byte ILOP        :1;                                       /* Illegal Opcode */
    byte LOC         :1;                                       /* Lose-the-clock Reset */
    byte PIN         :1;                                       /* External Reset Pin */
    byte POR         :1;                                       /* Power-On Reset */
  } Bits;
} SRSSTR;
extern volatile SRSSTR _SRS @0xFFFF9800;
#define SRS                             _SRS.Byte
#define SRS_LVD                         _SRS.Bits.LVD
#define SRS_WDOG                        _SRS.Bits.WDOG
#define SRS_ILAD                        _SRS.Bits.ILAD
#define SRS_ILOP                        _SRS.Bits.ILOP
#define SRS_LOC                         _SRS.Bits.LOC
#define SRS_PIN                         _SRS.Bits.PIN
#define SRS_POR                         _SRS.Bits.POR

#define SRS_LVD_MASK                    2
#define SRS_WDOG_MASK                   4
#define SRS_ILAD_MASK                   8
#define SRS_ILOP_MASK                   16
#define SRS_LOC_MASK                    32
#define SRS_PIN_MASK                    64
#define SRS_POR_MASK                    128


/*** SOPT1 - System Options Register 1; 0xFFFF9802 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte WAITE       :1;                                       /* WAIT Mode Enable */
    byte STOPE       :1;                                       /* Stop Mode Enable */
    byte             :1; 
    byte             :1; 
  } Bits;
} SOPT1STR;
extern volatile SOPT1STR _SOPT1 @0xFFFF9802;
#define SOPT1                           _SOPT1.Byte
#define SOPT1_WAITE                     _SOPT1.Bits.WAITE
#define SOPT1_STOPE                     _SOPT1.Bits.STOPE

#define SOPT1_WAITE_MASK                16
#define SOPT1_STOPE_MASK                32


/*** SMCLK - System MCLK Control Register; 0xFFFF9803 ***/
typedef union {
  byte Byte;
  struct {
    byte MCSEL0      :1;                                       /* MCLK Divide Select, bit 0 */
    byte MCSEL1      :1;                                       /* MCLK Divide Select, bit 1 */
    byte MCSEL2      :1;                                       /* MCLK Divide Select, bit 2 */
    byte             :1; 
    byte MPE         :1;                                       /* MCLK Pin Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpMCSEL :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SMCLKSTR;
extern volatile SMCLKSTR _SMCLK @0xFFFF9803;
#define SMCLK                           _SMCLK.Byte
#define SMCLK_MCSEL0                    _SMCLK.Bits.MCSEL0
#define SMCLK_MCSEL1                    _SMCLK.Bits.MCSEL1
#define SMCLK_MCSEL2                    _SMCLK.Bits.MCSEL2
#define SMCLK_MPE                       _SMCLK.Bits.MPE
#define SMCLK_MCSEL                     _SMCLK.MergedBits.grpMCSEL

#define SMCLK_MCSEL0_MASK               1
#define SMCLK_MCSEL1_MASK               2
#define SMCLK_MCSEL2_MASK               4
#define SMCLK_MPE_MASK                  16
#define SMCLK_MCSEL_MASK                7
#define SMCLK_MCSEL_BITNUM              0


/*** SDID - System Device Identification Register; 0xFFFF9806 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SDIDH - System Device Identification Register High; 0xFFFF9806 ***/
    union {
      byte Byte;
      struct {
        byte ID8         :1;                                       /* Part Identification Number, bit 8 */
        byte ID9         :1;                                       /* Part Identification Number, bit 9 */
        byte ID10        :1;                                       /* Part Identification Number, bit 10 */
        byte ID11        :1;                                       /* Part Identification Number, bit 11 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpID_8 :4;
        byte     :1;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SDIDHSTR;
    #define SDIDH                       _SDID.Overlap_STR.SDIDHSTR.Byte
    #define SDIDH_ID8                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID8
    #define SDIDH_ID9                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID9
    #define SDIDH_ID10                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID10
    #define SDIDH_ID11                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID11
    #define SDIDH_ID_8                  _SDID.Overlap_STR.SDIDHSTR.MergedBits.grpID_8
    #define SDIDH_ID                    SDIDH_ID_8
    
    #define SDIDH_ID8_MASK              1
    #define SDIDH_ID9_MASK              2
    #define SDIDH_ID10_MASK             4
    #define SDIDH_ID11_MASK             8
    #define SDIDH_ID_8_MASK             15
    #define SDIDH_ID_8_BITNUM           0
    

    /*** SDIDL - System Device Identification Register Low; 0xFFFF9807 ***/
    union {
      byte Byte;
      struct {
        byte ID0         :1;                                       /* Part Identification Number, bit 0 */
        byte ID1         :1;                                       /* Part Identification Number, bit 1 */
        byte ID2         :1;                                       /* Part Identification Number, bit 2 */
        byte ID3         :1;                                       /* Part Identification Number, bit 3 */
        byte ID4         :1;                                       /* Part Identification Number, bit 4 */
        byte ID5         :1;                                       /* Part Identification Number, bit 5 */
        byte ID6         :1;                                       /* Part Identification Number, bit 6 */
        byte ID7         :1;                                       /* Part Identification Number, bit 7 */
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

  struct {
    word ID0         :1;                                       /* Part Identification Number, bit 0 */
    word ID1         :1;                                       /* Part Identification Number, bit 1 */
    word ID2         :1;                                       /* Part Identification Number, bit 2 */
    word ID3         :1;                                       /* Part Identification Number, bit 3 */
    word ID4         :1;                                       /* Part Identification Number, bit 4 */
    word ID5         :1;                                       /* Part Identification Number, bit 5 */
    word ID6         :1;                                       /* Part Identification Number, bit 6 */
    word ID7         :1;                                       /* Part Identification Number, bit 7 */
    word ID8         :1;                                       /* Part Identification Number, bit 8 */
    word ID9         :1;                                       /* Part Identification Number, bit 9 */
    word ID10        :1;                                       /* Part Identification Number, bit 10 */
    word ID11        :1;                                       /* Part Identification Number, bit 11 */
    word             :1; 
    word             :1; 
    word             :1; 
    word             :1; 
  } Bits;
  struct {
    word grpID   :12;
    word         :1;
    word         :1;
    word         :1;
    word         :1;
  } MergedBits;
} SDIDSTR;
extern volatile SDIDSTR _SDID @0xFFFF9806;
#define SDID                            _SDID.Word
#define SDID_ID0                        _SDID.Bits.ID0
#define SDID_ID1                        _SDID.Bits.ID1
#define SDID_ID2                        _SDID.Bits.ID2
#define SDID_ID3                        _SDID.Bits.ID3
#define SDID_ID4                        _SDID.Bits.ID4
#define SDID_ID5                        _SDID.Bits.ID5
#define SDID_ID6                        _SDID.Bits.ID6
#define SDID_ID7                        _SDID.Bits.ID7
#define SDID_ID8                        _SDID.Bits.ID8
#define SDID_ID9                        _SDID.Bits.ID9
#define SDID_ID10                       _SDID.Bits.ID10
#define SDID_ID11                       _SDID.Bits.ID11
#define SDID_ID                         _SDID.MergedBits.grpID

#define SDID_ID0_MASK                   1
#define SDID_ID1_MASK                   2
#define SDID_ID2_MASK                   4
#define SDID_ID3_MASK                   8
#define SDID_ID4_MASK                   16
#define SDID_ID5_MASK                   32
#define SDID_ID6_MASK                   64
#define SDID_ID7_MASK                   128
#define SDID_ID8_MASK                   256
#define SDID_ID9_MASK                   512
#define SDID_ID10_MASK                  1024
#define SDID_ID11_MASK                  2048
#define SDID_ID_MASK                    4095
#define SDID_ID_BITNUM                  0


/*** SPMSC1 - System Power Management Status and Control 1 Register; 0xFFFF9809 ***/
typedef union {
  byte Byte;
  struct {
    byte BGBE        :1;                                       /* Bandgap Buffer Enable */
    byte             :1; 
    byte LVDE        :1;                                       /* Low-Voltage Detect Enable */
    byte LVDSE       :1;                                       /* Low-Voltage Detect Stop Enable */
    byte LVDRE       :1;                                       /* Low-Voltage Detect Reset Enable */
    byte LVWIE       :1;                                       /* Low-Voltage Warning Interrupt Enable */
    byte LVWACK      :1;                                       /* Low-Voltage Warning Acknowledge */
    byte LVWF        :1;                                       /* Low-Voltage Warning status */
  } Bits;
} SPMSC1STR;
extern volatile SPMSC1STR _SPMSC1 @0xFFFF9809;
#define SPMSC1                          _SPMSC1.Byte
#define SPMSC1_BGBE                     _SPMSC1.Bits.BGBE
#define SPMSC1_LVDE                     _SPMSC1.Bits.LVDE
#define SPMSC1_LVDSE                    _SPMSC1.Bits.LVDSE
#define SPMSC1_LVDRE                    _SPMSC1.Bits.LVDRE
#define SPMSC1_LVWIE                    _SPMSC1.Bits.LVWIE
#define SPMSC1_LVWACK                   _SPMSC1.Bits.LVWACK
#define SPMSC1_LVWF                     _SPMSC1.Bits.LVWF

#define SPMSC1_BGBE_MASK                1
#define SPMSC1_LVDE_MASK                4
#define SPMSC1_LVDSE_MASK               8
#define SPMSC1_LVDRE_MASK               16
#define SPMSC1_LVWIE_MASK               32
#define SPMSC1_LVWACK_MASK              64
#define SPMSC1_LVWF_MASK                128


/*** SPMSC2 - System Power Management Status and Control 2 Register; 0xFFFF980A ***/
typedef union {
  byte Byte;
  struct {
    byte PPDC        :1;                                       /* Partial Power Down Control */
    byte             :1; 
    byte PPDACK      :1;                                       /* Partial Power Down Acknowledge */
    byte PPDF        :1;                                       /* Partial Power Down Flag */
    byte LVWV        :1;                                       /* Low-Voltage Warning Voltage Select */
    byte LVDV        :1;                                       /* Low-Voltage Detect Voltage Select */
    byte             :1; 
    byte             :1; 
  } Bits;
} SPMSC2STR;
extern volatile SPMSC2STR _SPMSC2 @0xFFFF980A;
#define SPMSC2                          _SPMSC2.Byte
#define SPMSC2_PPDC                     _SPMSC2.Bits.PPDC
#define SPMSC2_PPDACK                   _SPMSC2.Bits.PPDACK
#define SPMSC2_PPDF                     _SPMSC2.Bits.PPDF
#define SPMSC2_LVWV                     _SPMSC2.Bits.LVWV
#define SPMSC2_LVDV                     _SPMSC2.Bits.LVDV

#define SPMSC2_PPDC_MASK                1
#define SPMSC2_PPDACK_MASK              4
#define SPMSC2_PPDF_MASK                8
#define SPMSC2_LVWV_MASK                16
#define SPMSC2_LVDV_MASK                32


/*** SOPT2 - System Options Register 2; 0xFFFF980C ***/
typedef union {
  byte Byte;
  struct {
    byte ADHWTS0     :1;                                       /* ADC Hardware Trigger Source, bit 0 */
    byte ADHWTS1     :1;                                       /* ADC Hardware Trigger Source, bit 1 */
    byte ACIC1       :1;                                       /* Analog Comparator 1 to Input Capture Enable */
    byte TPMCCFG     :1;                                       /* TPM Clock Configuration */
    byte ACIC2       :1;                                       /* Analog Comparator 2 to Input Capture Enable */
    byte FTM1SYNC_bit :1;                                      /* FTM1 synchronization trigger */ /*Warning: bit name is duplicated with register name*/
    byte FTM2SYNC_bit :1;                                      /* FTM2 synchronization trigger */ /*Warning: bit name is duplicated with register name*/
    byte CME         :1;                                       /* ICS External reference clock monitor */
  } Bits;
  struct {
    byte grpADHWTS :2;
    byte grpACIC_1 :1;
    byte         :1;
    byte grpACIC_2 :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SOPT2STR;
extern volatile SOPT2STR _SOPT2 @0xFFFF980C;
#define SOPT2                           _SOPT2.Byte
#define SOPT2_ADHWTS0                   _SOPT2.Bits.ADHWTS0
#define SOPT2_ADHWTS1                   _SOPT2.Bits.ADHWTS1
#define SOPT2_ACIC1                     _SOPT2.Bits.ACIC1
#define SOPT2_TPMCCFG                   _SOPT2.Bits.TPMCCFG
#define SOPT2_ACIC2                     _SOPT2.Bits.ACIC2
#define SOPT2_FTM1SYNC                  _SOPT2.Bits.FTM1SYNC_bit
#define SOPT2_FTM2SYNC                  _SOPT2.Bits.FTM2SYNC_bit
#define SOPT2_CME                       _SOPT2.Bits.CME
#define SOPT2_ADHWTS                    _SOPT2.MergedBits.grpADHWTS

#define SOPT2_ADHWTS0_MASK              1
#define SOPT2_ADHWTS1_MASK              2
#define SOPT2_ACIC1_MASK                4
#define SOPT2_TPMCCFG_MASK              8
#define SOPT2_ACIC2_MASK                16
#define SOPT2_FTM1SYNC_MASK             32
#define SOPT2_FTM2SYNC_MASK             64
#define SOPT2_CME_MASK                  128
#define SOPT2_ADHWTS_MASK               3
#define SOPT2_ADHWTS_BITNUM             0


/*** SCGC1 - System Clock Gating Control 1 Register; 0xFFFF980D ***/
typedef union {
  byte Byte;
  struct {
    byte SCI1        :1;                                       /* SCI1 Clock Gate Control */
    byte SCI2        :1;                                       /* SCI2 Clock Gate Control */
    byte IIC         :1;                                       /* IIC Clock Gate Control */
    byte DMA         :1;                                       /* DMA Clock Gate Control */
    byte ADC         :1;                                       /* ADC Clock Gate Control */
    byte FTM1        :1;                                       /* FTM1 Clock Gate Control */
    byte FTM2        :1;                                       /* FTM2 Clock Gate Control */
    byte TPM3        :1;                                       /* TPM3 Clock Gate Control */
  } Bits;
  struct {
    byte grpSCI_1 :2;
    byte         :1;
    byte grpDMx  :1;
    byte         :1;
    byte grpFTM_1 :2;
    byte grpTPM_3 :1;
  } MergedBits;
} SCGC1STR;
extern volatile SCGC1STR _SCGC1 @0xFFFF980D;
#define SCGC1                           _SCGC1.Byte
#define SCGC1_SCI1                      _SCGC1.Bits.SCI1
#define SCGC1_SCI2                      _SCGC1.Bits.SCI2
#define SCGC1_IIC                       _SCGC1.Bits.IIC
#define SCGC1_DMA                       _SCGC1.Bits.DMA
#define SCGC1_ADC                       _SCGC1.Bits.ADC
#define SCGC1_FTM1                      _SCGC1.Bits.FTM1
#define SCGC1_FTM2                      _SCGC1.Bits.FTM2
#define SCGC1_TPM3                      _SCGC1.Bits.TPM3
#define SCGC1_SCI_1                     _SCGC1.MergedBits.grpSCI_1
#define SCGC1_FTM_1                     _SCGC1.MergedBits.grpFTM_1
#define SCGC1_SCI                       SCGC1_SCI_1
#define SCGC1_FTM                       SCGC1_FTM_1

#define SCGC1_SCI1_MASK                 1
#define SCGC1_SCI2_MASK                 2
#define SCGC1_IIC_MASK                  4
#define SCGC1_DMA_MASK                  8
#define SCGC1_ADC_MASK                  16
#define SCGC1_FTM1_MASK                 32
#define SCGC1_FTM2_MASK                 64
#define SCGC1_TPM3_MASK                 128
#define SCGC1_SCI_1_MASK                3
#define SCGC1_SCI_1_BITNUM              0
#define SCGC1_FTM_1_MASK                96
#define SCGC1_FTM_1_BITNUM              5


/*** SCGC2 - System Clock Gating Control 2 Register; 0xFFFF980E ***/
typedef union {
  byte Byte;
  struct {
    byte SPI1        :1;                                       /* SPI1 Clock Gate Control */
    byte SPI2        :1;                                       /* SPI2 Clock Gate Control */
    byte RTC         :1;                                       /* RTC Clock Gate Control */
    byte iEVT        :1;                                       /* iEVENT Clock Gate Control */
    byte PDB         :1;                                       /* PDB Clock Gate Control */
    byte IRQ         :1;                                       /* IRQ Clock Gate Control */
    byte FLS         :1;                                       /* FTSR Clock Gate Control */
    byte CRC         :1;                                       /* CRC Clock Gate Control */
  } Bits;
  struct {
    byte grpSPI_1 :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SCGC2STR;
extern volatile SCGC2STR _SCGC2 @0xFFFF980E;
#define SCGC2                           _SCGC2.Byte
#define SCGC2_SPI1                      _SCGC2.Bits.SPI1
#define SCGC2_SPI2                      _SCGC2.Bits.SPI2
#define SCGC2_RTC                       _SCGC2.Bits.RTC
#define SCGC2_iEVT                      _SCGC2.Bits.iEVT
#define SCGC2_PDB                       _SCGC2.Bits.PDB
#define SCGC2_IRQ                       _SCGC2.Bits.IRQ
#define SCGC2_FLS                       _SCGC2.Bits.FLS
#define SCGC2_CRC                       _SCGC2.Bits.CRC
#define SCGC2_SPI_1                     _SCGC2.MergedBits.grpSPI_1
#define SCGC2_SPI                       SCGC2_SPI_1

#define SCGC2_SPI1_MASK                 1
#define SCGC2_SPI2_MASK                 2
#define SCGC2_RTC_MASK                  4
#define SCGC2_iEVT_MASK                 8
#define SCGC2_PDB_MASK                  16
#define SCGC2_IRQ_MASK                  32
#define SCGC2_FLS_MASK                  64
#define SCGC2_CRC_MASK                  128
#define SCGC2_SPI_1_MASK                3
#define SCGC2_SPI_1_BITNUM              0


/*** SCGC3 - System Clock Gating Control 3 Register; 0xFFFF980F ***/
typedef union {
  byte Byte;
  struct {
    byte EWM         :1;                                       /* EWM Clock Gate Control */
    byte DAC1        :1;                                       /* DAC1 Clock Gate Control */
    byte DAC2        :1;                                       /* DAC2 Clock Gate Control */
    byte HSCMP1      :1;                                       /* HSCMP1 Clock Gate Control */
    byte HSCMP2      :1;                                       /* HSCMP2 Clock Gate Control */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte         :1;
    byte grpDAC_1 :2;
    byte grpHSCMP_1 :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SCGC3STR;
extern volatile SCGC3STR _SCGC3 @0xFFFF980F;
#define SCGC3                           _SCGC3.Byte
#define SCGC3_EWM                       _SCGC3.Bits.EWM
#define SCGC3_DAC1                      _SCGC3.Bits.DAC1
#define SCGC3_DAC2                      _SCGC3.Bits.DAC2
#define SCGC3_HSCMP1                    _SCGC3.Bits.HSCMP1
#define SCGC3_HSCMP2                    _SCGC3.Bits.HSCMP2
#define SCGC3_DAC_1                     _SCGC3.MergedBits.grpDAC_1
#define SCGC3_HSCMP_1                   _SCGC3.MergedBits.grpHSCMP_1
#define SCGC3_DAC                       SCGC3_DAC_1
#define SCGC3_HSCMP                     SCGC3_HSCMP_1

#define SCGC3_EWM_MASK                  1
#define SCGC3_DAC1_MASK                 2
#define SCGC3_DAC2_MASK                 4
#define SCGC3_HSCMP1_MASK               8
#define SCGC3_HSCMP2_MASK               16
#define SCGC3_DAC_1_MASK                6
#define SCGC3_DAC_1_BITNUM              1
#define SCGC3_HSCMP_1_MASK              24
#define SCGC3_HSCMP_1_BITNUM            3


/*** FCDIV - FLASH Clock Divider Register; 0xFFFF9820 ***/
typedef union {
  byte Byte;
  struct {
    byte FDIV0       :1;                                       /* Divisor for FLASH Clock Divider, bit 0 */
    byte FDIV1       :1;                                       /* Divisor for FLASH Clock Divider, bit 1 */
    byte FDIV2       :1;                                       /* Divisor for FLASH Clock Divider, bit 2 */
    byte FDIV3       :1;                                       /* Divisor for FLASH Clock Divider, bit 3 */
    byte FDIV4       :1;                                       /* Divisor for FLASH Clock Divider, bit 4 */
    byte FDIV5       :1;                                       /* Divisor for FLASH Clock Divider, bit 5 */
    byte PRDIV8      :1;                                       /* Prescale (Divide) FLASH Clock by 8 */
    byte FDIVLD      :1;                                       /* Divisor Loaded Status Flag */
  } Bits;
  struct {
    byte grpFDIV :6;
    byte grpPRDIV_8 :1;
    byte         :1;
  } MergedBits;
} FCDIVSTR;
extern volatile FCDIVSTR _FCDIV @0xFFFF9820;
#define FCDIV                           _FCDIV.Byte
#define FCDIV_FDIV0                     _FCDIV.Bits.FDIV0
#define FCDIV_FDIV1                     _FCDIV.Bits.FDIV1
#define FCDIV_FDIV2                     _FCDIV.Bits.FDIV2
#define FCDIV_FDIV3                     _FCDIV.Bits.FDIV3
#define FCDIV_FDIV4                     _FCDIV.Bits.FDIV4
#define FCDIV_FDIV5                     _FCDIV.Bits.FDIV5
#define FCDIV_PRDIV8                    _FCDIV.Bits.PRDIV8
#define FCDIV_FDIVLD                    _FCDIV.Bits.FDIVLD
#define FCDIV_FDIV                      _FCDIV.MergedBits.grpFDIV

#define FCDIV_FDIV0_MASK                1
#define FCDIV_FDIV1_MASK                2
#define FCDIV_FDIV2_MASK                4
#define FCDIV_FDIV3_MASK                8
#define FCDIV_FDIV4_MASK                16
#define FCDIV_FDIV5_MASK                32
#define FCDIV_PRDIV8_MASK               64
#define FCDIV_FDIVLD_MASK               128
#define FCDIV_FDIV_MASK                 63
#define FCDIV_FDIV_BITNUM               0


/*** FOPT - Flash Options Register; 0xFFFF9821 ***/
typedef union {
  byte Byte;
  struct {
    byte SEC0        :1;                                       /* Flash Security Bit 0 */
    byte SEC1        :1;                                       /* Flash Security Bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte KEYEN0      :1;                                       /* Backdoor Key Security Enable Bit 0 */
    byte KEYEN1      :1;                                       /* Backdoor Key Security Enable Bit 1 */
  } Bits;
  struct {
    byte grpSEC  :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpKEYEN :2;
  } MergedBits;
} FOPTSTR;
extern volatile FOPTSTR _FOPT @0xFFFF9821;
#define FOPT                            _FOPT.Byte
#define FOPT_SEC0                       _FOPT.Bits.SEC0
#define FOPT_SEC1                       _FOPT.Bits.SEC1
#define FOPT_KEYEN0                     _FOPT.Bits.KEYEN0
#define FOPT_KEYEN1                     _FOPT.Bits.KEYEN1
#define FOPT_SEC                        _FOPT.MergedBits.grpSEC
#define FOPT_KEYEN                      _FOPT.MergedBits.grpKEYEN

#define FOPT_SEC0_MASK                  1
#define FOPT_SEC1_MASK                  2
#define FOPT_KEYEN0_MASK                64
#define FOPT_KEYEN1_MASK                128
#define FOPT_SEC_MASK                   3
#define FOPT_SEC_BITNUM                 0
#define FOPT_KEYEN_MASK                 192
#define FOPT_KEYEN_BITNUM               6


/*** FCNFG - Flash Configuration Register; 0xFFFF9823 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte KEYACC      :1;                                       /* Enable Security Key Writing */
    byte             :1; 
    byte             :1; 
  } Bits;
} FCNFGSTR;
extern volatile FCNFGSTR _FCNFG @0xFFFF9823;
#define FCNFG                           _FCNFG.Byte
#define FCNFG_KEYACC                    _FCNFG.Bits.KEYACC

#define FCNFG_KEYACC_MASK               32


/*** FPROT - Flash Protection Register; 0xFFFF9824 ***/
typedef union {
  byte Byte;
  struct {
    byte FPOPEN      :1;                                       /* Flash Protection Open */
    byte FPS0        :1;                                       /* Flash Protection Size, bit 0 */
    byte FPS1        :1;                                       /* Flash Protection Size, bit 1 */
    byte FPS2        :1;                                       /* Flash Protection Size, bit 2 */
    byte FPS3        :1;                                       /* Flash Protection Size, bit 3 */
    byte FPS4        :1;                                       /* Flash Protection Size, bit 4 */
    byte FPS5        :1;                                       /* Flash Protection Size, bit 5 */
    byte FPS6        :1;                                       /* Flash Protection Size, bit 6 */
  } Bits;
  struct {
    byte         :1;
    byte grpFPS  :7;
  } MergedBits;
} FPROTSTR;
extern volatile FPROTSTR _FPROT @0xFFFF9824;
#define FPROT                           _FPROT.Byte
#define FPROT_FPOPEN                    _FPROT.Bits.FPOPEN
#define FPROT_FPS0                      _FPROT.Bits.FPS0
#define FPROT_FPS1                      _FPROT.Bits.FPS1
#define FPROT_FPS2                      _FPROT.Bits.FPS2
#define FPROT_FPS3                      _FPROT.Bits.FPS3
#define FPROT_FPS4                      _FPROT.Bits.FPS4
#define FPROT_FPS5                      _FPROT.Bits.FPS5
#define FPROT_FPS6                      _FPROT.Bits.FPS6
#define FPROT_FPS                       _FPROT.MergedBits.grpFPS

#define FPROT_FPOPEN_MASK               1
#define FPROT_FPS0_MASK                 2
#define FPROT_FPS1_MASK                 4
#define FPROT_FPS2_MASK                 8
#define FPROT_FPS3_MASK                 16
#define FPROT_FPS4_MASK                 32
#define FPROT_FPS5_MASK                 64
#define FPROT_FPS6_MASK                 128
#define FPROT_FPS_MASK                  254
#define FPROT_FPS_BITNUM                1


/*** FSTAT - Flash Status Register; 0xFFFF9825 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte FBLANK      :1;                                       /* FLASH Flag Indicating the Erase Verify Operation Status */
    byte             :1; 
    byte FACCERR     :1;                                       /* FLASH Access Error Flag */
    byte FPVIOL      :1;                                       /* FLASH Protection Violation Flag */
    byte FCCF        :1;                                       /* FLASH Command Complete Interrupt Flag */
    byte FCBEF       :1;                                       /* FLASH Command Buffer Empty Flag */
  } Bits;
} FSTATSTR;
extern volatile FSTATSTR _FSTAT @0xFFFF9825;
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


/*** FCMD - Flash Command Register; 0xFFFF9826 ***/
typedef union {
  byte Byte;
  struct {
    byte FCMD0       :1;                                       /* Flash Command, bit 0 */
    byte FCMD1       :1;                                       /* Flash Command, bit 1 */
    byte FCMD2       :1;                                       /* Flash Command, bit 2 */
    byte FCMD3       :1;                                       /* Flash Command, bit 3 */
    byte FCMD4       :1;                                       /* Flash Command, bit 4 */
    byte FCMD5       :1;                                       /* Flash Command, bit 5 */
    byte FCMD6       :1;                                       /* Flash Command, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpFCMD :7;
    byte         :1;
  } MergedBits;
} FCMDSTR;
extern volatile FCMDSTR _FCMD @0xFFFF9826;
#define FCMD                            _FCMD.Byte
#define FCMD_FCMD0                      _FCMD.Bits.FCMD0
#define FCMD_FCMD1                      _FCMD.Bits.FCMD1
#define FCMD_FCMD2                      _FCMD.Bits.FCMD2
#define FCMD_FCMD3                      _FCMD.Bits.FCMD3
#define FCMD_FCMD4                      _FCMD.Bits.FCMD4
#define FCMD_FCMD5                      _FCMD.Bits.FCMD5
#define FCMD_FCMD6                      _FCMD.Bits.FCMD6
#define FCMD_FCMD                       _FCMD.MergedBits.grpFCMD

#define FCMD_FCMD0_MASK                 1
#define FCMD_FCMD1_MASK                 2
#define FCMD_FCMD2_MASK                 4
#define FCMD_FCMD3_MASK                 8
#define FCMD_FCMD4_MASK                 16
#define FCMD_FCMD5_MASK                 32
#define FCMD_FCMD6_MASK                 64
#define FCMD_FCMD_MASK                  127
#define FCMD_FCMD_BITNUM                0


/*** SAPE1 - System ADC Pin Enable 1 Register; 0xFFFF9831 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC0       :1;                                       /* ADC Pin Control 0 */
    byte ADPC1       :1;                                       /* ADC Pin Control 1 */
    byte ADPC2       :1;                                       /* ADC Pin Control 2 */
    byte ADPC3       :1;                                       /* ADC Pin Control 3 */
    byte ADPC4       :1;                                       /* ADC Pin Control 4 */
    byte ADPC5       :1;                                       /* ADC Pin Control 5 */
    byte ADPC6       :1;                                       /* ADC Pin Control 6 */
    byte ADPC7       :1;                                       /* ADC Pin Control 7 */
  } Bits;
} SAPE1STR;
extern volatile SAPE1STR _SAPE1 @0xFFFF9831;
#define SAPE1                           _SAPE1.Byte
#define SAPE1_ADPC0                     _SAPE1.Bits.ADPC0
#define SAPE1_ADPC1                     _SAPE1.Bits.ADPC1
#define SAPE1_ADPC2                     _SAPE1.Bits.ADPC2
#define SAPE1_ADPC3                     _SAPE1.Bits.ADPC3
#define SAPE1_ADPC4                     _SAPE1.Bits.ADPC4
#define SAPE1_ADPC5                     _SAPE1.Bits.ADPC5
#define SAPE1_ADPC6                     _SAPE1.Bits.ADPC6
#define SAPE1_ADPC7                     _SAPE1.Bits.ADPC7

#define SAPE1_ADPC0_MASK                1
#define SAPE1_ADPC1_MASK                2
#define SAPE1_ADPC2_MASK                4
#define SAPE1_ADPC3_MASK                8
#define SAPE1_ADPC4_MASK                16
#define SAPE1_ADPC5_MASK                32
#define SAPE1_ADPC6_MASK                64
#define SAPE1_ADPC7_MASK                128


/*** SAPE2 - System ADC Pin Enable 2 Register; 0xFFFF9832 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC8       :1;                                       /* ADC Pin Control 8 */
    byte ADPC9       :1;                                       /* ADC Pin Control 9 */
    byte ADPC10      :1;                                       /* ADC Pin Control 10 */
    byte ADPC11      :1;                                       /* ADC Pin Control 11 */
    byte ADPC12      :1;                                       /* ADC Pin Control 12 */
    byte ADPC13      :1;                                       /* ADC Pin Control 13 */
    byte ADPC14      :1;                                       /* ADC Pin Control 14 */
    byte ADPC15      :1;                                       /* ADC Pin Control 15 */
  } Bits;
} SAPE2STR;
extern volatile SAPE2STR _SAPE2 @0xFFFF9832;
#define SAPE2                           _SAPE2.Byte
#define SAPE2_ADPC8                     _SAPE2.Bits.ADPC8
#define SAPE2_ADPC9                     _SAPE2.Bits.ADPC9
#define SAPE2_ADPC10                    _SAPE2.Bits.ADPC10
#define SAPE2_ADPC11                    _SAPE2.Bits.ADPC11
#define SAPE2_ADPC12                    _SAPE2.Bits.ADPC12
#define SAPE2_ADPC13                    _SAPE2.Bits.ADPC13
#define SAPE2_ADPC14                    _SAPE2.Bits.ADPC14
#define SAPE2_ADPC15                    _SAPE2.Bits.ADPC15

#define SAPE2_ADPC8_MASK                1
#define SAPE2_ADPC9_MASK                2
#define SAPE2_ADPC10_MASK               4
#define SAPE2_ADPC11_MASK               8
#define SAPE2_ADPC12_MASK               16
#define SAPE2_ADPC13_MASK               32
#define SAPE2_ADPC14_MASK               64
#define SAPE2_ADPC15_MASK               128


/*** SAPE3 - System ADC Pin Enable 3 Register; 0xFFFF9833 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC16      :1;                                       /* ADC Pin Control 16 */
    byte ADPC17      :1;                                       /* ADC Pin Control 17 */
    byte ADPC18      :1;                                       /* ADC Pin Control 18 */
    byte ADPC19      :1;                                       /* ADC Pin Control 19 */
    byte ADPC20      :1;                                       /* ADC Pin Control 20 */
    byte ADPC21      :1;                                       /* ADC Pin Control 21 */
    byte ADPC22      :1;                                       /* ADC Pin Control 22 */
    byte ADPC23      :1;                                       /* ADC Pin Control 23 */
  } Bits;
} SAPE3STR;
extern volatile SAPE3STR _SAPE3 @0xFFFF9833;
#define SAPE3                           _SAPE3.Byte
#define SAPE3_ADPC16                    _SAPE3.Bits.ADPC16
#define SAPE3_ADPC17                    _SAPE3.Bits.ADPC17
#define SAPE3_ADPC18                    _SAPE3.Bits.ADPC18
#define SAPE3_ADPC19                    _SAPE3.Bits.ADPC19
#define SAPE3_ADPC20                    _SAPE3.Bits.ADPC20
#define SAPE3_ADPC21                    _SAPE3.Bits.ADPC21
#define SAPE3_ADPC22                    _SAPE3.Bits.ADPC22
#define SAPE3_ADPC23                    _SAPE3.Bits.ADPC23

#define SAPE3_ADPC16_MASK               1
#define SAPE3_ADPC17_MASK               2
#define SAPE3_ADPC18_MASK               4
#define SAPE3_ADPC19_MASK               8
#define SAPE3_ADPC20_MASK               16
#define SAPE3_ADPC21_MASK               32
#define SAPE3_ADPC22_MASK               64
#define SAPE3_ADPC23_MASK               128


/*** SEIS1 - System eGPIO Interrupt Status 1 Register; 0xFFFF9834 ***/
typedef union {
  byte Byte;
  struct {
    byte PTA         :1;                                       /* PORT A Interrupt Status */
    byte PTB         :1;                                       /* PORT B Interrupt Status */
    byte PTC         :1;                                       /* PORT C Interrupt Status */
    byte PTD         :1;                                       /* PORT D Interrupt Status */
    byte PTE         :1;                                       /* PORT E Interrupt Status */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTx  :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SEIS1STR;
extern volatile SEIS1STR _SEIS1 @0xFFFF9834;
#define SEIS1                           _SEIS1.Byte
#define SEIS1_PTA                       _SEIS1.Bits.PTA
#define SEIS1_PTB                       _SEIS1.Bits.PTB
#define SEIS1_PTC                       _SEIS1.Bits.PTC
#define SEIS1_PTD                       _SEIS1.Bits.PTD
#define SEIS1_PTE                       _SEIS1.Bits.PTE
#define SEIS1_PTx                       _SEIS1.MergedBits.grpPTx

#define SEIS1_PTA_MASK                  1
#define SEIS1_PTB_MASK                  2
#define SEIS1_PTC_MASK                  4
#define SEIS1_PTD_MASK                  8
#define SEIS1_PTE_MASK                  16
#define SEIS1_PTx_MASK                  31
#define SEIS1_PTx_BITNUM                0


/*** SEIS2 - System eGPIO Interrupt Status 2 Register; 0xFFFF9835 ***/
typedef union {
  byte Byte;
  struct {
    byte PTF         :1;                                       /* PORTF Interrupt Status */
    byte PTG         :1;                                       /* PORTG Interrupt Status */
    byte PTH         :1;                                       /* PORTH Interrupt Status */
    byte PTJ         :1;                                       /* PORTJ Interrupt Status */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SEIS2STR;
extern volatile SEIS2STR _SEIS2 @0xFFFF9835;
#define SEIS2                           _SEIS2.Byte
#define SEIS2_PTF                       _SEIS2.Bits.PTF
#define SEIS2_PTG                       _SEIS2.Bits.PTG
#define SEIS2_PTH                       _SEIS2.Bits.PTH
#define SEIS2_PTJ                       _SEIS2.Bits.PTJ

#define SEIS2_PTF_MASK                  1
#define SEIS2_PTG_MASK                  2
#define SEIS2_PTH_MASK                  4
#define SEIS2_PTJ_MASK                  8


/*** SPINPS - System Pin Positioning Register; 0xFFFF9836 ***/
typedef union {
  byte Byte;
  struct {
    byte TPM3CH0     :1;                                       /* TPM3 Channel 0 Pin Control */
    byte TPM3CH1     :1;                                       /* TPM3 Channel 1 Pin Control */
    byte FTM2CH2     :1;                                       /* FTM2 Channel 2 Pin Control */
    byte FTM2CH3     :1;                                       /* FTM2 Channel 3 Pin Control */
    byte FTM2CH4     :1;                                       /* FTM2 Channel 4 Pin Control */
    byte FTM2CH5     :1;                                       /* FTM2 Channel 5 Pin Control */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpTPM3CH :2;
    byte grpFTM2CH_2 :4;
    byte         :1;
    byte         :1;
  } MergedBits;
} SPINPSSTR;
extern volatile SPINPSSTR _SPINPS @0xFFFF9836;
#define SPINPS                          _SPINPS.Byte
#define SPINPS_TPM3CH0                  _SPINPS.Bits.TPM3CH0
#define SPINPS_TPM3CH1                  _SPINPS.Bits.TPM3CH1
#define SPINPS_FTM2CH2                  _SPINPS.Bits.FTM2CH2
#define SPINPS_FTM2CH3                  _SPINPS.Bits.FTM2CH3
#define SPINPS_FTM2CH4                  _SPINPS.Bits.FTM2CH4
#define SPINPS_FTM2CH5                  _SPINPS.Bits.FTM2CH5
#define SPINPS_TPM3CH                   _SPINPS.MergedBits.grpTPM3CH
#define SPINPS_FTM2CH_2                 _SPINPS.MergedBits.grpFTM2CH_2
#define SPINPS_FTM2CH                   SPINPS_FTM2CH_2

#define SPINPS_TPM3CH0_MASK             1
#define SPINPS_TPM3CH1_MASK             2
#define SPINPS_FTM2CH2_MASK             4
#define SPINPS_FTM2CH3_MASK             8
#define SPINPS_FTM2CH4_MASK             16
#define SPINPS_FTM2CH5_MASK             32
#define SPINPS_TPM3CH_MASK              3
#define SPINPS_TPM3CH_BITNUM            0
#define SPINPS_FTM2CH_2_MASK            60
#define SPINPS_FTM2CH_2_BITNUM          2


/*** SIMPTA - System Port A Input Buffer Enable Register; 0xFFFF9837 ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTA0     :1;                                       /* PTA0 Input Buffer Control */
    byte SIMPTA1     :1;                                       /* PTA1 Input Buffer Control */
    byte SIMPTA2     :1;                                       /* PTA2 Input Buffer Control */
    byte SIMPTA3     :1;                                       /* PTA3 Input Buffer Control */
    byte SIMPTA4     :1;                                       /* PTA4 Input Buffer Control */
    byte SIMPTA5     :1;                                       /* PTA5 Input Buffer Control */
    byte SIMPTA6     :1;                                       /* PTA6 Input Buffer Control */
    byte SIMPTA7     :1;                                       /* PTA7 Input Buffer Control */
  } Bits;
} SIMPTASTR;
extern volatile SIMPTASTR _SIMPTA @0xFFFF9837;
#define SIMPTA                          _SIMPTA.Byte
#define SIMPTA_SIMPTA0                  _SIMPTA.Bits.SIMPTA0
#define SIMPTA_SIMPTA1                  _SIMPTA.Bits.SIMPTA1
#define SIMPTA_SIMPTA2                  _SIMPTA.Bits.SIMPTA2
#define SIMPTA_SIMPTA3                  _SIMPTA.Bits.SIMPTA3
#define SIMPTA_SIMPTA4                  _SIMPTA.Bits.SIMPTA4
#define SIMPTA_SIMPTA5                  _SIMPTA.Bits.SIMPTA5
#define SIMPTA_SIMPTA6                  _SIMPTA.Bits.SIMPTA6
#define SIMPTA_SIMPTA7                  _SIMPTA.Bits.SIMPTA7

#define SIMPTA_SIMPTA0_MASK             1
#define SIMPTA_SIMPTA1_MASK             2
#define SIMPTA_SIMPTA2_MASK             4
#define SIMPTA_SIMPTA3_MASK             8
#define SIMPTA_SIMPTA4_MASK             16
#define SIMPTA_SIMPTA5_MASK             32
#define SIMPTA_SIMPTA6_MASK             64
#define SIMPTA_SIMPTA7_MASK             128


/*** SIMPTB - System Port B Input Buffer Enable Register; 0xFFFF9838 ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTB0     :1;                                       /* PTB0 Input Buffer Control */
    byte SIMPTB1     :1;                                       /* PTB1 Input Buffer Control */
    byte SIMPTB2     :1;                                       /* PTB2 Input Buffer Control */
    byte SIMPTB3     :1;                                       /* PTB3 Input Buffer Control */
    byte SIMPTB4     :1;                                       /* PTB4 Input Buffer Control */
    byte SIMPTB5     :1;                                       /* PTB5 Input Buffer Control */
    byte SIMPTB6     :1;                                       /* PTB6 Input Buffer Control */
    byte SIMPTB7     :1;                                       /* PTB7 Input Buffer Control */
  } Bits;
} SIMPTBSTR;
extern volatile SIMPTBSTR _SIMPTB @0xFFFF9838;
#define SIMPTB                          _SIMPTB.Byte
#define SIMPTB_SIMPTB0                  _SIMPTB.Bits.SIMPTB0
#define SIMPTB_SIMPTB1                  _SIMPTB.Bits.SIMPTB1
#define SIMPTB_SIMPTB2                  _SIMPTB.Bits.SIMPTB2
#define SIMPTB_SIMPTB3                  _SIMPTB.Bits.SIMPTB3
#define SIMPTB_SIMPTB4                  _SIMPTB.Bits.SIMPTB4
#define SIMPTB_SIMPTB5                  _SIMPTB.Bits.SIMPTB5
#define SIMPTB_SIMPTB6                  _SIMPTB.Bits.SIMPTB6
#define SIMPTB_SIMPTB7                  _SIMPTB.Bits.SIMPTB7

#define SIMPTB_SIMPTB0_MASK             1
#define SIMPTB_SIMPTB1_MASK             2
#define SIMPTB_SIMPTB2_MASK             4
#define SIMPTB_SIMPTB3_MASK             8
#define SIMPTB_SIMPTB4_MASK             16
#define SIMPTB_SIMPTB5_MASK             32
#define SIMPTB_SIMPTB6_MASK             64
#define SIMPTB_SIMPTB7_MASK             128


/*** SIMPTC - System Port C Input Buffer Enable Register; 0xFFFF9839 ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTC0     :1;                                       /* PTC0 Input Buffer Control */
    byte SIMPTC1     :1;                                       /* PTC1 Input Buffer Control */
    byte SIMPTC2     :1;                                       /* PTC2 Input Buffer Control */
    byte SIMPTC3     :1;                                       /* PTC3 Input Buffer Control */
    byte SIMPTC4     :1;                                       /* PTC4 Input Buffer Control */
    byte SIMPTC5     :1;                                       /* PTC5 Input Buffer Control */
    byte SIMPTC6     :1;                                       /* PTC6 Input Buffer Control */
    byte             :1; 
  } Bits;
  struct {
    byte grpSIMPTC :7;
    byte         :1;
  } MergedBits;
} SIMPTCSTR;
extern volatile SIMPTCSTR _SIMPTC @0xFFFF9839;
#define SIMPTC                          _SIMPTC.Byte
#define SIMPTC_SIMPTC0                  _SIMPTC.Bits.SIMPTC0
#define SIMPTC_SIMPTC1                  _SIMPTC.Bits.SIMPTC1
#define SIMPTC_SIMPTC2                  _SIMPTC.Bits.SIMPTC2
#define SIMPTC_SIMPTC3                  _SIMPTC.Bits.SIMPTC3
#define SIMPTC_SIMPTC4                  _SIMPTC.Bits.SIMPTC4
#define SIMPTC_SIMPTC5                  _SIMPTC.Bits.SIMPTC5
#define SIMPTC_SIMPTC6                  _SIMPTC.Bits.SIMPTC6
#define SIMPTC_SIMPTC                   _SIMPTC.MergedBits.grpSIMPTC

#define SIMPTC_SIMPTC0_MASK             1
#define SIMPTC_SIMPTC1_MASK             2
#define SIMPTC_SIMPTC2_MASK             4
#define SIMPTC_SIMPTC3_MASK             8
#define SIMPTC_SIMPTC4_MASK             16
#define SIMPTC_SIMPTC5_MASK             32
#define SIMPTC_SIMPTC6_MASK             64
#define SIMPTC_SIMPTC_MASK              127
#define SIMPTC_SIMPTC_BITNUM            0


/*** SIMPTD - System Port D Input Buffer Enable Register; 0xFFFF983A ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTD0     :1;                                       /* PTD0 Input Buffer Control */
    byte SIMPTD1     :1;                                       /* PTD1 Input Buffer Control */
    byte SIMPTD2     :1;                                       /* PTD2 Input Buffer Control */
    byte SIMPTD3     :1;                                       /* PTD3 Input Buffer Control */
    byte SIMPTD4     :1;                                       /* PTD4 Input Buffer Control */
    byte SIMPTD5     :1;                                       /* PTD5 Input Buffer Control */
    byte SIMPTD6     :1;                                       /* PTD6 Input Buffer Control */
    byte SIMPTD7     :1;                                       /* PTD7 Input Buffer Control */
  } Bits;
} SIMPTDSTR;
extern volatile SIMPTDSTR _SIMPTD @0xFFFF983A;
#define SIMPTD                          _SIMPTD.Byte
#define SIMPTD_SIMPTD0                  _SIMPTD.Bits.SIMPTD0
#define SIMPTD_SIMPTD1                  _SIMPTD.Bits.SIMPTD1
#define SIMPTD_SIMPTD2                  _SIMPTD.Bits.SIMPTD2
#define SIMPTD_SIMPTD3                  _SIMPTD.Bits.SIMPTD3
#define SIMPTD_SIMPTD4                  _SIMPTD.Bits.SIMPTD4
#define SIMPTD_SIMPTD5                  _SIMPTD.Bits.SIMPTD5
#define SIMPTD_SIMPTD6                  _SIMPTD.Bits.SIMPTD6
#define SIMPTD_SIMPTD7                  _SIMPTD.Bits.SIMPTD7

#define SIMPTD_SIMPTD0_MASK             1
#define SIMPTD_SIMPTD1_MASK             2
#define SIMPTD_SIMPTD2_MASK             4
#define SIMPTD_SIMPTD3_MASK             8
#define SIMPTD_SIMPTD4_MASK             16
#define SIMPTD_SIMPTD5_MASK             32
#define SIMPTD_SIMPTD6_MASK             64
#define SIMPTD_SIMPTD7_MASK             128


/*** SIMPTE - System Port E Input Buffer Enable Register; 0xFFFF983B ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTE0     :1;                                       /* PTE0 Input Buffer Control */
    byte SIMPTE1     :1;                                       /* PTE1 Input Buffer Control */
    byte SIMPTE2     :1;                                       /* PTE2 Input Buffer Control */
    byte SIMPTE3     :1;                                       /* PTE3 Input Buffer Control */
    byte SIMPTE4     :1;                                       /* PTE4 Input Buffer Control */
    byte SIMPTE5     :1;                                       /* PTE5 Input Buffer Control */
    byte SIMPTE6     :1;                                       /* PTE6 Input Buffer Control */
    byte SIMPTE7     :1;                                       /* PTE7 Input Buffer Control */
  } Bits;
} SIMPTESTR;
extern volatile SIMPTESTR _SIMPTE @0xFFFF983B;
#define SIMPTE                          _SIMPTE.Byte
#define SIMPTE_SIMPTE0                  _SIMPTE.Bits.SIMPTE0
#define SIMPTE_SIMPTE1                  _SIMPTE.Bits.SIMPTE1
#define SIMPTE_SIMPTE2                  _SIMPTE.Bits.SIMPTE2
#define SIMPTE_SIMPTE3                  _SIMPTE.Bits.SIMPTE3
#define SIMPTE_SIMPTE4                  _SIMPTE.Bits.SIMPTE4
#define SIMPTE_SIMPTE5                  _SIMPTE.Bits.SIMPTE5
#define SIMPTE_SIMPTE6                  _SIMPTE.Bits.SIMPTE6
#define SIMPTE_SIMPTE7                  _SIMPTE.Bits.SIMPTE7

#define SIMPTE_SIMPTE0_MASK             1
#define SIMPTE_SIMPTE1_MASK             2
#define SIMPTE_SIMPTE2_MASK             4
#define SIMPTE_SIMPTE3_MASK             8
#define SIMPTE_SIMPTE4_MASK             16
#define SIMPTE_SIMPTE5_MASK             32
#define SIMPTE_SIMPTE6_MASK             64
#define SIMPTE_SIMPTE7_MASK             128


/*** SIMPTF - System Port F Input Buffer Enable Register; 0xFFFF983C ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTF0     :1;                                       /* PTF0 Input Buffer Control */
    byte SIMPTF1     :1;                                       /* PTF1 Input Buffer Control */
    byte SIMPTF2     :1;                                       /* PTF2 Input Buffer Control */
    byte SIMPTF3     :1;                                       /* PTF3 Input Buffer Control */
    byte SIMPTF4     :1;                                       /* PTF4 Input Buffer Control */
    byte SIMPTF5     :1;                                       /* PTF5 Input Buffer Control */
    byte SIMPTF6     :1;                                       /* PTF6 Input Buffer Control */
    byte SIMPTF7     :1;                                       /* PTF7 Input Buffer Control */
  } Bits;
} SIMPTFSTR;
extern volatile SIMPTFSTR _SIMPTF @0xFFFF983C;
#define SIMPTF                          _SIMPTF.Byte
#define SIMPTF_SIMPTF0                  _SIMPTF.Bits.SIMPTF0
#define SIMPTF_SIMPTF1                  _SIMPTF.Bits.SIMPTF1
#define SIMPTF_SIMPTF2                  _SIMPTF.Bits.SIMPTF2
#define SIMPTF_SIMPTF3                  _SIMPTF.Bits.SIMPTF3
#define SIMPTF_SIMPTF4                  _SIMPTF.Bits.SIMPTF4
#define SIMPTF_SIMPTF5                  _SIMPTF.Bits.SIMPTF5
#define SIMPTF_SIMPTF6                  _SIMPTF.Bits.SIMPTF6
#define SIMPTF_SIMPTF7                  _SIMPTF.Bits.SIMPTF7

#define SIMPTF_SIMPTF0_MASK             1
#define SIMPTF_SIMPTF1_MASK             2
#define SIMPTF_SIMPTF2_MASK             4
#define SIMPTF_SIMPTF3_MASK             8
#define SIMPTF_SIMPTF4_MASK             16
#define SIMPTF_SIMPTF5_MASK             32
#define SIMPTF_SIMPTF6_MASK             64
#define SIMPTF_SIMPTF7_MASK             128


/*** SIMPTG - System Port G Input Buffer Enable Register; 0xFFFF983D ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTG0     :1;                                       /* PTG0 Input Buffer Control */
    byte SIMPTG1     :1;                                       /* PTG1 Input Buffer Control */
    byte SIMPTG2     :1;                                       /* PTG2 Input Buffer Control */
    byte SIMPTG3     :1;                                       /* PTG3 Input Buffer Control */
    byte SIMPTG4     :1;                                       /* PTG4 Input Buffer Control */
    byte SIMPTG5     :1;                                       /* PTG5 Input Buffer Control */
    byte SIMPTG6     :1;                                       /* PTG6 Input Buffer Control */
    byte             :1; 
  } Bits;
  struct {
    byte grpSIMPTG :7;
    byte         :1;
  } MergedBits;
} SIMPTGSTR;
extern volatile SIMPTGSTR _SIMPTG @0xFFFF983D;
#define SIMPTG                          _SIMPTG.Byte
#define SIMPTG_SIMPTG0                  _SIMPTG.Bits.SIMPTG0
#define SIMPTG_SIMPTG1                  _SIMPTG.Bits.SIMPTG1
#define SIMPTG_SIMPTG2                  _SIMPTG.Bits.SIMPTG2
#define SIMPTG_SIMPTG3                  _SIMPTG.Bits.SIMPTG3
#define SIMPTG_SIMPTG4                  _SIMPTG.Bits.SIMPTG4
#define SIMPTG_SIMPTG5                  _SIMPTG.Bits.SIMPTG5
#define SIMPTG_SIMPTG6                  _SIMPTG.Bits.SIMPTG6
#define SIMPTG_SIMPTG                   _SIMPTG.MergedBits.grpSIMPTG

#define SIMPTG_SIMPTG0_MASK             1
#define SIMPTG_SIMPTG1_MASK             2
#define SIMPTG_SIMPTG2_MASK             4
#define SIMPTG_SIMPTG3_MASK             8
#define SIMPTG_SIMPTG4_MASK             16
#define SIMPTG_SIMPTG5_MASK             32
#define SIMPTG_SIMPTG6_MASK             64
#define SIMPTG_SIMPTG_MASK              127
#define SIMPTG_SIMPTG_BITNUM            0


/*** SIMPTH - System Port H Input Buffer Enable Register; 0xFFFF983E ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTH0     :1;                                       /* PTH0 Input Buffer Control */
    byte SIMPTH1     :1;                                       /* PTH1 Input Buffer Control */
    byte SIMPTH2     :1;                                       /* PTH2 Input Buffer Control */
    byte SIMPTH3     :1;                                       /* PTH3 Input Buffer Control */
    byte SIMPTH4     :1;                                       /* PTH4 Input Buffer Control */
    byte SIMPTH5     :1;                                       /* PTH5 Input Buffer Control */
    byte SIMPTH6     :1;                                       /* PTH6 Input Buffer Control */
    byte             :1; 
  } Bits;
  struct {
    byte grpSIMPTH :7;
    byte         :1;
  } MergedBits;
} SIMPTHSTR;
extern volatile SIMPTHSTR _SIMPTH @0xFFFF983E;
#define SIMPTH                          _SIMPTH.Byte
#define SIMPTH_SIMPTH0                  _SIMPTH.Bits.SIMPTH0
#define SIMPTH_SIMPTH1                  _SIMPTH.Bits.SIMPTH1
#define SIMPTH_SIMPTH2                  _SIMPTH.Bits.SIMPTH2
#define SIMPTH_SIMPTH3                  _SIMPTH.Bits.SIMPTH3
#define SIMPTH_SIMPTH4                  _SIMPTH.Bits.SIMPTH4
#define SIMPTH_SIMPTH5                  _SIMPTH.Bits.SIMPTH5
#define SIMPTH_SIMPTH6                  _SIMPTH.Bits.SIMPTH6
#define SIMPTH_SIMPTH                   _SIMPTH.MergedBits.grpSIMPTH

#define SIMPTH_SIMPTH0_MASK             1
#define SIMPTH_SIMPTH1_MASK             2
#define SIMPTH_SIMPTH2_MASK             4
#define SIMPTH_SIMPTH3_MASK             8
#define SIMPTH_SIMPTH4_MASK             16
#define SIMPTH_SIMPTH5_MASK             32
#define SIMPTH_SIMPTH6_MASK             64
#define SIMPTH_SIMPTH_MASK              127
#define SIMPTH_SIMPTH_BITNUM            0


/*** SIMPTJ - System Port J Input Buffer Enable Register; 0xFFFF983F ***/
typedef union {
  byte Byte;
  struct {
    byte SIMPTJ0     :1;                                       /* PTJ0 Input Buffer Control */
    byte SIMPTJ1     :1;                                       /* PTJ1 Input Buffer Control */
    byte SIMPTJ2     :1;                                       /* PTJ2 Input Buffer Control */
    byte SIMPTJ3     :1;                                       /* PTJ3 Input Buffer Control */
    byte SIMPTJ4     :1;                                       /* PTJ4 Input Buffer Control */
    byte SIMPTJ5     :1;                                       /* PTJ5 Input Buffer Control */
    byte SIMPTJ6     :1;                                       /* PTJ6 Input Buffer Control */
    byte SIMPTJ7     :1;                                       /* PTJ7 Input Buffer Control */
  } Bits;
} SIMPTJSTR;
extern volatile SIMPTJSTR _SIMPTJ @0xFFFF983F;
#define SIMPTJ                          _SIMPTJ.Byte
#define SIMPTJ_SIMPTJ0                  _SIMPTJ.Bits.SIMPTJ0
#define SIMPTJ_SIMPTJ1                  _SIMPTJ.Bits.SIMPTJ1
#define SIMPTJ_SIMPTJ2                  _SIMPTJ.Bits.SIMPTJ2
#define SIMPTJ_SIMPTJ3                  _SIMPTJ.Bits.SIMPTJ3
#define SIMPTJ_SIMPTJ4                  _SIMPTJ.Bits.SIMPTJ4
#define SIMPTJ_SIMPTJ5                  _SIMPTJ.Bits.SIMPTJ5
#define SIMPTJ_SIMPTJ6                  _SIMPTJ.Bits.SIMPTJ6
#define SIMPTJ_SIMPTJ7                  _SIMPTJ.Bits.SIMPTJ7

#define SIMPTJ_SIMPTJ0_MASK             1
#define SIMPTJ_SIMPTJ1_MASK             2
#define SIMPTJ_SIMPTJ2_MASK             4
#define SIMPTJ_SIMPTJ3_MASK             8
#define SIMPTJ_SIMPTJ4_MASK             16
#define SIMPTJ_SIMPTJ5_MASK             32
#define SIMPTJ_SIMPTJ6_MASK             64
#define SIMPTJ_SIMPTJ7_MASK             128


/*** EWMCTRL - EWM Control Register; 0xFFFF9840 ***/
typedef union {
  byte Byte;
  struct {
    byte EWMEN       :1;                                       /* Module enable */
    byte ASSIN       :1;                                       /* EWM_in assert state invert */
    byte INEN        :1;                                       /* EWM_in enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} EWMCTRLSTR;
extern volatile EWMCTRLSTR _EWMCTRL @0xFFFF9840;
#define EWMCTRL                         _EWMCTRL.Byte
#define EWMCTRL_EWMEN                   _EWMCTRL.Bits.EWMEN
#define EWMCTRL_ASSIN                   _EWMCTRL.Bits.ASSIN
#define EWMCTRL_INEN                    _EWMCTRL.Bits.INEN

#define EWMCTRL_EWMEN_MASK              1
#define EWMCTRL_ASSIN_MASK              2
#define EWMCTRL_INEN_MASK               4


/*** EWMSERV - EWM Service Register; 0xFFFF9841 ***/
typedef union {
  byte Byte;
} EWMSERVSTR;
extern volatile EWMSERVSTR _EWMSERV @0xFFFF9841;
#define EWMSERV                         _EWMSERV.Byte


/*** EWMCMPL - EWM Compare Low Register; 0xFFFF9842 ***/
typedef union {
  byte Byte;
} EWMCMPLSTR;
extern volatile EWMCMPLSTR _EWMCMPL @0xFFFF9842;
#define EWMCMPL                         _EWMCMPL.Byte


/*** EWMCMPH - EWM Compare High Register; 0xFFFF9843 ***/
typedef union {
  byte Byte;
} EWMCMPHSTR;
extern volatile EWMCMPHSTR _EWMCMPH @0xFFFF9843;
#define EWMCMPH                         _EWMCMPH.Byte


/*** IRQSC - Interrupt request status and control register; 0xFFFF9844 ***/
typedef union {
  byte Byte;
  struct {
    byte IRQMOD      :1;                                       /* IRQ Detection Mode */
    byte IRQIE       :1;                                       /* IRQ Interrupt Enable */
    byte IRQACK      :1;                                       /* IRQ Acknowledge */
    byte IRQF        :1;                                       /* IRQ Flag */
    byte IRQPE       :1;                                       /* IRQ Pin Enable */
    byte IRQEDG      :1;                                       /* IRQ Edge Select */
    byte IRQPDD      :1;                                       /* IRQ Pull Device Disable */
    byte             :1; 
  } Bits;
} IRQSCSTR;
extern volatile IRQSCSTR _IRQSC @0xFFFF9844;
#define IRQSC                           _IRQSC.Byte
#define IRQSC_IRQMOD                    _IRQSC.Bits.IRQMOD
#define IRQSC_IRQIE                     _IRQSC.Bits.IRQIE
#define IRQSC_IRQACK                    _IRQSC.Bits.IRQACK
#define IRQSC_IRQF                      _IRQSC.Bits.IRQF
#define IRQSC_IRQPE                     _IRQSC.Bits.IRQPE
#define IRQSC_IRQEDG                    _IRQSC.Bits.IRQEDG
#define IRQSC_IRQPDD                    _IRQSC.Bits.IRQPDD

#define IRQSC_IRQMOD_MASK               1
#define IRQSC_IRQIE_MASK                2
#define IRQSC_IRQACK_MASK               4
#define IRQSC_IRQF_MASK                 8
#define IRQSC_IRQPE_MASK                16
#define IRQSC_IRQEDG_MASK               32
#define IRQSC_IRQPDD_MASK               64


/*** CRCH - CRC High Register; 0xFFFF9850 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT8        :1;                                       /* High byte of CRC register ,bit 8 */
    byte BIT9        :1;                                       /* High byte of CRC register ,bit 9 */
    byte BIT10       :1;                                       /* High byte of CRC register ,bit 10 */
    byte BIT11       :1;                                       /* High byte of CRC register ,bit 11 */
    byte BIT12       :1;                                       /* High byte of CRC register ,bit 12 */
    byte BIT13       :1;                                       /* High byte of CRC register ,bit 13 */
    byte BIT14       :1;                                       /* High byte of CRC register ,bit 14 */
    byte BIT15       :1;                                       /* High byte of CRC register ,bit 15 */
  } Bits;
} CRCHSTR;
extern volatile CRCHSTR _CRCH @0xFFFF9850;
#define CRCH                            _CRCH.Byte
#define CRCH_BIT8                       _CRCH.Bits.BIT8
#define CRCH_BIT9                       _CRCH.Bits.BIT9
#define CRCH_BIT10                      _CRCH.Bits.BIT10
#define CRCH_BIT11                      _CRCH.Bits.BIT11
#define CRCH_BIT12                      _CRCH.Bits.BIT12
#define CRCH_BIT13                      _CRCH.Bits.BIT13
#define CRCH_BIT14                      _CRCH.Bits.BIT14
#define CRCH_BIT15                      _CRCH.Bits.BIT15

#define CRCH_BIT8_MASK                  1
#define CRCH_BIT9_MASK                  2
#define CRCH_BIT10_MASK                 4
#define CRCH_BIT11_MASK                 8
#define CRCH_BIT12_MASK                 16
#define CRCH_BIT13_MASK                 32
#define CRCH_BIT14_MASK                 64
#define CRCH_BIT15_MASK                 128


/*** CRCL - CRC Low Register; 0xFFFF9851 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* Low byte of CRC register, bit 0 */
    byte BIT1        :1;                                       /* Low byte of CRC register, bit 1 */
    byte BIT2        :1;                                       /* Low byte of CRC register, bit 2 */
    byte BIT3        :1;                                       /* Low byte of CRC register, bit 3 */
    byte BIT4        :1;                                       /* Low byte of CRC register, bit 4 */
    byte BIT5        :1;                                       /* Low byte of CRC register, bit 5 */
    byte BIT6        :1;                                       /* Low byte of CRC register, bit 6 */
    byte BIT7        :1;                                       /* Low byte of CRC register, bit 7 */
  } Bits;
} CRCLSTR;
extern volatile CRCLSTR _CRCL @0xFFFF9851;
#define CRCL                            _CRCL.Byte
#define CRCL_BIT0                       _CRCL.Bits.BIT0
#define CRCL_BIT1                       _CRCL.Bits.BIT1
#define CRCL_BIT2                       _CRCL.Bits.BIT2
#define CRCL_BIT3                       _CRCL.Bits.BIT3
#define CRCL_BIT4                       _CRCL.Bits.BIT4
#define CRCL_BIT5                       _CRCL.Bits.BIT5
#define CRCL_BIT6                       _CRCL.Bits.BIT6
#define CRCL_BIT7                       _CRCL.Bits.BIT7

#define CRCL_BIT0_MASK                  1
#define CRCL_BIT1_MASK                  2
#define CRCL_BIT2_MASK                  4
#define CRCL_BIT3_MASK                  8
#define CRCL_BIT4_MASK                  16
#define CRCL_BIT5_MASK                  32
#define CRCL_BIT6_MASK                  64
#define CRCL_BIT7_MASK                  128


/*** TRANSPOSE - CRC Transpose Register; 0xFFFF9852 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* CRC transpose register, bit 0 */
    byte BIT1        :1;                                       /* CRC transpose register, bit 1 */
    byte BIT2        :1;                                       /* CRC transpose register, bit 2 */
    byte BIT3        :1;                                       /* CRC transpose register, bit 3 */
    byte BIT4        :1;                                       /* CRC transpose register, bit 4 */
    byte BIT5        :1;                                       /* CRC transpose register, bit 5 */
    byte BIT6        :1;                                       /* CRC transpose register, bit 6 */
    byte BIT7        :1;                                       /* CRC transpose register, bit 7 */
  } Bits;
} TRANSPOSESTR;
extern volatile TRANSPOSESTR _TRANSPOSE @0xFFFF9852;
#define TRANSPOSE                       _TRANSPOSE.Byte
#define TRANSPOSE_BIT0                  _TRANSPOSE.Bits.BIT0
#define TRANSPOSE_BIT1                  _TRANSPOSE.Bits.BIT1
#define TRANSPOSE_BIT2                  _TRANSPOSE.Bits.BIT2
#define TRANSPOSE_BIT3                  _TRANSPOSE.Bits.BIT3
#define TRANSPOSE_BIT4                  _TRANSPOSE.Bits.BIT4
#define TRANSPOSE_BIT5                  _TRANSPOSE.Bits.BIT5
#define TRANSPOSE_BIT6                  _TRANSPOSE.Bits.BIT6
#define TRANSPOSE_BIT7                  _TRANSPOSE.Bits.BIT7

#define TRANSPOSE_BIT0_MASK             1
#define TRANSPOSE_BIT1_MASK             2
#define TRANSPOSE_BIT2_MASK             4
#define TRANSPOSE_BIT3_MASK             8
#define TRANSPOSE_BIT4_MASK             16
#define TRANSPOSE_BIT5_MASK             32
#define TRANSPOSE_BIT6_MASK             64
#define TRANSPOSE_BIT7_MASK             128


/*** CRCL0 - CRC Low Register 0; 0xFFFF9854 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* Low byte of CRC register, bit 0 */
    byte BIT1        :1;                                       /* Low byte of CRC register, bit 1 */
    byte BIT2        :1;                                       /* Low byte of CRC register, bit 2 */
    byte BIT3        :1;                                       /* Low byte of CRC register, bit 3 */
    byte BIT4        :1;                                       /* Low byte of CRC register, bit 4 */
    byte BIT5        :1;                                       /* Low byte of CRC register, bit 5 */
    byte BIT6        :1;                                       /* Low byte of CRC register, bit 6 */
    byte BIT7        :1;                                       /* Low byte of CRC register, bit 7 */
  } Bits;
} CRCL0STR;
extern volatile CRCL0STR _CRCL0 @0xFFFF9854;
#define CRCL0                           _CRCL0.Byte
#define CRCL0_BIT0                      _CRCL0.Bits.BIT0
#define CRCL0_BIT1                      _CRCL0.Bits.BIT1
#define CRCL0_BIT2                      _CRCL0.Bits.BIT2
#define CRCL0_BIT3                      _CRCL0.Bits.BIT3
#define CRCL0_BIT4                      _CRCL0.Bits.BIT4
#define CRCL0_BIT5                      _CRCL0.Bits.BIT5
#define CRCL0_BIT6                      _CRCL0.Bits.BIT6
#define CRCL0_BIT7                      _CRCL0.Bits.BIT7
/* CRCL_ARR: Access 4 CRCLx registers in an array */
#define CRCL_ARR                        ((volatile byte *) &CRCL0)

#define CRCL0_BIT0_MASK                 1
#define CRCL0_BIT1_MASK                 2
#define CRCL0_BIT2_MASK                 4
#define CRCL0_BIT3_MASK                 8
#define CRCL0_BIT4_MASK                 16
#define CRCL0_BIT5_MASK                 32
#define CRCL0_BIT6_MASK                 64
#define CRCL0_BIT7_MASK                 128


/*** CRCL1 - CRC Low Register 1; 0xFFFF9855 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* Low byte of CRC register, bit 0 */
    byte BIT1        :1;                                       /* Low byte of CRC register, bit 1 */
    byte BIT2        :1;                                       /* Low byte of CRC register, bit 2 */
    byte BIT3        :1;                                       /* Low byte of CRC register, bit 3 */
    byte BIT4        :1;                                       /* Low byte of CRC register, bit 4 */
    byte BIT5        :1;                                       /* Low byte of CRC register, bit 5 */
    byte BIT6        :1;                                       /* Low byte of CRC register, bit 6 */
    byte BIT7        :1;                                       /* Low byte of CRC register, bit 7 */
  } Bits;
} CRCL1STR;
extern volatile CRCL1STR _CRCL1 @0xFFFF9855;
#define CRCL1                           _CRCL1.Byte
#define CRCL1_BIT0                      _CRCL1.Bits.BIT0
#define CRCL1_BIT1                      _CRCL1.Bits.BIT1
#define CRCL1_BIT2                      _CRCL1.Bits.BIT2
#define CRCL1_BIT3                      _CRCL1.Bits.BIT3
#define CRCL1_BIT4                      _CRCL1.Bits.BIT4
#define CRCL1_BIT5                      _CRCL1.Bits.BIT5
#define CRCL1_BIT6                      _CRCL1.Bits.BIT6
#define CRCL1_BIT7                      _CRCL1.Bits.BIT7

#define CRCL1_BIT0_MASK                 1
#define CRCL1_BIT1_MASK                 2
#define CRCL1_BIT2_MASK                 4
#define CRCL1_BIT3_MASK                 8
#define CRCL1_BIT4_MASK                 16
#define CRCL1_BIT5_MASK                 32
#define CRCL1_BIT6_MASK                 64
#define CRCL1_BIT7_MASK                 128


/*** CRCL2 - CRC Low Register 2; 0xFFFF9856 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* Low byte of CRC register, bit 0 */
    byte BIT1        :1;                                       /* Low byte of CRC register, bit 1 */
    byte BIT2        :1;                                       /* Low byte of CRC register, bit 2 */
    byte BIT3        :1;                                       /* Low byte of CRC register, bit 3 */
    byte BIT4        :1;                                       /* Low byte of CRC register, bit 4 */
    byte BIT5        :1;                                       /* Low byte of CRC register, bit 5 */
    byte BIT6        :1;                                       /* Low byte of CRC register, bit 6 */
    byte BIT7        :1;                                       /* Low byte of CRC register, bit 7 */
  } Bits;
} CRCL2STR;
extern volatile CRCL2STR _CRCL2 @0xFFFF9856;
#define CRCL2                           _CRCL2.Byte
#define CRCL2_BIT0                      _CRCL2.Bits.BIT0
#define CRCL2_BIT1                      _CRCL2.Bits.BIT1
#define CRCL2_BIT2                      _CRCL2.Bits.BIT2
#define CRCL2_BIT3                      _CRCL2.Bits.BIT3
#define CRCL2_BIT4                      _CRCL2.Bits.BIT4
#define CRCL2_BIT5                      _CRCL2.Bits.BIT5
#define CRCL2_BIT6                      _CRCL2.Bits.BIT6
#define CRCL2_BIT7                      _CRCL2.Bits.BIT7

#define CRCL2_BIT0_MASK                 1
#define CRCL2_BIT1_MASK                 2
#define CRCL2_BIT2_MASK                 4
#define CRCL2_BIT3_MASK                 8
#define CRCL2_BIT4_MASK                 16
#define CRCL2_BIT5_MASK                 32
#define CRCL2_BIT6_MASK                 64
#define CRCL2_BIT7_MASK                 128


/*** CRCL3 - CRC Low Register 3; 0xFFFF9857 ***/
typedef union {
  byte Byte;
  struct {
    byte BIT0        :1;                                       /* Low byte of CRC register, bit 0 */
    byte BIT1        :1;                                       /* Low byte of CRC register, bit 1 */
    byte BIT2        :1;                                       /* Low byte of CRC register, bit 2 */
    byte BIT3        :1;                                       /* Low byte of CRC register, bit 3 */
    byte BIT4        :1;                                       /* Low byte of CRC register, bit 4 */
    byte BIT5        :1;                                       /* Low byte of CRC register, bit 5 */
    byte BIT6        :1;                                       /* Low byte of CRC register, bit 6 */
    byte BIT7        :1;                                       /* Low byte of CRC register, bit 7 */
  } Bits;
} CRCL3STR;
extern volatile CRCL3STR _CRCL3 @0xFFFF9857;
#define CRCL3                           _CRCL3.Byte
#define CRCL3_BIT0                      _CRCL3.Bits.BIT0
#define CRCL3_BIT1                      _CRCL3.Bits.BIT1
#define CRCL3_BIT2                      _CRCL3.Bits.BIT2
#define CRCL3_BIT3                      _CRCL3.Bits.BIT3
#define CRCL3_BIT4                      _CRCL3.Bits.BIT4
#define CRCL3_BIT5                      _CRCL3.Bits.BIT5
#define CRCL3_BIT6                      _CRCL3.Bits.BIT6
#define CRCL3_BIT7                      _CRCL3.Bits.BIT7

#define CRCL3_BIT0_MASK                 1
#define CRCL3_BIT1_MASK                 2
#define CRCL3_BIT2_MASK                 4
#define CRCL3_BIT3_MASK                 8
#define CRCL3_BIT4_MASK                 16
#define CRCL3_BIT5_MASK                 32
#define CRCL3_BIT6_MASK                 64
#define CRCL3_BIT7_MASK                 128


/*** PDBCTRL1 - PDB Control 1 Register; 0xFFFF9860 ***/
typedef union {
  byte Byte;
  struct {
    byte LDOK        :1;                                       /* Load OK */
    byte ENA         :1;                                       /* Trigger A Enable */
    byte AOS0        :1;                                       /* Trigger A Output Select, bit 0 */
    byte AOS1        :1;                                       /* Trigger A Output Select, bit 1 */
    byte ENB         :1;                                       /* Trigger B Enable */
    byte BOS0        :1;                                       /* Trigger B Output Select, bit 0 */
    byte BOS1        :1;                                       /* Trigger B Output Select, bit 1 */
    byte LDMOD       :1;                                       /* Load mode select */
  } Bits;
  struct {
    byte         :1;
    byte grpENx  :1;
    byte grpAOS  :2;
    byte         :1;
    byte grpBOS  :2;
    byte         :1;
  } MergedBits;
} PDBCTRL1STR;
extern volatile PDBCTRL1STR _PDBCTRL1 @0xFFFF9860;
#define PDBCTRL1                        _PDBCTRL1.Byte
#define PDBCTRL1_LDOK                   _PDBCTRL1.Bits.LDOK
#define PDBCTRL1_ENA                    _PDBCTRL1.Bits.ENA
#define PDBCTRL1_AOS0                   _PDBCTRL1.Bits.AOS0
#define PDBCTRL1_AOS1                   _PDBCTRL1.Bits.AOS1
#define PDBCTRL1_ENB                    _PDBCTRL1.Bits.ENB
#define PDBCTRL1_BOS0                   _PDBCTRL1.Bits.BOS0
#define PDBCTRL1_BOS1                   _PDBCTRL1.Bits.BOS1
#define PDBCTRL1_LDMOD                  _PDBCTRL1.Bits.LDMOD
#define PDBCTRL1_AOS                    _PDBCTRL1.MergedBits.grpAOS
#define PDBCTRL1_BOS                    _PDBCTRL1.MergedBits.grpBOS

#define PDBCTRL1_LDOK_MASK              1
#define PDBCTRL1_ENA_MASK               2
#define PDBCTRL1_AOS0_MASK              4
#define PDBCTRL1_AOS1_MASK              8
#define PDBCTRL1_ENB_MASK               16
#define PDBCTRL1_BOS0_MASK              32
#define PDBCTRL1_BOS1_MASK              64
#define PDBCTRL1_LDMOD_MASK             128
#define PDBCTRL1_AOS_MASK               12
#define PDBCTRL1_AOS_BITNUM             2
#define PDBCTRL1_BOS_MASK               96
#define PDBCTRL1_BOS_BITNUM             5


/*** PDBCTRL2 - PDB Control 2 Register; 0xFFFF9861 ***/
typedef union {
  byte Byte;
  struct {
    byte SWTRIG      :1;                                       /* Software Trigger */
    byte CONT        :1;                                       /* Continuous Mode Enable */
    byte TRIGSEL0    :1;                                       /* Input Trigger, bit 0 */
    byte TRIGSEL1    :1;                                       /* Input Trigger, bit 1 */
    byte TRIGSEL2    :1;                                       /* Input Trigger, bit 2 */
    byte PRESCALER0  :1;                                       /* Clock Prescaler, bit 0 */
    byte PRESCALER1  :1;                                       /* Clock Prescaler, bit 1 */
    byte PRESCALER2  :1;                                       /* Clock Prescaler, bit 2 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpTRIGSEL :3;
    byte grpPRESCALER :3;
  } MergedBits;
} PDBCTRL2STR;
extern volatile PDBCTRL2STR _PDBCTRL2 @0xFFFF9861;
#define PDBCTRL2                        _PDBCTRL2.Byte
#define PDBCTRL2_SWTRIG                 _PDBCTRL2.Bits.SWTRIG
#define PDBCTRL2_CONT                   _PDBCTRL2.Bits.CONT
#define PDBCTRL2_TRIGSEL0               _PDBCTRL2.Bits.TRIGSEL0
#define PDBCTRL2_TRIGSEL1               _PDBCTRL2.Bits.TRIGSEL1
#define PDBCTRL2_TRIGSEL2               _PDBCTRL2.Bits.TRIGSEL2
#define PDBCTRL2_PRESCALER0             _PDBCTRL2.Bits.PRESCALER0
#define PDBCTRL2_PRESCALER1             _PDBCTRL2.Bits.PRESCALER1
#define PDBCTRL2_PRESCALER2             _PDBCTRL2.Bits.PRESCALER2
#define PDBCTRL2_TRIGSEL                _PDBCTRL2.MergedBits.grpTRIGSEL
#define PDBCTRL2_PRESCALER              _PDBCTRL2.MergedBits.grpPRESCALER

#define PDBCTRL2_SWTRIG_MASK            1
#define PDBCTRL2_CONT_MASK              2
#define PDBCTRL2_TRIGSEL0_MASK          4
#define PDBCTRL2_TRIGSEL1_MASK          8
#define PDBCTRL2_TRIGSEL2_MASK          16
#define PDBCTRL2_PRESCALER0_MASK        32
#define PDBCTRL2_PRESCALER1_MASK        64
#define PDBCTRL2_PRESCALER2_MASK        128
#define PDBCTRL2_TRIGSEL_MASK           28
#define PDBCTRL2_TRIGSEL_BITNUM         2
#define PDBCTRL2_PRESCALER_MASK         224
#define PDBCTRL2_PRESCALER_BITNUM       5


/*** PDBDLYA - PDB Delay A Register; 0xFFFF9862 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** PDBDLYAH - PDB Delay A High Register; 0xFFFF9862 ***/
    union {
      byte Byte;
      struct {
        byte DELAYA8     :1;                                       /* Delay A, bit 8 */
        byte DELAYA9     :1;                                       /* Delay A, bit 9 */
        byte DELAYA10    :1;                                       /* Delay A, bit 10 */
        byte DELAYA11    :1;                                       /* Delay A, bit 11 */
        byte DELAYA12    :1;                                       /* Delay A, bit 12 */
        byte DELAYA13    :1;                                       /* Delay A, bit 13 */
        byte DELAYA14    :1;                                       /* Delay A, bit 14 */
        byte DELAYA15    :1;                                       /* Delay A, bit 15 */
      } Bits;
    } PDBDLYAHSTR;
    #define PDBDLYAH                    _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Byte
    #define PDBDLYAH_DELAYA8            _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA8
    #define PDBDLYAH_DELAYA9            _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA9
    #define PDBDLYAH_DELAYA10           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA10
    #define PDBDLYAH_DELAYA11           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA11
    #define PDBDLYAH_DELAYA12           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA12
    #define PDBDLYAH_DELAYA13           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA13
    #define PDBDLYAH_DELAYA14           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA14
    #define PDBDLYAH_DELAYA15           _PDBDLYA.Overlap_STR.PDBDLYAHSTR.Bits.DELAYA15
    
    #define PDBDLYAH_DELAYA8_MASK       1
    #define PDBDLYAH_DELAYA9_MASK       2
    #define PDBDLYAH_DELAYA10_MASK      4
    #define PDBDLYAH_DELAYA11_MASK      8
    #define PDBDLYAH_DELAYA12_MASK      16
    #define PDBDLYAH_DELAYA13_MASK      32
    #define PDBDLYAH_DELAYA14_MASK      64
    #define PDBDLYAH_DELAYA15_MASK      128
    

    /*** PDBDLYAL - PDB Delay A Low Register; 0xFFFF9863 ***/
    union {
      byte Byte;
      struct {
        byte DELAYA0     :1;                                       /* Delay A, bit 0 */
        byte DELAYA1     :1;                                       /* Delay A, bit 1 */
        byte DELAYA2     :1;                                       /* Delay A, bit 2 */
        byte DELAYA3     :1;                                       /* Delay A, bit 3 */
        byte DELAYA4     :1;                                       /* Delay A, bit 4 */
        byte DELAYA5     :1;                                       /* Delay A, bit 5 */
        byte DELAYA6     :1;                                       /* Delay A, bit 6 */
        byte DELAYA7     :1;                                       /* Delay A, bit 7 */
      } Bits;
    } PDBDLYALSTR;
    #define PDBDLYAL                    _PDBDLYA.Overlap_STR.PDBDLYALSTR.Byte
    #define PDBDLYAL_DELAYA0            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA0
    #define PDBDLYAL_DELAYA1            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA1
    #define PDBDLYAL_DELAYA2            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA2
    #define PDBDLYAL_DELAYA3            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA3
    #define PDBDLYAL_DELAYA4            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA4
    #define PDBDLYAL_DELAYA5            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA5
    #define PDBDLYAL_DELAYA6            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA6
    #define PDBDLYAL_DELAYA7            _PDBDLYA.Overlap_STR.PDBDLYALSTR.Bits.DELAYA7
    
    #define PDBDLYAL_DELAYA0_MASK       1
    #define PDBDLYAL_DELAYA1_MASK       2
    #define PDBDLYAL_DELAYA2_MASK       4
    #define PDBDLYAL_DELAYA3_MASK       8
    #define PDBDLYAL_DELAYA4_MASK       16
    #define PDBDLYAL_DELAYA5_MASK       32
    #define PDBDLYAL_DELAYA6_MASK       64
    #define PDBDLYAL_DELAYA7_MASK       128
    
  } Overlap_STR;

  struct {
    word DELAYA0     :1;                                       /* Delay A, bit 0 */
    word DELAYA1     :1;                                       /* Delay A, bit 1 */
    word DELAYA2     :1;                                       /* Delay A, bit 2 */
    word DELAYA3     :1;                                       /* Delay A, bit 3 */
    word DELAYA4     :1;                                       /* Delay A, bit 4 */
    word DELAYA5     :1;                                       /* Delay A, bit 5 */
    word DELAYA6     :1;                                       /* Delay A, bit 6 */
    word DELAYA7     :1;                                       /* Delay A, bit 7 */
    word DELAYA8     :1;                                       /* Delay A, bit 8 */
    word DELAYA9     :1;                                       /* Delay A, bit 9 */
    word DELAYA10    :1;                                       /* Delay A, bit 10 */
    word DELAYA11    :1;                                       /* Delay A, bit 11 */
    word DELAYA12    :1;                                       /* Delay A, bit 12 */
    word DELAYA13    :1;                                       /* Delay A, bit 13 */
    word DELAYA14    :1;                                       /* Delay A, bit 14 */
    word DELAYA15    :1;                                       /* Delay A, bit 15 */
  } Bits;
} PDBDLYASTR;
extern volatile PDBDLYASTR _PDBDLYA @0xFFFF9862;
#define PDBDLYA                         _PDBDLYA.Word
#define PDBDLYA_DELAYA0                 _PDBDLYA.Bits.DELAYA0
#define PDBDLYA_DELAYA1                 _PDBDLYA.Bits.DELAYA1
#define PDBDLYA_DELAYA2                 _PDBDLYA.Bits.DELAYA2
#define PDBDLYA_DELAYA3                 _PDBDLYA.Bits.DELAYA3
#define PDBDLYA_DELAYA4                 _PDBDLYA.Bits.DELAYA4
#define PDBDLYA_DELAYA5                 _PDBDLYA.Bits.DELAYA5
#define PDBDLYA_DELAYA6                 _PDBDLYA.Bits.DELAYA6
#define PDBDLYA_DELAYA7                 _PDBDLYA.Bits.DELAYA7
#define PDBDLYA_DELAYA8                 _PDBDLYA.Bits.DELAYA8
#define PDBDLYA_DELAYA9                 _PDBDLYA.Bits.DELAYA9
#define PDBDLYA_DELAYA10                _PDBDLYA.Bits.DELAYA10
#define PDBDLYA_DELAYA11                _PDBDLYA.Bits.DELAYA11
#define PDBDLYA_DELAYA12                _PDBDLYA.Bits.DELAYA12
#define PDBDLYA_DELAYA13                _PDBDLYA.Bits.DELAYA13
#define PDBDLYA_DELAYA14                _PDBDLYA.Bits.DELAYA14
#define PDBDLYA_DELAYA15                _PDBDLYA.Bits.DELAYA15

#define PDBDLYA_DELAYA0_MASK            1
#define PDBDLYA_DELAYA1_MASK            2
#define PDBDLYA_DELAYA2_MASK            4
#define PDBDLYA_DELAYA3_MASK            8
#define PDBDLYA_DELAYA4_MASK            16
#define PDBDLYA_DELAYA5_MASK            32
#define PDBDLYA_DELAYA6_MASK            64
#define PDBDLYA_DELAYA7_MASK            128
#define PDBDLYA_DELAYA8_MASK            256
#define PDBDLYA_DELAYA9_MASK            512
#define PDBDLYA_DELAYA10_MASK           1024
#define PDBDLYA_DELAYA11_MASK           2048
#define PDBDLYA_DELAYA12_MASK           4096
#define PDBDLYA_DELAYA13_MASK           8192
#define PDBDLYA_DELAYA14_MASK           16384
#define PDBDLYA_DELAYA15_MASK           32768


/*** PDBDLYB - PDB Delay B Register; 0xFFFF9864 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** PDBDLYBH - PDB Delay B High Register; 0xFFFF9864 ***/
    union {
      byte Byte;
      struct {
        byte DELAYB8     :1;                                       /* Delay B, bit 8 */
        byte DELAYB9     :1;                                       /* Delay B, bit 9 */
        byte DELAYB10    :1;                                       /* Delay B, bit 10 */
        byte DELAYB11    :1;                                       /* Delay B, bit 11 */
        byte DELAYB12    :1;                                       /* Delay B, bit 12 */
        byte DELAYB13    :1;                                       /* Delay B, bit 13 */
        byte DELAYB14    :1;                                       /* Delay B, bit 14 */
        byte DELAYB15    :1;                                       /* Delay B, bit 15 */
      } Bits;
    } PDBDLYBHSTR;
    #define PDBDLYBH                    _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Byte
    #define PDBDLYBH_DELAYB8            _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB8
    #define PDBDLYBH_DELAYB9            _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB9
    #define PDBDLYBH_DELAYB10           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB10
    #define PDBDLYBH_DELAYB11           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB11
    #define PDBDLYBH_DELAYB12           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB12
    #define PDBDLYBH_DELAYB13           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB13
    #define PDBDLYBH_DELAYB14           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB14
    #define PDBDLYBH_DELAYB15           _PDBDLYB.Overlap_STR.PDBDLYBHSTR.Bits.DELAYB15
    
    #define PDBDLYBH_DELAYB8_MASK       1
    #define PDBDLYBH_DELAYB9_MASK       2
    #define PDBDLYBH_DELAYB10_MASK      4
    #define PDBDLYBH_DELAYB11_MASK      8
    #define PDBDLYBH_DELAYB12_MASK      16
    #define PDBDLYBH_DELAYB13_MASK      32
    #define PDBDLYBH_DELAYB14_MASK      64
    #define PDBDLYBH_DELAYB15_MASK      128
    

    /*** PDBDLYBL - PDB Delay B Low Register; 0xFFFF9865 ***/
    union {
      byte Byte;
      struct {
        byte DELAYB0     :1;                                       /* Delay B, bit 0 */
        byte DELAYB1     :1;                                       /* Delay B, bit 1 */
        byte DELAYB2     :1;                                       /* Delay B, bit 2 */
        byte DELAYB3     :1;                                       /* Delay B, bit 3 */
        byte DELAYB4     :1;                                       /* Delay B, bit 4 */
        byte DELAYB5     :1;                                       /* Delay B, bit 5 */
        byte DELAYB6     :1;                                       /* Delay B, bit 6 */
        byte DELAYB7     :1;                                       /* Delay B, bit 7 */
      } Bits;
    } PDBDLYBLSTR;
    #define PDBDLYBL                    _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Byte
    #define PDBDLYBL_DELAYB0            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB0
    #define PDBDLYBL_DELAYB1            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB1
    #define PDBDLYBL_DELAYB2            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB2
    #define PDBDLYBL_DELAYB3            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB3
    #define PDBDLYBL_DELAYB4            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB4
    #define PDBDLYBL_DELAYB5            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB5
    #define PDBDLYBL_DELAYB6            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB6
    #define PDBDLYBL_DELAYB7            _PDBDLYB.Overlap_STR.PDBDLYBLSTR.Bits.DELAYB7
    
    #define PDBDLYBL_DELAYB0_MASK       1
    #define PDBDLYBL_DELAYB1_MASK       2
    #define PDBDLYBL_DELAYB2_MASK       4
    #define PDBDLYBL_DELAYB3_MASK       8
    #define PDBDLYBL_DELAYB4_MASK       16
    #define PDBDLYBL_DELAYB5_MASK       32
    #define PDBDLYBL_DELAYB6_MASK       64
    #define PDBDLYBL_DELAYB7_MASK       128
    
  } Overlap_STR;

  struct {
    word DELAYB0     :1;                                       /* Delay B, bit 0 */
    word DELAYB1     :1;                                       /* Delay B, bit 1 */
    word DELAYB2     :1;                                       /* Delay B, bit 2 */
    word DELAYB3     :1;                                       /* Delay B, bit 3 */
    word DELAYB4     :1;                                       /* Delay B, bit 4 */
    word DELAYB5     :1;                                       /* Delay B, bit 5 */
    word DELAYB6     :1;                                       /* Delay B, bit 6 */
    word DELAYB7     :1;                                       /* Delay B, bit 7 */
    word DELAYB8     :1;                                       /* Delay B, bit 8 */
    word DELAYB9     :1;                                       /* Delay B, bit 9 */
    word DELAYB10    :1;                                       /* Delay B, bit 10 */
    word DELAYB11    :1;                                       /* Delay B, bit 11 */
    word DELAYB12    :1;                                       /* Delay B, bit 12 */
    word DELAYB13    :1;                                       /* Delay B, bit 13 */
    word DELAYB14    :1;                                       /* Delay B, bit 14 */
    word DELAYB15    :1;                                       /* Delay B, bit 15 */
  } Bits;
} PDBDLYBSTR;
extern volatile PDBDLYBSTR _PDBDLYB @0xFFFF9864;
#define PDBDLYB                         _PDBDLYB.Word
#define PDBDLYB_DELAYB0                 _PDBDLYB.Bits.DELAYB0
#define PDBDLYB_DELAYB1                 _PDBDLYB.Bits.DELAYB1
#define PDBDLYB_DELAYB2                 _PDBDLYB.Bits.DELAYB2
#define PDBDLYB_DELAYB3                 _PDBDLYB.Bits.DELAYB3
#define PDBDLYB_DELAYB4                 _PDBDLYB.Bits.DELAYB4
#define PDBDLYB_DELAYB5                 _PDBDLYB.Bits.DELAYB5
#define PDBDLYB_DELAYB6                 _PDBDLYB.Bits.DELAYB6
#define PDBDLYB_DELAYB7                 _PDBDLYB.Bits.DELAYB7
#define PDBDLYB_DELAYB8                 _PDBDLYB.Bits.DELAYB8
#define PDBDLYB_DELAYB9                 _PDBDLYB.Bits.DELAYB9
#define PDBDLYB_DELAYB10                _PDBDLYB.Bits.DELAYB10
#define PDBDLYB_DELAYB11                _PDBDLYB.Bits.DELAYB11
#define PDBDLYB_DELAYB12                _PDBDLYB.Bits.DELAYB12
#define PDBDLYB_DELAYB13                _PDBDLYB.Bits.DELAYB13
#define PDBDLYB_DELAYB14                _PDBDLYB.Bits.DELAYB14
#define PDBDLYB_DELAYB15                _PDBDLYB.Bits.DELAYB15

#define PDBDLYB_DELAYB0_MASK            1
#define PDBDLYB_DELAYB1_MASK            2
#define PDBDLYB_DELAYB2_MASK            4
#define PDBDLYB_DELAYB3_MASK            8
#define PDBDLYB_DELAYB4_MASK            16
#define PDBDLYB_DELAYB5_MASK            32
#define PDBDLYB_DELAYB6_MASK            64
#define PDBDLYB_DELAYB7_MASK            128
#define PDBDLYB_DELAYB8_MASK            256
#define PDBDLYB_DELAYB9_MASK            512
#define PDBDLYB_DELAYB10_MASK           1024
#define PDBDLYB_DELAYB11_MASK           2048
#define PDBDLYB_DELAYB12_MASK           4096
#define PDBDLYB_DELAYB13_MASK           8192
#define PDBDLYB_DELAYB14_MASK           16384
#define PDBDLYB_DELAYB15_MASK           32768


/*** PDBMOD - PDB Counter Modulus Register; 0xFFFF9866 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** PDBMODH - PDB Counter Modulus High Register; 0xFFFF9866 ***/
    union {
      byte Byte;
      struct {
        byte MOD8        :1;                                       /* Counter Modulus, bit 8 */
        byte MOD9        :1;                                       /* Counter Modulus, bit 9 */
        byte MOD10       :1;                                       /* Counter Modulus, bit 10 */
        byte MOD11       :1;                                       /* Counter Modulus, bit 11 */
        byte MOD12       :1;                                       /* Counter Modulus, bit 12 */
        byte MOD13       :1;                                       /* Counter Modulus, bit 13 */
        byte MOD14       :1;                                       /* Counter Modulus, bit 14 */
        byte MOD15       :1;                                       /* Counter Modulus, bit 15 */
      } Bits;
    } PDBMODHSTR;
    #define PDBMODH                     _PDBMOD.Overlap_STR.PDBMODHSTR.Byte
    #define PDBMODH_MOD8                _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD8
    #define PDBMODH_MOD9                _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD9
    #define PDBMODH_MOD10               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD10
    #define PDBMODH_MOD11               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD11
    #define PDBMODH_MOD12               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD12
    #define PDBMODH_MOD13               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD13
    #define PDBMODH_MOD14               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD14
    #define PDBMODH_MOD15               _PDBMOD.Overlap_STR.PDBMODHSTR.Bits.MOD15
    
    #define PDBMODH_MOD8_MASK           1
    #define PDBMODH_MOD9_MASK           2
    #define PDBMODH_MOD10_MASK          4
    #define PDBMODH_MOD11_MASK          8
    #define PDBMODH_MOD12_MASK          16
    #define PDBMODH_MOD13_MASK          32
    #define PDBMODH_MOD14_MASK          64
    #define PDBMODH_MOD15_MASK          128
    

    /*** PDBMODL - PDB Counter Modulus Low Register; 0xFFFF9867 ***/
    union {
      byte Byte;
      struct {
        byte MOD0        :1;                                       /* Counter Modulus, bit 0 */
        byte MOD1        :1;                                       /* Counter Modulus, bit 1 */
        byte MOD2        :1;                                       /* Counter Modulus, bit 2 */
        byte MOD3        :1;                                       /* Counter Modulus, bit 3 */
        byte MOD4        :1;                                       /* Counter Modulus, bit 4 */
        byte MOD5        :1;                                       /* Counter Modulus, bit 5 */
        byte MOD6        :1;                                       /* Counter Modulus, bit 6 */
        byte MOD7        :1;                                       /* Counter Modulus, bit 7 */
      } Bits;
    } PDBMODLSTR;
    #define PDBMODL                     _PDBMOD.Overlap_STR.PDBMODLSTR.Byte
    #define PDBMODL_MOD0                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD0
    #define PDBMODL_MOD1                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD1
    #define PDBMODL_MOD2                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD2
    #define PDBMODL_MOD3                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD3
    #define PDBMODL_MOD4                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD4
    #define PDBMODL_MOD5                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD5
    #define PDBMODL_MOD6                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD6
    #define PDBMODL_MOD7                _PDBMOD.Overlap_STR.PDBMODLSTR.Bits.MOD7
    
    #define PDBMODL_MOD0_MASK           1
    #define PDBMODL_MOD1_MASK           2
    #define PDBMODL_MOD2_MASK           4
    #define PDBMODL_MOD3_MASK           8
    #define PDBMODL_MOD4_MASK           16
    #define PDBMODL_MOD5_MASK           32
    #define PDBMODL_MOD6_MASK           64
    #define PDBMODL_MOD7_MASK           128
    
  } Overlap_STR;

  struct {
    word MOD0        :1;                                       /* Counter Modulus, bit 0 */
    word MOD1        :1;                                       /* Counter Modulus, bit 1 */
    word MOD2        :1;                                       /* Counter Modulus, bit 2 */
    word MOD3        :1;                                       /* Counter Modulus, bit 3 */
    word MOD4        :1;                                       /* Counter Modulus, bit 4 */
    word MOD5        :1;                                       /* Counter Modulus, bit 5 */
    word MOD6        :1;                                       /* Counter Modulus, bit 6 */
    word MOD7        :1;                                       /* Counter Modulus, bit 7 */
    word MOD8        :1;                                       /* Counter Modulus, bit 8 */
    word MOD9        :1;                                       /* Counter Modulus, bit 9 */
    word MOD10       :1;                                       /* Counter Modulus, bit 10 */
    word MOD11       :1;                                       /* Counter Modulus, bit 11 */
    word MOD12       :1;                                       /* Counter Modulus, bit 12 */
    word MOD13       :1;                                       /* Counter Modulus, bit 13 */
    word MOD14       :1;                                       /* Counter Modulus, bit 14 */
    word MOD15       :1;                                       /* Counter Modulus, bit 15 */
  } Bits;
} PDBMODSTR;
extern volatile PDBMODSTR _PDBMOD @0xFFFF9866;
#define PDBMOD                          _PDBMOD.Word
#define PDBMOD_MOD0                     _PDBMOD.Bits.MOD0
#define PDBMOD_MOD1                     _PDBMOD.Bits.MOD1
#define PDBMOD_MOD2                     _PDBMOD.Bits.MOD2
#define PDBMOD_MOD3                     _PDBMOD.Bits.MOD3
#define PDBMOD_MOD4                     _PDBMOD.Bits.MOD4
#define PDBMOD_MOD5                     _PDBMOD.Bits.MOD5
#define PDBMOD_MOD6                     _PDBMOD.Bits.MOD6
#define PDBMOD_MOD7                     _PDBMOD.Bits.MOD7
#define PDBMOD_MOD8                     _PDBMOD.Bits.MOD8
#define PDBMOD_MOD9                     _PDBMOD.Bits.MOD9
#define PDBMOD_MOD10                    _PDBMOD.Bits.MOD10
#define PDBMOD_MOD11                    _PDBMOD.Bits.MOD11
#define PDBMOD_MOD12                    _PDBMOD.Bits.MOD12
#define PDBMOD_MOD13                    _PDBMOD.Bits.MOD13
#define PDBMOD_MOD14                    _PDBMOD.Bits.MOD14
#define PDBMOD_MOD15                    _PDBMOD.Bits.MOD15

#define PDBMOD_MOD0_MASK                1
#define PDBMOD_MOD1_MASK                2
#define PDBMOD_MOD2_MASK                4
#define PDBMOD_MOD3_MASK                8
#define PDBMOD_MOD4_MASK                16
#define PDBMOD_MOD5_MASK                32
#define PDBMOD_MOD6_MASK                64
#define PDBMOD_MOD7_MASK                128
#define PDBMOD_MOD8_MASK                256
#define PDBMOD_MOD9_MASK                512
#define PDBMOD_MOD10_MASK               1024
#define PDBMOD_MOD11_MASK               2048
#define PDBMOD_MOD12_MASK               4096
#define PDBMOD_MOD13_MASK               8192
#define PDBMOD_MOD14_MASK               16384
#define PDBMOD_MOD15_MASK               32768


/*** PDBCNT - PDB Counter Value Register; 0xFFFF9868 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** PDBCNTH - PDB Counter Value High Register; 0xFFFF9868 ***/
    union {
      byte Byte;
      struct {
        byte COUNT8      :1;                                       /* Counter Value, bit 8 */
        byte COUNT9      :1;                                       /* Counter Value, bit 9 */
        byte COUNT10     :1;                                       /* Counter Value, bit 10 */
        byte COUNT11     :1;                                       /* Counter Value, bit 11 */
        byte COUNT12     :1;                                       /* Counter Value, bit 12 */
        byte COUNT13     :1;                                       /* Counter Value, bit 13 */
        byte COUNT14     :1;                                       /* Counter Value, bit 14 */
        byte COUNT15     :1;                                       /* Counter Value, bit 15 */
      } Bits;
    } PDBCNTHSTR;
    #define PDBCNTH                     _PDBCNT.Overlap_STR.PDBCNTHSTR.Byte
    #define PDBCNTH_COUNT8              _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT8
    #define PDBCNTH_COUNT9              _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT9
    #define PDBCNTH_COUNT10             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT10
    #define PDBCNTH_COUNT11             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT11
    #define PDBCNTH_COUNT12             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT12
    #define PDBCNTH_COUNT13             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT13
    #define PDBCNTH_COUNT14             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT14
    #define PDBCNTH_COUNT15             _PDBCNT.Overlap_STR.PDBCNTHSTR.Bits.COUNT15
    
    #define PDBCNTH_COUNT8_MASK         1
    #define PDBCNTH_COUNT9_MASK         2
    #define PDBCNTH_COUNT10_MASK        4
    #define PDBCNTH_COUNT11_MASK        8
    #define PDBCNTH_COUNT12_MASK        16
    #define PDBCNTH_COUNT13_MASK        32
    #define PDBCNTH_COUNT14_MASK        64
    #define PDBCNTH_COUNT15_MASK        128
    

    /*** PDBCNTL - PDB Counter Value Low Register; 0xFFFF9869 ***/
    union {
      byte Byte;
      struct {
        byte COUNT0      :1;                                       /* Counter Value, bit 0 */
        byte COUNT1      :1;                                       /* Counter Value, bit 1 */
        byte COUNT2      :1;                                       /* Counter Value, bit 2 */
        byte COUNT3      :1;                                       /* Counter Value, bit 3 */
        byte COUNT4      :1;                                       /* Counter Value, bit 4 */
        byte COUNT5      :1;                                       /* Counter Value, bit 5 */
        byte COUNT6      :1;                                       /* Counter Value, bit 6 */
        byte COUNT7      :1;                                       /* Counter Value, bit 7 */
      } Bits;
    } PDBCNTLSTR;
    #define PDBCNTL                     _PDBCNT.Overlap_STR.PDBCNTLSTR.Byte
    #define PDBCNTL_COUNT0              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT0
    #define PDBCNTL_COUNT1              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT1
    #define PDBCNTL_COUNT2              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT2
    #define PDBCNTL_COUNT3              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT3
    #define PDBCNTL_COUNT4              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT4
    #define PDBCNTL_COUNT5              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT5
    #define PDBCNTL_COUNT6              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT6
    #define PDBCNTL_COUNT7              _PDBCNT.Overlap_STR.PDBCNTLSTR.Bits.COUNT7
    
    #define PDBCNTL_COUNT0_MASK         1
    #define PDBCNTL_COUNT1_MASK         2
    #define PDBCNTL_COUNT2_MASK         4
    #define PDBCNTL_COUNT3_MASK         8
    #define PDBCNTL_COUNT4_MASK         16
    #define PDBCNTL_COUNT5_MASK         32
    #define PDBCNTL_COUNT6_MASK         64
    #define PDBCNTL_COUNT7_MASK         128
    
  } Overlap_STR;

  struct {
    word COUNT0      :1;                                       /* Counter Value, bit 0 */
    word COUNT1      :1;                                       /* Counter Value, bit 1 */
    word COUNT2      :1;                                       /* Counter Value, bit 2 */
    word COUNT3      :1;                                       /* Counter Value, bit 3 */
    word COUNT4      :1;                                       /* Counter Value, bit 4 */
    word COUNT5      :1;                                       /* Counter Value, bit 5 */
    word COUNT6      :1;                                       /* Counter Value, bit 6 */
    word COUNT7      :1;                                       /* Counter Value, bit 7 */
    word COUNT8      :1;                                       /* Counter Value, bit 8 */
    word COUNT9      :1;                                       /* Counter Value, bit 9 */
    word COUNT10     :1;                                       /* Counter Value, bit 10 */
    word COUNT11     :1;                                       /* Counter Value, bit 11 */
    word COUNT12     :1;                                       /* Counter Value, bit 12 */
    word COUNT13     :1;                                       /* Counter Value, bit 13 */
    word COUNT14     :1;                                       /* Counter Value, bit 14 */
    word COUNT15     :1;                                       /* Counter Value, bit 15 */
  } Bits;
} PDBCNTSTR;
extern volatile PDBCNTSTR _PDBCNT @0xFFFF9868;
#define PDBCNT                          _PDBCNT.Word
#define PDBCNT_COUNT0                   _PDBCNT.Bits.COUNT0
#define PDBCNT_COUNT1                   _PDBCNT.Bits.COUNT1
#define PDBCNT_COUNT2                   _PDBCNT.Bits.COUNT2
#define PDBCNT_COUNT3                   _PDBCNT.Bits.COUNT3
#define PDBCNT_COUNT4                   _PDBCNT.Bits.COUNT4
#define PDBCNT_COUNT5                   _PDBCNT.Bits.COUNT5
#define PDBCNT_COUNT6                   _PDBCNT.Bits.COUNT6
#define PDBCNT_COUNT7                   _PDBCNT.Bits.COUNT7
#define PDBCNT_COUNT8                   _PDBCNT.Bits.COUNT8
#define PDBCNT_COUNT9                   _PDBCNT.Bits.COUNT9
#define PDBCNT_COUNT10                  _PDBCNT.Bits.COUNT10
#define PDBCNT_COUNT11                  _PDBCNT.Bits.COUNT11
#define PDBCNT_COUNT12                  _PDBCNT.Bits.COUNT12
#define PDBCNT_COUNT13                  _PDBCNT.Bits.COUNT13
#define PDBCNT_COUNT14                  _PDBCNT.Bits.COUNT14
#define PDBCNT_COUNT15                  _PDBCNT.Bits.COUNT15

#define PDBCNT_COUNT0_MASK              1
#define PDBCNT_COUNT1_MASK              2
#define PDBCNT_COUNT2_MASK              4
#define PDBCNT_COUNT3_MASK              8
#define PDBCNT_COUNT4_MASK              16
#define PDBCNT_COUNT5_MASK              32
#define PDBCNT_COUNT6_MASK              64
#define PDBCNT_COUNT7_MASK              128
#define PDBCNT_COUNT8_MASK              256
#define PDBCNT_COUNT9_MASK              512
#define PDBCNT_COUNT10_MASK             1024
#define PDBCNT_COUNT11_MASK             2048
#define PDBCNT_COUNT12_MASK             4096
#define PDBCNT_COUNT13_MASK             8192
#define PDBCNT_COUNT14_MASK             16384
#define PDBCNT_COUNT15_MASK             32768


/*** PDBSCR - PDB Status Register; 0xFFFF986A ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte DAF         :1;                                       /* Delay A flag */
    byte             :1; 
    byte DBF         :1;                                       /* Delay B flag */
    byte             :1; 
    byte COF         :1;                                       /* Counter overflow flag */
    byte PDBEN       :1;                                       /* PDB module enable */
    byte             :1; 
  } Bits;
} PDBSCRSTR;
extern volatile PDBSCRSTR _PDBSCR @0xFFFF986A;
#define PDBSCR                          _PDBSCR.Byte
#define PDBSCR_DAF                      _PDBSCR.Bits.DAF
#define PDBSCR_DBF                      _PDBSCR.Bits.DBF
#define PDBSCR_COF                      _PDBSCR.Bits.COF
#define PDBSCR_PDBEN                    _PDBSCR.Bits.PDBEN

#define PDBSCR_DAF_MASK                 2
#define PDBSCR_DBF_MASK                 8
#define PDBSCR_COF_MASK                 32
#define PDBSCR_PDBEN_MASK               64


/*** WDOG_ST_CTRL - Watchdog Status and Control Register; 0xFFFF9880 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** WDOG_ST_CTRL_H - Watchdog Status and Control Register High; 0xFFFF9880 ***/
    union {
      word Word;
      struct {
        word WDOG_EN     :1;                                       /* Watchdog Enable */
        word CLK_SRC     :1;                                       /* Clock Source Select */
        word IRQ_RST_EN  :1;                                       /* Interrupt Enable */
        word WIN_EN      :1;                                       /* Window mode */
        word ALLOW_UPDATE :1;                                      /* Write once registers update */
        word DBG_EN      :1;                                       /* Debug mode enable */
        word STOP_EN     :1;                                       /* Stop mode enable */
        word WAIT_EN     :1;                                       /* Wait mode enable */
        word             :1; 
        word             :1; 
        word TESTWDOG    :1;                                       /* Test watchdog */
        word TESTSEL     :1;                                       /* Test Selection */
        word BYTESEL0    :1;                                       /* Byte Test Selection, bit 0 */
        word BYTESEL1    :1;                                       /* Byte Test Selection, bit 1 */
        word DISABLE_TESTWDOG :1;                                  /* Test Mode Disable */
        word             :1; 
      } Bits;
      struct {
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word     :1;
        word grpBYTESEL :2;
        word     :1;
        word     :1;
      } MergedBits;
    } WDOG_ST_CTRL_HSTR;
    #define WDOG_ST_CTRL_H              _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Word
    #define WDOG_ST_CTRL_H_WDOG_EN      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.WDOG_EN
    #define WDOG_ST_CTRL_H_CLK_SRC      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.CLK_SRC
    #define WDOG_ST_CTRL_H_IRQ_RST_EN   _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.IRQ_RST_EN
    #define WDOG_ST_CTRL_H_WIN_EN       _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.WIN_EN
    #define WDOG_ST_CTRL_H_ALLOW_UPDATE _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.ALLOW_UPDATE
    #define WDOG_ST_CTRL_H_DBG_EN       _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.DBG_EN
    #define WDOG_ST_CTRL_H_STOP_EN      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.STOP_EN
    #define WDOG_ST_CTRL_H_WAIT_EN      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.WAIT_EN
    #define WDOG_ST_CTRL_H_TESTWDOG     _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.TESTWDOG
    #define WDOG_ST_CTRL_H_TESTSEL      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.TESTSEL
    #define WDOG_ST_CTRL_H_BYTESEL0     _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.BYTESEL0
    #define WDOG_ST_CTRL_H_BYTESEL1     _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.BYTESEL1
    #define WDOG_ST_CTRL_H_DISABLE_TESTWDOG _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.Bits.DISABLE_TESTWDOG
    #define WDOG_ST_CTRL_H_BYTESEL      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_HSTR.MergedBits.grpBYTESEL
    
    #define WDOG_ST_CTRL_H_WDOG_EN_MASK 1
    #define WDOG_ST_CTRL_H_CLK_SRC_MASK 2
    #define WDOG_ST_CTRL_H_IRQ_RST_EN_MASK 4
    #define WDOG_ST_CTRL_H_WIN_EN_MASK  8
    #define WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK 16
    #define WDOG_ST_CTRL_H_DBG_EN_MASK  32
    #define WDOG_ST_CTRL_H_STOP_EN_MASK 64
    #define WDOG_ST_CTRL_H_WAIT_EN_MASK 128
    #define WDOG_ST_CTRL_H_TESTWDOG_MASK 1024
    #define WDOG_ST_CTRL_H_TESTSEL_MASK 2048
    #define WDOG_ST_CTRL_H_BYTESEL0_MASK 4096
    #define WDOG_ST_CTRL_H_BYTESEL1_MASK 8192
    #define WDOG_ST_CTRL_H_DISABLE_TESTWDOG_MASK 16384
    #define WDOG_ST_CTRL_H_BYTESEL_MASK 12288
    #define WDOG_ST_CTRL_H_BYTESEL_BITNUM 12
    

    /*** WDOG_ST_CTRL_L - Watchdog Status and Control Register Low; 0xFFFF9882 ***/
    union {
      word Word;
      struct {
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word             :1; 
        word INT_FLG     :1;                                       /* Interrupt Flag */
      } Bits;
    } WDOG_ST_CTRL_LSTR;
    #define WDOG_ST_CTRL_L              _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_LSTR.Word
    #define WDOG_ST_CTRL_L_INT_FLG      _WDOG_ST_CTRL.Overlap_STR.WDOG_ST_CTRL_LSTR.Bits.INT_FLG
    
    #define WDOG_ST_CTRL_L_INT_FLG_MASK 32768
    
  } Overlap_STR;

  struct {
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword INT_FLG    :1;                                       /* Interrupt Flag */
    dword WDOG_EN    :1;                                       /* Watchdog Enable */
    dword CLK_SRC    :1;                                       /* Clock Source Select */
    dword IRQ_RST_EN :1;                                       /* Interrupt Enable */
    dword WIN_EN     :1;                                       /* Window mode */
    dword ALLOW_UPDATE :1;                                     /* Write once registers update */
    dword DBG_EN     :1;                                       /* Debug mode enable */
    dword STOP_EN    :1;                                       /* Stop mode enable */
    dword WAIT_EN    :1;                                       /* Wait mode enable */
    dword            :1; 
    dword            :1; 
    dword TESTWDOG   :1;                                       /* Test watchdog */
    dword TESTSEL    :1;                                       /* Test Selection */
    dword BYTESEL0   :1;                                       /* Byte Test Selection, bit 0 */
    dword BYTESEL1   :1;                                       /* Byte Test Selection, bit 1 */
    dword DISABLE_TESTWDOG :1;                                 /* Test Mode Disable */
    dword            :1; 
  } Bits;
  struct {
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpBYTESEL :2;
    dword        :1;
    dword        :1;
  } MergedBits;
} WDOG_ST_CTRLSTR;
extern volatile WDOG_ST_CTRLSTR _WDOG_ST_CTRL @0xFFFF9880;
#define WDOG_ST_CTRL                    _WDOG_ST_CTRL.Dword
#define WDOG_ST_CTRL_INT_FLG            _WDOG_ST_CTRL.Bits.INT_FLG
#define WDOG_ST_CTRL_WDOG_EN            _WDOG_ST_CTRL.Bits.WDOG_EN
#define WDOG_ST_CTRL_CLK_SRC            _WDOG_ST_CTRL.Bits.CLK_SRC
#define WDOG_ST_CTRL_IRQ_RST_EN         _WDOG_ST_CTRL.Bits.IRQ_RST_EN
#define WDOG_ST_CTRL_WIN_EN             _WDOG_ST_CTRL.Bits.WIN_EN
#define WDOG_ST_CTRL_ALLOW_UPDATE       _WDOG_ST_CTRL.Bits.ALLOW_UPDATE
#define WDOG_ST_CTRL_DBG_EN             _WDOG_ST_CTRL.Bits.DBG_EN
#define WDOG_ST_CTRL_STOP_EN            _WDOG_ST_CTRL.Bits.STOP_EN
#define WDOG_ST_CTRL_WAIT_EN            _WDOG_ST_CTRL.Bits.WAIT_EN
#define WDOG_ST_CTRL_TESTWDOG           _WDOG_ST_CTRL.Bits.TESTWDOG
#define WDOG_ST_CTRL_TESTSEL            _WDOG_ST_CTRL.Bits.TESTSEL
#define WDOG_ST_CTRL_BYTESEL0           _WDOG_ST_CTRL.Bits.BYTESEL0
#define WDOG_ST_CTRL_BYTESEL1           _WDOG_ST_CTRL.Bits.BYTESEL1
#define WDOG_ST_CTRL_DISABLE_TESTWDOG   _WDOG_ST_CTRL.Bits.DISABLE_TESTWDOG
#define WDOG_ST_CTRL_BYTESEL            _WDOG_ST_CTRL.MergedBits.grpBYTESEL

#define WDOG_ST_CTRL_INT_FLG_MASK       32768
#define WDOG_ST_CTRL_WDOG_EN_MASK       65536
#define WDOG_ST_CTRL_CLK_SRC_MASK       131072
#define WDOG_ST_CTRL_IRQ_RST_EN_MASK    262144
#define WDOG_ST_CTRL_WIN_EN_MASK        524288
#define WDOG_ST_CTRL_ALLOW_UPDATE_MASK  1048576
#define WDOG_ST_CTRL_DBG_EN_MASK        2097152
#define WDOG_ST_CTRL_STOP_EN_MASK       4194304
#define WDOG_ST_CTRL_WAIT_EN_MASK       8388608
#define WDOG_ST_CTRL_TESTWDOG_MASK      67108864
#define WDOG_ST_CTRL_TESTSEL_MASK       134217728
#define WDOG_ST_CTRL_BYTESEL0_MASK      268435456
#define WDOG_ST_CTRL_BYTESEL1_MASK      536870912
#define WDOG_ST_CTRL_DISABLE_TESTWDOG_MASK 1073741824
#define WDOG_ST_CTRL_BYTESEL_MASK       805306368
#define WDOG_ST_CTRL_BYTESEL_BITNUM     28


/*** WDOG_TO_VAL - Watchdog Control and Status Register; 0xFFFF9884 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** WDOG_TO_VAL_H - Watchdog Control and Status Register High; 0xFFFF9884 ***/
    union {
      word Word;
    } WDOG_TO_VAL_HSTR;
    #define WDOG_TO_VAL_H               _WDOG_TO_VAL.Overlap_STR.WDOG_TO_VAL_HSTR.Word
    

    /*** WDOG_TO_VAL_L - Watchdog Control and Status Register Low; 0xFFFF9886 ***/
    union {
      word Word;
    } WDOG_TO_VAL_LSTR;
    #define WDOG_TO_VAL_L               _WDOG_TO_VAL.Overlap_STR.WDOG_TO_VAL_LSTR.Word
    
  } Overlap_STR;

  struct {
    dword TO_VAL_LOW :16;                                      /* Timeout Value Low */
    dword TO_VAL_HIGH :16;                                     /* Timeout Value High */
  } Bits;
} WDOG_TO_VALSTR;
extern volatile WDOG_TO_VALSTR _WDOG_TO_VAL @0xFFFF9884;
#define WDOG_TO_VAL                     _WDOG_TO_VAL.Dword
#define WDOG_TO_VAL_TO_VAL_LOW          _WDOG_TO_VAL.Bits.TO_VAL_LOW
#define WDOG_TO_VAL_TO_VAL_HIGH         _WDOG_TO_VAL.Bits.TO_VAL_HIGH

#define WDOG_TO_VAL_TO_VAL_LOW_MASK     65535
#define WDOG_TO_VAL_TO_VAL_LOW_BITNUM   0
#define WDOG_TO_VAL_TO_VAL_HIGH_MASK    4294901760
#define WDOG_TO_VAL_TO_VAL_HIGH_BITNUM  16


/*** WDOG_WIN - Watchdog Window Register; 0xFFFF9888 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** WDOG_WIN_H - Watchdog Window Register High; 0xFFFF9888 ***/
    union {
      word Word;
    } WDOG_WIN_HSTR;
    #define WDOG_WIN_H                  _WDOG_WIN.Overlap_STR.WDOG_WIN_HSTR.Word
    

    /*** WDOG_WIN_L - Watchdog Window Register Low; 0xFFFF988A ***/
    union {
      word Word;
    } WDOG_WIN_LSTR;
    #define WDOG_WIN_L                  _WDOG_WIN.Overlap_STR.WDOG_WIN_LSTR.Word
    
  } Overlap_STR;

  struct {
    dword WIN_LOW    :16;                                      /* Window Low */
    dword WIN_HIGH   :16;                                      /* Window High */
  } Bits;
} WDOG_WINSTR;
extern volatile WDOG_WINSTR _WDOG_WIN @0xFFFF9888;
#define WDOG_WIN                        _WDOG_WIN.Dword
#define WDOG_WIN_WIN_LOW                _WDOG_WIN.Bits.WIN_LOW
#define WDOG_WIN_WIN_HIGH               _WDOG_WIN.Bits.WIN_HIGH

#define WDOG_WIN_WIN_LOW_MASK           65535
#define WDOG_WIN_WIN_LOW_BITNUM         0
#define WDOG_WIN_WIN_HIGH_MASK          4294901760
#define WDOG_WIN_WIN_HIGH_BITNUM        16


/*** WDOG_REFRESH - Watchdog Refresh Register; 0xFFFF988C ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** WDOG_REFRESH_H - Watchdog Refresh High Register; 0xFFFF988C ***/
    union {
      byte Byte;
    } WDOG_REFRESH_HSTR;
    #define WDOG_REFRESH_H              _WDOG_REFRESH.Overlap_STR.WDOG_REFRESH_HSTR.Byte
    

    /*** WDOG_REFRESH_L - Watchdog Refresh Low Register; 0xFFFF988D ***/
    union {
      byte Byte;
    } WDOG_REFRESH_LSTR;
    #define WDOG_REFRESH_L              _WDOG_REFRESH.Overlap_STR.WDOG_REFRESH_LSTR.Byte
    
  } Overlap_STR;

} WDOG_REFRESHSTR;
extern volatile WDOG_REFRESHSTR _WDOG_REFRESH @0xFFFF988C;
#define WDOG_REFRESH                    _WDOG_REFRESH.Word


/*** WDOG_UNLOCK - Watchdog Unlock Register; 0xFFFF988E ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** WDOG_UNLOCK_H - Watchdog Unlock High Register; 0xFFFF988E ***/
    union {
      byte Byte;
    } WDOG_UNLOCK_HSTR;
    #define WDOG_UNLOCK_H               _WDOG_UNLOCK.Overlap_STR.WDOG_UNLOCK_HSTR.Byte
    

    /*** WDOG_UNLOCK_L - Watchdog Unlock Low Register; 0xFFFF988F ***/
    union {
      byte Byte;
    } WDOG_UNLOCK_LSTR;
    #define WDOG_UNLOCK_L               _WDOG_UNLOCK.Overlap_STR.WDOG_UNLOCK_LSTR.Byte
    
  } Overlap_STR;

} WDOG_UNLOCKSTR;
extern volatile WDOG_UNLOCKSTR _WDOG_UNLOCK @0xFFFF988E;
#define WDOG_UNLOCK                     _WDOG_UNLOCK.Word


/*** WDOG_TIMER_OUT - Watchdog Timer Output Register; 0xFFFF9890 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** WDOG_TIMER_OUT_H - Watchdog Timer Output Register High; 0xFFFF9890 ***/
    union {
      word Word;
    } WDOG_TIMER_OUT_HSTR;
    #define WDOG_TIMER_OUT_H            _WDOG_TIMER_OUT.Overlap_STR.WDOG_TIMER_OUT_HSTR.Word
    

    /*** WDOG_TIMER_OUT_L - Watchdog Timer Output Register Low; 0xFFFF9892 ***/
    union {
      word Word;
    } WDOG_TIMER_OUT_LSTR;
    #define WDOG_TIMER_OUT_L            _WDOG_TIMER_OUT.Overlap_STR.WDOG_TIMER_OUT_LSTR.Word
    
  } Overlap_STR;

  struct {
    dword TIMER_OUT_LOW :16;                                   /* Timer Output Low */
    dword TIMER_OUT_HIGH :16;                                  /* Timer Output High */
  } Bits;
} WDOG_TIMER_OUTSTR;
extern volatile WDOG_TIMER_OUTSTR _WDOG_TIMER_OUT @0xFFFF9890;
#define WDOG_TIMER_OUT                  _WDOG_TIMER_OUT.Dword
#define WDOG_TIMER_OUT_TIMER_OUT_LOW    _WDOG_TIMER_OUT.Bits.TIMER_OUT_LOW
#define WDOG_TIMER_OUT_TIMER_OUT_HIGH   _WDOG_TIMER_OUT.Bits.TIMER_OUT_HIGH

#define WDOG_TIMER_OUT_TIMER_OUT_LOW_MASK 65535
#define WDOG_TIMER_OUT_TIMER_OUT_LOW_BITNUM 0
#define WDOG_TIMER_OUT_TIMER_OUT_HIGH_MASK 4294901760
#define WDOG_TIMER_OUT_TIMER_OUT_HIGH_BITNUM 16


/*** WDOG_RST_CNT - Watchdog Reset Count Register; 0xFFFF9894 ***/
typedef union {
  word Word;
} WDOG_RST_CNTSTR;
extern volatile WDOG_RST_CNTSTR _WDOG_RST_CNT @0xFFFF9894;
#define WDOG_RST_CNT                    _WDOG_RST_CNT.Word


/*** IEVENT_DR0 - iEvent Data Register 0; 0xFFFF9A00 ***/
typedef union {
  byte Byte;
  struct {
    byte In_A        :1;                                       /* Event Input A */
    byte In_B        :1;                                       /* Event Input B */
    byte In_C        :1;                                       /* Event Input C */
    byte In_D        :1;                                       /* Event Input D */
    byte Output_FSM0 :1;                                       /* Output Finite State Machine, bit 0 */
    byte Output_FSM1 :1;                                       /* Output Finite State Machine, bit 1 */
    byte Output_FSM2 :1;                                       /* Output Finite State Machine, bit 2 */
    byte Ev_Out      :1;                                       /* Event Output. This bit represents the state of the configured event output */
  } Bits;
  struct {
    byte grpIn_x :4;
    byte grpOutput_FSM :3;
    byte         :1;
  } MergedBits;
} IEVENT_DR0STR;
extern volatile IEVENT_DR0STR _IEVENT_DR0 @0xFFFF9A00;
#define IEVENT_DR0                      _IEVENT_DR0.Byte
#define IEVENT_DR0_In_A                 _IEVENT_DR0.Bits.In_A
#define IEVENT_DR0_In_B                 _IEVENT_DR0.Bits.In_B
#define IEVENT_DR0_In_C                 _IEVENT_DR0.Bits.In_C
#define IEVENT_DR0_In_D                 _IEVENT_DR0.Bits.In_D
#define IEVENT_DR0_Output_FSM0          _IEVENT_DR0.Bits.Output_FSM0
#define IEVENT_DR0_Output_FSM1          _IEVENT_DR0.Bits.Output_FSM1
#define IEVENT_DR0_Output_FSM2          _IEVENT_DR0.Bits.Output_FSM2
#define IEVENT_DR0_Ev_Out               _IEVENT_DR0.Bits.Ev_Out
/* IEVENT_DR_ARR: Access 4 IEVENT_DRx registers in an array */
#define IEVENT_DR_ARR                   ((volatile byte *) &IEVENT_DR0)
#define IEVENT_DR0_In_x                 _IEVENT_DR0.MergedBits.grpIn_x
#define IEVENT_DR0_Output_FSM           _IEVENT_DR0.MergedBits.grpOutput_FSM

#define IEVENT_DR0_In_A_MASK            1
#define IEVENT_DR0_In_B_MASK            2
#define IEVENT_DR0_In_C_MASK            4
#define IEVENT_DR0_In_D_MASK            8
#define IEVENT_DR0_Output_FSM0_MASK     16
#define IEVENT_DR0_Output_FSM1_MASK     32
#define IEVENT_DR0_Output_FSM2_MASK     64
#define IEVENT_DR0_Ev_Out_MASK          128
#define IEVENT_DR0_In_x_MASK            15
#define IEVENT_DR0_In_x_BITNUM          0
#define IEVENT_DR0_Output_FSM_MASK      112
#define IEVENT_DR0_Output_FSM_BITNUM    4


/*** IEVENT_DR1 - iEvent Data Register 1; 0xFFFF9A01 ***/
typedef union {
  byte Byte;
  struct {
    byte In_A        :1;                                       /* Event Input A */
    byte In_B        :1;                                       /* Event Input B */
    byte In_C        :1;                                       /* Event Input C */
    byte In_D        :1;                                       /* Event Input D */
    byte Output_FSM0 :1;                                       /* Output Finite State Machine, bit 0 */
    byte Output_FSM1 :1;                                       /* Output Finite State Machine, bit 1 */
    byte Output_FSM2 :1;                                       /* Output Finite State Machine, bit 2 */
    byte Ev_Out      :1;                                       /* Event Output. This bit represents the state of the configured event output */
  } Bits;
  struct {
    byte grpIn_x :4;
    byte grpOutput_FSM :3;
    byte         :1;
  } MergedBits;
} IEVENT_DR1STR;
extern volatile IEVENT_DR1STR _IEVENT_DR1 @0xFFFF9A01;
#define IEVENT_DR1                      _IEVENT_DR1.Byte
#define IEVENT_DR1_In_A                 _IEVENT_DR1.Bits.In_A
#define IEVENT_DR1_In_B                 _IEVENT_DR1.Bits.In_B
#define IEVENT_DR1_In_C                 _IEVENT_DR1.Bits.In_C
#define IEVENT_DR1_In_D                 _IEVENT_DR1.Bits.In_D
#define IEVENT_DR1_Output_FSM0          _IEVENT_DR1.Bits.Output_FSM0
#define IEVENT_DR1_Output_FSM1          _IEVENT_DR1.Bits.Output_FSM1
#define IEVENT_DR1_Output_FSM2          _IEVENT_DR1.Bits.Output_FSM2
#define IEVENT_DR1_Ev_Out               _IEVENT_DR1.Bits.Ev_Out
#define IEVENT_DR1_In_x                 _IEVENT_DR1.MergedBits.grpIn_x
#define IEVENT_DR1_Output_FSM           _IEVENT_DR1.MergedBits.grpOutput_FSM

#define IEVENT_DR1_In_A_MASK            1
#define IEVENT_DR1_In_B_MASK            2
#define IEVENT_DR1_In_C_MASK            4
#define IEVENT_DR1_In_D_MASK            8
#define IEVENT_DR1_Output_FSM0_MASK     16
#define IEVENT_DR1_Output_FSM1_MASK     32
#define IEVENT_DR1_Output_FSM2_MASK     64
#define IEVENT_DR1_Ev_Out_MASK          128
#define IEVENT_DR1_In_x_MASK            15
#define IEVENT_DR1_In_x_BITNUM          0
#define IEVENT_DR1_Output_FSM_MASK      112
#define IEVENT_DR1_Output_FSM_BITNUM    4


/*** IEVENT_DR2 - iEvent Data Register 2; 0xFFFF9A02 ***/
typedef union {
  byte Byte;
  struct {
    byte In_A        :1;                                       /* Event Input A */
    byte In_B        :1;                                       /* Event Input B */
    byte In_C        :1;                                       /* Event Input C */
    byte In_D        :1;                                       /* Event Input D */
    byte Output_FSM0 :1;                                       /* Output Finite State Machine, bit 0 */
    byte Output_FSM1 :1;                                       /* Output Finite State Machine, bit 1 */
    byte Output_FSM2 :1;                                       /* Output Finite State Machine, bit 2 */
    byte Ev_Out      :1;                                       /* Event Output. This bit represents the state of the configured event output */
  } Bits;
  struct {
    byte grpIn_x :4;
    byte grpOutput_FSM :3;
    byte         :1;
  } MergedBits;
} IEVENT_DR2STR;
extern volatile IEVENT_DR2STR _IEVENT_DR2 @0xFFFF9A02;
#define IEVENT_DR2                      _IEVENT_DR2.Byte
#define IEVENT_DR2_In_A                 _IEVENT_DR2.Bits.In_A
#define IEVENT_DR2_In_B                 _IEVENT_DR2.Bits.In_B
#define IEVENT_DR2_In_C                 _IEVENT_DR2.Bits.In_C
#define IEVENT_DR2_In_D                 _IEVENT_DR2.Bits.In_D
#define IEVENT_DR2_Output_FSM0          _IEVENT_DR2.Bits.Output_FSM0
#define IEVENT_DR2_Output_FSM1          _IEVENT_DR2.Bits.Output_FSM1
#define IEVENT_DR2_Output_FSM2          _IEVENT_DR2.Bits.Output_FSM2
#define IEVENT_DR2_Ev_Out               _IEVENT_DR2.Bits.Ev_Out
#define IEVENT_DR2_In_x                 _IEVENT_DR2.MergedBits.grpIn_x
#define IEVENT_DR2_Output_FSM           _IEVENT_DR2.MergedBits.grpOutput_FSM

#define IEVENT_DR2_In_A_MASK            1
#define IEVENT_DR2_In_B_MASK            2
#define IEVENT_DR2_In_C_MASK            4
#define IEVENT_DR2_In_D_MASK            8
#define IEVENT_DR2_Output_FSM0_MASK     16
#define IEVENT_DR2_Output_FSM1_MASK     32
#define IEVENT_DR2_Output_FSM2_MASK     64
#define IEVENT_DR2_Ev_Out_MASK          128
#define IEVENT_DR2_In_x_MASK            15
#define IEVENT_DR2_In_x_BITNUM          0
#define IEVENT_DR2_Output_FSM_MASK      112
#define IEVENT_DR2_Output_FSM_BITNUM    4


/*** IEVENT_DR3 - iEvent Data Register 3; 0xFFFF9A03 ***/
typedef union {
  byte Byte;
  struct {
    byte In_A        :1;                                       /* Event Input A */
    byte In_B        :1;                                       /* Event Input B */
    byte In_C        :1;                                       /* Event Input C */
    byte In_D        :1;                                       /* Event Input D */
    byte Output_FSM0 :1;                                       /* Output Finite State Machine, bit 0 */
    byte Output_FSM1 :1;                                       /* Output Finite State Machine, bit 1 */
    byte Output_FSM2 :1;                                       /* Output Finite State Machine, bit 2 */
    byte Ev_Out      :1;                                       /* Event Output. This bit represents the state of the configured event output */
  } Bits;
  struct {
    byte grpIn_x :4;
    byte grpOutput_FSM :3;
    byte         :1;
  } MergedBits;
} IEVENT_DR3STR;
extern volatile IEVENT_DR3STR _IEVENT_DR3 @0xFFFF9A03;
#define IEVENT_DR3                      _IEVENT_DR3.Byte
#define IEVENT_DR3_In_A                 _IEVENT_DR3.Bits.In_A
#define IEVENT_DR3_In_B                 _IEVENT_DR3.Bits.In_B
#define IEVENT_DR3_In_C                 _IEVENT_DR3.Bits.In_C
#define IEVENT_DR3_In_D                 _IEVENT_DR3.Bits.In_D
#define IEVENT_DR3_Output_FSM0          _IEVENT_DR3.Bits.Output_FSM0
#define IEVENT_DR3_Output_FSM1          _IEVENT_DR3.Bits.Output_FSM1
#define IEVENT_DR3_Output_FSM2          _IEVENT_DR3.Bits.Output_FSM2
#define IEVENT_DR3_Ev_Out               _IEVENT_DR3.Bits.Ev_Out
#define IEVENT_DR3_In_x                 _IEVENT_DR3.MergedBits.grpIn_x
#define IEVENT_DR3_Output_FSM           _IEVENT_DR3.MergedBits.grpOutput_FSM

#define IEVENT_DR3_In_A_MASK            1
#define IEVENT_DR3_In_B_MASK            2
#define IEVENT_DR3_In_C_MASK            4
#define IEVENT_DR3_In_D_MASK            8
#define IEVENT_DR3_Output_FSM0_MASK     16
#define IEVENT_DR3_Output_FSM1_MASK     32
#define IEVENT_DR3_Output_FSM2_MASK     64
#define IEVENT_DR3_Ev_Out_MASK          128
#define IEVENT_DR3_In_x_MASK            15
#define IEVENT_DR3_In_x_BITNUM          0
#define IEVENT_DR3_Output_FSM_MASK      112
#define IEVENT_DR3_Output_FSM_BITNUM    4


/*** IEVENT_CR0 - iEvent Control Register 0; 0xFFFF9A80 ***/
typedef union {
  byte Byte;
  struct {
    byte Type0       :1;                                       /* Output Channel Type, bit 0 */
    byte Type1       :1;                                       /* Output Channel Type, bit 1 */
    byte OSE         :1;                                       /* One-Shot Enable */
    byte DDB         :1;                                       /* Disable Done Broadcast */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RO          :1;                                       /* Read-Only. This bit determines if the configuration registers can be written or are read-only */
  } Bits;
  struct {
    byte grpType :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IEVENT_CR0STR;
extern volatile IEVENT_CR0STR _IEVENT_CR0 @0xFFFF9A80;
#define IEVENT_CR0                      _IEVENT_CR0.Byte
#define IEVENT_CR0_Type0                _IEVENT_CR0.Bits.Type0
#define IEVENT_CR0_Type1                _IEVENT_CR0.Bits.Type1
#define IEVENT_CR0_OSE                  _IEVENT_CR0.Bits.OSE
#define IEVENT_CR0_DDB                  _IEVENT_CR0.Bits.DDB
#define IEVENT_CR0_RO                   _IEVENT_CR0.Bits.RO
/* IEVENT_CR_ARR: Access 4 IEVENT_CRx registers in an array */
#define IEVENT_CR_ARR                   ((volatile byte *) &IEVENT_CR0)
#define IEVENT_CR0_Type                 _IEVENT_CR0.MergedBits.grpType

#define IEVENT_CR0_Type0_MASK           1
#define IEVENT_CR0_Type1_MASK           2
#define IEVENT_CR0_OSE_MASK             4
#define IEVENT_CR0_DDB_MASK             8
#define IEVENT_CR0_RO_MASK              128
#define IEVENT_CR0_Type_MASK            3
#define IEVENT_CR0_Type_BITNUM          0


/*** IEVENT_CR1 - iEvent Control Register 1; 0xFFFF9A81 ***/
typedef union {
  byte Byte;
  struct {
    byte Type0       :1;                                       /* Output Channel Type, bit 0 */
    byte Type1       :1;                                       /* Output Channel Type, bit 1 */
    byte OSE         :1;                                       /* One-Shot Enable */
    byte DDB         :1;                                       /* Disable Done Broadcast */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RO          :1;                                       /* Read-Only. This bit determines if the configuration registers can be written or are read-only */
  } Bits;
  struct {
    byte grpType :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IEVENT_CR1STR;
extern volatile IEVENT_CR1STR _IEVENT_CR1 @0xFFFF9A81;
#define IEVENT_CR1                      _IEVENT_CR1.Byte
#define IEVENT_CR1_Type0                _IEVENT_CR1.Bits.Type0
#define IEVENT_CR1_Type1                _IEVENT_CR1.Bits.Type1
#define IEVENT_CR1_OSE                  _IEVENT_CR1.Bits.OSE
#define IEVENT_CR1_DDB                  _IEVENT_CR1.Bits.DDB
#define IEVENT_CR1_RO                   _IEVENT_CR1.Bits.RO
#define IEVENT_CR1_Type                 _IEVENT_CR1.MergedBits.grpType

#define IEVENT_CR1_Type0_MASK           1
#define IEVENT_CR1_Type1_MASK           2
#define IEVENT_CR1_OSE_MASK             4
#define IEVENT_CR1_DDB_MASK             8
#define IEVENT_CR1_RO_MASK              128
#define IEVENT_CR1_Type_MASK            3
#define IEVENT_CR1_Type_BITNUM          0


/*** IEVENT_CR2 - iEvent Control Register 2; 0xFFFF9A82 ***/
typedef union {
  byte Byte;
  struct {
    byte Type0       :1;                                       /* Output Channel Type, bit 0 */
    byte Type1       :1;                                       /* Output Channel Type, bit 1 */
    byte OSE         :1;                                       /* One-Shot Enable */
    byte DDB         :1;                                       /* Disable Done Broadcast */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RO          :1;                                       /* Read-Only. This bit determines if the configuration registers can be written or are read-only */
  } Bits;
  struct {
    byte grpType :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IEVENT_CR2STR;
extern volatile IEVENT_CR2STR _IEVENT_CR2 @0xFFFF9A82;
#define IEVENT_CR2                      _IEVENT_CR2.Byte
#define IEVENT_CR2_Type0                _IEVENT_CR2.Bits.Type0
#define IEVENT_CR2_Type1                _IEVENT_CR2.Bits.Type1
#define IEVENT_CR2_OSE                  _IEVENT_CR2.Bits.OSE
#define IEVENT_CR2_DDB                  _IEVENT_CR2.Bits.DDB
#define IEVENT_CR2_RO                   _IEVENT_CR2.Bits.RO
#define IEVENT_CR2_Type                 _IEVENT_CR2.MergedBits.grpType

#define IEVENT_CR2_Type0_MASK           1
#define IEVENT_CR2_Type1_MASK           2
#define IEVENT_CR2_OSE_MASK             4
#define IEVENT_CR2_DDB_MASK             8
#define IEVENT_CR2_RO_MASK              128
#define IEVENT_CR2_Type_MASK            3
#define IEVENT_CR2_Type_BITNUM          0


/*** IEVENT_CR3 - iEvent Control Register 3; 0xFFFF9A83 ***/
typedef union {
  byte Byte;
  struct {
    byte Type0       :1;                                       /* Output Channel Type, bit 0 */
    byte Type1       :1;                                       /* Output Channel Type, bit 1 */
    byte OSE         :1;                                       /* One-Shot Enable */
    byte DDB         :1;                                       /* Disable Done Broadcast */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte RO          :1;                                       /* Read-Only. This bit determines if the configuration registers can be written or are read-only */
  } Bits;
  struct {
    byte grpType :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IEVENT_CR3STR;
extern volatile IEVENT_CR3STR _IEVENT_CR3 @0xFFFF9A83;
#define IEVENT_CR3                      _IEVENT_CR3.Byte
#define IEVENT_CR3_Type0                _IEVENT_CR3.Bits.Type0
#define IEVENT_CR3_Type1                _IEVENT_CR3.Bits.Type1
#define IEVENT_CR3_OSE                  _IEVENT_CR3.Bits.OSE
#define IEVENT_CR3_DDB                  _IEVENT_CR3.Bits.DDB
#define IEVENT_CR3_RO                   _IEVENT_CR3.Bits.RO
#define IEVENT_CR3_Type                 _IEVENT_CR3.MergedBits.grpType

#define IEVENT_CR3_Type0_MASK           1
#define IEVENT_CR3_Type1_MASK           2
#define IEVENT_CR3_OSE_MASK             4
#define IEVENT_CR3_DDB_MASK             8
#define IEVENT_CR3_RO_MASK              128
#define IEVENT_CR3_Type_MASK            3
#define IEVENT_CR3_Type_BITNUM          0


/*** IEVENT_IMXCR0 - iEvent Input Mux Configuration Register 0; 0xFFFF9B00 ***/
typedef union {
  dword Dword;
  struct {
    dword D_Select0  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 0 */
    dword D_Select1  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 1 */
    dword D_Select2  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 2 */
    dword D_Select3  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword C_Select0  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 0 */
    dword C_Select1  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 1 */
    dword C_Select2  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 2 */
    dword C_Select3  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword B_Select0  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 0 */
    dword B_Select1  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 1 */
    dword B_Select2  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 2 */
    dword B_Select3  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword A_Select0  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 0 */
    dword A_Select1  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 1 */
    dword A_Select2  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 2 */
    dword A_Select3  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
  struct {
    dword grpD_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpC_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpB_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpA_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
  } MergedBits;
} IEVENT_IMXCR0STR;
extern volatile IEVENT_IMXCR0STR _IEVENT_IMXCR0 @0xFFFF9B00;
#define IEVENT_IMXCR0                   _IEVENT_IMXCR0.Dword
#define IEVENT_IMXCR0_D_Select0         _IEVENT_IMXCR0.Bits.D_Select0
#define IEVENT_IMXCR0_D_Select1         _IEVENT_IMXCR0.Bits.D_Select1
#define IEVENT_IMXCR0_D_Select2         _IEVENT_IMXCR0.Bits.D_Select2
#define IEVENT_IMXCR0_D_Select3         _IEVENT_IMXCR0.Bits.D_Select3
#define IEVENT_IMXCR0_C_Select0         _IEVENT_IMXCR0.Bits.C_Select0
#define IEVENT_IMXCR0_C_Select1         _IEVENT_IMXCR0.Bits.C_Select1
#define IEVENT_IMXCR0_C_Select2         _IEVENT_IMXCR0.Bits.C_Select2
#define IEVENT_IMXCR0_C_Select3         _IEVENT_IMXCR0.Bits.C_Select3
#define IEVENT_IMXCR0_B_Select0         _IEVENT_IMXCR0.Bits.B_Select0
#define IEVENT_IMXCR0_B_Select1         _IEVENT_IMXCR0.Bits.B_Select1
#define IEVENT_IMXCR0_B_Select2         _IEVENT_IMXCR0.Bits.B_Select2
#define IEVENT_IMXCR0_B_Select3         _IEVENT_IMXCR0.Bits.B_Select3
#define IEVENT_IMXCR0_A_Select0         _IEVENT_IMXCR0.Bits.A_Select0
#define IEVENT_IMXCR0_A_Select1         _IEVENT_IMXCR0.Bits.A_Select1
#define IEVENT_IMXCR0_A_Select2         _IEVENT_IMXCR0.Bits.A_Select2
#define IEVENT_IMXCR0_A_Select3         _IEVENT_IMXCR0.Bits.A_Select3
#define IEVENT_IMXCR0_D_Select          _IEVENT_IMXCR0.MergedBits.grpD_Select
#define IEVENT_IMXCR0_C_Select          _IEVENT_IMXCR0.MergedBits.grpC_Select
#define IEVENT_IMXCR0_B_Select          _IEVENT_IMXCR0.MergedBits.grpB_Select
#define IEVENT_IMXCR0_A_Select          _IEVENT_IMXCR0.MergedBits.grpA_Select

#define IEVENT_IMXCR0_D_Select0_MASK    1
#define IEVENT_IMXCR0_D_Select1_MASK    2
#define IEVENT_IMXCR0_D_Select2_MASK    4
#define IEVENT_IMXCR0_D_Select3_MASK    8
#define IEVENT_IMXCR0_C_Select0_MASK    256
#define IEVENT_IMXCR0_C_Select1_MASK    512
#define IEVENT_IMXCR0_C_Select2_MASK    1024
#define IEVENT_IMXCR0_C_Select3_MASK    2048
#define IEVENT_IMXCR0_B_Select0_MASK    65536
#define IEVENT_IMXCR0_B_Select1_MASK    131072
#define IEVENT_IMXCR0_B_Select2_MASK    262144
#define IEVENT_IMXCR0_B_Select3_MASK    524288
#define IEVENT_IMXCR0_A_Select0_MASK    16777216
#define IEVENT_IMXCR0_A_Select1_MASK    33554432
#define IEVENT_IMXCR0_A_Select2_MASK    67108864
#define IEVENT_IMXCR0_A_Select3_MASK    134217728
#define IEVENT_IMXCR0_D_Select_MASK     15
#define IEVENT_IMXCR0_D_Select_BITNUM   0
#define IEVENT_IMXCR0_C_Select_MASK     3840
#define IEVENT_IMXCR0_C_Select_BITNUM   8
#define IEVENT_IMXCR0_B_Select_MASK     983040
#define IEVENT_IMXCR0_B_Select_BITNUM   16
#define IEVENT_IMXCR0_A_Select_MASK     251658240
#define IEVENT_IMXCR0_A_Select_BITNUM   24


/*** IEVENT_BFECR0 - iEvent Boolean Function Evaluation Configuration Register 0; 0xFFFF9B04 ***/
typedef union {
  dword Dword;
  struct {
    dword PT3_DC0    :1;                                       /* Product Term 3, D input Configuration, bit 0 */
    dword PT3_DC1    :1;                                       /* Product Term 3, D input Configuration, bit 1 */
    dword PT3_CC0    :1;                                       /* Product Term 3, C input Configuration, bit 0 */
    dword PT3_CC1    :1;                                       /* Product Term 3, C input Configuration, bit 1 */
    dword PT3_BC0    :1;                                       /* Product Term 3, B input Configuration, bit 0 */
    dword PT3_BC1    :1;                                       /* Product Term 3, B input Configuration, bit 1 */
    dword PT3_AC0    :1;                                       /* Product Term 3, A input Configuration, bit 0 */
    dword PT3_AC1    :1;                                       /* Product Term 3, A input Configuration, bit 1 */
    dword PT2_DC0    :1;                                       /* Product Term 2, D input Configuration, bit 0 */
    dword PT2_DC1    :1;                                       /* Product Term 2, D input Configuration, bit 1 */
    dword PT2_CC0    :1;                                       /* Product Term 2, C input Configuration, bit 0 */
    dword PT2_CC1    :1;                                       /* Product Term 2, C input Configuration, bit 1 */
    dword PT2_BC0    :1;                                       /* Product Term 2, B input Configuration, bit 0 */
    dword PT2_BC1    :1;                                       /* Product Term 2, B input Configuration, bit 1 */
    dword PT2_AC0    :1;                                       /* Product Term 2, A input Configuration, bit 0 */
    dword PT2_AC1    :1;                                       /* Product Term 2, A input Configuration, bit 1 */
    dword PT1_DC0    :1;                                       /* Product Term 1, D input Configuration, bit 0 */
    dword PT1_DC1    :1;                                       /* Product Term 1, D input Configuration, bit 1 */
    dword PT1_CC0    :1;                                       /* Product Term 1, C input Configuration, bit 0 */
    dword PT1_CC1    :1;                                       /* Product Term 1, C input Configuration, bit 1 */
    dword PT1_BC0    :1;                                       /* Product Term 1, B input Configuration, bit 0 */
    dword PT1_BC1    :1;                                       /* Product Term 1, B input Configuration, bit 1 */
    dword PT1_AC0    :1;                                       /* Product Term 1, A input Configuration, bit 0 */
    dword PT1_AC1    :1;                                       /* Product Term 1, A input Configuration, bit 1 */
    dword PT0_DC0    :1;                                       /* Product Term 0, D input Configuration, bit 0 */
    dword PT0_DC1    :1;                                       /* Product Term 0, D input Configuration, bit 1 */
    dword PT0_CC0    :1;                                       /* Product Term 0, C input Configuration, bit 0 */
    dword PT0_CC1    :1;                                       /* Product Term 0, C input Configuration, bit 1 */
    dword PT0_BC0    :1;                                       /* Product Term 0, B input Configuration, bit 0 */
    dword PT0_BC1    :1;                                       /* Product Term 0, B input Configuration, bit 1 */
    dword PT0_AC0    :1;                                       /* Product Term 0, A input Configuration, bit 0 */
    dword PT0_AC1    :1;                                       /* Product Term 0, A input Configuration, bit 1 */
  } Bits;
  struct {
    dword grpPT3_DC :2;
    dword grpPT3_CC :2;
    dword grpPT3_BC :2;
    dword grpPT3_AC :2;
    dword grpPT2_DC :2;
    dword grpPT2_CC :2;
    dword grpPT2_BC :2;
    dword grpPT2_AC :2;
    dword grpPT1_DC :2;
    dword grpPT1_CC :2;
    dword grpPT1_BC :2;
    dword grpPT1_AC :2;
    dword grpPT0_DC :2;
    dword grpPT0_CC :2;
    dword grpPT0_BC :2;
    dword grpPT0_AC :2;
  } MergedBits;
} IEVENT_BFECR0STR;
extern volatile IEVENT_BFECR0STR _IEVENT_BFECR0 @0xFFFF9B04;
#define IEVENT_BFECR0                   _IEVENT_BFECR0.Dword
#define IEVENT_BFECR0_PT3_DC0           _IEVENT_BFECR0.Bits.PT3_DC0
#define IEVENT_BFECR0_PT3_DC1           _IEVENT_BFECR0.Bits.PT3_DC1
#define IEVENT_BFECR0_PT3_CC0           _IEVENT_BFECR0.Bits.PT3_CC0
#define IEVENT_BFECR0_PT3_CC1           _IEVENT_BFECR0.Bits.PT3_CC1
#define IEVENT_BFECR0_PT3_BC0           _IEVENT_BFECR0.Bits.PT3_BC0
#define IEVENT_BFECR0_PT3_BC1           _IEVENT_BFECR0.Bits.PT3_BC1
#define IEVENT_BFECR0_PT3_AC0           _IEVENT_BFECR0.Bits.PT3_AC0
#define IEVENT_BFECR0_PT3_AC1           _IEVENT_BFECR0.Bits.PT3_AC1
#define IEVENT_BFECR0_PT2_DC0           _IEVENT_BFECR0.Bits.PT2_DC0
#define IEVENT_BFECR0_PT2_DC1           _IEVENT_BFECR0.Bits.PT2_DC1
#define IEVENT_BFECR0_PT2_CC0           _IEVENT_BFECR0.Bits.PT2_CC0
#define IEVENT_BFECR0_PT2_CC1           _IEVENT_BFECR0.Bits.PT2_CC1
#define IEVENT_BFECR0_PT2_BC0           _IEVENT_BFECR0.Bits.PT2_BC0
#define IEVENT_BFECR0_PT2_BC1           _IEVENT_BFECR0.Bits.PT2_BC1
#define IEVENT_BFECR0_PT2_AC0           _IEVENT_BFECR0.Bits.PT2_AC0
#define IEVENT_BFECR0_PT2_AC1           _IEVENT_BFECR0.Bits.PT2_AC1
#define IEVENT_BFECR0_PT1_DC0           _IEVENT_BFECR0.Bits.PT1_DC0
#define IEVENT_BFECR0_PT1_DC1           _IEVENT_BFECR0.Bits.PT1_DC1
#define IEVENT_BFECR0_PT1_CC0           _IEVENT_BFECR0.Bits.PT1_CC0
#define IEVENT_BFECR0_PT1_CC1           _IEVENT_BFECR0.Bits.PT1_CC1
#define IEVENT_BFECR0_PT1_BC0           _IEVENT_BFECR0.Bits.PT1_BC0
#define IEVENT_BFECR0_PT1_BC1           _IEVENT_BFECR0.Bits.PT1_BC1
#define IEVENT_BFECR0_PT1_AC0           _IEVENT_BFECR0.Bits.PT1_AC0
#define IEVENT_BFECR0_PT1_AC1           _IEVENT_BFECR0.Bits.PT1_AC1
#define IEVENT_BFECR0_PT0_DC0           _IEVENT_BFECR0.Bits.PT0_DC0
#define IEVENT_BFECR0_PT0_DC1           _IEVENT_BFECR0.Bits.PT0_DC1
#define IEVENT_BFECR0_PT0_CC0           _IEVENT_BFECR0.Bits.PT0_CC0
#define IEVENT_BFECR0_PT0_CC1           _IEVENT_BFECR0.Bits.PT0_CC1
#define IEVENT_BFECR0_PT0_BC0           _IEVENT_BFECR0.Bits.PT0_BC0
#define IEVENT_BFECR0_PT0_BC1           _IEVENT_BFECR0.Bits.PT0_BC1
#define IEVENT_BFECR0_PT0_AC0           _IEVENT_BFECR0.Bits.PT0_AC0
#define IEVENT_BFECR0_PT0_AC1           _IEVENT_BFECR0.Bits.PT0_AC1
#define IEVENT_BFECR0_PT3_DC            _IEVENT_BFECR0.MergedBits.grpPT3_DC
#define IEVENT_BFECR0_PT3_CC            _IEVENT_BFECR0.MergedBits.grpPT3_CC
#define IEVENT_BFECR0_PT3_BC            _IEVENT_BFECR0.MergedBits.grpPT3_BC
#define IEVENT_BFECR0_PT3_AC            _IEVENT_BFECR0.MergedBits.grpPT3_AC
#define IEVENT_BFECR0_PT2_DC            _IEVENT_BFECR0.MergedBits.grpPT2_DC
#define IEVENT_BFECR0_PT2_CC            _IEVENT_BFECR0.MergedBits.grpPT2_CC
#define IEVENT_BFECR0_PT2_BC            _IEVENT_BFECR0.MergedBits.grpPT2_BC
#define IEVENT_BFECR0_PT2_AC            _IEVENT_BFECR0.MergedBits.grpPT2_AC
#define IEVENT_BFECR0_PT1_DC            _IEVENT_BFECR0.MergedBits.grpPT1_DC
#define IEVENT_BFECR0_PT1_CC            _IEVENT_BFECR0.MergedBits.grpPT1_CC
#define IEVENT_BFECR0_PT1_BC            _IEVENT_BFECR0.MergedBits.grpPT1_BC
#define IEVENT_BFECR0_PT1_AC            _IEVENT_BFECR0.MergedBits.grpPT1_AC
#define IEVENT_BFECR0_PT0_DC            _IEVENT_BFECR0.MergedBits.grpPT0_DC
#define IEVENT_BFECR0_PT0_CC            _IEVENT_BFECR0.MergedBits.grpPT0_CC
#define IEVENT_BFECR0_PT0_BC            _IEVENT_BFECR0.MergedBits.grpPT0_BC
#define IEVENT_BFECR0_PT0_AC            _IEVENT_BFECR0.MergedBits.grpPT0_AC

#define IEVENT_BFECR0_PT3_DC0_MASK      1
#define IEVENT_BFECR0_PT3_DC1_MASK      2
#define IEVENT_BFECR0_PT3_CC0_MASK      4
#define IEVENT_BFECR0_PT3_CC1_MASK      8
#define IEVENT_BFECR0_PT3_BC0_MASK      16
#define IEVENT_BFECR0_PT3_BC1_MASK      32
#define IEVENT_BFECR0_PT3_AC0_MASK      64
#define IEVENT_BFECR0_PT3_AC1_MASK      128
#define IEVENT_BFECR0_PT2_DC0_MASK      256
#define IEVENT_BFECR0_PT2_DC1_MASK      512
#define IEVENT_BFECR0_PT2_CC0_MASK      1024
#define IEVENT_BFECR0_PT2_CC1_MASK      2048
#define IEVENT_BFECR0_PT2_BC0_MASK      4096
#define IEVENT_BFECR0_PT2_BC1_MASK      8192
#define IEVENT_BFECR0_PT2_AC0_MASK      16384
#define IEVENT_BFECR0_PT2_AC1_MASK      32768
#define IEVENT_BFECR0_PT1_DC0_MASK      65536
#define IEVENT_BFECR0_PT1_DC1_MASK      131072
#define IEVENT_BFECR0_PT1_CC0_MASK      262144
#define IEVENT_BFECR0_PT1_CC1_MASK      524288
#define IEVENT_BFECR0_PT1_BC0_MASK      1048576
#define IEVENT_BFECR0_PT1_BC1_MASK      2097152
#define IEVENT_BFECR0_PT1_AC0_MASK      4194304
#define IEVENT_BFECR0_PT1_AC1_MASK      8388608
#define IEVENT_BFECR0_PT0_DC0_MASK      16777216
#define IEVENT_BFECR0_PT0_DC1_MASK      33554432
#define IEVENT_BFECR0_PT0_CC0_MASK      67108864
#define IEVENT_BFECR0_PT0_CC1_MASK      134217728
#define IEVENT_BFECR0_PT0_BC0_MASK      268435456
#define IEVENT_BFECR0_PT0_BC1_MASK      536870912
#define IEVENT_BFECR0_PT0_AC0_MASK      1073741824
#define IEVENT_BFECR0_PT0_AC1_MASK      2147483648
#define IEVENT_BFECR0_PT3_DC_MASK       3
#define IEVENT_BFECR0_PT3_DC_BITNUM     0
#define IEVENT_BFECR0_PT3_CC_MASK       12
#define IEVENT_BFECR0_PT3_CC_BITNUM     2
#define IEVENT_BFECR0_PT3_BC_MASK       48
#define IEVENT_BFECR0_PT3_BC_BITNUM     4
#define IEVENT_BFECR0_PT3_AC_MASK       192
#define IEVENT_BFECR0_PT3_AC_BITNUM     6
#define IEVENT_BFECR0_PT2_DC_MASK       768
#define IEVENT_BFECR0_PT2_DC_BITNUM     8
#define IEVENT_BFECR0_PT2_CC_MASK       3072
#define IEVENT_BFECR0_PT2_CC_BITNUM     10
#define IEVENT_BFECR0_PT2_BC_MASK       12288
#define IEVENT_BFECR0_PT2_BC_BITNUM     12
#define IEVENT_BFECR0_PT2_AC_MASK       49152
#define IEVENT_BFECR0_PT2_AC_BITNUM     14
#define IEVENT_BFECR0_PT1_DC_MASK       196608
#define IEVENT_BFECR0_PT1_DC_BITNUM     16
#define IEVENT_BFECR0_PT1_CC_MASK       786432
#define IEVENT_BFECR0_PT1_CC_BITNUM     18
#define IEVENT_BFECR0_PT1_BC_MASK       3145728
#define IEVENT_BFECR0_PT1_BC_BITNUM     20
#define IEVENT_BFECR0_PT1_AC_MASK       12582912
#define IEVENT_BFECR0_PT1_AC_BITNUM     22
#define IEVENT_BFECR0_PT0_DC_MASK       50331648
#define IEVENT_BFECR0_PT0_DC_BITNUM     24
#define IEVENT_BFECR0_PT0_CC_MASK       201326592
#define IEVENT_BFECR0_PT0_CC_BITNUM     26
#define IEVENT_BFECR0_PT0_BC_MASK       805306368
#define IEVENT_BFECR0_PT0_BC_BITNUM     28
#define IEVENT_BFECR0_PT0_AC_MASK       3221225472
#define IEVENT_BFECR0_PT0_AC_BITNUM     30


/*** IEVENT_IMXCR1 - iEvent Input Mux Configuration Register 1; 0xFFFF9B08 ***/
typedef union {
  dword Dword;
  struct {
    dword D_Select0  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 0 */
    dword D_Select1  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 1 */
    dword D_Select2  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 2 */
    dword D_Select3  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword C_Select0  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 0 */
    dword C_Select1  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 1 */
    dword C_Select2  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 2 */
    dword C_Select3  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword B_Select0  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 0 */
    dword B_Select1  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 1 */
    dword B_Select2  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 2 */
    dword B_Select3  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword A_Select0  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 0 */
    dword A_Select1  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 1 */
    dword A_Select2  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 2 */
    dword A_Select3  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
  struct {
    dword grpD_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpC_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpB_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpA_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
  } MergedBits;
} IEVENT_IMXCR1STR;
extern volatile IEVENT_IMXCR1STR _IEVENT_IMXCR1 @0xFFFF9B08;
#define IEVENT_IMXCR1                   _IEVENT_IMXCR1.Dword
#define IEVENT_IMXCR1_D_Select0         _IEVENT_IMXCR1.Bits.D_Select0
#define IEVENT_IMXCR1_D_Select1         _IEVENT_IMXCR1.Bits.D_Select1
#define IEVENT_IMXCR1_D_Select2         _IEVENT_IMXCR1.Bits.D_Select2
#define IEVENT_IMXCR1_D_Select3         _IEVENT_IMXCR1.Bits.D_Select3
#define IEVENT_IMXCR1_C_Select0         _IEVENT_IMXCR1.Bits.C_Select0
#define IEVENT_IMXCR1_C_Select1         _IEVENT_IMXCR1.Bits.C_Select1
#define IEVENT_IMXCR1_C_Select2         _IEVENT_IMXCR1.Bits.C_Select2
#define IEVENT_IMXCR1_C_Select3         _IEVENT_IMXCR1.Bits.C_Select3
#define IEVENT_IMXCR1_B_Select0         _IEVENT_IMXCR1.Bits.B_Select0
#define IEVENT_IMXCR1_B_Select1         _IEVENT_IMXCR1.Bits.B_Select1
#define IEVENT_IMXCR1_B_Select2         _IEVENT_IMXCR1.Bits.B_Select2
#define IEVENT_IMXCR1_B_Select3         _IEVENT_IMXCR1.Bits.B_Select3
#define IEVENT_IMXCR1_A_Select0         _IEVENT_IMXCR1.Bits.A_Select0
#define IEVENT_IMXCR1_A_Select1         _IEVENT_IMXCR1.Bits.A_Select1
#define IEVENT_IMXCR1_A_Select2         _IEVENT_IMXCR1.Bits.A_Select2
#define IEVENT_IMXCR1_A_Select3         _IEVENT_IMXCR1.Bits.A_Select3
#define IEVENT_IMXCR1_D_Select          _IEVENT_IMXCR1.MergedBits.grpD_Select
#define IEVENT_IMXCR1_C_Select          _IEVENT_IMXCR1.MergedBits.grpC_Select
#define IEVENT_IMXCR1_B_Select          _IEVENT_IMXCR1.MergedBits.grpB_Select
#define IEVENT_IMXCR1_A_Select          _IEVENT_IMXCR1.MergedBits.grpA_Select

#define IEVENT_IMXCR1_D_Select0_MASK    1
#define IEVENT_IMXCR1_D_Select1_MASK    2
#define IEVENT_IMXCR1_D_Select2_MASK    4
#define IEVENT_IMXCR1_D_Select3_MASK    8
#define IEVENT_IMXCR1_C_Select0_MASK    256
#define IEVENT_IMXCR1_C_Select1_MASK    512
#define IEVENT_IMXCR1_C_Select2_MASK    1024
#define IEVENT_IMXCR1_C_Select3_MASK    2048
#define IEVENT_IMXCR1_B_Select0_MASK    65536
#define IEVENT_IMXCR1_B_Select1_MASK    131072
#define IEVENT_IMXCR1_B_Select2_MASK    262144
#define IEVENT_IMXCR1_B_Select3_MASK    524288
#define IEVENT_IMXCR1_A_Select0_MASK    16777216
#define IEVENT_IMXCR1_A_Select1_MASK    33554432
#define IEVENT_IMXCR1_A_Select2_MASK    67108864
#define IEVENT_IMXCR1_A_Select3_MASK    134217728
#define IEVENT_IMXCR1_D_Select_MASK     15
#define IEVENT_IMXCR1_D_Select_BITNUM   0
#define IEVENT_IMXCR1_C_Select_MASK     3840
#define IEVENT_IMXCR1_C_Select_BITNUM   8
#define IEVENT_IMXCR1_B_Select_MASK     983040
#define IEVENT_IMXCR1_B_Select_BITNUM   16
#define IEVENT_IMXCR1_A_Select_MASK     251658240
#define IEVENT_IMXCR1_A_Select_BITNUM   24


/*** IEVENT_BFECR1 - iEvent Boolean Function Evaluation Configuration Register 1; 0xFFFF9B0C ***/
typedef union {
  dword Dword;
  struct {
    dword PT3_DC0    :1;                                       /* Product Term 3, D input Configuration, bit 0 */
    dword PT3_DC1    :1;                                       /* Product Term 3, D input Configuration, bit 1 */
    dword PT3_CC0    :1;                                       /* Product Term 3, C input Configuration, bit 0 */
    dword PT3_CC1    :1;                                       /* Product Term 3, C input Configuration, bit 1 */
    dword PT3_BC0    :1;                                       /* Product Term 3, B input Configuration, bit 0 */
    dword PT3_BC1    :1;                                       /* Product Term 3, B input Configuration, bit 1 */
    dword PT3_AC0    :1;                                       /* Product Term 3, A input Configuration, bit 0 */
    dword PT3_AC1    :1;                                       /* Product Term 3, A input Configuration, bit 1 */
    dword PT2_DC0    :1;                                       /* Product Term 2, D input Configuration, bit 0 */
    dword PT2_DC1    :1;                                       /* Product Term 2, D input Configuration, bit 1 */
    dword PT2_CC0    :1;                                       /* Product Term 2, C input Configuration, bit 0 */
    dword PT2_CC1    :1;                                       /* Product Term 2, C input Configuration, bit 1 */
    dword PT2_BC0    :1;                                       /* Product Term 2, B input Configuration, bit 0 */
    dword PT2_BC1    :1;                                       /* Product Term 2, B input Configuration, bit 1 */
    dword PT2_AC0    :1;                                       /* Product Term 2, A input Configuration, bit 0 */
    dword PT2_AC1    :1;                                       /* Product Term 2, A input Configuration, bit 1 */
    dword PT1_DC0    :1;                                       /* Product Term 1, D input Configuration, bit 0 */
    dword PT1_DC1    :1;                                       /* Product Term 1, D input Configuration, bit 1 */
    dword PT1_CC0    :1;                                       /* Product Term 1, C input Configuration, bit 0 */
    dword PT1_CC1    :1;                                       /* Product Term 1, C input Configuration, bit 1 */
    dword PT1_BC0    :1;                                       /* Product Term 1, B input Configuration, bit 0 */
    dword PT1_BC1    :1;                                       /* Product Term 1, B input Configuration, bit 1 */
    dword PT1_AC0    :1;                                       /* Product Term 1, A input Configuration, bit 0 */
    dword PT1_AC1    :1;                                       /* Product Term 1, A input Configuration, bit 1 */
    dword PT0_DC0    :1;                                       /* Product Term 0, D input Configuration, bit 0 */
    dword PT0_DC1    :1;                                       /* Product Term 0, D input Configuration, bit 1 */
    dword PT0_CC0    :1;                                       /* Product Term 0, C input Configuration, bit 0 */
    dword PT0_CC1    :1;                                       /* Product Term 0, C input Configuration, bit 1 */
    dword PT0_BC0    :1;                                       /* Product Term 0, B input Configuration, bit 0 */
    dword PT0_BC1    :1;                                       /* Product Term 0, B input Configuration, bit 1 */
    dword PT0_AC0    :1;                                       /* Product Term 0, A input Configuration, bit 0 */
    dword PT0_AC1    :1;                                       /* Product Term 0, A input Configuration, bit 1 */
  } Bits;
  struct {
    dword grpPT3_DC :2;
    dword grpPT3_CC :2;
    dword grpPT3_BC :2;
    dword grpPT3_AC :2;
    dword grpPT2_DC :2;
    dword grpPT2_CC :2;
    dword grpPT2_BC :2;
    dword grpPT2_AC :2;
    dword grpPT1_DC :2;
    dword grpPT1_CC :2;
    dword grpPT1_BC :2;
    dword grpPT1_AC :2;
    dword grpPT0_DC :2;
    dword grpPT0_CC :2;
    dword grpPT0_BC :2;
    dword grpPT0_AC :2;
  } MergedBits;
} IEVENT_BFECR1STR;
extern volatile IEVENT_BFECR1STR _IEVENT_BFECR1 @0xFFFF9B0C;
#define IEVENT_BFECR1                   _IEVENT_BFECR1.Dword
#define IEVENT_BFECR1_PT3_DC0           _IEVENT_BFECR1.Bits.PT3_DC0
#define IEVENT_BFECR1_PT3_DC1           _IEVENT_BFECR1.Bits.PT3_DC1
#define IEVENT_BFECR1_PT3_CC0           _IEVENT_BFECR1.Bits.PT3_CC0
#define IEVENT_BFECR1_PT3_CC1           _IEVENT_BFECR1.Bits.PT3_CC1
#define IEVENT_BFECR1_PT3_BC0           _IEVENT_BFECR1.Bits.PT3_BC0
#define IEVENT_BFECR1_PT3_BC1           _IEVENT_BFECR1.Bits.PT3_BC1
#define IEVENT_BFECR1_PT3_AC0           _IEVENT_BFECR1.Bits.PT3_AC0
#define IEVENT_BFECR1_PT3_AC1           _IEVENT_BFECR1.Bits.PT3_AC1
#define IEVENT_BFECR1_PT2_DC0           _IEVENT_BFECR1.Bits.PT2_DC0
#define IEVENT_BFECR1_PT2_DC1           _IEVENT_BFECR1.Bits.PT2_DC1
#define IEVENT_BFECR1_PT2_CC0           _IEVENT_BFECR1.Bits.PT2_CC0
#define IEVENT_BFECR1_PT2_CC1           _IEVENT_BFECR1.Bits.PT2_CC1
#define IEVENT_BFECR1_PT2_BC0           _IEVENT_BFECR1.Bits.PT2_BC0
#define IEVENT_BFECR1_PT2_BC1           _IEVENT_BFECR1.Bits.PT2_BC1
#define IEVENT_BFECR1_PT2_AC0           _IEVENT_BFECR1.Bits.PT2_AC0
#define IEVENT_BFECR1_PT2_AC1           _IEVENT_BFECR1.Bits.PT2_AC1
#define IEVENT_BFECR1_PT1_DC0           _IEVENT_BFECR1.Bits.PT1_DC0
#define IEVENT_BFECR1_PT1_DC1           _IEVENT_BFECR1.Bits.PT1_DC1
#define IEVENT_BFECR1_PT1_CC0           _IEVENT_BFECR1.Bits.PT1_CC0
#define IEVENT_BFECR1_PT1_CC1           _IEVENT_BFECR1.Bits.PT1_CC1
#define IEVENT_BFECR1_PT1_BC0           _IEVENT_BFECR1.Bits.PT1_BC0
#define IEVENT_BFECR1_PT1_BC1           _IEVENT_BFECR1.Bits.PT1_BC1
#define IEVENT_BFECR1_PT1_AC0           _IEVENT_BFECR1.Bits.PT1_AC0
#define IEVENT_BFECR1_PT1_AC1           _IEVENT_BFECR1.Bits.PT1_AC1
#define IEVENT_BFECR1_PT0_DC0           _IEVENT_BFECR1.Bits.PT0_DC0
#define IEVENT_BFECR1_PT0_DC1           _IEVENT_BFECR1.Bits.PT0_DC1
#define IEVENT_BFECR1_PT0_CC0           _IEVENT_BFECR1.Bits.PT0_CC0
#define IEVENT_BFECR1_PT0_CC1           _IEVENT_BFECR1.Bits.PT0_CC1
#define IEVENT_BFECR1_PT0_BC0           _IEVENT_BFECR1.Bits.PT0_BC0
#define IEVENT_BFECR1_PT0_BC1           _IEVENT_BFECR1.Bits.PT0_BC1
#define IEVENT_BFECR1_PT0_AC0           _IEVENT_BFECR1.Bits.PT0_AC0
#define IEVENT_BFECR1_PT0_AC1           _IEVENT_BFECR1.Bits.PT0_AC1
#define IEVENT_BFECR1_PT3_DC            _IEVENT_BFECR1.MergedBits.grpPT3_DC
#define IEVENT_BFECR1_PT3_CC            _IEVENT_BFECR1.MergedBits.grpPT3_CC
#define IEVENT_BFECR1_PT3_BC            _IEVENT_BFECR1.MergedBits.grpPT3_BC
#define IEVENT_BFECR1_PT3_AC            _IEVENT_BFECR1.MergedBits.grpPT3_AC
#define IEVENT_BFECR1_PT2_DC            _IEVENT_BFECR1.MergedBits.grpPT2_DC
#define IEVENT_BFECR1_PT2_CC            _IEVENT_BFECR1.MergedBits.grpPT2_CC
#define IEVENT_BFECR1_PT2_BC            _IEVENT_BFECR1.MergedBits.grpPT2_BC
#define IEVENT_BFECR1_PT2_AC            _IEVENT_BFECR1.MergedBits.grpPT2_AC
#define IEVENT_BFECR1_PT1_DC            _IEVENT_BFECR1.MergedBits.grpPT1_DC
#define IEVENT_BFECR1_PT1_CC            _IEVENT_BFECR1.MergedBits.grpPT1_CC
#define IEVENT_BFECR1_PT1_BC            _IEVENT_BFECR1.MergedBits.grpPT1_BC
#define IEVENT_BFECR1_PT1_AC            _IEVENT_BFECR1.MergedBits.grpPT1_AC
#define IEVENT_BFECR1_PT0_DC            _IEVENT_BFECR1.MergedBits.grpPT0_DC
#define IEVENT_BFECR1_PT0_CC            _IEVENT_BFECR1.MergedBits.grpPT0_CC
#define IEVENT_BFECR1_PT0_BC            _IEVENT_BFECR1.MergedBits.grpPT0_BC
#define IEVENT_BFECR1_PT0_AC            _IEVENT_BFECR1.MergedBits.grpPT0_AC

#define IEVENT_BFECR1_PT3_DC0_MASK      1
#define IEVENT_BFECR1_PT3_DC1_MASK      2
#define IEVENT_BFECR1_PT3_CC0_MASK      4
#define IEVENT_BFECR1_PT3_CC1_MASK      8
#define IEVENT_BFECR1_PT3_BC0_MASK      16
#define IEVENT_BFECR1_PT3_BC1_MASK      32
#define IEVENT_BFECR1_PT3_AC0_MASK      64
#define IEVENT_BFECR1_PT3_AC1_MASK      128
#define IEVENT_BFECR1_PT2_DC0_MASK      256
#define IEVENT_BFECR1_PT2_DC1_MASK      512
#define IEVENT_BFECR1_PT2_CC0_MASK      1024
#define IEVENT_BFECR1_PT2_CC1_MASK      2048
#define IEVENT_BFECR1_PT2_BC0_MASK      4096
#define IEVENT_BFECR1_PT2_BC1_MASK      8192
#define IEVENT_BFECR1_PT2_AC0_MASK      16384
#define IEVENT_BFECR1_PT2_AC1_MASK      32768
#define IEVENT_BFECR1_PT1_DC0_MASK      65536
#define IEVENT_BFECR1_PT1_DC1_MASK      131072
#define IEVENT_BFECR1_PT1_CC0_MASK      262144
#define IEVENT_BFECR1_PT1_CC1_MASK      524288
#define IEVENT_BFECR1_PT1_BC0_MASK      1048576
#define IEVENT_BFECR1_PT1_BC1_MASK      2097152
#define IEVENT_BFECR1_PT1_AC0_MASK      4194304
#define IEVENT_BFECR1_PT1_AC1_MASK      8388608
#define IEVENT_BFECR1_PT0_DC0_MASK      16777216
#define IEVENT_BFECR1_PT0_DC1_MASK      33554432
#define IEVENT_BFECR1_PT0_CC0_MASK      67108864
#define IEVENT_BFECR1_PT0_CC1_MASK      134217728
#define IEVENT_BFECR1_PT0_BC0_MASK      268435456
#define IEVENT_BFECR1_PT0_BC1_MASK      536870912
#define IEVENT_BFECR1_PT0_AC0_MASK      1073741824
#define IEVENT_BFECR1_PT0_AC1_MASK      2147483648
#define IEVENT_BFECR1_PT3_DC_MASK       3
#define IEVENT_BFECR1_PT3_DC_BITNUM     0
#define IEVENT_BFECR1_PT3_CC_MASK       12
#define IEVENT_BFECR1_PT3_CC_BITNUM     2
#define IEVENT_BFECR1_PT3_BC_MASK       48
#define IEVENT_BFECR1_PT3_BC_BITNUM     4
#define IEVENT_BFECR1_PT3_AC_MASK       192
#define IEVENT_BFECR1_PT3_AC_BITNUM     6
#define IEVENT_BFECR1_PT2_DC_MASK       768
#define IEVENT_BFECR1_PT2_DC_BITNUM     8
#define IEVENT_BFECR1_PT2_CC_MASK       3072
#define IEVENT_BFECR1_PT2_CC_BITNUM     10
#define IEVENT_BFECR1_PT2_BC_MASK       12288
#define IEVENT_BFECR1_PT2_BC_BITNUM     12
#define IEVENT_BFECR1_PT2_AC_MASK       49152
#define IEVENT_BFECR1_PT2_AC_BITNUM     14
#define IEVENT_BFECR1_PT1_DC_MASK       196608
#define IEVENT_BFECR1_PT1_DC_BITNUM     16
#define IEVENT_BFECR1_PT1_CC_MASK       786432
#define IEVENT_BFECR1_PT1_CC_BITNUM     18
#define IEVENT_BFECR1_PT1_BC_MASK       3145728
#define IEVENT_BFECR1_PT1_BC_BITNUM     20
#define IEVENT_BFECR1_PT1_AC_MASK       12582912
#define IEVENT_BFECR1_PT1_AC_BITNUM     22
#define IEVENT_BFECR1_PT0_DC_MASK       50331648
#define IEVENT_BFECR1_PT0_DC_BITNUM     24
#define IEVENT_BFECR1_PT0_CC_MASK       201326592
#define IEVENT_BFECR1_PT0_CC_BITNUM     26
#define IEVENT_BFECR1_PT0_BC_MASK       805306368
#define IEVENT_BFECR1_PT0_BC_BITNUM     28
#define IEVENT_BFECR1_PT0_AC_MASK       3221225472
#define IEVENT_BFECR1_PT0_AC_BITNUM     30


/*** IEVENT_IMXCR2 - iEvent Input Mux Configuration Register 2; 0xFFFF9B10 ***/
typedef union {
  dword Dword;
  struct {
    dword D_Select0  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 0 */
    dword D_Select1  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 1 */
    dword D_Select2  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 2 */
    dword D_Select3  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword C_Select0  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 0 */
    dword C_Select1  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 1 */
    dword C_Select2  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 2 */
    dword C_Select3  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword B_Select0  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 0 */
    dword B_Select1  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 1 */
    dword B_Select2  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 2 */
    dword B_Select3  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword A_Select0  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 0 */
    dword A_Select1  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 1 */
    dword A_Select2  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 2 */
    dword A_Select3  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
  struct {
    dword grpD_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpC_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpB_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpA_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
  } MergedBits;
} IEVENT_IMXCR2STR;
extern volatile IEVENT_IMXCR2STR _IEVENT_IMXCR2 @0xFFFF9B10;
#define IEVENT_IMXCR2                   _IEVENT_IMXCR2.Dword
#define IEVENT_IMXCR2_D_Select0         _IEVENT_IMXCR2.Bits.D_Select0
#define IEVENT_IMXCR2_D_Select1         _IEVENT_IMXCR2.Bits.D_Select1
#define IEVENT_IMXCR2_D_Select2         _IEVENT_IMXCR2.Bits.D_Select2
#define IEVENT_IMXCR2_D_Select3         _IEVENT_IMXCR2.Bits.D_Select3
#define IEVENT_IMXCR2_C_Select0         _IEVENT_IMXCR2.Bits.C_Select0
#define IEVENT_IMXCR2_C_Select1         _IEVENT_IMXCR2.Bits.C_Select1
#define IEVENT_IMXCR2_C_Select2         _IEVENT_IMXCR2.Bits.C_Select2
#define IEVENT_IMXCR2_C_Select3         _IEVENT_IMXCR2.Bits.C_Select3
#define IEVENT_IMXCR2_B_Select0         _IEVENT_IMXCR2.Bits.B_Select0
#define IEVENT_IMXCR2_B_Select1         _IEVENT_IMXCR2.Bits.B_Select1
#define IEVENT_IMXCR2_B_Select2         _IEVENT_IMXCR2.Bits.B_Select2
#define IEVENT_IMXCR2_B_Select3         _IEVENT_IMXCR2.Bits.B_Select3
#define IEVENT_IMXCR2_A_Select0         _IEVENT_IMXCR2.Bits.A_Select0
#define IEVENT_IMXCR2_A_Select1         _IEVENT_IMXCR2.Bits.A_Select1
#define IEVENT_IMXCR2_A_Select2         _IEVENT_IMXCR2.Bits.A_Select2
#define IEVENT_IMXCR2_A_Select3         _IEVENT_IMXCR2.Bits.A_Select3
#define IEVENT_IMXCR2_D_Select          _IEVENT_IMXCR2.MergedBits.grpD_Select
#define IEVENT_IMXCR2_C_Select          _IEVENT_IMXCR2.MergedBits.grpC_Select
#define IEVENT_IMXCR2_B_Select          _IEVENT_IMXCR2.MergedBits.grpB_Select
#define IEVENT_IMXCR2_A_Select          _IEVENT_IMXCR2.MergedBits.grpA_Select

#define IEVENT_IMXCR2_D_Select0_MASK    1
#define IEVENT_IMXCR2_D_Select1_MASK    2
#define IEVENT_IMXCR2_D_Select2_MASK    4
#define IEVENT_IMXCR2_D_Select3_MASK    8
#define IEVENT_IMXCR2_C_Select0_MASK    256
#define IEVENT_IMXCR2_C_Select1_MASK    512
#define IEVENT_IMXCR2_C_Select2_MASK    1024
#define IEVENT_IMXCR2_C_Select3_MASK    2048
#define IEVENT_IMXCR2_B_Select0_MASK    65536
#define IEVENT_IMXCR2_B_Select1_MASK    131072
#define IEVENT_IMXCR2_B_Select2_MASK    262144
#define IEVENT_IMXCR2_B_Select3_MASK    524288
#define IEVENT_IMXCR2_A_Select0_MASK    16777216
#define IEVENT_IMXCR2_A_Select1_MASK    33554432
#define IEVENT_IMXCR2_A_Select2_MASK    67108864
#define IEVENT_IMXCR2_A_Select3_MASK    134217728
#define IEVENT_IMXCR2_D_Select_MASK     15
#define IEVENT_IMXCR2_D_Select_BITNUM   0
#define IEVENT_IMXCR2_C_Select_MASK     3840
#define IEVENT_IMXCR2_C_Select_BITNUM   8
#define IEVENT_IMXCR2_B_Select_MASK     983040
#define IEVENT_IMXCR2_B_Select_BITNUM   16
#define IEVENT_IMXCR2_A_Select_MASK     251658240
#define IEVENT_IMXCR2_A_Select_BITNUM   24


/*** IEVENT_BFECR2 - iEvent Boolean Function Evaluation Configuration Register 2; 0xFFFF9B14 ***/
typedef union {
  dword Dword;
  struct {
    dword PT3_DC0    :1;                                       /* Product Term 3, D input Configuration, bit 0 */
    dword PT3_DC1    :1;                                       /* Product Term 3, D input Configuration, bit 1 */
    dword PT3_CC0    :1;                                       /* Product Term 3, C input Configuration, bit 0 */
    dword PT3_CC1    :1;                                       /* Product Term 3, C input Configuration, bit 1 */
    dword PT3_BC0    :1;                                       /* Product Term 3, B input Configuration, bit 0 */
    dword PT3_BC1    :1;                                       /* Product Term 3, B input Configuration, bit 1 */
    dword PT3_AC0    :1;                                       /* Product Term 3, A input Configuration, bit 0 */
    dword PT3_AC1    :1;                                       /* Product Term 3, A input Configuration, bit 1 */
    dword PT2_DC0    :1;                                       /* Product Term 2, D input Configuration, bit 0 */
    dword PT2_DC1    :1;                                       /* Product Term 2, D input Configuration, bit 1 */
    dword PT2_CC0    :1;                                       /* Product Term 2, C input Configuration, bit 0 */
    dword PT2_CC1    :1;                                       /* Product Term 2, C input Configuration, bit 1 */
    dword PT2_BC0    :1;                                       /* Product Term 2, B input Configuration, bit 0 */
    dword PT2_BC1    :1;                                       /* Product Term 2, B input Configuration, bit 1 */
    dword PT2_AC0    :1;                                       /* Product Term 2, A input Configuration, bit 0 */
    dword PT2_AC1    :1;                                       /* Product Term 2, A input Configuration, bit 1 */
    dword PT1_DC0    :1;                                       /* Product Term 1, D input Configuration, bit 0 */
    dword PT1_DC1    :1;                                       /* Product Term 1, D input Configuration, bit 1 */
    dword PT1_CC0    :1;                                       /* Product Term 1, C input Configuration, bit 0 */
    dword PT1_CC1    :1;                                       /* Product Term 1, C input Configuration, bit 1 */
    dword PT1_BC0    :1;                                       /* Product Term 1, B input Configuration, bit 0 */
    dword PT1_BC1    :1;                                       /* Product Term 1, B input Configuration, bit 1 */
    dword PT1_AC0    :1;                                       /* Product Term 1, A input Configuration, bit 0 */
    dword PT1_AC1    :1;                                       /* Product Term 1, A input Configuration, bit 1 */
    dword PT0_DC0    :1;                                       /* Product Term 0, D input Configuration, bit 0 */
    dword PT0_DC1    :1;                                       /* Product Term 0, D input Configuration, bit 1 */
    dword PT0_CC0    :1;                                       /* Product Term 0, C input Configuration, bit 0 */
    dword PT0_CC1    :1;                                       /* Product Term 0, C input Configuration, bit 1 */
    dword PT0_BC0    :1;                                       /* Product Term 0, B input Configuration, bit 0 */
    dword PT0_BC1    :1;                                       /* Product Term 0, B input Configuration, bit 1 */
    dword PT0_AC0    :1;                                       /* Product Term 0, A input Configuration, bit 0 */
    dword PT0_AC1    :1;                                       /* Product Term 0, A input Configuration, bit 1 */
  } Bits;
  struct {
    dword grpPT3_DC :2;
    dword grpPT3_CC :2;
    dword grpPT3_BC :2;
    dword grpPT3_AC :2;
    dword grpPT2_DC :2;
    dword grpPT2_CC :2;
    dword grpPT2_BC :2;
    dword grpPT2_AC :2;
    dword grpPT1_DC :2;
    dword grpPT1_CC :2;
    dword grpPT1_BC :2;
    dword grpPT1_AC :2;
    dword grpPT0_DC :2;
    dword grpPT0_CC :2;
    dword grpPT0_BC :2;
    dword grpPT0_AC :2;
  } MergedBits;
} IEVENT_BFECR2STR;
extern volatile IEVENT_BFECR2STR _IEVENT_BFECR2 @0xFFFF9B14;
#define IEVENT_BFECR2                   _IEVENT_BFECR2.Dword
#define IEVENT_BFECR2_PT3_DC0           _IEVENT_BFECR2.Bits.PT3_DC0
#define IEVENT_BFECR2_PT3_DC1           _IEVENT_BFECR2.Bits.PT3_DC1
#define IEVENT_BFECR2_PT3_CC0           _IEVENT_BFECR2.Bits.PT3_CC0
#define IEVENT_BFECR2_PT3_CC1           _IEVENT_BFECR2.Bits.PT3_CC1
#define IEVENT_BFECR2_PT3_BC0           _IEVENT_BFECR2.Bits.PT3_BC0
#define IEVENT_BFECR2_PT3_BC1           _IEVENT_BFECR2.Bits.PT3_BC1
#define IEVENT_BFECR2_PT3_AC0           _IEVENT_BFECR2.Bits.PT3_AC0
#define IEVENT_BFECR2_PT3_AC1           _IEVENT_BFECR2.Bits.PT3_AC1
#define IEVENT_BFECR2_PT2_DC0           _IEVENT_BFECR2.Bits.PT2_DC0
#define IEVENT_BFECR2_PT2_DC1           _IEVENT_BFECR2.Bits.PT2_DC1
#define IEVENT_BFECR2_PT2_CC0           _IEVENT_BFECR2.Bits.PT2_CC0
#define IEVENT_BFECR2_PT2_CC1           _IEVENT_BFECR2.Bits.PT2_CC1
#define IEVENT_BFECR2_PT2_BC0           _IEVENT_BFECR2.Bits.PT2_BC0
#define IEVENT_BFECR2_PT2_BC1           _IEVENT_BFECR2.Bits.PT2_BC1
#define IEVENT_BFECR2_PT2_AC0           _IEVENT_BFECR2.Bits.PT2_AC0
#define IEVENT_BFECR2_PT2_AC1           _IEVENT_BFECR2.Bits.PT2_AC1
#define IEVENT_BFECR2_PT1_DC0           _IEVENT_BFECR2.Bits.PT1_DC0
#define IEVENT_BFECR2_PT1_DC1           _IEVENT_BFECR2.Bits.PT1_DC1
#define IEVENT_BFECR2_PT1_CC0           _IEVENT_BFECR2.Bits.PT1_CC0
#define IEVENT_BFECR2_PT1_CC1           _IEVENT_BFECR2.Bits.PT1_CC1
#define IEVENT_BFECR2_PT1_BC0           _IEVENT_BFECR2.Bits.PT1_BC0
#define IEVENT_BFECR2_PT1_BC1           _IEVENT_BFECR2.Bits.PT1_BC1
#define IEVENT_BFECR2_PT1_AC0           _IEVENT_BFECR2.Bits.PT1_AC0
#define IEVENT_BFECR2_PT1_AC1           _IEVENT_BFECR2.Bits.PT1_AC1
#define IEVENT_BFECR2_PT0_DC0           _IEVENT_BFECR2.Bits.PT0_DC0
#define IEVENT_BFECR2_PT0_DC1           _IEVENT_BFECR2.Bits.PT0_DC1
#define IEVENT_BFECR2_PT0_CC0           _IEVENT_BFECR2.Bits.PT0_CC0
#define IEVENT_BFECR2_PT0_CC1           _IEVENT_BFECR2.Bits.PT0_CC1
#define IEVENT_BFECR2_PT0_BC0           _IEVENT_BFECR2.Bits.PT0_BC0
#define IEVENT_BFECR2_PT0_BC1           _IEVENT_BFECR2.Bits.PT0_BC1
#define IEVENT_BFECR2_PT0_AC0           _IEVENT_BFECR2.Bits.PT0_AC0
#define IEVENT_BFECR2_PT0_AC1           _IEVENT_BFECR2.Bits.PT0_AC1
#define IEVENT_BFECR2_PT3_DC            _IEVENT_BFECR2.MergedBits.grpPT3_DC
#define IEVENT_BFECR2_PT3_CC            _IEVENT_BFECR2.MergedBits.grpPT3_CC
#define IEVENT_BFECR2_PT3_BC            _IEVENT_BFECR2.MergedBits.grpPT3_BC
#define IEVENT_BFECR2_PT3_AC            _IEVENT_BFECR2.MergedBits.grpPT3_AC
#define IEVENT_BFECR2_PT2_DC            _IEVENT_BFECR2.MergedBits.grpPT2_DC
#define IEVENT_BFECR2_PT2_CC            _IEVENT_BFECR2.MergedBits.grpPT2_CC
#define IEVENT_BFECR2_PT2_BC            _IEVENT_BFECR2.MergedBits.grpPT2_BC
#define IEVENT_BFECR2_PT2_AC            _IEVENT_BFECR2.MergedBits.grpPT2_AC
#define IEVENT_BFECR2_PT1_DC            _IEVENT_BFECR2.MergedBits.grpPT1_DC
#define IEVENT_BFECR2_PT1_CC            _IEVENT_BFECR2.MergedBits.grpPT1_CC
#define IEVENT_BFECR2_PT1_BC            _IEVENT_BFECR2.MergedBits.grpPT1_BC
#define IEVENT_BFECR2_PT1_AC            _IEVENT_BFECR2.MergedBits.grpPT1_AC
#define IEVENT_BFECR2_PT0_DC            _IEVENT_BFECR2.MergedBits.grpPT0_DC
#define IEVENT_BFECR2_PT0_CC            _IEVENT_BFECR2.MergedBits.grpPT0_CC
#define IEVENT_BFECR2_PT0_BC            _IEVENT_BFECR2.MergedBits.grpPT0_BC
#define IEVENT_BFECR2_PT0_AC            _IEVENT_BFECR2.MergedBits.grpPT0_AC

#define IEVENT_BFECR2_PT3_DC0_MASK      1
#define IEVENT_BFECR2_PT3_DC1_MASK      2
#define IEVENT_BFECR2_PT3_CC0_MASK      4
#define IEVENT_BFECR2_PT3_CC1_MASK      8
#define IEVENT_BFECR2_PT3_BC0_MASK      16
#define IEVENT_BFECR2_PT3_BC1_MASK      32
#define IEVENT_BFECR2_PT3_AC0_MASK      64
#define IEVENT_BFECR2_PT3_AC1_MASK      128
#define IEVENT_BFECR2_PT2_DC0_MASK      256
#define IEVENT_BFECR2_PT2_DC1_MASK      512
#define IEVENT_BFECR2_PT2_CC0_MASK      1024
#define IEVENT_BFECR2_PT2_CC1_MASK      2048
#define IEVENT_BFECR2_PT2_BC0_MASK      4096
#define IEVENT_BFECR2_PT2_BC1_MASK      8192
#define IEVENT_BFECR2_PT2_AC0_MASK      16384
#define IEVENT_BFECR2_PT2_AC1_MASK      32768
#define IEVENT_BFECR2_PT1_DC0_MASK      65536
#define IEVENT_BFECR2_PT1_DC1_MASK      131072
#define IEVENT_BFECR2_PT1_CC0_MASK      262144
#define IEVENT_BFECR2_PT1_CC1_MASK      524288
#define IEVENT_BFECR2_PT1_BC0_MASK      1048576
#define IEVENT_BFECR2_PT1_BC1_MASK      2097152
#define IEVENT_BFECR2_PT1_AC0_MASK      4194304
#define IEVENT_BFECR2_PT1_AC1_MASK      8388608
#define IEVENT_BFECR2_PT0_DC0_MASK      16777216
#define IEVENT_BFECR2_PT0_DC1_MASK      33554432
#define IEVENT_BFECR2_PT0_CC0_MASK      67108864
#define IEVENT_BFECR2_PT0_CC1_MASK      134217728
#define IEVENT_BFECR2_PT0_BC0_MASK      268435456
#define IEVENT_BFECR2_PT0_BC1_MASK      536870912
#define IEVENT_BFECR2_PT0_AC0_MASK      1073741824
#define IEVENT_BFECR2_PT0_AC1_MASK      2147483648
#define IEVENT_BFECR2_PT3_DC_MASK       3
#define IEVENT_BFECR2_PT3_DC_BITNUM     0
#define IEVENT_BFECR2_PT3_CC_MASK       12
#define IEVENT_BFECR2_PT3_CC_BITNUM     2
#define IEVENT_BFECR2_PT3_BC_MASK       48
#define IEVENT_BFECR2_PT3_BC_BITNUM     4
#define IEVENT_BFECR2_PT3_AC_MASK       192
#define IEVENT_BFECR2_PT3_AC_BITNUM     6
#define IEVENT_BFECR2_PT2_DC_MASK       768
#define IEVENT_BFECR2_PT2_DC_BITNUM     8
#define IEVENT_BFECR2_PT2_CC_MASK       3072
#define IEVENT_BFECR2_PT2_CC_BITNUM     10
#define IEVENT_BFECR2_PT2_BC_MASK       12288
#define IEVENT_BFECR2_PT2_BC_BITNUM     12
#define IEVENT_BFECR2_PT2_AC_MASK       49152
#define IEVENT_BFECR2_PT2_AC_BITNUM     14
#define IEVENT_BFECR2_PT1_DC_MASK       196608
#define IEVENT_BFECR2_PT1_DC_BITNUM     16
#define IEVENT_BFECR2_PT1_CC_MASK       786432
#define IEVENT_BFECR2_PT1_CC_BITNUM     18
#define IEVENT_BFECR2_PT1_BC_MASK       3145728
#define IEVENT_BFECR2_PT1_BC_BITNUM     20
#define IEVENT_BFECR2_PT1_AC_MASK       12582912
#define IEVENT_BFECR2_PT1_AC_BITNUM     22
#define IEVENT_BFECR2_PT0_DC_MASK       50331648
#define IEVENT_BFECR2_PT0_DC_BITNUM     24
#define IEVENT_BFECR2_PT0_CC_MASK       201326592
#define IEVENT_BFECR2_PT0_CC_BITNUM     26
#define IEVENT_BFECR2_PT0_BC_MASK       805306368
#define IEVENT_BFECR2_PT0_BC_BITNUM     28
#define IEVENT_BFECR2_PT0_AC_MASK       3221225472
#define IEVENT_BFECR2_PT0_AC_BITNUM     30


/*** IEVENT_IMXCR3 - iEvent Input Mux Configuration Register 3; 0xFFFF9B18 ***/
typedef union {
  dword Dword;
  struct {
    dword D_Select0  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 0 */
    dword D_Select1  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 1 */
    dword D_Select2  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 2 */
    dword D_Select3  :1;                                       /* D_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input D, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword C_Select0  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 0 */
    dword C_Select1  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 1 */
    dword C_Select2  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 2 */
    dword C_Select3  :1;                                       /* C_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input C, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword B_Select0  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 0 */
    dword B_Select1  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 1 */
    dword B_Select2  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 2 */
    dword B_Select3  :1;                                       /* B_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input B, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword A_Select0  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 0 */
    dword A_Select1  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 1 */
    dword A_Select2  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 2 */
    dword A_Select3  :1;                                       /* A_Select. This 4-bit field defines the bit number of the 16-bit event input vector to be selected as input A, bit 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
  struct {
    dword grpD_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpC_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpB_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword grpA_Select :4;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
  } MergedBits;
} IEVENT_IMXCR3STR;
extern volatile IEVENT_IMXCR3STR _IEVENT_IMXCR3 @0xFFFF9B18;
#define IEVENT_IMXCR3                   _IEVENT_IMXCR3.Dword
#define IEVENT_IMXCR3_D_Select0         _IEVENT_IMXCR3.Bits.D_Select0
#define IEVENT_IMXCR3_D_Select1         _IEVENT_IMXCR3.Bits.D_Select1
#define IEVENT_IMXCR3_D_Select2         _IEVENT_IMXCR3.Bits.D_Select2
#define IEVENT_IMXCR3_D_Select3         _IEVENT_IMXCR3.Bits.D_Select3
#define IEVENT_IMXCR3_C_Select0         _IEVENT_IMXCR3.Bits.C_Select0
#define IEVENT_IMXCR3_C_Select1         _IEVENT_IMXCR3.Bits.C_Select1
#define IEVENT_IMXCR3_C_Select2         _IEVENT_IMXCR3.Bits.C_Select2
#define IEVENT_IMXCR3_C_Select3         _IEVENT_IMXCR3.Bits.C_Select3
#define IEVENT_IMXCR3_B_Select0         _IEVENT_IMXCR3.Bits.B_Select0
#define IEVENT_IMXCR3_B_Select1         _IEVENT_IMXCR3.Bits.B_Select1
#define IEVENT_IMXCR3_B_Select2         _IEVENT_IMXCR3.Bits.B_Select2
#define IEVENT_IMXCR3_B_Select3         _IEVENT_IMXCR3.Bits.B_Select3
#define IEVENT_IMXCR3_A_Select0         _IEVENT_IMXCR3.Bits.A_Select0
#define IEVENT_IMXCR3_A_Select1         _IEVENT_IMXCR3.Bits.A_Select1
#define IEVENT_IMXCR3_A_Select2         _IEVENT_IMXCR3.Bits.A_Select2
#define IEVENT_IMXCR3_A_Select3         _IEVENT_IMXCR3.Bits.A_Select3
#define IEVENT_IMXCR3_D_Select          _IEVENT_IMXCR3.MergedBits.grpD_Select
#define IEVENT_IMXCR3_C_Select          _IEVENT_IMXCR3.MergedBits.grpC_Select
#define IEVENT_IMXCR3_B_Select          _IEVENT_IMXCR3.MergedBits.grpB_Select
#define IEVENT_IMXCR3_A_Select          _IEVENT_IMXCR3.MergedBits.grpA_Select

#define IEVENT_IMXCR3_D_Select0_MASK    1
#define IEVENT_IMXCR3_D_Select1_MASK    2
#define IEVENT_IMXCR3_D_Select2_MASK    4
#define IEVENT_IMXCR3_D_Select3_MASK    8
#define IEVENT_IMXCR3_C_Select0_MASK    256
#define IEVENT_IMXCR3_C_Select1_MASK    512
#define IEVENT_IMXCR3_C_Select2_MASK    1024
#define IEVENT_IMXCR3_C_Select3_MASK    2048
#define IEVENT_IMXCR3_B_Select0_MASK    65536
#define IEVENT_IMXCR3_B_Select1_MASK    131072
#define IEVENT_IMXCR3_B_Select2_MASK    262144
#define IEVENT_IMXCR3_B_Select3_MASK    524288
#define IEVENT_IMXCR3_A_Select0_MASK    16777216
#define IEVENT_IMXCR3_A_Select1_MASK    33554432
#define IEVENT_IMXCR3_A_Select2_MASK    67108864
#define IEVENT_IMXCR3_A_Select3_MASK    134217728
#define IEVENT_IMXCR3_D_Select_MASK     15
#define IEVENT_IMXCR3_D_Select_BITNUM   0
#define IEVENT_IMXCR3_C_Select_MASK     3840
#define IEVENT_IMXCR3_C_Select_BITNUM   8
#define IEVENT_IMXCR3_B_Select_MASK     983040
#define IEVENT_IMXCR3_B_Select_BITNUM   16
#define IEVENT_IMXCR3_A_Select_MASK     251658240
#define IEVENT_IMXCR3_A_Select_BITNUM   24


/*** IEVENT_BFECR3 - iEvent Boolean Function Evaluation Configuration Register 3; 0xFFFF9B1C ***/
typedef union {
  dword Dword;
  struct {
    dword PT3_DC0    :1;                                       /* Product Term 3, D input Configuration, bit 0 */
    dword PT3_DC1    :1;                                       /* Product Term 3, D input Configuration, bit 1 */
    dword PT3_CC0    :1;                                       /* Product Term 3, C input Configuration, bit 0 */
    dword PT3_CC1    :1;                                       /* Product Term 3, C input Configuration, bit 1 */
    dword PT3_BC0    :1;                                       /* Product Term 3, B input Configuration, bit 0 */
    dword PT3_BC1    :1;                                       /* Product Term 3, B input Configuration, bit 1 */
    dword PT3_AC0    :1;                                       /* Product Term 3, A input Configuration, bit 0 */
    dword PT3_AC1    :1;                                       /* Product Term 3, A input Configuration, bit 1 */
    dword PT2_DC0    :1;                                       /* Product Term 2, D input Configuration, bit 0 */
    dword PT2_DC1    :1;                                       /* Product Term 2, D input Configuration, bit 1 */
    dword PT2_CC0    :1;                                       /* Product Term 2, C input Configuration, bit 0 */
    dword PT2_CC1    :1;                                       /* Product Term 2, C input Configuration, bit 1 */
    dword PT2_BC0    :1;                                       /* Product Term 2, B input Configuration, bit 0 */
    dword PT2_BC1    :1;                                       /* Product Term 2, B input Configuration, bit 1 */
    dword PT2_AC0    :1;                                       /* Product Term 2, A input Configuration, bit 0 */
    dword PT2_AC1    :1;                                       /* Product Term 2, A input Configuration, bit 1 */
    dword PT1_DC0    :1;                                       /* Product Term 1, D input Configuration, bit 0 */
    dword PT1_DC1    :1;                                       /* Product Term 1, D input Configuration, bit 1 */
    dword PT1_CC0    :1;                                       /* Product Term 1, C input Configuration, bit 0 */
    dword PT1_CC1    :1;                                       /* Product Term 1, C input Configuration, bit 1 */
    dword PT1_BC0    :1;                                       /* Product Term 1, B input Configuration, bit 0 */
    dword PT1_BC1    :1;                                       /* Product Term 1, B input Configuration, bit 1 */
    dword PT1_AC0    :1;                                       /* Product Term 1, A input Configuration, bit 0 */
    dword PT1_AC1    :1;                                       /* Product Term 1, A input Configuration, bit 1 */
    dword PT0_DC0    :1;                                       /* Product Term 0, D input Configuration, bit 0 */
    dword PT0_DC1    :1;                                       /* Product Term 0, D input Configuration, bit 1 */
    dword PT0_CC0    :1;                                       /* Product Term 0, C input Configuration, bit 0 */
    dword PT0_CC1    :1;                                       /* Product Term 0, C input Configuration, bit 1 */
    dword PT0_BC0    :1;                                       /* Product Term 0, B input Configuration, bit 0 */
    dword PT0_BC1    :1;                                       /* Product Term 0, B input Configuration, bit 1 */
    dword PT0_AC0    :1;                                       /* Product Term 0, A input Configuration, bit 0 */
    dword PT0_AC1    :1;                                       /* Product Term 0, A input Configuration, bit 1 */
  } Bits;
  struct {
    dword grpPT3_DC :2;
    dword grpPT3_CC :2;
    dword grpPT3_BC :2;
    dword grpPT3_AC :2;
    dword grpPT2_DC :2;
    dword grpPT2_CC :2;
    dword grpPT2_BC :2;
    dword grpPT2_AC :2;
    dword grpPT1_DC :2;
    dword grpPT1_CC :2;
    dword grpPT1_BC :2;
    dword grpPT1_AC :2;
    dword grpPT0_DC :2;
    dword grpPT0_CC :2;
    dword grpPT0_BC :2;
    dword grpPT0_AC :2;
  } MergedBits;
} IEVENT_BFECR3STR;
extern volatile IEVENT_BFECR3STR _IEVENT_BFECR3 @0xFFFF9B1C;
#define IEVENT_BFECR3                   _IEVENT_BFECR3.Dword
#define IEVENT_BFECR3_PT3_DC0           _IEVENT_BFECR3.Bits.PT3_DC0
#define IEVENT_BFECR3_PT3_DC1           _IEVENT_BFECR3.Bits.PT3_DC1
#define IEVENT_BFECR3_PT3_CC0           _IEVENT_BFECR3.Bits.PT3_CC0
#define IEVENT_BFECR3_PT3_CC1           _IEVENT_BFECR3.Bits.PT3_CC1
#define IEVENT_BFECR3_PT3_BC0           _IEVENT_BFECR3.Bits.PT3_BC0
#define IEVENT_BFECR3_PT3_BC1           _IEVENT_BFECR3.Bits.PT3_BC1
#define IEVENT_BFECR3_PT3_AC0           _IEVENT_BFECR3.Bits.PT3_AC0
#define IEVENT_BFECR3_PT3_AC1           _IEVENT_BFECR3.Bits.PT3_AC1
#define IEVENT_BFECR3_PT2_DC0           _IEVENT_BFECR3.Bits.PT2_DC0
#define IEVENT_BFECR3_PT2_DC1           _IEVENT_BFECR3.Bits.PT2_DC1
#define IEVENT_BFECR3_PT2_CC0           _IEVENT_BFECR3.Bits.PT2_CC0
#define IEVENT_BFECR3_PT2_CC1           _IEVENT_BFECR3.Bits.PT2_CC1
#define IEVENT_BFECR3_PT2_BC0           _IEVENT_BFECR3.Bits.PT2_BC0
#define IEVENT_BFECR3_PT2_BC1           _IEVENT_BFECR3.Bits.PT2_BC1
#define IEVENT_BFECR3_PT2_AC0           _IEVENT_BFECR3.Bits.PT2_AC0
#define IEVENT_BFECR3_PT2_AC1           _IEVENT_BFECR3.Bits.PT2_AC1
#define IEVENT_BFECR3_PT1_DC0           _IEVENT_BFECR3.Bits.PT1_DC0
#define IEVENT_BFECR3_PT1_DC1           _IEVENT_BFECR3.Bits.PT1_DC1
#define IEVENT_BFECR3_PT1_CC0           _IEVENT_BFECR3.Bits.PT1_CC0
#define IEVENT_BFECR3_PT1_CC1           _IEVENT_BFECR3.Bits.PT1_CC1
#define IEVENT_BFECR3_PT1_BC0           _IEVENT_BFECR3.Bits.PT1_BC0
#define IEVENT_BFECR3_PT1_BC1           _IEVENT_BFECR3.Bits.PT1_BC1
#define IEVENT_BFECR3_PT1_AC0           _IEVENT_BFECR3.Bits.PT1_AC0
#define IEVENT_BFECR3_PT1_AC1           _IEVENT_BFECR3.Bits.PT1_AC1
#define IEVENT_BFECR3_PT0_DC0           _IEVENT_BFECR3.Bits.PT0_DC0
#define IEVENT_BFECR3_PT0_DC1           _IEVENT_BFECR3.Bits.PT0_DC1
#define IEVENT_BFECR3_PT0_CC0           _IEVENT_BFECR3.Bits.PT0_CC0
#define IEVENT_BFECR3_PT0_CC1           _IEVENT_BFECR3.Bits.PT0_CC1
#define IEVENT_BFECR3_PT0_BC0           _IEVENT_BFECR3.Bits.PT0_BC0
#define IEVENT_BFECR3_PT0_BC1           _IEVENT_BFECR3.Bits.PT0_BC1
#define IEVENT_BFECR3_PT0_AC0           _IEVENT_BFECR3.Bits.PT0_AC0
#define IEVENT_BFECR3_PT0_AC1           _IEVENT_BFECR3.Bits.PT0_AC1
#define IEVENT_BFECR3_PT3_DC            _IEVENT_BFECR3.MergedBits.grpPT3_DC
#define IEVENT_BFECR3_PT3_CC            _IEVENT_BFECR3.MergedBits.grpPT3_CC
#define IEVENT_BFECR3_PT3_BC            _IEVENT_BFECR3.MergedBits.grpPT3_BC
#define IEVENT_BFECR3_PT3_AC            _IEVENT_BFECR3.MergedBits.grpPT3_AC
#define IEVENT_BFECR3_PT2_DC            _IEVENT_BFECR3.MergedBits.grpPT2_DC
#define IEVENT_BFECR3_PT2_CC            _IEVENT_BFECR3.MergedBits.grpPT2_CC
#define IEVENT_BFECR3_PT2_BC            _IEVENT_BFECR3.MergedBits.grpPT2_BC
#define IEVENT_BFECR3_PT2_AC            _IEVENT_BFECR3.MergedBits.grpPT2_AC
#define IEVENT_BFECR3_PT1_DC            _IEVENT_BFECR3.MergedBits.grpPT1_DC
#define IEVENT_BFECR3_PT1_CC            _IEVENT_BFECR3.MergedBits.grpPT1_CC
#define IEVENT_BFECR3_PT1_BC            _IEVENT_BFECR3.MergedBits.grpPT1_BC
#define IEVENT_BFECR3_PT1_AC            _IEVENT_BFECR3.MergedBits.grpPT1_AC
#define IEVENT_BFECR3_PT0_DC            _IEVENT_BFECR3.MergedBits.grpPT0_DC
#define IEVENT_BFECR3_PT0_CC            _IEVENT_BFECR3.MergedBits.grpPT0_CC
#define IEVENT_BFECR3_PT0_BC            _IEVENT_BFECR3.MergedBits.grpPT0_BC
#define IEVENT_BFECR3_PT0_AC            _IEVENT_BFECR3.MergedBits.grpPT0_AC

#define IEVENT_BFECR3_PT3_DC0_MASK      1
#define IEVENT_BFECR3_PT3_DC1_MASK      2
#define IEVENT_BFECR3_PT3_CC0_MASK      4
#define IEVENT_BFECR3_PT3_CC1_MASK      8
#define IEVENT_BFECR3_PT3_BC0_MASK      16
#define IEVENT_BFECR3_PT3_BC1_MASK      32
#define IEVENT_BFECR3_PT3_AC0_MASK      64
#define IEVENT_BFECR3_PT3_AC1_MASK      128
#define IEVENT_BFECR3_PT2_DC0_MASK      256
#define IEVENT_BFECR3_PT2_DC1_MASK      512
#define IEVENT_BFECR3_PT2_CC0_MASK      1024
#define IEVENT_BFECR3_PT2_CC1_MASK      2048
#define IEVENT_BFECR3_PT2_BC0_MASK      4096
#define IEVENT_BFECR3_PT2_BC1_MASK      8192
#define IEVENT_BFECR3_PT2_AC0_MASK      16384
#define IEVENT_BFECR3_PT2_AC1_MASK      32768
#define IEVENT_BFECR3_PT1_DC0_MASK      65536
#define IEVENT_BFECR3_PT1_DC1_MASK      131072
#define IEVENT_BFECR3_PT1_CC0_MASK      262144
#define IEVENT_BFECR3_PT1_CC1_MASK      524288
#define IEVENT_BFECR3_PT1_BC0_MASK      1048576
#define IEVENT_BFECR3_PT1_BC1_MASK      2097152
#define IEVENT_BFECR3_PT1_AC0_MASK      4194304
#define IEVENT_BFECR3_PT1_AC1_MASK      8388608
#define IEVENT_BFECR3_PT0_DC0_MASK      16777216
#define IEVENT_BFECR3_PT0_DC1_MASK      33554432
#define IEVENT_BFECR3_PT0_CC0_MASK      67108864
#define IEVENT_BFECR3_PT0_CC1_MASK      134217728
#define IEVENT_BFECR3_PT0_BC0_MASK      268435456
#define IEVENT_BFECR3_PT0_BC1_MASK      536870912
#define IEVENT_BFECR3_PT0_AC0_MASK      1073741824
#define IEVENT_BFECR3_PT0_AC1_MASK      2147483648
#define IEVENT_BFECR3_PT3_DC_MASK       3
#define IEVENT_BFECR3_PT3_DC_BITNUM     0
#define IEVENT_BFECR3_PT3_CC_MASK       12
#define IEVENT_BFECR3_PT3_CC_BITNUM     2
#define IEVENT_BFECR3_PT3_BC_MASK       48
#define IEVENT_BFECR3_PT3_BC_BITNUM     4
#define IEVENT_BFECR3_PT3_AC_MASK       192
#define IEVENT_BFECR3_PT3_AC_BITNUM     6
#define IEVENT_BFECR3_PT2_DC_MASK       768
#define IEVENT_BFECR3_PT2_DC_BITNUM     8
#define IEVENT_BFECR3_PT2_CC_MASK       3072
#define IEVENT_BFECR3_PT2_CC_BITNUM     10
#define IEVENT_BFECR3_PT2_BC_MASK       12288
#define IEVENT_BFECR3_PT2_BC_BITNUM     12
#define IEVENT_BFECR3_PT2_AC_MASK       49152
#define IEVENT_BFECR3_PT2_AC_BITNUM     14
#define IEVENT_BFECR3_PT1_DC_MASK       196608
#define IEVENT_BFECR3_PT1_DC_BITNUM     16
#define IEVENT_BFECR3_PT1_CC_MASK       786432
#define IEVENT_BFECR3_PT1_CC_BITNUM     18
#define IEVENT_BFECR3_PT1_BC_MASK       3145728
#define IEVENT_BFECR3_PT1_BC_BITNUM     20
#define IEVENT_BFECR3_PT1_AC_MASK       12582912
#define IEVENT_BFECR3_PT1_AC_BITNUM     22
#define IEVENT_BFECR3_PT0_DC_MASK       50331648
#define IEVENT_BFECR3_PT0_DC_BITNUM     24
#define IEVENT_BFECR3_PT0_CC_MASK       201326592
#define IEVENT_BFECR3_PT0_CC_BITNUM     26
#define IEVENT_BFECR3_PT0_BC_MASK       805306368
#define IEVENT_BFECR3_PT0_BC_BITNUM     28
#define IEVENT_BFECR3_PT0_AC_MASK       3221225472
#define IEVENT_BFECR3_PT0_AC_BITNUM     30


/*** DMAREQC - DMA Request Control; 0xFFFFE400 ***/
typedef union {
  dword Dword;
  struct {
    dword DMAC3      :4;                                       /* DMA Channel 3 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword DMAC2      :4;                                       /* DMA Channel 2 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword DMAC1      :4;                                       /* DMA Channel 1 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword DMAC0      :4;                                       /* DMA Channel 0 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} DMAREQCSTR;
extern volatile DMAREQCSTR _DMAREQC @0xFFFFE400;
#define DMAREQC                         _DMAREQC.Dword
#define DMAREQC_DMAC3                   _DMAREQC.Bits.DMAC3
#define DMAREQC_DMAC2                   _DMAREQC.Bits.DMAC2
#define DMAREQC_DMAC1                   _DMAREQC.Bits.DMAC1
#define DMAREQC_DMAC0                   _DMAREQC.Bits.DMAC0

#define DMAREQC_DMAC3_MASK              15
#define DMAREQC_DMAC3_BITNUM            0
#define DMAREQC_DMAC2_MASK              3840
#define DMAREQC_DMAC2_BITNUM            8
#define DMAREQC_DMAC1_MASK              983040
#define DMAREQC_DMAC1_BITNUM            16
#define DMAREQC_DMAC0_MASK              251658240
#define DMAREQC_DMAC0_BITNUM            24


/*** SAR0 - Source Address Register 0; 0xFFFFE500 ***/
typedef union {
  dword Dword;
  struct {
    dword SAR        :24;                                      /* Source address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} SAR0STR;
extern volatile SAR0STR _SAR0 @0xFFFFE500;
#define SAR0                            _SAR0.Dword
#define SAR0_SAR                        _SAR0.Bits.SAR

#define SAR0_SAR_MASK                   16777215
#define SAR0_SAR_BITNUM                 0


/*** DAR0 - Destination Address Register 0; 0xFFFFE504 ***/
typedef union {
  dword Dword;
  struct {
    dword DAR        :24;                                      /* Destination address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} DAR0STR;
extern volatile DAR0STR _DAR0 @0xFFFFE504;
#define DAR0                            _DAR0.Dword
#define DAR0_DAR                        _DAR0.Bits.DAR

#define DAR0_DAR_MASK                   16777215
#define DAR0_DAR_BITNUM                 0


/*** BCR0 - Byte Count Register 0; 0xFFFFE508 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** DSR0 - DMA Status Register 0; 0xFFFFE508 ***/
    union {
      byte Byte;
      struct {
        byte DONE        :1;                                       /* Transactions done. Set when all DMA controller transactions complete, as determined by transfer count or error conditions. When BCR reaches zero, DONE is set when the final transfer completes successfully. DONE can also be used to abort a transfer by resetting the status bits. When a transfer completes, software must clear DONE before reprogramming the DMA */
        byte BSY         :1;                                       /* Busy */
        byte REQ         :1;                                       /* Request */
        byte             :1; 
        byte BED         :1;                                       /* Bus error on destination */
        byte BES         :1;                                       /* Bus error on source */
        byte CE          :1;                                       /* Configuration error. Occurs when BCR, SAR, or DAR does not match the requested transfer size, or if BCR = 0 when the DMA receives a start condition. CE is cleared at hardware reset or by writing a 1 to DSR[DONE] */
        byte             :1; 
      } Bits;
    } DSR0STR;
    #define DSR0                        _BCR0.Overlap_STR.DSR0STR.Byte
    #define DSR0_DONE                   _BCR0.Overlap_STR.DSR0STR.Bits.DONE
    #define DSR0_BSY                    _BCR0.Overlap_STR.DSR0STR.Bits.BSY
    #define DSR0_REQ                    _BCR0.Overlap_STR.DSR0STR.Bits.REQ
    #define DSR0_BED                    _BCR0.Overlap_STR.DSR0STR.Bits.BED
    #define DSR0_BES                    _BCR0.Overlap_STR.DSR0STR.Bits.BES
    #define DSR0_CE                     _BCR0.Overlap_STR.DSR0STR.Bits.CE
    
    #define DSR0_DONE_MASK              1
    #define DSR0_BSY_MASK               2
    #define DSR0_REQ_MASK               4
    #define DSR0_BED_MASK               16
    #define DSR0_BES_MASK               32
    #define DSR0_CE_MASK                64
    
  } Overlap_STR;

  struct {
    dword BCR        :24;                                      /* Byte count */
    dword DSR        :8;                                       /* DMA Status Register */
  } Bits;
} BCR0STR;
extern volatile BCR0STR _BCR0 @0xFFFFE508;
#define BCR0                            _BCR0.Dword
#define BCR0_BCR                        _BCR0.Bits.BCR
#define BCR0_DSR                        _BCR0.Bits.DSR

#define BCR0_BCR_MASK                   16777215
#define BCR0_BCR_BITNUM                 0
#define BCR0_DSR_MASK                   4278190080
#define BCR0_DSR_BITNUM                 24


/*** DCR0 - DMA Control Register 0; 0xFFFFE50C ***/
typedef union {
  dword Dword;
  struct {
    dword LCH2       :2;                                       /* Link channel 2. Indicates the DMA channel assigned as link channel 2. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR0[CE] is set) */
    dword LCH1       :2;                                       /* Link channel 1. Indicates the DMA channel assigned as link channel 1. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR0[CE] is set) */
    dword LINKCC     :2;                                       /* Link channel control. Allows DMA channels to have their transfers linked. The current DMA channel will trigger a DMA request to the linked channels (LCH1 or LCH2) depending on the condition described by the LINKCC bits. The LINKCC field provides a 2-bit encoded value defining the applicable channel-to-channel linking */
    dword            :1; 
    dword D_REQ      :1;                                       /* Disable request. If this flag is set, the DMA hardware automatically clears the DCR0[ERQ] bit when the byte count register reaches zero */
    dword DMOD       :4;                                       /* Destination address modulo. Defines the size of the destination data circular buffer used by the DMA controller */
    dword SMOD       :4;                                       /* Source address modulo. Defines the size of the source data circular buffer used by the DMA controller */
    dword START      :1;                                       /* Start transfer */
    dword DSIZE      :2;                                       /* Destination size. Determines the data size of the destination bus cycle for the DMA controller */
    dword DINC       :1;                                       /* Destination increment. Controls whether a destination address increments after each successful transfer */
    dword SSIZE      :2;                                       /* Source size. Determines the data size of the source bus cycle for the DMA control module */
    dword SINC       :1;                                       /* Source increment. Controls whether a source address increments after each successful transfer */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword AA         :1;                                       /* Auto-align. AA and SIZE determine whether the source or destination is auto-aligned, that is, transfers are optimized based on the address and size */
    dword CS         :1;                                       /* Cycle steal */
    dword ERQ        :1;                                       /* Enable peripheral request. Care should be taken because a collision can occur between the START bit and DREQ0 when ERQ equals 1 */
    dword EINT       :1;                                       /* Interrupt on completion of transfer. Determines whether an interrupt is generated by completing a transfer or by the occurrence of an error condition */
  } Bits;
} DCR0STR;
extern volatile DCR0STR _DCR0 @0xFFFFE50C;
#define DCR0                            _DCR0.Dword
#define DCR0_LCH2                       _DCR0.Bits.LCH2
#define DCR0_LCH1                       _DCR0.Bits.LCH1
#define DCR0_LINKCC                     _DCR0.Bits.LINKCC
#define DCR0_D_REQ                      _DCR0.Bits.D_REQ
#define DCR0_DMOD                       _DCR0.Bits.DMOD
#define DCR0_SMOD                       _DCR0.Bits.SMOD
#define DCR0_START                      _DCR0.Bits.START
#define DCR0_DSIZE                      _DCR0.Bits.DSIZE
#define DCR0_DINC                       _DCR0.Bits.DINC
#define DCR0_SSIZE                      _DCR0.Bits.SSIZE
#define DCR0_SINC                       _DCR0.Bits.SINC
#define DCR0_AA                         _DCR0.Bits.AA
#define DCR0_CS                         _DCR0.Bits.CS
#define DCR0_ERQ                        _DCR0.Bits.ERQ
#define DCR0_EINT                       _DCR0.Bits.EINT

#define DCR0_LCH2_MASK                  3
#define DCR0_LCH2_BITNUM                0
#define DCR0_LCH1_MASK                  12
#define DCR0_LCH1_BITNUM                2
#define DCR0_LINKCC_MASK                48
#define DCR0_LINKCC_BITNUM              4
#define DCR0_D_REQ_MASK                 128
#define DCR0_DMOD_MASK                  3840
#define DCR0_DMOD_BITNUM                8
#define DCR0_SMOD_MASK                  61440
#define DCR0_SMOD_BITNUM                12
#define DCR0_START_MASK                 65536
#define DCR0_DSIZE_MASK                 393216
#define DCR0_DSIZE_BITNUM               17
#define DCR0_DINC_MASK                  524288
#define DCR0_SSIZE_MASK                 3145728
#define DCR0_SSIZE_BITNUM               20
#define DCR0_SINC_MASK                  4194304
#define DCR0_AA_MASK                    268435456
#define DCR0_CS_MASK                    536870912
#define DCR0_ERQ_MASK                   1073741824
#define DCR0_EINT_MASK                  2147483648


/*** SAR1 - Source Address Register 1; 0xFFFFE510 ***/
typedef union {
  dword Dword;
  struct {
    dword SAR        :24;                                      /* Source address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} SAR1STR;
extern volatile SAR1STR _SAR1 @0xFFFFE510;
#define SAR1                            _SAR1.Dword
#define SAR1_SAR                        _SAR1.Bits.SAR

#define SAR1_SAR_MASK                   16777215
#define SAR1_SAR_BITNUM                 0


/*** DAR1 - Destination Address Register 1; 0xFFFFE514 ***/
typedef union {
  dword Dword;
  struct {
    dword DAR        :24;                                      /* Destination address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} DAR1STR;
extern volatile DAR1STR _DAR1 @0xFFFFE514;
#define DAR1                            _DAR1.Dword
#define DAR1_DAR                        _DAR1.Bits.DAR

#define DAR1_DAR_MASK                   16777215
#define DAR1_DAR_BITNUM                 0


/*** BCR1 - Byte Count Register 1; 0xFFFFE518 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** DSR1 - DMA Status Register 1; 0xFFFFE518 ***/
    union {
      byte Byte;
      struct {
        byte DONE        :1;                                       /* Transactions done. Set when all DMA controller transactions complete, as determined by transfer count or error conditions. When BCR reaches zero, DONE is set when the final transfer completes successfully. DONE can also be used to abort a transfer by resetting the status bits. When a transfer completes, software must clear DONE before reprogramming the DMA */
        byte BSY         :1;                                       /* Busy */
        byte REQ         :1;                                       /* Request */
        byte             :1; 
        byte BED         :1;                                       /* Bus error on destination */
        byte BES         :1;                                       /* Bus error on source */
        byte CE          :1;                                       /* Configuration error. Occurs when BCR, SAR, or DAR does not match the requested transfer size, or if BCR = 0 when the DMA receives a start condition. CE is cleared at hardware reset or by writing a 1 to DSR[DONE] */
        byte             :1; 
      } Bits;
    } DSR1STR;
    #define DSR1                        _BCR1.Overlap_STR.DSR1STR.Byte
    #define DSR1_DONE                   _BCR1.Overlap_STR.DSR1STR.Bits.DONE
    #define DSR1_BSY                    _BCR1.Overlap_STR.DSR1STR.Bits.BSY
    #define DSR1_REQ                    _BCR1.Overlap_STR.DSR1STR.Bits.REQ
    #define DSR1_BED                    _BCR1.Overlap_STR.DSR1STR.Bits.BED
    #define DSR1_BES                    _BCR1.Overlap_STR.DSR1STR.Bits.BES
    #define DSR1_CE                     _BCR1.Overlap_STR.DSR1STR.Bits.CE
    
    #define DSR1_DONE_MASK              1
    #define DSR1_BSY_MASK               2
    #define DSR1_REQ_MASK               4
    #define DSR1_BED_MASK               16
    #define DSR1_BES_MASK               32
    #define DSR1_CE_MASK                64
    
  } Overlap_STR;

  struct {
    dword BCR        :24;                                      /* Byte count */
    dword DSR        :8;                                       /* DMA Status Register */
  } Bits;
} BCR1STR;
extern volatile BCR1STR _BCR1 @0xFFFFE518;
#define BCR1                            _BCR1.Dword
#define BCR1_BCR                        _BCR1.Bits.BCR
#define BCR1_DSR                        _BCR1.Bits.DSR

#define BCR1_BCR_MASK                   16777215
#define BCR1_BCR_BITNUM                 0
#define BCR1_DSR_MASK                   4278190080
#define BCR1_DSR_BITNUM                 24


/*** DCR1 - DMA Control Register 1; 0xFFFFE51C ***/
typedef union {
  dword Dword;
  struct {
    dword LCH2       :2;                                       /* Link channel 2. Indicates the DMA channel assigned as link channel 2. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR1[CE] is set) */
    dword LCH1       :2;                                       /* Link channel 1. Indicates the DMA channel assigned as link channel 1. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR1[CE] is set) */
    dword LINKCC     :2;                                       /* Link channel control. Allows DMA channels to have their transfers linked. The current DMA channel will trigger a DMA request to the linked channels (LCH1 or LCH2) depending on the condition described by the LINKCC bits. The LINKCC field provides a 2-bit encoded value defining the applicable channel-to-channel linking */
    dword            :1; 
    dword D_REQ      :1;                                       /* Disable request. If this flag is set, the DMA hardware automatically clears the DCR1[ERQ] bit when the byte count register reaches zero */
    dword DMOD       :4;                                       /* Destination address modulo. Defines the size of the destination data circular buffer used by the DMA controller */
    dword SMOD       :4;                                       /* Source address modulo. Defines the size of the source data circular buffer used by the DMA controller */
    dword START      :1;                                       /* Start transfer */
    dword DSIZE      :2;                                       /* Destination size. Determines the data size of the destination bus cycle for the DMA controller */
    dword DINC       :1;                                       /* Destination increment. Controls whether a destination address increments after each successful transfer */
    dword SSIZE      :2;                                       /* Source size. Determines the data size of the source bus cycle for the DMA control module */
    dword SINC       :1;                                       /* Source increment. Controls whether a source address increments after each successful transfer */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword AA         :1;                                       /* Auto-align. AA and SIZE determine whether the source or destination is auto-aligned, that is, transfers are optimized based on the address and size */
    dword CS         :1;                                       /* Cycle steal */
    dword ERQ        :1;                                       /* Enable peripheral request. Care should be taken because a collision can occur between the START bit and DREQ1 when ERQ equals 1 */
    dword EINT       :1;                                       /* Interrupt on completion of transfer. Determines whether an interrupt is generated by completing a transfer or by the occurrence of an error condition */
  } Bits;
} DCR1STR;
extern volatile DCR1STR _DCR1 @0xFFFFE51C;
#define DCR1                            _DCR1.Dword
#define DCR1_LCH2                       _DCR1.Bits.LCH2
#define DCR1_LCH1                       _DCR1.Bits.LCH1
#define DCR1_LINKCC                     _DCR1.Bits.LINKCC
#define DCR1_D_REQ                      _DCR1.Bits.D_REQ
#define DCR1_DMOD                       _DCR1.Bits.DMOD
#define DCR1_SMOD                       _DCR1.Bits.SMOD
#define DCR1_START                      _DCR1.Bits.START
#define DCR1_DSIZE                      _DCR1.Bits.DSIZE
#define DCR1_DINC                       _DCR1.Bits.DINC
#define DCR1_SSIZE                      _DCR1.Bits.SSIZE
#define DCR1_SINC                       _DCR1.Bits.SINC
#define DCR1_AA                         _DCR1.Bits.AA
#define DCR1_CS                         _DCR1.Bits.CS
#define DCR1_ERQ                        _DCR1.Bits.ERQ
#define DCR1_EINT                       _DCR1.Bits.EINT

#define DCR1_LCH2_MASK                  3
#define DCR1_LCH2_BITNUM                0
#define DCR1_LCH1_MASK                  12
#define DCR1_LCH1_BITNUM                2
#define DCR1_LINKCC_MASK                48
#define DCR1_LINKCC_BITNUM              4
#define DCR1_D_REQ_MASK                 128
#define DCR1_DMOD_MASK                  3840
#define DCR1_DMOD_BITNUM                8
#define DCR1_SMOD_MASK                  61440
#define DCR1_SMOD_BITNUM                12
#define DCR1_START_MASK                 65536
#define DCR1_DSIZE_MASK                 393216
#define DCR1_DSIZE_BITNUM               17
#define DCR1_DINC_MASK                  524288
#define DCR1_SSIZE_MASK                 3145728
#define DCR1_SSIZE_BITNUM               20
#define DCR1_SINC_MASK                  4194304
#define DCR1_AA_MASK                    268435456
#define DCR1_CS_MASK                    536870912
#define DCR1_ERQ_MASK                   1073741824
#define DCR1_EINT_MASK                  2147483648


/*** SAR2 - Source Address Register 2; 0xFFFFE520 ***/
typedef union {
  dword Dword;
  struct {
    dword SAR        :24;                                      /* Source address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} SAR2STR;
extern volatile SAR2STR _SAR2 @0xFFFFE520;
#define SAR2                            _SAR2.Dword
#define SAR2_SAR                        _SAR2.Bits.SAR

#define SAR2_SAR_MASK                   16777215
#define SAR2_SAR_BITNUM                 0


/*** DAR2 - Destination Address Register 2; 0xFFFFE524 ***/
typedef union {
  dword Dword;
  struct {
    dword DAR        :24;                                      /* Destination address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} DAR2STR;
extern volatile DAR2STR _DAR2 @0xFFFFE524;
#define DAR2                            _DAR2.Dword
#define DAR2_DAR                        _DAR2.Bits.DAR

#define DAR2_DAR_MASK                   16777215
#define DAR2_DAR_BITNUM                 0


/*** BCR2 - Byte Count Register 2; 0xFFFFE528 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** DSR2 - DMA Status Register 2; 0xFFFFE528 ***/
    union {
      byte Byte;
      struct {
        byte DONE        :1;                                       /* Transactions done. Set when all DMA controller transactions complete, as determined by transfer count or error conditions. When BCR reaches zero, DONE is set when the final transfer completes successfully. DONE can also be used to abort a transfer by resetting the status bits. When a transfer completes, software must clear DONE before reprogramming the DMA */
        byte BSY         :1;                                       /* Busy */
        byte REQ         :1;                                       /* Request */
        byte             :1; 
        byte BED         :1;                                       /* Bus error on destination */
        byte BES         :1;                                       /* Bus error on source */
        byte CE          :1;                                       /* Configuration error. Occurs when BCR, SAR, or DAR does not match the requested transfer size, or if BCR = 0 when the DMA receives a start condition. CE is cleared at hardware reset or by writing a 1 to DSR[DONE] */
        byte             :1; 
      } Bits;
    } DSR2STR;
    #define DSR2                        _BCR2.Overlap_STR.DSR2STR.Byte
    #define DSR2_DONE                   _BCR2.Overlap_STR.DSR2STR.Bits.DONE
    #define DSR2_BSY                    _BCR2.Overlap_STR.DSR2STR.Bits.BSY
    #define DSR2_REQ                    _BCR2.Overlap_STR.DSR2STR.Bits.REQ
    #define DSR2_BED                    _BCR2.Overlap_STR.DSR2STR.Bits.BED
    #define DSR2_BES                    _BCR2.Overlap_STR.DSR2STR.Bits.BES
    #define DSR2_CE                     _BCR2.Overlap_STR.DSR2STR.Bits.CE
    
    #define DSR2_DONE_MASK              1
    #define DSR2_BSY_MASK               2
    #define DSR2_REQ_MASK               4
    #define DSR2_BED_MASK               16
    #define DSR2_BES_MASK               32
    #define DSR2_CE_MASK                64
    
  } Overlap_STR;

  struct {
    dword BCR        :24;                                      /* Byte count */
    dword DSR        :8;                                       /* DMA Status Register */
  } Bits;
} BCR2STR;
extern volatile BCR2STR _BCR2 @0xFFFFE528;
#define BCR2                            _BCR2.Dword
#define BCR2_BCR                        _BCR2.Bits.BCR
#define BCR2_DSR                        _BCR2.Bits.DSR

#define BCR2_BCR_MASK                   16777215
#define BCR2_BCR_BITNUM                 0
#define BCR2_DSR_MASK                   4278190080
#define BCR2_DSR_BITNUM                 24


/*** DCR2 - DMA Control Register 2; 0xFFFFE52C ***/
typedef union {
  dword Dword;
  struct {
    dword LCH2       :2;                                       /* Link channel 2. Indicates the DMA channel assigned as link channel 2. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR2[CE] is set) */
    dword LCH1       :2;                                       /* Link channel 1. Indicates the DMA channel assigned as link channel 1. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR2[CE] is set) */
    dword LINKCC     :2;                                       /* Link channel control. Allows DMA channels to have their transfers linked. The current DMA channel will trigger a DMA request to the linked channels (LCH1 or LCH2) depending on the condition described by the LINKCC bits. The LINKCC field provides a 2-bit encoded value defining the applicable channel-to-channel linking */
    dword            :1; 
    dword D_REQ      :1;                                       /* Disable request. If this flag is set, the DMA hardware automatically clears the DCR2[ERQ] bit when the byte count register reaches zero */
    dword DMOD       :4;                                       /* Destination address modulo. Defines the size of the destination data circular buffer used by the DMA controller */
    dword SMOD       :4;                                       /* Source address modulo. Defines the size of the source data circular buffer used by the DMA controller */
    dword START      :1;                                       /* Start transfer */
    dword DSIZE      :2;                                       /* Destination size. Determines the data size of the destination bus cycle for the DMA controller */
    dword DINC       :1;                                       /* Destination increment. Controls whether a destination address increments after each successful transfer */
    dword SSIZE      :2;                                       /* Source size. Determines the data size of the source bus cycle for the DMA control module */
    dword SINC       :1;                                       /* Source increment. Controls whether a source address increments after each successful transfer */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword AA         :1;                                       /* Auto-align. AA and SIZE determine whether the source or destination is auto-aligned, that is, transfers are optimized based on the address and size */
    dword CS         :1;                                       /* Cycle steal */
    dword ERQ        :1;                                       /* Enable peripheral request. Care should be taken because a collision can occur between the START bit and DREQ2 when ERQ equals 1 */
    dword EINT       :1;                                       /* Interrupt on completion of transfer. Determines whether an interrupt is generated by completing a transfer or by the occurrence of an error condition */
  } Bits;
} DCR2STR;
extern volatile DCR2STR _DCR2 @0xFFFFE52C;
#define DCR2                            _DCR2.Dword
#define DCR2_LCH2                       _DCR2.Bits.LCH2
#define DCR2_LCH1                       _DCR2.Bits.LCH1
#define DCR2_LINKCC                     _DCR2.Bits.LINKCC
#define DCR2_D_REQ                      _DCR2.Bits.D_REQ
#define DCR2_DMOD                       _DCR2.Bits.DMOD
#define DCR2_SMOD                       _DCR2.Bits.SMOD
#define DCR2_START                      _DCR2.Bits.START
#define DCR2_DSIZE                      _DCR2.Bits.DSIZE
#define DCR2_DINC                       _DCR2.Bits.DINC
#define DCR2_SSIZE                      _DCR2.Bits.SSIZE
#define DCR2_SINC                       _DCR2.Bits.SINC
#define DCR2_AA                         _DCR2.Bits.AA
#define DCR2_CS                         _DCR2.Bits.CS
#define DCR2_ERQ                        _DCR2.Bits.ERQ
#define DCR2_EINT                       _DCR2.Bits.EINT

#define DCR2_LCH2_MASK                  3
#define DCR2_LCH2_BITNUM                0
#define DCR2_LCH1_MASK                  12
#define DCR2_LCH1_BITNUM                2
#define DCR2_LINKCC_MASK                48
#define DCR2_LINKCC_BITNUM              4
#define DCR2_D_REQ_MASK                 128
#define DCR2_DMOD_MASK                  3840
#define DCR2_DMOD_BITNUM                8
#define DCR2_SMOD_MASK                  61440
#define DCR2_SMOD_BITNUM                12
#define DCR2_START_MASK                 65536
#define DCR2_DSIZE_MASK                 393216
#define DCR2_DSIZE_BITNUM               17
#define DCR2_DINC_MASK                  524288
#define DCR2_SSIZE_MASK                 3145728
#define DCR2_SSIZE_BITNUM               20
#define DCR2_SINC_MASK                  4194304
#define DCR2_AA_MASK                    268435456
#define DCR2_CS_MASK                    536870912
#define DCR2_ERQ_MASK                   1073741824
#define DCR2_EINT_MASK                  2147483648


/*** SAR3 - Source Address Register 3; 0xFFFFE530 ***/
typedef union {
  dword Dword;
  struct {
    dword SAR        :24;                                      /* Source address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} SAR3STR;
extern volatile SAR3STR _SAR3 @0xFFFFE530;
#define SAR3                            _SAR3.Dword
#define SAR3_SAR                        _SAR3.Bits.SAR

#define SAR3_SAR_MASK                   16777215
#define SAR3_SAR_BITNUM                 0


/*** DAR3 - Destination Address Register 3; 0xFFFFE534 ***/
typedef union {
  dword Dword;
  struct {
    dword DAR        :24;                                      /* Destination address */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
} DAR3STR;
extern volatile DAR3STR _DAR3 @0xFFFFE534;
#define DAR3                            _DAR3.Dword
#define DAR3_DAR                        _DAR3.Bits.DAR

#define DAR3_DAR_MASK                   16777215
#define DAR3_DAR_BITNUM                 0


/*** BCR3 - Byte Count Register 3; 0xFFFFE538 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** DSR3 - DMA Status Register 3; 0xFFFFE538 ***/
    union {
      byte Byte;
      struct {
        byte DONE        :1;                                       /* Transactions done. Set when all DMA controller transactions complete, as determined by transfer count or error conditions. When BCR reaches zero, DONE is set when the final transfer completes successfully. DONE can also be used to abort a transfer by resetting the status bits. When a transfer completes, software must clear DONE before reprogramming the DMA */
        byte BSY         :1;                                       /* Busy */
        byte REQ         :1;                                       /* Request */
        byte             :1; 
        byte BED         :1;                                       /* Bus error on destination */
        byte BES         :1;                                       /* Bus error on source */
        byte CE          :1;                                       /* Configuration error. Occurs when BCR, SAR, or DAR does not match the requested transfer size, or if BCR = 0 when the DMA receives a start condition. CE is cleared at hardware reset or by writing a 1 to DSR[DONE] */
        byte             :1; 
      } Bits;
    } DSR3STR;
    #define DSR3                        _BCR3.Overlap_STR.DSR3STR.Byte
    #define DSR3_DONE                   _BCR3.Overlap_STR.DSR3STR.Bits.DONE
    #define DSR3_BSY                    _BCR3.Overlap_STR.DSR3STR.Bits.BSY
    #define DSR3_REQ                    _BCR3.Overlap_STR.DSR3STR.Bits.REQ
    #define DSR3_BED                    _BCR3.Overlap_STR.DSR3STR.Bits.BED
    #define DSR3_BES                    _BCR3.Overlap_STR.DSR3STR.Bits.BES
    #define DSR3_CE                     _BCR3.Overlap_STR.DSR3STR.Bits.CE
    
    #define DSR3_DONE_MASK              1
    #define DSR3_BSY_MASK               2
    #define DSR3_REQ_MASK               4
    #define DSR3_BED_MASK               16
    #define DSR3_BES_MASK               32
    #define DSR3_CE_MASK                64
    
  } Overlap_STR;

  struct {
    dword BCR        :24;                                      /* Byte count */
    dword DSR        :8;                                       /* DMA Status Register */
  } Bits;
} BCR3STR;
extern volatile BCR3STR _BCR3 @0xFFFFE538;
#define BCR3                            _BCR3.Dword
#define BCR3_BCR                        _BCR3.Bits.BCR
#define BCR3_DSR                        _BCR3.Bits.DSR

#define BCR3_BCR_MASK                   16777215
#define BCR3_BCR_BITNUM                 0
#define BCR3_DSR_MASK                   4278190080
#define BCR3_DSR_BITNUM                 24


/*** DCR3 - DMA Control Register 3; 0xFFFFE53C ***/
typedef union {
  dword Dword;
  struct {
    dword LCH2       :2;                                       /* Link channel 2. Indicates the DMA channel assigned as link channel 2. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR3[CE] is set) */
    dword LCH1       :2;                                       /* Link channel 1. Indicates the DMA channel assigned as link channel 1. The link channel number cannot be the same as the currently executing channel, and generates a configuration error if this is attempted (DSR3[CE] is set) */
    dword LINKCC     :2;                                       /* Link channel control. Allows DMA channels to have their transfers linked. The current DMA channel will trigger a DMA request to the linked channels (LCH1 or LCH2) depending on the condition described by the LINKCC bits. The LINKCC field provides a 2-bit encoded value defining the applicable channel-to-channel linking */
    dword            :1; 
    dword D_REQ      :1;                                       /* Disable request. If this flag is set, the DMA hardware automatically clears the DCR3[ERQ] bit when the byte count register reaches zero */
    dword DMOD       :4;                                       /* Destination address modulo. Defines the size of the destination data circular buffer used by the DMA controller */
    dword SMOD       :4;                                       /* Source address modulo. Defines the size of the source data circular buffer used by the DMA controller */
    dword START      :1;                                       /* Start transfer */
    dword DSIZE      :2;                                       /* Destination size. Determines the data size of the destination bus cycle for the DMA controller */
    dword DINC       :1;                                       /* Destination increment. Controls whether a destination address increments after each successful transfer */
    dword SSIZE      :2;                                       /* Source size. Determines the data size of the source bus cycle for the DMA control module */
    dword SINC       :1;                                       /* Source increment. Controls whether a source address increments after each successful transfer */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword AA         :1;                                       /* Auto-align. AA and SIZE determine whether the source or destination is auto-aligned, that is, transfers are optimized based on the address and size */
    dword CS         :1;                                       /* Cycle steal */
    dword ERQ        :1;                                       /* Enable peripheral request. Care should be taken because a collision can occur between the START bit and DREQ3 when ERQ equals 1 */
    dword EINT       :1;                                       /* Interrupt on completion of transfer. Determines whether an interrupt is generated by completing a transfer or by the occurrence of an error condition */
  } Bits;
} DCR3STR;
extern volatile DCR3STR _DCR3 @0xFFFFE53C;
#define DCR3                            _DCR3.Dword
#define DCR3_LCH2                       _DCR3.Bits.LCH2
#define DCR3_LCH1                       _DCR3.Bits.LCH1
#define DCR3_LINKCC                     _DCR3.Bits.LINKCC
#define DCR3_D_REQ                      _DCR3.Bits.D_REQ
#define DCR3_DMOD                       _DCR3.Bits.DMOD
#define DCR3_SMOD                       _DCR3.Bits.SMOD
#define DCR3_START                      _DCR3.Bits.START
#define DCR3_DSIZE                      _DCR3.Bits.DSIZE
#define DCR3_DINC                       _DCR3.Bits.DINC
#define DCR3_SSIZE                      _DCR3.Bits.SSIZE
#define DCR3_SINC                       _DCR3.Bits.SINC
#define DCR3_AA                         _DCR3.Bits.AA
#define DCR3_CS                         _DCR3.Bits.CS
#define DCR3_ERQ                        _DCR3.Bits.ERQ
#define DCR3_EINT                       _DCR3.Bits.EINT

#define DCR3_LCH2_MASK                  3
#define DCR3_LCH2_BITNUM                0
#define DCR3_LCH1_MASK                  12
#define DCR3_LCH1_BITNUM                2
#define DCR3_LINKCC_MASK                48
#define DCR3_LINKCC_BITNUM              4
#define DCR3_D_REQ_MASK                 128
#define DCR3_DMOD_MASK                  3840
#define DCR3_DMOD_BITNUM                8
#define DCR3_SMOD_MASK                  61440
#define DCR3_SMOD_BITNUM                12
#define DCR3_START_MASK                 65536
#define DCR3_DSIZE_MASK                 393216
#define DCR3_DSIZE_BITNUM               17
#define DCR3_DINC_MASK                  524288
#define DCR3_SSIZE_MASK                 3145728
#define DCR3_SSIZE_BITNUM               20
#define DCR3_SINC_MASK                  4194304
#define DCR3_AA_MASK                    268435456
#define DCR3_CS_MASK                    536870912
#define DCR3_ERQ_MASK                   1073741824
#define DCR3_EINT_MASK                  2147483648


/*** INTC_IMRH - Interrupt Mask Register High; 0xFFFFFFC8 ***/
typedef union {
  dword Dword;
  struct {
    dword IMR32      :1;                                       /* Interrupt mask register 32 */
    dword IMR33      :1;                                       /* Interrupt mask register 33 */
    dword IMR34      :1;                                       /* Interrupt mask register 34 */
    dword IMR35      :1;                                       /* Interrupt mask register 35 */
    dword IMR36      :1;                                       /* Interrupt mask register 36 */
    dword IMR37      :1;                                       /* Interrupt mask register 37 */
    dword IMR38      :1;                                       /* Interrupt mask register 38 */
    dword IMR39      :1;                                       /* Interrupt mask register 39 */
    dword IMR40      :1;                                       /* Interrupt mask register 40 */
    dword IMR41      :1;                                       /* Interrupt mask register 41 */
    dword IMR42      :1;                                       /* Interrupt mask register 42 */
    dword IMR43      :1;                                       /* Interrupt mask register 43 */
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
    dword            :1; 
  } Bits;
  struct {
    dword grpIMR_32 :12;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
    dword        :1;
  } MergedBits;
} INTC_IMRHSTR;
extern volatile INTC_IMRHSTR _INTC_IMRH @0xFFFFFFC8;
#define INTC_IMRH                       _INTC_IMRH.Dword
#define INTC_IMRH_IMR32                 _INTC_IMRH.Bits.IMR32
#define INTC_IMRH_IMR33                 _INTC_IMRH.Bits.IMR33
#define INTC_IMRH_IMR34                 _INTC_IMRH.Bits.IMR34
#define INTC_IMRH_IMR35                 _INTC_IMRH.Bits.IMR35
#define INTC_IMRH_IMR36                 _INTC_IMRH.Bits.IMR36
#define INTC_IMRH_IMR37                 _INTC_IMRH.Bits.IMR37
#define INTC_IMRH_IMR38                 _INTC_IMRH.Bits.IMR38
#define INTC_IMRH_IMR39                 _INTC_IMRH.Bits.IMR39
#define INTC_IMRH_IMR40                 _INTC_IMRH.Bits.IMR40
#define INTC_IMRH_IMR41                 _INTC_IMRH.Bits.IMR41
#define INTC_IMRH_IMR42                 _INTC_IMRH.Bits.IMR42
#define INTC_IMRH_IMR43                 _INTC_IMRH.Bits.IMR43
#define INTC_IMRH_IMR_32                _INTC_IMRH.MergedBits.grpIMR_32
#define INTC_IMRH_IMR                   INTC_IMRH_IMR_32

#define INTC_IMRH_IMR32_MASK            1
#define INTC_IMRH_IMR33_MASK            2
#define INTC_IMRH_IMR34_MASK            4
#define INTC_IMRH_IMR35_MASK            8
#define INTC_IMRH_IMR36_MASK            16
#define INTC_IMRH_IMR37_MASK            32
#define INTC_IMRH_IMR38_MASK            64
#define INTC_IMRH_IMR39_MASK            128
#define INTC_IMRH_IMR40_MASK            256
#define INTC_IMRH_IMR41_MASK            512
#define INTC_IMRH_IMR42_MASK            1024
#define INTC_IMRH_IMR43_MASK            2048
#define INTC_IMRH_IMR_32_MASK           4095
#define INTC_IMRH_IMR_32_BITNUM         0


/*** INTC_IMRL - Interrupt Mask Register Low; 0xFFFFFFCC ***/
typedef union {
  dword Dword;
  struct {
    dword IMR0       :1;                                       /* Interrupt mask register 0 */
    dword IMR1       :1;                                       /* Interrupt mask register 1 */
    dword IMR2       :1;                                       /* Interrupt mask register 2 */
    dword IMR3       :1;                                       /* Interrupt mask register 3 */
    dword IMR4       :1;                                       /* Interrupt mask register 4 */
    dword IMR5       :1;                                       /* Interrupt mask register 5 */
    dword IMR6       :1;                                       /* Interrupt mask register 6 */
    dword IMR7       :1;                                       /* Interrupt mask register 7 */
    dword IMR8       :1;                                       /* Interrupt mask register 8 */
    dword IMR9       :1;                                       /* Interrupt mask register 9 */
    dword IMR10      :1;                                       /* Interrupt mask register 10 */
    dword IMR11      :1;                                       /* Interrupt mask register 11 */
    dword IMR12      :1;                                       /* Interrupt mask register 12 */
    dword IMR13      :1;                                       /* Interrupt mask register 13 */
    dword IMR14      :1;                                       /* Interrupt mask register 14 */
    dword IMR15      :1;                                       /* Interrupt mask register 15 */
    dword IMR16      :1;                                       /* Interrupt mask register 16 */
    dword IMR17      :1;                                       /* Interrupt mask register 17 */
    dword IMR18      :1;                                       /* Interrupt mask register 18 */
    dword IMR19      :1;                                       /* Interrupt mask register 19 */
    dword IMR20      :1;                                       /* Interrupt mask register 20 */
    dword IMR21      :1;                                       /* Interrupt mask register 21 */
    dword IMR22      :1;                                       /* Interrupt mask register 22 */
    dword IMR23      :1;                                       /* Interrupt mask register 23 */
    dword IMR24      :1;                                       /* Interrupt mask register 24 */
    dword IMR25      :1;                                       /* Interrupt mask register 25 */
    dword IMR26      :1;                                       /* Interrupt mask register 26 */
    dword IMR27      :1;                                       /* Interrupt mask register 27 */
    dword IMR28      :1;                                       /* Interrupt mask register 28 */
    dword IMR29      :1;                                       /* Interrupt mask register 29 */
    dword IMR30      :1;                                       /* Interrupt mask register 30 */
    dword IMR31      :1;                                       /* Interrupt mask register 31 */
  } Bits;
} INTC_IMRLSTR;
extern volatile INTC_IMRLSTR _INTC_IMRL @0xFFFFFFCC;
#define INTC_IMRL                       _INTC_IMRL.Dword
#define INTC_IMRL_IMR0                  _INTC_IMRL.Bits.IMR0
#define INTC_IMRL_IMR1                  _INTC_IMRL.Bits.IMR1
#define INTC_IMRL_IMR2                  _INTC_IMRL.Bits.IMR2
#define INTC_IMRL_IMR3                  _INTC_IMRL.Bits.IMR3
#define INTC_IMRL_IMR4                  _INTC_IMRL.Bits.IMR4
#define INTC_IMRL_IMR5                  _INTC_IMRL.Bits.IMR5
#define INTC_IMRL_IMR6                  _INTC_IMRL.Bits.IMR6
#define INTC_IMRL_IMR7                  _INTC_IMRL.Bits.IMR7
#define INTC_IMRL_IMR8                  _INTC_IMRL.Bits.IMR8
#define INTC_IMRL_IMR9                  _INTC_IMRL.Bits.IMR9
#define INTC_IMRL_IMR10                 _INTC_IMRL.Bits.IMR10
#define INTC_IMRL_IMR11                 _INTC_IMRL.Bits.IMR11
#define INTC_IMRL_IMR12                 _INTC_IMRL.Bits.IMR12
#define INTC_IMRL_IMR13                 _INTC_IMRL.Bits.IMR13
#define INTC_IMRL_IMR14                 _INTC_IMRL.Bits.IMR14
#define INTC_IMRL_IMR15                 _INTC_IMRL.Bits.IMR15
#define INTC_IMRL_IMR16                 _INTC_IMRL.Bits.IMR16
#define INTC_IMRL_IMR17                 _INTC_IMRL.Bits.IMR17
#define INTC_IMRL_IMR18                 _INTC_IMRL.Bits.IMR18
#define INTC_IMRL_IMR19                 _INTC_IMRL.Bits.IMR19
#define INTC_IMRL_IMR20                 _INTC_IMRL.Bits.IMR20
#define INTC_IMRL_IMR21                 _INTC_IMRL.Bits.IMR21
#define INTC_IMRL_IMR22                 _INTC_IMRL.Bits.IMR22
#define INTC_IMRL_IMR23                 _INTC_IMRL.Bits.IMR23
#define INTC_IMRL_IMR24                 _INTC_IMRL.Bits.IMR24
#define INTC_IMRL_IMR25                 _INTC_IMRL.Bits.IMR25
#define INTC_IMRL_IMR26                 _INTC_IMRL.Bits.IMR26
#define INTC_IMRL_IMR27                 _INTC_IMRL.Bits.IMR27
#define INTC_IMRL_IMR28                 _INTC_IMRL.Bits.IMR28
#define INTC_IMRL_IMR29                 _INTC_IMRL.Bits.IMR29
#define INTC_IMRL_IMR30                 _INTC_IMRL.Bits.IMR30
#define INTC_IMRL_IMR31                 _INTC_IMRL.Bits.IMR31

#define INTC_IMRL_IMR0_MASK             1
#define INTC_IMRL_IMR1_MASK             2
#define INTC_IMRL_IMR2_MASK             4
#define INTC_IMRL_IMR3_MASK             8
#define INTC_IMRL_IMR4_MASK             16
#define INTC_IMRL_IMR5_MASK             32
#define INTC_IMRL_IMR6_MASK             64
#define INTC_IMRL_IMR7_MASK             128
#define INTC_IMRL_IMR8_MASK             256
#define INTC_IMRL_IMR9_MASK             512
#define INTC_IMRL_IMR10_MASK            1024
#define INTC_IMRL_IMR11_MASK            2048
#define INTC_IMRL_IMR12_MASK            4096
#define INTC_IMRL_IMR13_MASK            8192
#define INTC_IMRL_IMR14_MASK            16384
#define INTC_IMRL_IMR15_MASK            32768
#define INTC_IMRL_IMR16_MASK            65536
#define INTC_IMRL_IMR17_MASK            131072
#define INTC_IMRL_IMR18_MASK            262144
#define INTC_IMRL_IMR19_MASK            524288
#define INTC_IMRL_IMR20_MASK            1048576
#define INTC_IMRL_IMR21_MASK            2097152
#define INTC_IMRL_IMR22_MASK            4194304
#define INTC_IMRL_IMR23_MASK            8388608
#define INTC_IMRL_IMR24_MASK            16777216
#define INTC_IMRL_IMR25_MASK            33554432
#define INTC_IMRL_IMR26_MASK            67108864
#define INTC_IMRL_IMR27_MASK            134217728
#define INTC_IMRL_IMR28_MASK            268435456
#define INTC_IMRL_IMR29_MASK            536870912
#define INTC_IMRL_IMR30_MASK            1073741824
#define INTC_IMRL_IMR31_MASK            2147483648


/*** INTC_FRC - INTC Force Interrupt Register; 0xFFFFFFD0 ***/
typedef union {
  byte Byte;
  struct {
    byte LVL7        :1;                                       /* Force Level 7 interrupt */
    byte LVL6        :1;                                       /* Force Level 6 interrupt */
    byte LVL5        :1;                                       /* Force Level 5 interrupt */
    byte LVL4        :1;                                       /* Force Level 4 interrupt */
    byte LVL3        :1;                                       /* Force Level 3 interrupt */
    byte LVL2        :1;                                       /* Force Level 2 interrupt */
    byte LVL1        :1;                                       /* Force Level 1 interrupt */
    byte             :1; 
  } Bits;
} INTC_FRCSTR;
extern volatile INTC_FRCSTR _INTC_FRC @0xFFFFFFD0;
#define INTC_FRC                        _INTC_FRC.Byte
#define INTC_FRC_LVL7                   _INTC_FRC.Bits.LVL7
#define INTC_FRC_LVL6                   _INTC_FRC.Bits.LVL6
#define INTC_FRC_LVL5                   _INTC_FRC.Bits.LVL5
#define INTC_FRC_LVL4                   _INTC_FRC.Bits.LVL4
#define INTC_FRC_LVL3                   _INTC_FRC.Bits.LVL3
#define INTC_FRC_LVL2                   _INTC_FRC.Bits.LVL2
#define INTC_FRC_LVL1                   _INTC_FRC.Bits.LVL1

#define INTC_FRC_LVL7_MASK              1
#define INTC_FRC_LVL6_MASK              2
#define INTC_FRC_LVL5_MASK              4
#define INTC_FRC_LVL4_MASK              8
#define INTC_FRC_LVL3_MASK              16
#define INTC_FRC_LVL2_MASK              32
#define INTC_FRC_LVL1_MASK              64


/*** INTC_PL6P7 - INTC Programmable Level 6, Priority 7 Register; 0xFFFFFFD8 ***/
typedef union {
  byte Byte;
  struct {
    byte REQN0       :1;                                       /* Request number, bit 0 */
    byte REQN1       :1;                                       /* Request number, bit 1 */
    byte REQN2       :1;                                       /* Request number, bit 2 */
    byte REQN3       :1;                                       /* Request number, bit 3 */
    byte REQN4       :1;                                       /* Request number, bit 4 */
    byte REQN5       :1;                                       /* Request number, bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpREQN :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_PL6P7STR;
extern volatile INTC_PL6P7STR _INTC_PL6P7 @0xFFFFFFD8;
#define INTC_PL6P7                      _INTC_PL6P7.Byte
#define INTC_PL6P7_REQN0                _INTC_PL6P7.Bits.REQN0
#define INTC_PL6P7_REQN1                _INTC_PL6P7.Bits.REQN1
#define INTC_PL6P7_REQN2                _INTC_PL6P7.Bits.REQN2
#define INTC_PL6P7_REQN3                _INTC_PL6P7.Bits.REQN3
#define INTC_PL6P7_REQN4                _INTC_PL6P7.Bits.REQN4
#define INTC_PL6P7_REQN5                _INTC_PL6P7.Bits.REQN5
#define INTC_PL6P7_REQN                 _INTC_PL6P7.MergedBits.grpREQN

#define INTC_PL6P7_REQN0_MASK           1
#define INTC_PL6P7_REQN1_MASK           2
#define INTC_PL6P7_REQN2_MASK           4
#define INTC_PL6P7_REQN3_MASK           8
#define INTC_PL6P7_REQN4_MASK           16
#define INTC_PL6P7_REQN5_MASK           32
#define INTC_PL6P7_REQN_MASK            63
#define INTC_PL6P7_REQN_BITNUM          0


/*** INTC_PL6P6 - INTC Programmable Level 6, Priority 6 Register; 0xFFFFFFD9 ***/
typedef union {
  byte Byte;
  struct {
    byte REQN0       :1;                                       /* Request number, bit 0 */
    byte REQN1       :1;                                       /* Request number, bit 1 */
    byte REQN2       :1;                                       /* Request number, bit 2 */
    byte REQN3       :1;                                       /* Request number, bit 3 */
    byte REQN4       :1;                                       /* Request number, bit 4 */
    byte REQN5       :1;                                       /* Request number, bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpREQN :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_PL6P6STR;
extern volatile INTC_PL6P6STR _INTC_PL6P6 @0xFFFFFFD9;
#define INTC_PL6P6                      _INTC_PL6P6.Byte
#define INTC_PL6P6_REQN0                _INTC_PL6P6.Bits.REQN0
#define INTC_PL6P6_REQN1                _INTC_PL6P6.Bits.REQN1
#define INTC_PL6P6_REQN2                _INTC_PL6P6.Bits.REQN2
#define INTC_PL6P6_REQN3                _INTC_PL6P6.Bits.REQN3
#define INTC_PL6P6_REQN4                _INTC_PL6P6.Bits.REQN4
#define INTC_PL6P6_REQN5                _INTC_PL6P6.Bits.REQN5
#define INTC_PL6P6_REQN                 _INTC_PL6P6.MergedBits.grpREQN

#define INTC_PL6P6_REQN0_MASK           1
#define INTC_PL6P6_REQN1_MASK           2
#define INTC_PL6P6_REQN2_MASK           4
#define INTC_PL6P6_REQN3_MASK           8
#define INTC_PL6P6_REQN4_MASK           16
#define INTC_PL6P6_REQN5_MASK           32
#define INTC_PL6P6_REQN_MASK            63
#define INTC_PL6P6_REQN_BITNUM          0


/*** INTC_WCR - INTC Wake-up Control Register; 0xFFFFFFDB ***/
typedef union {
  byte Byte;
  struct {
    byte MASK0       :1;                                       /* Interrupt mask level, bit 0 */
    byte MASK1       :1;                                       /* Interrupt mask level, bit 1 */
    byte MASK2       :1;                                       /* Interrupt mask level, bit 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte ENB         :1;                                       /* Enable */
  } Bits;
  struct {
    byte grpMASK :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_WCRSTR;
extern volatile INTC_WCRSTR _INTC_WCR @0xFFFFFFDB;
#define INTC_WCR                        _INTC_WCR.Byte
#define INTC_WCR_MASK0                  _INTC_WCR.Bits.MASK0
#define INTC_WCR_MASK1                  _INTC_WCR.Bits.MASK1
#define INTC_WCR_MASK2                  _INTC_WCR.Bits.MASK2
#define INTC_WCR_ENB                    _INTC_WCR.Bits.ENB
#define INTC_WCR_MASK                   _INTC_WCR.MergedBits.grpMASK

#define INTC_WCR_MASK0_MASK             1
#define INTC_WCR_MASK1_MASK             2
#define INTC_WCR_MASK2_MASK             4
#define INTC_WCR_ENB_MASK               128
#define INTC_WCR_MASK_MASK              7
#define INTC_WCR_MASK_BITNUM            0


/*** INTC_SIMR - Set Interrupt Mask Register; 0xFFFFFFDC ***/
typedef union {
  byte Byte;
  struct {
    byte SIMR0       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 0 */
    byte SIMR1       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 1 */
    byte SIMR2       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 2 */
    byte SIMR3       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 3 */
    byte SIMR4       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 4 */
    byte SIMR5       :1;                                       /* Set the corresponding bit in the INTC_IMR{H,L} register, masking the interrupt request, bit 5 */
    byte SALL        :1;                                       /* Set all bits in the INTC_IMR{H,L} register, masking all interrupt requests */
    byte             :1; 
  } Bits;
  struct {
    byte grpSIMR :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_SIMRSTR;
extern volatile INTC_SIMRSTR _INTC_SIMR @0xFFFFFFDC;
#define INTC_SIMR                       _INTC_SIMR.Byte
#define INTC_SIMR_SIMR0                 _INTC_SIMR.Bits.SIMR0
#define INTC_SIMR_SIMR1                 _INTC_SIMR.Bits.SIMR1
#define INTC_SIMR_SIMR2                 _INTC_SIMR.Bits.SIMR2
#define INTC_SIMR_SIMR3                 _INTC_SIMR.Bits.SIMR3
#define INTC_SIMR_SIMR4                 _INTC_SIMR.Bits.SIMR4
#define INTC_SIMR_SIMR5                 _INTC_SIMR.Bits.SIMR5
#define INTC_SIMR_SALL                  _INTC_SIMR.Bits.SALL
#define INTC_SIMR_SIMR                  _INTC_SIMR.MergedBits.grpSIMR

#define INTC_SIMR_SIMR0_MASK            1
#define INTC_SIMR_SIMR1_MASK            2
#define INTC_SIMR_SIMR2_MASK            4
#define INTC_SIMR_SIMR3_MASK            8
#define INTC_SIMR_SIMR4_MASK            16
#define INTC_SIMR_SIMR5_MASK            32
#define INTC_SIMR_SALL_MASK             64
#define INTC_SIMR_SIMR_MASK             63
#define INTC_SIMR_SIMR_BITNUM           0


/*** INTC_CIMR - Clear Interrupt Mask Register; 0xFFFFFFDD ***/
typedef union {
  byte Byte;
  struct {
    byte CIMR0       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 0 */
    byte CIMR1       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 1 */
    byte CIMR2       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 2 */
    byte CIMR3       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 3 */
    byte CIMR4       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 4 */
    byte CIMR5       :1;                                       /* Clear the corresponding bit in the INTC_IMR{H,L} registers, enabling the interrupt request, bit 5 */
    byte CALL        :1;                                       /* Clear all bits in the INTC_IMR{H,L} register, enabling all interrupt requests */
    byte             :1; 
  } Bits;
  struct {
    byte grpCIMR :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_CIMRSTR;
extern volatile INTC_CIMRSTR _INTC_CIMR @0xFFFFFFDD;
#define INTC_CIMR                       _INTC_CIMR.Byte
#define INTC_CIMR_CIMR0                 _INTC_CIMR.Bits.CIMR0
#define INTC_CIMR_CIMR1                 _INTC_CIMR.Bits.CIMR1
#define INTC_CIMR_CIMR2                 _INTC_CIMR.Bits.CIMR2
#define INTC_CIMR_CIMR3                 _INTC_CIMR.Bits.CIMR3
#define INTC_CIMR_CIMR4                 _INTC_CIMR.Bits.CIMR4
#define INTC_CIMR_CIMR5                 _INTC_CIMR.Bits.CIMR5
#define INTC_CIMR_CALL                  _INTC_CIMR.Bits.CALL
#define INTC_CIMR_CIMR                  _INTC_CIMR.MergedBits.grpCIMR

#define INTC_CIMR_CIMR0_MASK            1
#define INTC_CIMR_CIMR1_MASK            2
#define INTC_CIMR_CIMR2_MASK            4
#define INTC_CIMR_CIMR3_MASK            8
#define INTC_CIMR_CIMR4_MASK            16
#define INTC_CIMR_CIMR5_MASK            32
#define INTC_CIMR_CALL_MASK             64
#define INTC_CIMR_CIMR_MASK             63
#define INTC_CIMR_CIMR_BITNUM           0


/*** INTC_SFRC - INTC Set Interrupt Force Register; 0xFFFFFFDE ***/
typedef union {
  byte Byte;
  struct {
    byte SET0        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 0 */
    byte SET1        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 1 */
    byte SET2        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 2 */
    byte SET3        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 3 */
    byte SET4        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 4 */
    byte SET5        :1;                                       /* Sets corresponding bits in the INTC_FRC register, bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpSET  :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_SFRCSTR;
extern volatile INTC_SFRCSTR _INTC_SFRC @0xFFFFFFDE;
#define INTC_SFRC                       _INTC_SFRC.Byte
#define INTC_SFRC_SET0                  _INTC_SFRC.Bits.SET0
#define INTC_SFRC_SET1                  _INTC_SFRC.Bits.SET1
#define INTC_SFRC_SET2                  _INTC_SFRC.Bits.SET2
#define INTC_SFRC_SET3                  _INTC_SFRC.Bits.SET3
#define INTC_SFRC_SET4                  _INTC_SFRC.Bits.SET4
#define INTC_SFRC_SET5                  _INTC_SFRC.Bits.SET5
#define INTC_SFRC_SET                   _INTC_SFRC.MergedBits.grpSET

#define INTC_SFRC_SET0_MASK             1
#define INTC_SFRC_SET1_MASK             2
#define INTC_SFRC_SET2_MASK             4
#define INTC_SFRC_SET3_MASK             8
#define INTC_SFRC_SET4_MASK             16
#define INTC_SFRC_SET5_MASK             32
#define INTC_SFRC_SET_MASK              63
#define INTC_SFRC_SET_BITNUM            0


/*** INTC_CFRC - INTC Clear Interrupt Force Register; 0xFFFFFFDF ***/
typedef union {
  byte Byte;
  struct {
    byte CLR0        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 0 */
    byte CLR1        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 1 */
    byte CLR2        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 2 */
    byte CLR3        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 3 */
    byte CLR4        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 4 */
    byte CLR5        :1;                                       /* Clears corresponding bits in the INTC_FRC register, bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpCLR  :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} INTC_CFRCSTR;
extern volatile INTC_CFRCSTR _INTC_CFRC @0xFFFFFFDF;
#define INTC_CFRC                       _INTC_CFRC.Byte
#define INTC_CFRC_CLR0                  _INTC_CFRC.Bits.CLR0
#define INTC_CFRC_CLR1                  _INTC_CFRC.Bits.CLR1
#define INTC_CFRC_CLR2                  _INTC_CFRC.Bits.CLR2
#define INTC_CFRC_CLR3                  _INTC_CFRC.Bits.CLR3
#define INTC_CFRC_CLR4                  _INTC_CFRC.Bits.CLR4
#define INTC_CFRC_CLR5                  _INTC_CFRC.Bits.CLR5
#define INTC_CFRC_CLR                   _INTC_CFRC.MergedBits.grpCLR

#define INTC_CFRC_CLR0_MASK             1
#define INTC_CFRC_CLR1_MASK             2
#define INTC_CFRC_CLR2_MASK             4
#define INTC_CFRC_CLR3_MASK             8
#define INTC_CFRC_CLR4_MASK             16
#define INTC_CFRC_CLR5_MASK             32
#define INTC_CFRC_CLR_MASK              63
#define INTC_CFRC_CLR_BITNUM            0


/*** INTC_SWIACK - INTC Software IACK Register; 0xFFFFFFE0 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_SWIACKSTR;
extern volatile INTC_SWIACKSTR _INTC_SWIACK @0xFFFFFFE0;
#define INTC_SWIACK                     _INTC_SWIACK.Byte
#define INTC_SWIACK_VECN0               _INTC_SWIACK.Bits.VECN0
#define INTC_SWIACK_VECN1               _INTC_SWIACK.Bits.VECN1
#define INTC_SWIACK_VECN2               _INTC_SWIACK.Bits.VECN2
#define INTC_SWIACK_VECN3               _INTC_SWIACK.Bits.VECN3
#define INTC_SWIACK_VECN4               _INTC_SWIACK.Bits.VECN4
#define INTC_SWIACK_VECN5               _INTC_SWIACK.Bits.VECN5
#define INTC_SWIACK_VECN6               _INTC_SWIACK.Bits.VECN6
#define INTC_SWIACK_VECN                _INTC_SWIACK.MergedBits.grpVECN

#define INTC_SWIACK_VECN0_MASK          1
#define INTC_SWIACK_VECN1_MASK          2
#define INTC_SWIACK_VECN2_MASK          4
#define INTC_SWIACK_VECN3_MASK          8
#define INTC_SWIACK_VECN4_MASK          16
#define INTC_SWIACK_VECN5_MASK          32
#define INTC_SWIACK_VECN6_MASK          64
#define INTC_SWIACK_VECN_MASK           127
#define INTC_SWIACK_VECN_BITNUM         0


/*** INTC_LVL1IACK - INTC Level 1 IACK Register; 0xFFFFFFE4 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL1IACKSTR;
extern volatile INTC_LVL1IACKSTR _INTC_LVL1IACK @0xFFFFFFE4;
#define INTC_LVL1IACK                   _INTC_LVL1IACK.Byte
#define INTC_LVL1IACK_VECN0             _INTC_LVL1IACK.Bits.VECN0
#define INTC_LVL1IACK_VECN1             _INTC_LVL1IACK.Bits.VECN1
#define INTC_LVL1IACK_VECN2             _INTC_LVL1IACK.Bits.VECN2
#define INTC_LVL1IACK_VECN3             _INTC_LVL1IACK.Bits.VECN3
#define INTC_LVL1IACK_VECN4             _INTC_LVL1IACK.Bits.VECN4
#define INTC_LVL1IACK_VECN5             _INTC_LVL1IACK.Bits.VECN5
#define INTC_LVL1IACK_VECN6             _INTC_LVL1IACK.Bits.VECN6
#define INTC_LVL1IACK_VECN              _INTC_LVL1IACK.MergedBits.grpVECN

#define INTC_LVL1IACK_VECN0_MASK        1
#define INTC_LVL1IACK_VECN1_MASK        2
#define INTC_LVL1IACK_VECN2_MASK        4
#define INTC_LVL1IACK_VECN3_MASK        8
#define INTC_LVL1IACK_VECN4_MASK        16
#define INTC_LVL1IACK_VECN5_MASK        32
#define INTC_LVL1IACK_VECN6_MASK        64
#define INTC_LVL1IACK_VECN_MASK         127
#define INTC_LVL1IACK_VECN_BITNUM       0


/*** INTC_LVL2IACK - INTC Level 2 IACK Register; 0xFFFFFFE8 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL2IACKSTR;
extern volatile INTC_LVL2IACKSTR _INTC_LVL2IACK @0xFFFFFFE8;
#define INTC_LVL2IACK                   _INTC_LVL2IACK.Byte
#define INTC_LVL2IACK_VECN0             _INTC_LVL2IACK.Bits.VECN0
#define INTC_LVL2IACK_VECN1             _INTC_LVL2IACK.Bits.VECN1
#define INTC_LVL2IACK_VECN2             _INTC_LVL2IACK.Bits.VECN2
#define INTC_LVL2IACK_VECN3             _INTC_LVL2IACK.Bits.VECN3
#define INTC_LVL2IACK_VECN4             _INTC_LVL2IACK.Bits.VECN4
#define INTC_LVL2IACK_VECN5             _INTC_LVL2IACK.Bits.VECN5
#define INTC_LVL2IACK_VECN6             _INTC_LVL2IACK.Bits.VECN6
#define INTC_LVL2IACK_VECN              _INTC_LVL2IACK.MergedBits.grpVECN

#define INTC_LVL2IACK_VECN0_MASK        1
#define INTC_LVL2IACK_VECN1_MASK        2
#define INTC_LVL2IACK_VECN2_MASK        4
#define INTC_LVL2IACK_VECN3_MASK        8
#define INTC_LVL2IACK_VECN4_MASK        16
#define INTC_LVL2IACK_VECN5_MASK        32
#define INTC_LVL2IACK_VECN6_MASK        64
#define INTC_LVL2IACK_VECN_MASK         127
#define INTC_LVL2IACK_VECN_BITNUM       0


/*** INTC_LVL3IACK - INTC Level 3 IACK Register; 0xFFFFFFEC ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL3IACKSTR;
extern volatile INTC_LVL3IACKSTR _INTC_LVL3IACK @0xFFFFFFEC;
#define INTC_LVL3IACK                   _INTC_LVL3IACK.Byte
#define INTC_LVL3IACK_VECN0             _INTC_LVL3IACK.Bits.VECN0
#define INTC_LVL3IACK_VECN1             _INTC_LVL3IACK.Bits.VECN1
#define INTC_LVL3IACK_VECN2             _INTC_LVL3IACK.Bits.VECN2
#define INTC_LVL3IACK_VECN3             _INTC_LVL3IACK.Bits.VECN3
#define INTC_LVL3IACK_VECN4             _INTC_LVL3IACK.Bits.VECN4
#define INTC_LVL3IACK_VECN5             _INTC_LVL3IACK.Bits.VECN5
#define INTC_LVL3IACK_VECN6             _INTC_LVL3IACK.Bits.VECN6
#define INTC_LVL3IACK_VECN              _INTC_LVL3IACK.MergedBits.grpVECN

#define INTC_LVL3IACK_VECN0_MASK        1
#define INTC_LVL3IACK_VECN1_MASK        2
#define INTC_LVL3IACK_VECN2_MASK        4
#define INTC_LVL3IACK_VECN3_MASK        8
#define INTC_LVL3IACK_VECN4_MASK        16
#define INTC_LVL3IACK_VECN5_MASK        32
#define INTC_LVL3IACK_VECN6_MASK        64
#define INTC_LVL3IACK_VECN_MASK         127
#define INTC_LVL3IACK_VECN_BITNUM       0


/*** INTC_LVL4IACK - INTC Level 4 IACK Register; 0xFFFFFFF0 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL4IACKSTR;
extern volatile INTC_LVL4IACKSTR _INTC_LVL4IACK @0xFFFFFFF0;
#define INTC_LVL4IACK                   _INTC_LVL4IACK.Byte
#define INTC_LVL4IACK_VECN0             _INTC_LVL4IACK.Bits.VECN0
#define INTC_LVL4IACK_VECN1             _INTC_LVL4IACK.Bits.VECN1
#define INTC_LVL4IACK_VECN2             _INTC_LVL4IACK.Bits.VECN2
#define INTC_LVL4IACK_VECN3             _INTC_LVL4IACK.Bits.VECN3
#define INTC_LVL4IACK_VECN4             _INTC_LVL4IACK.Bits.VECN4
#define INTC_LVL4IACK_VECN5             _INTC_LVL4IACK.Bits.VECN5
#define INTC_LVL4IACK_VECN6             _INTC_LVL4IACK.Bits.VECN6
#define INTC_LVL4IACK_VECN              _INTC_LVL4IACK.MergedBits.grpVECN

#define INTC_LVL4IACK_VECN0_MASK        1
#define INTC_LVL4IACK_VECN1_MASK        2
#define INTC_LVL4IACK_VECN2_MASK        4
#define INTC_LVL4IACK_VECN3_MASK        8
#define INTC_LVL4IACK_VECN4_MASK        16
#define INTC_LVL4IACK_VECN5_MASK        32
#define INTC_LVL4IACK_VECN6_MASK        64
#define INTC_LVL4IACK_VECN_MASK         127
#define INTC_LVL4IACK_VECN_BITNUM       0


/*** INTC_LVL5IACK - INTC Level 5 IACK Register; 0xFFFFFFF4 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL5IACKSTR;
extern volatile INTC_LVL5IACKSTR _INTC_LVL5IACK @0xFFFFFFF4;
#define INTC_LVL5IACK                   _INTC_LVL5IACK.Byte
#define INTC_LVL5IACK_VECN0             _INTC_LVL5IACK.Bits.VECN0
#define INTC_LVL5IACK_VECN1             _INTC_LVL5IACK.Bits.VECN1
#define INTC_LVL5IACK_VECN2             _INTC_LVL5IACK.Bits.VECN2
#define INTC_LVL5IACK_VECN3             _INTC_LVL5IACK.Bits.VECN3
#define INTC_LVL5IACK_VECN4             _INTC_LVL5IACK.Bits.VECN4
#define INTC_LVL5IACK_VECN5             _INTC_LVL5IACK.Bits.VECN5
#define INTC_LVL5IACK_VECN6             _INTC_LVL5IACK.Bits.VECN6
#define INTC_LVL5IACK_VECN              _INTC_LVL5IACK.MergedBits.grpVECN

#define INTC_LVL5IACK_VECN0_MASK        1
#define INTC_LVL5IACK_VECN1_MASK        2
#define INTC_LVL5IACK_VECN2_MASK        4
#define INTC_LVL5IACK_VECN3_MASK        8
#define INTC_LVL5IACK_VECN4_MASK        16
#define INTC_LVL5IACK_VECN5_MASK        32
#define INTC_LVL5IACK_VECN6_MASK        64
#define INTC_LVL5IACK_VECN_MASK         127
#define INTC_LVL5IACK_VECN_BITNUM       0


/*** INTC_LVL6IACK - INTC Level 6 IACK Register; 0xFFFFFFF8 ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL6IACKSTR;
extern volatile INTC_LVL6IACKSTR _INTC_LVL6IACK @0xFFFFFFF8;
#define INTC_LVL6IACK                   _INTC_LVL6IACK.Byte
#define INTC_LVL6IACK_VECN0             _INTC_LVL6IACK.Bits.VECN0
#define INTC_LVL6IACK_VECN1             _INTC_LVL6IACK.Bits.VECN1
#define INTC_LVL6IACK_VECN2             _INTC_LVL6IACK.Bits.VECN2
#define INTC_LVL6IACK_VECN3             _INTC_LVL6IACK.Bits.VECN3
#define INTC_LVL6IACK_VECN4             _INTC_LVL6IACK.Bits.VECN4
#define INTC_LVL6IACK_VECN5             _INTC_LVL6IACK.Bits.VECN5
#define INTC_LVL6IACK_VECN6             _INTC_LVL6IACK.Bits.VECN6
#define INTC_LVL6IACK_VECN              _INTC_LVL6IACK.MergedBits.grpVECN

#define INTC_LVL6IACK_VECN0_MASK        1
#define INTC_LVL6IACK_VECN1_MASK        2
#define INTC_LVL6IACK_VECN2_MASK        4
#define INTC_LVL6IACK_VECN3_MASK        8
#define INTC_LVL6IACK_VECN4_MASK        16
#define INTC_LVL6IACK_VECN5_MASK        32
#define INTC_LVL6IACK_VECN6_MASK        64
#define INTC_LVL6IACK_VECN_MASK         127
#define INTC_LVL6IACK_VECN_BITNUM       0


/*** INTC_LVL7IACK - INTC Level 7 IACK Register; 0xFFFFFFFC ***/
typedef union {
  byte Byte;
  struct {
    byte VECN0       :1;                                       /* Vector number, bit 0 */
    byte VECN1       :1;                                       /* Vector number, bit 1 */
    byte VECN2       :1;                                       /* Vector number, bit 2 */
    byte VECN3       :1;                                       /* Vector number, bit 3 */
    byte VECN4       :1;                                       /* Vector number, bit 4 */
    byte VECN5       :1;                                       /* Vector number, bit 5 */
    byte VECN6       :1;                                       /* Vector number, bit 6 */
    byte             :1; 
  } Bits;
  struct {
    byte grpVECN :7;
    byte         :1;
  } MergedBits;
} INTC_LVL7IACKSTR;
extern volatile INTC_LVL7IACKSTR _INTC_LVL7IACK @0xFFFFFFFC;
#define INTC_LVL7IACK                   _INTC_LVL7IACK.Byte
#define INTC_LVL7IACK_VECN0             _INTC_LVL7IACK.Bits.VECN0
#define INTC_LVL7IACK_VECN1             _INTC_LVL7IACK.Bits.VECN1
#define INTC_LVL7IACK_VECN2             _INTC_LVL7IACK.Bits.VECN2
#define INTC_LVL7IACK_VECN3             _INTC_LVL7IACK.Bits.VECN3
#define INTC_LVL7IACK_VECN4             _INTC_LVL7IACK.Bits.VECN4
#define INTC_LVL7IACK_VECN5             _INTC_LVL7IACK.Bits.VECN5
#define INTC_LVL7IACK_VECN6             _INTC_LVL7IACK.Bits.VECN6
#define INTC_LVL7IACK_VECN              _INTC_LVL7IACK.MergedBits.grpVECN

#define INTC_LVL7IACK_VECN0_MASK        1
#define INTC_LVL7IACK_VECN1_MASK        2
#define INTC_LVL7IACK_VECN2_MASK        4
#define INTC_LVL7IACK_VECN3_MASK        8
#define INTC_LVL7IACK_VECN4_MASK        16
#define INTC_LVL7IACK_VECN5_MASK        32
#define INTC_LVL7IACK_VECN6_MASK        64
#define INTC_LVL7IACK_VECN_MASK         127
#define INTC_LVL7IACK_VECN_BITNUM       0



/* Flash commands */
#define mBlank                          0x05
#define mBurstProg                      0x25
#define mByteProg                       0x20
#define mMassErase                      0x41
#define mPageErase                      0x40


/***********************************************/
/**   D E P R E C I A T E D   S Y M B O L S   **/
/***********************************************/
#pragma options align=reset

extern volatile byte _RST_COPY_VALUE @0x00803FFF;

#endif
