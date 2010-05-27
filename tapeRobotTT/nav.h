//navigation related functions
//
//will be commented later
//
//

#define kTHRESHOLD_HIGH 100
#define kTHRESHOLD_LOW  70

#define TBSENSOR_IR_FRONT 2
#define TBSENSOR_IR_RIGHT 1
#define TBSENSOR_IR_LEFT  0


//High level (called by Main)
void getOutOfStartBox(); //does an S move to get out of the box
void innerSquare();
void crossPattern();
void followEdge();


//primatives (called by above)
void countLines(u08 sensor,u08 numLines); // counts till the sensor hits the black
void makeRightTurn(u08 r);
void turnTheCornerRight();
void turnTheInnerCornerRight();
void plowTheCenter(u08 lines);
void plowTheInner(u08 lines);
void squareBackSensors(u08 brake);
int  findInnerSquare();
void spin90Left();

