#include <Motoren.h>
#include <Arduino.h>

void motor (double dir, double velocity, double rotation, uint8_t M1_FW,uint8_t M1_RW,uint8_t M1_PWM, uint8_t M2_FW,uint8_t M2_RW,uint8_t M2_PWM, uint8_t M3_FW,uint8_t M3_RW,uint8_t M3_PWM, uint8_t M4_FW,uint8_t M4_RW,uint8_t M4_PWM) {
  // dir Umrechnen ins Bogenmaß
  double phi = (dir / 180) * PI;
  // Aufteilen des damit aufgespannten Vektors auf die 4 Motoren
  double m4 = sin(phi - 5 * PI / 4) * velocity + rotation; //VR
  double m1 = sin(phi - 7 * PI / 4) * velocity + rotation; //VL
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