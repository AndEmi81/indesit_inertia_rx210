/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\n
\par		    OCDIO - On Chip Digital IO Driver Source file
\n			    Contains implementation of the OCDIO driver
\n
\file		    OCDIO_drv.c
\ingroup	    OCDIO
\date		    2/02/2010
\version	    00.00.01
\author		    Jaroslav Musil
\n
\n
\par	History:
\n
\n 		        00.00.01
\n          	2/02/2010
\n          	Jaroslav Musil
\n            - Creation on the base of existing files for S08
\n
*/


/* ******** */
/* Includes */
/* ******** */


#include "OCDIO_drv.h"
#include "LIB_Trace.h"
/*lint -esym(715,pinIndex)*/
/*lint -esym(715,filterFrequency */
/*lint -esym(715,nSamples) */
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ********* */
/* Variables */
/* ********* */

#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)

typedef uint8_t  OCDIO_StoredStatus;

#if defined(__GNUC__)
OCDIO_StoredStatus StoredStatus[CONFIG_INIT_PIN_MAX] 		__attribute__ ((section (".noinit")));
OCDIO_StoredStatus StoredStatusCheck[CONFIG_INIT_PIN_MAX] 	__attribute__ ((section (".noinit")));

#elif defined (__CWCC__)
#pragma define_section noinit ".noinit" far_absolute
#if _lint
#define NO_INIT_VAR
#else   /* _lint */
#define NO_INIT_VAR __declspec(noinit)
#endif  /* _lint */
NO_INIT_VAR static  OCDIO_StoredStatus StoredStatus[CONFIG_INIT_PIN_MAX] 		;
NO_INIT_VAR static  OCDIO_StoredStatus StoredStatusCheck[CONFIG_INIT_PIN_MAX] 	;

#elif defined(__ICCARM__)
#define __ram_initialization __iar_data_init3
#pragma location = ".noinit"
 __no_init static OCDIO_StoredStatus StoredStatus[CONFIG_INIT_PIN_MAX] 		;
 __no_init static OCDIO_StoredStatus StoredStatusCheck[CONFIG_INIT_PIN_MAX] 	;
#else
#error "No IDE defined"
static OCDIO_StoredStatus StoredStatus[CONFIG_INIT_PIN_MAX] 		;
static OCDIO_StoredStatus StoredStatusCheck[CONFIG_INIT_PIN_MAX] ;
#endif

const static uint8_t Init_PIN_IDX[CONFIG_INIT_PIN_MAX] = {
#if CONFIG_INIT_PIN_MAX > 0
			CONFIG_INIT_PIN_1 -1
#if CONFIG_INIT_PIN_MAX > 1
			,CONFIG_INIT_PIN_2 -1
#if CONFIG_INIT_PIN_MAX > 2
			,CONFIG_INIT_PIN_3 -1
#if CONFIG_INIT_PIN_MAX > 3
			,CONFIG_INIT_PIN_4 -1
#endif
#endif
#endif
#endif
};
#endif /* #if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS) */


#if defined(PORT_DIRECTION_MIRRORING)
DIO_ModuleMapMaskType PortDirectionMirror[NUM_DIRECTION_REGS];
#endif

/* Exported */
extern const IO_ModuleIOMapType ModuleIOmap[IO_MODULE_MAP_SIZE];

/* Static */

/* ******************** */
/* Functions Prototypes */
/* ******************** */

/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
Perform some useful initializations.

\return 	IO_ErrorType

\retval		IO_E_OK

\par Architecture Requirements ID: SARS_WMMT_MN_DrvDIO_2
*/

IO_ErrorType OCDIO_InitSync(void)
{
	DIO_Init();

#if defined(PORT_DIRECTION_MIRRORING)
	(void)memset(&PortDirectionMirror[0],DFLT_PORT_DIR_VALUE,NUM_DIRECTION_REGS);
#endif


#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)
	{	int i;
		void OCDIO_SetPinValueSyncNoOS(uint8_t pinIndex, uint8_t pinValue);
		for( i=0; i<CONFIG_INIT_PIN_MAX; i++) {
			if((StoredStatus[i] + StoredStatusCheck[i]) == 0xFF) {
				OCDIO_SetPinDirValueSync(Init_PIN_IDX[i], 1);
				OCDIO_SetPinValueSyncNoOS(Init_PIN_IDX[i], StoredStatus[i]);
			}
		}
	}
#endif

	return (IO_ErrorType)IO_E_OK;
}


/**
Deinit all the ports to the default state, M.C.U. reset state.

\return 	IO_ErrorType

\retval		IO_E_OK

\note		After a OCDIO_DeInitSync call it's mandatory to call a OCDIO_InitSync\n
			if you want use the driver.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvDIO_2
*/

IO_ErrorType OCDIO_DeInitSync(void)
{
	DeInitGPIO();
	return (IO_ErrorType)IO_E_OK;
}

/**
 * Check if the pin index refers to a valid pin in the ModuleIOmap
 * \return 	IO_ErrorType
 * \retval		IO_E_OK, IO_E_INVALID_VALUE
 */
IO_ErrorType OCDIO_IsValidPin( uint8_t pinIndex)
{
	const IO_ModuleIOMapType* p = 0;
	if (pinIndex < sizeof(ModuleIOmap)/sizeof(IO_ModuleIOMapType))
	{
		p = OCDIO_GetIOMapItem(pinIndex);
		if(DIO_IsNotDummyIO(p))
		{
			return IO_E_OK;
		}
	}

	return IO_E_INVALID_VALUE;
}
/**
 * Get the pointer to IO_ModuleIOMapType ( port address, and  pin mask )
 * \return 	IO_ModuleIOMapType*
 * \retval		0 ( if pinIndex is invalid ) or pointer to IO_ModuleIOMapType
 * 				that can be also to PIN_DUMMY that is an invalid pin data
 */
const IO_ModuleIOMapType* OCDIO_GetIOMapItem( uint8_t pinIndex)
{
	const IO_ModuleIOMapType* p = 0;
	if (pinIndex < sizeof(ModuleIOmap)/sizeof(IO_ModuleIOMapType))
	{
		 p = &ModuleIOmap[pinIndex];
	}

	return p;
}
/**
Sets the direction of the pin by the index of a table passed via pointer

\param  [in]	map pointer to IOmap
\param  [in]	pinIndex index of the pin in the ModuleIOMap table
\param  [in]	dirValue  value of the direction  (0->Input,!0->Output)

\attention\b    read access to map[] as global

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
\retval		IO_E_INVALID_VALUE   invalid pin

\note		It's high recommended to test return value of this function in order to avoid\n
			calls to the other functions where invalid pins are not tested.

*/
#if defined(PORT_DIRECTION_MIRRORING)
IO_ErrorType OCDIO_SetIOMapPinDirValueSync(IO_ModuleIOMapType* map, uint8_t pinIndex, uint8_t dirValue)
{
	portreg_struct_t *port = (portreg_struct_t *)map[pinIndex].PortAddress.basic;
    IO_ModuleIOMapType* pMap= &map[pinIndex];

#if defined(DIO_DEBUG)
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif
	if (port == (void*)DUMMY_PORT_DATA_ADDR)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}
	if (dirValue)
	{
#if (DIO_OUTPUT_IS_AVAILABLE == TRUE)
        if(IsAvailableAsOutput(pMap))
		{
			DIO_ModuleMapMaskType bit = map[pinIndex].PinMask;
			uint8_t i;
			i = GetMirrorPortDirectionRegister(port);
			PortDirectionMirror[i] |= bit;
			port -> direction = PortDirectionMirror[i];
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_OUTPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
	else
	{
#if (DIO_INPUT_IS_AVAILABLE == TRUE)
        if(IsAvailableAsInput(pMap))
		{
			DIO_ModuleMapMaskType bit = map[pinIndex].PinMask;
			uint8_t i;
			i = GetMirrorPortDirectionRegister(port);
			PortDirectionMirror[i] &= ~bit;
			port -> direction = PortDirectionMirror[i];
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_INPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
}


#else

IO_ErrorType OCDIO_SetIOMapPinDirValueSync(IO_ModuleIOMapType* map, uint8_t pinIndex, uint8_t dirValue)
{
	portreg_struct_t *port = (portreg_struct_t *)map[pinIndex].PortAddress.basic;
    IO_ModuleIOMapType* pMap= &map[pinIndex];

#if defined(DIO_DEBUG)
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif
	if (port == (void*)DUMMY_PORT_DATA_ADDR)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}
	if (dirValue)
	{
#if (DIO_OUTPUT_IS_AVAILABLE == TRUE)
        if(IsAvailableAsOutput(pMap))
		{
            DIO_SetDirAsOutput(pMap);
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_OUTPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
	else
	{
#if (DIO_INPUT_IS_AVAILABLE == TRUE)
        if(IsAvailableAsInput(pMap))
		{

            DIO_SetDirAsInput(pMap);
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_INPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
}
#endif /* defined(PORT_DIRECTION_MIRRORING) */

/**
Sets the direction of the pin of the ModuleIOMap table specified by the index in the table

\param  [in]	pinIndex index of the pin in the ModuleIOMap table
\param  [in]	dirValue  value of the direction  (0->Input,!0->Output)

\attention\b    ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
\retval		IO_E_INVALID_VALUE   invalid pin

\note		It's high reccomended to test return value of this function in order to avoid
			calls to the other functions where invalid pins are not tested.

*/
IO_ErrorType OCDIO_SetPinDirValueSync(uint8_t pinIndex, uint8_t dirValue)
{
	portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[pinIndex].PortAddress.basic;
    const        IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

#if defined(CONFIG_LIB_TRACE_SUPPORT)
    if ( (pinIndex == (CONFIG_TRACE_PIN_1-1)) || (pinIndex == (CONFIG_TRACE_PIN_2-1)) || (pinIndex == (CONFIG_TRACE_PIN_3-1)) || (pinIndex == (CONFIG_TRACE_PIN_4-1)))
	{
		return (IO_ErrorType) IO_E_OK;
	}
#endif

#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
	uint8_t i;
#endif

#if defined(DIO_DEBUG)
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif
	if (port == (void*)DUMMY_PORT_DATA_ADDR)
	{
		return (IO_ErrorType)IO_E_INVALID_VALUE;
	}
	if (dirValue)
	{
#if (DIO_OUTPUT_IS_AVAILABLE == TRUE)
		if(IsAvailableAsOutput(pMap))
		{
#if defined(PORT_DIRECTION_MIRRORING)
			i = GetMirrorPortDirectionRegister(port);
			PortDirectionMirror[i] |= bit;
			port -> direction = PortDirectionMirror[i];
#else
            DIO_SetDirAsOutput(pMap);
#endif
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_OUTPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
	else
	{
#if (DIO_INPUT_IS_AVAILABLE == TRUE)
		if(IsAvailableAsInput(pMap))
		{
#if defined(PORT_DIRECTION_MIRRORING)
			i = GetMirrorPortDirectionRegister(port);
			PortDirectionMirror[i] &= ~bit;
			port -> direction = PortDirectionMirror[i];
#else
            DIO_SetDirAsInput(pMap);
#endif
			return (IO_ErrorType)IO_E_OK;
		}
#endif /* #if (DIO_INPUT_IS_AVAILABLE == TRUE) */
		return (IO_ErrorType)IO_E_UNKNOWN_MODE;
	}
}
/**
Returns the value of the direction of the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_IS_UNKNOWN
\retval		DIO_STATE_PIN_IS_OUTPUT
\retval		DIO_STATE_PIN_IS_INPUT

\note       routine completely rewritten
*/
enum DIO_STATE_PIN OCDIO_GetIODirPinSync(uint8_t pinIndex)
{
	portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[pinIndex].PortAddress.basic;
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
	uint8_t i;
#endif
	enum DIO_STATE_PIN pinState=DIO_STATE_PIN_IS_UNKNOWN;

	if (port != (void*)DUMMY_PORT_DATA_ADDR)
	{
#if ((DIO_OUTPUT_IS_AVAILABLE == TRUE) || (DIO_INPUT_IS_AVAILABLE == TRUE))
		/*lint -e960 */
        if(IsAvailableAsOutput(pMap)||IsAvailableAsInput(pMap))
		{
			/*lint +e960 */
#if defined(PORT_DIRECTION_MIRRORING)
			i = GetMirrorPortDirectionRegister(port);
			if (PortDirectionMirror[i] & bit)
			{
				pinState = DIO_STATE_PIN_IS_OUTPUT;
			}
			else
			{
				pinState = DIO_STATE_PIN_IS_INPUT;
			}
#else
			if(DIO_IsOuput(pMap))
			{
				pinState = DIO_STATE_PIN_IS_OUTPUT;
			}
			else
			{
				pinState = DIO_STATE_PIN_IS_INPUT;
			}
#endif /* #if defined(PORT_DIRECTION_MIRRORING) */
		}
#endif  /* #if ((DIO_OUTPUT_IS_AVAILABLE == TRUE) || (DIO_INPUT_IS_AVAILABLE == TRUE)) */
	}
	return(pinState);
}

/**
Sets the value of the pin of the ModuleIOMap table specified by its index

\param  [in]    pinIndex index of the pin in the ModuleIOMap table. The index is zero-based.
\param  [in]	pinValue  desired value to write on a output pin.

\attention\b    ModuleIOmap[] global used

\note		if necessary, implement port data output mirroring.
\par Architecture Requirements ID: SARS_WMMT_MN_DrvDIO_2
*/
void OCDIO_SetPinValueSync(uint8_t pinIndex, uint8_t pinValue)
{

#if defined(CONFIG_LIB_TRACE_SUPPORT)
     if ( (pinIndex == (CONFIG_TRACE_PIN_1-1)) || (pinIndex == (CONFIG_TRACE_PIN_2-1)) || (pinIndex == (CONFIG_TRACE_PIN_3-1)) || (pinIndex == (CONFIG_TRACE_PIN_4-1)))
		{
			return;
		}
#endif

#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
#endif
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[pinIndex].PortAddress.basic;
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif

	EnterCriticalSection();

	if(pinValue)
	{
#if defined(PORT_DATA_OUTPUT_MIRRORING)

#else
		DIO_SetHigh(pMap);
#endif
	}
	else
	{
#if defined(PORT_DATA_OUTPUT_MIRRORING)

#else
        DIO_SetLow(pMap);
#endif
	}

#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)
	{	int i;
		for( i=0; i<CONFIG_INIT_PIN_MAX; i++) {
			if(Init_PIN_IDX[i] == pinIndex) {
				StoredStatus[i] = pinValue;
				StoredStatusCheck[i] = ~pinValue;
			}
		}
	}
#endif
	ExitCriticalSection();
}

#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)
void OCDIO_SetPinValueSyncNoOS(uint8_t pinIndex, uint8_t pinValue)
{
#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
#endif
const volatile   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[pinIndex].PortAddress.basic;
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif

	if(pinValue)
	{

#if defined(PORT_DATA_OUTPUT_MIRRORING)

#else
		DIO_SetHigh(pMap);
#endif
	}
	else
	{

#if defined(PORT_DATA_OUTPUT_MIRRORING)

#else
        DIO_SetLow(pMap);
#endif
	}

#if defined(CONFIG_OCDIO_INIT_PIN_STORED_STATUS)
	{	int i;
		for( i=0; i<CONFIG_INIT_PIN_MAX; i++) {
			if(Init_PIN_IDX[i] == pinIndex) {
				StoredStatus[i] = pinValue;
				StoredStatusCheck[i] = ~pinValue;
	}
		}
	}
#endif

}
#endif

/**
Get the value of the pin in the table IOMap passed by pointer.

\param  [in]	map pointer to IOmap
\param  [in]    pinIndex zero-based index of the pin in the table map.

\return 	bool_t

\retval		0 low level
\retval     1 high level

\attention\b    read access to map[] as global

\note		if necessary, implement port data input mirroring.

\par Architecture Requirements ID: SARS_WMMT_MN_DrvDIO_2
*/

bool_t OCDIO_GetIOMapPinValueSync(const IO_ModuleIOMapType* map, uint8_t pinIndex)
{
#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = map[pinIndex].PinMask;
#endif
    const   IO_ModuleIOMapType* pMap= &map[pinIndex];
#if defined(DIO_DEBUG)
	portreg_struct_t *port = (portreg_struct_t *)map[pinIndex].PortAddress.basic;
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif

#if defined(PORT_DATA_INPUT_MIRRORING)

#else
        if(DIO_GetVal(pMap))
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}


/**
Get the value of the pin in the table ModuleIOMap.

\param  [in]    pinIndex pin index in the table ModuleIOMap. The index is zero-based.

\return 	bool_t

\retval		0 low level
\retval     1 high level

\attention\b    ModuleIOmap[] global used

\note		if necessary, implement port data input mirroring.
\par Architecture Requirements ID: SARS_WMMT_MN_DrvDIO_2
*/

bool_t OCDIO_GetPinValueSync(uint8_t pinIndex)
{
#if defined(PORT_DIRECTION_MIRRORING)
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
#endif
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	portreg_struct_t *port = (portreg_struct_t *)ModuleIOmap[pinIndex].PortAddress.basic;
	static portreg_struct_t *portMirror;
	portMirror = port;
#endif

#if defined(PORT_DATA_INPUT_MIRRORING)

#else
if(DIO_GetVal(pMap))
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}
/**
Set internal pull up on the pin in the table IOMap passed by pointer.

\param  [in]	map pointer to IOMap
\param  [in]    pinIndex zero-based index of the pin in the table map.

\attention \b 	ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/
IO_ErrorType OCDIO_SetIOMapPinPullUpSync(IO_ModuleIOMapType* map, uint8_t pinIndex)
{
#if (DIO_PULL_UP_IS_AVAILABLE == TRUE)
#if defined(PORT_DATA_PULLING_ENABLE)
/* TICKET_RF003 unused variables invariant AG128  only. not for H8  eventhough PORT_DATA_PULLING_ENABLE is not defined and port is not used*/
    /*portreg_ext_struct_t *port = (portreg_ext_struct_t *)map[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = map[pinIndex].PinMask;*/
#endif
    const   IO_ModuleIOMapType* pMap= &map[pinIndex];
    
#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

	if (DIO_IsAvailablePullUp(pMap))
	{
        DIO_PullUpEnable(pMap);

		return (IO_ErrorType)IO_E_OK;
	}
#endif /* #if (DIO_PULL_UP_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}


/**
Set internal pull up on the pin in the table ModuleIOMap.

\param  [in]    pinIndex pin index in the table ModuleIOMap. The index is zero-based.

\attention \b 	ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

*/
IO_ErrorType OCDIO_SetPinPullUpSync(uint8_t pinIndex)
{
#if (DIO_PULL_UP_IS_AVAILABLE == TRUE)
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

    if(DIO_IsAvailablePullUp(pMap))
	{
        DIO_PullUpEnable(pMap);

		return (IO_ErrorType)IO_E_OK;
	}
#endif /* #if (DIO_PULL_UP_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}
/**
Returns the pullup value related to the pin specified by its index in the ModuleIOMap table.

\param  [in]		pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_PULLUP_IS_ON
\retval		DIO_STATE_PIN_PULLUP_IS_OFF
\retval		DIO_STATE_PIN_PULLUP_IS_NOT_PRESENT

\note       -it's high recommended to call OCDIO_GetIODirPinSync before this routine in order
			 to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOPullUpStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN pullUpState = DIO_STATE_PIN_PULLUP_IS_NOT_PRESENT;
#if (DIO_PULL_UP_IS_AVAILABLE == TRUE)
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsAvailablePullUp(pMap))
	{
		if(DIO_IsPullUpEnabled(pMap))
		{
			pullUpState = DIO_STATE_PIN_PULLUP_IS_ON;
		}
		else
		{
			pullUpState = DIO_STATE_PIN_PULLUP_IS_OFF;
		}
	}
#endif /* #if (DIO_PULL_UP_IS_AVAILABLE == TRUE) */
	return (pullUpState);
}
/**
Remove internal pull up on the pin in the table ModuleIOMap.

\param  [in]    pinIndex pin index in the table ModuleIOMap. The index is zero-based.

\attention \b 	ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/
IO_ErrorType OCDIO_ResetPinPullUpSync(uint8_t pinIndex)
{
#if (DIO_PULL_UP_IS_AVAILABLE == TRUE)
	
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

	if (DIO_IsAvailablePullUp(pMap))
	{
	    DIO_PullUpDisable(pMap);

		return (IO_ErrorType)IO_E_OK;
	}
#endif /* #if (DIO_PULL_UP_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}

/**
Set internal pull down on the pin in the table ModuleIOMap.

\param  [in]    pinIndex pin index in the table ModuleIOMap. The index is zero-based.

\attention \b 	ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/
IO_ErrorType OCDIO_SetPinPullDownSync(uint8_t pinIndex)
{
#if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

	if (DIO_IsAvailablePullDown(pMap))
	{
        DIO_PullDownEnable(pMap);

		return (IO_ErrorType)IO_E_OK;
	}
#else
    (void) pinIndex;
#endif /* #if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}
/**
Remove internal pull down on the pin in the table ModuleIOMap.

\param  [in]    pinIndex pin index in the table ModuleIOMap. The index is zero-based.

\attention \b 	ModuleIOmap[] global used

\return 	IO_ErrorType

\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/
IO_ErrorType OCDIO_ResetPinPullDownSync(uint8_t pinIndex)
{
#if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

	if (DIO_IsAvailablePullDown(pMap))
	{
		DIO_PullDownDisable(pMap);

		return (IO_ErrorType)IO_E_OK;
	}
#else
    (void) pinIndex;
#endif /* #if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}
/**
Returns the pulldown value related to the pin specified by its index in the ModuleIOMap table.

\param  [in]		pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_PULLDOWN_IS_NOT_PRESENT
\retval		DIO_STATE_PIN_PULLDOWN_IS_ON
\retval		DIO_STATE_PIN_PULLDOWN_IS_OFF

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			 to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOPullDownStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN pullDownState = DIO_STATE_PIN_PULLDOWN_IS_NOT_PRESENT;
#if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsAvailablePullDown(pMap))
	{
		/* To check */
        if(DIO_IsPullDownEnabled(pMap))
		{
			pullDownState = DIO_STATE_PIN_PULLDOWN_IS_ON;
		}
		else
		{
			pullDownState = DIO_STATE_PIN_PULLDOWN_IS_OFF;
		}
	}
#else
    (void) pinIndex;
#endif /* #if (DIO_PULL_DOWN_IS_AVAILABLE == TRUE) */
	return (pullDownState);
}

/**
Enables the slew rate related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added check on slew rate functionality
*/
IO_ErrorType OCDIO_SetIOSlewRatePinSync(uint8_t pinIndex)
{
#if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	static portreg_ext_struct_t *portMirror;
	static uint8_t bitMirror;
	portMirror = port;
	bitMirror = bit;
#endif

	if (DIO_IsAvailableSlewRate(pMap))
	{
		port -> slewRate |= bit;
		return (IO_ErrorType)IO_E_OK;
	}
#else /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Disables the slew rate related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added check on slew rate functionality
*/
IO_ErrorType OCDIO_ResetIOSlewRatePinSync(uint8_t pinIndex)
{
#if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsAvailableSlewRate(pMap))
	{
		port -> slewRate &= ~bit;
		return (IO_ErrorType)IO_E_OK;
	}
#else /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}

/**
Returns the value of the slew rate mode related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_SLEWRATE_IS_ON
\retval		DIO_STATE_PIN_SLEWRATE_IS_OFF
\retval		DIO_STATE_PIN_SLEWRATE_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
 	 	 	 to establish that the pin is configured as output
*/
enum DIO_STATE_PIN OCDIO_GetIOSlewRateStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN slwRate = DIO_STATE_PIN_SLEWRATE_IS_NOT_PRESENT;
#if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
        
        if (DIO_IsAvailableSlewRate(pMap))
	{
		if(port -> slewRate & bit)
		{
			slwRate = DIO_STATE_PIN_SLEWRATE_IS_ON;
		}
		else
		{
			slwRate = DIO_STATE_PIN_SLEWRATE_IS_OFF;
		}
	}
#else /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_SLEW_RATE_IS_AVAILABLE == TRUE) */
	return (slwRate);
}

/**
Enables the drive strength mode to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added check on drive strenght functionality
*/
IO_ErrorType OCDIO_SetIODriveStrengthPinSync(uint8_t pinIndex)
{
#if (DIO_DRIVE_STRENGTH == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
#if defined(DIO_DEBUG)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
    static portreg_ext_struct_t *portMirror;
	portMirror = port;
#endif

	if (DIO_IsAvailableDriveStrength(pMap))
	{
    	DIO_DriveStrengthEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else /* #if (DIO_DRIVE_STRENGTH == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_DRIVE_STRENGTH == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Disables the drive strength mode to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added check on drive strenght functionality
*/
IO_ErrorType OCDIO_ResetIODriveStrengthPinSync(uint8_t pinIndex)
{
#if (DIO_DRIVE_STRENGTH == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsAvailableDriveStrength(pMap))
	{
    	DIO_DriveStrengthDisable(pMap);
    	return (IO_ErrorType)IO_E_OK;
	}
#else /* #if (DIO_DRIVE_STRENGTH == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_DRIVE_STRENGTH == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Returns the value of the drive strength mode related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_DRIVESTRENGTH_IS_ON
\retval		DIO_STATE_PIN_DRIVESTRENGTH_IS_OFF
\retval		DIO_STATE_PIN_DRIVESTRENGTH_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			to establish that the pin is configured as output
*/
enum DIO_STATE_PIN OCDIO_GetIODriveStrengthStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN drvStrength = DIO_STATE_PIN_DRIVESTRENGTH_IS_NOT_PRESENT;
#if (DIO_DRIVE_STRENGTH == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsAvailableDriveStrength(pMap))
	{
    	if(DIO_IsDriveStrengthEnabled(pMap))
		{
			drvStrength = DIO_STATE_PIN_DRIVESTRENGTH_IS_ON;
		}
		else
		{
			drvStrength = DIO_STATE_PIN_DRIVESTRENGTH_IS_OFF;
		}
	}
#else /* #if (DIO_DRIVE_STRENGTH == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_DRIVE_STRENGTH == TRUE) */
	return (drvStrength);
}

/**
Disables the passive filter related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_PASSIVE_FILTER_IS_AVAILABLE property \n
            -added  PassiveFilterIsAvaibleOn
*/
IO_ErrorType OCDIO_ResetIOPassiveFilterPinSync(uint8_t pinIndex)
{
#if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;

	if (PassiveFilterIsAvaibleOn(port,bit))
	{
		port -> passiveFilter &= ~bit;
		return (IO_ErrorType)IO_E_OK;
	}

#else  /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Enables the passive filter related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_PASSIVE_FILTER_IS_AVAILABLE property \n
            -added  PassiveFilterIsAvaibleOn
*/
IO_ErrorType OCDIO_SetIOPassiveFilterPinSync(uint8_t pinIndex)
{
#if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;

	if (PassiveFilterIsAvaibleOn(port,bit))
	{
		port -> passiveFilter |= bit;
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Returns the passive filter status related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_PASSIVEFILTER_IS_ON
\retval		DIO_STATE_PIN_PASSIVEFILTER_IS_OFF
\retval		DIO_STATE_PIN_PASSIVEFILTER_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOPassiveFilterStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN passFilter = DIO_STATE_PIN_PASSIVEFILTER_IS_NOT_PRESENT;
#if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE)
	portreg_ext_struct_t *port = (portreg_ext_struct_t *)ModuleIOmap[pinIndex].PortAddress.extended;
	DIO_ModuleMapMaskType bit = ModuleIOmap[pinIndex].PinMask;

	if (PassiveFilterIsAvaibleOn(port,bit))
	{
		if(port -> passiveFilter & bit)
		{
			passFilter = DIO_STATE_PIN_PASSIVEFILTER_IS_ON;
		}
		else
		{
			passFilter = DIO_STATE_PIN_PASSIVEFILTER_IS_OFF;
		}
	}
#else /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_PASSIVE_FILTER_IS_AVAILABLE == TRUE) */
	return (passFilter);
}

/**
Enables the interrupt related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn \n

\todo:      vector interrupt and related routine
*/
IO_ErrorType OCDIO_SetIOInterruptPinSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
    	DIO_InterruptEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Sets the rising edge, for the interrupt, related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn
*/
IO_ErrorType OCDIO_SetIORisingEdgePinSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
        DIO_SetInterruptMode_RisingEdge(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Sets the falling edge related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn
*/
IO_ErrorType OCDIO_SetIOFallingEdgePinSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
    	DIO_SetInterruptMode_FallingEdge(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Returns the type of the edge related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_INTRPT_EDGE_IS_RISING
\retval		DIO_STATE_PIN_INTRPT_EDGE_IS_FALLING
\retval		DIO_STATE_PIN_INTRPT_EDGE_IS_NOT_PRESENT

\note       -routine completely rewritten
\n          -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
\n           to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOEdgeStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN edge = DIO_STATE_PIN_INTRPT_EDGE_IS_NOT_PRESENT;
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		if(DIO_IsInterruptMode_RisingEdge(pMap))
		{
			edge = DIO_STATE_PIN_INTRPT_EDGE_IS_RISING;
		}
		else
		{
			edge = DIO_STATE_PIN_INTRPT_EDGE_IS_FALLING;
		}
	}
#else /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (edge);
}

/**
Sets the level detection mode of the port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn and DIO_LevelDetectIntrpt \n
*/

IO_ErrorType OCDIO_SetPortLevelDetectSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		DIO_LevelDetectInterruptEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
		
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}

/**
Sets the edge detection mode of the port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn and DIO_EdgeDetectIntrpt
*/

IO_ErrorType OCDIO_SetPortEdgeDetectSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		DIO_EdgeDetectInterruptEnable(pMap);		
		return (IO_ErrorType)IO_E_OK;
		
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}
/**
Reads the detection mode of the port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_DETECT_MODE_INTRPT_IS_LEVEL
\retval		DIO_STATE_DETECT_MODE_INTRPT_IS_EDGE
\retval		DIO_STATE_DETECT_MODE_INTRPT_IS_NOT_PRESENT

*/

enum DIO_STATE_PIN OCDIO_GetPortDetectStatusSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN detectModePinIntrpt = DIO_STATE_DETECT_MODE_INTRPT_IS_NOT_PRESENT;
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
	uint8_t tempDetectMode;
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		tempDetectMode = (uint8_t) DIO_GetDetectInterruptMode(pMap);
		if (tempDetectMode)
		{
			if(1 == tempDetectMode)
			{
				detectModePinIntrpt = DIO_STATE_DETECT_MODE_INTRPT_IS_LEVEL;
			}
			else if(2 == tempDetectMode)
			{
				detectModePinIntrpt = DIO_STATE_DETECT_MODE_INTRPT_IS_EDGE;
			}
			else
			{

			}
		}
	}
#else /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
   	return (detectModePinIntrpt);
}

/**
Disables the interrupt related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn \n
*/
IO_ErrorType OCDIO_ResetIOInterruptPinSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		DIO_InterruptDisable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Returns the interrupt value related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_INTRPT_IS_ON
\retval		DIO_STATE_PIN_INTRPT_IS_OFF
\retval		DIO_STATE_PIN_INTRPT_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			 to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOInterruptStatePinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN intrptPin = DIO_STATE_PIN_INTRPT_IS_NOT_PRESENT;
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))

	{
		if(DIO_IsInterruptEnabled(pMap))
		{
			intrptPin = DIO_STATE_PIN_INTRPT_IS_ON;
		}
		else
		{
			intrptPin = DIO_STATE_PIN_INTRPT_IS_OFF;
		}
	}
#else /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (intrptPin);
}
/**
Resets the interrupt flag related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property\n
            -added  DIO_InterruptIsAvaibleOn \n
*/
IO_ErrorType OCDIO_ResetIOInterruptFlagPinSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		DIO_ClearInterrupt(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Returns the interrupt flag related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_INTRPT_FLG_IS_ON
\retval		DIO_STATE_PIN_INTRPT_FLG_IS_OFF
\retval		DIO_STATE_PIN_INTRPT_FLG_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIOInterruptFlagPinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN intrptFlgPin = DIO_STATE_PIN_INTRPT_FLG_IS_NOT_PRESENT;
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		if(DIO_IsInterruptFlag(pMap))
		{
			intrptFlgPin = DIO_STATE_PIN_INTRPT_FLG_IS_ON;
		}
		else
		{
			intrptFlgPin = DIO_STATE_PIN_INTRPT_FLG_IS_OFF;
		}
	}
#else /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (intrptFlgPin);
}
/**
Enables interrupt of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn  \n
            -added DIO_EnableInterruptOnPort

*/

IO_ErrorType OCDIO_EnablePortInterruptSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
    	DIO_InterruptEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}

/**
Disables interrupt of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_INTERRUPT_IS_AVAILABLE property \n
            -added  DIO_InterruptIsAvaibleOn \n
            -added DIO_DisableInterruptOnPort

*/

IO_ErrorType OCDIO_DisablePortInterruptSync(uint8_t pinIndex)
{
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		DIO_InterruptDisable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}
/**
Reads the interrupt status of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]		pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PORT_INTRPT_IS_ON
\retval		DIO_STATE_PORT_INTRPT_IS_OFF
\retval		DIO_STATE_PORT_INTRPT_IS_NOT_PRESENT

*/

enum DIO_STATE_PIN OCDIO_GetPortInterruptStatusSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN intrptEnPort = DIO_STATE_PORT_INTRPT_IS_NOT_PRESENT;
#if (DIO_INTERRUPT_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsInterruptAvailable(pMap))
	{
		if(DIO_IsInterruptEnabled(pMap))
		{
			intrptEnPort = DIO_STATE_PORT_INTRPT_IS_ON;
		}
		else
		{
			intrptEnPort = DIO_STATE_PORT_INTRPT_IS_OFF;
		}
	}
#else /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
    (void)pinIndex;
#endif /* #if (DIO_INTERRUPT_IS_AVAILABLE == TRUE) */
	return (intrptEnPort);
}
/**
Enables DMA of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_DMA_IS_AVAILABLE properties \n
            -added  DIO_DMA_IsAvaibleOn \n
            -added DIO_EnableDMAOnPort

*/

IO_ErrorType OCDIO_EnablePortDMASync(uint8_t pinIndex)
{
#if (DIO_DMA_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsDmaAvailable(pMap))
	{
		DIO_DmaEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}

#else  /* #if (DIO_DMA_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_DMA_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Disables DMA of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]		pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_DMA_IS_AVAILABLE properties \n
            -added  DIO_DMA_IsAvaibleOn  \n
            -added DIO_DisableDMAOnPort

*/

IO_ErrorType OCDIO_DisablePortDMASync(uint8_t pinIndex)
{
#if (DIO_DMA_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsDmaAvailable(pMap))
	{
		DIO_DmaDisable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_DMA_IS_AVAILABLE == TRUE)  */
	(void) pinIndex;
#endif  /* #if (DIO_DMA_IS_AVAILABLE == TRUE)  */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;

}

/**
Reads the DMA status of a port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PORT_DMA_IS_NOT_PRESENT
\retval		DIO_STATE_PORT_DMA_IS_ON
\retval		DIO_STATE_PORT_DMA_IS_OFF

*/
enum DIO_STATE_PIN OCDIO_GetPortDMAStatusSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN dmaPortStatus = DIO_STATE_PORT_DMA_IS_NOT_PRESENT;
#if (DIO_DMA_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];
	if (DIO_IsDmaAvailable(pMap))
	{
		if(DIO_IsDmaEnabled(pMap))
		{
			dmaPortStatus = DIO_STATE_PORT_DMA_IS_ON;
		}
		else
		{
			dmaPortStatus = DIO_STATE_PORT_DMA_IS_OFF;
		}
	}
#else  /* #if (DIO_DMA_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_DMA_IS_AVAILABLE == TRUE) */
	return(dmaPortStatus);

}

/**
Enables the digital filter related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_DIGITAL_FILTER_IS_AVAILABLE property \n
            -added  DIO_DigitalFilterIsAvaibleOn \n
*/
IO_ErrorType OCDIO_SetIODigitalFilterPinSync(uint8_t pinIndex)
{
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
        DIO_DigitalFilterEnable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Disables the digital filter related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE

\note       -added  DIO_DIGITAL_FILTER_IS_AVAILABLE property \n
            -added  DIO_DigitalFilterIsAvaibleOn \n
*/
IO_ErrorType OCDIO_ResetIODigitalFilterPinSync(uint8_t pinIndex)
{
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))

	{
		DIO_DigitalFilterDisable(pMap);
		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Returns the digital filter status related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	enum DIO_STATE_PIN

\retval		DIO_STATE_PIN_DIGITAL_FLT_IS_ON
\retval		DIO_STATE_PIN_DIGITAL_FLT_IS_OFF
\retval		DIO_STATE_PIN_DIGITAL_FLT_IS_NOT_PRESENT

\note       -it's high reccomended to call OCDIO_GetIODirPinSync before this routine in order
			to establish that the pin is configured as input
*/
enum DIO_STATE_PIN OCDIO_GetIODigitalFilterPinSync(uint8_t pinIndex)
{
	enum DIO_STATE_PIN digitalFlt = DIO_STATE_PIN_DIGITAL_FLT_IS_NOT_PRESENT;
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)
    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
		if(DIO_IsDigitalFilterEnabled(pMap))
		{
			digitalFlt = DIO_STATE_PIN_DIGITAL_FLT_IS_ON;
		}
		else
		{
			digitalFlt = DIO_STATE_PIN_DIGITAL_FLT_IS_OFF;
		}
	}
#else /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (digitalFlt);
}

/**
Set the digital filter frequency, closer to that request, related to the pin specified by its index in the ModuleIOMap table

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.
\param  [in]    filterFrequency   frequency of digital filter in KHz.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/
IO_ErrorType OCDIO_SetIODigitalFilterFrequencySync(uint8_t pinIndex,uint16_t filterFrequency)
{
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)

    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
		DIO_DigitalFilterClkFrequencySet(pMap, filterFrequency);
   		return (IO_ErrorType)IO_E_OK;
	}
#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}
/**
Return the real digital filter frequency related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	uint16_t

\retval     frequency of digital filter in KHz, if supported, otherwise 0
*/

uint16_t OCDIO_GetPortDigitalFilterModeSync(uint8_t pinIndex)
{
	uint32_t digitalFilterFrequency = 0;
	
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)
const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
		digitalFilterFrequency = DIO_DigitalFilterClkFrequencyGet(pMap);
	}
#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (uint16_t) digitalFilterFrequency;
}


/**
Sets the digital filter samples, closer to that request, of the port related to the pin specified \n
by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.
\param  [in]	nSamples  number of samples to perform.

\return     IO_ErrorType
\retval		IO_E_OK
\retval		IO_E_UNKNOWN_MODE
*/

IO_ErrorType OCDIO_SetPortDigitalFilterNsamplesSync(uint8_t pinIndex, uint8_t nSamples)
{
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)

    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
		if ( 0 == nSamples )
		{
			return (IO_ErrorType)IO_E_INVALID_VALUE;
		}
		else
		{
			DIO_DigitalFilterNumSamplesSet(pMap,nSamples);
			return (IO_ErrorType)IO_E_OK;
		}
	}

#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	(void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return (IO_ErrorType)IO_E_UNKNOWN_MODE;
}

/**
Get the digital filter samples of the port related to the pin specified by its index in the ModuleIOMap table.

\param  [in]	pinIndex  pin zero-based index in the in the ModuleIOMap table.

\return 	uint8_t

\retval		nSamples othewise 0 if not supported
*/

uint8_t OCDIO_GetPortDigitalFilterNsamplesSync(uint8_t pinIndex)
{
	uint8_t nSamples = 0;
#if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE)

    const   IO_ModuleIOMapType* pMap= &ModuleIOmap[pinIndex];

	if (DIO_IsDigitalFilterAvailable(pMap))
	{
		nSamples = DIO_DigitalFilterNumSamplesGet(pMap);
	}
#else  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
    (void) pinIndex;
#endif  /* #if (DIO_DIGITAL_FILTER_IS_AVAILABLE == TRUE) */
	return nSamples;
}



/*--------------------------------------------------------------------------------------*/

/**
Funzioni DRIVER per la Mcu
*/

#ifdef DIO_DEBUG_ALL

/**
Setta il valore dei pin di una intera porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

void OCDIO_SetMcuPortSync(const InOutDriverParamType *pParam)
{

	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIOPin(port_id, port_val);
	}

}


/**
Legge il valore dei pin di una intera porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\return 	PortValueType

\retval		Qualsiasi valore tra 0x00 e 0xFF

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

uint8_t OCDIO_GetMcuPortSync(const InOutDriverParamType *pParam)
{

	uint8_t port_id = (*pParam).PortId;

	return HAL_GetMcuIOVal(port_id);

}


/**
Legge il valore di un pin di una porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

bool_t OCDIO_GetMcuPortPinSync(const InOutDriverParamType *pParam)
{

	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOValPin(port_id, port_val);

}


/**
Configura il valore di direzione per i pin di una intera porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

void OCDIO_SetMcuPortDirSync(const InOutDriverParamType *pParam)
{

	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIODirPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIODirPin(port_id, port_val);
	}

}


/**
Legge il valore di direzione per un pin di una porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

bool_t OCDIO_GetMcuPortPinDirSync(const InOutDriverParamType *pParam)
{

	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIODirPin(port_id, port_val);
}


/**
Cunfigures the pulling mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_SetMcuPortPullingSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOPullingPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIOPullingPin(port_id, port_val);
	}
}


/**
Reads the value of the pulling mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinPullingSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOPullingPin(port_id, port_val);
}

/**
Configura il valore di pullup per i pin di una intera porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

void OCDIO_SetMcuPortPullupSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOPullupPin(port_id, port_val);
	}
	else
	{
		HAL_SetMcuIOPulldownPin(port_id, port_val);
	}
}


/**
Legge il valore di pullup per un pin di una porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/


bool_t OCDIO_GetMcuPortPinPullupSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOPullupPin(port_id, port_val);
}


/**
Configura il valore di slew-rate per i pin di una intera porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

void OCDIO_SetMcuPortSlewRateSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOSlewRatePin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIOSlewRatePin(port_id, port_val);
	}
}


/**
Legge il valore di slew-rate di un pin di una porta

\pParam  [in]		pParam puntatore alla struttura che contiene i parametri relativi al pin e al valore di setting

\attention \b Accesso diretto al microcontrollore

\return 	bool_t

\retval		0, 1

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug

*/

bool_t OCDIO_GetMcuPortPinSlewRateSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOSlewRatePin(port_id, port_val);
}

/**
Cunfigures the drive strength mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_SetMcuPortDriveStrengthSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIODriveStrengthPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIODriveStrengthPin(port_id, port_val);
	}
}


/**
Reads the drive strength mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinDriveStrengthSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIODriveStrengthPin(port_id, port_val);
}


/**
Cunfigures the passive filter mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
*/

void OCDIO_SetMcuPortPassiveFilterSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOPassiveFilterPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIOPassiveFilterPin(port_id, port_val);
	}
}


/**
Reads the passive filter mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinPassiveFilterSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOPassiveFilterPin(port_id, port_val);
}

/**
Cunfigures the interrupt mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_SetMcuPortInterruptSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIOInterruptPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIOInterruptPin(port_id, port_val);
	}
}

/**
Reads the interrupt mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinInterruptSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOInterruptPin(port_id, port_val);
}


/**
Resets the interrupt flag of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_ResetMcuPortInterruptFlagSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_ResetMcuIOInterruptPin(port_id, port_val);
	}
}

/**
Reads the interrupt flag of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinInterruptFlagSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOInterruptFlagPin(port_id, port_val);
}


/**
Cunfigures the edge mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_SetMcuPortEdgeSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIORisingEdgePin(port_id, port_val);
	}
	else
	{
		HAL_SetMcuIOFallingEdgePin(port_id, port_val);
	}
}


/**
Reads the edge mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinEdgeSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIOEdgePin(port_id, port_val);
}


/**
Cunfigures the digital filter mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

void OCDIO_SetMcuPortDigitalFilterSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	if((*pParam).PinValue==1)
	{
		HAL_SetMcuIODigitalFilterPin(port_id, port_val);
	}
	else
	{
		HAL_ResetMcuIODigitalFilterPin(port_id, port_val);
	}
}


/**
Reads the digital filter mode of a pin of a port

\pParam  [in]		pParam pointer to the structure that contains parameters of a pin and its value

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only

*/

bool_t OCDIO_GetMcuPortPinDigitalFilterSync(const InOutDriverParamType *pParam)
{
	uint8_t port_id = (*pParam).PortId;

	uint8_t port_val = OCDIO_GetIOPinMask((*pParam).PinNumber);

	return HAL_GetMcuIODigitalFilterPin(port_id, port_val);
}


#endif /* DIO_DEBUG_ALL */

/* Local Function */

/* *********** */
/* End of file */
/* *********** */
