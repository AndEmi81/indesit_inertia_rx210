#ifndef _LIB_TRACE_H_
#define _LIB_TRACE_H_

#include "Config.h"


#if defined(CONFIG_LIB_TRACE_SUPPORT)
#include "DigitalOutputs.h"
#include "OCDIO_drv.h"
#if defined(RN_RX200)
#include "RN_RX200_reg.h"
#include "RN_RX200_cfg.h"
#endif
#include "IO_driver.h"




/**
Marks the start of the trace
Makes sense if called once.
*/
#define START_TRACE(pin) { \
  OCDIO_SetPinDirValueSync(pin-1, 1);\
  OCDIO_SetPinValueSync( pin-1, 0);\
  OCDIO_SetPinValueSync( pin-1, 1);\
  OCDIO_SetPinValueSync( pin-1, 0);\
  OCDIO_SetPinValueSync( pin-1, 1);\
  }

/**
Drives the pin
*/
#if 0
#define TRACE_TO_PIN(status , pin )   {  OCDIO_SetPinDirValueSync(pin-1, 1); OCDIO_SetPinValueSync(pin-1, status); }
#else

/*lint -emacro(506, TRACE_TO_PIN) ignore 506 Constant value Boolean [MISRA Rule 52]*/
#define TRACE_TO_PIN(status , pin )	\
{ \
	const   IO_ModuleIOMapType* pMap= OCDIO_GetIOMapItem(pin-1);\
	DIO_SetDirAsOutput(pMap);	\
	if((status))				\
	{							\
		DIO_SetHigh(pMap);		\
	}else						\
	{							\
		DIO_SetLow(pMap);		\
	}  							\
}
#endif



/**
Generates a sequence of pulses.
*/
#define TRACE_TO_PIN_IMPULSE( n_impulse, pin) \
{ int i;\
for(i=0; i<n_impulse; i++){\
  if( (i & 0x01) == 0 ){    \
  OCDIO_SetPinDirValueSync(pin-1, 1);              OCDIO_SetPinValueSync(pin-1, 0);\
  }else{    OCDIO_SetPinDirValueSync(pin-1, 1);    OCDIO_SetPinValueSync(pin-1, 1);}\
}\
}


#define TRACE_LED( status, pin )   { OCDIO_SetPinDirValueSync(pin-1, 1); OCDIO_SetPinValueSync(pin-1, status?0:1); }
#define TRACE_LED_N( status, nled ) { OCDIO_SetPinDirValueSync(pin-1, 1); OCDIO_SetPinValueSync( LED_YALLOW + nled, status?0:1 );}
#define TRACE_ERROR_STOP( pin )   {  while(1) {int status; OCDIO_SetPinDirValueSync(pin-1, 1); OCDIO_SetPinValueSync( pin-1, status = !status);{ int i; for( i = 0; i<200000; i++ ) ;} }}

#else /************************************/
#define START_TRACE()							{}

/* Riporta lo stato indicato sull pin */
#define TRACE_TO_PIN(status , pin )				{}

/* Ivia un treno di N impulsi sul pin */
#define TRACE_TO_PIN_IMPULSE( n_impulse, pin)	{}
#define TRACE_LED( status, nled )				{}
#define TRACE_LED_N( status, nled )				{}
#define TRACE_ERROR_STOP( led )					{}

#define	TRACE_CPU_IDLE_START()					{}
#define	TRACE_CPU_IDLE_END()					{}
#define TRACE_ZC_IRQ_START()					{}
#define TRACE_ZC_IRQ_END()						{}

#endif
#endif /* _LIB_TRACE_H_*/
