/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		ZcLoadsExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		ZcLoadsExp.h
\ingroup	LIB_ZcLoads
\date		Nov 26, 2010 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n
\n 		01.00.00 
\n		Nov 26, 2010 
\n		stefano.ughi 
\n		changes 
\n 			Creation
\n
*/

#ifndef __ZC_LOADS_EXP_H
#define __ZC_LOADS_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */
#if ( ZC_LOADS_NUM <= 8 )
typedef uint8_t zcLoads_output_t;
#elif ( ( ZC_LOADS_NUM > 8 ) && ( ZC_LOADS_NUM <= 16 ) )
typedef uint16_t zcLoads_output_t;
#else
	#error "\n ZC_LOADS_NUM property : ZC_LOADS_NUM too high max value 16 "
#endif

/** return value typedef zcLoadsError_t */
#define ZC_LOADS_E_OK   0
/** return value typedef zcLoadsError_t */
#define ZC_LOADS_E_KO   1

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
typedef uint8_t zcLoadsError_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */
#if !defined(MODULE_BODY_LIB_ZCLOADS)
extern zcLoads_output_t ZcLoads_Enabled;
extern uint8_t ZcLoads_Index;
#endif

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */

#endif /* __ZC_LOADS_EXP_H */

/* *********** */
/* End of file */
/* *********** */



