/*
 * button_debouncer.h
 *
 * Created: 25/11/2018 01:54:47
 *  Author: daagu
 */ 


#ifndef BUTTON_DEBOUNCER_H_
#define BUTTON_DEBOUNCER_H_

typedef void (*button_callback)();

typedef struct button_debouncer_t{
	unsigned long last_interrupt_time;
	button_callback button_action;
}button_debouncer_t;

void init_button_interrupts();
void debounce_button(button_debouncer_t *button);

#endif /* BUTTON_DEBOUNCER_H_ */