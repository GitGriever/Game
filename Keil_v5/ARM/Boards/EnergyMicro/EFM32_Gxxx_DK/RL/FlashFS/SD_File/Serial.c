/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.c
 *      Purpose: Serial Input Output for NXP NXP LPC178x/7x
 *      Note(s): Possible defines to select the used communication interface:
 *               __DBG_ITM   - ITM SWO interface (USART is used by default)
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2012 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

/*-----------------------------------------------------------------------------
 *       SER_Init:  Initialize Serial Interface
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;

  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_USART1, true);

  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* Use default location 0: TX - Pin C0, RX - Pin C1 */
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);
  /* Define input, no filtering */
  GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 0);

  /* Configure USART for basic async operation */
  init.enable = usartDisable;
  USART_InitAsync(USART1, &init);

  /* Enable pins at default location */
  USART1->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

  /* Finally enable it */
  USART_Enable(USART1, usartEnable);
}


/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {

  while (!(USART1->STATUS & USART_STATUS_TXBL));
  USART1->TXDATA = (uint32_t)ch;

  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from the Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {

  if (USART1->STATUS & USART_STATUS_RXDATAV) {
    return (USART1->RXDATA);
  }

  return (-1);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
