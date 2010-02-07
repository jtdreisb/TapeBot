// This program tests the LCD printing functions with an assortment of numbers.
// After verifying the output, advance to the next test by pressing the button.
// Patrick McCarty, 5-09-2009

#include "../XiphosLibrary/globals.h"

#define DELAY 10

int main()
{
	u08 i = 0;
	u08 p = 0;
	initialize();

	//Test 1
	clearScreen();
	printString("   Xiphos 1.0   ");
	lowerLine();
	printString(" RoboAddict.com");
	delayMs(1000);
	
	/*while( p < 127 )
	{
	  lowerLine();
	  print_u08(p);
	  servo(0,p);
	  p++;
	  delayMs(10);
	  while( p > 0)
	  {
	    lowerLine();
	    print_u08(p);
	    servo(0,p);
	    p--;
	    delayMs(20);
	  }
	} */
	
	servo(0,0);
	delayMs(1000);
	servo(0,127);
	delayMs(1000);
	servo(0,255);
	p = 255;
	while(p > 0)
	{
	  servo(0,p);
	  p -= 5;
	  delayMs(50);
	}
	 
	clearScreen();
	//printString("__(Sphere-bot)___");
	//softReset();
   /*motor0(200); // 0 at 127. full forward at 255
   delayMs(3000);
   brake0();*/
   while(1);
}
