#ifndef BODEN_H_
#define BODEN_H_

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void AutoCalibration(int* LED, int* Schwellwerte, uint8_t LEDboden, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3);
int lesenMultiplexerUnten(int s0, int s1, int s2, int s3, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM);
void bodenlesen(bool& minEinerDa,int* LED, int* Schwellwerte, bool* Photo, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3);
void Boden(bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t M1_FW, uint8_t M1_RW, uint8_t M1_PWM, uint8_t M2_FW, uint8_t M2_RW, uint8_t M2_PWM, uint8_t M3_FW, uint8_t M3_RW, uint8_t M3_PWM, uint8_t M4_FW, uint8_t M4_RW, uint8_t M4_PWM);
double bodenrichtungszuweisung(int n);
void bodenverarbeiten(bool* gesehenSensor,bool& minEinerDa,bool* Photo, double& bodenrichtung, int* LED, Adafruit_BNO055& gyro,bool& buttonGpressed, int* Schwellwerte, double& minus, long& alteZeit, int& alterWinkel, double& rotation, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2, uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t M1_FW, uint8_t M1_RW, uint8_t M1_PWM, uint8_t M2_FW, uint8_t M2_RW, uint8_t M2_PWM, uint8_t M3_FW, uint8_t M3_RW, uint8_t M3_PWM, uint8_t M4_FW, uint8_t M4_RW, uint8_t M4_PWM);


#endif