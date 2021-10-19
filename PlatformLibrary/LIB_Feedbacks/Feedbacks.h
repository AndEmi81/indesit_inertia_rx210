/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		Feedbacks.h
\n			header file of Feedbacks.c
\n
\file		Feedbacks.h
\ingroup	LIB_Feedbacks
\date		Nov 8, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Nov 8, 2010
\n		stefano.ughi
\n		changes
\n 		    Creation
\n
\n
*/

#ifndef __FEEDBACKS_H
#define __FEEDBACKS_H

/* ********** */
/* Includes */
/* ********* */
/*lint -e537*/
#include "StandardTypes.h"
#include "FeedbacksExp.h"
/*lint +e537*/
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void LIB_FDB_InitSync(void);
void LIB_FDB_RegisterFeedback( uint8_t pos, uint8_t pin, uint8_t readtype, const uint8_t *const pConfigData );
void LIB_FDB_Manager(bool_t loadType, bool_t zcIsAvailable);
void LIB_FDB_FeedbackInitMeasure(uint8_t pos);


#endif /* __FEEDBACKS_H */

/* *********** */
/* End of file */
/* *********** */


