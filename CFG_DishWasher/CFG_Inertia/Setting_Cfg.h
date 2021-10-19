/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		[Title]
\n			[Brief description]
\n
\file		Setting_Cfg.h
\ingroup	[module_name_slave] 
\date		Apr 5, 2013
\version	01.00.00 
\author		Roberto Fioravanti

*/

#ifndef __SETTING_CFG_H
#define __SETTING_CFG_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* SF_PTR_FAULT Pointer */
/*#define SF_PTR_FAULT                                    0x0027u*/
/* SF_PTR_FAULT Displacements */
#define SF_DISPL_FAULT_DOORLOCK									0x00u
#define	SF_DISPL_FAULT_PROBE									0x02u
#define	SF_DISPL_FAULT_MOTOR									0x03u
#define	SF_DISPL_FAULT_WASH_RES									0x04u
#define SF_DISPL_FAULT_DRAINPUMP								0x05u
#define SF_DISPL_FAULT_DRYING									0x06u

#define SF_DISPL_FAULT_APP_PRIO 								5
#define SF_DISPL_FAULT_PLT_PRIO									10

/* SF_PTR_SAFETY_PARAMETERS Pointer */
/*#define SF_PTR_SAFETY_PARAMETERS                        0x002Au*/
/* SF_PTR_SAFETY_PARAMETERS Safety Displacements */
#define SF_DISPL_SFT_DRAINPUMP									0x00u
#define SF_DISPL_SFT_FAULT_ACTIONS								0x02u
#define SF_DISPL_SFT_DRYING										0x05u



/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */



#endif /* __SETTING_CFG_H */

/* *********** */
/* End of file */
/* *********** */
