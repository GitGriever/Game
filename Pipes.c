#include "LCD_Nokia.h"

//------------------- Global Constants -------------------

// test values for now

const int PIPE_W = 3; // width of each pipe

const int GAP_W = 7; // gap of each pipe

//------------------- Global Variables -------------------

Pipe_v p1, p2; // pair of vertical pipes

//*************** Pipe_v *************** 
// represents a vertical piple
typedef struct Pipe_v
{
	float Pipe_gap // a fraction of the height of the screen
	int center; // center of the gap
} Pipe_v;

void Pipe_new(vpipe *p)
{
	// New pipe shows up when a pipe leaves on the left of the screen
	if (p->center + PIPE_W < 0)
	{
		p->center =  

