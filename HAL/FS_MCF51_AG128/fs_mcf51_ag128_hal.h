/**
\defgroup HAL_FS_MCF51_AG128 HAL support classes for Freescale MCF51 AG128

The module indicates the support classes for the Freescale MCF51 AG128 microcontroller
- OCI2C On Chip I2C #1
- OCDMA On Chip DMA
- OCFLS On Chip flash
- OCWDT On Chip watchdog
- OCDIO On Chip digital input/output ports
- OCCLK On Chip clock synthesis
- Reset source support
- OCUART On Chip uart
- OCADC On Chip ADC


\todo
- I2C supportare lo switch sui pin
- I2C supportare General Call Address
- remove __RESET_WATCHDOG from flash routines executed in R.A.M.

\date    22/2/2010
\author  Jaroslav Musil
\version 1.0.13
\ingroup HAL
*/


/**
\defgroup HAL_FS_MCF51_AG128_CLS  Freescale CFV1 AG128 supported drivers Classes

This module lists the driver classes supported by this HAL.


\date    07/05/2009
\author  Roberto Fioravanti
\version 1.0.0
\ingroup HAL_FS_MCF51_AG128
*/

/**
Header file of the FS_MCF51_AG128_hal.c file

\file    FS_MCF51_AG128_hal.h
\ingroup HAL
\date    22/02/2011
\author  Jaroslav Musil
\version 00.00.14

\par		History
\n 		00.00.29
\n		Apr 4,2011
\n		Stefano Ughi
\n		changes
\n 		- added inline bool_t TMR_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pADch)
\n
\n 		00.00.28
\n		Feb 14,2011
\n		Stefano Ughi
\n		changes
\n 		- FLS_ErasePageCmdIsInRam in assembly
\n		- FLS_BurstProgCmdIsInRam in assembly
\n
\n 		00.00.27
\n		27/01/2010
\n		Roberto Fioravanti
\n		changes
\n 		- added I2C_Level3BlockMonitorSync()	I2C_Level4BlockMonitorSync() I2C_Level5BlockMonitorSync()
\n		- added I2C_NestedIsr_Begin() I2C_NestedIsr_End() as empty because nested isr not supported
\n 		00.00.14
\n		22/02/2010
\n		Jaroslav Musil
\n      Changed the ADC_StopConversion macro
\n
\n 		00.00.13
\n		18/02/2010
\n		Jaroslav Musil
\n      ADC modul uses the ADC_USE_SW_DMA and ADC_USE_HW_DMA defines
\n      ADC modul: added DisableDMA macro into the start conversion routines and changed comments
\n
\n 		00.00.12
\n		18/02/2010
\n		Jaroslav Musil
\n      Added ADC modul
\n
\n 		00.00.11
\n		16/02/2010
\n		Jaroslav Musil
\n      The CLK function corps moved to hal.
\n
\n 		00.00.10
\n		16/02/2010
\n		Jaroslav Musil
\n      The WDT driver inline functions moved to hal.
\n
\n 		00.00.09
\n		15/02/2010
\n		Jaroslav Musil
\n      The Init function of WDT was created instead of having it in the driver.
\n
\n 		00.00.08
\n		10/02/2010
\n		Jaroslav Musil
\n      The CLK_EngageClkRefSync function was created instead of having in the driver.
\n
\n 		00.00.07
\n		5/02/2010
\n		Jaroslav Musil
\n      Added UART support
\n
\n 		00.00.06
\n		5/02/2010
\n		Jaroslav Musil
\n      Added reset source support
\n
\n 		00.00.05
\n		5/02/2010
\n		Jaroslav Musil
\n      Added CLK support
\n
\n 		00.00.04
\n		3/02/2010
\n		Jaroslav Musil
\n      Added DIO support
\n
\n 		00.00.03
\n		26/01/2010
\n		Jaroslav Musil
\n      Added watchdog support
\n		Function prototypes changed to use the same interface as in the S. Ughi's driver on S08
\n
\n      00.00.02
\n      11/1/2010
\n      Jaroslav Musil
\n      Added flash support
\n
\n      00.00.01
\n      1/12/2009
\n      Jaroslav Musil
\n      Creation
*/

/*
Copyright (c) 2009-2010, Indesit Company, IDD.
Tutti i diritti sono riservati. All rights reserved.
*/

/* Old Versions
\version
\date
\author
*/

/**
\addtogroup HAL_FS_MCF51_AG128_CLS
@{*/



#ifndef __HAL_FREESCALE_MCF51_AG128_H
#define __HAL_FREESCALE_MCF51_AG128_H





/* ******** */
/* Includes */
/* ******** */
#include "Config.h"                               /* For: Application configuration */
#include "fs_mcf51_ag128_reg.h"

/* ****** */
/* Errors */
/* ****** */

void HAL_ErrorHandler( int err );

/**  header must not be included direcly by application level */
#if !defined(__DRIVER_H)
#error "private header file.  -- do not include directly."
#endif

/* ****************** */
/* Defines and Macros */
/* ****************** */

/* ********************************************************************************************** */
/*                                  BEGIN System Class HAL Support                                */
/* ********************************************************************************************** */

/** @name H.A.L. System 
 *
 */
/*@{*/


/* ****************** */
/* Defines and Macros */
/* ****************** */

/** Define System Capability */
#define _HAL_SYS_CAPABILITY

/* Map mcu specific registers names to a 'standard' set of names

              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>

    (this helps when generating new files for additional parts of the same family).
*/

#define LPO_FREQUENCY_Hz    1000

extern word SR_Ipl;
extern byte NestedLevel;
extern byte IRQSC_copy;

/*
- save previous Interrupt priority level in SR_Ipl and set I.p.l. of SR to 7
- disable IRQ Interrupt
*/
inline __declspec(register_abi) asm void DisableInt(void)
{
	move.w SR,D0;       /* [SR]->D0 */
	move.l D0,D1;       /* [SR]->D1 */
	ori.l #0x0700,D1;
	move.w D1,SR;       /* applies 7 to I.p.l. */
	andi.l #0x0700,D0;
	move.w D0,SR_Ipl;   /* Save previous I.p.l. */

	move.b   IRQSC,D0
	move.b   D0,IRQSC_copy
	mov3q    #1,D0
	bclr     D0,IRQSC

}

#define EnterCritical()\
{\
if (0 == NestedLevel) { DisableInt(); }\
NestedLevel++;\
}

/*
- Restore IRQ with previous value saved in IRQSC_copy
- Restore SR with previous I.p.l. saved in SR_Ipl
*/
inline __declspec(register_abi) asm void EnableInt(void)
{
	mov3q    #1,D0
	btst     D0,IRQSC_copy
	beq.s    NoIRQIEset
	bset     D0,IRQSC
	bra      RestoreSR
NoIRQIEset:
	bclr     D0,IRQSC;
RestoreSR:
	move.w  SR,D1;         /* [SR] -> D1 */
	andi.l #0xF8FF,D1;     /* Clear I.l.p  */
	move.w SR_Ipl,D0;      /* [SR_Ipl] -> D0 */
	or.l D1,D0;
	move.w D0,SR;
}

#define ExitCritical()\
{\
NestedLevel--;\
if (0 == NestedLevel) { EnableInt(); }\
}



#if IRQ_ON_EDGE_AND_LEVEL == 1
/*
- disable IRQ Interrupt
*/
inline __declspec(register_abi) asm void DisableIRQInt(void)
{
	move.b   IRQSC,D0
	move.b   D0,IRQSC_copy
	mov3q    #1,D0
	bclr     D0,IRQSC
}

/*
- Restore IRQ with previous value saved in IRQSC_copy
*/
inline __declspec(register_abi) asm void RestoreIRQInt(void)
{
	mov3q    #1,D0
	btst     D0,IRQSC_copy
	beq.s    IRQIE_notset
	bset     D0,IRQSC
	bra      QuitEnableIRQInt
IRQIE_notset:
	bclr     D0,IRQSC;
QuitEnableIRQInt:
}

#else /* IRQ_ON_EDGE_AND_LEVEL == 1 */

#define DisableIRQInt()
#define RestoreIRQInt()

#endif /* IRQ_ON_EDGE_AND_LEVEL == 1 */



/* Returns the Status Register */
inline __declspec(register_abi) asm word Get_SR_value(void)
{
	move.w  SR,D0;
}


/** Reset status register */
#define _HAL_MCF51_SRS_REG              (SRS)

#define _HAL_MCF51_SRS_LVD_MASK         (SRS_LVD_MASK)
#define _HAL_MCF51_SRS_WDOG_MASK        (SRS_WDOG_MASK)
#define _HAL_MCF51_SRS_ILAD_MASK        (SRS_ILAD_MASK)
#define _HAL_MCF51_SRS_ILOP_MASK        (SRS_ILOP_MASK)
#define _HAL_MCF51_SRS_LOC_MASK         (SRS_LOC_MASK)
#define _HAL_MCF51_SRS_PIN_MASK         (SRS_PIN_MASK)
#define _HAL_MCF51_SRS_POR_MASK         (SRS_POR_MASK)

#define _RST_VALUE _HAL_MCF51_SRS_REG

#define SYS_GetResetSrcVal()            (_RST_VALUE)

#define SYS_LowVoltageReset(reg)        ((reg == _HAL_MCF51_SRS_LVD_MASK))
#define SYS_WatchdogReset(reg)          ((reg == _HAL_MCF51_SRS_WDOG_MASK))
#define SYS_IllegalAddressReset(reg)    ((reg == _HAL_MCF51_SRS_ILAD_MASK))
#define SYS_IllegalOpcodeReset(reg)     ((reg == _HAL_MCF51_SRS_ILOP_MASK))
#define SYS_LossOfClockReset(reg)       ((reg == _HAL_MCF51_SRS_LOC_MASK))
#define SYS_ExternalPinReset(reg)       ((reg == _HAL_MCF51_SRS_PIN_MASK))
#define SYS_DebuggerReset(reg)          (reg == 0)
/* This define MUST be set according to UserReset() */
#define SYS_UserReset(reg)              SYS_WatchdogReset(reg)

#define SYS_ClearResetSource(reg)       {reg = 0;}
#define SYS_SetDebuggerReset(reg)       {reg = 0;}
#define SYS_SetLowVoltageReset(reg)     {reg = _HAL_MCF51_SRS_LVD_MASK;}
#define SYS_SetWatchdogReset(reg)       {reg = _HAL_MCF51_SRS_WDOG_MASK;}
#define SYS_SetIllegalAddressReset(reg) {reg = _HAL_MCF51_SRS_ILAD_MASK;}
#define SYS_SetIllegalOpcodeReset(reg)  {reg = _HAL_MCF51_SRS_ILOP_MASK;}
#define SYS_SetLossOfClockReset(reg)    {reg = _HAL_MCF51_SRS_LOC_MASK;}
#define SYS_SetExternalPinReset(reg)    {reg = _HAL_MCF51_SRS_PIN_MASK;}
#define SYS_SetPowerOnReset(reg)        {reg = (_HAL_MCF51_SRS_POR_MASK|_HAL_MCF51_SRS_LVD_MASK);}
/* This define MUST be set according to UserReset() */
#define SYS_SetUserReset(reg)           SYS_SetWatchdogReset(reg)
/* After POR this M.C.U. requires a reset because of LVD */
#define MCU_RequiresResetAfterPOR()     TRUE

inline bool_t SYS_PowerOnReset(resetSource_t rstSrc)
{
	if ( rstSrc == (_HAL_MCF51_SRS_POR_MASK|_HAL_MCF51_SRS_LVD_MASK) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*lint -emacro(506,LVD_AllowUpDate) */
/*lint -emacro(774,LVD_AllowUpDate) */
#define LVD_AllowUpDate()   FALSE
/* LVD * 10 */
#define SYS_VLVDL0  25.6
#define SYS_VLVDL1  40.0

/* LVW * 10 */
#define SYS_VLVWL0  27.4
#define SYS_VLVWH0  29.2
#define SYS_VLVWL1  43.0
#define SYS_VLVWH1  46.0

/*
Low-Voltage Warning Voltage Select
*/
#define LVWLowTripPoint()   {SPMSC2_LVWV = 0;}
#define LVWHighTripPoint()  {SPMSC2_LVWV = 1;}
/*
LVD warning interrupt Enable/Disable
*/
#define LVDWarningAcknoledge()  {SPMSC1_LVWACK = 1;}

#define LVDWarningIntOn()\
{\
LVDWarningAcknoledge()\
SPMSC1_LVWIE = 1;\
}
#define LVDWarningIntOff()  {SPMSC1_LVWIE=0;}

#define LVDGetWarningFlag()     (SPMSC1 & SPMSC1_LVWF_MASK)
/*
LVD in stop mode Enable/Disable
*/
#define LVDSetStopOn()  {SPMSC1_LVDSE = 1;}
#define LVDSetStopOff() {SPMSC1_LVDSE = 0;}
/*
LVD reset disable
*/
#define LVDR_Disable()  { SPMSC1 &= (~(SPMSC1_LVDE_MASK|SPMSC1_LVDRE_MASK));}

/*
Set voltage trip to generate LVD reset
N.B.: LVDV,LVDE,LVDRE can be written only one time after reset
*/
inline void LVDR_SetUp(uint8_t tripPointVR)
{
    int dist_0, dist_1;
    uint8_t SPMSC2_Mirror;

	dist_0 = abs((int16_t)tripPointVR - (int16_t)SYS_VLVDL0);

	dist_1 = abs((int16_t)tripPointVR - (int16_t)SYS_VLVDL1);

	SPMSC2_Mirror = SPMSC2;
	if (dist_0 < dist_1)
	{
		/* Low trip point selected (n.v. 2.56 V) */
		SPMSC2_Mirror &=(~(SPMSC2_LVDV_MASK));
	}
	else
	{
		/* High trip point selected (n.v. 4.00 V) */
		SPMSC2_Mirror |=((SPMSC2_LVDV_MASK));
	}
	SPMSC2 = SPMSC2_Mirror;
	SPMSC1 = (SPMSC1_LVDE_MASK|SPMSC1_LVDRE_MASK);
}

/* Set voltage trip to generate LVD warning */
inline void LVDW_SetTrip(uint8_t tripPointVW)
{
    int dist_0, dist_1;

	if ((SPMSC2 & SPMSC2_LVDV_MASK) == 0)
	{
		/* Low Trip Point Selected */
		dist_0 = abs((int16_t)tripPointVW-(int16_t)SYS_VLVWL0);
		dist_1 = abs((int16_t)tripPointVW-(int16_t)SYS_VLVWH0);
	}
	else
	{
		/* High Trip Point Selected */
		dist_0 = abs((int16_t)tripPointVW-(int16_t)SYS_VLVWL1);
		dist_1 = abs((int16_t)tripPointVW-(int16_t)SYS_VLVWH1);
	}


	if (dist_0 < dist_1)
	{
		LVWLowTripPoint();
	}
	else
	{
		LVWHighTripPoint();
	}
}

/* Reads the LVD trip */
inline uint8_t LVDR_GetTrip(void)
{
	if ((SPMSC2 & SPMSC2_LVDV_MASK) == 0)
	{
		return (uint8_t)SYS_VLVDL0;
	}
	else
	{
		return (uint8_t)SYS_VLVDL1;
	}
}

/* Reads the LVD warning trip */
inline uint8_t LVDW_GetTrip(void)
{
	if ((SPMSC2 & SPMSC2_LVDV_MASK) == 0)
	{
		if ((SPMSC2 & SPMSC2_LVWV_MASK) == 0)
		{
			return (uint8_t)SYS_VLVWL0;
		}
		else
		{
			return (uint8_t)SYS_VLVWH0;
		}
	}
	else
	{
		if ((SPMSC2 & SPMSC2_LVWV_MASK) == 0)
		{
			return (uint8_t)SYS_VLVWL1;
		}
		else
		{
			return (uint8_t)SYS_VLVWH1;
		}
	}
}
/*@}*/

/* ********************************************************************************************** */
/*                                  END System Class HAL Support                                  */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN CLK Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Oscillator
 *
 */
/*@{*/


/* ****************** */
/* Defines and Macros */
/* ****************** */

/*<belonging to the module, visible to the whole Platform project>*/
/* Max allow device frequency */
#define CLK_MAX_DEVICE_FREQ         50330000

/* Range of FLL refference frequency */
#define CLK_MIN_REF_RANGE           31250
#define CLK_MAX_REF_RANGE           39062

/* Frequency of the max. FLL factor */
#define CLK_DCO_MAX_FREQ            32768

/* Factors of FLL */
#define CLK_FLL_FACTOR_0            512
#define CLK_FLL_FACTOR_0_DMX32      608
#define CLK_FLL_FACTOR_1            1024
#define CLK_FLL_FACTOR_1_DMX32      1216
#define CLK_FLL_FACTOR_2            1536
#define CLK_FLL_FACTOR_2_DMX_32     1824



/** Define Clock Capability */
#define _HAL_CLK_CAPABILITY

#define _HAL_MCF51_CLK_ICSC1_REG                    (ICSC1)
#define _HAL_MCF51_CLK_ICSC2_REG                    (ICSC2)
#define _HAL_MCF51_CLK_ICSTRM_REG                   (ICSTRM)
#define _HAL_MCF51_CLK_ICSSC_REG                    (ICSSC)

#define _HAL_MCF51_CLK_ICSC1_CLKS_MASK              (ICSC1_CLKS_MASK)
#define _HAL_MCF51_CLK_ICSC1_CLKS_SHIFT             (ICSC1_CLKS_BITNUM)
#define _HAL_MCF51_CLK_ICSC1_RDIV_MASK              (ICSC1_RDIV_MASK)
#define _HAL_MCF51_CLK_ICSC1_RDIV_SHIFT             (ICSC1_RDIV_BITNUM)
#define _HAL_MCF51_CLK_ICSC1_IREFS_MASK             (ICSC1_IREFS_MASK)
#define _HAL_MCF51_CLK_ICSC1_IRCLKEN_MASK           (ICSC1_IRCLKEN_MASK)
#define _HAL_MCF51_CLK_ICSC1_IREFSTEN_MASK          (ICSC1_IREFSTEN_MASK)

#define _HAL_MCF51_CLK_ICSC2_BDIV_MASK              (ICSC2_BDIV_MASK)
#define _HAL_MCF51_CLK_ICSC2_BDIV_SHIFT             (ICSC2_BDIV_BITNUM)
#define _HAL_MCF51_CLK_ICSC2_RANGE_MASK             (ICSC2_RANGE_MASK)
#define _HAL_MCF51_CLK_ICSC2_HGO_MASK               (ICSC2_HGO_MASK)
#define _HAL_MCF51_CLK_ICSC2_LP_MASK                (ICSC2_LP_MASK)
#define _HAL_MCF51_CLK_ICSC2_EREFS_MASK             (ICSC2_EREFS_MASK)
#define _HAL_MCF51_CLK_ICSC2_ERCLKEN_MASK           (ICSC2_ERCLKEN_MASK)
#define _HAL_MCF51_CLK_ICSC2_EREFSTEN_MASK          (ICSC2_EREFSTEN_MASK)

#define _HAL_MCF51_CLK_ICSSC_DRST_MASK              (ICSSC_DRST_DRS_MASK)
#define _HAL_MCF51_CLK_ICSSC_DRST_SHIFT             (ICSSC_DRST_DRS_BITNUM)
#define _HAL_MCF51_CLK_ICSSC_DRS_MASK               (ICSSC_DRST_DRS_MASK)
#define _HAL_MCF51_CLK_ICSSC_DRS_SHIFT              (ICSSC_DRST_DRS_BITNUM)
#define _HAL_MCF51_CLK_ICSSC_DMX32_MASK             (ICSSC_DMX32_MASK)
#define _HAL_MCF51_CLK_ICSSC_IREFST_MASK            (ICSSC_IREFST_MASK)
#define _HAL_MCF51_CLK_ICSSC_CLKST_MASK             (ICSSC_CLKST_MASK)
#define _HAL_MCF51_CLK_ICSSC_CLKST_SHIFT            (ICSSC_CLKST_BITNUM)
#define _HAL_MCF51_CLK_ICSSC_OSCINIT_MASK           (ICSSC_OSCINIT_MASK)
#define _HAL_MCF51_CLK_ICSSC_FTRIM_MASK             (ICSSC_FTRIM_MASK)

#define CLK_SetFLLClockSource()                     {_HAL_MCF51_CLK_ICSC1_REG = (uint8_t)((_HAL_MCF51_CLK_ICSC1_REG & ~_HAL_MCF51_CLK_ICSC1_CLKS_MASK) | (((uint8_t)0 << (uint8_t)_HAL_MCF51_CLK_ICSC1_CLKS_SHIFT) & _HAL_MCF51_CLK_ICSC1_CLKS_MASK)); }
#define CLK_SetInternalClockSource()                {_HAL_MCF51_CLK_ICSC1_REG = (uint8_t)((_HAL_MCF51_CLK_ICSC1_REG & ~_HAL_MCF51_CLK_ICSC1_CLKS_MASK) | (((uint8_t)1 << (uint8_t)_HAL_MCF51_CLK_ICSC1_CLKS_SHIFT) & _HAL_MCF51_CLK_ICSC1_CLKS_MASK)); }
#define CLK_SetExternalClockSource()                {_HAL_MCF51_CLK_ICSC1_REG = (uint8_t)((_HAL_MCF51_CLK_ICSC1_REG & ~_HAL_MCF51_CLK_ICSC1_CLKS_MASK) | (((uint8_t)2 << (uint8_t)_HAL_MCF51_CLK_ICSC1_CLKS_SHIFT) & _HAL_MCF51_CLK_ICSC1_CLKS_MASK)); }

#define CLK_SetRefDiv(dividor)                      {_HAL_MCF51_CLK_ICSC1_REG = (uint8_t)((_HAL_MCF51_CLK_ICSC1_REG & ~_HAL_MCF51_CLK_ICSC1_RDIV_MASK) | (((uint8_t)dividor << (uint8_t)_HAL_MCF51_CLK_ICSC1_RDIV_SHIFT) & _HAL_MCF51_CLK_ICSC1_RDIV_MASK)); }

#define CLK_SetInternalFLLRefClock()                {_HAL_MCF51_CLK_ICSC1_REG |= _HAL_MCF51_CLK_ICSC1_IREFS_MASK; }
#define CLK_SetExternalFLLRefClock()                {_HAL_MCF51_CLK_ICSC1_REG &= ~_HAL_MCF51_CLK_ICSC1_IREFS_MASK; }

#define CLK_EnableInternalRefClock()                {_HAL_MCF51_CLK_ICSC1_REG |= _HAL_MCF51_CLK_ICSC2_ERCLKEN_MASK; }
#define CLK_DisableInternalRefClock()               {_HAL_MCF51_CLK_ICSC1_REG &= ~_HAL_MCF51_CLK_ICSC2_ERCLKEN_MASK; }

#define CLK_EnableInternalRefClockInStopMode()      {_HAL_MCF51_CLK_ICSC1_REG |= _HAL_MCF51_CLK_ICSC1_IREFSTEN_MASK; }
#define CLK_DisableInternalRefClockInStopMode()     {_HAL_MCF51_CLK_ICSC1_REG &= ~_HAL_MCF51_CLK_ICSC1_IREFSTEN_MASK; }

#define CLK_SetBusFreqDiv(dividor)                  {_HAL_MCF51_CLK_ICSC2_REG = (uint8_t)((_HAL_MCF51_CLK_ICSC2_REG & ~_HAL_MCF51_CLK_ICSC2_BDIV_MASK) | (((uint8_t)dividor << (uint8_t)_HAL_MCF51_CLK_ICSC2_BDIV_SHIFT) & _HAL_MCF51_CLK_ICSC2_BDIV_MASK)); }

#define CLK_SetHighFreqRange()                      {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_RANGE_MASK; }
#define CLK_SetLowFreqRange()                       {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_RANGE_MASK; }

#define CLK_SetHighGainOscillator()                 {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_HGO_MASK; }
#define CLK_SetLowPowerOscillator()                 {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_HGO_MASK; }

#define CLK_DisableFLLInBypassMode()                {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_LP_MASK; }
#define CLK_EnableFLLInBypassMode()                 {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_LP_MASK; }

#define CLK_SetExternalRefOscillator()              {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_EREFS_MASK; }
#define CLK_SetExternalRefClockSource()             {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_EREFS_MASK; }

#define CLK_EnableExternalRefClock()                {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_ERCLKEN_MASK; }
#define CLK_DisableExternalRefClock()               {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_ERCLKEN_MASK; }

#define CLK_EnableExternalRefClockInStopMode()      {_HAL_MCF51_CLK_ICSC2_REG |= _HAL_MCF51_CLK_ICSC2_EREFSTEN_MASK; }
#define CLK_DisableExternalRefClockInStopMode()     {_HAL_MCF51_CLK_ICSC2_REG &= ~_HAL_MCF51_CLK_ICSC2_EREFSTEN_MASK; }

#define CLK_SetTrim(trim)                           {_HAL_MCF51_CLK_ICSTRM_REG = (uint8_t)trim; }
#define CLK_GetTrim()                               (_HAL_MCF51_CLK_ICSTRM_REG)

#define CLK_GetDCORangeStatus()                     ((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_DRST_MASK) >> _HAL_MCF51_CLK_ICSSC_DRST_SHIFT)
#define CLK_SetDCORange(dco)                        {_HAL_MCF51_CLK_ICSSC_REG = (uint8_t)((_HAL_MCF51_CLK_ICSSC_REG & ~_HAL_MCF51_CLK_ICSSC_DRS_MASK) | (((uint8_t)dco << (uint8_t)_HAL_MCF51_CLK_ICSSC_DRS_SHIFT) & _HAL_MCF51_CLK_ICSSC_DRS_MASK)); }

#define CLK_IsDCOMaxFreq()                          ((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_DMX32_MASK) != 0)
#define CLK_SetDCOMaxFreq()                         {_HAL_MCF51_CLK_ICSSC_REG |= _HAL_MCF51_CLK_ICSSC_DMX32_MASK; }
#define CLK_SetDCODefaultFreq()                     {_HAL_MCF51_CLK_ICSSC_REG &= ~_HAL_MCF51_CLK_ICSSC_DMX32_MASK; }

#define CLK_IsInternalFLLRefClock()                 ((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_IREFST_MASK) != 0)
#define CLK_IsExternalFLLRefClock()                 ((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_IREFST_MASK) == 0)

#define CLK_IsFLLClockSource()                      (((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_CLKST_MASK) >> _HAL_MCF51_CLK_ICSSC_CLKST_SHIFT) == (uint8_t)0)
#define CLK_IsInternalClockSource()                 (((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_CLKST_MASK) >> _HAL_MCF51_CLK_ICSSC_CLKST_SHIFT) == (uint8_t)1)
#define CLK_IsExternalClockSource()                 (((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_CLKST_MASK) >> _HAL_MCF51_CLK_ICSSC_CLKST_SHIFT) == (uint8_t)2)

#define CLK_IsOscillatorReady()                     ((_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_OSCINIT_MASK) != 0)

#define CLK_SetFTrim(ftrim)                         {_HAL_MCF51_CLK_ICSSC_REG = (uint8_t)((_HAL_MCF51_CLK_ICSSC_REG & ~_HAL_MCF51_CLK_ICSSC_FTRIM_MASK) | ((uint8_t)ftrim & _HAL_MCF51_CLK_ICSSC_FTRIM_MASK)); }
#define CLK_GetFTrim()                              (_HAL_MCF51_CLK_ICSSC_REG & _HAL_MCF51_CLK_ICSSC_FTRIM_MASK)

#if defined (BDM_ACTIVE)
#if !defined(USER_TRIM) || !defined(USER_FTRIM)
#error "Please define USER_TRIM and USER_FTRIM"
#else
#define SET_TRIMMING()\
CLK_SetTrim(USER_TRIM);\
CLK_SetFTrim(USER_FTRIM);
#endif
#else
#define SET_TRIMMING()
#endif

inline void CLK_EngageClkRefSync(void)
{
#if defined (CLK_HIGH_GAIN_OSCILLATOR)
	/* High gain for the oscilator */
	CLK_SetHighGainOscillator();
#else
	/* Low power for the oscilator */
	CLK_SetLowPowerOscillator();
#endif

#if defined (CLK_EXT_CLOCK_XTAL)
	/* Crystal oscillator selected */
	CLK_SetExternalRefOscillator();
#else
	/* External clock source selected */
	CLK_SetExternalRefClockSource();
#endif

#if defined (CLK_EXT_CLOCK_IN_STOP_MODE)
	/* Enable external clock in stop mode */
	CLK_EnableExternalRefClockInStopMode();
#else
	/* Disable external clock in stop mode */
	CLK_DisableExternalRefClockInStopMode();
#endif

#if defined (CLK_EXT_REF_CLOCK_ACTIVE)
	/* Enable external clock source for the processor */
	CLK_EnableExternalRefClock();
#else
	/* Disable external clock source for the processor */
	CLK_DisableExternalRefClock();
#endif

#if defined(CLK_INT_REF_CLOCK_ACTIVE)
	/* Enable internal clock source for the processor */
	CLK_EnableInternalRefClock();
#else
	/* Disable internal clock source for the processor */
	CLK_DisableInternalRefClock();
#endif

#if defined (CLK_FLL_IN_BYPASS_MODE)
	/* Turns on FLL in bypass mode */
	CLK_EnableFLLInBypassMode();
#else
	/* Turns off FLL in bypass mode */
	CLK_DisableFLLInBypassMode();
#endif
}

/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the internal
reference clock in the FLL bypass mode.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline bool_t CLK_SetInternalClockSync(uint32_t desiredClock)
{
	int32_t frequency, frequency0;
	int32_t difference, difference0;
	uint8_t i;
#if defined (CLK_INT_REF_CLOCK_ACTIVE)

	i = 0;

	frequency0 = (int32_t)CLK_INT_CLOCK_FREQ;

	do
	{
		frequency = frequency0 >> 1;
		i++;
	}
	while ((i < 3) && !(frequency < desiredClock));

	/* Difference between the desired and possible frequency */
	difference = desiredClock - frequency;

	/* Makes absolute value */
	if (difference < 0)
	{
		difference = -difference;
	}

	/* Difference between the desired and possible frequency from the previous step */
	difference0 = desiredClock - frequency0;

	/* Makes absolute value */
	if (difference0 < 0)
	{
		difference0 = -difference0;
	}

	/* If the previous step frequncy was closer to the disered, changes the i */
	if (difference0 < difference)
	{
		i--;
	}

	/* Sets the internal reference clock for FLL */
	CLK_SetInternalFLLRefClock();

    /* Waits until the internal clock is selected */
    while (!CLK_IsInternalFLLRefClock()) {
    }

	/* Selects the internal clock source */
	CLK_SetInternalClockSource();

	/* Sets the bus frequency dividor */
	CLK_SetBusFreqDiv(i);

	return (bool_t)1;
#else
	return (bool_t)0;
#endif
}


/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on the external
reference clock in the FLL bypass mode.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline bool_t CLK_SetExternalClockSync(uint32_t desiredClock)
{
#if defined (CLK_EXT_REF_CLOCK_ACTIVE)
	int32_t frequency, frequency0;
	int32_t difference, difference0;
	uint8_t i;
	i = 0;

	frequency0 = (int32_t)CLK_EXT_CLOCK_FREQ;

	do
	{
		frequency = frequency0 >> 1;
		i++;
	}
	while ((i < 3) && !(frequency < desiredClock));

	/* Difference between the desired and possible frequency */
	difference = desiredClock - frequency;

	/* Makes absolute value */
	if (difference < 0)
	{
		difference = -difference;
	}

	/* Difference between the desired and possible frequency from the previous step */
	difference0 = desiredClock - frequency0;

	/* Makes absolute value */
	if (difference0 < 0)
	{
		difference0 = -difference0;
	}

	/* If the previous step frequncy was closer to the disered, changes the i */
	if (difference0 < difference)
	{
		i--;
	}


	/* Sets the external reference clock for FLL */
	CLK_SetExternalFLLRefClock();


	/* Waits until the external clock is selected */
	while (!CLK_IsExternalFLLRefClock());

	/* Selects the external clock source */
	CLK_SetExternalClockSource();


	/* Sets the bus frequency dividor */
	CLK_SetBusFreqDiv(i);

	return (bool_t)1;
#else
    (void) desiredClock;
    return (bool_t)0;
#endif
}


/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with FLL on the external
reference clock.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline bool_t CLK_SetFLLExternalClockSync(uint32_t desiredClock)
{


	int16_t referenceFactor[6] =
	{
		CLK_FLL_FACTOR_0, CLK_FLL_FACTOR_0_DMX32,
		CLK_FLL_FACTOR_1, CLK_FLL_FACTOR_1_DMX32,
		CLK_FLL_FACTOR_2, CLK_FLL_FACTOR_2_DMX_32
	};

#if defined (CLK_EXT_REF_CLOCK_ACTIVE)
	int32_t difference, lastDifference;

	uint8_t i_ref, i_factor, i_div;
	uint8_t incr;
	uint8_t factorResult = 0;
	uint8_t divResult = 0;
	int32_t reference;
	i_ref = 0;
	reference = (int32_t)CLK_EXT_CLOCK_FREQ;

	/* Calculates the parameters the get the reference into the valid range */
	while ((i_ref < 10) & ((reference < 31250) || (reference > CLK_MAX_REF_RANGE)))
	{
		reference << 1;
		i_ref++;
	}

	/* if reference frequency was not reached */
	if (reference > CLK_MAX_REF_RANGE)
	{
		return (bool_t)0;
	}

	/* if reference frequency is the frequecy to be able to apply the max FLL gain */
	if (reference == CLK_DCO_MAX_FREQ)
	{
		incr = 1;
	}
	else
	{
		incr = 2;
	}

	/* Initializes the counter */
	i_div = 0;

	/* Initializes the variable for the max. value */
	lastDifference = 0x7fffffff;

	do
	{

		/* Initializes the counter */
		i_factor = 0;

		do
		{
			/* Calculates the difference between the desired and actual frequency */
			difference = desiredClock - ((reference * referenceFactor[i_factor]) >> i_div);

			/* Makes the absolute value */
			if (difference < 0)
			{
				difference = -difference;
			}

			/* If the new frequency difference is lower than the previous */
			if (difference < lastDifference)
			{
				/* Saves the parameters */
				lastDifference = difference;
				factorResult = i_factor;
				divResult = i_div;
			}

			/* counter incrementation */
			i_factor += incr;
		}
		while (i_factor < 6);

		/* counter incrementation */
		i_div++;
	}
	while (i_div < 4);

	/* Sets the calculated reference dividor */
	if (i_ref < 8)
	{
		CLK_SetLowFreqRange();
		CLK_SetRefDiv(i_ref);
	}
	else
	{
		CLK_SetHighFreqRange();
		CLK_SetRefDiv(i_ref - 5);
	}

	/* Sets the external reference clock for FLL */
	CLK_SetExternalFLLRefClock();


	/* Waits until the external clock is selected */
	while (!CLK_IsExternalFLLRefClock());

	/* Selects the external clock source */
	CLK_SetFLLClockSource();

	/* if DMX32 max FLL factor applied */
	if (factorResult & 0x1)
	{
		CLK_SetDCOMaxFreq();
	}
	else
	{
		CLK_SetDCODefaultFreq();
	}

	/* Sets the frequency range */
	CLK_SetDCORange(factorResult >> 1);

	/* Sets the bus frequency dividor */
	CLK_SetBusFreqDiv(divResult);

	return (bool_t)1;
#else
    (void) desiredClock;
    return (bool_t)0;
#endif
}


/**
Sets up the system clock frequency as close to the desiredClock
value as possible. The system will run on with FLL on the internal
reference clock.

\param  [in]		desiredClock 32-bit value of the desired system clock in Hz.

\return 	bool_t

\retval		1 - ok, 0 - error

\author 	Stefano Ughi
\date		25/03/2010
\version	1.0.1
\Note:		-Removed unused code (Jaroslav's E-mail 24/03/2010)

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline bool_t CLK_SetFLLInternalClockSync(uint32_t desiredClock)
{
	int32_t reference;

	int16_t referenceFactor[6] =
	{
		CLK_FLL_FACTOR_0, CLK_FLL_FACTOR_0_DMX32,
		CLK_FLL_FACTOR_1, CLK_FLL_FACTOR_1_DMX32,
		CLK_FLL_FACTOR_2, CLK_FLL_FACTOR_2_DMX_32
	};

	int32_t difference, lastDifference;

    uint8_t i_factor, i_div;
    uint8_t incr;
    uint8_t factorResult = 0;
    uint8_t divResult = 0;


	SET_TRIMMING();

#if defined (CLK_INT_REF_CLOCK_ACTIVE)

	reference = (int32_t)CLK_INT_CLOCK_FREQ;


	/* if reference frequency is the frequecy to be able to apply the max FLL gain */
	if (reference == CLK_DCO_MAX_FREQ)
	{
		incr = 1;
	}
	else
	{
		incr = 2;
	}

	/* Initializes the counter */
	i_div = 0;

	/* Initializes the variable for the max. value */
	lastDifference = 0x7fffffff;

	do
	{

		/* Initializes the counter */
		i_factor = 0;

		do
		{
			/* Calculates the difference between the desired and actual frequency */
			difference = desiredClock - ((reference * referenceFactor[i_factor]) >> i_div);

			/* Makes the absolute value */
			if (difference < 0)
			{
				difference = -difference;
			}

			/* If the new frequency difference is lower than the previous */
			if (difference < lastDifference)
			{
				/* Saves the parameters */
				lastDifference = difference;
				factorResult = i_factor;
				divResult = i_div;
			}

			/* counter incrementation */
			i_factor += incr;
		}
		while (i_factor < 6);

		/* counter incrementation */
		i_div++;
	}
	while (i_div < 4);

	/* Sets the internal reference clock for FLL */
	CLK_SetInternalFLLRefClock();


    /* Waits until the external clock is selected */
    while (!CLK_IsInternalFLLRefClock()){
    }

	/* Selects the external clock source */
	CLK_SetFLLClockSource();

	/* if DMX32 max FLL factor applied */
	if (factorResult & 0x1)
	{
		CLK_SetDCOMaxFreq();
	}
	else
	{
		CLK_SetDCODefaultFreq();
	}

	/* Sets the frequency range */
	CLK_SetDCORange(factorResult >> 1);

	/* Sets the bus frequency dividor */
	CLK_SetBusFreqDiv(divResult);

	return (bool_t)1;
#else
	return (bool_t)0;
#endif
}


/**
Sets up the trimming value. The function feeds both
the trimming register and the fine trimming bit.

\param  [in]		desiredTrim 16-bit value where the trim value is the higher 8 bits
                                and the fine trim bit is the highest bit of the lower 8 bits.
                                I.e. trim bit 8 - 15, fine trim bit 7, unused bits 0 - 6.


\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline void CLK_SetTrimSync(uint16_t desiredTrim)
{
	CLK_SetTrim((uint8_t)(desiredTrim >> 8));

	CLK_SetFTrim((uint8_t)(desiredTrim >> 7));
}

/**
Gets the trimming value. The function reads both
the trimming register and the fine trimming bit.

\return 	uint16_t

\retval		0 - 0xff80

\author 	Jaroslav Musil
\date		5/02/2010
\version	1.0.0

*/
inline uint16_t CLK_GetTrimSync(void)
{
	return (uint16_t)((CLK_GetTrim() << 8) | (CLK_GetFTrim() << 7));
}

/*@}*/


/* ********************************************************************************************** */
/*                                  END CLK Class HAL Support                                   */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN ADC Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. A/D Converter
 *
 */
/*@{*/

#if defined(HAL_ADC_SUPPORT)
/* ****************** */
/* Defines and Macros */
/* ****************** */
#define ADC_PIN(x,y,z)\
{\
(DIO_PortAddressBasicType)&PT##x##D,(DIO_ModuleMapMaskType)PT##x##D_PT##x##D##y##_MASK, (uint8_t)z \
}

/* Max clock in Hz */
#define ADC_MAX_CLOCK               8000000

/* Max clock in Hz when low power mode */
#define ADC_MAX_CLOCK_LOW_POWER     4000000

/* Max clock in Hz when low power mode */
#define ADC_MIN_CLOCK               400000

/* This defines if the Vrefh-Vrefl reference is used. Otherwise the Vdda-Vssa is used */
#define ADC_REFERENCE_VREFH_VREFL   1

/* Defines if the ADC runs in low power mode; otherwise it runs in fast mode */
#define ADC_LOW_POWER_MODE          0

/* Defines the long sample time; otherwise it is in the short sample time mode */
#define ADC_LONG_SAMPLE_TIME        1


/* 8 is the max. avaible value for flash module clock divisor */
#if ((BUS_FREQUENCY_Hz / ADC_CONV_FREQUENCY_Hz) > 8)
#define ADC_BUS_CLOCK_DIV_2	1
#define ADC_DIV ((BUS_FREQUENCY_Hz / 2 / ADC_CONV_FREQUENCY_Hz) - 1)
#if (ADC_DIV > 8)
#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, divisor too high!"
#endif
#else
#define ADC_BUS_CLOCK_DIV_2	0
#define ADC_DIV ((BUS_FREQUENCY_Hz / ADC_CONV_FREQUENCY_Hz) - 1)
#if (ADC_DIV < 0)
#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, divisor is too low!"
#endif
#endif

#if (ADC_LOW_POWER_MODE)
#if ((BUS_FREQUENCY_Hz / (2 * ADC_BUS_CLOCK_DIV_2) / (ADC_DIV + 1)) > ADC_MAX_CLOCK_LOW_POWER)
#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, the resulted frequnecy is too high!"
#endif
#else
#if ((BUS_FREQUENCY_Hz / (2 * ADC_BUS_CLOCK_DIV_2) / (ADC_DIV + 1)) > ADC_MAX_CLOCK)
#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, the resulted frequnecy is too high!"
#endif
#endif

#if ((BUS_FREQUENCY_Hz / (2 * ADC_BUS_CLOCK_DIV_2) / (ADC_DIV + 1)) < ADC_MIN_CLOCK)
#error "Check BUS_FREQUENCY_Hz and ADC_CONV_FREQUENCY_Hz, the resulted frequnecy is too low!"
#endif

#if (ADC_RESOLUTION > 10)
#define ADC_RESOLUTION_MODE         12
#elif (ADC_RESOLUTION > 8)
#define ADC_RESOLUTION_MODE         10
#else
#define ADC_RESOLUTION_MODE         8
#endif

#define ADC_RESOLUTION_RIGHT_SHIFT (3 + ADC_RESOLUTION_MODE - ADC_RESOLUTION)



#define _HAL_MCF51_ADC_ADCSC1A_REG              (ADCSC1A)
#define _HAL_MCF51_ADC_ADCSC1B_REG              (ADCSC1B)
#define _HAL_MCF51_ADC_ADCSC2_REG               (ADCSC2)
#define _HAL_MCF51_ADC_ADCCFG_REG               (ADCCFG)
#define _HAL_MCF51_ADC_ADCRA_REG                (ADCRA)
#define _HAL_MCF51_ADC_ADCRB_REG                (ADCRB)

#define _HAL_MCF51_ADC_ADCSC1_COCO_MASK         (ADCSC1A_COCO_MASK)
#define _HAL_MCF51_ADC_ADCSC1_AIEN_MASK         (ADCSC1A_AIEN_MASK)
#define _HAL_MCF51_ADC_ADCSC1_ADCO_MASK         (ADCSC1A_ADCO_MASK)
#define _HAL_MCF51_ADC_ADCSC1_ADCH_MASK         (ADCSC1A_ADCH_MASK)

#define _HAL_MCF51_ADC_ADCSC2_ADACT_MASK        (ADCSC2_ADACT_MASK)
#define _HAL_MCF51_ADC_ADCSC2_ADTRG_MASK        (ADCSC2_ADTRG_MASK)
#define _HAL_MCF51_ADC_ADCSC2_BB_MASK           (ADCSC2_BB_MASK)
#define _HAL_MCF51_ADC_ADCSC2_DMAEN_MASK        (ADCSC2_DMAEN_MASK)
#define _HAL_MCF51_ADC_ADCSC2_REFSEL_MASK       (ADCSC2_REFSEL_MASK)

#define _HAL_MCF51_ADC_ADCCFG_ADLPC_MASK        (ADCCFG_ADLPC_MASK)
#define _HAL_MCF51_ADC_ADCCFG_ADIV_MASK         (ADCCFG_ADIV_MASK)
#define _HAL_MCF51_ADC_ADCCFG_ADIV_SHIFT        (ADCCFG_ADIV_BITNUM)
#define _HAL_MCF51_ADC_ADCCFG_ADLSMP_MASK       (ADCCFG_ADLSMP_MASK)
#define _HAL_MCF51_ADC_ADCCFG_MODE_MASK         (ADCCFG_MODE_MASK)
#define _HAL_MCF51_ADC_ADCCFG_MODE_SHIFT        (ADCCFG_MODE_BITNUM)
#define _HAL_MCF51_ADC_ADCCFG_ADICLK_MASK       (ADCCFG_ADICLK_MASK)

#define _HAL_MCF51_ADC_SAPE1_REG                (SAPE1)
#define _HAL_MCF51_ADC_SAPE2_REG                (SAPE2)
#define _HAL_MCF51_ADC_SAPE3_REG                (SAPE3)

#define ADC_ClockGateOn()                       (SCGC1|=SCGC1_ADC_MASK)
#define ADC_ClockGateOff()                      (SCGC1&=(~SCGC1_ADC_MASK))

#define ADC_IsConversionCompleteA()		        ((_HAL_MCF51_ADC_ADCSC1A_REG & _HAL_MCF51_ADC_ADCSC1_COCO_MASK) != 0)
#define ADC_IsConversionCompleteB()		        ((_HAL_MCF51_ADC_ADCSC1B_REG & _HAL_MCF51_ADC_ADCSC1_COCO_MASK) != 0)

#define ADC_EnableInterruptA()  		        {_HAL_MCF51_ADC_ADCSC1A_REG |= _HAL_MCF51_ADC_ADCSC1_AIEN_MASK;}
#define ADC_DisableInterruptA()  		        {_HAL_MCF51_ADC_ADCSC1A_REG &= ~_HAL_MCF51_ADC_ADCSC1_AIEN_MASK;}

#define ADC_EnableInterruptB()  		        {_HAL_MCF51_ADC_ADCSC1B_REG |= _HAL_MCF51_ADC_ADCSC1_AIEN_MASK;}
#define ADC_DisableInterruptB()  		        {_HAL_MCF51_ADC_ADCSC1B_REG &= ~_HAL_MCF51_ADC_ADCSC1_AIEN_MASK;}

#define ADC_ContinuesConversionA()  		    {_HAL_MCF51_ADC_ADCSC1A_REG |= _HAL_MCF51_ADC_ADCSC1_ADCO_MASK;}
#define ADC_SingleConversionA()  		        {_HAL_MCF51_ADC_ADCSC1A_REG &= ~_HAL_MCF51_ADC_ADCSC1_ADCO_MASK;}

#define ADC_ContinuesConversionB()  		    {_HAL_MCF51_ADC_ADCSC1B_REG |= _HAL_MCF51_ADC_ADCSC1_ADCO_MASK;}
#define ADC_SingleConversionB()  		        {_HAL_MCF51_ADC_ADCSC1B_REG &= ~_HAL_MCF51_ADC_ADCSC1_ADCO_MASK;}

#define ADC_SelectChannelA(channel)  		    {_HAL_MCF51_ADC_ADCSC1A_REG = (uint8_t)((_HAL_MCF51_ADC_ADCSC1A_REG & ~_HAL_MCF51_ADC_ADCSC1_ADCH_MASK) | ((uint8_t)channel & _HAL_MCF51_ADC_ADCSC1_ADCH_MASK));}
#define ADC_SelectChannelB(channel)  		    {_HAL_MCF51_ADC_ADCSC1B_REG = (uint8_t)((_HAL_MCF51_ADC_ADCSC1B_REG & ~_HAL_MCF51_ADC_ADCSC1_ADCH_MASK) | ((uint8_t)channel & _HAL_MCF51_ADC_ADCSC1_ADCH_MASK));}

#define ADC_ReadChannelA()  		            (_HAL_MCF51_ADC_ADCSC1A_REG & _HAL_MCF51_ADC_ADCSC1_ADCH_MASK)
#define ADC_ReadChannelB()  		            (_HAL_MCF51_ADC_ADCSC1B_REG & _HAL_MCF51_ADC_ADCSC1_ADCH_MASK)

#define ADC_DisableChannelA()  		            {_HAL_MCF51_ADC_ADCSC1A_REG |= _HAL_MCF51_ADC_ADCSC1_ADCH_MASK;}
#define ADC_DisableChannelB()         		    {_HAL_MCF51_ADC_ADCSC1B_REG |= _HAL_MCF51_ADC_ADCSC1_ADCH_MASK;}

#define ADC_IsConversionActive()  		        ((_HAL_MCF51_ADC_ADCSC2_REG & _HAL_MCF51_ADC_ADCSC2_ADACT_MASK) =! 0;}

#define ADC_SetHWTrigger()        		        {_HAL_MCF51_ADC_ADCSC2_REG |= _HAL_MCF51_ADC_ADCSC2_ADTRG_MASK;}
#define ADC_SetSWTrigger()        		        {_HAL_MCF51_ADC_ADCSC2_REG &= ~_HAL_MCF51_ADC_ADCSC2_ADTRG_MASK;}

#define ADC_SetBackToBackConversion()           {_HAL_MCF51_ADC_ADCSC2_REG |= _HAL_MCF51_ADC_ADCSC2_BB_MASK;}
#define ADC_SetIndependentConversion()          {_HAL_MCF51_ADC_ADCSC2_REG &= ~_HAL_MCF51_ADC_ADCSC2_BB_MASK;}

#define ADC_EnableDMA()                         {_HAL_MCF51_ADC_ADCSC2_REG |= _HAL_MCF51_ADC_ADCSC2_DMAEN_MASK;}
#define ADC_DisableDMA()                        {_HAL_MCF51_ADC_ADCSC2_REG &= ~_HAL_MCF51_ADC_ADCSC2_DMAEN_MASK;}

#define ADC_SetVRefToVrefhVrefl()               {_HAL_MCF51_ADC_ADCSC2_REG = (uint8_t)((_HAL_MCF51_ADC_ADCSC2_REG & ~_HAL_MCF51_ADC_ADCSC2_REFSEL_MASK) | ((uint8_t)(0) & _HAL_MCF51_ADC_ADCSC2_REFSEL_MASK));}
#define ADC_SetVRefToVddaVssa()                 {_HAL_MCF51_ADC_ADCSC2_REG = (uint8_t)((_HAL_MCF51_ADC_ADCSC2_REG & ~_HAL_MCF51_ADC_ADCSC2_REFSEL_MASK) | ((uint8_t)(1) & _HAL_MCF51_ADC_ADCSC2_REFSEL_MASK));}
#define ADC_SetVRefToOnChipBandgap()            {_HAL_MCF51_ADC_ADCSC2_REG = (uint8_t)((_HAL_MCF51_ADC_ADCSC2_REG & ~_HAL_MCF51_ADC_ADCSC2_REFSEL_MASK) | ((uint8_t)(2) & _HAL_MCF51_ADC_ADCSC2_REFSEL_MASK));}

#define ADC_SetLowPowerConversion()             {_HAL_MCF51_ADC_ADCCFG_REG |= _HAL_MCF51_ADC_ADCCFG_ADLPC_MASK;}
#define ADC_SetHighSpeedConversion()            {_HAL_MCF51_ADC_ADCCFG_REG &= ~_HAL_MCF51_ADC_ADCCFG_ADLPC_MASK;}

#define ADC_SelectClockDiv(div)                 {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_ADIV_MASK) | (((uint8_t)div << _HAL_MCF51_ADC_ADCCFG_ADIV_SHIFT) & _HAL_MCF51_ADC_ADCCFG_ADIV_MASK));}

#define ADC_SetLongSampleTime()                 {_HAL_MCF51_ADC_ADCCFG_REG |= _HAL_MCF51_ADC_ADCCFG_ADLSMP_MASK;}
#define ADC_SetShortSampleTime()                {_HAL_MCF51_ADC_ADCCFG_REG &= ~_HAL_MCF51_ADC_ADCCFG_ADLSMP_MASK;}

#define ADC_SetConversion8bit()                 {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_MODE_MASK) | (((uint8_t)(0) << _HAL_MCF51_ADC_ADCCFG_MODE_SHIFT) & _HAL_MCF51_ADC_ADCCFG_MODE_MASK));}
#define ADC_SetConversion10bit()                {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_MODE_MASK) | (((uint8_t)(2) << _HAL_MCF51_ADC_ADCCFG_MODE_SHIFT) & _HAL_MCF51_ADC_ADCCFG_MODE_MASK));}
#define ADC_SetConversion12bit()                {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_MODE_MASK) | (((uint8_t)(1) << _HAL_MCF51_ADC_ADCCFG_MODE_SHIFT) & _HAL_MCF51_ADC_ADCCFG_MODE_MASK));}

#define ADC_SetBusClock()                       {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_ADICLK_MASK) | ((uint8_t)(0) & _HAL_MCF51_ADC_ADCCFG_ADICLK_MASK));}
#define ADC_SetBusClockDiv2()                   {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_ADICLK_MASK) | ((uint8_t)(1) & _HAL_MCF51_ADC_ADCCFG_ADICLK_MASK));}
#define ADC_SetAlternateClock()                 {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_ADICLK_MASK) | ((uint8_t)(2) & _HAL_MCF51_ADC_ADCCFG_ADICLK_MASK));}
#define ADC_SetAsynchronousClock()              {_HAL_MCF51_ADC_ADCCFG_REG = (uint8_t)((_HAL_MCF51_ADC_ADCCFG_REG & ~_HAL_MCF51_ADC_ADCCFG_ADICLK_MASK) | ((uint8_t)(3) & _HAL_MCF51_ADC_ADCCFG_ADICLK_MASK));}

#define ADC_ReadResultA()		                (_HAL_MCF51_ADC_ADCRA_REG)
#define ADC_ReadResultB()		                (_HAL_MCF51_ADC_ADCRB_REG)

/* added channel parameter for compatibility with others uc S.U. 31/05/2010 */
#define ADC_pResultRegister(channel)            ((uint16_t *)&_HAL_MCF51_ADC_ADCRA_REG)
#define ADC_pChannelRegister(channel)           ((uint8_t *)&_HAL_MCF51_ADC_ADCSC1A_REG)

#define ADC_INT_ENABLED 1
#define ADC_INT_DISABLED 0

/**
Disable the I/O port control of MCU pins that are used as analog inputs.
*/
/*lint -emacro(734,ADC_ConfigurePinAsChannel) */
#define ADC_ConfigurePinAsChannel(channel)\
{\
if (channel <= 7)\
{\
_HAL_MCF51_ADC_SAPE1_REG |= (1 << channel);\
}\
else if (channel <= 15)\
{\
_HAL_MCF51_ADC_SAPE2_REG |= (1 << (channel-8));\
}\
else if (channel <= 23)\
{\
_HAL_MCF51_ADC_SAPE3_REG |= (1 << (channel-16));\
}\
else\
{\
}\
}
/**
Enable the I/O port control of MCU pins that are not used as analog inputs.*/
#define ADC_DeConfigurePinAsChannel(channel)\
{if (channel <= 7) _HAL_MCF51_ADC_SAPE1_REG &= (~(1 << channel));\
else if (channel <= 15) _HAL_MCF51_ADC_SAPE2_REG &= (~(1 << (channel-8)));\
else if (channel <= 23) _HAL_MCF51_ADC_SAPE3_REG &= (~(1 << (channel-16)));\
}


/**
'0' if channel is valid; otherwise '1'
*/
#define ADC_CheckChannel(channel)               (channel >= 0x1f)

/**
\n Writes to ADC registers:
\n\li enables/disables interrupt from conversion on A
\n\li sets independent conversion
\n\li sets single converison
\n\li interrupts any running conversion
\n\li disables hw trigger
\n\li disables dma
\n\li initiates a new conversion on the defined channel.
*/
/*lint -emacro(774,ADC_StartConvertionOnChannel) */
#define ADC_StartConvertionOnChannel(channel, atdInterrupt)\
{\
if (atdInterrupt == ADC_INT_ENABLED)\
{\
ADC_EnableInterruptA()\
}\
else\
{\
ADC_DisableInterruptA();\
}\
ADC_SetIndependentConversion();\
ADC_SingleConversionA();\
ADC_SetSWTrigger();\
ADC_DisableDMA();\
ADC_SelectChannelA(channel);\
}

#if (ADC_USE_SW_DMA | ADC_USE_HW_DMA)
/**
\n Writes to ADC registers:
\n\li enables interrupt from conversion on A
\n\li sets independent conversion
\n\li sets continues converison
\n\li interrupts any running conversion
\n\li disables hw trigger
\n\li disables dma
\n\li initiates a new conversion on the defined channel.

*/
#define ADC_StartMultiSampleConvertionOnChannel(channel)    {ADC_EnableInterruptA(); ADC_SetIndependentConversion(); ADC_ContinuesConversionA(); ADC_SetSWTrigger(); ADC_SelectChannelA(channel);}

/**
\n Writes to ADC registers:
\n\li enables interrupt from conversion on A
\n\li sets independent conversion
\n\li sets single converison
\n\li interrupts any running conversion
\n\li disables hw trigger
\n\li disables dma

*/
#define ADC_StartMultiSampleConvertionMultiChannel()        {ADC_EnableInterruptA(); ADC_SetIndependentConversion(); ADC_SingleConversionA(); ADC_SetSWTrigger();}

/**
\n Converts the channel number to register value:
\n\li Ores the channel number with the IE mask in order the interrupt to be still enabled
*/
#define ADC_CorrectChannel(channel)     (channel | _HAL_MCF51_ADC_ADCSC1_AIEN_MASK)
#endif
typedef struct ADC_Map_tag
{
	DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register relaated to the port pin */
	DIO_ModuleMapMaskType        PinMask;     /**< Port pin mask  */
	uint8_t channel; /**< ADC Channel number  */
} ADC_MapType;
#define BEGIN_MODULE_ADC_MAP(n)    static const ADC_MapType ADCMap[n]={

#define END_MODULE_ADC_MAP()       };

#if defined(MCU_CASTELLATED)
#define ADC_MAPPED_CHANNELS_NUMBER 19
/* Table of mapped channels */
BEGIN_MODULE_ADC_MAP(ADC_MAPPED_CHANNELS_NUMBER)
ADC_PIN(C,2,8),
        ADC_PIN(D,7,7),
        ADC_PIN(D,3,5),
        ADC_PIN(B,6,12),
        ADC_PIN(D,5,1),
        ADC_PIN(D,4,2),
        ADC_PIN(G,3,4),
        ADC_PIN(B,4,14),
        ADC_PIN(B,3,15),
        ADC_PIN(B,2,16),
        ADC_PIN(A,7,23),
        ADC_PIN(B,0,18),
        ADC_PIN(B,1,17),
        ADC_PIN(B,5,13),
        ADC_PIN(B,7,11),
        ADC_PIN(D,6,0),
        ADC_PIN(D,0,10),
        ADC_PIN(D,2,6),
        ADC_PIN(D,1,9)
        END_MODULE_ADC_MAP()
#endif /* #if defined(MCU_CASTELLATED) */
#if defined (DEBUG_BOARD)
#define ADC_MAPPED_CHANNELS_NUMBER 24
        /* Table of mapped channels */
        BEGIN_MODULE_ADC_MAP(ADC_MAPPED_CHANNELS_NUMBER)
        ADC_PIN(A,7,23),

        ADC_PIN(B,0,18),
        ADC_PIN(B,1,17),
        ADC_PIN(B,2,16),
        ADC_PIN(B,3,15),
        ADC_PIN(B,4,14),
        ADC_PIN(B,5,13),
        ADC_PIN(B,6,12),
        ADC_PIN(B,7,11),

        ADC_PIN(C,2,8),

        ADC_PIN(D,0,10),
        ADC_PIN(D,1,9),
        ADC_PIN(D,2,6),
        ADC_PIN(D,3,5),
        ADC_PIN(D,4,2),
        ADC_PIN(D,5,1),
        ADC_PIN(D,6,0),
        ADC_PIN(D,7,7),

        ADC_PIN(G,3,4),
        ADC_PIN(G,4,3),

        ADC_PIN(H,0,22),
        ADC_PIN(H,1,21),
        ADC_PIN(H,2,20),
        ADC_PIN(H,3,19),
        END_MODULE_ADC_MAP()

#endif  /* #if defined (DEBUG_BOARD) */

        /*
        Starting from portAddress and pinMask (read pin) search for AD channel
        */
        inline bool_t ADC_GetChannel(DIO_ModuleMapPortAddressType portAddress, DIO_ModuleMapMaskType pinMask, uint8_t *pADch)
{
	uint8_t j;
	bool_t channelfound = FALSE;


	for (j = 0; j < ADC_MAPPED_CHANNELS_NUMBER; j++)
	{
		if (ADCMap[j].PortAddress == portAddress.basic)
		{
			if (ADCMap[j].PinMask == pinMask)
			{
				channelfound = TRUE;
				break;
			}
		}
	}

	if (channelfound)
	{
		*pADch = ADCMap[j].channel;
	}

	return channelfound;
}
/**
Channel selected for ATD convertion
*/
#define ADC_GetActiveChannel()	(ADC_ReadChannelA())

/**
\n Wait for Conversion Complete.
*/
#define ADC_WaitForConvertionOnChannel(channel)\
while(!ADC_IsConversionCompleteA())\
{\
}
/**
\n\li ADC Conversion Result depends on the desired resolution and the module can be programmed for 8, 10 or 12 bits accordingly
\n\li This function get the converted value and modify it to the desired resolution
 */
#if (ADC_RESOLUTION_RIGHT_SHIFT > 0)
#define ADC_GetConvertedValueOnChannel(ch)		                (ADC_ReadResultA() >> ADC_RESOLUTION_RIGHT_SHIFT)
#define ADC_GetCorrectResolutionResult(result)                  (result >> ADC_RESOLUTION_RIGHT_SHIFT)
#elif (ADC_RESOLUTION_RIGHT_SHIFT < 0)
#define ADC_GetConvertedValueOnChannel(ch)		                (ADC_ReadResultA() << -ADC_RESOLUTION_RIGHT_SHIFT)
#define ADC_GetCorrectResolutionResult(result)                  (result << -ADC_RESOLUTION_RIGHT_SHIFT)
#else
#define ADC_GetConvertedValueOnChannel(ch)		                (ADC_ReadResultA())
#define ADC_GetCorrectResolutionResult(result)                  (result)
#endif




/**
\n Test Conversion Complete Flag.
\n The CCF is a read-only bit which is set each time a conversion is complete.
*/
#define ADC_EndConvertion()             (ADC_IsConversionCompleteA())

/**
\n Stops any possible ADC conversion by applying the 0x1f channel.
*/
#define ADC_StopConversion()            ADC_DisableChannelA()

inline void ADC_Init(void)
{
	/* Bus clock to the ADC module is enabled  */
	ADC_ClockGateOn();
#if (ADC_REFERENCE_VREFH_VREFL)
	/* Uses the Vrefh and Vrefl pins for the reference */
	ADC_SetVRefToVrefhVrefl();
#else
	/* Uses the Vdda and Vssa pins for the reference */
	ADC_SetVRefToVddaVssa();
#endif

#if (ADC_LOW_POWER_MODE)
	/* Low power mode */
	ADC_SetLowPowerConversion();
#else
	/* High speed mode */
	ADC_SetHighSpeedConversion();
#endif

#if (ADC_LONG_SAMPLE_TIME)
	/* Long sample time */
	ADC_SetLongSampleTime();
#else
	/* Short sample time */
	ADC_SetShortSampleTime();
#endif

#if (ADC_RESOLUTION_MODE == 12)
	/* ADC will sample in 12-bit operation */
	ADC_SetConversion12bit();
#elif (ADC_RESOLUTION_MODE == 10)
	/* ADC will sample in 10-bit operation */
	ADC_SetConversion10bit();
#else
	/* ADC will sample in 8-bit operation */
	ADC_SetConversion8bit();
#endif

#if (ADC_BUS_CLOCK_DIV_2)
	/* The ADC source clock is bus / 2 */
	ADC_SetBusClockDiv2();
#else
	/* The ADC source clock is bus */
	ADC_SetBusClock();
#endif

	/* Sets the ADC dividor */
	ADC_SelectClockDiv(ADC_DIV);

	/* Sets the independent conversion of A & B results */
	ADC_SetIndependentConversion();
}

/**
Set all ADC Registers to the reset state value
*/
#define ADC_DeInit()\
{\
_HAL_MCF51_ADC_SAPE1_REG = 0;\
_HAL_MCF51_ADC_SAPE2_REG = 0;\
_HAL_MCF51_ADC_SAPE3_REG = 0;\
_HAL_MCF51_ADC_ADCSC1A_REG = 0x001F;\
_HAL_MCF51_ADC_ADCSC1B_REG = 0x001F;\
_HAL_MCF51_ADC_ADCSC2_REG = 0x0000;\
_HAL_MCF51_ADC_ADCCFG_REG = 0x0000;\
ADC_ClockGateOff();\
}

#endif /* #if defined(HAL_ADC_SUPPORT) */
/*@}*/
/* ********************************************************************************************** */
/*                                  END ADC Class HAL Support                                     */
/* ********************************************************************************************** */


/* ********************************************************************************************** */
/*                                  BEGIN UART Class HAL Support                                   */
/* ********************************************************************************************** */


/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Define UART1 Capability */
#define _HAL_UARTx_CAPABILITY
#ifdef HAL_OPTION_UART
#if     HAL_OPTION_UART ==  1
#define HAL_UART_DEV   1
#elif   HAL_OPTION_UART ==  2
#define HAL_UART_DEV   2
#else
#error "uart not available"
#endif
#endif



/* Baud rates calculations */

/*
OPTION__DEVICE_UART_BAUDRATE_DIVISOR=[0,1,2,3]
APPL_HALUART_CALC_n_DIVISOR calcola 2^(2n-1)*/
/*#define HAL_UART_CALC_n_DIVISOR             		(HAL_OPTION_UART_BAUDRATE_DIVISOR==0 ? 0.5: \
																								(HAL_OPTION_UART_BAUDRATE_DIVISOR==1 ? 2.0: \
																								(HAL_OPTION_UART_BAUDRATE_DIVISOR==2 ? 8.0: 32.0)))*/
//#define HAL_UART_DIVISOR												HAL_UART_CALC_n_DIVISOR
#define _HALUART_CALC_BAUD(baud,divisor)   (uint16_t)((uint16_t)((BUS_FREQUENCY_Hz / (16.0 * (baud)))))

/** Baud Rates */
#define _HALUART_BAUD_115200               (_HALUART_CALC_BAUD(115200UL,	))
#define _HALUART_BAUD_76800                (_HALUART_CALC_BAUD(76800UL,		))
#define _HALUART_BAUD_57600                (_HALUART_CALC_BAUD(57600UL,		))
#define _HALUART_BAUD_38400                (_HALUART_CALC_BAUD(38400UL,		))
#define _HALUART_BAUD_28800                (_HALUART_CALC_BAUD(28800U,		))
#define _HALUART_BAUD_19200                (_HALUART_CALC_BAUD(19200U,		))
#define _HALUART_BAUD_14400                (_HALUART_CALC_BAUD(14400,			))
#define _HALUART_BAUD_9600                 (_HALUART_CALC_BAUD(9600,			))
#define _HALUART_BAUD_4800                 (_HALUART_CALC_BAUD(4800,			))
#define _HALUART_BAUD_2400                 (_HALUART_CALC_BAUD(2400,			))
#define _HALUART_BAUD_1200                 (_HALUART_CALC_BAUD(1200,			))



#define UART_CLOCK_GATE(channel) 			SCGC1_SCI##channel

#define _HAL_MCF51_UART_ERROR_MASK		    (SCI1S1_FE_MASK | SCI1S1_PF_MASK | SCI1S1_OR_MASK)


#define _HAL_MCF51_UART_BD_REG(channel) 			    (SCI##channel##BD)
#define _HAL_MCF51_UART_C1_REG(channel) 			    (SCI##channel##C1)
#define _HAL_MCF51_UART_C2_REG(channel) 			    (SCI##channel##C2)
#define _HAL_MCF51_UART_S1_REG(channel) 			    (SCI##channel##S1)
#define _HAL_MCF51_UART_S2_REG(channel) 			    (SCI##channel##S2)
#define _HAL_MCF51_UART_C3_REG(channel) 			    (SCI##channel##C3)
#define _HAL_MCF51_UART_D_REG(channel)                  (SCI##channel##D)
#define _HAL_MCF51_UART_C5_REG(channel) 			    (SCI##channel##C5)

#define _HAL_MCF51_UART_BD_SBR_MASK                     (SCI1BD_SBR_MASK)

#define _HAL_MCF51_UART_C1_PE_MASK                      (SCI1C1_PE_MASK)
#define _HAL_MCF51_UART_C1_PT_MASK                      (SCI1C1_PT_MASK)
#define _HAL_MCF51_UART_C1_M_MASK                       (SCI1C1_M_MASK)

#define _HAL_MCF51_UART_C2_TE_MASK                      (SCI1C2_TE_MASK)
#define _HAL_MCF51_UART_C2_RE_MASK                      (SCI1C2_RE_MASK)
#define _HAL_MCF51_UART_C2_TIE_MASK                     (SCI1C2_TIE_MASK)
#define _HAL_MCF51_UART_C2_RIE_MASK                     (SCI1C2_RIE_MASK)

#define _HAL_MCF51_UART_S1_PF_MASK                      (SCI1S1_PF_MASK)
#define _HAL_MCF51_UART_S1_FE_MASK                      (SCI1S1_FE_MASK)
#define _HAL_MCF51_UART_S1_OR_MASK                      (SCI1S1_OR_MASK)
#define _HAL_MCF51_UART_S1_TDRE_MASK                    (SCI1S1_TDRE_MASK)
#define _HAL_MCF51_UART_S1_TC_MASK                      (SCI1S1_TC_MASK)
#define _HAL_MCF51_UART_S1_RDRF_MASK                    (SCI1S1_RDRF_MASK)

#define _HAL_MCF51_UART_C5_TDMAS_MASK 			        (SCI1C5_TDMAS_MASK)
#define _HAL_MCF51_UART_C5_RDMAS_MASK 			        (SCI1C5_RDMAS_MASK)

/*  Peripheral management and configuration
 *
 */



#define 	UART_ClkEnable(n)		        UART_CLOCK_GATE(n) = 1
#define 	UART_ClkDisable(n)		        UART_CLOCK_GATE(n) = 0
#define 	UART_Enable(n)		            /* not used */
#define     UART_TxEnable(n)                { _HAL_MCF51_UART_C2_REG(n) |= _HAL_MCF51_UART_C2_TE_MASK; }
#define     UART_RxEnable(n)                { _HAL_MCF51_UART_C2_REG(n) |= _HAL_MCF51_UART_C2_RE_MASK; }



#define     UART_TxDisable(n)  	            { _HAL_MCF51_UART_C2_REG(n) &= ~_HAL_MCF51_UART_C2_TE_MASK; }
#define     UART_RxDisable(n)  	            { _HAL_MCF51_UART_C2_REG(n) &= ~_HAL_MCF51_UART_C2_RE_MASK; }
#define 	UART_Disable(n)		            /* not used */


#define     UART_SetParityVal(n, val)	    { (val) ? _HAL_MCF51_UART_C1_REG(n) |= _HAL_MCF51_UART_C1_PE_MASK; : _HAL_MCF51_UART_C1_REG(n) &= ~_HAL_MCF51_UART_C1_PE_MASK;}
#define     UART_SetParityMode(n, val)	    { if(val) \
                                              _HAL_MCF51_UART_C1_REG(n) |= _HAL_MCF51_UART_C1_PT_MASK; \
                                              else _HAL_MCF51_UART_C1_REG(n) &= ~_HAL_MCF51_UART_C1_PT_MASK;}

#define     UART_SetDataLen(n, val)			{ if (val) \
                                              _HAL_MCF51_UART_C1_REG(n) |= _HAL_MCF51_UART_C1_M_MASK; \
                                              else _HAL_MCF51_UART_C1_REG(n) &= ~_HAL_MCF51_UART_C1_M_MASK;}

#define     UART_SetStop(n ,val)					    /* not used */
#define     UART_SetMode(n, val)					    /* not used */
#define     UART_SetClockDivisor(n, val)	//{_HAL_MCF51_UART_BD_REG(n) = (uint16_t)((_HAL_MCF51_UART_BD_REG(n) & ~_HAL_MCF51_UART_BD_SBR_MASK)) | (_HAL_MCF51_UART_BD_REG(n) | ((uint16_t)val & _HAL_MCF51_UART_BD_SBR_MASK));}


/*@}*/



/*  Data register
 *
 */

#define     UART_ReadData(n) 		        (_HAL_MCF51_UART_D_REG(n));

#define     UART_WriteData(n, b) 	        {_HAL_MCF51_UART_D_REG(n) = (UART_DataRegType)(b); }


#define     UART_pReceiveDataRegister(n) 	((UART_DataRegType*)&_HAL_MCF51_UART_D_REG(n))

#define     UART_pTransmitDataRegister(n)   ((UART_DataRegType*)&_HAL_MCF51_UART_D_REG(n))


/* Configuration
 *
 */
#define     UART_SetBaudRate(n ,b) 	        {_HAL_MCF51_UART_BD_REG(n) = (uint16_t)((_HAL_MCF51_UART_BD_REG(n) & (uint16_t)(SCI1BD_RXEDGIE_MASK | SCI1BD_LBKDIE_MASK)) | ( (UART_BaudRateType)(b)));}


/* Interrupt & DMA
 *
 */
#define     UART_TxIntEnable(n)             { _HAL_MCF51_UART_C2_REG(n) |= _HAL_MCF51_UART_C2_TIE_MASK; }
#define     UART_RxIntEnable(n)             { _HAL_MCF51_UART_C2_REG(n) |= _HAL_MCF51_UART_C2_RIE_MASK; }

#define     UART_TxIntDisable(n)            { _HAL_MCF51_UART_C2_REG(n) &= ~_HAL_MCF51_UART_C2_TIE_MASK; }
#define     UART_RxIntDisable(n)            { _HAL_MCF51_UART_C2_REG(n) &= ~_HAL_MCF51_UART_C2_RIE_MASK; }

#define     UART_TxDMAEnable(n)             { _HAL_MCF51_UART_C5_REG(n) |= _HAL_MCF51_UART_C5_TDMAS_MASK; }
#define     UART_RxDMAEnable(n)             { _HAL_MCF51_UART_C5_REG(n) |= _HAL_MCF51_UART_C5_RDMAS_MASK; }

#define     UART_TxDMADisable(n)            { _HAL_MCF51_UART_C5_REG(n) &= ~_HAL_MCF51_UART_C5_TDMAS_MASK; }
#define     UART_RxDMADisable(n)            { _HAL_MCF51_UART_C5_REG(n) &= ~_HAL_MCF51_UART_C5_RDMAS_MASK; }



/*  State
 *
 */
#define     UART_IsPeError(n)        	    (_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_PF_MASK)	/* \todo deve ritornare un valore fisso se non abilitato */
#define     UART_IsFerError(n)       	    (_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_FE_MASK)
#define     UART_IsOerError(n)       	    (_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_OR_MASK)
#define     UART_IsError(n)				    (_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_ERROR_MASK)


#define     UART_ClearError(n)       	    {_HAL_MCF51_UART_S1_REG(n); _HAL_MCF51_UART_D_REG(n);}
#define 	UART_ResetPeError(n)		    {_HAL_MCF51_UART_S1_REG(n); _HAL_MCF51_UART_D_REG(n);}
#define     UART_ClearRxIrq(n)       	    {} /* not needed : regiter reading does it */
#define     UART_ClearTxIrq(n)       	    {}

#define     UART_IsTxInt(n)           	    ((_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_TDRE_MASK) != 0)
#define     UART_IsTxEndInt(n)			    ((_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_TC_MASK) != 0)
#define     UART_IsRxInt(n)           	    ((_HAL_MCF51_UART_S1_REG(n) & _HAL_MCF51_UART_S1_RDRF_MASK) != 0)

#define 	UART_IsTxDataRegFull(n)   	    ((_HAL_MCF51_UART_S1_REG(n) & (_HAL_MCF51_UART_S1_TDRE_MASK | _HAL_MCF51_UART_S1_TC_MASK)) == 0)



/* ********************************************************************************************** */
/*                                  END UART Class HAL Support                                   */
/* ********************************************************************************************** */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* I2C Class HAL Support */
/* ********************* */

/** @name Class I2C
 *
 */
/*@{*/


/* ****************** */
/* Defines and Macros */
/* ****************** */
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_ISR                        (1)
#define OCI2C_ON_SLAVE_MATCH_ADDRESS_TX_EVENT_IN_DMACALLBACK                (0)
#define OCI2C_ON_SLAVE_TX_MODE_IN_DMACALLBACK                               (1)

/** Define I2C Capability */
#define _HAL_I2C_CAPABILITY

/** Define DMA Capability */
#define _HAL_DMA_CAPABILITY

/** Map mcu specific registers names to a 'standard' set of names

              HAL_<FAMILY>_<HAL_CLASS_PREFIX>_<NAME/DESCRIPTION>

    (this helps when generating new files for additional parts of the same family).
*/

/*******   IIC   ************************************/

#define _HAL_MCF51_I2C_SAR_REG                      (IICA1)
#define _HAL_MCF51_I2C_IICF_REG                     (IICF)
#define _HAL_MCF51_I2C_CTRL1_REG                    (IICC1)
#define _HAL_MCF51_I2C_TX_DATA_REG                  (IICD)
#define _HAL_MCF51_I2C_RX_DATA_REG                  (IICD)
#define _HAL_MCF51_I2C_STATUS_REG                   (IICS)
#define _HAL_MCF51_I2C_CTRL2_REG                    (IICC2)
#define _HAL_MCF51_I2C_IICSMB_REG                   (IICSMB)
#define _HAL_MCF51_I2C_PTCD_REG                     (PTCD)

#define _HAL_MCF51_I2C_ENABLE_MASK                  (IICC1_IICEN_MASK)
#define _HAL_MCF51_I2C_TX_CONTROL_MASK              (IICC1_TX_MASK)
#define _HAL_MCF51_I2C_STARTSTOP_MASK               (IICC1_MST_MASK)
#define _HAL_MCF51_I2C_RESTART_MASK                 (IICC1_RSTA_MASK)
#define _HAL_MCF51_I2C_TXAK_EN_MASK                 (IICC1_TXAK_MASK)
#define _HAL_MCF51_I2C_DMA_EN_MASK                  (IICC1_DMAEN_MASK)
#define _HAL_MCF51_I2C_INT_MASK                     (IICC1_IICIE_MASK)
#define _HAL_MCF51_I2C_BUSBUSY_MASK                 (IICS_BUSY_MASK)
#define _HAL_MCF51_I2C_AL_MASK                      (IICS_ARBL_MASK)
#define _HAL_MCF51_I2C_ADDRMATCH_MASK               (IICS_IAAS_MASK)
#define _HAL_MCF51_I2C_SLAVE_RW_MASK                (IICS_SRW_MASK)
#define _HAL_MCF51_I2C_RXACKBITVAL_MASK             (IICS_RXAK_MASK)
#define _HAL_MCF51_I2C_TCF_MASK                     (IICS_TCF_MASK)
#define _HAL_MCF51_I2C_TXACKBITVAL_MASK             (IICC1_TXAK_MASK)
#define _HAL_MCF51_I2C_INT_REQ_FLAG_MASK            (IICS_IICIF_MASK)
#define _HAL_MCF51_I2C_INT_REQ_MASK                 (IICS_IICIF_MASK)
#define _HAL_MCF51_I2C_MULT_MASK                    (IICF_MULT_MASK)
#define _HAL_MCF51_I2C_ICR_MASK                     (IICF_ICR_MASK)
#define _HAL_MCF51_I2C_SDA_IO_MASK                  (PTCD_PTCD1_MASK)
#define _HAL_MCF51_I2C_SCL_IO_MASK                  (PTCD_PTCD0_MASK)
#define _HAL_MCF51_I2C_GCALL_ADDRMATCH_MASK         (IICC2_GCAEN_MASK)



/*******   DMA   ************************************/

#define _HAL_MCF51_DMA_CHANNEL_COUNT                (4)
#define _HAL_MCF51_DMA_REQ_CTRL_REG                 (DMAREQC)

#define _HAL_MCF51_DMA_REQ_CTRL_C0_MASK             (DMAREQC_DMAC0_MASK)
#define _HAL_MCF51_DMA_REQ_CTRL_C0_SHIFT            (DMAREQC_DMAC0_BITNUM)
#define _HAL_MCF51_DMA_REQ_CTRL_C1_MASK             (DMAREQC_DMAC1_MASK)
#define _HAL_MCF51_DMA_REQ_CTRL_C1_SHIFT            (DMAREQC_DMAC1_BITNUM)
#define _HAL_MCF51_DMA_REQ_CTRL_C2_MASK             (DMAREQC_DMAC2_MASK)
#define _HAL_MCF51_DMA_REQ_CTRL_C2_SHIFT            (DMAREQC_DMAC2_BITNUM)
#define _HAL_MCF51_DMA_REQ_CTRL_C3_MASK             (DMAREQC_DMAC3_MASK)
#define _HAL_MCF51_DMA_REQ_CTRL_C3_SHIFT            (DMAREQC_DMAC3_BITNUM)

#define _HAL_MCF51_DMA_SAR_C0_REG              		  (SAR0)
#define _HAL_MCF51_DMA_SAR_C1_REG              		  (SAR1)
#define _HAL_MCF51_DMA_SAR_C2_REG              		  (SAR2)
#define _HAL_MCF51_DMA_SAR_C3_REG              		  (SAR3)

#define _HAL_MCF51_DMA_DAR_C0_REG              		  (DAR0)
#define _HAL_MCF51_DMA_DAR_C1_REG              		  (DAR1)
#define _HAL_MCF51_DMA_DAR_C2_REG              		  (DAR2)
#define _HAL_MCF51_DMA_DAR_C3_REG              		  (DAR3)

#define _HAL_MCF51_DMA_BCR_C0_REG                   (BCR0)
#define _HAL_MCF51_DMA_BCR_C1_REG                   (BCR1)
#define _HAL_MCF51_DMA_BCR_C2_REG                   (BCR2)
#define _HAL_MCF51_DMA_BCR_C3_REG                   (BCR3)

#define _HAL_MCF51_DMA_BCR_BCR_MASK                 (BCR0_BCR_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_MASK                 (BCR0_DSR_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_SHIFT                (BCR0_DSR_BITNUM)
#define _HAL_MCF51_DMA_BCR_DSR_DONE_MASK            (DSR0_DONE_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_BSY_MASK             (DSR0_BSY_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_REQ_MASK             (DSR0_REQ_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_BED_MASK             (DSR0_BED_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_BES_MASK             (DSR0_BES_MASK)
#define _HAL_MCF51_DMA_BCR_DSR_CE_MASK              (DSR0_CE_MASK)

#define _HAL_MCF51_DMA_DCR_C0_REG                   (DCR0)
#define _HAL_MCF51_DMA_DCR_C1_REG                   (DCR1)
#define _HAL_MCF51_DMA_DCR_C2_REG                   (DCR2)
#define _HAL_MCF51_DMA_DCR_C3_REG                   (DCR3)

#define _HAL_MCF51_DMA_DCR_EINT_MASK                (DCR0_EINT_MASK)
#define _HAL_MCF51_DMA_DCR_ERQ_MASK                 (DCR0_ERQ_MASK)
#define _HAL_MCF51_DMA_DCR_CS_MASK                  (DCR0_CS_MASK)
#define _HAL_MCF51_DMA_DCR_AA_MASK                  (DCR0_AA_MASK)
#define _HAL_MCF51_DMA_DCR_SINC_MASK                (DCR0_SINC_MASK)
#define _HAL_MCF51_DMA_DCR_SSIZE_MASK               (DCR0_SSIZE_MASK)
#define _HAL_MCF51_DMA_DCR_SSIZE_SHIFT              (DCR0_SSIZE_BITNUM)
#define _HAL_MCF51_DMA_DCR_DINC_MASK                (DCR0_DINC_MASK)
#define _HAL_MCF51_DMA_DCR_DSIZE_MASK               (DCR0_DSIZE_MASK)
#define _HAL_MCF51_DMA_DCR_DSIZE_SHIFT              (DCR0_DSIZE_BITNUM)
#define _HAL_MCF51_DMA_DCR_START_MASK               (DCR0_START_MASK)
#define _HAL_MCF51_DMA_DCR_SMOD_MASK                (DCR0_SMOD_MASK)
#define _HAL_MCF51_DMA_DCR_SMOD_SHIFT               (DCR0_SMOD_BITNUM)
#define _HAL_MCF51_DMA_DCR_DMOD_MASK                (DCR0_DMOD_MASK)
#define _HAL_MCF51_DMA_DCR_DMOD_SHIFT               (DCR0_DMOD_BITNUM)
#define _HAL_MCF51_DMA_DCR_D_REQ_MASK               (DCR0_D_REQ_MASK)
#define _HAL_MCF51_DMA_DCR_LINKCC_MASK              (DCR0_LINKCC_MASK)
#define _HAL_MCF51_DMA_DCR_LINKCC_SHIFT             (DCR0_LINKCC_BITNUM)
#define _HAL_MCF51_DMA_DCR_LCH1_MASK                (DCR0_LCH1_MASK)
#define _HAL_MCF51_DMA_DCR_LCH1_SHIFT               (DCR0_LCH1_BITNUM)
#define _HAL_MCF51_DMA_DCR_LCH2_MASK                (DCR0_LCH2_MASK)
#define _HAL_MCF51_DMA_DCR_LCH2_SHIFT               (DCR0_LCH2_BITNUM)

#if 0
/*******     INSERITO SPISW   ************************************/

#define _HAL_S08_SPIHW2_EN_REG                      (SPI2C1_SPE)
#define _HAL_S08_SCI1_EN_TX_REG                     (SCI1C2_TE)
#define _HAL_S08_SCI1_EN_RX_REG                     (SCI1C2_RE)
#define _HAL_S08_KBI1_PE5_REG                       (KBI1PE_KBIPE5)
#define _HAL_S08_KBI1_PE4_REG                       (KBI1PE_KBIPE4)
#define _HAL_S08_KBI1_PE0_REG                       (KBI1PE_KBIPE0)
#define _HAL_S08_KBI2_PE2_REG                       (KBI2PE_KBIPE2)
#define _HAL_S08_ADP_PC5_REG                        (APCTL1_ADPC5)
#define _HAL_S08_ADP_PC4_REG                        (APCTL1_ADPC4)
#define _HAL_S08_ADP_PC10_REG                       (APCTL2_ADPC10)


#define _HAL_S08_PTB0_GPIO_DIR_REG                  (PTBDD_PTBDD0)
#define _HAL_S08_PTB1_GPIO_DIR_REG                  (PTBDD_PTBDD1)
#define _HAL_S08_PTD0_GPIO_DIR_REG                  (PTDDD_PTDDD0)
#define _HAL_S08_PTD2_GPIO_DIR_REG                  (PTDDD_PTDDD2)
#define _HAL_S08_PTF0_GPIO_DIR_REG                  (PTFDD_PTFDD0)

#define _HAL_S08_PTB0_GPIO_DATA_REG                 (PTBD_PTBD0)
#define _HAL_S08_PTB1_GPIO_DATA_REG                 (PTBD_PTBD1)
#define _HAL_S08_PTD0_GPIO_DATA_REG                 (PTDD_PTDD0)
#define _HAL_S08_PTD2_GPIO_DATA_REG                 (PTDD_PTDD2)
#define _HAL_S08_PTF0_GPIO_DATA_REG                 (PTFD_PTFD0)


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

/** @name I2C
 *
 */
/*@{*/

/**
Provides clock to the peripheral
*/
#define     I2C_ClkEnable()        { SCGC1_IIC = 1; }

/**
Do not provide clock to the peripheral
*/
#define     I2C_ClkDisable()       { SCGC1_IIC = 0;}


/**
Enable the peripheral
*/
#define     I2C_Enable() { _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_ENABLE_MASK; }

/**
Disable the peripheral
*/
#define     I2C_Disable()       { _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_ENABLE_MASK; }

/**
Reset ctrl
*/
#define     I2C_CtrlReset() 	  { _HAL_MCF51_I2C_CTRL1_REG = 0; _HAL_MCF51_I2C_CTRL2_REG = 0; }

/**
Reset interrupt
*/
#define     I2C_IntRegReset() 	{  }


/**
Reset lines SDA e SCL
*/
#define     I2C_Reset()   {   }
/*@}*/

/** @name DMA
 *
 */
/*@{*/
#define     I2C_EnableDMA() { _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_DMA_EN_MASK; }

#define     I2C_DisableDMA() { _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_DMA_EN_MASK; }

#define     I2C_IsEnabledDMA() (_HAL_MCF51_I2C_CTRL1_REG & _HAL_MCF51_I2C_DMA_EN_MASK)


/** @name Data registers
 *
 */
/*@{*/
#define     I2C_ReadData() _HAL_MCF51_I2C_RX_DATA_REG

#define     I2C_WriteData(b) { _HAL_MCF51_I2C_TX_DATA_REG = (I2C_DataRegType)(b); }
#define     I2C_SendAddr(b)   I2C_WriteData(b)

/*I2C_DataRegType* */
#define     I2C_pReceiveDataRegister() ((uint8_t *)&_HAL_MCF51_I2C_RX_DATA_REG)

#define     I2C_pTransmitDataRegister() ((uint8_t *)&_HAL_MCF51_I2C_TX_DATA_REG)
/*@}*/


/** @name Configuration
 *
 */
/*@{*/
#define     I2C_SetClockRate(b) { _HAL_MCF51_I2C_IICF_REG= (b);}
/**/
#define     I2C_ResetClockRate(b)    I2C_SetClockRate(b)


#define     I2C_SetSlaveAddress(b) { _HAL_MCF51_I2C_SAR_REG = (I2C_SlaveAddressType)(b); }

#define     I2C_MasterEnable()  { /* do nothing: n.a. */ }


#define     I2C_MasterDisable() { /* do nothing: n.a. */ }


#define     I2C_SetTxMode()    { _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_TX_CONTROL_MASK; }

#define     I2C_SetRxMode()   { _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_TX_CONTROL_MASK; }

#define     I2C_IsTxMode()     ( (_HAL_MCF51_I2C_CTRL1_REG & _HAL_MCF51_I2C_TX_CONTROL_MASK) != 0x00)
/*@}*/


/** @name Start & Stop condition
 *
 */
/*@{*/
#define     I2C_IssueStartCondition() {    _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_STARTSTOP_MASK; }

#define     I2C_IssueReStartCondition() {   _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_RESTART_MASK; }

#define     I2C_IssueStopCondition()  {   _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_STARTSTOP_MASK; }

/*@}*/


/** @name Interrupt
 *
 */
/*@{*/
#define     I2C_IntEnable() { _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_INT_MASK; }

#define     I2C_IntDisable()  { _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_INT_MASK; }
/*@}*/


/** @name Status
 *
 */
/*@{*/
#define     I2C_IsBusBusy() ( (_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_BUSBUSY_MASK) != 0x00)

#define     I2C_IsSdaIoHigh() ( (_HAL_MCF51_I2C_PTCD_REG & _HAL_MCF51_I2C_SDA_IO_MASK) != 0x00)

#define     I2C_IsSclIoHigh() ( (_HAL_MCF51_I2C_PTCD_REG & _HAL_MCF51_I2C_SCL_IO_MASK) != 0x00 )

#define     I2C_GetStatus() _HAL_MCF51_I2C_STATUS_REG

#define     I2C_ResetStatus() 	{(void)I2C_GetStatus(); /*_HAL_300H_I2C_STATUS_REG=0;*/ }

#define     I2C_ResetIntReqFlag() { _HAL_MCF51_I2C_STATUS_REG |= _HAL_MCF51_I2C_INT_REQ_FLAG_MASK; }

#define     I2C_IsArbitrationLost() ( (_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_AL_MASK) != 0x00)

#define     I2C_IsSlaveAddressed()  ( (_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_ADDRMATCH_MASK) != 0x00)

#define     I2C_IsGeneralCallAddress() ( (_HAL_MCF51_I2C_CTRL2_REG & _HAL_MCF51_I2C_GCALL_ADDRMATCH_MASK) != 0x00)

#define     I2C_ResetSlaveAddressedSig() { _HAL_MCF51_I2C_STATUS_REG |= _HAL_MCF51_I2C_ADDRMATCH_MASK; }

#define     I2C_ResetGeneralCallAddressSig() { _HAL_MCF51_I2C_CTRL2_REG &= ~_HAL_MCF51_I2C_GCALL_ADDRMATCH_MASK; }

#define     I2C_ResetArbitrationLost() {  _HAL_MCF51_I2C_STATUS_REG |= _HAL_MCF51_I2C_AL_MASK; }

#define     I2C_IsSlaveTrasmitRequest() ( (_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_SLAVE_RW_MASK) != 0)

#define     I2C_IsAcknowledgeEnabled() ( (_HAL_MCF51_I2C_CTRL1_REG & _HAL_MCF51_I2C_TXAK_EN_MASK) == 0)

#define     I2C_IsMaster() ( (_HAL_MCF51_I2C_CTRL1_REG & _HAL_MCF51_I2C_STARTSTOP_MASK) != 0)

#define     I2C_ResetTxSig() {}

#define     I2C_IsTrasmitCompleted() ( (_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_TCF_MASK) != 0)

/*@}*/



/** @name Acknolegde bit
 *
 */
/*@{*/
#define     I2C_ReceivedAcknowledgeVal()   ((_HAL_MCF51_I2C_STATUS_REG & _HAL_MCF51_I2C_RXACKBITVAL_MASK)!=0)

#define     I2C_EnableAcknowledge() { _HAL_MCF51_I2C_CTRL1_REG &= ~_HAL_MCF51_I2C_TXACKBITVAL_MASK; }

#define     I2C_DisableAcknowledge() { _HAL_MCF51_I2C_CTRL1_REG |= _HAL_MCF51_I2C_TXACKBITVAL_MASK; }
/*@}*/
#define 		I2C_SlaveStartRx()
#define 		I2C_SlaveStartTx() 									I2C_IntEnable()





#define I2C_OnEnterIsr()                        I2C_ResetIntReqFlag()
#define I2C_OnIntAck()
#define I2C_DummyRead()                         (void)I2C_ReadData();

/* Slave receives unexpected data */
#define I2C_OnSlaveRxDummy()                    I2C_DummyRead();

#define I2C_OnSlaveAddressMatchTransmit()\
			        I2C_ResetSlaveAddressedSig();\
             		I2C_SetTxMode();\
    		        I2C_DisableAcknowledge();
    		
#define I2C_SlaveAddressedTx_Action()


/* This section contains HAL class extension */


#define I2C_NestedIsr_Begin()
#define I2C_NestedIsr_End()


#define I2C_Level3BlockMonitorSync()				((_HAL_MCF51_I2C_CTRL1_REG & (IICC1_TX_MASK|IICC1_TXAK_MASK)) == IICC1_TX_MASK)
#define I2C_Level4BlockMonitorSync()				((_HAL_MCF51_I2C_CTRL1_REG & (IICC1_TX_MASK|IICC1_TXAK_MASK|IICC1_MST_MASK)) == IICC1_TXAK_MASK)
#define I2C_Level5BlockMonitorSync()				((_HAL_MCF51_I2C_CTRL1_REG & (IICC1_TX_MASK|IICC1_TXAK_MASK|IICC1_MST_MASK)) || I2C_IsEnabledDMA())
/*@}*/
/* ********************* */
/* I2C Class HAL Support */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */


/* ****************** */
/* Exported variables */
/* ****************** */

/* ******************* */
/* Function prototypes */
/* ******************* */



/** @name DMA
 *
 */
/*@{*/

/**
Device assignment
*/
#define     DMA_SetDevice(channel, device) { *((uint8_t*)&_HAL_MCF51_DMA_REQ_CTRL_REG + (uint8_t)channel) = device; }
#define     DMA_GetDevice(channel) (*((uint8_t*)&_HAL_MCF51_DMA_REQ_CTRL_REG + (uint8_t)channel))

/**
DMA Source address assignment
*/
#define     DMA_SetSAR(channel, address) { *(&_HAL_MCF51_DMA_SAR_C0_REG + (uint32_t)(channel << 2)) = address; }
#define     DMA_GetSAR(channel) (*(&_HAL_MCF51_DMA_SAR_C0_REG + (uint32_t)(channel << 2)))


/**
DMA Destination address assignment
*/
#define     DMA_SetDAR(channel, address) { *(&_HAL_MCF51_DMA_DAR_C0_REG + (uint32_t)(channel << 2)) = address; }
#define     DMA_GetDAR(channel) (*(&_HAL_MCF51_DMA_DAR_C0_REG + (uint32_t)(channel << 2)))

/**
DMA Byte count register
*/
#define     DMA_SetBCR(channel, count) { *(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) = ((uint32_t)count & _HAL_MCF51_DMA_BCR_BCR_MASK) ; }
#define     DMA_GetBCR(channel) (*(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2))  & _HAL_MCF51_DMA_BCR_BCR_MASK)


/**
DMA status register check
*/

#define     DMA_IsTransDone(channel) ( (*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_DONE_MASK) != 0x00)

#define     DMA_IsChannelBusy(channel) ( (*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_BSY_MASK) != 0x00)

#define     DMA_IsPending(channel) ((*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_REQ_MASK) != 0x00)

#define     DMA_IsDestinationError(channel) ((*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_BED_MASK) != 0x00)

#define     DMA_IsSourceError(channel) ((*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_BES_MASK) != 0x00)

#define     DMA_IsConfigError(channel) ((*(uint8_t*)(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)) & DSR0_CE_MASK) != 0x00)

/**
DMA status register clear flags
*/
#define     DMA_ClearTransDone(channel) { *((uint8_t*)&(*(&_HAL_MCF51_DMA_BCR_C0_REG + (uint32_t)(channel << 2)))) = (uint8_t)DSR0_DONE_MASK;}

/**
DMA control register setting
*/

#define     DMA_EnableInt(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_EINT_MASK; }
#define     DMA_DisableInt(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_EINT_MASK; }

#define     DMA_PeripheralRequestOn(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_ERQ_MASK; }
#define     DMA_PeripheralRequestOff(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_ERQ_MASK; }

#define     DMA_ContinuousTransfer(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_CS_MASK; }
#define     DMA_SingleTransfer(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_CS_MASK; }

#define     DMA_EnableAutoAlign(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_AA_MASK; }
#define     DMA_DisableAutoAlign(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_AA_MASK; }

#define     DMA_EnableSourceIncrement(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_SINC_MASK; }
#define     DMA_DisableSourceIncrement(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_SINC_MASK; }

#define     DMA_SetSourceSizeToLong(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_SSIZE_MASK) | (((uint32_t)0 << _HAL_MCF51_DMA_DCR_SSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_SSIZE_MASK); }
#define     DMA_SetSourceSizeToByte(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_SSIZE_MASK) | (((uint32_t)1 << _HAL_MCF51_DMA_DCR_SSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_SSIZE_MASK); }
#define     DMA_SetSourceSizeToWord(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_SSIZE_MASK) | (((uint32_t)2 << _HAL_MCF51_DMA_DCR_SSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_SSIZE_MASK); }

#define     DMA_EnableDestinationIncrement(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_DINC_MASK; }
#define     DMA_DisableDestinationIncrement(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_DINC_MASK; }

#define     DMA_SetDestinationSizeToLong(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_DSIZE_MASK) | (((uint32_t)0 << _HAL_MCF51_DMA_DCR_DSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_DSIZE_MASK); }
#define     DMA_SetDestinationSizeToByte(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_DSIZE_MASK) | (((uint32_t)1 << _HAL_MCF51_DMA_DCR_DSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_DSIZE_MASK); }
#define     DMA_SetDestinationSizeToWord(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_DSIZE_MASK) | (((uint32_t)2 << _HAL_MCF51_DMA_DCR_DSIZE_SHIFT) & _HAL_MCF51_DMA_DCR_DSIZE_MASK); }

#define     DMA_SetStartOn(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_START_MASK; }
#define     DMA_SetStartOff(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_START_MASK; }

#define     DMA_SetSourceAddressModulo(channel, modulo) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_SMOD_MASK) | (((uint32_t)modulo << _HAL_MCF51_DMA_DCR_SMOD_SHIFT) & _HAL_MCF51_DMA_DCR_SMOD_MASK); }
#define     DMA_SetDestinationAddressModulo(channel, modulo) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_DMOD_MASK) | (((uint32_t)modulo << _HAL_MCF51_DMA_DCR_DMOD_SHIFT) & _HAL_MCF51_DMA_DCR_DMOD_MASK); }

#define     DMA_SetDisableRequestOn(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) |= _HAL_MCF51_DMA_DCR_D_REQ_MASK; }
#define     DMA_SetDisableRequestOff(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) &= ~_HAL_MCF51_DMA_DCR_D_REQ_MASK; }

#define     DMA_LinkChannelControlMode(channel, mode) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_LINKCC_MASK) | (((uint32_t)mode << _HAL_MCF51_DMA_DCR_LINKCC_SHIFT) & _HAL_MCF51_DMA_DCR_LINKCC_MASK); }

#define     DMA_LinkChannel1Mode(channel, mode) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_LCH1_MASK) | (((uint32_t)mode << _HAL_MCF51_DMA_DCR_LCH1_SHIFT) & _HAL_MCF51_DMA_DCR_LCH1_MASK); }

#define     DMA_LinkChannel2Mode(channel, mode) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = (*(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) & ~_HAL_MCF51_DMA_DCR_LCH2_MASK) | (((uint32_t)mode << _HAL_MCF51_DMA_DCR_LCH2_SHIFT) & _HAL_MCF51_DMA_DCR_LCH2_MASK); }

#define     DMA_ControlDefault(channel) { *(&_HAL_MCF51_DMA_DCR_C0_REG + (uint32_t)(channel << 2)) = 0;}



/* This section contains HAL class extension */


/*@}*/
/* ********************* */
/* DMA Class HAL Support */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */

/* ********************************************************************************************** */
/*                                  BEGIN WDT Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Watchdog
 *
 */
/*@{*/

/* Define WatchDog Capability */
#define _HAL_WDT_CAPABILITY
/* Preprocessor Errors */
#if (WDT_TIMEOUT > 4294967295)
#error "WDT_TIMEOUT must be less than 2^32"
#endif
#if ((WDT_WINDOWED_MODE == 1) & (WDT_TIMEOUT <= (WDT_WINDOW+1)))
#error "WDT_WINDOW must be less than than (WDT_TIMEOUT-1)!"
#endif
#if (WDT_TIMEOUT < 4)
#error "The time-out value of the watchdog must be set to a minimum of four watchdog clock cycles."
#endif

/* Watchdog Control and Status Register */
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_REG                     (WDOG_ST_CTRL)

#define _HAL_MCF51_WDT_WDOG_ST_CTRL_INT_FLG_MASK            (WDOG_ST_CTRL_INT_FLG_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_WDOG_EN_MASK            (WDOG_ST_CTRL_WDOG_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_CLK_SRC_MASK            (WDOG_ST_CTRL_CLK_SRC_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_IRQ_RST_EN_MASK         (WDOG_ST_CTRL_IRQ_RST_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_WIN_EN_MASK             (WDOG_ST_CTRL_WIN_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_ALLOW_UPDATE_MASK       (WDOG_ST_CTRL_ALLOW_UPDATE_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_DBG_EN_MASK             (WDOG_ST_CTRL_DBG_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_STOP_EN_MASK            (WDOG_ST_CTRL_STOP_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_WAIT_EN_MASK            (WDOG_ST_CTRL_WAIT_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_TESTWDOG_MASK           (WDOG_ST_CTRL_TESTWDOG_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_TESTSEL_MASK            (WDOG_ST_CTRL_TESTSEL_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_BYTESEL_MASK            (WDOG_ST_CTRL_BYTESEL_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_BYTESEL_SHIFT           (WDOG_ST_CTRL_BYTESEL_BITNUM)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_DISABLE_TESTWDOG_MASK   (WDOG_ST_CTRL_DISABLE_TESTWDOG_MASK)

/* Watchdog Control and Status Register - high; used 16-bit access to save program memory */
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG                   (WDOG_ST_CTRL_H)

#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WDOG_EN_MASK          (WDOG_ST_CTRL_H_WDOG_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_CLK_SRC_MASK          (WDOG_ST_CTRL_H_CLK_SRC_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_IRQ_RST_EN_MASK       (WDOG_ST_CTRL_H_IRQ_RST_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WIN_EN_MASK           (WDOG_ST_CTRL_H_WIN_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK     (WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_DBG_EN_MASK           (WDOG_ST_CTRL_H_DBG_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_STOP_EN_MASK          (WDOG_ST_CTRL_H_STOP_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WAIT_EN_MASK          (WDOG_ST_CTRL_H_WAIT_EN_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTWDOG_MASK         (WDOG_ST_CTRL_H_TESTWDOG_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTSEL_MASK          (WDOG_ST_CTRL_H_TESTSEL_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_BYTESEL_MASK          (WDOG_ST_CTRL_H_BYTESEL_MASK)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_BYTESEL_SHIFT         (WDOG_ST_CTRL_H_BYTESEL_BITNUM)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_H_DISABLE_TESTWDOG_MASK (WDOG_ST_CTRL_H_DISABLE_TESTWDOG_MASK)

/* Watchdog Control and Status Register - low; used 16-bit access to save program memory */
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_L_REG                   (WDOG_ST_CTRL_L)
#define _HAL_MCF51_WDT_WDOG_ST_CTRL_L_INT_FLG_MASK          (WDOG_ST_CTRL_L_INT_FLG_MASK)


/* Watchdog Time-out Value Register */
#define _HAL_MCF51_WDT_WDOG_TO_VAL_REG              (WDOG_TO_VAL)

/* Watchdog Window Register */
#define _HAL_MCF51_WDT_WDOG_WIN_REG                 (WDOG_WIN)

/* Watchdog Refresh Register */
#define _HAL_MCF51_WDT_WDOG_REFRESH_REG             (WDOG_REFRESH)

/* Watchdog Unlock Register */
#define _HAL_MCF51_WDT_WDOG_UNLOCK_REG              (WDOG_UNLOCK)

/* Watchdog Timer Output Register */
#define _HAL_MCF51_WDT_WDOG_TIMER_OUT_REG           (WDOG_TIMER_OUT)

/* Watchdog Reset and Count Register */
#define _HAL_MCF51_WDT_WDOG_RST_CNT_REG             (WDOG_RST_CNT)


#define WDT_EnableTest(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_DISABLE_TESTWDOG_MASK;}
#define WDT_DisableTest(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_DISABLE_TESTWDOG_MASK;}

#define WDT_SetTestByte(setting, n) {setting |= (setting & ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_BYTESEL_MASK) | (((uint16_t)n << _HAL_MCF51_WDT_WDOG_ST_CTRL_H_BYTESEL_SHIFT) & _HAL_MCF51_WDT_WDOG_ST_CTRL_H_BYTESEL_MASK);}

#define WDT_SetQuickTest(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTSEL_MASK;}
#define WDT_SetByteTest(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTSEL_MASK;}

#define WDT_StartTest(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTWDOG_MASK;}
#define WDT_IsTest() ((_HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG & _HAL_MCF51_WDT_WDOG_ST_CTRL_H_TESTWDOG_MASK) != 0)

#define WDT_EnableInWait(setting) {setting |= WDOG_ST_CTRL_H_WAIT_EN_MASK;}
#define WDT_DisableInWait(setting) {setting &= ~WDOG_ST_CTRL_H_WAIT_EN_MASK;}

#define WDT_EnableInStop(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_STOP_EN_MASK;}
#define WDT_DisableInStop(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_STOP_EN_MASK;}

#define WDT_EnableInDebug(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_DBG_EN_MASK;}
#define WDT_DisableInDebug(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_DBG_EN_MASK;}

#define WDT_EnableUpdate(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK;}
#define WDT_DisableUpdate(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK;}

#define WDT_EnableWindowing(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WIN_EN_MASK;}
#define WDT_DisableWindowing(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_WIN_EN_MASK;}

#define WDT_EnableIRQ(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_IRQ_RST_EN_MASK;}
#define WDT_DisableIRQ(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_IRQ_RST_EN_MASK;}

#define WDT_SetLPOClock(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_CLK_SRC_MASK;}
#define WDT_SetAlternateClock(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_CLK_SRC_MASK;}

#define WDT_Enable(setting) {setting |= _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WDOG_EN_MASK;}
#define WDT_Disable(setting) {setting &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_H_WDOG_EN_MASK;}

#define WDT_WriteControl(setting) {_HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG = setting;}
#define WDT_ReadControl() (_HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG)

#define WDT_GetIntFlag() ((_HAL_MCF51_WDT_WDOG_ST_CTRL_L_REG & _HAL_MCF51_WDT_WDOG_ST_CTRL_L_INT_FLG_MASK) != 0)
#define WDT_ClearIntFlag() {_HAL_MCF51_WDT_WDOG_ST_CTRL_L_REG &= ~_HAL_MCF51_WDT_WDOG_ST_CTRL_L_INT_FLG_MASK;}

#define WDT_SetTimeOut(time) {_HAL_MCF51_WDT_WDOG_TO_VAL_REG = (uint32_t)time;}

#define WDT_SetWindow(time) {_HAL_MCF51_WDT_WDOG_WIN_REG = (uint32_t)time;}
#define WDT_Window()        _HAL_MCF51_WDT_WDOG_WIN_REG
#define WDT_AllowUpdate()   (_HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG & _HAL_MCF51_WDT_WDOG_ST_CTRL_H_ALLOW_UPDATE_MASK)


/* The required refresh within 20 bus clock cycles.
You must take care not only to refresh the watchdog within the watchdog timer's actual time-out
period, but also provide enough allowance for the time it takes for the refresh sequence to be
detected by the watchdog timer, on the watchdog clock
(2/3 watchdog cycles for window mode, 1 watchdog cycle for no window mode)
*/
#define WDT_Refresh()\
{\
EnterCriticalSection();\
_HAL_MCF51_WDT_WDOG_REFRESH_REG = 0xA602;\
_HAL_MCF51_WDT_WDOG_REFRESH_REG = 0xB480;\
ExitCriticalSection();\
}
/* The required unlock within 20  bus clock cycles.
Updates cannot be made in the bus clock cycle immediately following the write of
the unlock sequence, but one bus clock cycle later
*/
#define WDT_Unlock()\
{\
_HAL_MCF51_WDT_WDOG_UNLOCK_REG = (uint16_t)0xC520;\
_HAL_MCF51_WDT_WDOG_UNLOCK_REG = (uint16_t)0xD928;\
asm(nop);\
}

#define WDT_Counter() WDOG_TIMER_OUT

#define WDT_RSTCounter() _HAL_MCF51_WDT_WDOG_RST_CNT_REG

#define WDT_ClearRSTCounter() {_HAL_MCF51_WDT_WDOG_RST_CNT_REG = (uint16_t)0xFFFF;}

#define WDT_ResetMCU()\
{\
_HAL_MCF51_WDT_WDOG_UNLOCK_REG = 0xd928;\
_HAL_MCF51_WDT_WDOG_UNLOCK_REG = 0xc520;\
}
#define UserReset() WDT_ResetMCU()

#define WDT_Enabled()    (_HAL_MCF51_WDT_WDOG_ST_CTRL_H_REG & _HAL_MCF51_WDT_WDOG_ST_CTRL_H_WDOG_EN_MASK)

#define WDT_IsWindowOpen()    (WDT_Counter() >= WDT_Window()? 1 : 0)

#ifdef HAL_WDT_SUPPORT

/** This service must be called with interrupts disabled and before WCT window elapsed
otherwise you may have reset */
inline void WDT_InitSync(void)
{
	/* Control register in memory because it can only write once to the register after unlock */
	uint16_t setting = 0;

	/* Enable/Disable watchdog $ */
#if (WDT_ENABLED)
	WDT_Enable(setting);
#else
	WDT_Disable(setting);
#endif

	/* Allow/Disallow reconfigurate watchdog $ */
#if (WDT_UPDATE_AFTER_WRITE)
	WDT_EnableUpdate(setting);
#else
	WDT_DisableUpdate(setting);
#endif

	/* Enable/Disable windowing $ */
#if (WDT_WINDOWED_MODE)
	WDT_EnableWindowing(setting);
#else
	WDT_DisableWindowing(setting);
#endif

	/* Enable/Disable interrupt before reset $ */
#if (WDT_INT_ENABLED)
	WDT_EnableIRQ(setting);
#else
	WDT_DisableIRQ(setting);
#endif

	/* Sets the clock source $ */
#if (WDT_ALTERNATE_CLOCK)
	WDT_SetAlternateClock(setting);
#else
	WDT_SetLPOClock(setting);
#endif

	/* Enable/Disable watchdog in stop mode $ */
#if (WDT_ENABLED_IN_STOP)
	WDT_EnableInStop(setting);
#else
	WDT_DisableInStop(setting);
#endif

	/* Enable/Disable watchdog in debug mode $ */
#if (WDT_ENABLED_IN_DEBUG)
	WDT_EnableInDebug(setting);
#else
	WDT_DisableInDebug(setting);
#endif

	/* Enable/Disable watchdog in wait mode $ */
#if (WDT_ENABLED_IN_WAIT)
	WDT_EnableInWait(setting);
#else
	WDT_DisableInWait(setting);
#endif

	/* Allows the WDOG's functional test mode to be disabled permanently until next reset. $ */
#if (WDT_DISABLE_TEST)
	WDT_DisableTest(setting);
#else
	WDT_EnableTest(setting);
#endif
	/* A context switch during unlocking may lead to a watchdog reset */

	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Sets the watchdog timeout period */
	WDT_SetTimeOut(WDT_TIMEOUT);
#if (WDT_WINDOWED_MODE)
	/* Sets the window period */
	WDT_SetWindow(WDT_WINDOW);
#endif
	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
}


/**
This sets the desired timeout.

\param  [in]	timeout  specifies the 32-bit timeout value.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil
			    \par \b Note 2:
			    \b Watchdog is not refreshed by this function.
			    \par \b Note 3:
			    - Stefano Ughi Feb 28, 2011 removed watchdog enable bit

*/
inline void WDT_SetSync(uint32_t timeout)
{
	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Sets the timeout */
	WDT_SetTimeOut(timeout);

	ExitCriticalSection();
}

/**
This function deactivates the watchdog. The function
influences the enable bit of the control register.
When the watchdog is disabled the watchdog timer is reset to zero
and is disabled from counting until you enable it or it is again enabled by
the system reset.

\author 	Jaroslav Musil
\date		Jan 19, 2010
\version	1.0.0

\note		\par \b Note 1:
			documentation by Jaroslav Musil, Stefano Ughi

*/
inline void WDT_StopSync(void)
{
	/* Control register in memory because it is write once */
	uint16_t setting;

	/* Read control register */
	setting = WDT_ReadControl();

	/* Disables the watchdog */
	WDT_Disable(setting);

	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
	ExitCriticalSection();
}
/**
This function activates the watchdog. The function
influences the enable bit of the control register.

\author 	Stefano Ughi
\date		Feb 28, 2011
\version	1.0.0

\note		\par \b Note 1:
			documentation by Stefano Ughi

*/
inline void WDT_StartSync(void)
{
	/* Control register in memory because it is write once */
	uint16_t setting;

	/* Read control register */
	setting = WDT_ReadControl();

	/* Enables the watchdog */
	WDT_Enable(setting);

	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
	ExitCriticalSection();
}

/**
This function sets the desired window.

\param  [in]	window  specifies the 32-bit window value.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil
			    \par \b Note 2:
			    \b Watchdog is not refreshed by this fuction.
			    \par \b Note 3:
			    - Feb 28,2011 Stefano Ughi
                  removed window enable bit

*/
inline void WDT_SetWindowingSync(WDT_windowType window)
{
	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Sets the timeout */
	WDT_SetWindow(window);

	ExitCriticalSection();
}

/**
This function deactivates the windowing feature of watchdog. The function
influences the windowing enable bit of the control register.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline void WDT_StopWindowingSync(void)
{
	/* Control register in memory because it is write once */
	uint16_t setting;

	/* Read control register */
	setting = WDT_ReadControl();

	/* Disables windowing of the watchdog */
	WDT_DisableWindowing(setting);
	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
	ExitCriticalSection();
}

/**
This function reads the watchdog counter register.

\return 	    Watchdog counter

\retval		    uint32_t

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline uint32_t WDT_ReadCounter(void)
{

	return WDT_Counter();
}
/**
This function returns the watchdog window register.

\return 	    Watchdog Window Register

\retval		    uint32_t

\author 	    Stefano Ughi
\date		    Feb 28, 2011
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Stefano Ughi

*/
inline uint32_t WDT_GetWindow(void)
{
	return WDT_Window();
}

/**
This function reads the watchdog reset counter register.

\return 	    Watchdog reset counter

\retval		    uint16_t

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline uint16_t WDT_ReadRSTCounter(void)
{

	return WDT_RSTCounter();
}

/**
This function resets the watchdog reset counter register.
The function influences the RST counter register.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline void WDT_ResetRSTCounter(void)
{
	WDT_ClearRSTCounter();
}

/**
This function switches the watchdog in the quick test mode and sets a desired time-out.
The function influences the test selection, test enable and enable bits of the control register
and the time-out register.

\param  [in]	timeout  specifies the 32-bit time-out value.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline void WDT_SetQuickTestSync(uint32_t timeout)
{
	/* Control register in memory because it is write once */
	uint16_t setting;

	/* Read control register */
	setting = WDT_ReadControl();

	/* Assigns the quick test */
	WDT_SetQuickTest(setting);

	/* Sets the test mode */
	WDT_StartTest(setting);

	/* Enables the WDT */
	WDT_Enable(setting);

	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Sets the timeout */
	WDT_SetTimeOut(timeout);

	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
	ExitCriticalSection();
}

/**
This function switches the watchdog in the byte test mode and sets a desired time-out and the byte to be tested.
The function influences the test selection, test enable, test byte and enable bits of the control register
and the time-out register.

\param  [in]	timeout  specifies the 32-bit time-out value.
\param  [in]	testByte  specifies the byte of the 4-byte watchdog counter to be tested; possible value 0 to 3.

\author 	    Jaroslav Musil
\date		    Jan 19, 2010
\version	    1.0.0

\note		    \par \b Note 1:
			    documentation by Jaroslav Musil

*/
inline void WDT_SetByteTestSync(uint32_t timeout, uint8_t testByte)
{
	/* Control register in memory because it is write once */
	uint16_t setting;

	/* Read control register */
	setting = WDT_ReadControl();

	/* Assigns the quick test */
	WDT_SetByteTest(setting);

	/* Specifies the byte to test */
	WDT_SetTestByte(setting, testByte);

	/* Sets the test mode */
	WDT_StartTest(setting);

	/* Enables the WDT */
	WDT_Enable(setting);

	EnterCriticalSection();
	/* Unlocks the watchdog, the next configuration must be written in 256 clocks */
	WDT_Unlock();

	/* Sets the timeout */
	WDT_SetTimeOut(timeout);

	/* Writes the configuration of watchdog */
	WDT_WriteControl(setting);
	ExitCriticalSection();
}

#endif /* HAL_WDT_SUPPORT */
/*@}*/

/* ********************************************************************************************** */
/*                                  END WDT Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN CRC Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Cyclic redundancy check
 *
 */
/*@{*/
#define CRC_HW_SUPPORTED
#define CRC_ClockGateOn()    (SCGC2|=SCGC2_CRC_MASK)
#define CRC_ClockGateOff()   (SCGC2&=(~SCGC2_CRC_MASK))

#define InitCRC(seed) {CRCH = (uint8_t)(seed >> 8); CRCL = (uint8_t)(seed);}
#define ComputeCRC(data) {CRCL = (uint8_t)data;}
#define GetFinalCRC() ((uint16_t)CRCH << 8 | (uint16_t)CRCL)
#define InitCRCmodule()\
{\
CRC_ClockGateOn();\
}
#define DeInitCRCmodule()\
{\
CRC_ClockGateOff();\
}
/*@}*/
/* ********************************************************************************************** */
/*                                  END CRC Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN FLS Class HAL Support                                   */
/* ********************************************************************************************** */

/** @name H.A.L. Flash
 *
 */
/*@{*/

#if defined(HAL_FLS_SUPPORT)
/* ****************** */
/* Defines and Macros */
/* ****************** */

/* Define Flash Capability */
#define _HAL_FLS_CAPABILITY
/* Define Commands for managing flash resident in RAM */
/* All flash's Pages have the same size */
#define ALL_FLASH_PAGES_HAVE_THE_SAME_SIZE
/* This is the size in bytes of a flash's page.*/
#define FLASH_PAGE_SIZE_IN_BYTES    1024

/* Operation was successful */
#define FLASH_SUCCESS           (0x00)

/* 63 is the max. avaible value for flash module clock divisor */
#if ((BUS_FREQUENCY_Hz / FLS_MODULE_FREQUENCY_Hz) > 63)
#define FLS_FLG_PRDIV8	1
#define FLS_DIV ((BUS_FREQUENCY_Hz / 8 / FLS_MODULE_FREQUENCY_Hz) - 1)
#if (FLS_DIV > 63)
#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor too high!"
#endif
#else
#define FLS_FLG_PRDIV8	0
#define FLS_DIV ((BUS_FREQUENCY_Hz / FLS_MODULE_FREQUENCY_Hz) - 1)
#if (FLS_DIV < 0)
#error "Check BUS_FREQUENCY_Hz and FLS_MODULE_FREQUENCY_Hz, divisor is too low!"
#endif
#endif

#define REAL_FLS_MODULE_FREQUENCY_Hz (BUS_FREQUENCY_Hz/(FLS_FLG_PRDIV8*(FLS_DIV+1)))

/* This is the  FLASH Clock Divider Register (FCDIV) */
#define _HAL_MCF51_FLS_FCDIV_REG					(FCDIV)

/** This is the bit mask of PRDIV8 bit */
#define _HAL_MCF51_FLS_FCDIV_PRDIV8_MASK	(FCDIV_PRDIV8_MASK)

/** This is the  FLASH Status Register (FSTAT) */
#define _HAL_MCF51_FLS_FSTAT_REG					    (FSTAT)

/** This is the bit mask of FCBEF bit */
#define _HAL_MCF51_FLS_FSTAT_FCBEF_MASK			  (FSTAT_FCBEF_MASK)

/** This is the bit mask of FCCF bit */
#define _HAL_MCF51_FLS_FSTAT_FCCF_MASK		    (FSTAT_FCCF_MASK)

/** This is the bit mask of FPVIOL bit */
#define _HAL_MCF51_FLS_FSTAT_FPVIOL_MASK 		  (FSTAT_FPVIOL_MASK)

/** This is the bit mask of FACCERR bit */
#define _HAL_MCF51_FLS_FSTAT_FACCERR_MASK		  (FSTAT_FACCERR_MASK)

#define _HAL_MCF51_FLS_FSTAT_FBLANK_MASK		    (FSTAT_FBLANK_MASK)

#define _HAL_MCF51_FLS_FSTAT_ERR_MASK				  (_HAL_MCF51_FLS_FSTAT_FPVIOL_MASK | _HAL_MCF51_FLS_FSTAT_FACCERR_MASK)

/** This is the  FLASH Command Register (FCMD) */
#define _HAL_MCF51_FLS_FCMD_REG					(FCMD)


/*
Initialization of the FLASH Module Clock.\n
Programming and erase operations are allowed.
*/
#define FLS_Init()\
{\
	_HAL_MCF51_FLS_FCDIV_REG = \
	((FLS_FLG_PRDIV8 * _HAL_MCF51_FLS_FCDIV_PRDIV8_MASK) | FLS_DIV);\
}
/*
Abort any command and clear errors
*/
#define FLS_InitRegsForErasing()\
{\
	_HAL_MCF51_FLS_FSTAT_REG |= _HAL_MCF51_FLS_FSTAT_ERR_MASK;\
}
/*
Abort any command and clear errors
*/
#define FLS_InitRegsForEraseVerifying()\
{\
	_HAL_MCF51_FLS_FSTAT_REG |= _HAL_MCF51_FLS_FSTAT_ERR_MASK;\
}
/*
Abort any command and clear errors
*/
#define FLS_InitRegsForProgramming()\
{\
	_HAL_MCF51_FLS_FSTAT_REG |= _HAL_MCF51_FLS_FSTAT_ERR_MASK;\
}

#define FLS_CheckErrors() ((_HAL_MCF51_FLS_FSTAT_REG & _HAL_MCF51_FLS_FSTAT_ERR_MASK) != 0)
#define FLS_NoErrors() ((_HAL_MCF51_FLS_FSTAT_REG & _HAL_MCF51_FLS_FSTAT_ERR_MASK) == 0)

#define FLS_CheckBlank() ((_HAL_MCF51_FLS_FSTAT_REG & _HAL_MCF51_FLS_FSTAT_FBLANK_MASK) != 0)


/**
\n Page Erase Routine
\n\li position  (d0) specify a location inside flash's block.
\n Stefano Ughi Feb 14 2011
*/

asm static void __declspec(register_abi) FLS_ErasePageCmdIsInRam(FLS_paramType position)
{
	/* Write a dummy value "0" at address "position" */
	movea.l  d0,a0
	clr.l    (a0)
	/* Write Erase Page Command */
	moveq    #0x40,d0
	move.b   d0,FCMD
	/* Launch the command (Set FCBEF) */
	mov3q    #7,d0
	bset     d0,FSTAT
	/* Test FCCF: Warning we must wait min. 4 bus cycle before testing FCCF */
	nop		/* 3 processor clock cycles */
WaitEraseCompl:
	mov3q    #6,d0	/* 1 processor clock cycle */
	btst     d0,FSTAT
	beq.s    WaitEraseCompl
	rts
}

/**
\n Write Routine, Burst Command, must be executed in RAM.
\n Flash memory on MCF51AG128 series MCUs must be programmed 32 bits at a time.
\n\li pSource   (pointer-a0) specify start point of the source data.
\n\li size      (d0) specify how many bytes must be written, this parameter must be != 0 and multiple of 4.
\n\li position  (d1) specify start point of destination.
\n Stefano Ughi Feb 14 2011
*/

asm static void __declspec(register_abi) FLS_BurstProgCmdIsInRam(FLS_SourcePtrType pSource, FLS_sizeType size, FLS_positionType position)
{
	move.l   d7,-(a7)
fcbef_is_low:
	mov3q #7,d2
	btst  d2,FSTAT
	beq.s fcbef_is_low
write_data:
	movea.l  d1,a1
	move.l   (a0),(a1)
	/* burst cmd. */
	moveq    #0x25,d2
	move.b   d2,FCMD
	/* set FCBEF */
	mov3q    #7,d2
	bset     d2,FSTAT
	/* check errors */
	mvz.b    FSTAT,d7
	moveq    #_HAL_MCF51_FLS_FSTAT_ERR_MASK,d2
	and.l    d7,d2
	tst.w    d2
	bne.s    exit
	/* inc. pSource */
	addq.l   #4,a0
	/* inc. position */
	addq.l   #4,d1
	/* dec. size */
	subq.l   #4,d0
	/* test size */
	bne.s    fcbef_is_low
	/* all data written --> test if FFCF is high */
FFCF_is_low:
	mov3q    #6,d0
	btst     d0,FSTAT
	beq.s    FFCF_is_low
exit:
	move.l   (a7)+,d7
	rts
}

/**
Execute Erase Command in R.A.M.
- Copy FLS_ErasePageCmdIsInRam in RAM
- Execute in RAM Block Erase Cmd
*/
uint8_t FLS_Erase(FLS_positionType position);

/**
Execute Erase Verify Command in R.A.M.
- Copy FLS_ErasePageCmdIsInRam in RAM
- Execute in RAM Block Erase Verify Cmd
*/
/*lint -emacro(545,FLS_Erase) */
/*lint -emacro(546,FLS_Erase) */
/*lint -emacro(420,FLS_Erase) */
/*lint -emacro(643,FLS_Erase) */
/*lint -emacro(611,FLS_Erase) */
/*lint -emacro(826,FLS_Erase) */
/*lint -emacro(740,FLS_Erase) */
#define FLS_EraseVerify(pPosition)\
{\
(void)memcpy(&flashCmdInRAM, &FLS_EraseVerifyPageCmdIsInRam, SIZE_CMD_ERASE_VERIFY_IN_RAM);\
((pFLS_EraseVerifyPageCmdIsInRam)&flashCmdInRAM)(pPosition);\
}


/**
Execute Burst Command in R.A.M.
- Copy FLS_BurstProgCmdIsInRam in RAM
- Execute in RAM Burst Program Cmd
*/
uint8_t FLS_Programming(uint8_t* pSource, uint32_t size, FLS_positionType address);


#endif /* #ifdef HAL_FLS_SUPPORT */
/*@}*/
/* ********************************************************************************************** */
/*                                  END FLS Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN DIO Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Digital Inputs Outputs
 *
 */
/*@{*/

#if defined(HAL_DIO_SUPPORT)

/** Define DIO Capability */
#define _HAL_DIO_CAPABILITY
#define PIN_DUMMY\
{\
{(DIO_PortAddressBasicType)DUMMY_PORT_DATA_ADDR,(DIO_PortAddressExtendedType)DUMMY_PORT_DATA_ADDR},\
(DIO_ModuleMapMaskType)0\
}

#define PIN(x,y)\
{\
{(DIO_PortAddressBasicType)&PT##x##D,(DIO_PortAddressExtendedType)&PT##x##PUE},\
(DIO_ModuleMapMaskType)PT##x##D_PT##x##D##y##_MASK\
}
/* Basic port structure */
typedef struct
{
	uint8_t dataOutput;
	uint8_t direction;
	uint8_t dataInput;
} portreg_struct_t;

/* Extended port structure */
typedef struct
{
	uint8_t pulling;
	uint8_t pullup;
	uint8_t driveStrength;
	uint8_t slewRate;
	uint8_t passiveFilter;
	uint8_t interruptControl;
	uint8_t intEn;
	uint8_t intFlag;
	uint8_t risingEdge;
	uint8_t digitalFilter;
	uint8_t digitalFilterControl;
} portreg_ext_struct_t;


/** Port Interrupt control register */
#define _HAL_MCF51_PTIC_REG(port)                   (PT##port##IC)

#define _HAL_MCF51_PTIC_PTMOD_MASK                  (PTAIC_PTAMOD_MASK)
#define _HAL_MCF51_PTIC_IE_MASK                     (PTAIC_PTAIE_MASK)
#define _HAL_MCF51_PTIC_DMAEN_MASK                  (PTAIC_PTADMAEN_MASK)

/** Port digital filter control register */
#define _HAL_MCF51_PTDFC_REG(port)                  (PT##port##DFC)

#define _HAL_MCF51_PTDFC_PTFF_MASK                  (PTADFC_PTAFF_MASK)            /* 31 */
#define _HAL_MCF51_PTDFC_PTFF_SHIFT                 (PTADFC_PTAFF_BITNUM)          /* 0  */
#define _HAL_MCF51_PTDFC_PTCLKS_MASK                (PTADFC_PTACLKS_MASK)

#define DIO_PULL_UP_IS_AVAILABLE	TRUE
#define DIO_IsAvailablePullUp(ptr) (1)

#define DIO_PULL_DOWN_IS_AVAILABLE	TRUE
#define DIO_IsAvailablePullDown(ptr) (1)
#define DIO_INPUT_IS_AVAILABLE	TRUE


#define IsAvailableAsInput(ptr)            (1)


#define DIO_SLEW_RATE_IS_AVAILABLE	TRUE
#define DIO_IsAvailableSlewRate(ptr)      (1)
#define DIO_DRIVE_STRENGTH	TRUE
#define DIO_IsAvailableDriveStrength(ptr) (1)
#define DIO_DriveStrengthEnable(ptr)   ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->driveStrength |= (ptr)->PinMask;
#define DIO_DriveStrengthDisable(ptr)   ((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength &= ~(ptr)->PinMask ;

#define DIO_IsDriveStrengthEnabled(ptr)   (((portreg_ext_struct_t *)ptr->PortAddress.extended)->driveStrength & (ptr)->PinMask)


#define DIO_IsOuput(ptr)\
    (((portreg_struct_t *)ptr->PortAddress.basic)->direction & ptr->PinMask)
#define DIO_OUTPUT_IS_AVAILABLE	TRUE
/*
\param [in] ptr points to IO_ModuleIOMapType.
*/
#define IsAvailableAsOutput(ptr)            (1)
#define DIO_PASSIVE_FILTER_IS_AVAILABLE  TRUE



#define DIO_IsPullUpEnabled(ptr)       (((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pulling & (ptr)->PinMask) && (((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pullup & (ptr)->PinMask)

#define DIO_PullUpEnable(ptr)       	((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pulling |= (ptr)->PinMask;\
                                    	((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->pullup |= (ptr)->PinMask;

#define DIO_PullUpDisable(ptr)      	((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pulling &= ~((ptr)->PinMask);\
                                    	((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->pullup &= ~(ptr)->PinMask;

#define DIO_IsPullDownEnabled(ptr)		(((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pulling & (ptr)->PinMask) && (!(((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pullup & (ptr)->PinMask))


#define DIO_PullDownEnable(ptr)			((portreg_ext_struct_t *)((ptr)->PortAddress.extended))->pulling |= (ptr)->PinMask;\
										((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->pullup &= ~(ptr)->PinMask;

#define DIO_PullDownDisable(ptr)    	DIO_PullUpDisable(ptr)

inline bool_t PassiveFilterIsAvaibleOn(portreg_ext_struct_t *port, DIO_ModuleMapMaskType bit)
{
    (void) port;
    (void) bit;
    return 1;
}
#define DIO_INTERRUPT_IS_AVAILABLE    TRUE
#define DIO_IsInterruptAvailable(ptr)       (1)
#define DIO_InterruptEnable(ptr) 			((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->intEn |= (ptr)->PinMask;
#define DIO_InterruptDisable(ptr)			((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->intEn &= ~(ptr)->PinMask;
#define DIO_IsInterruptEnabled(ptr) 		(((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->intEn & (ptr)->PinMask)

#define DIO_SetInterruptMode_FallingEdge(ptr) 	((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->risingEdge &= ~(ptr)->PinMask;
#define DIO_SetInterruptMode_RisingEdge(ptr) 	((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->risingEdge |= (ptr)->PinMask;
#define DIO_IsInterruptMode_RisingEdge(ptr) 	(((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->risingEdge & (ptr)->PinMask)

#define DIO_ClearInterrupt(ptr) 			((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->intFlag |= (ptr)->PinMask;
#define DIO_IsInterruptFlag(ptr)			(((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->intFlag & (ptr)->PinMask)

#define DIO_DIGITAL_FILTER_IS_AVAILABLE   TRUE
#define DIO_SetDirAsOutput(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->direction |= ptr->PinMask;  
#define DIO_SetDirAsInput(ptr)\
    ((portreg_struct_t *)ptr->PortAddress.basic)->direction &= ~(ptr->PinMask);  
#define DIO_IsDigitalFilterAvailable(ptr) 	(1)
#define DIO_DigitalFilterEnable(ptr)		((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->digitalFilter |= (ptr)->PinMask;
#define DIO_DigitalFilterDisable(ptr)		((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->digitalFilter &= ~(ptr)->PinMask;
#define DIO_IsDigitalFilterEnabled(ptr)		(((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->digitalFilter & (ptr)->PinMask)

/* filterFrequency is in KHz */
#define DIO_DigitalFilterClkFrequencySet(ptr, filterFrequency)\
{\
if (labs(filterFrequency-BUS_FREQUENCY_Hz/1000)<labs(filterFrequency-LPO_FREQUENCY_Hz/1000))\
	{\
		((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl &= ~_HAL_MCF51_PTDFC_PTCLKS_MASK;\
	}\
	else\
	{\
		((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl |= _HAL_MCF51_PTDFC_PTCLKS_MASK;\
	}\
}
/*
inline void DIO_SetDigitalFltClkFreq(portreg_ext_struct_t *port,uint16_t filterFrequency)
{
	if (labs(filterFrequency-BUS_FREQUENCY_Hz/1000)<labs(filterFrequency-LPO_FREQUENCY_Hz/1000))
	{
		port -> digitalFilterControl &= ~_HAL_MCF51_PTDFC_PTCLKS_MASK;
	}
	else
	{
		port -> digitalFilterControl |= _HAL_MCF51_PTDFC_PTCLKS_MASK;
	}
}*/

#define DIO_DigitalFilterClkFrequencyGet(ptr)\
	(((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl & _HAL_MCF51_PTDFC_PTCLKS_MASK)?(LPO_FREQUENCY_Hz/1000):(BUS_FREQUENCY_Hz/1000);
/*	
inline uint16_t DIO_GetDigitalFltFrequency(portreg_ext_struct_t *port)
{
	if (port -> digitalFilterControl & _HAL_MCF51_PTDFC_PTCLKS_MASK)
	{
		return (LPO_FREQUENCY_Hz/1000);
	}
	else
	{
		return (BUS_FREQUENCY_Hz/1000);
	}
}*/
#define DIO_GetVal(ptr)\
(( ((portreg_struct_t *)ptr->PortAddress.basic)->dataInput & ptr->PinMask ) !=0 )
#define DIO_SetLow(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput &= ~ptr->PinMask;
#define DIO_SetHigh(ptr)\
  ((portreg_struct_t *)ptr->PortAddress.basic)->dataOutput |= ptr->PinMask;
#define DIO_DigitalFilterNumSamplesSet(ptr,nSamples)\
((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl = (uint8_t)(((((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl) & ~(_HAL_MCF51_PTDFC_PTFF_MASK << _HAL_MCF51_PTDFC_PTFF_SHIFT)) | ((nSamples << _HAL_MCF51_PTDFC_PTFF_SHIFT) & _HAL_MCF51_PTDFC_PTFF_MASK));

#define DIO_DigitalFilterNumSamplesGet(ptr)\
(uint8_t)(((((portreg_ext_struct_t *)(ptr)->PortAddress.extended) -> digitalFilterControl) & _HAL_MCF51_PTDFC_PTFF_MASK) >> _HAL_MCF51_PTDFC_PTFF_SHIFT);
inline uint8_t DIO_GetDigitalFilterNsamples(portreg_ext_struct_t *port)
{
	return (uint8_t)(((port -> digitalFilterControl) & _HAL_MCF51_PTDFC_PTFF_MASK) >> _HAL_MCF51_PTDFC_PTFF_SHIFT);
}
inline void DIO_EnableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    (void) bit;
    port -> interruptControl |= _HAL_MCF51_PTIC_IE_MASK;
}
inline void	DIO_DisableInterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    (void) bit;
    port -> interruptControl &= ~_HAL_MCF51_PTIC_IE_MASK;
}
inline bool_t DIO_InterruptOnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    (void) bit;
    if (port -> interruptControl & _HAL_MCF51_PTIC_IE_MASK) {
        return 1;
    }
    else {
        return 0;
    }
}
#define DIO_DMA_IS_AVAILABLE  TRUE
#define DIO_IsDmaAvailable(ptr) (1)
#define DIO_DmaEnable(ptr) 			((portreg_ext_struct_t *)(ptr)->PortAddress.extended)-> interruptControl |= _HAL_MCF51_PTIC_DMAEN_MASK;
#define DIO_DmaDisable(ptr) 		((portreg_ext_struct_t *)(ptr)->PortAddress.extended)-> interruptControl &= ~_HAL_MCF51_PTIC_DMAEN_MASK;
#define DIO_IsDmaEnabled(ptr)		(((portreg_ext_struct_t *)(ptr)->PortAddress.extended)-> interruptControl & _HAL_MCF51_PTIC_DMAEN_MASK)

inline bool_t DIO_DMA_OnPort(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
    (void) bit;
    if (port -> interruptControl & _HAL_MCF51_PTIC_DMAEN_MASK) {
        return 1;
    }
    else {
        return 0;
    }
}

#define DIO_LevelDetectInterruptEnable(ptr) ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->interruptControl |= _HAL_MCF51_PTIC_PTMOD_MASK;

#define DIO_EdgeDetectInterruptEnable(ptr) ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->interruptControl &= ~_HAL_MCF51_PTIC_PTMOD_MASK;
#define DIO_GetDetectInterruptMode(ptr) ((( ((portreg_ext_struct_t *)(ptr)->PortAddress.extended)->interruptControl & _HAL_MCF51_PTIC_PTMOD_MASK )!=0)?1:2)
/*inline uint8_t DIO_GetDetectModeIntrpt(portreg_ext_struct_t *port,DIO_ModuleMapMaskType bit)
{
	if (port -> interruptControl & _HAL_MCF51_PTIC_PTMOD_MASK)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}*/
static const portreg_ext_struct_t ResetValueExtPort=
{
	0x00,    /*PTxPUE*/
	0x00,    /*PTxPUS*/
	0x00,    /*PTxDS*/
	0x00,    /*PTxSRE*/
	0xFF,    /*PTxPFE*/
	0x00,    /*PTxIC*/
	0x00,    /*PTxIPE*/
	0x00,    /*PTxIF*/
	0x00,    /*PTxIES*/
	0x00,    /*PTxDFE*/
	0x00     /*PTxDFC*/
};
inline void DeInitGPIO(void)
{
	volatile uint8_t *pBasic = &PTADD;
	volatile uint8_t *pExt = &PTAPUE;
	uint8_t i;


	for(i=0; i<9; i++)
	{
		*pBasic = 0x00;                     /* Clear Direction Reg. */
		*(portreg_ext_struct_t*)pExt  = ResetValueExtPort;
		pBasic +=4;                         /* Point to next Direction Reg. */
		pExt += 16;                         /* Point to next Pulling Enable Reg. */
	}
}

/*--------------------------------------------------------------------------------------*/

/**
Funzioni HAL per la Mcu: scritte per permettere il debug con la maggior flessibilità possibile,
permettendo di settare le singole porte indipendentemente dalla matrice ModuleIOMap della porta.
Particolarmente utile quando, durante il debug, si fa ricorso a pin non disponibili sulla
linkage board.
*/



#ifdef DIO_DEBUG

/**
Setta i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_SetMcuIOPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAD |= mcupin;
		break;
	case DIO_PORTB :
		PTBD |= mcupin;
		break;
	case DIO_PORTC :
		PTCD |= mcupin;
		break;
	case DIO_PORTD :
		PTDD |= mcupin;
		break;
	case DIO_PORTE :
		PTED |= mcupin;
		break;
	case DIO_PORTF :
		PTFD |= mcupin;
		break;
	case DIO_PORTG :
		PTGD |= mcupin;
		break;
	case DIO_PORTH :
		PTHD |= mcupin;
		break;
	case DIO_PORTJ :
		PTJD |= mcupin;
		break;

	default:
		break; /*gestione errore*/
	}
}


/**
Resetta i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin		maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_ResetMcuIOPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAD &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBD &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCD &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDD &= ~mcupin;
		break;
	case DIO_PORTE :
		PTED &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFD &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGD &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHD &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJD &= ~mcupin;
		break;

	default:
		break; /*gestione errore*/
	}
}


/**
Legge il valore dei pin di un'intera porta

\param  [in]		port		identificatore della porta

\return 	uint8_t

\retval		valore dei bit della porta (da 0x00 a 0xFF)

\attention \b Accesso diretto al microcontrollore

\return 	Le dimensioni del parametro ritornato sono pari alle dimensioni di una intera porta (GB60: 8 bit)

\retval		none

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline uint8_t HAL_GetMcuIOVal(uint8_t port)
{

	uint8_t portValue;
	switch (port)
	{
	case DIO_PORTA :
		portValue = PTAPV;
		break;
	case DIO_PORTB :
		portValue = PTBPV;
		break;
	case DIO_PORTC :
		portValue = PTCPV;
		break;
	case DIO_PORTD :
		portValue = PTDPV;
		break;
	case DIO_PORTE :
		portValue = PTEPV;
		break;
	case DIO_PORTF :
		portValue = PTFPV;
		break;
	case DIO_PORTG :
		portValue = PTGPV;
		break;
	case DIO_PORTH :
		portValue = PTHPV;
		break;
	case DIO_PORTJ :
		portValue = PTJPV;
		break;
	default:
		break; /*gestione errore*/
	}

	return portValue;
}


/**
Legge il valore del pin selezionato di una porta

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline bool_t HAL_GetMcuIOValPin(uint8_t port, uint8_t mcupin)
{

	bool_t portBitValue;
	switch (port)
	{
	case DIO_PORTA :
		portBitValue = (bool_t)(PTAPV & mcupin);
		break;
	case DIO_PORTB :
		portBitValue = (bool_t)(PTBPV & mcupin);
		break;
	case DIO_PORTC :
		portBitValue = (bool_t)(PTCPV & mcupin);
		break;
	case DIO_PORTD :
		portBitValue = (bool_t)(PTDPV & mcupin);
		break;
	case DIO_PORTE :
		portBitValue = (bool_t)(PTEPV & mcupin);
		break;
	case DIO_PORTF :
		portBitValue = (bool_t)(PTFPV & mcupin);
		break;
	case DIO_PORTG :
		portBitValue = (bool_t)(PTGPV & mcupin);
		break;
	case DIO_PORTH :
		portBitValue = (bool_t)(PTHPV & mcupin);
		break;
	case DIO_PORTJ :
		portBitValue = (bool_t)(PTJPV & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}

	return (bool_t)(portBitValue != 0);
}


/**
Impone i pin di un'intera porta come uscite usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_SetMcuIODirPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTADD |= mcupin;
		break;
	case DIO_PORTB :
		PTBDD |= mcupin;
		break;
	case DIO_PORTC :
		PTCDD |= mcupin;
		break;
	case DIO_PORTD :
		PTDDD |= mcupin;
		break;
	case DIO_PORTE :
		PTEDD |= mcupin;
		break;
	case DIO_PORTF :
		PTFDD |= mcupin;
		break;
	case DIO_PORTG :
		PTGDD |= mcupin;
		break;
	case DIO_PORTH :
		PTHDD |= mcupin;
		break;
	case DIO_PORTJ :
		PTJDD |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Impone i pin di un'intera porta come ingressi usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_ResetMcuIODirPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTADD &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBDD &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCDD &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDDD &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEDD &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFDD &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGDD &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHDD &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJDD &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Legge il valore della direzione della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline bool_t HAL_GetMcuIODirPin(uint8_t port, uint8_t mcupin)
{

	bool_t portPinDirection;

	switch (port)
	{
	case DIO_PORTA :
		portPinDirection = (bool_t)(PTADD & mcupin);
		break;
	case DIO_PORTB :
		portPinDirection = (bool_t)(PTBDD & mcupin);
		break;
	case DIO_PORTC :
		portPinDirection = (bool_t)(PTCDD & mcupin);
		break;
	case DIO_PORTD :
		portPinDirection = (bool_t)(PTDDD & mcupin);
		break;
	case DIO_PORTE :
		portPinDirection = (bool_t)(PTEDD & mcupin);
		break;
	case DIO_PORTF :
		portPinDirection = (bool_t)(PTFDD & mcupin);
		break;
	case DIO_PORTG :
		portPinDirection = (bool_t)(PTGDD & mcupin);
		break;
	case DIO_PORTH :
		portPinDirection = (bool_t)(PTHDD & mcupin);
		break;
	case DIO_PORTJ :
		portPinDirection = (bool_t)(PTJDD & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(portPinDirection =! 0);
}


/**
Sets the pulling mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIOPullingPin(uint8_t port, uint8_t mcupin)
{
	switch (port)
	{
	case DIO_PORTA :
		PTAPUE |= mcupin;
		break;
	case DIO_PORTB :
		PTBPUE |= mcupin;
		break;
	case DIO_PORTC :
		PTCPUE |= mcupin;
		break;
	case DIO_PORTD :
		PTDPUE |= mcupin;
		break;
	case DIO_PORTE :
		PTEPUE |= mcupin;
		break;
	case DIO_PORTF :
		PTFPUE |= mcupin;
		break;
	case DIO_PORTG :
		PTGPUE |= mcupin;
		break;
	case DIO_PORTH :
		PTHPUE |= mcupin;
		break;
	case DIO_PORTJ :
		PTJPUE |= mcupin;
		break;

	default:
		break; /*gestione errore*/
	}
}


/**
Resets the pulling mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIOPullingPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAPUE &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBPUE &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCPUE &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDPUE &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEPUE &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFPUE &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGPUE &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHPUE &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJPUE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the pulling mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIOPullingPin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinPulling;

	switch (port)
	{
	case DIO_PORTA :
		PortPinPulling = (bool_t)(PTAPUE & mcupin);
		break;
	case DIO_PORTB :
		PortPinPulling = (bool_t)(PTBPUE & mcupin);
		break;
	case DIO_PORTC :
		PortPinPulling = (bool_t)(PTCPUE & mcupin);
		break;
	case DIO_PORTD :
		PortPinPulling = (bool_t)(PTDPUE & mcupin);
		break;
	case DIO_PORTE :
		PortPinPulling = (bool_t)(PTEPUE & mcupin);
		break;
	case DIO_PORTF :
		PortPinPulling = (bool_t)(PTFPUE & mcupin);
		break;
	case DIO_PORTG :
		PortPinPulling = (bool_t)(PTGPUE & mcupin);
		break;
	case DIO_PORTH :
		PortPinPulling = (bool_t)(PTHPUE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinPulling = (bool_t)(PTJPUE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinPulling != 0);
}



/**
Setta il pullup per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_SetMcuIOPullupPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAPUS |= mcupin;
		break;
	case DIO_PORTB :
		PTBPUS |= mcupin;
		break;
	case DIO_PORTC :
		PTCPUS |= mcupin;
		break;
	case DIO_PORTD :
		PTDPUS |= mcupin;
		break;
	case DIO_PORTE :
		PTEPUS |= mcupin;
		break;
	case DIO_PORTF :
		PTFPUS |= mcupin;
		break;
	case DIO_PORTG :
		PTGPUS |= mcupin;
		break;
	case DIO_PORTH :
		PTHPUS |= mcupin;
		break;
	case DIO_PORTJ :
		PTJPUS |= mcupin;
		break;

	default:
		break; /*gestione errore*/
	}
}

/**
Resetta il pullup per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_SetMcuIOPulldownPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAPUS &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBPUS &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCPUS &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDPUS &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEPUS &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFPUS &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGPUS &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHPUS &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJPUE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Legge il valore di pullup della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline bool_t HAL_GetMcuIOPullupPin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinPulling;

	switch (port)
	{
	case DIO_PORTA :
		PortPinPulling = (bool_t)(PTAPUS & mcupin);
		break;
	case DIO_PORTB :
		PortPinPulling = (bool_t)(PTBPUS & mcupin);
		break;
	case DIO_PORTC :
		PortPinPulling = (bool_t)(PTCPUS & mcupin);
		break;
	case DIO_PORTD :
		PortPinPulling = (bool_t)(PTDPUS & mcupin);
		break;
	case DIO_PORTE :
		PortPinPulling = (bool_t)(PTEPUS & mcupin);
		break;
	case DIO_PORTF :
		PortPinPulling = (bool_t)(PTFPUS & mcupin);
		break;
	case DIO_PORTG :
		PortPinPulling = (bool_t)(PTGPUS & mcupin);
		break;
	case DIO_PORTH :
		PortPinPulling = (bool_t)(PTHPUS & mcupin);
		break;
	case DIO_PORTJ :
		PortPinPulling = (bool_t)(PTJPUS & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinPulling != 0);
}


/**
Sets the drive strength mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIODriveStrengthPin(uint8_t port, uint8_t mcupin)
{
	switch (port)
	{
	case DIO_PORTA :
		PTADS |= mcupin;
		break;
	case DIO_PORTB :
		PTBDS |= mcupin;
		break;
	case DIO_PORTC :
		PTCDS |= mcupin;
		break;
	case DIO_PORTD :
		PTDDS |= mcupin;
		break;
	case DIO_PORTE :
		PTEDS |= mcupin;
		break;
	case DIO_PORTF :
		PTFDS |= mcupin;
		break;
	case DIO_PORTG :
		PTGDS |= mcupin;
		break;
	case DIO_PORTH :
		PTHDS |= mcupin;
		break;
	case DIO_PORTJ :
		PTJDS |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Resets the drive strength mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIODriveStrengthPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTADS &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBDS &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCDS &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDDS &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEDS &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFDS &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGDS &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHDS &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJDS &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the drive strength mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIODriveStrengthPin(uint8_t port, uint8_t mcupin)
{
	bool_t PortPinDriveStrength;

	switch (port)
	{
	case DIO_PORTA :
		PortPinDriveStrength = (bool_t)(PTADS & mcupin);
		break;
	case DIO_PORTB :
		PortPinDriveStrength = (bool_t)(PTBDS & mcupin);
		break;
	case DIO_PORTC :
		PortPinDriveStrength = (bool_t)(PTCDS & mcupin);
		break;
	case DIO_PORTD :
		PortPinDriveStrength = (bool_t)(PTDDS & mcupin);
		break;
	case DIO_PORTE :
		PortPinDriveStrength = (bool_t)(PTEDS & mcupin);
		break;
	case DIO_PORTF :
		PortPinDriveStrength = (bool_t)(PTFDS & mcupin);
		break;
	case DIO_PORTG :
		PortPinDriveStrength = (bool_t)(PTGDS & mcupin);
		break;
	case DIO_PORTH :
		PortPinDriveStrength = (bool_t)(PTHDS & mcupin);
		break;
	case DIO_PORTJ :
		PortPinDriveStrength = (bool_t)(PTJDS & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinDriveStrength != 0);
}

/**
Setta lo slew-rate per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/
inline void HAL_SetMcuIOSlewRatePin(uint8_t port, uint8_t mcupin)
{
	switch (port)
	{
	case DIO_PORTA :
		PTASRE |= mcupin;
		break;
	case DIO_PORTB :
		PTBSRE |= mcupin;
		break;
	case DIO_PORTC :
		PTCSRE |= mcupin;
		break;
	case DIO_PORTD :
		PTDSRE |= mcupin;
		break;
	case DIO_PORTE :
		PTESRE |= mcupin;
		break;
	case DIO_PORTF :
		PTFSRE |= mcupin;
		break;
	case DIO_PORTG :
		PTGSRE |= mcupin;
		break;
	case DIO_PORTH :
		PTHSRE |= mcupin;
		break;
	case DIO_PORTJ :
		PTJSRE |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Resetta lo slew-rate per i pin di un'intera porta usando una maschera

\param  [in]		port		identificatore della porta
\param  [in]		mcupin	maschera di settaggio dei pin della porta

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline void HAL_ResetMcuIOSlewRatePin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTASRE &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBSRE &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCSRE &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDSRE &= ~mcupin;
		break;
	case DIO_PORTE :
		PTESRE &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFSRE &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGSRE &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHSRE &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJSRE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Legge il valore di slew-rate della porta per il pin selezionato

\param  [in]		port		identificatore della porta
\param  [in]		mcupin 	maschera di selezione del pin

\return 	bool_t

\retval		0, 1

\attention \b Accesso diretto al microcontrollore

\author 	Pierluigi Ianni
\date		24/06/2009
\version	1.0.0

\note		\par \b Nota 1:
			documentation by Pierluigi Ianni
			\par \b Nota 2:
			funzione usata solo in debug
			\par \b Nota 3:
			Target dependent

*/

inline bool_t HAL_GetMcuIOSlewRatePin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinSlewRate;

	switch (port)
	{
	case DIO_PORTA :
		PortPinSlewRate = (bool_t)(PTASRE & mcupin);
		break;
	case DIO_PORTB :
		PortPinSlewRate = (bool_t)(PTBSRE & mcupin);
		break;
	case DIO_PORTC :
		PortPinSlewRate = (bool_t)(PTCSRE & mcupin);
		break;
	case DIO_PORTD :
		PortPinSlewRate = (bool_t)(PTDSRE & mcupin);
		break;
	case DIO_PORTE :
		PortPinSlewRate = (bool_t)(PTESRE & mcupin);
		break;
	case DIO_PORTF :
		PortPinSlewRate = (bool_t)(PTFSRE & mcupin);
		break;
	case DIO_PORTG :
		PortPinSlewRate = (bool_t)(PTGSRE & mcupin);
		break;
	case DIO_PORTH :
		PortPinSlewRate = (bool_t)(PTHSRE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinSlewRate = (bool_t)(PTJSRE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinSlewRate != 0);
}

/**
Sets the passive filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIOPassiveFilterPin(uint8_t port, uint8_t mcupin)
{
	switch (port)
	{
	case DIO_PORTA :
		PTAPFE |= mcupin;
		break;
	case DIO_PORTB :
		PTBPFE |= mcupin;
		break;
	case DIO_PORTC :
		PTCPFE |= mcupin;
		break;
	case DIO_PORTD :
		PTDPFE |= mcupin;
		break;
	case DIO_PORTE :
		PTEPFE |= mcupin;
		break;
	case DIO_PORTF :
		PTFPFE |= mcupin;
		break;
	case DIO_PORTG :
		PTGPFE |= mcupin;
		break;
	case DIO_PORTH :
		PTHPFE |= mcupin;
		break;
	case DIO_PORTJ :
		PTJPFE |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Resets the passive filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIOPassiveFilterPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAPFE &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBPFE &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCPFE &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDPFE &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEPFE &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFPFE &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGPFE &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHPFE &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJPFE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the passive filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIOPassiveFilterPin(uint8_t port, uint8_t mcupin)
{
	bool_t PortPinPassiveFilter;

	switch (port)
	{
	case DIO_PORTA :
		PortPinPassiveFilter = (bool_t)(PTAPFE & mcupin);
		break;
	case DIO_PORTB :
		PortPinPassiveFilter = (bool_t)(PTBPFE & mcupin);
		break;
	case DIO_PORTC :
		PortPinPassiveFilter = (bool_t)(PTCPFE & mcupin);
		break;
	case DIO_PORTD :
		PortPinPassiveFilter = (bool_t)(PTDPFE & mcupin);
		break;
	case DIO_PORTE :
		PortPinPassiveFilter = (bool_t)(PTEPFE & mcupin);
		break;
	case DIO_PORTF :
		PortPinPassiveFilter = (bool_t)(PTFPFE & mcupin);
		break;
	case DIO_PORTG :
		PortPinPassiveFilter = (bool_t)(PTGPFE & mcupin);
		break;
	case DIO_PORTH :
		PortPinPassiveFilter = (bool_t)(PTHPFE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinPassiveFilter = (bool_t)(PTJPFE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinPassiveFilter != 0);
}

/**
Sets the digital filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIODigitalFilterPin(uint8_t port, uint8_t mcupin)
{
	switch (port)
	{
	case DIO_PORTA :
		PTADFE |= mcupin;
		break;
	case DIO_PORTB :
		PTBDFE |= mcupin;
		break;
	case DIO_PORTC :
		PTCDFE |= mcupin;
		break;
	case DIO_PORTD :
		PTDDFE |= mcupin;
		break;
	case DIO_PORTE :
		PTEDFE |= mcupin;
		break;
	case DIO_PORTF :
		PTFDFE |= mcupin;
		break;
	case DIO_PORTG :
		PTGDFE |= mcupin;
		break;
	case DIO_PORTH :
		PTHDFE |= mcupin;
		break;
	case DIO_PORTJ :
		PTJDFE |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Resets the digital filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIODigitalFilterPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTADFE &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBDFE &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCDFE &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDDFE &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEDFE &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFDFE &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGDFE &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHDFE &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJDFE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the digital filter mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIODigitalFilterPin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinDigitalFilter;

	switch (port)
	{
	case DIO_PORTA :
		PortPinDigitalFilter = (bool_t)(PTADFE & mcupin);
		break;
	case DIO_PORTB :
		PortPinDigitalFilter = (bool_t)(PTBDFE & mcupin);
		break;
	case DIO_PORTC :
		PortPinDigitalFilter = (bool_t)(PTCDFE & mcupin);
		break;
	case DIO_PORTD :
		PortPinDigitalFilter = (bool_t)(PTDDFE & mcupin);
		break;
	case DIO_PORTE :
		PortPinDigitalFilter = (bool_t)(PTEDFE & mcupin);
		break;
	case DIO_PORTF :
		PortPinDigitalFilter = (bool_t)(PTFDFE & mcupin);
		break;
	case DIO_PORTG :
		PortPinDigitalFilter = (bool_t)(PTGDFE & mcupin);
		break;
	case DIO_PORTH :
		PortPinDigitalFilter = (bool_t)(PTHDFE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinDigitalFilter = (bool_t)(PTJDFE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinDigitalFilter != 0);
}


/**
Sets the interrupt mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIOInterruptPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIPE |= mcupin;
		break;
	case DIO_PORTB :
		PTBIPE |= mcupin;
		break;
	case DIO_PORTC :
		PTCIPE |= mcupin;
		break;
	case DIO_PORTD :
		PTDIPE |= mcupin;
		break;
	case DIO_PORTE :
		PTEIPE |= mcupin;
		break;
	case DIO_PORTF :
		PTFIPE |= mcupin;
		break;
	case DIO_PORTG :
		PTGIPE |= mcupin;
		break;
	case DIO_PORTH :
		PTHIPE |= mcupin;
		break;
	case DIO_PORTJ :
		PTJIPE |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Resets the interrupt mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIOInterruptPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIPE &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBIPE &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCIPE &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDIPE &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEIPE &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFIPE &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGIPE &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHIPE &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJIPE &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the interrupt mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIOInterruptPin(uint8_t port, uint8_t mcupin)
{
	bool_t PortPinInterrupt;

	switch (port)
	{
	case DIO_PORTA :
		PortPinInterrupt = (bool_t)(PTAIPE & mcupin);
		break;
	case DIO_PORTB :
		PortPinInterrupt = (bool_t)(PTBIPE & mcupin);
		break;
	case DIO_PORTC :
		PortPinInterrupt = (bool_t)(PTCIPE & mcupin);
		break;
	case DIO_PORTD :
		PortPinInterrupt = (bool_t)(PTDIPE & mcupin);
		break;
	case DIO_PORTE :
		PortPinInterrupt = (bool_t)(PTEIPE & mcupin);
		break;
	case DIO_PORTF :
		PortPinInterrupt = (bool_t)(PTFIPE & mcupin);
		break;
	case DIO_PORTG :
		PortPinInterrupt = (bool_t)(PTGIPE & mcupin);
		break;
	case DIO_PORTH :
		PortPinInterrupt = (bool_t)(PTHIPE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinInterrupt = (bool_t)(PTJIPE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinInterrupt != 0);
}


/**
Resets the interrupt flag of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_ResetMcuIOInterruptFlagPin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIF |= mcupin;
		break;
	case DIO_PORTB :
		PTBIF |= mcupin;
		break;
	case DIO_PORTC :
		PTCIF |= mcupin;
		break;
	case DIO_PORTD :
		PTDIF |= mcupin;
		break;
	case DIO_PORTE :
		PTEIF |= mcupin;
		break;
	case DIO_PORTF :
		PTFIF |= mcupin;
		break;
	case DIO_PORTG :
		PTGIF |= mcupin;
		break;
	case DIO_PORTH :
		PTHIF |= mcupin;
		break;
	case DIO_PORTJ :
		PTJIF |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the interrupt flag of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIOInterruptFlagPin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinInterruptFlag;

	switch (port)
	{
	case DIO_PORTA :
		PortPinInterruptFlag = (bool_t)(PTAIF & mcupin);
		break;
	case DIO_PORTB :
		PortPinInterruptFlag = (bool_t)(PTBIF & mcupin);
		break;
	case DIO_PORTC :
		PortPinInterruptFlag = (bool_t)(PTCIF & mcupin);
		break;
	case DIO_PORTD :
		PortPinInterruptFlag = (bool_t)(PTDIF & mcupin);
		break;
	case DIO_PORTE :
		PortPinInterruptFlag = (bool_t)(PTEIF & mcupin);
		break;
	case DIO_PORTF :
		PortPinInterruptFlag = (bool_t)(PTFIF & mcupin);
		break;
	case DIO_PORTG :
		PortPinInterruptFlag = (bool_t)(PTGIF & mcupin);
		break;
	case DIO_PORTH :
		PortPinInterruptFlag = (bool_t)(PTHIF & mcupin);
		break;
	case DIO_PORTJ :
		PortPinInterruptFlag = (bool_t)(PTJIF & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinInterruptFlag != 0);
}

/**
Sets the rising edge mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIORisingEdgePin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIES |= mcupin;
		break;
	case DIO_PORTB :
		PTBIES |= mcupin;
		break;
	case DIO_PORTC :
		PTCIES |= mcupin;
		break;
	case DIO_PORTD :
		PTDIES |= mcupin;
		break;
	case DIO_PORTE :
		PTEIES |= mcupin;
		break;
	case DIO_PORTF :
		PTFIES |= mcupin;
		break;
	case DIO_PORTG :
		PTGIES |= mcupin;
		break;
	case DIO_PORTH :
		PTHIES |= mcupin;
		break;
	case DIO_PORTJ :
		PTJIES |= mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Sets the falling edge mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline void HAL_SetMcuIOFallingEdgePin(uint8_t port, uint8_t mcupin)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIES &= ~mcupin;
		break;
	case DIO_PORTB :
		PTBIES &= ~mcupin;
		break;
	case DIO_PORTC :
		PTCIES &= ~mcupin;
		break;
	case DIO_PORTD :
		PTDIES &= ~mcupin;
		break;
	case DIO_PORTE :
		PTEIES &= ~mcupin;
		break;
	case DIO_PORTF :
		PTFIES &= ~mcupin;
		break;
	case DIO_PORTG :
		PTGIES &= ~mcupin;
		break;
	case DIO_PORTH :
		PTHIES &= ~mcupin;
		break;
	case DIO_PORTJ :
		PTJIES &= ~mcupin;
		break;
	default:
		break; /*gestione errore*/
	}
}


/**
Reads the edge mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/

inline bool_t HAL_GetMcuIOEdgePin(uint8_t port, uint8_t mcupin)
{

	bool_t PortPinEdge;

	switch (port)
	{
	case DIO_PORTA :
		PortPinEdge = (bool_t)(PTAIPE & mcupin);
		break;
	case DIO_PORTB :
		PortPinEdge = (bool_t)(PTBIPE & mcupin);
		break;
	case DIO_PORTC :
		PortPinEdge = (bool_t)(PTCIPE & mcupin);
		break;
	case DIO_PORTD :
		PortPinEdge = (bool_t)(PTDIPE & mcupin);
		break;
	case DIO_PORTE :
		PortPinEdge = (bool_t)(PTEIPE & mcupin);
		break;
	case DIO_PORTF :
		PortPinEdge = (bool_t)(PTFIPE & mcupin);
		break;
	case DIO_PORTG :
		PortPinEdge = (bool_t)(PTGIPE & mcupin);
		break;
	case DIO_PORTH :
		PortPinEdge = (bool_t)(PTHIPE & mcupin);
		break;
	case DIO_PORTJ :
		PortPinEdge = (bool_t)(PTJIPE & mcupin);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortPinEdge != 0);
}

/**
Sets the level detection mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIOLevelDetectPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC |= PTAIC_PTAMOD_MASK;
		break;
	case DIO_PORTB :
		PTBIC |= PTBIC_PTBMOD_MASK;
		break;
	case DIO_PORTC :
		PTCIC |= PTCIC_PTCMOD_MASK;
		break;
	case DIO_PORTD :
		PTDIC |= PTDIC_PTDMOD_MASK;
		break;
	case DIO_PORTE :
		PTEIC |= PTEIC_PTEMOD_MASK;
		break;
	case DIO_PORTF :
		PTFIC |= PTFIC_PTFMOD_MASK;
		break;
	case DIO_PORTG :
		PTGIC |= PTGIC_PTGMOD_MASK;
		break;
	case DIO_PORTH :
		PTHIC |= PTHIC_PTHMOD_MASK;
		break;
	case DIO_PORTJ :
		PTJIC |= PTJIC_PTJMOD_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Sets the edge detection mode of a pin of a port

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIOEdgeDetectPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC &= ~PTAIC_PTAMOD_MASK;
		break;
	case DIO_PORTB :
		PTBIC &= ~PTBIC_PTBMOD_MASK;
		break;
	case DIO_PORTC :
		PTCIC &= ~PTCIC_PTCMOD_MASK;
		break;
	case DIO_PORTD :
		PTDIC &= ~PTDIC_PTDMOD_MASK;
		break;
	case DIO_PORTE :
		PTEIC &= ~PTEIC_PTEMOD_MASK;
		break;
	case DIO_PORTF :
		PTFIC &= ~PTFIC_PTFMOD_MASK;
		break;
	case DIO_PORTG :
		PTGIC &= ~PTGIC_PTGMOD_MASK;
		break;
	case DIO_PORTH :
		PTHIC &= ~PTHIC_PTHMOD_MASK;
		break;
	case DIO_PORTJ :
		PTJIC &= ~PTJIC_PTJMOD_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Reads if the level detection mode of a pin of a port is selected

\param  [in]		port	port identificator
\param  [in]		mcupin	mask of the pin within the port

\return 	bool_t

\retval		0 - edge, 1 - level

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline bool_t HAL_IsMcuIOLevelDetectPort(uint8_t port)
{

	bool_t PortLevelMode;

	switch (port)
	{
	case DIO_PORTA :
		PortLevelMode = (bool_t)(PTAIC & PTAIC_PTAMOD_MASK);
		break;
	case DIO_PORTB :
		PortLevelMode = (bool_t)(PTBIC & PTBIC_PTBMOD_MASK);
		break;
	case DIO_PORTC :
		PortLevelMode = (bool_t)(PTCIC & PTCIC_PTCMOD_MASK);
		break;
	case DIO_PORTD :
		PortLevelMode = (bool_t)(PTDIC & PTDIC_PTDMOD_MASK);
		break;
	case DIO_PORTE :
		PortLevelMode = (bool_t)(PTEIC & PTEIC_PTEMOD_MASK);
		break;
	case DIO_PORTF :
		PortLevelMode = (bool_t)(PTFIC & PTFIC_PTFMOD_MASK);
		break;
	case DIO_PORTG :
		PortLevelMode = (bool_t)(PTGIC & PTGIC_PTGMOD_MASK);
		break;
	case DIO_PORTH :
		PortLevelMode = (bool_t)(PTHIC & PTHIC_PTHMOD_MASK);
		break;
	case DIO_PORTJ :
		PortLevelMode = (bool_t)(PTJIC & PTJIC_PTJMOD_MASK);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortLevelMode != 0);
}

/**
Enable interrupt of a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_EnableMcuIOInterruptPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC |= PTAIC_PTAIE_MASK;
		break;
	case DIO_PORTB :
		PTBIC |= PTBIC_PTBIE_MASK;
		break;
	case DIO_PORTC :
		PTCIC |= PTCIC_PTCIE_MASK;
		break;
	case DIO_PORTD :
		PTDIC |= PTDIC_PTDIE_MASK;
		break;
	case DIO_PORTE :
		PTEIC |= PTEIC_PTEIE_MASK;
		break;
	case DIO_PORTF :
		PTFIC |= PTFIC_PTFIE_MASK;
		break;
	case DIO_PORTG :
		PTGIC |= PTGIC_PTGIE_MASK;
		break;
	case DIO_PORTH :
		PTHIC |= PTHIC_PTHIE_MASK;
		break;
	case DIO_PORTJ :
		PTJIC |= PTJIC_PTJIE_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Disable interrupt of a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_DisableMcuIOInterruptPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC &= ~PTAIC_PTAIE_MASK;
		break;
	case DIO_PORTB :
		PTBIC &= ~PTBIC_PTBIE_MASK;
		break;
	case DIO_PORTC :
		PTCIC &= ~PTCIC_PTCIE_MASK;
		break;
	case DIO_PORTD :
		PTDIC &= ~PTDIC_PTDIE_MASK;
		break;
	case DIO_PORTE :
		PTEIC &= ~PTEIC_PTEIE_MASK;
		break;
	case DIO_PORTF :
		PTFIC &= ~PTFIC_PTFIE_MASK;
		break;
	case DIO_PORTG :
		PTGIC &= ~PTGIC_PTGIE_MASK;
		break;
	case DIO_PORTH :
		PTHIC &= ~PTHIC_PTHIE_MASK;
		break;
	case DIO_PORTJ :
		PTJIC &= ~PTJIC_PTJIE_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Reads the interrupt mode of a port

\param  [in]		port	port identificator

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline bool_t HAL_IsMcuIOInterruptPort(uint8_t port)
{

	bool_t PortInterrupt;

	switch (port)
	{
	case DIO_PORTA :
		PortInterrupt = (bool_t)(PTAIC & PTAIC_PTAIE_MASK);
		break;
	case DIO_PORTB :
		PortInterrupt = (bool_t)(PTBIC & PTBIC_PTBIE_MASK);
		break;
	case DIO_PORTC :
		PortInterrupt = (bool_t)(PTCIC & PTCIC_PTCIE_MASK);
		break;
	case DIO_PORTD :
		PortInterrupt = (bool_t)(PTDIC & PTDIC_PTDIE_MASK);
		break;
	case DIO_PORTE :
		PortInterrupt = (bool_t)(PTEIC & PTEIC_PTEIE_MASK);
		break;
	case DIO_PORTF :
		PortInterrupt = (bool_t)(PTFIC & PTFIC_PTFIE_MASK);
		break;
	case DIO_PORTG :
		PortInterrupt = (bool_t)(PTGIC & PTGIC_PTGIE_MASK);
		break;
	case DIO_PORTH :
		PortInterrupt = (bool_t)(PTHIC & PTHIC_PTHIE_MASK);
		break;
	case DIO_PORTJ :
		PortInterrupt = (bool_t)(PTJIC & PTJIC_PTJIE_MASK);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortInterrupt != 0);
}

/**
Enable DMA of a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_EnableMcuIODMAPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC |= PTAIC_PTADMAEN_MASK;
		break;
	case DIO_PORTB :
		PTBIC |= PTBIC_PTBDMAEN_MASK;
		break;
	case DIO_PORTC :
		PTCIC |= PTCIC_PTCDMAEN_MASK;
		break;
	case DIO_PORTD :
		PTDIC |= PTDIC_PTDDMAEN_MASK;
		break;
	case DIO_PORTE :
		PTEIC |= PTEIC_PTEDMAEN_MASK;
		break;
	case DIO_PORTF :
		PTFIC |= PTFIC_PTFDMAEN_MASK;
		break;
	case DIO_PORTG :
		PTGIC |= PTGIC_PTGDMAEN_MASK;
		break;
	case DIO_PORTH :
		PTHIC |= PTHIC_PTHDMAEN_MASK;
		break;
	case DIO_PORTJ :
		PTJIC |= PTJIC_PTJDMAEN_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Disable DMA of a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_DisableMcuIODMAPort(uint8_t port)
{

	switch (port)
	{
	case DIO_PORTA :
		PTAIC &= ~PTAIC_PTADMAEN_MASK;
		break;
	case DIO_PORTB :
		PTBIC &= ~PTBIC_PTBDMAEN_MASK;
		break;
	case DIO_PORTC :
		PTCIC &= ~PTCIC_PTCDMAEN_MASK;
		break;
	case DIO_PORTD :
		PTDIC &= ~PTDIC_PTDDMAEN_MASK;
		break;
	case DIO_PORTE :
		PTEIC &= ~PTEIC_PTEDMAEN_MASK;
		break;
	case DIO_PORTF :
		PTFIC &= ~PTFIC_PTFDMAEN_MASK;
		break;
	case DIO_PORTG :
		PTGIC &= ~PTGIC_PTGDMAEN_MASK;
		break;
	case DIO_PORTH :
		PTHIC &= ~PTHIC_PTHDMAEN_MASK;
		break;
	case DIO_PORTJ :
		PTJIC &= ~PTJIC_PTJDMAEN_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Reads the DMA mode of a port

\param  [in]		port	port identificator

\return 	bool_t

\retval		0, 1

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline bool_t HAL_IsMcuIODMAPort(uint8_t port)
{

	bool_t PortDMA;

	switch (port)
	{
	case DIO_PORTA :
		PortDMA = (bool_t)(PTAIC & PTAIC_PTADMAEN_MASK);
		break;
	case DIO_PORTB :
		PortDMA = (bool_t)(PTBIC & PTBIC_PTBDMAEN_MASK);
		break;
	case DIO_PORTC :
		PortDMA = (bool_t)(PTCIC & PTCIC_PTCDMAEN_MASK);
		break;
	case DIO_PORTD :
		PortDMA = (bool_t)(PTDIC & PTDIC_PTDDMAEN_MASK);
		break;
	case DIO_PORTE :
		PortDMA = (bool_t)(PTEIC & PTEIC_PTEDMAEN_MASK);
		break;
	case DIO_PORTF :
		PortDMA = (bool_t)(PTFIC & PTFIC_PTFDMAEN_MASK);
		break;
	case DIO_PORTG :
		PortDMA = (bool_t)(PTGIC & PTGIC_PTGDMAEN_MASK);
		break;
	case DIO_PORTH :
		PortDMA = (bool_t)(PTHIC & PTHIC_PTHDMAEN_MASK);
		break;
	case DIO_PORTJ :
		PortDMA = (bool_t)(PTJIC & PTJIC_PTJDMAEN_MASK);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortDMA != 0);
}

/**
Sets the digital filter LPO clock for a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIODigitalFilterLPOClockPort(uint8_t port)
{
	switch (port)
	{
	case DIO_PORTA :
		PTADFC |= PTADFC_PTACLKS_MASK;
		break;
	case DIO_PORTB :
		PTBDFC |= PTBDFC_PTBCLKS_MASK;
		break;
	case DIO_PORTC :
		PTCDFC |= PTCDFC_PTCCLKS_MASK;
		break;
	case DIO_PORTD :
		PTDDFC |= PTDDFC_PTDCLKS_MASK;
		break;
	case DIO_PORTE :
		PTEDFC |= PTEDFC_PTECLKS_MASK;
		break;
	case DIO_PORTF :
		PTFDFC |= PTFDFC_PTFCLKS_MASK;
		break;
	case DIO_PORTG :
		PTGDFC |= PTGDFC_PTGCLKS_MASK;
		break;
	case DIO_PORTH :
		PTHDFC |= PTHDFC_PTHCLKS_MASK;
		break;
	case DIO_PORTJ :
		PTJDFC |= PTJDFC_PTJCLKS_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Sets the digital filter bus clock for a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIODigitalFilterBusClockPort(uint8_t port)
{
	switch (port)
	{
	case DIO_PORTA :
		PTADFC &= ~PTADFC_PTACLKS_MASK;
		break;
	case DIO_PORTB :
		PTBDFC &= ~PTBDFC_PTBCLKS_MASK;
		break;
	case DIO_PORTC :
		PTCDFC &= ~PTCDFC_PTCCLKS_MASK;
		break;
	case DIO_PORTD :
		PTDDFC &= ~PTDDFC_PTDCLKS_MASK;
		break;
	case DIO_PORTE :
		PTEDFC &= ~PTEDFC_PTECLKS_MASK;
		break;
	case DIO_PORTF :
		PTFDFC &= ~PTFDFC_PTFCLKS_MASK;
		break;
	case DIO_PORTG :
		PTGDFC &= ~PTGDFC_PTGCLKS_MASK;
		break;
	case DIO_PORTH :
		PTHDFC &= ~PTHDFC_PTHCLKS_MASK;
		break;
	case DIO_PORTJ :
		PTJDFC &= ~PTJDFC_PTJCLKS_MASK;
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Reads if the digital filter LPO clock is selected

\param  [in]		port	port identificator

\return 	bool_t

\retval		0 - bus clock, 1 - LPO clock

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline bool_t HAL_IsMcuIODigitalFilterLPOClockPort(uint8_t port)
{
	bool_t PortLPOClock;

	switch (port)
	{
	case DIO_PORTA :
		PortLPOClock = (bool_t)(PTADFC & PTADFC_PTACLKS_MASK);
		break;
	case DIO_PORTB :
		PortLPOClock = (bool_t)(PTBDFC & PTBDFC_PTBCLKS_MASK);
		break;
	case DIO_PORTC :
		PortLPOClock = (bool_t)(PTCDFC & PTCDFC_PTCCLKS_MASK);
		break;
	case DIO_PORTD :
		PortLPOClock = (bool_t)(PTCDFC & PTDDFC_PTDCLKS_MASK);
		break;
	case DIO_PORTE :
		PortLPOClock = (bool_t)(PTEDFC & PTEDFC_PTECLKS_MASK);
		break;
	case DIO_PORTF :
		PortLPOClock = (bool_t)(PTFDFC & PTFDFC_PTFCLKS_MASK);
		break;
	case DIO_PORTG :
		PortLPOClock = (bool_t)(PTGDFC & PTGDFC_PTGCLKS_MASK);
		break;
	case DIO_PORTH :
		PortLPOClock = (bool_t)(PTHDFC & PTHDFC_PTHCLKS_MASK);
		break;
	case DIO_PORTJ :
		PortLPOClock = (bool_t)(PTJDFC & PTJDFC_PTJCLKS_MASK);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortLPOClock != 0);
}

/**
Sets the digital filter factor of a port

\param  [in]		port	port identificator

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline void HAL_SetMcuIODigitalFilterFactorPort(uint8_t port, uint8_t factor)
{
	switch (port)
	{
	case DIO_PORTA :
		PTADFC = (bool_t)((PTADFC & ~(PTADFC_PTAFF_MASK << PTADFC_PTAFF_BITNUM)) | ((factor << PTADFC_PTAFF_BITNUM) & PTADFC_PTAFF_MASK));
		break;
	case DIO_PORTB :
		PTBDFC = (bool_t)((PTBDFC & ~(PTBDFC_PTBFF_MASK << PTBDFC_PTBFF_BITNUM)) | ((factor << PTBDFC_PTBFF_BITNUM) & PTBDFC_PTBFF_MASK));
		break;
	case DIO_PORTC :
		PTCDFC = (bool_t)((PTCDFC & ~(PTCDFC_PTCFF_MASK << PTCDFC_PTCFF_BITNUM)) | ((factor << PTCDFC_PTCFF_BITNUM) & PTCDFC_PTCFF_MASK));
		break;
	case DIO_PORTD :
		PTDDFC = (bool_t)((PTDDFC & ~(PTDDFC_PTDFF_MASK << PTDDFC_PTDFF_BITNUM)) | ((factor << PTDDFC_PTDFF_BITNUM) & PTDDFC_PTDFF_MASK));
		break;
	case DIO_PORTE :
		PTEDFC = (bool_t)((PTEDFC & ~(PTEDFC_PTEFF_MASK << PTEDFC_PTEFF_BITNUM)) | ((factor << PTEDFC_PTEFF_BITNUM) & PTEDFC_PTEFF_MASK));
		break;
	case DIO_PORTF :
		PTFDFC = (bool_t)((PTFDFC & ~(PTFDFC_PTFFF_MASK << PTFDFC_PTFFF_BITNUM)) | ((factor << PTFDFC_PTFFF_BITNUM) & PTFDFC_PTFFF_MASK));
		break;
	case DIO_PORTG :
		PTGDFC = (bool_t)((PTGDFC & ~(PTGDFC_PTGFF_MASK << PTGDFC_PTGFF_BITNUM)) | ((factor << PTGDFC_PTGFF_BITNUM) & PTGDFC_PTGFF_MASK));
		break;
	case DIO_PORTH :
		PTHDFC = (bool_t)((PTHDFC & ~(PTHDFC_PTHFF_MASK << PTHDFC_PTHFF_BITNUM)) | ((factor << PTHDFC_PTHFF_BITNUM) & PTHDFC_PTHFF_MASK));
		break;
	case DIO_PORTJ :
		PTJDFC = (bool_t)((PTJDFC & ~(PTJDFC_PTJFF_MASK << PTJDFC_PTJFF_BITNUM)) | ((factor << PTJDFC_PTJFF_BITNUM) & PTJDFC_PTJFF_MASK));
		break;
	default:
		break; /*gestione errore*/
	}
}

/**
Reads the digital filter factor

\param  [in]		port	port identificator

\return 	uint8_t

\retval		0 - 31

\attention \b Direct access to the peripheral

\author 	Jaroslav Musil
\date		2/02/2010
\version	1.0.0

\note		\par \b Nota 1:
			to be used in the debug only
			\par \b Nota 2:
			Target dependent

*/
inline uint8_t HAL_GetMcuIODigitalFilterFactorPort(uint8_t port)
{
	uint8_t PortFactor;

	switch (port)
	{
	case DIO_PORTA :
		PortFactor = (bool_t)((PTADFC & PTADFC_PTAFF_MASK) >> PTADFC_PTAFF_BITNUM);
		break;
	case DIO_PORTB :
		PortFactor = (bool_t)((PTBDFC & PTBDFC_PTBFF_MASK) >> PTBDFC_PTBFF_BITNUM);
		break;
	case DIO_PORTC :
		PortFactor = (bool_t)((PTCDFC & PTCDFC_PTCFF_MASK) >> PTCDFC_PTCFF_BITNUM);
		break;
	case DIO_PORTD :
		PortFactor = (bool_t)((PTDDFC & PTDDFC_PTDFF_MASK) >> PTDDFC_PTDFF_BITNUM);
		break;
	case DIO_PORTE :
		PortFactor = (bool_t)((PTEDFC & PTEDFC_PTEFF_MASK) >> PTEDFC_PTEFF_BITNUM);
		break;
	case DIO_PORTF :
		PortFactor = (bool_t)((PTFDFC & PTFDFC_PTFFF_MASK) >> PTFDFC_PTFFF_BITNUM);
		break;
	case DIO_PORTG :
		PortFactor = (bool_t)((PTGDFC & PTGDFC_PTGFF_MASK) >> PTGDFC_PTGFF_BITNUM);
		break;
	case DIO_PORTH :
		PortFactor = (bool_t)((PTHDFC & PTHDFC_PTHFF_MASK) >> PTHDFC_PTHFF_BITNUM);
		break;
	case DIO_PORTJ :
		PortFactor = (bool_t)((PTJDFC & PTJDFC_PTJFF_MASK) >> PTJDFC_PTJFF_BITNUM);
		break;
	default:
		break; /*gestione errore*/
	}
	return (bool_t)(PortFactor != 0);
}

#endif /* DIO_DEBUG */
bool_t DIO_IsNotDummyIO(const IO_ModuleIOMapType* ptr);
#endif /* #if defined(HAL_DIO_SUPPORT) */
/*@}*/
/* ********************************************************************************************** */
/*                                  END DIO Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN TMR Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. Timers 
 *
 */
/*@{*/

#if defined(HAL_TIMER_SUPPORT)

/** Define TMR Capability */
#define _HAL_TMR_CAPABILITY

#define TMR_PIN(x,y)\
{\
(DIO_PortAddressBasicType)&PT##x##D,(DIO_ModuleMapMaskType)PT##x##D_PT##x##D##y##_MASK \
}

static const TMR_CNTAddressType TmrCntMap[NUM_HW_CNTR]=
{
	(TMR_CNTAddressType)&FTM2SC,
	(TMR_CNTAddressType)&FTM1SC,
	(TMR_CNTAddressType)&TPM3SC
};

static const uint8_t TimerClockGate[NUM_HW_CNTR]=
{
	SCGC1_FTM2_MASK,
	SCGC1_FTM1_MASK,
	SCGC1_TPM3_MASK
};


typedef struct TMR_Map_tag
{
	DIO_PortAddressBasicType PortAddress; /**< Address of the Port Data Register relaated to the port pin */
	DIO_ModuleMapMaskType PinMask;        /**< Port pin mask  */
} TMR_MapType;

#define BEGIN_MODULE_TMR_MAP(n)    static const TMR_MapType TmrChToPinMap[n]={

#define END_MODULE_TMR_MAP()       };

/* Table of mapped channels */
BEGIN_MODULE_TMR_MAP(NUM_HW_CNTR_CH)
        TMR_PIN(F,4),
        TMR_PIN(F,5),
        TMR_PIN(J,7),
        TMR_PIN(J,6),
        TMR_PIN(E,2),
        TMR_PIN(E,3),
        TMR_PIN(F,0),
        TMR_PIN(F,1),
        TMR_PIN(F,2),
        TMR_PIN(F,3),
        TMR_PIN(B,0),
        TMR_PIN(B,1)
        END_MODULE_TMR_MAP()

        /* Don't remove this pragma otherwise the structures will be aligned and incompatible to the registers */
#pragma options align=packed

        /* Timer counter structure */
        typedef struct
{
	uint8_t sc;
	uint16_t cnt;
	uint16_t mod;
} tmrcntreg_struct_t;

/* Timer channel structure */
typedef struct
{
	uint8_t sc;
	uint16_t v;
} tmrchreg_struct_t;


#pragma options align=reset

inline bool_t CheckClearSourceIdFailed(uint8_t clearingSourceId)
{
	if(TMR_CLEAR_SOURCE_IS_INTERNAL==clearingSourceId)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

inline void  TMR_ClockGate(uint8_t hwCntrlId, bool_t gate)
{
	uint8_t scg1Mask;
	scg1Mask = TimerClockGate[hwCntrlId];
	if ( TRUE == gate )
	{
		SCGC1 |= scg1Mask;
	}
	else
	{
		SCGC1 &= (~scg1Mask);
	}
}


void TMR_SetPrescaler(TMR_CNTAddressType TimerID, uint8_t prescaler);

/* Prescaler to the M.C.U. Reset Value */
void TMR_SetPrescalerDfltValue(TMR_CNTAddressType TimerID);

void TMR_SetCountDir( TMR_CNTAddressType pTmr_cnt, uint8_t direction ) ;
/* Counting Dir. to the M.C.U. Reset Value */
void TMR_SetCountDirDfltValue(TMR_CNTAddressType tmrId);

void TMR_SetModulo(TMR_CNTAddressType tmrID, Period_ValueType period, uint8_t clrSrcId);

/* Set Timer Counter Modulo Reg. to the default value */
void TMR_SetModuloDfltValue(TMR_CNTAddressType tmrId, uint8_t clrSrcId);

Period_ValueType TMR_GetModulo(TMR_CNTAddressType tmdID, uint8_t channel);

void TMR_SetCounter( TMR_CNTAddressType tmrID, uint16_t value);

/* Set Timer Counter to the default value */
void TMR_SetCounterDfltValue(TMR_CNTAddressType tmrID);

IO_HwTimerType TMR_GetCounter( TMR_CNTAddressType tmrID);

void TMR_SetClockSource(TMR_CNTAddressType tmrID, uint8_t source);
void TMR_StopClockSource( TMR_CNTAddressType tmrID);

void TMR_EnableTI( TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId);
void TMR_DisableTI(TMR_CNTAddressType tmrID, TMR_CHAddressType clrSrcId);

uint8_t TMR_ReadTOF(TMR_CNTAddressType tmrID);
bool_t TMR_GetTOF(  TMR_CNTAddressType tmrID, uint8_t channelId);
void   TMR_ClearTOF(TMR_CNTAddressType tmrID, TMR_CHAddressType ch);

/* Set a channel in Input Capture Mode */
void TMR_SetChannelInputChapture( TMR_CHAddressType chId);

#define TMR_SetIniPinState(invertion)

/* Set a channel in Output Compare Mode (Software compare only) */
void TMR_SetChannelOutputCompare( TMR_CHAddressType chID);

void TMR_SetChannelPWM( TMR_CHAddressType chID);


uint8_t TMR_GetChannelEdgeMode( TMR_CHAddressType chID);
void    TMR_ToggleChannelEdgeMode( TMR_CHAddressType chID);
void    TMR_SetChannelEdgeMode( TMR_CHAddressType chID, uint8_t mode, bool_t inversion);


void TMR_SetValue(TMR_CHAddressType chID, Period_ValueType value); 

Period_ValueType TMR_GetChValue( TMR_CHAddressType chID );

void TMR_EnableCHI( TMR_CHAddressType chID);
void TMR_DisableCHI( TMR_CHAddressType chID);

#define TMR_ReadCHF(channel) (((channel) -> sc & TPM3C0SC_CH0F_MASK) != 0)

/*
 * Function: TMR_ClearCHF
 * Description: This function clears the channel interrupt flag both capture and match mode
 * Note: Used in ICU (input capture Unit)
 * Status:
 */
void TMR_ClearCHF( TMR_CHAddressType channel);

void TMR_SetThreshold1( TMR_CHAddressType chID, Period_ValueType value);

/* This M.C.U. doesn't support Threshold2 config. */
#define Threshold2SetUpUnsupported(channel) 1
#define TMR_SetThreshold2(channel, value)   {}

/* Check if Output Compare Mode is supported */
bool_t OutputCompareModeUnsupported(uint8_t mode, bool_t inversion);

void TMR_SetChannelOutputMode(TMR_CHAddressType chID, uint8_t mode, bool_t inversion);

#if 0 /* MS */
inline Period_ValueType TMR_GetDelta(tmrcntreg_struct_t *pTmr_cnt, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR)
{
    (void) pTmr_cnt;
    return ((Period_ValueType) ((uint16_t) CntrVal_CURR - (uint16_t) CntrVal_OLD ));
}
#endif
Period_ValueType TMR_GetDelta(TMR_CNTAddressType trmId, Period_ValueType CntrVal_OLD, Period_ValueType CntrVal_CURR);

void   TMR_DeInitChannel( TMR_CHAddressType pChannel);

void TMR_SetTimeOn( TMR_CHAddressType chID, Period_ValueType tOn);
void TMR_GetTimeOn( TMR_CHAddressType chID, Period_ValueType *tOn);


void   TMR_SetChannelPulses(TMR_CHAddressType chId, uint8_t inverted);
bool_t TMR_CheckClearSourceIdFailed(TMR_CNTAddressType tmrID, uint8_t clrSrcId); /* TODO */

#endif /* #if defined(HAL_TIMER_SUPPORT) */
/*@}*/
/* ********************************************************************************************** */
/*                                  END TMR Class HAL Support                                     */
/* ********************************************************************************************** */

/* ********************************************************************************************** */
/*                                  BEGIN IRQ Class HAL Support                                   */
/* ********************************************************************************************** */
/** @name H.A.L. I.R.Q. 
 *
 */
/*@{*/

#if defined(HAL_IRQ_SUPPORT)
/*
- Bus clock to the IRQ module is enabled.
- Disable IRQ Interrupt
- IRQ pin function is enabled
- Pulling
- Edge or Level Plus Edge
- Init. Sync
- Acknowledges interrupt request events
- IRQ Interrupt Enable

*/
#define IRQPDD_POS  6
#define IRQEDG_POS  5
#define IRQMOD_POS  0

/*Il parametro irq_id nelle macro seguenti è inserito solo per compatibilità ma non utilizzato*/
#define IRQ_Acknowledge(irq_id){IRQSC_IRQACK = 1;}
#define IRQ_Init(irq_id)\
{\
    SCGC2_IRQ = 1;\
    IRQSC_IRQIE = 0;\
    IRQSC |= (IRQSC_IRQPE_MASK|((IRQ_PULL^1)<<IRQPDD_POS)|(IRQ_EDGE<<IRQMOD_POS)|(IRQ_FIRST_SYNC<<IRQEDG_POS));\
    IRQSC_IRQACK = 1;\
    IRQSC_IRQIE = 1;\
}
#define IRQ_SetEdge(edge,irq_id)\
{\
  if (edge){IRQSC |= IRQSC_IRQEDG_MASK;}\
  else{IRQSC &= (~IRQSC_IRQEDG_MASK);}\
}
inline bool_t IRQ_GetEdge(uint8_t irq_id)
{
  (void)irq_id;
	return(bool_t)((IRQSC & IRQSC_IRQEDG_MASK)?1:0);
}
/*
- Clear Interrupt Request Status and Control Register
- Disable Bus clock to the IRQ module.
*/
#define IRQ_DeInit(irq_id)\
{\
    IRQSC = 0x00;\
    SCGC2_IRQ = 0;\
}
#define IRQ_IntEnable(enable, irq_id)\
{\
if (enable){IRQSC_IRQACK = 1;IRQSC_IRQIE = 1;}\
else{IRQSC_IRQIE = 0;IRQSC_IRQACK = 1;}\
}
inline bool_t IRQ_pinIsHigh(uint8_t irq_id)
{
  (void)irq_id;    
	IRQSC_IRQACK = 1;
	return(bool_t)((IRQSC & IRQSC_IRQF_MASK)?1:0);

}
#define IRQ_pinIsLow IRQ_pinIsHigh
#endif /* #if defined(HAL_IRQ_SUPPORT) */
/*@}*/
/* ********************************************************************************************** */
/*                                   END IRQ Class HAL Support                                    */
/* ********************************************************************************************** */

/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* General               */
/* ********************* */
/** nop */
#define _NOP_() asm("nop")

/** Genera un reset se opportunamento definita in config.h
\code
extern void IlopCmd(void);
#define SW_RESET_MCU IlopCmd
\endcode
*/
#ifndef SW_RESET_MCU
#define SW_RESET_MCU()
#endif

/**
\def DELAY_MICROSEC
converte us in cicli nop in base alla frequenza di bus.

\todo rivedere il calcolo.
*/
#if (BUS_FREQUENCY_Hz == 10000000)
/* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
// 4 = us di ritardo fisso dovuto alla chiamata della routine
// *1000/2614 = 2,614us multiplier   which is the loop duration
#define DELAY_MICROSEC(x)	((((long)x-3)*1000)/2614)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */

#if (BUS_FREQUENCY_Hz == 16000000)
/* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
// 4 = us di ritardo fisso dovuto alla chiamata della routine
// *1000/2614 = 2,614us multiplier   which is the loop duration
#define DELAY_MICROSEC(x)	((((long)x-3)*1000)/1634)
#endif /* (BUS_FREQUENCY_Hz == 10000000) */


#if (BUS_FREQUENCY_Hz == 20000000)
/* macro di calcolo del valore corretto da passare alla routine di delay in microsecondi */
// 4 = us di ritardo fisso dovuto alla chiamata della routine
// *1000/1307 = 1,307us multiplier   which is the loop duration
#define DELAY_MICROSEC(x)	((((long)x-2)*1000)/1307)
#endif /* (BUS_FREQUENCY_Hz == 20000000) */
/* ********************* */
/* General               */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


/* ********************* */
/*        BEGIN          */
/* ********************* */
/* ********************* */
/* IO                    */
/* ********************* */

/* ****************** */
/* Defines and Macros */
/* ****************** */





/* *************************** */
/* Typedef, Structs and Unions */
/* *************************** */



/* ****************** */
/* Exported variables */
/* ****************** */
extern const boot_short_info_t BootShortInfo  @0x00000410;

/* ******************* */
/* Function prototypes */
/* ******************* */

/* ********************* */
/* IO                    */
/* ********************* */
/* ********************* */
/*          END          */
/* ********************* */


#endif /* __HAL_FREESCALE_MCF51_AG128_H */

/*@}*/

/* *********** */
/* End of file */
/* *********** */