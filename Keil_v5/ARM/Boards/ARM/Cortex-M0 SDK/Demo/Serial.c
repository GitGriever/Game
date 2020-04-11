/******************************************************************************/
/* Serial.c: Cortex-M0 System Design Kit Low Level Serial Routines            */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2011 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <CMSDK_CM0.h>
/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init(void)
{
  CMSDK_UART2->BAUDDIV = 16;
  CMSDK_UART2->CTRL    = 0x41;          /* High speed test mode, TX only */
  CMSDK_GPIO1->ALTFUNCSET = (1<<5);
  return;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar(int c)
{
  while ((CMSDK_UART2->STATE & 1));     /* Wait if Transmit Holding register is full */
  CMSDK_UART2->DATA = c;                /* write to transmit holding register		 */
  return (c);
}
/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) 
{
  while ((CMSDK_UART2->STATE & 2)==0);  /* Wait if Receive Holding register is empty */
  return (CMSDK_UART2->DATA);
}


