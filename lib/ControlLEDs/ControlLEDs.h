#ifndef CONTROL_LED_S_H_
#define CONTROL_LED_S_H_

#include<Arduino.h>

void ControlLEDs(bool& buttonGpressed, double &richtung,double& IRbest, int& Icball, double& rotation,bool& minEinerDa, bool& irAutoCalibration, bool& IRsave);
void wave(uint8_t pin[],size_t s, int time);
void blink(uint8_t pin[],size_t s, int time);

#endif