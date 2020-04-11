/******************************************************************************/
/* Name:    Timer.c                                                           */
/* Purpose: Cortex-M0 System Design Kit low level 32-Bit Timer functions      */
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2011 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <CMSDK_CM0.h>
#include "Timer.h"
volatile uint32_t msT0Ticks = 0;                          /* counts 1ms timeTicks */
volatile uint32_t msT1Ticks = 0;                          /* counts 1ms timeTicks */

#define GPIO0_DATA (CMSDK_GPIO0->LB_MASKED[0x1])
#define GPIO1_DATA (CMSDK_GPIO1->LB_MASKED[0x1])


uint32_t Timer0_Init (uint32_t ticks)
{
  NVIC_EnableIRQ(TIMER0_IRQn);                                                    /* Enable Timer 0 Interrupt */
  CMSDK_TIMER0->RELOAD = ticks - 1;
  CMSDK_TIMER0->VALUE  = ticks - 1;
  CMSDK_TIMER0->CTRL   = (CMSDK_TIMER_CTRL_IRQEN_Msk | CMSDK_TIMER_CTRL_EN_Msk);  /* Timer 0 enabled in IRQ Mode*/
  return (0);                                                                     /* Function successful */
}

uint32_t Timer1_Init (uint32_t ticks)
{
  NVIC_EnableIRQ(TIMER1_IRQn);                                                    /* Enable Timer 1 Interrupt */
  CMSDK_TIMER1->RELOAD = ticks - 1;
  CMSDK_TIMER1->VALUE  = ticks - 1;
  CMSDK_TIMER1->CTRL   = (CMSDK_TIMER_CTRL_IRQEN_Msk | CMSDK_TIMER_CTRL_EN_Msk);  /* Timer 1 enabled in IRQ Mode*/
  return (0);                                                                     /* Function successful */
}


/* --------------------------------------------------------------- */
/*  Timer exception handlers                                       */
/* --------------------------------------------------------------- */

void TIMER0_Handler(void)
{
  if (msT0Ticks++ >= 100) {
    GPIO0_DATA ^= 0x1;      /* Toggle GPIO0.0 every 100 ticks*/
    msT0Ticks = 0;  
  }
  CMSDK_TIMER0->INTCLEAR = 1;
}

void TIMER1_Handler(void)
{
  if (msT1Ticks++ >= 100) {
    GPIO1_DATA ^= 0x1;      /* Toggle GPIO1.0 every 100 ticks */
    msT1Ticks = 0;  
  }
  CMSDK_TIMER1->INTCLEAR = 1;
}

