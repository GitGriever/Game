/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                        modified by Keil */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "M051Series.h"


/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/    

int main (void)
{	

  SystemCoreClockUpdate();

	/*set GP0 outpui*/
  _GPIO_SET_PIN_MODE(P3, 6, GPIO_PMD_OUTPUT);

	while(1)
	{
    P36 = 0;
    
		SYS_SysTickDelay(100000);

    P36 = 1;

		SYS_SysTickDelay(100000);
		
	} 

}




