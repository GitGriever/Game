/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SERIAL.C
 *      Purpose: Serial Input Output for STmicroelectronics STM32
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stm32f10x_cl.h>

/*----------------------------------------------------------------------------
 *       init_serial:  Initialize Serial Interface
 *---------------------------------------------------------------------------*/
void init_serial (void) {
  /* Initialize the serial interface */

  /* Configure UART2 for 115200 baud. */
  RCC->APB2ENR |= 0x00000021;
  AFIO->MAPR   |= 0x00000008;
  GPIOD->CRL   &= 0xF00FFFFF;
  GPIOD->CRL   |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;
}

/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
int sendchar (int ch) {
  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  while (!(USART2->SR & 0x0020));
  return (USART2->DR);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
