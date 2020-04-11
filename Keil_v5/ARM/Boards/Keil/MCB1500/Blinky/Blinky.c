/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): possible defines set in "options for target - C/C++ - Define"
 *            __USE_LCD   - enable Output on GLCD
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <LPC15xx.h>                         /* LPC15xx definitions           */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"
#include "JOY.h"

#define __FI        1                       /* Font index 16x24               */
                                                                         
char text[10];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;
   int32_t joy    = 0, joy_    = -1;

  SystemCoreClockUpdate();
  
  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */
  JOY_Init();                                /* Joystick Initialization       */

#ifdef __USE_LCD
  GLCD_Init();                               /* GLCD Initialization           */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, (unsigned char *)"    MCB1500 Demo    ");
  GLCD_DisplayString(1, 0, __FI, (unsigned char *)"       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, (unsigned char *)"    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, (unsigned char *)"AD value:            ");
  GLCD_DisplayString(8, 0, __FI, (unsigned char *)"Joystick:            ");
#endif

  SysTick_Config(SystemCoreClock / 100);     /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
    /* Collect all input signals                                              */

    /* AD converter input                                                     */
    if (AD_done) {                           /* If conversion has finished    */
      AD_done = 0;

      ad_avg += AD_last << 8;                /* Add AD value to averaging     */
      ad_avg ++;
      if ((ad_avg & 0xFF) == 0x10) {         /* average over 16 values        */
        ad_val = (ad_avg >> 8) >> 4;         /* average devided by 16         */
        ad_avg = 0;
      }
    }

    /* Joystick input                                                         */
    joy = 0;
    if (JOY_Get() & JOY_DOWN)   joy |= (1 << 0);  /* Joystick left            */
    if (JOY_Get() & JOY_UP)     joy |= (1 << 1);  /* Joystick right           */
    if (JOY_Get() & JOY_LEFT)   joy |= (1 << 2);  /* Joystick up              */
    if (JOY_Get() & JOY_RIGHT)  joy |= (1 << 3);  /* Joystick down            */
    if (JOY_Get() & JOY_CENTER) joy |= (1 << 4);  /* Joystick select          */
    joy ^= 0x1F;

    /* Show all signals                                                       */
    if (ad_val ^ ad_val_) {                  /* AD value changed              */
      ad_val_ = ad_val;

      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(5,  9, __FI,  (unsigned char *)text);
      GLCD_Bargraph (144, 6*24, 176, 20, (ad_val >> 2)); /* max bargraph is 10 bit */
#endif
    }

    if (joy ^ joy_) {                         /* Show joystick status          */
#ifdef __USE_LCD
      if (joy & (1 << 0)) {                   /* check  "Left" Key             */
        GLCD_SetTextColor(Red);
        GLCD_DisplayChar(8,  9, __FI, 0x80+ 9);
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayChar(8,  9, __FI, 0x80+ 8);
      }
      if (joy & (1 << 1)) {                   /* check "Right" Key             */
        GLCD_SetTextColor(Red);
        GLCD_DisplayChar(8, 11, __FI, 0x80+11);
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayChar(8, 11, __FI, 0x80+10);
      }
      if (joy & (1 << 2)) {                  /* check "Up" Key                 */
        GLCD_SetTextColor(Red);
        GLCD_DisplayChar(7, 10, __FI, 0x80+ 5);
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayChar(7, 10, __FI, 0x80+ 4);
      }
      if (joy & (1 << 3)) {                  /* check "Down" Key               */
        GLCD_SetTextColor(Red);
        GLCD_DisplayChar(9, 10, __FI, 0x80+ 7);
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayChar(9, 10, __FI, 0x80+ 6);
      }
      if (joy & (1 << 4)) {                  /* check "Sel" Key                */
        GLCD_SetTextColor(Red);
        GLCD_DisplayChar(8, 10, __FI, 0x80+ 1);
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayChar(8, 10, __FI, 0x80+ 0);
      }
#endif

      joy_ = joy;
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
