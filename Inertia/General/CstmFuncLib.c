/**
\defgroup	CstmFuncLib Modulo funzioni di libreria
\ingroup	ApplGeneral

Il modulo e' una libreria di macro e funzioni condivise.

Contiene funzioni che non risultano legate a moduli specifici ma che
coprono funzionalita' riutilizzabili trasversalmente.
*/

/**
Source file del modulo Application Platform Library Functions

\file		CstmFuncLib.c
\ingroup	CstmFuncLib
\date		27/07/2010
\version	01.00.00
\author		Emanuela Antognoli

*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "PM_TurbiditySensor.h"
#include "PM_PulseCounterExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* GetModifierData */

/** Estrae la posizione della funzione che funge da modificatore */
#define GMD_FUNCPOS(p)		((uint8_t)(*((uint8_t*)(p)) & 0x7F))

/** Estrae il dato/id tabella relativo al modificatore corrente */
#define GMD_MOD_DATA(p)		OFFS_B(p, 1)

/** Verifica se il dato è semplice o è l'id di una tabella*/
#define GMD_HAVE_TABLE(p)	TESTBIT(*((uint8_t*)(p)), 7)

/**/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
/*static void APP_AlphaCoefficientsComputation(void);*/
#endif
/* ****************** */
/* Exported variables */
/* ****************** */

/* ****************** */
/* Imported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/**
Aggiornamento dei litri di acqua caricati ai fini di effettuare la rigenera.
Chiamata da uP Drain 
\param  [in,out] Reg_WaterLoaded 	 
\param  [in,out] WaterInDW, decilitri di acqua caricati fino ad uno scarico
                 a V.P.+x sec. 	 

\return 	none

\retval		none 	 

\warning	none		

\author 	Stefano Ughi - modifica nomi var Valeria Catani
\date		14/09/2006
\version	1.0.1

\bug
\to do
\note		1.0.0 -> 1.0.1 
			poichè la variabile che contiene i litri accumulati prima di una rigenera
			è del tipo uint8_t non posso conteggiare più di 255 litri 
			viene quindi verificata la condizione di overflow dei litri accumulati
			se tale condizione si presenta la variabile viene posta a 255
*/
void updateRegWaterLoaded( void )
{

uint16_t waterLitresLoadedTemp;

	/*  
	N.B.: in teoria un overflow su Reg_WaterLoaded.dl non dovrebbe essere possibile 
	     visto che WaterInDW non dovrebbe superare il limite 
	     di tracimamento della macchina 110 dl c.a. e
	     Reg_WaterLoaded.dl parte dal valore '0' e viene comunque riportata
	     ad un valore inferiore a 10dl = 1l
	     Se comunque ci dovesse essere un overflow per "sporcamento" variabili
	     ciò non sarebbe preoccupante.
	*/	

	Reg_WaterLoaded.dl += WaterInDW;	
	
	/* 
	check if overflow 
	N.B.: un overflow su Reg_WaterLoaded.l ( litri accumulati dalla macchina prima di effettuare
	      una rigenera è possibile e rappresenterebbe un problema perch� si rischierebbe di
	      non effettuare una rigenera. 
	*/	
	waterLitresLoadedTemp = ( (uint16_t)Reg_WaterLoaded.l + (Reg_WaterLoaded.dl/10) );
	
	if ( waterLitresLoadedTemp < 255 )
	{ 
		Reg_WaterLoaded.l = (uint8_t)waterLitresLoadedTemp;
		Reg_WaterLoaded.dl = Reg_WaterLoaded.dl%10; 
	}
	else
	{
		/* Massimo valore consentito */
		Reg_WaterLoaded.l = 255;
		Reg_WaterLoaded.dl = 0;
	}

}  
/**/

/**
Azzeramento dei litri/dl di acqua caricati ai fini di effettuare la rigenera.
Chiamata dal uP Load

\author 	Valeria Catani
\date		26/06/2008
\version	1.0.1

\bug
\to do
\note	


*/
void clearRegWaterLoaded( void )
{
	Reg_WaterLoaded.l = 0;
	Reg_WaterLoaded.dl = 0;


}
/**/

/**
Reset variabili legate alla turbina

\author 	Valeria Catani
\date		4/06/2008
\version	1.0.1

\bug
\to do
\note	
*/

void WaterLoadedReset(void)
{

  PM_PulseCounter_SetCount(0); /* init pulse counter per nuovo conteggio */

  GIWaterLoaded = 0;	 /* reset variabile acqua caricata */

}



/**
Prepara il buffer per la lettura dello stato da parte della User Interface

\param [in]		TabMachineStateAddr[] (global)
\param [in]		TabMachineStateDim[] (global)

\param [out]	MachineStateBuf[] (global)
	 
\return 		none
\warning		none

\notes:			

\author			Marco Quintini
\date   		20/06/2006
\version    	0.0.1

*/
void UpdateMachineState(void)
{

uint8_t stateItem, stateItemByte, stateBufferIndex;
    
	stateBufferIndex=0;
	
	for( stateItem = 0; stateItem < (uint8_t)(sizeof(TabMachineStateDim)); stateItem ++ )
	{
		for( stateItemByte = 0; stateItemByte < TabMachineStateDim[stateItem]; stateItemByte ++ )
		{
			MachineStateBuf[stateBufferIndex] = *( (uint8_t *)(TabMachineStateAddr[stateItem]) + stateItemByte );
		    
		    stateBufferIndex++;
		}
	}
}
/**/

/**
Aggiorna ApplicationState e ApplicationFlags.

\param [in]		GICriteria (global)
\param [in]		CurrentFault (global)

\param [out]	ApplicationState (global)
\param [out]	ApplicationFlags (global)
	 
\return 		none
\warning		none

\notes:			non va chiamata in PC Control

\author			Marco Quintini
\date   		12/07/2006
\version    	1.0.0

*/
void UpdateStateBitmaps(void)
{
	uint8_t nRegMax;

	/**** Update ApplicationFlags ***/

	/* Vengono messi al valore di init tutti i bit di ApplicationFlags che poi saranno aggiornati */
	ApplicationFlags = BUILD_BMP(ApplicationFlags, APPL_FLAGS_INIT_VAL, APPL_FLAGS_MASK);

  /** Stato sale per UI */
  
	if (HWC_Flags.bit.saltreed)
	{/* reed sale presente */

		if (TESTBIT(GIValid, (GI_FIRST_CRITERION_BIT + GI_SALT_IND) ) != 0)
		{
			nRegMax = OFFS_B(Tab_HWCRegenerateData.Data,2); /* Introdurre delle define, qui e in tutti i punti in cui si usa Tab_HWCRegenerateData F_F */

			if( (TESTBIT(GICriteria, GI_SALT_IND) == 0) && (uP_Regenerate_Data.nReg >= nRegMax ) )
			{
				/* Sale assente */
				SETBIT(ApplicationFlags, AF_NO_SALT_BIT);								
#ifdef _STATISTICS_ON_
				if ( TESTBIT(Statistics_update_flag, STATISTIC_FLAG_NO_SALT)==0)
				{ 
					IncWordWithCheck(&MachineStatistics.globalStats.salt_reset);
					SETBIT(Statistics_update_flag, STATISTIC_FLAG_NO_SALT);
				}	
	
			}else
			{
				CLRBIT(Statistics_update_flag, STATISTIC_FLAG_NO_SALT);
#endif	
			}
			
		}
	}

    /** Stato brillantante per UI */	 /*QUESTO VIENE AGGIORNATO ANCHE NEL UP SPARKLING AID... VA BENE????*/
	/*if( TESTBIT(GICriteria, GI_SPARKLING_AID_IND) == 0 )*/
	if(Flags_2 & FLAGS2_NO_SPARKL_AID_MSK)
	{
		/* Brillantante assente */
		SETBIT(ApplicationFlags, AF_NO_SPARKLING_AID_BIT);
			
#ifdef _STATISTICS_ON_
		if ( TESTBIT (GICriteriaOld, GI_SPARKLING_AID_IND) )
		{/* transizione brillantante presente -> assente*/
			IncWordWithCheck(&MachineStatistics.globalStats.sparkling_reset);
		}	
#endif	
	}


	/** Stato warning H2O per UI */  
	
	if( FltWarning.load )
	{
		/* Problemi caricamento acqua */
		SETBIT(ApplicationFlags, AF_H2O_WARN_BIT);
	}
	

	/** Stato porta per UI */
	if( TESTBIT(GICriteria, GI_CR_DOOR_IND) == 0 )
	{
		/* Porta aperta */
		CLRBIT(ApplicationFlags, AF_DOOR_CLOSED_BIT);
	}
	
	/** Disabilitazione Standby durante lo scarico di sicurezza */
	if ( TESTBIT(Flags_2,FLAG2_SAFETY_DRAIN_PP_ON_POS) ||New_safety_VS.drain_act ||(SftPulseWarn.status==SFT_PULSE_WARN_EXE))
	{
		SETBIT(ApplicationFlags,AF_STANDBY_DISABLE_BIT); 
	}

	if ( TESTBIT(Flags_2, FLAG2_CLOGGED_FILTER))
	{
		SETBIT(ApplicationFlags,AF_DIRTY_FILTER_BIT); 
	}
	
	/* Aggiungere l'aggiornamento di eventuali altri bit */


	/**** Update ApplicationState ***/

	/* Vengono messi al valore di init tutti i bit di ApplicationState che poi saranno aggiornati */
	ApplicationState = BUILD_BMP(ApplicationState, APPL_STATE_INIT_VAL, APPL_STATE_MASK);

	/** Stato PC Control */
	/* Qui il bit AS_REMOTE_CTRL_ON_BIT viene solo resettato, il set avviene altrove */

	/** Stato di fault per UI */
	if( CurrentFault[0].code != CODE_NO_FAULT )
	{
		/* Main in stato di FAULT */
		SETBIT(ApplicationState, AS_APPL_FAULT_BIT);
	}

	/** Validità dati per UI */
	if( FLT_checkForSettingFault() == TRUE )
	{
		/* Richiesta UI: se fault file di setting => azzerare il bit DATA_READY */
		CLRBIT(ApplicationState, AS_DATA_READY_BIT);
	}
	
#if	(SBY_STANDBY_ENABLE == 1)
	if (HarakiriLowPower.Mode != HKM_SBY_OFF_MODE)
	{	/* aggiornamento bit relativo allo stato di Standby ON :
		si mette nei criteri perchè nei flag di applicazione non c'è posto */

		if (HarakiriLowPower.Req ==  HKM_LOW_POWER_REQ   )
		{
			SETBIT(ApplicationFlags, AF_SBY_SLEEP_ON_FLAG );	
		}
	}	
	
	if ( TESTBIT(GIValid,GI_FIRST_WAKEUP_SOURCE_IND) && (TESTBIT(GICriteria, GI_FIRST_WAKEUP_SOURCE_IND)==0))
	{
		SETBIT(ApplicationFlags, AF_WAKEUP_OFBUTTON_BIT );
	}else
	{
		if (TESTBIT(GICriteria, GI_FIRST_WAKEUP_SOURCE_IND))
		{
			if  ( TESTBIT(Flags_2, FLAG2_SAFETY_TURB_TEST_BIT) || ( TESTBIT (GICriteria,GI_CR_OVFL_IND) != 0 )|| (New_safety_VS.drain_act)
			||(SftPulseWarn.status==SFT_PULSE_WARN_EXE)  )
			{
				SETBIT(ApplicationFlags, AF_STANDBY_DISABLE_BIT );	
			}	
		}else
		 {
		 /* ingresso circuito di standby non presente o comunque non valido
			i  flag sono sempre a 0 . la main non va in standby autonomamente, quindi la visual non dovrebbe rimandare il pacchetto.
			per cui si simula un reset da tasto*/
			SETBIT(ApplicationFlags, AF_WAKEUP_OFBUTTON_BIT );
		 }	
	}
#endif
}
/**/

/**
Dato un valore e la variazione percentuale (con segno) richiesta, restituisce il delta
da applicare al valore in ingresso (es. target di conteggio).
 
\param [in] variation: variazione percentuale (con segno) richiesta
\param [in] value: target a cui applicare 

\return il delta (con segno) da applicare al valore in ingresso

\version		1.0.0
\date 			16/11/2006
\author  		Valeria Catani
*/

int16_t GetDeltaFromVariation(uint16_t value, int8_t variation)
{
ldiv_t deltadiv;
int32_t delta;

	delta = ((int32_t)value * abs((int)variation));

	deltadiv = ldiv(delta, (int32_t)100);

	delta = deltadiv.quot;

	if ( deltadiv.rem > 50 )
	{
		delta++;
	}
	
	if (variation > 0)
	{
		/* value è sempre > 0, il segno di delta dipende da variation */
		return ((int16_t) delta);
	}
	
	return ((int16_t)(-delta));
}
/**/


/**
Dato il puntatore ad una tabella a word in cui il byte alto (1) è un indice
ed il byte basso (2) un valore, preso il valore regVal restituisce 
il valore (2) corrispondente all'indice (1) minore o uguale a regVal.
	
\param [in] pTable puntatore alla tabella
\param [in] regVal valore di confronto dell'indice

\return	il valore della tabella corrispondente all'indice fornito
*/
uint8_t TabQueryB(const Loader_Struct* pTable, uint8_t regVal)
{
uint16_t *pRegIndex;
uint8_t tabValue;

	pRegIndex = (uint16_t*)(pTable->Data);
	tabValue = LOBYTE(*pRegIndex++);
	
	while ((pRegIndex < ((uint16_t*)(pTable->Data) + pTable->DataLenght)) && (HIBYTE(*pRegIndex) <= regVal))
	{

		tabValue = LOBYTE(*pRegIndex++);
	}
	
	return tabValue;
}
/**/



/**
Ritorna la variazione assciata ad un modificatore

\param [in] pModTable è il puntatore ai dati relativi al modificatore

\return la variazione associata al modificatore
 
\version		1.0.0
\date 			14/02/2006
\author  		Simone Costantini
*/


int8_t GetModifierData(const uint8_t* pModTable)
{
Loader_Struct regTable;
uint16_t regVal;
uint8_t indReg;
uint8_t modData;
uint8_t funcPos = GMD_FUNCPOS(pModTable);
int8_t variation = MOD_DONT_CARE;

	/* L'indice di regolazione ha un offset di 1 */
	indReg = BufNewReg[funcPos] - 1;
	
	if(0xFF == indReg)
	{
		/*
		(BufNewReg[funcPos] == 0) è una situazione anomala che non dovrebbe	mai	verificarsi. 
		Tuttavia, nella remota possibilità che ciò accada e considerato che questa 
		funzione deve ritornare sempre un valore, scegliamo MOD_DONT_CARE 
		(ovvero non vengono applicati i modificatori).
		*/
		variation = MOD_DONT_CARE;
	}
	else
	{
		/* 
		E' stato letto correttamente il valore di regolazione della
		funzione specificata.
		*/
		modData = GMD_MOD_DATA(pModTable);

		if (GMD_HAVE_TABLE(pModTable))
		{
			/* La funzione ha più regolazioni */

			if (BasicLoader(PTR_MICROPROG_MODIFIERS_DATA, modData, &regTable) == S_OK)			
			{							
				/* Si lavora su INDICE della Regolazione */
				/* 
				Viene presa nella tabella la variazione corrispondente al
				più grande valore di regolazione <= regVal.
				*/
				variation = (int8_t) TabQueryB(&regTable, indReg);
				/*
				Variante: tabella ad accesso diretto
				if (indReg < 2*regTable.DataLenght)
				{
					variation = (int8_t) *((uint8_t*)regTable.Data+ indReg);
				}
				*/
			}

		}
		else
		{
			/* 
			La funzione è di tipo ON/OFF, 
			se è il valore è 0 (OFF) non viene applicata alcuna variazione
			*/
			
			if (REG_getFunctionValueByPos(funcPos, &regVal) == S_OK)
			{
				if (regVal != 0)
				{		
					variation = (int8_t) modData;
				}
			}
		}
	}	
	
	return variation;
}


/**
Ricerca il valore ingresso nella tabella restituisce il corrispondente valore di uscita.

E' applicabile a Generic Inputs 
(ANALOG TO DEGREES, FREQ TO WATER LEVEL (PRESSURE SENSOR - LINEAR)
ANALOG TO CRITERION, FREQ TO CRITERION, ANALOG TO LEVELS) e modificatori

Formato dati supportato: Input 
                            8/16 bit unsigned
                         Output.
                            8/16 bit signed/unsigned
                        
Operazione sui dati di uscita:interpolazione oppure no

 
\param [in]		*_in : puntatore alla struttura TabQueryIn_t                                            
\param [out]	*_result : puntatore al risultato della conversione (TabQueryResult_t)
\return 			uint8_t
\retvalue 		S_OK se la conversione è andata a buon fine
\retvalue       S_FALSE se il numer di punti è nullo.o se la ricerca fallisce.

				
\version		1.0.1
\date 			30/06/2008
\author			Roberto Fioravanti

\note		\par \b Nota 1:           
\to do       -
            
*/
uint8_t TabQuery(const TabQueryIn_t * _in, TabQueryResult_t * _result)
{        
    uint8_t *pInputData;                    /*pnt ai dati in ingresso*/
    uint8_t *pOutputData;	                /*pnt ai dati in uscita*/
    uint8_t *pCurrInputData;                /*pnt al valore range corrente  */
    uint8_t range_index = 0;                /*indice del range della lettura*/
    uint16_t currInputVal,prevInputVal;     /*valori ingresso range corrente e precendente*/
    uint16_t currOutputVal,prevOutputVal;	/*valori uscita range corernte e precedente*/
    uint16_t psLowerBound, psUpperBound;    /*limiti*/
    bool_t bFound=FALSE;
    
	if(_in->tab->pointNumber == 0)
	{
		/* Numero di elementi della tabella nullo: non è possibile valorizzare *returnValue */
		return S_FALSE;
		
	}
	
	pInputData = ( (uint8_t*)_in->tab + FIXED_GI_TAB_LEN ); 
    /* puntatore al campo dati dei valori di output (allineato a word) */ 
	pOutputData = ( (uint8_t*)_in->tab + FIXED_GI_TAB_LEN + (_in->tab->pointNumber<<(_in->tab->S1&GI_WORD)) ); 
	if(((_in->tab->S1 & GI_WORD) == GI_BYTE) && (_in->tab->pointNumber&1))
	{
	    pOutputData++;
	}
	
	pCurrInputData=pInputData;
	
    /* Limite inferiore e superiore */
    switch(_in->tab->S1)
    {
        case 0:
            /* uc */
            psLowerBound = (uint16_t)(*pInputData);
	        psUpperBound = (uint16_t)(*(pInputData+_in->tab->pointNumber-1));
	    break;
        
        case 1:
            /* us */
            psLowerBound = *((uint16_t*)pInputData);
	        psUpperBound = *((uint16_t*)pInputData+_in->tab->pointNumber-1);
	       
        break;
    }
	
	 _result->b.lbound = (_in->cfr_val<psLowerBound)?TRUE:FALSE;
	 _result->b.ubound = (_in->cfr_val>psUpperBound)?TRUE:FALSE;
	        
	 
	/* ricerca */
    while( (!bFound) && (range_index < _in->tab->pointNumber))  
    {	  
 	     	    
        if(_in->tab->S1&GI_WORD) 
        {
          /* formato input a WORD */
          prevInputVal = *((uint16_t*)pCurrInputData);     
          pCurrInputData+=sizeof(uint16_t);
         currInputVal = *((uint16_t*)pCurrInputData);     	
            
        }
        else 
        { 
          /* formato input a BYTE */      
          prevInputVal = (uint16_t)*(pCurrInputData);   
          pCurrInputData++;
          currInputVal = (uint16_t)*(pCurrInputData);      
        	
        }

        /* verisfica della condizione di ricerca */
        if(_in->opt.b.findexactvalue == 0)
        {            
            if(_in->cfr_val <= currInputVal)
            {
            	bFound=TRUE;
            }
        }
        else
        {
            if(_in->cfr_val == prevInputVal)
            {
            	bFound=TRUE;
            }
        }
        
        range_index++;
        
    }
    
    if(bFound)
    {
        if(!_in->tab->Int)
        {            
            if(_in->tab->S2&GI_WORD)
            {
                
                _result->data=*((uint16_t*)pOutputData + range_index - 1);
            }
            else
            {
                _result->data=(uint16_t)*(pOutputData + range_index - 1);
            }
        }
        else
        {
            if(_in->tab->S2&GI_WORD)
            {/* Output dati a WORD con interpolazione */
            	   		   
            	currOutputVal = (uint16_t)(*((uint16_t*)pOutputData + range_index));
            	prevOutputVal = (uint16_t)(*((uint16_t*)pOutputData + range_index - 1));
            }
            else
            {/* Output dati a BYTE con interpolazione */
            			  
            	currOutputVal = (uint8_t)(*((uint8_t*)pOutputData + range_index));			  
            	prevOutputVal = (uint8_t)(*((uint8_t*)pOutputData + range_index-1));				
            }	

            _result->data = (uint16_t)(((((int32_t)_in->cfr_val - (int32_t)prevInputVal) * (int32_t)(currOutputVal - prevOutputVal))/(int32_t)(currInputVal - prevInputVal)) + (int32_t)(prevOutputVal));
            
            if((_in->tab->S2&GI_WORD)==0)
            {			  
                _result->data=(uint8_t)_result->data;
            }
        }
        return S_OK;
    }
    
    if(_result->b.lbound)
    {        
        if(_in->tab->S2&GI_WORD)
        {
            
            _result->oBoundVal=*((uint16_t*)pOutputData);
        }
        else
        {
            _result->oBoundVal=(uint16_t)*(pOutputData);
        }
    }
    
    if(_result->b.ubound)
    {        
        if(_in->tab->S2&GI_WORD)
        {
            
            _result->oBoundVal=*((uint16_t*)pOutputData+_in->tab->pointNumber-1);
        }
        else
        {
            _result->oBoundVal=(uint16_t)*(pOutputData+_in->tab->pointNumber-1);
        }
    }
	
	return S_FALSE;
	
}

/**
Data una lista di modificatori restituisce variazione percentuale associata con segno.
 
\param [in] tableID indice della tabella dei modificatori
\param [out] pModifierValue  se S_OK restituisce il modificatore

\return uint8_t 
\retvalue   MOD_DONT_CARE modificatori non individuato o non attivo
			S_FALSE Indice tabella non valido (tabella variabili, tabella modificatori 1� lev)
			S_OK    modificatori attivo
 
\version		1.0.0
\date 			01/07/2008
\author  		Roberto Fioravanti
*/
uint8_t GetModifierValue(uint8_t tabIndex, int8_t *pModifierValue)
{
Loader_Struct ldVarTable, ldData;
ModifierL1Table_t   *pL1Table;
int16_t modifierVarValue=0, funcVal;
uint8_t dataType, dataIndex;

bool_t bModFound=FALSE,valid=TRUE;
uint8_t sRet =S_FALSE;
uint16_t *pEndL1Table;


    if(tabIndex==0xFF)
    {
        
	    *pModifierValue=0;
	    return S_FALSE;
    }
    
    /* Caricamento puntatore alla tabella delle variabili che agiscono da modificatori */
	if( BasicLoader(PTR_HW_CONFIG_DATA, DSPL_HWC_MODIFIERS_VAR_TABLE, &ldVarTable) != S_OK)
	{
		return S_FALSE;
	}

	
	if( BasicLoader(PTR_MICROPROG_MODIFIERS_DATA, tabIndex, &ldData) != S_OK)
	{
		
		return S_FALSE;
	}
	

    /* Puntatore ai parametri della tabella di livello 1 */
	/*lint -e826 */
	pL1Table = (ModifierL1Table_t *)ldData.Data;
    /*lint +e826 */


	pEndL1Table = (uint16_t*)ldData.Data + ldData.DataLenght;

    while ( (!bModFound) && ((uint16_t*)pL1Table < pEndL1Table) )
    {
        /* scorre la lista dei modificatori */
        
        	dataType=((Condition_SelDataStruct *)(ldVarTable.Data) + pL1Table->varPos)->dataType;
        	dataIndex=((Condition_SelDataStruct *)(ldVarTable.Data) + pL1Table->varPos)->dataIndex;
                	
        	
            if(pL1Table->dataFormat == MOD_LEV1_DT_VALUE)
            {
                /* il campo data è il dato */
                switch(dataType)
            	{
            	    case JIF_TAB_FUNCTIONS_SEG:
            	    
            	        /* estrae il dato della funzione */
                	    if (REG_getFunctionValueByPos(dataIndex, (uint16_t*)&funcVal) == S_OK)
            			{
            				if (funcVal != 0)
            				{		            					
            					*pModifierValue=(int8_t)pL1Table->data;
            					bModFound=TRUE;
            				}
            			}
            	        break;
            	        
            	    default:
            	        *pModifierValue=0;
            	}
                
                
            }
            else
            {
                
                TabQueryIn_t        qin;                
                TabQueryResult_t    qresult;
                
                                              
            	/* Caricamento della tabella di livello 2 dei modificatori */
            	if(BasicLoader(PTR_MICROPROG_MODIFIERS_DATA, pL1Table->data, &ldData) != S_OK)
            	{

            		return S_FALSE;
            	}
            	
            	switch(dataType)
            	{
            	    case JIF_TAB_GI_TURBIDITY_SEG: 
            	     
            	        if( HWC_Flags.bit.turbsens )
            	        {
            	            
            	            modifierVarValue=GITurbidity;
            	            qin.opt.b.findexactvalue=1;
            	            
            	            valid=TRUE;
            	            
            	        }
            	        else
            	        {
            	            valid=FALSE;
            	        }
            	        break;
            	    case JIF_TAB_FUNCTIONS_SEG:
            	        modifierVarValue=BufNewReg[dataIndex] - 1;
            	        qin.opt.b.findexactvalue=1;
            	        valid=TRUE;
            	        break;
					
					default:
						break;
            	}
            	
            	/**/
            	
            	if(valid)
            	{            	               	
                	qin.cfr_val=modifierVarValue;
                    
                    qin.tab=(TabHeader_t *)ldData.Data;
                    
                	if(S_OK == TabQuery(&qin, &qresult))
                	{
                	    *pModifierValue=qresult.data;
                	    bModFound=TRUE;
                	}
            	}
            	
            	
            }
    
        
        pL1Table++;
    }
	
	
	if(bModFound)
	{	    
	    sRet=S_OK;
	}
	else
	{
	    sRet=MOD_DONT_CARE;
	    *pModifierValue=0;
	}
	
    return sRet;
}
/**/

/**
    Comando di calibrazione per il sensore di torbidità
    se non è in corso una operazione analoga.
    
    Forza lo stato del gestore della calibrazione a TURBSENS_CALIBR_REQUEST_STATE
    

\version		1.0.0
\date 			05/05/2008
\author  		Roberto Fioravanti
*/
void TurbidityCalibrationCmd(void)
{
    if((TurbiditySensorCal.state != TURBSENS_CALIBR_REQUEST_STATE) &&
    (TurbiditySensorCal.state != TURBSENS_WAIT_4_CALIBR_VAL_STATE))
    {
        TurbiditySensorCal.state = TURBSENS_CALIBR_REQUEST_STATE;
    }
}
/**/



/**
    Gestione del valore pwm per la calibrazione del sensore di torbidità
    Tramite una password, è possibile utilizzare un valore custom di pwm 
    per la lettura del sensore.
    

\version		1.0.0
\date 			05/05/2008
\author  		Roberto Fioravanti
*/
void TurbidityPwmValManager(void)
{
	switch(TurbiditySensor.state)
	{
		case TURBIDITY_PWM_DEFAULT_STATE:	
    		if(TurbiditySensor.pwd == TURBIDITY_CONTINUOUS_READ_PWD)
    		{
    			TurbiditySensor.state=TURBIDITY_CONTINUOUS_READ_INIT_CUSTOM_STATE;
    		}
    		else if(TurbiditySensor.pwd == USE_TURBIDITY_PWM_CUSTOM_VAL_PWD)
    		{
    			TurbiditySensor.state=TURBIDITY_PWM_INIT_CUSTOM_STATE;
    		}
    		else
    		{
    		    if(TurbiditySensor.pwmval!=TurbiditySensorCal.val) 
    		    {
    		    	/*valore pwm in tick è cambiato
					pwm (per mille) update ( da tick a per mille), lo fa già il modulo.*/
    		        TurbiditySensor.pwm = (uint32_t)TurbiditySensorCal.val;
    		        /*pwm tick update*/
    	            TurbiditySensor.pwmval=TurbiditySensorCal.val;
    		    }
    	        HIO_trbReadDisable(TURBSENS_READ_cstm_PWM);
    	        HIO_trbReadDisable(TURBSENS_READ_cstm_CONTINUOUS);
    	    }
		
		break;		
		
		case TURBIDITY_PWM_CUSTOM_STATE:
			if( (TurbiditySensor.pwd != USE_TURBIDITY_PWM_CUSTOM_VAL_PWD) )
			{
				TurbiditySensor.state=TURBIDITY_PWM_DEFAULT_STATE;
			}
			else if(TurbiditySensor.pwm!=TurbiditySensor.cstm_pwmval)
			{
			    /* valore pwm in per mille è cambiato*/
				/*TurbiditySensor.pwmval=TurbiditySensor.cstm_pwmval; original*/
				
				 /*pwm (per mille) update*/
    		    TurbiditySensor.pwm=TurbiditySensor.cstm_pwmval;
		         /*pwm tick update  (da per mille a tick):le conversioni le fa il modulo;*/
    		    TurbiditySensor.pwmval=(uint32_t)(TurbiditySensor.cstm_pwmval);
    	            
				TurbiditySensorCal.flags.bit.cmdAcquireDone=0;
			}
		break;
		
		case TURBIDITY_PWM_INIT_CUSTOM_STATE:
			
        	TurbiditySensor.state=TURBIDITY_PWM_CUSTOM_STATE;
        	HIO_trbReadEnable(TURBSENS_READ_cstm_PWM);
		break;


		
	    case TURBIDITY_CONTINUOUS_READ_INIT_CUSTOM_STATE:
	        TurbiditySensor.state=TURBIDITY_CONTINUOUS_READ_CUSTOM_STATE;
        	HIO_trbReadEnable(TURBSENS_READ_cstm_CONTINUOUS);
	    break;
	        
	     
	    case TURBIDITY_CONTINUOUS_READ_CUSTOM_STATE:
	        if( (TurbiditySensor.pwd != TURBIDITY_CONTINUOUS_READ_PWD) )
			{
				TurbiditySensor.state=TURBIDITY_PWM_DEFAULT_STATE;
			}
			
			if(TurbiditySensor.pwmval!=TurbiditySensorCal.val) 
			{
			     /*pwm di calibrazione (tick) cambiato, quindi forzare un nuovo comando di acquisizione*/
		         /*pwm (per mille) update ( da tick a per mille), lo fa il modulo la conversione*/
		        TurbiditySensor.pwm=(uint32_t)TurbiditySensorCal.val;
		         /*pwm tick update*/
			    TurbiditySensor.pwmval=TurbiditySensorCal.val;
    	            			    
			    TurbiditySensorCal.flags.bit.cmdAcquireDone=0;
			}
	    
	    break;
	}
}
/**/

/**
    Gestione della calibrazione del sensore di torbidità (anche in PROGRAMMING_MODE).
    
    Implementa la macchina a stati della calibrazione del sensore di torbidità.
    Lo stato iniziale (dopo il reset) è TURBSENS_INVALID_CALIBR_VALUE_STATE (valore di calibrazione non valido).
    
    Dopo una calibrazione terminata con successo o in caso di valore valido lo stato si porta in 
    TURBSENS_VALID_CALIBR_VALUE_STATE.
    
    E' possibile forzare una calibrazione chiamando TurbidityCalibrationCmd.
    
    

\version		1.0.0
\date 			05/05/2008
\author  		Roberto Fioravanti
*/

uint16_t turbidityCal;

void TurbidityCalibrationManager(void)
{
	
	switch(TurbiditySensorCal.state)
	{
/*
    case TURBSENS_INVALID_CALIBR_VALUE_STATE:
      // se il valore di calibrazione non � valido rimane in questo stato 
    if(TurbiditySensorCal.val)
    {
        TurbiditySensorCal.state=TURBSENS_VALID_CALIBR_VALUE_STATE;
    }
      
      break;
*/
    	case TURBSENS_VALID_CALIBR_VALUE_STATE:
    		/*idle state*/
    		break;
            
    	case TURBSENS_CALIBR_REQUEST_STATE:
    	{
    		uint16_t turbidityCmd;

    		turbidityCmd = TURBIDITY_CAL_ADVAL;
    		PM_TurbiditySensor_StartCalib(turbidityCmd);

        
    		TurbiditySensorCal.state=TURBSENS_WAIT_4_CALIBR_VAL_STATE;
    	}
    		break;
      
    	case TURBSENS_WAIT_4_CALIBR_VAL_STATE:
    	{

    		/*legge il risultato*/
    		/*turbidityCal=TurbidityGetCalibration();*/
            
    		if(PM_TurbiditySensor_GetCalibration(&turbidityCal)  != 0)
    		{
          
    			/*TurbiditySensorCal.val=turbidityCal.pulseWidthCal;*/
    			/*INSERIRE TEST IN CASO DI SATURAZIONE (VALORE NON VALIDO!!!!)*/
          
    			TurbiditySensorCal.flags.bit.cmdAcquireDone=0;
    			TurbiditySensorCal.state=TURBSENS_CALIBR_VALIDATE_STATE;
    			TurbiditySensorCal.pwd=RESET_TURBIDITY_CALIBRATION_PWD;
    			TurbiditySensorCal.idRead= TurbidityReads.id;
    			HIO_trbReadEnable(TURBSENS_READ_calib_FLB);
		  		  
    			TurbiditySensorCal.adH2oRef=0;
    		}

        
    	}
    		break;
      
    	case TURBSENS_CALIBR_VALIDATE_STATE:
    
    		if((GIValid&GI_TURB_SENSOR_MSK) && (TurbiditySensorCal.idRead!= TurbidityReads.id))
    		{
    			if((TurbidityReads.ADvalue<TURBIDITY_H2O_MIN_ADVAL))
    			{
    				TurbiditySensorCal.val=TURBIDITY_PWM_MIN;
    				TurbiditySensorCal.adH2oRef=TURBIDITY_H2O_DEFAULT_ADVAL;
    			}
    			else if((TurbidityReads.ADvalue>TURBIDITY_H2O_MAX_ADVAL))
    			{
    				TurbiditySensorCal.val=TURBIDITY_PWM_MIN;
    				TurbiditySensorCal.adH2oRef=TURBIDITY_H2O_DEFAULT_ADVAL;
    			}
    			else
    			{
    				/* lettura in range*/
    				TurbiditySensorCal.adH2oRef=TurbidityReads.ADvalue;
    				/* la fase di validazione in aria può cambiare questo valore in uno di default*/
    			}

    			/*delta is 1% of adH2oRef*/
    			TurbiditySensorCal.adH2oRefDelta=TurbiditySensorCal.adH2oRef/100;
    			TurbiditySensorCal.state=TURBSENS_VALID_CALIBR_VALUE_STATE;
    			HIO_trbReadDisable(TURBSENS_READ_calib_FLB);

    		}
    			break;
	}
}/*end TurbidityCalibrationManager*/


/**
Disabilita i contatori utilizzati nei uPs : chiamata in PAUSE e al RESET FREDDO (Backup Data Analisys)
 
\return 		

\version		1.0.0
\date 			28/04/2008
\author  		Valeria Catani	
*/

void StopMicroprogCounters(void)
{
	/* stop uP counters */
  (void)SWTC_Stop((SWTC_CountersType)MP_MAINTAIN_M_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_ALTCTRL_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_DRAIN_M_COUNTER);	
  (void)SWTC_Stop((SWTC_CountersType)MP_LOADCONTROL_M_COUNTER1);
  (void)SWTC_Stop((SWTC_CountersType)MP_LOADCONTROL_M_COUNTER2);

  (void)SWTC_Stop((SWTC_CountersType)MP_LOADRESTORE_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_TIMELOAD_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_SETDRYPROFILE_M_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_REGENERATE_EVDELAY_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_SPARKLINGAID_M_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_LEVTOTIME_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)SFT_RES_PRES_STABLE_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)PULSE_CHECK_DELAY_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)PULSE_STILL_M_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_LOAD_FULLSTABILITY_S_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)FLT_HEATTOUT_M_COUNTER);
  (void)SWTC_Stop((SWTC_CountersType)MP_LOADTIMEOUT_REST_M_COUNTER);
  
  /* &Delay: Delay deve proseguire anche in pausa */
  /* &OverflowFault_timer: il conteggio per fault overflow prosegue anche in pausa*/
  /* &ProbeFault_timer : il conteggio per fault probe prosegue anche in pausa */
  
  
  /*&DoorClosedEvent_Pause_timer: il conteggio prosegue anche in pausa */
 
  
}

/**/


/**
Incrementa il parametro se questo risulta inferiore a 0xFFFF, altrimenti lo lascia invariato.

\param [in] 	statisticWord è il parametro da incrementare
	 
\return     	none

\notes:			

\author			Marco Quintini
\date   		30/11/2006
\version    	1.0.0

*/
void IncWordWithCheck(uint16_t *statisticWord)
{
	if((*statisticWord) < 0xFFFF)
	{
		(*statisticWord) ++;
	}
}
/**/

/**
restituisce TRUE se il ciclo se esiste almeno una cavità che è in run o in delay
 
\return 		

\version		1.0.0
\date 			02/02/2010
\author  		Emanuela Antognoli
*/


uint8_t IsOvenRunningDelay (void)
{
	if((Interpreter_Data[0].completeLabel.s.label == LBL_RUN)||(Interpreter_Data[0].completeLabel.s.label == LBL_DELAY) )
	{
		
		return TRUE;
	}
	
	return FALSE;
}

/**
restituisce TRUE se il ciclo se esiste almeno una cavità che è in PAuSE o FAULT
 
\return 		

\version		1.0.0
\date 			02/02/2010
\author  		Emanuela Antognoli
*/


uint8_t IsOvenPauseFAult (void)
{
	if((Interpreter_Data[0].completeLabel.s.label == LBL_PAUSE)||(Interpreter_Data[0].completeLabel.s.label == LBL_FAULT) )
	{
		
		return TRUE;
	}
	
	return FALSE;

}


/**
Aggiornamento del buffer del modulo Power Meter SW con gli ultimi dati ricevuti dal DSP.
Questa funzione e' stata pensata per essere chiamata all'interno del ciclo del task applicativo.

\return     S_OK nel caso di aggiornamento positivo, S_FALSE nel caso opposto.

\author     Pietro Loschiavo
\date     26/06/2013
\version    1.0.0
*/
uint8_t UpdateInfoBufferToPowerMeterModule(void)
{
  sEMS_ExtInfo_t* pEMSBuffer;
  uint8_t i, retVal = S_FALSE;

  for(i=0; i < CONFIG_EMS_MLOAD_ITEMS_MAX; i++)
  {
	  pEMSBuffer = EMS_GetPtrToDSPInfoBuffer(i);
	  if ( pEMSBuffer != NULL )
	  {

	    pEMSBuffer->ui16PVal = (uint16_t) (( (uint16_t) ExtMotor_DataBuffer[0] << 8)
	        | ExtMotor_DataBuffer[1]) ;
	    pEMSBuffer->ui32EVal = (uint32_t) ((uint32_t) ( ExtMotor_DataBuffer[2] << 24)
	        | (uint32_t) ( ExtMotor_DataBuffer[3] << 16 )
	        | (uint32_t) ( ExtMotor_DataBuffer[4] << 8 )
	        | (uint32_t) ExtMotor_DataBuffer[5]);

	    retVal =  S_OK;
	  }
  }

  return retVal;

}


/**
 *

\param [in] tableID indice della tabella dei modificatori
\param [out] pModifierValue  se S_OK restituisce il modificatore

\return uint8_t
\retvalue   MOD_DONT_CARE modificatori non individuato o non attivo
			S_FALSE Indice tabella non valido (tabella variabili, tabella modificatori 1� lev)
			S_OK    modificatori attivo

\version		1.0.0
\date 			21/03/2014
\author  		Andrea Emili
*/
uint8_t GetModifierValueAPP2_0(uint8_t tabIndex, int16_t *pModifierValue)
{

 Loader_Struct ldVarTable;
 uint8_t dataType, dataIndex;
 uint8_t sRet =S_FALSE;


    if(tabIndex==0xFF)
    {

	    *pModifierValue=0;
	    return S_FALSE;
    }

    /* Caricamento puntatore alla tabella delle variabili che agiscono da modificatori */
    if( BasicLoader(PTR_HW_CONFIG_DATA, DSPL_HWC_POWER_PROFILE_VARIABLE_TABLE, &ldVarTable) != S_OK)
    {
    	return S_FALSE;
    }


    dataType=((Condition_SelDataStruct *)(ldVarTable.Data) + tabIndex)->dataType;
    dataIndex=((Condition_SelDataStruct *)(ldVarTable.Data) + tabIndex)->dataIndex;

    switch(dataType)
    {
    	case JIF_TAB_GI_TURBIDITY_SEG:
	        if( HWC_Flags.bit.turbsens )
	        {
	        	*pModifierValue = GITurbidity;
	        	sRet = TRUE;
	        }
	        else
	        {
	        	sRet = FALSE;
	        }
	        break;
	    case JIF_TAB_FUNCTIONS_SEG:
	    	*pModifierValue = BufNewReg[dataIndex] - 1;
	    	sRet = TRUE;
	        break;
	    default:
	    	*pModifierValue = 0;
	    	break;
    }

    return sRet;
}
/**/

/**
 *
\function GetAlphaModifierValueAPP2_0
\param [in] tabIndex indice della tabella dei modificatori
\param [in] pModifierValue se S_OK restituisce il modificatore

\return uint8_t
\retvalue   MOD_DONT_CARE modificatori non individuato o non attivo
			S_FALSE Indice tabella non valido (tabella variabili, tabella modificatori 1� lev)
			S_OK    modificatori attivo

\version		1.0.0
\date 			26/03/2014
\author  		Andrea Emili
*/

uint8_t GetAlphaModifierValueAPP2_0(uint8_t tabIndex, LIB_Fixed_ValueType *pModifierValue)
{

 Condition_Struct *pCurrentCondition;
 Loader_Struct ldVarTable;

	pModifierValue->fractional = 0;
	pModifierValue->notation = 0;

	/*APP_AlphaCoefficientsComputation();*/		/*AE TODO Evaluate if the position is here*/

	if( BasicLoader(PTR_HW_CONFIG_DATA, DSPL_HWC_POWER_PROFILE_VARIABLE_TABLE, &ldVarTable) != S_OK)
	{
		return S_FALSE;
	}


	pCurrentCondition->dataSel.dataType = ((Condition_SelDataStruct *)(ldVarTable.Data) +tabIndex)->dataType ;
	pCurrentCondition->dataSel.dataIndex = ((Condition_SelDataStruct *)(ldVarTable.Data) +tabIndex)->dataIndex ;


	if( SelectOperandsAPP2_0(pCurrentCondition, pModifierValue) != S_OK )
	{
		/*return ERR_CODE;*/
		pModifierValue->fractional = 1;
		pModifierValue->notation = 0;
	}

	return S_OK;
}
/**/
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
void APP_GetReferenceParameters()
{
	Loader_Struct ldVarTable;
	if( BasicLoader(PTR_OTHER_ALGORITHMS_TABLE_DATA, DSPL_ALPHA0_VOLTAGE_PARAMETERS, &ldVarTable) != S_OK)
		{
			/*return 1;*/
		}


		Vref.fractional =  OFFS_W(ldVarTable.Data, 0);
		Vref.notation = OFFS_B(ldVarTable.Data, 2);
		/*voltage_sample_to_mean = OFFS_B(ldVarTable.Data, 3);*/

		if( BasicLoader(PTR_OTHER_ALGORITHMS_TABLE_DATA, DSPL_ALPHA1_TEMPERA_PARAMETERS, &ldVarTable) != S_OK)
			{
				/*return 1;*/
			}

			Tref.fractional = OFFS_W(ldVarTable.Data, 0);
			Tref.notation = OFFS_B(ldVarTable.Data, 2);
			/*temperature_sample_to_mean = OFFS_B(ldVarTable.Data, 3);*/
}


/*
 * Function: APP_AlphaCoefficientsComputation
 * Description: Computation of Inertia's Alpha coefficients for APP2.0 Module
 * Author: Andrea Emili, SPES scpa
 * Note:
 * 	- Use the data type FixedData_tag
 * 	- Load data from 0x2D pointer
 */
LIB_Fixed_ValueType Tupd;
LIB_Fixed_ValueType Vupd;
void APP_AlphaCoefficientsComputation(void)
{
	LIB_Fixed_ValueType app_var, result_var,result_var_1, result_vefd;
	Loader_Struct ldVarTable;
	/*uint16_t voltage_sample_to_mean = 0;
	uint16_t temperature_sample_to_mean = 0;*/


	 /*Init*/
	 app_alpha_voltage_on_power.fractional = 1;
	 app_alpha_voltage_on_power.notation = 0;
	 app_alpha_voltage_on_durationON.fractional = 1;
	 app_alpha_voltage_on_durationON.notation = 0;
	 app_alpha_voltage_temperature_on_durationON.fractional = 1;
	 app_alpha_voltage_temperature_on_durationON.notation = 0;
	 app_alpha_temperature_on_durationOFF.fractional = 1;
	 app_alpha_temperature_on_durationOFF.notation = 0;


	/*Lettura Vline*/
	if(ExtMotor_DataBuffer[1] > 0)
	{/*Se DSP pronto leggo tensione*/
		VlineCalcolusFromDSP(&Vupd);
	}
	else if(MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].Vline > 0)
	{/*Altrimenti prendo vecchio riferimento*/
		LIB_Fixed_FromInt(MachineStatistics.lastCycles[MOST_RECENT_STAT_CYCLE_POS].Vline,7, &Vupd);
	}
	else
	{/*Se non c'è gli assegno la Vref*/
		Vupd.fractional = Vref.fractional ;
		Vupd.notation = Vref.notation;
	}
	/**/


	LIB_Fixed_FromInt(GITemp, 8, &Tupd);




	 /*Calcolo Voltage effect */
 	 if(LIB_Fixed_Div(Vupd, Vref, &result_var))
 	 {
 		 /*error*/
 	 }

 	 if(LIB_Fixed_Sqr(result_var,&result_var_1))
 	 {
 		 /*error*/
 	 }
 	 if(result_var_1.fractional != 0)
 	 {
 		 app_alpha_voltage_on_power.fractional = result_var_1.fractional;
 		 app_alpha_voltage_on_power.notation = result_var_1.notation;
 	 }

 	 /**/
 	 /*Voltage effect on Duration*/

 	 if(LIB_Fixed_Div(Vref, Vupd, &result_var))
 	 {
 		 /*error*/
 	 }

 	 if(LIB_Fixed_Sqr(result_var,&result_vefd))
 	 {
 		 /*error*/
 	 }

 	 if(result_vefd.fractional != 0)
 	 {
 		 app_alpha_voltage_on_durationON.fractional = result_var_1.fractional;
 		 app_alpha_voltage_on_durationON.notation = result_var_1.notation;
 	 }
 	 /**/

 	 /*Voltage and Temperature effect on Duration*/
 	 app_var.fractional = LIB_FIXED_FRACTIONAL_FROMFLOAT(1.8,4);
 	 app_var.notation = 4;

 	 if(LIB_Fixed_Sub(Tref,Tupd,&result_var))
 	 {
 		 /*error*/
 	 }
 	 if(LIB_Fixed_Div(result_var, app_var, &result_var_1))
 	 {
 		 /*error*/
 	 }

 	 if(LIB_Fixed_Mul(result_var_1,result_vefd,&result_var))
 	 {
 		 /*errore*/
 	 }

 	 if(result_var.fractional != 0)
 	 {
 		 app_alpha_voltage_temperature_on_durationON.fractional = result_var.fractional;
 		 app_alpha_voltage_temperature_on_durationON.notation = result_var.notation;
 	 }
 	 /**/
 	 result_var_1.fractional = 1;
 	 result_var_1.notation = 0;
 	 if(Tupd.fractional > Tref.fractional)
 	 {
 		 if(LIB_Fixed_Sub(Tupd,Tref,&result_var))
 		 {
 			 /*errore*/
 		 }
 		 if(LIB_Fixed_Div(result_var, app_var, &result_var_1))
 		 {
 			 /*error*/
 		 }
 	 }
 	 if(result_var_1.fractional != 0)
 	 {
 		app_alpha_temperature_on_durationOFF.fractional = result_var_1.fractional;
		app_alpha_temperature_on_durationOFF.notation = result_var_1.notation;
 	 }

}
/**/

/*
 * Function: VlineCalcolusFromDSP
 */

uint8_t VlineCalcolusFromDSP(LIB_Fixed_ValueType *valuePtr)
{
	int16_t funcValue;
	LIB_Fixed_ValueType Vmeasured,result;
	int16_t V_from_dsp;
	LIB_Fixed_ValueType coeff;

	   funcValue = (uint16_t) (( (uint16_t) ExtMotor_DataBuffer[6] << 8) | ExtMotor_DataBuffer[7]);
	   V_from_dsp = (int16_t)(((uint32_t)funcValue)>>(15-7));
	   Vmeasured.fractional = V_from_dsp;
	   Vmeasured.notation = 7;
	   coeff.fractional = 444;
	   coeff.notation = 0;

	   LIB_Fixed_Mul(Vmeasured, coeff, &result);
	   coeff.fractional = LIB_FIXED_FRACTIONAL_FROMFLOAT(1.4,7);
	   coeff.notation = 7;
	   LIB_Fixed_Div(result, coeff, &Vmeasured);

	   *valuePtr = Vmeasured;
	   funcValue = LIB_Fixed_Round(Vmeasured);


	   return((uint8_t)funcValue);
}
#endif

/* ************** */
/* Local Function */
/* ************** */

/* *********** */
/* End of file */
/* *********** */
