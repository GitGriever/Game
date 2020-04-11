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
#include <Net_Config.h>
#include <a2fxxxm3.h>

BOOL LEDrun;
BOOL tick;
U32  dhcp_tout;

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

  init_io ();
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemFrequency / 10) - 1;
  SysTick->CTRL = 0x05;
}

#else

__task void init (void) {
  /* Add System initialisation code here */ 

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

  /* Configure the GPIO for Push Buttons */
  GPIO->GPIO_8_CFG = 2;
  GPIO->GPIO_9_CFG = 2;

  /* Configure UART0 for 115200 baud. */
  SYSREG->SOFT_RST_CR &= ~0x00000080;
  UART0->LCR = 0x83;                         /* 8 bits, no Parity, 1 Stop bit*/
  UART0->DLR = 54;                           /* for 100MHz PCLK0 clock       */
  UART0->DMR = 0;
  UART0->LCR = 0x03;                         /* DLAB = 0                     */
}


/*--------------------------- fputc -----------------------------------------*/

int fputc(int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART0->LSR & 0x20));
    UART0->THR = '\r';                      /* output CR                    */
  }
  while (!(UART0->LSR & 0x20));
  return (UART0->THR = ch);
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  GPIO->GPIO_OUT = (GPIO->GPIO_OUT & ~0xFF) | ~val;
}


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) {
  /* Read ARM Digital Input */
  U32 val = 0;

  if ((GPIO->GPIO_IN & 0x100) == 0) {
    val |= 0x01;
  }
  if ((GPIO->GPIO_IN & 0x200) == 0) {
    val |= 0x02;
  }
  return (val);
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
  }
}

#else

__task void blink_led () {
  /* Blink the LEDs on an eval board */
  const U8 led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
                           0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
  U32 cnt = 0;

  LEDrun = __TRUE;
  while(1) {
    /* Every 100 ms */
    if (LEDrun == __TRUE) {
      LED_out (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
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
