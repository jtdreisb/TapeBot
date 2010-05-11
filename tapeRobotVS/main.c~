

#include "../XiphosLibrary/globals.h"



int main()
{
	initialize();
	cli();
	
	//Test 1
	clearScreen();
	printString("    TapeBot     ");
	lowerLine();

	while(1);
	
	
	//Turn motors on and wait until not against wall to turn on interrupt
	sei();
}

ISR(SOMEBUTTON_VECTOR) {	
	//if 2 bump sensors are activated turn right
	//count the walls that the bot has run into
	turnRight();
	//numWalls++
	//when it reaches 4 walls dump the balls then find inner circle
	//dump();
	//otherwise correct the trajectory of the bot
	
}

int findInnerSquare() {
	//back up past two lines
	
	//turn 90 degrees right
	
	//forward past two lines
	
	//90 degree right
	
	//linefollow 
	return 0;
}

int turnRight() {
	//90 degree turn
	return 0;
}
