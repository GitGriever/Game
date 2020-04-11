/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): possible defines set in "options for target - C/C++ - Define"
 *            __USE_LCD   - enable Output on LCD
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
#include "stm32f10x.h"                  /* STM32F10x.h definitions            */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"

#define __FI        1                   /* Font index 16x24                   */


/* Import functions from Setup.c                                              */
extern void CLK_Init (void);
extern void ADC_Init_(void);
extern void Backlight_Init(void);

extern volatile unsigned short ADC_ConvertedValue;
extern volatile unsigned char  clock_1s;
extern volatile unsigned long  TimeTick;

#define AD_VALUE_MAX 0xFFF

/*------------------------------------------------------------------------------
  note: 
  LCD corrupts AD values due to HW design.
  set __USE_LCD in "options for target - C/C++ - Define" to enable Output on LCD
  set __USE_IRQ in "options for target - C/C++ - Define" to enable ADC in IRQ mode
                                                        default is ADC in DMA mode
 *------------------------------------------------------------------------------*/

int  AD_DbgVal;
char text[40];



/*------------------------------------------------------------------------------
  Delay function
  delays number of tick Systicks (happens every 10 ms)
 *------------------------------------------------------------------------------*/
void Delay (unsigned long tick) {
  unsigned long timetick;

  timetick = TimeTick;
  while ((TimeTick - timetick) < tick);
}

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int AD_value;
  int AD_valueOld = -1;

  CLK_Init ();
  ADC_Init_();
  LED_Init ();
  SER_Init ();

#ifdef __USE_LCD
  GLCD_Init();                               /* GLCD Initialization           */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "   MCBSTM32E Demo   ");
  GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(4, 0, __FI, "AD value:            ");
#endif

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 100);     /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
    AD_value  = ADC_ConvertedValue;          /* Read AD value                 */

    if (AD_value != AD_valueOld) {          /* If new AD value is different   */
      AD_DbgVal    = AD_value;

      sprintf(text, "0x%04X", AD_value);       /* format text for print out     */
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(4,  9, __FI,  (unsigned char *)text);
      GLCD_Bargraph (144, 5*24, 176, 20, (AD_value>>2));
#endif // __USE_LCD


      AD_valueOld = AD_value;
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
