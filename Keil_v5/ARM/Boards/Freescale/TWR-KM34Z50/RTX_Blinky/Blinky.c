/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2006-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/
#include <RTL.h>
#include <MKM34Z5.h>
#include "lcd.h"
#include "adc.h"
#include "gpio.h"


OS_TID t_phaseA;                        /* assigned task id of task: phase_a */
OS_TID t_phaseB;                        /* assigned task id of task: phase_b */
OS_TID t_phaseC;                        /* assigned task id of task: phase_c */
OS_TID t_phaseD;                        /* assigned task id of task: phase_d */
OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */


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

#define LED_A      1
#define LED_B      2
#define LED_C      3
#define LED_D      1
#define LED_CLK    0


/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_init(void) {

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

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {
  ((GPIO_Type *)gpio_id[num])->PDOR &= ~led_mask[num];
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {
  ((GPIO_Type *)gpio_id[num])->PDOR |= led_mask[num];
}

/*----------------------------------------------------------------------------
 *        Function 'signal_func' called from multiple tasks
 *---------------------------------------------------------------------------*/
void signal_func (OS_TID task)  {
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
  os_evt_set (0x0001, task);             /* send event to task 'task'        */
  os_dly_wait (50);                      /* delay 50 clock ticks             */
}

/*----------------------------------------------------------------------------
 *        Task 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
__task void phaseA (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_On (LED_A);
    signal_func (t_phaseB);              /* call common signal function      */
    LED_Off(LED_A);
  }
}

/*----------------------------------------------------------------------------
 *        Task 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
__task void phaseB (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_On (LED_B);
    signal_func (t_phaseC);              /* call common signal function      */
    LED_Off(LED_B);
  }
}

/*----------------------------------------------------------------------------
 *        Task 3 'phaseC': Phase C output
 *---------------------------------------------------------------------------*/
__task void phaseC (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_On (LED_C);
    signal_func (t_phaseD);              /* call common signal function      */
    LED_Off(LED_C);
  }
}

/*----------------------------------------------------------------------------
 *        Task 4 'phaseD': Phase D output
 *---------------------------------------------------------------------------*/
__task void phaseD (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
    LED_On (LED_D);
    signal_func (t_phaseA);              /* call common signal function      */
    LED_Off(LED_D);
  }
}

/*----------------------------------------------------------------------------
 *        Task 5 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  for (;;) {
    os_evt_wait_and (0x0100, 0xffff);    /* wait for an event flag 0x0100    */
    LED_On (LED_CLK);
    os_dly_wait (8);                     /* delay 8 clock ticks              */
    LED_Off(LED_CLK);
  }
}

/*----------------------------------------------------------------------------
  Task 6 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {
  uint32_t tmp32;
  uint16_t tmp16;
  uint8_t  tmpNum;

  _LCD_CHAR_V_S3_ON;
  _LCD_CHAR_V_S6_ON;
  _LCD_CHAR_V_S7_ON;
  _LCD_CHAR_V_S9_ON;
  _LCD_CHAR_V_S11_ON;
  _LCD_CHAR_V_S12_ON;
  _LCD_CHAR_V_S13_ON;
  _LCD_DOT_P3_ON;

  for (;;) {
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

    os_dly_wait (200);
  }
}

/*----------------------------------------------------------------------------
 *        Task 6 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {
  t_phaseA = os_tsk_create (phaseA, 0);  /* start task phaseA                */
  t_phaseB = os_tsk_create (phaseB, 0);  /* start task phaseB                */
  t_phaseC = os_tsk_create (phaseC, 0);  /* start task phaseC                */
  t_phaseD = os_tsk_create (phaseD, 0);  /* start task phaseD                */
  t_clock  = os_tsk_create (clock, 0);   /* start task clock                 */
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
  os_evt_set (0x0001, t_phaseA);         /* send signal event to task phaseA */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  /* Disable NMI */
  SIM->CTRL_REG |= (1 << SIM_CTRL_REG_NMIDIS_SHIFT);

  LED_init ();                              /* Initialize the LEDs           */
 
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
  
 
  os_sys_init(init);                        /* Initialize RTX and start init */
}
