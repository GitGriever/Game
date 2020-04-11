/**
  ******************************************************************************
  * @file    main.c
  * @author  Microcontroller Division
  * @version V1.0.0
  * @date    Apri-2011
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */
 
/* Includes ------------------------------------------------------------------*/

#include "misc.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_lcd.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_syscfg.h"
#include "stm32l1xx_dbgmcu.h"

#include "stm32_tsl_api.h"
#include "stm32l15x_tsl_ct_acquisition.h"

#include "discover_board.h"
#include "icc_measure.h"
#include "discover_functions.h"
#include "stm32l_discovery_lcd.h"
#include "stm32_tsl_timebase.h"

static volatile uint32_t TimingDelay;
extern unsigned char Bias_Current;
/* LCD bar graph: used for displaying active function */
extern uint8_t t_bar[2];
/* Auto_test activation flag: set by interrupt handler if user button is pressed for a few seconds */
extern bool Auto_test;
extern bool Idd_WakeUP; 
/* Set by interrupt handler to indicate that user button is pressed */ 
extern volatile bool KeyPressed;
extern bool UserButton ;

/* Machine status used by main() wich indicats the active function, set by user button in interrupt handler */
uint8_t state_machine;

uint16_t Int_CurrentSTBY;


/*******************************************************************************/
/**
  * @brief main entry point.
  * @par Parameters None
  * @retval void None
  * @par Required preconditions: None
  */
                                  
int main(void)
{ 
  bool StanbyWakeUp ;
  float Current_STBY;
  
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */ 
  

  Int_CurrentSTBY = Current_Measurement();
  
  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {
    /* System resumed from STANDBY mode */
    /* Clear StandBy flag */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    PWR_ClearFlag(PWR_FLAG_SB); 

    StanbyWakeUp = TRUE;
    
  } else
  {
    StanbyWakeUp = FALSE;    
  } 

  PWR_PVDCmd(DISABLE);

  RCC_Configuration();
  
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);

  /* Wait Until the Voltage Regulator is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) ;
     
  /* Init I/O ports */
  Init_GPIOs ();

  /* Initializes ADC */
  ADC_Icc_Init();
  
  enableInterrupts();	

  /* Warning ! in TSL Init the sysTick interrupt is setted to:
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 2000 ---> 500 �s*/
  
  /* Init Touch Sensing configuration */  
  TSL_Init();
  
  sMCKeyInfo[0].Setting.b.IMPLEMENTED = 1;
  sMCKeyInfo[0].Setting.b.ENABLED = 1;
  sMCKeyInfo[0].DxSGroup = 0x00; 
 
  /* Initializes the LCD glass */
  LCD_GLASS_Init();
        

  /* Reset Keypressed used in interupt and Scrollsentence */
  KeyPressed = FALSE;

  /* user button actif */  
  UserButton = TRUE;
  	 
  /* Check the User button at Power ON if pressed --> Bias current measurement and store in E�Prom */	
  if ((GPIOA->IDR & USER_GPIO_PIN) != 0x0)
  {
    Bias_measurement();
  }	

  
  /* Welcome display */
  if ( !StanbyWakeUp )
  {
    
    /* No auto test at startup */
    AUTOTEST(FALSE) ;
  
    LCD_GLASS_ScrollSentence("      ** STM32L-DISCOVERY **",1,SCROLL_SPEED);

    if (!KeyPressed)
    {
      LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM,LCD_BlinkFrequency_Div512);
      LCD_GLASS_DisplayString("JP1 ON");
      TEMPO;
      TEMPO;
      TEMPO;
      TEMPO;
      LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div32);	
    }
  }  else  {
    
    if (Auto_test)
    {
      /* Wake UP: Return of RESET by Auto test */
      auto_test_part2();
      
    } else {
      /* Wake UP: Return of RESET by Current STAND BY measurement */
            LCD_GLASS_ScrollSentence("     STANDBY WAKEUP",1,SCROLL_SPEED);
  
      /* Substract bias curent*/
      if ( Int_CurrentSTBY > Bias_Current )
        Int_CurrentSTBY -= Bias_Current;
  
      Current_STBY = Int_CurrentSTBY * Vdd_appli()/ADC_CONV;  
    
      Current_STBY *= 20L;
      display_MuAmp((uint32_t)Current_STBY);
      
      /* To wait user button */
      while(!KeyPressed);
    }
  }
   
  KeyPressed = FALSE; 

  /* All the bars on LCD display are OFF, then the first VDD function is displayed */
  BAR0_OFF;
  BAR1_OFF;
  BAR2_OFF;
  BAR3_OFF;	
  
  /* Switch off the leds at start */
  GPIO_HIGH(LD_PORT,LD_GREEN);	
  GPIO_LOW(LD_PORT,LD_BLUE);	
    
  /* At power on VDD diplays */
  state_machine = STATE_VREF ;
 
  while (1)
  {
    
/* if auto test is set --> run it once */
    if (Auto_test)
      auto_test();

    switch (state_machine)
    {
        /* Must be remove Displays VDD */
        case STATE_VREF:
          GPIO_TOGGLE(LD_PORT,LD_BLUE);
          GPIO_TOGGLE(LD_PORT,LD_GREEN);
          Vref_measure();
          TEMPO ;
        break;
        
        case STATE_SLIDER_VALUE:
          TSL_Action();
          Slider_value();   
          Delay(5);
        break;
        
        case STATE_SLIDER_BUTTON: 
          TSL_Action();
          Button_value();         
          Delay(5);
          break;
          
        case STATE_ICC_RUN:
          /* Init Touch Sensing configuration */ 
          LCD_GLASS_DisplayString(" RUN  ");
          TEMPO;
          Icc_RUN();
          TEMPO;
          TEMPO;
          LCD_GLASS_DisplayString(" SLEEP ");
          TEMPO;
          Icc_SLEEP(); 
          TEMPO ;
          TEMPO ;
        break;
 
        case STATE_ICC_LP_RUN:
          LCD_GLASS_DisplayString("LP RUN");
          TEMPO;
          Icc_LPRUN();
          TEMPO;
          TEMPO;
          LCD_GLASS_DisplayString("LP SLP");
          TEMPO;
          Icc_LPSLEEP();
          TEMPO ;
          TEMPO ;          
        break;
         
        case STATE_ICC_STOP:
          LCD_GLASS_DisplayString(" STP  ");
          TEMPO;
          Icc_STOP();
          TEMPO ;
          TEMPO ;          
          LCD_GLASS_DisplayString("STPNo ");
          TEMPO;
          Icc_Stop_NoRTC();
          TEMPO ;
          TEMPO ;       
          break;
               
        case STATE_ICC_STBY:
          LCD_GLASS_DisplayString("STBY  ");
          TEMPO;
        /* To init the mode and measurement*/
        ADC_Icc_Test(MCU_STBY);
        
        /* No Return -- Reset */       
        break;
           
        /* for safe: normaly never reaches */ 			
        default:
          LCD_GLASS_Clear();
          LCD_GLASS_DisplayString("ERROR");
        break;
      }
     
      KeyPressed = FALSE;
   
    }
}		

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{  
  
  /* Enable HSI Clock */
  RCC_HSICmd(ENABLE);
  
  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}

  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  
  RCC_MSIRangeConfig(RCC_MSIRange_6);
  
  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);     

  /* Enable  comparator clock LCD and PWR mngt */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP | RCC_APB1Periph_LCD | RCC_APB1Periph_PWR,ENABLE);
    
  /* Enable ADC clock & SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG , ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /* LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  RCC_RTCCLKCmd(ENABLE);
   
  /* LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  RCC_HSEConfig(RCC_HSE_OFF);
  
  if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
  {
    while(1);
  }

}

/**
  * @brief  To initialize the I/O ports
  * @caller main
  * @param None
  * @retval None
  */
void  Init_GPIOs (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USER button and WakeUP button init: GPIO set in input interrupt active mode */
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure User Button pin as input */
  GPIO_InitStructure.GPIO_Pin = USER_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);

  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

  /* Configure User Button and IDD_WakeUP EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;  // PA0 for User button AND IDD_WakeUP
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set User Button and IDD_WakeUP EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 

/* Configure the GPIO_LED pins  LD3 & LD4*/
  GPIO_InitStructure.GPIO_Pin = LD_GREEN|LD_BLUE;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LD_PORT, &GPIO_InitStructure);
  GPIO_LOW(LD_PORT,LD_GREEN);	
  GPIO_LOW(LD_PORT,LD_BLUE);
    
/* Counter enable: GPIO set in output for enable the counter */
  GPIO_InitStructure.GPIO_Pin = CTN_CNTEN_GPIO_PIN;
  GPIO_Init( CTN_GPIO_PORT, &GPIO_InitStructure);
  
/* To prepare to start counter */
  GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
      
/* Configure Output for LCD */
/* Port A */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_LCD) ;  
  
/* Configure Output for LCD */
/* Port B */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 \
                                 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_LCD) ;  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_LCD) ;   
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15,GPIO_AF_LCD) ;   
  
/* Configure Output for LCD */
/* Port C*/  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 \
                                 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOC, &GPIO_InitStructure);  
  

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1,GPIO_AF_LCD) ; 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_LCD) ; 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11,GPIO_AF_LCD) ;  
  
/* ADC input */
  GPIO_InitStructure.GPIO_Pin = IDD_MEASURE  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init( IDD_MEASURE_PORT, &GPIO_InitStructure);

}  



/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
  
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{

  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }

}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
