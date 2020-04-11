/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    NAND_File.c
 *      Purpose: File manipulation example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <sam3u.h>
#include <File_Config.h>
#include <rl_usb.h>
#include "Memory.h"
#include "GLCD.h"

FAT_VI *nand0;                            /* Media Control Block */

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
void LED_On (unsigned int value) {

  PIOB->PIO_CODR = (value);               /* Turn On  LED */
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {

  PIOB->PIO_SODR = (value);               /* Turn Off LED */
}

/*-----------------------------------------------------------------------------
 *        Setup GLCD and print out example title
 *---------------------------------------------------------------------------*/
static void glcd_setup (void) {
  GLCD_Init          ();
  GLCD_Clear         (Blue);
  GLCD_SetBackColor  (Blue);
  GLCD_SetTextColor  (White);
  GLCD_DisplayString (3, 0, 1, " SAM3U-EK MSD Demo  ");
  GLCD_DisplayString (4, 0, 1, "   www.keil.com     ");
}

/*----------------------------------------------------------------------------
 *        Main: 
 *---------------------------------------------------------------------------*/
int main (void) {
  Media_INFO info;

  SystemCoreClockUpdate();
  
  /* Enable Clock for PIO */
  PMC->PMC_WPMR  = 0x504D4300;            /* Disable write protect           */
  PMC->PMC_PCER0 = (1 << ID_PIOB);        /* Enable clock for LEDs           */
  PMC->PMC_WPMR  = 0x504D4301;            /* Enable write protect            */

  PIOB->PIO_IDR  = 
  PIOB->PIO_OER  =
  PIOB->PIO_PER  = (PIO_PB1  | PIO_PB0 ); /* Setup Pins PB0..PB1 for LEDs    */
  PIOB->PIO_SODR = (PIO_PB1  | PIO_PB0 ); /* Turn LEDs Off                   */

  glcd_setup();                           /* Setup GLCD                      */
  usbd_init();                            /* USB Initialization              */

  nand0 = ioc_getcb (NULL);

  if (ioc_init (nand0) == 0) {
    ioc_read_info (&info, nand0);
    USBD_MSC_BlockSize  = 512;
    USBD_MSC_BlockGroup = nand0->CaSize + 2;   /* Cache buffer from File_Config.c */
    USBD_MSC_BlockCount = info.block_cnt * (info.read_blen / 512);
    USBD_MSC_MemorySize = USBD_MSC_BlockCount * info.read_blen;
    USBD_MSC_BlockBuf   = (U8 *)nand0->CaBuf;
    USBD_MSC_MediaReady = __TRUE;
    usbd_connect(__TRUE);                 /* USB Connect */
  }

  while (1);                              /* Loop forever                    */
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
