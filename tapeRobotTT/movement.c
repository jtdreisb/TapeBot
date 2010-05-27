//Movement.c

#include "../XiphosLibrary/globals.h"


void move(s08 speed) {
	motor0(127-speed);
	motor1(127-speed);
}

void smoothAccelerate(u08 topSpeed) {
	u08 delay = 19; //slower delay = faster acceleration!
	u08 intervals = topSpeed / delay;
	u08 i;
	for (i = 1; i <= intervals; i++) {
		move(topSpeed * i/intervals);
		delayMs(delay);
	}
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
