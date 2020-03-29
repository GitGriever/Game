void HUD(unsigned long intCounter, unsigned short timerSpecialAttack, unsigned short timetoSpecial)
{
	unsigned char i;
	for (i = 0; i < SCREENW; i++)		 // Draws a line at underneath to separate the HUD from the playing area
	{
		LCD_SetPixel(i, SCREENH - 43); // need to adjust slide pot to be between upper HUD and ground
	}
	LCD_OutStr(0, SCREENH - 48, "Score:"); 
	LCD_OutUDec(30, SCREENH - 48, player.score);
	LCD_OutStr(50, SCREENH - 48, "Best:"); 
	LCD_OutUDec(65, SCREENH - 48, player.bestScore);

	LCD_OutStr(0, SCREENH - 7, "----------------"); 	// separates the ground
	LCD_OutStr(0, SCREENH - 4, "////////////////"); 	// screen only displays 16 characters

	// Nokia is a 48x84 LCD
	// Range of SCREENW is 0(left)-82(right)
	// Range of SCREENH is 2(top)-47(bottom)
}