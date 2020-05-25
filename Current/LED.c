//// Implementation of three LEDs on PF1, PF2, and PF3.

//#include <stdint.h>
//#include "C:\ti\TivaWare_C_Series-2.1.4.178\inc\tm4c123gh6pm.h"
//#include "LED.h"

//#define PORTF_DATA  (*((volatile unsigned long *)0x40025038))   //offset of DATA register for PF1, PF2, PF3 is 0x38 [PF7:PF0::9:2]

//void LED_Init(void){
//	
//volatile unsigned long delay;

//  SYSCTL_RCGC2_R |= 0x00000020;     // activate clock for Port F
//  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
//  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
//  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
//  // only PF0 needs to be unlocked, other bits can't be locked
//  GPIO_PORTF_AMSEL_R = 0x00;        // disable analog on PF
//  GPIO_PORTF_PCTL_R = 0x00000000;   // PCTL GPIO on PF4-0
//  GPIO_PORTF_DIR_R = 0x0E;          // PF4,PF0 in, PF3-1 out
//  GPIO_PORTF_AFSEL_R = 0x00;        // disable alt funct on PF7-0
//  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
//  GPIO_PORTF_DEN_R = 0x1F;          // enable digital I/O on PF4-0
//}

//void Red_on(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Red_off(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Green_on(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Green_off(void)
//{
//	GPIO_PORTB_DATA_R &= ~0x10;
//}

//void Blue_on(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Blue_off(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Cyan_on(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Cyan_off(void)
//{
//	GPIO_PORTF_DATA_R |= 0x10;
//}

//void Rainbow(void)
//{
//	PORTF_DATA = 0;
//	PORTF_DATA = (s << 1);
//	s = (bit2(~s) >> 2) | (((bit2(~s) >> 1) & bit1(s) | (bit2(s) >> 1) & (bit0(~s) << 1))) | ((bit2(s) & (bit1(~s) << 1) | bit2(~s) & (bit0(s) << 2))); //flash all combinations
//	Delay100ms(3); // 3 red, yellow, green, cyan, blue, pink, white, off
//}




