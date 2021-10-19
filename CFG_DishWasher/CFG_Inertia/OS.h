/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/


/**
\par		Purpose
\n			Application configuration by OS definitions
\n          The definitions in this file could override default ones of the Main framework and can be used 
as starting point for a new final application.
\file		OS.h 
\ingroup	MainFrameworkCfg
\date		Feb 15, 2011 
\version	01.00.00 
\author		Roberto Fioravanti 
\n
\n
\par\b	History:
\n
\n 		00.00.00 
\n		Feb 15, 2011 
\n		Roberto Fioravanti
\n		changes 
\n 			Creation
\n
*/



#ifndef __OS_H
#define __OS_H
/* ******** */
/* Includes */
/* ******** */

#if defined(OS_EmbOS)
#include "RTOS.h"
#endif




/* ****** */
/* Errors */
/* ****** */



/* ****************** */
/* Defines and Macros */
/* ****************** */


/**
@name Critical Sections
@{
*/
#if defined(OS_EmbOS)
#define EnterCriticalSection()      OS_EnterRegion()
#define ExitCriticalSection()       OS_LeaveRegion()
#endif
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
#endif /* __OS_H */

/* *********** */
/* End of file */
/* *********** */