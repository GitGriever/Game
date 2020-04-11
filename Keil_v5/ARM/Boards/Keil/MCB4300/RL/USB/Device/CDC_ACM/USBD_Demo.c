/*------------------------------------------------------------------------------
 *      RL-ARM - USB
 *------------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.70
 *------------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include "GLCD.h"
#include "UART.h"
#include <stdio.h>


int main (void) {
  int32_t usb_rx_ch = -1;
  int32_t usb_tx_ch = -1;

  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (4, 0, 1, " USB CDC ACM Demo   ");
  GLCD_DisplayString (5, 0, 1, "   www.keil.com     ");

  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  while (!usbd_configured ());          /* Wait for device to configure       */

  while (1) {                           /* Loop forever                       */
                                        /* USB -> UART                        */
    if (usb_rx_ch == -1) {
      usb_rx_ch = USBD_CDC_ACM_GetChar ();
    }
    if (usb_rx_ch != -1) {
      if (UART_PutChar (usb_rx_ch) == usb_rx_ch) {
        usb_rx_ch = -1;
      }
    }

                                        /* UART -> USB                        */
    if (usb_tx_ch == -1) {
      usb_tx_ch = UART_GetChar ();
    }
    if (usb_tx_ch != -1) {
      if (USBD_CDC_ACM_PutChar (usb_tx_ch) == usb_tx_ch) {
        usb_tx_ch = -1;
      }
    }
  }
}
