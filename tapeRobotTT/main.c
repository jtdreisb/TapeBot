//TapeBot

#include "../XiphosLibrary/globals.h"

#define TEST 0


void initInterrupts();
void gateClose();
void gateOpen();
void sensorTest();



int main()
{
	
	initialize();
	cli();
	//initInterrupts();
	
	
	digitalDirection(0,INPUT);
	digitalDirection(1,INPUT);
	
	//Test 1
	clearScreen();
	printString("    TapeBot V1.0  ");
	lowerLine();
	
	//Turn motors on and wait until not against wall to turn on interrupt
	sei();
	
	gateClose();
	buttonWait();
	if(TEST) {
		followEdge();
	} else {
	
	getOutOfStartBox();
	brake();
	crossPattern();
	//innerSquare();	
	
	
	followEdge();
	
	
	clearScreen();
	printString("TapeBot Wins!");
	
	}


	
	while(1){
	}
}



void initInterrupts() {
	cli();
	//Data Direction of PINB4 => input 
	DDRB |= (1<<DDB4);
	//Disable internal pullup
	PORTB &= ~(1<<PB4); 
	//Enable PCINT4 of PINB4 and PINB7
	PCMSK0 |= (1<<PCINT4);
	PCMSK0 |= (1<<PCINT7);
	//Enable to first 8 interrupts of the board (even though there are only 8)
	PCICR |= (1<<PCIE0);
	//System wide enable of interrupts
	sei();
}
void gateClose() {
	servo(0,115);
}
void gateOpen() {
	servo(0,255);
}


void sensorTest() {
	u08 fLines = 0;
	u08 rLines = 0;
	u08 lLines = 0;
	u08 fVal;
	u08 rVal;
	u08 lVal;
	u08 sawLine = 0x0;

	
	upperLine();
	printString("F:   R:   L:   ");
	
	lcdCursor(1,2);
	print_u08(fLines);
	lcdCursor(1,6);
	print_u08(rLines);
	lcdCursor(1,12);
	print_u08(rLines);
	
	while(1) {
		rVal = analog(TBSENSOR_IR_RIGHT);
		lVal = analog(TBSENSOR_IR_LEFT);
		fVal = analog(TBSENSOR_IR_FRONT);
		
		lcdCursor(0,2);
		print_u08(fVal);
		lcdCursor(0,7);
		print_u08(rVal);
		lcdCursor(0,12);
		print_u08(lVal);
		
		if(!sawLine) {
			
			if(fVal > 100) {
				fLines++;
				lcdCursor(1,2);
				print_u08(fLines);
				sawLine |= 0x4;
			}
			if(rVal > 100) {
				rLines++;
				lcdCursor(1,7);
				print_u08(rLines);
				sawLine |= 0x1;
			}
			if(lVal > 100) {
				lLines++;
				lcdCursor(1,12);
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
			if(fVal > 100) {
				fLines++;
				lcdCursor(1,2);
				print_u08(fLines);
				sawLine |= 0x4;
			}
			if(rVal < 50)	sawLine &= ~0x1;
		}
		else if(sawLine == 0x2) {
			if(rVal > 100) {
				rLines++;
				lcdCursor(1,3);
				print_u08(rLines);
				sawLine |= 0x1;
			}
			if(fVal > 100) {
				fLines++;
				lcdCursor(1,2);
				print_u08(fLines);
				sawLine |= 0x4;
			}
			if(lVal < 50 )	sawLine &= ~0x2;
		}
		else if(sawLine == 0x4) {
			if(rVal > 170) {
				rLines++;
				lcdCursor(1,7);
				print_u08(rLines);
				sawLine |= 0x1;
			}
			if(lVal > 100) {
				lLines++;
				lcdCursor(1,12);
				print_u08(lLines);
				sawLine |= 0x2;
			}
			if(fVal < 50 )	sawLine &= ~0x4;
		} else {
			if(rVal < 50)	sawLine &= ~0x1;
			if(lVal < 50)	sawLine &= ~0x2;
			if(fVal < 50) 	sawLine &= ~0x4;
		}
		
	}
}



