
/*
** **************************************************************************
** **************************************************************************
*/ 
/**
 
Si occupa della gestione dello stand by dell'elettrodomestico
 
\file    	APP_StandBy.c

\version 	1.0.0
\date    	27/03/2008
\authors	Simone Costantini
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

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "LIB_BackUpExp.h"
#include "LIB_ResetExp.h"


#if	(SBY_STANDBY_ENABLE == 1)

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static uint8_t STB_CheckStandByFlags(uint8_t flag);

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
/**
DUMMY & DEBUG
*/

#ifdef _STANDBY_DBG
_WM_GLOBAL_ bool_t DBG_seppukuEnabled;
#endif

/* ***************** */
/* Exported function */
/* ***************** */
/**
Processa la variabile ExternalRequest e a seconda del suo valore
attiva la modalità di standby.

\param [in,out]	ExternalRequest (global) è la modalità speciale di funzionamento richiesta

\return			none

\notes:

\author			Simone Costantini
\date			27/03/2008
\version		1.0.0

\review 		Valeria Catani
\date 			11/12/09
\notes			pilotaggio pin solo in presenza del pin Autokill in Output Map

\review 		Emanuela Antognoli
\date 			19/03/10
\notes			inserimento numeri di tentativi se fallisce lo standby con cella Hw (autotaratura stile arcadia)

*/
void STB_HarakiriManager(void)
{
L_PARAM load;
static bool_t backupDisabled = FALSE;

	/*lint -save -e960 */
	if(
		( STB_CheckStandByFlags((uint8_t) SBY_DISABLE_FLAG) == 0 )
		&&
		(!( (STB_CheckStandByFlags((uint8_t) SBY_WAIT_DISABLE_FLAG) != 0) && (RegDev_Pos[LD_STANDBY_REG_IND] == NO_LD_POS) ) )
	  )
	/*lint -restore */
	{/* Standby abilitato */

		load.b = LOAD_OFF; /* SbyLowPow 14.12.09: inizializzato a LOAD_ON 
							per essere a FALSE nel controllo successivo
							se RegDev_Pos[LD_STANDBY_REG_IND] = NO_LD_POS */
	
		if(RegDev_Pos[LD_STANDBY_REG_IND] != NO_LD_POS)	/* 11.12.09 SbyLowPow*/
		{
			/* Non viene resettato ExternalRequest: finchè non avviene un reset freddo
			la Main continua a pilotare il pin */
			/* ExternalRequest = 0; */
		 	load = LoadsReq_New[RegDev_Pos[LD_STANDBY_REG_IND]]; /* SS - TODO verificare quando avviene la lettura */
		}

		if (
		(( ExternalRequest == SYSTEM_NORMAL_FAST_STANDBY_MODE) || (LOAD_ON == load.b)  )
#ifdef _STANDBY_DBG
			|| (DBG_seppukuEnabled)
#endif /* _STANDBY_DBG */
		)
		{
			/* StandBy Fast */
			HarakiriRequired = HKM_FAST_SB_ON;
			HarakiriLowPower.Req = HKM_LOW_POWER_RESET;	/* SbyLowPow 21.01.10 */
		}
		else if(SYSTEM_NORMAL_STANDBY_MODE == ExternalRequest)
		{
			/* StandBy Full */
			if(HKM_SB_RESET == HarakiriRequired)
			{
				/* Setta il counter prima di mandare il comando di harakiri */
				HarakiriRequired = HKM_SB_ON;
				HarakiriLowPower.Req = HKM_LOW_POWER_RESET;	/* SbyLowPow 21.01.10 */
#if ( SW_TIMER_NORM_MODE ==	FALSE)
				Set_MediumCounter((SWTC_CountersType)STB_HARAKIRI_COUNTER_M,
				                (SWTC_PrescalerType)COUNTERS2_TBASE_100MS,
				                TOUT_PRE_STANDBY(Tab_SFTStandByData.Data));
#else
				Set_DownCounter((uint8_t) MEDIUM_COUNTER, (SWTC_CountersType) STB_HARAKIRI_COUNTER_M, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_TimeType) TOUT_PRE_STANDBY(Tab_SFTStandByData.Data));								
#endif				
				SWTC_Run((SWTC_CountersType)STB_HARAKIRI_COUNTER_M);
			}
			else if ( SWTC_IsExpired((SWTC_CountersType)STB_HARAKIRI_COUNTER_M) )
			{
				/* Terminato il tempo di attesa */
				HarakiriRequired = HKM_FAST_SB_ON;
			}
			else
			{
				/* do nothing... */
			}
		}
		else if(ExternalRequest == SYSTEM_NORMAL_RESET_STANDBY_MODE)
		{
			/* Pacchetto Reset StandBy */
			HarakiriLowPower.Req = HKM_LOW_POWER_RESET;	/* SbyLowPow 21.01.10 */
			HarakiriRequired = HKM_SB_RESET;
			SWTC_Stop((SWTC_CountersType) STB_HARAKIRI_COUNTER_M);
		}
		else
		{
			/* Ricevuto altro Normal Mode o ExternalRequest = 0 dopo RESET FREDDO */
			if(HarakiriLowPower.Req !=  HKM_LOW_POWER_REQ )
			{
				/* non siamo in attesa della comunicazione UI,
				potrebbe essere un caso di Wait disabilitato da setting (vecchia gestione con cella)
				o un qualsiasi altro stato della macchina a stati Standby - Wait */
				HarakiriLowPower.Req = HKM_LOW_POWER_RESET;	/* SbyLowPow 21.01.10 */
				HarakiriRequired = HKM_SB_RESET;
				SWTC_Stop((SWTC_CountersType) STB_HARAKIRI_COUNTER_M);
			}
		}
		
		/* SbyLowPow 15.12.09 */
		if(HarakiriLowPower.Req == HKM_LOW_POWER_REQ)
		{
			/* attesa comunicazione UI dopo RESET FREDDO in Standby Low Power */
			if(SWTC_IsExpired((SWTC_CountersType) STB_HARAKIRI_COUNTER_M))
			{
				/* conteggio attesa comunicazione UI concluso -> auto -standby */
				HarakiriRequired = HKM_FAST_SB_ON;	/* rientro nello stato FAST_SBY_ON ma con stato HarakiriLowPower.Req = 1 che varr� backuppato (caso di auto -standby)*/	
			}
		}
	
		if(HarakiriRequired == HKM_FAST_SB_ON)
		{
			/* StandBy Attivato */
#if defined(SLEEP_MODE_ENABLE)
			if(STB_CheckStandByFlags(SBY_WAIT_DISABLE_FLAG) == 0)
			{
				if(RegDev_Pos[LD_STANDBY_REG_IND] != NO_LD_POS)	/* 11.12.09 SbyLowPow*/
				{
					/* stanby con cella di memoria HW */
					if(HarakiriLowPower.Hitpoints < HKM_N_MAX_HITPOINTS)
					{
						HarakiriLowPower.Mode = HKM_SBY_HW_MODE;	
					}
					else
					{
						HarakiriLowPower.Mode = HKM_SBY_LOW_POWER_MODE;
					}
				}
				else
				{
					/* wait mode*/
					HarakiriLowPower.Mode = HKM_SBY_LOW_POWER_MODE;
				}
			}
			else
#endif /* SLEEP_MODE_ENABLE */
			{
				/* NB: il caso in cui il flag wait disable sia impostato ma non sia definito il pin è stato coperto sopra, quindi quando arrivo qui è perchè devo andare in
				 * standby in qualche modo.
				 */
				/* stanby con cella Hw ma senza tentativi di sleep*/
				HarakiriLowPower.Mode = HKM_SBY_NO_SLEEP_MODE;
			}

			if (!backupDisabled)
			{
				backupDisabled = TRUE;

				HarakiriLowPower.Req =  HKM_LOW_POWER_REQ;
				/* Config+init timer reset post StandBy */
#if ( SW_TIMER_NORM_MODE ==	FALSE)
				Set_MediumCounter((SWTC_CountersType)STB_HARAKIRI_COUNTER_M,
				                (SWTC_PrescalerType)COUNTERS2_TBASE_100MS,
				                (2*TOUT_RESET_STANDBY(Tab_SFTStandByData.Data)));
#else
				Set_DownCounter((uint8_t) MEDIUM_COUNTER, (SWTC_CountersType) STB_HARAKIRI_COUNTER_M, (SWTC_PrescalerType) COUNTERS2_TBASE_100MS, (SWTC_TimeType) (2 * TOUT_RESET_STANDBY(Tab_SFTStandByData.Data)));
#endif				
				/* Disables futher backups */
				LIB_Backup_SetBackupControlMonitor(BACKUP_NO_SOURCE);
				/* Force backup */
				LIB_Backup_ForceBackup();
			}

			SWTC_Run((SWTC_CountersType) STB_HARAKIRI_COUNTER_M);

			if((HarakiriLowPower.Mode == HKM_SBY_NO_SLEEP_MODE) || (HarakiriLowPower.Hitpoints < HKM_N_MAX_HITPOINTS))
			{
				if(RegDev_Pos[LD_STANDBY_REG_IND] != NO_LD_POS)	/* 11.12.09 SbyLowPow*/
				{
					load.b = LOAD_ON;
#if ( PILOT_NORM_STANDBY_PIN ==  1) 
					/* MQ AN Tutta da verificare */
					LoadsReq_New[RegDev_Pos[LD_STANDBY_REG_IND]] = load;
#else
					(void)LIB_IOM_SetLoad(RegDev_Pos[LD_STANDBY_REG_IND], load);
#endif /* PILOT_NORM_STANDBY_PIN ==   1 */
				}
			}
		}
	}
	else
	{
		/* Standby disabilitato */
		HarakiriLowPower.Req = HKM_LOW_POWER_RESET;	/* SbyLowPow 21.01.10 */
		HarakiriRequired = HKM_SB_RESET;
		SWTC_Stop((SWTC_CountersType) STB_HARAKIRI_COUNTER_M);
	}
} /* END: STB_HarakiriManager */

/**
Forza un reset quando il count-down impostato
dallo StandBy è scaduto.

\return			none

\notes:			

\author			Simone Costantini
\date			04/04/2008
\version		1.0.0

\review			Emanuela Antognoli
\date			19/03/10
\notes			inserimento doppio backup stile arcadia se fallisce il pilotaggio della cella di standby

*/
void STB_CheckStandByCounter(void)
{
#if defined(SLEEP_MODE_ENABLE)
WAKEUPTYPE wakeUpRequest;
#endif /* defined(SLEEP_MODE_ENABLE) */
#if(STANDBY_EXCEPTION_PROCEDURE == 1)
	STB_Standby_Exception_Proc();
#endif /*STANDBY_EXCEPTION_PROCEDURE */	
	if(HarakiriRequired == HKM_FAST_SB_ON)
	{
		/* Andando in Off... */
		if(!(SWTC_IsStopped((SWTC_CountersType) STB_HARAKIRI_COUNTER_M)))
		{
			if(SWTC_IsExpired((SWTC_CountersType) STB_HARAKIRI_COUNTER_M))
			{
				/* TimeOut scaduto */
				if(HarakiriLowPower.Mode == HKM_SBY_NO_SLEEP_MODE)
				{
					/* non posso andare in sleep mode*/
					LIB_Reset_ForceColdReset();
				}
				else
				{
					/* posso andare in sleep mode*/
					if( HarakiriLowPower.Mode == HKM_SBY_HW_MODE)
					{
						/* Sby con cella di memoria -> Sby HW abilitato ->! */
						if(HarakiriLowPower.Hitpoints < HKM_N_MAX_HITPOINTS)
						{
							/* Entra qui solo le prime volta dopo la ripogrammazione del setting
							e in caso di RESET FREDDI con mancato backup */
							HarakiriLowPower.Mode= HKM_SBY_LOW_POWER_MODE;
							HarakiriLowPower.Hitpoints++;
							HarakiriLowPower.Req = HKM_LOW_POWER_REQ;

							/* Forza cancellazione pagina + Backup - da questo momento la modalità di
							Wait è fissa e sarà ribadita ad ogni salvataggio in Backup */
							(void)LIB_Backup_InvalidateBackupArea();
							LIB_Backup_ForceBackup();	/* backup dello stato di Wait */
						}
					}
					/* Sby Wait abilitato -> Wait Mode Request! */
#if defined(SLEEP_MODE_ENABLE) /* SS - FIXME MQ PA Manca la parte di piattaforma che gestisce lo sleep */
					wakeUpRequest.I2C = 1;
					Plt_Sleep_Request(&wakeUpRequest);		/* SS - TODO sostituire funzioni con PLT */
#endif				
				}
			}
		}
	}
} /* END: STB_CheckStandByCounter */

/**
Verifica disabilitazione Standby - disabilitazione Wait Mode
\return			codice che indica quale tipo di Standby è disabilitato

\notes:			in mancanza di RAM si usa la Flash
				(routine chiamata più volte che lavora su variabili locali)

\author			Valeria Catani
\date			14/12/2009
\version		1.0.0

\review 		Emanuela Antognoli
\date 			1/03/10
\notes			Hottima già si carica i flag in ram , quindi la gestionne è diversa.
*/
static uint8_t STB_CheckStandByFlags(uint8_t flag )
{
#if defined (CK_HOTTIMA) 
	return (TESTBIT(N_Application, flag)? TRUE:FALSE);
#elif defined (DW_DISHWASHER) || defined (CK_HOOD)
	return (TESTBIT(HWC_Flags.flags, flag)? TRUE: FALSE);	
#else
Loader_Struct flagData;	/* SbyLowPow 11.12.09 */
uint8_t standbyFlags = 0; /* Default: flag SBY_WAIT_DISABLE_FLAG e SBY_DISABLE_FLAG entrambi 0 */
	
	if(BasicLoader(APP_PTR_HW_CONFIG_DATA, FLAG_DISPL, &flagData) == S_OK)
	{
		standbyFlags = (OFFS_B(flagData.Data, 0) & SBY_FLAGS_BITMASK);
	}
	/* else non necessario perchè la validità del campo dati è verificata in Hound */

	if(TESTBIT(standbyFlags,flag) == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#endif
} /* END: STB_CheckStandByFlags */

/**
Init dello standby dopo backup

\param			none

\return			none

\notes:			

\author			Emanuela Antognoli
\date			17/03/2010
\version		1.0.0
*/
void STB_InitStandBy_AfterBackUp (void)
{
	if(STB_CheckStandByFlags((uint8_t) SBY_DISABLE_FLAG) == 0)
	{
		/* Standby abilitato */
		if(HarakiriLowPower.Mode == HKM_SBY_HW_MODE)
		{
			/* venivo da uno standby Hw quindi andato a buon fine -> resetto il numero di tentativi */
			HarakiriLowPower.Hitpoints = 0;
		}

		if(
			STB_getStatusAppliance()
#if(STANDBY_RESTORE_ONLY_IN_SLEEP == 1)
			&& (HarakiriLowPower.Mode == HKM_SBY_LOW_POWER_MODE)
#else
			/* questa condizione copre il caso di Hottima in cui
			lo stato di sleep non è presente: in questo caso se la
			cella è rotta la scheda non va in standby.
			Vale anche per gli altri prodotti, la cui condizione sopra,
			però, risulta ancora più stringente */
			&&( (HarakiriLowPower.Mode != HKM_SBY_NO_SLEEP_MODE)
#ifdef STANDBY_EXCEPTION_PROCEDURE
			|| (STB_Standby_Exception_Restore()  )
#endif
			 )
#endif /*STANDBY_RESTORE_ONLY_IN_SLEEP == 1  */
		)
		{
			/* cioè sono in PROGRAMMING o END : stati in cui lo Standby - Wait è ammesso */
			if(HarakiriLowPower.Req == HKM_LOW_POWER_REQ)
			{
				/* provengo dallo stato di Wait -> check comunicazione con UI */
				/* Init + Enable Standby Delay Counter per check su comunicazione UI */
#if ( SW_TIMER_NORM_MODE ==	FALSE)				
	        	Set_MediumCounter((SWTC_CountersType)STB_HARAKIRI_COUNTER_M,
	        	                (SWTC_PrescalerType)COUNTERS2_TBASE_100MS,
	        	                TOUT_PRE_STANDBY(Tab_SFTStandByData.Data));
#else
				Set_DownCounter((uint8_t) MEDIUM_COUNTER, (SWTC_CountersType) STB_HARAKIRI_COUNTER_M, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_TimeType) TOUT_PRE_STANDBY(Tab_SFTStandByData.Data));
#endif
				SWTC_Run((SWTC_CountersType) STB_HARAKIRI_COUNTER_M);
			}
		}
		else
		{
			/* RUN, PAUSE, DELAY, FAULT : stati in cui NON è ammesso lo Standby */
			HarakiriRequired = HKM_SB_RESET;
			HarakiriLowPower.Req = HKM_LOW_POWER_RESET;
		}
	}
	else
	{
		/* Standby disabilitato */
		HarakiriLowPower.Mode = HKM_SBY_OFF_MODE;
		HarakiriRequired = HKM_SB_RESET;
		HarakiriLowPower.Req = HKM_LOW_POWER_RESET;
	}
} /* END: STB_InitStandBy_AfterBackUp */
#endif /* (SBY_STANDBY_ENABLE == 1) */

/* *********** */
/* End of file */
/* *********** */
