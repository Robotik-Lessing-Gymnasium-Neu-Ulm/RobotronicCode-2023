#ifndef KOMPASS_H_
#define KOMPASS_H_

#include <Adafruit_BNO055.h>

void compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel, double& addRot, bool pireads, int PixyG);

#endif