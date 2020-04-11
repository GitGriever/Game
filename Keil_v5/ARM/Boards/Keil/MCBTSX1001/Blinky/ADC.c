/*----------------------------------------------------------------------------
 * Name:    ADC.c
 * Purpose: low level ADC functions
 * Note(s): possible defines select the used ADC interface:
 *                        - ADC works in Interrupt mode (default)
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

#include <TSX1001.h>
#include "ADC.h"

uint16_t AD_last;                         /* Last converted value             */
uint8_t  AD_done = 0;                     /* AD conversion done flag          */

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_Init (void) {

  GPIO2->DATA[0].WORD |= ((1UL << 3) |      /* enable VREF0 */
                          (1UL << 4)  );    /* enable VREF1 */

  ADC->CFG0[0].WORD =    ADC_DECIMATE_1024
                       | ADC_CLK_XTALx1
                       | ADC_REF_VDDx0p5
                       | ADC_MOD_EN;

  NVIC_EnableIRQ(ADC_IRQn);               /* enable ADC Interrupt             */

}


/*----------------------------------------------------------------------------
  start AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StartCnv (void) {

  AD_done = 0;
  ADC->CFG0[0].WORD  |=   (1UL <<  0);    /* Start A/D conversion             */
}


/*----------------------------------------------------------------------------
  stop AD Conversion
 *----------------------------------------------------------------------------*/
void ADC_StopCnv (void) {

  ADC->CFG0[0].WORD  &=  ~(1UL <<  0);    /* Stop  A/D conversion             */
}


/*----------------------------------------------------------------------------
  get converted AD value
 *----------------------------------------------------------------------------*/
uint16_t ADC_GetCnv (void) {

  return(AD_last);
}


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is done
 *----------------------------------------------------------------------------*/
void ADC_IRQHandler(void) {
  uint32_t cfg0;

  cfg0 = ADC->CFG0[0].WORD;
  if (cfg0 & (1 << 15)) {                  /* ADC EOC interrupt?              */
     ADC->CFG0[0].WORD  = (cfg0 & 0x0000FFFE);    /* Stop  A/D conversion     */

//     AD_last = ((cfg0 >> 16) + 0x0000);         /* scale value to 0x0 .. 0xFFFF     */
     AD_last = ((cfg0 >> 16) + 0x8000);         /* scale value to 0x0 .. 0xFFFF     */
     AD_done = 1;

     ADC->CFG1.WORD  =  1;                 /* clear EOC interrupt             */
   }

}
