
#include "driver/usart/drv_usart_static.h"

//Uses U1TX and U1RX
#define RX_FREE 0
#define RX_READING 1
#define RX_OK 2

//Represents the serial read state machine
short int readState = 0;
//Saves the received bytes from serial(implemented on communication protocol)
char receivedBytes[] = {0,0,0,0};
//Saves the message received address
char address = 0;
//Saves the message received data
short int data = 0;

//Verifies if the received message implements the protocol
char verifyData(){
    short int i = 0;
    char confirmBytes[] = {0,0,0,0};
    char organizedBytes[] = {0,0,0,0};
    
    //Checks 4 bytes message
    for(i = 0; i < 4; i++){
        //Search for the first byte message
        if(((receivedBytes[i] >> 6) & 0x03) == 0){
            confirmBytes[0] = 1;
            organizedBytes[0] = receivedBytes[i];
        }
        //Search for the second byte message
        else if(((receivedBytes[i] >> 6) & 0x03) == 1){
            confirmBytes[1] = 1;
            organizedBytes[1] = receivedBytes[i];
        }
        //Search for the third byte message
        else if(((receivedBytes[i] >> 6) & 0x03) == 2){
            confirmBytes[2] = 1;
            organizedBytes[2] = receivedBytes[i];
        }
        //Search for the fourth byte message
        else if(((receivedBytes[i] >> 6) & 0x03) == 3){
            confirmBytes[3] = 1;
            organizedBytes[3] = receivedBytes[i];
        }
        //If the bytes is out of the pattern
        else 
            return 0;
    }
    
    //If the bytes is on protocol pattern
    if((confirmBytes[0] & confirmBytes[1] & confirmBytes[2] & confirmBytes[3]) == 1){
        short int recBytes[] = {organizedBytes[1], organizedBytes[2], organizedBytes[3]};
        
        //Takes the address data
        address = ((organizedBytes[0] << 2) & 0b11111100) | (((organizedBytes[1] << 2) >> 6) & 0b00000011);
        //Takes the data
        data = ((recBytes[0] << 12) & 0b1111000000000000) | ((recBytes[1] << 6) & 0b0000111111000000) | (recBytes[2]& 0b0000000000111111);
        
        return 1;
    }

    return 0;
}

//Reads the RPI data
short int readRPI(){
    //If contains byte on rx
    if (!DRV_USART1_ReceiverBufferIsEmpty())
    {
        //Read byte
        receivedBytes[readState] = DRV_USART1_ReadByte();
        //Controls the read state machine
        if(readState != 3)
            readState++;
        else{
            readState = 0;
            
            //Confirms read
            if(verifyData() == 1)
                return RX_OK;
            
            //Data not agree
            else
                return RX_READING;
            
            
        }
        return RX_READING;
    }
    return RX_FREE;
}

//Writes data on RPI
void writeRPI(char address, short data){
    char sendBytes[] = {0,0,0,0};
    char dataH = data>>8;
    
    //Put data on message pattern
    sendBytes[0] = (address>>2) & 0b00111111;
    sendBytes[1] = 0b01000000 | ((address << 4) & 0b00110000) | ((dataH >> 4) & 0b00001111);
    sendBytes[2] = 0b10000000 | ((dataH << 2) & 0b00111100) | ((data >> 6) & 0b00000011);
    sendBytes[3] = 0b11000000 | (data & 0b00111111);
    
    //Sends over serial
    int i;
    for(i = 0; i < 4; i++)
        DRV_USART1_WriteByte(sendBytes[i]);
}

char getAddress(){
    return address;
}

short int getData(){
    return data;
}