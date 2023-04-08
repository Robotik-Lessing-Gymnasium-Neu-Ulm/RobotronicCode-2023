#ifndef CONTROL_LED_S_H_
#define CONTROL_LED_S_H_
#include<Arduino.h>

void ControlLEDs(bool& buttonGpressed, double &richtung,double& IRbest, int& Icball, double& rotation,bool& minEinerDa, bool& irAutoCalibration, int* LED, int* Schwellwerte,  uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t ButtonII, uint8_t ButtonIII, uint8_t ButtonIV, uint8_t LEDir, uint8_t LEDballcaught, uint8_t LEDgyro, uint8_t Lichtschranke, char& PhaseLSKalibration, int minWertLS);

#endif