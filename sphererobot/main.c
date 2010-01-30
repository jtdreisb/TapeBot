// This program tests the LCD printing functions with an assortment of numbers.
// After verifying the output, advance to the next test by pressing the button.
// Patrick McCarty, 5-09-2009

#include "../XiphosLibrary/globals.h"

#define DELAY 10

int main()
{
	u08 i = 0;
	initialize();

	//Test 1
	clearScreen();
	printString("   Xiphos 1.0   ");
	lowerLine();
	printString(" RoboAddict.com");
	delayMs(1000);
	
	for(i = 0; i <= DELAY ;i++)
	{
	  clearScreen();
	  printString("T minus : ");
	  
	  printPlain_u08(DELAY-i);
	  delayMs(1000);
	}  
	clearScreen();
	printString("__(Sphere-bot)___");
	//softReset();
   /*motor0(200); // 0 at 127. full forward at 255
   delayMs(3000);
   brake0();*/
   while(1);
}
