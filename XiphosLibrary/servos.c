#include "globals.h"
#include <util/delay.h>

//Validate the number of enabled servos
#if NUM_SERVOS < 0 || NUM_SERVOS > 8
	#error "NUM_SERVOS must be set to a value of 0 to 8 in the project's Makefile"
#endif

#define MAX_PERIOD 20*((u16)(F_CPU/(8*1000)/NUM_SERVOS))
//20ms divided by NUM_SERVOS = number of milliseconds to work on each servo.
//F_CPU/(8*1000) = number of clock cycles in 1 millisecond with a prescalar of 8.

//hard-coded constant to signify the servo is off
#define OFF_VALUE 4600

volatile u08 activeServoNumber = 0;
volatile u08 high = 0;
volatile u16 servoPosition[NUM_SERVOS];
volatile u16 servoLowTime[NUM_SERVOS];

//This function turns a servo off.
//Servo will still have power, but no control pulses will be sent to the servo.
//servoNum can be 0 to (NUM_SERVOS-1).
void servoOff(u08 servoNum)
{
	//Validate servoNum parameter so that we don't overwrite other memory locations.
	if (servoNum < NUM_SERVOS)
	{
		servoPosition[servoNum] = OFF_VALUE;
		servoLowTime[servoNum] = MAX_PERIOD - servoPosition[servoNum];
	}
}

//This function sets the position of a servo.
//The first parameter (0-7) selects the servo.
//The second parameter (0-255) sets the servo position.
void servo(u08 servoNum, u08 position)
{
	//Validate servoNum parameter so that we don't overwrite other memory locations.
	if (servoNum < NUM_SERVOS)
	{
		//Traditional range for servo pulse is 1ms to 2ms with 1.5ms as the center.
		//The following line sets the servo pulse width to .8625ms to 2.1375ms.
		//u16 highTime = 1725 + (10 * position);

		//This is an expanded range for servo motion but will not work with all servos.
		//The following line sets the servo pulse width to .796ms to 2.1986ms.
		//u16 highTime = 1592 + (11 * position);

		//Expanded range 2.
		//The following line sets the servo pulse width to .732ms to 2.262ms.
		u16 highTime = 1464 + (12 * position);

		//this sets the time low to (20ms/NUM_SERVOS) - the pulse width.
		u16 lowTime = MAX_PERIOD - servoPosition[servoNum];

		cli();
		servoPosition[servoNum] = highTime;
		servoLowTime[servoNum] = lowTime;
		sei();
	}
}

//This function initializes the timers so that the other servo 
//routines function correctly. Call this function before 
//using any of the servo functions.
//Normally called only by the initialize() function in utility.c.
void servoInit()
{
	u08 i;

	for (i = 0; i < NUM_SERVOS; i++)
	{
		servoOff(i);
	}

	//enable timer (set prescalar to /8)
	TCCR3B |= _BV(CS31);

	//enable interrupt for OCIE3C
	TIMSK3 |= _BV(OCIE3C);

	//enable interrupts
	sei();
}

void writeServoOutput(u08 servoOutput)
{	
	PORTC = servoOutput;

	_delay_loop_1(1);
	//clock in the servo output
	sbi(PORTD,5);
	_delay_loop_1(1);
	//clock in the servo output
	cbi(PORTD,5);
	_delay_loop_1(1);
}

//This is the interrupt service routine to control 1-8 servos.
ISR(TIMER3_COMPC_vect)
{
	if (high == 1)
	{
		writeServoOutput(0);
		OCR3C += servoLowTime[activeServoNumber];
		high = 0;
	}
	else
	{
		activeServoNumber++;

		//when activeServoNumber goes beyond the last enabled servo, start over at servo 0
		if (activeServoNumber == NUM_SERVOS)
		{
			activeServoNumber = 0;
		}

		if (servoPosition[activeServoNumber] != OFF_VALUE)
		{
			writeServoOutput(_BV(activeServoNumber));
		}

		OCR3C += servoPosition[activeServoNumber];
		high = 1;
	}
}
