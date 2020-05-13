// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

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
// ******* Required Hardware I/O connections*******************
// PA1, PA0 UART0 connected to PC through USB cable
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Title.h"
#include "Sprites.h"
// #include "ADC.h"
#include "Scoreboard.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void TitleScreen(void);
void GameOver(void);
unsigned long TimerCount;
unsigned long Semaphore;


// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10
// const unsigned char SmallEnemy30PointA[] = {

// enemy ship that starts at the top of the screen (arms/mouth open)
// width=16 x height=10
// const unsigned char SmallEnemy30PointB[] = {

// enemy ship that starts in the middle of the screen (arms together)
// width=16 x height=10
// const unsigned char SmallEnemy20PointA[] = {

// enemy ship that starts in the middle of the screen (arms apart)
// width=16 x height=10
// const unsigned char SmallEnemy20PointB[] = {

	// enemy ship that starts at the bottom of the screen (arms down)
// width=16 x height=10
// const unsigned char SmallEnemy10PointA[] = {
 
// enemy ship that starts at the bottom of the screen (arms up)
// width=16 x height=10
// const unsigned char SmallEnemy10PointB[] = {

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 x height=8
// const unsigned char PlayerShip0[] = {

// small, fast bonus enemy that occasionally speeds across the top of the screen after enough enemies have been killed to make room for it
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=20 x height=7
// const unsigned char SmallEnemyBonus0[] = {

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 x height=5
// const unsigned char Bunker0[] = {

// small shield floating in space to cover the player's ship from enemy fire (moderate generic damage)
// width=18 x height=5
// const unsigned char Bunker1[] = {

// small shield floating in space to cover the player's ship from enemy fire (heavy generic damage)
// width=18 x height=5
// const unsigned char Bunker2[] = {

// blank space used to cover a bunker that is destroyed
// width=18 x height=5
// const unsigned char Bunker3[] = {

// large explosion that can be used upon the demise of the player's ship (first frame)
// width=18 x height=8
// const unsigned char BigExplosion0[] = {

// large explosion that can be used upon the demise of the player's ship (second frame)
// width=18 x height=8
// const unsigned char BigExplosion1[] = {

// small explosion best used for the demise of an enemy
// width=16 x height=10
// const unsigned char SmallExplosion0[] = {


// blank space following the small explosion for the demise of an enemy
// width=16 x height=10
// const unsigned char SmallExplosion1[] = {

// a missile in flight
// includes one blacked out row on the top, bottom, and right of the image to prevent smearing when moved 1 pixel down, up, or left
// width=4 x height=9
// const unsigned char Missile0[] = {

// a missile in flight
// includes one blacked out row on the top, bottom, and left of the image to prevent smearing when moved 1 pixel down, up, or right
// width=4 x height=9
// const unsigned char Missile1[] = {

// blank space to cover a missile after it hits something
// width=4 x height=9
// const unsigned char Missile2[] = {

// a laser burst in flight
// includes one blacked out row on the top and bottom of the image to prevent smearing when moved 1 pixel up or down
// width=2 x height=9
// const unsigned char Laser0[] = {

// blank space to cover a laser after it hits something
// width=2 x height=9
// const unsigned char Laser1[] = {

// *************************** Capture image dimensions out of BMP**********
#define BUNKERW     ((unsigned char)Bunker0[18])
#define BUNKERH     ((unsigned char)Bunker0[22])
#define ENEMY30W    ((unsigned char)SmallEnemy30PointA[18])
#define ENEMY30H    ((unsigned char)SmallEnemy30PointA[22])
#define ENEMY20W    ((unsigned char)SmallEnemy20PointA[18])
#define ENEMY20H    ((unsigned char)SmallEnemy20PointA[22])
#define ENEMY10W    ((unsigned char)SmallEnemy10PointA[18])
#define ENEMY10H    ((unsigned char)SmallEnemy10PointA[22])
#define ENEMYBONUSW ((unsigned char)SmallEnemyBonus0[18])
#define ENEMYBONUSH ((unsigned char)SmallEnemyBonus0[22])
#define LASERW      ((unsigned char)Laser0[18])
#define LASERH      ((unsigned char)Laser0[22])
#define MISSILEW    ((unsigned char)Missile0[18])
#define MISSILEH    ((unsigned char)Missile0[22])
// #define PLAYERW     ((unsigned char)PlayerShip0[18])
// #define PLAYERH     ((unsigned char)PlayerShip0[22])
//	
//#define PLAYERW		 ((unsigned char)Sprite[18])				
//#define PLAYERW		 ((unsigned char)Sprite[22])				



//struct State {
//  unsigned long x;      // x coordinate
//  unsigned long y;      // y coordinate
//  const unsigned char *image[2];				// two pointers to images
//  long life;            // 0=dead, 1=alive
//};         
//typedef struct State STyp;
//STyp Enemy[4];		// [4] for 4 sprites
//void Init(void){ int i;
//  for(i=0;i<4;i++){
//    Enemy[i].x = 20*i;
//    Enemy[i].y = 10;
//    Enemy[i].image[0] = SmallEnemy30PointA;
//    Enemy[i].image[1] = SmallEnemy30PointB;
//    Enemy[i].life = 1;
//   }
//}
//void Move(void){ int i;
//  for(i=0;i<4;i++){
//    if(Enemy[i].x < 72){    
//      Enemy[i].x -= 1; // move to left  // +- to move to the right
//    }else{
//      Enemy[i].life = 0;
//    }
//  }
//}
//unsigned long FrameCount=0;
//void Draw(void){ int i;
//  Nokia5110_ClearBuffer();
//  for(i=0;i<4;i++){
//    if(Enemy[i].life > 0){
//     Nokia5110_PrintBMP(Enemy[i].x, Enemy[i].y, Enemy[i].image[FrameCount], 0);
//    }
//  }
//  Nokia5110_DisplayBuffer();      // draw buffer
//  FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,...
//}

struct State {
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  const unsigned char *image[2];				// pointer to image
  long life;            // 0=dead, 1=alive
};         
typedef struct State STyp;
STyp Player;		
void Init(void){ 
    Player.x = 5;
    Player.y = 0;
    Player.image[0] = Sprite;
    Player.image[1] = Sprite_fall;
// 		Player.image[2] = Sprite_fly;
    Player.life = 1;
}
void Move(void){ int i;
    if(Player.y < 83){    
      Player.y += 1; // move down
			
			
			// if SW2 is pressed
			// Player.y -= 1 // move up
			
    }else{
      Player.life = 0;
    }
}
unsigned long FrameCount=0;
void Draw(void){ int i;
  Nokia5110_ClearBuffer();
    if(Player.life > 0){
			// if SW2 pressed
			// Nokia5110_PrintBMP(Player.x, Player.y, Player.image[2], 0);
			// else: bird falls
     Nokia5110_PrintBMP(Player.x, Player.y, Player.image[1], 0);
    }
  Nokia5110_DisplayBuffer();      // draw buffer
  FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,...
}

int main(void)
	{ 
	
	int AnyLife = 1; int i;
		
	TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  // you cannot use both the Scope and the virtual Nokia (both need UART0)
  Random_Init(1);
  Nokia5110_Init();
  EnableInterrupts(); // virtual Nokia uses UART0 interrupts
	
	TitleScreen();		

  Init();
  Timer2_Init(80000000/30);  //  80000000/30 for 30 Hz  // increase denom to speed up
//  while(AnyLife){
//		
//	Nokia5110_Clear();
//	
//	Nokia5110_PrintBMP(5, 25, Sprite, 0);
//	
//	Nokia5110_DisplayBuffer(); 
//	
//	Nokia5110_SetCursor(0, 4);	// 6 rows (0-5), 12 characters wide
//	Nokia5110_OutString("____________"); 	
//	Nokia5110_SetCursor(0, 5);
//	Nokia5110_OutString("////////////"); 
//		
//    while(Semaphore == 0){};
//    Semaphore = 0; // runs at 30 Hz
//    AnyLife = 0;
//    for(i=0; i<1 ; i++){
//      AnyLife |= Player[i].life;
//			
////		while(Semaphore == 0){};
////    Semaphore = 0; // runs at 30 Hz
////    AnyLife = 0;
////    for(i=0; i<4 ; i++){
////    AnyLife |= Enemy[i].life;
//    }
    Draw();
		
//  }

	// ~~~ Game Over Screen
	 
	Nokia5110_ClearBuffer();
	
	Nokia5110_PrintBMP(15, 17, Game_Over, 0);		// y = 21
		
  Nokia5110_DisplayBuffer();   // draw buffer
	
	 Nokia5110_SetCursor(1, 3);
   Nokia5110_OutString("Score:");
	//	Nokia5110_OutUDec(Score);
	 Nokia5110_SetCursor(1, 5);
   Nokia5110_OutString("Best:"); 
  //	Nokia5110_OutUDec(bestScore);	
	
  Nokia5110_SetCursor(0, 0); // renders screen
	
	
  while(1){
		
		Move();
		Draw();
		
	//~~~ During Game

  Nokia5110_Clear();
	
	// Nokia5110_PrintBMP(5, 25, Sprite, 0);
	
	Nokia5110_DisplayBuffer(); 
	
	Nokia5110_SetCursor(0, 4);	// 6 rows (0-5), 12 characters wide
	Nokia5110_OutString("____________"); 	
	Nokia5110_SetCursor(0, 5);
	Nokia5110_OutString("//Score:1 ||"); 
		
	Delay100ms(1);              // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim
		
    }			
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
}
void Delay100ms(unsigned long count){unsigned long volatile time;
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
		
  Delay100ms(5);              // delay 5 sec at 80 MHz //c 50 to 1 to speed up sim
//		while(!FIRE){};							
	Delay100ms(5);
	
}

void GameOver(void){
	
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(15, 17, Game_Over, 0);		// y = 21
	Nokia5110_DisplayBuffer();   // draw buffer
	
	Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("Score:");
	//	Nokia5110_OutUDec(Score);
	 Nokia5110_SetCursor(1, 5);
   Nokia5110_OutString("Best:"); 
  //	Nokia5110_OutUDec(bestScore);	
	
  Nokia5110_SetCursor(0, 0); // renders screen
	
	Delay100ms(5);	
	// while(!FIRE){};
	Delay100ms(5);
}


