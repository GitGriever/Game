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

GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
ADC_InitTypeDef   ADC_InitStructure;
DMA_InitTypeDef   DMA_InitStructure;

void CLK_Init (void) {
  RCC_HCLKConfig   (RCC_SYSCLK_Div1);   /* HCLK   = SYSCLK  */
  RCC_PCLK2Config  (RCC_HCLK_Div1);     /* PCLK2  = HCLK    */
  RCC_PCLK1Config  (RCC_HCLK_Div2);     /* PCLK1  = HCLK/2  */
  RCC_ADCCLKConfig (RCC_PCLK2_Div6);    /* ADCCLK = PCLK2/6 */

  FLASH_SetLatency(FLASH_Latency_2);    /* Flash 2 wait state */
  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}


void ADC_Init_ (void) {

  /* Enable GPIOA clock                                                       */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Enable DMA clock                                                         */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 clock                                                        */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure PC4 (ADC Channel 14) as analog input */
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

#ifndef __USE_IRQ
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
#endif //__USE_IRQ
     
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);

#ifndef __USE_IRQ
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
#else
  /* Configure and enable ADC interrupt */
  NVIC_EnableIRQ(ADC1_2_IRQn);

  /* Enable EOC interupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
#endif //__USE_IRQ

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
