#include<Kompass.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include<Defines.h>

void compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel, double& addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwart) {
  double p,d;
  sensors_event_t orientationData;                                          //momentane Aufnahmeder der Sensorwerte
  sensors_event_t angVelocityData;
  gyro.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  gyro.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);           //holt neue Werte in Grad
  double winkel = orientationData.orientation.x;                            //variable winkel enthält Drehung auf der Ebene in Grad
  double rotationSpeed = angVelocityData.orientation.z;
  if (buttonGpressed) {                                                     //wenn Button gedrückt speichern des Offsets
    minus = winkel;
    Serial.print("MINUS:");
    Serial.println(minus);
    buttonGpressed = false;                                                 //automatisch terminieren
  }
  if(piread&&hatBall){                                                      //wenn die vordere Kamera beachtet werden soll
    if(Roboter==LILA){
      //winkel=-pixyG/1.8;                                                   //die "schlechtere Drehung"
      int buf=-pixyG;
      int buf2=winkel-minus;
      if(pixyG<180){
        pixyG+=360;
      }
      while(buf>=180){
        buf-=360;
      }while(buf<-180){
        buf+=360;
      }
      while(buf2>=180){
        buf2-=360;
      }while(buf2<-180){
        buf2+=360;
      }
      Serial.print(buf);Serial.print("   ");Serial.print(buf2); Serial.print("    ");Serial.println(buf-buf2);
      winkel=winkel+buf-buf2;
      p = 4.8;                                                                  //korrekturfaktor (diese sind noch nicht perfekt)
      d = 60;                                                                 //korrekturfaktor
    }else{  //black
      int buf=-pixyG;
      int buf2=winkel-minus;
      if(pixyG<180){
        pixyG+=360;
      }
      while(buf>=180){
        buf-=360;
      }while(buf<-180){
        buf+=360;
      }
      while(buf2>=180){
        buf2-=360;
      }while(buf2<-180){
        buf2+=360;
      }
      //Serial.print(buf);Serial.print("   ");Serial.print(buf2); Serial.print("    ");Serial.println(buf-buf2);
      winkel=winkel+buf-buf2;
      p = 3.5;                                                                  //korrekturfaktor (diese sind noch nicht perfekt)
      d = 45;                                                                 //korrekturfaktor
    }
  }else if(torwart){
    //winkel=-pixyG2/1.8;                                                   //die "schlechtere Drehung"
    int buf=-pixyG2;
    int buf2=winkel-minus;
    if(pixyG2<180){
      pixyG+=360;
    }
    while(buf>=180){
      buf-=360;
    }while(buf<-180){
      buf+=360;
    }
    while(buf2>=180){
      buf2-=360;
    }while(buf2<-180){
      buf2+=360;
    }
    //Serial.print(buf);Serial.print("   ");Serial.print(buf2); Serial.print("    ");Serial.println(buf-buf2);
    winkel=winkel+buf-buf2;
    p = 5;                                                                  //korrekturfaktor (diese sind noch nicht perfekt)
    d = 50;                                                                  //korrekturfaktor
  }else{
    winkel = winkel - minus + addRot;
    p = 11;                                                                 //korrekturfaktor
    d = 50;                                                                 //korrekturfaktor
  }
  while (winkel > 180) {                                                    //Werte umrechnen von 0-359 auf Werte von -180 - +180 => für Formel
    winkel = winkel - 360;
  }
  while (winkel < -180) {
    winkel = winkel + 360;
  }
  rotation = (p * winkel) - d * rotationSpeed;                              //Berechnung der drehung
  alterWinkel = winkel;
  rotation = -rotation / 4.5;
}