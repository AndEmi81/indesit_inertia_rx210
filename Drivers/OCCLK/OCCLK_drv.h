/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		    OCCLK
\n              On Chip Clock Sysnthesis Driver Header file
\n			    Contains the implementation of the clock sysntesis driver
\n
\file		    OCCLK_drv.h
\ingroup	    OCCLK
\date		    16/02/2010
\version	    00.00.02
\author		    Jaroslav Musil
\n
\n
\par	History:
\n
\n 		00.00.02
\n      16/02/2010
\n      Jaroslav Musil
\n
\n 		00.00.01
\n      3/02/2010
\n      Jaroslav Musil
\n      creation
\n
*/


#ifndef __OCCLK_HAL_H
#define __OCCLK_HAL_H


#define __DRIVER_H

/* ******** */
/* Includes */
/* ******** */

/*lint -e537*/
#include "HAL_Interface.h"
/*lint +e537*/
/*lint -e537*/
#include  HAL_CFG
/*lint +e537*/
/*lint -e537*/
#include  HAL
/*lint +e537*/
/*lint -e537*/
#include "IO_driver.h"
/*lint +e537*/


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


IO_ErrorType OCCLK_EngageClkRefSync(void);

IO_ErrorType OCCLK_SetInternalClockSync(uint32_t desiredClock);

IO_ErrorType OCCLK_SetExternalClockSync(uint32_t desiredClock);

IO_ErrorType OCCLK_SetFLLExternalClockSync(uint32_t desiredClock);

IO_ErrorType OCCLK_SetFLLInternalClockSync(uint32_t desiredClock);

bool_t OCCLK_IsExtRefClockReady(void);

IO_ErrorType OCCLK_SetTrimSync(CLK_TrimmingType desiredTrim);

CLK_TrimmingType OCCLK_GetTrimSync(void);


#endif /* __OCCLK_HAL_H */



/* *********** */
/* End of file */
/* *********** */
