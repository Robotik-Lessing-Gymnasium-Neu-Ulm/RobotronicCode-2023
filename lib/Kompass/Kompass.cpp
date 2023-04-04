#include<Kompass.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel, double& addRot) {
  sensors_event_t orientationData;                                          //momentane Aufnahmeder der Sensorwerte
  sensors_event_t angVelocityData;
  gyro.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  gyro.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);           //holt neue Werte in Grad
  double winkel = orientationData.orientation.x;         //variable winkel enthält Drehung auf der Ebene in Grad
  double rotationSpeed = angVelocityData.orientation.z;
  if (buttonGpressed) {                                                     //wenn Button gedrückt speichern des Offsets
    minus = winkel;
    Serial.print("MINUS:");
    Serial.println(minus);
    buttonGpressed = false;
  }
  winkel = winkel - minus +addRot;
  if (winkel > 180) {                                                       //Werte umrechnen von 0-359 auf Werte von -180 - +180 => für Formel
    winkel = winkel - 360;
  }
  if (winkel < -180) {
    winkel = winkel + 360;
  }
  double p = 11;                                                          //korrekturfaktor
  double d = 50;                                                          //korrekturfaktor
  rotation = (p * winkel) - d * rotationSpeed;   //Berechnung der drehung
  alterWinkel = winkel;
  rotation = -rotation / 4.5;
}