/**

This file defines the callback in the skeleton for the application

The body of the function here must be implemented by the Application iven if the funcion is not used

\file    callback.h
\date    02/12/2004
\version 0.0.2
\author  Stefano Bonci Alberto Camerin
*/

/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


#ifndef __CALLBACK_H
#define __CALLBACK_H

#include "Config.h"

typedef struct SupervisorInfoStruct
{
    SysMode_Type    systemMode;
    bool_t          zcGood;
    uint8_t         tick;
}SupervisorInfoType;


/* Main */
void CALLBACK_Main_StartUp(void);
void CALLBACK_RemoteLoadsControl_BeforeProcess(void * pRemoteLoadsControlData, int *len);
void CALLBACK_RemoteLoadsControl_AfterProcess(void * pRemoteLoadsControlData, int *len);

/*Backup and restore*/
void CALLBACK_Main_ColdReset_BeforeRestore(void);
void CALLBACK_Main_ColdReset_OnRestoredData(void);
void CALLBACK_Main_ColdReset_OnNotRestoredData(void);
void CALLBACK_Main_ColdReset_AfterRestore(void);
void CALLBACK_Main_WarmReset(void);
bool_t CALLBACK_Main_BackupAreaInvalidationRequired(void);
void CALLBACK_Main_ApplicationTask_Body(void);
bool_t CALLBACK_Main_ApplicationTask_Ctrl(void);

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
void CALLBACK_Main_ApplBody_PostLoads(void);
#endif
  
/*Init*/
void CALLBACK_Main_InitTask(SysMode_Type sysmode, bool_t coldreset);

/*GP*/
void CALLBACK_Main_GP(SupervisorInfoType spInfo);
bool_t CALLBACK_EnableCheckColdResetTimeout(uint8_t SystemMode);


/* Viene eseguita in DigitalProtocol_CmdExecute.
Avvisa il Supervisor che e' stata richiesta dall'esterno una modalita'
speciale di funzionamento */
uint8_t CALLBACK_Protocol_SubModeManage(uint8_t submode);
void CALLBACK_Protocol_UIRemoteControlManage(uint8_t subMode);
/* chiamata in IO_Manager.c case _PROFILER */
void CBF_Fault_InserEVIndex(uint8_t _loadID, uint8_t _Index);

/* special operating modes */
void CBF_Task_Protocol_SetSpecialModeManager(uint8_t mode);
/* Chiamata in Analog.c */
void CBF_AnalogInputIntManager_Polling(void);
/* Chiamata nel task backup e utilizzata per comunicare alla 
   display una probabile mancanza rete */
void CBF_notifyBackupAlarm(void);
/* sincronizzazione UI-Main mediante la sequenza 
   READ_STATUS - READ_REGS - WRITE_REGS */
void CBF_OnReadStatusCompleted(void);
/* sincronizzazione UI-Main mediante la sequenza 
   READ_STATUS - READ_REGS - WRITE_REGS */
uint8_t CBF_OnReadRegulations(void);
/* sincronizzazione UI-Main mediante la sequenza 
   READ_STATUS - READ_REGS - WRITE_REGS */
uint8_t CBF_OnWriteRegulations(void);
/* sincronizzazione UI-Main mediante la sequenza 
   READ_STATUS - READ_REGS - WRITE_REGS */
void CBF_OnReadRegulationsCompleted(void);
/* Notifica aggiornamento UI Info */
void CBF_OnWriteUIInfoCompleted(void);

#if !defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
/* L'applicazione decide se pilotare il carico con la LIB_IOM_SetLoad() standard o 
con la LIB_IOM_GIOM_LoadPilot(). */
uint8_t CBF_GIOM_LoadPilot(uint8_t );

uint8_t CBF_SetAllLoaddOff(uint8_t loadID);
#endif /* */
/* call in OutputManager.c if TRUE switch off the load */
bool_t CBF_IOManager_IOVal_Ctrl(uint8_t loadID);

uint8_t CALLBACK_OnStatusRequest(void);
uint8_t CBF_NormalMode_InitTask_WarmReset(void);
uint8_t CBF_NormalMode_InitTask_ColdReset_RestoredData(void);
uint8_t CBF_NormalMode_InitTask_ColdReset_PostRestore(void);
uint8_t CBF_Task_Main_Application_Body0(void);
uint8_t CBF_Task_Main_Application_Body1(void);
uint8_t CBF_BackupAreaInvalidationReq(void);
#endif /* __CALLBACK_H */

/* *********** */
/* End of file */
/* *********** */

