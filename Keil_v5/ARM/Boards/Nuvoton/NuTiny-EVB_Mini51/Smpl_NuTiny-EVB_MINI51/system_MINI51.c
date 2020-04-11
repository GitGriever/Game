/**
  ******************************************************************************
  * @file    system_MINI51.c
  * @author  NuMicro MCU Software Team
  * @version V2.00
  * @date    27 December 2012
  * @brief   CMSIS ARM Cortex-M0 Device Peripheral Access Layer Source File.
  ******************************************************************************
  * @note
  *
  * Copyright (c) 2012 Nuvoton Technology Corp. All rights reserved.
  */

#include <stdint.h>
#include "MINI51.h"



/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock  = __HSI;   /*!< System Clock Frequency (Core Clock) */

uint32_t CyclesPerUs      = (__HSI / 1000000); /* Cycles per micro second */


/**
  * @brief  This function is used to update the variable SystemCoreClock 
  *   and must be called whenever the core clock is changed.
  * @param  None.
  * @retval None.
  */

void SystemCoreClockUpdate (void)            
{
    uint32_t u32CoreFreq, u32ClkSrc;
 
    u32ClkSrc = SYSCLK->CLKSEL0 & 0x7;
        
    if (u32ClkSrc == 0)
	u32CoreFreq = __XTAL;		/* External crystal clock */

    else if (u32ClkSrc == 3)
        u32CoreFreq = __IRC10K;	/* Interanl 10K crystyl clock */

    else if (u32ClkSrc ==  7)
        u32CoreFreq = __IRC22M;	/* Internal 22M */

    else
	u32CoreFreq = __IRC22M;	 /* unknown value, use default Internal 22M */

 
    SystemCoreClock = (u32CoreFreq/((SYSCLK->CLKDIV & 0xF) + 1));
    CyclesPerUs = (SystemCoreClock + 500000) / 1000000;
}

/**
  * @brief  System initial function 
  * @param  None.
  * @retval None.
  */

void SystemInit (void)
{

}

#if USE_ASSERT

/**
 * @brief      Assert Error Message
 *
 * @param[in]  file  the source file name
 * @param[in]  line  line number
 *
 * @return     None
 *
 * @details    The function prints the source file name and line number where
 *             the ASSERT_PARAM() error occurs, and then stops in an infinite loop.
 */
void AssertError(uint8_t * file, uint32_t line)
{
  
    printf("[%s] line %d : wrong parameters.\r\n", file, line);

    /* Infinite loop */
    while(1) ;
}
#endif

