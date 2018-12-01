# ArduinoInstrumentTuner
Arduino instrument tuner based on the work of amandaghassaei.

#Introduction
From the LCD display the user will be able to select a frequency (100hz, 200hz or ) or even a guitar string (that has a frequency bonded). 
Connecting a microphone to the audio jack (1/4”) and making the sound near the microphone the Arduino will be able to capture the sound and determine its frequency.
Because there is not instrument available the audio of a video from youtube where a guitar is being tuned will be input to Arduino in order to detect the different 
frequencies of each string and knowing if the guitar is being tuned properly. At the same time the user will be able to see the frequency of the captured sound in the LCD display. 
There are two buttons that the user can use to change the pre-saved frequencies. 
 
Because the guitar tuning is being used for the demo the pre-saved target frequencies will be the strings of the guitar:
1 (E)	329.63 Hz
2 (B)	246.94 Hz
3 (G)	196.00 Hz
4 (D)	146.83 Hz
5 (A)	110.00 Hz
6 (E)	82.41 Hz

Therefore, the user will use both buttons (next and previous) to navigate through the strings and select the string to tune. 

#Audio input
An audio signal is composed of sine waves of different frequencies. Normally these waves are centered on 0V so that the amplitude of these will vary from negative voltage values to positive.  
With the Analog to Digital converter (ADC) that is found on the Arduino board we can only measure positives values in the range of 0 – 5V. 
It’s because of this that we need to apply and offset to center the waves in 2.5V (half of the valid range) and be able of capturing the audio signal with the ADC.
Another problem of audio signals is that their amplitude is usually very small (in the order of mV). In order to make easier the detection of the sound’s frequency it’s necessary to amplify 
the audio signal for having amplitudes between 1V and 2V. In this way an audio signal centered at 2.5V will have valid values between 0 - 5V. 
In order to apply this offset and amplification the next circuit will be implemented: 
 
From this circuit we can distinguish two parts:
	Amplification phase: this phase amplificates the sound signal using an operational amplifier in non-inverting configuration. In this way we have a 100 kΩ resistor 
	on the feedback loop between the output and the negative input. A potentiometer is placed in the negative input to regulate the gain. The input audio signal is connected 
	to the positive input of the op. amp.  so that we get in the output the input signal multiplied by the gain. The equation to obtain the gain in this configuration is as follows:
V_out= V_in∙(1+R_2/R_1 )

Where R2 is the 100 kΩ resistor and R1 the 10 kΩ potentiometer. In this way if we have an audio signal with an amplitude of 200 mV and the potentiometer at its maximum value (10 kΩ), 
the next gain will be applied:
V_out= 0.2∙(1+100/10)=2.2V
	Offset phase: this part of the circuit generates a DC offset of  +2.5V. This is done connecting two 100 kΩ resistors in serie from the 5V output to the GND of the Arduino. 
	With the help of the 10 µF capacitor the audio signal amplified is summed up to the DC offset. In this way an audio signal amplified 11 times and centered at 2.5V is input in the 
	Arduino through the pin A0. 

#Frequency detector algorithm
The frequency detection of signal can be done in different ways applying techniques in the time domain or frequency domain (FFT-Fast Fourier Transform).  
Algorithms that work in the time domain of the signal are more light-weight than the ones that use frequency domain techniques. In order to keep it simple an 
algorithm that uses a time domain technique will be implemented. 
The most common algorithm to the detect the frequency of a signal is to count the time passed between two events obtaining in this way a period. 
This event usually is triggered when the signal crosses the 0V with a positive slope. Because the audio signal is centered in 2.5V the vent will be triggered every time the signal 
crosses the 2.5V value with positive slope. Then the time between events will be measured obtaining in this way the period of the signal. 
A problem that is usually present in an audio signal is that the wave is formed by sine waves of different frequencies, that is something that can corrupt the values of 
frequency obtained with the algorithm. To avoid this problem, every time the event is triggered the slope of the signal will be saved and the different slopes compared among them. 
Then the maximum slope will be chosen, and the time will be counted until another slope matches the maximum one. If a new maximum slope is identified the time counter will be reset. 
This frequency detection algorithm can be found in the file “frequency_detector.c” in the function “frequency_detector_update()”.
The ADC will be configured to sample the audio signal at a rate of 38462 Hz. This is done configuring the ADC clock with a prescales of 1/32. This means that the ADC clock will work at a frequency of:
16MHz/32=500kHz
Because the ADC module takes 13 clock cycles to obtain a new sample:
500kHz/13=38461.5 Hz

Also, the ADC interrupts are enabled generating an interrupt every time a new sample is ready so that the interrupt will be executed at rate of 38462 Hz.
Button debounce
Because buttons are being used is necessary to apply a software algorithm in order to debounce both buttons and avoid undesired software behaviors. 
Every time there is a rising edge on the button signal (button pressed) an interrupt will be generated. Because of bounces more than one interrupts may be 
generated in one press. So that the time between interrupts will be counted using a millis function (like the one defined in Arduino library). If the time passed between 
interrupts is less than 200 ms means that the button is bouncing and the software action linked to the button will not be executed. If the time passed is greater that 200 ms 
the action will be executed. 
Since this is a common problem a library has been created in the files “button_debouncer.h” and “button_debouncer.c”. This library defines the next C-Struct:
 
This struct define a variable to save the last time an interrupt of this button was generated, the other struct member is a function pointer to a function that no return any value and does not take arguments. When the button is created a function (has to be static) has to assigned to this function pointer. The assigned function will be executed every time the button is pressed. 

