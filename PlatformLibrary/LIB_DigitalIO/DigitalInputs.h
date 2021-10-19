/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par		DigitalInputs.h
\n			header file of DigitalInputs.c
\n
\file		DigitalInputs.h
\ingroup	DigitalInputs
\date		Jun 11, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.04.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n 		01.03.00
\n		dd/mm/yyyy
\n		developer
\n		changes
\n 		- change 1
\n		- change 2
\n
\n
\n etc.
*/

#ifndef __DIGITAL_INPUTS_H
#define __DIGITAL_INPUTS_H

/* ********** */
/* Includes */
/* ********* */
#include "StandardTypes.h"
#include "DigitalInputsExp.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/*<belonging to the module, visible to the whole Platform project>*/
#define NO_MPX_INPUT    0   /* the input does not belong to multiplexer */
#define MPX_INPUT       1   /* the input belong to multiplexer */
/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/*<belonging to the module, visible to the Platform project>*/
/* Digital L.L.I. params field Setting File structure */
typedef union digitalLLIparams_tag
{
	uint16_t wrd;

	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic: 1;
		uint8_t unused_0:  4;
		uint8_t nRead:     3;
		/* Low Byte */
		uint8_t inverted:  1;
		uint8_t unused_2:  1;
		uint8_t pullUp:    1;
		uint8_t unused_1:  5;
#else
		/* High Byte */
		uint8_t nRead:     3;
		uint8_t unused_0:  4;
		uint8_t automatic: 1;
		/* Low Byte */
		uint8_t unused_1:  5;
		uint8_t pullUp:    1;
		uint8_t unused_2:  1;
		uint8_t inverted:  1;
#endif
	} bit;

} digitalLLIparams_t;


/* Digital Multiplexed L.L.I. params field Setting File structure */
typedef union digitalMpxLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic: 1;
		uint8_t unused_0:  4;
		uint8_t nRead:     3;
		/* Low Byte */
		uint8_t inverted:  1;
		uint8_t unused_2:  1;
		uint8_t pullUp:    1;
		uint8_t unused_1:  2;		
		uint8_t mpxCfg:    3;
#else
		/* High Byte */
		uint8_t nRead:     3;
		uint8_t unused_0:  4;
		uint8_t automatic: 1;
		/* Low Byte */
		uint8_t mpxCfg:    3;
		uint8_t unused_1:  2;
		uint8_t pullUp:    1;
		uint8_t unused_2:  1;
		uint8_t inverted:  1;
#endif				
	} bit;
} digitalMpxLLIparams_t;

/* Data structure for digital inputs  */
typedef union
{
	uint16_t w;
	struct
	{
		/* if '1' input read enabled */
		uint8_t active:1;
		/* if '1' multiplexed input otherwise simple input */
		uint8_t readtype:  1;
		/* (1<<filterPot2) = numbers of sample before outputting a new data  */
		uint8_t filterPot2:3;
		/* Mpx configuration in order to obtain the desired input */
		uint8_t chsel:3;
		/* pin linked to the input */
		uint8_t pin:       6;
		/* If '1' the input must be inverted (used only with mpx) */
		uint8_t inversion:1;
		/* If '1' the input linked to the mpx output must be configured as input with pull up */
		uint8_t pullupEn:1;
	} s;
} digitalInput_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/*<belonging to the module, visible to the Platform project, keyword extern used>*/

/* ******************* */
/* Function prototypes */
/* ******************* */
/*<belonging to the module, visible to the Platform project>*/
void LIB_DIO_InitDigInputs(void);
void LIB_DIO_InsertDigInput ( uint8_t pos, uint8_t pin, uint8_t readtype, const uint8_t *pConfigData );
void LIB_DIO_DigInputManager(void);
bool_t LIB_DIO_GET_INPUT( uint8_t pin );
bool_t LIB_DIO_GET_INPUT_F( uint8_t pin, int8_t filterValue );
#endif /* __DIGITAL_INPUTS_H */

/* *********** */
/* End of file */
/* *********** */
