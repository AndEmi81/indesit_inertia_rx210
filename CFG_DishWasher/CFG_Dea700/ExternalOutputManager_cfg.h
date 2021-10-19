/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		[Title]
\n			[Brief description]
\n
\file		ExternalOutputManager_cfg.h
\ingroup	[module_name_slave] 
\date		Apr 5, 2013
\version	01.00.00 
\author		Marco Sperandini

*/

#ifndef __EXTERNALOUTPUTMANAGER_CFG_H
#define __EXTERNALOUTPUTMANAGER_CFG_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/*Command buffer protocol header */
#define EXTERNAL_COMMAND_BUFFER_HDR_SZ		CONFIG_EXTERNAL_COMMAND_BUFFER_HDR_SZ /* (2) */ /* PilotType|displacement */


/*Protocol level sizes for requests */
/** number of bytes for read ram in command buffer */
#define IOM_EXT_CMDDATA_RAM_READ_SZ  		CONFIG_IOM_EXT_CMDDATA_RAM_READ_SZ /* 4 */
/** number of bytes for write ram in command buffer */
#define IOM_EXT_CMDDATA_RAM_WRITE_SZ    	CONFIG_IOM_EXT_CMDDATA_RAM_WRITE_SZ /* 4 */
/** number of bytes for extra request in command buffer */
#define IOM_EXT_CMDDATA_EXTRA_REQ_SZ    	CONFIG_IOM_EXT_CMDDATA_EXTRA_REQ_SZ /* 2 */
/** number of bytes for applyware */
#define IOM_EXT_CMDDATA_AW_HDR_SZ			CONFIG_IOM_EXT_CMDDATA_AW_HDR_SZ /* 1 */

/** whole AW Command Data request */
#define IOM_EXT_AW_CDM_DATA_SZ				(1/*ToolID request*/+IOM_EXT_CMDDATA_RAM_READ_SZ+IOM_EXT_CMDDATA_RAM_WRITE_SZ+IOM_EXT_CMDDATA_EXTRA_REQ_SZ)


/* library level command sizes for pilot types */
#define EXTERN_MODULE_L_CMD_SZ				CONFIG_EXTERN_MODULE_L_CMD_SZ /* (6) */
#define PMSM_MOTOR_L_CMD_SZ       			CONFIG_PMSM_MOTOR_L_CMD_SZ /* (6) */
#define BLDC_WASH_L_CMD_SZ        			CONFIG_BLDC_WASH_L_CMD_SZ /* (6) */
#define BLDC_DRAIN_L_CMD_SZ       			CONFIG_BLDC_DRAIN_L_CMD_SZ /* (6) */
#define PMSM_DRYER_L_CMD_SZ       			CONFIG_PMSM_DRYER_L_CMD_SZ /* (8) */
#define MULTIPOWER_COMP_L_CMD_SZ  			CONFIG_MULTIPOWER_COMP_L_CMD_SZ /*(6) */ /* TODO : check if MULTIPOWER_COMP_L_CMD_SZ is 8 */
#define BRUSHLESS_HOOD_L_CMD_SZ   			CONFIG_BRUSHLESS_HOOD_L_CMD_SZ /* (8) */
#define AIRSENSOR_HOOD_L_CMD_SZ				CONFIG_AIRSENSOR_HOOD_L_CMD_SZ /*(6) */

/*  protocol level command sizes */
/** number of bytes for EXTERN_MODULE_L command*/
#define EXTERN_MODULE_L_CMD_DATA_SZ			(EXTERN_MODULE_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for PMSM_MOTOR_L command*/
#define PMSM_MOTOR_L_CMD_DATA_SZ			(PMSM_MOTOR_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for BLDC_WASH_LWASH_L command*/
#define BLDC_WASH_L_CMD_DATA_SZ				(BLDC_WASH_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for BLDC_DRAIN_L command*/
#define BLDC_DRAIN_L_CMD_DATA_SZ			(BLDC_DRAIN_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for PMSM_DRYER_L command*/
#define PMSM_DRYER_L_CMD_DATA_SZ			(PMSM_DRYER_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for MULTIPOWER_COMP_L command*/
#define MULTIPOWER_COMP_L_CMD_DATA_SZ		10 /* TODO (MULTIPOWER_COMP_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ) */
/** number of bytes for BRUSHLESS_HOOD_L command*/
#define BRUSHLESS_HOOD_L_CMD_DATA_SZ		(BRUSHLESS_HOOD_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)
/** number of bytes for AIRSENSOR_HOOD_L command*/
#define AIRSENSOR_HOOD_L_CMD_DATA_SZ		(AIRSENSOR_HOOD_L_CMD_SZ + EXTERNAL_COMMAND_BUFFER_HDR_SZ)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */



#endif /* __EXTERNALOUTPUTMANAGER_CFG_H */

/* *********** */
/* End of file */
/* *********** */
