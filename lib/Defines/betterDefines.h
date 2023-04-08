#ifndef BETTER_DEFINES_H_
#define BETTER_DEFINES_H_
#include <Arduino.h>

uint8_t M1_FW = 0;
uint8_t M1_RW = 2;
uint8_t M1_PWM = 1;

uint8_t M2_FW = 3;
uint8_t M2_RW = 5;
uint8_t M2_PWM = 4;

uint8_t M3_FW = 6;
uint8_t M3_RW = 8;
uint8_t M3_PWM = 7;

uint8_t M4_FW = 9;
uint8_t M4_RW = 11;
uint8_t M4_PWM = 10;

int breite = 182;
int laenge = 200;

// Multiplexer Unten
uint8_t S0 = 17;
uint8_t S1 = 16;
uint8_t S2 = 15;
uint8_t S3 = 14;

uint8_t UAM1 = A15; // analog Multiplexer Unten 1
uint8_t UAM2 = A16; // analog Multplexer Unten 2
uint8_t UAM3 = A14; // analog Multiplexer Unten 3
uint8_t AM1 = A17;  // analog Multiplexer Oben

uint8_t ButtonIV = 2;
uint8_t ButtonIII = 3;
uint8_t ButtonII = 4;
uint8_t ButtonI = 5;

uint8_t VR = 1; // m1
uint8_t VL = 2; // m2
uint8_t HR = 3; // m3
uint8_t HL = 4; // m4

// Control LEDs
uint8_t LEDboden = 13;
uint8_t LEDir = 37;
uint8_t LEDgyro = 17;
uint8_t LEDballcaught = 20;

int Nah = 25;
int Mittel = 50;
int Fern = 70;

#endif