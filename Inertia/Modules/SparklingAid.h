/**
\defgroup SparklingAid	Modulo microprogramma SparklingAid

Gestisce il rilascio del brillantante tramite il dispencer

\par \b PCLint: 0 Errors, 7 Warnings, 10 Info 17/02/2008 Roberto Fioravanti


\date    17/03/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file SparklingAid.c

\file    PumpControl.h
\ingroup PumpControl
\date    17/03/2008
\author  Roberto Fioravanti
\version 1.0.0
*/

/*
Copyright (c) 2004-2008, Indesit Company, EDT.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version 
\date    
\author  
*/



/**
\addtogroup SparklingAid
@{*/

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */



#ifndef __UP_SPARKLING_AID_H
#define __UP_SPARKLING_AID_H

#define SPRKLING_TABLE_NML_OPT_LEN    2 /* word */
#define SPRKLING_TABLE_TURB_OPT_LEN   3 /* word */


/* up options */
#ifdef _CORRECT_BUG_F1_F0_
	#define SPRKLING_OPTION               (F1|F0)
#else
	#define SPRKLING_OPTION               F1|F0
#endif /* _CORRECT_BUG_F1_F0_ */
#define SPRKLING_NORMAL_OPT           0
#define SPRKLING_WITH_TURB_READ_OPT   F0
#define SPRKLING_WITH_REGULATION_OPT  F1
#define SPRKLING_WITH_REG_TURB_REG_OPT (F1|F0)

/* Displacements */
#define SPRKLING_TIME_D1_DISPL                      0
#define SPRKLING_DELAY_D1_D2_DISPL                  1
#define SPRKLING_TIME_D2_DISPL                      2
#define SPRKLING_TURB_DELTA_DISPL                   3
#define SPRKLING_DELAY_T1_D1_DISPL                  4
#define SPRKLING_DELAY_D2_T2_DISPL                  5

/* Valori */
#define SPRKLING_TIME_D1      (OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_TIME_D1_DISPL))
#define SPRKLING_DELAY_D1_D2  OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_DELAY_D1_D2_DISPL)
#define SPRKLING_TIME_D2      OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_TIME_D2_DISPL)
#define SPRKLING_TURB_DELTA   OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_TURB_DELTA_DISPL)
#define SPRKLING_DELAY_T1_D1  OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_DELAY_T1_D1_DISPL)
#define SPRKLING_DELAY_D2_T2  OFFS_B(uPSparklingAid_setting_data.Data,SPRKLING_DELAY_D2_T2_DISPL)
										
/* Stati di SparklingAid */
enum
{ 
  SPRKLING_INIT=0,
  SPRKLING_T1,
  SPRKLING_WAIT_FOR_D1,
  SPRKLING_D1,
  SPRKLING_WAIT_FOR_D2,
  SPRKLING_D2,
  SPRKLING_WAIT_FOR_T2,
  SPRKLING_T2,
  SPRKLING_END,
  SPRKLING_ERROR,
};

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */

/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */

uint8_t SPA_uP_SparklingAid_Exe( uint8_t HighStatement, uint8_t LowStatement);
 
#endif /* __UP_PUMP_CONTROL_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */

