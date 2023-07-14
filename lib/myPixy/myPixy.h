#ifndef MY_PIXY_H_
#define MY_PIXY_H_

#include <Pixy2I2C.h>


double Pixy(Pixy2I2C& pixy,bool& piread,int& TorHoehe);
double Pixy2(Pixy2I2C& pixy2,bool& piread2, int& TorHoehe2);
void position(Pixy2I2C& pixy,bool& piread, int& TorHoehe,Pixy2I2C& pixy2,bool& piread2,int &TorHoehe2,int &xH, int &yH, int &xV, int &yV);
#endif