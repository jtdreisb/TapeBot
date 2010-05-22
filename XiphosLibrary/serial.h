#include "../XiphosLibrary/globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//Baud rate settings
#define BAUD     9600
//Defines the acceptable baud rate tolerance in percent. Default 2%.
//Also #defines USE_2X as 1 if the desired baud rate tolerance could only be achieved by setting the U2X bit in the UART configuration.
#define BAUD_TOL 2 

//Xiphos Remote System
#define MAX_DATA 18

//parameterless functions
#define LED_ON           1
#define LED_OFF          2
#define RELAY_ON         3
#define RELAY_OFF        4
#define LCD_ON           5
#define LCD_OFF          6
#define CLEAR_SCREEN     7
#define NEXT_LINE        8
#define GET_SW1          9
#define KNOB             10
#define SOFT_RESET       11
#define BRAKE0           12
#define BRAKE1           13

//single-parameter functions
#define DELAY_MS        20
#define DELAY_US        21
#define PRINT_STRING    22
#define DIGITAL         23
#define ANALOG          24
#define SERVO_OFF       25

//two-parameter functions
#define SET_SERVO_POS   30
#define MOTOR0          31
#define LCD_CURSOR      32
#define MOTOR1          33
