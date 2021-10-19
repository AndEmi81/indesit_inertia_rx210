/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		SWCRC_drv.h
\n			Header file of SWCRC_drv.c
\n
\file		SWCRC_drv.h
\ingroup	SWCRC
\date		Apr 6, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Apr 6, 2010
\n		Stefano Ughi
\n		First Insertion
\n
\n
*/

#ifndef __SWCRC_DRV_H
#define __SWCRC_DRV_H

/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
/*lint -e537*/
/*lint +e537*/
#include "Hal_Interface.h"
#include  HAL_CFG
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
IO_ErrorType SWCRC_InitSync(void);
IO_ErrorType SWCRC_DeInitSync(void);
uint16_t SWCRC_Calculate (IO_BufferSizeType len, uint8_t *pSource, uint16_t seed);

#endif /* __SWCRC_DRV_H */

/* *********** */
/* End of file */
/* *********** */
