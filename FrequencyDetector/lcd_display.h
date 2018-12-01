/*
 * lcd_display.h
 *
 * Created: 22/11/2018 19:22:58
 *  Author: daagu
 */ 


#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#define FREQ_KEY_NAME_POS_ON_DISPLAY	0
#define FREQ_KEY_VALUE_POS_ON_DISPLAY	9

typedef struct freq_key_t{
	char name[FREQ_KEY_VALUE_POS_ON_DISPLAY];
	char freq_value[16-FREQ_KEY_VALUE_POS_ON_DISPLAY+1];
}freq_key_t;

void freq_key_writer(int *index);
void lcd_display_init();
void draw_menu();
void draw_target_frequency(unsigned int target_frequency);
void draw_actual_frequency(unsigned int target_frequency);


#endif /* LCD_DISPLAY_H_ */