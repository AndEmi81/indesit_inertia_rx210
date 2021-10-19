	/*
** **************************************************************************
** **************************************************************************
*/ 
/**
Gestisce il microprogramma LoadsControl per il pilotaggio dei carichi.

Source file
 
\file    	LoadsControl.c
 
\version 	1.0.0
\date    	21/02/2008
\author  Roberto Fioravanti

*/ 
/*
** **************************************************************************
** **************************************************************************
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
#define LDC_SEQ1_TABLE_LEN				3


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* **************************** */
/* Prototype of local functions */
/* **************************** */
static void LDC_pulseSeq_Init(loadsPulse_Struct *p,SWTC_CountersType indice);
static bool_t LDC_pulseStepMgmt(loadsPulse_Struct *p, SWTC_CountersType indice);
static void LDC_pulseLoadMgmt(loadsPulse_Struct const *p);
static void LDC_activateLoads(uint16_t loadBitmap);

/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

uint16_t enabledloads_mask;

/* ***************** */
/* Exported function */
/* ***************** */

/**
Gestisce l'esecuzione del microprogramma LoadsControl.
E' possibile attivare carichi in continua o a impulsi.
Nella modalità ad impulsi è possibile specificare il tipo di sequenza e 
fino ad un massimo di due set di carichi indipendenti.

Lo stato dei carichi non selezionati nella/e bitmap viene ignorato.

\param [in] HighStatement specifica l'high statement (opcode +  modificatori F0-F1 + N flag + T flag del microprogramma) .
\param [in] LowStatement specifica il low statement (displacement ai dati) del microprogramma.
 
\return S_OK se la condizione di avanzamento passo per il microprogramma è verificata (nel caso di pilotaggio ad impulsi tutti i profili eseguiti 
almeno una volta)
\return S_FALSE se non è ancora verificata
\return codice di errore in altri casi

\par \bSospensione del microprogramma
Il microprogramma (ritorma sempre S_FALSE) viene sospeso durante:
-porta aperta o timeout di porta aperta
-Master rele

In queste condizioni fa eccezione la pompa scarico che può continuare ad essere pilotata.


\note   Il controllo sulla sovrapposizione tra le due bitmap bitmap deve essere fatto in Editor.

\version		1.0.0
\date 			26/02/2008
\author			Roberto Fioravanti


*/
uint8_t LDC_uP_LoadsControl_Exe (uint8_t highStatement,uint8_t lowStatement)
{
Loader_Struct lcDirect_SettingData;
uint8_t nRet = S_FALSE;
bool_t  suspend;


	if ( (FALSE == Interpreter_Data[0].stepInitialized) || (FALSE == Interpreter_Data[0].optionalStepInitialized) )
	{/* Init passo o restore from backup */

		switch ( (highStatement & LDC_DRIVE_TYPE_MSK) )
		{

			case LDC_PULSE_DRIVE:

				/* LoadsControl con profilo: Init Data */
				if (BasicLoader(PTR_UP_LOADSCONTROL, lowStatement, &uPLoadsControl_setting_data) != S_OK )
				{
					/* Non esegue il uP, ma viene abilitato l'avanzamento passo */
					nRet = S_OK;
				}
				else
				{
					/* Dati caricati correttamente: Init */

					uP_LoadsControl_Data.status = LOADSCONTROL_INIT;

					/* seq #1 */
					loadsPulseData1.offset = SEQ1_OFFSET;				
					LDC_pulseSeq_Init(&loadsPulseData1,(SWTC_CountersType)MP_LOADCONTROL_M_COUNTER1);

					if (uPLoadsControl_setting_data.DataLenght > LDC_SEQ1_TABLE_LEN) 
					{				  
						/* seq #2 */
						loadsPulseData2.offset = SEQ2_OFFSET;
						LDC_pulseSeq_Init(&loadsPulseData2,(SWTC_CountersType)MP_LOADCONTROL_M_COUNTER2);
					} 
				}
				break;


			default:

				/*			
				case LDC_NOSTOP_DRIVE:

				Per quanto riguarda LoadsControl in continua, nella prima iterazione
				non vengono pilotati i carichi, non viene data la condizione di uscita
				e soprattutto non vengono inizializzate le variabili relative alla
				modalità di pilotaggio con profilo. In pratica non fa nulla, 
				ma lo fa con molta attenzione.
				(In questo modo si possono usare più LoadsControl in continua nello stesso passo)
				*/

				break;
		}

	}
	else 
	{
		suspend = FALSE;

		if ( uPDoorStatus == UP_DOOR_WAIT )
		{ /* porta aperta o in attesa di timeout dopo la chiusura o delay MR -> il uP va sospeso */   
			suspend=TRUE;          				    
		}

		if(suspend)
		{  	
			enabledloads_mask = 0;
			if( Pump_Pos[LD_DRAIN_PUMP_IND] != NO_LD_POS )
			{
				enabledloads_mask |= (1<<Pump_Pos[LD_DRAIN_PUMP_IND]);
			}

			nRet = S_FALSE;
		}
		else
		{
#ifdef _CORRECT_LOADS_CONTROL_BUG_
			enabledloads_mask=0xFFFF;
#else
			enabledloads_mask=0xFF;
#endif /* !_CORRECT_LOADS_CONTROL_BUG_ */
			nRet = S_OK;
		}

		if ((highStatement & LDC_DRIVE_TYPE_MSK) == LDC_NOSTOP_DRIVE)
		{
		/* Esecuzione LoadsControl in continua */

			if (BasicLoader(PTR_UP_LOADSCONTROL, lowStatement, &lcDirect_SettingData) == S_OK)
			{
				/* Caricata la bitmap dei carichi */

				/* Viene effettuato il pilotaggio */
				LDC_activateLoads((*((uint16_t*) lcDirect_SettingData.Data)) & enabledloads_mask);
			}
			else
			{
				/* Errore nel caricamento */
				/* Non pilota i carichi, ma si abilita comunque l'avanzamento passo */
			}
		}
		else
		{
			/* Esecuzione LoadsControl con profilo */  		

			/* Pilotaggio carichi */ 
			LDC_pulseLoadMgmt(&loadsPulseData1);
			/**/

			/* Gestione transizione stati */
			if(LDC_pulseStepMgmt(&loadsPulseData1,(SWTC_CountersType)MP_LOADCONTROL_M_COUNTER1)) 
			{		  
				uP_LoadsControl_Data.status|=LOADSCONTROL_SEQ1_DONE_MSK;
			}

			/* seq 2 */
			if (uPLoadsControl_setting_data.DataLenght > LDC_SEQ1_TABLE_LEN)
			{/* Presente la sequenza 2 */

				/* Gestione transizione stati */
				if(LDC_pulseStepMgmt(&loadsPulseData2,(SWTC_CountersType)MP_LOADCONTROL_M_COUNTER2)) 
				{			  
					uP_LoadsControl_Data.status|=LOADSCONTROL_SEQ2_DONE_MSK;
				}
				/* Pilotaggio carichi */ 
				LDC_pulseLoadMgmt(&loadsPulseData2);			
			} 
			else 
			{	
				/* seq 2 non presente:
				viene rimosso il blocco all'avanzamento passo che la riguarda
				*/
				uP_LoadsControl_Data.status|=LOADSCONTROL_SEQ2_DONE_MSK;
			}
			/**/

			if (LOADSCONTROL_ALL_SEQ_DONE_MSK == (uP_LoadsControl_Data.status & LOADSCONTROL_ALL_SEQ_DONE_MSK))
			{
				/* Entrambi i profili sono stati eseguiti almeno una volta */
				nRet = S_OK;
			}
		}

	}
	return nRet;
}
/**/

/* ************** */
/* Local Function */
/* ************** */

/**
Gestisce l'inizializzazione delle sequenze temporali da eseguire.
 
\param [in] 	*p : puntatore alla struttura della sequenza da inizializzare
 
\version		1.0.0
\date 			20/07/2006
\author			Roberto Fioravanti

*/
static void LDC_pulseSeq_Init(loadsPulse_Struct *p, SWTC_CountersType indice)
{
uint8_t cntTime;

	/* start seq con ON */
	p->step = LDC_PULSE_EXE_DELAY;

	cntTime = LDC_PULSE_TDELAY(p->offset);

	/*config+init timer*/
	
	(void)SWTC_SetMValue((SWTC_CountersType)indice,cntTime);
}
/**/


/**
Gestisce il passaggio da uno step al successivo nella sequenza temporale di attivazione
 
\param [in] 	*p : puntatore alla struttura della sequenza in questione

\return     bool_t  
\retvalue   FALSE   L'esecuzione del profilo non è terminata
\retvalue   TRUE    L'esecuzione del profilo è terminata, è stato apppena eseguito lo step finale del profilo

\version		1.0.0
\date 			20/07/2006
\author     Roberto Fioravanti

*/
static bool_t LDC_pulseStepMgmt(loadsPulse_Struct *p, SWTC_CountersType indice)
{
bool_t end=FALSE;

	if ( SWTC_IsExpired((SWTC_CountersType)indice) )
	{/* conteggio terminato */

		switch (p->step)
		{/* gestione passaggio di stato */

			case LDC_PULSE_EXE_DELAY:/* OFF1 -> ON */
				/* possibili transizioni:
				  -DELAY -> ON
				  -DELAY -> OFF
			  */
				 
				if ( LDC_PULSE_START(p->offset) == LDC_SEQ_START_ON ) 
				{
				  /* start con ON */
					p->step = LDC_PULSE_EXE_ON;

					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TON(p->offset));				
				}
				else 
				{
				  /* start con OFF */
					p->step = LDC_PULSE_EXE_OFF;

					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TOFF(p->offset));				
				}				
			break;

			case LDC_PULSE_EXE_ON:
                /* possibili transizioni:
                    -ON -> OFF 
                */
			  
                if ( LDC_PULSE_START(p->offset) == LDC_SEQ_START_OFF ) 
                {
                    end=TRUE;					
                } 

                if(LDC_PULSE_TOFF(p->offset))
                {                    
      			    p->step = LDC_PULSE_EXE_OFF;

					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TOFF(p->offset));
                }
                else
                {
					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TON(p->offset));
                }

			break;

			case LDC_PULSE_EXE_OFF:
                /* possibili transizioni:
                    -OFF -> ON 
                */
                if ( LDC_PULSE_START(p->offset) == LDC_SEQ_START_ON )
                {
                    end=TRUE;
                } 
                
                if( LDC_PULSE_TON(p->offset) )
                {                    
          			p->step = LDC_PULSE_EXE_ON;

					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TON(p->offset));
                }
                else
                {
					(void)SWTC_SetMValue((SWTC_CountersType)indice,LDC_PULSE_TOFF(p->offset));
                }
			break;

			default:
				/* stato non possibile: errore */
			break;
		}
	}

	(void)SWTC_Run((SWTC_CountersType)indice);
	return end;
}
/**/

/**
Gestisce il pilotaggio carichi appartenenti alla sequenza in questione

\param [in] 	*p : puntatore alla struttura della sequenza in questione
 
\version		1.0.0
\date 			20/07/2006
\author

*/
static void LDC_pulseLoadMgmt(loadsPulse_Struct const *p)
{
uint16_t loadBitmap;
uint8_t* pBmpOffset;

	if (p->step == LDC_PULSE_EXE_ON)
	{
		pBmpOffset = (uint8_t*)(uPLoadsControl_setting_data.Data + p->offset);

		loadBitmap = MAKEWORD(OFFS_B(pBmpOffset, 0), OFFS_B(pBmpOffset, 1));

		LDC_activateLoads(loadBitmap & enabledloads_mask);
	}
}
/**/


/**
Attiva i carichi appartenenti alla sequenza in questione

\param [in] 	loadBitmap : bitmap dei carichi da attivare
 
\version		1.0.0
\date 			26/02/2008
\author     Roberto Fioravanti

*/
static void LDC_activateLoads(uint16_t loadBitmap)
{
uint8_t i;
#ifdef _CORRECT_LOADS_CONTROL_BUG_
uint16_t msk;
#else
uint8_t msk;
#endif /* !_CORRECT_LOADS_CONTROL_BUG_ */

	for (i = 0, msk=1; i < N_LDC_BITMAP; i++, msk<<=1)
	{
		if (loadBitmap & msk)
		{		  
		  LoadsReq_New[i].b = LOAD_ON;		  		  
		}
	}
}
/**/


/* *********** */
/* End of file */
/* *********** */
