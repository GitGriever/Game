/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SERIAL.C
 *      Purpose: Serial Input Output for Luminary LM3S6965
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <LM3Sxxxx.H>

/*----------------------------------------------------------------------------
 *       init_serial:  Initialize Serial Interface
 *---------------------------------------------------------------------------*/
void init_serial (void) {
  /* Initialize the serial interface */

  /* Configure UART0 for 115200 baud. */
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
  SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);
  GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSet(UART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                     UART_CONFIG_STOP_ONE |
                                     UART_CONFIG_PAR_NONE));
}

/*----------------------------------------------------------------------------
 *       sendchar:  Write a character to Serial Port
 *---------------------------------------------------------------------------*/
int sendchar (int ch) {
  if (ch == '\n')  {
    UARTCharPut (UART0_BASE, '\r');        /* output CR                    */
  }
  UARTCharPut (UART0_BASE, ch);
  return (ch);
}

/*----------------------------------------------------------------------------
 *       getkey:  Read a character from Serial Port
 *---------------------------------------------------------------------------*/
int getkey (void) {
  return (UARTCharGet (UART0_BASE));
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
