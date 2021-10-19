/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Descrizione
\n		Definizione di strutture dati e di parametri per il Communication Manager e Platform Protocol.
\n
\file		BusCommPltf_cfg.h
\ingroup	CP_CFG
\date		22/09/2010
\version	01.00.00 
\author	Roberto Fioravanti
\n
\n
\par		History (titolo)
\n
\n 		00.00.64
\n		22/09/2010
\n		developper Roberto Fioravanti
\n		changes 
\n 		- creation

*/

#ifndef __BUSCOMMPLTF_H
#define __BUSCOMMPLTF_H


#define BCP_COUNTER_OPTION__SAME_COUNTER_FOR_LEV_3_4_5

/* ********** */
/* Includes */
/* ********* */

#include "Hal_Interface.h"
#include HAL_CFG
#include "IO_driver.h"
#include "BCP_General.h"



/* used only by ident table */
#if defined(FREESCALE_MCF51_AG128)
#define START_OF_RAM                                           0x00800000
#define SIZE_OF_VECTORS                                        0x03FC

#define REMAP_READ_RAM_BLOCK                                   0x00800000, 0x0000, 0x3FFF
#define REMAP_READ_ROM_BLOCK                                   0x00010000, 0x4010, 0xFFFF
#define REMAP_READ_BOOT_ROM_BLOCK                              0xFFFFC3F0, 0x4000, 0x400F
#define REMAP_WRITE_RAM_BLOCK                                  0x00800000, 0x03FC, 0x3FFF


#if OS_ISRHANDLERTABLE_STATIC
#define PP_END_OF_UNTOUCHABLE_RAM                         (START_OF_RAM+SIZE_OF_VECTORS)
#else
#define PP_END_OF_UNTOUCHABLE_RAM                         (0x00800000)
#endif
#endif  /* FREESCALE_MCF51_AG128 */

#if defined(NXP_LPC12xx_1227)
#define START_OF_RAM                                           0x10000000
#define SIZE_OF_VECTORS                                        0x00C0

#define REMAP_READ_RAM_BLOCK                                   0x10000000, 0x0000, 0x1FFF
#define REMAP_READ_ROM_BLOCK                                   0x00010000, 0x2000, 0xFFFF
#define REMAP_WRITE_RAM_BLOCK                                  0x10000000, 0x00C0, 0x1FFF


#define PP_END_OF_UNTOUCHABLE_RAM                         (START_OF_RAM+SIZE_OF_VECTORS)

#endif /* defined(NXP_LPC12xx_1227) */

#if defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)
#define START_OF_RAM                                           0x00000000
#define SIZE_OF_VECTORS                                        0x00D0
#define REMAP_READ_RAM_BLOCK                                   0x00000000, 0x0000, 0x3FFF
#define REMAP_READ_DATA_BLOCK                                  BCKP_STAT_OFFSET, BCKP_STAT_VIRTUAL_ADDRESS, 0x5FFF
#define REMAP_READ_ROM_BLOCK                                   SETTING_ADDRESS_OFFSET, 0x6000, 0xEFFF
#define REMAP_WRITE_RAM_BLOCK                                  0x00000000, 0x0000, 0x3FFF
#define REMAP_READ_BOOT_ROM_BLOCK                              0xFFFF0000, 0xF000, 0xFFFF
#endif /* defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0) */





/**
\addtogroup BCP
@{*/


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define ENABLE_SYNC_UI_MAIN     (1)

/**
enumeration of shared data between application and Communication platform
*/
typedef enum 
{
CP_APPLDATA_IDENT       =0,
CP_APPLDATA_EXTENDED_IDENT,
CP_APPLDATA_R_STATUS,
CP_APPLDATA_R_REGS,
CP_APPLDATA_W_REGS,
CP_APPLDATA_W_UIINFO,                           
CP_APPLDATA_NUM                                 /**< number of data */
}CPApplianceDataTableIndexType;

/*Communication Platform general settings */
#define BCP_MASTER_SUPPORT	                    FALSE
#define BCP_SLAVE_SUPPORT		                TRUE

#if !defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
#define BCP_FRAGMENT_SLAVE
#endif

/* Bus Communication Platform */
#define BUS_COM_PLT_I2C_SUPPORT
#define BUS_COM_PLT_I2C_BUF_SHARED_SIZE         (256+8)
#if defined(CONFIG_LIB_EXTERNAL_BOARD_SUPPORT)
#define BUS_COM_PLT_UART_SUPPORT
#endif



#define BCM_SLAVE_ACTION_CB                     ActionComm          /**< CP callback */
#define ENABLE_BCP_EOSN	                        TRUE                /**< End of slave request notification , Enabled*/




#define BUS_COM_PLT_I2C_BUF_SHARED  /* dummy define: unica opzione supportata */

#define BUS_COM_PLT_BUF_SIZE_DEFAULT            (16+8)

#if defined(CONFIG_PM_POWER_PROFILE_SUPPORT)
#define BUS_COM_PLT_BUF_SIZE_UART			    (256+8) /*7(fixed)+1(seq id)+(16+2)info+(24+2)ram/ident*/
#else
#define BUS_COM_PLT_BUF_SIZE_UART			    (80)/*7(fixed)+1(seq id)+(36+2)info+(32+2)ram/ident*/
#endif

#define BCP_UART_RX_BUF_SIZE					BUS_COM_PLT_BUF_SIZE_UART
#define BCP_UART_TX_BUF_SIZE					BUS_COM_PLT_BUF_SIZE_UART

#ifndef BUS_COM_PLT_I2C_BUF_SHARED
#error "BUS_COM_PLT_I2C_BUF_SHARED non definita. Altre opzioni non sono supportate."
#endif /* BUS_COM_PLT_I2C_BUF_SHARED */

#ifndef BUS_COM_PLT_I2C_BUF_SHARED_SIZE
#error "BUS_COM_PLT_I2C_BUF_SHARED_SIZE non definita. Altre opzioni non sono supportate."
#define BCP_I2C_TX_BUF_SIZE     BUS_COM_PLT_BUF_SIZE_DEFAULT
#define BCP_I2C_RX_BUF_SIZE     BCP_I2C_TX_BUF_SIZE
#endif /* BUS_COM_PLT_I2C_BUF_SHARED_SIZE */

#if BUS_COM_PLT_I2C_BUF_SHARED_SIZE==0
#define BCP_I2C_TX_BUF_SIZE BUS_COM_PLT_BUF_SIZE_DEFAULT
#else
#define BCP_I2C_TX_BUF_SIZE     BUS_COM_PLT_I2C_BUF_SHARED_SIZE
#define BCP_I2C_RX_BUF_SIZE     BCP_I2C_TX_BUF_SIZE
#endif /* BUS_COM_PLT_I2C_BUF_SHARED_SIZE */

#define RESET_TIMEDREQ_TIMEOUT     5000

/* Identificatori canale fisico di comunicazione 
La struttura per la tabella di riferimento � #BCP_ChannelTableType
*/
enum ePP_ChannelId
{
#ifdef BUS_COM_PLT_I2C_SUPPORT  
  CP_I2C_CH_ID,                            /**< Identificatore Canale I2C */
#endif /* BUS_COM_PLT_I2C_SUPPORT */
#ifdef BUS_COM_PLT_UART_SUPPORT
  CP_UART1_CH_ID,
#endif	
  /*BCP_UART2_CH_ID,*/
  BCP_CH_NUM
};

/*for compatibility*/
#define BCP_I2C_CH_ID       CP_I2C_CH_ID

#define TOTAL_CHANNELS      BCP_CH_NUM  

/* ri-define of data struct of the communication manager to code-optimize in the case of single channel */
#define COMM_MNG(channel)  CommMng[channel]


#ifdef BUS_COM_PLT_I2C_BUF_SHARED
#define BCP_I2CRxBuffer BCP_I2CTxBuffer
#endif /* BUS_COM_PLT_I2C_BUF_SHARED */



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** 
Structure that allows to define properties about shared data between CP and application.
*/
typedef struct 
{
    void*                   pData;                  /**< pointer to data */
    CP_PacketSizeType       szMaxLength;            /**< size in bytes */
}CPApplianceDataTableType;

/** Struttura dati di ritorno della funzione getRefExpansionData() */
typedef struct 
{    
    uint8_t sizeBufDataForExp; 
    /** bitmap */
    uint8_t whichExp;  
    void *pBufDataForExp;

}expansionRef_t;


/* ****************** */
/* Exported variables */
/* ****************** */

extern const BCP_ChannelTableType BCP_ChannelTable[];

/* ******************* */
/* Function prototypes */
/* ******************* */


void ActionComm(IO_ChannelType ch, IO_NotificationType notif);
void CP_I2CCommunicationPlatformInit(void);


void Set_CPApplianceDataItem(CPApplianceDataTableIndexType index, CPApplianceDataTableType* pData);


void UseRegs(void);
void UnuseRegs(void);

void UseUiInfo(void);
void UnuseUiInfo(void);

#endif /* __BUSCOMMPLTF_H */

/* *********** */
/* End of file */
/* *********** */
