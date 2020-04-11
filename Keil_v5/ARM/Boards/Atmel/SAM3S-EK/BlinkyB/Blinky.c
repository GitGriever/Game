/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2010 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
#include <sam3s.h>


volatile unsigned long SysTickCnt;         /* SysTick Counter                 */
unsigned int count = 0;

void SysTick_Handler (void) {              /* SysTick Interrupt Handler (1ms) */
  SysTickCnt++;
}

void Delay (unsigned long tick) {          /* Delay Function                  */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}

int main (void) {                          /* Main Program                    */
  int num = 1; 

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);    /* Generate interrupt each 1 ms    */

  PMC->PMC_PCER0   =   (1<<ID_PIOC);
  PIOC->PIO_PER    =  ((1<<20));           /* Setup Pins PC20 for RED LED     */
  PIOC->PIO_OER    =  ((1<<20));
  PIOC->PIO_PUDR   =  ((1<<20));
  PIOC->PIO_OWER   =  ((1<<20));

  PMC->PMC_PCER0   =   (1<<ID_PIOA);
  PIOA->PIO_PER    =  ((1<<20) |(1<<19));  /* Setup Pins PA19..PA20           */
  PIOA->PIO_OER    =  ((1<<20) |(1<<19));  /* for Green, Blue LEDs            */
  PIOA->PIO_PUDR   =  ((1<<20) |(1<<19));  /* These instructions are for RevB */
  PIOA->PIO_OWER   =  ((1<<20) |(1<<19));
   
  for (;;) {                               /* Loop forever                    */
    
    num = !num;
   	if (count > 10) (count = 0);

    PIOC->PIO_CODR = (1<<20);              /* red LED ON                      */

    /* toggle blue and green LEDs using PIO_ODSR instead of CODR */
	if (num == 1)
      PIOA->PIO_ODSR = (1<<20); 
    else
      PIOA->PIO_ODSR = (1<<19);
	count ++;
	Delay(500);                	           /* Delay 500ms                     */
    
	PIOC->PIO_SODR = (1<<20);              /* red LEd OFF                     */

    /* toggle blue and green LEDs using PIO_ODSR instead of CODR */
	if (num == 1)
      PIOA->PIO_ODSR = (1<<20); 
    else
      PIOA->PIO_ODSR = (1<<19);
	count ++;
	Delay(500);                            /* Delay 500ms                     */

  }
}
