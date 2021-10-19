#include "Config.h"
#include "StandardTypes.h"
#include "NX_M0_LPC1227_reg.h"
#include "IEC60335_typedefs.h"
#include "IEC60335_B_WDTest.h"

#include "LIB_Trace.h"

static UINT32 resetSourceBackup; 
static UINT32 wdTestStatus;


INT32  IEC60335_WDTest(void)
{
UINT32 resetSrc;
//  TRACE_LED(1, LED_GREEN);
//  TRACE_LED(0, LED_RED);
//  TRACE_LED(0, LED_YELLOW);
  __disable_interrupt();
  /* enable clock just to be sure */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1u << 15);
  __enable_interrupt();
  
  /* get the reset source */
  resetSrc = LPC_SYSCON->SYSRESSTAT;
  
  /* if reset source is POR then start to perform a wd test at power on */
  if((resetSrc & RESET_POR))
  {
    /* power on reset */
    resetSourceBackup = resetSrc;        /* backup the reset source */
    LPC_SYSCON->SYSRESSTAT |= RESET_POR;  /* clear the reset source */

    wdTestStatus = WDTEST_STARTED;

    LPC_WDT->TC = WD_TICKS;             /* configure timeout */
    LPC_WDT->MOD = WD_ENABLE_RESET;     /* enable wd, reset at expire, clear TOF  */
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
    //TRACE_ERROR_STOP(LED_GREEN); //MS
    while(1);                           /* wait for expiration */
  }
  else if(resetSrc & RESET_WD)
  {
    /* if we got a wd reset, then check if we are under test */
    if(wdTestStatus == WDTEST_STARTED)
    {
      /* we are performing the test and got the wd timeout */
      LPC_WDT->MOD = 0x0;                 /* disable wd */
      LPC_WDT->FEED = 0xAA;
      LPC_WDT->FEED = 0x55;

      resetSourceBackup = resetSrc;
      LPC_SYSCON->SYSRESSTAT |= RESET_WD; /* clear the reset source */

      wdTestStatus = WDTEST_COMPLETE;           /* set to completed */
//      TRACE_LED(0, LED_GREEN);
//      TRACE_LED(0, LED_RED);
//      TRACE_LED(0, LED_YELLOW);
      return (IEC60335_testPassed);      
    }
  }

  
  
  
  
  
  /* in other cases, restart the test when function is called again */
  resetSourceBackup = resetSrc;
  LPC_SYSCON->SYSRESSTAT |= resetSrc; /* clear the reset source */
  
  wdTestStatus = WDTEST_STARTED;
  
  
  
  
  

  
  
  
    LPC_WDT->TC = WD_TICKS;             /* configure timeout */
    LPC_WDT->MOD = WD_ENABLE_RESET;     /* enable wd, reset at expire, clear TOF */
    LPC_WDT->FEED = 0xAA;
    LPC_WDT->FEED = 0x55;
//  TRACE_ERROR_STOP(LED_RED); //MS
    
    /* invalid operation to avoid waiting WD expiration*/
    LPC_WDT->FEED = 0xAA;               
    LPC_WDT->TC = 0;
  
    //while(1);                           /* wait for expiration */
  
    return (IEC60335_testFailed);       /* dummy return, should never get here */      
}

