/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		Descrizione
\n		Platform utilities.
\n
\file		LIB_Utilities.h
\ingroup	PM_Library
\date		12/05/2009
\version	01.00.00
\author	Roberto Fioravanti
\n
\n
\par		History (titolo)
\n 		00.00.75
\n		28/10/2010
\n		Roberto Fioravanti
\n		changes
\n 		- addition of SFGetDisplacementNumber
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
\n
\n 		00.00.--
\n		--/--/2009
\n		Roberto Fioravanti
\n		changes
\n		- removed LIB_Delay_us
\n
\n 		00.00.05
\n		26/06/2009
\n		Roberto Fioravanti
\n		changes
\n    	creation:
\n 		- addiction of LIB_Delay_us
\n    	- addiction of LIB_MemcpyFromLast
\n
\n etc.
*/



/* Old Versions
\version
\date
\author
*/

/**

@{*/



#ifndef __LIB_UTILITIES_H
#define __LIB_UTILITIES_H


/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG

/* ****************** */
/* Defines and Macros */
/* ****************** */
/**
This is the maximum number of bytes on which the checksum is calculated \n
each time the function LIB_ComputeSettingFileCheckSum is called. \n
*/
#define SF_NUM_BYTES_TO_CHECK	64


/*
questa define  in CstmFuncLib.h

Dato un puntatore a uint8_t puint8_t, restituisce il valore dell'uint8_t
puntato da (puint8_t + i)
*/
#define OFFS_B(puint8_t, i)				((uint8_t)(*((uint8_t*)(puint8_t) + (i))))


/*
questa define  in CstmFuncLib.h

Dato un puntatore a uint8_t puint16_t, restituisce il valore dell'uint16_t
puntato da (puint16_t + i)
*/
#define OFFS_W(puint16_t, i)      ((uint16_t)(*((uint16_t*)(puint16_t) + (i))))

/*
dato un pauntatore a uint8_t restituisce l'uint16_t puntato in caso che il dato sia salvato come Big Endian
e cio con la parte alta HI_W all'indirizzo pi basso e la LO_W a quello pialto
*/
#define GETW_BE(puint8_t)          (uint16_t) ( (((uint16_t) (*((uint8_t*)(puint8_t))) << 8) & 0xFF00) + ((uint16_t) (*((uint8_t*)(puint8_t+1))) & 0x00FF))

/*
dato un pauntatore a uint8_t restituisce l'uint16_t puntato in caso che il dato sia salvato come Little Endian
e cio con la parte alta HI_W all'indirizzo pi alto e la LO_W a quello pi basso
*/
#define GETW_LE(puint8_t)          (uint16_t) ( (((uint16_t) (*((uint8_t*)(puint8_t+1))) << 8) & 0xFF00) + ((uint16_t) (*((uint8_t*)(puint8_t))) & 0x00FF))

/*
dato un pauntatore a uint8_t restituisce l'uint16_t puntato in caso che il dato sia salvato come Big Endian
e cio con la parte alta HI_W all'indirizzo pi basso e la LO_W a quello pialto
*/
#define GETDW_BE(puint8_t)         (uint32_t) (   (((uint32_t) (*((uint8_t*)(puint8_t)))   << 24) & 0xFF000000) + (((uint32_t) (*((uint8_t*)(puint8_t+1))) << 16) & 0x00FF0000) + (((uint32_t) (*((uint8_t*)(puint8_t+2))) <<  8) & 0x0000FF00) + (((uint32_t) (*((uint8_t*)(puint8_t+3))    )) & 0x000000FF) )
/*
dato un pauntatore a uint8_t restituisce l'uint16_t puntato in caso che il dato sia salvato come Little Endian
e cio con la parte alta HI_W all'indirizzo pi alto e la LO_W a quello pi basso
*/
#define GETDW_LE(puint8_t)         (uint32_t) (   (((uint32_t) (*((uint8_t*)(puint8_t+3))) << 24) & 0xFF000000) + (((uint32_t) (*((uint8_t*)(puint8_t+2))) << 16) & 0x00FF0000) + (((uint32_t) (*((uint8_t*)(puint8_t+1))) <<  8) & 0x0000FF00) + (((uint32_t) (*((uint8_t*)(puint8_t))      )) & 0x000000FF) )


/*
Valori particolari assunti dai puntatori nel file di setting.
Queste define stanno nell'appliance in XX_Loader.h
Bisogna ripensare come organizzare queste info.
*/
/**/
#define N_MAX_EXTRAPTR_WORDS	127	/**< Numero massimo di extra-pointer in un'area dati del file di setting */
#define N_MAX_EXTRAPTR16_WORDS  0x7FFE  /**< Numero massimo di extra-pointer in un'area dati */
#define NULL_POINTER				0x0000U	/**< Puntatore nullo */
#define INVALID_POINTER			0xFFFFU	/**< Puntatori non valido */


/** MAX E MIN */
#define MAX(a,b)    					(((a) > (b)) ? (a) : (b))
#define MIN(a,b)						(((a) < (b)) ? (a) : (b))
/**/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** Delay in us */
typedef uint16_t LIB_Delay_usType;
/** Returned type of LIB_ComputeSettingFileCheckSum() */
typedef struct LIB_CompChkTypeTag
{
	/** if TRUE checksum computation done */
	bool_t jobDone;
	/** this the checksum value */
	uint16_t checkSum;
}LIB_CompChkType;

#if (SF_NUM_BYTES_TO_CHECK<256)
typedef uint8_t LIB_NumBytesToCheckType;
#else
typedef uint16_t LIB_NumBytesToCheckType;
#endif

/** Setting File error type */
typedef enum
{
  SF_OK=0,
  /** Displacement has invalid value */
  SF_LDR_ERR_INVALID_DISPL,
  /** Pointer has invalid value */
  SF_LDR_ERR_INVALID_POINTER,
  /** Too many estrapointers respect to a MAX number declared */
  SF_LDR_ERR_OVFL_EXTRAPTR,
  SF_LDR_ERR_NODATA_ENOUGH,
   /** Displacement is grater than last available displ for the given pointer */
  SF_LDR_ERR_ENDOFPOINTER,
  SF_LDR_ERR_NOT_FOUND,
  /** Data pointed has length=0*/
  SF_LDR_WRN_LENGHT,
  /** no mappIndex */
  SF_LDR_ERR_NO_MAPPINDEX
} SFErrorType;
/* Application doesn't use it's BasicLoader */
#if defined (IO_MANAGER_BASICLOADER_CSTM)
#if( IO_MANAGER_BASICLOADER_CSTM == FALSE )
#define _BSLDR_ENABLE_ERROR_MNG_ /* if defined the routine check for errors */
#ifndef settingPntr_t

typedef uint16_t settingPntr_t;
#endif
#ifdef _BSLDR_ENABLE_ERROR_MNG_
#define BSLDR_ERR_INVALID_DISPL		0x82
#define BSLDR_ERR_INVALID_POINTER	0x89
#define BSLDR_ERR_OVFL_EXTRAPTR		0x88
#endif /* #ifdef _BSLDR_ENABLE_ERROR_MNG_ */
#define BSLDR_OK					0x00
#define BSLDR_WRN_LENGHT			0x02
#endif /* #if defined (IO_MANAGER_BASICLOADER_CSTM) */
#endif /* #if ( IO_MANAGER_BASICLOADER_CSTM == FALSE ) */

#define SF_FAULT_PRIO_MASK			0x7F
#define SF_FAULT_DISPL_MASK			0x80

/* ID for LIB_CAL_GetCalibData routine */
#define ID_CAL_PNTR_ELECT_SUPP	0
#define ID_CAL_PNTR_FACTORY		1


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint16_t Endian16_Swap(uint16_t value);

void LIB_MemcpyFromLast
(
  void *pDst, 		/* Destination buffer */
  const void *pSrc, /* Source buffer */
  uint8_t n   		/* Byte count */
);

#if defined(HAL_FLS_SUPPORT)
uint16_t LIB_SettingFileChecksumCalculate(uint16_t len, uint8_t *pSource, uint16_t partialChecksum);

LIB_CompChkType LIB_ComputeSettingFileCheckSum(FLS_positionType Start,FLS_positionType End);
#endif /* defined(HAL_FLS_SUPPORT) */


uint16_t LIB_ConvTwoByteInWord(const uint8_t *const pBuf);
uint16_t LIB_SwapWord(uint16_t data);


/** Functions used for Setting File manipulation */
uint8_t SFBasicLoader(uint16_t* pdataStartAddr, uint8_t pointer, uint8_t displacement, Loader_Struct* pDataOut);

uint8_t SFBasicLoaderGetPage(uint16_t* pdataStartAddr, uint8_t displacement,uint16_t* ui16Page);

uint8_t SFBasicLoaderGetDisplOffset(uint16_t ui16Displ,uint16_t ui16Page,Loader_Struct* pDataOut);

uint8_t SFBasicLoaderPointer(uint8_t* pPointerAddr, uint8_t displacement, Loader_Struct* pDataOut);

uint8_t SFGetDisplacementNumber(uint8_t* pPointerAddr, uint8_t *pDisplNum);

uint16_t LIB_ChkComputation (uint16_t  len, uint8_t * pdata, uint8_t seed);

uint16_t* GetDefaultSettingStartAddress(void);

#if defined (IO_MANAGER_BASICLOADER_CSTM) 
#if ( IO_MANAGER_BASICLOADER_CSTM == FALSE )
uint8_t BasicLoader(settingPntr_t pointer, uint16_t displacement, Loader_Struct* pDataOut);
uint8_t MSettingBasicLoader(uint8_t SFIndex, settingPntr_t pointer, uint16_t displacement,  Loader_Struct* pDataOut);
#endif /* #if defined (IO_MANAGER_BASICLOADER_CSTM == FALSE)  */
#endif /* #if ( IO_MANAGER_BASICLOADER_CSTM )*/

uint8_t LIB_SEF_GetMaxMsettings(void);
uint16_t *LIB_SEF_GetMsettingStartAddr(uint8_t SFIndex);
uint16_t *LIB_SEF_GetBEWrdRelShort_Ptr(uint16_t *pDataStartAddr, settingPntr_t pointer);
bool_t LIB_SEF_IsExtdDisplPtr(uint8_t SFIndex, settingPntr_t pointer );
uint8_t LIB_SEF_GetDisplData(uint16_t* dataStartAddr, uint16_t displacement, Loader_Struct* pDataOut);

uint8_t LIB_SEF_GetFaultPriority(bool_t getApplTable, uint8_t fltCode, uint8_t fltSubCode, uint8_t *fltPrio);
uint8_t LIB_SEF_FaultPriorityField(uint8_t faultPrio);
bool_t LIB_SEF_IsFaultToDisplayON(uint8_t faultPrio);
bool_t LIB_SEF_GetFaultToDisplay(bool_t getApplTable, uint8_t fltCode, uint8_t fltSubCode);

uint8_t LIB_CAL_GetCalibData(uint8_t caliBoxID, Loader_Struct* pDataOut);
/*@}*/

/* *********** */
/* End of file */
/* *********** */
#endif /* __LIB_UTILITIES_H */
