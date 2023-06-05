#include <Motoren.h>
#include <Arduino.h>

#ifndef M1_FW
#define M1_FW 26 //richtig
#endif
#ifndef M1_RW
#define M1_RW 28
#endif
#ifndef M1_PWM
#define M1_PWM 29
#endif

#ifndef M2_FW
#define M2_FW 12 //richtig
#endif
#ifndef M2_RW
#define M2_RW 25
#endif
#ifndef M2_PWM
#define M2_PWM 24
#endif

#ifndef M3_FW
#define M3_FW  9  //richtig!
#endif
#ifndef M3_RW
#define M3_RW 11
#endif
#ifndef M3_PWM
#define M3_PWM 10
#endif

#ifndef M4_FW
#define M4_FW  6  //richtig!
#endif
#ifndef M4_RW
#define M4_RW 8
#endif
#ifndef M4_PWM
#define M4_PWM 7
#endif

void motor (double dir, double velocity, double rotation) {
  // dir Umrechnen ins Bogenmaß
  double phi = (dir / 180) * PI;
  // Aufteilen des damit aufgespannten Vektors auf die 4 Motoren
  double m4 = sin(phi - 5 * PI / 4) * velocity + rotation; //VR
  double m1 = sin(phi - 7 * PI / 4) * velocity + rotation; //VL     //LILA ist so m2 Black anders bei m4,m1,m2,m3
  double m2 = sin(phi - PI / 4) * velocity + rotation;     //HL
  double m3 = sin(phi - 3 * PI / 4) * velocity + rotation; //HR
  //Aufteilen in Drehrichtungen
  //VL
  if (m1 < 0) {
    digitalWrite(M1_FW, HIGH);
    digitalWrite(M1_RW, LOW);
    analogWrite(M1_PWM, abs(m1));
  }
  if (m1 > 0) {
    digitalWrite(M1_FW, LOW);
    digitalWrite(M1_RW, HIGH);
    analogWrite(M1_PWM, abs(m1));
  }
  if (m1 == 0) {
    digitalWrite(M1_FW, HIGH);
    digitalWrite(M1_RW, HIGH);
    analogWrite(M1_PWM, 255);
  }
  // VR
  if (m2 < 0) {
    digitalWrite(M2_FW, HIGH);
    digitalWrite(M2_RW, LOW);
    analogWrite(M2_PWM, abs(m2));
  }
  if (m2 > 0) {
    digitalWrite(M2_FW, LOW);
    digitalWrite(M2_RW, HIGH);
    analogWrite(M2_PWM, abs(m2));
  }
  if (m2 == 0) {
    digitalWrite(M2_FW, HIGH);
    digitalWrite(M2_RW, HIGH);
    analogWrite(M2_PWM, 255);
  }
  // HR
  if (m3 < 0) {
    digitalWrite(M3_FW, HIGH);
    digitalWrite(M3_RW, LOW);
    analogWrite(M3_PWM, abs(m3));
  }
  if (m3 > 0) {
    digitalWrite(M3_FW, LOW);
    digitalWrite(M3_RW, HIGH);
    analogWrite(M3_PWM, abs(m3));
  }
  if (m3 == 0) {
    digitalWrite(M3_FW, HIGH);
    digitalWrite(M3_RW, HIGH);
    analogWrite(M3_PWM, 255);
  }
  // HL
  if (m4 < 0) {
    digitalWrite(M4_FW, HIGH);
    digitalWrite(M4_RW, LOW);
    analogWrite(M4_PWM, abs(m4));
  }
  if (m4 > 0) {
    digitalWrite(M4_FW, LOW);
    digitalWrite(M4_RW, HIGH);
    analogWrite(M4_PWM, abs(m4));
  }
  if (m4 == 0) {
    digitalWrite(M4_FW, HIGH);
    digitalWrite(M4_RW, HIGH);
    analogWrite(M4_PWM, 255);
  }
}