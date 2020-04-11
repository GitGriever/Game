/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM324x9I-EVAL
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

#include <stdio.h>
#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

char  text[40];

/* Import external variables from IRQ.c file                                  */
extern volatile unsigned char  clock_1s;


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {                       /* Main Program                       */
  unsigned short AD_value, AD_print = 0;

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);  /* Generate interrupt each 10 ms      */

  LED_Init();                           /* LED Initialization                 */
  SER_Init();                           /* USART  Initialization              */
  ADC_Init();                           /* ADC Initialization                 */

  while (1) {                           /* Loop forever                       */
    ADC_StartCnv ();
    while (!ADC_DoneCnv());
    AD_value = ADC_GetCnv();            /* Read AD_last value                 */

    if (AD_value != AD_print) {

      AD_print = AD_value;              /* Get unscaled value for printout    */
    }

    /* Printf message with AD value to serial port every 1 second */
    if (clock_1s) {
      clock_1s = 0;
      sprintf(text, "AD value = 0x%04X", AD_print);

      printf("%s\r\n", text);
    }
  }
}
