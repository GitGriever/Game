/*----------------------------------------------------------------------------
 * Name:    BTN.c
 * Purpose: low Level Push Button functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LM4Fxxxx.H>


/*----------------------------------------------------------------------------
  initialize Push Button Pins (PF.4)
 *----------------------------------------------------------------------------*/
void BTN_Init (void) {

  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
  ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4));
}
