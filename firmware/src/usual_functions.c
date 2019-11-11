#include "driver/usart/src/drv_usart_static_local.h"
#include "driver/usart/drv_usart_static.h"


void DRV_USART0_WriteString(char *text){
    int counter;
    int text_size = strlen(text);
    for(counter = 0; counter < text_size; counter++)
        DRV_USART0_WriteByte(text[counter]);
}

void DRV_USART1_WriteString(char *text){
    int counter;
    int text_size = strlen(text);
    for(counter = 0; counter < text_size; counter++)
        DRV_USART1_WriteByte(text[counter]);
}

void __delay_ms(unsigned int ms){
	
	unsigned long int j = 0; 
	for(ms; ms > 0; ms--){
		for(j = 0; j < 3200; j++){
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
		}
	}
   
}

void __delay_us(unsigned int us){
	
	unsigned long int j = 0; 
	for(us; us > 0; us--){
		for(j = 0; j < 3; j++){
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
			asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
		}
	}
   
}



