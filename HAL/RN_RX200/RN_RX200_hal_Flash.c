/**
 HAL Functions of Flash Device
 Hal Functions to access at hardware Resources from Driver layer
 \file    RN_RX200_Hal_Flash.c
 */
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "RN_RX200_hal.h"
#include "RN_RX200_hal_Flash.h"


/******************************************************************************
Private global variables and functions
******************************************************************************/

/* Signals whether FCU firmware has been transferred to the FCU RAM
0 : No, 1 : Yes */
static uint8_t fcu_transfer_complete = 0;

/* Valid values are 'READ_MODE','ROM_PE_MODE' or 'FLD_PE_MODE' */
static uint8_t current_mode;

/* Stores whether the peripheral clock notification command has
been executed */
static uint8_t fcu_pclk_command = 0;

/* Semaphore for making sure flash routines do not stomp on each other */
static int32_t flash_semaphore = 0;

/* States for flash operations */
static enum Flash_States { FLASH_READY,
                           FLASH_ERASING,
                           FLASH_WRITING,
                           FLASH_BLANKCHECK,
                           FLASH_LOCK_BIT
                         } flash_state;

#ifndef IGNORE_LOCK_BITS
/* Determines whether lock bit protection is used when programming/erasing */
static uint8_t lock_bit_protection = TRUE;
#endif

uint32_t get_block_addresses(uint8_t blockNum )
{
    uint32_t retVal;

    if( blockNum < ROM_NUM_BLOCKS)
    {
        retVal = (0x00FFFFFF-FLASH_ERASE_BLK_SZ +1)- (FLASH_ERASE_BLK_SZ*blockNum);
    }
    else
    {
        retVal = 0x00100000 + (FLASH_ERASE_BLK_SZ*(blockNum-(ROM_NUM_BLOCKS)));
    }
    return retVal;
}

/* Enter PE mode function prototype */
#ifdef ENABLE_ROM_PROGRAMMING
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes ) __attribute__((section(".PRAM")));
#else
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes );
#endif
/* Exit PE mode function prototype */
#ifdef ENABLE_ROM_PROGRAMMING
static void _Exit_PE_Mode(void) __attribute__((section(".PRAM")));
#else
static void _Exit_PE_Mode(void);
#endif
/* ROM write function prototype */
#ifdef ENABLE_ROM_PROGRAMMING
static uint8_t _ROM_Write(uint32_t address, uint32_t data) __attribute__((section(".PRAM")));
#else
static uint8_t _ROM_Write(uint32_t address, uint32_t data);
#endif
/* Data flash write function prototype */
static uint8_t _DataFlash_Write(uint32_t address, uint32_t data, uint8_t size);
/* Data flash status clear function prototype */
static void _DataFlash_Status_Clr(void);
/* Notify peripheral clock function prototype */
#ifdef ENABLE_ROM_PROGRAMMING
static uint8_t _Notify_Peripheral_Clock(volatile uint8_t * flash_addr) __attribute__((section(".PRAM")));
#else
static uint8_t _Notify_Peripheral_Clock(volatile uint8_t * flash_addr);
#endif

/* Used to grab flash state */
#ifdef ENABLE_ROM_PROGRAMMING
static uint8_t _FlashGrabState(enum Flash_States new_state) __attribute__((section(".PRAM")));
#else
static uint8_t _FlashGrabState(enum Flash_States new_state);
#endif
/* Used to release flash state */
#ifdef ENABLE_ROM_PROGRAMMING
static void _FlashReleaseState(void) __attribute__((section(".PRAM")));
#else
static void _FlashReleaseState(void);
#endif
/* Used to issue an erase command to the FCU */
#ifdef ENABLE_ROM_PROGRAMMING
static uint8_t _Flash_Erase_Command(volatile uint8_t * const erase_addr) __attribute__((section(".PRAM")));
#else
static uint8_t _Flash_Erase_Command(volatile uint8_t * const erase_addr);
#endif

/******************************************************************************
* Function Name: _Flash_Init
* Description  : Initializes the FCU peripheral block.
*                NOTE: This function does not have to execute from in RAM.
* Arguments    : none
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
uint8_t _Flash_Init(void)
{
    /* Declare source and destination pointers */
    uint32_t * src, * dst;

    /* Declare iteration counter variable */
    uint16_t i;

    /* Disable FCU interrupts in FCU block */
    FLASH.FAEINT.BIT.ROMAEIE = 0;
    FLASH.FAEINT.BIT.CMDLKIE = 0;
    FLASH.FAEINT.BIT.DFLAEIE = 0;
    FLASH.FAEINT.BIT.DFLRPEIE = 0;
    FLASH.FAEINT.BIT.DFLWPEIE = 0;

    /* Disable FCU interrupts in ICU */

    /* Disable flash interface error (FIFERR) */
    IPR(FCU, FIFERR) = 0;
    IEN(FCU, FIFERR) = 0;

    /* Disable flash ready interrupt (FRDYI) */
    IPR(FCU, FRDYI) = 0;
    IEN(FCU, FRDYI) = 0;

    /* Transfer Firmware to the FCU RAM. To use FCU commands, the FCU firmware
        must be stored in the FCU RAM. */

    /* Before writing data to the FCU RAM, clear FENTRYR to stop the FCU. */
    if(FLASH.FENTRYR.WORD != 0x0000)
    {
        /* Disable the FCU from accepting commands - Clear both the
           FENTRY0(ROM) and FENTRYD(Data Flash) bits to 0 */
        FLASH.FENTRYR.WORD = 0xAA00;
    }

    /* Enable the FCU RAM */
    FLASH.FCURAME.WORD = 0xC401;

    /* Copies the FCU firmware to the FCU RAM.
       Source: H'FEFFE000 to H'FF00000 (FCU firmware area)
       Destination: H'007F8000 to H'007FA000 (FCU RAM area) */
    /* Set source pointer */
    src = (uint32_t *)FCU_PRG_TOP;

    /* Set destination pointer */
    dst = (uint32_t *)FCU_RAM_TOP;

    /* Iterate for loop to copy the FCU firmware */
    for( i=0; i<(FCU_RAM_SIZE/4); i++)
    {
        /* Copy data from the source to the destination pointer */
        *dst = *src;

        /* Increment the source and destination pointers */
        src++;
        dst++;
    }

    /* FCU firmware transfer complete, set the flag to 1 */
    fcu_transfer_complete = 1;

    /* Return no errors */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _Flash_Init
******************************************************************************/

/******************************************************************************
* Function Name: _DataFlash_Status_Clr
* Description  : Clear the status of the Data Flash operation.
*	             NOTE: This function does not have to execute from in RAM.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _DataFlash_Status_Clr(void)
{
    /* Declare temporaty pointer */
    volatile uint8_t *ptrb;

    /* Set pointer to Data Flash to issue a FCU command if needed */
    ptrb = (volatile uint8_t *)(DF_ADDRESS);

    /* Check to see if an error has occurred with the FCU.  If set, then
       issue a status clear command to bring the FCU out of the
       command-locked state */
    if(FLASH.FSTATR0.BIT.ILGLERR == 1)
    {
        /* FASTAT must be set to 0x10 before the status clear command
           can be successfully issued  */
        if(FLASH.FASTAT.BYTE != 0x10)
        {
            /* Set the FASTAT register to 0x10 so that a status clear
               command can be issued */
            FLASH.FASTAT.BYTE = 0x10;
        }
    }

    /* Issue a status clear command to the FCU */
    *ptrb = 0x50;
}
/******************************************************************************
End of function  _DataFlash_Status_Clr
******************************************************************************/


/******************************************************************************
* Function Name: _DataFlash_Write
* Description  : Write either bytes to Data Flash area.
*	             NOTE: This function does not have to execute from in RAM.
* Arguments    : address - The address (in the Data Flash programming area)
*                          to write the data to
*                data - The address of the data to write
*                size - The size of the data to write. Must be set to
*                       either DF_PROGRAM_SIZE_LARGE or DF_PROGRAM_SIZE_SMALL.
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _DataFlash_Write(uint32_t address, uint32_t data, uint8_t size)
{
    /* Declare wait counter variable */
    volatile uint32_t wait_cnt;

    /* Define loop iteration count variable */
    uint8_t n = 0;

    /* Check data size is valid */
#if defined(DF_PROGRAM_SIZE_LARGE)
    if((DF_PROGRAM_SIZE_LARGE == size) || (DF_PROGRAM_SIZE_SMALL == size))
#else
    if(DF_PROGRAM_SIZE_SMALL == size)
#endif
    {
        /* Perform bit shift since 2 bytes are written at a time */
        size = size >> 1;

        /* Send command to data flash area */
        *(volatile uint8_t *)DF_ADDRESS = 0xE8;

        /* Specify data transfer size to data flash area */
        *(volatile uint8_t *)DF_ADDRESS = size;

        /* Iterate through the number of data bytes */
        while(n++ < size)
        {
            /* Copy data from source address to destination area */
            *(volatile uint16_t *)address = *(uint16_t *)data;

            /* Increment data address by two bytes */
            data += 2;
        }

        /* Write the final FCU command for programming */
        *(volatile uint8_t *)(DF_ADDRESS) = 0xD0;

        /* Set the wait counter with timeout value */
        wait_cnt = WAIT_MAX_DF_WRITE;

        /* Check if FCU has completed its last operation */
        while(FLASH.FSTATR0.BIT.FRDY == 0)
        {
            /* Decrement the wait counter */
            wait_cnt--;

            /* Check if the wait counter has reached zero */
            if(wait_cnt == 0)
            {
                /* Maximum time for writing a block has passed,
                   operation failed, reset FCU */
                _Flash_Reset();

                /* Return FLASH_FAILURE, operation failure */
                return FLASH_FAILURE;
            }
        }

        /* Check for illegal command or programming errors */
        if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR == 1))
        {
            /* Return FLASH_FAILURE, operation failure */
            return FLASH_FAILURE;
        }
    }
    /* Data size is invalid */
    else
    {
        /*Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Return FLASH_SUCCESS, operation success */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _DataFlash_Write
******************************************************************************/


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
uint8_t R_FlashDataAreaBlankCheck(uint32_t address, uint8_t size)
{
    /* Declare data flash pointer */
    volatile uint8_t *ptrb;

    /* Declare result container variable */
    uint8_t result = FLASH_NOT_BLANK;

    /* Declare wait counter variable */
    volatile uint32_t wait_cnt;


	FLS_EnterCriticalSection();

    /* Check to make sure address is valid. */
    if( ((address > (DF_NUM_BLOCKS+BLOCK_DB0)) && (address < DF_ADDRESS))
            || (address > DF_ADDRESS + DF_SIZE) )
    {
        /* Address is not a valid DF address or block number */
    	result = FLASH_ERROR_ADDRESS;
    }
    /* Check to make sure 'size' parameter is valid */
    else if( (size != BLANK_CHECK_ENTIRE_BLOCK)
#if defined(BLANK_CHECK_8_BYTE)
            && (size != BLANK_CHECK_8_BYTE)
#elif defined(BLANK_CHECK_2_BYTE)
            && (size != BLANK_CHECK_2_BYTE)
#endif
      )
    {
        /* 'size' parameter is not valid. */
    	result = FLASH_ERROR_BYTES;
    }
    /* Attempt to grab state */
    else if( _FlashGrabState(FLASH_BLANKCHECK) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
    	result = FLASH_BUSY;
    }
    else
    {
        /* Set current FCU mode to Data Flash PE Mode */
        current_mode = FLD_PE_MODE;

        /* Enter Data Flash PE mode in the FCU */
        if( _Enter_PE_Mode( address, 0 ) != FLASH_SUCCESS)
        {
            /* Release state */
            _FlashReleaseState();

            /* Return FLASH_FAILURE, operation failure */
            result = FLASH_FAILURE;
        }
        else
        {
            /* Set  bit FRDMD (bit 4) in FMODR to 1 */
            FLASH.FMODR.BIT.FRDMD = 1;

            /* Check if 2 byte size has been passed */
            if(size == BLANK_CHECK_2_BYTE)
            {
                /* Set data flash pointer to beginning of the memory block */
                ptrb = (volatile uint8_t *)(address & DF_MASK);

                /* Check if the next 2 bytes are blank
                   Bits BCADR to the address of the 2-byte location to check.
                   Set bit BCSIZE in EEPBCCNT to 0. */
                FLASH.DFLBCCNT.WORD = (uint16_t)(address & (DF_BLOCK_SIZE_LARGE-2));
            }
            else
            {
                /* Check entire data block */
                /* Check to see if user sent in a data block number or the
                   address.  The function description tells the user to send
                   in the block number but in some early examples the address
                   was used. To make this work with both versions we detect which
                   is sent in below and make it work either way. */
                if( address < (DF_NUM_BLOCKS+BLOCK_DB0) )
                {
                	uint32_t tmp_addr = GET_BLOCK_ADDR(address);

                    /* A data block number was sent in */
                    ptrb = (uint8_t *)tmp_addr;
                }
                else
                {
                    /* Any address in the erasure block */
                    ptrb = (volatile uint8_t *)address;
                }

                /* Check if the entire block is blank
                   Set bit BCSIZE in EEPBCCNT to 1. */
                FLASH.DFLBCCNT.BIT.BCSIZE = 1;
            }

            /* Send commands to FCU */
            *ptrb = 0x71;
            *ptrb = 0xD0;

            /* Set timeout wait counter value */
            wait_cnt = WAIT_MAX_BLANK_CHECK;

            /* Wait until FCU operation finishes, or a timeout occurs */
            while(FLASH.FSTATR0.BIT.FRDY == 0)
            {
                /* Increment the wait counter */
                wait_cnt--;

                /* Check if the wait counter has reached zero */
                if(wait_cnt == 0)
                {
                    /* Maximum timeout duration for writing to ROM has elapsed -
                       assume operation failure and reset the FCU */
                    _Flash_Reset();

                    /* Return FLASH_FAILURE, operation failure */
                    result = FLASH_FAILURE;
                }
            }
        }

    }


    if (result == FLASH_NOT_BLANK)
    {
        /* Reset the FRDMD bit back to 0 */
        FLASH.FMODR.BIT.FRDMD = 0x00;

        /* Check if the 'ILGERR' was set during the command */
        if(FLASH.FSTATR0.BIT.ILGLERR == 1)	/* Check 'ILGERR' bit */
        {
            /* Return FLASH_FAILURE, operation failure */
            result = FLASH_FAILURE;
        }
        else
        {
            /* (Read the 'BCST' bit (bit 0) in the 'DFLBCSTAT' register
               0=blank, 1=not blank */
            result = (FLASH.DFLBCSTAT.BIT.BCST ? FLASH_NOT_BLANK : FLASH_BLANK);

        }


    }

    if (current_mode == FLD_PE_MODE)
    {
        /* Take the FCU out of PE mode */
        _Exit_PE_Mode( );

        /* Release state */
        _FlashReleaseState();
    }

    FLS_ExitCriticalSection();

    return result;

}
/******************************************************************************
End of function  R_FlashDataAreaBlankCheck
******************************************************************************/

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
void R_FlashDataAreaAccess(uint16_t read_en_mask, uint16_t write_en_mask)
{
    /* Set Read access for the Data Flash blocks DB0-DB7 */
    FLASH.DFLRE0.WORD = 0x2D00 | (read_en_mask & 0x00FF);

    /* Set Erase/Program access for the Data Flash blocks DB0-DB7 */
    FLASH.DFLWE0.WORD = 0x1E00 |  (write_en_mask & 0x00FF);
}
/******************************************************************************
End of function  R_FlashDataAreaAccess
******************************************************************************/


/******************************************************************************
* Function Name: _ROM_Write
* Description  : Write bytes to ROM Area Flash.
*                NOTE: This function MUST execute from in RAM.
* Arguments    : address - ROM address of where to write to
*                data - Pointer to the data to write
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _ROM_Write(uint32_t address, uint32_t data)
{
    /* Declare iteration loop count variable */
    uint8_t i;
    /* Declare wait counter variable */
    volatile uint32_t wait_cnt;
    /* Number of bytes to write */
    uint32_t size = ROM_PROGRAM_SIZE;

    /* Writes are done 16-bit at a time, scale 'size' argument */
    size = ROM_PROGRAM_SIZE >> 1;

    /* Write the FCU Program command */
    *(volatile uint8_t *)address = 0xE8;
    *(volatile uint8_t *)address = size;

    /* Write 'size' bytes into flash, 16-bits at a time */
    for(i = 0; i < size; i++)
    {
        /* Copy data from source address to destination ROM */
        *(volatile uint16_t *)address = *(uint16_t *)data;

        /* Increment destination address by 2 bytes */
        data += 2;
    }

    /* Write the final FCU command for programming */
    *(volatile uint8_t *)address = 0xD0;

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_ROM_WRITE;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Decrement the wait counter */
        wait_cnt--;

        /* Check if the wait counter has reached zero */
        if(wait_cnt == 0)
        {
            /* Maximum timeout duration for writing to ROM has elapsed -
               assume operation failure and reset the FCU */
            _Flash_Reset();

            /* Return FLASH_FAILURE, operation failure */
            return FLASH_FAILURE;
        }
    }

    /* Check for illegal command or programming errors */
    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR  == 1))
    {
        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Return FLASH_SUCCESS, operation successful */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  _ROM_Write
******************************************************************************/

/******************************************************************************
* Function Name: _Enter_PE_Mode
* Description  : Puts the FCU into program/erase mode.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - The programming/erasure address
*                bytes - The number of bytes you are writing (if you are
*                        writing).
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Enter_PE_Mode( uint32_t flash_addr, uint16_t bytes )
{
    /* If FCU firmware has already been transferred to FCU RAM,
       no need to do it again */
    if( !fcu_transfer_complete )
    {
        /* Initialise the FCU, and store operation resilt in result variable */
        /* Check if FCU initialisation was successful */
        if( _Flash_Init() != FLASH_SUCCESS )
        {
            /* FCU initialisiation failed - return operation failure */
            return FLASH_FAILURE;
        }
    }

    /* FENTRYR must be 0x0000 before bit FENTRY0 or FENTRYD can be set to 1 */
    FLASH.FENTRYR.WORD = 0xAA00;

    /* Execute null operations to allow FENTRYR register to settle */

    __asm__("nop");
    __asm__("nop");

    /* Check if FCU mode is set to ROM PE */
    if( current_mode == ROM_PE_MODE)
    {
        /* Disable the FRDYI interrupt */
        FLASH.FRDYIE.BIT.FRDYIE = 0;
        FLASH.FENTRYR.WORD = 0xAA01;
    }
    /* Check if FCU mode is set to data flash PE */
    else if( current_mode == FLD_PE_MODE )
    {
        /* Disable the FRDYI interrupt */
        FLASH.FRDYIE.BIT.FRDYIE = 0;

        /* Set FENTRYD bit(Bit 7) and FKEY (B8-15 = 0xAA) */
        FLASH.FENTRYR.WORD = 0xAA80;

        /*  First clear the FCU's status before doing Data Flash programming.
        	This is to clear out any previous errors that may have occured.
        	For example, if you attempt to read the Data Flash area
        	before you make it readable using R_FlashDataAreaAccess(). */
        _DataFlash_Status_Clr();

    }
    /* Catch-all for invalid FCU mode */
    else
    {
        /* Invalid value of 'current_mode' */
        return FLASH_FAILURE;
    }

    /* Enable Write/Erase of ROM/Data Flash */
    FLASH.FWEPROR.BYTE = 0x01;

    /* Check for FCU error */
    if(		(FLASH.FSTATR0.BIT.ILGLERR == 1)
            ||	(FLASH.FSTATR0.BIT.ERSERR  == 1)
            ||	(FLASH.FSTATR0.BIT.PRGERR  == 1)
            ||	(FLASH.FSTATR1.BIT.FCUERR  == 1))
    {
        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Check to see if peripheral clock notification command is needed */
    if( fcu_pclk_command == 0 )
    {
        /* Disable FCU interrupts, so interrupt will not trigger after
           peripheral clock notification command */
        FLASH.FRDYIE.BIT.FRDYIE = 0;

        /* Inform FCU of flash clock speed, check if operation is succesful */
        if(_Notify_Peripheral_Clock((volatile uint8_t *)flash_addr)!=0)
        {
            /* Return FLASH_FAILURE, operation failure */
            return FLASH_FAILURE;
        }

        /* No need to notify FCU of clock supplied to flash again */
        fcu_pclk_command = 1;
    }

    /* Return FLASH_SUCCESS, operation successful */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function _Enter_PE_Mode
******************************************************************************/

/******************************************************************************
* Function Name: _Exit_PE_Mode
* Description  : Takes the FCU out of program/erase mode.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash
*                programming, this function can reside and execute in Flash.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _Exit_PE_Mode( void )
{
    /* Declare wait timer count variable */
    volatile uint32_t	wait_cnt;

    /* Declare address pointer */
    volatile uint8_t *pAddr=0;

    /* Set wait timer count duration */
    wait_cnt = WAIT_MAX_ERASE;

    /* Iterate while loop whilst FCU operation is in progress */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Decrement wait timer count variable */
        wait_cnt--;

        /* Check if wait timer count value has reached zero */
        if(wait_cnt == 0)
        {
            /* Timeout duration has elapsed, assuming operation failure and
               resetting the FCU */
            _Flash_Reset();

            /* Break from the while loop prematurely */
            break;
        }
    }

    /* Check FSTATR0 and execute a status register clear command if needed */
    if(		(FLASH.FSTATR0.BIT.ILGLERR == 1)
            ||  (FLASH.FSTATR0.BIT.ERSERR  == 1)
            ||  (FLASH.FSTATR0.BIT.PRGERR  == 1))
    {

        /* Clear ILGLERR */
        if(FLASH.FSTATR0.BIT.ILGLERR == 1)
        {
            /* FASTAT must be set to 0x10 before the status clear command
               can be successfully issued  */
            if(FLASH.FASTAT.BYTE != 0x10)
            {
                /* Set the FASTAT register to 0x10 so that a status clear
                    command can be issued */
                FLASH.FASTAT.BYTE = 0x10;
            }
        }

        /* Issue a status register clear command to clear all error bits */
        if( current_mode == ROM_PE_MODE )
        {
            /* If there is more than one area then we need to check and make
               sure we are sending the status register clear command
               to the correct section. */
            /* Check to see which area we are in */
            if( FLASH.FENTRYR.BIT.FENTRY0 == 1 )
            {
                /* Area 0 */
                pAddr = (volatile uint8_t *)(ROM_AREA_0);
                /* Send status clear command to FCU */
                *pAddr = 0x50;
            }
        }
        else
        {
            /* Set address pointer to data flash area */
            pAddr = (volatile uint8_t *)DF_ADDRESS;

            /* Send status clear command to FCU */
            *pAddr = 0x50;
        }
    }

    /* Enter ROM Read mode */
    FLASH.FENTRYR.WORD = 0xAA00;

    /* Disable the FRDYI interrupt */
    FLASH.FRDYIE.BIT.FRDYIE = 0;

    /* Flash write/erase disabled */
    FLASH.FWEPROR.BYTE = 0x02;

}
/******************************************************************************
End of function  _Exit_PE_Mode
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
uint8_t R_FlashErase_1(uint8_t block)
{
    /* Declare address pointer */
    uint32_t pAddr;
    /* Declare erase operation result container variable */
    uint8_t result = FLASH_SUCCESS;


    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_ERASING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Erase Command Address */
    pAddr = GET_BLOCK_ADDR( block );

    /* Do we want to erase a Data Flash block or ROM block? */
    if( block >= BLOCK_DB0 )
    {
        /* Set current FCU mode to data flash PE */
        current_mode = FLD_PE_MODE;
    }
    else
    {
        /* Set current FCU mode to ROM PE */
        current_mode = ROM_PE_MODE;
    }

    /* Enter ROM PE mode, check if operation successful */
    if( _Enter_PE_Mode(pAddr, 0) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

#ifdef  IGNORE_LOCK_BITS
    /* Cancel the ROM Protect feature
       NOTE: If you are using block locking feature to protect your data,
       then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX600_UserConfig.h */
    FLASH.FPROTR.WORD = 0x5501;
#else
    /* Only disable lock bit protection if user has specified to
       do so earlier */
    if( lock_bit_protection == FALSE )
    {
        /* Cancel the ROM Protect feature */
        FLASH.FPROTR.WORD = 0x5501;
    }
#endif



    /* NOTE:
       The RX630/631/63N actually has 1024 x 32 byte blocks instead of the
       16 x 2Kbyte blocks defined in r_flash_api_rx600.h. These are grouped
       into 16 blocks to make it easier for the user to delete larger sections
       of the data flash at once. The user can still delete individual blocks
       but they will need to use the new flash erase function that takes
       addresses instead of blocks. For reference, the 2KB blocks defined
       are called 'fake' blocks. */
    if( FLD_PE_MODE == current_mode )
    {
        /* This is a DATA block erase */

        /* Calculate how many bytes to erase */
        int32_t bytes_to_erase = FLASH_ERASE_BLK_SZ;

        /* Erase real data flash blocks until the 'fake' block is erased .*/
        while(0 < bytes_to_erase)
        {

            /* Send FCU command to erase block */
            result = _Flash_Erase_Command((volatile uint8_t *)pAddr);

            /* Advance pointer to next block */
            pAddr += DF_BLOCK_SIZE;

            /* Subtract off bytes erased */
            bytes_to_erase -= DF_BLOCK_SIZE;
        }
    }
    else
    {
        /* This is a ROM block erase */
        /* Send FCU command to erase block */
        result = _Flash_Erase_Command((volatile uint8_t *)pAddr);
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return erase result */
    return result;
}


#if defined(OPT_FLASH_1)

/*******************************************************************/
/* Check entire data block */
uint8_t _Flash_BlankCheck_Command(uint32_t address )
{
    /* Declare data flash pointer */
    volatile uint8_t *ptrb;

    /* Declare result container variable */
    uint8_t result;

    /* Declare wait counter variable */
    volatile uint32_t wait_cnt;

    /* Any address in the erasure block */
    ptrb = (volatile uint8_t *)address;

    /* Set  bit FRDMD (bit 4) in FMODR to 1 */
    FLASH.FMODR.BIT.FRDMD = 1;

    /* Check if the entire block is blank, Set bit BCSIZE in EEPBCCNT to 1. */
    FLASH.DFLBCCNT.BIT.BCSIZE = 1;

    /* Send commands to FCU */
    *ptrb = 0x71;
    *ptrb = 0xD0;

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_BLANK_CHECK;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Increment the wait counter */
        wait_cnt--;

        /* Check if the wait counter has reached zero */
        if(wait_cnt == 0)
        {
            /* Maximum timeout duration for writing to ROM has elapsed -
               assume operation failure and reset the FCU */
            _Flash_Reset();

            /* Return FLASH_FAILURE, operation failure */
            return FLASH_FAILURE;
        }
    }

    /* Reset the FRDMD bit back to 0 */
    FLASH.FMODR.BIT.FRDMD = 0x00;

    /* Check if the 'ILGERR' was set during the command */
    if(FLASH.FSTATR0.BIT.ILGLERR == 1)	/* Check 'ILGERR' bit */
    {
        /* Take the FCU out of PE mode */
        _Exit_PE_Mode( );

        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* (Read the 'BCST' bit (bit 0) in the 'DFLBCSTAT' register  0=blank, 1=not blank */
    result = FLASH.DFLBCSTAT.BIT.BCST;

    return result;
}



/*
 * R_FlashDataErase
 * erase data flash
 * */
uint8_t R_FlashDataErase(uint32_t pAddr, uint32_t bytes_to_erase)
{
    /* Declare erase operation result container variable */
    uint8_t result = FLASH_SUCCESS;

    /* Attempt to grab state, */ /*MS questo check puo essere fatt a livello superiore */
    if( _FlashGrabState(FLASH_ERASING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }
    current_mode = FLD_PE_MODE;
    /* Enter ROM PE mode, check if operation successful */
    if( _Enter_PE_Mode(pAddr, 0) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Cancel the ROM Protect feature */
    FLASH.FPROTR.WORD = 0x5501;

    while(0 < bytes_to_erase)
    {
        /*Blank check before erase */
        result = _Flash_BlankCheck_Command((volatile uint8_t *)pAddr);
        if( result )
        {
            /* Send FCU command to erase block */
            result = _Flash_Erase_Command((volatile uint8_t *)pAddr);
        }
        /* Advance pointer to next block */
        pAddr += DF_BLOCK_SIZE;

        /* Subtract off bytes erased */
        bytes_to_erase -= DF_BLOCK_SIZE;
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return erase result */
    return result;
}
#endif


/******************************************************************************
End of function  R_FlashErase
******************************************************************************/


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
uint8_t R_FlashEraseRange(uint32_t start_addr, uint32_t bytes)
{
    /* Declare erase operation result container variable */
    uint8_t result = FLASH_SUCCESS;

    /* Take off upper byte since for programming/erase addresses for ROM are
    	the same as read addresses except upper byte is masked off to 0's.
    	Data Flash addresses are not affected. */
    start_addr &= 0x00FFFFFF;

    /* Confirm this is for data flash */
    if( start_addr >= DF_ADDRESS && start_addr < (DF_ADDRESS + DF_SIZE) )
    {
        /* Check if the number of bytes were passed is a multiple of the
           programming size for the data flash */
        if( bytes & (DF_BLOCK_SIZE-1) )
        {
            /* Return number of bytes not a multiple of the programming size */
            return FLASH_ERROR_BYTES;
        }
        /* Check for an address on an erase boundary. */
        if( start_addr & (DF_BLOCK_SIZE-1) )
        {
            /* Return address not on a programming boundary */
            return FLASH_ERROR_ALIGNED;
        }
    }
    else
    {
        /* This function is only for data flash blocks. */
        return FLASH_ERROR_ADDRESS;
    }

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_ERASING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Set current FCU mode to data flash PE */
    current_mode = FLD_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
    if( _Enter_PE_Mode(start_addr, 0) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

#ifdef  IGNORE_LOCK_BITS
    /* Cancel the ROM Protect feature
       NOTE: If you are using block locking feature to protect your data,
       then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX600_UserConfig.h */
    FLASH.FPROTR.WORD = 0x5501;
#else
    /* Only disable lock bit protection if user has specified to
       do so earlier */
    if( lock_bit_protection == FALSE )
    {
        /* Cancel the ROM Protect feature */
        FLASH.FPROTR.WORD = 0x5501;
    }
#endif

    /* Erase real data flash blocks until the 'fake' block is erased .*/
    while(0 < bytes)
    {
        /* Send FCU command to erase block */
        result = _Flash_Erase_Command((volatile uint8_t *)start_addr);

        /* Advance pointer to next block */
        start_addr += DF_BLOCK_SIZE;

        /* Subtract off bytes erased */
        bytes -= DF_BLOCK_SIZE;
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return erase result */
    return result;
}
/******************************************************************************
End of function  R_FlashEraseRange
******************************************************************************/


/******************************************************************************
* Function Name: _Flash_Erase_Command
* Description  : Issues the FCU command to erase a flash block
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : erase_addr = An address in the block to be erased
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Flash_Erase_Command(volatile uint8_t * const erase_addr)
{
    /* Declare timer wait count variable */
    volatile uint32_t wait_cnt;
    /* Declare erase operation result container variable */
    uint8_t result = FLASH_SUCCESS;

    /* Send the FCU Command */
    *erase_addr = 0x20;
    *erase_addr = 0xD0;

    /* Set the wait counter timeout duration */
    wait_cnt = WAIT_MAX_ERASE;

    /* Wait while FCU operation is in progress */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Decrement the wait counter */
        wait_cnt--;

        /* Check if the wait counter has reached zero */
        if(wait_cnt == 0)
        {

            /* Maximum timeout duration for erasing a ROM block has
               elapsed, assuming operation failure - reset the FCU */
            _Flash_Reset();

            /* Return FLASH_FAILURE, operation failure */
            return FLASH_FAILURE;
        }
    }

    /* Check if erase operation was successful by checking
       bit 'ERSERR' (bit5) and 'ILGLERR' (bit 6) of register 'FSTATR0' */
    /* Check FCU error */
    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.ERSERR  == 1))
    {
        result = FLASH_FAILURE;
    }

    /* Return erase result */
    return result;
}
/******************************************************************************
End of function  _Flash_Erase_Command
******************************************************************************/

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
uint8_t R_FlashWrite_1 ( uint32_t flash_addr,
                         uint32_t buffer_addr,
                         uint16_t bytes )
{
    /* Declare result container and number of bytes to write variables */
    uint8_t result = FLASH_SUCCESS;
    uint8_t num_byte_to_write;

    /* Take off upper byte since for programming/erase addresses for ROM are
    	the same as read addresses except upper byte is masked off to 0's.
    	Data Flash addresses are not affected. */
    flash_addr &= 0x00FFFFFF;

    /* ROM area or Data Flash area - Check for DF first */
    if( flash_addr >= DF_ADDRESS && (flash_addr < DF_ADDRESS + DF_SIZE) )
    {
        /* Check if the number of bytes were passed is a multiple of the
           programming size for the data flash */
        if( bytes & (DF_PROGRAM_SIZE_SMALL-1) )
        {
            /* Return number of bytes not a multiple of the programming size */
            return FLASH_ERROR_BYTES;
        }
        /* Check for an address on a programming boundary. */
        if( flash_addr & (DF_PROGRAM_SIZE_SMALL-1) )
        {
            /* Return address not on a programming boundary */
            return FLASH_ERROR_ALIGNED;
        }
    }
    /* Check for ROM area */
    else if( (flash_addr >= ROM_PE_ADDR) &&	(flash_addr < ROM_PE_ADDR + ROM_SIZE) )
    {
        /* Check if the number of bytes were passed is a multiple of the
           programming size for ROM */
        if( bytes & (ROM_PROGRAM_SIZE-1) )
        {
            /* Return number of bytes not a multiple of the programming size */
            return FLASH_ERROR_BYTES;
        }

        /* Check for an address on a programming boundary. */
        if( flash_addr & (ROM_PROGRAM_SIZE-1))
        {
            /* Return address not on a ROM programming byte boundary */
            return FLASH_ERROR_ALIGNED;
        }

        /* Check for attempts to program over flash area boundaries. These
           boundaries are defined in r_flash_api_rx600.h as 'ROM_AREA_#'. */

        if((flash_addr < (ROM_AREA_0))	&& ((flash_addr + bytes) > ROM_AREA_0))
        {
            /* Return cannot write across a flash area boundary */
            return FLASH_ERROR_BOUNDARY;
        }
    }
    else
    {
        /* Return invalid flash address */
        return FLASH_ERROR_ADDRESS;
    }

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_WRITING) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Do we want to program a DF area or ROM area? */
    if( flash_addr < GET_BLOCK_ADDR(ROM_NUM_BLOCKS-1) )
    {
        /* Set current FCU mode to data flash PE */
        current_mode = FLD_PE_MODE;
    }
    else
    {
        /* Set FCU to ROM PE mode */
        current_mode = ROM_PE_MODE;
    }

    /* Enter PE mode, check if operation is successful */
    if( _Enter_PE_Mode( flash_addr, bytes ) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return operation failure */
        return FLASH_FAILURE;
    }

#ifdef  IGNORE_LOCK_BITS
    /* Cancel the ROM Protect feature
       NOTE: If you are using block locking feature to protect your data,
       then comment out 'IGNORE_LOCK_BITS' in Flash_API_RX600_UserConfig.h */
    FLASH.FPROTR.WORD = 0x5501;
#else
    /* Only disable lock bit protection if user has specified to
       do so earlier */
    if( lock_bit_protection == FALSE )
    {
        /* Cancel the ROM Protect feature */
        FLASH.FPROTR.WORD = 0x5501;
    }
#endif


    /* Check if FCU mode is set to data flash PE */
    if( current_mode == FLD_PE_MODE )
    {
        /* Iterate while there are still bytes remaining to write */
        while( bytes )
        {
            /* Check if address is on a data flash programming boundary, and
               there is at least 1 'page' remaining to write. */
            /* Not all MCUs have two programming sizes for the data flash. If a
               MCU only has 1 then DF_PROGRAM_SIZE_SMALL is defined and not
               DF_PROGRAM_SIZE_LARGE. */
#if defined(DF_PROGRAM_SIZE_LARGE)
            if( (bytes >= DF_PROGRAM_SIZE_LARGE) &&
                    ((flash_addr & (DF_PROGRAM_SIZE_LARGE-1)) == 0x00))
            {
                /* Use large write size since it's more time efficient */
                num_byte_to_write = DF_PROGRAM_SIZE_LARGE;
            }
            else
            {
                /* Cannot use large program size, use small */
                num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
            }
#else
            /* Set the next data transfer size to be DF_PROGRAM_SIZE_SMALL */
            num_byte_to_write = DF_PROGRAM_SIZE_SMALL;
#endif

            /* Call the Programming function, store the operation status in the
               container variable, result */
            result = _DataFlash_Write( flash_addr, buffer_addr, num_byte_to_write);

            /* Check the container variable result for errors */
            if( result != FLASH_SUCCESS )
            {
                /* Data flash write error detected, break from flash write
                   while loop prematurely */
                break;
            }

            /* Increment the flash address and the buffer address by the size
               of the transfer thats just completed */
            flash_addr += num_byte_to_write;
            buffer_addr += num_byte_to_write;

            /* Decrement the number of bytes remaining by the size of the last
               flash write */
            bytes -= num_byte_to_write;
        }
    }
    /* Catch-all - FCU mode must be ROM PE */
    else
    {
        /* Iterate loop while there are still bytes remaining to write */
        while( bytes )
        {
            /* Call the Programming function */
            result = _ROM_Write(flash_addr, buffer_addr);

            /* Check for errors */
            if( result != FLASH_SUCCESS )
            {
                /* Stop programming */
                break;
            }

            /* Increment the flash address and the buffer address by the size
               of the transfer thats just completed */
            flash_addr += ROM_PROGRAM_SIZE;
            buffer_addr += ROM_PROGRAM_SIZE;

            /* Decrement the number of bytes remaining by the size of the last
               flash write */
            bytes -= ROM_PROGRAM_SIZE;
        }
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return flash programming result */
    return result;
}
/******************************************************************************
End of function  R_FlashWrite
******************************************************************************/

/******************************************************************************
* Function Name: _Notify_Peripheral_Clock
* Description  : Notifies FCU or clock supplied to flash unit
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash programming,
*                this function can reside and execute in Flash.
* Arguments    : flash_addr - Flash address you will be erasing or writing to
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_FAILURE = Operation Failed
******************************************************************************/
static uint8_t _Notify_Peripheral_Clock(volatile uint8_t * flash_addr)
{
    /* Declare wait counter variable */
    volatile uint32_t wait_cnt;

    /* Notify Peripheral Clock(PCK) */
    /* Set frequency of PCK */
    FLASH.PCKAR.WORD = (FLASH_CLOCK_FREQUENCY / 1000000);

    /* Execute Peripheral Clock Notification Commands */
    *flash_addr = 0xE9;
    *flash_addr = 0x03;
    *(volatile uint16_t *)flash_addr = 0x0F0F;
    *(volatile uint16_t *)flash_addr = 0x0F0F;
    *(volatile uint16_t *)flash_addr = 0x0F0F;
    *flash_addr = 0xD0;

    /* Set timeout wait duration */
    wait_cnt = WAIT_MAX_NOTIFY_PERIPHERAL_CLOCK;

    /* Check FRDY */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Decrement wait counter */
        wait_cnt--;

        /* Check if wait counter has reached zero */
        if(wait_cnt == 0)
        {
            /* Timeout duration elapsed, assuming operation failure - Reset
               FCU */
            _Flash_Reset();

            /* Return FLASH_FAILURE, operation failure  */
            return FLASH_FAILURE;
        }
    }

    /* Check ILGLERR */
    if(FLASH.FSTATR0.BIT.ILGLERR == 1)
    {
        /* Return FLASH_FAILURE, operation failure*/
        return FLASH_FAILURE;
    }

    /* Return FLASH_SUCCESS, operation success */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function _Notify_Peripheral_Clock
******************************************************************************/

/******************************************************************************
* Function Name: _Flash_Reset
* Description  : Reset the FCU.
*                NOTE: This function MUST execute from in RAM for 'ROM Area'
*                programming, but if you are ONLY doing Data Flash
*                programming, this function can reside and execute in Flash.
* Arguments    : none
* Return Value : none
******************************************************************************/
void _Flash_Reset(void)
{
    /* Declare wait counter variable */
    volatile uint32_t	wait_cnt;

    /* Reset the FCU */
    FLASH.FRESETR.WORD = 0xCC01;

    /* Give FCU time to reset */
    wait_cnt = WAIT_tRESW;

    /* Wait until the timer reaches zero */
    while(wait_cnt != 0)
    {
        /* Decrement the timer count each iteration */
        wait_cnt--;
    }

    /* Release state */
    _FlashReleaseState();

    /* FCU is not reset anymore */
    FLASH.FRESETR.WORD = 0xCC00;

    /* Enter ROM Read mode */
    FLASH.FENTRYR.WORD = 0xAA00;

    /* Flash write/erase disabled */
    FLASH.FWEPROR.BYTE = 0x02;
}
/******************************************************************************
End of function  _Flash_Reset
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashGetStatus
* Description  : Returns the current state of the flash
* Arguments    : none
* Return Value : FLASH_SUCCESS = Flash is ready to use
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
uint8_t R_FlashGetStatus(void)
{

    /* Return flash status */
    if( flash_state == FLASH_READY )
    {
        return FLASH_SUCCESS;
    }

    return FLASH_BUSY;
}
/******************************************************************************
End of function  R_FlashGetStatus
******************************************************************************/

/******************************************************************************
* Function Name: _FlashGrabState
* Description  : Attempt to grab the flash state to perform an operation
* Arguments    : Which state to attempt to transition to
* Return Value : FLASH_SUCCESS = State was grabbed
*                FLASH_BUSY = Flash is busy with another operation
******************************************************************************/
static uint8_t _FlashGrabState(enum Flash_States new_state)
{
    /* Variable used in trying to grab semaphore. Using the xchg instruction
       makes this atomic */
    int32_t semaphore = 1;

    /* Try to grab semaphore to change state */
    /*    xchg(&semaphore, &flash_semaphore);*/


    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    semaphore = flash_semaphore;

    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");


    /* Check to see if semaphore was successfully taken */
    if( semaphore == 0 )
    {
        /* Semaphore grabbed, we can change state */
        flash_state = new_state;

        /* Return success */
        return FLASH_SUCCESS;
    }
    else
    {
        /* Another operation is on-going */
        return FLASH_BUSY;
    }
}
/******************************************************************************
End of function  _FlashGrabState
******************************************************************************/

/******************************************************************************
* Function Name: _FlashReleaseState
* Description  : Release state so another flash operation can take place
* Arguments    : none
* Return Value : none
******************************************************************************/
static void _FlashReleaseState(void)
{
    /* Set current FCU mode to READ */
    current_mode = READ_MODE;

    /* Done with programming */
    flash_state = FLASH_READY;

    /* Release hold on semaphore */
    flash_semaphore = 0;
}
/******************************************************************************
End of function  _FlashReleaseState
******************************************************************************/

#ifndef  IGNORE_LOCK_BITS

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
uint8_t R_FlashProgramLockBit(uint8_t block)
{
    /* Declare address pointer */
    volatile uint8_t *pAddr;

    /* Declare operation result container variable */
    uint8_t result = FLASH_SUCCESS;

    /* Declare timer wait count variable */
    volatile uint32_t wait_cnt;

    /* Attempt to grab state */
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Flash Command Address */
    pAddr = (volatile uint8_t *)(GET_BLOCK_ADDR( block ));

    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
    if( _Enter_PE_Mode((uint32_t)pAddr, 0) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Send the FCU Command */
    *pAddr = 0x77;
    *pAddr = 0xD0;

    /* Set timeout wait counter value */
    wait_cnt = WAIT_MAX_ROM_WRITE;

    /* Wait until FCU operation finishes, or a timeout occurs */
    while(FLASH.FSTATR0.BIT.FRDY == 0)
    {
        /* Decrement the wait counter */
        wait_cnt--;

        /* Check if the wait counter has reached zero */
        if(wait_cnt == 0)
        {
            /* Maximum timeout duration for writing to ROM has elapsed -
               assume operation failure and reset the FCU */
            _Flash_Reset();

            /* Return FLASH_FAILURE, operation failure */
            result = FLASH_FAILURE;
        }
    }

    /* Check for illegal command or programming errors */
    if((FLASH.FSTATR0.BIT.ILGLERR == 1) || (FLASH.FSTATR0.BIT.PRGERR  == 1))
    {
        /* Return FLASH_FAILURE, operation failure */
        result = FLASH_FAILURE;
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return result of lock bit programming */
    return result;
}
/******************************************************************************
End of function  R_FlashProgramLockBit
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashReadLockBit
* Description  : Reads and returns the lock bit status for a ROM block
* Arguments    : block - Which ROM erasure block to read the lock bit of
* Return Value : FLASH_LOCK_BIT_SET = Lock bit was set
*                FLASH_LOCK_BIT_NOT_SET = Lock bit was not set
*                FLASH_FAILURE = Operation Failed
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashReadLockBit(uint8_t block)
{
    /* Declare address pointer */
    volatile uint8_t *pAddr;

    /* Declare operation result container variable */
    uint8_t result = FLASH_SUCCESS;

    /* Holds outcome of lock-bit read */
    uint8_t lock_bit;

    /* Attempt to grab state */;
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Flash Command Address */
    pAddr = (volatile uint8_t *)(GET_BLOCK_ADDR( block ));

    /* Set FCU to ROM PE mode */
    current_mode = ROM_PE_MODE;

    /* Enter ROM PE mode, check if operation successful */
    if( _Enter_PE_Mode((uint32_t)pAddr, 0) != FLASH_SUCCESS)
    {
        /* Release state */
        _FlashReleaseState();

        /* Return FLASH_FAILURE, operation failure */
        return FLASH_FAILURE;
    }

    /* Switch to ROM Lock-Bit Read Mode */
    *pAddr = 0x71;

    /* Check for illegal command */
    if(FLASH.FSTATR0.BIT.ILGLERR == 1)
    {
        /* Return FLASH_FAILURE, operation failure */
        result = FLASH_FAILURE;
    }
    else
    {
        /* Read lock bit value */
        lock_bit = *pAddr;

        /* Set return value */
        if(lock_bit != 0x00)
        {
            /* Lock bit is not set */
            result = FLASH_LOCK_BIT_NOT_SET;
        }
        else
        {
            /* Lock bit is set */
            result = FLASH_LOCK_BIT_SET;
        }
    }

    /* Leave Program/Erase Mode */
    _Exit_PE_Mode();

    /* Release state */
    _FlashReleaseState();

    /* Return result of lock bit programming */
    return result;
}
/******************************************************************************
End of function  R_FlashReadLockBit
******************************************************************************/

/******************************************************************************
* Function Name: R_FlashSetLockBitProtection
* Description  : Enables or disables lock bit protection.
* Arguments    : lock_bit - Boolean, whether to enable or disable lock bit
*                           protection (true = enabled, false = disabled)
* Return Value : FLASH_SUCCESS = Operation Successful
*                FLASH_BUSY = Another flash operation is in progress
******************************************************************************/
uint8_t R_FlashSetLockBitProtection(uint8_t lock_bit)
{
    /* Attempt to grab state */;
    if( _FlashGrabState(FLASH_LOCK_BIT) != FLASH_SUCCESS )
    {
        /* Another operation is already in progress */
        return FLASH_BUSY;
    }

    /* Enable or disable? */
    lock_bit_protection = lock_bit;

    /* Release state */
    _FlashReleaseState();

    /* Return success */
    return FLASH_SUCCESS;
}
/******************************************************************************
End of function  R_FlashSetLockBitProtection
******************************************************************************/

#endif /* IGNORE_LOCK_BITS */


/* ********************************************************************************************** */
/*                                  START FLS Class HAL Support                                   */
/* ********************************************************************************************** */


uint8_t cloneSector(uint32_t dest, uint32_t source);

uint8_t cloneSector(uint32_t dest, uint32_t source)
{
	uint8_t result;
	uint32_t buff[FLS_WRITE_SIZE/sizeof(uint32_t)];
	int i;
	/* Erase the destination sector */
	result = R_FlashEraseRange(dest, CLONE_BLOCK_SIZE);
	if(result == FLASH_SUCCESS )
    {
		/* Write - No write sector are left blank */
		for(i = 0; i < CLONE_BLOCK_SIZE; i+= FLS_WRITE_SIZE)
		{
			if(BlankCheckDataFlash((source + i), FLS_WRITE_SIZE) != FLASH_BLANK)
        {
				/* copy to ram */
				/*for(j=0; j<(FLS_WRITE_SIZE/sizeof(uint32_t)); j+= sizeof(uint32_t)) */
				buff[0] = *(uint32_t*)(source+i);
				buff[1] = *(uint32_t*)(source+i+4);

				/* write to destination */
				result = R_FlashWrite(dest + i, (uint32_t)&buff[0], FLS_WRITE_SIZE);
				if(result != FLASH_SUCCESS )
            break;
        }
    }
	}
    return result;
}
#if defined(DATA_CLONE_RECOVERY)
uint8_t restoreUncompleteWrite(void);

uint8_t restoreUncompleteWrite(void)
{
    uint32_t sector_restore_addr;
    uint8_t result=FLASH_SUCCESS;
    uint8_t check_word_start_clone = 0;

    if(BlankCheckDataFlash(CLONE_TRANS_BLOCK_ADDRESS, FLS_WRITE_SIZE))
    {
    	sector_restore_addr    = *(uint32_t*)CLONE_TRANS_BLOCK_ADDRESS;
		check_word_start_clone = *(uint8_t*)(CLONE_TRANS_BLOCK_ADDRESS + CLONE_START_WORD_POS);
        if(check_word_start_clone == CLONE_START_WORD_VALUE) /* verifica che non sia un valore casuale */
        {
        	result = cloneSector(sector_restore_addr, CLONE_BLOCK_ADDRESS);

        	if(result != FLASH_SUCCESS)
            {
            /*== Erase clone sector ==*/
            result = R_FlashEraseRange((FLS_positionType) CLONE_TRANS_BLOCK_ADDRESS, CLONE_BLOCK_SIZE);
            }
        }/* end if check word */
    }/* end if restore clone block */
	R_FlashDataAreaAccess(0xFFFF, 0x0000); /*MS ????? */
	return result;
}
#endif
/*
 * Function: FLS_Init
 * Description: Initialization of the FLASH Module Clock. Programming and erase operation are allowed
 * Note: Modified from define to function.
 * Status: Test me on Hardware!
 */
void FLS_Init(void)
{
    CopyFlashRoutinesToRAM();
    _Flash_Init();	/* It is call by Flash Erase Function*/
    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
#if defined(DATA_CLONE_RECOVERY)
    restoreUncompleteWrite();
#endif
}/*FlashInit*/

/*
 * Function: FLS_Ease
 * Description: Call "R_FlashErase(position)" API From Renesas Flash Library
 * Note:
 * Status:
 */
uint8_t FLS_Erase(FLS_positionType address)
{
    volatile uint8_t fls_erase_status;
    uint8_t block;

    /* Block Calcolus ROM*/
    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
    if((address >= FLASH_BASE_ADDRESS) && (address < FLASH_END_ADDRESS))
    {
        block = ((FLASH_END_ADDRESS - address )/FLASH_ERASE_BLK_SZ);
        fls_erase_status = R_FlashErase(block);	/* !< Block ID as parameter */
    }
    /* Block Calcolus DATA*/
    else if((address >= FLASH_DATA_BASE_ADDRESS) && (address <= FLASH_DATA_END_ADDRESS))
    {
#if defined(OPT_FALSH_1) /*MS prova ottimizzazione */
        fls_erase_status = R_FlashDataErase(address, FLASH_ERASE_BLK_SZ);
#else
        block = ((address - FLASH_DATA_BASE_ADDRESS)/FLASH_ERASE_BLK_SZ)+ BLOCK_DB0;
        fls_erase_status = R_FlashErase(block);	/* !< Block ID as parameter */
#endif
    }
    else
    {
        fls_erase_status = FLASH_ERROR_ADDRESS;
    }
    R_FlashDataAreaAccess(0xFFFF, 0x0000);
    return fls_erase_status;
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

    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);

    if((dest >= DF_ADDRESS) && (dest < DF_ADDRSS_END))	/* Write Data in DATA FLASH */
    {
        fls_programming_status=FLS_ProgrammingData( pSource, len, dest);
    }
    else /*Write Data in ROM */
    {
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

            fls_programming_status=R_FlashWrite((uint32_t)dest-headOffset, (uint32_t)&writeBuff[0], ROM_PROGRAM_SIZE);
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
                fls_programming_status=R_FlashWrite(writePos, (uint32_t)srcPtr, nByte);
                srcPtr += nByte;
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
                fls_programming_status=R_FlashWrite(beginTail, (uint32_t)&writeBuff[0], ROM_PROGRAM_SIZE);
            }
        }

        R_FlashDataAreaAccess(0xFFFF, 0x0000);
    }/* end else to write in ROM */
    return fls_programming_status;
}

/*
* Function: FLS_CheckErrors
* Description: Check the Flash Status Error Byte
* Note:
*/
uint8_t FLS_CheckErrors(void)
{
    uint8_t result = FLASH_SUCCESS;
    if(FLASH.FSTATR0.BIT.ERSERR == 1)	/* Flash Erase Error*/
    {
        result = FLASH_FAILURE;		/* Return FLASH_FAILURE, operation failure*/
    }
    if(FLASH.FSTATR0.BIT.PRGERR == 1)	/* Flash Programming Error*/
    {
        result = FLASH_FAILURE;		/* Return FLASH_FAILURE, operation failure*/
    }
    if(FLASH.FSTATR0.BIT.ILGLERR == 1)	/* Flash Illegal Istraction Error*/
    {
        result = FLASH_FAILURE;		/* Return FLASH_FAILURE, operation failure*/
    }
    if(FLASH.FSTATR1.BIT.FCUERR == 1)	/* FCU processing Error*/
    {
        result = FLASH_FAILURE;		/* Return FLASH_FAILURE, operation failure*/
    }
    if( (FLASH.FSTATR0.BIT.FRDY) && (FLASH.FPESTAT.BIT.PEERRST != 0) )	/* FCU processing Error*/
    {
        /*The value of the PEERRST bits is valid only when the FSTATR0.FRDY bit is set to 1 while the
        FSTATR0.ERSERR bit or FSTATR0.PRGERR bit is 1. The value of the reason of the past error is retained in the
        PEERRST[7:0] bits when the ERSERR bit and PRGERR bit is 0*/

        result = FLASH_FAILURE;		/* Return FLASH_FAILURE, operation failure*/
        _Flash_Reset();
    }
    return result;
}/* end FLS_CheckErrors(void) */

/*
* Function: FLS_InitRegsForErasing
* Description: Should clear the last Flash error
* Note: The Flash Status registers are read only
*/
void FLS_InitRegsForErasing(void)
{
}/*end FLS_InitRegsForErasing*/


#define BLANK_CHECK_BYTE_SIZE 	2
#define BLANK_CHECK_SIZE_CMD 	BLANK_CHECK_2_BYTE
#define ADDR_MASK_BLANK_CHECK 	0x1

/*
#define BLANK_CHECK_BYTE_SIZE 	8
#define BLANK_CHECK_SIZE_CMD 	BLANK_CHECK_8_BYTE
#define ADDR_MASK_BLANK_CHECK 	0x7
*/
uint8_t BlankCheckDataFlash(FLS_positionType dest, FLS_sizeType len)
{
    uint8_t fls_status=FLASH_SUCCESS;
    uint32_t headOffset=0;
    uint32_t headLen=0;
    uint32_t tailLen=0;
    uint32_t writePos;
    uint32_t nByte;
    unsigned int i;

    headOffset=0;
    headLen=0;
    tailLen=0;
    /********************/
    /* Ottimizzazione */
    if(((dest&0x7F) == 0) && (len & 0x7F)==0)
    {
        for(writePos = 0;(fls_status == FLASH_SUCCESS) && (writePos < len) ; writePos += DF_BLOCK_SIZE)
        {
            fls_status=R_FlashDataAreaBlankCheck(dest + writePos, BLANK_CHECK_ENTIRE_BLOCK);
        }
        return fls_status;
    }

    /*******************/

    headOffset = (int)(dest & ADDR_MASK_BLANK_CHECK);

    if(headOffset > 0)
        headLen = BLANK_CHECK_BYTE_SIZE-headOffset;

    /* head ------------------------------------------*/
    if((headLen >0) || (len < BLANK_CHECK_BYTE_SIZE))
    {
        fls_status=R_FlashDataAreaBlankCheck(dest-headOffset, BLANK_CHECK_SIZE_CMD);
    }
    /* body -----------------------------------------*/
    if( !fls_status )
    {
        writePos= dest+headLen;
        nByte=len;
        tailLen = (len + headOffset) & ADDR_MASK_BLANK_CHECK;
        if(headLen>0)
        {
            nByte -= headLen;
        }
        nByte -= tailLen;
        if(nByte>0)
        {
                for(i=0; (i < nByte) && (fls_status == FLASH_BLANK); i += BLANK_CHECK_BYTE_SIZE)
                {
                    fls_status=R_FlashDataAreaBlankCheck(writePos + i, BLANK_CHECK_SIZE_CMD);
            }
        }
    }
    /* tail ------------------------------------------*/
    if( !fls_status )
    {
        unsigned int beginTail = dest+(len-tailLen);
        if( (tailLen > 0) && (dest < beginTail))
        {
            fls_status=R_FlashDataAreaBlankCheck(beginTail, BLANK_CHECK_SIZE_CMD);
        }
    }

    return fls_status;
}

/*
* Function: FLS_BlankCheck
* Description:
*/
uint8_t FLS_BlankCheck(FLS_sizeType size, FLS_positionType position)
{
    char *p = (char*)position;
    unsigned int i = 0;
    uint8_t result = FLASH_SUCCESS;
    /* Check if ROM PROGRAM FLASH IS BLANK */
    if((position >= FLASH_BASE_ADDRESS) && (position < FLASH_END_ADDRESS))
    {
        for(i = 0; i < size; i++, p++)
        {
            if( *p != 0xFF )
            {
                result = FLASH_FAILURE;
                break;
            }
        }
    }
    else
    {
        /* Check if DATA FLASH IS BLANK */
        if((position >= FLASH_DATA_BASE_ADDRESS) && (position <= FLASH_DATA_END_ADDRESS))
        {
            result = BlankCheckDataFlash(position, size);
            if( result != FLASH_BLANK)
            	result = FLASH_FAILURE;
        }
    }
    return result;
}

/*
 * Function: DataFlashClone(FLS_positionType dest)
 * Parameters:
 * source: of DATA to clone
 * data: New data to write
 *
 */
uint8_t FLS_DataFlashClone(FLS_positionType aligned_address, uint32_t pSource, uint32_t len, uint8_t headOffset)
{
    uint8_t result = 0;
    int i = 0;
    int j = 0;
    uint32_t  wBuff[FLS_WRITE_SIZE/sizeof(uint32_t)];
    uint8_t  *writeBuff;
    uint32_t sector_addr;

    writeBuff = (uint8_t*)wBuff;
    sector_addr = aligned_address & 0xFFFFFF80;

    /*== a. Erase the clone sector  ==*/
    result = R_FlashEraseRange(CLONE_BLOCK_ADDRESS, CLONE_BLOCK_SIZE);
    if(result != FLASH_SUCCESS)
    {
        return FLASH_FAILURE;
    }

    /*== b. Clone the sector to write - preserve the blank block ==*/
    result = cloneSector(CLONE_BLOCK_ADDRESS, sector_addr);

#if defined(DATA_CLONE_RECOVERY)

    if(BlankCheckDataFlash(CLONE_TRANS_BLOCK_ADDRESS, FLS_WRITE_SIZE) != FLASH_BLANK)
    {
        result = R_FlashEraseRange(CLONE_TRANS_BLOCK_ADDRESS, CLONE_BLOCK_SIZE);
		if(result != FLASH_SUCCESS)
		{
			return FLASH_FAILURE;
		}
    }
    /*== c. Write address of block to clone ==*/
    wBuff[0] = sector_addr;

    wBuff[CLONE_START_WORD_POS/sizeof(uint32_t)] = CLONE_START_WORD_VALUE;
    result = R_FlashWrite(CLONE_TRANS_BLOCK_ADDRESS, (uint32_t)&wBuff[0], FLS_WRITE_SIZE);
    if(result != FLASH_SUCCESS)
    {
        return FLASH_FAILURE;
    }/* end if error in flash critical section write */
#endif /* defined(DATA_CLONE_RECOVERY) */

    /*== d. Erase sector to write ==*/
    result = R_FlashEraseRange(sector_addr, CLONE_BLOCK_SIZE);
    if(result != FLASH_SUCCESS )
        return FLASH_FAILURE;
    /*== e. copy data to clone sector ==*/
    for(i = 0; i < CLONE_BLOCK_SIZE ; i+=FLS_WRITE_SIZE)
    {
        if(BlankCheckDataFlash((CLONE_BLOCK_ADDRESS + i), FLS_WRITE_SIZE) != FLASH_BLANK)
        {
            for(j = 0; j < FLS_WRITE_SIZE ; j++)
            {
            	if( (sector_addr+(i+j)) >= (aligned_address + headOffset)
            			&& (sector_addr+(i+j)) < (aligned_address + headOffset+ len))
                {
                    writeBuff[j] =  *(uint8_t*)(CLONE_BLOCK_ADDRESS+i+j) &  *((uint8_t*)pSource + j);
                }
                else
                {
                	writeBuff[j] =  *(uint8_t*)(CLONE_BLOCK_ADDRESS+i+j);
                }
            }
            result = R_FlashWrite(sector_addr + i, (uint32_t)&writeBuff[0], FLS_WRITE_SIZE);
            if(result != FLASH_SUCCESS )
                return FLASH_FAILURE;
        }
    }
#if defined(DATA_CLONE_RECOVERY)
    /*== g. Erase of clone transaction  block ==*/
    result = R_FlashEraseRange((FLS_positionType) CLONE_TRANS_BLOCK_ADDRESS, CLONE_BLOCK_SIZE);
    if(result != FLASH_SUCCESS)
    {
        return FLASH_FAILURE;
    }
#endif
    return FLASH_SUCCESS;
}/* end DataFlashClone */

/*
 * Function: FLS_ProgrammingData
 * Description:
 *
 */
uint8_t FLS_ProgrammingData(uint8_t* pSource, uint32_t len, FLS_positionType dest)
{
	unsigned int headOffset = 0;
	unsigned int headLen = 0;
	unsigned int bodyLen = 0;
    unsigned int i = 0;
    FLS_positionType alineated_address = 0;
    uint8_t result = FLASH_SUCCESS;
    uint8_t *srcPtr=(uint8_t *)pSource;
    uint8_t writeBuff[ROM_PROGRAM_SIZE];

    headOffset = (dest & (ROM_PROGRAM_SIZE-1));

    /*  Calcolus start address */
    alineated_address = (uint32_t)(dest-headOffset);

    /* head ------------------------------------------*/
    if (headOffset)
    {
        headLen    = ROM_PROGRAM_SIZE - headOffset;

        if (headLen > len)
        {
            headLen = len;
        }

        for(i=0; i<ROM_PROGRAM_SIZE; i++)  /* init write buffer */
        {
            if ( (i < headOffset) || (i >= (headOffset + headLen)) )
            {
                writeBuff[i] = 0xFF;
            }
            else
            {
                writeBuff[i]=*srcPtr;
                srcPtr++;
            }
        }

        /* scrive nel buffer i dati */
        if(BlankCheckDataFlash(alineated_address, ROM_PROGRAM_SIZE) == FLASH_BLANK)
        {
            result = R_FlashWrite(alineated_address, (uint32_t)&writeBuff[0],ROM_PROGRAM_SIZE);
        }
        else
        {
            result = FLS_DataFlashClone(alineated_address, (uint32_t)&writeBuff[0], headLen, headOffset);
        }

        alineated_address += ROM_PROGRAM_SIZE;

    }

    /* body -----------------------------------------*/
	if( result == FLASH_SUCCESS  )
    {
		bodyLen = len - headLen;
		while( bodyLen > ROM_PROGRAM_SIZE )
        {
			if(BlankCheckDataFlash(alineated_address, ROM_PROGRAM_SIZE) == FLASH_BLANK)
            {
                result = R_FlashWrite(alineated_address, (uint32_t)srcPtr, ROM_PROGRAM_SIZE);
            }
            else
            {
                result = FLS_DataFlashClone(alineated_address, (uint32_t)srcPtr, ROM_PROGRAM_SIZE, 0);
            }
            bodyLen -= ROM_PROGRAM_SIZE;
            srcPtr += ROM_PROGRAM_SIZE;
            alineated_address += ROM_PROGRAM_SIZE;

            if(FLASH_SUCCESS != result)
                break;
        }
    }/* end if body */

    /* tail ------------------------------------------*/
    if( (result == FLASH_SUCCESS) &&  (bodyLen > 0))
    {

        for(i=0; i<ROM_PROGRAM_SIZE; i++)   /* init write buffer */
        {
            if( i<bodyLen )
            {
                writeBuff[i] = *srcPtr;
                srcPtr++;
            }
            else
            {
                writeBuff[i] = 0xFF;
            }
        }

        if(BlankCheckDataFlash(alineated_address, ROM_PROGRAM_SIZE) == FLASH_BLANK)
        {
            result = R_FlashWrite(alineated_address, (uint32_t)&writeBuff[0], ROM_PROGRAM_SIZE);
        }
        else
        {
            result = FLS_DataFlashClone(alineated_address, (uint32_t)&writeBuff[0], ROM_PROGRAM_SIZE, 0);
        }
    }/* end if tail */


    if(result != FLASH_SUCCESS)
        return FLASH_FAILURE;

    return FLASH_SUCCESS;
}/* FLS_ProgrammingData */

/******************************************************************************
Typedef definitions
******************************************************************************/
extern char _load_start_PRAMERASE, _load_stop_PRAMERASE;
extern char _load_start_PRAMWRITE, _load_stop_PRAMWRITE;
extern char hal_ovl ;

uint8_t R_FlashErase_2(uint8_t block)
{
    uint8_t retVal=FLASH_SUCCESS;

    memcpy( (void*)&hal_ovl, (const void*)&_load_start_PRAMERASE, (&_load_stop_PRAMERASE - &_load_start_PRAMERASE) );

    retVal = R_FlashErase_1(block);

    return retVal;
}

uint8_t R_FlashWrite_2 ( uint32_t flash_addr, uint32_t buffer_addr, uint16_t bytes )
{
    uint8_t retVal=FLASH_SUCCESS;

    memcpy( (void*)&hal_ovl, (const void*)&_load_start_PRAMWRITE, (&_load_stop_PRAMWRITE - &_load_start_PRAMWRITE) );

    retVal = R_FlashWrite_1 ( flash_addr, buffer_addr, bytes );

    return retVal;
}




