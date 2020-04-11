/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *            __ADC_IRQ   - ADC works in Interrupt mode
 *                        - ADC works in polling mode (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "TMPM369.h"                    /* Toshiba TMPM369 definitions        */
#include "ADC.h"

uint16_t AD_last;                            /* Last converted value          */
uint8_t  AD_done = 0;                        /* AD conversion done flag       */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {


  TSB_PJ->PUP  &= ~0x01;                     /* Disable Pull-up for PJ0       */
  TSB_PJ->IE   &= ~0x01;                     /* Set PJ0 as Analog input pin   */

  /* Setup A/D converter                                                      */
  TSB_CG->SYSCR &= ~(1UL << 20);             /* enable ADC clock              */

  TSB_ADB->MOD1  =  (1UL << 7);              /* DACON = 1                     */
  TSB_ADB->MOD6  =  (2UL << 0);              /* Reset ADC unit B              */
  TSB_ADB->MOD6  =  (1UL << 0);              /* Reset ADC unit B              */
  __NOP();__NOP();__NOP();__NOP();__NOP();   /* delay                         */

  TSB_ADB->CLK   =  ((4UL << 0) |            /* ADCLK = fc/16                 */
                     (1UL << 4)  );          /* ADSH = ADCLK * 20             */
  TSB_ADB->MOD1  =  ((1UL << 7) |            /* DACON = 1                     */
                     (1UL << 5) |            /* RCUT = 1                      */
                     (1UL << 0)  );          /* ADHWE = 1                     */
  TSB_ADB->MOD2  =  4;                       /* Input Channel: AINB4          */

#ifdef __ADC_IRQ
  NVIC_EnableIRQ(INTADB_IRQn);               /* enable ADC Interrupt          */
#endif
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {
  TSB_ADB->MOD0 |=  (1UL << 0);              /* Start ADC conversion          */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {
  ;
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

#ifndef __ADC_IRQ
  while (!(TSB_ADB->MOD5 & (1UL << 1)));     /* Wait until finished           */
  AD_last = (TSB_ADB->REG04 & ADC_VALUE_MAX);  /* Store converted value       */

  AD_done = 1;
#endif

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
#ifdef __ADC_IRQ
void INTADB_IRQHandler(void) {
  volatile uint32_t adstat;

  adstat = TSB_ADB->MOD5;
  AD_last = (TSB_ADB->REG04 & ADC_VALUE_MAX);      /* Store converted value   */

  AD_done = 1;
}
#endif
