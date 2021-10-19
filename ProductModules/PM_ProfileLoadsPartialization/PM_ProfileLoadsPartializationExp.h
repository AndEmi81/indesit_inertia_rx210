/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		PM_ProfileLoadsPartializationExp.h
\n			This file contains all defines,macro,typedef and functions's prototypes that Application
\n          can use. 
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		PM_ProfileLoadsPartializationExp.h
\ingroup	PM_ProfileLoadsPartialization
\date		Dec 13, 2011 
\version	01.00.00 
\author		Leonardo Sparti 
\n
\n
\Par\b	History:
\n
\n 		01.00.00 
\n		Dec 13, 2011 
\n		Leonardo Sparti 
\n		changes 
\n 			Creation
\n
*/


#ifndef __PLP_EXP_H
#define __PLP_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "LIB_ZeroCrossingExp.h"


/* ******************* */
/* Function prototypes */
/* ******************* */

void PM_PLP_Init_OnWarmReset(void);
void PM_PLP_Init(void);

#endif /* __PLP_EXP_H */

/* *********** */
/* End of file */
/* *********** */

