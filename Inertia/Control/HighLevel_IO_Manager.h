/**
\defgroup DW_HL_IO_Manager	Modulo Gestione Input-Output DW

Il modulo contiene le funzioni di gestione di ingressi e uscite a livello applicazione

\par \b PCLint: x Errors, 7 Warnings, x Info 12/06/2008 Valeria Catani


\date    30/01/2008
\author  Valeria Catani, Roberto Fioravanti
\version 1.0.0
\ingroup ApplControl
*/

/**
Header file del file DW_HL_IO_Manager.c

\file    DW_HL_IO_Manager.h
\ingroup DW_HL_IO_Manager
\date    30/01/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/

/**
\addtogroup DW_HL_IO_Manager
@{*/


#ifndef __DW_HL_IO_MANAGER_H
#define __DW_HL_IO_MANAGER_H


/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* campi Sx Tabella generic Input */
#define GI_WORD 0x01
#define GI_BYTE 0x00

/* campo Int Tabella generic Input */
#define GI_INTERPOLATE_ENABLE 1

/*Dimensione del singolo item della tabella dei Generic Input in WORD*/
#define GI_ITEM_SIZE	2

/*#define FIXED_ANALOG_PROBE_LEN	2*/   /*AnalogProbe_Struct length (bytes)*/
#define FIXED_GI_TAB_LEN	(sizeof(TabHeader_t))   	/*TabHeader_Struct length (word)*/

/*#define PROBE_SHORT_TABLE		((p_ProbeParams -> TabsFlags) & b1) >> 1*/ /*ELIMINARE LA DOPPIA POSSIBILITA' 8 O 10 BIT*/

/*estremi di validita dei valori analogici*/
#ifdef RESOLUTION_10BIT
	#define DEFAULT_INF_ANALOG_VAL	5		/*??*/
	#define DEFAULT_SUP_ANALOG_VAL	1020	/*??*/
#else
	#define DEFAULT_INF_ANALOG_VAL	5		/*??*/
	#define DEFAULT_SUP_ANALOG_VAL	250		/*??*/
#endif

/*estremi in gradi centigradi*/
#define DEFAULT_INF_DEGREES_VAL		0
#define DEFAULT_SUP_DEGREES_VAL		255


#define GI_SPA_TBL_HIBOUND_VAL(p)        OFFS_B(p,0)
#define GI_SPA_TBL_LOBOUND_VAL(p)        OFFS_B(p,1)
#define GI_SPA_TBL_SAMPLES_VAL(p)        OFFS_B(p,2)
#define GI_SPA_TBL_FLAGS_VAL(p)          OFFS_B(p,3)

#define GI_SPA_TBL_FLAGS_ON_MSK     

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
	#define NEW_BLDC_ENABLE_FLAG_POS   0
	#define NEW_BLDC_STALLO_FLAG_POS   1
	
	#define DISABLED_OP_ALGO	0
	#define ON_OP_ALGO			1
	#define OFF_OP_ALGO			2
	#define REDUX_OP_ALGO		3
	

	#define OVER_POWER_DURATION			(OFFS_B(Tab_SFT_MotorWash.Data,1) & 0x7F) /* generalmente 5 sec*/
	#define OVER_POWER_TIMEONOFF 		(OFFS_B(Tab_SFT_MotorWash.Data,2)& 0x7F) /* generalmente 5 sec*/	
	#define MIN_SPEED					(OFFS_B(Tab_SFT_MotorWash.Data,3)*ALT_CTRL_BLDC_SPEED_MUL)	/* moltiplicato per 25*/
	#define OVER_POWER_SPEED_REDUCTION	OFFS_W(Tab_SFT_MotorWash.Data,2) 
	#define OVER_POWER_THR				OFFS_W(Tab_SFT_MotorWash.Data,3) 

	#define FILTER_WASH_VS				OFFS_B(Tab_SFT_MotorWash.Data,0)& 0x7F
	
#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */


#define FILTER_DRAIN_VS     OFFS_B(Tab_SFT_MotorDrain.Data,6)& 0x7F

#define VS_UNDEFINED   4
#define VS_UNDEFINED_NEW 6


#define DRAINPUMP_OVERHEAT_THRESHOLD            (OFFS_W(Tab_SFT_MotorDrain.Data,0))
#define DRAINPUMP_REDUX_OVERHEAT_DURATION       (OFFS_W(Tab_SFT_MotorDrain.Data,1) )
#define DRAINPUMP_OVERHEAT_SPEED_REDUCTION		(OFFS_B(Tab_SFT_MotorDrain.Data,4)*ALT_CTRL_BLDC_SPEED_MUL)	/* moltiplicato per 25*/
#define DRAINPUMP_OFF_OVERHEAT_DURATION         (OFFS_B(Tab_SFT_MotorDrain.Data,7) & 0x7F)
#define DRAINPUMP_ON_OVERHEAT_DURATION			(OFFS_W(Tab_SFT_MotorDrain.Data,4) )
#define DRAINPUMP_SAFETY_MIN_SPEED              (OFFS_B(Tab_SFT_MotorDrain.Data,5)*ALT_CTRL_BLDC_SPEED_MUL)

#define DRAIN_SFT_REQ_SPEED	((Tab_SFT_EV.DataLenght >2)?(OFFS_B(Tab_SFT_EV.Data,4)*ALT_CTRL_BLDC_SPEED_MUL):3200)
#define DRAIN_SFT_REQ_TIME 	((Tab_SFT_EV.DataLenght >2)?(OFFS_B(Tab_SFT_EV.Data,3)*DRAINPUMP_CTRL_BLDC_RAMP_TIME_MUL):1)


#define MIN_SPEED_WASH 		((Tab_SFT_MotorWash.DataLenght >1)?(OFFS_B(Tab_SFT_MotorWash.Data,3)*ALT_CTRL_BLDC_SPEED_MUL):1200)
#define SPEEDY_STEP_WASH 	((Tab_SFT_MotorWash.DataLenght >1)?(OFFS_B(Tab_SFT_MotorWash.Data,2)*ALT_CTRL_BLDC_SPEED_MUL):200)
#define HIGH_IMPROB_SECURITY_FILTER ((Tab_HWCVS_data.DataLenght>2)?(OFFS_B(Tab_HWCVS_data.Data, 4)& 0x7F) :60)
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void HIO_loadsManagement(void);
void HIO_loadsReset(void);
uint8_t HIO_ldPosInit(void);
void HIO_saltCriterionRegMgmt(void);
void HIO_gICriteriaOld_Update(void);
void HIO_inputManagement(void);
void HIO_consistencyManagement(void);
void HIO_updateEngineData(void);
void HIO_trbReadEnable(uint8_t id);
void HIO_trbReadDisable(uint8_t id);

void Fan_GenericLoadMgmt(void);

void HIO_OverHeat_DrainPump_Control(void);

#endif /* __DW_HL_IO_MANAGER_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
