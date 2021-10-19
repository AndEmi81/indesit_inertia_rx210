#ifndef __DIO_CFG_H
#define __DIO_CFG_H

#define HAL_DIO_SUPPORT
#include "StandardTypes.h"

#if  defined(EDGE_MODULE_LP1_0258_01) || defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)
#define IO_MODULE_MAP_SIZE		44
#elif defined(CASTELLATED_MODULE_LP1_0241_01)
#define IO_MODULE_MAP_SIZE		64
#else
#error "Module not yet supported or check you pre-processor settings."
#endif
/* Max Number of Digital Inputs Managed From Application */
#define DIGINPUT_NUM    		8u
/* Max Number of Loads Managed From Application */
#define N_MAXLOADS      		16u

/* 
In order to enable local filtering please enable DIGITAL_LOCAL_FILTER_ENABLED define
and set the value of DIGITAL_LOCAL_FILTER_NUM  
N.B: n° of local samples is DIGITAL_LOCAL_FILTER_NUM+1 
max value for DIGITAL_LOCAL_FILTER_NUM is 127 
*/
#define  DIGITAL_LOCAL_FILTER_ENABLED  



#if defined (DIGITAL_LOCAL_FILTER_ENABLED)
  #define DIGITAL_LOCAL_FILTER_NUM       8
#endif

/*------------------------ Don't touch below -----------------------------*/

#if (DIGINPUT_NUM>32)
#error "DIGINPUT_NUM max value is 32"
#endif

#if defined (DIGITAL_LOCAL_FILTER_ENABLED)
#if (DIGITAL_LOCAL_FILTER_NUM>127)
#error "\n DIGITAL_LOCAL_FILTER_NUM max value is 127"
#endif 
#if (DIGITAL_LOCAL_FILTER_NUM < 0)
	#error "\n are you joking? "
#endif
#endif /* #if defined (DIGITAL_LOCAL_FILTER_ENABLED) */

#if (DIGINPUT_NUM <= 8) 
typedef uint8_t dig_input_t;
#elif (DIGINPUT_NUM <= 16)
typedef uint16_t dig_input_t;
#else
typedef uint32_t dig_input_t;
#endif

#endif /* #ifndef __DIO_CFG_H */
