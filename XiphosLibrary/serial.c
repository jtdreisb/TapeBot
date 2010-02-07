#include "../XiphosLibrary/globals.h"
#include "serial.h"
#include <inttypes.h>
#include <avr/interrupt.h>    // include interrupt support
#include <avr/io.h>           // include I/O definitions (port names, pin names, etc)
#include <math.h>

volatile u08 ReceivedData[MAX_DATA];
volatile u08 DataIndex = 0;

/*
From atmega documentation:
The USART has to be initialized before any communication can take place. The initialization process
normally consists of setting the baud rate, the frame format, and enabling the
Transmitter and/or the Receiver depending on the usage. For interrupt driven USART operation, the
Global Interrupt Flag should be cleared (and interrupts globally disabled) when doing the
initialization.

BAUD should be one of: 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 76800, 115200, 230400, 250000
*/
 

void uart0Init()
{ 
	cli();
	UBRR0L = (uint8_t)(F_CPU/(BAUD*16L)-1);
	UBRR0H = (F_CPU/(BAUD*16L)-1) >> 8;
	UCSR0A = 0x00;
   // Set USART Control and Status Register C to configure frame format, etc
	// asynchronous, 8 data bits, no parity, 1 stop bit, UCPOL must be 0 in async mode
	UCSR0C = 0x06;
	//(1<<URSEL) is a register selecter only needed for atmega32. URSEL must be one when writing to UCSRC.
	// asynchronous=(0<<UMSEL00), synchronous=(1<<UMSEL00)
	// 5 databits=(0<<UCSZ00), 6 databits=(1<<UCSZ00), 7 databits=(2<<UCSZ00), 8 databits=(3<<UCSZ00), a 9 databit mode is also supported but uses additional registers
	// no parity=(0<<UPM00), even parity=(2<<UPM00), odd parity=(3<<UPM00) - parity mismatches set UPE bit in UCSRA
	// 1 stop bit=(0<<USBS0), 2 stop bits=(1<<USBS0) - this bit is only used for transmitter, ignored by receiver
	// clock polarity (for synchronous mode only, set to 0 for async mode):
	//     (0<<UCPOL) = transmit on rising XCK (synchronous clock) edge and receive on falling XCK edge
	//     (1<<UCPOL) = transmit on falling XCK (synchronous clock) edge and receive on rising XCK edge
	UCSR0B = _BV(TXEN0)|_BV(RXEN0)|_BV(RXCIE0);
	// Enable Receiver=(1<<RXEN)
	// Enable Transmitter=(1<<TXEN)
	// Enable RX Complete Interrupt=(1<<RXCIE), fires USART_RXC_vect/USART0_RX_vect ISR when RXC bit in UCSRA is set
	// Enable TX Complete Interrupt=(1<<TXCIE), fires USART_TXC_vect/USART0_TX_vect ISR when TXC bit in UCSRA is set
	// Enable USART Data Register Empty Interrupt=(1<<UDRIE), fires USART_UDRE_vect/USART0_UDRE_vect ISR when UDRE bit in UCSRA is set
	sei();
}

u08 get0(void)
{
		// Wait for a buffer to fill
		while(!(UCSR0A & _BV(RXC0)));
		// Return buffer
		return UDR0;
}
void put0(u08 data)
{
   // Wait for empty transmit buffer 
    while (!(UCSR0A & (1 << UDRE0))); 

    // Start transmition 
    UDR0 = data;  
}

void uart1Init()
{
	cli();
	UBRR1L = (uint8_t)(F_CPU/(BAUD*16L)-1);
	UBRR1H = (F_CPU/(BAUD*16L)-1) >> 8;
	UCSR1A = 0x00;

	UCSR1C = 0x06;
	
	UCSR1B = _BV(TXEN1)|_BV(RXEN1)|_BV(RXCIE1);
	sei();
}


u08 get1(void)
{
		while(!(UCSR1A & _BV(RXC1)));
		return UDR1;
}

void put1(u08 data)
{
   while (!(UCSR1A & _BV(UDRE1)));
	UDR1 = data;
}
    
u16 parse_u16()
{
	u16 output;
	output = ((u16)ReceivedData[1])<<8; //load high byte
	output |= ReceivedData[2];   //load low byte
	return output;
} 


void executeFunction()
{
	u16 u16temp;

	//command is stored in first position of the array
	switch(ReceivedData[0])
	{
		//parameterless functions
		case LED_ON:
			ledOn();
			break;
		case LED_OFF:
         ledOff();
         break;
//		case RELAY_ON:
//			relayOn();
//	      break;
//		case RELAY_OFF:
//			relayOff();
//			break;
		case LCD_ON:
			lcdOn();
			break;
		case LCD_OFF:
			lcdOff();
			break;
		case CLEAR_SCREEN:
			clearScreen();
			break;
		case NEXT_LINE:
			upperLine();
			break;
		case SOFT_RESET:
			softReset();
			break;
	   case BRAKE0:
	        brake0();
	   break;
	   case BRAKE1:
	        brake1();
	   break;     
		//single-parameter functions
		case DELAY_MS:
			delayMs(parse_u16()); break;
		case DELAY_US:
			delayUs(parse_u16()); break;
		case PRINT_STRING:
			printString(ReceivedData+1); break;
		case SERVO_OFF:
			servoOff(parse_u16()); break;

		//two-parameter functions
		case SET_SERVO_POS:
			u16temp = parse_u16();
			servo(u16temp>>8,(u08)u16temp);
			break;
		case MOTOR0:
			u16temp = parse_u16();
			motor0((u08)u16temp);
			break;
		case MOTOR1:
			u16temp = parse_u16();
			motor1((u08)u16temp);
			break;
		case LCD_CURSOR:
			u16temp = parse_u16();
			lcdCursor(u16temp>>8, (u08)u16temp);
			break;

		//functions with return values
		case DIGITAL:
			//put0(digital(parse_u16())); break;
		case ANALOG:
			//put0(analog(parse_u16())); break;
		case GET_SW1:
			put0(getButton1()); break;
		case KNOB:
			//put0(knob()); break;
		default:
			// Command was not recognized
			lcdCursor(0, 0);
			printString("CMD Unrecognized"); break;
	}
	
	//reset DataIndex so the next command is saved into the first array position
	DataIndex = 0;
}
#if (USE_UART0 == 1) 
/* This interrupt is triggered when a character is received on the UART.
 * If parameters are needed, polling is used to receive them.
 * The requested function is called and the return value, if any, is transmitted.
 */
ISR(USART0_RX_vect)
{
	u08 data = UDR0;
//#endif
	//prevent overruns of the ReceivedData array
	if (DataIndex >= MAX_DATA)
	{
		//reset DataIndex to 0, effectively ignoring the command
		DataIndex = 0;
	}
	else
	{
		//else we can safely store another byte of data
		ReceivedData[DataIndex++] = data;
		//execute parameterless functions immediately
		if (ReceivedData[0] < DELAY_MS)
		{
			executeFunction();
		}
		//string parameter has variable length so it needs special handling
		else if (ReceivedData[0] == PRINT_STRING)
		{
			//execute when null terminator is received
			if (data == 0)
			{
				executeFunction();
			}
		}
		//execute 1- and 2-parameter functions once 3 bytes have been received
		else
		{
			if (DataIndex == 3)
			{
				executeFunction();
			}
		}
	}
}
#endif
#if (USE_UART1 == 1)
/* This interrupt is triggered when a character is received on the UART.
 * If parameters are needed, polling is used to receive them.
 * The requested function is called and the return value, if any, is transmitted.
 */
ISR(USART1_RX_vect)
{
	u08 data = UDR0;
//#endif
	//prevent overruns of the ReceivedData array
	if (DataIndex >= MAX_DATA)
	{
		//reset DataIndex to 0, effectively ignoring the command
		DataIndex = 0;
	}
	else
	{
		//else we can safely store another byte of data
		ReceivedData[DataIndex++] = data;
		//execute parameterless functions immediately
		if (ReceivedData[0] < DELAY_MS)
		{
			executeFunction();
		}
		//string parameter has variable length so it needs special handling
		else if (ReceivedData[0] == PRINT_STRING)
		{
			//execute when null terminator is received
			if (data == 0)
			{
				executeFunction();
			}
		}
		//execute 1- and 2-parameter functions once 3 bytes have been received
		else
		{
			if (DataIndex == 3)
			{
				executeFunction();
			}
		}
	}
}
#endif

