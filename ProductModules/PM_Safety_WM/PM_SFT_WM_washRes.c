/**
\n
\par		PM_SFT_washRes
\n
\n
\file		PM_SFT_WM_washRes.c
\ingroup	PM_SFT_washRes
\date		11/03/2013
\author		Francesco Barletta
\n
*/


/* ******** */
/* Includes */
/* ******** */
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_washRes.h"
#include "PM_ConsistencyExp.h"
#include "LIB_Utilities.h"
#include "LIB_ClassBVar.h"
#include "Lib_ProductSafety.h"
#include "LIB_ZeroCrossingExp.h"
#include "Tools.h"
#include "PM_SFT_WM_NTCProbe.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/**
bit SwitchRelayTestStateType
 */
enum eSwitchRelayTestState_Bit{
	SWITCH_RELAY_TEST_OK_BIT=0,
	SWITCH_RELAY_TEST_WARNING_BIT,
	SWITCH_RELAY_TEST_PREFAULT_BIT,
	SWITCH_RELAY_TEST_FAULT_BIT,
	SWITCH_RELAY_TEST_STEP1_BIT,
	SWITCH_RELAY_TEST_STEP2_BIT,
	SWITCH_RELAY_TEST_STEP3_BIT,
};
/**
bit SwTestReqBmp
 */
enum eSwitchRelayTest_Bit{
	SWITCH_RELAY_TEST_START_BIT=0,
	SWITCH_RELAY_TEST_HALT_BIT,
};

/**
bit SwTestStepResult
 */
enum eSwitchRelayTestStep_Bit{
	SWITCH_RELAY_TEST_STEP1_KO_BIT=0,
	SWITCH_RELAY_TEST_STEP2_KO_BIT,
	SWITCH_RELAY_TEST_STEP3_KO_BIT
};


#define PM_SFT_WM_GET_PREFAULT_TIME_CA(_ptr)			MBGET(2, _ptr, relayCAprefault)
#define PM_SFT_WM_GET_FAULT_TIME_CA(_ptr)				MBGET(1, _ptr, relayCAfault)
#define PM_SFT_WM_GET_FAULT_TIME_CA_OPCODE(_ptr)		MBGET(1, _ptr, relayCAfaultOpcode)
#define PM_SFT_WM_GET_FAULT_TIME_CA_HIGH_TEMP(_ptr)		MBGET(1, _ptr, relayCAfaultHighTemp)

#define PM_SFT_WM_GET_PREFAULT_TIME_CC(_ptr)			MBGET(2, _ptr, relayCCprefault)
#define PM_SFT_WM_GET_FAULT_TIME_CC(_ptr)				MBGET(1, _ptr, relayCCfault)
#define PM_SFT_WM_GET_FAULT_TIME_CC_OPCODE(_ptr)		MBGET(1, _ptr, relayCCfaultOpcode)



#define DEFAULT_TIME_CA_HIGH_TEMP	30 /* 10 = 1 sec */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*
typedef struct {
	uint8_t pos;
	uint8_t status;
	uint8_t statusOld;
}nomeStruct_t;
*/

/**
 *
 */
typedef enum eSwitchRelayTest_Step{
	SWITCH_RELAY_TEST_IDLE=0,
	SWITCH_RELAY_TEST_RESET,
	SWITCH_RELAY_TEST_STEP1,
	SWITCH_RELAY_TEST_STEP2,
	SWITCH_RELAY_TEST_STEP3,
	SWITCH_RELAY_END_OF_SEQ,
	SWITCH_RELAY_TEST_COMPLETED,
}SwitchTestStepType;

typedef uint8_t	SwitchRelayTestReqBmpType;

typedef uint8_t	SwitchRelayTestStateType;

/** WR state bit */
enum eSFT_wrState
{
	/* bitmap corrispondente alle azioni da intraprendere nella gestione
	 * fault resistenza lavaggio
	 */
	PM_SFT_WM_WR_ON_INIT,
	PM_SFT_WM_WR_ON,
	PM_SFT_WM_WR_OFF_INIT,
	PM_SFT_WM_WR_OFF,
	PM_SFT_WM_WR_ZC_OK,
	PM_SFT_WM_WRON_FAULT_RESET,
	PM_SFT_WM_WROFF_FAULT_RESET,
	PM_SFT_WM_PREFAULTRESET_START
};

/** Bit for motor safety event bitmap SFT_MotorEventBmpType */
enum eSFT_WrSftEvent
{
	EVENT_WM_WR_FAULT_RESET,
	EVENT_WM_WROFF_FAULT_MON,
	EVENT_WM_WRON_FAULT_MON,
	EVENT_WM_WR_ZC_OK,
	EVENT_WM_WR_RESET_DELAY,	/* richiede di attivare il delay per passare dal prefualt al resety è attivo */
	EVENT_WM_WR_ON_INIT,
	EVENT_WM_WR_OFF_INIT
};

/**
 * Info related to the control of Washing Res.
 * */
typedef struct {
	uint8_t loadPos;
	uint8_t state;
	uint16_t faultCnt;
	uint8_t prefaultResetCnt;
	uint8_t faultCode;
	uint8_t faultsubCode;
#if 0
	SwitchRelayTestStateType	SwTestState;
	SwitchTestStepType 			SwTestStep;
	SwitchRelayTestReqBmpType	SwTestReqBmp;
	SwitchTestStepType 			SwTestStepResultBmp;
	int8_t						SwTestCycles;
	int8_t						SwTestGoodResult;
	TOOLS_TIMER					SwStepTimer;
	int32_t						SwStepDelayValue;
	int32_t						SwTestKO;
#endif
} WashResFaultStatus_t;

typedef struct WashResFaultParam_s
{
	uint8_t	relayCAprefault1;				/**< CA prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t relayCAprefault0;
	uint8_t relayCAfaultOpcode0;
	uint8_t relayCAfault0;					/**< CA fault x 100 ms - 1 bytes, unsigned	*/
	uint8_t	relayCCprefault1;				/**< CC prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t relayCCprefault0;
	uint8_t relayCCfaultOpcode0;
	uint8_t relayCCfault0;					/**< CA fault x 100 ms - 1 bytes, unsigned	*/
	uint8_t relayCAfaultHighTemp0;
}PM_SFT_WM_WashResFaultParamType;
/* ********* */
/* Variables */
/* ********* */
/* Global */

/* Static */
WashResFaultStatus_t	sftWRstatus;

/* ******************* */
/* Functions Prototype */
/* ******************* */
/* Exported function */

/* Local Function */



/* *************** */
/* Function Bodies */
/* *************** */
/**
\n This function contains inizializations

\param
\return 	tipoRet	Descritpion

\author		Francesco Barletta
\date		11/03/2013
\version

\note

*/
void PM_SFT_WM_washRes_Init(sft_module_status_t *safetyData )
{
	struct Loader faultParam;

	SAFETY_REGION_START();

	/*switch relay procedure init*/
#if 0
	sftWRstatus.SwTestStep=SWITCH_RELAY_TEST_IDLE;
	sftWRstatus.SwTestReqBmp=0;
	sftWRstatus.SwTestGoodResult=0;
#endif

	/* washing resistor position */
	sftWRstatus.loadPos = LIB_IOM_GetLoadPos((uint8_t) LD_WASH_RES_POS);

	SAFETY_DATA_UPDATE( sftWRstatus );
	SAFETY_REGION_END();

	/* Il check sulla struttura dati faultParam viene eseguito una unica volta   */
	if( 0 != BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam))
	{
		safetyData->status = SFT_MODULE_FAULT;
		safetyData->faultSrc = FAULTSRC_WR_BAD_SETTING;
	}
	else
	{
		safetyData->status = SFT_MODULE_IDLE;
		safetyData->faultSrc = NO_FAULTSRC_WR;
	}
}

/**
\n This function implements the state machine which manages safety of the Washing
\n Resistor module.
\n
\n
\param [in,out] newLoads		Info related to loads status request
\param [in,out] safetyData		Info related to the safety status of this module
\param [in] 	applReqFlags	Flags of requests by the application


\return void
\retval	N.A.


\par Architecture Requirements ID:
\li
\n\li

\author		Francesco Barletta
\date		11/03/2013
\version



*/
void PM_SFT_WM_washResMon(iom_loads_struct_t *pNewLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
uint8_t curConsistency = PM_CS_NOT_VALID_CODE;
SFT_wrEventBmpType event = 0;	/* clear bmp*/

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( sftWRstatus );

	/* Temporizzazione macchina a stati per rilevazione guasto su switch relay fatta per mezzo
	 * di down-counter
	 */
	/* Down-counter qui implementato per mezzo di una var statica; la routine in oggetto
	 * dovrebbe essere richiamata con una frequenza di 100Hz (@10ms)
	 */

	if(sftWRstatus.faultCnt)
	{
		sftWRstatus.faultCnt--;
	}
	if(sftWRstatus.prefaultResetCnt)
	{
		sftWRstatus.prefaultResetCnt--;
	}

	if( safetyData->status != SFT_MODULE_FAULT )
	{

		if( goodZCSignal() == TRUE )
		{

		/* Il check dei fault resistenza lavaggio via monitoraggio FB, la loro gestione per mezzo
		 * di una macchina a stati temporizzata, è indipendente dalla gestione fault switch relay
		 * incollato lato Loading, ma ad essa subordinata in quanto ad esito.
		 */
			(void) PM_Consistency_IsValueReady(sftWRstatus.loadPos, &curConsistency);

		}
		/* segnale ZC non presente e/o non valido */
		else
		{
			/* Se la porta é sbloccata e lo stato gestione guasti NON è FAULT(tutti gli altri),
			 * viene considerato come caso OK. */

			curConsistency = ((pNewLoads->loads_req[sftWRstatus.loadPos] != LOAD_REG_OFF) ? WR_CS_ON_OK : WR_CS_OFF_OK);

		}

		/* Analisi Consistency Resistenza Lavaggio */
		switch(curConsistency)
		{

			/* Washing Heater, Stato Indipendente */
			case WR_CS_ON_OK:

				/* Con feedback OK, vengono comunque resettati tutti i guasti della resistenza lavaggio */
				/* Se è attiva la procedura di rilevazione fault su resistenza lavaggio, si avvia il
				* reset della stessa per liberare le risorse impiegate.
				*/
				/* In ogni caso se, dopo l’inizio della rilevazione di un guasto e prima
				* che venga dichiarato il Fault, il feedback torna a segnalare un valore coerente
				* con lo stato, la procedura di rilevazione del guasto viene resettata immediatamente
				* (se ancora non era stato dichiarato il Prefault) o dopo 5 secondi (se era già stato
				* dichiarato il Prefault). Nel caso però di F01.12 oppure 08.01 non c'è un fault time-out:
				* prefault e fault coincidono.
				*/

				if( 0 == (sftWRstatus.state & BFMASK(PM_SFT_WM_WR_ON,1)))
				{
					sftWRstatus.state |= (BFMASK(PM_SFT_WM_WR_ON_INIT,1));
				}
				else
				{
					sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_ON_INIT,1));
				}

				sftWRstatus.state |= BFMASK(PM_SFT_WM_WR_ON,1);
				sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_OFF,1)|BFMASK(PM_SFT_WM_WR_OFF_INIT,1));

				sftWRstatus.state |= (BFMASK(PM_SFT_WM_WRON_FAULT_RESET,1));
				/* event |= (BFMASK(EVENT_WM_WRON_FAULT_RESET,1)); */

			break;

			case WR_CS_OFF_OK:

				/* Nel caso in cui la WR non è pilotata (OFF) ed il FB è OK, si ribadisce un
				 * reset della macchina a stati, per poi andare (dopo 100ms) nello stato IDLE.
				 * In questo modo, tutti gli stati macchina diversi da quello di FAULT ormai
				 * attivo, vengono trattati alla stessa maniera.
				 */
				/* In ogni caso se, dopo l’inizio della rilevazione di un guasto e prima
				 * che venga dichiarato il Fault, il feedback torna a segnalare un valore coerente
				 * con lo stato, la procedura di rilevazione del guasto viene resettata immediatamente
				 * (se ancora non era stato dichiarato il Prefault) o dopo 5 secondi (se era già stato
				 * dichiarato il Prefault). Nel caso però di F01.12 oppure 08.01 non c'è un fault time-out:
				 * prefault e fault coincidono.
				 */

				if( 0 == (sftWRstatus.state & BFMASK(PM_SFT_WM_WR_OFF,1)))
				{
					sftWRstatus.state |= (BFMASK(PM_SFT_WM_WR_OFF_INIT,1));
				}
				else
				{
					sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_OFF_INIT,1));
				}

				sftWRstatus.state |= BFMASK(PM_SFT_WM_WR_OFF,1);
				sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_ON,1)|BFMASK(PM_SFT_WM_WR_ON_INIT,1));

				/* ATTENZIONE: in questo punto viene stabilito il reset dello stato macchina
				 * per mezzo della impostazione dell'evento, gestito poi direttamente in FSM.
				 * E' corretto e coerente che qui venga gestito il filtro dei 5s relativo a
				 * reset prefault */
				sftWRstatus.state |= (BFMASK(PM_SFT_WM_WROFF_FAULT_RESET,1));
				/* event |= (BFMASK(EVENT_WM_WROFF_FAULT_RESET,1)); */


			break;

			/* Washing Heater, Stato OFF e Generic Error o relay resistenza aperto rilevati */
			case WR_CS_OFF_GENERIC_ERROR:
			case WR_CS_OFF_ERROR_RELE_TRIAC_CC:

				if( 0 == (sftWRstatus.state & BFMASK(PM_SFT_WM_WR_OFF,1)))
				{
					sftWRstatus.state |= (BFMASK(PM_SFT_WM_WR_OFF_INIT,1));
				}
				else
				{
					sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_OFF_INIT,1));
				}

				sftWRstatus.state |= BFMASK(PM_SFT_WM_WR_OFF,1);
				sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_ON,1)|BFMASK(PM_SFT_WM_WR_ON_INIT,1));

				/* Lo stato che diventa evento viene gestito con una concatenzazione di cicli if, non si
				 * gestisce una priorità di condizioni, per questo è necessario resettare le flag settate
				 * negli altri case dello switch in oggetto */
				sftWRstatus.state &= ~( BFMASK(PM_SFT_WM_WROFF_FAULT_RESET,1) |
										BFMASK(PM_SFT_WM_WRON_FAULT_RESET,1)  |
										BFMASK(PM_SFT_WM_PREFAULTRESET_START,1) );
				sftWRstatus.prefaultResetCnt = 0;

				/*sftWRstatus.state |= (BFMASK(PM_SFT_WM_WROFF_FAULT_MON,1));*/
				event |= (BFMASK(EVENT_WM_WROFF_FAULT_MON,1));


			break;

			/* Washing Heater, Stato ON e Generic Error rilevato */
			case WR_CS_ON_GENERIC_ERROR:
			case WR_CS_ON_ERROR_RELE_TRIAC_CA:

				/* Questa situazione della Consistency quando la procedura ricerca guasto relay
				 * incollato su ramo WR non è attiva.
				 */

				if( 0 == (sftWRstatus.state & BFMASK(PM_SFT_WM_WR_ON,1)))
				{
					sftWRstatus.state |= (BFMASK(PM_SFT_WM_WR_ON_INIT,1));
				}
				else
				{
					sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_ON_INIT,1));
				}

				sftWRstatus.state |= BFMASK(PM_SFT_WM_WR_ON,1);
				sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_WR_OFF,1)|BFMASK(PM_SFT_WM_WR_OFF_INIT,1));

				/* Lo stato che diventa evento viene gestito con una concatenzazione di cicli if, non si
				 * gestisce una priorità di condizioni, per questo è necessario resettare le flag settate
				 * negli altri case dello switch in oggetto */
				sftWRstatus.state &= ~( BFMASK(PM_SFT_WM_WROFF_FAULT_RESET,1) |
										BFMASK(PM_SFT_WM_WRON_FAULT_RESET,1)  |
										BFMASK(PM_SFT_WM_PREFAULTRESET_START,1) );
				sftWRstatus.prefaultResetCnt = 0;

				event |= (BFMASK(EVENT_WM_WRON_FAULT_MON,1));
			break;

			/* Valore Consistency Resistenza Lavaggio diverso da quelli attesi o
			 * Consistency non valido */
			default:
			break;

		}	/* fine switch Analisi Consistency Resistenza Lavaggio */



	}
	/* if( safetyData->status == SFT_MODULE_FAULT ) */


	if( sftWRstatus.state & BFMASK(PM_SFT_WM_WR_ON_INIT,1))
	{
		event |= BFMASK(EVENT_WM_WR_ON_INIT,1);
	}
	if( sftWRstatus.state & BFMASK(PM_SFT_WM_WR_OFF_INIT,1))
	{
		event |= BFMASK(EVENT_WM_WR_OFF_INIT,1);
	}
	if( sftWRstatus.state & ( BFMASK(PM_SFT_WM_WROFF_FAULT_RESET,1) | BFMASK(PM_SFT_WM_WRON_FAULT_RESET,1) ) )
	{
		if(safetyData->status == SFT_MODULE_MONITORING)
		{
			/* Utilizzata una unica flag evento impiegata nella FSM gestione fault */
			event |= (BFMASK(EVENT_WM_WR_FAULT_RESET,1));
		}
		else if(safetyData->status == SFT_MODULE_PREFAULT)
		{

			/* Check valore faultCnt */
			/* Se faultCnt <= 50(5s): nessun effetto, prefault time-out deve scadere, si deve
			 * andare in FAULT */
			/* Altrimenti > 50(5s): alla scadenza dei 5s si controlla che il valore FB sia
			 * ancora OK (caso 1) oppure no (caso 2) */
				/* caso 1: reset FSM di gestione FAULT; */
				/* caso 2: mentre FSM è rimasta nello stato di PREFAULT(fault time running),
				 * si riprende la gestione del guasto da lì dove è stata interrotta, ma con
				 * 5s di fault time in meno */

			if(sftWRstatus.faultCnt > GEN_FLT_RESET_TIMER_VALUE)
			{
				/* Se il counter utilizzato per il reset prefault è nullo, vuole dire che
				 * può essere impostato, ovvero che non è in running */
				if( !(sftWRstatus.state & (BFMASK(PM_SFT_WM_PREFAULTRESET_START,1))) )
				{
					sftWRstatus.prefaultResetCnt = GEN_FLT_RESET_TIMER_VALUE;
					sftWRstatus.state |= (BFMASK(PM_SFT_WM_PREFAULTRESET_START,1));	/* FLAG ONE SHOT */
				}

			}
			/* In seguito al precedente ciclo if-else: analisi dello stato FSM e valore
			 * timer FSM, si passa alla esecuzione delle operazioni durante il periodo
			 * di filtro(reset prefault)  */
			if( !(sftWRstatus.prefaultResetCnt) && (sftWRstatus.state & (BFMASK(PM_SFT_WM_PREFAULTRESET_START,1))) )
			{
				/* Utilizzata una unica flag impiegata in FSM gestione faults */
				event |= (BFMASK(EVENT_WM_WR_FAULT_RESET,1));

				sftWRstatus.state &= ~(BFMASK(PM_SFT_WM_PREFAULTRESET_START,1));
			}
		}
	}

	switch(safetyData->status)
	{
		case SFT_MODULE_IDLE:

			if(event & BFMASK(EVENT_WM_WROFF_FAULT_MON,1))
			{
				/* go to SFT_MODULE_MONITORING */
				safetyData->status = SFT_MODULE_MONITORING;

				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				/* set the fault source */
				if(curConsistency == WR_CS_OFF_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_WR_OFF_ERR_RELAY_CC;
				}

			}
			else if ( event & BFMASK(EVENT_WM_WRON_FAULT_MON,1) )
			{
				/* go to SFT_MODULE_MONITORING */
				safetyData->status = SFT_MODULE_MONITORING;

				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				/* set the fault source */
				if(curConsistency == WR_CS_ON_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					if(PM_SFT_WM_NTCProbe_isHighTemp(wash))
					{
						sftWRstatus.faultCnt = 0;
					}
					safetyData->faultSrc = FAULTSRC_WR_ON_ERR_RELAY_CA;
				}
			}

		break;

		case SFT_MODULE_RESET:

			if(!(sftWRstatus.faultCnt))
			{

				sftWRstatus.faultCode = 0;
				sftWRstatus.faultsubCode = 0;
				safetyData->faultSrc = NO_FAULTSRC_WR;

				safetyData->status = SFT_MODULE_IDLE;
			}

		break;

		case SFT_MODULE_MONITORING:

			/* In questo stato FSM è attivo il prefault time */

			if( event & (BFMASK(EVENT_WM_WR_FAULT_RESET,1)) )
			{
				/* resistor is working fine */
				/* si imposta lo stato di reset nella gestione fault */
				safetyData->status = SFT_MODULE_RESET;

				sftWRstatus.faultCnt=0;

			}else if(!(sftWRstatus.faultCnt))
			{
				/* prefault time elapsed */
				safetyData->status = SFT_MODULE_PREFAULT;

				if(event & BFMASK(EVENT_WM_WROFF_FAULT_MON,1))
				{
					sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetFaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);
				}
				else if(event & BFMASK(EVENT_WM_WRON_FAULT_MON,1))
				{
					if((safetyData->faultSrc == FAULTSRC_WR_ON_ERR_RELAY_CA) &&
							(PM_SFT_WM_NTCProbe_isHighTemp(wash)))
					{
						sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetFltTimeCAOnHighTemp(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);
					}
					else
					{
						sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetFaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);
					}
				}

			}else if((event & (BFMASK(EVENT_WM_WR_OFF_INIT,1)|BFMASK(EVENT_WM_WROFF_FAULT_MON,1)))==(BFMASK(EVENT_WM_WR_OFF_INIT,1)|BFMASK(EVENT_WM_WROFF_FAULT_MON,1)))
			{
				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == WR_CS_OFF_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_WR_OFF_ERR_RELAY_CC;
				}
			}
			else if((event & (BFMASK(EVENT_WM_WR_ON_INIT,1)|BFMASK(EVENT_WM_WRON_FAULT_MON,1)))==(BFMASK(EVENT_WM_WR_ON_INIT,1)|BFMASK(EVENT_WM_WRON_FAULT_MON,1)))
			{
				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == WR_CS_ON_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_WR_ON_ERR_RELAY_CA;
				}
			}

		break;

		case SFT_MODULE_WARNING:
		break;

		case SFT_MODULE_PROTECTION:
		break;

		case SFT_MODULE_PREFAULT:

			/* In questo stato FSM è attivo il fault time */

			if( event & (BFMASK(EVENT_WM_WR_FAULT_RESET,1)) )
			{

				/* Se evento di reset prefault è stato settato, vuol dire che sono già
				 * trascorsi i 5s di filtro su check FB, il cui valore è tornato OK da KO.
				 * Al fine di resettare lo stato prefault è allora necessario impostare
				 * il successivo stato macchina, processato immediatamente */

				/* resistor is working fine: si imposta lo stato di reset nella gestione fault */
				safetyData->status = SFT_MODULE_RESET;

				sftWRstatus.faultCnt = 0;

			}
			else if( (event & (BFMASK(EVENT_WM_WR_OFF_INIT,1) | BFMASK(EVENT_WM_WROFF_FAULT_MON,1))) ==
					(BFMASK(EVENT_WM_WR_OFF_INIT,1) | BFMASK(EVENT_WM_WROFF_FAULT_MON,1)) )
			{
				safetyData->status = SFT_MODULE_MONITORING;

				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == WR_CS_OFF_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_WR_OFF_ERR_RELAY_CC;
				}
			}
			else if((event & (BFMASK(EVENT_WM_WR_ON_INIT,1)|BFMASK(EVENT_WM_WRON_FAULT_MON,1))) == (BFMASK(EVENT_WM_WR_ON_INIT,1)|BFMASK(EVENT_WM_WRON_FAULT_MON,1)))
			{
				safetyData->status = SFT_MODULE_MONITORING;

				sftWRstatus.faultCnt = PM_SFT_WM_washRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == WR_CS_ON_GENERIC_ERROR)
				{
					safetyData->faultSrc = FAULTSRC_WR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_WR_ON_ERR_RELAY_CA;
				}
			}

			else if(!(sftWRstatus.faultCnt))
			{
				/* fault time elapsed */
				safetyData->status = SFT_MODULE_FAULT;
			}
		break;

		case SFT_MODULE_FAULT:
			/* Handle reset fault request */
			if (applReqFlags & SFT_FLAGS_RESET_REQ)
			{
				safetyData->status = SFT_MODULE_RESET;
			}
		break;

		default:
		break;
	}

	SAFETY_DATA_UPDATE( sftWRstatus );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);

}

uint16_t PM_SFT_WM_washRes_GetTime(sft_module_status_t *status)
{
	return sftWRstatus.faultCnt;
}

#if 0 /* switch relay test*/
/**
Request to start the relay functional test
 */
void PM_SFT_WM_WashRes_SwitchTest_Start(void)
{
	sftWRstatus.SwTestReqBmp |= BFMASK(SWITCH_RELAY_TEST_START_BIT,1);
}
/**
Request to halt the relay functional test
 */
void PM_SFT_WM_WashRes_SwitchTest_Halt(void)
{
	sftWRstatus.SwTestReqBmp |= BFMASK(SWITCH_RELAY_TEST_HALT_BIT,1);
}

/**
indicates whether test has been completed
 */
bool_t PM_SFT_WM_WashRes_SwitchTest_Completed(void)
{
	return (bool_t)(sftWRstatus.SwTestStep == SWITCH_RELAY_TEST_COMPLETED);
}
#endif
/* Local Function */

uint16_t PM_SFT_WM_washRes_GetPrefaultTimeCA(uint8_t bAutoTest)
{
	struct Loader faultParam;
	uint16_t res = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam);

	res = PM_SFT_WM_GET_PREFAULT_TIME_CA((PM_SFT_WM_WashResFaultParamType*)faultParam.Data);

	if(bAutoTest)
	{
		if(res > GEN_FLT_RESET_TIMER_VALUE)
		{
			res = GEN_FLT_RESET_TIMER_VALUE;
		}
	}

	return res;

}

uint16_t PM_SFT_WM_washRes_GetFaultTimeCA(uint8_t bAutoTest)
{
	struct Loader faultParam;
	uint16_t res = 0;
	uint8_t localOp = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam);
	localOp = (uint8_t) (PM_SFT_WM_GET_FAULT_TIME_CA_OPCODE((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));

	if(bAutoTest)
	{
		res = 0;
	}
	else
	{
		if(localOp == 0x10)			/* valore Opcode = 2 */
			res = (uint16_t)(10*PM_SFT_WM_GET_FAULT_TIME_CA((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));
		else if(localOp == 0x18)	/* valore Opcode = 3 */
			res = (uint16_t)(100*PM_SFT_WM_GET_FAULT_TIME_CA((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));
	}

	return res;
}

uint16_t PM_SFT_WM_washRes_GetPrefaultTimeCC(uint8_t bAutoTest)
{
	struct Loader faultParam;
	uint16_t res=0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam);

	res = PM_SFT_WM_GET_PREFAULT_TIME_CC((PM_SFT_WM_WashResFaultParamType*)faultParam.Data);

	if(bAutoTest)
	{
		if(res > GEN_FLT_RESET_TIMER_VALUE)
		{
			res = GEN_FLT_RESET_TIMER_VALUE;
		}
	}

	return res;

}

uint16_t PM_SFT_WM_washRes_GetFaultTimeCC(uint8_t bAutoTest)
{
	struct Loader faultParam;
	uint16_t res = 0;
	uint8_t localOp = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam);
	localOp = (uint8_t) (PM_SFT_WM_GET_FAULT_TIME_CC_OPCODE((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));

	if(bAutoTest)
	{
		res = 0;
	}
	else
	{

		if(localOp == 0x10)			/* valore Opcode = 2 */
			res = (uint16_t)(10*PM_SFT_WM_GET_FAULT_TIME_CC((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));
		else if(localOp == 0x18)	/* valore Opcode = 3 */
			res = (uint16_t)(100*PM_SFT_WM_GET_FAULT_TIME_CC((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));

	}

	return res;
}


uint16_t PM_SFT_WM_washRes_GetFltTimeCAOnHighTemp(uint8_t bAutoTest)
{
	struct Loader faultParam;
	uint16_t res = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_WASH_RES, &faultParam);
	if(faultParam.DataLenght > 0x04)
	{
		res = (uint16_t)(10*PM_SFT_WM_GET_FAULT_TIME_CA_HIGH_TEMP((PM_SFT_WM_WashResFaultParamType*)faultParam.Data));
	}
	else
	{
		res = DEFAULT_TIME_CA_HIGH_TEMP;
	}

	if(bAutoTest)
	{
		res = 0;
	}

	return res;
}
