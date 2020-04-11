/*******************************************************************************
 * @file     system_s3fn429.c
 * @brief    CMSIS Cortex-M0 Device System Source File
 *           for S3FN429 Device
 * @version  V1.0
 * @date     01. May 2012
 *
 * @note
 * Copyright (C) 2012 by MCU Application Team, Samsung Electronics CO.,LTD.. 
 *			  All rights reserved.
 *
 * @attention
 * The software is delivered "AS IS" without warranty or condition of any kind,
 * either express, implied or statutory. This includes without limitation
 * any warranty or condition with respect to merchantability or fitness for
 * any particular purpose, or against the infringements of intellectual property
 * rights of others.
 *
 ******************************************************************************/

#include "s3fn429.h"	

#define DISABLE_WDT 	1
/* Predefined watchdog timer operation
  |--------------------------------------------------|
  | 0 |	No change enabled status (Enabled by default)|
  | 1 |	Disable watchdog timer					     |
  |--------------------------------------------------|
*/

#define CLOCK_SETUP     2
/* Predefined clock setups
 0 - System clock source will be IMCLK.
	 Core clock = IMCLK 40MHz, APB Clock = IMCLK 40MHz  

 1 - System clock source will be EMCLK.	 
	 Core clock = __XTAL, APB Clock = __XTAL		

 2 - System clock source will be PLL output clock.
     Reference clock source for PLL is an external crystal __XTAL.
     Core Clock = 40MHz, APB Clock = 40MHz
	 		 
 ¡Ø  System clock sources (Clock Manager)
 	 |-------------------------------|
 	 |Source | SYSCLK[1:0]<-MR1 Reg. |
 	 |-------------------------------|  
 	 |EMCLK  |		0				 |
 	 |IMCLK	 |		1				 |
 	 |PLLCLK |		2			 	 |
 	 |-------------------------------|
	  CORECLK = SYSCLK/SDIV
	  PCLK = (SYSCLK/SDIV)/PDIV
		
 ¡Ø	 Flash speed mode (Internal Flash Controller)
	 Set or not accoring to the core operating frequency 
 	 |-------------------------------------|   
 	 | Core Clock 		 | FSMODE <-MR Reg.|			
 	 |-------------------|-----------------|   
  	 | CoreClock <= 20MHz|	 0  		   |      
 	 |-------------------|-----------------|   
 	 | CoreClock  > 20MHz|	 1	   		   | 
 	 |-------------------------------------|	 
*/


/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* Oscillator frequency,EMCLK 4MHz, - Need to change depending on system condition */
#define __XTAL            ( 4000000UL)  
#define EMCLK_Value        __XTAL	

#define IMCLK_Value		  (40000000UL)  

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = IMCLK_Value;/*!< System Clock Frequency (Core Clock)*/

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

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
	uint8_t i=8;

#if (DISABLE_WDT)
	/* Disable watch-dog timer for test*/
	/* Configure WDT for target system */ 
  	WDT0->OMR = WDT_OKEY;		
#endif /* (DISABLE_WDT) */
	
#if (CLOCK_SETUP == 0) /* IMCLK */
	
	/* Check the reset value of SYSCLK in MR1 register. */
	/* If the default system clock source is EMCLK */
	if((CM0->MR1 & CM_SYSCLK_MASK) == CM_SYSCLK_SEL(0)) 
	{
		/* Check IMCLK */
		while((CM0->SR & CM_IMCLK)!= CM_IMCLK);

		/* Clear STABLE flag */		
		CM0->ICR = CM_STABLE;
		
		/* Change the system clock to IMCLK clock */
		CM0->MR1 = (CM0->MR1 & (~CM_SYSCLK_MASK))|CM_SYSCLK_SEL(0x1);						
		while((CM0->RISR & CM_STABLE)!= CM_STABLE);
		
		/* Clear STABLE flag */
		CM0->ICR = CM_STABLE;	
	}		
	
	/* Enable peripheral clock to access SFR */  
	/* You can access and control SFR of each peripheral(IFC) */  
	CM0->PCSR = CM_IFCCLK; 	
	
	/* Enable IFC block */
	IFC0->CEDR = IFC_CLKEN;	
	
	/* Configure the flash mode. Set high speed flash mode for 20MHz over */
	IFC0->MR |= IFC_FSMODE; 
	
	/* system clock divider from /8(reset default value) to /1 */
	CM0->SCDR = CM_SDIVKEY|CM_SDIV(CM_SYSCLK_DIV1);
	while(i--);	
		
#elif (CLOCK_SETUP == 1) /* EMCLK */	
	
	/* Check the reset value of SYSCLK in MR1 register. */
	/* If the default system clock source is IMCLK */
	if((CM0->MR1 & CM_SYSCLK_MASK) == CM_SYSCLK_SEL(1))
	{		
		/* Check EMCLK */
		while((CM0->SR & CM_EMCLK) != CM_EMCLK);
	
		/* Clear STABLE flag */		
		CM0->ICR = CM_STABLE;
		
		/* Change the system clock to EMCLK clock */
		CM0->MR1 = (CM0->MR1 & (~CM_SYSCLK_MASK))|CM_SYSCLK_SEL(0x0);						
		while((CM0->RISR & CM_STABLE)!= CM_STABLE);
		
		/* Clear STABLE flag */
		CM0->ICR = CM_STABLE;
	}	

	/* system clock divider from /8(reset default value) to /1 */
	CM0->SCDR = CM_SDIVKEY|CM_SDIV(CM_SYSCLK_DIV1);
	while(i--);	
	
#elif (CLOCK_SETUP == 2) /* PLLCLK */	
	
	/* Check EMCLK */
	while((CM0->SR & CM_EMCLK) != CM_EMCLK);
	
	/* Set P,M,S value for PLL clock generation (40MHz) */
	CM0->PDPR = CM_PLLKEY|CM_PLLPRE(2)|CM_PLLMUL(32)|CM_PLLPOST(0);	 
	
	/* Enable PLL */
	CM0->CSR = CM_PLL; 		
	while((CM0->RISR & CM_PLL)!= CM_PLL);
	
	/* Clear PLL flag */
	CM0->ICR = CM_PLL;
		
	/* Enable peripheral clock - IFC */  
	/* You can access and control SFR of each peripheral */  
	CM0->PCSR = CM_IFCCLK;

	/* Enable IFC clock */
	IFC0->CEDR = IFC_CLKEN;	
		
	/* Configure the flash mode. Set fast-mode for 20MHz over */
	IFC0->MR |= IFC_FSMODE; 		
			
	/* Clear STABLE flag */		
	CM0->ICR = CM_STABLE;
	
	/* Change the system clock to PLL clock */
	CM0->MR1 = (CM0->MR1 & (~CM_SYSCLK_MASK))|CM_SYSCLK_SEL(0x2);						
	while((CM0->RISR & CM_STABLE)!= CM_STABLE);

	/* system clock divider from /8(reset default) to /1 */
	CM0->SCDR = CM_SDIVKEY|CM_SDIV(CM_SYSCLK_DIV1);
	while(i--);	
	
	/* Clear STABLE flag */
	CM0->ICR = CM_STABLE;

#endif /* (CLOCK_SETUP == x) */
} 


/**
 * Update CoreClock
 *
 * @param  none
 * @return none
 *
 * @brief  Update SystemCoreClock according to clock register values
 */
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
	uint32_t temp = 0;
	uint32_t sysClk_u32 = 1, get_sdiv = 0;
	uint32_t pms = 0, pllpre = 2, pllmul = 32, pllpost = 0;


	/* get the SYSCLK field value */
	sysClk_u32 = CM0->MR1 & CM_SYSCLK_MASK;

	switch(sysClk_u32)
	{
	case 0x0:/* If SYSCLK = EMCLK */
			temp = EMCLK_Value;
			break;
	case 0x1:/* If SYSCLK = IMCLK */
			temp = IMCLK_Value;
			break;
	case 0x2: /* If SYSCLK = PLLCLK */
			pms = CM0->PDPR;
			pllpre = (pms & CM_PLLPRE_MASK) >> 8;
			pllmul = (pms & CM_PLLMUL_MASK);
			pllpost = (pms & CM_PLLPOST_MASK) >> 16;
			temp = ((EMCLK_Value * (pllmul+8) / (pllpre+2)) >> pllpost); //-> MAIN
			break;
	default:temp = IMCLK_Value;	 
			break;
	}

	/* ROOTCLK */
	/* get the number of shift to calculate the actual SDIV ratio */
	get_sdiv = CM0->SCDR & CM_SDIV_MASK;
	SystemCoreClock = temp /(get_sdiv+1);
}






