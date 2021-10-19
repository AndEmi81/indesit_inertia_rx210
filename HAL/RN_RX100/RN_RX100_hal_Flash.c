/**
 HAL Functions of Flash Device
 Hal Functions to access at hardware Resources from Driver layer
 \file    RN_RX100_Hal_Flash.c
 */
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "RN_RX100_hal.h"
#include "RN_RX100_hal_Flash.h"
#include "LIB_Error.h"
#include "lib/r_fcl_types.h"
#include "r_fcl.h"

/* ********************************************************************************************** */
/*                                  START FLS Class HAL Support                                   */
/* ********************************************************************************************** */
/* Set-up the CPU frequency in MHz */
volatile static FSL_Error=0;
#define FCL_CONFIG_FREQUENCY_MHZ     (8u) /*32u max */
/** map r_fcl_status_t to FLS Error */
const mapError[] ={
		/*R_FCL_OK*/   						FLASH_SUCCESS,
		/*R_FCL_BUSY*/ 						FLASH_BUSY,
		/*R_FCL_ERR_PARAMETER*/				FLASH_ERROR_ADDRESS,
		/*R_FCL_ERR_PROTECTION*/			FLASH_FAILURE,
		/*R_FCL_ERR_REJECTED*/				FLASH_FAILURE,
		/*R_FCL_ERR_WRITE*/					FLASH_FAILURE,
		/*R_FCL_ERR_ERASE*/					FLASH_FAILURE,
		/*R_FCL_ERR_BLANKCHECK*/			FLASH_FAILURE,
		/*R_FCL_ERR_INTERNAL*/				FLASH_FAILURE,
};

#define FLASH_ERROR_ALIGNED     (0x01)
#define FLASH_NOT_BLANK         (0x01)
#define FLASH_ERROR_BYTES       (0x02)
#define FLASH_ERROR_ADDRESS     (0x03)
#define FLASH_ERROR_BOUNDARY    (0x04)

#define FLASH_FAILURE           (0x06)
#define FLASH_LOCK_BIT_SET      (0x07)
#define FLASH_LOCK_BIT_NOT_SET  (0x08)

/*
 * Function: FLS_Init
 * Description: Initialization of the FLASH Module Clock. Programming and erase operation are allowed
 * Note: Modified from define to function.
 * Status: Test me on Hardware!
 */
void FLS_Init(void)
{
	volatile r_fcl_status_t    status_enu;
    CopyFlashRoutinesToRAM();

    R_FCL_Init();

    status_enu = R_FCL_SetFrequency(FCL_CONFIG_FREQUENCY_MHZ);

    if (R_FCL_OK != status_enu)
    {
		/* An error ocurred. Stop here */
		while (1)
		   ;
    }
}/*FlashInit*/

/*
 * Function: FLS_Ease
 * Description: Call "R_FlashErase(position)" API From Renesas Flash Library
 * Note:
 * Status:
 */
uint8_t FLS_Erase(FLS_positionType address)
{
    r_fcl_status_t    status_enu;
    uint32_t          start_block_u32;
    uint32_t		  block_start_addr_u32;
    block_start_addr_u32 = address & 0xFFFFFC00;

    start_block_u32 = (FLASH_END_ADDRESS - block_start_addr_u32) / FLASH_ERASE_BLK_SZ;
    status_enu = R_FCL_Erase (start_block_u32, start_block_u32);

    FSL_Error=status_enu;

    return status_enu;
}/*end FLS_Erase*/

/*
 * Function: FLS_Programming
 * Description:  The Function call
 * Status:
 *
 */
#define ADDR_MASK	  ((uint32_t)(ROM_PROGRAM_SIZE-1))
uint8_t FLS_Programming(uint8_t* pSource, uint32_t len, FLS_positionType dest)
{
    uint8_t fls_programming_status=0;
    unsigned char *srcPtr=(unsigned char *)pSource;
    unsigned int headOffset=0;
    unsigned int headLen=0;
    unsigned int tailLen=0;
    unsigned int i;
    char writeBuff[FLS_WRITE_SIZE];


	headOffset = (dest & ADDR_MASK);

	/* head ------------------------------------------*/
	if(headOffset > 0)
	{
		headLen = FLS_WRITE_SIZE-headOffset;

		if (headLen > len)
		{
			headLen = len;
		}

		for(i=0; i<FLS_WRITE_SIZE; i++)  /* init write buffer */
		{
			if ( (i < headOffset) || (i >= headOffset+headLen) )
			{
				writeBuff[i] = 0xFF;
			}
			else
			{
				writeBuff[i]=*srcPtr;
				srcPtr++;
			}
		}

		fls_programming_status=R_FCL_Write((uint32_t *)&writeBuff[0], (uint32_t)((uint32_t)dest-headOffset), 1);
	}

	/* body -----------------------------------------*/
	if( !fls_programming_status )
	{
		int writePos= dest+headLen;
		int nByte=len;
		tailLen = (len + headOffset) & ADDR_MASK;
		if(headLen>0)
		{
			nByte -= headLen;
		}
		nByte -= tailLen;
		if(nByte>0)
		{
			fls_programming_status=R_FCL_Write((uint32_t *)srcPtr, (uint32_t)writePos, nByte/FLS_WRITE_SIZE);
			srcPtr += (nByte*FLS_WRITE_SIZE);
		}
	}

	/* tail ------------------------------------------*/
	if( !fls_programming_status )
	{
		unsigned int beginTail = dest+(len-tailLen);
		if( tailLen > 0 && dest < beginTail)
		{
			for(i=0; i<ROM_PROGRAM_SIZE; i++)  /* init write buffer */
			{
				writeBuff[i] = 0xFF;
			}
			for(i=0; i<tailLen; i++)
			{
				writeBuff[i] = *srcPtr++;
			}
			fls_programming_status=R_FCL_Write((uint32_t *)&writeBuff[0], (uint32_t)beginTail, 1);
		}
	}

	FSL_Error = fls_programming_status;
    return fls_programming_status;
}

/*
* Function: FLS_CheckErrors
* Description: Check the Flash Status Error Byte
* Note:
*/

uint8_t FLS_CheckErrors(void)
{
    return FSL_Error;
}/* end FLS_CheckErrors(void) */

/*
* Function: FLS_InitRegsForErasing
* Description: Should clear the last Flash error
* Note: The Flash Status registers are read only
*/
void FLS_InitRegsForErasing(void)
{
}/*end FLS_InitRegsForErasing*/


/*
* Function: FLS_BlankCheck
* Description:
*/
uint8_t FLS_BlankCheck(FLS_sizeType size, FLS_positionType position)
{
    uint8_t result;
    r_fcl_status_t    status_enu;
    uint32_t block_size_u32;

    if((size % 4) != 0){
    	PLT_ERROR_WARNING(PLT_ERROR_OCFLS_BAD_PARAMS);
    }
    if((position & 0x03) != 0 ){
    	PLT_ERROR_WARNING(PLT_ERROR_OCFLS_BAD_PARAMS);
    }

    block_size_u32 = size/4;

    status_enu = R_FCL_BlankCheck (position, block_size_u32);
    result = mapError[status_enu];

    return result;
}








