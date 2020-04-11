/**
  ******************************************************************************
  * @file    system_stm32f2xx.c
  * @author  MCD Application Team
  * @version V1.1.3         modified by Keil for MDK-ARM V5.0
  * @date    05-March-2012
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  *          This file contains the system clock configuration for STM32F2xx devices,
  *          and is generated by the clock configuration tool
  *          "STM32f2xx_Clock_Configuration_V1.0.0.xls"
  *             
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),
  *                      depending on the configuration made in the clock xls tool. 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f2xx.s" file.
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
  *    Then SystemInit() function is called, in "startup_stm32f2xx.s" file, to
  *    configure the system clock before to branch to main program.
  *
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and HSI still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.
  *
  * 4. The default value of HSE crystal is set to 25MHz, refer to "HSE_VALUE" define
  *    in "stm32f2xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  *
  * 5. This file configures the system clock as follows:
  *=============================================================================
  *=============================================================================
  *        Supported STM32F2xx device revision    | Rev B and Y
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 120000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 120000000
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
  *        PLL_N                                  | 240
  *-----------------------------------------------------------------------------
  *        PLL_P                                  | 2
  *-----------------------------------------------------------------------------
  *        PLL_Q                                  | 5
  *-----------------------------------------------------------------------------
  *        PLLI2S_N                               | NA
  *-----------------------------------------------------------------------------
  *        PLLI2S_R                               | NA
  *-----------------------------------------------------------------------------
  *        I2S input clock                        | NA
  *-----------------------------------------------------------------------------
  *        VDD(V)                                 | 3.3
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 3
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | ON
  *-----------------------------------------------------------------------------
  *        Instruction cache                      | ON
  *-----------------------------------------------------------------------------
  *        Data cache                             | ON
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB OTG FS,          | Enabled
  *        SDIO and RNG clock                     |
  *-----------------------------------------------------------------------------
  *=============================================================================
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/** @addtogroup stm32f2xx_system
  * @{
  */  
  
/** @addtogroup STM32F2xx_System_Private_Includes
  * @{
  */

#include "stm32f2xx.h"

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Defines
  * @{
  */

/*!< Uncomment the following line if you need to use external SRAM mounted
     on STM322xG_EVAL board as data memory  */
/* #define DATA_IN_ExtSRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */


/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25
#define PLL_N      240

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      5

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Variables
  * @{
  */

  uint32_t SystemCoreClock = 120000000;

  __I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);
#if defined(DATA_IN_ExtSRAM) || defined (PREMAIN_FSMC_SETUP)
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Functions
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

#if defined(DATA_IN_ExtSRAM) || defined(PREMAIN_FSMC_SETUP)
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM */
         
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
  *         (*) HSI_VALUE is a constant defined in stm32f2xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f2xx.h file (default value
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
    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS;

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
  * @brief  Setup the external memory controller. Called in startup_stm32f2xx.s
  *         before jump to __main
  * @param  None
  * @retval None
  */
#if PREMAIN_FSMC_SETUP
extern void FSMC_Setup (void);
void SystemInit_ExtMemCtl(void) {
  FSMC_Setup();
}
#elif DATA_IN_ExtSRAM
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f2xx.s before jump to main.
  *         This function configures the external SRAM mounted on STM322xG_EVAL board
  *         This SRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
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
