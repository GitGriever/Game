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


#define ADC1_DR_Address    ((u32)0x4001244C)

unsigned short int ADC_ConvertedValue;


void CLK_Init (void)
{
  RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK                    */
  RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK                      */
  RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2                    */
  RCC_ADCCLKConfig (RCC_PCLK2_Div4);    /* ADCCLK = PCLK2/4                   */

  *(vu32 *)0x40022000 = 0x01;           /* Flash 2 wait state                 */
}


void ADC_Init_ (void) {
  GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_InitTypeDef   ADC_InitStructure;
  DMA_InitTypeDef   DMA_InitStructure;

  /* Enable GPIOA clock                                                       */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* Enable DMA clock                                                         */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  

  /* Enable ADC1 clock                                                        */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure PA1 (ADC Channel1) as analog input                             */
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* DMA Channel1 Configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = 1;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  DMA_Cmd(DMA1_Channel1, ENABLE);       /* Enable DMA Channel1                */
  
  /* ADC1 Configuration (ADC1CLK = 18 MHz) -----------------------------------*/
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 Regular Channel1 Configuration                                      */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

  ADC_DMACmd(ADC1, ENABLE);             /* Enable ADC1's DMA interface        */
  ADC_Cmd   (ADC1, ENABLE);             /* Enable ADC1                        */
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);/* Start ADC1 Software Conversion     */ 
}
