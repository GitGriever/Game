/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    SNMP_DEMO.C 
 *      Purpose: SNMP Agent demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
#include <stm32f2xx.h>
#include "GLCD.h"


BOOL tick;
U32  dhcp_tout;
BOOL LCDupdate;
U8   lcd_text[2][16+1] = {" ",                /* Buffer for LCD text         */
                          "Waiting for DHCP"};

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

/*--------------------------- init ------------------------------------------*/

static void init () {
  /* Add System initialisation code here */

  /* Set the clocks. */
  RCC->APB1ENR |= (1UL << 19) ;    /* Enable UART 4 clock */
  RCC->AHB1ENR |= (1UL <<  0) |    /* Enable GPIOA clock  */
                  (1UL <<  2) |    /* Enable GPIOC clock  */
                  (1UL <<  6) |    /* Enable GPIOG clock  */
                  (1UL <<  8) ;    /* Enable GPIOI clock  */

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

  /* Configure Button pins (PG15, PC13, PA0) as floating inputs */
  GPIOG->MODER  &= ~(3UL << 2 * 15);
  GPIOC->MODER  &= ~(3UL << 2 * 13);
  GPIOA->MODER  &= ~(3UL << 2 *  0);

  /* Configure UART4 (PC10, PC11) for 115200 baud @ 30MHz */  
  UART4->BRR = (16 << 4) | 4;
  UART4->CR3 = 0x0000;    
  UART4->CR2 = 0x0000;
  UART4->CR1 = 0x200C;
  
  GPIOC->MODER  &= ~((3UL << 2 * 11) | (3UL << 2 * 10));
  GPIOC->MODER  |=   (2UL << 2 * 11) | (2UL << 2 * 10);  
  GPIOC->OTYPER &= ~((1UL <<     11) | (1UL <<     10));

  GPIOC->AFR[1] &= ~0x0000FF00;
  GPIOC->AFR[1] |=  0x00008800;

  /* Setup and enable the SysTick timer for 100ms. */
  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- fputc -----------------------------------------*/

int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(UART4->SR & 0x0080));
    UART4->DR = 0x0D;
  }
  while (!(UART4->SR & 0x0080));
  UART4->DR = (ch & 0xFF);
  return (ch);
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


/*--------------------------- get_button ------------------------------------*/

U8 get_button (void) {
  /* Read ARM Digital Input */
  U32 val = 0;

  if ((GPIOG->IDR & (1 << 15)) == 0) {
    /* Key button */
    val |= 0x01;
  }
  if ((GPIOC->IDR & (1 << 13)) == 0) {
    /* Tamper button */
    val |= 0x02;
  }
  if ((GPIOA->IDR & (1 << 0)) != 0) {
    /* Wakeup button */
    val |= 0x04;
  }

  return (val);
}


/*--------------------------- upd_display -----------------------------------*/

static void upd_display () {
  /* Update GLCD Module display text. */

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
  GLCD_DisplayString (2, 4, 1, "   RL-ARM");
  GLCD_DisplayString (3, 4, 1, "SNMP example");
}


/*--------------------------- timer_poll ------------------------------------*/

static void timer_poll () {
  /* System tick timer running in poll mode */

  if (SysTick->CTRL & 0x10000) {
    /* Timer tick every 100 ms */
    timer_tick ();
    tick  = __TRUE;
  }
}


/*--------------------------- dhcp_check ------------------------------------*/

static void dhcp_check () {
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }
  tick = __FALSE;
  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;
    sprintf((char *)lcd_text[0]," IP address:");
    sprintf((char *)lcd_text[1]," %d.%d.%d.%d", MY_IP[0], MY_IP[1],
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
    sprintf((char *)lcd_text[0]," IP address:");
    sprintf((char *)lcd_text[1]," %d.%d.%d.%d", MY_IP[0], MY_IP[1],
                                                MY_IP[2], MY_IP[3]);
    LCDupdate = __TRUE;
  }
}


/*---------------------------------------------------------------------------*/

int main (void) {
  /* Main Thread of the TcpNet */

  init ();
  init_display ();
  init_TcpNet ();

  dhcp_tout = DHCP_TOUT;
  LED_out (0x55);

  while (1) {
    timer_poll ();
    main_TcpNet ();
    dhcp_check ();
    if (LCDupdate == __TRUE) {
      upd_display ();
    }
  }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
