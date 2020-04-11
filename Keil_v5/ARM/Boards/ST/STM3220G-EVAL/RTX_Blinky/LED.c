/******************************************************************************/
/* LED.c: Low Level LED Routines                                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <RTL.h>
#include <stm32f2xx.h>                  /* STM32F2xx Definitions              */
#include "LED.h"

const U16 led_mask[] = { 1UL<<6, 1UL<<8, 1UL<<9, 1UL<< 7 };

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_init (void) {
  RCC->AHB1ENR  |= ((1UL <<  2) |       /* Enable GPIOC clock                 */
                    (1UL <<  6) |       /* Enable GPIOG clock                 */
                    (1UL <<  8)  );     /* Enable GPIOI clock                 */

  GPIOC->MODER    &= ~(3UL << 2*7);     /* PC.7 is output                     */
  GPIOC->MODER    |=  (1UL << 2*7); 
  GPIOC->OTYPER   &= ~(1UL <<   7);     /* PC.7 is output Push-Pull           */
  GPIOC->OSPEEDR  &= ~(3UL << 2*7);     /* PC.7 is 50MHz Fast Speed           */
  GPIOC->OSPEEDR  |=  (2UL << 2*7); 
  GPIOC->PUPDR    &= ~(3UL << 2*7);     /* PC.7 is Pull up                    */
  GPIOC->PUPDR    |=  (1UL << 2*7); 

  GPIOG->MODER    &= ~((3UL << 2*6) |
                       (3UL << 2*8)  ); /* PG.6, PG.8 is output               */
  GPIOG->MODER    |=  ((1UL << 2*6) |
                       (1UL << 2*8)  ); 
  GPIOG->OTYPER   &= ~((1UL <<   6) |
                       (1UL << 2*8)  ); /* PG.6, PG.8 is output Push-Pull     */
  GPIOG->OSPEEDR  &= ~((3UL << 2*6) |
                       (3UL << 2*8)  ); /* PG.6, PG.8 is 50MHz Fast Speed     */
  GPIOG->OSPEEDR  |=  ((2UL << 2*6) |
                       (2UL << 2*8)  ); 
  GPIOG->PUPDR    &= ~((3UL << 2*6) |
                       (3UL << 2*8)  ); /* PG.6, PG.8 is Pull up              */
  GPIOG->PUPDR    |=  ((1UL << 2*6) |
                       (1UL << 2*8)  ); 

  GPIOI->MODER    &= ~(3UL << 2*9);     /* PI.9 is output                     */
  GPIOI->MODER    |=  (1UL << 2*9); 
  GPIOI->OTYPER   &= ~(1UL <<   9);     /* PI.9 is output Push-Pull           */
  GPIOI->OSPEEDR  &= ~(3UL << 2*9);     /* PI.9 is 50MHz Fast Speed           */
  GPIOI->OSPEEDR  |=  (2UL << 2*9); 
  GPIOI->PUPDR    &= ~(3UL << 2*9);     /* PI.9 is Pull up                    */
  GPIOI->PUPDR    |=  (1UL << 2*9); 
}


/*----------------------------------------------------------------------------
  Switch LED on. LED = Bitposition
 *----------------------------------------------------------------------------*/
void LED_on  (int led) {

  if (led < 2) {
    GPIOG->BSRRL = led_mask[led];
  }
  else if (led == 2) {
    GPIOI->BSRRL = led_mask[led];
  }
  else if (led == 3) {
    GPIOC->BSRRL = led_mask[led];
  }
}

/*----------------------------------------------------------------------------
  Switch Led off. LED = Bitposition
 *----------------------------------------------------------------------------*/
void LED_off (int led) {

  if (led < 2) {
    GPIOG->BSRRH = led_mask[led];
  }
  else if (led == 2) {
    GPIOI->BSRRH = led_mask[led];
  }
  else if (led == 3) {
    GPIOC->BSRRH = led_mask[led];
  }
}

/*----------------------------------------------------------------------------
  Output value to LEDs
 *----------------------------------------------------------------------------*/
void LED_out (int led) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (led & (1<<i)) {
      LED_on (i);
    } else {
      LED_off(i);
    }
  }
}
