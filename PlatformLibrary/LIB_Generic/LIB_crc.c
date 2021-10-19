/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		LIB_crc.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		LIB_crc.c
\ingroup	LIB_Generic
\date		Sep 05, 2012
\version	01.00.01
\author		alessandro.sabbatini

*/

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

#ifdef CONFIG_LIB_CRC_OCCRC
#include "OCCRC_drv.h"
#define LIB_CRC_calculate OCCRC_Calculate
#else
#include "SWCRC_drv.h"
#define LIB_CRC_calculate SWCRC_Calculate
#endif

#include "LIB_crcExp.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Partial CRC computation.
Polynomial: x^16+x^12+x^5+1

\param  [in]		len number of bytes on which CRC is calculated
\param  [in]		pSource point to the first byte of the array on which CRC is calculated
\param  [in]		seed is the initial value for CRC computation, typically 0xFFFF but\n
                    if you have some fixed bytes you may precompute CRC and pass it as seed.
                    (E.g. Master Mode Tx)

\return 	uint16_t

\retval		crc


\author 	alessandro.sabbatini
\date		Sep 05, 2012
\version	1.0.1

\note		\par \b Note 1:
			documentation by alessandro.sabbatini

*/

uint16_t LIB_crc_partial_calculate (uint16_t len, uint8_t *pSource, uint16_t seed)
{
    return LIB_CRC_calculate((IO_BufferSizeType) len, pSource, seed);
}

/**
CRC computation.

\param  [in]            len number of bytes on which CRC is calculated
\param  [in]            pSource point to the first byte of the array on which CRC is calculated

\return         uint16_t

\retval         crc


\author         alessandro.sabbatini
\date           Sep 05, 2012
\version        1.0.1

\note           \par \b Note 1:
                        documentation by alessandro.sabbatini

*/

uint16_t LIB_crc_calculate (uint16_t len, uint8_t *pSource)
{
    return LIB_CRC_calculate((IO_BufferSizeType) len, pSource, LIB_CRC_DEFAULTSEED);
}


/* Local Function */

/* *********** */
/* End of file */
/* *********** */

