/*
 * RN_RX200_hal_Flash.h
 *
 *  Created on: 25/feb/2013
 *      Author: marco
 */

#ifndef RN_RX200_HAL_FLASH_H_
#define RN_RX200_HAL_FLASH_H_

/******************************************************************************
Macro defines
******************************************************************************/
/******************************************************************************
 ENABLE ROM PROGRAMMING
******************************************************************************/
/* When uncommented, code is included to program the User ROM program area of
   Flash. Since this code must be executed from within RAM, the sections
   'PFRAM'(ROM) and 'RPFRAM'(RAM) must be added to the linker settings. Also
   the linker option '-rom=PFRAM=RPFRAM' must be added. Finally, the
   initialization of the 'RPFRAM' must be added to the 'dbsct.c' file as such:
   { __sectop("PFRAM"), __secend("PFRAM"), __sectop("RPFRAM") } */
#define ENABLE_ROM_PROGRAMMING

/* Define read mode macro */
#define READ_MODE 0
/* Define ROM PE mode macro */
#define ROM_PE_MODE 1
/* Define data flash PE mode macro */
#define FLD_PE_MODE 2
/*  The number of ICLK ticks needed for 35us delay are calculated below */
#define	WAIT_tRESW ( (35*ICLK_FREQUENCY) / 1000000)
/*  The number of ICLK ticks needed for 10us delay are calculated below */
#define	WAIT_t10USEC ( (10*ICLK_FREQUENCY) / 1000000)

#define GET_BLOCK_ADDR( blockNum )	get_block_addresses(blockNum)

uint32_t get_block_addresses(uint8_t blockNum );
/* Flash intialisation function prototype */
uint8_t _Flash_Init(void);
/* FCU reset function prototype */
#ifdef ENABLE_ROM_PROGRAMMING
void    _Flash_Reset(void) __attribute__((section(".PRAM")));
#else
static void    _Flash_Reset(void);
#endif

extern void CopyFlashRoutinesToRAM(void);

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */

/** @name H.A.L. Flash
 *
 */
/*@{*/

#if defined(HAL_FLS_SUPPORT)
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* Define Flash Capability */

/*Renesas FLASH API Definition*/
#define RX210	1		/* This define is necessary for FLASH Renesas API*/
/* Pointer definitions for what should be sent in to  */

#define FLASH_BASE_ADDRESS	(ROM_PE_ADDR + 0xFF000000)
#define FLASH_END_ADDRESS	0xFFFFFFFF	/* All Flash memory end at this value*/

#ifdef RN_RX200_R5F52Z0_5
#define FLASH_ERASE_BLK_SZ 1024
#else
#define FLASH_ERASE_BLK_SZ 2048
#endif
#define MAX_FLASH_WRITING_SIZE	4000	/* Max size for writing in RAM*/

#define FLASH_DATA_BASE_ADDRESS	(0x00100000)
#define FLASH_DATA_END_ADDRESS	(0x00101FFF)
#define FLASH_DATA_SIZE			(FLASH_DATA_END_ADDRESS-FLASH_DATA_BASE_ADDRESS +1)
/*==============*/
/*PROGRAM ROM SIZE*/
/*==============*/
#if defined(RN_RX200_R5F52Z0_5)
#define ROM_SIZE            0x20000      	/* 128KB*/
#elif defined(RN_RX200_R5F52Z0_6)
#define ROM_SIZE            0x40000      	/* 256KB*/
#else
#define ROM_SIZE            0x80000      	/* 512KB*/
#endif
/*==============*/
/* DATA FLASH SIZE*/
/*==============*/
#define DF_SIZE             8192  /*Specify the size of the Data Flash Area in Bytes*/



#define     FCLK_DIV            (1)		/* This Macro now are used only for flash_module*/
#define     ICLK_DIV            (1)		/* This Macro now are used only for flash_module*/
#define     BCLK_DIV            (2)		/* This Macro now are used only for flash_module*/
#define     PCLK1215_DIV        (64)    /* This Macro now are used only for flash_module*/
#define     PCLKB_DIV           (1)     /* This Macro now are used only for flash_module*/
#define     PCLK47_DIV          (64)    /* This Macro now are used only for flash_module*/
#define     PCLKD_DIV           (1)     /* This Macro now are used only for flash_module*/

#define     FCLK_FREQUENCY      (CLK_INT_CLOCK_FREQ / FCLK_DIV)		/* This Macro now are used only for flash_module*/
#define     ICLK_FREQUENCY      (CLK_INT_CLOCK_FREQ / ICLK_DIV)		/* This Macro now are used only for flash_module*/
#define     BCLK_FREQUENCY      (CLK_INT_CLOCK_FREQ / BCLK_DIV)		/* This Macro now are used only for flash_module*/
#define     PCLKB_FREQUENCY     (CLK_INT_CLOCK_FREQ / PCLKB_DIV)	/* This Macro now are used only for flash_module*/
#define     PCLKD_FREQUENCY     (CLK_INT_CLOCK_FREQ / PCLKD_DIV)	/* This Macro now are used only for flash_module*/

#define FLASH_CLOCK_FREQUENCY FCLK_FREQUENCY



/******************************************************************************
 ENABLE ROM to ROM or DF to DF programs
******************************************************************************/
/* If you are doing ROM to ROM writes or DF to DF writes then this DEFINE
   needs to be uncommented. This is necessary because when programming a ROM
   area the MCU cannot read from ROM and when programming a DF area the MCU
   cannot read from the DF. To get by this, if this DEFINE is uncommented
   then data that is moving from either ROM to ROM or DF to DF is first
   buffered in a RAM array, then written. If you are never doing one of these
   operations then you can comment out this DEFINE and save yourself
   the RAM needed to buffer one ROM write (RX62x = 256 bytes,
   RX630 = 128 bytes). */
/*#define FLASH_TO_FLASH*/

/******************************************************************************
 ENABLE BGO & NON-BLOCKING DATA FLASH OPERATIONS
******************************************************************************/
/* If this is defined then the flash ready interrupt will be used and
   FlashAPI routines that deal with the data flash will exit after the
   operation has been started instead of polling for it to finish. */
/*#define DATA_FLASH_BGO*/

/******************************************************************************
 ENABLE BGO & NON-BLOCKING ROM OPERATIONS
 EXTRA CARE SHOULD BE TAKEN IF YOU ENABLE ROM_BGO. SINCE PROGRAM/ERASE
 FUNCTIONS WILL BE NON-BLOCKING THE API WILL RETURN BEFORE THE ROM OPERATION
 HAS FINISHED. THIS MEANS THAT THE USER CODE THAT CALLS THE FLASH API
 FUNCTION MUST BE IN RAM OR EXTERNAL MEMORY. THIS WOULD ALSO MEAN THAT THE
 USER HAS TO RELOCATE THE INTERRUPT VECTOR TABLE TO SOMEWHERE OTHER THAN
 ROM OR DATA FLASH. IF THE USER ATTEMPTS TO ACCESS ROM DURING A ROM PROGRAM/
 ERASE OPERATION THE FLASH CONTROL UNIT WILL THROW AN ERROR.
******************************************************************************/
/* If this is defined then the flash ready interrupt will be used and
   FlashAPI routines that deal with the on-chip ROM will exit after the
   operation has been started instead of polling for it to finish. */
/*#define ROM_BGO*/
/******************************************************************************
 SET IPL OF FLASH READY INTERRUPT
******************************************************************************/
/* If using BGO then the Flash Ready Interrupt will need to have an IPL. */
#define FLASH_READY_IPL     5

/******************************************************************************
 ENABLE OR DISABLE LOCK BIT PROTECTION
******************************************************************************/
/* Each erasure block has a corresponding lock bit that can be used to
   protect that block from being programmed/erased after the lock bit is
   set. The use of lock bits can be used or ignored. If the #define below
   is uncommented then lock bits will be ignored and programs/erases to a
   block will not be limited. This only applies to ROM since the DF does
   not have lock bits. */
#define IGNORE_LOCK_BITS

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
#ifdef ENABLE_ROM_PROGRAMMING
#if defined(CONFIG_HAL_OVERLAY)
uint8_t R_FlashErase_1(   uint8_t block ) __attribute__((section(".PRAMERASE")));
#define R_FlashErase R_FlashErase_2
#else
uint8_t R_FlashErase_1(   uint8_t block ) __attribute__((section(".PRAM")));
uint8_t R_FlashDataErase(uint32_t pAddr, uint32_t bytes_to_erase) __attribute__((section(".PRAM")));
#define R_FlashErase R_FlashErase_1
#endif
#else
uint8_t R_FlashErase_1(   uint8_t block );
#endif

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
#ifdef ENABLE_ROM_PROGRAMMING
uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes) __attribute__((section(".PRAM")));
#else
uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes);
#endif


/******************************************************************************
* Function Name: R_FlashWrite
* Description  : Writes bytes into flash.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address location to write to. This address
*                             must be on a program boundary (e.g. RX62N has
*                             256-byte ROM writes and 8-byte DF writes).
*                buffer_addr - Address location of data buffer to write
*                              into flash.
*                bytes - The number of bytes to write. You must always pass a
*                        multiple of the programming size (e.g. RX62N has
*                        256-byte ROM writes and 8-byte DF writes).
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_ERROR_ALIGNED = Flash address was not on correct boundary
*                FLASH_ERROR_BYTES = Number of bytes did not match programming
*                                    size of ROM or DF.
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BOUNDARY = (ROM) Cannot write across flash areas.
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
#ifdef ENABLE_ROM_PROGRAMMING
#if defined(CONFIG_HAL_OVERLAY)
uint8_t R_FlashWrite_1(	uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes ) __attribute__((section(".PRAMWRITE")));
#define R_FlashWrite R_FlashWrite_2
#else
uint8_t R_FlashWrite_1(	uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes ) __attribute__((section(".PRAM")));
#define R_FlashWrite R_FlashWrite_1
#endif
#else
uint8_t R_FlashWrite_1(	uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes );
#endif

/******************************************************************************
* Function Name: R_FlashProgramLockBit
* Description  : Programs the lock bit for a specified ROM erasure block. If
*                the lock bit for a block is set and lock bit protection is
*                enabled then that block cannot be programmed/erased.
* Arguments    : block - Which ROM erasure block to set the lock bit for
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
#ifdef ENABLE_ROM_PROGRAMMING
uint8_t R_FlashProgramLockBit(uint8_t block) __attribute__((section(".PRAM")));
#else
uint8_t R_FlashProgramLockBit(uint8_t block);
#endif

/******************************************************************************
* Function Name: R_FlashReadLockBit
* Description  : Reads and returns the lock bit status for a ROM block
* Arguments    : block - Which ROM erasure block to read the lock bit of
* Return Value : FLASH_LOCK_BIT_SET = Lock bit was set
*                FLASH_LOCK_BIT_NOT_SET = Lock bit was not set
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
#ifdef ENABLE_ROM_PROGRAMMING
uint8_t R_FlashReadLockBit(uint8_t block) __attribute__((section(".PRAM")));
#else
uint8_t R_FlashReadLockBit(uint8_t block);
#endif

/******************************************************************************
* Function Name: R_FlashSetLockBitProtection
* Description  : Enables or disables lock bit protection.
* Arguments    : lock_bit - Boolean, whether to enable or disable lock bit
*                           protection (true = enabled, false = disabled)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
#ifdef ENABLE_ROM_PROGRAMMING
uint8_t R_FlashSetLockBitProtection(uint8_t lock_bit) __attribute__((section(".PRAM")));
#else
uint8_t R_FlashSetLockBitProtection(uint8_t lock_bit);
#endif

/******************************************************************************
* Function Name: R_FlashGetStatus
* Description  : Returns the current state of the flash
* Arguments    : none
* Return Value : FLASH_SUCCESS = Flash is ready to use
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
#ifdef ENABLE_ROM_PROGRAMMING
uint8_t R_FlashGetStatus(void) __attribute__((section(".PRAM")));
#else
uint8_t R_FlashGetStatus(void);
#endif
/* Data Flash Only Functions */

/******************************************************************************
* Function Name: R_FlashDataAreaAccess
* Description  : This function is used to allow read and program permissions
*                to the Data Flash areas. NOTE: This function does not have to
*                execute from in RAM.
* Arguments    : read_en_mask - Bitmasked value. Bits 0-3 represents each Data
*                               Blocks 0-3 (respectively).
*                               '0'=no Read access.
*                               '1'=Allows Read by CPU
*                write_en_mask - Bitmasked value. Bits 0-3 represents each Data
*                               Blocks 0-3 (respectively).
*                               '0'=no Erase/Write access.
*                               '1'=Allows Erase/Write by FCU
* Return Value : none
******************************************************************************/
void R_FlashDataAreaAccess(uint16_t read_en_mask, uint16_t write_en_mask);

/******************************************************************************
* Function Name: R_FlashDataAreaBlankCheck
* Description  : Performs a blank check on a specified data flash block
*                NOTE1: This function does not have to execute from in RAM.
*                NOTE2: RX610 & RX62x support block and 8 byte checks
*                       RX630/631/63N support block and 2 byte checks
* Arguments    : address - The address to check if is blank.
*                          If the parameter 'size'=='BLANK_CHECK_8_BYTE',
*                          this should be set to an 8-byte address boundary.
*                          If the parameter 'size'=='BLANK_CHECK_ENTIRE_BLOCK',
*                          this should be set to a defined Data Block Number
*                          ('BLOCK_DB0', 'BLOCK_DB1', etc...) or an address
*                          in the data flash block.  Either option will work.
*                          If the parameter 'size'=='BLANK_CHECK_2_BYTE',
*                          this should be set to a 2 byte address boundary.
*                size - This specifies if you are checking an 8-byte location,
*                       2-byte location, or an entire block. You must set this
*                       to either 'BLANK_CHECK_8_BYTE', 'BLANK_CHECK_2_BYTE',
*                       or 'BLANK_CHECK_ENTIRE_BLOCK'.
* Return Value : FLASH_BLANK = (2 or 8 Byte check or non-BGO) Blank
*                              (Entire Block & BGO) Blank check operation
*                              started
*                FLASH_NOT_BLANK = Not Blank
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
*                FLASH_ERROR_ADDRESS = Invalid address
*                FLASH_ERROR_BYTES = Incorrect 'size' was submitted
******************************************************************************/
uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size);

/******************************************************************************
ROM FLASH CONFIGURATION DEFINE
******************************************************************************/
#define BLANK_CHECK_ENTIRE_BLOCK 0 	/* 'size' parameter for R_FlashDataAreaBlankCheck*/
#define BLANK_CHECK_2_BYTE       2	/* 'size' parameter for R_FlashDataAreaBlankCheck*/



/*  According to HW Manual the Max Programming Time for 128 bytes (ROM)
	is 12ms.  This is with a FCLK of 50MHz. The calculation below
	calculates the number of ICLK ticks needed for the timeout delay.
	The 12ms number is adjusted linearly depending on the FCLK frequency.
*/
#define	WAIT_MAX_ROM_WRITE ((int32_t)(12000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

/*  According to HW Manual the Max Programming Time for 2 bytes
	(Data Flash) is 2ms.  This is with a FCLK of 50MHz. The calculation
	below calculates the number of ICLK ticks needed for the timeout delay.
	The 5ms number is adjusted linearly depending on the FCLK frequency.
*/
#define	WAIT_MAX_DF_WRITE ((int32_t)(2000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

/*  According to HW Manual the Max Blank Check time for 2k bytes
	(Data Flash) is 0.7ms.  This is with a FCLK of 50MHz. The calculation
	below calculates the number of ICLK ticks needed for the timeout delay.
	The 0.7ms number is adjusted linearly depending on the FCLK frequency.
*/
#define	WAIT_MAX_BLANK_CHECK \
		((int32_t)(700 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

/*  According to HW Manual the max timeout value when using the peripheral
	clock notification command is 60us. This is with a FCLK of 50MHz. The
	calculation below calculates the number of ICLK ticks needed for the
	timeout delay. The 10us number is adjusted linearly depending on
	the FCLK frequency.
*/
#define	WAIT_MAX_NOTIFY_PERIPHERAL_CLOCK \
		(int32_t)((60 * ICLK_FREQUENCY * 50.0) / FLASH_CLOCK_FREQUENCY)

/* FCU-RAM address define */
/* FCU F/W Store Address */
#define FCU_PRG_TOP		0xFEFFE000
/* FCU RAM Address */
#define FCU_RAM_TOP		0x007F8000
/* FCU RAM Size */
#define FCU_RAM_SIZE	0x2000

#define NUM_ROM_AREAS		1	/* Defines the number of flash area*/

/** Defines the start program/erase address for the different flash areas **/
#if defined(RN_RX200_R5F52Z0_5)
#define ROM_AREA_0          	(0x00FE0000)
#define ROM_AREA_START_ADDRESS	0xFFFE0000
#define ROM_AREA_END_ADDRESS	0xFFFFFFFF
#define ROM_BLOCK_SIZE			1024
#elif defined(RN_RX200_R5F52Z0_6)
#define ROM_AREA_0          	(0x00FC0000)
#define ROM_AREA_START_ADDRESS	0xFFFC0000
#define ROM_AREA_END_ADDRESS	0xFFFFFFFF
#define ROM_BLOCK_SIZE			2048
#else
#define ROM_AREA_0          	(0x00F80000)
#define ROM_AREA_START_ADDRESS	0xFFF80000
#define ROM_AREA_END_ADDRESS	0xFFFFFFFF
#define ROM_BLOCK_SIZE			2048
#endif

#if ROM_SIZE == 2097152
/*  Bottom of User Flash Area */
#define ROM_PE_ADDR     0x00E00000		/*2MB part*/
/* Defines how many ROM blocks are on this part */
#define ROM_NUM_BLOCKS  70				/*2MB part*/
#elif ROM_SIZE == 1048576
/*  Bottom of User Flash Area */
#define ROM_PE_ADDR     0x00F00000		/*1MB part*/
/* Defines how many ROM blocks are on this part */
#define ROM_NUM_BLOCKS  54				/*1MB part*/
#elif ROM_SIZE == 524288
/*  Bottom of User Flash Area */
#define ROM_PE_ADDR     0x00F80000		/*512KB part*/
/* Defines how many ROM blocks are on this part */
#define ROM_NUM_BLOCKS  128				/*512KB part*/
#elif ROM_SIZE == 262144
#define ROM_PE_ADDR     0x00FC0000		/*256KB part*/
/* Defines how many ROM blocks are on this part */
#define ROM_NUM_BLOCKS  128				/*256KB part*/
#elif ROM_SIZE == 131072
/*  Bottom of User Flash Area */
#define ROM_PE_ADDR     0x00FE0000		/*128KB part*/
/* Defines how many ROM blocks are on this part */
#define ROM_NUM_BLOCKS  128				/*128KB part*/
#endif

/* Programming size for ROM in bytes */
#define ROM_PROGRAM_SIZE_LARGE    128
#define ROM_PROGRAM_SIZE_MEDIUM   8
#define ROM_PROGRAM_SIZE_SMALL    2
#define ROM_PROGRAM_SIZE    ROM_PROGRAM_SIZE_MEDIUM	/*ROM Programming Line Sizie: 2,8,128*/
#define FLS_WRITE_SIZE      ROM_PROGRAM_SIZE
/***************************************/
/** DATA FLASH CONFIGURATION DEFINE ****/
/***************************************/
/* NOTE:
   The RX210 actually has 64 x 128 byte blocks instead of the
   16 x 2Kbyte blocks shown below. These are grouped into 4 blocks to
   make it easier for the user to delete larger sections of the data
   flash. The user can still delete individual blocks but they will
   need to use the new flash erase function that takes addresses
   instead of blocks. */

/* Data Flash Block Area  */
#define BLOCK_DB0    128   /*    first block data flash 0x00100000 */

/*  Bottom of DF Area */
#define DF_ADDRESS      0x00100000
#define DF_ADDRSS_END	0x00102000

/* Clone Block Define */
#define CLONE_START_WORD_POS 	4
#define CLONE_START_WORD_VALUE	0xC5
#define CLONE_BLOCK_SIZE		128				/* !< Size of clone block in Byte */
#define CLONE_BLOCK_ADDRESS			(DF_ADDRSS_END - (CLONE_BLOCK_SIZE*2))   /* !< Specify the first clone block */
#define CLONE_TRANS_BLOCK_ADDRESS	(DF_ADDRSS_END - (CLONE_BLOCK_SIZE))	 /* !< Specify the second clone block */
/* Used for getting DF block */
#define DF_MASK         0xFFFFF800
/* Used for getting 2 byte boundary in DF block when doing blank checking */

#define DF_BLOCK_SIZE   		0x80
#define DF_BLOCK_SIZE_LARGE     0x800	/* This is used to get the boundary of the 'fake' blocks that are 2KB*/
#define DF_NUM_BLOCKS   	    4		    /*Defines how many DF blocks are on this part*/
#define DF_PROGRAM_SIZE_SMALL   2		    /* Programming size for data flash in bytes*/
#define DF_PROGRAM_SIZE_LARGE   8

/*  According to HW Manual the Max Erasure Time for a 64kB block is
around 1152ms.  This is with a FCLK of 50MHz. The calculation below
calculates the number of ICLK ticks needed for the timeout delay.
The 1152ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE 	((int32_t)(1152000 * (50.0/FLASH_CLOCK_FREQUENCY))*ICLK_FREQUENCY)

/* 63 is the max. avaible value for flash module clock divisor */
#if ((BUS_FREQUENCY_Hz / FLS_MODULE_FREQUENCY_Hz) > 63)
#define FLS_FLG_PRDIV8	1
#define FLS_DIV ((BUS_FREQUENCY_Hz / 8 / FLS_MODULE_FREQUENCY_Hz) - 1)
#if (FLS_DIV > 63)
#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor too high!"
#endif
#else
#define FLS_FLG_PRDIV8	0
#define FLS_DIV ((BUS_FREQUENCY_Hz / FLS_MODULE_FREQUENCY_Hz) - 1)
#if (FLS_DIV < 0)
#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor is too low!"
#endif
#endif

#define REAL_FLS_MODULE_FREQUENCY_Hz (BUS_FREQUENCY_Hz/(FLS_FLG_PRDIV8*(FLS_DIV+1)))
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
 * Function: FLS_DataFlashClone
 * Description:
 * Status:
 *
 */
IO_ErrorType FLS_DataFlashClone(FLS_positionType alineated_address, uint32_t pSource, uint32_t len, uint8_t headOffset);

/**
* Function: FLS_BlankCheck
* Description:
*/
IO_ErrorType FLS_BlankCheck (FLS_sizeType size, FLS_positionType position);
uint8_t BlankCheckDataFlash(FLS_positionType dest, FLS_sizeType len);
#define FLS_BlankCheck FLS_BlankCheck
/**
* Function: R_FlashErase_Overlay
*/
uint8_t R_FlashErase_2(uint8_t block);
/**
* Function: R_FlashWrite_Overlay
*/
uint8_t R_FlashWrite_2 ( uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes );

#endif /* #if defined(HAL_FLS_SUPPORT) */
/*@}*/


#endif /* RN_RX200_HAL_FLASH_H_ */
