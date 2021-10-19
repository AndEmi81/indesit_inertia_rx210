/*
Copyright (c) 2004-2009, Indesit Company, I.D.D..
Tutti i diritti sono riservati. All rights reserved.
*/
/**
\defgroup PM_Multiplexer
@{
\todo Inserire descrizione
\n
\par Configuration:
\todo
\par Exported Functions:
\todo
\n
@}
\ingroup        ProductM
*/

/**
\defgroup	    Multiplexer_n2n Multiplexer n:2^n
@{
\n              Il modulo si occupa di gestire un generico multiplexer con n ingressi di selezione e2^n ingressi
\n
\par Configuration:
\todo
\par Exported Functions:
\todo
@}
\ingroup        PM_Multiplexer
*/
/**
\n
\Par		    Multiplexer Source file
\n			    Contiene le funzioni per la gestione di un generico multiplexer n:2^n
\n
\attention \b   Il modulo è in grado di gestire un mumero di multiplexer \n
pari al valore della costante MAXNUMMUX definita in multiplexer.h e \n
un numero massimo di ingressi di selezione pari a 8
\n
\file		    Multiplexer.c
\ingroup	    Multiplexer_n2n 
\date		    29/06/2009
\version	    00.00.01
\author		    Pierluigi Ianni
\n
\n
\Par\b	History:
\n 
\n              00.00.01
\n              29/06/2009
\n              Pierluigi Ianni
\n          	Prima versione
\n              - Sostituisce le vecchie versioni di gestione separata dei multiplexer 2:4 (multi2) \n
e 3:8 (multi)
\n
*/


/* ******** */
/* Includes */
/* ******** */

#ifndef DIO_DEBUG
#include "debug.h"
#endif /* DIO_DEBUG */

#ifndef DIO_DEBUG
/*PI begin: rimosso solo per permettere la compilazione */
#include "ProductConfig.h"
#include "SkeletonGlobal.h"	/* 05/06/2009 : Aggiunto Pierluigi Ianni */
#include "IO_Manager.h"			/* 05/06/2009 : Aggiunto Pierluigi Ianni */
/*PI end: rimosso solo per permettere la compilazione */
#else
/*PI begin: Aggiunte solo per permettere la compilazione */
#include "StandardTypes.h"
#include "StandardDefines.h"
/*PI end: solo per permettere la compilazione */
#endif /* DIO_DEBUG */

#include "Multiplexer.h"	/* 05/06/2009 : Aggiunto Pierluigi Ianni */
#include "string.h"			/* 05/06/2009 : Aggiunto Pierluigi Ianni */



#if !defined (OLD_MULTI)
/* PIZZOLATO 25/09/09 header sempre incluso. */
/*#include <string.h>*/

#ifndef DIO_DEBUG
/*PI begin: rimosso solo per permettere la compilazione */
#include "PlatformConfig.h"
/*PI end: rimosso solo per permettere la compilazione */
#endif /* DIO_DEBUG */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */
/** struttura variabili utilizzate dal modulo */
/*typedef struct
{ */
    /** vettore contenente i pin del micro che agiscono sui Chip Select 
    del componente */
/*    uint8_t chipSelectPins[NUM_CS_3_8_PINS]; */
    /** contatore utilizzato per tener nota dei pin registrati in fase 
    d'inizializzazione del modulo */
/*    uint8_t numChipSelectPins;    
}multiplexer_3_8_t; */

typedef struct
{ 
    /** vettore contenente i pin del micro che agiscono sui Chip Select 
    del componente */
    uint8_t chipSelectPins[MAXSELINPUT];
    /** contatore utilizzato per tener nota dei pin registrati in fase 
    d'inizializzazione del modulo */
    uint8_t chipSelectPinsCounter;    
    /** vettore contenente il numero degli ingressi dei selezione per ogni mux (tipo di mux) */
    uint8_t muxType;    
}multiplexer_t;


/* ********* */
/* Variables */
/* ********* */

/* Exported */

/* Static */

/* Global */

/** struttura per la gestione del modulo */
multiplexer_t MultiPlexer_Vars[NUMMUXTOT];


/* ******************* */
/* Function Prototypes */
/* ******************* */


/* *************** */
/* Function Bodies */
/* *************** */

/* Exported function */

/**
La funzione viene chiamata in OnColdReset().
Ha il compito di azzerare le variabili utilizzate dal modulo.

\attention \b MultiPlexer_Vars used as W


\author 	Pierluigi Ianni
\date		30/06/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void MulInit ()
{
    uint8_t muxId;
    for (muxId=0; muxId<NUMMUXTOT; muxId++)
    {
      
      MultiPlexer_Vars[muxId].chipSelectPinsCounter = 0;
      MultiPlexer_Vars[muxId].muxType = NOMUX;
      (void)memset(MultiPlexer_Vars[muxId].chipSelectPins,0,MAXSELINPUT*sizeof(MultiPlexer_Vars[muxId].chipSelectPins[0]));
    }
}


/**
La funzione ha il compito di creare un multiplexer

\param  [in] muxId    identificatore del multiplexer
\param  [in] muxType  definisce il tipo di mux ossia il numero di ingressi di selezione

\attention \b MultiPlexer_Vars used as W

\return 	mpx_n_Error_t

\retval		MPX_N_E_OK no problem 	 
\retval		MPX_N_E_KO muxId out of range o multiplexer relativo a muxId già presente

\author 	Pierluigi Ianni
\date		30/06/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/
/*
mpx_n_Error_t SingleMulSet (uint8_t muxId, uint8_t muxType)
{
    
    if ((muxId<MAXNUMMUX)&&( MultiPlexer_Vars[muxId].muxType = NOMUX))
    {
      
        MultiPlexer_Vars[muxId].muxType = muxType;
        return MPX_N_E_OK;
    }
    else
    {
        return MPX_N_E_KO;  
    }
}
*/


/**
La funzione ha il compito di creare i multiplexer del tipo e in numero secondo le define in \n
multiplexer.h

\attention \b I multiplexer sono identificati da valori ordinali e sono ordinati \n
per numero di ingressi di selezione crescente
\attention \b Possono esister più multiplexer aventi lo stesso numero di ingressi di selzione \n
ma un identificativo diverso
lo stesso numero di ingressi di selzione
\attention \b MultiPlexer_Vars used as W

\author 	Pierluigi Ianni
\date		30/06/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

void MulSet ()
{
    uint8_t muxId = 0;
    uint8_t i;

#ifdef MUX_2_4
	for (i=0; i<NUMMUX_2_4; i++)
   	{
   		MultiPlexer_Vars[muxId].muxType = MUX_2_4;
   		muxId++;
   	}
#endif /* NUMMUX_2_4 */  
#ifdef MUX_3_8
	for (i=0; i<NUMMUX_3_8; i++)
   	{
   		MultiPlexer_Vars[muxId].muxType = MUX_3_8;
   		muxId++;
   	}
#endif /* NUMMUX_3_8 */
#ifdef MUX_4_16
	for (i=0; i<NUMMUX_4_16; i++)
	{
   		MultiPlexer_Vars[muxId].muxType = MUX_4_16;
   		muxId++;
   	}
#endif /* NUMMUX_4_16 */  
 
}

    
/**
La funzione viene chiamata in OutputInitialization() se il reset è freddo.
Ha il compito di memorizzare i pin del micro utilizzati per pilotare i chip select\n
del multiplexer.  

\param  [in] chipSelectPin pin del micro utilizzato per gestire i chip select del multiplexer.
\param  [in] muxId         identificatore del multiplexer

\attention \b MultiPlexer_Vars used as R/W

\return 	mpx_n_Error_t

\retval		MPX_N_E_OK no problem 	 
\retval		MPX_N_E_KO too many pins or muxId out of range


\author 	Pierluigi Ianni
\date		30/06/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/

mpx_n_Error_t InsMulChSelPin ( uint8_t chipSelectPin, uint8_t muxId )
{
    if (muxId<NUMMUXTOT)
    {
      
      if ( MultiPlexer_Vars[muxId].chipSelectPinsCounter < MultiPlexer_Vars[muxId].muxType ) 
      {
          MultiPlexer_Vars[muxId].chipSelectPins[MultiPlexer_Vars[muxId].chipSelectPinsCounter] = chipSelectPin;    
          MultiPlexer_Vars[muxId].chipSelectPinsCounter++;
          return MPX_N_E_OK;
      }
      else
      {
          MultiPlexer_Vars[muxId].chipSelectPinsCounter = 0;
          return MPX_N_E_KO;        
      }
    }
    else
    {
      return MPX_N_E_KO;
    }
}

/**
Seleziona la configurazione da applicare al multiplexer.  

\param  [in] chipSelectConf configurazione da applicare al multiplexer.
\param  [in] muxId          identificatore del multiplexer

\attention \b MultiPlexer_Vars used as R

\return 	mpx_n_Error_t

\retval		MPX_N_E_OK no problem 	 
\retval		MPX_N_E_KO muxId out of range

\author 	Pierluigi Ianni
\date		30/06/2009
\version	1.0.0

   
\note		\par \b Nota 1:
			documentation by Pierluigi Ianni

*/


/*fp NOTA: no_loop unroll pragma presente solo per gb60, definire se definirla o no, e come con l'utilizzo del driver*/
/*#if defined(BUILD_FREESCALE_GB60) || defined(BUILD_FREESCALE_GT32_44P)
#pragma NO_LOOP_UNROLL
#endif*/
mpx_n_Error_t MulSelect ( uint8_t chipSelectConfig, uint8_t muxId )
{
uint8_t pinCount;
uint8_t pinState;
    
    if (muxId<NUMMUXTOT)
    {
      
      for ( pinCount = 0; pinCount < MultiPlexer_Vars[muxId].muxType; pinCount++ )
      {
          pinState = ((chipSelectConfig & (1<<pinCount))?ON:OFF);
          /*lint -e718 PIZZOLATO 25/09/09 SET_LOAD macro non definita nel progetto HAL*/
          /*lint -e746 */
          SET_LOAD(pinState, MultiPlexer_Vars[muxId].chipSelectPins[pinCount]);
          /*lint +e746 */
          /*lint +e718 */
          
          
          
      }
      return MPX_N_E_OK;
    }
    else
    {
      return MPX_N_E_KO;
    }
}



/* Local Function */


/* *********** */
/* End of file */
/* *********** */

#else /* OLD_MULTI is defined */
/*
     SPES/EDT

     File:            Multi.c
     Header File:     Multi.h
     
     Author:          Franco Boldreghini
     
     Microcontroller: Motorola HCS08 GB60
     Version:         1.0.0
     Date:            13/05/2004
     
     Compiler:        Metrowerks HCS08 C Compiler
     
     
     Description:


*/


#include "SkeletonGlobal.h"
#include "IO_Manager.h"		/* 07/12/2004 : aggiunto */

/* AC: 1/12/2004 Begin */
#include "Multiplexer.h"
/* AC: 1/12/2004 Begin */

/* static unsigned char MulChSelPin[3]; */
static unsigned char MulChSelPin[MUX_3_8]; /* SB AC 6/12/2004 */

static unsigned char NumPin;

void MulInit ( void)
  {
    unsigned char i;
    
    NumPin=0;
    /*for (i=0; i<3; i++)*/
    for (i=0; i<MUX_3_8; i++) /* SB AC 6/12/2004 */
      MulChSelPin[i]=0;
  }


void InsMulChSelPin ( unsigned char MulPin)
  {    
     // i pin vanno inseriti di seguito a partire dal LSB     
     MulChSelPin[NumPin++] = MulPin;
     /* if (NumPin >= 3) */
     if (NumPin >= MUX_3_8) /* SB AC 6/12/2004 */
         NumPin=0;
  }

/*
void InsMulOutPin (unsigned char MulId, unsigned char MulPin)
  {    
     // vanno inserite le uscite relative al MUL o più uscite
     // se più MUL sono pilotati insieme
     
     MulOutPin[MulId][MulNumOut[MulId]++] = MulPin;
     if (MulNumOut[MulId] >= 3)
       MulNumOut[MulId]=0;
       
  }
*/

void MulSelect ( unsigned char MulChSelVal)
  {
/*   
	 SET_LOAD((MulChSelVal & 0x01), MulChSelPin[0]);
     SET_LOAD((MulChSelVal & 0x02), MulChSelPin[1]);
     SET_LOAD((MulChSelVal & 0x04), MulChSelPin[2]);     
*/

	 SET_LOAD((MulChSelVal & BIT0_MASK), MulChSelPin[0]); /* SB AC 6/12/2004 */
     SET_LOAD((MulChSelVal & BIT1_MASK), MulChSelPin[1]);
     SET_LOAD((MulChSelVal & BIT2_MASK), MulChSelPin[2]);     

  }

#endif /* OLD_MULTI */



/* Vecchio file Multi2.c */
/*
static unsigned char MulChSelPin2[MAXNUMMUL_2];

static unsigned char NumPin2;

void MulInit2 ( void)
{
    unsigned char i;
    
    NumPin2=0;
 
 	memset(MulChSelPin2,0,sizeof(MulChSelPin2[MAXNUMMUL_2]));     
      
}


void InsMulChSelPin2 ( unsigned char MulPin)
{    
     // i pin vanno inseriti di seguito a partire dal LSB     
    
     MulChSelPin2[NumPin2++] = MulPin;
    
     if (NumPin2 >= MAXNUMMUL_2) 
         NumPin2=0;
}


void MulSelect2 ( unsigned char MulChSelVal)
{

	 SET_LOAD((MulChSelVal & BIT0_MASK), MulChSelPin2[0]); 
     SET_LOAD((MulChSelVal & BIT1_MASK), MulChSelPin2[1]);

}
*/
