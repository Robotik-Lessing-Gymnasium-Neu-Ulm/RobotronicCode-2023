#ifndef DEFINES_H_
#define DEFINES_H_

#include <EEPROM.h>

#define M1_FW 26 //richtig
#define M1_RW 28
#define M1_PWM 29

#define M2_FW 12 //richtig
#define M2_RW 25
#define M2_PWM 24

#define M3_FW  9  //richtig!
#define M3_RW 11
#define M3_PWM 10

#define M4_FW  6  //richtig!
#define M4_RW 8
#define M4_PWM 7

#define breite 182
#define laenge 200

//Multiplexer Unten
#define S0 33
#define S1 34
#define S2 35
#define S3 36

#define UAM1 A17 //analog Multiplexer Unten 1
#define UAM2 A15 //analog Multplexer Unten 2
#define UAM3 A16 // analog Multiplexer Unten 3
#define AM1 A14 //analog Multiplexer Oben

#define ButtonIV 2
#define ButtonIII 3
#define ButtonII 4
#define ButtonI 5

#define VR 1 //m1
#define VL 2 //m2
#define HR 3 //m3
#define HL 4 //m4

//Control LEDs
#define LEDIV 13
#define LEDII    17
#define LEDIII  37
#define LEDI    20

#define Nah 25
#define Mittel 50
#define Fern 70

#define LichtSchranke A2

#define Roboter EEPROM.read(0)  //255: lila, sonst anderer
#define LILA 255

#define Schuss_FW 23
#define Schuss_RW 21
#define Schuss_PWM 22

#define SuperTeamBT 30

#endif