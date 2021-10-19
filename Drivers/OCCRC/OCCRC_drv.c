/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		OCCRC_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCCRC_drv.c
\ingroup	OCCRC
\date		Feb 11, 2011
\version	01.00.01
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Apr 07,2011
\n		Stefano Ughi
\n		First Insertion
\n
\n 		01.00.01
\n		Apr 07,2011
\n		Stefano Ughi
\n		- Added IO_ErrorType CRC_InitSync(void)
\n      - Added IO_ErrorType CRC_DeInitSync(void)
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "OCCRC_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */
#if defined (CONFIG_PL_DRV_OCCRC)
#if defined (CRC_HW_SUPPORTED)

/* Exported function */
/**
Module's init.

\return 	IO_ErrorType

\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvCRC_2
*/
IO_ErrorType OCCRC_InitSync(void)
{
	InitCRCmodule();
	return (IO_ErrorType)IO_E_OK;
}
/**
Deinit CRC module, M.C.U. reset state.

\return 	IO_ErrorType

\retval		IO_E_OK

\note		After an CRC_DeInitSync call it's mandatory to call a CRC_InitSync
			if you want use the driver.
\par Architecture Requirements ID: SARS_WMMT_MN_DrvCRC_2
*/
IO_ErrorType OCCRC_DeInitSync(void)
{
	DeInitCRCmodule();
	return (IO_ErrorType)IO_E_OK;
}

/**
Crc CCITT hardware computation.
Polynomial: x^16+x^12+x^5+1

\param  [in]		len number of bytes on which CRC is calculated
\param  [in]		pSource point to the first byte of the array on which CRC is calculated
\param  [in]		seed is the initial value for CRC computation, typically 0xFFFF but\n
                    if you have some fixed bytes you may precompute CRC and pass it as seed.\n
                    (E.g. Master Mode Tx)

\return 	uint16_t
\retval		crc

\todo       Insert WatchDog Refresh
*/
uint16_t OCCRC_Calculate (IO_BufferSizeType len, uint8_t *pSource, uint16_t seed)
{
	IO_BufferSizeType i;
	uint16_t orig_crc;
	uint16_t crc;
	uint8_t	*pData=pSource;

	orig_crc = (uint16_t) GetFinalCRC();

	InitCRC(seed);

	for(i=0; i<len; i++)
	{

		ComputeCRC(*pData);
		pData++;

	}
	crc = (uint16_t) GetFinalCRC();
	InitCRC(orig_crc);
	return crc;

}

#else
#error "You defined CRC_HW but this microcontroller doesn't support hardware CRC\n try again and the next one you'll be, probably, more lucky."
#endif /* #if defined (CRC_HW_SUPPORTED) */
#endif /* #if defined (PL_DRV_OCCRC) */
/* Local Function */

/* *********** */
/* End of file */
/* *********** */

