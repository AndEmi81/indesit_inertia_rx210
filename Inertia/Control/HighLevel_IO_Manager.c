/*
** **************************************************************************
** **************************************************************************
*/ 
/**
 
Gestisce elaborazioni su ingressi e uscite ad alto livello (applicazione).
Source file 
 
\file    	HighLevel_IO_Manager.c
 
\version 	1.0.0
\date    	09/02/2006
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

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "AnalogExp.h"
#include "InputManagerExp.h"
#include "ExternalOutputManagerExp.h"
#include "DigitalInputsExp.h"
#include "LIB_ZeroCrossingExp.h"

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
#include "ExternalOutputManagerExp.h"
#endif	/* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */
#include "BusCommPltf_cfg.h"
#include "PM_ConsistencyExp.h"
#include "PM_TurbiditySensor.h"
#include "PM_PulseCounterExp.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Dimensione in byte dei dati relativi ad un carico nella Output Map */
#define HIO_LOAD_DATA_SIZE_B						8

#ifndef _ENABLE_TOUT_BULK_OFF_IN_SETTING_
/* Delay da attendere per spegnere il DSP dopo lo spegnimento della BULK (x 100 msec) */
#define TOUT_BULK_OFF	30
#endif /* _ENABLE_TOUT_BULK_OFF_IN_SETTING_ */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
uint8_t HIO_trbProcessSmpl(uint16_t sample, uint8_t* p_samples);
uint8_t HIO_GetSparklingAidCriterionValue(GIMap_Struct const* pGI, uint16_t* cr_val);
uint8_t HIO_GIConv(GIMap_Struct const* pGI, uint16_t *pResult);
uint8_t HIO_GIScaleConv(GIMap_Struct const* pGI, uint16_t *pResult);
uint8_t HIO_GIGetCriterionDigitalTable(GIMap_Struct const* pGI, Loader_Struct *pTable);
void HIO_BulkAndKillDSP_Mgmt(void);
#if (VIRTUAL_SENSOR_ENABLE == 1)
uint8_t HIO_Check_VS_Valid(uint8_t aux_index);
void HIO_Check_VS_Stability(void);
static void set_limit4_state5(uint16_t target);
static void HIO_State5_SpeedControl(void);
#endif

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

uint8_t CriterionOldState;
uint8_t CriterionFilter[GI_CRITERION_NUM];

/* ******************* */
/* External variables  */
/* ******************* */
/* ***************** */
/* Exported function */
/* ***************** */


/**
Gestisce le elaborazioni sui carichi ad alto livello e l'attivazione.
 
\version		1.0.0
\date 			30/01/2008
\author			Valeria Catani, Roberto Fioravanti
 
\note	

*/
void HIO_loadsManagement(void)
{
uint8_t i;
uint8_t nLoads;
GIOM_LoadStruct load;
bool_t  setLoadCallEn;

	if ( Fan_Pos[LD_DRY_FAN_IND] != NO_LD_POS )
	{
		/* funzione per lo spunto iniziale delle ventole */
		Fan_GenericLoadMgmt() ;
	}

	if( HWC_Flags.bit.bldc_drain != 0 ) 
	{
		/* Presente pompa lavaggio BLAC */
		HIO_OverHeat_DrainPump_Control();

		/* Servir� una routine analoga anche per la pompa scarico MQ.*/
	}	


#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
	if( HWC_Flags.bit.bldc1 != 0 ) 
	{
			/*uint8_t prova_speed[2]={87,76};*/
		/* Presente pompa lavaggio BLAC */
		if (( uP_Load_Data.pump_act)&&(uPDoorStatus != UP_DOOR_WAIT) )
		/* uP carica cambia la velocità della pompa lavaggio*/
		{		
			SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],OFFS_B(uPLoad_setting_data.Data, 2)*PUMP_CTRL_BLDC_SPEED_MUL);
			SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP], OFFS_B(uPLoad_setting_data.Data,3)*PUMP_CTRL_BLDC_RAMP_TIME_MUL);
		}
#if (VIRTUAL_SENSOR_ENABLE == 1)
		if (( dirty_foam_Data.reduc_speed)&&(uPDoorStatus != UP_DOOR_WAIT))
		{/* riduzione della velocit� pompa lavaggio per presenza di sporco*/
			uint16_t speed;
			speed=((GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP])/PUMP_CTRL_BLDC_SPEED_MUL)* OFFS_B(dirty_foam_setting_data.Data, dirty_foam_Data.reduc_speed))/100  ;	
			SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],(speed*PUMP_CTRL_BLDC_SPEED_MUL) );
		}

		if (HWC_Flags.bit.vs_wash_presence != 0) 
		{
			HIO_State5_SpeedControl();
		}
#endif
		/* Servirà una routine analoga anche per la pompa scarico MQ.*/
	}	

	if((HWC_Flags.bit.bldc_drain != 0) && (HWC_Flags.bit.bldc1 != 0))
	{ 
	  
    /* LR 25/05/2011: controllo per evitare che entrambe le pompe siano accese */
		if((GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP])!=0)||(DrainSftReq_New.b == LOAD_ON))
		{ /* LR 25/5/2011: se la pompa scarico è attiva, la pompa lavaggio s'adda stuta' */
#if (VIRTUAL_SENSOR_ENABLE == 1)
			if (HWC_Flags.bit.vs_wash_presence != 0) 
			{/* l'algoritmo di riduzione della velocità deve essere azzerato  quando si attiva la pompa scarico*/		
				State5.max_speed_value =0xFFFF;
			}
#endif
	
			if ( DrainSftReq_New.b == LOAD_ON)
			{
				SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP],DRAIN_SFT_REQ_SPEED);
				SET_BLDC_TIME(BLDC_PumpReq_New[DRAIN_BLDC_PUMP], DRAIN_SFT_REQ_TIME);
			}
			SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP], 0);
			SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP], 0);
				
		}
		
	}

    if( (HWC_Flags.bit.bldc_drain != 0) && (HWC_Flags.bit.vs_wash_presence != 0)  && (HWC_Flags.bit.vs_drain_presence != 0) )
	{
		 
		if((GET_BLDC_SPEED(BLDC_PumpReq_Old[DRAIN_BLDC_PUMP])==0) && (GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP])>0) )
		{ /* qui impostare il timer per il calcolo dell'attivazione della pompa drain */
			
		 	SETBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);			 
			 
		}
		else
		{
		
			if (GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP])==0 )
			{
				CLRBIT(Flags_2, FLAG2_VIRTUALSENSOR_TEST_STARTED_FLAG);
			}
			
		}	
	}




#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */

	if( ( ( HWC_Flags.bit.bldc1 != 0 ) || ( HWC_Flags.bit.bldc_drain != 0 ) )
		&&
 	 	( RegDev_Pos[LD_KILL_DSP_REG_IND] != NO_LD_POS )
 	  )
 	{
		/* Presenti pompa lavaggio e/o pompa scarico BLAC e DSP */
		/*
		Deve stare dopo HIO_OverPower_Pump_Control() per avere la velocità aggiornata
		della pompa (più che altro per pulizia, in realtà guarda solo se è accesa o
		spenta, e questo non viene modificato da HIO_OverPower_Pump_Control().
		*/
		HIO_BulkAndKillDSP_Mgmt();
	}
#if (VIRTUAL_SENSOR_ENABLE == 1)
 	if(HWC_Flags.bit.vs_wash_presence)
 	{
  		HIO_Check_VS_Stability();
 	}	
#endif
/* Numero di carichi */ 

 	/* DEBUG: da sistemare la #define "IO_MANAGER_LOAD_TABLE_ITEM_SIZE" */
	nLoads = ( Tab_OutputData.DataLenght /4 /*IO_MANAGER_LOAD_TABLE_ITEM_SIZE*/); /* lunghezze espresse in Word */

/* Loop pilotaggio carichi */
	for ( i = 0; i < nLoads; i++ ) 	
	{		  
		/* Default: abilitata richiesta aggiornamento stato carico */
		setLoadCallEn = TRUE;
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT				
		if( Pump_Pos[LD_WASH_PUMP_IND] == i )
		{

			/* QI 06.11.09 gestione flag nuovo BLDC */
			if(( HWC_Flags.bit.bldc )&&(!HWC_Flags.bit.bldc1)) 
			{
				/* Flag non necessario in DEA 700 */
				SET_BLDC_FLAG(BLDC_PumpReq_New[WASH_BLDC_PUMP].data,NEW_BLDC_ENABLE_FLAG_POS );
			}	

#if (VIRTUAL_SENSOR_ENABLE == 1)
			if( HWC_Flags.bit.bldc1 )
			{
				if (SWTC_IsExpired((SWTC_CountersType)SFT_FILTER_VS56_STATE_W) )
				{ 					
					SET_BLDC_FLAG((BLDC_PumpReq_New[WASH_BLDC_PUMP].data+1),NEW_BLDC_ENABLE_FLAG_POS );
																				
#ifdef _STATISTICS_ON_
					if ( !SWTC_IsStopped((SWTC_CountersType )SFT_FILTER_VS56_STATE_W) )
					{/* transizione brillantante presente -> assente*/
						IncWordWithCheck(&MachineStatistics.globalStats.unblockvirtualsensorStateW);
						IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].reset_VS_StateW);
					}	
#endif	
					(void)SWTC_Stop((SWTC_CountersType)SFT_FILTER_VS56_STATE_W); 
								
						
				}	
				if ( (LONIBBLE(FltBLDCUnblock.pumpandspeed ) ==WASH_BLDC_PUMP)&& TESTBIT(FltBLDCUnblock.flags,PUMP_STALLO))
				{
					SET_BLDC_FLAG((BLDC_PumpReq_New[WASH_BLDC_PUMP].data+1),NEW_BLDC_STALLO_FLAG_POS );
				}
			}		
#endif /*#if (VIRTUAL_SENSOR_ENABLE == 1)*/
			BLDC_PumpReq_New[WASH_BLDC_PUMP].loadID = i;
			load = BLDC_PumpReq_New[WASH_BLDC_PUMP];
			BLDC_PumpReq_Old[WASH_BLDC_PUMP] = BLDC_PumpReq_New[WASH_BLDC_PUMP];      /* salvataggio stato pompa lavaggio */

		}

		else
		{	        
			if ( i == Pump_Pos[LD_DRAIN_PUMP_IND] )
			{ /* drain pump */

                if(HWC_Flags.bit.bldc_drain != 0)
				{
					BLDC_PumpReq_New[DRAIN_BLDC_PUMP].loadID = i;
					if ( (LONIBBLE(FltBLDCUnblock.pumpandspeed )==DRAIN_BLDC_PUMP)&& TESTBIT(FltBLDCUnblock.flags,PUMP_STALLO))
					{
						SET_BLDC_FLAG((BLDC_PumpReq_New[DRAIN_BLDC_PUMP].data+1),NEW_BLDC_STALLO_FLAG_POS );
					}
	
					load = BLDC_PumpReq_New[DRAIN_BLDC_PUMP];
					BLDC_PumpReq_Old[DRAIN_BLDC_PUMP] = BLDC_PumpReq_New[DRAIN_BLDC_PUMP];      /* salvataggio stato pompa scarico */
					
				}
				else
				{
				    load.loadID = i;
					load.loadParam.b = LOAD_OFF;
/* loads req new -- file di setting, DrainSftReq_New è per la sicurezza */
					if ( (LoadsReq_New[Pump_Pos[LD_DRAIN_PUMP_IND]].b == LOAD_ON) || (DrainSftReq_New.b == LOAD_ON)  )   
					{
						load.loadParam.b = LOAD_ON;  
					}
				}
			}
			else
			
			{
				load.loadID = i;
			    load.loadParam = LoadsReq_New[i];	  
			}
		}

#else /*CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/			        
		load.loadID = i;
		load.loadParam = LoadsReq_New[i];	  
#endif /*CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */
	    
		if ( i == EV_Pos[LD_WASH_EV_IND] )
		{ /* EV wash */

			load.loadParam.b = EV_OFF;

			if ( (LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b == EV_ON) || (LoadEVFltReq_New.b == EV_ON)  )   
			{
				load.loadParam.b = EV_ON;   
			}
		}

		if ( i == Pump_Pos[LD_DRAIN_PUMP_IND] )
		{ /* drain pump */

			load.loadParam.b = LOAD_OFF;

			if ( (LoadsReq_New[Pump_Pos[LD_DRAIN_PUMP_IND]].b == LOAD_ON) || (DrainSftReq_New.b == LOAD_ON)  )   
			{
				load.loadParam.b = LOAD_ON;   
			}
		}


/* QI 10.12.09 */			
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT

		if ( (HWC_Flags.bit.bldc1) && ( i == RegDev_Pos[LD_KILL_DSP_REG_IND] ) )
		{
			if (load.loadParam.b == LOAD_OFF) 
			{
				uint8_t cnt;

				for(cnt=0; cnt< NR_OF_BLDC_PUMPS; cnt++)
				{
		      		memset(&BLDC_Pump_InfoBuffer[cnt][0], 0, IOM_EXTERNAL_0_INFOBUFFER_SIZE);
				}
			}
		}

#endif /* CONFIG_LIB_EXTERNAL_BOARD_SUPPORT */			

		if( ( Alt_Pos == i ) && ( LoadsReq_Old[i].b == LoadsReq_New[i].b ) )
		{
			/* per l'alternato si filtrano i comandi */
			setLoadCallEn = FALSE;
		}

		if( setLoadCallEn )
		{	        
			(void)LIB_IOM_GIOM_SetLoad(&load);
		}

		LoadsReq_Old[i] = LoadsReq_New[i];	/* salvataggio stato carichi */
	}		
		
}

/**
Aggiorna lo stato del buffer contenente i dati
del motore 3fase ad uso dell'interprete

\version		1.0.0
\date 			02/07/2008
\author			Roberto Fioavanti

*/
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
void HIO_updateEngineData(void)
{/* EA commentato*/

	if ( ( HWC_Flags.bit.bldc ) || ( HWC_Flags.bit.bldc1 ) )
	{
		UseExternalInfo();
		memcpy(ExtMotor_DataBuffer, ExternalGlobalData_0, IOM_EXTERNAL_0_GLOBALDATABUFFER_SIZE);
		UnuseExternalInfo();
	}
}
#endif /*end CONFIG_LIB_EXTERNAL_BOARD_SUPPORT*/



/**
Azzera lo stato attuale dei carichi
 
\version		1.0.0
\date 			17/09/2013
\author			Andrea Emili

*/
void HIO_loadsReset(void)
{
L_PARAM loadsReqNew_tmp;


	if( HWC_Flags.bit.alternate != 0 )
	{
		loadsReqNew_tmp = LoadsReq_New[Alt_Pos];
	}

  
/* reset array di stato carichi */
	memset(&LoadsReq_New[0], 0, sizeof(LoadsReq_New));


	if( HWC_Flags.bit.alternate != 0 )
	{

/* lo stato dell'alternato non va resettato perchè contiene la posizione del motorino */
/*lint -e644
	Se HWC_Flags.bit.alternate != 0 loadsReqNew_tmp è inizializzato
*/
		LoadsReq_New[Alt_Pos] = loadsReqNew_tmp;
/*lint +e644*/


	}
	
{
	uint8_t cnt;
/* reset stato pompe bldc */
	
	for(cnt=0; cnt< NR_OF_BLDC_PUMPS; cnt++)
	{
		memset(&BLDC_PumpReq_New[cnt], 0, sizeof(BLDC_PumpReq_New[0])); 
	}
}

/* reset stato richiesto per EV */	
	LoadEVFltReq_New.b = 0;

/* reset stato richiesto per Drain */	
	DrainSftReq_New.b = 0;

}

/**
Inizializza le variabili globali con posizione dei carichi in base alla Output Map
 
\return S_OK se non ci sono errori di caricamento o nel contenuto dei dati, un codice d'errore altrimenti.
 
\version		1.0.0
\date 			30/01/2008
\author			Valeria Catani

*/

uint8_t HIO_ldPosInit(void)
{
uint8_t i,aux;
Id_Struct *pId;

	/* Init var posizioni al default: NO_LD_POS */
	Pump_Pos[LD_WASH_PUMP_IND] = NO_LD_POS;
	Pump_Pos[LD_DRAIN_PUMP_IND] = NO_LD_POS;
	Res_Pos[LD_WASH_RES_IND] = NO_LD_POS;
	Res_Pos[LD_DRY_RES_IND]  = NO_LD_POS;
	Res_Pos[LD_RESIN_RES_IND]  = NO_LD_POS;
	EV_Pos[LD_WASH_EV_IND] = NO_LD_POS;
	EV_Pos[LD_RIG_EV_IND] = NO_LD_POS;
	EV_Pos[LD_7LITER_EV_IND] = NO_LD_POS;
	Disp_Pos = NO_LD_POS;
	Alt_Pos = NO_LD_POS;
	Fan_Pos[LD_DRY_FAN_IND] = NO_LD_POS;
	Fan_Pos[LD_RESIN_FAN_IND] = NO_LD_POS;
	Ozonator_Pos[LD_OZONATOR_IND]= NO_LD_POS;
	Ozonator_Pos[LD_AEROSOL_IND]= NO_LD_POS;
	Lamp_Pos= NO_LD_POS;
	
	for (i=0; i<N_MAX_LD_REG_DEV; i++)
	{
		RegDev_Pos[i] = NO_LD_POS;		
	}
 

	/* Init con posizioni dalla output map */
	i = 0;
	do
	{
	
	
		pId =(Id_Struct *)((uint8_t *)(Tab_OutputData.Data + (HIO_LOAD_DATA_SIZE_B * i)));
		
		aux = pId->index;				
		
		switch (pId->type)
		{
						
			case(LD_PUMP_TYPE):
			
			    Pump_Pos[aux] = i;

/* LR: pregevoli colleghi, in data 13/05/2011 ho spostato qui le istruzioni seguenti, cos� da legare lo spiazzamento inviato al dsp nel pacchetto di command a come le pompe sono messe in sequenza nella output map (la prima che si incontra avr� spiazzamento 0, la seconda 1, e via cos�) */

            

			
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT	
            if ( (aux == LD_WASH_PUMP_IND)&&  ( HWC_Flags.bit.bldc || HWC_Flags.bit.bldc1 ))
			{
            	/* qui bisogna aggiungere if( ( ( HWC_Flags.bit.bldc != 0 ) || ( HWC_Flags.bit.bldc1 != 0 ) )
				 dopo aver memorizzato le posizioni dei carichi configuro la "UART" ,WASH_PUMP */
				(void)LIB_IOM_External_New ( &BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][0], (uint8_t)IOM_EXTERNAL_0_INFOBUFFER_SIZE,  Pump_Pos[LD_WASH_PUMP_IND]);
			}
			if((aux == LD_DRAIN_PUMP_IND)&&  ( HWC_Flags.bit.bldc_drain!=0) )
			{
				/* configurazione uart anche per questo, � ok???? */	
			    (void)LIB_IOM_External_New ( &BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][0], IOM_EXTERNAL_0_INFOBUFFER_SIZE,  Pump_Pos[LD_DRAIN_PUMP_IND]);
			}
#endif
			break;
			
			case(LD_RES_TYPE):
			
				Res_Pos[aux] = i;
			
			break;
			
			case(LD_EV_TYPE):
			
				EV_Pos[aux] = i;
			
			break;
			
			case(LD_DISPENSER_TYPE):
			
				Disp_Pos = i;
			
			break;
			
			case(LD_ALT_ENGINE_TYPE):
			
				Alt_Pos = i;
			
			break;
			
			case(LD_REGULATION_DEV_TYPE):
			
				RegDev_Pos[aux] = i;
			
			break;
			case ( LD_FAN_TYPE):
			
				Fan_Pos[aux] = i;
			
			break;	
		 
			case(LD_LAMP_TYPE):
			
				Lamp_Pos = i;
			
			break;


			case (LD_DUMMY_TYPE):
			  
			  
			break;
			case (LD_OZONE_TYPE):
				Ozonator_Pos[aux]=i;
			  
			break;
			default:
			
				return HIO_ERR_OUT_NOT_FOUND;
				
			/*break;*/
		}
	
		i++;	/*campo dati per ogni carico = 8 byte*/
	
	} while(i<(Tab_OutputData.DataLenght/4));/*IO_MANAGER_LOAD_TABLE_ITEM_SIZE));*/
	
	
	return S_OK;
}
/**/

/**
Gestisce l'elaborazione sul criterio della presenza sale per
avviare conteggio rigenere
 
\version		1.0.0
\date 			30/06/2008
\author			Valeria Catani
\todo       -
*/
void HIO_saltCriterionRegMgmt(void)
{
  
	if (HWC_Flags.bit.saltreed)
	{/* reed sale presente */

		if ( TESTBIT (GIValid, (GI_FIRST_CRITERION_BIT + GI_SALT_IND) )!=0 )	
		{/* criterio sale valido */

			if ( TESTBIT(GICriteria, GI_SALT_IND) == 0 )
			{	/* stato attuale : sale assente */

				/* richiesto aggiornamento conteggio rigenere */
				NoSalt_Reg = TRUE; /* LO RIBADISCO SEMPRE */

				if ( TESTBIT(GICriteriaOld, GI_SALT_IND) != 0)
				{
					/*
					transizione sale presente -> sale assente =>
					start conteggio rigenere
					*/

					uP_Regenerate_Data.nReg = 1;

				}
			}
			else
			{	/* stato attuale : sale presente */
				
				if ( TESTBIT(GICriteriaOld, GI_SALT_IND) == 0 ) 
				{	/*
					transizione sale assente -> sale presente
					*/

					/* rigenera forzata */

					uP_Regenerate_Data.forceReg_cnt = 1;
				}

				NoSalt_Reg = FALSE; 
			}

			/*GICriteriaOld = GICriteria;*/
		}
		/* else WAIT for valid*/
	}
}

/**
Gestisce l'aggiornamento della variabile GICriteriaOld

\version		1.0.0
\date 			09/07/2008
\author			Valeria Catani
\todo       -
*/

void HIO_gICriteriaOld_Update(void)
{
  
  GICriteriaOld = GICriteria; /*FATTO IN OGNI CASO O CONTROLLANDO GIVALID???*/
}
/*
Gestisce l'aggiornamento e le elaborazioni sugli ingressi ad alto livello
(si basa sul presupposto che i dati nel setting vengono ordinati per ID (categoria ed indice))
 
\version		1.0.0
\date 			09/02/2008
\author			Roberto Fioravanti
\todo       -
*/
void HIO_inputManagement(void)
{
GIMap_Struct *pGenInput=(GIMap_Struct *)Tab_GIData.Data;
uint16_t GI_tmp; 
uint8_t aux_index, fault;

	/* GI_PRESS_SENSOR_LEVEL_BIT nn può essere resettato perché va testato in un criterio (si andrebbe incontro a una perdità di dati) */
	GIValid &= (GI_PRESS_SENSOR_LEVEL_MSK|GI_TURB_SENSOR_MSK);
  
	FltGICriteria = 0;
		
	while((uint8_t*)pGenInput<Tab_GIData.Data+Tab_GIData.DataLenght*2)
	{
		
		aux_index = pGenInput->id.s.index;
						
		switch (pGenInput->id.s.type)
		{
		
			case GI_PROBE_TYPE:
			
				FltGITemp = 0;
								
				if ( HIO_GIConv(pGenInput,&GI_tmp) == S_OK )
				{	
					if ( ((uint8_t)(GI_tmp) != PROBE_UNDEF_CC) && ((uint8_t)(GI_tmp) != PROBE_UNDEF_CA) ) 
					{
						GITemp = (uint8_t)(GI_tmp);
						
						SETBIT(GIValid,(aux_index+GI_FIRST_PROBE_BIT));
					}	
					else
					{/* byte indicativo per Fault su NTC - valori out of range */
					
						if ( (uint8_t)(GI_tmp) == PROBE_UNDEF_CC )
						{
							FltGITemp = FLT_SUBCODE_PROBE_CC;
						}
						else
						{
							FltGITemp = FLT_SUBCODE_PROBE_CA;
						}
					}	
				}
					
				break;

			case GI_CRITERION_TYPE:
				
				switch (pGenInput->readType)
				{
					/* @_@ */
					case ANALOG_I:
					
						if ( HIO_GIConv(pGenInput,&GI_tmp) == S_OK )
						{							
							if (CRITERION_UNDEF != (uint8_t)(GI_tmp))
							{
								SETBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);

								if ( aux_index == GI_SALT_IND)
								/* si sta prendendo il considerazione il Sale come ingresso analogico*/
								{	

									/* Criterio presenza sale*/
									if( RegDev_Pos[LD_SALT_ACTIVATION_REG_IND] != NO_LD_POS )
									{
										/* Presente il pilotaggio del sensore sale */
										
										if( LoadsReq_Old[RegDev_Pos[LD_SALT_ACTIVATION_REG_IND]].b == LOAD_OFF )
										{
											/* Sensore sale non pilotato */
											/*
											Si forza il criterio a non valido e si resetta
											il filtro.
											*/
											CLRBIT(GIValid, (GI_FIRST_CRITERION_BIT + GI_SALT_IND) );
											Salt_Time_Of_Stability = 0;
											
											break;
										}
									}

									if ( GETBITVAL(GICriteria, GI_SALT_IND) != ((uint8_t)(GI_tmp)))
									{
										if ( Salt_Time_Of_Stability < Salt_Filtro_Stab)
										{/*criterio non costante*/
											/* Attendi che passi il tempo corretto all'incirca 10 secondi sono sufficenti */
											Salt_Time_Of_Stability++;
											/* non  è considerato uno stato valido per quel criterio*/
											CLRBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);	
										}
									}
									else
									{
										/* il criterio  non è stabile */
										Salt_Time_Of_Stability = 0;
									}
								}
								
								if ( TESTBIT(GIValid,aux_index+GI_FIRST_CRITERION_BIT))
								{
									if (CRITERION_ON == (uint8_t)(GI_tmp))
									{
										SETBIT(GICriteria, aux_index);
									}
									else
									{
										CLRBIT(GICriteria, aux_index);
									}
								}	
							}
							else
							{
								/* bit indicativo per il fault su pressostato - valori out of range */
								SETBIT(FltGICriteria, aux_index);
							}
						}
					
						break;
					
					case DIGITAL_I:
					
						{
							/* filtro */
							uint8_t currInputVal=(LIB_DIO_GetDigInputVal(pGenInput->pos)<<aux_index);
							Loader_Struct table={0,0};
		                    
							HIO_GIGetCriterionDigitalTable(pGenInput,&table);

							if(table.Data && ((GICriterionDigitalTab_Struct*)(table.Data))->Filter) 
							{
								/* Previsto un filtro di stabilit� */

								if( aux_index == GI_SALT_IND )
								{
									/* Criterio presenza sale*/
									if( RegDev_Pos[LD_SALT_ACTIVATION_REG_IND] != NO_LD_POS )
									{
										/* Presente il pilotaggio del sensore sale */
										
										if( LoadsReq_Old[RegDev_Pos[LD_SALT_ACTIVATION_REG_IND]].b == LOAD_OFF )
										{
											/* Sensore sale non pilotato */
											
											/*
											Si forza il criterio a non valido e si resetta
											il filtro.
											Il valore di CriterionOldState[] non viene modificato ma
											tanto cambia poco, visto che il filtro è azzerato.
											*/
											CLRBIT(GIValid, (GI_FIRST_CRITERION_BIT + GI_SALT_IND) );
											CriterionFilter[GI_SALT_IND] = 0;
											
											break;
										}
									}
								}


								if ( (currInputVal&(1<<aux_index)) == (CriterionOldState&(1<<aux_index)) )
								{
									if ( CriterionFilter[aux_index] < ((GICriterionDigitalTab_Struct*)(table.Data))->Filter )
									{
										CriterionFilter[aux_index]++;                                    
										CLRBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
									} 
									else
									{
										if( currInputVal&(1<<aux_index) )
										{
											SETBIT(GICriteria,aux_index);
										}
										else
										{
											CLRBIT(GICriteria,aux_index);
										}                                        

										SETBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
									}		                        
								}
								else
								{
									CriterionFilter[aux_index] = 0;

									CLRBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);

									if( currInputVal&(1<<aux_index) ) 
									{
										SETBIT(CriterionOldState, aux_index);   
									}
									else 
									{
										CLRBIT(CriterionOldState, aux_index);
									}                    		        
								}
							}
							else
							{
								/* Non previsto un filtro di stabilit� */

								if( aux_index == GI_SALT_IND )
								{
									/* Criterio presenza sale*/
									if( RegDev_Pos[LD_SALT_ACTIVATION_REG_IND] != NO_LD_POS )
									{
										/* Presente il pilotaggio del sensore sale */
										
										if( LoadsReq_Old[RegDev_Pos[LD_SALT_ACTIVATION_REG_IND]].b == LOAD_OFF )
										{
											/* Sensore sale non pilotato */
											
											/* Si forza il criterio a non valido */
											CLRBIT(GIValid, (GI_FIRST_CRITERION_BIT + GI_SALT_IND) );
											
											break;
										}
									}
								}


								if( currInputVal&(1<<aux_index) )
								{
									SETBIT(GICriteria,aux_index);
								}
								else
								{
									CLRBIT(GICriteria,aux_index);
								}                                        
								
								SETBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
							}

						}											    
					
						break;
				  
					case I2C_I:
						CLRBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
						if( UI_Info_Ctrl == TRUE )
						{
							
							if (uPDoorStatus == UP_DOOR_CLOSE)
							{
								if (TESTBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE) )
								{
								
									SETBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
									
								}else
								{	
								           
									if(HIO_GetSparklingAidCriterionValue(pGenInput,&GI_tmp)==S_OK)
									{
										if((uint8_t)GI_tmp)
										{                                                                        
											SETBIT(GICriteria,aux_index);
											CLRBIT(Flags_2,FLAGS2_REED_NO_SPARKL_BIT);
											SETBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE);
										}
										else 
										{                                                                          
												
											if( (Interpreter_Data[0].completeLabel.s.label == LBL_RUN )|| (Interpreter_Data[0].completeLabel.s.label == LBL_DELAY ))
											{
												CLRBIT(GICriteria,aux_index);
												SETBIT(Flags_2,FLAGS2_REED_NO_SPARKL_BIT);
												SETBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE);	
								
											}else
											{
												SparklingReads.nSamples=0;
											}	
		
										}
									
										SETBIT(GIValid, aux_index+GI_FIRST_CRITERION_BIT);
									}
								}	
							}else
							{
								if( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING )
								{
									SparklingReads.nSamples=0;
									CLRBIT(Flags_2,FLAG2_HIO_READ_SPARKLING_DONE);
								}	
							}
							UI_Info_Ctrl=FALSE;
						}

						break;


					default:
					
						break;
				}	
				
			break;
			
			case GI_PULSE_COUNTER_TYPE:	
			    /* Lettura warnings */
			    /*rientra in questo caso anche TURBINA_BLOCCATA     ???*/
			    fault=PM_PulseCounter_GetWarning();
			    if(fault) 
			    {
			        /*Gestire del fault*/
			        FltGIPulseCount=fault;
			        break; /* non aggiornare la lettura dell'ingresso */
			    }
			
			    /* è possibile convertire la lettura */
			    if(HIO_GIScaleConv(pGenInput, &GI_tmp) == S_OK)
			    {
			      GIWaterLoaded=GI_tmp;
#ifdef CONFIG_PM_PULSECOUNTER
		            if (pGenInput->readType == PULSECNTR_I)
		          	{
		          	  PulseLoaded=PM_PulseCounter_GetCount();
		          	}
#endif /*__PULSECOUNTER_H	*/
		          	SETBIT(GIValid, aux_index+GI_FIRST_PULSE_COUNTER_BIT);
			    } 			    
			
			break;
			
		  case GI_TURB_SENSOR_TYPE:
		    if((TurbiditySensorCal.state == (uint8_t)TURBSENS_VALID_CALIBR_VALUE_STATE)
			|| (TurbiditySensorCal.state == (uint8_t)TURBSENS_CALIBR_VALIDATE_STATE)
		    || (TurbiditySensor.state==TURBIDITY_PWM_CUSTOM_STATE)
		    || (TurbiditySensor.state==TURBIDITY_CONTINUOUS_READ_CUSTOM_STATE))
		    {   
		         /*gestione del sensore solo in caso di valore di calibrazione valido*/
		        		    
		        if(!TurbidityReads.readers)
		        {
		             /*disabilita la lettura*/
		        	/*AE Fioravanti dice che il modulo si auto disabilità
		        	 * quindi non necessario invio del comando*/

                     /*segnala 'acquisizione disabilitata'*/
                    TurbiditySensorCal.flags.bit.cmdAcquireDone=0;

                    /* lettura non valida*/
                    CLRBIT(GIValid, (aux_index+GI_FIRST_TURB_SENSOR_BIT));
		        }
		        else
		        {
		             /*at least one reader*/
		            
                    if((TurbiditySensorCal.flags.bit.cmdAcquireDone==0))
                    {
                         /*avvia una nuova lettura*/

                    	PM_TurbiditySensor_StartAcq(TurbiditySensor.pwmval);
                        /* segnala 'acquisizione abilitata'*/
                        TurbiditySensorCal.flags.bit.cmdAcquireDone=1;
															    
						TurbidityReads.ADvalue=0;
						TurbidityReads.nSamples=0;
						TurbidityReads.a=0;
						TurbidityReads.flags.b.SampleInit=1;
						
                    }
		        
		      		    
          		     /*è possibile convertire la lettura*/
          		    if(PM_TurbiditySensor_AcqIsReady())
          		    {		      
                    
                        FltGITurbidity = 0;
                    
                        if ( HIO_GIConv(pGenInput,&GI_tmp) == S_OK )
                        {

                            if(TurbidityReads.readers!=(1<<((uint8_t)TURBSENS_READ_cstm_CONTINUOUS)))
                            {           
                                /* non aggiornare la lettura in caso di SOLA lettura custom in continuo
    						    per non influenzare il comportamento dei microprogrammi*/

						                         
                                GITurbidity = (uint8_t)(GI_tmp);
                                SETBIT(GIValid, (GI_FIRST_TURB_SENSOR_BIT));
                            }
                            TurbiditySensor.lvl=(uint8_t)(GI_tmp);
                        
                        }
          		    }		    
      		    }
		    } 
		    else
		    {
		        TurbiditySensorCal.flags.bit.cmdAcquireDone=0;
		    }
		     
			break;
			/* nuovo generic input per gestire i sensori virtuali dati dalle pompe*/
#if (VIRTUAL_SENSOR_ENABLE == 1)
			case GI_VIRTUAL_SENSOR_TYPE:
				
				if (HIO_Check_VS_Valid( aux_index))/*controllare se i dati per il sensore sono validi*/
				/* dati validi per il sensore*/
				{
					
					GI_tmp = MAKEWORD(BLDC_Pump_InfoBuffer[aux_index][BLDC_VS_STATE_MSB], BLDC_Pump_InfoBuffer[aux_index][BLDC_VS_STATE_LSB]);	
					if (( aux_index== WASH_BLDC_PUMP)&& ((GI_tmp == VS_UNDEFINED_NEW)||( GI_tmp ==5)) )
					{
						 if( !SWTC_IsExpired((SWTC_CountersType)SFT_FILTER_VS56_STATE_W) )
						 {
							(void)SWTC_Run((SWTC_CountersType)SFT_FILTER_VS56_STATE_W); 
						 }	
					}else
					{
						(void)SWTC_SetSValue ((SWTC_CountersType)SFT_FILTER_VS56_STATE_W, HIGH_IMPROB_SECURITY_FILTER);
						(void)SWTC_Stop((SWTC_CountersType)SFT_FILTER_VS56_STATE_W); 
					}


					if (( GI_tmp == VS_UNDEFINED)||( GI_tmp == VS_UNDEFINED_NEW)||(!SWTC_IsStopped((SWTC_CountersType)HIO_STABILITY_FILTER_VS) && !SWTC_IsExpired((SWTC_CountersType)HIO_STABILITY_FILTER_VS) ))
					{
						CLRBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+aux_index));	
					}
					else
					{
						Virtual_Sensor_Value[aux_index]=(uint8_t)GI_tmp;
						SETBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+aux_index));	
						/*GI_VIRTUAL_SENSOR_WASH ==LD_WASH_PUMP_IND*/
					
					
					}
						
				}	else
				{
					CLRBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+aux_index));
					if ( aux_index== WASH_BLDC_PUMP)
					{
						(void)SWTC_SetSValue ((SWTC_CountersType)SFT_FILTER_VS56_STATE_W, HIGH_IMPROB_SECURITY_FILTER);
						(void)SWTC_Stop((SWTC_CountersType)SFT_FILTER_VS56_STATE_W); 
					}	
				

				}
				
			break;
#endif
			default:
			
			break;
		
		} 			
		pGenInput++;
	}


	
}
/**/

/**
Gestisce l'aggiornamento dell'array di consistency
(valori di coerenza tra stato del carico e feedback associato)

\version		1.0.0
\date 			03/10/2006
\author			Valeria Catani

*/

void HIO_consistencyManagement(void)
{
	PM_Consistency_GetAllCsValues(&Consistency_Current[0]);

}
/**/

/* ************** */
/* Local Function */
/* ************** */

/**
Routine di conversione per Generic Inputs.
(ANALOG TO DEGREES, FREQ TO WATER LEVEL, PRESSURE SENSOR - LINEAR
ANALOG TO CRITERION, FREQ TO CRITERION, ANALOG TO LEVELS)

 
\param [in]		*pGI : puntatore alla struttura del Generic Input in questione
\param [in]		*pResult : puntatore al risultato della conversione
\return 			uint8_t
\retvalue 		S_OK se la conversione è andata a buon fine
\retvalue 		HIO_ERR_BASIC_LOADER errore caricamento dati dal file di setting
				
\version		1.0.1
\date 			12/02/2008
\author			Valeria Catani, Roberto Fioravanti

\note		\par \b Nota 1:
            Generic Inputs supportati:
            ANALOG_I
            TURBIDITY_I
            PRESSLIN_I

\todo       introdurre 
            -gestione calibrazione del pressostato lineare
            
*/	
uint8_t HIO_GIConv(GIMap_Struct const* pGI, uint16_t *pResult) 
{
Loader_Struct table;                    /*pnt alla tabella del GI*/
uint8_t *pSamples;	                    /*pnt campo samples */
uint16_t value = 0;                     /*valore lettura ingresso*/
TabQueryResult_t    qresult;        
TabQueryIn_t        qin;


    /* caricamento tabella parametri di conversione */
	if( BasicLoader( (settingPntr_t)APP_PTR_IO_DEVICE_DATA,pGI->hlParamOffset,&table ) != S_OK )
	{
		return HIO_ERR_BASIC_LOADER;
	}
    
    if (pGI->readType == ANALOG_I)
    {
    	value = LIB_ADC_GetAnalogValue (pGI->pos);
    }
		
    if (pGI->readType == TURBIDITY_I)
    {             
        /* lettura valore analogico */
        value = LIB_ADC_GetAnalogValue (pGI->pos);

        /* puntatore al campo Samples (allineato a word) */        
        pSamples=( (uint8_t*)table.Data + sizeof(TabHeader_t) + (((TabHeader_t *)table.Data)->pointNumber<<(((TabHeader_t *)table.Data)->S1&GI_WORD)) ); 


        if((( ((TabHeader_t *)table.Data)->S1 & GI_WORD ) == GI_BYTE) && (((TabHeader_t *)table.Data)->pointNumber&1))
        {
            pSamples++;
        }

        pSamples+=(((TabHeader_t *)table.Data)->pointNumber<<(((TabHeader_t *)table.Data)->S2&GI_WORD));    
        if(((((TabHeader_t *)table.Data)->S2&GI_WORD) == GI_BYTE) && (((TabHeader_t *)table.Data)->pointNumber&1))
        {
            pSamples++;
        }
                
        if(!HIO_trbProcessSmpl(value, pSamples))
        {        
          return S_FALSE;
        }
            
        value=TurbidityReads.perc;    
    }
  
    
    qin.cfr_val=value;
    qin.opt.all=0;
    qin.tab=(TabHeader_t *)table.Data;
        
    TabQuery(&qin, &qresult);
    
 
	
	/*formato dati in uscita*/
	if( (pGI->id.s.type == GI_CRITERION_TYPE) ) 
	{
		  if(qresult.b.ubound) 
		  {
		    *pResult = qresult.oBoundVal;
		  } else 
		  {
		    *pResult = qresult.data;
		  }
		  
	} else
	{


	
		if(qresult.b.lbound)
		{
			/* Error : out of range MIN */

			*pResult = GIVALUE_UNDEF_MIN;

		    if (pGI->readType == TURBIDITY_I)
			{
			    /* anche se non trova il valore, esce con l'output massimo possibile */
			    *pResult=qresult.oBoundVal;
			}
			
			return S_OK;
		}		
		else if(qresult.b.ubound)
		{
			/* Error : out of range MAX */

			*pResult = GIVALUE_UNDEF_MAX;

			if (pGI->readType == TURBIDITY_I)
			{
			    /* anche se non trova il valore, esce con l'output massimo possibile */
			    *pResult=qresult.oBoundVal;
			}
			
            return S_OK;
		}
		else						
		{
		  *pResult = qresult.data;
		}
		
	
	}
		

  return S_OK;

}


/**
Routine di conversione con fattore di scala per Generic Inputs
 
\param [in]		*pGI : puntatore alla struttura del Generic Input in questione
\param [in]		*pResult : puntatore al risultato della conversione
\return 			uint8_t
\retvalue 		S_OK se la conversione è andata a buon fine
\retvalue 		HIO_ERR_BASIC_LOADER errore caricamento dati dal file di setting

				
				
\version		1.0.0
\date 			11/02/2008
\author			Roberto Fioravanti

\note		\par \b Nota 1:
            Generic Inputs supportati:
            PULSECNTR_I
            
*/	
uint8_t HIO_GIScaleConv(GIMap_Struct const* pGI, uint16_t *pResult) 
{
	Loader_Struct table;
  
	/* caricamento tabella parametri*/
	if( BasicLoader( APP_PTR_IO_DEVICE_DATA,pGI->hlParamOffset,&table ) != S_OK )
	{
		return HIO_ERR_BASIC_LOADER;
	}

#ifdef CONFIG_PM_PULSECOUNTER
	if (pGI->readType == PULSECNTR_I)
	{
		/*caricamento divisore*/

		FlowMeterDivider = OFFS_B(table.Data,0);
		/*risultato*/
		*pResult = PM_PulseCounter_GetCount()/FlowMeterDivider;
	}
#endif /*CONFIG_PM_PULSECOUNTER	*/

	return S_OK;
}


/**
restituisce puntatore alla tabella dei parametri per Generic Inputs Criterion Digital
 
\param [in]		*pGI : puntatore alla struttura del Generic Input in questione
\param [out]	*pTable : puntatore alla tabella dei parametri
\return 			uint8_t
\retvalue 		S_OK se la conversione è andata a buon fine
\retvalue 		HIO_ERR_BASIC_LOADER errore caricamento dati dal file di setting
\retvalue 		HIO_ERR_ARG_NOT_SUPPORTED errore GI non supportato

				
				
\version		1.0.0
\date 			3/09/2008
\author			Roberto Fioravanti

\note		\par \b Nota 1:
            Generic Inputs supportati:
            idType:     GI_CRITERION_TYPE
            readType:   DIGITAL_I
            
*/	
uint8_t HIO_GIGetCriterionDigitalTable(GIMap_Struct const* pGI, Loader_Struct *pTable) 
{
  Loader_Struct table;
  
    if(pGI->id.s.type != GI_CRITERION_TYPE) 
    {
        return HIO_ERR_ARG_NOT_SUPPORTED;
    }
    
    if(pGI->readType != DIGITAL_I) 
    {
        return HIO_ERR_ARG_NOT_SUPPORTED;
    }

    /* caricamento tabella parametri*/
    if( BasicLoader( APP_PTR_IO_DEVICE_DATA,pGI->hlParamOffset,&table ) != S_OK )
	{
		return HIO_ERR_BASIC_LOADER;
	}
	
	*pTable = table; /* QI 06.11.09 correzione baco */
  
	return S_OK;
	
}


/**
  Effettua il calcolo della media tra TurbidityReads.readings
  campioni
  
\param [in]		trbsample : campione
\param [in]		p_samples : # campione per il calcolo della media

\return 			uint8_t
\retvalue 		TRUE nuovo valore disponibile in TurbidityReads.ADvalue
\retvalue 		FALSE media non completata

\version		1.0.0
\date 			11/04/2008
\author			Roberto Fioravanti

*/
uint8_t HIO_trbProcessSmpl(uint16_t trbsample, uint8_t * p_samples) 
{
uint8_t retval=FALSE;
uint16_t adval;
 
  if(TurbidityReads.nSamples==0) 
  {
    
    /* imposta il valore del numero di campioni*/
    TurbidityReads.pReadings=p_samples;
    TurbidityReads.nSamples=*TurbidityReads.pReadings;
		
		if(TurbidityReads.flags.b.SampleInit==1)
		{
			TurbidityReads.flags.b.SampleInit=0;
			return FALSE;
		}
    
    if(!TurbidityReads.nSamples)
    {      
      return FALSE;
    }
    /* calcolo media */
    adval=TurbidityReads.a/TurbidityReads.nSamples;
    TurbidityReads.ADvalue=adval;
    
    /* calcolo torbidità percentuale */
    if(TurbiditySensorCal.adH2oRef)
    {
		adval=TurbidityReads.ADvalue;
		if(TurbiditySensorCal.adH2oRef + (uint16_t)TurbiditySensorCal.adH2oRefDelta <= TurbidityReads.ADvalue)
	    {
	        TurbidityReads.ADvalue=0;
	        TurbidityReads.flags.b.AdTooLarge=1;
	        adval=0;
	    }
	    else
	    {
	        TurbidityReads.flags.b.AdTooLarge=0;
            if(TurbiditySensorCal.adH2oRef < TurbidityReads.ADvalue)
    	    {
    	        
    	        adval=TurbiditySensorCal.adH2oRef;
    	    }    	    
	    }
            
        TurbidityReads.perc=(uint16_t)100*(TurbiditySensorCal.adH2oRef- adval)/TurbiditySensorCal.adH2oRef;
        TurbidityReads.flags.b.H2ORefNull=0;
    }
    else
    {
        TurbidityReads.perc=100;
        TurbidityReads.flags.b.H2ORefNull=1;
    }
    
    
    
	TurbidityReads.id++;
	retval=TRUE;
	
	/* azzera accumulatore */
    TurbidityReads.a=0;
  } 
  
  /* aggiorna numero di letture rimanenti e accumulatore */
  TurbidityReads.nSamples--;
  TurbidityReads.a+=trbsample;
      
  return retval;
}

/**  
 Richiede lettura del sensore di torbidità con id
\param [in]	        id
\return 			none

\version		1.0.0
\date 			21/05/2008
\author			Roberto Fioravanti

*/
void HIO_trbReadEnable(uint8_t id)
{
    SETBIT(TurbidityReads.readers,id);
}

/**  
 Termina la richiesta della lettura del sensore di torbidità con id
\param [in]	        id
\return 			none

\version		1.0.0
\date 			21/05/2008
\author			Roberto Fioravanti

*/
void HIO_trbReadDisable(uint8_t id)
{
    CLRBIT(TurbidityReads.readers,id);
}


/**
  Determina il criterio per il brillantante
  
\param [in]		pGI : puntatore al generic input.
\param [out]	cr_val : valore del criterio
\return 			uint8_t
\retvalue 		HIO_ERR_BASIC_LOADER errore basic loader
\retvalue 		S_OK criterio valutato correttamente
\version		2.0.0
\date 			14/01/2011
\author			Emanuela Antognoli
Modificata logica per correggere il problema della perdita della vaschetta e delle gocce di condensa che si formano alla fine dell'asciugatura.



*/
uint8_t HIO_GetSparklingAidCriterionValue(GIMap_Struct const* pGI, uint16_t* cr_val)
{
Loader_Struct table;
GITabSpa_Struct *pParams;                /*pnt ai dati del GI*/
uint8_t       on;
uint8_t       cr_val_tmp;
uint8_t sparklingsample=BufUIInfo[pGI->pos];
    
	if( BasicLoader( APP_PTR_IO_DEVICE_DATA,pGI->hlParamOffset,&table ) != S_OK )
	{
		return HIO_ERR_BASIC_LOADER;
	}

	pParams=(GITabSpa_Struct *)table.Data;
	on=pParams->On;
	cr_val_tmp=(1 && TESTBIT(GICriteria, GI_SPARKLING_AID_IND) );        
	*cr_val=cr_val_tmp;


	if(SparklingReads.nSamples<pParams->Samples)
	{
		SparklingReads.nSamples++;
		return HIO_ERR_SPRK_NOT_READY;
	}
	else
	{

		if(!
			(
			((on==cr_val_tmp) && (sparklingsample<pParams->Hb))
			||
			(((!on)==cr_val_tmp) && (sparklingsample>pParams->Lb))
			)
		  )
		{
			*cr_val=cr_val_tmp^1;

		}
		SparklingReads.nSamples=0;

	}
	return S_OK;
}


/**
<Fan_GenericLoad_Mgmt> 

\ param  [in]		none 


\warning	[variabile globale : modificato il valore LoadsReq_New: stato finale di attivazione del carico ventola]		
			 e' usata come input/output
\author 	Emanuela Antognoli
\date		 9/09/10
\version	1.0.0
\bug		
\
\note	

*/  
void Fan_GenericLoadMgmt() 
{


	if( Surge_Fan_Params.fan_startup_timer != 0 )
	{
		if( LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b != FAN_SPEED_OFF )
		{
			if( LoadsReq_Old[Fan_Pos[LD_DRY_FAN_IND]].b == FAN_SPEED_OFF )
			{/*trans OFF - ON*/
				/*Setup Timer*/
				(void)Set_SmallCounter((SWTC_CountersType)SFT_FAN_STARTUP_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)Surge_Fan_Params.fan_startup_timer);
			}
			
			if( SWTC_IsExpired((SWTC_CountersType)SFT_FAN_STARTUP_S_COUNTER) )
			{
				/*
				Tempo di startup scaduto, si lascia il valore impostato precedentemente
				di LoadsReq_New[Fan_pos]
				*/
				(void)SWTC_Stop((SWTC_CountersType)SFT_FAN_STARTUP_S_COUNTER);			
			}
			else
			{
				/* Tempo di startup non scaduto, si impone la velocità fan_startup_speed */
				(void)SWTC_Run((SWTC_CountersType)SFT_FAN_STARTUP_S_COUNTER);

				LoadsReq_New[Fan_Pos[LD_DRY_FAN_IND]].b= Surge_Fan_Params.fan_startup_speed;
			}
		}
	}
	else
	{
		/* Se fan_startup_timer == 0 la protezione non va applicata */	
	}


}


/**
<HIO_OverPower_Pump_Control> 

\ param  [in]		none 


\warning	[variabile globale : modificato il valore WashPumpReq_New: stato finale della pompa lavaggio]		
			 e' usata come output
\author 	Emanuela Antognoli
\date		 10/09/10
\version	1.0.0
\bug		
\
\note	

*/  
void HIO_OverHeat_DrainPump_Control ( void) 
{

static Over_Power_Bldc_Struct over_power_algorithm = {0};
uint16_t act_power, threshold;
uint16_t targetSpeed;



	act_power = MAKEWORD(BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][BLDC_OH_INDEX_MSB], BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][BLDC_OH_INDEX_LSB]);
    threshold = DRAINPUMP_OVERHEAT_THRESHOLD;

	
	targetSpeed = GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP]);

	if( targetSpeed == 0 )
	{
		over_power_algorithm.s.status=DISABLED_OP_ALGO;
	}
	
	switch ( over_power_algorithm.s.status )
	{
		case DISABLED_OP_ALGO:
		
			if ( act_power > threshold ) 
			{ /* LR 19 maggio, anno del Signore 2011 (benché molte circostanze facciano pensare che il demonio stia prevalendo): il DSP ha già un filtro temporale di 30s, quindi non serve mettere un filtro sulla rilevazione dell'overheating, si può fare partire la procedura istantaneamente. */
				 

				(void)SWTC_SetMValue((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER, (SWTC_MCounterValue)DRAINPUMP_REDUX_OVERHEAT_DURATION);
				(void)SWTC_Stop((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER); 
				over_power_algorithm.s.status = REDUX_OP_ALGO;
		    }
			else
			{
				(void)SWTC_Stop((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER);
				/* permane nello stato disable*/
			}
			break;

		case REDUX_OP_ALGO:
		
			(void)SWTC_Run((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER);

			if ( SWTC_IsExpired((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER))
			{
				if ( DRAINPUMP_OFF_OVERHEAT_DURATION )
				{
					(void)SWTC_SetMValue((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER, (SWTC_MCounterValue)DRAINPUMP_OFF_OVERHEAT_DURATION);
					over_power_algorithm.s.status = OFF_OP_ALGO;		
				}else
				{
					(void)SWTC_SetMValue((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER, (SWTC_MCounterValue)DRAINPUMP_ON_OVERHEAT_DURATION);
					over_power_algorithm.s.status=ON_OP_ALGO;
					
				}
			
			}


			/* DURANTE 	lo stato di ON la velocit� va ridotta*/


			if( targetSpeed >= 0 )
			{
				if( targetSpeed >= DRAINPUMP_OVERHEAT_SPEED_REDUCTION )
				{
					targetSpeed -= DRAINPUMP_OVERHEAT_SPEED_REDUCTION;
				}
				
				if( targetSpeed < DRAINPUMP_SAFETY_MIN_SPEED )
				{
					targetSpeed = DRAINPUMP_SAFETY_MIN_SPEED;
				}

				SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP], targetSpeed);

	            SET_PUMP(LD_DRAIN_PUMP_IND, BLDC_PumpReq_New[DRAIN_BLDC_PUMP].loadParam.b);
			}
			else
			{
				/*
				La richiesta pompa è già i fase di spegnimento, non si fa niente,
				altrimenti si corre il rischio di riaccenderla chiedendo MIN_SPEED,
				e quindi di modificare lo stato On/Off del carico
				*/
				/* Verificare con Danilo/Alessio MQ */
			}


			break;

		case OFF_OP_ALGO:

			(void)SWTC_Run((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER);
				
			if( SWTC_IsExpired((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER) )
			{

				(void)SWTC_SetMValue((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER, (SWTC_MCounterValue)DRAINPUMP_ON_OVERHEAT_DURATION);					 
				over_power_algorithm.s.status=ON_OP_ALGO;
		
				
			}
			SET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP], 0);

	        SET_PUMP(LD_DRAIN_PUMP_IND, LOAD_OFF);
			break;
			case ON_OP_ALGO:
			/* non applico nessun cambiamento di velocità*/
				(void)SWTC_Run((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER);
				
				if( SWTC_IsExpired((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER) )
				{		
						(void)SWTC_SetMValue((SWTC_CountersType)SFT_DRAINPUMP_OVER_HEATING_M_COUNTER, (SWTC_MCounterValue)DRAINPUMP_REDUX_OVERHEAT_DURATION);			 
						over_power_algorithm.s.status=REDUX_OP_ALGO;
				}
				 
				
			
			break;

		default:
			over_power_algorithm.s.status = DISABLED_OP_ALGO;
			break;
	}	
	
	
}


void HIO_BulkAndKillDSP_Mgmt( void) 
{
uint8_t BLDC_washPumpState;
uint8_t BLDC_drainPumpState;
uint8_t bulk_DSP_Side_ReqState;
uint8_t bulk_uP_Side_ReqState;
uint8_t killDSPReqState;
uint8_t bulkFilteredState;
static uint8_t oldBulkState = LOAD_OFF;
/*
Come inizializzare oldBulkState (conta solo alla prima esecuzione dopo
un reset freddo)?
Se la inizializzo LOAD_ON:
	- se al reset freddo la BULK è richiesta spenta, vedo una transizione
	  che mi fa scattare il delay e accendere il DSP.
	- se al reset freddo la BULK è richiesta accesa, il valore di oldBulkState
	  è ininfluente.
Se la inizializzo LOAD_OFF:
	- se al reset freddo la BULK è richiesta spenta, non vedo nessuna transizione
	  e, dato che il timer è inizializzato a scaduto, il DSP si può spegnere.
	- se al reset freddo la BULK è richiesta accesa, il valore di oldBulkState
	  è ininfluente.
*/

	/* **** Determinazione dello stato richiesto per la pompa lavaggio BLDC **** */

	/* Default: stato richiesto OFF */
	BLDC_washPumpState = LOAD_OFF;

	if( HWC_Flags.bit.bldc1 != 0 ) 
	{
		/*
		Il controllo su bldc1 non è ridondante perchè non necessariamente,
		se eseguo questa routine, è presente la pompa lavaggio BLAC. Potrebbe
		essere presente anche solo la pompa scarico BLAC.
		*/
		if( BLDC_PumpReq_New[WASH_BLDC_PUMP].loadParam.b == LOAD_ON )
		{
			/* Pompa lavaggio BLAC presente e richiesta accesa */
			BLDC_washPumpState = LOAD_ON;
		}
	}

	/* **** Determinazione dello stato richiesto per la pompa scarico BLDC **** */

	/* Default: stato richiesto OFF */
	BLDC_drainPumpState = LOAD_OFF;


	if( HWC_Flags.bit.bldc_drain != 0 )
	{
		/*
		Il controllo su bldc_drain non è ridondante perchè non necessariamente,
		se eseguo questa routine, è presente la pompa scarico BLAC. Potrebbe
		essere presente anche solo la pompa lavaggio BLAC.
		*/
		if( ( LoadsReq_New[Pump_Pos[LD_DRAIN_PUMP_IND]].b == LOAD_ON ) ||
		    ( DrainSftReq_New.b == LOAD_ON ) )
		/*
		Quando sarà gestita anche la pompa scarico BLAC lo stato della pompa sarà
		dedotto da DrainPumpReq_New.loadParam.b, che però al momento non esiste.
		Per ora, come pro-memoria, si controlla LoadsReq_New[]. MQ
		*/
		{
			/* Pompa scarico BLAC presente e richiesta accesa */
			BLDC_drainPumpState = LOAD_ON;
		}
	}
	
	/* **** Impostazione dello stato richiesto per la BULK DSP SIDE (pilot 4) **** */

	/* Default: BULK DSP SIDE non presente */
	bulk_DSP_Side_ReqState = 0xFF;	/* Introdurre una define MQ */
	
	if( RegDev_Pos[LD_BULK_ACT_DSPSIDE_REG_IND] != NO_LD_POS )
	{
		/* BULK DSP SIDE presente */

		if( TESTBIT(FltBLDCUnblock.flags,PUMPOFF) )
		{
			/* Spegnimento forzato della BULK per routine di sblocco */
			bulk_DSP_Side_ReqState = LOAD_OFF;
		}
		else
		{
			/* Normale gestione */
			
			if( Interpreter_Data[0].completeLabel.s.label == LBL_RUN )
			{
				/* In RUN BULK DSP sempre pilotata */
				bulk_DSP_Side_ReqState = LOAD_ON;	
			}
			else
			{
				/*
				Se non in RUN, BULK DSP pilotata se almeno una delle pompe BLDC
				è richiesta attiva.
				*/
				if( ( BLDC_washPumpState == LOAD_ON ) || ( BLDC_drainPumpState == LOAD_ON ) )
				{
					/* Almeno una delle due pompe BLDC è richiesta attiva */
					bulk_DSP_Side_ReqState = LOAD_ON;	
				}
				else
				{
					/* Nessuna delle due pompe BLDC è richiesta attiva */
					bulk_DSP_Side_ReqState = LOAD_OFF;	
				}
			}
		}
		
		/*
		Posso agire direttamente su LoadsReq_New[]: per i carichi BULK DSP SIDE,
		BULK uPR SIDE e KILL_DSP lo stato richiesto dai microprogrammi è ignorato.
		*/
		LoadsReq_New[RegDev_Pos[LD_BULK_ACT_DSPSIDE_REG_IND]].b = bulk_DSP_Side_ReqState;
	}

	
	/* **** Impostazione dello stato richiesto per la BULK uPR SIDE (pilot 1-2-3) **** */
	
	/* Default: BULK uPR SIDE non presente */
	bulk_uP_Side_ReqState = 0xFF;	/* Introdurre una define MQ */

	if( RegDev_Pos[LD_BULK_ACT_UPSIDE_REG_IND] != NO_LD_POS )
	{
		/* BULK uPR SIDE presente */

		if( TESTBIT(FltBLDCUnblock.flags,PUMPOFF) )
		{
			/* Spegnimento forzato della BULK per routine di sblocco */
			bulk_uP_Side_ReqState = LOAD_OFF;
		}
		else
		{

			if( ( BLDC_drainPumpState == LOAD_ON ) && ( BLDC_washPumpState == LOAD_OFF ) )
			{
				/*
				La pompa scarico è BLAC ed è richiesta attiva e la pompa lavaggio o
				non è BLAC o comunque è richiesta spenta (è una sicurezza
				ulteriore in ottica certificazione, in realtà è una situazione che
				non dovrebbe mai verificarsi, funzionalmente le due pompe non devono
				mai essere accese insieme).
				*/
				bulk_uP_Side_ReqState = LOAD_ON;
			}
			else
			{
				/* La pompa scarico non è BLAC o è richiesta spenta */
				bulk_uP_Side_ReqState = LOAD_OFF;
			}
		}
		
		/*
		Posso agire direttamente su LoadsReq_New[]: per i carichi BULK DSP SIDE,
		BULK uPR SIDE e KILL_DSP lo stato richiesto dai microprogrammi è ignorato.
		*/
		LoadsReq_New[RegDev_Pos[LD_BULK_ACT_UPSIDE_REG_IND]].b = bulk_uP_Side_ReqState;
	}


	/* **** Determinazione dello stato filtrato della BULK **** */

	if( ( bulk_DSP_Side_ReqState != LOAD_OFF )
		&&
		( ( bulk_uP_Side_ReqState == LOAD_ON ) || ( TESTBIT(GICriteria,GI_CR_DOOR_IND) != 0 ) )
	  )
	{
		/*
		BULK attiva: relay collegato a GND da LD_BULK_ACT_DSPSIDE_REG_IND
		e collegato a RTN da LD_BULK_ACT_UPSIDE_REG_IND o dalla porta chiusa.
		*/
		/*
		E' corretto usare come informazione lo stato che verrà richiesto per la
		BULK in questa iterazione (e non ad esempio quello all'iterazione precedente)?
		Dato che l'informazione viene utilizzata per decidere lo stato che sarà
		richiesto al KILL DSP in questa iterazione, usando bulk_DSP/uP_Side_ReqState e
		e non considerando altri filtraggi, l'accensione (o lo spegnimento) di KILL DSP
		e BULK sarà richiesto nella stessa iterazione.
		Eventuali problemi potrebbero nascere da specifiche che richiedono di accendere
		un carico prima dell'altro o viceversa, e dal fatto che, nell'ambito della stessa
		iterazione, l'ordine di attuazione dello stato dipende dall'Output Map.
		- In spegnimento: è richiesto che il KILL DSP venga spento con qualche secondo
		  di ritardo rispetto alla BULK. Questo è gestito da un apposito contatore,
		  per cui la BULK è spenta nella stessa iterazione in cui bulk_DSP/uP_Side_ReqState
		  la indica come spenta (a meno di Pylot Type di tipo ritardato, di cui si dovrà
		  tener conto nel definire il valore del contatore), mentre il KILL DSP qualche
		  secondo più tardi. Quindi non ci sono problemi.
		- In accensione: è richiesto che il DSP sia acceso prima della BULK, quindi,
		  richiedendone l'accensione nella stessa iterazione, potrebbero nascere dei
		  problemi se nell'Ouput Map la BULK precede il KILL DSP.
		  Tuttavia, se anche vengono accesi entrambi nella stessa iterazione, il fatto
		  che la BULK sia comandata da un relay ci assicura qualche millisecondo di margine,
		  e quindi anche in questo caso non ci sono problemi.
		  L'importante è che siano effettivamente accesi nella stessa iterazione, e non
		  il KILL DSP un'iterazione (== 100 ms) dopo.
		  In caso di dubbi si potrebbe scegliere per la BULK un Digital Delayed, ma
		  poi occorre tenerne conto anche in spegnimento. MQ
		*/
		bulkFilteredState = LOAD_ON;
	
		oldBulkState = LOAD_ON;
	}
	else
	{
		/*
		BULK non attiva.
		*/
		if( oldBulkState == LOAD_ON )
		{
			/* Transizione ON -> OFF della BULK: filtro sullo spegnimento */

			(void)SWTC_SetSValueAndRun((SWTC_CountersType)HIO_BULK_OFF_DELAY_S_COUNTER, (SWTC_SCounterValue)Bulk_DSP_Off_Delay);
		}

		if( SWTC_IsExpired((SWTC_CountersType)HIO_BULK_OFF_DELAY_S_COUNTER) ) 
		{
			 /* BULK non attiva e timeout scaduto */

			bulkFilteredState = LOAD_OFF;

			(void)SWTC_Stop((SWTC_CountersType)HIO_BULK_OFF_DELAY_S_COUNTER);
		}
		else
		{
			/* BULK non attiva ma timeout non scaduto */

			bulkFilteredState = LOAD_ON;

			(void)SWTC_Run((SWTC_CountersType)HIO_BULK_OFF_DELAY_S_COUNTER); 
		}

		oldBulkState = LOAD_OFF;
	}


	/* **** Impostazione dello stato richiesto per il KILL DSP **** */

	if( ( ( Interpreter_Data[0].completeLabel.s.label == LBL_PROGRAMMING ) &&
			(Interpreter_Data[0].phase == 0) )
		||
		( TESTBIT(FltBLDCUnblock.flags,PUMPOFF) )
	  )
	{
		/* Richiesta di spegnimento per tentativi di sblocco o OFF logico */

		/*
		Il KILL DSP segue lo stato filtrato della BULK: se è attiva o spenta
		da poco il KILL DSP dev'essere attivo, se è spenta da abbastanza tempo
		il KILL DSP può essere spento.
		*/
		killDSPReqState = bulkFilteredState;
	}
	else
	{
		/*
		Non siamo in OFF LOGICO nè durante i tentativi di sblocco:
		il KILL DSP dev'essere attivo.
		*/
		killDSPReqState = LOAD_ON;
	}
	
	/*
	Posso agire direttamente su LoadsReq_New[]: per i carichi BULK DSP SIDE,
	BULK uPR SIDE e KILL_DSP lo stato richiesto dai microprogrammi è ignorato.
	*/
	LoadsReq_New[RegDev_Pos[LD_KILL_DSP_REG_IND]].b = killDSPReqState;

}


/**
<HIO_Check_VS_Valid> 

\ param  [in]		pump index 
\author 	Emanuela Antognoli
\date		 19/07/11
\version	1.0.0
\bug		
\
\note	
questa funzione mi dice se il dato di quel determinato sensore virtuale è valido oppure no.
non devono esserci fault /warning attivi e non deve mancare la comunicazione con il DSP e il motore deve girare.
 */
#if (VIRTUAL_SENSOR_ENABLE == 1)
uint8_t HIO_Check_VS_Valid(uint8_t aux_index)
{
/*	if (FltBLDCUnblock.data_ready_4SV==FALSE )
	{
		return FALSE;
	}
	else{*/
	if (LoadsReq_Old[RegDev_Pos[LD_KILL_DSP_REG_IND]].b==0 )
	{/* in realtà il none del pin è sbagliato : 0 vuol dire che il dsp non è alimentato, 1 vuol dire che è ALIVE*/
		return FALSE;
	}
	else{
	
		if (GET_BLDC_SPEED(BLDC_PumpReq_Old[aux_index])>0)
		{
			if ( BLDC_Pump_InfoBuffer[aux_index][BLDC_FLAGS_MSB] & 0x08)
			{/* questo flag lo passa il DSP quando lo stato del sensore è valido ( rampa+4 sec)*/ 
				
				return TRUE;
			}	
		}
	
	}	
	return FALSE;
}
/**
<HIO_Check_VS_Stability> 

\ param  [in]		carico  
\author 	Emanuela Antognoli
\date		 19/07/11
\version	1.0.0
\bug		
\
\note	
quando il motore cambia velocità o stato è necessario aspettare un tempo per permettere di rendere stabile ed affidabile il sensore.

il timer viene inizializzato a -1(expired) in modo che solo il cambiamento di velocità delle influisce sulla partenza di questo contatore.


*/

void HIO_Check_VS_Stability(void)
{
	uint16_t speedy_check;
	
	speedy_check= GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP]);
	
	if ( (speedy_check!=GET_BLDC_SPEED(BLDC_PumpReq_Old[WASH_BLDC_PUMP]) )&& (speedy_check>0))  /* cambio stato alla pompa*/
	{
		(void)Set_SmallCounter((SWTC_CountersType)HIO_STABILITY_FILTER_VS, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)FILTER_WASH_VS);
	}else{
		
		if ( (speedy_check>0)&& (MAKEWORD(BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VS_STATE_MSB], BLDC_Pump_InfoBuffer[WASH_BLDC_PUMP][BLDC_VS_STATE_LSB]) == 4))
		{
				(void)Set_SmallCounter((SWTC_CountersType)HIO_STABILITY_FILTER_VS, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)FILTER_WASH_VS);
		}else
		{	
			speedy_check= GET_BLDC_SPEED(BLDC_PumpReq_New[DRAIN_BLDC_PUMP]);
			
			if ( ( speedy_check!= GET_BLDC_SPEED(BLDC_PumpReq_Old[DRAIN_BLDC_PUMP]))&&(speedy_check>0)) /*  cambio stato alla pompa scarico */
			{
				(void)Set_SmallCounter((SWTC_CountersType)HIO_STABILITY_FILTER_VS, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)FILTER_DRAIN_VS);
			}else
			{
				if ( (speedy_check>0)&& (MAKEWORD(BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][BLDC_VS_STATE_MSB], BLDC_Pump_InfoBuffer[DRAIN_BLDC_PUMP][BLDC_VS_STATE_LSB]) == 4))
				{
					(void)Set_SmallCounter((SWTC_CountersType)HIO_STABILITY_FILTER_VS, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)FILTER_DRAIN_VS);
				}
			}
		}
	}	
	/* se entrambe le pompe sono spente non c'è bisogno che parte il contatore del filtro, tanto lo stato del sensore
	è comunque invalido perchè il motore non gira*/
	
	if ( SWTC_IsExpired((SWTC_CountersType)HIO_STABILITY_FILTER_VS))  
	{/* conteggio concluso */
	
		(void)SWTC_Stop((SWTC_CountersType)HIO_STABILITY_FILTER_VS); 
	}else
	{	
		(void)SWTC_Run((SWTC_CountersType)HIO_STABILITY_FILTER_VS);

	}
}




static void HIO_State5_SpeedControl(void)
{	

	if ( GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP]) ==0 )
	{
		return;
	}

	if( (Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH]==5)&&(TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH))) )
	{		
		if ( State5.max_speed_value==0xFFFF)
		{
			set_limit4_state5( (uint16_t)GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP]));
		}else
		{
			set_limit4_state5( State5.max_speed_value);
		}
		
	}
	if ( (State5.max_speed_value!=0xFFFF) && ( GET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP])> State5.max_speed_value )) 
	{ 
		SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],State5.max_speed_value);
	}
}


static void set_limit4_state5(uint16_t target)
{
#ifdef _STATISTICS_ON_
	uint16_t old_target;
	
	old_target=target;
#endif
	if ( target > SPEEDY_STEP_WASH)
	{
		State5.max_speed_value= target-SPEEDY_STEP_WASH;
	}
	if ( State5.max_speed_value < MIN_SPEED_WASH )
	{
		State5.max_speed_value= MIN_SPEED_WASH;
	}	
#ifdef _STATISTICS_ON_
	if (( old_target!=State5.max_speed_value)&& (State5.max_speed_value!=0xFFFF))
	{
		IncByteWithCheck(&MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].state5_reduction);
	}
#endif
}
#endif
/* End of file */
/* *********** */
