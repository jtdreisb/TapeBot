//Real Time Counter for the Xiphos 1.0
//Darron Baida 5-20-09
#include "globals.h"

//There is a 32.768kHz crystal oscillator attached to the Xiphos board which allows keeping of real time.
//This means 32768 ticks of the oscillator happen per second.
//When using a prescalar of 128, 256 post-prescalar ticks happen per second. (32768/128 = 256)

//Normally called only by the initialize() function in utility.c.
void rtcInit()
{
	//Make the crystal oscillator the clock source for timer2.
	ASSR = (1<<AS2);

	//Set timer2 prescalar to 128.
	//By doing so, the timer will overflow exactly once per second, a very convenient scheme indeed!
	//This prescalar value can be modified to have the interrupt fire at a different interval.
	//Search for TCCR2B in the data sheet for details.
	TCCR2B = (1<<CS22) | (1<<CS20);

	//Interupt when timer2 overflows.
	TIMSK2 = (1<<TOIE2);

	//Reset secCount.
	secCount = 0;
	//Reset fractional seconds to 0.
	TCNT2 = 0;

	//enable interrupts
	sei();
}

ISR(TIMER2_OVF_vect)
{
	//The timer has overflowed, one second has elapsed so increment secCount.
	secCount++;

	//secCount is declared in globals.h so that it will be available to the main file.
	//You can add more functionality before the last curly bracket and this code will be executed once per second.
	//If necessary, additional global variables like "minutes" or "hours" can be declared in projectGlobals.h or globals.h.
}
