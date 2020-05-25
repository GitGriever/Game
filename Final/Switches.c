#include "Switches.h"
#include "tm4c123gh6pm.h"

//#include <stdint.h>
//#include <stdbool.h>
//#include "inc/hw_types.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_gpio.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/gpio.h"
//#include "driverlib/interrupt.h"

void Switch_Init(void){
	
	volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000020;     // activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // enable digital I/O on PF4-0
}


// SW1 and SW2 are negative logic

// #define GPIO_PORTF_DATA_R (*((volatile unsigned long *) 0x400253FC))

//int main(void)
//{
//PortFunctionInit(); //initialize the GPIO ports
//while(1) // Loop forever.
//{
//if((GPIO_PORTF_DATA_R&0x01)==0) //SW2 is pressed
//{
//GPIO_PORTF_DATA_R |= 0x04; // Turn on the LED.
//}
//else {
//GPIO_PORTF_DATA_R &= ~0x04; // Turn off the LED.
//}
//}
//} 

//if((GPIO_PORTF_DATA_R&0x01)==0x00) //if SW2 is pressed
//{ ... }
//if((GPIO_PORTF_DATA_R&0x01)==0x01) //if SW2 is released
//{ ... }


//void GPIOPortF_Handler(void)
//{
//	//SW2 is pressed // PF0
//  if(GPIO_PORTF_RIS_R&0x01)
//	{
//		GPIO_PORTF_ICR_R = 0x01;
//		Player.y += 4;
//		Nokia5110_PrintBMP(Player.x, Player.y, Player.image[1], 0);
//	}
//    else if (GPIO_PORTF_RIS_R&0x10)
//	{
//		GPIO_PORTF_ICR_R = 0x10;
//		pause game
//	}
//  }



