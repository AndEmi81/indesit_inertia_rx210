/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		[Title]
\n			[Brief description]
\n
\file		FlashConfig.h 
\ingroup	[module_name_slave] 
\date		Nov 11, 2009 
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

#ifndef __FLSPLTFCFG_H
#define __FLSPLTFCFG_H

/* ********** */
/* Includes */
/* ********* */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define HAL_FLS_SUPPORT
#define NUM_FLASH_BLOCKS                                3
#define FLS_MODULE_FREQUENCY_Hz	                        175000
/** 
OCFLS_REraseVerify() number of tests performed before feed the dog\n 
if '0' no food for the dog\n
max value is 255. 
*/
#define FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG  255
/** 
OCFLS_RWriteVerify() number of tests performed before feed the dog\n 
if '0' no food for the dog\n
max value is 255. 
*/
#define FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG  128
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

#if defined(RN_RX200)
#if defined(RN_RX200_R5F5210)

#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
#define FLASH_PAGE_SIZE_IN_BYTES	(1024*2)
#define HAL_BLANK_CHECK_ENABLED		1
#elif defined (RN_RX200_R5F52Z0_5) /*2Z0 128K ROM*/
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
#define FLASH_PAGE_SIZE_IN_BYTES	(1024)
#define HAL_BLANK_CHECK_ENABLED		1
#elif defined (RN_RX200_R5F52Z0_6) /*2Z0 256K ROM*/
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
#define FLASH_PAGE_SIZE_IN_BYTES	(1024*2)
#define HAL_BLANK_CHECK_ENABLED		1
#else
#error "MCU not yet supported or check you pre-processor settings."
#endif
#endif /* defined(RN_RX200)*/

#endif /* __FLSPLTFCFG_H */

/* *********** */
/* End of file */
/* *********** */
