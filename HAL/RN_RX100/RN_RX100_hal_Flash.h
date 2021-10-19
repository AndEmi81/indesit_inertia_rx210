/*
 * RN_RX100_hal_Flash.h
 *
 *  Created on: 25/feb/2013
 *      Author: marco
 */

#ifndef RN_RX100_HAL_FLASH_H_
#define RN_RX100_HAL_FLASH_H_


extern void CopyFlashRoutinesToRAM(void);

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */

/** @name H.A.L. Flash
 *
 */
/*@{*/

#if defined(HAL_FLS_SUPPORT)
/******************************************************************************
Function Return Values
******************************************************************************/
/* Operation was successful */
#define FLASH_SUCCESS           (0x00)
/* Flash area checked was blank, making this 0x00 as well to keep existing
   code checking compatibility */
#define FLASH_BLANK             (0x00)
/* The address that was supplied was not on aligned correctly for ROM or DF */
#define FLASH_ERROR_ALIGNED     (0x01)
/* Flash area checked was not blank, making this 0x01 as well to keep existing
   code checking compatibility */
#define FLASH_NOT_BLANK         (0x01)
/* The number of bytes supplied to write was incorrect */
#define FLASH_ERROR_BYTES       (0x02)
/* The address provided is not a valid ROM or DF address */
#define FLASH_ERROR_ADDRESS     (0x03)
/* Writes cannot cross the 1MB boundary on some parts */
#define FLASH_ERROR_BOUNDARY    (0x04)
/* Flash is busy with another operation */
#define FLASH_BUSY              (0x05)
/* Operation failed */
#define FLASH_FAILURE           (0x06)
/* Lock bit was set for the block in question */
#define FLASH_LOCK_BIT_SET      (0x07)
/* Lock bit was not set for the block in question */
#define FLASH_LOCK_BIT_NOT_SET  (0x08)


/* ****************** */
/* Defines and Macros */
/* ****************** */
/* Define Flash Capability */

/*Renesas FLASH API Definition*/

/* Pointer definitions for what should be sent in to  */
#define ROM_PE_ADDR     	0x00FE0000		/*128KB part*/
#define FLASH_BASE_ADDRESS	(ROM_PE_ADDR + 0xFF000000)
#define FLASH_END_ADDRESS	0xFFFFFFFF	/* All Flash memory end at this value*/
#define FLASH_ERASE_BLK_SZ  1024

#if 0
#define FLASH_DATA_BASE_ADDRESS	(0x00100000)
#define FLASH_DATA_END_ADDRESS	(0x00101FFF)
#define FLASH_DATA_SIZE			(FLASH_DATA_END_ADDRESS-FLASH_DATA_BASE_ADDRESS +1)
#endif
/*=============================*/
/* PROGRAM ROM SIZE AND LAYOUT */
/*=============================*/

#define ROM_AREA_END_ADDRESS	0xFFFFFFFF
#define ROM_BLOCK_SIZE			1024

#if defined(RN_RX100_R5F5111)
	#if defined(RN_RX100_R5F5111_5)
	#define ROM_SIZE				0x00020000   /* 128 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFE0000
	#define ROM_NUM_BLOCKS  		128

	#elif defined(RN_RX100_R5F5111_4)
	#define ROM_SIZE				0x00018000	 /* 96 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFE8000
	#define ROM_NUM_BLOCKS  		96

	#elif defined(RN_RX100_R5F5111_3)
	#define ROM_SIZE				0x00010000	 /* 64 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFF0000
	#define ROM_NUM_BLOCKS  		64


	#elif defined(RN_RX100_R5F5111_1)
	#define ROM_SIZE				0x00008000	 /* 32 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFF8000
	#define ROM_NUM_BLOCKS  		32

	#endif
#elif defined(RN_RX100_R5F5110)
	#if defined(RN_RX100_R5F5110_5)
	#define ROM_SIZE				0x00020000   /* 128 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFE0000
	#define ROM_NUM_BLOCKS  		128

	#elif defined(RN_RX100_R5F5110_4)
	#define ROM_SIZE				0x00018000	 /* 96 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFE8000
	#define ROM_NUM_BLOCKS  		96

	#elif defined(RN_RX100_R5F5110_3)
	#define ROM_SIZE				0x00010000	 /* 64 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFF0000
	#define ROM_NUM_BLOCKS  		64

	#elif defined(RN_RX100_R5F5110_1)
	#define ROM_SIZE				0x00008000	 /* 32 KB */
	#define ROM_AREA_START_ADDRESS	0xFFFF8000
	#define ROM_NUM_BLOCKS  		32

	#endif
#endif /* defined(RN_RX100_R5F5111) */


/* Programming size for ROM in bytes */
#define ROM_PROGRAM_SIZE    4
#define FLS_WRITE_SIZE      ROM_PROGRAM_SIZE


/******************************************************************************
Function Prototype
******************************************************************************/
/******************************************************************************
* Function Name: R_FlashErase
* Description  : Erases an entire flash block.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : block - The block number to erase (BLOCK_0, BLOCK_1, etc...)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashErase(   uint8_t block ) __attribute__((section(".PRAM")));

/******************************************************************************
* Function Name: R_FlashEraseRange
* Description  : Erases blocks starting at an address and stopping when the
*                number of bytes to erase has been reached.
*                NOTE: This function is currently only for data flash blocks
*                on RX MCUs that have 32 byte erase sectors.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : start_addr - The address of where to start erasing. Must be on
*                             erase boundary.
*                bytes - The numbers of bytes to erase. Must be
*                        multiple of erase size.
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = Flash address was not on correct boundary
*                FLASH_BUSY = Another flash operation is in progress
*                FLASH_ERROR_BYTES = Number of bytes did not match erase size
*                FLASH_ERROR_ADDRESS = Invalid address, this is only for DF
******************************************************************************/

uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes) __attribute__((section(".PRAM")));
uint8_t R_FlashWrite(	uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes ) __attribute__((section(".PRAM")));







/*
 * Function: FLS_Init
 * Description: Initialization of the FLASH Module Clock. Programming and erase operation are allowed
 * Note:
 */
void FLS_Init(void);

/*
 * Function: FLS_Erase
 * Description: cancella un blocco di flash
 * Note:
 */
uint8_t FLS_Erase(FLS_positionType address);

void FLS_InitRegsForErasing(void);
/*
 Abort any command and clear errors
 */
void FLS_InitRegsForEraseVerifying(void);
#define FLS_InitRegsForEraseVerifying()	{}	/* non necessaria l'implementazione */

/*
 Abort any command and clear errors
 */
void FLS_InitRegsForProgramming(void);
#define FLS_InitRegsForProgramming()	{}  /* non necessaria l'implementazione */

uint8_t FLS_CheckErrors(void);


/**
 * Function: FLS_Programming
 * Description:
 * Status:
 *
 */
uint8_t FLS_Programming(uint8_t* pSource, uint32_t size, FLS_positionType address);

/**
 * Function: FLS_ProgrammingDAta
 * Description:
 * Status:
 *
 */
uint8_t FLS_ProgrammingData(uint8_t* pSource, uint32_t len, FLS_positionType dest);



/**
* Function: FLS_BlankCheck
* Description:
*/
IO_ErrorType FLS_BlankCheck (FLS_sizeType size, FLS_positionType position);
#define FLS_BlankCheck FLS_BlankCheck


#endif /* #if defined(HAL_FLS_SUPPORT) */
/*@}*/


#endif /* RN_RX100_HAL_FLASH_H_ */
