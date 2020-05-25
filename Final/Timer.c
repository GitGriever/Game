#include <stdint.h>
#include "..//tm4c123gh6pm.h"
#include "Timer.h"
#include "LED.h"
#include "Switches.h"
#include "Game.h"

//unsigned long Countdown;
//unsigned long TimerCount;
//unsigned long tSeconds;
// unsigned long buttonPress = 0;

void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
	
	
	buttonPress = 0;
	if(Countdown == 1)
	{
 TimerCount++;
		buttonPress = 0;
   	if(SW1 == 0){
		Countdown = 0;
			Timer2A_Stop();
			buttonPress = 0;
			Nokia5110_Clear();
		Nokia5110_ClearBuffer();
			TimerCount = 0;
		}
		
	}
	
	if((TimerCount % 30) == 0)
	{ 
		tSeconds++;
	}
	
	if(tSeconds < 4){
		buttonPress = 0;
	}
	
	if(tSeconds > 3)
	{
		TimerCount = 0;
		tSeconds = 0;
		Countdown = 0;
		Timer2A_Stop(); //  needed
	}
if (tSeconds == 1){
	Delay100ms(0.25);
	Red_on();
//	Green_off();
//	Blue_off();
//	GPIO_PORTF_DATA_R |= 0x02; // Turn on  LED.
//		GPIO_PORTF_DATA_R &= ~0x04;
//		GPIO_PORTF_DATA_R &= ~0x08;
		Nokia5110_SetCursor(3, 1);
    Nokia5110_OutString("Ready,");
	Nokia5110_SetCursor(0, 0);
}
else if (tSeconds == 2){
	Delay100ms(0.25);
  Yellow_on();
//	GPIO_PORTF_DATA_R |= 0x08; // Turn on  LED.
//	GPIO_PORTF_DATA_R &= ~0x04;
//	GPIO_PORTF_DATA_R |= 0x02;
	Nokia5110_SetCursor(3, 1);
    Nokia5110_OutString("Ready,");
	Nokia5110_SetCursor(3, 2);
    Nokia5110_OutString("Set,");
	Nokia5110_SetCursor(0, 0);
}
else if (tSeconds == 3){
	Delay100ms(0.25);
	Green_on();
//	GPIO_PORTF_DATA_R |= 0x08; // Turn on  LED.
//	GPIO_PORTF_DATA_R &= ~0x04;
//	GPIO_PORTF_DATA_R &= ~0x02;
	Nokia5110_SetCursor(3, 1);
    Nokia5110_OutString("Ready,");
	Nokia5110_SetCursor(3, 2);
    Nokia5110_OutString("Set,");
	Nokia5110_SetCursor(3, 3);
    Nokia5110_OutString("Go!");
	Nokia5110_SetCursor(0, 0);
}
else{
	LED_off();
//		  GPIO_PORTF_DATA_R &= ~0x08; 
//		GPIO_PORTF_DATA_R &= ~0x04; 
//		GPIO_PORTF_DATA_R &= ~0x02; 
}
}

void Timer2A_Start(void)
{
	TIMER2_CTL_R |= 0x00000001;
}

void Timer2A_Stop(void)
{
	TIMER2_CTL_R &= ~0x00000001;
}

//void Ready(void){
//	tSeconds = 0;
//buttonPress = 0;
//	
//	if(Countdown == 1){
//		Nokia5110_Clear();
//			Nokia5110_ClearBuffer();
//		Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
//		Nokia5110_PrintBMP(X_MIN, Y_MAX, Ground, 0);
//		Nokia5110_DisplayBuffer();
//		buttonPress = 0;
//		
//			if(SW1 == 0){
//		Countdown = 0;
//			Timer2A_Stop();
//			buttonPress = 0;
//			Nokia5110_Clear();
//		Nokia5110_ClearBuffer();
//			Move();
//			Draw();
//			TimerCount = 0;
//		}
//	}
//	
//	if(tSeconds > 3)
//	{
//		TimerCount = 0;
//		tSeconds = 0;
//		Countdown = 0;
//	}
//	
//}	

void Delay100ms(float count){
	unsigned long volatile time;
  while(count>0){
    // time = 727240;  // 0.1sec at 80 MHz
		time = 300000; // decrease to speed up
    while(time){
	  	time--;
    }
    count--;
  }
}