/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		CIRRUS_5460-5461 Definitions
\n		definitions of registers, macroes and default config values
\n
\file		${Cirrus5460def.h}
\ingroup	EM Energy Meter
\date		  01.10.2009
\version	Platform2_00.00.12 (project baseline related to creation of this module)
\author		${Andrea Mantovani}
\n
\n
\Par\b	History:
\n
\n 		01.04.00 <baseline di progetto relativa alla versione corrente>
\n		gg/mm/aaaa <data in cui è stata realizzata la versione corrente>
\n		developer <progettista autore della modifica>
\n		changes <principali modifiche apportate>
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 <baseline di progetto relativa alla versione precedente la corrente>
\n		gg/mm/aaaa <data in cui è stata realizzata >
\n		developer <progettista autore della modifica>
\n		changes <principali modifiche apportate>
\n 		- change 1
\n		- change 2
\n
\n
\n 
*/

#ifndef __CIRRUS5460DEF_H
#define __CIRRUS5460DEF_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */




/* Commands */
#define POWUPHALT       0xA0
#define SWRESET         0x80
#define STANDBY         0x88
#define SLEEP           0x90
#define SYNC1           0xFF
#define SYNC0           0xFE 
#define SYNC0inv        0x7F
#define NOP             SYNC0inv
#define DRDY            0x80            /* Flag: ciclo di conversione completato (1) */
#define WRITE_REGISTER  0x40
#define READ_REGISTER   0x3E
#define STARTSINGLECONV 0xE0
#define STARTCONTCONV   0xE8


/* Cirrus Internal Registers */
#define Config_Register           0x00
#define Offset_Register_I         0x01
#define Gain_Register_I           0x02
#define Offset_Register_V         0x03
#define Gain_Register_V           0x04
#define Cycle_Count_Register      0x05
#define Prate_Register            0x06
#define I_Register                0x07
#define V_Register                0x08
#define P_Register                0x09
#define E_Register                0x0A
#define IRMS_Register             0x0B
#define VRMS_Register             0x0C
#define TBC_Register              0x0D
#define Status_Register           0x0F
#define Mask_Register             0x1A


 
#define Cirrus_Status       0xFFFFFF

#define Cirrus_Mask         0x000000    /*   disable interrupts   */ 

/* 
   this is a counter 
   the actual time difference depends on the rate the PowerMeter_Accum() function is called
*/
#define CIRRUS_NOTCOMM_TIMEOUT		2000		

/**********************************************************************/
/*	Following values are used only for the crash test                 */
/**********************************************************************/

#define DefaultConfigRegister	0x000061UL  /* VHPF=en IHPW=en K=1*/
#define DefaultIoffset				0x000000UL
#define DefaultIgain  				0x200000UL	/* gain 1/2 */
#define DefaultVoffset				0x000000UL
#define DefaultVgain          0x200000UL  /* gain 1/2  */                        
#define DefaultCirrusCycle		0x000FA0UL  /* 4000 */
#define DefaultPrate			  	0x007D00UL  /* 500 */
#define DefaultTBC					  0x800000UL


/* 
  Kvv is to obtain V measure in 100mV units and is obtained as:
  (250mV Full_Scale_Cirrus /( Vgain * partitore resistivo V))*10 =
  (250m/(0.5*0.000563841))*10
  Then it has been multiplied by 2^2 to obtain better resolution
*/
/*
#define DefaultKVV_ma		35470   
#define DefaultKVV_exp		2
*/

/* Adjusted */
/* value trimmered according to actual measurements */
#define DefaultKVV_ma		34867U   
#define DefaultKVV_exp		2


/*
Kii is to obtain I measure in 100mA units and is obtained as
(250mV Full_Scale_Cirrus/ (Igain* Rserie))*10 = (250m/(0.5*5m))*10 =1000
Then it has been multiplied by 2^6 to obtain better resolution

#define DefaultKII_ma		64000
#define DefaultKII_exp		6
*/

/* I in unit of 10mA*/
#define DefaultKII_ma		40000U
#define DefaultKII_exp		2



/* CirrusKPP= 88652;	 (4919*2)*(440*2)*1.024 /100 (because CirrusKII is in 10mA unit) Watt!!! */
/*  
#define DefaultKPP_ma    44326
#define DefaultKPP_exp   -1
*/  
/*
Kpp is to obtain P measure in 100mW units and is obtained as
(Kvv(100mV)*Kii(100mA))/10
*/
//#define DefaultKPP_ma    55423
/* value trimmered according to actual measurements */
#define DefaultKPP_ma    50000U
#define DefaultKPP_exp     -4


/*
Kee = Kpp(100mW)/3600*10 [Wh]
*/
#define DefaultKEE_ma   45511U
#define DefaultKEE_exp  11   

/*
#define DefaultI_in    	0x0003  
#define DefaultV_inf		0x0003  

#define DefaultKVMAX	  420 
*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */


#endif /* __CIRRUS5460DEF_H */

/* *********** */
/* End of file */
/* *********** */





