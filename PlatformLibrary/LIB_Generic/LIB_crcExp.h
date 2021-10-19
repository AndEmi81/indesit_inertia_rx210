/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		LIB_crc.h
\n			Header file of LIB_crc.c
\n
\file		LIB_crcExp.h
\ingroup	LIB_Generic
\date		Sep 05, 2012
\version	01.00.01
\author		alessandro.sabbatini
\n
\n
\par	History:
\n
\n              01.00.01
\n              Sep 05, 2012
\n              Alessandro Sabbatini
\n              Changed to become an Application interface to CRC driver
\n
\n
\n 		01.00.00
\n		Apr 6, 2010
\n		Stefano Ughi
\n		First Insertion
\n
\n
*/

#ifndef _LIB_CRC_H_
#define _LIB_CRC_H_

/* ********** */
/* Includes */
/* ********* */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
#define LIB_CRC_DEFAULTSEED             0xFFFF

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
uint16_t LIB_crc_calculate (uint16_t len, uint8_t *pSource);
uint16_t LIB_crc_partial_calculate (uint16_t len, uint8_t *pSource, uint16_t seed);

#endif /* _LIB_CRC_H_ */

/* *********** */
/* End of file */
/* *********** */
