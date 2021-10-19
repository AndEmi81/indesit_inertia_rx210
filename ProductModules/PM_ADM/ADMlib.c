/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par    ADMlib.c
\n      Implementation of function used by ADM Manager
\n
\file   ADMlib.c
\ingroup  PM_ADM
\date   Feb X, 2010
\version  01.00.00
\author   Andrea.Mantovani
\n
\n
\Par\b  History:
\n
\n    00.00.49
\n    16/06/2010
\n    Roberto Fioravanti
\n    changes:
\n		-	LKT2 and STS track parameters updated with data about event
\n		-	addition: UDM suport for event and time acquisition
\n		-	ADM_GetSF_ASFDataRecord, ADM_GetSFDataRecord, ADM_GetSDATSize, ADM_GetSFDataRecordInfo prototypes changed (pSetting), 
\n
\n
\n    00.00.24
\n    Feb X, 2010
\n    Andrea.Mantovani
\n    creation
\n
\n
\n etc.
*/

/* **********/
/* Inlcudes */
/* **********/

#include "StandardTypes.h"
#include "StandardDefines.h"

#include "Hal_Interface.h"
#include HAL_CFG
#include "IO_driver.h"


#include "ADMlib.h"
#include "LIB_Utilities.h"


/* **********/
/* Externs  */
/* **********/
extern const struct IdentTableStruct IdentTable;

/* **********/
/* Statics  */
/* **********/

static uint16_t s_ui16SDATSize=0;


Loader_Struct stSFData;



/* **********/
/* Functions */
/* **********/

/**
This function returns the address in Flash where is stored the Ident table of the Appliance provided  
   It must search the ui16ApplI2CAdd inside the S-DAT and then get the IdentTable address.
   This address is stored at the first position after the pointer to Next Appliance (see doc) 

\param  [in]		ui16ApplI2CAdd: 
\param  [out] 	    pstData: 

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

uint8_t ADM_GetApplIdentAdd(uint16_t ui16ApplI2CAdd, ADMLibDataType* pstData) 
{
  uint8_t*  pui8SDATPtr = (uint8_t*)START_SDAT;
  uint8_t*  pui8IdentAddPos;
  uint16_t ui16I2CAdd=0xFFFF,ui16Tmp1,ui16Tmp2;
  bool_t bFound=FALSE, bSDATend=FALSE;
  
  while(!bFound && !bSDATend) 
  {
    /* SDAT is always BE */
    ui16I2CAdd = GETW_BE(pui8SDATPtr);
    if(ui16I2CAdd == ui16ApplI2CAdd) 
    {
      bFound=TRUE;
    } 
    else 
    {
      pui8SDATPtr += (uint32_t)GETW_BE(pui8SDATPtr+APPL_NEXT_OFF);
      if(
        (pui8SDATPtr > (uint8_t*)(START_SDAT+s_ui16SDATSize))
        ||
        (pui8SDATPtr < (uint8_t*)(START_SDAT))
      ) 
      {
        bSDATend=TRUE;
      }
    }
  }
  
  if((ui16I2CAdd == ui16ApplI2CAdd) && (ui16I2CAdd != 0xFFFF))
  {
    /* position of the Ident address */
    pui8IdentAddPos  =  pui8SDATPtr + GETW_BE(pui8SDATPtr+APPL_IDENT_OFF); 
    
    // Ident address size is sizeof(FLS_positionType) or sizeof(SettingSizePointerType) ? 
    // to be decided which size is correct to do...
    
    /* 
       you can use either the macro GETW_xE/GETDW_xE or the memcpy.
       In case the data to be get is a memory address you could use the macro but defining:
       
       #if sizeof(FLS_positionType ==2)
       #define GETADD  GETW
       #else
         #if sizeof(FLS_positionType ==4)
         #define GETADD  GETDW
         #endif
       #endif
     */
    
    if(sizeof(FLS_positionType) == 2) 
    {
      pstData->pui8Data = (uint8_t*)GETW_BE(pui8IdentAddPos);
    } 
    
    /* 110721 ASaccà: remove condition always false and NULL pointer assignment
    
    else if (sizeof(FLS_positionType) == 4)
    {
      pstData->pui8Data = (uint8_t*)GETDW_BE(pui8IdentAddPos);
    }
    */    
    
    
    /* calculate also the size of Ident table */
      (void)memcpy((uint8_t*)&(ui16Tmp1),
             (uint8_t*)(pui8IdentAddPos),
              SDAT_SIZEOF_DATAPTR
            );
             
        (void)memcpy((uint8_t*)&(ui16Tmp2),
              (uint8_t*)(pui8IdentAddPos+sizeof(SettingSizePointerType)),
              SDAT_SIZEOF_DATAPTR
             );
             
    pstData->ui16DataSize=ui16Tmp2-ui16Tmp1;          
             
    return ADMLIB_OK;
  } 
  else
  {
    return ADMLIB_GENERIC_ERR;
  }
}

 

/**
This function calculates the size of the SDAT table by parsing SF.
It returns an error code and the size calculated.
This info is useful to know where to start saving data to flash.

\param  [in] 	  pSetting, pointer to setting file
\param  [out] 	pui16SDATSize: 

\return 	uint8_t
 
\retval	 ADMLIB_OK : OK
\retval  ADMLIB_GENERIC_ERR: error in setting file parsing

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0

        
\note		\par \b Note 1: it updates a global var (s_ui16SDATSize) used outside this function.
			documentation by Andrea.Mantovani

*/
uint8_t ADM_GetSDATSize(uint16_t* pSetting, uint16_t* pui16SDATSize) 
{
  static uint8_t ui8Result;
  uint8_t  iCount,ui8NumAppl=0;
  uint16_t ui16Tmp,ui16NumStaticData=0,ui16SDATSize;
  AddressingTracksType stTrackData;
  
  
  iCount=0;
  ui16Tmp=0xFFFF;
  
  ui8Result = SFBasicLoader(pSetting, ADM_SF_ADDRESSING_TRACKS_PTR, iCount, &stSFData);
  while (ui8Result == SF_OK)
  {
    stTrackData.ui8ApplI2CAdd = (stSFData.Data)[0];
    stTrackData.ui8DataTypeID = (stSFData.Data)[1];
    
    if(stSFData.DataLenght !=0)
    {
      if(stTrackData.ui8ApplI2CAdd != ui16Tmp)
      {
       ui8NumAppl++;
       ui16Tmp=stTrackData.ui8ApplI2CAdd;
      }
      
      if( (stTrackData.ui8DataTypeID == DATATYPEID_ASF) ||
          (stTrackData.ui8DataTypeID == DATATYPEID_IOF)     )
      {
        ui16NumStaticData++;
      }
    }
    iCount++;
    ui8Result = SFBasicLoader(pSetting, ADM_SF_ADDRESSING_TRACKS_PTR, iCount, &stSFData);    
  }
  
  if(ui8Result == SF_LDR_ERR_ENDOFPOINTER)
  {
    /* parsing completed
       size of S-DAT =
     */
    *pui16SDATSize =  (uint16_t) (ui8NumAppl * SDAT_FIX_PART_SIZE_PERAPPL)+      /*Fixed part*/
                      (uint16_t) (ui8NumAppl * sizeof(FLS_positionType))  +      /* Appl Ident pointers*/
                      (uint16_t) (ui8NumAppl * sizeof(FLS_positionType))  +      /* Dummy fixed datat pointers */
                      (uint16_t) (ui16NumStaticData * sizeof(FLS_positionType)); /* Fixed data pointers*/
                      
   /* update also the global var */
   s_ui16SDATSize= *pui16SDATSize;                  
                   
   return ADMLIB_OK;
                   
  }else
  {
    /* error condition */
    return ADMLIB_GENERIC_ERR;
  }
}


/**
This function return the Appliance I2C address and the data_type(ASF, IOF, LKT, UDM, IOFD)
of the curently pointed track.

\param  [in] 	  pSetting, pointer to setting file
\param  [in] 	  ui8CurrDisplIdx - Index of the Addressing track to be processed 
\param  [out] 	pstDataInfo: 

\return 	uint8_t  (error in setting file)

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
uint8_t ADM_GetSFDataRecordInfo(uint16_t* pSetting, ADM_DataInfoType* pstDataInfo,uint8_t ui8CurrDisplIdx) 
{
  uint8_t  ui8Ret,ui8DataParamsIdx;
  
  ui8Ret=SFBasicLoader(pSetting,
					   ADM_SF_ADDRESSING_TRACKS_PTR,
					   ui8CurrDisplIdx, &stSFData);
  
  if (ui8Ret == SF_OK) 
  {
    pstDataInfo->ui16ApplI2CAdd =(stSFData.Data)[0];
    pstDataInfo->ui8DataTypeID =(stSFData.Data)[1];
  } 
  
  return ui8Ret;
  
}


/**
 This function returns info about the record extracted from Setting File 
 You must provide the Ident address of the current appliance.

\param  [in] 	  pSetting, pointer to setting file
\param  [in] 	  pui8IdentAdd - Address where Ident Table is stored.
\param  [in] 	  ui8CurrDisplIdx - Index of the Addressing track to be processed. 
\param  [out] 	pstDataInfo  - track parameters. Some fields of this struct are menaingful only for certain types of data (ASF, LKT,etc etc)

 

\return 	uint8_t
 
\retval	 ADMLIB_OK : OK
\retval  ADMLIB_GENERIC_ERR: error in setting file parsing

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
        
\note		\par \b Note 1: it updates a global var (s_ui16SDATSize) used outside this function.
			documentation by Andrea.Mantovani

*/
uint8_t ADM_GetSFDataRecord(uint16_t* pSetting, ADM_DataInfoType* pstDataInfo,uint8_t* pui8IdentAdd,uint8_t ui8CurrDisplIdx) 
{
  uint8_t  ui8Ret,ui8MinSize,ui8DataParamsIdx,ui8IDTDispl;
  uint16_t ui16Tmp,ui16NumStaticData=0,ui16SDATSize;
  uint16_t* pui16ApplEEPAddress;
  Loader_Struct stSFData;
  
  /* initialize all returned struct with 0xFF
     this could be usefull when copying some files do flash memory
   */
  (void)memset (pstDataInfo,0xFF,sizeof(ADM_DataInfoType));
  
  ui16Tmp=0xFFFF;
  
  /* Extract info from TRACKS POINTER */
  ui8Ret=SFBasicLoader(pSetting,
					   ADM_SF_ADDRESSING_TRACKS_PTR,
					   ui8CurrDisplIdx, &stSFData);
  
  if (ui8Ret == SF_OK) 
  {
    pstDataInfo->ui16ApplI2CAdd =(stSFData.Data)[0];
    pstDataInfo->ui8DataTypeID =(stSFData.Data)[1];
    ui8DataParamsIdx =(stSFData.Data)[2];
    pstDataInfo->ui8Tag=(stSFData.Data)[3];
  } else 
  {
    return ui8Ret;
  }
   /* Extract info from TRACKS PARAM POINTER */ 
  ui8Ret=SFBasicLoader(pSetting,
                        ADM_SF_TRACKS_PARAM_PTR,
                        ui8DataParamsIdx, &stSFData);
    
    if (ui8Ret == SF_OK) 
    { 
      /* Appliance IDENT has been saved prior to calling this function */
      
        switch(pstDataInfo->ui8DataTypeID) 
        {
          case DATATYPEID_ASF:
          if(2*(stSFData.DataLenght) >= ASF_RECORD_SIZE_MIN) 
          {
            /* 
               (stSFData.Data)[0]  APPL_SF pointer
               (stSFData.Data)[1]  APPL_SF displ
            
                get the absolute address in Appliance
                For this kind of data you must parse the appliance Ident Table.
                Appl Ident table is stored in Flash and its position is passed as parameter.
            */
            
            
            if((stSFData.Data)[0] == 0xFF) 
            {
              /* Download whole setting file
                 modified the data type ID */
              pstDataInfo->ui8DataTypeID = DATATYPEID_ASF_ALL;
            } 
            else if ((stSFData.Data)[1] == 0xFF) 
            {
              /* Download the whole pointer range
               we need to download a segment of the SF to serach in it for the displacement
               this can be done by returning a dedicated DataTypeID */
              pstDataInfo->ui8DataTypeID = DATATYPEID_ASF_WHOLE_PTR;
              pstDataInfo->ui8ASFPointer = (stSFData.Data)[0];
            } 
            else
            {
              pstDataInfo->ui8ASFPointer = (stSFData.Data)[0];
              pstDataInfo->ui8ASFDispl= (stSFData.Data)[1];
            }
              /* not really usefull for current implementation */
             /*pstDataInfo->bSave2Flash=TRUE;*/
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          break;
          
          case DATATYPEID_IOF:
          case DATATYPEID_IOFD:
          
          if(pstDataInfo->ui8DataTypeID == DATATYPEID_IOF) 
          {
            ui8MinSize = IOF_RECORD_SIZE_MIN;
            pstDataInfo->bSave2Flash=FALSE;
          }
          else  /* IOFD */ 
          {
            ui8MinSize =  IOFD_RECORD_SIZE_MIN;
            /* 
               For this type of data we have additional info:
               (stSFData.Data)[4] Data Acquisition Time
               (stSFData.Data)[5] Data Acquisition Event
             */
             pstDataInfo->ui16DataAcqTime =(stSFData.Data)[4];
             pstDataInfo->ui8DataAcqEvent=(stSFData.Data)[5];
             pstDataInfo->eUpdatePol=ADM_UPDATEBY_EVENT;
             pstDataInfo->bSave2Flash=TRUE;
             pstDataInfo->eUpdateWith=ADM_UPDATEWITH_RAMREQ;
             
          }
           
          if(2*(stSFData.DataLenght) >=ui8MinSize) 
          {
            /* (stSFData.Data)[0]  IDT displacement ADDRESS
               (stSFData.Data)[1]  IDT displacement SIZE
               ((stSFData.Data)[2]) & 0x0F  SIZE OF SIZE field
               (((stSFData.Data)[2]) & 0xF0)>>4  SIZE OF ADDRESS field
               (stSFData.Data)[3] Size Multipl Factor
            */
            
            /* Get the address */
            ui8IDTDispl=(stSFData.Data)[0];
            switch (((((stSFData.Data)[2]) & 0xF0)>>4)) 
            {
              case 1:
                pstDataInfo->ui32DataAddInAppl = (uint32_t)pui8IdentAdd[ui8IDTDispl];
              break;
              
              case 2:
                /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (((struct IdentTableStruct *)pui8IdentAdd)->header.littleEndian)
                {
                  pstDataInfo->ui32DataAddInAppl = (uint32_t) (GETW_LE(pui8IdentAdd+ui8IDTDispl));
                } 
                else 
                {
                  pstDataInfo->ui32DataAddInAppl = (uint32_t) (GETW_BE(pui8IdentAdd+ui8IDTDispl));
                }
              break;
              
              case 4:
              /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (((struct IdentTableStruct *)pui8IdentAdd)->header.littleEndian)
                {
                  pstDataInfo->ui32DataAddInAppl = (uint32_t) (GETDW_LE(pui8IdentAdd+ui8IDTDispl));
                } 
                else 
                {
                  pstDataInfo->ui32DataAddInAppl = (uint32_t) (GETDW_BE(pui8IdentAdd+ui8IDTDispl));
                }
                
              break;
              
              default:
                /* invalid size of address, save error to do...*/
                ui8Ret= ADMLIB_GENERIC_ERR; 
              break;
            }
            
            /* Get the size */
            ui8IDTDispl=(stSFData.Data)[1];
            switch (((stSFData.Data)[2]) & 0x0F) 
            {
              case 1:
                pstDataInfo->ui16DataSize = (uint16_t)pui8IdentAdd[ui8IDTDispl];
              break;
              
              case 2:
                /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (((struct IdentTableStruct *)pui8IdentAdd)->header.littleEndian)
                {
                  pstDataInfo->ui16DataSize = (uint16_t) (GETW_LE(pui8IdentAdd+ui8IDTDispl));
                } 
                else 
                {
                  pstDataInfo->ui16DataSize = (uint16_t) (GETW_BE(pui8IdentAdd+ui8IDTDispl));
                }
              break;
              
              case 4:
              /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (((struct IdentTableStruct *)pui8IdentAdd)->header.littleEndian)
                {
                  pstDataInfo->ui16DataSize = (uint16_t) (GETDW_LE(pui8IdentAdd+ui8IDTDispl));
                } 
                else 
                {
                  pstDataInfo->ui16DataSize = (uint16_t) (GETDW_BE(pui8IdentAdd+ui8IDTDispl));
                }
              break;
              
              default:
              /* invalid size of address, save error to do...*/
                ui8Ret= ADMLIB_GENERIC_ERR;
              break;
            }
            pstDataInfo->ui16DataSize *= (stSFData.Data)[3];
         
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
                   
          break;
          
            
          case DATATYPEID_STS:
         
          if(2*(stSFData.DataLenght) >=STS_RECORD_SIZE_MIN) 
          {
           /*   
               (stSFData.Data)[0]  Data Size
               (stSFData.Data)[1]  Data Acquisition Time
           */
           
           /* Data size for Status is updated upon receivement of response to data request
              MUST be put to 0xFF cause when it will be updated it will be in FLASH memory.
              If you put a vsalue different from 0xFFFF it cannot be overwritten.
            */
           pstDataInfo->ui16DataSize    = 0xFFFF;
           
           pstDataInfo->ui16DataAcqTime = (stSFData.Data)[0];
           pstDataInfo->eUpdatePol=ADM_UPDATEBY_TIME;
           pstDataInfo->bSave2Flash=FALSE;
           /* next info is used by request manager to know how to get data */
           pstDataInfo->eUpdateWith=ADM_UPDATEWITH_STATREQ;
           
          } 
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          
          break;
          
          case DATATYPEID_LKT:
          
          if(2*(stSFData.DataLenght) >=LKT_RECORD_SIZE_MIN) 
          {
            /* (stSFData.Data)[0]  LKT element
               (stSFData.Data)[1]  LKT data position
               (stSFData.Data)[2]  LKT Data Size
               (stSFData.Data)[3]  LKT Data Type
               (stSFData.Data)[4]  LKT Data Acq time
            */
            pstDataInfo->ui8LKTElem=(stSFData.Data)[0];
            pstDataInfo->ui8LKTDataPos=(stSFData.Data)[1];
            pstDataInfo->ui16DataAcqTime=(stSFData.Data)[4];
            //pstDataInfo->ui16DataSize = (stSFData.Data)[2] * (0x0F & (stSFData.Data)[3]);
            pstDataInfo->ui16DataSize = (stSFData.Data)[2] * (0x07 & (stSFData.Data)[3]);
            pstDataInfo->eUpdatePol=ADM_UPDATEBY_TIME;
            pstDataInfo->bSave2Flash=FALSE;
            pstDataInfo->eUpdateWith=ADM_UPDATEWITH_RAMREQ;
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          break;
          
          /*
           uint8_t   ui8LKTID;
           uint8_t   ui8LKTIDSearchType;
          */
          case DATATYPEID_LKT2:
          
          if(2*(stSFData.DataLenght) >=LKT2_RECORD_SIZE_MIN) 
          {
            /* (stSFData.Data)[0]  LKT Element
               (stSFData.Data)[1]  LKT Data ID
               (stSFData.Data)[2]  LKT Data Size
               (stSFData.Data)[3]  LKT Data Type
               (stSFData.Data)[4]  LKT Data Acq time
               (stSFData.Data)[5]  LKT event
               (stSFData.Data)[6]  LKT Search Type
            */
            pstDataInfo->ui8LKTElem=(stSFData.Data)[0];
            pstDataInfo->ui8LKTID=(stSFData.Data)[1];
            pstDataInfo->ui16DataAcqTime=(stSFData.Data)[4];
            pstDataInfo->ui8LKTIDSearchType=(stSFData.Data)[6];            
            //pstDataInfo->ui16DataSize = (stSFData.Data)[2] * (0x0F & (stSFData.Data)[3]);
            pstDataInfo->ui16DataSize = (stSFData.Data)[2] * (0x07 & (stSFData.Data)[3]);
            pstDataInfo->eUpdatePol=ADM_UPDATEBY_TIME;
            pstDataInfo->bSave2Flash=FALSE;
            pstDataInfo->eUpdateWith=ADM_UPDATEWITH_RAMREQ;
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          break;
          case DATATYPEID_UDM:
          
          if(2*(stSFData.DataLenght) >= UDM_RECORD_SIZE_MIN) 
          {
            /* (stSFData.Data)[0]  UDM Data Address
               (stSFData.Data)[1]  UDM Data Address
               (stSFData.Data)[2]  UDM Data Size
               (stSFData.Data)[3]  UDM Data Type
               (stSFData.Data)[4]  UDM Data Acq time
            */
            pstDataInfo->ui32DataAddInAppl  = (stSFData.Data)[1];
            pstDataInfo->ui32DataAddInAppl += (((stSFData.Data)[0] <<8) & 0xFF00);        
            pstDataInfo->ui16DataAcqTime=(stSFData.Data)[4];
            pstDataInfo->ui8DataAcqEvent=(stSFData.Data)[5];
            
			pstDataInfo->eUpdatePol= ADM_UPDATE_DISABLE;
			if(pstDataInfo->ui16DataAcqTime != 0)
			{
				pstDataInfo->eUpdatePol += (ADM_DataUpdPolicyType) ADM_UPDATEBY_TIME;
			}
			if(pstDataInfo->ui8DataAcqEvent != 0xFF)
			{
				pstDataInfo->eUpdatePol += (ADM_DataUpdPolicyType) ADM_UPDATEBY_EVENT;
			}
                         
            pstDataInfo->ui16DataSize = (stSFData.Data)[2] * (0x07 & (stSFData.Data)[3]);
            pstDataInfo->bSave2Flash=FALSE;
            pstDataInfo->eUpdateWith=ADM_UPDATEWITH_RAMREQ;
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          break;
          
          default:
            /* error invalid data type to do...*/
            ui8Ret= ADMLIB_GENERIC_ERR;
          break;
          
        }
    } 
   
 
 return ui8Ret; 
}



/**
 This function return info about the record extracted from Setting File only if it is of ASF type
 
\param  [in] 	  pSetting, pointer to setting file
\param  [in] 	  ui8CurrDisplIdx - Index of the Addressing track to be processed 
\param  [out] 	pstDataInfo  - track parameters. Some fields of this struct are menaingful only for certain types of data (ASF, LKT,etc etc)

 

\return 	uint8_t
 
\retval	 ADMLIB_OK : OK
\retval  ADMLIB_GENERIC_ERR: error in setting file parsing

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
        
\note		\par \b Note 1: it updates a global var (s_ui16SDATSize) used outside this function.
			documentation by Andrea.Mantovani

*/
uint8_t ADM_GetSF_ASFDataRecord(uint16_t* pSetting, ADM_DataInfoType* pstDataInfo,uint8_t ui8CurrDisplIdx) 
{
  uint8_t  ui8Ret,ui8MinSize,ui8DataParamsIdx,ui8IDTDispl;
  uint16_t ui16Tmp,ui16NumStaticData=0,ui16SDATSize;
  uint16_t* pui16ApplEEPAddress;
  Loader_Struct stSFData;
  
  ui16Tmp=0xFFFF;
  ui8Ret=SFBasicLoader(pSetting,
					   ADM_SF_ADDRESSING_TRACKS_PTR,
					   ui8CurrDisplIdx, &stSFData);
  
  if (ui8Ret == SF_OK) 
  {
    pstDataInfo->ui16ApplI2CAdd =(stSFData.Data)[0];
    pstDataInfo->ui8DataTypeID =(stSFData.Data)[1];
    ui8DataParamsIdx =(stSFData.Data)[2];
    pstDataInfo->ui8Tag=(stSFData.Data)[3];
  } else 
  {
    return ui8Ret;
  }
   
  ui8Ret=SFBasicLoader(pSetting,
                       ADM_SF_TRACKS_PARAM_PTR,
                       ui8DataParamsIdx, &stSFData);
    
    if (ui8Ret == SF_OK) 
    { 
      /* Appliance IDENT has been saved prior to calling this function */
      
        switch(pstDataInfo->ui8DataTypeID) 
        {
          case DATATYPEID_ASF:
          if(2*(stSFData.DataLenght) >= ASF_RECORD_SIZE_MIN) 
          {
            /* 
               (stSFData.Data)[0]  APPL_SF pointer
               (stSFData.Data)[1]  APPL_SF displ
            
                get the absolute address in Appliance
                For this kind of data you must parse the appliance Ident Table.
                Appl Ident table is stored in Flash and its position is passed as parameter.
            */
            
            
            if((stSFData.Data)[0] == 0xFF) 
            {
              /* Download whole setting file
                 modified the data type ID */
              pstDataInfo->ui8DataTypeID = DATATYPEID_ASF_ALL;
            } 
            else if ((stSFData.Data)[1] == 0xFF) 
            {
              /* Download the whole pointer range
               we need to download a segment of the SF to serach in it for the displacement
               this can be done by returning a dedicated DataTypeID */
              pstDataInfo->ui8DataTypeID = DATATYPEID_ASF_WHOLE_PTR;
              pstDataInfo->ui8ASFPointer = (stSFData.Data)[0];
            } 
            else
            {
              pstDataInfo->ui8ASFPointer = (stSFData.Data)[0];
              pstDataInfo->ui8ASFDispl= (stSFData.Data)[1];
            }
              /* not really usefull for current implementation */
             /*pstDataInfo->bSave2Flash=TRUE;*/
          }
          else 
          {
            ui8Ret= ADMLIB_GENERIC_ERR;
          }
          break;
          
          default:
            /* error invalid data type to do...*/
            ui8Ret= ADMLIB_GENERIC_ERR;
          break;
          
        }
    } 
   
 
 return ui8Ret; 
}



/**
 This function apply a serach algorithm inside the Setting file to find the LKT position respect to the LKT element
 

\param  [in] 	  ui8SearchID - Type of ID to search: 
\b              Note: This search is for \b LOADS.  
 

\param  [in] 	  pui8SFAddress - start address of the Setting File where to serach for the ui8SearchID
  
\param  [out] 	pui8Position  - position respect to the LKT element of the searched ID

\return 	uint8_t
 
\retval	 ADMLIB_OK : OK
\retval  ADMLIB_GENERIC_ERR: error in setting file parsing or LOAD NOT FOUND !

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
        
\note		\par \b Note 1: 

*/
uint8_t ADM_SearchPos_1(uint8_t ui8SearchID,uint16_t* pui16SFAddress,uint8_t* pui8Position) 
{
  Loader_Struct Tab_OutputData;
  uint8_t ui8Result,ui8nLoads; 
  

  ui8Result = SFBasicLoader( pui16SFAddress,
                             PTR_IO_CONFIG_DATA,
                             DSPL_IO_OUTPUT_MAP, 
                             &Tab_OutputData); 	
                      
  if(ui8Result != SF_OK) 
  {
    return ADMLIB_GENERIC_ERR;
  }
  /*
  You getthe pointer (char*)to the start of load description part of the setting file (Tab_OutputData.Data)and the length in words (Tab_OutputData.DataLenght).
  As the length of the description field of each load is 4 words (Cfr. Setting File par. Outputs Map) the number of loads on the board is:
  	nLoads = (Tab_OutputData.DataLenght/4); 
  */

  ui8nLoads = (Tab_OutputData.DataLenght/4); 
  	
  for(*pui8Position=0; *pui8Position < ui8nLoads; (*pui8Position)++)
  {
  	if(*Tab_OutputData.Data == ui8SearchID)
  	{
  		break;
  	}
  	else
  	{
  		Tab_OutputData.Data+=8; /* cause is pointer to char */
  	}
  }
  
  if(*pui8Position >= ui8nLoads)
  {
  	//Error!! Load Not Found
  	*pui8Position = 0xFF;
  	return ADMLIB_GENERIC_ERR;
  }
  return ADMLIB_OK;

}


/**
 This function apply a serach algorithm inside the Setting file to find the LKT position respect to the LKT element
 

\param  [in] 	  ui8SearchID - Type of ID to search
\b                Note: This search is for \b GENERIC \b INPUTS.


\param  [in] 	  pui8SFAddress - start address of the Setting File where to serach for the ui8SearchID
  
\param  [out] 	pui8Position  - position respect to the LKT element of the searched ID

\return 	uint8_t
 
\retval	 ADMLIB_OK : OK
\retval  ADMLIB_GENERIC_ERR: error in setting file parsing or GI NOT FOUND!

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
        
\note		\par \b Note 1: 

*/
uint8_t ADM_SearchPos_0(uint8_t ui8SearchID,uint16_t* pui16SFAddress,uint8_t* pui8Position) 
{
 Loader_Struct Tab_GIData;
  uint8_t ui8Result,ui8nGI; 
  

  ui8Result = SFBasicLoader( pui16SFAddress,
                             PTR_IO_CONFIG_DATA,
                             DSPL_IO_GENERIC_INPUT_MAP, 
                             &Tab_GIData); 	
                      
  if(ui8Result != SF_OK) 
  {
    return ADMLIB_GENERIC_ERR;
  }
  /*
  You getthe pointer (char*)to the start of Generic Input description part of the setting file (Tab_OutputData.Data)and the length in words (Tab_OutputData.DataLenght).
  As the length of the description field of each Generic Input is 2 words (Cfr. Setting File par. Outputs Map) the number of loads on the board is:
  	ui8nGI = (Tab_GIData.DataLenght/2); 
  */

  ui8nGI = (Tab_GIData.DataLenght/2); 
  	
  for(*pui8Position=0; *pui8Position < ui8nGI; (*pui8Position)++)
  {
    /* NOTE that the ID is in position (Tab_GIData.Data+1) !!! */
  	if(*(Tab_GIData.Data+1) == ui8SearchID)
  	{
  		break;
  	}
  	else
  	{
  		Tab_GIData.Data+=4; /* cause is pointer to char */
  	}
  }
  
  if(*pui8Position >= ui8nGI)
  {
  	//Error!! Generic Input Not Found
  	*pui8Position = 0xFF;
  	return ADMLIB_GENERIC_ERR;
  }
  
  *pui8Position &= 0x0F;
  
  return ADMLIB_OK;
}




/* *********** */
/* End of file */
/* *********** */




