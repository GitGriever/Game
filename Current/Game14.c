#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Images.h"
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

// #include "bit.h"
#include "LED.h"
#include "Timer.h"

#define Y_MIN 2 // Minimum screen y-coord (top)
#define Y_MAX 47 // Maximum screen y-coord (bottom)
#define X_MIN 0 // Minimum screen x-coord (left)
#define X_MAX 82 // Maximum screen x-coord (right)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(float count); // Delay for count*100ms (excluding interrupts)
void TitleScreen(void);
void PauseMenu(void);
void GameOver(void);
void PortFunction_Init(void);
void LED_Init(void);
void Switch_Init(void);
void Score_Init(void); // Initializes player's score
void Ready(void);
void Timer2A_Start(void);
void Timer2A_Stop(void);

unsigned long Countdown;
unsigned long TimerCount;
unsigned long tSeconds;
unsigned long score = 0; 
unsigned long best = 0;
unsigned long d = 0;  // offset for bottom collision check
unsigned long buttonPress = 0;
unsigned long seed = 0;
// unsigned int s = 1;

struct State 
	{
		unsigned long x;      // x coordinate
		unsigned long y;      // y coordinate
		const unsigned char *image[3];				// pointer to image
		long life;            // 0 = dead, 1 = alive
  };         

typedef struct State STyp;
STyp Player;		
void Init(void)  // 8x12, xrange is [0. ?]. yrange is [height-5, ?]
  {
    Player.x = 0; // 5 , begins with leftmost pixel of the bmp, leftmost is x = 0
    Player.y = 25;		// 23, highest point is y = 7
    Player.image[0] = Sprite;
    Player.image[1] = Sprite_fall;
    Player.image[2] = Sprite_fly;
    Player.life = 1;
  }

typedef struct Obstacle 
	{
		unsigned long x;      		// x coordinate
		unsigned long yTop;      // yTop coordinate
		unsigned long yBot;     // yBot coordinate
		const unsigned char *top;
		const unsigned char *bottom;
		unsigned char	var;  // pipe variant
  } 	OTyp;

OTyp Pipe;  

void Obstacle_Init(void)
	{ 
		unsigned char	var;  // pipe variant
    Pipe.x = 70;  // [0, 70]
		Pipe.yBot = Y_MAX;
  }

void Move(void)
	{
	
// Collision Detection
// if (( (Player.x +12) > Pipe.x) && ((Player.x + 12) < (Pipe.x + 12) ))
	if (( (Player.x +12) > Pipe.x) && (Player.x < (Pipe.x + 12) ))
		{			
			if ((Player.y - 7) < Pipe.yTop)
				{
							GameOver();
				}	
				if (Player.y > (Pipe.yTop + d)) // 18
				{
							GameOver();
				}	
		 }	
						
if((Player.y < Y_MAX) && (Player.y >= Y_MIN)){ 
			
	if(Pipe.x < X_MAX) {    
			Pipe.x  -= 1; // move left
	}
	else{
			Pipe.x = 70;
	};
	while(Countdown==1){
			if(SW1==0){
					Countdown=0;
			}
	}
	if ((SW2 == 0) && (SW1 != 0)){  //SW2 is pressed
			// for cyan: green, blue on
			Cyan_on();
//			GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
//			GPIO_PORTF_DATA_R |= 0x08;	// Turn off green LED.
//			GPIO_PORTF_DATA_R |= 0x04; // Turn on blue LED.
				
			Player.y -= 3;   // move up
						
	}
	else if (SW1 == 0){ //&& (TIMER2_CTL_R &= ~0x00000001)) // && stops timer but blocked pause

			TIMER2_CTL_R &= ~0x00000001;
			Delay100ms(1);
				
			if(SW1 == 0)
			{ 
				  Delay100ms(1);
					buttonPress = 1;
					PauseMenu();
		  }
					
			if(buttonPress == 1)
			{

					Delay100ms(1);
					PauseMenu();
			}
	}
	else{
		
		LED_off();
				
//			   GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
//				 // GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED.
//				 GPIO_PORTF_DATA_R &= ~0x08;	// Turn off green LED.
				
				 Player.y += 1; // move down 1  // 0 to freeze bird
	}
			
}
else{
    GameOver();
}
}

void Draw(void){
		Nokia5110_ClearBuffer();
		// while(Countdown != 1){}
    if(Player.life > 0){			
				Nokia5110_PrintBMP(X_MIN, Y_MAX, Ground, 0);
				if(Pipe.x  ==  70) {
						while(Countdown != 0){} // needed, important
						Pipe.var = ((Random()>>24)%15)+1;  // returns random num in range 1 to 15
						// Pipe.var = 5;
						
						if((Pipe.var == 1) || (Pipe.var == 2)){
								d = (2*Pipe.var) + 16;
						}
						else if(Pipe.var == 15){
								d = 22;
						}
						else {
								d = 21;
						}
						
						switch(Pipe.var){
										
				    case 1:  
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
			else 
			{
//				 Nokia5110_PrintBMP(Player.x, Player.y, Player.image[1], 0);
				Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
			}
    }
  Nokia5110_DisplayBuffer();      // draw buffer
	
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

void Ready(void){
	tSeconds = 0;
buttonPress = 0;
	
	if(Countdown == 1){
		Nokia5110_Clear();
			Nokia5110_ClearBuffer();
		Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
		Nokia5110_PrintBMP(X_MIN, Y_MAX, Ground, 0);
		Nokia5110_DisplayBuffer();
		buttonPress = 0;
		
			if(SW1 == 0){
		Countdown = 0;
			Timer2A_Stop();
			buttonPress = 0;
			Nokia5110_Clear();
		Nokia5110_ClearBuffer();
			Move();
			Draw();
			TimerCount = 0;
		}
	}
	
	if(tSeconds > 3)
	{
		TimerCount = 0;
		tSeconds = 0;
		Countdown = 0;
	}
	
}	

//	void Timer2A_Start(void)
//{
//	TIMER2_CTL_R |= 0x00000001;
//}

//void Timer2A_Stop(void)
//{
//	TIMER2_CTL_R &= ~0x00000001;
//}

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

//void PortF_Init(void){ 
//	
//	volatile unsigned long delay;

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
//	
//}

void Interrupt_Init(void)
{
	NVIC_EN0_R |= 0x40000000;  		// enable interrupt 30 in NVIC (GPIOF)
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; 		// configure GPIOF interrupt priority as 2
	GPIO_PORTF_IM_R |= 0x11;   		// arm interrupt on PF0 and PF4
	GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;   	// PF0 and PF4 not both edges trigger 
  GPIO_PORTF_IEV_R &= ~0x11;  	// PF0 and PF4 falling edge event
}

int main(void)
	{ 

	TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  Nokia5110_Init();
  EnableInterrupts(); // virtual Nokia uses UART0 interrupts
	LED_Init();
	Switch_Init();
 // PortF_Init();	
//	Interrupt_Init();
	TitleScreen();	
	Random_Init(seed);  // 1 for same random order
  Init();
	Obstacle_Init();
  Timer2_Init(80000000/30);  //  80000000/30 for 30 Hz  // increase denom to speed up		 // 10 // 12 // max = 4294967295)
  while(1){
		Ready();
		 Draw();		
		 Move();
		Score_Init();
		
		  Delay100ms(1);    // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim // time = 300000;
	}
}			


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

//void Delay100ms(float count){
//	unsigned long volatile time;
//  while(count>0){
//    // time = 727240;  // 0.1sec at 80 MHz
//		time = 300000; // decrease to speed up
//    while(time){
//	  	time--;
//    }
//    count--;
//  }
//}

void TitleScreen(void){
	
	Countdown = 1;

	score = 0;
	
  Nokia5110_ClearBuffer();
		
	Nokia5110_PrintBMP(16, 22, Birdy, 0);
	Nokia5110_PrintBMP(48, 22, Flap, 0);
  Nokia5110_PrintBMP(27, 40, Start, 0);
		
  Nokia5110_DisplayBuffer();   // draw buffer
	
	

	// PORTF_DATA = 0;
	while(SW2 != 0){seed++;
		Rainbow();
//		PORTF_DATA = 0;
//		PORTF_DATA = (s << 1);
//		 s = (bit2(~s) >> 2) | (((bit2(~s) >> 1) & bit1(s) | (bit2(s) >> 1) & (bit0(~s) << 1))) | ((bit2(s) & (bit1(~s) << 1) | bit2(~s) & (bit0(s) << 2))); //flash all combinations
//		Delay100ms(3); // 3 red, yellow, green, cyan, blue, pink, white, off
	};					 // displays until SW2 is pressed		
}

void PauseMenu(void){
  Nokia5110_ClearBuffer();
		
	Nokia5110_PrintBMP(23, 16, Paused, 0);  // 18
	Nokia5110_PrintBMP(17, 28, Continue, 0);			
	
  Nokia5110_PrintBMP(10, 44, No, 0);				// 10, 47
	Nokia5110_PrintBMP(52, 44, Yes, 0);			// 52, 47
		
  Nokia5110_DisplayBuffer();   // draw buffer

		if(SW1 == 0)
		{
			Delay100ms(0.01); // 0.1 
			if(SW1 == 0){
				main();
			}
		}

	while((SW1!= 0) && (SW2 != 0)){
	// yellow
	Yellow_on();
//	GPIO_PORTF_DATA_R |= 0x02; // turn on red led
//	GPIO_PORTF_DATA_R &= ~0x04; // turn off  blue led
//	GPIO_PORTF_DATA_R |= 0x08; // turn on green led
	}
}

void GameOver(void){
	Player.life = 0;
	
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(14, 17, Game_Over, 0);		// y = 21
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
				Nokia5110_SetCursor(8, 5);
				Nokia5110_OutUDec(best);
				Nokia5110_SetCursor(0, 0); // renders screen
				}

	Nokia5110_SetCursor(1, 5);
  Nokia5110_OutString("Best:"); 

  Nokia5110_SetCursor(0, 0); // renders screen

	if ((SW2 == 0) && (SW1 != 0)) 
	{
		Delay100ms(0.1);
		Red_off();
		// GPIO_PORTF_DATA_R &= ~0x02; // turn off red led
		score=0;
		main();
	}

	while(SW2 !=0){
		Red_on();
		// GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED
		}
	
}

void Score_Init(void){

if(Player.life != 0)
			{		
				if (Pipe.x == Player.x) 
				{
					Green_on();
			//		Cyan_off();
//					GPIO_PORTF_DATA_R |= 0x08; // turn on green led
//					GPIO_PORTF_DATA_R &= ~0x04; // Turn off blue LED.
					score = score + 1;
				}
			}
	
		if (score > best){
		best = score;
			}
		
 if(Player.life > 0){  // ingame score
				if(score < 10)
				{
			Nokia5110_SetCursor(11, 0);
			Nokia5110_OutUDec(score);
					
				Nokia5110_SetCursor(0, 0); // renders screen
				}
				if((score < 100) && (score >= 10) )
				{
				 Nokia5110_SetCursor(10, 0);
				Nokia5110_OutUDec(score);
				Nokia5110_SetCursor(0, 0); // renders screen
				}	
			}
				
	}
