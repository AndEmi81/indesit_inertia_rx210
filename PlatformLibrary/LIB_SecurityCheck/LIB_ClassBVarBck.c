/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par	SecurityCheck
\n		BackUp ClassB Variables
\n
\file		LIB_ClassBVarBck.c
\ingroup    grpDataCare
\author		 Marco Sperandini
*/

/* ******** */
/* Includes */
/* ******** */
#include <string.h>
#include "Config.h"
#include "LIB_ClassBVarBck.h"

#if defined(CONFIG_LIB_CLASS_B_USE_OCCRC)
#include "OCCRC_drv.h"
#define LIB_ClassBVar_CRC_calculate OCCRC_Calculate
#else
#include "SWCRC_drv.h"
#define LIB_ClassBVar_CRC_calculate SWCRC_Calculate
#endif

#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)

#define CLEAR_TEMPORARY_BUFFER(d,v,s) memset(d, v, s)

uint16_t Sec_getBackUpDataLen(void){
	int varCnt;
	uint16_t size=0;
	for(varCnt=0;varCnt<SEC_NUM_VAR_BCK; varCnt++){
		size += SecBackUpTable[varCnt].dataLen;
	}
	return size;
}
uint8_t Sec_BackupBlockSave(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
	bool_t neverEndLoop= TRUE;
	IO_ErrorType err;

#if 0
	while( neverEndLoop )
	{
		err = OCFLS_WriteAndVerifyAsync(pSource, size, position);
		if(err == IO_E_OK){
			neverEndLoop = FALSE;
		}else if(err == IO_E_UNEXPECTED_CONDITION){
			neverEndLoop = FALSE;
		}
	}
#else
		err=OCFLS_RWriteSync(pSource,size,position);
		{
			int i;
			for(i=0; i<size; i++){
				if( *(pSource+i) != *(uint8_t*)(position+i))
					err=IO_E_UNEXPECTED_CONDITION;
			}
		}
		/* err = OCFLS_RWriteVerify(pSource,size,position); */
#endif

	return (err);
}

/*
 * return 0 for OK
 * else error
 */

uint8_t Sec_Backup_SecDataSave (void)
{
	 uint8_t  wBuf[FLASH_WRITE_SIZE];
	 uint16_t numSecVars = SEC_NUM_VAR_BCK;
	 uint8_t* pSrc;
	 uint8_t* pDest;
	 uint16_t varCnt=0;
	 uint16_t varSaveLen=0;

	 uint8_t* pFlash=(uint8_t*)SEC_BCK_FLASH_BASE;

	 bool_t   noMoreVars = FALSE;
	 bool_t   endSave    = FALSE;
	 uint8_t  retVal     = 0;
	 uint16_t dataCrc;

	pSrc  = SecBackUpTable[varCnt].dataPtr;
	pDest = &wBuf[0];

	/* check sulla dimensione riservata al backup */
	PLT_ASSERT(((Sec_getBackUpDataLen()+2) <= SEC_BCK_FLASH_SIZE))

	CLEAR_TEMPORARY_BUFFER(wBuf,0xFF,FLASH_WRITE_SIZE);

	while(!endSave)
	{
		if( pDest < (wBuf+FLASH_WRITE_SIZE) && !noMoreVars)
		{ /* c'e' spazio nel buffer */

			if(varSaveLen >= SecBackUpTable[varCnt].dataLen )
			{ /* ha finito di salvare una var */
				varCnt++;

				if(varCnt < numSecVars){ /* ci sono ancora variabili da salvare*/
					varSaveLen=0;
					pSrc = SecBackUpTable[varCnt].dataPtr;
				}else{ /* sono finite  le var da salvare */
					noMoreVars= 1;
					/* calcola il crc di tutta l'area in flash */
					dataCrc = LIB_crc_calculate( pFlash -(uint8_t*)SEC_BCK_FLASH_BASE, (uint8_t*)SEC_BCK_FLASH_BASE);
					dataCrc = LIB_crc_partial_calculate((pDest-&wBuf[0]), &wBuf[0], dataCrc);
					/* scrive il crc nella flash in big endian*/
					if( (pDest+1) < (wBuf+FLASH_WRITE_SIZE))
					{	/* nel buffer c'è' spazion per i 2 byte del crc */
						*pDest++ = (dataCrc >> 8) & 0xFF;
						*pDest++ = dataCrc & 0xFF;
						retVal |= Sec_BackupBlockSave((FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
						/* pDest = &wBuf[0]; non serve*/
						/* pFlash += FLASH_WRITE_SIZE; non serve*/
					}else
					if( (pDest) < (wBuf+FLASH_WRITE_SIZE))
					{ /* nel buffer c'e' spazio solo per un byte */
						*pDest++ = (dataCrc >> 8) & 0xFF;
						retVal |= Sec_BackupBlockSave((FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
						pDest = &wBuf[0];
						pFlash += FLASH_WRITE_SIZE;
						*pDest++ = dataCrc & 0xFF;
						retVal |= Sec_BackupBlockSave((FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
						/* pDest = &wBuf[0]; non serve*/
						/* pFlash += FLASH_WRITE_SIZE; non serve*/
					}else{ /* nel buffer non c'è spazio per il crc */
						retVal |= Sec_BackupBlockSave((FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
						pDest = &wBuf[0];
						pFlash += FLASH_WRITE_SIZE;
						*pDest++ = (dataCrc >> 8) & 0xFF;
						*pDest++ = dataCrc & 0xFF;
						retVal |= Sec_BackupBlockSave((FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
						/* pDest = &wBuf[0]; non serve*/
						/* pFlash += FLASH_WRITE_SIZE; non serve*/
					}
					endSave=TRUE;
				}
			}
			if( !noMoreVars){
				*pDest++ = *pSrc++;
				varSaveLen++;
			}
		}else{ /* il buffer è pieno */
			retVal |= Sec_BackupBlockSave( (FLS_SourcePtrType)wBuf, FLASH_WRITE_SIZE, pFlash);
			pDest = &wBuf[0];
			pFlash += FLASH_WRITE_SIZE;
			CLEAR_TEMPORARY_BUFFER(wBuf,0xFF,FLASH_WRITE_SIZE);
		}
		
		if(retVal != 0)
			endSave=TRUE;		
	}
	return retVal;
}
/*
 * return 0 for OK
 * else error
 */

uint8_t Sec_Backup_SecDataRestore (void)
{
		uint16_t numSecVars = SEC_NUM_VAR_BCK;
		uint8_t* pSrc;
		uint8_t* pDest;
		uint16_t varCnt=0;
		uint16_t varRestoreLen=0;

		uint8_t* pFlash=(uint8_t*)SEC_BCK_FLASH_BASE;

		bool_t   noMoreVars = FALSE;
		uint8_t  retVal=0;
		uint16_t dataSize;
		uint16_t flashCrc, dataCrc;

		pSrc  = pFlash;
		pDest = SecBackUpTable[varCnt].dataPtr;
		/* verifica del crc in flash */
		dataSize = Sec_getBackUpDataLen();
		dataCrc = LIB_crc_calculate(dataSize, (uint8_t*)SEC_BCK_FLASH_BASE);
		/* flashCrc = (*((uint8_t*)SEC_BCK_FLASH_BASE+dataSize))<<8 +(*((uint8_t*)SEC_BCK_FLASH_BASE+dataSize+1)); */
		flashCrc = (*((uint8_t*)SEC_BCK_FLASH_BASE+dataSize));
		flashCrc <<=8;
		flashCrc |= (*((uint8_t*)SEC_BCK_FLASH_BASE+dataSize+1));
		if(dataCrc == flashCrc)
		{
			while( !noMoreVars )
			{
				if(varRestoreLen >= SecBackUpTable[varCnt].dataLen )
				{
					/* update CRC */
					Sec_UpdateExtVar(SecBackUpTable[varCnt].id, SecBackUpTable[varCnt].dataPtr, SecBackUpTable[varCnt].dataLen);
					/* check for the next var to restore */
					varCnt++;
					if(varCnt < numSecVars){ /* sono finite  le var da restoraare */
						varRestoreLen=0;
						pDest = SecBackUpTable[varCnt].dataPtr;
					}else{
						noMoreVars= 1;
					}
				}
				if( !noMoreVars){
					*pDest++ = *pSrc++;
					varRestoreLen++;
				}
			}
		}else{
			retVal=1;
		}
		return retVal;
}

uint8_t Sec_Backup_SecDataInvalidate (void)
{
	uint8_t err;
	bool_t neverEndLoop;
	while( neverEndLoop )
	{
		err = OCFLS_REraseVerify(SEC_BCK_FLASH_SIZE,(uint8_t*)SEC_BCK_FLASH_BASE);
		if(err == IO_E_OK){
			neverEndLoop = FALSE;
		}else if(err == IO_E_UNEXPECTED_CONDITION){
			neverEndLoop = FALSE;
		}
	}
	return err;
}
#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)*/

/* *********** */
/* End of file */
/* *********** */
