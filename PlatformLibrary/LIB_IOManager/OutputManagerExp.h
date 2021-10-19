/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OutputManagerExp.h
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		OutputManagerExp.h
\ingroup	OutputsManager
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		00.00.66
\n		30/09/2010
\n		Roberto Fioravanti
\n		changes
\n		-	addition of PTR_IO_CONFIG_DATA, PTR_IO_DEVICE_DATA, PILOT_TYPE_PARAMS_OFFSET, PILOT_TYPE_FIELD
\n
\n 		00.00.64
\n		22/09/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of new PilotType BLDC_WASH_L, BLDC_DRAIN_L
\n		- addition of enum eLIB_IOM_Error
\n 		00.00.55
\n		30/07/2010
\n		Roberto Fioravanti
\n		changes
\n 		- New PilotType FIXED_SPEED_MOTOR_L
\n
\n 		00.00.64
\n		14/09/2010
\n		Stefano Ughi
\n		OUTPUT_TABLE_ITEM_SIZE_IN_WORDS moved from OutputManager.c to OutputManagerExp.h
\n
\n
*/

#ifndef __OUTPUT_MANAGER_EXP_H
#define __OUTPUT_MANAGER_EXP_H

/* ******** */
/* Includes */
/* ******** */
/* PLEASE DON'T CHANGE THESE INCLUDE TO AVOID loops: 
 This file is included in lots of files including Config.h*/
#include "autoconf.h"
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#include "Loads_cfg.h"
#endif
#include "DIO_cfg.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

#define LOAD_NOT_FOUND  		0xFF    /* Load not found by LIB_IOM_FindLoad() */
#define LOAD_DONT_EXE_SETLOAD	0xFE    /* Dont apply SetLoad */

#define LOAD_REG_ON				0x01u
#define LOAD_REG_OFF			0x00u

#define	LOAD_CFG_RELAY_ONLY		0x10u
#define	LOAD_CFG_TRIAC_ONLY		0x20u

#define	LOAD_CFG_SR_FORCE_ON	0x40u
#define	LOAD_CFG_SR_FORCE_OFF	0x80u

#define	LOAD_CFG_MR_MULTI_1		0x10u
#define	LOAD_CFG_MR_MULTI_2		0x20u

#define	LOAD_CFG_MR_FORCE_DELAY	0x80u
#define	LOAD_CFG_DONT_CARE		0x80u


/* Setting pointers */
#define EEP_OUTPUT_TABPTR_OFFSET									3		/* Output displacement vs PTR_IO_CONFIG_DATA */

#define LOAD_ID(pByte)                        	  	      			(*(pByte))   /* Load ID Field */
#define PILOT_TYPE_PARAMS_OFFSET(pByte)     						(*(pByte+4))            /* Pilot Type Params Offset vs PTR_IO_DEVICE_DATA */
#define PRIORITY_BITMAP(pByte)     									(*(pByte+5))            /* Priority Bitmap */
#define PILOT_TYPE_FIELD(pByte)             						(*(pByte+2))   /* Load Pilot Type Field */
#define LOAD_FEEDBACKS_OFFSET(pByte)                                (*(pByte+1))   /* Field Feedbacks linked to the load */
#define NUM_FEEDBACKS_LINKED(pByte)                             	(uint8_t)(*(pByte+1)&0x0F)  /* Number of feedbacks linked to the load */
#define FEEDBACK_READ_TYPE_PNTR(pByte,k)                        	(pByte+2+2*k)
#define FEEDBACK_READ_TYPE_VALUE(pByte,k)                       	(*FEEDBACK_READ_TYPE_PNTR(pByte,k))
#define FEEDBACK_POSITION_PNTR(pByte,k)                         	(pByte+3+2*k)
#define FEEDBACK_POSITION_VALUE(pByte,k)                        	(*FEEDBACK_POSITION_PNTR(pByte,k))

/* Load pilot type */
#define DIGITAL_L                   0
#define COMPTR_L                    1
#define STEPPER_L                   2
#define PROFILE_L                   3
#define ZC_L                        4
#define RELEZC_L                    5
#define BUZZER_L                    6
#define CHIPSEL_L                   7
#define ACQUACARE_L                 8
#define DUMPEROO_L                  9
#define PWM_L                       10
#define I2C_L                       11
#define TRIACRELAY_L                12
#define BLOCCO_PORTA_IMPULSI        13
#define BLOCCO_PORTA_MOTORE         14
#define BLOCCO_PORTA_CERA           15
#define CHIPSEL2_L                  16
#define DIGITALDELAYED_L	        17
#define PMSM_MOTOR_L                18
#define PLP_L			            19
#define RELAY_TRIAC_L	            20
#define EXTERN_MODULE_L             21	/* external board */
#define COLLECTORMOTOR_L            22
#define RELAY_LOAD_L	            23
#define BLOCCO_PORTA_CERA_2PIN      24
#define SCHEDA_RADIANTE             25 /* piani cottura */
#define EXTERN                      26 /* piani cottura */
#define ISI                         27 /* piani cottura */
#define ALTERNATO_L                 28
#define DIGITAL_OUTPUT_MULTI_L      29
#define BLOCCO_PORTA_MONO_IMPULSO	30
#define DIM_LAMP_L              	31
#define FIXED_SPEED_MOTOR_L         32
#define PARTIAL_AND_RELAY_L         33
#define BLDC_WASH_L                 34	/* external board */
#define BLDC_DRAIN_L                35	/* external board */
#define DIM_LED_L                   36
#define PWM_PROF_L                  37
#define DIGITAL_OUTPUT_MULTI_3P_L   38
#define PMSM_DRYER_L				39	/* external board */
#define ALTERNATO_ABS_L             40
#define DIGITALDELAYED_MULTI_L      41
#define MULTIPOWER_COMP_L           42	/* external board */
#define HOODMOTOR_4RELAY_L          43
#define CHIPSEL_OUT_L				44
#define BRUSHLESS_HOOD_L            45	/* external board */
#define AIRSENSOR_HOOD_L            46	/* external board */
#define DRUM_LOCK_L                 47
#define PROBES_AND_SENSORS_L        48	/* external board */
#define BLOCCO_PORTA_MR				49
/* Returns Values of LIB_IOM_SetLoad */
#define SET_LOAD_NO_ERROR         	0
#define SET_LOAD_ERROR              1


/* Returns Values of LIB_IOM_GIOM_SetLoad  */
#define GIOM_SET_LOAD_NO_ERROR    		0 /**< no error */
#define GIOM_USE_STANDARD_SET_LOAD		1 
#define GIOM_ERROR_LOAD_NOT_PRESENT    	2

/* Size in words of a single item inside the output map */
#define OUTPUT_TABLE_ITEM_SIZE_IN_WORDS 	4

#define OUTPUT_TABLE_ITEM_SIZE_IN_BYTES         (OUTPUT_TABLE_ITEM_SIZE_IN_WORDS * 2)               /* Size in bytes of a single item inside the output map */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/** LIB_IOManager errors */
typedef enum eLIB_IOM_Error
{
    /** no error */
	LIB_IOM_NO_ERROR=0,
	LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR,
	LIB_IOM_EXTERNAL_GETCMD_ERR,
	LIB_IOM_EXTERNAL_SETCOMMAND_ERR,
	LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR,
	LIB_IOM_EXTERNAL_SETWRITERAMREQ_ERR,
	LIB_IOM_EXTERNAL_SETEXTRAREQ_ERR
} LIB_IOM_ErrorType;

/*
Basic Structure for Loads
Don't modify this structure !!! */
#ifndef L_PARAM_TYPEDEF
typedef union
{
        uint8_t b;
        struct
        {
#if defined(__BIT_ORDER_MSB_FIRST__)
            uint8_t config: 4;  /* load's configuration */
            uint8_t reg:    4;  /* load's regulation */
#else
            uint8_t reg:    4;  /* load's regulation */
            uint8_t config: 4;  /* load's configuration */
#endif
        } s;

} L_PARAM;

#define LOAD_REG_MASK   (0x0Fu)
#define LOAD_CFG_MASK   (0xF0u)
#define L_PARAM_TYPEDEF
#endif


/** Extended Structure for Loads must be always defined even though extended loads are not used
 * BEWARE! loadID is used for backward compatibility only
 * Its real meaning is loadPos - load position into setting file!
 * */

typedef struct
{
	uint8_t loadID;
	L_PARAM loadParam;
	uint8_t data[CONFIG_EXT_LOAD_PARAMDATA_SZ];
} GIOM_LoadStruct;

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
#if !defined(MODULE_BODY_LIB_IOMANAGER)
extern const uint8_t LoadsStatus[N_MAXLOADS];
#if defined(CONFIG_NUM_EXTDLOADS)
extern const uint8_t *ExtdLoadsStatus[CONFIG_EXT_LOAD_PARAMDATA_SZ];
#endif
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
uint8_t LIB_IOM_SetLoad( uint8_t loadID, L_PARAM loadParam);
bool_t LIB_IOM_GIOM_SetLoad (const GIOM_LoadStruct *const pLoad);
uint8_t LIB_IOM_GetLoadStatus(uint8_t position);
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0)
uint8_t LIB_IOM_GetExtdLoadStatus(uint8_t extLoadItm, uint8_t pExtdStatus[CONFIG_EXT_LOAD_PARAMDATA_SZ]);
#endif /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) && defined(CONFIG_NUM_EXTDLOADS) && (CONFIG_NUM_EXTDLOADS > 0) */
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
void LIB_IOM_GetAllLoadsStatus(uint8_t *newLoadStatus, uint8_t (*newExtdLoadStatus)[CONFIG_EXT_LOAD_PARAMDATA_SZ]);
#endif

uint8_t LIB_IOM_NumberOfLoads(void);
uint8_t LIB_IOM_GetLoadMaxRegValue(uint8_t loadPos);
uint8_t LIB_IOM_GetLoadPilotType(uint8_t loadPos);
uint8_t LIB_IOM_GetLoadPinTabTable(uint8_t loadPos, uint8_t *PinTabPtr);
uint8_t LIB_IOM_GetLoadPilotParamTable(uint8_t loadPos, Loader_Struct *ParmTableLdrPtr);
uint8_t LIB_IOM_GetLoadPriorityBitmap(uint8_t loadPos);

#endif	/* __OUTPUT_MANAGER_EXP_H */

/* *********** */
/* End of file */
/* *********** */

