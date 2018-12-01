/*
 * millis.c
 *
 * Created: 25/11/2018 02:43:37
 *  Author: daagu
 */ 

#include <avr/interrupt.h>

static unsigned long millis_value = 0;

void millis_init()
{
	TCCR0A |= (1 << WGM01);
	TCNT0 = 0x00;
	//TIFR0 |= (1 << OCF0A);
	OCR0A = 0xF9;
	TCCR0B |= (1 << CS00) | (1 << CS01);
	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect
}

unsigned long millis()
{
	unsigned long value;
	cli();
	value = millis_value;
	sei();
	return value;
}

void update_millis()
{
	millis_value++;
}