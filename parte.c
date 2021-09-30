/*
 * parte.c
 *
 * Created: 9/30/2021 12:01:22 PM
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
#include <string.h>
#include "uart.h"

/*--------------------Variables---------------------------*/

char astr[6];
char String[25];

volatile int count = 0;
//volatile int time_interval_High = 0;
volatile int time_interval = 0;
volatile int T = 0;
volatile int edge_End = 0;
int fallingedge = 0;
int red = 0;//dot
int green = 0;//dash

//int firstTime = 1;

void Is_Equal(){
	
	if(strcmp(astr,".-") == 0){
		sprintf(String,"A");
		UART_putstring(String);
		}else if(strcmp(astr,"-...") == 0){
		sprintf(String,"B");
		UART_putstring(String);
		}else if(strcmp(astr,"-.-.") == 0){
		sprintf(String,"C");
		UART_putstring(String);
		}else if(strcmp(astr,"-..") == 0){
		sprintf(String,"D");
		UART_putstring(String);
		}else if(strcmp(astr,".") == 0){
		sprintf(String,"E");
		UART_putstring(String);
		}else if(strcmp(astr,"..-.") == 0){
		sprintf(String,"F");
		UART_putstring(String);
		}else if(strcmp(astr,"--.") == 0){
		sprintf(String,"G");
		UART_putstring(String);
		}else if(strcmp(astr,"....") == 0){
		sprintf(String,"H");
		UART_putstring(String);
		
		}else if(strcmp(astr,"..") == 0){
		sprintf(String,"I");
		UART_putstring(String);
		}else if(strcmp(astr,".---") == 0){
		sprintf(String,"J");
		UART_putstring(String);
		}else if(strcmp(astr,"-.-") == 0){
		sprintf(String,"K");
		UART_putstring(String);
		}else if(strcmp(astr,".-..") == 0){
		sprintf(String,"L");
		UART_putstring(String);
		}else if(strcmp(astr,"--") == 0){
		sprintf(String,"M");
		UART_putstring(String);
		}else if(strcmp(astr,"-.") == 0){
		sprintf(String,"N");
		UART_putstring(String);
		}else if(strcmp(astr,"---") == 0){
		sprintf(String,"O");
		UART_putstring(String);
		}else if(strcmp(astr,".--.") == 0){
		sprintf(String,"P");
		UART_putstring(String);
		}else if(strcmp(astr,"--.-") == 0){
		sprintf(String,"Q");
		UART_putstring(String);
		}else if(strcmp(astr,".-.") == 0){
		sprintf(String,"R");
		UART_putstring(String);
		}else if(strcmp(astr,"...") == 0){
		sprintf(String,"S");
		UART_putstring(String);
		}else if(strcmp(astr,"-") == 0){
		sprintf(String,"T");
		UART_putstring(String);
		}else if(strcmp(astr,"..-") == 0){
		sprintf(String,"U");
		UART_putstring(String);
		}else if(strcmp(astr,"...-") == 0){
		sprintf(String,"V");
		UART_putstring(String);
		}else if(strcmp(astr,".--") == 0){
		sprintf(String,"W");
		UART_putstring(String);
		}else if(strcmp(astr,"-..-") == 0){
		sprintf(String,"X");
		UART_putstring(String);
		}else if(strcmp(astr,"-.--") == 0){
		sprintf(String,"Y");
		UART_putstring(String);
		}else if(strcmp(astr,"---..") == 0){
		sprintf(String,"Z");
		UART_putstring(String);
		}else if(strcmp(astr,".----") == 0){
		sprintf(String,"1");
		UART_putstring(String);
		}else if(strcmp(astr,"..---") == 0){
		sprintf(String,"2");
		UART_putstring(String);
		}else if(strcmp(astr,"...--") == 0){
		sprintf(String,"3");
		UART_putstring(String);
		}else if(strcmp(astr,"....-") == 0){
		sprintf(String,"4");
		UART_putstring(String);
		}else if(strcmp(astr,".....") == 0){
		sprintf(String,"5");
		UART_putstring(String);
		}else if(strcmp(astr,"-....") == 0){
		sprintf(String,"6");
		UART_putstring(String);
		}else if(strcmp(astr,"--...") == 0){
		sprintf(String,"7");
		UART_putstring(String);
		}else if(strcmp(astr,"----..") == 0){
		sprintf(String,"8");
		UART_putstring(String);
		}else if(strcmp(astr,"----.") == 0){
		sprintf(String,"9");
		UART_putstring(String);
		}else if(strcmp(astr,"-----") == 0){
		sprintf(String,"0");
		UART_putstring(String);
	}
	
}

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
	
	//Set Timer 1 clock to be internal div by 1024 to 15625  Hz
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS12);
	TCCR1B &= ~(1<<CS11);
	
	//TCNT1 = 0;
	
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
			
				time_interval = ICR1 - edge_End + 65536*count  + 1 ;
			
				if(time_interval>=469 && time_interval<=3125){
					red =1;
					strcat(astr,".");
					//sprintf(String,". \n");
					//sprintf(String,"%d \n", time_interval);
					//UART_putstring(String);
				}else if(time_interval>3125 && time_interval<=6250){
					green = 1;
					strcat(astr,"-");
					//sprintf(String,"- \n");
					//sprintf(String,"%d \n", time_interval);
					//UART_putstring(String);
				}
				fallingedge =0;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}else{
				
				fallingedge =0;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}
		}else{
			if(count==0){
				
				time_interval = ICR1 - edge_End + 65536*count + 1 ;
				//sprintf(String,"%d \n", time_interval);
				
				if(time_interval>6250){
					sprintf(String,"A space \n");
					UART_putstring(String);
					Is_Equal();
					strcpy(astr,"");
				}
				
				fallingedge =1;
				edge_End = ICR1;
				count = 0 ;
				TCCR1B ^= (1<<ICES1);
			}else{
				sprintf(String,"A space \n");
				UART_putstring(String);
				Is_Equal();
				strcpy(astr,"");
				
			}
			fallingedge =1;
			edge_End = ICR1;
			count = 0 ;
			TCCR1B ^= (1<<ICES1);
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
    while (1) {
		if(red){
			PORTB |= (1<<PORTB3);//turn on red LED, 11
			_delay_ms(50);
			red = 0;
			PORTB &= ~(1<<PORTB3);//turn off red LED, 11
			
			
		}
		if(green){
			PORTB |= (1<<PORTB2);//turn on green LED, 10
			_delay_ms(50);
			green = 0;
			PORTB &= ~(1<<PORTB2);//turn off green LED, 10
		}
		
	}
	
		
		
    
}



