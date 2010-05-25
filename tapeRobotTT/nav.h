//navigation related functions
//
//will be commented later
//
//

#define kTHRESHOLD_HIGH 130
#define kTHRESHOLD_LOW  70

void getOutOfStartBox(); //does an S move to get out of the box
void makeRightTurn(u08 r);
void turnTheCorner(); 
void plowTheCenter();
void countLines(u08 sensor,u08 numLines); // counts till the sensor hits the black
void waitLines(u08 sensor,u08 numLines); // waits till the sensor is off the line
void squareBackSensors();
int  findInnerSquare();
void move(s08 speed);
void reverse(u08 speed);
void brake();
void spinRight(s08 delta);
