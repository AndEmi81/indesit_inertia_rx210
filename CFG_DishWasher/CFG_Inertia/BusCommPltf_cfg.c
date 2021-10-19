/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	CP_CFG Communication Platform Configuration
@{
\par		Description
\n		It contains the EmbOS-based Communication platform configuration.
@}
\ingroup WizardPrj
*/

/**
\par		Description
\n		Application specific variables (Buffers and structures) and methods for Communication Platform
\file		BusCommPltf_cfg.c
\ingroup	CP_CFG
\date		22/01/2011
\version	01.00.00 
\author		Roberto Fioravanti
\n
\n
\par		History
\n
\n 		00.00.64
\n		22/01/2011
\n		developper Roberto Fiorvanti
\n		changes 
\n 		- creation
\n
*/

/* ******** */
/* Includes */
/* ******** */

#include "StandardDefines.h"
#include "StandardTypes.h"

#include "BusCommPltf_cfg.h"
#include "BCP_BusCommMng.h"

#include "LIB_Reset.h"
#include "OutputManagerExp.h"

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#include "ExternalOutputManagerExp.h" 
#endif

#include "Main.h"
#include "MainInterface.h"
#include "callback.h"


#ifdef BUS_COM_PLT_I2C_SUPPORT
#include "OCI2C_drv.h"
#endif /* BUS_COM_PLT_I2C_SUPPORT */

#ifdef BUS_COM_PLT_UART_SUPPORT
#include "OCUART_drv.h"
#endif /* BUS_COM_PLT_UART_SUPPORT */

#if 	defined(I2C_OCDMA_SUPPORT) || defined(UART_OCDMA_SUPPORT)
#include "OCDMA_drv.h"
#endif
#include "OCFLS_drv.h"

#include "Lib_Utilities.h"

#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
#include "PM_PowerProfile.h"
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

#if defined(CONFIG_PM_ENERGY_METER_SW)
#include "PM_EnergyMeterSw.h"
#endif /*defined(CONFIG_PM_ENERGY_METER_SW)*/

#include "Tools.h"

#include "LIB_ClassBVar.h"

extern bool_t CBF_CurrentLoadedWater(uint32_t * LoadedWater); /*AE TODO LOCAL FOR NOW*/

/* ****************** */
/* Defines and Macros */
/* ****************** */


#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/* External board protocol */
#define EXT_PROT_CMD_SID_SIZE				              (1)
#define EXT_PROT_TOOL_ID_REQ_SIZE		                  (1)
#endif	/* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */

/**
EmbOS CP task events */
enum OS_CP_Events{
/**
I2C
*/
MAIN_OS_EVENT_I2C_BIT=0,
/**
Uart
*/
MAIN_OS_EVENT_UART_BIT
};


#define MAIN_OS_EVENT_I2C     ((OS_TASK_EVENT)(1<<MAIN_OS_EVENT_I2C_BIT))
#define MAIN_OS_EVENT_UART    ((OS_TASK_EVENT)(1<<MAIN_OS_EVENT_UART_BIT))


/* *****************************************
**	Extended Types Data buffer dimension
*/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */


/* Exported */

/* Vpeak normalized for Energy Meter => nominal is 230V */
uint32_t	normVpeak = (((230 * 1414) / 1000 ) * 65535 / 444 );

/* Static */
/**
@name Memory remap tables
Tables to adapt 16-bit address coming from protocol to 32-bit memory space.
\par Property
(target depentent)
@{
*/
/**
remap table for 'read command'
\li 16-bit addesses from 0x0000 to 0x3FFF are mapped to ram
\li 16-bit addesses from 0x4000 to 0xFFFF are mapped to rom
*/
#if defined(FREESCALE_MCF51_AG128)
static PP_RemapTableType const RFRemapTable[3]=
{
  REMAP_READ_RAM_BLOCK,
  REMAP_READ_BOOT_ROM_BLOCK,
  REMAP_READ_ROM_BLOCK
};
#endif
#if defined(NXP_LPC12xx_1227)
static PP_RemapTableType const RFRemapTable[2]=
{
  REMAP_READ_RAM_BLOCK,
  REMAP_READ_ROM_BLOCK
};
#endif
#if defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)
static PP_RemapTableType const RFRemapTable[4]=
{
	{ REMAP_READ_RAM_BLOCK },
	{ REMAP_READ_BOOT_ROM_BLOCK },
	{ REMAP_READ_ROM_BLOCK },
	{ REMAP_READ_DATA_BLOCK }
};

static PP_MemoryRangeType const ProtocolReadingAccessTable[3]=
{
	{RAM_AREA_START_ADDRESS, 		RAM_AREA_START_ADDRESS+RAM_SIZE-1}, 				/*RAM*/
	{FLASH_DATA_BASE_ADDRESS, 		FLASH_DATA_BASE_ADDRESS+FLASH_DATA_SIZE-1}, 		/*DATA FLASH*/
	{ROM_AREA_START_ADDRESS, 		ROM_AREA_START_ADDRESS+(ROM_SIZE-1) }, 				/*ROM (includes fw, boot)*/
};

static PP_MemoryRangeType const ProtocolWritingRamAccessTable[1]=
{
	{RAM_AREA_START_ADDRESS, 		RAM_AREA_START_ADDRESS+RAM_SIZE-1}, 				/*RAM*/
};

static PP_MemoryRangeType const ProtocolWritingAccessTable[3]=
{
	{BACKUPAREA_START_ADDR, 		BACKUPAREA_START_ADDR+BACKUPAREA_SIZE_IN_BYTES-1}, 	/*BACKUP, DATA FLASH*/
	{APPL_WBOX_ADDRESS,      		APPL_WBOX_END_ADDRESS},								/*WBOX, DATA FLASH*/
	{APPL_SETTING_ADDRESS,   		APPL_SETTING_END_ADDRESS  }, 						/*ROM*/
};

static PP_MemoryRangeType const ProtocolErasingAccessTable[2]=
{
	{BACKUPAREA_START_ADDR, 		BACKUPAREA_START_ADDR+BACKUPAREA_SIZE_IN_BYTES-1}, 	/*BACKUP, DATA FLASH*/
	{APPL_SETTING_ADDRESS,   		APPL_SETTING_END_ADDRESS  }, 						/*ROM*/
};
#endif

/**
remap table for 'write ram' command
*/
static PP_RemapTableType const WRRemapTable[1]=
{
  { REMAP_WRITE_RAM_BLOCK }
};


/*@}*/


/**
	@name Communication Platform Data
	@{
*/

/** driver I2C communication buffer */
static uint8_t BCP_I2CTxBuffer[BCP_I2C_TX_BUF_SIZE];

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/** driver UART communication buffer */
static uint8_t BCP_UARTRxBuffer[BCP_UART_TX_BUF_SIZE];
#define BCP_UARTTxBuffer BCP_UARTRxBuffer
#endif /*defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)*/
 


/** Communication Platform tasks's stack */
static STACK_SEC OS_STACKPTR int CP_CommunicationTaskStack[128] STACK_SEC_POST;
/** Communication Platform Task Contro Block*/
static OS_TASK CP_CommunicationTaskTCB;

#if defined(CONFIG_MAIN_PROTOCOL_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_ProtocolStackMonitor;
#endif

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/** Communication Platform tasks's stack */
static STACK_SEC OS_STACKPTR int CP_UartParserTaskStack[80] STACK_SEC_POST;
/** Communication Platform Task Control Block*/
static OS_TASK CP_UartParserTaskTCB;

#if defined(CONFIG_MAIN_UARTPARSER_STACK_MONITOR)
static 	TOOLS_STACKUSAGE_MONITOR	Main_UartParserStackMonitor;
#endif

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)*/

/**Appliance data table */
static CPApplianceDataTableType CPApplianceDataTable[CP_APPLDATA_NUM];
/*@}*/

/* Communication Channels table */
const BCP_ChannelTableType BCP_ChannelTable[BCP_CH_NUM]=
{ 
#ifdef BUS_COM_PLT_I2C_SUPPORT
  {(PP_MasterProtocolTableType*)NULL, OCI2C_InitSync, OCI2C_TxSync, OCI2C_RxSync, BCP_I2CTxBuffer, BCP_I2CRxBuffer, BCP_I2C_TX_BUF_SIZE, BCP_I2C_RX_BUF_SIZE}, /* BCP_I2C_CH_ID */
#endif /* BUS_COM_PLT_I2C_SUPPORT */
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#ifdef BUS_COM_PLT_UART_SUPPORT
  {(PP_MasterProtocolTableType*)NULL, OCUART_InitSync, OCUART_TxSync, OCUART_RxSync, BCP_UARTTxBuffer, BCP_UARTRxBuffer, BCP_UART_TX_BUF_SIZE, BCP_UART_RX_BUF_SIZE}, /* BCP_UART1_CH_ID */
#endif /* BUS_COM_PLT_UART_SUPPORT */
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */

};

/**
@name Resources
@{
*/

static OS_RSEMA SemaRegs;             /* Resource semaphore for regulations */
static OS_RSEMA SemaUiInfo;           /* Resource semaphore for UiInfo */


/*@}*/


#if	defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
static const uint8_t PowerProfileError=0xFF;
static PM_PowerProfile_PowerProfileNumType num;
#endif	/* CONFIG_PM_POWER_PROFILE_SUPPORT*/

#if defined(CONFIG_PM_ENERGY_METER_SW)
uint8_t PowerMeterBuffer[10];
#endif /*defined(CONFIG_PM_ENERGY_METER_SW)*/

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
static uint8_t ExternalBoardCommStatus;
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)*/

/* ******************* */
/* Functions Prototype */
/* ******************* */

static void CommunicationProtocolTask (void);

/**
Communication Platform Slave manager
*/
static DECLARE_CP_SLAVEMANAGER(CP_SlaveManager);

static DEF_CSM_EE_CTRL(Ctrl_IdentRequest);

static DEF_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock);

static DEF_CSM_EE_CTRL(Ctrl_ReadRegsRequest);
static DEF_CSM_EE_CTRL(Ctrl_StatusRequest);
static DEF_CSM_EE_CTRL(Ctrl_WriteRegsRequest);
static DEF_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest);


DEF_CSM_EE_CUSTOM_FUNC(BS_SetLoads);
DEF_CSM_EE_CUSTOM_FUNC(EE_SystemMode);
DEF_CSM_EE_CUSTOM_FUNC(EE_SetLoads);

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest);

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpGetInfo);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpSetInfo);

void CP_UartParserTask(void);
#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
static void EndOfPowerProfileRequest(void);
DEF_CSM_EE_CUSTOM_FUNC(EE_PowerProfileReq);
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */

#if defined(CONFIG_PM_ENERGY_METER_SW)
DEF_CSM_EE_CUSTOM_FUNC(EE_ReadPowerMeter);
#endif /*defined(CONFIG_PM_ENERGY_METER_SW)*/

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Allows the CP to access the data of the application during one of the following request:
\li PP_EE_TYPE_IDENT
\li PP_EE_TYPE_READ_STATUS
\li PP_EE_TYPE_READ_REGS
\li PP_EE_TYPE_WRITE_REGS
\li PP_EE_TYPE_WRITE_UIINFO

PP_EE_TYPE_IDENT is preconfigured by Platform framework.

\param[in]  index item index, see preconfigured CPApplianceDataTableIndexType
\param[in]  pData pointer to CPApplianceDataTableType


\par Example
\code
    CPApplianceDataTableType ident= {(void*)&IdentTable, sizeof(IdentTable)};
    
    Set_CPApplianceDataItem(CP_APPLDATA_IDENT, &ident);
\endcode
*/
void Set_CPApplianceDataItem(CPApplianceDataTableIndexType eIndex, CPApplianceDataTableType* pData)
{
    if(eIndex < (CPApplianceDataTableIndexType)(sizeof(CPApplianceDataTable)/sizeof(CPApplianceDataTable[0])))
    {
        CPApplianceDataTable[eIndex] = *pData;
    }
}



/**
Avoids conflicts caused by simultaneous access to regulations

\par Example
\code

\endcode
*/
void UseRegs(void) 
{
  OS_Use(&SemaRegs);
}

/**
Avoids conflicts caused by simultaneous access to regulations

\par Example
\code

\endcode
*/
void UnuseRegs(void) 
{
  OS_Unuse(&SemaRegs);
}

/**
Avoids conflicts caused by simultaneous access to UiInfo

\par Example
\code

\endcode
*/
void UseUiInfo(void) 
{
  OS_Use(&SemaUiInfo);
}

/**
Avoids conflicts caused by simultaneous access to UiInfo

\par Example
\code

\endcode
*/
void UnuseUiInfo(void) 
{
  OS_Unuse(&SemaUiInfo);
}




#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
/**
Returns if communication with External Board is active

\par Example
\code

\endcode
*/
uint8_t CP_GetExternalBoardCommStatus(void)
{
  uint8_t val;

  /*
  UseExternalBoard();
  needed if not atomic operation */
  val = ExternalBoardCommStatus;
  /*UnuseExternalBoard();*/
  
  return val;
}



/**
Signals communication with External Board as not active

\par Example
\code

\endcode
*/
void CP_ResetExternalBoardCommStatus(void)
{
  /*
  UseExternalBoard();
  needed if not atomic operation
  */
  ExternalBoardCommStatus = 0;
  /*
  UnuseExternalBoard();
  */
}

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */



/* End of slave */		
BEGIN_BCP_EOSN()
	BEGIN_BCP_EOSN__EE()
		BCP_EOSN__EE(PP_EE_TYPE_READ_STATUS,    CBF_OnReadStatusCompleted)
		BCP_EOSN__EE(PP_EE_TYPE_READ_REGS,      CBF_OnReadRegulationsCompleted)
		END_BCP_EOSN__EE()
	BEGIN_BCP_EOSN__EE_RX()
		BCP_EOSN__EE(PP_EE_TYPE_WRITE_UIINFO,   CBF_OnWriteUIInfoCompleted)
	END_BCP_EOSN__EE()
END_BCP_EOSN()


/**
Communication Platform callback: called in the driver interrupt by platform protocol
\param [in] ch communication channel
\param [in] notif, not used
*/
void ActionComm(IO_ChannelType ch, IO_NotificationType notif) 
{
    (void) notif;
    
	if(ch == (IO_ChannelType)(CP_I2C_CH_ID))
	{
  	    OS_SignalEvent(MAIN_OS_EVENT_I2C, &CP_CommunicationTaskTCB);
	}
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
        if(ch == (IO_ChannelType)(CP_UART1_CH_ID))
        {
            OS_SignalEvent(MAIN_OS_EVENT_UART, &CP_CommunicationTaskTCB);
        }
#endif         
}




/* Local Function */

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
static void CP_SetExternalBoardCommStatus(void)
{
  /*
  UseExternalBoard();
  needed if not atomic operation
  */
  ExternalBoardCommStatus = 1;
  /*
  UnuseExternalBoard();
  */
}


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest)
{
    (void) ch;
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetExtraRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest)
{
	(void) ch;
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetReadRamRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest)
{
    (void) ch;
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetWriteRamRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest)
{
    (void) ch;
	LIB_IOM_External_ProcessInfoPacketData(0, pdata, len);

	/* Update norm. Vpeak - get from DSP at bytes 6-7 then multiplied by 2 due to different normalization */
	normVpeak = GETW_BE(ExternalGlobalData_0+6) * 2;

	pinfo->error=IO_E_OK;
    CP_SetExternalBoardCommStatus();
}

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest)
{
    
	uint8_t* pCmd=0;
	uint8_t  cmdlen=0;
	(void) ch;
	(void) pdata;
	(void) len;
	pinfo->error=IO_E_INVALID_VALUE;

	if(LIB_IOM_NO_ERROR == LIB_IOM_External_GetCommandBuffer(0, &pCmd, &cmdlen))
	{
		pinfo->len=cmdlen;
		if(pinfo->len)
		{
			pinfo->error=IO_E_OK;
			pinfo->address= (IO_MemPtrType)pCmd;
			pinfo->option |= PP_SL_RESP_ANSW_OPT;
		}
	}
}

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */


#ifdef CONFIG_PM_POWER_PROFILE_SUPPORT
static void EndOfPowerProfileRequest(void)
{
	UnusePowerProfile();
}
DEF_CSM_EE_CUSTOM_FUNC(EE_PowerProfileReq)
{
	PM_PowerProfile_EnergyPhaseType* pEnergyPhase= (PM_PowerProfile_EnergyPhaseType*)NULL;
	IO_MemPtrType pN= (IO_MemPtrType)NULL;

	/* request requires answer */
	pinfo->option |= PP_SL_RESP_ANSW_OPT;

	/*get number of power profiles */
	num = PM_PowerProfile_GetPowerProfilesNum();

	if(!pdata[0])
	{
		/* Answer: total number of power profiles */
		pinfo->address= &num;
		pinfo->len=sizeof(PM_PowerProfile_PowerProfileNumType);
	}
	else
	{
		PM_PowerProfile_PowerProfileSizeType anslen = 0;

		if(num < pdata[0])
		{
			/* not existing such a power profile ID*/
			pinfo->len=1;
			pinfo->address = (IO_MemPtrType)&PowerProfileError;
		}
		else
		{
			UsePowerProfile();
			PM_PowerProfile_GetPowerProfileByProfileID((PM_PowerProfile_ProfileIDType)pdata[0], &pEnergyPhase, &anslen);

			if(anslen > 0)
			{
				pinfo->address = (IO_MemPtrType)pEnergyPhase;
				pinfo->len=anslen;
			}
			else
			{
				pinfo->len=1;
				pinfo->address = (IO_MemPtrType)&PowerProfileError;
			}

			pinfo->callback = (CP_Callback)EndOfPowerProfileRequest;
		}
	}

	pinfo->error=IO_E_OK;
}
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */


#if defined(CONFIG_PM_ENERGY_METER_SW)
DEF_CSM_EE_CUSTOM_FUNC(EE_ReadPowerMeter)
{
	uint16_t instantP;
	uint32_t avgEnergyWh, elapsedTime;
	uint32_t loadedWater = 0;
	uint8_t err=0;

	/* zero fill */
	memset(PowerMeterBuffer, 0, sizeof(PowerMeterBuffer));

	pinfo->error = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;

	if(EMS_E_OK != PowerMeter_GetInstant_P(&instantP))
	{
		err+=1;
	}

	if(EMS_E_OK != PowerMeter_GetAvgSinceStart_E_Wh(&avgEnergyWh, &elapsedTime))
	{
		err+=1;
	}

	if(S_OK_PLAT != CBF_CurrentLoadedWater(&loadedWater))
	{
		err+=1;
	}

	if(0 == err)
	{
		/*TODO: prefer Endian macro (instant P is BE) */
		PowerMeterBuffer[0]=(uint8_t)(instantP>>8);
		PowerMeterBuffer[1]=(uint8_t)(instantP&0xFF);
		/*TODO: prefer Endian macro (avg energy P is BE) */
		PowerMeterBuffer[2]=(uint8_t)(avgEnergyWh>>24);
		PowerMeterBuffer[3]=(uint8_t)(avgEnergyWh>>16);
		PowerMeterBuffer[4]=(uint8_t)(avgEnergyWh>>8);
		PowerMeterBuffer[5]=(uint8_t)(avgEnergyWh&0xFF);

		/*AE TODO ONLY LOCAL loadeWater*/
	    PowerMeterBuffer[6]=(uint8_t)(loadedWater>>24);
	    PowerMeterBuffer[7]=(uint8_t)(loadedWater>>16);
	    PowerMeterBuffer[8]=(uint8_t)(loadedWater>>8);
	    PowerMeterBuffer[9]=(uint8_t)(loadedWater&0xFF);

		pinfo->len = sizeof(PowerMeterBuffer);
		pinfo->address= (IO_MemPtrType)PowerMeterBuffer;
		pinfo->error = (IO_ErrorType)IO_E_OK;
		pinfo->option|=PP_SL_RESP_ANSW_OPT;
	}
	else
	{
		pinfo->error = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
	}
}
#endif /* defined(CONFIG_PM_ENERGY_METER_SW)*/

/*
uint8_t GetExpansionSetInfoBuffer_Index( uint8_t *DataBuffer)
{
  uint8_t index = PP_S_NUM_REQ;
  
  index = PP_S_EXP_WCMD_REQ;
  return index;
}
*/

/**
Extended SetLoad Request
The request will end successfully, Platform Protocol PP_ACK will be sent, if:
\li __GENERICIOMANAGER_H is defined
\li current system mode is SYSTEM_NORMAL_MODE
\li no error on queue at low level.
*/
DEF_CSM_EE_CUSTOM_FUNC(EE_SetLoads)
{
    (void) ch;
    
#ifdef __GENERICIOMANAGER_H     
    
	if (LIB_Reset_GetSystemMode() == SYSTEM_NORMAL_MODE)
	{
		if (SAFETY_CHECK_ISDISABLED())
		{
			if( Main_RemoteLoadsControl_PutCommand(REMOTE_LOADS_CONTROL_EXTENDED,len, pdata))
			{
			    pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
			}
		}
		else
		{
		    pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
		}
	}	

#else

	pinfo->error =(IO_ErrorType)IO_E_INVALID_VALUE;

#endif /*__GENERICIOMANAGER_H*/
}

/**
Setting Data block Request
The request will end successfully, Platform Protocol packet with the reqested data will be sent, 
if data exist (no error is returned  by BasicLoader)
*/
DEF_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock)
{
    Loader_Struct Data;
    pinfo->error = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
    
    (void) ch;    
    (void) len;    
    
    
    if(BSLDR_OK == BasicLoader(pdata[0], pdata[1], &Data))
    {
        pinfo->len = (uint16_t) (Data.DataLenght*2);
		pinfo->address= (IO_MemPtrType)Data.Data;
        pinfo->error = (IO_ErrorType)IO_E_OK;
		pinfo->option|=PP_SL_RESP_ANSW_OPT;
    }
}


/**
System mode Request
The request will end successfully, Platform Protocol PP_ACK will be sent if,
system mode is recognized.
*/
DEF_CSM_EE_CUSTOM_FUNC(EE_SystemMode)
{
    (void) ch;    
    
    pinfo->error = (IO_ErrorType)IO_E_OK;
    switch(len)
	{
		case 1:
		    switch(pdata[0]) 
		    {
		      case SYSTEM_BOOT_MODE:
		        /* do nothing */
		      break;
		      default:
		      {
                  if (LIB_Reset_IsTimeoutSet() && (pdata[0] == SYSTEM_NORMAL_MODE))
                  {
                  	LIB_Reset_SetResetTimeout(100);
                  }
                  LIB_Reset_SetNewSystemMode((SysMode_Type)pdata[0]);
		      }
		    }
			
			break;
		case 2:
			switch(LIB_Reset_GetSystemMode())
            {
                case SYSTEM_NORMAL_MODE:
                    switch(pdata[0])
                    {
                        case SYSTEM_NORMAL_MODE:
                        {/*
                        	if (pdata[1] == SYSTEM_NORMAL_NOCHK_STEP1)
                        	{
								LIB_Reset_SetTimedRequest(( SYSTEM_NORMAL_MODE << 8) | SYSTEM_NORMAL_NOCHK_STEP1, RESET_TIMEDREQ_TIMEOUT);
                        	}
                        	else if (pdata[1] == SYSTEM_NORMAL_NOCHK_STEP2)
                        	{
                        		if (LIB_Reset_ChkTimedRequest(( SYSTEM_NORMAL_MODE << 8) | SYSTEM_NORMAL_NOCHK_STEP1))
                        		{
                        			SAFETY_CHECK_DISABLE();
                        			LIB_Reset_SetResetTimeout(PROGRAMMING_MODE_TIMEOUT_s * 1000);
                        		}
                    			else
                    			{
                        			pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
                    			}
                        	}
                        	else if (pdata[1] == SYSTEM_NORMAL_NOCHK_REFRESH)
                        	{
                        		if (SAFETY_CHECK_ISDISABLED())
                        		{
                        			LIB_Reset_SetResetTimeout(PROGRAMMING_MODE_TIMEOUT_s * 1000);
                        		}
                        		else
								{
									pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
								}
                        	}
                        	else*/
                        	{
                                (void)CALLBACK_Protocol_SubModeManage(pdata[1]);
                        	}
                        }
                        break;
                        case SYSTEM_SET_UI_REMOTE_CONTROL:
                            CALLBACK_Protocol_UIRemoteControlManage(pdata[1]);
                        break;
                        case SYSTEM_SPECIAL_MODE:
                            (void)CBF_Task_Protocol_SetSpecialModeManager(pdata[1]);
                        break;
                     }
                break;

                default:
                    if(pdata[0] == SYSTEM_BOOT_MODE)
                    {
                    	/* Do nothing */
                    }
                    else
                    {
                        pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
                    }
                    
            }
            
            /* any system mode */
            switch(pdata[0]) 
			{
			case SYSTEM_BOOT_MODE:
			{
				if (pdata[1] == SYSTEM_BOOT_ENTER_STEP1)
				{
					LIB_Reset_SetTimedRequest(( SYSTEM_BOOT_MODE << 8) | SYSTEM_BOOT_ENTER_STEP1, RESET_TIMEDREQ_TIMEOUT);
				}
#if defined(BOOTLOADER_IEC60335)
				else if (pdata[1] == SYSTEM_BOOT_SETTING_ENTER_STEP1)
				{
					LIB_Reset_SetTimedRequest(( SYSTEM_BOOT_MODE << 8) | SYSTEM_BOOT_SETTING_ENTER_STEP1, RESET_TIMEDREQ_TIMEOUT);
				}
				else if (pdata[1] == SYSTEM_BOOT_SETTING_ENTER_STEP2)
				{
					if (LIB_Reset_ChkTimedRequest(( SYSTEM_BOOT_MODE << 8) | SYSTEM_BOOT_SETTING_ENTER_STEP1))
					{
						LIB_Reset_SetNewSystemMode(SYSTEM_BOOT_MODE);
						OCRST_WriteBootSettingKeywordArea();
					}
					else
					{
						pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
					}
				}
#endif
				else if (pdata[1] == SYSTEM_BOOT_ENTER_STEP2)
				{
					if (LIB_Reset_ChkTimedRequest(( SYSTEM_BOOT_MODE << 8) | SYSTEM_BOOT_ENTER_STEP1))
					{
						LIB_Reset_SetNewSystemMode(SYSTEM_BOOT_MODE);
						OCRST_WriteBootKeywordArea();
					}
					else
					{
						pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
					}
				}
				else
				{
					pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
				}
			}
			break;
			}
	}
}


/**
Basic Set Load Request
The request will end successfully, Platform Protocol PP_ACK will be sent if,
\li system mode is SYSTEM_NORMAL_MODE
\li no error on queue at low level.
*/
DEF_CSM_EE_CUSTOM_FUNC(BS_SetLoads)
{
	/* format:
	TYPE	: 0x5x
	DATA0  : load index in Outputs Map
	DATA1  : XXXXYYYY (YYYY: activation level, XXXX: configuration)
	DATA2  : not used
	*/

    (void) ch;    
    
	if(LIB_Reset_GetSystemMode() == SYSTEM_NORMAL_MODE)
	{
		if (SAFETY_CHECK_ISDISABLED())
		{
			/*Add the flag for basic frame */
			if( Main_RemoteLoadsControl_PutCommand(REMOTE_LOADS_CONTROL_BASIC,len, pdata))
			{
				pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
			}
		}
		else
		{
			pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
		}

	}
	else
	{
	    pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
	}
}


/**
WriteUIInfo Request
The request will end successfully, answering with the content of data of the application, if 
WriteUIInfo variable is configured by using Set_CPApplianceDataItem
*/
DEF_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest)
{
	return (IO_ErrorType)IO_E_OK;
}


/**
Status Request
The request will end successfully, answering with the content of data of the application, if
Status variable is configured by using Set_CPApplianceDataItem
*/
DEF_CSM_EE_CTRL(Ctrl_StatusRequest)
{
	return (IO_ErrorType)IO_E_OK;
}

/**
Ident Request
Preconfigured by platform framework.
*/
DEF_CSM_EE_CTRL(Ctrl_IdentRequest)
{
    return (IO_ErrorType)IO_E_OK;
}





/**
Write Regulations Request
The request will end successfully, answering with the content of data of the application, if
WriteRegs variable is configured by using Set_CPApplianceDataItem and CBF_OnWriteRegulations()
returns TRUE.
*/
DEF_CSM_EE_CTRL(Ctrl_WriteRegsRequest)
{
	
#if ENABLE_SYNC_UI_MAIN == 1				
	if(CBF_OnWriteRegulations() == FALSE)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}
#endif /* ( ENABLE_SYNC_UI_MAIN == 1 ) */
return (IO_ErrorType)IO_E_OK;
}


/**
Status Request
The request will end successfully, answering with the content of data of the application, if 
ReadRegs variable is configured by using Set_CPApplianceDataItem and CBF_OnReadRegulations()
returns TRUE.
*/
DEF_CSM_EE_CTRL(Ctrl_ReadRegsRequest)
{
/* read regulations returns a NACK if DP_Reg_Var_dim is zero*/
#if  ENABLE_SYNC_UI_MAIN == 1			   	
        /* sincronizzazione UI-Main mediante la sequenza */
  	    /* READ_STATUS - READ_REGS - WRITE_REGS */
    if(FALSE == CBF_OnReadRegulations() )
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }
#endif /* ( ENABLE_SYNC_UI_MAIN == 1 ) */			
	    
	return (IO_ErrorType)IO_E_OK;
}







BEGIN_CP_SLAVEMANAGER(CP_SlaveManager)
		
        if(CP_SLAVEMANAGER_REQ_IS_EXTENDED() && CP_SLAVEMANAGER_REQ_IS_TYP8())
        {            
            BEGIN_CP_SLAVEMANAGER__REQ_FILTER_EETYP8_TYPE()
                /* General types */
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_SETTING_DATA_BLOCK)
    				CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock))
    			END_CP_MATCH_VAL()
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITE_RAM)
    				CP_CSM_EETYP8_WRITERAM(WRRemapTable)
    			END_CP_MATCH_VAL()
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_READ_FLASH)
    			    CP_CSM_EETYP8_READFLASH(RFRemapTable)
    		    END_CP_MATCH_VAL()
    		    BEGIN_CP_MATCH_VAL(PP_EE_TYPE_ERASE_FLASH_PAGE)
    				CP_CSM_EETYP8_ERASEFLASH(RFRemapTable)
    			END_CP_MATCH_VAL()
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITE_FLASH)
    				CP_CSM_EETYP8_WRITEFLASH(RFRemapTable)
    			END_CP_MATCH_VAL()
#if defined(CONFIG_CP_EXTENDED_REQUESTS_ENABLED)
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_READMEMORY_EXTENDED)
    				CP_CSM_EETYP8_EXTENDEDMEMORYREADING(ProtocolReadingAccessTable)
				END_CP_MATCH_VAL()
				BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITERAMMEMORY_EXTENDED)
					CP_CSM_EETYP8_EXTENDEDRAMMEMORYWRITING(ProtocolWritingRamAccessTable)
				END_CP_MATCH_VAL()
				BEGIN_CP_MATCH_VAL(PP_EE_TYPE_ERASEFLSMEMORY_EXTENDED)
					CP_CSM_EETYP8_EXTENDEDFLSMEMORYERASING(ProtocolErasingAccessTable)
				END_CP_MATCH_VAL()
				BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITEFLSMEMORY_EXTENDED)
					CP_CSM_EETYP8_EXTENDEDFLSMEMORYWRITING(ProtocolWritingAccessTable)
				END_CP_MATCH_VAL()
				BEGIN_CP_MATCH_VAL(PP_EE_TYPE_EXTENDED_IDENT_REQ)
					CP_CSM_EETYP8_READ_APPLDATA(USE_CSM_EE_CTRL(Ctrl_IdentRequest), CPApplianceDataTable[CP_APPLDATA_EXTENDED_IDENT].pData, CPApplianceDataTable[CP_APPLDATA_EXTENDED_IDENT].szMaxLength, ;, ;)
				END_CP_MATCH_VAL()
#endif



    		    
    		    BEGIN_CP_MATCH_VAL(PP_EE_TYPE_SYSTEMMODE)
    				CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(EE_SystemMode))
    			END_CP_MATCH_VAL()
    			
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_LOADS)
    				CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(EE_SetLoads))
    			END_CP_MATCH_VAL()
    		    
    		    /* Application */
    		    BEGIN_CP_MATCH_VAL(PP_EE_TYPE_IDENT)
    			    CP_CSM_EETYP8_READ_APPLDATA(USE_CSM_EE_CTRL(Ctrl_IdentRequest), CPApplianceDataTable[CP_APPLDATA_IDENT].pData, CPApplianceDataTable[CP_APPLDATA_IDENT].szMaxLength, ;, ;)
    			END_CP_MATCH_VAL()
    			
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_READ_STATUS)
    			    CP_CSM_EETYP8_READ_APPLDATA(USE_CSM_EE_CTRL(Ctrl_StatusRequest), CPApplianceDataTable[CP_APPLDATA_R_STATUS].pData, CPApplianceDataTable[CP_APPLDATA_R_STATUS].szMaxLength, ; , ;)
    			END_CP_MATCH_VAL()
    			
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_READ_REGS)
    			    CP_CSM_EETYP8_READ_APPLDATA(USE_CSM_EE_CTRL(Ctrl_ReadRegsRequest), CPApplianceDataTable[CP_APPLDATA_R_REGS].pData, CPApplianceDataTable[CP_APPLDATA_R_REGS].szMaxLength, ; , ;)
    			END_CP_MATCH_VAL()
    			
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITE_REGS)
    			    CP_CSM_EETYP8_WRITE_APPLDATA(USE_CSM_EE_CTRL(Ctrl_WriteRegsRequest), CPApplianceDataTable[CP_APPLDATA_W_REGS].pData, CPApplianceDataTable[CP_APPLDATA_W_REGS].szMaxLength, UseRegs(); , UnuseRegs();)
    			END_CP_MATCH_VAL()
    			
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_WRITE_UIINFO)
    			    CP_CSM_EETYP8_WRITE_APPLDATA(USE_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest), CPApplianceDataTable[CP_APPLDATA_W_UIINFO].pData, CPApplianceDataTable[CP_APPLDATA_W_UIINFO].szMaxLength, UseUiInfo(); , UnuseUiInfo();)
    			END_CP_MATCH_VAL()
                          
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
				BEGIN_CP_MATCH_VAL(PP_EE_TYPE_DSP_READ_EXT_RAM)
					CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest))
    			END_CP_MATCH_VAL()
                          
                BEGIN_CP_MATCH_VAL(PP_EE_TYPE_DSP_WRITE_EXT_RAM)
                	CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest))
    			END_CP_MATCH_VAL()
                        
                BEGIN_CP_MATCH_VAL(PP_EE_TYPE_DSP_EXTRA_REQ_RAM)
                	CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest))
    			END_CP_MATCH_VAL()

                BEGIN_CP_MATCH_VAL(PP_EE_TYPE_DSP_INFO_WRITE)
                	CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest))
    			END_CP_MATCH_VAL()
                        
                BEGIN_CP_MATCH_VAL(PP_EE_TYPE_DSP_CMD_READ)
                	CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest))
    			END_CP_MATCH_VAL()

#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */

#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_REQ_APPL_POWER_PROFILE)
    				CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(EE_PowerProfileReq))
    			END_CP_MATCH_VAL()
#endif /* CONFIG_PM_POWER_PROFILE_SUPPORT */
#if defined(CONFIG_PM_ENERGY_METER_SW)
    			BEGIN_CP_MATCH_VAL(PP_EE_TYPE_REQ_APPL_READPOWERMETER)
					CP_CSM_EETYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(EE_ReadPowerMeter))
				END_CP_MATCH_VAL()
#endif /*defined(CONFIG_PM_ENERGY_METER_SW)*/

			END_CP_SLAVEMANAGER__REQ_FILTER()
        }
        else
        {
            BEGIN_CP_SLAVEMANAGER__REQ_FILTER_BSTYP8_TYPE() 
                BEGIN_CP_MATCH_VAL(PP_BS_TYPE_LOADS)
    				CP_CSM_BSTYP8_CUSTUMFUNC(USE_CSM_EE_CUSTOM_FUNC(BS_SetLoads))
    			END_CP_MATCH_VAL()
    			BEGIN_CP_MATCH_VAL(PP_BS_TYPE_WRITE_RAM)
    				CP_CSM_BS_WRITERAM(WRRemapTable)
    			END_CP_MATCH_VAL()
    			BEGIN_CP_MATCH_VAL(PP_BS_TYPE_READ_RAM)
    				CP_CSM_BS_READRAM(RFRemapTable)
    			END_CP_MATCH_VAL()
            END_CP_SLAVEMANAGER__REQ_FILTER()
        }
        
END_CP_SLAVEMANAGER()
                                                      




/**
Communication Platform initialization
*/
void CP_I2CCommunicationPlatformInit(void) 
{
    IO_PhyInitSyncType drvParam={0};

	OS_CREATERSEMA(&SemaRegs);
	OS_CREATERSEMA(&SemaUiInfo);

	OS_CREATETASK(&CP_CommunicationTaskTCB, 
              "CommunicationProtocolTask", 
              CommunicationProtocolTask, 
              CP_COMM_I2C_PRIORITY,
              CP_CommunicationTaskStack);
#if defined(CONFIG_MAIN_PROTOCOL_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&CP_CommunicationTaskTCB, &Main_ProtocolStackMonitor);
#endif
	
	drvParam.eventCB=PP_SlaveNotification;   
	drvParam.id=(IO_ChannelType)(CP_I2C_CH_ID);

    (void)OCI2C_InitSync(&drvParam);
    

    InitSema();
    
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
    OS_CREATETASK(&CP_UartParserTaskTCB, 
          "CP_UartParserTask", 
          CP_UartParserTask, 
          CP_COMM_UART_PRIORITY,
          CP_UartParserTaskStack);
#if defined(CONFIG_MAIN_PROTOCOL_STACK_MONITOR)
	Tools_CreateTaskStackMonitor(&CP_UartParserTaskTCB, &Main_UartParserStackMonitor);
#endif
          
    drvParam.id=(IO_ChannelType)CP_UART1_CH_ID;
    (void)OCUART_InitSync(&drvParam);
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */
}


/**
CommunicationProtocolTask: normal, preemptable, task.
 
\author 	Roberto Fioravanti
\date		13/01/2011
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
void CommunicationProtocolTask (void)
{

    OS_U8 MyEvent;
    while(1) 
    {
        MyEvent = OS_WaitSingleEvent(MAIN_OS_EVENT_I2C|MAIN_OS_EVENT_UART);
        if(MyEvent & MAIN_OS_EVENT_I2C) 
        {
            (void)CALL_CP_SLAVEMANAGER(CP_SlaveManager, (IO_ChannelType)CP_I2C_CH_ID, BCP_ChannelTable[CP_I2C_CH_ID].rxBuffer, BCP_ChannelTable[BCP_I2C_CH_ID].txBuffer, (CP_InProtParamPType*)NULL);
        }    
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
        if(MyEvent & MAIN_OS_EVENT_UART) 
        {
            (void)CALL_CP_SLAVEMANAGER(CP_SlaveManager, (IO_ChannelType)CP_UART1_CH_ID, BCP_ChannelTable[CP_UART1_CH_ID].rxBuffer, BCP_ChannelTable[CP_UART1_CH_ID].txBuffer, (CP_InProtParamPType*)NULL);
        }
#endif /* defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT) */
        
        
        /*check if System mode is changed */
        if((LIB_Reset_GetSystemMode() != LIB_Reset_GetNewSystemMode()) && 
         (BCP_ChannelTable[CP_I2C_CH_ID].rxBuffer[PP_DTYP_INDEX]==PP_EE_TYPE_SYSTEM_MODE))
        {
        	/*lint -e530*/
        	/*Warning non consistente*/
            while(I2C_IsBusBusy())
            /*lint +e530*/
            {}
            
            if(LIB_Reset_GetNewSystemMode() == SYSTEM_NORMAL_MODE)
            {
            	LIB_KeywordsRamCorruption();
            }
            
            OCRST_GenerateUserResetSync();
        }        
    }
}

#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
void CP_UartParserTask(void)
{
  int t0 = OS_GetTime();
  uint8_t tout=0;
  while(1)
  {
    OCUART_ErrMngr();
    
    if(!OCUART_RxStreamMngr(CP_UART1_CH_ID))
    {
            tout++;		
            tout %= 8;
            if(!tout)
            {
            	CP_UART_EnableComm();
            }
    }
    else
    {
            tout=0;
    }

    t0 += 5;
    OS_DelayUntil ( t0 );
  }
}

void CP_UART_EnableComm(void)
{
    OCUART_EnableSync();
    OCUART_TxByteSync(0x55);
}

void CP_UART_DisableComm(void)
{
    OCUART_DisableSync();
}
#endif

uint16_t CP_GetPPAddr(const void *realAddr)
{
	return PP_MappingGetPPAddr((const IO_MemPtrType) realAddr, RFRemapTable, sizeof(RFRemapTable)/sizeof(RFRemapTable[0]));
}
/* *********** */
/* End of file */
/* *********** */

