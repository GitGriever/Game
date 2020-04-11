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
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <TMPM369.H>

#define  USER_BUTTON  0x01


/*----------------------------------------------------------------------------
  initialize Push Button Pins
 *----------------------------------------------------------------------------*/
void BTN_Init (void) {

  TSB_PC->FR1 &= ~USER_BUTTON;          /* Set PushButton pin func. as "Port" */
  TSB_PC->FR2 &= ~USER_BUTTON;          /* Set PushButton pin func. as "Port" */
  TSB_PC->CR  &= ~USER_BUTTON;          /* Disable PushButton pin as output   */
  TSB_PC->OD  &= ~USER_BUTTON;          /* Disable PushButton pin as op-drain */
  TSB_PC->PUP &= ~USER_BUTTON;          /* Disable PushButton pin pull ups    */
  TSB_PC->PDN &= ~USER_BUTTON;          /* Disable PushButton pin pull ups    */
  TSB_PC->IE  |=  USER_BUTTON;          /* Enable  PushButton pin as inputs   */
}


/*----------------------------------------------------------------------------
  Get Push Button status
 *----------------------------------------------------------------------------*/
unsigned int BTN_Get (void) {

  return (~(TSB_PC->DATA) & USER_BUTTON);
}
