/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low Level LED functions
 * Note(s): possible defines select the used board type:
 *            __PROTOTYPE  - board prototype version V1
 *                         - board release version V1
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

#include "XMC4500.h"                         /* XMC4500 definitions           */
#include "LED.h"

#ifdef __PROTOTYPE
  const unsigned long led_mask[] = {1UL << 14, 1UL << 15}; /* GPIO P2.14..15  */
  #define PORTx   PORT2
#else
  const unsigned long led_mask[] = {1UL <<  1, 1UL <<  0}; /* GPIO P1.0..1    */
  #define PORTx   PORT1
#endif

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

#ifdef __PROTOTYPE
  /* configure PORT P2.14..15 pin as output */
  PORT2->IOCR12 &= ~((0xFFUL << 16) |       /* clear P2.14 settings           */
                     (0xFFUL << 24)  );     /* clear P2.15 settings           */
  PORT2->IOCR12 |=  ((0x80UL << 16) |       /* P2.14 output push pull         */
                     (0x80UL << 24)  );     /* P2.15 output push pull         */
  PORT2->PDR1   &= ~((0x07UL << 24) |       /* clear P2.14 output settings    */
                     (0x07UL << 28)  );     /* clear P2.15 output settings    */
  PORT2->PDR1   |=  ((0x02UL << 24) |       /* P2.14 output strong            */
                     (0x02UL << 28)  );     /* P2.15 output strong            */
#else
  /* configure PORT P1.0..1 pin as output */
  PORT1->IOCR0  &= ~((0xFFUL <<  0) |       /* clear P1.0  settings           */
                     (0xFFUL <<  8)  );     /* clear P1.1  settings           */
  PORT1->IOCR0  |=  ((0x80UL <<  0) |       /* P1.0  output push pull         */
                     (0x80UL <<  8)  );     /* P1.1  output push pull         */
  PORT1->PDR0   &= ~((0x07UL <<  0) |       /* clear P1.0  output settings    */
                     (0x07UL <<  4)  );     /* clear P1.1  output settings    */
  PORT1->PDR0   |=  ((0x02UL <<  0) |       /* P1.0  output strong            */
                     (0x02UL <<  4)  );     /* P1.1  output strong            */
#endif

  LED_Out (0);                               /* switch LEDs off               */
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    PORTx->OMR = (led_mask[num]      );
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    PORTx->OMR = (led_mask[num] << 16);
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
