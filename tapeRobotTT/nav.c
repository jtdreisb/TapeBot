//Navigation code
#include "../XiphosLibrary/globals.h"
#include "nav.h"


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
	clearScreen();
	printString(TBSENSOR_IR_RIGHT, TBSENSOR_IR_LEFT);
	motor0(70);
	motor1(80);
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
	makeRightTurn(100);
	
	clearScreen();
	printString("PLOW");
	plowTheCenter(5);
	
	clearScreen();
	printString("PLOW - RTURN");
	makeRightTurn(0);
}

void followEdge() {
	//spin90Left();
	while(1) {
		clearScreen();
		printString("Line Follow1");
		lineFollow();
		clearScreen();
		printString("Dump");
		u08 valid = dump();
		clearScreen();
		printString("Turn Out1");
		outerRightCorner();
		if (!valid) continue;
		clearScreen();
		printString("Line Follow2");
		lineFollow();
		clearScreen();
		printString("Turn Out1");
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
		if(range < 45) {
			if(val < kTHRESHOLD_LOW ) {
				motor1(45);
			} else if (val > kTHRESHOLD_HIGH) {
				motor1(90);
			} else {
				motor1(60);
			}
		} else {
			delayMs(50);
			range = analog(TBSENSOR_PROX);
			if (range >= 45) {
				brake();
				break;
			}
		}
		
	}
}
void outerRightCorner() {
	u08 prox = analog(TBSENSOR_PROX);
	motor0(70);
	motor1(100);
	delayMs(400);
	motor1(160);
	u08 timeout;
	for(timeout=0; prox > 20; timeout++) {
		prox = analog(TBSENSOR_PROX);
		delayMs(10);
		if (timeout > 250) {
			reverse(50);
			delayMs(400);
			motor1(160);
			motor0(70);
			timeout = 0;
		}
	}
	move(0);
}

u08 dump() {
	cli();
	motor1(55);
	motor0(60);
	u16 timeout = 0;
	u16 timeout2 = 0;
	while(!(digitalInput(0) || digitalInput(1))) {
		delayMs(1);
		if (timeout > 1000) {
			timeout = 0;
			break;
		}
		timeout++;
	}
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
			while(!(digitalInput(0) || digitalInput(1))) {
				delayMs(1);
				if (timeout2 > 1000) {
					timeout2 = 0;
					break;
				}
				timeout2++;
			}
			i = 105;
		}
	}
	if( i <= 100 || !timeout || !timeout2 ) {
		brake();
		if (i > 102) {
			return 0;
		} else {
			return 1;
		}
	}
	gateOpen();
	delayMs(1000);
	reverse(35);
	delayMs(100);
	move(65);
	delayMs(300);
	brake();
	delayMs(2000);
	gateClose();
	delayMs(400);
	motor0(220);
	motor1(200);
	delayMs(400);
	return 1;
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
	u16 timeout = 0;
	
	while(lineCount != numLines) {
		delayMs(1);
		timeout++;
		if (timeout > 7000) break;
		
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
	if(!r) {
		delayMs(350);
	} else {
		delayMs(100);
	}
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
	motor0(60);
	motor1(65);
	delayMs(300);
		
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
	smoothAccelerate(125);
	motor0(3);
	motor1(1);
	countLines(TBSENSOR_IR_FRONT, lines);
	sei();
	if (lines == 5) {
		delayMs(200);
		move(50);
		squareBackSensors(1);
	}
	brake();
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
	u16 timeout = 0;
	//upperLine();
	//printString("Left  |   Right ");
	
	while(onLine < 3) {
		delayMs(1);
		timeout++;
		if (timeout > 7000) break;
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




