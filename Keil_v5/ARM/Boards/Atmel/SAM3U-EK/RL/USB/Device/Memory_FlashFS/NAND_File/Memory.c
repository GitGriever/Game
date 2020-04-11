/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    Memory.c
 *      Purpose: USB Memory Storage Demo
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <sam3u.h>

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "NAND_File.h"
#include "Memory.h"
#include "GLCD.h"
#include "Serial.h"

FAT_VI *nand0;                              /* Media Control Block */

/*-----------------------------------------------------------------------------
  WakeUp Event (INT0 Key)
 *---------------------------------------------------------------------------*/
volatile BOOL WakeUp   = __FALSE;           /* WakeUp Event          */
volatile BOOL USR_Key  = __FALSE;           /* User Right Key Active */

/*-----------------------------------------------------------------------------
  Systick Timer IRQ (happens every 50ms)
 *---------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  BOOL key;

  key = ((PIOA->PIO_PDSR & (1<<19)) == 0);

  if (key && !USR_Key) {
    WakeUp = __TRUE;
  }
  USR_Key = key;
}


/*-----------------------------------------------------------------------------
  Switch on LEDs
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int value) {

  PIOB->PIO_CODR = (value);                 /* Turn On  LED */
}


/*-----------------------------------------------------------------------------
  Switch off LEDs
 *---------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {

  PIOB->PIO_SODR = (value);                 /* Turn Off LED */
}

/*-----------------------------------------------------------------------------
 *        Setup GLCD and print out example title
 *---------------------------------------------------------------------------*/
static void glcd_setup (void) {
  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (2, 0, 1, " SAM3U-EK MSD Demo  ");
  GLCD_DisplayString (3, 0, 1, "   www.keil.com     ");
  GLCD_DisplayString (5, 0, 1, "Current:            ");
}

/*-----------------------------------------------------------------------------
  Main Program
 *---------------------------------------------------------------------------*/
int main (void) {
  Media_INFO info;

  SystemCoreClockUpdate();
  SysTick_Config (4200000);                 /* Systick frequency 50ms        */
  SER_Init ();                              /* Initialize serial interface   */

  glcd_setup ();                            /* Setup GLCD                    */
  usbd_init  ();                            /* USB Initialization            */

  /* Enable Clock for PIO */
  PMC->PMC_WPMR  = 0x504D4300;              /* Disable write protect         */
  PMC->PMC_PCER0 = (1 << ID_PIOA) |         /* Enable clock for button       */
                   (1 << ID_PIOB) ;         /* Enable clock for LEDs         */
  PMC->PMC_WPMR  = 0x504D4301;              /* Enable write protect          */

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                /* USB Disconnect */
      LED_Off(LED_MSK);
      GLCD_DisplayString (6, 0, 1, "   Term Interface   ");
      NAND_main();
    } 
    else {
      nand0 = ioc_getcb (NULL);
      if (ioc_init (nand0) == 0) {
        ioc_read_info (&info, nand0);
        USBD_MSC_BlockSize  = 512;
        USBD_MSC_BlockGroup = nand0->CaSize + 2;   /* Cache buffer from File_Config.c */
        USBD_MSC_BlockCount = info.block_cnt * (info.read_blen / 512);
        USBD_MSC_MemorySize = USBD_MSC_BlockCount * info.read_blen;
        USBD_MSC_BlockBuf   = (U8 *)nand0->CaBuf;
        USBD_MSC_MediaReady = __TRUE;
        usbd_connect(__TRUE);               /* USB Connect */
        GLCD_DisplayString (6, 0, 1, "   PC Interface     ");
      }
      else {
        LED_On(LED_MSK);                    /* NAND Failure! */
        GLCD_DisplayString (6, 0, 1, "   NAND Failure     ");
      }
      while (!WakeUp);
    }
  }
}
