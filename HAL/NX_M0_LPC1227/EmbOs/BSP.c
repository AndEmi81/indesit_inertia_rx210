/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2011  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 3.82s                                            *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : BSP.c
Purpose : BSP for LPC122x-SK evaluation board
--------  END-OF-HEADER  ---------------------------------------------
*/

#define BSP_C
#include "BSP.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** SFRs used for LED-Port **************************************/

#define GPIO1_BASE_ADDR  0x50010000

#define AHBCLKCTRL (*(volatile unsigned int*) (0x40048080))


/****** Assign LEDs to Ports ****************************************/

/*      LED0 = LED2 on eval board = PIO 1.4 */
/*      LED1 = LED1 on eval board = PIO 1.5 */

#define LED_PORT_BASE_ADDR GPIO1_BASE_ADDR
#define LED0_BIT           (4)
#define LED1_BIT           (5)
#define LED0_MASK          (1uL << LED0_BIT)
#define LED1_MASK          (1uL << LED1_BIT)
#define LED_MASK_ALL       (LED0_MASK | LED1_MASK)


#define LED_PORT_MASK (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x00))
#define LED_PORT_OUT  (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x08))
#define LED_PORT_SET  (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x0C))
#define LED_PORT_CLR  (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x10))
#define LED_PORT_NOT  (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x14))
#define LED_PORT_DIR  (*(volatile unsigned int*) (LED_PORT_BASE_ADDR + 0x20))

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  LED_PORT_DIR  |= LED_MASK_ALL;      // Set LED ports to output
  LED_PORT_MASK &= ~(LED_MASK_ALL);   // Enable SET / CLR / NOT access for LED ports
  LED_PORT_SET   = LED_MASK_ALL;      // Initially switch off LEDs (active low)
}

/*********************************************************************
*
*       LED switching routines
*       LEDs are switched on with low level on port lines
*/
void BSP_SetLED(int Index) {  // Switch on LED (active low)
  if (Index == 0) {
    LED_PORT_CLR = LED0_MASK;
  } else if (Index == 1) {
    LED_PORT_CLR = LED1_MASK;
  }
}

void BSP_ClrLED(int Index) {  // Switch off LED (active low)
  if (Index == 0) {
    LED_PORT_SET = LED0_MASK;
  } else if (Index == 1) {
    LED_PORT_SET = LED1_MASK;
  }
}

void BSP_ToggleLED(int Index) {  // Toggle LED
  if (Index == 0) {
    LED_PORT_NOT = LED0_MASK;
  } else if (Index == 1) {
    LED_PORT_NOT = LED1_MASK;
  }
}

/****** End Of File *************************************************/
