#ifndef BODEN_H_
#define BODEN_H_

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void AutoCalibration(int* LED, int* Schwellwerte);
int lesenMultiplexerUnten(int s0, int s1, int s2, int s3);
int lesenMultiplexerUnten2(int s0, int s1, int s2, int s3);
int lesenMultiplexerUnten3(int s0, int s1, int s2, int s3);
void bodenlesen(bool& minEinerDa,int* LED, int* Schwellwerte, bool* Photo);
void bodenverarbeiten(bool* gesehenSensor,bool& minEinerDa,bool* Photo, double& bodenrichtung, int* LED, Adafruit_BNO055& gyro,bool& buttonGpressed, int* Schwellwerte, double& minus, long& alteZeit, int& alterWinkel, double& rotation, double &addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwart, bool& hBall, bool& surface);
void Boden(bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation,double &addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwartbool,bool& hBall, bool& surface);
double bodenrichtungszuweisung(int n);
bool onLine(int* LED, int* Schwellwerte);

#endif