/*
 * button_debouncer.c
 *
 * Created: 25/11/2018 01:54:21
 *  Author: daagu
 */ 
#include <avr/io.h>
#include "button_debouncer.h"
#include "millis.h"

//on rising edge
void init_button_interrupts()
{
	EIMSK |= (1 << INT0) | (1 << INT1); //enable external nterrupt 0
	EICRA = 0x0F; //make INTO and INT1 rising edge
}

void debounce_button(button_debouncer_t *button)
{
	 unsigned long interrupt_time = millis();
	 // If interrupts come faster than 200ms, assume it's a bounce and ignore
	 if (interrupt_time - button->last_interrupt_time > 200)
	 {
		button->button_action();
	 }
	 button->last_interrupt_time = interrupt_time;	
}
