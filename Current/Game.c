#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Images.h"
// #include "ADC.h"
#include "Switches.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
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
void SysTick_Init(unsigned long period);
void Delay100ms(float count); // Delay for count*100ms (excluding interrupts)
void TitleScreen(void);
void PauseMenu(void);
void GameOver(void);
void PortFunction_Init(void);
void Score_Init(void); // Initializes player's score
void Player_Crash(void);
void Crash_Check(void);
void Dup(void);

unsigned long TimerCount;
unsigned long Semaphore;
unsigned long score = 0; 
unsigned long best = 0;
unsigned long d = 0;  // offset for bottom collision check
unsigned long c = 0; 	// timer counter
unsigned long buttonPress = 0;
unsigned long seed = 0;

unsigned char pipeCleared;



void Timer1A_Handler(void){ // timer 2A in full
// Generate pipes from the right
	TIMER1_ICR_R = 0x00000001;	
	while(SW2 != 0)
	{
	seed++;
	}
	// if(c>7) { c = 0; } // restart timer

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
	
				if(buttonPress > 2)
				{ buttonPress = 0; }
	
	// checks for collisions
		if ( (Player.x >= (Pipe.x - 9)) && ((Player.x) <= Pipe.x) ) 
		{			// isnt detected if flew up between range :(
				if (Player.y < (Pipe.yTop + 8))
				{
						Player.life = 0;
						GameOver();
				}	
				if (Player.y > (Pipe.yTop + d)) // 18
				{
						Player.life = 0;
					  GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
						GameOver();
				}	
			}	
				
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
				buttonPress = 0;
				// for cyan: green, blue on
				 GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
					GPIO_PORTF_DATA_R |= 0x08;	// Turn off green LED.
			    GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
				
					Player.y -= 3;   // move up
					
			}
			else if (SW1 == 0)// && (SW2 != 0))
			{
				//buttonPress++;
				//Delay100ms(0.0000000000000000000000000000000000117); // helps with switch debounce
				Delay100ms(.1);
				buttonPress = 1;
								
				if(SW1 == 0)
				{ 
					PauseMenu();
				}
				
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
					GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
				GPIO_PORTF_DATA_R |= 0x08; // Turn on green LED.
				
				if(buttonPress == 1)
				{
				PauseMenu();
				}
				
				if(buttonPress == 2)
				{
					Delay100ms(3.5);
					TitleScreen();
				}
			}
			else 
			{
				
			   GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				 // GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
				 GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
				
				 Player.y += 0; // move down 1  // 0 to freeze bird
			}

			 if(GPIO_PORTF_RIS_R&0x01)
	{
		GPIO_PORTF_ICR_R = 0x01;
					GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
					GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
				
			    GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
				
					Player.y -= 3;   // move up
					
			}
			else  if(GPIO_PORTF_RIS_R&0x10)
	{
		GPIO_PORTF_ICR_R = 0x10;
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				
				// Turn on yellow LED:
			    GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED.
					GPIO_PORTF_DATA_R |= 0x08; // Turn on green LED.

				
			// make it pause game and turn on orange or yellow LED
				
			}
			else 
			{
				
			   GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
				 GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
				 GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
				
				 Player.y += 1; // move down 1
			}
			
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
					if(Pipe.x  ==  70) 
					{
						Pipe.var = ((Random()>>24)%15)+1;  // returns random num in range 1 to 15
					// Pipe.var = (rand() % 15)+1;
						// Pipe.var = (10*rand() /16);
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

// Initialize SysTick interrupts  // for random
void SysTick_Init(unsigned long period){
	NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;     // reload value
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF; // priority 0               
  NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
  EnableInterrupts();
}

void SysTick_Handler(void){
//	srand(NVIC_ST_CURRENT_R);
}

int main(void)
	{ 

		
	TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  //Random_Init(NVIC_ST_CTRL_R);  //  from systick
	// Random_Init(1); // same random order
	// srand(NVIC_ST_CTRL_R);	
	// srand(c);
	// srand(buttonPress);	
	
  Nokia5110_Init();
  EnableInterrupts(); // virtual Nokia uses UART0 interrupts
  PortF_Init();	
	// Interrupt_Init();
	TitleScreen();	
	Random_Init(seed);
  Init();
	Obstacle_Init();
		

  Timer2_Init(80000000/30);  //  80000000/30 for 30 Hz  // increase denom to speed up		 // 10 // 12 // max = 4294967295)
//		Timer2_Init(16000000);
  while(1){
		 Draw();		
		 Move();
		Score_Init();
		
		  Delay100ms(1);    // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim // time = 300000;
			// increase to slow down speed
	}
}			
		
void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
	if(Player.life > 0)
	{c++;}
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

void Delay100ms(float count){
	unsigned long volatile time;
  while(count>0){
    // time = 727240;  // 0.1sec at 80 MHz
		time = 300000; // decrease to speed up // flickers a lot when score < 10
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
	while(SW2 != 0){seed++;};					 // displays until SW2 is pressed		
		//Delay100ms(5);
	
}

void PauseMenu(void){
if(buttonPress == 1)
{
  Nokia5110_ClearBuffer();
		
	Nokia5110_PrintBMP(23, 16, Paused, 0);  // 18
	Nokia5110_PrintBMP(17, 28, Continue, 0);			
	
  Nokia5110_PrintBMP(10, 44, No, 0);				// 10, 47
	Nokia5110_PrintBMP(52, 44, Yes, 0);			// 52, 47
		
  Nokia5110_DisplayBuffer();   // draw buffer
	
	// Nokia5110_SetCursor(8, 3);
	// Nokia5110_OutUDec(buttonPress);
	
		if(SW1 == 0)
		{
			Delay100ms(0.1); 
			if(SW1 == 0){
			TitleScreen();
			}
		}
		
		if(SW2 == 0)
{ buttonPress = 0; }

	while((SW1!= 0) && (SW2 != 0)){}
}	
}

void GameOver(void){
	GPIO_PORTF_DATA_R &= 0x02; // Turn on red LED
	
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
	Delay100ms(1);
	if ((SW2 == 0) && (SW1 != 0)) 
	{
		score=0;
		
		main();
	}
	
}

void Score_Init(void){
	
				// best = 0;
			
				if (Pipe.x == Player.x) 
				{
					GPIO_PORTF_DATA_R |= 0x08; // turn on green led
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
					score = score + 1;
				}
	
		if (score > best){
		best = score;
			// display rainbow led?
			}
		
 if(Player.life > 0){  // ingame score
				if(score < 10)
				{
			Nokia5110_SetCursor(11, 0);
			Nokia5110_OutUDec(score);
					
			//		Nokia5110_SetCursor(10, 0); // make sure to comment out if not used to remove flickering
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
			}
					else if(Player.life == 0)
				{ GPIO_PORTF_DATA_R |= 0x02; // turn on red led
					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.	
				}  
				
	}

void Dup(void)
{ 
	main();
}

	

