/*
Copyright (c) 2009-2010, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Description
\n		On chip I2C driver (\ref OCI2C_Class) implementation. Peripheral configuration, data transmission and reception.
		Data transmission and reception is managed through DMA. SW DMA driver is used if not available
		On chip DMA. The DMA channel used is CPU-dependent.
		\warning On chip DMA tested for Freecale AG128
\n
\file		OCI2C_drv.c
\ingroup	OCI2C
\date		07/2010
\version	00.00.55
\author		Roberto Fioravanti, Jaroslav Musil

\n
\n
\par		History
\n 		00.00.59
\n		08/09/2010
\n		developper Roberto Fioravanti
\n		changes
\n 		- fixed hw dma compiler options defines
\n
\n 		00.00.53
\n		04/07/2010
\n		developper Roberto Fioravanti, Jaroslav Musil
\n		changes
\n 		- added hw dma support
\n
\n 		00.00.45
\n		06/05/2010
\n		developper Roberto Fioravanti
\n		changes
\n 		- protection against unwanted ram writing: 
\n		ISR tests match address and in master mode Arbitration lost. 
\n		In these cases DMA is stopped or not executed
\n
\n 		00.00.32
\n		24/02/2010
\n		developper Roberto Fioravanti
\n		changes
\n		-	optimization: addiction of OCI2C_SetDmaParamSync
\n
\n 		00.00.26
\n		11/01/2010
\n		developper Roberto Fioravanti
\n		changes
\n		-	bug fix: OCI2C_Locked (precompiler options was inverted)
\n
\n 		00.00.25
\n		07/01/2010
\n		developper Roberto Fioravanti
\n		changes
\n		-	unblocking routines re-organization.
\n		- unblocking routines options added
\n
\n 		00.00.24
\n		12/2009
\n		developper Jaroslav Musil
\n		changes
\n		-	Added support for OCDMA but for an issue it was not tested with success.
\n
\n 		00.00.22
\n		26/11/2009
\n		developper Roberto Fioravanti
\n		changes
\n		-	SWDMA init added in OCI2C_Action_Lev1 unblock routine
\n
\n 		00.00.21
\n		16/11/2009
\n		developper Roberto Fioravanti
\n		changes
\n		-	English translation
\n
\n 		00.00.12
\n		18/09/2009
\n		developper Roberto Fioravanti
\n		changes
\n		-	Moved #include  HAL avoided effect of #pragma bit_order _reg (Renesas)
\n    	- 	Addiction (void OCI2C_InitHwSync(void);)
\n
\n 		00.00.10
\n		18/09/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- Introduzione del reset delle linee (Renesas) in modo slave tx dopo la ricezione del nack bit (comunicazione con bridge 4.0.17, con pacchetti con un solo byte di risposta)
\n
\n 		00.00.09
\n		09/09/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- OCI2C_SlaveCallBack: introduzione del reset del bit che segnala match dell'indirizzo
\n
\n 		00.00.02
\n		23/06/2009
\n		developper Roberto Fioravanti
\n		changes
\n 		- creazione
*/





#define __DRIVER_H
/* ******** */
/* Includes */
/* ******** */

#include "OCI2C_drv.h"

#include HAL

#if !defined(CONFIG_PL_DRV_OCI2C_USE_CUSTOM_RESET)
#include "OCRST_drv.h"
#endif

#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
#include "SWDMA_drv.h"
#else /* !defined(_HAL_DMA_CAPABILITY) */
#include "OCDMA_drv.h"
#endif /* !defined(_HAL_DMA_CAPABILITY) */

#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
#include "OCLPM_drv.h"
#endif /* defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT) */

#include "LIB_TaskMonitoring.h"
#include "OCCRC_drv.h"

#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
#include "LIB_Reset.h"
#endif /* defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)*/
/* ****************** */
/* Defines and Macros */
/* ****************** */


/**
Called in the driver ISR. Restores no-activity counter.

\warning

\note

\version		1.0.0
\date 			12/05/2009
\author			Roberto Fioravanti
*/

#define OCI2C_OnActivity() 	\
{\
	OCI2C.NoActivityTimeout=(I2CMONITOR_NOACTIVITY_TIMEOUT/PERIPHERAL_MONITOR_POLL);\
	OCI2C.Pending=TRUE;\
}

#if !defined(I2C_ProcessRxRequest)
#define I2C_ProcessRxRequest(dat)
#endif /* !defined(I2C_ProcessRxRequest) */

enum OCI2C_STATE{
	OCI2C_RX_DUMMY_BIT=0,				/* dummy read */
	OCI2C_I2C_RX_2ND_LAST_BIT,
	OCI2C_UNEXPECTED_DATA_RX_BIT,
	OCI2C_MSTRX_ONGOING_BIT
};

#define OCI2C_RX_DUMMY				(1 << (OCI2C_StateSizeType)OCI2C_RX_DUMMY_BIT)
#define OCI2C_RX_2ND_LAST			(1 << (OCI2C_StateSizeType)OCI2C_I2C_RX_2ND_LAST_BIT)
#define OCI2C_UNEXPECTED_DATA_RX	(1 << (OCI2C_StateSizeType)OCI2C_UNEXPECTED_DATA_RX_BIT)
#define OCI2C_MSTRX_ONGOING			(1 << (OCI2C_StateSizeType)OCI2C_MSTRX_ONGOING_BIT)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef uint8_t OCI2C_State;
typedef uint8_t OCI2C_StateSizeType;


/**
 * On chip I2C class.
 */
typedef struct OCI2C_Class {
  /* slave */
	Comm_Notification		notifyCB;            	/** notification callback */
	Comm_Notification		EOO_callback;        	/** End Of Operation callback */

	IO_MemPtrType			TxBuffer;            	/** pnt tx buffer  */
	IO_MemPtrType			RxBuffer;            	/** pnt rx buffer  */
	IO_PacketSizeType    	TxLen;               	/** bytes to be trasnmitted */
	IO_PacketSizeType    	RxLen;               	/** bytes to be received */
	IO_ChannelType		    DmaCh;               	/** dma channel id*/
	#if defined(_HAL_DMA_CAPABILITY)
	IO_DeviceType			DmaDevice;
	#endif
	uint16_t				BusBusyCount;        	/** block counter */
	uint16_t		        NoActivityTimeout;   	/** no activity timeout counter */
	I2C_SlaveAddressType	CurrentSlave;        	/** current slave address */

	IO_OptionType        	options;				/** options */
	OCI2C_State				state;					/** state */
	IO_ChannelType			id;						/** Driver channel ID */

	I2C_StatusType			BusStatus;            	/** Bus status */
	uint8_t					Pending;				/** interrupt activity */
	uint8_t					Block2Cnt;				/** block level2 counter activity */
	
}OCI2C_Type;

/* **************************** */
/* Prototype of local functions */
/* **************************** */

void OCI2C_AckMngr(void);
#if I2C_MASTER_SUPPORT
void OCI2C_MasterMngr(void);
#endif /* I2C_MASTER_SUPPORT */
void OCI2C_SlaveMngr(void);
void OCI2C_SlaveStopCommSync(void);
I2C_StatusType OCI2C_IsBusBusy(void);

void OCI2C_UpdateBusStatusSync(void);
void OCI2C_SlaveRxCfg(void);
void OCI2C_MasterStopComm(void);


#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
void OCI2C_SetDmaParamSync(IO_PacketSizeType len, IO_MemPtrType sar, IO_MemPtrType dar, bool_t tx, SWDMA_TransferType* transfer);
#endif


#if I2C_MASTER_SUPPORT
IO_ErrorType OCI2C_CallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);
#endif /* I2C_MASTER_SUPPORT */
IO_ErrorType OCI2C_SlaveCallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);

void OCI2C_Notification(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error);


/* ****************** */
/* Exported variables */
/* ****************** */

/* **************** */
/* Global variables */
/* **************** */
OCI2C_Type  OCI2C;
static uint8_t  bCallbackEnable;

/* Low power mode related variables */
#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
static uint8_t OCI2C_LowPowerModeOn = 0;
static Comm_Notification OCI2C_LPMCallback;
#endif /* defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT) */

/* ***************** */
/* Exported function */
/* ***************** */
#if defined( CONFIG_DEBUG_I2C_DRV )
#include "OCI2C_drv_trace.h"
#else
#define DEBUG_I2C_TRACE_MODULE(x)
#define DEBUG_I2C_TRACE_INIT(x)
#define DEBUG_I2C_TRACE_IRQ(x)
#endif

DEBUG_I2C_TRACE_MODULE( )

/**
Initializes and enables the peripheral. It can be used to reset the peripheral's status. 
It sets the:
\li clockrate according to \b I2C_CLOCKRATE_kbps
\li slave address according to \b I2C_SLAVEADDRESS

\return 	void 	 

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date		08/oct/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:	
\public \memberof OCI2C_Class
*/
void OCI2C_InitHwSync(void)
{
	DEBUG_I2C_TRACE_INIT(0)
	/* hw init - reset peripheral*/
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
	(void)SWDMA_ChannelDisableSync(0);	
#else
	I2C_DisableDMA();
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
	
	I2C_ClkDisable();
	I2C_SetSlaveAddress(0);
	I2C_ResetClockRate(0);
	
	I2C_ClkEnable();
	I2C_CtrlReset();
	I2C_IntRegReset();
  
	I2C_ResetStatus();
	I2C_Reset();
	
	/* hw init - set peripheral*/
	I2C_SetSlaveAddress(I2C_SLAVEADDRESS);
	I2C_SetClockRate(I2C_CLOCKRATE_SETTING);


	OCI2C_Enable();
	DEBUG_I2C_TRACE_INIT(1)
}


#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
/**
Menage Low Power Mode and wake up event setting.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Leonardo Sparti
\date		17/07/2012
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
IO_ErrorType OCI2C_LowPowerMode(Comm_Notification LPMCallback)
{
    if (LPMCallback != NULL)
    {
    	/* hw init - reset peripheral*/
    	I2C_SetSlaveAddress(0);
    	I2C_SetClockRate(0);
    	
    	I2C_CtrlReset();
    	I2C_IntRegReset();
      
    	I2C_ResetStatus();
    	I2C_Reset();
    	
    	/* hw init - set peripheral*/
    	I2C_SetSlaveAddress(I2C_SLAVEADDRESS);
    	I2C_SetClockRate(I2C_CLOCKRATE_SETTING);
        I2C_EnableWU();
#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
        I2C_DisableDMA();
#endif /* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */
    	I2C_Enable();
#if I2C_SLAVE_SUPPORT
    	I2C_IntEnable();
#endif
        OCI2C_LowPowerModeOn = 1;
        
        OCI2C_LPMCallback = LPMCallback;
        
        return (IO_ErrorType)IO_E_OK;
    }
    else
    {
        return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
    }
    
}


/**
Reset Low Power Mode condition.

\return

\retval

\author 	Leonardo Sparti
\date		17/07/2012
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Leonardo Sparti

*/
void OCI2C_ResetLowPowerMode(void)
{
    OCI2C_LowPowerModeOn = 0;

}

#endif /* defined(I2C_OCLPM_SUPPORT) */


/**
Sync I2C driver initialization. It enables I2C peripheral and intializes DMA driver. 

\param  [in]		initSync Input paramenter for I2C driver
This param allows to set:
\li the \c event-callback which is called when a transfer has been completed or, 
in slave mode, when peripheral is addressed in \e write or \e read mode;
\li the \c id: identifier of the channel used in notification.
\code 
	IO_PhyInitSyncType chParam={0};		//clean
	
	chParam.eventCB=<event callback identifier>;
	chParam.id=ch;
  
	OCI2C_InitSync(&chParam);  
\endcode

\return 	IO_ErrorType

\retval		IO_E_OK Initialization successfully completed. Always returned this value. 	 	 

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date		16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:	

\public \memberof OCI2C_Class
*/
IO_ErrorType OCI2C_InitSync(IO_PhyInitSyncType * initSync)
{
#if !defined(_HAL_DMA_CAPABILITY)
  OCI2C.DmaCh=0;
#else
		/* DMA channel and device selection */
    /* option 1 */
//	TBD Channel and device properties
    OCI2C.DmaCh=I2C_DMA_CH;
    OCI2C.DmaDevice=I2C_DMA_DEV;
    
 #endif  
    OCI2C.notifyCB=initSync->eventCB;
    OCI2C.id=initSync->id;
    
    OCI2C.CurrentSlave=0;

	OCI2C.TxLen=0;
	OCI2C.RxLen=0;
	OCI2C.state=0;
	OCI2C.options=0;

	
	OCI2C_InitHwSync();
	

	return (IO_ErrorType)IO_E_OK;
}


/**
Disables I2C drivers. With DMA, the DMA channel is deinitialized.

\param  [in]		deinitSync ignored.

\return 	IO_ErrorType

\retval		IO_E_OK	Deinitialization successuffully completed.	 	 

\warning	Does not disable DMA channel is SW DMA mode.		

\author 	Roberto Fioravanti
\date		16/mar/2009
\version	00.00.53

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:	
\public \memberof OCI2C_Class
*/
IO_ErrorType OCI2C_DeInitSync(void * deinitSync)
{

  (void) deinitSync;
  /* TODO: disabilita il canale DMA associato */
  
  I2C_SetSlaveAddress(0);
  OCI2C_Disable();
  
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
  return (IO_ErrorType)IO_E_OK;
#else 	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
    I2C_DisableDMA();
    return OCDMA_DeInitSync(OCI2C.DmaCh);
#endif  /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
}

#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
void OCI2C_SetDmaParamSync(IO_PacketSizeType len, IO_MemPtrType sar, IO_MemPtrType dar, bool_t tx, SWDMA_TransferType* transfer)
{
	transfer->bcr = len;
    transfer->sar = sar;
    transfer->dar = dar;
				
	transfer->channel = OCI2C.DmaCh;
#if !defined(_HAL_DMA_CAPABILITY)
	transfer->options.optionbyte = 		SWDMA_OPTION_SSIZE_BYTE|
  										SWDMA_OPTION_DSIZE_BYTE|
  										SWDMA_OPTION_CS|
  										SWDMA_OPTION_START;
#endif
  if(tx == 0)
  {
	  transfer->options.optionbyte |= SWDMA_OPTION_DINC;
  }
  else
  {
	  transfer->options.optionbyte |= SWDMA_OPTION_SINC;
  }
}
#endif /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */



/**

Starts transmission on I2C bus in both in master and slave mode making use of DMA tranfer.

\param  [in]		param pointer to Communication parameters
\code
	CommDriverParamType txRequestParam={0};		//clean 
	uint8_t bytecount=<number of bytes to send>;
	uint8_t txBuffer[<tx buffer size>];
	 
	requestParam.len = bytecount;
	requestParam.option|=IO_CLOSE_TX;				//closes the driver
	requestParam.buffer=txBuffer;
	
	OCI2C_TxSync(&txRequestParam);
\endcode

\return 	IO_ErrorType

\retval		\b IO_E_INVALID_VALUE: invalid \e buffer or \e length passed trough \e param. \n  
            \b IO_E_BUSY: Bus ii busy (only master). \n
            \b IO_E_OK: transmission successfully started. \n 		 	 

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date		16/mar/2009
\version	1.0.0

\bug		[Type bugs]

\todo		change name to OCI2C_TxAsync
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:	
\public \memberof OCI2C_Class
*/
IO_ErrorType OCI2C_TxSync(CommDriverParamType * param) 
{
#if !defined(_HAL_DMA_CAPABILITY)
    SWDMA_TransferType  transfer = {0};
#else
    OCDMA_TransferType  transfer = {0};
#endif   
  
    if(!param->buffer) 
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }
  
    if(!param->len) 
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }
  
    OCI2C.TxBuffer = param -> buffer;
    OCI2C.TxLen = param -> len;
    OCI2C.options = param -> option;    
      
    OCI2C.EOO_callback = param -> callback;  
  
    if(OCI2C.options & IO_OPT_MASTER) 
    {
        OCI2C.CurrentSlave = (I2C_SlaveAddressType) (param -> slaveAddress & 0xFE);

    if(I2C_IsBusBusy() != 0)
    {
      return (IO_ErrorType)IO_E_BUSY;
    }
    
        I2C_MasterEnable();
    }
  
    I2C_SetTxMode();
#if I2C_MASTER_SUPPORT    
    if(OCI2C.options & IO_OPT_MASTER) 
    {    
        /*
        01-Address byte is sent
        */
        I2C_DisableAcknowledge();
		/* TODO : new macro:
		I2C_MasterStart(slaveaddress)
		
		+ default actions
			- I2C_IssueStartCondition();
        	- OCI2C.Pending=TRUE;
	        - I2C_WriteData(OCI2C.CurrentSlave);
		
		+ NXP LPC1227 actions:
			- I2C_WriteData(OCI2C.CurrentSlave);
			- I2C_IssueStartCondition();
		    - OCI2C.Pending=TRUE;
		*/
        I2C_IssueStartCondition();
		OCI2C.Pending=TRUE;
		I2C_SendAddr(OCI2C.CurrentSlave);
    }
#endif /*I2C_MASTER_SUPPORT*/

#if I2C_MASTER_SUPPORT && I2C_SLAVE_SUPPORT 
    else
#endif /* I2C_MASTER_SUPPORT && I2C_SLAVE_SUPPORT */

#if I2C_SLAVE_SUPPORT		 
    {
        /* Slave 06 - Slave sends an answer */
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
	OCI2C_SetDmaParamSync(OCI2C.TxLen, OCI2C.TxBuffer, (uint8_t*)I2C_pTransmitDataRegister(), 1, &transfer);
 
        (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
        (void)SWDMA_SetTransferSync(&transfer);
        (void)SWDMA_StartTransferSync(OCI2C.DmaCh, OCI2C_SlaveCallBack);
#else	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
        transfer.bcr = OCI2C.TxLen;
        transfer.sar = OCI2C.TxBuffer;
        transfer.dar = I2C_pTransmitDataRegister();
        transfer.channel = (IO_ChannelType) (OCDMA_CHANNEL_ID(OCI2C.DmaCh) | OCDMA_CHANNEL_DEVICE(OCI2C.DmaDevice));

        transfer.addr = OCDMA_ADDR_SINC | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_CS;

        transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;  
        I2C_EnableDMA();

        (void)OCDMA_SetChannelDeviceSync(&transfer);
        OCDMA_SetTransferParamSync(&transfer);    
        OCDMA_SetTransferSync(&transfer);
        OCDMA_StartTransferSync(1, &transfer, OCI2C_SlaveCallBack);
#endif /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
        
        OCI2C.TxLen = 0;
        OCI2C.TxBuffer = (IO_MemPtrType)NULL;
    }
#endif /* I2C_SLAVE_SUPPORT */
    
	I2C_SlaveStartTx();
   
    return (IO_ErrorType)IO_E_OK;
}


/**
Starts data reception on I2C bus both in master and in slave mode making use of DMA tranfer.   

\param  [in]		param pointer to communication parameters
\code
	CommDriverParamType rxRequestParam={0};		//clean 
	uint8_t bytecount=<number of bytes to send>;
	uint8_t txBuffer[<tx buffer size>];
	 
	requestParam.len = bytecount;
	requestParam.option |= IO_CLOSE;				//closes the driver
	requestParam.buffer=rxBuffer;
	
	OCI2C_RxSync(&txRequestParam);
\endcode

\return 	IO_ErrorType

\retval		\b IO_E_INVALID_VALUE: invalid \e buffer or \e length passed trough \e param \n
            \b IO_E_INVALID_CHANNEL_ID: invalid DMA channel. \n
            \b IO_E_OK: reception successfully started. \n	 	 

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date		16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:	

\public \memberof OCI2C_Class
*/
IO_ErrorType OCI2C_RxSync(CommDriverParamType * param) 
{
    IO_ErrorType error = (IO_ErrorType)IO_E_OK;
    Comm_Notification callback = (Comm_Notification)NULL;
  
    if ((!param -> buffer) || (!param -> len)) 
    {
        return (IO_ErrorType)IO_E_INVALID_VALUE;
    }   
  
    OCI2C.RxBuffer = param -> buffer;
    OCI2C.RxLen = param -> len;
    OCI2C.options = param -> option;      
#if I2C_MASTER_SUPPORT
    if (I2C_IsTxMode()) 
    {
     /*
     05-trasmit address
     */
        I2C_IssueReStartCondition();
        I2C_SendAddr(OCI2C.CurrentSlave + 1);
#if defined(I2C_TXRX_SWITCH_BYDRV)
        OCI2C.state |= OCI2C_MSTRX_ONGOING;
#endif
    }
#endif
#if I2C_MASTER_SUPPORT && I2C_SLAVE_SUPPORT
    else
#endif
    {
#if !defined(_HAL_DMA_CAPABILITY)
        SWDMA_TransferType  transfer = {0};
#else
        uint8_t swStart = 1;

        OCDMA_TransferType  transfer = {0};

        transfer.channel = (IO_ChannelType) OCDMA_CHANNEL_ID(OCI2C.DmaCh);

#endif


#if I2C_MASTER_SUPPORT    
        if (OCI2C.options & IO_OPT_MASTER)
        {
            I2C_MasterEnable();
      
            transfer.bcr = OCI2C.RxLen;
      
            OCI2C_AckMngr();
      
            
            if (OCI2C.options & IO_CLOSE) 
            {                
                if (OCI2C.RxLen > 2)
                {
                    OCI2C.state |= OCI2C_RX_2ND_LAST;
                    transfer.bcr -= 2;
                }
                else if(OCI2C.RxLen == 2)
                {
                    transfer.bcr = 1;
                    I2C_DisableAcknowledge();
                }
            
            }
#if defined(_HAL_DMA_CAPABILITY)
            /* TOCHECK */
            else
            {
                swStart = 0;
            }
			/* TOCHECK */
            
            error = OCDMA_FollowTransferSync(&transfer);
#endif            
            callback = (Comm_Notification)OCI2C_CallBack;
           
        } 
#endif /* I2C_MASTER_SUPPORT	*/

#if I2C_MASTER_SUPPORT && I2C_SLAVE_SUPPORT
        else 
#endif
				
#if I2C_SLAVE_SUPPORT				
        {
            transfer.bcr=OCI2C.RxLen;
          
            if(OCI2C.options & IO_CLOSE) 
            {        
                /*
                    Slave 03 - 2 bytes to be received
                */

                callback=(Comm_Notification)OCI2C_SlaveCallBack;

#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
                swStart = 0;
                
                error = OCDMA_FollowTransferSync(&transfer);
#endif	/* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

            }
            else 
            {
                /*
                    Slave 02 - 5 bytes to be received
                */

			callback=(Comm_Notification)OCI2C_Notification;
			
#if defined(I2C_OCDMA_SUPPORT)  && defined(_HAL_DMA_CAPABILITY)
                
                transfer.sar = I2C_pReceiveDataRegister();
                transfer.dar = OCI2C.RxBuffer;
                error = OCDMA_SetTransferSync(&transfer);
#endif	/* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

            }
        }
#endif /*I2C_SLAVE_SUPPORT				*/
				
  
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
		OCI2C_SetDmaParamSync(transfer.bcr, (uint8_t*)I2C_pReceiveDataRegister(), OCI2C.RxBuffer, 0, &transfer);

        (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
        error = SWDMA_SetTransferSync(&transfer);
        error = SWDMA_StartTransferSync(OCI2C.DmaCh, callback);
#else	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */

        transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;
        error = OCDMA_StartTransferSync(swStart, &transfer, callback);
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
    
        OCI2C.RxBuffer += transfer.bcr;
    }
  
    return error;
}


/**
I2C Driver event manager for master and slave.
Call this method in the I2C interrupt.

Nested interrupt, if supported by HAL, are allowed by the define I2C_NESTING_ISR_ENABLE


\return 	void

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date		16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note 2:	
			\li OSEK (S08 GBXX, )
			\code
			ISR(Isr_I2C)
			{
			  OCI2C_ISR();
			}
			\endcode
	
			\li MCF51_AG128
			interrupt 99
			void Isr_I2C(void)
			{
				OCI2C_ISR();
			}
			
\public \memberof OCI2C_Class
*/
void OCI2C_ISR(void)
{    
	DEBUG_I2C_TRACE_IRQ(1)
	IRQ_MONITOR_ENTER();

#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
	if (OCI2C_LowPowerModeOn)
	{
		if (OCLPM_Callback((IO_NotificationType)0, (IO_ChannelType)LPM_ID_I2C, (IO_ErrorType)IO_E_OK) == IO_E_OK)
		{
			OCI2C_LowPowerModeOn = 0;

			I2C_OnEnterIsr();

			if ( LIB_Reset_GetSystemMode() == SYSTEM_SLEEP_MODE )
			{
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY)
				OCCRC_InitSync();
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) */
				/* Si reimposta il NewSystemMode */
				LIB_Reset_SetNewSystemMode( (SysMode_Type)SYSTEM_NORMAL_MODE );

				/* Viene forzato un reset freddo */
				LIB_Reset_ForceColdReset();
			}
		}
	}
	else
#endif /* defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT) */
	{
		I2C_NestedIsr_Begin();

#ifdef I2C_ISR_CALLBACK
		I2C_ISR_CALLBACK();
#endif

		OCI2C_OnActivity();

		I2C_OnEnterIsr();

#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
	  if(
			!(
				(
				( I2C_IsSlaveAddressed() && ( !I2C_IsSlaveTrasmitRequest() ) )
#if I2C_MASTER_SUPPORT
				|| ( I2C_IsArbitrationLost() )
#endif
				)
		 /*|| (I2C_IsTxMode() && I2C_ReceivedAcknowledgeVal()) */
			)
			&& (SWDMA_CheckChannelEnableSync(OCI2C.DmaCh) == (IO_ErrorType)IO_STATE_ACTIVE)
		)
		{
		  (void)SWDMA_ExeTransferMngrSync(OCI2C.DmaCh);
		  I2C_OnIntAck();
		}

	else
#endif /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
	{

#if I2C_SLAVE_SUPPORT
		if(!I2C_IsMaster())
		{
			/*
			Slave 01 - Address byte is received
			Slave 05 - Address byte for the transmission
			*/
			OCI2C_SlaveMngr();
		}
#endif
#if I2C_MASTER_SUPPORT
		if(I2C_IsMaster())
		{
			OCI2C_MasterMngr();
		}
#endif
}

	  I2C_NestedIsr_End();
	}

	IRQ_MONITOR_EXIT();
	DEBUG_I2C_TRACE_IRQ(0)
}

/**
Updates Bus Status

This method is available if I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE.

\return 	void

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			10/dec/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
void OCI2C_UpdateBusStatusSync(void) 
{
  I2C_StatusType I2CStatus = (I2C_StatusType)OCI2C_IsBusBusy();
    
  if(I2CStatus != OCI2C.BusStatus)
  {
    /* status is changed */
    
    
#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE)
    if(OCI2C.BusBusyCount < 0xFFFF)
    {
      OCI2C.BusBusyCount++;
    }
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE) */

#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE)
    OCI2C.Pending=TRUE;       
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE) */
    
    OCI2C.BusStatus = I2CStatus;  /* update old value */
  }  
}

/**
I2C Driver Level1 block monitor.
Level1: at least one line of the bus kept low indefinitely.

The implementation of this method is affected by I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC.

\return 	bool_t

\retval 	FALSE		regular activity
\retval 	TRUE		unblock

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level1BlockMonitorSync(void)
{
  bool_t retval = FALSE;
  
#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE)
  I2C_StatusType I2CStatus = (I2C_StatusType)OCI2C_IsBusBusy();
  
  if(I2CStatus != 0) 
  { 
    if (OCI2C.Pending == TRUE) 
    {
      OCI2C.Pending = FALSE;
    }
    else if(OCI2C_Locked() == TRUE) 
    {
      OCI2C_Action_Lev1();
      OCI2C.Block2Cnt++;
			
      retval = TRUE;
    }
  }  
  
  OCI2C.BusBusyCount = 0;  //I2CClearCheck();
  
  OCI2C.BusStatus = I2CStatus;
  
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE) */



#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE)  
  
  
  OCI2C_UpdateBusStatusSync();

  if(OCI2C.BusStatus != 0) 
  { 
    if (OCI2C.Pending == TRUE) 
    {
      OCI2C.Pending = FALSE;
      
      OCI2C.Block2Cnt=0;
      OCI2C.BusBusyCount=0;
    } else 
    {
      
      if(OCI2C_Locked()) 
      {
        uint8_t cnt=0;

      	do
        {
			/* lev1 action */
			OCI2C_Action_Lev1();
			cnt++;
		}
		while (OCI2C_IsBusBusy() && (cnt < I2CMONITOR_LEV1_LOCAL_OCCURS));
        
        
        if(cnt<I2CMONITOR_LEV1_LOCAL_OCCURS)
        {
            /* only if bus is not busy , resets activity */
            OCI2C_OnActivity();
        }
        else
        {
            /* increment OCI2C.Block2Cnt to trigger level 2 unblock */
            OCI2C.Block2Cnt++;
        }
        
        retval = TRUE;
		
        
        /* reset lev 1 cnt */
        OCI2C.BusBusyCount=0;
      }
      
      OCI2C.BusBusyCount++;
    }
  }
	else
	{
		OCI2C.Block2Cnt=0;
		OCI2C.BusBusyCount = 0; 
	}
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE) */
    
  return (bool_t)retval;
}

/**
I2C Driver Level2 block monitor.
Level2: busy bit high indefinitely although attempts by level 1 routines.
Each time level 1 attempts cannot unblock the bus, internal counter is incremented.
Level 2 unblock occurs when internal counter is greater than I2CMONITOR_LEV1_GLOBAL_OCCURS.
Unblock is the reset of the mcu.
This method is affected by I2CMONITOR_LEV1_GLOBAL_OCCURS property.

\return 	void

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level2BlockMonitorSync(void)
{
  if(OCI2C.Block2Cnt >= I2CMONITOR_LEV1_GLOBAL_OCCURS && OCI2C_IsBusBusy()) 
  {   
		OCI2C.Block2Cnt = 0;
		OCI2C_OnActivity();
    
		return TRUE;
  }
  return FALSE;
}

/**
I2C Driver Level3 block monitor.
Level3: wrong peripheral configuration: ACK enable bit = 0 and Slave-TX o Master-TX.

\return 	bool_t

\retval 	FALSE		regular activity
\retval 	TRUE		level3 block detected

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level3BlockMonitorSync(void)
{
    volatile bool_t res = (bool_t) I2C_Level3BlockMonitorSync();
    return (res && (res == I2C_Level3BlockMonitorSync()));
}

/**
I2C Driver Level4 block monitor.
Level4: wrong peripheral configuration in slave mode: 	ACK enable bit = 1 and Slave-RX.

\return 	bool_t

\retval 	FALSE		regular activity
\retval 	TRUE		level4 block detected

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level4BlockMonitorSync(void)
{
    volatile bool_t res = (bool_t) I2C_Level4BlockMonitorSync();
    return (res && (res == I2C_Level4BlockMonitorSync()));
}

/**
I2C Driver Level5 block monitor.
Level5: wrong peripheral configuration in slave mode: when bus is free (busy = 0), only slave Rx accepted.

\return 	bool_t

\retval 	FALSE		regular activity
\retval 	TRUE		level5 block detected

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level5BlockMonitorSync(void)
{  
	return (!OCI2C.BusStatus && I2C_Level5BlockMonitorSync());
}

/**
I2C Driver Level6 block monitor.
Level6: timeout for no request to slave.

This method is affected by I2CMONITOR_NOACTIVITY_TIMEOUT.

\return 	bool_t

\retval 	FALSE		regular activity
\retval 	TRUE		level6 block detected

\warning	[Type warnings]		

\author 	Roberto Fioravanti
\date			16/mar/2009
\version	1.0.0

\bug		[Type bugs]
   
\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
\public \memberof OCI2C_Class
*/
bool_t OCI2C_Level6BlockMonitorSync(void)
{
   if(!OCI2C.NoActivityTimeout) 
   {    
      
      OCI2C_InitHwSync();			
      OCI2C_OnActivity();
      
      return TRUE;
   }
   
   OCI2C.NoActivityTimeout--;
   return FALSE;
}

/* ************** */
/* Local Function */
/* ************** */
/**

Sets slave rx configuration
(bit di acknoledge abilitato, SDA in ricezione)

\warning		

\note:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
void OCI2C_SlaveRxCfg(void) 
{  
  I2C_SetRxMode();  
  I2C_EnableAcknowledge();
}
/**
 
Generates stop condition

\warning		

\note:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
void OCI2C_MasterStopComm(void) 
{
#if defined(I2C_TXRX_SWITCH_BYDRV)
	OCI2C.state &= ~OCI2C_MSTRX_ONGOING;
#endif
  if (I2C_IsBusBusy() && I2C_IsMaster())
  {
		I2C_IssueStopCondition();
		OCI2C_SlaveRxCfg();
		I2C_MasterDisable();
	}
}

/**
Abilita o disabilita il bit di acknoledge in base al numero di byte da ricevere.
Se il numero di byte e' 1, allora disabilita il bit di acknoledge.
 
\warning		

\note:usare solo in modalità di ricezione

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
void OCI2C_AckMngr(void)
{
  if(OCI2C.RxLen==1) 
  {
    I2C_DisableAcknowledge();
  }
  else
  {
    I2C_EnableAcknowledge();
  }
}


/**
Gestione slave I2C basata su DMA
 
\warning		

\note:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
void OCI2C_SlaveMngr(void)
{
#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
    OCDMA_TransferType  transfer = {0};
    IO_ErrorType error = IO_E_OK;
#endif	/* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

	if(I2C_IsSlaveAddressed()) 
	{
		I2C_ResetSlaveAddressedSig();
    
		if(I2C_IsGeneralCallAddress()) 
		{
			/* non accettare il broadcasting */
			I2C_ResetGeneralCallAddressSig();
			return;
		}

#if I2C_MASTER_SUPPORT == TRUE    
	    if (I2C_IsArbitrationLost())
		{				
			/*Arbitration Lost?*/
			I2C_ResetArbitrationLost();
	      	OCI2C.TxLen=0;
	      	OCI2C.RxLen=0;
		}
#endif /* (I2C_MASTER_SUPPORT == TRUE) */		
		
		if (!I2C_IsSlaveTrasmitRequest())
		{
			uint8_t dat;			
			
			/* Slave rx */
			I2C_SlaveStartRx();
			
			OCI2C_SlaveRxCfg();
			
			OCI2C.RxLen=0;
									
			/* Inizio Ricezione dati */
			if(OCI2C.notifyCB) 
			{
			  /* notify about Read data init */
				(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_READ_DATA_INIT, OCI2C.id, (IO_ErrorType)IO_E_OK);
			}

#if ! (defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY))
			/* disable swdma */
			SWDMA_ChannelDisableSync(OCI2C.id);
#endif /* ! (defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)) */

			/* Dummy reading to initiate the first byte receiving */
			dat=I2C_ReadData();
			I2C_ProcessRxRequest(dat);
		}

#if OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR    
	else
	{
		I2C_OnSlaveAddressMatchTransmit()
		/* notification */
		if (OCI2C.notifyCB)
		{
            (OCI2C.notifyCB)(IO_N_TRANSFER_DONE, OCI2C.id, IO_E_OK);		    		    
            (OCI2C.notifyCB)(IO_N_WRITE_DATA_INIT, OCI2C.id, IO_E_OK);
		}												
			    
        I2C_SlaveAddressedTx_Action();
	}
#endif  /* OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR */



  } else 
  {

#if (I2C_MASTER_SUPPORT == TRUE)
    if (I2C_IsArbitrationLost())
	{
		I2C_ResetArbitrationLost();
		OCI2C.TxLen=0;
		OCI2C.RxLen=0;
	}
#endif /* (I2C_MASTER_SUPPORT == TRUE) */		
		
    	if(I2C_IsTxMode())
		{
			if (I2C_ReceivedAcknowledgeVal()==TRUE)
			{			
				OCI2C_SlaveStopCommSync();
			}
			else
			{	
				if(!OCI2C.TxLen && OCI2C.notifyCB && bCallbackEnable)
			    {
					bCallbackEnable = FALSE;
					/* notification */
					(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_TX_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
			    } else
				{
		        	bCallbackEnable=FALSE;
		        	I2C_WriteData(0x55);
					OCI2C_SlaveStopCommSync();
				}
			}
		}
		else
		{ 
			/*device in RX mode*/
		    if(!OCI2C.RxLen && OCI2C.notifyCB)
		    {
#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
                transfer.channel = (IO_ChannelType) (OCDMA_CHANNEL_ID(OCI2C.DmaCh) | OCDMA_CHANNEL_DEVICE(OCI2C.DmaDevice));

                transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_CS;

                transfer.perlink = OCDMA_PERLINK_D_ERQ;

                error = OCDMA_SetChannelDeviceSync(&transfer);            
                error = OCDMA_SetTransferParamSync(&transfer);
                
                I2C_EnableDMA();
#endif	/* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

		      	/* notification */
		    	(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
		    } else
		    {
				OCI2C.state |= OCI2C_UNEXPECTED_DATA_RX;
				
				I2C_OnSlaveRxDummy();
#if ! (defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY))				
				SWDMA_ChannelDisableSync(OCI2C.id);
#endif /* ! (defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY))*/
		    }

			I2C_OnIntAck()
		}
  }
}

/**
Gestione master I2C basata su DMA
 
\warning		

\note:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
#if I2C_MASTER_SUPPORT
void OCI2C_MasterMngr(void)
{
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
    SWDMA_TransferType  transfer = {0};
#else	/* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */
    OCDMA_TransferType  transfer = {0};
#endif   /* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

    if(
    		I2C_IsTxMode()
#if defined(I2C_TXRX_SWITCH_BYDRV)
    		|| (OCI2C.state & OCI2C_MSTRX_ONGOING)
#endif
    	)
    {
        if(I2C_ReceivedAcknowledgeVal() == 0) 
        {
    
            if(OCI2C.TxLen) 
            {		
            /*
            02-send the all the request
            */
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
				OCI2C_SetDmaParamSync(OCI2C.TxLen, OCI2C.TxBuffer, I2C_pTransmitDataRegister(), 1, &transfer);
#else	/* !defined(I2C_OCDMA_SUPPORT) && !defined(_HAL_DMA_CAPABILITY) */
                transfer.channel = OCDMA_CHANNEL_ID(OCI2C.DmaCh) | OCDMA_CHANNEL_DEVICE(OCI2C.DmaDevice);

                transfer.bcr = OCI2C.TxLen;
                transfer.sar = OCI2C.TxBuffer;
                transfer.dar = I2C_pTransmitDataRegister();

                transfer.addr = OCDMA_ADDR_SINC | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_CS;

                transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;

                (void)OCDMA_SetChannelDeviceSync(&transfer);
                OCDMA_SetTransferParamSync(&transfer);    
#endif	/* !defined(I2C_OCDMA_SUPPORT) && !defined(_HAL_DMA_CAPABILITY) */

                OCI2C.TxLen = 0;
                OCI2C.TxBuffer = NULL;
            }
            else 
            {
             /*TOCHECK*/
                if((bCallbackEnable==TRUE) && (OCI2C.EOO_callback))
                {
                 /*
                 04-trasmit address in RxSync
                 */
                    (void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
                    bCallbackEnable=FALSE;
                    return;
                }
	         /*TOCHECK*/
                
                /* dummy */
                if((OCI2C.state & OCI2C_RX_DUMMY) ==0) 
                {
                	OCI2C.state |= OCI2C_RX_DUMMY;
                }

                if(OCI2C.options & IO_CLOSE) 
                {
                    OCI2C_AckMngr();
                }
                else 
                {          
                    I2C_EnableAcknowledge();
                }
            
                I2C_SetRxMode();
#if defined(I2C_TXRX_SWITCH_BYDRV)
                OCI2C.state &= ~OCI2C_MSTRX_ONGOING;
#endif
                /*
                 06- dummy read and lanch dma to read the first byte
                 */
            

#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)

#else /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
                transfer.channel = OCDMA_CHANNEL_ID(OCI2C.DmaCh) | OCDMA_CHANNEL_DEVICE(OCI2C.DmaDevice);
                
                transfer.addr = OCDMA_ADDR_DINC | OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_CS; 

                transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;
                
                transfer.bcr = OCI2C.RxLen;
                transfer.sar = I2C_pReceiveDataRegister();
                transfer.dar = OCI2C.RxBuffer;

                I2C_EnableDMA();

                OCDMA_SetChannelDeviceSync(&transfer);
                OCDMA_SetTransferParamSync(&transfer);
                OCDMA_SetTransferSync(&transfer);  
                OCDMA_StartTransferSync(0 ,&transfer, OCI2C_CallBack);
#endif /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */

                (void)I2C_DummyRead();
                return;
            }


#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
            (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);          
            (void)SWDMA_SetTransferSync(&transfer);                               
      			(void)SWDMA_StartTransferSync(OCI2C.DmaCh, (Comm_Notification)OCI2C_CallBack);        
#else	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
                I2C_EnableDMA();
                OCDMA_SetTransferSync(&transfer);
                OCDMA_StartTransferSync(1, &transfer, (Comm_Notification)OCI2C_CallBack);
#endif	/* !defined(I2C_OCDMA_SUPPORT) && !defined(_HAL_DMA_CAPABILITY) */
    
        }
        else
        {
            OCI2C_MasterStopComm();
          
            if(OCI2C.EOO_callback)
    	    {
    	        /* notifica */
            	(void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_ERROR, OCI2C.id, (IO_ErrorType)IO_E_ACK_BUS);
    	    }
        }
    }
    else 
    {
        if((OCI2C.state & OCI2C_RX_DUMMY)) 
				{
        	(void)OCI2C_CallBack((IO_NotificationType)IO_N_TRANSFER_DONE, 0, (IO_ErrorType)IO_E_OK);
          return;
        }

#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
        else if ((OCI2C.options & IO_CLOSE))
        {
        
            /*
             11 - send stop condition and save last byte
             */
         
            OCI2C.options &= (~IO_CLOSE);
        
            OCI2C_MasterStopComm();

            bCallbackEnable = TRUE;

            /* Deinitializes and release the DMA channel */
            OCDMA_DeInitSync(OCI2C.DmaCh);
            
            *OCI2C.RxBuffer = *I2C_pReceiveDataRegister();

        }

        if((bCallbackEnable==TRUE) && (OCI2C.EOO_callback))
        {
            (void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
            bCallbackEnable=FALSE;
            return;
        }
#endif  /* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */
    
        if(OCI2C.EOO_callback)
        {
            /* notify about nack */
        	(void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_ERROR, OCI2C.id, (IO_ErrorType)IO_E_UNEXPECTED_CONDITION);
        }
    }
  
}
#endif /* I2C_MASTER_SUPPORT */


/*

*/
void OCI2C_SlaveStopCommSync(void)
{
#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
    /*TODO*/
	I2C_DisableDMA();
    OCDMA_DeInitSync(OCI2C.DmaCh);
#endif /* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */

	/* NACK received */
	I2C_ResetTxSig()								
	OCI2C_SlaveRxCfg();

	
	I2C_DummyRead();
	
	I2C_ResetTxSig()

	I2C_Reset() /* consente di ovviare alla trasmisione del treno di clock successivo al nack bit durante la comunicazione con bridge 4.0.17 */
	if(OCI2C.notifyCB)
	{
		/* notifica */
		(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_COMPLETED, OCI2C.id, (IO_ErrorType)IO_E_OK);
	}

	OCI2C.RxLen=1;/*0xFF;*/ /*il bridge puo' inviare un treno di clock intenzionalmente dopo il nack bit:
										in questo modo, dopo aver configurato la periferica in slave receive, nel ramo di ricezione non si esegue la callback .notifyCB*/
}


/**
Disables I2C peripheral
*/
void OCI2C_Disable(void)
{
#if I2C_SLAVE_SUPPORT
	I2C_IntDisable();
#endif
    I2C_Disable();
}

/**
Enables I2C peripheral
*/
void OCI2C_Enable(void)
{
    I2C_Enable();
#if I2C_SLAVE_SUPPORT
	I2C_IntEnable();
#endif
}

/**
Callback del driver slave.
\param [in] 	notifVal notification code
\param [in]		channel	channel id
\param [in] 	error code


\warning		

\note:

\version		1.0.0
\date 			16/03/2009
\author			Roberto Fioravanti
*/
IO_ErrorType OCI2C_SlaveCallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{
    (void) notifVal;
    (void) channel;
    (void) error;

#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)

    /*
    Slave 04 - Message received, disables DMA and waits for the I2C interrupt
    Slave 06 - All bytes of message sent to data register, disables DMA and enables the I2C interrupt 
    */


    /* Disables the DMA on I2C */
    I2C_DisableDMA();
    OCI2C_OnActivity();
    /* Deinitializes and release the DMA channel */
    OCDMA_DeInitSync(OCI2C.DmaCh);
    
    if (I2C_IsTxMode())
    {
	    if (OCI2C.options & IO_CLOSE_TX) 
	    {
	        OCI2C.options &= (~IO_CLOSE_TX);
	        if (I2C_IsTrasmitCompleted() && I2C_ReceivedAcknowledgeVal() == TRUE)
	        {
	            OCI2C_SlaveStopCommSync();
	            OCI2C_InitHwSync();
	        }
            
	        bCallbackEnable=FALSE;
      
	    } else 
	    {
      		bCallbackEnable=TRUE;
        }
    }
    
#endif /* defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY) */
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)

    if (OCI2C.options & IO_CLOSE) 
    {
        OCI2C.options &= (~IO_CLOSE);      
#if OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK

        if (I2C_IsSlaveTrasmitRequest() && I2C_IsSlaveAddressed())
        {	
			
			I2C_OnSlaveAddressMatchTransmit();			
			
			I2C_SlaveStartTx();
    						
    		/* notifica */
    		if(OCI2C.notifyCB)
    		{
    			(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
			    (void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_WRITE_DATA_INIT, OCI2C.id, (IO_ErrorType)IO_E_OK);
    		}


			return IO_E_OK;
    	}
#endif  /* OCI2C_ON_SLAVE_TX_EVENT_IN_DMACALLBACK */


#if OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK    	
    	if(I2C_IsTxMode())
    	{
			if (I2C_ReceivedAcknowledgeVal()==TRUE)
			{			
				OCI2C_SlaveStopCommSync();
			}
			else
			{
				I2C_WriteData(0xFF);
	    	}
    	}
#endif /* OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK  */
    } else
    	
    	
#if OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK    	
    	if (I2C_IsTxMode())
		{
			if (I2C_ReceivedAcknowledgeVal()==TRUE)
			{			
				OCI2C_SlaveStopCommSync();
			} else
			{	
				if(!OCI2C.TxLen && OCI2C.notifyCB)
		    	{
					/* notifica */
					(void)(OCI2C.notifyCB)((IO_NotificationType)IO_N_TX_DONE, OCI2C.id, (IO_ErrorType)IO_E_OK);
			    } else
				{
        			I2C_WriteData(0x55);
				}
			}
		}
#endif /* OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK  */
    
#endif
		
		return IO_E_OK; 
}



/**
Callback del driver master.
\param [in]	notifVal notification code
\param [in]	channel channel id
\param [in]	error error code

\warning		

\note

\version		1.0.0
\date 			17/03/2009
\author			Roberto Fioravanti
*/
#if I2C_MASTER_SUPPORT
IO_ErrorType OCI2C_CallBack(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{ 
#if !defined(_HAL_DMA_CAPABILITY)
    SWDMA_TransferType  transfer = {0};
#else
    OCDMA_TransferType  transfer = {0};
#endif   

    uint8_t  bCallbackEn=TRUE;


#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
    transfer.channel = OCI2C.DmaCh;
#else	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
    transfer.channel = OCDMA_CHANNEL_ID(OCI2C.DmaCh) | OCDMA_CHANNEL_DEVICE(OCI2C.DmaDevice);

    transfer.addr = OCDMA_ADDR_DSIZE_BYTE | OCDMA_ADDR_SSIZE_BYTE | OCDMA_ADDR_CS;
    
    transfer.perlink = OCDMA_PERLINK_ERQ | OCDMA_PERLINK_D_ERQ;
    
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
  
    /*TODO: Assertion on channel value */
    /*TODO: Process error */
  
    if (OCI2C.state & OCI2C_RX_2ND_LAST) 
    {
        /*
         08 - save 2nd last data to buffer 
         */
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
    	OCI2C_SetDmaParamSync(1, I2C_pReceiveDataRegister(), OCI2C.RxBuffer, 0, &transfer);
    	
#else /* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
        transfer.bcr = 1;
        transfer.sar = I2C_pReceiveDataRegister();
        transfer.dar = OCI2C.RxBuffer;

        transfer.addr |= OCDMA_ADDR_DINC;
    
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
        
        OCI2C.RxLen = 1;
		/*TOCHECK*/
        OCI2C.RxBuffer++;
		/*TOCHECK*/
        
        I2C_DisableAcknowledge();
    
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
        (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
        (void)SWDMA_SetTransferSync(&transfer);  
    		(void)SWDMA_StartTransferSync(OCI2C.DmaCh, (Comm_Notification)OCI2C_CallBack);
#else

        I2C_EnableDMA();

        OCDMA_FollowTransferSync(&transfer);  
        OCDMA_StartTransferSync(0, &transfer, (Comm_Notification)OCI2C_CallBack);

#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
    
        OCI2C.state &= (~OCI2C_RX_2ND_LAST);
        bCallbackEn = FALSE;
    }
    else if(OCI2C.state & OCI2C_RX_DUMMY) 
    {
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
		OCI2C_SetDmaParamSync(OCI2C.RxLen, I2C_pReceiveDataRegister(), OCI2C.RxBuffer, 0, &transfer);
#else

        transfer.bcr = OCI2C.RxLen;
        transfer.sar = I2C_pReceiveDataRegister();
        transfer.dar = OCI2C.RxBuffer;

        transfer.addr |= OCDMA_ADDR_DINC;
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */



        OCI2C.state &= (~OCI2C_RX_DUMMY);

   
        if (OCI2C.options & IO_CLOSE) 
        {
            /* last byte */           
            OCI2C_MasterStopComm();
            OCI2C.options &= (~IO_CLOSE);

           /*
         09-disable dma and wait for I2C interrupt to read last byte
         */
         
#if defined(I2C_OCDMA_SUPPORT) && defined(_HAL_DMA_CAPABILITY)
            /* Deinitializes and release the DMA channel */
            OCDMA_DeInitSync(OCI2C.DmaCh);

            /* Disables the DMA on I2C */
            I2C_DisableDMA();
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */

        }
        else 
        {  

#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
            (void)SWDMA_ChannelEnableSync(OCI2C.DmaCh);
#else

#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */

            bCallbackEn = FALSE;
        }

        /*
         07-set dma to receive until len 
         */
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
        (void)SWDMA_SetTransferSync(&transfer);
        (void)SWDMA_StartTransferSync(OCI2C.DmaCh, OCI2C_CallBack);
#else
        (void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, error);

#endif    
    } 
    else if(OCI2C.options & IO_CLOSE) 
    {
        /* last byte */           
        /*
         10 - wait for last byte is transferred on the bus , disable DMA, (then go in I2C)  
         */
		
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
		/*TOCHECK*/
        OCI2C_MasterStopComm();
		/*TOCHECK*/
         
		 OCI2C_SetDmaParamSync(1, I2C_pReceiveDataRegister(), OCI2C.RxBuffer, 0, &transfer);

		/*TOCHECK*/
        OCI2C.options &= (~IO_CLOSE);
		/*TOCHECK*/

        (void)SWDMA_SetTransferSync(&transfer);  
        (void)SWDMA_StartTransferSync(OCI2C.DmaCh, NULL);       
        /* Disables the DMA on I2C */
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)*/
    }


    if((bCallbackEn==TRUE) && (OCI2C.EOO_callback))
    {
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
        (void)(OCI2C.EOO_callback)((IO_NotificationType)IO_N_TRANSFER_DONE, OCI2C.id, error);
#else        
        /* Disables the DMA on I2C */
        I2C_DisableDMA();
        
         /*
         03-continues on I2C interrupt to send restart cond and address
         */
        
#endif	/* !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY) */
    }
	
	return IO_E_OK;
}
#endif /*I2C_MASTER_SUPPORT*/

void OCI2C_Notification(IO_NotificationType notifVal, IO_ChannelType channel, IO_ErrorType error)
{
    (void)channel;
#if !defined(I2C_OCDMA_SUPPORT) || !defined(_HAL_DMA_CAPABILITY)
    (void)(OCI2C.notifyCB)(notifVal, OCI2C.id, error);    
#else
 	if((OCI2C.notifyCB)(notifVal, OCI2C.id, error)==IO_E_INVALID_VALUE)
	{
    	OCDMA_DeInitSync(I2C_DMA_CH);
	}
#endif
}


bool_t OCI2C_Locked(void)
{

#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE)
	return (bool_t) (OCI2C.BusBusyCount > (I2CMONITOR_BUSY_BLOCK_TIME_LEV1/PERIPHERAL_MONITOR_POLL));
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == TRUE) */

#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE) 
  return (!OCI2C.BusBusyCount);
#endif /* (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE) */
}

void OCI2C_ClearCheck(void) 
{
  OCI2C.BusBusyCount=0;
}



/** 
   Test the I2C bus.
*/
I2C_StatusType OCI2C_IsBusBusy(void)
{
	I2C_StatusType tmpValue = 0;

	if (
			I2C_IsTxMode()
#if defined(I2C_TXRX_SWITCH_BYDRV)
			|| (OCI2C.state & OCI2C_MSTRX_ONGOING)
#endif
			)
	{
		tmpValue |= I2C_BUS_STATUS_TX;
	}

	if (I2C_IsBusBusy())
	{
		tmpValue |= I2C_BUS_STATUS_BUSY;
	}

#if (I2C_BUS_MONITOR_SDA == TRUE)
	if (!I2C_IsSdaIoHigh())
	{
		tmpValue |= I2C_BUS_STATUS_SDA;
	}
#endif /* (I2C_BUS_MONITOR_SDA == TRUE) */

	if (!I2C_IsSclIoHigh())
	{
		tmpValue |= I2C_BUS_STATUS_SCL;
	}

    return tmpValue;
}

/** 
  Level 1 unblock. 
  re-initialization
*/
void OCI2C_Action_Lev1(void)
{
  
  OCI2C_InitHwSync();
#if defined(I2C_TXRX_SWITCH_BYDRV)
  OCI2C.state &= ~OCI2C_MSTRX_ONGOING;
#endif
  OCI2C.TxLen=0;
  OCI2C.RxLen=0;
}
/** 
  Level 2 unblock
  Reset mcu. Critical lock
*/
void OCI2C_Action_Lev2(void)
{
#if defined(CONFIG_PL_DRV_OCI2C_USE_CUSTOM_RESET)
	SW_RESET_MCU();
#else
	OCRST_GenerateUserResetSync();
#endif
}


/* *********** */
/* End of file */
/* *********** */
