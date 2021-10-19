/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup PM_ADM
@{
\Par    Appliance Data Managing
\n
\n\b    Features:
\n          This module is able to read and store in M.C.U. flash or RAM some kinds of data retrieved 
\n          by appliances whose I2C address is known. The communication between the ADM module and other appliances is made by I2C bus 
\n          (Communication Platform).
\n          The data to be get by ADM are specified in its Setting File (see ADM module documentation)
\n
\n\li       \b Ident \b Table (of a generic appliance with I2C address)
\n\li       \b ASF  data (Static stored in flash)
\n\li       \b IOF  data (Static stored in flash)
\n\li       \b LKT  data (Dynamic stored in RAM)
\n\li       \b UDM  data (Dynamic stored in RAM)
\n\li       \b IOFD data (Dynamic stored in RAM)
\n 
\n\b    Interface:
\n          The module provides APIs for data accessing once it has been stored to local memory. See ApplDataMng_full.h
\n
\n\b    Use:
\n\li       Link, inside the project, B.C.P. and the right HAL, module need config. cfr. related documentation.
\n          Note: in BusCommPltf_cfg.h you must also define the size of I2C Answer Buffer.
\n          Eg.:
\n          \#define \b MAX_DATA_IN_BYTES_TO_ACQUIRE    \b 64  
\n          This is the Answer Buffer size minus 2
\n          Note: Read Ram Packet with and without CRC must be supported.
\n          Note: in ApplDataMng_cfg.h you must define the Answer Buffer/s where requests on I2C Bus are stored and
\n          Ident Request Lenght.
\n          
\n          
\n\li       Link inside the project OCFLS driver and the right HAL,module need config. cfr. related documentation.
\n\li       Link inside the project OCWDT driver and the right HAL,module need config. cfr. related documentation.
\n\li       Link inside the project LIB_Utilities.
\n
\n\b    Configuration:
\n\li       Inside the table
\n\b        BEGIN_MCU_FLASH_MAP(NUM_FLASH_BLOCKS)
\n
\n\b        END_MCU_FLASH_MAP()
\n          insert a suitable flash's segment large enough to store data listed above.
\n          Eg.:
\n\b          BEGIN_MCU_FLASH_MAP(NUM_FLASH_BLOCKS)
\n            0x182C,0x5FFF, // ADM reserved FLASH
\n            0x6000,0x61FF  // block reserved for Statistics (IOFD)
\n\b          END_MCU_FLASH_MAP()
\n
\n          Note: a detailed description of the above table is in FLS driver documentation.
\n          Note: on \b HCS08QExy you can't use banked page.
\n\li       In ApplDataMng_cfg.h define the start avaible flash for ADM
\n          Eg.
\n          \#define \b START_AVAIBLE_FLASH     \b 0x0182C
\n\li       In ApplDataMng_cfg.h define the end avaible flash for ADM
\n          Eg.
\n          \#define \b END_AVAIBLE_FLASH     \b 0x05FFF
\n
\n
\n\li       Enable Request Optimizer by adding the following define 
\n            \#define \b REQ_OPTIM \b
\n           
\n\li       Enable Event Engine by adding the following define
\n            \#define \b EVENT_ENGINE\b
\n
\n\li       Define the kind of request ADM uses over communication platform
\n            \#define \b ADM_USES_BCP_BRIDGE_MODE \b
\n
\n\li       Define the positions of the three ADM Setting File pointers. 
\n          They are specified in ADM documentation and can be changed only if agreed with editor.
\n            \#define  \b ADM_SF_ADDRESSING_TRACKS_PTR  \b        0x05
\n            \#define  \b ADM_SF_TRACKS_PARAM_PTR       \b        0x06
\n            \#define  \b ADM_SF_EVENTS_PTR             \b        0x08 
\n
\n\li       Define MAX number of appliances (I2C addresses) on I2C bus
\n            \#define  \b MAX_APPLS                     \b        3
\n
\n\li       Define MAX number of static and dynamic data that you want to read
\n          These are the sizes of SDAT and DDAT reference tables: change this to better fit needed dimension 
\n            \#define \b  MAX_STAT_ELEM                 \b        20 
\n            \#define \b  MAX_DYN_ELEM                  \b        20 
\n
\n\li       In order to find the absolute address of a displacement you must get the whole extrapointers area and process it. 
\n          This is the dimension of array containing the extrapointers portion of Appliance Setting File
\n          If you set this value big enough you are able to get whatever displacement from a pointer.
\n            \#define \b  MAX_ASF_EXTRAPOINTERS_BYTES   \b        (128+1)
\n
\n\li       Define size of the buffer used to store dynamic data (except IOFD) 
\n        \b N.B. This buffer stores also DDAT and MAX_ASF_EXTRAPOINTERS_BYTES before they are copied to flash or no more used.
\n               This buffer must be at least:
\n                       MAX_ASF_EXTRAPOINTERS_BYTES +
\n                       MAX_DYN_ELEM * sizeof( DDATReferenceType )
\n 
\n            \#define \b  MAX_DYN_DATA_STORAGE_SIZE     \b        512
\n
\n\li       Define number of macroblocks. This is meaningful only when Request Optimizer is enabled.      
\n          This size determine the size of macro block table : change this to better fit needed dimension 
\n            \#define \b MAX_MACROBLOCK_NUMBER          \b        10
\n
\n\li        Define MAX gap between consecutive Macro Blocks for Request Optimizer (See ADM documentation for more details)
\n            \#define  \b MAXGAP                        \b        20
\n          
\n 
\n\li        Define MAX Number of Search functions used in LKT2 data type (See ADM documentation for more details)
\n            \#define  \b MAX_SEARCHID_FUNCTS           \b        3
\n
\n                                             
@}
\ingroup    ProductM
*/
/**
\n
\Par    Implementation of all functions necessary to achieve the aim.
\n
\file   ApplDataMng_full.c
\ingroup  PM_ADM
\date   Feb X, 2010
\version  01.00.00
\author   andrea.mantovani
\n
\n
\Par\b  History:
\n
\n    00.00.59
\n    30/07/2010
\n    Roberto Fioravanti
\n		-	ADM_REQMAN_CHECK_TMO in ADM_RequestManager: changed in order to process all the track
\n		-	ADM_BCP_GetAnswParams : addition of protection against unwanted ram writings
\n
\n    00.00.55
\n    30/07/2010
\n    Roberto Fioravanti
\n    	-	ADM manager state machine: bug fix SDAT not updated if setting embedded
\n		-	ADM Request Manager: improvement LKT data not acquired if LKT item is not in Lookuk table  
\n
\n    00.00.49
\n    14/06/2010
\n    Roberto Fioravanti
\n    	-	ADM manager state machine: addition of ADM_STATIC_DATA_GET_ASF_CHK_PTR_START,  ADM_STATIC_DATA_GET_ASF_CHK_PTR, ADM_STATIC_DATA_GET_ASF_CHK_VAL
\n		-	afpSearchTypeFuncts to const
\n		-	ADM manager state machine: reser all flash based on ADM_InitOptions value
\n		-	Init option management APIs: ADM_SetInitOptions
\n		-	Dynamic data acquisition: addition of LastUpdateID field to astDDATRef to keep trace of tha global last update
\n
\n    00.00.00
\n
\n    Feb X, 2010
\n    andrea.mantovani
\n    creation
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */

#include "ApplDataMng_full.h"
#include "OCFLS_drv.h"
#include "LIB_Utilities.h"

#include "GlobalVars.h"

#include "MngStatus.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */



//#define ADM_DEBUG_0

#ifdef ADM_DEBUG_0
  #define SCOPE static
#else
  #define SCOPE
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/** \enum Status for ADM MANGER which parse the Setting FIle, extracts static data and prepares struct for dynamic data*/
typedef enum
{ /** Clear flash */
  ADM_CLEAR_AVAILABLE_FLASH=0,
  /** Check if ADM must start from SF scan */
  ADM_PREREQUISITES_STATE,
  /** Preset pointers used in SF SCAN */
  ADM_SF_PRESCAN,
  /** Check if SF SCAN has just started or finished */
  ADM_SF_SCAN_INFO,
  /** Scan SF and get main info about each data to be retrieved */
  ADM_SF_SCAN,
  /** GET CHK_PTR on ASF track */
  ADM_STATIC_DATA_GET_ASF_CHK_PTR_START,
  ADM_STATIC_DATA_GET_ASF_CHK_PTR,
  ADM_STATIC_DATA_GET_ASF_CHK_VAL,
  /** Prepare to save fixed part of SDAT */
  ADM_UPDATE_FIXED_SDAT_START,
  /** save fixed part of SDAT */
  ADM_UPDATE_FIXED_SDAT,
  /** save DDAT to flase to save RAM memory */
  ADM_SAVE_DDAT_2FLASH,
  /** prepare to get Ident table */
  ADM_IDENT_GET_START,
  /** Get Ident table */
  ADM_IDENT_GET,
  /** Save Ident table */
  ADM_IDENT_SAVE,
  /** Prepare to get status */
  ADM_GET_STATUS_START,
  /** Get status */
  ADM_GET_STATUS,
  /** parse and save status in format common to all eldos */
  ADM_SAVE_STATUS,
  /** Prepare to get a static data */
  ADM_STATIC_DATA_GET_START,
  /** Get a static data */
  ADM_STATIC_DATA_GET,
  /** Save a static data */
  ADM_STATIC_DATA_SAVE,
  /** Save the pointer of a static data */
  ADM_STATIC_PTR_SAVE,
  /** Save the last pointer */
  ADM_STATIC_LAST_PTR_SAVE,
  /** Get data address from LKT */
  ADM_DYN_GET_LKT_DATA_ADDRESS_START,
  ADM_DYN_GET_LKT_DATA_ADDRESS,
  /** Update ddat table */
  ADM_UPDATE_DDAT,
  /** Get setting file size */
  ADM_GET_SETTING_FILE_SIZE_START,
  ADM_GET_SETTING_FILE_SIZE,
  /** Get a setting file pointer */
  ADM_STATIC_DATA_GET_ASF_PTR_START,
  ADM_STATIC_DATA_GET_ASF_PTR,
  /** Get number of extrapointers associated to a pointer */
  ADM_STATIC_DATA_GET_NUM_EXTRAP_START,
  ADM_STATIC_DATA_GET_NUM_EXTRAP,
  /** Locally save the extrapointers area */
  ADM_STATIC_DATA_GET_ASF_EXTRAPTRS_START,
  ADM_STATIC_DATA_GET_ASF_EXTRAPTRS,
  ADM_STATIC_DATA_SAVE_ASF_EXTRAPTRS,
  /** Get first displacement of a pointer */
  ADM_STATIC_DATA_GET_FIRST_DISP,
  ADM_STATIC_DATA_GET_FIRST_DISP_START,
  /** Calculate page of a given displacement */
  ADM_STATIC_DATA_CALC_ASF_DISPL_PAGE,
  /** Get data related to a displacement */
  ADM_STATIC_DATA_GET_ASF_DISPL_START,
  ADM_STATIC_DATA_GET_ASF_DISPL,
  /** Idle state */
  ADM_IDLE,
  /** Error state*/
  ADM_IDLE_STATE_JOB_KO
} ADMManagerStateType;


/** Save of important info during SF scan*/
typedef struct
{
  uint16_t ui16CurrApplI2CAdd;
  uint8_t* pui8IdentAdd;
  uint8_t  ui8CurrIOFDataNum;
  uint8_t  ui8CurrASFDataNum;
} CurrApplDataType;

/** Info on MAIN board */
typedef struct 
{
  /** I2C address of the MAIN board */
  uint8_t  ui8I2CAdd;
  /** Appliance type 
\n  APPLIANCE_CL 0 // Cooling
\n  APPLIANCE_DW 1 // Diswasher
\n  APPLIANCE_WM 2 // Wash Machine
\n  APPLIANCE_CK 3 // Coking
\n  APPLIANCE_DR 4 // Dryer
\n  APPLIANCE_HO 5 // Hobs
\n  BRIDGE_CECED 10 // Indoor Communication Node (Bridge, HW Key, ZigBee, etc..)
\n  CK_DOUBLE_OVEN 15
\n  CK_BIG_CAVITY 16
\n  DW_INDESIT_BASIC 26
\n  DW_INDESIT_PREMIUM 27
*/
  uint8_t  ui8AppType;
  /** Address where is stored the Ident table: 0xFFFF is invalid address */
  uint8_t* pui8IdentAdd;
  /** Address where is stored the raw status data */
  uint8_t* pui8StatAdd;
  /** Tells if Stat address is already updated */
  bool_t bValidStatAdd;
} MainBoardInfoType;


/** Table where is saved the store address of the SettingFile (if requested ) for each I2C address */
typedef struct 
{
  uint8_t           ui8I2CAdd;
  FLS_positionType  flsposSFaddress;
} SFMapTableType;


/** record of Macro Block table */
typedef struct 
{
  /** Macro Block start address */
  uint16_t ui16MBStartAdd;
  /** Macro Block end address */
  uint16_t ui16MBEndAdd;
  /** Macro Block Acquisition time */
  uint16_t ui16MBAcqTime;
  /** Macro Block Time stamp of the last request */
  uint16_t ui16MBLastTimeStamp;
  /** Ram address where MB is stored */
  uint16_t ui16RamAddress;
  /** Appliance I2C address*/
  uint8_t  ui8MBI2CAdd;
  /** tells if ui16RamAddress is valid */
  bool_t   bValidAddress;
} MBRecordType;


/**
This data type is used to build a single D-DAT record
*/
typedef struct 
{
  /** address of the data in Appliance */
  FLS_positionType        flsposDataPtr;
  /** Data Size */
  uint16_t                ui16DataSize;
  /** Address where is stored in ADM memory */
  FLS_positionType        flsposDataAdd;
  /** data acquisition time */
  uint16_t                ui16AcqTime;
  /** pointer to the event in case the update policy is by event */  
  uint8_t                 ui8EventIdx;
  /** This is the Macro Block index which data belongs to 
    this is used to calculate RAM address where data is stored */
#ifdef REQ_OPTIM     
  uint8_t                 ui8MBIndex;  
#endif
} DDATType;




/* ********* */
/* Extern    */
/* ********* */ 
extern const uint16_t Eeprom[];
extern const struct IdentTableStruct IdentTable;


/* ********* */
/* Variables */
/* ********* */

/* Global */

/* START DEBUG */
#if defined (ADM_DEBUG_0)
  bool_t FailChk = FALSE;
#endif
/* END DEBUG */

/* Static */

/** Start Address and lenght of Read Ram Requests. */
static PP_RRamParamType ReadRamAddresses;


/**
This variable contains the current position in flash from which data are stored, these positions must be inside\n
the avaible flash reserved for writing/erasing operations, check BEGIN_MCU_FLASH_MAP-END_MCU_FLASH_MAP.
*/
static FLS_positionType FlsDst;

/**
\n  This is the array containing function pointers to serarch ID function used in LKT2 Data type.
\n  ADM_SearchPos_0 is at index 0. This search is for GENERIC INPUTS.
\n  ADM_SearchPos_1 is at index 1. This search is for LOADS.
*/
const FuncPtrType afpSearchTypeFuncts[MAX_SEARCHID_FUNCTS] = {ADM_SearchPos_0,ADM_SearchPos_1};


/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */


/* Local Functions */

void ADM_BCP_GetAnswParams (uint8_t *pAnswerBuffer,uint16_t* pui16AnswLen,uint8_t* pui8PayloadOffset);

ADM_ErrorType ADM_DataReq(uint8_t ui8I2CAddress,ADMRequestType eRequest, void *pReqParams, IO_LenType lenReqParams);

void DDATRefSort(DDATReferenceType* astArray, uint8_t Nelem);

ADM_ErrorType ADM_GetMainProduct(uint8_t* pui8ApplType) ;
 
ADM_ErrorType ADM_GetMainStatAddr(uint8_t** ppui8ApplI2CAdd);


 
/**
State Machine info
*/
/** Error code used to discover the fault reason*/
static ADM_ManagerErrorType eADMErrorState; 
/** Main state*/
static ADMManagerStateType ADMManagerState;
/** Secondary state*/
static ADMManagerStateType ADMManagerSavedState;
/** */ 
static ADM_InitOptionsType ADM_InitOptions;


/**
This variables contain the current position where to write the S-DAT data \n
two pointers to the fixed part and one for the variable part.
*/
static FLS_positionType SDAT_CurrFixedPartPtr;
static FLS_positionType SDAT_PrevFixedPartPtr;
static FLS_positionType SDAT_CurrVarPartPtr;

/** Used to handle SF parse Start/End*/
static bool_t bFirstElement/*=TRUE*/;
static bool_t bEndJob=FALSE;

/* temporary to store Flash adress . Used when a data is bigger than maximum packet size */
static FLS_positionType StartFLSDest;

/** Keep Current appliance I2C address and other info during SF parse */
static CurrApplDataType stCurrApplData = {0xFFFF,(uint8_t*)0xFFFF,0,0};

/* temporary struct used to store data info while processing ADM setting file */
static ADM_DataInfoType stADMDataRecord;

/** index of address track pointer */
static uint8_t ui8CurrDisplIdx=0;

/* temp vars */
static uint16_t ui16EepAddr,ui16LktAddr;

/* 
   This is a buffer used to store the RAM copy of the fixed part of S-DAT for one appliance
   Once the RAM copy is update it is copied in flash
 */                                                             
static uint8_t aui8SDATFixedPart[SDAT_FIX_PART_SIZE_PERAPPL];

/**
\n  For D-DAT allocate a defined RAM area.
\n  SDAT size is determined at runtime.
\n  DDAtReference is a table of information related to each dynamic data
\n  (including TAG and pointer to data record in DDAT). 
\n  SDATReference contains data TAG and the pointer to data record in SDAT
\n  This represent a single element of DDAT table .
*/
static DDATType stDDATRecord;


/**-------------------------------------------------------
\n  In order to optimize the RAM usage some static variables that are used 
\n  only in Setting file parsing phase are put in a common byte array (aui8DynamicData) that is used in a second phase
\n  as buffer for dynamic data.
\n  \b WARNING !: if you put a struct in this area make sure it is word aligned otherwise 
\n  access to struct member could be affected.
\n---------------------------------------------------------*/
static uint8_t aui8DynamicData[MAX_DYN_DATA_STORAGE_SIZE];


/** mem where to copy the extrapointers area is first in mem buffer 
   this is used as a buffer and there's no need to have it word aligned */
static uint8_t* aui8ASFPointerData   =  aui8DynamicData;

/* 
   mem where to copy the DDAT is put just after aui8ASFPointerData 
   this is accessed by memcpy and there's no need to have it word aligned
*/ 
static FLS_positionType DDATBaseAdd = (FLS_positionType) (aui8DynamicData+64/*MAX_ASF_EXTRAPOINTERS_BYTES*/);

/*--------------------------------------------------------------------*/

/** DDATReference */
DDATReferenceType astDDATRef[MAX_DYN_ELEM+1];
uint8_t ui8DDATRefIndex;
uint16_t ui16DynDataWaterMark=0;

/** SDATReference */
SDATReferenceType astSDATRef[MAX_STAT_ELEM+1];
uint8_t ui8SDATRefIndex;


/* temporary  variable. Must be static */
static uint16_t ui16CurrDynApplI2CAdd = 0xFFFF;

/**
This variable contains the current position where to write the D-DAT data \n
*/
static FLS_positionType DDAT_CurrVarPartPtr;

/** Info on MAIN board */
MainBoardInfoType stMainBoardInfo ={0xFF,0xFF,(uint8_t*)0xFFFF};

/** 
    Table where is saved the store address of the SettingFile (if requested ) for each I2C address 
    This is used by LKT2 data search.
  */
SFMapTableType astSFMapTable[MAX_APPLS];


/*--------------------------------------*/
/* REQUEST OPTIMIZER Macro Block Table  */
/*--------------------------------------*/

/** REQUEST OPTIMIZER Macro Block Table */
#ifdef REQ_OPTIM
static MBRecordType astMBTable[MAX_MACROBLOCK_NUMBER];
#endif



/** General variables 
 * 
 @{*/
ADM_LastUpdateIdType 				ADM_DynLastUpdateID;
ADM_TracksIndexType					ADM_TracksUpdatedByTmoCount;
uint8_t 							ui8DDATRefOnTimeIndex;
/*@}*/
/* *************** */
/* Function Bodies */
/* *************** */

void ADM_SetADMManagerStateKO(void)
{
    ADMManagerState = ADM_IDLE_STATE_JOB_KO;
}

bool_t ADM_IsADMManagerStateKO(void)
{
    return (bool_t)(ADMManagerState == ADM_IDLE_STATE_JOB_KO);
}


void ADM_SetInitOptions(ADM_InitOptionsType initOptionsValue)
{
	ADM_InitOptions=initOptionsValue;
}
/**
This function returns the ID of the last completed update.   

\return 	ADM_LastUpdateIdType


\retval		ID
 

\author 	Roberto Fioravanti 
\date		04/06/2010 
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
ADM_LastUpdateIdType ADM_GetDynLastUpdateID(void)
{
	return ADM_DynLastUpdateID;
}

/**
This function returns the size in bytes of the dynamic area used.   

\return 	uint16_t

\retval		size of dynamic area
 

\author 	Roberto Fioravanti 
\date		04/06/2010 
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
uint16_t ADM_GetDynDataSize(void)
{
	return ui16DynDataWaterMark;
}

/**
This function returns pointer to the dynamic area   

\return 	uint8_t*

\retval		pointer to dynamic area
 

\author 	Roberto Fioravanti 
\date		04/06/2010 
\version	0.0.1

\note		\par \b Note 1:
			documentation by Roberto Fioravanti
*/
uint8_t* ADM_GetDynDataPnt(void)
{
	return aui8DynamicData;
}

/**
This function is an API to access ADM services.
\n It returns the address in Flash where is stored the Ident table of the Appliance provided.  
\nThis is an overload function that calls an ADM lib function.   

\param  [in]		ui16ApplI2CAdd: Appliance I2C address 
\param  [out] 	pstData: data poointer and size of the Ident 


\return 	ADM_ErrorType


\retval	ADM_E_OK	Ident found! pstData contains valid info
\retval	ADM_E_KO	Ident NOT found! pstData contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0

\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_GetIdent(uint8_t ui8ApplI2CAdd,ADMDataType* pstData) 
{
  ADMLibDataType stLibData= {0,0};
  uint8_t ui8Res;
  
  pstData->pui8Data     = stLibData.pui8Data;
  pstData->ui16DataSize = stLibData.ui16DataSize;
  
  ui8Res=ADM_GetApplIdentAdd((uint16_t)ui8ApplI2CAdd, &stLibData) ;
  
  if(ui8Res == ADMLIB_OK) 
  {
    pstData->pui8Data     = stLibData.pui8Data;
    pstData->ui16DataSize = stLibData.ui16DataSize;
    return ADM_E_OK; 
  } 
  else 
  {
    return ADM_E_KO;
  }
}

/**
This function is an API to access ADM services.
\n It allows users to make a data request to ADM module.
\n Data are retrieved starting from data TAG that is unique inside an entire ADM context.
\n For static data seach is made inside SDAT while for dinamic data there are two typed handled in a different way:
\n Static data update at regular time intervals are retrieved by the Macro block table cause they are handled by the request optimizer.
\n Static Datat updated upon events (IOFD) are retrieved directly by the DDAT reference.

\param  [in] 	  ui8Tag: tag to search for
\param  [out] 	pstData :Data and size of the requested var

\return 	ADM_ErrorType  

\retval	ADM_E_OK	tag found! pstData contains valid info
\retval	ADM_E_KO	tag NOT found! pstData contains NO valid info 

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0	
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/

ADM_ErrorType ADM_GetData(uint8_t ui8Tag, ADMDataType* pstData) 
 {
   /* search for TAG inside DDAT reference and SDAT reference */
  uint8_t   ui8Count=0;
  bool_t bFound=FALSE, bSearchEnd=FALSE;
  
  SettingSizePointerType ssptTmp1,ssptTmp2;
   
  /* search for TAG in DDAT */
  while(!bFound && !bSearchEnd) 
  {
    if(astDDATRef[ui8Count].ui16I2CAdd != 0xFFFF) 
    {
      if(ui8Tag == astDDATRef[ui8Count].ui8Tag) 
      {
      
#ifdef REQ_OPTIM  
        /* if data is updated upon time intervals.... */
        if ( (astDDATRef[ui8Count].opt_eUpdatePol == ADM_UPDATEBY_TIME) &&
             (astDDATRef[ui8Count].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ)
           ) 
        {
          
          uint8_t  ui8MbIdx;
          
          // MEM OPTIM
          /* get MBIndex from DDAT */
           (void)memcpy((uint8_t*)&(ui8MbIdx),
            (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATAMBIDX_OFFSET),
             DDAT_SIZEOF_MBIDX
           );
              
          if(astMBTable[ui8MbIdx].bValidAddress == TRUE) 
          { 
            /* in this case in DDAT is stored the offset respect to Macro Block address */
            (void)memcpy(&ssptTmp1,
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATAADD_OFFSET),
                  DDAT_SIZEOF_DATAPTR
                 );
            /* sum the macro block address (where is stored in RAM) to get the actual address 
               where this dynamic data is stored */     
            ssptTmp1 += astMBTable[ui8MbIdx].ui16RamAddress;
            
            
            /* return this address */
            pstData->pui8Data = (uint8_t*)ssptTmp1;
           
            /* return size*/     
            (void)memcpy((uint8_t*)&(pstData->ui16DataSize),
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATASIZE_OFFSET),
                  DDAT_SIZEOF_DATASIZE
                 );
        
            bFound=TRUE;
          } 
          else 
          {
            /* TAG found but data not yet available */
            pstData->pui8Data=0;
            pstData->ui16DataSize=0;
            return ADM_E_KO;
          }
        }
        else  /* if update by event or update with status request 
                 the data is get without request optimizer */
        {
          if(astDDATRef[ui8Count].opt_bValidAddress == 1) 
          {
            /* in this case in the DDAT reference is stored the address */
            (void)memcpy(&(pstData->pui8Data),
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATAADD_OFFSET),
                  DDAT_SIZEOF_DATAPTR
                 );
                 
            /* return the size */     
            (void)memcpy((uint8_t*)&(pstData->ui16DataSize),
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATASIZE_OFFSET),
                  DDAT_SIZEOF_DATASIZE
                 );
        
            bFound=TRUE;
          } 
          else 
          {
            /* TAG found but data not yet available */
            pstData->pui8Data=0;
            pstData->ui16DataSize=0;
            return ADM_E_KO;
          }
        }
#else
        
        if(astDDATRef[ui8Count].opt_bValidAddress == 1) 
          {
            /* in this case in the DDAT reference is stored the address */
            (void)memcpy(&(pstData->pui8Data),
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATAADD_OFFSET),
                  DDAT_SIZEOF_DATAPTR
                 );
                 
            /* return the size */     
            (void)memcpy((uint8_t*)&(pstData->ui16DataSize),
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui8Count].DataRecPtrOff+DATASIZE_OFFSET),
                  DDAT_SIZEOF_DATASIZE
                 );
        
            bFound=TRUE;
          } 
          else 
          {
            /* TAG found but data not yet available */
            pstData->pui8Data=0;
            pstData->ui16DataSize=0;
            return ADM_E_KO;
          }
          
#endif
      } 
      else 
      {
        ui8Count++;
      }
    } 
    else 
    {
      bSearchEnd=TRUE;
    }
  }
  
  bSearchEnd=FALSE;
  ui8Count=0;
  
  /* search for tag in static data*/
  while(!bFound && !bSearchEnd) 
  {
    /* search FOR TAG in SDAT */
    if(astSDATRef[ui8Count].ui16I2CAdd != 0xFFFF) 
    {
      if(ui8Tag == astSDATRef[ui8Count].ui8Tag) 
      {
        (void)memcpy((uint8_t*)&(ssptTmp1),
              (uint8_t*)(START_SDAT+astSDATRef[ui8Count].DataRecPtrOff+S_DATAADD_OFFSET),
              SDAT_SIZEOF_DATAPTR
             );
        
        /* return address where this data is stored. It will be a flash address */
        pstData->pui8Data=(uint8_t*)ssptTmp1;
             
        /* Get next pointer to calculate size */     
        (void)memcpy((uint8_t*)&(ssptTmp2),
              (uint8_t*)(START_SDAT+astSDATRef[ui8Count].DataRecPtrOff+S_DATAADD_OFFSET+sizeof(SettingSizePointerType)),
              SDAT_SIZEOF_DATAPTR
             );  

        /* in SDAT data size is not stored but must be calculated */
        pstData->ui16DataSize = ssptTmp2-ssptTmp1;
        
        bFound=TRUE;
      } 
      else 
      {
        ui8Count++;
      }
    } 
    else 
    {
      bSearchEnd=TRUE;
    }
  }
  
  
  if(bFound) 
  {
    return ADM_E_OK;
  } 
  else 
  { 
    pstData->pui8Data=0;
    pstData->ui16DataSize=0;
    return ADM_E_KO;
  }
 }


/**
This function extracts address and size of data related to a given displacement 
of a given pointer belonging to the setting file of an appliance. The appliance is identified by its I2C address.
This operation is made by searching among ASF data elements of ADM setting file.
The data can be found only if in the ADM setting file you configured the retrievement of either the specified pointer/displacement 
or the entire pointer or the entire Setting File.  


\param  [in]		ui8I2CAddr: where to get the info.
\param  [in] 	  pointer: Setting file pointer. This value corresponds to the position in word 
                respect to the SF start address of the pointer value.(see setting file documentation)
\param  [in] 	  displacement: index of the displacement. (see setting file documentation)
\param  [out] 	pDataOut: is composed by the absolute address and the size (in words) of the returned data

\return 	uint8_t

\retval	ADM_E_OK	data     found! pstData contains    valid info
\retval	ADM_E_KO	data NOT found! pstData contains NO valid info 
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_BasicLoader(uint8_t ui8I2CAddr, uint8_t pointer, uint8_t displacement, ADMDataType* pstData) 
{
  bool_t  bDataFound=FALSE;
  uint8_t ui8Idx=0,ui8Ret=SF_OK,ui8Ret2;
  ADM_DataInfoType stDataRecord;
  ADMDataType  stData;
  ADM_ErrorType eResult;
  Loader_Struct DataOut;
  
  /* search loop inside ADM SF */
  while(ui8Ret==SF_OK && bDataFound==FALSE) 
  {
    ui8Ret=ADM_GetSFDataRecordInfo((uint16_t*)&Eeprom[0], &stDataRecord, ui8Idx);
    
    if(
        (ui8I2CAddr == (uint8_t)stDataRecord.ui16ApplI2CAdd) &&     /* I2C address */
        (ui8Ret== SF_OK)                                            /* result OK */
    ) 
    {
      /* process only ASF data */
      ui8Ret=ADM_GetSF_ASFDataRecord((uint16_t*)&Eeprom[0], &stDataRecord, ui8Idx);
      
      if(ui8Ret != SF_OK) 
      {
        return ADM_E_KO;  
      }
        
      switch(stDataRecord.ui8DataTypeID) 
      {
        case DATATYPEID_ASF_ALL:
       
           /* get the address where is stored the whole setting file */
           eResult=ADM_GetData(stDataRecord.ui8Tag,&stData);
           
           if(eResult==ADM_E_OK) 
           {
             /* apply SFBasicLoader to stData that represent the whole Setting file */
             ui8Ret2 = SFBasicLoader((uint16_t*) stData.pui8Data, pointer, displacement, &DataOut);
             if(ui8Ret2==SF_OK) 
             {
               bDataFound=TRUE;
               pstData->pui8Data     = DataOut.Data;
               pstData->ui16DataSize = DataOut.DataLenght*2;
             } 
             else 
             {
               return ADM_E_KO;
             }
           } 
           else 
           {
             return ADM_E_KO;
           }
         
        break;
        
        case DATATYPEID_ASF_WHOLE_PTR:
        
            
            /* filter by pointer value */
            if(stDataRecord.ui8ASFPointer != pointer)
            {
                ui8Idx++;
                continue;
            }
           
            /* get the address where is stored the whole pointer data */
            eResult=ADM_GetData(stDataRecord.ui8Tag,&stData);
           
           if(eResult==ADM_E_OK) 
           {
             /* apply a specific function SFPointBasicLoader() that is found in lib 
               to extract the data related to the specified displacement
              */
              ui8Ret2=SFBasicLoaderPointer(stData.pui8Data, displacement, &DataOut);
             
             if(ui8Ret2==SF_OK) 
             {
               bDataFound=TRUE;
               pstData->pui8Data     = DataOut.Data;
               pstData->ui16DataSize = DataOut.DataLenght*2;
             } 
             else 
             {
               return ADM_E_KO;
             }
           } 
           else 
           {
             return ADM_E_KO;
           }
           
        break;
        
        case DATATYPEID_ASF:
            
            /* filter by pointer and displacemente values */
            if(
                (stDataRecord.ui8ASFPointer != pointer) ||
                (stDataRecord.ui8ASFDispl != displacement)
              )
            {
                ui8Idx++;
                continue;
            }
            
           /* get the address where is stored the displacement data */
           eResult=ADM_GetData(stDataRecord.ui8Tag, pstData);
           
           if(eResult==ADM_E_OK) 
           {
             bDataFound=TRUE;
           }
           else 
           {
             return ADM_E_KO;
           }
           
        break;
              
      }
    }
    ui8Idx++;
  } /* while */
  
  if(bDataFound) 
  {
    return ADM_E_OK;
  } 
  else 
  {
    return ADM_E_KO;
  }
}



/**
This function is an API to access ADM services.
It returns the I2C address of the MAIN board.
This info is retrieved by the ADM Manager.
You must call this function once ADM manager has finished his task otherwise you'll get an error   

\param  [in]		none
\param  [out] 	pui8ApplI2CAdd: I2C Address of the MAIN board


\return 	ADM_ErrorType


\retval	ADM_E_OK	Ident found! pstData contains valid info
\retval	ADM_E_KO	Ident NOT found! pstData contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0

\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_GetMainAddr(uint8_t* pui8ApplI2CAdd) 
{
   if(stMainBoardInfo.ui8I2CAdd != 0xFF) 
   {
     *pui8ApplI2CAdd = stMainBoardInfo.ui8I2CAdd;
     return ADM_E_OK;
   } 
   else 
   {
     return ADM_E_KO;
   }
}

/**
This function is an API to access ADM services.
It returns the Appliance type
This info is retrieved by the ADM Manager.
You must call this function once ADM manager has finished his task otherwise you'll get an error   

\param  [in]		none
\param  [out] 	pui8ApplType: Appliance type. Possible values are:
\n               APPLIANCE_CL				  0	 // Cooling
\n               APPLIANCE_DW				  1	 // Diswasher
\n               APPLIANCE_WM				  2	 // Wash Machine
\n               APPLIANCE_CK				  3	 // Coking
\n               APPLIANCE_DR				  4	 // Dryer
\n               APPLIANCE_HO				  5	 // Hobs
\n               BRIDGE_CECED        10  // Indoor Communication Node (Bridge, HW Key, ZigBee, etc..)
\n               CK_DOUBLE_OVEN			 15
\n               CK_BIG_CAVITY			 16
\n               DW_INDESIT_BASIC		 26
\n               DW_INDESIT_PREMIUM	 27

\return 	ADM_ErrorType


\retval	ADM_E_OK	Ident found! pstData contains valid info
\retval	ADM_E_KO	Ident NOT found! pstData contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_GetMainProduct(uint8_t* pui8ApplType) 
{
   if(stMainBoardInfo.ui8I2CAdd != 0xFF) 
   {
     *pui8ApplType = stMainBoardInfo.ui8AppType;
     return ADM_E_OK;
   } 
   else 
   {
     return ADM_E_KO;
   }
}

 
/**
This function is an API to access ADM services.
It returns the Address where is stored raw status data of the MAIN board.
This info is retrieved by the ADM Request Manager.
You must call this function once ADM Request manager has has already get this data otherwise you'll get an error   

\param  [in]		none
\param  [out] 	pui8ApplI2CAdd: I2C Address of the MAIN board


\return 	ADM_ErrorType


\retval	ADM_E_OK	Ident found! pstData contains valid info
\retval	ADM_E_KO	Ident NOT found! pstData contains NO valid info
 

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0
 
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_GetMainStatAddr(uint8_t** ppui8ApplI2CAdd) 
{
   if  ( (stMainBoardInfo.ui8I2CAdd != 0xFF) && 
         (stMainBoardInfo.bValidStatAdd== TRUE) 
         )
   {
     *ppui8ApplI2CAdd = stMainBoardInfo.pui8StatAdd;
     return ADM_E_OK;
   } 
   else 
   {
     return ADM_E_KO;
   }
}


/**
This function is an API to access ADM services.
It returns the Status info formatted as Struct_Status


\param  [in]		ui8ColdReset : Cold reset event is used to calculate some status info
\param  [out] 	RefStatus    : status formatted as Struct_Status


\return 	ADM_ErrorType


\retval	ADM_E_OK	Status correctly processed. RefStatus contains valid info.
\retval	ADM_E_KO	Status NOT correctly processed. RefStatus contains NO valid info.
 

\author 	Andrea.Mantovani 
\date		Apr X, 2010 
\version	1.0.0
	   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_GetApplianceStatus(uint8_t ui8ColdReset,Struct_Status *RefStatus)
{
  ADM_StatusErrType eErr;
  ADM_ErrorType     eErr2;
  byte_def coldResetEvent;
  uint8_t ui8Product;
  uint8_t* pui8RawStatAdd;
  
  coldResetEvent.BYTE=ui8ColdReset;
  
  eErr2=ADM_GetMainProduct(&ui8Product);
  if(eErr2 != ADM_E_OK) 
  {
    return ADM_E_KO;
  }
  
  eErr2=ADM_GetMainStatAddr(&pui8RawStatAdd);
  if(eErr2 != ADM_E_OK) 
  {
    return ADM_E_KO;
  }
  
  /* Call the right function to decode the appliance status */
  switch (ui8Product)
  {
  case APPLIANCE_WM: /* washing machine */
  	eErr=GetApplianceStatusWM((byte_def*)pui8RawStatAdd, RefStatus);
  	break;

  case APPLIANCE_DW: /* dishwashing */
  	eErr=GetApplianceStatusDW(coldResetEvent,(byte_def*)pui8RawStatAdd, RefStatus);
  	break;

  case APPLIANCE_CK: /* oven */
    eErr= (ADM_StatusErrType) GetApplianceStatusCK((byte_def*)pui8RawStatAdd, RefStatus);
  	break;

  case APPLIANCE_CL: /* fridge */
  	eErr=GetApplianceStatusCL2008((byte_def*)pui8RawStatAdd, RefStatus);
  	break;
  	
  	default:
  	/* invalid Appliance code */
  	 eErr=E_STAT_KO;
  	break;
  }
  
  if(eErr != E_STAT_OK) 
  {
    return ADM_E_KO;
  }
  
  return ADM_E_OK;
}

/*===========================================================================
  ADM MANAGER
===========================================================================*/


/**
This function implements the state machine for the ADM functionality related to:
\n - Setting file parsing
\n - Static data retrievement
\n - Creation of internal data structures used by RequestManager for dynamic data.

It must be called inside a main loop

\param  [in] 	  none
\param  [out] 	ADM_ManagerErrorType eADMErrorState (global var)

\return 	ADM_ErrorType  

\retval	ADM_E_OK      Job is done
\retval	ADM_E_WORKING	Job is ongoing
\retval	ADM_E_KO	    Job failed.An error code can be found in eADMErrorState

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_Manager(void)
{
  SCOPE IO_ErrorType flsErr; /*stack*/
  SCOPE uint8_t  ui8Ret; /* stack */
  SCOPE uint16_t ui16Tmp;/* stack */
  SCOPE uint8_t* pAnswerBuffer = I2CAnswerBuffer;  /* stack */
  SCOPE ADM_ErrorType admErr;  /* stack */

  static ADMRequestType  eReqType;
  static uint16_t ui16NumStaticData=0,ui16SDATSize;  
  static uint16_t ui16ASFNumExtraP,ui16ASFPtrVal;
  static uint16_t ui16ASFIndex;
  static uint16_t ui16ASFPage;
  static uint16_t ui16ASFDispl;
      
  static IO_BufferSizeType  incomingDataSize;
  static uint8_t            incomingDataOff;
  /* Num Blocks to acquire according to MAX_DATA_IN_BYTES_TO_ACQUIRE */
  static uint16_t NumBlocksToAcquire;
  /* Bytes to be acquired above NumBlocksToAcquire*MAX_DATA_IN_BYTES_TO_ACQUIRE */
  #if (MAX_DATA_IN_BYTES_TO_ACQUIRE>255)
  static uint16_t NumBytesRem;
  #else
  static uint8_t NumBytesRem;
  #endif /* MAX_DATA_IN_BYTES_TO_ACQUIRE */
  
  

  switch (ADMManagerState)
  {
   /*------------------------
        ADM_PREREQUISITES_STATE
     -------------------------*/
    case ADM_PREREQUISITES_STATE: 	
    	
    	/*
        Check if there's a valid S-DAT (HOW ?)
        if NOT, clear FLASH and then go to ADM_SF_PRESCAN
        ADMManagerSavedState=ADM_SF_PRESCAN;
        ADMManagerState=ADM_CLEAR_AVAILABLE_FLASH_START;
        */
    
    	bFirstElement=TRUE;
		stCurrApplData.pui8IdentAdd= (uint8_t*)0xFFFF;
		stCurrApplData.ui16CurrApplI2CAdd=0xFFFF;
		stCurrApplData.ui8CurrASFDataNum=0;
		stCurrApplData.ui8CurrIOFDataNum=0;
    	  
    	if(ADM_INIT_OPTIONS_RESET_ALL == ADM_InitOptions)
    	{
    		ADMManagerSavedState=ADM_SF_PRESCAN;
    		ADMManagerState=ADM_CLEAR_AVAILABLE_FLASH;
    	}
    	else
    	{
    		ADMManagerState=ADM_SF_PRESCAN;
    	}
      break;

    /*----------------------------
        ADM_CLEAR_AVAILABLE_FLASH
     -----------------------------*/
    case ADM_CLEAR_AVAILABLE_FLASH:

      flsErr = FLS_BlanckCheckEraseVerifyAsync(((END_AVAIBLE_FLASH-START_AVAIBLE_FLASH)+1), START_AVAIBLE_FLASH);
      if ((IO_ErrorType)IO_E_OK == flsErr)
      {
        /* go to ADMManagerSavedState*/
        ADMManagerState = ADMManagerSavedState;

      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* flag erase operation failed */
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        eADMErrorState=(ADM_ManagerErrorType)64;
      }
      else
      {
        /* wait */
      }
      break;

     /*------------------------
        ADM_SF_PRESCAN
     -------------------------*/
    case ADM_SF_PRESCAN: 
    {
      uint8_t ui8Count;
    /*
      In case of an empty file do nothing.(invalid SF ?)
      Go to ADM_IDLE waiting for resynch command!

      First step PRE_SCAN ADM SF to determine size of S-DAT.
      This avoids deciding a fixed max size of the S-DAT.
      This also allows putting data just at the end of the S-DAT (no gap)

      size of S-DAT:
      for (i=0;i<#of appl;i++)
      {
        // This is the size of the fixed part 
        // (fixed cause always present for each appliance and with se same size=10 bytes)
        sizeof APPLI2CADD      = 2 (bytes)   +
        sizeof APPL_IDENT_OFF  = 2 (bytes)   +
        sizeof APPL_ASF_OFF    = 2 (bytes)   +
        sizeof APPL_IOF_OFF    = 2 (bytes)   +
		sizeof APPL_CHK_OFF    = 2 (bytes)   +
        sizeof APPL_NEXT_OFF   = 2 (bytes)   +

      }
      + # appliance      * sizeof(pointer)   (1 IDENT_ADD pointer per appliance )
      + # static_elements * sizeof(pointer)

      Otherwise go to ADM_SF_SCAN
    */

      ui8CurrDisplIdx=0;
      
      ui8Ret=ADM_GetSDATSize((uint16_t*)&Eeprom[0], &ui16SDATSize);

      if(ui8Ret == ADMLIB_OK)
      {
        /* Init For S-DAT */
        StartFLSDest= (FLS_positionType)(START_SDAT+ ui16SDATSize);
        FlsDst = StartFLSDest;
        SDAT_CurrFixedPartPtr=(FLS_positionType)START_SDAT;
        SDAT_PrevFixedPartPtr=(FLS_positionType)START_SDAT;
        SDAT_CurrVarPartPtr=SDAT_CurrFixedPartPtr+SDAT_FIX_PART_SIZE_PERAPPL;
        ui8SDATRefIndex=0;
       
        /* Init for D-DAT */
        DDAT_CurrVarPartPtr  = DDATBaseAdd;
        ui8DDATRefIndex=0; 
        ui8DDATRefOnTimeIndex=0;

        astDDATRef[ui8DDATRefIndex].ui16I2CAdd=0xFFFF;
        
        ADMManagerState = ADM_SF_SCAN_INFO;
      }
      else
      {
        /* error condition
         Some error in setting file */
        eADMErrorState=ADM_PRESCAN_ERROR;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        break;
      }
      
      for(ui8Count=0; ui8Count<MAX_APPLS; ui8Count++) 
      {
        astSFMapTable[ui8Count].flsposSFaddress=0xFFFF;
        astSFMapTable[ui8Count].ui8I2CAdd=0xFF;
      }
    }
    break;


    /*------------------
        ADM_SF_SCAN_INFO
     ------------------*/
    case ADM_SF_SCAN_INFO:
    
      ui8Ret=ADM_GetSFDataRecordInfo((uint16_t*)&Eeprom[0], &stADMDataRecord, ui8CurrDisplIdx);

      if(ui8Ret == SF_OK)
      {
        if (stADMDataRecord.ui16ApplI2CAdd != stCurrApplData.ui16CurrApplI2CAdd)
        {
          /* New appliance I2C address encountered */

          if(!bFirstElement)
          {
            /* 
               save dummy ptr (tappo) to calculate size of last data 
               before going to get Ident table
            */
            ADMManagerState = ADM_STATIC_LAST_PTR_SAVE;
          } 
          else
          {
            /* go to get Ident table */
            bFirstElement = FALSE;
            ADMManagerState = ADM_IDENT_GET_START;
          }
        }
        else if(stADMDataRecord.ui16ApplI2CAdd != 0xFFFF)
        {
          /* go to actual scan */
          ADMManagerState = ADM_SF_SCAN;
        }
        else
        {
          /* save error flag */
          eADMErrorState=ADM_INVALID_I2C_ADD;
          ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        }
      }
      else if(ui8Ret == SF_LDR_ERR_ENDOFPOINTER)
      {
        /* go to save save dummy ptr (tappo) to calculate size of last data
         and other pointers */
        bEndJob=TRUE;
        ADMManagerState = ADM_STATIC_LAST_PTR_SAVE;
      }
      else
      {
        /* save the error flag */
        eADMErrorState=ADM_SF_PARSE_ERR;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
    break;
    
   /*------------------
        ADM_SF_SCAN
        Scan the ADM setting file
     ------------------*/
    case ADM_SF_SCAN:
      
      ui8Ret=ADM_GetSFDataRecord((uint16_t*)&Eeprom[0], &stADMDataRecord,stCurrApplData.pui8IdentAdd,ui8CurrDisplIdx);
      if(ui8Ret == SF_OK)
      {
          /* next call to ADM_GetSFDataRecord must return the next record */
          ui8CurrDisplIdx++;
          
          /* Check the different Data Types */
          switch(stADMDataRecord.ui8DataTypeID)
          {
            case DATATYPEID_ASF:
            case DATATYPEID_ASF_WHOLE_PTR:
            
              if(0 == stCurrApplData.ui8CurrASFDataNum) 
              {
                /* if it's the first ASF of appliance save CurrVar-CurrFix in *(CurrFix+ASF_OFF) */
                ui16Tmp = SDAT_CurrVarPartPtr-SDAT_CurrFixedPartPtr;
                (void)memcpy(&aui8SDATFixedPart[APPL_ASF_OFF],&ui16Tmp,OFFSET_SIZE_INBYTES);
                
                ADMManagerSavedState = ADM_SF_SCAN;
                
                /* get chk setting file value */
                ADMManagerState = ADM_STATIC_DATA_GET_ASF_CHK_PTR_START;
                ui8CurrDisplIdx--;
              }
              else
              {
            	  ADMManagerState = ADM_STATIC_DATA_GET_ASF_PTR_START;
              }
              
              stCurrApplData.ui8CurrASFDataNum++;
              
            break;
            
            case DATATYPEID_IOF:
            
              if(0==stCurrApplData.ui8CurrIOFDataNum) 
              {
                /* if it's the first IOF of appliance save CurrVar-CurrFix in *(CurrFix+IOF_OFF) */
                ui16Tmp = SDAT_CurrVarPartPtr-SDAT_CurrFixedPartPtr;
                (void)memcpy(&aui8SDATFixedPart[APPL_IOF_OFF],&ui16Tmp,OFFSET_SIZE_INBYTES);
              }
              stCurrApplData.ui8CurrIOFDataNum++;
              ADMManagerState = ADM_STATIC_DATA_GET_START;
              
            break;

            
            case DATATYPEID_LKT:
            case DATATYPEID_LKT2:
            /* Go to get LKT Address */
             ADMManagerState = ADM_DYN_GET_LKT_DATA_ADDRESS_START;
            break;
            
            case DATATYPEID_STS:
            case DATATYPEID_IOFD:
            case DATATYPEID_UDM:
            /* All info are available: simply save in D-DAT */
              ADMManagerState = ADM_UPDATE_DDAT;
            break;

            case DATATYPEID_ASF_ALL:
            
				if(0==stCurrApplData.ui8CurrASFDataNum) 
				{
					/* if it's the first ASF of appliance save CurrVar-CurrFix in *(CurrFix+ASF_OFF)*/
					ui16Tmp = SDAT_CurrVarPartPtr-SDAT_CurrFixedPartPtr;
					(void)memcpy(&aui8SDATFixedPart[APPL_ASF_OFF],&ui16Tmp,OFFSET_SIZE_INBYTES);
					
					ADMManagerSavedState = ADM_SF_SCAN;
							  
					/* get chk setting file value */
					ADMManagerState = ADM_STATIC_DATA_GET_ASF_CHK_PTR_START;
					ui8CurrDisplIdx--;
				}
				else
				{
					ADMManagerState = ADM_GET_SETTING_FILE_SIZE_START;
				}
				
				stCurrApplData.ui8CurrASFDataNum++;             
            break;

            default:
              /* save the error flag*/
              eADMErrorState=ADM_INVALID_DATATYPE;
              ADMManagerState = ADM_IDLE_STATE_JOB_KO;
            break;
          } /* end switch */
       
      }
      else if (ui8Ret == SF_LDR_ERR_INVALID_DISPL)
      {
        /* go to save save dummy ptr (tappo) to calculate size of last data
         and other pointers*/
        bEndJob=TRUE;
        ADMManagerState = ADM_STATIC_LAST_PTR_SAVE;
        
      }
      else
      {
        /* save the error flag */
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        //eADMErrorState=65;
      }
      break;

    

      
    case ADM_STATIC_DATA_GET_ASF_CHK_PTR_START:
    	/* Check if Appl supports C.R.C.*/
    if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.crcReady)  
		{
		  eReqType = ADM_REQ_RAM_DATA_CRC;
		}
		else
		{
		  eReqType = ADM_REQ_RAM_DATA_NOCRC;
		}

		/* you must use either GETW_BE or GETW_LE depending on endianess */
    if (((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.littleEndian)
		{
		   ui16EepAddr=(uint16_t) (GETW_LE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
		} 
		else 
		{
		   ui16EepAddr=(uint16_t) (GETW_BE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
		}
			
		/* Request Setting file size that is found at offset SETTING_FILE_PNTR_EOF_OFFSET
		   respect to setting file start address
		  */
		ReadRamAddresses.addr.data = (ui16EepAddr+(SETTING_FILE_PNTR_CHK_OFFSET*sizeof(SettingSizePointerType)));
		ReadRamAddresses.len = 2;
		
		ADMManagerState = ADM_STATIC_DATA_GET_ASF_CHK_PTR;
    	        
    	break;
    	
    case ADM_STATIC_DATA_GET_ASF_CHK_PTR:
    
		admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd), eReqType, &ReadRamAddresses,sizeof(PP_RRamParamType));
		if(ADM_E_OK == admErr)
		{ 
			ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
			ui16ASFPtrVal = LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff]);
			
			ReadRamAddresses.addr.data = (ui16EepAddr+(ui16ASFPtrVal*sizeof(uint16_t)));
			ReadRamAddresses.len = 2;
		  
		  /* go to get the data. From now on this will be treated as a standard static data */
			ADMManagerState = ADM_STATIC_DATA_GET_ASF_CHK_VAL;
		}
		else if(ADM_E_KO == admErr)
		{
		  //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
		  BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
		  eADMErrorState=66;
		}
		else
		{
		  /* wait for response */
		}
    	break;
    
    case ADM_STATIC_DATA_GET_ASF_CHK_VAL:
        
		admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd), eReqType, &ReadRamAddresses,sizeof(PP_RRamParamType));
		if(ADM_E_OK == admErr)
		{ 
			ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
			ui16ASFPtrVal = LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff]);		 
			
			if	(	
					(ADM_InitOptions == ADM_INIT_OPTIONS_RESET_IF_CHANGED) &&
					(ui16ASFPtrVal != *(uint16_t*)(SDAT_CurrFixedPartPtr+APPL_CHK_SF_OFF))
				)
			{
				ADM_InitOptions = ADM_INIT_OPTIONS_RESET_ALL;
				ADMManagerState = ADM_PREREQUISITES_STATE;				
			}
			else
			{
				(void)memcpy(&aui8SDATFixedPart[APPL_CHK_SF_OFF], &ui16ASFPtrVal, OFFSET_SIZE_INBYTES);
				ADMManagerState = ADMManagerSavedState;
			}
		}
		else if(ADM_E_KO == admErr)
		{
		  //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
		  BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
		  eADMErrorState=67;
		}
		else
		{
		  /* wait for response */
		}
		break;
    	
    	
    /*-------------------------------------
        ADM_DYN_GET_LKT_DATA_ADDRESS_START
      -------------------------------------*/
    case ADM_DYN_GET_LKT_DATA_ADDRESS_START:
    
      /* Check if Appl supports C.R.C.*/
      if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.crcReady)
      {
        eReqType = ADM_REQ_RAM_DATA_CRC;
      }
      else
      {
        eReqType = ADM_REQ_RAM_DATA_NOCRC;
      }
      
       /* 
         LKT address is a word (16bit)
         You must use either GETW_BE or GETW_LE depending on endianess.
         LKT address is in Aplliance Ident table that has been already get and stored locally.
       */
      if (((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.littleEndian)
      {
         ui16LktAddr=(uint16_t) (GETW_LE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_LKT_START_ADDR)));
      }
      else
      {
         ui16LktAddr=(uint16_t) (GETW_BE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_LKT_START_ADDR)));
      }
    
      /**/
      if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->LUTsize-1 >= stADMDataRecord.ui8LKTElem)
      {
    	  
      }
      else
      {
    	  stADMDataRecord.eUpdatePol=ADM_UPDATE_DISABLE;
      }
      
      /* 
         Get of word contained in LKT element offset respect to start of LKT
       */
      /* (*2) cause the pointers are words
         must use sizeof(SettingSizePointerType) or it's for other type of pointer?
         To do...
       */
      ReadRamAddresses.addr.data = (ui16LktAddr+2*(stADMDataRecord.ui8LKTElem));
      ReadRamAddresses.len = 2;
      
      ADMManagerState = ADM_DYN_GET_LKT_DATA_ADDRESS;      
            
      break;
     
     /*-----------------------------
        ADM_DYN_GET_LKT_DATA_ADDRESS
      ------------------------------*/
     case ADM_DYN_GET_LKT_DATA_ADDRESS: 
     {
      SCOPE uint16_t ui16LktElem; /* stack */
      uint8_t ui8Count;
      
      admErr = ADM_DataReq((uint8_t)stADMDataRecord.ui16ApplI2CAdd,eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
        /*LKT element is a word (16 bit)*/
        ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
        if (((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.littleEndian)
        {
          ui16LktElem = GETW_LE(&pAnswerBuffer[incomingDataOff]);
        }
        else 
        {
          ui16LktElem = GETW_BE(&pAnswerBuffer[incomingDataOff]);
        }

       /* Handle type LKT2 
          Get the LKT data position by a special search function then it is the same as LKT type */
       if(stADMDataRecord.ui8DataTypeID == DATATYPEID_LKT2) 
       {
         uint8_t  ui8Ret,ui8Position;
          
         /* Call function identified by ID SearchType passing SearchID */
         if(stADMDataRecord.ui8LKTIDSearchType >= MAX_SEARCHID_FUNCTS) 
         {
           /* Error condition - ID Search Type out of range */
           ADMManagerState = ADM_IDLE_STATE_JOB_KO;
           //eADMErrorState=67;
           break;
         }
         
         /* search for SF store address */
         ui8Count=0;
         while( (astSFMapTable[ui8Count].ui8I2CAdd !=(uint8_t)stADMDataRecord.ui16ApplI2CAdd) &&
                 (ui8Count<MAX_APPLS)  
              ) 
         {
           ui8Count++;
         }
           
         /* Found I2Caddress in map table */
         if( ui8Count<MAX_APPLS &&
             (astSFMapTable[ui8Count].flsposSFaddress != 0xFFFF)
           ) 
         {
           /* Call the ID search type function */
           ui8Ret= afpSearchTypeFuncts[stADMDataRecord.ui8LKTIDSearchType](stADMDataRecord.ui8LKTID,
                                                                          (uint16_t*) astSFMapTable[ui8Count].flsposSFaddress,
                                                                          &ui8Position);
           if(ui8Ret== ADMLIB_OK)
           {
             /* 
               Calculate data address by adding LKT offset to LKT element address 
               then UPDATE the DDAT table 
             */
             stADMDataRecord.ui32DataAddInAppl =(uint32_t)(ui16LktElem+ui8Position);
             ADMManagerState = ADM_UPDATE_DDAT;                                                                        
           } 
           else 
           {
             /* Error condition - Position not found */
             ADMManagerState = ADM_IDLE_STATE_JOB_KO;
             eADMErrorState=68;
           }
         } 
         else 
         {
           /* Error condition - Position not found */
             ADMManagerState = ADM_IDLE_STATE_JOB_KO;
             eADMErrorState=69;
         }
       } 
       else 
       {
         /* 
           Calculate data address by adding LKT offset to LKT element address 
           then UPDATE the DDAT table 
         */
         stADMDataRecord.ui32DataAddInAppl =(uint32_t)(ui16LktElem+stADMDataRecord.ui8LKTDataPos);
         //stADMDataRecord.ui32DataAddInAppl =(uint32_t)(ui16LktElem);
         ADMManagerState = ADM_UPDATE_DDAT;
       }
       
      }
      else if(ADM_E_KO == admErr)
      {
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        eADMErrorState=70;
      }
      else
      {
        /* wait for response */
      }
     }
     break;
    
     /*-----------------------------
        ADM_UPDATE_DDAT
      ------------------------------*/ 
    case ADM_UPDATE_DDAT: 
    {
       SCOPE FLS_positionType FLSAddTmp; /* stack */
       /* Save Data PTR in D-DAT*/
       
       // MEM OPTIM 
       /* save to temporary struct that will be saved in RAM */
       stDDATRecord.flsposDataPtr=(FLS_positionType)(stADMDataRecord.ui32DataAddInAppl);
     
        /* Update the DDAT reference table
           This will be used by request optimiser and Request Manager to retrieve data from
           Appliances
        */
        if(ui8DDATRefIndex<MAX_DYN_ELEM) 
        {
          astDDATRef[ui8DDATRefIndex].ui16I2CAdd=stADMDataRecord.ui16ApplI2CAdd;
          astDDATRef[ui8DDATRefIndex].DataRecPtrOff=(DDAT_CurrVarPartPtr-DDATBaseAdd);
          astDDATRef[ui8DDATRefIndex].ui16LastTS=0;
          astDDATRef[ui8DDATRefIndex].opt_bValidAddress=0;
          astDDATRef[ui8DDATRefIndex].opt_eUpdatePol=stADMDataRecord.eUpdatePol;
          astDDATRef[ui8DDATRefIndex].opt_eUpdateBy=stADMDataRecord.eUpdateWith;
          astDDATRef[ui8DDATRefIndex].ui8Tag=stADMDataRecord.ui8Tag; 
                    
        
          // MEM OPTIM
          /* save event index in case it is saved in DDAT instead of DDAT reference */
          stDDATRecord.ui8EventIdx=stADMDataRecord.ui8DataAcqEvent;
          
          
			if(stADMDataRecord.bSave2Flash) 
			{
				astDDATRef[ui8DDATRefIndex].opt_bSave2Flash=1;
			} 
			else 
			{
				astDDATRef[ui8DDATRefIndex].opt_bSave2Flash=0;
			}
          
			if(stADMDataRecord.eUpdatePol & ADM_UPDATEBY_TIME) 
			{
				ui8DDATRefOnTimeIndex++;
			}
          
			ui8DDATRefIndex++;
			astDDATRef[ui8DDATRefIndex].ui16I2CAdd=0xFFFF;
        }
        #if 0
		else 
        {
          /* astDDATRef too small for dynamic data: change MAX_DYN_ELEM accordingly */
          eADMErrorState=ADM_DDAT_SIZE_TOO_SMALL;
          ADMManagerState = ADM_IDLE_STATE_JOB_KO;
          break;
        }
        #endif
		
		// MEM OPTIM 
		/* save Data size to temporary struct that will be saved in RAM  */
		stDDATRecord.ui16DataSize=stADMDataRecord.ui16DataSize;

		// MEM OPTIM 
		/* save ACQ TIME to temporary struct that will be saved in RAM  */
		stDDATRecord.ui16AcqTime=stADMDataRecord.ui16DataAcqTime;
		
		
		// MEM OPTIM
		/* data address and MBIndex are updated once they are stored in flash.
		to be overwritten it MUST be initialized to all FF !!! 
		*/
		  stDDATRecord.flsposDataAdd = (FLS_positionType)0xFFFFFFFF;
		#ifdef REQ_OPTIM   
		  stDDATRecord.ui8MBIndex    =  0xFF;
		#endif

		// MEM OPTIM 
		/* save record to RAM buffer and increment ram buffer write pointer */
		(void)memcpy((uint8_t*)DDAT_CurrVarPartPtr,(uint8_t*)&stDDATRecord,sizeof(stDDATRecord));
		DDAT_CurrVarPartPtr += sizeof(stDDATRecord);

       ADMManagerState = ADM_SF_SCAN_INFO;
    }
    break;
   
    
    /*------------------------------
        ADM_UPDATE_FIXED_SDAT_START
        this operation is made at the end of the SF parse or at each change in I2C address
     -------------------------------*/
    case ADM_UPDATE_FIXED_SDAT_START:
    
        /*
          Update S-DAT (Ram copy of the fixed part)
        */
        SDAT_CurrFixedPartPtr = SDAT_CurrVarPartPtr;
        /* update next appliance offset of the previous appliance. */
        ui16Tmp= SDAT_CurrFixedPartPtr-SDAT_PrevFixedPartPtr;
        (void)memcpy(&aui8SDATFixedPart[APPL_NEXT_OFF],&ui16Tmp,OFFSET_SIZE_INBYTES);
        
        
       /* update previous IOF and ASF offsets in case of No data found of each category */
       if (0 == stCurrApplData.ui8CurrIOFDataNum)
       {
         /* save to ram copy the IOF Offset */
         (void)memcpy(&aui8SDATFixedPart[APPL_IOF_OFF],&aui8SDATFixedPart[APPL_NEXT_OFF],OFFSET_SIZE_INBYTES);             
       }
       
       if(0 == stCurrApplData.ui8CurrASFDataNum)
       {
         /* save to ram copy the ASF Offset */
         (void)memcpy(&aui8SDATFixedPart[APPL_ASF_OFF],&aui8SDATFixedPart[APPL_IOF_OFF],OFFSET_SIZE_INBYTES);
       }
       ADMManagerState = ADM_UPDATE_FIXED_SDAT;
       
    break;
    
    /*------------------------
      ADM_UPDATE_FIXED_SDAT
      save to flash the fixed part of SDAT
     -------------------------*/
    case ADM_UPDATE_FIXED_SDAT:
    
      flsErr = FLS_WriteAndVerifyAsync(aui8SDATFixedPart, SDAT_FIX_PART_SIZE_PERAPPL, (FLS_positionType)SDAT_PrevFixedPartPtr);
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
        if(bEndJob) 
        {
          // MEM OPTIM
          /* copy the DDAT to flash at address FlsDst and then go to IDLE */
          ADMManagerState = ADM_SAVE_DDAT_2FLASH;
        } 
        else
        {
          /* update S-DAT pointers */
          SDAT_CurrVarPartPtr += SDAT_FIX_PART_SIZE_PERAPPL;
          SDAT_PrevFixedPartPtr = SDAT_CurrFixedPartPtr;
          
          /* go to get Ident table */
          ADMManagerState = ADM_IDENT_GET_START;
        }
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+100;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }
      
    break;
    
    /*------------------------
        ADM_IDENT_GET_START
        Prepare to get Ident Table
     -------------------------*/
    case ADM_IDENT_GET_START:
     
      /*
        Update Ram copy of Fixed part of S-DAT with I2C Address and Ident Offset information
      */
      ui16Tmp=SDAT_FIX_PART_SIZE_PERAPPL;
      (void)memcpy(&aui8SDATFixedPart[APPL_I2CADDR_OFF],&(stADMDataRecord.ui16ApplI2CAdd),OFFSET_SIZE_INBYTES);
      (void)memcpy(&aui8SDATFixedPart[APPL_IDENT_OFF],&ui16Tmp,OFFSET_SIZE_INBYTES);
     
     /* Update current appliance I2C address */
      stCurrApplData.ui16CurrApplI2CAdd = stADMDataRecord.ui16ApplI2CAdd;
      
      /* init number of IOF and ASF elements to 0*/
      stCurrApplData.ui8CurrIOFDataNum=0;
      stCurrApplData.ui8CurrASFDataNum=0;
      
      eReqType = ADM_REQ_IDENT;
      ADMManagerState = ADM_IDENT_GET;
          
    break;
    
    /*------------------------------------
    ADM_IDENT_GET:
       Get Ident table of the appliance.
       Saving to flash of this data is 
       the same as any other static data
      ------------------------------------*/
    case ADM_IDENT_GET: 
    {
      
      uint8_t *pReqLen =IdentMainReqLen; /* stack */
      admErr = ADM_DataReq((uint8_t)(stCurrApplData.ui16CurrApplI2CAdd),eReqType,pReqLen,1);

      if(ADM_E_OK == admErr)
      {
        StartFLSDest = FlsDst;
        /* save the Current Ident address */
        stCurrApplData.pui8IdentAdd=(uint8_t*)FlsDst;
        
        ADMManagerState=ADM_IDENT_SAVE;
                 
      }
      else if(ADM_E_KO == admErr)
      {
         /* save error condition*/
        eADMErrorState=ADM_IDENT_GET_ERR;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait for response */
      }
    }
      break;
    
    	
    /*------------------------
        ADM_IDENT_SAVE
     -------------------------*/   
    case ADM_IDENT_SAVE:
      /* Store Data in Flash */
      ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
      
      if(ADM_InitOptions == ADM_INIT_OPTIONS_RESET_IF_CHANGED)
      {
    	  ADMDataType stData;
    	  ADM_ErrorType error;
          error = ADM_GetIdent((uint8_t) stCurrApplData.ui16CurrApplI2CAdd, &stData);
    	  flsErr = (IO_ErrorType)IO_E_WORKING;
    	  
    	  if( (ADM_E_OK != error) || 
    	      (incomingDataSize!=stData.ui16DataSize) ||
    	      (0!=memcmp(pAnswerBuffer+incomingDataOff, stData.pui8Data, incomingDataSize))
    	    )
    	  {    		  
    		  ADM_InitOptions = ADM_INIT_OPTIONS_RESET_ALL;
    		  ADMManagerState = ADM_PREREQUISITES_STATE;    		  
    	  }
    	  else
    	  {
    		  flsErr = (IO_ErrorType)IO_E_OK;	  
    	  }
    	  
      }
      else
      {
    	  flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+incomingDataOff,incomingDataSize, FlsDst);
      }
      
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
         FlsDst+=incomingDataSize;
         /* 
            save this pointer to a ram struct used to retrieve Ident info of this Appliance during SF parse.
            This is needed cause the Flash struct is not yet completed and this info can not be retrieved 
            until completion of SF parse.
         */
         stCurrApplData.pui8IdentAdd = (uint8_t*)StartFLSDest;
         
         /* Save info about the MAIN board
            I2C address, Ident table address (where locally stored) and the Appliance type 
            The appliance type is used to decode the status message 
          */
         // check: is this endianess independent ? to do...
         if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.Board == IDT_BOARD_MAIN )
          {
            stMainBoardInfo.ui8I2CAdd=(uint8_t)stCurrApplData.ui16CurrApplI2CAdd;
            stMainBoardInfo.pui8IdentAdd=stCurrApplData.pui8IdentAdd;
            stMainBoardInfo.ui8AppType = ((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.Appliance;
            stMainBoardInfo.bValidStatAdd=FALSE;
          }

         
         /* TAG for Ident is not defined: assign invalid TAG = 0xFF */
         stADMDataRecord.ui8Tag=0xFF;
         ADMManagerState = ADM_STATIC_PTR_SAVE;
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+101;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }

    break; 
           
    /*-----------------------------------------------------------------
        ADM_STATIC_DATA_GET_START
        Calculate number of blocks and remaining bytes to be requetsed.
        It sets also the address of the data to be get.
        The address is NOT calculated here but by the previous state
     ------------------------------------------------------------------*/ 
    case ADM_STATIC_DATA_GET_START:
      /*
        retrieve static data from its absolute address in appliance
      */
      /* Check if Appl supports C.R.C.*/
      if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.crcReady)
      {
        eReqType = ADM_REQ_RAM_DATA_CRC;
      }
      else
      {
        eReqType = ADM_REQ_RAM_DATA_NOCRC;
      }
      
       /* 
         StartFLSDest is the address where data will be stored.
         Here it is initialized to the first free flash address 
       */
      StartFLSDest =  FlsDst;
      
      
      /* calculate segmentation of requests */
      
      NumBlocksToAcquire = (stADMDataRecord.ui16DataSize/MAX_DATA_IN_BYTES_TO_ACQUIRE);
      NumBytesRem        = (stADMDataRecord.ui16DataSize%MAX_DATA_IN_BYTES_TO_ACQUIRE);

      if(NumBlocksToAcquire)
      {
        ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
      }
      else if (NumBytesRem)
      {
        ReadRamAddresses.len = NumBytesRem;
        NumBytesRem = 0;
      }
      else 
      {
       /* end job cause no data to read */
       ADMManagerState = ADM_STATIC_PTR_SAVE;
       break;
      }
      
      /* set address */
      ReadRamAddresses.addr.data = (uint16_t)stADMDataRecord.ui32DataAddInAppl;
      
     
      /* go to make request */
      ADMManagerState = ADM_STATIC_DATA_GET;
      break;
      
    
    /*------------------------
        ADM_STATIC_DATA_GET
        Make the requets to Communication Platform
     -------------------------*/  
    case ADM_STATIC_DATA_GET:
      
      admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
        /* Go to save data */
        ADMManagerState = ADM_STATIC_DATA_SAVE;
      }
      else if(ADM_E_KO == admErr)
      {
       /* save error condition*/
        eADMErrorState=ADM_DATAREQ_ERR;
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);

      }
      else
      {
        /* wait for response */
      }

      break;

    /*-----------------------
        ADM_STATIC_DATA_SAVE
      -----------------------*/
    case ADM_STATIC_DATA_SAVE:
      
      /* Store Data in Flash */
      ADM_BCP_GetAnswParams (pAnswerBuffer, (uint16_t*) &incomingDataSize, (uint8_t*) &incomingDataOff);
      
      if(incomingDataSize==0) 
      {
        flsErr= IO_E_OK;
      }
      else
      {
    	  if(ADM_InitOptions == ADM_INIT_OPTIONS_RESET_IF_CHANGED)
    	  {
    		  flsErr = IO_E_FAIL;
    		  if(memcmp((pAnswerBuffer+incomingDataOff), (uint8_t*)FlsDst, incomingDataSize))
    		  {
    			  ADM_InitOptions = ADM_INIT_OPTIONS_RESET_ALL;
    			  ADMManagerState = ADM_PREREQUISITES_STATE;
    		  }
    		  else
    		  {
    			  flsErr=(IO_ErrorType)IO_E_OK;
    		  }
    	  }
    	  else
    	  {
    		  flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+incomingDataOff,incomingDataSize, FlsDst);
    	  }
      }
      
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
         FlsDst+=incomingDataSize;
         
        /* calculate remaining bytes */
        if ( NumBlocksToAcquire > 1 )
        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
          NumBlocksToAcquire--;
          ADMManagerState = ADM_STATIC_DATA_GET;
        }
        else if (NumBytesRem)
        {
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = NumBytesRem;
          NumBytesRem = 0;
          ADMManagerState = ADM_STATIC_DATA_GET;
        }
        else
        {
          /* End Job : save pointer */
          ADMManagerState = ADM_STATIC_PTR_SAVE;
        }
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
       /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+102;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }
      break;

  /*-----------------------
      ADM_STATIC_PTR_SAVE
      Save address (PTR) where data is stored.
    -----------------------*/
    case ADM_STATIC_PTR_SAVE:

    	if(ADM_InitOptions == ADM_INIT_OPTIONS_RESET_IF_CHANGED)
    	{
    		flsErr = IO_E_OK;
    	}
    	else
    	{
			/*
			Save StartFLSDest at CurrVarPtr then  CurrVarPtr += sizeof(FLS_positionType) 
			Store Pointer in S-DAT 
			*/
    		flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&StartFLSDest,sizeof(FLS_positionType), SDAT_CurrVarPartPtr);
    	}
    	
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
        /* update pointers */
        astSDATRef[ui8SDATRefIndex].ui16I2CAdd   = stADMDataRecord.ui16ApplI2CAdd;
        astSDATRef[ui8SDATRefIndex].DataRecPtrOff=(SDAT_CurrVarPartPtr-START_SDAT);
        
        /* update SDAT reference table with TAG and address */
        astSDATRef[ui8SDATRefIndex].ui8Tag=stADMDataRecord.ui8Tag;
        ui8SDATRefIndex++;
        
        /* write the next address to 0xFFFF used to detect end of elements in reference table */
        astSDATRef[ui8SDATRefIndex].ui16I2CAdd   = 0xFFFF;
        
        SDAT_CurrVarPartPtr += sizeof(FLS_positionType);
        ADMManagerState = ADM_SF_SCAN_INFO;
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+103;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }

      break;
    
    /*------------------------
      ADM_STATIC_LAST_PTR_SAVE
      ------------------------*/  
    case ADM_STATIC_LAST_PTR_SAVE:
        
     /* 
       Save FLSDest at CurrVarPtr then  CurrVarPtr += sizeof(FLS_positionType) 
       Store Pointer in S-DAT.
     */
      flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&FlsDst,sizeof(FLS_positionType), SDAT_CurrVarPartPtr);
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
        /* update pointers */
        SDAT_CurrVarPartPtr += sizeof(FLS_positionType);
        ADMManagerState = ADM_UPDATE_FIXED_SDAT_START;
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* save error condition */
        eADMErrorState=ADM_FLASH_WRITE_ERR+104;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }
    break;
    

     /*---------------------------------
        ADM_GET_SETTING_FILE_SIZE_START
        Get the setting file size. 
        This is needed in case we want to read the whole setting file.
       ---------------------------------*/
    case ADM_GET_SETTING_FILE_SIZE_START:
        
        /* Check if Appl supports C.R.C.*/
        if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.crcReady)
        {
          eReqType = ADM_REQ_RAM_DATA_CRC;
        }
        else
        {
          eReqType = ADM_REQ_RAM_DATA_NOCRC;
        }

        /* you must use either GETW_BE or GETW_LE depending on endianess */
        if (((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.littleEndian)
        {
           ui16EepAddr=(uint16_t) (GETW_LE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
        } 
        else 
        {
           ui16EepAddr=(uint16_t) (GETW_BE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
        }
            
        /* Request Setting file size that is found at offset SETTING_FILE_PNTR_EOF_OFFSET
           respect to setting file start address
          */
        ReadRamAddresses.addr.data = (ui16EepAddr+(SETTING_FILE_PNTR_EOF_OFFSET*sizeof(SettingSizePointerType)));
        ReadRamAddresses.len = 2;
        
        ADMManagerState = ADM_GET_SETTING_FILE_SIZE;
     break;
    
     /*---------------------------
        ADM_GET_SETTING_FILE_SIZE
       ---------------------------*/      
    case ADM_GET_SETTING_FILE_SIZE: 
    {
      uint8_t ui8Count;
      
        admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
        if(ADM_E_OK == admErr)
        {
          uint16_t settingFileLengthInBytes; 
          ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
          settingFileLengthInBytes = (LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff])*sizeof(SettingSizePointerType));

          /* calculate segmentation of requests */
          NumBlocksToAcquire = (settingFileLengthInBytes/MAX_DATA_IN_BYTES_TO_ACQUIRE);
          NumBytesRem = (settingFileLengthInBytes%MAX_DATA_IN_BYTES_TO_ACQUIRE);

          ReadRamAddresses.addr.data = ui16EepAddr;
          
          
           /* save the current fist available flash address (FlsDst) as the address where data will be stored 
             this pointer will be saved in S-DAT */
          StartFLSDest =  FlsDst;
          
          /* Save StartFLSDest in Setting file map table */
          /* Search for free position in map table */
          ui8Count=0;
          while( (astSFMapTable[ui8Count].ui8I2CAdd !=0xFF) &&
                 (ui8Count<MAX_APPLS)  
               ) 
           {
             ui8Count++;
           }
           
            /* Found a free position to store info*/
            if(ui8Count<MAX_APPLS) 
            {
             astSFMapTable[ui8Count].flsposSFaddress=StartFLSDest;
             astSFMapTable[ui8Count].ui8I2CAdd=(uint8_t)stADMDataRecord.ui16ApplI2CAdd;
            }
           
           
           
          if (NumBlocksToAcquire)
          {
            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
          }
          else if (NumBytesRem)
          {
            ReadRamAddresses.len = NumBytesRem;
            NumBytesRem = 0;
          }
          else 
          {
            /* end job cause no data to read */
            ADMManagerState = ADM_STATIC_PTR_SAVE;
            break;
          }
          
          /* go to get the data. From now on this will be treated as a standard static data */
          ADMManagerState = ADM_STATIC_DATA_GET;
        }
        else if(ADM_E_KO == admErr)
        {
          //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
          eADMErrorState=80;
          BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        }
        else
        {
          /* wait for response */
        }
    }
    break;
    
    /*------------------------------------
        ADM_STATIC_DATA_GET_ASF_PTR_START
        Prepare to get the value of a Setting file Pointer
     -------------------------------------*/  
    case ADM_STATIC_DATA_GET_ASF_PTR_START:
    
        /* Check if Appl supports C.R.C.*/
        if(((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.crcReady)
        {
          eReqType = ADM_REQ_RAM_DATA_CRC;
        }
        else
        {
          eReqType = ADM_REQ_RAM_DATA_NOCRC;
        }

        /* you must use either GETW_BE or GETW_LE depending on endianess */
        if (((struct IdentTableStruct *)stCurrApplData.pui8IdentAdd)->header.littleEndian)
        {
          ui16EepAddr=(uint16_t) (GETW_LE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
        } 
        else 
        {
          ui16EepAddr=(uint16_t) (GETW_BE((stCurrApplData.pui8IdentAdd+IDT_OFFSETOF_EEP_START_ADDR)));
        }
        
        /* Get the value of a Setting file Pointer that is located at offset ui8ASFPointer (words)
           respect to setting file start address.
         */    
        ReadRamAddresses.addr.data = (ui16EepAddr+(stADMDataRecord.ui8ASFPointer*sizeof(SettingSizePointerType)));
        ReadRamAddresses.len = 2;
        
        ADMManagerState = ADM_STATIC_DATA_GET_ASF_PTR;
        
        break;
    
    /*------------------------------------------
        ADM_STATIC_DATA_GET_ASF_PTR
        Get the value of a Setting file Pointer
     -------------------------------------------*/      
    case ADM_STATIC_DATA_GET_ASF_PTR:
      
        admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
        if(ADM_E_OK == admErr)
        {
          /* Store the value of the setting file pointer */
          ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
          ui16ASFPtrVal = LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff]);
          ADMManagerState = ADM_STATIC_DATA_GET_NUM_EXTRAP_START;
        } 
        else if(ADM_E_KO == admErr)
        {
          /* save error condition*/
          eADMErrorState=ADM_DATAREQ_ERR;
          //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
          BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        }
        else
        {
          /* wait for response */
        }
    
    break;
    
     /*----------------------------------------------
        ADM_STATIC_DATA_GET_NUM_EXTRAP_START:
        prepare to state ADM_STATIC_DATA_GET_ASF_PTR_EXTRAP 
       ----------------------------------------------*/
    case ADM_STATIC_DATA_GET_NUM_EXTRAP_START:
    
      /* Get the number of extrapointers that are located at first byte pointed by the pointer itself */
       ReadRamAddresses.addr.data = (ui16EepAddr+(ui16ASFPtrVal*sizeof(SettingSizePointerType)));
       ReadRamAddresses.len = 2;
       
       /* The request type has already been set */
      
       ADMManagerState = ADM_STATIC_DATA_GET_NUM_EXTRAP;
       
    break;
    
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_NUM_EXTRAP:
        Get the first word pointed by pointer.
        The MSB byte is the number of extrapointers/2
       -------------------------------------------*/     
    case ADM_STATIC_DATA_GET_NUM_EXTRAP:
    
        admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
        if(ADM_E_OK == admErr)
        {
          ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
          ui16ASFNumExtraP = LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff]);
          ui16ASFNumExtraP = 2* ((ui16ASFNumExtraP & 0xFF00)>>8);
          /* 
             We decide to give a max size of the ASF portion to read 
             that is necessary to calculate the Absolute ASF Address
           */
          if(ui16ASFNumExtraP > MAX_ASF_EXTRAPOINTERS_BYTES) 
          {
            /* save error condition*/
            eADMErrorState=ADM_RAM_TOOSMALL_FOR_EXTRAP;
            ADMManagerState = ADM_IDLE_STATE_JOB_KO;
          } 
          else 
          {
            /* Go to get extrapointers */
            ADMManagerState = ADM_STATIC_DATA_GET_ASF_EXTRAPTRS_START;
          }
        } 
        else if(ADM_E_KO == admErr)
        {
            /* save error condition*/
            eADMErrorState=ADM_DATAREQ_ERR;
            //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
            BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        }
        else
        {
          /* wait for response */
        }
        
    break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_ASF_EXTRAPTRS_START:
        Prepare to get the extrapointers of a given pointer
       -------------------------------------------*/
    case ADM_STATIC_DATA_GET_ASF_EXTRAPTRS_START:
    
     /* Calculate address where are extrapointers : first adress of the pointer */
     ReadRamAddresses.addr.data = (ui16EepAddr+(ui16ASFPtrVal*sizeof(SettingSizePointerType)));
     
      /* calculate segmentation of requests */
      NumBlocksToAcquire = (ui16ASFNumExtraP/MAX_DATA_IN_BYTES_TO_ACQUIRE);
      NumBytesRem        = (ui16ASFNumExtraP%MAX_DATA_IN_BYTES_TO_ACQUIRE);

      if (NumBlocksToAcquire)
      {
        ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
      }
      else if(NumBytesRem)
      {
        ReadRamAddresses.len = NumBytesRem;
        NumBytesRem = 0;
      } 
      else 
      {
        /* error condition cause num extrapointers must be !=0 */
        /* save error condition*/
        eADMErrorState=ADM_INVALID_EXTRAP;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        break;
      }
      
     /* request type has already been set */
     ui16ASFIndex=0;
     ADMManagerState = ADM_STATIC_DATA_GET_ASF_EXTRAPTRS;
    break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_ASF_EXTRAPTRS:
        Get the extrapointers of a given pointer
     -------------------------------------------*/
    case ADM_STATIC_DATA_GET_ASF_EXTRAPTRS:
    
      admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
        /* save extrapointers */
        ADMManagerState = ADM_STATIC_DATA_SAVE_ASF_EXTRAPTRS;
      }
      else if(ADM_E_KO == admErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_DATAREQ_ERR;
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
      }
      else
      {
        /* wait for response */
      }

    break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_SAVE_ASF_EXTRAPTRS:
        Save Extrapointer of a given pointer to local array of bytes  (aui8ASFPointerData)
       -------------------------------------------*/
    case ADM_STATIC_DATA_SAVE_ASF_EXTRAPTRS:
    
      ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
      
      /* Save extrapointers to RAM buffer*/
     (void)memcpy(aui8ASFPointerData+ui16ASFIndex,pAnswerBuffer+incomingDataOff,incomingDataSize);
      
      /* Update write index */
      ui16ASFIndex += incomingDataSize;
      
      /* Calculate remaining bytes */
      if ( NumBlocksToAcquire > 1 )
      {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
        NumBlocksToAcquire--;
        ADMManagerState = ADM_STATIC_DATA_GET_ASF_EXTRAPTRS;
      }
      else if (NumBytesRem)
      {
        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        ReadRamAddresses.len = NumBytesRem;
        NumBytesRem = 0;
        ADMManagerState = ADM_STATIC_DATA_GET_ASF_EXTRAPTRS;
      }
      else
      {
        /* End Job */
        /* if ASF whole pointer has been requested additional Info are needed */
        if(stADMDataRecord.ui8DataTypeID == DATATYPEID_ASF_WHOLE_PTR)
        {
          /* Need to get the value of first displacement */
          ADMManagerState = ADM_STATIC_DATA_GET_FIRST_DISP_START;
        } 
        else 
        {
          ADMManagerState = ADM_STATIC_DATA_CALC_ASF_DISPL_PAGE;
        }
        
      }
         
    break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_FIRST_DISP_START:
        prepare to Get the value of displacement 0
       -------------------------------------------*/
    case ADM_STATIC_DATA_GET_FIRST_DISP_START:
      
       /* Get the value of first displacement */
       ReadRamAddresses.addr.data = (  ui16EepAddr +
                                      (ui16ASFPtrVal*sizeof(SettingSizePointerType)) +
                                       ui16ASFNumExtraP
                                     );
       ReadRamAddresses.len = 1;
       
       /* the request type has already been set */
       ADMManagerState = ADM_STATIC_DATA_GET_FIRST_DISP;
      
    break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_FIRST_DISP:
        Get the value of displacement 0 that is used to calculate the
        index of last displacement.
     -------------------------------------------*/
    case ADM_STATIC_DATA_GET_FIRST_DISP:
    
      /* the first displacement is always at page 0 ! */
      admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
        SCOPE uint8_t ui8ASFDispl;    /* stack */
        SCOPE uint16_t ui16NumDispls; /* stack */
        
        ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
        /* Get data of the Appliance */
        ui8ASFDispl  = pAnswerBuffer[incomingDataOff];
        
        /* Num displacements: -1 cause the last one is the dummy one */
        ui16NumDispls = (2*ui8ASFDispl)-1;
        
        /* index of last displacement is the Number of displ -1: first displ is index 0 */
        stADMDataRecord.ui8ASFDispl = ui16NumDispls-1;
        
        /* go to get the page of thsi displacement */
        ADMManagerState = ADM_STATIC_DATA_CALC_ASF_DISPL_PAGE;
      }  
      else if(ADM_E_KO == admErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_DATAREQ_ERR;
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
      }
      else
      {
        /* wait for response */
      }
      
    break;
    
    /*------------------------------------------
        ADM_STATIC_DATA_CALC_ASF_DISPL_PAGE:
      Get the page value related to the given displacement
     -------------------------------------------*/
    case ADM_STATIC_DATA_CALC_ASF_DISPL_PAGE:
    
     /* Get Page for Displ*/
     (void)SFBasicLoaderGetPage((uint16_t*) aui8ASFPointerData, stADMDataRecord.ui8ASFDispl,&ui16ASFPage);
     ADMManagerState = ADM_STATIC_DATA_GET_ASF_DISPL_START;
    break;
    
    
    /*------------------------------------------
        ADM_STATIC_DATA_GET_ASF_DISPL_START:
      Prepare to get the Displacement value
     -------------------------------------------*/
    case ADM_STATIC_DATA_GET_ASF_DISPL_START:
    
      /* Get Displacement */
       ReadRamAddresses.addr.data = (  ui16EepAddr +
                                      (ui16ASFPtrVal*sizeof(SettingSizePointerType)) +
                                       ui16ASFNumExtraP +
                                       stADMDataRecord.ui8ASFDispl
                                     );
       ReadRamAddresses.len = 2;
       
       /* the request type has already been set */
      
       ADMManagerState = ADM_STATIC_DATA_GET_ASF_DISPL;
     break;
    
     /*------------------------------------------
        ADM_STATIC_DATA_GET_ASF_DISPL:
      Get the Displacement value
     -------------------------------------------*/ 
    case  ADM_STATIC_DATA_GET_ASF_DISPL: 
    {
      SCOPE Loader_Struct stLoaderData; /* stack */
      
      admErr = ADM_DataReq((uint8_t)(stADMDataRecord.ui16ApplI2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
        ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
        /* Get data of the Appliance */
        ui16ASFDispl = LIB_ConvTwoByteInWord(&pAnswerBuffer[incomingDataOff]);
        
        /* calc address */
        ui8Ret=SFBasicLoaderGetDisplOffset(ui16ASFDispl,ui16ASFPage,&stLoaderData);
        if(SF_OK == ui8Ret) 
        {
          
          if(stADMDataRecord.ui8DataTypeID == DATATYPEID_ASF_WHOLE_PTR) 
          {
            /*
              If the WHOLE pointer was requested, the address is ui16ASFPtrVal and the size is calculated as 
              last displacement offset + its size + numextrapointers.
            */
            stADMDataRecord.ui32DataAddInAppl= ui16EepAddr+(ui16ASFPtrVal*sizeof(SettingSizePointerType));
            stADMDataRecord.ui16DataSize     = (FLS_positionType)stLoaderData.Data + 
                                               ui16ASFNumExtraP +
                                               2*(stLoaderData.DataLenght) ;
          } 
          else /* otherwise */
          {
            
            /* save abs address and size and go to get data */
            stADMDataRecord.ui32DataAddInAppl=((FLS_positionType)stLoaderData.Data + 
                                                        ui16EepAddr + 
                                                        ui16ASFPtrVal*sizeof(SettingSizePointerType) +
                                                        ui16ASFNumExtraP );
            
            /* Size returned in stLoaderData in in words*/                                            
            stADMDataRecord.ui16DataSize=2*(stLoaderData.DataLenght);
          }
         
           /* Go to get data */
          ADMManagerState = ADM_STATIC_DATA_GET_START;     
        } 
        else 
        {
          /* save error condition */
          eADMErrorState=ADM_DISPL_CALC_ERR;
          ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        }
      }
      else if(ADM_E_KO == admErr)
      {
        /* save error condition */
          eADMErrorState=ADM_DATAREQ_ERR;
        //ADMManagerState = ADM_IDLE_STATE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
      }
      else
      {
        /* wait for response */
      }
    }
    break;
    
     /*-----------------------
        ADM_SAVE_DDAT_2FLASH
        now DDAT is almost completed and can be saved to flash.
        There are some fields that will be filled by Request Manager that was left 0xFF
        to allow write without erase.
      -----------------------*/           
    case ADM_SAVE_DDAT_2FLASH:
    {
      uint16_t ui16Size;
      
      ui16Size = (DDAT_CurrVarPartPtr-DDATBaseAdd);
      
      if(ui16Size==0 || (ADM_InitOptions == ADM_INIT_OPTIONS_RESET_IF_CHANGED)) 
      {
        flsErr= IO_E_OK;
      }
      else
      {
        flsErr = FLS_WriteAndVerifyAsync((uint8_t*)DDATBaseAdd,ui16Size, FlsDst);
      }
      
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
        /* set the new value for DDAT base address that from now on is in FLASH */
        DDATBaseAdd = FlsDst;
        /* update flash write pointer */
        FlsDst += ui16Size;
        /* go to IDLE */
        ADMManagerState = ADM_IDLE; 
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
        /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+105;
        ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      }
      else
      {
        /* wait */
      }
    }
      
    break;
    
      /*-----------------------
        ADM_IDLE
      -----------------------*/               
      case ADM_IDLE:
      /*
           ADM_IDLE:
              wait requets for resynch.
      */
      break;

      /*-----------------------
        ADM_IDLE_STATE_JOB_KO
      -----------------------*/      
    case ADM_IDLE_STATE_JOB_KO:
      break;

    default:
       // unexpected state
       ADMManagerState = ADM_IDLE_STATE_JOB_KO;
      break;
  }
  
  if(ADMManagerState == ADM_IDLE) 
  {
    return ADM_E_OK;
  }
  else if(ADMManagerState == ADM_IDLE_STATE_JOB_KO) 
  {
    return ADM_E_KO;
  }
  else 
  {
    return ADM_E_WORKING;
  }

}




/*===========================================================================
  REQUEST MANAGER
===========================================================================*/



typedef enum 
{
  /** Idle state */
  ADM_REQMAN_IDLE=0,
  /** First step */
  ADM_REQMAN_START,
  /** This state assings ram position to dynamic data according 
      to the order they are declared in ADM setting file */
  ADM_REQMAN_ASSIGN_RAM_POS,
  /** Check if timer of single data is expired */
  ADM_REQMAN_CHECK_TMO,
  /** Check if timer of Macro Block is expired */
  ADM_MBREQMAN_CHECK_TMO,
  /** sort the DDAT reference ascending for address in applliance*/
  ADM_REQMAN_SORT_DDAT,
  /** build Macro block table*/
  ADM_REQMAN_BUILD_MBTABLE,
  /** Get MB from appliance */
  ADM_REQMAN_MBREQDATA_START,
  ADM_MBREQMAN_REQDATA,
  /** Save Macro block to RAM */
  ADM_MBREQMAN_SAVEDATA,
  /** Get data from appliance */
  ADM_REQMAN_REQDATA_START,
  ADM_REQMAN_REQDATA,
  /** Save data to RAM */
  ADM_REQMAN_SAVEDATA,
  /** Save data to flash in case it is specified */
  ADM_REQMAN_SAVEDATA_2FLASH,
  /** Update address in DDAT */
  ADM_REQMAN_UPDATE_ADD,
  ADM_MBREQMAN_UPDATE_ADD,
  /** Go to next var */
  ADM_REQMAN_STEPNEXT,
  ADM_MBREQMAN_STEPNEXT,
  /** Error state */
  ADM_REQMAN_IDLE_JOB_KO
  
} ADMReqManagStateType;

typedef enum 
{
  ADM_SAVE2FLASH_ERASE=0,
  ADM_SAVE2FLASH_WRITE,
} ADMSaveToFlashStateType;


/** For each event you have the old value and the new one */
typedef struct 
{
  uint8_t ui8Old;
  uint8_t ui8New;
} EventFlagsType;



/** Main Request manager state */
static ADMReqManagStateType ADMReqManState;

/** There is a nested state machine used for saving data to flash */
static ADMSaveToFlashStateType eSave2FlashState;



static uint16_t ui16SavedDynDataIdx;
static uint16_t ui16CurrI2CAdd;
static uint16_t ui16CurrSaveIdx;
FLS_positionType ADM_RMFlsDst;



/** Array where events are stored.
\n    For each event you have the old value and the new one. The new one is set to 0xFF in case 
    the event is present and the old value was 0 (False).
\n    The value 0xFF allow different users (each has its own bit position) to use the same event.
\n    A new event overrides the previous.
 */
EventFlagsType aucEvents[MAX_NUM_EVENTS_HANDLED]=
{ {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; 


/**
This function implements the Bubble Sort of DDATreference elements belonging to the same appliance I2C address.
the sort is made ascending on data memory addresses (DATAPTR).
Request optimizer need sorted elements to work! See documentation for details.

\param  [in] 	  DDATReferenceType* astArray: pointer to an array of DDATReferenceType elements.
\param  [in]    ui8Nelem: number of elements to be sorted starting from astArray[0]. 
\param  [out]   none

\return 	void


\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0
   
\note		\par \b Note 1: 
			documentation by Andrea.Mantovani

*/
void DDATRefSort(DDATReferenceType* astArray, uint8_t ui8Nelem)
{
  DDATReferenceType DDATRecTmp;
  uint8_t i;
  SettingSizePointerType ssptTmp1,ssptTmp2;
  
  /* in N-1 iteration array is sorted */ 
  while (ui8Nelem > 0) 
  { 
    /* as at each iteration at least the greatest elements goes to its final position we
       can order the last N-1 elements */
    for(i=0;i<(ui8Nelem-1);i++)
    {
      
      (void)memcpy(&ssptTmp1,((uint8_t*)(DDATBaseAdd+astArray[i].DataRecPtrOff+DATAPTR_OFFSET)),sizeof(SettingSizePointerType));
      (void)memcpy(&ssptTmp2,((uint8_t*)(DDATBaseAdd+astArray[i+1].DataRecPtrOff+DATAPTR_OFFSET)),sizeof(SettingSizePointerType));
      
     
      /* If you change '>' with '<' you'll get descending order */
      if(ssptTmp1>ssptTmp2)  
      { 
        /* swap elements */      
        DDATRecTmp    = astArray[i];
        astArray[i]   = astArray[i+1]; 
        astArray[i+1] = DDATRecTmp;
      }
    }
    ui8Nelem--;
  }
}

       
/**
This function implements the state machine for the ADM Request Manager 
It must be called in main after ADM_Manager has completed its job.
It scans the Table of reference to D-DAT and performs request to BCP to get the Dynamic Data that is stored in RAM to a reserved area (aui8DynamicData)
It also implements the Request Optimizer (only for data that is get on time basis).
Data get on Event based is handled without Request Optimizer.
It updates update-ID once all dynamic data (periodic once) are refreshed in the dynamic area   

It must be called inside a main loop

\param  [in] 	  none
\param  [out]   

\return 	ADM_ErrorType  

\retval	ADM_E_OK      Job is done
\retval	ADM_E_WORKING	Job is ongoing
\retval	ADM_E_KO	    Job failed.

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_RequestManager(void)
{

  SCOPE ADMLIB_ErrorType eRet;/* stack */
  SCOPE ADM_ErrorType admErr; /* stack */
  SCOPE uint8_t* pAnswerBuffer = I2CAnswerBuffer;
  SCOPE uint8_t* pAddr;      /* stack */
  SCOPE IO_ErrorType flsErr; /* stack */ 
  
  static ADM_TracksIndexType ui16RefIdx;
  static uint16_t ui16MBIndex;
  static ADMLibDataType stDataTmp; /* move to stack */
  static ADMRequestType eReqType;
  static IO_BufferSizeType  incomingDataSize;
  static uint8_t            incomingDataOff;
  /* Num Blocks to acquire according to MAX_DATA_IN_BYTES_TO_ACQUIRE */
  static uint16_t NumBlocksToAcquire;
  /* Bytes to be acquired above NumBlocksToAcquire*MAX_DATA_IN_BYTES_TO_ACQUIRE */
  #if (MAX_DATA_IN_BYTES_TO_ACQUIRE>255)
  static uint16_t NumBytesRem;
  #else
  static uint8_t NumBytesRem;
  #endif /* MAX_DATA_IN_BYTES_TO_ACQUIRE */
  static uint16_t ui16Tmp;
  static uint16_t ui16DataSize;
  static uint8_t ui8NumBlocks;
  static uint8_t ui8Count=0;
  static uint16_t ui16MBCurrI2CAdd;
  
  //just for debug
  uint8_t ui8err;
  static uint8_t dbg1 = 0;
  
  switch(ADMReqManState) 
  {
    case ADM_REQMAN_START:
    
#ifdef REQ_OPTIM
    /* if Req Optimizer is enabled.*/
    ui16MBIndex=0;
    ui16MBCurrI2CAdd =0xFFFF;
    ADMReqManState = ADM_REQMAN_SORT_DDAT ;
#else
    ADMReqManState = ADM_REQMAN_ASSIGN_RAM_POS ;
#endif
    /* in any case these are used */
     ui16RefIdx=0;
     ui16CurrI2CAdd=0xFFFF;
     
     /* Init last update */
     ADM_TracksUpdatedByTmoCount=0;
    break;
    
    /*-------------------------------------------
        ADM_REQMAN_SORT_DDAT
        First of all sort DDAT reference elements
      -------------------------------------------*/ 
    case ADM_REQMAN_SORT_DDAT:
    
#ifdef REQ_OPTIM      
      {
        uint8_t aucNumElem[MAX_APPLS];
        uint8_t ui8numElem=0;
        uint8_t applID=0;
        uint16_t ui16RefIdx=0;
        uint8_t ui8CurrI2cAdd =(uint8_t)astDDATRef[ui16RefIdx].ui16I2CAdd;
        
        /* get num of elements for each appliance I2C address and save to local array 
           These info are used by the sort 
         */
        while(astDDATRef[ui16RefIdx].ui16I2CAdd != 0xFFFF)
        {
          while (ui8CurrI2cAdd == (uint8_t)(astDDATRef[ui16RefIdx].ui16I2CAdd))
          {  
            ui8numElem++;
            ui16RefIdx++;
          }
          ui8CurrI2cAdd = (uint8_t)astDDATRef[ui16RefIdx].ui16I2CAdd;
          aucNumElem[applID] = ui8numElem;
          applID++;
          ui8numElem=0;
        }
        
        /*
          Apply sort among elements of each appliance I2C address 
          The sort is made on DDAT reference and not directly on DDAT that now is in flash.
         */
        applID=0;
        ui16RefIdx=0;
        /* For eack Appliance I2C address ....*/
        while(astDDATRef[ui16RefIdx].ui16I2CAdd != 0xFFFF)
        {
          /* ... apply sort*/
          DDATRefSort(&astDDATRef[ui16RefIdx], aucNumElem[applID]);
          ui16RefIdx += aucNumElem[applID];
          applID++;
        }
        
        /* once the DDAT reference is sorted we can build the macro block table appliang the algorithm
           described in ADM document
         */
        ADMReqManState=ADM_REQMAN_BUILD_MBTABLE;
      }

 #endif     
    break;
    
     /*--------------------------
        ADM_REQMAN_BUILD_MBTABLE
      --------------------------*/ 
    case ADM_REQMAN_BUILD_MBTABLE:
    
 #ifdef REQ_OPTIM   
        /*
          Here we have to build a table describing macro blocks (see ADM documentation).
          We have to decide if a dynamic var belongs to a certain block or not.
          If it does we save the offset of Start Address (SA) of this var respect to MB StartAddress (SAMB).
          When the MB will be get from appliance ad saved to local RAM at address X, the address where the var is sored is 
          X+offset.
          
          N.B.: in DDAT are stored also dynamic data that are saved to flash and updated on events (IOFD)
          these data must be skipped in building Macro Blocks. 
        */
        {
        
        /* 
          "i" is the dynamic variable index inside DDAT 
          "j" is the index of macro block
        */
        uint8_t i=0,j=0;
        uint16_t ui16Tmp1,ui16Tmp2,ui16Tmp3,ui16Tmp4,ui16Tmp5,ui16GAP;
        // uint8_t ui8CurrI2cAdd = astDDATRef[i].ui16I2CAdd;
        bool_t bSameBlock=TRUE;
        SettingSizePointerType AddTmp;
        IO_ErrorType flsErr;
        
        /* while there are no more dynamic data... */
          while(astDDATRef[i].ui16I2CAdd != 0xFFFF)
          {
             /* Skip dynamic data saved to flash */
             if( (astDDATRef[i].opt_eUpdatePol == ADM_UPDATEBY_EVENT) ||
                 (astDDATRef[i].opt_eUpdateBy == ADM_UPDATEWITH_STATREQ)
               )
             {
              i++;
              continue;
             }
             
             /* Get Start Address of var i from DDAT using DDATref: SAi...*/
             (void)memcpy( &ui16Tmp1,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAPTR_OFFSET)),
                     sizeof(SettingSizePointerType));
             
              /* save this ad start address of current MB : SAMBj = SAi */
             astMBTable[j].ui16MBStartAdd = ui16Tmp1;
             astMBTable[j].ui8MBI2CAdd = (uint8_t)astDDATRef[i].ui16I2CAdd;       
             
             /* Get len : Li */
             (void)memcpy( &ui16Tmp2,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATASIZE_OFFSET)),
                     DDAT_SIZEOF_DATASIZE);
                                    
             /* Save end address of current var as End address of current MB :EAMBj = EAi */
             astMBTable[j].ui16MBEndAdd = ui16Tmp1+ui16Tmp2;
             
             /* Offset(i) =0 
                the first element of MB has always offset 0 */
             AddTmp = (SettingSizePointerType)0;
             
             // MEM OPTIM
             /*
                 memcpy((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAADD_OFFSET),
                       &AddTmp,
                       DDAT_SIZEOF_DATAADD);
              */         
              
             flsErr =IO_E_WORKING;
             do
             {
              flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&AddTmp,
                                               DDAT_SIZEOF_DATAADD,
                                               (DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAADD_OFFSET));
             }
             while (flsErr ==IO_E_WORKING);
                 
             if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
             {
               // save error condition
               eADMErrorState=ADM_FLASH_WRITE_ERR+106;
               ADMManagerState = ADM_IDLE_STATE_JOB_KO;
               /* exit here */
               return ADM_E_KO;
             }
                       
             /* T(i) */
             (void)memcpy( &ui16Tmp5,
                 ((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAACQTIME_OFFSET)),
                 DDAT_SIZEOF_ACQTIME);
            
            /* Save acq time of element i as acq time for MB. The MB acq time will be
               updated with the smallest among those of data belonging to MB
             */                                
            astMBTable[j].ui16MBAcqTime = ui16Tmp5;

            /* Init valid address to false */            
            astMBTable[j].bValidAddress = FALSE;
            
            /* Save block index to DDATref struct . 
               This tells which block the data belongs to
            */
            
             // MEM OPTIM
             /* save MBIndex to DDAT */
             flsErr =IO_E_WORKING;
             do
             {
              flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&j,
                                               DDAT_SIZEOF_MBIDX,
                                               (DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAMBIDX_OFFSET));
             }
             while (flsErr ==IO_E_WORKING);
                 
             if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
             {
               /* save error condition*/
               eADMErrorState=ADM_FLASH_WRITE_ERR+107;
               ADMManagerState = ADM_IDLE_STATE_JOB_KO;
               /* exit here */
               return ADM_E_KO;
             }
             
            
            /* 
               This while loop appends dynamic data to the current MB if the condition is met
               otherwise it will be the first element of the new block.
             */
            bSameBlock=TRUE;
            while ((astDDATRef[i].ui16I2CAdd == astDDATRef[i+1].ui16I2CAdd) &&
                   (bSameBlock==TRUE) )
            { 
               /* Skip dynamic data that is updated on events */
               if( (astDDATRef[i].opt_eUpdatePol == ADM_UPDATEBY_EVENT) ||
                   (astDDATRef[i].opt_eUpdateBy == ADM_UPDATEWITH_STATREQ)
                 )
               {
                i++;
                continue;
               }
               
               /* SA(i+1) */
               (void)memcpy( &ui16Tmp1,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATAPTR_OFFSET)),
                     sizeof(SettingSizePointerType));
               /* SA(i) */
               (void)memcpy( &ui16Tmp2,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATAPTR_OFFSET)),
                     sizeof(SettingSizePointerType));
               /* L(i)  */
               (void)memcpy( &ui16Tmp3,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i].DataRecPtrOff+DATASIZE_OFFSET)),
                     DDAT_SIZEOF_DATASIZE);
               /* L(i+1) */
               (void)memcpy( &ui16Tmp4,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATASIZE_OFFSET)),
                     DDAT_SIZEOF_DATASIZE);
              
                          
               /* GAP(i-1)=SAi-EA(i-1) = SAi-SA(i-1)-L(i-1) */
               ui16GAP= ui16Tmp1-ui16Tmp2-ui16Tmp3;
              
              /*
                This is the condition that a dyn data must met to belong to the current MB.
                See document for details.
              
                if ( (EAi-SAMBj<N_MAX_BYTES)  &&
                   ( ( GAP(i-1) < MAXGAP ) || (Li>GAP(i-1))  )
                 )
               */
               if( ((ui16Tmp1+ui16Tmp4-astMBTable[j].ui16MBStartAdd) < MAX_DATA_IN_BYTES_TO_ACQUIRE) && (ui16GAP < MAXGAP || ui16GAP < ui16Tmp4) )
               {
                 /* EAMBj=EAi */
                 astMBTable[j].ui16MBEndAdd = ui16Tmp1+ui16Tmp4;
                 
                 /* 
                    We have to save the offset of SA respect to block start address.( SA(i+1)-SAMBj=offset)
                    The sum of this offset with the RAM address where the MB is stored will give the RAM address where
                    variable is stored.
                  */ 
                     
                 AddTmp = (SettingSizePointerType)(ui16Tmp1-astMBTable[j].ui16MBStartAdd);
                 
                 // MEM OPTIM
                 
                 /*
                 memcpy((uint8_t*)(DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATAADD_OFFSET),
                       &AddTmp,
                       DDAT_SIZEOF_DATAADD);
                  */
                 
                 /* Store data to DDAT that is in Flash */ 
                 flsErr =IO_E_WORKING;
                 do
                 {
                  flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&AddTmp,
                                                   DDAT_SIZEOF_DATAADD,
                                                   (DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATAADD_OFFSET));
                 }
                 while (flsErr ==IO_E_WORKING);
                     
                 if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
                 {
                   /* save error condition */
                   eADMErrorState=ADM_FLASH_WRITE_ERR+108;
                   ADMManagerState = ADM_IDLE_STATE_JOB_KO;
                   /* exit here */
                   return ADM_E_KO;
                 }
                 
                  
                 /* T(i+1) */
                 (void)memcpy( &ui16Tmp5,
                     ((uint8_t*)(DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATAACQTIME_OFFSET)),
                     DDAT_SIZEOF_ACQTIME);
                                  
                 /* if(Ti<TMBj) 
                 The MB acq time is updated with the smallest among those of data belonging to MB 
                 */
            
                 /* acquisition time =0 means never !*/    
                 if(ui16Tmp5>0) 
                 {
                   if(ui16Tmp5 < astMBTable[j].ui16MBAcqTime) 
                   {
                     /* TMBj=Ti; */
                     astMBTable[j].ui16MBAcqTime=ui16Tmp5;
                   }
                 }
                 
                  // MEM OPTIM
                  /* save Macro Block index (MBIndex) to DDAT that is in flash */ 
                  
                   flsErr =IO_E_WORKING;
                   do
                   {
                      flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&j,
                                                        DDAT_SIZEOF_MBIDX,
                                                        (DDATBaseAdd+astDDATRef[i+1].DataRecPtrOff+DATAMBIDX_OFFSET));
                   }
                   while (flsErr ==IO_E_WORKING);
                       
                   if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
                   {
                     // save error condition
                     eADMErrorState=ADM_FLASH_WRITE_ERR+109;
                     ADMManagerState = ADM_IDLE_STATE_JOB_KO;
                     /* exit here */
                     return ADM_E_KO;
                   }
                   
                 /* Go to next data */
                 i++;
               } 
               else 
               {
                 bSameBlock=FALSE;
               }
            }
            /* Go to next data and next Macro Block */
            i++;
            j++;
          }
          
          /* save total number of blocks */
          ui8NumBlocks =j;
          ADMReqManState=ADM_MBREQMAN_CHECK_TMO;
        }
#endif        
    break;
    
   /*-------------------------------------------------------
     ADM_MBREQMAN_CHECK_TMO
     This state check both and alternatively if the acquisition time 
     of each Macro Block (MB) has expired 
     and if a variable updated upon event must be updated.     
    ------------------------------------------------------*/
    case ADM_MBREQMAN_CHECK_TMO: 
    {
      
#ifdef REQ_OPTIM
      uint16_t ui16Tmp;
      
      if((ui8Count & 0x1) == 0) 
      {
        /* If is a status request */
        if (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_STATREQ) 
        {
           /* Get ReqTime */
           (void)memcpy((uint8_t*)&(ui16Tmp),
                (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAACQTIME_OFFSET),
                 DDAT_SIZEOF_ACQTIME
               );
           
           /* acquisition time =0 means never !*/    
           if(ui16Tmp>0) 
           {
             if((g_ui16Currtime-astDDATRef[ui16RefIdx].ui16LastTS) > ui16Tmp) 
             {
               ADMReqManState=ADM_REQMAN_REQDATA_START;
             }
           }
        } 
        else /* supposed RAM request */
        {
          /* on EVEN values of ui8Count */
          /* check MB acq time expiration */
           
           /* acquisition time =0 means never !*/    
           if(astMBTable[ui16MBIndex].ui16MBAcqTime>0) 
           {     
              if((g_ui16Currtime-astMBTable[ui16MBIndex].ui16MBLastTimeStamp) > astMBTable[ui16MBIndex].ui16MBAcqTime) 
              {
                ADMReqManState=ADM_REQMAN_MBREQDATA_START;
              }
           }
        }
      } 
      else 
      {
        /* on ODD values of ui8Count */
        /* Check of data that is updated on events */
        if ( (astDDATRef[ui16RefIdx].opt_eUpdatePol==ADM_UPDATEBY_EVENT) &&
             (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ)
           )
        {
          uint8_t  ui8EvIdx;
          
          // MEM OPTIM
          /* get the event index */
           (void)memcpy((uint8_t*)&(ui8EvIdx),
            (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAEVIDX_OFFSET),
             DDAT_SIZEOF_EVIDX
           );
          
          /* This check should be modified: this module should check and clear ONLY the bit dedicated to it to do...*/
          if(aucEvents[ui8EvIdx].ui8New) 
          {
            /* clear event */
            aucEvents[ui8EvIdx].ui8New=0;
            ADMReqManState=ADM_REQMAN_REQDATA_START;
          } 
          else 
          {
            ADMReqManState=ADM_REQMAN_STEPNEXT;
          }
        }
        else 
        {
          ADMReqManState=ADM_REQMAN_STEPNEXT;
        } 
       
      }
      ui8Count++;  
#endif
    }
    break;
    
     
   /*--------------------------
      ADM_REQMAN_MBREQDATA_START
    --------------------------*/
    case ADM_REQMAN_MBREQDATA_START:
    
#ifdef REQ_OPTIM      
      if(ui16MBCurrI2CAdd != astMBTable[ui16MBIndex].ui8MBI2CAdd) 
      {
        eRet=(ADMLIB_ErrorType)ADM_GetApplIdentAdd(astMBTable[ui16MBIndex].ui8MBI2CAdd, &stDataTmp);
        if(ADMLIB_OK == eRet) 
        {
          ui16MBCurrI2CAdd = astMBTable[ui16MBIndex].ui8MBI2CAdd;
          /* Check if Appl supports C.R.C.*/
          if(((struct IdentTableStruct *)stDataTmp.pui8Data)->crcReady)
          {
            eReqType = ADM_REQ_RAM_DATA_CRC;
          }
          else
          {
            eReqType = ADM_REQ_RAM_DATA_NOCRC;
          }
        } else 
        {
          ADMReqManState = ADM_REQMAN_IDLE_JOB_KO;
          eADMErrorState=ADM_DATAREQ_ERR+153;
          ui8err=1;
          break;
        }
      }
      
      /*
         If it's the first time this data is retrieved the address where data is stored is invalid
         Data is saved at the watermark of the RAM region where all dyn data are stored.
         At the end this watermark will be saved as data address.
       */
     
      if(FALSE == astMBTable[ui16MBIndex].bValidAddress) 
      {   
        ui16SavedDynDataIdx = ui16DynDataWaterMark;
      }
      else
      {
        ui16CurrSaveIdx=0;
      }
     
     
      /*
        Retrieve data PTR (Data address in appliance) and data size
      */
     
      ReadRamAddresses.addr.data = astMBTable[ui16MBIndex].ui16MBStartAdd;
      ui16DataSize=astMBTable[ui16MBIndex].ui16MBEndAdd-astMBTable[ui16MBIndex].ui16MBStartAdd;      
      
      /* calculate segmentation of request */     
      NumBlocksToAcquire = (ui16DataSize/MAX_DATA_IN_BYTES_TO_ACQUIRE);
      NumBytesRem        = (ui16DataSize%MAX_DATA_IN_BYTES_TO_ACQUIRE);

      if (NumBlocksToAcquire)
      {
        ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
      }
      else if (NumBytesRem)
      {
        ReadRamAddresses.len = NumBytesRem;
        NumBytesRem = 0;
      } 
      else 
      {
         /* data size is 0 :End Job */
          ui16Tmp = ui16SavedDynDataIdx+(uint16_t)aui8DynamicData;
          ADMReqManState = ADM_REQMAN_UPDATE_ADD;
        break;
      }
      
      ADMReqManState = ADM_MBREQMAN_REQDATA;  
#endif      
     break;
    
     /*-----------------------
        ADM_MBREQMAN_REQDATA
      -----------------------*/
    case ADM_MBREQMAN_REQDATA:
    
#ifdef REQ_OPTIM
    
     admErr = ADM_DataReq(astMBTable[ui16MBIndex].ui8MBI2CAdd,eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      if(ADM_E_OK == admErr)
      {
         ADMReqManState = ADM_MBREQMAN_SAVEDATA;
      }
      else if(ADM_E_KO == admErr)
      {
       /* save error condition*/
        //eADMErrorState=ADM_DATAREQ_ERR;
    	 eADMErrorState=ADM_DATAREQ_ERR+152;
        //ADMReqManState = ADM_REQMAN_IDLE_JOB_KO;
        BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);
        ui8err=2;
      }
      else
      {
        /* wait for response */
      }
#endif
    break;
    
     /*-----------------------
        ADM_MBREQMAN_SAVEDATA
      -----------------------*/
    case ADM_MBREQMAN_SAVEDATA:

#ifdef REQ_OPTIM    
    ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
      if(FALSE == astMBTable[ui16MBIndex].bValidAddress) 
      {
          /* if this data doesn't have an address yet I put it at first available ram address (watermark)...*/
          pAddr = (uint8_t*)(aui8DynamicData+ui16DynDataWaterMark);
          (void)memcpy(pAddr,pAnswerBuffer+incomingDataOff,incomingDataSize);
          ui16DynDataWaterMark += incomingDataSize;
      } 
      else 
      {
      /* ...otherwise the address is stored in MB table */
          
          pAddr  = (uint8_t*)astMBTable[ui16MBIndex].ui16RamAddress;
          pAddr += ui16CurrSaveIdx;     
          (void)memcpy(pAddr,pAnswerBuffer+incomingDataOff,incomingDataSize);
          ui16CurrSaveIdx += incomingDataSize;
      }
      
      /* calculate remaining bytes */
      if ( NumBlocksToAcquire > 1 )
      {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
        NumBlocksToAcquire--;
        ADMReqManState = ADM_MBREQMAN_REQDATA;
      }
      else if (NumBytesRem)
      {
        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        ReadRamAddresses.len = NumBytesRem;
        NumBytesRem = 0;
        ADMReqManState = ADM_MBREQMAN_REQDATA;
      }
      else
      {
        /* End Job */
        ui16Tmp = ui16SavedDynDataIdx+(uint16_t)aui8DynamicData;
        ADMReqManState = ADM_MBREQMAN_UPDATE_ADD;
      }
#endif      
    break;
    
    /*--------------------------
        ADM_MBREQMAN_UPDATE_ADD
      --------------------------*/
    case ADM_MBREQMAN_UPDATE_ADD:
    
 #ifdef REQ_OPTIM   
      /* End Job */
      /* Update time stamp of last update with current time */
      astMBTable[ui16MBIndex].ui16MBLastTimeStamp=g_ui16Currtime;
      
      /* Update of Address */
      if(FALSE == astMBTable[ui16MBIndex].bValidAddress) 
      {
        /* save ram address where MB has been saved */
        astMBTable[ui16MBIndex].ui16RamAddress=ui16Tmp;       
        /* Validate address */       
        astMBTable[ui16MBIndex].bValidAddress = TRUE;       
      }
      
      /* 
         Increment Data ref Index 
         use the num of dyn data previously saved
      */
     
      ADMReqManState = ADM_MBREQMAN_STEPNEXT;
 #endif

    break;
    
    /*-----------------------
        ADM_MBREQMAN_STEPNEXT
      -----------------------*/
    case ADM_MBREQMAN_STEPNEXT:
    
#ifdef REQ_OPTIM    
      /* 
         Increment Data ref Index 
         use the num of dyn data previously saved
      */
      if(ui16MBIndex+1 == ui8NumBlocks) 
      {
        ui16MBIndex=0;
        ui16MBCurrI2CAdd =0xFFFF;
      } 
      else
      {
        ui16MBIndex++;
      }
      ADMReqManState = ADM_MBREQMAN_CHECK_TMO;
#endif
    
    break;
    
    /*-----------------------
       ADM_REQMAN_ASSIGN_RAM_POS
       This state assings ram position to dynamic data according 
      to the order they are declared in ADM setting file.
      These data don't include the raw status cause its size is unknown until it is requested.
      -----------------------*/
    case ADM_REQMAN_ASSIGN_RAM_POS: 
    {
    
 #ifndef REQ_OPTIM
   
      uint16_t ui16Count=0,ui16Size;
      IO_ErrorType flsErr;
      SettingSizePointerType AddTmp;
      
      while(astDDATRef[ui16Count].ui16I2CAdd !=0xFFFF) 
      {
         if ( (astDDATRef[ui16Count].opt_eUpdatePol == ADM_UPDATEBY_TIME) &&
              (astDDATRef[ui16Count].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ)
            ) 
         {
         
            /* Get data size */
            (void)memcpy((uint8_t*)&(ui16Size),
                         (uint8_t*)(DDATBaseAdd+astDDATRef[ui16Count].DataRecPtrOff+DATASIZE_OFFSET),
                          DDAT_SIZEOF_DATASIZE
                        );
            
            /* save DATA_ADD */
            AddTmp = (SettingSizePointerType)(aui8DynamicData+ui16DynDataWaterMark);
        
       
            /* Save address to DDAT that is in flash */
            if(ADM_INIT_OPTIONS_RESET_ALL == ADM_InitOptions)
            {
				flsErr =IO_E_WORKING;
				do
				{
				  flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&AddTmp,
													DDAT_SIZEOF_DATAADD,
												   (DDATBaseAdd+astDDATRef[ui16Count].DataRecPtrOff+DATAADD_OFFSET));
				}
				while (flsErr ==IO_E_WORKING);
			}
            else
            {
            	flsErr=IO_E_OK;
            }
            
            
            if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
            {
              // save error condition
              eADMErrorState=ADM_FLASH_WRITE_ERR+110;
              ADMManagerState = ADM_IDLE_STATE_JOB_KO;
              /* exit here */
              return ADM_E_KO;
            }  
            
            /* update address valid flag */
            astDDATRef[ui16Count].opt_bValidAddress =1;
               
            /* increment RAM watermark */
            ui16DynDataWaterMark += ui16Size;
            
         }
         ui16Count++; 
      }
      
      ADMReqManState = ADM_REQMAN_CHECK_TMO;
      
  #endif
    }

     
    break;
      
    /*-----------------------
       ADM_REQMAN_CHECK_TMO
       used when request optimizer is disabled
      -----------------------*/
    case ADM_REQMAN_CHECK_TMO:
    {
        bool_t bReqEnable=FALSE;
        
    
 #ifndef REQ_OPTIM
    while(ui16RefIdx < ui8DDATRefIndex)
    {
        
    
    
    if(astDDATRef[ui16RefIdx].opt_eUpdatePol==ADM_UPDATEBY_TIME ) 
    {
          
        /* Get ReqTime */
        (void)memcpy((uint8_t*)&(ui16Tmp),
                    (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAACQTIME_OFFSET),
                    DDAT_SIZEOF_ACQTIME
                    );
      
      /* acquisition time =0 means never !*/    
        if(ui16Tmp>0) 
        {     
            if((g_ui16Currtime-astDDATRef[ui16RefIdx].ui16LastTS) > ui16Tmp) 
            {
                bReqEnable=TRUE;
                //ADMReqManState = ADM_REQMAN_REQDATA_START;
                //break;
            }
        }
    } 
    else if (astDDATRef[ui16RefIdx].opt_eUpdatePol==ADM_UPDATEBY_EVENT ) 
    {
       uint8_t ui8EvIdx;
       
       // MEM OPTIM
       /* get the event index */
      (void)memcpy((uint8_t*)&(ui8EvIdx),
            (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAEVIDX_OFFSET),
             DDAT_SIZEOF_EVIDX
           );
          
      // This check should be modified: this module should check and clear ONLY the bit dedicated to it to do...
      if(aucEvents[ui8EvIdx].ui8New) 
      {
        /* clear event*/
        aucEvents[ui8EvIdx].ui8New=0;
        bReqEnable=TRUE;
      }
    }

    if(bReqEnable==TRUE)
    {
        ADMReqManState = ADM_REQMAN_REQDATA_START;
        break;
    }
        
        ui16RefIdx++;
    }            
    
    /* In case neither occurs
       Increment Data ref Index 
       use the num of dyn data previously saved
    */    
    if(ui16RefIdx == ui8DDATRefIndex) 
    {
        ui16RefIdx=0;
        ui16CurrI2CAdd=0xFFFF; 
    }
    
    /*    
    if(ui16RefIdx+1 == ui8DDATRefIndex) 
    {
        ui16RefIdx=0;
        ui16CurrI2CAdd=0xFFFF; 
    } 
    else
    {
        ui16RefIdx++;
    }
#ifdef REQ_OPTIM
      ADMReqManState = ADM_MBREQMAN_CHECK_TMO;
#else
      //ADMReqManState = ADM_REQMAN_CHECK_TMO;
#endif
    */


#endif
    }      
    break;
 
      
   /*--------------------------
      ADM_REQMAN_REQDATA_START
      This status is used to prepare request of data that is updated on events 
      and is saved either to Flash or to RAM
    --------------------------*/
    case ADM_REQMAN_REQDATA_START:
      
     /*
       If it's the first time this data is retrieved the address where data is stored is invalid
       Data is saved at the watermark of the RAM region where all dyn data are stored.
       At the end this watermark will be saved as data address.
     */
     if(astDDATRef[ui16RefIdx].opt_bSave2Flash == 0) 
     {
      if(0 == astDDATRef[ui16RefIdx].opt_bValidAddress) 
      {   
        ui16SavedDynDataIdx = ui16DynDataWaterMark;
      }
      else
      {
        ui16CurrSaveIdx=0;
      }
     }
     
     
      if (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ) 
      {
        if(ui16CurrI2CAdd != astDDATRef[ui16RefIdx].ui16I2CAdd) 
        {
          eRet=(ADMLIB_ErrorType)ADM_GetApplIdentAdd(astDDATRef[ui16RefIdx].ui16I2CAdd, &stDataTmp);
          if(ADMLIB_OK == eRet) 
          {
            ui16CurrI2CAdd = astDDATRef[ui16RefIdx].ui16I2CAdd;
            /* Check if Appl supports C.R.C.*/
            if(((struct IdentTableStruct *)stDataTmp.pui8Data)->header.crcReady)
            {
              eReqType = ADM_REQ_RAM_DATA_CRC;
            }
            else
            {
              eReqType = ADM_REQ_RAM_DATA_NOCRC;
            }
          } 
          else 
          {
           ADMReqManState = ADM_REQMAN_IDLE_JOB_KO;
           eADMErrorState=ADM_DATAREQ_ERR+151;
           ui8err=3;
           break;
          }
        }
      
     
        /*
          Retrieve data PTR (Data address in appliance) and data size
        */
              
         (void)memcpy((uint8_t*)&(ReadRamAddresses.addr.data),
                (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAPTR_OFFSET),
                DDAT_SIZEOF_DATAPTR
               );
               
         (void)memcpy((uint8_t*)&(ui16DataSize),
                (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATASIZE_OFFSET),
                DDAT_SIZEOF_DATASIZE
               );
        
        /* calculate segmentation of request */     
        NumBlocksToAcquire = (ui16DataSize/MAX_DATA_IN_BYTES_TO_ACQUIRE);
        NumBytesRem        = (ui16DataSize%MAX_DATA_IN_BYTES_TO_ACQUIRE);

        if (NumBlocksToAcquire)
        {
          ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
        }
        else if(NumBytesRem)
        {
          ReadRamAddresses.len = NumBytesRem;
          NumBytesRem = 0;
        } 
        else 
        {
          /* data length is =0: skip data request but save pointer */
            if(astDDATRef[ui16RefIdx].opt_bSave2Flash == 1) 
            {
              /* End Job */
               ui16Tmp = ADM_START_LOGFLASH;
              eSave2FlashState = ADM_SAVE2FLASH_ERASE;
             
            } 
            else 
            {
               ui16Tmp = ui16SavedDynDataIdx+(uint16_t)aui8DynamicData;
            }
            
            ADMReqManState = ADM_REQMAN_UPDATE_ADD;
            break;
        }
      }
     
     
      ADMReqManState = ADM_REQMAN_REQDATA;  

   // break; /*to save time*/
    
     /*-----------------------
        ADM_REQMAN_REQDATA
        This status is used to request data that is updated on events 
      and is saved either to Flash of to RAM
      -----------------------*/
    case ADM_REQMAN_REQDATA: 
    {
      uint8_t *pReqLen =IdentMainReqLen; 
      
      if (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ) 
      {
        admErr = ADM_DataReq((uint8_t)(astDDATRef[ui16RefIdx].ui16I2CAdd),eReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
      } 
      else 
      {
        // by default get the status from main board? to do...
        admErr = ADM_DataReq((uint8_t)(astDDATRef[ui16RefIdx].ui16I2CAdd),ADM_REQ_STATUS,pReqLen,1);
      }
      
      if(ADM_E_OK == admErr)
      {
        /* Get data of the Appliance */
        if(astDDATRef[ui16RefIdx].opt_bSave2Flash == 1) 
        {
          ADMReqManState = ADM_REQMAN_SAVEDATA_2FLASH;
        } 
        else 
        {
          ADMReqManState = ADM_REQMAN_SAVEDATA;
        }
        
      }
      else if(ADM_E_KO == admErr)
      {
       /* save error condition*/
          BCM_KillPacketRequest( (IO_ChannelType)BCP_I2C_CH_ID , (BCM_RequestType)ADM_CP_REQUEST_ID);        
      }
      else
      {
        /* wait for response */
      }
    }
    
    if(ADMReqManState != ADM_REQMAN_SAVEDATA) /* to save time */
    {
       break; 
    }
    
    
   /*-----------------------
      ADM_REQMAN_SAVEDATA
      This status is used to save data to RAM in case of dynamic data updated on events.
    -----------------------*/
    case ADM_REQMAN_SAVEDATA:
    
      ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
      if(0 == astDDATRef[ui16RefIdx].opt_bValidAddress) 
      {
          /* if this data doesn't have an address yet I put it at first available ram address (watermark)...*/
          pAddr = (uint8_t*)(aui8DynamicData+ui16DynDataWaterMark);
          (void)memcpy(pAddr,pAnswerBuffer+incomingDataOff,incomingDataSize);
          ui16DynDataWaterMark += incomingDataSize;
      } 
      else 
      {
          /* ...otherwise the address is stored in DDAT */
          
          
          (void)memcpy( (uint8_t*)&pAddr,
                  (uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAADD_OFFSET),
                  DDAT_SIZEOF_DATAADD
                );
          pAddr += ui16CurrSaveIdx;     
          (void)memcpy(pAddr,pAnswerBuffer+incomingDataOff,incomingDataSize);
          ui16CurrSaveIdx += incomingDataSize;
      }
      
      if (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_RAMREQ) 
      {
        /* calculate remaining bytes */
        if ( NumBlocksToAcquire > 1 )
        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read  */
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
          NumBlocksToAcquire--;
          ADMReqManState = ADM_REQMAN_REQDATA;
        }
        else if (NumBytesRem)
        {
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = NumBytesRem;
          NumBytesRem = 0;
          ADMReqManState = ADM_REQMAN_REQDATA;
        }
        else
        {
          /* End Job */
          ui16Tmp = ui16SavedDynDataIdx+(uint16_t)aui8DynamicData;
          ADMReqManState = ADM_REQMAN_UPDATE_ADD;
          
          
          /* */
          if(astDDATRef[ui16RefIdx].LastUpdateID != ADM_DynLastUpdateID)
          {
        	  astDDATRef[ui16RefIdx].LastUpdateID = ADM_DynLastUpdateID;
        	  ADM_TracksUpdatedByTmoCount++;
        	  if(ADM_TracksUpdatedByTmoCount == ui8DDATRefOnTimeIndex)
        	  {
        		  ADM_TracksUpdatedByTmoCount=0;
        		  ADM_DynLastUpdateID++;
        		  if(!ADM_DynLastUpdateID)
        		  {
        			  ADM_DynLastUpdateID++;
        		  }
        	  }
          }
 
        }
      } 
      else /* supposed  REQSTATUS*/
      {
          /* End Job 
             Status request stay in one single message
          */
          ui16Tmp = ui16SavedDynDataIdx+(uint16_t)aui8DynamicData;
          ADMReqManState = ADM_REQMAN_UPDATE_ADD;
          
          /* */
			if(astDDATRef[ui16RefIdx].LastUpdateID != ADM_DynLastUpdateID)
			{
				astDDATRef[ui16RefIdx].LastUpdateID = ADM_DynLastUpdateID;
				ADM_TracksUpdatedByTmoCount++;
				if(ADM_TracksUpdatedByTmoCount == ui8DDATRefOnTimeIndex)
				{
					ADM_TracksUpdatedByTmoCount=0;
					ADM_DynLastUpdateID++;
					if(!ADM_DynLastUpdateID)
					{
						ADM_DynLastUpdateID++;
					}
				}
			}
      }
     
    break;
        
        
     /*-------------------------------------------
        ADM_REQMAN_SAVEDATA_2FLASH
        This status is used to save data to Flash
      --------------------------------------------*/
    case ADM_REQMAN_SAVEDATA_2FLASH:
    
    if(eSave2FlashState == ADM_SAVE2FLASH_ERASE) 
    {
     /* Erase flash block (flash address is hard coded) */
     flsErr = FLS_BlanckCheckEraseVerifyAsync(ui16DataSize, ADM_START_LOGFLASH);
     if((IO_ErrorType)IO_E_OK == flsErr)
      {
        ADM_RMFlsDst = ADM_START_LOGFLASH;
        eSave2FlashState = ADM_SAVE2FLASH_WRITE;
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
       /* save error condition*/
        eADMErrorState=ADM_FLASH_WRITE_ERR+111;
       
        ADMReqManState = ADM_REQMAN_STEPNEXT;
      }
      else
      {
        /* wait */
      }
    } 
    else if(eSave2FlashState == ADM_SAVE2FLASH_WRITE) 
    {
      ADM_BCP_GetAnswParams (pAnswerBuffer,(uint16_t*) &incomingDataSize,(uint8_t*) &incomingDataOff);
      
      if(incomingDataSize==0) 
      {
        flsErr= IO_E_OK;
      }
      else
      {
        flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+incomingDataOff,incomingDataSize, ADM_RMFlsDst);
      }
      
      if((IO_ErrorType)IO_E_OK == flsErr)
      {
         ADM_RMFlsDst+=incomingDataSize;
         
        /* calculate remaining bytes */
        if ( NumBlocksToAcquire > 1 )
        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
          NumBlocksToAcquire--;
          ADMReqManState = ADM_REQMAN_REQDATA;
        }
        else if (NumBytesRem)
        {
          ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
          ReadRamAddresses.len = NumBytesRem;
          NumBytesRem = 0;
          ADMReqManState = ADM_REQMAN_REQDATA;
        }
        else
        {
          /* End Job */
          ui16Tmp = ADM_START_LOGFLASH;
          ADMReqManState = ADM_REQMAN_UPDATE_ADD;
          eSave2FlashState = ADM_SAVE2FLASH_ERASE;
        }
      }
      else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
      {
       /* save error condition*/
         eADMErrorState=ADM_FLASH_WRITE_ERR+112;
        
        ADMReqManState = ADM_REQMAN_STEPNEXT;
        eSave2FlashState = ADM_SAVE2FLASH_ERASE;
      }
      else
      {
        /* wait */
      }
    } 
    else
    {
      /* error unexpected state */
    }
    
    break;
    
    /*-----------------------
        ADM_REQMAN_UPDATE_ADD
        This status is used to update address in DDAT of dynamic data that is updated on events
      -----------------------*/
    case ADM_REQMAN_UPDATE_ADD: 
    {
      
    IO_ErrorType flsErr;
      /* End Job */
      /* Update time stamp of last update with current time */
      astDDATRef[ui16RefIdx].ui16LastTS=g_ui16Currtime;
      
      /* Update of Address */
      if(0 == astDDATRef[ui16RefIdx].opt_bValidAddress) 
      {
        SettingSizePointerType AddTmp = (SettingSizePointerType)ui16Tmp;
        
        // MEM OPTIM
        /*
        memcpy((uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAADD_OFFSET),
               &AddTmp,
               DDAT_SIZEOF_DATAADD);
        */
        
        /* Save address to DDAT that is in flash */
         flsErr =IO_E_WORKING;
         do
         {
           flsErr = FLS_WriteAndVerifyAsync((uint8_t*)&AddTmp,
                                             DDAT_SIZEOF_DATAADD,
                                            (DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATAADD_OFFSET));
         }
         while (flsErr ==IO_E_WORKING);
             
         if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
         {
           // save error condition
           eADMErrorState=ADM_FLASH_WRITE_ERR+113;
           ADMManagerState = ADM_IDLE_STATE_JOB_KO;
           /* exit here */
           return ADM_E_KO;
         }       

       
       /* if it was a status request the size is not initialized and must be updated with
          actual size received.
        */
        if (astDDATRef[ui16RefIdx].opt_eUpdateBy == ADM_UPDATEWITH_STATREQ) 
        {
          // MEM OPTIM
          /*
          memcpy((uint8_t*)(DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATASIZE_OFFSET),
               &incomingDataSize,
               DDAT_SIZEOF_DATASIZE);
           */
           
         flsErr =IO_E_WORKING;
         do
         {
           flsErr = FLS_WriteAndVerifyAsync( (uint8_t*)&incomingDataSize,
                                             DDAT_SIZEOF_DATASIZE,
                                             (DDATBaseAdd+astDDATRef[ui16RefIdx].DataRecPtrOff+DATASIZE_OFFSET));
         }
         while (flsErr ==IO_E_WORKING);
             
         if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
         {
           // save error condition
           eADMErrorState=ADM_FLASH_WRITE_ERR+114;
           ADMManagerState = ADM_IDLE_STATE_JOB_KO;
           /* exit here */
           return ADM_E_KO;
         }       
         
         /* save address also in main board info */        
          stMainBoardInfo.pui8StatAdd= (uint8_t*)AddTmp;
          stMainBoardInfo.bValidStatAdd=TRUE;
        
        }
        
        /* Validate address */       
        astDDATRef[ui16RefIdx].opt_bValidAddress = 1;       
      }
      
      /* 
         Increment Data ref Index 
         use the num of dyn data previously saved
      */
     
      ADMReqManState = ADM_REQMAN_STEPNEXT;
    }
    
    //break;  /*to save time*/
    
    /*-----------------------
        ADM_REQMAN_STEPNEXT
      -----------------------*/
    case ADM_REQMAN_STEPNEXT:
    
      /* 
         Increment Data ref Index 
         use the num of dyn data previously saved
      */
      if(ui16RefIdx+1 == ui8DDATRefIndex) 
      {
        ui16RefIdx=0;
        ui16CurrI2CAdd=0xFFFF; 
      } 
      else
      {
        ui16RefIdx++;
      }
#ifdef REQ_OPTIM
      ADMReqManState = ADM_MBREQMAN_CHECK_TMO;
#else
      ADMReqManState = ADM_REQMAN_CHECK_TMO;
#endif
      
    
    break;
    
     /*-----------------------
        ADM_REQMAN_IDLE
      -----------------------*/
    case ADM_REQMAN_IDLE:
    break;
    
    /*-----------------------
        ADM_REQMAN_IDLE_JOB_KO
      -----------------------*/
    case ADM_REQMAN_IDLE_JOB_KO:
    break;
    
    default:
    
    break;
  }
  
  if(ADMReqManState == ADM_REQMAN_IDLE) 
  {
    return ADM_E_OK;
  }
  else if(ADMReqManState == ADM_REQMAN_IDLE_JOB_KO) 
  {
    return ADM_E_KO;
  }
  else 
  {
    return ADM_E_WORKING;
  }
}





/**
\n Initialize ADM_Manager() function.
\n This routine must be called before calling ADM_Manager() at cold and hot reset, if supported.

\author   Andrea.Mantovani
\date   Feb X, 2010
\version  1.0.0

\note   \par \b Note 1:
      documentation by Andrea.Mantovani

*/

void ADM_ManagerInit(void)
{
  bFirstElement=TRUE;
  bEndJob=FALSE;
  
  stADMDataRecord.ui16ApplI2CAdd=0xFF;

  stCurrApplData.pui8IdentAdd= (uint8_t*)0xFFFF;
  stCurrApplData.ui16CurrApplI2CAdd=0xFFFF;
  stCurrApplData.ui8CurrASFDataNum=0;
  stCurrApplData.ui8CurrIOFDataNum=0;

   /* Initialize ADM_Manager state machine */
  ADMManagerState = ADM_PREREQUISITES_STATE;
  
  /* Initialize last update ID */
  ADM_DynLastUpdateID=1;
  
  DDATBaseAdd = (FLS_positionType) (aui8DynamicData+64/*MAX_ASF_EXTRAPOINTERS_BYTES*/);
  
  
}



/**
\n Initialize ADM_RequestManager() function.
\n This routine must be called before calling ADM_Manager() at cold and hot reset, if supported.

\author   Andrea.Mantovani
\date   Feb X, 2010
\version  1.0.0

\note   \par \b Note 1:
      documentation by Andrea.Mantovani

*/
void ADM_ReqManagerInit(void)
{
  /* Initialize ADM_ReqManager state machine */
   ADMReqManState =ADM_REQMAN_START;       
  eSave2FlashState = ADM_SAVE2FLASH_ERASE;
}



/* Local Function */

enum ePP_BoardId I2c_to_Board(uint8_t ui8I2CAddress ) 
{
  //uint8_t ui8BoardID;
  enum ePP_BoardId ui8BoardID;
  switch (ui8I2CAddress) 
  {
    
    case MB_ADDRESS:   /**< I2C Main Board */
    ui8BoardID=MB_ID;
    break;
    case UI_ADDRESS:   /**< I2C Visual */
    ui8BoardID=UI_ID;
    break;
    
    /*
    case BM_ADDRESS:   //< I2C Bootloader Main Board 
    ui8BoardID=;
    break;
    case WS_ADDRESS:   // I2C Weight Sensor 
    ui8BoardID=;
    break;
    case ZB_ADDRESS:          
    ui8BoardID=;
    break;
    */
    
    default:
    ui8BoardID=0xFF;
    break;
  }
  return ui8BoardID;
}

/*
  Temporary function to be replaced by BCP function
*/
void ADM_BCP_GetAnswParams (uint8_t* pAnswerBuffer,uint16_t* pui16AnswLen,uint8_t* pui8PayloadOffset) 
{
   *pui16AnswLen = LIB_ConvTwoByteInWord(pAnswerBuffer);

#ifdef ADM_USES_BCP_BRIDGE_MODE
    if(*pui16AnswLen>(PP_GP_OUT_BUFF_RESERVED_SZ-PP_OUT_BUFF_RESERVED_SZ))
    {
        *pui16AnswLen -= (PP_GP_OUT_BUFF_RESERVED_SZ-PP_OUT_BUFF_RESERVED_SZ);
    } else
    {
        *pui16AnswLen=0;
    }
   
    *pui8PayloadOffset = PP_GP_OUT_BUFF_RESERVED_SZ;
#else
   *pui8PayloadOffset = PP_OUT_BUFF_RESERVED_SZ;
#endif

}

#ifndef ADM_USES_BCP_BRIDGE_MODE

static const uint8_t aaConvTab[BOARD_NUM][ADM_REQ_NUMREQ] = 
{ 
  /*Board 0*/ {PP_IDENT_MAIN_REQ,PP_RRAM_CRC_MAIN_REQ,PP_RRAM_MAIN_REQ}, 
  /*Board 1*/ {PP_IDENT_UI_REQ  ,PP_RRAM_CRC_UI_REQ  ,PP_RRAM_UI_REQ}
};

#endif

/**
This function perform a generic request on the bus. 

\param  [in]    ui8I2CAddress: I2C appliance address.
\param  [in]		eRequest: request type
\param  [in] 	  pReqParams pointer to the paramenters's request
\param  [in] 	  lenReqParams size in bytes of the pReqParams.

\return 	ADM_ErrorType

\retval		ADM_E_OK work done
\retval		ADM_E_WORKING work in progress
\retval		ADM_E_KO problem occurred		 	 

\author 	Andrea.Mantovani 
\date		Feb X, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_DataReq(uint8_t ui8I2CAddress,ADMRequestType eRequest, void *pReqParams, IO_LenType lenReqParams)
{
  IO_ErrorType result;
  ADM_ErrorType retValue;
  enum ePP_BoardId ui8BoardID;
  BCM_RequestType BCMRequest;
  void* pvRequest;
  IO_LenType paramLen;
  uint8_t aucTmp[4];

	#ifdef ADM_SIMUL
		return IO_E_OK;
	#endif

#ifdef ADM_USES_BCP_BRIDGE_MODE

  ((CP_GP_ParamType*)aucParamBuff)->ZBParam.Hdr.HdrByte.CommandID=PP_CMD_EXTENDED_WITH_ANSW;
  ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.I2cAddress=ui8I2CAddress;
  
  if(eRequest==ADM_REQ_IDENT) 
  {
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Type=PP_EE_TYPE_IDENT;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Hdr = 0; /* Checksum */
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Len=1;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Data=0; /* read all Ident*/
  } 
  else if(eRequest==ADM_REQ_STATUS) 
  {
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Type=PP_EE_TYPE_READ_STATUS;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Hdr = 0; /* Checksum */
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Len=1;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Data=0; /* read all Status*/
  }
  else if(eRequest==ADM_REQ_RAM_DATA_CRC) 
  { 
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Type=PP_EE_TYPE_READ_FLASH;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Hdr = 1; /* CRC */
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Len = 4;
    
    
    aucTmp[0]=(((PP_RRamParamType*)pReqParams)->addr.data) & 0x00FF;
    aucTmp[1]=((((PP_RRamParamType*)pReqParams)->addr.data) & 0xFF00)>>8;
    aucTmp[2]=(((PP_RRamParamType*)pReqParams)->addr.data+((PP_RRamParamType*)pReqParams)->len - 1)& 0x00FF;
    aucTmp[3]=((((PP_RRamParamType*)pReqParams)->addr.data+(((PP_RRamParamType*)pReqParams)->len - 1)& 0xFF00)>>8);
    (void)memcpy( &(((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Data),aucTmp,4);
  }
  else
  {
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Type=PP_EE_TYPE_READ_FLASH;
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Hdr = 0; /* Checksum */
    ((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Len = 4;
    aucTmp[0]=(((PP_RRamParamType*)pReqParams)->addr.data) & 0x00FF;
    aucTmp[1]=((((PP_RRamParamType*)pReqParams)->addr.data) & 0xFF00)>>8;
    aucTmp[2]=(((PP_RRamParamType*)pReqParams)->addr.data+((PP_RRamParamType*)pReqParams)->len - 1)& 0x00FF;
    aucTmp[3]=((((PP_RRamParamType*)pReqParams)->addr.data+(((PP_RRamParamType*)pReqParams)->len - 1)& 0xFF00)>>8);
    (void)memcpy( &(((CP_GP_ParamType*)aucParamBuff)->ZBParam.PL.PlByte.PP_Data),aucTmp,4);
  }

  BCMRequest=ADM_CP_REQUEST_ID;
  
  /* answer buffer is used also to pass parameters */
  pvRequest = aucParamBuff;
  paramLen  = sizeof(CP_GP_ParamType)+1;
  
#else

  /* perform conversion for BCP */
  ui8BoardID = I2c_to_Board(ui8I2CAddress);
  BCMRequest =  aaConvTab[ui8BoardID][eRequest];
  pvRequest= pReqParams;
  paramLen = lenReqParams;
  
#endif	

  /* 0) Set Params for Request */
  
  (void)CRM_SetPacketParamSync((IO_ChannelType)BCP_I2C_CH_ID, BCMRequest, pvRequest, paramLen);

  /* 1) Append request to the queue */
  
  (void)CRM_SetPacketRequestSync((IO_ChannelType)BCP_I2C_CH_ID, BCMRequest);
  /* 2) Verify the request state */
      
  result = BCM_TestRequest((IO_ChannelType)BCP_I2C_CH_ID, BCMRequest);
  /* Prevedere timeout uscita ToCheck */
  switch (result)
  {
      case IO_E_OK:
          /* Data Avaible */
          retValue = ADM_E_OK;        
          break;
      case IO_E_ALREADY_PENDING:
          /* Here check timeout */
          retValue = ADM_E_WORKING;
          break;
      case IO_E_FAIL:
      case IO_E_NOT_QUEUED:
      default:
          retValue = ADM_E_KO;
          break;
      
  }
  return (retValue);
}


/*===========================================================================
  EVENT MANAGER
===========================================================================*/

#define ANDOR_POSITION_MASK   0x80
#define OPER_POSITION_MASK    0x60

#define SIGNED_POSITION_MASK  0x10
#define SIZE_POSITION_MASK    0x0E

#define CONST_POSITION_MASK   0x01

/**
N.B.: These values must be the same as those specified in Documentation 
*/
typedef enum 
{
/** condition == */
  _EQ_    =0,
/** condition != */
  _DIFF_  =1,
/** condition > */  
  _GREAT_ =2,
/** condition < */  
  _LESS_  =3

} OperType;

typedef enum 
{
  ADM_EVHND_IDLE=0,
  ADM_EVHND_WORKING,
  ADM_EVHND_JOB_KO
} ADMEventHndStateType;

static ADMEventHndStateType ADMEventHndState=ADM_EVHND_IDLE;

static uint8_t ui8EvId=0 ;

/**
This function implements the EventManager that search for event definitions inside the Setting File
 and evaluates them saving result in an events array.
It must be called in a main loop after ADM_Manager has completed its job.(return ADM_E_OK)

\param  [in] 	  none
\param  [out]   none

\return 	ADM_ErrorType  

\retval	ADM_E_OK      Job is done (never used cause this task is always working)
\retval	ADM_E_WORKING	Job is ongoing
\retval	ADM_E_KO	    Job failed.

\author 	Andrea.Mantovani 
\date		Mar X, 2010 
\version	1.0.0

   
\note		\par \b Note 1:
			documentation by Andrea.Mantovani

*/
ADM_ErrorType ADM_EventManager(void)
{

#ifdef EVENT_ENGINE

  uint8_t ui8Result,ui8Size;
  uint8_t ui8Index=0,ui8Sum2Index;
  bool_t BoolCond,bFirstCond=TRUE, bPartial;

  uint32_t ui32Data1,ui32Data2;
  ADMDataType stADMData;
  ADM_ErrorType eResult;
  Loader_Struct stSFData;

  switch(ADMEventHndState)
  {
    case ADM_EVHND_IDLE:
      ADMEventHndState= ADM_EVHND_WORKING;
      break;

    case ADM_EVHND_WORKING:

      ui8Result = SFBasicLoader((uint16_t*)&Eeprom[0],
                                ADM_SF_EVENTS_PTR,
                                ui8EvId, &stSFData);

      if(ui8Result == SF_OK)
      {

        /* size returned by SFBasicLoader is in words */
        while(ui8Index<stSFData.DataLenght*2)
        {

          /* This is the size of second operand should it be a constant value but it also represent
              the cast performed to data types of data identified by the two operands wathever should be their
              original data type.
            */
          ui8Size = (stSFData.Data[ui8Index] & SIZE_POSITION_MASK)>>1;


          /* At offset 1 in data pointed in SettingFile there's the TAG of first operand
              Data size must be 1,2 or 4 that are the sizes of the data types that can be compared each other
            */
          eResult=ADM_GetData((stSFData.Data)[ui8Index+1],&stADMData);

          if(eResult!=ADM_E_OK)
          {
            /* return error but don't go to error state */
            return eResult;
          }

          /* Actual size of data (stADMData.ui16DataSize) is ignored cause it is casted to the data type used for comparison */
          switch(ui8Size)
          {
            case 1:
              ui32Data1 = (uint32_t) stADMData.pui8Data[0];
              break;

            case 2:
              ui32Data1 = (uint32_t) (GETW_BE(stADMData.pui8Data));
              break;

            case 4:
              ui32Data1 = (uint32_t) (GETDW_BE(stADMData.pui8Data));
              break;

            default:
            /* invalid size of address, save error to do...*/
              ADMEventHndState =  ADM_EVHND_JOB_KO;
              break;
          }

          if(((stSFData.Data)[ui8Index] & CONST_POSITION_MASK) !=0)
          {
            /* constant is saved as BIG ENDIAN in Setting file so we need to convert to Little Endian
                                                in case the micro where ADM is running was Little Endian.*/


            /* At offset 2 in data pointed in SettingFile there's the data rapresenting the second operand */
            switch(ui8Size)
            {
              case 1:
                ui32Data2 = (uint32_t) stSFData.Data[ui8Index+2];
                /* go to next condition */
                ui8Sum2Index = ui8Size+1+2;
                break;

              case 2:
              /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (IdentTable.header.littleEndian)
                {
                  ui32Data2 = (uint32_t) (GETW_LE(&(stSFData.Data[ui8Index+2])));
                }
                else
                {
                  ui32Data2 = (uint32_t) (GETW_BE(&(stSFData.Data[ui8Index+2])));
                }
                /* go to next condition*/
                ui8Sum2Index = ui8Size+2;
                break;

              case 4:
              /* you must use either GETW_BE or GETW_LE depending on endianess */
                if (IdentTable.header.littleEndian)
                {
                  ui32Data2 = (uint32_t) (GETDW_LE(&(stSFData.Data[ui8Index+2])));
                }
                else
                {
                  ui32Data2 = (uint32_t) (GETDW_BE(&(stSFData.Data[ui8Index+2])));
                }
                /* go to next condition */
                ui8Sum2Index = ui8Size+2;
                break;

              default:
              /* invalid size of address, save error to do...*/
                ADMEventHndState =  ADM_EVHND_JOB_KO;
                break;

            }
          }
          else
          {
            /* At offset 2 in data pointed in SettingFile there's the TAG of data rapresenting the second operand */
            eResult=ADM_GetData((stSFData.Data)[2],&stADMData);

            if(eResult!=ADM_E_OK)
            {
              /* return error but don't go to error state */
              return eResult;
            }

            switch(ui8Size)
            {
              case 1:
                ui32Data2 = (uint32_t) stADMData.pui8Data[0];
                break;

              case 2:
                ui32Data2 = (uint32_t) (GETW_BE(stADMData.pui8Data));
                break;

              case 4:
                ui32Data2 = (uint32_t) (GETDW_BE(stADMData.pui8Data));
                break;

              default:
              /* invalid size of address, save error to do... */
                ADMEventHndState =  ADM_EVHND_JOB_KO;
                break;
            }

            /* go to next condition */
            ui8Sum2Index = 4;
          }


          switch(((stSFData.Data)[ui8Index] & OPER_POSITION_MASK)>>5)
          {
            case _EQ_ :
              bPartial = (ui32Data1 == ui32Data2);
              break;

            case _DIFF_ :
              bPartial = (ui32Data1 != ui32Data2);
              break;

            case _GREAT_  :
            /* Must check with sign */
              switch( (stSFData.Data[ui8Index] & (SIGNED_POSITION_MASK | SIZE_POSITION_MASK))>>1 )
              {
                case 0x09:
                  bPartial = ((int8_t)ui32Data1 > (int8_t)ui32Data2);
                  break;
                case 0x01:
                  bPartial = ((uint8_t)ui32Data1 > (uint8_t)ui32Data2);
                  break;
                case 0x0A:
                  bPartial = ((int16_t)ui32Data1 > (int16_t)ui32Data2);
                  break;
                case 0x02:
                  bPartial = ((uint16_t)ui32Data1 > (uint16_t)ui32Data2);
                  break;
                case 0x0C:
                  bPartial = ((int32_t)ui32Data1 > (int32_t)ui32Data2);
                  break;
                case 0x04:
                  bPartial = (ui32Data1 > ui32Data2);
                  break;
                default:
                /* error in setting file*/
                  ADMEventHndState =  ADM_EVHND_JOB_KO;
                  break;

              }

              break;

            case _LESS_  :
            /* Must check with sign */
              switch( (stSFData.Data[ui8Index] & (SIGNED_POSITION_MASK | SIZE_POSITION_MASK))>>1 )
              {
                case 0x09:
                  bPartial = ((int8_t)ui32Data1 < (int8_t)ui32Data2);
                  break;
                case 0x01:
                  bPartial = ((uint8_t)ui32Data1 < (uint8_t)ui32Data2);
                  break;
                case 0x0A:
                  bPartial = ((int16_t)ui32Data1 < (int16_t)ui32Data2);
                  break;
                case 0x02:
                  bPartial = ((uint16_t)ui32Data1 < (uint16_t)ui32Data2);
                  break;
                case 0x0C:
                  bPartial = ((int32_t)ui32Data1 < (int32_t)ui32Data2);
                  break;
                case 0x04:
                  bPartial = (ui32Data1 < ui32Data2);
                  break;
                default:
                /* error in setting file*/
                  ADMEventHndState =  ADM_EVHND_JOB_KO;
                  break;

              }
              break;

            default:
              break;
          }

          if(bFirstCond==FALSE)
          {
            if(((stSFData.Data)[ui8Index] & ANDOR_POSITION_MASK)!=0 )
            {
              /* OR */
              BoolCond |= bPartial;
            }
            else
            {
              /* AND */
              BoolCond &= bPartial;
            }
          }
          else
          {
            BoolCond = bPartial;
            bFirstCond=FALSE;
          }

          /* update index according to position of next condition */
          ui8Index+=ui8Sum2Index;


        } /* end while(ui8Index<stSFData.DataLenght*2) */

        /*  
            Update Bool flags related to event ui8EvId
            Following instructions sets the New condition to 0xFF (0xFF instead of true cause each bit could be assigned to an event "user")
            The set is made only if the old value was 0 (positive edge).
         */
        if( BoolCond == TRUE && aucEvents[ui8EvId].ui8Old ==FALSE)
        {
          aucEvents[ui8EvId].ui8Old = BoolCond;
          aucEvents[ui8EvId].ui8New = 0xFF;
        }

       /* Reset of old bool condition */
        if(BoolCond == FALSE && aucEvents[ui8EvId].ui8Old ==TRUE)
        {
          aucEvents[ui8EvId].ui8Old = BoolCond;
        }

        ui8EvId++;
      }
      else if (ui8Result == SF_LDR_ERR_ENDOFPOINTER)
      {
        /* end of events: restart from the first one */
        ui8EvId =0;
      }
      else
      {
        ADMEventHndState =  ADM_EVHND_JOB_KO;
      }
      break;

      case ADM_EVHND_JOB_KO:

      break;

  }

  if(ADMEventHndState == ADM_EVHND_IDLE)
  {
    return ADM_E_OK;
  }
  else if(ADMEventHndState == ADM_EVHND_JOB_KO)
  {
    return ADM_E_KO;
  }
  else
  {
    return ADM_E_WORKING;
  }
 
 #else
   return ADM_E_KO;
 #endif
 
}




/* *********** */
/* End of file */
/* *********** */