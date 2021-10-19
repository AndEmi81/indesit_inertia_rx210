/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	BCP_CFG Configurazione del Bus Communication Platform
\ingroup BCP
\par		Descrizione
\n		Contiene le informazioni sulla configurabilità del Bus Communication Platform.
\n    Configurare il modulo Bus Communication Paltform significa predisporre i seguenti file:
- BusCommPltf_cfg.c Contiene le strutture dati per il Communication Manager e Platform Protocol, le variabili dell'applicazione.
- BusCommPltf_cfg.h

\todo 
Opzioni per modo bridge
-INDESIT_BRIDGE_ZIGBEE
-INDESIT_BRIDGE_SERIAL
*/

/**
\par		Descrizione
\n		Strutture dati per il Communication Manager e Platform Protocol.
\n    Sono strutture dati che l'applicazione può configurare a seconda delle proprie esigenze.
\n    Questo è da considerarsi a tutti gli effetti un file dell'applicazione specifica.
\file		BusCommPltf_cfg.c
\ingroup	BCP_CFG
\date		22/09/2010
\version	01.00.00 
\author		Roberto Fioravanti
\todo
\n
\n
\par		History
\n
\n 		00.00.64
\n		22/09/2010
\n		developper Roberto Fiorvanti
\n		changes 
\n 		- creation
\n		- Introduzione tabelle
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "CstmHeaders.h"
#include "ProductConfig.h"

#include "SkeletonGlobal.h"
#include "FlashModule.h"


#include "StandardDefines.h"


#include "BusCommPltf_cfg.h"
#include "BCP_BusCommMng.h"


#include "ExternalOutputManagerExp.h"
#include <string.h>




#ifdef BUS_COM_PLT_I2C_SUPPORT
#include "OCI2C_drv.h"
#endif /* BUS_COM_PLT_I2C_SUPPORT */

#ifdef BUS_COM_PLT_UART_SUPPORT
#include "OCUART_drv.h"
#endif /* BUS_COM_PLT_UART_SUPPORT */

#if defined(RENESAS_300H_39079)
#include "osek.h"
#endif

#if defined(FREESCALE_MC9S08_GT60) \
    || defined(FREESCALE_S08_GB60)
#include "OsekInclude.h"
#endif /* defined(FREESCALE_MC9S08_GT60) \
    || defined(FREESCALE_S08_GB60) */

#include "callback.h"
#include "callbackUI.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
#define UI_GetResource (void)GetResource(UserInterface_Res);
#define UI_ReleaseResource (void)ReleaseResource(UserInterface_Res);
#ifdef EXTERNAL_MOTOR_CONTROL
#define Engine_GetResource (void)GetResource(Engine_Res);
#define Engine_ReleaseResource (void)ReleaseResource(Engine_Res);
#endif

#define EXT_PROT_CMD_SID_SIZE				1
#define EXT_PROT_TOOL_ID_REQ_SIZE		1



/* *****************************************
**	Extended Types Data buffer dimension
*/
#define PP_PCCONTROLDATABUFFER_DIM			10

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */
#ifdef EXTERNAL_MOTOR_CONTROL

#pragma section
#endif /*EXTERNAL_MOTOR_CONTROL*/

BYTE DP_Status_Var_dim = 0;
BYTE DP_Reg_Var_dim = 0;
BYTE DP_UIInfoBuff_dim = 0;
#pragma section
/* Exported */

/* Static */
/**
	\addtogroup BCP_CFG_HOTTIMA_VARS  Variabili
	\ingroup	BCP_CFG_HOTTIMA
@{*/

/** Buffer di comunicazione per driver I2C */
static uint8_t BCP_I2CTxBuffer[BCP_I2C_TX_BUF_SIZE];

/** Buffer di comunicazione per driver UART */
static uint8_t BCP_UARTRxBuffer[BCP_UART_TX_BUF_SIZE];
#define BCP_UARTTxBuffer BCP_UARTRxBuffer


/*@}*/

/* Tabella dei canali. 
è un descrittore di driver. Contiene le informazioni relative a 
\li buffer di ricezione e trasmissione
\li puntatore alle funzioni di trasmissione, ricezione */
const BCP_ChannelTableType BCP_ChannelTable[BCP_CH_NUM]=
{ 
#ifdef BUS_COM_PLT_I2C_SUPPORT
  {NULL, OCI2C_InitSync, OCI2C_TxSync, OCI2C_RxSync, BCP_I2CTxBuffer, BCP_I2CRxBuffer, BCP_I2C_TX_BUF_SIZE, BCP_I2C_RX_BUF_SIZE}, /* BCP_I2C_CH_ID */
#endif /* BUS_COM_PLT_I2C_SUPPORT */
#ifdef EXTERNAL_MOTOR_CONTROL
#ifdef BUS_COM_PLT_UART_SUPPORT
  {NULL, OCUART_InitSync, OCUART_TxSync, OCUART_RxSync, BCP_UARTTxBuffer, BCP_UARTRxBuffer, BCP_UART_TX_BUF_SIZE, BCP_UART_RX_BUF_SIZE}, /* BCP_UART1_CH_ID */
#endif /* BUS_COM_PLT_UART_SUPPORT */
#endif

};


#include "SkeletonTypes.h" /* contiene la definizione di IdentTableStruct */
extern const struct IdentTableStruct IdentTable;
extern  unsigned char* const UIInfoBuff_Addr;
/*aggiungere i riferimenti alle altre variabili dell'applicazione (regolazioni stato)*/






/* ******************* */
/* Functions Prototype */
/* ******************* */

#if defined (SUPPORT_EXPANSIONS_PACKETS)
static expansionRef_t getRefExpansionData( uint8_t *pDigitalProtocolDataBuffer, uint8_t packetType );
#endif

uint8_t GetExpansionGetInfoBuffer_Index( uint8_t *DataBuffer);
uint8_t GetExpansionSetInfoBuffer_Index( uint8_t *DataBuffer);
DEF_CSM_EE_CUSTOM_FUNC(EE_SetLoads);
DEF_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock);
void BS_SetLoads(uint8_t* data, uint8_t len);
DEF_CSM_EE_CTRL(Ctrl_IdentRequest);
DEF_CSM_EE_CTRL(Ctrl_ReadRegsRequest);
DEF_CSM_EE_CTRL(Ctrl_StatusRequest);
DEF_CSM_EE_CTRL(Ctrl_WriteRegsRequest);
DEF_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest);
DEF_CSM_EE_BUFF(GetExpansionBuffer_SetInfo_Index);
DEF_CSM_EE_BUFF(GetExpansionBuffer_GetInfo_Index);
DEF_CSM_EE_CUSTOM_FUNC(EE_SystemMode);
IO_BufferInfoType_t GetExpansionBuffer_SetInfo_Index( uint8_t *DataBuffer);
IO_BufferInfoType_t GetExpansionBuffer_GetInfo_Index( uint8_t *DataBuffer);
DECLARE_BCP_SLAVEMANAGER(PP_SlaveManager);


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest);

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpGetInfo);
DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpSetInfo);



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**/		
BEGIN_BCP_EOSN()
	BEGIN_BCP_EOSN__EE()
		BCP_EOSN__EE(PP_EE_TYPE_READ_STATUS, CBF_OnReadStatusCompleted)
		BCP_EOSN__EE(PP_EE_TYPE_READ_REGS, CBF_OnReadRegulationsCompleted)
	END_BCP_EOSN__EE()
END_BCP_EOSN()



#if defined (SUPPORT_EXPANSIONS_PACKETS)

static expansionRef_t getRefExpansionData( uint8_t *pDigitalProtocolDataBuffer, uint8_t packetType )
{
uint8_t i;
idTypeExpansion_t idTypeExpansion;
expansionRef_t expansionRef;

    expansionRef.pBufDataForExp = NULL;
    expansionRef.sizeBufDataForExp = 0;
	expansionRef.whichExp = 0;
	
    idTypeExpansion = *(idTypeExpansion_t*)pDigitalProtocolDataBuffer;
    for ( i = 0; i < NUM_MAX_EXP; i++ )
    {
        if ( ID_TypeExpansion[i].all == idTypeExpansion.all )               
        {
            break;
        }
    }
    if ( ( i < NUM_MAX_EXP ) && ( ( ExpEnabled & ( 1 << i ) ) != 0 ) )
    {
        /* this is the expansion recognized */
        expansionRef.whichExp = ( 1 << i );
        
        switch ( packetType )
        {
            case PP_EE_TYPE_EXP_GET_INFO:
				/* here are stored data for expansion */
                expansionRef.pBufDataForExp=pVectRefExp[i]->pBufDataForExp;
                expansionRef.sizeBufDataForExp=pVectRefExp[i]->sizeBufDataForExp;
                
                break;
            
            case PP_EE_TYPE_EXP_SET_INFO:
				/* here I will store data from expansion */
                expansionRef.pBufDataForExp=pVectRefExp[i]->pBufExpInfo;
                expansionRef.sizeBufDataForExp=pVectRefExp[i]->sizeBufExpInfo;
                
                break; 
            
            default:
                break;       
        }
        
    }
    return expansionRef;
}

/**
Funzione chiamata dall'applicazione.\n
Esegue il clear del/dei flag di avvenuta ricezione pacchetto dalla/dalle Expansion\n
individuate con la bitmap whichExpansion. 

\param  [in]		whichExpansion, bitmap indicante la/le expansions\n
                    ordine identico a come sono state inserite dall'applicazione con \
                    la setExpansionData(). 

\attention \b DataFromExpReceived used as W


\author 	Stefano Ughi
\date		14/11/2008
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

void Digital_Protocol_Clear_Expansion_Boards_Com_Ctrl( uint8_t whichExpansion )
{
	DataFromExpReceived &= (whichExpansion^0xFF);
}



/**
Funzione chiamata dall'applicazione.\n
Ritorna se sono arrivati pacchetti da una o più expansion.

\param  [in]		whichExpansion, bitmap indicante la/le expansions\n
                    ordine identico a come sono state inserite dall'applicazione con \
                    la setExpansionData(). 

\attention \b DataFromExpReceived used as R

\return 	uint8_t

\retval		bitmap, in base alla/alle expansions su cui si vuole sapere se sono\n
            arrivati dei pacchetti di info.  	 


\author 	Stefano Ughi
\date		14/11/2008
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/

uint8_t Digital_Protocol_Get_Expansion_Boards_Com_Ctrl( uint8_t whichExpansion )
{
	return ( DataFromExpReceived & whichExpansion );
}


#endif /* SUPPORT_EXPANSIONS_PACKETS is defined */

#ifdef EXTERNAL_MOTOR_CONTROL
#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
#if defined(BUILD_RENESAS_36079)
#pragma inline (Digital_Protocol_Get_External_Board_Com_Ctrl)
#endif
uint8_t Digital_Protocol_Get_External_Board_Com_Ctrl(void)
{
	return External_Board_Com_Ctrl;
}

#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
void Digital_Protocol_Set_External_Board_Com_Ctrl(uint8_t value)
{
	External_Board_Com_Ctrl = value;
}
#endif /*EXTERNAL_MOTOR_CONTROL*/

#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
#if defined(BUILD_RENESAS_36079)
#pragma inline (Get_Digital_Protocol_Reg_Var_dim)
#endif
/** Setta dim. Buffer "Read Regulatation Request Packet" */
void Get_Digital_Protocol_Reg_Var_dim (uint8_t v)
{

    DP_Reg_Var_dim = v;

}

#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
#if defined(BUILD_RENESAS_36079)
#pragma inline (Get_Digital_Protocol_Status_Addr_dim)
#endif
void Get_Digital_Protocol_Status_Addr_dim (BYTE v)
{

    //DP_Write_Status_Addr_dim= v;

}
#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
#if defined(BUILD_RENESAS_36079)
#pragma inline (Get_Digital_Protocol_Status_Var_dim)
#endif
void Get_Digital_Protocol_Status_Var_dim (BYTE v)
  {

      DP_Status_Var_dim =v;

  }
	
#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma INLINE
#endif
#if defined(BUILD_RENESAS_36079)
#pragma inline (Get_Digital_Protocol_UIInfoBuff_dim)
#endif
void Get_Digital_Protocol_UIInfoBuff_dim (BYTE v)
  {

      DP_UIInfoBuff_dim =v;

  }
	
/* i2c interrupt */
ISR(Isr_I2C)
{
  OCI2C_ISR();
}

/* callback chiamata nell'interrupt */
void ActionComm(IO_ChannelType ch, IO_NotificationType notif) 
{
	if(ch == BCP_I2C_CH_ID)
	{
  	SetEvent(Task_Protocol, EVENT_I2C_PROTOCOL);
	}
	else if(ch == BCP_UART1_CH_ID)
	{
		SetEvent(Task_Protocol, EVENT_UART_PROTOCOL);
	}
}

BEGIN_BCP_SLAVEMANAGER(PP_SlaveManager)

  BEGIN_BCP_SLAVEMANAGER__EE()	
#ifdef EXTERNAL_MOTOR_CONTROL
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_DSP_READ_EXT_RAM, USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_DSP_WRITE_EXT_RAM, USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_DSP_EXTRA_REQ_RAM, USE_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_DSP_INFO_WRITE, USE_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_DSP_CMD_READ, USE_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest))
#endif

#if defined (SUPPORT_EXPANSIONS_PACKETS)
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_EXP_SET_INFO, USE_CSM_EE_CUSTOM_FUNC(Ctrl_ExpSetInfo))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_EXP_GET_INFO, USE_CSM_EE_CUSTOM_FUNC(Ctrl_ExpGetInfo))
#endif

    BCP_SLAVEMANAGER__EE_READ_FLASH()

    BCP_SLAVEMANAGER__EE_WRITE_RAM()
		BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH(FLASH_WRITE_ERASE_FLASH_PLT1())
    
		BCP_SLAVEMANAGER__EE_READ_APPLDATA(PP_EE_TYPE_IDENT, USE_CSM_EE_CTRL(Ctrl_IdentRequest), &IdentTable, sizeof(IdentTable))
		BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_SETTING_DATA_BLOCK, USE_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock))
    
    BCP_SLAVEMANAGER__EE_READ_APPLDATA(PP_EE_TYPE_READ_REGS, USE_CSM_EE_CTRL(Ctrl_ReadRegsRequest), &BufOldReg, REG_VAR_SIZE)
    BCP_SLAVEMANAGER__EE_READ_APPLDATA(PP_EE_TYPE_READ_STATUS, USE_CSM_EE_CTRL(Ctrl_StatusRequest), &MachineStateBuf, STATUS_VAR_SIZE)
    
    BCP_SLAVEMANAGER__EE_WRITE_APPLDATA(PP_EE_TYPE_WRITE_UIINFO, USE_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest), UIInfoBuff_Addr, DP_UIInfoBuff_dim, UI_GetResource, UI_ReleaseResource)
	  BCP_SLAVEMANAGER__EE_WRITE_APPLDATA(PP_EE_TYPE_WRITE_REGS, USE_CSM_EE_CTRL(Ctrl_WriteRegsRequest), BufNewReg, UI_WRITE_STATUS_LEN, UI_GetResource, UI_ReleaseResource)

        
    BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_LOADS, USE_CSM_EE_CUSTOM_FUNC(EE_SetLoads))
    
    BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_SYSTEM_MODE, USE_CSM_EE_CUSTOM_FUNC(EE_SystemMode))
  END_BCP_SLAVEMANAGER__EE()
  
  BEGIN_BCP_SLAVEMANAGER__BS()
    BCP_SLAVEMANAGER__BS_APPL_DEFINED(PP_BS_TYPE_LOADS, BS_SetLoads)
    BCP_SLAVEMANAGER__BS_WRITE_RAM()
    BCP_SLAVEMANAGER__BS_READ_RAM()
  END_BCP_SLAVEMANAGER__BS()
END_BCP_SLAVEMANAGER()
/**/


/* Local Function */

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpGetInfo)
{
#if 0
	/* Ritorna all'expansion le informazioni richieste */
	expansionRef_t expansionRef;
  expansionRef = getRefExpansionData( pdata, (uint8_t)PP_EE_TYPE_EXP_GET_INFO );

	if ( expansionRef.sizeBufDataForExp == 0 )
	{
	    /* Expansion ID not recognized or not enabled */
	    pinfo->error=IO_E_INVALID_VALUE;
			return;
	}
	
	pinfo->address=(uint8_t*)expansionRef.pBufDataForExp;
	pinfo->len = expansionRef.sizeBufDataForExp;
	pinfo->error=IO_E_OK;
#endif	
}

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_ExpSetInfo)
{
#if 0
	/* Data from Expansion is arrived! */
	expansionRef_t expansionRef;
  expansionRef = getRefExpansionData( pdata, (uint8_t)PP_EE_TYPE_EXP_SET_INFO );

   if ( ( expansionRef.sizeBufDataForExp != 0 ) && 
      ( len >= 2 ) &&   
      ( (len - 2 ) <= expansionRef.sizeBufDataForExp ) ) 
    {
          
			(void)GetResource(UserInterface_Res); 
			(void)memcpy(expansionRef.pBufDataForExp, pdata+2, (len - 2));					
			(void)ReleaseResource(UserInterface_Res);
			/* E' arrivato un pacchetto da un'expansion (flag per l'applicazione) */
			DataFromExpReceived |= expansionRef.whichExp; 
			pinfo->error=IO_E_OK;
     } 
    else 
    {
        pinfo->error=IO_E_INVALID_VALUE;						            
    }
#endif		
}



DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspExtraRequest)
{
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetExtraRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspReadRamRequest)
{
	
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetReadRamRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_DspWriteRamRequest)
{
	pinfo->error=IO_E_INVALID_VALUE;
	
	if(LIB_IOM_NO_ERROR == LIB_IOM_External_SetWriteRamRequest(pdata, len))
	{
		pinfo->error=IO_E_OK;
	}
}


DEF_CSM_EE_CUSTOM_FUNC(Ctrl_InfoDspRequest)
{
	LIB_IOM_External_ProcessInfoPacketData(0, pdata, len);
	


	pinfo->error=IO_E_OK;									
	
	External_Board_Com_Ctrl = 1;					
			
}

DEF_CSM_EE_CUSTOM_FUNC(Ctrl_CmdDspRequest)
{
	uint8_t* pCmd=0;
	uint8_t  cmdlen=0;
	
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





/**

\todo verificare se l'expansion è abilitata */
/*
uint8_t GetExpansionSetInfoBuffer_Index( uint8_t *DataBuffer)
{
  uint8_t index = PP_S_NUM_REQ;
  
  index = PP_S_EXP_WCMD_REQ;
  return index;
}
*/

/**
Setting Data block Request
The request will end successfully, Platform Protocol packet with the reqested data will be sent, 
if data exist (no error is returned  by BasicLoader)
*/
DEF_CSM_EE_CUSTOM_FUNC(EE_SettingDataBlock)
{
    Loader_Struct Data;
    pinfo->error = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
    
    if(BSLDR_OK == BasicLoader(pdata[0], pdata[1], &Data))
    {
        pinfo->len = Data.DataLenght*2;
		pinfo->address= (IO_MemPtrType)Data.Data;
        pinfo->error = (IO_ErrorType)IO_E_OK;
		pinfo->option|=PP_SL_RESP_ANSW_OPT;
    }
}

DEF_CSM_EE_CUSTOM_FUNC(EE_SetLoads)
{
  #ifdef __GENERICIOMANAGER_H
				
	if(SystemMode == SYSTEM_NORMAL_MODE)
	{	
		if(len < PP_PCCONTROLDATABUFFER_DIM)
		{
			/*Add the header of the MSG	*/			
			*RemoteLoadsControl_TxBuffer.DataBuffer = PCCONTROL_EXTENDED; /*Declare as extended the source pkt */
			/*Add the len of the pkt to the buffer in case of PCCONTROL_EXTENDED */
			*(RemoteLoadsControl_TxBuffer.DataBuffer + PCCONTROL_MASG_HEADER_LEN) = len;
			/*Copy the buffer in the receiving buffer */
			(void)memcpy(RemoteLoadsControl_TxBuffer.DataBuffer+PCCONTROL_MASG_HEADER_LEN+PCCONTROL_MASG_LEN_LEN, pdata, len);
			if (SendMessage(RemoteLoadsControl, &RemoteLoadsControl_TxBuffer)!= ERR_OK)
			{
			    pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
					return;
			}
		}
		else
		{
			pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
			return;
		}    
	}
	
	pinfo->error =(IO_ErrorType)IO_E_OK;
	
#else

	pinfo->error =(IO_ErrorType)IO_E_INVALID_VALUE;

#endif /*__GENERICIOMANAGER_H*/
}

DEF_CSM_EE_CUSTOM_FUNC(EE_SystemMode)
{
  switch(len)
	{
		case 1:
			NewSystemMode=pdata[0];
			break;
		case 2:
			switch(SystemMode)
	    {
	    	case SYSTEM_NORMAL_MODE:
	      	if (pdata[0]==SYSTEM_NORMAL_MODE)
          {
              (void)CBF_Task_Protocol_SubModeManage(pdata[1]);
          }
					if (pdata[0]==SYSTEM_SPECIAL_MODE)
					{
	      		CBF_Task_Protocol_SetSpecialModeManager(pdata[1]);                    
	        } 
          #if ( UI_ONBOARD_PRESENT == 1 )
          if ( pdata[0] == SYSTEM_SET_UI_REMOTE_CONTROL )
          {
              CBF_UI_Task_Protocol_UIRemoteControlManage(pdata[1]);
          }
          #endif   
						break;
            
	        default:
						pinfo->error = (IO_ErrorType)IO_E_INVALID_VALUE;
						return ;
	        	break;
	    }
			break;
	}
	
	pinfo->error = (IO_ErrorType)IO_E_OK;
}

void BS_SetLoads(uint8_t* data, uint8_t len)
{
  /* TYPE	: 0x5x
			     DATA0: load index in Outputs Map
			     DATA1: XXXXYYYY (YYYY: livello attivazione, XXXX: configurazione)
			     DATA2: nu*/		

	if(SystemMode == SYSTEM_NORMAL_MODE)
	{
		/*Add the flag for basic frame */
		*RemoteLoadsControl_TxBuffer.DataBuffer = PCCONTROL_BASIC;
		(void)memcpy((RemoteLoadsControl_TxBuffer.DataBuffer+PCCONTROL_MASG_HEADER_LEN), data, len);
		if (SendMessage(RemoteLoadsControl, &RemoteLoadsControl_TxBuffer)!= ERR_OK)
		{
		    //pDPSlData->AnswLen=0;    /*sends nack */
		}
	}
}



DEF_CSM_EE_CTRL(Ctrl_WriteUIInfoRequest)
{
	UI_Info_Ctrl = 1;
	if(UIInfoBuff_Addr!=NULL)
	{
		return (IO_ErrorType)IO_E_OK;
	}
	return (IO_ErrorType)IO_E_INVALID_VALUE;
}

DEF_CSM_EE_CTRL(Ctrl_IdentRequest)
{
    return (IO_ErrorType)IO_E_OK;
}

DEF_CSM_EE_CTRL(Ctrl_WriteRegsRequest)
{
	
#if ENABLE_SYNC_UI_MAIN == 1				
	if(CBF_OnWriteRegulations()==FALSE)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}
#endif /* ( ENABLE_SYNC_UI_MAIN == 1 ) */
return (IO_ErrorType)IO_E_OK;
}

DEF_CSM_EE_CTRL(Ctrl_ReadRegsRequest)
{
/* read regulations returns a NACK if DP_Reg_Var_dim is zero*/
			   	
   	/*lint -e960 */
    if ( (!DP_Reg_Var_dim)             
#if  ENABLE_SYNC_UI_MAIN == 1
          /* sincronizzazione UI-Main mediante la sequenza */
  	    /* READ_STATUS - READ_REGS - WRITE_REGS */
    || (CBF_OnReadRegulations()==FALSE)
#endif /* ( ENABLE_SYNC_UI_MAIN == 1 ) */			    
    )			
			return (IO_ErrorType)IO_E_INVALID_VALUE;
		return (IO_ErrorType)IO_E_OK;
}

DEF_CSM_EE_CTRL(Ctrl_StatusRequest)
{
	if(!DP_Status_Var_dim)
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	Read_Status_Ctrl = 1;
	return (IO_ErrorType)IO_E_OK;
}

////////////////////////////////////
//Osek Task 

TASK(Task_Protocol)
{
EventMaskType event;
uint8_t ch;

  /*lint -e716 */
	while(TRUE)
	{
    /*lint +e716 */
/*lint -e245 -e436 -e118 dovuto alla scelta dell'implementazione WaitEvent (Renesas) */
		(void)WaitEvent(
			EVENT_I2C_PROTOCOL
#if (UART_ENABLE==TRUE)
			| EVENT_UART_PROTOCOL
#endif
		);
/*lint +e245 +e436 +e118 */		
		(void)GetEvent(Task_Protocol,&event);
		(void)ClearEvent(event);
		
		while(event)
		{
			if(event & EVENT_I2C_PROTOCOL)
			{
				event&=~EVENT_I2C_PROTOCOL;	
				CALL_BCP_SLAVEMANAGER(PP_SlaveManager, (IO_ChannelType)BCP_I2C_CH_ID);
			}
#if (UART_ENABLE==TRUE)

			else if(event & EVENT_UART_PROTOCOL)
			{
				event&=~EVENT_UART_PROTOCOL;
				CALL_BCP_SLAVEMANAGER(PP_SlaveManager, (IO_ChannelType)BCP_UART1_CH_ID);
			}
#endif
			else
			{   /* nessuno degli eventi previsti --> uscita immediata dal while(event) */
				break;
			}
			
			
			if((NewSystemMode!=SystemMode) && (BCP_ChannelTable[BCP_I2C_CH_ID].rxBuffer[PP_DTYP_INDEX]==PP_EE_TYPE_SYSTEM_MODE))
			{
				while(I2C_IsBusBusy());					
				
				if(NewSystemMode==SYSTEM_NORMAL_MODE)
				{
					KeywordsRamCorruption();
				}
				IlopCmd();
			}
		}
	}
}


TASK(Task_SendBytes)
{
 	(void)TerminateTask();
}

/************************************************************************
OSEK_TASK_Task_UartParser
________________________________________________________________________
Questo Task è attivato dall'allarme periodico (5 msec) ALARM_Uart_Parser
*************************************************************************/
uint8_t tout=0;
TASK(Task_UartParser)
{
	if(!OCUART_RxStreamMngr(BCP_UART1_CH_ID))
	{
		tout++;		
		tout %= 8;
		if(!tout)
		{
			OCUART_InitSWSync();
			OCUART_TxByteSync(0x55);
		}
	}
	else
	{
		tout=0;
	}
	
	
		
	(void)TerminateTask();
}

ISR(UartRx_Interrupt)
{
	OCUART_ErrMngr();
	OCUART_RxDataMngr();
	OCUART_TxDataMngr();
}

BYTE Digital_Protocol_Get_UI_Info_Ctrl(void)
{
	return UI_Info_Ctrl;
}

void Digital_Protocol_Set_UI_Info_Ctrl(BYTE value)
{
	UI_Info_Ctrl = value;
}
/* *********** */
/* End of file */
/* *********** */

