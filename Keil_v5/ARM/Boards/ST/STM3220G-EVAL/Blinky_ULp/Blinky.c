/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM3220-EVAL
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
#include "stm32f2xx.h"                  /* STM32F2xx Definitions              */
#include "GLCD.h"
#include "ADC.h"

#define __FI        1                   /* Font index 16x24                   */
#if (__FI == 1)                         /* Font index  6x8                    */                         
  #define __FONT_WIDTH  16
  #define __FONT_HEIGHT 24
#else                                   /* Font index 16x24                   */
  #define __FONT_WIDTH   6
  #define __FONT_HEIGHT  8
#endif

#define LED_NUM     4                   /* Number of user LEDs                */
const unsigned long led_mask[] = { 1UL<<6, 1UL<<8, 1UL<<9, 1UL<<7 };

char  text[40];

/* Import external functions from Serial.c file                               */
extern void SER_Init (void);

/* Import external variables from IRQ.c file                                  */
extern volatile unsigned char  clock_1s;

/* variable to trace in LogicAnalyzer (should not read to often)              */
volatile unsigned short AD_dbg;          


/*-----------------------------------------------------------------------------
  note: 
  set __USE_LCD in "options for target - C/C++ - Define" to enable Output on LCD
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_Init(void) {
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
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < 2) {
    GPIOG->BSRRL = led_mask[num];
  }
  else if (num == 2) {
    GPIOI->BSRRL = led_mask[num];
  }
  else {
    GPIOC->BSRRL = led_mask[num];
  }
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < 2) {
    GPIOG->BSRRH = led_mask[num];
  }
  else if (num == 2) {
    GPIOI->BSRRH = led_mask[num];
  }
  else {
    GPIOC->BSRRH = led_mask[num];
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


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {                       /* Main Program                       */
  unsigned short AD_value, AD_print = 0;

  SysTick_Config(SystemCoreClock/100);  /* Generate interrupt each 10 ms      */

  LED_Init();                           /* LED Initialization                 */
  SER_Init();                           /* UART#1 Initialization              */
  ADC_Init();                           /* ADC Initialization                 */

#ifdef __USE_LCD
  GLCD_Init();                          /* Initialize graphical LCD display   */

  GLCD_Clear(White);                    /* Clear graphical LCD display        */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, " STM3220G-EVAL Demo ");
  GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
#endif // __USE_LCD

  while (1) {                           /* Loop forever                       */
    ADC_StartCnv ();
    while (!ADC_DoneCnv());
    AD_value = ADC_GetCnv();            /* Read AD_last value                 */

    if (AD_value != AD_print) {
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_Bargraph (9 * __FONT_WIDTH, 6 * __FONT_HEIGHT, 10 * __FONT_WIDTH, __FONT_HEIGHT - 2, (AD_value>>2));
      GLCD_SetTextColor(White);
#endif // __USE_LCD

      AD_print = AD_value;              /* Get unscaled value for printout    */
      AD_dbg   = AD_value;
    }

    /* Printf message with AD value to serial port every 1 second */
    if (clock_1s) {
      clock_1s = 0;
      sprintf(text, "AD value = 0x%04X", AD_print);

#ifdef __USE_LCD
      GLCD_SetTextColor(Blue);
      GLCD_DisplayString(5, 0, __FI,  (unsigned char *)text);
#endif // __USE_LCD
      printf("%s\r\n", text);
    }
  }
}
