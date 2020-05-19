// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano

// This virtual Nokia project only runs on the real board, this project cannot be simulated
// Instead of having a real Nokia, this driver sends Nokia
//   commands out the UART to TExaSdisplay
// The Nokia5110 is 48x84 black and white
// pixel LCD to display text, images, or other information.

// April 19, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Images.h"
// #include "ADC.h"
#include "Switches.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "..//tm4c123gh6pm.h"

#define Y_MIN 2 // Minimum screen y-coord (top)
#define Y_MAX 47 // Maximum screen y-coord (bottom)
#define X_MIN 0 // Minimum screen x-coord (left)
#define X_MAX 82 // Maximum screen x-coord (right)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // Delay for count*100ms (excluding interrupts)
void TitleScreen(void);
void GameOver(void);
void PortFunction_Init(void);
void Score_Init(void); // Initializes player's score
void Player_Crash(void);
void Crash_Check(void);

unsigned long TimerCount;
unsigned long Semaphore;
unsigned long score = 0; 
unsigned long best = 0;
unsigned long d = 0;  // offset for bottom collision check
unsigned long c = 0; 	// timer counter

unsigned char pipeCleared;



void Timer1A_Handler(void){ // timer 2A in full
// Generate pipes from the right
	TIMER2_ICR_R = 0x00000001;	
	c++;
	
	if(c>7) { c = 0; } // restart timer

}

void Timer1_Init(unsigned long seconds){ 
// Initalise Timer2 to trigger every few seconds(assuming 880Mhz PLL)
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;
  delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R = 0x00000000;
  TIMER2_CFG_R = 0x00000000; // 32-bit mode
  TIMER2_TAMR_R = 0x00000002; // periodic mode, default down-count
  TIMER2_TAILR_R = seconds * 80000000 - 1; 
  TIMER2_TAPR_R = 0; // bus clock resolution
  TIMER2_ICR_R = 0x00000001;
  TIMER2_IMR_R = 0x00000001;
  NVIC_PRI5_R = (NVIC_PRI5_R&0x0FFFFFFF)|(0x03 << 29); // priority 3
	NVIC_EN0_R |= 1<<23;
	TIMER2_CTL_R = 0x00000001; 
}

struct State {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  const unsigned char *image[3];				// pointer to image
  long life;            // 0=dead, 1=
};         

typedef struct State STyp;
STyp Player;		
void Init(void){  // 8x12, xrange is [0. ?]. yrange is [height-5, ?]
    Player.x = 0; // 5 , begins with leftmost pixel of the bmp, leftmost is x = 0
    Player.y = 23;		// 23, highest point is y = 7
    Player.image[0] = Sprite;
    Player.image[1] = Sprite_fall;
    Player.image[2] = Sprite_fly;
    Player.life = 1;
}

typedef struct Obstacle {
  unsigned long x;      // x coordinate
  unsigned long yTop;      // yTop coordinate
  unsigned long yBot;      // yBot coordinate
  const unsigned char *top;
	const unsigned char *bottom;
	unsigned char	var;  // pipe variant
} OTyp;

OTyp Pipe;  

void Obstacle_Init(void){ 
		unsigned char	var;  // pipe variant
    Pipe.x = 70;  // [0, 70]
		Pipe.yBot = Y_MAX;
//    Pipe.yTop = 9;  // PipeY --> Pipe.yTop = Y - 1
			
}

void Crash_Check(void)
{
	if ((Player.x == Pipe.x) && (Player.y == Pipe.x))
	{
		Player.life = 0;
		GameOver();
	}
}
	

void Move(void){
	
//	 if (((Player.x-1) == Pipe.x) && (Player.y < Pipe.yTop + 8))
			// Player.x == (Pipe.x - 9
			// Pipe10, rightedge, ~14x10, Pipe.yTop - 2 = 9 -2 = 7
			// 8x12 player, highest point is player.y = 7 @(0,7)
		// x begins with leftmost pixel of the bmp, leftmost is x = 0

	// checks for collisions
//		if ( (Player.x >= (Pipe.x - 9)) && ((Player.x) <= Pipe.x) ) 
//		{			// isnt detected if flew up between range :(
//				if (Player.y < (Pipe.yTop + 8))
//				{
//						Player.life = 0;
//						GameOver();
//				}	
//				if (Player.y > (Pipe.yTop + d)) // 18
//				{
//						Player.life = 0;
//					  GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
//						GameOver();
//				}	
//			}	
				
	
    if((Player.y < Y_MAX) && (Player.y >= Y_MIN)){ 
			if(Pipe.x < X_MAX) {    
			Pipe.x  -= 1; // move left
				
				
			}
			else
			{
				Pipe.x = 70;
			}
		
			if ((SW2 == 0) && (SW1 != 0))  //SW2 is pressed
			{
//					GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
//					GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
//			    GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
				
				// for cyan: green, blue on
				 GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
					GPIO_PORTF_DATA_R |= 0x08;	// Turn off green LED.
			    GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
				
					Player.y -= 3;   // move up
					
			}
			else if ((SW1 == 0) && (SW2 != 0)) // SW1 is pressed to pause the game
			{
				while(SW2 != 0) // game remains paused as long as the user does not press SW2
				{
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				
					// Turn on yellow LED:
			    		GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
					GPIO_PORTF_DATA_R |= 0x08; // Turn on green LED.
					Player.y -= 0;
					Pipe.x  -= 0;
				}

				
			// make it pause game and turn on orange or yellow LED
				
			}
			else 
			{
				
			   GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				 // GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
				 GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
				
				 Player.y += 0; // move down 1  // 0 to freeze bird
			}

//			 if(GPIO_PORTF_RIS_R&0x01)
//	{
//		GPIO_PORTF_ICR_R = 0x01;
//					GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
//					GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
//				
//			    GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
//				
//					Player.y -= 3;   // move up
//					
//			}
//			else  if(GPIO_PORTF_RIS_R&0x10)
//	{
//		GPIO_PORTF_ICR_R = 0x10;
//					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
//				
//				// Turn on yellow LED:
//			    GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
//					GPIO_PORTF_DATA_R |= 0x08; // Turn on green LED.

//				
//			// make it pause game and turn on orange or yellow LED
//				
//			}
//			else 
//			{
//				
//			   GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
//				 GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
//				 GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
//				
//				 Player.y += 1; // move down 1
//			}
			
    }
		else{
      Player.life = 0;
      GameOver();
    }
		
}

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


unsigned long FrameCount=0;
void Draw(void){
  Nokia5110_ClearBuffer();

    if(Player.life > 0){			
			Nokia5110_PrintBMP(X_MIN, Y_MAX, Ground, 0);
		//	c = c%35;
		//	if(Pipe.x == 66)
					if(Pipe.x  ==  70)
					{
						Pipe.var = ((Random()>>24)%15)+1;  // returns random num in range 1 to 15
						// Pipe.var = 5;
						
						if((Pipe.var == 1) || Pipe.var == 2) 
						{
							d = (2*Pipe.var) + 16;
						}
						else if(Pipe.var == 15)
						{
							d = 22;
						}
						else
						{
							d = 21;
						}
						
						switch(Pipe.var)
									{
										
													case 1:  // 7bit diff b/t top and bottom
															Pipe.top = Pipe0;
															Pipe.yTop = 0;
															Pipe.bottom = Pipe29b;
															// d = 18;
															break;
													
													case 2: 
															Pipe.top = Pipe0;
															Pipe.yTop = 0;
													    Pipe.bottom = Pipe27b;
															// d = 20;
															break;
													
													case 3: 
															Pipe.top = Pipe2;
															Pipe.yTop = 1;
													    Pipe.bottom = Pipe25b;
															// d = 21;
															break;
													
													case 4: 
															Pipe.top = Pipe4;
															Pipe.yTop = 3;
													    Pipe.bottom = Pipe23b;
															// d = 21;
															break;
													
													case 5: 
															Pipe.top = Pipe6;
															Pipe.yTop = 5;
													    Pipe.bottom = Pipe21b;
															// d = 21;
															break;
													
													case 6: 
															Pipe.top = Pipe8;
															Pipe.yTop = 7;
													    Pipe.bottom = Pipe19b;
															// d = 21;
															break;
													
													case 7: 
															Pipe.top = Pipe10;
															Pipe.yTop = 9;
													    Pipe.bottom = Pipe17b;
															// d = 21;
															break;
									
														case 8:
															Pipe.top = Pipe12;
															Pipe.yTop = 11;
													    Pipe.bottom = Pipe15b;
															// d = 21;
															break;
														
														case 9:
															Pipe.top = Pipe14;
															Pipe.yTop = 13;
													    Pipe.bottom = Pipe13b;
															// d = 21;
															break;
														
														case 10:
															Pipe.top = Pipe16;
															Pipe.yTop = 15;
													    Pipe.bottom = Pipe11b;
															// d = 21;
															break;
														
														case 11:
															Pipe.top = Pipe18;
															Pipe.yTop = 17;
													    Pipe.bottom = Pipe9b;
															// d = 21;
															break;
														
														case 12:
															Pipe.top = Pipe20;
															Pipe.yTop = 19;
													    Pipe.bottom = Pipe7b;
															// d = 21;
															break;
														
														case 13:
															Pipe.top = Pipe22;
															Pipe.yTop = 21;
													    Pipe.bottom = Pipe5b;
															// d = 21;
															break;
														
														case 14:
															Pipe.top = Pipe24;
															Pipe.yTop = 23;
													    Pipe.bottom = Pipe3b;
															break;
														
														case 15:
															Pipe.top = Pipe25;
															Pipe.yTop = 24;
															Pipe.bottom = Pipe0b;
															// d = 22;
															break;
														}
													// edge of Player.y  = -bot + 47
														
								
													}

			Nokia5110_PrintBMP(Pipe.x, Pipe.yTop, Pipe.top, 0);
			Nokia5110_PrintBMP(Pipe.x, Pipe.yBot, Pipe.bottom, 0);
			
						if(SW2 == 0) // SW2 is pressed
			{
					Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
			}
			// else if (SW1 pressed) then pause game
			else 
			{
//				 Nokia5110_PrintBMP(Player.x, Player.y, Player.image[1], 0);
				Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
			}
    }
  Nokia5110_DisplayBuffer();      // draw buffer
	
}

// PF4 (0x01) is input SW1 and PF2 (0x04) is output Blue LED
void PortF_Init(void){ 
	
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

	
	/*
	
  // IntEnable(INT_GPIOF);  							// enable interrupt 30 in NVIC (GPIOF)
	// IntPrioritySet(INT_GPIOF, 0x00); 		// configure GPIOF interrupt priority as 0
	
	NVIC_EN0_R |= 0x40000000;  		// enable interrupt 30 in NVIC (GPIOF)
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; 		// configure GPIOF interrupt priority as 2
	// NVIC_PRI7_R &= ~0x00E00000; // priority 0
	GPIO_PORTF_IM_R |= 0x11;   		// arm interrupt on PF0 and PF4
	GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
  // GPIO_PORTF_IBE_R |= 0x11;   	// PF0 and PF4 both edges trigger 
	GPIO_PORTF_IBE_R &= ~0x11;   	// PF0 and PF4 not both edges trigger 
  GPIO_PORTF_IEV_R &= ~0x11;  	// PF0 and PF4 falling edge event
	
  setting bits 23-21 in NVIC_PRI7
	0x00A00000; for 5
	0x00400000 for priority 2
	NVIC_PRI7_R &= ~0x00E00000; // priority 0
	*/
	
}



void Timer0A_Init(unsigned char second)
{
	volatile uint32_t ui32Loop;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; // activate timer0
  ui32Loop = SYSCTL_RCGC1_R;				// Do a dummy read to insert a few cycles after enabling the peripheral.
  TIMER0_CTL_R &= ~0x00000001;     // disable timer0A during setup
  TIMER0_CFG_R = 0x00000000;       // configure for 32-bit timer mode
  TIMER0_TAMR_R = 0x00000002;      // configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = (second*16000000)-1;       // reload value
	NVIC_PRI4_R &= ~0xE0000000; 	 // configure Timer0A interrupt priority as 0
  NVIC_EN0_R |= 0x00080000;     // enable interrupt 19 in NVIC (Timer0A)
	TIMER0_IMR_R |= 0x00000001;      // arm timeout interrupt
  TIMER0_CTL_R |= 0x00000001;      // enable timer0A																	// configure GPIOF interrupt priority as 0 
}

void Timer0A_Handler(void)
{
	  // acknowledge flag for Timer0A
	  TIMER0_ICR_R |= 0X00000001;
	
    // Update the periodic interrupt counter.
	  c++;
}	


//void Interrupt_Init(void)
//{
//  IntEnable(INT_GPIOF);  							// enable interrupt 30 in NVIC (GPIOF)
//	IntPrioritySet(INT_GPIOF, 0x00); 		// configure GPIOF interrupt priority as 0
//	GPIO_PORTF_IM_R |= 0x11;   		// arm interrupt on PF0 and PF4
//	GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
//  GPIO_PORTF_IBE_R &= ~0x11;   	// PF0 and PF4 not both edges trigger 
//  GPIO_PORTF_IEV_R &= ~0x11;  	// PF0 and PF4 falling edge event
//}

//**********************GameEngine_Init***********************
// Calls the initialization of the SysTick and give
// initial values to some of the game engine variables
// It needs to be called by the main function of the program
// inputs: none
// outputs: none

int main(void)
	{ 
		
	Player.life = 1;
	score = 0;	
		
	TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  Random_Init(1);
  Nokia5110_Init();
  EnableInterrupts(); // virtual Nokia uses UART0 interrupts
		
  PortF_Init();	
		
	// Interrupt_Init();
		
	TitleScreen();		

  Init();
	Obstacle_Init();
		
//			 //initialize Timer0 and configure the interrupt
//	 Timer0A_Init(1);	// increments counter every 1 second
	
	// Crash_Check();
		
  Timer2_Init(80000000/10);  //  80000000/30 for 30 Hz  // increase denom to speed up		
//		Timer2_Init(16000000);
  while(1){

		Draw();		
		Move();
		
		Score_Init();
		
//		Player_Crash();
		
//		Nokia5110_Clear();
//	
//		Nokia5110_PrintBMP(0, 82, Ground, 0);
//	
//		Nokia5110_DisplayBuffer(); 

		Delay100ms(1);    // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim
		
	}
		
		
	

		
	//~~~ During Game

//  Nokia5110_Clear();
//	
//	// Nokia5110_PrintBMP(5, 25, Sprite, 0);
//	
//	Nokia5110_DisplayBuffer(); 
//	
//	Nokia5110_SetCursor(0, 4);	// 6 rows (0-5), 12 characters wide
//	Nokia5110_OutString("____________"); 	
//Nokia5110_SetCursor(0, 5);
//Nokia5110_OutUDec(c);
//Nokia5110_SetCursor(0, 0); // renders screen
		

		
}			
		




// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
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
  TimerCount++;
  Move(); 
  Semaphore = 1; // trigger
	if(Player.life > 0)
	{c++;}
}

void Delay100ms(unsigned long count){
	unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void TitleScreen(void){
	
  Nokia5110_ClearBuffer();
		
	Nokia5110_PrintBMP(16, 22, Birdy, 0);
	Nokia5110_PrintBMP(48, 22, Flap, 0);
  Nokia5110_PrintBMP(27, 40, Start, 0);
		
  Nokia5110_DisplayBuffer();   // draw buffer
		
  // Delay100ms(5);              // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim
	while(SW2 != 0){};					 // displays until SW2 is pressed		
		//Delay100ms(5);
	
}

void GameOver(void){
	
	Player.life = 0;
	
	if(Player.life == 0)
	{
	GPIO_PORTF_DATA_R &= 0x02; // Turn on red LED
	}
	
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(15, 17, Game_Over, 0);		// y = 21
	Nokia5110_DisplayBuffer();   // draw buffer
	
	Nokia5110_SetCursor(8, 3);
	Nokia5110_OutUDec(score);
	Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("Score:");
	
	if(best < 10)
				{
				Nokia5110_SetCursor(8, 5);
				Nokia5110_OutUDec(best);
				Nokia5110_SetCursor(0, 0); // renders screen
				}
				if((best < 100) && (best >= 10) )
				{
				Nokia5110_SetCursor(7, 5);
				Nokia5110_OutUDec(best);
				Nokia5110_SetCursor(0, 0); // renders screen
				}
		
				
//	Nokia5110_SetCursor(7, 5);
//  Nokia5110_OutUDec(best); // check c	
	Nokia5110_SetCursor(1, 5);
  Nokia5110_OutString("Best:"); 

  Nokia5110_SetCursor(0, 0); // renders screen
	
//			if ((SW2 == 0) && (SW1 != 0)) 
//			{
//			main();
//			}
			
  // Delay100ms(10);
}

void Score_Init(void){
	
				// best = 0;
				score = 0;

		// score = ((37+c) / 37); // 35
	score = c/37;
				// score = Pipe.var; or c for check
	
		if (score > best){
		best = score;
			// display rainbow led?
			}
		
 if(Player.life > 0){  // ingame score
				if(score < 10)
				{
			Nokia5110_SetCursor(11, 0);
			Nokia5110_OutUDec(score);
					
					Nokia5110_SetCursor(10, 0);
			//	Nokia5110_OutUDec(c);
					
				Nokia5110_SetCursor(0, 0); // renders screen
				}
				if((score < 100) && (score >= 10) )
				{
				Nokia5110_SetCursor(10, 0);
				// Nokia5110_OutUDec(c);
				Nokia5110_OutUDec(score);
				Nokia5110_SetCursor(0, 0); // renders screen
				}
				
				if (Pipe.x == Player.x) 
				{
					GPIO_PORTF_DATA_R |= 0x08; // turn on green led
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				}
					
				
			}
					else if(Player.life == 0)
				{ GPIO_PORTF_DATA_R |= 0x02; // turn on red led
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.	
				}  
	}


char Screen[X_MAX*Y_MAX/8]; // buffer stores the next image to be printed on the screen

//********Nokia5110_AskPixel*****************
// Evaluates if the pixel searched is on or off
// inputs: x - horizontal coordinate of the pixel, must be less than 84
//         y - vertical coordinate of the pixel, must be less than 48
// outputs: true if the pixel is setted already, false if the pixel is cleared
bool Nokia5110_AskPixel(unsigned char x, unsigned char y) 
{
  unsigned short PixelByte;            // byte# in screen buffer
  unsigned char PixelBit;              // bit# in byte
	unsigned char Result;
  if ((x<84) && (y<48)) 
	{              // check screen boundaries
    PixelByte = ((y/8)*84) + x;
    PixelBit = y % 8;
    Result = Screen[PixelByte]&(1U<<PixelBit);
  }
	return (bool)Result;
}
 
//**********************Enemy_ControlDeath***********************
// This function controls the death of an enemy pointed by "this".
// It determines when the enemy should die based on the pixels that 
// are already turned on
// inputs: enemy: Pointer to an element of the enemy array
// outputs: 1: Enemy was killed, 0: Enemy was not killed
void Player_Crash(void)
{
	unsigned char i = Player.y;
	while (Player.life == 1)	//i corresponds to the coordinates in Y axis from the bottom of
	{																												//the enemy image to its top
		if (Nokia5110_AskPixel(Player.x - 2, i) && (Pipe.x < Player.x - 4))		//If a pixel is turned on 2 pixels before the enemy image
		{																											//the enemy is killed
			Player.life = 0;
			GameOver();
			// Sound_GameOver();
		}
		i--;
	}
}

	

