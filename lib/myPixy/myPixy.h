#ifndef MY_PIXY_H_
#define MY_PIXY_H_

#include <Pixy2I2C.h>


double Pixy(Pixy2I2C& pixy,bool& piread);
double Pixy2(Pixy2I2C& pixy2,bool& piread2);
void position(double AbstandX, double AbstandY,Pixy2I2C& pixy2,bool& piread2,Pixy2I2C& pixy,bool& piread);
#endif