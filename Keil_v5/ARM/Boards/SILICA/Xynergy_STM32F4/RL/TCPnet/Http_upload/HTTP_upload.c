/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_UPLOAD.C
 *      Purpose: HTTP File Upload example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
#include <stm32f4xx.h>
#include "LED.h"
#include <string.h>

BOOL tick;
U32  dhcp_tout;

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

static void init_io (void);

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */ 

  init_io ();
  finit (NULL);
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 10ms. */
  SysTick->LOAD = (SystemCoreClock / 100) - 1;
  SysTick->CTRL = 0x05;
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


/*--------------------------- init_io ---------------------------------------*/

static void init_io () {
  /* Init LEDs */
  LED_Init();
}


/*--------------------------- sendchar --------------------------------------*/

int sendchar (int ch)  {
  /* A dummy function for 'retarget.c' */
  return (ch);
}


/*--------------------------- getkey ----------------------------------------*/

int getkey (void) {
  /* A dummy function for 'retarget.c' */
  return (0);
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
    LED_out (led_val[cnt]);
    if (++cnt >= sizeof(led_val)) {
      cnt = 0;
    }
  }
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();

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
