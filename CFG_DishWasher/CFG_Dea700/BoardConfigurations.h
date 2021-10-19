/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		BoardConfigurations.h
\n			Board Configuration Parameters
\n
\file		BoardConfigurations.h 
\ingroup	[module_name_slave] 
\date		Jul 7, 2010 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n
\n 		01.04.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 
\n		dd/mm/yyyy 
\n		developer 
\n		changes 
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define RTN					12.0	/* [V] nominal voltage supply  */
#define VCC					3.3 	/* [V] uC nominal supply voltage */

/* V ref. measure: Pull Up and Pull Down (Backup and Compensation) */
#define R_PULL_UP			12000.0				/* [Ohm] */
#define R_PULL_DN	 		2000.0				/* [Ohm] */

/* Validation range for Vref */ 
#define RTN_ACC_MAX			22.1 				/* [V] Upper Limit */
#define RTN_ACC_MIN	 		9.0 				/* [V] Lower Limit (Backup Threshold) */

#define V_EFF_TH            179     /* Threshold to distinguish 220V/110V [V] */

/* V Line Measure */
#define RES_1				220.0   /* Nominal Value Pull Up [KOhm]   */
#define RES_2				1.8     /* Nominal Value Pull Down [KOhm] */

#define VRMS_BACKUP_220V    150     /* Threshold for V Line Back Up with nominal VRMS=220V */
#define VRMS_BACKUP_100V     70     /* Threshold for V Line Back Up with nominal VRMS=110V */


#define RTN_TOL_MAX			13.22 				/* in V */
#define RTN_TOL_MIN	 		10.78 				/* in V */

#define UPPER_BOUND_RTN		RTN_TOL_MAX				/* in V */
#define BOTTOM_BOUND_RTN 	RTN_TOL_MIN				/* in V */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/

#endif /* __BOARD_CONFIG_H */

/* *********** */
/* End of file */
/* *********** */
