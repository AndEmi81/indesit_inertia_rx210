/*
** **************************************************************************
** **************************************************************************
*/
/**
Contiene le dichiarazioni dei tipi per il livello applicazione.

\file    	CstmTypes.h	

\version 	1.0.0
\date    	21/12/2005
\authors	Simone Costantini - Valeria Catani - Lucio Bianchi
*/
/*
** **************************************************************************
** **************************************************************************
*/

/*
Copyright (c) 2004-2006, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version
\date
\author
*/

#ifndef __CSTMTYPES_H
#define __CSTMTYPES_H

/* ******** */
/* Includes */
/* ******** */
#ifndef AUTOCONF_INCLUDED
#include "autoconf.h"
#endif /* !AUTOCONF_INCLUDED */
#include "StandardTypes.h"
#include "CstmDefines.h"
#include "SwTimersCounters.h"
#include "Config.h"
#include "APP_Types.h"
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#include "BCP_BusCommMng.h"
#endif


/* ****************** */
/* Defines and Macros */
/* ****************** */

/*#if defined(RN_RX200_R5F5210) */

/* La seguente define permette di stabilire in che modo devono essere impacchettate le strutture dati:
 * se PACK_USING_MACROS == FALSE allora viene utilizzato il metodo di impacchettamento messo a disposizione
 * dal compilatore (es: __attribute__((packed)) per il GCC, #pragma pack per altri compilatori).
 *
 * Qualora il compilatore non metta a disposizione alcun metodo di impacchettamento allora si puo'
 * definire PACK_USING_MACROS == TRUE
 * In questo modo la macro qui sotto permettono di definire le strutture dati in modo da garantire
 * il corretto impacchettamento e di accedere ai campi in modo astratto.
 */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct _GIMap_Struct{
/* FC: struttura verificata!! */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t readType:4;
	uint8_t free	:4;


	union{

		uint8_t	b;

		struct{
			uint8_t type	:4;
			uint8_t index	:4;
		}s;

	}id;

	uint8_t hlParamOffset;
	uint8_t pos;

#else /* __BIT_ORDER_MSB_FIRST__ */
	uint8_t free	:4;
	uint8_t readType:4;		
	
	union{
	
		uint8_t	b;
		
		struct{
		
			uint8_t index	:4;
			uint8_t type	:4;	
		}s; 
	
	}id;
						
	uint8_t hlParamOffset;	
	uint8_t pos;				

#endif /* !__BIT_ORDER_MSB_FIRST__ */

}GIMap_Struct;

/*
Output Map
*/
/** Elemento dell'Output Map */
typedef struct _OutputMap_Struct
{
#if defined(__BIT_ORDER_MSB_FIRST__)

	/* byte 0 */
	union
	{
		uint8_t	b;
		struct
		{

			uint8_t type	:4;
			uint8_t index	:4;
		}s;
	} id;

	/* byte 1 */
	uint8_t feedbackParamOffset;

	/* byte 2 */
	uint8_t ext				:1;
	uint8_t pilotType		:5;
	uint8_t free_b2			:2;



	/* byte 3 */
	uint8_t free_b3			:2;
	uint8_t pinTabOffset	:6;


	/* byte 4 */
	uint8_t pilotTypeParamOffset;
	/* byte 5 */
	uint8_t priorityBitmap;

	/* byte 6 */
	uint8_t HiLoActivatione	:4;
	uint8_t maxRegulation	:4;


	/* byte 7 */
	uint8_t loadTypeParamOffset;

#else /* __BIT_ORDER_MSB_FIRST__ */

	/* byte 0 */
	union
	{
		uint8_t	b;
		struct
		{
			uint8_t index	:4;
			uint8_t type	:4;
		}s;
	} id;

	/* byte 1 */
	uint8_t feedbackParamOffset;

	/* byte 2 */
	uint8_t free_b2			:2;
	uint8_t pilotType		:5;
	uint8_t ext				:1;

	/* byte 3 */
	uint8_t pinTabOffset	:6;
	uint8_t free_b3			:2;

	/* byte 4 */
	uint8_t pilotTypeParamOffset;
	/* byte 5 */
	uint8_t priorityBitmap;

	/* byte 6 */
	uint8_t maxRegulation	:4;
	uint8_t HiLoActivatione	:4;

	/* byte 7 */
	uint8_t loadTypeParamOffset;

#endif /* !__BIT_ORDER_MSB_FIRST__ */

} OutputMap_Struct;

typedef struct PACKED _GITabSpa_Struct{
  uint8_t Hb;           /** Hi-bound */
	uint8_t Lb;	          /** lo-bound */
	uint8_t	Samples;	    /** samples */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t unused		:7;
	uint8_t On        :1; /**   Valore di on */
#else
	/** Flags*/
	uint8_t On        :1; /**   Valore di on */
	uint8_t unused		:7;
#endif
} GITabSpa_Struct;

typedef struct PACKED _Id_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)

	uint8_t type		:4;
	uint8_t index       :4; /**   Valore di on */
#else
	uint8_t index	:4;
	uint8_t type	:4;
#endif
}Id_Struct;


typedef struct PACKED _GICriterionDigitalTab_Struct{

	uint8_t Filter; /* filtro di stabilità sui campioni */
	uint8_t free;

} GICriterionDigitalTab_Struct;


/*parametri per lo spunto della ventola*/
typedef struct PACKED _Surge_Fan_Params_Struct{

	uint8_t	fan_startup_timer;
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t free				:6;
	uint8_t fan_startup_speed 	:2;
#else

uint8_t fan_startup_speed 	:2;
	uint8_t free				:6;
#endif

}Surge_Fan_Params_t;


typedef union PACKED _Over_Power_Bldc_Struct{

	uint8_t b;
	struct {
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t over	:4;
	uint8_t status	:4;
#else
	uint8_t status	:4;
	uint8_t over	:4;
#endif
	}s;
}Over_Power_Bldc_Struct;

typedef struct PACKED _uP_Maintain_Struct
{
	/* byte 0 */
	uint8_t status;							/* stato */
	/* byte 1 */
	SWTC_CounterConfigurationType timer_config;	/* configurazione del timer */
	/* byte 2 */
	M_COUNTER_FIELD( timer);		/* timer */
	/* byte 3 - 4 */
	UINT16_FIELD(time_target);					/* accumulatore */
	/* byte 5 - 6 */
	uint8_t *pModTab;           		/* pnt alla tab modificatori */
} uP_Maintain_Struct;

typedef struct PACKED _loadsPulse_Struct{
	uint8_t offset;
	uint8_t step;
	M_COUNTER_FIELD(timer);
	SWTC_CounterConfigurationType   timer_config;

}loadsPulse_Struct;

typedef struct PACKED _uP_LoadsControl_Struct{

	uint8_t status;				/* stato esecuzione uP */

}uP_LoadsControl_Struct;

/* DRAIN */
typedef struct PACKED _uP_drain_Struct{

	uint8_t step;	            						/* stato uP */
	SWTC_CounterConfigurationType	timer_config;		/* timer config */
	M_COUNTER_FIELD(timer);				/* timer */
} uP_Drain_Struct;

typedef struct PACKED _uP_Heat_Struct{

	uint8_t	tgTemp;				/* temp target */
	uint8_t status;				/* stato esecuzione uP */

}uP_Heat_Struct;

/* 7 LITER */
typedef struct PACKED _uP_7liter_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t Sir_I_HaveLoadedSomeWater:1;                /* flag che indica se ho caricato dell'acqua nella tasca 7 litri */
	uint8_t status:7;             						/* stato */
#else
	uint8_t status:7;             						/* stato */
	uint8_t Sir_I_HaveLoadedSomeWater:1;                /* flag che indica se ho caricato dell'acqua nella tasca 7 litri */
#endif
	SWTC_CounterConfigurationType	timer_config;		/* No Backup */
	M_COUNTER_FIELD(timer);			/* No Backup */
	uint16_t timeON_ev;								/* target time */
	uint16_t timeOFF_ev;							/* target time */
}uP_7liter_Struct;
/* PUMP CONTROL */
typedef struct  PACKED _uP_PumpControl_Struct{
	uint8_t step;
	uint8_t flags;
}uP_PumpControl_Struct;


/* ALTERNATE CONTROL */
typedef struct PACKED _uP_AltCtrl_Struct{

  	SWTC_CounterConfigurationType	timer_config;
  	SWTC_SCounterVariableType timer;			/* timer */
  	SWTC_CounterConfigurationType	filter_timer_config;
  	SWTC_SCounterVariableType filter_timer;			/* timer */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t step			:6;           /* stato */
	uint8_t seq_done_could_esc :1;		/* variabile per permettere avanzamento del passo una volta eseguita almeno una volta la sequenza*/
	uint8_t SV_invalid_4alt	:1;           /* stato */
#else
	uint8_t SV_invalid_4alt	:1;           /* stato */
	uint8_t seq_done_could_esc :1;		/* variabile per permettere avanzamento del passo una volta eseguita almeno una volta la sequenza*/
	uint8_t step			:6;           /* stato */
#endif
	uint8_t timing;         /* tempistica */
	uint8_t curr_pos;	    /* posizione corrente Alternato*/
	uint8_t curr_cmd;        /* comando corrente */
}uP_AltCtrl_Struct;


typedef struct PACKED _uP_Load_Struct{

	uint8_t levTOtimeRef;		  /* valore temporale di riferimento per raggiungere il P.P (portata) */
  	uint8_t waterInDWInit;        /* acqua in vasca ad inizio caricamento */
	uint8_t	tgWater;			  /* livello di acqua target */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t	status        :3;	/* stato esecuzione uP */
	uint8_t free		  :1;
	uint8_t turbina_KO	:1; /* dichiare se la turbina è funzionante o meno*/
	uint8_t VS_check	:1;  /* serve per verificare lo stato del sensore Virtuale*/
	uint8_t pump_act    :1;  /* indica se la pompa lavaggio deve essere attivata*/
  	uint8_t init 		: 1; /* bit per inizializzazione waterInDWInit */
 #else
  	uint8_t init 		: 1; /* bit per inizializzazione waterInDWInit */
	uint8_t pump_act    :1;  /* indica se la pompa lavaggio deve essere attivata*/
	uint8_t VS_check	:1;  /* serve per verificare lo stato del sensore Virtuale*/
	uint8_t turbina_KO	:1; /* dichiare se la turbina è funzionante o meno*/
	uint8_t free		:1;
	uint8_t	status      :3;	/* stato esecuzione uP */
#endif
	SWTC_CounterConfigurationType	levTOtime_cnt_config;
	SWTC_CounterConfigurationType	fullStability_cnt_config;
	SWTC_CounterConfigurationType	timeLoad_cnt_config;
	SWTC_CounterConfigurationType 	pump_Act_4VS_cfg;
	SWTC_SCounterVariableType		levTOtime_counter;			/* contatore temporale per calcolo portata  */
	SWTC_SCounterVariableType		fullStability_counter;		/* contatore stabilità pressostato in pieno per caricamento a P.P.*/
	SWTC_SCounterVariableType		timeLoad_counter;			/* contatore temporale per caricamento a tempo (in base a portata) */
	SWTC_SCounterVariableType 		pump_Act_4VS_counter;		/* contatore temporale per check del sensore virtuale) */
	UINT16_FIELD(time) ;				    /* tempo totale di caricamento - per caricamento a portata */
	UINT16_FIELD(timeAcc) ;				/* accumulatore temporale per caricamento */

}uP_Load_Struct;

typedef struct PACKED _uP_LoadRestore_Struct{

  uint8_t maxWater;			        /* quantitÃ  di acqua max */
  uint8_t status;               /* stato esecuzione uP */
  uint8_t waterInDWInit;        /* acqua in vasca ad inizio caricamento */
  uint8_t init;                 /* inizializzazione caricamento in caso di vuoto */
  SWTC_CounterConfigurationType	timeLoad_cnt_config;
  SWTC_SCounterVariableType		timeLoad_counter;/* contatore temporale per caricamento a tempo (in base a portata) */
  UINT16_FIELD(fullPulseIn);         /* variabile per impulsi in ingresso in pieno */
  UINT16_FIELD(emptyPulseIn) ;	     /* variabile per impulsi in ingresso in vuoto */
  UINT16_FIELD(time) ;				 /* tempo totale di caricamento - per caricamento a portata */
  UINT16_FIELD(timeAcc) ;			/* accumulatore temporale per caricamento */

}uP_LoadRestore_Struct;


/* REGENERATE */
typedef struct PACKED _uP_Regenerate_Struct{


	SWTC_CounterConfigurationType	EVdelay_cnt_config;		/* No Backup */
	SWTC_SCounterVariableType		EVdelay_counter;		/*counter per delay attivazione EV*/

	UINT16_FIELD( tout_cnt);			/*timeout rigenera*/

  	uint8_t waterPulseReg;  /* impulsi di rigenera (in corso) */

  	uint8_t status;				/* stato esecuzione uP */

  	uint8_t forceReg_cnt;	 /*contatore per forzare rigenera */

  	uint8_t nReg;		/* numero rigenere eseguite */
#if defined(__BIT_ORDER_MSB_FIRST__)
  	uint8_t resetAfterRegReq:1;	 /* 1: richiesto reset dopo rigenera */
	uint8_t waterPulseRegReq:7;	 /* impulsi richiesti per rigenera */
#else
  	uint8_t waterPulseRegReq:7;	 /* impulsi richiesti per rigenera */
	uint8_t resetAfterRegReq:1;	 /* 1: richiesto reset dopo rigenera */
#endif
}uP_Regenerate_Struct;

typedef struct PACKED _Reg_WaterLoaded_Struct{

  uint8_t l;    /* litri accumulati ai fini della rigenera */
  uint8_t dl;   /* dl accumulati ai fini della rigenera */

}Reg_WaterLoaded_Struct;

typedef struct PACKED _SetProfile_Struct
{
	/* Byte 0 */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t status				:5;             /* stato */
	uint8_t corr_factor			:1;
	uint8_t cntrl_profile		:1;
	uint8_t	end_profile			:1;
#else
	uint8_t	end_profile			:1;
	uint8_t cntrl_profile		:1;
	uint8_t corr_factor			:1;
	uint8_t status				:5;             /* stato */
#endif
	/* Byte 1 */
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t	 c_counter			:4;
	uint8_t	 nSeg_counter		:4;
#else
	uint8_t	 nSeg_counter		:4;
	uint8_t	 c_counter			:4;
#endif

	/* Byte 2-3 */
	UINT16_FIELD(loadMap_profile) ;

	/* Byte 4-5 */
	M_COUNTER_FIELD(seg_timer) ; /* timer */

	/* Byte 6 */
	SWTC_CounterConfigurationType	seg_cnt_config;


} uP_SetProfile_Struct;

typedef struct PACKED _uP_SparklingAid_Struct{

	uint8_t turbidity1;           /* lettura 1 torbidità */
	uint8_t turbidity2;           /* lettura 2 torbidità */
	uint8_t IdRead;               /* id di riferimento lettura torbidità */
	uint8_t step;                 /* stato uP */

	SWTC_CounterConfigurationType	timer_config;		/* timer config */
	M_COUNTER_FIELD(timer)	;

	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		uint8_t Repeat_reg_dx    :7;
		uint8_t RepeatTurbRead   :1;  /* Ripetizione lettura torbidità */
#else
		uint8_t RepeatTurbRead   :1;  /* Ripetizione lettura torbidità */
		uint8_t Repeat_reg_dx    :7;
#endif
	}cmd;
}uP_SparklingAid_Struct;

/* algo */
typedef struct PACKED _al_DirtyFoam_Struct{

  uint8_t waterInDWInit;        /* acqua in vasca ad inizio caricamento */
#if defined(__BIT_ORDER_MSB_FIRST__)
  uint8_t status		:3;
  uint8_t load_exe_time :1;
  uint8_t filter_foam_on:1;
  uint8_t reduc_speed	:2;		/* riduzione della velocità della pompa per sporco*/
  uint8_t init		    :1;         /* inizializzazione caricamento in caso di vuoto */



  		/* free*/
#else
  uint8_t init			:1;         /* inizializzazione caricamento in caso di vuoto */
  uint8_t reduc_speed	:2;		/* riduzione della velocità della pompa per sporco*/
  uint8_t filter_foam_on:1;
  uint8_t load_exe_time	:1;
  uint8_t status		:3;			/* free*/

#endif
  SWTC_CounterConfigurationType	aal_Stab_sens_cnt_config;
  SWTC_SCounterVariableType		aal_Stab_sens_counter;/* contatore temporale per caricamento a tempo (in base a portata) */
  uint16_t fullPulseIn;         /* variabile per impulsi in ingresso in pieno */
  uint16_t emptyPulseIn;        /* variabile per impulsi in ingresso in vuoto */
  uint16_t time;				        /* tempo totale di caricamento - per caricamento a portata */
  uint16_t timeAcc;			        /* accumulatore temporale per caricamento */
  SWTC_CounterConfigurationType	aal_stab_stato3_config;
  SWTC_SCounterVariableType		aal_stab_stato3_counter;/* contatore temporale per lo stato 3 del sensore virtuale */

}al_DirtyFoam_Struct;



typedef struct  PACKED _ReadVS_struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
    uint8_t free                    :5;
    uint8_t control_done            :1;
	uint8_t ctrl_active				:1;
	uint8_t pumpType                :1;

#else

	uint8_t pumpType                :1;
	uint8_t ctrl_active				:1;
	uint8_t control_done            :1;
    uint8_t free                    :5;
#endif
	SWTC_CounterConfigurationType	timer_config;		/* No Backup */
	M_COUNTER_FIELD(timer);			/* No Backup */
	UINT16_FIELD( targetSpeed);		/* target speed */
	UINT16_FIELD( targetRamp);	    /* target time */
}ReadVS_struct;

typedef struct PACKED _Pump_Load_Struct
{
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t state   :7;
	uint8_t init	:1;
#else
	uint8_t init	:1;
	uint8_t state   :7;

#endif
	SWTC_CounterConfigurationType	timer_config;
	 M_COUNTER_FIELD(timer);
}Pump_Load_Struct;

typedef struct PACKED _TurbiditySensorCal_Struct
{
	UINT16_FIELD(  val);                /* valore pwm di calibrazione */
	UINT16_FIELD(  adH2oRef);           /* valore AD di rifermento per H2O pulita */
	uint8_t   pwd;                /* password per richiesta calibrazione */
	uint8_t   state;              /* stato della calibrazione */
	union _flags
	{
		uint8_t flags;
		struct bit
		{
#if defined(__BIT_ORDER_MSB_FIRST__)
			uint8_t free           :7;
			uint8_t cmdAcquireDone :1;    /* comando acquisizione */
#else
			uint8_t cmdAcquireDone :1;    /* comando acquisizione */
			uint8_t free			:7;
#endif
		}bit;
	}flags;
	uint8_t   idRead;             /* id ultima lettura disponibile */
	uint8_t   adH2oRefDelta;       /* 1% margin */

} TurbiditySensorCalibration_Struct;

typedef struct PACKED  _TurbiditySensor_Struct
{
	UINT16_FIELD(  cstm_pwmval);        /* valore pwm custom (per mille)*/
	UINT16_FIELD(  pwm);                /* valore pwm corrente (per mille)*/
	UINT16_FIELD(  pwmval);             /* valore pwm corrente (tick)*/
	uint8_t   pwd;                /* password abilitazione pwmval  */
	uint8_t   state;              /* stato della calibrazione */
	uint8_t   lvl;                /* livello: nel caso di SOLA lettura custom continua il livello di torbidità corrispondente
                                viene aggiornato solo in questo campo e non in GITurbidity */
} TurbiditySensor_t;

/* Sparkling Aid */
typedef struct PACKED _SparklingReads_Struct{

	uint8_t  ADvalue;           /* media valori AD */
	uint8_t  nSamples;          /* numero di campioni */

}SparklingReads_t;

#if (VIRTUAL_SENSOR_ENABLE == 1)
typedef struct PACKED _state5_struct
{
	UINT16_FIELD ( 	max_speed_value);
}state5_struct;

typedef struct PACKED _New_SafetyVs_Struct{

#if defined(__BIT_ORDER_MSB_FIRST__)
  uint8_t free 			:4;
  uint8_t drainStatus   :3;
  uint8_t drain_act		:1;
#else
  uint8_t drain_act		:1;
  uint8_t drainStatus   :3;
  uint8_t free 			:4;
#endif
  SWTC_CounterConfigurationType	timer_config;	/* No Backup */
  SWTC_SCounterVariableType		timer;			/* No Backup */
}New_SafetyVs_Struct;

#endif


typedef struct _SftPulseWarn_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t pilot_disable        : 1;
	uint8_t mode    			 : 2;
	uint8_t vs_presence_h2o   	 : 1;
	uint8_t status 				 : 4;
#else
	uint8_t status        : 4;
	uint8_t vs_presence_h2o    : 1;
	uint8_t mode          : 2;
	uint8_t pilot_disable : 1;
#endif
}SftPulseWarn_Struct;

/* general*/



/* Turbidity */
typedef struct _TurbidityReads_Struct{

	UINT32_FIELD(    a);               /* accumulatore */
	UINT16_FIELD (  ADvalue) ;         /* torbidità AD (media valori AD di nSamples campioni)  */
	uint8_t     nSamples;        /* numero di campioni per il calcolo della media*/
	uint8_t     readers;         /* lettori */
	uint8_t     id;              /* indice lettura */
	uint8_t     perc;            /* torbidità percentuale */
	union
	{
	    uint8_t    all;
	    struct
	    {
#if defined(__BIT_ORDER_MSB_FIRST__)
	        uint8_t unused:5;
			uint8_t SampleInit:1;
			uint8_t H2ORefNull:1;/* TurbiditySensorCal.adH2oRef vale zero */
			uint8_t AdTooLarge:1;/* ADvalue > di TurbiditySensorCal.adH2oRef +TurbiditySensorCal.adH2oRefDelta */
#else
	        uint8_t AdTooLarge:1;/* ADvalue > di TurbiditySensorCal.adH2oRef +TurbiditySensorCal.adH2oRefDelta */
	        uint8_t H2ORefNull:1;/* TurbiditySensorCal.adH2oRef vale zero */
					uint8_t SampleInit:1;
	        uint8_t unused:5;
#endif
	    }b;
	}flags;
	uint8_t*    pReadings;		 /* pnt a numero di campioni */

}TurbidityReads_t;


/* Hardware Configuration */
union _HWC_Flags_u
{
	UINT16_FIELD( flags) ;
  struct _bit
  {
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint16_t UIP:1;      	     /* presenza (1)- assenza(0) UIP */
	uint16_t bldc1:1;           /* presenza (1)- assenza(0) pilotaggio bldc_wash */
	uint16_t bldc:1;            /* presenza (1)- assenza(0) bldc */
	uint16_t alternate:1;       /* presenza (1)- assenza(0) alternato */
	uint16_t standby_dis:1;     /* stand by abilitato(0) - disabilitato(1) */
	int16_t stby_wait_dis :1;	/* sleep mode abilitato (0) - disabilitato (1)*/

	uint16_t bldc_drain :1;		/* presenza (1)- assenza(0) pilotaggio bldc_drain*/
	uint16_t free:3;            /* free */
	uint16_t vs_drain_presence:1;/* presenza (1)- assenza(0) sensore virtuale pompa scarico */
	uint16_t vs_wash_presence:1;/* presenza (1)- assenza(0) sensore virtuale pompa lavaggio */
	uint16_t flowmeter:1;       	/* presenza (1)- assenza(0) flowmeter */
	uint16_t sparklaidreed:1;   /* presenza (1)- assenza(0) reed brillantante */
	uint16_t saltreed:1;        /* presenza (1)- assenza(0) reed sale */
	uint16_t turbsens:1;        /* presenza (1)- assenza(0) sensore di torbidità */
#else
    uint16_t turbsens:1;        /* presenza (1)- assenza(0) sensore di torbidità */
    uint16_t saltreed:1;        /* presenza (1)- assenza(0) reed sale */
    uint16_t sparklaidreed:1;   /* presenza (1)- assenza(0) reed brillantante */
    uint16_t flowmeter:1;       /* presenza (1)- assenza(0) flowmeter */
    uint16_t vs_wash_presence:1;/* presenza (1)- assenza(0) sensore virtuale pompa lavaggio */
	uint16_t vs_drain_presence:1;/* presenza (1)- assenza(0) sensore virtuale pompa scarico */
	uint16_t free:3;            /* free */
	uint16_t bldc_drain :1;		/* presenza (1)- assenza(0) pilotaggio bldc_drain*/
	uint16_t stby_wait_dis :1;	/* sleep mode abilitato (0) - disabilitato (1)*/
    uint16_t standby_dis:1;     /* stand by abilitato(0) - disabilitato(1) */
    uint16_t alternate:1;       /* presenza (1)- assenza(0) alternato */
    uint16_t bldc:1;            /* presenza (1)- assenza(0) bldc */
    uint16_t bldc1:1;           /* presenza (1)- assenza(0) pilotaggio bldc_wash */
    uint16_t UIP:1;      	     /* presenza (1)- assenza(0) UIP */

#endif
  }bit;
};

typedef union _HWC_Flags_u HWC_Flags_Struct;

/*
    Modifiers
*/
typedef struct _ModifierL1Table_s
{
#if defined(__BIT_ORDER_MSB_FIRST__)

	uint8_t dataFormat			:1;		/* '0' -> il dato nel secondo byte è un dato diretto
										   '1' -> il dato nel secondo byte è l'indice dello
											spiazzamento di una tabella */
	uint8_t modLogic		    :1;     /* logica da utilizzare nel caso di più modificatori.
    non implementato: viene sempre applicato il primo modificatore attivo  */
	uint8_t varPos	            :6;		/* posizione della variabile che agisce da modificatore
											nella tabella Modifier Variables Table in HW
											Configuration */
#else
	uint8_t varPos	            :6;		/* posizione della variabile che agisce da modificatore
											nella tabella Modifier Variables Table in HW
											Configuration */
	uint8_t modLogic		    :1;     /* logica da utilizzare nel caso di più modificatori.
	                                        non implementato: viene sempre applicato il primo modificatore attivo  */

	uint8_t dataFormat			:1;		/* '0' -> il dato nel secondo byte è un dato diretto
										   '1' -> il dato nel secondo byte è l'indice dello
											spiazzamento di una tabella */
#endif
	uint8_t data;                   /* indice spiazzamento tabella di livello 2 del
											modificatore (o valore modificato diretto) */

} ModifierL1Table_t;


typedef struct PACKED _TabHeader_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
	 uint8_t unused		:2;
	 uint8_t Int        :1;     /** Interpolazione */
	 uint8_t S2	    	:2;	    /** S2*/
	 uint8_t S1	        :2;	    /** S1*/
	 uint8_t PD         :1;     /** PullDownProbe */

#else
	uint8_t PD          :1;     /** PullDownProbe */
	uint8_t S1	        :2;	    /** S1*/
	uint8_t	S2	        :2;	    /** S2*/
	uint8_t Int         :1;     /** Interpolazione */
	uint8_t unused		:2;
#endif
	uint8_t pointNumber;	    /**	PointNumber*/

} TabHeader_t;

typedef struct PACKED _TabQueryResult_s
{
	UINT16_FIELD(    data);
	UINT16_FIELD(    oBoundVal);
    struct
    {
#if defined(__BIT_ORDER_MSB_FIRST__)
	 uint8_t free			:7;
     uint8_t ubound  :1;
	 uint8_t lbound  :1;             /*1: ricerca del valore esatto   0: range (<=) */

#else
        uint8_t lbound  :1;
        uint8_t ubound  :1;
        uint8_t free			:7;
#endif
    }b;
}TabQueryResult_t;

typedef union PACKED _TabQueryOpt_s
{
    uint8_t all;
    struct {
#if defined(__BIT_ORDER_MSB_FIRST__)
	 uint8_t free			:7;
	 uint8_t findexactvalue  :1;             /*1: ricerca del valore esatto   0: range (<=) */
#else
    uint8_t findexactvalue  :1;             /*1: ricerca del valore esatto   0: range (<=) */
    uint8_t free			:7;
#endif
    }b;
}TabQueryOpt_t;

typedef struct PACKED _TabQueryIn_s
{
    TabQueryOpt_t   opt;
    TabHeader_t*    tab;
    UINT16_FIELD ( cfr_val) ;

}TabQueryIn_t;

typedef struct PACKED _Sft_DrainPP_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
  uint16_t init_empty:1;   /* QI init counter permanenza in vuoto */
  uint16_t init_full :1;   /* QI init counter delay scarico*/
  uint16_t cnt       :14;  /* QI da byte a word per contare anche delay scarico */
#else
  uint16_t cnt       :14;  /* QI da byte a word per contare anche delay scarico */
  uint16_t init_full :1;   /* QI init counter delay scarico*/
  uint16_t init_empty:1;   /* QI init counter permanenza in vuoto */
#endif
}Sft_DrainPP_Struct;


typedef struct PACKED _FltFlagsVS_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
	  uint8_t warninglostwater 			: 4;
	  uint8_t VScheckinconsistency      : 4;
#else
	  uint8_t VScheckinconsistency      : 4;
	  uint8_t warninglostwater 			: 4;
#endif
}FltFlagsVS_Struct;
/* fault */
/*utilizzata per tout Heat e Load */
typedef struct PACKED _FltTout_Struct{
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t uP_exe:1;  /* indica passaggio per uP in quell'iterazione */
	uint8_t state :7;  /* stato di esecuzione (var tra uP e Fault) */
#else
  uint8_t state :7;  /* stato di esecuzione (var tra uP e Fault) */
  uint8_t uP_exe:1;  /* indica passaggio per uP in quell'iterazione */
#endif
  SWTC_CounterConfigurationType	timer_config;	/* No Backup */
  M_COUNTER_FIELD(timer);				/* No Backup */

}FltTout_Struct;


typedef struct PACKED _FltWarning_Struct{

#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t free 						: 7;
	uint8_t bldc_low_voltage_memo		: 1;	/* warning in memoria per tout riscaldamento */
	uint8_t	overheating					: 1;
	uint8_t undervoltage				: 1;
	uint8_t bldc_OH         		 	: 1;	/* warning BLDC OH  dato al prefault*/
	uint8_t heatimeout            		: 1;
	uint8_t bldc_low_voltage		 	: 1;	/* warning BLDC low voltage dato al prefault*/
	uint8_t pulseCounterKO       		: 1;	/* warning turbina KO (ex F07) */  /* QI - 23.10.09 */
	uint8_t outOfRange_EVKO      		: 1;  /*warning tentativi conclusi con fallimento */
	uint8_t load                 		: 1;  /*warning H2O durante tout Load*/
#else
	uint8_t load                 		: 1;  /*warning H2O durante tout Load*/
	uint8_t outOfRange_EVKO      		: 1;  /*warning tentativi conclusi con fallimento */
	uint8_t pulseCounterKO       		: 1;	/* warning turbina KO (ex F07) */  /* QI - 23.10.09 */
	uint8_t bldc_low_voltage		 	: 1;	/* warning BLDC low voltage dato al prefault*/
	uint8_t heatimeout            		: 1;
	uint8_t bldc_OH         		 	: 1;	/* warning BLDC OH  dato al prefault*/
	uint8_t undervoltage				: 1;
	uint8_t	overheating					: 1;
	uint8_t bldc_low_voltage_memo		: 1;	/* warning in memoria per tout riscaldamento */
	uint8_t free 						: 7;
#endif

}FltWarning_Struct;
typedef struct PACKED _FltBLDCUnblock_Struct{
	uint8_t flags;
    uint8_t pumpandspeed;				/* indicatore del tipo di pompa +contatore pacchetti con velocità BLDC > soglia */
	uint16_t wait_cnt;					/* contatore temporale di attesa dei tentativi autonomi BLDC */ /* 07.01.10 */

}FltBLDCUnblock_Struct;


typedef struct PACKED _LoadEVCheck_Struct{
  uint8_t status;
  uint8_t cnt;
  SWTC_CounterConfigurationType	timer_config;	/* No Backup */
  SWTC_SCounterVariableType		timer;			/* No Backup */

}LoadEVCheck_Struct;


/* STATISTICS */
#ifdef _STATISTICS_ON_


typedef struct PACKED _GlobalStatistics_Struct{

	UINT16_ARRAY ( executedCycles_Num,[N_MAX_STAT_CYCLES]);
	UINT16_FIELD( KORestoreCycles_Num);
	UINT16_FIELD( delayedCycles_Num);
	UINT16_FIELD( OnTime);
	uint8_t functionsValues_Buffer[N_MAX_STAT_FUNCTIONS];
	UINT16_FIELD( salt_reset);
	UINT16_FIELD( sparkling_reset);
	UINT16_FIELD( user_cycle_reset);
	UINT16_FIELD( unblockvirtualsensorStateW);/* sbloco stato 5 o stato 6*/
	/*Connectivity field*/
	UINT16_FIELD( numReJoin);
	UINT16_FIELD( executedschedulCycles);
	UINT16_FIELD( backup_num);
	UINT32_FIELD( water_loaded);
	UINT32_FIELD( energyConsumption);
	/**/

}GlobalStatistics_Struct;


typedef uint8_t StatOnOffFunctionsBmp_t;

typedef union PACKED _dishRack_RegenerateCnt_SparkingAidState_u{
    uint8_t dishRack_regenerateCnt_sparklingAidState;
    struct {
#if defined(__BIT_ORDER_MSB_FIRST__)
    uint8_t sparkling_aid_state:1;
	uint8_t dishRack           :2;
	uint8_t regenerate_cnt     :5;
#else

	uint8_t regenerate_cnt     :5;
	uint8_t dishRack           :2;
	uint8_t sparkling_aid_state:1;
#endif
    }b;
}DishRack_RegenerateCnt_SparkingAidState_t;

typedef union PACKED _TurbPerc_TurbCalType_u{
    uint8_t turbPerc_turbCalType [STAT_SAVED_CYCLES_TURB_READS_NUM];
    struct {
#if defined(__BIT_ORDER_MSB_FIRST__)
    uint8_t turbCalType:1;      /* 0: normale 1: PWM default*/
	uint8_t turbidity:7;
#else
	uint8_t turbidity:7;
	uint8_t turbCalType:1;      /* 0: normale 1: PWM default*/
#endif
    }b [STAT_SAVED_CYCLES_TURB_READS_NUM];

}TurbPerc_TurbCalType_t;


typedef union PACKED RestoreNum_KoRestoreNum_u{
    uint8_t restore_koRestore;
    struct {
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t koRestore:4;
	uint8_t restore:4;
#else
	uint8_t restore:4;
	uint8_t koRestore:4;
#endif
    }b;
}RestoreNum_KoRestoreNum_t;

typedef struct PACKED _TempCycleStatistics_Struct{

	uint8_t turbindex;
	uint8_t ONTime_Minutes;   /* Minuti residui rispetto al calcolo dell'On Time (NB: manipolare solo in un punto)*/


	union Lor_flags
	{
		uint8_t flags;
		struct lor_bit
		{
#if defined(__BIT_ORDER_MSB_FIRST__)
			uint8_t free		 :7;
			uint8_t restoreCntEn :1;    /* restore counter enabled */
#else
			uint8_t restoreCntEn :1;    /* restore counter enabled */
			uint8_t free 		 :7;
#endif
		}bit;
	}flags;

	SWTC_CounterConfigurationType	duration_timer_cfg;
	M_COUNTER_FIELD(duration_timer);
	/* Connectivity statistic support */
	 uint8_t clockHH;
	 uint8_t clockMM;
	 UINT16_FIELD( APPEnergyConsumption );
	 UINT16_FIELD( APPTotalDuration );
	 UINT16_FIELD( loaded_water);
	/**/


}TempCycleStatistics_t;


typedef struct PACKED _CycleStatistics_Struct{
	uint8_t cycle;
	uint8_t userH2oHardness;    /* inizio ciclo */
	DishRack_RegenerateCnt_SparkingAidState_t    dishRack_regenerate_spaklingAid;     /* inizio ciclo */
	StatOnOffFunctionsBmp_t             on_off_functions;           /* inizio ciclo */
	TurbPerc_TurbCalType_t              turbPerc_turbCalType;	    /* tbd */
	RestoreNum_KoRestoreNum_t           restore_koRestore;		    /* fine ciclo */
	uint8_t 	                        duration;                   /* fine ciclo */
	sft_fault_codes_t  faultCodes; /*(Code+Subcode);(salvato subito prima di passare al ciclo Fault)*/
	sft_fault_codes_t  prefaultCodes;
	uint8_t coldResetNum;
	uint8_t warmResetNum;
	uint8_t dirty_foam_redux_speed;
#if defined(__BIT_ORDER_MSB_FIRST__)
	uint8_t washzone_reg:4;
	uint8_t rinse_aid:4;
#else
	uint8_t rinse_aid:4;
	uint8_t washzone_reg:4;
#endif
	uint8_t reset_VS_StateW;
	uint8_t state5_reduction;
	uint8_t load_ko;
	/*Connectivity field*/
	uint8_t schedule_mode;
	uint8_t clockHH;
	uint8_t	clockMM;
	uint8_t	delayHH;
	uint8_t	delayMM;
	uint8_t	delayConstrHH;
	uint8_t	delayConstrMM;
	uint8_t	numRejoin;
	uint8_t waterxcycle ;
	uint8_t	turbidity;
	uint32_t energyCycleConsumption;
	uint16_t APPEnergyConsumption ;
	uint16_t APPTotalDuration;
	uint8_t Vline;
	/**/
}CycleStatistics_t;

typedef struct PACKED _BlackBox_Struct{

	uint8_t event;
	sft_fault_codes_t faultCodes;
	uint8_t cycle;
	uint8_t phase;
	uint8_t first_statement;
	UINT16_FIELD( activeLoadsBmp);
	INT16_FIELD ( pumpSpeed);
	UINT16_ARRAY( analogInputs,[8]);
	uint8_t analogfeedbackVal;
	uint8_t digInputs;
	UINT16_FIELD( waterLoaded);
	uint8_t	washTemperature;
	uint8_t turbidity;
	UINT16_FIELD( giValid);
	uint8_t criteria;
	uint8_t coldResetNum;
	uint8_t warmResetNum;
	uint8_t dirty_foam_state;
	CM_Err_ClassType commErr[sizeof(CommErr)/sizeof(CommErr[0])][sizeof(CommErr[0])/sizeof(CommErr[0][0])];
	UINT16_FIELD( State5_algo);
	uint8_t virtual_Sensor[GI_VIRTUAL_SENSOR_NUM];
	uint8_t flags_2copy;
	FltFlagsVS_Struct failnumVs;

}BlackBox_Struct;

typedef struct PACKED _TestCycleStatistics_Struct{
	uint8_t 	coldResetNum;
	uint8_t 	warmResetNum;
	sft_fault_codes_t  prefaultCodes;
	CM_Err_ClassType commErr[sizeof(CommErr)/sizeof(CommErr[0])][sizeof(CommErr[0])/sizeof(CommErr[0][0])];

}TestCycleStatistics_Struct;

/* Almeno i Last Fault ci devono sempre essere, anche se si eliminano le altre statistiche */
typedef struct PACKED  _Statistics_Struct{

	GlobalStatistics_Struct globalStats;	/* Controllare che la dimensione non superi 255 bytes,
										   altrimenti in backup_address.h occorre riportare
										   i campi divisi dato che BackupDimTab[] è un array
										   di uint8_t */
	CycleStatistics_t lastCycles[N_STAT_SAVED_CYCLES];
	LastFault_Struct lastFault[N_MAX_LAST_FAULT];
	LastFault_Struct lastPreFault[N_MAX_LAST_FAULT];
	TestCycleStatistics_Struct testCycleStats;
	BlackBox_Struct blackBox_Data[BB_BLACK_BOX_MAX_NUM];
}Statistics_Struct;

#else

typedef struct PACKED _Statistics_Struct{

	LastFault_Struct lastFault[N_MAX_LAST_FAULT];
	LastFault_Struct lastPreFault[N_MAX_LAST_FAULT];

}Statistics_Struct;


#endif /*_STATISTICS_ON_*/

typedef struct _EngineData3PH_Struct {
	uint8_t buf[IOM_EXTERNAL_0_INFOBUFFER_SIZE];		/* Info buffer DSP */
} EngineData3PH_Struct;



/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/*@}*/

#endif /* __CSTMTYPES_H */

/* *********** */
/* End of file */
/* *********** */

