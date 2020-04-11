/******************** (C) COPYRIGHT 2012 Triad Semiconductor, Inc.**************
* File Name          : system_TSX1001.c
* Version            : V1.00
* Date               : 23 April 2012
* Description        : CMSIS Cortex-M0 System Configuration File for Device TSX1001
*
*******************************************************************************/

#include <TSX1001.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL          (3686400UL)
#define __EXT_OSC       (44000000UL)
//#define __EXT_OSC       (20000000UL)

#define __PLL_FREQ      (12 * __XTAL)
#define __INPUT_CLK_PLL (__PLL_FREQ / 2)
#define __INPUT_CLK_EXT (__EXT_OSC  / 2)

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __INPUT_CLK_EXT; /*!< System Clock Frequency (Core Clock)*/



/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
uint32_t PllBypass;
uint32_t ClkDiv;
uint32_t SysClk;

  SysClk = 0UL;

  //  Determine if PLL is bypassed (bypassed if bit is set)
  PllBypass = (GPIO1->DATA[0].WORD) & 0x00000040UL;

  //  Read CLK_RATE field in MOCHA1_CTRL0 register to determine core clock divider
  switch(SYSCTRL->DATA[0] & 0x00000007UL) {
  case 0x00000000UL:
    ClkDiv = 16UL;
    break;
  case 0x00000001UL:
    ClkDiv = 8UL;
    break;
  case 0x00000002UL:
    ClkDiv = 4UL;
    break;
  case 0x00000003UL:
    ClkDiv = 2UL;
    break;
  default:
    ClkDiv = 1UL;
    break;
  }

  //  Calculate core clock frequency
  switch(PllBypass) {
  case 0x00000000UL:
    SysClk = __INPUT_CLK_PLL / ClkDiv;
    break;
  case 0x00000040UL:
    SysClk = __INPUT_CLK_EXT / ClkDiv;
    break;
  default:
    break;
  }

  SystemCoreClock = SysClk;
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
  uint32_t i;

//  Delay included here to allow the PLL to settle before trying to write to
//  PLL related registers.
//
  for (i = 0; i < 4000; i++);

//  Set the PLL lock ignore bit in the event that the PLL is used
//
  GPIO1->DATA[0].WORD |= 0x00000080UL | 0x00000000UL;                    /* LOCK_IGNORE */

//  Uncomment the following code to allow the EEPROM to be written as single
//  words.
//
//  SYSCTRL->DATA[0] |= (EE_WORD | EE_WREN);

//  Uncomment the following code to modify EEPROM wait states.
//  (0-3) is OK if HCLK is up to 8MHz
//  (1-3) is OK if HCLK is between 8MHz and 16MHz
//  (2-3) is OK if HCLK is greater than 16MHz
//
//  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFCFFFFUL) | (2UL << 16);  /* EEPROM Waitstate   */


//  Uncomment the following code to enable/disable EEPROM speculative reads.
//
//  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] | 0x00400000UL;                  /* EEPROM Speculative Read Enable  */
//  SYSCTRL->DATA[0] = SYSCTRL->DATA[0] & 0xFFBFFFFFUL;                  /* EEPROM Speculative Read Disable */

//  Uncomment the following code to control EEPROM speculative reads.
//
//  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0x3FFFFFFFUL) | (6UL << 28);  /* EEPROM Speculative Mask */

//  Uncomment the following code to control the input_clock to system_clock ratio.
//  0 -> 1:16
//  1 -> 1:8
//  2 -> 1:4
//  3 -> 1:2
//  4-7 -> 1:1
//
  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFFFFF8UL) | (4UL <<  0);  /* System clock rate */

//  Uncomment the following code to control the HCLK to PCLK (APB) ratio.
//  APB Rate 0 -> 1:1
//           1 -> 1:2
//           2 -> 1:3
//           3 -> 1:4
//
//  SYSCTRL->DATA[0] = (SYSCTRL->DATA[0] & 0xFFFFFCFFUL) | (0UL <<  8);  /* APB bus clock rate */

//  Uncomment the following code to modify IRQ latency.
//
//  SYSCTRL->DATA[1] = (SYSCTRL->DATA[1] & 0xFFFFFF00UL) | (7UL <<  0);  /* IRQ latency (range 0->255) */

//  SYSCTRL->DATA[1] = SYSCTRL->DATA[1] & 0xFFDFFFFFUL;                  /* Keep Peripherals indicator disable */

}

