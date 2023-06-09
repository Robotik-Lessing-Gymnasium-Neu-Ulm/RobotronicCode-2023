#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include<PID_v1.h>

#ifndef BALLVERFOLGUNG_H_
#define BALLVERFOLGUNG_H_

void verfolgeBall(double& IRbest, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation,double &addRot, bool piread, int PixyG, int PixyG2, bool hBall, bool torwart, int* IR, int& Icball, double& richtung,double &entfSet, double &wiIn, int* minWert, bool& irAutoCalibration, double& WinkelBall, unsigned long& addRotTime, bool& IRsave, bool& surface);

#endif