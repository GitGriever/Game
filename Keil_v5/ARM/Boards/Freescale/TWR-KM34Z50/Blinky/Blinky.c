/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MK21DN512xxx5 LED Flasher
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

#include <MKM34Z5.h>
#include "lcd.h"
#include "adc.h"
#include "gpio.h"

/* LEDs definition                                                            */
#define LED_G  PIN5        /* PTE5 */ 
#define LED_R  PIN1        /* PTF1 */ 
#define LED_Y  PIN1        /* PTD1 */ 
#define LED_O  PIN1        /* PTC1 */ 
#define LED_NUM     4                   /* Number of user LEDs                */

static const char digits[]=
{
  '0','1','2','3','4','5','6','7','8','9','A','b','c','d','E','F','8'
};

const uint32_t led_mask[] = {1UL << 5, 1UL << 1, 1UL << 1, 1UL << 1};
const uint32_t gpio_id[] = {GPIOE_BASE, GPIOF_BASE, GPIOD_BASE, GPIOC_BASE};

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
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

	/*Enable Port E/F/D/C Clock */
  SIM->SCGC5 |= (1 << SIM_SCGC5_PORTE_SHIFT) | (1 << SIM_SCGC5_PORTF_SHIFT) |\
	              (1 << SIM_SCGC5_PORTD_SHIFT) | (1 << SIM_SCGC5_PORTC_SHIFT);

  /*Port Initialization */
  PORT_Init (PORTE, PORT_MODULE_LED_MODE, LED_G);
  PORT_Init (PORTF, PORT_MODULE_LED_MODE, LED_R);
  PORT_Init (PORTD, PORT_MODULE_LED_MODE, LED_O);
  PORT_Init (PORTC, PORT_MODULE_LED_MODE, LED_Y);
  /*GPIO Initialization */
  GPIO_Init (GPIOE, GPIO_OUT_MODE, LED_G);
  GPIO_Init (GPIOF, GPIO_OUT_MODE, LED_R);
  GPIO_Init (GPIOD, GPIO_OUT_MODE, LED_O);
  GPIO_Init (GPIOC, GPIO_OUT_MODE, LED_Y);
  /*All LEDs Off */
  GPIO_Set  (GPIOC, LED_Y);
  GPIO_Set  (GPIOE, LED_G);
  GPIO_Set  (GPIOF, LED_R);
  GPIO_Set  (GPIOD, LED_O);
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {
  ((GPIO_Type *)gpio_id[led])->PDOR &= ~led_mask[led];
}


/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {
((GPIO_Type *)gpio_id[led])->PDOR |= led_mask[led];
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

int main (void) {
  int num     = -1; 
  int dir     =  1;
  uint32_t tmp32;
  uint16_t tmp16;
  uint8_t  tmpNum;

  /* Disable NMI */
  SIM->CTRL_REG |= (1 << SIM_CTRL_REG_NMIDIS_SHIFT);
  
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  
  LED_Config();                             

  /* Enable ADC Clock */
  SIM->SCGC6 |= (1 << SIM_SCGC6_ADC_SHIFT);
  
  ADC_Init (ADC_MODULE_16B_SWTRG_XREF_CONFIG, HWAVG_32, ADC_CH_SE_POLL_CONFIG(AD_OFF),
            ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG, 
            0, NULL);

/* Enable SLCD Clock */
  SIM->SCGC5 |= (1 << SIM_SCGC5_SLCD_SHIFT);

  LCD_Init         (LCD_CHARGEPUMP_EN_SOURCE_ALT_BACKPLANES_8,                 \
                    LCD_FRONT_PLANE_PINS_TWR, LCD_BACK_PLANE_PINS_TWR,         \
                    LCD_ASSIGN_BACK_PLANES_TWR);
  Delay(1000);
  
  _LCD_CHAR_V_S3_ON;
  _LCD_CHAR_V_S6_ON;
  _LCD_CHAR_V_S7_ON;
  _LCD_CHAR_V_S9_ON;
  _LCD_CHAR_V_S11_ON;
  _LCD_CHAR_V_S12_ON;
  _LCD_CHAR_V_S13_ON;
  _LCD_DOT_P3_ON;

  while(1) {
    ADC_Start(CHA,AD10);
    while (!ADC_Ready (CHA));
    tmp16 = ADC_Read(CHA);
    tmp32 = 50050 * tmp16;
  
    tmpNum = (uint8) (tmp32/1000000000);
    tmp32 = tmp32 % 1000000000;
    LCD_Delete_Char(3);
    LCD_Write_Char ( digits[tmpNum] );

    tmpNum = (uint8) (tmp32/100000000);
    tmp32 = tmp32 % 100000000;
    LCD_Delete_Char(2);
    LCD_Write_Char ( digits[tmpNum] );

    tmpNum = (uint8) (tmp32/10000000);
    tmp32 = tmp32 % 10000000;
    LCD_Delete_Char(1);
    LCD_Write_Char ( digits[tmpNum] );
  
    tmpNum = (uint8) (tmp32/1000000);
    LCD_Delete_Char(0);
    LCD_Write_Char ( digits[tmpNum] );

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

