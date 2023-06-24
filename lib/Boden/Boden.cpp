#include <Boden.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Motoren.h>
#include <Kompass.h>

//Multiplexer Unten
#ifndef S0
#define S0 36
#endif
#ifndef S1
#define S1 35
#endif
#ifndef S2
#define S2 34
#endif
#ifndef S3
#define S3 33
#endif

#ifndef UAM1
#define UAM1 A17 //analog Multiplexer Unten 1
#endif
#ifndef UAM2
#define UAM2 A15 //analog Multplexer Unten 2
#endif
#ifndef UAM3
#define UAM3 A16 // analog Multiplexer Unten 3
#endif

#ifndef LEDIV
#define LEDIV 13
#endif

#ifndef ButtonI
#define ButtonI 5
#endif

void AutoCalibration(int* LED, int* Schwellwerte) {
  int CalStatus = 0;
  int bestLED[48];
  while (CalStatus < 100) {
  //Multiplexer 1
  LED[0] = lesenMultiplexerUnten(0, 0, 0, 0);
  LED[1] = lesenMultiplexerUnten(0, 0, 0, 1);
  LED[2] = lesenMultiplexerUnten(0, 0, 1, 0);
  LED[3] = lesenMultiplexerUnten(0, 0, 1, 1);
  LED[4] = lesenMultiplexerUnten(0, 1, 0, 0);
  LED[5] = lesenMultiplexerUnten(0, 1, 0, 1);
  LED[6] = lesenMultiplexerUnten(0, 1, 1, 0);
  LED[7] = lesenMultiplexerUnten(0, 1, 1, 1);
  LED[8] = lesenMultiplexerUnten(1, 0, 0, 0);
  LED[9] = lesenMultiplexerUnten(1, 0, 0, 1);
  LED[10] = lesenMultiplexerUnten(1, 0, 1, 0);
  LED[11] = lesenMultiplexerUnten(1, 0, 1, 1);
  LED[12] = lesenMultiplexerUnten(1, 1, 0, 0);
  LED[13] = lesenMultiplexerUnten(1, 1, 0, 1);
  LED[14] = lesenMultiplexerUnten(1, 1, 1, 0);
  LED[15] = lesenMultiplexerUnten(1, 1, 1, 1);
  //Multiplexer 2
  LED[16] = lesenMultiplexerUnten2(0, 0, 0, 0);
  LED[17] = lesenMultiplexerUnten2(0, 0, 0, 1);
  LED[18] = lesenMultiplexerUnten2(0, 0, 1, 0);
  LED[19] = lesenMultiplexerUnten2(0, 0, 1, 1);
  LED[20] = lesenMultiplexerUnten2(0, 1, 0, 0);
  LED[21] = lesenMultiplexerUnten2(0, 1, 0, 1);
  LED[22] = lesenMultiplexerUnten2(0, 1, 1, 0);
  LED[23] = lesenMultiplexerUnten2(0, 1, 1, 1);
  LED[24] = lesenMultiplexerUnten2(1, 0, 0, 0);
  LED[25] = lesenMultiplexerUnten2(1, 0, 0, 1);
  LED[26] = lesenMultiplexerUnten2(1, 0, 1, 0);
  LED[27] = lesenMultiplexerUnten2(1, 0, 1, 1);
  LED[28] = lesenMultiplexerUnten2(1, 1, 0, 0);
  LED[29] = lesenMultiplexerUnten2(1, 1, 0, 1);
  LED[30] = lesenMultiplexerUnten2(1, 1, 1, 0);
  LED[31] = lesenMultiplexerUnten2(1, 1, 1, 1);
  //Multiplexer 3
  LED[32] = lesenMultiplexerUnten3(0, 0, 0, 0);
  LED[33] = lesenMultiplexerUnten3(0, 0, 0, 1);
  LED[34] = lesenMultiplexerUnten3(0, 0, 1, 0);
  LED[35] = lesenMultiplexerUnten3(0, 0, 1, 1);
  LED[36] = lesenMultiplexerUnten3(0, 1, 0, 0);
  LED[37] = lesenMultiplexerUnten3(0, 1, 0, 1);
  LED[38] = lesenMultiplexerUnten3(0, 1, 1, 0);
  LED[39] = lesenMultiplexerUnten3(0, 1, 1, 1);
  LED[40] = lesenMultiplexerUnten3(1, 0, 0, 0);
  LED[41] = lesenMultiplexerUnten3(1, 0, 0, 1);
  LED[42] = lesenMultiplexerUnten3(1, 0, 1, 0);
  LED[43] = lesenMultiplexerUnten3(1, 0, 1, 1);
  LED[44] = lesenMultiplexerUnten3(1, 1, 0, 0);
  LED[45] = lesenMultiplexerUnten3(1, 1, 0, 1);
  LED[46] = lesenMultiplexerUnten3(1, 1, 1, 0);
  LED[47] = lesenMultiplexerUnten3(1, 1, 1, 1);
    for (int i = 0; i < 48; i++) {
       bestLED[i] = 0;
       if(LED[i] > bestLED[i]){
          bestLED[i] = LED[i]; 
          Schwellwerte[i] = LED[i]+100;
       }
    }
    CalStatus++;
  }
}
int lesenMultiplexerUnten(int s0, int s1, int s2, int s3) {  //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(UAM1);
}
int lesenMultiplexerUnten2(int s0, int s1, int s2, int s3) {  //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(UAM2);
}
int lesenMultiplexerUnten3(int s0, int s1, int s2, int s3) {  //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(UAM3);
}
void bodenlesen(bool& minEinerDa,int* LED, int* Schwellwerte, bool* Photo) {
  minEinerDa = false;
  //Multiplexer 1
  LED[0] = lesenMultiplexerUnten(0, 0, 0, 0);
  LED[1] = lesenMultiplexerUnten(0, 0, 0, 1);
  LED[2] = lesenMultiplexerUnten(0, 0, 1, 0);
  LED[3] = lesenMultiplexerUnten(0, 0, 1, 1);
  LED[4] = lesenMultiplexerUnten(0, 1, 0, 0);
  LED[5] = lesenMultiplexerUnten(0, 1, 0, 1);
  LED[6] = lesenMultiplexerUnten(0, 1, 1, 0);
  LED[7] = lesenMultiplexerUnten(0, 1, 1, 1);
  LED[8] = lesenMultiplexerUnten(1, 0, 0, 0);
  LED[9] = lesenMultiplexerUnten(1, 0, 0, 1);
  LED[10] = lesenMultiplexerUnten(1, 0, 1, 0);
  LED[11] = lesenMultiplexerUnten(1, 0, 1, 1);
  LED[12] = lesenMultiplexerUnten(1, 1, 0, 0);
  LED[13] = lesenMultiplexerUnten(1, 1, 0, 1);
  LED[14] = lesenMultiplexerUnten(1, 1, 1, 0);
  LED[15] = lesenMultiplexerUnten(1, 1, 1, 1);
  //Multiplexer 2
  LED[16] = lesenMultiplexerUnten2(0, 0, 0, 0);
  LED[17] = lesenMultiplexerUnten2(0, 0, 0, 1);
  LED[18] = lesenMultiplexerUnten2(0, 0, 1, 0);
  LED[19] = lesenMultiplexerUnten2(0, 0, 1, 1);
  LED[20] = lesenMultiplexerUnten2(0, 1, 0, 0);
  LED[21] = lesenMultiplexerUnten2(0, 1, 0, 1);
  LED[22] = lesenMultiplexerUnten2(0, 1, 1, 0);
  LED[23] = lesenMultiplexerUnten2(0, 1, 1, 1);
  LED[24] = lesenMultiplexerUnten2(1, 0, 0, 0);
  LED[25] = lesenMultiplexerUnten2(1, 0, 0, 1);
  LED[26] = lesenMultiplexerUnten2(1, 0, 1, 0);
  LED[27] = lesenMultiplexerUnten2(1, 0, 1, 1);
  LED[28] = lesenMultiplexerUnten2(1, 1, 0, 0);
  LED[29] = lesenMultiplexerUnten2(1, 1, 0, 1);
  LED[30] = lesenMultiplexerUnten2(1, 1, 1, 0);
  LED[31] = lesenMultiplexerUnten2(1, 1, 1, 1);
  //Multiplexer 3
  LED[32] = lesenMultiplexerUnten3(0, 0, 0, 0);
  LED[33] = lesenMultiplexerUnten3(0, 0, 0, 1);
  LED[34] = lesenMultiplexerUnten3(0, 0, 1, 0);
  LED[35] = lesenMultiplexerUnten3(0, 0, 1, 1);
  LED[36] = lesenMultiplexerUnten3(0, 1, 0, 0);
  LED[37] = lesenMultiplexerUnten3(0, 1, 0, 1);
  LED[38] = lesenMultiplexerUnten3(0, 1, 1, 0);
  LED[39] = lesenMultiplexerUnten3(0, 1, 1, 1);
  LED[40] = lesenMultiplexerUnten3(1, 0, 0, 0);
  LED[41] = lesenMultiplexerUnten3(1, 0, 0, 1);
  LED[42] = lesenMultiplexerUnten3(1, 0, 1, 0);
  LED[43] = lesenMultiplexerUnten3(1, 0, 1, 1);
  LED[44] = lesenMultiplexerUnten3(1, 1, 0, 0);
  LED[45] = lesenMultiplexerUnten3(1, 1, 0, 1);
  LED[46] = lesenMultiplexerUnten3(1, 1, 1, 0);
  LED[47] = lesenMultiplexerUnten3(1, 1, 1, 1);
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
void bodenverarbeiten(bool* gesehenSensor,bool& minEinerDa,bool* Photo, double& bodenrichtung, int* LED, 
Adafruit_BNO055& gyro,bool& buttonGpressed, int* Schwellwerte, double& minus, long& alteZeit, int& alterWinkel, double& rotation, double &addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwart,double& accel) {
  int AnzahlSens = 0;
  for (int i = 0; i < 32; i++) {
    gesehenSensor[i] = false;

  }
  if (minEinerDa) {

    long zeitlinieda = millis();

    while (millis() - zeitlinieda <= 75) {
      Serial.println(LED[1]);
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
      LED[0] = lesenMultiplexerUnten(0, 0, 0, 0);
      LED[1] = lesenMultiplexerUnten(0, 0, 0, 1);
      LED[2] = lesenMultiplexerUnten(0, 0, 1, 0);
      LED[3] = lesenMultiplexerUnten(0, 0, 1, 1);
      LED[4] = lesenMultiplexerUnten(0, 1, 0, 0);
      LED[5] = lesenMultiplexerUnten(0, 1, 0, 1);
      LED[6] = lesenMultiplexerUnten(0, 1, 1, 0);
      LED[7] = lesenMultiplexerUnten(0, 1, 1, 1);
      LED[8] = lesenMultiplexerUnten(1, 0, 0, 0);
      LED[9] = lesenMultiplexerUnten(1, 0, 0, 1);
      LED[10] = lesenMultiplexerUnten(1, 0, 1, 0);
      LED[11] = lesenMultiplexerUnten(1, 0, 1, 1);
      LED[12] = lesenMultiplexerUnten(1, 1, 0, 0);
      LED[13] = lesenMultiplexerUnten(1, 1, 0, 1);
      LED[14] = lesenMultiplexerUnten(1, 1, 1, 0);
      LED[15] = lesenMultiplexerUnten(1, 1, 1, 1);
      //Multiplexer 2
      LED[16] = lesenMultiplexerUnten2(0, 0, 0, 0);
      LED[17] = lesenMultiplexerUnten2(0, 0, 0, 1);
      LED[18] = lesenMultiplexerUnten2(0, 0, 1, 0);
      LED[19] = lesenMultiplexerUnten2(0, 0, 1, 1);
      LED[20] = lesenMultiplexerUnten2(0, 1, 0, 0);
      LED[21] = lesenMultiplexerUnten2(0, 1, 0, 1);
      LED[22] = lesenMultiplexerUnten2(0, 1, 1, 0);
      LED[23] = lesenMultiplexerUnten2(0, 1, 1, 1);
      LED[24] = lesenMultiplexerUnten2(1, 0, 0, 0);
      LED[25] = lesenMultiplexerUnten2(1, 0, 0, 1);
      LED[26] = lesenMultiplexerUnten2(1, 0, 1, 0);
      LED[27] = lesenMultiplexerUnten2(1, 0, 1, 1);
      LED[28] = lesenMultiplexerUnten2(1, 1, 0, 0);
      LED[29] = lesenMultiplexerUnten2(1, 1, 0, 1);
      LED[30] = lesenMultiplexerUnten2(1, 1, 1, 0);
      LED[31] = lesenMultiplexerUnten2(1, 1, 1, 1);
      for (int i = 0; i < 32; i++) {
        if (LED[i] > Schwellwerte[i]) {
          Photo[i] = true;
          minEinerDa = true;
          //Serial.println(i);
          zeitlinieda = millis();
        }else if(Photo[i]){
          Photo[i]=true;
        }
        else {
          Photo[i] = false;
        }
      }
      compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,piread,pixyG,pixyG2,hatBall,torwart,accel);
      motor(bodenrichtung, 150,rotation);
      //Serial.println(LED[1]);
      digitalWrite(LEDIV, HIGH);
      if (digitalRead(ButtonI) == LOW) {
        AutoCalibration(LED,Schwellwerte);

      }
    }
  }
  else {
    bodenrichtung = -1;
  }
}
void Boden(bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation,double &addRot, bool piread, int pixyG, int pixyG2, bool hatBall, bool torwart,double& accel) {
  bodenlesen(minEinerDa,LED,Schwellwerte,Photo);
  bodenverarbeiten(gesehenSensor,minEinerDa,Photo,bodenrichtung,LED,gyro,buttonGpressed,Schwellwerte,minus,alteZeit,alterWinkel,rotation,addRot,piread,pixyG,pixyG2,hatBall,torwart, accel);
}
double bodenrichtungszuweisung(int n) {
  return ((n * 11.25 + 90 ) / 180 * PI );
}

bool onLine(int* LED, int* Schwellwerte){   //für TW
  if(LED[0]>Schwellwerte[0]||LED[1]>Schwellwerte[1]||LED[47] > Schwellwerte[47] || LED[46] > Schwellwerte[46]){
    return true;
  }else{
    return false;
  }
}