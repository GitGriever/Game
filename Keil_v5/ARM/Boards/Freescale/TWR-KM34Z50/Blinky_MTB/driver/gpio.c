/*****************************************************************************
 * (c) Copyright 2010-2012, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      gpio.c
 * @version   1.0.2.0
 * @date      May-14-2012
 * @brief     General-Purpose Input/Output (GPIO) driver source code..
 *
 * Revision History:
 *  1.	01/01/2010   R55013   Initially written
 *  2.	01/08/2012   R55013   Ported to Kinetis M  
 *  3.	16/04/2013   By ARM   Modifed for RVCT & CMSIS adaptation
 ******************************************************************************/
#include "common.h"
#include "gpio.h"

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static PORT_CALLBACK pCallbackPTx=NULL;

/******************************************************************************
 * interrupt function prototypes                                              *
 ******************************************************************************/
void PTx_isr (void);

/******************************************************************************
 * Public function definitions                                                *
 ******************************************************************************/
void PORT_InstallCallback (uint8 ip, PORT_CALLBACK pCallback)
{
  if (pCallback != NULL) { pCallbackPTx = pCallback; NVIC_EnableIRQ(PTx_IRQn); NVIC_SetPriority(PTx_IRQn, ip); }
}


#if defined(__ARMCC_VERSION)
  #pragma diag_suppress=1
#else
  #pragma diag_suppress=Pa082
#endif
void PORTA_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTA_BASE_PTR->PCR[i] = port.PCR; } 
  PORTA_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTA_DFCR  = port.DFCR;
  PORTA_DFWR  = port.DFWR;   
  PORTA_ISFR &= ~pin_mask;  
}

void PORTB_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;

  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTB_BASE_PTR->PCR[i] = port.PCR; }
  PORTB_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTB_DFCR  = port.DFCR;              
  PORTB_DFWR  = port.DFWR;   
  PORTB_ISFR &= ~pin_mask;  
}

void PORTC_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTC_BASE_PTR->PCR[i] = port.PCR; }
  PORTC_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTC_DFCR  = port.DFCR;              
  PORTC_DFWR  = port.DFWR;  
  PORTC_ISFR &= ~pin_mask;
}

void PORTD_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;

  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTD_BASE_PTR->PCR[i] = port.PCR; }
  PORTD_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTD_DFCR  = port.DFCR;              
  PORTD_DFWR  = port.DFWR;  
  PORTD_ISFR &= ~pin_mask;  
}

void PORTE_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTE_BASE_PTR->PCR[i] = port.PCR; } 
  PORTE_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTE_DFCR  = port.DFCR;              
  PORTE_DFWR  = port.DFWR;   
  PORTE_ISFR &= ~pin_mask;  
}

void PORTF_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTF_BASE_PTR->PCR[i] = port.PCR; } 
  PORTF_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTF_DFCR  = port.DFCR;              
  PORTF_DFWR  = port.DFWR;   
  PORTF_ISFR &= ~pin_mask;  
}

void PORTG_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTG_BASE_PTR->PCR[i] = port.PCR; } 
  PORTG_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTG_DFCR  = port.DFCR;              
  PORTG_DFWR  = port.DFWR;   
  PORTG_ISFR &= ~pin_mask;  
}

void PORTH_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTH_BASE_PTR->PCR[i] = port.PCR; } 
  PORTH_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTH_DFCR  = port.DFCR;              
  PORTH_DFWR  = port.DFWR;   
  PORTH_ISFR &= ~pin_mask;  
}

void PORTI_Init (tPORT port, uint8 pin_mask)
{
  register uint16 i;
  
  for (i=0; i<8; i++)
    if(pin_mask & (uint8)(1l << i)) { PORTI_BASE_PTR->PCR[i] = port.PCR; } 
  PORTI_DFER |= (port.DFER & pin_mask); /* DFER content OR-ed with pin_mask   */ 
  PORTI_DFCR  = port.DFCR;              
  PORTI_DFWR  = port.DFWR;   
  PORTI_ISFR &= ~pin_mask;  
}

void GPIOA_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOA_PDDR = (GPIOA_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOB_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOB_PDDR = (GPIOB_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOC_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOC_PDDR = (GPIOC_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOD_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOD_PDDR = (GPIOD_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOE_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOE_PDDR = (GPIOE_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOF_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOF_PDDR = (GPIOF_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOG_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOG_PDDR = (GPIOG_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOH_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOH_PDDR = (GPIOH_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask);
}

void GPIOI_Init (tGPIO gpio, uint8 pin_mask)
{
  GPIOI_PDDR = (GPIOI_PDDR & ~pin_mask) | (gpio.PDDR & pin_mask); 
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
void PTx_isr (void) 
{ 
  register uint8 tmp;
  
  /* process interrupt callback function for PORTA                            */  
  tmp = (uint8)PORTA_ISFR;
  PORTA_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTA_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTB                            */  
  tmp = (uint8)PORTB_ISFR;
  PORTB_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTB_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTC                            */  
  tmp = (uint8)PORTC_ISFR;
  PORTC_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTC_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTD                            */  
  tmp = (uint8)PORTD_ISFR;
  PORTD_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTD_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTE                            */  
  tmp = (uint8)PORTE_ISFR;
  PORTE_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTE_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTF                            */  
  tmp = (uint8)PORTF_ISFR;
  PORTF_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTF_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTG                            */  
  tmp = (uint8)PORTG_ISFR;
  PORTG_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTG_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTH                            */  
  tmp = (uint8)PORTH_ISFR;
  PORTH_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTH_CALLBACK, tmp);
  
  /* process interrupt callback function for PORTI                            */  
  tmp = (uint8)PORTI_ISFR;
  PORTI_ISFR |= tmp;                /* clear peripheral interrupt flags       */
  if ((tmp != 0) && (pCallbackPTx != (PORT_CALLBACK)NULL)) 
    pCallbackPTx (PORTI_CALLBACK, tmp);
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/