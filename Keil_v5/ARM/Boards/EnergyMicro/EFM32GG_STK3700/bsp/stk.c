/**************************************************************************//**
 * @file
 * @brief EFM32GG_STK3700, Board Support Package support functions
 * @author Energy Micro AS
 * @version 1.0.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/

#include "efm32.h"
#include "em_cmu.h"
#include "em_ebi.h"
#include "em_gpio.h"
#include "stk.h"
#include "stkcomm.h"

/***************************************************************************//**
 * @addtogroup BSP
 * @{
 ******************************************************************************/

static USART_TypeDef *bcUSART;

/* Local prototypes */
void STK_InitGPIO( void );

void STK_Init( void )
{

   /* Setup config for STK3700 */
   bcUSART = UART0;

   /* Initialize STKCOMM */
   STKCOMM_bcInit( bcUSART, 115200, UART_ROUTE_LOCATION_LOC1 );

   /* Init GPIO */
   STK_InitGPIO();

   return;

}

USART_TypeDef *STK_ConfigGetUSART( void ){
   return bcUSART;
}

void STK_InitGPIO( void )
{

   /* RX pin */
   GPIO_PinModeSet( gpioPortE, 1, gpioModeInput, 0 );

   /* TX pin - To avoid false start, set pin high */
   GPIO_PinModeSet( gpioPortE, 0, gpioModePushPull, 1 );

   /* Enable switch U602A "VMCU switch" - to enable USART communication */
   /* See board schematics for details */
   GPIO_PinModeSet( gpioPortF, 7, gpioModePushPull, 1);

   return;

}


/***************************************************************************//**
 * @brief Initialize the EBI of the STK3700 board.
 ******************************************************************************/
void STK_EbiInit(void)
{

   /* ------------------------------------------ */
   /* NAND Flash, Bank0, Base Address 0x80000000 */
   /* Micron flash NAND256W3A                    */
   /* ------------------------------------------ */

   EBI_Init_TypeDef ebiConfig =
   {   ebiModeD8A8,      /* 8 bit address, 8 bit data */
       ebiActiveLow,     /* ARDY polarity */
       ebiActiveLow,     /* ALE polarity */
       ebiActiveLow,     /* WE polarity */
       ebiActiveLow,     /* RE polarity */
       ebiActiveLow,     /* CS polarity */
       ebiActiveLow,     /* BL polarity */
       false,            /* disble BL */
       true,             /* enable NOIDLE */
       false,            /* disable ARDY */
       true,             /* disable ARDY timeout */
       EBI_BANK0,        /* enable bank 0 */
       0,                /* no chip select */
       0,                /* addr setup cycles */
       0,                /* addr hold cycles */
       false,            /* disable half cycle ALE strobe */
       0,                /* read setup cycles */
       2,                /* read strobe cycles */
       1,                /* read hold cycles */
       false,            /* disable page mode */
       false,            /* disable prefetch */
       false,            /* disable half cycle REn strobe */
       0,                /* write setup cycles */
       2,                /* write strobe cycles */
       1,                /* write hold cycles */
       false,            /* enable the write buffer */
       false,            /* disable half cycle WEn strobe */
       ebiALowA24,       /* ALB - Low bound, address lines */
       ebiAHighA26,      /* APEN - High bound, address lines */
       ebiLocation1,     /* Use Location 1 */
       true,             /* enable EBI */
   };

   /* Enable clocks */
   CMU_ClockEnable(cmuClock_GPIO, true);
   CMU_ClockEnable(cmuClock_EBI, true);

   /* Enable GPIO's */
   /* ALE and CLE */
   GPIO_PinModeSet(gpioPortC, 1, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0);

   /* WP, CE and R/B */
   GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 0);  /* active low write-protect */
   GPIO_PinModeSet(gpioPortD, 14, gpioModePushPull, 1);  /* active low chip-enable */
   GPIO_PinModeSet(gpioPortD, 15, gpioModeInput, 0);     /* ready/busy */

   /* IO pins */
   GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0);
   GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0);

   /* WE and RE */
   GPIO_PinModeSet(gpioPortF, 8, gpioModePushPull, 1);
   GPIO_PinModeSet(gpioPortF, 9, gpioModePushPull, 1);

   /* NAND Power Enable */
   GPIO_PinModeSet(gpioPortB, 15, gpioModePushPull, 1);


   EBI_Init(&ebiConfig);
   EBI->NANDCTRL = (EBI_NANDCTRL_BANKSEL_BANK0 | EBI_NANDCTRL_EN);

   return;

}

/** @} (end group BSP) */
