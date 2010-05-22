//TapeBot

#include "../XiphosLibrary/globals.h"

#define kTHRESHOLD_HIGH 100
#define kTHRESHOLD_LOW  70

void getOutOfStartBox(); //does an S move to get out of the box
void driveOuterSquare(); 
void countLines(u08 sensor,u08 numLines); // counts till the sensor hits the black
void waitLines(u08 sensor,u08 numLines); // waits till the sensor is off the line
int findInnerSquare();
void move(s08 speed);
void brake();
void spinRight(s08 delta);
void initInterrupts();


int main()
{
	initialize();
	cli();
	
	//Test 1
	clearScreen();
	printString("    TapeBot V1.0  ");
	lowerLine();
	
	//Turn motors on and wait until not against wall to turn on interrupt
	sei();
	
	buttonWait();
	//Drive around and don't crash
	getOutOfStartBox();
	
	driveOuterSquare();
	buttonWait();
	
	
	move(70);
	delayMs(2000);
	brake();
	
	clearScreen();
	printString("Yay it Finished");
	while(1){
	}

}

void getOutOfStartBox() {
	motor0(70);
	motor1(90);
	delayMs(300);
	move(70);
	delayMs(100);
}

void driveOuterSquare() {
	
	move(60);
	countLines(TBSENSOR_IR_FRONT, 2);
	delayMs(100);
	brake();
	spinRight(40);
	countLines(TBSENSOR_IR_LEFT, 2);
	brake();
	motor1(90);
	countLines(TBSENSOR_IR_RIGHT,1);
	brake();
	
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

void waitLines(u08 sensor,u08 numLines) {
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
				onLine = 1;
			}
		} else {
			if(val < kTHRESHOLD_LOW)
				lineCount++;
				lcdCursor(1,11); 
				print_u08(lineCount);
				onLine = 0;
		}
	}
}


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

void initInterrupts() {
	cli();
	//Data Direction of PINB4 => input 
	DDRB |= (1<<DDB4);
	//Disable internal pullup
	PORTB &= ~(1<<PB4); 
	//Enable PCINT4 of PINB4
	PCMSK0 |= (1<<PCINT4);
	//Enable to first 8 interrupts of the board (even though there are only 8)
	PCICR |= (1<<PCIE0);
	//System wide enable of interrupts
	sei();
}

ISR(PCINT0_vect) { // Bump Sensor
	motor0(160);
	motor1(160);
	delayMs(750);
	spinRight(50);
	delayMs(750);
}

