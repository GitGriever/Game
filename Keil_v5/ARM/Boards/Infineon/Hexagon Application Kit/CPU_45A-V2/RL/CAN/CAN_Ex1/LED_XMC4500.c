/*----------------------------------------------------------------------------
 * Name:    LED_XMC4500.c
 * Purpose: Low Level LED functions for Infineon XMC4500 Device Series
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "XMC4500.h"                         /* XMC4500 definitions           */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 9 };              /* GPIO P3.9       */

/*----------------------------------------------------------------------------
 *  Initialize LED Pins
 *----------------------------------------------------------------------------*/

void LED_Init (void) {

  /* Configure PORT P3.9 pin as output                                        */
  PORT3->IOCR8  &=  ~(0xFFUL << 8);     /* Clear P3.9 settings                */
  PORT3->IOCR8  |=   (0x80UL << 8);     /* P3.9 output push-pull              */
  PORT3->PDR1   &=  ~(0x07UL << 4);     /* Clear P3.9 output settings         */
  PORT3->PDR1   |=   (0x02UL << 4);     /* P3.9 output strong                 */

  LED_Val (0);                          /* Switch LEDs off                    */
}


/*----------------------------------------------------------------------------
 *  Unitialize LED Pins
 *----------------------------------------------------------------------------*/

void LED_Uninit (void) {

  /* Unconfigure PORT P3.9 pin as after reset                                 */
  PORT3->IOCR8  &=  ~(0xFFUL << 8);     /* Clear P3.9 settings                */
  PORT3->PDR1   &=  ~(0x07UL << 4);     /* Clear P3.9 output settings         */
  PORT3->PDR1   |=   (0x02UL << 4);     /* P3.9 output strong                 */
}


/*----------------------------------------------------------------------------
 *  Function that turns on requested LED
 *----------------------------------------------------------------------------*/

void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PORT3->OMR = (led_mask[num] << 16);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PORT3->OMR = (led_mask[num]      );
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Val (unsigned int val) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (val & (1 << i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
