/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		SWCRC_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		SWCRC_drv.c
\ingroup	SWCRC
\date		Apr 06, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Apr 06,2010
\n		Stefano Ughi
\n		First Insertion
\n
\n 		01.00.01
\n		Apr 07,2011
\n		Stefano Ughi
\n		- Added IO_ErrorType CRC_InitSync(void)
\n      - Added IO_ErrorType CRC_DeInitSync(void)
\n
*/

/* ******** */
/* Includes */
/* ******** */
#include "SWCRC_drv.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */
/* loop driven crc_ccitt calculation */
#define WIDTH (8 * sizeof(uint16_t))
#define TOPBIT (1u << (WIDTH - 1))
#define POLYNOMIAL	0x1021

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* Static */
#if defined(CONFIG_PL_DRV_SWCRC_TABLEDRIVEN)

/** CRC_CCITT Table driven computation */
static const uint16_t Table_CCITT[256]=
{
	/*            0,     1,     2,     3,     4,     5,     6,     7,*/
	/*   0 */     0,  4129,  8258, 12387, 16516, 20645, 24774, 28903,
	/*   8 */ 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935,
	/*  16 */  4657,   528, 12915,  8786, 21173, 17044, 29431, 25302,
	/*  24 */ 37689, 33560, 45947, 41818, 54205, 50076, 62463, 58334,
	/*  32 */  9314, 13379,  1056,  5121, 25830, 29895, 17572, 21637,
	/*  40 */ 42346, 46411, 34088, 38153, 58862, 62927, 50604, 54669,
	/*  48 */ 13907,  9842,  5649,  1584, 30423, 26358, 22165, 18100,
	/*  56 */ 46939, 42874, 38681, 34616, 63455, 59390, 55197, 51132,
	/*  64 */ 18628, 22757, 26758, 30887,  2112,  6241, 10242, 14371,
	/*  72 */ 51660, 55789, 59790, 63919, 35144, 39273, 43274, 47403,
	/*  80 */ 23285, 19156, 31415, 27286,  6769,  2640, 14899, 10770,
	/*  88 */ 56317, 52188, 64447, 60318, 39801, 35672, 47931, 43802,
	/*  96 */ 27814, 31879, 19684, 23749, 11298, 15363,  3168,  7233,
	/* 104 */ 60846, 64911, 52716, 56781, 44330, 48395, 36200, 40265,
	/* 112 */ 32407, 28342, 24277, 20212, 15891, 11826,  7761,  3696,
	/* 120 */ 65439, 61374, 57309, 53244, 48923, 44858, 40793, 36728,
	/* 128 */ 37256, 33193, 45514, 41451, 53516, 49453, 61774, 57711,
	/* 136 */  4224,   161, 12482,  8419, 20484, 16421, 28742, 24679,
	/* 144 */ 33721, 37784, 41979, 46042, 49981, 54044, 58239, 62302,
	/* 152 */   689,  4752,  8947, 13010, 16949, 21012, 25207, 29270,
	/* 160 */ 46570, 42443, 38312, 34185, 62830, 58703, 54572, 50445,
	/* 168 */ 13538,  9411,  5280,  1153, 29798, 25671, 21540, 17413,
	/* 176 */ 42971, 47098, 34713, 38840, 59231, 63358, 50973, 55100,
	/* 184 */  9939, 14066,  1681,  5808, 26199, 30326, 17941, 22068,
	/* 192 */ 55628, 51565, 63758, 59695, 39368, 35305, 47498, 43435,
	/* 200 */ 22596, 18533, 30726, 26663,  6336,  2273, 14466, 10403,
	/* 208 */ 52093, 56156, 60223, 64286, 35833, 39896, 43963, 48026,
	/* 216 */ 19061, 23124, 27191, 31254,  2801,  6864, 10931, 14994,
	/* 224 */ 64814, 60687, 56684, 52557, 48554, 44427, 40424, 36297,
	/* 232 */ 31782, 27655, 23652, 19525, 15522, 11395,  7392,  3265,
	/* 240 */ 61215, 65342, 53085, 57212, 44955, 49082, 36825, 40952,
	/* 248 */ 28183, 32310, 20053, 24180, 11923, 16050,  3793,  7920,
};
#endif  /* CRC_SW_TABLE_DRIVEN */

/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Module's init.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Stefano Ughi
\date		Feb 11,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi

*/
IO_ErrorType SWCRC_InitSync(void)
{
	return (IO_ErrorType)IO_E_OK;
}
/**
Deinit CRC module, M.C.U. reset state.

\return 	IO_ErrorType

\retval		IO_E_OK

\author 	Stefano Ughi
\date		Feb 11,2011
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Stefano Ughi
			\par \b Note 2:
			After an CRC_DeInitSync call it's mandatory to call a CRC_InitSync\n
			if you want use the driver.

*/
IO_ErrorType SWCRC_DeInitSync(void)
{
	return (IO_ErrorType)IO_E_OK;
}

#if !defined (CONFIG_PL_DRV_SWCRC_TABLEDRIVEN)
/**
Crc CCITT loop driven computation.
Polynomial: x^16+x^12+x^5+1

\param  [in]		len number of bytes on which CRC is calculated
\param  [in]		pSource point to the first byte of the array on which CRC is calculated
\param  [in]		seed is the initial value for CRC computation, typically 0xFFFF but\n
                    if you have some fixed bytes you may precompute CRC and pass it as seed.
                    (E.g. Master Mode Tx)

\return 	uint16_t

\retval		crc


\author 	stefano.ughi
\date		Apr 6, 2010
\version	1.0.0

\todo       Insert WatchDog Refresh

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint16_t SWCRC_Calculate (IO_BufferSizeType len, uint8_t *pSource, uint16_t seed)
{
	IO_BufferSizeType i;
	uint8_t	*pData=pSource;
	uint16_t crc;
	uint8_t bit;

	crc = seed;

	for(i=0; i<len; i++)
	{
		/*
		Bring the next byte into the remainder.
		*/
		crc ^= (*pData << (WIDTH - 8));

		/*
		Perform modulo-2 division, a bit at a time.
		*/
		for (bit = 0; bit < 8; bit++)
		{
			/*
			Try to divide the current data bit.
			*/
			if (crc & TOPBIT)
			{
				crc = (uint16_t) ((crc<<1)^(POLYNOMIAL));
			}
			else
			{
				crc = (uint16_t) (crc<<1);
			}
		}

		pData++;
	}

	return crc;

}
#else
/**
Crc CCITT table driven computation.
Polynomial: x^16+x^12+x^5+1

\param  [in]		len number of bytes on which CRC is calculated
\param  [in]		pSource point to the first byte of the array on which CRC is calculated
\param  [in]		seed is the initial value for CRC computation, typically 0xFFFF but\n
                    if you have some fixed bytes you may precompute CRC and pass it as seed.\n
                    (E.g. Master Mode Tx)

\return 	uint16_t

\retval		crc


\author 	stefano.ughi
\date		Apr 6, 2010
\version	1.0.0

\todo       Insert WatchDog Refresh

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint16_t SWCRC_Calculate( IO_BufferSizeType len, uint8_t *pSource,uint16_t seed )
{
	IO_BufferSizeType i;
	uint8_t	*pData=pSource;
	uint16_t crc;
	uint8_t tmp;

	crc = seed;

	for(i=0; i<len; i++)
	{

		tmp = (crc >> 8) ^ *pData;
		crc = (crc << 8) ^ Table_CCITT[tmp];
		pData++;

	}

	return crc;

}
#endif /* CONFIG_PL_DRV_SWCRC_TABLEDRIVEN */

/* Local Function */

/* *********** */
/* End of file */
/* *********** */

