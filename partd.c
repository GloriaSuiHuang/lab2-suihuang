/*
 * Dash_Or_Dot.c
 *
 * Created: 9/26/2021 12:59:37 PM
 * Author : glori
 */ 
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

/*--------------------Variables---------------------------*/


char String[25];

volatile int count = 0;
//volatile int time_interval_High = 0;
volatile int time_interval = 0;
volatile int T = 0;
volatile int edge_End = 0;
int fallingedge = 0;


//int firstTime = 1;


void initialize(){
	UART_init(BAUD_PRESCALER);
	cli();//disable global interrupts
	
	//GPIO Pins Setup
	DDRB |= (1<<DDB5);//SET PB5 to be output pin
	DDRB &= ~(1<<DDB0);//set PB0 to be input pin
	//PORTB |= (1<<PORTB5);//Enable internal pull up resistor
	
	//Prescale 16MHz System Clock by 8 to 2MHz
	//CLKPR = (1<<CLKPCE);//Enable changes to CLKPS bits
	//Divide by 8
	//CLKPR = (1<<CLKPS1);
	//CLKPR = (1<<CLKPS0);
	
	//Set Timer 1 clock to be internal div by 256 to 62500 Hz
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS12);
	TCCR1B &= ~(1<<CS11);
	
	TCNT1 = 0;
	
	//Looking for falling edge, active low, button pressed
	TCCR1B &= ~(1<<ICES1);
	//Clear input capture flag,
	TIFR1 |= (1<<ICF1);
	
	//Enable input capture interrupt, the correspnding interrupt vector
	//is exected when the ICF1 flag is set.
	
	TIMSK1 |= (1<<ICIE1);//if enable(ICIE1=1), the input capture flag
	//generates an input capture interrupt.
	TIMSK1 |= (1<<TOIE0);//Enable Overflow Interrupt
	
	
	
	sei();//Enable global interrupts
	
}

/*-----------------------------------------------------------*/
ISR(TIMER1_CAPT_vect){
	//volatile int edge_End = ICR1-1;//?
		
		if( fallingedge ){
			if(count==0){
			
				time_interval = ICR1 - edge_End + 65536*count + 1 ;
			
				if(time_interval>=1875 && time_interval<=12500){
					sprintf(String,"A dot \n");
					//sprintf(String,"%d \n", time_interval);
					UART_putstring(String);
				}else if(time_interval>12500 && time_interval<=25000){
					sprintf(String,"A Dash \n");
					//sprintf(String,"%d \n", time_interval);
					UART_putstring(String);
				}
				fallingedge =0;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}else{
				sprintf(String,"Too long \n");
				UART_putstring(String);
				fallingedge =0;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}
		}else{
			if(count==0){
				
				time_interval = ICR1 - edge_End + 65536*count + 1 ;
				//sprintf(String,"%d \n", time_interval);
				
				if(time_interval>25000){
					sprintf(String,"A space \n");
					UART_putstring(String);
				
				}
				fallingedge =1;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}else{
				sprintf(String,"A space \n");
				UART_putstring(String);
				fallingedge =1;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}
		}
		
		
	
	
}

ISR(TIMER1_OVF_vect){
	
	count++;
	
}

/*-----------------------------------------------------------*/
int main(void)
{
    /* Replace with your application code */
	//UART_init(BAUD_PRESCALER);
	initialize();
    while (1) {}
		
		
    
}

