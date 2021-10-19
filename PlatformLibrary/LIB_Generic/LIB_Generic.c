/*
Copyright (c) 2004-2013, Indesit Company
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	LibGeneric
\n	This module contains generic routines
\n
\file		Lib_Generic.c
\ingroup        LIB_Generic
\date	    2013-02-21
\version	01.00.00
\author		 Roberto Fiorvanti
\n
\n
\par	History:
\n
\n 		01.00.00
\n		2013-02-21
\n		Roberto Fiorvanti
\n		changes
\n 			Creation
\n
*/

/* ******** */
/* Includes */
/* ******** */


#include "LIB_Generic.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported functions */

/**
Partial modified checksum computation.

\param  [in]		len number of bytes of which CHK must be computed
\param  [in]		pSource pointer to the first byte of the array of which CRC must be computed
\param  [in]		initval is the initial value for CHK computation

\return 	LIB_ModifiedChkType

\retval		modified chk


\author 	Roberto Fioravanti
\date		Feb 21, 2013
\version	1.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
 */
LIB_ModifiedChkType	LIB_ModifiedChk_Computation(uint16_t len, uint8_t* pSource, LIB_ModifiedChkType initval)
{
	LIB_ModifiedChkType res = initval;
	uint8_t* pMem=pSource;
	uint8_t* pEnd=pSource+len;

	if(pEnd > pSource)
	{
		while ( pMem < pEnd )
		{
			res ^= *pMem;
			res = (uint16_t)(((res & 0x8000)? 1 : 0) | (res<<1));
			pMem++;
		}
	}
	return res;
}

/**
modified 16-bit checksum computation.

\param  [in]		len number of words (16 bits) of which CHK must be computed
\param  [in]		pSource pointer to the first byte of the array of which CRC must be computed
\param  [in]		initval is the initial value for CHK computation

\return 	LIB_ModifiedChkType

\retval		modified chk


\author 	Roberto Fioravanti
\date		Feb 21, 2013
\version	1.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
 */
LIB_ModifiedChkType	LIB_ModifiedChk16_Computation(uint16_t len, uint16_t* pSource)
{
	LIB_ModifiedChkType chk = 0;
	uint16_t temp = 0;

	int  i = 0;
	uint16_t c = 0;
	uint16_t temp2 = 0;

	for(i=0; i<len; i++)
	{
		temp = *pSource;

		chk ^= temp;

		temp2 = c | (chk<<1);

		if(chk & 0x8000)
		{
			c = 1;
		}
		else
		{
			c = 0;
		}
		chk = temp2;

		pSource++;
	}

	return chk;
}



/* Local functions */
