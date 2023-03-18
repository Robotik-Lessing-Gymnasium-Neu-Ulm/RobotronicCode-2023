#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#define I2C
#include <Pixy2I2C.h>
//eigene Bibliotheken
#include<Boden.h>
#include<Motoren.h>
#include <myPixy.h>
#include <Lichtschranke.h>
#include<IR2.h>
#include <Kompass.h>
#include<ControlLEDs.h>
#include <Bluetooth.h>
#include<Defines.h>


//pixi im i2c-kommunikations-modus initialisieren
Pixy2I2C pixy;

//Erstellen eines Obketes der Klasse Adafruit_BNO055 mit Namen gyro
Adafruit_BNO055 gyro = Adafruit_BNO055(55, 0x28);

//Variablen für Boden
bool Photo[48];
int Schwellwerte[48];
bool gesehenSensor[48];
int LED[48];
int LEDbest;
double bodenrichtung = -1;
bool minEinerDa = false;

/*//Variablen US
double vect;
long timeold;
int x;
int y;
bool xSum;
bool ySum;
int US[4];
int Wert[4];*/

//Other
bool buttonGpressed = true;

//Variablen für Kamera
bool piread;
double TorRichtungKamera;

//Variablen für Gyro
long alteZeit;
int alterWinkel;
double minus;
double rotation;

//Variablen für IR
int IR[16];
int IRbest = -1;
double richtung = -1;
int Icball = -1;
double veloAnf = 50; //Geschwindigkeit vom PID gesteuert bei der Ballanfahrt
/*int IRalt0[16];
int IRalt1[16];
int IRalt2[16];*/

/*//Motoren
double m1;
double m2;
double m3;
double m4;
double phi;*/

//bluetooth
bool torwart;

void setup() {
  //Seriellen Monitor initialisieren
  Serial.begin(115200);
  //Bluetooth initialisieren
  Serial5.begin(115200);
  //Multiplexer Oben Pin Festlegung
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(AM1, INPUT);
  pinMode(UAM1, INPUT);
  pinMode(UAM2, INPUT);
  pinMode(UAM3, INPUT);
  //Linker Motor Pin Festlegung
  pinMode(M1_FW, OUTPUT);
  pinMode(M1_RW, OUTPUT);
  //Hinterer Motor Pin Festlegung
  pinMode(M2_FW, OUTPUT);
  pinMode(M2_RW, OUTPUT);
  //Rechter Motor Pin Festlegung
  pinMode(M3_FW, OUTPUT);
  pinMode(M3_RW, OUTPUT);
  //links hinten
  pinMode(M4_FW, OUTPUT);
  pinMode(M4_RW, OUTPUT);
  //Control LEDs
  pinMode(LEDboden, OUTPUT);
  pinMode(LEDir, OUTPUT);
  pinMode(LEDgyro, OUTPUT);
  pinMode(LEDballcaught, OUTPUT);
  //Die Button-Pins an einen Pullup-Widerstand hängen
  pinMode(calibrationButton, INPUT_PULLUP);
  pinMode(gyroButton, INPUT_PULLUP);
  //den Boden automatisch kalibrieren
  AutoCalibration(LED,Schwellwerte);
  //den gyro losmessen lassen (ich musste die 8 auskommentieren, hoffentlich funktionierts trotzdem)
  gyro.begin(/*8*/);
}

void loop() {
  //Die grünen Kontroll-LEDs leuchten lassen
  ControlLEDs(buttonGpressed,richtung,IRbest,Icball,rotation,minEinerDa);
  //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen
  IRsens(IR,IRbest,Icball,richtung,veloAnf); //………
  Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation);
  compass(gyro,buttonGpressed,minus,rotation,alterWinkel);
  //empfangen und senden
  bluetooth(torwart,IRbest);
  if (!torwart) {
    if (bodenrichtung == -1) { //der Boden sieht nichts
      //geschwindigkeiten ändern, je nach Entfernung zum Ball
      if (richtung != -1) { //der IR sieht etwas
        motor(richtung,veloAnf,rotation);
      }
      else { //der IR sieht nichts (später wahrscheinlich: auf neutralen Punkt fahren)
        motor(0, 0,rotation); //nur ausrichten
      }
    }
    else { //der Boden sieht etwas
      motor(bodenrichtung, 200,rotation); //sehr schnell von der Linie wegfahren
    }
  }
  else { //stehen bleiben, falls der Andere den Ball hat (torwart)
    motor(0, 0,rotation);
  }
  if (hatBall() && ( Icball == 0 || Icball == 15 || Icball == 1 )) { //Ermitteln ob er den Ball hat
    if (piread) { //sieht die pixy etwas
      motor(Pixy(pixy,piread), 100,rotation); //mit 100 aufs Tor zufahren (später mit Ausrichtung zum Tor -> Ausrichtung auf Pixywinkel ändern)
    }
    else {
      motor(90, 60, rotation); //nach vorne fahren
    }
  }
}