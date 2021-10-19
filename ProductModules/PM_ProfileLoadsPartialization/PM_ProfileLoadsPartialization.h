/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		PM_ProfileLoadsPartialization.h
\n			header file of  PM_ProfileLoadsPartialization.c
\n
\file		  PM_ProfileLoadsPartialization.h
\ingroup  ProfileLoadsPartialization
\date		  Dec 02, 2011
\version	01.00.00
\author   Leonardo Sparti
\n
\n
\par	History:
\n
\n 		01.00.00
\n		Dec 02, 2011
\n		Leonardo Sparti
\n		changes
\n 			Creation
\n
*/

#ifndef __PLP_H
#define __PLP_H



/* ********** */
/*  Includes  */
/* ********** */
#include "Config.h"

/* ******************* */
/* Function prototypes */
/* ******************* */
void PM_PLP_SetLoad(uint8_t on, uint8_t pin, uint16_t* pProfile, uint8_t len, uint8_t repeat, uint8_t cfg);
void PM_DIM_SetLoad(uint8_t on, uint8_t config, uint8_t pin, const uint8_t* stepPart);
void DIM_Scheduler(void);
void PM_PLP_Scheduler(void);
void PM_PLP_InsertLoad(uint8_t pin, uint8_t type);
uint8_t PM_PLP_LoadsToDrive(void);


void PLP_ISR(void);




#endif  /* __PLP_H */
