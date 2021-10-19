/*
Copyright (c) 2009-2010, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		Descrizione
\n		Implementazione funzioni di utilità.
\n
\file		LIB_Utilities.c
\ingroup	PM_Library
\date		12/05/2009
\version	01.00.00
\author		Roberto Fioravanti
\n
\n
\par		History
\n
\n 		00.00.75
\n		25/10/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of SFGetDisplacementNumber method
\n
\n 		00.00.59
\n		03/09/2010
\n		Stefano Ughi
\n		changes
\n 		- added uint8_t WboxLoader(uint8_t displacement, Loader_Struct* pDataOut);
\n
\n 		00.00.58
\n		01/09/2010
\n		Stefano Ughi
\n		changes
\n 		- added uint8_t BasicLoader(settingPntr_t pointer, uint8_t displacement, Loader_Struct* pDataOut);
\n
\n 		00.00.55
\n		30/07/2010
\n		Roberto Fioravanti
\n		changes
\n 		- LIB_ComputeSettingFileCheckSum available #if defined(HAL_FLS_SUPPORT)
\n
\n 		00.00.39
\n		12/04/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addiction LIB_ChkComputation
\n
\n 		00.00.26
\n		12/01/2010
\n		Stefano Ughi
\n		changes
\n 		- LIB_ComputeSettingFileCheckSum added
\n      - LIB_ConvTwoByteInWord added
\n
\n 		00.00.25
\n		08/01/2009
\n		Roberto Fioravanti
\n		changes
\n 		- LIB_Delay_us
\n
\n 		00.00.12
\n		08/10/2009
\n		Roberto Fioravanti
\n		changes
\n 		- _NOP_();
\n
\n 		00.00.05
\n		26/06/2009
\n		Roberto Fioravanti
\n		changes
\n 		- LIB_Delay_us
\n 		- LIB_MemcpyFromLast

\n etc.
*/





/* ******** */
/* Includes */
/* ******** */
#include "LIB_Utilities.h"
#include "LIB_Setting.h"
#include "LIB_Generic.h"
#include "LIB_crcExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Setting file pointer Checksum */

#if !defined(FCT_START_ADDR) && defined(APPL_WBOX_ADDRESS)
#define FCT_START_ADDR        APPL_WBOX_ADDRESS
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* **************************** */
/* Prototype of local functions */
/* **************************** */


/* ****************** */
/* Exported variables */
/* ****************** */
extern const unsigned char Eeprom[];

#if defined(CONFIG_LIB_SETTING_CALIBRATION_ON_WBOX)
extern const uint16_t WBOXData[];
#endif
/* **************** */
/* Global variables */
/* **************** */


/* ***************** */
/* Exported function */
/* ***************** */

uint16_t Endian16_Swap(uint16_t value)
{
	return (uint16_t)(((value & (uint16_t)0x00FF) << 8) | ((value & (uint16_t)0xFF00) >> 8));
}


/**
Copies until 255 bytes from last.
\warning

\note
\version		1.0.0
\date 			06/07/2009
\author			Roberto Fioravanti
*/
void LIB_MemcpyFromLast
(
  void *pDst, 					/* Destination buffer */
  const void *pSrc, 		/* Source buffer */
  uint8_t n   					/* Byte count */
)
{
  uint8_t * sd = (uint8_t *)pDst+n-1;
  const uint8_t * ss = (const uint8_t *)pSrc+n-1;
	while(n--)
	{
	  *sd--=*ss--;
	}
}
/**
Setting File CheckSum Computation.

\param  [in]		Start checksum start point computation.
\param  [in]		End checksum end point computation.

\attention \b  SF_NUM_BYTES_TO_CHECK must be fit by user.

\return 	LIB_CompChkType

\retval		jobDone TRUE/FALSE if TRUE checkSum avaible.
\retval		checkSum checksum computed.

\author 	stefano.ughi
\date		Jan 12, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
*/
#if defined(HAL_FLS_SUPPORT)
LIB_CompChkType LIB_ComputeSettingFileCheckSum(FLS_positionType Start,FLS_positionType End)
{
LIB_CompChkType libCompChk;
LIB_NumBytesToCheckType i;
static uint16_t PartialChecksum;
static enum CompChkStateTag
{
    COMP_CHK_INIT=0,
    COMP_CHK_RUN,
    COMP_CHK_END
}CompChkState={COMP_CHK_INIT};
static const uint8_t *pData;

	libCompChk.jobDone = FALSE;
    switch(CompChkState)
    {
        case COMP_CHK_INIT:
            PartialChecksum = 0;
            pData = (uint8_t*)Start;
            CompChkState = COMP_CHK_RUN;
            /*lint -fallthrough */
        case COMP_CHK_RUN:
            for ( i = 0; i < SF_NUM_BYTES_TO_CHECK; i++ )
            {
                if ( pData < (uint8_t*)End )
                {
                    PartialChecksum ^= *pData;
                    PartialChecksum = (uint16_t)(((PartialChecksum & 0x8000) ? 1 : 0) | (PartialChecksum << 1));
                    pData++;
                }
                else
                {
                    i = SF_NUM_BYTES_TO_CHECK;
                    CompChkState = COMP_CHK_END;
                }
            }
            break;
        case COMP_CHK_END:
        	libCompChk.jobDone = TRUE;
        	libCompChk.checkSum = PartialChecksum;
        	/* Restore machine state for the next call */
        	CompChkState = COMP_CHK_INIT;
            break;
        default:
            CompChkState = COMP_CHK_INIT;
            break;
    }
    return(libCompChk);
}

/**
Calculate partial checksum - setting file algorithm.

\param  [in]            Length of chunk to process
\param  [in]            Pointer to actual position
\param  [in]            Partial 16-bit checksum (given from a previous call - 0 for first time)

\return         uint16_t - (partial) Checksum updated


\author         Alessandro Sabbatini
\date           Aug 30, 2012
\version        1.0.0

*/
uint16_t LIB_SettingFileChecksumCalculate(uint16_t len, uint8_t *pSource, uint16_t partialChecksum)
{
    int i;
    for( i=0; i<len; i++)
    {
        partialChecksum ^= *pSource;
        partialChecksum = (uint16_t) (((partialChecksum & 0x8000)? 1 : 0) | (partialChecksum<<1));
        pSource++;
    }

    return partialChecksum;
}

#endif



/**
Converts two consecutive bytes in a word

\param  [in]		pBuf pointer to first byte M.S.B.

\return 	uint16_t

\retval		1th byte*256+2nd byte

\author 	stefano.ughi
\date		Jan 14, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint16_t LIB_ConvTwoByteInWord(const uint8_t *const pBuf)
{
    return (uint16_t) ((*pBuf << 8) | *(pBuf + 1));
}
/**
Swap a data word.

\param  [in]	data

\return 	uint16_t

\author 	stefano.ughi
\date		Feb 01, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
uint16_t LIB_SwapWord(uint16_t data)
{
uint16_t swapWord;
    swapWord = (uint16_t)(((uint16_t)(data << 8) | (data >> 8)));
    return (swapWord);
}

/* Setting file manipulation Utilities */

/**
Returns the start address of the setting.

\return 	uint16_t *		Pointer to Setting file area

\author 	Roberto Fioravanti
\date		Apr 18,2013
\version	1.0.0
*/
uint16_t* GetDefaultSettingStartAddress(void)
{
	return (uint16_t*)(&Eeprom[0]);
}



/**
This function extract address and size of data related to a given displacement
of a given pointer belonging to a setting file.
The start address of the Setting file is provided as parameter .

\param  [in]		pdataStartAddr: start address of the setting file.
\param  [in] 	  pointer: Setting file pointer. This value corresponds to the position in word
                respect to the SF start address of the pointer value.(see setting file documentation)
\param  [in] 	  displacement: index of the displscement. (see setting file documentation)
\param  [out] 	pDataOut: is composed by the absolute address and the size (in words) of the returned data

\return 	uint8_t

\retval	 SF_OK : OK
\retval  SF_LDR_ERR_INVALID_DISPL: invalid displ value
\retval  SF_LDR_ERR_INVALID_POINTER: invalid pointer value
\retval  SF_LDR_ERR_OVFL_EXTRAPTR: too many extrapointers respect to a MAX defined value
\retval  SF_LDR_ERR_ENDOFPOINTER: displacement is > of the last available displacement
\retval  SF_LDR_WRN_LENGHT : data has length =0


\author 	Andrea.Mantovani
\date		Feb X, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
uint8_t SFBasicLoader(uint16_t* pdataStartAddr, uint8_t pointer, uint8_t displacement, Loader_Struct* pDataOut)
{
  uint16_t* pDataTmp;
  uint16_t nPage, pointerVal, ui16DisplNum;
  uint8_t* pData, *pData2;

  uint8_t nExtraPointerWords;
  uint8_t i;


  if (0xFF == displacement)
  {
    /**
      0xFF is the last admitted value and is an odd value and for this reason it must be a dummy
      displacemnt cause the size cannot be calculated
      */
    return SF_LDR_ERR_INVALID_DISPL;
  }


  pDataTmp  = pdataStartAddr ;
  pDataTmp += pointer;

  pointerVal = *(pDataTmp);


    if ((NULL_POINTER == pointerVal) || (INVALID_POINTER == pointerVal))
    {
      return SF_LDR_ERR_INVALID_POINTER;
    }


  pdataStartAddr += pointerVal;
  pData = (uint8_t*)(pdataStartAddr);
  nExtraPointerWords = *pData;



    if (nExtraPointerWords > N_MAX_EXTRAPTR_WORDS)
    {
      return SF_LDR_ERR_OVFL_EXTRAPTR;
    }



 /* Checks the ExtraPointer and calculates the page */
  nPage = 0;
  for (i = 1; i < (2 * nExtraPointerWords); i++)
  {
    /* displacement < 0xFF */
    if (displacement >= OFFS_B(pData, i))
    {
      nPage += 0x0100U;
    }
  }

/* pData points to the fist byte of the displacement area */
  pData += (2 * nExtraPointerWords);


   ui16DisplNum = (uint16_t)((2*(uint8_t)(*pData)) - 1);

  /* pData2 points to the last dummy displacement */
   pData2 =pData+ui16DisplNum;
   if (*pData2 == *(pData2-1))
   {
      ui16DisplNum -=1;
   }

    if (displacement >= ui16DisplNum)
    {
      return SF_LDR_ERR_ENDOFPOINTER;
    }

  /*
    Calculation is valid also in case *( EepData + Displacement + 1 ) < *( EepData + Displacement )
    cause an overflow page occurs.
  */

  pDataOut->DataLenght = (uint8_t)( *( pData + displacement + 1 ) - *( pData + displacement ) );
  pDataOut->Data = pData + (2 * (*(pData + displacement) + nPage));
  if (0 == pDataOut->DataLenght)
  {
    return SF_LDR_WRN_LENGHT;
  }

  return SF_OK;

}



/**
This function extract the page related to a given displacement belonging to a setting file.
The start address of the Extrapointers area is provided as parameter.
This function performs a subset of functionality of the SFBasicLoader.

\param  [in]		pdataStartAddr: start address of the extrapointers area.
\param  [in] 	  displacement: index of the displscement. (see setting file documentation)
\param  [out] 	ui16Page: start address of the page related to the displacement (see setting file documentation)

\return 	uint8_t

\retval	 SF_OK : OK
\retval  SF_LDR_ERR_INVALID_DISPL: invalid displ value
\retval  SF_LDR_ERR_INVALID_POINTER: invalid pointer value
\retval  SF_LDR_ERR_OVFL_EXTRAPTR: too many extrapointers respect to a MAX defined value
\retval  SF_LDR_ERR_ENDOFPOINTER: displacement is > of the last available displacement
\retval  SF_LDR_WRN_LENGHT : data has length =0


\author 	Andrea.Mantovani
\date		Feb X, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
uint8_t SFBasicLoaderGetPage(uint16_t* pdataStartAddr, uint8_t displacement,uint16_t* ui16Page)
{
  uint16_t nPage;
  uint8_t* pData;
  uint8_t nExtraPointerWords;
  uint8_t i;


  if (0xFF == displacement)
  {
    /**
      0xFF  l'ultimo indice consentito per un displacement tuttavia, essendo dispari,
       anche l'ultimo displacement in assoluto quindi non  uno spiazzamento valido
      in quanto non  possibile specificare la sua lunghezza.
      */
    return SF_LDR_ERR_INVALID_DISPL;
  }

  pData = (uint8_t*)(pdataStartAddr);
  nExtraPointerWords = *pData;


  if (nExtraPointerWords > N_MAX_EXTRAPTR_WORDS)
  {
    return SF_LDR_ERR_OVFL_EXTRAPTR;
  }

/* Checks the ExtraPointer and calculates the page */
  nPage = 0;
  for (i = 1; i < (2 * nExtraPointerWords); i++)
  {
    /* displacement < 0xFF */
    if (displacement >= OFFS_B(pData, i))
    {
      nPage += 0x0100U;
    }
  }

 *ui16Page= nPage;
 return SF_OK;
}


/**
This function simply calculates the address and size of data related to the given displacement.
Also the displacement page must be rpovided as parameter.
This function performs a subset of functionality of the SFBasicLoader.

\param  [in]		ui16Displ: being a uint16 and each displacement is only one byte the MSByte represent the
                           Displ while LSByte the Displ+1
\param  [in] 	  ui16Page: Page related to the Displ (see setting file documentation)
\param  [out] 	pDataOut: is composed by the address and the size (in words) of the returned data.
                          the address isnot absolute but is the offset respect to the first displacement.
                          To get the absolute address you must add Eepadd+pointer+numextrapointers (see setting file documentation)

\return 	uint8_t

\retval	 SF_OK : OK
\retval  SF_LDR_ERR_INVALID_DISPL: invalid displ value
\retval  SF_LDR_ERR_INVALID_POINTER: invalid pointer value
\retval  SF_LDR_ERR_OVFL_EXTRAPTR: too many extrapointers respect to a MAX defined value
\retval  SF_LDR_ERR_ENDOFPOINTER: displacement is > of the last available displacement
\retval  SF_LDR_WRN_LENGHT : data has length =0


\author 	Andrea.Mantovani
\date		Feb X, 2010
\version	1.0.0

\todo
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
uint8_t SFBasicLoaderGetDisplOffset(uint16_t ui16Displ,uint16_t ui16Page,Loader_Struct* pDataOut)
{
 /* The MSByte of ui16Displ is the displ while the LSByte is the displ+1 that is used to calculate data length*/
 /* The Data length is in Word ! */
 /* pDataOut->Data is the diaplacement offset respect to the first displacement
    to get the absolute address you must add Eepadd+pointer+numextrapointers
 */
  pDataOut->DataLenght = (uint8_t)((uint8_t)(ui16Displ & 0x00FF) - (uint8_t)((ui16Displ & 0xFF00) >> 8));
  pDataOut->Data = (uint8_t*)(2 * (((ui16Displ&0xFF00)>>8) + ui16Page));
  return SF_OK;
}



/**
E' la funzione di base per il caricamento di dati dal file di setting: a partire
dai parametri indicati in ingresso (puntatore e spiazzamento) restituisce
un puntatore al record dei dati cercati e lunghezza in word dello stesso.

\param [in]		pointer e' il puntatore del file di setting da cui caricare i dati
\param [in]		displacement e' lo spiazzamento relativo al puntatore pointer
				da cui caricare i dati
\param [out]	pDataOut punta alla tabella dati individuata dai parametri
				specificati in input
\attention 		Eeprom (global, buffer area dedicata al file di setting) e'
				usata come input

\return			BSLDR_OK se la funzione e' stata eseguita correttamente, un codice
				d'errore altrimenti.

\warning		none

\todo			il nome della funzione e i campi del tipo Loader_Struct sono difformi dalle
				convenzioni adottate nel resto del modulo per garantire la compatibilita'
				all'indietro con il modulo I/O Manager che sfrutta la funzione BasicLoader
				e ne specifica il prototipo tramite extern.
				Anche il type Loader_Struct e' definito in SkeletonTypes.h, tuttavia sarebbe
				opportuno cambiare i nomi dei campi, in particolare correggere l'errore
				"DataLenght".

\version		1.0.0
\date 			04/01/2006
\author			Simone Costantini
*/
#if defined (IO_MANAGER_BASICLOADER_CSTM) 
#if ( IO_MANAGER_BASICLOADER_CSTM == FALSE )

uint8_t BasicLoader(settingPntr_t pointer, uint16_t displacement, Loader_Struct* pDataOut)
{
	uint8_t retVal = BSLDR_ERR_INVALID_POINTER;

	uint16_t* dataStartAddr = GetDefaultSettingStartAddress();
	dataStartAddr = LIB_SEF_GetBEWrdRelShort_Ptr(dataStartAddr, pointer);
	if (dataStartAddr != NULL)
	{
		retVal = LIB_SEF_GetDisplData(dataStartAddr, displacement, pDataOut);
	}

	return retVal;
}

/**
This routine give absolute address in a big endian 16-bit pointers table

\param  [in]    pDataStartAddr		Pointer of pointers table
\param  [in]	pointer				Pointer item to read


\return 	uint16_t *	- Absolute address of pointer desired
 	 	 	 NULL if starting address or pointer is invalid

\author 	Alessandro Sabbatini
\date		19/06/2014
\version	1.0.0


\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
uint16_t *LIB_SEF_GetBEWrdRelShort_Ptr(uint16_t *pDataStartAddr, settingPntr_t pointer)
{
	uint16_t *tmpAddr = NULL;

	if ((pointer != INVALID_POINTER) && (pDataStartAddr != NULL))
	{
		uint16_t pointerVal = EndianU16_BToN((pDataStartAddr[pointer]));

		if ((NULL_POINTER != pointerVal) && (INVALID_POINTER != pointerVal))
		{
			tmpAddr = pDataStartAddr + pointerVal;
		}

	}

	return tmpAddr;
}

uint16_t *LIB_SEF_GetMsettingStartAddr(uint8_t SFIndex)
{
	uint16_t *tmpAddr = GetDefaultSettingStartAddress();	/**< Pointer to mSetting */
	uint16_t pMapNul = EndianU16_BToN(*(tmpAddr+SF_PTR_MAP_LENGTH));
	uint16_t *pMappIndex;
	uint16_t pMapIdx;

	if ( SFIndex != 0 )
	{
		pMappIndex= LIB_SEF_GetBEWrdRelShort_Ptr(tmpAddr, SF_PTR_MAP_LENGTH);
		pMapIdx = EndianU16_BToN(*pMappIndex);

		if ( (INVALID_POINTER != pMapNul ) && (NULL_POINTER != pMapNul)
				&& (SFIndex < (pMapIdx - pMapNul)))
		{
			pMapIdx = EndianU16_BToN(*(pMappIndex+(SFIndex - 1)));
			tmpAddr += pMapIdx;
		}
		else
		{
			tmpAddr = NULL;
		}
	}

	return tmpAddr;

}

uint8_t LIB_SEF_GetMaxMsettings(void)
{
	uint16_t *tmpAddr = GetDefaultSettingStartAddress();	/**< Pointer to mSetting */
	uint16_t *pMappIndex= LIB_SEF_GetBEWrdRelShort_Ptr(tmpAddr, SF_PTR_MAP_LENGTH);
	uint16_t pMapNul = EndianU16_BToN(*pMappIndex);
	uint8_t retVal = 0;

	if ( (INVALID_POINTER != pMapNul) && (NULL_POINTER != pMapNul))
	{
		uint16_t pMapIdx = EndianU16_BToN(*(tmpAddr+SF_PTR_MAP_LENGTH));

		retVal = pMapNul - pMapIdx - 1;
	}

	return retVal;
}

bool_t LIB_SEF_IsExtdDisplPtr(uint8_t SFIndex, settingPntr_t pointer )
{
	bool_t retVal = FALSE;
#if defined(CONFIG_LIB_SETTING_EXTD_DISPL)
	uint16_t *tmpAddr = LIB_SEF_GetMsettingStartAddr(SFIndex);

	tmpAddr = LIB_SEF_GetBEWrdRelShort_Ptr(tmpAddr, pointer);

	if (tmpAddr != NULL)
	{
		retVal = (*((uint8_t *)tmpAddr) & 0x80) != 0;
	}
#endif
	return retVal;
}


uint8_t LIB_SEF_GetDisplData(uint16_t* dataStartAddr, uint16_t displacement, Loader_Struct* pDataOut)
{
#if defined(CONFIG_LIB_SETTING_EXTD_DISPL)
	bool_t isExtDispl = FALSE;
#endif
	uint16_t maxDispl = 0xFF;
	uint16_t nMaxExtraPointerWords = N_MAX_EXTRAPTR_WORDS;
	uint16_t nPage;
	uint16_t nExtraPointerWords;
	uint8_t i;

#if defined(CONFIG_LIB_SETTING_EXTD_DISPL)
	isExtDispl = (*((uint8_t *)dataStartAddr) & 0x80) != 0;

	if (isExtDispl)
	{
		/* Extended Displacement => 16 bit */
		maxDispl = 0xFFFF;
		nMaxExtraPointerWords = N_MAX_EXTRAPTR16_WORDS;
		nExtraPointerWords = (*dataStartAddr) & 0x7FFF;  /* Rimozione bit di formato a 16 bit (bit 15) */
	}
	else
#endif /* defined(CONFIG_LIB_SETTING_EXTD_DISPL) */
	{
		nExtraPointerWords =(uint16_t) *(uint8_t*)(dataStartAddr);
	}

#ifdef _BSLDR_ENABLE_ERROR_MNG_

	if (maxDispl == displacement)
	{
		/**
		0xFF / 0xFFFF e' l'ultimo indice consentito per un displacement tuttavia, essendo dispari,
	  e' anche l'ultimo displacement in assoluto quindi non e' uno spiazzamento valido
		in quanto non e' possibile specificare la sua lunghezza.
		*/
		return BSLDR_ERR_INVALID_DISPL;
	}

#endif

#ifdef _BSLDR_ENABLE_ERROR_MNG_

	if (nExtraPointerWords > nMaxExtraPointerWords)
	{
		return BSLDR_ERR_OVFL_EXTRAPTR;
	}
#endif

	/* Controlla gli ExtraPointer e imposta la pagina di conseguenza */
	nPage = 0;

#if defined(CONFIG_LIB_SETTING_EXTD_DISPL)
	if (isExtDispl)
	{
		  for ( i = 1; i < nExtraPointerWords; i++ )
		  {
		    if ( displacement >= EndianU16_BToN(dataStartAddr[i]) )
		    {
		      nPage += 0x100U;
		    }
		  }

		  /* dataStartAddr punta all'inizio dell'array dei displacement */
		  dataStartAddr += nExtraPointerWords;

		#ifdef _BSLDR_ENABLE_ERROR_MNG_

		  if ( (displacement + 2) > EndianU16_BToN(*dataStartAddr) )
		  {
		    return BSLDR_ERR_INVALID_DISPL;
		  }

		#endif

		  /*
		  Il calcolo e' valido anche nel caso in cui  *( EepData + Displacement + 1 ) < *( EepData + Displacement )
		  a causa di un'overflow page
		  */
		  pDataOut->DataLenght = EndianU16_BToN(dataStartAddr[displacement + 1]) - EndianU16_BToN(dataStartAddr[displacement]);
		  pDataOut->Data = (uint8_t *)&dataStartAddr[EndianU16_BToN(dataStartAddr[displacement]) + nPage];

	}
	else
#endif
	{
		uint8_t* pData = (uint8_t *)(dataStartAddr);

		for (i = 1; i < (2 * nExtraPointerWords); i++)
		{
			/* displacement < 0xFF */
			if (displacement >= OFFS_B(pData, i))
			{
				nPage += 0x0100U;
			}

		}

		pData += (2 * nExtraPointerWords);		/* pData points to the begin of displacement */

	#ifdef _BSLDR_ENABLE_ERROR_MNG_

		if (displacement >= ((2 * (*pData)) - 1))
		{
			return BSLDR_ERR_INVALID_DISPL;
		}

	#endif

		/*
		Il calcolo è valido anche nel caso in cui  *( EepData + Displacement + 1 ) < *( EepData + Displacement )
		a causa di un'overflow page
		*/
		pDataOut->DataLenght = (uint8_t)( *( pData + displacement + 1 ) - *( pData + displacement ) );
		pDataOut->Data = pData + (2 * (*(pData + displacement) + nPage));
	}

	if (0 == pDataOut->DataLenght)
    {
		return BSLDR_WRN_LENGHT;
    }

	return BSLDR_OK;
}


/*!

\brief Extended Basic Loader
\param[in,out]
\return
\date
\author
\version
\ingroup
\attention
\warning
*/
uint8_t MSettingBasicLoader( uint8_t SFIndex, settingPntr_t pointer, uint16_t displacement,  Loader_Struct* pDataOut)
{
	uint16_t *pmSetting = LIB_SEF_GetMsettingStartAddr(SFIndex);	/**< Pointer to mSetting */
	uint8_t errorSFCode = BSLDR_ERR_INVALID_POINTER;	/**< Error code for SFBasicLoader and SFmSettingPointerSearch */

	pmSetting = LIB_SEF_GetBEWrdRelShort_Ptr(pmSetting, pointer);

	if (pmSetting != NULL)
	{
		errorSFCode = LIB_SEF_GetDisplData(pmSetting, displacement, pDataOut);
	}

	return errorSFCode;
}


/**
This routine perform access to Calibration data (WBOX or not).

\param  [in]    caliBoxID 	Calibration box to be used (ID_CAL_PNTR_ELECT_SUPP, ID_CAL_PNTR_FACTORY)
\param  [out]	pDataOut	pointer and data length of the requested field


\return 	uint8_t

\retval		BSLDR_ERR_INVALID_DISPL
\retval		BSLDR_ERR_OVFL_EXTRAPTR
\retval		BSLDR_WRN_LENGHT
\retval		BSLDR_OK
		 	 		

\author 	Platform's Group
\date		02/09/2010
\version	1.0.0


\author 	Platform's Group
\date		13/09/2010
\version	1.0.1

\author 	Alessandro Sabbatini
\date		28/05/2013
\version	1.1.0
   
\note		\par \b Note 1:
			documentation by Stefano Ughi\n
			\par \b Note 2:
			13/09/2010
            pDataOut->Data = pData + (2 * (*(pData + displacement) + nPage)) + 3 + ((nWriting-1) * pDataOut->DataLenght);
            becames\n
            pDataOut->Data = pData + (2 * (*(pData + displacement) + nPage)) + 4 + ((nWriting-1) * pDataOut->DataLenght);
			\par \b Note 3:
			Update by Alessandro Sabbatini
			LIB_CAL_GetCalibData() is the new function to get calibration data

*/

uint8_t LIB_CAL_GetCalibData(uint8_t caliBoxID, Loader_Struct* pDataOut)
{
	uint8_t retVal = BSLDR_ERR_INVALID_POINTER;
	uint8_t tmpDspl = 0xff;
	uint16_t* pdataStartAddr;

#if defined(CONFIG_LIB_ADC_CALIBRATION)

#if defined(CONFIG_LIB_SETTING_CALIBRATION_ON_WBOX)
	pdataStartAddr = (uint16_t *) WBOXData;
#else
	pdataStartAddr = LIB_SEF_GetBEWrdRelShort_Ptr(GetDefaultSettingStartAddress(), CONFIG_LIB_SETTING_CALIB_PTR);
#endif

	switch (caliBoxID)
	{
	case ID_CAL_PNTR_ELECT_SUPP:
	{
		tmpDspl = CONFIG_LIB_SETTING_CALIB_EMS_DISPL;
	}
	break;

	case ID_CAL_PNTR_FACTORY:
	{
		tmpDspl = CONFIG_LIB_SETTING_CALIB_PLANT_DISPL;
	}
	break;

	default:

	break;
	}

	retVal = LIB_SEF_GetDisplData(pdataStartAddr, tmpDspl, pDataOut);
	/* retVal = ExtendedBasicLoader(pdataStartAddr, CONFIG_LIB_SETTING_CALIB_PTR, tmpDspl, pDataOut); */


#if defined(CONFIG_LIB_SETTING_CALIBRATION_ON_WBOX)

	if (retVal == BSLDR_OK)
	{
		unsigned int i;
	  	unsigned int nWriting = 8;
	  	uint8_t *pData = pDataOut->Data;

	  	/* Get unmber of writings done on Calibration area, according to bitmap */
	  	for (i = 0; i < 8; i++)
	  	{
	  		if( ( ( pData[0] >> i ) & 0x01 ) == 1 )
	  		{
	  			nWriting = i;
	  			break;
	  		}
	  	}

	  	/* Check if value found is not out of boundaries set by WBox structure */
	  	if( nWriting > pData[1] )
	  	{
	  		nWriting = pData[1];
	  	}

	    if(nWriting != 0)
	    {
	        pDataOut->DataLenght = pData[2];
	        pDataOut->Data = pData + 4 + ((nWriting-1) * pDataOut->DataLenght);
	    }
	    else
	    {
	        pDataOut->DataLenght = 0;
	        pDataOut->Data       = (uint8_t *) NULL;
	        retVal = BSLDR_WRN_LENGHT;
	    }

	}
#endif /* defined(CONFIG_LIB_SETTING_CALIBRATION_ON_WBOX) */

#endif /* defined(CONFIG_LIB_ADC_CALIBRATION) && defined(LIB_SETTING_CALIBRATION_ON_WBOX) */

	return retVal;
}

#endif /* #if defined (IO_MANAGER_BASICLOADER_CSTM == FALSE) */
#endif /* #if defined (IO_MANAGER_BASICLOADER_CSTM) */


/**
Computes the checksum as the sum of all \c len bytes starting form the pointer with initial seed.

\param [in]		len number of bytes (>=0)
\param [in]		data  pointer to data
\param [in]		initial seed value

\warning

\note

\version		1.0.0
\date 			15/04/2009
\author			Roberto Fioravanti
*/
uint16_t LIB_ChkComputation (uint16_t  len, uint8_t * pdata, uint8_t seed)
{
	uint8_t  *_pdata=pdata;
	uint16_t i;

	uint16_t chk;
	chk = seed;

	for(i=0; i<len; i++)
	{
		chk+=*_pdata;
		_pdata++;
	}

	return chk;
}
/**
This function extract address and size of data related to a given displacement 
of a given pointer belonging to a setting file.
The start address of the Pointer is provided as parameter.

\param  [in]		pPointerAddr: start address of the pointer area
\param  [in] 	  displacement: index of the displscement. (see setting file documentation)
\param  [out] 	pDataOut: is composed by the absolute address and the size (in words) of the returned data

\return 	uint8_t

\retval	 SF_OK : OK
\retval  SF_LDR_ERR_INVALID_DISPL: invalid displ value
\retval  SF_LDR_ERR_INVALID_POINTER: invalid pointer value
\retval  SF_LDR_ERR_OVFL_EXTRAPTR: too many extrapointers respect to a MAX defined value
\retval  SF_LDR_ERR_ENDOFPOINTER: displacement is > of the last available displacement
\retval  SF_LDR_WRN_LENGHT : data has length =0
 

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
uint8_t SFBasicLoaderPointer(uint8_t* pPointerAddr, uint8_t displacement, Loader_Struct* pDataOut) 
{
  uint16_t nPage, ui16DisplNum;
  uint8_t *pData2;
  
  uint8_t nExtraPointerWords;
  uint8_t i;

    
  if (0xFF == displacement)
  {
    /**
      0xFF is the last admitted value and is an odd value and for this reason it must be a dummy 
      displacement cause the size cannot be calculated
      */
    return SF_LDR_ERR_INVALID_DISPL;
  }
 
  
  if (((uint8_t*)NULL_POINTER == pPointerAddr) || ((uint8_t*)INVALID_POINTER == pPointerAddr))
  {
    return SF_LDR_ERR_INVALID_POINTER;
  }
 
  nExtraPointerWords = *pPointerAddr;
  
  if (nExtraPointerWords > N_MAX_EXTRAPTR_WORDS)
  {
    return SF_LDR_ERR_OVFL_EXTRAPTR;
  }

 /* Checks the ExtraPointer and calculates the page */
  nPage = 0;
  for (i = 1; i < (2 * nExtraPointerWords); i++)
  {
    /* displacement < 0xFF */
    if (displacement >= OFFS_B(pPointerAddr, i))
    {
      nPage += 0x0100U;
    }
  }

/* pData points to the first byte of the displacement area */
  pPointerAddr += (2 * nExtraPointerWords); 


   ui16DisplNum = (uint16_t)((2*(uint8_t)(*pPointerAddr))-1);
   
  /* pData2 points to the last dummy displacement */
   pData2 =pPointerAddr+ui16DisplNum;
   if (*pData2 == *(pData2-1)) 
   {
      ui16DisplNum -=1;
   }
   
    if (displacement >= ui16DisplNum)
    {
      return SF_LDR_ERR_ENDOFPOINTER;
    }
 
  /*
    Calculation is valid also in case *( EepData + Displacement + 1 ) < *( EepData + Displacement ) 
    cause an overflow page occurs.
  */

  pDataOut->DataLenght = (uint8_t)( *( pPointerAddr + displacement + 1 ) - *( pPointerAddr + displacement ) );
  pDataOut->Data = pPointerAddr + (2 * (*(pPointerAddr + displacement) + nPage));
  if (0 == pDataOut->DataLenght)
  {
    return SF_LDR_WRN_LENGHT;
  }

  return SF_OK;

}


/**
This function the number of displacements for a give setting file pointer passed by its memory pointer

\param  [in]		pPointerAddr: start address of the pointer area
\param  [out] 	    pDisplNum: number of displacement


\return 	uint8_t

\retval	 SF_OK : OK
\retval  SF_LDR_ERR_INVALID_POINTER: invalid pointer value (*pDisplNum not changed)
\retval  SF_LDR_ERR_OVFL_EXTRAPTR: too many extrapointers respect to a MAX defined value (*pDisplNum not changed)


\author 	Roberto Fioravanti
\date		21/10/2010
\version	1.0.0	
   
\note		\par \b Note 1:
			documentation by Roberto Fioravanti

*/
uint8_t SFGetDisplacementNumber(uint8_t* pPointerAddr, uint8_t *pDisplNum)
{
    uint16_t ui16DisplNum;
    uint8_t *pData2;

    uint8_t nExtraPointerWords;
  
    if (((uint8_t*)NULL_POINTER == pPointerAddr))
    {
        return SF_LDR_ERR_INVALID_POINTER;
    }

    nExtraPointerWords = *pPointerAddr;

    if (nExtraPointerWords > N_MAX_EXTRAPTR_WORDS)
    {
        return SF_LDR_ERR_OVFL_EXTRAPTR;
    }

    
    /* pData points to the first byte of the displacement area */
    pPointerAddr += (2 * nExtraPointerWords); 


    ui16DisplNum = (uint16_t)((2*(uint8_t)(*pPointerAddr)) - 1);

    /* pData2 points to the last dummy displacement */
    pData2 =pPointerAddr+ui16DisplNum;
    if (*pData2 == *(pData2-1)) 
    {
        ui16DisplNum -=1;
    }
    
    if (ui16DisplNum < 256) {
        *pDisplNum = (uint8_t) ui16DisplNum;  
    }
    else 
	{
		*pDisplNum = 255;
	}
    
    return SF_OK;
}

/**
This function gives Fault Priority code according to

\param  [in]		getApplTable: TRUE if Application Faults priority table to use
\param  [in] 	    fltCode: Fault code to search
\param  [in] 	    fltSubCode: Fault sub code to search
\param  [out] 	    fltPrio: Returns fault priority read from setting file (if all OK, unchanged otherwise)


\return 	uint8_t

\retval	 SF_OK : 	OK
\retval  SF_LDR_ERR_INVALID_DISPL: invalid displacement (not found)
\retval  SF_LDR_ERR_NOT_FOUND: Fault priority value not found


\author 	Alessandro Sabbatini
\date		11/02/2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
uint8_t LIB_SEF_GetFaultPriority(bool_t getApplTable, uint8_t fltCode, uint8_t fltSubCode, uint8_t *fltPrio)
{
#if defined(SF_DISPL_FAULT_APP_PRIO) && defined(SF_DISPL_FAULT_PLT_PRIO)
	uint8_t resCode = SF_LDR_ERR_NOT_FOUND;
	uint8_t reqDispl;
	uint8_t tmpResVal;
	Loader_Struct tmpLdrData;

	if (getApplTable)
	{
		reqDispl = SF_DISPL_FAULT_APP_PRIO;
	}
	else
	{
		reqDispl = SF_DISPL_FAULT_PLT_PRIO;
	}

	tmpResVal = BasicLoader(SF_PTR_FAULT, reqDispl, &tmpLdrData);

#if defined(LIB_SETTINGFILE_CUSTOM_FAULT_PRIORITY)
	if (tmpResVal != BSLDR_OK)
	{
		if (getApplTable)
		{
			tmpLdrData = CSTM_ApplFaultPriorities;
		}
		else
		{
			tmpLdrData = CSTM_PltFaultPriorities;
		}
		tmpResVal = BSLDR_OK;

	}
#endif

	if (tmpResVal == BSLDR_OK)
	{
		uint8_t numItems = ((tmpLdrData.DataLenght * 2) / 3);
		uint8_t i;

		for (i=0; i < numItems; i++)
		{
			uint8_t tmpFltCode = tmpLdrData.Data[2*i];
			uint8_t tmpFltSubCode = tmpLdrData.Data[(2*i) + 1];

			if ((tmpFltCode == fltCode) && ((tmpFltSubCode == fltSubCode) || (tmpFltSubCode == 0xFF)))
			{
				*fltPrio = tmpLdrData.Data[(2*numItems)+i];
				resCode = SF_OK;
				break;
			}
		}
	}

	return resCode;

#else
	return SF_LDR_ERR_NOT_FOUND;

#endif

}

/**
This function gives Fault Priority field from fault priority retrieved by setting file,
excluding Fault to display field.

\param  [in]		faultPrio: Full fault priority item get from setting file table

\return uint8_t		Fault priority without Fault to display bit

\author 	Alessandro Sabbatini
\date		18/02/2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
uint8_t LIB_SEF_FaultPriorityField(uint8_t faultPrio)
{
	return (uint8_t) (faultPrio & ((uint8_t) SF_FAULT_PRIO_MASK));
}


/**
This function gives Fault To Display flag from fault priority retrieved by setting file

\param  [in]		faultPrio: Full fault priority item get from setting file table

\return 	bool_t

\retval	 TRUE		if Fault to display flag is set
\retval  FALSE		if Fault to display flag is not set


\author 	Alessandro Sabbatini
\date		18/02/2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
bool_t LIB_SEF_IsFaultToDisplayON(uint8_t faultPrio)
{
	return (uint8_t) ((faultPrio & ((uint8_t) SF_FAULT_DISPL_MASK)) != 0);
}

/**
This function gives Fault To Display flag according to Fault code or sub code

\param  [in]		getApplTable: TRUE if Application Faults priority table to use
\param  [in] 	    fltCode: Fault code to search
\param  [in] 	    fltSubCode: Fault sub code to search


\return 	bool_t

\retval	 TRUE		if Fault to display flag is set
\retval  FALSE		if Fault to display flag is not set or fault is not present


\author 	Alessandro Sabbatini
\date		18/02/2013
\version	1.0.0

\note		\par \b Note 1:
			documentation by Alessandro Sabbatini

*/
bool_t LIB_SEF_GetFaultToDisplay(bool_t getApplTable, uint8_t fltCode, uint8_t fltSubCode)
{
	bool_t resVal = FALSE;
	uint8_t fltPrio = 0;

	if (LIB_SEF_GetFaultPriority(getApplTable, fltCode, fltSubCode, &fltPrio) == SF_OK)
	{
		resVal = LIB_SEF_IsFaultToDisplayON(fltPrio);
	}

	return resVal;
}

/* *********** */
/* End of file */
/* *********** */
