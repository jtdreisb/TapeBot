//Global defines

#ifndef GLOBALS_H
#define GLOBALS_H

#include <avr/io.h>          //I/O definitions (port/pin names, register names, named register bits, etc.)
#include <avr/interrupt.h>   //interrupt support
#include "projectGlobals.h"  //user-configured project-specific global #defines. To work, this requires "-I ." in the avr-gcc arguments.

#if USE_LCD == 1
	#include "LCDSpecialChars.h" //defines friendly names for various LCD special character codes.
#endif

//Define enumeration to specify the valid values that can be passed to the digitalDirection function.
typedef enum {INPUT=0, OUTPUT=1, INPUT_PULLUP=2} Direction;

//Define CPU speed (16MHz) for delay loop computations
#define F_CPU 16000000UL

//Define new datatypes as easier shorthand
typedef unsigned char u08;
typedef signed char   s08;
typedef unsigned int  u16;
typedef signed int    s16;
typedef unsigned long u32;
typedef signed long   s32;

//Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //sets bit b in variable a
#define cbi(a, b) ((a) &= ~(1 << (b)))    //clears bit b in variable a
#define tbi(a, b) ((a) ^= 1 << (b))       //toggles bit b in variable a
#define gbi(a, b) ((a) & (1 << (b)))      //gets bit b in variable a (masks out everything else)
#define gbis(a, b) (gbi(a, (b)) >> (b))   //gets bit b in variable a and shifts it to the LSB.

#if USE_RTC == 1
	volatile u16 secCount;
#endif

#endif //ifndef GLOBALS_H
