/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s):        - ADC works in Interrupt mode
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <HT32F175x_275x.h>
#include "ADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
  ADC Clock source = 9 MHz, conversion time = (15 + 1.5 + 12.5) / 9 MHz = 3.22 us
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  /* configure Pins used for ADC                                              */
  HT_CKCU->APBCCR0 |=  ((1UL <<  14 ) );  /* enable AFIO Clock                */

  HT_AFIO->GPACFGR &= ~((3UL <<  6*2) );  /* configure PA6 as GPIO            */
  HT_AFIO->GPACFGR |=  ((1UL <<  6*2) );  /* configure PA6 as ADC_IN6         */

  /* Setup and initialize ADC converter                                       */
  HT_CKCU->APBCFGR  =  ((3UL <<  16 ) );  /* ADC prescaler is 8 (72MHz/8)     */
  HT_CKCU->APBCCR1 |=  ((1UL <<  24 ) );  /* enable ADC Clock                 */

  HT_ADC->RST       =   (1UL <<   0 );    /* reset ADC                        */

  HT_ADC->CONV      =  ((0UL <<   0 ) |   /* one shot mode                    */
                        (0UL <<   8 ) |   /* sequence length = 1              */
                        (0UL <<  16 ) );  /* reg. chn. subgroup length = 1    */
  HT_ADC->LST[0]    =   (6UL <<   0 );    /* ADSEQ0 = ADC_IN6                 */

  HT_ADC->STR[0]    =   15;               /* sample time = 15 + 1.5 latency   */
  HT_ADC->TCR       =   (1UL <<   0 );    /* enable SW trigger                */

  HT_ADC->IM        =   (1UL <<  2  );    /* enable Regular Cycle EOC IRQ     */
  NVIC_EnableIRQ(ADC_IRQn);               /* enable ADC Interrupt             */
}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  HT_ADC->TSR    |=  (1UL <<  0);         /* Start A/D conversion             */ 
}

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void) {

  if (HT_ADC->IMASK & (1UL << 2)) {    /* ADC Regular Cycle EOC IRQ ?         */
    AD_last = HT_ADC->DR[0] & 0x0FFF;
    AD_done = 1;

    HT_ADC->ICLR &= ~(1UL << 2);       /* clear Regular Cycle EOC IRQ         */
  }

}
