/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		SW SPI Driver
\n		This is the header file for SW SPI driver
\n
\file		${SWSPI_drv.h}
\ingroup	SWSPI
\date		01-10-2009
\version	Platform2_00.00.12 (project baseline related to creation of this module)
\author		Andrea Mantovani
\n
\n
\Par\b	History:
\n
\n 		01.04.00 <baseline di progetto relativa alla versione corrente>
\n		gg/mm/aaaa <data in cui è stata realizzata la versione corrente>
\n		developer <progettista autore della modifica>
\n		changes <principali modifiche apportate>
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 <baseline di progetto relativa alla versione precedente la corrente>
\n		gg/mm/aaaa <data in cui è stata realizzata >
\n		developer <progettista autore della modifica>
\n		changes <principali modifiche apportate>
\n 		- change 1
\n		- change 2
\n
\n
\n
*/

#ifndef __SWSPI_DRV_H
#define __SWSPI_DRV_H

#define __DRIVER_H

/* ********** */
/* Includes   */
/* ********** */
/*lint -e537*/
#include "HAL_Interface.h"
#include  HAL_CFG
#include  HAL
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



/**
Initializes GPIO pins to be used by SW SPI. The pins are 
MOSI(output)      data output
MISO(input)       data input
SCK (output)      clock
CS  (output)      chip select
EM_RESET (output) Used to reset the EM chip

This function uses the OCDIO to drive GPIO ports

\param  [in]		  void* unused [For compliance with HIS]
\param  [out]		  none
\param  [in,out]	none

\attention \b 

\return 	IO_ErrorType

\retval   returns only IO_E_OK 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
IO_ErrorType SWSPI_InitSync(void* unused);


/**
It reads a given number of bytes (NBytes) from MISO pin while it clocks out Clock signal and given bytes (DataBufOut) on MOSI
This function uses the OCDIO to drive GPIO ports.

\param  [in]		  NBytes      [ number of bytes to be read ]
\param  [in]		  DataBufOut  [ Bytes to be sent out while reading ]

\param  [out]		  none
\param  [in,out]	DataBufIn   [ Bytes read.DataBufIn[0] is the first read]

\attention \b 

\return 	IO_ErrorType

\retval   returns only IO_E_OK 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
IO_ErrorType SWSPI_ReadDataSync(uint8_t NBytes, uint8_t *DataBufIn, const uint8_t *DataBufOut);


/**
It writes a given number of bytes (NBytes) to MOSI pin while it clocks out Clock signal.
This function uses the OCDIO to drive GPIO ports.

\param  [in]		  NBytes      [ number of bytes to be written ]
\param  [in]		  DataBuf     [ Bytes to be sent out]

\param  [out]		  none
\param  [in,out]	none

\attention \b 

\return 	IO_ErrorType

\retval   returns only IO_E_OK 

\warning	[none]		

\author 	Andrea Mantovani
\date		  19-10-2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by ${Andrea Mantovani}

*/
IO_ErrorType SWSPI_WriteDataSync(uint8_t NBytes, const uint8_t *DataBuf);




#endif /* __SWSPI_DRV_H */

/* *********** */
/* End of file */
/* *********** */









