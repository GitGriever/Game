/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: low level LED functions
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

#include "stm32l1xx.h"                  /* STM32L1xx.h definitions            */
#include "LED.h"

const unsigned long led_mask[] = { 1UL <<  3, 1UL <<  7, 1UL << 14, 1UL << 15};


/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  RCC->AHBENR  |= ((1UL << 3) |             /* Enable GPIOD clock             */
                   (1UL << 7)  );           /* Enable GPIOG clock             */

  GPIOD->ODR   |=  0x00000088;              /* switch off LEDs                */
  GPIOD->MODER &= ~0x0000C0C0;
  GPIOD->MODER  =  0x00004040;              /* General purpose output mode    */

  GPIOG->ODR   |=  0x0000C000;              /* switch off LEDs                */
  GPIOD->MODER &= ~0xF0000000;
  GPIOG->MODER  =  0x50000000;	            /* General purpose output mode    */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    if (num < 2) {
      GPIOD->BSRRH = led_mask[num];
    }
    else {
      GPIOG->BSRRH = led_mask[num];
    }
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    if (num < 2) {
      GPIOD->BSRRL = led_mask[num];
    }
    else {
      GPIOG->BSRRL = led_mask[num];
    }
  }
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
