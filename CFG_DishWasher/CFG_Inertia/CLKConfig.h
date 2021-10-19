
#ifndef __CLK_CONFIG_H_
#define __CLK_CONFIG_H_

#define HAL_CLK_SUPPORT
/* Select Internal Source Clock for oscillator otherwise Crystal/resonator or External Clock
   is selected */
#define CLK_INT_REF_CLOCK_ACTIVE


#if defined(FREESCALE_MCF51_AG128)
#define CLK_INT_CLOCK_FREQ  32768   /* range 31250 - 39062 Hz */
#define OCCLK_SetClockSync(freq)  OCCLK_SetFLLInternalClockSync(freq)
#endif

#if defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0)
#define OCCLK_SetClockSync(freq)  OCCLK_SetInternalClockSync(freq)
#define CLK_INT_CLOCK_FREQ  	20000000   /* RX210 Max Inernal Clock  */



#if !defined(CLK_INT_CLOCK_FREQ)
    #error "ERROR !!! You must specify the System Clock Frequency";
#endif

#endif /* defined (RN_RX200_R5F5210) || defined (RN_RX200_R5F52Z0) */

#endif /* __CLK_CONFIG_H_ */
