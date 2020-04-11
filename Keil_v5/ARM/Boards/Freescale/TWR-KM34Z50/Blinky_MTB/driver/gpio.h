/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      gpio.h
 * @version   1.0.3.0
 * @date      May-24-2012
 * @brief     General-Purpose Input/Output (GPIO) driver header file.
 *
 * Revision History:
 *  1.	01/01/2010   R55013   Initially written
 *  2.	01/08/2012   R55013   Ported to Kinetis M  
 *  3.	16/04/2013   By ARM   Modifed for RVCT & CMSIS adaptation
 ******************************************************************************/
/******************************************************************************
 *//*! @addtogroup gpio_example
 * @{
 * @par       Source code:
 * @include   gpio_test\gpio_test.c
 * @par       Appconfig.h:
 * @include   gpio_test\appconfig.h  
 ******************************************************************************/
/*! @} End of gpio_example                                                    */
#ifndef __GPIO_H
#define __GPIO_H 

#include "MKM34Z5.h"

/* PORT - Register accessors */
#define PORT_PCR_REG(base,index)                 ((base)->PCR[index])
#define PORT_GPCLR_REG(base)                     ((base)->GPCLR)
#define PORT_GPCHR_REG(base)                     ((base)->GPCHR)
#define PORT_ISFR_REG(base)                      ((base)->ISFR)
#define PORT_DFER_REG(base)                      ((base)->DFER)
#define PORT_DFCR_REG(base)                      ((base)->DFCR)
#define PORT_DFWR_REG(base)                      ((base)->DFWR)

/* PORT - Peripheral instance base addresses */
/** Peripheral PORTA base pointer */
#define PORTA_BASE_PTR                           (PORTA)
/** Peripheral PORTB base pointer */
#define PORTB_BASE_PTR                           (PORTB)
/** Peripheral PORTC base pointer */
#define PORTC_BASE_PTR                           (PORTC)
/** Peripheral PORTD base pointer */
#define PORTD_BASE_PTR                           (PORTD)
/** Peripheral PORTE base pointer */
#define PORTE_BASE_PTR                           (PORTE)
/** Peripheral PORTF base pointer */
#define PORTF_BASE_PTR                           (PORTF)
/** Peripheral PORTG base pointer */
#define PORTG_BASE_PTR                           (PORTG)
/** Peripheral PORTH base pointer */
#define PORTH_BASE_PTR                           (PORTH)
/** Peripheral PORTI base pointer */
#define PORTI_BASE_PTR                           (PORTI)
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASE_PTRS                           { PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR, PORTF_BASE_PTR, PORTG_BASE_PTR, PORTH_BASE_PTR, PORTI_BASE_PTR }

/* PORT - Register instance definitions */
/* PORTA */
#define PORTA_PCR0                               PORT_PCR_REG(PORTA_BASE_PTR,0)
#define PORTA_PCR1                               PORT_PCR_REG(PORTA_BASE_PTR,1)
#define PORTA_PCR2                               PORT_PCR_REG(PORTA_BASE_PTR,2)
#define PORTA_PCR3                               PORT_PCR_REG(PORTA_BASE_PTR,3)
#define PORTA_PCR4                               PORT_PCR_REG(PORTA_BASE_PTR,4)
#define PORTA_PCR5                               PORT_PCR_REG(PORTA_BASE_PTR,5)
#define PORTA_PCR6                               PORT_PCR_REG(PORTA_BASE_PTR,6)
#define PORTA_PCR7                               PORT_PCR_REG(PORTA_BASE_PTR,7)
#define PORTA_GPCLR                              PORT_GPCLR_REG(PORTA_BASE_PTR)
#define PORTA_GPCHR                              PORT_GPCHR_REG(PORTA_BASE_PTR)
#define PORTA_ISFR                               PORT_ISFR_REG(PORTA_BASE_PTR)
#define PORTA_DFER                               PORT_DFER_REG(PORTA_BASE_PTR)
#define PORTA_DFCR                               PORT_DFCR_REG(PORTA_BASE_PTR)
#define PORTA_DFWR                               PORT_DFWR_REG(PORTA_BASE_PTR)
/* PORTB */
#define PORTB_PCR0                               PORT_PCR_REG(PORTB_BASE_PTR,0)
#define PORTB_PCR1                               PORT_PCR_REG(PORTB_BASE_PTR,1)
#define PORTB_PCR2                               PORT_PCR_REG(PORTB_BASE_PTR,2)
#define PORTB_PCR3                               PORT_PCR_REG(PORTB_BASE_PTR,3)
#define PORTB_PCR4                               PORT_PCR_REG(PORTB_BASE_PTR,4)
#define PORTB_PCR5                               PORT_PCR_REG(PORTB_BASE_PTR,5)
#define PORTB_PCR6                               PORT_PCR_REG(PORTB_BASE_PTR,6)
#define PORTB_PCR7                               PORT_PCR_REG(PORTB_BASE_PTR,7)
#define PORTB_GPCLR                              PORT_GPCLR_REG(PORTB_BASE_PTR)
#define PORTB_GPCHR                              PORT_GPCHR_REG(PORTB_BASE_PTR)
#define PORTB_ISFR                               PORT_ISFR_REG(PORTB_BASE_PTR)
#define PORTB_DFER                               PORT_DFER_REG(PORTB_BASE_PTR)
#define PORTB_DFCR                               PORT_DFCR_REG(PORTB_BASE_PTR)
#define PORTB_DFWR                               PORT_DFWR_REG(PORTB_BASE_PTR)
/* PORTC */
#define PORTC_PCR0                               PORT_PCR_REG(PORTC_BASE_PTR,0)
#define PORTC_PCR1                               PORT_PCR_REG(PORTC_BASE_PTR,1)
#define PORTC_PCR2                               PORT_PCR_REG(PORTC_BASE_PTR,2)
#define PORTC_PCR3                               PORT_PCR_REG(PORTC_BASE_PTR,3)
#define PORTC_PCR4                               PORT_PCR_REG(PORTC_BASE_PTR,4)
#define PORTC_PCR5                               PORT_PCR_REG(PORTC_BASE_PTR,5)
#define PORTC_PCR6                               PORT_PCR_REG(PORTC_BASE_PTR,6)
#define PORTC_PCR7                               PORT_PCR_REG(PORTC_BASE_PTR,7)
#define PORTC_GPCLR                              PORT_GPCLR_REG(PORTC_BASE_PTR)
#define PORTC_GPCHR                              PORT_GPCHR_REG(PORTC_BASE_PTR)
#define PORTC_ISFR                               PORT_ISFR_REG(PORTC_BASE_PTR)
#define PORTC_DFER                               PORT_DFER_REG(PORTC_BASE_PTR)
#define PORTC_DFCR                               PORT_DFCR_REG(PORTC_BASE_PTR)
#define PORTC_DFWR                               PORT_DFWR_REG(PORTC_BASE_PTR)
/* PORTD */
#define PORTD_PCR0                               PORT_PCR_REG(PORTD_BASE_PTR,0)
#define PORTD_PCR1                               PORT_PCR_REG(PORTD_BASE_PTR,1)
#define PORTD_PCR2                               PORT_PCR_REG(PORTD_BASE_PTR,2)
#define PORTD_PCR3                               PORT_PCR_REG(PORTD_BASE_PTR,3)
#define PORTD_PCR4                               PORT_PCR_REG(PORTD_BASE_PTR,4)
#define PORTD_PCR5                               PORT_PCR_REG(PORTD_BASE_PTR,5)
#define PORTD_PCR6                               PORT_PCR_REG(PORTD_BASE_PTR,6)
#define PORTD_PCR7                               PORT_PCR_REG(PORTD_BASE_PTR,7)
#define PORTD_GPCLR                              PORT_GPCLR_REG(PORTD_BASE_PTR)
#define PORTD_GPCHR                              PORT_GPCHR_REG(PORTD_BASE_PTR)
#define PORTD_ISFR                               PORT_ISFR_REG(PORTD_BASE_PTR)
#define PORTD_DFER                               PORT_DFER_REG(PORTD_BASE_PTR)
#define PORTD_DFCR                               PORT_DFCR_REG(PORTD_BASE_PTR)
#define PORTD_DFWR                               PORT_DFWR_REG(PORTD_BASE_PTR)
/* PORTE */
#define PORTE_PCR0                               PORT_PCR_REG(PORTE_BASE_PTR,0)
#define PORTE_PCR1                               PORT_PCR_REG(PORTE_BASE_PTR,1)
#define PORTE_PCR2                               PORT_PCR_REG(PORTE_BASE_PTR,2)
#define PORTE_PCR3                               PORT_PCR_REG(PORTE_BASE_PTR,3)
#define PORTE_PCR4                               PORT_PCR_REG(PORTE_BASE_PTR,4)
#define PORTE_PCR5                               PORT_PCR_REG(PORTE_BASE_PTR,5)
#define PORTE_PCR6                               PORT_PCR_REG(PORTE_BASE_PTR,6)
#define PORTE_PCR7                               PORT_PCR_REG(PORTE_BASE_PTR,7)
#define PORTE_GPCLR                              PORT_GPCLR_REG(PORTE_BASE_PTR)
#define PORTE_GPCHR                              PORT_GPCHR_REG(PORTE_BASE_PTR)
#define PORTE_ISFR                               PORT_ISFR_REG(PORTE_BASE_PTR)
#define PORTE_DFER                               PORT_DFER_REG(PORTE_BASE_PTR)
#define PORTE_DFCR                               PORT_DFCR_REG(PORTE_BASE_PTR)
#define PORTE_DFWR                               PORT_DFWR_REG(PORTE_BASE_PTR)
/* PORTF */
#define PORTF_PCR0                               PORT_PCR_REG(PORTF_BASE_PTR,0)
#define PORTF_PCR1                               PORT_PCR_REG(PORTF_BASE_PTR,1)
#define PORTF_PCR2                               PORT_PCR_REG(PORTF_BASE_PTR,2)
#define PORTF_PCR3                               PORT_PCR_REG(PORTF_BASE_PTR,3)
#define PORTF_PCR4                               PORT_PCR_REG(PORTF_BASE_PTR,4)
#define PORTF_PCR5                               PORT_PCR_REG(PORTF_BASE_PTR,5)
#define PORTF_PCR6                               PORT_PCR_REG(PORTF_BASE_PTR,6)
#define PORTF_PCR7                               PORT_PCR_REG(PORTF_BASE_PTR,7)
#define PORTF_GPCLR                              PORT_GPCLR_REG(PORTF_BASE_PTR)
#define PORTF_GPCHR                              PORT_GPCHR_REG(PORTF_BASE_PTR)
#define PORTF_ISFR                               PORT_ISFR_REG(PORTF_BASE_PTR)
#define PORTF_DFER                               PORT_DFER_REG(PORTF_BASE_PTR)
#define PORTF_DFCR                               PORT_DFCR_REG(PORTF_BASE_PTR)
#define PORTF_DFWR                               PORT_DFWR_REG(PORTF_BASE_PTR)
/* PORTG */
#define PORTG_PCR0                               PORT_PCR_REG(PORTG_BASE_PTR,0)
#define PORTG_PCR1                               PORT_PCR_REG(PORTG_BASE_PTR,1)
#define PORTG_PCR2                               PORT_PCR_REG(PORTG_BASE_PTR,2)
#define PORTG_PCR3                               PORT_PCR_REG(PORTG_BASE_PTR,3)
#define PORTG_PCR4                               PORT_PCR_REG(PORTG_BASE_PTR,4)
#define PORTG_PCR5                               PORT_PCR_REG(PORTG_BASE_PTR,5)
#define PORTG_PCR6                               PORT_PCR_REG(PORTG_BASE_PTR,6)
#define PORTG_PCR7                               PORT_PCR_REG(PORTG_BASE_PTR,7)
#define PORTG_GPCLR                              PORT_GPCLR_REG(PORTG_BASE_PTR)
#define PORTG_GPCHR                              PORT_GPCHR_REG(PORTG_BASE_PTR)
#define PORTG_ISFR                               PORT_ISFR_REG(PORTG_BASE_PTR)
#define PORTG_DFER                               PORT_DFER_REG(PORTG_BASE_PTR)
#define PORTG_DFCR                               PORT_DFCR_REG(PORTG_BASE_PTR)
#define PORTG_DFWR                               PORT_DFWR_REG(PORTG_BASE_PTR)
/* PORTH */
#define PORTH_PCR0                               PORT_PCR_REG(PORTH_BASE_PTR,0)
#define PORTH_PCR1                               PORT_PCR_REG(PORTH_BASE_PTR,1)
#define PORTH_PCR2                               PORT_PCR_REG(PORTH_BASE_PTR,2)
#define PORTH_PCR3                               PORT_PCR_REG(PORTH_BASE_PTR,3)
#define PORTH_PCR4                               PORT_PCR_REG(PORTH_BASE_PTR,4)
#define PORTH_PCR5                               PORT_PCR_REG(PORTH_BASE_PTR,5)
#define PORTH_PCR6                               PORT_PCR_REG(PORTH_BASE_PTR,6)
#define PORTH_PCR7                               PORT_PCR_REG(PORTH_BASE_PTR,7)
#define PORTH_GPCLR                              PORT_GPCLR_REG(PORTH_BASE_PTR)
#define PORTH_GPCHR                              PORT_GPCHR_REG(PORTH_BASE_PTR)
#define PORTH_ISFR                               PORT_ISFR_REG(PORTH_BASE_PTR)
#define PORTH_DFER                               PORT_DFER_REG(PORTH_BASE_PTR)
#define PORTH_DFCR                               PORT_DFCR_REG(PORTH_BASE_PTR)
#define PORTH_DFWR                               PORT_DFWR_REG(PORTH_BASE_PTR)
/* PORTI */
#define PORTI_PCR0                               PORT_PCR_REG(PORTI_BASE_PTR,0)
#define PORTI_PCR1                               PORT_PCR_REG(PORTI_BASE_PTR,1)
#define PORTI_PCR2                               PORT_PCR_REG(PORTI_BASE_PTR,2)
#define PORTI_PCR3                               PORT_PCR_REG(PORTI_BASE_PTR,3)
#define PORTI_PCR4                               PORT_PCR_REG(PORTI_BASE_PTR,4)
#define PORTI_PCR5                               PORT_PCR_REG(PORTI_BASE_PTR,5)
#define PORTI_PCR6                               PORT_PCR_REG(PORTI_BASE_PTR,6)
#define PORTI_PCR7                               PORT_PCR_REG(PORTI_BASE_PTR,7)
#define PORTI_GPCLR                              PORT_GPCLR_REG(PORTI_BASE_PTR)
#define PORTI_GPCHR                              PORT_GPCHR_REG(PORTI_BASE_PTR)
#define PORTI_ISFR                               PORT_ISFR_REG(PORTI_BASE_PTR)
#define PORTI_DFER                               PORT_DFER_REG(PORTI_BASE_PTR)
#define PORTI_DFCR                               PORT_DFCR_REG(PORTI_BASE_PTR)
#define PORTI_DFWR                               PORT_DFWR_REG(PORTI_BASE_PTR)

/* PORT - Register array accessors */
#define PORTA_PCR(index)                         PORT_PCR_REG(PORTA_BASE_PTR,index)
#define PORTB_PCR(index)                         PORT_PCR_REG(PORTB_BASE_PTR,index)
#define PORTC_PCR(index)                         PORT_PCR_REG(PORTC_BASE_PTR,index)
#define PORTD_PCR(index)                         PORT_PCR_REG(PORTD_BASE_PTR,index)
#define PORTE_PCR(index)                         PORT_PCR_REG(PORTE_BASE_PTR,index)
#define PORTF_PCR(index)                         PORT_PCR_REG(PORTF_BASE_PTR,index)
#define PORTG_PCR(index)                         PORT_PCR_REG(PORTG_BASE_PTR,index)
#define PORTH_PCR(index)                         PORT_PCR_REG(PORTH_BASE_PTR,index)
#define PORTI_PCR(index)                         PORT_PCR_REG(PORTI_BASE_PTR,index)

/* GPIO - Register instance definitions */

/** Peripheral GPIOD base pointer */
#define GPIOD_BASE_PTR                           ((GPIO_Type *)0x400FF003u)
/** Peripheral GPIOC base pointer */
#define GPIOC_BASE_PTR                           ((GPIO_Type *)0x400FF002u)
/** Peripheral GPIOB base pointer */
#define GPIOB_BASE_PTR                           ((GPIO_Type *)0x400FF001u)
/** Peripheral GPIOA base pointer */
#define GPIOA_BASE_PTR                           ((GPIO_Type *)0x400FF000u)
/** Peripheral GPIOH base pointer */
#define GPIOH_BASE_PTR                           ((GPIO_Type *)0x400FF043u)
/** Peripheral GPIOG base pointer */
#define GPIOG_BASE_PTR                           ((GPIO_Type *)0x400FF042u)
/** Peripheral GPIOF base pointer */
#define GPIOF_BASE_PTR                           ((GPIO_Type *)0x400FF041u)
/** Peripheral GPIOE base pointer */
#define GPIOE_BASE_PTR                           ((GPIO_Type *)0x400FF040u)
/** Peripheral GPIOI base pointer */
#define GPIOI_BASE_PTR                           ((GPIO_Type *)0x400FF080u)
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS                           { GPIOA_BASE_PTR, GPIOB_BASE_PTR, GPIOC_BASE_PTR, GPIOD_BASE_PTR, GPIOE_BASE_PTR, GPIOF_BASE_PTR, GPIOG_BASE_PTR, GPIOH_BASE_PTR, GPIOI_BASE_PTR }

/* GPIO - Register accessors */
#define GPIO_PDOR_REG(base)                      ((base)->PDOR)
#define GPIO_PDIR_REG(base)                      ((base)->PDIR)
#define GPIO_PDDR_REG(base)                      ((base)->PDDR)
#define GPIO_GACR_REG(base)                      ((base)->GACR)

/* GPIOA */
#define GPIOA_PDOR                               GPIO_PDOR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDIR                               GPIO_PDIR_REG(GPIOA_BASE_PTR)
#define GPIOA_PDDR                               GPIO_PDDR_REG(GPIOA_BASE_PTR)
#define GPIOA_GACR                               GPIO_GACR_REG(GPIOA_BASE_PTR)
/* GPIOB */
#define GPIOB_PDOR                               GPIO_PDOR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDIR                               GPIO_PDIR_REG(GPIOB_BASE_PTR)
#define GPIOB_PDDR                               GPIO_PDDR_REG(GPIOB_BASE_PTR)
#define GPIOB_GACR                               GPIO_GACR_REG(GPIOB_BASE_PTR)
/* GPIOC */
#define GPIOC_PDOR                               GPIO_PDOR_REG(GPIOC_BASE_PTR)
#define GPIOC_PDIR                               GPIO_PDIR_REG(GPIOC_BASE_PTR)
#define GPIOC_PDDR                               GPIO_PDDR_REG(GPIOC_BASE_PTR)
#define GPIOC_GACR                               GPIO_GACR_REG(GPIOC_BASE_PTR)
/* GPIOD */
#define GPIOD_PDOR                               GPIO_PDOR_REG(GPIOD_BASE_PTR)
#define GPIOD_PDIR                               GPIO_PDIR_REG(GPIOD_BASE_PTR)
#define GPIOD_PDDR                               GPIO_PDDR_REG(GPIOD_BASE_PTR)
#define GPIOD_GACR                               GPIO_GACR_REG(GPIOD_BASE_PTR)
/* GPIOE */
#define GPIOE_PDOR                               GPIO_PDOR_REG(GPIOE_BASE_PTR)
#define GPIOE_PDIR                               GPIO_PDIR_REG(GPIOE_BASE_PTR)
#define GPIOE_PDDR                               GPIO_PDDR_REG(GPIOE_BASE_PTR)
#define GPIOE_GACR                               GPIO_GACR_REG(GPIOE_BASE_PTR)
/* GPIOF */
#define GPIOF_PDOR                               GPIO_PDOR_REG(GPIOF_BASE_PTR)
#define GPIOF_PDIR                               GPIO_PDIR_REG(GPIOF_BASE_PTR)
#define GPIOF_PDDR                               GPIO_PDDR_REG(GPIOF_BASE_PTR)
#define GPIOF_GACR                               GPIO_GACR_REG(GPIOF_BASE_PTR)
/* GPIOG */
#define GPIOG_PDOR                               GPIO_PDOR_REG(GPIOG_BASE_PTR)
#define GPIOG_PDIR                               GPIO_PDIR_REG(GPIOG_BASE_PTR)
#define GPIOG_PDDR                               GPIO_PDDR_REG(GPIOG_BASE_PTR)
#define GPIOG_GACR                               GPIO_GACR_REG(GPIOG_BASE_PTR)
/* GPIOH */
#define GPIOH_PDOR                               GPIO_PDOR_REG(GPIOH_BASE_PTR)
#define GPIOH_PDIR                               GPIO_PDIR_REG(GPIOH_BASE_PTR)
#define GPIOH_PDDR                               GPIO_PDDR_REG(GPIOH_BASE_PTR)
#define GPIOH_GACR                               GPIO_GACR_REG(GPIOH_BASE_PTR)
/* GPIOI */
#define GPIOI_PDOR                               GPIO_PDOR_REG(GPIOI_BASE_PTR)
#define GPIOI_PDIR                               GPIO_PDIR_REG(GPIOI_BASE_PTR)
#define GPIOI_PDDR                               GPIO_PDDR_REG(GPIOI_BASE_PTR)
#define GPIOI_GACR                               GPIO_GACR_REG(GPIOI_BASE_PTR)


/******************************************************************************
 * definition configuration structures                                        *
 ******************************************************************************/
typedef struct { uint32 PCR, DFER, DFCR, DFWR;  } tPORT; 
typedef struct { uint8 PDDR, GACR;              } tGPIO; 

/******************************************************************************
* PIN names definition
*
*//*! @addtogroup pin_names
* @{
* This device contains 9 eGPIOs ports each having 8 pins (number of ports varies
* and port pins varies on each package).
*******************************************************************************/
#define PIN0    (uint8)(1<<0)     ///< PIN 0 (LSB)
#define PIN1    (uint8)(1<<1)     ///< PIN 1 
#define PIN2    (uint8)(1<<2)     ///< PIN 2 
#define PIN3    (uint8)(1<<3)     ///< PIN 3 
#define PIN4    (uint8)(1<<4)     ///< PIN 4
#define PIN5    (uint8)(1<<5)     ///< PIN 5
#define PIN6    (uint8)(1<<6)     ///< PIN 6
#define PIN7    (uint8)(1<<7)     ///< PIN 7 (MSB)
/*! @} End of pin_names                                                       */

/******************************************************************************
* PORT default configurations used by PORT_init() function
*
*//*! @addtogroup port_config
* @{
*******************************************************************************/
/**************************************************************************/ /*!
 * @brief Disables pin (default mode after reset).
 *****************************************************************************/
#define PORT_MODULE_DISABLED_MODE                                             \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(0))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Disables pin with internal pull-up on.
 *****************************************************************************/
#define PORT_MODULE_DISABLED_PULLUP                                           \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(0))|  \
           CLR(PORT_PCR_SRE_MASK)|SET(PORT_PCR_PE_MASK)|                      \
           SET(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 1 (GPIO) mode. Internal pull-ups, filter 
 * and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT1_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(1))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 1 (GPIO) mode. Prepared for controlling 
 * slow outputs such as LEDs: Slew rate control enabled, internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_LED_MODE                                                  \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(1))|  \
           SET(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 2 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT2_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(2))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 3 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT3_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(3))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 4 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT4_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(4))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 5 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT5_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(5))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 6 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT6_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(6))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 7 (chip-specific) mode. Internal pull-ups, 
 * filter and interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_ALT7_MODE                                                 \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(7))|  \
           CLR(PORT_PCR_SRE_MASK)|CLR(PORT_PCR_PE_MASK)|                      \
           CLR(PORT_PCR_PS_MASK),                                             \
/* DFER */ CLR(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(0)),                                            \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 1 (GPIO) mode. Prepared for button 
 * processing using pooling mode: Internal pull-up enabled, digital filter enabled 
 * and clocked by bus clock. Interrupt disabled.
 *****************************************************************************/
#define PORT_MODULE_BUTTON_MODE                                               \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(0))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(1))|  \
           CLR(PORT_PCR_SRE_MASK)|SET(PORT_PCR_PE_MASK)|                      \
           SET(PORT_PCR_PS_MASK),                                             \
/* DFER */ SET(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(10)),                                           \
}

/**************************************************************************/ /*!
 * @brief Configures pin to Alternative 1 (GPIO) mode. Prepared for button 
 * processing using interrupts: Internal pull-up enabled, digital filter enabled 
 * and clocked by bus clock. Interrupt enabled.
 *****************************************************************************/
#define PORT_MODULE_BUTTON_IRQ_MODE                                           \
(tPORT){                                                                      \
/* PCR  */ SET(PORT_PCR_IRQC(10))|CLR(PORT_PCR_LK_MASK)|SET(PORT_PCR_MUX(1))| \
           CLR(PORT_PCR_SRE_MASK)|SET(PORT_PCR_PE_MASK)|                      \
           SET(PORT_PCR_PS_MASK),                                             \
/* DFER */ SET(PORT_DFER_DFE_MASK), /* use mask for all pins */               \
/* DFCR */ CLR(PORT_DFCR_CS_MASK),                                            \
/* DFWR */ SET(PORT_DFWR_FILT(10)),                                           \
}
/*! @} End of port_config                                                     */

/******************************************************************************
* GPIO default configurations used by GPIO_init() function
*
*//*! @addtogroup gpio_config
* @{
*******************************************************************************/
/**************************************************************************/ /*!
 * @brief Configures pin in output mode.
 *****************************************************************************/
#define GPIO_OUT_MODE                                                         \
(tGPIO){                                                                      \
/* PDDR     */ SET(GPIO_PDDR_PDD_MASK), /* use mask for all pins */           \
/* GACR     */ SET(GPIO_GACR_ACB(0)),                                         \
}

/**************************************************************************/ /*!
 * @brief Configures pin in input mode.
 *****************************************************************************/
#define GPIO_INP_MODE                                                         \
(tGPIO){                                                                      \
/* PDDR     */ CLR(GPIO_PDDR_PDD_MASK), /* use mask for all pins */           \
/* GACR     */ SET(GPIO_GACR_ACB(0)),                                         \
}
/*! @} End of gpio_config                                                     */

/******************************************************************************
* PORT callback registered by PORT_InstallCallback () function
*
*//*! @addtogroup port_callback
* @{
*******************************************************************************/
/*! @brief PORT_CALLBACK source declaration                                   */
typedef enum 
{ 
  PORTA_CALLBACK = (1<<0),  ///< PORTA interrupt
  PORTB_CALLBACK = (1<<1),  ///< PORTB interrupt
  PORTC_CALLBACK = (1<<2),  ///< PORTC interrupt
  PORTD_CALLBACK = (1<<3),  ///< PORTD interrupt
  PORTE_CALLBACK = (1<<4),  ///< PORTE interrupt
  PORTF_CALLBACK = (1<<5),  ///< PORTF interrupt
  PORTG_CALLBACK = (1<<6),  ///< PORTG interrupt
  PORTH_CALLBACK = (1<<7),  ///< PORTH interrupt
  PORTI_CALLBACK = (1<<8),  ///< PORTI interrupt
} PORT_CALLBACK_SRC;

/*! @brief PORT_CALLBACK function declaration                                 */
typedef void (*PORT_CALLBACK)(PORT_CALLBACK_SRC src, uint8 pin);
/*! @} End of port_callback                                                   */

/******************************************************************************
* GPIO function and macro definitions
*
*//*! @addtogroup gpio_macro
* @{
*******************************************************************************/
/***************************************************************************//*!
 * @brief   Macro returns state of pins.
 * @details This macro returns state of pins.
 * @param   gpio    GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI
 * @return  @ref uint8 value
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define GPIO_Get(gpio)      gpio##_PDIR       

/***************************************************************************//*!
 * @brief   Macro sets pins of the port.
 * @details This macro sets pins of the port.
 * @param   gpio      GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI
 * @param   mask      Use one or more ORed pins @ref pin_names  
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define GPIO_Set(gpio,mask) gpio##_PDOR|=(mask);

/***************************************************************************//*!
 * @brief   Macro clears pins of the port.
 * @details This macro clears pins of the port.
 * @param   gpio      GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI
 * @param   mask      Use one or more ORed pins @ref pin_names  
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define GPIO_Clr(gpio,mask) gpio##_PDOR&=~(mask);

/***************************************************************************//*!
 * @brief   Macro toggles pins of the port.
 * @details This macro toggles pins of the port.
 * @param   gpio      GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI
 * @param   mask      Use one or more ORed pins @ref pin_names  
 * @note    Implemented as inlined macro.
 ******************************************************************************/
#define GPIO_Tgl(gpio,mask) gpio##_PDOR^=(mask);

/***************************************************************************//*!
 * @brief   Function installs callback function.
 * @details This function install callback function for interrupt vector 33.
 * @param   ip        Interrupt priority 0..3
 * @param   callback  Pointer to @ref PORT_CALLBACK function
 * @note    Implemented as function call.
 * @warning All PORT interrupt sources are ORed into single interrupt vector.
 ******************************************************************************/
#define PORT_InstallCallback(ip,callback) PORT_InstallCallback (ip,callback)

/***************************************************************************//*!
 * @brief   PORT initialization function.
 * @details This function initalizes selected port pins.
 * @param   port      PORTA,PORTB,PORTC,PORTD,PORTE,PORTF,PORTG,PORTH,PORTI
 * @param   cfg       Use one of following port configuration structures @ref 
 *                    port_config
 * @param   mask      Use one or more ORed pin names @ref pin_names
 * @note    Implemented as function call.
 ******************************************************************************/ 
#define PORT_Init(port,cfg,mask)    port##_Init(cfg,mask)

/***************************************************************************//*!
 * @brief   GPIO initialization function.
 * @details This function initalizes selected pins into either input or output 
 *          mode .
 * @param   gpio      GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI
 * @param   cfg       Use one of following port configuration structures @ref 
 *                    gpio_config
 * @param   mask      Use one or more ORed pin names @ref pin_names
 * @note    Implemented as function call.
 ******************************************************************************/ 
#define GPIO_Init(gpio,cfg,mask)    gpio##_Init(cfg,mask) 
/*! @} End of gpio_macro                                                      */

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/    
extern void PORT_InstallCallback (uint8 ip, PORT_CALLBACK pCallback);

extern void PORTA_Init (tPORT port, uint8 pin_mask);
extern void PORTB_Init (tPORT port, uint8 pin_mask);
extern void PORTC_Init (tPORT port, uint8 pin_mask);
extern void PORTD_Init (tPORT port, uint8 pin_mask);
extern void PORTE_Init (tPORT port, uint8 pin_mask);
extern void PORTF_Init (tPORT port, uint8 pin_mask);
extern void PORTG_Init (tPORT port, uint8 pin_mask);
extern void PORTH_Init (tPORT port, uint8 pin_mask);
extern void PORTI_Init (tPORT port, uint8 pin_mask);

extern void GPIOA_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOB_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOC_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOD_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOE_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOF_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOG_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOH_Init (tGPIO gpio, uint8 pin_mask);
extern void GPIOI_Init (tGPIO gpio, uint8 pin_mask);
   
#endif /* __GPIO_H */
