/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for STmicroelectronics STM32F4xx
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
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
  RCC->APB2ENR  |= (1UL << 5);          /* Enable USART6 clock                */
  RCC->AHB1ENR  |= (1UL << 2);          /* Enable GPIOC clock                 */

  /* Connect PC6 and PC7 to Alternate Function 8 (AF8) */
  GPIOC->AFR[0] &= ~0xFF000000;
  GPIOC->AFR[0] |=  0x88000000;
  GPIOC->MODER  &= (3 << 2*6) | (3 << 2*7);
  GPIOC->MODER  |= (2 << 2*6) | (2 << 2*7);

  /* Configure USART6: 115200 baud @ 84MHz, 8 bits, 1 stop bit, no parity     */
  USART6->BRR = (45 << 4) | 9;
  USART6->CR3 = 0x0000;
  USART6->CR2 = 0x0000;
  USART6->CR1 = 0x200C;
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
  while (!(USART6->SR & 0x0080));
  USART6->DR = (ch & 0xFF);
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
  if (USART6->SR & 0x0020)
    return (USART6->DR);
#endif
  return (-1);
}

/*-----------------------------------------------------------------------------
 * end of file
 *----------------------------------------------------------------------------*/
