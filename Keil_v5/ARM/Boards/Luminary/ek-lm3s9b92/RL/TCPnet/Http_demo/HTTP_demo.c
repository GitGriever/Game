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
#include <LM3Sxxxx.H>

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
  SysTickPeriodSet(SysCtlClockGet() / 10);
  SysTickEnable();
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

  if ((HWREG (NVIC_ST_CTRL) >> 16) & 1) {
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

  /* Set the clocking to run from the PLL at 50 MHz */
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                 SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  /* Configure the GPIO for the LED and Push Buttons. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOD);
  GPIOPadConfigSet (GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
                    GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet (GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);
  GPIODirModeSet (GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
  GPIODirModeSet (GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);

  /* Configure UART0 for 115200 baud. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
  GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSet(UART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                     UART_CONFIG_STOP_ONE |
                                     UART_CONFIG_PAR_NONE));
  UARTEnable(UART0_BASE);
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    UARTCharPut (UART0_BASE, '\r');          /* output CR                    */
  }
  UARTCharPut (UART0_BASE, ch);
  return (ch);
}


/*--------------------------- LED_out ---------------------------------------*/

void LED_out (U32 val) {
  GPIOPinWrite (GPIO_PORTD_BASE, GPIO_PIN_0, val & 1);
}


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) {
  /* Read ARM Digital Input */
  U32 val = 0;

  if (GPIOPinRead (GPIO_PORTB_BASE, GPIO_PIN_4) == 0) {
    val |= 0x01;
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
  static U32 LEDstat = 1;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    if (LEDrun == __TRUE) {
      LEDstat = ~LEDstat & 0x01;
      LED_out (LEDstat);
    }
  }
}

#else

__task void blink_led () {
  /* Blink the LEDs on an eval board */
  U32 LEDstat = 1;

  LEDrun = __TRUE;
  while(1) {
    /* Every 100 ms */
    if (LEDrun == __TRUE) {
      LEDstat = ~LEDstat & 0x01;
      LED_out (LEDstat);
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
