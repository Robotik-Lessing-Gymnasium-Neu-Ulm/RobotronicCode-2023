#include <Boden.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Motoren.h>

void AutoCalibration(int* LED, int* Schwellwerte, uint8_t LEDboden, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3) {
  int CalStatus = 0;
  int bestLED[48];
  while (CalStatus < 200) {
    //Multiplexer 1
    LED[0] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM1);
    LED[1] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM1);
    LED[2] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM1);
    LED[3] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM1);
    LED[4] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM1);
    LED[5] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM1);
    LED[6] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM1);
    LED[7] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM1);
    LED[8] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM1);
    LED[9] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM1);
    LED[10] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM1);
    LED[11] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM1);
    LED[12] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM1);
    LED[13] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM1);
    LED[14] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM1);
    LED[15] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM1);
    //Multiplexer 2
    LED[16] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM2);
    LED[17] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM2);
    LED[18] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM2);
    LED[19] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM2);
    LED[20] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM2);
    LED[21] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM2);
    LED[22] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM2);
    LED[23] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM2);
    LED[24] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM2);
    LED[25] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM2);
    LED[26] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM2);
    LED[27] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM2);
    LED[28] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM2);
    LED[29] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM2);
    LED[30] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM2);
    LED[31] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM2);
    //Multiplexer 3
    LED[32] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM3);
    LED[33] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM3);
    LED[34] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM3);
    LED[35] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM3);
    LED[36] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM3);
    LED[37] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM3);
    LED[38] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM3);
    LED[39] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM3);
    LED[40] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM3);
    LED[41] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM3);
    LED[42] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM3);
    LED[43] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM3);
    LED[44] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM3);
    LED[45] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM3);
    LED[46] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM3);
    LED[47] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM3);
    for (int i = 0; i < 48; i++) {
        bestLED[i] = 0;
        if(LED[i] > bestLED[i]){
          bestLED[i] = LED[i]; 
          Schwellwerte[i] = LED[i]+100;
        }
    }
    if(CalStatus%10){
      digitalWrite(LEDboden,LOW);
    }else{
      digitalWrite(LEDboden,HIGH);
    }
    CalStatus++;
  }
}
int lesenMultiplexerUnten(int s0, int s1, int s2, int s3, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM) {  //Verkürzung Auslesen
  digitalWrite(S0, s0);
  digitalWrite(S1, s1);
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  return analogRead(UAM);
}

void bodenlesen(bool& minEinerDa,int* LED, int* Schwellwerte, bool* Photo, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3) {
  minEinerDa = false;
  //Multiplexer 1
  LED[0] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM1);
  LED[1] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM1);
  LED[2] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM1);
  LED[3] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM1);
  LED[4] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM1);
  LED[5] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM1);
  LED[6] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM1);
  LED[7] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM1);
  LED[8] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM1);
  LED[9] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM1);
  LED[10] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM1);
  LED[11] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM1);
  LED[12] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM1);
  LED[13] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM1);
  LED[14] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM1);
  LED[15] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM1);
  //Multiplexer 2
  LED[16] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM2);
  LED[17] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM2);
  LED[18] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM2);
  LED[19] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM2);
  LED[20] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM2);
  LED[21] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM2);
  LED[22] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM2);
  LED[23] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM2);
  LED[24] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM2);
  LED[25] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM2);
  LED[26] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM2);
  LED[27] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM2);
  LED[28] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM2);
  LED[29] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM2);
  LED[30] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM2);
  LED[31] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM2);
  //Multiplexer 3
  LED[32] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM3);
  LED[33] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM3);
  LED[34] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM3);
  LED[35] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM3);
  LED[36] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM3);
  LED[37] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM3);
  LED[38] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM3);
  LED[39] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM3);
  LED[40] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM3);
  LED[41] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM3);
  LED[42] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM3);
  LED[43] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM3);
  LED[44] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM3);
  LED[45] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM3);
  LED[46] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM3);
  LED[47] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM3);
  for (int i = 0; i < 32; i++) {
    if (LED[i] > Schwellwerte[i]) {
      Photo[i] = true;
      minEinerDa = true;
      //Serial.println(i);
    }
    else {
      Photo[i] = false;
    }
  }

  /*for (int i = 0; i < 32; i++) {
    if (LED[i] > 450) {
      Serial.println  ("Das ist LED:");
      Serial.println(i);
      Serial.println(LED[i]);
      delay(10);
    }
    }*/
}
void bodenverarbeiten(bool* gesehenSensor,bool& minEinerDa,bool* Photo, double& bodenrichtung, int* LED, Adafruit_BNO055& gyro,bool& buttonGpressed, int* Schwellwerte, double& minus, long& alteZeit, int& alterWinkel, double& rotation, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2, uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t M1_FW, uint8_t M1_RW, uint8_t M1_PWM, uint8_t M2_FW, uint8_t M2_RW, uint8_t M2_PWM, uint8_t M3_FW, uint8_t M3_RW, uint8_t M3_PWM, uint8_t M4_FW, uint8_t M4_RW, uint8_t M4_PWM) {
  int AnzahlSens = 0;
  for (int i = 0; i < 32; i++) {
    gesehenSensor[i] = false;

  }
  if (minEinerDa) {

    long zeitlinieda = millis();

    while (millis() - zeitlinieda <= 75) {
      for (int i = 0; i < 32; i++) {
        if (Photo[i] && AnzahlSens <= 30 && !gesehenSensor[i]) {
          gesehenSensor[i] = true;
          Serial.println(i);
          AnzahlSens++;
        }
      }

      double xSumme = 0;
      double ySumme = 0;
      for (int i = 0; i < 32; i++) {
        if (gesehenSensor[i]) {
          xSumme += cos(bodenrichtungszuweisung(i));
          ySumme += sin(bodenrichtungszuweisung(i));
        }
      }
      bodenrichtung = atan2(ySumme, xSumme) * 180.0 / PI;
      bodenrichtung -= 180;
      bodenrichtung = abs(bodenrichtung);
      //Multiplexer 1
      LED[0] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM1);
      LED[1] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM1);
      LED[2] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM1);
      LED[3] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM1);
      LED[4] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM1);
      LED[5] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM1);
      LED[6] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM1);
      LED[7] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM1);
      LED[8] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM1);
      LED[9] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM1);
      LED[10] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM1);
      LED[11] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM1);
      LED[12] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM1);
      LED[13] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM1);
      LED[14] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM1);
      LED[15] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM1);
      //Multiplexer 2
      LED[16] = lesenMultiplexerUnten(0, 0, 0, 0,S0,S1,S2,S3,UAM2);
      LED[17] = lesenMultiplexerUnten(0, 0, 0, 1,S0,S1,S2,S3,UAM2);
      LED[18] = lesenMultiplexerUnten(0, 0, 1, 0,S0,S1,S2,S3,UAM2);
      LED[19] = lesenMultiplexerUnten(0, 0, 1, 1,S0,S1,S2,S3,UAM2);
      LED[20] = lesenMultiplexerUnten(0, 1, 0, 0,S0,S1,S2,S3,UAM2);
      LED[21] = lesenMultiplexerUnten(0, 1, 0, 1,S0,S1,S2,S3,UAM2);
      LED[22] = lesenMultiplexerUnten(0, 1, 1, 0,S0,S1,S2,S3,UAM2);
      LED[23] = lesenMultiplexerUnten(0, 1, 1, 1,S0,S1,S2,S3,UAM2);
      LED[24] = lesenMultiplexerUnten(1, 0, 0, 0,S0,S1,S2,S3,UAM2);
      LED[25] = lesenMultiplexerUnten(1, 0, 0, 1,S0,S1,S2,S3,UAM2);
      LED[26] = lesenMultiplexerUnten(1, 0, 1, 0,S0,S1,S2,S3,UAM2);
      LED[27] = lesenMultiplexerUnten(1, 0, 1, 1,S0,S1,S2,S3,UAM2);
      LED[28] = lesenMultiplexerUnten(1, 1, 0, 0,S0,S1,S2,S3,UAM2);
      LED[29] = lesenMultiplexerUnten(1, 1, 0, 1,S0,S1,S2,S3,UAM2);
      LED[30] = lesenMultiplexerUnten(1, 1, 1, 0,S0,S1,S2,S3,UAM2);
      LED[31] = lesenMultiplexerUnten(1, 1, 1, 1,S0,S1,S2,S3,UAM2);
      for (int i = 0; i < 32; i++) {
        if (LED[i] > Schwellwerte[i]) {
          Photo[i] = true;
          minEinerDa = true;
          //Serial.println(i);
          zeitlinieda = millis();
        }
        else {
          Photo[i] = false;
        }
      }
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
      winkel = winkel - minus;
      if (winkel > 180) {                                                       //Werte umrechnen von 0-359 auf Werte von -180 - +180 => für Formel
        winkel = winkel - 360;
      }
      if (winkel < -180) {
        winkel = winkel + 360;
      }
      double p = 8;                                                          //korrekturfaktor
      double d = 50;                                                           //korrekturfaktor
      long zeit = millis() - alteZeit;                                          //vergangene Zeit seit der letzten Berechnung von drehung
      rotation = (p * winkel) - d * rotationSpeed;   //Berechnung der drehung
      alterWinkel = winkel;
      alteZeit = zeit;
      rotation = -rotation / 4.5;
      motor(bodenrichtung, 150,rotation,M1_FW, M1_RW, M1_PWM, M2_FW, M2_RW, M2_PWM, M3_FW, M3_RW, M3_PWM, M4_FW, M4_RW, M4_PWM);
      //Serial.println(compass());
      digitalWrite(LEDboden, HIGH);
      if (digitalRead(ButtonI) == LOW) {
        AutoCalibration(LED,Schwellwerte,LEDboden,S0,S1,S2,S3,UAM1,UAM2,UAM3);
      }
    }
  }
  else {
    bodenrichtung = -1;
  }
}
void Boden(bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation, uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t M1_FW, uint8_t M1_RW, uint8_t M1_PWM, uint8_t M2_FW, uint8_t M2_RW, uint8_t M2_PWM, uint8_t M3_FW, uint8_t M3_RW, uint8_t M3_PWM, uint8_t M4_FW, uint8_t M4_RW, uint8_t M4_PWM) {
  bodenlesen(minEinerDa,LED,Schwellwerte,Photo,S0,S1,S2,S3,UAM1,UAM2,UAM3);
  bodenverarbeiten(gesehenSensor,minEinerDa,Photo,bodenrichtung,LED,gyro,buttonGpressed,Schwellwerte,minus,alteZeit,alterWinkel,rotation,S0,S1,S2,S3,UAM1,UAM2,UAM3,LEDboden,ButtonI,M1_FW, M1_RW, M1_PWM, M2_FW, M2_RW, M2_PWM, M3_FW, M3_RW, M3_PWM, M4_FW, M4_RW, M4_PWM);
}
double bodenrichtungszuweisung(int n) {
  return ((n * 11.25 + 90 ) / 180 * PI );
}