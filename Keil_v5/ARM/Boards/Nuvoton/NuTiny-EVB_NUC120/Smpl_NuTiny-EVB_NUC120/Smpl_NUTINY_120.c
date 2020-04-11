/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvGPIO.h"

void delay_loop(void)
 {
 uint32_t j;
 		for(j=0;j<60000;j++);		
		for(j=0;j<60000;j++);
    	for(j=0;j<60000;j++);		
		for(j=0;j<60000;j++);
 
 }


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/    

int main (void)
{	
  // uint32_t i,k;

	/* SYSCLK =>12Mhz*/
	UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
	
	/*set GPB0 output*/
	
	DrvGPIO_Open(E_GPB,0, E_IO_OUTPUT);
		
	while(1)
	{
	    
        DrvGPIO_ClrBit(E_GPB,0);
		delay_loop();
		DrvGPIO_SetBit(E_GPB,0);
		delay_loop();
		


	} 



}




