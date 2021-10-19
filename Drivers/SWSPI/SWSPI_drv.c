/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/



/**
\n
\Par	SW SPI Driver 
\n		Implementation of SW SPI driver.
\n
\file		${SWSPI_drv.c}
\ingroup	SWSPI
\date		01-10-2009
\version	Platform2_00.00.12 (project baseline related to creation of this module)
\author	  Andrea Mantovani
\n
\n
\Par\b	History:
\n 
\n 		01.04.00 (baseline di progetto relativa alla versione corrente)
\n		gg/mm/aaaa <data in cui � stata realizzata la versione corrente)
\n		developer (progettista autore della modifica)
\n		changes (principali modifiche apportate)
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00 (baseline di progetto relativa alla versione precedente la corrente)
\n		gg/mm/aaaa (data in cui � stata realizzata)
\n		developer (progettista autore della modifica)
\n		changes (principali modifiche apportate)
\n 		- change 1
\n		- change 2
\n
\n
\n 
*/

/* ******** */
/* Includes */
/* ******** */

#include "Config.h"

#include "OCDIO_drv.h"
#include "SWSPI_drv.h"


/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */

void WriteByte_SPIsw(uint8_t Data);	

uint8_t ReadByte_SPIsw(uint8_t DataOut);	


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/*
 initializes uP pins to implement the SPI sw
*/
IO_ErrorType SWSPI_InitSync(void* unused)
{
  /*  pin configuration */
    
  /*by default is enabled as GPIO*/
  OCDIO_SetPinDirValueSync((uint8_t)SPISW_CS_IO_PIN,(uint8_t)PIN_OUT_DIR);
  
  /*by default is enabled as GPIO*/
  OCDIO_SetPinDirValueSync((uint8_t)SPISW_MOSI_IO_PIN,(uint8_t)PIN_OUT_DIR);
  
  /*by default is enabled as GPIO*/
  OCDIO_SetPinDirValueSync((uint8_t)SPISW_MISO_IO_PIN,(uint8_t)PIN_IN_DIR);
  
  /*by default is enabled as GPIO*/
  OCDIO_SetPinDirValueSync((uint8_t)SPISW_CK_IO_PIN,(uint8_t)PIN_OUT_DIR);
    
  /*by default is enabled as GPIO*/
  OCDIO_SetPinDirValueSync((uint8_t)EM_RESET_IO_PIN,(uint8_t)PIN_OUT_DIR);
  
  /* pin initial value*/
  /* CS HIGH (disabled) */
  OCDIO_SetPinValueSync((uint8_t)SPISW_CS_IO_PIN, (uint8_t)HI);
  
  /* This bit is not an SPI bit but a module reset.
   This initialization should be moved to product module level
   to do...
   */
  /* Energy Meter reset   (disabled) */
  OCDIO_SetPinValueSync((uint8_t)EM_RESET_IO_PIN, (uint8_t)HI);
  

#ifdef AIM_BOARD
#ifdef AIM_debug
  /*
    Init borad leds
    only for debug purposes
  */  
  KBI1PE_KBI1PE1 =0; /* GPIO */
  KBI1PE_KBI1PE2 =0; /* GPIO */
  
  PTADD_PTADD1  =1; /* out */
  PTADD_PTADD2  =1; /* out */
#endif
#endif

return IO_E_OK;
  
}


/* 
   Reads a buffer from SPI 
   You have to provide also the bytes to be written to MOSI while reading from MISO
*/
IO_ErrorType SWSPI_ReadDataSync(uint8_t NBytes, uint8_t *DataBufIn, const uint8_t *DataBufOut)
{
  
  uint8_t i;    
  
  for(i=0; i<NBytes;i++)
  { 
    *(DataBufIn+i) = ReadByte_SPIsw(DataBufOut[i]);       	/* Read Data  */    
    
  }
  return IO_E_OK;
    
}



/* 
  Writes a buffer to SPI 
*/

IO_ErrorType SWSPI_WriteDataSync(uint8_t NBytes, const uint8_t *DataBuf)
{ 

	uint8_t i;
	
	for(i= 0; i< NBytes ;i++)
 	{
   	WriteByte_SPIsw( *(DataBuf+i) );		/* write Data */
 	}
 	return IO_E_OK;
}




/* Local Function */

/*
  Writes 8 bits to SPI 
*/
void WriteByte_SPIsw(uint8_t Data)	
{
  uint8_t x,y; 
  
  for (x=0;x<8;x++)
  {
    if (Data & 0x80)    
    {      
      /* Output '1'	*/
      OCDIO_SetPinValueSync(SPISW_MOSI_IO_PIN, HI);
      
    }	    
    else
    {      
     /* Output '0'	*/
      OCDIO_SetPinValueSync(SPISW_MOSI_IO_PIN, LOW);
      
    }	    
  
    /* clock high	*/
    OCDIO_SetPinValueSync(SPISW_CK_IO_PIN, HI);
    
    /* note:
       this delay is HW dependent, not portable
    */
    for(y=0;y<=2;y++){
      ;
    }
    
    /* clock low	*/
    OCDIO_SetPinValueSync(SPISW_CK_IO_PIN, LOW);
      
    Data<<=1;
  }
}


/* 
  Reads 8 bits from SPI 
*/
uint8_t ReadByte_SPIsw( uint8_t DataOut )		
{
  uint8_t i, Data,x;

  Data=0; 
    
  for (i=0;i<8;i++)
  { 
    
    Data <<= 1;            /* shift Data left */
    
    /* generate output*/
    if (DataOut & 0x80)    
    {      
 	   /* Output '1'	*/
 	   OCDIO_SetPinValueSync(SPISW_MOSI_IO_PIN, HI);
    }	    
    else
    {      
	   /* Output '0'	*/
	   OCDIO_SetPinValueSync(SPISW_MOSI_IO_PIN, LOW);
    }	    
    
    
    /* clock high	*/
    OCDIO_SetPinValueSync(SPISW_CK_IO_PIN, HI);
    
    for (x=0; x<=2; x++) {
      ;  
    }
    		  							
    /* read bit from E2_DO */
    Data |=(uint8_t) OCDIO_GetPinValueSync(SPISW_MISO_IO_PIN);
    /* clock low */
    OCDIO_SetPinValueSync(SPISW_CK_IO_PIN, LOW);
      
    DataOut<<=1;
  }	
  return Data;
}


/* *********** */
/* End of file */
/* *********** */

