/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for MCB1200
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2010 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC122x.h"                            /* LPC122x definitions        */

#define LED_NUM     8                           /* Number of user LEDs        */
const unsigned long led_mask[] = {1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3, 
                                  1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7 };

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

/* Import external variables from IRQ.c file                                  */
extern volatile unsigned short AD_last;
extern volatile unsigned char  clock_1s;

/* variable to trace in LogicAnalyzer (should not read to often)              */
       volatile unsigned short AD_dbg;          

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_init (void) {

  /* configure PIN GPIO0.30 for AD0 */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL << 31) |   /* enable clock for GPIO0     */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->PIO0_30   = ((3UL <<  0) |         /* P0.30 is AD0               */
                          (0UL <<  4) |         /* PullUp disabled            */
                          (0UL <<  7) );        /* Analog mode enablde        */

  LPC_GPIO0->DIR &= ~(1UL << 30);               /* configure GPIO as input    */

  /* configure ADC */
  LPC_SYSCON->PDRUNCFG      &= ~(1UL <<  4);    /* Enable power to ADC block  */
  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 14);    /* Enable clock to ADC block  */

  LPC_ADC->CR          =  ( 1UL <<  0) |        /* select AD0 pin             */
                          (23UL <<  8) |        /* ADC clock is 24MHz/24      */
                          ( 1UL << 21);         /* enable ADC                 */ 

  LPC_ADC->INTEN       =  ( 1UL <<  8);         /* global enable interrupt    */

  NVIC_EnableIRQ(ADC_IRQn);                     /* enable ADC Interrupt       */

}


/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_init(void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 29);     /* enable clock for GPIO2     */

  /* configure GPIO as output */
  LPC_GPIO2->DIR |= (led_mask[0] | led_mask[1] | led_mask[2] | led_mask[3] |
                     led_mask[4] | led_mask[5] | led_mask[6] | led_mask[7] );
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  LPC_GPIO2->SET = led_mask[num];
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  LPC_GPIO2->CLR = led_mask[num];
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


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {                               /* Main Program               */
  short AD_value, AD_print;

  SysTick_Config(SystemCoreClock/100);          /* Generate IRQ each 10 ms    */

  LED_init();                                   /* LED Initialization         */
  SER_init();                                   /* UART#1 Initialization      */
  ADC_init();                                   /* ADC Initialization         */

  while (1) {                                   /* Loop forever               */
    if (AD_value != AD_last) {                  /* AD value changed           */
      AD_value = AD_last;                       
      AD_dbg   = AD_value;
    }
    AD_print  = AD_value;

    if (clock_1s) {
      clock_1s = 0;
      printf ("AD value = 0x%03x\n\r", AD_print);
    }
  }
}
