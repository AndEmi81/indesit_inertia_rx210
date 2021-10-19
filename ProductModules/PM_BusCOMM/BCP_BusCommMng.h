/*
Copyright (c) 2004-2009, Indesit Company, I.D.D.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\par		Descrizione
\n		Communication Manager header file.
\n
\file		BCP_BusCommMng.h
\ingroup	CM
\date		20/04/2009
\version	01.00.00
\author	Gabriele Merlonghi
\n
\n
\par		History (titolo)
\n 		00.00.14
\n		23/10/2009
\n		Roberto Fioravanti
\n		changes
\n 		- dimensione dell'array PacketReq fissata in base alla propriet� BCM_REQ_NUM
\n
\n 		00.00.01
\n		05/05/2009
\n		Gabriele Merlonghi
\n		changes
\n 		- creation
*/




#ifndef __BUSCOMMMNG_H
#define __BUSCOMMMNG_H

/* ********** */
/* Includes */
/* ********* */
#include "BusCommPltf_cfg.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
  \addtogroup CRM
  @{
*/



/*@} addtogroup CRM*/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t   BCM_RequestType;








/* ****************** */
/* Exported variables */
/* ****************** */
extern CM_Err_ClassType CommErr[TOTAL_CHANNELS][BCP_COUNTERS_NUM];

/* ******************* */
/* Function prototypes */
/* ******************* */
/**
  \addtogroup CRM
  @{
*/

#if BCP_MASTER_SUPPORT == TRUE
void BCM_Init(IO_ChannelType idChannel);
void BCM_MasterDisable(IO_ChannelType idChannel);
void BCM_ForceRebuild(IO_ChannelType idChannel);
void BCM_SetWait(IO_ChannelType idChannel, uint8_t newWaitValue);
IO_ErrorType BCM_TestRequest(IO_ChannelType idChannel, BCM_RequestType idRequest);
void BCM_KillPacketRequest(IO_ChannelType idChannel, BCM_RequestType idRequest);
void BCM_RequestManager(uint8_t idChannel);
IO_ErrorType BCM_SetPacketParam(BCM_RequestType idRequest, void * pParam, IO_LenType len);
IO_ErrorType CRM_SetPacketRequestSync(IO_ChannelType ch, BCM_RequestType idRequest);
IO_ErrorType CRM_SetPacketParamSync(IO_ChannelType ch, BCM_RequestType idRequest, void * pParam, IO_LenType len);
IO_ErrorType CRM_IsPacketRequestSync(IO_ChannelType ch, BCM_RequestType idRequest);
#endif

/*@} addtogroup CRM*/




/**
  \addtogroup CM
  @{
*/

void BCP_BusMonitor(void);
void BCM_CountersUpdater(IO_ChannelType idChannel, IO_ErrorType idError);
IO_ErrorType BCP_InitChSync(IO_ChannelType ch);

void CP_SlaveManager__Read_ApplData(uint8_t pos, IO_MemPtrType pRxBuff, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen);
void CP_SlaveManager__Write_ApplData(uint8_t pos, IO_MemPtrType pRxBuff, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen);

int8_t PP_MappingGetOffset(IO_MemPtrType pStart, IO_MemPtrType pEnd, const PP_RemapTableType* pTable, int8_t size, unsigned long* pOffset);
uint16_t PP_MappingGetPPAddr(const IO_MemPtrType realAddr, const PP_RemapTableType* pTable, int8_t size);

void BCP_SlaveManager__Read_ApplData(IO_ChannelType ch, uint8_t* pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen);
void BCP_SlaveManager__Write_ApplData(IO_ChannelType ch, uint8_t* pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen);


void BCM_OnRequestCompleted(IO_ChannelType ch);

int8_t PP_IsAccessAllowed(const PP_MemoryRangeType* pTable, int8_t tablelength, IO_MemPtrType pStart, IO_LenType size);
/*@} addtogroup CM*/


#endif /* __BUSCOMMMNG_H */
/* *********** */
/* End of file */
/* *********** */
