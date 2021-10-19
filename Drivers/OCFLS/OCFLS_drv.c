/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\n			Source File
\n
\file		OCFLS_drv.c
\ingroup	OCFLS
\date		Nov 10, 2009
\version	01.00.00
\author		stefano.ughi
\n
\n
\par	History:
\n
\n 		00.00.35
\n		June 18, 2010
\n		stefano.ughi
\n		changes:
\n      compliant to ISO/ANSI 9899-1990 6.3.4 and Annex B
\n 		00.00.34
\n		June 18, 2010
\n		stefano.ughi
\n		changes:
\n      changed macro's name EnterCritical() in EnterCriticalSection()
\n      changed macro's name ExitCritical() in ExitCriticalSection()
\n      In Config.h must be defined
\n      #if defined(OSEK_SUPPORT)
\n      #define EnterCriticalSection()  SuspendAllInterrupts()
\n      #define ExitCriticalSection()   ResumeAllInterrupts()
\n      #else
\n      #define EnterCriticalSection()  EnterCritical()
\n      #define ExitCriticalSection()   ExitCritical()
\n      #endif
\n
\n 		00.00.34
\n		Jan 27, 2011
\n		stefano.ughi
\n		changes:
\n      Fixed bug on OCFLS_RWriteVerify
\n      OCWDT_SetSync(0) call changed with OCWDT_RefreshSync()
\n
\n 		00.00.33
\n		March 15, 2010
\n		stefano.ughi
\n		changes:
\n      added only write or erase/write permission on the selected flash block
\n
\n 		00.00.30
\n		Jan 14, 2010
\n		stefano.ughi
\n		changes:
\n      due to watchdog
\n 		- added FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG property
\n		- added FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG property
\n
\n
\n
\n
\n etc.
*/
#define MODULE_BODY_OCFLS
/* ******** */
/* Includes */
/* ******** */
#include "Config.h"
#include "RTOS.h"
#include "IO_Driver.h"
#include "OCFLS_drv.h"
#include "OCWDT_drv.h"
#include "LIB_Error.h"

#if (FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG > 255)
#error "Value of FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG property out of range."
#endif

#if (FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG > 255)
#error "Value of FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG property out of range."
#endif



/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* Function's pointer for Erase Page Command Executed in RAM  */
typedef void (*pFLS_ErasePageCmdIsInRam)(FLS_paramType param);
/* Function's pointer for Burst Writing Command Executed in RAM  */
typedef void (*pFLS_BurstProgCmdIsInRam)(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);

/* ********* */
/* Variables */
/* ********* */

/* Global */
extern const FLS_MapType FlashMap[];
extern const uint8_t FlashMapEraseForbidden[];
/* Static */

/* ******************* */
/* Functions Prototype */
/* ******************* */
static IO_ErrorType OCFLS_CheckParams(FLS_sizeType size, FLS_positionType position);

/* *************** */
/* Function Bodies */
/* *************** */
/* Exported function */
/**
This function initialises the FLS driver.

\return 	IO_ErrorType

\retval		IO_E_OK succes

\author 	stefano.ughi
\date		Nov 12, 2009
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
*/
IO_ErrorType OCFLS_InitSync(void)
{
	FLS_Init();
	return (IO_ErrorType)IO_E_OK;
}


/**
This service implements the erase functionality.\n
Verify is not performed, use \b OCFLS_REraseVerify function.

\param  [in]		size  specifies the number of bytes to be erased.
\n					- \b probably more than size bytes will be erase bacause of uC flash
\n                  organization.
\n
\param  [in]		position  specifies the position where the erase operation starts.


\return 	IO_ErrorType

\retval		IO_E_INVALID_SIZE size is invalid
\retval		IO_E_OUT_OF_MEMORY position + (size-1) out of memory allocation.
\retval     IO_E_OUT_OF_BOUNDS check failed out of segments.
\retval 	IO_E_ERASE_FORBIDDEN  on the selected block only write operations are allowed
\retval		IO_E_PROGRAMMING problems during erasing procedure.
\retval		IO_E_OK erase is o.k..

\author 	stefano.ughi
\date		Jan 13, 2010
\version	1.0.1

\todo       implementazione per i micro con pagine flash di dim. non uniforme.

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
IO_ErrorType OCFLS_REraseSync (FLS_sizeType size, FLS_positionType position)
{
	IO_ErrorType err;

	static FLS_positionType EndPosition;


	/* Check if the parameters are valid */
	err = OCFLS_CheckParams(size,position);
	if( (IO_ErrorType)IO_E_OK != err )
	{
		/* Problems founded check err */
		PLT_ERROR_WARNING(PLT_ERROR_OCFLS_BAD_PARAMS);
		return err;
	}

	FLS_ERASE_BEGIN();

	EndPosition = (position+size);
	while ( position < EndPosition )
	{
		FLS_InitRegsForErasing();
		/* Enter Critical Section */
		FLS_EnterCriticalSection();
		/* Erasing */
		err = FLS_Erase(position);
		PLT_ERROR_WARNING_IF(err, PLT_ERROR_OCFLS_BAD_ERASE);

		/* Exit Critical Section */
		FLS_ExitCriticalSection();

		FLS_ERASE_END();

		/* Check Errors during erasing operation */
		if( FLS_CheckErrors() )
		{
			PLT_ERROR_WARNING(PLT_ERROR_OCFLS_BAD_PROGRAMMING);
			return (IO_ErrorType)IO_E_PROGRAMMING;
		}
		else
		{
#ifdef ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
			position+=FLASH_PAGE_SIZE_IN_BYTES;
#else
#error "ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE, not supported different page size"
#endif
		}
	}
	return (IO_ErrorType)IO_E_OK;
}

/**
This function implements write functionality.\n
We assume that flash is blank.\n
Verify is not performed, use \b OCFLS_RWriteVerify function.\n
\b HCS08: pSource must point to R.A.M. data, it is not possible to access flash during write operation.

\param  [in]	pSource  Points to the initial data where to read from.
\param	[in]	size  Specifies the number of bytes to be write.
\param	[in]	position  Specifies the start point where the data is written.

\return 	IO_ErrorType

\retval		IO_E_OK Successful write operation.
\retval		IO_E_INVALID_SIZE size is invalid.
\retval		IO_E_OUT_OF_MEMORY position + (size-1) out of memory allocation.
\retval     IO_E_OUT_OF_BOUNDS check failed out of avaible segments.
\retval		IO_E_PROGRAMMING Programming Error.

\author 	stefano.ughi
\date		Nov 17, 2009
\version	1.0.0

\warning    check inside map file the right length of routine that is performed in R.A.M.
\note		\par \b Note 1:
			documentation by stefano.ughi
*/
IO_ErrorType OCFLS_RWriteSync (FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
	IO_ErrorType err;


	/* Check if the parameters are valid */
	err = OCFLS_CheckParams(size,position);
	if( ((IO_ErrorType)IO_E_OK != err) && ((IO_ErrorType)IO_E_ERASE_FORBIDDEN != err) )
	{
		/* Problems founded check err */
		PLT_ERROR_WARNING(PLT_ERROR_OCFLS_BAD_PARAMS);
		return err;
	}

	FLS_WRITE_BEGIN();

	/* Init Flash regs */
	FLS_InitRegsForProgramming();
	/* Enter Critical Section */
	FLS_EnterCriticalSection();
	/* Programming */
	err = FLS_Programming((uint8_t *) pSource,size,position);
	PLT_ERROR_WARNING_IF(err, PLT_ERROR_OCFLS_BAD_PROGRAMMING);

	/* Exit Critical Section */
	FLS_ExitCriticalSection();

	FLS_WRITE_END();

	/* Check Errors during erasing operation */
	if( FLS_CheckErrors() )
	{
		err = (IO_ErrorType)IO_E_PROGRAMMING;
	}
	else
	{
		err = (IO_ErrorType)IO_E_OK;
	}
	return err;
}
/**
This service verify if flash is blanck.

\param  [in]		size  specifies the number of bytes to be checked.
\param	[in]		position  specifies the position where the check operation start.

\return 	IO_ErrorType

\retval     IO_E_NOT_BLANK check failed flash is not blanck.
\retval		IO_E_OK check is o.k.


\author 	stefano.ughi
\date		Nov 12, 2009
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			\b FLS_BlankCheckType must be the largest possible type in order to decrease time of checking.
			\par \b Note 3:
			\b FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG must be set in FlashConfig.h
*/
#ifdef FLS_BlankCheck
IO_ErrorType OCFLS_REraseVerify(FLS_sizeType size, FLS_positionType position)
{
	IO_ErrorType Err;
	Err = FLS_BlankCheck(size, position);
	if(Err != 0)
	{
		Err = IO_E_NOT_BLANK;
	}
	return Err;
}
#else
IO_ErrorType OCFLS_REraseVerify(FLS_sizeType size, FLS_positionType position)
{
#if (FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)
	static uint8_t NumCheck=0;
#endif

	while( size >= sizeof(FLS_BlankCheckType) )
	{
		if( FLS_BLANK_VALUE != *(FLS_BlankCheckType*)position )
		{
			return (IO_ErrorType)IO_E_NOT_BLANK;
		}
		position+=sizeof(FLS_BlankCheckType);
		size-=sizeof(FLS_BlankCheckType);

#if (FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)

		NumCheck++;
		if ( NumCheck >= FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG )
		{
			(void)OCWDT_RefreshSync();
			NumCheck = 0;
		}

#endif
	}
#if (SIZE_OF_FLS_BlankCheckType > 1)
	/* This control is not necessary if FLS_BlankCheckType is 1 byte size */
	while( size > 0 )
	{
		if( (uint8_t)FLS_BLANK_VALUE != *(uint8_t*)position )
		{
			return (IO_ErrorType)IO_E_NOT_BLANK;
		}
		position++;
		size--;
	}
#endif /* (SIZE_OF_FLS_BlankCheckType > 1) */

#if (FLS_ERASE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)
	/* For the next call */
	NumCheck = 0;
#endif

	return (IO_ErrorType)IO_E_OK;
}
#endif /* FLS_BlankCheck*/
/**
This function implements the verify functionality.\n

\param  [in]	pSource  Points to the initial data where to read from.
\param	[in]	size  Specifies the number of bytes to verify.
\param	[in]	position  Specifies the start point where the data is written.

\return 	IO_ErrorType

\retval		IO_E_OK Successful verify operation.
\retval		IO_E_VERIFY	Verify Error.

\author 	stefano.ughi
\date		Jan 27, 2011
\version	1.0.1

\note		\par \b Note 1:
			documentation by stefano.ughi
			\par \b Note 2:
			\b FLS_VerifyType must be the largest possible type in order to decrease time of checking.
			\par \b Note 3:
			\b FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG must be set in FlashConfig.
			\par \b Note 4:
			Jan 27,2011 fixed bug on pSource inc.
			\par \b Note 5:
			May 09,2011 fixed problem on pSource inc due to ISO/ANSI 9899-1990 6.3.4 and Annex B

*/
IO_ErrorType OCFLS_RWriteVerify (FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
#if (FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)
	static uint8_t NumCheck=0;
#endif
	IO_ErrorType err=IO_E_OK;
	uint8_t* pData=(uint8_t*)pSource;
	/* Check if data correctly written */
	while( (size >0) && (err == IO_E_OK))
	{
		if( *pData++ != *(uint8_t*)position++ )
		{
			err = (IO_ErrorType)IO_E_VERIFY;
		}
		size--;

#if (FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)

		NumCheck++;
		if ( NumCheck >= FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG )
		{
			(void)OCWDT_RefreshSync();
			NumCheck = 0;
		}

#endif

	}
#if 0
    {
        uint8_t * p = (uint8_t*)pSource;
        while( size > 0 )
        {    
            if( *p != *(uint8_t*)position )
            {
                return (IO_ErrorType)IO_E_VERIFY;
            }
            position++;

            p++;

            size--;
        }
    }
#endif /* ( SIZE_OF_FLS_VerifyType > 1 ) */

#if (FLS_WRITE_VERIFY_NUM_CHECK_BEFORE_FEED_THE_DOG!=0)
	/* For the next call */
	NumCheck = 0;
#endif

	return err;
}
/**
This function perform write-verify operations in flash.

\param  [in]	pSource  Points to the initial data where to read from.
\param	[in]	size  Specifies the number of bytes to be write.
\param	[in]	position  Specifies the start point where the data is written.

\attention \b	FLS_WriteSteps states of routine, used as global variable R/W

\return 	IO_ErrorType

\retval		IO_E_WORKING job running
\retval		IO_E_OK job done correctly
\retval		IO_E_UNEXPECTED_CONDITION problem occurred

\author 	stefano.ughi
\date		Jan 8, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
*/
IO_ErrorType OCFLS_WriteAndVerifyAsync(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
	static enum FLS_WriteStepsTag
	{
		FLS_WRITE= 0,
		FLS_WRITE_VERIFY,
		FLS_WRITE_SUCCES,
		FLS_ERR
	} FLS_WriteSteps= {FLS_WRITE};

	IO_ErrorType err;
	IO_ErrorType fls_State = (IO_ErrorType)IO_E_WORKING;

	switch(FLS_WriteSteps)
	{
	case FLS_WRITE:

		err=OCFLS_RWriteSync(pSource,size,position);
		if((IO_ErrorType)IO_E_OK == err)
		{
			FLS_WriteSteps = FLS_WRITE_VERIFY;
		}
		else
		{
			FLS_WriteSteps = FLS_ERR;
		}
		break;

	case FLS_WRITE_VERIFY:

		err=OCFLS_RWriteVerify(pSource,size,position);
		if((IO_ErrorType)IO_E_OK == err)
		{
			FLS_WriteSteps = FLS_WRITE_SUCCES;
		}
		else
		{
			FLS_WriteSteps = FLS_ERR;
		}
		break;

	case FLS_WRITE_SUCCES:
		/* success */
		fls_State = (IO_ErrorType)IO_E_OK;
		/* restore machine state for the next call */
		FLS_WriteSteps = FLS_WRITE;
		break;

	case FLS_ERR:
		/* problem */
		fls_State = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
		/* restore machine state for the next call */
		FLS_WriteSteps = FLS_WRITE;
		break;

	default:
		FLS_WriteSteps = FLS_WRITE;
		break;

	}
	return fls_State;
}
/**
This function perform verify-erase-verify operations in flash.

\param	[in]	size  Specifies the number of bytes to erase.
\param	[in]	position  Specifies the start point where the data is erased.

\attention \b	FLS_EraseSteps states of routine, used as global variable R/W

\return 	IO_ErrorType

\retval		IO_E_WORKING job running
\retval		IO_E_OK job done correctly
\retval		IO_E_UNEXPECTED_CONDITION problem occurred

\author 	stefano.ughi
\date		Jan 14, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
IO_ErrorType OCFLS_BlankCheckEraseVerifyAsync(FLS_sizeType size, FLS_positionType position)
{
	static enum FLS_EraseStepsTag
	{
		FLS_CHECK_IF_BLANK=0,
		FLS_ERASE,
		FLS_ERASE_VERIFY,
		FLS_ERASE_SUCCES,
		FLS_ERR
	} FLS_EraseSteps= {FLS_CHECK_IF_BLANK};

	IO_ErrorType err;
	IO_ErrorType fls_State = (IO_ErrorType)IO_E_WORKING;

	switch(FLS_EraseSteps)
	{
	case FLS_CHECK_IF_BLANK:

		err=OCFLS_REraseVerify(size,position);
		if ((IO_ErrorType)IO_E_NOT_BLANK == err)
		{
			FLS_EraseSteps = FLS_ERASE;
		}
		else
		{
			FLS_EraseSteps = FLS_ERASE_SUCCES;
		}
		break;

	case FLS_ERASE:

		err=OCFLS_REraseSync(size, position);
		if((IO_ErrorType)IO_E_OK == err)
		{
			FLS_EraseSteps = FLS_ERASE_VERIFY;
		}
		else
		{
			FLS_EraseSteps = FLS_ERR;
		}
		break;

	case FLS_ERASE_VERIFY:

		err=OCFLS_REraseVerify(size,position);
		if((IO_ErrorType)IO_E_NOT_BLANK == err)
		{
			FLS_EraseSteps = FLS_ERR;
		}
		else
		{
			FLS_EraseSteps = FLS_ERASE_SUCCES;
		}
		break;

	case FLS_ERASE_SUCCES:
		/* success */
		fls_State = (IO_ErrorType)IO_E_OK;
		/* restore machine state for the next call */
		FLS_EraseSteps = FLS_CHECK_IF_BLANK;
		break;

	case FLS_ERR:
		/* problem */
		fls_State = (IO_ErrorType)IO_E_UNEXPECTED_CONDITION;
		/* restore machine state for the next call */
		FLS_EraseSteps = FLS_CHECK_IF_BLANK;
		break;

	default:
		FLS_EraseSteps = FLS_CHECK_IF_BLANK;
		break;

	}
	return fls_State;
}
/* Local Function */
/**
This service implements the check flash in range functionality.

\param  [in]		size  specifies the number of bytes to be checked.
\param	[in]		position  specifies the position where the check operation starts.

\return 	IO_ErrorType

\retval		IO_E_INVALID_SIZE size is invalid.
\retval		IO_E_OUT_OF_MEMORY position + (size-1) out of memory allocation.
\retval     IO_E_OUT_OF_BOUNDS check failed out of segments.
\retval 	IO_E_ERASE_FORBIDDEN check is o.k. but on the selected block only write operations are permitted
\retval		IO_E_OK check is o.k. and all operations are permitted

\author 	stefano.ughi
\date		Nov 11, 2009
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
static IO_ErrorType OCFLS_CheckParams(FLS_sizeType size, FLS_positionType position)
{
	FLS_sizePLUSpositionType endPosition;
	uint8_t j;

	if( (FLS_sizeType)0 == size )
	{
		return (IO_ErrorType)IO_E_INVALID_SIZE;
	}
	endPosition = ((FLS_sizePLUSpositionType)position + (size-1));
	if( endPosition > (FLS_sizePLUSpositionType)FLS_MAX_ADDR )
	{
		return (IO_ErrorType)IO_E_OUT_OF_MEMORY;
	}
	for( j = 0; j < NUM_FLASH_BLOCKS; j++ )
	{
		if( ( position >= FlashMap[j].StartAddress ) &&
		        ( endPosition <= FlashMap[j].EndAddress ) )
		{
			if( FlashMapEraseForbidden[j] )
			{
				return (IO_ErrorType)IO_E_ERASE_FORBIDDEN;
			}
			else
			{
				return (IO_ErrorType)IO_E_OK;
			}
		}
	}
	return (IO_ErrorType)IO_E_OUT_OF_BOUNDS;
}



/* *********** */
/* End of file */
/* *********** */

