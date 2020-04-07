/*
 * SPWM.c
 *
 * Created: 4/3/2015 5:22:27 PM
 *  Author: Shuvangkar Chandra das
 * email: s.c.das@ieee.org
 * microcontroller: atmega8
 *Fuse bit: Low Fuse: EF , High Fuse: C9
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

char channel,x;
uint16_t sampleA=0,sampleB =0;

//Look up table for sine wave 
const uint16_t sinewave[] PROGMEM =
{0, 0, 10, 15, 20, 24, 29, 34, 39, 44, 49, 53, 58, 63, 68, 72, 77, 82, 86, 91, 95, 100, 104, 109, 113, 117, 
122, 126, 130, 134, 138, 142, 146, 150, 154, 158, 162, 165, 169, 173, 176, 179, 183, 186, 189, 192, 196, 199,
201, 204, 207, 210, 212, 215, 217, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 242, 243,
244, 245, 246, 247, 247, 248, 248, 249, 249, 249, 249, 249, 249, 249, 248, 248, 247, 247, 246, 245, 244, 243,
242, 241, 240, 238, 237, 235, 234, 232, 230, 228, 226, 224, 222, 220, 217, 215, 212, 210, 207, 204, 201, 199, 
196, 192, 189, 186, 183, 179, 176, 173, 169, 165, 162, 158, 154, 150, 146, 142, 138, 134, 130, 126, 122, 117, 
113, 109, 104, 100, 95, 91, 86, 82, 77, 72, 68, 63, 58, 53, 49, 44, 39, 34, 29, 24, 20, 15, 0, 0};

int main(void)
{
	DDRB |=1<<PB1;    //declare OC1A as output mode
	DDRC |= 1<<PC5;	  //declare PC5 pin as output mode
	init_fast_pwm_interrupt(); //initializing PWM 
	channel=0;
	sampleA=0;
    while(1)	
    {
        
    }
}

ISR(TIMER1_COMPA_vect)
{	
	switch(channel) // When compare match found, compare value will change here
	{
		case 0:
					OCR1A  = pgm_read_byte(&sinewave[sampleA]);
					OCR1B = 0;
					PORTB &=~(1<<PB0);
					PORTC =(1<<PC5);

		
				break;
		case 1:
					OCR1B = pgm_read_byte(&sinewave[sampleA]);
					OCR1A = 0 ;
					PORTB =(1<<PB0);
					PORTC &=~(1<<PC5);

				break;
	}
	sampleA++;
	if (sampleA>=160)
	{
		sampleA=0;
		channel ^=1;
 		DDRB ^=1<<PB0;
 		DDRC ^=(1<<PC5);
 		DDRB ^=1<<PB1;
 		DDRB ^=1<<PB2;		
	}

}
init_fast_pwm_interrupt()
{
		
	TCCR1B |=1<<CS10;   //timer pwm is activated with prescaler 1
	TCCR1A |= 1<<WGM11;	//fast pwm
	TCCR1B |= 1<<WGM13;	//fast pwm
	TCCR1B |= 1<<WGM12;	//fast pwm
	ICR1 = 249;			//Top value ICR1=(F_cpu/(Prescaler*Frefuency))-1
	TCCR1A |=1<<COM1A1;	//Non-Invertin mode pwm
	TCCR1A |=1<<COM1B1;	//Non-Invertin mode pwm
	OCR1A =  0;	
	OCR1B = 0;	
	sei();				//global interrupt enable
	TIMSK |= 1<<OCIE1A; //Timer/Counter1, Output Compare A Match Interrupt Enable
	
}

