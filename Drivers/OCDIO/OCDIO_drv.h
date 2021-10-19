/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		    OCDIO
\n              On Chip Digital IO Driver Header file
\n			    Contains defines, typedef, prototypes of the Digital IO driver
\n
\file		    OCDIO_drv.h
\ingroup	    OCDIO
\date		    13/05/2010
\version	    00.00.02
\author		    Stefano Ughi
\n
\n
\par	History:
\n 		        00.00.01
\n          	3/02/2010
\n          	Jaroslav Musil
\n          	creation
\n
*/


#ifndef __OCDIO_HAL_H
#define __OCDIO_HAL_H


#define __DRIVER_H

/* ******** */
/* Includes */
/* ******** */

/*lint -e537*/
#include "HAL_Interface.h"
#include  HAL_CFG
#include  HAL
#include "IO_driver.h"
/*lint +e537*/

/* ****************** */
/* Defines and Macros */
/* ****************** */
#if !defined(DIO_PULL_UP_IS_AVAILABLE)
#error " DIO_PULL_UP_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_PULL_DOWN_IS_AVAILABLE)
#error " DIO_PULL_DOWN_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_INPUT_IS_AVAILABLE)
#error " DIO_INPUT_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_SLEW_RATE_IS_AVAILABLE)
#error " DIO_SLEW_RATE_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_DRIVE_STRENGTH)
#error " DIO_DRIVE_STRENGTH not defined! "
#endif
#if !defined(DIO_OUTPUT_IS_AVAILABLE)
#error " DIO_OUTPUT_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_PASSIVE_FILTER_IS_AVAILABLE)
#error " DIO_PASSIVE_FILTER_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_INTERRUPT_IS_AVAILABLE)
#error " DIO_INTERRUPT_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_DMA_IS_AVAILABLE)
#error " DIO_DMA_IS_AVAILABLE not defined! "
#endif
#if !defined(DIO_DIGITAL_FILTER_IS_AVAILABLE)
#error " DIO_DIGITAL_FILTER_IS_AVAILABLE not defined! "
#endif


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

IO_ErrorType OCDIO_InitSync(void);
IO_ErrorType OCDIO_DeInitSync(void);

const IO_ModuleIOMapType*	OCDIO_GetIOMapItem( uint8_t item);
IO_ErrorType 				OCDIO_IsValidPin( uint8_t pinIndex);

IO_ErrorType OCDIO_SetPinDirValueSync(uint8_t pinIndex, uint8_t dirValue);
IO_ErrorType OCDIO_SetIOMapPinDirValueSync(IO_ModuleIOMapType* map, uint8_t pinIndex, uint8_t dirValue);

void OCDIO_SetPinValueSync(uint8_t pinIndex, uint8_t pinValue);

bool_t OCDIO_GetPinValueSync(uint8_t pinIndex);
bool_t OCDIO_GetIOMapPinValueSync(const IO_ModuleIOMapType* map, uint8_t pinIndex);

IO_ErrorType OCDIO_SetPinPullDownSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetIOMapPinPullUpSync(IO_ModuleIOMapType* map, uint8_t pinIndex);
IO_ErrorType OCDIO_SetPinPullUpSync(uint8_t pinIndex);

IO_ErrorType OCDIO_SetIOSlewRatePinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_ResetIOSlewRatePinSync(uint8_t pinIndex);

IO_ErrorType OCDIO_SetIODriveStrengthPinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_ResetIODriveStrengthPinSync(uint8_t pinIndex);

IO_ErrorType OCDIO_ResetIOPassiveFilterPinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetIOPassiveFilterPinSync(uint8_t pinIndex);

IO_ErrorType OCDIO_SetPortLevelDetectSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetPortEdgeDetectSync(uint8_t pinIndex);


IO_ErrorType OCDIO_SetIOInterruptPinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetIORisingEdgePinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetIOFallingEdgePinSync(uint8_t pinIndex);

IO_ErrorType OCDIO_EnablePortInterruptSync(uint8_t pinIndex);
IO_ErrorType OCDIO_DisablePortInterruptSync(uint8_t pinIndex);

IO_ErrorType OCDIO_ResetIOInterruptPinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_ResetIOInterruptFlagPinSync(uint8_t pinIndex);

IO_ErrorType OCDIO_EnablePortDMASync(uint8_t pinIndex);
IO_ErrorType OCDIO_DisablePortDMASync(uint8_t pinIndex);

IO_ErrorType OCDIO_ResetPinPullUpSync(uint8_t pinIndex);
IO_ErrorType OCDIO_ResetPinPullDownSync(uint8_t pinIndex);

IO_ErrorType OCDIO_SetIODigitalFilterPinSync(uint8_t pinIndex);
IO_ErrorType OCDIO_SetIODigitalFilterFrequencySync(uint8_t pinIndex,uint16_t filterFrequency);
IO_ErrorType OCDIO_SetPortDigitalFilterNsamplesSync(uint8_t pinIndex, uint8_t nSamples);

IO_ErrorType OCDIO_ResetIODigitalFilterPinSync(uint8_t pinIndex);



/* Diagnostic routines */
enum DIO_STATE_PIN OCDIO_GetIODirPinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOPullUpStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOPullDownStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIODriveStrengthStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOSlewRateStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOPassiveFilterStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOInterruptStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOInterruptFlagPinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIOEdgeStatePinSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetIODigitalFilterPinSync(uint8_t pinIndex);
uint16_t OCDIO_GetPortDigitalFilterModeSync(uint8_t pinIndex);
uint8_t OCDIO_GetPortDigitalFilterNsamplesSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetPortInterruptStatusSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetPortDMAStatusSync(uint8_t pinIndex);
enum DIO_STATE_PIN OCDIO_GetPortDetectStatusSync(uint8_t pinIndex);





#ifdef DIO_DEBUG_ALL

/*funzioni usate per debug (controllo per porte intere di pin del mcro)*/

void OCDIO_SetMcuPortSync(const InOutDriverParamType *pParam);
uint8_t OCDIO_GetMcuPortSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortDirSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinDirSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortPullingSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinPullingSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortPullupSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinPullupSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortSlewRateSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinSlewRateSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortDriveStrengthSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinDriveStrengthSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortPassiveFilterSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinPassiveFilterSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortInterruptSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinInterruptSync(const InOutDriverParamType *pParam);

void OCDIO_ResetMcuPortInterruptFlagSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinInterruptFlagSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortEdgeSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinEdgeSync(const InOutDriverParamType *pParam);

void OCDIO_SetMcuPortDigitalFilterSync(const InOutDriverParamType *pParam);
bool_t OCDIO_GetMcuPortPinDigitalFilterSync(const InOutDriverParamType *pParam);

#endif /*  DIO_DEBUG_ALL */

#endif /* __OCDIO_HAL_H */



/* *********** */
/* End of file */
/* *********** */
