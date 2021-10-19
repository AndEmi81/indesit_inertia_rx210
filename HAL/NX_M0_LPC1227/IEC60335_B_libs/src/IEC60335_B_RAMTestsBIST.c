/****************************************************************************
 *   $Id:: IEC60335_B_RAMTestsBIST.c 7461 2011-06-01 13:01:58Z gerritdewaar#$
 *   Project: NXP Cortex-M0 IEC60335 Class B certified library
 *
 *   Description:
 *     Source file for the IEC60335 Class B RAM (variable memory) BIST testing 
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

#include "IEC60335.h"
type_testResult RamPostTestStatus; //MS
/***************************************************************************//**
** @brief 		March increase testing
** @param[in]	startAddrs		Defines the start address of the memory range to be tested
** @param[in]	length			Defines the length of the memory range to be tested
** @param[in]	*pntr			Pointer to the current address tested
** @param[in]	pat				Contains the pattern that will be written to the address tested
** @param[in]	rd_cntr			With this variable the number of read cycles of the tested memory range can be defined
** @param[in]	wr_cntr			With this variable the number of write cycles of the tested memory range can be defined
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_marchIncr (UINT32 startAddrs, UINT32 length, UINT32 *pntr, UINT32 pat, UINT8 rd_cntr, UINT8 wr_cntr );
type_testResult IEC60335_marchIncr (UINT32 startAddrs, UINT32 length, UINT32 *pntr, UINT32 pat, UINT8 rd_cntr, UINT8 wr_cntr )
{
	type_testResult result = IEC60335_testPassed;
	UINT8	wr_tmp = wr_cntr, rc_tmp = rd_cntr;

	for (; (pntr< (UINT32 *)((UINT32)(startAddrs + length))) && (result == IEC60335_testPassed ); pntr+=2)
	{
		wr_tmp = (UINT8)wr_cntr;
		while (wr_tmp)
		{
			*pntr = (UINT32)pat;
			wr_tmp--;
		}

		if ((UINT32)*pntr != (UINT32)pat)
		{
			result = IEC60335_testFailed;
		}
		*pntr = (UINT32)(~pat);

		rc_tmp = (UINT8)rd_cntr;
		while (rc_tmp)
		{
			if ((UINT32)*pntr != (UINT32)(~pat))
			{
				result = IEC60335_testFailed;
			}
			rc_tmp--;
		}
	}
	return (result);
}

/***************************************************************************//**
** @brief 		March decrease testing
** @param[in]	startAddrs		Defines the start address of the memory range to be tested
** @param[in]	length			Defines the length of the memory range to be tested
** @param[in]	*pntr			Pointer to the current address tested
** @param[in]	pat				Contains the pattern that will be written to the address tested
** @param[in]	rd_cntr			With this variable the number of read cycles of the tested memory range can be defined
** @param[in]	wr_cntr			With this variable the number of write cycles of the tested memory range can be defined
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_marchDecr (UINT32 startAddrs, UINT32 length, UINT32 *pntr, UINT32 pat, UINT8 rd_cntr, UINT8 wr_cntr);
type_testResult IEC60335_marchDecr (UINT32 startAddrs, UINT32 length, UINT32 *pntr, UINT32 pat, UINT8 rd_cntr, UINT8 wr_cntr)
{
	type_testResult result = IEC60335_testPassed;
	UINT8	wr_tmp = wr_cntr, rc_tmp = rd_cntr;

	for (; (pntr >= (UINT32 *)((UINT32)startAddrs)) && (result == IEC60335_testPassed ); pntr-=2)
	{
		wr_tmp = (UINT8)wr_cntr;
		while (wr_tmp)
		{
			*pntr = (UINT32)pat;
			wr_tmp--;
		}

		if ((UINT32)*pntr != (UINT32)pat)
		{
			result = IEC60335_testFailed;
		}
		*pntr = (UINT32)(~pat);

		rc_tmp = (UINT8)rd_cntr;
		while (rc_tmp)
		{
			if (*pntr != (UINT32)(~pat))
			{
				result = IEC60335_testFailed;
			}
			rc_tmp--;
		}
	}

	return (result);
}

/***************************************************************************//**
** @brief 		This function executes sequentially the nine march tests. The user can use 
**				this function to execute a RAM test on a defined memory range.
** @param[in]	startAddrs		Defines the start address of the memory range to be tested
** @param[in]	length			Defines the length of the memory range to be tested
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_RAMtest (UINT32 startAddrs, UINT32 length)
{
	UINT32 	tmp = 0;
	UINT32  *pntr = (UINT32 *)((UINT32)startAddrs);
	type_testResult result = IEC60335_testPassed;


	/** MARCH 0 *****************************************/
	for (; pntr < (UINT32 *)((UINT32)(startAddrs+length)) ; pntr++)
	{
		*pntr =  (UINT32)PATTERN;
	}

	/** MARCH 1 *****************************************/
	/* march 1_even */
	isEven_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 1_odd */
	isOdd_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 2 *****************************************/
	/* march 2_even */
	isEven_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, ~PATTERN, 5, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 2_odd */
	isOdd_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, ~PATTERN, 5, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 3 *****************************************/
	/* march 3_even */
	isEven_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 3_odd */
	isOdd_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 4 *****************************************/
	/* march 4_even */
	isEven_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, ~PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 4_odd */
	isOdd_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, ~PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 5 *****************************************/
	/* march 5_even */
	isEven_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 5, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 5_odd */
	isOdd_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 5, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 6 *****************************************/
	/* march 6_even */
	isEven_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 0, 1) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 6_odd */
	isOdd_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, PATTERN, 0, 1) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 7 *****************************************/
	/* march 7_even */
	isEven_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, ~PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 7_odd */
	isOdd_down(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp + length-4);
	if (IEC60335_marchDecr(startAddrs, length, pntr, ~PATTERN, 0, 0) == IEC60335_testFailed) return (IEC60335_testFailed);

	/** MARCH 8 *****************************************/
	/* march 8_even */
	isEven_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, ~PATTERN, 0, 1) == IEC60335_testFailed) return (IEC60335_testFailed);

	/* march 8_odd */
	isOdd_up(startAddrs, tmp);
	pntr = (UINT32 *)((UINT32)tmp);
	if (IEC60335_marchIncr(startAddrs, length, pntr, ~PATTERN, 0, 1) == IEC60335_testFailed) return (IEC60335_testFailed);

	return (result);
}

/***************************************************************************//**
** @brief 		This function executes sequentially the nine march tests in BIST-mode. 
**				The user can use this function to execute a RAM test on a defined memory range.
** @param[in]	startAddrs		Defines the start address of the memory range to be tested
** @param[in]	length			Defines the length of the memory range to be tested
** @return		type_testResult IEC60335 Pass/Fail return See @link #tag_testResult type_testResult @endlink
*****************************************************************************/
type_testResult IEC60335_RAMtest_BIST(UINT32 startAddrs, UINT32 length)
{
    return (IEC60335_RAMtest(startAddrs, length));
}
