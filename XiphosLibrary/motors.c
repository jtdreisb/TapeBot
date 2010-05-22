//Code for controlling two brushed DC motors

#include "globals.h"

//Normally called only by the initialize() function in utility.c.
void motorInit()
{
       // Set up Timer 1 in 8-bit fast PWM mode with the prescaler at I/O clock / 64,
       // with the PWM in normal polarity (meaning motor on or brake on is logic 1)
       TCCR1A = (1 << WGM10);
       TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12);

       #if USE_MOTOR0 == 1
               //configure INA and INB as outputs
               DDRE |= _BV(7) | _BV(6);
               //configure PWM as an ouput
               sbi(DDRB,5);

               //Set OC1A (output compare unit 1A) for fast PWM, normal polarity
               TCCR1A |= (1 << COM1A1);
               //Set the duty cycle to zero
               OCR1AH = 0;
               OCR1AL = 0;
       #endif

       #if USE_MOTOR1 == 1
               //configure INA and INB as outputs
               DDRE |= _BV(3) | _BV(2);//4 3
               //configure PWM as an ouput
               sbi(DDRB,6);

               //Set OC1B (output compare unit 1B) for fast PWM, normal polarity
               TCCR1A |= (1 << COM1B1);
               //Set the duty cycle to zero
               OCR1BH = 0;
               OCR1BL = 0;
       #endif
}

//don't compile the motor 0 functions if motor 0 is not enabled (prevents accidental use)
#if USE_MOTOR0 == 1
//Sets motor speed and direction for Motor 0 according to the following key.
//0 = full speed reverse
//127 = glide to stop
//254 = full speed forward
//255 = invalid
void motor0(u08 speedAndDirection)
{
       //glide to a stop
       if (speedAndDirection == 127)
       {
               //***TODO: disable the interrupt watching for a DIAGA/DIAGB fault condition

               //set PWM to lowest duty cycle
               OCR1AL = 0;

               //disable the H-bridge chip by driving the combined DIAGA/DIAGB low
               //configure the combined DIAGA/DIAGB as an output
               sbi(DDRE,5);
               //drive DIAGA/DIAGB low
               cbi(PORTE,5);
       }
       else
       {
               //***TODO: enable the interrupt watching for a DIAGA/DIAGB fault condition

               //enable the H-bridge chip by pulling the combined DIAGA/DIAGB high
               //configure the combined DIAGA/DIAGB as an input
               cbi(DDRE,5);
               //enable pullup resistor to pull the combined DIAGA/DIAGB high
               sbi(PORTE,5);

               //drive forward
               if (speedAndDirection > 127)
               {
                       //set INA high and INB low to drive "clockwise"
                       PORTE = (PORTE & ~(_BV(6) | _BV(7))) | _BV(7);

                       //Set the duty cycle by writing to the output compare register for Timer 1,
                       //which is the duty cycle register when the timer is in 8-bit fast PWM mode.
                       //Subtract 128 from the parameter to produce a range of 0-127 from 128-255.
                       //Multiply the new 0-127 range by 2 to make it 0-254
                       //Add 1 to make the final range 1-255
                       OCR1AL = ((speedAndDirection - 128) * 2) + 1;
               }
               //drive backward
               else
               {
                       //set INA low and INB high to drive "counterclockwise"
                       PORTE = (PORTE & ~(_BV(6) | _BV(7))) | _BV(6);

                       //Set the duty cycle by writing to the output compare register for Timer 1,
                       //which is the duty cycle register when the timer is in 8-bit fast PWM mode.
                       //Subtract the parameter from 127 to produce a range of 127-1 from 0-126.
                       //Multiply the new 127-1 range by 2 to make it 254-2
                       OCR1AL = (127 - speedAndDirection) * 2;
               }
       }
}

//Shorts both of motor0's terminals to ground to oppose motor0 movement
void brake0()
{
       //set PWM to highest duty cycle
       OCR1AL = 255;
       //set INA low and INB low to brake to GND
       PORTE &= ~(_BV(6) | _BV(7));
}
#endif //USE_MOTOR0 == 1


//don't compile the motor 1 functions if motor 1 is not enabled (prevents accidental use)
#if USE_MOTOR1 == 1
//Sets motor speed and direction for Motor 1 according to the following key.
//0 = full speed reverse
//127 = glide to stop
//254 = full speed forward
//255 = invalid
void motor1(u08 speedAndDirection)
{
       //glide to a stop
       if (speedAndDirection == 127)
       {
               //***TODO: disable the interrupt watching for a DIAGA/DIAGB fault condition

               //set PWM to lowest duty cycle
               OCR1BL = 0;

               //disable the H-bridge chip by driving the combined DIAGA/DIAGB low
               //configure the combined DIAGA/DIAGB as an output
               sbi(DDRE,4);//2
               //drive DIAGA/DIAGB low
               cbi(PORTE,4);//2
       }
       else
       {
               //***TODO: enable the interrupt watching for a DIAGA/DIAGB fault condition

               //enable the H-bridge chip by pulling the combined DIAGA/DIAGB high
               //configure the combined DIAGA/DIAGB as an input
               cbi(DDRE,4);//2
               //enable pullup resistor to pull the combined DIAGA/DIAGB high
               sbi(PORTE,4);//2

               //drive forward
               if (speedAndDirection > 127)
               {
                       //set INA high and INB low to drive "clockwise"
                       PORTE = (PORTE & ~(_BV(3) | _BV(2))) | _BV(2);//3 4 4

                       //Set the duty cycle by writing to the output compare register for Timer 1,
                       //which is the duty cycle register when the timer is in 8-bit fast PWM mode.
                       //Subtract 128 from the parameter to produce a range of 0-127 from 128-255.
                       //Multiply the new 0-127 range by 2 to make it 0-254
                       //Add 1 to make the final range 1-255
                       OCR1BL = ((speedAndDirection - 128) * 2) + 1;
               }
               //drive backward
               else
               {
                       //set INA low and INB high to drive "counterclockwise"
                       PORTE = (PORTE & ~(_BV(3) | _BV(2))) | _BV(3);//3 4 3

                       //Set the duty cycle by writing to the output compare register for Timer 1,
                       //which is the duty cycle register when the timer is in 8-bit fast PWM mode.
                       //Subtract the parameter from 127 to produce a range of 127-1 from 0-126.
                       //Multiply the new 127-1 range by 2 to make it 254-2
                       OCR1BL = (127 - speedAndDirection) * 2;
               }
       }
}

//Shorts both of motor1's terminals to ground to oppose motor1 movement
void brake1()
{
       //set PWM to highest duty cycle
       OCR1BL = 255;
       //set INA low and INB low to brake to GND
       PORTE &= ~(_BV(2) | _BV(3));//3 4
}
#endif //USE_MOTOR1 == 1

