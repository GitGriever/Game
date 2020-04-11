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

#include <stm32f10x.h>                      /* stm32f10x Definitions */

#include <RTL.h>
#include <File_Config.h>
#include <rl_usb.h>

#include "SD_File.h"
#include "Memory.h"
#include "LCD.h"

FAT_VI *mc0;                                /* Media Control Block */
Media_INFO info;

/*------------------------------------------------------------------------------
  WKUP Key
 *------------------------------------------------------------------------------*/
volatile BOOL WakeUp   = __FALSE;           /* WakeUp Event */
volatile BOOL WKUP_Key = __FALSE;           /* WakeUp Key Active */


/*------------------------------------------------------------------------------
  Systick Interrupt Handler (happens every 50ms)
 *------------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  BOOL key;

  key = ((GPIOA->IDR & (1 << 0)) == 0);
  if (key && !WKUP_Key) {
    WakeUp = __TRUE;
  }
  WKUP_Key = key;
}


/*------------------------------------------------------------------------------
  Initialize GPIOs for LEDs and PushButtons
 *------------------------------------------------------------------------------*/
void init_io (void) {

  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                    /* Enable GPIOB clock          */

  GPIOB->ODR   &= ~(0xFF << 8);
  GPIOB->CRH   &= ~0xFFFFFFFF;                   /* PB.8..15 clear              */
  GPIOB->CRH   |=  0x33333333;                   /* PB.8..15 is output          */

  /* configure Push Buttons */
  RCC->APB2ENR |= ((1UL << 2) | (1UL << 4));     /* enable GPIOA, GPIOC clock   */

  GPIOA->CRL   &= ~0x0000000F;                   /* PA.0 clear                  */
  GPIOA->CRL   |=  0x00000004;                   /* PA.0 is Floating Input      */

  GPIOC->CRH   &= ~0x00F00000;                   /* PC.13 clear                 */
  GPIOC->CRH   |=  0x00400000;                   /* PC.13 is Floating Input     */
}


/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
void LED_On (unsigned int value) {

  GPIOB->BSRR = (value);                    /* Turn On  LED */
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
void LED_Off (unsigned int value) {

  GPIOB->BRR  = (value);                    /* Turn Off LED */
}


/*------------------------------------------------------------------------------
  Main Program
 *------------------------------------------------------------------------------*/
int main (void) {
  BOOL usb_initialized = __FALSE;

  init_io ();
  init_serial ();

  SysTick_Config(SystemCoreClock / 20);     /* Generate interrupt each 50 ms */

  lcd_init();                               /* LCD Initialization */
  lcd_clear();
  lcd_print ("  MCBSTM32 MSD  ");

  while (1) {
    if (WakeUp) {
      WakeUp = __FALSE;
      usbd_connect(__FALSE);                /* USB Device Disconnect */
      LED_Off(LED_MSK);
      set_cursor (0, 1);
      lcd_print (" Term Interface ");       /* Term Interface */
      sd_main();
    } else {
      mc0 = ioc_getcb (NULL);
      if (ioc_init (mc0) == 0) {
        ioc_read_info (&info, mc0);
        if (usb_initialized == __FALSE) {
          usbd_init();                      /* USB Device Initialization */
          usb_initialized = __TRUE;
        } else {
          usbd_msc_init();
        }
        usbd_connect(__TRUE);               /* USB Device Connect */
        set_cursor (0, 1);
        lcd_print ("  PC Interface  ");     /* PC Interface */
      } else {
        set_cursor (0, 1);
        lcd_print (" Card Failure!  ");
      }
      while (!WakeUp);
    }
  }
}
