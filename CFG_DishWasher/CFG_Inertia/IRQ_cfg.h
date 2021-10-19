#ifndef __IRQPLTFCFG_H
#define __IRQPLTFCFG_H

#define HAL_IRQ_SUPPORT
/* IRQ Detection Mode */
#define IRQ_ON_EDGE_ONLY                    (0)        /* IRQ event on falling edges or rising edges only */
#define IRQ_ON_EDGE_AND_LEVEL               (1)        /* IRQ event on falling edges and low levels or on rising edges and high levels */
/* IRQ First Sync */
#define IRQ_ON_NEGATIVE_EDGE                (bool_t)0
#define IRQ_ON_POSITIVE_EDGE                (bool_t)1
/* Zero Crossing Circuit Inverting ? */
#define ZERO_CROSSING_CIRCUIT_INVERTING     TRUE

#if (ZERO_CROSSING_CIRCUIT_INVERTING == TRUE)
#define VPEAK_EDGE                          IRQ_ON_NEGATIVE_EDGE
#else
#define VPEAK_EDGE                          IRQ_ON_POSITIVE_EDGE
#endif  /* #if (ZERO_CROSSING_CIRCUIT_INVERTING == TRUE) */

#if defined(FREESCALE_MCF51_AG128)
#define IRQ_ZC_ID 	0 /*  IRQ_ZC_ID non used on AG128, there is custom HW management */

/* pulling enable/disable, 1 -> enable, 0 -> disable  */
#define IRQ_PULL                            (1)
#define IRQ_EDGE                            IRQ_ON_EDGE_AND_LEVEL
#define IRQ_FIRST_SYNC                      IRQ_ON_POSITIVE_EDGE
/* Filtering: number of times that the pin must be found high
   if the IRQ is sync on rising edge or low, if it is synchronized on the negative edge,
   to accept the signal */
#define IRQ_FILTER                          5u
/*
#define ZC_IO_MODULE_MAP_SIZE	1
#define ZC_IO_MODULE_MAP_NAME   ZcIOmap
*/

#endif /* #if defined(FREESCALE_MCF51_AG128) */

#if defined(NXP_LPC12xx_1227)
/* pulling enable/disable, 1 -> enable, 0 -> disable  */
#define IRQ_PULL                            (1)
#define IRQ_EDGE                            IRQ_ON_EDGE_AND_LEVEL
#define IRQ_FIRST_SYNC                      IRQ_ON_POSITIVE_EDGE
/* Filtering: number of times that the pin must be found high
   if the IRQ is sync on rising edge or low, if it is synchronized on the negative edge,
   to accept the signal */
#define IRQ_FILTER                        5u
#endif /* #if defined(NXP_LPC12xx_1227) */

#if defined(RN_RX200)
/* pulling enable/disable, 1 -> enable, 0 -> disable  */
#define IRQ_PULL    1
#define IRQ_EDGE                          IRQ_ON_EDGE_AND_LEVEL
#define IRQ_FIRST_SYNC                    IRQ_ON_POSITIVE_EDGE
/* Filtering: number of times that the pin must be found high
	   if the IRQ is sync on rising edge or low, if it is synchronized on the negative edge,
	   to accept the signal */
#define IRQ_FILTER                        5u

#endif /* defined(RN_RX200) */

#define LIB_ZC_SET_OFFSET_VPEAK_MEASURE(usOffset)               Main_ActivateMeasureLiveWithOffset((OS_U32)usOffset)
#define LIB_ZC_SET_OFFSET_ANALOGINPUTS_MEASURE(usOffset)        Main_ActivateAnalogMeasureWithOffset((OS_U32)usOffset)
#define LIB_ZC_SET_OFFSET_OUTPUTS(usOffset, usPeriod, times)    Main_ActivateOutputs((OS_U32)usOffset, (OS_U32)usPeriod, (OS_I8)times)

#if defined(RN_RX200)
#if defined(RN_RX200_R5F5210) || defined(RN_RX200_R5F52Z0)
#if defined(EDGE_MODULE_LP1_0258_01) || defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)
/* configured on pin28, PH1 */
#define IRQ_ZC_ID 	RX210_IRQ0_PH1
#define ZC_PIN			(28)
#define ZC_PIN_IDX		(ZC_PIN-1)

#elif defined(CASTELLATED_MODULE_LP1_0241_01)
#define IRQ_ZC_ID 	RX210_IRQ0_PH1
/*RX210_IRQ_PB1*/
#define ZC_PIN			(50)
#define ZC_PIN_IDX		(ZC_PIN-1)

#elif defined(TEST_MODULE)
#define IRQ_ZC_ID 	RX210_IRQ_PB1
#else
#error "Module not yet supported or check you pre-processor settings."
#endif /*MODULE*/

#else
#error "MCU not yet supported or check you pre-processor settings."
#endif /* defined(RN_RX200_R5F5210) || defined(RN_RX200_R5F52Z0)*/

#endif /* defined(RN_RX200) */
#endif /* __IRQPLTFCFG_H */
