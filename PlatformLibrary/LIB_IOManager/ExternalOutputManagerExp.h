/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		ExternalOutputManagerExp.h
\ingroup	ExtOutputsManager
\date		Sep 11, 2010 
\version	01.00.00 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:

\n 		00.00.65
\n		30/07/2010 
\n		Roberto Fioravanti
\n		changes 
\n 		- updated size for external variables
\n
\n 		00.00.55
\n		30/07/2010 
\n		Roberto Fioravanti
\n		changes 
\n 		- New PilotType FIXED_SPEED_MOTOR_L
\n
\n
\n etc.
*/

#ifndef __EXERNAL_OUTPUT_MANAGER_EXP_H
#define __EXERNAL_OUTPUT_MANAGER_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "OutputManagerExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)
#define IOM_EXTERNAL_0_GLOBALRAMBUFFER_SIZE		(IOM_EXTERNAL_0_RAMBUFFER_SIZE + 2/*seq id + toolid */)

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
#define IOM_EXTERNAL_0_GLOBALDATABUFFER_SIZE	IOM_EXTERNAL_0_GLOBAL_DATA_BLOCK_SZ
#endif
#endif


#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_1_ENABLED)
#define IOM_EXTERNAL_1_GLOBALRAMBUFFER_SIZE			(IOM_EXTERNAL_1_RAMBUFFER_SIZE + 2/*seq id + toolid */)

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
#define IOM_EXTERNAL_0_GLOBALDATABUFFER_SIZE	IOM_EXTERNAL_1_GLOBAL_DATA_BLOCK_SZ
#endif

#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

enum
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_0_ENABLED)
EXTERNAL_0,
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_0_ENABLED)*/
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_1_ENABLED)
EXTERNAL_1,
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_1_ENABLED)*/
EXTERNAL_NUM
};


/* ****************** */
/* Exported variables */
/* ****************** */
#if !defined(MODULE_BODY_LIB_IOMANAGER)
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)
extern const uint8_t	IOM_CommandBufferExternal_0[IOM_EXTERNAL_0_COMMANDBUFFER_SIZE];
extern const uint8_t	IOM_ReadRamExternal_0[IOM_EXTERNAL_0_GLOBALRAMBUFFER_SIZE];

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
extern const uint8_t 	ExternalGlobalData_0[IOM_EXTERNAL_0_GLOBALDATABUFFER_SIZE];
#endif /* defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT) */

#endif
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_1_ENABLED)
extern const uint8_t IOM_CommandBufferExternal_1[IOM_EXTERNAL_1_COMMANDBUFFER_SIZE];
extern const uint8_t	IOM_ReadRamExternal_1[IOM_EXTERNAL_1_GLOBALRAMBUFFER_SIZE];

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
extern const uint8_t 	ExternalGlobalData_1[IOM_EXTERNAL_1_GLOBALDATABUFFER_SIZE];
#endif /* defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT) */

#endif
#endif /* !defined(MODULE_BODY_LIB_IOMANAGER) */
/* ******************* */
/* Function prototypes */
/* ******************* */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
void LIB_IOM_External_Init(void);


LIB_IOM_ErrorType LIB_IOM_External_New( uint8_t* pInfoBuffer, uint8_t szInfoBuffer,  uint8_t loadPos);
bool_t LIB_IOM_External_GetInfoBuffer(uint8_t extLoadItem, uint8_t *pByteData, uint8_t szData);
bool_t LIB_IOM_Ext_GetCommonBuffer(uint8_t commCh, uint8_t *pCmnData, uint8_t szData);
uint8_t LIB_IOM_Ext_GetExtdLoadComChan(uint8_t extloadItem);
uint8_t LIB_IOM_Ext_GetLoadComChan(uint8_t loadPos);
uint8_t LIB_IOM_Ext_GetBoardCounter(void);

void	LIB_IOM_External_ProcessInfoPacketData(uint8_t ch, uint8_t* pInfoPacketData, uint8_t szInfoPacketData);
LIB_IOM_ErrorType LIB_IOM_External_GetCommandBuffer(uint8_t uart, uint8_t** ppCmd, uint8_t* pCmdLen);

LIB_IOM_ErrorType LIB_IOM_External_SetReadRamRequest(uint8_t * pDataRamReq, uint8_t len);
LIB_IOM_ErrorType LIB_IOM_External_SetWriteRamRequest(uint8_t * pDataRamReq, uint8_t len);
LIB_IOM_ErrorType LIB_IOM_External_SetExtraRequest(uint8_t * pDataExtraReq, uint8_t len);

uint8_t LIB_IOM_External_GetExternalOutMngError(void);
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)*/
#endif /* __EXERNAL_OUTPUT_MANAGER_EXP_H */

/* *********** */
/* End of file */
/* *********** */

