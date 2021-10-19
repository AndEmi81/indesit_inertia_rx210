/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	CK_Safety.c
\ingroup	ApplControl

Il modulo contiene le funzioni di gestione delle azioni di sicurezza
*/

/**

\file    DW_Safety.c
\ingroup DW_Safety
\date    11/04/2008
\author  Valeria Catani
\version 1.0.0


\Par		History (titolo)

\n	to do 	


*/


/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"
#include "LIB_ZeroCrossingExp.h"
#include "PM_PulseCounterExp.h"
#if ( ITP_ZC_ENABLE== 1)

#include "OCUART_drv.h"

#endif /*ITP_ZC_ENABLE */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
/*void SFT_backupManagement(void);*/

static void SFT_7liter_Mgmt(void);
static void SFT_washResManagement(void);
static void SFT_WashPump_ResSafetymanagement(void);

static void SFT_drainPump_Mgmt(void);
void SFT_LoadEV_Mgmt(void);

static void SFT_salt_Activation(void);

static void SFT_dry_sincronizyng ( void) ;

#if ( ITP_ZC_ENABLE== 1)
static void SFT_checkGoodZC(void);
#endif /* ITP_ZC_ENABLE */

#if (VIRTUAL_SENSOR_ENABLE == 1)
void SFT_Sensor_EV_Unblock(void);
void SFT_drainPump_EVOFH_Mgmt(void);
#endif

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
Gestisce controlli e azioni di sicurezza.
 
 
\version		1.0.0
\date 			30/06/2006
\author			Valeria Catani

*/
void SFT_safetyManagement(void)
{

	/** Gestione Res Lavaggio */
	if (Res_Pos[LD_WASH_RES_IND] != NO_LD_POS)
	{/* Resistenza lavaggio presente */
		
		SFT_washResManagement();

		if( ( HWC_Flags.bit.bldc1 != 0 ) && ( Sft_WashPumpSpeedLow != 0 ) )
		{
			/* Presente pompa lavaggio BLAC e sicurezza abilitata */

			SFT_WashPump_ResSafetymanagement();
		}
	}

	if ( RegDev_Pos[LD_SALT_ACTIVATION_REG_IND] != NO_LD_POS)
	{
	  SFT_salt_Activation();
	}
	/** Gestione 7 litri */
	SFT_7liter_Mgmt();

	/** Gestione pompa scarico in stati stazionari (con p.p.) */
	SFT_drainPump_Mgmt();


	/** Gestione sblocco EV in caso di warning turbina */
	SFT_LoadEV_Mgmt();

	if (( Fan_Pos[LD_DRY_FAN_IND] != NO_LD_POS) && ( Res_Pos[LD_DRY_RES_IND] != NO_LD_POS) )
	{
		SFT_dry_sincronizyng();
	}

#if (VIRTUAL_SENSOR_ENABLE == 1)
	/* EA nuovo aprocedura di attivazione della pompa scarico in caso di turbina oUT of range*/
	if ( HWC_Flags.bit.vs_drain_presence  )
	{
		SFT_drainPump_EVOFH_Mgmt();
	}
	if ( HWC_Flags.bit.vs_wash_presence )
	{
		SFT_Sensor_EV_Unblock();
	}
#endif


#if ( ITP_ZC_ENABLE== 1)
	/* Verifica la corretta presenza dello ZC per controllare l'oscillatore */
	SFT_checkGoodZC();
#endif /* ITP_ZC_ENABLE */

}
/**/




static void SFT_7liter_Mgmt()
{
	if(SWTC_IsExpired((SWTC_CountersType)SLITER_DIRTYWATER_TIMEOUT_M_COUNTER))
	{
       SETBIT(Flags_1,JIF_FLAG1_7LITER_TIMER_EXPIRED_POS);
	}
	else
	{
	   CLRBIT(Flags_1,JIF_FLAG1_7LITER_TIMER_EXPIRED_POS);
	}
}



/**
Aggiorna la variabile che indica che la porta è aperta o che
è stata chiusa ma siamo in attesa che il timeout scada.
Gestisce il Timeout a porta chiusa.

\return 			none

\version		1.0.0
\date 			14/05/2008
\author			Valeria Catani

*/
void SFT_uPdoorStatus_Mgmt(void)
{

	/* Gestione counter e Gestione stato porta per uP */

	uPDoorStatus = UP_DOOR_CLOSE;

	if ( (GICriteria & MASK_B(GI_CR_DOOR_IND)) == GI_DOOR_CLOSE )
	{
		if (( GICriteriaOld & MASK_B(GI_CR_DOOR_IND ))== GI_DOOR_OPEN )
		{/* transizione porta aperta - porta chiusa */

			(void)Set_SmallCounter((SWTC_CountersType)SFT_DOOR_CLOSE_PAUSE_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)TOUT_DOOR_CLOSE);
		}

		if(SWTC_IsExpired((SWTC_CountersType)SFT_DOOR_CLOSE_PAUSE_S_COUNTER))
		{ /* porta chiusa e timeout scaduto */

			(void)SWTC_Stop((SWTC_CountersType)SFT_DOOR_CLOSE_PAUSE_S_COUNTER);
		}
		else
		{/* porta chiusa e timeout non scaduto */

			uPDoorStatus = UP_DOOR_WAIT;

			(void)SWTC_Run((SWTC_CountersType)SFT_DOOR_CLOSE_PAUSE_S_COUNTER);
		}
	}
	else
	{/* porta aperta */

		uPDoorStatus = UP_DOOR_WAIT;
	}

}

/* ************** */
/* Local Function */
/* ************** */
/**
- Gestisce l'attivazione della lettura del sensore sale:
viene sempre pilotato ad eccezione dello stato di OFF logico ( ciclo 0 fase 0)
La disablitazione del sensore è necessaria per il risparmio energetico, anche se non c'è lo standby
il sensore deve essere disabilitato in caso di standby "virtuale" ( solo visual)


\version		1.0.0
\date 			25/8/2010
\author			Emanuela Antognoli

-created
*/
static void SFT_salt_Activation( void )
{
	if ( ( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING) &&
	 (Interpreter_Data[0].phase == 0) )
	 /* standby virtuale o OFF logico*/
	{
		LoadsReq_New[RegDev_Pos[LD_SALT_ACTIVATION_REG_IND]].b = LOAD_OFF;

	}else
	{
		LoadsReq_New[RegDev_Pos[LD_SALT_ACTIVATION_REG_IND]].b = LOAD_ON;

	}

}

/**/
/* ************** */
/* Local Function */
/* ************** */



/**
Controlla condizioni di sicurezza per la resistenza lavaggio
(attivazione della resistenza lavaggio possibile solo con il pieno pressostato).

Viene verificato lo stato richiesto per la RES da uP/algoritmi processati in precedenza
nella stessa iterazione del main.

Se richiesta RES on può venir imposto stato di RES OFF e questo verrà effettivamente
applicato nella ActivateLoads. 

Il caso di Porta Aperta (disabilitazione timer) ricade nel caso di richiesta RES OFF
 
\version		1.0.0
\date 			23/08/2006
\author			Valeria Catani

\note
- è corretto spegnere la resistenza anche in caso di stato non valido sul pressostato?
				 
*/
static void SFT_washResManagement(void)
{
static uint8_t pressStatus_Old = 0;
uint8_t pressStatus;
uint8_t pressValid;

	if (LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b == RES_ON)
	{/* il controllo va fatto solo in caso di richiesta RES ON */
        if(HWC_Flags.bit.vs_wash_presence)
		{ /* è valido se sto alternando o è valido il criterio  */
		   pressValid = ( GET_BLDC_SPEED(BLDC_PumpReq_Old[WASH_BLDC_PUMP])>0);
		   				 
		   /* se il timer è spirato e la pompa gira*/
		}
		else
		{
		   pressValid = TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0;
		}

		if (!pressValid)
		{/* stato criterio non valido  - la res lavaggio viene spenta precauzionalmente */
		/* questo caso copre il criterio invalido senza che si stia alternando*/	

			LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_OFF;	
			if(HWC_Flags.bit.vs_wash_presence)
			{
				pressStatus_Old=0;
			}

		}
		else
		{/* stato criterio valido - test sul valore  */
#if (VIRTUAL_SENSOR_ENABLE == 1)
            if(HWC_Flags.bit.vs_wash_presence)
			{
			   if ( ( (uP_AltCtrl_Data.SV_invalid_4alt==TRUE)||(!SWTC_IsExpired((SWTC_CountersType)HIO_STABILITY_FILTER_VS)) ) &&( Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=5 )&&
			   ( Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]!=0 )) 
			   {/* se sta alternando o il criterio non è valido  attiva comunque la resistenza se deve farlo*/
			   		if ( MAKEWORD(BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VS_STATE_MSB], BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VS_STATE_LSB])==0)	
					{ 
						/* se il senore virtuale manda uno 0 la resistenza deve comunque spegnersi 
						NB: virtual_sensor_data non viene aggiornata mentre il c'è il filtro di stabilità attivo.*/
						pressStatus = 0;
					}else
					{	
						pressStatus= pressStatus_Old;	
						LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = LoadsReq_Old[Res_Pos[LD_WASH_RES_IND]].b;  /* new status = old status */ 
					}	
								
			   }else
			   {
			   
				   switch(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH])
				   {
					   default:/* comprende stato 3 e 5 */
					   case 0:
					   case 1:
					   	 /* stato pressostato = vuoto -> RES OFF forzatamente */
					     pressStatus = 0;
						 break;
					
					   case 2:
					     pressStatus = 1;
						 break;
					 
				   }
			   }

			}
			else
#endif  /*#if (VIRTUAL_SENSOR_ENABLE == 1)*/
			{
			   pressStatus = TESTBIT(GICriteria,GI_PRESS_SENS_CR_FULL_IND);
			}
			
			if ( pressStatus != pressStatus_Old )
			{/* cambiamento di stato pressostato - init timer stabilità  pressostato */

				if (pressStatus == 0)
				{/* stato attuale = vuoto */

					(void)Set_SmallCounter((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)TOUT_PRESS_EMPTY_FOR_RES_OFF);  
				}
				else
				{ /* stato attuale = pieno */

					(void)Set_SmallCounter((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)TOUT_PRESS_FULL_FOR_RES_ON); 
				}

				LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = LoadsReq_Old[Res_Pos[LD_WASH_RES_IND]].b;  /* new status = old status */
			}
			else
			{/* stato stabile pressostato */

				if( SWTC_IsExpired((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER)  )
				{/* conteggio stabilità concluso */

					if (pressStatus == 0)
					{ /* stato pressostato = vuoto -> RES OFF forzatamente */

						LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_OFF;    
					}
					/* else stato resistenza invariato (RES ON) */

					(void)SWTC_Stop((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER);
				}
				else
				{/* conteggio in corso */

					(void)SWTC_Run((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER);

					LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = LoadsReq_Old[Res_Pos[LD_WASH_RES_IND]].b;  /* new status = old status */ 
				}

			}

			pressStatus_Old = pressStatus;    /* aggiornamento old status press */ 
		}
	}
	else
	{/* se richiedo RES OFF, comprende anche caso di porta aperta */

		(void)SWTC_Stop((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER);
		if(HWC_Flags.bit.vs_wash_presence)
		 {
			 pressStatus_Old=0;
		 }
	}
	
}



#if ( ITP_ZC_ENABLE== 1)
/* Verifica la corretta presenza dello ZC per controllare l'oscillatore */
static void SFT_checkGoodZC(void)
{
bool_t goodZC;
static bool_t oldGoodZC = 1;
static uint8_t filterCounter = 0;
static uint8_t old_Sft_FilteredGoodZC = 1;


	/* valore corrente dello Zero Crossing*/
	goodZC = goodZCSignal();

	if( goodZC != oldGoodZC )
	{
		filterCounter = 0;
	}
	else
	{
		if( filterCounter < SFT_ZC_GOOD_FILTER_TIME )
		{
			filterCounter ++;
		}
		else
		{
			/* Stato filtrato Zero Crossing = stato corrente zero crossing */
			if (goodZC )
			{/* logica negata.*/
				Interpreter_Data[0].ZC_not_good = FALSE;
			}else
			{
				Interpreter_Data[0].ZC_not_good = TRUE;
			}
		}
	}

	oldGoodZC = goodZC;


	if( Interpreter_Data[0].ZC_not_good  )
	{
		/* Stato filtrato: Zero Crossing assente */

		/* Viene disabilitata la UART */

		OCUART_DisableSync();

		/*
		Viene resettato il buffer condiviso (info buffer), in modo da far intervenire la
		sicurezza sul motore
		*/
		{
		   uint8_t cnt;
			for(cnt=0; cnt< NR_OF_BLDC_PUMPS; cnt++)
			{
				memset(&BLDC_Pump_InfoBuffer[cnt][0], 0, IOM_EXTERNAL_0_INFOBUFFER_SIZE);
			}
		}
	}
	else
	{
		/* Stato filtrato: Zero Crossing presente */

		if( old_Sft_FilteredGoodZC )
		{
			/*
			Solo se provengo da Zero Crossing assente (==UART disabilitata) riabilito
			la UART, altrimenti starei sempre a resettare la periferica
			*/

			/* Riabilito la UART */

			OCUART_EnableSync();

		}
	}

	old_Sft_FilteredGoodZC = Interpreter_Data[0].ZC_not_good;
}
#endif /*ITP_ZC_ENABLE */

/**
Controlla condizioni di sicurezza per la resistenza lavaggio:
se la velocita' della pompa lavaggio e' minore di un parametro specificato
nel file di setting la resistenza lavaggio viene forzatamente spenta.

\return 			none
				
\version		1.0.0
\date 			13/04/2011
\author			Marco Quintini
*/
static void SFT_WashPump_ResSafetymanagement(void)
{
int16_t currentSpeed;
static uint8_t speedUnderThreshold = TRUE;

	currentSpeed = ((int16_t)(BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VCURR_MSB] << 8) + (int16_t)BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VCURR_LSB]);

	if( currentSpeed < 0 )
	{
		/* La velocità viene considerata in valore assoluto */
		currentSpeed = -currentSpeed;	
	}

	/* Stato della velocità della pompa rispetto alla soglia */
/*lint -e574
	currentSpeed non può essere negativa perchè c'è un controllo in precedenza
*/
	if( currentSpeed < Sft_WashPumpSpeedLow )
	{
		/* Velocità sotto soglia */
		speedUnderThreshold = TRUE;	
	}
	else if( currentSpeed > Sft_WashPumpSpeedHigh )
/*lint +e574 */
	{
		/* Velocità sopra soglia */
		speedUnderThreshold = FALSE;	
	}
	else
	{
		/* speedUnderThreshold invariato, siamo nella fascia dell'isteresi */
	}
	
	if( ( speedUnderThreshold == TRUE )
		 && ( TESTBIT(FltBLDCUnblock.flags,PUMP_TRES_OFF)	== 0) && ( FltWarning.bldc_low_voltage == 0) && ( FltWarning.bldc_OH == 0)
			 
		 )

	{
		/*
		Se lo stato della velocità è sotto soglia, la resistenza
		viene spenta per sicurezza, ma solo se non ne è gia richiesto
		lo spegnimento a causa di un'anomalia segnalata dal DSP.
		In questo modo, ad esempio in condizioni di Under Voltage,
		questa sicurezza non interviene e la routine FLT_bldcFaultLoadMgmt()
		può vedere LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_ON,
		altrimenti, dato che in Under Voltage il motore viene fermato,
		la suddetta routine vedrebbe sempre lo stato richiesto uguale a
		OFF a causa di questa routine di sicurezza.
		*/
		LoadsReq_New[Res_Pos[LD_WASH_RES_IND]].b = RES_OFF;    
	}
	
}


/**
Controllo delle condizioni (stato + PP) per innescare verifica
su acqua entrante e gestione eventuale scarico
(eseguito indipendentemente dalle condizioni della turbina)

\return 			none

\version		1.0.0
\date 			05/09/2008
\author			Valeria Catani

\ revisione QI 28.10.09
*/
static void SFT_drainPump_Mgmt(void)
{

  CLRBIT(Flags_2,FLAG2_SAFETY_DRAIN_PP_ON_POS);

  if( (Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING) ||
      (Interpreter_Data[0].completeLabel.s.label == LBL_END) ||
      (Interpreter_Data[0].completeLabel.s.label == LBL_DELAY)||
      (Interpreter_Data[0].completeLabel.s.label == LBL_FAULT) )
  { /* stati stazionari (con EV OFF) in cui va monitorato il pressostato */

    if ( (TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_PRESS_SENS_CR_FULL_IND) ) != 0)||
	HWC_Flags.bit.vs_drain_presence )
    {/* criterio pressostato valido */

      if ( (TESTBIT (GICriteria,GI_PRESS_SENS_CR_FULL_IND) != 0 )|| (  HWC_Flags.bit.vs_drain_presence &&  WaterInDW ) )
      {/* pieno pressostato -> attivazione pompa scarico + disabilitazione standby */

         /* pieno pressostato -> attivazione pompa scarico + disabilitazione standby */

        /* QI 28.10.09 */
        if (Sft_DrainPP.init_full == 0)
        {
          Sft_DrainPP.cnt = 0;

          if (Tab_SFT_EV.DataLenght == 2)
          {/* nuovi setting file : nuova gestione con ritardo */

            Sft_DrainPP.cnt = CNT_DRAIN_DELAY;

          }/* else contatore ritardo = 0, rimane la vecchia gestione */

          Sft_DrainPP.init_full = 1;
        }

        if (Sft_DrainPP.cnt > 0)
        {/*conteggio delay in corso : PUMP OFF */
          Sft_DrainPP.cnt--;
        }
        else
        {/* conteggio delay concluso : PUMP ON */
          DrainSftReq_New.b = LOAD_ON;

          Sft_DrainPP.init_empty = 1;
		  if ( (TESTBIT(GIValid, GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))&& (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN]==0 ))
		  {/* NB: 28/11/11 il valore del sensore virtuale della pompa scarico ha valori:0-2-3-5*/

     		 WaterInDW=0;
	      }
        }
        SETBIT(Flags_2,FLAG2_SAFETY_DRAIN_PP_ON_POS);

        /* END QI 28.10.09 */

      }
      else
      {/* vuoto -> attivo pompa per 1 sec in pi� per evitare On-OFF */

        if (Sft_DrainPP.init_empty == 1)
        {
          Sft_DrainPP.cnt = CNT_DRAIN_EMPTY;

          Sft_DrainPP.init_empty = 0;
        }

        if (Sft_DrainPP.cnt > 0)
  	    {
  	      	DrainSftReq_New.b = LOAD_ON;

  	      	SETBIT(Flags_2,FLAG2_SAFETY_DRAIN_PP_ON_POS);

  	    	if ( (TESTBIT(GIValid, GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_DRAIN))&& (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_DRAIN]))
		    {/* NB: 28/11/11 il valore del sensore virtuale della pompa scarico ha valori:0-2-3-5*/

     		   WaterInDW=0xFF;/* valore per far ripartire lo scarico a sensore , perch� ancora non vedo il vuoto */
	        }

			if ( (Sft_DrainPP.cnt == 1) && (WaterInDW!=0xFF) )
			{/*ultima iterazione - eseguo reset dati acqua caricata e turbina */

  				updateRegWaterLoaded(); /*?? vedere se va inserito anche questo!!*/
  				WaterInDW = 0;

				/* restart dati turbina: OK, una singola iterazione non altera il funzionamento delle
						altre procedure di Fault e Safety */
				WaterLoadedReset();

			}

  	      	Sft_DrainPP.cnt --;

  	    }
		/* else scarico concluso */

		Sft_DrainPP.init_full = 0;

      }
    }
    else
    {
      /* criterio non valido */

      Sft_DrainPP.init_full = 0; /* init solo per contatore in pieno, se rientrassi da stato invalido
                                    in vuoto scaricherei per il tempo aggiuntivo, ma senza essere passato per il pieno */
    }
  }
  else
  {/* RUN - PAUSE */
    Sft_DrainPP.init_full = 0;

  }

}



/**
Sincronizzazione ventola asciugatura e resistenza asciugatura.
Per evitare shock termici e permettere ai flussi d'aria di stabilizzarsi la resistenza asciugatura dovr�
accendersi solo qualche secondo dopo la ventola di asciugatura.
Se invece la ventola è spenta non può accendersi la resistenza.


-in ( stato ventola, parametro in setting( Dry_SfyTime_Res_On) )
-out stato resistenza


\version		1.0.0
\date 			1/9/2010
\author			Emanuela Antognoli

-created
*/

static void SFT_dry_sincronizyng ( void)
{

	if ( LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b == 0)
	{/* ventola spenta -> resistenza asciugatura spenta*/
		LoadsReq_New[Res_Pos[LD_DRY_RES_IND]].b	= RES_OFF;

		(void)SWTC_Stop((SWTC_CountersType)SFT_DRY_RES_DELAYACT_S_COUNTER);
		return;
	}

	if (( LoadsReq_Old[Fan_Pos[LD_DRY_FAN_IND]].b ==  LOAD_OFF ) &&  (LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b!=  LOAD_OFF ))
	{
		(void)Set_SmallCounter((SWTC_CountersType)SFT_DRY_RES_DELAYACT_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)Dry_SfyTime_Res_On);
	}

	if  (SWTC_IsExpired((SWTC_CountersType)SFT_DRY_RES_DELAYACT_S_COUNTER) )
    {/* conteggio transizione OFF -> ON concluso -> La resistenza asciugatura può accendersi se vuole */

       (void)SWTC_Stop((SWTC_CountersType)SFT_DRY_RES_DELAYACT_S_COUNTER);

    }
    else
    {	/*conteggio in corso*/
        /*  La Resistenza asciugatura deve rimanere spenta  */

        (void)SWTC_Run((SWTC_CountersType)SFT_DRY_RES_DELAYACT_S_COUNTER);

		LoadsReq_New[Res_Pos[LD_DRY_RES_IND]].b= RES_OFF;
		 	/*la resistenza deve accendersi, ma solo  tot secondi dopo la ventola.*/
	}
}





#if (VIRTUAL_SENSOR_ENABLE == 1)
/**
 attivazione della pompa di scarico nel caso di impulsi della turbina fuori range, nelle macchine con sensori virtuale.
la variabile New_safety_VS.drain_act viene attivata quando si rilevano inpulsi out of range e si avviano i tentativi di sblocco della turbina.
contremporaneamente si attua un duty cycle per la pompa scarico.


\return 			none

\version		1.0.0
\date 			26/08/13
\author			Emanuela Antognoli

\ revisione QI 28.10.09
*/
void SFT_drainPump_EVOFH_Mgmt(void)
{

	if( (Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING) ||
	(Interpreter_Data[0].completeLabel.s.label == LBL_END) ||
	(Interpreter_Data[0].completeLabel.s.label == LBL_DELAY) )
	{
	  if (  New_safety_VS.drain_act )
	  {
		  /* impulsi out of range rilevati */
		  	switch ( New_safety_VS.drainStatus)
			{
				case VS_DRAINSTATUS_IDLE:
				/* attivazione della pompa scarico per dilevazione impulsi out of range.*/
					(void)Set_SmallCounter((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)VS_DRAIN_TIMEON);
		 			 New_safety_VS.drainStatus=VS_DRAINSTATUS_ON;
					 (void)SWTC_Run((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER);
				break;
				case VS_DRAINSTATUS_ON:
						 SET_PUMP(LD_DRAIN_PUMP_IND, LOAD_ON);
						 SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],DRAIN_SFT_REQ_SPEED);
						 SET_BLDC_TIME(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],DRAIN_SFT_REQ_TIME);
				case VS_DRAINSTATUS_OFF:
					if ( SWTC_IsExpired((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER))
					{
						(void)Set_SmallCounter((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_10SEC, (SWTC_SCounterValue)VS_DRAIN_TIMEOFF);
						(void)SWTC_Stop((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER);
						 if (New_safety_VS.drainStatus++ >= VS_DRAINSTATUS_OFF)
						 {
						 	New_safety_VS.drainStatus= VS_DRAINSTATUS_IDLE;
						 }

					}else
		        	{
		   				(void)SWTC_Run((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER);

					}
				break;
			}
		}else
		{
			New_safety_VS.drainStatus= VS_DRAINSTATUS_IDLE;
			(void)SWTC_Stop((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER);
		}
	}else
	{
		New_safety_VS.drainStatus= VS_DRAINSTATUS_IDLE;
		(void)SWTC_Stop((SWTC_CountersType)SFT_EVOORDRAIN_COUNTER);
	}

}
#endif

/**
funzione per la determinazione di acqua in vasca a seguito dell'introduzuione dei sensori virtuali e della
assenza del pressostato .
Se si trova acqua in vasca si fanno i tentativi di sblocco della turbina e poi si va in fault.


-in ( stato sensore virtuale)
-out eventuale fault


\version		1.0.0
\date 			4/4/2012
\author			Emanuela Antognoli

*/

void SFT_Sensor_EV_Unblock ( void)
{

	if ( (LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_OFF )&& ReadVS_Data.ctrl_active)
	{/* non sto pilotando EV da uP, cioè non sto caricando acqua intenzionalmente , ma neanche sto facendo i tentativi di sblocco  dell'elettrovalvola*/

		if (TESTBIT(ActiveAlgorithmBmp[0],AAL_READVIRTUALSENSOR_ALG))
		{	/* questo dovrebbe essere vero solo in run , prima dell'end*/
			if ( TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+ReadVS_Data.pumpType)) && (ReadVS_Data.control_done==FALSE)  )
			{
				if( ( Virtual_Sensor_Value[ReadVS_Data.pumpType]==0 )||(Virtual_Sensor_Value[ReadVS_Data.pumpType]==5 ))
				{
					/* CONTROLLO TERMINATO PER ASSENZA DI ACQUA*/

					SftPulseWarn.vs_presence_h2o = FALSE;
					LoadEVCheck.status	=LOAD_EV_PILOT_INIT;

					ReadVS_Data.ctrl_active=FALSE;
				}else
				{

					SftPulseWarn.vs_presence_h2o =TRUE ;
				}
				ReadVS_Data.control_done=TRUE;
			}/* SE NON è VALIDO RIMANE IN ATTESA E NON FA NULLA */
			else
			if (SWTC_IsExpired((SWTC_CountersType)READ_VS_M_TIMER) && (ReadVS_Data.control_done==FALSE) )
			{

				SftPulseWarn.vs_presence_h2o = FALSE;
				LoadEVCheck.status	=LOAD_EV_PILOT_INIT;

				ReadVS_Data.ctrl_active=FALSE;

			}
		}else
		{
			if ( SftPulseWarn.vs_presence_h2o==FALSE)
			{/* uP terminato e non ha dato mai un valore valido del sensore */
				ReadVS_Data.ctrl_active=FALSE;
				ReadVS_Data.control_done=FALSE;

				LoadEVCheck.status =LOAD_EV_PILOT_INIT;

			}
		}
		if (SftPulseWarn.vs_presence_h2o)
		{
			FLT_pilotLoadEV_Mgmt(CHECK_EV_VS_FAULT_CTRL);
			if ( LoadEVCheck.status != LOAD_EV_PILOT_OK)
			{
				if ( FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO,FLT_CODE_LOAD_EV_KO))
				{/* già gestisco il fault*/
					if (PREFAULT_STEP != FaultMgmt_Data[0].step)
					{/* se non sono in prefault richiamo la macchina a stati finchè non va in prefault ( i tempi sono azzerati, tanto dipende dalla FLT_pilotLoadEV_Mgmt*/
						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);
					}else
					{/* sono in prefault , la richiamo solo quando ho finito i tentativi di sblocco */
						if(LoadEVCheck.status==LOAD_EV_PILOT_END)
						{/* dovrebbe andare in FAULT.*/
							FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);
							if ( (FAULT_STEP == FaultMgmt_Data[0].step))
							{
								ReadVS_Data.ctrl_active = FALSE;
							}
						}
					}
				}else
				{/* se non sto gestendo il fault , allora richiamo la macchina, magari il problema che la tiene occupata si ripristina.*/
					FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);

				}
			}else
			{
				FLT_ResetFaultDetectProc( FLT_CODE_LOAD_EV_KO, FLT_CODE_LOAD_EV_KO,(uint16_t)TOUT_RESET_PREFAULT );
				if ( FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO,FLT_CODE_LOAD_EV_KO)==FALSE)
				{/* già gestisco il fault*/
					
					ReadVS_Data.ctrl_active = FALSE;
				
				}
			}				
		}
	}else
	{
		ReadVS_Data.control_done=FALSE;
		SftPulseWarn.vs_presence_h2o= FALSE;
	}

}






/**
Gestisce tentativi di sblocco EV nel caso di warning su turbina
negli stati END-DELAY-PROGRAMMING

negli stati RUN-PAUSE il fallimento dei tentativi porta alla dichiarazione del Fault F02


La sequenza dei due tentativi può essere eseguita una sola volta

\return 			none
				
\version		1.0.0
\date 			05/09/2008
\author			Valeria Catani

*/

void SFT_LoadEV_Mgmt(void)
{
	/*selezione modalità in base allo stato */

	
	if( (Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING) ||
		(Interpreter_Data[0].completeLabel.s.label == LBL_END) ||
		(Interpreter_Data[0].completeLabel.s.label == LBL_DELAY)||
		(Interpreter_Data[0].completeLabel.s.label == LBL_FAULT) )  
	{ /* PROG - END - DELAY - FAULT : no fault */

		SftPulseWarn.mode = SFT_PULSE_WARN_SAFETY_MODE; 
	}
	else if ( (Interpreter_Data[0].completeLabel.s.label == LBL_RUN) || 
			  (Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE) )
	{/* RUN - PAUSE : dopo tentativi falliti c'� fault*/
		SftPulseWarn.mode = SFT_PULSE_WARN_FAULT_MODE;

		SftPulseWarn.pilot_disable = FALSE;  /* tentativi di sblocco EV (ri)abilitati */         

		FltWarning.outOfRange_EVKO = FALSE;	/* reset warning */
	}
	else
	{
		/* Do nothing */
	}
	
	if ( (LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_OFF )&&(ReadVS_Data.ctrl_active==FALSE))
	{/* non sto pilotando EV da uP, cioè non sto caricando acqua intenzionalmente
	MA NON STO FACENDO IL CHECH COL SENSORE VIRTUALE */
	

		if ( SftPulseWarn.status == SFT_PULSE_WARN_NO )
		{ /*stato stazionario di monitoraggio*/

			if ( PM_PulseCounter_GetWarning() == b1 )
			{ /*low frequency warning*/

				 /*config + init counter per monitoraggio warning su turbina*/
				(void)Set_SmallCounter((SWTC_CountersType)SFT_EVCHECK_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)TOUT_PULSE_WARN_FOR_EV_PILOT);				


				PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING);         /*  Reset di tutti i warning presenti*/

				LoadEVCheck.status = LOAD_EV_PILOT_OK;

				SftPulseWarn.status = SFT_PULSE_WARN_INIT;
			} 

		}

		if ( SftPulseWarn.status == SFT_PULSE_WARN_INIT )
		{
			(void)SWTC_Run((SWTC_CountersType)SFT_EVCHECK_S_COUNTER);

			if ( SWTC_IsExpired((SWTC_CountersType)SFT_EVCHECK_S_COUNTER))
			{/* end counting preliminare -> controllo warning low frequency persistente -> tentativi di sblocco */ 
				/* il controllo sul warning viene fatto alla fine 
				del tempo perch� la frequenza da monitorare è molto bassa (<2.5 Hz typ) */
				if ( PM_PulseCounter_GetWarning() == b1 )
				{
					if ( SftPulseWarn.pilot_disable == TRUE )
					{/* tentativi non consentiti : non eseguo sbloco */

						LoadEVCheck.status = LOAD_EV_PILOT_END;  
					}
					else
					{/* tentativi consentiti */

						LoadEVCheck.status = LOAD_EV_PILOT_INIT;  /* init tentativi di pilotaggio 
						                             gestiti da macchina a stati */

						PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_BOTH_WARNING); /* Reset di tutti i warning presenti*/

					}
					               
					SftPulseWarn.status = SFT_PULSE_WARN_EXE;
				}
				else
				{/* no warning o warning high freq */

					SftPulseWarn.status = SFT_PULSE_WARN_NO; 
				}
			}
		}

		if ( SftPulseWarn.status == SFT_PULSE_WARN_EXE )
		{/* esecuzione tentativi pilotaggio */

			/*SFT_pilotLoadEV_Mgmt(CHECK_EV_WARN_CTRL);*/
			FLT_pilotLoadEV_Mgmt(CHECK_EV_WARN_CTRL);
			  
			if ( LoadEVCheck.status == LOAD_EV_PILOT_END )
			{/* end procedura : esauriti tentativi di sblocco EV */

				if ( SftPulseWarn.mode == SFT_PULSE_WARN_FAULT_MODE )
				{/* viene dichiarato il Fault F02- sub 2*/

					/* dichiarazione Fault F02 - sub 2 */
					if ( SftPulseWarn.mode == SFT_PULSE_WARN_FAULT_MODE )
					{
						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_PULSE_LOW_RANGE);
					}
				}
				else
				{
					FltWarning.outOfRange_EVKO = TRUE;	
				}

				SftPulseWarn.status = SFT_PULSE_WARN_NO;		/*VERIFICA CHE SIA OK QUESTO STATO!*/

				SftPulseWarn.pilot_disable = TRUE; 	 /* tentativi disabilitati */
			}
			else if ( LoadEVCheck.status == LOAD_EV_PILOT_OK )
			{/* tentativo di pilotaggio OK - reset fault e prefault */

				FltWarning.outOfRange_EVKO = FALSE;
				/* reset PreFault F02 - sub 2 */
				FLT_ResetFaultDetectProc( FLT_CODE_LOAD_EV_KO, FLT_SUBCODE_PULSE_LOW_RANGE,(uint16_t)TOUT_RESET_PREFAULT );
				if (FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_PULSE_LOW_RANGE)==FALSE )
				{
					SftPulseWarn.status = SFT_PULSE_WARN_NO;
				}

			}

			else
			{
				/* Do nothing */
				if ( SftPulseWarn.mode == SFT_PULSE_WARN_FAULT_MODE )
				{
					if ( FLT_checkForCurrentFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_PULSE_LOW_RANGE))
					{/* già gestisco il fault*/
						if (PREFAULT_STEP != FaultMgmt_Data[0].step)
						{/* se non sono in prefault richiamo la macchina a stati finchè non va in prefault ( i tempi sono azzerati, tanto dipende dalla FLT_pilotLoadEV_Mgmt*/
							FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_PULSE_LOW_RANGE);
						}
					}else
					{/* se non sto gestendo il fault , allora richiamo la macchina, magari il problema che la tiene occupata si ripristina.*/
						FLT_manageFault(FLT_CODE_LOAD_EV_KO,FLT_SUBCODE_LOAD_EV_KO);

					}
				}
			}

		}
	}
	else
	{/* EV pilotata da uP per caricamento */

		SftPulseWarn.status = SFT_PULSE_WARN_NO;  

		SftPulseWarn.pilot_disable = FALSE;  /* tentativi di sblocco EV abilitati */ 
	}

}

/**/

/* *********** */
/* End of file */
/* *********** */
