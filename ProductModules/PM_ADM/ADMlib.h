/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		ADMlib.h 
\n			Header File of ADMlib.c
\n
\file		ADMlib.h
\ingroup	PM_ADM 
\date		Feb X, 2010 
\version	01.00.00 
\author		Andrea.Mantovani 
\n
\n
\Par\b	History:
\n
\n    xx/06/2010
\n    Roberto Fioravanti
\n    changes:
\n		-	addition: field lastUpdateID in DDATReferenceType
\n		-	addition: General types
\n		-   addition: APPL_CHK_SF_OFF to S-DAT
\n
\n 		00.24.00 
\n		Feb X, 2010 
\n		Andrea.Mantovani
\n		creation 
\n
\n
\n etc.
*/

#ifndef __ADMLIB_H
#define __ADMLIB_H

/* **********/
/* Includes */
/* **********/
#include "StandardTypes.h"
#include "ApplDataMng_cfg.h"


/* **********/
/* Defines  */
/* **********/
/** 
   Definition of offset of EEP START ADDRESS inside Ident Table 
   This is usefull in case you need to access Ident fileds in format
   ui8Ptr[OFFSET] instead of IDTPtr->EEPaddress 
*/
#define IDT_OFFSETOF_EEP_START_ADDR  0x08

/** 
   Definition of offset of LKT START ADDRESS inside Ident Table 
   This is usefull in case you need to access Ident fileds in format
   ui8Ptr[OFFSET] instead of IDTPtr->EEPaddress 
*/
#define IDT_OFFSETOF_LKT_START_ADDR  0x0A



/* ----------  S-DAT ---------- */

/** Start address of SDAT */
#define START_SDAT              (START_AVAIBLE_FLASH)

#define OFFSET_SIZE_INBYTES          2

/** Offset of I2C address inside fixed part of SDAT - see ADM doc*/
#define APPL_I2CADDR_OFF             0
/** Offset of APPL IDENT address inside fixed part of SDAT - see ADM doc*/
#define APPL_IDENT_OFF             (APPL_I2CADDR_OFF+OFFSET_SIZE_INBYTES)
/** Offset of First ASF data address inside fixed part of SDAT - see ADM doc*/
#define APPL_ASF_OFF               (APPL_IDENT_OFF+OFFSET_SIZE_INBYTES)
/** Offset of first IOF data address inside fixed part of SDAT - see ADM doc*/
#define APPL_IOF_OFF               (APPL_ASF_OFF+OFFSET_SIZE_INBYTES)

/**  - see ADM doc*/
#define APPL_CHK_SF_OFF               (APPL_IOF_OFF+OFFSET_SIZE_INBYTES)

/** Offset of next appliance record address inside fixed part of SDAT - see ADM doc*/
#define APPL_NEXT_OFF              (APPL_CHK_SF_OFF+OFFSET_SIZE_INBYTES)


/** offsets inside each data record */
#define S_DATAADD_OFFSET      0
#define SDAT_SIZEOF_DATAPTR             sizeof(SettingSizePointerType)



/**
 for each appliance the constant part is :
 
  sizeof APPLI2CADD      	= 2 (bytes)   +
  sizeof APPL_IDENT_OFF  	= 2 (bytes)   +
  sizeof APPL_ASF_OFF    	= 2 (bytes)   +
  sizeof APPL_IOF_OFF    	= 2 (bytes)   +
  sizeof APPL_CHK_ASF		= 2 (bytes)   +
  sizeof APPL_NEXT_OFF   	= 2 (bytes)   

 See ADM doc
*/
#define SDAT_FIX_PART_SIZE_PERAPPL      12


/* ----------  D-DAT ---------- */
/** position of I2Caddress inside the DDAT - see ADM doc */
//#define APPL_DYN_I2CADDR_OFF             0
/** position of Next appl Pointer inside the DDAT - see ADM doc */
//#define APPL_DYN_NEXT_OFF                (APPL_DYN_I2CADDR_OFF+OFFSET_SIZE_INBYTES)


/**
 for each appliance the constant part is :
 
  sizeof APPLI2CADD      = 2 (bytes)   +
  sizeof APPL_NEXT_OFF   = 2 (bytes)   

*/
#define DDAT_FIX_PART_SIZE_PERAPPL      4

/** Size of fileds of DDATType struct */
#define DDAT_SIZEOF_DATAPTR             sizeof(FLS_positionType)
#define DDAT_SIZEOF_DATAADD             sizeof(FLS_positionType)
#define DDAT_SIZEOF_DATASIZE            2
#define DDAT_SIZEOF_ACQTIME             2
#define DDAT_SIZEOF_EVIDX               1
#define DDAT_SIZEOF_MBIDX               1


/** offsets inside each data record */
#define DATAPTR_OFFSET      ((size_t) offsetof(DDATType,flsposDataPtr))
#define DATASIZE_OFFSET     ((size_t) offsetof(DDATType,ui16DataSize))
#define DATAADD_OFFSET      ((size_t) offsetof(DDATType,flsposDataAdd))
#define DATAACQTIME_OFFSET  ((size_t) offsetof(DDATType,ui16AcqTime))
#define DATAEVIDX_OFFSET    ((size_t) offsetof(DDATType,ui8EventIdx))
#ifdef REQ_OPTIM 
  #define DATAMBIDX_OFFSET    ((size_t) offsetof(DDATType,ui8MBIndex))
#endif


/** Data Type ID the values are specified in ADM documentation
    don't change unless agreed
 */
#define  DATATYPEID_RES             0x0
#define  DATATYPEID_ASF             0x01 /*Static*/
#define  DATATYPEID_LKT             0x02 /*Dynamic*/
#define  DATATYPEID_UDM             0x03 /*Dynamic*/
#define  DATATYPEID_IOF             0x04 /*Static*/
#define  DATATYPEID_IOFD            0x05 /*Dynamic*/
#define  DATATYPEID_STS             0x06 /*Dynamic*/
#define  DATATYPEID_LKT2            0x07 /*Dynamic*/
#define  DATATYPEID_ASF_ALL         0x08 /*Static: whole SF*/
#define  DATATYPEID_ASF_WHOLE_PTR   0x09 /*Static: whole Pointer*/

/** this is the minimun size of data pointed by Track Parameter pointer */
#define  ASF_RECORD_SIZE_MIN   2
#define  IOF_RECORD_SIZE_MIN   4
#define  LKT_RECORD_SIZE_MIN   5
#define  LKT2_RECORD_SIZE_MIN  7 
#define  UDM_RECORD_SIZE_MIN   5
#define  IOFD_RECORD_SIZE_MIN  6
#define  STS_RECORD_SIZE_MIN   2

/* **********/
/* Typedefs */
/* **********/

/** function pointer to functions used for LKT2 type */
typedef uint8_t (*FuncPtrType) (uint8_t SearchID,uint16_t* pui16SFAddress,uint8_t* pui8Position);

typedef enum 
{
  ADMLIB_OK=0,
  ADMLIB_GENERIC_ERR
} 
ADMLIB_ErrorType;


typedef enum 
{
  ADM_REQ_IDENT=0,
  ADM_REQ_RAM_DATA_CRC,
  ADM_REQ_RAM_DATA_NOCRC,
  ADM_REQ_STATUS,  
  ADM_REQ_NUMREQ
} ADMRequestType;

/*Warning: This value is rapresented with 2 bits */
typedef enum 
{
	ADM_UPDATE_DISABLE	=0,
	ADM_UPDATEBY_EVENT, /*1*/
	ADM_UPDATEBY_TIME	/*2*/
} 
ADM_DataUpdPolicyType;

/*Warning: This value is rapresented with 2 bits */
typedef enum 
{
  ADM_UPDATEWITH_RAMREQ=0,
  ADM_UPDATEWITH_STATREQ=1   
}
ADM_DataUpdWithType;

/** struct containing data pointed by Addressing Track pointer defined in ADM documentation */
typedef struct 
{
  uint8_t ui8ApplI2CAdd;
  uint8_t ui8DataTypeID;
  uint8_t ui8DataParamsIdx;
  uint8_t ui8Dummy;
} 
AddressingTracksType;



typedef struct 
{
  uint32_t  ui32DataAddInAppl;
  uint16_t  ui16ApplI2CAdd;
  uint16_t  ui16DataSize;
  uint16_t  ui16DataAcqTime;
  uint8_t   ui8DataAcqEvent;
  uint8_t   ui8Tag;  
  uint8_t   ui8DataTypeID;
  uint8_t   ui8ASFPointer;
  uint8_t   ui8ASFDispl;
  uint8_t   ui8LKTElem;
  uint8_t   ui8LKTDataPos;
  uint8_t   ui8LKTID;
  uint8_t   ui8LKTIDSearchType;
  ADM_DataUpdWithType     eUpdateWith;
  ADM_DataUpdPolicyType   eUpdatePol;
  bool_t    bSave2Flash; 
}
ADM_DataInfoType;

/* General Types */
typedef uint16_t 	ADM_TracksIndexType;
typedef uint8_t 	ADM_LastUpdateIdType;

/**
This data type is used to build a reference array to each dynamic data record inside the D-DAT
*/
typedef struct 
{
/** I2C address : This could be put as uint8_t in case I2C address 0xFF is not a valid one */
	uint16_t                ui16I2CAdd;
/**  Data TAG */
	uint8_t                  ui8Tag;
/** First address of the data record */
	FLS_positionType        DataRecPtrOff;
/** Time Stamp of the last update of this data
    in case request optimizer is enbaled this is used only by status request */
	uint16_t                ui16LastTS;
/** Id of the last Update */	
	ADM_LastUpdateIdType	LastUpdateID;
/** Tells the update policy of the data (Event &/| Time) */    
	uint8_t                 opt_eUpdatePol:2;
/** This is for particular dynamic data that must be saved in flash */  
	uint8_t                 opt_bSave2Flash:1;
/** Tells if the address has already been updated */
	uint8_t                 opt_bValidAddress:1;
/** Kind of request used to get data */  
	uint8_t                 opt_eUpdateBy:2    ;  
	uint8_t                 opt_free:2         ;  
  
} 
DDATReferenceType;

/**
This data type is used to build a reference array to each static data record inside the S-DAT
*/
typedef struct 
{
/** I2C address : This could be put as uint8_t in case I2C address 0xFF is not a valid one */
	uint16_t                ui16I2CAdd;
/**  Data TAG */
	uint8_t                  ui8Tag;
/** First address of the data record */
	FLS_positionType        DataRecPtrOff;
} 
SDATReferenceType;


typedef struct 
{
  uint8_t* pui8Data;
  uint16_t ui16DataSize;
}
ADMLibDataType;

/* ***********/
/* Functions */
/* ***********/

uint8_t ADM_GetSDATSize(uint16_t* pSetting, uint16_t* pui16SDATSize);

uint8_t ADM_GetSFDataRecordInfo(uint16_t* pSetting, ADM_DataInfoType* pstDataInfo,uint8_t ui8CurrDisplIdx);

uint8_t ADM_GetSFDataRecord(uint16_t* pSetting, ADM_DataInfoType*,uint8_t* pui8IdentAdd,uint8_t ui8CurrDisplIdx);

uint8_t ADM_GetSF_ASFDataRecord(uint16_t* pSetting, ADM_DataInfoType* pstDataInfo,uint8_t ui8CurrDisplIdx) ;

uint8_t ADM_GetApplIdentAdd(uint16_t ui16ApplI2CAdd,ADMLibDataType* pstData);

uint8_t ADM_SearchPos_1(uint8_t SearchID,uint16_t* pui8SFAddress,uint8_t* pui8Position);

uint8_t ADM_SearchPos_0(uint8_t SearchID,uint16_t* pui8SFAddress,uint8_t* pui8Position);

#endif



/* *********** */
/* End of file */
/* *********** */