/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**

  \defgroup	PP Platform Protocol
  \ingroup PM_BusCOMM

  \par		Description
  \n    Platform Protocol implements protocol related fetaures:
  \li   encode and decode of data,
  \li   communication driver management.

  \n    E' organizzato in due sezioni Master e Slave.
*/

/**
  \defgroup PPM Platform Protocol Master
  \ingroup PP
Platform Protocol master
  \par    Description
  \n      Descrizione del Platform Protocol master
*/

/**
\defgroup	PPS Platform Protocol Slave
\ingroup PP
Platform Protocol slave
\par		Description
\n      Descrizione del Platform Protocol Slave
*/






/**
\n
\par		Description
\n	  Platform Protocol Implementation
\n    driver management for master and slave communication
\n
\file		BCP_PlatformProtocol.c
\ingroup	PM_BusCOMM
\date		20/04/2009
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par		History
\n
\n 		00.00.59
\n		08/09/2010
\n		Roberto Fioravanti
\n		changes
\n    	- Prototype of PP_EncodeDataGPAnsParam changed (2nd arg to IO_BufferSizeType)
\n		- PP_EncodeDataGPAnsParam removed u8Len check
\n 		- bug fix: PP_EncodeHdrAnsBridgeParam: avoid overwriting type
\n		- bug fix: PP_BridgeToTyp16Conv manages Basic types
\n
\n    00.00.41
\n		20/04/2010
\n		Roberto Fioravanti
\n		changes
\n 		- INDESIT_BRIDGE_ZIGBEE becames CP_GENERAL_PURPOSE
\n		- removed crc_calculate
\n
\n    00.00.39
\n		15/04/2010
\n		Roberto Fioravanti
\n		changes
\n 		- bug fix: BCP_MasterResponseStateMachine updates esit when answer data is available
\n
\n    00.00.23
\n		23/10/2009
\n		Roberto Fioravanti
\n		changes
\n 		- deprecated removed
\n		- fragmentation
\n
\n    00.00.14
\n		23/10/2009
\n		Roberto Fioravanti
\n		changes
\n 		- data format: pDataRes in PP_MasterProtocolTableType contains data size in bytes (IO_BufferSizeType)
\n		- BCP_MasterResponseStateMachine: updated with the define PP_OUT_BUFF_RESERVED_SZ and PP_ZB_OUT_BUFF_RESERVED_SZ
\n
\n 		00.00.12
\n		08/10/2009
\n		Roberto Fioravanti
\n		changes
\n 		- PP_SlaveNotification added: IO_N_COMPLETED notifications
\n		- updating .esit field in PP_Ntwk for bridge zigbee packets without answer
\n		- ack error couter updating (for bridge zigbee packets witout answer)
\n    - PP_SlaveResponseBuilder returns void
\n
\n 		00.00.10
\n		18/09/2009
\n		Roberto Fioravanti
\n		changes
\n 		- BCP_MasterResponseStateMachine: zigbee packets added
\n
\n 		00.00.09
\n		15/09/2009
\n		Roberto Fioravanti
\n		changes
\n		- Optimizations: BCP_MasterResponseStateMachine
\n
\n 		00.00.08
\n		10/09/2009
\n		Roberto Fioravanti
\n		changes
\n      - bug fix: PP_BS_TYPE_FROM_ZB_PARAM(x) PP_BS_PDATA_FROM_ZB_PARAM(x)
\n
\n 		00.00.01
\n		05/05/2009
\n		Roberto Fioravanti
\n		changes
\n 		- creazione
\n		- 'builder', 'decoder' master and slave,  driver interface for transmission and reception.
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */

#include <string.h>

#include "BCP_BusCommMng.h"

#include "LIB_Utilities.h"

#ifdef BUS_COM_PLT_I2C_SUPPORT
#include "OCI2C_drv.h"
#endif /* BUS_COM_PLT_I2C_SUPPORT */

#ifdef CONFIG_PL_DRV_OCCRC
#include "OCCRC_drv.h"
#define BCP_CRC_calculate OCCRC_Calculate
#else
#include "SWCRC_drv.h"
#define BCP_CRC_calculate SWCRC_Calculate
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
  \ingroup PPS_SLAVE_PRIV
  Definizioni per state machine PP slave (richiesta).
*/

#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
/* Parser */
enum{
  /*Basic frame Rules*/
  PP_READ_BS_HDR_RULE_LBL = 0,
  PP_NO_MATCH_STX_RULE_LBL,
  PP_NO_MATCH_EE_RULE_LBL,

  /* Extended frame */
  PP_READ_EE_UNTIL_LEN_RULE_LBL,
  PP_MODLEN_BY_TYP01_RULE_LBL,
  PP_MODLEN_BY_CRC_RULE_LBL,
  PP_TEST_HDR_LEN,
  PP_SETLEN_TYP0_RULE_LBL,
  PP_SETLEN_TYP01_RULE_LBL,
  PP_EE_READ_ALL_RULE_LBL,
  PP_EE_CRC_TEST_RULE_LBL,


  /*Common Rules */
  PP_CHK_VER_RULE_LBL,
  PP_CRC_VER_RULE_LBL,
  PP_READ_BS_REM_RULE_LBL,

  PP_INVALID_RULE_LBL,
  PP_SLAVE_RX_OK,

  PP_RULE_NUM
};
#else

enum ePP_SLAVE_STATE{
PP_SLAVE_RX_HDR_START,          /**<  \hideinitializer starting receiving header */
PP_SLAVE_RX_ALL_START,          /**<  \hideinitializer Receive all frame */
PP_SLAVE_RX_ALL_COMPLETED,      /**<  \hideinitializer all frame received */
PP_SLAVE_RX_OK,                 /**<  \hideinitializer frame ok */
PP_SLAVE_RX_ERR                 /**<  \hideinitializer frame ko (chk o crc error),
                                      \note the cause of error is not available */
};
#endif


/** master Platform Protocol  state machine (Answer) */
enum ePP_MASTER_RESPONSE_STATE{
PP_RESPONSE_GET_ACK,
PP_RESPONSE_GET_HDR,
PP_RESPONSE_COMPLETION,
PP_RESPONSE_PROCESS,
PP_END
};

enum eBOOT_MASTER_RESPONSE_STATE{
BOOT_RESPONSE_GET_ACK,
BOOT_RESPONSE_GET_LEN,
BOOT_GET_DATA,
BOOT_END_DATA,
BOOT_END_ACK,
/*PP_RESPONSE_GET_HDR,
PP_RESPONSE_COMPLETION,
PP_RESPONSE_PROCESS,*/
BOOT_END
};

/**
\todo description
General purpose request macros
*/
/*
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
*/




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/**
 * Platform Protocol Master class.
 */
#if BCP_MASTER_SUPPORT == TRUE
typedef struct
{
  uint8_t                   state;						/**< Stato della macchina a stati */
  IO_ChannelType            indexRequest;     /**< indice PP_MasterRequestTable */
  IO_ChannelType            chBoard;          /**< indice board */
  BCP_PacketSizeType        bcount;           /**< numero di byte */
  BCP_PacketSizeType        lastRequestLen;   /**< numero di byte relativi all'ultima richiesta */
  IO_ErrorType              error;
}BCP_MasterPlatformProtocolType;
#endif


/**
	\ingroup PPS
 	Platform Protocol Slave class.
*/
#if BCP_SLAVE_SUPPORT == TRUE
typedef struct
{
	IO_MemPtrType				pdata;				/* builder: pnt to data */
	BCP_PacketSizeType        	dataCount;        	/* global current request bytes number */
	BCP_PacketSizeType        	dataCountLast;    	/* partial current request bytes number*/
	BCP_PacketSizeType        	answerCount;      	/* answer bytes number */
	uint16_t 				  	crc;				/* crc */
	IO_ValueType              	state;            	/* rx state machine*/
	uint8_t						option;				/* options */
}PP_SlavePlatformProtocolClassType;
#endif

typedef struct BCP_Builder_tag
{
  IO_BufferType            data;
  IO_BufferSizeType        datalen;
  BCP_PacketCodeType        code;
  BCP_PacketOptionType      option;   /* chk/crc */
}BCP_BuilderType;




/* ********* */
/* Variables */
/* ********* */




/* Exported */

/* Static */

/*Parser*/
#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
const PP_ParserType PP_Parser[PP_RULE_NUM]= {
  /* Basic frame */
  {PP_READ_CMD, 3, 0, 0 },
  {PP_NO_MATCH_CMD, PP_STX_INDEX, PP_STX, PP_INVALID_RULE_LBL},
  {PP_NO_MATCH_CMD, PP_FTYP_INDEX, PP_FTYP_EE, PP_READ_BS_REM_RULE_LBL},


  /* Extended frame */
  {PP_READ_CMD, 3, 0, 0},
  {PP_MODLEN_CMD, PP_HDR_INDEX, 2, 1},
  {PP_MODLEN_CMD, PP_HDR_INDEX, 1, 1},

  {PP_AND_CMD,  PP_HDR_INDEX, 2, PP_SETLEN_TYP01_RULE_LBL},
  {PP_SETLEN_CMD, PP_LEN_INDEX, 0, PP_EE_READ_ALL_RULE_LBL},
  {PP_SETLEN_CMD, PP_GLEN_INDEX, 0, PP_EE_READ_ALL_RULE_LBL},

  {PP_READ_ALL_CMD, 0, IO_CLOSE, 0},

  {PP_AND_CMD,  PP_HDR_INDEX, 1, PP_CRC_VER_RULE_LBL},
  /* common Rules*/
  {PP_CHK_CHECK_CMD, 0, 0, PP_SLAVE_RX_OK},
  {PP_CRC_CHECK_CMD, 0, 0, PP_SLAVE_RX_OK},
  {PP_READ_CMD, 3, IO_CLOSE, PP_CHK_VER_RULE_LBL},

  {PP_INVALID_CMD, 0, 0 ,0},
  {PP_RX_OK_CMD, 0, 0 ,0}
};
#endif


/**
  \addtogroup PPM
  @{
*/
#if BCP_MASTER_SUPPORT == TRUE
static BCP_MasterPlatformProtocolType MasterPlatformProtocol[BCP_CH_NUM];
#endif
/* @} */



/* Stato Platform Protocol Slave */
#if (BCP_SLAVE_SUPPORT == TRUE)
static PP_SlavePlatformProtocolClassType   PP_SlavePlatformProtocol[BCP_CH_NUM];
BCP_PacketSizeType PP_len;
#endif /* (BCP_SLAVE_SUPPORT == TRUE) */



#if BCP_MASTER_SUPPORT == TRUE
static PP_BoardStateType PP_Ntwk[BOARD_NUM];
#endif

/* ******************* */
/* Functions Prototype */
/* ******************* */

static void crc_append (IO_BufferSizeType  len, IO_BufferType pdata);
static void chk_append (IO_BufferSizeType  len, IO_BufferType pdata);


#if (BCP_MASTER_SUPPORT == TRUE)
static IO_ErrorType BCP_MasterNotify (IO_NotificationType notifVal,IO_ChannelType channel, IO_ErrorType error);
static IO_BufferSizeType  BCP_MasterBuilder (IO_BufferType txBuffer, PP_MasterProtocolTableType info);
static void BCP_MasterResponseStateMachine(IO_ChannelType channel, IO_ErrorType error);
#if defined(CP_BOOTLOADER)
static void BCP_BootLoaderResponseStateMachine(IO_ChannelType channel);
#endif

IO_ErrorType BCP_MasterStartResponce(  IO_ChannelType channel, const BCP_MasterType * master);
void PP_MasterValidateEEAnswer(IO_ChannelType channel, PP_MasterProtocolTableType* pProt);
#endif /* (BCP_MASTER_SUPPORT == TRUE) */

#if (BCP_SLAVE_SUPPORT == TRUE)

#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
static IO_ErrorType PP_SlaveRxInterpreter(IO_ChannelType channel);
#else
static IO_ErrorType PP_SlaveReceiverStateMachine(IO_ChannelType chid);
#endif

static IO_ErrorType PP_SlaveReceiveData(IO_ChannelType chid, BCP_PacketSizeType * pLen, BCP_OptionType option);
static void PP_SlaveFragmentBuilder(IO_ChannelType ch, uint8_t chk_displ, uint8_t data_displ);



#endif /* (BCP_SLAVE_SUPPORT == TRUE) */








/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/* Platform Protocol Slave */

/**
This method receives notification on events durig slave commmunication

\param [in]   notifVal notification codes:

\li #IO_N_READ_DATA_INIT
Initialization state machine.
\li #IO_N_WRITE_DATA_INIT
calls the callback defined by
\li #IO_N_TRANSFER_DONE
transfer is done
\li #IO_N_ERROR
ignored

\param [in]   chid channel index (sender of the notification)
\param [in]   error code error (ignorato)


\Example
This method is used as callback
\code
	IO_PhyInitSyncType i2cParam={0};

	i2cParam.eventCB=PP_SlaveNotification;
  i2cParam.id=BCP_I2C_CH_ID;

	OCI2C_InitSync(&i2cParam);
\endcode

\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\public \memberof PP_SlavePlatformProtocolClassType
*/
IO_ErrorType PP_SlaveNotification(IO_NotificationType notifVal, IO_ChannelType chid, IO_ErrorType error)
{
	IO_ErrorType result=(IO_ErrorType)IO_E_OK;
	(void) error;

#if BCP_MASTER_SUPPORT == TRUE
	BCM_ForceRebuild(chid);
	BCM_SetWait(chid, 0xff);
#endif

	switch(notifVal)
	{
    	case IO_N_READ_DATA_INIT:
#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
    		PP_SlavePlatformProtocol[chid].state= (IO_ValueType)0;
#else
    		PP_SlavePlatformProtocol[chid].state= (IO_ValueType)PP_SLAVE_RX_HDR_START;
#endif
    		PP_SlavePlatformProtocol[chid].dataCount=0;
    		break;

    	case IO_N_WRITE_DATA_INIT:
      /* call appl callback */
#ifdef BCM_SLAVE_ACTION_CB
    		if(PP_SlavePlatformProtocol[chid].dataCount)
    		{
    			(BCM_SLAVE_ACTION_CB)(chid, (IO_NotificationType)IO_N_ACTION);
    		}
#endif
			break;

    	case IO_N_TRANSFER_DONE:
#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
      result=PP_SlaveRxInterpreter(chid);
#else
			result=PP_SlaveReceiverStateMachine(chid);
#endif
			break;

#ifdef BCP_FRAGMENT_SLAVE
		case IO_N_TX_DONE:
			PP_SlavePlatformProtocol[chid].answerCount=0;
			PP_SlaveFragmentBuilder(chid, 0, 0);
			(void)PP_SlaveStartResponse(chid);
			break;
#endif

		case IO_N_COMPLETED:
		{
#if (ENABLE_BCP_EOSN==TRUE)
				PP_EosnFunction(chid);
#endif
#if BCP_MASTER_SUPPORT == TRUE
    		BCM_SetWait(chid, 0);
#endif
		}
			break;

		case IO_N_ERROR:
			PP_SlavePlatformProtocol[chid].dataCount=64;
#if BCP_MASTER_SUPPORT == TRUE
    		BCM_SetWait(chid, 0);
#endif
			break;

		default:
			break;
	}
	return result;
}

/**
Returns the state of the specified channel

\param [in]   ch channel id
\return     IO_ValueType
\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/
IO_ValueType PP_GetSlaveState(IO_ChannelType ch)
{
	return PP_SlavePlatformProtocol[ch].state;
}

/**
Returns if the protocol level of the specified channel has received correctly the request

\param [in]   ch channel id
\return     bool_t
\retval     TRUE, if the request has been receved correctly

\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/
bool_t PP_IsSlaveStateRxOK(IO_ChannelType ch)
{
#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
	return (PP_SlavePlatformProtocol[ch].state ==(IO_ValueType)PP_SLAVE_RX_OK);
#else
	IO_ErrorType err;
	uint16_t crc = 0xFFFF;

	if(BCP_ChannelTable[ch].rxBuffer[PP_STX_INDEX]!= PP_STX)
	{
		err = (IO_ErrorType)IO_E_CHECK_PP;
		return (bool_t)(err == (IO_ErrorType)IO_E_OK);
	}
	if(BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX]==PP_FTYP_EE)
	{
		if(BCP_ChannelTable[ch].rxBuffer[PP_HDR_INDEX] & PP_HDR_CRC)
		{
			crc = BCP_CRC_calculate(PP_SlavePlatformProtocol[ch].dataCount, BCP_ChannelTable[ch].rxBuffer, (uint16_t) 0xFFFF);
		} else
		{
			crc = LIB_ChkComputation((uint16_t)(PP_SlavePlatformProtocol[ch].dataCount-1), BCP_ChannelTable[ch].rxBuffer,(uint8_t) 0);
			crc &= (uint16_t)0x00FF;
			crc ^= BCP_ChannelTable[ch].rxBuffer[PP_SlavePlatformProtocol[ch].dataCount-1];
		}
	}
	else
	{
		if(PP_SlavePlatformProtocol[ch].dataCount == 6)
		{
			crc = LIB_ChkComputation((uint16_t)(PP_SlavePlatformProtocol[ch].dataCount - 1), BCP_ChannelTable[ch].rxBuffer,(uint8_t) 0);
			crc &= (uint16_t)0x00FF;
			crc ^= BCP_ChannelTable[ch].rxBuffer[PP_SlavePlatformProtocol[ch].dataCount-1];
		}
		else
		{
			err = (IO_ErrorType)IO_E_CHECK_PP;
			return (bool_t)(err == (IO_ErrorType)IO_E_OK);
		}
	}

	if(crc==0)
	{
		/* ok!!! */
		BCM_CountersUpdater(ch, (IO_ErrorType) BCP_PACKET_NUM_NOERR_CNT);
		err = (IO_ErrorType)IO_E_OK;
	} else
	{
		BCM_CountersUpdater(ch, (IO_ErrorType) BCP_PP_CORRUPTED_PKT_ERR_CNT);
		PP_SlavePlatformProtocol[ch].state++;
		err = (IO_ErrorType)IO_E_CHECK_PP;
	}

	return (bool_t) (err == (IO_ErrorType) IO_E_OK);
#endif
}


/**
Starts answer as slave.

\param [in]   ch Identificatore del canale di comunicazione.

\return     IO_ErrorType
\retval     IO_E_INVALID_VALUE: buffer o len non validi
            IO_E_BUSY: bus occupato (solo master)
            IO_E_OK: trasmissione avviata regolarmente.
\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/
IO_ErrorType PP_SlaveStartResponse(IO_ChannelType ch)
{

  CommDriverParamType requestParam={0};

  requestParam.len = PP_SlavePlatformProtocol[ch].answerCount;
	if(!PP_SlavePlatformProtocol[ch].dataCount)
	{
  	    requestParam.option |= IO_CLOSE_TX; /* last fragment */
	}
  requestParam.buffer=BCP_ChannelTable[ch].txBuffer;
  
  return ((IO_PhyCommunicationSync)(BCP_ChannelTable[ch].StartTxSync))(&requestParam);
}




#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType PP_InitChSync(IO_ChannelType ch)
{

  if(ch>=BCP_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  /*TODO : only for allocated address*/
  /* PP_Ntwk[ch].esit=IO_E_OK; */
  /* PP_Ntwk[ch].idReq=0xFF; */

  return (IO_ErrorType)IO_E_OK;
}
#endif

/**
Returns the result of the last request.

\param [in]   ch  Master Protocol channel

\return     IO_ErrorType
\retval   	IO_E_OK: successfully
\retval   	TODO : list other possible value
\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType PP_GetEsit(IO_ChannelType ch)
{
  return MasterPlatformProtocol[ch].error;
}
#endif

/**
Imposta i paramentri relativi ad una richiesta.

\param [in]   indexReq Indice dellla richiesta
\param [in]   idBoard  Indice del destinatario
\param [in]   pData Puntatore ai dati
\param [in]   len  dimensione in byte dei dati

\return     IO_ErrorType
\retval   IO_E_OK: successo
\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType PP_MasterSetParamRequest(IO_ChannelType indexReq, IO_ChannelType idBoard, IO_MemPtrType pData, IO_LenType len)
{
  IO_ChannelType channel=PP_NtwkTable[idBoard].drvCh;

  if(indexReq>PP_REQ_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(len>*BCP_ChannelTable[channel].reqTable[indexReq].pLenReq)
  {
    return (IO_ErrorType)IO_E_INVALID_SIZE;
  }

  switch(BCP_ChannelTable[channel].reqTable[indexReq].code)
  {
    case 0x3F:
    {
      PP_AddressType addr;
      addr=((PP_RRamParamType*)pData)->addr;
      *(BCP_ChannelTable[channel].reqTable[indexReq].pDataReq)=(uint8_t)(addr.data & 0xFF);
      *(BCP_ChannelTable[channel].reqTable[indexReq].pDataReq+1)=(uint8_t)((addr.data >> 8) & 0xFF);
      if( ( ((PP_RRamParamType*)pData)->len == 0) || (((PP_RRamParamType*)pData)->len >= 0x0100) )
      {
        ((PP_RRamParamType*)pData)->len = 0x0100;
      }

      addr.data+=((PP_RRamParamType*)pData)->len-1;

      *(BCP_ChannelTable[channel].reqTable[indexReq].pDataReq+2)=(uint8_t)(addr.data & 0xFF);
      *(BCP_ChannelTable[channel].reqTable[indexReq].pDataReq+3)=(uint8_t)((addr.data >> 8) & 0xFF);

    }
      break;

    default:
        {
	        IO_BufferType  ptr;
	        ptr = BCP_ChannelTable[channel].reqTable[indexReq].pDataReq;
	        (void)memcpy(ptr, pData,(size_t) len);
        }
  }


  return (IO_ErrorType)IO_E_OK;
}
#endif


/**
Imposta i paramentri relativi ad una richiesta.

\param [in]   channel Channel index
\param [in]   indexReq  Request index
\param [in]   pData pointer to data
\param [in]   len  size in bytes of data

\return     IO_ErrorType
\retval   IO_E_OK: successo
\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType PP_MasterSetParamRequestSync(IO_ChannelType channel, IO_ChannelType indexReq, IO_MemPtrType pData, IO_LenType len)
{

  PP_MasterProtocolTableType* pMstProtTbl= &BCP_ChannelTable[channel].reqTable[indexReq];

  if(indexReq>PP_REQ_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(len>* pMstProtTbl->pLenReq)
  {
    return (IO_ErrorType)IO_E_INVALID_SIZE;
  }

  switch(pMstProtTbl->code)
  {
    case 0x3F:
    {
      PP_AddressType addr;
      addr=((PP_RRamParamType*)pData)->addr;
      *(pMstProtTbl->pDataReq)=(uint8_t)(addr.data & 0xFF);
      *(pMstProtTbl->pDataReq+1)=(uint8_t)((addr.data >> 8) & 0xFF);
      if( (((PP_RRamParamType*)pData)->len==0) || (((PP_RRamParamType*)pData)->len>= 0x0100) )
      {
        ((PP_RRamParamType*)pData)->len=0x0100;
      }

      addr.data+=((PP_RRamParamType*)pData)->len-1;

      *(pMstProtTbl->pDataReq+2)=(uint8_t)(addr.data & 0xFF);
      *(pMstProtTbl->pDataReq+3)=(uint8_t)((addr.data >> 8) & 0xFF);

    }
      break;

    default:
        {
	        IO_BufferType  ptr;
	        ptr = pMstProtTbl->pDataReq;
	        (void)memcpy(ptr, pData,(size_t) len);
        }
  }


  return (IO_ErrorType)IO_E_OK;
}
#endif


IO_BufferSizeType PP_GetRequestLen(IO_BufferType txBuffer, const PP_MasterProtocolTableType * info)
{
	IO_BufferSizeType retVal = (IO_BufferSizeType)(PP_LEN_INDEX+1+*info->pLenReq+((info->option&PPM_OPTION_CRC)?2:1));

#if !defined(CP_GENERAL_PURPOSE)
    (void) txBuffer;
#endif

#if defined(CP_BOOTLOADER) || defined(CP_GENERAL_PURPOSE)

	switch(info->paramType)
	{
#if defined(CP_BOOTLOADER)
		case PPM_REQPARAMTYPE_BOOT:
			retVal = (IO_BufferSizeType)PP_LEN_FROM_GP_PARAM(info->pDataReq);
		/*lint -e{527} Warning 527: */
		break;
#endif

	#if defined(CP_GENERAL_PURPOSE)

	    case PPM_REQPARAMTYPE_GP:
	      switch(PP_CMD_ID_FROM_GP_PARAM(info->pDataReq))
	      {
	        case PP_CMD_EXTENDED_WITH_ANSW:
	        case PP_CMD_EXTENDED_NO_ANSW:

	        	retVal = (IO_BufferSizeType)(PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX)+((*(txBuffer + PP_HDR_INDEX) & PP_HDR_CRC)?2:1));

			break;

	        case PP_CMD_BASIC:
	        	retVal = 6;
	        break;
	      }
	    break;
	#endif

	  default:
		break;

	}
#endif

	  return retVal;
}

#if BCP_MASTER_SUPPORT == TRUE
IO_BufferSizeType PP_PrepareRequest(IO_BufferType txBuffer, const PP_MasterProtocolTableType * info)
{

	IO_BufferSizeType packetLen = 0;

	/* data to be copied parameters */
	IO_BufferType      pData=0;						/*TODO to delete buffer */
	IO_BufferSizeType  len=0;							/*TODO to delete len */
	IO_BufferSizeType  s_index=0, d_index=0;			/*TODO to delete source and destination index */

	IO_BufferSizeType  ed_index=0;					/* number of data on which crc or chr will be calculated */


	switch(info->paramType)
	{
#if defined(CP_BOOTLOADER)
		case PPM_REQPARAMTYPE_BOOT:
			packetLen=(IO_BufferSizeType)PP_LEN_FROM_GP_PARAM(info->pDataReq);
			if(info->pDataReq)
			{
				(void)memcpy(txBuffer, PP_PDATA_FROM_GP_PARAM(info->pDataReq), PP_LEN_FROM_GP_PARAM(info->pDataReq));
			}
		break;
#endif

	#if defined(CP_GENERAL_PURPOSE)

	    case PPM_REQPARAMTYPE_GP:
	      switch(PP_CMD_ID_FROM_GP_PARAM(info->pDataReq))
	      {
	        case PP_CMD_EXTENDED_WITH_ANSW:
	        case PP_CMD_EXTENDED_NO_ANSW:
				*(txBuffer + PP_STX_INDEX) = PP_STX;
				*(txBuffer + PP_FTYP_INDEX) = PP_FTYP_EE;
				*(txBuffer + PP_HDR_INDEX) = PP_HDR_FROM_GP_PARAM(info->pDataReq);
				*(txBuffer + PP_DTYP_INDEX) = PP_TYPE_FROM_GP_PARAM(info->pDataReq);
				*(txBuffer + PP_LEN_INDEX) = PP_LEN_FROM_GP_PARAM(info->pDataReq);


				packetLen=(IO_BufferSizeType)(PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX)+((*(txBuffer + PP_HDR_INDEX) & PP_HDR_CRC)?2:1));
				ed_index= PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX);

				if(info->pDataReq)
				{
					(void)memcpy((txBuffer + PP_DAT_INDEX), PP_PDATA_FROM_GP_PARAM(info->pDataReq), *(txBuffer + PP_LEN_INDEX));
				}

			break;


	        case PP_CMD_BASIC:
				*(txBuffer + PP_STX_INDEX) = PP_STX;
				*(txBuffer + PP_BS_TYP_INDEX) = PP_BS_TYPE_FROM_GP_PARAM(info->pDataReq);

				packetLen=6;
				ed_index=5;

				if(info->pDataReq)
				{
					(void)memcpy((txBuffer + 2), PP_BS_PDATA_FROM_GP_PARAM(info->pDataReq), 3);
				}

	        break;
	      }
	    break;
	#endif

	default:
	      *(txBuffer + PP_STX_INDEX) = PP_STX;
	      *(txBuffer + PP_FTYP_INDEX) = PP_FTYP_EE;
	      *(txBuffer + PP_HDR_INDEX) = (info->option&PPM_OPTION_CRC)?PP_HDR_CRC:PP_HDR_CHK;
	      *(txBuffer + PP_DTYP_INDEX) = info->code;
	      *(txBuffer + PP_LEN_INDEX) = (uint8_t)(*info->pLenReq);
	      pData=info->pDataReq;
	      len=*info->pLenReq;
	      d_index=PP_DAT_INDEX;
	      s_index=0;
	      packetLen=(IO_BufferSizeType)(PP_LEN_INDEX+1+*info->pLenReq+((info->option&PPM_OPTION_CRC)?2:1));
	      ed_index= PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX);


	      if(info->pDataReq)
			{
            (void) memcpy((txBuffer + d_index), pData+s_index, (size_t) len);
			}
			else
			{
			/* memset(txBuffer + d_index, 0, len); */
			}

	    break;
	  }

	  /* calculate crc or chk */
	  if(*(txBuffer + PP_STX_INDEX) == PP_STX)
	  {
	    if((*(txBuffer + PP_FTYP_INDEX) == PP_FTYP_EE) && (*(txBuffer + PP_HDR_INDEX) & PP_HDR_CRC)/*info.option & PPM_OPTION_CRC &&*/ )
	    {
	      crc_append(ed_index,txBuffer);
	    } else
	    {
	      chk_append(ed_index,txBuffer);
	    }
	  }

	  return packetLen;
}



I2C_SlaveAddressType PP_GetSlaveAddress(IO_ChannelType idBoard, const PP_MasterProtocolTableType *info)
{
	I2C_SlaveAddressType retVal = PP_NtwkTable[idBoard].slaveAddress;

#if defined(CP_GENERAL_PURPOSE) || defined(CP_BOOTLOADER)

	switch(info->paramType)
	  {
	    case PPM_REQPARAMTYPE_GP:
	    case PPM_REQPARAMTYPE_BOOT:
	    	retVal I2C_ADDRESS_FROM_GP_PARAM(info->pDataReq);
	    break;

	    default:
	    break;
	  }
#endif

    return retVal;
}



IO_ErrorType PP_StartDriverRequestSync(IO_ChannelType channel, CommDriverParamType *param)
{
	MasterPlatformProtocol[channel].error = ((IO_PhyCommunicationSync)(BCP_ChannelTable[channel].StartTxSync))(param);


	if(MasterPlatformProtocol[channel].error == (IO_ErrorType)IO_E_OK )
	{
	    MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_WORKING;
	}

	return MasterPlatformProtocol[channel].error;
}
#endif
/**
Starts the request \a indexReq on the channel \a channel of the protocol.

builds the packet and calls the driver's method. It doesn't wait for the request is over.

\param [in] 	channel
Protocol channel
\param [in] 	indexReq IO_ChannelType
Protocol request index (PP_MasterRequestTable).
\param [in]		param
pointer to driver parameters

\return 		  IO_ErrorType

\retval     #IO_E_WORKING
Richiesta avviata con successo.
\retval     #IO_E_BUSY
Richiesta gia' in atto
\retval     #IO_E_INVALID_VALUE
Parametri non corretti

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti

\todo  PP_BOOT_TYPE_READ not implemented yet
\todo PPM_REQPARAMTYPE_EXTENDED not implemented
\todo change name
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType PP_MasterPrepareRequest(IO_ChannelType indexReq, IO_ChannelType channel, CommDriverParamType *param)
{

  BCP_BuilderType commPrepareInfo;

  if(indexReq>PP_REQ_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  /* Initializes state machine Protocol Protocol */
  MasterPlatformProtocol[channel].indexRequest=indexReq;
  MasterPlatformProtocol[channel].lastRequestLen=0;

  /* Default action */
  MasterPlatformProtocol[channel].state=PP_RESPONSE_GET_ACK;
  param->option=IO_OPT_MASTER;
  param->buffer=BCP_ChannelTable[channel].txBuffer;
  param->callback=BCP_MasterNotify;

#if defined(CP_BOOTLOADER)

  switch(PP_MasterProtocolTable[indexReq].idStateMachine)
  {
    case BCP_BOOTLOADER:
      param->option=IO_OPT_MASTER;
      param->buffer=BCP_ChannelTable[channel].txBuffer;
      param->callback=BCP_MasterNotify;
      switch(PP_TYPE_FROM_GP_PARAM(PP_MasterProtocolTable[indexReq].pDataReq))
      {
		  case PP_BOOT_TYPE_ACK:
		  case PP_BOOT_TYPE_ERASE:
		  case PP_BOOT_TYPE_WRITE:
		  case PP_BOOT_TYPE_QUIT:
			  MasterPlatformProtocol[channel].state=BOOT_RESPONSE_GET_ACK;
			  break;
		  case PP_BOOT_TYPE_IDENT:
			  MasterPlatformProtocol[channel].state=BOOT_RESPONSE_GET_LEN;
			  break;
		  case PP_BOOT_TYPE_READ:
			  MasterPlatformProtocol[channel].state=BOOT_GET_DATA;
			  break;
      }
      break;

    default:
  }
#endif

  return (IO_ErrorType)IO_E_OK;
}
#endif

/**

Avvia la procedura per la risposta su un canale di comunicazione.

\param [in] 	channel Indice del canale di comunicazione
\param [in] 	master  Parametri per il processamento della risposta.

\return     #IO_ErrorType

\retval     IO_E_OK
Richiesta avviata con successo.
\retval     IO_E_INVALID_VALUE
Parametri non corretti.
\retval     IO_E_INVALID_CHANNEL_ID
Canale non corretto.

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType BCP_MasterStartResponce(  IO_ChannelType channel, const BCP_MasterType * master)
{
  CommDriverParamType responceParam={0};
  responceParam.len=master->requestLen;
  responceParam.callback=master->callback;
  responceParam.option= IO_OPT_MASTER | master->option;
  responceParam.buffer=BCP_ChannelTable[channel].rxBuffer+MasterPlatformProtocol[channel].lastRequestLen;

  MasterPlatformProtocol[channel].lastRequestLen+=master->requestLen;

  return ((IO_PhyCommunicationSync)(BCP_ChannelTable[channel].StartRxSync))(&responceParam);
}




void PP_MasterValidateEEAnswer(IO_ChannelType channel, PP_MasterProtocolTableType* pProt)
{
IO_BufferSizeType  len;
uint16_t crc;

BCP_ChannelTableType const * pChannel = &BCP_ChannelTable[channel];
    switch(pProt->paramType)
    {
#if defined(CP_GENERAL_PURPOSE)
    case PPM_REQPARAMTYPE_GP:

      if((pChannel->rxBuffer[PP_STX_INDEX] == PP_ACK))
      {

            switch(pProt->pDataReq[PP_GP_CMDID_INDEX])
    		{

    		case PP_CMD_EXTENDED_WITH_ANSW:
    			/* extended with answer */
    			len=(pChannel->rxBuffer[PP_LEN_INDEX+PP_ACK_SZ]==0)?0x100:pChannel->rxBuffer[PP_LEN_INDEX+PP_ACK_SZ];
    			
    			len=(len+PP_OUT_BUFF_RESERVED_SZ>=pProt->LenRes)?pProt->LenRes-PP_OUT_BUFF_RESERVED_SZ:len;
    			
    			if(pChannel->txBuffer[PP_HDR_INDEX+PP_ACK_SZ] & PP_HDR_CRC)
                {
                  crc = BCP_CRC_calculate(len+PP_DAT_INDEX+2, &(pChannel->rxBuffer[PP_ACK_SZ]), 0xFFFF);
                }
                else
                {
                  crc= LIB_ChkComputation((uint16_t)(len+PP_DAT_INDEX), &(pChannel->rxBuffer[PP_ACK_SZ]),0);
                  crc &= (uint16_t)0x00FF;
                  crc = crc^(pChannel->rxBuffer[len+PP_ACK_SZ+PP_DAT_INDEX]);
                }

                if(!crc && (pChannel->rxBuffer[PP_STX_INDEX+PP_ACK_SZ] == PP_STX)) 
                {

                    MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;
                    
                    PP_EncodeHdrGPAnsParam(pProt, pChannel->rxBuffer[PP_ACK_POS]);
    			    PP_EncodeDataGPAnsParam(pProt, len, &pChannel->rxBuffer[PP_DAT_INDEX+PP_ACK_SZ]);
    			    
    				
                    BCM_CountersUpdater(channel, BCP_PACKET_NUM_NOERR_CNT);
                } else 
                {
                  BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
                  
                  MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_CHECK_PP;
                }
        
    			break;
    		case PP_CMD_EXTENDED_NO_ANSW:
    		        PP_EncodeHdrGPAnsParam(pProt, pChannel->rxBuffer[PP_ACK_POS]);
    		        MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;
    		    break;

    		case PP_CMD_BASIC:
    		    len=(pChannel->rxBuffer[PP_BS_LEN_ANS_INDEX+PP_ACK_SZ]==0)?0x100:pChannel->rxBuffer[PP_BS_LEN_ANS_INDEX+PP_ACK_SZ];
    		    len=(len+PP_OUT_BUFF_RESERVED_SZ>=pProt->LenRes)?pProt->LenRes-PP_OUT_BUFF_RESERVED_SZ:len;

                if( ((*(pChannel->txBuffer+PP_BS_TYP_INDEX+PP_ACK_SZ) & PP_BS_TYPE_MSK) == PP_BS_TYPE_READ_RAM)
                && (pChannel->rxBuffer[PP_STX_INDEX+PP_ACK_SZ] == PP_STX) 
                )
                {
                    crc= LIB_ChkComputation((uint16_t)(len+PP_BS_DAT_ANS_INDEX), &(pChannel->rxBuffer[PP_ACK_SZ]),0);
                    crc &= (uint16_t)0x00FF;
                    crc = crc^(pChannel->rxBuffer[len+PP_ACK_SZ+PP_BS_DAT_ANS_INDEX]);
                } else
                {
                    crc=0;
                }
    		    
    			/* basic with answer */
    			if(!crc)
    			{
    			    PP_EncodeHdrGPAnsParam(pProt, pChannel->rxBuffer[PP_ACK_POS]);
    			    if( ((*(pChannel->txBuffer+PP_BS_TYP_INDEX+PP_ACK_SZ) & PP_BS_TYPE_MSK) == PP_BS_TYPE_READ_RAM))
    			    {
    			        PP_EncodeDataGPAnsParam(pProt, len, &pChannel->rxBuffer[PP_BS_DAT_ANS_INDEX+PP_ACK_SZ]);
    			    }
    			    MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;
    				
    			}
    			else 
                {

                  BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
                  MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_CHECK_PP;

                }
                
    			break;
    		}

    		break;
    	}
    	else if(pChannel->rxBuffer[PP_STX_INDEX] == PP_NACK)
    	{
    		PP_EncodeHdrGPAnsParam(pProt, pChannel->rxBuffer[PP_ACK_POS]);
    		MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_ACK_PP;
    	}else
    	{
    		MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_ACK_BUS;
    	}
      break;
    #endif

    default:
        /* 'len' extended protocol*/
        len= ((pChannel->rxBuffer[PP_LEN_INDEX+PP_ACK_SZ] == (IO_BufferSizeType) 0) ? 0x100 : pChannel->rxBuffer[PP_LEN_INDEX+PP_ACK_SZ]);

        if(pChannel->txBuffer[PP_HDR_INDEX+PP_ACK_SZ] & PP_HDR_CRC)
        {
          crc = BCP_CRC_calculate(len + (IO_BufferSizeType) (PP_DAT_INDEX + 2), &(pChannel->rxBuffer[PP_ACK_SZ]), (uint16_t) 0xFFFF);
        }
        else
        {
          crc= LIB_ChkComputation((uint16_t)(len+PP_DAT_INDEX), &(pChannel->rxBuffer[PP_ACK_SZ]),(uint8_t) 0);
          crc &= (uint16_t)0x00FF;
          crc = crc^(pChannel->rxBuffer[len+PP_ACK_SZ+PP_DAT_INDEX]);
        }

        if(!crc) {

          /* TODO delete PP_Ntwk[MasterPlatformProtocol[channel].chBoard].esit=IO_E_OK;*/
        	MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;

          if(pProt->option & PPM_OPTION_DATACOPY)
          {
              /* copia: dati */

              len = ((len+PP_OUT_BUFF_RESERVED_SZ) >= pProt->LenRes) ? (IO_BufferSizeType) pProt->LenRes-PP_OUT_BUFF_RESERVED_SZ : len;
        	  (void)memcpy(pProt->pDataRes+PP_OUT_BUFF_RESERVED_SZ, &pChannel->rxBuffer[PP_DAT_INDEX+PP_ACK_SZ], len);
              *((IO_BufferSizeType *)pProt->pDataRes)=len + (PP_OUT_BUFF_RESERVED_SZ - sizeof(IO_BufferSizeType));
          }
          BCM_CountersUpdater(channel, BCP_PACKET_NUM_NOERR_CNT);
        } else {

          BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
#if BCP_MASTER_SUPPORT == TRUE
          BCM_ForceRebuild((IO_ChannelType)channel);
#endif
          /* TODO delete PP_Ntwk[MasterPlatformProtocol[channel].chBoard].esit=IO_E_CHECK_PP;*/
          MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;

        }
    break;

    }   
}

IO_ErrorType PP_MasterValidateAnswer (IO_ChannelType channel)
{
IO_ChannelType idx=MasterPlatformProtocol[channel].indexRequest;
PP_MasterProtocolTableType *pProt = (PP_MasterProtocolTableType *) &PP_MasterProtocolTable[idx];
    switch(pProt->idStateMachine)
    {
#if defined(CP_BOOTLOADER)
        case BCP_BOOTLOADER:
          /* TODO CP_MasterValidateBootLoaderAnswer(channel); */
          break;
#endif
        default:
          PP_MasterValidateEEAnswer(channel, pProt);
    }
    return PP_GetEsit(channel);    
}
#endif


/**

Callback del driver di comunicazione per il processamento delle notifiche.

\param [in]   notifVal  Identificatore della notifica
\param [in]   channel   Indice del canale di comunicazione
\param [in]   error     Codice dell'errore della notifica

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType BCP_MasterNotify (IO_NotificationType notifVal,IO_ChannelType channel, IO_ErrorType error)
{
IO_ChannelType idx=MasterPlatformProtocol[channel].indexRequest;

  switch(notifVal)
  {
    case IO_N_TRANSFER_DONE:
      switch(PP_MasterProtocolTable[idx].idStateMachine)
      {
#if defined(CP_BOOTLOADER)
        case BCP_BOOTLOADER:
          BCP_BootLoaderResponseStateMachine(channel);
          break;
#endif
        default:
          BCP_MasterResponseStateMachine(channel, error);
          break;
      }

      break;
    case IO_N_ERROR:
      PP_Ntwk[MasterPlatformProtocol[channel].chBoard].esit=error;
      if((channel == BCP_I2C_CH_ID) && (error == (IO_ErrorType)IO_E_ACK_BUS))
      {
        BCM_CountersUpdater(channel, BCP_DRV2_CNT);
      }

      MasterPlatformProtocol[channel].error = error;
      break;
    default:
    ;

  }
  return IO_E_OK;
}
#endif


#if BCP_MASTER_SUPPORT == TRUE
void PP_EncodeHdrGPAnsParam(PP_MasterProtocolTableType* pGpReq, uint8_t ackval)
{
	
	if(*(pGpReq->pDataRes+PP_GP_CMDID_ANS_INDEX) != PP_CMD_EXTENDED_NO_ANSW)
	{
		/* HDR */
		*(pGpReq->pDataRes+PP_GP_HDR_ANS_INDEX)=0x02 | *(pGpReq->pDataReq+ PP_GP_HDR_INDEX);

		/* TYP */
		*(pGpReq->pDataRes+PP_GP_TYP_ANS_INDEX)=*(pGpReq->pDataReq+ PP_GP_TYP_INDEX);
	}
	
	/* copying seq num and CmdId */
	(void)memcpy(pGpReq->pDataRes+PP_OUT_BUFF_RESERVED_SZ, pGpReq->pDataReq, PP_OUT_BUFF_RESERVED_SZ);

	/* ACK */
	*(pGpReq->pDataRes+PP_GP_ACK_ANS_INDEX)=ackval;

	/* bridge data len */
	*((IO_BufferSizeType *)pGpReq->pDataRes)= (IO_BufferSizeType)(PP_GP_ACK_ANS_INDEX - PP_OUT_BUFF_RESERVED_SZ + 1);

}

#if 0
void PP_EncodeHdrAnsBridgeParam(IO_ChannelType idx, uint8_t ackval)
{
	/* copying seq num and CmdId */
	(void)memcpy(PP_MasterProtocolTable[idx].pDataRes+PP_OUT_BUFF_RESERVED_SZ, PP_MasterProtocolTable[idx].pDataReq, PP_OUT_BUFF_RESERVED_SZ);

	if(*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_CMDID_ANS_INDEX) != PP_CMD_EXTENDED_NO_ANSW)
	{
		/* HDR */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_HDR_ANS_INDEX)=0x02;

		/* TYP */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_TYP_ANS_INDEX)=*(PP_MasterProtocolTable[idx].pDataReq+ PP_GP_TYP_INDEX);
	}

	/* ACK */
	*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_ACK_ANS_INDEX)=ackval;

	/* bridge data len */
	*((IO_BufferSizeType *)PP_MasterProtocolTable[idx].pDataRes)= (IO_BufferSizeType)(PP_GP_ACK_ANS_INDEX - PP_OUT_BUFF_RESERVED_SZ + 1);

}
#endif
#endif

#if BCP_MASTER_SUPPORT == TRUE

void PP_EncodeDataGPAnsParam(PP_MasterProtocolTableType* pGpReq, IO_BufferSizeType u8Len, uint8_t* pData)
{
	IO_BufferSizeType len = u8Len;
	
	if(len)
	{
	    /* copying: LEN , DATA */
    	*(pGpReq->pDataRes+(PP_GP_OUT_BUFF_RESERVED_SZ - PP_LEN_SZ)) = (uint8_t)len;
    	(void)memcpy((pGpReq->pDataRes + PP_GP_OUT_BUFF_RESERVED_SZ ), pData, len);

    	/* bridge data len update */
    	*((IO_BufferSizeType *)pGpReq->pDataRes) += (len + PP_GP_LEN_ANS_INDEX - PP_GP_ACK_ANS_INDEX);
	}	
}

#endif
#if BCP_MASTER_SUPPORT == TRUE
void PP_EncodeDataBootAnsParam(IO_ChannelType idx, uint8_t* pData)
{

      /* TYP */
      /*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_TYP_ANS_INDEX)=*(PP_MasterProtocolTable[idx].pDataReq+ PP_GP_TYP_INDEX);*/

      switch(*(PP_MasterProtocolTable[idx].pDataReq+ PP_GP_TYP_INDEX))
      {
      case PP_BOOT_TYPE_ACK:
      case PP_BOOT_TYPE_QUIT:
    	  /* LEN */
    	  *(PP_MasterProtocolTable[idx].pDataRes+PP_GP_LEN_ANS_INDEX)=1;
  		  /* DATA */
  		  (void)memcpy((PP_MasterProtocolTable[idx].pDataRes + PP_GP_OUT_BUFF_RESERVED_SZ ), pData, 1);
    	  break;
      default:
    	  /* PP_BOOT_TYPE_WRITE, PP_BOOT_TYPE_ERASE */

		  /* LEN */
		  *(PP_MasterProtocolTable[idx].pDataRes+PP_GP_LEN_ANS_INDEX)=3;

		  /* DATA */
		  (void)memcpy((PP_MasterProtocolTable[idx].pDataRes + PP_GP_OUT_BUFF_RESERVED_SZ), pData, 1);
		  (void)memcpy((PP_MasterProtocolTable[idx].pDataRes + PP_GP_OUT_BUFF_RESERVED_SZ +1 ), PP_MasterProtocolTable[idx].pDataReq + PP_GP_DAT_INDEX + 1, 2);

      }

      /* bridge data len update */
      *((IO_BufferSizeType *)PP_MasterProtocolTable[idx].pDataRes) += (*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_LEN_ANS_INDEX) + PP_GP_LEN_ANS_INDEX - PP_GP_ACK_ANS_INDEX);
}
#endif

#if (BCP_MASTER_SUPPORT == TRUE) && defined(CP_BOOTLOADER)
static void BCP_BootLoaderResponseStateMachine(IO_ChannelType channel)
{
  BCP_MasterType responce={0};
  IO_ChannelType idx=MasterPlatformProtocol[channel].indexRequest;

  switch(MasterPlatformProtocol[channel].state)
  {
    case BOOT_RESPONSE_GET_ACK:
		responce.callback=BCP_MasterNotify;
		responce.requestLen=1;
		responce.option |= (IO_CLOSE|IO_OPT_MASTER);

		MasterPlatformProtocol[channel].state = BOOT_END_ACK;

		(void)BCP_MasterStartResponce(channel, &responce);
		break;

    case BOOT_RESPONSE_GET_LEN:
    	responce.callback=BCP_MasterNotify;
		responce.requestLen=1;
		responce.option |= (IO_OPT_MASTER);

		MasterPlatformProtocol[channel].state = BOOT_GET_DATA;

		(void)BCP_MasterStartResponce(channel, &responce);
    	break;

    case BOOT_GET_DATA:
    	responce.callback=BCP_MasterNotify;
		responce.requestLen=BCP_ChannelTable[channel].rxBuffer[0];
		responce.option |= (IO_CLOSE|IO_OPT_MASTER);

		MasterPlatformProtocol[channel].state = BOOT_END_DATA;

		(void)BCP_MasterStartResponce(channel, &responce);
    	break;

    case BOOT_END_DATA:
    	MasterPlatformProtocol[channel].state=BOOT_END;

		/* copying seq num and CmdId */
    	(void)memcpy(PP_MasterProtocolTable[idx].pDataRes+PP_OUT_BUFF_RESERVED_SZ, PP_MasterProtocolTable[idx].pDataReq, PP_OUT_BUFF_RESERVED_SZ);

		/* TYP */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_TYP_ANS_INDEX)=*(PP_MasterProtocolTable[idx].pDataReq+ PP_GP_TYP_INDEX);


		/* ACK */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_ACK_ANS_INDEX)=0x5A;

		/* bridge data len */
		*((IO_BufferSizeType *)PP_MasterProtocolTable[idx].pDataRes)= (IO_BufferSizeType)(PP_GP_ACK_ANS_INDEX - PP_OUT_BUFF_RESERVED_SZ + 1);

		/* HDR */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_HDR_ANS_INDEX)=0x02;


		/* LEN */
		*(PP_MasterProtocolTable[idx].pDataRes+PP_GP_LEN_ANS_INDEX)=MasterPlatformProtocol[channel].lastRequestLen-1;

		/* DATA */
		(void)memcpy((PP_MasterProtocolTable[idx].pDataRes + PP_GP_OUT_BUFF_RESERVED_SZ ), &BCP_ChannelTable[channel].rxBuffer[1], MasterPlatformProtocol[channel].lastRequestLen);

		/* bridge data len update */
		*((IO_BufferSizeType *)PP_MasterProtocolTable[idx].pDataRes) += (MasterPlatformProtocol[channel].lastRequestLen + PP_GP_LEN_ANS_INDEX - PP_GP_ACK_ANS_INDEX);

		MasterPlatformProtocol[channel].error=IO_E_OK;

    	break;

    case BOOT_END_ACK:

        PP_EncodeHdrGPAnsParam((PP_MasterProtocolTableType *) &PP_MasterProtocolTable[idx], (uint8_t)PP_ACK);
    	PP_EncodeDataBootAnsParam(idx, &BCP_ChannelTable[channel].rxBuffer[PP_ACK_POS]);

      MasterPlatformProtocol[channel].state=BOOT_END;

      MasterPlatformProtocol[channel].error=IO_E_OK;
    break;

    case BOOT_END:
    default:
    break;
  }


}
#endif


/**

Macchina a stati di ricezione Master.

\param [in]  channel Indice del canale di comunicazione
\param [in]  error Codice errore della notifica

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
#if BCP_MASTER_SUPPORT == TRUE
static void BCP_MasterResponseStateMachine(IO_ChannelType channel, IO_ErrorType error)
{
  BCP_MasterType responce={0};
  IO_ChannelType idx=MasterPlatformProtocol[channel].indexRequest;
  (void) error;

  switch(MasterPlatformProtocol[channel].state)
  {
    case PP_RESPONSE_GET_ACK:

      responce.callback=BCP_MasterNotify;
      responce.requestLen=1;

      switch(PP_MasterProtocolTable[idx].paramType) {
#if defined(CP_GENERAL_PURPOSE)
        case PPM_REQPARAMTYPE_GP:
          MasterPlatformProtocol[channel].state++;
          switch(PP_CMD_ID_FROM_GP_PARAM(PP_MasterProtocolTable[idx].pDataReq))
          {

            case PP_CMD_EXTENDED_NO_ANSW:
              responce.option |= (IO_CLOSE|IO_OPT_MASTER);
              MasterPlatformProtocol[channel].state=PP_RESPONSE_PROCESS;
              break;

            case PP_CMD_BASIC:

              if((*(BCP_ChannelTable[channel].txBuffer+PP_BS_TYP_INDEX) & PP_BS_TYPE_MSK) != PP_BS_TYPE_READ_RAM)
              {
                responce.option |= (IO_CLOSE|IO_OPT_MASTER);
                MasterPlatformProtocol[channel].state=PP_RESPONSE_PROCESS;
              }

              break;

            default:
            case PP_CMD_EXTENDED_WITH_ANSW:
            break;
          }
        break;
#endif
        default:
          MasterPlatformProtocol[channel].state++;
          if(0 == (PP_MasterProtocolTable[idx].option & PPM_OPTION_GETDATA))
          {
            responce.option |= (IO_CLOSE|IO_OPT_MASTER);
            MasterPlatformProtocol[channel].state=PP_RESPONSE_PROCESS;
          }
        break;

      }
      (void)BCP_MasterStartResponce(channel,&responce);
    break;

    case PP_RESPONSE_GET_HDR:

      /*ACK analysis */
      if(BCP_ChannelTable[channel].rxBuffer[0] != PP_ACK)
      {
        /* */

        BCM_CountersUpdater(channel, BCP_PP_ACK_MISSING_ERR_CNT);
#if BCP_MASTER_SUPPORT == TRUE
        BCM_ForceRebuild(channel);
#endif

        if((PP_MasterProtocolTable[idx].option & PPM_OPTION_GETDATA) ||
         (PP_CMD_ID_FROM_GP_PARAM(PP_MasterProtocolTable[idx].pDataReq) == PP_CMD_EXTENDED_WITH_ANSW))
        {
          responce.option|=IO_CLOSE;
          responce.requestLen = 1;
        }

        (void)BCP_MasterStartResponce(channel,&responce);
        switch(PP_MasterProtocolTable[idx].paramType)
        {
#if defined(CP_GENERAL_PURPOSE)
          case PPM_REQPARAMTYPE_GP:
            MasterPlatformProtocol[channel].state=PP_RESPONSE_PROCESS;
          break;
#endif
          default:
		  	MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_ACK_PP;
            MasterPlatformProtocol[channel].state=PP_END;
        }

        break;
      }


      switch(PP_MasterProtocolTable[idx].paramType) {
#if defined(CP_GENERAL_PURPOSE)
        case PPM_REQPARAMTYPE_GP:
          switch(PP_CMD_ID_FROM_GP_PARAM(PP_MasterProtocolTable[idx].pDataReq))
          {
            case PP_CMD_EXTENDED_WITH_ANSW:
              {
                responce.callback=BCP_MasterNotify;
                responce.requestLen=PP_LEN_INDEX+1;

                MasterPlatformProtocol[channel].state++;
                (void)BCP_MasterStartResponce(channel,&responce);
              }
              break;

            case PP_CMD_BASIC:
                /* TODO: in base al pacchetto */

                responce.callback=BCP_MasterNotify;
                responce.requestLen=PP_BS_LEN_ANS_INDEX+1;

                MasterPlatformProtocol[channel].state++;
                (void)BCP_MasterStartResponce(channel,&responce);
              break;
            case PP_CMD_EXTENDED_NO_ANSW:
              BCM_CountersUpdater(channel, BCP_PACKET_NUM_NOERR_CNT);
              break;
          }
        break;
#endif
        default:
          /*TODO: forse l'if non serve */
          if((PP_MasterProtocolTable[idx].option & PPM_OPTION_GETDATA))
          {
            responce.callback=BCP_MasterNotify;
            responce.requestLen=PP_LEN_INDEX+1;

            MasterPlatformProtocol[channel].state++;
            (void)BCP_MasterStartResponce(channel,&responce);
          }
          else
      		{
        	  MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;
      			/* TODO delete PP_Ntwk[MasterPlatformProtocol[channel].chBoard].esit = (IO_ErrorType)IO_E_OK;*/
      		}
        break;

      }


    break;

    case PP_RESPONSE_COMPLETION:

      if(BCP_ChannelTable[channel].rxBuffer[PP_FTYP_INDEX+PP_ACK_SZ]==PP_FTYP_EE)
      {
        responce.requestLen = BCP_ChannelTable[channel].rxBuffer[PP_LEN_INDEX+PP_ACK_SZ];

        if(!BCP_ChannelTable[channel].rxBuffer[PP_LEN_INDEX+PP_ACK_SZ])
        {
          responce.requestLen = 0x100;
        }

        responce.requestLen +=((BCP_ChannelTable[channel].txBuffer[PP_HDR_INDEX+PP_ACK_SZ] & PP_HDR_CRC)?2:1);
      } else
      {
        responce.requestLen = (BCP_ChannelTable[channel].rxBuffer[PP_BS_LEN_ANS_INDEX+PP_ACK_SZ]);

        if(!BCP_ChannelTable[channel].rxBuffer[PP_BS_LEN_ANS_INDEX+PP_ACK_SZ])
        {
          responce.requestLen = 0x100;
        }

        responce.requestLen++; /* chk */
      }

      responce.callback=BCP_MasterNotify;
      responce.option|=IO_CLOSE;

      (void)BCP_MasterStartResponce(channel,&responce);

      MasterPlatformProtocol[channel].state++;
    break;

    case PP_RESPONSE_PROCESS:

      MasterPlatformProtocol[channel].state++;
      MasterPlatformProtocol[channel].error = (IO_ErrorType)IO_E_OK;
      PP_OnMasterRxCompleted(channel);
    break;
    
    
    
    case PP_END:
    default:
    ;
  }
}

void PP_SetMasterError(IO_ChannelType ch, IO_ErrorType error)
{
	MasterPlatformProtocol[ch].error = error;
}
#endif /* BCP_MASTER_SUPPORT == TRUE */


/**
\todo commentare
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_BufferSizeType  BCP_MasterBuilder (IO_BufferType txBuffer, PP_MasterProtocolTableType info)
{

  IO_BufferSizeType packetLen;

  /* dati da copiare */
  IO_BufferType      pData=0;
  IO_BufferSizeType  len=0;
  IO_BufferSizeType  s_index=0, d_index=0;
  IO_BufferSizeType  ed_index=0;


  switch(info.paramType)
  {
#if defined(CP_BOOTLOADER)
    case PPM_REQPARAMTYPE_BOOT:
      len=PP_LEN_FROM_GP_PARAM(info.pDataReq);
      pData=PP_PDATA_FROM_GP_PARAM(info.pDataReq);
      packetLen=(IO_BufferSizeType)len;
    break;
#endif

#if defined(CP_GENERAL_PURPOSE)
    case PPM_REQPARAMTYPE_GP:
      switch(PP_CMD_ID_FROM_GP_PARAM(info.pDataReq))
      {
        case PP_CMD_EXTENDED_WITH_ANSW:
        case PP_CMD_EXTENDED_NO_ANSW:
          *(txBuffer + PP_STX_INDEX) = PP_STX;
          *(txBuffer + PP_FTYP_INDEX) = PP_FTYP_EE;
          *(txBuffer + PP_HDR_INDEX) = PP_HDR_FROM_GP_PARAM(info.pDataReq);
          *(txBuffer + PP_DTYP_INDEX) = PP_TYPE_FROM_GP_PARAM(info.pDataReq);
          *(txBuffer + PP_LEN_INDEX) = PP_LEN_FROM_GP_PARAM(info.pDataReq);

          len=*(txBuffer + PP_LEN_INDEX);
          pData=PP_PDATA_FROM_GP_PARAM(info.pDataReq);
          s_index=0;
          d_index=PP_DAT_INDEX;
          packetLen=(IO_BufferSizeType)(PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX)+((*(txBuffer + PP_HDR_INDEX) & PP_HDR_CRC)?2:1));
          ed_index= PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX);

          break;


        case PP_CMD_BASIC:
          *(txBuffer + PP_STX_INDEX) = PP_STX;
          *(txBuffer + PP_BS_TYP_INDEX) = PP_BS_TYPE_FROM_GP_PARAM(info.pDataReq);
          pData =PP_BS_PDATA_FROM_GP_PARAM(info.pDataReq);
          len=3;
          s_index=0;
          d_index=2;
          packetLen=6;
          ed_index=5;
          break;
      }
    break;
#endif

    default:
      *(txBuffer + PP_STX_INDEX) = PP_STX;
      *(txBuffer + PP_FTYP_INDEX) = PP_FTYP_EE;
      *(txBuffer + PP_HDR_INDEX) = (info.option&PPM_OPTION_CRC)?PP_HDR_CRC:PP_HDR_CHK;
      *(txBuffer + PP_DTYP_INDEX) = info.code;
      *(txBuffer + PP_LEN_INDEX) = (uint8_t) (*info.pLenReq);
      pData=info.pDataReq;
      len=*info.pLenReq;
      d_index=PP_DAT_INDEX;
      s_index=0;
      packetLen=(IO_BufferSizeType)(PP_LEN_INDEX+1+*info.pLenReq+((info.option&PPM_OPTION_CRC)?2:1));
      ed_index= PP_LEN_INDEX+1+*(txBuffer + PP_LEN_INDEX);
    break;
  }

  if(info.pDataReq)
  {
	  (void)memcpy((txBuffer + d_index), pData+s_index, len);
  }
  else
  {
	  (void)memset(txBuffer + d_index, 0, len);
  }

  /* calculate crc or chk */
  if(*(txBuffer + PP_STX_INDEX) == PP_STX)
  {
    if((*(txBuffer + PP_FTYP_INDEX) == PP_FTYP_EE) && (*(txBuffer + PP_HDR_INDEX) & PP_HDR_CRC)/*info.option & PPM_OPTION_CRC &&*/ )
    {
      crc_append(ed_index,txBuffer);
    } else
    {
      chk_append(ed_index,txBuffer);
    }
  }

  return packetLen;
}
#endif /* BCP_MASTER_SUPPORT == TRUE */

/* Local Function */
static void PP_SlaveFragmentBuilder(IO_ChannelType ch, uint8_t chk_displ, uint8_t data_displ)
{
		IO_BufferSizeType aval, data_len, data;
		bool_t empty=FALSE;
		data=PP_SlavePlatformProtocol[ch].dataCount;

		if(!PP_SlavePlatformProtocol[ch].dataCount)
		{
			empty=TRUE;
			data=1; 				/* overwriting */
		}

		/*numero di locazioni disponibili per i dati : size - ack - hdr - len - crc(=2). */
		aval = (uint16_t) (BCP_ChannelTable[ch].txBufferSize - (PP_SlavePlatformProtocol[ch].answerCount + 2 + chk_displ));
		data_len = (uint16_t) ((data < aval) ? data : aval);

		PP_SlavePlatformProtocol[ch].dataCountLast =	data_len;
		if(PP_SlavePlatformProtocol[ch].dataCount)
		{
			PP_SlavePlatformProtocol[ch].dataCount -= data_len;
		}

		(void)memcpy(&BCP_ChannelTable[ch].txBuffer[data_displ], PP_SlavePlatformProtocol[ch].pdata,  data_len);
		PP_SlavePlatformProtocol[ch].pdata+=data_len;
		PP_SlavePlatformProtocol[ch].answerCount+=data_len;

		/* hdr test to choose between chk or crc */
    /*TODO: testare rxBuffer o txBuffer a seconda della condivisione del buffer */
    if(PP_SlavePlatformProtocol[ch].option&1)
    {
    	/* packet check */
    	PP_SlavePlatformProtocol[ch].crc = BCP_CRC_calculate (PP_SlavePlatformProtocol[ch].answerCount, &BCP_ChannelTable[ch].txBuffer[chk_displ], PP_SlavePlatformProtocol[ch].crc);
    }
    else
    {
    	/* packet check */
    	PP_SlavePlatformProtocol[ch].crc = LIB_ChkComputation ((uint16_t)PP_SlavePlatformProtocol[ch].answerCount, &BCP_ChannelTable[ch].txBuffer[chk_displ], (uint8_t)PP_SlavePlatformProtocol[ch].crc);
    }

		if(!PP_SlavePlatformProtocol[ch].dataCount && (empty==FALSE))
		{
			if(PP_SlavePlatformProtocol[ch].option&1)
			{
				BCP_ChannelTable[ch].txBuffer[chk_displ+PP_SlavePlatformProtocol[ch].answerCount++] = (uint8_t)(PP_SlavePlatformProtocol[ch].crc >> 8);
			}

			BCP_ChannelTable[ch].txBuffer[chk_displ+PP_SlavePlatformProtocol[ch].answerCount++] = (uint8_t)PP_SlavePlatformProtocol[ch].crc;
		}
}

/**
Builder slave.

\param [in]   ch
canale
\param [in]   info
parametri

\return BCP_BufferSizeType dimensione in byte
\warning
\note
\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/
void  PP_SlaveResponseBuilder (IO_ChannelType ch, PP_SlaveBuilderInfoType info)
{
    uint8_t cpy_len, displ, displ_len;

    /* ack */
	PP_SlavePlatformProtocol[ch].dataCount=info.len;	/* init data */
	PP_SlavePlatformProtocol[ch].pdata=info.address;	/* init address */

    PP_SlavePlatformProtocol[ch].answerCount=0;				/* init answerCount */
	PP_SlavePlatformProtocol[ch].option=0;						/* init chk mode */
	PP_SlavePlatformProtocol[ch].crc=0;						/* init chk val */


    /* answer building */
    if((info.option & PP_SL_RESP_ANSW_OPT) && (info.error == (IO_ErrorType)IO_E_OK))
    {
				/* hdr test: choose between chk or crc */
		if((BCP_ChannelTable[ch].rxBuffer[PP_HDR_INDEX]&PP_HDR_CRC) && (BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX] == PP_FTYP_EE))
		{
			PP_SlavePlatformProtocol[ch].option |= 1;
			PP_SlavePlatformProtocol[ch].crc=0xFFFF;
		}

        if(BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX] == PP_FTYP_EE)
        {
          cpy_len=4;
          displ_len=(uint8_t)((uint8_t)PP_ACK_SZ+(uint8_t)PP_LEN_INDEX);
          displ=(uint8_t)((uint8_t)PP_ACK_SZ+(uint8_t)PP_DAT_INDEX);

        } else
        {
          cpy_len=2;
          displ_len=(uint8_t)PP_ACK_SZ+(uint8_t)PP_BS_LEN_ANS_INDEX;
          displ=(uint8_t)PP_ACK_SZ+(uint8_t)PP_BS_DAT_ANS_INDEX;
        }
#ifdef BCP_FRAGMENT_SLAVE
		/* header copy */
        LIB_MemcpyFromLast(&BCP_ChannelTable[ch].txBuffer[PP_ACK_SZ], BCP_ChannelTable[ch].rxBuffer,  cpy_len);

		/* len */
        BCP_ChannelTable[ch].txBuffer[displ_len]=(uint8_t)info.len;

        PP_SlavePlatformProtocol[ch].answerCount+=cpy_len+PP_LEN_SZ; /* chk not counted */

		PP_SlaveFragmentBuilder(ch, PP_ACK_SZ, (uint8_t)(PP_SlavePlatformProtocol[ch].answerCount+1));

#else	/* #ifdef BCP_FRAGMENT_SLAVE */
		/* use all the space */
		if(info.len + PP_SlavePlatformProtocol[ch].answerCount + 2 +PP_LEN_SZ > BCP_ChannelTable[ch].txBufferSize)
        {
			/* removes header(ack - a5 - ee - hdr - type - len) and crc*/
			info.len= (BCP_PacketSizeType) (BCP_ChannelTable[ch].txBufferSize-PP_SlavePlatformProtocol[ch].answerCount-2-PP_LEN_SZ);
        }

        if(PP_SlavePlatformProtocol[ch].dataCount)
		{
			PP_SlavePlatformProtocol[ch].dataCount -= info.len;
		}
		
		PP_SlavePlatformProtocol[ch].answerCount+=cpy_len+PP_LEN_SZ; /* chk not counted */

        /* header copy */
		LIB_MemcpyFromLast(&BCP_ChannelTable[ch].txBuffer[PP_ACK_SZ], BCP_ChannelTable[ch].rxBuffer,  cpy_len);

        /* len */
        BCP_ChannelTable[ch].txBuffer[displ_len] = (uint8_t) info.len;
        PP_SlavePlatformProtocol[ch].answerCount += info.len;

				/* data copy */
        (void) memcpy(&BCP_ChannelTable[ch].txBuffer[displ], info.address,  info.len);

		/* hdr test to choose between chk or crc */
		/*TODO: testare rxBuffer o txBuffer a seconda della condivisione del buffer */
		if((BCP_ChannelTable[ch].rxBuffer[PP_HDR_INDEX+1]&PP_HDR_CRC) && (BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX+1] == PP_FTYP_EE))
		{
		  /* packet check */
		  crc_append(PP_SlavePlatformProtocol[ch].answerCount, &BCP_ChannelTable[ch].txBuffer[PP_ACK_SZ]);
		  PP_SlavePlatformProtocol[ch].answerCount+=2;
		}
		else
		{
		  /* packet check */
		  chk_append(PP_SlavePlatformProtocol[ch].answerCount, &BCP_ChannelTable[ch].txBuffer[PP_ACK_SZ]);
		  PP_SlavePlatformProtocol[ch].answerCount+=1;
		}

		if(info.callback != 0)
		{
			(info.callback)();
		}
 #endif   /* #ifdef BCP_FRAGMENT_SLAVE */
  }

  BCP_ChannelTable[ch].txBuffer[0] = (uint8_t) ((info.error == (IO_ErrorType)IO_E_OK) ? PP_ACK : PP_NACK);
  PP_SlavePlatformProtocol[ch].answerCount+=1;
}







#if defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
static IO_ErrorType PP_CmdExe(PP_ParserType* pCmd, IO_ChannelType channel)
{
  IO_ErrorType error = (IO_ErrorType)IO_E_BUSY;
  BCP_OptionType option=0;
  switch( pCmd->fields.cmd)
  {
    case PP_READ_CMD:
    {
      BCP_PacketSizeType Len = pCmd->fields.par1;
      (void)PP_SlaveReceiveData(channel, &Len, (BCP_OptionType)(pCmd->fields.par2));
      PP_len=0;
      error = (IO_ErrorType)IO_E_WORKING;

      if(pCmd->fields.par3)
      {
        /* manage the goto */
        PP_SlavePlatformProtocol[channel].state=pCmd->fields.par3;
      }
    }
    break;
    case PP_AND_CMD:
      if(BCP_ChannelTable[channel].rxBuffer[pCmd->fields.par1] & (uint8_t)pCmd->fields.par2)
      {
        /* manage the goto */
        PP_SlavePlatformProtocol[channel].state=pCmd->fields.par3;
      }
    break;

    case PP_NO_MATCH_CMD:
      if(BCP_ChannelTable[channel].rxBuffer[pCmd->fields.par1] != pCmd->fields.par2)
      {
        /* manage the goto */
        PP_SlavePlatformProtocol[channel].state=pCmd->fields.par3;
      }
    break;
    case PP_MODLEN_CMD:
      if(BCP_ChannelTable[channel].rxBuffer[pCmd->fields.par1] & pCmd->fields.par2)
      {
        /* manage the goto */
        PP_len+=pCmd->fields.par3;
      }
    break;
    case PP_SETLEN_CMD:
      PP_len+=BCP_ChannelTable[channel].rxBuffer[pCmd->fields.par1];
      if(pCmd->fields.par3)
      {
        /* manage the goto */
        PP_SlavePlatformProtocol[channel].state=pCmd->fields.par3;
      }
    break;

    case PP_READ_ALL_CMD:
    {
		option=IO_CLOSE;
		(void)PP_SlaveReceiveData(channel, &PP_len, option);
		error = (IO_ErrorType)IO_E_WORKING;
    }
    break;

    case PP_CHK_CHECK_CMD:
    {
     	uint16_t crc;

      crc = LIB_ChkComputation((uint16_t)PP_SlavePlatformProtocol[channel].dataCount-1, BCP_ChannelTable[channel].rxBuffer, 0);
      crc &= (uint16_t)0x00FF;
      crc ^= BCP_ChannelTable[channel].rxBuffer[PP_SlavePlatformProtocol[channel].dataCount-1];

      if(crc == (uint16_t)0)
      {
        /* ok!!! */
        BCM_CountersUpdater(channel, BCP_PACKET_NUM_NOERR_CNT);
        PP_SlavePlatformProtocol[channel].state=PP_SLAVE_RX_OK;
	  		error = (IO_ErrorType)IO_E_OK;
      } else
      {
        	BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
					error = (IO_ErrorType)IO_E_CHECK_PP;
      }
    }
    break;

    case PP_CRC_CHECK_CMD:
    {
      uint16_t crc;

      crc = BCP_CRC_calculate(PP_SlavePlatformProtocol[channel].dataCount, BCP_ChannelTable[channel].rxBuffer, 0xFFFF);

      if(crc == 0)
      {
        /* ok!!! */
        BCM_CountersUpdater(channel, BCP_PACKET_NUM_NOERR_CNT);
        PP_SlavePlatformProtocol[channel].state=PP_SLAVE_RX_OK;
	  		error = (IO_ErrorType)IO_E_OK;
      } else
      {
        	BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
					error = (IO_ErrorType)IO_E_CHECK_PP;
      }
    }
    break;

    case PP_INVALID_CMD:
      error = (IO_ErrorType)IO_E_INVALID_VALUE;
    default:
    case PP_RX_OK_CMD:

    break;
  }

  return error;
}
#endif


#if BCP_SLAVE_SUPPORT == TRUE && defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
static IO_ErrorType PP_SlaveRxInterpreter(IO_ChannelType channel)
{
  IO_ErrorType error = (IO_ErrorType)IO_E_WORKING;
  PP_ParserType* pCmd;
//  uint8_t code; ToBeRemoved

  do {
    pCmd = (PP_ParserType* const)PP_ParserTable[channel]+PP_SlavePlatformProtocol[channel].state;
    PP_SlavePlatformProtocol[channel].state++;
    error=PP_CmdExe(pCmd, channel);
    if(error!=(IO_ErrorType)IO_E_BUSY)
    {
      break;
    }
  }while(1);

  return error;
}
#endif

BCP_BufferSizeType PP_Typ16ToBridgeConv(uint8_t seq_id, uint8_t* pInMem, uint8_t* pOutMem )
{

	pOutMem[PP_GP_SEQ_INDEX]=seq_id;
	pOutMem[PP_GP_CMDID_INDEX]=pInMem[PP_TYP0_INDEX];
	pOutMem[PP_GP_HDR_INDEX]     = (uint8_t) (pInMem[PP_HDR_INDEX] & 0x01);
	pOutMem[PP_GP_TYP_INDEX]=pInMem[PP_TYP1_INDEX];
	pOutMem[PP_GP_LEN_INDEX]     = (uint8_t) (pInMem[PP_GLEN_INDEX] - 1);
	pOutMem[PP_GP_I2C_ADDR_INDEX]=pInMem[PP_GDAT_INDEX];

	(void)memcpy((&pOutMem[PP_GP_LEN_INDEX]+1), (&pInMem[PP_GDAT_INDEX]+1), pOutMem[PP_GP_LEN_INDEX]);

	return (uint16_t) (6 + pOutMem[PP_GP_LEN_INDEX]);
}

/* TODO: temp. */
BCP_BufferSizeType PP_BridgeToTyp16Conv(uint8_t* in, uint8_t* out)
{

	BCP_BufferSizeType len;

	out[PP_ACK_POS] = in[PP_GP_ACK_ANS_INDEX];
	if(
	(in[PP_GP_CMDID_ANS_INDEX] == PP_CMD_EXTENDED_NO_ANSW)
	|| ((in[PP_GP_CMDID_ANS_INDEX] == PP_CMD_BASIC) && ((in[PP_GP_TYP_ANS_INDEX] & PP_BS_TYPE_MSK) != PP_BS_TYPE_READ_RAM) )
	)
	{
		return PP_ACK_SZ;
	}
	out[PP_STX_INDEX + PP_ACK_SZ] = PP_STX;
	out[PP_FTYP_INDEX + PP_ACK_SZ] = PP_FTYP_EE;
	out[PP_HDR_INDEX + PP_ACK_SZ] = in[PP_GP_HDR_ANS_INDEX];
	out[PP_TYP0_INDEX + PP_ACK_SZ] = in[PP_GP_CMDID_ANS_INDEX];
	out[PP_TYP1_INDEX + PP_ACK_SZ] = in[PP_GP_TYP_ANS_INDEX];
	out[PP_GLEN_INDEX + PP_ACK_SZ] = in[PP_GP_LEN_ANS_INDEX];

	/*lint -e{826} Warning 826 suspicious pointer-to-pointer conversion (area too small) */
    len=*(pBCP_BufferSizeType)(&in[PP_GP_SZ_ANS_INDEX]);
    if(len<PP_GP_LEN_ANS_INDEX)
    {
        len=1;
        return 1;
    } else
    {
        len= out[PP_GLEN_INDEX + PP_ACK_SZ];
	    len = (uint16_t) ((len == 0x00)? 0x100 : len);
    }
	

	
	(void)memcpy(out + (PP_GDAT_INDEX + PP_ACK_SZ), in+PP_GP_DAT_ANS_INDEX, len);

	if(out[PP_HDR_INDEX + PP_ACK_SZ] & PP_HDR_CRC)
	{
		crc_append((uint16_t) (PP_GLEN_INDEX + 1 + len), out+PP_ACK_SZ);
		return (uint16_t) (PP_GLEN_INDEX + 1 + len + 2 + PP_ACK_SZ);
	}
	else
	{
		chk_append((uint16_t) (PP_GLEN_INDEX + 1+len), out+PP_ACK_SZ);
		return (uint16_t) (PP_GLEN_INDEX + 1 + len + 1 + PP_ACK_SZ);
	}

}

/**
Macchina a stati di ricezione slave.
Nel primo step imposta la ricezione dell'header.
Il secondo step serve a determinare il numero dei byte da ricevere.
Se il campo #PP_FTYP_INDEX e' di tipo #PP_FTYP_EE aggiorna la lunghezza
con il valore campo #PP_LEN_INDEX e #PP_HDR_INDEX (checksum o crc).
altrimenti (basic frame) rimane da ricevere solo il checksum.
Nel terzo step verifica l'integrita' del frame calcolando il checksum o il crc.

\param [in]   channel Identificatore del canale di comunicazione.

\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/

#if !defined(CP_TYP8_16_SLAVE_PP_STATE_MACHINE)
static IO_ErrorType PP_SlaveReceiverStateMachine(IO_ChannelType channel)
{
  BCP_PacketSizeType len;
  BCP_OptionType option=0;
  IO_ErrorType err = (IO_ErrorType)IO_E_WORKING;

  switch(PP_SlavePlatformProtocol[channel].state)
  {
    case PP_SLAVE_RX_HDR_START:

      PP_SlavePlatformProtocol[channel].state++;
      len=5;
      (void) PP_SlaveReceiveData(channel, &len, option); /* header len */
    break;
    case PP_SLAVE_RX_ALL_START:
      PP_SlavePlatformProtocol[channel].state++;

      /* check syntax */
      if(BCP_ChannelTable[channel].rxBuffer[PP_STX_INDEX]==PP_STX)
      {
        if(BCP_ChannelTable[channel].rxBuffer[PP_FTYP_INDEX]==PP_FTYP_EE)
        {
          /* receive extended frame */
          len =   (BCP_ChannelTable[channel].rxBuffer[PP_LEN_INDEX]);
          len +=  ((BCP_ChannelTable[channel].rxBuffer[PP_HDR_INDEX] & PP_HDR_CRC)?2:1);
        } else
        {
			/* receive basic frame */
			len = 1;
        }
        
		option=IO_CLOSE;
		
		if((len+5) > BCP_ChannelTable[channel].rxBufferSize)
		{
			len=1;
		}
				
        (void) PP_SlaveReceiveData(channel, &len,option);

      } else 
	  {
		BCM_CountersUpdater(channel, BCP_PP_CORRUPTED_PKT_ERR_CNT);
		err = (IO_ErrorType)IO_E_INVALID_VALUE;
		PP_SlavePlatformProtocol[channel].state=(IO_ValueType)PP_SLAVE_RX_ERR;
      }

    break;
    case PP_SLAVE_RX_ALL_COMPLETED:
        PP_SlavePlatformProtocol[channel].state++;
    break;

	case PP_SLAVE_RX_OK:
		err = (IO_ErrorType)IO_E_OK;
		/* -fallthrough */
	case PP_SLAVE_RX_ERR:
		/* lint -e{825} -fallthrough */
    default:

    	;
  }
  return err;
}
#endif


/**
Avvia la ricezione dello slave.

\param [in]   chid  Indice del canale di comunicazione
\param [in]   pLen  Puntatore alla dimensione in byte dei byte da ricevere
\param [in]   option Opzioni
opzioni

\return 		codice di errore
\retval     IO_E_INVALID_VALUE: buffer o len non validi
            IO_E_INVALID_CHANNEL_ID: canale DMA non corretto.
            IO_E_OK: ricezione avviata regolarmente.

\warning

\note

\version		1.0.0
\date 			10/04/2009
\author			Roberto Fioravanti
\private \memberof PP_SlavePlatformProtocolClassType
*/
static IO_ErrorType PP_SlaveReceiveData(IO_ChannelType chid, BCP_PacketSizeType * pLen, BCP_OptionType option)
{
  CommDriverParamType driverParam={0};
  driverParam.buffer=BCP_ChannelTable[chid].rxBuffer + PP_SlavePlatformProtocol[chid].dataCount;
  driverParam.len= *pLen;
  driverParam.option=option;

  PP_SlavePlatformProtocol[chid].dataCountLast= *pLen;
  PP_SlavePlatformProtocol[chid].dataCount+=PP_SlavePlatformProtocol[chid].dataCountLast;

  return ((IO_PhyCommunicationSync)(BCP_ChannelTable[chid].StartRxSync))(&driverParam);
}


#if BCP_MASTER_SUPPORT == TRUE
static IO_ErrorType  BCP_MasterProcessHeader (IO_ChannelType channel, BCP_BuilderType info)
{
    (void) info;
    
  if( (*(BCP_ChannelTable[channel].rxBuffer + (uint8_t)PP_STX_INDEX) != PP_STX)  ||
      (*(BCP_ChannelTable[channel].rxBuffer + (uint8_t)PP_FTYP_INDEX) != PP_FTYP_EE))
      {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
      }

  return (IO_ErrorType)IO_E_OK;
}
#endif /* BCP_MASTER_SUPPORT == TRUE */



/**
Appends checksum  to buffer \c pdata of length \c len

\param [in]   len number of bytes  (>=0)
\param [in]   pdata  pointer to data

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
static void chk_append (IO_BufferSizeType  len, IO_BufferType pdata)
{
	uint16_t crc = LIB_ChkComputation ((uint16_t)len, pdata, 0);
  *(pdata+len) = (uint8_t)crc;
}

/**
Appends CRC to buffer \c pdata of length \c len

\param [in]   len number of bytes (>=0)
\param [in]   pdata  pointer to data

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
static void crc_append (IO_BufferSizeType  len, IO_BufferType pdata)
{
  uint16_t crc = BCP_CRC_calculate (len, pdata, 0xFFFF);
  *(pdata+len) = (uint8_t) (crc/256);
  *(pdata+len+1) = (uint8_t)crc;
}





/* *********** */
/* End of file */
/* *********** */

