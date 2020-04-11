/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_DEMO.C
 *      Purpose: HTTP Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
#include <XMC4500.h>                          /* XMC4500 Definitions         */
#include "Serial.h"
#include "ADC.h"
#include "LED.h"
#include <string.h>

BOOL LEDrun;
BOOL tick;
U32  dhcp_tout;
U8   pbstat;

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

static void init_io (void);

#ifdef RTX_KERNEL
 U64 tcp_stack[800/8];                        /* A bigger stack for tcp_task */

 /* Forward references */
 __task void init       (void);
 __task void blink_led  (void);
 __task void timer_task (void);
 __task void tcp_task   (void);
#endif

/*--------------------------- init ------------------------------------------*/

#ifndef RTX_KERNEL

static void init () {
  /* Add System initialisation code here */ 


  /* update SystemCoreClock value */
  SystemCoreClockUpdate();

  init_io ();
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}

#else

__task void init (void) {
  /* Add System initialisation code here */ 

  /* update SystemCoreClock value */
  SystemCoreClockUpdate();

  init_io ();
  init_TcpNet ();

  /* Initialize Tasks */
  os_tsk_prio_self (100);
  os_tsk_create (blink_led, 20);
  os_tsk_create (timer_task, 30);
  os_tsk_create_user (tcp_task, 0, &tcp_stack, sizeof(tcp_stack));
  os_tsk_delete_self();
}

#endif

/*--------------------------- timer_poll ------------------------------------*/

#ifndef RTX_KERNEL

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}

#else

__task void timer_task (void) {
  /* System tick timer task */
  os_itv_set (10);
  while (1) {
    timer_tick ();
    tick = __TRUE;
    os_itv_wait ();
  }
}

#endif

/*--------------------------- init_io ---------------------------------------*/

static void init_io () {

  /* Configure LEDs */
  LED_Init();

  /* Configure buttons */
  pbstat = 0;

  /* Configure serial output */
  SER_Init();

  /* Setup and initialize potentiometer and ADC converter */
  ADC_Init();
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f) {
  /* Debug output to serial port. */
  if (ch == '\n')  {
    SER_PutChar (0x0D);
  }
  SER_PutChar (ch & 0xFF);
  return (ch);
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
  ADC_StartCnv();
  return (ADC_GetVal());
}


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) {
  /* Read ARM Digital Input */

  /* Simulate push buttons for WEB server page 'Buttons'. */
  return (pbstat);
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

#ifndef RTX_KERNEL

static void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[10] = { 0x01,0x01,0x03,0x07,0x07,0x06,0x04,0x00,0x08,0x00 };
  static U32 cnt;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    if (LEDrun == __TRUE) {
      LED_out (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
        pbstat++;
      }
    }
  }
}

#else

__task void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[10] = { 0x01,0x01,0x03,0x07,0x07,0x06,0x04,0x00,0x08,0x00 };
  U32 cnt = 0;

  LEDrun = __TRUE;
  while(1) {
    /* Every 100 ms */
    if (LEDrun == __TRUE) {
      LED_out (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
        pbstat++;
      }
    }
    os_dly_wait(10);
  }
}

#endif


/*---------------------------------------------------------------------------*/

#ifndef RTX_KERNEL

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  LEDrun = __TRUE;
  dhcp_tout = DHCP_TOUT;
  while (1) {
    timer_poll ();
    main_TcpNet ();
    dhcp_check ();
    blink_led ();
  }
}

#else

__task void tcp_task (void) {
  /* Main Thread of the TcpNet. This task should have */
  /* the lowest priority because it is always READY. */
  dhcp_tout = DHCP_TOUT;
  while (1) {
    main_TcpNet();
    dhcp_check ();
    os_tsk_pass();
  }
}


int main (void) {
  /* Start with 'init' task. */
  os_sys_init(init);
  while(1);
}

#endif


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
