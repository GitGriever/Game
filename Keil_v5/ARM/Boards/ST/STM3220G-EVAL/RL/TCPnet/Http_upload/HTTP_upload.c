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
#include <stm32f2xx.h>
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
  RCC->AHB1ENR |= (1UL <<  0) |    /* Enable GPIOA clock  */
                  (1UL <<  2) |    /* Enable GPIOC clock  */
                  (1UL <<  6) |    /* Enable GPIOG clock  */
                  (1UL <<  8) ;    /* Enable GPIOI clock  */
                    
  RCC->AHB1ENR |= (1UL <<  2) | (1UL <<  6) | (1UL <<  8);

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


/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update LCD Module display text. */

  GLCD_ClearLn (5, 1);
  GLCD_DisplayString (5, 0, 1, lcd_text[0]);
  GLCD_ClearLn (6, 1);
  GLCD_DisplayString (6, 0, 1, lcd_text[1]);

  LCDupdate =__FALSE;
}


/*--------------------------- init_display ----------------------------------*/

static void init_display () {
  /* LCD Module init */

  GLCD_Init ();
  GLCD_Clear (Blue);
  GLCD_SetBackColor (Blue);
  GLCD_SetTextColor (White);
  GLCD_DisplayString (1, 2, 1, "    RL-ARM");
  GLCD_DisplayString (2, 2, 1, "HTTP file upload");

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
