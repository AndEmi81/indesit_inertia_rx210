/*
Copyright (c) 2004-2005, Merloni Elettrodomestici, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\file    PM_UM_Control.h
\brief   Interfaces for the controllore
\addtogroup PM_MotorGate 
\author  Stefano Ughi, Roberto Fioravanti, Marco Sperandini
*/
/**
\addtogroup PM_MotorGate
*@{*/
#ifndef _PM_UM_CONTROL_H_
#define _PM_UM_CONTROL_H_
typedef struct inControlloreData_s
{
    const void * pST;	     /**< param ST pointer  6 bytes */
    const void * pCD;        /**< param CD pointer  2 bytes */
    const void * pCPD;       /**< param CPD pointer 2 bytes */
    uint8_t      rFactor;    /**< fattore di shift */
} inControlloreData_t;

typedef struct inControlloreVariables_s
{
    uint16_t vTarget;    /**< setpoint del controllo in r.p.m.*/    
    uint16_t tau_n;	 /**< valore corrente della tachimetrica in tick del timer */
    uint16_t tau_n_1;    /** Valore della tachimetrica all'istante (n-1) espresso in tick del timer */
    uint16_t alpha_n_1;        
} inControlloreVariables_t;


typedef struct outControllore_s
{
    uint16_t alpha;	/**< nuovo valore dell'angolo di parzializzazione di fase */
    uint8_t  error;	/**< possibili errori nel calcolo di alpha */
} outControllore_t;

typedef struct outFiltersAlpha_s
{
    uint16_t alpha;	/**< nuovo valore dell'angolo di parzializzazione di fase, filtrato  */
    uint8_t  boundary;	/**< diagnostica su alpha */
} outFiltersAlpha_t;

typedef struct filterParams_s
{
    uint8_t maxDev;	/**<
						massima deviazione consentita tra nuovo e vecchio
						angolo espressa in us
						N.B. deve risultare maxDev<=minAlpha,
						e maxDev+maxAlpha<=0xFFFF
						*/
    uint16_t maxAlpha;  /**< angolo massimo applicabile in us*/
    uint16_t minAlpha;  /**< angolo minimo applicabile in us */
} filterParams_t;

void controllore(inControlloreData_t inControlloData, inControlloreVariables_t inControlloreVariables, outControllore_t *outControllore );
void filterNewAlpha( outControllore_t outControllore, const filterParams_t *filterParamsPtr,  uint16_t alpha_n_1_, outFiltersAlpha_t* pOut);
#endif /* _PM_UM_CONTROL_H_ */
/**@}*/
