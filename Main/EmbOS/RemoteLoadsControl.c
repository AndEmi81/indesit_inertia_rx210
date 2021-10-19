/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	RemoteloadsControl  RemoteLoads Control Library
@{
\par            Description
\n              This module contains the implementation of the Remote Loads control, also known as
'PC Control'. Remote control is the control by an external tool that communicates with the application.
                Each command is a buffer which contains data for only one load.

                Two types of command are availale: normal and extended.

\par            Configuration
\li             PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE is the size of the queue in number of bytes for each load.

\par            Use

                After the initializazion of the module by calling Main_RemoteLoadsControl_Init, a command
                can stored in the queue by Main_RemoteLoadsControl_PutCommand.
                Main_RemoteLoadsControl_GetCommand retrieves the command, if exists,that must be processed by
                Main_RemoteLoadsControl_ProcessCommand.
                Once processed, the command must be deleted by calling Main_RemoteLoadsControl_PurgeCommand.
@}
\ingroup        MainLibrary
*/
/**
\n
\par		    Description
\n			    This module uses EmbOS queues that work as FIFO, because it allows commands of various sizes.
                EmbOS queues enhance performance as they save memory and time: retrieving a message from the queue does not copy the content,
                but returns a pointer allowing to process the message by accessing the content throught the pointer.
\n
\file		    RemoteLoadsControl.c
\ingroup	    MainLibrary
\date		    Feb 2011
\version	    00.00.00
\author		    Roberto Fioravanti
\n
\n
\par\b	History:
\n
\n 		        00.00.00
\n          	Feb 2011
\n          	Roberto Fioravanti
\n            - Creation
\n
*/
/*lint -idlen(60) */

/* ******** */
/* Includes */
/* ******** */

#include "Main.h"
#include "OutputManager.h"
#include "OutputManagerExp.h"
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
#include "LoadsRequestExp.h"
#endif
/* ****************** */
/* Defines and Macros */
/* ****************** */

#define RLC_HDR_SIZE()      (sizeof(GIOM_RemoteLoadsControlExtendedType)-sizeof(GIOM_RemoteLoadsControlBufferType))


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */



/**

*/
typedef  enum
{
	REMOTELOADSCONTROL_STATE_INVALID = 0,
	REMOTELOADSCONTROL_STATE_RUNNING
} Main_RemoteLoadsControlStateType;

/**

*/
typedef struct
{
	Main_RemoteLoadsControlStateType  state;
	Main_RemoteLoadsControlErrorType  error;
	OS_Q Q;
	char QBuffer[PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE * 1 /*\todo make property*/];
} Main_RemoteLoadsControlObjType;

typedef struct
{
	uint8_t data [PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE];
} GIOM_RemoteLoadsControlBufferType;
/**

*/
typedef struct
{
	RemoteLoadsControlType type;
	uint8_t	len;
	GIOM_RemoteLoadsControlBufferType buffer;
} GIOM_RemoteLoadsControlExtendedType;



/* ********* */
/* Variables */
/* ********* */

/* Static */
/**

*/
static Main_RemoteLoadsControlObjType Main_RemoteLoadsControl; /* Instance of the object */

/* Global */

/* ******************** */
/* Functions Prototypes */
/* ******************** */

bool_t Main_RemoteLoadsControl_IsInitialized ( void );

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Initializes EmbOs objects and global variables of the module.
*/
void Main_RemoteLoadsControl_Init ( void )
{
	OS_Q_Create ( & ( Main_RemoteLoadsControl.Q ),
				  & ( Main_RemoteLoadsControl.QBuffer ),
				  sizeof ( Main_RemoteLoadsControl.QBuffer ) );
	Main_RemoteLoadsControl.state = REMOTELOADSCONTROL_STATE_RUNNING;
	Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_NO_ERROR;
}

/**
Stores a command in the queue
\param [in] type type of command (see RemoteLoadsControlType)
\param [in] len lenght in bytes of the command
\param [in] pData pointer to command

\return bool_t

\retval REMOTELOADSCONTROL_NO_ERROR no error
\retval REMOTELOADSCONTROL_UNKNOWN_OBJ uninitilized module. Call Main_RemoteLoadsControl_Init.
\retval REMOTELOADSCONTROL_FULLQUEUE_ON_PUTCOMMAND queue is full, not enough space. Purge command or
define PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE with a greater value.
\retval REMOTELOADSCONTROL_INVALID_DATA_ON_PUTCOMMAND   invalid pData (null) or len is greater than PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE .

The returned value is available on Main_RemoteLoadsControl.error
*/
Main_RemoteLoadsControlErrorType Main_RemoteLoadsControl_PutCommand ( RemoteLoadsControlType type, uint8_t len, void* pData )
{
	Main_RemoteLoadsControlErrorType result = REMOTELOADSCONTROL_UNKNOWN_OBJ;
	GIOM_RemoteLoadsControlExtendedType param;
	
	if ( !Main_RemoteLoadsControl_IsInitialized() )
	{
		result = Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_UNKNOWN_OBJ;
	}
	else
	{
		OS_MEMSET(&param, 0, sizeof(param));
		param.type = type;
		
		if ( len <= sizeof ( GIOM_RemoteLoadsControlBufferType ) )
		{
			param.len = len;
			
			if ( pData )
			{
			    bool_t res;
				memcpy ( param.buffer.data, pData, ( size_t ) len );
				
				res = ( bool_t ) OS_Q_Put ( &Main_RemoteLoadsControl.Q, &param, len + RLC_HDR_SIZE() );
				
				if ( res )
				{
					result = Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_FULLQUEUE_ON_PUTCOMMAND;
				}
				else
				{
					result = Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_NO_ERROR;
				}
			}
			else
			{
				result = Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_INVALID_DATA_ON_PUTCOMMAND;
			}
		}
		else
		{
			result = Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_INVALID_DATA_ON_PUTCOMMAND;
		}
	}
	
	return result;
}

/**
Retrieves a command from the queue
\param [out] ppData pointer to pointer to command
\param [out] pLen lenght in bytes of the command

\attention
Global variables:
Main_RemoteLoadsControl.error assumes the following values
\li REMOTELOADSCONTROL_NO_ERROR no error
\li REMOTELOADSCONTROL_UNKNOWN_OBJ uninitilized module. Call Main_RemoteLoadsControl_Init.
\li REMOTELOADSCONTROL_FULLQUEUE_ON_PUTCOMMAND queue is full, not enough space. Purge command or
define PP_REMOTE_LOADS_CONTROL_BUFFER_SIZE with a greater value.
\li REMOTELOADSCONTROL_INVALID_DATA_POINTER_ON_GETCOMMAND   invalid pData or len (null).
*/
void Main_RemoteLoadsControl_GetCommand ( void** ppData, int* pLen )
{
	if ( !Main_RemoteLoadsControl_IsInitialized() )
	{
		Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_UNKNOWN_OBJ;
	}
	else
	{
		if ( pLen && ppData )
		{
			Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_NO_ERROR;
			*pLen = OS_Q_GetPtrCond ( &Main_RemoteLoadsControl.Q, ppData );
		}
		else
		{
			Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_INVALID_DATA_POINTER_ON_GETCOMMAND;
		}
	}
}



/**
Processes the command.
If the command is valid it calls LIB_IOM_SetAllLoadsOff() or LIB_IOM_GIOM_SetLoad()

\param [in] pData pointer to the command

\attention
Global variables:
Main_RemoteLoadsControl.error assumes the following values:
\li REMOTELOADSCONTROL_NO_ERROR no error
\li REMOTELOADSCONTROL_UNKNOWN_COMMAND command is invalid
*/
void Main_RemoteLoadsControl_ProcessCommand ( void* pData )
{
	uint8_t* pFirstData;
	GIOM_LoadStruct load;
	uint8_t len;
	GIOM_RemoteLoadsControlExtendedType* pRemoteLoadsData = ( GIOM_RemoteLoadsControlExtendedType* ) pData;
	
	switch ( ( ( GIOM_RemoteLoadsControlExtendedType* ) pData )->type )
	{
	case REMOTE_LOADS_CONTROL_EXTENDED:
	case REMOTE_LOADS_CONTROL_BASIC:
		pFirstData = pRemoteLoadsData->buffer.data;
		len = pRemoteLoadsData->len;
		break;
	default:
		Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_UNKNOWN_COMMAND;
		return;
	}
	
	if ( *pFirstData == 0xFF )
	{
#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
		LIB_IOM_ResetAllLoadRequest();
#else
		LIB_IOM_SetAllLoadsOff ( SOURCE_SALO_PCCONTROL );
#endif
	}
	else
	{
		load.loadID = *pFirstData; /* ID Load / Position */
		load.loadParam.b = * ( pFirstData + 1 );
		memcpy ( load.data, ( pFirstData + 1 ), ( size_t ) ( len - 1 ) );

#if defined(CONFIG_LIB_LOADSREQUEST_SUPPORT)
		LIB_IOM_ProcessSingleLoadRequest( &load );
#else
		( void ) LIB_IOM_GIOM_SetLoad ( &load );
#endif

	}
	
	Main_RemoteLoadsControl.error = REMOTELOADSCONTROL_NO_ERROR;
}


/**
Deletes the last retrieved command.
*/
void Main_RemoteLoadsControl_PurgeCommand ( void )
{
	OS_Q_Purge ( &Main_RemoteLoadsControl.Q );
}



/* Local Function */

/**
Returns if module has beel initialized.
*/
bool_t Main_RemoteLoadsControl_IsInitialized ( void )
{
	return ( bool_t ) ( Main_RemoteLoadsControl.state == REMOTELOADSCONTROL_STATE_RUNNING );
}


/* *********** */
/* End of file */
/* *********** */
