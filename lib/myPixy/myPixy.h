#ifndef MY_PIXY_H_
#define MY_PIXY_H_

#include <Pixy2I2C.h>


double Pixy(Pixy2I2C& pixy,bool& piread,int&  TorHoehe);
double Pixy2(Pixy2I2C& pixy2,bool& piread2,int&  TorHoehe2);
void position(double WinkelToreGes, double AbstandX, double AbstandY,Pixy2I2C& pixy2,bool& piread2,Pixy2I2C& pixy,bool& piread,int&  TorHoehe,int&  TorHoehe2);
#endif