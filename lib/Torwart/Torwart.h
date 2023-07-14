#ifndef TORWART_H_
#define TORWART_H_

#include<Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#include <myPixy.h>
#include<PID_v1.h>

void torwartProgramm(Pixy2I2C& pixy2,int* LED,int* Schwellwerte, double rotation,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot, bool piread2,bool piread, int pixyG2, int& PixyG, int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave, bool hatBall, bool& torwart, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa, bool* Photo,bool* gesehenSensor,double& bodenrichtung,long& alteZeit,double &entfSet,unsigned long& addRotTime,bool& surface,double& accel, int& TorHoehe);

#endif