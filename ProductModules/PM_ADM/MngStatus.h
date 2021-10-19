/**
MngStatus.h
contiene le define e i prototipi delle funzioni definite in MngStatus.c

\file    MngStatus.h
\ingroup  PM_ADM
\version 0.0.1
\date    Apr X 2010
\author  Andrea Mantovani
*/

/*
Copyright (c) 2007-2008, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


#ifndef MngStatus_H
#define MngStatus_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
#include "StandardDefines.h"
#include "ApplDataMng_cfg.h"
#include "ApplDataMng_full.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *********** */
/* Door status */
/* *********** */
#define DOOR_OPEN		  0
#define DOOR_CLOSED		1


/* Appliance Models */
#define APPLIANCE_CL				0	// Cooling
#define APPLIANCE_DW				1	// Diswasher
#define APPLIANCE_WM				2	// Wash Machine
#define APPLIANCE_CK				3	// Coking
#define APPLIANCE_DR				4	// Dryer
#define APPLIANCE_HO				5	// Hobs
#define BRIDGE_CECED        10 // Indoor Communication Node (Bridge, HW Key, ZigBee, etc..)
/* Unique Code Visual Fw */
#define	CK_DOUBLE_OVEN			15
#define CK_BIG_CAVITY				16
#define	DW_INDESIT_BASIC		26
#define DW_INDESIT_PREMIUM	27


/* Stati Generali del prodotto */
#define STATUS_PROGRAMMING	IC_PROGRAMMING
#define STATUS_RUN			IC_RUN
#define STATUS_DELAY		IC_DELAY
#define STATUS_END			IC_END
#define STATUS_PAUSE		IC_PAUSE
#define STATUS_FAULT		IC_FAULT
#define	STATUS_UIOFF		6	// stato di OFF logico nel display: la main è in programming
#define	STATUS_INVALID		7	// invalid data: i dati del pacchetto di stato sono invalidi



/*Indici del buffer di risposta al pacchetto READ STATUS*/
#define OFFSET_FLAG      0

/* APPLIANCE_CL */
#define	OFFSET_CYCLE_FR           1
#define	OFFSET_CYCLE_FZ           5
#define	OFFSET_CYCLE_VR           13
#define	OFFSET_TEMP_REG_FR        32
#define	OFFSET_TEMP_REG_FZ        33
#define	OFFSET_ALARM3             39
#define	OFFSET_PROBES_FAULT       40
#define	OFFSET_SPECIAL_FUNC       43
#define	OFF_CYCLE_FR              4
#define	OFF_CYCLE_FZ              7

/* APPLIANCE_CL2008 */
#define	OFFSET_CYCLE_FR_CL2008           10
#define	OFFSET_CYCLE_FZ_CL2008           12
#define	OFFSET_CYCLE_SP_CL2008           14
#define	OFFSET_CYCLE_VR_CL2008           16

#define	OFFSET_TEMP_REG_FR_CL2008        
#define	OFFSET_TEMP_REG_FZ_CL2008        
#define	OFFSET_ALARMS_CL2008              1  
#define	OFFSET_PROBES_FAULT_CL2008       
#define	OFFSET_SPECIAL_FUNC_CL2008       
#define	OFF_CYCLE_FR_CL2008              
#define	OFF_CYCLE_FZ_CL2008              


/* APPLIANCE_CK */
#define OFFSET_OVEN_FLAG             0
#define	OFFSET_MAINCAVITY_CYCLE_CK   1
#define	OFFSET_MAINCAVITY_PHASE_CK	 2
#define	OFFSET_CRITERIA_CK	         3
#define	OFFSET_CLOCK			           4
#define	OFFSET_DURATIOM_MAINCAV	     6
#define	OFFSET_PROBE_MAINCAV	       8
#define	OFFSET_DELAY_MAINCAV  	    10

/* APPLIANCE_WM */
#define	OFFSET_CRITERIA_WM        1
#define OFFSET_DELAY_HI           4
#define OFFSET_DELAY_LO           5
#define OFFSET_TEMPERATURE        6
#define	OFFSET_CYCLE	            2
#define	OFFSET_PHASE	            3

/* APPLIANCE_DW */
#define	DW_RS_FLAGS_1				      1
#define	OFFSET_MAINCAVITY_CYCLE		2
#define	OFFSET_MAINCAVITY_PHASE		3
#define	DW_RS_DELAY_HOURS			    4
#define	DW_RS_DELAY_MINUTES			  5
#define	DW_RS_DELAY_SECONDS			  6

/* APPLIANCE_DR */
#define	OFFSET_CYCLE_DR	          0
#define	OFFSET_PHASE_DR	          1
#define	OFFSET_CRITERIA_DR        2
#define	OFFSET_FAULT_DR           3
#define OFFSET_TEMPERATURE_DR     4
#define OFFSET_DELAY_HI_DR        5
#define OFFSET_DELAY_LO_DR        6
#define OFFSET_ALARM1_DR          7
//nello stesso buffer dello stato ci riverso sia la lettura con il 0x60 che con 0x1E, la seconda lettura è spiazzata di DRYER_STATUS_LEN
#define	OFFSET_CYCLE_UI_DR	      0+DRYER_STATUS_LEN
#define OFFSET_FLAGS_LO_DR        1+DRYER_STATUS_LEN
#define OFFSET_FLAGS_HI_DR        2+DRYER_STATUS_LEN
#define OFFSET_DELAY_DR           5+DRYER_STATUS_LEN
#define OFFSET_DRY_DR             6+DRYER_STATUS_LEN
#define OFFSET_INTENS_DR          7+DRYER_STATUS_LEN
#define OFFSET_BACKUP_FLAGS_DR    9+DRYER_STATUS_LEN
#define OFFSET_TTE_HI_DR          12+DRYER_STATUS_LEN
#define OFFSET_TTE_LO_DR          13+DRYER_STATUS_LEN


/* *************** */
/* Halfload values */
/* *************** */
#define HALFLOAD_OFF			1
#define HALFLOAD_UP				2
#define HALFLOAD_DOWN			3

/* ************* */
/* 3 in 1 values */
/* ************* */
#define THREE_IN_ONE_OFF		1
#define THREE_IN_ONE_ON			2

/* *************** */
/* Extradry values */
/* *************** */
#define EXTRADRY_OFF			1
#define EXTRADRY_ON				2

/* ************ */
/* Sound values */
/* ************ */
#define SOUND_OFF				1
#define SOUND_ON				2

#define DRYER_STATUS_LEN        10    //numero di bytes letti con il pacchetto di lettura stato su Evo2

/* Intestazione Ciclo */
#define IC_PROGRAMMING		0
#define IC_RUN				1
#define IC_DELAY			2
#define IC_END				3
#define IC_PAUSE			4
#define IC_FAULT			5

/* Sub Intestazione Ciclo */
#define SIC_GENERIC			0
#define SIC_PIRO			  1
#define SIC_AUTOTEST		2
#define SIC_COLLAUDO		3
#define SIC_DEMO			  4
#define SIC_MANUAL			5
#define SIC_AUTOMATIC		6
#define SIC_RISING			7
#define SIC_ANTISHOCK		8
#define SIC_ANTIFOAM		9
#define SIC_PUMP_OUT		10
#define SIC_GRILL			  13	//per compatibilità con le vecchie UI in cui c'era solo SIC_MANUAL


// Offset intestazione programmi	// §_§ eventualmente spostare in un include più visibile
#define OFFSET_SIC_IC					    1
#define OFFSET_PROGRAM_ID_NAME		2
#define OFFSET_REG_SEL_INDEX			3

#define LOW_LEWEL_INPUT_MAP				0
#define MODULE_PIN_MAP 					  4
#define TIMEOUT_F12_DISPL				  5
                                  
#define PIN_NUMBER_BRILL_RX			  30	// pin 31
#define LLI_READ_TYPE_BRILL_RX		 1	// Analog	
#define LLI_POSITION_BRILL_RX		   1	// 2

#define UI_FI_DGT					        1



/* ------------ MACROS ------------------- */
// Maschere intestazione programmi
#define MASK_IC				0x0F
#define MASK_SIC			0xF0	//0x70

#define PROGRAM_HEADING(ptrCycleHeading)		( (*((uint8_t *)((ptrCycleHeading)) + OFFSET_SIC_IC)) & MASK_IC )
#define SUBYCLE_HEADING(ptrCycleHeading)		(( (*((uint8_t *)((ptrCycleHeading)) + OFFSET_SIC_IC)) & MASK_SIC ) >> 4)
#define PROGRAM_ID_NAME(ptrCycleHeading)		(*((uint8_t *)((ptrCycleHeading)) + OFFSET_PROGRAM_ID_NAME))
#define REG_SEL_INDEX(ptrCycleHeading)		(*((uint8_t *)((ptrCycleHeading)) + OFFSET_REG_SEL_INDEX))


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */




typedef struct 
{
	uint8_t B0 :1;
	uint8_t B1 :1;
	uint8_t B2 :1;
	uint8_t B3 :1;
	uint8_t B4 :1;
	uint8_t B5 :1;
	uint8_t B6 :1;
	uint8_t B7 :1;
}
BYTEBIT;


typedef union
{
	BYTEBIT BIT;
	uint8_t BYTE;
} 
byte_def;

typedef enum 
{
  E_STAT_OK=0,
  E_STAT_KO
} 
ADM_StatusErrType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
ADM_StatusErrType GetApplianceStatusWM(byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusDW(byte_def coldResetEvent,byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusCK_HOT2005(byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusCK_HOTTIMA(byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusDR(byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusCL(byte_def *PointerBuf, Struct_Status *RefStatus);
ADM_StatusErrType GetApplianceStatusCL2008(byte_def *PointerBuf, Struct_Status *RefStatus);


#endif /* MngStatus_H */

/* *********** */
/* End of file */
/* *********** */