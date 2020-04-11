

//*************** Intro Screen *************** 
// display game title
// Press <SW2> to start game
// Press <SW1> for help screen

//*************** Help Screen *************** 
// display game rules
// press <SW1> again to go back to intro screen

//*************** Game Over Screen *************** 
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("<SW2> to try again");
  Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("<SW1> to quit,");
  Nokia5110_SetCursor(2, 4);
  Nokia5110_OutUDec(1234);
  Nokia5110_SetCursor(0, 0); // renders screen

