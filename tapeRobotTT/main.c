//TapeBot

#include "../XiphosLibrary/globals.h"

#define kTHRESHOLD_HIGH 160
#define kTHRESHOLD_LOW  140

void countLines(u08 sensor,u08 numLines);
int findInnerSquare();
void move(s08 speed);
void brake();
void spinRight(s08 delta);


int main()
{
	initialize();
	cli();
	
	//Test 1
	clearScreen();
	printString("    TapeBot     ");
	lowerLine();
	
	//Turn motors on and wait until not against wall to turn on interrupt
	sei();
	
	buttonWait();
	motor0(70);
	motor1(90);
	delayMs(500);

	move(60);
	countLines(TBSENSOR_IR_FRONT, 1);
	delayMs(100);
	brake();
	motor0(80);
	motor1(150);	//spinRight(30);
	countLines(TBSENSOR_IR_LEFT, 2);
	brake();
	motor1(90);
	countLines(TBSENSOR_IR_RIGHT,1);
	brake();
	
	buttonWait(); //assuming 90 degrees
	
	move(60);
	delayMs(100);
	brake();
	move(60);
	delayMs(1000);
	brake();
	
	clearScreen();
	printString("Yay it Finished");
	while(1){
	}

}

void countLines(u08 sensor,u08 numLines) {
	u08 lineCount = 0;
	u08 val;
	u08 onLine;
	upperLine();
	printString("Count      lines");
	lcdCursor(0, 6);
	print_u08(numLines);
	lowerLine("Val:");
	lcdCursor(1,11); 
	print_u08(lineCount);
	
	while(lineCount != numLines) {
		val = analog(sensor);
		lcdCursor(1,3);
		print_u08(val);
		if(!onLine) {
			if(val > kTHRESHOLD_HIGH) {
				lineCount++;
				lcdCursor(1,11); 
				print_u08(lineCount);
				onLine = 1;
			}
		} else {
			if(val < kTHRESHOLD_LOW)
				onLine = 0;
		}
	}
}

/*
void countLines() {
	u08 rLines = 0;
	u08 lLines = 0;
	u08 rVal;
	u08 lVal;
	u08 sawLine = 0x0;
	//motor0(90);
	//motor1(90);
	
	upperLine();
	printString("F:      S:      ");
	
	lcdCursor(1, 3);
	print_u08(rLines);
	lcdCursor(1,9);
	print_u08(lLines);
	
	while(1) {
		rVal = analog(TBSENSOR_IR_FRONT);
		lVal = analog(TBSENSOR_IR_SIDE);
		
		lcdCursor(0,3);
		print_u08(rVal);
		lcdCursor(0,11);
		print_u08(lVal);
		
		if(!sawLine) {
			
			if(rVal > 170) {
				rLines++;
				lcdCursor(1,3);
				print_u08(rLines);
				sawLine |= 0x1;
			}
			if(lVal > 100) {
				lLines++;
				lcdCursor(1,9);
				print_u08(lLines);
				sawLine |= 0x2;
			}
		} 
		else if(sawLine == 0x1) {
			if(lVal > 100) {
				lLines++;
				lcdCursor(1,9);
				print_u08(lLines);
				sawLine |= 0x2;
			}
			if(rVal < 140)	sawLine &= ~0x1;
		}
		else if(sawLine == 0x2) {
			if(rVal > 170) {
				rLines++;
				lcdCursor(1,3);
				print_u08(rLines);
				sawLine |= 0x1;
			}
			if(lVal < 50 )	sawLine &= ~0x2;
		} else {
			if(rVal < 140)	sawLine &= ~0x1;
			if(lVal < 50 )	sawLine &= ~0x2;
		}
		
	}
}*/

/*ISR(SOMEBUTTON_VECTOR) {	
	//if 2 bump sensors are activated turn right
	//count the walls that the bot has run into
	turnRight();
	//numWalls++
	//when it reaches 4 walls dump the balls then find inner circle
	//dump();
	//otherwise correct the trajectory of the bot
	
}
*/
int findInnerSquare() {
	//back up past two lines
	
	//turn 90 degrees right
	
	//forward past two lines
	
	//90 degree right
	
	//linefollow 
	return 0;
}

//Movement functions

void move(s08 speed) {
	motor0(127-speed);
	motor1(127-speed);
}

void brake() {
	brake0();
	brake1();
}
void spinRight(s08 delta) {
	//Spin like a top
	motor0(127-delta);
	motor1(127+delta);
}
