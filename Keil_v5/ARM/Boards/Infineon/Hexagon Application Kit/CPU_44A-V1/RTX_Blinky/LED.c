/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low Level LED functions
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

#include "XMC4400.h"                         /* XMC4500 definitions           */
#include "LED.h"

const unsigned long led_mask[] = {1UL << 2, 1UL << 7}; /* GPIO P5.2, P5.7     */

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* configure PORT P5.2, P5.7 pin as output */
  PORT5->IOCR0  &=  ~((0xFFUL << 16));       /* clear P5.2 settings           */
  PORT5->IOCR0  |=    (0x80UL << 16);        /* P5.2 output push pull         */
  PORT5->IOCR4  &=  ~((0xFFUL << 24));       /* clear P5.7 settings           */
  PORT5->IOCR4  |=    (0x80UL << 24);        /* P5.7 output push pull         */
  PORT5->PDR0   &=  ~((0x07UL <<  8) |       /* clear P5.2 output settings    */
                      (0x07UL << 28)  );     /* clear P5.7 output settings    */
  PORT5->PDR0   |=   ((0x02UL <<  8) |       /* P5.2 output strong            */
                      (0x02UL << 28)  );     /* P5.7 output strong            */

  LED_Out (0);                               /* switch LEDs off               */
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PORT5->OMR = (led_mask[num] << 16);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PORT5->OMR = (led_mask[num]      );
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1 << i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
