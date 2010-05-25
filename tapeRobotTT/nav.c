//Navigation code
#include "../XiphosLibrary/globals.h"
#include "nav.h"

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
//doesnt work right
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


void getOutOfStartBox() {
	motor0(70);
	motor1(75);
	countLines(TBSENSOR_IR_LEFT,1);
	move(70);
}

void makeRightTurn(u08 r) {
	
	move(60);
	countLines(TBSENSOR_IR_FRONT, 1);
	if (r == 2 || r == 3) {
		delayMs(150);
	}
	brake();
	spinRight(40);
	countLines(TBSENSOR_IR_LEFT, 2);
	if (r == 1 || r == 3) {
		reverse(60);
		delayMs(200);
	}	
	squareBackSensors();
	brake();
	//square a second time
	spinRight(40);
	delayMs(100);
	reverse(50);
	delayMs(300);
	squareBackSensors();
	
	brake();
}

void turnTheCornerRight() {
	clearScreen();
	upperLine();
	printString("Turning!");
	
	move(70);
	countLines(TBSENSOR_IR_FRONT, 2);
	spinRight(40);
	countLines(TBSENSOR_IR_LEFT, 1);
	brake();
	motor1(160);
	countLines(TBSENSOR_IR_FRONT, 1);
	move(60);
	delayMs(300);
	brake();
	delayMs(30);
	move(0);
	
}

void plowTheCenter() {
	cli();
	move(40);
	delayMs(50);
	move(80);
	delayMs(50);
	move(120);
	countLines(TBSENSOR_IR_FRONT, 5);
	sei();
	brake();
	reverse(50);
	squareBackSensors();
	move(50);	
}



void squareBackSensors() {
	u08 rVal,lVal;
	u08 onLine = 0;
	upperLine();
	printString("Left  |   Right ");
	
	move(50);
	
	while(onLine < 3) {
		lVal = analog(TBSENSOR_IR_LEFT);
		rVal = analog(TBSENSOR_IR_RIGHT);
		
		lcdCursor(1,2);
		print_u08(lVal);
		lcdCursor(1,9);
		print_u08(rVal);
		
		if(lVal > kTHRESHOLD_HIGH) {
			brake0();
			lcdCursor(1,2);
			printString("ON ");
			onLine |= 0x01;
		} 
		/*else if(lVal < kTHRESHOLD_LOW) {
			lcdCursor(1,2);
			printString("OFF");
			onLine &= ~0x01;
			motor0(150);
			delayMs(500);
			motor0(70);
		}*/
		
		if(rVal > kTHRESHOLD_HIGH) {
			brake1();
			lcdCursor(1,9);	
			printString("ON ");
			onLine |= 0x02;
		} /*else if(rVal < kTHRESHOLD_LOW) {
			lcdCursor(1,2);
			printString("OFF");
			onLine &= ~0x02;
			motor1(150);
			delayMs(500);
			motor1(70);
		}*/
	}
}


void move(s08 speed) {
	motor0(127-speed);
	motor1(127-speed);
}

void reverse(u08 speed) {
	motor0(127+speed);
	motor1(127+speed);
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

ISR(PCINT0_vect) { // Bump Sensor
	cli();
	brake();
	
	clearScreen();
	upperLine();
	printString("Oh No");
	lowerLine();
	printString("I am Stuck");
	
	//motor0(160);
	//motor1(160);
	//delayMs(750);
	//spinRight(50);
	//delayMs(750);
	clearScreen();
	move(0);
	sei();
}
