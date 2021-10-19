/**
Source File 

\file    SparklingAid.c
\ingroup SparklingAid
\date    17/03/2008
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, IDD.
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

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
struct up_Spa_setting_data_s
{
  uint8_t d1;
  uint8_t d1_d2;
  uint8_t d2;
  uint8_t delta;
  uint8_t t1_d1;
  uint8_t t2_d2;
};

/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void SPA_uP_SparklingAid_Init(void);
static uint8_t SPA_uP_SparklingAid_StepMgmt(uint8_t highStatement, uint8_t lowStatement);

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
Gestisce l'esecuzione del microprogramma SparklingAid.
Inizializza la macchina a stati (SPA_uP_SparklingAid_Init).
sospende il microprogramma in particolari condizioni (SPA_uP_SparklingAid_Suspend).
Esegue la macchina a stati associata al microprogramma (SPA_uP_SparklingAid_StepMgmt).

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/


uint8_t SPA_uP_SparklingAid_Exe( uint8_t highStatement, uint8_t lowStatement)
{
uint8_t nRet=S_FALSE;


    /* Init passo o restore from backup */
  if ( (FALSE == Interpreter_Data[0].stepInitialized) )
  {
    
		SPA_uP_SparklingAid_Init();    
		return nRet;
  } 
  
  /* TODO1: condizioni particolari in cui deve essere sospesa l'attività del microprogramma */
/*    
  if( !Alarm )
  {
*/	
    if (uPDoorStatus == UP_DOOR_WAIT)
    {/* porta aperta o in attesa di timeout dopo la chiusura o delay MR -> il uP va sospeso */
   
  /*  DisableCounter(&uP_SparklingAid_Data.timer); */
    	SWTC_Stop((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER);
	  	if( (uP_SparklingAid_Data. step== SPRKLING_T1 ) || (uP_SparklingAid_Data.step == SPRKLING_T2) )
		{
	  		uP_SparklingAid_Data.cmd.RepeatTurbRead = TRUE;
		}


			return S_FALSE;  				    
		}
/*		
	}
*/
	nRet= SPA_uP_SparklingAid_StepMgmt(highStatement, lowStatement);
  
  return nRet;   
}

/* ************** */
/* Local Function */
/* ************** */

/**
Inizializza la macchina a stati del microprogramma SparklingAid.

\param nesssuno
\return nessuno

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

*/
static void SPA_uP_SparklingAid_Init(void)
{
  uP_SparklingAid_Data.step = SPRKLING_INIT;
}




/**
Gestisce l'avanzamento della macchina a stati del microprogramma SparklingAid.

\par \b Opzioni:
1-rilascio brillantante normale 
2-rilascio brillantante con lettura di torbidità

\par \b Condizione di avanzamento:
tutti gli step sono stati eseguiti:

1- normale
    doppia attivazione dispenser 
2- con lettura di torbidità
    doppia lettura del sensore di torbidità separate da una doppia attivazione dispenser.
    Confronto tra i valori delle due letture per la notifica dello stato del brillantante.

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\retvalue S_OK se la condizione di avanzamento passo per il microprogramma è verificata
\retvalue S_FALSE se non è ancora verificata

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti

\todo       -La condizione sulla porta viene testata solo se non ci sono faults
            
*/
uint8_t SPA_uP_SparklingAid_StepMgmt(uint8_t highStatement, uint8_t lowStatement) 
{
  uint8_t nRet=S_FALSE;
  
  if(uP_SparklingAid_Data.cmd.RepeatTurbRead ==TRUE)
  {
    uP_SparklingAid_Data.IdRead=TurbidityReads.id;
    uP_SparklingAid_Data.cmd.RepeatTurbRead =FALSE;
    HIO_trbReadEnable(TURBSENS_READ_up_SPA);
  }
  
  switch(uP_SparklingAid_Data.step) 
  {
    case SPRKLING_INIT:
		if (BasicLoader(PTR_UP_SPARKLING_AID, lowStatement, &uPSparklingAid_setting_data) != S_OK )
		{
				uP_SparklingAid_Data.step = SPRKLING_ERROR;
				return S_FALSE;
		}

		if (RinseAidFunctionValue==0)
		{
			return S_OK;
		}
		SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,0);
		
		switch((highStatement) & (SPRKLING_OPTION))
		{
			case SPRKLING_NORMAL_OPT:
          
				uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_D1;
			break;
        
			case SPRKLING_WITH_TURB_READ_OPT:
			case SPRKLING_WITH_REG_TURB_REG_OPT:
			  if(uPSparklingAid_setting_data.DataLenght<SPRKLING_TABLE_TURB_OPT_LEN)
			  {
				uP_SparklingAid_Data.step=SPRKLING_ERROR;
				return S_FALSE;
			  }
			  uP_SparklingAid_Data.step=SPRKLING_T1;
			  uP_SparklingAid_Data.cmd.RepeatTurbRead=TRUE;
          
			  break;
			case SPRKLING_WITH_REGULATION_OPT:
				uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_D1;
				uP_SparklingAid_Data.cmd.Repeat_reg_dx =0;
			

				break;
		
			default:
				uP_SparklingAid_Data.step=SPRKLING_ERROR;
		}
		  
	break;
    
    case SPRKLING_T1:
		if(GIValid&GI_TURB_SENSOR_MSK)
		{
			uint8_t id=TurbidityReads.id;

			/* attesa della nuova lettura */
			if(uP_SparklingAid_Data.IdRead!=id)
			{
			  /* nuova lettura disponibile */
			  uP_SparklingAid_Data.IdRead=id;

			  /* leggi valore torbidità */
			  uP_SparklingAid_Data.turbidity1=TurbidityReads.perc;
			  uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_D1;

			  SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_DELAY_T1_D1);

			  HIO_trbReadDisable((uint8_t)TURBSENS_READ_up_SPA);
			}
        }
      
    break;
    
    case SPRKLING_WAIT_FOR_D1:
    
		if(SWTC_IsExpired((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER))
		{
			uP_SparklingAid_Data.step=SPRKLING_D1;

			SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_TIME_D1);
		}
      
      break;
      
    case SPRKLING_D1:
      if(SWTC_IsExpired((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER))
      {
		 uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_D2;
		 SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_DELAY_D1_D2);
      } 
      else
      {
        /*turn on dispenser */
        LoadsReq_New[Disp_Pos].b=LOAD_ON;        
      }

      break;
      
    case SPRKLING_WAIT_FOR_D2:
		if(SWTC_IsExpired((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER))
		{	
			if ( (((highStatement) & (SPRKLING_OPTION))== (SPRKLING_WITH_REGULATION_OPT))||
			(((highStatement) & (SPRKLING_OPTION))== (SPRKLING_WITH_REG_TURB_REG_OPT)) )
			{
				uP_SparklingAid_Data.cmd.Repeat_reg_dx++;
				if(uP_SparklingAid_Data.cmd.Repeat_reg_dx >= RinseAidFunctionValue)
				{ /* finite le ripetizioni indicate dalla regolazione */
					if( ((highStatement) & (SPRKLING_OPTION))== (SPRKLING_WITH_REG_TURB_REG_OPT))
					{
						uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_T2;
					
					}else
					{
						uP_SparklingAid_Data.step=SPRKLING_END;
					}	
					
				}else
				{
					uP_SparklingAid_Data.step=SPRKLING_D1;
					SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_TIME_D1);     
				}
			}else
			{
	        	uP_SparklingAid_Data.step=SPRKLING_D2;
				SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_TIME_D2);
			}	
		
		} 

      break;
            
    case SPRKLING_D2:
      if(SWTC_IsExpired((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER))
      {
        
        switch((highStatement) & (SPRKLING_OPTION))
        {
          case SPRKLING_NORMAL_OPT:
            uP_SparklingAid_Data.step=SPRKLING_END;
            break;
          case SPRKLING_WITH_TURB_READ_OPT:
            uP_SparklingAid_Data.step=SPRKLING_WAIT_FOR_T2;
            break;        
          default:
          uP_SparklingAid_Data.step=SPRKLING_ERROR;
        }        
        
		SWTC_SetMValue((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER,SPRKLING_DELAY_D2_T2);
      }
      else
      {
        /*turn on dispenser */
        LoadsReq_New[Disp_Pos].b=LOAD_ON;
      }
      break;
      
    case SPRKLING_WAIT_FOR_T2:

	  if(SWTC_IsExpired((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER))
      {
        uP_SparklingAid_Data.step=SPRKLING_T2;        
        uP_SparklingAid_Data.cmd.RepeatTurbRead =TRUE;
      }    

      break;
    case SPRKLING_T2:
      if((GIValid&GI_TURB_SENSOR_MSK) && (uP_SparklingAid_Data.IdRead!=TurbidityReads.id))
      {      
        /* leggi valore torbidità */
        uP_SparklingAid_Data.turbidity2=TurbidityReads.perc;
        
        if((!HWC_Flags.bit.sparklaidreed) && 
            ((uP_SparklingAid_Data.turbidity2-uP_SparklingAid_Data.turbidity1)<=SPRKLING_TURB_DELTA) &&
            ((uP_SparklingAid_Data.turbidity1+SPRKLING_TURB_DELTA)<100)
            ) 
        {                
          /* notify sparkling KO!! */              
          SETBIT(Flags_2,FLAGS2_TURB_NO_SPARKL_BIT);              
        }                        
        else 
        {            
          CLRBIT(Flags_2,FLAGS2_TURB_NO_SPARKL_BIT);
        }
        
        uP_SparklingAid_Data.step=SPRKLING_END;
				
		HIO_trbReadDisable((uint8_t)TURBSENS_READ_up_SPA);
      }
      
      
      break;

    case SPRKLING_END:                                           
    case SPRKLING_ERROR:
      nRet=S_OK;     
      break;
    default:
      uP_SparklingAid_Data.step=SPRKLING_INIT;
      nRet=S_FALSE;
    
  }    
  
  SWTC_Run((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER);
  
  return nRet;
}
/**/


/* *********** */
/* End of file */
/* *********** */
