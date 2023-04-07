#ifndef BETTER_DEFINES_H_
#define BETTER_DEFINES_H_
#include<Arduino.h>

const uint8_t M1_FW=0;
const uint8_t M1_RW=2;
const uint8_t M1_PWM =1;

const uint8_t M2_FW =3; //richtig
const uint8_t M2_RW= 5;
const uint8_t M2_PWM= 4;

const uint8_t M3_FW = 6;  //richtig!
const uint8_t M3_RW =8;
const uint8_t M3_PWM= 7;

const uint8_t M4_FW = 9 ; //richtig!
const uint8_t M4_RW =11;
const uint8_t M4_PWM= 10;

const int breite= 182;
const int laenge= 200;

//Multiplexer Unten
const uint8_t S0= 17;
const uint8_t S1= 16;
const uint8_t S2= 15;
const uint8_t S3= 14;

const uint8_t UAM1 =A15; //analog Multiplexer Unten 1
const uint8_t UAM2= A16; //analog Multplexer Unten 2
const uint8_t UAM3 =A14; // analog Multiplexer Unten 3
const uint8_t AM1= A17; //analog Multiplexer Oben

const uint8_t ButtonIV= 2;
const uint8_t ButtonIII =3;
const uint8_t ButtonII =4;
const uint8_t ButtonI= 5;

const uint8_t VR =1; //m1
const uint8_t VL =2; //m2
const uint8_t HR= 3; //m3
const uint8_t HL= 4; //m4

//Control LEDs
const uint8_t LEDboden =13;
const uint8_t LEDir    =37;
const uint8_t LEDgyro =17;
const uint8_t LEDballcaught =   20;

const int Nah =25;
const int Mittel= 50;
const int Fern =70;

#endif