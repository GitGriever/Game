/**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @version V1.3.0         modified by Keil (ExtSRAM initialisation)
  * @date    08-November-2013
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *          This file contains the system clock configuration for STM32F4xx devices.
  *             
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),
  *                      depending on the configuration made in the clock xls tool. 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  * 2. After each device reset the HSI (16 MHz) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32f4xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HSI still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.
  *
  * 4. The default value of HSE crystal is set to 25MHz, refer to "HSE_VALUE" define
  *    in "stm32f4xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  *
  * 5. This file configures the system clock as follows:
  *=============================================================================
  *=============================================================================
  *                    Supported STM32F40xxx/41xxx devices
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 168000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 168000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 4
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 25000000
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 25
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 336
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 7
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Main regulator output voltage          | Scale1 mode
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 5
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  *=============================================================================
  *                    Supported STM32F42xxx/43xxx devices
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 180000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 180000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 4
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 25000000
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 25
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 360
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 7
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Main regulator output voltage          | Scale1 mode
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 5
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  *=============================================================================
  *                         Supported STM32F401xx devices
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 84000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 84000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 1
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 25000000
  *-----------------------------------------------------------------------------
  *        PLL_M                                  | 25
  *-----------------------------------------------------------------------------
  *        PLL_N                                  | 336
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 4
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 7
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Main regulator output voltage          | Scale1 mode
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 2
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================      
  ****************************************************************************** 
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  
  
/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */

#include "stm32f4xx.h"

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM mounted
     on STM324xG_EVAL/STM324x7I_EVAL/STM324x9I_EVAL boards as data memory  */
     
#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx)
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F40_41xxx || STM32F427_437x || STM32F429_439xx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
/* #define DATA_IN_ExtSDRAM */
#endif /* STM32F427_437x || STM32F429_439xx */ 

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
/******************************************************************************/

/************************* PLL Parameters *************************************/
/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

#if defined (STM32F40_41xxx)
#define PLL_N      336
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
#endif /* STM32F40_41xxx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define PLL_N      360
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xx)
#define PLL_N      336
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      4
#endif /* STM32F401xx */

/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */

#if defined (STM32F40_41xxx)
  uint32_t SystemCoreClock = 168000000;
#endif /* STM32F40_41xxx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
  uint32_t SystemCoreClock = 180000000;
#endif /* STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xx)
  uint32_t SystemCoreClock = 84000000;
#endif /* STM32F401xx */

  __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM) || defined (PREMAIN_FSMC_SETUP)  /* Keil */
static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM || defined (PREMAIN_FSMC_SETUP) */             /* Keil */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM) || defined (PREMAIN_FSMC_SETUP) /* Keil */
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM || defined (PREMAIN_FSMC_SETUP) */            /* Keil */
         
  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings ----------------------------------*/
  SetSysClock();

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f4xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f4xx.h file (default value
  *              25 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *     
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock source, PLL Multiplier and Divider factors, 
  *         AHB/APBx prescalers and Flash settings
  * @Note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).   
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

#if defined (STM32F40_41xxx) || defined (STM32F427_437xx) || defined (STM32F429_439xx)      
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
#endif /* STM32F40_41xxx || STM32F427_437x || STM32F429_439xx */

#if defined (STM32F401xx)
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
#endif /* STM32F401xx */
   
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR->CR |= PWR_CR_ODEN;
    while((PWR->CSR & PWR_CSR_ODRDY) == 0)
    {
    }
    PWR->CR |= PWR_CR_ODSWEN;
    while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)
    {
    }      
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif /* STM32F427_437x || STM32F429_439xx  */

#if defined (STM32F40_41xxx)     
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
#endif /* STM32F40_41xxx  */

#if defined (STM32F401xx)
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;
#endif /* STM32F401xx */

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }

}

/**
  * @brief  Setup the external memory controller. Called in startup_stm32f4xx.s 
  *          before jump to __main
  * @param  None
  * @retval None
  */ 
#ifdef PREMAIN_FSMC_SETUP              /* Keil */
extern void FSMC_Setup (void);
void SystemInit_ExtMemCtl(void) {
  FSMC_Setup();
}
#endif /* PREMAIN_FSMC_SETUP */        /* Keil */

#ifdef DATA_IN_ExtSRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external SRAM mounted on STM324xG_EVAL/STM324x7I boards
  *         This SRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
/*-- GPIOs Configuration -----------------------------------------------------*/
/* Configure the data/address bus pins ---------------------------------------*/
  RCC->AHB1ENR  |= ((1UL <<  3) |       /* Enable GPIOD clock                 */
                    (1UL <<  4) |       /* Enable GPIOE clock                 */
                    (1UL <<  5) |       /* Enable GPIOF clock                 */
                    (1UL <<  6));       /* Enable GPIOG clock                 */

  /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE), PD.07 (NE3 (NOR/CS)) */ 
  /* PD.08(D13), PD.09(D14), PD.10(D15), PD.11(A16)           */
  /* PD.12(A17), PD.13(A18), PD.14(D0),  PD.15(D1)            */
  GPIOD->MODER    &= ~0xFFFFCF0F;       /* clear Bits                         */
  GPIOD->MODER    |=  0xAAAA8A0A;       /* Alternate Function mode            */   
  GPIOD->OSPEEDR  &= ~0xFFFFCF0F;       /* clear Bits                         */
  GPIOD->OSPEEDR  |=  0xAAAA8A0A;       /* 50 MHz Fast speed                  */   
  GPIOD->AFR[0]   &= ~0xF0FF00FF;       /* clear Bits                         */
  GPIOD->AFR[0]   |=  0xC0CC00CC;       /* Alternate Function mode AF12       */  
  GPIOD->AFR[1]   &= ~0xFFFFFFFF;       /* clear Bits                         */
  GPIOD->AFR[1]   |=  0xCCCCCCCC;       /* Alternate Function mode AF12       */  

  /* PE.00(BLR), PE.01(BHE), PE.03(A19), PE.04(A20), PE.05(A21) */
  /* PE.07(D4),  PE.08(D5),  PE.09(D6),  PE.10(D7),  PE.11(D8)  */
  /* PE.12(D9),  PE.13(D10), PE.14(D11), PE.15(D12)             */
  GPIOE->MODER    &= ~0xFFFFCFCF;       /* clear Bits                         */
  GPIOE->MODER    |=  0xAAAA8A8A;       /* Alternate Function mode            */
  GPIOE->OSPEEDR  &= ~0xFFFFCFCF;       /* clear Bits                         */
  GPIOE->OSPEEDR  |=  0xAAAA8A8A;       /* 50 MHz Fast speed                  */
  GPIOE->AFR[0]   &= ~0xF0FFF0FF;       /* clear Bits                         */
  GPIOE->AFR[0]   |=  0xC0CCC0CC;       /* Alternate Function mode AF12       */  
  GPIOE->AFR[1]   &= ~0xFFFFFFFF;       /* clear Bits                         */
  GPIOE->AFR[1]   |=  0xCCCCCCCC;       /* Alternate Function mode AF12       */  

  /* PF.00(A0),  PF.01(A1),  PF.02(A2),  PF.03(A3),  PF.04(A4)  */
  /* PF.05(A5),  PF.12(A6),  PF.13(A7),  PF.14(A8),  PF.15(A9)  */
  GPIOF->MODER   &= ~0xFF000FFF;        /* clear Bits                         */
  GPIOF->MODER   |=  0xAA000AAA;        /* Alternate Function mode            */
  GPIOF->OSPEEDR &= ~0xFF000FFF;        /* clear Bits                         */
  GPIOF->OSPEEDR |=  0xAA000AAA;        /* 50 MHz Fast speed                  */
  GPIOF->AFR[0]  &= ~0x00FFFFFF;        /* clear Bits                         */
  GPIOF->AFR[0]  |=  0x00CCCCCC;        /* Alternate Function mode AF12       */
  GPIOF->AFR[1]  &= ~0xFFFF0000;        /* clear Bits                         */
  GPIOF->AFR[1]  |=  0xCCCC0000;        /* Alternate Function mode AF12       */

  /* PG.00(A10),  PG.01(A11),  PG.02(A12),  PG.03(A13),  PG.04(A14)           */
  /* PG.05(A15),  PG.10(NE3 (SRAM/CS))                                        */
  GPIOG->MODER   &= ~0x00300FFF;        /* clear Bits                         */
  GPIOG->MODER   |=  0x00200AAA;        /* Alternate Function mode            */
  GPIOG->OSPEEDR &= ~0x00300FFF;        /* clear Bits                         */
  GPIOG->OSPEEDR |=  0x00200AAA;        /* 50 MHz Fast speed                  */
  GPIOG->AFR[0]  &= ~0x00FFFFFF;        /* clear Bits                         */
  GPIOG->AFR[0]  |=  0x00CCCCCC;        /* Alternate Function mode AF12       */  
  GPIOG->AFR[1]  &= ~0x00000F00;        /* clear Bits                         */
  GPIOG->AFR[1]  |=  0x00000C00;        /* Alternate Function mode AF12       */  

/*-- FSMC Configuration ------------------------------------------------------*/
  RCC->AHB3ENR  |= (1UL << 0);                           /* enable FSMC clock */

  FSMC_Bank1->BTCR[(3-1)*2 + 1] =       /* Bank3 NOR/SRAM timing register configuration */
                          (0 << 28) |   /* FSMC AccessMode A */
                          (0 << 24) |   /* Data Latency */
                          (0 << 20) |   /* CLK Division */
                          (0 << 16) |   /* Bus Turnaround Duration */
                          (9 <<  8) |   /* Data SetUp Time */
                          (0 <<  4) |   /* Address Hold Time */
                          (1 <<  0);    /* Address SetUp Time */

  FSMC_Bank1->BTCR[(3-1)*2 + 0] =       /* Bank3 NOR/SRAM timing register configuration */
                          (0 << 19) |   /* Write burst disabled */
                          (0 << 15) |   /* Async wait disabled */
                          (0 << 14) |   /* Extended mode disabled */
                          (0 << 13) |   /* NWAIT signal is disabled */ 
                          (1 << 12) |   /* Write operation enabled */
                          (0 << 11) |   /* NWAIT signal is active one data cycle before wait state */
                          (0 << 10) |   /* Wrapped burst mode disabled */
                          (0 <<  9) |   /* Wait signal polarity active low */
                          (0 <<  8) |   /* Burst access mode disabled */
                          (1 <<  4) |   /* Memory data  bus width is 16 bits */
                          (0 <<  2) |   /* Memory type is SRAM */
                          (0 <<  1) |   /* Address/Data Multiplexing disable */
                          (1 <<  0);    /* Memory Bank enable */

  FSMC_Bank1->BTCR[(1-1)*2 + 1] =       /* Bank1 NOR/SRAM timing register configuration */
                          (0 << 28) |   /* FSMC AccessMode A */
                          (0 << 24) |   /* Data Latency */
                          (0 << 20) |   /* CLK Division */
                          (0 << 16) |   /* Bus Turnaround Duration */
                          (9 <<  8) |   /* Data SetUp Time */
                          (0 <<  4) |   /* Address Hold Time */
                          (1 <<  0);    /* Address SetUp Time */

  FSMC_Bank1->BTCR[(1-1)*2 + 0] =       /* Bank1 NOR/SRAM timing register configuration */
                          (0 << 19) |   /* Write burst disabled */
                          (0 << 15) |   /* Async wait disabled */
                          (0 << 14) |   /* Extended mode disabled */
                          (0 << 13) |   /* NWAIT signal is disabled */ 
                          (1 << 12) |   /* Write operation enabled */
                          (0 << 11) |   /* NWAIT signal is active one data cycle before wait state */
                          (0 << 10) |   /* Wrapped burst mode disabled */
                          (0 <<  9) |   /* Wait signal polarity active low */
                          (0 <<  8) |   /* Burst access mode disabled */
                          (1 <<  4) |   /* Memory data  bus width is 16 bits */
                          (0 <<  2) |   /* Memory type is SRAM */
                          (0 <<  1) |   /* Address/Data Multiplexing disable */
                          (1 <<  0);    /* Memory Bank enable */
}
#endif /* DATA_IN_ExtSRAM */
  
#ifdef DATA_IN_ExtSDRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external SDRAM mounted on STM324x9I_EVAL board
  *         This SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  register uint32_t index;

  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface 
      clock */
  RCC->AHB1ENR |= 0x000001FC;
  
  /* Connect PCx pins to FMC Alternate function */
  GPIOC->AFR[0]  = 0x0000000c;
  GPIOC->AFR[1]  = 0x00007700;
  /* Configure PCx pins in Alternate function mode */  
  GPIOC->MODER   = 0x00a00002;
  /* Configure PCx pins speed to 50 MHz */  
  GPIOC->OSPEEDR = 0x00a00002;
  /* Configure PCx pins Output type to push-pull */  
  GPIOC->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PCx pins */ 
  GPIOC->PUPDR   = 0x00500000;
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x000000CC;
  GPIOD->AFR[1]  = 0xCC000CCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xA02A000A;
  /* Configure PDx pins speed to 50 MHz */  
  GPIOD->OSPEEDR = 0xA02A000A;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00000CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA800A;
  /* Configure PEx pins speed to 50 MHz */ 
  GPIOE->OSPEEDR = 0xAAAA800A;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;

  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0xcccccccc;
  GPIOF->AFR[1]  = 0xcccccccc;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0xcccccccc;
  GPIOG->AFR[1]  = 0xcccccccc;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xaaaaaaaa;
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0xaaaaaaaa;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x00000000;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00000000;
  
/*-- FMC Configuration ------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001;
  
  /* Configure and enable SDRAM bank1 */
  FMC_Bank5_6->SDCR[0] = 0x000039D0;
  FMC_Bank5_6->SDTR[0] = 0x01115351;      
  
  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011; 
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while((tmpreg != 0) & (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
  
  /* Delay */
  for (index = 0; index<1000; index++);
  
  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012;           
  timeout = 0xFFFF;
  while((tmpreg != 0) & (timeout-- > 0))
  {
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
  
  /* Auto refresh command */
  FMC_Bank5_6->SDCMR = 0x00000073;
  timeout = 0xFFFF;
  while((tmpreg != 0) & (timeout-- > 0))
  {
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
 
  /* MRD register program */
  FMC_Bank5_6->SDCMR = 0x00046014;
  timeout = 0xFFFF;
  while((tmpreg != 0) & (timeout-- > 0))
  {
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  } 
  
  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027C<<1));
  
  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0]; 
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);
  
/*
  Bank1_SDRAM is configured as follow:

  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = 2;      
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 6;  
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = 4;        
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = 6;         
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = 2;      
  FMC_SDRAMTimingInitStructure.FMC_RPDelay = 2;                
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay = 2;               

  FMC_SDRAMInitStructure.FMC_Bank = SDRAM_BANK;
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b;
  FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_11b;
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_16b;
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_3; 
  FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable;
  FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2;
  FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_disable;
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1;
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
*/
  
}
#endif /* DATA_IN_ExtSDRAM */


/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
