 #ifndef GAME_H
#define GAME_H

#define Y_MIN 2 // Minimum screen y-coord (top)
#define Y_MAX 47 // Maximum screen y-coord (bottom)
#define X_MIN 0 // Minimum screen x-coord (left)
#define X_MAX 82 // Maximum screen x-coord (right)

#include "Nokia5110.h"

extern unsigned long Countdown;
extern unsigned long TimerCount;
extern unsigned long tSeconds;
extern unsigned long buttonPress;
extern unsigned long score; 
extern unsigned long best;
extern unsigned long d;  // offset for bottom collision check
extern unsigned long seed;

#endif
