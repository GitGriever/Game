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
#include "LED.h"
#include "UART.h"
#include <stdio.h>


/* Check if status has changed and if so, send notify to USB Host on Int EP   */

void NotifyOnStatusChange (void) {
  static int32_t old_notify = -1;
         int32_t stat, com_err, notify;

  stat    = UART_GetStatusLineState();
  com_err = UART_GetCommunicationErrorStatus();
  notify = (((com_err & UART_OVERRUN_ERROR_Msk) == UART_OVERRUN_ERROR_Msk)   << CDC_SERIAL_STATE_OVERRUN_Pos)    | 
           (((com_err & UART_PARITY_ERROR_Msk ) == UART_PARITY_ERROR_Msk )   << CDC_SERIAL_STATE_PARITY_Pos )    | 
           (((com_err & UART_FRAMING_ERROR_Msk) == UART_FRAMING_ERROR_Msk)   << CDC_SERIAL_STATE_FRAMING_Pos)    | 
           (((stat & UART_STATUS_LINE_RI_Msk )  == UART_STATUS_LINE_RI_Msk)  << CDC_SERIAL_STATE_RING_Pos   )    | 
             (UART_GetBreak() << CDC_SERIAL_STATE_BREAK_Pos)                                                     | 
           (((stat & UART_STATUS_LINE_DSR_Msk)  == UART_STATUS_LINE_DSR_Msk) << CDC_SERIAL_STATE_TX_CARRIER_Pos) | 
           (((stat & UART_STATUS_LINE_DCD_Msk)  == UART_STATUS_LINE_DCD_Msk) << CDC_SERIAL_STATE_RX_CARRIER_Pos) ;
  if (notify ^ old_notify) {            /* If notify changed                  */
    if (USBD_CDC_ACM_Notify (notify))   /* Send new notification              */
      old_notify = notify;
  }
}

__task void init (void) {
  int32_t usb_rx_ch = -1;
  int32_t usb_tx_ch = -1;

  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (4, 0, 1, "USB CDC ACM RTX Demo");
  GLCD_DisplayString (5, 0, 1, "   www.keil.com     ");

  LED_Initialize ();

  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  while (!usbd_configured ());          /* Wait for device to configure       */

  while (1) {                           /* Loop forever                       */
    NotifyOnStatusChange();             /* Notification handling              */

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


int main (void) {

  os_sys_init(init);                    /* Init RTX and start 'init'          */
}
