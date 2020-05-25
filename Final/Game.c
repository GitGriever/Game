// #include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "..//tm4c123gh6pm.h"

#include "Game.h"
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
