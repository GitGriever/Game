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
#include <stm32f4xx.h>                  /* STM32F4xx Definitions             */
#include "Serial.h"
#include "LED.h"
#include <string.h>


BOOL tick;
BOOL LEDrun;
U32  dhcp_tout;

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Configure LEDs */
  LED_Init();

  /* Configure serial output */
  SER_Init();

  /* Setup and enable the SysTick timer for 10ms. */
  SysTick->LOAD = (SystemCoreClock / 100) - 1;
  SysTick->CTRL = 0x05;

  LEDrun = __TRUE;
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  /* Turn LEDs ON or OFF */
  int i;

  for (i = 0; i < LED_Num(); i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}


/*--------------------------- AD_in -----------------------------------------*/

U16 AD_in (U32 ch) {
  /* Read ARM Analog Input */
  U16 adval = 0;

  if (ch == 0) {
    /* No potentiometer available, generate a random number. */
    adval = SysTick->VAL & 0x3FF;
  }
  return (adval);
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */
  static U32 time;

  if (SysTick->CTRL & 0x10000) {
    time++;
    if (time == 10) {
      /* Timer tick every 100 ms */
      time = 0;
      timer_tick ();
      tick = __TRUE;
    }
  }
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */
  if (ch == '\n')  {
    SER_PutChar (0x0D);
  }
  SER_PutChar (ch & 0xFF);
  return (ch);
}


/*--------------------------- dhcp_check ------------------------------------*/

static void dhcp_check () {
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }
  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
  }
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[6] = { 0x01,0x00,0x00, 0x02, 0x00,0x00 };
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
  }
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_TcpNet ();

  dhcp_tout = DHCP_TOUT;
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
