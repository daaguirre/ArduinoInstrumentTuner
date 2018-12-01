/*
 * frequency_detector.c
 *
 * Created: 24/11/2018 20:21:12
 *  Author: daagu
 */ 

#include "frequency_detector.h"
#include <stdlib.h>
#include <avr/io.h>

//clipping indicator variables
uint8_t clipping = 0;

//data storage variables
uint8_t prevData = 0;
unsigned int time = 0;//keeps time and sends vales to store in timer[] occasionally
int timer[10];//sstorage for timing of events
int slope[10];//storage for slope of events
unsigned int totalTimer;//used to calculate period
unsigned int period;//storage for period of wave
uint8_t my_index = 0;//current storage index
float frequency;//storage for frequency calculations
int maxSlope = 0;//used to calculate max slope as trigger point
int newSlope;//storage for incoming slope data

//variables for decided whether you have a match
uint8_t noMatch = 0;//counts how many non-matches you've received to reset variables if it's been too long
uint8_t slopeTol = 3;//slope tolerance- adjust this if you need
int timerTol = 10;//timer tolerance- adjust this if you need

//variables for amp detection
unsigned int ampTimer = 0;
uint8_t maxAmp = 0;
uint8_t checkMaxAmp;
uint8_t ampThreshold = 30;//raise if you have a very noisy signal

static void reset(){//clea out some variables
	my_index = 0;//reset index
	noMatch = 0;//reset match couner
	maxSlope = 0;//reset slope
}

void frequency_detector_init()
{
	//clear ADCSRA and ADCSRB registers
	ADCSRA = 0;
	ADCSRB = 0;
	
	ADMUX |= (1 << REFS0); //set reference voltage
	ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
	ADCSRA |= (1 << ADATE); //enabble auto trigger
	ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
	ADCSRA |= (1 << ADEN); //enable ADC
	ADCSRA |= (1 << ADSC); //start ADC measurements
}

bool new_frequency_detected()
{
	return checkMaxAmp>ampThreshold;
}

float get_frequency()
{
	return 38462.0f/period;
}


void frequency_detector_update(uint8_t new_data)
{
	//positive slope and crossing midpoint (2.5V)
	if (prevData < 127 && new_data >=127)
	{
		newSlope = new_data - prevData;//calculate slope
		
		if (abs(newSlope-maxSlope)<slopeTol)//if slopes are ==
		{
			//record new data and reset time
			slope[my_index] = newSlope;
			timer[my_index] = time;
			time = 0;
			if (my_index == 0)//new max slope just reset
			{
				noMatch = 0;
				my_index++;//increment index
			}
			else if (abs(timer[0]-timer[my_index])<timerTol && abs(slope[0]-newSlope)<slopeTol)//if timer duration and slopes match
			{
				//sum timer values
				totalTimer = 0;
				for (uint8_t i=0;i<my_index;i++)
				{
					totalTimer+=timer[i];
				}
				period = totalTimer;//set period
				//reset new zero index values to compare with
				timer[0] = timer[my_index];
				slope[0] = slope[my_index];
				my_index = 1;//set index to 1
				noMatch = 0;
			}
			else//crossing midpoint but not match
			{
				my_index++;//increment index
				if (my_index > 9)
				{
					reset();
				}
			}
		}
		else if (newSlope>maxSlope)//if new slope is much larger than max slope
		{
			maxSlope = newSlope;
			time = 0;//reset clock
			noMatch = 0;
			my_index = 0;//reset index
		}
		else//slope not steep enough
		{
			noMatch++;//increment no match counter
			if (noMatch>9)
			{
				reset();
			}
		}
	}
	
	if (new_data == 0 || new_data == 1023){//if clipping
		clipping = 1;//currently clipping
	}
	
	time++;//increment timer at rate of 38.5kHz
	ampTimer++;//increment amplitude timer
	if (abs(127-new_data)>maxAmp){
		maxAmp = abs(127-new_data);
	}
	if (ampTimer==1000)
	{
		ampTimer = 0;
		checkMaxAmp = maxAmp;
		maxAmp = 0;
	}
	
	prevData = new_data;//store previous value
}