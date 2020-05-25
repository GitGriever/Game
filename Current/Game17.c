#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"

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

#include "Game.h"
#include "Sprites.h"
#include "Screens.h"
#include "Switches.h"
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
unsigned long buttonPress = 0;
unsigned long score = 0; 
unsigned long best = 0;
unsigned long d = 0;  // offset for bottom collision check
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
void Player_Init(void)  // 8x12, xrange is [0. ?]. yrange is [height-5, ?]
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
			Cyan_on();
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

int main(void)
	{ 

	TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  Nokia5110_Init();
  EnableInterrupts(); // virtual Nokia uses UART0 interrupts
	LED_Init();
	Switch_Init();
	TitleScreen();	
	Random_Init(seed);  // 1 for same random order
  Player_Init();
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

void TitleScreen(void){
	
	Countdown = 1;

	score = 0;
	
  Nokia5110_ClearBuffer();
		
	Nokia5110_PrintBMP(16, 22, Birdy, 0);
	Nokia5110_PrintBMP(48, 22, Flap, 0);
  Nokia5110_PrintBMP(27, 40, Start, 0);
		
  Nokia5110_DisplayBuffer();   // draw buffer

	while(SW2 != 0){seed++;
		Rainbow();
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
	Yellow_on();
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
		score=0;
		main();
	}

	while(SW2 !=0){
		Red_on();
		}
	
}

void Score_Init(void){

if(Player.life != 0)
			{		
				if (Pipe.x == Player.x) 
				{
					Green_on();
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
