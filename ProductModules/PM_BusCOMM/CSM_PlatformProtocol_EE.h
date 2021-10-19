/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup	CSM_SAM_EE    Slave Answer Manager for Platform Protocol Extended frames
\ingroup CSM_SAM

\par Table of Contents
\li     \ref CSM_EE__SECTION
\li     \ref CSM_EE__REQMNG
\li     \ref CSM_EE__EXTENSIONS
\li     \ref CSM_EE__SUPP_PLATFORM1

\par Configuration
\ref csm_sam_cfg_ee

\page csm_sam_cfg_ee Slave Answer Manager support for Platform Protocol Extended  frames (EE)

\par		Table of content
\li     \subpage csm_ee_sam_section
\li     \ref CSM_EE__REQMNG
\li     \ref CSM_EE__EXTENSIONS
\li     \ref CSM_EE__SUPP_PLATFORM1



\page csm_eosn_cfg_ee End Of Slave Notification support for Platform Protocol Extended frames

\par		Table of content
\li     \subpage csm_ee_eosn_section
\li     \ref CSM_EE__EOSN_REQMNG

*/

/**
\defgroup	CSM_EOSN_EE    End Of Slave Notification Extended (EE)
\ingroup CSM_EOSN
Supporto Slave dei pacchetti estesi del Platform Protocol (EE)
\par		Descrizione
\n      Questo modulo documenta;
\li i gestori delle richieste basate su pacchetti estesi
\n
\ref 	csm_eosn_cfg_ee
*/


/**
This file contains
\li \ref CSM_EE__REQMNG
\li \subpage csm_ee_sam_section
\n
\file		CSM_PlatformProtocol_EE.h
\ingroup	CSM
\date		9/06/2009
\version	00.00.01
\author	Roberto Fioravanti
\n
\n
\par		History (titolo)
\n		00.00.64
\n		22/09/2010
\n		Roberto Fioravanti
\n		changes
\n	- BCP_SLAVEMANAGER__EE_APPL_DEFINED addition of ch arg in fnc_user
\n	- 
\n
\n		00.00.49
\n		xx/06/2010
\n		Roberto Fioravanti
\n		changes
\n    - CP_CSM_READ_FLASH_CB(callback)
\n
\n 		00.00.12
\n		08/06/2009
\n		Roberto Fioravanti
\n		changes
\n    - BCP_SLAVEMANAGER__EE_WRITE_RAM, BCP_SLAVEMANAGER__EE_READ_FLASH, BEGIN_BCP_EOSN__EE, BCP_SLAVEMANAGER__READ_APPLDATA, BCP_SLAVEMANAGER__WRITE_APPLDATA: cast applied to enum's item and error codes.
\n
\n 		00.00.12
\n		08/06/2009
\n		Roberto Fioravanti
\n		changes
\n    - BCP_SLAVEMANAGER_WRITE_APPLDATA: ritorna NACK anche se e' verificata la condizione (BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX] > maxlen)
\n		- Introduzione di EOSS (End-Of-Slave-Service): complesso di macro per la definizione delle callback da richiamare a livello protocollo
\n			per in corrispondenza al termine della comunicazione.
\n    - Eliminazione elementi deprecati
\n    -  BCP_SLAVEMANAGER__EE_READ_FLASH: corretto baco lettura range 0xFF00 - 0xFFFF
\n
\n 		00.00.01
\n		09/06/2009
\n		Roberto Fioravanti
\n		changes
\n 		- creazione
\n
\n etc.
*/


#ifndef __PLATFORMPROTOCOL_EE_H
#define __PLATFORMPROTOCOL_EE_H

/* ********** */
/* Includes   */
/* ********** */
#include "StandardDefines.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
  \page csm_ee_eosn_section Setting an Extended section in EOSN

  This page explains how to support Platform Protocol Extended frames requests in EOSN.

  \par Example:
  See the following code:

    \code
    BEGIN_BCP_EOSN()
    	BEGIN_BCP_EOSN__EE()
    	[...]                     //<-  EOSN frames managers area
    	END_BCP_EOSN__EE()
    END_BCP_EOSN()
    \endcode



    \defgroup CSM_EE__EOSN_SECTION EOSN section for Platform Protocol Extended frames
    \ingroup  CSM_EOSN_EE
    \addtogroup CSM_EE__EOSN_SECTION
    @{
*/

/**
  \hideinitializer
Begins an Extended section in EOSN.

\version		1.0.0
\date 			23/09/2009
\author			Roberto Fioravanti

*/

#define BEGIN_BCP_EOSN__EE()  \
  if(BCP_ChannelTable[ch].txBuffer[(uint8_t)PP_FTYP_INDEX+1] == PP_FTYP_EE)  \
	{  \
	  switch (BCP_ChannelTable[ch].txBuffer[(uint8_t)PP_DTYP_INDEX+1]) \
    {

/**
  \hideinitializer
Ends an Extended section in EOSN.

\version		1.0.0
\date 			23/09/2009
\author			Roberto Fioravanti

*/
#define END_BCP_EOSN__EE()  \
		default:  \
      ; \
    } \
	}

/**
  \hideinitializer
Begins an Extended section in EOSN for RX frames (e.g. Write UI Info).

\version		1.0.0
\date 			25/07/2014
\author			Alessandro Sabbatini

*/

#define BEGIN_BCP_EOSN__EE_RX()  \
  if(BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_FTYP_INDEX] == PP_FTYP_EE)  \
	{  \
	  switch (BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DTYP_INDEX]) \
    {

/*  @} addtogroup CSM_EE__EOSN_SECTION */





/**   \defgroup CSM_EE__EOSN_REQMNG EOSN requests managers for Platform Protocol Estended frames.
      \ingroup  CSM_EOSN_EE

      Each request manager has specific parameters.
      \addtogroup CSM_EE__EOSN_REQMNG
      @{
*/

/**
  \hideinitializer
Application data reading

\param [in] eeType  code of the frame to process.
\param [in] ctrlAppl application defined function that will process the frame.

\par
Prototype of \e ctrlAppl function
\code
void fnc_name(void);
\endcode

\n
\par Example
\li In \e BusCommPltf_cfg.c Define the EOSN manager.
\n
\code
BEGIN_BCP_EOSN()
	BEGIN_BCP_EOSN__EE()
		BCP_EOSN__EE(PP_EE_TYPE_READ_STATUS, CBF_OnReadStatusCompleted)
		BCP_EOSN__EE(PP_EE_TYPE_READ_REGS, CBF_OnReadRegulationsCompleted)
	END_BCP_EOSN__EE()
END_BCP_EOSN()
\endcode

where \c CBF_OnReadStatusCompleted and \c CBF_OnReadRegulationsCompleted are defined as

\code
void CBF_OnReadStatusCompleted(void)
{
	if(recoveryComProc == 0)
	{
		recoveryComProc++;
	}
}

void CBF_OnReadRegulationsCompleted(void)
{
	if(recoveryComProc >= 1)
	{
		if(recoveryComProc == 1)
		{
			recoveryComProc++;
		}

		SETBIT(ApplicationState, AS_BACKUP_ALM_OFF_BIT);
		MachineStateBuf[POS_APPL_STATE] |= PRE_BACKUP_BIT_MASK;
	}
}
\endcode

\todo provide a macro for the definition and delaration of \c ctrlAppl function
*/
#define BCP_EOSN__EE(eeType, ctrlAppl) \
case eeType:\
  ctrlAppl();\
break;

/*  @} addtogroup CSM_EE__EOSN_REQMNG */





/**
  \page csm_ee_sam_section Setting an Extended section in SAM

  This page explains how to support Platform Protocol Extended frame requests in SAM.

  \par Example:
  See the following code:

    \code
    BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)

      BEGIN_BCP_SLAVEMANAGER__EE()    // <- Begin of Extended section

        [...]                         // Extended requests managers area

      END_BCP_SLAVEMANAGER__EE()      // <- End of Extended section

    END_BCP_SLAVEMANAGER()
    \endcode


   \defgroup CSM_EE__SECTION Slave Answer Manager section for Platform Protocol Extended frames
   \ingroup CSM_SAM_EE
   \addtogroup CSM_EE__SECTION
    @{
*/


/**
\hideinitializer
Begins the definition of a Slave Answer Manager
\param [in]   	fcn_name function identifier
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
*/
#define BEGIN_CP_SLAVEMANAGER(fcn_name)  \
IO_ErrorType fcn_name(IO_ChannelType ch, IO_MemPtrType pRxBuff, IO_MemPtrType pTxBuff, const CP_InProtParamPType* pProtParam ) \
{\
	PP_SlaveBuilderInfoType info={0};\
	uint8_t temp[5];\
	(void) pProtParam;\
	(void) pTxBuff;\
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
#define END_CP_SLAVEMANAGER() \
	} else\
	{\
	  info.error = (IO_ErrorType)IO_E_CHECK_PP;\
	}\
	if(info.error == (IO_ErrorType)IO_E_WORKING)\
	{\
		return info.error;\
	}\
	PP_SlaveResponseBuilder(ch, info);\
	(void)PP_SlaveStartResponse(ch);\
	return info.error;\
}




#define BEGIN_CP_SLAVEMANAGER__EE_REQ_CHECK()  \
		if((pRxBuff[pProtParam->pos.item[7]] == PP_FTYP_EE)\
		  && ((pRxBuff[pProtParam->pos.item[2]]&PP_HDR_TYP_16) == PP_HDR_TYP_8))\
		{

#define END_CP_SLAVEMANAGER__EE_REQ_CHECK() \
		}




/**
  \hideinitializer
Begins an extended section TYP16 in SAM.

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

*/
#define BEGIN_BCP_SLAVEMANAGER__EE_TYP16()  \
  if((BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX] == PP_FTYP_EE) \
  && ((BCP_ChannelTable[ch].rxBuffer[PP_HDR_INDEX]&PP_HDR_TYP_16) == PP_HDR_TYP_16))  \
	{  \
	  switch (BCP_ChannelTable[ch].rxBuffer[PP_TYP0_INDEX]) \
    {

#define BEGIN_BCP_SLAVEMANAGER__EE_TYP16_FILTER_REQ_TYPE1()	\
	switch (BCP_ChannelTable[ch].rxBuffer[PP_TYP1_INDEX]) \
	{

#define END_BCP_SLAVEMANAGER__EE_TYP16_FILTER_REQ_TYPE1()	\
	}

#define BEGIN_BCP_SLAVEMANAGER__EE_TYP16_FILTER_REQ_TYPE()	\
	switch (BCP_ChannelTable[ch].rxBuffer[PP_TYP0_INDEX]) \
	{

#define END_BCP_SLAVEMANAGER__EE_TYP16_FILTER_REQ_TYPE()	\
	}

#define BEGIN_BCP_SLAVEMANAGER__EE_TYP16_FILTER_ADDRESS()  \
	switch(BCP_ChannelTable[ch].rxBuffer[PP_I2C_ADDRESS])	\
	{

#define CP_SLAVEMANAGER__MATCH_VAL(val)  \
	case val

#define CP_SLAVEMANAGER__END_MATCH()  \
	break

#define CP_SLAVEMANAGER__MATCH_ANY()  \
	default

#define END_BCP_SLAVEMANAGER__EE_TYP16_FILTER_ADDRESS()  \
	}

/**
  \hideinitializer
Ends an extended section TYP16 in SAM.
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

\note case default : non gestito
*/
#define END_BCP_SLAVEMANAGER__EE_TYP16() \
    default:  \
      ; \
    } \
	info.error = (IO_ErrorType)IO_E_WORKING;  \
	}



/**
  \hideinitializer
Begins an exended section in SAM.

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

*/
#define BEGIN_BCP_SLAVEMANAGER__EE()  \
  if((BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX] == PP_FTYP_EE) \
  && ((BCP_ChannelTable[ch].rxBuffer[PP_HDR_INDEX]&PP_HDR_TYP_16) == PP_HDR_TYP_8))  \
	{  \
	  switch (BCP_ChannelTable[ch].rxBuffer[PP_DTYP_INDEX]) \
    {

/**
  \hideinitializer
Ends an extended section in SAM.
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

\note case default : non gestito
*/
#define END_BCP_SLAVEMANAGER__EE() \
    default:  \
	info.error=(IO_ErrorType)IO_E_INVALID_VALUE;\
      ; \
    } \
	}

/*  @} addtogroup CSM_EE__SECTION*/











/**
    \defgroup CSM_EE__EXTENSIONS Extensions for SAM
    \ingroup CSM_SAM_EE

    This page shows the use of extension for SAM
    \addtogroup CSM_EE__EXTENSIONS
    @{
*/


#define CP_SLAVEMANAGER_RXBUFF_ASSERT_EXTENDED_REQ_POS(ee_pos)  \
		(pRxBuff[(ee_pos)] == (uint8_t)PP_FTYP_EE)
		


#define CP_SLAVEMANAGER_RXBUFF_ASSERT_TYP8_REQ_POS(hdr_pos)  \
		  ((pRxBuff[(hdr_pos)]&PP_HDR_TYP_16) == (uint8_t)PP_HDR_TYP_8)


/**
\hideinitializer
Defines a function to control the request.
The esit of the communication is determined by the returned value.
\param fncName function identifier
\return IO_ErrorType.
\retval IO_E_OK The request is good.
\retval != IO_E_OK The request fails. (NACK will be sent)

\par Example
  Declaration of a control request function
  \code
  DEF_CSM_EE_CTRL(My_CtrlRequestFunction)
  \endcode

  Definition of a control request function
  \code
  DEF_CSM_EE_CTRL(My_CtrlRequestFunction)   // <- definizione
  {
    {
      return IO_E_xxx; //  <- NACK wil be sent
    }

    return IO_E_OK; //  <-  good! ACK will be sent
  }
  \endcode

*/
#define DEF_CSM_EE_CTRL(fncName) IO_ErrorType CMS_EE_CTRL_##fncName(void)

/**
\hideinitializer
Defines a function to pass to an extended request manager info about the buffer.
\param fncName function identifier
\return IO_BufferInfoType_t

\par Prototype
The function has the following prototype
\code
 IO_BufferInfoType_t fncName(uint8_t* pdata)
\endcode

where \e pdata is the pointer to

\par Example
  Declarations
  \code
uint8_t Data[16];

  DEF_CSM_EE_BUFF(My_GetBufferFunction)
  \endcode

  Definitions
  \code
  DEF_CSM_EE_BUFF(My_GetBufferFunction)
  {
    IO_BufferInfoType_t info={0};

    info.pData = Data;
    info.size =sizeof(Data);
    return info;
  }
  \endcode
*/
#define DEF_CSM_EE_BUFF(fncName) IO_BufferInfoType_t CMS_EE_BUFF_##fncName(uint8_t* pdata)


/**
\hideinitializer
This macro allows the definition of  a function to pass informations about the buffer.
\param fncName Identifier of the function

\note The function defined by this macro has the following arguments
- uint8_t ch		communication platform channel
- uint8_t* pdata  pointer to data
- uint8_t len     size of the data in byte
- PP_SlaveBuilderInfoType* pinfo, allows the control of the answer.

If you write
-	pinfo->error =IO_E_INVALID_VALUE; packet will fail ( protocol nack )
-	pinfo->error =IO_E_OK; success

and returns void.

\par Example
  \code

DEF_CSM_EE_CUSTOM_FUNC(My_CustomFunction)   // <- declaration

  [...]

DEF_CSM_EE_CUSTOM_FUNC(My_CustomFunction)   // <- definition
{
  [...]                                     // <- custom code
}
   \endcode

\see USE_CSM_EE_CUSTOM_FUNC
*/
#define DEF_CSM_EE_CUSTOM_FUNC(fncName) void CMS_EE_FUNC_##fncName(uint8_t ch, uint8_t* pdata, uint8_t len, PP_SlaveBuilderInfoType* pinfo)

/**
\hideinitializer
Allows to use the control request function defined througth #DEF_CSM_EE_CTRL in the SAM
\param fncName function identifier

\par Eexample
  \code
  uint8_t Data[16];                       // <- data Buffer


  [...]



  //Slave Answer Manager
  [...]
  BEGIN_BCP_SLAVEMANAGER__EE()

    BCP_SLAVEMANAGER__EE_READ_APPLDATA( PP_EE_TYPE_IDENT,
                                        USE_CSM_EE_CTRL(My_CtrlRequestFunction),
                                        Data,
                                        sizeof(Data))

  END_BCP_SLAVEMANAGER__EE()
  [...]
  \endcode

*/
#define USE_CSM_EE_CTRL(fncName) CMS_EE_CTRL_##fncName

/**
\hideinitializer
Allows to use control buffer function defined througth #DEF_CSM_EE_BUFF
\param fncName function identifier
\par Example
the following example shows also the use of \c pre and \c post use of BCP_SLAVEMANAGER__EE_WRITE_APPLDATA_EX
  \code

  // Defines
  #define UI_GetResource (void)GetResource(UserInterface_Res);
  #define UI_ReleaseResource (void)ReleaseResource(UserInterface_Res);

  [...]

  // Slave Manager

  BEGIN_BCP_SLAVEMANAGER__EE()

    BCP_SLAVEMANAGER__EE_WRITE_APPLDATA_EX(PP_EE_TYPE_EXP_SET_INFO,
                                              USE_CSM_EE_CTRL(My_CtrlRequestFunction),
                                              USE_CSM_EE_BUFF(GetExpansionBuffer_SetInfo_Index),
                                              UI_GetResource,
                                              UI_ReleaseResource)

  END_BCP_SLAVEMANAGER__EE()
  \endcode
*/
#define USE_CSM_EE_BUFF(fncName) CMS_EE_BUFF_##fncName


/**
\hideinitializer
Allows to use the control request function for #BCP_SLAVEMANAGER__EE_APPL_DEFINED.
\param fncName function identifier
\par Example
  \code

  // Slave Manager
  [...]
  BEGIN_BCP_SLAVEMANAGER__EE()

    BCP_SLAVEMANAGER__EE_APPL_DEFINED(PP_EE_TYPE_SYSTEM_MODE, USE_CSM_EE_CUSTOM_FUNC(EE_SystemMode))

  END_BCP_SLAVEMANAGER__EE()
  \endcode
*/
#define USE_CSM_EE_CUSTOM_FUNC(fncName) CMS_EE_FUNC_##fncName

/*  @} addtogroup CSM_EE__EXTENSIONS*/



/*
Application data reader.
*/
#define BCP_SLAVEMANAGER__READ_APPLDATA(ctrlAppl, pData, maxlen)\
if(ctrlAppl() != (IO_ErrorType)IO_E_OK || !(IO_MemPtrType)pData)  \
{  \
	info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
} else \
{ \
	BCP_SlaveManager__Read_ApplData(ch, (IO_MemPtrType)pData, &info, (BCP_PacketSizeType)maxlen);\
}

/*
Macro generale per scrittura dati applicazione.
*/
#define BCP_SLAVEMANAGER_WRITE_APPLDATA(ctrlAppl, pData, maxlen, pre, post) \
	if((ctrlAppl() != (IO_ErrorType)IO_E_OK) || \
			(!(IO_MemPtrType)pData) || \
			(maxlen == 0) || \
			(BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX] > maxlen) \
			)  \
  {  \
		info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
  } else \
  { \
		pre \
    BCP_SlaveManager__Write_ApplData(ch, (IO_MemPtrType)pData, &info, (BCP_PacketSizeType)maxlen);\
		post  \
  }

/** \defgroup CSM_EE__REQMNG Platform Protocol Extended requests managers SAM and their use
    \ingroup CSM_SAM_EE
    Each request manager has specific parameters.
    \addtogroup CSM_EE__REQMNG
    @{
*/

/**
\hideinitializer
Non volatile memory (flash) reading.
Works correctly if socified a ram address.
*/
#define BCP_SLAVEMANAGER__EE_READ_FLASH() \
case PP_EE_TYPE_READ_FLASH: \
{ \
  PP_AddressType start; \
  PP_AddressType end; \
  start.data= (BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+1]<<8) | BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX]; \
  end.data= (BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+3]<<8) | BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+2]; \
	if (end.data>=start.data)  \
	{ \
	  IO_BufferSizeType datalen= end.data-start.data; \
		if(datalen<=255)	\
		{	\
		  info.len= datalen+1;  \
			info.address= (IO_MemPtrType)(start.data);  \
			if(info.address>=(IO_MemPtrType)REMAP_ADDRESS1)\
			{	\
				info.address+=REMAP_ADDRESS1_OFFSET;\
			}	\
			info.error = (IO_ErrorType)IO_E_OK; \
			info.option|=PP_SL_RESP_ANSW_OPT; \
		}	\
		else\
		{\
		    info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}\
	} \
} \
break;


/**
\hideinitializer
Non volatile memory (flash) reading.
Works correctly if socified a ram address.
*/

#define CP_CSM_READ_FLASH__POS_REMAP(dat_pos_, PP_RemapTable_)\
{ \
    PP_AddressType start; \
    PP_AddressType end; \
    unsigned long offset=0;\
    start.data=  (uint16_t) ((pRxBuff[(uint8_t)(dat_pos_)+1]<<8) | pRxBuff[(uint8_t)(dat_pos_)]); \
    end.data=  (uint16_t) ((pRxBuff[(uint8_t)(dat_pos_)+3]<<8) | pRxBuff[(uint8_t)(dat_pos_)+2]); \
	if (end.data>=start.data)  \
	{ \
		IO_BufferSizeType datalen=  (uint16_t) (end.data-start.data); \
		if(datalen<=255)	\
		{	\
			info.len=  (uint16_t) (datalen+1);  \
			info.address= (IO_MemPtrType)((unsigned int)start.data);  \
		    if(PP_MappingGetOffset((IO_MemPtrType)((unsigned int)start.data), (IO_MemPtrType)((unsigned int)end.data), PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType), &offset)>=0)\
		    {\
		        info.address += offset;\
			    info.error = (IO_ErrorType)IO_E_OK; \
			    info.option |=PP_SL_RESP_ANSW_OPT; \
		    }\
		    else\
		    {\
		        info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		    }\
		}	\
		else\
		{\
		    info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}\
	} \
}

/* from 0 to 3 because Native format is LE */
typedef struct{
	unsigned int addr0	:8;
	unsigned int addr1	:8;
	unsigned int addr2	:8;
	unsigned int addr3	:8;
	unsigned int len0	:8;
}ExtendedReadingMemoryInfoType;

/* from 0 to 3 because Native format is LE */
typedef struct{
	uint8_t addr0		:8;
	uint8_t addr1		:8;
	uint8_t addr2		:8;
	uint8_t addr3		:8;
	uint8_t datatowrite	:8;
}ExtendedWriteRamMemoryInfoType;

/* from 0 to 3 because Native format is LE */
typedef struct{
	uint8_t addr0	:8;
	uint8_t addr1	:8;
	uint8_t addr2	:8;
	uint8_t addr3	:8;
}ExtendedErasingMemoryInfoType;

/* from 0 to 3 because Native format is LE */
typedef struct{
	uint8_t	res0;
	uint8_t addr0	;
	uint8_t addr1	;
	uint8_t addr2	;
	uint8_t addr3	;
}ExtendedErasingMemoryAnsInfoType;


#define CP_CSM_EXTENDEDREADING_MEMORY(dat_pos_, PP_RemapTable_)\
	if((5==(pRxBuff[(uint8_t)(dat_pos_-1)]))) /*validate length of the request*/	\
	{\
		IO_LenType readinglen=0;\
		uint32_t decodedaddress=0;\
		decodedaddress = MBGET_4((ExtendedReadingMemoryInfoType*)&pRxBuff[(uint8_t)(dat_pos_)], addr);\
		readinglen=(IO_LenType)MBGET_1((ExtendedReadingMemoryInfoType*)&pRxBuff[(uint8_t)(dat_pos_)], len);\
		if(!readinglen)\
		{\
			readinglen=256;\
		}\
		if(0 <= PP_IsAccessAllowed(PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_MemoryRangeType), (IO_MemPtrType)decodedaddress, readinglen))\
		{\
			info.len=  readinglen;\
			info.address = (IO_MemPtrType)decodedaddress;\
			info.error = (IO_ErrorType)IO_E_OK;\
			info.option |=PP_SL_RESP_ANSW_OPT;\
		}\
		else\
		{\
			info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}\
	}\
	else\
	{\
		info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
	}


#define CP_CSM_EXTENDEDWRITING_RAMMEMORY(dat_pos_, PP_RemapTable_)\
		if((sizeof(ExtendedWriteRamMemoryInfoType)<=(pRxBuff[(uint8_t)(dat_pos_-1)]))) /*validate length of the request*/	\
		{\
			uint32_t decodedaddress=0;\
			decodedaddress = MBGET_4((ExtendedWriteRamMemoryInfoType*)&pRxBuff[(uint8_t)(dat_pos_)], addr);\
			if(0 <= PP_IsAccessAllowed(PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_MemoryRangeType), (IO_MemPtrType)decodedaddress, (FLS_sizeType)pRxBuff[(dat_pos_) -1]-sizeof(ExtendedErasingMemoryInfoType)))\
			{\
				info.error = (IO_ErrorType)IO_E_OK;\
				(void)memcpy((void*)decodedaddress, (FLS_SourcePtrType)&pRxBuff[(dat_pos_)+sizeof(ExtendedErasingMemoryInfoType)] , (FLS_sizeType)pRxBuff[(dat_pos_) -1]-sizeof(ExtendedErasingMemoryInfoType)); \
			}\
			else\
			{\
				info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
			}\
		}\
		else\
		{\
			info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}



#define CP_CSM_EXTENDEDERASING_FLSMEMORY(dat_pos_, PP_RemapTable_)\
		if((sizeof(ExtendedErasingMemoryInfoType)==(pRxBuff[(uint8_t)(dat_pos_-1)]))) /*validate length of the request*/	\
		{\
			uint8_t res=0;\
			uint32_t decodedaddress=0;\
			decodedaddress = MBGET_4((ExtendedErasingMemoryInfoType*)&pRxBuff[(uint8_t)(dat_pos_)], addr);\
			if(0 <= PP_IsAccessAllowed(PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_MemoryRangeType), (IO_MemPtrType)decodedaddress, 1))\
			{\
				info.address = (IO_MemPtrType)decodedaddress;\
				info.error = (IO_ErrorType)IO_E_OK;\
				info.option |=PP_SL_RESP_ANSW_OPT;\
				res = (uint8_t) ((IO_ErrorType)(IO_E_OK == OCFLS_REraseSync ((FLS_sizeType)1, decodedaddress)) ? 0 : 1);\
				info.len = sizeof(ExtendedErasingMemoryAnsInfoType);\
				info.address=(IO_MemPtrType)temp;\
				MBSET(1,(ExtendedErasingMemoryAnsInfoType*)&temp[0],res, res);\
				MBSET(4,(ExtendedErasingMemoryAnsInfoType*)&temp[0],addr, decodedaddress);\
			}\
			else\
			{\
				info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
			}\
		}\
		else\
		{\
			info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}

#define CP_CSM_EXTENDEDWRITING_FLSMEMORY(dat_pos_, PP_RemapTable_)\
		if((pRxBuff[(uint8_t)(dat_pos_-1)]==0)||(sizeof(ExtendedErasingMemoryInfoType)<(pRxBuff[(uint8_t)(dat_pos_-1)]))) /*validate length of the request*/	\
		{\
			uint8_t res=0;\
			uint32_t decodedaddress=0;\
			decodedaddress = MBGET_4((ExtendedErasingMemoryInfoType*)&pRxBuff[(uint8_t)(dat_pos_)], addr);\
			if(0 <= PP_IsAccessAllowed(PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_MemoryRangeType), (IO_MemPtrType)decodedaddress, (FLS_sizeType)pRxBuff[(dat_pos_) -1]-sizeof(ExtendedErasingMemoryInfoType)))\
			{\
				info.address = (IO_MemPtrType)decodedaddress;\
				info.error = (IO_ErrorType)IO_E_OK;\
				info.option |=PP_SL_RESP_ANSW_OPT;\
				res = (uint8_t) ((IO_ErrorType)(IO_E_OK == OCFLS_RWriteSync((FLS_SourcePtrType)&pRxBuff[(dat_pos_)+sizeof(ExtendedErasingMemoryInfoType)] , (FLS_sizeType)pRxBuff[(dat_pos_) -1]-sizeof(ExtendedErasingMemoryInfoType), decodedaddress)) ? 0 : 1);\
				info.len = sizeof(ExtendedErasingMemoryAnsInfoType);\
				info.address=(IO_MemPtrType)temp;\
				MBSET(1,(ExtendedErasingMemoryAnsInfoType*)&temp[0],res, res);\
				MBSET(4,(ExtendedErasingMemoryAnsInfoType*)&temp[0],addr, decodedaddress);\
			}\
			else\
			{\
				info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
			}\
		}\
		else\
		{\
			info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}




/**
\hideinitializer

Erase non volatile memory (flash) page. The amuont of memory erased is terget dependant.
\param [in] WriteEraseMacro Macro to use to do the operation

\note For Platform 1.0-based projects see \ref CSM_EE__SUPP_PLATFORM1
*/
#define BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH(WriteEraseMacro) \
case PP_EE_TYPE_WRITE_FLASH:\
case PP_EE_TYPE_ERASE_FLASH_PAGE: \
{ \
	WriteEraseMacro \
} \
break;


/**
  \hideinitializer
write ram
\note areas that can be written are target dependant
*/
#define BCP_SLAVEMANAGER__EE_WRITE_RAM() \
case PP_EE_TYPE_WRITE_RAM: \
{ \
    PP_AddressType 	address; \
	IO_MemPtrType		addr;\
    address.data = ((uint16_t)BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+1]<<8) | BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX]; \
    if(address.data >= (uint16_t)PP_END_OF_UNTOUCHABLE_RAM)  \
    { \
    	addr=(IO_MemPtrType)address.data;\
    	if(addr>=(IO_MemPtrType)REMAP_ADDRESS1)\
    	{\
    		addr+=REMAP_ADDRESS1_OFFSET;\
    	}\
    info.error = (IO_ErrorType)IO_E_OK; \
    (void)memcpy((void*)addr, &BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+2], BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_LEN_INDEX]-2); \
    }else \
    { \
    } \
} \
break;

/**
  \hideinitializer
write ram
\note areas that can be written are target dependant
*/

#define CP_CSM_WRITE_RAM__POS_REMAP(dat_pos_, PP_RemapTable_)\
{ \
    uint16_t len = (uint16_t) (pRxBuff[(uint8_t)(dat_pos_)-1]-2);\
    unsigned long offset;   \
    PP_AddressType address; \
    PP_AddressType end; \
    int8_t remap = 0;\
	address.data =  (uint16_t) ((pRxBuff[(uint8_t)(dat_pos_)+1]<<8) | pRxBuff[(uint8_t)(dat_pos_)]);\
	end.data =  (uint16_t) (address.data + len -1);\
	if(end.data >= address.data)\
	{\
        remap = PP_MappingGetOffset((IO_MemPtrType)((unsigned int)address.data), (IO_MemPtrType)((unsigned int)end.data), PP_RemapTable_, (sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType)), &offset);\
        if(remap>=0)\
    	{\
    	    (void)memcpy((void*)(address.data + offset), &pRxBuff[(uint8_t)(dat_pos_)+2], len); \
    	}\
    	else\
	    {\
	        info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
	    }\
	}\
}

#define BCP_SLAVEMANAGER__EE_WRITE_RAM_REMAP(PP_RemapTable) \
case PP_EE_TYPE_WRITE_RAM: \
{ \
    PP_AddressType 	address; \
    IO_MemPtrType		addr;\
    int8_t remap= sizeof(PP_RemapTable)/sizeof(PP_RemapTableType)-1;\
    uint16_t len = BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_LEN_INDEX]-2;\
    address.data = ((uint16_t)BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+1]<<8) | BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX]; \
	do\
	{\
	    addr =(IO_MemPtrType)address.data;\
        if((address.data >= PP_RemapTable[remap].start) && ((address.data + len -1) <= PP_RemapTable[remap].end))\
        {\
            addr += PP_RemapTable[remap].offset;\
            break;\
        }\
	}while(--remap>=0);\
	if(remap>=0)\
	{\
	    (void)memcpy((void*)addr, &BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_DAT_INDEX+2], BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_LEN_INDEX]-2); \
	}\
} \
break;

/**
  \hideinitializer
application data reading.

\param [in] eeType hexadecimal code of the frame.
\param [in] ctrlAppl Application defined function to control the request. The function must be defined trought #DEF_CSM_EE_CTRL.
\param [in] pData pointer to data
\param [in] maxlen maximum size in byte of the data

\par Behaviour
The request fails (nack transmitted) if:
\li the returned value by ctrlAppl() is different from IO_E_OK
\li pData is NULL
*/
#define BCP_SLAVEMANAGER__EE_READ_APPLDATA(eeType, ctrlAppl, pData, maxlen) \
case eeType:\
  BCP_SLAVEMANAGER__READ_APPLDATA( ctrlAppl, pData, maxlen)\
break;




/**
  \hideinitializer
application data reading (extended version).

\param [in] eeType hexadecimal code of the frame.
\param [in] ctrlAppl Application defined function to control the request. The function must be defined trought #DEF_CSM_EE_CTRL.
\param [in] infoAppl Application defined function to provide info about buffer and its dimension. The function must be defined trought #DEF_CSM_EE_BUFF.
*/
#define BCP_SLAVEMANAGER__EE_READ_APPLDATA_EX(eeType, ctrlAppl, infoAppl) \
case eeType:\
{\
  IO_BufferInfoType_t bufferinfo={0};\
  bufferinfo = infoAppl(&BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX]); \
  BCP_SLAVEMANAGER__READ_APPLDATA( ctrlAppl, bufferinfo.pData, bufferinfo.size)\
}\
break;




/**
  \hideinitializer
Application data writing.

\param [in] eeType hexadecimal code of the frame.
\param [in] ctrlAppl Application defined function to control the request. The function must be defined trought #DEF_CSM_EE_CTRL.
\param [in] pData pointer to data
\param [in] maxlen maximum size in bytes of data
\param [in] pre application defined macro executed before the operation
\param [in] post application defined macro executed before the operation

\par Behaviour
The request fails (nack transmitted) if:
\li pData is NULL;
\li maxlen = 0 ;
\li data size > maxlen;
\li the returned value by ctrlAppl() is different from IO_E_OK
*/
#define BCP_SLAVEMANAGER__EE_WRITE_APPLDATA(eeType, ctrlAppl, pData, maxlen, pre, post) \
case eeType:\
  BCP_SLAVEMANAGER_WRITE_APPLDATA(ctrlAppl, pData, maxlen, pre, post)\
break;


/**
  \hideinitializer
application data writing (extended version).

\param [in] eeType hexadecimal code of the frame.
\param [in] ctrlAppl Application defined function to control the request. The function must be defined trought #DEF_CSM_EE_CTRL.
\param [in] infoAppl Application defined function to provide info about buffer and its dimension. The function must be defined trought #DEF_CSM_EE_BUFF.
\param [in] pre application defined macro executed before the operation
\param [in] post application defined macro executed before the operation
*/
#define BCP_SLAVEMANAGER__EE_WRITE_APPLDATA_EX(eeType, ctrlAppl, infoAppl, pre, post) \
case eeType : \
{ \
  IO_BufferInfoType_t bufferinfo={0};\
  bufferinfo = infoAppl(&BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX]); \
  BCP_SLAVEMANAGER_WRITE_APPLDATA(ctrlAppl, bufferinfo.pData, bufferinfo.size, pre, post)\
} \
break;



#define APPL_DEFINED_FUNC_ARGS(x,y) &BCP_ChannelTable[ch].rxBuffer[x], BCP_ChannelTable[ch].rxBuffer[y], &info


/**
  \hideinitializer
Application defined manager
\param [in]   eeType hexadecimal code of the frame.
\param [in]   fnc_user Application defined function processing the frame (defined throught #DEF_CSM_EE_CUSTOM_FUNC).

The  prototipe is:
\code
void fnc_user(uint8_t* pdata, uint8_t len, PP_SlaveBuilderInfoType * pinfo);
\endcode
where
\li \e pdata pointer to data
\li \e len size in byte of data
\li \e pinfo pointer to a Platform Protocol structure containing info for building the answer frame.
If the function returns a value different from IO_E_OK a NACK is generated.
*/

#define BCP_SLAVEMANAGER__EE_TYP16_APPL_DEFINED(eeType, fnc_user)  \
case eeType: \
  fnc_user(ch, APPL_DEFINED_FUNC_ARGS(PP_GDAT_INDEX, PP_GLEN_INDEX));\
break;

#define BCP_SLAVEMANAGER__EE_APPL_DEFINED(eeType, fnc_user)  \
case eeType: \
  fnc_user(ch, &BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX], BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX], &info);\
break;


/*  @} addtogroup CSM_EE__REQMNG*/




/**
    \defgroup CSM_EE__SUPP_PLATFORM1 Platform 1.0-based projects SAM extensions
    \ingroup CSM_SAM
    \addtogroup CSM_EE__SUPP_PLATFORM1
    @{
*/

/**
  \hideinitializer
  Write and erase flash extension for Platform 1.0-based projects to be used with BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH request manager.

  Use FLASH_WRITE_PLT1 as shown in the following example
  \code
  #include "SkeletonGlobal.h"
  #include "FlashModule.h"

  	...

  BEGIN_BCP_SLAVEMANAGER__EE()
      ...
  		BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH(FLASH_WRITE_ERASE_FLASH_PLT1())
  		...
  END_BCP_SLAVEMANAGER__EE()
  \endcode
*/

#define FLASH_WRITE_ERASE_FLASH_PLT1()\
{\
			static uint8_t answerdata[3];\
			static uint8_t state_flashcmd=0;\
			uint8_t* pdata =&BCP_ChannelTable[ch].rxBuffer[PP_DAT_INDEX];\
			if(state_flashcmd==0)\
			{\
				static	PP_AddressType start;\
				\
				uint8_t len=BCP_ChannelTable[ch].rxBuffer[PP_LEN_INDEX];\
				\
				start.data = *(pdata);\
				start.data |= ( ((uint16_t) *(pdata+1)) << 8 );\
				\
				info.error = (IO_ErrorType)IO_E_WORKING;\
				state_flashcmd++;\
				FlashData.Type=BCP_ChannelTable[ch].rxBuffer[PP_DTYP_INDEX];\
				FlashData.DataPtr=(Flash_Data_Struct_DataPtr)(pdata+2);\
				FlashData.Len=len-2;\
				if(ch==(uint8_t)BCP_I2C_CH_ID)\
				{\
					FlashData.Event=EVENT_I2C_PROTOCOL;\
				}\
				FlashData.TaskId=Task_Protocol;\
				FlashData.Address=(Flash_Data_Struct_Address)start.data;/*#if defined(BUILD_RENESAS_36079)*/\
				FlashData.EsitAddr=(Flash_Data_Struct_EsitAddr)&answerdata[0];/*#endif*/\
        \
				FlashData.NotificationEn=0x01;\
				(void)ActivateTask(Task_Flash);\
			}\
			else\
			{\
				\
				answerdata[1]=*(pdata);\
				answerdata[2]=*(pdata+1);\
				info.len= 3;\
				info.address= (IO_MemPtrType)(answerdata);\
				info.error = (IO_ErrorType)IO_E_OK;\
				info.option|=(BCP_OptionType)PP_SL_RESP_ANSW_OPT;\
				state_flashcmd=0;\
			}\
}


/*  @}  addtgroup CSM_EE__SUPP_PLATFORM1 */



/*
Application data reader.
*/
#define CP_GP_PARAM__READ_APPLDATA(ctrlAppl, pData, maxlen)\
if(ctrlAppl() != (IO_ErrorType)IO_E_OK || !(IO_MemPtrType)pData)  \
{  \
	PP_EncodeHdrGPAnsParam(pProtReq, PP_NACK);\
} else \
{ \
	uint8_t len= PP_LEN_FROM_GP_PARAM(pProtReq->pDataReq);\
	if(len >= maxlen)\
	{\
		len = maxlen;\
	}\
	PP_EncodeHdrGPAnsParam(pProtReq, PP_ACK);\
	PP_EncodeDataGPAnsParam(pProtReq, len, (IO_MemPtrType)pData);\
	PP_SetMasterError(ch, (IO_ErrorType)IO_E_OK);\
}



#define MAKEWORD(h, l)     				((uint16_t)(((uint8_t)((uint16_t)(l) & 0xFF)) | ((uint16_t)((uint8_t)((uint16_t)(h) & 0xFF))) << 8))

#define PP_pntGP_PARAM_GET_WE_ADDRESS(x)           (uint16_t)((uint16_t)((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[5]*256+((CP_GP_ParamType*)(x))->ZBParam.PL.PLByte[4])

#define PP_GET_FLS_ADDRESS(pData_, pProtParam_)           			(uint16_t)((uint16_t)((uint8_t)(pData_[pProtParam_->pos.item[5]+1]))*256 + ((uint8_t)(pData_[pProtParam_->pos.item[5]])))



                           

#define BEGIN_CP_SLAVEMANAGER__BS_REQ_FILTER_POS(pos)  \
	switch (*(pRxBuff + (pos)) &PP_BS_TYPE_MSK) \
    {

#define BEGIN_CP_SLAVEMANAGER__REQ_FILTER_POS(pos)  \
	switch (*(pRxBuff + (pos))) \
    {


#define END_CP_SLAVEMANAGER__REQ_FILTER()  \
        default:\
            info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
    }

#define CP_SLAVEMANAGER_REQ_IS_EXTENDED()                       CP_SLAVEMANAGER_RXBUFF_ASSERT_EXTENDED_REQ_POS(1)
#define CP_SLAVEMANAGER_REQ_IS_TYP8()                           CP_SLAVEMANAGER_RXBUFF_ASSERT_TYP8_REQ_POS(2)

#define BEGIN_CP_SLAVEMANAGER__REQ_FILTER_EETYP8_TYPE()         BEGIN_CP_SLAVEMANAGER__REQ_FILTER_POS(3)
#define BEGIN_CP_SLAVEMANAGER__REQ_FILTER_BSTYP8_TYPE()         BEGIN_CP_SLAVEMANAGER__BS_REQ_FILTER_POS(1)


#define CP_CSM_EETYP8_CUSTUMFUNC( userfunc)         CP_CSM_CUSTUMFUNC_POS(5/*data pos*/, pRxBuff[(uint8_t)(5)-1]/*len*/, userfunc)


#define CP_CSM_EETYP8_READFLASH( PP_RemapTable_)    CP_CSM_READ_FLASH__POS_REMAP(5, PP_RemapTable_)

#define CP_CSM_EETYP8_WRITERAM( PP_RemapTable_)     CP_CSM_WRITE_RAM__POS_REMAP(5, PP_RemapTable_)

#define CP_CSM_EETYP8_ERASEFLASH( PP_RemapTable_)   CP_CSM_ERASE_FLASH__POS_REMAP(5, PP_RemapTable_)

#define CP_CSM_EETYP8_WRITEFLASH( PP_RemapTable_)   CP_CSM_WRITE_FLASH__POS_REMAP(5, PP_RemapTable_)

#define PP_TO_UINT16_DECODE_ADDRESS(pData_, pos)    (uint16_t)((uint16_t)((uint8_t)(pData_[(pos)+1]))*256 + ((uint8_t)(pData_[(pos)])))

#define CP_CSM_EETYP8_EXTENDEDMEMORYREADING( PP_AccessTable_)    CP_CSM_EXTENDEDREADING_MEMORY(5, PP_AccessTable_)

#define CP_CSM_EETYP8_EXTENDEDRAMMEMORYWRITING( PP_AccessTable_)    CP_CSM_EXTENDEDWRITING_RAMMEMORY(5, PP_AccessTable_)

#define CP_CSM_EETYP8_EXTENDEDFLSMEMORYERASING( PP_AccessTable_)    CP_CSM_EXTENDEDERASING_FLSMEMORY(5, PP_AccessTable_)

#define CP_CSM_EETYP8_EXTENDEDFLSMEMORYWRITING( PP_AccessTable_)    CP_CSM_EXTENDEDWRITING_FLSMEMORY(5, PP_AccessTable_)

/**
  \hideinitializer
Begins .... .

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

*/

#define BEGIN_CP_SLAVEMANAGER__REQ_TYPE_FILTER()  \
	switch (*(pRxBuff + pProtParam->pos.item[3])) \
    {

/**
  \hideinitializer
Ends .... .
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti

\note case default : non gestito
*/
#define END_CP_SLAVEMANAGER__REQ_TYPE_FILTER() \
	}


#define CP_CSM_READ_FLASH()\
{ \
  PP_AddressType start; \
  PP_AddressType end; \
  start.data= (pRxBuff[(uint8_t)pProtParam->pos.item[5]+1]<<8) | pRxBuff[(uint8_t)pProtParam->pos.item[5]]; \
  end.data= (pRxBuff[(uint8_t)pProtParam->pos.item[5]+3]<<8) | pRxBuff[(uint8_t)pProtParam->pos.item[5]+2]; \
	if (end.data>=start.data)  \
	{ \
	  IO_BufferSizeType datalen= end.data-start.data; \
		if(datalen<=255)	\
		{	\
			info.len= datalen+1;  \
			info.address= (IO_MemPtrType)(start.data);  \
			if(info.address>=(IO_MemPtrType)REMAP_ADDRESS1)\
			{	\
				info.address+=REMAP_ADDRESS1_OFFSET;\
			}	\
			info.error = (IO_ErrorType)IO_E_OK; \
			info.option|=PP_SL_RESP_ANSW_OPT; \
		}	\
	} \
}

#define CP_CSM_READ_FLASH_CB(callback)\
{ \
  PP_AddressType start; \
  PP_AddressType end; \
  start.data= (pRxBuff[(uint8_t)pProtParam->pos.item[5]+1]<<8) | pRxBuff[(uint8_t)pProtParam->pos.item[5]]; \
  end.data= (pRxBuff[(uint8_t)pProtParam->pos.item[5]+3]<<8) | pRxBuff[(uint8_t)pProtParam->pos.item[5]+2]; \
	if (end.data>=start.data)  \
	{ \
		IO_BufferSizeType datalen= end.data-start.data; \
		if(datalen<=255 && (IO_E_OK == callback(&start, &end, &info)))	\
		{	\
			info.len= datalen+1;  \
			info.address= (IO_MemPtrType)(start.data);  \
			if(info.address>=(IO_MemPtrType)REMAP_ADDRESS1)\
			{	\
				info.address+=REMAP_ADDRESS1_OFFSET;\
			}	\
			info.error = (IO_ErrorType)IO_E_OK; \
			info.option|=PP_SL_RESP_ANSW_OPT; \
		}	\
		else\
		{\
		    info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}\
	} \
}


#define CP_CSM_CUSTUMFUNC(userfunc)\
userfunc(ch, &pRxBuff[(uint8_t)pProtParam->pos.item[5]], pRxBuff[(uint8_t)pProtParam->pos.item[4]], &info);

#define CP_CSM_CUSTUMFUNC_POS(data_pos, len, userfunc)\
userfunc(ch, &pRxBuff[(uint8_t)(data_pos)/*data*/], (len), &info);

                       
                       

#define CP_CSM_EETYP8_READ_APPLDATA(ctrlAppl, pData, maxlen, pre, post)    CP_CSM_READ_APPLDATA(ctrlAppl, pData, maxlen, pre, post, 5)
#define CP_CSM_EETYP8_WRITE_APPLDATA(ctrlAppl, pData, maxlen, pre, post)    CP_CSM_WRITE_APPLDATA(ctrlAppl, pData, maxlen, pre, post, 4)
                       
                       
/*pos = pProtParam->pos.item[5]*/                       
#define CP_CSM_READ_APPLDATA(ctrlAppl, pData, maxlen, pre, post, pos)\
if(ctrlAppl() != (IO_ErrorType)IO_E_OK				\
	|| !(IO_MemPtrType)(pData) || (!(maxlen)))  	\
{  													\
	info.error = (IO_ErrorType)IO_E_INVALID_VALUE;	\
} else \
{ \
	pre \
	CP_SlaveManager__Read_ApplData((pos), pRxBuff, (IO_MemPtrType)(pData), &info, (maxlen));\
	post \
}

/*
Macro generale per scrittura dati applicazione.
*/
#define CP_CSM_WRITE_APPLDATA(ctrlAppl, pData, maxlen, pre, post, pos) \
	if((ctrlAppl() != (IO_ErrorType)IO_E_OK) || 		\
			(!(IO_MemPtrType)(pData)) || 					\
			((maxlen) == 0) || 							\
			(pRxBuff[(pos)] > (maxlen)) \
			)  \
	{  \
		info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
	} else \
	{ \
		pre \
		CP_SlaveManager__Write_ApplData((pos), pRxBuff, (IO_MemPtrType)pData, &info, (BCP_PacketSizeType)maxlen);\
		post  \
	}


/**
  \hideinitializer
write ram
\note areas that can be written are target dependant
*/
#define CP_CSM_WRITE_RAM() \
{ \
  PP_AddressType 	address; \
  IO_MemPtrType		addr;\
  address.data = ((uint16_t)pRxBuff[(uint8_t)pProtParam->pos.item[5]+1]<<8) | pRxBuff[(uint8_t)pProtParam->pos.item[5]]; \
  if(address.data >= (uint16_t)PP_END_OF_UNTOUCHABLE_RAM)  \
  { \
		addr=(IO_MemPtrType)address.data;\
		if(addr>=(IO_MemPtrType)REMAP_ADDRESS1)\
		{\
			addr+=REMAP_ADDRESS1_OFFSET;\
		}\
    info.error = (IO_ErrorType)IO_E_OK; \
    (void)memcpy((void*)addr, &pRxBuff[(uint8_t)pProtParam->pos.item[5]+2], pRxBuff[(uint8_t)pProtParam->pos.item[4]]-2); \
  }else \
  { \
  } \
}

/**
  \hideinitializer
  Erase flash

  Use FLASH_ERASE as shown in the following example
  \code
  #include "SkeletonGlobal.h"
  #include "FlashModule.h"

  	...

  BEGIN_BCP_SLAVEMANAGER__EE()
      ...
  		BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH(FLASH_WRITE_ERASE_FLASH_PLT1())
  		...
  END_BCP_SLAVEMANAGER__EE()
  \endcode
*/
#define FLASH_ERASE()\
{\
	FLS_positionType addr = PP_GET_FLS_ADDRESS(pRxBuff, pProtParam);\
	temp[0] = OCFLS_REraseSync((FLS_sizeType)1, addr);\
	temp[1] = (uint8_t)addr;\
	temp[2] = (uint8_t)(addr/256);\
	info.len = 3;\
	info.address=(IO_MemPtrType)temp;\
	info.option|=PP_SL_RESP_ANSW_OPT;\
}


#define CP_CSM_ERASE_FLASH__POS_REMAP(dat_pos_, PP_RemapTable_)\
{ \
    FLS_positionType addr = PP_TO_UINT16_DECODE_ADDRESS(pRxBuff, dat_pos_);\
    FLS_positionType offset=0;\
	{	\
	    if(PP_MappingGetOffset((IO_MemPtrType)addr, (IO_MemPtrType)addr, PP_RemapTable_, (sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType)), &offset)>=0)\
	    {\
	    	temp[1] = (uint8_t) addr;\
            temp[2] = (uint8_t) (addr/256);\
    	    addr += offset;\
    	    temp[0] = (uint8_t) ((IO_ErrorType)(IO_E_OK == OCFLS_REraseSync ((FLS_sizeType)1, addr)) ? 0 : 1);\
    		info.error = (IO_ErrorType)IO_E_OK; \
    		info.option|=PP_SL_RESP_ANSW_OPT; \
    		info.len = 3;\
        	info.address=(IO_MemPtrType)temp;\
	    }\
	    else\
	    {\
	        info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
	    }\
	}	\
}



/**
  \hideinitializer
  Write flash

  Use FLASH_WRITE as shown in the following example
  \code
  #include "SkeletonGlobal.h"
  #include "FlashModule.h"

  	...

  BEGIN_BCP_SLAVEMANAGER__EE()
      ...
  		BCP_SLAVEMANAGER__EE_WRITE_ERASE_FLASH(FLASH_WRITE_ERASE_FLASH_PLT1())
  		...
  END_BCP_SLAVEMANAGER__EE()
  \endcode
*/
#define FLASH_WRITE()\
{\
	FLS_positionType addr = PP_GET_FLS_ADDRESS(pRxBuff, pProtParam);\
	temp[0] = OCFLS_RWriteSync((FLS_SourcePtrType)&pRxBuff[pProtParam->pos.item[5]+2] , (FLS_sizeType)pRxBuff[pProtParam->pos.item[4]]-2, addr);\
	temp[1] = (uint8_t)addr;\
	temp[2] = (uint8_t)(addr/256);\
	info.len = 3;\
	info.address=(IO_MemPtrType)temp;\
	info.option|=PP_SL_RESP_ANSW_OPT;\
}


#define CP_CSM_WRITE_FLASH__POS_REMAP(dat_pos_, PP_RemapTable_)\
{ \
    FLS_positionType addr = PP_TO_UINT16_DECODE_ADDRESS(pRxBuff, dat_pos_);\
    unsigned long offset=0;\
    uint16_t len = (uint16_t) (pRxBuff[(uint8_t)(dat_pos_)-1]-2);\
    FLS_positionType end; \
    end =  (uint16_t) (addr + len -1);\
    if(end >= addr)\
	{	\
	    if(PP_MappingGetOffset((IO_MemPtrType)addr, (IO_MemPtrType)end, PP_RemapTable_, (sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType)), &offset)>=0)\
	    {\
	    	temp[1] = (uint8_t)addr;\
	    	temp[2] = (uint8_t)(addr/256);\
    	    addr += offset;\
    	    temp[0] = OCFLS_RWriteSync((FLS_SourcePtrType)&pRxBuff[(dat_pos_)+2] , (FLS_sizeType)pRxBuff[(dat_pos_) -1]-2, addr);\
    		info.option|=PP_SL_RESP_ANSW_OPT; \
    		info.len = 3;\
        	info.address=(IO_MemPtrType)temp;\
	    }\
	    else\
	    {\
	        info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
	    }\
	}	\
}


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef struct 
{
  unsigned long offset;
  unsigned short start;
  unsigned short end;
}PP_RemapTableType;


typedef struct
{
  unsigned long start;
  unsigned long end;
}PP_MemoryRangeType;

/**
\addtogroup PPS
@{*/

/*@}*/

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */



#endif /* __PLATFORMPROTOCOL_EE_H */

/* *********** */
/* End of file */
/* *********** */
