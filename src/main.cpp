#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#define I2C
#include <Pixy2I2C.h>
#include<PID_v1.h>
#include<SD.h>

#include<Boden.h>                                   //eigene Bibliotheken
#include<Motoren.h>
#include <myPixy.h>
#include <Lichtschranke.h>
#include<IR2.h>
#include <Kompass.h>
#include<ControlLEDs.h>
#include <Bluetooth.h>
#include<Defines.h>

Pixy2I2C pixy;                                      //pixi im i2c-kommunikations-modus initialisieren

Adafruit_BNO055 gyro = Adafruit_BNO055(55, 0x28);   //Erstellen eines Obketes der Klasse Adafruit_BNO055 mit Namen gyro

bool Photo[48];                                     //Variablen für Boden
int Schwellwerte[48];
bool gesehenSensor[48];
int LED[48];
int LEDbest;
double bodenrichtung = -1;
bool minEinerDa = false;

bool irAutoCalibration = false;                     //für IR-Kalibration
int minWert[16];


/*//Variablen US
double vect;
long timeold;
int x;
int y;
bool xSum;
bool ySum;
int US[4];
int Wert[4];*/

bool piread;                                          //Variablen für Kamera
double TorRichtungKamera;

long alteZeit;                                        //Variablen für Gyro
int alterWinkel;
double minus;
double rotation;

int IR[16];                                           //Variablen für IR
double IRbest = -1;
double richtung = -1;
int Icball = -1;
double entfVelo = 50;                                 //Geschwindigkeit vom PID gesteuert bei der Ballanfahrt (Entfernungs-PID)
double wiVelo = 50;                                   //Gesch.-Output des Winkelpids
double addRot = 0;                                    //temporäre Drehung, wenn er nah am Ball ist
unsigned long addRotTime=0;
double WinkelBall=90;

/*int IRalt0[16];
int IRalt1[16];
int IRalt2[16];*/

/*//Motoren
double m1;
double m2;
double m3;
double m4;
double phi;*/

bool torwart;                                         //bluetooth

double entfSet=5;                                                 //wird sich nach dem Anfahrtsradius richten
PID entfPID(&IRbest,&entfVelo,&entfSet,7,0,1.5,REVERSE);          //PID-Regler über die Enfernung
double wiSet=0;                                                   //Setpoint des Winkelpids (vorne)
double wiIn;                                                      //Inpunt des Winkelpids
PID wiPID(&wiIn,&wiVelo,&wiSet,0.9,0,0.2,REVERSE);               //PID-Regler über den Winkel

bool buttonGpressed = true;                           //other

void setup() {
  Serial.begin(115200);                         //Seriellen Monitor initialisieren
  Serial5.begin(115200);                        //Bluetooth initialisieren
  pinMode(S0, OUTPUT);                          //Multiplexer Oben Pin Festlegung
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(AM1, INPUT);
  pinMode(UAM1, INPUT);
  pinMode(UAM2, INPUT);
  pinMode(UAM3, INPUT);                         //Linker Motor Pin Festlegung
  pinMode(M1_FW, OUTPUT);
  pinMode(M1_RW, OUTPUT);                       //Hinterer Motor Pin Festlegung
  pinMode(M2_FW, OUTPUT);
  pinMode(M2_RW, OUTPUT);                       //Rechter Motor Pin Festlegung
  pinMode(M3_FW, OUTPUT);
  pinMode(M3_RW, OUTPUT);                       //links hinten
  pinMode(M4_FW, OUTPUT);
  pinMode(M4_RW, OUTPUT);                       //Control LEDs
  pinMode(LEDboden, OUTPUT);
  pinMode(LEDir, OUTPUT);
  pinMode(LEDgyro, OUTPUT);
  pinMode(LEDballcaught, OUTPUT);               //Die Button-Pins an einen Pullup-Widerstand hängen
  pinMode(ButtonI, INPUT_PULLUP);
  pinMode(ButtonII, INPUT_PULLUP);
  pinMode(ButtonIII, INPUT_PULLUP);
  pinMode(ButtonIV, INPUT_PULLUP);              //IR-Kalibration
  AutoCalibration(LED,Schwellwerte);
  gyro.begin(/*8*/);                            //den gyro losmessen lassen (ich musste die 8 auskommentieren, es funktioniert trotzdem)
  entfPID.SetMode(AUTOMATIC);
  wiPID.SetMode(AUTOMATIC);
  SD.begin(BUILTIN_SDCARD);                     //SD-Karte initialisieren
  File myFile=SD.open("minWerte.txt",FILE_READ);//Datei öffnen, lesen
  char buf[1];
  int count=0;
  for(int i=0;i<16;){
    myFile.read(buf,1);
    Serial.print(buf);
    switch(buf[0]){
      case ';'||-1:
        minWert[i++]=600;
        break;
      case '|':
        count=0;
        minWert[++i]=0;
        break;
      default:
        count++;
        minWert[i]+=10^count*buf[0];
    }
  }
  myFile.close();
}

void loop() {
  ControlLEDs(buttonGpressed,richtung,IRbest,Icball,rotation,minEinerDa,irAutoCalibration); //Die grünen Kontroll-LEDs leuchten lassen
  IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart);   //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen
  //Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation);
  compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot);
  // bluetooth(torwart,IRbest);                                           //empfangen und senden
  if (!torwart) {
    if (bodenrichtung == -1) {                                            //der Boden sieht nichts
      if (richtung != -1) {                                               //der IR sieht etwas
        if(WinkelBall<130&&WinkelBall>50){                                //Ball Vor dem Roboter
          int delay=50;
          if(addRot!=0){
            delay=1200;
          }
          if(WinkelBall>=90&&WinkelBall<270){
            if(addRotTime+delay<millis()){
              addRot=-25;
              addRotTime=millis();
            }
          }else{
            if(addRotTime+delay<millis()){
              addRot=25;
              addRotTime=millis();
            }
          }
          motor(90-addRot/2,100,rotation);
        }else{
          motor(richtung-addRot,((IRbest-entfSet)/(75-entfSet))*entfVelo+wiVelo,(1-(IRbest-entfSet)/(75-entfSet))*rotation);
          int delay=50;
          if(addRot!=0){
            delay=500;
          }
          if(addRotTime+delay<millis()){
            addRot=0;
            addRotTime=millis();
          }
        }
      }
      else {                                                              //der IR sieht nichts (später wahrscheinlich: auf neutralen Punkt fahren)
        motor(0, 0,rotation);                                             //nur ausrichten
      }
    }
    else {                                                                //der Boden sieht etwas
      motor(bodenrichtung, 200,rotation);                                 //sehr schnell von der Linie wegfahren
    }
  }
  else {                                                                  //stehen bleiben, falls der Andere den Ball hat (torwart)
    motor(0, 0,rotation);
  }
  if (hatBall() && ( Icball == 0 || Icball == 15 || Icball == 1 )) {      //Ermitteln ob er den Ball hat
    if (piread) {                                                         //sieht die pixy etwas
      motor(Pixy(pixy,piread), 100,rotation);                             //mit 100 aufs Tor zufahren (später mit Ausrichtung zum Tor -> Ausrichtung auf Pixywinkel ändern)
    }
    else {
      motor(0,0, rotation);                                               //nach vorne fahren
    }
  }
  entfPID.Compute();
  wiPID.Compute();
}