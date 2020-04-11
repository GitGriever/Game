/*----------------------------------------------------------------------------
 * Name:    CMP.c
 * Purpose: low level CMP functions
 * Note(s):
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

#include <LPC8xx.h>
#include "CMP.h"

uint16_t CMP_last;                              /* Last Ladder value       */

/*----------------------------------------------------------------------------
  Function that initializes CMP
 *----------------------------------------------------------------------------*/
void CMP_Init (void) {

  /* configure PIN P0.0 for ACMP_I1 */
  LPC_SYSCON->SYSAHBCLKCTRL |= (( 1UL <<  6) |  /* enable clock for GPIO      */
                                ( 1UL <<  7) ); /* enable clock for SWM       */

  LPC_IOCON->PIO0_0   &= ~( 3UL <<  3);         /* no pull up/down            */
  LPC_GPIO_PORT->DIR0 &= ~( 1UL <<  0);         /* configure GPIO as input    */

  LPC_SWM->PINENABLE0 &= ~( 1UL <<  0);         /* P0.0 is ACMP_I1            */

  /* configure CMP */
  LPC_SYSCON->PDRUNCFG      &= ~( 1UL <<  3);   /* Enable power to BOD block  */
  LPC_SYSCON->PDRUNCFG      &= ~( 1UL << 15);   /* Enable power to CMP block  */
  LPC_SYSCON->SYSAHBCLKCTRL |=  ( 1UL << 19);   /* Enable clock to CMP block  */

  LPC_SYSCON->PRESETCTRL    &= ~( 1UL << 12);   /* reset CMP */
  LPC_SYSCON->PRESETCTRL    |=  ( 1UL << 12);

  LPC_CMP->CTRL  =  (( 0UL <<  3) |             /* EDGESEL, Falling edges */
                     ( 0UL <<  6) |             /* COMPSA, output used directly */
                     ( 1UL <<  8) |             /* COMP_VP_SEL, ACMP_I1 */
                     ( 0UL << 11) |             /* COMP_VM_SEL, Voltage ladder output */
                     ( 0UL << 25)  );           /* HYS, none */
  LPC_CMP->LAD   =  (( 1UL <<  0) |             /* Voltage ladder enable */
                     (CMP_LAD_MID <<  1) |
                     ( 0UL <<  6)  );           /* LADREF, Supply pin VDD */

}


/*----------------------------------------------------------------------------
  get CMP Ladder value (according potentiometer position)
 *----------------------------------------------------------------------------*/
uint16_t CMP_GetLad (void) {
  uint32_t cnt, i;
  uint32_t ladVal;

  for (cnt = 1; cnt < 5; cnt++) {
    ladVal = (LPC_CMP->LAD >> 1) & CMP_LAD_MAX;

    if (LPC_CMP->CTRL & (1UL << 21)) {
      ladVal +=  (CMP_LAD_MID >> cnt);     /* increment ladder value */
    } else {
      ladVal -=  (CMP_LAD_MID >> cnt);     /* decrement ladder value */
    }
    LPC_CMP->LAD = ((ladVal & CMP_LAD_MAX) << 1) | 1;
    for (i = 0; i < 10; i++)  __NOP();
  }
  LPC_CMP->LAD = ((CMP_LAD_MID) << 1) | 1;

  return(ladVal);
}
