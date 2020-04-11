/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SERIAL.C
 *      Purpose: Serial Input Output for STmicroelectronics STM32
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <STM32F10x.h>                              /* STM32F10x definitions */


/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USART 
 *----------------------------------------------------------------------------*/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))


/*----------------------------------------------------------------------------
  Initialize USART pins, Baudrate
 *---------------------------------------------------------------------------*/
void init_serial (void) {
  int i;

  RCC->APB2ENR |=  (   1UL <<  0);        /* enable clock Alternate Function */
  AFIO->MAPR   &= ~(   1UL <<  2);        /* clear USART1 remap              */

  RCC->APB2ENR |=  (   1UL <<  2);        /* enable GPIOA clock              */
  GPIOA->CRH   &= ~(0xFFUL <<  4);        /* clear PA9, PA10                 */
  GPIOA->CRH   |=  (0x0BUL <<  4);        /* USART1 Tx (PA9) output push-pull*/
  GPIOA->CRH   |=  (0x04UL <<  8);        /* USART1 Rx (PA10) input floating */

  RCC->APB2ENR |=  (   1UL << 14);        /* enable USART#1 clock            */

  USART1->BRR  = __USART_BRR(72000000UL, 115200UL);  /* 115200 baud @ 72MHz   */
  USART1->CR1   = ((   1UL <<  2) |       /* enable RX                       */
                   (   1UL <<  3) |       /* enable TX                       */
                   (   0UL << 12) );      /* 1 start bit, 8 data bits        */
  USART1->CR2   = 0x0000;                 /* 1 stop bit                      */
  USART1->CR3   = 0x0000;                 /* no flow control                 */
  for (i = 0; i < 0x1000; i++) __NOP();   /* avoid unwanted output           */
  USART1->CR1  |= ((   1UL << 13) );      /* enable USART                    */
}


/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
int sendchar (int ch) {
  if (ch == '\n')  {
    while (!(USART1->SR & (1UL << 7)));
    USART1->DR = '\r';
  }
  while (!(USART1->SR & (1UL << 7)));
  return (USART1->DR = (ch & 0x1FF));
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while (!(USART1->SR & (1UL << 5)));
  return ((int)(USART1->DR & 0x1FF));
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
