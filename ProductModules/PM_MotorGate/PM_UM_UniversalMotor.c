/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
Implementazione di un controllo motore collettore per lavabiancheria
algoritmo attualmente implementato sulle lavabiancheria Indesit

\defgroup PM_MotorGate Universal Motor
\author  Stefano Ughi, Roberto Fioravanti, Marco Sperandini 
\ingroup  ProductM   
*/
/*@{*/
/* ******** */
/* Includes */
/* ******** */
#include "LIB_ClassBVar.h"
#if defined(CONFIG_PM_MOTORGATE)
#include "LIB_Utilities.h"


#include "OCTPM_drv.h"
#include "OCDIO_drv.h"

#include "LIB_ZeroCrossingExp.h"
#include "DigitalOutputs.h"
#include "PM_DoorLockExp.h"
#include "PM_UM_Control.h"
#include "PM_UM_Measure.h"
#include "PM_UM_Tachometer.h"
#include "PM_UM_MotorGateDriver.h"
#include "PM_UM_UniversalMotor.h"
#include "PM_UniversalMotorExp.h"
#include "PM_UM_GetParams.h"
#include "Main.h"
#include "LIB_Trace.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */
#define ALPHA_MIN	(150) /* alpha minimo quando in stato MOVIM_ON */
/** Task Stack */
static STACK_SEC OS_STACKPTR int  PM_UM_Task_Stack[CONFIG_PM_UM_TASK_STACK_SIZE] STACK_SEC_POST;
static OS_TASK   		  PM_UM_Task_TCB;

/* list of pin  */
enum pinlist
{
	PIN_TRIAC=0,
	PIN_ACW,
	PIN_CW,
	PIN_HF,
};

#define PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(pinID)	(*(PM_UM_Cfg.pPinTable+pinID))

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


struct {
	uint16_t 	value;
	bool_t		valid;
} PhaseAngle ;


#ifdef CONFIG_PM_UM_TRACE_ENABLE
typedef unsigned char TraceCntType;

typedef struct TraceCollDataItem
{
	unsigned short	tacho;	/*tacho*/
	unsigned short	falpha;	/*filtered alpha*/
	unsigned short	alpha;  /*unfiltered alpha*/
	unsigned char   errors; /*errors :
							hi nibble-> errors by controllore;
							lo nibble-> errors by filter*/
	unsigned char	cnt;	/*counter*/
}TraceCollDataItemType;
TraceCollDataItemType	TraceCollData[CONFIG_PM_UM_TRACE_DEPTH];

TraceCollDataItemType TraceCollTmp;
#endif

typedef struct sPM_UM_ZC
{
	TOOLS_TIMER LastZC;
	bool8_t bSuspendTriac;
}PM_UM_ZC_Type;

/* *************************** */
/* Extern Data 				   */
/* *************************** */

/* *************************** */
/* Local Data 				   */
/* *************************** */
#ifdef CONFIG_PM_UM_TRACE_ENABLE
static TraceCntType  TraceCnt=0,TracePos=0;
#endif

uint16_t            tau_n_1, alpha_n_1;/* nominal */
PM_UM_CtrlVar_t 	PM_UM_CtrlVar;
PM_UM_SftType		PM_UM_Sft;
PM_UM_CfgDataType	PM_UM_Cfg;
#if defined (CONFIG_PARAM_FROM_FIRMWARE)
/* nothing */
#else
uint8_t  *pcommmonparam;
uint8_t	 u8CommonParamSize=0;
uint8_t  *pHalfFieldParam;
#endif  /* CONFIG_PARAM_FROM_FIRMWARE */

/** half field : state machine relates variables */
static uint8_t stateHFrelay     = RELAY_HF_OFF_TRIAC_ON;
static uint8_t tgt_stateHFrelay = RELAY_HF_OFF_TRIAC_ON;

static PM_UM_ZC_Type PM_UM_ZC;

/* mezzocampo impostabile dall'esterno */
sethf_t set_hf = { 0, 0 };


/* **************************** */
/* Prototype of local functions */
/* **************************** */

static void PM_UM_Task(void);

static uint16_t          convTickInRPM( uint32_t measure );
static void              setEngineRelays( void );

static uint8_t           checkIfActHalfFieldRelay( void );
static uint16_t          get_velAttHalfFieldR( void );
static void              set_alpha_n( uint16_t );
static void              resetTachStatistics(void);
static void              motionlessAssumption(void);
static void              calculateUnbalance(void);
static void              set_delay_cnt( void );
uint32_t 				 PM_UM_GetTach(void);

#ifdef CONFIG_PM_UM_TRACE_ENABLE
void SetTraceError(unsigned char err);
void SetTraceAlphaFiltered(unsigned short falpha);
void SetTraceTacho(unsigned short tacho);
void CollectTrace(void);
void SetTraceAlpha(unsigned short alpha);
void InitTrace(void);
#endif



static void PM_UM_ZC_Monitor(void);
static void SetRele( uint8_t val );
static void SetStatus( uint8_t val );

/* ***************** */
/* Exported Functions*/
/* ***************** */

/** Ritorna il valore corrente, con segno, della velocit??? motore. */
int16_t PM_UM_GetMotorSpeed(void)
{
    if(PM_UM_CtrlVar.motorflags & MOTOR_FLAGS_SPEED_SIGN_BIT)
    {
        return(- PM_UM_CtrlVar.tach.curr_rpm);
    }
    else
    {
        return(PM_UM_CtrlVar.tach.curr_rpm);
    }
}

/**  TRUE se SoftStart sta applicando l'angolo minimo consentito. */
bool_t PM_UM_IsPhaseAngleMinAtSoftStart( void )
{
    return (PM_UM_Sft.tachFlags & 4);
}

/** reset del flag  softStartAlphaMin */
void PM_UM_ResetPhaseAngleMinAtSoftStart ( void )
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_UM_Sft);

	PM_UM_Sft.tachFlags &= ~4;

	SAFETY_DATA_UPDATE(PM_UM_Sft);
	SAFETY_REGION_END();
}

/**  restituisce l'angolo di pilotaggio corrente in us*/
uint16_t getAlpha_n( void )
{
     return PM_UM_CtrlVar.alpha_n;
}

/** se TRUE la tachimetrica ??? mancata troppo a lungo durante la fase di on del motore. */

bool_t PM_UM_IsTimeoutTachometer( void )
{
    return (PM_UM_Sft.tachFlags & 1);
}
/** Setup delle variabili velocit??? Target e Tempo di rampa e flag. Chiamata in GIOM_LoadPilot(.). */
void PM_UM_SetTargetSync( const GIOM_LoadStruct *cmd )
{
    PM_UM_MotorCmd motorCmd;
    /* speed target signed */
    motorCmd.v_tgt = *((uint16_t *)((uint8_t *)&cmd->data[GIOM_V_TARGET_INDX]));

    /* tempo target: tempo rampa */
    motorCmd.t_tgt = *((uint16_t *)((uint8_t *)&cmd->data[GIOM_T_TARGET_INDX]));

    motorCmd.cmdflags=*((uint16_t *)((uint8_t *)&cmd->data[GIOM_FLAGS_INDX]));

    motorCmd.cmdflags &= ~(1<<MOTOR_FLAGS_LAST_TARGET_REACHED);
    
    PM_UM_SetMotorCmd( &motorCmd );
}

/** Setup delle variabili velocit??? Target e Tempo di rampa e flag.*/
void PM_UM_SetMotorCmd( PM_UM_MotorCmd *cmd )
{
	PM_UM_CtrlVar.motorCmd = *cmd;
}

/**
 *
 */
uint16_t	PM_UM_GetTargetSync(void)
{
	return PM_UM_CtrlVar.target_rpm;
}



void PM_UM_InitParams(  uint8_t const * paramPtr)
{
	PM_UM_InitData cfgData;

	cfgData.pinTriac = *paramPtr;
	cfgData.pinClockwise= *(paramPtr+2);
	cfgData.pinAntiClockwise = *(paramPtr+1);
	cfgData.pinHalfField = *(paramPtr+3);

	PM_UM_CtrlVar.cfg = cfgData;

    memset(&PM_UM_CtrlVar.stateMachine, 0, sizeof(PM_UM_CtrlVar.stateMachine));

    /* Check If 1/2 campo presente */
    PM_UM_CtrlVar.engineControlFlags.bit.halfField = setHalfFieldParamPtr();

    SAFETY_REGION_START();
	
	SAFETY_DATA_CHECK(PhaseAngle);
	SAFETY_DATA_CHECK(PM_UM_Sft);
	
    if(paramPtr)
    {
		SAFETY_DATA_CHECK(PM_UM_Cfg);
		PM_UM_Cfg.pPinTable = paramPtr;
		PM_UM_Cfg.cfg |= 1;
		SAFETY_DATA_UPDATE(PM_UM_Cfg);

		PM_UM_CtrlVar.engineControlFlags.bit.universalEngineIsPresent = TRUE;

		/* Create Task */
		OS_CREATETASK ( &PM_UM_Task_TCB,
						"PM_UM Task",
						PM_UM_Task,
						( OS_U8 ) PM_UM_TASK_PRIORITY,
						PM_UM_Task_Stack );

		PM_UM_GateDrv_Init( PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_TRIAC) );
		PM_UM_GateDrv_SetImpulseLen( Get_common_min_fire_triac() );
    }
    SAFETY_REGION_END();

#ifdef CONFIG_PM_UM_TRACE_ENABLE
	InitTrace();
#endif
}


/** Procedura inizializzazione del modulo UniversalMotor.

\param  [in] pinlist puntatore alla lista dei pin (4) interessati dal controllo motore
			 1??? Triac
			 2??? Rel??? Orario
			 3??? Rel??? Antiorario
			 4??? Rel??? Mezzo Campo
*/
void PM_UM_InitSync( void )
{
	SAFETY_REGION_START();
	memset(&PM_UM_Cfg, 0, sizeof(PM_UM_Cfg));
	SAFETY_DATA_UPDATE(PM_UM_Cfg);
	memset(&PM_UM_Sft, 0, sizeof(PM_UM_Sft));
	SAFETY_DATA_UPDATE(PM_UM_Sft);
	
	PhaseAngle.valid=TRUE;
    PhaseAngle.value=0;
    SAFETY_DATA_UPDATE(PhaseAngle);
	SAFETY_REGION_END();

	setCommonParamsPtr();
}

void PM_UM_ActivateTask( void )
{
    if ( OS_GetSuspendCnt ( &PM_UM_Task_TCB ) > 0 )
    {
        OS_Resume ( &PM_UM_Task_TCB );
    }
}




/** Macchina a stati gestione motore universale.

Chiamata all'interno del Task Task_10_ms e attivazione ogni STATE_MACHINE_MOTOR_PERIOD volte.

Lettura tachimetrica e conversione del valore in rpm,
se non viene rilevata la tachimetrica entro il timeout NO_TACH_TIMEOUT x 10msec oppure
se il valore della tachimetrica risulta superiore a MAX_TACH_VALUE_TICK (65535)
si assume il motore fermo (tach.curr_rpm=0).

Calcolo sbilanciamento come differenza tra il valore massimo e il valore minimo della
tachimetrica in 1 giro del cesto.

Attivazione Motore:

il passaggio dalla condizione motorCmd.v_tgt=0 a motorCmd.v_tgt>MIN_V_TGT  con applicazione
sincronizzata con lo zero crossing voltage costituisce l'innesco
per l'avvio del motore, dapprima vengono pilotati i rel??? in base al verso di rotazione richiesto
(deducibile dal segno di motorCmd.v_tgt, maggiore di zero -> orario), atteso un tempo
RELE_DELAY * 10msec, viene attivato il driver del triac motore (cfr. documentazione Task).

Solo dopo aver rilevato il segnale della tachimetrica, viene abilitata la gestione delle rampe.

Rampe:

rilevata una variazione della velocit??? target, il tempo di rampa, espresso in multipli di 100ms,
viene convertito in unit??? di tempo interna (STATE_MACHINE_MOTOR_PERIOD*10ms=40 msec),
ci??? permette la generazione di un riferimento di velocit??? target che varia linearmente
nel tempo fino al raggiungimento della velocit??? finale.

v_rampa = v_finale - v_correzione;
v_correzione = (v_delta/#_step_rampa)*(#_step_rimanenti)
v_delta=v_finale-v_iniziale

Il tempo di rampa max 65535 * 100ms

Parametri da editor:
rpm_min:     velocit??? target minima da fermo
rele_delay:  ritardo attivazione rele

Misura di sbilanciamento
Ogni volta che varia il target sia di velocit??? che di tempo, eccetto durante la soft start,
la misura dello sbilanciamento viene sospesa. finch??? il nuovo target non ??? stato raggiunto
la misura non viene riattivata, ma il dato non ??? disponibile prima di aver completato un
numero di giri cesto specificato in editor.

\author 	Roberto Fioravanti
\date		17/03/2006
\version	1.0.0

\bug

\todo

\note		nel caso venga individuato un comando di inversione del senso di rotazione
            del motore, questo viene prima fermato e poi riavviato secondo il nuovo verso.

17/03/2006 Roberto Fioravanti
*/




void PM_UM_MotorManager (void)
{
    static uint8_t sm_mot_period = 0;
    uint32_t tacho;
    uint8_t	newtach=TACH_VALUE_KO;

    PM_UM_ZC_Monitor();

    ++sm_mot_period;
    sm_mot_period %= STATE_MACHINE_MOTOR_PERIOD;

    /* read tach period */
    newtach=PM_UM_Tacho_GetNewValue(&tacho, !(PM_UM_Cfg.cfg & 1)?TRUE:FALSE);


    PM_UM_CtrlVar.tach.curr_meas = tacho;


    if(  newtach == TACH_VALUE_OK )
    {

    	SAFETY_REGION_START();
		SAFETY_DATA_CHECK(PM_UM_Sft);

		/* reset timeouts */
		PM_UM_Sft.cnt_no_tach = NO_TACH_TIMEOUT;
		SAFETY_DATA_UPDATE(PM_UM_Sft);
		SAFETY_REGION_END();

    	if(PM_UM_IsWaiting4Tach())
    	{
    		sm_mot_period=0;
    	}

    	if( tacho >= MAX_TACH_VALUE_TICK )
		{
			/* Let's assume motor stop */
			motionlessAssumption();
		}
		else
		{
			PM_UM_CtrlVar.tach.curr_meas = tacho;

			if( (sm_mot_period == 0) || (!(PM_UM_Cfg.cfg & 1)))
			{
				uint16_t tmp_new_rpm= convTickInRPM(tacho);

				if (tmp_new_rpm != 0xffff)
				{
					/* converti tach in rpm */
					PM_UM_CtrlVar.tach.curr_rpm = tmp_new_rpm;
				}
			}
		}
    }
    else
    {
		SAFETY_REGION_START();
		SAFETY_DATA_CHECK(PM_UM_Sft);

		if( PM_UM_Sft.cnt_no_tach == 0 )
		{
			motionlessAssumption();
		}
		else
		{
			if( sm_mot_period == 0 )
			{
				/* update no tach remaining timeout*/
				PM_UM_Sft.cnt_no_tach--;
			}
		}
		/* tach not available */


		SAFETY_DATA_UPDATE(PM_UM_Sft);
		SAFETY_REGION_END();
    }

    if( sm_mot_period != 0 )
	{
		return;
	}


    SAFETY_REGION_START();
    SAFETY_DATA_CHECK(PM_UM_Cfg);

    if( !(PM_UM_Cfg.cfg & 1)  )
    {
		/* ensure reading high frequencies without motor */
    	PM_UM_Tacho_SetMode(PM_UM_TACHO_CONTINUOUS);
    	PM_UM_Tacho_Enable();
    	SAFETY_REGION_END();
        return;
    }

    SAFETY_REGION_END();

    newtach=PM_UM_Tacho_GetNewValue(&tacho, TRUE);

    if(  newtach == TACH_VALUE_OK )
    {
        /* misura disponibile */
    	SAFETY_REGION_START();
    	SAFETY_DATA_CHECK(PM_UM_Sft);

    	/* reset timeouts */
    	PM_UM_Sft.cnt_no_tach = NO_TACH_TIMEOUT;

        if(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO)
        {
            /* tach is back */
            SetStatus( MOVIM_ON );
        }
        /**
        gestione segnalazione mancanza tacho durante fase On Motore
        troppo a lungo
        */
        if( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON )
        {
        	PM_UM_Sft.noTachoInEnginePhaseOn_Cntr = 0;
        	PM_UM_Sft.tachFlags &= ~1;
        }

        SAFETY_DATA_UPDATE(PM_UM_Sft);
        SAFETY_REGION_END();

        /* abilita tachimetrica per la misura di un nuovo periodo */    
        if( tacho > CONTINUE_MODE_READ_TACHO )
        {
            PM_UM_Tacho_SetMode(PM_UM_TACHO_CONTINUOUS);

        }else
        {
            PM_UM_Tacho_SetMode(PM_UM_TACHO_ONE_SHOT);
        }

        PM_UM_Tacho_Enable();

        if( !(tacho >= MAX_TACH_VALUE_TICK ))
        {
            /* calcolo sbilanciamento */
            calculateUnbalance();

            /* calcolo sforzo ?*/
        }
    }
    else
    {
    	SAFETY_REGION_START();
    	SAFETY_DATA_CHECK(PM_UM_Sft);

    	/* tach not available */
    	if(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON)
		{
			/* same logic of soft start */
    		PM_UM_CtrlVar.stateMachine.curr_phase = MOVIM_ON_WITHOUT_TACHO;
		}

        /* misura non disponibile */       
        if( PM_UM_Sft.cnt_no_tach == 0 )
        {
        	/* force tach disabled */
        	PM_UM_Tacho_Disable();
            /* Enable tach in countinuous mode */
            PM_UM_Tacho_SetMode(PM_UM_TACHO_CONTINUOUS);
            PM_UM_Tacho_Enable();

            if(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO)
            {
            	PM_UM_Sft.cnt_no_tach = NO_TACH_TIMEOUT;
            }

            PM_UM_Tacho_SetNewValue(14500);

            /**  gestione segnalazione mancanza tacho durante fase On Motore troppo a lungo */
            if(( (PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON)
	        || (PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO) ) && (!PM_UM_IsSuspendedTriac()) && (!PM_UM_IsSuspendedTriacBySafety()))
            {
                if( PM_UM_Sft.noTachoInEnginePhaseOn_Cntr < NO_TACH_TIMEOUT_ENGINE_ON )
                {
                	PM_UM_Sft.noTachoInEnginePhaseOn_Cntr ++;
                }
                else
                {
                	PM_UM_Sft.tachFlags |= 1;
                }
            }
            else
            {
            	PM_UM_Sft.noTachoInEnginePhaseOn_Cntr = 0;
            	PM_UM_Sft.tachFlags &= ~1;
            }
        }
        else
        {
        	/* nothing to do */
        }

        SAFETY_DATA_UPDATE(PM_UM_Sft);
        SAFETY_REGION_END();
    }
    /*========================================*/
    /* Controlla problemi con il triac driver */
    /*========================================*/
    if( (      PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_SOFT_START )
    	  || ( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON )
          || ( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO)

      )
    {
        if( !PM_UM_GateDrvIsEnabled() )
        {
            /**
            Problem !!
            */

            PM_UM_ActivateTask();
        }
    }


    /*=============================================*/
    /* Controlla velocita' e verso rotazione motore */
    /*=============================================*/
    if( (( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON ) ||(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO))
            && ( PM_UM_CtrlVar.stateMachine.engineSpin == ANTICLOCKWISE ) && ( PM_UM_CtrlVar.motorCmd.v_tgt < 0 ) )
    {
        /* movimentazione corrente oraria: richiesta movimentazione antioraria */
        SetStatus(MOVIM_PAUSA__AVVIA_SPEGNIMENTO);
    }
    else if(( ( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON )||(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO)) && ( PM_UM_CtrlVar.stateMachine.engineSpin == CLOCKWISE ) && ( PM_UM_CtrlVar.motorCmd.v_tgt > 0 ) )
    {
        /* movimentazione corrente antioraria: richiesta movimentazione oraria */
    	SetStatus(MOVIM_PAUSA__AVVIA_SPEGNIMENTO);
    }
    else
    {
        PM_UM_CtrlVar.stateMachine.v_tgt_abs = abs( PM_UM_CtrlVar.motorCmd.v_tgt );
    }


    if( (( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON )||(PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_ON_WITHOUT_TACHO)) &&
            ( PM_UM_CtrlVar.stateMachine.v_to == 0 ) &&
            ( PM_UM_CtrlVar.stateMachine.stepcnt == 0 ) )
    {
        SetStatus( MOVIM_PAUSA__AVVIA_SPEGNIMENTO );
    }

    /*==================*/
    /* macchina a stati */
    /*==================*/

    switch( PM_UM_CtrlVar.stateMachine.curr_phase )
    {
    case MOVIM_PAUSA__AVVIA_SPEGNIMENTO:
    default:

        /* imposta delay attivazione rel???-triac */
        set_delay_cnt();
        SetStatus( MOVIM_PAUSA__AVVIA_SPEGNIMENTO_WAIT );
        idleUnbalance();
        break;

    case MOVIM_PAUSA__AVVIA_SPEGNIMENTO_WAIT:

        /* attesa per la disattivazione dei rel?? */
        if (PM_UM_CtrlVar.stateMachine.rele_delay_cnt == 0)
        {
            SetStatus( MOVIM_SPEGNI );
        }
        break;

    case MOVIM_SPEGNI:
    case MOVIM_PAUSA__STOP:
        /* Il motore potrebbe non essere fermo !! */

		if((PM_UM_CtrlVar.motorCmd.cmdflags & RELAY_CFG_FOR_DOOR_LOCK_MASK)==0)
		{
			SetRele(ALL_OPEN);
		}else
		{
			SetRele(ALL_CLOSED);
		}
	
        if( (PM_UM_CtrlVar.stateMachine.rele_delay_cnt == 0)  && ( PM_UM_CtrlVar.tach.curr_rpm == 0 ))
        {
            /* attende l'eventuale apertura dei rele e il completo arresto del cesto */
            SetStatus( MOVIM_PAUSA__ATTENDI_RICHIESTA );
        }

        break;

    case MOVIM_PAUSA__ATTENDI_RICHIESTA:

        if( (PM_UM_CtrlVar.stateMachine.v_tgt_abs >= Get_common_rpm_min()) && goodZCSignal() )
        {
            if( PM_UM_CtrlVar.motorCmd.v_tgt > 0 )
            {
                SetRele( ANTICLOCKWISE );
                
            }
            if( PM_UM_CtrlVar.motorCmd.v_tgt < 0 )
            {
                SetRele( CLOCKWISE );
            }
            /* imposta delay attivazione rele-triac */
            set_delay_cnt();
            /* rilevata nuova richiesta */
            SetStatus( MOVIM_PAUSA__ATTENDI_RICHIESTA_WAIT );



            PM_UM_CtrlVar.engineControlFlags.bit.softStartAlphaMin = FALSE;
            PM_UM_CtrlVar.engineControlFlags.bit.softStartAlphaMin_TimeLimit = FALSE;

            SAFETY_REGION_START();
            SAFETY_DATA_CHECK(PM_UM_Sft);

            /* reset flag softStartAlphaMin */
            PM_UM_Sft.tachFlags &= ~6;
            /* init softStartAlphaMin_cnt */
            PM_UM_Sft.softStartAlphaMin_cnt=25;

            SAFETY_DATA_UPDATE(PM_UM_Sft);
            SAFETY_REGION_END();

        }
        else
        {
            if((PM_UM_CtrlVar.motorCmd.cmdflags & RELAY_CFG_FOR_DOOR_LOCK_MASK)==0)
            {
                SetRele( ALL_OPEN );
            }
            else
            {
               SetRele( ALL_CLOSED );
            }
        }

        break;

    case MOVIM_PAUSA__ATTENDI_RICHIESTA_WAIT:

        if( (PM_UM_CtrlVar.stateMachine.rele_delay_cnt == 0) 
#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
			&& (PM_UM_Sft.bSoftStartSuspend == 0)
#endif /*defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)*/
			)
        {
            /* l'istruzione sotto serve per il Task_UniversalMotorSupervisor */
            SetStatus(MOVIM_SOFT_START);
            PM_UM_ActivateTask();
        }

#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
		/* prj ark2 norm  : CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START not enabled.
		the following statements could have dangerous effect 
		*/
        if(PM_UM_CtrlVar.motorCmd.v_tgt == 0)
		{
			SetStatus( MOVIM_PAUSA__AVVIA_SPEGNIMENTO );
		}
#endif /*defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)*/
        break;

    case MOVIM_SOFT_START:

    	if(  newtach == TACH_VALUE_OK )
        {
            /* se tachimetrica disponibile */
            PM_UM_CtrlVar.stateMachine.stepcnt 	= 0;
            PM_UM_CtrlVar.stateMachine.deltav 	= 0;
            PM_UM_CtrlVar.target_rpm 			= Get_common_rpm_min();

            PM_UM_CtrlVar.motorflags|=MOTOR_FLAGS_LAST_TARGET_UP_BIT;

            /* go on !! */
            SetStatus( MOVIM_ON );
        }
        else
        {
        	SAFETY_REGION_START();
        	SAFETY_DATA_CHECK(PM_UM_Sft);

            /**
            Se il controllo ha raggiunto l'angolo minimo in Soft Start
            la macchina a stati si riporta in sicurezza
            */
            if( PM_UM_Sft.tachFlags & 2 )
            {
                if(!PM_UM_Sft.softStartAlphaMin_cnt)
                {
                    PM_UM_CtrlVar.motorCmd.v_tgt = 0;
                    SetStatus( MOVIM_PAUSA__AVVIA_SPEGNIMENTO );
                    PM_UM_Sft.tachFlags |= 4;
                }
                else
                {
                    /* dec conuter */
                	PM_UM_Sft.softStartAlphaMin_cnt--;
                }
            }

            if(PM_UM_CtrlVar.motorCmd.v_tgt == 0)
			{
				SetStatus( MOVIM_PAUSA__AVVIA_SPEGNIMENTO );
			}

            SAFETY_DATA_UPDATE(PM_UM_Sft);
            SAFETY_REGION_END();

            break;
        }
        /* -fallthrough  */
    case MOVIM_ON_WITHOUT_TACHO:
    case MOVIM_ON:

        if((PM_UM_CtrlVar.stateMachine.v_tgt_abs != PM_UM_CtrlVar.stateMachine.v_to) || (PM_UM_CtrlVar.stateMachine.t_tgt != PM_UM_CtrlVar.motorCmd.t_tgt))
        {
            /* target finale rampa e' cambiato */

            PM_UM_CtrlVar.motorflags &= ( ~(1<<MOTOR_FLAGS_LAST_TARGET_UP));
            if(PM_UM_CtrlVar.stateMachine.v_tgt_abs > PM_UM_CtrlVar.target_rpm)
            {
                PM_UM_CtrlVar.motorflags |= (1<<MOTOR_FLAGS_LAST_TARGET_UP);
            }

            /* gestione rampa */
            PM_UM_CtrlVar.stateMachine.cmdflags = PM_UM_CtrlVar.motorCmd.cmdflags;
            PM_UM_CtrlVar.stateMachine.t_tgt    = PM_UM_CtrlVar.motorCmd.t_tgt;
            PM_UM_CtrlVar.stateMachine.step     =((uint32_t)(PM_UM_CtrlVar.stateMachine.t_tgt*10))/STATE_MACHINE_MOTOR_PERIOD;
            PM_UM_CtrlVar.stateMachine.stepcnt  = PM_UM_CtrlVar.stateMachine.step;
            PM_UM_CtrlVar.stateMachine.v_to     = PM_UM_CtrlVar.stateMachine.v_tgt_abs;

            PM_UM_CtrlVar.unbalance.gripFlag=0;

            /* ribilanciamento */
            if((PM_UM_CtrlVar.stateMachine.v_tgt_abs < PM_UM_CtrlVar.target_rpm ) && (PM_UM_CtrlVar.stateMachine.cmdflags & CMDFLAGS_REBALANCE_MASK))
            {
                PM_UM_CtrlVar.unbalance.gripFlag=1;
            }

            /**/
            PM_UM_CtrlVar.stateMachine.deltav=(int16_t)PM_UM_CtrlVar.stateMachine.v_tgt_abs - PM_UM_CtrlVar.target_rpm;

            /* suspend unbalance */
            idleUnbalance();
        }

        if(PM_UM_CtrlVar.stateMachine.stepcnt && PM_UM_CtrlVar.stateMachine.step)
        {
            PM_UM_CtrlVar.stateMachine.stepcnt--;
            PM_UM_CtrlVar.stateMachine.diffv=(int16_t)(((int32_t)PM_UM_CtrlVar.stateMachine.deltav*(int32_t)PM_UM_CtrlVar.stateMachine.stepcnt)/(signed long)(PM_UM_CtrlVar.stateMachine.step));
        }
        else
        {
            /* reset rebalance flags */
            PM_UM_CtrlVar.stateMachine.cmdflags &= ~(CMDFLAGS_REBALANCE_MASK);
            PM_UM_CtrlVar.stateMachine.diffv=0;

            /* gestione misura sbilanciamento: riattivazione misura */

            if(PM_UM_CtrlVar.stateMachine.step)
            {
                /* ramp with time != 0 completed */
                startUnbalance();
            }
            else
            {
                if(
                    (((PM_UM_CtrlVar.motorflags & (1<<MOTOR_FLAGS_LAST_TARGET_UP))!=0)&&(PM_UM_CtrlVar.tach.curr_rpm >= PM_UM_CtrlVar.stateMachine.v_to))||
                    (((PM_UM_CtrlVar.motorflags & (1<<MOTOR_FLAGS_LAST_TARGET_UP))==0)&&(PM_UM_CtrlVar.tach.curr_rpm <= PM_UM_CtrlVar.stateMachine.v_to))
                )
                {
                    /* ramp with time = 0 completed*/
                    startUnbalance();
                }
            }
        }

        PM_UM_CtrlVar.target_rpm = PM_UM_CtrlVar.stateMachine.v_to + (uint16_t)(-PM_UM_CtrlVar.stateMachine.diffv);

        break;
    }


    /* impostazione motorflags target raggiunto */
    if((PM_UM_CtrlVar.motorflags & (1<<MOTOR_FLAGS_LAST_TARGET_UP)) && (PM_UM_CtrlVar.tach.curr_rpm >= PM_UM_CtrlVar.stateMachine.v_tgt_abs))
    {
        PM_UM_CtrlVar.motorflags |= (1<<MOTOR_FLAGS_LAST_TARGET_REACHED);
    }
    else if(((PM_UM_CtrlVar.motorflags & (1<<MOTOR_FLAGS_LAST_TARGET_UP))==0) && (PM_UM_CtrlVar.tach.curr_rpm <= PM_UM_CtrlVar.stateMachine.v_tgt_abs))
    {
        PM_UM_CtrlVar.motorflags|=(1<<MOTOR_FLAGS_LAST_TARGET_REACHED);
    }

    /* impostazione motorflags verso velocita'*/
    if(PM_UM_CtrlVar.stateMachine.engineSpin == CLOCKWISE)
    {
        PM_UM_CtrlVar.motorflags |= (MOTOR_FLAGS_SPEED_SIGN_BIT);
    }
    else if(PM_UM_CtrlVar.stateMachine.engineSpin == ANTICLOCKWISE)
    {
        PM_UM_CtrlVar.motorflags &= ~(MOTOR_FLAGS_SPEED_SIGN_BIT);
    }


    /* wait */
    if(PM_UM_CtrlVar.stateMachine.rele_delay_cnt)
    {
        PM_UM_CtrlVar.stateMachine.rele_delay_cnt--;
    }
    setEngineRelays();

}

bool_t PM_UM_IsWaiting4Tach( void )
{
	return (bool_t)(PM_UM_CtrlVar.stateMachine.curr_phase==MOVIM_SOFT_START);
}

void PM_UM_OverrideTriacSuspend(bool8_t bNewSuspend)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_UM_Sft);

	PM_UM_Sft.bSuspendTriac = bNewSuspend;

	SAFETY_DATA_UPDATE(PM_UM_Sft);
	SAFETY_REGION_END();

}


#if defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)
/*
 control triac activation at soft start
 \param [in]  bNewSuspend TRUE suspend triac at soft start ; FALSE do not suspend triac activation
*/
void PM_UM_SoftStartSuspend(bool8_t bNewSuspend)
{
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_UM_Sft);

	PM_UM_Sft.bSoftStartSuspend = bNewSuspend;

	SAFETY_DATA_UPDATE(PM_UM_Sft);
	SAFETY_REGION_END();

}
#endif /*defined(CONFIG_PM_UM_SUSPEND_TRIAC_AT_SOFT_START)*/
/* ************** */
/* Local Function */
/* ************** */



static void SetRele( uint8_t val )
{
	PM_UM_CtrlVar.stateMachine.engineSpin = val;
}

static void SetStatus( uint8_t val )
{
	PM_UM_CtrlVar.stateMachine.curr_phase = val;
}



/** Fissa il valore di alpha_n (calcolato dal modulo controllo motore) da passare al timer
per l'attivazione del gate del triac che controlla il motore

\param alpha da supervisore_controllo_motore()
\return -
*/
static void set_alpha_n( uint16_t alpha )
{
    PM_UM_CtrlVar.alpha_n = alpha;
}

/** Reset delle statistiche relative alla tachimetrica
17/03/2006 Roberto Fioravanti
*/
void resetTachStatistics(void)
{
    PM_UM_CtrlVar.unbalance.max_tick = 0;
    PM_UM_CtrlVar.unbalance.min_tick = MAX_TACH_VALUE_TICK;
}

/** reset current tach measure and drum speed
*/
void motionlessAssumption(void)
{
    PM_UM_CtrlVar.tach.curr_rpm = 0;
    PM_UM_CtrlVar.tach.curr_meas = MAX_TACH_VALUE_TICK;
}

uint32_t PM_UM_GetTach(void)
{
	return PM_UM_CtrlVar.tach.curr_meas;
}
/** tacho measure to drum speed conversion

\param [in]  tach.curr_period_tick in us
\param [in]  RTx10
\param [in]  N??? poli tachimetrica

*/
uint16_t convTickInRPM( uint32_t measure )
{
    uint32_t velCesto;
    uint32_t divisor;
    uint32_t rtx10 = Get_common_RTx10();
    
    divisor = ( measure * rtx10 * Get_common_tacho_poles() );

    if( divisor != 0)
    {
        velCesto = (K_CONV_VEL/divisor);
    }else
    {
    	velCesto = 0xFFFF;
    }
    return velCesto;
}

/*
\param [out]	value pointer to result
\retvalue bool_t	TRUE if valid, FALSE else
*/
bool_t PM_UM_getBasketStopped(bool_t *value)
{
	bool_t retVal = TRUE;

	*value = FALSE;
	if( MAX_TACH_VALUE_TICK == PM_UM_CtrlVar.tach.curr_meas )
	{
		*value = TRUE;
	}
	return retVal;
}

/** Calcolo dello sbilanciamento
17/03/2006 Roberto Fioravanti
*/
void calculateUnbalance(void)
{
    switch(PM_UM_CtrlVar.unbalance.init)
    {
    case UNBALANCE_IDLE:
        break;
    case UNBALANCE_START:
    {
        PM_UM_CtrlVar.unbalance.init=UNBALANCE_UPDT_DELTA;
        PM_UM_CtrlVar.unbalance.acc=0;
        PM_UM_CtrlVar.unbalance.acc_cnt=Get_common_unb_avg_sampl_num()+1;
        PM_UM_CtrlVar.unbalance.cnt=0;

        resetTachStatistics();
    }
    /* lint -fallthrough */
    case UNBALANCE_UPDT_DELTA:
    {
        if( PM_UM_CtrlVar.tach.curr_meas > PM_UM_CtrlVar.unbalance.max_tick)
        {
            PM_UM_CtrlVar.unbalance.max_tick = PM_UM_CtrlVar.tach.curr_meas;
        }

        if( PM_UM_CtrlVar.tach.curr_meas < PM_UM_CtrlVar.unbalance.min_tick)
        {
            PM_UM_CtrlVar.unbalance.min_tick = PM_UM_CtrlVar.tach.curr_meas;
        }

        if(PM_UM_CtrlVar.unbalance.cnt)
        {
            PM_UM_CtrlVar.unbalance.cnt--;
            break;
        }
    }

    /* lint -fallthrough */
    case UNBALANCE_UPDT_ACCU:
    {
        PM_UM_CtrlVar.unbalance.cnt=(60000/(STATE_MACHINE_MOTOR_PERIOD*10))/(PM_UM_CtrlVar.target_rpm)+3;

        if(PM_UM_CtrlVar.unbalance.discard)
        {
            /* scarta */
            PM_UM_CtrlVar.unbalance.acc=0;
            PM_UM_CtrlVar.unbalance.discard--;

            if(!PM_UM_CtrlVar.unbalance.discard)
            {
                PM_UM_CtrlVar.unbalance.init = UNBALANCE_START;
            }
            else
            {
                PM_UM_CtrlVar.unbalance.init = UNBALANCE_UPDT_DELTA;
            }
        }
        else
        {
            PM_UM_CtrlVar.unbalance.acc += (PM_UM_CtrlVar.unbalance.max_tick - PM_UM_CtrlVar.unbalance.min_tick);
            PM_UM_CtrlVar.unbalance.acc_cnt--;

            if(!PM_UM_CtrlVar.unbalance.acc_cnt)
            {
                PM_UM_CtrlVar.unbalance.acc *= Get_common_RTx10();
                PM_UM_CtrlVar.unbalance.acc /= Get_common_unb_avg_sampl_num();
                PM_UM_CtrlVar.unbalance.acc /= 128;

                if(PM_UM_CtrlVar.unbalance.acc > 0xFFFF)
                {
                    PM_UM_CtrlVar.unbalance.measure = 0xFFFF;
                }
                else
                {
                    PM_UM_CtrlVar.unbalance.measure = STD_TO_NOMINAL(PM_UM_CtrlVar.unbalance.acc);
                }

                PM_UM_CtrlVar.unbalance.init = UNBALANCE_START;
            }
            else
            {
                PM_UM_CtrlVar.unbalance.init = UNBALANCE_UPDT_DELTA;
            }
        }

        resetTachStatistics();
    }
    break;
    default:
        PM_UM_CtrlVar.unbalance.init=UNBALANCE_IDLE;
    }
}

/** Avvia la misura dello sbilanciamento */
void startUnbalance(void)
{
    if(PM_UM_CtrlVar.unbalance.init == UNBALANCE_IDLE)
    {
        PM_UM_CtrlVar.unbalance.init    = UNBALANCE_START;
        PM_UM_CtrlVar.unbalance.measure = 0;
        PM_UM_CtrlVar.unbalance.discard = Get_common_unb_discard_sampl();
    }
}

void idleUnbalance(void)
{
    PM_UM_CtrlVar.unbalance.init    = UNBALANCE_IDLE;
    PM_UM_CtrlVar.unbalance.measure = 0;
}

uint16_t PM_UM_GetUnbalance(void)
{
    return PM_UM_CtrlVar.unbalance.measure;
}
/**
Procedura gestione Relays Orario/Antiorario del Motore + apertura Relay 1/2 campo se presente
Procedura gestione del flag per il controllo del blocco porta.

\param [in]  stateMachine.engineSpin (static): verso desiderato relays
\param [in]  stateHFrelay (static): macchina stati per la gestione del 1/2 campo

01/05/2006 Stefano Ughi
*/

uint8_t delayComBPPilot = 0;
static void setEngineRelays( void )
{
	uint8_t pinTriac=0, pinAcw=0, pinCw=0;

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PM_UM_Cfg);
	
   
	pinTriac = PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_TRIAC);
	pinAcw = PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_ACW);
	pinCw = PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_CW);
	SAFETY_REGION_END();
    switch( PM_UM_CtrlVar.stateMachine.engineSpin )
    {

    case ALL_OPEN:
    default:

        if( PM_UM_CtrlVar.engineControlFlags.bit.halfField == TRUE )
        {
            stateHFrelay = RELAY_HF_OFF_TRIAC_ON;
            LIB_DIO_SET_LOAD(OFF, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));
            PM_UM_CtrlVar.stateMachine.flags&=(~1);           /* segnalazione mezzo campo non attivo */
        }

        LIB_DIO_SET_LOAD(OFF, pinCw);
        LIB_DIO_SET_LOAD(OFF, pinAcw);

#ifdef CONFIG_PM_DOORLOCK
        if( delayComBPPilot )
        {
            if(PM_UM_CtrlVar.stateMachine.v_tgt_abs < Get_common_rpm_min() && PM_UM_CtrlVar.stateMachine.v_tgt_abs>0)
            {
            	PM_DoorLock_SetBPPilot(OFF);
            }
            else
            {
                /* comunica al modulo blocco porta che pu??? pilotare il componente */
            	PM_DoorLock_SetBPPilot(ON);
            }
        }
        else
        {
            /* assicura che i relays siano effettivamente aperti prima della comunicazione */
            delayComBPPilot = 1;
        }
#endif

        break;

    case CLOCKWISE:

        if(delayComBPPilot >0)
        {
            delayComBPPilot--;
#ifdef CONFIG_PM_DOORLOCK	    
            PM_DoorLock_SetBPPilot(OFF);
#endif /* CONFIG_PM_DOORLOCK */
        }
        else
        {
            LIB_DIO_SET_LOAD(OFF, pinAcw);
            LIB_DIO_SET_LOAD(ON,  pinCw);
        }

        (void)OCDIO_SetPinDirValueSync(pinTriac,0);

        break;

    case ANTICLOCKWISE:

        if(delayComBPPilot >0)
        {
            delayComBPPilot--;

#ifdef CONFIG_PM_DOORLOCK
            PM_DoorLock_SetBPPilot(OFF);
#endif /* CONFIG_PM_DOORLOCK */

        }
        else
        {
            LIB_DIO_SET_LOAD(ON,  pinAcw);
            LIB_DIO_SET_LOAD(OFF, pinCw);
        }

        (void)OCDIO_SetPinDirValueSync(pinTriac,0);
        break;

    case ALL_CLOSED:

        if( PM_UM_CtrlVar.engineControlFlags.bit.halfField == TRUE )
        {
            stateHFrelay = RELAY_HF_OFF_TRIAC_ON;
            LIB_DIO_SET_LOAD(OFF, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));
            PM_UM_CtrlVar.stateMachine.flags&=(~1);           /* segnalazione mezzo campo non attivo */
        }

        LIB_DIO_SET_LOAD(ON, pinCw);
        LIB_DIO_SET_LOAD(ON, pinAcw);

#ifdef CONFIG_PM_DOORLOCK
        if( delayComBPPilot )
        {
            if(PM_UM_CtrlVar.stateMachine.v_tgt_abs < Get_common_rpm_min() && PM_UM_CtrlVar.stateMachine.v_tgt_abs>0)
            {
            	PM_DoorLock_SetBPPilot(OFF);
            }
            else
            {
                /* comunica al modulo blocco porta che pu??? pilotare il componente */
            	PM_DoorLock_SetBPPilot(ON);
            }
        }
        else
        {
            /*
            assicura che i relays siano effettivamente aperti
            prima della comunicazione
            */
            delayComBPPilot = 1;
        }
#endif /* CONFIG_PM_DOORLOCK */

        break;

    }
}


uint16_t PM_UM_GetSecureAlpha(void)
{
	uint16_t retval=0;
	SAFETY_REGION_START();
	SAFETY_DATA_CHECK(PhaseAngle);
	/* no need to check PhaseAngle.valid because if @ FALSE PhaseAngle.value evals 0*/
	retval = PhaseAngle.value;
	SAFETY_REGION_END();
	return retval;
}

/**
Calcolo dell'angolo di fase da applicare al motore e attivazione del driver per il pilotaggio del triac.

Due sono gli stati possibili all'interno di questo Task

SoftStart (transitorio):

durante la fase di soft start l'angolo di attivazione del triac, alpha_n, viene decrementato a partire da
un valore iniziale alpha_ini (parametro da setting) di uno step fisso alpha_step (parametro da setting)
sino ad un valore minimo alpha_min (dipendente dalla V_line), se la macchina a stati, 
rileva la tachimetrica commuta lo stato del task a On.
Il raggiungimento dell'angolo minimo in fase di soft start viene comunicato mettendo a TRUE il flag
engineControlFlags.bit.softStartAlphaMin

On (regime):
    controllo per l'attivazione del rel??? mezzo-campo.
    applicazione algoritmo di controllo:
        se la velocit??? corrente ??? esterno all'intervallo centrato sulla velocit??? target
        applica la varizione a step
        altrimenti applica il controllore
    filtraggio dell'angolo alpha:
        limitazione sul valore massimo e minimo.
        limitazione sulla variazione in valore assoluto rispetto al valore precedente


* Parametri embedded nel fw o nel file di setting in base alla definizione di CONFIG_PARAM_FROM_FIRMWARE
-estremo inf e sup dell'intervallo centrato sulla vtarget.
-massima variazione dell'angolo alpha
-angolo massimo applicabile

* Parametri embedded nel fw
alpha_ini_soft_start:           angolo di soft start iniziale
alpha_step_soft_start:          massima variazione angolo di soft start.
calcolo angolo minimo di soft start applicabile:
alpha_max:                      massimo valore angolo applicabile [tick]
OutOfRangeParams:               parametri out of range
ControlParam:                   paametri controllo
formule:                        mezzo campo, phase, antischiuma.

\author Roberto Fioravanti Stefano Ughi

\date 	16/03/2006


\note	durata max task 2.3 msec
		10/10/2006 introdotto controllo, in fase di SOFT START, caso
		filterParams.minAlpha > filterParams.maxAlpha

*/
#define TAU_N_1_INI		(29000)
 
static outControllore_t         outController;
static outFiltersAlpha_t        outFiltersAlpha;
static outOfRangeParams_t       outOfRangeParams;
static inControlloreVariables_t inControlloreVariables;
static inControlloreData_t      inControlloreData;

static void PM_UM_Task(void)
{
    uint16_t	Vcurrent;
#ifdef CONFIG_PM_UM_MOTOR_MEASURE
    PM_UM_Meas_t	measID;
#endif

    while ( 1 )
    {
        OS_Suspend ( NULL );

        /* Fix Voltage Frequency ----------------------------*/
        PM_UM_CtrlVar.engineControlFlags.bit.vFreq = HZ_60;
        if( LIB_ZC_GetNominalFrequency() == NOM_FREQ_IS_50_HZ )
        {
            PM_UM_CtrlVar.engineControlFlags.bit.vFreq = HZ_50;
        }

        /* default value for timer configuration ------------*/

        PM_UM_CtrlVar.engineControlFlags.bit.triacState = FALSE;

        switch(PM_UM_CtrlVar.stateMachine.curr_phase)
        {
        case MOVIM_SOFT_START:

            if( PM_UM_CtrlVar.alpha_n == 0 )
            {
                /* Inizializzazioni */
#ifdef CONFIG_PM_UM_MOTOR_MEASURE
            	for (measID = 0; measID < PM_UM_NUM_MEASURES; measID++)
            	{
            		engineMeasureInit(measID);
            	}
#endif /* CONFIG_PM_UM_MOTOR_MEASURE */

                /* set alpha_min_soft_start ----------------------------*/
                PM_UM_CtrlVar.filterParams.minAlpha = Get_common_alpha_min();
		
                /* set alpha_n -----------------------------------------*/
                outController.alpha = Get_common_alpha_ini_soft_start();

                /* set step soft start ---------------------------------*/
                PM_UM_CtrlVar.filterParams.maxDev = Get_common_alpha_step_soft_start();

                /* set alpha_max_soft_start */
                PM_UM_CtrlVar.filterParams.maxAlpha = NOMINAL_TO_STD(outController.alpha);

                if( PM_UM_CtrlVar.filterParams.minAlpha > PM_UM_CtrlVar.filterParams.maxAlpha )
                {
                    /* problem !! */
                    PM_UM_CtrlVar.filterParams.minAlpha = PM_UM_CtrlVar.filterParams.maxAlpha;
                }
                alpha_n_1=outController.alpha;
                PM_UM_CtrlVar.alpha_n_1 = NOMINAL_TO_STD(outController.alpha);	/* Assunzione !! */

            }
            else
            {
        case MOVIM_ON_WITHOUT_TACHO:
                PM_UM_CtrlVar.filterParams.maxDev = Get_common_alpha_step_soft_start();
                outController.alpha = alpha_n_1 - STD_TO_NOMINAL(Get_common_alpha_step_soft_start());
            }

            inControlloreVariables.tau_n 	= TAU_N_1_INI;
            outController.error 			= CONTROLLORE_NO_ERROR;

            PM_UM_CtrlVar.engineControlFlags.bit.alphaStep = TRUE;

            break;

        case MOVIM_ON:
		{
			uint8_t bHF = FALSE;
			bHF = (checkIfActHalfFieldRelay() == TRUE)?1:0;


			/* check if disabilitare uscita triac per attivazione rel??? m.c. */
			if( bHF )
			{
				PM_UM_CtrlVar.engineControlFlags.bit.triacState = TRUE;
			}
			else
			{
				PM_UM_CtrlVar.engineControlFlags.bit.triacState = FALSE;
			}

			/* TODO: da valutare se inizializzare una volta sola all'interno di ogni stato */
			PM_UM_CtrlVar.filterParams.minAlpha = ALPHA_MIN;
			PM_UM_CtrlVar.filterParams.maxAlpha = Get_common_alpha_max();

			inControlloreVariables.tau_n = (uint16_t)STD_TO_NOMINAL(PM_UM_Tacho_GetValue()); 		/* getting tach measure*/

			inControlloreVariables.vTarget 	= PM_UM_CtrlVar.target_rpm;      			/* vel target */
			Vcurrent						= PM_UM_CtrlVar.tach.curr_rpm;           	/* vel corrente rpm */

			getOffsetSpeedRangeFromSetting();
			getOutOfRangeParamsFromSetting(&outOfRangeParams);
#ifdef CONFIG_PM_UM_TRACE_ENABLE
			SetTraceTacho(inControlloreVariables.tau_n);/*trace*/
#endif
			/* default value: P.D. attivo */
			PM_UM_CtrlVar.engineControlFlags.bit.alphaStep = FALSE;

			if( Vcurrent > inControlloreVariables.vTarget )
			{
				if( outOfRangeParams.ratioVelTarget_up != 0 )
				{
					if( Vcurrent > ( inControlloreVariables.vTarget + inControlloreVariables.vTarget/outOfRangeParams.ratioVelTarget_up ) )
					{
						outController.alpha = alpha_n_1 + (outOfRangeParams.stepOutOfRange_up);
						outController.error = CONTROLLORE_NO_ERROR;

						PM_UM_CtrlVar.filterParams.maxDev = NOMINAL_TO_STD(outOfRangeParams.stepOutOfRange_up);
						PM_UM_CtrlVar.engineControlFlags.bit.alphaStep = TRUE;
					}
				}
			}
			else if( Vcurrent < inControlloreVariables.vTarget )
			{
				if( outOfRangeParams.ratioVelTarget_dn != 0 )
				{
					if( Vcurrent < ( inControlloreVariables.vTarget - inControlloreVariables.vTarget/outOfRangeParams.ratioVelTarget_dn ) )
					{
						outController.alpha = alpha_n_1 - (outOfRangeParams.stepOutOfRange_dn);
						outController.error = CONTROLLORE_NO_ERROR;

						PM_UM_CtrlVar.filterParams.maxDev = NOMINAL_TO_STD(outOfRangeParams.stepOutOfRange_dn);

						PM_UM_CtrlVar.engineControlFlags.bit.alphaStep = TRUE;
					}
				}
			}
			else
			{
				/* do nothing */
			}
		}
            break;
        default:
				/* Terminate ISR Fire Triac */
			set_alpha_n(0); /* ferma il motore */
			PM_UM_GateDrv_Disable();
			PM_UM_GateDrv_SetAlpha(0, 0);
#ifdef CONFIG_PM_UM_MOTOR_MEASURE	    
			/* Terminate all engine measures */
			for (measID = 0; measID < PM_UM_NUM_MEASURES; measID++)
			{
				switchOffEngineMeasure(measID);
			}
#endif /* CONFIG_PM_UM_MOTOR_MEASURE */

			SAFETY_REGION_START();
			SAFETY_DATA_CHECK(PhaseAngle);

			PhaseAngle.value=0;
			PhaseAngle.valid=TRUE;

			SAFETY_DATA_UPDATE(PhaseAngle);
			SAFETY_REGION_END();

            continue;
            break;
        }
        /* -----------------------------------------------------------------------------------*/
        if( (PM_UM_CtrlVar.engineControlFlags.bit.alphaStep == FALSE) )
        {
            getEngineControlParamFromSetting(&inControlloreData);
            inControlloreVariables.alpha_n_1 =  alpha_n_1;
            inControlloreVariables.tau_n_1 = tau_n_1;
            controllore( inControlloreData, inControlloreVariables, &outController);
        }
        else
        {
        	/* nothing to do */
        }
        if( (PM_UM_CtrlVar.engineControlFlags.bit.triacState == FALSE) &&  !(PM_UM_ZC.bSuspendTriac || PM_UM_Sft.bSuspendTriac))
        {
			PM_UM_CtrlVar.eOcCfg=PM_UM_TRIAC_MOTOR_OC_HIGH;
		}
		else
		{
			PM_UM_CtrlVar.eOcCfg=PM_UM_TRIAC_MOTOR_OC_LOW;
		}


        if(PM_UM_ZC.bSuspendTriac || PM_UM_Sft.bSuspendTriac)
        {
        	outController.alpha = alpha_n_1;
        }

#if defined(CONFIG_PM_UM_TRACE_ENABLE)
     	SetTraceAlpha(outController.alpha);/*trace 31*/

#endif
        filterNewAlpha( outController, &PM_UM_CtrlVar.filterParams, alpha_n_1, &outFiltersAlpha );
#ifdef CONFIG_PM_UM_TRACE_ENABLE
        SetTraceError((unsigned char)(((unsigned char)outController.error))|((unsigned char)outFiltersAlpha.boundary<<4)); /*21*/
        SetTraceAlphaFiltered(STD_TO_NOMINAL((uint32_t)outFiltersAlpha.alpha));/*trace 11*/
#endif
        /* ribilanciamento, attende che la velocit?? corrente sia inferiore al target maggiorato */
        if((PM_UM_CtrlVar.tach.curr_rpm <= (PM_UM_CtrlVar.stateMachine.v_tgt_abs + PM_UM_UNBALANCE_DELTA_SPEED)) && (PM_UM_CtrlVar.unbalance.gripFlag))
        {
            PM_UM_CtrlVar.unbalance.gripFlag--;

            outFiltersAlpha.alpha += PM_UM_UNBALANCE_DELTA_ALPHA;
            outController.alpha = STD_TO_NOMINAL(outFiltersAlpha.alpha);
        }

        set_alpha_n(outFiltersAlpha.alpha);
        
        SAFETY_REGION_START();
        SAFETY_DATA_CHECK(PhaseAngle);

        if(outFiltersAlpha.alpha < 10000)
        {
        	PhaseAngle.value = outFiltersAlpha.alpha;
        	PhaseAngle.valid=TRUE;
        }
        else
        {
        	PhaseAngle.value=0;
        	PhaseAngle.valid=FALSE;
        }
        SAFETY_DATA_UPDATE(PhaseAngle);
        SAFETY_REGION_END();




        PM_UM_GateDrv_SetAlpha(PM_UM_CtrlVar.alpha_n, (PM_UM_CtrlVar.eOcCfg==PM_UM_TRIAC_MOTOR_OC_HIGH));

#ifdef CONFIG_PM_UM_MOTOR_MEASURE
      	for (measID = 0; measID < PM_UM_NUM_MEASURES; measID++)
      	{
      		engineMeasures(measID);
      	}
#endif /* CONFIG_PM_UM_MOTOR_MEASURE */

        /* update last value */
        if( (outFiltersAlpha.boundary == FILTER_NEW_ALPHA_ALPHA_IN_RANGE) )
        {
        	alpha_n_1 = outController.alpha;
        }
        else
        {
        	/* convert filtered alpha */
        	alpha_n_1 = STD_TO_NOMINAL(outFiltersAlpha.alpha);
        }

        tau_n_1 = inControlloreVariables.tau_n;


        if( ( PM_UM_CtrlVar.stateMachine.curr_phase == MOVIM_SOFT_START ) &&
                ( outFiltersAlpha.boundary == FILTER_NEW_ALPHA_ALPHA_MIN_VALUE ) )
        {
        	SAFETY_REGION_START();
        	SAFETY_DATA_CHECK(PM_UM_Sft);

            PM_UM_Sft.tachFlags |= 2;

            SAFETY_DATA_UPDATE(PM_UM_Sft);
            SAFETY_REGION_END();
        }
#ifdef CONFIG_PM_UM_TRACE_ENABLE
        CollectTrace();
#endif

    }
}

/**
Se presente mezzo campo gestisce le modalit??? di attivazione del rel??? m.c..
e comunica al task Task_UniversalMotorSupervisor se disattivare/attivare
il triac controllo motore.
Il rel??? di mezzo campo viene attivato se vCurrent > velAttHalfFieldR e
vTarget > velAttHalfFieldR
con la sequenza sotto descritta
alla riattivazione del triac l'angolo di fase corrente viene aumentato di una
quantit??? pari a DeltaAlfa_MFOn ( param da setting )
T1 e T2 sono ugualmente parametri da setting
					 :			:
	   1 ------+T1	 :	 T2		+----------
TRIAC 		   |	 :			|
	   0	   +-----:----------+
	   				 :			:
	   1			 +----------:-----------
RELE'				 |			:
	   0 ------------+			:					(attivazione mc)
					 :			:
					 :			:
	   1 ------------+			:
RELE'				 |			:
	   0   			 +-----------------------		(disattivazione mc)
					 :			:

Il rel??? di m.c. pu??? essere disattivato:
 -in conseguenza di un comando di STOP motore
 -velocit??? target inferiore alla soglia di attivazione del mezzocampo
 -velocit??? corrente inferiore alla met??? dela soglia di attivazione del mezzocampo

\param [in]  engineControlFlags.bit.halfField (TRUE/FALSE)
\param [in]  vTarget  r.p.m.
\param [in]  vCurrent r.p.m.
\param [in]  velAttHalfFieldR r.p.m., velocit??? di attivazione del mezzo campo
			 dipendente dalla Vline
\param [in]  pHalfFieldParam, puntatore alla tabella dati relativa alla gestione
			 del m.c.

\return FALSE/TRUE, TRUE -> disabilita uscita triac

\author Stefano Ughi, Roberto Fioravanti
\date 	20/12/2007

\note

*/
static uint8_t checkIfActHalfFieldRelay( void )
{
    static uint8_t delayAttHalfFieldRelay;

    uint8_t  retValue;
    uint16_t velCurrent;
    uint16_t velTarget;
    static uint16_t velAttHalfFieldR; /* beware of the sign !!! */

    retValue = FALSE;
    if( PM_UM_CtrlVar.engineControlFlags.bit.halfField == TRUE )
    {
        switch ( stateHFrelay )
        {
        case RELAY_HF_OFF_TRIAC_ON:
        default:

            LIB_DIO_SET_LOAD(OFF, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));

            if((set_hf.ext_hf_rpm >= Get_common_rpm_min()) && (set_hf.ext_hf_pwd == EXT_HF_PWD))
            {
                velAttHalfFieldR = set_hf.ext_hf_rpm;
            }
            else
            {
                velAttHalfFieldR = get_velAttHalfFieldR();
            }

            velCurrent 		 = PM_UM_CtrlVar.tach.curr_rpm;
            velTarget		 = PM_UM_CtrlVar.target_rpm;


            if( (( velCurrent > velAttHalfFieldR ) &&
                    ( velTarget > velAttHalfFieldR )) )
            {

                delayAttHalfFieldRelay = GET_DELAY_TRIAC_OFF_VS_HF_ON();


                stateHFrelay = RELAY_HF_OFF_TRIAC_OFF;
                tgt_stateHFrelay = RELAY_HF_ON_TRIAC_ON;

                retValue = TRUE;
            }

            PM_UM_CtrlVar.stateMachine.flags &= (~1);           /* segnalazione mezzo campo non attivo */

            break;

        case RELAY_HF_OFF_TRIAC_OFF:

            LIB_DIO_SET_LOAD(OFF, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));

            delayAttHalfFieldRelay--;

            if( delayAttHalfFieldRelay == 0 )
            {
                if(tgt_stateHFrelay==RELAY_HF_ON_TRIAC_ON)
                {
                    LIB_DIO_SET_LOAD(ON, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));

                    delayAttHalfFieldRelay = GET_DELAY_TRIAC_ON_VS_REL_HF_ON();

                    /* verso l'attivazione del mezzocampo */
                    stateHFrelay = RELAY_HF_ON_TRIAC_OFF;
                }
                else
                {
                    /* verso la disattivazione del mezzocampo */
                    stateHFrelay = RELAY_HF_OFF_TRIAC_ON;
                }
            }
            retValue = TRUE;

            PM_UM_CtrlVar.stateMachine.flags&=(~1);           /* segnalazione mezzo campo non attivo */

            break;

        case RELAY_HF_ON_TRIAC_OFF:

            LIB_DIO_SET_LOAD(ON, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));

            delayAttHalfFieldRelay --;

            if( delayAttHalfFieldRelay == 0 )
            {
                if(tgt_stateHFrelay == RELAY_HF_ON_TRIAC_ON)
                {
                    /* verso l'attivazione del mezzocampo */
                    stateHFrelay = RELAY_HF_ON_TRIAC_ON;
		    
                    /* Correzione angolo di fase  */
                    alpha_n_1 += STD_TO_NOMINAL(GET_DELTA_ALPHA_HF_ON());
                    PM_UM_CtrlVar.stateMachine.flags |= 1;              /* segnalazione mezzo campo attivo */

                }
                else
                {
                    delayAttHalfFieldRelay = GET_DELAY_TRIAC_ON_VS_REL_HF_ON();

                    /* verso la disattivazione del mezzocampo */
                    stateHFrelay = RELAY_HF_OFF_TRIAC_OFF;
		    
                    /* Correzione angolo di fase  */
                    alpha_n_1 -= STD_TO_NOMINAL(GET_DELTA_ALPHA_HF_ON());
                    PM_UM_CtrlVar.stateMachine.flags&=(~1);           /* segnalazione mezzo campo non attivo */
                }
            }
            else
            {
            	PM_UM_CtrlVar.stateMachine.flags |= 1;              /* segnalazione mezzo campo attivo */
            }

            retValue = TRUE;


            break;
        case RELAY_HF_ON_TRIAC_ON:

            LIB_DIO_SET_LOAD(ON, PM_UM_GET_IO_MAP_PIN_FROM_PIN_TABLE(PIN_HF));
	    
            PM_UM_CtrlVar.stateMachine.flags |= 1;              /* segnalazione mezzo campo attivo */
            if((set_hf.ext_hf_rpm >= Get_common_rpm_min()) && (set_hf.ext_hf_pwd == EXT_HF_PWD))
            {
                velAttHalfFieldR=set_hf.ext_hf_rpm;
            }
            else
            {
                velAttHalfFieldR = get_velAttHalfFieldR();
            }

            /*aggiornamento della velocit??? corrente e della velocit??? target */
            velCurrent 		 = PM_UM_CtrlVar.tach.curr_rpm;
            velTarget		 = PM_UM_CtrlVar.target_rpm;

            if(( velCurrent<(velAttHalfFieldR/2) ) || ( velTarget < velAttHalfFieldR) )
            {
                /* disattiva il mezzocampo se la vel corrente diminuisce
                oltre la met??? della soglia di attivazione del mezzocampo o
                se la velocit??? target ??? inferiore alla soglia */

                /* imposta il nuovo stato */
                stateHFrelay = RELAY_HF_ON_TRIAC_OFF;
                tgt_stateHFrelay = RELAY_HF_OFF_TRIAC_ON;

                /*parametri da utilizzare per le transizioni di stato*/
                delayAttHalfFieldRelay = GET_DELAY_TRIAC_OFF_VS_HF_ON();
            }
            break;
        }
    }

    return retValue;
}

/** Ritorna la velocita in r.p.m.  per l'attivazione del mezzo campo.
*	La velocita' di mezzo campo e' una funzione lineare della Vline
*/
static uint16_t get_velAttHalfFieldR( void )
{
    getEngineData_t engineData;

    PM_UM_GetMotorData( PM_UM_MEASURE_MID_FIELD, &engineData );

    if( engineData.error == FALSE )
    {
        return engineData.dato;
    }
    else
    {
        return 0xFFFF;
    }
}

/** Imposta delay attivazione rele'-triac.
*	\param  [in,out] stateMachine.rele_delay_cnt
*/
static void set_delay_cnt( void )
{
    PM_UM_CtrlVar.stateMachine.rele_delay_cnt = Get_common_rele_delay();
}


/**
Ritorna lo stato di pilotaggio del triac.

\param  [in]  stateMachine

\return 	uint8_t

\retval		TRUE,  motore non pilotato
			FALSE, motore pilotato

\warning	none

\author 	Roberto Fioravanti
\date		04/07/2006
\version	x.y.z

*/
uint8_t PM_UM_TriacMotorNotDriven(void)
{
    return (PM_UM_CtrlVar.stateMachine.rele_delay_cnt
            && ((   PM_UM_CtrlVar.stateMachine.curr_phase != MOVIM_ON)
                || (PM_UM_CtrlVar.stateMachine.curr_phase != MOVIM_ON_WITHOUT_TACHO)
                || (PM_UM_CtrlVar.stateMachine.curr_phase != MOVIM_SOFT_START)
               )
           );
}

/**
Ritorna lo stato di pilotaggio dei rel?? orario/antiorario.

\param  [in]  stateMachine

\return 	bool_t

\retval		TRUE,  uno dei due rel?? (orario o antiorario) attivo
		FALSE, nessuno o entrambi i rel?? attivi

\warning	none

\author 	Alessandro Sabbatini
\date		28/01/2014
\version	00.00.01

*/
bool_t PM_UM_RelaysInRunningState(void)
{
	return ((delayComBPPilot == 0)  &&
			((PM_UM_CtrlVar.stateMachine.engineSpin == CLOCKWISE)
			|| (PM_UM_CtrlVar.stateMachine.engineSpin == ANTICLOCKWISE)));
}

PM_UM_RpmType PM_UM_GetCurrentRpmSync(void) 
{
    return PM_UM_CtrlVar.tach.curr_rpm;
}




#if defined(CONFIG_PM_UM_TRACE_ENABLE)
void SetTraceError(unsigned char err)
{
	TraceCollTmp.errors=err;
}

void InitTrace(void)
{
	TraceCnt=0;
	TracePos=0;
	memset(&TraceCollTmp,0,sizeof(TraceCollTmp));
	memset(&TraceCollData,0,sizeof(TraceCollData));
}

void CollectTrace(void)
{
	if(TracePos>=CONFIG_PM_UM_TRACE_DEPTH)
	{
		TracePos=0;
	}

	TraceCollTmp.cnt=TraceCnt;
	TraceCollData[TracePos].errors=TraceCollTmp.errors;
	TraceCollData[TracePos].falpha=TraceCollTmp.falpha;
	TraceCollData[TracePos].alpha=TraceCollTmp.alpha;
	TraceCollData[TracePos].tacho=TraceCollTmp.tacho;
	TraceCollData[TracePos].cnt=TraceCollTmp.cnt;
	TraceCnt++;
	TracePos++;

}

void SetTraceAlphaFiltered(unsigned short falpha)
{
	TraceCollTmp.falpha = falpha;
}

void SetTraceTacho(unsigned short tacho)
{
	TraceCollTmp.tacho = tacho;
}

void SetTraceAlpha(unsigned short alpha)
{
	TraceCollTmp.alpha = alpha;
}
#endif /* defined(CONFIG_PM_UM_TRACE_ENABLE */

void PM_UM_OnZC_OK (void)
{
	PM_UM_ZC.LastZC = OS_GetTime();
	PM_UM_ZC.bSuspendTriac = FALSE;
}

void PM_UM_OnZC_KO (void)
{
	PM_UM_ZC.bSuspendTriac = TRUE;
}


bool_t PM_UM_IsSuspendedTriac(void)
{
	return (bool_t)PM_UM_ZC.bSuspendTriac;
}

bool_t PM_UM_IsSuspendedTriacBySafety(void)
{
	return (bool_t)PM_UM_Sft.bSuspendTriac;
}


static void PM_UM_ZC_Monitor(void)
{
	if( Tools_IsTimerElapsed(&(PM_UM_ZC.LastZC), 25)  )
	{
		PM_UM_ZC.bSuspendTriac = TRUE;
	}
}
#endif /* CONFIG_PM_MOTORGATE */
/*@}*/
/* *********** */
/* End of file */
/* *********** */
