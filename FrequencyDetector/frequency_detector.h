/*
 * frequency_detector.h
 *
 * Created: 24/11/2018 20:21:42
 *  Author: daagu
 */ 


#ifndef FREQUENCY_DETECTOR_H_
#define FREQUENCY_DETECTOR_H_

#include <stdbool.h>
#include <stdint.h>

void frequency_detector_init();
void frequency_detector_update(uint8_t new_data);
bool new_frequency_detected();
float get_frequency();

#endif /* FREQUENCY_DETECTOR_H_ */