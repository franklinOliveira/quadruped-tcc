#include "app.h"

//Define the movement types of quadruped
#define STOP 0
#define FORWARD 1
#define TURN_LEFT 2
#define TURN_RIGHT 3

//Movement type D1 (OC3)
#define MOVD1_TRIS TRISDbits.TRISD2
#define MOVD1 PORTDbits.RD2
//Movement type D0 (OC1)
#define MOVD0_TRIS TRISDbits.TRISD0
#define MOVD0 PORTDbits.RD0

APP_DATA appData;

//Saves actually movement type
char currentMovement = 0;
//Sinalizes if movement is finish
char movementFlag = 0;

void APP_Initialize ( void )
{    
    appData.state = APP_STATE_INIT;
    
    //Set movement type data pins as input
    MOVD1_TRIS = 1;
    MOVD0_TRIS = 1;
}

void APP_Tasks ( void )
{   
    //If 00 state stop the robot
    if((!MOVD1)&(!MOVD0))
        currentMovement = STOP;
    
    //If 01 state moves the robot forward
    else if((!MOVD1)&MOVD0)
        currentMovement = FORWARD;
    
    //If 10 state turns robot right
    else if(MOVD1&(!MOVD0))
        currentMovement = TURN_LEFT;
    
    //If 11 state turns robot left
    else if(MOVD1&MOVD0)
        currentMovement = TURN_RIGHT;
    
    //Executes the movement
    move(currentMovement);
    
    //Sends stop data to RPI
    if(currentMovement == STOP){
        writeRPI(1, 0);
        writeRPI(2, 0);
    }
    
    //Sends move data to RPI
    else if(getMovementState() == 1 && movementFlag == 0){
        if(currentMovement == FORWARD){
            writeRPI(1, getStep());
            writeRPI(2, 0);
        }
        else if(currentMovement == TURN_RIGHT || currentMovement == TURN_LEFT){
            writeRPI(1, 0);
            writeRPI(2, getSpin());
        }

        movementFlag = 1;
    }
    else if(getMovementState() != 1 && movementFlag == 1)
        movementFlag = 0;
    
    __delay_ms(50);
}
