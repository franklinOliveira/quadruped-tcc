
#include "driver/usart/drv_usart_static.h"

//Uses U2TX
short int movement_state = 0;

//Forward state machine
char* getForwardMovement() {
    if (movement_state == 0)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 1)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1329 #18 p2010 #12 p2010 #13 p1670 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 2)
        return "#0 p1500 #1 p1500 #2 p1202 #16 p1500 #17 p1329 #18 p2223 #12 p1797 #13 p1670 #14 p1500 #28 p776 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 3)
        return "#0 p1500 #1 p1500 #2 p1202 #16 p1500 #17 p1500 #18 p2223 #12 p1797 #13 p1500 #14 p1500 #28 p776 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 4)
        return "#0 p1500 #1 p1670 #2 p1202 #16 p1500 #17 p1500 #18 p2223 #12 p1797 #13 p1500 #14 p1500 #28 p776 #29 p1329 #30 p1500 t100\r\n";
    else if (movement_state == 5)
        return "#0 p1500 #1 p1670 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1329 #30 p1500 t100\r\n";

}

//Left state machine
char* getT_LeftMovement() {
    if (movement_state == 0)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 1)
        return "#0 p1500 #1 p1670 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1329 #30 p1500 t100\r\n";
    else if (movement_state == 2)
        return "#0 p1500 #1 p1670 #2 p989 #16 p1500 #17 p1500 #18 p1840 #12 p1840 #13 p1500 #14 p1500 #28 p989 #29 p1329 #30 p1500 t100\r\n";
    else if (movement_state == 3)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1500 #18 p1840 #12 p1840 #13 p1500 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 4)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1329 #18 p1840 #12 p1840 #13 p1670 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 5)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1329 #18 p2010 #12 p2010 #13 p1670 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
}

//Right state machine
char* getT_RightMovement() {
    if (movement_state == 0)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 1)
        return "#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1329 #18 p2010 #12 p2010 #13 p1670 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 2)
        return "#0 p1500 #1 p1500 #2 p1159 #16 p1500 #17 p1329 #18 p2010 #12 p2010 #13 p1670 #14 p1500 #28 p1159 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 3)
        return "#0 p1500 #1 p1500 #2 p1159 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1478 #14 p1500 #28 p1159 #29 p1500 #30 p1500 t100\r\n";
    else if (movement_state == 4)
        return "#0 p1500 #1 p1670 #2 p1159 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1478 #14 p1500 #28 p1159 #29 p1329 #30 p1500 t100\r\n";
    else if (movement_state == 5)
        return "#0 p1500 #1 p1670 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1478 #14 p1500 #28 p989 #29 p1329 #30 p1500 t100\r\n";
}

//Executes the movement state machine
void move(char type){
    
    //Refresh movement state
    if(movement_state != 5)
        movement_state++;
    else
        movement_state=0;
    
    //Stop move
    if(type == 0){
        DRV_USART0_WriteString("#0 p1500 #1 p1500 #2 p989 #16 p1500 #17 p1500 #18 p2010 #12 p2010 #13 p1500 #14 p1500 #28 p989 #29 p1500 #30 p1500 t100\r\n");
        movement_state = 1;
    }
    //Forward move
    else if(type == 1)
        DRV_USART0_WriteString(getForwardMovement());
    //Turn left move
    else if(type == 2)
        DRV_USART0_WriteString(getT_LeftMovement());
    //Turn right move
    else if(type == 3)
        DRV_USART0_WriteString(getT_RightMovement());      
        
}

//Returns the movement state
short int getMovementState(){
    return movement_state;
}
