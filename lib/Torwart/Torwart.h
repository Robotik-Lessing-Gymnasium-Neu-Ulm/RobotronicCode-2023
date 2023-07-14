#ifndef TORWART_H_
#define TORWART_H_

#include<Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#include <myPixy.h>
#include<PID_v1.h>

// void torwartProgramm(Pixy2I2C& pixy2,int* LED,int* Schwellwerte, double rotation,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot, bool piread2,bool piread, int pixyG2, int& PixyG, int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave, bool hatBall, bool& torwart, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa, bool* Photo,bool* gesehenSensor,double& bodenrichtung,long& alteZeit,double &entfSet,unsigned long& addRotTime,bool& surface,double& accel);
void torwartProgramm(Pixy2I2C& pixy2,bool piread2,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot,double rotation,int& PixyG,bool hatBall,double& accel,bool &minEinerDa, int *LED, int *Schwellwerte, bool *Photo, bool *gesehenSensor, double &bodenrichtung,
long &alteZeit,bool piread,int PixyG2, bool hBall, bool torwart, bool &surface,int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave,
PID &entfPID, PID &wiPID, double &offsetVorne, double &entfVelo, double &wiVelo,double &entfSet,unsigned long &addRotTime, int &TorHoehe2);

#endif