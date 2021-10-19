/**
\defgroup ApplControl 	Application Control Modules
\defgroup ApplSettings	Application Settings Modules
\defgroup ApplGeneral	Application General Modules
\defgroup AppluProgr	Application uPrograms Modules
\defgroup ApplUI		Application UI Modules
*/


/**
\defgroup	CstmDefines Modulo defines condivise
\ingroup	ApplGeneral

Il modulo contiene le define che sono utilizzate dagli altri file di applicazione

\par \b PCLint: x Errors, x Warnings, x Info 14/01/2008 Valeria Catani


\date    14/01/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplGeneral
*/

/**
Header file del modulo defines condivise

\file    CstmDefines.h
\ingroup CstmDefines
\date    14/01/2008
\author  Valeria Catani
\version 1.0.0
*/
/**
Header file del modulo defines condivise

\file    CstmDefines.h
\ingroup CstmDefines
\date    27/09/2013
\author  Andrea Emili
\version 1.0.0
\note
\	-Adeguamento file per Inertia e Plt2.5
*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __CSTM_DEFINES_H
#define __CSTM_DEFINES_H

/* ******** */
/* Includes */
/* ******** */
#ifndef AUTOCONF_INCLUDED
#include "autoconf.h"
#endif /* !AUTOCONF_INCLUDED */
#include "StandardDefines.h"
#include "DIO_cfg.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** APPLICATION CONFIG **/
/** Functions & Features */




/** Enable Statistics */
#define _STATISTICS_ON_
#define  VIRTUAL_SENSOR_ENABLE  1



/**
COUNTERS
*/
#define MTN_COUNTERS	1 /* contatori uP Maintain */
#define LDC_COUNTERS	2 /* contatori uP Loads Control */
#define DRN_COUNTERS	1 /* contatori uP Drain */
#define SLT_COUNTERS	2 /* contatori uP 7litri */
#define ALT_COUNTERS	2 /* contatori uP alternato */
#define LOD_COUNTERS	4 /* contatori uP load */
#define LODRES_COUNTERS	1 /* contatori uP load  restore*/
#define REG_COUNTERS	1 /* contatori uP regenerate */
#define SPF_COUNTERS	1 /* contatori uP set profile  */
#define SKL_COUNTERS	1 /* contatori uP SPARKLING*/
#define GNRL_COUNTERS 	2 /* contatori generali , I/O, bulk, delay*/
#define SFT_COUNTERS	6 /* contatori sicurezze ( door, press stable,dry res, overheating e fan ,&LoadEVCheck),time_4_unblock_EV_VS_counte*/
#define FLT_COUNTERS    6 /* contatori per fault ( Pulsestill e Pulse check delay,FLT_UNDER_VOLTAGER,FLT_OVERHEATING_RES_OFF_M_COUNTER,FLT_HEATTOUT_M_COUNTER,MP_LOADTIMEOUT_REST_M_COUNTER)*/
#define STA_COUNTERS	1 /* contatori per statistiche */
#if (VIRTUAL_SENSOR_ENABLE == 1)
#define VS_COUNTERS     6 /* contatori sensori virtuali */
#else
#define VS_COUNTERS     0
#endif
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#define POWER_PROFILE_COUNTERS			4/* TimeBtwPhases_Counter, EnPhElapsedTime_Counter, tte_enphnospecialselapsedtime_Counter */
/* Timeouts */
#define ENERGY_PHASE_PAUSE_TIMEOUT		5
#define PP_ENERGYPAUSE_STATUS			TRUE
#define PP_MULTIPLE_ENERGYPAUSE_STATUS TRUE
#define PP_SAVE_ELAPSED_POWERPHASE_COUNTER TRUE
#define MAINTAIN_LOADS_AFTER_PHASE TRUE
#define APP_PAUSE_STATE_BACKUP TRUE
#else /* CONFIG_PM_POWER_PROFILE_SUPPORT */
#define POWER_PROFILE_COUNTERS			0	/* No APP */
#define PP_ENERGYPAUSE_STATUS			FALSE
#define PP_MULTIPLE_ENERGYPAUSE_STATUS FALSE
#define PP_SAVE_ELAPSED_POWERPHASE_COUNTER FALSE
#define MAINTAIN_LOADS_AFTER_PHASE FALSE
#define APP_PAUSE_STATE_BACKUP FALSE
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

#define SWTC_COUNTERS_NUM			(	SWTC_APP_PLT_COUNTERS_NUM + \
										SWTC_PLATFORM_COUNTERS_NUM + \
										MTN_COUNTERS + \
										LDC_COUNTERS + \
										DRN_COUNTERS + \
										SLT_COUNTERS + \
										ALT_COUNTERS + \
										VS_COUNTERS  + \
										SPF_COUNTERS + \
										LOD_COUNTERS + \
										SKL_COUNTERS + \
										LODRES_COUNTERS + \
										REG_COUNTERS 	+ \
										SFT_COUNTERS    + \
										FLT_COUNTERS    + \
										STA_COUNTERS	+ \
										GNRL_COUNTERS	+ \
										POWER_PROFILE_COUNTERS)

#define SWTC_PRESCALERS_NUM				(8)

#if (SWTC_PRESCALERS_NUM>256)
#error "SWTC_PRESCALERS_NUM: too many prescalers."
#endif

#if (SWTC_COUNTERS_NUM>256)
#error "SWTC_COUNTERS_NUM: too many timercounters."
#endif

enum Tbase_NewCounters{
/**
configurare opportunamente questa struct sulla base della
tabella "BEGIN__SWTC_PRESCALERS_TABLE"
*/
	COUNTERS2_TBASE_10MS = 0,
	COUNTERS2_TBASE_100MS,
	COUNTERS2_TBASE_200MS,
	COUNTERS2_TBASE_1SEC,
	COUNTERS2_TBASE_2SEC,
	COUNTERS2_TBASE_10SEC,
	COUNTERS2_TBASE_1MIN,
	COUNTERS2_TBASE_15MIN,
};

#if ( COUNTERS2_TBASE_NUM > 15 )
#error "\n SW Timers prescaler field size is 4 bit, therefore ... "
#endif /* ( COUNTERS2_TBASE_NUM > 15 ) */
/**/



#ifdef _CHECKERRORS_
	#define DEBUG_BUFFER	20
#endif



#define	SWTC_APP_COUNTERS_NUM			48

#define SWTC_APPLICATION_COUNTERS_NUM	(SWTC_APP_COUNTERS_NUM + SWTC_APP_PLT_COUNTERS_NUM)



#define N_UI_BACKUP_DATA		16	/* dimensione buffer riservato per backup UI */
#define N_UI_PHASE_BMP			10	/* dimensione riservata al salvataggio delle fasi eseguite (nel buffer backup UI) */
#define INT_CYCLE_ACTIVE_POS		4 /* Posizione nel buffer dei dati di backup UI del ciclo corrente*/
/**
FLAGS
*/

/**Flags_1*/
#define JIF_FLAG1_LOAD_RESTORE_KO_POS     	0 	/* overflow ripristina */
#define JIF_FLAG1_REGENERATE_OK_POS       	1 	/* rigenera effettuata */
#define JIF_FLAG1_PULSE_COUNTER_KO_POS    	2 	/* turbina bloccata */
#define JIF_FLAG1_7LITER_TIMER_EXPIRED_POS 	3 	/* 7 liters */
#define JIF_FLAG1_UP_TERMINATED_KO_POS      4
#define JIF_FLAG1_ALGO_DIRTY_F_KO_POS		5
#define JIF_FLAG1_TARGET_TEMP_OK_POS	    6	/*forse non necessario*/
#define JIF_FLAG1_MAINTAIN_TARGET_OK_POS	7   /*forse non necessario*/

/** Flags_2*/
#define FLAGS2_REED_NO_SPARKL_BIT        0   	/* segnalazione mancanza brillantante da reed */
#define FLAGS2_TURB_NO_SPARKL_BIT        1   	/* segnalazione mancanza brillantante da lettura torbidità */
#define FLAG2_REGENERATE_ON_POS          2   	/*per ora non in editor da JumpIF*/
#define FLAG2_SAFETY_DRAIN_PP_ON_POS     3   	/* scarico per p.p. ON (safety)*/
#define FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG	4	/* indica che il test è partito */
#define FLAG2_CLOGGED_FILTER			 5
#define FLAG2_SAFETY_TURB_TEST_BIT		 6
#define FLAG2_HIO_READ_SPARKLING_DONE	 7

#define FLAGS2_NO_SPARKL_AID_MSK         ((MASK_B(FLAGS2_TURB_NO_SPARKL_BIT))|(MASK_B(FLAGS2_REED_NO_SPARKL_BIT)))

/** Flags_2*/
#define JIF_FLAG3_WASH_UNDERVOLTAGE_WARN	0 /* warning undervoltage */
#define JIF_FLAG3_WASH_OVERHEATING_WARN		1 /* warning overheating */
#define JIF_FLAG3_HEAT_TOUT_WARN 			2 /* warning heat timeout */
#define JIF_FLAG3_VIRTUALSENSOR_END_CYCLE	5

/** ApplicationFlags */
/* Bit position in ApplicationFlags */
enum
{
  AF_NO_SALT_BIT=0,
  AF_NO_SPARKLING_AID_BIT,
  AF_H2O_WARN_BIT,
  AF_DOOR_CLOSED_BIT,
  AF_STANDBY_DISABLE_BIT,	/*AF_UNUSED1_BIT,*/
  AF_DIRTY_FILTER_BIT,/* indica se il filtro è otturato*/
  AF_SBY_SLEEP_ON_FLAG,/*se '1' sleep mode abilitato (indica che la Main andrà in sleep entro un parametro */
  AF_WAKEUP_OFBUTTON_BIT,/* la causa di reset freddo è stata la pressione del tasto ON/ OFF*/
};

/* Maschera dei bit di ApplicationFlags aggiornati in UpdateStateBitmaps */
#define APPL_FLAGS_MASK	  ( MASK_B(AF_NO_SALT_BIT) | \
							MASK_B(AF_NO_SPARKLING_AID_BIT) | \
							MASK_B(AF_H2O_WARN_BIT) | \
							MASK_B(AF_DOOR_CLOSED_BIT) | \
							MASK_B(AF_STANDBY_DISABLE_BIT) | \
							MASK_B(AF_DIRTY_FILTER_BIT) | \
							MASK_B(AF_SBY_SLEEP_ON_FLAG)| \
							MASK_B(AF_WAKEUP_OFBUTTON_BIT) )

/* Valore di inizializzazione di ApplicationFlags in UpdateStateBitmaps() */
#define APPL_FLAGS_INIT_VAL		( MASK_B(AF_DOOR_CLOSED_BIT)  )		 /*COMPLETARE se serve*/
/**/

/** ApplicationState */

/* Bit position in ApplicationState */


/* Maschera dei bit di ApplicationState aggiornati in UpdateStateBitmaps() */
#define APPL_STATE_MASK			( MASK_B(AS_DATA_READY_BIT) | MASK_B(AS_REMOTE_CTRL_ON_BIT) | MASK_B(AS_APPL_FAULT_BIT) )

/* Valore di inizializzazione di ApplicationFlags in UpdateStateBitmaps() */
#define APPL_STATE_INIT_VAL		MASK_B(AS_DATA_READY_BIT)



/** W-BOX CALIBRATIONS OFFSET */
/* Indesit Plant Cali-Box Dynamic Part Offset */
/** Coefficiente riduzione resistenza lavaggio */
#define WASH_RESISTOR_CAL_OFFSET	0x00
/** Coefficiente riduzione resistenza asciugatura */
#define DRY_RESISTOR_CAL_OFFSET		0x01
/** End of line Id */
#define END_OF_LINE_ID_OFFSET		0x02
/** Unused offset, alignment byte */
#define UNSED_CALI_BOX_OFFSET		0x03



/**
FAULT
*/ 


#define PULSE_COUNTER_STILL_INIT  0
#define PULSE_COUNTER_STILL_EXE   1


#define FLT_LOAD_TOUT_DISABLE     0
#define FLT_LOAD_TOUT_INIT        1
#define FLT_LOAD_TOUT_EXE         2
#define FLT_LOAD_TOUT_END         3
/* QI 05.11.09 Gestione timeout su caricamento dinamico */
#define FLT_PUMP_LOAD_INIT				0
#define FLT_PUMP_LOAD_EXE					1

#define FLT_HEAT_TOUT_DISABLE     0
#define FLT_HEAT_TOUT_INIT        1
#define FLT_HEAT_TOUT_EXE_P       2
#define FLT_HEAT_TOUT_EXE_F       3
#define FLT_HEAT_TOUT_END         4


/* F01: overflow */
#define FLT_CODE_OVERFLOW       1

  #define FLT_SUBCODE_OVERFLOW  0x01
/* F02: EV KO */
#define FLT_CODE_LOAD_EV_KO     2

  #define FLT_SUBCODE_LOAD_EV_KO      0x01
  #define FLT_SUBCODE_PULSE_LOW_RANGE 0x02
  #define FLT_SUBCODE_EV_VIA_VS   	  0x03

/* F03: drain */
#define FLT_CODE_DRAIN          3
  #define FLT_SUBCODE_DRAIN    0x01

/* F04: probe */
#define FLT_CODE_PROBE          4

  #define FLT_SUBCODE_PROBE_CC  0x01
  #define FLT_SUBCODE_PROBE_CA  0x02

/* F05: press switch */
#define FLT_CODE_PRESS_SWITCH   5

  #define FLT_SUBCODE_PRESS_SWITCH_LOAD       0x01
  #define FLT_SUBCODE_PRESS_SWITCH_HEAT       0x02
  #define FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT  0x03

/* F06 : timeout caricamento (rubinetto ostruito) */
#define FLT_CODE_LOAD_TIMEOUT   6

  #define FLT_SUBCODE_LOAD_TIMEOUT   0x01

/* F07: turbina */
#define FLT_CODE_PULSE_COUNTER  7

  #define FLT_SUBCODE_PULSE_COUNTER  0x01


/* F08: timeout riscaldamento */
#define FLT_CODE_HEAT_TOUT      8

  #define FLT_SUBCODE_HEAT_TOUT  0x01


/* F09: setting */
#define FLT_CODE_SETTING_FILE		9	/* Fault file di setting */

	#define FLT_SUBCODE_MAIN_BOARD_SETTING_FILE		0x01	/* rilevato da scheda main */

/* F10: resistenza aperta */
#define FLT_CODE_RES_OPEN       10

  #define FLT_SUBCODE_RES_OPEN  0x01

  /* F11: BLDC Comm */
#define FLT_CODE_BLDC			11

#define FLT_CODE_VS      15
  #define FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH  1
  #define FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN  2


/* F13: circuiti scheda main */
#define FLT_CODE_MAIN_BOARD     13

	/* circuito resistenza */
	#define FLT_SUBCODE_RELE_RES_CC				0x01
	#define FLT_SUBCODE_RELE_RES_CA				0x02
	#define FLT_SUBCODE_FEEDBACK_KO				0x03
	#define FLT_SUBCODE_GENERIC_ERROR			0x04

	/* circuito porta */
	#define FLT_SUBCODE_DOOR_OUT_OF_RANGE		0x10

	#define FLT_SUBCODE_BLDC_COMM				0x18

	/* circuito pressostato */
	#define FLT_SUBCODE_PRESS_OUT_OF_RANGE  	0x20
	#define FLT_SUBCODE_OVERFLOW_OUT_OF_RANGE	0x30


	#define FLT_SUBCODE_BLDC_UNDER_VOLTAGE		0x40	/* Verificare F_F */

	#define FLT_SUBCODE_ZC_NOT_GOOD				0x50

	#define FLT_SUBCODE_BLDC_OVERHEATING		0x88



#define N_MAX_LAST_FAULT		5	/* Numero ultimi fault e prefault da salvare */

/**
WARNING
*/ 
#define WARNING_DRAIN_KO		0
#define WARNING_DRAIN_PREKO		1
#define WARNING_LOADPP_KO		2
#define FLAG_BLDC_UNDVL			3
#define FLAG_BLDC_OH			4
#define FLAG_BLDC_HEATTOUT		5
#define FLAG_BLDC_PRESS_SWITCH	6
#define FLAG_PULSE_KO_LOAD		7
#define FLAG_LOADEV_KO			8


#define PUMPOFF   0
#define PUMP_TRES_OFF 1
#define	PUMP_CHANGE_PILOT	2
#define PUMP_STALLO	3
#define UPDATE_FAULT	4

#define BLDC_NOTHING  NR_OF_BLDC_PUMPS /* il valore max dell'indice viene usato per indicare nessun indice */
#define BLDC_FAULT_DRAIN    DRAIN_BLDC_PUMP
#define BLDC_FAULT_WASH     WASH_BLDC_PUMP
/**
STRUTTURA MICROPROGRAMMI
*/
#define PULSE_COUNTER_STILL_INIT  0
#define PULSE_COUNTER_STILL_EXE   1

/**
MICROPROGRAMMI (opcode)
*/

/**
INPUTS
*/
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
enum thatManyBLDC_Pumps{
	WASH_BLDC_PUMP=0,
	DRAIN_BLDC_PUMP,
    NR_OF_BLDC_PUMPS
};

#define SET_BLDC_SPEED(p, val)		{\
    (*(uint8_t*)(p.data)) = ( (uint8_t)( (((uint16_t)(val)) >> BYTE_SIZE) & 0xFF) );\
    (*(uint8_t*)(p.data+1)) = ( (uint8_t)( ((uint16_t)(val)) & 0xFF) );\
    p.loadParam.b=((val)?(LOAD_ON):(LOAD_OFF));\
}

#define SET_BLDC_TIME(p, val)		{\
    (*(uint8_t*)(p.data+2)) = ( (uint8_t)( (((uint16_t)(val)) >> BYTE_SIZE) & 0xFF) );\
    (*(uint8_t*)(p.data+3)) = ( (uint8_t)( ((uint16_t)(val)) & 0xFF) );\
}
#define SET_BLDC_FLAG(p, bitPos)	SETBIT(*((uint8_t*)(p) + 4), (uint8_t)(bitPos))

#define GET_BLDC_SPEED(p)  MAKEWORD(p.data[0], p.data[1])
#define GET_BLDC_TIME(p)  MAKEWORD(p.data[2], p.data[3])

/* coefficienti rappesentazione dati */
#define PUMP_CTRL_BLDC_SPEED_MUL            25
#define PUMP_CTRL_BLDC_RAMP_TIME_MUL        5
/* coefficienti rappesentazione dati */
#define DRAINPUMP_CTRL_BLDC_SPEED_MUL            25
#define DRAINPUMP_CTRL_BLDC_RAMP_TIME_MUL        5



#define BLDC_COMMAND_BUFFER_LEN			(DSP_CMD_DATA_SIZE + DSP_CMD_RAM_READ_SIZE + DSP_CMD_RAM_WRITE_SIZE)
#define BLDC_INFO_BUFFER_LEN			 (DSP_CONSTANT_DATA_LEN + DSP_RAM_DATA_LEN)/* su arcadia c'? un +2*/
#define BLDC_INFO_INTERPRETER_BUFFER_LEN	10

/* formattazione info buffer dsp data (da DEA 601) */

#define BLDC_FAULT_MSB		0
#define BLDC_FAULT_LSB		1

#define BLDC_VCURR_MSB		2
#define BLDC_VCURR_LSB		3

#define BLDC_VS_STATE_MSB 	4/* nel protocollo vecchio qui c'? la versione Fw*/
#define BLDC_VS_STATE_LSB 	5/* nel protocollo vecchio qui c'? la versione Fw*/
/* nel protocollo vecchio c'? il vecchio dsp, quindi non c'? il valore del sensore virtuale*/
#define BLDC_OH_INDEX_MSB   6
#define BLDC_OH_INDEX_LSB   7

#define BLDC_TORQUE_MSB 	8
#define BLDC_TORQUE_LSB 	9

#define BLDC_FLAGS_MSB    10
#define BLDC_FLAGS_LSB    11


#endif /*CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/

#define SET_PUMP(index, state )  {\
  LoadsReq_New[Pump_Pos[index]].b=(state);\
}



#define LOAD_ON					1
#define LOAD_OFF				0

#define RES_ON  1
#define RES_OFF 0

#define EV_ON   1
#define EV_OFF  0

#define FAN_SPEED_OFF	 			0x00

/**Generic Input States */
#define DOOR_CLOSE_VAL				0x01
#define DOOR_OPEN_VAL				0x00
/* stato porta per uP */
#define UP_DOOR_WAIT          0
#define UP_DOOR_CLOSE         1

#define GI_DOOR_CLOSE				LROT_B(DOOR_CLOSE_VAL,GI_CR_DOOR_IND)
#define GI_DOOR_OPEN				LROT_B(DOOR_OPEN_VAL,GI_CR_DOOR_IND)


/** Bitmap GIValid */
#define GI_FIRST_CRITERION_BIT		0 /*ai criteri sono associati gli indici più bassi */
#define GI_FIRST_PROBE_BIT			(GI_FIRST_CRITERION_BIT+GI_CRITERION_NUM)			/* 6 */
#define GI_FIRST_PRESS_SENSOR_BIT	(GI_FIRST_PROBE_BIT+GI_PROBE_NUM)					/* 7 */
#define GI_FIRST_PULSE_COUNTER_BIT	(GI_FIRST_PRESS_SENSOR_BIT+GI_PRESS_SENSOR_NUM)		/* 8 */
#define GI_FIRST_TURB_SENSOR_BIT	(GI_FIRST_PULSE_COUNTER_BIT+GI_PULSE_COUNTER_NUM)	/* 9 */
#define GI_FIRST_VIRTSENSOR_BIT		(GI_FIRST_TURB_SENSOR_BIT+GI_TURB_SENSOR_NUM)		/* 10 -11*/
#define GI_PRESS_SENSOR_LEVEL_BIT	(GI_FIRST_PRESS_SENSOR_BIT+GI_PRESS_SENSOR_LEVEL_IND)/* 7 Come GI_FIRST_PRESS_SENSOR_BIT */

/** Masks */
#define GI_PRESS_SENSOR_LEVEL_MSK	MASK_W(GI_PRESS_SENSOR_LEVEL_BIT)
#define GI_TURB_SENSOR_MSK			MASK_W(GI_FIRST_TURB_SENSOR_BIT)

#define GI_PROBE_TYPE			0x00

enum{
	GI_WASH_PROBE_TEMP_IND =		0x00,
	GI_PROBE_NUM
};

/** Press Sensor */
#define GI_PRESS_SENSOR_TYPE	0x01
enum{
	GI_PRESS_SENSOR_LEVEL_IND =	0x00,
	GI_PRESS_SENSOR_NUM
};

/** Criteria */
#define GI_CRITERION_TYPE		0x02
enum{

	GI_PRESS_SENS_CR_FULL_IND = 0x00,
	GI_CR_OVFL_IND,				/*0x01*/
	GI_CR_DOOR_IND,				/*0x02*/
	GI_SALT_IND,				/*0x03*/
	GI_SPARKLING_AID_IND,		/*0x04*/
	GI_FIRST_WAKEUP_SOURCE_IND,	/*0x05*/
	GI_CRITERION_NUM
};

/** PulseCounter */
#define GI_PULSE_COUNTER_TYPE	0x03
enum{
    GI_PULSE_COUNTER_IND =		0x00,
    GI_PULSE_COUNTER_NUM
};

#define GI_TURB_SENSOR_TYPE		0x04
enum{
  GI_TURB_SENSOR_IND =			0x00,
  GI_TURB_SENSOR_NUM
};

#define GI_VIRTUAL_SENSOR_TYPE		0x05
enum{
	GI_VIRTUAL_SENSOR_WASH =		0x00,
	GI_VIRTUAL_SENSOR_DRAIN,
	GI_VIRTUAL_SENSOR_NUM
};

#define EMPTY_OK					(b0)
#define FULL_OK						(b1)
#define OVERFLOW_OK					(b2|b1)

#define CRITERION_OFF				0
#define CRITERION_ON				1
#define CRITERION_UNDEF				2

#define GIVALUE_UNDEF_MIN			0xFFFE		    /* valore non ammesso (MIN) per GI (pressostato/sonda/pulsecounter/turbidity) */
#define GIVALUE_UNDEF_MAX			0xFFFF		    /* valore non ammesso (MAX) per GI (pressostato/sonda/pulsecounter/turbidity) */

#define PROBE_UNDEF_CC				0xFF		      /* valore non ammesso per la sonda CC */
#define PROBE_UNDEF_CA				0xFE		      /* valore non ammesso per la sonda CA */

#define PRESS_SENSOR_UNDEF_MIN		0xFFFE		/* valore non ammesso per pressostato lineare (MIN) */
#define PRESS_SENSOR_UNDEF_MAX		0xFFFF		/* valore non ammesso per pressostato lineare (MAX) */

#define TURBIDITY_UNDEF_MIN			0xFF		      /* valore non ammesso per il sensore di torbidità (MIN) */
#define TURBIDITY_UNDEF_MAX			0xFE		      /* valore non ammesso per il sensore di torbidità (MAX) */




#define NO_LD_POS					    0xFF

#define ID_IND_MASK					  0x0F

#define MAKE_IO_ID(type,index)		            ((uint8_t)(((type) << 4)|((index) & ID_IND_MASK)))

/** Loads Types & indexes */

#define LD_PUMP_TYPE				    0x00
enum 	{
  LD_WASH_PUMP_IND=0,               /* 0x00, Pompa lavaggio BLDC sinc */
  LD_DRAIN_PUMP_IND,                /*0x01, Pompa scarico */
  N_MAX_LD_PUMP
};


#define LD_RES_TYPE					    0x01
enum{
	LD_WASH_RES_IND=0x00,           /*0x00, Resistenza lavaggio */
	LD_DRY_RES_IND= 0x01, 			/*0x01, Resistenza asciugatura */
	LD_RESIN_RES_IND= 0x02,
  N_MAX_LD_RES
};

#define LD_EV_TYPE					    0x02
enum	{
   LD_WASH_EV_IND=0,                /*0x00  Elettrovalvola lavaggio */
   LD_RIG_EV_IND,                   /*0x01  Elettrovalvola rigenera */
   LD_DISP_EV_IND,                   /*0x02  Elettrovalvola Dispenser */
   LD_7LITER_EV_IND,                /*0x03  Elettrovalvola Dispenser */
   N_MAX_LD_EV
};

#define LD_DISPENSER_TYPE			  0x03
enum{
	LD_DISPENSER_IND=0x00,          /*0x00, Dispenser */
	N_MAX_LD_DISPENSER
};

#define LD_ALT_ENGINE_TYPE		  0x04
enum{
  LD_ALT_ENGINE_IND=0x00,           /*0x00, Alternato */
  N_MAX_LD_ALT_ENGINE
};


#define LD_REGULATION_DEV_TYPE	0x05
enum{
  	LD_PUMP_ACTIVATION_REG_IND=0,     /*0x00 Pump activation */
	LD_STANDBY_REG_IND,	            /*0x01 standby */
	LD_MASTER_RELAY_REG_IND,	    /*0x02 Master rele */
	LD_SALT_ACTIVATION_REG_IND,		/*0x03 pilotaggio sensore sale */
	LD_BULK_ACT_UPSIDE_REG_IND,		/*0x04 pilotaggio Bulk del DPS lato microcontrollore ( sopra) */
	LD_BULK_ACT_DSPSIDE_REG_IND,	/*0x05 pilotaggio Bulk del DPS lato DSP (sotto)*/
	LD_KILL_DSP_REG_IND,			/*0x06 pilotaggio Kill  del DPS per togliere alimentazione */
	N_MAX_LD_REG_DEV
};

#define LD_FAN_TYPE					    0x06
enum 	{
  LD_DRY_FAN_IND=0,               /*0x00, Ventola asciugatura */
  LD_RESIN_FAN_IND,
  N_MAX_LD_FAN
};

#define LD_LAMP_TYPE					0x07
enum 	{
  LD_LAMP_INSIDE_IND=0,               /*0x00,LAmpada interna */
  N_MAX_LD_LAMP
};

#define LD_OZONE_TYPE				    0x08
enum 	{
  LD_OZONATOR_IND=0,               /*0x00, ozonizzatore (digital) */
  LD_AEROSOL_IND,
  N_MAX_LD_OZONE
};

/* ID */
#define LD_DUMMY_TYPE				  0x0F


#define LD_WASH_PUMP          MAKE_IO_ID(LD_PUMP_TYPE,LD_WASH_PUMP_IND)
#define LD_DRAIN_PUMP         MAKE_IO_ID(LD_PUMP_TYPE,LD_DRAIN_PUMP_IND)

#define LD_WASH_RES           MAKE_IO_ID(LD_RES_TYPE,LD_WASH_RES_IND)

#define LD_WASH_EV            MAKE_IO_ID(LD_EV_TYPE,LD_WASH_EV_IND)
#define LD_RIG_EV             MAKE_IO_ID(LD_EV_TYPE,LD_RIG_EV_IND)
#define LD_7LITER_EV          MAKE_IO_ID(LD_EV_TYPE,LD_7LITER_EV_IND)

#define LD_DISPENSER      		MAKE_IO_ID(LD_DISPENSER_TYPE,LD_DISPENSER_IND)

#define LD_ALT_ENGINE         MAKE_IO_ID(LD_ALT_ENGINE_TYPE,LD_ALT_ENGINE_IND)

#define LD_PUMPACTIVATION_REG	MAKE_IO_ID(LD_REGULATION_DEV_TYPE,LD_PUMP_ACTIVATION_REG_IND)
#define LD_STANDBY_REG        MAKE_IO_ID(LD_REGULATION_DEV_TYPE,LD_STANDBY_REG_IND)
#define LD_MASTER_RELAY_REG		MAKE_IO_ID(LD_REGULATION_DEV_TYPE,LD_MASTER_RELAY_REG_IND)


/** Pressostato */
#define LEV_TO_TIME_MIN		(OFFS_B(Tab_HWCPressSensorData.Data, 1)& 0x7F)		/* tempo in secondi per raggiungimento pieno pressostato (min) */
#define LEV_TO_TIME_MAX		(OFFS_B(Tab_HWCPressSensorData.Data, 2)& 0x7F)		/* tempo in secondi per raggiungimento pieno pressostato (max) */
#define LEV_TO_TIME_DEF		(OFFS_B(Tab_HWCPressSensorData.Data, 3)& 0x7F)		/* tempo in secondi per raggiungimento pieno pressostato (def) */
#define LEV_TO_TIME_HIST	(OFFS_B(Tab_HWCPressSensorData.Data, 4)& 0x7F)		/* tempo in secondi per raggiungimento pieno pressostato (def) */

#define FULL_REF_LEVEL		(OFFS_B(Tab_HWCPressSensorData.Data, 5))		/* quantità in dl/riferimento (se PL) corrispondente al pieno pressostato */
#define EMPTY_REF_LEVEL   0 //DEFINIRE DA SETTING
/**
CONSISTENCY - Coerenza stato carico - stato feedback
*/ 
/**/

/**
JUMPIF & CONDITIONS
*/


/** JUMPIF & CONDITIONS	 */
#define JIF_TAB_GI_PROBE_SEG		    0x00	/* 00-000 */
#define JIF_TAB_GI_PRESSSENSOR_SEG		0x01	/* 00-001 */
#define JIF_TAB_GI_CRITERION_SEG	  	0x02	/* 00-010 */
#define JIF_TAB_GI_FLOW_METER_SEG	  	0x03	/* 00-011 */
#define JIF_TAB_GI_TURBIDITY_SEG	  	0x04	/* 00-100 */

#define JIF_TAB_FUNCTIONS_SEG		    0x08	/* 01-000 */

#define JIF_TAB_BOOLEANS_SEG		    0x10	/* 10-000 */
#define JIF_TAB_FLAGS_SEG			    0x11	/* 10-001 */
#define JIF_TAB_LAST_FAULT			    0x12	/* 10-010 */
#define JIF_TAB_ALPHA_COEFFICIENTS		0x13	/* 10-011 */

#define JIF_TAB_WASH_VS	    		    0x18	/* 11-011 */
#define JIF_TAB_DRAIN_VS	    		0x19	/* 11-100 */

/**
STATISTICS
*/

#define N_MAX_STAT_CYCLES				32			/* numero max statistiche per cicli globali */
#define N_MAX_STAT_FUNCTIONS  	 		20

#define STAT_SAVED_CYCLES_TURB_READS_NUM    4		/* #letture torbidità salvateper ogni ciclo degli ultimi dieci cicli */

#define N_STAT_SAVED_CYCLES		  			6		/* Numero dei cicli pi� recenti i cui dati vengono memorizzati nelle statistiche */

#define STATISTIC_FLAG_DIRTY_UPDATE     	0 /* flag per determinare il numero di riduzioni di velocità */
#define STATISTIC_FLAG_NO_SALT   			1 /*sale assente*/

/* Massimo numero di Black Box salvate contemporaneamente */
#define BB_BLACK_BOX_MAX_NUM		2

#define FIXED_GI_TAB_LEN	(sizeof(TabHeader_t))   	/*TabHeader_Struct length (word)*/

/* Numero di regolazioni di una funzione On-Off */
#define ON_OFF_FUNC_REG_NUM			2
/* Valore dell'indice di regolazione di una funzione On-Off quando la funzione è On */
#define INDEX_ON_VALUE				2

/* Codici eventi salvataggio dati black box */
#define BB_EVENT_PREFAULT			0
#define BB_EVENT_FAULT				1

/* Numero di carichi il cui stato di attivazione è salvato nella Black Box */
#define BB_LOADS_NUM				16

#if (N_MAXLOADS >= BB_LOADS_NUM)
#define BB_VALID_LOADS_NUM			BB_LOADS_NUM
#else
#define BB_VALID_LOADS_NUM			N_MAXLOADS
#endif

#define BB_AN_NOT_READY_STAT_DEFAULT 0xFF

/* Indice del ciclo più recente */
#define MOST_RECENT_STAT_CYCLE_POS	0
/**
ALGORITHMS
*/
/* Calibrazione Sensore di torbidità */
#define RESET_TURBIDITY_CALIBRATION_PWD             0x00
#define ACTIVATE_TURBIDITY_CALIBRATION_PWD          0x01

/** Sensore di Torbidità: PWM */
#define USE_TURBIDITY_PWM_CUSTOM_VAL_PWD            0x01    /* valore custom pwm, lettura continua con effetto sui microprogrammi */
#define TURBIDITY_CONTINUOUS_READ_PWD               0x02    /* valore pwm normale, lettura continua senza effetto sui microprogrammi */

/* state */
#define TURBIDITY_PWM_DEFAULT_STATE                 0
#define TURBIDITY_PWM_INIT_CUSTOM_STATE             1
#define TURBIDITY_PWM_CUSTOM_STATE                  2
#define TURBIDITY_CONTINUOUS_READ_INIT_CUSTOM_STATE 3
#define TURBIDITY_CONTINUOUS_READ_CUSTOM_STATE      4

enum /*AAL_ALG*/
{
    AAL_TURBIDITY_SENSOR_CAL_ALG=0,
    AAL_LEV_TO_TIME_REF_UPDATE_ALG,
    AAL_DIRTY_FOAM_IDENTIFY_ALG,
	AAL_ALG3,
	AAL_READVIRTUALSENSOR_ALG,
    AAL_TURBIDITY_READ_CONTINUOUS,
    AAL_TURBIDITY_READ_ONESHOT,
    AAL_TURBIDITY_VALIDATE_CALIB,
};

#define TURBIDITY_CAL_ADVAL         OFFS_W(Tab_HWCTurbidity.Data,0)

#define TURBIDITY_H2O_MIN_ADVAL     OFFS_W(Tab_HWCTurbidity.Data,1)
#define TURBIDITY_H2O_MAX_ADVAL     OFFS_W(Tab_HWCTurbidity.Data,2)
#define TURBIDITY_AIR_MIN_ADVAL     OFFS_W(Tab_HWCTurbidity.Data,3)
#define TURBIDITY_AIR_MAX_ADVAL     OFFS_W(Tab_HWCTurbidity.Data,4)
#define TURBIDITY_PWM_MIN           OFFS_W(Tab_HWCTurbidity.Data,5)
#define TURBIDITY_H2O_DEFAULT_ADVAL OFFS_W(Tab_HWCTurbidity.Data,6)

enum TURBSENS_READERS_TABLE
{
    /* Algoritmi torbidità */
    TURBSENS_READ_al_TRB_OS=0,          /* Algoritmo torbidità one shot */
    TURBSENS_READ_al_TRB_CN,            /* Algoritmo torbidità continuous */
    TURBSENS_READ_al_TRB_VL,            /* Algoritmo torbidità validation */

    /* microprogrammi */
    TURBSENS_READ_up_SPA,               /* microprogramma SparklingAid*/

    /* custom */
    TURBSENS_READ_cstm_PWM,             /* PWM custom*/
    TURBSENS_READ_cstm_CONTINUOUS,      /* continuous*/

    /* calibration */
    TURBSENS_READ_calib_FLB,			/* Turbidity Calibration */

};

enum CALIBRATION_STATE
{
  TURBSENS_VALID_CALIBR_VALUE_STATE=0,
  TURBSENS_CALIBR_REQUEST_STATE,
  TURBSENS_WAIT_4_CALIBR_VAL_STATE,
  TURBSENS_CALIBR_VALIDATE_STATE

};

/* Calibrazione Sensore di torbidità */
#define RESET_TURBIDITY_CALIBRATION_PWD             0x00
#define ACTIVATE_TURBIDITY_CALIBRATION_PWD          0x01

#define  MOD_LEV1_DT_VALUE          0

/**
USER INTERFACE
*/


/** ApplicationState */
/* Bit position in ApplicationState */
#define AS_DATA_READY_BIT			0	/* Se '1' dati su buffer di stato per UI validi */
#define AS_REMOTE_CTRL_ON_BIT		1	/* Se '1' PC Control attivo (indipendente da dataReady) */
#define AS_REGS_WRITTEN_BIT			2	/* A '0' dopo un reset freddo, va a '1' dopo una WRITE_REGS */
#define AS_COLLAUDO_ON_BIT			3 /* AGGIORNARE CON GESTIONE COLLAUDO - SAT !!!!!! */
#define AS_AUTOTEST_ON_BIT			4 /* AGGIORNARE CON GESTIONE COLLAUDO - SAT !!! */
#define AS_SPECIAL_MODE_ON_BIT		5	/* Se '1' applicazione (UI?) in modalità di funzionamento SPECIAL MODE. */
#define AS_APPL_FAULT_BIT			6	/* Se '1' segnala applicazione in fault */
#define AS_BACKUP_ALM_OFF_BIT		7	/* Se '0' notifica pre-backup alarm (O.C. flag). Va a '1' dopo la procedura READ_STATUS -> READ_REGS */

#define NUM_REG_MAX				40

#define NUM_UI_INFO_MAX 		3   /* numero byte buffer BufUIInfo. To be defined */


/**
UI REGULATION MAIN PARAM TABLES
*/
#define DATA_FORMAT_OFFSET		0
#define DATA_FORMAT_MASK		(b7|b6)
#define STEP_OFFSET				1
/** Regolazione a step */
#define CHAR_INIT_VAL_OFFSET	3
#define WORD_INIT_VAL_OFFSET	2
/** Regolazione a tabella */
#define INIT_VAL_OFFSET			2
/**/


/**
FUNCTION HEADING
*/
#define FUNC_ID_GENERIC     0
#define FUNC_ID_SELECTOR    1 /* Selettore Cicli */
#define FUNC_ID_START			  2 /* Start Command */

	/* define on START Value */
	#define REG_DONT_CARE			0
	#define REG_START_CONDITION		1
	#define REG_RESET_CONDITION		2
	#define REG_PAUSE_CONDITION		3
	#define REG_RESET_FAULT		  	4
	#define REG_RESTART_CONDITION	5
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
	#define REG_OVERLOAD_PAUSE_CONDITION 		6
	#define REG_ENERGY_PHASES_PAUSE_CONDITION	7
#endif
#define	FUNC_ID_TEMPERATURE		  3 	/* Temperatura */
#define FUNC_ID_CLOCK_HH		  4 	/* Clock Ore   */
#define FUNC_ID_CLOCK_MM		  5 	/* Clock Minuti  */
#define FUNC_ID_DURATION_HH 	  6 	/* Durata Ore */
#define FUNC_ID_DURATION_MM 	  7 	/* Durata Minuti */
#define FUNC_ID_END_PROGRAM_HH	  8 	/* Fine Ciclo Ore */
#define FUNC_ID_END_PROGRAM_MM	  9 	/* Fine Ciclo Minuti */
#define FUNC_ID_COUNT_DOWN_HH	  10 	/* CountDown Ore */
#define FUNC_ID_COUNT_DOWN_MM	  11 	/* CountDown Minuti */
#define FUNC_ID_MODE			  13	/* Mode - UI */
#define FUNC_ID_SELECT			  14	/* Select - UI */
#define FUNC_IDPOWER			  15  	/* Grill - Oven */
#define FUNC_ID_CHILDLOCK		  16	/* Blocco porta */
#define FUNC_ID_SWITCH_ONOFF	  17	/* On-Off - UI */
#define FUNC_ID_ESCAPE			    18	/* Esci - UI */


#define FUNC_ID_DELAY_HH		    19	/* Delay Timer Ore */

#define FUNC_ID_DELAY_MM		    20	/* Delay Timer Minuti */
#define FUNC_ID_SETUP			      21	/* Setup - UI */
#define FUNC_ID_CONFIRM			    22	/* OK - UI */
#define FUNC_ID_DEMO			      23	/* Demo Mode */
#define FUNC_ID_DOOR_LOCK		    24	/* Blocco Porta */
#define FUNC_ID_LAMP			      25	/* Lampada */

#define FUNC_ID_3IN1			      26	/* Pastiglie - DW */
#define FUNC_ID_EXTRADRY		    27	/* Extradry  - DW */
#define FUNC_ID_BASKET			    28	/* Cesti - DW */
#define FUNC_ID_MODE2			      29	/* Mode 2 - UI */
#define FUNC_ID_LANGUAGE		    30	/* Lingua */
#define FUNC_ID_SOUND			      31	/* Tono - UI */
#define FUNC_ID_CLOCKENABLE		  32
#define FUNC_ID_PYROLISESAUTO	  33
#define FUNC_ID_REMOTECONTROL	  34
#define FUNC_ID_PHASE_ICON		  35
#define FUNC_ID_PHASE_STRING	  36
#define FUNC_ID_DURATION_MM_ELP	  37
#define FUNC_ID_DURATION_HH_ELP   38
#define FUNC_ID_RINSE_AID	      56

#define FUNC_ID_WATER_HARDNESS  57  /* Durezza Acqua - DW*/
#define FUNC_ID_NIGHT_OPTION    58  /* Night Option - DW*/
#define FUNC_ID_TIME_SHORTENING 59  /* Time Shortening - DW */
#define FUNC_ID_EXTENSION  		60
#define FUNC_ID_ZONE_WASH  		61 /* wash zone */
#define FUNC_ID_OZONATOR 		62 /* ozono*/

#define FUNC_ID_ENERGY_REMOTE_CONTROL      111


#define FUNCTION_ID_OFFSET		0
#define FUNCTION_ID_MASK		(b6|b5|b4|b3|b2|b1|b0)
#define STEP_REG_MASK			(b5)
#define NUM_REG_OFFSET			2
#define MAIN_REG_TAB_OFFSET		4


/** Valore di default per la selezione cesto nel caso non sia possibile ricavarlo dalla regolazione */
#define DISH_RACK_DFLT_VALUE	      0 /* entrambi i cesti */
#define JOIN_LEAVE_REG_DFLT_VALUE 	  0

/** Valore di default per la quantità di acqua necessaria alla rigenera
 nel caso non sia possibile ricavarlo dalla regolazione durezza acqua */
#define REGENERATE_WATER_DFLT_VALUE 65	 /* 65 litri */

/** Valore di default per le zone wash nel caso non sia possibile ricavarlo dalla regolazione  */

#define ZONE_WASH_DFLT_VALUE 0	 /*  */
#define RINSE_AID_DFLT_VALUE 1	/* almeno un impulso*/

/**
PROGRAM HEADING
*/
/** Numero di cicli selettore */
#define CYCLES_NUM				(*(Tab_HWCCyclesNum.Data))
#define PROGRAM_HEADING_ITEM_SIZE	2

/** #define on sublabel */
#define SUBL_GENERIC_CYCLE		0
#define SUBL_RESERVED_1			1
#define SUBL_AUTOTEST			2
#define SUBL_COLLAUDO			3
#define SUBL_DEMO				4
#define SUBL_RESERVED_2			5
#define SUBL_RESERVED_3			6
#define SUBL_RESERVED_4			7
#define SUBL_ANTISHOCK			8
#define SUBL_ANTIFOAM			9

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#define PP_INDEX_OFFSET			7 /*Cycle index APP2.0 */
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */


/** #define on complete label */
#define CMPLT_LBL_PROGRAMMING	((uint8_t)((SUBL_GENERIC_CYCLE << 4) | LBL_PROGRAMMING))
#define CMPLT_LBL_DELAY			((uint8_t)((SUBL_GENERIC_CYCLE << 4) | LBL_DELAY))
#define CMPLT_LBL_END			((uint8_t)((SUBL_GENERIC_CYCLE << 4) | LBL_END))
#define CMPLT_LBL_PAUSE			((uint8_t)((SUBL_GENERIC_CYCLE << 4) | LBL_PAUSE))
#define CMPLT_LBL_FAULT			((uint8_t)((SUBL_GENERIC_CYCLE << 4) | LBL_FAULT))
#define CMPLT_LBL_AUTOTEST		((uint8_t)((SUBL_AUTOTEST << 4) | LBL_RUN))
#define CMPLT_LBL_COLLAUDO		((uint8_t)((SUBL_COLLAUDO << 4) | LBL_RUN))
#define CMPLT_LBL_ANTIFOAM		((uint8_t)((SUBL_ANTIFOAM << 4) | LBL_RUN))
#define CMPLT_LBL_ANTISHOCK		((uint8_t)((SUBL_ANTISHOCK << 4) | LBL_RUN))
#define CMPLT_LBL_DEMO			((uint8_t)((SUBL_DEMO << 4) | LBL_RUN))

/** Data Restore From Fault */
#define CYCLE_RF_OFFSET			0
#define CYCLE_RF_MASK			(b3|b2)

#define NO_RESTORE_FROM_FAULT					0
#define RESERVED_VALUE_RESTORE_FROM_FAULT		1	/* In oven CHECK_TEMP_FOR_RESTORE_FROM_FAULT */
#define RESTORE_FROM_FAULT_IN_ANY_CASE			2

#define SHIFTED_NO_RESTORE_FROM_FAULT			(NO_RESTORE_FROM_FAULT << 2)
#define SHIFTED_RESTORE_FROM_FAULT_IN_ANY_CASE	(RESTORE_FROM_FAULT_IN_ANY_CASE << 2)

/** Data Restore From Power Fail */
#define CYCLE_BC_OFFSET			0
#define CYCLE_BC_MASK			(b1|b0)

#define NEVER_RESTORE_FROM_POWER_FAIL			0
#define RESERVED_VALUE_RESTORE_FROM_POWER_FAIL	1	/* In oven CHECK_TEMP_FOR_RESTORE_FROM_POWER_FAIL */
#define RESTORE_FROM_POWER_FAIL_IN_ANY_CASE		2


/** Dimensione dell'intestazione di un ciclo in WORD */
#define PROGRAM_HEADING_ITEM_SIZE	2
#define PROGRAM_HEADING_ITEM_SIZE_WITH_PP 3
/** Cycle complete label */
#define CYCLE_LABEL_OFFSET		1
#define CYCLE_LABEL_MASK		(b3|b2|b1|b0)
#define CYCLE_SUBLABEL_OFFSET	1
#define CYCLE_SUBLABEL_MASK		(b7|b6|b5|b4)
/**
LOOKUP TABLE
*/ 
/* Tenere sempre aggiornato */
#define N_LOOKUP_TABLE_ELEMENTS   73
/**/

#ifndef DIM_MACHINE_STATE_BUF
#define DIM_MACHINE_STATE_BUF	(11)
#endif

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#define NUM_MACHINE_STATE_VARS	9	/* Dimensione dei buffer TabMachineStateAddr[] e TabMachineStateDim */
#else
#define NUM_MACHINE_STATE_VARS	5	/* Dimensione dei buffer TabMachineStateAddr[] e TabMachineStateDim */
#endif


#define POS_APPL_STATE		0		/* Posizione del byte ApplicationState in MachineStateBuf[]*/
#define PRE_BACKUP_BIT_MASK	0x80	/* Maschera per il bit di pre-backup in MachineStateBuf[POS_APPL_STATE]*/
/**
PUNTATORI SETTING FILE
*/
#define PTR_SERIAL_NUMBER_DATA				APP_PTR_SERIAL_NUMBER_DATA	/**< 0x0000. Dati identificativi dell'elettrodomestico */
#define PTR_MAIN_EXT_PROCESSOR				APP_PTR_MAIN_EXT_PROCESSOR	/**< 0x0001. Parametri schede esterne */
#define PTR_EXPANSION_BOARD					APP_PTR_EXPANSION_BOARD		/**< 0x0002. Dati schede di espansione */
#define PTR_EXPANSION_FACTORY_TEST_DATA		APP_PTR_EXPANSION_FACTORY_TEST_DATA	/**< 0x0003. Dati di calibrazione schede di espansione */
#define PTR_EXPANSION_BOARD_SPEC_DATA		APP_PTR_EXPANSION_BOARD_SPEC_DATA	/**< 0x0004. Parametri specifici schede di espansione */
#define PTR_RESERVED_05						((uint16_t)0x0005)	/**< ********* Non utilizzato (ex Acquisition Data) ********* */
#define	PTR_FACTORY_TEST_DATA				APP_PTR_FACTORY_TEST_DATA /**< 0xFFFE. Dati White Box/esiti collaudi */
#define PTR_MAP_LENGTH						APP_PTR_MAP_LENGTH			/**< 0x0007. Prima locazione successiva al file di setting (== lunghezza file di setting) */
#define PTR_RESERVED_08						((uint16_t)0x0008)	/**< ********* Non utilizzato ( ex Backup Area ) ********* */
#define PTR_CHECKSUM_POSITION				APP_PTR_CHECKSUM_POSITION	/**< 0x0009. Checksum file di setting */
#define PTR_HW_CONFIG_DATA					APP_PTR_HW_CONFIG_DATA	/**< 0x000A. Hardware Configuration */

	/* Displacements */
	#define DSPL_HWC_PRESS_SENSOR_DATA		0x00	/* Pressure Sensor Params */
	#define DSPL_HWC_REGENERATE_DATA		0x01
	#define DSPL_HWC_TURBIDITY_DATA			0x02
	#define DSPL_HWC_FLAGS_DATA				0x03
	#define DSPL_HWC_CYCLES_NUM				0x04
	#define DSPL_HWC_FUNC_NUM				0x05
	#define DSPL_HWC_MODIFIERS_VAR_TABLE	0x06
	#define DSPL_HWC_SALT_DATA				0x07
	#define DSPL_HWC_DRYER_DATA				0x08
	#define DSPL_HWC_VS_DATA				0x09
	#define DSPL_HWC_POWER_PROFILE_VARIABLE_TABLE	0x0A
#define PTR_OTHER_ALGORITHMS_TABLE_DATA		((uint16_t)0x002D)
	#define DSPL_ALPHA0_VOLTAGE_PARAMETERS	0x00
	#define DSPL_ALPHA1_TEMPERA_PARAMETERS	0x01

#define PTR_RESERVED_0B						((uint16_t) 0x000B )	/**< ********* Non utilizzato (solo per ARCADIA duplicato il Selector Data) ********* */
#define PTR_HI_STATEMENTS					APP_PTR_HI_STATEMENTS	/**< 0x000C. High Statement microprogrammi */
#define PTR_LO_STATEMENTS					APP_PTR_LO_STATEMENTS	/**< 0x000D. Low Statement microprogrammi */
#define PTR_MOVEMENT_DATA					((uint16_t) 0x000E )
#define PTR_SPIN_DATA						((uint16_t) 0x000F )
#define PTR_UP_SETSTATE						APP_PTR_MP_SETSTATE_DATA	/**< 0x0010. Microprogramma SetState */
#define PTR_UP_LOAD 						0x0011
#define PTR_UP_LOAD_RESTORE 				0x0012
#define PTR_UP_HEAT							0x0013
#define PTR_UP_PUMPCONTROL					0x0014
#define PTR_UP_ALTERNATECONTROL				0x0015
#define PTR_UP_LOADSCONTROL					0x0016
#define PTR_UP_SPARKLING_AID				0x0017
#define PTR_UP_DRAIN      					0x0018
#define PTR_UP_MAINTAIN						0x0019
#define PTR_MP_ACTIVATE_ALGORITHM			APP_PTR_MP_ACTIVATEALGO_DATA	/**< 0x0018. Microprogramma Activate Algorithm */

#define PTR_MP_JUMPIF_DATA					APP_PTR_MP_JUMPIF_DATA	/**< Microprogramma Jump If */
#define PTR_UP_SETALARM						0x001C
#define PTR_UP_REGENERATE					0x001D
#define PTR_UP_SET_PROFILE					0x001E
#define PTR_UP_7LITER					    0x001F
#define PTR_IO_CONFIG_DATA		 			((uint16_t) SF_PTR_IO_CONFIG ) /**< 0x0020. Mappa Input/Output */

	#define DSPL_IO_LOW_LEVEL_INPUT_MAP		((uint8_t) 0x00 )
	#define DSPL_IO_GENERIC_INPUT_MAP		((uint8_t) 0x01 )
	#define DSPL_IO_UII_MAP					((uint8_t) 0x02 )
	#define DSPL_IO_OUTPUT_MAP				((uint8_t) 0x03 )
	#define DSPL_IO_MODULE_PIN_MAP			((uint8_t) 0x04 )

#define PTR_IO_DEVICE_DATA					SF_PTR_IO_DEVICE		/**< 0x0021. Parametri di configurazione ingressi e uscite */
#define PTR_LOADS_PROFILE_DATA				SF_PTR_LOADS_PROFILE	/**< 0x0022. Profili di pilotaggio carichi Profiler e PLP */
#define PTR_UI_MAIN_REG_PARAM_TABLE			APP_PTR_UI_MAIN_REG_PARAM_TABLE	/**< 0x0023. Tabelle di regolazione main per le funzioni UI */
#define PTR_ENGINE_PARAM_DATA				((uint16_t) 0x0024 )

#define PTR_COND_TABLE_DATA					APP_PTR_COND_TABLE_DATA	/**< 0x0026. Tabelle delle condizioni */
#define PTR_FAULT_DATA						APP_PTR_FAULT_DATA	/**< 0x0027. Parametri relativi ai fault */
#define PTR_FAULT_DATA_SPECIAL				0x0028
	/* Displacements per Fault Data e Fault Data Special*/
	#define DSPL_FLT_DRAIN					0x00	/* DrainParams */
	#define DSPL_FLT_LOAD					0x01	/* Load Fault Params*/
	#define DSPL_FLT_HEAT					0x02	/* Heat Fault Params*/
	#define DSPL_FLT_EXT_DEV				0x03	/* External Devices Fault Params */
	#define DSPL_FLT_MAIN_BOARD				0x04	/* Main Board Fault Params */
	#define DSPL_FLT_PRIORITIES				0x05	/* Priorità fault solo per Fault data  */
	#define DSPL_FLT_EXT_DEV_DRAINPUMP		0x06	/* External Devices Fault Params pompa drain */
	#define DSPL_FLT_VS                     0x07    /* Virtual Sensor Fault PArams */

	#define DSPL_FLT_SPEC_DEV_DRAINPUMP		0x05	/* External Devices Fault Params pompa drain */
	#define DSPL_FLT_VS_SPEC                0x06    /* Virtual Sensor Fault PArams */


#define PTR_MICROPROG_MODIFIERS_DATA		APP_PTR_MICROPROG_MODIFIERS_DATA	/**< 0x0029. Tabelle modificatori dati microprogrammi */
#define PTR_SAFETY_PARAM_DATA				APP_PTR_SAFETY_PARAM_DATA	/**< 0x002A. Parametri sicurezze e allarmi */

/* Displacements */
	#define DSPL_SFT_DOOR_LOAD_ON			0x00	/* Safety Load On */
	#define DSPL_SFT_PRESS_RES      		0x01	/* Safety Res/ Press */
	#define DSPL_SFT_STANDBY				0x02	/* Safety Standby */
	#define DSPL_SFT_EV						0x03	/* Safety EV */
	#define DSPL_SFT_MR						0x04	/* Safety Master Relè */
	#define DSPL_SFT_DRY					0x05	/* Safety Dry Params*/
	#define DSPL_SFT_MOTOR_WASH				0x06	/* Safety Motor wash Params*/
	#define DSPL_SFT_BULK_DSP				0x07	/* Safety Bulk-DSP Params*/
	#define DSPL_SFT_RESISTORWASH_PUMP_		0x08	/* Safety Resistor - Wash Pump Params*/
	#define DSPL_SFT_MOTOR_PARAMS_DRAINPUMP_		0x09	/* Safety Resistor - DRAIN Pump Params*/
	#define DSPL_SFT_MOTOR_PARAMS_7LITER_TANK_		0x0A	/* Massimo numero di giorni per acqua puzzolente in tasca 7 litri */

#define PTR_ALGORITHM_DATA					APP_PTR_ALGORITHM_PARAM_DATA	/**< 0x002B. Parametri algoritmi */

	#define DIRTY_FOAM_DISPL				((uint8_t) 0x00 )

#define PTR_ALGORITHM_READ_VS_DATA			0x002C	/**< Parametri algoritmi lettura sensori virtuali */
#define PTR_EXECUTE_ALGORITHM_DATA			APP_PTR_ALGORITHM_INDEX_DATA	/**< 0x002E. Indice algoritmi */
#define PTR_ALGORITHM_CODE					APP_PTR_ALGORITHM_EXPR_DATA		/**< 0x002F. Espressione algoritmi */

#define PTR_SELECTOR_DATA					APP_PTR_SELECTOR_TARGET_0	/**< 0x0030. Dati composizione selettore */


#define PTR_ALGORITHM_PARAMETERS			APP_PTR_ALGORITHM_DATA	/**< 0x0038. Dati algoritmi */
#define PTR_STATISTICS_DATA					APP_PTR_STATISTICS_DATA	/**< 0x0039. Parametri statistiche */

#ifdef _STATISTICS_ON_
	#define DSPL_FUNCTIONS_STATISTICS_DATA	((uint8_t) 0x00 )
#endif

#define PTR_FUNCTION_HEADING				APP_PTR_FUNCTION_HEADING	/**< 0x0052. Intestazione funzioni User Interface */
#define PTR_PROGRAM_HEADING					APP_PTR_PROGRAM_HEADING_TARGET_0	/**< 0x0054. Intestazione cicli */
#define PTR_FUNC_REG_MODIFIER				APP_PTR_FUNC_REG_MODIFIER	/**< 0x0057. Tabelle di regolazione alternative */

/**
SCHEDULE MODE
*/
#define DISABLE_ERC       1
#define JOIN_ERC          2
#define LEAVE_ERC         3
#define RE_JOIN_ERC       4
#define OFF_ERC           5
#define ON_ERC_SMART      6
#define ON_ERC_ECO		  7

/**
 * APP
 */
#if ( PP_ENERGYPAUSE_STATUS == TRUE )
#define PP_ENERGYPAUSE_STATUS_NO_PAUSE         0x00
#define PP_ENERGYPAUSE_STATUS_PAUSE_EN_PHASES  0x01
#define PP_ENERGYPAUSE_STATUS_PAUSE_OVERLOAD   0x02
#endif /* ( PP_ENERGYPAUSE_STATUS == TRUE ) */

/**
Regulation filter time window
*/
#define REGS_FILTER_INIT  20 /* x 100ms */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */
/* **************** */
/* Global variables */
/* **************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __CSTM_DEFINES_H */

/* *********** */
/* End of file */
/* *********** */
