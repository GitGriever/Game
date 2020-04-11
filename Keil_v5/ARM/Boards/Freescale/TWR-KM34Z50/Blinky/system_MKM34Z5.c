/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    MKMxxZxxCxx5RM Rev. 4, June 2013
**     Version:             rev. 1.5, 2015-07-23
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2015 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2012-06-21)
**         Initial version.
**     - rev. 1.1 (2012-09-04)
**         Update according to reference manual rev. 2, draft A.
**     - rev. 1.2 (2012-10-18)
**         Update according to reference manual rev. 2.
**     - rev. 1.3 (2013-04-05)
**         Changed start of doxygen comment.
**     - rev. 1.4 (2013-05-06)
**         Update according to reference manual rev. 4.
**     - rev. 1.5 (2015-07-23)
**         Update according to final version of reference manual rev. 4.
**         - Added CLOCK_SETUP '0': by ARM 
** ###################################################################
*/

/*!
 * @file MKM34Z5
 * @version 1.5
 * @date 2015-07-23
 * @brief Device specific configuration file for MKM34Z5 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MKM34Z5.h"

#define DISABLE_WDOG    1

#define CLOCK_SETUP                     (0)


#if (CLOCK_SETUP == 0)
/* 
   FLL Engaged External (DCO Range: 48 MHz). FLL clocked by 32 kHz RTC 
   Oscillator Clock Source (32.768kHz external crystal).
   Core Clock = 48 MHz, Bus Clock = 24 MHz, Flash Clock = 24 MHz
*/

#define DEFAULT_SYSTEM_CLOCK            48000000u                              /* Default System clock value */

/******************************************************************************
 * MCG value definitions
 ******************************************************************************/
#define MCG_C1_VAL ((MCG_C1_CLKS(0x00))|(MCG_C1_FRDIV(0x00)))
#define MCG_C2_VAL ((MCG_C2_LOCRE0_MASK)|(MCG_C2_RANGE0(0x00))|(MCG_C2_HGO0_MASK))
#define MCG_C4_VAL ((MCG_C4_DMX32_MASK)|(MCG_C4_DRST_DRS(0x01)))
#define MCG_C6_VAL ((MCG_C6_CHGPMP_BIAS(0x00)))
#define MCG_SC_VAL ((MCG_SC_FCRDIV(0x00)))
#define MCG_C7_VAL ((MCG_C7_OSCSEL_MASK))
#define MCG_C8_VAL ((MCG_C8_LOCRE1_MASK))

#endif
/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if (DISABLE_WDOG)
  /* Disable the WDOG module */
  /* WDOG_UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = (uint16_t)0xC520u;     /* Key 1 */
  /* WDOG_UNLOCK : WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK  = (uint16_t)0xD928u;    /* Key 2 */
  /* WDOG_STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = (uint16_t)0x01D2u;
#endif /* (DISABLE_WDOG) */



#if (CLOCK_SETUP == 0)
  /* initialize SIM module                                                   */
  /* System clock : Bus clock : Flash clock */
  /*             2:          1:           1 */
  SIM->CLKDIV1 = (1 << SIM_CLKDIV1_SYSCLKMODE_SHIFT) | (0 << SIM_CLKDIV1_SYSDIV_SHIFT);

  MCG->SC = MCG_SC_VAL;
  MCG->C8 = MCG_C8_VAL;
  /* Select MCG PLL 32Khz Reference Clock Select and MCG OSC Clock Select     */
  MCG->C7 = MCG_C7_VAL;
  /* External Reference clock setup                                           */
  MCG->C2 = MCG_C2_VAL;
  
  if (MCG->C2 & MCG_C2_EREFS0_MASK) { 
    while (((MCG->S&MCG_S_OSCINIT0_MASK)>>MCG_S_OSCINIT0_SHIFT)!=              \
      ((MCG->C2&MCG_C2_EREFS0_MASK)>>MCG_C2_EREFS0_SHIFT));                    \

  }  
  /* Setup C1 register keeping MCGOUTCLK clocked from internal reference clock*/
  MCG->C1 = (MCG_C1_VAL & ~MCG_C1_CLKS_MASK) | MCG_C1_CLKS(1); 
 // if (MCG_C1 &   MCG_C1_IREFS_MASK) { WAIT_FOR_STATUS_FLAG (MCG_C1, IREFS, IREFST); } 
  MCG->C4 = (MCG->C4 & (MCG_C4_FCTRIM_MASK | MCG_C4_SCFTRIM_MASK)) | MCG_C4_VAL;
  while ((MCG->C4 & MCG_C4_DRST_DRS_MASK) != (MCG_C4_VAL & MCG_C4_DRST_DRS_MASK)); 
  MCG->C6 = MCG_C6_VAL;  

  /* Switch MCGOUTCLK clock source                                            */
  MCG->C1 = MCG_C1_VAL;   
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT)!=               \
    ((MCG->C1 & MCG_C1_CLKS_MASK) >> MCG_C1_CLKS_SHIFT));                   \

  if (MCG->C1 & MCG_C1_IRCLKEN_MASK) { 
    while (((MCG->S & MCG_S_IRCST_MASK) >> MCG_S_IRCST_SHIFT)!=             \
      ((MCG->C2 & MCG_C2_IRCS_MASK) >> MCG_C2_IRCS_SHIFT));                 \

  }
#endif
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {
}
