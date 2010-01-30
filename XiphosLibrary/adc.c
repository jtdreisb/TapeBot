

#include "globals.h"

//Normally called only by the initialize() function in utility.c.
void adcInit()
{
       //set analog source as AVCC with capacitor and left shift data
       ADMUX = _BV(REFS0) | _BV(ADLAR);

       //enable ADC and set prescalar to /128
       ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1)| _BV(ADPS0);
}

//Returns an 8-bit resolution reading from analog input number <num>.
//where <num> can be 0 to 7.
//Each conversion takes 104us = 62.5ns * 13 cycles * 128 prescalar
u08 analog(u08 num)
{
       //validate num parameter and return early if num is out of range
       if (num > 7)
               {
               return 0xBD;
               }

       //clear lower 5 bits and set left shifting
       ADMUX = _BV(REFS0) | _BV(ADLAR);
       //select the analog input to read
       ADMUX |= num;

       //start conversion
       ADCSRA |= _BV(ADSC);
       //wait for conversion to complete
       loop_until_bit_is_clear(ADCSRA, ADSC);

       //return 8-bit result
       return ADCH;
}

//Returns a 10-bit resolution reading from analog input number <num>,
//where <num> can be 0 to 7.
u16 analog10(u08 num)
{
       //validate num parameter and return early if num is out of range
       if (num > 7)
               {
               return 0x0BAD;
               }

       //clear lower 5 bits and set right shifting
       ADMUX = _BV(REFS0);
       //select the analog input to read
   ADMUX |= num;

       //start conversion
       ADCSRA |= _BV(ADSC);
       //wait for conversion to complete
       loop_until_bit_is_clear(ADCSRA, ADSC);

       //lower 8 bits of result must be read first
       u08 temp = ADCL;
       //combine the high and low bits and return result as 16-bit number.
       return ((u16)ADCH<<8) | temp;
}
