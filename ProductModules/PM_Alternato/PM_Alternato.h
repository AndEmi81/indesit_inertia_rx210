/*
 * PM_Alternato.h
 *
 *  Created on: 06/nov/2013
 *      Author: consul.fimiani
 */

#ifndef PM_ALTERNATO_H_
#define PM_ALTERNATO_H_

/* ******** */
/* Includes */
/* ******** */
#include "PM_AlternatoExp.h"
#include "OutputManagerExp.h"
#include "StandardTypes.h"
#include "StandardDefines.h"



/* ****************** */
/* Defines and Macros */
/* ****************** */
/** Valore ammissibile per AlternatoError_t */
#define ALTERNATO_E_OK  0
/** Valore ammissibile per AlternatoError_t */
#define ALTERNATO_E_KO  1


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
typedef uint8_t AlternatoError_t;
typedef const struct Loader*  pAlternatoData_t;




AlternatoError_t PM_ALT_Init(pAlternatoData_t pAlternatoData, uint8_t pin);
void PM_ALT_SetPosition(L_PARAM loadParam);
void PM_ALT_EngineDrv(void);

#endif /* PM_ALTERNATO_H_ */
