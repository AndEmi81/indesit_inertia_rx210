/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n	Il file contiene le funzioni per la gestione delle variabili di classe B
\n
\file		LIB_ClassBVarBck.h
\ingroup   grpDataCare
\author		Marco Sperandini
\n
*/


#ifndef __CLASSBVAR_H_
#define __CLASSBVAR_H_

/* ******** */
/* Includes */
/* ******** */
#include "Config.h"

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
#if defined(CONFIG_CLASS_B_DATA_INTEGRITY) && defined(CONFIG_CLASS_B_DATA_BACKUP)

#include "DataSafety_cfg.h"

typedef struct Sec_DataCheck {
	uint16_t  id;
	uint16_t  dataLen;
	uint8_t*  dataPtr;
} Sec_DataCheck_t;

uint8_t Sec_Backup_SecDataSave (void);
uint8_t Sec_Backup_SecDataRestore (void);
uint8_t Sec_Backup_SecDataInvalidate (void);

#define SEC_INIT_BCK_ENTRY(name) {SEC_ID_##name, sizeof(name),&name }

/*Sec_DataCheck */
extern const Sec_DataCheck_t SecBackUpTable[];

#endif /* defined(CONFIG_CLASS_B_DATA_INTEGRITY)  &&  defined(CONFIG_CLASS_B_DATA_BACKUP)*/


#endif /* __CLASSBVAR_H_ */

/* *********** */
/* End of file */
/* *********** */
