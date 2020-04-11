/**************************************************************************//**
 * @file     system_TMS470Mx.c
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File
 *           for the TI TMS470Mx Device Series
 * @version  V0.01
 * @date     16. August 2012
 *
 * @note
 * Copyright (C) 2012 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/
#include <stdint.h>
#include <TMS470Mx.H>




/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define SYS_CLK         (80000000UL)        /* System Clock Frequency (Core Clock)           */


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYS_CLK;     /*!< System Clock Frequency (Core Clock)  */

/**
 * Retrieve the system core clock
 *
 * @param  none
 * @return none
 *
 * @brief  retrieve system core clock from register settings.
 */
void SystemCoreClockUpdate (void) {
  SystemCoreClock = SYS_CLK;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit (void)
{
/* USER CODE BEGIN (3) */
/* USER CODE END */

    /** @b Initialize @b Flash @b Wrapper: */

    /** - Setup flash read mode, address wait states and data wait states */
    flashWREG->FRDCNTL =  0x01000000U 
                       | (2U << 8U) 
                       | (0U << 4U) 
                       |  1U;

    /** - Setup flash bank power modes */
    flashWREG->FBFALLBACK = 0x05050000
                          | (SYS_SLEEP << 14U) 
                          | (SYS_SLEEP << 12U) 
                          | (SYS_SLEEP << 10U) 
                          | (SYS_SLEEP << 8U) 
                          | (SYS_SLEEP << 6U) 
                          | (SYS_SLEEP << 4U) 
                          | (SYS_ACTIVE << 2U) 
                          |  SYS_ACTIVE;

    /** @b Initialize @b Pll: */

    /** - Setup pll control register 1:
    *     - Setup reset on oscillator slip 
    *     - Setup bypass on pll slip
    *     - Setup Pll output clock divider
    *     - Setup reset on oscillator fail
    *     - Setup reference clock divider 
    *     - Setup Pll multiplier          
    */
    systemREG->PLLCTL1  =  0x00000000U 
                        |  0x20000000U 
                        | (1U - 1U << 24U) 
                        |  0x00000000U 
                        | (4U - 1U << 16U) 
                        | (100U  - 1U << 8U);

    /** - Setup pll control register 1 
    *     - Enable/Disable frequency modulation
    *     - Setup spreading rate
    *     - Setup bandwidth adjustment
    *     - Setup internal Pll output divider
    *     - Setup spreading amount
    */
    systemREG->PLLCTL2  = 0x00000000U
                        | (255U << 22U)
                        | (0U << 12U)
                        | (5U - 1U << 9U)
                        |  61U;


    /** @b Initialize @b Clock @b Tree: */

    /** - Start clock source lock */
    systemREG->CSDIS =	0x00000000U 
						| 0x00000000U 
						| 0x00000008U 	
						| 0x00000004U 
						| 0x00000000U;

    /** - Wait for until clocks are locked */
    while ((systemREG->CSVSTAT & ((systemREG->CSDIS ^ 0xFF) & 0xFF)) != ((systemREG->CSDIS ^ 0xFF) & 0xFF));

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG->GHVSRC = (SYS_PLL << 24U) 
                       | (SYS_PLL << 16U) 
                       |  SYS_PLL;

    /** - Power-up all peripharals */
    pcrREG->PSPWRDWNCLR0 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR1 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR2 = 0xFFFFFFFFU;
    pcrREG->PSPWRDWNCLR3 = 0xFFFFFFFFU;

    /** - Setup synchronous peripheral clock dividers for VCLK1 and VCLK2 */
    systemREG->VCLKR  = 15U;
    systemREG->VCLK2R = (1U - 1U);
    systemREG->VCLKR  = (1U - 1U);

    /** - Setup RTICLK1 clock */
    systemREG->RCLKSRC  = (1U << 8U)  
                        |  SYS_VCLK;

    /** - Setup asynchronous peripheral clock sources for AVCLK1 and AVCLK2 */
    systemREG->VCLKASRC  = SYS_VCLK;

    /** - Enable Peripherals */
    systemREG->PENA = 1U;

    /** @note: HCLK >= VCLK2 >= VCLK_sys */

/* USER CODE BEGIN (4) */
/* USER CODE END */
}
