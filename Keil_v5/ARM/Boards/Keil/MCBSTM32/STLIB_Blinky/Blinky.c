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
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "STM32F10x.h"
#include "LCD.h"
#include "Serial.h"
#include "LED.h"

/* Import functions from Setup.c                                              */
extern void CLK_Init (void);
extern void ADC_Init_(void);

extern unsigned short int ADC_ConvertedValue;

/* Import external variables from IRQ.c file                                  */
extern unsigned char clock_1s;

/* variable demonstrating trace  capabilities */
int AD_DbgVal;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int i;
  int AD_value;
  int AD_scaled_ex = -1;
  int AD_scaled;

  CLK_Init ();
  ADC_Init_();
  LED_Init ();
  SER_Init ();

  lcd_init  ();
  lcd_clear ();
  lcd_print (" MCBSTM32 DEMO  ");
  set_cursor(0, 1);
  lcd_print ("  www.keil.com  ");

  for (i = 0; i < 10000000; i++);       /* Wait for initial display           */

  SysTick_Config(SystemCoreClock / 100);     /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */

    AD_value  = ADC_ConvertedValue;     /* Read AD value                      */
    AD_scaled = AD_value / 52;          /* AD value scaled to 0 .. 78         */
    if (AD_scaled != AD_scaled_ex) {    /* If new AD value different than old */
      AD_scaled_ex = AD_scaled;
      AD_DbgVal    = AD_value;
      lcd_bargraphXY(0, 1, AD_scaled);  /* Display bargraph according to AD   */
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value = 0x%04X\r\n", AD_value);
    }
  }
}
