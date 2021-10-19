/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		TrashExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		TrashExp.h
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

#ifndef __TRASH_EXP_H
#define __TRASH_EXP_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */
/* ID for struct Loader GetCalibrationPointers(uint8_t id_cal_pntrs) routine */
#define ID_CAL_PNTR_ELECT_SUPP	0
#define ID_CAL_PNTR_FACTORY		1
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */
struct Loader GetCalibrationPointers(uint8_t id_cal_pntrs);
#endif /* __SOURCE_EXP_H */

/* *********** */
/* End of file */
/* *********** */

