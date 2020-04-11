/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher using also LCD
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <LM4Fxxxx.H>
#include "grlib\grlib.h"
#include "..\cfal96x64x16.h"


/* Status LED and Navigator pin definitions */
#define LED             GPIO_PIN_2      /* PG2 */
#define NAV_UP          GPIO_PIN_0      /* PM0 */
#define NAV_DN          GPIO_PIN_1      /* PM1 */
#define NAV_L           GPIO_PIN_2      /* PM2 */
#define NAV_R           GPIO_PIN_3      /* PM3 */
#define NAV_SEL         GPIO_PIN_4      /* PM4 */
#define NAV_MSK        (NAV_UP | NAV_DN | NAV_L | NAV_R | NAV_SEL)
#define AR_MSK         (NAV_UP | NAV_DN | NAV_L | NAV_R)    /* Autorepeat mask*/
#define DEB_TIME        10              /* Debounce time (10*10ms=100ms)      */
#define AR_TIME         20              /* Autorepeat time (20*10ms=100ms)    */


/* Global variables */
volatile unsigned char Tick;            /* Tick Counter (0..99)               */
volatile unsigned long Time;            /* Time Counter (10ms)                */
volatile unsigned char Nav;             /* Navigator States                   */
volatile unsigned char TraceB;          /* Trace Navigator                    */


/* SysTick interrupt happens every 10 ms */
void SysTick_Handler (void) {
  static unsigned int cur, old;
  static unsigned int deb_cnt = DEB_TIME + 1;
  static unsigned int aut_cnt = AR_TIME;
  static unsigned int led;

  /* Handle debouncing and autorepeat for navigator */
  cur = ~GPIOPinRead(GPIO_PORTM_BASE, NAV_MSK) & NAV_MSK;
  
  if (cur ^ old) {
    old     = cur;
    deb_cnt = DEB_TIME + 1;
    aut_cnt = AR_TIME;
  } else {
    if (deb_cnt) {
      deb_cnt --;
      aut_cnt ++;
    }
    aut_cnt --;
  }

  if (deb_cnt == 1) {
    Nav = cur;
  }
  if (aut_cnt == 0) {
    aut_cnt = AR_TIME;
    Nav |= cur & AR_MSK;
  }

  /* If navigator select pressed, time is stopped */
  if (!(Nav & NAV_SEL)) {
    Time++;
  }
  
  /* Toggle LED every 1 second */
  if (Tick ++ > 99) {
    Tick  = 0;
    led  ^= LED;
    GPIOPinWrite(GPIO_PORTG_BASE, LED, led);
  }
}


/* Main program */
int main (void) {
  tContext    sContext;
  tRectangle  sRect;
  unsigned    int  w, h; 
  unsigned    int  time, nav, n;
              char buf[13+1];

  /* Set the clocking to run from XTAL */
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  /* Enable peripherals */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);

  /* Configure Status LED as output */
  GPIOPadConfigSet(GPIO_PORTG_BASE, LED,     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
  GPIODirModeSet  (GPIO_PORTG_BASE, LED,     GPIO_DIR_MODE_OUT);
  GPIOPinWrite    (GPIO_PORTG_BASE, LED, 0);

  /* Configure navigator pins as inputs */
  GPIOPadConfigSet(GPIO_PORTM_BASE, NAV_MSK, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIODirModeSet  (GPIO_PORTM_BASE, NAV_MSK, GPIO_DIR_MODE_IN);

  /* Initialize LCD */
  CFAL96x64x16Init();
  GrContextInit(&sContext, &g_sCFAL96x64x16);
  w = GrContextDpyWidthGet(&sContext);
  h = GrContextDpyHeightGet(&sContext);

  /* Display initial message */
  sRect.sXMin = 0; sRect.sYMin = 0; sRect.sXMax = w - 1; sRect.sYMax = 21;
  GrContextForegroundSet(&sContext, ClrDarkBlue);
  GrRectFill(&sContext, &sRect);
  GrContextForegroundSet(&sContext, ClrWhite);
  GrRectDraw(&sContext, &sRect);
  sRect.sXMax = w - 1; sRect.sYMax = h - 1;
  GrRectDraw(&sContext, &sRect);
  sRect.sYMin = sRect.sYMax - 12;
  GrRectDraw(&sContext, &sRect);
  GrContextFontSet(&sContext, &g_sFontCm12b);
  GrStringDrawCentered(&sContext, "KEIL Demo",    -1, w / 2 ,  5    , 0);
  GrContextFontSet(&sContext, &g_sFontFixed6x8);
  GrStringDrawCentered(&sContext, "for LM4F232",  -1, w / 2 , 16    , 0);
  GrStringDraw(&sContext, "press SW1..5",         -1, 4     , h - 22, 0);
  GrStringDraw(&sContext, "          .   s",      -1, w - 90, h - 10, 0);
  sprintf(buf, "%1.2f s", (float)time / 100);

  /* Setup and enable SysTick with interrupt (100Hz) */
  SysTickPeriodSet(ROM_SysCtlClockGet() / 100);
  SysTickEnable();
  SysTickIntEnable();

  time = ~Time;
  n    =  0;

  /* Endless Loop */
  while (1) {
    /* Display time */
    if (time ^ Time) {
      /* Clear old time value */
      GrContextForegroundSet(&sContext, ClrBlack);
      sprintf(buf, "%13.2f", (float)time / 100); 
      buf[10] =' ';
      GrStringDraw(&sContext, buf,    -1, w - 90, h - 10, 0);
      time = Time;

      /* Display new time value */
      GrContextForegroundSet(&sContext, ClrWhite);
      sprintf(buf, "%13.2f", (float)time / 100);
      GrStringDraw(&sContext, buf,    -1, w - 90, h - 10, 0);
    }

    /* Display Navigator state */
    nav  = Nav;
    n   ^= nav;
    if (n & NAV_UP) {
      if (nav & NAV_UP) {
        /* UP pressed */
        GrContextForegroundSet(&sContext, ClrWhite);
      } else {
        /* UP released */
        GrContextForegroundSet(&sContext, ClrBlack);
      }
      GrStringDraw(&sContext, "SW1",    -1, 8 + (3*6 + 2)* 0, h - 38, 0);
    }
    if (n & NAV_DN) {
      if (nav & NAV_DN) {
        /* DOWN pressed */
        GrContextForegroundSet(&sContext, ClrWhite);
      } else {
        /* DOWN released */
        GrContextForegroundSet(&sContext, ClrBlack);
      }
      GrStringDraw(&sContext, "SW2",  -1, 8 + (3*6 + 2)* 1, h - 38, 0);
    }
    if (n & NAV_L) {
      if (nav & NAV_L) {
        /* LEFT pressed */
        GrContextForegroundSet(&sContext, ClrWhite);
      } else {
        /* LEFT released */
        GrContextForegroundSet(&sContext, ClrBlack);
      }
      GrStringDraw(&sContext, "SW3",  -1, 8 + (3*6 + 2)* 2, h - 38, 0);
    }
    if (n & NAV_R) {
      if (nav & NAV_R) {
        /* RIGHT pressed */
        GrContextForegroundSet(&sContext, ClrWhite);
      } else {
        /* RIGHT released */
        GrContextForegroundSet(&sContext, ClrBlack);
      }
      GrStringDraw(&sContext, "SW4",    -1, 8 + (3*6 + 2)* 3, h - 38, 0);
    }
    if (n & NAV_SEL) {
      if (nav & NAV_SEL) {
        /* SELECT pressed */
        GrContextForegroundSet(&sContext, ClrWhite);
      } else {
        /* SELECT released */
        GrContextForegroundSet(&sContext, ClrBlack);
      }
      GrStringDrawCentered(&sContext, "Time",     -1, w / 2, h - 38, 0);
      GrStringDrawCentered(&sContext, "stopped",  -1, w / 2, h - 30, 0);
    }
    n = nav;
  }
}
