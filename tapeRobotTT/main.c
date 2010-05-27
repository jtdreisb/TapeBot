//TapeBot

#include "../XiphosLibrary/globals.h"


void initInterrupts();
void gateClose();
void gateOpen();


int main()
{
	
	initialize();
	cli();
	initInterrupts();
	
	//Test 1
	clearScreen();
	printString("    TapeBot V1.0  ");
	lowerLine();
	
	//Turn motors on and wait until not against wall to turn on interrupt
	sei();
	
	gateClose();
	buttonWait();
	
	getOutOfStartBox();
	brake();
	crossPattern();
	innerSquare();	
	
	
	followEdge();
	
	
	clearScreen();
	printString("TapeBot Wins!");
	gateOpen();
	buttonWait();
	gateClose();
	
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




