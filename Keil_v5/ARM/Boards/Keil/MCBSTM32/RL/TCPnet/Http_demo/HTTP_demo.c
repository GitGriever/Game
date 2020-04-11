/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_DEMO.C
 *      Purpose: HTTP Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <stm32f10x.h>                        /* STM32F10x definitions       */
#include "LCD.h"

#define SYSTICK         72000000              /* SysTick count frequency     */

BOOL LEDrun;
BOOL LCDupdate;
BOOL tick;
U8   lcd_text[2][16+1] = {"    RL-ARM",       /* Buffer for LCD text         */
                          "  HTTP example"};

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SYSTICK / 10) - 1;
  SysTick->CTRL = 0x05;

  /* configure LEDs */
  RCC->APB2ENR |= (1UL << 3);                 /* Enable GPIOB clock          */

  GPIOB->ODR   &= ~0x0000FF00;                /* switch off LEDs             */
  GPIOB->CRH   &= ~0xFFFFFFFF;                /* PB.8..15 clear              */
  GPIOB->CRH   |=  0x33333333;                /* PB.8..15 is output          */

  /* configure Push Buttons */
  RCC->APB2ENR |= ((1UL << 2) | (1UL << 4));  /* enable GPIOA, GPIOC clock   */

  GPIOA->CRL   &= ~0x0000000F;                /* PA.0 clear                  */
  GPIOA->CRL   |=  0x00000004;                /* PA.0 is Floating Input      */

  GPIOC->CRH   &= ~0x00F00000;                /* PC.13 clear                 */
  GPIOC->CRH   |=  0x00400000;                /* PC.13 is Floating Input     */

  /* Configure ADC1 input. */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  ADC1->SQR1  = 0x00000000;
  ADC1->SMPR2 = 0x00000028;
  ADC1->SQR3  = 0x00000001;
  ADC1->CR1   = 0x00000100;
  ADC1->CR2   = 0x000E0003;
  ADC1->CR2  |= 0x00500000;
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  val  <<= 8;
  GPIOB->BSRR = val;
  GPIOB->BRR  = val ^ 0xFF00;
}


/*--------------------------- AD_in -----------------------------------------*/

U16 AD_in (U32 ch) {
  /* Read ARM Analog Input */
  U32 val = 0;

  if (ch == 1) {
    while(!(ADC1->SR & ADC_SR_EOC));
    val = ADC1->DR & ADC_DR_DATA;
  }
  return (val);
}


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) {
  /* Read ARM Digital Input */
  U32 val = 0;

  if ((GPIOA->IDR & (1 << 0)) == 0) {
    /* Wkup button */
    val |= 0x01;
  }
  if ((GPIOC->IDR & (1 << 13)) == 0) {
    /* Tamp button */
    val |= 0x02;
  }
  return (val);
}


/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update LCD Module display text. */

  lcd_clear ();
  lcd_print ((char *)lcd_text[0]);
  set_cursor (0,1);
  lcd_print ((char *)lcd_text[1]);
  LCDupdate = __FALSE;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module.2x16 init*/

  lcd_init ();
  upd_display ();
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
                           0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
  static U32 cnt;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    if (LEDrun == __TRUE) {
      LED_out (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
      }
    }
    if (LCDupdate == __TRUE) {
      upd_display ();
    }
  }
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_display ();
  init_TcpNet ();

  /* Enable PPP server to accept incomming connections. */
  ppp_listen ("Keil", "test");

  LEDrun = __TRUE;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
