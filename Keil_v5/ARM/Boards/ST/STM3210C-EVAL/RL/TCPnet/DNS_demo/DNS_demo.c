/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    DNS_DEMO.C
 *      Purpose: DNS Resolver demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <stm32f10x.h>
#include "GLCD.h"

BOOL tick;
BOOL tick2;

char const *hosts[] = {
  "www.google.com",
  "www.keil.com",
  "www.microsoft.com",
  "www.yahoo.com",
  "www.notexisting.site",
  "192.168.0.253",
  "www.google-.com",
  "goble"
};
unsigned int index;
unsigned int delay;

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  SystemCoreClockUpdate();
  RCC->APB2ENR |= 0x00000261;

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

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_init();
  GLCD_clear(White);
  GLCD_setTextColor(Blue);
  GLCD_displayStringLn(Line2, "       RL-ARM");
  GLCD_displayStringLn(Line3, "    DNS example");
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


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick  = __TRUE;
    tick2 = __TRUE;
  }
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[8] = { 0x01,0x03,0x07,0x0F,0x0E,0x0C,0x08,0x00 };
  static U32 cnt;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    LED_out (led_val[cnt]);
    if (++cnt >= sizeof(led_val)) {
      cnt = 0;
    }
  }
}


/*--------------------------- next_host -------------------------------------*/

static void next_host () {
  /* Select next host from the table. */

  if (++index == sizeof(hosts)/sizeof(hosts[0])) {
    index = 0;
  }
}


/*--------------------------- dns_cbfunc ------------------------------------*/

static void dns_cbfunc (unsigned char event, unsigned char *ip) {
  /* This function is called by the DNS Client when dns event occurs. */

  switch (event) {
    case DNS_EVT_SUCCESS:
      /* Host Address successfully resolved. When IP address is already */
      /* cached, there is no DNS Request sent to remote DNS Server.     */
      printf("IP Address    : %d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);
      break;

    case DNS_EVT_NONAME:
      /* Host Name does not exist in DNS record database. */
      printf("Host name does not exist.\n");
      break;

    case DNS_EVT_TIMEOUT:
      /* All DNS Resolver retries used up and timeouts expired. */
      printf("DNS Resolver Timeout expired, Host Address not resolved.\n");
      break;

    case DNS_EVT_ERROR:
      /* DNS Protocol Error, invalid or corrupted reply received. */
      printf("DNS Resolver Protocol Error, Host Address not resolved.\n");
      delay = 0;
      return;
  }
  next_host();
  delay = 30;
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */
  int res;

  init ();
  init_display ();
  init_TcpNet ();

  printf ("\nDNS Resolver Demo Example\n");

  index = 0;
  delay = 0;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
    if (tick2 == __FALSE) {
      continue;
    }
    tick2 = __FALSE;
    if (++delay == 50) {
      /* After 5 seconds start DNS Resolver. */
      printf ("\nResolving host: %s\n",hosts[index]);
      res = get_host_by_name ((U8 *)hosts[index],dns_cbfunc);
      switch (res) {
        case DNS_RES_OK:
          /* Resolver started, wait for callback event. */
          break;

        case DNS_ERROR_BUSY:
          /* Busy, retry on next tick. */
          delay--;
          break;

        case DNS_ERROR_LABEL:
          printf ("Invalid label specified!\n");
          next_host();
          delay = 30;
          break;

        case DNS_ERROR_NAME:
          printf ("Hostname is not valid!\n");
          next_host();
          delay = 30;
          break;

        case DNS_ERROR_NOSRV:
          printf ("DNS Server unknown!\n");
          delay = 0;
          break;

        case DNS_ERROR_PARAM:
          printf ("Invalid parameters!\n");
          delay = 0;
          break;
      }
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
