//Navigation code
#include "../XiphosLibrary/globals.h"
#include "nav.h"


void getOutOfStartBox() {
	clearScreen();
	printString(TBSENSOR_IR_RIGHT, TBSENSOR_IR_LEFT);
	motor0(70);
	motor1(75);
	countLines(TBSENSOR_IR_LEFT,1);
	move(70);
}


void crossPattern() {
	clearScreen();
	printString("START - RTURN");
	makeRightTurn(150);
	
	clearScreen();
	printString("RTURN - PLOW");
	plowTheCenter(5);
	
	clearScreen();
	printString("RTURN");	
	makeRightTurn(0);
	
	clearScreen();
	printString("RCORNER");
	turnTheCornerRight();
	
	clearScreen();
	printString("RTURN");
	makeRightTurn(150);
	
	clearScreen();
	printString("PLOW");
	plowTheCenter(4);
	
	clearScreen();
	printString("PLOW - RTURN");
	makeRightTurn(51);
}

void followEdge() {
	spin90Left();
	while(1) {
		//makeRightTurn();
	}
}


void innerSquare() {
	clearScreen();
	printString("RCORNER");
	turnTheInnerCornerRight();
	clearScreen();
	printString("Plow Inner");
	plowTheInner(2);
	clearScreen();
	printString("Turn Inner");
	turnTheInnerCornerRight();
	clearScreen();
	printString("Continue");
}


void countLines(u08 sensor,u08 numLines) {
	u08 lineCount = 0;
	u08 val;
	u08 onLine;

	
	while(lineCount != numLines) {
		val = analog(sensor);
		if(!onLine) {
			if(val > kTHRESHOLD_HIGH) {
				lineCount++;
				onLine = 1;
			}
		} else {
			if(val < kTHRESHOLD_LOW)
				onLine = 0;
		}
	}
}


void makeRightTurn(u08 r) {
	
	move(50);
	countLines(TBSENSOR_IR_FRONT, 1);
	delayMs(r);
	brake();
	spinRight(40);
	countLines(TBSENSOR_IR_LEFT, 2);
	reverse(60);
	if (r == 51) {
		motor1(170);
		delayMs(200);
	}
	delayMs(150);
	move(40);
	squareBackSensors(1);
	brake();
	//square a second time
	spinRight(40);
	delayMs(100);
	reverse(50);
	delayMs(300);
	move(40);
	squareBackSensors(1);
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
	motor1(180);
	countLines(TBSENSOR_IR_FRONT, 1);
	move(60);
	delayMs(300);
	brake();	
}

void turnTheInnerCornerRight() {
	
	move(70);
	delayMs(200);
	move(50);
	squareBackSensors(1);
	reverse(50);
	delayMs(200);
	move(50);
	squareBackSensors(1);
	brake();
	
	//on diagonal
	reverse(60);
	delayMs(200);
	motor0(160);
	motor1(200);
	countLines(TBSENSOR_IR_LEFT, 1);
	squareBackSensors(1);
	reverse(50);
	delayMs(200);
	move(70);
	squareBackSensors(1);
	brake();
	delayMs(200);
}

void plowTheCenter(u08 lines) {
	cli();
	smoothAccelerate(120);
	motor0(7);
	motor1(5);
	countLines(TBSENSOR_IR_FRONT, lines);
	sei();
	brake();
	if (lines == 5) {
		move(50);
		squareBackSensors(1);
	}
}

void plowTheInner(u08 lines) {
	cli();
	move(70);
	motor1(50);
	delayMs(100);
	countLines(TBSENSOR_IR_RIGHT, lines);
	delayMs(50);
	squareBackSensors(1);
	reverse(50);
	delayMs(200);
	move(50);
	squareBackSensors(1);
	sei();
	brake();
}

void spin90Left() {
	//Spin like a top
	motor0(180);
	motor1(145);
	delayMs(200);
	countLines(TBSENSOR_IR_LEFT, 2);
	reverse(60);
	delayMs(300);
	move(60);
	squareBackSensors(1);
	reverse(50);
	delayMs(200);
	move(50);
	squareBackSensors(1);
	brake();
}

void squareBackSensors(u08 brake) {
	u08 rVal,lVal;
	u08 onLine = 0;
	//upperLine();
	//printString("Left  |   Right ");
	
	while(onLine < 3) {
		lVal = analog(TBSENSOR_IR_LEFT);
		rVal = analog(TBSENSOR_IR_RIGHT);
		
		// lcdCursor(1,2);
		// print_u08(lVal);
		// lcdCursor(1,9);
		// print_u08(rVal);
		
		if(lVal > kTHRESHOLD_HIGH) {
			if (brake) {
				brake0();
			} else {
				motor0(127);
			}
			// lcdCursor(1,2);
			// printString("ON ");
			onLine |= 0x01;
		}
		
		if(rVal > kTHRESHOLD_HIGH) {
			if (brake) {
				brake1();
			} else {
				motor1(127);
			}
			// lcdCursor(1,9);	
			// printString("ON ");
			onLine |= 0x02;
		}
	}
}



ISR(PCINT0_vect) { // Bump Sensor
	cli();
	brake();
	
	clearScreen();
	upperLine();
	printString("Oh No");
	lowerLine();
	printString("I am Stuck");
	delayMs(1000);
	clearScreen();	
	
	move(0);
	while (1);
	//sei();
}
