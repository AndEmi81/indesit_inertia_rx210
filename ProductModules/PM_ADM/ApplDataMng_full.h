/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		Appliance Data Managing
\n			Header File of ApplDataMng_full.c
\n
\file		ApplDataMng_full.h 
\ingroup	PM_ADM 
\date		Feb X, 2010 
\version	01.00.00 
\author		andrea.mantovani 
\n
\n
\Par\b	History:
\n
\n 		00.24.00 
\n		Dec 9, 2009 
\n		Andrea.Mantovani 
\n		creation 
\n
\n
\n etc.
*/

#ifndef __APPLDATAMNG_H
#define __APPLDATAMNG_H

/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include "IO_driver.h"

#include "BCP_General.h"
#include "BusCommPltf_cfg.h"
#include "BCP_BusCommMng.h"

#include "ADMlib.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t ADM_InitOptionsType;


 /* struttura unificata di gestione dello stato della scheda main ---- */

/** This represent the unified rappresentation of status for main board */
typedef struct
{
	uint8_t MainState; //è un codice univoco indicante lo stato della MainCavity (PROGRAMMING,RUN,DELAY,FAULT,END,PAUSE.. aggiunto codice PC_control e DataValid)
	uint8_t MainPHASE; //fase impostata nella main cavity
	uint8_t MainCYCLE; //ciclo impostato nella main cavity

	uint8_t SubCycleHeading_MAIN :4; //memorizza il sottocodice per identificare cicli speciali tipo DEMO, COLLAUDO, AUTOTEST (main cavity)
	uint8_t PendingPhase :1; //pending indicante il cambiamento di ciclo e/o fase
	uint8_t PendingStatus :1; //pending indicante lo stato del prodotto cambiato
	uint8_t Alarm :1; //questo flag rimane ad "1" per tutta la durata del fault sulla main board (vale sia per la top che per la main cavity)
	uint8_t ColdReset :1; //ad "1" quando la scheda main ha eseguito un reset freddo

	uint8_t Program_ID_Name_MAIN; //nome del programma corrente main_cavity
	uint8_t Regulation_Sel_Index_MAIN; //index del valore della regolazione corrente

	uint8_t PendingDoorOpen :1; //pending apertura porta
	uint8_t PendingDoorClosed :1; /* pending utilizzato per Autostart in DW DGT&LED FI */
	uint8_t PC_control :1; //se ad "1" l'elettrodomestico viene controllato da remoto tramite PC
	uint8_t CollaudoState :1;
	uint8_t AutotestState :1;
	uint8_t DemoState :1;
	uint8_t WarningH2O :1; // warning acqua, '1' -> manca acqua
	uint8_t WarningDoor :1;

	uint8_t CycleSubHeading :3; // memorizza il sottocodice per identificare cicli speciali tipo DEMO, COLLAUDO, AUTOTEST (main cavity)
	uint8_t :5;

	//DW
	uint8_t SaltState :1; //stato sale, '1' -> manca sale
	uint8_t RinseAidState :1; //stato brillantante, '1' -> manca brillantante
	uint8_t DoorState :1; //stato porta, '1' -> porta chiusa
	uint8_t PreBackup :1;
	uint8_t :4;

	uint8_t DelayMain_HH; //valore delle ore del delay gestito dalla main
	uint8_t DelayMain_MM; //valore dei minuti del delay gestito dalla main
	uint8_t DelayMain_SS; //valore dei secondi del delay gestito dalla main

	//CK
	uint8_t ClosedDoorMainC :1; //porta chiusa main cavity
	uint8_t LockedDoorMainC :1; //porta bloccata main cavity
	uint8_t LockedDoorMainUser :1; //porta main cavity bloccata da utente
	uint8_t LampMainC :1; //lampada main cavity accesa
	uint8_t PendingStatusLamp :1; //pending variazione stato lampada 
	uint8_t PendingClock :1; //pending indicante il cambiamento del minuto nell'orologio
	uint8_t :2;

	uint16_t MinutesClock; //valore dell'orologio in minuti
	uint16_t DurationTimeMainC; //valore in minuti della durata cottura nella main cavity
	uint16_t ProbeTempMainC; //valore della temperatura misurata nella main cavity
	uint16_t DelayTimeMainC; //valore del delay gestito dalla main cavity

	//WM
	uint8_t PendingDelay :1; //pending indicante il cambiamento del minuto nell'orologio
	uint8_t PendingTemperature :1;
	uint8_t FaultInProgressFlag :1;
	uint8_t PendingHydroStop :1;
	uint8_t rebalancingsOnCourse :1;
	uint8_t LockedDoor :1; //porta bloccata main cavity
	uint8_t ClosedDoor :1; //porta chiusa main cavity
	uint8_t HydroStop :1;

	uint8_t DelayMinuti_HI;
	uint8_t DelayMinuti_LO;
	uint8_t Temperature;
	uint8_t Cycle_Group;

  //DR
  uint8_t FaultActive;     //fault corrente
  
	//CL
	uint8_t WarningTemper1FZ :1; //warning di temperatura alta del frizeer -> consumare velocemente il cibo in freezer
	uint8_t WarningTemper2FZ :1; //warning di temperatura alta del frizeer -> cibo in freezer da buttare
	uint8_t WarningBuzzer :1;
	uint8_t WarningTemperFR :1; //warning temperatura frigo   //ESmark280709
	uint8_t WarningDoorFR :1; //warning porta frigo aperta   //ESmark280709
	uint8_t WarningEndEverFresh :1; //warning fine funzione ever fresh   //ESmark280709
	uint8_t WarningEndIceParty :1; //warning fine ice party   //ESmark280709
  uint8_t EverFreshFnc :1; //stato funzione ever fresh    //ESmark280709
  
	uint8_t FreezerCYCLE; //ciclo impostato nel freezer   //ESmark280709
	uint8_t VirtualCYCLE; //ciclo impostato nel vano virtuale (serve per capire il defrost)   //ESmark280709VirtualCYCLE
 	uint8_t TemperLevelFR; //livello di temperatura impostata per il frigo   //ESmark280709
 	uint8_t TemperLevelFZ; //livello di temperatura impostata per il freezer   //ESmark280709
   	
  uint8_t ProbesFault;       //bitmap dei fault relativi alle sonde di temperature    //ESmark280709
/*
	uint8_t AirProbeFG :1; //stato sonda aria frigo    //ESmark280709
	uint8_t EvpProbeFG :1; //stato sonda evaporatore frigo    //ESmark280709
 	uint8_t AirProbeFZ :1; //stato sonda aria freezer    //ESmark280709
	uint8_t EvpProbeFZ :1; //stato sonda evaporatore freezer    //ESmark280709
	uint8_t AirProbe   :1; //stato sonda aria ulteriore    //ESmark280709
	uint8_t EvpProbe   :1; //stato sonda evaporatore ulteriore    //ESmark280709
	uint8_t EnvirProbe :1; //stato sonda ambiente      //ESmark280709
  uint8_t            :1;
*/

} 
Struct_Status;
/* ------------------------------------------------------------------ */

/**
\enum eADM_INIT_OPTIONS
*/
enum eADM_INIT_OPTIONS{
    /**  */
    ADM_INIT_OPTIONS_DEFAULT=0,
    /**  */
    ADM_INIT_OPTIONS_RESET_ALL,
    /**  */
    ADM_INIT_OPTIONS_RESET_IF_CHANGED
};


/**
\enum eADM_ERROR
*/
enum eADM_ERROR{
    /** Success */
    ADM_E_OK=0,
    /** Working */
    ADM_E_WORKING,
    /** Fail */
    ADM_E_KO
};

typedef enum eADM_ERROR ADM_ErrorType;


/** \enum */
typedef enum 
{
  ADM_PRESCAN_ERROR,
  ADM_INVALID_I2C_ADD,
  ADM_SF_PARSE_ERR,
  ADM_INVALID_DATATYPE,
  ADM_DDAT_SIZE_TOO_SMALL,
  ADM_FLASH_WRITE_ERR,
  ADM_DATAREQ_ERR,
  ADM_RAM_TOOSMALL_FOR_EXTRAP,
  ADM_DISPL_CALC_ERR,
  ADM_INVALID_POINTERTYPE_SIZE,
  ADM_IDENT_GET_ERR,
  ADM_INVALID_EXTRAP
  
}
ADM_ManagerErrorType;

typedef struct 
{
  uint8_t* pui8Data;
  uint16_t ui16DataSize;
}
ADMDataType;




/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

/** Exported API for init */
void ADM_SetInitOptions(ADM_InitOptionsType initOptionsValue);

/** Exported API for data access */
ADM_ErrorType ADM_GetData(uint8_t ui8Tag, ADMDataType* pstData);
ADM_ErrorType ADM_GetIdent(uint8_t ui8ApplI2CAdd,ADMDataType* pstData);
ADM_ErrorType ADM_BasicLoader(uint8_t ui8I2CAddr, uint8_t pointer, uint8_t displacement, ADMDataType* pstData);
ADM_ErrorType ADM_GetMainAddr(uint8_t* pui8ApplI2CAdd);
ADM_ErrorType ADM_GetApplianceStatus(uint8_t ui8ColdReset,Struct_Status *RefStatus);

ADM_LastUpdateIdType ADM_GetDynLastUpdateID(void);
uint16_t ADM_GetDynDataSize(void);
uint8_t* ADM_GetDynDataPnt(void);

/** Processes */
void ADM_ManagerInit(void);
ADM_ErrorType ADM_Manager(void);
void ADM_ReqManagerInit(void);
ADM_ErrorType ADM_RequestManager(void);
ADM_ErrorType ADM_EventManager(void);

/*status*/
bool_t ADM_IsADMManagerStateKO(void);
void ADM_SetADMManagerStateKO(void);

#endif /* __APPLDATAMNG_H */

/* *********** */
/* End of file */
/* *********** */


