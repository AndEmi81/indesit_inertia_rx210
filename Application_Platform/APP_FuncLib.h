/**
Header file del modulo Application Platform Library Functions

\file		APP_FuncLib.h
\ingroup	APP_FuncLib
\date		08/07/2009
\version	01.00.00
\author		Marco Quintini

\par		History

*/

/*
Copyright (c) 2004-2007, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

#ifndef __APP_FUNC_LIB_H
#define __APP_FUNC_LIB_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */

/**
\name Macro per manipolazione di bit
*/
/*@{*/ 
/**
Restituisce 0 se il bit in posizione 'bit' di 'var' vale 0, un valore diverso da 0
se il bit vale 1
*/
/*lint -emacro(912, TESTBIT)*/
#define TESTBIT(var, bit)				((var) & ((uint32_t)(1) << (bit)))
/** Imposta a 1 il bit in posizione 'bit' di 'var' */
/*lint -emacro(734, SETBIT)*/
/*lint -emacro(912, SETBIT)*/
#define SETBIT(var, bit)			    ((var) |= ((uint32_t)(1) << (bit)))
/** Imposta a 0 il bit in posizione 'bit' di 'var' */
/*lint -emacro(734, CLRBIT)*/
/*lint -emacro(912, CLRBIT)*/
#define CLRBIT(var, bit)				((var) &= ~((uint32_t)(1) << (bit)))
/** Restituisce il valore del bit in posizione 'bit' di 'var' */
#define GETBITVAL(var, bit)				((TESTBIT((var), (bit))) != 0)
/**
Restituisce una copia di 'var' in cui i bit specificati in 'setmask'
hanno i valori specificati in 'val'
*/
#define BUILD_BMP(var, val, setmask)	(((var) & ~(setmask)) | ((val) & (setmask)))
/** Crea una maschera (con dimensione 1 byte) cha ha il solo bit i-esimo pari a 1 */
#define MASK_B(i)						((uint8_t)(1 << (i)))
/** Crea una maschera (con dimensione 1 word) cha ha il solo bit numero x pari a 1 */
#define MASK_W(i)						((uint16_t)(1 << (i)))
/** Shifta a sinistra l'argomento val (con dimensione 1 byte) di n bit */
#define LROT_B(val,n)					((uint8_t)((val) << (n)))
/*@}*/ 

/**
\name Macro generiche
*/
/*@{*/ 
/**
Dato un puntatore a uint8_t puint8_t, restituisce il valore dell'uint8_t
puntato da (puint8_t + i)
*/
/*lint -emacro(912, OFFS_B)*/
#define OFFS_B(puint8_t, i)				((uint8_t)(*((uint8_t*)(puint8_t) + (i))))
/**
Dato un puntatore a uint16_t puint16_t, restituisce il valore dell'uint16_t
puntato da (puint16_t + i)
*/
/*lint -emacro(826, OFFS_W)*/
#define OFFS_W(puint16_t, i)			((uint16_t)(*((uint16_t*)(puint16_t) + (i))))
/**
Dato un puntatore a int16_t pint16_t, restituisce il valore dell'int16_t
puntato da (pint16_t + i)
*/
#define OFFS_SW(pint16_t, i)      ((int16_t)(*((int16_t*)(pint16_t) + (i))))

/** Restituisce il numero di elementi di un buffer x */
#define SIZE(x)							(sizeof(x)/sizeof(x[0]))
/*@}*/ 

/** Macro (temporanea?) per Set_UpCounter */
#define SHORT_COUNTER	((uint8_t) 1 )
#define MEDIUM_COUNTER	((uint8_t) 2 )
#define LARGE_COUNTER	((uint8_t) 3 )

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
#if (SW_TIMER_NORM_MODE	== FALSE) 

void Set_SmallCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_SCounterValue value);
void Set_MediumCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_MCounterValue value);
#if defined(SWTC_L_ENABLED)
void Set_LargeCounter(SWTC_CountersType idCounter, SWTC_PrescalerType prescalerValue, SWTC_LCounterValue value);
#endif	/*#if defined(SWTC_L_ENABLED)*/
void Reset_SmallCounter(SWTC_CountersType idCounter);
void Reset_MediumCounter(SWTC_CountersType idCounter);
#if defined(SWTC_L_ENABLED)
void Reset_LargeCounter(SWTC_CountersType idCounter);
#endif	/*#if defined(SWTC_L_ENABLED)*/

#else /*(SW_TIMER_NORM_MODE	== FALSE) */

void Set_UpCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode);
void Set_DownCounter(uint8_t counterSize, SWTC_CountersType counterId, SWTC_PrescalerType opCode, SWTC_TimeType value);
void StopCounterToZero(uint8_t counterSize, SWTC_CountersType counterId);

#endif /*(SW_TIMER_NORM_MODE	== FALSE) */

void IncByteWithCheck(uint8_t *pAccumulator);

#if (CHECKERRORS_ENABLE == 1)	/* Senza else poichè si usa solo se (CHECKERRORS_ENABLE == 1) */
void DBG_CheckErr(uint8_t codeToVerify);
#endif /* (CHECKERRORS_ENABLE == 1) */

#endif /* __APP_FUNC_LIB_H */

/* *********** */
/* End of file */
/* *********** */
