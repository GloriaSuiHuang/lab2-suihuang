/*
 * lab2_suihuang_Polling.c
 *
 * Created: 9/22/2021 6:53:46 PM
 * Author : glori
 */ 

#include <avr/io.h>


void initialize(){
	DDRB &= ~(1<<DDB0);
	DDRB |= (1<<DDB5);
}
int main(void)
{
    /* Replace with your application code */
	initialize();
	
    while (1) 
    {
		while(PINB & (1<<PINB0));
		PORTB |= (1<<PORTB5);
		
		
    }
}

