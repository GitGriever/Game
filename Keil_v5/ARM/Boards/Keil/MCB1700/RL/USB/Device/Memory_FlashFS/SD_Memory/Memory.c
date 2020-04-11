/*-----------------------------------------------------------------------------
 *      RL-ARM - USB
 *-----------------------------------------------------------------------------
 *      Name:    Memory.c
 *      Purpose: USB Memory Storage Demo
 *      Rev.:    V4.70
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>
#include <..\..\RL\USB\INC\usb.h>

#include "Memory.h"
#include "LED.h"
#include "GLCD.h"

FAT_VI *mc0;                                        /* Media Control Block    */
Media_INFO info;
extern SPI_DRV      spi0_drv;

/*-----------------------------------------------------------------------------
  Setup GLCD and print out example title
 *----------------------------------------------------------------------------*/
static void GLCD_Setup (void) {
  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (1, 0, 1, "      MCB1700       ");
  GLCD_DisplayString (2, 0, 1, "       RL-ARM       ");
  GLCD_DisplayString (3, 0, 1, "    MSD example     ");
}

/*-----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  U8 media, sd_ok;

  LED_Init   ();                            /* Init LEDs                      */
  GLCD_Setup ();                            /* Init and setup GLCD            */

  mc0 = ioc_getcb (NULL);
  sd_ok = (!ioc_init (mc0));
  if (sd_ok) {
    ioc_read_info (&info, mc0);
  }
  usbd_init();                              /* USB Device Initialization      */
  usbd_connect(__TRUE);                     /* USB Device Connect             */
  media = 0x80;

  while (1) {                               /* Loop forever                   */
    if (media ^ (spi0_drv.CheckMedia() & M_INSERTED)) {
      media &= 1;
      media ^= 1;
      if (media) {
                                            /* SD Card inserted               */
        if (!sd_ok) {
          sd_ok = (!ioc_init (mc0));
        }
        if (sd_ok) {
          ioc_read_info (&info, mc0);
          usbd_msc_init ();
          GLCD_DisplayString (5, 0, 1, "  SD Card ok!       ");
        } else {
          GLCD_DisplayString (5, 0, 1, "  SD Card not ok!   ");
        }
      } else {
                                            /* SD Card removed                */
        sd_ok = 0;
        GLCD_DisplayString (5, 0, 1, "  SD Card missing!  ");
        USBD_MSC_MediaReady = __FALSE;
      }
    }
  }
}
