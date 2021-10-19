/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\defgroup	CSM_SAM_BS  Slave Answer Manager for Platform Protocol Basic frames
\ingroup CSM_SAM

par Table of Contents
\li     \ref CSM_BS__SECTION
\li     \ref CSM_BS__REQMNG

\par Configuration
\ref csm_sam_cfg_bs   




\page csm_sam_cfg_bs Slave Answer Manager Support for Platform Protocol Basic frames (BS)

\par		Table of content
\li     \subpage csm_bs_section
\li     \ref CSM_BS__REQMNG
*/

/**
\par		Description
This file contains
\li \ref CSM_BS__REQMNG
\li \subpage csm_bs_section
\n
\file		CSM_PlatformProtocol_BS.h
\ingroup	CSM
\date		10/06/2009
\version	00.00.01
\author	Roberto Fioravanti 
\n
\n
\par		History
\n
\n 		00.00.01
\n		09/06/2009 
\n		Roberto Fioravanti 
\n		changes 
\n 		- BCP_SLAVEMANAGER__BS_WRITE_RAM and SLAVEMANAGER__BS_READ_RAM : cast to uint8_t applied to enum's items
\n
\n 		00.00.01
\n		09/06/2009 
\n		Roberto Fioravanti 
\n		changes 
\n 		- creazione
\n
\n etc.
*/


#ifndef __PLATFORMPROTOCOL_BS_H
#define __PLATFORMPROTOCOL_BS_H

/* ********** */
/* Includes */
/* ********* */



/* ****************** */
/* Defines and Macros */
/* ****************** */

/** 
  
  \page csm_bs_section Setting a Basic section in SAM
  
  This page explains how to support Platform Protocol Basic frame requests in SAM.
  
  \par Example
  See the following code:
  \code
  BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
    
    BEGIN_BCP_SLAVEMANAGER__BS()    // <- Begin of Basic section
    
      [...]                         // Basic requests managers area
      
    END_BCP_SLAVEMANAGER__BS()      // <- End of Basic section
    
  END_BCP_SLAVEMANAGER()
  \endcode
  
  For details about Basic Sections see \ref CSM_BS__SECTION
  
    
  \defgroup CSM_BS__SECTION SAM section for Platform Protocol Basic frames
  \ingroup  CSM_SAM_BS
  \addtogroup CSM_BS__SECTION
  @{
*/

/**
\hideinitializer

Begins a basic section in SAM.

\note direct access to the rx buffer of the channel.
\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
\todo completare commento
\todo gestire il default (tipo non riconosciuto?)
*/
#define BEGIN_BCP_SLAVEMANAGER__BS()  \
  if(BCP_ChannelTable[ch].rxBuffer[PP_FTYP_INDEX] != PP_FTYP_EE)  \
	{  \
	  switch (BCP_ChannelTable[ch].rxBuffer[PP_BS_TYP_INDEX]&PP_BS_TYPE_MSK) \
    {
/**
\hideinitializer

Ends a basic section in SAM.

\note unknown frames are ignored.

\version		1.0.0
\date 			05/06/2009
\author			Roberto Fioravanti
*/
#define END_BCP_SLAVEMANAGER__BS() \
    default:  \
	info.error=(IO_ErrorType)IO_E_INVALID_VALUE;\
      ; \
    } \
	}
/*  @}*/	
	

/** 
    \page csm_bs_reqmng Platform Protocol Basic requests managers SAM and their use

    For details about Basic Sections see \ref CSM_BS__SECTION
        
    \defgroup CSM_BS__REQMNG Platform Protocol Basic requests managers SAM and their use
    \ingroup  CSM_SAM_BS
    Each request manager has specific parameters.
    \addtogroup CSM_BS__REQMNG    
    @{
*/ 

/** 
  \hideinitializer

This request manager is customizable by application.
  \param [in] eeType hexadecimal code of the request to be processed.
In the basic section it is possible use this manager with different values in each istance.
  \param [in] fnc_user application defined function that processes the request.
The prototype of this function is
  \code
void fnc_user(uint8_t* data, uint8_t len);
  \endcode
where
  \li \e data is the pointer to the data in the receive buffer
  \li \e len is their size in bytes

  \par Example
The following code shows a basic section in which this request manager is used to process PP_BS_TYPE_LOADS.
  \code
BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
  
  BEGIN_BCP_SLAVEMANAGER__BS()
    BCP_SLAVEMANAGER__BS_APPL_DEFINED(PP_BS_TYPE_LOADS, BS_SetLoads)  //PP_BS_TYPE_LOADS management    
  END_BCP_SLAVEMANAGER__BS()
  
END_BCP_SLAVEMANAGER()
  \endcode
  \n The reason of this choice is due to the fact that driving load depends of the system state unknown to BCP.
  \n Function BS_SetLoads is defined in \e BusCommPltf_cfg.c by application as 
  \code
  void BS_SetLoads(uint8_t* data, uint8_t len)
  {
    if(SystemMode==SYSTEM_NORMAL_MODE)
    {
      [...]
    }
  }
  \endcode
*/
#define BCP_SLAVEMANAGER__BS_APPL_DEFINED(eeType, fnc_user)  \
case eeType: \
  fnc_user(&BCP_ChannelTable[ch].rxBuffer[PP_BS_DAT_INDEX], 3);\
break;



#define CP_CSM_BSTYP8_CUSTUMFUNC( userfunc)         CP_CSM_CUSTUMFUNC_POS(2, 3,userfunc)

/** 
  \hideinitializer
Write ram request manager.
Manages also target-dependent memory displacement (defined at HAL level)
  \par Example
  The following code shows the write ram request manager.
  \code
BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
  
  BEGIN_BCP_SLAVEMANAGER__BS()
    BCP_SLAVEMANAGER__BS_WRITE_RAM()  //write ram management
  END_BCP_SLAVEMANAGER__BS()
  
END_BCP_SLAVEMANAGER()
  \endcode
  
*/
#define BCP_SLAVEMANAGER__BS_WRITE_RAM()  \
case PP_BS_TYPE_WRITE_RAM: \
{ \
  PP_AddressType address={0}; \
	IO_MemPtrType addr;\
  address.data = BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX];\
  address.data |= (uint16_t)((BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_FTYP_INDEX]&0x0F) << 8);\
  address.data |= (uint16_t)((BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX+2]&0x0F) << 12);\
  if(address.data > PP_END_OF_UNTOUCHABLE_RAM)  \
  { \
		addr=(IO_MemPtrType)address.data;\
		if(addr>=(IO_MemPtrType)REMAP_ADDRESS1)\
		{\
			addr+=REMAP_ADDRESS1_OFFSET;\
		}\
    info.error = (IO_ErrorType)IO_E_OK; \
    *(addr) = BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX+1];\
  }else \
  { \
  } \
} \
break;

/** 
  \hideinitializer
  Read ram request manager
\li Manages also target-dependent memory displacement (defined at HAL level)
\li maximum data length: 256 bytes
    \par Example
  The following code shows the read ram request manager.
\code
BEGIN_BCP_SLAVEMANAGER(My_SlaveManager)
  
  BEGIN_BCP_SLAVEMANAGER__BS()
    BCP_SLAVEMANAGER__BS_READ_RAM()  //read ram
  END_BCP_SLAVEMANAGER__BS()
  
END_BCP_SLAVEMANAGER()
\endcode
*/
#define BCP_SLAVEMANAGER__BS_READ_RAM()  \
case PP_BS_TYPE_READ_RAM: \
{ \
    PP_AddressType start={0}; \
    start.data = BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX];  \
    start.data |= (uint16_t)((BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_FTYP_INDEX]&0x0F) << 8);  \
    start.data |= (uint16_t)((BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX+2]&0x0F)<<12); \
	info.len= (uint16_t)((uint8_t)(BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX+1]-BCP_ChannelTable[ch].rxBuffer[(uint8_t)PP_BS_DAT_INDEX]))+1;\
	info.address= (IO_MemPtrType)(start.data);\
	if(start.data>=REMAP_ADDRESS1)\
	{\
	info.address+=REMAP_ADDRESS1_OFFSET; \
	}\
	info.error = (IO_ErrorType)IO_E_OK;\
	info.option|= (BCP_OptionType)PP_SL_RESP_ANSW_OPT; \
}\
break;



#define CP_CSM_BS_READRAM( PP_RemapTable_)     CP_CSM_READ_RAM__BS_POS_REMAP(PP_RemapTable_)



#define CP_CSM_READ_RAM__BS_POS_REMAP(PP_RemapTable_)\
{ \
    PP_AddressType start={0}; \
    unsigned long offset=0;\
    start.data = pRxBuff[(uint8_t)PP_BS_DAT_INDEX];  \
    start.data |= (uint16_t)((pRxBuff[(uint8_t)PP_FTYP_INDEX]&0x0F) << 8);  \
    start.data |= (uint16_t)((pRxBuff[(uint8_t)PP_BS_DAT_INDEX+2]&0x0F)<<12); \
	info.len= (uint16_t)((uint8_t)(pRxBuff[(uint8_t)PP_BS_DAT_INDEX+1]-pRxBuff[(uint8_t)PP_BS_DAT_INDEX]));\
	{ \
		if(info.len<=255)	\
		{	\
			info.address= (IO_MemPtrType)((unsigned int)start.data);  \
		    (void)PP_MappingGetOffset((IO_MemPtrType)((unsigned int)start.data), (IO_MemPtrType)((unsigned int)(start.data+info.len)), PP_RemapTable_, sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType), &offset);\
		    info.len += 1;\
		    info.address += offset;\
			info.error = (IO_ErrorType)IO_E_OK; \
			info.option|=PP_SL_RESP_ANSW_OPT; \
		}	\
		else\
		{\
		    info.error = (IO_ErrorType)IO_E_INVALID_VALUE;\
		}\
	} \
}



#define CP_CSM_BS_WRITERAM( PP_RemapTable_)     CP_CSM_WRITE_RAM__BS_POS_REMAP(PP_RemapTable_)

#define CP_CSM_WRITE_RAM__BS_POS_REMAP(PP_RemapTable_)\
{ \
    unsigned long offset;   \
    PP_AddressType address = {0}; \
    int8_t remap = 0;\
    address.data = pRxBuff[(uint8_t)PP_BS_DAT_INDEX];\
    address.data |= (uint16_t)((pRxBuff[(uint8_t)PP_FTYP_INDEX]&0x0F) << 8);\
    address.data |= (uint16_t)((pRxBuff[(uint8_t)PP_BS_DAT_INDEX+2]&0x0F)<<12);\
    remap = PP_MappingGetOffset((IO_MemPtrType)((unsigned int)address.data), (IO_MemPtrType)((unsigned int)address.data), PP_RemapTable_, (sizeof(PP_RemapTable_)/sizeof(PP_RemapTableType)), &offset);\
    if(remap>=0)\
	{\
        *((uint8_t*)(address.data+ offset)) = pRxBuff[(uint8_t)PP_BS_DAT_INDEX+1];\
	}\
}

/*@}*/
		


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */



#endif /* __PLATFORMPROTOCOL_BS_H */

/* *********** */
/* End of file */
/* *********** */
