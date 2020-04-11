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
#include <a2fxxxm3.h>

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

  /* Disable the watchdog. */
  WATCHDOG->WDOGENABLE = 0x4C6E55FA;
  SYSREG->SOFT_RST_CR |= 0x00080000;

  /* Configure the GPIO for LEDs. */
  GPIO->GPIO_0_CFG = 5;
  GPIO->GPIO_1_CFG = 5;
  GPIO->GPIO_2_CFG = 5;
  GPIO->GPIO_3_CFG = 5;
  GPIO->GPIO_4_CFG = 5;
  GPIO->GPIO_5_CFG = 5;
  GPIO->GPIO_6_CFG = 5;
  GPIO->GPIO_7_CFG = 5;
  GPIO->GPIO_OUT  |= 0xFF;

  /* Configure UART0 for 115200 baud. */
  SYSREG->SOFT_RST_CR &= ~0x00000080;
  UART0->LCR = 0x83;                         /* 8 bits, no Parity, 1 Stop bit*/
  UART0->DLR = 54;                           /* for 100MHz PCLK0 clock       */
  UART0->DMR = 0;
  UART0->LCR = 0x03;                         /* DLAB = 0                     */

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemFrequency / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  GPIO->GPIO_OUT = (GPIO->GPIO_OUT & ~0xFF) | ~val;
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
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART0->LSR & 0x20));
    UART0->THR = '\r';                      /* output CR                    */
  }
  while (!(UART0->LSR & 0x20));
  return (UART0->THR = ch);
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
