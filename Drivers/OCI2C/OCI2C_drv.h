/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Description
\n		I2C Driver header files.
\n
\file		OCI2C_drv.h
\ingroup	OCI2C 
\date		08/01/2010
\version	00.00.25 
\author	Roberto Fioravanti
\n
\n
\n
\n
\par		History
\n
\n 		00.00.01 
\n		12/03/2009 
\n		Roberto Fioravanti
\n		changes
\n 		- creazione
\n
\n
*/

/*
Copyright (c) 2009-2010, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date   
\author  
*/





#ifndef __OCI2C_DRV_H
#define __OCI2C_DRV_H


/* ******** */
/* Includes */
/* ******** */

#include  "Config.h"
#include  "Hal_Interface.h"

#include  HAL_CFG

#include "io_driver.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
enum I2C_BUS_STATUS_BIT
{
  I2C_BUS_STATUS_BUSY_BIT=0,
  I2C_BUS_STATUS_SDA_BIT,
  I2C_BUS_STATUS_SCL_BIT,
  I2C_BUS_STATUS_TX_BIT
};
#define I2C_BUS_STATUS_BUSY   (1<<I2C_BUS_STATUS_BUSY_BIT)
#define I2C_BUS_STATUS_SDA    (1<<I2C_BUS_STATUS_SDA_BIT)
#define I2C_BUS_STATUS_SCL    (1<<I2C_BUS_STATUS_SCL_BIT)
#define I2C_BUS_STATUS_TX     (1<<I2C_BUS_STATUS_TX_BIT)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

typedef uint8_t OCI2C_LenType;
typedef uint8_t OCI2C_OptionType;


typedef struct OCI2C_MasterTxParam_tag
{
  IO_MemPtrType buffer;
  OCI2C_LenType len;
  OCI2C_OptionType  option;
}OCI2C_MasterTxParamType;

typedef OCI2C_MasterTxParamType OCI2C_MasterRxParamType;

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
IO_ErrorType OCI2C_InitSync(IO_PhyInitSyncType * initSync);
void OCI2C_InitHwSync(void);

#if defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT)
IO_ErrorType OCI2C_LowPowerMode(Comm_Notification LPMCallback);
void OCI2C_ResetLowPowerMode(void);
#endif /* defined(CONFIG_OCLPM_DRV_ENABLE) && defined(CONFIG_OCLPM_DRV_I2C_TO_CANCEL_LP_MODE_SUPPORT) */

IO_ErrorType OCI2C_DeInitSync(void * deinitSync);
IO_ErrorType OCI2C_TxSync(CommDriverParamType * param);
IO_ErrorType OCI2C_RxSync(CommDriverParamType * param);
void OCI2C_ISR(void);



#if (I2C_BUS_STATUS_UPDATE_AND_MONITOR_IN_SYNC == FALSE)
void OCI2C_UpdateBusStatusSync(void);
#endif

bool_t OCI2C_Locked(void);
void OCI2C_ClearCheck(void);



void OCI2C_Action_Lev1(void);
void OCI2C_Action_Lev2(void);
bool_t OCI2C_Level1BlockMonitorSync(void);
bool_t OCI2C_Level2BlockMonitorSync(void);
bool_t OCI2C_Level3BlockMonitorSync(void);
bool_t OCI2C_Level4BlockMonitorSync(void);
bool_t OCI2C_Level5BlockMonitorSync(void);
bool_t OCI2C_Level6BlockMonitorSync(void);
 


void OCI2C_Disable(void);
void OCI2C_Enable(void);

#endif /* __OCI2C_DRV_H */



/* *********** */
/* End of file */
/* *********** */
