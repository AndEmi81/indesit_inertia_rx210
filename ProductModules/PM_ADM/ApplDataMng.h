/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		ApplDataMng.h 
\n			Header File of ApplDataMng.c
\n
\file		ApplDataMng.h 
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

#ifndef __APPLDATAMNG_H
#define __APPLDATAMNG_H

/* ********** */
/* Includes */
/* ********* */
#include "Config.h"
#include "Hal_Interface.h"
#include HAL_CFG
#include "IO_driver.h"

#include "BCP_General.h"
#include "BusCommPltf_cfg.h"
#include "BCP_BusCommMng.h"

#include "ApplDataMng_cfg.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */


/**
\enum eADM_ERROR
*/
enum eADM_ERROR{
    /** Success */
    ADM_E_OK=0,
    /** Working */
    ADM_E_WORKING,
    /** Fail */
    ADM_E_KO
};

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef enum eADM_ERROR ADM_ErrorType;
/**
ADM_Manager commands/states
*/
typedef struct ADM_ManagerCmdTag
{
    /**
\n\li if '0' Do nothing
\n\li if '1' Get Main Ident 
\n\li if '2' Main Ident ready and saved in MCU's flash
\n\li if '3' Problem occurred 
    */
    bool_t mainIdent:2;
    /**
\n\li if '0' Do nothing
\n\li if '1' Get Main Setting File 
\n\li if '2' Main Setting File ready and saved in MCU's flash
\n\li if '3' Problem occurred 
    */
    bool_t mainSettingFile:2;
    /**
\n\li if '0' Do nothing
\n\li if '1' Erase avaible A.D.M. flash data
\n\li if '2' Erase operation on A.D.M. flash data success
\n\li if '3' Problem occurred 
    */
    bool_t eraseAvaibleFlash:2;     
    /**
\n\li if '0' Do nothing
\n\li if '1' Get Main LookUp Table 
\n\li if '2' Main LookUp Table ready and saved in MCU's flash
\n\li if '3' Problem occurred 
    */
    bool_t mainLookUpTable:2;
    /**
\n\li if '0' Do nothing
\n\li if '1' Get User Interface Ident 
\n\li if '2' User Interface Ident ready and saved in MCU's flash
\n\li if '3' Problem occurred 
    */
    bool_t uiIdent:2;
    /**
\n\li if '0' Do nothing
\n\li if '1' Get U.I. LookUp Table 
\n\li if '2' U.I. LookUp Table ready and saved in MCU's flash
\n\li if '3' Problem occurred 
    */
    bool_t uiLookUpTable:2;   
};

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void ADM_ManagerInit(void);
ADM_ErrorType ADM_Manager(void);
struct ADM_ManagerCmdTag ADM_MangerGetState(void);

#endif /* __APPLDATAMNG_H */

/* *********** */
/* End of file */
/* *********** */


