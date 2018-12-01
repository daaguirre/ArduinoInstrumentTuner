/*
 * lcd_display.c
 *
 * Created: 22/11/2018 19:23:40
 *  Author: daagu
 */ 

#define D0 eS_PORTD5//D0
#define D1 eS_PORTD6//D1
#define D2 eS_PORTD7//D2
#define D3 eS_PORTB0//D3
#define D4 eS_PORTB1//D4
#define D5 eS_PORTB2//D5
#define D6 eS_PORTB3//D6
#define D7 eS_PORTB5//D7
#define RS eS_PORTD1//B0
#define EN eS_PORTD4//B1

#include "lcd.h"
#include "lcd_display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_FREQ_KEYS		6

freq_key_t guitar_string_1 = {"STR 1E","329.63"};
freq_key_t guitar_string_2 = {"STR 2B","246.94"};
freq_key_t guitar_string_3 = {"STR 3G","196.00"};
freq_key_t guitar_string_4 = {"STR 4D","146.83"};
freq_key_t guitar_string_5 = {"STR 5A","110.00"};
freq_key_t guitar_string_6 = {"STR 6E","82.41"};
	
static freq_key_t freq_keys_map[6];// = {guitar_string_e, guitar_string_b};

void freq_map_init()
{	
	freq_keys_map[0] = guitar_string_1;
	freq_keys_map[1] = guitar_string_2;
	freq_keys_map[2] = guitar_string_3;
	freq_keys_map[3] = guitar_string_4;
	freq_keys_map[4] = guitar_string_5;
	freq_keys_map[5] = guitar_string_6;
}

void freq_key_writer(int* index)
{
	if(*index >= MAX_FREQ_KEYS) *index = 0;
	if(*index < 0) *index = 5;
	Lcd8_Set_Cursor(1,0);
	Lcd8_Write_String("                ");//CLEAR row
	
	Lcd8_Set_Cursor(1,FREQ_KEY_NAME_POS_ON_DISPLAY);
	Lcd8_Write_String(freq_keys_map[*index].name);
	Lcd8_Set_Cursor(1,FREQ_KEY_VALUE_POS_ON_DISPLAY);
	Lcd8_Write_String(freq_keys_map[*index].freq_value);
}

void lcd_display_init()
{
	DDRB = 0xFF;
	DDRD = 0xFF;

	Lcd8_Init();
	freq_map_init();
	Lcd8_Clear();
	freq_key_writer(0);
	Lcd8_Set_Cursor(2,0);
	Lcd8_Write_String("Freq:");
}

void draw_menu()
{
	//Lcd8_Set_Cursor(1,0);
	//Lcd8_Write_String("Set Freq:");
	Lcd8_Set_Cursor(2,0);
	Lcd8_Write_String("Freq:");
}

void draw_target_frequency(unsigned int target_frequency)
{
	Lcd8_Set_Cursor(1,9);
	char target_freq_str[4];
	sprintf(target_freq_str,"%d",target_frequency);
	Lcd8_Write_String(target_freq_str);
}

void draw_actual_frequency(unsigned int target_frequency)
{			  
	Lcd8_Set_Cursor(2,9);
	Lcd8_Write_String("       ");
	char target_freq_str[4];
	sprintf(target_freq_str,"%d",target_frequency);
	Lcd8_Set_Cursor(2,9);
	Lcd8_Write_String(target_freq_str);
}