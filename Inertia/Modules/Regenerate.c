/**
Source File 

\file    Regenerate.c
\ingroup Regenerate
\date    26/06/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, IDD.
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
uint8_t RGN_checkCondReg (void);

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
Gestisce l'esecuzione del microprogramma Regenerate.
 
\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma � verificata (rigenera completata),
\return S_FALSE se non � ancora verificata(rigenera non ancora completata),
\return codice di errore in altri casi
 
\version		1.0.0
\date 			26/06/2008
\author			Valeria Catani

*/
uint8_t RGN_uP_Regenerate_Exe (uint8_t HighStatement,uint8_t LowStatement)
{
	Loader_Struct uPRegenerate_setting_data;


	if (Interpreter_Data[0].stepInitialized == FALSE)
	{	/* Prima iterazione del passo */
		
		uP_Regenerate_Data.status = REGENERATE_INIT;
		
	}
	
	if (uPDoorStatus == UP_DOOR_WAIT)
	{/* porta aperta -> il uP va sospeso */

    
    	(void)SWTC_Stop((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER);;
    
		return S_FALSE;
			    
	}
	
	
	if ( uP_Regenerate_Data.status == REGENERATE_INIT )
	{/* Init uP */
	
	  /* caricamento dati dalla flash */
	
		if ( BasicLoader( PTR_UP_REGENERATE, LowStatement, &uPRegenerate_setting_data ) != S_OK )
		{
			return S_OK;	/*non eseguo il uP, ma abilito avanzamento passo*/
		}
		
		/* inizializzazione dati uP */
		
		uP_Regenerate_Data.waterPulseRegReq = OFFS_B(uPRegenerate_setting_data.Data,1); /*acqua da impiegare per rigenera
		                                                                               espressa in impulsi turbina */
		
		uP_Regenerate_Data.resetAfterRegReq = FALSE;
		
		if ( TESTBIT ( OFFS_B(uPRegenerate_setting_data.Data,0), 7 ) )
		{ 
		  uP_Regenerate_Data.resetAfterRegReq = TRUE;
		}
		
		/* init var conteggio impulsi per rigenera */
		uP_Regenerate_Data.waterPulseReg = 0;
		
	  /* verifica condizioni per eseguire o meno rigenera */	
		
		if ( uP_Regenerate_Data.resetAfterRegReq == FALSE)
		{ /* non richiesto reset after reg -> primo passaggio di rigenera */
		  
			if ( RGN_checkCondReg() == TRUE )
			{/* la rigenera va eseguita */

			  SETBIT(Flags_2,FLAG2_REGENERATE_ON_POS);  /* per il passaggio di rigenera successivo */

			  /*uP_Regenerate_Data.status = REGENERATE_EXE_0;*/

			}
	    else
	    {/* la rigenera non va eseguita */
	      
	      uP_Regenerate_Data.status = REGENERATE_TRUE;
	      
	      return S_OK;   
	      
	    }
	    
		}
		else
		{ /* richiesto reset after reg -> secondo passaggio di rigenera */
		 
		  if ( TESTBIT(Flags_2,FLAG2_REGENERATE_ON_POS) == 0 )
		  {/*in base al test effettuato al passaggio precedente la rigenera non va eseguita */
		  
				uP_Regenerate_Data.status = REGENERATE_TRUE;

				return S_OK;
		      
		  }
		  
		  CLRBIT(Flags_2,FLAG2_REGENERATE_ON_POS);  
		}
		
		/*init counter per delay attivazione EVR - attivazione EVC*/
		(void)Set_SmallCounter((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_200MS, (SWTC_SCounterValue)(OFFS_B(Tab_HWCRegenerateData.Data,0)& 0x7F));

		if ( HWC_Flags.bit.vs_wash_presence != 0)
		{ /* l'unit� di misura in setting � 500 ms*/
			uP_Regenerate_Data.tout_cnt =(uint16_t)(OFFS_B(uPRegenerate_setting_data.Data,0) & 0x7F )* 5;

		}
		uP_Regenerate_Data.status = REGENERATE_EXE_0;
		
		
		return S_FALSE;	/* nella prima iterazione non vengono pilotati i carichi */
		
	}/*REGENERATE_INIT END*/
	
	if ( uP_Regenerate_Data.status == REGENERATE_EXE_0 )
	{
	  	if (SWTC_IsExpired((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER) )
	  	{/* conteggio ritardo EVR ON - EVC ON concluso */
	  
	    /*Set Timeout di sicurezza caricamento acqua per rigenera */
		    if (uP_Regenerate_Data.waterPulseRegReq != 0)
		    {/* richiesta quantità  acqua per rigenera non nulla */
	      
			    if ( HWC_Flags.bit.vs_wash_presence == 0)				
				{
				  /* init timeout di attivazione EV in base a portata storica, impulsi da caricare e margine di sicurezza */
	      
					uP_Regenerate_Data.tout_cnt =  ((uP_Regenerate_Data.waterPulseRegReq - uP_Regenerate_Data.waterPulseReg) * uP_Load_Data.levTOtimeRef * LEV_TO_TIME_REF_PRECISION) / 
								                    (FlowMeterDivider * (FULL_REF_LEVEL)) ;
		
				}
				if ( (PM_PulseCounter_GetWarning() == NO_PULSE_COUNTER_WARNING) && (TESTBIT(Flags_1,JIF_FLAG1_PULSE_COUNTER_KO_POS) == 0) )
				{ /*Pulse counter OK && non bloccata*/
    
	         		uP_Regenerate_Data.tout_cnt += MARGIN; 		/* aggiungo margine di sicurezza al tout di caricamento*/
		      	}
		  		if ( HWC_Flags.bit.vs_wash_presence != 0)	
				{
	      			PulseLoaded=0;
				}

		  		PM_PulseCounter_SetCount(uP_Regenerate_Data.waterPulseReg);/* init Pulse Counter */


		      	PM_PulseCounter_Reset_Warning(PULSECOUNTER_RESET_HIGH_WARNING); /* reset warning alta frequenza prima di attivare EV carico
		                                                                      potrebbe dipendere dall'attivazione EV rigenera */
	      
	      		uP_Regenerate_Data.status = REGENERATE_EXE_1;
	       						
	    	}
		    else
		    {/* non deve essere caricata acqua */
      
		      	/*init counter per delay disattivazione EVC - disattivazione EVR*/
			  	(void)Set_SmallCounter((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_200MS, (SWTC_SCounterValue)(OFFS_B(Tab_HWCRegenerateData.Data,1)& 0x7F));
			    
	
		      	uP_Regenerate_Data.status = REGENERATE_EXE_2;  
		    }
	    
	  	}
	  
	  	/* in ogni caso */	   
	  	(void)SWTC_Run((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER);
	   
	  	LoadsReq_New[EV_Pos[LD_RIG_EV_IND]].b = EV_ON;
	    
	}/* REGENERATE_EXE_0 END*/
	
	if (uP_Regenerate_Data.status == REGENERATE_EXE_1)
	{		
		/* aggiornamento impulsi caricati */
		
		uP_Regenerate_Data.waterPulseReg = (uint8_t)PulseLoaded;
	
		
		
		/* aggiornamento timeout di sicurezza */
		uP_Regenerate_Data.tout_cnt--;
		
	  /* controllo termine caricamento */
	  
	  if ( (uP_Regenerate_Data.waterPulseReg >= uP_Regenerate_Data.waterPulseRegReq) || 
	       (uP_Regenerate_Data.tout_cnt == 0) )
	  {
	    /*init counter per delay disattivazione EVC - disattivazione EVR*/
		(void)Set_SmallCounter((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER, (SWTC_PrescalerType)COUNTERS2_TBASE_200MS, (SWTC_SCounterValue)(OFFS_B(Tab_HWCRegenerateData.Data,1)& 0x7F));
    
	     uP_Regenerate_Data.status = REGENERATE_EXE_2;  
	    
	  }
	  
	  /* pilotaggio EVR - EVC */
	  LoadsReq_New[EV_Pos[LD_RIG_EV_IND]].b = EV_ON;
	  LoadsReq_New[EV_Pos[LD_WASH_EV_IND]].b = EV_ON;

	}/* REGENERATE_EXE_1 END*/
	
	if (uP_Regenerate_Data.status == REGENERATE_EXE_2)
	{
	
	  if (SWTC_IsExpired((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER) )
	  {/* conteggio ritardo EVC OFF - EVR OFF concluso */
		  
		  if ( uP_Regenerate_Data.resetAfterRegReq == TRUE)
		  {/* richiesto reset quantità acqua : processo di rigenera in 2 step concluso */
		     
		    /* aggiornamento numero di rigenere eseguite */
		    RGN_updateRegNumber();
		    		    		  
		    /* Flag rigenera eseguita (anche per JumpIF)*/
		    SETBIT(Flags_1, JIF_FLAG1_REGENERATE_OK_POS);
		    
		    if (uP_Regenerate_Data.forceReg_cnt != 0)
		    {
				Reg_WaterLoaded.l = WaterForRegenerate;

				uP_Regenerate_Data.forceReg_cnt --;
		    
		    }
		  
		  }
		  
		  uP_Regenerate_Data.status = REGENERATE_TRUE;
	  }
	  
	  /* in ogni caso */	   
	 (void)SWTC_Run((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER);

	  LoadsReq_New[EV_Pos[LD_RIG_EV_IND]].b = EV_ON;
	  
	}/*REGENERATE_EXE_2 END*/
	
	if ( uP_Regenerate_Data.status == REGENERATE_TRUE )
	{
	   return S_OK;
	
	}/*REGENERATE_TRUE END*/
	
	return S_FALSE;
	  
}





/* ************** */
/* Local Function */
/* ************** */

/**
Controlla le condizioni per effettuare la rigenera
 
 
\return S_OK se la condizione per la rigenera � verificata 
\return S_FALSE se non � verificata
\return codice di errore in altri casi
 
\version		1.0.0
\date 			27/06/2008
\author			Valeria Catani

*/
uint8_t RGN_checkCondReg (void)
{
uint16_t waterForReg;

  if (uP_Regenerate_Data.forceReg_cnt != 0)
  {/* rigenera forzata (con contatore) */
  
    return TRUE; 
  }
  
  if ( WaterForRegenerate == 0xFF )
	{/* Non ho dato di confronto per stabilire se eseguire rigenera */  /*CASO REALE?????*/
	
		return FALSE;
	}

/* rigenera abilitata anche dopo segnalazione sale assente */
/*	
	if ( TESTBIT(ApplicationFlags, AF_NO_SALT_BIT) )	
	{// E' stata comunicata mancanza sale alla UI
	
	  return FALSE; 
	}
*/	
  
  waterForReg = ( (uint16_t)Reg_WaterLoaded.l + (WaterInDW/10) );
  
  if ( waterForReg >= WaterForRegenerate  )
  {
    return TRUE;  
  }
  
  return FALSE;
    
}

/**
Aggiorna il numero di rigenere eseguite
 
 
\return 

\version		1.0.0
\date 			30/06/2008
\author			Valeria Catani

*/
void RGN_updateRegNumber (void)
{
uint8_t nMaxReg;

  
  nMaxReg = OFFS_B(Tab_HWCRegenerateData.Data,2);

  if (HWC_Flags.bit.saltreed)
  {/* reed sale presente */
  
    if ( NoSalt_Reg == TRUE )
    {/* richiesto aggiornamento num rigenere poich� sale assente */
    
            if ( uP_Regenerate_Data.nReg < 0xFF )
			{				
				uP_Regenerate_Data.nReg++;
			}
      
    }
  }
  else
  {/* reed sale assente */
  
    /* per compatibilit� con vecchia piattaforma, forse inutile! */
    
    uP_Regenerate_Data.nReg = nMaxReg;   
  }
  
    
}


/* *********** */
/* End of file */
/* *********** */
