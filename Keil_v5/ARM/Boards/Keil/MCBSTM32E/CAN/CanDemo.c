/*----------------------------------------------------------------------------
 * Name:    CanDemo.c
 * Purpose: CAN example for MCBSTM32E
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
#include "stm32f10x.h"                            /* STM32F10x Definitions    */
#include "GLCD.h"                                 /* GLCD function prototypes */
#include "ADC.h"                                  /* LCD function prototypes  */
#include "CAN.h"                                  /* STM32 CAN adaption layer */

#define __FI        1                             /* Font index 16x24         */

char text[17];
char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

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
  convert one byte to string in hexadecimal notation
 *----------------------------------------------------------------------------*/
void Hex_Str (unsigned char hex, char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}

/*----------------------------------------------------------------------------
  display transmit and receieve values
 *----------------------------------------------------------------------------*/
void val_display (void) {
#ifdef __USE_LCD
  static char disp_buf[] = " Tx:    , Rx:    ";   /* display string */

  Hex_Str(val_Tx, &disp_buf[ 4]);                 /* display Tx and Rx values to LCD disp */
  Hex_Str(val_Rx, &disp_buf[13]);
  GLCD_DisplayString(6, 0, __FI, (u8*)disp_buf);  /* print string to LCD */
#endif

  Delay (10);                                     /* dlay for 10ms            */
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {

  CAN_setup ();                                   /* setup CAN Controller     */
  CAN_wrFilter (33, STANDARD_FORMAT);             /* Enable reception of msgs */

  /* ! COMMENT LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK !    */
  CAN_testmode(CAN_BTR_SILM | CAN_BTR_LBKM);      // Loopback, Silent Mode (self-test)

  CAN_start ();                                   /* start CAN Controller   */

  CAN_waitReady ();                               /* wait til tx mbx is empty */
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)  {
  int i;

  ADC_Init ();                                    /* initialize A/D converter */

  SysTick_Config(SystemCoreClock / 1000);         /* SysTick 1 msec IRQ       */

#ifdef __USE_LCD
  GLCD_Init();                                    /* Initialize the GLCD      */
  GLCD_Clear(White);                              /* Clear the GLCD           */

  GLCD_SetBackColor(Blue);                        /* Set the Back Color       */
  GLCD_SetTextColor(White);                       /* Set the Text Color       */
  GLCD_DisplayString(0, 0, __FI, "   MCBSTM32E Demo   ");
  GLCD_DisplayString(1, 0, __FI, "        CAN         ");
  GLCD_DisplayString(2, 0, __FI, "    www.keil.com    ");
  GLCD_SetBackColor(White);                       /* Set the Back Color       */
  GLCD_SetTextColor(Blue);                        /* Set the Text Color       */
  GLCD_DisplayString(5, 0, __FI, " CAN at 500kbit/s   ");
#endif

  can_Init ();                                    /* initialize CAN interface */

  CAN_TxMsg.id = 33;                              /* initialize msg to send   */
  for (i = 0; i < 8; i++) CAN_TxMsg.data[i] = 0;
  CAN_TxMsg.len = 1;
  CAN_TxMsg.format = STANDARD_FORMAT;
  CAN_TxMsg.type = DATA_FRAME;

  while (1) {
    ADC_StartCnv();                               /* start A/D conversion     */
    val_Tx = ((ADC_GetCnv() >> 4) & 0xFF);        /* use upper 8 bits of ADC  */
    ADC_StopCnv();                                /* stop A/D conversion      */


    if (CAN_TxRdy) {                              /* tx msg on CAN Ctrl       */
      CAN_TxRdy = 0;

      CAN_TxMsg.data[0] = val_Tx;                 /* data[0] = ADC value      */
      CAN_wrMsg (&CAN_TxMsg);                     /* transmit message         */
    }

    Delay (10);                                   /* delay for 10ms           */

    if (CAN_RxRdy) {                              /* rx msg on CAN Ctrl       */
      CAN_RxRdy = 0;

      val_Rx = CAN_RxMsg.data[0];
    }

    val_display ();                               /* display TX and RX values */
  }
}
