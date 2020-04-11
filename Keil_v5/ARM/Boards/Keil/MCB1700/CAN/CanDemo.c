/*----------------------------------------------------------------------------
 * Name:    CanDemo.c
 * Purpose: CAN example for LPC17xx
 * Note(s):
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
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                              /* LPC17xx definitions      */
#include "GLCD.h"
#include "CAN.h"
#include "LED.h"
#include "ADC.h"

#define __FI        1                             /* Font index 16x24         */
                                                                         
char text[10];

unsigned int val_Tx = 0, val_Rx = 0;              /* Globals used for display */

volatile uint32_t msTicks;                        /* counts 1ms timeTicks     */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}


/*----------------------------------------------------------------------------
  display transmit and receieve values
 *---------------------------------------------------------------------------*/
void val_display (void) {

#ifdef __USE_LCD
  GLCD_SetTextColor(Red);
  sprintf(text, "0x%02X", val_Tx);
  GLCD_DisplayString(6,  4, __FI, (uint8_t*)text);/* print string to GLCD    */
  sprintf(text, "0x%02X", val_Rx);
  GLCD_DisplayString(6, 13, __FI, (uint8_t*)text);/* print string to GLCD    */
#endif
  LED_Out (val_Rx);                               /* display RX val on LEDs  */

    Delay (10);                                   /* dlay for 10ms           */
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {

  CAN_setup (1);                                  /* setup CAN Controller #1  */
  CAN_setup (2);                                  /* setup CAN Controller #2  */
  CAN_wrFilter (1, 33, STANDARD_FORMAT);          /* Enable reception of msgs */

  CAN_start (1);                                  /* start CAN Controller #2  */
  CAN_start (2);                                  /* start CAN Controller #2  */

  CAN_waitReady (1);                              /* wait til tx mbx is empty */
  CAN_waitReady (2);                              /* wait til tx mbx is empty */
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {
  int i;

  ADC_Init ();                                    /* initialize A/D converter */
  LED_Init ();                                    /* initialize the LEDs      */

#ifdef __USE_LCD
  GLCD_Init();                                    /* initialize the GLCD      */

  GLCD_Clear(White);                              /* Clear GLCD display       */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "    MCB1700 Demo    ");
  GLCD_DisplayString(1, 0, __FI, "       Blinky       ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);

  GLCD_DisplayString(5, 0, __FI, " CAN at 500kbit/s   ");
  GLCD_DisplayString(6, 0, __FI, " Tx:    , Rx:       ");
#endif

  SysTick_Config(SystemCoreClock / 1000);         /* Setup SysTick Timer for 1 msec interrupts  */

  can_Init ();                                    /* initialize CAN interface */

  CAN_TxMsg[1].id = 33;                           /* initialize msg to send   */
  for (i = 0; i < 8; i++) CAN_TxMsg[1].data[i] = 0;
  CAN_TxMsg[1].len = 1;
  CAN_TxMsg[1].format = STANDARD_FORMAT;
  CAN_TxMsg[1].type = DATA_FRAME;

  while (1) {
    ADC_StartCnv();                               /* start A/D conversion     */
    val_Tx = ((ADC_GetCnv() >> 4) & 0xFF);        /* use upper 8 bits of ADC  */
    ADC_StopCnv();                                /* stop A/D conversion      */


    if (CAN_TxRdy[1]) {                           /* tx msg on CAN Ctrl #2    */
      CAN_TxRdy[1] = 0;

      CAN_TxMsg[1].data[0] = val_Tx;              /* data[0] = ADC value      */
      CAN_wrMsg (2, &CAN_TxMsg[1]);               /* transmit message         */
    }

    Delay (10);                                   /* dlay for 10ms            */

    if (CAN_RxRdy[0]) {                           /* rx msg on CAN Ctrl #1    */
      CAN_RxRdy[0] = 0;

      val_Rx = CAN_RxMsg[0].data[0];
    }

    val_display ();                               /* display TX and RX values */

  }
}
