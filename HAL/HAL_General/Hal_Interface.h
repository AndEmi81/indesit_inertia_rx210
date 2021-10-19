/**
\defgroup HAL Hardware Abstraction Layer
\par Introduction
\ref whatisit



\defgroup HAL_TOC Table of content
\todo Insert refs
\ingroup HAL

\defgroup HAL_CONVENTIONS   Conventions
\n \ref mcu_hal_names
\n \ref mcu_hal_filenames
\ingroup HAL

\defgroup HAL_GETTING_STARTED Getting Started
\ingroup HAL
\todo Insert refs

\defgroup HAL_SUPPORTED_MCUs Supported MCUs
\todo Insert refs
\ingroup HAL

\defgroup HAL_MNUFACT_CODES Manufacturer codes
\ingroup HAL_SUPPORTED_MCUs
\todo Insert refs to manufaturer codes


\page whatisit  What is it?

\par  Description
  It is the fundamental component of the Platform 2.0.
  The HAL arranges a standard interface for the direct access to the hardware resources of a MCU.
  The replacement of the hardware has not to involve structural interventions on the application, but only on the driver, at most

  It consists of a series of software modules whose task is making application code and platform code independent from the implementation of the hardware interface. 
  In our actual main firmware platform, many routines interface directly to the microcontroller registers and so they depend on its choice. 
  This will be avoided by the Hw Abstraction Layer, through standard library calls.
 
\par  Why?

\li Recycle as much as possible the same source code on many microcontrollers. HAL provides a set of macro or functions that allow to interface peripheral drivers 
to registers.
\li Fastest porting of an application from one microcontroller to the other. 
\li The application developer should not know the details of the firmware platform, but only those of its interface.



\page conventions  Conventions

\n \ref mcu_hal_names
\n \ref mcu_hal_filenames
  
\page mcu_hal_names MCU HAL names

The HAL names is arranged according to:
\li manufacturer. \see \subpage manufaturercodes
\li family 
\li model

\section mcu_hal_names_format  Name format of a HAL component
The format of the name for the HAL component of a specific MCU is:
\<Manufaturer Code\>_\<Family\>_\<Model\>

\section mcu_hal_names_examples Examples
\par Freescale HCS08 GB60
FS_MC9S08_GB60
\par Renesas 300H 39079
RN_300H_39079


\page mcu_hal_filenames MCU HAL filenames

\section mcu_hal_filenames_registers Registers

Registers map and registers structure definition are supplied by manufaturer, in one or two files.

In the first case HAL probably will contain a single registers file:
\<Manufacturer Code\>_\<Family\>_\<Model\>_reg.h containing both registers map and registers structure definition.

In the second case registers map is contained in the file 
\<Manufacturer Code\>_\<Family\>_\<Model\>_reg.c
and the definitions of the structures of registers are in 
\<Manufacturer Code\>_\<Family\>_\<Model\>_reg.h

\see \subpage manufaturercodes

\section mcu_hal_filenames_hal HAL implementation file

The HAL implementation file \<Manufacturer Code\>_\<Family\>_\<Model\>_hal.h

\section mcu_hal_filenames_hal_cfg HAL configuration file

The HAL configuration file \<Manufacturer Code\>_\<Family\>_\<Model\>_cfg.h checks 
the correct definition of the compulsory properties and takes care of the error generation 
in order to inform the user about any unavailable functions or forbidden configurations.

\section mcu_hal_names_examples Examples
\par Freescale HCS08 GT60
Ther are two files:
\li FS_MC9S08_GT60_reg.h
\li FS_MC9S08_GT60_reg.c
\li FS_MC9S08_GT60_hal.h
\li FS_MC9S08_GT60_cfg.h

\par Renesas 300H 39079
\li RN_300H_39079_reg.h
\li RN_300H_39079_hal.h
\li RN_300H_39079_cfg.h



\page manufaturercodes Manufacturer codes

\li Freescale Semiconductor: \b FS
\li Renesas : \b RN
\li NXP: \b NX


\page TableOfContent  Table of content

  \par Supported MCUs
  \ref HAL_SUPPORTED_MCUs
 
  \par History
  \ref HAL_HISTORY
  

\page freescale Freescale
\ref FS_HCS08_GB60 
\ref FS_MC9S08_GT60 
\ref FS_MC9S08_QE128
\ref FS_MCF51_AG128 

\page renesas Renesas
\ref RN_300H_39079
*/
/**
\page gettingstarted  Getting started

\section gettingstarted_intro Introduction
This page gives guideline in order to support the use of HAL in your application project. It is assumed the your project is under versioning control (Clear Case).

\section gettingstarted_prepareingclearcase Preparing Clear Case
The first thing to do is preparing the project under versioning control adding the following components (update to the latest baseline):
\li \b HAL_General,
\li at least one MCU-specific HAL, among  supported ones. \ref HAL_SUPPORTED_MCUs

\section gettingstarted_preparingprj Preparing your project

\subsection gettingstarted_preparingprj_addpaths  Adding component paths
Add in your project theese \e relative paths:
\li <b> HAL\\HAL_General</b> : this allows your project to use some \e standard \e types and \e standard \e defines.
\li <b> HAL\\<MCU-specific HAL></b> path: this allows the drivers to access MCU-specific registers and to use MCU-specific settings.

\subsection gettingstarted_preparingprj_addpaths  Adding HAL \e source files
Add in your project these \e source files:
\li <MCU-specific HAL>_reg.c if theese file is not provided, this is not a required step 

\subsection gettingstarted_multiMCUprj Multi-MCUs projects
If your project needs to support several MCU, create as many \e targets (\e configurations) as MCUs to support are.

\section gettingstarted_drivers Drivers
HAL is not usable as is: the \e dependency rule states that the application cannot directly access the hal, you need to add the \e Driver component
and the following paths:

\li <b> Drivers\\ </b> :this allows the drivers to access some drivers-specific \e types, \e defines and \e enums.
\li <b> Drivers\\XYZ </b>: this allows the application to use the driver. Add all the paths corresponding to the drivers needed by the application

\section gettingstarted_precompopt Procompiler options
To allows drivers to include hal files, MCU define must be defined in the precompiler
as indicated in \ref HAL_SUPPORTED_MCUs

\section gettingstarted_configfile Config.h file
Specify in this file all the options concerning HAL and Drivers.
Each HAL has its own options and supported values. \ref HAL_SUPPORTED_MCUs
*/



/**
\n
\par		    Header file

This file defines HAL_CFG e HAL according to the precompiler option, which allow to include HAL files by the drivers

\par \b PCLint: x Errors, x Warnings: indentation, x Info dd/mm/yyyy Roberto Fioravanti


\file    HAL_Interface.h
\ingroup HAL
\date    12/03/2009
\author  Roberto Fioravanti
\version 00.00.12

\defgroup HAL_HISTORY  HAL History
\ingroup HAL
This module contains the history

\addtogroup HAL_HISTORY
@{
\par\b	History:
\n 
\n 		        00.00.12
\n          	08/09/2009
\n          	Roberto Fioravanti
\n          	changes
\n            -  S08 GT60 hal added
\n
\n 		        00.00.01
\n          	25/02/2009
\n          	Roberto Fioravanti
\n          	Prima versione
\n
@}
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



#ifndef __HAL_INTERFACE_H
#define __HAL_INTERFACE_H


/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/**
\defgroup FS_HCS08_GB60 Freescale HCS08 GB60
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b FREESCALE_S08_GB60

\par Supported classes
\ref HAL_FS_HCS08_GB60_CLS
*/
#ifdef FREESCALE_S08_GB60
#include <stddef.h>			 /* Please don't remove Stefano Ughi */
#include <string.h>			 /* Please don't remove Stefano Ughi */
#include <stdlib.h>          /* Please don't remove Stefano Ughi */
#define HAL       "FS_HCS08_GB60_hal.h"
#define HAL_CFG   "FS_HCS08_GB60_cfg.h"
#endif /* FREESCALE_S08_GB60 */
/**
\defgroup FS_MC9S08_GT60 Freescale HCS08 GT60
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b FREESCALE_MC9S08_GT60

\par Supported classes
\ref HAL_FS_MC9S08_GT60_CLS
*/
#ifdef FREESCALE_MC9S08_GT60
//#include <stddef.h>
#define HAL       "FS_MC9S08_GT60_hal.h"
#define HAL_CFG   "FS_MC9S08_GT60_cfg.h"
#endif /* FREESCALE_MC9S08_GT60 */
/**
\defgroup FS_MC9S08_QE128 Freescale HCS08 QE128
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b FREESCALE_MC9S08_QE128

\par Supported classes
\ref HAL_FS_MC9S08_QE128_CLS
*/
#ifdef FREESCALE_MC9S08_QE128
#include <stddef.h>          /* Please don't remove Stefano Ughi */
#include <string.h>			 /* Please don't remove Stefano Ughi */

#define HAL       "FS_MC9S08_QE128_hal.h"
#define HAL_CFG   "FS_MC9S08_QE128_cfg.h"
#endif /* FREESCALE_MC9S08_QE128 */
/**
\defgroup RN_300H_39079 Renesas 300H 39079
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b RENESAS_300H_39079

\par Supported classes
\ref HAL_RN_300H_39079_CLS
*/
#ifdef RENESAS_300H_39079
#include <stddef.h>
#include <string.h>
#define HAL       "RN_300H_39079_hal.h"
#define HAL_CFG   "RN_300H_39079_cfg.h"
#endif /* RENESAS_300H_39079 */

/**
\defgroup FS_MCF51_AG128 Freescale MCF51 AG128
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b FREESCALE_MCF51_AG128

\par Supported classes
\ref HAL_FS_MCF51_AG128_CLS
*/
#ifdef FREESCALE_MCF51_AG128
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define HAL       "FS_MCF51_AG128_hal.h"
#define HAL_CFG   "FS_MCF51_AG128_cfg.h"
#endif /* FREESCALE_MCF51_AG128 */
/**
\defgroup NXP_LPC12xx_1227 NXP LPC1227
\ingroup HAL_SUPPORTED_MCUs

\par Precompiler option
To support this MCU define in precompiler option \b NXP_LPC12xx_1227

\par Supported classes
\ref HAL_NX_M0_LPC1227_CLS
*/
#ifdef NXP_LPC12xx_1227
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define HAL       "NX_M0_LPC1227_hal.h"
#define HAL_CFG   "NX_M0_LPC1227_cfg.h"
#endif /* NXP_LPC12xx_1227 */


/**
\defgroup RN_RX210_R5F52108 Renesas RX210
\ingroup HAL_SUPPORTED_MCUs

\par Procompiler option
To support this MCU define in precompiler option \b RN_RX210_R5F52108

\par Supported classes
\ref HAL_RN_RX210_R5F52108_CLS

\par Author
	-"Andrea Emili,SPES scpa"
*/
#if defined(RN_RX200_R5F5210) || defined(RN_RX200_R5F52Z0)
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "RN_RX200_reg.h"
#define HAL       "RN_RX200_hal.h"
#define HAL_CFG   "RN_RX200_cfg.h"
#endif /* defined(RN_RX200_R5F5210) || defined(RN_RX200_R5F52Z0) */


#if defined(RN_RX100)
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "RN_RX100_reg.h"
#define HAL       "RN_RX100_hal.h"
#define HAL_CFG   "RN_RX100_cfg.h"
#endif /* defined(RN_RX100_R5F5111)  */


/* ****** */
/* Errors */
/* ****** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __HAL_INTERFACE_H */



/* *********** */
/* End of file */
/* *********** */
