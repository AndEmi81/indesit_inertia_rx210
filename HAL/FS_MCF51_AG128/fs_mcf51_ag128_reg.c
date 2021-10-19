/* Based on CPU DB MCF51AG128_80, version 3.00.000 (RegistersPrg V2.29) */
/* DataSheet : MCF51AG128RM Rev. 3 5/2009 */

#include "fs_mcf51_ag128_reg.h"

/*lint -save -esym(765, *) */


/* * * * *  8-BIT REGISTERS  * * * * * * * * * * * * * * * */
/* NVFTRIM - macro for reading non volatile register       Nonvolatile ICS Fine Trim; 0x000003FE */
/* Tip for register initialization in the user code:  const byte NVFTRIM_INIT @0x000003FE = <NVFTRIM_INITVAL>; */
/* NVICSTRM - macro for reading non volatile register      Nonvolatile ICS Trim Register; 0x000003FF */
/* Tip for register initialization in the user code:  const byte NVICSTRM_INIT @0x000003FF = <NVICSTRM_INITVAL>; */
/* NVBACKKEY0 - macro for reading non volatile register    Backdoor Comparison Key 0; 0x00000400 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY0_INIT @0x00000400 = <NVBACKKEY0_INITVAL>; */
/* NVBACKKEY1 - macro for reading non volatile register    Backdoor Comparison Key 1; 0x00000401 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY1_INIT @0x00000401 = <NVBACKKEY1_INITVAL>; */
/* NVBACKKEY2 - macro for reading non volatile register    Backdoor Comparison Key 2; 0x00000402 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY2_INIT @0x00000402 = <NVBACKKEY2_INITVAL>; */
/* NVBACKKEY3 - macro for reading non volatile register    Backdoor Comparison Key 3; 0x00000403 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY3_INIT @0x00000403 = <NVBACKKEY3_INITVAL>; */
/* NVBACKKEY4 - macro for reading non volatile register    Backdoor Comparison Key 4; 0x00000404 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY4_INIT @0x00000404 = <NVBACKKEY4_INITVAL>; */
/* NVBACKKEY5 - macro for reading non volatile register    Backdoor Comparison Key 5; 0x00000405 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY5_INIT @0x00000405 = <NVBACKKEY5_INITVAL>; */
/* NVBACKKEY6 - macro for reading non volatile register    Backdoor Comparison Key 6; 0x00000406 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY6_INIT @0x00000406 = <NVBACKKEY6_INITVAL>; */
/* NVBACKKEY7 - macro for reading non volatile register    Backdoor Comparison Key 7; 0x00000407 */
/* Tip for register initialization in the user code:  const byte NVBACKKEY7_INIT @0x00000407 = <NVBACKKEY7_INITVAL>; */
/* NVPROT - macro for reading non volatile register        Nonvolatile Flash Protection Register; 0x0000040D */
/* Tip for register initialization in the user code:  const byte NVPROT_INIT @0x0000040D = <NVPROT_INITVAL>; */
/* NVOPT - macro for reading non volatile register         Nonvolatile Flash Options Register; 0x0000040F */
/* Tip for register initialization in the user code:  const byte NVOPT_INIT @0x0000040F = <NVOPT_INITVAL>; */
volatile PTADSTR _PTAD;                                    /* Port A Data Register; 0xFFFF8000 */
volatile PTADDSTR _PTADD;                                  /* Port A Data Direction Register; 0xFFFF8001 */
volatile PTAPVSTR _PTAPV;                                  /* Port A Pin Value Register; 0xFFFF8002 */
volatile PTBDSTR _PTBD;                                    /* Port B Data Register; 0xFFFF8004 */
volatile PTBDDSTR _PTBDD;                                  /* Port B Data Direction Register; 0xFFFF8005 */
volatile PTBPVSTR _PTBPV;                                  /* Port B Pin Value Register; 0xFFFF8006 */
volatile PTCDSTR _PTCD;                                    /* Port C Data Register; 0xFFFF8008 */
volatile PTCDDSTR _PTCDD;                                  /* Port C Data Direction Register; 0xFFFF8009 */
volatile PTCPVSTR _PTCPV;                                  /* Port C Pin Value Register; 0xFFFF800A */
volatile PTDDSTR _PTDD;                                    /* Port D Data Register; 0xFFFF800C */
volatile PTDDDSTR _PTDDD;                                  /* Port D Data Direction Register; 0xFFFF800D */
volatile PTDPVSTR _PTDPV;                                  /* Port D Pin Value Register; 0xFFFF800E */
volatile PTEDSTR _PTED;                                    /* Port E Data Register; 0xFFFF8010 */
volatile PTEDDSTR _PTEDD;                                  /* Port E Data Direction Register; 0xFFFF8011 */
volatile PTEPVSTR _PTEPV;                                  /* Port E Pin Value Register; 0xFFFF8012 */
volatile PTFDSTR _PTFD;                                    /* Port F Data Register; 0xFFFF8014 */
volatile PTFDDSTR _PTFDD;                                  /* Port F Data Direction Register; 0xFFFF8015 */
volatile PTFPVSTR _PTFPV;                                  /* Port F Pin Value Register; 0xFFFF8016 */
volatile PTGDSTR _PTGD;                                    /* Port G Data Register; 0xFFFF8018 */
volatile PTGDDSTR _PTGDD;                                  /* Port G Data Direction Register; 0xFFFF8019 */
volatile PTGPVSTR _PTGPV;                                  /* Port G Pin Value Register; 0xFFFF801A */
volatile PTHDSTR _PTHD;                                    /* Port H Data Register; 0xFFFF801C */
volatile PTHDDSTR _PTHDD;                                  /* Port H Data Direction Register; 0xFFFF801D */
volatile PTHPVSTR _PTHPV;                                  /* Port H Pin Value Register; 0xFFFF801E */
volatile PTJDSTR _PTJD;                                    /* Port J Data Register; 0xFFFF8020 */
volatile PTJDDSTR _PTJDD;                                  /* Port J Data Direction Register; 0xFFFF8021 */
volatile PTJPVSTR _PTJPV;                                  /* Port J Pin Value Register; 0xFFFF8022 */
volatile ICSC1STR _ICSC1;                                  /* ICS Control Register 1; 0xFFFF8024 */
volatile ICSC2STR _ICSC2;                                  /* ICS Control Register 2; 0xFFFF8025 */
volatile ICSTRMSTR _ICSTRM;                                /* ICS Trim Register; 0xFFFF8026 */
volatile ICSSCSTR _ICSSC;                                  /* ICS Status and Control Register; 0xFFFF8027 */
volatile RTCSCSTR _RTCSC;                                  /* RTC Status and Control Register; 0xFFFF8028 */
volatile RTCCNTSTR _RTCCNT;                                /* RTC Counter Register; 0xFFFF8029 */
volatile RTCMODSTR _RTCMOD;                                /* RTC Modulo Register; 0xFFFF802A */
volatile RTCSC1STR _RTCSC1;                                /* RTC DMA Enable Register; 0xFFFF802B */
volatile DAC1CTRLSTR _DAC1CTRL;                            /* DAC Control Register; 0xFFFF802D */
volatile DAC2CTRLSTR _DAC2CTRL;                            /* DAC Control Register; 0xFFFF802E */
volatile SCI1C1STR _SCI1C1;                                /* SCI1 Control Register 1; 0xFFFF8052 */
volatile SCI1C2STR _SCI1C2;                                /* SCI1 Control Register 2; 0xFFFF8053 */
volatile SCI1S1STR _SCI1S1;                                /* SCI1 Status Register 1; 0xFFFF8054 */
volatile SCI1S2STR _SCI1S2;                                /* SCI1 Status Register 2; 0xFFFF8055 */
volatile SCI1C3STR _SCI1C3;                                /* SCI1 Control Register 3; 0xFFFF8056 */
volatile SCI1DSTR _SCI1D;                                  /* SCI1 Data Register; 0xFFFF8057 */
volatile SCI1MA1STR _SCI1MA1;                              /* SCI1 Match Address Registers 1; 0xFFFF8058 */
volatile SCI1MA2STR _SCI1MA2;                              /* SCI1 Match Address Registers 2; 0xFFFF8059 */
volatile SCI1C4STR _SCI1C4;                                /* SCI1 Control Register 4; 0xFFFF805A */
volatile SCI1C5STR _SCI1C5;                                /* SCI1 Control Register 5; 0xFFFF805B */
volatile SCI2C1STR _SCI2C1;                                /* SCI2 Control Register 1; 0xFFFF8062 */
volatile SCI2C2STR _SCI2C2;                                /* SCI2 Control Register 2; 0xFFFF8063 */
volatile SCI2S1STR _SCI2S1;                                /* SCI2 Status Register 1; 0xFFFF8064 */
volatile SCI2S2STR _SCI2S2;                                /* SCI2 Status Register 2; 0xFFFF8065 */
volatile SCI2C3STR _SCI2C3;                                /* SCI2 Control Register 3; 0xFFFF8066 */
volatile SCI2DSTR _SCI2D;                                  /* SCI2 Data Register; 0xFFFF8067 */
volatile SCI2MA1STR _SCI2MA1;                              /* SCI2 Match Address Registers 1; 0xFFFF8068 */
volatile SCI2MA2STR _SCI2MA2;                              /* SCI2 Match Address Registers 2; 0xFFFF8069 */
volatile SCI2C4STR _SCI2C4;                                /* SCI2 Control Register 4; 0xFFFF806A */
volatile SCI2C5STR _SCI2C5;                                /* SCI2 Control Register 5; 0xFFFF806B */
volatile TPM3SCSTR _TPM3SC;                                /* TPM3 Status and Control Register; 0xFFFF8070 */
volatile TPM3C0SCSTR _TPM3C0SC;                            /* TPM3 Timer Channel 0 Status and Control Register; 0xFFFF8075 */
volatile TPM3C1SCSTR _TPM3C1SC;                            /* TPM3 Timer Channel 1 Status and Control Register; 0xFFFF8078 */
volatile FTM1SCSTR _FTM1SC;                                /* FTM1 Status and Control Register; 0xFFFF8080 */
volatile FTM1C0SCSTR _FTM1C0SC;                            /* FTM1 Timer Channel 0 Status and Control Register; 0xFFFF8085 */
volatile FTM1C1SCSTR _FTM1C1SC;                            /* FTM1 Timer Channel 1 Status and Control Register; 0xFFFF8088 */
volatile FTM1C2SCSTR _FTM1C2SC;                            /* FTM1 Timer Channel 2 Status and Control Register; 0xFFFF808B */
volatile FTM1C3SCSTR _FTM1C3SC;                            /* FTM1 Timer Channel 3 Status and Control Register; 0xFFFF808E */
volatile FTM1C4SCSTR _FTM1C4SC;                            /* FTM1 Timer Channel 4 Status and Control Register; 0xFFFF8091 */
volatile FTM1C5SCSTR _FTM1C5SC;                            /* FTM1 Timer Channel 5 Status and Control Register; 0xFFFF8094 */
volatile FTM1STATUSSTR _FTM1STATUS;                        /* FTM1 Capture and Compare Status Register; 0xFFFF80A2 */
volatile FTM1MODESTR _FTM1MODE;                            /* FTM1 Features Mode Selection Register; 0xFFFF80A3 */
volatile FTM1SYNCSTR _FTM1SYNC;                            /* FTM1 Synchronization Register; 0xFFFF80A4 */
volatile FTM1OUTINITSTR _FTM1OUTINIT;                      /* FTM1 Initial State for Channels Output Register; 0xFFFF80A5 */
volatile FTM1OUTMASKSTR _FTM1OUTMASK;                      /* FTM1 Output Mask Register; 0xFFFF80A6 */
volatile FTM1COMBINE0STR _FTM1COMBINE0;                    /* FTM1 Function For Linked Channel 0 Register; 0xFFFF80A7 */
volatile FTM1COMBINE1STR _FTM1COMBINE1;                    /* FTM1 Function For Linked Channel 1 Register; 0xFFFF80A8 */
volatile FTM1COMBINE2STR _FTM1COMBINE2;                    /* FTM1 Function For Linked Channel 2 Register; 0xFFFF80A9 */
volatile FTM1DEADTIMESTR _FTM1DEADTIME;                    /* FTM1 Deadtime Insertion Control Register; 0xFFFF80AB */
volatile FTM1EXTTRIGSTR _FTM1EXTTRIG;                      /* FTM1 ExternalTrigger Register; 0xFFFF80AC */
volatile FTM1POLSTR _FTM1POL;                              /* FTM1 Channels Polarity Register; 0xFFFF80AD */
volatile FTM1FMSSTR _FTM1FMS;                              /* FTM1 Fault Mode Status Register; 0xFFFF80AE */
volatile FTM1FILTER0STR _FTM1FILTER0;                      /* FTM1 Input Capture Filter Control Register 0; 0xFFFF80AF */
volatile FTM1FILTER1STR _FTM1FILTER1;                      /* FTM1 Input Capture Filter Control Register 1; 0xFFFF80B0 */
volatile FTM1FLTFILTERSTR _FTM1FLTFILTER;                  /* FTM1 Fault Input Filter Control Register; 0xFFFF80B1 */
volatile FTM1FLTCTRLSTR _FTM1FLTCTRL;                      /* FTM1 Fault Control Register; 0xFFFF80B2 */
volatile FTM2SCSTR _FTM2SC;                                /* FTM2 Status and Control Register; 0xFFFF80C0 */
volatile FTM2C0SCSTR _FTM2C0SC;                            /* FTM2 Timer Channel 0 Status and Control Register; 0xFFFF80C5 */
volatile FTM2C1SCSTR _FTM2C1SC;                            /* FTM2 Timer Channel 1 Status and Control Register; 0xFFFF80C8 */
volatile FTM2C2SCSTR _FTM2C2SC;                            /* FTM2 Timer Channel 2 Status and Control Register; 0xFFFF80CB */
volatile FTM2C3SCSTR _FTM2C3SC;                            /* FTM2 Timer Channel 3 Status and Control Register; 0xFFFF80CE */
volatile FTM2C4SCSTR _FTM2C4SC;                            /* FTM2 Timer Channel 4 Status and Control Register; 0xFFFF80D1 */
volatile FTM2C5SCSTR _FTM2C5SC;                            /* FTM2 Timer Channel 5 Status and Control Register; 0xFFFF80D4 */
volatile FTM2STATUSSTR _FTM2STATUS;                        /* FTM2 Capture and Compare Status Register; 0xFFFF80E2 */
volatile FTM2MODESTR _FTM2MODE;                            /* FTM2 Features Mode Selection Register; 0xFFFF80E3 */
volatile FTM2SYNCSTR _FTM2SYNC;                            /* FTM2 Synchronization Register; 0xFFFF80E4 */
volatile FTM2OUTINITSTR _FTM2OUTINIT;                      /* FTM2 Initial State for Channels Output Register; 0xFFFF80E5 */
volatile FTM2OUTMASKSTR _FTM2OUTMASK;                      /* FTM2 Output Mask Register; 0xFFFF80E6 */
volatile FTM2COMBINE0STR _FTM2COMBINE0;                    /* FTM2 Function For Linked Channel 0 Register; 0xFFFF80E7 */
volatile FTM2COMBINE1STR _FTM2COMBINE1;                    /* FTM2 Function For Linked Channel 1 Register; 0xFFFF80E8 */
volatile FTM2COMBINE2STR _FTM2COMBINE2;                    /* FTM2 Function For Linked Channel 2 Register; 0xFFFF80E9 */
volatile FTM2DEADTIMESTR _FTM2DEADTIME;                    /* FTM2 Deadtime Insertion Control Register; 0xFFFF80EB */
volatile FTM2EXTTRIGSTR _FTM2EXTTRIG;                      /* FTM2 ExternalTrigger Register; 0xFFFF80EC */
volatile FTM2POLSTR _FTM2POL;                              /* FTM2 Channels Polarity Register; 0xFFFF80ED */
volatile FTM2FMSSTR _FTM2FMS;                              /* FTM2 Fault Mode Status Register; 0xFFFF80EE */
volatile FTM2FILTER0STR _FTM2FILTER0;                      /* FTM2 Input Capture Filter Control Register 0; 0xFFFF80EF */
volatile FTM2FILTER1STR _FTM2FILTER1;                      /* FTM2 Input Capture Filter Control Register 1; 0xFFFF80F0 */
volatile FTM2FLTFILTERSTR _FTM2FLTFILTER;                  /* FTM2 Fault Input Filter Control Register; 0xFFFF80F1 */
volatile FTM2FLTCTRLSTR _FTM2FLTCTRL;                      /* FTM2 Fault Control Register; 0xFFFF80F2 */
volatile PTAPUESTR _PTAPUE;                                /* Port A Pulling Enable Register; 0xFFFF8100 */
volatile PTAPUSSTR _PTAPUS;                                /* Port A Pullup/Pulldown Select Register; 0xFFFF8101 */
volatile PTADSSTR _PTADS;                                  /* Port A Drive Strength Selection Register; 0xFFFF8102 */
volatile PTASRESTR _PTASRE;                                /* Port A Slew Rate Enable Register; 0xFFFF8103 */
volatile PTAPFESTR _PTAPFE;                                /* Port A Passive Filter Enable Register; 0xFFFF8104 */
volatile PTAICSTR _PTAIC;                                  /* Port A Interrupt Control Register; 0xFFFF8105 */
volatile PTAIPESTR _PTAIPE;                                /* Port A Interrupt Pin Enable Register; 0xFFFF8106 */
volatile PTAIFSTR _PTAIF;                                  /* Port A Interrupt Flag Register; 0xFFFF8107 */
volatile PTAIESSTR _PTAIES;                                /* Port A Interrupt Edge Select Register; 0xFFFF8108 */
volatile PTADFESTR _PTADFE;                                /* Port A Digital Filter Enable Register; 0xFFFF8109 */
volatile PTADFCSTR _PTADFC;                                /* Port A Digital Filter Control Register; 0xFFFF810A */
volatile PTBPUESTR _PTBPUE;                                /* Port B Pulling Enable Register; 0xFFFF8110 */
volatile PTBPUSSTR _PTBPUS;                                /* Port B Pullup/Pulldown Select Register; 0xFFFF8111 */
volatile PTBDSSTR _PTBDS;                                  /* Port B Drive Strength Selection Register; 0xFFFF8112 */
volatile PTBSRESTR _PTBSRE;                                /* Port B Slew Rate Enable Register; 0xFFFF8113 */
volatile PTBPFESTR _PTBPFE;                                /* Port B Passive Filter Enable Register; 0xFFFF8114 */
volatile PTBICSTR _PTBIC;                                  /* Port B Interrupt Control Register; 0xFFFF8115 */
volatile PTBIPESTR _PTBIPE;                                /* Port B Interrupt Pin Enable Register; 0xFFFF8116 */
volatile PTBIFSTR _PTBIF;                                  /* Port B Interrupt Flag Register; 0xFFFF8117 */
volatile PTBIESSTR _PTBIES;                                /* Port B Interrupt Edge Select Register; 0xFFFF8118 */
volatile PTBDFESTR _PTBDFE;                                /* Port B Digital Filter Enable Register; 0xFFFF8119 */
volatile PTBDFCSTR _PTBDFC;                                /* Port B Digital Filter Control Register; 0xFFFF811A */
volatile PTCPUESTR _PTCPUE;                                /* Port C Pulling Enable Register; 0xFFFF8120 */
volatile PTCPUSSTR _PTCPUS;                                /* Port C Pullup/Pulldown Select Register; 0xFFFF8121 */
volatile PTCDSSTR _PTCDS;                                  /* Port C Drive Strength Selection Register; 0xFFFF8122 */
volatile PTCSRESTR _PTCSRE;                                /* Port C Slew Rate Enable Register; 0xFFFF8123 */
volatile PTCPFESTR _PTCPFE;                                /* Port C Passive Filter Enable Register; 0xFFFF8124 */
volatile PTCICSTR _PTCIC;                                  /* Port C Interrupt Control Register; 0xFFFF8125 */
volatile PTCIPESTR _PTCIPE;                                /* Port C Interrupt Pin Enable Register; 0xFFFF8126 */
volatile PTCIFSTR _PTCIF;                                  /* Port C Interrupt Flag Register; 0xFFFF8127 */
volatile PTCIESSTR _PTCIES;                                /* Port C Interrupt Edge Select Register; 0xFFFF8128 */
volatile PTCDFESTR _PTCDFE;                                /* Port C Digital Filter Enable Register; 0xFFFF8129 */
volatile PTCDFCSTR _PTCDFC;                                /* Port C Digital Filter Control Register; 0xFFFF812A */
volatile PTDPUESTR _PTDPUE;                                /* Port D Pulling Enable Register; 0xFFFF8130 */
volatile PTDPUSSTR _PTDPUS;                                /* Port D Pullup/Pulldown Select Register; 0xFFFF8131 */
volatile PTDDSSTR _PTDDS;                                  /* Port D Drive Strength Selection Register; 0xFFFF8132 */
volatile PTDSRESTR _PTDSRE;                                /* Port D Slew Rate Enable Register; 0xFFFF8133 */
volatile PTDPFESTR _PTDPFE;                                /* Port D Passive Filter Enable Register; 0xFFFF8134 */
volatile PTDICSTR _PTDIC;                                  /* Port D Interrupt Control Register; 0xFFFF8135 */
volatile PTDIPESTR _PTDIPE;                                /* Port D Interrupt Pin Enable Register; 0xFFFF8136 */
volatile PTDIFSTR _PTDIF;                                  /* Port D Interrupt Flag Register; 0xFFFF8137 */
volatile PTDIESSTR _PTDIES;                                /* Port D Interrupt Edge Select Register; 0xFFFF8138 */
volatile PTDDFESTR _PTDDFE;                                /* Port D Digital Filter Enable Register; 0xFFFF8139 */
volatile PTDDFCSTR _PTDDFC;                                /* Port D Digital Filter Control Register; 0xFFFF813A */
volatile PTEPUESTR _PTEPUE;                                /* Port E Pulling Enable Register; 0xFFFF8140 */
volatile PTEPUSSTR _PTEPUS;                                /* Port E Pullup/Pulldown Select Register; 0xFFFF8141 */
volatile PTEDSSTR _PTEDS;                                  /* Port E Drive Strength Selection Register; 0xFFFF8142 */
volatile PTESRESTR _PTESRE;                                /* Port E Slew Rate Enable Register; 0xFFFF8143 */
volatile PTEPFESTR _PTEPFE;                                /* Port E Passive Filter Enable Register; 0xFFFF8144 */
volatile PTEICSTR _PTEIC;                                  /* Port E Interrupt Control Register; 0xFFFF8145 */
volatile PTEIPESTR _PTEIPE;                                /* Port E Interrupt Pin Enable Register; 0xFFFF8146 */
volatile PTEIFSTR _PTEIF;                                  /* Port E Interrupt Flag Register; 0xFFFF8147 */
volatile PTEIESSTR _PTEIES;                                /* Port E Interrupt Edge Select Register; 0xFFFF8148 */
volatile PTEDFESTR _PTEDFE;                                /* Port E Digital Filter Enable Register; 0xFFFF8149 */
volatile PTEDFCSTR _PTEDFC;                                /* Port E Digital Filter Control Register; 0xFFFF814A */
volatile PTFPUESTR _PTFPUE;                                /* Port F Pulling Enable Register; 0xFFFF8150 */
volatile PTFPUSSTR _PTFPUS;                                /* Port F Pullup/Pulldown Select Register; 0xFFFF8151 */
volatile PTFDSSTR _PTFDS;                                  /* Port F Drive Strength Selection Register; 0xFFFF8152 */
volatile PTFSRESTR _PTFSRE;                                /* Port F Slew Rate Enable Register; 0xFFFF8153 */
volatile PTFPFESTR _PTFPFE;                                /* Port F Passive Filter Enable Register; 0xFFFF8154 */
volatile PTFICSTR _PTFIC;                                  /* Port F Interrupt Control Register; 0xFFFF8155 */
volatile PTFIPESTR _PTFIPE;                                /* Port F Interrupt Pin Enable Register; 0xFFFF8156 */
volatile PTFIFSTR _PTFIF;                                  /* Port F Interrupt Flag Register; 0xFFFF8157 */
volatile PTFIESSTR _PTFIES;                                /* Port F Interrupt Edge Select Register; 0xFFFF8158 */
volatile PTFDFESTR _PTFDFE;                                /* Port F Digital Filter Enable Register; 0xFFFF8159 */
volatile PTFDFCSTR _PTFDFC;                                /* Port F Digital Filter Control Register; 0xFFFF815A */
volatile PTGPUESTR _PTGPUE;                                /* Port G Pulling Enable Register; 0xFFFF8160 */
volatile PTGPUSSTR _PTGPUS;                                /* Port G Pullup/Pulldown Select Register; 0xFFFF8161 */
volatile PTGDSSTR _PTGDS;                                  /* Port G Drive Strength Selection Register; 0xFFFF8162 */
volatile PTGSRESTR _PTGSRE;                                /* Port G Slew Rate Enable Register; 0xFFFF8163 */
volatile PTGPFESTR _PTGPFE;                                /* Port G Passive Filter Enable Register; 0xFFFF8164 */
volatile PTGICSTR _PTGIC;                                  /* Port G Interrupt Control Register; 0xFFFF8165 */
volatile PTGIPESTR _PTGIPE;                                /* Port G Interrupt Pin Enable Register; 0xFFFF8166 */
volatile PTGIFSTR _PTGIF;                                  /* Port G Interrupt Flag Register; 0xFFFF8167 */
volatile PTGIESSTR _PTGIES;                                /* Port G Interrupt Edge Select Register; 0xFFFF8168 */
volatile PTGDFESTR _PTGDFE;                                /* Port G Digital Filter Enable Register; 0xFFFF8169 */
volatile PTGDFCSTR _PTGDFC;                                /* Port G Digital Filter Control Register; 0xFFFF816A */
volatile PTHPUESTR _PTHPUE;                                /* Port H Pulling Enable Register; 0xFFFF8170 */
volatile PTHPUSSTR _PTHPUS;                                /* Port H Pullup/Pulldown Select Register; 0xFFFF8171 */
volatile PTHDSSTR _PTHDS;                                  /* Port H Drive Strength Selection Register; 0xFFFF8172 */
volatile PTHSRESTR _PTHSRE;                                /* Port H Slew Rate Enable Register; 0xFFFF8173 */
volatile PTHPFESTR _PTHPFE;                                /* Port H Passive Filter Enable Register; 0xFFFF8174 */
volatile PTHICSTR _PTHIC;                                  /* Port H Interrupt Control Register; 0xFFFF8175 */
volatile PTHIPESTR _PTHIPE;                                /* Port H Interrupt Pin Enable Register; 0xFFFF8176 */
volatile PTHIFSTR _PTHIF;                                  /* Port H Interrupt Flag Register; 0xFFFF8177 */
volatile PTHIESSTR _PTHIES;                                /* Port H Interrupt Edge Select Register; 0xFFFF8178 */
volatile PTHDFESTR _PTHDFE;                                /* Port H Digital Filter Enable Register; 0xFFFF8179 */
volatile PTHDFCSTR _PTHDFC;                                /* Port H Digital Filter Control Register; 0xFFFF817A */
volatile PTJPUESTR _PTJPUE;                                /* Port J Pulling Enable Register; 0xFFFF8180 */
volatile PTJPUSSTR _PTJPUS;                                /* Port J Pullup/Pulldown Select Register; 0xFFFF8181 */
volatile PTJDSSTR _PTJDS;                                  /* Port J Drive Strength Selection Register; 0xFFFF8182 */
volatile PTJSRESTR _PTJSRE;                                /* Port J Slew Rate Enable Register; 0xFFFF8183 */
volatile PTJPFESTR _PTJPFE;                                /* Port J Passive Filter Enable Register; 0xFFFF8184 */
volatile PTJICSTR _PTJIC;                                  /* Port J Interrupt Control Register; 0xFFFF8185 */
volatile PTJIPESTR _PTJIPE;                                /* Port J Interrupt Pin Enable Register; 0xFFFF8186 */
volatile PTJIFSTR _PTJIF;                                  /* Port J Interrupt Flag Register; 0xFFFF8187 */
volatile PTJIESSTR _PTJIES;                                /* Port J Interrupt Edge Select Register; 0xFFFF8188 */
volatile PTJDFESTR _PTJDFE;                                /* Port J Digital Filter Enable Register; 0xFFFF8189 */
volatile PTJDFCSTR _PTJDFC;                                /* Port J Digital Filter Control Register; 0xFFFF818A */
volatile IICA1STR _IICA1;                                  /* IIC Address Register; 0xFFFF8190 */
volatile IICFSTR _IICF;                                    /* IIC Frequency Divider Register; 0xFFFF8191 */
volatile IICC1STR _IICC1;                                  /* IIC Control Register 1; 0xFFFF8192 */
volatile IICSSTR _IICS;                                    /* IIC Status Register; 0xFFFF8193 */
volatile IICDSTR _IICD;                                    /* IIC Data I/O Register; 0xFFFF8194 */
volatile IICC2STR _IICC2;                                  /* IIC Control Register 2; 0xFFFF8195 */
volatile IICFLTSTR _IICFLT;                                /* IIC Filter register; 0xFFFF8196 */
volatile IICSMBSTR _IICSMB;                                /* SMBus Control and Status Register; 0xFFFF8197 */
volatile IICA2STR _IICA2;                                  /* IIC Address Register 2; 0xFFFF8198 */
volatile SPI1C1STR _SPI1C1;                                /* SPI1 Control Register 1; 0xFFFF81A0 */
volatile SPI1C2STR _SPI1C2;                                /* SPI1 Control Register 2; 0xFFFF81A1 */
volatile SPI1BRSTR _SPI1BR;                                /* SPI1 Baud Rate Register; 0xFFFF81A2 */
volatile SPI1SSTR _SPI1S;                                  /* SPI1 Status Register; 0xFFFF81A3 */
volatile SPI2C1STR _SPI2C1;                                /* SPI2 Control Register 1; 0xFFFF81A8 */
volatile SPI2C2STR _SPI2C2;                                /* SPI2 Control Register 2; 0xFFFF81A9 */
volatile SPI2BRSTR _SPI2BR;                                /* SPI2 Baud Rate Register; 0xFFFF81AA */
volatile SPI2SSTR _SPI2S;                                  /* SPI2 Status Register; 0xFFFF81AB */
volatile HSCMP1CR0STR _HSCMP1CR0;                          /* HSCMP1 Control Register 0; 0xFFFF81B0 */
volatile HSCMP1CR1STR _HSCMP1CR1;                          /* HSCMP1 Control Register 1; 0xFFFF81B1 */
volatile HSCMP1FPRSTR _HSCMP1FPR;                          /* HSCMP1 Filter Period Register; 0xFFFF81B2 */
volatile HSCMP1SCRSTR _HSCMP1SCR;                          /* HSCMP1 Status & Control Register; 0xFFFF81B3 */
volatile HSCMP1PCRSTR _HSCMP1PCR;                          /* Pin Control Register; 0xFFFF81B4 */
volatile HSCMP2CR0STR _HSCMP2CR0;                          /* HSCMP2 Control Register 0; 0xFFFF81B8 */
volatile HSCMP2CR1STR _HSCMP2CR1;                          /* HSCMP2 Control Register 1; 0xFFFF81B9 */
volatile HSCMP2FPRSTR _HSCMP2FPR;                          /* HSCMP2 Filter Period Register; 0xFFFF81BA */
volatile HSCMP2SCRSTR _HSCMP2SCR;                          /* HSCMP2 Status & Control Register; 0xFFFF81BB */
volatile HSCMP2PCRSTR _HSCMP2PCR;                          /* Pin Control Register; 0xFFFF81BC */
volatile SRSSTR _SRS;                                      /* System Reset Status Register; 0xFFFF9800 */
volatile SOPT1STR _SOPT1;                                  /* System Options Register 1; 0xFFFF9802 */
volatile SMCLKSTR _SMCLK;                                  /* System MCLK Control Register; 0xFFFF9803 */
volatile SPMSC1STR _SPMSC1;                                /* System Power Management Status and Control 1 Register; 0xFFFF9809 */
volatile SPMSC2STR _SPMSC2;                                /* System Power Management Status and Control 2 Register; 0xFFFF980A */
volatile SOPT2STR _SOPT2;                                  /* System Options Register 2; 0xFFFF980C */
volatile SCGC1STR _SCGC1;                                  /* System Clock Gating Control 1 Register; 0xFFFF980D */
volatile SCGC2STR _SCGC2;                                  /* System Clock Gating Control 2 Register; 0xFFFF980E */
volatile SCGC3STR _SCGC3;                                  /* System Clock Gating Control 3 Register; 0xFFFF980F */
volatile FCDIVSTR _FCDIV;                                  /* FLASH Clock Divider Register; 0xFFFF9820 */
volatile FOPTSTR _FOPT;                                    /* Flash Options Register; 0xFFFF9821 */
volatile FCNFGSTR _FCNFG;                                  /* Flash Configuration Register; 0xFFFF9823 */
volatile FPROTSTR _FPROT;                                  /* Flash Protection Register; 0xFFFF9824 */
volatile FSTATSTR _FSTAT;                                  /* Flash Status Register; 0xFFFF9825 */
volatile FCMDSTR _FCMD;                                    /* Flash Command Register; 0xFFFF9826 */
volatile SAPE1STR _SAPE1;                                  /* System ADC Pin Enable 1 Register; 0xFFFF9831 */
volatile SAPE2STR _SAPE2;                                  /* System ADC Pin Enable 2 Register; 0xFFFF9832 */
volatile SAPE3STR _SAPE3;                                  /* System ADC Pin Enable 3 Register; 0xFFFF9833 */
volatile SEIS1STR _SEIS1;                                  /* System eGPIO Interrupt Status 1 Register; 0xFFFF9834 */
volatile SEIS2STR _SEIS2;                                  /* System eGPIO Interrupt Status 2 Register; 0xFFFF9835 */
volatile SPINPSSTR _SPINPS;                                /* System Pin Positioning Register; 0xFFFF9836 */
volatile SIMPTASTR _SIMPTA;                                /* System Port A Input Buffer Enable Register; 0xFFFF9837 */
volatile SIMPTBSTR _SIMPTB;                                /* System Port B Input Buffer Enable Register; 0xFFFF9838 */
volatile SIMPTCSTR _SIMPTC;                                /* System Port C Input Buffer Enable Register; 0xFFFF9839 */
volatile SIMPTDSTR _SIMPTD;                                /* System Port D Input Buffer Enable Register; 0xFFFF983A */
volatile SIMPTESTR _SIMPTE;                                /* System Port E Input Buffer Enable Register; 0xFFFF983B */
volatile SIMPTFSTR _SIMPTF;                                /* System Port F Input Buffer Enable Register; 0xFFFF983C */
volatile SIMPTGSTR _SIMPTG;                                /* System Port G Input Buffer Enable Register; 0xFFFF983D */
volatile SIMPTHSTR _SIMPTH;                                /* System Port H Input Buffer Enable Register; 0xFFFF983E */
volatile SIMPTJSTR _SIMPTJ;                                /* System Port J Input Buffer Enable Register; 0xFFFF983F */
volatile EWMCTRLSTR _EWMCTRL;                              /* EWM Control Register; 0xFFFF9840 */
volatile EWMSERVSTR _EWMSERV;                              /* EWM Service Register; 0xFFFF9841 */
volatile EWMCMPLSTR _EWMCMPL;                              /* EWM Compare Low Register; 0xFFFF9842 */
volatile EWMCMPHSTR _EWMCMPH;                              /* EWM Compare High Register; 0xFFFF9843 */
volatile IRQSCSTR _IRQSC;                                  /* Interrupt request status and control register; 0xFFFF9844 */
volatile CRCHSTR _CRCH;                                    /* CRC High Register; 0xFFFF9850 */
volatile CRCLSTR _CRCL;                                    /* CRC Low Register; 0xFFFF9851 */
volatile TRANSPOSESTR _TRANSPOSE;                          /* CRC Transpose Register; 0xFFFF9852 */
volatile CRCL0STR _CRCL0;                                  /* CRC Low Register 0; 0xFFFF9854 */
volatile CRCL1STR _CRCL1;                                  /* CRC Low Register 1; 0xFFFF9855 */
volatile CRCL2STR _CRCL2;                                  /* CRC Low Register 2; 0xFFFF9856 */
volatile CRCL3STR _CRCL3;                                  /* CRC Low Register 3; 0xFFFF9857 */
volatile PDBCTRL1STR _PDBCTRL1;                            /* PDB Control 1 Register; 0xFFFF9860 */
volatile PDBCTRL2STR _PDBCTRL2;                            /* PDB Control 2 Register; 0xFFFF9861 */
volatile PDBSCRSTR _PDBSCR;                                /* PDB Status Register; 0xFFFF986A */
volatile IEVENT_DR0STR _IEVENT_DR0;                        /* iEvent Data Register 0; 0xFFFF9A00 */
volatile IEVENT_DR1STR _IEVENT_DR1;                        /* iEvent Data Register 1; 0xFFFF9A01 */
volatile IEVENT_DR2STR _IEVENT_DR2;                        /* iEvent Data Register 2; 0xFFFF9A02 */
volatile IEVENT_DR3STR _IEVENT_DR3;                        /* iEvent Data Register 3; 0xFFFF9A03 */
volatile IEVENT_CR0STR _IEVENT_CR0;                        /* iEvent Control Register 0; 0xFFFF9A80 */
volatile IEVENT_CR1STR _IEVENT_CR1;                        /* iEvent Control Register 1; 0xFFFF9A81 */
volatile IEVENT_CR2STR _IEVENT_CR2;                        /* iEvent Control Register 2; 0xFFFF9A82 */
volatile IEVENT_CR3STR _IEVENT_CR3;                        /* iEvent Control Register 3; 0xFFFF9A83 */
volatile INTC_FRCSTR _INTC_FRC;                            /* INTC Force Interrupt Register; 0xFFFFFFD0 */
volatile INTC_PL6P7STR _INTC_PL6P7;                        /* INTC Programmable Level 6, Priority 7 Register; 0xFFFFFFD8 */
volatile INTC_PL6P6STR _INTC_PL6P6;                        /* INTC Programmable Level 6, Priority 6 Register; 0xFFFFFFD9 */
volatile INTC_WCRSTR _INTC_WCR;                            /* INTC Wake-up Control Register; 0xFFFFFFDB */
volatile INTC_SIMRSTR _INTC_SIMR;                          /* Set Interrupt Mask Register; 0xFFFFFFDC */
volatile INTC_CIMRSTR _INTC_CIMR;                          /* Clear Interrupt Mask Register; 0xFFFFFFDD */
volatile INTC_SFRCSTR _INTC_SFRC;                          /* INTC Set Interrupt Force Register; 0xFFFFFFDE */
volatile INTC_CFRCSTR _INTC_CFRC;                          /* INTC Clear Interrupt Force Register; 0xFFFFFFDF */
volatile INTC_SWIACKSTR _INTC_SWIACK;                      /* INTC Software IACK Register; 0xFFFFFFE0 */
volatile INTC_LVL1IACKSTR _INTC_LVL1IACK;                  /* INTC Level 1 IACK Register; 0xFFFFFFE4 */
volatile INTC_LVL2IACKSTR _INTC_LVL2IACK;                  /* INTC Level 2 IACK Register; 0xFFFFFFE8 */
volatile INTC_LVL3IACKSTR _INTC_LVL3IACK;                  /* INTC Level 3 IACK Register; 0xFFFFFFEC */
volatile INTC_LVL4IACKSTR _INTC_LVL4IACK;                  /* INTC Level 4 IACK Register; 0xFFFFFFF0 */
volatile INTC_LVL5IACKSTR _INTC_LVL5IACK;                  /* INTC Level 5 IACK Register; 0xFFFFFFF4 */
volatile INTC_LVL6IACKSTR _INTC_LVL6IACK;                  /* INTC Level 6 IACK Register; 0xFFFFFFF8 */
volatile INTC_LVL7IACKSTR _INTC_LVL7IACK;                  /* INTC Level 7 IACK Register; 0xFFFFFFFC */


/* * * * *  16-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile RGPIO_DIRSTR _RGPIO_DIR;                          /* RGPIO Data Direction Register; 0x00C00000 */
volatile RGPIO_DATASTR _RGPIO_DATA;                        /* RGPIO Data Register; 0x00C00002 */
volatile RGPIO_ENBSTR _RGPIO_ENB;                          /* RGPIO Pin Enable Register; 0x00C00004 */
volatile RGPIO_CLRSTR _RGPIO_CLR;                          /* RGPIO Clear Data Register; 0x00C00006 */
volatile RGPIO_SETSTR _RGPIO_SET;                          /* RGPIO Set Data Register; 0x00C0000A */
volatile RGPIO_TOGSTR _RGPIO_TOG;                          /* RGPIO Toggle Data Register; 0x00C0000E */
volatile ADCSC1ASTR _ADCSC1A;                              /* ADC Status and control register 1A; 0xFFFF8030 */
volatile ADCSC2STR _ADCSC2;                                /* ADC Status and control register 2; 0xFFFF8032 */
volatile ADCCFGSTR _ADCCFG;                                /* ADC Configuration register; 0xFFFF803C */
volatile ADCSC1BSTR _ADCSC1B;                              /* ADC Status and control register 1B; 0xFFFF8044 */
volatile ADCRASTR _ADCRA;                                  /* ADC Data Result Registers A; 0xFFFF8046 */
volatile ADCRBSTR _ADCRB;                                  /* ADC Data Result Registers B; 0xFFFF8048 */
volatile SCI1BDSTR _SCI1BD;                                /* SCI1 Baud Rate Register; 0xFFFF8050 */
volatile SCI2BDSTR _SCI2BD;                                /* SCI2 Baud Rate Register; 0xFFFF8060 */
volatile TPM3CNTSTR _TPM3CNT;                              /* TPM3 Timer Counter Register; 0xFFFF8071 */
volatile TPM3MODSTR _TPM3MOD;                              /* TPM3 Timer Counter Modulo Register; 0xFFFF8073 */
volatile TPM3C0VSTR _TPM3C0V;                              /* TPM3 Timer Channel 0 Value Register; 0xFFFF8076 */
volatile TPM3C1VSTR _TPM3C1V;                              /* TPM3 Timer Channel 1 Value Register; 0xFFFF8079 */
volatile FTM1CNTSTR _FTM1CNT;                              /* FTM1 Timer Counter Register; 0xFFFF8081 */
volatile FTM1MODSTR _FTM1MOD;                              /* FTM1 Timer Counter Modulo Register; 0xFFFF8083 */
volatile FTM1C0VSTR _FTM1C0V;                              /* FTM1 Timer Channel 0 Value Register; 0xFFFF8086 */
volatile FTM1C1VSTR _FTM1C1V;                              /* FTM1 Timer Channel 1 Value Register; 0xFFFF8089 */
volatile FTM1C2VSTR _FTM1C2V;                              /* FTM1 Timer Channel 2 Value Register; 0xFFFF808C */
volatile FTM1C3VSTR _FTM1C3V;                              /* FTM1 Timer Channel 3 Value Register; 0xFFFF808F */
volatile FTM1C4VSTR _FTM1C4V;                              /* FTM1 Timer Channel 4 Value Register; 0xFFFF8092 */
volatile FTM1C5VSTR _FTM1C5V;                              /* FTM1 Timer Channel 5 Value Register; 0xFFFF8095 */
volatile FTM1CNTINSTR _FTM1CNTIN;                          /* FTM1 Counter Initial Value Registers; 0xFFFF80A0 */
volatile FTM2CNTSTR _FTM2CNT;                              /* FTM2 Timer Counter Register; 0xFFFF80C1 */
volatile FTM2MODSTR _FTM2MOD;                              /* FTM2 Timer Counter Modulo Register; 0xFFFF80C3 */
volatile FTM2C0VSTR _FTM2C0V;                              /* FTM2 Timer Channel 0 Value Register; 0xFFFF80C6 */
volatile FTM2C1VSTR _FTM2C1V;                              /* FTM2 Timer Channel 1 Value Register; 0xFFFF80C9 */
volatile FTM2C2VSTR _FTM2C2V;                              /* FTM2 Timer Channel 2 Value Register; 0xFFFF80CC */
volatile FTM2C3VSTR _FTM2C3V;                              /* FTM2 Timer Channel 3 Value Register; 0xFFFF80CF */
volatile FTM2C4VSTR _FTM2C4V;                              /* FTM2 Timer Channel 4 Value Register; 0xFFFF80D2 */
volatile FTM2C5VSTR _FTM2C5V;                              /* FTM2 Timer Channel 5 Value Register; 0xFFFF80D5 */
volatile FTM2CNTINSTR _FTM2CNTIN;                          /* FTM2 Counter Initial Value Registers; 0xFFFF80E0 */
volatile IICSLTSTR _IICSLT;                                /* IIC SCL Low Time Out register; 0xFFFF8199 */
volatile SPI1D16STR _SPI1D16;                              /* SPI1 Data Register; 0xFFFF81A4 */
volatile SPI1MSTR _SPI1M;                                  /* SPI1 Match Register; 0xFFFF81A6 */
volatile SPI2D16STR _SPI2D16;                              /* SPI2 Data Register; 0xFFFF81AC */
volatile SPI2MSTR _SPI2M;                                  /* SPI2 Match Register; 0xFFFF81AE */
volatile SDIDSTR _SDID;                                    /* System Device Identification Register; 0xFFFF9806 */
volatile PDBDLYASTR _PDBDLYA;                              /* PDB Delay A Register; 0xFFFF9862 */
volatile PDBDLYBSTR _PDBDLYB;                              /* PDB Delay B Register; 0xFFFF9864 */
volatile PDBMODSTR _PDBMOD;                                /* PDB Counter Modulus Register; 0xFFFF9866 */
volatile PDBCNTSTR _PDBCNT;                                /* PDB Counter Value Register; 0xFFFF9868 */
volatile WDOG_REFRESHSTR _WDOG_REFRESH;                    /* Watchdog Refresh Register; 0xFFFF988C */
volatile WDOG_UNLOCKSTR _WDOG_UNLOCK;                      /* Watchdog Unlock Register; 0xFFFF988E */
volatile WDOG_RST_CNTSTR _WDOG_RST_CNT;                    /* Watchdog Reset Count Register; 0xFFFF9894 */


/* * * * *  32-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile WDOG_ST_CTRLSTR _WDOG_ST_CTRL;                    /* Watchdog Status and Control Register; 0xFFFF9880 */
volatile WDOG_TO_VALSTR _WDOG_TO_VAL;                      /* Watchdog Control and Status Register; 0xFFFF9884 */
volatile WDOG_WINSTR _WDOG_WIN;                            /* Watchdog Window Register; 0xFFFF9888 */
volatile WDOG_TIMER_OUTSTR _WDOG_TIMER_OUT;                /* Watchdog Timer Output Register; 0xFFFF9890 */
volatile IEVENT_IMXCR0STR _IEVENT_IMXCR0;                  /* iEvent Input Mux Configuration Register 0; 0xFFFF9B00 */
volatile IEVENT_BFECR0STR _IEVENT_BFECR0;                  /* iEvent Boolean Function Evaluation Configuration Register 0; 0xFFFF9B04 */
volatile IEVENT_IMXCR1STR _IEVENT_IMXCR1;                  /* iEvent Input Mux Configuration Register 1; 0xFFFF9B08 */
volatile IEVENT_BFECR1STR _IEVENT_BFECR1;                  /* iEvent Boolean Function Evaluation Configuration Register 1; 0xFFFF9B0C */
volatile IEVENT_IMXCR2STR _IEVENT_IMXCR2;                  /* iEvent Input Mux Configuration Register 2; 0xFFFF9B10 */
volatile IEVENT_BFECR2STR _IEVENT_BFECR2;                  /* iEvent Boolean Function Evaluation Configuration Register 2; 0xFFFF9B14 */
volatile IEVENT_IMXCR3STR _IEVENT_IMXCR3;                  /* iEvent Input Mux Configuration Register 3; 0xFFFF9B18 */
volatile IEVENT_BFECR3STR _IEVENT_BFECR3;                  /* iEvent Boolean Function Evaluation Configuration Register 3; 0xFFFF9B1C */
volatile DMAREQCSTR _DMAREQC;                              /* DMA Request Control; 0xFFFFE400 */
volatile SAR0STR _SAR0;                                    /* Source Address Register 0; 0xFFFFE500 */
volatile DAR0STR _DAR0;                                    /* Destination Address Register 0; 0xFFFFE504 */
volatile BCR0STR _BCR0;                                    /* Byte Count Register 0; 0xFFFFE508 */
volatile DCR0STR _DCR0;                                    /* DMA Control Register 0; 0xFFFFE50C */
volatile SAR1STR _SAR1;                                    /* Source Address Register 1; 0xFFFFE510 */
volatile DAR1STR _DAR1;                                    /* Destination Address Register 1; 0xFFFFE514 */
volatile BCR1STR _BCR1;                                    /* Byte Count Register 1; 0xFFFFE518 */
volatile DCR1STR _DCR1;                                    /* DMA Control Register 1; 0xFFFFE51C */
volatile SAR2STR _SAR2;                                    /* Source Address Register 2; 0xFFFFE520 */
volatile DAR2STR _DAR2;                                    /* Destination Address Register 2; 0xFFFFE524 */
volatile BCR2STR _BCR2;                                    /* Byte Count Register 2; 0xFFFFE528 */
volatile DCR2STR _DCR2;                                    /* DMA Control Register 2; 0xFFFFE52C */
volatile SAR3STR _SAR3;                                    /* Source Address Register 3; 0xFFFFE530 */
volatile DAR3STR _DAR3;                                    /* Destination Address Register 3; 0xFFFFE534 */
volatile BCR3STR _BCR3;                                    /* Byte Count Register 3; 0xFFFFE538 */
volatile DCR3STR _DCR3;                                    /* DMA Control Register 3; 0xFFFFE53C */
volatile INTC_IMRHSTR _INTC_IMRH;                          /* Interrupt Mask Register High; 0xFFFFFFC8 */
volatile INTC_IMRLSTR _INTC_IMRL;                          /* Interrupt Mask Register Low; 0xFFFFFFCC */

/*---------------------- USER ------------------------*/
/* Copy of SR for the EnterCritical and ExitCritical functions */
word SR_Ipl;
byte NestedLevel;
byte IRQSC_copy;
/* Copy of Reset Source from BootLoader */
volatile byte _RST_COPY_VALUE;
/*lint -restore */

/* EOF */
