/*----------------------------------------------------------------------------
 * Name:    Setup.c
 * Purpose: Setup Functions
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "STM32F10x.h"




void CLK_Init (void)
{
  RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                    */
  RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                      */
  RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                    */
  RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4                   */

  *(vu32 *)0x40022000 = 0x01;           /* Flash 2 wait state                 */
}


void LED_Init (void) {
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable GPIOB clock                                                       */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PB8..PB15 as outputs push-pull, max speed 50 MHz               */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | 
                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                 GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
