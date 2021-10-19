/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file       PM_UM_UniversalMotor.h
\ingroup    PM_MotorGate
\author     Stefano Ughi, Roberto Fioravanti, Marco Sperandini
*/

/**
\addtogroup PM_MotorGate
*@{*/

#ifndef __PM_UM_UNIVERSAL_MOTOR_H
#define __PM_UM_UNIVERSAL_MOTOR_H

/* ******** */
/* Includes */
/* ******** */
#include "PM_UM_UniversalMotorExp.h"
#include "OutputManagerExp.h"
#include "PM_UM_Control.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** tick in us of the params */
#define NOMINAL_TICK                	(1.6)

#define CONV_TACHO_TICK(x)		((x)/2) /**< converte i tick della tachimetrica al tick di riferimento con cui sono stati calcolati i parametri
					attualmente il riferimento  e' 1 tick = 1.6 us (freescale gb60) , un domani il riferimento sara' 1 tick = 1us
					*/
#define CONV_PHASE_TICK(x)		((x)/4) /**< converte i tick dell'angolo di fase al tick di riferimento con cui sono stati calcolati i parametri
					attualmente il riferimento  e' 1 tick = 1.6 us (freescale gb60) , un domani il riferimento sara' 1 tick = 1us
					*/
#define UNCONV_PHASE_TICK(x)	(4*(x)) /**< converte i tick di riferimento con cui sono stati calcolati i parametri ai  tick del timer dell'angolo di fase al
					attualmente il riferimento  e' 1 tick = 1.6 us (freescale gb60) , un domani il riferimento sara' 1 tick = 1us
					*/					
#define NOMINAL_TO_STD(x)		((uint32_t)(NOMINAL_TICK*10)*(x)/10) /**< converte i tick di riferimento con cui sono stati calcolati i parametri ai  tick del timer dell'angolo di fase al					*/
#define STD_TO_NOMINAL(x)       (10*(x)/(uint32_t)(NOMINAL_TICK*10))
					
#define K_CONV_VEL				(uint32_t)1200000000
    
enum movimentazioni
{
    MOVIM_PAUSA__STOP=0,
    MOVIM_ON,								/*1*/
    MOVIM_SOFT_START,						/*2*/
    MOVIM_ON_WITHOUT_TACHO,					/*3*/
    MOVIM_PAUSA__AVVIA_SPEGNIMENTO,			/*4*/
    MOVIM_PAUSA__AVVIA_SPEGNIMENTO_WAIT,	/*5*/
    MOVIM_PAUSA__ATTENDI_RICHIESTA,			/*6*/
    MOVIM_PAUSA__ATTENDI_RICHIESTA_WAIT,	/*7*/
    MOVIM_SPEGNI							/*8*/
};

enum relaysStateDef
{
    ALL_OPEN = 0,
    CLOCKWISE,
    ANTICLOCKWISE,
    ALL_CLOSED
};

enum stateHFrelayDef
{
    RELAY_HF_OFF_TRIAC_ON = 0,
    RELAY_HF_OFF_TRIAC_OFF,
    RELAY_HF_ON_TRIAC_OFF,
    RELAY_HF_ON_TRIAC_ON
};

/* macchina a stati misura sbilanciamento*/
enum
{
    UNBALANCE_IDLE=0,
    UNBALANCE_START,
    UNBALANCE_UPDT_DELTA,
    UNBALANCE_UPDT_ACCU
};

#if defined (CONFIG_PARAM_FROM_FIRMWARE)

#define GET_DELAY_TRIAC_OFF_VS_HF_ON()		((uint8_t)1)		 /**< Param da Setting in unita' STATE_MACHINE_MOTOR_PERIOD [40 msec]
delay disattivazione Triac Motore vs attivazione Rel� Mezzo campo */

#define Get_common_rele_delay()                 ((uint8_t)10)		 /**< Param da Setting in unita' STATE_MACHINE_MOTOR_PERIOD [40 msec]*/

#define Get_common_rpm_min()                    (MIN_V_TGT)
#define Get_common_unb_avg_sampl_num()          (8)

#define Get_common_unb_discard_sampl()       	(4)

 
#define GET_DELTA_ALPHA_HF_ON()			(uint16_t)400		 /**< 640uSec/1.6uSec (1.6uSec tick timer) */

/** Param da Setting in unita' STATE_MACHINE_MOTOR_PERIOD [40 msec]
delay attivazione Rel� Mezzo campo vs attivazione Triac Motore */
#define GET_DELAY_TRIAC_ON_VS_REL_HF_ON()  	((uint8_t)1)
#define TACH_POLES_NUM      			16	 	 /**< Param da Setting */

#define RTx10               			118		 /**< Param da Setting */


#define MIN_V_TGT                               (uint16_t)16             /**< V target minima */

#else
/* ************************************* */
/* Defines per parametri da setting file */
/* ************************************* */

/* ** Indici tabella COMMON_PARAM_DISPL */      /* from 1.0.63 */
#define CMN_PAR_RPM_MIN                         0
#define CMN_PAR_ALPHA_STEP_SOFT_START           1
#define CMN_PAR_ALPHA_INI_SOFT_START            2

#define CMN_PAR_RELE_DELAY                      4
#define CMN_PAR_RTx10                           5
#define CMN_PAR_TACHO_POLES                     6 
#define CMN_PAR_TIME_MIN_FIRE_TRIAC        	    7
#define CMN_PAR_ALPHA_MAX                       8

#define CMN_PAR_REBALANCE_PD_PARAM_DISPL        10  /**< fornisce offset nel motor const data */
#define CMN_PAR_UNBALANCE_AVG_SAMPLES_DISPL     11
#define CMN_PAR_UNBALANCE_DISCARD_DISPL         12	 
#define CMN_PAR_PHASE_SAMPLES_DISPL				13

/** */
#define MID_FIELD_TBL_T_OFF__MF_ON_DELAY_DISPL    2
#define MID_FIELD_TBL_T_ON__MF_ON_DELAY_DISPL     3


/* ** Macro per tabella COMMMON PARAMS */
#define Get_common_rpm_min()                        (*(pcommmonparam+CMN_PAR_RPM_MIN))


#define Get_common_reb_pd_offset()                  (*((uint8_t*)pcommmonparam+CMN_PAR_REBALANCE_PD_PARAM_DISPL))
#define Get_common_unb_avg_sampl_num()              (*((uint8_t*)pcommmonparam+CMN_PAR_UNBALANCE_AVG_SAMPLES_DISPL))
#define Get_common_unb_discard_sampl()       	    (*((uint8_t*)pcommmonparam+CMN_PAR_UNBALANCE_DISCARD_DISPL))

#define GET_DELAY_TRIAC_OFF_VS_HF_ON()	            (*(pHalfFieldParam + MID_FIELD_TBL_T_OFF__MF_ON_DELAY_DISPL))
#define Get_common_rele_delay()                     (*(pcommmonparam+CMN_PAR_RELE_DELAY))
#define GET_DELTA_ALPHA_HF_ON()		            	(*(uint16_t*)pHalfFieldParam)
#define GET_DELAY_TRIAC_ON_VS_REL_HF_ON()           (*(pHalfFieldParam + MID_FIELD_TBL_T_ON__MF_ON_DELAY_DISPL))

#define Get_common_phase_samples(ptr)                (*((uint8_t*)(ptr)+CMN_PAR_PHASE_SAMPLES_DISPL))
#endif  /* CONFIG_PARAM_FROM_FIRMWARE */

#define ALPHA_INI_SOFT_START_50_HZ					(uint16_t) 4787 	 /* Param da Setting  */
#define ALPHA_MIN_SOFT_START_50_HZ					(uint16_t) 3191		 /* Param da Setting  */
#define ALPHA_STEP_SOFT_START_50_HZ					(uint8_t)  100		   /* Param da Setting  */
#define ALPHA_MAX_50_HZ								(uint16_t) 5125		 /* Param da Setting  */
#define ALPHA_MIN_50_HZ								(uint16_t) 94


#define ALPHA_INI_SOFT_START_60_HZ					(uint16_t) 4358 	 /* Param da Setting  */
#define ALPHA_MIN_SOFT_START_60_HZ					(uint16_t) 2394		 /* Param da Setting  */
#define ALPHA_STEP_SOFT_START_60_HZ					(uint8_t)  64		 /* Param da Setting  */
#define ALPHA_MAX_60_HZ								(uint16_t) 4438		 /* Param da Setting  */
#define ALPHA_MIN_60_HZ								(uint16_t) 60

/* Parametri Tachimetrica */
#ifdef CONFIG_PM_UM_NO_TACH_TIMEOUT
#define NO_TACH_TIMEOUT                         (CONFIG_PM_UM_NO_TACH_TIMEOUT)		
#else
#define NO_TACH_TIMEOUT                         (2)		
#endif

#ifdef CONFIG_PM_UM_NO_TACH_TIMEOUT_ENGINE_ON
	#define NO_TACH_TIMEOUT_ENGINE_ON	(CONFIG_PM_UM_NO_TACH_TIMEOUT_ENGINE_ON)   		
#else
	#define NO_TACH_TIMEOUT_ENGINE_ON	(10)
#endif   		

#define MAX_TACH_VALUE_TICK             (uint32_t) 60000
#define CONTINUE_MODE_READ_TACHO        (uint16_t) CONFIG_TACHO_ONE_SHOT_LIMIT   	

#define REL_MC_DISPL			((uint8_t)3)
#define REL_CLKWISE_DISPL       	((uint8_t)2)
#define REL_ACLKWISE_DISPL      	((uint8_t)1)

#define size(x)	(sizeof(x)/sizeof(x[0]))

#define HZ_50	1
#define HZ_60	0

#define EXT_HF_PWD  0x96


/**
codici di ritorno della funzione outControllore_t controllore()
*/
#define	CONTROLLORE_NO_ERROR					0
#define	CONTROLLORE_DIFFERENZA_ERROR    		1
#define	CONTROLLORE_ALPHA_TOO_LARGE_ERROR    	2


/**
codici di ritorno della funzione outFiltersAlpha_t filterNewAlpha( outControllore_t outControllore, filterParams_t filterParams )
*/
#define FILTER_NEW_ALPHA_ALPHA_OK				0
#define FILTER_NEW_ALPHA_ALPHA_SAME_PREVIOUS	1
#define FILTER_NEW_ALPHA_ALPHA_IN_RANGE			2
#define FILTER_NEW_ALPHA_ALPHA_OUT_DEV			3
#define FILTER_NEW_ALPHA_ALPHA_MAX_VALUE		4
#define FILTER_NEW_ALPHA_ALPHA_MIN_VALUE		5


/* stateMachine_s.flag defines */
#define STATE_MACHINE__FLAG__TACHO_OK         	(1<<0)
#define STATE_MACHINE__FLAG__CONTROLEN        	(1<<1)
#define STATE_MACHINE__FLAG__DRVEN            	(1<<2)

/* state Machine period x 10 ms */
#define STATE_MACHINE_MOTOR_PERIOD            	(uint8_t)4


#if !defined( CONFIG_PM_UM_UNBALANCE_DELTA_ALPHA )
#define PM_UM_UNBALANCE_DELTA_ALPHA				(240)
#else
#define PM_UM_UNBALANCE_DELTA_ALPHA  			(CONFIG_PM_UM_UNBALANCE_DELTA_ALPHA)
#endif

#if !defined(CONFIG_PM_UM_UNBALANCE_DELTA_SPEED)
#define PM_UM_UNBALANCE_DELTA_SPEED	(2)
#else
#define PM_UM_UNBALANCE_DELTA_SPEED  (CONFIG_PM_UM_UNBALANCE_DELTA_SPEED)
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Tach struct */
typedef struct tachData_s
{
    uint32_t curr_meas;             /* current measure of the period in us */
    uint32_t last_meas;             /* last measure of the period in us */
    uint16_t curr_rpm;              /* current rpm conversion */ 
} tachData_t;

/** state Machine struct */
typedef struct stateMachine_s
{
    uint8_t    curr_phase;	    /* fase motore corrente */
    uint8_t     rele_delay_cnt;     /* ritardo per attivazione rele */
    uint8_t     engineSpin;
    int16_t 	diffv;              /* differenza rispetto alla v finale rampa */
    int16_t 	deltav;             /* delta tra v iniziale e finale della rampa */
    uint16_t    step;
    uint16_t    stepcnt;            /* contatore gestione rampa */
    uint16_t    v_to;               /* velocita' finale rampa */
    uint16_t 	v_tgt_abs;          /* v target in valore assoluto */
    uint16_t 	cmdflags;           /* copia dell'omonimo campo in MotorCmd */
    uint8_t     flags;               /* flags di stato  bit0:1=mezzo campo attivo */
    uint16_t    t_tgt;              /* t target */
} stateMachine_t;

typedef struct sethf_s
{
    uint16_t    ext_hf_rpm;         /* velocita' mezzocampo impostata dall'esterno in rpm */
    uint8_t     ext_hf_pwd;         /* password impostazione mezzocampo */
} sethf_t;

typedef struct Unbalance_s
{
    uint8_t  init;               /* */
    uint16_t cnt;               /* */
    uint16_t acc_cnt;           /* */
    uint16_t measure;           /* */
    uint32_t acc;               /* */
    uint16_t max_tick;          /* */
    uint16_t min_tick;          /* */
    uint8_t  discard;			/* */
    uint8_t  gripFlag;
} Unbalance_t;


typedef union engineControl_u
{
    uint8_t all;
    struct engineControl_s
    {
        uint8_t softStartAlphaMin:1;		/* if TRUE SoftStart sta applicando l'angolo minimo consentito */
        uint8_t vFreq:1;					/* if '1' 50 Hz */
        uint8_t alphaStep:1;				/* if '1' variazione alpha a step if'0' con formula */
        uint8_t halfField:1;				/* if '1' mezzo campo presente */
        uint8_t triacState:1;				/* if '0' triac output enabled */
        uint8_t noTachoDuringEngineOn:1;	/* if '1' nessun segnale tachimetrica per troppo tempo */
        uint8_t softStartAlphaMin_TimeLimit:1;  /* 1: angolo min e' stato applicato x il tempo limite 0: */
        uint8_t universalEngineIsPresent:1;	/* if '1' gestione motore universale */
    } bit;
} engineControl_t;

typedef struct outOfRangeParams_s
{
    uint8_t stepOutOfRange_dn;
    uint8_t ratioVelTarget_dn;
    uint8_t stepOutOfRange_up;
    uint8_t ratioVelTarget_up;
    uint8_t maxDev;

} outOfRangeParams_t;


typedef enum {
	PM_UM_TRIAC_MOTOR_OC_HIGH=0,
	PM_UM_TRIAC_MOTOR_OC_LOW,
}
PM_UM_TriacMotorOC_CfgType;

typedef struct
{
    PM_UM_InitData      cfg;
    uint16_t            target_rpm;     /* ramp absolute speed  */
    Unbalance_t         unbalance;
    tachData_t          tach;
    uint8_t             motorflags;
    PM_UM_MotorCmd 		motorCmd;
    stateMachine_t 		stateMachine;
    
    engineControl_t     engineControlFlags;
    filterParams_t      filterParams;
   
    uint16_t 	        alpha_n;		/** Valore angolo parzializzazione di fase all'istante (n) espresso in tick del timer */
    uint16_t            alpha_n_1;		/** Valore angolo parzializzazione di fase all'istante (n-1) espresso in tick del timer */

    PM_UM_TriacMotorOC_CfgType	eOcCfg;
} PM_UM_CtrlVar_t;


typedef struct
{
    uint8_t				tachFlags;
    uint8_t  			cnt_no_tach;
    uint8_t  			noTachoInEnginePhaseOn_Cntr;
    uint8_t				softStartAlphaMin_cnt;
    bool8_t 			bSuspendTriac;
#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
    bool8_t 			bSoftStartSuspend;
#endif /* defined(CONFIG_MOTORGATE_SUSPEND_TRIAC_AT_SOFT_START) */

} PM_UM_SftType;
/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

extern uint8_t  Get_common_tacho_poles(void);
extern uint16_t Get_common_alpha_max(void); 
extern uint16_t Get_common_alpha_min(void);
extern uint16_t Get_common_alpha_ini_soft_start(void);
extern uint8_t  Get_common_alpha_step_soft_start(void);
extern uint16_t Get_common_min_fire_triac(void);
extern uint8_t Get_common_RTx10(void);

void startUnbalance(void);
void idleUnbalance(void);

/** ritorna il valore diell'angolo di parzializzazione di fase attuale */
uint16_t getAlpha_n( void );

/** Funzione per la gestione dell'universal motor, va chiamata ogni 10ms*/
void PM_UM_MotorManager (void);

void PM_UM_ActivateTask( void );
bool_t PM_UM_IsWaiting4Tach( void );

void PM_UM_OverrideTriacSuspend(bool8_t bNewSuspend);
#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
void PM_UM_SoftStartSuspend(bool8_t bNewSuspend);
#endif /* defined(CONFIG_MOTORGATE_SUSPEND_TRIAC_AT_SOFT_START) */

/** for lookup table use */
extern PM_UM_CtrlVar_t PM_UM_CtrlVar;
extern sethf_t set_hf;
#endif /* __PM_UM_UNIVERSAL_MOTOR_H */

/* *********** */
/* End of file */
/* *********** */
/*@}*/
