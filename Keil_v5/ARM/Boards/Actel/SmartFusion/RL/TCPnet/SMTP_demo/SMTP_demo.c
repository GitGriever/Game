/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    SMTP_DEMO.C
 *      Purpose: SMTP Client demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <a2fxxxm3.h>

/* Enter your valid SMTP Server IP address here. */
U8 srv_ip[4] = { 192,168,0,253 };

BOOL tick;
BOOL tick2;
BOOL sent;
U32  delay;

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


/*--------------------------- AD_in ----------------------------------------*/

U16 AD_in (U32 ch) {
  /* Read ARM Analog Input */

  /* Return something. */
  return (ch + 10);
}


/*---------------------------------------------------------------------------*/

static void smtp_cback (U8 code) {
  /* This function is called by TcpNet to inform about SMTP status. */
  switch (code) {
    case SMTP_EVT_SUCCESS:
      printf ("Email successfully sent\n");
      sent = __TRUE;
      break;
    case SMTP_EVT_TIMEOUT:
      /* Timeout, try again. */
      printf ("Mail Server timeout.\n");
      delay = 0;
      break;
    case SMTP_EVT_ERROR:
      /* Error, try again. */
      printf ("Error sending email.\n");
      delay = 0;
      break;
  }
}

/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_TcpNet ();

  delay = 0;
  sent  = __FALSE;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    blink_led ();
    if (tick2 == __FALSE) {
      continue;
    }
    tick2 = __FALSE;
    if (sent == __TRUE) {
      continue;
    }
    if (++delay == 100) {
      /* After 10 seconds send an email. */
      printf ("Sending email...\n");
      smtp_connect ((U8 *)&srv_ip,25,smtp_cback);
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
