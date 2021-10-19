/****************************************************************************
 *   $Id:: IEC60335_B_FLASHTest.c 7461 2011-06-01 13:01:58Z gerritdewaard   $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B FLASH (invariable memory) testing 
 *	   library.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/

#ifdef LPC1127
#include "lpc11xx.h"
#endif // LPC1127

#include "IEC60335.h"

/***************************************************************************//**
** @brief 		Function starts the execution of the hardware signature generation.
** @param[in]	startAddrs		This variable is the starting address of where the signature generation will start.
** @param[in]	length			The length variable is the region size to used for the signature generation.
** @param[in]	*pResultSign	The result after generation completion will be put in the pointed location by the pResultSign pointer.
** @return		NONE
*****************************************************************************/
#ifdef LPC1127
void StartHardSignatureGen (UINT32 startAddr, UINT32 length, FlashSign_t *pResultSign)
{
	/* align flash address to refer the flash word in the array*/
    startAddr = (startAddr >> 4) & 0x0001ffff;
    length  = 	((startAddr + length) >> 4)  & 0x0001ffff;

    /* write start address of the flash contents to the register*/
    FMC->FMSSTART = startAddr;

    /* write stop address of the flash contents to the register, start generating the signature*/
    FMC->FMSSTOP = length | MISR_START;

    /* wait for signature to be generated */
    while ((FMC->STATUS & EOM) != EOM);
    FMC->CLR_STATUS = EOM;

	/* Store the signatures in the structure */
    pResultSign->word0 = FMC->FMSW0;
    pResultSign->word1 = FMC->FMSW1;
    pResultSign->word2 = FMC->FMSW2;
    pResultSign->word3 = FMC->FMSW3;

    return;
}
#else
//#error "StartHardSignatureGen not supported"    
void StartHardSignatureGen (UINT32 startAddr, UINT32 length, FlashSign_t *pResultSign)
{
  return;  
}
#endif // LPC1227


/***************************************************************************//**
** @brief 		Function starts the execution of the software signature generation.
** @param[in]	startAddrs		This variable is the starting address of where the signature generation will start.
** @param[in]	length			The length variable is the region size to used for the signature generation.
** @param[in]	*pResultSign	The result after generation completion will be put in the pointed location by the pResultSign pointer.
** @return		NONE
*****************************************************************************/
void StartSoftSignatureGen (UINT32 startAddr, UINT32 length, FlashSign_t *pResultSign)
{
	FlashSign_t flashWord;
	FlashSign_t refSignature = {0,0,0,0};
	FlashSign_t nextSign;

	UINT32 		*PageAddr = 0,
				i;

	PageAddr = (UINT32 *)((UINT32)startAddr);

	for ( i = 0; i <= (length>>4); i++ )
	{
		flashWord.word0 = *PageAddr;
		PageAddr++;
		flashWord.word1 = *PageAddr;
		PageAddr++;
		flashWord.word2 = *PageAddr;
		PageAddr++;
		flashWord.word3 = *PageAddr;
		PageAddr++;

	    /* update 128 bit signature */
	    nextSign.word0 = flashWord.word0 ^ refSignature.word0>>1 ^ refSignature.word1<<31;
	    nextSign.word1 = flashWord.word1 ^ refSignature.word1>>1 ^ refSignature.word2<<31;
	    nextSign.word2 = flashWord.word2 ^ refSignature.word2>>1 ^ refSignature.word3<<31;
	    nextSign.word3 = flashWord.word3 ^ refSignature.word3>>1 ^
	                     ( refSignature.word0 & 1<<29 )<<2 ^
	                     ( refSignature.word0 & 1<<27 )<<4 ^
	                     ( refSignature.word0 & 1<<2 )<<29 ^
	                     ( refSignature.word0 & 1<<0 )<<31;

	    /* point to the calculated value */
	    refSignature.word0 = nextSign.word0;
	    refSignature.word1 = nextSign.word1;
	    refSignature.word2 = nextSign.word2;
	    refSignature.word3 = nextSign.word3;
	}

	/* Copy the reference signature to the result pointer */
	pResultSign->word0 = refSignature.word0;
	pResultSign->word1 = refSignature.word1;
	pResultSign->word2 = refSignature.word2;
	pResultSign->word3 = refSignature.word3;

}

/***************************************************************************//**
** @brief 		Function starts the execution of the hardware signature generation in the LPC11xx.
** @param[in]	startAddrs		This variable is the starting address of where the signature generation will start.
** @param[in]	length			The length variable is the region size to used for the signature generation.
** @param[in]	*pResultSign	The result after generation completion will be put in the pointed location by the pResultSign pointer.
** @param[in]	selectHS		Hardware or software signature generation selection byte, FLASH_HARD_SIGN or FLASH_SOFT_SIGN should be used.
** @return		type_testResult IEC60335 Pass/Fail return
*****************************************************************************/
type_testResult IEC60335_FLASHtestMISR_BIST (UINT32 startAddr, UINT32 length, FlashSign_t *pTestSign, UINT8 selectHS)
{
	FlashSign_t ResultSign = {0,0,0,0};

	/* Select hardware or software signature generation */
	if (selectHS == FLASH_HARD_SIGN)
	{
		/* Do a hardware signature generation */
		StartHardSignatureGen (startAddr, length, &ResultSign);		
	}
	else if (selectHS == FLASH_SOFT_SIGN)
	{
		/* Do a hardware signature generation */
		StartSoftSignatureGen (startAddr, length, &ResultSign);
	}
	else
	{
 		/* No Hard or Soft signature generation is selected, return fail */
		return (IEC60335_testFailed);
	}

	/* Test the result signature against the test signature */
	if (IEC60335_testMISRSignatures(&ResultSign, pTestSign) == IEC60335_testFailed)
		return (IEC60335_testFailed);

	return (IEC60335_testPassed);
}

/***************************************************************************//**
** @brief 		Function compares two signatures generated using the MISR algorithm 
**				on the LPC11x and returns a pass or fail if equal or not.
** @param[in]	*sign1			Pointer to the first signature to be tested.
** @param[in]	*sign2			Pointer to the second signature to be tested.
** @return		type_testResult IEC60335 Pass/Fail return
*****************************************************************************/
type_testResult IEC60335_testMISRSignatures (FlashSign_t *sign1, FlashSign_t *sign2)
{
	/* Test the signatures to the expected value */
	if (sign1->word0 != sign2->word0) return (IEC60335_testFailed);
	if (sign1->word1 != sign2->word1) return (IEC60335_testFailed);
	if (sign1->word2 != sign2->word2) return (IEC60335_testFailed);
	if (sign1->word3 != sign2->word3) return (IEC60335_testFailed);

	/* Return a Flash test Pass */
	return (IEC60335_testPassed);
}


