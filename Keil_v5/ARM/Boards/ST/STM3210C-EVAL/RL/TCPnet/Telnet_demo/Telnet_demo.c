/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    TELNET_DEMO.C
 *      Purpose: Telnet Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
#include <stm32f10x.h>
#include "GLCD.h"
#include <string.h>

BOOL LEDrun;
BOOL LCDupdate;
BOOL tick;
U32  dhcp_tout;
U8   lcd_text[2][16+1] = {" ",                /* Buffer for LCD text         */
                          "Waiting for DHCP"};

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */


/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  SystemCoreClockUpdate();
  RCC->APB2ENR |= 0x00000279;

  /* Configure the GPIO for LEDs. */
  GPIOD->CRL &= 0xFFF00FFF;
  GPIOD->CRL |= 0x00033000;
  GPIOD->CRH &= 0xFF0FFFFF;
  GPIOD->CRH |= 0x00300000;
  GPIOE->CRH &= 0xF0FFFFFF;
  GPIOE->CRH |= 0x03000000;

  /* Configure UART2 for 115200 baud. */
  AFIO->MAPR |= 0x00000008;
  GPIOD->CRL &= 0xF00FFFFF;
  GPIOD->CRL |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;

  /* Configure ADC.14 input. */
  GPIOC->CRL &= 0xFFF0FFFF;
  ADC1->SQR1  = 0x00000000;
  ADC1->SMPR1 = (5<<12);
  ADC1->SQR3  = (14<<0);
  ADC1->CR1   = 0x00000100;
  ADC1->CR2   = 0x000E0003;

  /* Reset calibration */
  ADC1->CR2  |= 0x00000008;
  while (ADC1->CR2 & 0x00000008);

  /* Start calibration */
  ADC1->CR2  |= 0x00000004;
  while (ADC1->CR2 & 0x00000004);
  ADC1->CR2  |= 0x00500000;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update LCD Module display text. */

  GLCD_clearLn (Line5);
  GLCD_displayStringLn(Line5, lcd_text[0]);
  GLCD_clearLn (Line6);
  GLCD_displayStringLn(Line6, lcd_text[1]);

  LCDupdate =__FALSE;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_init();
  GLCD_clear(White);
  GLCD_setTextColor(Blue);
  GLCD_displayStringLn(Line2, "       RL-ARM");
  GLCD_displayStringLn(Line3, "   Telnet example");

  upd_display ();
}


/*--------------------------- dhcp_check ------------------------------------*/

static void dhcp_check () {
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }
#ifdef RTX_KERNEL
  tick = __FALSE;
#endif
  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
    sprintf((char *)lcd_text[1]," DHCP failed    " );
    LCDupdate = __TRUE;
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
  }
}

/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  U32 rv;

  rv = 0;
  if (val & 0x01) rv |= 0x00004000;
  GPIOE->BSRR = rv;
  GPIOE->BRR  = rv ^ 0x00004000;

  rv = 0;
  if (val & 0x02) rv |= 0x00002000;
  if (val & 0x04) rv |= 0x00000008;
  if (val & 0x08) rv |= 0x00000010;
  GPIOD->BSRR = rv;
  GPIOD->BRR  = rv ^ 0x0002018;
}


/*--------------------------- AD_in -----------------------------------------*/

U16 AD_in (U32 ch) {
  /* Read ARM Analog Input */
  U32 val = 0;

  if (ch < 1) {
    val = ADC1->DR & 0x0FFF;
  }
  return (val);
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


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[8] = { 0x01,0x03,0x07,0x0F,0x0E,0x0C,0x08,0x00 };
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
  LEDrun = __TRUE;
  dhcp_tout = DHCP_TOUT;

  init_display ();
  init_TcpNet ();

  while (1) {
    timer_poll ();
    main_TcpNet ();
    dhcp_check ();
    blink_led ();
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
