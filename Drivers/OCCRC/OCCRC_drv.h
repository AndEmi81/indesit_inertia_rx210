/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		OCCRC_drv.h
\n			Header file of OCCRC_drv.c
\n
\file		OCCRC_drv.h
\ingroup	OCCRC
\date		Feb 11, 2011
\version	01.00.01
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		00.00.38
\n		07/04/2010
\n		Stefano Ughi
\n		First Insertion
\n
\n 		00.00.39
\n		Feb 11, 2011
\n		Stefano Ughi
\n		- added #include  HAL
\n
*/

#ifndef __OCCRC_DRV_H
#define __OCCRC_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include  HAL_CFG
#include  HAL
#include "IO_driver.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

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
IO_ErrorType OCCRC_InitSync(void);
IO_ErrorType OCCRC_DeInitSync(void);
uint16_t     OCCRC_Calculate (IO_BufferSizeType len, uint8_t *pSource, uint16_t seed);

#endif /* __OCCRC_DRV_H */

/* *********** */
/* End of file */
/* *********** */
