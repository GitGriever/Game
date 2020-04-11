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
#include <stm32f10x.h>
#include "GLCD.h"
#include <string.h>

BOOL tick;
BOOL LCDupdate;
U32  dhcp_tout;
U8   lcd_text[2][16+1] = {" ",                /* Buffer for LCD text         */
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
  finit (NULL);
  init_TcpNet ();

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick = __TRUE;
  }
}


/*--------------------------- init_io ---------------------------------------*/

static void init_io () {

  /* Set the clocks. */
  SystemCoreClockUpdate();
  RCC->APB2ENR |= 0x00000060;

  /* Configure the GPIO for LEDs. */
  GPIOD->CRL &= 0xFFF00FFF;
  GPIOD->CRL |= 0x00033000;
  GPIOD->CRH &= 0xFF0FFFFF;
  GPIOD->CRH |= 0x00300000;
  GPIOE->CRH &= 0xF0FFFFFF;
  GPIOE->CRH |= 0x03000000;
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
  val <<= 8;
  GPIOE->BSRR = val;
  GPIOE->BRR  = val ^ 0xFF00;
}

unsigned int SP3_CR1_GLCD;

/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update LCD Module display text. */
unsigned int SP3_CR1 = SPI3->CR1;

SPI3->CR1 = 0;
SPI3->CR1 = SP3_CR1_GLCD;

  GLCD_clearLn (Line5);
  GLCD_displayStringLn(Line5, lcd_text[0]);
  GLCD_clearLn (Line6);
  GLCD_displayStringLn(Line6, lcd_text[1]);

SPI3->CR1 = 0;
SPI3->CR1 = SP3_CR1;

  LCDupdate =__FALSE;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */
  
  GLCD_init();

SP3_CR1_GLCD = SPI3->CR1;
  
  GLCD_clear(White);
  GLCD_setTextColor(Blue);
  GLCD_displayStringLn(Line1, "       RL-ARM");
  GLCD_displayStringLn(Line2, " HTTP file upload");

  /* Uninit display, SPI3 is shared with SD card. */
  SPI3->CR1 = 0x0000;

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
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
    sprintf((char *)lcd_text[1]," DHCP failed    " );
    LCDupdate = __TRUE;
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0],"IP address:");
    sprintf((char *)lcd_text[1],"%d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                               MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
  }
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
