/**
\defgroup AlternateControl	Modulo microprogramma AlternateControl

Gestisce l'alternato in combinazione con la pompa lavaggio (sia sincrona che BLDC)

\par \b Opzioni:
-incremento della posizione corrente dell'alternato
-posizionamento assoluto dell'alternato
-posizionamento combinato con attivazione pompa (sia BLDC che sincrona)



\par \b PCLint: 0 Errors, 5 Warnings, 11 Info 28/03/2008 Roberto Fioravanti


\date    04/03/2008
\author  Roberto Fioravanti
\version 1.0.0
\ingroup AppluProgr
*/

/**
Header file del file AlternateControl.c

\file    AlternateControl.h
\ingroup AlternateControl
\date    04/03/2008
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
\addtogroup AlternateControl
@{*/

/* ******** */
/* Includes */
/* ******** */

/* ****************** */
/* Defines and Macros */
/* ****************** */



#ifndef __UP_ALTERNATE_CONTROL_H_
#define __UP_ALTERNATE_CONTROL_H_

/* mode */
#define ALT_CTRL_OPTION           (F1|F0)
#define ALT_CTRL_OPTION_TABLE     F1

#define ALT_CTRL_RELATIVE_POS     0
#define ALT_CTRL_ABSOLUTE_POS     F0
#define ALT_CTRL_TABLE            F1
#define ALT_CTRL_TABLE_PPOS		  (F0|F1)	 

/* Stati del microprogramma AlternateControl */
enum
{ 
  ALT_CTRL_INIT=0,
  ALT_CTRL_BEGIN,
  ALT_CTRL_SELECT_TABLE_EXE,
  ALT_CTRL_WAIT_RAMP_EXE,
  ALT_CTRL_WAIT_ALTPOS_EXE,
  ALT_CTRL_MNT_EXE,
  ALT_CTRL_END,
  ALT_CTRL_ERROR,  
  ALT_CTRL_PAUSE_VS,
};

/* dati microprogramma */
#define ALT_CTRL_MNT_TIME_DISPL             0
#define ALT_CTRL_ALT_POS_DISPL(highStatement)             (((highStatement&ALT_CTRL_OPTION)==ALT_CTRL_TABLE_PPOS)? 3: 0)
#define ALT_CTRL_SPEED_DISPL                1
#define ALT_CTRL_TIME_DISPL                 2

#define ALT_CTRL_MNT_TIME_BIT_DISPL         (((highStatement&ALT_CTRL_OPTION)==ALT_CTRL_TABLE_PPOS)? 0: 2)

#define ALT_CTRL_POS_MSK(highStatement)                    (((highStatement&ALT_CTRL_OPTION)==ALT_CTRL_TABLE_PPOS)?  (b0|b1|b2|b3):(b0|b1))

#define ALT_CTRL_GET_ALT_POS(row,highStatement)           ((OFFS_B((uPAlternateCtrl_setting_data.Data+row/**sizeof(up_Alt_setting_data_s)*/),ALT_CTRL_ALT_POS_DISPL(highStatement))) & ALT_CTRL_POS_MSK(highStatement))


#define ALT_CTRL_GET_MNT_TIME(row)          ((OFFS_B((uPAlternateCtrl_setting_data.Data+row/**sizeof(up_Alt_setting_data_s)*/),ALT_CTRL_MNT_TIME_DISPL))>>ALT_CTRL_MNT_TIME_BIT_DISPL)
#define ALT_CTRL_GET_SPEED(row)             (OFFS_B(uPAlternateCtrl_setting_data.Data+row/**sizeof(up_Alt_setting_data_s)*/, ALT_CTRL_SPEED_DISPL))
#define ALT_CTRL_GET_RAMPTIME(row)          (OFFS_B(uPAlternateCtrl_setting_data.Data+row/**sizeof(up_Alt_setting_data_s)*/, ALT_CTRL_TIME_DISPL))


#define ALT_CTRL_BLDC_SPEED_MUL             25
#define ALT_CTRL_BLDC_RAMP_TIME_MUL         5
#define ALT_CTRL_MNT_TIME_MUL               10

/* comandi per motorino alternato */
#define ALT_CTRL_SYNC_MSK                   b4


#define ALTERNATE_OK						0
#define ALTERNATE_KO						1	


/* DishRackFunctionValue */
#define DISH_RACK_FVALUE_MSK   (b3|b2|b1|b0)


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */
uint8_t ALT_uP_AlternateControl_Exe( uint8_t highStatement, uint8_t lowStatement);
void HIO_SetAltPosition(uint8_t pos);
#endif /* __UP_ALTERNATE_CONTROL_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */

