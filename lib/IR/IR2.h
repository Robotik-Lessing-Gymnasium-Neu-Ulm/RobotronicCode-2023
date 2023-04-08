#ifndef IR_H_
#define IR_H_

#include<PID_v1.h>
#include<Arduino.h>

int lesenMultiplexerOben(int s0, int s1, int s2, int s3, uint8_t S0, uint8_t S1,uint8_t S2,uint8_t S3,uint8_t AM1);
void IRsens(int* IR, double& IRbest, int& Icball, double& richtung,double &entfSet, double &wiIn, PID &wiPID, int* minWert, bool& irAutoCalibration, double& addRot, double& WinkelBall, unsigned long& addRotTime, bool& torwart, uint8_t S0, uint8_t S1,uint8_t S2,uint8_t S3,uint8_t AM1);

#endif