/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for STmicroelectronics STM32F4xx
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifdef __DBG_ITM
  ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

#else
  RCC->APB2ENR  |= (1UL << 4);          /* Enable USART1 clock                */
  RCC->AHB1ENR  |= (1UL << 1);          /* Enable GPIOB clock                 */

  /* Connect PB6 and PB7 to Alternate Function 7 (AF7) */
  GPIOB->MODER  &= (3 << 2*6) | (3 << 2*7);
  GPIOB->MODER  |= (2 << 2*6) | (2 << 2*7);
  GPIOB->AFR[0] &= ~0xFF000000;
  GPIOB->AFR[0] |=  0x77000000;

  /* Configure USART1: 115200 baud @ 84MHz, 8 bits, 1 stop bit, no parity     */
  USART1->BRR = (45 << 4) | 9;
  USART1->CR3 = 0x0000;
  USART1->CR2 = 0x0000;
  USART1->CR1 = 0x200C;
#endif
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
#ifdef __DBG_ITM
  int i;
  ITM_SendChar (ch & 0xFF);
  for (i = 10000; i; i--);
#else
  while (!(USART1->SR & 0x0080));
  USART1->DR = (ch & 0xFF);
#endif  

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART1->SR & 0x0020)
    return (USART1->DR);
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
