#ifndef __ZCV_PLTFCFG_H
#define __ZCV_PLTFCFG_H

/* please setup the hardware timer id used by Zero Crossing Voltage Module
in order to measure the grid frequency */

#if defined(FREESCALE_MCF51_AG128)
//#define ZCV_CH_ID       TMR_CH_ID_6
#endif

#if defined(RN_RX200)
#if defined(EDGE_MODULE_LP1_0258_01) || defined(EDGE_MODULE_LP1_0258_02) || defined(EDGE_MODULE_LP1_0258_04)
	/* configurato su pin28, PH1*/
	#define ZCV_HW_TMR_ID   	0
	#define MOTOR_HW_TMR_ID		3 /*MS timer associato al pin per pilotare il motore a collettore PIN 23 */
#elif defined(CASTELLATED_MODULE_LP1_0241_01)

/*MS prova configurazione ZC su ICU pin 38 */
#if 0
	#define ZCV_ON_ICU
	#define ZCV_CH_ID	7
	#define ZCV_PIN		37 /*PIN 38 */
	#define ZCV_HW_TMR_ID   	4
	#define MOTOR_HW_TMR_ID		4 /*MS timer associato al pin per pilotare il motore a collettore PIN 36*/
#else
	#define ZCV_HW_TMR_ID   	0
	#define MOTOR_HW_TMR_ID		4 /*MS timer associato al pin per pilotare il motore a collettore PIN 36*/
	/* Static binding of OCIRQ to ZC Lib */
	#define CONFIG_OCIRQ_STATIC_CFG 1

#endif /*MS PROVA */

#elif defined(TEST_MODULE)
	#define ZCV_HW_TMR_ID   	0
	#define MOTOR_HW_TMR_ID		4 /*MS timer associato al pin per pilotare il motore a collettore PIN 36*/
#else
#error "Module not yet supported or check you pre-processor settings."
#endif /* Module)*/
#endif /* defined(RN_RX200) */

/** Number of Zero Crossing Edges for syncronization's procedure */
#define FLT_NUM_ZC		10
/** To establish nominal frequency */
#define DELTA_FRQ       2
#endif /* __ZCV_PLTFCFG_H */
