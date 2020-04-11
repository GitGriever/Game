/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
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

#include <stdio.h>
#include <SAM4L.h>
#include "Serial.h"
#include "LED.h"


volatile uint32_t msTicks = 0;                      /* counts 10 ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 10 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t idx  = -1, dir = 1;

  SER_Init();
  LED_Init();

  /* set Flash Waite State to maximum */
  HFLASHC->FLASHCALW_FCR = FLASHCALW_FCR_FWS;

  /* set system clock to 40MHz (80 MHz RC Oscillator / 2) */
  SCIF->SCIF_UNLOCK  = SCIF_UNLOCK_KEY(0xAAu) |
                       SCIF_UNLOCK_ADDR((uint32_t)&SCIF->SCIF_RC80MCR - (uint32_t)SCIF);
  SCIF->SCIF_RC80MCR = SCIF_RC80MCR_EN;
  while((SCIF->SCIF_RC80MCR & SCIF_RC80MCR_EN) != SCIF_RC80MCR_EN);

  PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |   /* Disable write protect           */
                  PM_UNLOCK_ADDR((uint32_t)&PM->PM_CPUSEL - (uint32_t)PM);
  PM->PM_CPUSEL = PM_CPUSEL_CPUDIV;             /* 80 MHz / 2                 */
  while((PM->PM_SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  PM->PM_UNLOCK = PM_UNLOCK_KEY(0xAAu) |        /* Disable write protect      */
                  PM_UNLOCK_ADDR((uint32_t)&PM->PM_MCCTRL - (uint32_t)PM);
  PM->PM_MCCTRL = PM_MCCTRL_MCSEL(4);
  while((PM->PM_SR & PM_SR_CKRDY) != PM_SR_CKRDY);

  if (SysTick_Config(40000000UL / 100UL)) {     /* SysTick 10 msec interrupts */
    for(;;) { __NOP(); }                        /* Capture error */
  }

  while(1) {                                    /* Loop forever               */
    /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    idx += dir;
    if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; }
    else if   (idx < 0) { dir =  1; idx =  0;         }

    LED_On (idx);                               /* Turn on LED 'idx'          */
    Delay(20);                                  /* Delay 200ms                */
    LED_Off(idx);                               /* Turn off LED 'idx'         */
    Delay(20);                                  /* Delay 200ms                */

    printf ("Hello World\n\r");
  }

}
