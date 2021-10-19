/**
\n
\n
\file		LIB_BackUp_low.c
\ingroup	LIB_BackUp
\date
\version
\author
\n
\n
\par\b	History:
\n
\n
\n
*/
#define MODULE_BODY_LIB_BACKUP
#include "IO_driver.h"
#include "OCFLS_drv.h"
#include "LIB_BackUp.h"
#include "LIB_ClassBVar.h"
#include "LIB_Error.h"


#define MAX_IDX_BACKUP_TAB	2 /**< numero di tabelle di cui fare il backup */
#define CLEAR_TEMPORARY_BUFFER(d,v,s) memset(d, v, s)


static bool_t LIB_BackupBlockSave(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position);

uint16_t LIB_Backup_GetBackupLen(void)
{
	uint16_t i=0;
	uint16_t dataLen = 0;
	while(PltBackupAddrTab[i] != ((void *) BCK_ADDR_END_TAB )){
		dataLen += PltBackupDimTab[i++];
	}
	i=0;
	while(AppBackupAddrTab[i] != ((void *) BCK_ADDR_END_TAB )){
		dataLen += AppBackupDimTab[i++];
	}
	return dataLen;
}

/**
Writes in flash a block of backup data

\param  [in]	pSource pointer to source data
\param  [in]	size size in bytes of block
\param  [in]	position start addr in flash where to store data


\return 	bool_t

\retval		TRUE success
\retval		FALSE fail


\author 	stefano.ughi
\date		Feb 07, 2011
\version	1.0.0


\note		\par \b Note 1:
			documentation by stefano.ughi

*/

static bool_t LIB_BackupBlockSave(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
	bool_t neverEndLoop;
	bool_t result;
	IO_ErrorType err;

	result = FALSE;
	neverEndLoop = TRUE;

	while( neverEndLoop )
	{
		err = OCFLS_WriteAndVerifyAsync(pSource, size, position);

		switch (err)
		{
		case IO_E_OK:

			neverEndLoop = FALSE;
			result = TRUE;

			break;

		case IO_E_UNEXPECTED_CONDITION:

			neverEndLoop = FALSE;
			result = FALSE;

			break;

		default:
			break;
		}
	}
	return (result);
}


/**
Performs backup area clear.
Needs that FLS driver is inizialized.

\return 	bool_t

\retval		TRUE clear done
\retval		FALSE clear failed

\author 	stefano.ughi
\date		Feb 02, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
bool_t LIB_Backup_InvalidateBackupArea(void)
{
	IO_ErrorType err;
	bool_t endLessLoop;
	bool_t clear_OK;

	clear_OK = FALSE;
	endLessLoop = TRUE;
	while (endLessLoop)
	{
		err = OCFLS_BlankCheckEraseVerifyAsync(BACKUPAREA_SIZE_IN_BYTES, BACKUPAREA_START_ADDR);
		switch (err)
		{
		case IO_E_OK:
			endLessLoop = FALSE;
			clear_OK = TRUE;
			break;
		case IO_E_UNEXPECTED_CONDITION:
			endLessLoop = FALSE;
			break;
		default:
			/* running */
			break;
		}
	}
	return clear_OK;
}

/**
\n Saves in flash backup data.
\n Receives as inputs:
\n\li BackupAddrTab[]   addresses of locations that must be saved.
\n\li BackupDimTab[]    sizes of locations.

\return 	uint8_t

\retval		BACKUP_E_TOO_MANY_ELEMENTS too many elements inside BackupDimTab
\retval		BACKUP_E_TOO_MANY_BYTES too many backup bytes
\retval		BACKUP_E_BLOCK_WRITE error on write/verify block operation
\retval		BACKUP_E_OK Success


\author 	stefano.ughi
\date		Feb 08, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi
            \par \b Note 2:
            each backup location must have a size less than 256 bytes
*/

uint8_t LIB_Backup_DataSave (void)
{
	uint8_t temporaryWriteBuffer[BLOCK_FLASH_SIZE];
	/* Punto al primo indirizzo dei dati da salvare */
	uint8_t **ppBackupAddrTab = (uint8_t**)&PltBackupAddrTab[0];
	/* Punto al primo elemento dei dati da salvare */
	uint8_t *ppBackupDimTab = (uint8_t*)&PltBackupDimTab[0];
	uint8_t dataLen;
	
	/* Punto al primo elemento del buffer temporaneo che utilizzo per appoggiare i dati da backuppare */
	uint8_t *pTemporaryWriteBuffer = &temporaryWriteBuffer[0];
	/* Numero di bytes processati per il backup */
	uint16_t indexTemporaryWriteBuffer;
	/* Checksum */
	backupChecksum_t backupDataChecksum;
	/* Indice elemento BackupDimTab processato */
	indexBackupDimTab_t indexBackupDimTab;
	uint8_t i;
	uint8_t byteToSave;
	/* Contatore numero blocchi di dim. BLOCK_FLASH_SIZE scritti */
	uint8_t numBlock;
	uint8_t idxBackupTab = 0;

	PLT_ASSERT( LIB_Backup_GetBackupLen() + 2*(sizeof(backupChecksum_t)/sizeof(uint8_t)) < BACKUPAREA_SIZE_IN_BYTES )
	/*MS PLT_ASSERT( sizeTab(PltBackupDimTab) < (1<<(8*sizeof(indexBackupDimTab_t)))) */
	
	/*lint +e506 +e774*/
	CLEAR_TEMPORARY_BUFFER(pTemporaryWriteBuffer, 0xFF, BLOCK_FLASH_SIZE);

	/* Init. backup data checksum */
	backupDataChecksum = 0;
	
	/* Init. index BackupDimTab[] */
	indexBackupDimTab = 0;

	/* Inizializzazione indice vettore temporaryWriteBuffer[] */
	indexTemporaryWriteBuffer = 0;
	/* Inizializzazione numero blocchi da scrivere */
	numBlock=0;
	
	/* Per tutte le locazioni da salvare in backup */
	while ( idxBackupTab < MAX_IDX_BACKUP_TAB )
	{
		dataLen = ppBackupDimTab[indexBackupDimTab];
		/* Fissata una locazione da salvare in backup considero
		   tutti i bytes di cui e' costituita quella locazione */
		for ( i = 0; i < dataLen; i++ )
		{
			/* Prelevo il singolo byte */
			byteToSave = *(*ppBackupAddrTab+i);
			/* Riempio il vettore temporaryWriteBuffer con il dato da scrivere */
			*(pTemporaryWriteBuffer + (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE)) = byteToSave;
			/* Aggiorno cheksum	*/
			backupDataChecksum += byteToSave;
			/* Aggiorno indice buffer temporaneo */
			indexTemporaryWriteBuffer ++;

			if ( 0 == (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE))
			{
				/* Il vettore temporaneo e' pieno -> scrivo */
				numBlock++;

				if ( LIB_BackupBlockSave((FLS_SourcePtrType)pTemporaryWriteBuffer, BLOCK_FLASH_SIZE, (BACKUPAREA_START_ADDR+BLOCK_FLASH_SIZE*(numBlock-1))) == FALSE )
				{
					return(BACKUP_E_BLOCK_WRITE);
				}
				CLEAR_TEMPORARY_BUFFER(pTemporaryWriteBuffer,0xFF,BLOCK_FLASH_SIZE);
			}
		}

		/* Passo all'indirizzo successivo dei dati da salvare */
		ppBackupAddrTab++;
		/* Passo alla size successiva */
		indexBackupDimTab++;

		/**
		 * se è finita la tabella di plt passo alla tabella di applicazione
		 * la tabella è finita quando ha addr == 0
		 */
		if( *ppBackupAddrTab == ((void *)BCK_ADDR_END_TAB )){ /* condizione per passare alla tabella successiva */
			idxBackupTab++;
			indexBackupDimTab = 0;
			ppBackupAddrTab = (uint8_t**)&AppBackupAddrTab[0];
			ppBackupDimTab  = (uint8_t*)&AppBackupDimTab[0];
		}
	}

	/* Sono state processate tutte le locazioni da salvare in flash
	   probabilmente l'ultimo blocco non e' stato salvato  */

	/* Processo Checksum */
	for ( i = 0; i < sizeof(backupChecksum_t)/sizeof(uint8_t); i++ )
	{
		*(pTemporaryWriteBuffer + (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE)) = *((uint8_t*)&backupDataChecksum+i);
		indexTemporaryWriteBuffer ++;
		if ( 0 == (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE) )
		{
			numBlock++;

			if ( LIB_BackupBlockSave((FLS_SourcePtrType)pTemporaryWriteBuffer, BLOCK_FLASH_SIZE, (BACKUPAREA_START_ADDR+BLOCK_FLASH_SIZE*(numBlock-1))) == FALSE )
			{
				return(BACKUP_E_BLOCK_WRITE);
			}
			CLEAR_TEMPORARY_BUFFER(pTemporaryWriteBuffer,0xFF,BLOCK_FLASH_SIZE);
		}
	}
	/* Write CheckSum Compl. */
	backupDataChecksum = (backupChecksum_t) (~(uint32_t)backupDataChecksum);
	for ( i = 0; i<sizeof(backupChecksum_t)/sizeof(uint8_t); i++ )
	{
		*(pTemporaryWriteBuffer + (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE)) = *((uint8_t*)&backupDataChecksum+i);
		indexTemporaryWriteBuffer ++;
		if ( 0 == (indexTemporaryWriteBuffer%BLOCK_FLASH_SIZE) )
		{
			numBlock++;

			if ( LIB_BackupBlockSave((FLS_SourcePtrType)pTemporaryWriteBuffer, BLOCK_FLASH_SIZE, (BACKUPAREA_START_ADDR+BLOCK_FLASH_SIZE*(numBlock-1))) == FALSE )
			{
				return(BACKUP_E_BLOCK_WRITE);
			}
			CLEAR_TEMPORARY_BUFFER(pTemporaryWriteBuffer,0xFF,BLOCK_FLASH_SIZE);
		}
	}

	if ( (indexTemporaryWriteBuffer % BLOCK_FLASH_SIZE) != 0 )
	{
		numBlock++;
		/* Avrei dovuto scrivere indexTemporaryWriteBuffer%BLOCK_FLASH_SIZE) bytes */
		if ( LIB_BackupBlockSave((FLS_SourcePtrType)pTemporaryWriteBuffer,(indexTemporaryWriteBuffer%BLOCK_FLASH_SIZE), (BACKUPAREA_START_ADDR+BLOCK_FLASH_SIZE*(numBlock-1))) == FALSE )
		{
			return(BACKUP_E_BLOCK_WRITE);
		}
	}

	return(BACKUP_E_OK);
}


/**
Restores backup data.
\n\li Case 1: keyword found inside the first byte of backup data, the routines restores only
\n data from NULL pointer to the end. The NULL pointer is called marker.
\n\li Case 2: keyword not found inside the first byte of backup data, the routines restores
\n all backup data.
\n The routine receives as inputs:
\n\li BackupAddrTab[]   addresses of location to restore.
\n\li BackupDimTab[]    sizes of location.


\author 	stefano.ughi
\date		Feb 01, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void LIB_Backup_DataRestore(void)
{
	indexBackupDimTab_t i = 0;
	uint8_t j;
	uint8_t idxBackupTab = 0;
	/* Default: all data must be restored */
	bool_t partialRestore = FALSE;
	bool_t markerFound    = FALSE;
	bool_t skipNextItemToo = FALSE;
	bool_t skipItem = FALSE;
	uint8_t *pBackupArea = (uint8_t *)BACKUPAREA_START_ADDR;

	uint8_t** ppBackupAddrTab = (uint8_t**)&PltBackupAddrTab[0];
	/* Punto al primo elemento dei dati da salvare */
	uint8_t *ppBackupDimTab  = (uint8_t*)&PltBackupDimTab[0];

	/* Is present Keyword for partial restore ? */
	if ( *pBackupArea == PARTIAL_BACKUP_KEYWORD )
	{
		/* Restores the keyword that will be tested from application */
		Backup_Source = PARTIAL_BACKUP_KEYWORD;
		/* Partial restore enabled */
		partialRestore = TRUE;
	}

	while ( idxBackupTab < MAX_IDX_BACKUP_TAB )
	{

#if defined(CONFIG_LIB_BACKUP_DATACARE_SAVE)
		if (idxBackupTab == 0)
		{
			for (j = 0; (skipItem == FALSE) && (PltBackupSafetyIdx[j] != 0); j++)
			{
				if ( (i == PltBackupSafetyIdx[j]) && (PltBackupDimTab[i+1] == 2))
				{
					uint16_t chkVal = *((uint16_t *)(pBackupArea + PltBackupDimTab[i]));

					if (SAFETY_DATA_CHECKONLY(pBackupArea, PltBackupDimTab[i], chkVal))
					{
						skipNextItemToo = FALSE;
						skipItem = FALSE;
					}
					else
					{
						skipNextItemToo = TRUE;
						skipItem = TRUE;
					}
				}
			}
		}
		else
		{
			skipNextItemToo = FALSE;
			skipItem = FALSE;
		}
#else
		skipNextItemToo = FALSE;
#endif

		for (j = 0; j < ppBackupDimTab[i]; j++)
		{
			if ( ((FALSE == partialRestore) || (TRUE == markerFound)) && (FALSE == skipItem))
			{
				*(*ppBackupAddrTab + j) = *pBackupArea;
			}
			pBackupArea++;
		}

		if (*ppBackupAddrTab == PC_CTRL_NOT_RESTORED_END)
		{
			/* From here restore data if keyword found */
			markerFound = TRUE;
		}

		if (!skipNextItemToo)
		{
			skipItem = FALSE;
		}

		ppBackupAddrTab++;
		i++;

		if( *ppBackupAddrTab == ((void *) BCK_ADDR_END_TAB)){
			ppBackupAddrTab = (uint8_t**)&AppBackupAddrTab[0];
			ppBackupDimTab  = (uint8_t*)&AppBackupDimTab[0];
			skipNextItemToo = FALSE;
			skipItem = FALSE;
			markerFound = FALSE;
			i=0;
			idxBackupTab++;
		}
	}
}


/**
Verifies backup data checksum.

\return 	bool_t

\retval		TRUE checksum o.k.
\retval		FALSE checksum k.o.

\author 	stefano.ughi
\date		Feb 02, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by stefano.ughi

*/

bool_t LIB_Backup_ChecksumVerify(void)
{
	uint8_t *pBackupArea;
	uint16_t lenBackupTab;
	backupChecksum_t backupChk;
	backupChecksum_t not_backupChk;
	bool_t retVal=FALSE;

	backupChk = 0;
	lenBackupTab  = LIB_Backup_GetBackupLen();
	pBackupArea = (uint8_t *)BACKUPAREA_START_ADDR;

	while ( lenBackupTab != 0 )
	{
		backupChk += *pBackupArea++;
		lenBackupTab--;
	}

	not_backupChk = (backupChecksum_t) (~(uint32_t)backupChk);

	if ( (backupChk == *(backupChecksum_t*)pBackupArea) &&
	        (not_backupChk == *(backupChecksum_t*)(pBackupArea+sizeof(backupChecksum_t))) )
	{
		retVal = TRUE;
	}

	return retVal;
}
