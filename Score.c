
int score = 0; 
int sDigits = 1;  // sDigits = number of digits in score

int best_score = 0; 
int bDigits = 1; // bDigits = number of digits in best_score

//*************** currentScore *************** 
// number of pipes that have been passed


//*************** bestScore *************** 
// best score so far

if (score > best_score)
{
	best_score = score;
}
if (bDigits == 1 && best_score > 9)
{ 
	bDigits++;
}
else if(bDigits == 2 && best_score > 99)
{
  bDigits++;
	score = 0;
	sDigits = 1;
}	
	


//*************** Leaderboards *************** 
