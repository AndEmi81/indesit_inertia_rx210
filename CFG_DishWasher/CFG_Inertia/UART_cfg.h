#ifndef UART_CFG_H_
#define UART_CFG_H_

#define HAL_UART_SUPPORT

/* UART driver configuration */
#define UART_AUTOMATIC_CIRCULAR_BUFFER FALSE

#define OCUART_BUFFER_LSIZE	                        (16)            /* circular buffer*/

#if defined(FREESCALE_MCF51_AG128)
	#define UART_OCDMA_SUPPORT
#endif

#if defined(RN_RX200)
	#define HAL_OPTION_UART							12
#elif defined(FREESCALE_MCF51_AG128) 
	#define HAL_OPTION_UART							1
#endif 


#define HAL_OPTION_UART_ASYN_MODE					TRUE
#define HAL_OPTION_UART_DATALEN						8
#define HAL_OPTION_UART1_PARITY						UART_PARITY_NONE
#define HAL_OPTION_UART_USE_PARITY					FALSE
#define HAL_OPTION_UART_STOP						1
#define HAL_OPTION_UART_BAUDRATE_DIVISOR			0
#define HAL_OPTION_UART_BAUDRATE					9600

/*===========================  End UART Support ================================*/

#endif /* UART_CFG_H_ */
