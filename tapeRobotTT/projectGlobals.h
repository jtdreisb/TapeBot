//This file is for global defines specific to this project.
//It enables multiple project folders to share a single copy of the Xiphos library,
//without needing to modify the library itself to disable unused features.
//Users can also add their own custom #defines or macros to this file.

#ifndef PROJECTGLOBALS_H
#define PROJECTGLOBALS_H

//User configuration options to enable/disable board features.
//Disabling or reducing unused functionality can improve performance.
#define USE_LCD     1
#define USE_ADC     1
#define USE_MOTOR0  1
#define USE_MOTOR1  1
#define NUM_SERVOS  8 //valid values: 0-8
#define USE_I2C     0
#define USE_SPI     1
#define USE_UART0   1
#define USE_UART1   1
#define USE_RTC     1

//Custom user code should go here

#define TBSENSOR_IR_FRONT 2
#define TBSENSOR_IR_RIGHT 1
#define TBSENSOR_IR_LEFT  0

#endif //PROJECTGLOBALS_H
