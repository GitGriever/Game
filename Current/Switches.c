#include "Switches.h"
#include "tm4c123gh6pm.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

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



