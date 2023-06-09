#ifndef MOTOREN_H_
#define MOTOREN_H_

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

unsigned int motor (double dir, double velocity, double rotation);
void fahren(double dir, double velocity, double rotation, Adafruit_BNO055& gyro,bool& buttonGpressed, bool& RetSurface);

#endif