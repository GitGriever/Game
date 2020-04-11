/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <XMC4500.h>

volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */

int main (void) {
  int32_t usb_rx_ch = -1;
  int32_t usb_tx_ch = -1;
  int32_t i;
	
  usbd_init();                          /* USB Device Initialization          */
  usbd_connect(__TRUE);                 /* USB Device Connect                 */
  while (!usbd_configured ());          /* Wait for device to configure       */

  while (1) {                           /* Loop forever                       */
                                        /* USB -> ITM                         */
    if (usb_rx_ch == -1) {
      usb_rx_ch = USBD_CDC_ACM_GetChar ();
    }
    if (usb_rx_ch != -1) {
      if (ITM_SendChar (usb_rx_ch & 0xFF) == usb_rx_ch) {
        for (i = 5000; i; i--);
        usb_rx_ch = -1;
      }
    }

                                        /* ITM -> USB                         */
    if (usb_tx_ch == -1) {
      if (ITM_CheckChar())
        usb_tx_ch = ITM_ReceiveChar ();
    }
    if (usb_tx_ch != -1) {
      if (USBD_CDC_ACM_PutChar (usb_tx_ch) == usb_tx_ch) {
        usb_tx_ch = -1;
      }
    }
  }
}
