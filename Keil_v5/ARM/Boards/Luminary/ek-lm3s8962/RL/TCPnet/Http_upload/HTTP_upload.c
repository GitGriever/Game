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
#include <LM3Sxxxx.H>
#include <string.h>
#include "..\..\..\rit128x96x4.h"

BOOL LCDupdate;
BOOL tick;
U32  dhcp_tout;
U8   lcd_text[4][16+1] = {"    RL-ARM",       /* Buffer for LCD text         */
                          "HTTP file upload",
                          " ",
                          "Waiting for DHCP"};
extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

static void init_io (void);
static void init_display (void);

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */ 

  init_io ();
  init_display ();
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 100ms. */
  SysTickPeriodSet(SysCtlClockGet() / 10);
  SysTickEnable();
}

/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if ((HWREG (NVIC_ST_CTRL) >> 16) & 1) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}


/*--------------------------- init_io ---------------------------------------*/

static void init_io () {

  /* Set the clocking to run from the PLL at 50 MHz */
  SysCtlClockSet (SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
                  SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  /* Configure the GPIO for the LED. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);
  GPIOPadConfigSet (GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
                    GPIO_PIN_TYPE_STD);
  GPIODirModeSet (GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);

  /* Configure UART0 for 115200 baud. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
  GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSet(UART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                     UART_CONFIG_STOP_ONE |
                                     UART_CONFIG_PAR_NONE));
  UARTEnable(UART0_BASE);
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
  GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_0, val & 1);
}


/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update LCD Module display text. */

  RIT128x96x4Clear ();
  RIT128x96x4StringDraw ((const char *)&lcd_text[0], 20, 15, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[1], 20, 30, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[2], 20, 60, 11);
  RIT128x96x4StringDraw ((const char *)&lcd_text[3], 20, 75, 11);
  /* Move cursor away from screen to avoid display flickering. */
  RIT128x96x4StringDraw (" ", 128, 96, 1);
  LCDupdate =__FALSE;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* OLED Module init */

  RIT128x96x4Init(1000000);
  upd_display ();
}


/*--------------------------- dhcp_check ------------------------------------*/

static void dhcp_check () {
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }
  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
    sprintf((char *)lcd_text[2],"IP address:");
    sprintf((char *)lcd_text[3],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
    sprintf((char *)lcd_text[3]," DHCP failed    " );
    LCDupdate = __TRUE;
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
    sprintf((char *)lcd_text[2],"IP address:");
    sprintf((char *)lcd_text[3],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
  }
}


/*--------------------------- blink_led -------------------------------------*/

static void blink_led () {
  /* Blink the LEDs on an eval board */
  static U32 LEDstat = 1;

  if (tick == __TRUE) {
    /* Every 100 ms */
    tick = __FALSE;
    LEDstat = ~LEDstat & 0x01;
    LED_out (LEDstat);
    if (LCDupdate == __TRUE) {
      upd_display ();
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
