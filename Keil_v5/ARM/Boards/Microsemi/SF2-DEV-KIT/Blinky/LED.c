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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "mss_gpio.h"
#include "LED.h"

const uint32_t led_mask[] = { 1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3,
                              1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7 };
#define  LED_ALL   (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] | \
                    led_mask[4] | led_mask[5] | led_mask[6] | led_mask[7]  )

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  MSS_GPIO_init();

  MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE);
  MSS_GPIO_config(MSS_GPIO_7, MSS_GPIO_OUTPUT_MODE);

  MSS_GPIO_set_outputs(MSS_GPIO_get_outputs() & ~LED_ALL);
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < LED_NUM) {
    MSS_GPIO_set_output((mss_gpio_id_t)num, 1);
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < LED_NUM) {
    MSS_GPIO_set_output((mss_gpio_id_t)num, 0);
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
