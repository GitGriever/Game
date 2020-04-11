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
#include <stm32f2xx.h>
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
  RCC->APB1ENR |= (1UL << 19) ;    /* Enable UART 4 clock */
  RCC->AHB1ENR |= (1UL <<  0) |    /* Enable GPIOA clock  */
                  (1UL <<  2) |    /* Enable GPIOC clock  */
                  (1UL <<  6) |    /* Enable GPIOG clock  */
                  (1UL <<  8) ;    /* Enable GPIOI clock  */                  

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

  /* Configure UART4 (PC10, PC11) for 115200 baud @ 30MHz */
  UART4->BRR = (16 << 4) | 4;
  UART4->CR3 = 0x0000;
  UART4->CR2 = 0x0000;
  UART4->CR1 = 0x200C;
  
  GPIOC->MODER  &= ~((3UL << 2 * 11) | (3UL << 2 * 10));
  GPIOC->MODER  |=   (2UL << 2 * 11) | (2UL << 2 * 10);  
  GPIOC->OTYPER &= ~((1UL <<     11) | (1UL <<     10));

  GPIOC->AFR[1] &= ~0x0000FF00;
  GPIOC->AFR[1] |=  0x00008800;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (3, 4, 1, "   RL-ARM");
  GLCD_DisplayString (4, 4, 1, "DNS example");
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  volatile unsigned short *BSR;

  /* LD1 on/off */
   BSR  = (val & 1) ? &GPIOG->BSRRL : &GPIOG->BSRRH;
  *BSR |= (1 << 6);

  /* LD2 on/off */
   BSR  = (val & 2) ? &GPIOG->BSRRL : &GPIOG->BSRRH;
  *BSR |= (1 << 8);

  /* LD3 on/off */
   BSR  = (val & 4) ? &GPIOI->BSRRL : &GPIOI->BSRRH;
  *BSR |= (1 << 9);

  /* LD4 on/off */
   BSR  = (val & 8) ? &GPIOC->BSRRL : &GPIOC->BSRRH;
  *BSR |= (1 << 7);
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


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
#ifdef __DBG_ITM
  /* Debug output to ITM debug port */
  int i;
  if (ch == '\n') {
    ITM_SendChar (0x0D);
  }
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART4->SR & 0x0080));
    UART4->DR = 0x0D;
  }
  while (!(UART4->SR & 0x0080));
  UART4->DR = (ch & 0xFF);
#endif

  return (ch);
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
