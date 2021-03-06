/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	DW_Fault.c
\ingroup	ApplControl

Il modulo contiene le funzioni di gestione Dei FAULT
*/

/**

\file	  	DW_Fault.c
\ingroup	DW_Fault
\date		  14/04/2008
\version	01.00.00 
\author		Roberto Fioravanti - Valeria Catani

\Par		History (titolo)

*/


/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"


#include "BusCommPltf_cfg.h"
#include "OCI2C_drv.h"
#include "OCUART_drv.h"
#include "PM_PulseCounterExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */

 
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* **************************** */
/* Prototype of local functions */
/* **************************** */

static void FLT_PulseCounterStill_Reset(void);
static uint8_t FLT_Res_Get_Subcode ( uint8_t error);
static void  FLT_Reset_Circuit_Resistor_Fault(void);
static uint8_t FLT_IsCircuit_Resistor_Fault(void);
static void FLT_drainFaultMgmt(void);				/*F03*/
static void FLT_overflowFaultMgmt(void);			/*F01*/
static void FLT_probeFaultMgmt(void);				/*F04*/
static void FLT_resistorFaultMgmt(void);			/*F10 - F13*/
static void FLT_doorCircuitFaultMgmt(void);			/*F13 sub 0x10*/
static void FLT_heatToutFaultMgmt(void);			/*F08*/

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
void FLT_bldcFaultMgmt(void);
static bool_t FLT_checkForMotorFault(void);
static uint8_t GetAttempsTimeFault(uint8_t faultCode);
static uint8_t CheckExternalError(void);
void FLT_BLDCUnblock_Mgmt (uint8_t faultcode);
void FLT_bldcFaultReset(void);
static uint8_t Remap_Fault_step_2 (uint8_t subcode);
static void FLT_BLDCSpeedCheck(void);
static uint8_t FLT_Get_Motor_Timeout (uint8_t pump_type, uint8_t  displacement);
void FLT_bldcFaultLoadMgmt(void);
#endif

#if (ITP_ZC_ENABLE == 1)
static void FLT_checkZC(void);
#endif /* ITP_ZC_ENABLE */

static void FLT_Update_warning_proc( uint8_t  flag_pending, uint8_t code, uint8_t subcode);
static void FLT_VSFaultMgmt(void);			/* F01 subcode 02 + F15 */
static uint8_t  FLT_GetVS_timer(uint8_t displacement);
static void FLT_pressSwitchFaultMgmt(void);		/*F05*/
static void FLT_loadToutFaultMgmt(void);			/*F06*/
static void FLT_pulseCounterKOFaultMgmt(void);		/*F07*/
static void FLT_loadEVKOFaultMgmt(void);			/*F02*/
static uint8_t FLT_checkLoadEV(uint8_t mode);


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Gestisce la rilevazione dei fault.
 
\param [in] 	
\param [out] 	
 
\return 		
\warning		

\notes:

\version		1.0.0
\date 			09/07/2008
\author			Valeria Catani
*/
void FLT_faultManagement(void)
{
	/* Fault su Overflow (F01) */
	FLT_overflowFaultMgmt();
	/* Fault su resistenza e circuito resistenza (F10 - F13)  */
	FLT_resistorFaultMgmt();

	/* Fault su Timeout scarico (F03) */
	FLT_drainFaultMgmt();

	/* Fault su Sonda (F04) */
	FLT_probeFaultMgmt();

	/* Fault su circuito porta (F13 - sub 0x10)  */
	FLT_doorCircuitFaultMgmt();

#if (ITP_ZC_ENABLE == 1)
	/* Fault su mancanza segnale Zero Crossing (F13.0x50) */
	FLT_checkZC();
#endif /* ITP_ZC_ENABLE */

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT

 	if( (( HWC_Flags.bit.bldc1 ) || ( HWC_Flags.bit.bldc_drain ))
		&&
	 	( RegDev_Pos[LD_KILL_DSP_REG_IND] != NO_LD_POS ) )

 	{
 		/*
 		La gestione dei fault del DSP viene eseguita solo se sono presenti
 		carichi BLDC/BLAC e se ?? presente il DSP
 		*/

		/* Fault su BLDC (F11)  */
		FLT_bldcFaultMgmt();
		/*
		Gestione resistenza lavaggio durante anomalie su BLDC
		Deve stare dopo tutte le altre routine di fault (e dopo le sicurezze)
		per conoscere lo stato richiesto alla resistenza prima della
		verifica di FltBLDCUnblock.res_off.
		*/
		FLT_bldcFaultLoadMgmt();
 	}

#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */

 	/* Fault su Timeout riscaldamento (F08)  (F05 se pressostato in vuoto)*/
	if( Interpreter_Data[0].completeLabel.s.label != LBL_PAUSE )
	{
		FLT_heatToutFaultMgmt();
	}
	/* Fault sensori virtuali */
	if(( HWC_Flags.bit.vs_wash_presence != 0)&&( HWC_Flags.bit.vs_drain_presence != 0))
	{	/* BLDC/BLAC presente */
		FLT_VSFaultMgmt();
	}

	/* Fault su Anomalia Pressostato (F05) */
	FLT_pressSwitchFaultMgmt();

	/* Fault /Warning su Timeout Caricamento -rubinetto chiuso (F06) */
	if( Interpreter_Data[0].completeLabel.s.label != LBL_PAUSE )
	{
		FLT_loadToutFaultMgmt();
	}
	/* Fault su turbina bloccata (F07) */
	FLT_pulseCounterKOFaultMgmt();

	/* Fault su EV sfondata (F02) */
	FLT_loadEVKOFaultMgmt();

}
/**/



/**
Verifica se ?? attivo il fault del file di setting.

\param [in]		CurrentFault (global)

 
\return 		TRUE se il fault ??? attivo, FALSE altrimenti.

\version		1.0.0
\date			03/07/2006
\author			Marco Quintini
*/
bool_t FLT_checkForSettingFault(void)
{
	if( ( CurrentFault[0].code == FLT_CODE_SETTING_FILE )
		&& ( CurrentFault[0].subCode == FLT_SUBCODE_MAIN_BOARD_SETTING_FILE ) )
	{
		return TRUE;
	}	

  return FALSE;
    
}
/**/


/**
Routine di reset delle bitmap che gestiscono le azioni di sicurezza

\return
\version		1.0.0
\date			11/10/2006
\author			Valeria Catani

\in sospeso:

*/
void FLT_safetyBitmapReset(void)
{
	/* in dw non ci sono safety di sicurezza */
}
/**/

/**
QI 27.10.09 gestione finestra di buio su check turbina

Gestione Timer per Delay sul controllo dei warning su turbina e
su turbina bloccata

\return

\version		1.0.0
\date 			28/10/2009
\author  		Valeria Catani
*/

void FLT_PulseCounterCheckDelay_Mgmt(void)
{

	if (SWTC_GetRemainingTime((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER) > 0 )

	{	/* conteggio finestra di buio in corso : mantengo warning resettati e Init su controllo turbina bloccata */

		(void)SWTC_Run((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);

		PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING); /* Reset di eventuali warning presenti */

		(void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT); /* init funzione controllo turbina bloccata */

		CLRBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);
	}
	else
	{	/* conteggio finestra di buio concluso : parte la gestione di warning turbina e turbina bloccata */

		(void)SWTC_Stop((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);
	}

}

/**
Se il contatore dei decilitri caricati rimane costante per il tempo espresso nel dato
viene dichiarato un flag di Turbina bloccata che potr?? essere utilizzato dalla routine
di fault e nel JumpIF.

\return

\version		1.0.0
\date 			20/03/2008
\author  		Valeria Catani
*/

uint8_t FLT_CheckPulseCounterStill(uint8_t mode)
{
static uint16_t pulseLoaded_Old=0;

	if (mode == PULSE_COUNTER_STILL_INIT)
	{	/* INIT */

		if( SPV_isAutotestOrTestRunning() )
		{
			SWTC_SetMValue((SWTC_CountersType)PULSE_STILL_M_COUNTER, (SWTC_MCounterValue)TOUT_PULSE_COUNTER_STILL(Tab_LoadFault_Special.Data));
		}else
		{
			SWTC_SetMValue((SWTC_CountersType)PULSE_STILL_M_COUNTER, (SWTC_MCounterValue)TOUT_PULSE_COUNTER_STILL(Tab_LoadFault.Data));
		}


		SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);

	}

	/* Init & EXE */

	if( pulseLoaded_Old != PulseLoaded )
	{	/* impulsi in ingresso != 0 */

		pulseLoaded_Old = PulseLoaded;  /* aggiornamento impulsi in ingresso */

		FLT_PulseCounterStill_Reset(); /* QI - 02.12.09 reset Prefault, Flag e restart procedura */
	}
	else if ( TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) && (PM_PulseCounter_GetWarning() == b1) )		 /* QI - 19.10.09 */
	{	/* procedura di conferma : impulsi in ingresso in range non presenti, ma ci sono warning di impulsi under range */

		FLT_PulseCounterStill_Reset(); /* QI - 02.12.09 reset Prefault, Flag e restart procedura */
	}
	else
	{	/* quantit?? d'acqua in ingresso nulla (no impulsi in range, no impulsi under range) */

		if ( mode == PULSE_COUNTER_STILL_EXE )
		{	/* QI - 02.12.09 return TRUE e conteggio possibile solo in EXE */

			(void)SWTC_Run((SWTC_CountersType)PULSE_STILL_M_COUNTER);

			if ( SWTC_IsExpired((SWTC_CountersType)PULSE_STILL_M_COUNTER) )
			{/* end conteggio stazionariet?? acqua caricata */

				SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);

				return TRUE;
			}
		}
	}

	return FALSE;
}



/**
QI - 02.12.09
Reset Prefault turbina bloccata, reset Flag turbina KO e Restart contatore

\return

\version		1.0.0
\date 			02/12/2009
\author  		Valeria Catani
*/

void FLT_PulseCounterStill_Reset(void)
{
	/* vedo impulsi in ingresso -> reset flag turbina KO */
	CLRBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS);

	if (HWC_Flags.bit.vs_wash_presence)
	{
		FltWarning.load=FALSE;
	}
	CLRBIT(FltFlagsKO,FLAG_PULSE_KO_LOAD); /* QI - 02.12.09 restart procedura anzich?? solo restart contatore - ridondante */
	/* reset prefault se presente
	 * il prefault turbina bloccata non viene segnalato . al suo posto c'?? la segnalazione de flag 1 JIF_FLAG1_PULSE_COUNTER_KO_POS
	 *alla fine viene dichiarato il warning
	 */
    if( SPV_isAutotestOrTestRunning() )
	{
		SWTC_SetMValue((SWTC_CountersType)PULSE_STILL_M_COUNTER, (SWTC_MCounterValue)TOUT_PULSE_COUNTER_STILL(Tab_LoadFault_Special.Data));
	}else
	{
		SWTC_SetMValue((SWTC_CountersType)PULSE_STILL_M_COUNTER, (SWTC_MCounterValue)TOUT_PULSE_COUNTER_STILL(Tab_LoadFault.Data));
	}

}

/* questa funzione ?? custom per le applicazioni, quindi va definita qui
Prende da file di setting il tempo di rilevazione del prefault e fault per ogni codic e sottocodice*/
uint8_t FLT_getTimeout(uint8_t code, uint8_t subcode, Timeout_struct *timeoutdata)
{

uint8_t retval = TRUE;
	timeoutdata->config = 0;

		switch (code)
		{
			case FLT_CODE_MAIN_BOARD:

				switch (subcode)
				{
					case FLT_SUBCODE_FEEDBACK_KO:
					case FLT_SUBCODE_GENERIC_ERROR:
					case FLT_SUBCODE_RELE_RES_CC:
					case FLT_SUBCODE_RELE_RES_CA:

						if( SPV_isAutotestOrTestRunning() )
						{
							/* In Autotest e collaudo 3 tentativi */
							timeoutdata->prefaultTime = TOUT_RES_CIRCUIT_PREFAULT(Tab_MainBoardFault_Special.Data);
							timeoutdata->faultTime = TOUT_RES_CIRCUIT_FAULT(Tab_MainBoardFault_Special.Data);
							timeoutdata->config |= COUNTERS2_TBASE_1SEC;
						}
						else
						{
							timeoutdata->prefaultTime = TOUT_RES_CIRCUIT_PREFAULT(Tab_MainBoardFault.Data);
							timeoutdata->faultTime = TOUT_RES_CIRCUIT_FAULT(Tab_MainBoardFault.Data);
							timeoutdata->config |= COUNTERS2_TBASE_10SEC;
						}

					break;
#if ( ITP_ZC_ENABLE== 1)
					case FLT_SUBCODE_ZC_NOT_GOOD:

						if( SPV_isAutotestOrTestRunning() )
						{
							timeoutdata->faultTime = TOUT_ZC_CIRCUIT_AUTOTEST_FAULT;
							timeoutdata->config |= COUNTERS2_TBASE_1SEC;
						}else
						{
							timeoutdata->faultTime = TOUT_ZC_CIRCUIT_FAULT;
							timeoutdata->config |= COUNTERS2_TBASE_10SEC;
						}
						timeoutdata->prefaultTime = TOUT_ZC_CIRCUIT_PREFAULT;


					break;
#endif
					case FLT_SUBCODE_DOOR_OUT_OF_RANGE:
						if( SPV_isAutotestOrTestRunning() )
						{
							timeoutdata->faultTime = TOUT_DOOR_CIRCUIT_FAULT_AUTOTEST;
							timeoutdata->prefaultTime = TOUT_DOOR_CIRCUIT_PREFAULT_AUTOTEST;
							timeoutdata->config |= COUNTERS2_TBASE_1SEC;
						}else
						{
							timeoutdata->faultTime = TOUT_DOOR_CIRCUIT_FAULT;
							timeoutdata->prefaultTime = TOUT_DOOR_CIRCUIT_PREFAULT;
							timeoutdata->config |= COUNTERS2_TBASE_10SEC;
						}


					break;
					case FLT_SUBCODE_OVERFLOW_OUT_OF_RANGE:
						if( SPV_isAutotestOrTestRunning() )
						{
							timeoutdata->faultTime = TOUT_OVERFLOW_CIRCUIT_FAULT_AUTOTEST;
							timeoutdata->prefaultTime = TOUT_OVERFLOW_CIRCUIT_PREFAULT_AUTOTEST;
							timeoutdata->config |= COUNTERS2_TBASE_1SEC;
						}else
						{
							timeoutdata->faultTime = TOUT_OVERFLOW_CIRCUIT_FAULT;
							timeoutdata->prefaultTime = TOUT_OVERFLOW_CIRCUIT_PREFAULT;
							timeoutdata->config |= COUNTERS2_TBASE_10SEC;
						}
						break;
					case FLT_SUBCODE_BLDC_UNDER_VOLTAGE:
					case FLT_SUBCODE_BLDC_OVERHEATING:
					case FLT_SUBCODE_BLDC_COMM:
						/* questa funzione viene chiamata solo in autotest perch?? altrimenti vien egenerato un warning e comunque i tempo sono
						 * azzerato perch?? la macchina a stati viene forzata.
						 */
							timeoutdata->prefaultTime=0;
							timeoutdata->faultTime=0;
							/* se il fault viene gestito a tentativi, dal prefault serve solo una iterazione per andare in fault ,
							 * mentre con il timeolut 2 almeno 2.
							 */
							timeoutdata->config |= THEREATTEMPTFLT_MASK;
						break;

					default :
						if (subcode & 0x80)
						{
							timeoutdata->faultTime = FLT_Get_Motor_Timeout( LONIBBLE(FltBLDCUnblock.pumpandspeed ),2);
							timeoutdata->prefaultTime = 0;
							timeoutdata->config |= THEREATTEMPTFLT_MASK;
						}else
						{
							retval= FALSE;
						}
						break;

				}
				break;
				case FLT_CODE_RES_OPEN:

					if( SPV_isAutotestOrTestRunning() )
					{
						/* In Autotest e collaudo 3 tentativi */
						timeoutdata->prefaultTime = TOUT_RES_OPEN_PREFAULT(Tab_HeatFault_Special.Data);
						timeoutdata->faultTime = TOUT_RES_OPEN_FAULT(Tab_HeatFault_Special.Data);
						timeoutdata->config |= COUNTERS2_TBASE_1SEC;
					}
					else
					{
						timeoutdata->prefaultTime = TOUT_RES_OPEN_PREFAULT(Tab_HeatFault.Data);
						timeoutdata->faultTime = TOUT_RES_OPEN_FAULT(Tab_HeatFault.Data);
						timeoutdata->config |= COUNTERS2_TBASE_10SEC;
					}
					break;

				case FLT_CODE_DRAIN:
					/* i timeout sono gestiti nel uP drain. lo stato di prefault e fault in pratica viene forzato.*/
					if(subcode & 0x80 )
					{
						timeoutdata->faultTime = FLT_Get_Motor_Timeout(LONIBBLE(FltBLDCUnblock.pumpandspeed ),2);
					}
					else
					{
						
						timeoutdata->faultTime=0;						
					}
					timeoutdata->prefaultTime=0;
					timeoutdata->config |= THEREATTEMPTFLT_MASK;
					break;
				case FLT_CODE_PROBE:


					if( SPV_isAutotestOrTestRunning() )
					{
						timeoutdata->faultTime=TOUT_PROBE_FAULT(Tab_HeatFault_Special.Data);
						timeoutdata->prefaultTime=TOUT_PROBE_PREFAULT(Tab_HeatFault_Special.Data);
						timeoutdata->config |= COUNTERS2_TBASE_1SEC;

					}else
					{
						timeoutdata->faultTime=TOUT_PROBE_FAULT(Tab_HeatFault.Data);
						timeoutdata->prefaultTime=TOUT_PROBE_PREFAULT(Tab_HeatFault.Data);
						timeoutdata->config |= COUNTERS2_TBASE_10SEC;
					}
					break;
				case FLT_CODE_HEAT_TOUT:
				/* questa funzione viene chiamata solo in autotest perch?? altrimenti vien egenerato un warning e comunque i tempo sono
				 * azzerato perch?? la macchina a stati viene forzata.
				 */
					timeoutdata->prefaultTime=0;
					timeoutdata->faultTime=0;
					/* se il fault viene gestito a tentativi, dal prefault serve solo una iterazione per andare in fault ,
					 * mentre con il timeout 2 almeno 2.
					 */
					timeoutdata->config |= THEREATTEMPTFLT_MASK;
					break;

				case FLT_CODE_BLDC:
					timeoutdata->faultTime = FLT_Get_Motor_Timeout(LONIBBLE(FltBLDCUnblock.pumpandspeed ),2);
					timeoutdata->prefaultTime = 0;
					timeoutdata->config |= THEREATTEMPTFLT_MASK;
				break;
				case FLT_CODE_OVERFLOW:
					switch(subcode)
					{
						case FLT_SUBCODE_OVERFLOW:
							if( SPV_isAutotestOrTestRunning() )
							{
								timeoutdata->prefaultTime = TOUT_OVERFLOW_PREFAULT_AUTOTEST;
								timeoutdata->faultTime = TOUT_OVERFLOW_FAULT_AUTOTEST;

							}else
							{
								timeoutdata->prefaultTime = TOUT_OVERFLOW_PREFAULT;
								timeoutdata->faultTime = TOUT_OVERFLOW_FAULT;
							}
							timeoutdata->config |= COUNTERS2_TBASE_100MS;
						break;
						default:
							retval=FALSE;
						break;
					}
					break;
				case FLT_CODE_VS:
					if ( FLT_GetVS_timer(FLTN_INCONSISTENCY)>1)
					{	/* una iterazione serve per segnalare il prefault*/
						timeoutdata->faultTime = FLT_GetVS_timer(FLTN_INCONSISTENCY)-1;
					}else
					{
						timeoutdata->faultTime = 0;
					}
					timeoutdata->prefaultTime = 0;
					timeoutdata->config |= THEREATTEMPTFLT_MASK;
					break;
				case FLT_CODE_LOAD_EV_KO:

					if ( subcode == FLT_SUBCODE_EV_VIA_VS)
					{
						if ( FLT_GetVS_timer(FLTN_LOST_WATER)>1)
						{	/* una iterazione serve per segnalare il prefault*/
							timeoutdata->faultTime = FLT_GetVS_timer(FLTN_LOST_WATER)-1;
						}else
						{
							timeoutdata->faultTime = 0;
						}
					}else
					{
						timeoutdata->faultTime = 0;
					}

					timeoutdata->prefaultTime = 0;
					timeoutdata->config |= THEREATTEMPTFLT_MASK;

					break;
				case FLT_CODE_PRESS_SWITCH:
					if ( subcode !=FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT)
					{
						timeoutdata->prefaultTime=0;
						timeoutdata->faultTime=0;
						timeoutdata->config |= THEREATTEMPTFLT_MASK;
						break;
					}else
					{
						/* passa a prendere i tempi sotto del fault FLT_CODE_LOAD_TIMEOUT*/
					}
				case FLT_CODE_LOAD_TIMEOUT :
					if( SPV_isAutotestOrTestRunning() )
					{
						timeoutdata->faultTime =TOUT_LOAD_FAULT(Tab_LoadFault_Special.Data);
						timeoutdata->prefaultTime = TOUT_WARNING_LOAD(Tab_LoadFault_Special.Data);
					}else
					{
						timeoutdata->faultTime = TOUT_LOAD_FAULT(Tab_LoadFault.Data);
						timeoutdata->prefaultTime = TOUT_WARNING_LOAD(Tab_LoadFault.Data);
					}
					timeoutdata->config |= COUNTERS2_TBASE_1SEC;
					break;
				default:
					retval=FALSE;
					break;


		}
		return retval;
}

/**
Gestione fault resistenza (F10) e circuito resistenza (F13 - sub 1/2)
in base ai consistency

\param [out]


\return 		none.

\version		1.0.0
\date			21/07/08
\author		Emanuela Antognoli

*/

static void FLT_resistorFaultMgmt(void)
{
uint8_t subcode;


	switch (Consistency_Current[Res_Pos[LD_WASH_RES_IND]])
	{
		case CONSISTENCY_OFF_OK:

			/* fb ok -> reset condizioni per dichiarazione Fault F10 - F13 */
			FLT_Reset_Circuit_Resistor_Fault();

			FLT_ResetFaultDetectProc( FLT_CODE_RES_OPEN, FLT_SUBCODE_RES_OPEN,(uint16_t)TOUT_RESET_PREFAULT );

		break;

		case CONSISTENCY_OFF_ERROR_RELE_CC:
		case CONSISTENCY_ON_ERROR_RELE_CA:
		case CONSISTENCY_ON_FEEDBACK_KO_ERROR:
		case CONSISTENCY_OFF_FEEDBACK_KO_ERROR:
		case CONSISTENCY_ON_GENERIC_ERROR:
		case CONSISTENCY_OFF_GENERIC_ERROR:

			FLT_ResetFaultDetectProc( FLT_CODE_RES_OPEN, FLT_SUBCODE_RES_OPEN,(uint16_t)TOUT_RESET_PREFAULT );
			/* prendo solo la parte indicante il problema, cc, ca,..*/
			subcode = FLT_Res_Get_Subcode((Consistency_Current[Res_Pos[LD_WASH_RES_IND]] & 0x07));
			if (( FLT_checkForCurrentFault( FLT_CODE_MAIN_BOARD, subcode)==FALSE)&& (FLT_IsCircuit_Resistor_Fault()==TRUE ))
			{/* se sto gi?? monitorando il fault resistenza ma non era CC, rifaccio partire la macchina a stati.*/
				FaultMgmt_Data[0].faultCodes.code = FLT_CODE_MAIN_BOARD;
				FaultMgmt_Data[0].faultCodes.subCode = subcode;
				FaultMgmt_Data[0].step = ALARM_FLAG_STEP;

			}
			FLT_manageFault(FLT_CODE_MAIN_BOARD,subcode);
		break;
		case CONSISTENCY_OFF_ERROR_RES_OPEN:

			FLT_Reset_Circuit_Resistor_Fault();
			/* dichiarazione Fault F10 */
			FLT_manageFault(FLT_CODE_RES_OPEN,FLT_SUBCODE_RES_OPEN);

		break;
		case CONSISTENCY_ON_OK:
			/* fb ok -> reset condizioni per dichiarazione Fault F10 - F13 */
			FLT_Reset_Circuit_Resistor_Fault();

			/* disable counter per Fault F10  - sono in ON */
			if (FLT_checkForCurrentFault( FLT_CODE_RES_OPEN, FLT_SUBCODE_RES_OPEN)	)
			{	/* disable counter per Fault F10  - sono in ON: lo stato di rilevazione viene eventualmente sospeso */
				(void)SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
			}

		break;

		default:
		break;

	}

}
/* reset fault dovuto al circuito della resistenza */
static void  FLT_Reset_Circuit_Resistor_Fault(void)
{
	FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_FEEDBACK_KO,(uint16_t)TOUT_RESET_PREFAULT );
	FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_GENERIC_ERROR,(uint16_t)TOUT_RESET_PREFAULT );
	FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_RELE_RES_CC,(uint16_t)TOUT_RESET_PREFAULT );
	FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_RELE_RES_CA,(uint16_t)TOUT_RESET_PREFAULT );
}

static uint8_t FLT_Res_Get_Subcode ( uint8_t error)
{
	switch (error)
	{
		case CONSISTENCY_ERROR_RELE_CC:
			return FLT_SUBCODE_RELE_RES_CC;
		break;
		case CONSISTENCY_ERROR_RELE_CA:
			return FLT_SUBCODE_RELE_RES_CA;

		break;
		case CONSISTENCY_FEEDBACK_KO_ERROR:
			return FLT_SUBCODE_FEEDBACK_KO;
		break;
		default :
		/*CONSISTENCY_GENERIC_ERROR*/
			return FLT_SUBCODE_GENERIC_ERROR;
		break;

	}

	return(FLT_SUBCODE_GENERIC_ERROR);
}
/* indica se si sta monitorando un fault della resistenza dovuto al circuito o al Fb */
static uint8_t FLT_IsCircuit_Resistor_Fault(void)
{
	if ((FaultMgmt_Data[0].faultCodes.code = FLT_CODE_MAIN_BOARD) &&
			(FaultMgmt_Data[0].faultCodes.subCode  <= FLT_SUBCODE_GENERIC_ERROR))
	{
		return TRUE;
	}
	return FALSE;
}

/**
\name FLT_overflowFaultMgmt
\description: Manage of fault (F01) on overflow e circuito overflow
\return: None
\version:
\date: 19/11/2013
\author: Andrea Emili, SPES scpa
*/
static void FLT_overflowFaultMgmt(void)
{
	if (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_CR_OVFL_IND) ) != 0)
	{/* criterio overflow valido */

		if ( TESTBIT (GICriteria,GI_CR_OVFL_IND) != 0 )
		{/* overflow ON */
			FLT_manageFault(FLT_CODE_OVERFLOW, FLT_SUBCODE_OVERFLOW);
		}/* end if overflow ON */
		else
		{
			FLT_ResetFaultDetectProc( FLT_CODE_OVERFLOW, FLT_SUBCODE_OVERFLOW,(uint16_t)TOUT_RESET_PREFAULT );
		}
	}/* end criterio overflow valido */
	else
	{/*criterio overflow non valido*/
		if( FLT_checkForCurrentFault( FLT_CODE_OVERFLOW, FLT_SUBCODE_OVERFLOW) )
		{
			SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
		}
	}
	/* Fault su circuito pressostato (F13 - sub 0x30)  */

	if ( TESTBIT(FltGICriteria,GI_CR_OVFL_IND) )
	{/* lettura analogica out of range */

		FLT_manageFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_OVERFLOW_OUT_OF_RANGE);
	}
	else
	{
			/* Segnale filtrato di Zero Crossing presente */
		FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_OVERFLOW_OUT_OF_RANGE,(uint16_t)TOUT_RESET_PREFAULT );
	}
}

/**
Gestione fault per timeout di scarico (F03)
Il timeout di scarico ?? troppo strettamente connesso con il uP drain, quindi il timer viene usato quello del uP
mentre viene in qualche modo forzato lo stato di prefault e fault del

\param [out]


\return 		none.

\version		1.0.0
\date			20/11/13
\author		Emanuela Antognoli

*/
static void FLT_drainFaultMgmt(void)
{
	if ((TESTBIT(FltFlagsKO,WARNING_DRAIN_PREKO) == TRUE) ||(TESTBIT(FltFlagsKO,WARNING_DRAIN_KO) == TRUE))
	{/* il prefiltraggio del fault viene fatto dal uP drain perch?? ?? gi?? insito nel uP fare i tentativi di scarico. Di consegienza il tenpo per il
	prefault ?? nullo.*/
		FLT_manageFault(FLT_CODE_DRAIN,FLT_SUBCODE_DRAIN);

	}

	if (FLT_checkForCurrentFault( FLT_CODE_DRAIN, FLT_SUBCODE_DRAIN)==TRUE)
	{
		if ( FaultMgmt_Data[0].step == PREFAULT_STEP)
		{
			CLRBIT(FltFlagsKO,WARNING_DRAIN_PREKO);

		}
		if ( FaultMgmt_Data[0].step == FAULT_STEP)
		{
			CLRBIT(FltFlagsKO,WARNING_DRAIN_KO);
		}

	}
}



/**
Gestione fault su probe (F04)

\param [out]


\return 		none.

\version		1.0.0
\date			27/11/13
\author		Emanuela Antognoli

*/
static void FLT_probeFaultMgmt(void)
{
	if ( FltGITemp != 0 )
	{ /* rilevato valore out of range su sonda */
		if ( (Interpreter_Data[0].completeLabel.s.label == LBL_RUN) || (Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE) )
		{/* QI 28.10.09 gestione fault su sonda solo in RUN e PAUSE (sia a porta aperta che chiusa) */
			FLT_manageFault(FLT_CODE_PROBE,FltGITemp);
		}else
		{
			FLT_ResetFaultDetectProc( FLT_CODE_PROBE, FltGITemp,(uint16_t)TOUT_RESET_PREFAULT );
		}
	}else
	{
		FLT_ResetFaultDetectProc( FLT_CODE_PROBE, FLT_SUBCODE_PROBE_CC,(uint16_t)TOUT_RESET_PREFAULT );
		FLT_ResetFaultDetectProc( FLT_CODE_PROBE,  FLT_SUBCODE_PROBE_CA,(uint16_t)TOUT_RESET_PREFAULT );
	}
}


/* Fault su mancanza segnale Zero Crossing (F13.0x50)
 *
\param [out]


\return 		none.

\version		1.0.0
\date			28/11/13
\author		Emanuela Antognoli */
#if ( ITP_ZC_ENABLE== 1)
static void FLT_checkZC(void)
{
	if( Interpreter_Data[0].ZC_not_good  )
	{
		FLT_manageFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_ZC_NOT_GOOD);
	}
	else
	{
		/* Segnale filtrato di Zero Crossing presente */
		FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_ZC_NOT_GOOD,(uint16_t)TOUT_RESET_PREFAULT );
	}
}
#endif

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
/**
Gestione fault comunicazione con BLDC

\param [out]


\return 		none.

\version		1.0.0
\date			19/08/08
\rev 			28/11/13
\author			Emanuela Antognoli

*/
void FLT_bldcFaultMgmt(void)
{
uint8_t bldc_fault_warn;
uint8_t attemptsTime;
uint8_t BLDCSpeedNumMax_local;

#if ( ITP_ZC_ENABLE== 1)
	if(
		( ( LoadsReq_Old[RegDev_Pos[LD_KILL_DSP_REG_IND]].b == LOAD_OFF ) && ( TESTBIT(FltBLDCUnblock.flags,PUMPOFF)==0 ))
		||
		( Interpreter_Data[0].ZC_not_good )
	  )
#else
	if( ( LoadsReq_Old[RegDev_Pos[LD_KILL_DSP_REG_IND]].b == LOAD_OFF ) && ( TESTBIT(FltBLDCUnblock.flags,PUMPOFF)==0 ) )
#endif /* _VERIFY_ZC_GOOD_ */
	{
		/*
		Il DSP ?? spento per "cause naturali" o sto disabilitando la UART per problemi
		sullo Zero Crossing
		*/
		FLT_bldcFaultReset();
		FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_COMM,(uint16_t)TOUT_RESET_PREFAULT );
		FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_UNDER_VOLTAGE,(uint16_t)TOUT_RESET_PREFAULT );
	}
	else
	{

		if( ( CP_GetExternalBoardCommStatus()==TRUE )
			&&
			( TESTBIT(FltBLDCUnblock.flags,PUMPOFF)== 0)
			)
			/*
			Se ho comandato lo spegnimento del DSP, non considero comunque anche
			eventuali pacchetti che dovessero arrivare.
			*/

		{/* ricevuto pacchetto da BLDC -> comunicazione OK - gestione eventuale Fault/Warning BLDC  */

            uint8_t bldcIndex=BLDC_NOTHING;

			/* Analisi Fault BLDC */

            bldcIndex=CheckExternalError();

			BLDCSpeedNumMax_local = FLT_Get_Motor_Timeout(bldcIndex,5) & 0xF0;

			bldc_fault_warn=BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_MSB];

			if (  bldc_fault_warn != FLT_BLDC_CODE_NO_FAULT )
			{/* Presente Fault */
				/*test_debug[0]++;*/
				FltBLDCUnblock.pumpandspeed=SET_LONIBBLE(FltBLDCUnblock.pumpandspeed, bldcIndex);

				if(bldcIndex == WASH_BLDC_PUMP)
				{ /* spegniamo la resistenza lavaggio solo per il motore lavaggio */
				   /* QI - 11.11.09 - spegnimento precauzionale della resistenza */
					SETBIT(FltBLDCUnblock.flags,PUMP_TRES_OFF);
				}

				/*
				In DEA700 tutti i fault del DSP portano a tentativi di sblocco.
				Da verificare con Danilo e Alessio. MQ
				*/
				SETBIT(FltBLDCUnblock.flags,PUMP_STALLO);

				if( bldc_fault_warn == FLT_BLDC_CODE_F01 )
				{
					/* F01 DSP rimappato in F13 Main */
					bldc_fault_warn = FLT_CODE_MAIN_BOARD;
				}
				else if ( bldc_fault_warn != FLT_CODE_SETTING_FILE )
				{
					/*
					F09 DSP resta F09.
					Ogni altro codice (in teoria solo F02) ?? rimappato in F11
					MQ */
					/*
					LR 24/6/2011: l'equivalente di f11 per il motore lavaggio, diventa f14 per il motore drain
					EA: 1/3/12  l'equivalente di f11 per il motore lavaggio, diventa F03 su dea 700 secondo step
					*/
					bldc_fault_warn = (bldcIndex == WASH_BLDC_PUMP) ? FLT_CODE_BLDC : FLT_CODE_DRAIN;

					if ( Remap_Fault_step_2(BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_LSB]))
					{/* alcuni sottocodice del f02 diventano scheda*/
						bldc_fault_warn=FLT_CODE_MAIN_BOARD;
					}
				}
				else
				{
					/* Do nothing */
				}

				if ((TESTBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT) )&&FLT_checkForMotorFault())
				{/* ho cambiato pilotaggio della pompa o ?? cambiato il sottocodice rilevato dal dsp:devo aggiornare anche la macchina a stati dei fault*/

					if (FLT_checkForCurrentFault( bldc_fault_warn, BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_LSB]) )
					{
						CLRBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT);
					}else
					{
						FaultMgmt_Data[0].faultCodes.code=bldc_fault_warn;
						FaultMgmt_Data[0].faultCodes.subCode =	BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_LSB];
						/*FaultMgmt_Data[0].step =ALARM_FLAG_STEP*/
						/*FLT_ResetFaultDetectProc( FaultMgmt_Data[0].faultCodes.code, FaultMgmt_Data[0].faultCodes.subCode,(uint16_t)TOUT_RESET_PREFAULT );*/
					}
				}else
				{
					CLRBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT);
				}

				if (TESTBIT (FltBLDCUnblock.flags,UPDATE_FAULT)==0)
				{
					FLT_manageFault(bldc_fault_warn,BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_LSB]);

					FltBLDCUnblock.pumpandspeed=(BLDCSpeedNumMax_local)|(LONIBBLE(FltBLDCUnblock.pumpandspeed) );
					FltBLDCMotor_Tout = 0;
				}

				/*gestisce i tempi di spegnimento e pausa del dsp durante i tentativi*/
				FLT_BLDCUnblock_Mgmt(bldc_fault_warn);

				if ((FAULT_STEP == FaultMgmt_Data[0].step)&& FLT_checkForMotorFault())
				{/* se sono in fault non segnalo pi?? al dsp di fare tentativi */
					CLRBIT(FltBLDCUnblock.flags,PUMP_STALLO);
				}
			}
			else
			{/* No Fault BLDC - OK o Warning */
				/*test_debug[1]++;*/

				switch(BLDC_Pump_InfoBuffer[bldcIndex][BLDC_FAULT_LSB])
				{
					case FLT_BLDC_CODE_LOW_VOLTAGE_WARN:
						/* warning under voltage */
						FltBLDCUnblock.pumpandspeed=(BLDCSpeedNumMax_local)|(LONIBBLE(FltBLDCUnblock.pumpandspeed) );
						FltWarning.bldc_low_voltage = 1;	/* warning bldc_low_voltage */
						FltBLDCUnblock.wait_cnt=0;
					break;

					case FLT_BLDC_CODE_OH_WARN:
						/* warning overheating */
						FltBLDCUnblock.pumpandspeed=(BLDCSpeedNumMax_local)|(LONIBBLE(FltBLDCUnblock.pumpandspeed) );
						FltWarning.bldc_OH = 1;	/* warning bldc_low_voltage */
						FltBLDCUnblock.wait_cnt=0;
					break;

					default:
						/* No Fault - No Warning */

						/* controllo velocit??!=0 e spegnimento della resistenza per isteresi
						(se provengo da una situazione di perdita comunicazione ovvero di spegnimento preventivo)*/
						FltWarning.bldc_OH = 0;	/* warning bldc_low_voltage */
						FltWarning.bldc_low_voltage = 0;	/* warning bldc_low_voltage */
						FLT_BLDCSpeedCheck();

					break;
				}

			}

			/* reset prefault comunicazione, se presente */
			FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_COMM,(uint16_t)TOUT_RESET_PREFAULT );

			CP_ResetExternalBoardCommStatus();
			FltBLDCComm_Tout = 0;	/* abilitata init timeout comunicazione */
		}
		else
		{/* non ricevuto pacchetto da BLDC -> gestione Fault Comunicazione BLDC */
			if (FLT_checkForMotorFault())
			{/* sto monitorando il fault, potrei essere nello spegnimento forzato del dsp*/
				FLT_BLDCUnblock_Mgmt(FaultMgmt_Data[0].faultCodes.code);

			}
			if (TESTBIT (FltBLDCUnblock.flags,UPDATE_FAULT))
			{
				/* sto tenendo spento il dsp per tentativo di sblocco*/
				FltBLDCComm_Tout=0;
			}
			else
			{
				/*analizzo il fault comunicazione o rimango in attesa del pacchetto */
				if( FLT_checkForCurrentFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_COMM))
				{
					if (PREFAULT_STEP == FaultMgmt_Data[0].step )
					{
						attemptsTime=(FLT_Get_Motor_Timeout(WASH_BLDC_PUMP,0) & 0x7F);

					}else
					{
						attemptsTime=(FLT_Get_Motor_Timeout(WASH_BLDC_PUMP,1)& 0x7F);
					}
				}else
				{
					attemptsTime=10;
				}

				if ((FltBLDCComm_Tout >= attemptsTime) && (FLT_checkForMotorFault()==FALSE))
				{
				/* questo ?? un pre-filtraggio , perch?? la comunicazione tra dsp e main avviene in un tempo superiore al giro di main, quindi
				sicuramente tra un pacchetto e l'altro si cade qui. Se per?? non si vedono pacchetti per TOUT_BLDC_COMM_PREFAULT tempo allora si entra
				nella macchina a stati e si gestisce il fault.( siccome il tempo di filtraggio ?? stato fatto prima, il tempo di prefault ?? 0). */
					FLT_bldcFaultReset();
					FLT_manageFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_COMM);
					if ((PREFAULT_STEP == FaultMgmt_Data[0].step)&& FLT_checkForCurrentFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_COMM))
					{
						CP_UART_EnableComm();
					}

					FltBLDCComm_Tout = 0;
				}
				else
				{/* esecuzione procedura */
					FltBLDCComm_Tout++;
				}


			}
		}
		if (FltBLDCUnblock.wait_cnt >0 )
		{
			FltBLDCUnblock.wait_cnt--;

		}
	}

}

/*
Ritorna il valore dell'indice della pompa bldc da controllare per la presenza di fault

\param [out]


\return 		none.

\version	1.0.0
\date		29/11/2013
\author		Emanuela Antognoli

*/

static uint8_t CheckExternalError(void)
{
    uint8_t * pInfoWash;
	uint8_t * pInfoDrain;
	pInfoWash= (uint8_t *)&BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][0];
	pInfoDrain=(uint8_t *)&BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][0];
	 /* Ea 30/3/12 Il dsp riporta lo stesso fault rilevato su un motore nell'info buffer dell'altro  se si switcha il pilotaggio della pomp.
	 note: ce ne ?? sempre una attiva.
	 quindi basta considerare sempre il fault della pompa attiva .
	 Se nessuna delle due ?? attiva, rimane l'indicazione del fault sull'ultima attivata! */
	CLRBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT);

	if(GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP])>0)
	{/* bldc drain ?? pilotata */
	   if ( LONIBBLE(FltBLDCUnblock.pumpandspeed )!=DRAIN_BLDC_PUMP)
	   {/* se fosse nothing sarebbe da aggiornare comunque.*/
		   SETBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT);
		   CLRBIT(FltBLDCUnblock.flags,PUMP_STALLO);


		}
	   return DRAIN_BLDC_PUMP;


	}
	if(GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP])>0)
	{ /* bldc wash ?? pilotata */
	
		if ( LONIBBLE(FltBLDCUnblock.pumpandspeed )!=WASH_BLDC_PUMP)
	    {/* se fosse nothing sarebbe da aggiornare comunque.*/
			SETBIT(FltBLDCUnblock.flags,PUMP_CHANGE_PILOT);
			CLRBIT(FltBLDCUnblock.flags,PUMP_STALLO);

		}
	    return WASH_BLDC_PUMP;

	}
	/* entrambe spente*/
	if( ((LONIBBLE(FltBLDCUnblock.pumpandspeed ) != BLDC_NOTHING) && (LONIBBLE(FltBLDCUnblock.pumpandspeed ) < NR_OF_BLDC_PUMPS)) && ( FLT_checkForMotorFault()  || HINIBBLE(FltBLDCUnblock.pumpandspeed ) > 0  ) )
	{ /* continuiamo la gestione del fault che ?? in corso */

		return (LONIBBLE(FltBLDCUnblock.pumpandspeed ));
	}
	/* non c'?? nessun fault in corso */

	if(pInfoDrain[BLDC_FAULT_MSB] != CODE_NO_FAULT)
	{ /* bldc drain fault */
	   return DRAIN_BLDC_PUMP;
	}

	if(pInfoWash[BLDC_FAULT_MSB] != CODE_NO_FAULT)
	{ /* bldc wash fault */
	   return WASH_BLDC_PUMP;
	}
	if(pInfoDrain[BLDC_FAULT_LSB] == FLT_BLDC_CODE_LOW_VOLTAGE_WARN)
	{ /* bldc drain fault */
	   return DRAIN_BLDC_PUMP;
	}

	if(pInfoWash[BLDC_FAULT_LSB] == FLT_BLDC_CODE_LOW_VOLTAGE_WARN)
	{ /* bldc drain fault */
	   return WASH_BLDC_PUMP;
	}
	/* se non piloto niente, non ci sono fault non ci sono warning ,monitori la pompa di default*/
	return  DRAIN_BLDC_PUMP;

}
/**
Gestione tentativi di sblocco BLDC, tempo di spegnimento e pausa
\param [out]


\return 		none.

\version		1.0.0
\date			29/13/13
\author		Emanuela Antognoli

*/
void FLT_BLDCUnblock_Mgmt (uint8_t faultCode)
{


	uint8_t attemptsTime;

	attemptsTime = GetAttempsTimeFault (faultCode);

	if (FLT_checkForMotorFault())
	{

		if (FltBLDCMotor_Tout >=attemptsTime)
		{
			CLRBIT (FltBLDCUnblock.flags,UPDATE_FAULT);
			CLRBIT(FltBLDCUnblock.flags,PUMPOFF);

		}
		else
		{
			/* Procedura di spegnimento carichi */

			FltBLDCMotor_Tout++;
			FltBLDCUnblock.wait_cnt=FLT_Get_Motor_Timeout(LONIBBLE(FltBLDCUnblock.pumpandspeed ),4) * 10;
			SETBIT (FltBLDCUnblock.flags,UPDATE_FAULT);/* DSP spento */
			SETBIT(FltBLDCUnblock.flags,PUMPOFF);
		}
	}
	else
	{

		if (FltBLDCComm_Tout >=attemptsTime)
		{

			CLRBIT (FltBLDCUnblock.flags,UPDATE_FAULT);
			CLRBIT(FltBLDCUnblock.flags,PUMPOFF);


		}
		else
		{
			/* Procedura di spegnimento carichi */
			FltBLDCComm_Tout++;
			FltBLDCUnblock.wait_cnt=FLT_Get_Motor_Timeout(LONIBBLE(FltBLDCUnblock.pumpandspeed ),4) * 10;
			SETBIT (FltBLDCUnblock.flags,UPDATE_FAULT);/* DSP spento */
			SETBIT(FltBLDCUnblock.flags,PUMPOFF);
		}
	}


}

/**
Questa funzione frestituisce il tempo di attesa tra un tentativo e l'altro dei fault del motore.
\version		1.0.0
\date
\author			Emanuela Antognoli
*/
static uint8_t GetAttempsTimeFault(uint8_t faultCode)
{
uint16_t ret=0;

	if (FaultMgmt_Data[0].faultCodes.code == faultCode)
	{
		if (PREFAULT_STEP == FaultMgmt_Data[0].step)
		{
			ret=FLT_Get_Motor_Timeout(LONIBBLE(FltBLDCUnblock.pumpandspeed ),3);

		}else
		{/* per i fault del motore il tempo di rilevazione del prefault ?? immediato ( appena segnalato da dsp) */
			ret= 0;
		}

	}else
	{ /* se non sto considerando il fault che cerco le varibili interessate vengono resettate */
		FltBLDCComm_Tout=0;
		FltBLDCMotor_Tout = 0;
		CLRBIT (FltBLDCUnblock.flags,UPDATE_FAULT);
	}
	return ret;
}



static bool_t FLT_checkForMotorFault(void)
{
bool_t retval = FALSE;

	if(
		( FaultMgmt_Data[0].faultCodes.code == FLT_CODE_BLDC )

		||

		((FaultMgmt_Data[0].faultCodes.code==FLT_CODE_MAIN_BOARD)&& (FaultMgmt_Data[0].faultCodes.subCode & 0x80))
		||
		((FaultMgmt_Data[0].faultCodes.code==FLT_CODE_DRAIN)&& (FaultMgmt_Data[0].faultCodes.subCode & 0x80))
	  )
	{
		retval = TRUE;
	}


	return retval;
}



/**
Gestione reset fault comunicazione con BLDC

\param [out]


\return 		none.

\version		1.0.0
\date			29/11/13
\author		Emanuela Antognoli

*/
void FLT_bldcFaultReset(void)
{
	if ((FaultMgmt_Data[0].faultCodes.code==FLT_CODE_BLDC)||
		((FaultMgmt_Data[0].faultCodes.code==FLT_CODE_MAIN_BOARD)&& (FaultMgmt_Data[0].faultCodes.subCode & 0x80))
		||/* per il fault della pompa scarico */
		((FaultMgmt_Data[0].faultCodes.code==FLT_CODE_DRAIN)&& (FaultMgmt_Data[0].faultCodes.subCode & 0x80)))
		{

			FLT_ResetFaultDetectProc(FaultMgmt_Data[0].faultCodes.code, FaultMgmt_Data[0].faultCodes.subCode, (uint16_t) TOUT_RESET_PREFAULT);

		}
		CLRBIT (FltBLDCUnblock.flags,UPDATE_FAULT);
		FltBLDCComm_Tout=0;
		FltBLDCMotor_Tout = 0;

}


/*EA 5/3/12
remap del fault f02 del dsp come fault della scheda main.*/
static uint8_t Remap_Fault_step_2 (uint8_t subcode)
{

	if( ( subcode== 0x8A) ||( subcode== 0x8B)||( subcode== 0x8C))
	{
		return TRUE;
	}else
	{
		return FALSE;
	}


}


/**
QI 11.11.09 Controllo velocit?? BLDC per riaccensione resistenza
Viene applicato un filtro di 10 pacchetti consecutivi con velocit?? > 800 rpm

\param [out]


\return 		none.

\version		1.0.0
\date			09/11/09
\author		Valeria Catani

*/
static void FLT_BLDCSpeedCheck(void)
{

int16_t currentSpeed=0;
uint16_t targetSpeed=0;
uint8_t bldcIndex;
uint8_t BLDCSpeedTh_local, BLDCSpeedNumMax_local;

	bldcIndex = LONIBBLE(FltBLDCUnblock.pumpandspeed );
	/*lint -e574
	currentSpeed non pu?? essere negativa perch?? c'?? un controllo in precedenza
	*/
   BLDCSpeedTh_local = ( LONIBBLE(FLT_Get_Motor_Timeout(bldcIndex,5)) * 100 );
   BLDCSpeedNumMax_local = FLT_Get_Motor_Timeout(bldcIndex,5)& 0xF0;
	if (FltBLDCUnblock.wait_cnt >0 )
	{
		FltBLDCUnblock.pumpandspeed=(BLDCSpeedNumMax_local)|(LONIBBLE(FltBLDCUnblock.pumpandspeed) );
	}

	if ( HINIBBLE(FltBLDCUnblock.pumpandspeed )> 0 )
	{/* conteggio filtro per monitoraggio velocit?? in corso: indica che provengo da una situazione di anomalia */

		SETBIT (FltBLDCUnblock.flags,PUMP_TRES_OFF);
		/* richiesta spegnimento res : ?? gi?? cos??, ribadisco */


		if(bldcIndex < NR_OF_BLDC_PUMPS)
		{
		   currentSpeed = ((int16_t)(BLDC_Pump_InfoBuffer[bldcIndex][BLDC_VCURR_MSB] << 8) + (int16_t)BLDC_Pump_InfoBuffer[bldcIndex][BLDC_VCURR_LSB]);
		}

		if( currentSpeed < 0 )
		{
			currentSpeed = -currentSpeed;
		}

		if(bldcIndex < NR_OF_BLDC_PUMPS)
		{ /* Wash */
		  targetSpeed = MAKEWORD(BLDC_PumpReq_New[bldcIndex].data[0], BLDC_PumpReq_New[bldcIndex].data[1]);
		}


		if ( (targetSpeed == 0) || ( currentSpeed > BLDCSpeedTh_local ) )
/*lint +e574 */
		{/* isteresi RES OFF*/

			FltBLDCUnblock.pumpandspeed-=0x10;
		}
		else
		{/* ricevuto pacchetto con target velocit?? > 0 e velocit?? < soglia -> restart contatore */

			FltBLDCUnblock.pumpandspeed=(BLDCSpeedNumMax_local)|(LONIBBLE(FltBLDCUnblock.pumpandspeed) );
		}
	}
	else
	{/* filtro su velocit?? esaurito la resistenza pu?? rimanere accesa (se pilotata da microprogramma) */

		/* reset richiesta spegnimento RES */
		CLRBIT (FltBLDCUnblock.flags,PUMP_TRES_OFF);
		FltWarning.bldc_low_voltage = 0;	/* reset warning low voltage (rimane il memo per eventuale tout riscaldamento)*/

		FltWarning.bldc_OH = 0;	/* reset warning overheating */
		CLRBIT (FltBLDCUnblock.flags,PUMP_STALLO);
		/*rese presenti */
		FLT_bldcFaultReset();

	}
}

static uint8_t FLT_Get_Motor_Timeout (uint8_t pump_type, uint8_t  displacement)
{
	uint8_t ret=0;

	if( pump_type==DRAIN_BLDC_PUMP)
	{
		if( SPV_isAutotestOrTestRunning() )
		{/*la macchina a stati gestice i tentativi di sblocco cghe partono allo sacadere del tempo di fault*/

			ret= OFFS_B(Tab_ExtDevFaultDrainSpecial.Data,displacement);
		}else
		{
			ret=OFFS_B(Tab_ExtDevFaultDrain.Data,displacement) ;
		}
	}else
	{
		if( SPV_isAutotestOrTestRunning() )
		{/*la macchina a stati gestice i tentativi di sblocco cghe partono allo sacadere del tempo di fault*/

			ret= OFFS_B(Tab_ExtDevFault_Special.Data,displacement) ;
		}else
		{
			ret=OFFS_B(Tab_ExtDevFault.Data,displacement);
		}
	}
	return ret;
}

/**
Gestione resistenza durante anomalie su BLDC

\param [out]


\return 		none.

\version		1.0.0
\date			11/11/09
\author		Valeria Catani

*/
void FLT_bldcFaultLoadMgmt(void)
{

	if( Res_Pos[LD_WASH_RES_IND] != NO_LD_POS )
	{
		/* Resistenza lavaggio presente */

		if ((  FltWarning.bldc_low_voltage	== 1)|| (FltWarning.bldc_OH	== 1)||(TESTBIT(FltBLDCUnblock.flags,PUMP_TRES_OFF) ) )
		{
			/* Richiesta spegnimento resistenza a causa di anomalia su BLAC */

			if ( LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b == RES_ON )

			{
				/*
				E' in corso la valutazione del timeout di riscaldamento e la
				resistenza lavaggio viene spenta SOLO a causa delle anomalie
				segnalate dal BLAC, altrimenti sarebbe accesa.
				Solo in questo caso abilito il contatore, cos?? evito di conteggiare
				i "falsi" Under Voltage dovuti ad esempio all'apertura della porta
				o al ciclo messo in PAUSA.
				*/
				LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_OFF;

				if (FltHeatToutData.uP_exe ==TRUE )
				{/* sto eseguendo il uP HEAT*/
					if ( FltWarning.bldc_low_voltage	== 1)
					{
						(void)SWTC_Run((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER);
					}
					else
					{
						(void)SWTC_Stop((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER);
					}
					if ( FltWarning.bldc_OH	== 1)
					{	/* Richiesta spegnimento resistenza a causa di anomalia su BLAC */
						(void)SWTC_Run((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER);
					}
					else
					{
						(void)SWTC_Stop((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER);
					}
				}

			}/* resistenza spenta */
			else
			{
				(void)SWTC_Stop((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER);
				(void)SWTC_Stop((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER);
			}

		}/* non ci sono problemi su dsp */
		else
		{
			(void)SWTC_Stop((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER);
			(void)SWTC_Stop((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER);
		}
	}
}

#endif
/**
Gestione warning  su timeout riscaldamento (F08 - F05)

\param [out]


\return 		none.

\version		1.0.0
\date			15/07/08
\author		Valeria Catani

\ nota: dato che il timer ?? a minuti e la routine viene chiamata ogni 100 ms
\ entro nella routine di dichiarazione del prefault pi?? volte, fino a che il valore del timer ??
\ TOUT_HEAT_FAULT-TOUT_HEAT_PREFAULT. Sovrascriver?? pi?? volte la BBOX.

*/
void FLT_heatToutFaultMgmt(void)
{
uint8_t * ExtDevFaultParams;
uint8_t * ExtDevFaultParams_1;

	if( SPV_isAutotestOrTestRunning() )
	{
		ExtDevFaultParams= Tab_HeatFault_Special.Data;
		ExtDevFaultParams_1 = Tab_ExtDevFault_Special.Data;
	}
	else
	{
		ExtDevFaultParams = Tab_HeatFault.Data;
		ExtDevFaultParams_1 = ExtDevFaultParams=Tab_ExtDevFault.Data;
	}

	if ( FltHeatToutData.uP_exe  == FALSE )
	{/* nell'iterazione corrente non ?? stato eseguito uP Heat */

		/* reset prefault F05 sub 2 se presente e non sono in F05 */

		FLT_ResetFaultDetectProc(FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_HEAT, (uint16_t) TOUT_RESET_PREFAULT);
		FltHeatToutData.state = FLT_HEAT_TOUT_DISABLE;
		CLRBIT(FltFlagsKO,FLAG_BLDC_UNDVL);
		CLRBIT(FltFlagsKO,FLAG_BLDC_OH);
		CLRBIT(FltFlagsKO,FLAG_BLDC_HEATTOUT);
		CLRBIT(FltFlagsKO,FLAG_BLDC_PRESS_SWITCH);
		if(SPV_isAutotestRunning())
		{
			FLT_ResetFaultDetectProc(FLT_CODE_HEAT_TOUT,FLT_SUBCODE_HEAT_TOUT,(uint16_t)TOUT_RESET_PREFAULT );
			FLT_ResetFaultDetectProc(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_UNDER_VOLTAGE,(uint16_t)TOUT_RESET_PREFAULT );
			FLT_ResetFaultDetectProc(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_OVERHEATING,(uint16_t)TOUT_RESET_PREFAULT );

		}


	}

	switch (FltHeatToutData.state )
	{

		case FLT_HEAT_TOUT_INIT:
			(void)SWTC_SetMValue((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER, (SWTC_MCounterValue)(TOUT_HEAT_PREFAULT(ExtDevFaultParams)));


			/*
			Init contatore tempo Under Voltage al valore massimo (?? usato come se
			fosse un Up Counter, si controlla il tempo trascorso a partire da questa
			inizializzazione).
			Lo start/stop ?? gestito in FLT_bldcFaultLoadMgmt() in base allo stato
			della resistenza lavaggio.
			*/

			(void)SWTC_SetMValue((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER, (SWTC_MCounterValue)TOUT_HEAT_BY_UV_PREFAULT_THRESHOLD(ExtDevFaultParams));
			/* init contatore overheating */
			(void)SWTC_SetMValue((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER, (SWTC_MCounterValue)0x7FFF);


			FltHeatToutData.state ++;

			break;

		case FLT_HEAT_TOUT_EXE_P:
		case FLT_HEAT_TOUT_EXE_F:
			if ( uPDoorStatus == UP_DOOR_WAIT )
			{/* porta aperta o in attesa di timeout dopo la chiusura -> il uP va sospeso (res OFF)
				non posso far avanzare la procedura */

				/* disabilito timeout riscaldamento */
				(void)SWTC_Stop((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER);
				/* non serve fermare gli atri due timer, perch?? a porta aperta la resistenza ?? spenta e quindi si stoppano i contatori*/

			}
			else
			{/* porta chiusa */

				(void)SWTC_Run((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER);


				if( SWTC_IsExpired((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER) )
				{/* timeout prefault esaurito -> controllo pressostato per differenziare codici */
					(void)SWTC_SetMValue((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER, (SWTC_MCounterValue)(TOUT_HEAT_FAULT(ExtDevFaultParams)));
					/* Soglia di prefault/fault da Under Voltage (in secondi) */
					if( SWTC_IsExpired((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER) )
					{
						(void)SWTC_SetMValue((SWTC_CountersType)FLT_UNDER_VOLTAGE_RES_OFF_M_COUNTER, (SWTC_MCounterValue)TOUT_HEAT_BY_UV_FAULT_THRESHOLD(ExtDevFaultParams_1)); /* MQ introdurre define */
						if (FltHeatToutData.state == FLT_HEAT_TOUT_EXE_F)
						{

							SETBIT(Flags_3,JIF_FLAG3_WASH_UNDERVOLTAGE_WARN);
							FLT_warningSave(FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_UNDER_VOLTAGE,WRN_FAULT);
						}else
						{
							FLT_warningSave(FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_UNDER_VOLTAGE,WRN_PREFAULT);
						}
						SETBIT(FltFlagsKO,FLAG_BLDC_UNDVL);
						FLT_Update_warning_proc(FLAG_BLDC_UNDVL,FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_UNDER_VOLTAGE);

                        /*Setta il prefault?*/

					}else
					{
						if( (SWTC_TimeType)SWTC_GetElapsedTime((SWTC_CountersType)FLT_OVERHEATING_RES_OFF_M_COUNTER) >= TOUT_HEAT_BY_OH_PREFAULT_THRESHOLD(ExtDevFaultParams_1) )
						{	/* nel ciclo c'?? stato un warning bldc overheating (mancata accensione resistenza a causa di tale warning)*/
							if (FltHeatToutData.state  == FLT_HEAT_TOUT_EXE_F)
							{
								FLT_warningSave(FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_OVERHEATING,WRN_FAULT);
								SETBIT(Flags_3,JIF_FLAG3_WASH_OVERHEATING_WARN);

							}else
							{
								FLT_warningSave(FLT_CODE_MAIN_BOARD, FLT_SUBCODE_BLDC_OVERHEATING,WRN_PREFAULT);
							}
							SETBIT(FltFlagsKO,FLAG_BLDC_OH);
							FLT_Update_warning_proc(FLAG_BLDC_OH,FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_OVERHEATING);


						}
						else
						{		/* No warning BLDC */
							if ((TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0) || (HWC_Flags.bit.vs_wash_presence != 0) )
							{/* criterio pressostato valido o presenza di sensori virtuali per la pompa lavaggio  */

								if ( (TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 ) ||(HWC_Flags.bit.vs_wash_presence != 0) )
								{ /* Tout riscaldamento + pieno pressostato: RES pilotata -> problema imputato alla RES */
								  /*Tout riscaldamento + sensori virtuali ->problema imputato alla RES */
									/* prefault set  T_preFaultSet(FLT_CODE_HEAT_TOUT,FLT_SUBCODE_HEAT_TOUT); */

									if (FltHeatToutData.state == FLT_HEAT_TOUT_EXE_F)
									{
										FLT_warningSave(FLT_CODE_HEAT_TOUT, FLT_SUBCODE_HEAT_TOUT,WRN_FAULT);
										SETBIT(Flags_3,JIF_FLAG3_HEAT_TOUT_WARN);
									}else
									{
										FLT_warningSave(FLT_CODE_HEAT_TOUT, FLT_SUBCODE_HEAT_TOUT,WRN_PREFAULT);
									}

									SETBIT(FltFlagsKO,FLAG_BLDC_HEATTOUT);
									FLT_Update_warning_proc(FLAG_BLDC_HEATTOUT,FLT_CODE_HEAT_TOUT,FLT_SUBCODE_HEAT_TOUT);

								}else
								{
									SETBIT(FltFlagsKO,FLAG_BLDC_PRESS_SWITCH);
									FLT_manageFault(FLT_CODE_PRESS_SWITCH,FLT_SUBCODE_PRESS_SWITCH_HEAT);

								}
							}

						}

					}
					(void)SWTC_Stop((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER);
					FltHeatToutData.state++;

				}/* timeout scaduto */
				else
				{
					FLT_Update_warning_proc(FLAG_BLDC_HEATTOUT,FLT_CODE_HEAT_TOUT,FLT_SUBCODE_HEAT_TOUT);
					FLT_Update_warning_proc(FLAG_BLDC_UNDVL,FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_UNDER_VOLTAGE);
					FLT_Update_warning_proc(FLAG_BLDC_OH,FLT_CODE_MAIN_BOARD,FLT_SUBCODE_BLDC_OVERHEATING);
					if ( TESTBIT(FltFlagsKO,FLAG_BLDC_PRESS_SWITCH))
					{/* questo serve per forzare la macchina a stati dei fault . i tempi di fault e prefault per questo sono pari a 0, bisogna solo garantire la chiamata*/

						if (( FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_HEAT) )&& (FaultMgmt_Data[0].step==PREFAULT_STEP))
						{
							CLRBIT(FltFlagsKO,FLAG_BLDC_PRESS_SWITCH);
						}
						FLT_manageFault(FLT_CODE_PRESS_SWITCH,FLT_SUBCODE_PRESS_SWITCH_HEAT);
					}
				}

			} /* else porta chiusa */

			break;

		case FLT_HEAT_TOUT_END:

			break;

		case FLT_HEAT_TOUT_DISABLE:

			(void)SWTC_Stop((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER);

			break;

		default:
			break;

	}

	FltHeatToutData.uP_exe = FALSE; /* reset controllo su uP per iterazione in corso
									la var verr?? messa a TRUE dal passaggio per uP Heat */

}

/**
 * questa funzione serve per gestire i fault o gli warning
 * in autotest si deve andare in fault.
 *
 *


\param [in] 	FltWarning (global)

\return 		none
\warning

\notes:

\version		1.1.0
\date 			3/12/2013
\author			Emanuela Antognoli
*/
static void FLT_Update_warning_proc( uint8_t  flag_pending, uint8_t code, uint8_t subcode)
{
	if(SPV_isAutotestRunning())
	{
		if ( TESTBIT(FltFlagsKO,flag_pending))
		{/* questo serve per forzare la macchina a stati dei fault . i tempi di fault e prefault per questo sono pari a 0, bisogna solo garantire la chiamata*/

			if (( FLT_checkForCurrentFault( code, subcode)) && (FaultMgmt_Data[0].step == PREFAULT_STEP))
			{
				CLRBIT(FltFlagsKO,flag_pending);
			}
			FLT_manageFault(code,subcode);
		}
	}
}
/*
 * Fault su circuito porta  . se da setting non sono definiti i valori di out of range non viene mai rilevato.
 *
\param [out]


\return 		none.

\version		1.0.0
\date			28/11/13
\author		Emanuela Antognoli
*/
static void FLT_doorCircuitFaultMgmt(void)
{

	if ( TESTBIT(FltGICriteria,GI_CR_DOOR_IND) )
	{/* lettura analogica out of range */

		FLT_manageFault(FLT_CODE_MAIN_BOARD,FLT_SUBCODE_DOOR_OUT_OF_RANGE);
	}
	else
	{
			/* Segnale filtrato di Zero Crossing presente */
		FLT_ResetFaultDetectProc( FLT_CODE_MAIN_BOARD, FLT_SUBCODE_DOOR_OUT_OF_RANGE,(uint16_t)TOUT_RESET_PREFAULT );
	}

}
/**
 * Gestione Fault sensori virtuali ( incongruenza e legami tra sensore pompa lavaggio e scarico
  *


\param [in]

\return 		none
\warning

\notes:

\version		1.1.0
\date 			9/12/2013
\author			Emanuela Antognoli
*/

static void FLT_VSFaultMgmt(void)
{

	if (	TESTBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG))/* permette di fare il controllo una sola volta*/
	{

		if((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0)&&(TESTBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS) ) )
	    { /* la pompa lavaggio aveva visto il vuoto: errore VSFvs_ */
			if (TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))  )
			{
			 /* pompa scarico ha visto il vuoto */
				CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
				CLRBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS) ;
				if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN]==0)
				{

					SETBIT(Flags_3, JIF_FLAG3_VIRTUALSENSOR_END_CYCLE);

				}else
				{

				  	FltFlagsVS.VScheckinconsistency++;
					SETBIT(Flags_2, FLAG2_CLOGGED_FILTER);
					FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN);
					if (( FLT_checkForCurrentFault( FLT_CODE_VS, FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN)) && (FaultMgmt_Data[0].step != PREFAULT_STEP))
					{/* questa chiamata forza la macchina a stati ad andare direttamente in prefault . Se siamo gi?? in prefault viene
					chiamata una volta sola e se non ?? intervenuto un altro fault , dopo X Volte va in fault*/
						FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN);
					}

			    }
			}
		}else
		{

			 if((Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=0)&&(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=5)&&(TESTBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS) ) )
			 {
				 /* siamo in caricamento statico e ho visto il pieno*/
				if (TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN) ) )
				{
					CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
					CLRBIT(Flags_1,JIF_FLAG1_UP_TERMINATED_KO_POS) ;
					if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN]==0)
					{
						FltFlagsVS.VScheckinconsistency++;
						SETBIT(Flags_2, FLAG2_CLOGGED_FILTER);
						FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH);
						if (( FLT_checkForCurrentFault( FLT_CODE_VS, FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH)) && (FaultMgmt_Data[0].step != PREFAULT_STEP))
						{/* questa chiamata forza la macchina a stati ad andare direttamente in prefault . Se siamo gi?? in prefault viene
						chiamata una volta sola e se non ?? intervenuto un altro fault , dopo X Volte va in fault*/
							FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH);
						}

					}	 else
					{

						FltFlagsVS.warninglostwater++;
						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_EV_VIA_VS);
						if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_EV_VIA_VS)) && (FaultMgmt_Data[0].step != PREFAULT_STEP))
						{/* questa chiamata forza la macchina a stati ad andare direttamente in prefault . Se siamo gi?? in prefault viene
						chiamata una volta sola e se non ?? intervenuto un altro fault , dopo X Volte va in fault*/
							FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_EV_VIA_VS);
						}
				    }

				 }

			 }
			 else
			 if (TESTBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS) && (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0) )
			 {/* durante l'attivazione dell'algoritmo si ?? visto zero nel VS del lavaggio*/
				if (TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))  )
				{
					CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
					CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS) ;
					if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN]==0)
					{
						SETBIT(Flags_3, JIF_FLAG3_VIRTUALSENSOR_END_CYCLE);
					}else
					{
						FltFlagsVS.VScheckinconsistency++;
						FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN);
						if (( FLT_checkForCurrentFault( FLT_CODE_VS, FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN)) && (FaultMgmt_Data[0].step != PREFAULT_STEP))
						{/* questa chiamata forza la macchina a stati ad andare direttamente in prefault . Se siamo gi?? in prefault viene
						chiamata una volta sola e se non ?? intervenuto un altro fault , dopo X Volte va in fault*/
							FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN);
						}

					 }
				}
			}else
			{
				CLRBIT(Flags_1,JIF_FLAG1_ALGO_DIRTY_F_KO_POS) ;
			}
		}

	}



	if (FltFlagsVS.VScheckinconsistency>= FLT_GetVS_timer(FLTN_INCONSISTENCY))
	{/* questa ?? una sicurezza per cui , anche se la macchina a stati viene presa da un fault prioritaio, questo fault continua ad essere chiamato,
	fino a quando non viene resettato il valore.*/

		if (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==0)
		{
			FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN);
		}
		else
		{

			FLT_manageFault(FLT_CODE_VS,FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH);
		}
	}else
	{
		if (FltFlagsVS.VScheckinconsistency==0 )
		{
			FLT_ResetFaultDetectProc( FLT_CODE_VS, FLT_SUBCODE_EMPTY_WASH_FULL_DRAIN,(uint16_t)TOUT_RESET_PREFAULT );
			FLT_ResetFaultDetectProc( FLT_CODE_VS, FLT_SUBCODE_EMPTY_DRAIN_FULL_WASH,(uint16_t)TOUT_RESET_PREFAULT );
		}
	}
	if (FltFlagsVS.warninglostwater>= FLT_GetVS_timer(FLTN_LOST_WATER))
	{
		FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_EV_VIA_VS);

	}else
	{
		if (FltFlagsVS.warninglostwater==0 )
		{
			FLT_ResetFaultDetectProc( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_EV_VIA_VS,(uint16_t)TOUT_RESET_PREFAULT );
		}

	}

}
/* ritorna il numero di tentativi di controllo del fault sui sensoi virtuali, in caso di autotest e normale funzionamento */
static uint8_t  FLT_GetVS_timer(uint8_t displacement)
{
	uint8_t ret=0;
	uint8_t* VSFaultData;

	if(SPV_isAutotestRunning())
	{
		VSFaultData= Tab_VSFaultSpecial.Data;
	}
	else
	{
		VSFaultData= Tab_VSFault.Data;
	}
	if ( displacement)
	{
		ret=  HINIBBLE(OFFS_B(VSFaultData,0));
	}
	else
	{
		ret= LONIBBLE(OFFS_B(VSFaultData,0));
	}
	return ret;

}

/**
Gestione fault su anomalia pressostato (F05)

\param [out]


\return 		none.

\version		1.0.0
\date			10/07/08
\author		Valeria Catani

*/
static void FLT_pressSwitchFaultMgmt(void)
{

	if (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0)
	{/* criterio pressostato valido */

		if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
		{/* vuoto pressostato */

			if ( TESTBIT(FltFlagsKO,WARNING_LOADPP_KO) )
			{/* flag anomalia pressostato in uPLoad*/

			 /* il flag viene alzato nel uP load:
			  *  con caricamento a turbina, se ?? richiesto il controllo sul pieno pressostato  e vedo vuoto allora do allarme*/

				/* dichiarazione Fault F05 */
				FLT_manageFault(FLT_CODE_PRESS_SWITCH,FLT_SUBCODE_PRESS_SWITCH_LOAD);
			}else
			{
				FLT_ResetFaultDetectProc( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD,(uint16_t)TOUT_RESET_PREFAULT );
			}
		}else
		{
			FLT_ResetFaultDetectProc( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD,(uint16_t)TOUT_RESET_PREFAULT );
		}
	}
}


/**
Gestione fault su timeout carico (F06) e warning H20

\param [out]


\return 		none.

\version		1.0.0
\date			10/07/08
\author		Valeria Catani

*/
static void FLT_loadToutFaultMgmt(void)
{

	if ( FltLoadToutData.uP_exe == FALSE )
	{	/* nell'iterazione corrente non ?? stato eseguito uP Load */


		if (HWC_Flags.bit.vs_wash_presence!=0)
		{
			FltWarning.load = FALSE;
		}
		FltLoadToutData.state = FLT_LOAD_TOUT_DISABLE;
		FLT_ResetFaultDetectProc( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT,(uint16_t)TOUT_RESET_PREFAULT );
		FLT_ResetFaultDetectProc( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT,(uint16_t)TOUT_RESET_PREFAULT );
	}

	switch (FltLoadToutData.state)
	{
		case FLT_LOAD_TOUT_INIT:

			/* Il timeout viene gestito dalla macchina a stati, quindi la priorit?? di questo oggetto deve essere la pi?? alta */
		    if (HWC_Flags.bit.vs_wash_presence ==0)
			{
				FltWarning.load = FALSE;
			}
			FltLoadToutData.state = FLT_LOAD_TOUT_EXE;

			break;

		case FLT_LOAD_TOUT_EXE:

			if (uPDoorStatus == UP_DOOR_WAIT)
			{	/* porta aperta->
				il uP Load risulta sospeso, non posso far avanzare la procedura  */
				if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT)|| FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT))
						&&  (FaultMgmt_Data[0].step == PREFAULT_STEP))
				{/* fermo il timer per la rilevazione del fault.*/
					(void)SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
				}
			}
			else
			{	/* porta chiusa  */

				/*EA Va da se che se il pressostato non c'?? questo controllo viene disabilitato
				ma questa parte ?? importante per poter segnalare il warning H2o all'interfaccia ( tra le altre cose)*/
				if ((TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0)||
				 (  HWC_Flags.bit.vs_wash_presence  ))
				{

					/* criterio pressostato valido  o presenza di sensori virtuali*/

					if (( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )||
					((FltWarning.load == 0)&& HWC_Flags.bit.vs_wash_presence) )
					{	/* pieno pressostato */
						/* solo se vedo 0 devo dare allarme mancanza acqua.*/
						if (HWC_Flags.bit.vs_wash_presence)
						{
							FltLoadToutData.state =FLT_LOAD_TOUT_INIT;
						}else
						{
							FltWarning.load = FALSE;
						}
						FLT_ResetFaultDetectProc( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT,(uint16_t)TOUT_RESET_PREFAULT );
						FLT_ResetFaultDetectProc( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT,(uint16_t)TOUT_RESET_PREFAULT );

					}
					else
					{	/* vuoto pressostato  o presenza di sensori virtuali non validi ( nel  caso statico ??????)*/

						if ( (LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_ON) && (( !HWC_Flags.bit.vs_wash_presence  ) ||
						((FltWarning.load )&& HWC_Flags.bit.vs_wash_presence))	)
						{	/* caricamento in corso */
						/* EA qui  deve comunciare ad andarci perch?? sto facendo un caricamento dinamico a pieno sensore ( oltre al caso vecchio)
						e se scade il timeout vuol dire che non sono mai andata in pieno, quindi non c'?? acqua!*/
							if (HWC_Flags.bit.vs_wash_presence  )
							{
								FLT_manageFault(FLT_CODE_LOAD_TIMEOUT,FLT_SUBCODE_LOAD_TIMEOUT);
							}else
							{
								if (uP_Load_Data.status == LOAD_EXE_TIME)
								{	/* in corso caricamento statico a portata (tempo) */
									FltWarning.load = TRUE;
									/* dichiarazione Fault F06 */
									FLT_manageFault(FLT_CODE_LOAD_TIMEOUT,FLT_SUBCODE_LOAD_TIMEOUT);
								}
								else if (uP_Load_Data.status == LOAD_EXE)
								{/* in corso caricamento a turbina */
									/* dichiarazione Fault F05 - sub 3 */
									FLT_manageFault(FLT_CODE_PRESS_SWITCH,FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT);

								}else
								{
									/* Do nothing */
								}


							}
							if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT)|| FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT))
									&&  (FaultMgmt_Data[0].step == FAULT_STEP))
							{/* fermo il timer per la rilevazione del fault.*/
								FltLoadToutData.state = FLT_LOAD_TOUT_END;
								FltWarning.load = FALSE;

							}
						}
						else
						{	/* EV OFF */

							if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT)|| FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT))
									&&  (FaultMgmt_Data[0].step == PREFAULT_STEP))
							{/* fermo il timer per la rilevazione del fault.*/
								(void)SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
							}
						}
					}
				}
				else
				{	/* criterio pressostato non valido */
					if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT)|| FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT))
							&&  (FaultMgmt_Data[0].step == PREFAULT_STEP))
					{/* fermo il timer per la rilevazione del fault.*/
						(void)SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
					}
				}

			}

			break;

		case FLT_LOAD_TOUT_DISABLE:

			if (( FLT_checkForCurrentFault( FLT_CODE_LOAD_TIMEOUT, FLT_SUBCODE_LOAD_TIMEOUT)|| FLT_checkForCurrentFault( FLT_CODE_PRESS_SWITCH, FLT_SUBCODE_PRESS_SWITCH_LOAD_TOUT))
					&&  (FaultMgmt_Data[0].step == PREFAULT_STEP))
			{/* fermo il timer per la rilevazione del fault.*/
				(void)SWTC_Stop((SWTC_CountersType)MAIN_FAULT_MGMT_COUNTER_M);
			}

			break;

		case FLT_LOAD_TOUT_END:
			break;

		default:
			break;

	}

	FltLoadToutData.uP_exe = FALSE;   /* reset controllo su uP per iterazione in corso
									    la var verr?? messa a TRUE dal passaggio per uP Load */
}


/**
Gestione fault su turbina bloccata (F07)

QI - 23.10.09 : Fault F07 declassato a Warning

QI - 01.12.09 : Controllo su P.P. solo per procedura di conferma Fault (post Prefault e flag turbina Ko),
Set Prefault e Reset Prefault indip dal pressostato

\param [out]


\return 		none.

\version		1.0.0
\date			17/07/08
\author		Valeria Catani

*/
void FLT_pulseCounterKOFaultMgmt(void)
{
/* Facendo la verifica (ulteriore timeout) corro il rischio di terminare il caricamento prima di
aver dichiarato l'allarme (EV ON ?? condizione necessaria per F07) . Questo ovviamente per
caricamenti in cui il tempo confrontabile ?? con timeout turbina bloccata. */

/*Gestione porta aperta nel uP Load che utilizza le stesse routine */
/* Inoltre con porta aperta EV OFF -> non posso dichiarare fault */


	if( ( LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_ON ) &&
		( TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) != 0 ) )

	{	/* EV ON e Flag turbina KO settato da uP Load o LoadRestore -> Procedura di conferma Fault (post Prefault e Flag turbina KO)*/

		if ( TESTBIT(FltFlagsKO,FLAG_PULSE_KO_LOAD)  == FALSE )
		{	/* init procedura di conferma Fault */

			/* QI - 19.10.09 */
			PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);  /* Reset di eventuali warning presenti */

			(void)FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_INIT);

			/* flag che identifica una condizione di errore sul caricamento ->prefault FLT_CODE_PULSE_COUNTER(7)
			 * NB: il prefault viene salvato nella pila dei last prefault, ma non entra nella macchina a stati . il prefault corrente ?? segnalato da FltFlagsKO */
			SETBIT(FltFlagsKO,FLAG_PULSE_KO_LOAD);
		}
		else
		{	/* exe procedura di conferma Fault */

			if ( FLT_CheckPulseCounterStill(PULSE_COUNTER_STILL_EXE) == TRUE )
			{	/*end procedura di conferma fault */

				/* QI - 23.10.09 */
				/* Viene dichiarato un warning anzich?? un fault e come tale viene salvato immediatamente nei Last Fault */

				if ( FltWarning.pulseCounterKO == FALSE )
				{	/* first time : per non salvare il Last Fault pi?? volte perch?? rientra nella routine ad ogni iterazione */

					FLT_warningSave(FLT_CODE_PULSE_COUNTER, FLT_SUBCODE_PULSE_COUNTER,WRN_FAULT);

					FltWarning.pulseCounterKO =  TRUE;
				}

				/* dichiarazione Fault F07 */

			}
		}

		/* QI - 01.12.09 Durante la procedura di conferma Fault F07 in caso di vuoto pressostato, il contatore viene reinizializzato */
		/* Questo garantisce che il Fault verr?? dichiarato solo in caso di P.P., mentre il Set Prefault e ResetPrefault sono indipendenti dal pressostato */
		if( TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0 )
		{/* criterio pressostato valido */

			if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) == 0 )
			{	/* vuoto pressostato */

				CLRBIT(FltFlagsKO,FLAG_PULSE_KO_LOAD);   /* restart procedura */
			}
		}
		else
		{	/* criterio pressostato non valido - situazione transitoria? */
			(void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
		}
	}
	else
	{
		CLRBIT(FltFlagsKO,FLAG_PULSE_KO_LOAD);
	}
}



/**
Gestione fault su EV sfondata (F02) con tentativi di sblocco della stessa

\param [out]


\return 		none.

\version		1.0.0
\date			01/09/08
\author		Valeria Catani

*/
static void FLT_loadEVKOFaultMgmt(void)
{
uint8_t fltCheckEV;

	if ( (PM_PulseCounter_GetWarning() == NO_PULSE_COUNTER_WARNING) && (SftPulseWarn.status == SFT_PULSE_WARN_NO) && (ReadVS_Data.ctrl_active==FALSE))
	{/* Nessun warning dalla turbina - lettura in range E NON STO ESEGUENDO CONTROLLO PER SICUREZZA */

		if (TESTBIT(FltFlagsKO,FLAG_LOADEV_KO) == FALSE)
		{/* init procedura */

			/* config + init timer + contatore per EV KO */
			/* Come per DEA 601:
			Non essendoci warning turbina la frequenza ?? sicuramente in range,
			essendo tale frequenza a priori non impostabile al di sotto di 1 Hz
			viene fissato un tempo di campionamento per il contatore degli
			impulsi compreso fra 0,9 sec e 1 sec.
			*/

			(void)Set_SmallCounter((SWTC_CountersType)SFT_EVCHECK_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)TOUT_BASE_LOAD_EV_FAULT);
			if(SPV_isAutotestRunning())
			{
				LoadEVCheck.cnt = TOUT_LOAD_EV_FAULT(Tab_LoadFault_Special.Data);  /* il tempo da setting ?? il tempo delle pause per la rilevazione*/
			}else
			{
				LoadEVCheck.cnt = TOUT_LOAD_EV_FAULT(Tab_LoadFault.Data);
			}
			LoadEVCheck.status = LOAD_EV_PILOT_OK;

			FltLoadEVKOStatus = FLT_LOAD_EV_KO_INIT;

			SETBIT(FltFlagsKO,FLAG_LOADEV_KO);
			CLRBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);
		}
		else
		{/* esecuzione procedura */

			if ( LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_OFF )
			{/* EV lavaggio non pilotata da uP */

				if ( FltLoadEVKOStatus == FLT_LOAD_EV_KO_INIT )
				{
					fltCheckEV = FLT_checkLoadEV(CHECK_EV_PULSE_CTRL);
					CLRBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);

					if ( fltCheckEV == LOAD_EV_PAUSE_END )
					{ /* end pausa controllo acqua in ingresso con EV ON ->
						chiamata macchina a stati per gestione tentativi di sblocco */

						LoadEVCheck.status = LOAD_EV_PILOT_INIT;  /* init tentativi di pilotaggio
						gestiti da macchina a stati */
						SETBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);

						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);

						FltLoadEVKOStatus = FLT_LOAD_EV_KO_EXE;
					}
					else if ( fltCheckEV == LOAD_EV_OK )
					{/* non ho impulsi in ingresso */

						CLRBIT(FltFlagsKO,FLAG_LOADEV_KO);
						/* reset Prefault se presente */
						FLT_ResetFaultDetectProc( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_LOAD_EV_KO,(uint16_t)TOUT_RESET_PREFAULT );
					}
					else
					{
						/* Do nothing */
					}
				}

				if ( FltLoadEVKOStatus == FLT_LOAD_EV_KO_EXE )
				{/* esecuzione tentativi pilotaggio */
					FLT_pilotLoadEV_Mgmt(CHECK_EV_PULSE_CTRL); /* chiamata qui, solo se ho EV OFF */

					SETBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);

					if ( LoadEVCheck.status == LOAD_EV_PILOT_END )
					{/* esauriti tentativi di sblocco EV */

						/* dichiarazione Fault F02 */

						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);
						if ( FLT_checkForCurrentFault( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_LOAD_EV_KO)&& (FaultMgmt_Data[0].step==FAULT_STEP ))
						{
							FltLoadEVKOStatus = FLT_LOAD_EV_KO_END;
						}

					}
					else if ( LoadEVCheck.status == LOAD_EV_PILOT_OK )
					{/* tentativo di pilotaggio OK - reset fault */
						CLRBIT(FltFlagsKO,FLAG_LOADEV_KO);
						CLRBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);
						/* reset Prefault se presente */
						FLT_ResetFaultDetectProc( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_LOAD_EV_KO,(uint16_t)TOUT_RESET_PREFAULT );
					}
					else
					{
						/* Do nothing */
					}
				}
			}
			else
			{
				/* EV lavaggio pilotata ->restart */
				(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

				CLRBIT(FltFlagsKO,FLAG_LOADEV_KO);
				CLRBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT);
			}
		}

	}
	else
	{/* warning su turbina - entra la procedura di sicurezza - SONO MUTUAMENTE ESCLUSIVE */

		CLRBIT(FltFlagsKO,FLAG_LOADEV_KO);
	}

}
/**/


/**
Gestione macchina a stati per tentativi pilotaggio EV
\param [out]


\return 		none.

\version		1.0.0
\date			01/09/08
\author		Valeria Catani

*/
void FLT_pilotLoadEV_Mgmt(uint8_t mode)
{
	uint8_t checkEV;
	uint8_t EV_timer, EV_load;
	if(SPV_isAutotestRunning())
	{
		EV_timer = TOUT_LOAD_EV_FAULT(Tab_LoadFault_Special.Data);  /* il tempo da setting ?? il tempo delle pause per la rilevazione*/
		EV_load=TOUT_LOAD_EV_PILOT(Tab_LoadFault_Special.Data);
	}else
	{
		EV_timer = TOUT_LOAD_EV_FAULT(Tab_LoadFault.Data);
		EV_load=TOUT_LOAD_EV_PILOT(Tab_LoadFault.Data);
	}


	switch ( LoadEVCheck.status)
		{
			case LOAD_EV_PILOT_INIT:
			/*init + config counter per tentativo pilotaggio EV*/
				(void)Set_SmallCounter((SWTC_CountersType)SFT_EVCHECK_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)EV_load);
				if ( mode == CHECK_EV_VS_FAULT_CTRL )
				{/* vado avanti nello step solo se c'?? acqua , altrimenti no. Negli altri casi avanzo ogni volta che viene chiamata la funzione */
					if (SftPulseWarn.vs_presence_h2o)
					{
						LoadEVCheck.status = LOAD_EV_PILOT_1;
					}

				}else
				{
					LoadEVCheck.status = LOAD_EV_PILOT_1;
				}

			break;
			/* stati di tentativo di pilotaggio EV per sblocco */
			case LOAD_EV_PILOT_1 :
			case LOAD_EV_PILOT_2 :

				(void)SWTC_Run((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

				if ( SWTC_IsExpired((SWTC_CountersType)SFT_EVCHECK_S_COUNTER))
				{/* end counting EV pilot */
					/* init + config timer per pausa e check EV */
					(void)Set_SmallCounter((SWTC_CountersType)SFT_EVCHECK_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)TOUT_BASE_LOAD_EV_FAULT);

					if ( mode == CHECK_EV_WARN_CTRL )
					{
						PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING); /* Reset di tutti i warning presenti */

						LoadEVCheck.cnt = TOUT_PULSE_WARN_FOR_EV_PILOT;
					}
					else
					{
						if ( mode != CHECK_EV_VS_FAULT_CTRL)
						{
							LoadEVCheck.cnt = EV_timer;		/* il tempo da setting ?? il tempo delle pause per la rilevazione dell'ingresso acqua */
						}else
						{
							LoadEVCheck.cnt = 0;
						}

					}
					LoadEVCheck.status++;
				}

				LoadEVFltReq_New.b= EV_ON;

			break;
			case LOAD_EV_PILOT_PAUSE_1:
			case LOAD_EV_PILOT_PAUSE_2:

					checkEV = FLT_checkLoadEV(mode);

					if ( checkEV == LOAD_EV_PAUSE_END )
					{/* end controllo con EV OFF - */

						if (LoadEVCheck.status == LOAD_EV_PILOT_PAUSE_1)
						{
							/*init + config counter per tentativo pilotaggio EV*/
							(void)Set_SmallCounter((SWTC_CountersType)SFT_EVCHECK_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)EV_load);
						}

						LoadEVCheck.status++;
					}
					else if ( checkEV == LOAD_EV_OK )
					{
						LoadEVCheck.status = LOAD_EV_PILOT_OK;
					}
					else
					{
					/* Do nothing */
					}


			break;

			case LOAD_EV_PILOT_END:
				(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);
			break;

			case LOAD_EV_PILOT_OK:
			default:
				(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);
				if ( mode == CHECK_EV_VS_FAULT_CTRL)
				{
					ReadVS_Data.ctrl_active = FALSE;
				}
			break;

		}

}
/**
Gestione controllo su acqua in ingresso con EV OFF e turbina funzionante
\param [out]


\return 		none.

\version		1.0.0
\date			01/09/08
\author		Valeria Catani

	*/
uint8_t FLT_checkLoadEV(uint8_t mode)
{
static uint16_t pulseLoaded_Old=0;

	/* EV lavaggio non pilotata */

	(void)SWTC_Run((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

	if ( SWTC_IsExpired((SWTC_CountersType)SFT_EVCHECK_S_COUNTER) )
	{/* end conteggio base  */

		if ( mode == CHECK_EV_PULSE_CTRL )
		{/* controllo con turbina no warning */

			if ( LoadEVCheck.cnt > 0 )
			{/* conteggio timeout non esaurito -> campionamento impulsi */

				if (pulseLoaded_Old != PulseLoaded)
				{/* impulsi in ingresso != 0 */

					/* restart tout base */
					(void)SWTC_SetSValue ((SWTC_CountersType)SFT_EVCHECK_S_COUNTER,(SWTC_SCounterValue)TOUT_BASE_LOAD_EV_FAULT);

					LoadEVCheck.cnt--;  /* decremento contatore tout */

					pulseLoaded_Old = PulseLoaded;  /* aggiornamento impulsi */
				}
				else
				{/* no impulsi in ingresso -> no EV sfondata -> restart procedura */

					(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

					return LOAD_EV_OK;
				}
			}
			else
			{/* conteggio EV KO timeout esaurito */

				(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

				return LOAD_EV_PAUSE_END;
			}
		}
		else if ( mode == CHECK_EV_WARN_CTRL )
		{/* controllo in base a warning turbina */

			if ( LoadEVCheck.cnt > 0 )
			{/* conteggio in corso */
				/* restart tout base */
				(void)SWTC_SetSValue ((SWTC_CountersType)SFT_EVCHECK_S_COUNTER,(SWTC_SCounterValue)TOUT_BASE_LOAD_EV_FAULT);

				LoadEVCheck.cnt--;  /* decremento contatore tout */
			}
			else
			{/* end counting : controllo warning solo alla fine */

				(void)SWTC_Stop((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

				if ( PM_PulseCounter_GetWarning() == b1 )
				{/* low freq warning */

					return LOAD_EV_PAUSE_END;
				}
				else
				{
					return LOAD_EV_OK;
				}
			}
		}
		else
		{
			if ( mode == CHECK_EV_VS_FAULT_CTRL)
			{
				return LOAD_EV_PAUSE_END;
			}else
			{
				/* Do nothing */
			}
		}
	}

	return LOAD_EV_PAUSE_ON;
}

/**
 * EX FLT_Pump_Load_Mgmt
 Gestione timeout di caricamento in dinamico : timeout tra il P.P. statico
e il P.P. dinamico, gestione timeout dinamico anche con i sensori virtuali
_ gestisce il Ko ripristini JIF_FLAG1_LOAD_RESTORE_KO_POS

\param [out]


\return 		JIF_FLAG1_LOAD_RESTORE_KO_POS.

\version		1.0.0
\date		28/01/14
\author		Emanuela Antognoli
\note : non segnala nessun fault, setta solo il JIF_FLAG1_LOAD_RESTORE_KO_POS in base al quale i caricamenti sono bloccati
*/
void SFT_Timeout_4Loadrestore_Mgmt (uint8_t mode)
{
uint16_t ToutPumpLoad;

	if (mode == FLT_PUMP_LOAD_INIT)
	{	/* gestione in statico con PRESSOSTATO: in statico viene chiamata la funzione sempre con mode= INIT. in dinamico sempre EXE.
		 in statico , finch?? non vede pieno non fa nulla. una volta che vede pieno parte il timer MP_LOADTIMEOUT_REST_M_COUNTER
		 nell'attiva algoritmi , cos?? come in load restore ?? sempre EXE*/

		/* OCN SENSORI VIRTUALI:
		 * NELLA GESTIONE DELLA RIGENERE ?? SEMPRE RICHIAMATO COME init.
		 * NELLO STATICO e nel DINAMICO  viene richiamato sempre come EXE ( il timeout diventa comune sia nello statico che nel dinamico *brevetto BOSCH*
		 * viene per?? inizializzato durante la fase di "check della pompa " per  vedere se c'?? acqua o meno.
		 */
		if (( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )||( HWC_Flags.bit.vs_wash_presence ))
		{/* pieno pressostato -> init counter  */

			if ( Pump_Load_Data.init == FALSE )
			{	/* init counter : questa variabile viene inizializzata ad ogni inizio ciclo e ad ogni scarico */

				if (  HWC_Flags.bit.vs_wash_presence )
				{
					ToutPumpLoad=((Tab_HWCVS_data.DataLenght >3)?(OFFS_W(Tab_HWCVS_data.Data,3)& 0x7FFF):240);
				}else
				{
					if ( SPV_isAutotestOrTestRunning() == TRUE )
					{
						ToutPumpLoad = OFFS_B(Tab_LoadFault_Special.Data,8);
					}else
					{
						ToutPumpLoad = OFFS_B(Tab_LoadFault.Data,8);
					}
				}
				(void)SWTC_SetMValue((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER, (SWTC_MCounterValue)ToutPumpLoad);
				(void)SWTC_Stop((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
				Pump_Load_Data.init = TRUE;
			}
			else
			{	/* counting on */
				if ( TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )
				{/* con i sensori virtuali il contatore parte solo con elettrovalvola aperta*/
					(void)SWTC_Run((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
				}
				/* considerare scadere timeout e KO Restore anche in statico ???*/
			}
		}
	}
	else if (mode == FLT_PUMP_LOAD_EXE)
	{	/* gestione in dinamico (Load o Load Restore)*/

		if ( Pump_Load_Data.init == TRUE )
		{	/* counting ON. Il flag init = FALSE mi garantisce di non entrare accidentalmente
				nella gestione in dinamico, senza averla inizializzata in statico */

			if ( ((TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )&& (HWC_Flags.bit.vs_wash_presence == 0)) ||
			  ( ( LoadsReq_Old[EV_Pos[LD_WASH_EV_IND]].b == EV_OFF )  &&(HWC_Flags.bit.vs_wash_presence) ) )
			{	/* pieno pressostato in dinamico : disabilitazione timer , oppure elettrivalvola chiusa*/
				(void)SWTC_Stop((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
			}
			else
			{	/* vuoto in dinamico - counting on */

				if ( SWTC_IsExpired((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER) )
				{	/* end counting -> flag KO RESTORE */

					SETBIT(Flags_1,JIF_FLAG1_LOAD_RESTORE_KO_POS); /* set flag overflow ripristini */
				}
				else
				{
					(void)SWTC_Run((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
				}
			}
		}
	}
	 else
	 {
	     /* Do nothing */
	 }
}
/**/



/**
 * questa funzione serve per salvare nella struttura degli warning quello corrispomndente.
 * Come dea700 , gli warning vegono salvati dopo il tempo di FAULT, mentre al tempo di prefault vengono solo alzati i bit corrispondenti alla sagnalazione di errore.
 * il last fault dovrebbe essere salvato quando si fa in fault.
 *


\param [in] 	FltWarning (global)

\return 		none
\warning

\notes:

\version		1.1.0
\date 			27/06/2007
\author			Marco Quintini
*/
void FLT_warningSave(uint8_t code_To_Add, uint8_t subcode_To_Add, uint8_t where_to_save)
{
sft_fault_codes_t warningCode;

	/* Il warning viene salvato nei Last Fault se non ???? attivo l'Autotest */
	if(SPV_isAutotestRunning() == FALSE)
	{
		warningCode.code = code_To_Add;
		warningCode.subCode = subcode_To_Add;
		if(where_to_save)
		{
			FLT_lastFaultUpdate(warningCode, &MachineStatistics.lastFault[0]);
		}else
		{
			FLT_lastFaultUpdate(warningCode, &MachineStatistics.lastPreFault[0]);
		}
	}
}

void FLT_SafetyBitmap_Cstm_Exclusion(void)
{

}

void FLT_safetyBitmapStandard(void)
{

}
/* *********** */
/* End of file */
/* *********** */
