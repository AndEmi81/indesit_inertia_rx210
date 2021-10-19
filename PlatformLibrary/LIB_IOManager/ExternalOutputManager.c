/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\n			Implementation of all functions necessary to achieve the aim.
\n
\file			ExternalOutputManager.c 
\ingroup	ExtOutputsManager
\date			Sep 13, 2010 
\version	01.00.00 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:
\n 
\n 		00.00.64
\n		14/09/2010 
\n		Roberto Fioravanti 
\n		changes 
\n 		- creation
\n
\n etc.
*/
#define MODULE_BODY_LIB_IOMANAGER
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "LIB_Error.h"
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)

#include "LIB_Utilities.h"
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#include "LoadsRequest.h"
#endif  /* defined(CONFIG_LIB_LOADSREQUEST_SUPPORT) */
#include "OutputManager.h"
/*lint -e537*/
#include "ExternalOutputManager.h"
/*lint +e537*/
#include "ExternalOutputManagerExp.h"
#include "OutputManagerExp.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/*External params*/
#define EXTERNAL_PARAMS_UART(pByte)     			(((*((uint8_t*)(pByte))) & 0xC0)>>6)            /*  */
#define EXTERNAL_PARAMS_DISPL(pByte)     			(*((uint8_t*)(pByte) +1 ))            /*  */

/*Debug*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* structure for seq id management */
typedef struct {
	uint8_t Seq;
	uint8_t OldReq;
	uint8_t NewReq;
} ExtSeqIdType;

/* infopacketdata sub-block header format: each infopacket's sub-block 
has this header  */
typedef struct {
	uint8_t	szData;
	uint8_t	pilotType;
} InfoPacketDataHdrType;

/* bldc command value */
typedef struct {
	int16_t 	rpm;
	uint16_t 	time;
} ExternalBldcWashCmdType;


/* bldc command value */
typedef ExternalBldcWashCmdType ExternalBldcDrainCmdType;



/* external Command buffer params */
typedef struct{
	uint8_t* 	pCmdBuffer;
	uint8_t*	pCmdBufferTx;
	uint8_t		szCmdBuffer;
}IOM_External_CommandBufferTableType;

/* loads param */
typedef struct
{
	uint8_t* 	pPilotTypeInfo;
	uint8_t		szPilotTypeInfo;
	uint8_t		PilotType;
	uint8_t		CmdOffset;
	uint8_t		szCmd;
	uint8_t		Displacement;
	uint8_t		CommCh;
} IOM_External_LoadsTableType;

typedef struct 
{
	uint8_t* 	pRamBuffer;
	uint8_t		szRamBuffer;
#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
	uint8_t 	*pGlobalData;
	uint8_t 	szGlobal;
#endif
}ExternalDataType;


#if defined(IOM_EXTERNAL_0_ENABLED)
#define EXTERNAL_BOARD_MIN    0
#else
#define EXTERNAL_BOARD_MIN    1
#endif


/* ********* */
/* Variables */
/* ********* */

/* Global */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_1_ENABLED)
uint8_t IOM_CommandBufferExternal_1[IOM_EXTERNAL_1_COMMANDBUFFER_SIZE];
uint8_t IOM_CommandBufferExternal_1_Tx[IOM_EXTERNAL_1_COMMANDBUFFER_SIZE];	/*total size is seq ID size + cmd data size*/
uint8_t	IOM_ReadRamExternal_1[IOM_EXTERNAL_1_GLOBALRAMBUFFER_SIZE];

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
uint8_t ExternalGlobalData_1[IOM_EXTERNAL_1_GLOBALDATABUFFER_SIZE];
#endif /* defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT) */

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_1_ENABLED)*/

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)

uint8_t IOM_CommandBufferExternal_0[IOM_EXTERNAL_0_COMMANDBUFFER_SIZE];		/*total size is seq ID size + cmd data size*/
uint8_t IOM_CommandBufferExternal_0_Tx[IOM_EXTERNAL_0_COMMANDBUFFER_SIZE];	/*total size is seq ID size + cmd data size*/
uint8_t	IOM_ReadRamExternal_0[IOM_EXTERNAL_0_GLOBALRAMBUFFER_SIZE];					/*total size is cnt + seq id + ram size */

#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
uint8_t ExternalGlobalData_0[IOM_EXTERNAL_0_GLOBALDATABUFFER_SIZE];
#endif /* defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT) */

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)*/


IOM_External_LoadsTableType	IOM_External_LoadsTable[IOM_NUM_OF_EXTERNAL_LOADS];

const IOM_External_CommandBufferTableType IOM_External_CommandBufferTable[EXTERNAL_NUM]=
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)
{IOM_CommandBufferExternal_0, IOM_CommandBufferExternal_0_Tx, sizeof(IOM_CommandBufferExternal_0)},
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)*/
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_1_ENABLED)
{IOM_CommandBufferExternal_1, IOM_CommandBufferExternal_1_Tx, sizeof(IOM_CommandBufferExternal_1)},
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)*/
};

ExtSeqIdType ExternalSeqId[EXTERNAL_NUM];

const ExternalDataType ExternalData[EXTERNAL_NUM]=
{
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_0_ENABLED)
	{IOM_ReadRamExternal_0, sizeof(IOM_ReadRamExternal_0)
#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
	,
	ExternalGlobalData_0,
	sizeof(ExternalGlobalData_0)
#endif
	},
#endif	
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(IOM_EXTERNAL_1_ENABLED)
	{IOM_ReadRamExternal_1, sizeof(IOM_ReadRamExternal_1)
#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
	,
	ExternalGlobalData_1,
	sizeof(ExternalGlobalData_1)
#endif
	},
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)	&& defined(IOM_EXTERNAL_0_ENABLED)*/
};


uint8_t IOM_External_CommandBufferCntTable[EXTERNAL_NUM];


/* Static */
static uint8_t ExternalBoardCounter=0;

/* ******************* */
/* Functions Prototype */
/* ******************* */


/* Exported function */

/* Local Function */
static uint8_t LIB_IOM_External_PilotType2CmdBufferSize(uint8_t pilotType);



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */


/**
External outputs initialization.
*/
void LIB_IOM_External_Init(void)
{
	memset(IOM_External_CommandBufferCntTable, 0, sizeof(IOM_External_CommandBufferCntTable));
}

/**
\n Adds new external load to outputmanager

\param  [in] pInfoBuffer pointer to InfoBuffer allocated by Application layer.
\param  [in] szInfoBuffer size in bytes of the InfoBuffer.
\param  [in] loadPos Load's position inside Output Map


\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\par Example
Once defined info buffer and its size in bytes
\code
#define IOM_EXTERNAL_WP_INFOBUFFER_SIZE (32)

_DW_GLOBAL_ uint8_t Wash_Pump_InfoBuffer[IOM_EXTERNAL_WP_INFOBUFFER_SIZE];
\endcode

application level adds the load by specyfing load ID in the putput table map
\code
LIB_IOM_External_New ( &Wash_Pump_InfoBuffer[0], (uint8_t)IOM_EXTERNAL_WP_INFOBUFFER_SIZE,  Pump_Pos[LD_WASH_PUMP_IND]);
\endcode

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti

\attention you can add maximum IOM_NUM_OF_EXTERNAL_LOADS of external loads.
*/
LIB_IOM_ErrorType LIB_IOM_External_New( uint8_t* pInfoBuffer, uint8_t szInfoBuffer,  uint8_t loadPos)
{
#if !defined(CONFIG_EXT_OUTMNG_EMULATE_SETTING)
	struct Loader pilotTypeParametersPtr={0,0};
#endif
	uint8_t	*pByte;
	uint8_t		loadIndex;
	uint8_t		szCmd;
	uint8_t		pilotType;
	uint8_t		uart=0;
	uint8_t		szCmdBuffer;
	
	if(ExternalBoardCounter >= IOM_NUM_OF_EXTERNAL_LOADS)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_NUM_EXTERNAL_LOADS_REACHED_ERR);
		return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR; /* Serious Error */
	}

#if defined(CONFIG_EXT_OUTMNG_EMULATE_SETTING_PILOT_TYPE)
	pilotType = CONFIG_EXT_OUTMNG_EMULATE_SETTING_PILOT_TYPE_ID;
#else

	loadIndex = LIB_IOM_FindLoad(loadPos, &pByte);
	
	/* warning: pByte is modified by FinLoad and now point to the start addr. of load's descr.*/
	if ( loadIndex == LOAD_NOT_FOUND )
	{
	    return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;
	}
	
	pilotType = LIB_IOM_GetPilotType(PILOT_TYPE_FIELD(pByte));
#endif

#if !defined(CONFIG_EXT_OUTMNG_EMULATE_SETTING)
	/* get pointer to pylot type parameters */
	if (0xFF != PILOT_TYPE_PARAMS_OFFSET(pByte))
    {
        /* Pilot Parameters Available */
        if ( 0 != BasicLoader( SF_PTR_IO_DEVICE,PILOT_TYPE_PARAMS_OFFSET(pByte), &pilotTypeParametersPtr) )
        {
            PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_OUTPUT_PARAM_NOT_FOUND_ERR);
            return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;  /* Serious Error */            
        }
    }
	else
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_INVALID_PILOTTYPE_OFFSET_ERR);
        return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;  /* Serious Error */
	}
	
	if(!pilotTypeParametersPtr.DataLenght || (pilotTypeParametersPtr.Data==NULL))
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_INVALID_PILOTTYPE_PARAMS_ERR);
        return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;  /* Serious Error */
	}
	
	uart = (uint8_t)(EXTERNAL_PARAMS_UART(pilotTypeParametersPtr.Data)) - (uint8_t)EXTERNAL_BOARD_MIN;
#endif
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;  /* Serious Error */
	}
	
	szCmd=LIB_IOM_External_PilotType2CmdBufferSize(pilotType);
	
	szCmdBuffer=IOM_External_CommandBufferCntTable[uart];
	if(!szCmdBuffer)
	{
		szCmdBuffer++; /*byte reserved for Seq ID */
	}
	
	if((szCmdBuffer + szCmd + EXTERNAL_COMMAND_BUFFER_HDR_SZ) > IOM_External_CommandBufferTable[uart].szCmdBuffer )
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_TOOMANY_BYTES_CMDBUFFER_ERR);
        return LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR;  /* Serious Error */
	}
	
	
	IOM_External_LoadsTable[ExternalBoardCounter].pPilotTypeInfo	= pInfoBuffer;
	IOM_External_LoadsTable[ExternalBoardCounter].szPilotTypeInfo = szInfoBuffer;
	IOM_External_LoadsTable[ExternalBoardCounter].PilotType = pilotType;
	IOM_External_LoadsTable[ExternalBoardCounter].CmdOffset = szCmdBuffer;
	IOM_External_LoadsTable[ExternalBoardCounter].szCmd = szCmd;
#if !defined(CONFIG_EXT_OUTMNG_EMULATE_SETTING)
	IOM_External_LoadsTable[ExternalBoardCounter].Displacement = EXTERNAL_PARAMS_DISPL(pilotTypeParametersPtr.Data);
#else
	IOM_External_LoadsTable[ExternalBoardCounter].Displacement = ExternalBoardCounter;
#endif
	IOM_External_LoadsTable[ExternalBoardCounter].CommCh = uart;

	szCmdBuffer += (szCmd+EXTERNAL_COMMAND_BUFFER_HDR_SZ);
	IOM_External_CommandBufferCntTable[uart] = szCmdBuffer;
	
	ExternalBoardCounter++;

	return LIB_IOM_NO_ERROR;
}







/**
\n Writes the CommandBuffer with the command value relating to the specified PilotType.

\param  [in] pilotType PilotType of the load
\param  [in] pLoad pointer to GIOM_LoadStruct atructure containing the command


\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_SETINFOBUFFER_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti 
*/
LIB_IOM_ErrorType LIB_IOM_External_SetCommand(uint8_t pilotType, const GIOM_LoadStruct *const pLoad)
{
	uint8_t i=0;
	uint8_t uart;
	uint8_t	offset;
	uint8_t* pCmd;
	
	while((i<IOM_NUM_OF_EXTERNAL_LOADS))
	{
		if(pilotType ==  IOM_External_LoadsTable[i].PilotType)
		{
			break;
		}
		i++;
	}
	
	if(i>=IOM_NUM_OF_EXTERNAL_LOADS)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_NUM_EXTERNAL_LOADS_REACHED_ERR);
    	return LIB_IOM_EXTERNAL_SETCOMMAND_ERR;  /* Serious Error */
	}
	
	uart = IOM_External_LoadsTable[i].CommCh;
	offset = IOM_External_LoadsTable[i].CmdOffset;
	pCmd = IOM_External_CommandBufferTable[uart].pCmdBuffer + offset;
	UseExternalBoard();
	*pCmd = pilotType;
	*(pCmd+1) = IOM_External_LoadsTable[i].Displacement;	
	memcpy((pCmd+2), &pLoad->data[0], IOM_External_LoadsTable[i].szCmd);
	UnuseExternalBoard();
	
	return LIB_IOM_NO_ERROR;
}


/**
\n Sets a ram read request to command buffer

\param  [in] pDataRamReq pointer to read ram request parameters.
\param  [in] len size in bytes of the parameters

\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti
*/
LIB_IOM_ErrorType LIB_IOM_External_SetReadRamRequest(uint8_t * pDataRamReq, uint8_t len)
{
	uint8_t uart = 0;
	uint8_t* pCmd;
	
	if(len < 5)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MISSINGDATA_SETREADRAM_ERR);
        return LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR;  /* Serious Error */
	}
	
	uart = (uint8_t)(*(pDataRamReq+4) & 0x0F);
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR;  /* Serious Error */
	}
	
	pCmd = IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferCntTable[uart];
	
	if((IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferTable[uart].szCmdBuffer - 1) >= (pCmd+1) + IOM_EXT_CMDDATA_RAM_READ_SZ -1)
	{
		UseExternalBoard();
		*pCmd = 0; /* PilotType Reserved for read ram */
	
		(void)memcpy((uint8_t*)(pCmd+1), pDataRamReq, IOM_EXT_CMDDATA_RAM_READ_SZ);
		UnuseExternalBoard();
	
		ExternalSeqId[uart].NewReq= (uint8_t)(*(pDataRamReq+IOM_EXT_CMDDATA_RAM_READ_SZ) & 0xF0);
	}
	else
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_TOOMANY_BYTES_CMDBUFFER_ERR);
        return LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR;  /* Serious Error */
	}
	return LIB_IOM_NO_ERROR;
}


/**
\n Sets a write ram request to command buffer

\param  [in] pDataRamReq pointer to write ram request parameters.
\param  [in] len size in bytes of the parameters

\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_SETWRITERAMREQ_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti
*/
LIB_IOM_ErrorType LIB_IOM_External_SetWriteRamRequest(uint8_t * pDataRamReq, uint8_t len)
{
	uint8_t uart = 0;
	uint8_t* pCmd;
	
	if(len < 5)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MISSINGDATA_SETREADRAM_ERR);
        return LIB_IOM_EXTERNAL_SETREADRAMREQ_ERR;  /* Serious Error */
	}
	
	uart = (uint8_t)(*(pDataRamReq+4) & 0x0F);
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_SETWRITERAMREQ_ERR;  /* Serious Error */
	}
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_SETWRITERAMREQ_ERR;  /* Serious Error */
	}
	
	pCmd = IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferCntTable[uart];
	
	if((IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferTable[uart].szCmdBuffer - 1) >= (pCmd+1) +IOM_EXT_CMDDATA_RAM_WRITE_SZ -1)
	{
		UseExternalBoard();
		*pCmd = 0; /* PilotType Reserved for write ram */
		
		pCmd += IOM_EXT_CMDDATA_RAM_READ_SZ;
	
		(void)memcpy((uint8_t*)(pCmd+1), pDataRamReq, IOM_EXT_CMDDATA_RAM_WRITE_SZ);
		UnuseExternalBoard();
		ExternalSeqId[uart].NewReq= (uint8_t)(*(pDataRamReq+IOM_EXT_CMDDATA_RAM_WRITE_SZ) & 0xF0);
	}
	else
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_TOOMANY_BYTES_CMDBUFFER_ERR);
        return LIB_IOM_EXTERNAL_SETWRITERAMREQ_ERR;  /* Serious Error */
	}
	return LIB_IOM_NO_ERROR;
}

/**
\n Sets a extra request to command buffer

\param  [in] pDataRamReq pointer to write ram request parameters.
\param  [in] len size in bytes of the parameters

\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_SETEXTRAREQ_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti
*/
LIB_IOM_ErrorType LIB_IOM_External_SetExtraRequest(uint8_t * pDataExtraReq, uint8_t len)
{
	uint8_t uart = 0;
	uint8_t* pCmd;
	
	if(len < 3)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MISSINGDATA_SETREADRAM_ERR);
        return LIB_IOM_EXTERNAL_SETEXTRAREQ_ERR;  /* Serious Error */
	}
	
	uart = (uint8_t)(*(pDataExtraReq+2) & 0x0F);
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_SETEXTRAREQ_ERR;  /* Serious Error */
	}
	
	pCmd = IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferCntTable[uart];
	
	
	if((IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_CommandBufferTable[uart].szCmdBuffer - 1) >= (pCmd+1) +2 -1)
	{
		UseExternalBoard();
		
		*pCmd = 0; /* PilotType Reserved for write ram */
		pCmd += IOM_EXT_CMDDATA_RAM_READ_SZ + IOM_EXT_CMDDATA_RAM_WRITE_SZ;
		
		(void)memcpy((uint8_t*)(pCmd+1), pDataExtraReq, 2);
		UnuseExternalBoard();
	
		ExternalSeqId[uart].NewReq=(uint8_t)(*(pDataExtraReq+IOM_EXT_CMDDATA_EXTRA_REQ_SZ) & 0xF0);
	}
	else
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_TOOMANY_BYTES_CMDBUFFER_ERR);
        return LIB_IOM_EXTERNAL_SETEXTRAREQ_ERR;  /* Serious Error */
	}
	return LIB_IOM_NO_ERROR;
}

/**
\n Returns Command buffer for the specified external, if ready.
Command buffer becomes ready once  LIB_IOM_External_SetCommand has been called for each load.

\param  [in] uart external board index
\param  [out] ppCmd pointer to pointer to command buffer
\param  [out] pCmdLen pointer to size in bytes

\return 	LIB_IOM_ErrorType

\retval		LIB_IOM_NO_ERROR   	 no error
\retval		LIB_IOM_EXTERNAL_GETCMD_ERR (error, for detail use LIB_IOM_External_GetExternalOutMngError() function)

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti
*/
LIB_IOM_ErrorType LIB_IOM_External_GetCommandBuffer(uint8_t uart, uint8_t** ppCmd, uint8_t* pCmdLen)
{
	uint8_t* pCmd = IOM_External_CommandBufferTable[uart].pCmdBuffer;
    uint16_t    cmd_writeram = 0;
    uint16_t    cmd_readram = 0;
    uint16_t    cmd_extrareq = 0;
    uint8_t i=0;
    uint8_t *p;
    
	ExternalSeqId[uart].OldReq &= 0xF0;
	
	if(uart >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return LIB_IOM_EXTERNAL_GETCMD_ERR;  /* Serious Error */
	}
	
	UseExternalBoard();
    /* check if command buffer is ready */
	for(i=0; i< ExternalBoardCounter; i++)
	{
	    if(IOM_External_LoadsTable[i].CommCh == uart)
	    {
	        p=(IOM_External_CommandBufferTable[uart].pCmdBuffer + IOM_External_LoadsTable[i].CmdOffset);
			/* command buffer it's assumed to be not ready if there is at least one load not set*/
	        if((  *(p) != IOM_External_LoadsTable[i].PilotType)
	        && ( *(p + 1) != IOM_External_LoadsTable[i].Displacement))
	        {
	            *pCmdLen=0;
	            UnuseExternalBoard();
	            return LIB_IOM_EXTERNAL_GETCMD_ERR;
	        }
	    }
	}
	
	*pCmdLen = IOM_External_CommandBufferCntTable[uart]; /* */
	
    (void)memcpy(&cmd_writeram, 
                 (pCmd + IOM_External_CommandBufferCntTable[uart] + 1/*pilotype for ram*/ + IOM_EXT_CMDDATA_RAM_READ_SZ), 
                 sizeof(cmd_writeram));
    (void)memcpy(&cmd_readram, 
             (pCmd + IOM_External_CommandBufferCntTable[uart] + 1),
             sizeof(cmd_readram));
    (void)memcpy(&cmd_extrareq, 
                 (pCmd + IOM_External_CommandBufferCntTable[uart] + 1 + IOM_EXT_CMDDATA_RAM_READ_SZ + IOM_EXT_CMDDATA_RAM_WRITE_SZ), 
                 sizeof(cmd_extrareq));
    if(cmd_writeram)
	{
		/* write ram */
		*pCmdLen += 1+IOM_EXT_CMDDATA_RAM_READ_SZ+IOM_EXT_CMDDATA_RAM_WRITE_SZ; /*reserved pilotType +  read + write */
		ExternalSeqId[uart].OldReq |= 1;
	}
	else if(cmd_readram) 
	{
		/* read ram */
		*pCmdLen += 1+IOM_EXT_CMDDATA_RAM_READ_SZ; /*reserved pilotType +  read */
	}
	else if(cmd_extrareq) 
	{
		/* extra ram */
		*pCmdLen += 1+IOM_EXT_CMDDATA_RAM_READ_SZ+IOM_EXT_CMDDATA_RAM_WRITE_SZ+IOM_EXT_CMDDATA_EXTRA_REQ_SZ; /*reserved pilotType +  read + write + extra*/
		ExternalSeqId[uart].OldReq |= 2;
	}
	
	ExternalSeqId[uart].Seq++;
	
	*ppCmd = IOM_External_CommandBufferTable[uart].pCmdBufferTx;
	
	if((ExternalSeqId[uart].OldReq & 0xF0) != ExternalSeqId[uart].NewReq)
	{
		ExternalSeqId[uart].OldReq &= 0x0F;
		ExternalSeqId[uart].OldReq += ExternalSeqId[uart].NewReq;
		ExternalSeqId[uart].Seq=0;
	}
	
	*IOM_External_CommandBufferTable[uart].pCmdBuffer = ExternalSeqId[uart].OldReq;
	memcpy(IOM_External_CommandBufferTable[uart].pCmdBufferTx, IOM_External_CommandBufferTable[uart].pCmdBuffer, *pCmdLen);
	
	UnuseExternalBoard();
	
	return LIB_IOM_NO_ERROR;
}

/**
\n Returns Info buffer related to an Extended Load, if it's an External one

\param  [in] extloadItem		Extended Load Item
\param  [out] pByteData 		pointer to pointer to info buffer (destination buffer)
\param  [out] szData 			maximum size in bytes

\return 	bool_t

\retval		TRUE   	 			no error
\retval		FALSE				Error - extended board is not an external

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti
*/
bool_t LIB_IOM_External_GetInfoBuffer(uint8_t extloadItem, uint8_t *pByteData, uint8_t szData)
{
	bool_t resVal = FALSE;
	uint8_t extBoardPos = LIB_IOM_GetExtBoardPos(extloadItem);

	if (extBoardPos != LOAD_NOT_FOUND)
	{
		uint8_t maxSize = MIN(IOM_External_LoadsTable[extBoardPos].szPilotTypeInfo, szData);

		memcpy(pByteData, IOM_External_LoadsTable[extBoardPos].pPilotTypeInfo, maxSize);
		resVal = TRUE;
	}

	return resVal;

}

/**
\n Returns Common buffer related to an UART channel

\param  [in] commCh		UART channel
\param  [out] pCmnData 		pointer to pointer to common buffer (destination buffer)
\param  [out] szData 			maximum size in bytes

\return 	bool_t

\retval		TRUE   	 			no error
\retval		FALSE				Error - extended board is not an external

\author		Alessandro Sabbatini

\date		02/09/2014

\version    1.0.0

\note       \par \b Nota1:
            documentation by Alessandro Sabbatini
*/
bool_t LIB_IOM_Ext_GetCommonBuffer(uint8_t commCh, uint8_t *pCmnData, uint8_t szData)
{
	bool_t resVal = FALSE;

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) && defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)

#if defined(IOM_EXTERNAL_0_ENABLED)
	if (commCh == EXTERNAL_0)
	{
		uint8_t maxSize = MIN(sizeof(ExternalGlobalData_0), szData);

		memcpy(pCmnData, ExternalGlobalData_0, maxSize);
		resVal = TRUE;
	}
#endif
#if defined(IOM_EXTERNAL_1_ENABLED)
	if (commCh == EXTERNAL_1)
	{
		uint8_t maxSize = MIN(sizeof(ExternalGlobalData_1), szData);

		memcpy(pCmnData, ExternalGlobalData_1, maxSize);
		resVal = TRUE;
	}
#endif

#endif

	return resVal;

}



/**
\n Returns UART channel of an Extended load - if external

\param  [in] commCh		UART channel

\return 	uint8_t

\retval		LOAD_NOT_FOUND		Load is not external
\retval		!= LOAD_NOT_FOUND	UART channel

\author		Alessandro Sabbatini

\date		02/09/2014

\version    1.0.0

\note       \par \b Nota1:
            documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_Ext_GetExtdLoadComChan(uint8_t extloadItem)
{
	uint8_t retVal = EXTERNAL_NUM;
	uint8_t extBoardPos = LIB_IOM_GetExtBoardPos(extloadItem);

	if (extBoardPos != LOAD_NOT_FOUND)
	{
		retVal = IOM_External_LoadsTable[extBoardPos].CommCh;
	}

	return retVal;
}

/**
\n Returns last External Board counter set

\param  	none

\return 	uint8_t				ExternalBoardCounter value

\author		Alessandro Sabbatini

\date		02/09/2014

\version    1.0.0

\note       \par \b Nota1:
            documentation by Alessandro Sabbatini
*/
uint8_t LIB_IOM_Ext_GetBoardCounter(void)
{
	return ExternalBoardCounter;
}

/**
\n Returns the load's status value decoding the command buffer value

\param  [in] pilotType PilotType of the load
\param  [in] pLoad pointer to GIOM_LoadStruct structure containing the command

\return 	bool_t

\retval		FALSE load is inactive
\retval		TRUE load is active

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti 
*/
bool_t	LIB_IOM_External_GetLoadStatus(uint8_t pilotType, const GIOM_LoadStruct *const pLoad)
{
	bool_t bLoadStatus=FALSE;
	
	switch(pilotType)
	{
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
		case EXTERN_MODULE_L:
		{
			ExternalBldcWashCmdType* tmpPtr = (ExternalBldcWashCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
		}
			break;
#endif /* defined(CONFIG_EXTERN_MODULE_L_SUPPORT) */
#if defined(CONFIG_PMSM_MOTOR_L_SUPPORT)
		case PMSM_MOTOR_L:
		{
			ExternalBldcWashCmdType* tmpPtr = (ExternalBldcWashCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
		}
			break;
#endif
#if defined(CONFIG_BLDC_WASH_L_SUPPORT)
		case BLDC_WASH_L:
		{
			ExternalBldcWashCmdType* tmpPtr = (ExternalBldcWashCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
		}
			break;
#endif
#if defined(CONFIG_BLDC_DRAIN_L_SUPPORT)
		case BLDC_DRAIN_L:
		{
			ExternalBldcDrainCmdType* tmpPtr = (ExternalBldcDrainCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
		}
			break;
#endif
#if defined(CONFIG_PMSM_DRYER_L_SUPPORT)
		case PMSM_DRYER_L:
		{
			ExternalBldcDrainCmdType* tmpPtr = (ExternalBldcDrainCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
		}
			break;
#endif
#if defined(CONFIG_MULTIPOWER_COMP_L_SUPPORT)
	    case MULTIPOWER_COMP_L:
	    {
	    	ExternalBldcDrainCmdType* tmpPtr = (ExternalBldcDrainCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
	    }
			break;
#endif /* defined(MULTIPOWER_COMP_L_SUPPORT) */
#if defined(CONFIG_BRUSHLESS_HOOD_L_SUPPORT)
		case BRUSHLESS_HOOD_L:
		{
			ExternalBldcDrainCmdType* tmpPtr = (ExternalBldcDrainCmdType*) &pLoad->data[0];

			if(tmpPtr->rpm)
			{
				bLoadStatus=TRUE;
			}
			break;
		}
#endif
#if defined(CONFIG_AIRSENSOR_HOOD_L_SUPPORT)
		case AIRSENSOR_HOOD_L:
			/* as it's a sensor it's always active */
			bLoadStatus=TRUE;
			break;
#endif
	}
	
	return bLoadStatus;
}





/**
\n Returns the load's status value decoding the connad value

\param  [in] pilotType PilotType of the load
\param  [in] pLoad pointer to GIOM_LoadStruct atructure containing the command

\return 	bool_t

\retval		FALSE load is inactive
\retval		TRUE load is active

\author		Roberto Fioravanti

\date			13/09/2010

\version    1.0.0

\note       \par \b Nota1:
            documentation by Roberto Fioravanti 
*/
void	LIB_IOM_External_ProcessInfoPacketData(uint8_t ch, uint8_t* pInfoPacketData, uint8_t szInfoPacketData)
{
	uint8_t offset=0;
	uint8_t	rem = szInfoPacketData;
	uint8_t size;
	uint8_t pilotType;
	uint8_t i;
	
	if(ch >= EXTERNAL_NUM)
	{
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_MAX_CHANNEL_NUM_REACHED_ERR);
        return;  /* Serious Error */
	}

	*(ExternalData[ch].pRamBuffer) = ExternalSeqId[ch].Seq;	/* seqID*/
	*(ExternalData[ch].pRamBuffer+1) = *pInfoPacketData;	/* ReqID*/
	
	pInfoPacketData +=1;
	rem--;
	/*parse PiloTypeInfo data*/
	
	
	while (rem)
	{
		pilotType=((InfoPacketDataHdrType*)pInfoPacketData)->pilotType;
		switch(pilotType)
		{
			case 0x00:
			/* treats the pilotType 0 as read ram data */
			
			/* check if the sub field has data
			if szData is 0 the field is not valid
			*/
			if(((InfoPacketDataHdrType*)pInfoPacketData)->szData)
			{
			    
				offset= (uint8_t)(((InfoPacketDataHdrType*)pInfoPacketData)->szData + 1);
				size = (uint8_t) ((((InfoPacketDataHdrType*)pInfoPacketData)->szData -1 <= (ExternalData[ch].szRamBuffer-2))? 
					((InfoPacketDataHdrType*)pInfoPacketData)->szData -1 :
					(ExternalData[ch].szRamBuffer-2));

			
				if(size > ExternalData[ch].szRamBuffer - 2)
				{
					size =  (uint8_t)(ExternalData[ch].szRamBuffer - 2);
				}

				UseExternalInfo();
				memcpy(ExternalData[ch].pRamBuffer+2/*seqID+toolid*/, pInfoPacketData+2, size);
				UnuseExternalInfo();

				pInfoPacketData += offset;
				rem -= offset;
			}
			else 
		    {
			    rem =0;
			}
			break;
#if defined(CONFIG_EXTERNAL_GLOBAL_DATA_BLOCK_SUPPORT)
			case 0xFF:
				/* treats the pilotType 0xFF as GLOBAL data */

				if(((InfoPacketDataHdrType*)pInfoPacketData)->szData)
				{

					offset= (uint8_t)(((InfoPacketDataHdrType*)pInfoPacketData)->szData + 1);
					size = (uint8_t) ((((InfoPacketDataHdrType*)pInfoPacketData)->szData -1 <= (ExternalData[ch].szGlobal))?
						((InfoPacketDataHdrType*)pInfoPacketData)->szData -1 :
						(ExternalData[ch].szGlobal));


					if(size > ExternalData[ch].szGlobal - 2)
					{
						size =  (uint8_t)(ExternalData[ch].szGlobal - 2);
					}

					UseExternalInfo();
					memcpy(ExternalData[ch].pGlobalData, pInfoPacketData+2, size);
					UnuseExternalInfo();

					pInfoPacketData += offset;
					rem -= offset;
				}
				else
				{
					rem =0;
				}

				break;
#endif
			default:
			{
				
				i=0;
				
				/*search for PilotType */
				while((i<IOM_NUM_OF_EXTERNAL_LOADS))
				{
					if(pilotType ==  IOM_External_LoadsTable[i].PilotType)
					{
						break;
					}
					i++;
				}
	
				if(i>=IOM_NUM_OF_EXTERNAL_LOADS)
				{
					PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_INVALID_PILOTTYPE_ERR);
			    	return;  /* Serious Error */
				}
	
				offset=(uint8_t)(((InfoPacketDataHdrType*)pInfoPacketData)->szData + 1);
				size = (uint8_t)(((InfoPacketDataHdrType*)pInfoPacketData)->szData - 1);
				
				if (size > IOM_External_LoadsTable[i].szPilotTypeInfo)
				{
					size = IOM_External_LoadsTable[i].szPilotTypeInfo;
				}
				
				UseExternalInfo();				
				memcpy(IOM_External_LoadsTable[i].pPilotTypeInfo, pInfoPacketData+2, size);
				UnuseExternalInfo();				
				
				pInfoPacketData += offset;
				rem -= offset ;
			}
		}
	}

}



/* Local Function */
static uint8_t LIB_IOM_External_PilotType2CmdBufferSize(uint8_t pilotType)
{
	uint8_t cmdsize=0;
	switch(pilotType)
	{
#if defined(CONFIG_EXTERN_MODULE_L_SUPPORT)
		case EXTERN_MODULE_L:
			cmdsize = EXTERN_MODULE_L_CMD_SZ;
			break;
#endif /* defined(CONFIG_EXTERN_MODULE_L_SUPPORT) */
#if defined(CONFIG_PMSM_MOTOR_L_SUPPORT)
		case PMSM_MOTOR_L:
			cmdsize = PMSM_MOTOR_L_CMD_SZ;
			break;
#endif /* defined(CONFIG_PMSM_MOTOR_L_SUPPORT) */
#if defined(CONFIG_BLDC_WASH_L_SUPPORT)
		case BLDC_WASH_L:
			cmdsize = BLDC_WASH_L_CMD_SZ;
			break;
#endif /* defined(CONFIG_BLDC_WASH_L_SUPPORT)*/
#if defined(CONFIG_BLDC_DRAIN_L_SUPPORT)
		case BLDC_DRAIN_L:
			cmdsize = BLDC_DRAIN_L_CMD_SZ;
			break;
#endif /* defined(CONFIG_BLDC_DRAIN_L_SUPPORT)*/
#if defined(CONFIG_PMSM_DRYER_L_SUPPORT)			
		case PMSM_DRYER_L:
			cmdsize = PMSM_DRYER_L_CMD_SZ;
			break;
#endif /* defined(CONFIG_PMSM_DRYER_L_SUPPORT) */
#if defined(CONFIG_MULTIPOWER_COMP_L_SUPPORT)			
	    case MULTIPOWER_COMP_L:
	        cmdsize = MULTIPOWER_COMP_L_CMD_SZ;
	        break;
#endif /* defined(CONFIG_MULTIPOWER_COMP_L_SUPPORT) */
#if defined(CONFIG_BRUSHLESS_HOOD_L_SUPPORT)			
	    case BRUSHLESS_HOOD_L:
	        cmdsize = BRUSHLESS_HOOD_L_CMD_SZ;
	        break;
#endif /* defined(CONFIG_BRUSHLESS_HOOD_L_SUPPORT) */
#if defined(CONFIG_AIRSENSOR_HOOD_L_SUPPORT)			
	    case AIRSENSOR_HOOD_L:
	        cmdsize = AIRSENSOR_HOOD_L_CMD_SZ;
	        break;
#endif /* defined(AIRSENSOR_HOOD_L_SUPPORT) */
#if defined(CONFIG_PROBES_AND_SENSORS_L_SUPPORT)
	    case PROBES_ADN_SENSORS_L:
	        cmdsize = PROBES_AND_SENSORS_L_CMD_SZ;
	        break;
#endif /* defined(AIRSENSOR_HOOD_L_SUPPORT) */
		default:
		PLT_ERROR_WARNING(PLT_ERROR_IOM_EXT_OUTMNG_INVALID_PILOTTYPE_ERR);
	}
	return cmdsize;
}



#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */
/* *********** */
/* End of file */
/* *********** */

