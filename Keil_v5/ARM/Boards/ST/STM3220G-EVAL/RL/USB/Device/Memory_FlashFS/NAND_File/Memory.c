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

#include <stm32f2xx.h>                  /* STM32F2xx definitions              */

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "NAND_File.h"
#include "Memory.h"
#include "Serial.h"
#include "GLCD.h"

FAT_VI *nand0;                          /* FAT Media Control Block            */
Media_INFO info;                        /* Media information                  */ 

/*-----------------------------------------------------------------------------
  WKUP Key
 *----------------------------------------------------------------------------*/
volatile BOOL WakeUp   = __FALSE;                   /* WakeUp Event           */
volatile BOOL WKUP_Key = __FALSE;                   /* WakeUp Key Active      */


/*-----------------------------------------------------------------------------
  Systick Interrupt Handler (happens every 50ms)
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  BOOL key;

  key = ((GPIOA->IDR & (1 << 0)) == 0);
  if (key && !WKUP_Key) {
    WakeUp = __TRUE;
  }
  WKUP_Key = key;
}


/*-----------------------------------------------------------------------------
  Switch on LEDs
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int value) {
  
  /* Turn On LEDs */
  if (value & LED_RD)   GPIOG->BSRRL |= (1 << 6);
  if (value & LED_WR)   GPIOG->BSRRL |= (1 << 8);
  if (value & LED_CFG)  GPIOI->BSRRL |= (1 << 9);
  if (value & LED_SUSP) GPIOC->BSRRL |= (1 << 7);
}


/*-----------------------------------------------------------------------------
  Switch off LEDs
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {
  
  /* Turn Off LEDs */
  if (value & LED_RD)   GPIOG->BSRRH |= (1 << 6);
  if (value & LED_WR)   GPIOG->BSRRH |= (1 << 8);
  if (value & LED_CFG)  GPIOI->BSRRH |= (1 << 9);
  if (value & LED_SUSP) GPIOC->BSRRH |= (1 << 7);
}


/*-----------------------------------------------------------------------------
  LED Init
 *----------------------------------------------------------------------------*/
static void LED_Init (void) {
  /* Set the clocks */
  RCC->AHB1ENR |= (1UL <<  2) |                     /* Enable GPIOC clock     */
                  (1UL <<  6) |                     /* Enable GPIOG clock     */
                  (1UL <<  8) ;                     /* Enable GPIOI clock     */

  /* Configure LED (PG6, PG8, PI9, PC7) pins as push-pull outputs */
  GPIOG->MODER  &= ~((3UL << 2 * 6) | (3UL << 2 * 8));
  GPIOG->MODER  |=   (1UL << 2 * 6) | (1UL << 2 * 8);  
  GPIOG->OTYPER &= ~((1UL <<     6) | (1UL <<     8));

  GPIOI->MODER  &= ~(3UL << 2 * 9);
  GPIOI->MODER  |=  (1UL << 2 * 9);
  GPIOI->OTYPER &= ~(1UL <<     9);

  GPIOC->MODER  &= ~(3UL << 2 * 7);
  GPIOC->MODER  |=  (1UL << 2 * 7);
  GPIOC->OTYPER &= ~(1UL <<     7);  
}

/*-----------------------------------------------------------------------------
  Setup GLCD and print out example title
 *----------------------------------------------------------------------------*/
static void GLCD_Setup (void) {
  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (0, 0, 1, "   STM3220F-EVAL    ");
  GLCD_DisplayString (1, 0, 1, "       RL-ARM       ");
  GLCD_DisplayString (2, 0, 1, "    MSD example     ");
  GLCD_DisplayString (4, 0, 1, " Current:           ");
}

/*-----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  BOOL usb_initialized = __FALSE;

  SER_Init   ();                                    /* Init serial interface  */
  LED_Init   ();                                    /* Init LEDs              */
  GLCD_Setup ();                                    /* Init and setup GLCD    */

  SysTick_Config (SystemCoreClock / 1000);

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                        /* USB Device Disconnect  */
      LED_Off(LED_MSK);
      GLCD_DisplayString (5, 0, 1, "   Term Interface   ");
      NAND_main();
    }
    else {
      nand0 = ioc_getcb (NULL);
      if (ioc_init (nand0) == 0) {
        ioc_read_info (&info, nand0);
        if (usb_initialized == __FALSE) {
          usbd_init();                              /* Init USB Device        */
          usb_initialized = __TRUE;
        } else {
          usbd_msc_init();
        }
        usbd_connect(__TRUE);                       /* USB Device Connect     */
        GLCD_DisplayString (5, 0, 1, "   PC Interface     ");
      }
      else {
        LED_On (LED_MSK);
        GLCD_DisplayString (5, 0, 1, "   Card Failure!    ");
      }
      while (!WakeUp);
    }
  }
}
