//navigation related functions
//
//will be commented later
//
//

#define kTHRESHOLD_HIGH 100
#define kTHRESHOLD_LOW  70

void getOutOfStartBox(); //does an S move to get out of the box
void innerSquare();
void crossPattern();
void makeRightTurn(u08 r);
void turnTheCornerRight();
void turnTheInnerCornerRight();
void plowTheCenter(u08 lines);
void plowTheInner(u08 lines);
void countLines(u08 sensor,u08 numLines); // counts till the sensor hits the black
void waitLines(u08 sensor,u08 numLines); // waits till the sensor is off the line
void squareBackSensors(u08 brake);
int  findInnerSquare();
void move(s08 speed);
void reverse(u08 speed);
void brake();
void spin90Left();
void spinRight(s08 delta);
void smoothAccelerate(u08 topSpeed);