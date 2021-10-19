/*
 * LIB_Generic.h
 *
 *  Created on: Feb 21, 2013
 *      Author: Roberto Fioravanti
 */

#ifndef _LIB_GENERIC_H_
#define _LIB_GENERIC_H_

#include "Config.h"


typedef uint16_t	LIB_ModifiedChkType;

LIB_ModifiedChkType	LIB_ModifiedChk_Computation(uint16_t len, uint8_t* pSource, LIB_ModifiedChkType initval);
LIB_ModifiedChkType	LIB_ModifiedChk16_Computation(uint16_t len, uint16_t* pSource);

#endif /* _LIB_GENERIC_H_ */
