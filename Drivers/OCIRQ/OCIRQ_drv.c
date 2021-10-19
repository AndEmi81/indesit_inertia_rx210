/*
Copyright (c) 2004-2005, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\Par		OCIRQ_drv.c
\n			Implementation of all functions necessary to achieve the aim.
\n
\file		OCIRQ_drv.c
\ingroup	OCIRQ
\date		Nov 30, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Nov 30, 2010
\n		stefano.ughi
\n		changes
\n 			Creation
\n
\n
*/
#include "Config.h"

/* ******** */
/* Includes */
/* ******** */
#include "OCDIO_drv.h"
#include "OCIRQ_drv.h"
#include "LIB_TaskMonitoring.h"


#if defined(ZC_IO_MODULE_MAP_SIZE) && defined(ZC_IO_MODULE_MAP_NAME)

#if (ZC_IO_MODULE_MAP_SIZE == 0)
	#error "ZC_IO_MODULE_MAP_SIZE, if defined, must assume values greater than 0. IRQ_cfg.h"
#else   /* (ZC_IO_MODULE_MAP_SIZE == 0) */
	#include "OCDIO_drv.h"
#endif  /* (ZC_IO_MODULE_MAP_SIZE == 0) */

#endif  /* defined(ZC_IO_MODULE_MAP_SIZE) && defined(ZC_IO_MODULE_MAP_NAME) */

/* ****************** */
/* Defines and Macros */
/* ****************** */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ********* */
/* Variables */
/* ********* */

/* Global */
#if defined(ZC_IO_MODULE_MAP_SIZE) && defined(ZC_IO_MODULE_MAP_NAME)
extern const IO_ModuleIOMapType ZC_IO_MODULE_MAP_NAME[ZC_IO_MODULE_MAP_SIZE];
#endif /* defined(ZC_IO_MODULE_MAP_SIZE) && defined(ZC_IO_MODULE_MAP_NAME)*/
/* Static */
/** callback called inside ISR */
#if defined(CONFIG_OCIRQ_STATIC_CFG) && !defined(CONFIG_LIB_ZC_ON_ICU)
		#define IRQ_Callback OCIRQ_CALLBACK
#else
static Comm_Notification IRQ_Callback= (Comm_Notification)NULL;
#endif
/* ******************* */
/* Functions Prototype */
/* ******************* */

/* Exported function */
/* Local Function */
static bool_t IRQ_IsValid(bool_t edge);

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */
/**
IRQ Input Initialization. This function performs IRQ setup according to the properties defined
\n in IRQ_cfg.h.
\n Call this function at reset.

\param  [in]  callback used to notify user about IRQ event

\return 	IO_ErrorType
\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvIRQ_2
*/
IO_ErrorType OCIRQ_InitSync(Comm_Notification callback)
{
#if !defined(CONFIG_OCIRQ_STATIC_CFG)
	IRQ_Callback = callback;
#endif
#if defined(ZC_IO_MODULE_MAP_SIZE)
    OCDIO_SetIOMapPinDirValueSync((IO_ModuleIOMapType*)&ZC_IO_MODULE_MAP_NAME[0], 0, 1);
#endif /* defined(ZC_IO_MODULE_MAP_SIZE) */
	
	IRQ_Init(IRQ_ZC_ID);

#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) && defined(ZC_PIN_IDX) && defined(DIGITAL_FILTER_CLOCK_IRQ)
	OCDIO_SetIODigitalFilterPinSync(ZC_PIN_IDX);
	OCDIO_SetIODigitalFilterFrequencySync(ZC_PIN_IDX, DIGITAL_FILTER_CLOCK_IRQ);
#endif
	return (IO_ErrorType)IO_E_OK;
}
/**
This function sets the edge for IRQ interrupt.

\param  [in]	edge possibles values are:
\li IRQ_ON_NEGATIVE_EDGE
\li IRQ_ON_POSITIVE_EDGE

\return 	IO_ErrorType
\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/

IO_ErrorType OCIRQ_SetEdgeSync(bool_t edge)
{
	IRQ_SetEdge(edge, IRQ_ZC_ID);
	return (IO_ErrorType)IO_E_OK;
}
/**
This function Enables/Disables IRQ interrupt.

\param  [in]	enable possibles values are:
\li TRUE enable
\li FALSE disable

\return 	IO_ErrorType
\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/

IO_ErrorType OCIRQ_IntrptEnableSync(bool_t enable)
{
	IRQ_IntEnable(enable, IRQ_ZC_ID);
	return (IO_ErrorType)IO_E_OK;
}
/**
This function returns the edge selected for IRQ interrupt.

\return 	bool_t

\retval		IRQ_ON_NEGATIVE_EDGE
\retval		IRQ_ON_POSITIVE_EDGE

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/

bool_t OCIRQ_GetEdgeSync(void)
{
	return (bool_t)IRQ_GetEdge(IRQ_ZC_ID);
}

/**
This function changes synchronization edge for IRQ interrupt.

\param  [in] irqEdgeSync    current sync. edge

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
void OCIRQ_ChangeEdgeSync(bool_t irqEdgeSync)
{
	if(IRQ_ON_POSITIVE_EDGE == irqEdgeSync)
	{
		(void)OCIRQ_SetEdgeSync(IRQ_ON_NEGATIVE_EDGE);
	}
	else
	{
		(void)OCIRQ_SetEdgeSync(IRQ_ON_POSITIVE_EDGE);
	}
}

/**
This function deinitialises the IRQ driver and
\n sets involved registers back to their controller default configuration.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/
IO_ErrorType OCIRQ_DeInitSync(void)
{
	IRQ_DeInit(IRQ_ZC_ID);
#if !defined(CONFIG_OCIRQ_STATIC_CFG)
	IRQ_Callback=(Comm_Notification)NULL;
#endif
	return (IO_ErrorType)IO_E_OK;
}

/**
ISR of IRQ. This function calls the callback function defined by the user.
\n It checks first if IRQ is valid.

\note		This function must be called inside interrupt vector

\par Architecture Requirements ID: SARS_WMMT_MN_DrvADC_2
*/

void IRQ_ISR(void)
{
	bool_t irqValidation;
	bool_t irqEdgeSync;

	IRQ_MONITOR_ENTER();

	/* Get IRQ Sync Edge */
	irqEdgeSync = OCIRQ_GetEdgeSync();

	/* Validate Interrupt */
	irqValidation = IRQ_IsValid(irqEdgeSync);
	/* Calls the user callback */
	if( IRQ_Callback ){
		(void)(IRQ_Callback)((IO_NotificationType)irqEdgeSync, (IO_ChannelType)0, (IO_ErrorType)irqValidation);
	}
	/* Acknowledges interrupt request events */
	IRQ_Acknowledge(IRQ_ZC_ID);

	IRQ_MONITOR_EXIT();

}

/* Local Function */


/**
This function checks if interrupt is valid.

\param  [in]    edge syncronization edge valid values are:
\li             IRQ_ON_NEGATIVE_EDGE
\li             IRQ_ON_POSITIVE_EDGE

\return 	bool_t

\retval		TRUE    valid IRQ interrupt
\retval		FALSE   spurious IRQ interrupt
*/
#if defined(ZC_IO_MODULE_MAP_SIZE)

static bool_t IRQ_IsValid(bool_t edge)
{
#if (IRQ_FILTER > 0)
	{

		uint8_t filter = IRQ_FILTER;
		uint8_t cntr = 0;
		/* read more time to ensure that the signal level is correct*/
		do
		{   /*lint -e960 no side effects on right hand */
			if ( ((IRQ_ON_POSITIVE_EDGE == edge) && 
			        OCDIO_GetIOMapPinValueSync((IO_ModuleIOMapType*)&ZC_IO_MODULE_MAP_NAME[0], 0)) 
			||
			((IRQ_ON_NEGATIVE_EDGE == edge) && 
			    (!OCDIO_GetIOMapPinValueSync((IO_ModuleIOMapType*)&ZC_IO_MODULE_MAP_NAME[0], 0))) )
				/*lint +e960 no side effects on right hand */
			{
				cntr++;
			}
			filter--;
		}
		while(filter > 0);

		if ( IRQ_FILTER == cntr )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
#else
	return TRUE;
#endif  /* #if (IRQ_FILTER > 0) */

}


#else /* defined(ZC_IO_MODULE_MAP_SIZE) */

/**
This function checks if interrupt is valid.

\param  [in]    edge syncronization edge valid values are:
\li             IRQ_ON_NEGATIVE_EDGE
\li             IRQ_ON_POSITIVE_EDGE

\return 	bool_t

\retval		TRUE    valid IRQ interrupt
\retval		FALSE   spurious IRQ interrupt
*/

static bool_t IRQ_IsValid(bool_t edge)
{
#if (IRQ_FILTER > 0)
	{

		uint8_t filter = IRQ_FILTER;
		uint8_t cntr = 0;
		do
		{   /*lint -e960 no side effects on right hand */
			if ( ((IRQ_ON_POSITIVE_EDGE == edge) && IRQ_pinIsHigh(IRQ_ZC_ID)) ||
			     ((IRQ_ON_NEGATIVE_EDGE == edge) && IRQ_pinIsLow( IRQ_ZC_ID)) )
				/*lint +e960 no side effects on right hand */
			{
				cntr++;
			}
			filter--;
		}
		while(filter > 0);

		if ( IRQ_FILTER == cntr )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
#else
    (void) edge;
	return TRUE;
#endif  /* #if (IRQ_FILTER > 0) */

}
#endif /* defined(ZC_IO_MODULE_MAP_SIZE) */

#if (IRQ_FILTER > 255)
#error "IRQ_FILTER too high max. value 255, check IRQ_cfg.h"
#endif


/* *********** */
/* End of file */
/* *********** */

