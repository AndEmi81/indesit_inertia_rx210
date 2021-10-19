/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n			Header file of OCFLS_drv.c
\n
\file		OCFLS_drv.h
\ingroup	OCFLS
\date		Nov 10, 2009
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Nov 10, 2009
\n		stefano.ughi
\n		changes
\n 		- creation
\n
*/

#ifndef __OCFLS_DRV_H
#define __OCFLS_DRV_H

#define __DRIVER_H
/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include HAL
#include "IO_driver.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

#define FLS_IO_DEVICETYPE	RANDOM
#define FLS_IO_ERRORHOOK	FALSE
#define FLS_IO_DRIVERMODUS	SYNCRON

#define FLS_WRITE_BEGIN()     {}
#define FLS_WRITE_END()       {}
#define FLS_ERASE_BEGIN()     {}
#define FLS_ERASE_END()       {}

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

IO_ErrorType OCFLS_InitSync(void);
IO_ErrorType OCFLS_REraseSync (FLS_sizeType size, FLS_positionType position);
IO_ErrorType OCFLS_REraseVerify(FLS_sizeType size, FLS_positionType position);
IO_ErrorType OCFLS_RWriteSync (FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);
IO_ErrorType OCFLS_RWriteVerify (FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);
IO_ErrorType OCFLS_BlankCheckEraseVerifyAsync (FLS_sizeType size, FLS_positionType position);
IO_ErrorType OCFLS_WriteAndVerifyAsync(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);	  

#endif /* __OCFLS_DRV_H */

/* *********** */
/* End of file */
/* *********** */
