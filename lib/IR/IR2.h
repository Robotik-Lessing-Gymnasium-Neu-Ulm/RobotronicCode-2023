#ifndef IR_H_
#define IR_H_

#include<PID_v1.h>

void irAutoCal(int *minWert, bool& irAutoCalibration);
int lesenMultiplexerOben(int s0, int s1, int s2, int s3);
void IRsens(int* IR, double& IRbest, int& Icball, double& richtung,double &entfSet, double &wiIn, PID &wiPID, int* minWert, bool& irAutoCalibration, double& addRot, double& WinkelBall, unsigned long& addRotTime, bool& torwart, bool& IRsave);

#endif