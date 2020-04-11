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
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <SAM4E.h>                        /* SAM4E definitions                */
#include <system_SAM4E.h>                 /* System Header                    */
#include "GLCD.h"
#include "Serial.h"
#include "LED.h"
#include "ADC.h"

#define __FI        1                     /* Font index 16x24                 */

char text[40];

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_1s;


/*----------------------------------------------------------------------------
  Function that initializes User Buttons
 *----------------------------------------------------------------------------*/
void BUT_Init(void) {
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_IDR  =
  PIOA->PIO_ODR  =
  PIOA->PIO_PER  = (PIO_PA1 | PIO_PA2);

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;
  uint16_t but    = 0, but_    = 0xFFFF;

  SystemCoreClockUpdate();

  LED_Init();                                /* LED Initialization            */
  SER_Init();                                /* UART Initialization           */
  ADC_Init();                                /* ADC Initialization            */
  BUT_Init();                                /* User Button Initialization    */

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD      */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, (unsigned char *)"   SAM4E-EK Demo    ");
  GLCD_DisplayString(1, 0, __FI, (unsigned char *)"       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, (unsigned char *)"    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(4, 0, __FI, (unsigned char *)"AD value:            ");
  GLCD_DisplayString(7, 0, __FI, (unsigned char *)"Buttons :            ");
#endif

  SysTick_Config(SystemCoreClock/100);  /* Generate interrupt each 10 ms      */

  while (1) {                                /* Loop forever                  */

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

    /* Button inputs                                                          */
    but = 0;

    if (PIOA->PIO_PDSR & (PIO_PA1)) but |= (1 << 0);  /* Button SCROLL-UP     */
    if (PIOA->PIO_PDSR & (PIO_PA2)) but |= (1 << 1);  /* Button SCROLL-DN     */
    but ^= 0x03;


    if (ad_val ^ ad_val_) {                  /* AD value changed              */
      ad_val_ = ad_val;

      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(4,  9, __FI,  (unsigned char *)text);
      GLCD_Bargraph (144, 5*24, 176, 20, (ad_val >> 2)); /* max bargraph is 10 bit */
#endif
    }

    if (but ^ but_) {                        /* Buttons status changed        */
      but_ = but;

#ifdef __USE_LCD
      if (but & (1 << 0)) {
        GLCD_SetTextColor(DarkGrey);
        GLCD_DisplayString(7,  9, __FI, (unsigned char *)"SCROLL-UP");
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayString(7,  9, __FI, (unsigned char *)"SCROLL-UP");
      }
      if (but & (1 << 1)) {
        GLCD_SetTextColor(DarkGrey);
        GLCD_DisplayString(8,  9, __FI, (unsigned char *)"SCROLL-DN");
      } else {
        GLCD_SetTextColor(LightGrey);
        GLCD_DisplayString(8,  9, __FI, (unsigned char *)"SCROLL-DN");
      }
#endif
    }

    /* Print message with AD value every second                               */
    if (clock_1s) {
      clock_1s = 0;

      printf("AD value: %s\r\n", text);
    }
  }
}
