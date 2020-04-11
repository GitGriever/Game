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

#include <XMC4500.h>                    /* XMC4500 Definitions                */
#include "GPIO.h"
#include "LED.h"

#pragma diag_suppress 61,68


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* LED outputs driven in GPIO mode. */
  Control_P3_7 (OUTPUT_PP_GP, STRONG);
  Control_P3_9 (OUTPUT_PP_GP, STRONG);
  Control_P3_10 (OUTPUT_PP_GP, STRONG);
  Control_P0_9 (OUTPUT_PP_GP, STRONG);
  Reset (P0_9);
}


/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {
  switch (num) {
    case 0:
      Set (P3_7);
      break;
    case 1:
      Set (P3_9);
      break;
    case 2:
      Set (P3_10);
      break;
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {
  switch (num) {
    case 0:
      Reset (P3_7);
      break;
    case 1:
      Reset (P3_9);
      break;
    case 2:
      Reset (P3_10);
      break;
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(uint32_t val) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
