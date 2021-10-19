/**

\file    MngStatus.c
\ingroup  PM_ADM
\version 0.0.1
\date    Apr X 2010
\author  Andrea Mantovani
*/



/*
Copyright (c) 2007-2008, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


/* ******** */
/* Includes */
/* ******** */

#include "MngStatus.h"
#include "HighLevelConfig.h"
#include "UImngData.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* **************************** */
/* Prototype of local functions */
/* **************************** */



/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */

/* ***************** */
/* Exported function */
/* ***************** */

/* ************** */
/* Local Function */
/* ************** */



/**
This function  represent status info formatted as Struct_Status starting from raw status
for DW appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo		Code from AIM project.Check if ok.Turn comments to English   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusDW(byte_def coldResetEvent, byte_def *PointerBuf, Struct_Status *RefStatus)
{
  //struct Loader	cycle_header;
  ADMDataType cycle_header;
  uint8_t	mainStatus,ui8MainAdd;
  ADM_ErrorType eErr;

  eErr=ADM_GetMainAddr(&ui8MainAdd);
  if(eErr != ADM_E_OK) 
  {
    return E_STAT_KO;
  }
  
	//quando il flag PC control è a '1' l'elettrodomestico è sotto il controllo PC
	RefStatus->PC_control = (PointerBuf + OFFSET_FLAG)->BIT.B1; // PC_control è indipendente dal flag "ValidData"

	// Fault main, il flag allarmi è indipendente dallo stato
	RefStatus->Alarm = (PointerBuf + OFFSET_FLAG)->BIT.B6;

	// Pre-backup
	RefStatus->PreBackup = (PointerBuf+OFFSET_FLAG)->BIT.B7;

	// Valid Data
	if((PointerBuf + OFFSET_FLAG)->BIT.B0 == 0)
	{
		//quando il flag Data Valid è a '0' lo stato della main_board ancora non può essere letto
		mainStatus = STATUS_INVALID;
	}
	else
	{
		// §_§: manca la copia dei bit 3, 4, 5, del byte 0 dello stato 
		// Byte 0
		RefStatus->ColdReset = (PointerBuf+OFFSET_FLAG)->BIT.B2;

		// Byte 1
		RefStatus->SaltState 	   = (PointerBuf + DW_RS_FLAGS_1)->BIT.B0;
		RefStatus->RinseAidState = (PointerBuf + DW_RS_FLAGS_1)->BIT.B1;
		RefStatus->WarningH2O 	 = (PointerBuf + DW_RS_FLAGS_1)->BIT.B2;
		
		////////////  	sG
		//RefStatus->CollaudoState = (PointerBuf + DW_RS_FLAGS_1)->BIT.B3;
		//RefStatus->AutotestState = (PointerBuf + DW_RS_FLAGS_1)->BIT.B4;
		RefStatus->DemoState 	   = (PointerBuf + DW_RS_FLAGS_1)->BIT.B5;
		////////////	eG		

		if ((RefStatus->DoorState == DOOR_CLOSED) && (((PointerBuf + DW_RS_FLAGS_1)->BIT.B3) == DOOR_OPEN))
		{
			RefStatus->PendingDoorOpen = 1;	//fa vatto prima di copiare il bit nuovo sulla struttura
		}
		
		/* LL Introduzione PendingDoorClosed per gestione Autostart DW FI
			DW.UI_DGT&LED 2.0.0 sostituito firstGetApplStatusCompleted con coldResetEvent.BIT.B1 per bypassare aggiornamento PendingDoorClosed alla 1a lettura regolazioni (reset freddo UI) */
		if (((RefStatus->DoorState == DOOR_OPEN) && (((PointerBuf + DW_RS_FLAGS_1)->BIT.B3) == DOOR_CLOSED)) && ((coldResetEvent.BIT.B1==FALSE) && (RefStatus->ColdReset)))
		{																	
			RefStatus->PendingDoorClosed = 1;	
		}
		
		RefStatus->DoorState = (PointerBuf + DW_RS_FLAGS_1)->BIT.B3;

		// Ciclo impostato nella main
		RefStatus->MainCYCLE = (PointerBuf + OFFSET_MAINCAVITY_CYCLE)->BYTE;
		
		// Fase impostata nella main
		if(RefStatus->MainPHASE != (PointerBuf + OFFSET_MAINCAVITY_PHASE)->BYTE)
		{
			// Quando la fase è cambiata rispetto alla precedente setto un flag
			RefStatus->PendingPhase = 1;
		}
		RefStatus->MainPHASE = (PointerBuf + OFFSET_MAINCAVITY_PHASE)->BYTE;

		// Delay impostato nella main
		RefStatus->DelayMain_HH = (PointerBuf + DW_RS_DELAY_HOURS)->BYTE;
		RefStatus->DelayMain_MM = (PointerBuf + DW_RS_DELAY_MINUTES)->BYTE;
		RefStatus->DelayMain_SS = (PointerBuf + DW_RS_DELAY_SECONDS)->BYTE;

/*
		totalDelayMM = RefStatus->DelayMain_HH * 60 + RefStatus->DelayMain_MM;
		totalDelaySS = (((dword)(totalDelayMM)) * 60 + RefStatus->DelayMain_SS);
*/
    
    eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf + OFFSET_MAINCAVITY_CYCLE)->BYTE, &cycle_header);
    if(eErr != ADM_E_OK) 
    {
      return E_STAT_KO;
    }

		// Punto all'intestazione del ciclo corrente
		//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf + OFFSET_MAINCAVITY_CYCLE)->BYTE, &cycle_header);

		if ( (PROGRAM_HEADING(cycle_header.pui8Data) == STATUS_PROGRAMMING) && ((PointerBuf + OFFSET_MAINCAVITY_PHASE)->BYTE == 0) )
		{
			// Si assume che la fase 0 del ciclo etichettato come PROGRAMMING corrisponda allo
			// stato di OFF
			mainStatus = STATUS_UIOFF;		
		}
		else
		{
			mainStatus = PROGRAM_HEADING(cycle_header.pui8Data);
		}

		// Al momento non serve a niente
		// RefStatus->Program_ID_Name_MAIN = PROGRAM_ID_NAME(cycle_header.Data);

		if ((mainStatus != STATUS_RUN) && (mainStatus != STATUS_FAULT))
		{
			// Nei cicli di tipo diverso da RUN e FAULT le fasi corrispondono ai cicli di RUN
			// della macchina, quindi devo ricaricare il puntatore usando come displacement la fase
      eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf + OFFSET_MAINCAVITY_PHASE)->BYTE, &cycle_header);
      if(eErr != ADM_E_OK) 
      {
        return E_STAT_KO;
      }
    
			//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf + OFFSET_MAINCAVITY_PHASE)->BYTE, &cycle_header);
		}


		// Indice displacement tabella regolazioni
		RefStatus->Regulation_Sel_Index_MAIN = REG_SEL_INDEX(cycle_header.pui8Data);

		// SubHeading
		RefStatus->CycleSubHeading = SUBYCLE_HEADING(cycle_header.pui8Data);
		
		coldResetEvent.BIT.B1 = FALSE;
	}

	if (RefStatus->MainState != mainStatus)
	{
		// Quando lo stato è cambiata rispetto al precedente setto un flag
		RefStatus->PendingStatus = 1;
		/* start/stop of Energy consumption counter according to state change */
		//HandleEnergyConsCount(RefStatus->MainState,mainStatus); 
	}

	// Dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato
	RefStatus->MainState = mainStatus;

	return E_STAT_KO;
}


/**
This function  represent status info formatted as Struct_Status starting from raw status
for WM appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo	  Code from AIM project.Check if ok.Turn comments to English	   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusWM(byte_def *PointerBuf, Struct_Status *RefStatus) 
{ 
  //struct Loader	cycle_header;
  ADMDataType cycle_header; 
  uint8_t	localStatus,ui8MainAdd;
  ADM_ErrorType eErr; 
 
  eErr=ADM_GetMainAddr(&ui8MainAdd);
  if(eErr != ADM_E_OK) 
  {
    return E_STAT_KO;
  }
  
  
	//quando il flag "PC control" è a "1" l'elettrodomestico è sotto il controllo PC 
	RefStatus->PC_control = (PointerBuf+OFFSET_FLAG)->BIT.B1; 	// il PC_control è indipendete dal flag "ValidData" 
	RefStatus->Alarm = (PointerBuf+OFFSET_FLAG)->BIT.B6;			// memorizzo lo stato dell'allarme 
	 
	if(!((PointerBuf+OFFSET_FLAG)->BIT.B0))  
	{ 
        //quando il flag "Data Valid" è a "0" lo stato della main_board ancora non può essere letto 
		localStatus = STATUS_INVALID;		//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)		 
	} 
	else  
	{ 
		// Info Application Flags ////////////
		if ((RefStatus->ClosedDoor == 0) && (((PointerBuf+OFFSET_CRITERIA_WM)->BIT.B3) == 1)) 
		{
			RefStatus->PendingDoorOpen = 1;	// fa vatto prima di copiare il bit nuovo sulla struttura 
											// copio lo stato dei flag "clockset", "porta chiusa" e "porta bloccata" 
		}
		
		// sGO 06_09_2007
		RefStatus->WarningDoor = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B1; 
		RefStatus->WarningH2O = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B2; 
		// eGO
		
		RefStatus->ColdReset = (PointerBuf+OFFSET_FLAG)->BIT.B2; 
 
		RefStatus->ClosedDoor = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B3; 
		RefStatus->LockedDoor = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B4; 
		
		if(RefStatus->HydroStop != (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B5)
		{
		   RefStatus->PendingHydroStop = 1;
		   RefStatus->HydroStop = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B5; 
		}
		
		RefStatus->FaultInProgressFlag = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B6;
		
		RefStatus->rebalancingsOnCourse = (PointerBuf+OFFSET_CRITERIA_WM)->BIT.B7; 
		 
		if (RefStatus->MainPHASE != (PointerBuf+OFFSET_PHASE)->BYTE) 
		{
			RefStatus->PendingPhase = 1;		// quando la fase è stata cambiata dalla precedente setto un flag in RAM 
		}
		//////////////////////////////////////////
		
		RefStatus->MainPHASE = (PointerBuf+OFFSET_PHASE)->BYTE;	// fase impostata nella main cavity 
		RefStatus->MainCYCLE = (PointerBuf+OFFSET_CYCLE)->BYTE;	// ciclo impostato nella main cavity 
 		 
		if(RefStatus->DelayMinuti_HI != (PointerBuf+OFFSET_DELAY_HI)->BYTE || RefStatus->DelayMinuti_LO != (PointerBuf+OFFSET_DELAY_LO)->BYTE ) 
		{ 
			RefStatus->PendingDelay = 1; 
		} 
		
        RefStatus->DelayMinuti_HI = (PointerBuf+OFFSET_DELAY_HI)->BYTE; 
        RefStatus->DelayMinuti_LO = (PointerBuf+OFFSET_DELAY_LO)->BYTE; 
 
		if (RefStatus->Temperature != (PointerBuf+OFFSET_TEMPERATURE)->BYTE)
		{ 
			RefStatus->PendingTemperature = 1;	//quando la fase è stata cambiata dalla precedente setto un flag in RAM 
		}
		
		
    eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_CYCLE)->BYTE, &cycle_header);
    if(eErr != ADM_E_OK) 
    {
      return E_STAT_KO;
    }
      
		// come displacement passo il valore del "Ciclo" della MainCavity ---> PointerBuf+OFFSET_CYCLE 
		//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_CYCLE)->BYTE, &cycle_header); 
		
		if (( (0x0F&(*(cycle_header.pui8Data+1))) == STATUS_PROGRAMMING) && ((PointerBuf+OFFSET_PHASE)->BYTE == 0)) 
			localStatus = STATUS_UIOFF;	//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto) 
		else 
			localStatus = 0x0F & (*(cycle_header.pui8Data+1));	//load status of the main cavity 
 
		// RefStatus->Regulation_Sel_Index_MAIN = *(cycle_header.Data+3); 
		RefStatus->Program_ID_Name_MAIN = *(cycle_header.pui8Data+2); 
 
	    if ((localStatus != STATUS_RUN) && (localStatus != STATUS_FAULT)) 
		{
		  
       eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_PHASE)->BYTE, &cycle_header);
       if(eErr != ADM_E_OK) 
      {
        return E_STAT_KO;
      }
			//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_PHASE)->BYTE, &cycle_header); 
		}
		
		RefStatus->Regulation_Sel_Index_MAIN = *(cycle_header.pui8Data+3); 
 		
		// SIC_PUMP_OUT -> Questo campo è a tre bit quindi mi taglia valori come SIC_PUMP_OUT
		RefStatus->SubCycleHeading_MAIN = (0xF0 & (*(cycle_header.pui8Data+1))) >> 4;	
		
		// cycle_header e gia calcolato considerando la ciclo e fase in linea con lo stato.
		RefStatus->Cycle_Group = (0xE0 & (*cycle_header.pui8Data)) >> 5;		
	} 
	 
	if (RefStatus->MainState != localStatus) 
	{ 
		RefStatus->PendingStatus = 1;	//setto il pending per indicare stato elettrodomestico cambiato 
		/* start/stop of Energy consumption counter according to state change */
		//HandleEnergyConsCount(RefStatus->MainState,localStatus); 
	} 
   
	RefStatus->MainState = localStatus;	//dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato 
 
	return E_STAT_OK; 
} 


/**
This function  represent status info formatted as Struct_Status starting from raw status
for CK appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo		Code from AIM project.Check if ok.Turn comments to English   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusCK_HOT2005(byte_def *PointerBuf, Struct_Status *RefStatus)
{
	//struct Loader	cycle_header;
	ADMDataType cycle_header; 
	uint8_t	cavityM, cavityT;
	bool_t	statusApp = FALSE;
	uint16_t	delayTemp, clockTemp;
	signed short temperatura, temperatureTemp;	//variabile locale in cui vado a memorizzare temporaneamente la sonda
	static uint16_t CompareMinuti = 0;
	static uint8_t alarm = 0,ui8MainAdd;
	ADM_ErrorType eErr; 
	

  eErr=ADM_GetMainAddr(&ui8MainAdd);
  if(eErr != ADM_E_OK) 
  {
    return E_STAT_KO;
  }
  
	/* FILTRO CONSISTENZA VALORI "clock", "delay" e "temperature": QUANDO UNO DI QUESTI TRE VALORI 
	   SONO A 0xFFFF, IL PACCHETTO RICEVUTO NON VIENE CONSIDERATO VALIDO */
	if ((PointerBuf+OFFSET_FLAG)->BIT.B0)
		statusApp = TRUE;		//inizializza la variabile "statusApp" con il bit dello stato della main

	/*controllo della variabili DELAY, CLOCK e TEMPERATURE*/
	temperatureTemp = *((signed short *)((uint8_t *)PointerBuf+OFFSET_PROBE_MAINCAV));
	delayTemp = (((uint16_t)((PointerBuf+OFFSET_DELAY_MAINCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_DELAY_MAINCAV+1)->BYTE));	//aggiorna il delay impostato!
	clockTemp = (((uint16_t)((PointerBuf+OFFSET_CLOCK)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_CLOCK+1)->BYTE));

	if ((temperatureTemp > 800) || (delayTemp == 0xFFFF) || (clockTemp > 1439))
	{
		statusApp = FALSE;	//forzo un invalid status
	}
	/***************************************************/

//quando il flag "PC control" è a "1" l'elettrodomestico è sotto il controllo PC
	RefStatus->PC_control = (PointerBuf+OFFSET_FLAG)->BIT.B1; // il PC_control è indipendete dal flag "ValidData"
	RefStatus->Alarm = (PointerBuf+OFFSET_FLAG)->BIT.B6;	//memorizzo lo stato dell'allarme
	
	if (!(statusApp)) {
//quando il flag "Data Valid" è a "0" lo stato della main_board ancora non può essere letto
		cavityM = STATUS_INVALID;		//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)		
	}
	else {
		if ((RefStatus->ClosedDoorMainC == 1) && (((PointerBuf+OFFSET_CRITERIA_CK)->BIT.B0) == 0))
			RefStatus->PendingDoorOpen = 1;	//fa vatto prima di copiare il bit nuovo sulla struttura

//copio lo stato dei flag "clockset", "porta chiusa" e "porta bloccata"
		if ((RefStatus->ColdReset == 1) && (((PointerBuf+OFFSET_FLAG)->BIT.B2) == 0))
		{	
		  //con questo flag rilevo un reset freddo della main: OCCORRE IMPOSTARE IL CLOCK!
	    //se il flag O.C. è insertito, questo controllo non è più necessario!!
			//reg_old[position_clock_HH] = ~(reg_now[position_clock_HH]);	//forzo una scrittura regolazioni
		}

//copio lo stato dei flag "clockset", "porta chiusa" e "porta bloccata"
		RefStatus->ColdReset = (PointerBuf+OFFSET_FLAG)->BIT.B2;
//copio lo stato dei flag generici dello stato della MAIN_CAVITY o TOP_CAVITY
		RefStatus->ClosedDoorMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B0;
		RefStatus->LockedDoorMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B1;
		RefStatus->LampMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B7;
		
		if (RefStatus->MainPHASE != (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE)
		{
			RefStatus->PendingPhase = 1;	//quando la fase è stata cambiata dalla precedente setto un flag in RAM
		}
		
		RefStatus->MainPHASE = (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE;	//fase impostata nella main cavity
		RefStatus->MainCYCLE = (PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK)->BYTE;	//ciclo impostato nella main cavity

		RefStatus->MinutesClock = clockTemp;

		RefStatus->DurationTimeMainC = (((uint16_t)((PointerBuf+OFFSET_DURATIOM_MAINCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_DURATIOM_MAINCAV+1)->BYTE));

		if (temperatureTemp < 0)
			RefStatus->ProbeTempMainC = 0;
		else
			RefStatus->ProbeTempMainC = (uint16_t)temperatureTemp;

		RefStatus->DelayTimeMainC = delayTemp;
//	RefStatus->DelayTimeMainC = (((uint16_t)((PointerBuf+OFFSET_DELAY_MAINCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_DELAY_MAINCAV+1)->BYTE));	//aggiorna il delay impostato!

//come displacement passo il valore del "Ciclo" della MainCavity ---> PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK

    eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK)->BYTE, &cycle_header);
    if(eErr != ADM_E_OK) 
    {
      return E_STAT_KO;
    }
		//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK)->BYTE, &cycle_header);
		
		if (( (0x0F&(*(cycle_header.pui8Data+1))) == STATUS_PROGRAMMING) && ((PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE == 0))
			cavityM = STATUS_UIOFF;	//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)
		else
			cavityM = 0x0F & (*(cycle_header.pui8Data+1));	//load status of the main cavity

    if ((cavityM != STATUS_RUN) && (cavityM != STATUS_FAULT)) 
    {
      eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE, &cycle_header);
      if(eErr != ADM_E_OK) 
      {
        return E_STAT_KO;
      }
      //	UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE, &cycle_header);
    }
    
		
		RefStatus->Regulation_Sel_Index_MAIN = *(cycle_header.pui8Data+3);

		RefStatus->SubCycleHeading_MAIN = (0x70 & (*(cycle_header.pui8Data+1))) >> 4;	//ciclo impostato nella main cavity
	}
	if (RefStatus->MainState != cavityM)
	{
		RefStatus->PendingStatus = 1;	//setto il pending per indicare stato elettrodomestico cambiato
		RefStatus->PendingStatusLamp = 1;
		/* start/stop of Energy consumption counter according to state change */
		//HandleEnergyConsCount(RefStatus->MainState,cavityM);
	}
	
	if ((RefStatus->MainState == STATUS_RUN) && (cavityM == STATUS_PAUSE))
	{
		RefStatus->PendingStatusLamp = 0;	//setto il pending per indicare stato elettrodomestico cambiato
	}
	
	RefStatus->MainState = cavityM;	//dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato

	return E_STAT_OK;
}

/* 
	La funzione "GetApplianceStatus" ritorna con lo stato della main aggiornato in base 
	al buffer dei dati ricevuto
	*PointerBuf ---------> puntatore al buffer dei dati ricevuti dalla main
	*RefStatus ----------> puntatore alla struttura in cui è memorizzato lo stato
*/
ADM_StatusErrType GetApplianceStatusCK_HOTTIMA(byte_def *PointerBuf, Struct_Status *RefStatus)
{
	//struct Loader cycle_header;
	ADMDataType cycle_header;
	uint8_t	cavityM;
	#if (MAX_TARGETS == 2)
	uint8_t	cavityT=STATUS_UIOFF;		/* Va inizializzata se uso if (doubleApplication)*/
	#endif
	int16_t temperatureTemp;		// Variabile locale in cui vado a memorizzare temporaneamente la sonda
  ADM_ErrorType eErr; 
  uint8_t ui8MainAdd;
  
  eErr=ADM_GetMainAddr(&ui8MainAdd);
  if(eErr != ADM_E_OK) 
  {
    return E_STAT_KO;
  }
  	
	// Quando il flag "PC control" è a "1" l'elettrodomestico è sotto il controllo PC
	RefStatus->PC_control = (PointerBuf+OFFSET_OVEN_FLAG)->BIT.B1; // il PC_control è indipendete dal flag "ValidData"

	//quando il flag "Data Valid" è a "0" lo stato della main_board ancora non può essere letto
	if((PointerBuf + OFFSET_OVEN_FLAG)->BIT.B0 == FALSE)
	{
		cavityM = STATUS_INVALID;		// memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)		
		#if	(MAX_TARGETS == 2)
		cavityT = STATUS_INVALID;		// memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)		
		#endif
	}
	else
	{
		// Reset freddo della main
		//FT:RefStatus->ColdReset = (PointerBuf+OFFSET_OVEN_FLAG)->BIT.B2;
		
		// Fault Main Cavity e/o Top Cavity
		RefStatus->Alarm = (PointerBuf+OFFSET_OVEN_FLAG)->BIT.B6;		// memorizzo lo stato dell'allarme
		//FT: flag_ReadStatus_ReadReg = (PointerBuf+OFFSET_OVEN_FLAG)->BIT.B7;// controllo se è stata eseguita almeno una volta una lettura stato+una lettura regolazioni
		// Ciclo impostato nella main cavity
		RefStatus->MainCYCLE = (PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK)->BYTE;
		
		//FT: flag_DurationMainOn = (PointerBuf+OFFSET_OVEN_FLAG)->BIT.B3;
		
		//flag_Spillone = (PointerBuf+OFFSET_CRITERIA)->BIT.B5;

		// Fase impostata nella main cavity
		if (RefStatus->MainPHASE != (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE)
		{
			// Quando la fase è cambiata rispetto alla precedente setto un flag
			RefStatus->PendingPhase = 1;
		}
		RefStatus->MainPHASE = (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE;

		if ((RefStatus->ClosedDoorMainC==TRUE) && (((PointerBuf+OFFSET_CRITERIA_CK)->BIT.B0) == FALSE))
		{
			RefStatus->PendingDoorOpen = 1;	//va vatto prima di copiare il bit nuovo sulla struttura
		}
		
		RefStatus->ClosedDoorMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B0;
		RefStatus->LockedDoorMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B1;
		RefStatus->LockedDoorMainUser = ((PointerBuf+OFFSET_CRITERIA_CK)->BIT.B6) & ((PointerBuf+OFFSET_CRITERIA_CK)->BIT.B1);
		RefStatus->LampMainC = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B7;
		
		//FT: RefStatus->MainSleepMode = (PointerBuf+OFFSET_CRITERIA_CK)->BIT.B4;
		
		#if (CLOCK_MODULE == TRUE)	 /// copio nella vecchia struttura dello stato della Main, i valori dell'RTC sW
	    //FT:Clock_Get_Time(&real_hour,&real_minutes,&real_seconds);
	    //FT:RefStatus->MinutesClock =((uint16_t)(real_hour*60)+(word)(real_minutes));
	    //FT:RefStatus->StandByEnable = (PointerBuf+OFFSET_CLOCK)->BIT.B7;
		#else /*(CLOCK_MODULE==TRUE)*/
		// Valore dell'orologio in minuti
		// (Le word sono allocate su address pari conviene spostarsi sempre a byte) RefStatus->MinutesClock = *((word *) ((byte *)PointerBuf + OFFSET_CLOCK));
		//FT:RefStatus->MinutesClock = (((uint16_t)((PointerBuf+OFFSET_CLOCK)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_CLOCK+1)->BYTE));
		#endif /*(CLOCK_MODULE==FALSE)*/
		// Valore della durata in minuti - main cavity
		// (Le word sono allocate su address pari conviene spostarsi sempre a byte) RefStatus->DurationTimeMainC = *((word *) ((byte *)PointerBuf+OFFSET_DURATIOM_MAINCAV));
	//	RefStatus->DurationTimeMainC = (((word)((PointerBuf+OFFSET_DURATIOM_MAINCAV)->BYTE))<<8) | ((word)((PointerBuf+OFFSET_DURATIOM_MAINCAV+1)->BYTE));
		
		// Controllo del valore della temperatura - main cavity
		// (Le word sono allocate su address pari conviene spostarsi sempre a byte) temperatureTemp = *((sword *)((byte *)PointerBuf+OFFSET_PROBE_MAINCAV));
		temperatureTemp = (((uint16_t)((PointerBuf+OFFSET_PROBE_MAINCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_PROBE_MAINCAV+1)->BYTE));		
	
		if (temperatureTemp < 0)
		{
			RefStatus->ProbeTempMainC = 0;
		}
		else
		{
			RefStatus->ProbeTempMainC = (uint16_t)temperatureTemp;
		}
		
		
		// Valore della delay in minuti - main cavity
		// (Le word sono allocate su address pari conviene spostarsi sempre a byte) RefStatus->DelayTimeMainC = *((word *) ((byte *)PointerBuf+OFFSET_DELAY_MAINCAV));	//aggiorna il delay impostato
//		RefStatus->DelayTimeMainC = (((word)((PointerBuf+OFFSET_DELAY_MAINCAV)->BYTE))<<8) | ((word)((PointerBuf+OFFSET_DELAY_MAINCAV+1)->BYTE));	//aggiorna il delay impostato					
				
		// Punto all'intestazione del ciclo corrente nella main cavity
		eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_CYCLE_CK)->BYTE, &cycle_header);
    if(eErr != ADM_E_OK) 
    {
      return E_STAT_KO;
    }
		
		//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_CYCLE)->BYTE, &cycle_header);
		
		if (((0x0F&(*(cycle_header.pui8Data+1))) == STATUS_PROGRAMMING) && ((PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE == 0))
		{
			// Si assume che la fase 0 del ciclo etichettato come PROGRAMMING corrisponda allo stato di OFF
			cavityM = STATUS_UIOFF;						// memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)
		}
		else
		{
			cavityM = 0x0F & (*(cycle_header.pui8Data+1));	// load status of the main cavity
		}
		
		RefStatus->Program_ID_Name_MAIN = *(cycle_header.pui8Data+2);

	    if ((cavityM != STATUS_RUN) && (cavityM != STATUS_FAULT))
	    {
			// Nei cicli di tipo diverso da RUN e FAULT le fasi corrispondono ai cicli di RUN
			// della macchina, quindi devo ricaricare il puntatore usando come displacement la fase
			
			eErr=ADM_BasicLoader(ui8MainAdd, PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_PHASE_CK)->BYTE, &cycle_header);
      if(eErr != ADM_E_OK) 
      {
        return E_STAT_KO;
      }
			//UIBasicLoader(PTR_PROG_HEADING0, (PointerBuf+OFFSET_MAINCAVITY_PHASE)->BYTE, &cycle_header);
		}

		// SubHeading ciclo impostato nella main cavity (allungato campo a 4 bit)
		RefStatus->SubCycleHeading_MAIN = (0xF0 & (*(cycle_header.pui8Data+1))) >> 4;

		// Indice displacement tabella regolazioni
		RefStatus->Regulation_Sel_Index_MAIN = *(cycle_header.pui8Data+3);
		
		if (RefStatus->MainState != cavityM)
		{
			RefStatus->PendingStatus = 1;		// setto il pending per indicare stato elettrodomestico cambiato
			RefStatus->PendingStatusLamp = 1;
		}
		
		if (( (RefStatus->MainState == STATUS_RUN) && (cavityM == STATUS_PAUSE) )||( (RefStatus->MainState == STATUS_END) && (cavityM == STATUS_PROGRAMMING) ))
		{
			RefStatus->PendingStatusLamp = 0;	//reset del pending per evitare il DON'T CARE nella gestione della lampada
		}
				
		#if	(MAX_TARGETS == 2)	
		if (doubleApplication == TRUE)
		{
			// Ciclo impostato nella top cavity
			RefStatus->TopCYCLE = (PointerBuf+OFFSET_TOPCAVITY_CYCLE)->BYTE;
			
			// Fase impostata nella top cavity
			if (RefStatus->TopPHASE != (PointerBuf+OFFSET_TOPCAVITY_PHASE)->BYTE)
			{
				// Quando la fase è cambiata rispetto alla precedente setto un flag
				RefStatus->PendingPhaseTop = 1;
			}
			RefStatus->TopPHASE = (PointerBuf+OFFSET_TOPCAVITY_PHASE)->BYTE;
			
			// Variazione di stato dell'ingresso divider (gestito e filtrato dalla main)
			if ( (RefStatus->DividerIn != (PointerBuf+OFFSET_CRITERIA)->BIT.B2) && (coldResetDivider == FALSE) )
			{
				RefStatus->DividerVariation = TRUE;
			}
			
			// Stato dell'ingresso divider: se ad "1" divider inserito nel forno
			RefStatus->DividerIn = (PointerBuf+OFFSET_CRITERIA)->BIT.B2;
			
			// Valore della durata in minuti - main cavity
			// (Le word sono allocate su address pari conviene spostarsi sempre a byte) RefStatus->DurationTimeTopC = *((word *) ((byte *)PointerBuf+OFFSET_DURATIOM_TOPCAV));
			RefStatus->DurationTimeTopC = (((uint16_t)((PointerBuf+OFFSET_DURATIOM_TOPCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_DURATIOM_TOPCAV+1)->BYTE));
			
			// Controllo del valore della temperatura - main cavity
			// (Le word sono allocate su address pari conviene spostarsi sempre a byte) temperatureTemp = *((sword *)((byte *)PointerBuf+OFFSET_PROBE_TOPCAV));
			temperatureTemp = (((uint16_t)((PointerBuf+OFFSET_PROBE_TOPCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_PROBE_TOPCAV+1)->BYTE));
			
			if (temperatureTemp < 0)
			{
				RefStatus->ProbeTempTopC = 0;
			}
			else
			{
				RefStatus->ProbeTempTopC = (word)temperatureTemp;
			}
			
			// Valore della delay in minuti - main cavity
			// (Le word sono allocate su address pari conviene spostarsi sempre a byte) RefStatus->DelayTimeTopC = *((word *) ((byte *)PointerBuf+OFFSET_DELAY_TOPCAV));	//aggiorna il delay impostato
			RefStatus->DelayTimeTopC = (((uint16_t)((PointerBuf+OFFSET_DELAY_TOPCAV)->BYTE))<<8) | ((uint16_t)((PointerBuf+OFFSET_DELAY_TOPCAV+1)->BYTE));

			// Punto all'intestazione del ciclo corrente nella top cavity			
			UIBasicLoader(PTR_PROG_HEADING1, (PointerBuf+OFFSET_TOPCAVITY_CYCLE)->BYTE, &cycle_header);
			if (((0x0F&(*(cycle_header.Data+1))) == STATUS_PROGRAMMING) && ((PointerBuf+OFFSET_TOPCAVITY_PHASE)->BYTE == 0))
			{
				cavityT = STATUS_UIOFF;		//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)
			}
			else
			{
				cavityT = 0x0F & (*(cycle_header.pui8Data+1));	//load status of the top cavity
			}

			RefStatus->Program_ID_Name_TOP = *(cycle_header.pui8Data+2);
			
		    if ((cavityT != STATUS_RUN)	&& (cavityT != STATUS_FAULT))
			{	
				UIBasicLoader(PTR_PROG_HEADING1, (PointerBuf+OFFSET_TOPCAVITY_PHASE)->BYTE, &cycle_header);
			}
			
			// SubHeading ciclo impostato nella top cavity (allungato campo a 4 bit)
			RefStatus->SubCycleHeading_TOP = (0xF0 & (*(cycle_header.pui8Data+1))) >> 4;	//ciclo impostato nella top cavity

			RefStatus->Regulation_Sel_Index_TOP = *(cycle_header.pui8Data+3);
							
			if (RefStatus->TopState != cavityT)
			{
				RefStatus->PendingStatusTop = 1;	// setto il pending per indicare stato elettrodomestico cambiato
			}
		}
		else
		{
			cavityT = STATUS_UIOFF;					// BC utile nel MS per gestire l'OFF delle 2 cavità con stesso FW (MAX_TARGETS=2)
		}
		#endif /*(MAX_TARGETS == 2)*/
		
		//FT: coldResetDivider = FALSE; 			// E' terminata una lettura valida
	}
	
	// Dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato
	RefStatus->MainState = cavityM;

	#if	(MAX_TARGETS == 2)
	RefStatus->TopState = cavityT;	//dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato
	#endif
}


/**
This function  represent status info formatted as Struct_Status starting from raw status
for DR appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo		Code from AIM project.Check if ok.Turn comments to English   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusDR(byte_def *PointerBuf, Struct_Status *RefStatus) 
{
  uint8_t	localStatus;
  
	/*
	 ;BUFFER DI LETTURA DELLA MAIN             
0- rselector          ds.b 1    ;ritorno del selettore
1- rfase              ds.b 1    ;fase di lavaggio - (il sistema di collaudo legge e scrive in questa locazione per verificare la corretta comunicazione con la scheda di controllo)
2- rflags             ds.b 1    ;flags
3- rfault             ds.b 1    ;valore del fault
4- rntc               ds.b 1    ;temperatura della vasca (valore AD converter da NTC)
5- rcounter_h         ds.b 1    ;contatore a 16 bit per il delay timer
6- rcounter           ds.b 1
7- rwarnings1         ds.b 1    ;flags di warnings
8- rwarnings2         ds.b 1    ;flags di warnings
9	 rtimetoend         ds.b 1    ;tempo alla fine del ciclo (utilizzato sul dryer, nelle vecchie versioni della LVB c'era l'rselect_vis)
	 main_read_end
	 ;NOTA: per adesso questi non servono da leggere, bastano i primi 10
	 ;rdurezza          ds.b 1    
	 ;rloads1           ds.b 1    ;bitmap1 dei carichi attivi
	 ;rloads2           ds.b 1    ;bitmap2 dei carichi attivi
	 ;rrpm              ds.b 1
	 ;ralim             ds.b 1
	 ;rselect_vis       ds.b 1    ;selettore di visualizzazione (per la scheda con lo smartselector)
	 STATUS = rselector           ;label per la variabile di stato
	 FLAGS = rflags               ;label per la bitmap dei flags dalla main

	 ;FLAGS DEFINITI DIRETTAMENTE SUL BUFFER DI LETTURA
	 wash_status = 7              ;STATUS (R5H)
	 delay_status = 6             ;STATUS

	 -blocco_porta = 5             ;FLAGS  (R5L)
	 statistics_en = 6            ;FLAGS
	 -idrostop_main = 7            ;FLAGS  (è quello che Claudio chiama selector_en)
	 -switch_door = 1              ;FLAGS  (switch_door = 1 -> porta chiusa, switch_door = 0 -> porta aperta)
	 memo_ok = 0                  ;FLAGS  (feedback dell'operazione di store memory da parte della main: memo_ok = 0 -> memorizzazione eseguita correttamente, memo_ok = 1 -> funzione memory abilitata)
	 -pause_main = 2               ;FLAGS
	 H2O_warning = 0		          ;rwarnings1
	 -empty_warning = 0            ;rwarnings1


    ;BUFFER DI SCRITTURA DELLA MAIN
    main_write
0-  wselector	ds.b 1	;selettore
1-  wkeyb  		ds.b 1	;impostazione tasti (flags)
2-  wkeyb2		ds.b 1	;estensione dei tasti
3-  wspin 		ds.b 1	;impostazione centrifuga
4-  wtemper		ds.b 1	;impostazione temperatura
5-  wdelay		ds.b 1	;impostazione delay
6-  wdry		  ds.b 1	;impostazione asciugatura
7-  wintens		ds.b 1	;impostazione intensità
8-  wrinse		ds.b 1	;impostazione risciacqui
9-  countdown_bak	ds.b 1	;backup del countdown con il valore nominale da eprom
10- jolly		ds.b 1	;bitmap jolly (attualmente la uso per ripristinare la storia delle fasi precedenti)
11- bit_back	ds.b 1
12,13- countdown	ds.w 1	;contatore della durata del lavaggio in min (a word)
    main_write_end

	 */

//	RefStatus->Alarm = (PointerBuf+OFFSET_FLAG)->BIT.B6;			// memorizzo lo stato dell'allarme 
	 
//	if(!((PointerBuf+OFFSET_FLAG)->BIT.B0))  
//	{ 
// 		localStatus = STATUS_INVALID;		//memorizzo lo stato in questa variabile locale per controllare la transizione (vedi sotto)		 
//	} 
//	else  
	{ 
		// Info Application Flags ////////////
		if ((RefStatus->ClosedDoor == 0) && (((PointerBuf+OFFSET_CRITERIA_DR)->BIT.B1) == 1)) 
		{
			RefStatus->PendingDoorOpen = 1;
		}
		
		//RefStatus->WarningDoor = (PointerBuf+OFFSET_CRITERIA_DR)->BIT.B1; 
		RefStatus->WarningH2O = (PointerBuf+OFFSET_ALARM1_DR)->BIT.B0; 
		RefStatus->ClosedDoor = (PointerBuf+OFFSET_CRITERIA_DR)->BIT.B1; 
		RefStatus->LockedDoor = (PointerBuf+OFFSET_CRITERIA_DR)->BIT.B5; 
		
		if(RefStatus->HydroStop != (PointerBuf+OFFSET_CRITERIA_DR)->BIT.B7)
		{
		   RefStatus->PendingHydroStop = 1;
		   RefStatus->HydroStop = (PointerBuf+OFFSET_CRITERIA_DR)->BIT.B7; 
		}
		
		if (RefStatus->MainPHASE != ((((PointerBuf+OFFSET_PHASE_DR)->BYTE)&0xF0)>>4))
		{
			RefStatus->PendingPhase = 1;		// quando la fase è stata cambiata dalla precedente setto un flag in RAM 
		}
		
		RefStatus->MainPHASE = ((((PointerBuf+OFFSET_PHASE_DR)->BYTE)&0xF0)>>4);	    // fase in esecuzione sulla main
		//RefStatus->MainCYCLE = (((PointerBuf+OFFSET_CYCLE_DR)->BYTE)&0x3F);	        // ciclo in esecuzione sulla main
		RefStatus->MainCYCLE = ((PointerBuf+OFFSET_CYCLE_UI_DR)->BYTE);	              // ciclo impostato dalla visual

	  RefStatus->DurationTimeMainC = (uint16_t)(((PointerBuf+OFFSET_TTE_HI_DR)->BYTE) << 8 | ((PointerBuf+OFFSET_TTE_LO_DR)->BYTE));
    
    RefStatus->FaultActive = (PointerBuf+OFFSET_FAULT_DR)->BYTE;

    /* elabora tutte le condizioni di stato */
    if (((PointerBuf+OFFSET_FLAGS_HI_DR)->BIT.B7) == FALSE)  //se il bit power della UI è 0 il dryer è spento
    {
      localStatus = STATUS_UIOFF;
    }
    else if (((PointerBuf+OFFSET_FAULT_DR)->BYTE) != 0)
    {
      localStatus = STATUS_FAULT;
    }
    //else if ((((PointerBuf+OFFSET_CRITERIA_DR)->BIT.B2) == TRUE) && (((PointerBuf+OFFSET_CYCLE_DR)->BIT.B7) == TRUE)) //pause_main = 1 e wash_status = 1 il dyer è in pausa
    else if ((((PointerBuf+OFFSET_FLAGS_HI_DR)->BIT.B4) == TRUE) && (((PointerBuf+OFFSET_FLAGS_HI_DR)->BIT.B5) == TRUE)) //se i 2 bit di pausa della visual sono ad 1 il dyer è in pausa
    {
      localStatus = STATUS_PAUSE;
    }
    else if (((((PointerBuf+OFFSET_CYCLE_DR)->BYTE)&0x3F) == 32) || ((((PointerBuf+OFFSET_CYCLE_DR)->BYTE)&0xC0) == 0xC0)) //se stà eseguendo il ciclo 32 oppure entrambi i flags  wash + delay sono settati
    {
      localStatus = STATUS_DELAY;
    }
    else if (((PointerBuf+OFFSET_CYCLE_DR)->BIT.B7) == TRUE)   //se wash_status = 1 il dryer è in run
    {
      localStatus = STATUS_RUN;
    }
    //else if ((RefStatus->MainPHASE == 16) && ((PointerBuf+OFFSET_CYCLE_DR)->BIT.B7) == FALSE)  //transitorio... forse da leggere sulla visual
    else if (((PointerBuf+OFFSET_BACKUP_FLAGS_DR)->BIT.B2) == TRUE) //flag che la visual backuppa sulla ram della main che indica il rilevamento dello stato di END
    {
      localStatus = STATUS_END;
    }
    else
    {
      localStatus = STATUS_PROGRAMMING;
    }
	} 

  if (localStatus == STATUS_PROGRAMMING)
  {
    RefStatus->DelayTimeMainC = (uint16_t)(((PointerBuf+OFFSET_DELAY_DR)->BYTE)*60);
  }
  else if (((((PointerBuf+OFFSET_CYCLE_DR)->BYTE)&0x3F) == 32) || ((((PointerBuf+OFFSET_CYCLE_DR)->BYTE)&0xC0) == 0xC0))
  {
    RefStatus->DelayTimeMainC = (uint16_t)(((((PointerBuf+OFFSET_DELAY_HI_DR)->BYTE & 0x3F) << 8 | ((PointerBuf+OFFSET_DELAY_LO_DR)->BYTE)) +5)/10);
  }
  else
  {
    RefStatus->DelayTimeMainC = 0;
  }

	if (RefStatus->MainState != localStatus) 
	{ 
		RefStatus->PendingStatus = 1;	//setto il pending per indicare stato elettrodomestico cambiato 
	} 
 
	RefStatus->MainState = localStatus;	//dopo aver controllato nuovo e vecchio valore dello stato, aggiorno la variabile con il nuovo stato 
 
	return E_STAT_OK; 
}


/**
This function  represent status info formatted as Struct_Status starting from raw status
for CL appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo	 Code from AIM project.Check if ok.Turn comments to English	   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusCL(byte_def *PointerBuf, Struct_Status *RefStatus) 
{
  uint8_t newStatus;
  
  RefStatus->MainCYCLE = (PointerBuf+OFFSET_CYCLE_FR)->BYTE;
  RefStatus->FreezerCYCLE = (PointerBuf+OFFSET_CYCLE_FZ)->BYTE;
  RefStatus->VirtualCYCLE = (PointerBuf+OFFSET_CYCLE_VR)->BYTE;
  
  RefStatus->WarningTemper1FZ = (PointerBuf+OFFSET_ALARM3)->BIT.B0;
  RefStatus->WarningTemper2FZ = (PointerBuf+OFFSET_ALARM3)->BIT.B1;
  RefStatus->WarningTemperFR = (PointerBuf+OFFSET_ALARM3)->BIT.B3;
  RefStatus->WarningDoorFR = (PointerBuf+OFFSET_ALARM3)->BIT.B4;
  RefStatus->WarningEndEverFresh = (PointerBuf+OFFSET_ALARM3)->BIT.B6;
  RefStatus->WarningEndIceParty = (PointerBuf+OFFSET_ALARM3)->BIT.B7;
  RefStatus->EverFreshFnc = (PointerBuf+OFFSET_SPECIAL_FUNC)->BIT.B0;
  RefStatus->DemoState = (PointerBuf+OFFSET_SPECIAL_FUNC)->BIT.B1;
  RefStatus->ProbesFault = (PointerBuf+OFFSET_PROBES_FAULT)->BYTE;

  RefStatus->TemperLevelFR = (PointerBuf+OFFSET_TEMP_REG_FR)->BYTE;
  RefStatus->TemperLevelFZ = (PointerBuf+OFFSET_TEMP_REG_FZ)->BYTE;

  //purtroppo non abbiamo accesso al file di setting main direttamente, quindi non possiamo leggere le intestazioni delle funzioni
  //abbiamo quindi scelto di cablare lo stato dei vani in funzione del modello e del file di setting del frigo
  //file di setting: 25365580002 e modello FF200TX  
  if ((RefStatus->MainCYCLE == OFF_CYCLE_FR) && (RefStatus->FreezerCYCLE == OFF_CYCLE_FZ))
	{
		// stato di OFF
		//RefStatus->MainState =  STATUS_UIOFF;  		
		newStatus = STATUS_UIOFF;
	}
	else
	{
	  // stato di RUN
	  //RefStatus->MainState =  STATUS_RUN;
	  newStatus = STATUS_RUN;
	}
	
	/* start/stop of Energy consumption counter according to state change */
		//HandleEnergyConsCount(RefStatus->MainState,newStatus);
		
		RefStatus->MainState = newStatus;
	
	return E_STAT_OK;
		
}

/**
This function  represent status info formatted as Struct_Status starting from raw status
for CL2008 appliance


\param  [in]		coldResetEvent : Cold reset event is used to calculate some status info and must
                                 be passed as input parameter from application.
\param  [in]		PointerBuf     : raw status info
\param  [out] 	RefStatus      : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed RefStatus contains valid info
\retval	ADM_E_KO	Status NOT correctly processed RefStatus contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0

\todo		Code from AIM project.Check if ok.Turn comments to English   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/	
ADM_StatusErrType GetApplianceStatusCL2008(byte_def *PointerBuf, Struct_Status *RefStatus) 
{
  
  uint8_t newStatus;
  
  RefStatus->MainCYCLE    = (PointerBuf+OFFSET_CYCLE_FR_CL2008)->BYTE;
  RefStatus->FreezerCYCLE = (PointerBuf+OFFSET_CYCLE_FZ_CL2008)->BYTE;
  RefStatus->VirtualCYCLE = (PointerBuf+OFFSET_CYCLE_VR_CL2008)->BYTE;
  
  RefStatus->WarningTemper1FZ     = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B0;
  RefStatus->WarningTemper2FZ     = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B1;
  RefStatus->WarningTemperFR      = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B2;
  RefStatus->WarningDoorFR        = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B3;
  RefStatus->WarningEndIceParty   = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B5;
  RefStatus->WarningEndEverFresh  = (PointerBuf+OFFSET_ALARMS_CL2008)->BIT.B6;
  
  // These parameters are not updated here cause these infos are available in Regulations and not in status
  
  //???RefStatus->EverFreshFnc = (PointerBuf+OFFSET_SPECIAL_FUNC_CL2008)->BIT.B0;
  //???RefStatus->DemoState    = (PointerBuf+OFFSET_SPECIAL_FUNC_CL2008)->BIT.B1;
  //???RefStatus->ProbesFault  = (PointerBuf+OFFSET_PROBES_FAULT_CL2008)->BYTE;

  //???RefStatus->TemperLevelFR = (PointerBuf+OFFSET_TEMP_REG_FR_CL2008)->BYTE;
  //???RefStatus->TemperLevelFZ = (PointerBuf+OFFSET_TEMP_REG_FZ_CL2008)->BYTE;
  
  if ((RefStatus->MainCYCLE == OFF_CYCLE_FR) && (RefStatus->FreezerCYCLE == OFF_CYCLE_FZ))
	{
		// stato di OFF
		//RefStatus->MainState =  STATUS_UIOFF;  		
		newStatus = STATUS_UIOFF;
	}
	else
	{
	  // stato di RUN
	  //RefStatus->MainState =  STATUS_RUN;
	  newStatus = STATUS_RUN;
	}
	
	/* start/stop of Energy consumption counter according to state change */
		//HandleEnergyConsCount(RefStatus->MainState,newStatus);
		
		RefStatus->MainState = newStatus;
		
		return E_STAT_OK; 
}



