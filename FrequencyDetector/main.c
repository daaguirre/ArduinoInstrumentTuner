/*
 * FrequencyDetector.c
 *
 * Created: 21/11/2018 18:19:24
 * Author : daagu
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lcd_display.h"
#include "frequency_detector.h"
#include "button_debouncer.h"
#include "millis.h"


static int freq_key_index = 0;
button_debouncer_t next_key_button, previous_key_button;

static void previous_key_button_action()
{
	freq_key_index--;
	freq_key_writer(&freq_key_index);	
}

static void next_key_button_action()
{
	freq_key_index++;
	freq_key_writer(&freq_key_index);
}

int main(void)
{
	//USART_init();
	millis_init();
	lcd_display_init();
	frequency_detector_init();
	init_button_interrupts();
	
	next_key_button.button_action = next_key_button_action;
	previous_key_button.button_action = previous_key_button_action;
	
	sei();//enable interrupts

    while (1) 
    {
		if (new_frequency_detected()){
			  float frequency = get_frequency();//calculate frequency timer rate/period
			  char str[80];
			  sprintf(str, "Frequency = %d Hz\n", (int)frequency);
			  draw_actual_frequency((int)frequency);
		}
		_delay_ms(500);
    }
}

//when new ADC value ready
ISR(ADC_vect) 
{
	frequency_detector_update(ADCH);
}

ISR(INT0_vect) //ISR for external interrupt 0
{
	debounce_button(&next_key_button);
}

//code for changing motor speed with button
//it's commented because motor speed is being controlled with potentiometer
ISR(INT1_vect)
{
	debounce_button(&previous_key_button);
}

ISR(TIMER0_COMPA_vect)
{
	update_millis();
}











