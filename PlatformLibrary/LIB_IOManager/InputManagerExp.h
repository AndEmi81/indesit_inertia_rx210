/*
Copyright (c) 2004-2005, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\par		InputManagerExp.h
\n			This file contains all defines,macro,typedef and prototypes that Application
\n          can use.
\n          Access to the Platform's Variables without the functions decribed below is forbidden.
\n
\file		InputManagerExp.h
\ingroup	InputsManager
\date		Jun 8, 2010
\version	01.00.00
\author		stefano.ughi
\n
\n
\Par\b	History:
\n
\n 		01.00.00
\n		Jun 8, 2010
\n		Stefano Ughi
\n		changes
\n 		- creation
\n
*/

#ifndef __INPUT_MANAGER_EXP_H
#define __INPUT_MANAGER_EXP_H

/* ******** */
/* Includes */
/* ******** */
#include "StandardTypes.h"
/* ****************** */
/* Defines and Macros */
/* ****************** */
/* <belonging to the module, visible to the Application project> */
/* Read Type */
#define DIGITAL_I    	    0x00
#define ANALOG_I     	    0x01
#define FEEDBACK_I   	    0x02
#define I2C_I        	    0x03
#define KEY_I		 	    0x04
#define TIME_I       	    0x05
#define ENCODER_I           0x06
#define FREQFEEDBACK_I      0x07
#define TACHO_I			    0x08
#define PRESSLIN_I          0x09
#define ZEROCROSSING_I      0x0A
#define PULSECNTR_I         0x0B
#define TURBIDITY_I         0x0C
#define ANALOGFEEDBACK_I    0x0D
#define LONGFILL_I          0x0E
#define DDC_I               0x0F	/* Dynamic Demand Control */
#define CONDUCTIVITYSENSOR_I  0x0E
#define DIGMULT_I	    	0x10
#define FEEDMULT_I  	 	0x12
#define TIMEMULT_I	 		0x15

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/* <belonging to the module, visible to the Application project> */
/** frequency Feedback L.L.I. Setting File Params */
typedef union freqFeedbackLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Word */
		uint8_t automatic: 1;
		uint8_t unused_0:  4;		
		uint8_t nRead:     3;
		/* Low Byte */
		uint8_t inverted:  1;
		uint8_t unused_2:  1;		
		uint8_t pullUp:    1;		
		uint8_t unused_1:  5;
#else	
		/* High Word */
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
} freqFeedbackLLIparams_t;
/** Tachometer L.L.I. Setting File Params */
typedef union tachoLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t unused_0:  8;
		/* Low Byte */
		uint8_t unused_2:  2;
		uint8_t pullUp:    1;
		uint8_t unused_1:  5;
#else	
		/* High Byte */
		uint8_t unused_0:  8;
		/* Low Byte */
		uint8_t unused_1:  5;
		uint8_t pullUp:    1;
		uint8_t unused_2:  2;
#endif
	} bit;
} tachoLLIparams_t;
/** Linear Pressure Sensor L.L.I. Setting File Params */
typedef union linearPressureSensLLIparams_tag
{
	uint16_t wrd[4];
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* 1st word */
		/* High Byte */
		uint8_t automatic: 1;
		uint8_t unused_0:  4;		
		uint8_t nRead:     3;

		/* Low Byte */
		uint8_t unused_2:  2;
		uint8_t pullUp:    1;		
		uint8_t unused_1:  5;

		/* 2nd word */
		uint8_t  tOut;
		uint8_t  unused_3;

		/* 3rd word */  /* TODO MS NOTA: verificare se alloca cosi i bitfield*/
		uint16_t unused_4:  1;
		uint16_t tMax:      15;		
		/* 4th word */
		uint16_t unused_5:  1;
		uint16_t tMin:      15;
#else
		
		/* 1st word */
		/* High Byte */
		uint8_t nRead:     3;
		uint8_t unused_0:  4;
		uint8_t automatic: 1;
		/* Low Byte */
		uint8_t unused_1:  5;
		uint8_t pullUp:    1;
		uint8_t unused_2:  2;

		/* 2nd word */
		uint8_t  tOut;
		uint8_t  unused_3;

		/* 3rd word */
		uint16_t tMax:      15;
		uint16_t unused_4:  1;
		/* 4th word */
		uint16_t tMin:      15;
		uint16_t unused_5:  1;
#endif		
	} bit;
} linearPressureSensLLIparams_t;

/** Pulse Counter L.L.I. Setting File Params  */
typedef union pulseCounterLLIparams_tag
{
	uint16_t wrd[5];
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* 1st word */
		/* High Byte */
		uint8_t unused_0:   8;
		/* Low Byte */
		uint8_t unused_2:   2;
		uint8_t pullUp:     1;		
		uint8_t unused_1:   5;
		/* 2nd word */
		uint16_t t1;
		/* 3rd word */
		uint16_t t2;
		/* 4th word*/
		uint16_t t3;
		/* 5th word */
		uint8_t  n;
		uint8_t  unused_3;
#else		
		/* 1st word */
		/* High Byte */
		uint8_t unused_0:   8;
		/* Low Byte */
		uint8_t unused_1:   5;
		uint8_t pullUp:     1;
		uint8_t unused_2:   2;

		/* 2nd word */
		uint16_t t1;
		/* 3rd word */
		uint16_t t2;
		/* 4th word*/
		uint16_t t3;
		/* 5th word */
		uint8_t  n;
		uint8_t  unused_3;
#endif		
	} bit;
} pulseCounterLLIparams_t;
/** Time  L.L.I. Setting File Params */
typedef union timeLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic:  1;
		uint8_t unused_0:   4;
		uint8_t nRead:      3;

		/* Low Byte */
		uint8_t unused_2:   1;		
		uint8_t delta:      4;		
		uint8_t unused_1:   3;
#else	
		/* High Byte */
		uint8_t nRead:      3;
		uint8_t unused_0:   4;
		uint8_t automatic:  1;
		/* Low Byte */
		uint8_t unused_1:   3;
		uint8_t delta:      4;
		uint8_t unused_2:   1;
#endif

	} bit;
} timeLLIparams_t;
/** Time Multiplexed L.L.I. Setting File Params */
typedef union timeMpxLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t automatic:  1;		
		uint8_t unused_0:   4;		
		uint8_t nRead:      3;
		/* Low Byte */
		uint8_t unused_1:   1;		
		uint8_t delta:      4;		
		uint8_t mpxCfg:     3;
#else		
		/* High Byte */
		uint8_t nRead:      3;
		uint8_t unused_0:   4;
		uint8_t automatic:  1;
		/* Low Byte */
		uint8_t mpxCfg:     3;
		uint8_t delta:      4;
		uint8_t unused_1:   1;
#endif
	} bit;
} timeMpxLLIparams_t;
/** Key L.L.I. Setting File Params  */
typedef union keyLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t unused_0:   5;		
		uint8_t nRead:      3;
		/* Low Byte */
		uint8_t inverted:   1;		
		uint8_t unused_1:   7;
#else		
		/* High Byte */
		uint8_t nRead:      3;
		uint8_t unused_0:   5;
		/* Low Byte */
		uint8_t unused_1:   7;
		uint8_t inverted:   1;
#endif
	} bit;
} keyLLIparams_t;
/** feedback L.L.I. Setting File Params */
typedef union feedbackLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t inverted:       1;		
		uint8_t partialized:    1;
		uint8_t pullUp:         1;				
		uint8_t unused_1:       5;
#else
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t unused_1:       5;
		uint8_t pullUp:         1;
		uint8_t partialized:    1;
		uint8_t inverted:       1;
#endif		
	} bit;
} feedbackLLIparams_t;

/** feedback Multiplexed L.L.I. Setting File Params */
typedef union feedbackMpxLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t inverted:       1;
		uint8_t partialized:    1;
		uint8_t pullUp:         1;
		uint8_t unused_1:       2;																								
		uint8_t mpxCfg:         3;
#else		
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t mpxCfg:         3;
		uint8_t unused_1:       2;
		uint8_t pullUp:         1;
		uint8_t partialized:    1;
		uint8_t inverted:       1;
#endif
	} bit;
} feedbackMpxLLIparams_t;

/** Conductivity Sensor L.L.I. Setting File Params */
typedef union conductivitySensorLLIparams_tag
{
	uint16_t wrd;
	struct
	{
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t inverted:       1;
		uint8_t partialized:    1;
		uint8_t pullUp:         1;						
		uint8_t unused_1:       5;
#else		
		/* High Byte */
		uint8_t unused_0:       8;
		/* Low Byte */
		uint8_t unused_1:       5;
		uint8_t pullUp:         1;
		uint8_t partialized:    1;
		uint8_t inverted:       1;
#endif		
	} bit;
} conductivitySensorLLI_t;

/** D.D.C. L.L.I. Setting File Params */
typedef union ddcLLIparams_tag
{
	uint16_t wrd[19];
	struct
	{
		/* 1st Word */
		/* High Byte */
		uint8_t unused_0:   8;
#if defined(__BIT_ORDER_MSB_FIRST__)
		/* Low Byte */
		uint8_t unused_2:   2;
		uint8_t pullUp:     1;				
		uint8_t unused_1:   5;
#else
		/* Low Byte */
		uint8_t unused_1:   5;
		uint8_t pullUp:     1;
		uint8_t unused_2:   2;
#endif		
		/* 2nd word */
		uint16_t n_nom_ticks;
		/* 3rd word */
		uint16_t n_min_ticks;
		/* 4th word */
		uint16_t n_max_ticks;
		/* 5th word */
		uint16_t b0_pole;
		/* 6th word */
		uint16_t a1_pole;
		/* 7th word */
		uint16_t a2_pole;
		/* 8th word */
		uint16_t kt_norm_hw;
		/* 9th word */
		uint16_t kt_norm_lw;
		/* 10th word */
		uint16_t f_max_mc;
		/* 11th word */
		uint16_t f_min_mc;
		/* 12th word */
		uint16_t f_nom_mc;
		/* 13th word */
		uint16_t k_df;
		/* 14th word */
		uint16_t unused_;
		/* 15th word */
		uint16_t cff_mc_cmp;
		/* 16th word */
		uint16_t cff_mc_x_f_nom_mc_hw;
		/* 17th word */
		uint16_t cff_mc_x_f_nom_mc_lw;
		/* 18th word */
		uint16_t f_max_mc_minus_f_nom_mc;
		/* 19th word */
		uint16_t f_nom_mc_minus_f_min_mc;

	} bit;
} ddcLLIparams_t;

/* ****************** */
/* Exported variables */
/* ****************** */
/* <belonging to the module, visible to the Application project,keyword extern used> */

/* ******************* */
/* Function prototypes */
/* ******************* */
/* <belonging to the module, visible to the Application project> */

#endif /* __INPUT_MANAGER_EXP_H */

/* *********** */
/* End of file */
/* *********** */

