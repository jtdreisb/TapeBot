//Navigation code
#include "../XiphosLibrary/globals.h"
#include "nav.h"

u08 dumpable;

// ISR(PCINT0_vect) { // Bump Sensor
	// cli();
	// if(dumpable) {
		// gateOpen();
		// reverse(30);
		// delayMs(100);
		// move(80);
		// delayMs(50);
		// brake();
		// delayMs(2000);
		// reverse(60);
		// delayMs(500);
	// }	
	// brake();
	
	// delayMs(500);
	// sei();
// }


void getOutOfStartBox() {
	dumpable = 1;
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
	plowTheCenter(5);
	
	clearScreen();
	printString("PLOW - RTURN");
	makeRightTurn(100);
}

void followEdge() {
	//spin90Left();
	while(1) {
		lineFollow();
		dump();
		outerRightCorner();
		lineFollow();
		outerRightCorner();
	}
}
void lineFollow() {
	u08 range, val;
	motor0(60);
	motor1(60);
	while(1) {
		val = analog(TBSENSOR_IR_FRONT);
		range = analog(TBSENSOR_PROX);
		clearScreen();
		//print_u08(range);
		if(range < 45) {
			if(val < kTHRESHOLD_LOW ) {
				motor1(45);
			} else if (val > kTHRESHOLD_HIGH) {
				motor1(70);
			} else {
				motor1(60);
			}
		} else {
			brake();
			break;
		}
		
	}
	//lowerLine();
	//printString("WhatsUp");
	//delayMs(500);
}
void outerRightCorner() {
	u08 prox = analog(TBSENSOR_PROX);
	motor0(70);
	motor1(100);
	delayMs(400);
	motor1(160);
	while(prox > 20) {
		prox = analog(TBSENSOR_PROX);
	}
	move(0);
}

void dump() {
	cli();
	move(60);
	while(!(digitalInput(0) || digitalInput(1)));
	//on the wall
	if(digitalInput(0)) {
		brake1();
	} else if (digitalInput(1)) {
		brake0();
	}
	delayMs(500);
	reverse(50);
	u08 i;
	for(i=0; i<100; i++) {
		delayMs(10);
		if(analog(TBSENSOR_IR_LEFT) > kTHRESHOLD_HIGH) {
			move(50);
			while(!(digitalInput(0) || digitalInput(1)));
			i = 105;
			clearScreen();
			printString("FOUND LINE");
		}
	}
	if(i > 101) {
		gateOpen();
		delayMs(1000);
		reverse(50);
		delayMs(100);
		move(65);
		delayMs(1000);
	}

	brake();
	delayMs(2000);
	gateClose();
	delayMs(400);
	motor0(210);
	motor1(200);
	delayMs(500);
	sei();
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
	delayMs(350);
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




