/**
Source File 

\file    AlternateControl.c
\ingroup AlternateControl
\date    4/03/2008
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author Roberto Fioravanti
*/


/** 
\note : 
*/ 


/* ******** */
/* Includes */
/* ******** */

#include "CstmHeaders.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#include "PM_AlternatoExp.h"
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* EA : la struttura per l'alternato a 6 posizioni cambia per il microprogramma a tabella , ma la lunghezza rimane sempre uguale.
quindi finchè si usa solo per determinare l'avanzamento alla posizione della tabella successiva è ok. altrimenti la nuova tabella è */
/*
struct
{
  uint8_t t_mnt;  
  uint8_t speed;
  uint8_t t_ramp;
  uint8_t pos: 4
  uint8_t unused:4 ;  
}up_Alt_setting_data_s;
*/
struct
{
  uint8_t pos:2;
  uint8_t t_mnt:6;  
  uint8_t speed;
  uint8_t t_ramp;
  uint8_t unused;  
}up_Alt_setting_data_s;




/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void ALT_uP_AlternateControl_Init(void); 
static uint8_t ALT_uP_AlternateControl_StepMgmt(uint8_t highStatement, uint8_t lowStatement);

static void ALT_SelectTableRuleByDishRackSelection(uint8_t dishrakcselection, uint8_t highStatement);
static uint8_t ALT_SelectTableRuleByZoneWashSelection(uint8_t regulation_value);
static uint8_t Could_Alternate_from_Virtual_Sensor ( void);
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
Gestisce l'esecuzione del microprogramma AlternateControl.

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

Inizializza la macchina a stati (ALT_uP_AlternateControl_Init).
sospende il microprogramma in particolari condizioni (ALT_uP_AlternateControl_Suspend).
Esegue la macchina a stati associata al microprogramma (ALT_uP_AlternateControl_StepMgmt).

\par \bSospensione del microprogramma
Il microprogramma viene sospeso in caso di apertura della porta.
La sospensione del microprogramma durante il posizionamento dell'alternato richiede la 
ripetizione della procedre di posizionamento (l'alternato è autoalimentato). 
Le condizioni in cui ciò può avvenire sono:

1. apertura della porta 
2. apertura del master relè

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti            


\todo   \par \bFaults
Gestione della sospensione del micorprogramma in presenza di faults
Attualmente la condizione sulla porta viene testata solo se non ci sono faults
*/


uint8_t ALT_uP_AlternateControl_Exe( uint8_t highStatement, uint8_t lowStatement)
{
uint8_t nRet=S_FALSE;


  if ( (FALSE == Interpreter_Data[0].stepInitialized) || (FALSE == Interpreter_Data[0].optionalStepInitialized) ) 
  {
    /* Init passo o restore from backup */
    
		ALT_uP_AlternateControl_Init();    
		return nRet;
  } 
  
  /* TODO1: condizioni particolari in cui deve essere sospesa l'attività del microprogramma */
/*    
  if( !Alarm )
  {
*/	
    if ( uPDoorStatus == UP_DOOR_WAIT )
	{/* porta aperta o in attesa di timeout dopo la chiusura o delay MR -> il uP va sospeso */
	  		  
	  (void)SWTC_Stop((SWTC_CountersType)MP_ALTCTRL_S_COUNTER);
	  
	  if (uP_AltCtrl_Data.step == ALT_CTRL_WAIT_ALTPOS_EXE)
      {
      	/* Se la condizione occorre durante il posizionamento dell'alternato 
		    si ripete la procedura
		    */
      	uP_AltCtrl_Data.step= ALT_CTRL_BEGIN;
      }

		return S_FALSE;  				    
	}
/*		
}
*/
	nRet= ALT_uP_AlternateControl_StepMgmt(highStatement, lowStatement);
  
  return nRet;   
}

/* ************** */
/* Local Function */
/* ************** */

/**
Inizializza la macchina a stati del microprogramma AlternateControl.

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

*/
static void ALT_uP_AlternateControl_Init(void) 
{
  uP_AltCtrl_Data.step = ALT_CTRL_INIT;
}



/**
Gestisce l'avanzamento della macchina a stati del microprogramma AlternateControl

\par \b Opzioni:
-incremento della posizione corrente dell'alternato
-posizionamento assoluto dell'alternato
-posizionamento combinato con attivazione pompa (sia BLDC che sincrona)

\par \b Condizione di avanzamento:
Nel caso di incremento della posizione corrente e posizionamento assoluto la condizione 
consiste nel raggiungimento della posizione da parte dell'alternato.
Nel posizionamento combinato con attivazione della pompa ci sono due sotto-casi.
-mezzocarico:la condizione di avanzamento è il completamento dell'esecuzione della tabella 
per quel cesto
-cesto non selezionato:esecuzione di tutte le tabelle.

\par \bVariante mezzocarico
In caso di mezzocarico selezionato il microprogramma non esegue tutte le tabelle ma solo
quella compatibile con la selezione dell'utente.

\par \b Formato dati:
Vel:
-pompa sincrona:
ON  = FFh (per permettere la eventuale gestione della pompa asincrona )
OFF = 0h
-pompa BLDC
rpm/25
tempo di rampa:
-pompa sincrona
0
-pompa BLDC
s/500ms

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata:
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/
static uint8_t ALT_uP_AlternateControl_StepMgmt(uint8_t highStatement, uint8_t lowStatement) 
{
  uint8_t nRet=S_FALSE;
  uint8_t alternatecmd, altpos;
  uint8_t table_displacement=0;
  
    
  if(SWTC_IsExpired(MP_ALTERNATE_UP_VS_FILTER_S_TIMER)|| (HWC_Flags.bit.vs_wash_presence==0))
  {
    uP_AltCtrl_Data.SV_invalid_4alt=FALSE;
    (void)SWTC_Stop((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER);
  }

  switch(uP_AltCtrl_Data.step) 
  {
    case ALT_CTRL_INIT:
    
      	if (BasicLoader(PTR_UP_ALTERNATECONTROL, lowStatement, &uPAlternateCtrl_setting_data) != S_OK ) 
  		{		 			

  				uP_AltCtrl_Data.step = ALT_CTRL_ERROR;
  				return S_OK;
  		}
		if ( (highStatement & ALT_CTRL_OPTION) == ALT_CTRL_TABLE_PPOS)
		{
			table_displacement=ALT_SelectTableRuleByZoneWashSelection(ZoneWashFunctionValue);
			if (BasicLoader(PTR_UP_ALTERNATECONTROL, table_displacement, &uPAlternateCtrl_setting_data) != S_OK ) 
  			{	
				uP_AltCtrl_Data.step = ALT_CTRL_ERROR;
  				return S_OK;
			}
			
		}
  		
  		uP_AltCtrl_Data.step=ALT_CTRL_BEGIN;
  		
  	/* -fallthrough */  		
    case ALT_CTRL_BEGIN:
  		switch((highStatement & ALT_CTRL_OPTION)) 
  		{
  		  case ALT_CTRL_RELATIVE_POS:
		 	if( Could_Alternate_from_Virtual_Sensor()==ALTERNATE_KO)
			{	/* non posso alternare */
				nRet=S_OK;	
				return nRet;
			}
  		    /* posizione di default */
  		    alternatecmd=0;
  		    
  		    altpos=uP_AltCtrl_Data.curr_pos;

  		    if(altpos>=PM_ALT_getPosNotDef( ))
  		    {
  		      alternatecmd=ALT_CTRL_SYNC_MSK;
  		    } else 
  		    {  		      
  		      alternatecmd=altpos+1;
  		      alternatecmd%=PM_ALT_getNumPos();
  		    }

			 uP_AltCtrl_Data.SV_invalid_4alt=TRUE;
			(void)SWTC_SetSValue((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER, (SWTC_SCounterValue)OFFS_B(Tab_SFT_MotorWash.Data,1));
		    (void)SWTC_Run((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER);
  		    LoadsReq_New[Alt_Pos].b=alternatecmd;
  		    uP_AltCtrl_Data.step=ALT_CTRL_WAIT_ALTPOS_EXE;
			 uP_AltCtrl_Data.curr_cmd=alternatecmd;
  		    
  		  break;
  		  
  		  case ALT_CTRL_ABSOLUTE_POS:
		  	if( Could_Alternate_from_Virtual_Sensor()==ALTERNATE_KO)
			{	/* non posso alternare */
				nRet=S_OK;	
				return nRet;
			}
  		    alternatecmd=ALT_CTRL_GET_ALT_POS(0,highStatement);
  		    alternatecmd|=ALT_CTRL_SYNC_MSK;  		    
  		    
  		    LoadsReq_New[Alt_Pos].b=alternatecmd;
  		    uP_AltCtrl_Data.curr_cmd=alternatecmd;
			
			uP_AltCtrl_Data.SV_invalid_4alt=TRUE;
			(void)SWTC_SetSValue((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER, (SWTC_SCounterValue)OFFS_B(Tab_SFT_MotorWash.Data,1));
		    (void)SWTC_Run((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER);
			uP_AltCtrl_Data.step=ALT_CTRL_WAIT_ALTPOS_EXE;
  		    
  		    
  		    
  		  break;
  		  case ALT_CTRL_TABLE:
		  case ALT_CTRL_TABLE_PPOS:
		  
  		    uP_AltCtrl_Data.step=ALT_CTRL_SELECT_TABLE_EXE;
  		    
			break;
  		  default:
  		    
  		    uP_AltCtrl_Data.step=ALT_CTRL_ERROR;
  		  
  		}  		
        if((highStatement &  ALT_CTRL_OPTION_TABLE) != ALT_CTRL_TABLE)
        {        
      		break;	                    
        }
    /* -fallthrough */  
    case ALT_CTRL_SELECT_TABLE_EXE:
      /* todo: solo con BLDC? oppure anche per la sincrona? */
      if ((DishRackFunctionValue & DISH_RACK_FVALUE_MSK)&& ((highStatement &  ALT_CTRL_TABLE_PPOS)!=ALT_CTRL_TABLE_PPOS ))
      {
        ALT_SelectTableRuleByDishRackSelection(DishRackFunctionValue,highStatement);
	
      }     
      
      uP_AltCtrl_Data.timing=1;

#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
		if(  HWC_Flags.bit.bldc1 )

	    {
	      uP_AltCtrl_Data.timing+=ALT_CTRL_GET_RAMPTIME(0);
	    } 
#endif
		(void)Set_SmallCounter((SWTC_CountersType)MP_ALTCTRL_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_100MS, (SWTC_SCounterValue)0);
      
      
		uP_AltCtrl_Data.step=ALT_CTRL_WAIT_RAMP_EXE;
		/* -fallthrough */
    case ALT_CTRL_WAIT_RAMP_EXE:
      
      /* ribadire il comando alla pompa */       
		if(ALT_CTRL_GET_SPEED(0))
		{
		    
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
			if( HWC_Flags.bit.bldc1  )
            {	                                           
                SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP], ALT_CTRL_GET_RAMPTIME(0)*ALT_CTRL_BLDC_RAMP_TIME_MUL);
    	        SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],ALT_CTRL_GET_SPEED(0)*ALT_CTRL_BLDC_SPEED_MUL);
            }

#endif            
            SET_PUMP(LD_WASH_PUMP_IND, LOAD_ON)
			
		}
		
	  	
		
		if(SWTC_IsExpired((SWTC_CountersType)MP_ALTCTRL_S_COUNTER)) 
		{
			 	
			if(--uP_AltCtrl_Data.timing) 
			{
				(void)SWTC_SetSValue ((SWTC_CountersType)MP_ALTCTRL_S_COUNTER,(SWTC_SCounterValue)ALT_CTRL_BLDC_RAMP_TIME_MUL);
			}
			else
			{
				
				
				if( Could_Alternate_from_Virtual_Sensor()==ALTERNATE_KO)
				{	/* non posso alternare */
					nRet=S_OK;
					uP_AltCtrl_Data.timing=	1;	
					return nRet;
				}
				if ((DishRackFunctionValue & DISH_RACK_FVALUE_MSK) && ((highStatement &  ALT_CTRL_TABLE_PPOS)!=ALT_CTRL_TABLE_PPOS) )
				{/* con l'alyternato a 6 posizioni non ci sar� mail il mezzo carico attuale. 
				 anche se ci fosse sarebbe definito come zoneWash con due zone*/
					/* cesto selezionato */
					alternatecmd=(DishRackFunctionValue & DISH_RACK_FVALUE_MSK);

				} else 
				{
					alternatecmd=ALT_CTRL_GET_ALT_POS(0,highStatement);
				}

			  if(uP_AltCtrl_Data.curr_pos>=PM_ALT_getPosNotDef())
			    {
			      alternatecmd|=ALT_CTRL_SYNC_MSK;
			    }
				
			  
	 			if (uP_AltCtrl_Data.curr_cmd!=  alternatecmd)
				{
				  uP_AltCtrl_Data.SV_invalid_4alt=TRUE;
				  (void)SWTC_SetSValue((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER, (SWTC_SCounterValue)OFFS_B(Tab_SFT_MotorWash.Data,1));
			      (void)SWTC_Run((SWTC_CountersType)MP_ALTERNATE_UP_VS_FILTER_S_TIMER);
				} 
				
				LoadsReq_New[Alt_Pos].b=alternatecmd;
				uP_AltCtrl_Data.step=ALT_CTRL_WAIT_ALTPOS_EXE;
				uP_AltCtrl_Data.curr_cmd=alternatecmd;
			}
		}
		else
		{

		}
        (void)SWTC_Run((SWTC_CountersType)MP_ALTCTRL_S_COUNTER);
    
      break;

    case ALT_CTRL_WAIT_ALTPOS_EXE:
      /* ribadire il comando alla pompa BLDC e (a)sync */  
      if((highStatement & ALT_CTRL_OPTION_TABLE)) 
      {
        
        if(ALT_CTRL_GET_SPEED(0)) 
        {                          
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
/*lint -e572*/
			if( HWC_Flags.bit.bldc1  )
      	    {	           
      	          	      
          	    SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP], 0);
                SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],ALT_CTRL_GET_SPEED(0)*ALT_CTRL_BLDC_SPEED_MUL);
      	    }

#endif  

            SET_PUMP(LD_WASH_PUMP_IND, LOAD_ON)			
        }    
      }
      

      if(PM_ALT_posReached())
      {
    	 uP_AltCtrl_Data.curr_pos=PM_ALT_getPos();
        
        switch((highStatement & ALT_CTRL_OPTION)) 
  		{
			case ALT_CTRL_TABLE:
			case ALT_CTRL_TABLE_PPOS:
				 /*carica il tempo di mantenimento della posizione*/

				(void)Set_SmallCounter((SWTC_CountersType)MP_ALTCTRL_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_1SEC, (SWTC_SCounterValue)0);
				uP_AltCtrl_Data.timing=1+ALT_CTRL_GET_MNT_TIME(0);
				if ( uP_AltCtrl_Data.seq_done_could_esc)
				{
					nRet=S_OK;
				}

				uP_AltCtrl_Data.step=ALT_CTRL_MNT_EXE;
			break;

			case ALT_CTRL_RELATIVE_POS:
			case ALT_CTRL_ABSOLUTE_POS:
				nRet=S_OK;
				uP_AltCtrl_Data.step=ALT_CTRL_END;
			break;

			default:
			
			break;
  		  }
      }
      else
      {
    	uP_AltCtrl_Data.curr_pos=PM_ALT_getPosNotDef();
		uP_AltCtrl_Data.SV_invalid_4alt=TRUE;
        if((highStatement & ALT_CTRL_OPTION)==ALT_CTRL_ABSOLUTE_POS) 
        {
          LoadsReq_New[Alt_Pos].b=uP_AltCtrl_Data.curr_cmd;
        }
      }
      break;
      
    case ALT_CTRL_MNT_EXE:
      /* ribadire il comando alla pompa BLDC e (a)sync */ 
	  if ( uP_AltCtrl_Data.seq_done_could_esc)
	  {
			nRet=S_OK;	
	  }
	  if(ALT_CTRL_GET_SPEED(0)) 
      {
      
#ifdef CONFIG_LIB_EXTERNAL_BOARD_SUPPORT
		if( HWC_Flags.bit.bldc1  )

	    {	      
	      SET_BLDC_TIME(BLDC_PumpReq_New[WASH_BLDC_PUMP], 0);
	      SET_BLDC_SPEED(BLDC_PumpReq_New[WASH_BLDC_PUMP],ALT_CTRL_GET_SPEED(0)*ALT_CTRL_BLDC_SPEED_MUL);

		}
#endif


        SET_PUMP(LD_WASH_PUMP_IND, LOAD_ON)
		
		
      }   
      
      if(SWTC_IsExpired((SWTC_CountersType)MP_ALTCTRL_S_COUNTER))       
      {
        if(--uP_AltCtrl_Data.timing) 
        {
		  (void)SWTC_SetSValue ((SWTC_CountersType)MP_ALTCTRL_S_COUNTER,(SWTC_SCounterValue)ALT_CTRL_MNT_TIME_MUL);
        }
        else 
        {
          uPAlternateCtrl_setting_data.DataLenght-=sizeof(up_Alt_setting_data_s)/2;
          
         if( ( uPAlternateCtrl_setting_data.DataLenght )
         	 &&
         	 !( (( DishRackFunctionValue & DISH_RACK_FVALUE_MSK ) && ((highStatement &  ALT_CTRL_TABLE_PPOS)!=ALT_CTRL_TABLE_PPOS))
         	    &&
 				( HWC_Flags.bit.bldc1 )
         	  )
         	)
         {
            /* sequenza non terminata */
            uPAlternateCtrl_setting_data.Data+=sizeof(up_Alt_setting_data_s);
            uP_AltCtrl_Data.step=ALT_CTRL_BEGIN;
            
          }
          else           
          {
            /* sequenza terminata: ripete la sequenza */
            uP_AltCtrl_Data.step=ALT_CTRL_INIT;
			uP_AltCtrl_Data.seq_done_could_esc=TRUE;
            nRet=S_OK;
          }
        }	
		
      }
      
      (void)SWTC_Run((SWTC_CountersType)MP_ALTCTRL_S_COUNTER);
      break;
    case ALT_CTRL_END:
      nRet=S_OK;
      break;
    default:
      uP_AltCtrl_Data.step=ALT_CTRL_INIT;
      nRet=S_FALSE;
    
  }    

  return nRet;
}

/**
Seleziona la tabella in base al mezzocarico selezionato spostando i puntatori alle tabelle del 
up Alternato.
La logica di selezione è la seguente:
-Nel primo tentativo viene ricercata la tabella compatibile con il mezzocarico selezionato.
-Nel caso in cui la tabella non è presente, si effettua il secondo tentativo che ricerca la 
tebella per l'altro mezzocarico.
-Nel caso che la tebella non è presente si ricerca la tabella per entrambi i cesti

In tutti i tentativi vengono prese in considerazione le tabelle con velocità e tempo di 
mantenimento non nulli, escludendo le tabelle adoperate per la movimentazione dell'alternato.


\param [in] dishrackselection
 
\return     none
\attention  modifica uPAlternateCtrl_setting_data
\version		1.0.0
\date 			11/03/2008
\author			Roberto Fioravanti
            
*/
static void ALT_SelectTableRuleByDishRackSelection(uint8_t dishrackselection,uint8_t highStatement) 
{
  uint8_t found=FALSE;
  uint8_t drdatafound=FALSE;
  uint8_t i=0,nrows=uPAlternateCtrl_setting_data.DataLenght/(sizeof(up_Alt_setting_data_s)/2), numTentResearch=0;
  
  while(!found) 
  {
    while(((i/sizeof(up_Alt_setting_data_s))<nrows) && (!drdatafound)) 
    {      
      /* posizione cesto della riga (i/sizeof(up_Alt_setting_data_s))-esima */
      if((ALT_CTRL_GET_ALT_POS(i,highStatement)==dishrackselection) && ( ALT_CTRL_GET_MNT_TIME(i)!=0)) 
      {
        drdatafound=TRUE;
      } else
      {        
        i+=sizeof(up_Alt_setting_data_s);
      }
    }
    

    if ( drdatafound )
		{
			found = TRUE;
		}
		else
		{
			i=0;
			numTentResearch ++;
			
			if ( numTentResearch == 1 )
			{
				if ( dishrackselection == b0 )
				{
					dishrackselection = b1;
				}
				else
				{
					dishrackselection = b0;
				}
			}
			else if ( numTentResearch == 2 )
			{
				dishrackselection = 0;
			}
			else
			{
				/* tutti i tentativi non hanno prodotto un campo dati corretto 
				   prendo il primo */
				i = 0;
				found = TRUE;				    			
			}
		}
		
  }
  
  uPAlternateCtrl_setting_data.Data+=i;
  uPAlternateCtrl_setting_data.DataLenght-=i/(sizeof(up_Alt_setting_data_s)/2);
  
  
  /* modifica uPAlternateCtrl_setting_data.Data */
}

/**
Verifica lo stato del sensore virtuale , in base al quale si decide di alternare o meno

\param [in] none
 
\return     none
\version		1.0.0
\date 			01/12/2011
\author			Emanuela Antognoli
            
*/
static uint8_t Could_Alternate_from_Virtual_Sensor ( void)
{
		/* LR la pompa resta attiva, e si aspetta che il riconoscimento sporco/schiuma */
	if(HWC_Flags.bit.vs_wash_presence)
	{
#if (VIRTUAL_SENSOR_ENABLE == 1)
		if ( dirty_foam_Data.filter_foam_on )
		{/* se entro qui il riconoscimento sporco e schiuma sta intervenendo e 
		quindi l'alternato è in pausa*/
			return ALTERNATE_KO;		            	            
		}
#endif
	
	    if (  TESTBIT(GIValid, (GI_FIRST_VIRTSENSOR_BIT+GI_VIRTUAL_SENSOR_WASH)) )
		{	
			switch(Virtual_Sensor_Value[GI_VIRTUAL_SENSOR_WASH])
			{ 
				case 2:
					return ALTERNATE_OK  ;

				  break;

				case 0:
				case 3:
				case 1:
				default:/* comprende anche lo stato 5*/
				return ALTERNATE_KO;				    					
			}	            
		}
		
	}	
	/* se il valore del sensore non è ready comunque provo ad alternare oppure non ci sono i sensori*/
	return ALTERNATE_OK;
}



/**
Seleziona la tabeRestituisce il displacement del up Alternato dela tabella della zone wash selezionata.


\param [in] ZoneWashFunctionValue 
\param [in] uPAlCTRL_setting_preview tabella dei displacement

\return     displacement

\version		1.0.0
\date 			26/03/2012
\author			Emanuela Antongoli
            
*/
static uint8_t ALT_SelectTableRuleByZoneWashSelection(uint8_t regulation_value) 
{
	uint8_t found=FALSE;
	uint8_t drdatafound=FALSE;
	struct
	{
		uint8_t reg_val : 4;  
		uint8_t free    : 4;
		uint8_t displacement;
		
	}up_reg_displ_setting_s;

 	uint8_t i=0,nrows=uPAlternateCtrl_setting_data.DataLenght/(sizeof(up_reg_displ_setting_s)/2);
  
  
	while(!found) 
	{/* il numero di byte per ogni informazione  dacercare � pari a 2*/
		while(((i/sizeof(up_reg_displ_setting_s))<nrows) && (!drdatafound)) 
		{      
		  /* posizione cesto della riga (i/sizeof(up_Alt_setting_data_s))-esima */

			if ( regulation_value <=( OFFS_B(uPAlternateCtrl_setting_data.Data,i)& 0x0F) )
			{
				drdatafound=TRUE;
			}else 
			{        
				i+=sizeof(up_reg_displ_setting_s);
			}
		}


		if (! drdatafound )
		{/* tutti i tentativi non hanno prodotto un campo dati corretto 
			   prendo il primo */
			i=0;
		}
		
		found = TRUE;				    			
		

	}
	return (OFFS_B(uPAlternateCtrl_setting_data.Data,i+1));

}


/**
Imposta la posizione dell'Alternato, se presente nel file di setting
\param [in]		pos : Posizione

\version		1.0.0
\date 			18/09/2008
\author			Roberto Fioravanti

\note	        variabili coinvolte
uP_AltCtrl_Data
LoadsReq_New
LoadsReq_Old

*/
void HIO_SetAltPosition(uint8_t pos)
{

    if( HWC_Flags.bit.alternate )
    {
    	uP_AltCtrl_Data.curr_pos = pos;

    	LoadsReq_New[Alt_Pos].b=pos;
    	LoadsReq_Old[Alt_Pos].b=pos;
    }
}

/* *********** */
/* End of file */
/* *********** */
