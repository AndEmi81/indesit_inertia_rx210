/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Descrizione
\n		Definizione di strutture dati e di parametri per il Communication Manager e Platform Protocol.
\n
\file		BusCommPltf_cfg.h
\ingroup	CP_CFG_DEA700
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

#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include "IO_driver.h"
#include "BCP_General.h"

/**
\addtogroup BCP
@{*/

/* Istruzioni addizionali */
#if defined(RENESAS_300H_39079)
#pragma abs16(PP_SlavePlatformProtocol)		/* Renesas abs 16 #pragma */
#pragma abs16(BCP_I2CTxBuffer)
#pragma abs16(BCP_UARTRxBuffer)
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */




#define BCP_MASTER_SUPPORT	FALSE
#define BCP_SLAVE_SUPPORT		TRUE

#define BCP_FRAGMENT_SLAVE




/* Bus Communication Platform */
#define BUS_COM_PLT_I2C_SUPPORT
#define BUS_COM_PLT_I2C_BUF_SHARED_SIZE   (66+8)
#define BUS_COM_PLT_UART_SUPPORT



#define BCM_SLAVE_ACTION_CB ActionComm
#define ENABLE_BCP_EOSN	TRUE




#define BUS_COM_PLT_I2C_BUF_SHARED  /* dummy define: unica opzione supportata */

#define BUS_COM_PLT_BUF_SIZE_DEFAULT  (16+8)
#define BUS_COM_PLT_BUF_SIZE_UART			(70)


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


/* Identificatori canale fisico di comunicazione 
La struttura per la tabella di riferimento è #BCP_ChannelTableType
*/
enum ePP_ChannelId
{
#ifdef BUS_COM_PLT_I2C_SUPPORT  
  BCP_I2C_CH_ID,                            /**< Identificatore Canale I2C */
#endif /* BUS_COM_PLT_I2C_SUPPORT */
#ifdef BUS_COM_PLT_UART_SUPPORT
  BCP_UART1_CH_ID,
#endif	
  //BCP_UART2_CH_ID,
  BCP_CH_NUM
};

#define TOTAL_CHANNELS  BCP_CH_NUM  

/* ri-define of data struct of the communication manager to code-optimize in the case of single channel */
#define COMM_MNG(channel)  CommMng[channel]


#ifdef BUS_COM_PLT_I2C_BUF_SHARED
#define BCP_I2CRxBuffer BCP_I2CTxBuffer
#endif /* BUS_COM_PLT_I2C_BUF_SHARED */


/* user defines */



#if defined(RENESAS_300H_39079)
#include "osek.h"
#endif
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
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
#ifdef EXTERNAL_MOTOR_CONTROL
void Digital_Protocol_Set_External_Board_Com_Ctrl(uint8_t value);
uint8_t Digital_Protocol_Get_External_Board_Com_Ctrl(void);
#endif /*EXTERNAL_MOTOR_CONTROL*/

void Get_Digital_Protocol_Status_Addr_dim (BYTE v);
void Get_Digital_Protocol_Status_Var_dim (BYTE v);
void Get_Digital_Protocol_UIInfoBuff_dim (BYTE v);
void Get_Digital_Protocol_Reg_Var_dim (uint8_t v);



DECLARE_BCP_SLAVEMANAGER(PP_SlaveManager);
void ActionComm(IO_ChannelType ch, IO_NotificationType notif);

/*@}*/
#endif /* __BUSCOMMPLTF_H */

/* *********** */
/* End of file */
/* *********** */
