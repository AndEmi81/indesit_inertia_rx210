/**
\defgroup DW_Regulations	Modulo Gestione Regolazioni DW

Il modulo contiene le funzioni di gestione delle funzioni di regolazione UI-Main

\par \b PCLint: x Errors, x Warnings, x Info 12/02/2008 Valeria Catani


\date    12/02/2008
\author  Valeria Catani
\version 1.0.0
\ingroup ApplUI
*/

/**
Header file del file DW_Regulations.c

\file    DW_Regulations.h
\ingroup DW_Regulations
\date    12/02/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 1.0.0
\date    24/01/2006
\author  Marco Quintini -	Simone Costantini - Valeria Catani 
*/


/**
\addtogroup DW_Regulations
@{*/

#ifndef __REGULATIONS_H
#define __REGULATIONS_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */


/** Numero di funzioni definite nel file di setting con il flag 'To Main' settato */
#define FUNC_NUM_TO_MAIN	(*(Tab_HWCFuncNum.Data))
/** Numero totale delle funzioni definite nel file di setting */
#define FUNC_NUM_ALL		(*(Tab_HWCFuncNum.Data+1))


/** Valori di ritorno per REG_checkIfChanged() */
#define REG_NOT_EXIST	0
#define REG_CHANGED		1
#define REG_UNCHANGED	2

/** struttura dati Function Heading */
#define NUM_WORDS_FUNC_DESCR	3	/** Numero di word della descrizione della funzione */

/** formato dato regolazione*/
#define DATA_FORMAT_UCHAR	0
#define DATA_FORMAT_USHORT  b7
#define DATA_FORMAT_SCHAR   b6
#define DATA_FORMAT_SSHORT  (b7|b6)

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
void REG_getBufFunctionsIDs(void);
uint8_t REG_getFuncPosition(uint8_t functionID, uint8_t *pFuncPos);
uint8_t REG_getFunctionValueByPos(uint8_t funcPos, uint16_t *pFunctionVal);
uint8_t REG_getFunctionValueByID(uint8_t functionID, uint16_t *pFunctionVal);
uint8_t REG_checkIfChanged(uint8_t functionID);
void REG_updateOldReg(uint8_t functionID);
void REG_updateFuncToMainOldReg(void);



#endif /* __REGULATIONS_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
