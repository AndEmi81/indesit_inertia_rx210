/*
Copyright (c) 2004-2009, Indesit Company, I.D.D.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_BusCOMM
@{
\n	  Communication Platform is a Platform2 component providing a communication framework with multi-driver support.
\n    All the relevant structures (buffer dimension, buffer sharing,..), priorities and modules (unblock routines)
      are parametrized and can be changed from the caller application
\n    Communication Platform is organized in three separated levels:
      \li Communication Manager,
      \li Platform Protocol,
      \li Driver.

\link CM Communication Manager \endlink and Platform Protocol are part of the Communication Platform.
Drivers is another component of Platform2 containing drivers not only related to the commnunication.

\n
The mail goal of this component is:
\li Platformize this crucial piece of code among all the applications
\li Reduce the communication stress on the bus
\li Easy way to build an application in partnership that communicate with Indesit I2C Bus

\n
\n    Bus Communication Platform needs the following Platform 2 components:
\li HAL_General: general Hardware Abstraction Layer support.
\li MCU-specific HAL: HAL of the target MCU.
\li Drivers: peripheral drivers
\li PM_Library, Library

\par Configuration:
\n
\n
\par Exported Functions:
\n
\n

@}
\ingroup ProductM
*/

/**
\defgroup	CM Communication Manager
@{

\par		Description
\n      Communication Manager is the application interface of the Bus Communication Platform.
\n      It can work in two ways:
        \li master mode
        \li slave mode

\n      Master mode is described in \link CRM  Communication Request Manager \endlink
\n      Slave mode is described in \ref CP_CSM

\link CM More... \endlink
@}
\ingroup PM_BusCOMM
*/

/**
\defgroup CSM Communication Slave Manager
\ingroup  CM



\par		Description
\n    Communication Slave Manager is the application interface of Bus Communication Platform working in 'slave' mode.
Features:
	\li \ref CSM_SAM
	\li \ref CSM_EOSN
*/

/**
    \defgroup CRM  Communication Request Manager
    \ingroup CM
    Communication Slave Manager is the application interface of Bus Communication Platform working in 'master' mode..
*/


/**
\n
\par		Description
\n		This file is part of the Communication Request Manager and contains the implementations of the requests of the application.
\n
\file		BCP_BusCommMng.c
\ingroup	CM
\date		05/05/2009
\version	01.00.00
\author		Gabriele Merlonghi
\n
\n
\par		History
\n 		00.00.75
\n		28/10/2010
\n		Roberto Fioravanti
\n		changes
\n 		- deleted obsolete code
\n		- updated documentation
\n
\n 		00.00.43
\n		28/04/2010
\n		Roberto Fioravanti
\n		changes
\n 		- BCM_SetPacketRequest (no more available).
\n
\n 		00.00.38
\n		08/03/2010
\n		Roberto Fioravanti
\n		changes
\n 		- CRM_SetPacketParamSync replaces BCM_SetPacketRequest (deprecated).
\n
\n 		00.00.26
\n		08/12/2010
\n		Roberto Fioravanti
\n		changes
\n 		- Doc updated
\n
\n 		00.00.26
\n		08/12/2010
\n		Roberto Fioravanti
\n		changes
\n 		- Unblock Routines updated
\n
\n 		00.00.14
\n		23/10/2009
\n		Gabriele Merlonghi
\n		changes
\n 		- Requests priority in array COMM_MNG(idChannel).PacketReq refered to order defined by application.
\n
\n 		00.00.12
\n		08/10/2009
\n		Gabriele Merlonghi
\n		changes
\n 		- BCP_BusMonitor uses (OCI2C_InitHwSync)
\n
\n 		00.00.01
\n		05/05/2009
\n		Gabriele Merlonghi
\n		changes
\n 		- creation
\n		- request manager state machine implementation
\n    	- addiction of methods to set requests
*/

/* ******** */
/* Includes */
/* ******** */

#include <string.h>


#include "OCI2C_drv.h"
#include "BusCommPltf_cfg.h"
#include "BCP_BusCommMng.h"




/* ****************** */
/* Defines and Macros */
/* ****************** */

/** \addtogroup CRM
  @{
*/



/** @name Requests Management
  @{
*/
#define CP_REQ_COMPLETED_OK    0xFF    /**< requests array value for successfully completed requests */
#define CP_REQ_COMPLETED_KO    0xFE    /**< requests array value for unsuccessfully completed requests (reason: protocol error)*/
#define CP_REQ_TIMEOUT_KO      0xFD    /**< requests array value for unsuccessfully completed requests (reason: timeout expired)*/
#define CP_REQ_PENDING      0x01    /**< requests array value for pending request*/
/*@}*/

/** Request manager state machine states*/
enum eCRM_States
{
	COMM_PERIPHERAL_INIT=0,
	COMM_INIT,
	COMM_IDLE,
	COMM_PHYSICAL_LAYER_ACCESS,
	COMM_PHYSICAL_LAYER_MNG,
	COMM_REPETITION_MNG,
	COMM_REPETITION_TIMED
};
/*@} addtogroup CRM */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/**
 * Communication Request Manager Class
 */
#if BCP_MASTER_SUPPORT == TRUE
typedef struct BCM_Data_tag
{
	uint16_t  timeout;          		/* calcolo del timeout da impostare sul contatore per eseguire la ripetizione del pacchetto */
	uint8_t  PendingReqState;
	struct
	{
		uint8_t  MasterDisable  :1;
		uint8_t  ReqPending     :1;
		uint8_t  TxTimeout      :3;    /*  1: Timeouted request (busy)
		                                    2: Timeouted request (duration)
		                                    3: Repetitions
		                                */
		uint8_t  Rebuild        :1;     /* rebuild forced */
		uint8_t  DontValidate   :1;
		uint8_t                 :1;
	} flag;

	uint8_t  Wait;						/* Contatore di attesa per eventi Master/Slave */

	uint8_t  PacketReq[BCM_REQ_NUM];    /* il numero di richieste su un determinato canale dipende dall'enum delle richieste definito*/

	uint8_t  comm_repetitions; 			/* ripetizioni rimanenti */

	uint8_t  PacketReqCurrentIndex; 	/* indice richiesta corrente */
	IO_ChannelType  idBoard;          	/* current board id */
	BCP_PacketOptionType CurrentOption;	/* current option */
}CRM_ClassType;
#endif


/* ********* */
/* Variables */
/* ********* */

/* Exported */
#if BCP_MASTER_SUPPORT == TRUE
/*Master Request variable*/
CRM_ClassType CommMng[TOTAL_CHANNELS];
#endif

CM_Err_ClassType CommErr[TOTAL_CHANNELS][BCP_COUNTERS_NUM];

/* Static */





/* ******************* */
/* Functions Prototype */
/* ******************* */



/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
Initialization of the CP channel (protocol and CRM state machine).

If slave mode only call to this function can be omitted.

\param [in]   ch channel index (must be ch < BCP_CH_NUM)

\return   IO_ErrorType

\retval   IO_E_OK initialization successfully completed.
\retval   IO_E_INVALID_CHANNEL_ID wrong value for ch (maybe ch >= BCP_CH_NUM)

\par Example
\code
	BCP_InitChSync(BCP_I2C_CH_ID);
\endcode

\warning

\note

\version		1.0.0
\date 			30/dec/2009
\author			Roberto Fioravanti

*/
IO_ErrorType BCP_InitChSync(IO_ChannelType ch)
{
  if(ch>=(IO_ChannelType)BCP_CH_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

#if BCP_MASTER_SUPPORT == TRUE
    memset(&CommMng[ch], 0, sizeof(CommMng[ch]));
	CommMng[ch].PendingReqState = COMM_PERIPHERAL_INIT;
	CommMng[ch].PacketReqCurrentIndex=0xFF; /* Empty request */
	return PP_InitChSync(ch);
#else
  return (IO_ErrorType)IO_E_OK;
#endif
}

/**
Disables the Communication Request Manager for a specified channel.
This method can be used to inhibit requests .

\param [in] 	idChannel Index of the channel to be initialized.

\attention global variables used:
- CommMng

\return 	void

\warning	[Type warnings]

\author 	Roberto Fioravanti
\date			20/Apr/2009
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note 2:
Available only if BCP_MASTER_SUPPORT == TRUE

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
void BCM_MasterDisable(IO_ChannelType idChannel)
{
	CommMng[idChannel].flag.MasterDisable = TRUE;
}
#endif

/**
Force rebuild of master requests, if they have been started.
This method is used to avoid master/slave conflicts

\param [in] 	idChannel Index of the channel to request rebuild.

\attention global variables used:
- CommMng

\return 	void

\warning	[Type warnings]

\author 	Alessandro Sabbatini
\date			22/May/2014
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Alessandro Sabbatini
			\par \b Note 2:
Available only if BCP_MASTER_SUPPORT == TRUE

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
void BCM_ForceRebuild(IO_ChannelType idChannel)
{
	CRM_ClassType* pCommMng = &CommMng[idChannel];

	pCommMng->flag.Rebuild = TRUE;
	pCommMng->Wait = 5;
	if ( pCommMng->PendingReqState > COMM_PHYSICAL_LAYER_ACCESS)
	{
		pCommMng->PendingReqState = COMM_INIT;
	}
}

void BCM_SetWait(IO_ChannelType idChannel, uint8_t newWaitValue)
{
	CommMng[idChannel].Wait = newWaitValue;
}
#endif

/**
Allows to verify the state of the specified request id.
it changes the status of the request in order that all calls to this method,
before any call to BCM_SetChannelRequest method, return IO_E_NOT_QUEUED

\param [in]		idChannel CP channel
\param [in] 	idRequest Request index


\attention global variables used:
- CommMng (R/W)

\return 	IO_ErrorType

\retval     IO_E_OK
Request completed with success.
\retval     IO_E_ALREADY_PENDING
Request already pending
\retval     IO_E_NOT_QUEUED
Request not queued
\retval     IO_E_FAIL
Request not completed or completed with error.

\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		20/Apr/2009
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType BCM_TestRequest(IO_ChannelType idChannel, BCM_RequestType idRequest)
{
	IO_ErrorType ret;

	switch (CommMng[idChannel].PacketReq[idRequest])
	{
	case CP_REQ_PENDING:
	  return (IO_ErrorType)IO_E_ALREADY_PENDING;

	case CP_REQ_COMPLETED_OK:
	  /* request has been completed without error*/
	  ret = (IO_ErrorType)IO_E_OK;
	  break;

	case CP_REQ_COMPLETED_KO:
	  /* the request has been completed with error */
	  ret = (IO_ErrorType)IO_E_FAIL;
	  break;

	case CP_REQ_TIMEOUT_KO:
		/* the request has been completed with error */
		ret = (IO_ErrorType)IO_E_TIMEOUT_EXPIRED;
		break;

	default:
	  /* not queued request */
	  ret = (IO_ErrorType)IO_E_NOT_QUEUED;
	}

	CommMng[idChannel].PacketReq[idRequest] = FALSE;
	return ret;
}
#endif





/**
Check for valid request without changing its current status.

\param [in] 	idRequest Request index. The value for this parameter must satisfy the condition:
0 <= idRequest < BCM_REQ_NUM
\param [in] 	ch CP channel index. The value for this parameter must satisfy the condition:
0 <= ch < TOTAL_CHANNELS

\retval     IO_E_INVALID_CHANNEL_ID
out of range for input param (idRequest or ch).
One of the following condition are not satisfied.
0 <= idRequest < BCM_REQ_NUM
0 <= ch < TOTAL_CHANNELS

\attention global variables used:
- CommMng (R)

\return 	IO_ErrorType

\retval     IO_E_OK
Request successfully completed
\retval     IO_E_NOT_QUEUED
not queued or not valid request



\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		  26/Feb/2010
\version	0.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType CRM_IsPacketRequestSync(IO_ChannelType ch, BCM_RequestType idRequest)
{

  if(ch >= TOTAL_CHANNELS)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(idRequest >= BCM_REQ_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if (CommMng[ch].PacketReq[idRequest]) /* check for already pending requests */
  {
	  return (IO_ErrorType)IO_E_OK;
  }

  return (IO_ErrorType)IO_E_NOT_QUEUED;
}
#endif

/**
Inserts the specified request in the queue of the specified CP channel.

\param [in] 	idRequest Request index. The value for this param must satisfy the condition:
0 <= idRequest < BCM_REQ_NUM
\param [in] 	ch channel index. The value for this param must satisfy the condition:
0 <= ch < TOTAL_CHANNELS

\attention global variables used:
- CommMng (R/W)

\return 	IO_ErrorType

\retval     IO_E_OK
Request successfully completed
\retval     IO_E_ALREADY_PENDING
Already pending request

\retval     IO_E_INVALID_CHANNEL_ID
out of range for input param (idRequest or ch).
One of the following condition are not satisfied.
0 <= idRequest < BCM_REQ_NUM
0 <= ch < TOTAL_CHANNELS


\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		  26/Feb/2010
\version	0.0.0

\bug		[Type bugs]

\todo		[To do action]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType CRM_SetPacketRequestSync(IO_ChannelType ch, BCM_RequestType idRequest)
{

/* check for already pending requests */

  if(ch >= TOTAL_CHANNELS)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if(idRequest >= BCM_REQ_NUM)
  {
    return (IO_ErrorType)IO_E_INVALID_CHANNEL_ID;
  }

  if (CommMng[ch].PacketReq[idRequest]) /* check for already pending requests */
  {
	  return (IO_ErrorType)IO_E_ALREADY_PENDING;
  }

  CommMng[ch].PacketReq[idRequest] = CP_REQ_PENDING;

  CommMng[ch].flag.ReqPending = TRUE;

  return (IO_ErrorType)IO_E_OK;
}
#endif




/**
Sets the parameters for the specified request.
This parameters passed by this function will be processed by the Platform Protocol to build the the frame.

\param [in] 	idRequest Request index.
\param [in] 	pParam pointer to the paramenters
\param [in] 	len size in byte of the paramenters.

\attention global variables used:
- CommMng

\return 	IO_ErrorType

\retval     IO_E_OK
Request successfully completed
\retval     IO_E_NOT_QUEUED
Already pending request


\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		26/Feb/2010
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
IO_ErrorType CRM_SetPacketParamSync(IO_ChannelType ch, BCM_RequestType idRequest, void * pParam, IO_LenType len)
{

  IO_ChannelType idProtReq = BCM_MasterRequestTable[idRequest].PPReqIndex; /* get index in the protocol table */

  return PP_MasterSetParamRequestSync(ch, idProtReq, pParam, len);
}
#endif

/**
Removes the specified request from the queue.
\param [in] 	idChannel CP Channel.
\param [in] 	idRequest Request index.

\attention global variables used:
- CommMng (R/W)

\return 	void

\warning	[Type warnings]

\author 	Gabriele Merlonghi, Roberto Fioravanti
\date		20/Mar/2010
\version	00.00.35

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
void BCM_KillPacketRequest(IO_ChannelType idChannel, BCM_RequestType idRequest)
{

  CommMng[idChannel].PacketReq[idRequest] = FALSE;
  CommMng[idChannel].PacketReqCurrentIndex = 0xFF;
}
#endif


/**
Executes the Communication Request Manager state machine of the specified channel.

\param [in] 	idChannel Index of the channel.


\attention global variables used:
- CommMng (R/W)

\return 	void

\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		16/Apr/2010
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\public \memberof CRM_ClassType
*/
#if BCP_MASTER_SUPPORT == TRUE
void BCM_RequestManager(uint8_t idChannel)
{
	CRM_ClassType* pCommMng = &CommMng[idChannel];

	switch(pCommMng->PendingReqState)
	{
	case COMM_PERIPHERAL_INIT:
	{
		IO_PhyInitSyncType drvParam={0};

		drvParam.eventCB=PP_SlaveNotification;
		drvParam.id=idChannel;

		pCommMng->flag.TxTimeout=0;
		pCommMng->PacketReqCurrentIndex = 0xFF;


		(void)PP_InitChSync(idChannel); 				/* TODO: check ret val */

		/* physical channel initialization */
		(void)(IO_PhyInitSync)(BCP_ChannelTable[idChannel].InitSync)(&drvParam);
	}
    /* -fallthrough */
    case COMM_INIT:
		DISABLE_TIMER;									/* disable timer */
		pCommMng->flag.TxTimeout=0;
		pCommMng->PendingReqState = COMM_IDLE;

    /* -fallthrough */
    case COMM_IDLE:

		if ((pCommMng->flag.ReqPending == FALSE) ||
			(pCommMng->flag.MasterDisable))
		{
			break;
			/* exit because:
			 * - no requests
			 * - master disabled
			 */
		}
      /*  SET_TIMER_VAL(COMM_TIMEOUT); */

	/* -fallthrough */
    case COMM_PHYSICAL_LAYER_ACCESS:
		if ((bool_t)IS_TIMER_EXPIRED)
		{
			/* communication timeout has expired:
			 * then unlock the peripheral
			 */
			if(pCommMng->flag.TxTimeout == 2)
			{
				pCommMng->PendingReqState = COMM_PERIPHERAL_INIT;
				break;
			}
		}

		if (pCommMng->Wait)
		{
			pCommMng->Wait--;
			break;
			/* exit because:
			 * - requests pending
			 * - slave is carrying on
			 */
		}

		/*communication scheduler (lower array index means higher priority)*/
		{
			uint8_t i, noReq = TRUE;
			IO_ChannelType indexReq;
			IO_ChannelType idProtReq;

			CommDriverParamType param={0};

			for (i=0; i<sizeof(pCommMng->PacketReq); i++)
			{
				indexReq = pCommMng->PacketReq[i];
				if ((indexReq > 0) && (indexReq < 0xFE))
				{
					noReq = FALSE;
					break;  /* exit for-cycle */
				}
			}

			if (noReq == TRUE)
			{
				pCommMng->flag.ReqPending = FALSE;
				pCommMng->PendingReqState= COMM_IDLE;
				pCommMng->PacketReqCurrentIndex = 0xFF;
				pCommMng->CurrentOption = 0;
				break;   /* exit case COMM_PHYSICAL_LAYER_ACCESS */
			}
			/* prepare request*/


			indexReq = i;

			/* update the current idBoard */
			pCommMng->idBoard = BCM_MasterRequestTable[indexReq].idBoard;
			/* id Protocol Request */
			idProtReq = BCM_MasterRequestTable[indexReq].PPReqIndex;

#ifdef CP_PRE_BUILD_REQUEST_CB
			if(CP_PRE_BUILD_REQUEST_CB(idChannel, (PP_MasterProtocolTableType *) &PP_MasterProtocolTable[idProtReq]) == TRUE)
			{
				pCommMng->flag.DontValidate=1;
				pCommMng->PacketReqCurrentIndex=indexReq;			
			}
			else
#endif
			{
			    pCommMng->flag.DontValidate=0;
				/* Prepare request parameters */
				(void)PP_MasterPrepareRequest(idProtReq, idChannel, &param);

				if( (pCommMng->PacketReqCurrentIndex == 0xFF) ||   			/* current request empty */
					(pCommMng->PacketReqCurrentIndex != indexReq)	||		/* current request changed */
					(pCommMng->flag.Rebuild)
				)
				{
					/* build is request */

					pCommMng->PacketReqCurrentIndex=indexReq;

					pCommMng->CurrentOption = (&PP_MasterProtocolTable[idProtReq])->option;
					pCommMng->flag.TxTimeout=0;
					pCommMng->flag.Rebuild=0;
					DISABLE_TIMER;


					(void)PP_PrepareRequest(param.buffer, &PP_MasterProtocolTable[idProtReq]);
				}
				else 																/* build is not request */
				if (pCommMng->flag.TxTimeout == 3)
				{
					if(IS_TIMER_NOT_EXPIRED)		/* Timer is not expired */
					{
						break;
					}
					else
					{
						pCommMng->flag.TxTimeout = 0;
					}
				}

				/* calculate length */
				param.len = PP_GetRequestLen(param.buffer, (PP_MasterProtocolTableType *) &PP_MasterProtocolTable[idProtReq]);

				/* address resolution*/
				param.slaveAddress =  PP_GetSlaveAddress(pCommMng->idBoard, (PP_MasterProtocolTableType *) &PP_MasterProtocolTable[idProtReq]);

				/* Start driver request*/
				if ( (PP_StartDriverRequestSync(idChannel, &param) != (IO_ErrorType)IO_E_WORKING) || (pCommMng->flag.Rebuild))
				{
					if( pCommMng->CurrentOption & PRT_OPTION_TIMEOUTED )
        			{
        				if( pCommMng->flag.TxTimeout != 1 )
        				{

        					SET_TIMER_VAL(CP_MASTER_ATTEMPTS_TIMEOUT);
        					/* timeout started */
        					pCommMng->flag.TxTimeout = 1;
        				}

        				if (IS_TIMER_EXPIRED)
        				{
        					pCommMng->PendingReqState = COMM_PERIPHERAL_INIT;
        					pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_TIMEOUT_KO;
        					pCommMng->PacketReqCurrentIndex = 0xFF;
        				} else
        				{
        					pCommMng->PendingReqState = COMM_PHYSICAL_LAYER_ACCESS;
        				}
        			}
										
					break;  /* bus busy, exit retry next loop */
				}
				else
				{
					BCM_CountersUpdater(idChannel, BCP_PACKET_NUM_ALL_CNT);
				}

				/* request successfully started */


				if(!pCommMng->flag.TxTimeout)
				{
					/* Start timeout if not already running */
					SET_TIMER_VAL(CP_MASTER_TIMEOUT_REQUEST);
					pCommMng->flag.TxTimeout=2;
				}
			}
			/*next loop wait for result */

			if (pCommMng->flag.Rebuild == 0)
			{
				pCommMng->PendingReqState=COMM_PHYSICAL_LAYER_MNG;
			}
		}
	/* -fallthrough */
	case COMM_PHYSICAL_LAYER_MNG:		
		
		
		switch (PP_GetEsit(idChannel))
		{
			/* Protocol Errors */
		case IO_E_ACK_PP:
		    PP_MasterValidateAnswer(idChannel);
			pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_COMPLETED_KO;
			pCommMng->PendingReqState = COMM_IDLE;
			pCommMng->PacketReqCurrentIndex=0xFF;
    		pCommMng->flag.TxTimeout = 0;
			BCM_OnRequestCompleted(idChannel);
    	break;
	
		case IO_E_OK:
		{	
		    IO_ErrorType err = IO_E_OK;
		    if(pCommMng->flag.DontValidate==1)
		    {
		        err = IO_E_OK;
		    } else
		    {
		        err = PP_MasterValidateAnswer(idChannel);
		    }
		    
		    if((IO_E_OK == err ) || (IO_E_ACK_PP == err ) )
 		    {
 		    
 		        if(IO_E_OK == err )
 		        {
 		            pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_COMPLETED_OK;    
 		        } else if(IO_E_ACK_PP == err )
 		        {
 		            pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_COMPLETED_KO;
 		        }
  		        
    			pCommMng->PacketReqCurrentIndex=0xFF;
    			pCommMng->flag.TxTimeout = 0;
    			pCommMng->PendingReqState = COMM_IDLE;
    			
    			BCM_OnRequestCompleted(idChannel);
    			
    			DISABLE_TIMER;
    			break;
		    }
		    /* else IO_E_CHECK_PP is returned, then */
		}
        case IO_E_CHECK_PP:
            pCommMng->flag.Rebuild=1;
            BCM_OnRequestCompleted(idChannel);
		/* Peripheral Errors */
		case IO_E_OV_AL:
		case IO_E_ACK_BUS:

			if( !(pCommMng->CurrentOption & PRT_OPTION_TIMEOUTED) )
			{
				pCommMng->PendingReqState = COMM_REPETITION_MNG;
				break;
			}
		/* Peripheral Busy */
		case IO_E_BUSY:
			if( pCommMng->CurrentOption & PRT_OPTION_TIMEOUTED )
			{
				if( pCommMng->flag.TxTimeout != 1 )
				{

					SET_TIMER_VAL(CP_MASTER_ATTEMPTS_TIMEOUT);
					/* timeout started */
					pCommMng->flag.TxTimeout = 1;
				}

				if (IS_TIMER_EXPIRED)
				{
					pCommMng->PendingReqState = COMM_PERIPHERAL_INIT;
					pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_TIMEOUT_KO;
					pCommMng->PacketReqCurrentIndex = 0xFF;
				} else
				{
					pCommMng->PendingReqState = COMM_PHYSICAL_LAYER_ACCESS;
				}
			}
			else
			{
				pCommMng->PendingReqState=COMM_IDLE;
			}
		break;
		/* working good */
		case IO_E_WORKING:
		default:
		  /* test if comm timeout has expired */

			if (IS_TIMER_EXPIRED)
			{
				/*comm timeout has expired: reason could be the slave or anomalous behavior of the bus
				* unlock the peripheral */
				pCommMng->PendingReqState = COMM_PERIPHERAL_INIT;

				if( pCommMng->CurrentOption & PRT_OPTION_TIMEOUTED )
				{
					pCommMng->PacketReq[pCommMng->PacketReqCurrentIndex] = CP_REQ_TIMEOUT_KO;
				}

				pCommMng->PacketReqCurrentIndex = 0xFF;

			}
			break;
		}
		break;

	case COMM_REPETITION_MNG:
		++pCommMng->comm_repetitions;
		pCommMng->comm_repetitions %= CP_MASTER_NUM_REPETITIONS_PER_ATTEMPS;
		if (!pCommMng->comm_repetitions)
		{
			pCommMng->timeout = minValue(CP_MASTER_MAX_DELAY_ATTEMPTS,(pCommMng->timeout+CP_MASTER_STEP_DELAY_ATTEMPS));
			pCommMng->flag.TxTimeout = 3;
			SET_TIMER_VAL(pCommMng->timeout);
		}
		else
		{
			DISABLE_TIMER;
			pCommMng->flag.TxTimeout=0;
		}
		pCommMng->PendingReqState=COMM_IDLE;
		break;
  }
}
#endif








/* Local Functions */


/**
Increment of communication error counters

\param [in] 	idChannel Physical channel communication identifier (ie: i2c, uart, spi, etc..) see CHANNEL_ID enumeration.
\param [in] 	idError Error code id

\attention global variables used:
- CommErr (R/W)

\return 	void

\warning	[Type warnings]

\author 	Gabriele Merlonghi
\date		20/Apr/2009
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:

\private \memberof CM_Err_ClassType
*/

void BCM_CountersUpdater(IO_ChannelType idChannel, IO_ErrorType idError)
{
	if (CommErr[idChannel][idError] < 0xFFFF)
	{
		++CommErr[idChannel][idError];
	}
}




/**
Communication monitor detecting the following condition:
\li I2C:
- Block 1: this condition occurs whan at least one line is low indefinitely.
- Block 2: when busy bit high indefinitely although bus is free.
- Block 3: wrong peripheral configuration: ACK enable bit = 0 and Slave-TX o Master-TX.
- Block 4: wrong peripheral configuration in slave mode: 	ACK enable bit = 1 and Slave-RX.
- Block 5: wrong peripheral configuration in slave mode: when bus is free (busy = 0), only slave Rx accepted
- Block 6: timeout for no request to slave

\return 	void

\warning	[Type warnings]

\author 	Roberto Fioravanti
\date		20/Apr/2009
\version	1.0.0

\bug		[Type bugs]

\note		\par \b Note 1:
			documentation by: Roberto Fioravanti
			\par \b Note n:
*/
void BCP_BusMonitor(void)
{
    if(OCI2C_Level1BlockMonitorSync())
    {
        BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, (IO_ErrorType)BCP_I2C_UNBLOCK_LEV1_CNT);
    }

    if(TRUE == OCI2C_Level2BlockMonitorSync())
    {
        BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, (IO_ErrorType)BCP_I2C_UNBLOCK_LEV2_CNT);
#if defined(CP_LEV2_I2CDRIVER_ACTION_ENABLE)
        OCI2C_Action_Lev2();
#endif /* defined(CP_LEV2_I2CDRIVER_ACTION_ENABLE) */
    }

	/* Blocco 3
	slave configuration wrong: cannot be transimmted ACK bit  = 0
	during Slave-TX o Master-TX
	*/
    if(OCI2C_Level3BlockMonitorSync())
	{
		BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, (IO_ErrorType)BCP_I2C_UNBLOCK_LEV3_CNT);
		OCI2C_InitHwSync();	  
	}
	/* Blocco 4
	slave configuration wrong: cannot be transimmted ACK bit = 1
	during Slave-RX
	*/
	if(OCI2C_Level4BlockMonitorSync())
	{
		BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, (IO_ErrorType)BCP_I2C_UNBLOCK_LEV4_CNT);
		OCI2C_InitHwSync();
	}
	/* slave configuration wrong: when bus is freee (busy = 0),
	periphral must be  Slave-RX
	(any other configuration Slave-TX, Master-TX o Master-RX is not allowed)
	*/
	if(OCI2C_Level5BlockMonitorSync())
	{
		BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, (IO_ErrorType)BCP_I2C_UNBLOCK_LEV5_CNT);
		OCI2C_InitHwSync();
	}

	/* Blocco 6
	no request for more than 5 sec: */
#if BCP_SLAVE_SUPPORT	== TRUE
  if(OCI2C_Level6BlockMonitorSync())
	{
#if defined(BCP_COUNTER_OPTION__LEV_6)
	  BCM_CountersUpdater((IO_ChannelType)BCP_I2C_CH_ID, BCP_I2C_UNBLOCK_LEV6_CNT);
#endif	  /* defined(BCP_COUNTER_OPTION__LEV_6) */
	}
#endif /* BCP_SLAVE_SUPPORT	== TRUE */
}



/* FF: meaningless 
   other values: position */
const CP_InProtParamPType ProtParam[2]={
/*                  hdr typ len dat stx     ee  */
	{0xFF,	0xFF,	2,	3,	4,	5,	0,		1},		/*std format*/
	{1,		2, 		3,	4,	5,	6,	0xFF,	0xFF}	/*GP format*/
};

                       
void CP_SlaveManager__Read_ApplData(uint8_t pos, IO_MemPtrType pRxBuff, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen)
{
  pinfo->error = (IO_ErrorType)IO_E_OK;
  pinfo->len = pRxBuff[pos];
  if((pinfo->len == 0) || (pinfo->len >= maxlen))
  {
    pinfo->len=maxlen;
  }
  pinfo->address = pData;
  pinfo->option|=PP_SL_RESP_ANSW_OPT;
}
                             
void CP_SlaveManager__Write_ApplData(uint8_t pos, IO_MemPtrType pRxBuff, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen)
{
    if((pRxBuff[pos]!=0) && (pRxBuff[pos] <= maxlen))
    {
      pinfo->error = (IO_ErrorType)IO_E_OK;
      (void)memcpy((void*)pData, &pRxBuff[pos+1], pRxBuff[pos]);
    }
}


void BCP_SlaveManager__Read_ApplData(IO_ChannelType ch, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen)
{
  pinfo->error = (IO_ErrorType)IO_E_OK;
  pinfo->len = BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX];
  if((BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX] == 0) ||
  (BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX] >= maxlen))
  {
    pinfo->len=maxlen;
  }
  pinfo->address=pData;
  pinfo->option|=PP_SL_RESP_ANSW_OPT;
}

void BCP_SlaveManager__Write_ApplData(IO_ChannelType ch, IO_MemPtrType pData, PP_SlaveBuilderInfoType* pinfo, BCP_PacketSizeType maxlen)
{
    if((BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX]!=0) &&
    (BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX] <= maxlen))
    {
      pinfo->error = (IO_ErrorType)IO_E_OK;
      (void)memcpy((void*)pData, &BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX], BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX]);
    }
}



int8_t PP_MappingGetOffset(IO_MemPtrType pStart, IO_MemPtrType pEnd, const PP_RemapTableType* pTable, int8_t size, unsigned long* pOffset)
{

    while(size-->0)
    {
    	if(((void*)pStart >= (void*)((unsigned int)(pTable[size].start))) && ((void*)pEnd <= (void*)((unsigned int)(pTable[size].end))))
        {
            *pOffset = pTable[size].offset;
            break;
        }
    }   
    return size;
}


/*
\param pTable pointer to memory range type
\param tablelength number og items in pTable, must be at least 1
\param pStart start address
\param size size in bytes , must be at least 1

\return int_8_t
\retval	>=0 access is allowed
\retval	< access is denied

*/
int8_t PP_IsAccessAllowed(const PP_MemoryRangeType* pTable, int8_t tablelength, IO_MemPtrType pStart, IO_LenType size)
{
	void* pend;
	pend=pStart+size-1;
    while(tablelength-- > 0)
    {
    	if(	((void*)pStart >= (void*)((unsigned int)(pTable[tablelength].start))) &&
    			( (pend) <= (void*)((unsigned int)(pTable[tablelength].end))) &&
    			!((pend) < (void*)(pStart)))
        {
            break;
        }
    }
    return tablelength;
}

uint16_t PP_MappingGetPPAddr(const IO_MemPtrType realAddr, const PP_RemapTableType* pTable, int8_t size)
{
	uint16_t retValue = 0xffff;

    while(size-->0)
    {
        if((realAddr >= ((IO_MemPtrType) pTable[size].offset + pTable[size].start))
        		&& (realAddr <= ( (IO_MemPtrType) pTable[size].offset + pTable[size].end)) )
        {
        	retValue = (uint16_t) (realAddr - (IO_MemPtrType) pTable[size].offset);
            break;
        }
    }
    return retValue;
}


/* *********** */
/* End of file */
/* *********** */

