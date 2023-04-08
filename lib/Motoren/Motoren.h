#ifndef MOTOREN_H_
#define MOTOREN_H_

#include<Arduino.h>

void motor (double dir, double velocity, double rotation, uint8_t M1_FW,uint8_t M1_RW,uint8_t M1_PWM, uint8_t M2_FW,uint8_t M2_RW,uint8_t M2_PWM, uint8_t M3_FW,uint8_t M3_RW,uint8_t M3_PWM, uint8_t M4_FW,uint8_t M4_RW,uint8_t M4_PWM);

#endif