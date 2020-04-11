/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "stm32f4xx.h"                       /* STM32F4xx Definitions         */
#include "LED.h"
#include "GLCD.h"

#define __FI        1                        /* Font index 16x24              */
#if (__FI == 1)                              /* Font index  6x8               */                         
  #define __FONT_WIDTH  16
  #define __FONT_HEIGHT 24
#else                                        /* Font index 16x24              */
  #define __FONT_WIDTH   6
  #define __FONT_HEIGHT  8
#endif



uint32_t ClockLEDOn;                         /* On  for 150 ms every 200 ms   */
uint32_t ClockLEDOff;                        /* Off for  50 ms every 200 ms   */
/*----------------------------------------------------------------------------
  SysTick IRQ Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  static uint32_t ticks;

  switch (ticks++) {
    case 15:
      ClockLEDOff = 1;
      break;
    case 20:
      ticks       = 0;
      ClockLEDOn  = 1;
      break;
    default:
      if (ticks > 20) {
        ticks = 0;
      }
  }

}
/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Initialize(void) {

  RCC->AHB1ENR  |= ((1UL <<  0) );           /* Enable GPIOA clock            */

  GPIOA->MODER    &= ~((3UL << 2*0)  );      /* PA.0 is input                 */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );      /* PA.0 is 50MHz Fast Speed      */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );      /* PA.0 is no Pull up            */
}

/*----------------------------------------------------------------------------
  Function that read Button pins
 *----------------------------------------------------------------------------*/
uint32_t BTN_Get(void) {

 return (GPIOA->IDR & (1UL << 0));
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t but     = 0, but_    = -1;
  int32_t num     = 0;
  int32_t dir     = 1;
  int32_t max_num = LED_NUM - 1;

  LED_Initialize();                          /* LED Initialization            */
  BTN_Initialize();                          /* User Button Initialization    */

  GLCD_Initialize();                         /* GLCD Initialization           */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "STM32F429I-Discovery");
  GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 2, __FI, "LED:    ");
  GLCD_DisplayString(6, 2, __FI, "Button: ");

  SysTick_Config(SystemCoreClock / 100);     /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
    /* Collect all input signals                                              */

    /* Button inputs                                                          */
    but = 0;
    if (BTN_Get() == 0) but |= (1 << 0);     /* Button User (User)            */
    but ^= 0x01;

    /* Display all signals                                                    */
    if (ClockLEDOn) {
      ClockLEDOn = 0;
      LED_On (num);                         /* Turn specified LED on          */
      GLCD_SetTextColor(Green);
      GLCD_DisplayChar(5, 10+num, __FI, 0x80+ 1);
    }

    if (ClockLEDOff) {
      ClockLEDOff = 0;
      LED_Off (num);                         /* Turn specified LED off        */
      GLCD_SetTextColor(LightGrey);
      GLCD_DisplayChar(5, 10+num, __FI, 0x80+ 0);

      num += dir;                            /* Change LED number             */
      if (dir == 1 && num == max_num) {
        dir = -1;                            /* Change direction to down      */
      }
      else if (num == 0) {
        dir =  1;                            /* Change direction to up        */
      }
    }

    if (but ^ but_) {                        /* Show buttons status           */
      if (but & (1 << 0)) {
        GLCD_SetTextColor(Red);
        GLCD_DisplayString(6, 10, __FI, "User");
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayString(6, 10, __FI, "User");
      }
      but_ = but;
    }

  }

}
