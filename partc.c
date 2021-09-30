/*
 * interrupt.c
 *
 * Created: 9/26/2021 1:08:29 AM
 * Author : glori
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//int count = 0;
//volatile int edge = 0;

void initialize(){
	
	cli();//disable global interrupts
	
	//GPIO Pins Setup
	DDRB |= (1<<DDB5);//SET PB5 to be output pin
	DDRB &= ~(1<<DDB0);//set PB0 to be input pin
	//PORTB |= (1<<PORTB5);//Enable internal pull up resistor
	
	//Enable the clock prescale by 1
	TCCR1B |= (1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	//Looking for falling edge
	TCCR1B &= ~(1<<ICES1);
	//Clear input capture flag,
	TIFR1 |= (1<<ICF1);
	
	//Enable input capture interrupt, the correspnding interrupt vector
	//is exected when the ICF1 flag is set.
	
	TIMSK1 |= (1<<ICIE1);//if enable(ICIE1=1), the input capture flag
	//generates an input capture interrupt.
	
	
	sei();//Enable global interrupts
	
}
ISR(TIMER1_CAPT_vect){
	
	PORTB ^= (1<<PORTB5);
	TCCR1B ^= (1<<ICES1);//looking for rising edge
	//count ++;
}

int main(void)
{
	/* Replace with your application code */
	
	initialize();
	while (1) {}
}

//Enable overflow interrupt
//TIMSK1 |= (1<<TOIE1);

//edge = ICR1;
//When a capture is triggered, the 16-bit value of the counter (TCNT1)
// is written to the input capture register (ICR1).
//The input capture flag (ICF1) is set at the same system clock
//as the TCNT1 value is copied into ICR1 register.
//If enabled (ICIE1 = 1), the input capture flag generates an input
//capture interrupt. The ICF1 flag is automatically cleared when the
//interrupt is executed.


