/*
 * PM_SFT_WM_DryHeater.c
 *
 *  Created on: Mar 11, 2013
 *      Author: francesco.barletta
 */


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#if defined(CONFIG_APPLIANCE_WD)
#include "PM_SFT_WM_Common.h"
#include "PM_SFT_WM_DryHeater.h"
#include "PM_ConsistencyExp.h"
#include "LIB_Utilities.h"
#include "LIB_ZeroCrossingExp.h"
#include "LIB_ClassBVar.h"
#include "Lib_ProductSafety.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* DR state bit */
enum eSFT_drState
{
	/* bitmap corrispondente alle azioni da intraprendere nella
	 * gestione faults resistenza asciugatura
	 */
	PM_SFT_WM_DR_ON_INIT,
	PM_SFT_WM_DR_ON,
	PM_SFT_WM_DR_OFF_INIT,
	PM_SFT_WM_DR_OFF,
	PM_SFT_WM_DR_ZC_OK,
	PM_SFT_WM_DRON_FAULT_RESET,
	PM_SFT_WM_DROFF_FAULT_RESET,
	PM_SFT_WD_PREFAULTRESET_START
};

/* Bit for DR safety event bitmap SFT_DrrResEventBmpType */
enum eSFT_drSftEvent
{
	EVENT_WM_DR_FAULT_RESET,
	EVENT_WM_DROFF_FAULT_MON,
	EVENT_WM_DRON_FAULT_MON,
	EVENT_WM_DR_ZC_OK,
	EVENT_WM_DR_RESET_DELAY,
	EVENT_WM_DR_ON_INIT,
	EVENT_WM_DR_OFF_INIT
};

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
 * Info related to the control of Drying Resistor
 * */
typedef struct {
	uint8_t		loadPos;
	uint8_t 	state;
	uint16_t 	faultCnt;
	uint8_t 	prefaultResetCnt;
	uint8_t		DRPumpPos;
	uint8_t		EVPPos;
}PM_SFT_WM_DryResFaultStatus_t;

typedef struct DryResFaultParam_s
{
	uint8_t		dryProbePrefault1;		/**< CA prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t		dryProbePrefault0;
	uint8_t		dryProbeFaultOpcode0;
	uint8_t		dryProbeFault0;			/**< CA fault x 100 ms - 1 bytes, unsigned	*/

	uint8_t		relayCAprefault1;		/**< CA prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t		relayCAprefault0;
	uint8_t		relayCAfaultOpcode0;
	uint8_t		relayCAfault0;			/**< CA fault x 100 ms - 1 bytes, unsigned	*/

	uint8_t		swRelayCCprefault1;		/**< CC prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t		swRelayCCprefault0;
	uint8_t		swRelayCCfaultOpcode0;
	uint8_t		swRelayCCfault0;		/**< CA fault x 100 ms - 1 bytes, unsigned	*/

	uint8_t		relayCCprefault1;		/**< CC prefault x 100 ms - 2 bytes, unsigned , BE	*/
	uint8_t		relayCCprefault0;
	uint8_t		relayCCfaultOpcode0;
	uint8_t		relayCCfault0;			/**< CA fault x 100 ms - 1 bytes, unsigned	*/
}PM_SFT_WM_DryResFaultParamType;

/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
PM_SFT_WM_DryResFaultStatus_t	sftDRstatus;

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
\date		23/04/2013
\version

\note

*/
void PM_SFT_WM_DryRes_Init(sft_module_status_t *safetyData )
{
struct Loader faultParam;

	SAFETY_REGION_START();

	safetyData->status = SFT_MODULE_IDLE;
	safetyData->faultSrc = NO_FAULTSRC_DR;

	sftDRstatus.loadPos = LIB_IOM_GetLoadPos(LD_DRY_RES_POS);
	if (sftDRstatus.loadPos != LOAD_NOT_FOUND)
	{

		if( 0 != BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &faultParam) )
		{
			safetyData->status = SFT_MODULE_FAULT;
			safetyData->faultSrc = FAULTSRC_DR_BAD_SETTING;
		}
		else
		{
			uint8_t tmpDPPos = LIB_IOM_GetLoadPos(LD_DRAIN_PUMP_POS);
			uint8_t tmpEVPPos = LIB_IOM_GetLoadPos(LD_PRE_WASH_EV_POS);

			/* Assuming Drain pump configured as a coupled load into a switch relay
			 * means it's related to EVP
			 */
			if ( (LIB_IOM_FindLoadOnSwitchRelay(tmpEVPPos) & (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE))
				!= (LOAD_SR_FOUND | LOAD_SR_CFG_COUPLE))
			{
				tmpEVPPos = LOAD_NOT_FOUND;
			}

			sftDRstatus.DRPumpPos = tmpDPPos;
			sftDRstatus.EVPPos = tmpEVPPos;
		}

	}

	SAFETY_DATA_UPDATE( sftDRstatus );
	SAFETY_REGION_END();

}

/**
\n It manages drying resistor fault conditions

\param		none
\return 	none

\author		Francesco Barletta
\date		23/04/2013
\version

\note

*/
void PM_SFT_WM_DryRes_Monitor(iom_loads_struct_t *pNewLoads, sft_module_status_t *safetyData, uint8_t applReqFlags)
{
uint8_t curConsistency = PM_CS_NOT_VALID_CODE;
uint8_t drPumpCons = PM_CS_NOT_VALID_CODE;
SFT_drEventBmpType event = 0;

	LIB_SFT_EnterSafetyModule(safetyData);

	SAFETY_REGION_START();
	SAFETY_DATA_CHECK( sftDRstatus );

	/* Down-counter qui implementato per mezzo di una var statica;
	 * la routine in oggetto é richiamata con una frequenza di 100Hz (@10ms)
	 */
	if(sftDRstatus.faultCnt)
	{
		sftDRstatus.faultCnt--;
	}
	if(sftDRstatus.prefaultResetCnt)
	{
		sftDRstatus.prefaultResetCnt--;
	}

	if( safetyData->status != SFT_MODULE_FAULT )
	{

		if( goodZCSignal() == TRUE )
		{

		/* Il check dei fault resistenza lavaggio via monitoraggio FB, la loro gestione per mezzo
		 * di una macchina a stati temporizzata, è indipendente dalla gestione fault switch relay
		 * incollato lato Loading, ma ad essa subordinata in quanto ad esito.
		 */

			if(PM_CS_NO_ERROR == PM_Consistency_IsValueReady(sftDRstatus.loadPos, &curConsistency))
			{
				/*								 | LOW(0)	| HIGH(1) | UNPH(2)	| PH(3)	| */
				/*[riga 0] UNPILOT (EVP OFF)  	 | 1 [GE]	| 3 [SC]  | 1 [GE]	| 0 [OK]| */
				/*[riga 1] UNPILOT (EVP ON)   	 | 1 [GE]	| 0 [OK]  | 1 [GE]	| 3 [L]	| */
				/*[riga 2] PILOT    			 | 1 [GE]	| 0 [OK]  | 1 [GE]	| 4 [OC]| */
				/* GE: F01, 0x42 														  */
				/* SC: F15, 0x01 														  */
				/* OC: F01, 0x41 														  */
				/*  L: F15, 0x02 														  */

				if ((PM_CS_NO_ERROR == PM_Consistency_IsValueReady(sftDRstatus.DRPumpPos, &drPumpCons)))
				{
					if ( (drPumpCons == PM_CS_VALUE_ERR_SC) && !(curConsistency & PM_CS_STATUS_MASK) )
					{
						/* Drain pump consistency has to be checked if it's due to coupled
						 * load (Prewash EV) ON; only if so cousistency OK and SC can be swapped
						 */
						if ( (sftDRstatus.EVPPos != LOAD_NOT_FOUND) &&
							(LIB_IOM_GetLoadStatus(sftDRstatus.EVPPos) != LOAD_REG_OFF))
						{
							if(curConsistency == PM_CS_VALUE_OK)
								curConsistency = PM_CS_VALUE_ERR_SC;
							else if(curConsistency == PM_CS_VALUE_ERR_SC)
								curConsistency = PM_CS_VALUE_OK;
						}
					}
				}
			}
		}
		else
		{		/* segnale ZC non presente e/o non valido */
			/* Se la porta é sbloccata e lo stato gestione guasti NON è FAULT(tutti gli altri),
			 * viene considerato come caso OK. */

			curConsistency = ((pNewLoads->loads_req[sftDRstatus.loadPos] != LOAD_REG_OFF) ? DR_CS_ON_OK : DR_CS_OFF_OK);
		}

		/* Analisi Consistency Resistenza Asciugatura */
		switch(curConsistency)
		{

			/* Drying Res, Stato Indipendente */
			case DR_CS_ON_OK:

				/* Con feedback OK, vengono resettati tutti i guasti della resistenza asciugatura */
				/* Se è attiva la procedura di rilevazione fault su resistenza, si avvia il
				* reset della stessa per liberare le risorse impiegate.
				*/
				/* In ogni caso se, dopo l’inizio della rilevazione di un guasto e prima
				* che venga dichiarato il Fault, il feedback torna a segnalare un valore coerente
				* con lo stato del carico desiderato, la procedura di rilevazione guasto viene
				* resettata immediatamente (se stato FSM in corso è MONITORING con Prefault Time
				* attivo) o dopo 5 secondi (se stato FSM in corso è PREFAULT con Fault Time attivo).
				* Il primo secondo viene impiegato per filtrare condizioni spurie su valore feedback
				* dovute a rumore elettromagnatico, in generale condizioni elettriche ambientali
				* anomale, la parte restante del periodo è un vero e proprio delay.
				* A livello di logica,il reset della condizione di guasto in corso,nei casi di FB
				* che torna OK,nn viene gestito immediatamente via evento,bensì via "state":si
				* rimanda quindi tutto alla fase 2 del task in oggetto.Invece in caso di condizione
				* di guasto rilevata,il reset di cui sopra oppure l'impostazione dello stato di
				* monitoraggio deve essere immediata,si utilizza quindi l'evento.In questo caso
				* poi,tutti i dati relativi al reset di cui sopra vengono resettati al fine di
				* gestire correttamente l'iterazione.
				*/

				/* Meccanismo di prima volta per tracciare il passaggio dallo stato OFF a ON
				 * della resistenza asciugatura */
				if( 0 == (sftDRstatus.state & BFMASK(PM_SFT_WM_DR_ON,1)) )
				{
					sftDRstatus.state |= (BFMASK(PM_SFT_WM_DR_ON_INIT,1));
				}
				else
				{
					sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_ON_INIT,1));
				}

				sftDRstatus.state |= BFMASK(PM_SFT_WM_DR_ON,1);
				sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_OFF,1) | BFMASK(PM_SFT_WM_DR_OFF_INIT,1));

				sftDRstatus.state |= (BFMASK(PM_SFT_WM_DRON_FAULT_RESET,1));
				/* event |= (BFMASK(EVENT_WM_DRON_FAULT_RESET,1)); */

			break;

			case DR_CS_OFF_OK:
			case DR_CS_TRN_OK:

				/* Nel caso in cui la DR non è pilotata (OFF) ed il FB è OK, si ribadisce un
				 * reset della macchina a stati, per poi andare nel successivo ciclo: dopo 100ms,
				 * nello stato IDLE (attesa). In questo modo, tutti gli stati macchina diversi
				 * da quello di FAULT ormai attivo, vengono trattati alla stessa maniera.
				 */
				/* In ogni caso se, dopo l’inizio della rilevazione di un guasto e prima
				* che venga dichiarato il Fault, il feedback torna a segnalare un valore coerente
				* con lo stato del carico desiderato, la procedura di rilevazione guasto viene
				* resettata immediatamente (se stato FSM in corso è MONITORING con Prefault Time
				* attivo) o dopo 5 secondi (se stato FSM in corso è PREFAULT con Fault Time attivo).
				* Il primo secondo viene impiegato per filtrare condizioni spurie su valore feedback
				* dovute a rumore elettromagnatico, in generale condizioni elettriche ambientali
				* anomale, la parte restante del periodo è un vero e proprio delay
				*/

				/* Meccanismo di prima volta per tracciare il passaggio dallo stato ON a OFF
				 * della resistenza asciugatura */
				if( 0 == (sftDRstatus.state & BFMASK(PM_SFT_WM_DR_OFF,1)) )
				{
					sftDRstatus.state |= (BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}
				else
				{
					sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}

				sftDRstatus.state |= BFMASK(PM_SFT_WM_DR_OFF,1);
				sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_ON,1) | BFMASK(PM_SFT_WM_DR_ON_INIT,1));

				sftDRstatus.state |= (BFMASK(PM_SFT_WM_DROFF_FAULT_RESET,1));

			break;

			case DR_CS_OFF_GEN_ERR:
			case DR_CS_TRN_GEN_ERR:

				/* Meccanismo di prima volta per tracciare il passaggio dallo stato ON a OFF
				 * in presenza di FAULT della resistenza asciugatura */
				if( 0 == (sftDRstatus.state & BFMASK(PM_SFT_WM_DR_OFF,1)) )
				{
					sftDRstatus.state |= (BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}
				else
				{
					sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}

				sftDRstatus.state |= BFMASK(PM_SFT_WM_DR_OFF,1);
				sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_ON,1) | BFMASK(PM_SFT_WM_DR_ON_INIT,1));

				/* Lo stato che diventa evento viene gestito con una concatenzazione di cicli if, non si
				 * gestisce una priorità di condizioni, per questo è necessario resettare le flag settate
				 * negli altri case dello switch in oggetto */
				sftDRstatus.state &= ~( BFMASK(PM_SFT_WM_DROFF_FAULT_RESET,1) |
										BFMASK(PM_SFT_WM_DRON_FAULT_RESET,1)  |
										BFMASK(PM_SFT_WD_PREFAULTRESET_START,1) );
				sftDRstatus.prefaultResetCnt = 0;

				event |= (BFMASK(EVENT_WM_DROFF_FAULT_MON,1));

			break;

			case DR_CS_OFF_RELE_TRIAC_SC:
			case DR_CS_TRN_RELE_TRIAC_SC:

				/* Meccanismo di prima volta per tracciare il passaggio dallo stato ON a OFF
				 * in presenza di FAULT della resistenza asciugatura */
				if( 0 == (sftDRstatus.state & BFMASK(PM_SFT_WM_DR_OFF,1)) )
				{
					sftDRstatus.state |= (BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}
				else
				{
					sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_OFF_INIT,1));
				}

				sftDRstatus.state |= BFMASK(PM_SFT_WM_DR_OFF,1);
				sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_ON,1) | BFMASK(PM_SFT_WM_DR_ON_INIT,1));

				/* Lo stato che diventa evento viene gestito con una concatenzazione di cicli if, non si
				 * gestisce una priorità di condizioni, per questo è necessario resettare le flag settate
				 * negli altri case dello switch in oggetto */
				sftDRstatus.state &= ~( BFMASK(PM_SFT_WM_DROFF_FAULT_RESET,1) |
										BFMASK(PM_SFT_WM_DRON_FAULT_RESET,1)  |
										BFMASK(PM_SFT_WD_PREFAULTRESET_START,1) );
				sftDRstatus.prefaultResetCnt = 0;

				event |= (BFMASK(EVENT_WM_DROFF_FAULT_MON,1));

			break;

			case DR_CS_ON_GEN_ERR:
			case DR_CS_ON_RELE_TRIAC_OC:

				/* Meccanismo di prima volta per tracciare il passaggio dallo stato OFF a ON
				 * in presenza di FAULT della resistenza asciugatura */
				if( 0 == (sftDRstatus.state & BFMASK(PM_SFT_WM_DR_ON,1)) )
				{
					sftDRstatus.state |= (BFMASK(PM_SFT_WM_DR_ON_INIT,1));
				}
				else
				{
					sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_ON_INIT,1));
				}

				sftDRstatus.state |= BFMASK(PM_SFT_WM_DR_ON,1);
				sftDRstatus.state &= ~(BFMASK(PM_SFT_WM_DR_OFF,1) | BFMASK(PM_SFT_WM_DR_OFF_INIT,1));

				/* Lo stato che diventa evento viene gestito con una concatenzazione di cicli if, non si
				 * gestisce una priorità di condizioni, per questo è necessario resettare le flag settate
				 * negli altri case dello switch in oggetto */
				sftDRstatus.state &= ~( BFMASK(PM_SFT_WM_DROFF_FAULT_RESET,1) |
										BFMASK(PM_SFT_WM_DRON_FAULT_RESET,1)  |
										BFMASK(PM_SFT_WD_PREFAULTRESET_START,1) );
				sftDRstatus.prefaultResetCnt = 0;

				event |= (BFMASK(EVENT_WM_DRON_FAULT_MON,1));

			break;

			/* Valore Consistency Resistenza Asciugatura diverso da quelli attesi */
			default:
				/* do nothing */
			break;

		}	/* fine switch Analisi Consistency Resistenza Lavaggio */

	}	/* fine if check stato macchina diverso da FAULT */

	/* FSM di passaggio da stato FB ad evento */
	if( sftDRstatus.state & BFMASK(PM_SFT_WM_DR_ON_INIT,1) )
	{
		event |= BFMASK(EVENT_WM_DR_ON_INIT,1);
	}
	if( sftDRstatus.state & BFMASK(PM_SFT_WM_DR_OFF_INIT,1) )
	{
		event |= BFMASK(EVENT_WM_DR_OFF_INIT,1);
	}
	if( sftDRstatus.state & ( BFMASK(PM_SFT_WM_DROFF_FAULT_RESET,1) | BFMASK(PM_SFT_WM_DRON_FAULT_RESET,1) ) )
	{
		if(safetyData->status == SFT_MODULE_MONITORING)
		{
			/* Utilizzata una unica flag evento impiegata nella FSM gestione fault */
			event |= (BFMASK(EVENT_WM_DR_FAULT_RESET,1));
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

			if(sftDRstatus.faultCnt > GEN_FLT_RESET_TIMER_VALUE)
			{
				/* Se il counter utilizzato per il reset prefault è nullo, vuole dire che
				 * può essere impostato, ovvero che non è in running */
				if( !(sftDRstatus.state & (BFMASK(PM_SFT_WD_PREFAULTRESET_START,1))) )
				{
					sftDRstatus.prefaultResetCnt = GEN_FLT_RESET_TIMER_VALUE;
					sftDRstatus.state |= (BFMASK(PM_SFT_WD_PREFAULTRESET_START,1));	/* FLAG ONE SHOT */
				}

			}
			/* In seguito al precedente ciclo if-else: analisi dello stato FSM e valore
			 * timer FSM, si passa alla esecuzione delle operazioni durante il periodo
			 * di filtro(reset prefault)  */
			if( !(sftDRstatus.prefaultResetCnt) && (sftDRstatus.state & (BFMASK(PM_SFT_WD_PREFAULTRESET_START,1))) )
			{
				/* Utilizzata una unica flag impiegata in FSM gestione faults */
				event |= (BFMASK(EVENT_WM_DR_FAULT_RESET,1));

				sftDRstatus.state &= ~(BFMASK(PM_SFT_WD_PREFAULTRESET_START,1));
			}
		}
	}

	/* FSM gestione faults in corso per Resistenza Asciugatura */
	switch(safetyData->status)
	{
		case SFT_MODULE_IDLE:

			/* Se è stata rilevata una condizione di FAULT con stato carico OFF: */
			if( event & BFMASK(EVENT_WM_DROFF_FAULT_MON,1) )
			{
				/* Si imposta FSM next state pari a MONITOR */
				safetyData->status = SFT_MODULE_MONITORING;
				/* Si preleva dal file di Setting il valore di "Prefault Time": */
				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				/* Se Consistency indica un valore FB pari a LOW(0) oppure FREQ(2):  */
				if((curConsistency & PM_CS_VALUE_MASK) == PM_CS_VALUE_ERR_GENERIC)
				{
					/* Si modifica la sorgente di FAULT,si avrà F 01.42 */
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				else if((curConsistency & PM_CS_VALUE_MASK) == PM_CS_VALUE_ERR_SC)
				{
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP OFF: */
					if (PM_CS_VALUE_ERR_SC != drPumpCons)
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_RELAY_SC;
					}
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP ON: */
					else
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_DISP;
					}
				}

			}
			else if( event & BFMASK(EVENT_WM_DRON_FAULT_MON,1) )
			{
				safetyData->status = SFT_MODULE_MONITORING;
				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == DR_CS_ON_GEN_ERR)
				{
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				/* altrimenti, se Consistency indica un valore FB pari a PHASE(3), con
				 * stato stato carico ON: */
				else if(curConsistency == DR_CS_ON_RELE_TRIAC_OC)
				{
					safetyData->faultSrc = FAULTSRC_DR_ON_RELAY_OC;
				}

			}

		break;

		case SFT_MODULE_RESET:

			if( !(sftDRstatus.faultCnt) )
			{
				safetyData->status = SFT_MODULE_IDLE;
				safetyData->faultSrc = NO_FAULTSRC_DR;
			}

		break;

		/* Lo stato di monitoraggio FAULT per mezzo di FB resistenza asciugatura
		 * è attivo durante tutto il "Prefault Time" */
		case SFT_MODULE_MONITORING:

			/* Nel caso sia occorsa una condizione di reset procedura FAULT in corso: */
			if( event & (BFMASK(EVENT_WM_DR_FAULT_RESET,1)) )
			{
				safetyData->status = SFT_MODULE_RESET;
				sftDRstatus.faultCnt = 0;
			}
			else if( !(sftDRstatus.faultCnt) )
			{
				safetyData->status = SFT_MODULE_PREFAULT;

				if( event & BFMASK(EVENT_WM_DROFF_FAULT_MON,1) )
				{
					sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetFaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);
				}
				else if( event & BFMASK(EVENT_WM_DRON_FAULT_MON,1) )
				{
					sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetFaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);
				}

			}
			else if( (event & (BFMASK(EVENT_WM_DR_OFF_INIT,1) | BFMASK(EVENT_WM_DROFF_FAULT_MON,1))) ==
							  (BFMASK(EVENT_WM_DR_OFF_INIT,1) | BFMASK(EVENT_WM_DROFF_FAULT_MON,1)) )
			{

				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				/* Se Consistency indica un valore FB pari a LOW(0) oppure FREQ(2):  */
				if(curConsistency == DR_CS_OFF_GEN_ERR)
				{
					/* Si modifica la sorgente di FAULT,si avrà F 01.42 */
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				else if(curConsistency == DR_CS_OFF_RELE_TRIAC_SC)
				{
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP OFF: */
					if (PM_CS_VALUE_ERR_SC != drPumpCons)
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_RELAY_SC;
					}
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP ON: */
					else
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_DISP;
					}
				}

			}
			else if( (event & (BFMASK(EVENT_WM_DR_ON_INIT,1) | BFMASK(EVENT_WM_DRON_FAULT_MON,1))) ==
					 (BFMASK(EVENT_WM_DR_ON_INIT,1) | BFMASK(EVENT_WM_DRON_FAULT_MON,1)) )
			{
				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == DR_CS_ON_GEN_ERR)
				{
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				/* altrimenti, se Consistency indica un valore FB pari a PHASE(3), con
				 * stato stato carico ON: */
				else if(curConsistency == DR_CS_ON_RELE_TRIAC_OC)
				{
					safetyData->faultSrc = FAULTSRC_DR_ON_RELAY_OC;
				}

			}

		break;

		case SFT_MODULE_WARNING:
		break;

		case SFT_MODULE_PROTECTION:
		break;

		/* Lo stato di PREFAULT è attivo durante tutto il "Fault Time" */
		case SFT_MODULE_PREFAULT:

			if( event & (BFMASK(EVENT_WM_DR_FAULT_RESET,1)) )
			{
				/* Se evento di reset prefault è stato settato, vuol dire che sono già
				 * trascorsi i 5s di filtro su check FB, il cui valore è tornato OK da KO.
				 * Al fine di resettare lo stato prefault è allora necessario impostare
				 * il successivo stato macchina, processato immediatamente */
				safetyData->status = SFT_MODULE_RESET;
				sftDRstatus.faultCnt = 0;

			}
			else if( (event & (BFMASK(EVENT_WM_DR_OFF_INIT,1) | BFMASK(EVENT_WM_DROFF_FAULT_MON,1))) ==
					 (BFMASK(EVENT_WM_DR_OFF_INIT,1) | BFMASK(EVENT_WM_DROFF_FAULT_MON,1)) )
			{
				safetyData->status = SFT_MODULE_MONITORING;

				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCC(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				/* Se Consistency indica un valore FB pari a LOW(0) oppure FREQ(2):  */
				if((curConsistency & PM_CS_VALUE_MASK) == PM_CS_VALUE_ERR_GENERIC)
				{
					/* Si modifica la sorgente di FAULT,si avrà F 01.42 */
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				else if ((curConsistency & PM_CS_VALUE_MASK) == PM_CS_VALUE_ERR_SC)
				{
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP OFF: */
					if (PM_CS_VALUE_ERR_SC != drPumpCons)
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_RELAY_SC;
					}
					/* altrimenti, se Consistency indica un valore FB pari a HIGH(1), con
					 * stato EVP ON: */
					else
					{
						safetyData->faultSrc = FAULTSRC_DR_OFF_DISP;
					}
				}

			}
			else if( (event & (BFMASK(EVENT_WM_DR_ON_INIT,1) | BFMASK(EVENT_WM_DRON_FAULT_MON,1))) ==
					 (BFMASK(EVENT_WM_DR_ON_INIT,1) | BFMASK(EVENT_WM_DRON_FAULT_MON,1)))
			{
				safetyData->status = SFT_MODULE_MONITORING;
				sftDRstatus.faultCnt = PM_SFT_WM_DryRes_GetPrefaultTimeCA(applReqFlags & SFT_FLAGS_TEST_TMR_REQ);

				if(curConsistency == DR_CS_ON_GEN_ERR)
				{
					safetyData->faultSrc = FAULTSRC_DR_GEN_ERR;
				}
				else
				{
					safetyData->faultSrc = FAULTSRC_DR_ON_RELAY_OC;
				}
			}
			else if( !(sftDRstatus.faultCnt) )
			{
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

	SAFETY_DATA_UPDATE( sftDRstatus );
	SAFETY_REGION_END();

	LIB_SFT_LeaveSafetyModule(safetyData);
}

/* Local Function */
uint16_t PM_SFT_WM_DryRes_GetPrefaultTimeCA(bool_t bAutoTest)
{
struct Loader faultParam;
uint16_t res = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &faultParam);
	res = PM_SFT_WM_GET_PREFAULT_TIME_CA((PM_SFT_WM_DryResFaultParamType*)faultParam.Data);

	if(bAutoTest)
	{
		if(res > PM_SFT_CA_PREFAULT_AUTOTEST_TIME)
		{
			res = PM_SFT_CA_PREFAULT_AUTOTEST_TIME;
		}
	}

	return res;

}

uint16_t PM_SFT_WM_DryRes_GetFaultTimeCA(bool_t bAutoTest)
{
struct Loader faultParam;
uint16_t res = 0;
uint8_t localOp = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &faultParam);
	localOp = (uint8_t) (PM_SFT_WM_GET_FAULT_TIME_CA_OPCODE((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));

	if(bAutoTest)
	{
		res = 0;
	}
	else
	{

		if(localOp == 0x10)
			res = (uint16_t)(10*PM_SFT_WM_GET_FAULT_TIME_CA((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));
		else if(localOp == 0x18)
			res = (uint16_t)(100*PM_SFT_WM_GET_FAULT_TIME_CA((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));

	}

	return res;
}

uint16_t PM_SFT_WM_DryRes_GetPrefaultTimeCC(bool_t bAutoTest)
{
struct Loader faultParam;
uint16_t res = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &faultParam);
	res = PM_SFT_WM_GET_PREFAULT_TIME_CC((PM_SFT_WM_DryResFaultParamType*)faultParam.Data);

	if(bAutoTest)
	{
		if(res > PM_SFT_CA_PREFAULT_AUTOTEST_TIME)
		{
			res = PM_SFT_CA_PREFAULT_AUTOTEST_TIME;
		}
	}

	return res;

}

uint16_t PM_SFT_WM_DryRes_GetFaultTimeCC(bool_t bAutoTest)
{
struct Loader faultParam;
uint16_t res = 0;
uint8_t localOp = 0;

	BasicLoader(SF_PTR_FAULT, SF_DISPL_FAULT_DRYING, &faultParam);
	localOp = (uint8_t) (PM_SFT_WM_GET_FAULT_TIME_CC_OPCODE((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));

	if(bAutoTest)
	{
		res = 0;
	}
	else
	{

		if(localOp == 0x10)
			res = (uint16_t)(10*PM_SFT_WM_GET_FAULT_TIME_CC((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));
		else if(localOp == 0x18)
			res = (uint16_t)(100*PM_SFT_WM_GET_FAULT_TIME_CC((PM_SFT_WM_DryResFaultParamType*)faultParam.Data));

	}

	return res;
}

uint16_t PM_SFT_WM_DryRes_GetTime(sft_module_status_t *status)
{
	return sftDRstatus.faultCnt;
}

#endif	/* defined(CONFIG_APPLIANCE_WD) */
