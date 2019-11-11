#include "driver/usart/drv_usart_static.h"
#include "math.h"
#include<stdio.h>

//Values----------------------------
//Circle radius
#define R 35
//------------------------------------

//PIN PMD12
#define SCLK_TRIS TRISDbits.TRISD12
#define SCLK PORTDbits.RD12

//PIN PMD14
#define SDIO_TRIS TRISDbits.TRISD6
#define SDIO PORTDbits.RD6

//Plan values
float SpositionY = 0, SpositionX = 0;
int Sorientation = 0;

//Odometry starts
bool started = false;

//Converts two complement values
int removeTwoComplement(char value) {
    if (value > 127)
        return value - 256;
    else return value;
}

//Converts pixels to cm
float toMillimeters(int value){
    float valuef = (float)value;
    return (valuef * 0.03175)*100;
}

//Find the robot orientation angle
float angle(float x1, float y1, float x2, float y2){
    
    float deltax = (x2 - x1);
    float deltay = (y2 - y1);

    float d = sqrt(pow((deltax), 2) + pow((deltay), 2));
    float angle = (2*asin((d/2)/R))*57.2958;
    return angle;
}

//Sends address to sensor
void sendAddress(char address) {

    address = address & 0b01111111;
    //Set the SCLK and SDIO register as output
    SCLK_TRIS = 0;
    SDIO_TRIS = 0;

    //Informs the current bit position to send.
    short int bitNumber;

    //Send 8 address bit (7 6 5 4 3 2 1 0)
    for (bitNumber = 7; bitNumber >= 0; bitNumber--) {
        //Clock signal in '0'
        SCLK = 0;
        
        //Checks if the current bit is '1' or '0' and send it.
        if (address & 0b10000000)
            SDIO = 1;
        else 
            SDIO = 0;

        //Clock signal in '1'(PULSE)
        SCLK = 1;

        //Shift the address to left, taking the next bit.
        address = address << 1;
    }

}

//Receives sensor data
char receiveData() {
    //Set the SCLK register as output and SDIO register as input
    SCLK_TRIS = 0;
    SDIO_TRIS = 1;

    //Informs the current bit position to send.
    int bitNumber;

    //The read data
    char data = 0x00;

    //Send 8 address bit (7 6 5 4 3 2 1 0)
    for (bitNumber = 7; bitNumber >= 0; bitNumber--) {
        //Clock signal in '0'
        SCLK = 0;
        //Clock signal in '1' (PULSE)
        SCLK = 1;
        //Shift data to left to receiver the new bit.
        data = data << 1;

        //Put '1' in LSB if the SDIO receives high logic signal.
        if (SDIO)
            data = data | 0b00000001;
        else
            data = data & 0b11111110;
    }

    return data;
}

void initOdometry(){
    SpositionX = 0;
    SpositionY = 0;
    Sorientation = 0;
        
    //RESYNC
    SCLK_TRIS = 0;
    SCLK = 0;
    __delay_us(1);
    SCLK = 1;
    __delay_us(1600);

}

void updateMotion() {
    char motion = 0, deltaX = 0, deltaY = 0;

    //Read motion register
    sendAddress(0x02);
    __delay_ms(1);
    motion = receiveData();

    //If is motion
    if ((motion & 0b10000000) == 0b10000000) {

        //Read delta y register
        sendAddress(0x04);
        __delay_ms(1);
        deltaY = receiveData();

        //Read delta x register
        sendAddress(0x03);
        __delay_ms(1);
        deltaX = receiveData();
            
        float x0 = SpositionX, y0 = SpositionY;
            
        //Get the position
        if (deltaY != 0x00) 
            SpositionY += toMillimeters(removeTwoComplement(deltaY));

        if (deltaX != 0x00) 
            SpositionX += toMillimeters(removeTwoComplement(deltaX));
            
        SensorToPlan(angle(x0, y0, SpositionX, SpositionY), x0, y0, SpositionX, SpositionY);
            
    }
} 

short int getStep(){
    return 4;
}

short int getSpin(){
    return 11;
}