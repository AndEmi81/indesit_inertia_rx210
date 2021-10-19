/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\par		Description
\n		Data and definitions for Bus Communication Protocol.
\n
\file		BCP_General.h
\ingroup	BCP
\date		14/05/2009
\version	01.00.00
\author	Roberto Fioravanti
\n
\n
\par		History (titolo)
\n 		00.00.75
\n		28/10/2010
\n		Roberto Fioravanti
\n		changes
\n		- updated documentation
\n

\n    00.00.49
\n		xx/06/2010
\n		developer Roberto Fioravanti
\n		changes
\n 		- CP_InProtParamPType added
\n
\n    00.00.30
\n		27/01/2010
\n		developer Roberto Fioravanti
\n		changes
\n 		- labels changed:
\n						ePP_ZB_FIELD_POSITION -> ePP_GP_FIELD_POSITION,
\n						ePP_ZB_FIELD_ANS_POSITION -> ePP_GP_FIELD_ANS_POSITION
\n						PP_ZB_OUT_BUFF_RESERVED_SZ -> PP_GP_OUT_BUFF_RESERVED_SZ
\n						PPM_OPTION_BRIDGE_ZB -> PPM_OPTION_GP_REQUEST
\n		- bug fix: BEGIN_BCP_SLAVEMANAGER and END_BCP_SLAVEMANAGER
\n		didn't update info.error
\n
\n    00.00.30
\n		27/01/2010
\n		developer Roberto Fioravanti
\n		changes
\n 		- Doc updated
\n		- bug fix: BEGIN_BCP_SLAVEMANAGER and END_BCP_SLAVEMANAGER
\n		didn't updade info.error
\n
\n    00.00.27
\n		27/01/2010
\n		developer Roberto Fioravanti
\n		changes
\n 		- added ACK field into ZB bridge answer
\n
\n 		00.00.14
\n		23/10/2009
\n		developer Roberto Fioravanti
\n		changes
\n 		- define PP_OUT_BUFF_RESERVED_SZ and PP_ZB_OUT_BUFF_RESERVED_SZ addiction
\n
\n 		00.00.09
\n		15/09/2009
\n		developer Roberto Fioravanti
\n		changes
\n 		- fields DTYP, HDR, LEN (PP_HDR_SZ, PP_DTYPE_SZ, PP_LEN_SZ) size parametrized.
\n    - request and answer ZB bridge field positions (ePP_ZB_FIELD_POSITION e ePP_ZB_FIELD_ANS_POSITION) parametrized
\n    - BCP_ZB_ParamType Updated ('manufacturer' H and L removed)
\n    - ZB_ADDRESS  Updated to 0x96
\n
\n 		00.00.01
\n		14/05/2009
\n		developer Roberto Fioravanti, Gabriele Merlonghi
\n		changes
\n 		- creation
\n
*/


/**
  \page BCP_ConfigurationIntro How to use CP: The configuration

  The goal of this section is to explain the use of Communication Platform in a generic application.
  Comunication Platform provides a framework for communication with Indesit Company's applicances. It needs to be configured.
  This allows to cover a wide range of needs that can be related to architectural aspects of the application or....

  The configuration is based on two files \e BusCommPltf_cfg.c and \e BusCommPltf_cfg.h, provided by the application.

  File \e BusCommPltf_cfg.c contains:
  \li constants and variables necessary to define communication drivers to be managed;
  \li functions and callback define application-dependant behavior and allow the Communication Platform to adapt to many situations;
  \li configurable structure (like SMA to define supported packets)

  File \e BusCommPltf_cfg.h contains:
  \li Communication Platform options
  \li application specific include and objects (defines, methods, etc)

  \par Configurations
  \n If your application behaves as \b master in the communication, see \subpage bcpmastercfg
  \n If your application behaves as \b slave in the communication, see \subpage bcpslavecfg



  \page bcpmastercfg BCP master configuration
  These are the steps:
  \li Including files: see \subpage bcpmasterinclude
  \li Driver configuration: see \subpage bcpmasterdrivercfg
  \li Network configuration: see \subpage bcpmasternetworkcfg
  \li Protocol configuration: see \subpage bcpmasterprotocolcfg
  \li Communication Request Manager configuration \subpage bcpcrmcfg
  \li General options for Communication Manager: see \subpage bcp_options

*/


/**
  \page bcpmasterdrivercfg Driver configuration
    \section bcpcrmcfg_driverbuffer Buffers for drivers
    For each driver define the rx and tx buffers.

    In \e BusCommPltf_cfg.c
    \code
    //I2C driver buffer
    static uint8_t BCP_I2CTxBuffer[BCP_I2C_TX_BUF_SIZE];
    //I2C driver buffer
    static uint8_t BCP_I2CRxBuffer[BCP_I2C_RX_BUF_SIZE];
    \endcode

    \subsection bcpcrmcfg_driverbuffershared Shared buffer
    In order to save ram, you can share buffer between rx and tx by writing:
    \code
    #define BCP_I2CRxBuffer BCP_I2CTxBuffer

    //I2C driver buffer
    static uint8_t BCP_I2CTxBuffer[BCP_I2C_TX_BUF_SIZE];
    \endcode

    \section bcpcrmcfg_tableofchannels Define the table of drivers
    This row of the table contains information about a channel. These are:
    \li rx and tx buffer and their size
    \li pointer to functions of initialization, reception, and trasmission

    Enumerate the channels in \e BusCommPltf_cfg.h
    \code
    enum eChannelId
    {
      BCP_I2C_CH_ID,
      BCP_CH_NUM
    };
    \endcode

    BCP_CH_NUM is the total number of channels.

    In \e BusCommPltf_cfg.c
    \code
    const BCP_ChannelTableType BCP_ChannelTable[BCP_CH_NUM]=
    {
      {PP_MasterProtocolTable, OCI2C_InitSync, OCI2C_TxSync, OCI2C_RxSync, BCP_I2CTxBuffer, BCP_I2CRxBuffer, BCP_I2C_TX_BUF_SIZE, BCP_I2C_RX_BUF_SIZE},
    };
    \endcode

    If your application uses more the one driver the order in \e eChannelId and \e BCP_ChannelTable must match.


    \section bcpcrmcfg_interrupt Interrupt drivers

    Call the interrrupt method for driver in \e BusCommPltf_cfg.c . For example for Osek-based projects:
    \code
    ISR(OCI2C_Isr)
    {
      OCI2C_ISR();
    }
    \endcode
    or in general
    \code
    __interrupt void OCI2C_Isr(void)
    {
      OCI2C_ISR();
    }
    \endcode


*/

/**
  \page bcpmasternetworkcfg Network configuration
    This level allows to know the \e right driver to use to communicate with a \e particular device.

    Enumerate the device in \e BusCommPltf_cfg.h
    \code
    enum eBoardId
    {
        MB_ID=0,              // Main Board
        UI_ID,                // User Interface Board
        WS_ID,                // Weight Sensor Expansion Board
        ZB_ID,                // ZigBee
        BOARD_NUM             // Number of devices
    };
    \endcode

    BOARD_NUM is the total number of devices.


    Define the network table in \e BusCommPltf_cfg.c
    \code
    const BCP_MasterBoardTableType PP_NtwkTable[BOARD_NUM]=
    {
      {MB_ADDRESS, BCP_I2C_CH_ID},
      {UI_ADDRESS, BCP_I2C_CH_ID},
      {WS_ADDRESS, BCP_I2C_CH_ID},
      {ZB_ADDRESS, BCP_I2C_CH_ID},
    };
    \endcode


    You should get addresses from \ref CP_RESERVED_ADDRESSES, but it is possible to define a new one in configuration file.
    The order in \e eBoardId and \e PP_NtwkTable must match.
*/

/**
  \page bcpmasterprotocolcfg Protocol configuration
    \section bcpmasterprotocolcfg_options Protocol Options configuration
    \li The definition CP_GENERAL_PURPOSE in \e BusCommPltf_cfg.h allows the use of PPM_REQPARAMTYPE_GP in PP_MasterProtocolTable options.
    \li The definition CP_BOOTLOADER in \e BusCommPltf_cfg.h allows the use of BCP_BOOTLOADER in PP_MasterProtocolTable answer state machine.


    \section bcpmasterprotocolcfg_request Protocol Requests configuration
    Enumerate the Protocol requests in \e BusCommPltf_cfg.h
    \code
    enum ePP_Packet_Requests
    {
        PP_IDENT_REQ,         // Ident main request indentifier
        PP_WREGS_REQ,         //
        PP_REQ_NUM,           // Number of requests
    };
    \endcode

    PP_REQ_NUM is the total number of devices.

    Define the Packet Table in \e BusCommPltf_cfg.c

	In the packet Table these parameters must be specified:
	\li	Request parameters
	\li output buffer pointer
	\li code, if the request is not general purpose
	\li pointer to paremeter length
	\li size in bytes output buffer
	\li options. Related documentation: \ref PR_MASTER_REQUEST_OPTIONS
	\li answer state machine. Related documentation: \ref PR_MASTER_STATE_MACHINE
	\li parameter type. Related documentation: \ref PR_PARAM_TYPE


    \code
    const uint8_t IdentParamLen=1;              //const because doesn't change.
    uint8_t IdentReqLen[1], IdentAnswer[64];
    uint8_t WRegsReqParamLen, WRegsReqLen[14];  //WRegsReqParamLen can change

    const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
      {IdentReqLen,   IdentAnswer,    0x49, &IdentParamLen,     sizeof(IdentAnswer),  PPM_OPTION_DATACOPY|PPM_OPTION_GETDATA, BCP_DEFAULT, PPM_REQPARAMTYPE_EXTENDED},
      {WRegsReqLen,   0,              0x61, &WRegsReqParamLen,  0,                    PPM_OPTION_CRC, BCP_DEFAULT,                         PPM_REQPARAMTYPE_EXTENDED },
    };
    \endcode

    Note that in the second row the pointer to the answer data is zero because this request is without answer.


    In order to save memory you can share buffer for the param and the answer, like in the following example:
    \code
    const uint8_t IdentParamLen=1;              //const because doesn't change.
    uint8_t Ident[64];
    uint8_t WRegsReqParamLen, WRegsReqLen[14];  //WRegsReqParamLen can change


    const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
      {Ident,         Ident,          0x49, &IdentParamLen,     sizeof(PP_GPValue), PPM_OPTION_DATACOPY|PPM_OPTION_GETDATA, BCP_DEFAULT, PPM_REQPARAMTYPE_EXTENDED},
      {WRegsReqLen,   0,              0x61, &WRegsReqParamLen,  0,                  PPM_OPTION_CRC, BCP_DEFAULT,PPM_REQPARAMTYPE_EXTENDED },
    };
    \endcode


	\section bcpmasterprotocolcfg_gp General purpose requests
	\code
	const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
      {BridgeDataIn,    BridgeDataOut,     	0,  &BridgeParamLen,  	sizeof(BridgeDataOut), 	0	,	BCP_DEFAULT,	PPM_REQPARAMTYPE_GP},
    };
	\endcode

    \section bcpmasterprotocolcfg_boot Boot requests
    \code
	const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
      {BridgeDataIn,    BridgeDataOut,     	0,  &BridgeParamLen,  	sizeof(BridgeDataOut), 	0	,	BCP_BOOTLOADER, PPM_REQPARAMTYPE_BOOT}},
    };
	\endcode

    \section bcpmasterprotocolcfg_notimeout  Not Timeouted requests
    Not timeouted requests are subject to attempts.
    \code
    const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
		{BridgeDataIn,    BridgeDataOut,     	0,  &BridgeParamLen,  	sizeof(BridgeDataOut), 	0	,	BCP_DEFAULT,	PPM_REQPARAMTYPE_GP},
		...
    };
    \endcode

    Related documentation: \ref bcpcrmcfg_timeouts



	\section bcpmasterprotocolcfg_timeouted Timeouted requests
	\code
    const PP_MasterProtocolTableType PP_MasterProtocolTable[PP_REQ_NUM]=
    {
		{BridgeDataIn,    BridgeDataOut,     	0,	&BridgeParamLen,  	sizeof(BridgeDataOut), 	PRT_OPTION_TIMEOUTED,	BCP_BOOTLOADER, PPM_REQPARAMTYPE_BOOT},
		...
    };
	\endcode

	Related documentation: \ref bcpcrmcfg_timeouts
*/
/**

  \page cpErrors Errors and unblock routines
    \li Bus monitor method \ref BCP_BusMonitor
    \li Explanation of communication error counter \ref eCP_ErrorCounters  
*/

/**
  \page bcpmasterinclude Including files
  \section bcpcrmcfg_includes Including files
      \e BusCommPltf_cfg.c needs to include:


      \li Bus Communication Platform configuration
      \code
      #include "BusCommPltf_cfg.h"
      \endcode


      \li driver
      \code
      #ifdef BUS_COM_PLT_I2C_SUPPORT
      #include "OCI2C_drv.h"
      #endif
      \endcode

      \e BusCommPltf_cfg.h has only application specific includes. For example for Osek support:

      \li HAL and related files
      \code
      #include "Config.h"
      #include "Hal_Interface.h"
      #include  HAL_CFG
      \endcode

      \li drivers support
      \code
      #include "io_driver.h"
      \endcode

      \li Bus Communication Platform
      \code
      #include "BCP_General.h"
      \endcode


      \code
      #include "osek.h"
      \endcode

*/

/**

  \page bcpcrmcfg Communication Request Manager configuration

    \section bcpcrmcfg_mastersupport Enable BCP master support
     Define \b BCP_MASTER_SUPPORT as TRUE in \e BusCommPltf_cfg.h
    \code
    #define BCP_MASTER_SUPPORT	TRUE
    \endcode

    \section bcpcrmcfg_requesttable Request Table
    Define eRequest in \e BusCommPltf_cfg.h
    \code
    enum eRequest
    {
      PACKET_WR_REGS_REQ_MAIN,        // Main Write-regulations  request.
      PACKET_RD_IDNT_REQ_MAIN,        // Main Ident request
      BCM_REQ_NUM                     // Number of requests.
    };
    \endcode

    Define the corresponding BCM_MasterRequestTable in \e BusCommPltf_cfg.c
    \code
    const BCM_MasterRequestTableType BCM_MasterRequestTable[BCM_REQ_NUM]=
    {
      { PP_WREGS_REQ, MB_ID },
      { PP_IDENT_REQ, MB_ID },
    };
    \endcode

    \note     Lower index means higher priority.


    You can use the same protocol request to do the same request to more devices. In this case
    eRequest and BCM_MasterRequestTable became:

    \code
    enum eRequest
    {
      PACKET_WR_REGS_REQ_MAIN,        // Main Write-regulations  request.
      PACKET_RD_IDNT_REQ_MAIN,        // Main Ident request
      PACKET_RD_IDNT_REQ_VISUAL,      // Visual Ident request
      BCM_REQ_NUM                     // Number of requests.
    };
    \endcode

    \code
    const BCM_MasterRequestTableType BCM_MasterRequestTable[BCM_REQ_NUM]=
    {
      { PP_WREGS_REQ, MB_ID },
      { PP_IDENT_REQ, MB_ID },
      { PP_IDENT_REQ, UI_ID },
    };
    \endcode


	\section bcpcrmcfg_timers Timers
	In order to allow the Communication Request Manager to manage requests timeouts, attempts, timer configuration is needed in \e BusCommPltf_cfg.h.

	\li	\b CP_CRM_SET_TIMER_VAL, used to start the timer
	\li \b CP_CRM_ENABLE_TIMER, used to enable the timer
	\li \b CP_CRM_DISABLE_TIMER, used to disable the timer
	\li \b CP_CRM_IS_TIMER_EXPIRED, tests if timer is expired
	\li \b CP_CRM_IS_TIMER_NOT_EXPIRED, tests if timer is not expired
	\li \b CP_CRM_IS_TIMER_RUNNING, tests if timer is running

	Example:
	EmbOs:
	\code
	#define SET_TIMER_VAL(val)    OS_SetTimerPeriod(&CommLoLvl, val); \
	                              OS_RetriggerTimer(&CommLoLvl);

	#define ENABLE_TIMER          (OS_StartTimer(&CommLoLvl);)

	#define DISABLE_TIMER         OS_StopTimer(&CommLoLvl);

	#define IS_TIMER_EXPIRED      (OS_GetTimerStatus(&CommLoLvl)==0)

	#define IS_TIMER_NOT_EXPIRED  (OS_GetTimerStatus(&CommLoLvl)!=0)

	#define IS_TIMER_RUNNING	!IS_TIMER_EXPIRED
	\endcode

	where
	\code
	OS_TIMER CommLoLvl;
	\endcode

	\section bcpcrmcfg_timeouts Timeouts and attempts
	These are the properties to be defined to manage timeouts and attempts of the requests

	\li \b CP_MASTER_TIMEOUT_REQUEST	is maximum duration for the request. As this value is loaded into the timer, it must be computed taking into account the timer settings.

	There are two type of requests
\n	Timeouted requests:
	\li \b CP_MASTER_ATTEMPTS_TIMEOUT is the request timeout. As this value is loaded into the timer, it must be computed taking into account the timer settings.


\n	Not timeouted requests:
	\li \b CP_MASTER_NUM_REPETITIONS_PER_ATTEMPS	is the number of repetition in a sequence of an attempts
	\li \b CP_MASTER_MAX_DELAY_ATTEMPTS				is the distance between two sequences (stable). As this value is loaded into the timer, it must be computed taking into account the timer settings.
	\li \b CP_MASTER_STEP_DELAY_ATTEMPS				is the initial distance between two sequences (ramp). As this value is loaded into the timer, it must be computed taking into account the timer settings.

*/

/**
  \page bcpslavecfg BCP slave configuration
    \li Driver configuration \subpage bcpmasterdrivercfg
    \li \subpage bcp_options
    \li \subpage bcp_options_callback
    \li \subpage bcpcsmcfg
		\li Protocol configuration: see \subpage bcpslaveprotocolcfg
*/

/**

	\page bcpslaveprotocolcfg Platform Protocol configuration
	\par Slave mode

	\li BusCommPltf_cfg.h
	\b BCP_FRAGMENT_SLAVE
	If defined, fragmentation for answer on all channels is used.
	This option can be used in \e BusCommPltf_cfg.h to save ram for communication buffer.
	At least 20 bytes should be used.

	\code
    #define BCP_FRAGMENT_SLAVE
  \endcode
  	\b CP_TYP8_16_SLAVE_PP_STATE_MACHINE
	If defined, enables the TYP8 and TYP16 parser platform protocol, else only TYP8 is available
	\code
    #define CP_TYP8_16_SLAVE_PP_STATE_MACHINE
  	\endcode
  	
  	
  	\b CP_LEV2_I2CDRIVER_ACTION_ENABLE
  	If not defined any action on level 2 is prevented. If defined default action is provided by driver
*/

/**
  \page bcpcsmcfg Communication Slave Manager configuration

    \section bcpcrmcfg_slavedoc Reference documentation
    \ref CSM

    \section bcpcrmcfg_slavesupport Enable BCP slave support
     Define BCP_SLAVE_SUPPORT as TRUE in \e BusCommPltf_cfg.h
    \code
    #define BCP_SLAVE_SUPPORT	TRUE
    \endcode

    \section bcpcrmcfg_eosnsupport Enable Eosn support if needed
     Define ENABLE_BCP_EOSN as TRUE in \e BusCommPltf_cfg.h if the application requires to manage the event 'end of slave communication'
    \code
    #define ENABLE_BCP_EOSN	TRUE
    \endcode

    \section bcpcrmcfg_eosncfg Configure End Of Slave Notification
    Define in \e BusCommPltf_cfg.c if the End Of Slave Notification
    \code
    BEGIN_BCP_EOSN();   // <- Begin of EOSN
    [...]
    END_BCP_EOSN()      // <- End of EOSN
    \endcode

    See \subpage csm_eosn_cfg


    \section bcpcrmcfg_samsupport Slave Answer Manager
     Declare in \e BusCommPltf_cfg.h if the Slave Answer Manager 'My_SlaveManager'
    \code
    DECLARE_BCP_SLAVEMANAGER(My_SlaveManager);
    \endcode

    Define in \e BusCommPltf_cfg.c if the Slave Answer Manager 'My_SlaveManager'
    \code
    BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
    ...
    END_BCP_SLAVEMANAGER()
    \endcode

    Slave Answer Manager can manage several types of frames. See \subpage csm_sam_cfg

    Finally call it in your code
    \code
    ...
    CALL_BCP_SLAVEMANAGER(My_SlaveManager, BCP_I2C_CH_ID);
    ...
    \endcode
*/

#ifndef __BCP_GENERAL_H
#define __BCP_GENERAL_H



/* ********** */
/* Includes */
/* ********* */

#include  "Hal_Interface.h"

#include  HAL_CFG
#include "io_driver.h"





/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
  \ingroup PP
  @name Simboli dei frame del protocollo
  @{
*/
#define PP_STX          0xA5    /**< \hideinitializer Start byte */
#define PP_FTYP_EE      0xEE    /**< \hideinitializer Frame esteso */

#define PP_HDR_CRC      0x01    /**< \hideinitializer Header EDM: CRC */
#define PP_HDR_CHK      0x00    /**< \hideinitializer Header EDM: Checksum */

#define PP_HDR_TYP_8    0x00    /**< \hideinitializer Header TYP_H: TYP has 8 bit */
#define PP_HDR_TYP_16   0x02    /**< \hideinitializer Header TYP_H: TYP has 16 bit */

#define PP_EE_TYPE		  0xEE    /**< \hideinitializer valore del campo #PP_FTYP_EE per pacchetti estesi */
#define PP_NACK         0x55    /**< \hideinitializer Nack */
#define PP_ACK			    0x5A    /**< \hideinitializer ACK */
/*@}*/ /* PP */


/**
  \ingroup PP
  Extended frame fields index.
*/
enum ePP_FIELD_POSITION
{
  /**\hideinitializer  STX index */
  PP_STX_INDEX=0,
  /**\hideinitializer  FTYP (Frame type) index: EE -> extended, else basic */
  PP_FTYP_INDEX,
  /**\hideinitializer  HDR (Header) index (only if FTYP = EE) */
  PP_HDR_INDEX,
  /**\hideinitializer  TYP0 (DataType0, only if HDR&2 != 0 ) */
  PP_TYP0_INDEX,
  /**\hideinitializer  TYP1 (DataType1 in byte dei dati)*/
  PP_TYP1_INDEX,
  /**\hideinitializer  GLEN (Generic LEN size of data in bytes)*/
  PP_GLEN_INDEX,
  /**\hideinitializer  DAT (primo dato) */
  PP_GDAT_INDEX
};


#define PP_I2C_ADDRESS	PP_GDAT_INDEX

/* ACK */
/**\hideinitializer posizione dell'ACK nel buffer di risposta */
#define PP_ACK_POS      0
/**\hideinitializer dimensione in byte dell'ACK nel buffer di risposta */
#define PP_ACK_SZ       1

/**\hideinitializer dimensione in byte di HDR (Header, solo extended) */
#define PP_HDR_SZ       1
/**\hideinitializer dimensione in byte di DTYP (DataType, solo extended) */
#define PP_DTYP_SZ      1
/**\hideinitializer dimensione in byte di LEN (DataType, solo extended) */
#define PP_LEN_SZ       1



/* tipo e dati */
/* posizione dei campi master builder (richiesta, basic) */
/**\hideinitializer Posizione DAT richiesta, basic */
#define PP_BS_DAT_INDEX           PP_HDR_INDEX
/**\hideinitializer Posizione TYP richiesta, basic */
#define PP_BS_TYP_INDEX           PP_FTYP_INDEX

/* posizione dei campi master builder (risposta, basic) */
/**\hideinitializer Posizione DAT risposta, basic */
#define PP_BS_DAT_ANS_INDEX       PP_DTYP_INDEX
/**\hideinitializer Posizione LEN risposta,basic */
#define PP_BS_LEN_ANS_INDEX       PP_HDR_INDEX


#define PP_DTYP_INDEX             PP_TYP0_INDEX
#define PP_LEN_INDEX              PP_TYP1_INDEX
#define PP_DAT_INDEX              PP_GLEN_INDEX




#define PP_OUT_BUFF_RESERVED_SZ    (sizeof(IO_BufferSizeType))
/**
  \ingroup PP_ZB
  field's positions for a ZB request.
*/
enum ePP_GP_FIELD_POSITION
{
  /**\hideinitializer posizione Frame Control */
  PP_GP_SEQ_INDEX=0,
  /**\hideinitializer posizione Seq number */
  PP_GP_CMDID_INDEX,
  /**\hideinitializer posizione Indirizzo I2C */
  PP_GP_I2C_ADDR_INDEX,
  /**\hideinitializer posizione HDR */
  PP_GP_HDR_INDEX,
  /**\hideinitializer posizione TYP */
  PP_GP_TYP_INDEX,
  /**\hideinitializer posizione LEN (dimensione in byte dei dati)*/
  PP_GP_LEN_INDEX,
  /**\hideinitializer posizione DAT (primo dato) */
  PP_GP_DAT_INDEX
};



#define PP_GP_SZ_ANS_INDEX      0
/**
  \ingroup PP_GP
  field's positions for a GP answer.
*/
enum ePP_GP_FIELD_ANS_POSITION
{
  /**\hideinitializer Seq number position*/
  PP_GP_SEQ_ANS_INDEX=PP_OUT_BUFF_RESERVED_SZ,
  /**\hideinitializer Command ID */
  PP_GP_CMDID_ANS_INDEX,
  /**\hideinitializer ACK position*/
  PP_GP_ACK_ANS_INDEX,
  /**\hideinitializer HDR position*/
  PP_GP_HDR_ANS_INDEX,
  /**\hideinitializer TYP position*/
  PP_GP_TYP_ANS_INDEX,
  /**\hideinitializer LEN position (size in byte)*/
  PP_GP_LEN_ANS_INDEX,
  /**\hideinitializer DAT position(first data) */
  PP_GP_DAT_ANS_INDEX
};

#define PP_GP_OUT_BUFF_RESERVED_SZ    (PP_GP_DAT_ANS_INDEX)


/** Communication Platform Counters 
This documentation is relating to CommErr variable.
\sa
\n For further information about I2C blocks: 
\ref BCP_BusMonitor
\n For further information about options relating to error counters:
\ref bcp_options_err_counters
*/
enum eCP_ErrorCounters
{
  BCP_PACKET_NUM_ALL_CNT=0,               /**< Total packets (master only). This counter is alway present */
  BCP_PACKET_NUM_NOERR_CNT,               /**< Good packets. This counter is alway present.*/
  BCP_PP_CORRUPTED_PKT_ERR_CNT,           /**< Corrupted Packets 
                                                \n \li integrity (chk, crc)
                                                \li format (stx, header, len)
                                                This counter is alway present.
                                          \todo: type must be checked??
                                          */
  BCP_PP_ACK_MISSING_ERR_CNT,             /**< ACK protocol missing. This counter is alway present. */
  BCP_DRV1_CNT,                           /**< \li I2C channel :  Arbitration Lost 
                                               \li UART channel : Overrun
                                               \n This counter is always present.*/
  BCP_DRV2_CNT,                           /**< \li I2C:  unexpected I2c-nack received
                                                This counter is always present.*/
  BCP_DRV3_CNT,                           /**< I2C:  unblock lev 1 */
  BCP_DRV4_CNT,                           /**< I2C:  unblock lev 2 */
  BCP_DRV5_CNT,                           /**< I2C:  unblock lev 3 */
#if defined(BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5)

#else
  BCP_DRV6_CNT,                           /**< I2C:  unblock lev 4. If defined BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5 property this counter is grouped with BCP_DRV5_CNT */
  BCP_DRV7_CNT,                           /**< I2C:  unblock lev 5. If defined BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5 property this counter is grouped with BCP_DRV5_CNT */
#endif
#if defined(BCP_COUNTER_OPTION__LEV_6)
  BCP_I2C_UNBLOCK_LEV6_CNT,               /**< I2C:  unblock lev 6. This is available only if defined BCP_COUNTER_OPTION__LEV_6 property. the position of this counter is subject to 
                                                BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5definition */
#endif
  BCP_COUNTERS_NUM

};

#define BCP_I2C_AL_CNT              BCP_DRV1_CNT
#define BCP_I2C_NACK_CNT            BCP_DRV2_CNT
#define BCP_I2C_UNBLOCK_LEV1_CNT    BCP_DRV3_CNT
#define BCP_I2C_UNBLOCK_LEV2_CNT    BCP_DRV4_CNT
#define BCP_I2C_UNBLOCK_LEV3_CNT    BCP_DRV5_CNT

#if defined(BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5)
#define BCP_I2C_UNBLOCK_LEV4_CNT    BCP_DRV5_CNT
#define BCP_I2C_UNBLOCK_LEV5_CNT    BCP_DRV5_CNT
#else
#define BCP_I2C_UNBLOCK_LEV4_CNT    BCP_DRV6_CNT
#define BCP_I2C_UNBLOCK_LEV5_CNT    BCP_DRV7_CNT
#endif


#define BCP_UART_OV_CNT             BCP_DRV1_CNT
#define BCP_UART_FE_CNT             BCP_DRV2_CNT
/**
  \page bcp_options Communication Manager options

  \li \subpage  bcp_options_err_counters

  \page bcp_options_err_counters  Errors Counter options.

  \li \b BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5
		If defined levels 3 4 and 5 of unblocking routines are managed with the same counter.
  \li \b BCP_COUNTER_OPTION__LEV_6
		If defined this option enables the counter for the level 6 of unblocking rouines.



  \page bcp_options_callback  Callback fo slave request
  Mandatory.\n
  This callback is called when a valid frame is detected and it's time to call the Slave Answer Manager provided by the application.
  The way to call Slave Answer Manager is application dependant.
  In the following example the callback sets \c EVENT_I2C_PROTOCOL event for the task \c Task_Protocol, in which there is the call to Slave Answer Manager.

  In \e BusCommPltf_cfg.h
  \code
  #define BCM_SLAVE_ACTION_CB ActionComm

  void ActionComm(IO_ChannelType ch, IO_NotificationType notif); // callback prototype
  \endcode

  In \e BusCommPltf_cfg.c
  \code
  void ActionComm(IO_ChannelType ch, IO_NotificationType notif)
  {
  	if(ch == BCP_I2C_CH_ID)
  	{
    	SetEvent(Task_Protocol, EVENT_I2C_PROTOCOL);
  	}
  }
  \endcode
*/




/**
  \page csm_sam_cfg Slave Answer Manager configuration

  \par Table of contents
  \li \subpage csm_sam_cfg_bs
  \li \subpage csm_sam_cfg_ee

  \ref	CSM_SAM

  \page csm_eosn_cfg End Of Slave Notification
  \par Table of contents
  \li \subpage csm_eosn_cfg_ee
*/

/**
\page bcpinitcode How to use CP: initialization code examples
\par Driver initialization with CP
\li I2C Slave mode only
Includes required
\code
  #include "OCI2C_drv.h"
  #include "BCP_BusCommMng.h"
\endcode
Code:
\code
  #if defined(PLATFORM2_SUPPORT)
    IO_PhyInitSyncType i2cParam={0};

    i2cParam.eventCB = PP_SlaveNotification;		//-> Platform Protocol callback
    i2cParam.id=BCP_I2C_CH_ID;

    OCI2C_InitSync(&i2cParam);
  #endif
\endcode

\li I2C Slave mode and Master mode
Includes required
\code
  #include "OCI2C_drv.h"
  #include "BCP_BusCommMng.h"
\endcode
Code:
\code
  #if defined(PLATFORM2_SUPPORT)
    IO_PhyInitSyncType i2cParam={0};

    i2cParam.eventCB = PP_SlaveNotification;		//-> Platform Protocol callback
    i2cParam.id=BCP_I2C_CH_ID;

    OCI2C_InitSync(&i2cParam);

    (void)BCP_InitChSync(BCP_I2C_CH_ID);
  #endif

\endcode

Call to \c BCP_InitChSync can be omitted if Slave mode only is used.
*/

/**
  \page bcpinplt1prj How to use CP: integration in Platform 1.0-based projects

  Platform 1.0 since the baseline 1.1.86 has the support for BCP.

  \par Requirements
  \li Rebase the application on Platform 1.0 baseline 1.1.86


  \par Steps
  Follow this:
  \li Add to the stream of ClearCase project the following Platform 2.0 components
    - HAL (Hardware Abstraction Layer),
    - MCU-specific HAL among those available,
    - Drivers (driver I2C, Uart e DMA, SWCRC),
    - PM_Library,
    - PM_BusCOMM (Bus Communication Platform)
    - CFG_[ \c  ProductType ] (e.g. CFG_Oven, CFG_WahshAndDryer). In the CFG_[ \c ProductFolder ] (e.g. CFG_Arcadia2) you'll find files for drivers CP configuration)
  \n
  \li Add in the project of a new \e target (Codewarrior) or \e configuration (Hew) copying from the existing, This allows to
  keep the old behaviour. For Hew users: check that the 'Download Modules' refers to the new \e target.
  \li In the new \e target add the following paths as predefined:
    - HAL\\HAL_General;
    - HAL\\RN_300H_39079 o HAL\\FS_HCS08_GB60 or the one used;
    - Drivers\\SWCRC, Drivers\\OCI2C e/o Drivers\\OCUART, and all other driver need by application
    - Drivers;
    - ProductModules\\PM_BusCOMM;
    - ProductModules\\PM_Library;
    - CFG_[ \c ProductType ]\\[ \c ProductFolder ]
  \li add paths related to the HAL configuration and BCP configuration as predefined
  (Config.h e BusCommPltf_cfg.h)
  \li Define PLATFORM2_SUPPORT in the preprocesso to activate Platform 1 supportfor Paltform 2.0
  \li Define RENESAS_300H_39079 o FREESCALE_HCS08_GB60 o FREESCALE_HCS08_GT60 in the preprocessor to activate
  HAL of the MCU.
  \li Add to the project all files of the components, the configuration files.



  \li Exclude from compiler the files I2CModule.c, DigitalProtocol.c, Uart.c
  \li Insert the initialization code for slave mode in callback CBF_ProgrammingMode_InitTaskStartUp0 e CBF_NormalMode_InitTaskStartUp1, \ref bcpinitcode

  Note the use of the preprocessor option \c PLATFORM2_SUPPORT

  \see
  \subpage bcp_configurationintro
*/




/**
  \defgroup CSM_EOSN  End Of Slave Notification (EOSN)
\addtogroup CSM
@{

	This describes End Of Slave Notification (EOSN).
	End Of Slave Notification provides a framework for the notification to the application of end of communication event.

	\par Enable the service

  \code
  #define ENABLE_BCP_EOSN TRUE
  \endcode

	\par Configuration
	\ref csm_eosn_cfg_ee



  \note At this time only Platform Protocol Extended frames are supported.


	\addtogroup CSM_EOSN
	@{


*/



/**
\hideinitializer
Begins EOSN

\code
BEGIN_BCP_EOSN();
[...]
END_BCP_EOSN()
\endcode

\version		1.0.0
\date 			24/09/2009
\author			Roberto Fioravanti
*/
#define BEGIN_BCP_EOSN()  \
void PP_EosnFunction(IO_ChannelType ch) \
{

/**
\hideinitializer
Ends EOSN

\code
BEGIN_BCP_EOSN();
[...]
END_BCP_EOSN()
\endcode

\version		1.0.0
\date 			24/09/2009
\author			Roberto Fioravanti
*/
#define END_BCP_EOSN()  \
}

/*@}*/ /* CSM_EOSN_SUPP */


#define BEGIN_CP_GP_PARAM_FILTER_TYP(_pGpParamData) 	\
	switch(*(_pGpParamData+PP_GP_TYP_INDEX))	\
	{

#define END_CP_GP_PARAM_FILTER_TYP() 		\
	}

#define BEGIN_CP_MATCH_VAL(val)\
	case val:

#define END_CP_MATCH_VAL()\
	break;




/**
  \defgroup CSM_SAM  Slave Answer Manager (SAM)

@{

Slave Answer Manager is an interface for Platform Protocol frame management.
Pros:
 \li flexible management of the requests with the control of the application.
 \li reuse of code.

Platform Protocol frame supported are:
\li Extended
\li Basic

  \section CSM_SAM_example Example
  Slave Answer Manager with section for:
    \li Extended frames
    \li Basic frames

needs for
\par Definition

\code
BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
  //sezione pacchetti estesi
  BEGIN_BCP_SLAVEMANAGER__EE()
    [...]
  END_BCP_SLAVEMANAGER__EE()

  //sezione pacchetti basic
  BEGIN_BCP_SLAVEMANAGER__BS()
    [...]
  END_BCP_SLAVEMANAGER__BS()

END_BCP_SLAVEMANAGER()
\endcode


\par Declaration

\n
\code
DECLARE_BCP_SLAVEMANAGER(My_SlaveManager);
\endcode


\par Call
  pass:
- identifier (My_SlaveManager)
- identifier of the channel (I2C_CH)

\n
\code
CALL_BCP_SLAVEMANAGER(My_SlaveManager, I2C_CH);
\endcode


*/
/**
\hideinitializer
Calling to the Slave Answer Manager
\param [in]   fcn_name  identifier
\param [in]   ch  channel of communication
\param [in]   pRxBuff  pointer to Rx buffer
\param [in]   pTxBuff  pointer to Tx buffer
\param [in]   pProtParam  pointer to protocol parameters

\return error code (IO_ErrorType)
\retval IO_E_INVALID_VALUE invalid frame code
\retval IO_E_OK  frame sucessfully processed
\warning

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
\todo completare commento
*/
#define CALL_CP_SLAVEMANAGER(fcn_name, ch, pRxBuff, pTxBuff, pProtParam) fcn_name(ch, pRxBuff, pTxBuff, pProtParam)

/**
\hideinitializer
Declares a Slave Answer Manager
\param [in]   fcn_name  Slave Manager function identifier.
\version		1.0.0
\date 			08/06/2009
\author			Roberto Fioravanti
*/
#define DECLARE_CP_SLAVEMANAGER(fcn_name) IO_ErrorType fcn_name(IO_ChannelType ch, IO_MemPtrType pRxBuff, IO_MemPtrType pTxBuff,  const CP_InProtParamPType* pProtParam)



/**
\hideinitializer
Calling to the Slave Answer Manager
\param [in]   fcn_name  identifier
\param [in]   ch  channel of communication

\return error code (IO_ErrorType)
\retval IO_E_INVALID_VALUE invalid frame code
\retval IO_E_OK  frame sucessfully processed
\warning

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
\todo completare commento
*/
#define CALL_BCP_SLAVEMANAGER(fcn_name, ch) fcn_name(ch)

/**
\hideinitializer
Declares a Salve Answer Manager
\param [in]   fcn_name  Slave Manager function identifier.
\version		1.0.0
\date 			08/06/2009
\author			Roberto Fioravanti
*/
#define DECLARE_BCP_SLAVEMANAGER(fcn_name)  \
IO_ErrorType fcn_name(IO_ChannelType ch)

/**
\hideinitializer
Begins the definition of a Slave Answer Manager
\param [in]   	fcn_name function identifier
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
*/
#define BEGIN_BCP_SLAVEMANAGER(fcn_name)  \
  IO_ErrorType fcn_name(IO_ChannelType ch){ \
  PP_SlaveBuilderInfoType info={0};\
  if( PP_IsSlaveStateRxOK(ch) )\
  {\
    info.error = (IO_ErrorType)IO_E_OK;


/**
\hideinitializer
Ends the definition of a Slave Answer Manager
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
*/
#define END_BCP_SLAVEMANAGER() \
  } else\
  {\
    info.error = (IO_ErrorType)IO_E_CHECK_PP;\
  }\
	if(info.error == (IO_ErrorType)IO_E_WORKING)\
		{return info.error;}	\
  PP_SlaveResponseBuilder(ch, info);\
  (void)PP_SlaveStartResponse(ch);\
	return info.error; }

/*@}
\ingroup CSM
*/ /* CSM_SAM */



/**
  \defgroup PP_FRAME_TYPES Platform Protocol frame types
  This module describes all the frame types.

  \addtogroup PP_FRAME_TYPES
  @{
      @name Extended frame types
      @{

*/
#define PP_EE_TYPE_WRITE_RAM			          	0x20	/**<\hideinitializer Write RAM */
#define PP_EE_TYPE_READ_FLASH			          	0x3F	/**<\hideinitializer Read FLASH*/

#define PP_EE_TYPE_READ_STATUS						0x60	/**<\hideinitializer Status Request*/
#define PP_EE_TYPE_WRITE_REGS			          	0x61	/**<\hideinitializer Reg Request*/
#define PP_EE_TYPE_READ_REGS			          	0x62	/**<\hideinitializer Reg Write*/
#define PP_EE_TYPE_WRITE_UIINFO		          		0x63	/**<\hideinitializer Write UI info*/
#define PP_EE_TYPE_SYSTEMMODE			          	0x5F  	/**<\hideinitializer SystemMode */

#define PP_EE_TYPE_READMEMORY_EXTENDED              0x41	/**<\hideinitializer read memory extended */
#define PP_EE_TYPE_WRITERAMMEMORY_EXTENDED          0x42	/**<\hideinitializer write ram memory extended */
#define PP_EE_TYPE_ERASEFLSMEMORY_EXTENDED          0x43	/**<\hideinitializer erase flash memory extended */
#define PP_EE_TYPE_WRITEFLSMEMORY_EXTENDED          0x44	/**<\hideinitializer write flash memory extended */
#define PP_EE_TYPE_IDENT                            0x49	/**<\hideinitializer Ident Command (0x49 is 'I')*/
#define PP_EE_TYPE_EXTENDED_IDENT_REQ				0x46	/**<\hideinitializer Extended Ident Request */
#define PP_EE_TYPE_SETTING_DATA_BLOCK               0x4A	/**<\hideinitializer Request Setting Data Block */
#define PP_EE_TYPE_LKTABLE_DATA_BLOCK               0x4B	/**<\hideinitializer Request Lookup Table Data Block */
#define PP_EE_TYPE_GENERIC_READ_WRITE               0x4C	/**<\hideinitializer Request Generic Read/Write */
#define PP_EE_TYPE_ERASE_FLASH_PAGE                 0x4E	/**<\hideinitializer Erase Flash Page*/
#define PP_EE_TYPE_WRITE_FLASH                      0x4F	/**<\hideinitializer Write Flash*/
#define PP_EE_TYPE_LOADS                            0x50	/**<\hideinitializer PC Control Extended*/
#define PP_EE_TYPE_SYSTEM_MODE                      0x5F  	/**<\hideinitializer System Mode*/
#define PP_EE_TYPE_DSP_CMD_READ						0x6A  	/**<\hideinitializer DSP CMD READ*/
#define PP_EE_TYPE_DSP_INFO_WRITE					0x6B  	/**<\hideinitializer DSP INFO WRITE*/
#define PP_EE_TYPE_DSP_READ_EXT_RAM					0x6C  	/**<\hideinitializer DSP read external RAM */
#define PP_EE_TYPE_DSP_WRITE_EXT_RAM				0x6D  	/**<\hideinitializer DSP write external RAM*/
#define PP_EE_TYPE_DSP_EXTRA_REQ_RAM				0x69  	/**<\hideinitializer DSP Extra request */

#define PP_EE_TYPE_SET_I2C_CLK_RATE         		0x91  	/**<\hideinitializer Set Clock rate for bridge */
#define PP_EE_TYPE_SET_UART_BAUD_RATE       		0x99  	/**<\hideinitializer Set Baud rate for bridge */

#define PP_EE_TYPE_REQ_APPL_POWER_PROFILE			0xF6	/**<\hideinitializer Request Power Profile */
#define PP_EE_TYPE_REQ_APPL_COST_SET				0xF7	/**<\hideinitializer Read Cost */
#define PP_EE_TYPE_REQ_APPL_READPOWERMETER			0xF8	/**<\hideinitializer Read Power Meter  */
#define PP_EE_TYPE_REQ_APPL_WRITEPOWERMETER			0xF9	/**<\hideinitializer Write Power Meter  */


#define PP_EE_TYPE_CFG_SENSOR               0xB1	/**<\hideinitializer Sensor config */
/*#define PP_EE_TYPE_DATA_SENSOR				      0xB2*/	/**<\hideinitializer Sensor info */

#define PP_EE_TYPE_EXP_SET_INFO             0xB2  /**<\hideinitializer  Expansions's Type, Write Expansion Info Type
    utilizzata dalle espansion board per fornire dati alla Main */
#define PP_EE_TYPE_EXP_GET_INFO             0xB3  /**<\hideinitializer  Expansions's Type, Read Expansion Info Type
    utilizzata dalle espansion board per leggere dalla Main i comandi da eseguire */


#define PP_BOOT_TYPE_ACK					0xFC
#define PP_BOOT_TYPE_IDENT					0x49
#define PP_BOOT_TYPE_ERASE					0x45
#define PP_BOOT_TYPE_WRITE					0x57
#define PP_BOOT_TYPE_REQ_WE_ID				0x53
#define PP_BOOT_TYPE_QUIT					0x51
#define PP_BOOT_TYPE_READ					0x52

/*    @}*/

/**   @name Basic frame types
      @{
*/
#define PP_BS_TYPE_READ_RAM					        0x10	/**<\hideinitializer Basic Read Operation*/
#define PP_BS_TYPE_WRITE_RAM                0x20  /**<\hideinitializer Basic Write Operation*/
#define PP_BS_TYPE_LOADS                    0x50  /**<\hideinitializer Basic Set Loads*/
/*    @} Basic frame types*/

/*  @} addtogroup PP_FRAME_TYPES */

#define PP_BS_TYPE_MSK                      0xF0

/**
    @name Types
    @{
*/
#define PP_CMD_EXTENDED_WITH_ANSW        0x95  /**<\hideinitializer Extended frame with answer from slave */
#define PP_CMD_EXTENDED_NO_ANSW          0x9D  /**<\hideinitializer Extended frame without answer form slave */
#define PP_CMD_BASIC                     0x93  /**<\hideinitializer basic frame encapsulated */
#define PP_CMD_BOOT_LOADER               0x97  /**<\hideinitializer Boot frame encapsulated */
/*  @} */



/**
  \defgroup CP_RESERVED_ADDRESSES  I2C Reserved Addresses

  \addtogroup CP_RESERVED_ADDRESSES
  @{
  These are I2C reserved addresses
    @{
*/
#define BM_ADDRESS  0x98              /**< I2C Bootloader Main Board */
#define MB_ADDRESS  0x90              /**< I2C Main Board */
#define UI_ADDRESS  0x9E              /**< I2C Visual */
#define WS_ADDRESS  0xB0              /**< I2C Weight Sensor */
#define ETU_ADDRESS 0xC0              /**< I2C ETU Address */
#define ZB_ADDRESS  0x9C              /**< I2C Zigbee Module*/
/*   @} Reserved Address */
/* @} addtogroup BCP_RESERVED_ADDRESSES */



/* PP MasterRequestTable Options*/
enum
{
  PPM_OPTION_GETDATA_BIT=0,
  PPM_OPTION_CRC_BIT,
  PPM_OPTION_CALLBACK_BIT,
  PPM_OPTION_DATACOPY_BIT,
  PPM_OPTION_GP_REQUEST_BIT,
  PPM_OPTION_BRIDGE_SERIAL_BIT,
  PRT_OPTION_TIMEOUTED_BIT
};

/**
    \degroup PR_MASTER_REQUEST_OPTIONS	Packet Request Table Options

    \addtogroup PR_MASTER_REQUEST_OPTIONS
    @{
      @name Options for the Packet Table
      @{
 */
#define PPM_OPTION_GETDATA        (1<<PPM_OPTION_GETDATA_BIT)       /**< \hideinitializer Request with answer */
#define PPM_OPTION_CRC            (1<<PPM_OPTION_CRC_BIT)           /**< \hideinitializer use CRC */
/*#define PPM_OPTION_CALLBACK       (1<<PPM_OPTION_CALLBACK_BIT) */     /**< \hideinitializer (not implemented yet)*/
#define PPM_OPTION_DATACOPY       (1<<PPM_OPTION_DATACOPY_BIT)      /**< \hideinitializer At the end of the answer copy the data */
#define PPM_OPTION_GP_REQUEST     (1<<PPM_OPTION_GP_REQUEST_BIT)     /**< \hideinitializer General purpose request.
\attention  This option implies that the field code is ignored. */
/*#define PPM_OPTION_BRIDGE_SERIAL  (1<<PPM_OPTION_BRIDGE_SERIAL_BIT)*/
/**< \hideinitializer Richiesta da gestire in 'Bridge ZB'.
\attention  This option implies that the field code is ignored. */

/** Transparent mode is applied to the request  */
#define PPM_OPTION_TRASPARENT_A (PPM_OPTION_TRASPARENT|PPM_OPTION_GETDATA_BIT)
/*    @} Options for the Protocol Table */
/*  @} addtogroup PR_MASTER_REQUEST_OPTIONS */

#define PRT_OPTION_TIMEOUTED	(1<<PRT_OPTION_TIMEOUTED_BIT)		/**< \hideinitializer request is subject to timeout. This is not the default behavior. The default behavior is the periodical attempt.*/


/** State machine identifiers
\defgroup PR_MASTER_STATE_MACHINE	Packet Request Answer State machine

    \addtogroup PR_MASTER_STATE_MACHINE
    @{

*/
enum eBCP_STATEMACHINE_ID
{
    BCP_DEFAULT=0,         /**< \hideinitializer Standard */
    BCP_BOOTLOADER
};
/*    @} PR_MASTER_STATE_MACHINE */



/* Parser commands */
enum{
PP_INVALID_CMD,
PP_READ_CMD,
PP_NO_MATCH_CMD,
PP_AND_CMD,
PP_CHK_CHECK_CMD,
PP_CRC_CHECK_CMD,
PP_RX_OK_CMD,
PP_MODLEN_CMD,
PP_SETLEN_CMD,
PP_READ_ALL_CMD
};




/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct
{
	uint8_t item[8];
}CP_InProtPosType;

typedef struct
{
	CP_InProtPosType pos;
}CP_InProtParamPType;


typedef void (* BCP_ProtocolCallback)(IO_ChannelType, IO_ErrorType);


/**
@name Function pointers
@{*/
/** Communication driver initialization */
typedef IO_ErrorType (* IO_PhyInitSync)(IO_PhyInitSyncType * initSync);
/** Communication driver start rx/tx */
typedef IO_ErrorType (* IO_PhyCommunicationSync)(CommDriverParamType * param);
/*@}*/

/*lint -e18 Lint bug: suppress unjustified Error 18, fixed on version t of LINT*/
/**
\addtogroup PPM
\todo descrizione
ZB Bridge Group
*/
typedef struct{
  struct{
  union{
    struct GP_Hdr_s
    {
      uint8_t SequenceNumber;
      uint8_t CommandID;
    }HdrByte;
    uint8_t Hdr[2];
  }Hdr;

  union{
    struct GP_Data_s{
      uint8_t I2cAddress;
      uint8_t PP_Hdr;
      uint8_t PP_Type;
      uint8_t PP_Len;
      uint8_t PP_Data;
    }PlByte;
    uint8_t PLByte[5];
  }PL;
  }ZBParam;

}CP_GP_ParamType;

typedef struct{
  struct{
  uint16_t len;
  union{
    struct GP_AnsHdr_s
    {
      uint8_t SequenceNumber;
      uint8_t CommandID;
    }HdrByte;
    uint8_t Hdr[2];
  }Hdr;

  union{
    struct GP_AnsData_s{
      uint8_t PP_Ack;
      uint8_t PP_Hdr;
      uint8_t PP_Type;
      uint8_t PP_Len;
      uint8_t PP_Data;
    }PlByte;
    uint8_t PLByte[5];
  }PL;
  }AnsParam;

}CP_GP_AnsParamType;
/*lint +e18 */



typedef uint16_t       BCP_PacketSizeType;			/**< packet size */
typedef uint16_t       CP_PacketSizeType;			/**< packet size */

typedef uint8_t       BCP_OptionType;				/**< Tipo Opzione */
typedef union PP_Address_tag
{
  uint16_t  data;
  struct
  {
    uint8_t h;
    uint8_t l;
  }BYTE;
}PP_AddressType;

typedef void (*CP_Callback)(void) ;

/* Slave Builder info Type*/
typedef struct
{
	IO_MemPtrType       address;    /* pointer to data to be copied in the frame */
	BCP_PacketSizeType  len;        /* size in bytes of the data to be copied */
	IO_ErrorType        error;      /* esit */
	BCP_OptionType      option;     /* options */
	CP_Callback			callback;
}PP_SlaveBuilderInfoType;


typedef uint8_t       BCP_ParamType;


typedef I2C_SlaveAddressType BCP_SlaveAddressType;

typedef struct BCP_Master_tag
{
  Comm_Notification         callback;
  BCP_PacketSizeType        requestLen;
  BCP_OptionType            option;
  BCP_SlaveAddressType      slaveAddress;
} BCP_MasterType;


typedef uint8_t   BCP_PacketCodeType;
typedef uint8_t   BCP_PacketOptionType;
typedef uint8_t   BCP_PrioType;

typedef struct PP_MasterResponseAction_tag
{
  unsigned char* pDestination;
  unsigned char len;
}PP_MasterResponseActionType;

/**
  \defgroup BCP_CFG_TABLES  Tables in BCP configuration
  @{
*/

/** Each row of the Network Table gives \e address and \e channel by witch it is possible to reach a device.
\par Example
 see \subpage  bcpmasternetworkcfg
\par Related items
BCM_MasterRequestTableType.
*/
typedef struct
{
  /** Address. Actually this field is used only by I2C.
  \n For all reserved addresse see \ref CP_RESERVED_ADDRESSES.
  Not reserved values can be used.
  */
  BCP_SlaveAddressType slaveAddress;
  /** Index of the channel table BCP_ChannelTableType */
  IO_ChannelType  drvCh;
}BCP_MasterBoardTableType;

/** Each row of this table links the identifier (index) of the platform protocol table with the identifier of the device.

*/
typedef struct
{
/** Platform Protocol request identifier.*/
  IO_ChannelType  PPReqIndex;
/** device identifier. It is index of BCP_MasterBoardTableType */
  IO_ChannelType  idBoard;
}BCM_MasterRequestTableType;


typedef IO_ChannelType BCP_StateMachineType;

typedef void (* PP_MasterResponseAction)(unsigned char* pBodyMsg, PP_MasterResponseActionType * ra);


typedef  IO_LenType  BCP_BufferSizeType, *pBCP_BufferSizeType;

/**
This table is used by PlatformProtocol to encode the request and manage the driver to request the answer to the slave device.
*/
typedef struct
{
  IO_BufferType           pDataReq;           /**< Pointer to the param for the request. Usually it is an array containing data to be encapsulate in the frame*/
  IO_BufferType           pDataRes;           /**< Pointer to the buffer for data received in the answer */
  BCP_PacketCodeType      code;               /**< code of the frame. For the available frame code see \ref PP_FRAME_TYPES.
  It can be one of Both Extended and Basic type or an application defined type */
  pBCP_BufferSizeType     pLenReq;            /**< Pointer to the len in bytes of the pDataReq field */
  BCP_BufferSizeType      LenRes;             /**< Maximum size in byte of the answer */
  BCP_PacketOptionType    option;             /**< Options: for all available options \sa PR_PROTOCOL_TABLE_OPTIONS */
  BCP_StateMachineType    idStateMachine;     /**< not used */
  BCP_ParamType           paramType;          /**< type of param in pDataReq */
}PP_MasterProtocolTableType;


/**
This table is the interface between Platform Protocol and the driver.

\note
If allowed by application it possible to:
- share rx and tx buffer
- share #reqTable between several channels.
*/
typedef struct {
    PP_MasterProtocolTableType* reqTable;      /**< Protocol request table pointer. \todo: master only*/
    IO_PhyInitSync             InitSync;       /**< Driver init funcion. */
    IO_PhyCommunicationSync    StartTxSync;    /**< Driver transmit function */
    IO_PhyCommunicationSync    StartRxSync;    /**< Driver receive function */
    IO_BufferType              txBuffer;       /**< Pointer to tx buffer */
    IO_BufferType              rxBuffer;       /**< Pointer to rx buffer */
    IO_BufferSizeType          txBufferSize;   /**< Size in bytes of the tx buffer */
    IO_BufferSizeType          rxBufferSize;   /**< Size in bytes of the rx buffer */
}BCP_ChannelTableType;
/*@}*/


/*@}*/
/*@}*/

/*Parser*/
typedef
union {

  struct {
    uint8_t cmd;
    uint8_t par1;
    uint8_t par2;
    uint8_t par3;
  }fields;
  uint32_t statement;
} PP_ParserType;



/**
Error counters
*/
typedef uint16_t  CM_Err_ClassType;


/* ****************** */
/* Exported variables */
/* ****************** */
extern const CP_InProtParamPType ProtParam[];

/* ******************* */
/* Function prototypes */
/* ******************* */

void PP_EosnFunction(IO_ChannelType ch);

#include  "BCP_PlatformProtocol.h"
#include  "CSM_PlatformProtocol_EE.h"
#include  "CSM_PlatformProtocol_BS.h"

#endif /* __BCP_GENERAL_H */

/* *********** */
/* End of file */
/* *********** */
