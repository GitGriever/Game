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

  RCC->APB2ENR |=  (   1UL <<  0);         /* enable clock Alternate Function */
  AFIO->MAPR   &= ~(   1UL <<  3);         /* clear USART2 remap              */

  RCC->APB2ENR |=  (   1UL <<  2);         /* enable GPIOA clock              */
  GPIOA->CRL   &= ~(0xFFUL <<  8);         /* clear PA2, PA3                  */
  GPIOA->CRL   |=  (0x0BUL <<  8);         /* USART2 Tx (PA2) output push-pull*/
  GPIOA->CRL   |=  (0x04UL << 12);         /* USART2 Rx (PA3) input floating  */

  RCC->APB1ENR |=  (   1UL << 17);         /* enable USART#2 clock            */

  USART2->BRR  = __USART_BRR(36000000UL, 115200UL);  /* 115200 baud @ 36MHz   */
  USART2->CR1   = ((   1UL <<  2) |       /* enable RX                       */
                   (   1UL <<  3) |       /* enable TX                       */
                   (   0UL << 12) );      /* 1 start bit, 8 data bits        */
  USART2->CR2   = 0x0000;                 /* 1 stop bit                      */
  USART2->CR3   = 0x0000;                 /* no flow control                 */
  for (i = 0; i < 0x1000; i++) __NOP();   /* avoid unwanted output           */
  USART2->CR1  |= ((   1UL << 13) );      /* enable USART                    */
}


/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
int sendchar (int ch) {
  if (ch == '\n') {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = '\r';
  }
  while (!(USART2->SR & USART_SR_TXE));
  return (USART2->DR = (ch & 0x1FF));
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while (!(USART2->SR & USART_SR_RXNE));
  return ((int)(USART2->DR & 0x1FF));
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
