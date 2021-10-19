/**
\defgroup Regenerate	Modulo microprogramma Regenerate

Gestisce il microprogramma Regenerate per effettuare le rigenera delle resine.

\par \b PCLint: x Errors, x Warnings, x Info 26/06/2008 Valeria Catani


\date    26/06/2008
\author  Valeria Catani
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file Regenerate.c

\file    Regenerate.h
\ingroup Regenerate
\date    26/06/2008
\author  Valeria Catani
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author  
*/
 
/**
\addtogroup Regenerate
@{*/

#ifndef __UP_REGENERATE_H
#define __UP_REGENERATE_H

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */
/*
#ifdef _CHECKERRORS_
	#define _HEA_CHECKERRORS_
#endif
*/

#define REGENERATE_INIT		0
#define REGENERATE_EXE_0	1
#define REGENERATE_EXE_1	2
#define REGENERATE_EXE_2	3




#define REGENERATE_TRUE		4


#define MARGIN				10 	/*x100msec*/

#define LEV_TO_TIME_REF_PRECISION 5  /*PRECISIONE MISURA PORTATA*/


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t RGN_uP_Regenerate_Exe (uint8_t HighStatement,uint8_t LowStatement);
void RGN_updateRegNumber (void);

#endif /* __UP_REGENERATE_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */
