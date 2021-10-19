/****************************************************************************
 *   $Id:: IEC60335_B_FLASHTest.h 7460 2011-06-01 11:53:27Z gerritdewaard   $
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Include file for the ROM test in the IEC60335 Class B library.
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

#ifndef IEC60335_B_FLASHTEST_H_
#define IEC60335_B_FLASHTEST_H_

#define	FLASH_HARD_SIGN 1
#define FLASH_SOFT_SIGN	2

#define MISR_START	(1<<17)
#define EOM		(0x01<<2)	/* Signature Generation */


typedef struct IEC60335_FlashSign_struct {
	UINT32 word0;
	UINT32 word1;
	UINT32 word2;
	UINT32 word3;
} FlashSign_t;

typedef UINT32 Crc32Sig_t;
typedef UINT16 Crc16Sig_t;
typedef UINT16 CcittSig_t;

/*!
 * @brief IEC60335 ROM test (BIST)
 *        Function call for starting the Hardware MISR signature generator
 */
void StartHardSignatureGen (UINT32 startAddr, UINT32 length, FlashSign_t *ResultSign);

/*!
 * @brief IEC60335 ROM test (BIST)
 *        Function call for starting the Software MISR signature generator
 */
void StartSoftSignatureGen (UINT32 startAddr, UINT32 length, FlashSign_t *pResultSign);

/*!
 * @brief IEC60335 ROM test (BIST)
 *        Main Function call for starting the Hardware MISR in BIST mode
 */
type_testResult IEC60335_FLASHtestMISR_BIST (UINT32 startAddr,
											UINT32 length,
											FlashSign_t *TestSign,
											UINT8 	selectHS);

/*!
 * @brief IEC60335 ROM test (BIST)
 *        Function call for validating two generated/calculated signatures
 */
type_testResult IEC60335_testMISRSignatures (FlashSign_t *sign1, FlashSign_t *sign2);

/*!
 * @brief IEC60335 ROM test (BIST)
 *        Main Function call for the ASM-based BIST test
 */
type_testResult _FLASHTestBIST (void);

/*!
 * @brief IEC60335 ROM test (POST)
 *        Main Function call for the ASM-based POST test
 */
void _FLASHTestPOST (void);

#endif /* IEC60335_B_FLASHTEST_H_ */
