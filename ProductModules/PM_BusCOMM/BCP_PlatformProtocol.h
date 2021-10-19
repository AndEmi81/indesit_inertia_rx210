/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Description
\n		Platform Protocol library interface
\n
\file		BCP_PlatformProtocol.h
\ingroup	PP
\date		28/04/2010
\version	00.00.43
\author	Roberto Fioravanti
\n
\n
\par		History (titolo)
\n
\n 		00.00.59
\n		08/09/2010
\n		Roberto Fioravanti
\n		changes
\n    	- Prototype of PP_EncodeDataGPAnsParam changed (2nd arg to IO_BufferSizeType)
\n
\n 		00.00.41
\n		08/10/2009
\n		Roberto Fioravanti
\n		changes
\n    	- included files updated
\n
\n 		00.00.41
\n		08/10/2009
\n		Roberto Fioravanti
\n		changes
\n    	- addiction of prototypes PP_EncodeHdrAnsBridgeParam, PP_EncodeDataBootAnsParam, PP_EncodeDataPPAnsParam
\n
\n 		00.00.12
\n		08/10/2009
\n		Roberto Fioravanti
\n		changes
\n    	- PP_SlaveResponseBuilder returns void
\n
\n 		00.00.02
\n		05/05/2009
\n		Roberto Fioravanti
\n		changes
\n 		- creation
\n		- comments
\n
\n
\n
\n etc.
*/


#ifndef __PLATFORMPROTOCOL_H
#define __PLATFORMPROTOCOL_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "StandardDefines.h"

#include "Hal_Interface.h"
#include  HAL_CFG
#include "io_driver.h"




/* ****************** */
/* Defines and Macros */
/* ****************** */


/**
\todo description
General purpose request macros
*/
#define PP_HDR_FROM_GP_BRIDGE_MSK           (PP_HDR_CRC)
#define PP_HDR_TO_GP_BRIDGE_MSK             (PP_HDR_TYP_16)

#define PP_CMD_ID_FROM_GP_PARAM(x)          (((CP_GP_ParamType*)(x))->ZBParam.Hdr.HdrByte.CommandID)
#define I2C_ADDRESS_FROM_GP_PARAM(x)        (((CP_GP_ParamType*)(x))->ZBParam.PL.PlByte.I2cAddress)
#define PP_HDR_FROM_GP_PARAM(x)             ((((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[1]) & PP_HDR_FROM_GP_BRIDGE_MSK)


#define PP_TYPE_FROM_GP_PARAM(x)            (((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[2])
#define PP_LEN_FROM_GP_PARAM(x)             (((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[3])
#define PP_PDATA_FROM_GP_PARAM(x)           ((uint8_t*)&(((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[4]))

#define PP_BS_TYPE_FROM_GP_PARAM(x)         (((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[4])
#define PP_BS_PDATA_FROM_GP_PARAM(x)        (1+(uint8_t*)&(((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[4]))


#define PP_HDR_TO_GP_PARAM(x, val)          (*(((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[0])= val | PP_HDR_TO_GP_BRIDGE_MSK)




/**
	\defgroup PR_PARAM_TYPE	Packet Request Param Type
	\ingroup PP
	\addtogroup PR_PARAM_TYPE
  	@{
*/

enum eREQPARAMTYPE
{
  PPM_REQPARAMTYPE_EXTENDED=0,	/**< default */
  PPM_REQPARAMTYPE_GP,			/**< general purpose format */
  PPM_REQPARAMTYPE_BOOT			/**< bootloader format */
};
/*	@}*/


/* bit per le opzioni PP slave */
enum PP_SLAVE_RESPONSE_OPT{
PP_SL_RESP_ANSW_OPT_BIT=0        /* La richiesta prevede la risposta */
};

/** Opzione: richiesta con risposta */
#define PP_SL_RESP_ANSW_OPT     (BCP_OptionType)(1<<(uint8_t)PP_SL_RESP_ANSW_OPT_BIT)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Callback for Slave manager
\ref bcp_options_callback*/
typedef void (* BCP_ActionCB)(IO_ChannelType, IO_NotificationType);


typedef struct PP_Board_tag
{
  IO_ErrorType    esit;
  IO_ChannelType  idReq;
}PP_BoardStateType;


/*
  \addtogroup PPS
  @{
*/

/** Struttura per il passaggio dei parametri della lettura ram */
typedef struct
{
  PP_AddressType  addr;   /**< indirizzo ram */
  IO_LenType len;         /**< numero di byte da leggere */
} PP_RRamParamType;
/*@}*/

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/**
  \addtogroup PPM
  @{
*/
IO_ErrorType PP_InitChSync(IO_ChannelType ch);
IO_ErrorType PP_MasterPrepareRequest(IO_ChannelType indexReq, IO_ChannelType channel, CommDriverParamType *param);
IO_BufferSizeType PP_PrepareRequest(IO_BufferType txBuffer, const PP_MasterProtocolTableType * info);
IO_BufferSizeType PP_GetRequestLen(IO_BufferType txBuffer, const PP_MasterProtocolTableType * info);
IO_ErrorType PP_StartDriverRequestSync(IO_ChannelType channel, CommDriverParamType *param);
IO_ErrorType PP_MasterSetParamRequest(IO_ChannelType indexReq, IO_ChannelType idBoard, IO_MemPtrType pData, IO_LenType len);
IO_ErrorType PP_GetEsit(IO_ChannelType ch);
I2C_SlaveAddressType PP_GetSlaveAddress(IO_ChannelType idBoard, const PP_MasterProtocolTableType *info);
void PP_EncodeDataBootAnsParam(IO_ChannelType idx, uint8_t* pData);
void PP_EncodeDataGPAnsParam(PP_MasterProtocolTableType* pGpReq, IO_BufferSizeType u8Len, uint8_t* pData);
void PP_EncodeHdrGPAnsParam(PP_MasterProtocolTableType* pGpReq, uint8_t ackval);
IO_ErrorType PP_MasterSetParamRequestSync(IO_ChannelType channel, IO_ChannelType indexReq, IO_MemPtrType pData, IO_LenType len);
void PP_SetMasterError(IO_ChannelType ch, IO_ErrorType error);
IO_ErrorType PP_MasterValidateAnswer (IO_ChannelType channel);
void PP_OnMasterRxCompleted(IO_ChannelType ch);
/*@}*/

/*
  \addtogroup PPS
  @{
*/
void   				PP_SlaveResponseBuilder (IO_ChannelType ch, PP_SlaveBuilderInfoType info);
IO_ErrorType        PP_SlaveStartResponse(IO_ChannelType ch);
IO_ValueType		PP_GetSlaveState(IO_ChannelType ch);
bool_t		 		PP_IsSlaveStateRxOK(IO_ChannelType ch);
IO_ErrorType		PP_SlaveNotification(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error);
BCP_BufferSizeType	PP_Typ16ToBridgeConv(uint8_t seq_id, uint8_t* pInMem, uint8_t* pOutMem );
BCP_BufferSizeType	PP_BridgeToTyp16Conv(uint8_t* in, uint8_t* out);
/*@}*/

#endif /* __PLATFORMPROTOCOL_H */

/* *********** */
/* End of file */
/* *********** */
