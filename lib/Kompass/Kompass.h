#ifndef KOMPASS_H_
#define KOMPASS_H_

#include <Adafruit_BNO055.h>

double compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel, double& addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwart);

#endif