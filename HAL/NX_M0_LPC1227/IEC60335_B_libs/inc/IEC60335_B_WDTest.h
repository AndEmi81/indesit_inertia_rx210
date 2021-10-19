
#include "IEC60335.h"
#ifndef _IEC60335_B_WDTEST_H
#define _IEC60335_B_WDTEST_H
/*****************************************************************************/  
/*	WD test config                                                       */
/*****************************************************************************/
#define WDTEST_STARTED  0x1
#define WDTEST_COMPLETE 0x2

#define RESET_POR           0x1
#define RESET_EXTRST        0x2
#define RESET_WD            0x4
#define RESET_BOD           0x8
#define RESET_SYSRES        0x10

#define WD_ENABLE_RESET     ((1u << 3)| (1u << 1) | (1u << 0))
#define WD_TICKS            0xFFFFFF 

/*!
 * @brief IEC60335 WD Test
 */
INT32 IEC60335_WDTest(void);

#endif
