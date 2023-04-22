#include<Kompass.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel, double& addRot, bool piread, int pixyG) {
  double p,d;
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
  if(piread){
    // winkel=-pixyG/1.8;
    if(pixyG<0){
      pixyG+=360;
    }
    //Serial.print(pixyG);Serial.print("    ");Serial.print(winkel-minus);Serial.print("  =  ");Serial.println(pixyG+winkel);
    double buf=-pixyG-winkel;
    // while(buf>=360){
    //   buf-=360;
    // }while(buf<0){
    //   buf+=360;
    // }
    Serial.print(buf-minus);Serial.print("   ");Serial.print(minus);Serial.print("   ");Serial.print(pixyG);Serial.print("   ");Serial.println(winkel);
    winkel=winkel-minus+buf;
    p = 6;                                                          //korrekturfaktor
    d = 60;                                                          //korrekturfaktor
  }else{
    winkel = winkel - minus + addRot;
    p = 11;                                                          //korrekturfaktor
    d = 50;                                                          //korrekturfaktor
  }
  while (winkel > 180) {                                                       //Werte umrechnen von 0-359 auf Werte von -180 - +180 => für Formel
    winkel = winkel - 360;
  }
  while (winkel < -180) {
    winkel = winkel + 360;
  }
  rotation = (p * winkel) - d * rotationSpeed;   //Berechnung der drehung
  alterWinkel = winkel;
  rotation = -rotation / 4.5;
}