/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: SAMSUNG S3FM02G LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "S3FM02G.h"

#define LED_NUM     4                    /* Number of user LEDs                */
const uint32_t led_mask[] = {1UL << 5, 1UL << 7, 1UL << 9, 1UL << 15};
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                            /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*------------------------------------------------------------------------------
  configer LED pins
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Config(void) {
  S3FM_CM->PCSR0   = 0x40000000;			/* Enable IOCONF Clock */
  S3FM_GPIO0->CEDR = 0x00000001;            /* Enable GPIO0  Clock */ 

  S3FM_GPIO0->CODR = (led_mask[0] |        
                      led_mask[1] |
                      led_mask[2] |
                      led_mask[3] );        /* switch LEDs off  */

  S3FM_GPIO0->OER  = (led_mask[0] |        
                      led_mask[1] |
                      led_mask[2] |
                      led_mask[3] );        /* enable Output */
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {
  S3FM_GPIO0->CODR  =led_mask[led];
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {
  S3FM_GPIO0->SODR  =led_mask[led];
}
#if 1
S3FM_ADC0_TypeDef   * Test_S3FM_ADC0_TypeDef    =    S3FM_ADC0;       
S3FM_ADC1_TypeDef   * Test_S3FM_ADC1_TypeDef    =    S3FM_ADC1;       
S3FM_CAN_TypeDef    * Test_CAN0_TypeDef         =    S3FM_CAN0;       
S3FM_CAN_TypeDef    * Test_CAN1_TypeDef         =    S3FM_CAN1;       
S3FM_CM_TypeDef     * Test_S3FM_CM_TypeDef      =    S3FM_CM;         
S3FM_DMAC_TypeDef   * Test_S3FM_DMAC_TypeDef    =    S3FM_DMAC;       
S3FM_DFC_TypeDef    * Test_S3FM_DFC_TypeDef     =    S3FM_DFC;        
S3FM_PFC_TypeDef    * Test_S3FM_PFC_TypeDef     =    S3FM_PFC;        
S3FM_ENC_TypeDef    * Test_S3FM_ENC0_TypeDef    =    S3FM_ENC0;       
S3FM_ENC_TypeDef    * Test_S3FM_ENC1_TypeDef    =    S3FM_ENC1;       
S3FM_FRT_TypeDef    * Test_S3FM_FRT_TypeDef     =    S3FM_FRT0;       
S3FM_GPIO_TypeDef   * Test_S3FM_GPIO0_TypeDef   =    S3FM_GPIO0;      
S3FM_GPIO_TypeDef   * Test_S3FM_GPIO1_TypeDef   =    S3FM_GPIO1;      
S3FM_GPIO_TypeDef   * Test_S3FM_GPIO2_TypeDef   =    S3FM_GPIO2;      
S3FM_GPIO_TypeDef   * Test_S3FM_GPIO3_TypeDef   =    S3FM_GPIO3;      
S3FM_I2C_TypeDef    * Test_S3FM_I2C0_TypeDef    =    S3FM_I2C0;       
S3FM_I2C_TypeDef    * Test_S3FM_I2C1_TypeDef    =    S3FM_I2C1;       
S3FM_IMC_TypeDef    * Test_S3FM_IMC0_TypeDef    =    S3FM_IMC0;       
S3FM_IMC_TypeDef    * Test_S3FM_IMC1_TypeDef    =    S3FM_IMC1;       
S3FM_IOCONF_TypeDef * Test_S3FM_IOCONF_TypeDe   =    S3FM_IOCONF;     
S3FM_LCDC_TypeDef   * Test_S3FM_LCDC_TypeDef    =    S3FM_LCDC;       
S3FM_OPAMP_TypeDef  * Test_S3FM_OPAMP_TypeDef   =    S3FM_OPAMP;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM0_TypeDef    =    S3FM_PWM0;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM1_TypeDef    =    S3FM_PWM1;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM2_TypeDef    =    S3FM_PWM2;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM3_TypeDef    =    S3FM_PWM3;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM4_TypeDef    =    S3FM_PWM4;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM5_TypeDef    =    S3FM_PWM5;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM6_TypeDef    =    S3FM_PWM6;      
S3FM_PWM_TypeDef    * Test_S3FM_PWM7_TypeDef    =    S3FM_PWM7;      
S3FM_SSP_TypeDef    * Test_S3FM_SSP0_TypeDef    =    S3FM_SSP0;      
S3FM_SSP_TypeDef    * Test_S3FM_SSP1_TypeDef    =    S3FM_SSP1;      
S3FM_TC16_TypeDef   * Test_S3FM_TC160_TypeDef   =    S3FM_TC0;        
S3FM_TC16_TypeDef   * Test_S3FM_TC161_TypeDef   =    S3FM_TC1;        
S3FM_TC16_TypeDef   * Test_S3FM_TC162_TypeDef   =    S3FM_TC2;        
S3FM_TC16_TypeDef   * Test_S3FM_TC163_TypeDef   =    S3FM_TC3;        
S3FM_TC16_TypeDef   * Test_S3FM_TC164_TypeDef   =    S3FM_TC4;        
S3FM_TC16_TypeDef   * Test_S3FM_TC165_TypeDef   =    S3FM_TC5;        
S3FM_TC16_TypeDef   * Test_S3FM_TC166_TypeDef   =    S3FM_TC6;        
S3FM_TC16_TypeDef   * Test_S3FM_TC167_TypeDef   =    S3FM_TC7;        
S3FM_STT_TypeDef    * Test_S3FM_STT_TypeDef     =    S3FM_STT;        
S3FM_USART_TypeDef  * Test_S3FM_USART0_TypeDef  =    S3FM_USART0;     
S3FM_USART_TypeDef  * Test_S3FM_USART1_TypeDef  =    S3FM_USART1;     
S3FM_USART_TypeDef  * Test_S3FM_USART2_TypeDef  =    S3FM_USART2;     
S3FM_USART_TypeDef  * Test_S3FM_USART3_TypeDef  =    S3FM_USART3;     
S3FM_WDT_TypeDef    * Test_S3FM_WDT_TypeDef     =    S3FM_WDT;        
#endif
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int num     = -1; 
  int dir     =  1;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  
  LED_Config();                             
 
  while(1) {
    /* Calculate 'num': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...             */
    num += dir;
    if (num == LED_NUM) { dir = -1; num =  LED_NUM-1; } 
    else if   (num < 0) { dir =  1; num =  0;         }

    LED_On (num);
    Delay(250);
    LED_Off(num);
    Delay(250);
  }
  
}

