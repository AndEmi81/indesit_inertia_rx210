/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup	PM_ADM 
@{
\n
\n			At the moment this module is able to read and store in M.C.U. flash:
\n\li       Main Ident Table
\n\li       User Interface Ident Table
\n\li       Main Setting File
\n\li       Main LookupTablePointers plus LookupTableDataSizes
\n\li       User Interface LookupTablePointers plus LookupTableDataSizes
\n          The module create also a lookup table for data accessing.
\n
\par Configuration:
\n\li       Link, inside the project, B.C.P. and the right HAL, module need config. cfr. related documentation.
\n          Note: in BusCommPltf_cfg.h you must also define the size of I2C Answer Buffer.
\n          Eg.:
\n          \#define \b MAX_DATA_IN_BYTES_TO_ACQUIRE    64  // this is the Answer Buffer size minus 2 
\n          Note: Read Ram Packet with and without CRC must be supported.              
\n          Note: in ApplDataMng_cfg.h you must define the Answer Buffer/s where requests on I2C Bus are stored and
\n          Ident Request Lenght.
\n          Eg.: if you have defined only an Answer Buffer both U.I. and Main requests named I2CAnswerBuffer
\n          \#define \b MainI2CAnswerBuffer I2CAnswerBuffer
\n          \#define \b UII2CAnswerBuffer   I2CAnswerBuffer
\n          if you have defined only an Ident Request Lenght named IdentReqLen  
\n          \#define \b IdentMainReqLen IdentReqLen
\n          \#define \b IdentUIReqLen   IdentReqLen
\n\li       Link inside the project OCFLS driver and the right HAL,module need config. cfr. related documentation.
\n\li       Link inside the project OCWDT driver and the right HAL,module need config. cfr. related documentation.
\n\li       Link inside the project LIB_Utilities.
\n\li       Inside the table 
\n\b        BEGIN_MCU_FLASH_MAP(NUM_FLASH_BLOCKS)
\n
\n\b        END_MCU_FLASH_MAP()
\n          insert a suitable flash's segment large enough to store data listed above.
\n          Eg.:
\n\b            BEGIN_MCU_FLASH_MAP(NUM_FLASH_BLOCKS)
\n              0x03F00,0x03FFF,    //reserved backup
\n              0x04000,0x07FFF,    //reserved ADM
\n\b            END_MCU_FLASH_MAP()
\n
\n          Note: a detail description of the above table is in FLS driver documentation.
\n          Note: on \b HCS08QExy you can't use banked page.
\n\li       In ApplDataMng_cfg.h define the start avaible flash for ADM
\n          Eg.
\n          \#define \b START_AVAIBLE_FLASH     \b 0x04000
\n\li       In ApplDataMng_cfg.h define the end avaible flash for ADM
\n          Eg.
\n          \#define \b END_AVAIBLE_FLASH     \b 0x07FFF
\n\li       In ApplDataMng_cfg.h define how many pointers you need in ADM LookUp Table
\n          Eg.
\n          \#define \b N_LOOKUP_TABLE_POINTERS \b 30
\n          Note: the ADM LookUp Table will start from address defined in START_AVAIBLE_FLASH
\n          Pointers displacements inside ADM LookUpTable are:
\n\li\b          MAIN_IDENT_POINTER_OFFSET       \b    0
\n\li\b          UI_IDENT_POINTER_OFFSET         \b    1
\n\li\b          MAIN_SETTING_POINTER_OFFSET     \b    2
\n\li\b          MAIN_LOOKUP_POINTER_OFFSET      \b    3
\n\li\b          MAIN_LOOKUP_SIZE_OFFSET         \b    4
\n\li\b          UI_LOOKUP_POINTER_OFFSET        \b    5
\n\li\b          UI_LOOKUP_SIZE_OFFSET           \b    6
\n          Note: The difference between one pointer, P, and the next pointer, P+1,  indicates the size in bytes of
\n          the field pointed by P.
\n          \b Good \b Luck 
\n
\par Exported Functions:
\n
@}
\ingroup   	ProductM 
*/
/**
\n
\Par		Implementation of all functions necessary to achieve the aim.
\n
\file		ApplDataMng.c 
\ingroup	PM_ADM
\date		Dec 9, 2009 
\version	01.00.00 
\author		stefano.ughi 
\n
\n
\Par\b	History:
\n 
\n 		00.24.00 
\n		Dec 9, 2009 
\n		stefano.ughi 
\n		creation 
\n
\n
\n etc.
*/

/* ******** */
/* Includes */
/* ******** */
#include "ApplDataMng.h"
#include "OCFLS_drv.h"
#include "LIB_Utilities.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */

/*================================*/
/* Start Setting File Definitions */
/*================================*/
/**
This is the size in bytes of a pointer inside a generic Setting File
*/
#define SETTING_FILE_PNTR_SIZE_IN_BYTES         2
/**
This is the size in bytes of a LookUp Table item
*/
#define LUT_ITEM_SIZE_IN_BYTES                  2
/**
This is the value of an empty pointer inside A.D.M. Application
*/
#define EMPTY_POINTER   0xFFFFFFFF
/**
This is the Offset of Setting File EOF Pointer vs EEPaddress
*/
#define SETTING_FILE_PNTR_EOF_OFFSET    0x07
/**
This is the Offset of Setting File Checksum Pointer vs EEPaddress.\n
CheckSum is computed from Start Setting File up to CheckSum's Location stored in SETTING_FILE_PNTR_CHK_OFFSET. 
*/
#define SETTING_FILE_PNTR_CHK_OFFSET    0x09
/*==============================*/
/* End Setting File Definitions */
/*==============================*/


/** Here Start LookUpTable */
#define START_LOOKUP_TABLE              (START_AVAIBLE_FLASH) 

/** LookUpTable Pointers Positions inside LookUpTable */
#define MAIN_IDENT_POINTER_OFFSET       0
#define UI_IDENT_POINTER_OFFSET         1
#define MAIN_SETTING_POINTER_OFFSET     2
#define MAIN_LOOKUP_POINTER_OFFSET      3
/*lint -esym(750, MAIN_LOOKUP_SIZE_OFFSET) macro not referenced but necessary */
#define MAIN_LOOKUP_SIZE_OFFSET         4
/*lint +e750 local macro not referenced */
#define UI_LOOKUP_POINTER_OFFSET        5
/*lint -esym(750, UI_LOOKUP_SIZE_OFFSET) macro not referenced but necessary */
#define UI_LOOKUP_SIZE_OFFSET           6


/** Here start LookupTableDataSizes Vector */
#define START_LOOKUP_TABLE_SIZES        (START_AVAIBLE_FLASH+sizeof(FLS_positionType)*(N_LOOKUP_TABLE_POINTERS+1))
/** Here start Appliances's Ident Tables */
#define START_STATIC_APPLIANCES_DATA    (START_LOOKUP_TABLE_SIZES+N_LOOKUP_TABLE_POINTERS)

#ifdef ADM_DEBUG_0
#define SCOPE static
#else
#define SCOPE
#endif

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
#if (SETTING_FILE_PNTR_SIZE_IN_BYTES == 2)
/** Setting pointer is two bytes long */
typedef uint16_t SettingSizePointerType;
#else
#error "\n SettingSizePointerType not defined, please check SETTING_FILE_PNTR_SIZE_IN_BYTES and define SettingSizePointerType according to "
#endif 

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

/** Start Address and lenght of Read Ram Request. */
static PP_RRamParamType ReadRamAddresses;

/**
This variable contains the current position in flash from which LookUpTablePointers are stored, these positions must be inside\n
the avaible flash reserved for writing/erasing operations, check BEGIN_MCU_FLASH_MAP-END_MCU_FLASH_MAP.
*/
static FLS_positionType FlsLookUpPntrs;
/**
This is the current position in flash from which LookupTableDataSizes are stored, these positions must be inside\n
the avaible flash reserved for writing/erasing operations, check BEGIN_MCU_FLASH_MAP-END_MCU_FLASH_MAP.
*/
static FLS_positionType FlsLookUpSizes;
/**
This variable contains the current position in flash from which data are stored, these positions must be inside\n
the avaible flash reserved for writing/erasing operations, check BEGIN_MCU_FLASH_MAP-END_MCU_FLASH_MAP.
*/
static FLS_positionType FlsDst;
/**
ADM Manager Machine States 
*/
static enum ADM_ManagerStateTag
{
    /** Clear, if needed, flash reserved to the module  */
    ADM_CLEAR_AVAIBLE_FLASH=0, 
    /** Get all data you need in order to permorm next actions  */  
    ADM_PREREQUISITES_STATE, 
    /** Get Main Setting File  */    
    ADM_GET_SETTING_FILE,
    /** Get LookUp Tables plus LookUp Tables Sizes  */        
    ADM_GET_LOOKUP_TABLES,
    /** Write the cap to ADM LookUp Table */ 
    ADM_WRITE_LOOKUP_CAP,
    /** Nothing to do all job done with success */      
    ADM_IDLE_STATE_JOB_OK,    
    /** Nothing to do end job error encountered */      
    ADM_IDLE_STATE_JOB_KO    
               
    
}ADM_ManagerState;

/**
ADM_Manager Machine SubState: Ident Request 
*/
static enum IdentAcqStateTag
{
    /** Get Ident */
    IDENT_REQ_STATE=0,
    /** Save Ident in Flash */
    IDENT_SAVE_STATE,
    /** Save in flash the pointer where is stored Ident */
    IDENT_SAVE_POINTER_STATE,
    /** Save in flash the Ident's size */    
    IDENT_SAVE_SIZE_STATE,
    /** Error encountered */
    IDENT_ERR_STATE,
    /** Success */
    IDENT_END_STATE
    
}IdentAcqState;
/**
ADM_Manager Machine SubState: Setting File Request 
*/
static enum SettingAcqStateTag
{
    /** Get Setting File Checksum Pointer */
    SETTING_REQ_CHK_PNTR=0,
    /** Get Setting File CheckSum of Appliance */
    SETTING_GET_SETTING_CHKS_STATE,
    /** Compute Setting My Setting File CheckSum  */    
    SETTING_COMP_MY_SETTING_CHKS_STATE,
    /** Get End Of Setting File Pointer */
    SETTING_REQ_EOF_PNTR,
    /** Get Setting File Block */
    SETTING_GET_STATE,
    /** Save in flash Setting File Block */    
    SETTING_SAVE_STATE,
    /** Update params for next block request */
    SETTING_UPDATE_READ_PARAMS_STATE,
    /** Write Setting File Pointer in my LookUp Table */
    SETTING_SAVE_POINTER_STATE,
    /** Write Size in my LookUp Table Size */    
    SETTING_SAVE_SIZE_STATE,
    /** Error */
    SETTING_REQ_ERR_STATE,
    /** End Job */    
    SETTING_REQ_IDLE_STATE
    
}SettingAcqState;
/**
ADM_Manager Machine SubState: LookUp Table Request 
*/
static enum LookUpAcqStateTag
{
    /** Check if LookUp Table is avaible */
    LOOKUP_CHECK_STATE=0,
    /** Request LookUp Table Pointers to the target */
    LOOKUP_GET_POINTERS_STATE,
    /** Save LookUp Table Pointers in A.D.M. Flash */
    LOOKUP_SAVE_POINTERS_STATE,
    /** Verify if LookUp Table Pointers of the target are the same stored in A.D.M. Flash */    
    LOOKUP_VERIFY_POINTERS_STATE,
    /** Update params for next block request */
    LOOKUP_UPDATE_POINTERS_REQ_PARAMS_STATE,
    /** Save pointer to LookUp Table Pointers in A.D.M. Flash */
    LOOKUP_SAVE_LOOKUP_POINTER_STATE,
    /** Save size of LookUp Table Pointers in A.D.M. Flash */    
    LOOKUP_SAVE_LOOKUP_SIZE_STATE,
    /** Set up LookupTableDataSizes Vector Request */
    LOOKUP_SET_LOOKUP_SIZE_PARAMS_STATE,
    /** Request LookupTableDataSizes Vector */
    LOOKUP_GET_LOOKUP_VECTOR_SIZE_STATE,
    /** Save LookupTableDataSizes Vector in A.D.M. Flash */    
    LOOKUP_SAVE_LOOKUP_VECTOR_SIZE_STATE,
    /** Verify if LookupTableDataSizes of the target are the same stored in A.D.M. Flash */        
    LOOKUP_VERIFY_LOOKUP_VECTOR_SIZE_STATE,
    /** Update params for for next block request */
    LOOKUP_UPDATE_LOOKUP_SIZE_REQ_PARAMS_STATE,
    /** Save LookupTableDataSizes pointer in A.D.M. Flash */    
    LOOKUP_SAVE_SIZE_POINTER_STATE,
    /** Write Size in my LookUp Table Size */        
    LOOKUP_SAVE_SIZE_SIZE_STATE,
    /** Error */
    LOOKUP_REQ_ERR_STATE,
    /** Success */
    LOOKUP_IDLE_STATE
    
}LookUpAcqState;
/**
LookUp Write or Verify ?
*/
static bool_t LookUpWrite;
/**
ADM_Manager commands/states
*/
struct ADM_ManagerCmdTag ADM_ManagerCmd;
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */

/* Local Function */
static ADM_ErrorType ADM_DataReq(enum eBCM_MasterRequest request, void *pReqParams, IO_LenType lenReqParams);


/* Exported function */

/* *************** */
/* Function Bodies */
/* *************** */
/**
\n Initialize ADM_Manager() function.
\n This routine must be called before calling ADM_Manager() at cold and hot reset, if supported.

\author 	stefano.ughi 
\date		Jan 13, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by stefano.ughi

*/
void ADM_ManagerInit(void)
{
	FlsLookUpPntrs = START_AVAIBLE_FLASH;
	FlsLookUpSizes = START_LOOKUP_TABLE_SIZES;
	FlsDst = START_STATIC_APPLIANCES_DATA;
	
	IdentAcqState = IDENT_REQ_STATE;
	/* Initialize Request Main Ident */
	ADM_ManagerCmd.mainIdent = 1;
	/* Initialize Request User Interface Ident */
    ADM_ManagerCmd.uiIdent = 1;
	/* Initialize Request Main Setting */
	SettingAcqState = SETTING_REQ_CHK_PNTR;
	ADM_ManagerCmd.mainSettingFile = 1;

	ADM_ManagerCmd.eraseAvaibleFlash = 0;
	
	LookUpAcqState = LOOKUP_CHECK_STATE;
	/* Initialize Request Main LookUp Table. */
	ADM_ManagerCmd.mainLookUpTable = 1;
	/* Initialize Request U.I. LookUp Table. */	
	ADM_ManagerCmd.uiLookUpTable = 1;
	
	/* Initialize ADM_Manager state machine */
	ADM_ManagerState = ADM_PREREQUISITES_STATE;	
}

/**
\n Manager for aquisitions and managing data from Appliances.
\n   

\return 	ADM_ErrorType

\retval		cfr. documentation related to eADM_ERROR

\author 	stefano.ughi 
\date		Jan 7, 2010 
\version	1.0.0

\todo \li	    il puntatore della LookUpTableSize alla fine della procedura non è aggiornato\n
            ma a mio parere si potrebbe eliminare LookUpTableSize con notevole risparmio di codice\n
            in quanto tutte le informazioni necessarie si trovano nella LookUpTablePointers una volta\n
            inserito il "tappo" alla fine della procedura di acquisizione dei campi dati, noti naturalmente\n
            i displacements dei pointers.
\todo \li       debuggare il caso di scaricamento di un file di setting da micro little endian.
   
\note		\par \b Note 1:
			documentation by stefano.ughi

*/
ADM_ErrorType ADM_Manager(void)
{

SCOPE ADM_ErrorType admManagerState;

SCOPE ADM_ErrorType admErr;
SCOPE IO_ErrorType flsErr;
SCOPE LIB_CompChkType compChk;
SCOPE FLS_positionType start;			  
SCOPE FLS_positionType end;	
SCOPE uint16_t settingFileLengthInBytes; 
SCOPE uint16_t lookUpTableLengthInBytes; 
SCOPE uint16_t lookUpTableSizeLengthInBytes;
SCOPE uint8_t *pidentReqLen;


static uint16_t EepAddr;
static uint8_t *pAnswerBuffer;
/* Used for storing num. data requested */
static IO_BufferSizeType incomingDataSize;

/* Pointer to Ident stored in my flash */
static const struct IdentTableStruct *pIdent;
/* Num Blocks to acquire according to MAX_DATA_IN_BYTES_TO_ACQUIRE */
static uint16_t NumBlocksToAcquire;
/* Bytes to be acquired above NumBlocksToAcquire*MAX_DATA_IN_BYTES_TO_ACQUIRE */
#if (MAX_DATA_IN_BYTES_TO_ACQUIRE>255)
static uint16_t NumBytesRem;
#else
static uint8_t NumBytesRem;
#endif /* MAX_DATA_IN_BYTES_TO_ACQUIRE */
/* Type of Request */
static enum eBCM_MasterRequest ReqType;
/* This is the location from which will be store Data in Flash */
static FLS_positionType StartFLSDest;
/* Checksum Position inside Appliance */
static SettingSizePointerType ApplianceCheckSumPosition;
/* In this variable is stored Appliance Setting File CheckSum, it's size is always two bytes */
static uint16_t ApplianceCheckSum;
    
    /* Default return value for this function */
    admManagerState = ADM_E_WORKING;
    switch (ADM_ManagerState)
    {
        case ADM_CLEAR_AVAIBLE_FLASH:
        
        	if (1==ADM_ManagerCmd.eraseAvaibleFlash)
        	{
            	flsErr = FLS_BlanckCheckEraseVerifyAsync(((END_AVAIBLE_FLASH-START_AVAIBLE_FLASH)+1), START_AVAIBLE_FLASH);
            	if ((IO_ErrorType)IO_E_OK == flsErr)
            	{
        			ADM_ManagerInit();
        			/* flag erase operation success */
        			ADM_ManagerCmd.eraseAvaibleFlash = 2;			        		
            	}
       			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
    			{
       				/* flag esase operation failed */
       				ADM_ManagerCmd.eraseAvaibleFlash = 3;
       				ADM_ManagerState = ADM_IDLE_STATE_JOB_KO;
    			}
    			else
    			{
    			    /* wait */
    			}        	    
        	}
            
            break;
            
        case ADM_PREREQUISITES_STATE:
            
        	switch(IdentAcqState)
        	{
        	    case IDENT_REQ_STATE:
        	        
        	        if (1==ADM_ManagerCmd.mainIdent)
        	        {
            	        pidentReqLen = &IdentMainReqLen[0];
            	        pAnswerBuffer = &MainI2CAnswerBuffer[0];
            	        /* This is the  starting address of Main Ident in A.D.M. application */
					    start = (*(FLS_positionType*)(START_LOOKUP_TABLE + (MAIN_IDENT_POINTER_OFFSET*sizeof(FLS_positionType))));
            	        ReqType = PP_IDENT_MAIN_REQ; 
     	            
        	        }
        	        else if(1==ADM_ManagerCmd.uiIdent)
        	        {
            	        pidentReqLen = &IdentUIReqLen[0];
            	        pAnswerBuffer = &UII2CAnswerBuffer[0];
            	        /* This is the  starting address of U.I. Ident in A.D.M. application */
					    start = (*(FLS_positionType*)(START_LOOKUP_TABLE + (UI_IDENT_POINTER_OFFSET*sizeof(FLS_positionType))));      	                    	            
        	            ReqType = PP_IDENT_UI_REQ;
        	        }
        	        else
        	        {
        	            /* go Main Setting File request */
          		        ADM_ManagerState = ADM_GET_SETTING_FILE;          		    	            			                	                	                                    
                        break;
        	        }
        	        
        	        admErr = ADM_DataReq(ReqType,pidentReqLen,1);  

          		    if(ADM_E_OK == admErr)
          		    {
						if ((FLS_positionType)EMPTY_POINTER == start)
						{   
						    /* 
						     No Appliance's Ident avaible:
						     Possibles cases are:
						     1) First time A.D.M. firmware downloaded
						     2) Loss of data
						     3) I come from an erasing operation of A.D.M. flash reserved area
						    */
						    if (2==ADM_ManagerCmd.eraseAvaibleFlash)
						    {
						        /* A.D.M. reserved flash is clear, go on */
						        IdentAcqState = IDENT_SAVE_STATE;    						        
						    }
						    else
						    {
						        /* I don't know if A.D.M. reserved flash is clear, invoke erase flash */
						        ADM_ManagerCmd.eraseAvaibleFlash = 1;
						        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;
						    }
						}
						else
						{
						    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);

						    if ( memcmp((uint8_t*)start,pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize) )
						    {   
						        /* Ident from Appliance not equal to my Ident, I must clear all the avaible 
						        flash for A.D.M. also if Setting is O.K.*/
						        ADM_ManagerCmd.eraseAvaibleFlash = 1;
						        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;
						    }
						    else
						    {   
						        /* Update pointers in flash */
                			    FlsDst+=incomingDataSize;      
    							FlsLookUpPntrs+=sizeof(FLS_positionType);
    							FlsLookUpSizes+=1;
						        IdentAcqState = IDENT_END_STATE;  
						    } 
						}
          		    }
          		    else if(ADM_E_KO == admErr)
          		    {
                        IdentAcqState = IDENT_ERR_STATE;
          		    }
          		    else
          		    {
          		        /* wait for response */
          		    }
        	        
        	        break;
        	    
        	    case IDENT_SAVE_STATE:
                    /* Store Ident in Flash */	   
                    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);
                    flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize, FlsDst);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    IdentAcqState = IDENT_SAVE_POINTER_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        IdentAcqState = IDENT_ERR_STATE;
        			}
        			else
        			{
        			    /* wait */
        			}
        	        
        	        break;
        	        
        	    case IDENT_SAVE_POINTER_STATE:		
                    /* Store Ident Pointer in LookUp */
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&FlsDst,sizeof(FLS_positionType), FlsLookUpPntrs);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    IdentAcqState = IDENT_SAVE_SIZE_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        IdentAcqState = IDENT_ERR_STATE;
        			}
        			else
        			{
        			    /* wait */
        			}
        	        break;
        	        
        	    case IDENT_SAVE_SIZE_STATE:		
                    /* Store Main Ident Size in LookUp, only low byte! */
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&incomingDataSize+1,1, FlsLookUpSizes);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    /* Update pointers in flash */
        			    FlsDst+=incomingDataSize;      
						FlsLookUpPntrs+=sizeof(FLS_positionType);
						FlsLookUpSizes+=1;
        			    IdentAcqState = IDENT_END_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        IdentAcqState = IDENT_ERR_STATE;
        			}      
        			else
        			{
        			    /* wait */
        			}
        	        break;
        	    
        	    case IDENT_END_STATE:
                    /* Wich request I processed ? */
                    if( PP_IDENT_MAIN_REQ == ReqType )
                    {
    					/* flag Main Ident O.K. */
                        ADM_ManagerCmd.mainIdent=2;
                    }
                    if( PP_IDENT_UI_REQ == ReqType )
                    {
    					/* flag U.I.Ident O.K. */
                        ADM_ManagerCmd.uiIdent=2;
                    }
                    IdentAcqState = IDENT_REQ_STATE;                                               
                    
        	        break;
        	    
        	    case IDENT_ERR_STATE:
                    /* Wich request I processed ? */
                    if( PP_IDENT_MAIN_REQ == ReqType )
                    {
    					/* flag Main Ident O.K. */
                        ADM_ManagerCmd.mainIdent=3;
                    }
                    if( PP_IDENT_UI_REQ == ReqType )
                    {
    					/* flag U.I.Ident O.K. */
                        ADM_ManagerCmd.uiIdent=3;
                    }
      		        ADM_ManagerState = ADM_IDLE_STATE_JOB_KO;
      		        break;

        	    default:
        	        /* Initialize Machine's State */
        	        IdentAcqState = IDENT_REQ_STATE;
        	        break;
        	}
            break;
            
        case ADM_GET_SETTING_FILE:
        
            if(1==ADM_ManagerCmd.mainSettingFile)
            {
                /* We assume that Ident is avaible */
                switch(SettingAcqState)
                {   
                    case SETTING_REQ_CHK_PNTR:
                        /* Save the Start Address from which will be store Setting File */
                        StartFLSDest = FlsDst;
                        /* Point to Main Ident Table stored in my flash */ 
                        pIdent = (struct IdentTableStruct *)(*(FLS_positionType*)(START_LOOKUP_TABLE+MAIN_IDENT_POINTER_OFFSET*sizeof(FLS_positionType)));
						/* Check if Main support C.R.C.*/
						if (pIdent->crcReady)
						{
						    ReqType = PP_RRAM_CRC_MAIN_REQ;    
						}
						else
						{
						    ReqType = PP_RRAM_MAIN_REQ;    
						}
                       
                        /* Get Location where is stored CheckSum File Pointer */
                        if (pIdent->littleEndian) 
                        {
                            EepAddr = LIB_SwapWord(pIdent->EEPaddress);    
                        }
                        else
                        {
                            EepAddr = pIdent->EEPaddress;   
                        }
                        ReadRamAddresses.addr.data = (EepAddr+(SETTING_FILE_PNTR_CHK_OFFSET*sizeof(SettingSizePointerType)));
                        ReadRamAddresses.len  = 2;
            	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
              		    if(ADM_E_OK == admErr)
              		    {
              		        /* Here is stored Setting File CheckSumPosition on Main Appliance */
              		        ApplianceCheckSumPosition = (EepAddr + LIB_ConvTwoByteInWord(&MainI2CAnswerBuffer[PP_OUT_BUFF_RESERVED_SZ])*sizeof(SettingSizePointerType));
              		        
              		        SettingAcqState = SETTING_GET_SETTING_CHKS_STATE;
              		    }
              		    else if(ADM_E_KO == admErr)
              		    {
                            SettingAcqState = SETTING_REQ_ERR_STATE;
              		    }
              		    else
              		    {
              		        /* wait for response */
              		    }
                       
                        break;
                    
                    case SETTING_GET_SETTING_CHKS_STATE:
                        /* Get Appliance Setting File Checksum, ever big endian */
                        ReadRamAddresses.addr.data = ApplianceCheckSumPosition;
                        ReadRamAddresses.len  = 2;
            	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
              		    if(ADM_E_OK == admErr)
              		    {
              		        /* This is the Setting File CheckSum of the Appliance */
              		        ApplianceCheckSum = LIB_ConvTwoByteInWord(&MainI2CAnswerBuffer[PP_OUT_BUFF_RESERVED_SZ]);
              		        SettingAcqState = SETTING_COMP_MY_SETTING_CHKS_STATE;
              		    }
              		    else if(ADM_E_KO == admErr)
              		    {
                            SettingAcqState = SETTING_REQ_ERR_STATE;
              		    }
              		    else
              		    {
              		        /* wait for response */
              		    }
                    
                        break;
                    
                    case SETTING_COMP_MY_SETTING_CHKS_STATE:
                        /* This is the  starting address of Appliance Setting in my application */
					    start = (*(FLS_positionType*)(START_LOOKUP_TABLE + (MAIN_SETTING_POINTER_OFFSET*sizeof(FLS_positionType))));
						if ((FLS_positionType)EMPTY_POINTER == start)
						{
					        /* 
						     No Appliance's Setting avaible:
						     Possibles cases are:
						     1) First time A.D.M. firmware downloaded
						     2) Loss of data
						     3) I come from an erasing operation of A.D.M. flash reserved area
						    */
						    if (2==ADM_ManagerCmd.eraseAvaibleFlash)
						    {   
						        /* A.D.M. reserved flash is clear, go on */
						        SettingAcqState = SETTING_REQ_EOF_PNTR;
						    }
						    else
						    {   
						        /* I don't know if A.D.M. reserved flash is clear, invoke erase flash */
						        ADM_ManagerCmd.eraseAvaibleFlash = 1;
						        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;
						    }
						}
						else
						{
    						/* This is the pointer to the checksum */
    						end = *(FLS_positionType*)(start + SETTING_FILE_PNTR_CHK_OFFSET*sizeof(FLS_positionType));
    						/* This is the  ending address of Appliance Setting in my application under checksum control */
    						end = (start+(end*sizeof(FLS_positionType)));
    						/* Compute checksum of setting file stored in my application */		
    						compChk = LIB_ComputeSettingFileCheckSum(start,end);
                            if (compChk.jobDone)
                            {
                                #if defined (ADM_DEBUG_0)
                                {
                                    if (FailChk)
                                    {
                                        compChk.checkSum++;
                                        FailChk = FALSE;
                                    }
                                }
                                #endif /* ADM_DEBUG_0 */
                                if (compChk.checkSum == ApplianceCheckSum)
                                {   
                                    /* Restore FLSDest */
                                    /* This is the pointer to the E.O.F. */
            						end = *(FLS_positionType*)(start + SETTING_FILE_PNTR_EOF_OFFSET*sizeof(FLS_positionType));
            						/* This is the  ending address of Appliance Setting in my application */
            						end = (start+(end*sizeof(FLS_positionType)));
                                    /* Update pointers in flash */
        							FlsLookUpPntrs+=sizeof(FLS_positionType);
        							FlsLookUpSizes+=1;
        							FlsDst = end;
                                    SettingAcqState = SETTING_REQ_IDLE_STATE;    
                                }
                                else
                                {   
                                    /* Setting changed, restart !! */
    						        ADM_ManagerCmd.eraseAvaibleFlash = 1;
    						        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;
                                }
                            }						    
						}
                        
                        break;
                    
                    case SETTING_REQ_EOF_PNTR:
                        /* Request End Of Setting File Pointer */
                        ReadRamAddresses.addr.data = (EepAddr+(SETTING_FILE_PNTR_EOF_OFFSET*sizeof(SettingSizePointerType)));
                        ReadRamAddresses.len = 2;
            	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
              		    if(ADM_E_OK == admErr)
              		    {
              		        settingFileLengthInBytes = (LIB_ConvTwoByteInWord(&MainI2CAnswerBuffer[PP_OUT_BUFF_RESERVED_SZ])*sizeof(SettingSizePointerType)); 
              		        
              		        NumBlocksToAcquire = (settingFileLengthInBytes/MAX_DATA_IN_BYTES_TO_ACQUIRE);
              		        NumBytesRem = (settingFileLengthInBytes%MAX_DATA_IN_BYTES_TO_ACQUIRE);
              		        
              		        ReadRamAddresses.addr.data = EepAddr; 
              		        if (NumBlocksToAcquire)
              		        {
              		            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;    
              		        }
              		        else
              		        {
              		            ReadRamAddresses.len = NumBytesRem;
              		            NumBytesRem = 0;    
              		        }
              		        SettingAcqState = SETTING_GET_STATE;
              		    }
              		    else if(ADM_E_KO == admErr)
              		    {
                            SettingAcqState = SETTING_REQ_ERR_STATE;
              		    }
              		    else
              		    {
              		        /* wait for response */
              		    }
                    
                        break;
                    
                    case SETTING_GET_STATE:
                        
            	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
              		    if(ADM_E_OK == admErr)
              		    {                  		        
              		        SettingAcqState = SETTING_SAVE_STATE;
              		    }
              		    else if(ADM_E_KO == admErr)
              		    {
                            SettingAcqState = SETTING_REQ_ERR_STATE;
              		    }
              		    else
              		    {
              		        /* wait for response */
              		    }                            
                    
                        break;
                    
                    case SETTING_SAVE_STATE:
                        /* Store Setting File block in Flash. */
                        incomingDataSize = LIB_ConvTwoByteInWord(&MainI2CAnswerBuffer[0]);
                        
                        flsErr = FLS_WriteAndVerifyAsync(&MainI2CAnswerBuffer[PP_OUT_BUFF_RESERVED_SZ],incomingDataSize, FlsDst);
            			if((IO_ErrorType)IO_E_OK == flsErr)
            			{
            			    FlsDst+=incomingDataSize;      
            			    SettingAcqState = SETTING_UPDATE_READ_PARAMS_STATE;
            			}
            			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
            			{
                            SettingAcqState = SETTING_REQ_ERR_STATE;
            			}
            			else
            			{
            			    /* wait for writing */
            			}
            			
						break;

                    case SETTING_UPDATE_READ_PARAMS_STATE:
                    
            	        if ( NumBlocksToAcquire > 1 )
            	        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
            	            ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
            	            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
            	            NumBlocksToAcquire--;    
                  		    SettingAcqState = SETTING_GET_STATE;
            	        }
            	        else if (NumBytesRem)
            	        {
                            ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
                            ReadRamAddresses.len = NumBytesRem;    
            	            NumBytesRem = 0;
            	            SettingAcqState = SETTING_GET_STATE;
            	        }
                        else
                        {
                            /* End Job */
                      		SettingAcqState = SETTING_SAVE_POINTER_STATE;
                        }

                        break;

            	    case SETTING_SAVE_POINTER_STATE:
                        /* Store Main Setting File Pointer in My LookUp */
                        flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&StartFLSDest,sizeof(FLS_positionType), FlsLookUpPntrs);
            			if((IO_ErrorType)IO_E_OK == flsErr)
            			{
            			    SettingAcqState = SETTING_SAVE_SIZE_STATE;
            			}
            			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
            			{
                            SettingAcqState = SETTING_REQ_ERR_STATE;
            			}
            			else
            			{
            			    /* wait */
            			}
            	        break;
            	        
            	    case SETTING_SAVE_SIZE_STATE:
                        /* Store Main Setting File Size in LookUp (convention '0') */
                        incomingDataSize = 0;
                        flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&incomingDataSize,1, FlsLookUpSizes);
            			if((IO_ErrorType)IO_E_OK == flsErr)
            			{
            			    /* Update pointers in flash */
							FlsLookUpPntrs+=sizeof(FLS_positionType);
							FlsLookUpSizes+=1;
            			    SettingAcqState = SETTING_REQ_IDLE_STATE;
            			}
            			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
            			{
                            SettingAcqState = SETTING_REQ_ERR_STATE;
            			}      
            			else
            			{
            			    /* wait */
            			}
            	        break;
                        
                    case SETTING_REQ_IDLE_STATE:
                        ADM_ManagerCmd.mainSettingFile = 2;
                        ADM_ManagerState = ADM_GET_LOOKUP_TABLES;
                        break;
                    case SETTING_REQ_ERR_STATE:
                        ADM_ManagerCmd.mainSettingFile = 3;
                        ADM_ManagerState = ADM_IDLE_STATE_JOB_KO;
                        break;
                    default:
                        SettingAcqState = SETTING_REQ_CHK_PNTR;
                        break;
                }
            }
           
            break;
                        
        case ADM_GET_LOOKUP_TABLES:
            
            /* We assume that Main Ident Table is avaible in A.D.M. flash */
            switch(LookUpAcqState)
            {
                case LOOKUP_CHECK_STATE:
                    
                    if(1==ADM_ManagerCmd.mainLookUpTable)
                    {
                        /* Get Main LookUp Table Pointer inside A.D.M. LookUp Table */
        			    start = (*(FLS_positionType*)(START_LOOKUP_TABLE + (MAIN_LOOKUP_POINTER_OFFSET*sizeof(FLS_positionType))));
    			        /* Point to Main Ident Table stored in my flash */ 
                        pIdent = (struct IdentTableStruct *)(*(FLS_positionType*)(START_LOOKUP_TABLE+MAIN_IDENT_POINTER_OFFSET*sizeof(FLS_positionType)));
						/* Check if Main support C.R.C.*/
						if (pIdent->crcReady)
						{
						    ReqType = PP_RRAM_CRC_MAIN_REQ;    
						}
						else
						{
						    ReqType = PP_RRAM_MAIN_REQ;    
						}
						pAnswerBuffer = &MainI2CAnswerBuffer[0];
                    }
                    else if(1==ADM_ManagerCmd.uiLookUpTable)
                    {
                        /* Get U.I. LookUp Table Pointer inside A.D.M. LookUp Table */
        			    start = (*(FLS_positionType*)(START_LOOKUP_TABLE + (UI_LOOKUP_POINTER_OFFSET*sizeof(FLS_positionType))));
    			        /* Point to U.I. Ident Table stored in my flash */ 
                        pIdent = (struct IdentTableStruct *)(*(FLS_positionType*)(START_LOOKUP_TABLE+UI_IDENT_POINTER_OFFSET*sizeof(FLS_positionType)));
						/* Check if Main support C.R.C.*/
						if (pIdent->crcReady)
						{
						    ReqType = PP_RRAM_CRC_UI_REQ;    
						}
						else
						{
						    ReqType = PP_RRAM_UI_REQ;    
						}
						pAnswerBuffer = &UII2CAnswerBuffer[0];

                    }
                    else
                    {
                        /* End Job Write the cap in ADM LookUp Table */
                        ADM_ManagerState = ADM_WRITE_LOOKUP_CAP;
                        break;                    
                    }
			        /* Save the Start Address from which will be store LookupTablePointers if they are not present
			           in A.D.M. flash */
                    StartFLSDest = FlsDst;

                    /* Calculate # of bytes to request */
                    lookUpTableLengthInBytes = (pIdent->LUTsize*LUT_ITEM_SIZE_IN_BYTES); 

      		        NumBlocksToAcquire = (lookUpTableLengthInBytes/MAX_DATA_IN_BYTES_TO_ACQUIRE);
      		        NumBytesRem = (lookUpTableLengthInBytes%MAX_DATA_IN_BYTES_TO_ACQUIRE);
      		        /* Set up Read Ram request */
      		        if (pIdent->littleEndian)
      		        {
      		            ReadRamAddresses.addr.data = LIB_SwapWord(pIdent->LUTaddress);
      		        }
      		        else
      		        {
    			        ReadRamAddresses.addr.data = (pIdent->LUTaddress);      		            
      		        }
      		        if (NumBlocksToAcquire)
      		        {
      		            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;    
      		        }
      		        else
      		        {
      		            ReadRamAddresses.len = NumBytesRem;
      		            NumBytesRem = 0;    
      		        }

    				if ((FLS_positionType)EMPTY_POINTER == start)
    				{
    			        /* 
    				     No LookUp Table Pointer avaible:
    				     Possibles cases are:
    				     1) First time A.D.M. firmware downloaded
    				     2) Loss of data
    				     3) I come from an erasing operation of A.D.M. flash reserved area
    				    */
    				    if (2==ADM_ManagerCmd.eraseAvaibleFlash)
    				    {   
    				        /* A.D.M. reserved flash is clear, go on */
                            LookUpWrite = TRUE;
    				        LookUpAcqState = LOOKUP_GET_POINTERS_STATE;
    				    }
    				    else
    				    {   
    				        /* I don't know if A.D.M. reserved flash is clear, invoke erase flash */
    				        ADM_ManagerCmd.eraseAvaibleFlash = 1;
    				        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;
    				    }
    				}
    				else
    				{
    				    /* Check if my LookUp is equal to target's LookUp */
    				    LookUpWrite = FALSE;
    				    LookUpAcqState = LOOKUP_GET_POINTERS_STATE;
    				}                
                
                    break;  
                                      
                case LOOKUP_GET_POINTERS_STATE:
        	        /* Request LookupTablePointers */
        	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
          		    if(ADM_E_OK == admErr)
          		    {   
          		        if (LookUpWrite)
          		        {
              		        LookUpAcqState = LOOKUP_SAVE_POINTERS_STATE;              		            
          		        }
          		        else
          		        {
          		            LookUpAcqState = LOOKUP_VERIFY_POINTERS_STATE;
          		        }
          		    }
          		    else if(ADM_E_KO == admErr)
          		    {
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
          		    }
          		    else
          		    {
          		        /* wait for response */
          		    }                            
                    
                    break;
                
                case LOOKUP_VERIFY_POINTERS_STATE:
				    /* Verify if LookupTablePointers are the same of A:D.M. flash */
				    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);
				    if ( memcmp((void*)FlsDst,pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize) )
				    {
				        /* LookUp pointers in A.D.M. flash are not equal to target LookUp Pointers */
				        ADM_ManagerCmd.eraseAvaibleFlash = 1;
				        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;					        
				    }
				    else
				    {
        			    FlsDst+=incomingDataSize;      
        			    LookUpAcqState = LOOKUP_UPDATE_POINTERS_REQ_PARAMS_STATE;					        
				    }
                    break;
                
                case LOOKUP_SAVE_POINTERS_STATE:
                    /* Store LookUp Table block in A.D.M. Flash. */
                    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);
                    
                    flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize, FlsDst);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    FlsDst+=incomingDataSize;      
        			    LookUpAcqState = LOOKUP_UPDATE_POINTERS_REQ_PARAMS_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}
        			else
        			{
        			    /* wait for writing */
        			}
                
                    break;
                    
                case LOOKUP_UPDATE_POINTERS_REQ_PARAMS_STATE:
        	        
        	        if ( NumBlocksToAcquire > 1 )
        	        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
        	            ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        	            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
        	            NumBlocksToAcquire--;    
              		    LookUpAcqState = LOOKUP_GET_POINTERS_STATE;
        	        }
        	        else if (NumBytesRem)
        	        {
                        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
                        ReadRamAddresses.len = NumBytesRem;    
        	            NumBytesRem = 0;
        	            LookUpAcqState = LOOKUP_GET_POINTERS_STATE;
        	        }
                    else
                    {
                        
                        if (LookUpWrite)
                        {
                            /* End Storing LookUp Table pointers in A.D.M. Flash. */
                      		LookUpAcqState = LOOKUP_SAVE_LOOKUP_POINTER_STATE;                            
                        }
                        else
                        {
                            /* End Verifing LookUp Table pointers in A.D.M. Flash. */
                            /* Update pointers in flash */
							FlsLookUpPntrs+=sizeof(FLS_positionType);
							FlsLookUpSizes+=1;
							LookUpAcqState = LOOKUP_SET_LOOKUP_SIZE_PARAMS_STATE;
                        }
                    }
                
                    break;
                
                case LOOKUP_SAVE_LOOKUP_POINTER_STATE:
                
                    /* Store Main LookUp Pointer in A.D.M. LookUp */
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&StartFLSDest,sizeof(FLS_positionType), FlsLookUpPntrs);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    LookUpAcqState = LOOKUP_SAVE_LOOKUP_SIZE_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}
        			else
        			{
        			    /* wait */
        			}
                    
                    break;
                
                case LOOKUP_SAVE_LOOKUP_SIZE_STATE:

                    /* Store LookUp Table Size in my LookUp (convention: size in words) */
                    incomingDataSize = pIdent->LUTsize;
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&incomingDataSize+1,1, FlsLookUpSizes);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    /* Update pointers in flash */
						FlsLookUpPntrs+=sizeof(FLS_positionType);
						FlsLookUpSizes+=1;
        			    LookUpAcqState = LOOKUP_SET_LOOKUP_SIZE_PARAMS_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}      
        			else
        			{
        			    /* wait */
        			}
                    break;
                
                case LOOKUP_SET_LOOKUP_SIZE_PARAMS_STATE:
                    /* Save the Start Address from which will be store LookupTableDataSizes */
                    StartFLSDest = FlsDst;

                    /* Define how many data to request for LookupTableDataSizes */                        
                    lookUpTableSizeLengthInBytes = (pIdent->LUTsize-1);
      		        NumBlocksToAcquire = (lookUpTableSizeLengthInBytes/MAX_DATA_IN_BYTES_TO_ACQUIRE);
      		        NumBytesRem = (lookUpTableSizeLengthInBytes%MAX_DATA_IN_BYTES_TO_ACQUIRE);
                    
                    if(1==ADM_ManagerCmd.mainLookUpTable)
                    {
                        /* This is the address of Main LookupTableDataSizes */                                 		        
          		        ReadRamAddresses.addr.data = *(uint16_t*)(FlsDst-LUT_ITEM_SIZE_IN_BYTES);                    
                    }
                    else if(1==ADM_ManagerCmd.uiLookUpTable)
                    {
                        /* This is the address of User Interface LookupTableDataSizes */                                 		        
          		        ReadRamAddresses.addr.data = *(uint16_t*)(FlsDst-((pIdent->LUTsize)*LUT_ITEM_SIZE_IN_BYTES));                        
                    }
                    else
                    {
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
                        break;
                    }
                    if (pIdent->littleEndian)
                    {
                        ReadRamAddresses.addr.data = LIB_SwapWord(ReadRamAddresses.addr.data);    
                    }
      		        
      		        if (NumBlocksToAcquire)
      		        {
      		            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;    
      		        }
      		        else
      		        {
      		            ReadRamAddresses.len = NumBytesRem;
      		            NumBytesRem = 0;    
      		        }
                    LookUpAcqState = LOOKUP_GET_LOOKUP_VECTOR_SIZE_STATE;
                    
                    break;
                
                case LOOKUP_GET_LOOKUP_VECTOR_SIZE_STATE:
        	        /* Request Main LookupTableDataSizes */
        	        admErr = ADM_DataReq(ReqType,&ReadRamAddresses,sizeof(PP_RRamParamType));
          		    if(ADM_E_OK == admErr)
          		    {                  		        
          		        if (LookUpWrite)
          		        {
              		        LookUpAcqState = LOOKUP_SAVE_LOOKUP_VECTOR_SIZE_STATE;          		            
          		        }
          		        else
          		        {
          		            LookUpAcqState = LOOKUP_VERIFY_LOOKUP_VECTOR_SIZE_STATE;
          		        }
          		    }
          		    else if(ADM_E_KO == admErr)
          		    {
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
          		    }
          		    else
          		    {
          		        /* wait for response */
          		    }                            
                                    
                    break;
                
                case LOOKUP_VERIFY_LOOKUP_VECTOR_SIZE_STATE:
                     /* Verify if LookupTableDataSizes are the same of A:D.M. flash */
                    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);
				    if ( memcmp((void*)FlsDst,pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize) )
				    {
				        /* LookupTableDataSizes in A.D.M. flash are not equal to target LookupTableDataSizes */
				        ADM_ManagerCmd.eraseAvaibleFlash = 1;
				        ADM_ManagerState = ADM_CLEAR_AVAIBLE_FLASH;					        
				    }
				    else
				    {
        			    FlsDst+=incomingDataSize;      
        			    LookUpAcqState = LOOKUP_UPDATE_LOOKUP_SIZE_REQ_PARAMS_STATE;					        
				    }

                    break;
                
                case LOOKUP_SAVE_LOOKUP_VECTOR_SIZE_STATE:
                    /* Store LookupTableDataSizes in Flash. */
                    incomingDataSize = LIB_ConvTwoByteInWord(pAnswerBuffer);
                    
                    flsErr = FLS_WriteAndVerifyAsync(pAnswerBuffer+PP_OUT_BUFF_RESERVED_SZ,incomingDataSize, FlsDst);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    FlsDst+=incomingDataSize;      
        			    LookUpAcqState = LOOKUP_UPDATE_LOOKUP_SIZE_REQ_PARAMS_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}
        			else
        			{
        			    /* wait for writing */
        			}                  
                    break;
                
                case LOOKUP_UPDATE_LOOKUP_SIZE_REQ_PARAMS_STATE:
        	        
        	        if ( NumBlocksToAcquire > 1 )
        	        {   /* N.B.: NumBlocksToAcquire > 1 because first block already read */
        	            ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
        	            ReadRamAddresses.len = MAX_DATA_IN_BYTES_TO_ACQUIRE;
        	            NumBlocksToAcquire--;    
              		    LookUpAcqState = LOOKUP_GET_LOOKUP_VECTOR_SIZE_STATE;
        	        }
        	        else if (NumBytesRem)
        	        {
                        ReadRamAddresses.addr.data += MAX_DATA_IN_BYTES_TO_ACQUIRE;
                        ReadRamAddresses.len = NumBytesRem;    
        	            NumBytesRem = 0;
        	            LookUpAcqState = LOOKUP_GET_LOOKUP_VECTOR_SIZE_STATE;
        	        }
                    else
                    {
                        if (LookUpWrite)
                        {
                            /* End Storing LookupTableDataSizes in A.D.M. Flash. */
                      		LookUpAcqState = LOOKUP_SAVE_SIZE_POINTER_STATE;                            
                        }
                        else
                        {
                            /* End Verifing LookupTableDataSizes in A.D.M. Flash. */
                            /* Update pointers in flash */
							FlsLookUpPntrs+=sizeof(FLS_positionType);
							FlsLookUpSizes+=1;
							LookUpAcqState = LOOKUP_IDLE_STATE;
                        } 
                    }
                    
                    break;
                
                case LOOKUP_SAVE_SIZE_POINTER_STATE:
                    /* Store LookUpTableSize Pointer in My LookUp */
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&StartFLSDest,sizeof(FLS_positionType), FlsLookUpPntrs);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    LookUpAcqState = LOOKUP_SAVE_SIZE_SIZE_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}
        			else
        			{
        			    /* wait */
        			}

                    break;
                
                case LOOKUP_SAVE_SIZE_SIZE_STATE:
                    /* Store LookupTableDataSizes Vector Size in my LookUp */
                    incomingDataSize = pIdent->LUTsize-1;
                    flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&incomingDataSize+1,1, FlsLookUpSizes);
        			if((IO_ErrorType)IO_E_OK == flsErr)
        			{
        			    /* Update pointers in flash */
						FlsLookUpPntrs+=sizeof(FLS_positionType);
						FlsLookUpSizes+=1;
        			    LookUpAcqState = LOOKUP_IDLE_STATE;
        			}
        			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
        			{
                        LookUpAcqState = LOOKUP_REQ_ERR_STATE;
        			}      
        			else
        			{
        			    /* wait */
        			}
                
                    break;
                
                case LOOKUP_IDLE_STATE:
                    /* Wich request I processed ? */
                    if( (PP_RRAM_CRC_MAIN_REQ == ReqType) || (PP_RRAM_MAIN_REQ == ReqType) )
                    {
    					/* flag Main LookUp O.K. */
                        ADM_ManagerCmd.mainLookUpTable = 2;
                    }
                    if( (PP_RRAM_CRC_UI_REQ == ReqType) || (PP_RRAM_UI_REQ == ReqType) )
                    {
    					/* flag U.I.LookUp O.K. */
                        ADM_ManagerCmd.uiLookUpTable = 2;
                    }
                    LookUpAcqState = LOOKUP_CHECK_STATE;                                               

                    break;
                
                case LOOKUP_REQ_ERR_STATE:
                    /* Wich request I processed ? */
                    if( (PP_RRAM_CRC_MAIN_REQ == ReqType) || (PP_RRAM_MAIN_REQ == ReqType) )
                    {
    					/* flag Main LookUp K.O. */
                        ADM_ManagerCmd.mainLookUpTable = 3;
                    }
                    if( (PP_RRAM_CRC_UI_REQ == ReqType) || (PP_RRAM_UI_REQ == ReqType) )
                    {
    					/* flag U.I.LookUp K.O. */
                        ADM_ManagerCmd.uiLookUpTable = 3;
                    }
                    ADM_ManagerState = ADM_IDLE_STATE_JOB_KO;                    
                    break;
                default:
                    LookUpAcqState = LOOKUP_CHECK_STATE;
                    break;
            }
        
            break;
        
        case ADM_WRITE_LOOKUP_CAP:
            
            flsErr = FLS_WriteAndVerifyAsync((FLS_SourcePtrType)&FlsDst,sizeof(FLS_positionType), FlsLookUpPntrs);
			if((IO_ErrorType)IO_E_OK == flsErr)
			{
			    ADM_ManagerState = ADM_IDLE_STATE_JOB_OK;                    
			}
			else if((IO_ErrorType)IO_E_UNEXPECTED_CONDITION == flsErr)
			{
                ADM_ManagerState = ADM_IDLE_STATE_JOB_KO;                    
			}
			else
			{
			    /* wait */
			}

            break;
        case ADM_IDLE_STATE_JOB_KO:
            admManagerState = ADM_E_KO;
            break;
        case ADM_IDLE_STATE_JOB_OK:
            admManagerState = ADM_E_OK;
            break;
        default:
            ADM_ManagerState = ADM_PREREQUISITES_STATE;
            break;
    }
    return(admManagerState);
}
/**
This function return the state of all actions performed by ADM_Manager(). 

\return 	struct ADM_ManagerCmdTag.

\retval		cfr. ADM_ManagerCmdTag documentation.

\author 	stefano.ughi 
\date		Feb 03, 2010 
\version	1.0.0
   
\note		\par \b Note 1:
			documentation by stefano.ughi

*/

struct ADM_ManagerCmdTag ADM_MangerGetState(void)
{
    return (ADM_ManagerCmd);    
}

/* Local Function */

/**
This function perform a generic requests on the bus. 

\param  [in]		request request type Eg. PP_IDENT_MAIN_REQ
\param  [in] 	    pReqParams pointer to the paramenters's request
\param  [in] 	    lenReqParams size in bytes of the pReqParams.

\return 	ADM_ErrorType

\retval		ADM_E_OK work done
\retval		ADM_E_WORKING work in progress
\retval		ADM_E_KO problem occurred		 	 

\author 	stefano.ughi 
\date		Dec 16, 2009 
\version	1.0.0

\todo		implementing a timeout for the response (check Keyword ToCheck)
   
\note		\par \b Note 1:
			documentation by stefano.ughi

*/
static ADM_ErrorType ADM_DataReq(enum eBCM_MasterRequest request, void *pReqParams, IO_LenType lenReqParams)
{
IO_ErrorType result;
ADM_ErrorType retValue;

    /* 0) Set Params for Request */
    #if 0
    (void)BCM_SetPacketParam(request, pReqParams, lenReqParams);
    #endif
    (void)CRM_SetPacketParamSync((IO_ChannelType)BCP_I2C_CH_ID, request, pReqParams, lenReqParams);

    /* 1) Append request to the queue */
    #if 0
    (void)BCM_SetPacketRequest(request);
    #endif
    (void)CRM_SetPacketRequestSync((IO_ChannelType)BCP_I2C_CH_ID, request);

    /* 2) Verify the request state */
    #if 0
    result = BCM_TestRequest(request);
    #endif
    result = BCM_TestRequest((IO_ChannelType)BCP_I2C_CH_ID, request);

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

/* *********** */
/* End of file */
/* *********** */
