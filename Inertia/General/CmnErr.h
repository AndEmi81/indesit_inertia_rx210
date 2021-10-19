/**
\defgroup	CmnErr Modulo Common Errors Defines
\ingroup	ApplGeneral

Il modulo contiene le definizioni dei codici di errore dell'applicazione.
*/

/**
Header file del modulo Common Errors Defines

\file    CmnErr.h
\ingroup CmnErr
\par \b PCLint: x Errors, x Warnings, x Info 14/01/2008 Valeria Catani


\date    14/01/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplGeneral
\date    14/01/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __CMNERR_H
#define __CMNERR_H

/* ******** */
/* Includes */
/* ******** */
#include "APP_Err.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/**/

/* Macro per la generazione dei codici di errore */
/**
Codici iniziali dei moduli
*/

#define HIO_WRN_START_CODE						0x28	/**6 max */
#define HIO_ERR_START_CODE						(ERR_CODE + HIO_WRN_START_CODE)  

#define HND_WRN_START_CODE						0x2E	/**26 max*/
#define HND_ERR_START_CODE						(ERR_CODE + HND_WRN_START_CODE) 

#define REG_WRN_START_CODE						0x48	/**6 max */
#define REG_ERR_START_CODE						(ERR_CODE + REG_WRN_START_CODE) 

#define UP_LOA_WRN_START_CODE					0x4E	/**4 max*/
#define UP_LOA_ERR_START_CODE					(ERR_CODE + UP_LOA_WRN_START_CODE)  

#define UP_LOR_WRN_START_CODE					0x52	/**4 max*/
#define UP_LOR_ERR_START_CODE					(ERR_CODE + UP_LOR_WRN_START_CODE) 

#define UP_HEA_WRN_START_CODE					0x58	/**4 max*/
#define UP_HEA_ERR_START_CODE					(ERR_CODE + UP_HEA_WRN_START_CODE) 

#define UP_PMP_WRN_START_CODE					0x5C	/**4 max*/
#define UP_PMP_ERR_START_CODE					(ERR_CODE + UP_PMP_WRN_START_CODE) 

#define UP_ALT_WRN_START_CODE					0x60	/**4 max*/
#define UP_ALT_ERR_START_CODE					(ERR_CODE + UP_ALT_WRN_START_CODE) 

#define UP_LDC_WRN_START_CODE					0x64	/**4 max*/
#define UP_LDC_ERR_START_CODE					(ERR_CODE + UP_LDC_WRN_START_CODE) 

#define UP_SPA_WRN_START_CODE					0x68	/**4 max*/
#define UP_SPA_ERR_START_CODE					(ERR_CODE + UP_SPA_WRN_START_CODE) 

#define UP_DRN_WRN_START_CODE					0x6C	/**4 max*/
#define UP_DRN_ERR_START_CODE					(ERR_CODE + UP_DRN_WRN_START_CODE) 

#define UP_MTN_WRN_START_CODE					0x70	/**4 max*/
#define UP_MTN_ERR_START_CODE					(ERR_CODE + UP_MTN_WRN_START_CODE) 



#define SPV_WRN_START_CODE						0x74	/**4 max*/
#define SPV_ERR_START_CODE						(ERR_CODE + SPV_WRN_START_CODE)

#define FLB_WRN_START_CODE						0x78	/**4 max*/
#define FLB_ERR_START_CODE						(ERR_CODE + FLB_WRN_START_CODE)



/**/

/**
Codici di output per il modulo HighLevel_IO_Manager (MAX 6)
*/ 
/* Warning */

/* Errori */
/** errore caricamento dati con basic loader */
#define HIO_ERR_BASIC_LOADER					MAKE_ERR_CODE(HIO, 0)
#define HIO_ERR_OUT_NOT_FOUND					MAKE_ERR_CODE(HIO, 1)
#define HIO_ERR_ARG_NOT_SUPPORTED				MAKE_ERR_CODE(HIO, 2)
#define HIO_ERR_SPRK_NOT_READY					MAKE_ERR_CODE(HIO, 3)

/**
Codici di output per il modulo WM_Hound	(MAX 36)
*/ 
/* Warning */

/* Errori */
#ifdef _HND_CHECK_ERRORS_

#define HND_ERR_OUTPUT_DATA						MAKE_ERR_CODE(HND, 0)
#define HND_ERR_GI_DATA							MAKE_ERR_CODE(HND, 1)
#define HND_ERR_HWC_FUNC_NUM					MAKE_ERR_CODE(HND, 2)
#define HND_ERR_HW_CONF_DATA					MAKE_ERR_CODE(HND, 3)
/* il numero di funzioni definite nel file di setting supera la dimensione dei buffer per le regolazioni */
#define HND_ERR_OVFL_FUNC_NUM					MAKE_ERR_CODE(HND, 4)
/* errore nel caricamento dei dati sul numero di cicli e delle fasi del ciclo di PROGRAMMING */
#define HND_ERR_CYCLES_DATA						MAKE_ERR_CODE(HND, 5)
/* il numero di cicli o delle fasi del ciclo di PROGRAMMING supera il massimo consentito */
#define HND_ERR_OVFL_CYCLES_NUM					MAKE_ERR_CODE(HND, 6)
/* errore nell'elaborazione dei dati di backup */
#define HND_ERR_BACKUP_ANALYSIS					MAKE_ERR_CODE(HND, 7)
/* errore nel caricamento dei dati relativi alle sicurezze */
#define HND_ERR_SAFETY_DATA						MAKE_ERR_CODE(HND, 8)
/* errore nel caricamento dei dati relativi ai fault */
#define HND_ERR_FAULT           				MAKE_ERR_CODE(HND, 9)
/** errore nel caricamento dei dati relativi alle statistiche delle funzioni */
#define HND_ERR_FUNCTIONS_STATISTICS_DATA		MAKE_ERR_CODE(HND, 10)


#else	/*_HND_CHECK_ERRORS_*/

#define HND_ERR_OUTPUT_DATA						MAKE_ERR_CODE(HND, 0)
#define HND_ERR_GI_DATA							MAKE_ERR_CODE(HND, 0)
#define HND_ERR_HWC_FUNC_NUM					MAKE_ERR_CODE(HND, 0)

#define HND_ERR_HW_CONF_DATA					MAKE_ERR_CODE(HND, 0)
/** il numero di funzioni definite nel file di setting supera la dimensione dei buffer per le regolazioni */
#define HND_ERR_OVFL_FUNC_NUM					MAKE_ERR_CODE(HND, 0)
/** errore nel caricamento dei dati sul numero di cicli e delle fasi del ciclo di PROGRAMMING */
#define HND_ERR_CYCLES_DATA						MAKE_ERR_CODE(HND, 0)
/** il numero di cicli o delle fasi del ciclo di PROGRAMMING supera il massimo consentito */
#define HND_ERR_OVFL_CYCLES_NUM					MAKE_ERR_CODE(HND, 0)
/** errore nell'elaborazione dei dati di backup */
#define HND_ERR_BACKUP_ANALYSIS					MAKE_ERR_CODE(HND, 0)
/** errore nel caricamento dei dati relativi alle sicurezze */
#define HND_ERR_SAFETY_DATA						MAKE_ERR_CODE(HND, 0)
/* errore nel caricamento dei dati relativi ai fault */
#define HND_ERR_FAULT           				MAKE_ERR_CODE(HND, 0)
/** errore nel caricamento dei dati relativi alle statistiche delle funzioni */
#define HND_ERR_FUNCTIONS_STATISTICS_DATA		MAKE_ERR_CODE(HND, 0)

#endif	/*!_HND_CHECK_ERRORS_*/

/**
Codici di output per il modulo Regulations (MAX 6)
*/ 
/* Warning */
/** l'indice di regolazione è fuori dal max consentito (regolazione a step) */	
#define REG_WRN_OUT_OF_RANGE_INDEX				MAKE_WRN_CODE(REG, 0)		

/* Errori */
/** errore caricamento dati con basic loader */
#define REG_ERR_BASIC_LOADER					MAKE_ERR_CODE(REG, 0)
/** id funzione non presente nella lista */
#define REG_ERR_FUNC_NOT_FOUND					MAKE_ERR_CODE(REG, 1)
/** errore su reg offset */
#define REG_ERR_OFFSET							MAKE_ERR_CODE(REG, 2)
/** indice regolazione nullo */
#define REG_NULL_INDEX							MAKE_ERR_CODE(REG, 3)
/** l'indice di regolazione è fuori dal max consentito (regolazione a tabella) */	
#define REG_ERR_OUT_OF_RANGE_INDEX				MAKE_ERR_CODE(REG, 4)


/**
Codici di output per il modulo Maintain (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_MTN_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_MTN, 0) 
/** Sw trap Error */
#define UP_MTN_ERR_GENERIC						MAKE_ERR_CODE(UP_MTN, 1)


/**
Codici di output per il modulo Load (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_LOA_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_LOA, 0) 
/** Stato pressotato indeterminato */
#define UP_LOA_ERR_PRESS_SENSOR					MAKE_ERR_CODE(UP_LOA, 1)

/**
Codici di output per il modulo Load Restore (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_LOR_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_LOR, 0) 
/** Stato pressotato indeterminato */
#define UP_LOR_ERR_PRESS_SENSOR					MAKE_ERR_CODE(UP_LOR, 1)

/**
Codici di output per il modulo SetState (MAX 2)
*/ 
/* Warning */
/** Errore caricamento dati o contenuto dati */
#define UP_SST_DATA_ERR							MAKE_ERR_CODE(UP_SST, 0) 



/**
Codici di output per il modulo Heat (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_HEA_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_HEA, 0) 
/**carico resistenza lavaggio non presente*/
#define UP_HEA_ERR_NO_WASH_RES					MAKE_ERR_CODE(UP_HEA, 1)

/**
Codici di output per il modulo PumpControl (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_PMP_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_PMP, 0)


/**
Codici di output per il modulo AlternateControl (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_ALT_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_ALT, 0) 
/** HI statement Error */
#define UP_ALT_ERR_HI_STMT					    MAKE_ERR_CODE(UP_ALT, 1)



/**
Codici di output per il modulo LoadsControl (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_LDC_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_LDC, 0) 

/**
Codici di output per il modulo SparklingAid (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_SPA_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_SPA, 0)
/** Basic Invalid data len */
#define UP_SPA_ERR_DATA_LEN   					MAKE_ERR_CODE(UP_SPA, 1)
/** Basic Hi statement */
#define UP_SPA_ERR_HI_STMT              		MAKE_ERR_CODE(UP_SPA, 2)

/**
Codici di output per il modulo DRN (MAX 4)
*/ 
/* Warning */

/* Errori */
/** Basic Loader Error */
#define UP_DRN_ERR_BASIC_LOADER					MAKE_ERR_CODE(UP_DRN, 0) 
/** HI statement Error */
#define UP_DRN_ERR_HI_STMT					    MAKE_ERR_CODE(UP_DRN, 1

/**
Codici di output per il modulo DW_Supervisor (MAX 4)
*/ 
/* Warning */
/** Non è presente alcun ciclo con la label specificata */
#define SPV_WRN_LABEL_NOT_FOUND					MAKE_WRN_CODE(SPV, 0)

/* Errori */
/** Basic Loader Error */
#define SPV_ERR_BASIC_LOADER					MAKE_ERR_CODE(SPV, 0)

/** La dimensione del campo Program Heading non è corretta */
#define SPV_ERR_PRG_HEADING_SIZE				MAKE_ERR_CODE(SPV, 1)


/**
Codici di output per il modulo DW_Supervisor (MAX 4)
*/ 
/* Warning */
/* Errori */
#define FLB_ERROR_LD_MODIF_VAR_TABLE	MAKE_ERR_CODE(FLB, 0)	/* Errore nel caricamento della tabella delle variabili che agiscono da modificatori */
#define FLB_ERROR_LD_MODIF_LEV1_TABLE	MAKE_ERR_CODE(FLB, 1)	/* Errore nel caricamento della tabella di livello 1 dei modificatori */
#define FLB_ERROR_LD_MODIF_LEV2_TABLE   MAKE_ERR_CODE(FLB, 2)	/* Errore nel caricamento della tabella  di livello 2 ei modificatori */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

#endif /* __CMNERR_H */

/* *********** */
/* End of file */
/* *********** */
