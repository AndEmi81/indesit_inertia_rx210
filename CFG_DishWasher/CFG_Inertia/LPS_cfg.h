/*
Copyright (c) 2004-2011, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/

/**
\Par	Configuration files for Linear Pressure Sensor module
\n	Provides all configuration parameters to fit module to Application needs
\n
\file		        LPS_cfg.h
\ingroup	      PM_LinearPressureSensor
\date		        23/08/2011
\version	      00.00.01
\author		      Alessandro Sabbatini
\n
\n
\Par\b	History:
\n
\n    00.00.01
\n    23/08/2011
\n    Alessandro Sabbatini
\n    - Creation
\n
*/

/**
\addtogroup PM_LinearPressureSensor
@{*/

#ifndef __LPS_CFG_H
#define __LPS_CFG_H


/** LPS_SUSPEND_CHANNEL_AFTER_DATA_READY - Suspend channel synchronization
when all samples in an accumulator have been collected. If ON it behaves like
previuous module implementation */
#define LPS_SUSPEND_CHANNEL_AFTER_DATA_READY  OFF

/** LPS_BIT_ACCUMULATORS - Number of partial accumulators to be used;  
number of items equals 2^LPS_BIT_ACCUMULATORS:

  0:   1 item      -      1:   2 items
  2:   4 items     -      3:   8 items
  4:  16 items     -      5:  32 items
  6:  64 items     -      7: 128 items
 
Values higher than 7 generate an error.    */
#define LPS_BIT_ACCUMULATORS           2


/** LPS_ACCU_32BIT - Enables use of a 32bit accumulators; 
if set to TRUE, final value will be defined as 32 bit integer variable, 
otherwise it is a 16 bit integer variable. 
When using 16 bit accumulators, be sure that they fits properly - 
overflow checks are performed; if so, you can tune PRECONV_LPS_VALUE(x) 
macro to your needs. */
#define LPS_ACCU_32BIT    ON

/** LPS_DIVIDE_ACCUMULATOR_AFTER_SUM - Define if accumulators are divided only
after sum calculation or not. ON can give more accuracy with high risk of
overflow */
#define LPS_DIVIDE_ACCUMULATOR_AFTER_SUM    ON


/** Conversion used on file parameters to be.
Actually, PWD driver uses ns (nanoseconds) as unit measure, so if setting file
uses a different unit measue it can be automatically converted through this 
macro definition */
#define LPS_SETTING_CONV(x)   ( (x) * 10000UL )

/** Conversion used on each frequency sample read.
Actually, PWD driver uses ns (nanoseconds) as unit measure; if there is need to 
keep LPS values get from PWD in a different unit,they  can be automatically 
converted through this macro definition */
#define PRECONV_LPS_VALUE(x)     ( x )

/** Conversion used to update PSValue variable.
Actually, PWD driver uses ns (nanoseconds) as unit measure; if there is need to 
provide final value (PSValue variable) to application in a different unit 
(e.g. cast to 16 bit variable or keep compatibility with previous setting files)
it can be automatically converted through this macro definition */
#define POSTCONV_LPS_VALUE(x)     ( (x) / 12804UL )

#endif
