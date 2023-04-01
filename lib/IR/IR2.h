#ifndef IR_H_
#define IR_H_

#include<PID_v1.h>

int lesenMultiplexerOben(int s0, int s1, int s2, int s3);
void IRsens(int* IR, double& IRbest, int& Icball, double& richtung, double &entfSet, double &wiIn, PID &wiPID);

#endif