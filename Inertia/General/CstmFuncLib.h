/**
Header file del modulo funzioni di libreria

\file    CstmFuncLib.h
\ingroup CstmFuncLib
\date    14/01/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __CSTM_FUNC_LIB_H
#define __CSTM_FUNC_LIB_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
#ifdef _CHECKERRORS_
	#define _FLB_CHECKERRORS_
#endif


/** MANIPOLAZIONE DI BYTE E WORD */
/** Restituisce una word, forniti due byte  in ingresso */
#define MAKEWORD(h, l)     				((uint16_t)(((uint8_t)((uint16_t)(l) & 0xFF)) | ((uint16_t)((uint8_t)((uint16_t)(h) & 0xFF))) << 8))
/** Restituisce il byte meno significativo della word in ingresso */
#define LOBYTE(w)						((uint8_t)((uint16_t)(w) & 0xFF))
/** Restituisce il byte più significativo della word in ingresso */
#define HIBYTE(w)						((uint8_t)((uint16_t)(w) >> 8))
/** Restituisce il nibble meno significativo del byte in ingresso */
#define LONIBBLE(b)						((uint8_t)((uint8_t)(b) & 0x0F))
/** Restituisce il nibble più significativo del byte in ingresso */
#define HINIBBLE(b)						((uint8_t)((uint8_t)(b) >> 4))
/** 
Dati in ingresso una word 'w' ed un byte 'b' restituisce una word che ha come
byte più significativo quello di 'w' e come byte meno significativo 'b'
*/
#define SET_LOBYTE(w, b)				((uint16_t)(((uint16_t)(w) & 0xFF00) | (uint8_t)(b)))
/** 
Dati in ingresso una word 'w' ed un byte 'b' restituisce una word che ha come
byte più significativo 'b' e come byte meno significativo quello di'w'
*/
#define SET_HIBYTE(w, b)				((uint16_t)(((uint16_t)(b) << 8) | (uint8_t)(w)))
/** 
Dati in ingresso due byte 'b' e 'n' restituisce un byte che ha come
nibble più significativo quello di 'b' e come nibble meno significativo
i primi 8 bit di 'n'
*/
#define SET_LONIBBLE(b, n)				((uint8_t)(((uint8_t)(b) & 0xF0) | (uint8_t)(n)))
/** 
Dati in ingresso due byte 'b' e 'n' restituisce un byte che ha come
nibble più significativo i primi 8 bit di 'n' e come nibble meno significativo
quello di 'b' 
*/
#define SET_HINIBBLE(b, n)				((uint8_t)(((uint8_t)(n) << 4) | LONIBBLE(b)))
/** Restituisce una word che ha i byte invertiti rispetto a quella in ingresso */
#define SWAP_WORD(w)					((uint16_t)(LOBYTE((uint16_t)(w)) << 8) | HIBYTE((uint16_t)(w)))
/**/


/** MAX E MIN */
#define MAX(a,b)    					(((a) > (b)) ? (a) : (b))
#define MIN(a,b)						(((a) < (b)) ? (a) : (b))
/**/


/** GENERICHE */

/** Restituisce il numero in ingresso codificato in BCD */
#define BIN2BCD(x)						((uint8_t)(((x) % 0x0A) | (((x) / 0x0A) << 4)))

/** RELATIVE A FUNZIONI */
/* GetModifierData */
/** 
Valore del modificatore da trascurare che viene restituito ad esempio
quando la funzione non è attiva (come ritorno 0 è un valore 
potenzialmente significativo quando ci sono più modificatori)
*/
#define MOD_DONT_CARE			INT8_T_MAX


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */


/* ******************* */
/* Function prototypes */
/* ******************* */

/** Gestione stato DW */
void UpdateMachineState(void);
void UpdateStateBitmaps(void);


/** Caricamenti - Rigenere */
void updateRegWaterLoaded( void );
void clearRegWaterLoaded( void );
void WaterLoadedReset(void);


/** Gestione modificatori/variazioni */ 
int16_t GetDeltaFromVariation(uint16_t value, int8_t variation);
uint8_t TabQueryB(const Loader_Struct* pTable, uint8_t regVal);
int8_t GetModifierData(const uint8_t* pModTable);
uint8_t GetModifierValue(uint8_t tabIndex, int8_t *pModifierValue);
uint8_t GetModifierValueAPP2_0(uint8_t tabIndex, int16_t *pModifierValue);
uint8_t GetAlphaModifierValueAPP2_0(uint8_t tabIndex, LIB_Fixed_ValueType *pModifierValue);
uint8_t TabQuery(const TabQueryIn_t * _in, TabQueryResult_t * _result);
void APP_AlphaCoefficientsComputation(void);
uint8_t VlineCalcolusFromDSP(LIB_Fixed_ValueType *valuePtr);
void APP_GetReferenceParameters();


/* Generic */
void StopMicroprogCounters(void);
void IncWordWithCheck(uint16_t *statisticWord);
uint8_t IsOvenRunningDelay (void);
uint8_t IsOvenPauseFAult (void );


/** Calibrazione Sensore di Torbidità */
void TurbidityCalibrationCmd(void);
void TurbidityCalibrationEnd(void);
void TurbidityCalibrationManager(void);
void TurbidityPwmValManager(void);

/*PM power meter*/
uint8_t UpdateInfoBufferToPowerMeterModule(void);


/*@}*/

#endif /* __CSTM_FUNC_LIB_H */

/* *********** */
/* End of file */
/* *********** */
