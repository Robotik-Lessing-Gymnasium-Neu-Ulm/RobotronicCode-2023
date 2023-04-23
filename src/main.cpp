#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <EEPROM.h>
#define I2C
#include <Pixy2I2C.h>
#include<PID_v1.h>
#include<SD.h>
#include <ArduinoJson.h>

#include<Boden.h>                                   //eigene Bibliotheken
#include<Motoren.h>
#include <myPixy.h>
#include <Lichtschranke.h>
#include<IR2.h>
#include <Kompass.h>
#include<ControlLEDs.h>
#include <Bluetooth.h>
#include<Defines.h>
#include<Torwart.h>

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

int PixyG;
int PixyG2;


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
bool IRsave;

int minWertLS;

double entfSet=5;                                                 //wird sich nach dem Anfahrtsradius richten
PID entfPID(&IRbest,&entfVelo,&entfSet,5.2,0,0.35,REVERSE);          //PID-Regler über die Enfernung (5,0,0.3)
double wiSet=0;                                                   //Setpoint des Winkelpids (vorne)
double wiIn;                                                      //Inpunt des Winkelpids
PID wiPID(&wiIn,&wiVelo,&wiSet,0.79,0,0.32,REVERSE);               //PID-Regler über den Winkel (0.8,0,0.22)->(0.8,0,0.23); last: (0.79,0,0.2517)

bool buttonGpressed = true;                           //other

void setup() {
  while (!SD.begin(BUILTIN_SDCARD)) {                     //SD-Karte initialisieren
    Serial.println("Karte einstecken!");
  }
  Serial.println("Karte initialisiert.");
  File myFile=SD.open("minWerte.json",FILE_READ);         //Datei öffnen, lesen
    char buf[myFile.size()];                              //Zwischenspeicher des Inhalts der geöffneten *.json-Datei
    myFile.read(buf,myFile.size());
    StaticJsonDocument<1000> doc;                         //Json-Datei aus der Datei
    deserializeJson(doc, buf);
    Serial.println(">>>>>>>>>>>>>>(minWerte)");           //Befüllen von minWert & Ausgabe
    Serial.println(">>>>>>>>>>>>(IR)");
    for(int i{0};i<16;i++){
      minWert[i]=doc["IR"][i];
      Serial.print(" ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(minWert[i]);
    }
    Serial.println("(IR)<<<<<<<<<<<<");                   //Befüllen von minWertLS
    minWertLS=doc["Lichtschranke"];
    Serial.print("LS: ");
    Serial.println(minWertLS);
    Serial.println("(MinWerte)<<<<<<<<<<<<<<");
  myFile.close();
  
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
  pinMode(LEDIV, OUTPUT);
  pinMode(LEDII, OUTPUT);
  pinMode(LEDIII, OUTPUT);
  pinMode(LEDI, OUTPUT);               //Die Button-Pins an einen Pullup-Widerstand hängen
  pinMode(ButtonI, INPUT_PULLUP);
  pinMode(ButtonII, INPUT_PULLUP);
  pinMode(ButtonIII, INPUT_PULLUP);
  pinMode(ButtonIV, INPUT_PULLUP);              //IR-Kalibration
  pinMode(LichtSchranke,INPUT);
  AutoCalibration(LED,Schwellwerte);
  gyro.begin(/*8*/);                            //den gyro losmessen lassen (ich musste die 8 auskommentieren, es funktioniert trotzdem)
  entfPID.SetMode(AUTOMATIC);
  wiPID.SetMode(AUTOMATIC);
}

void loop() {
  ControlLEDs(buttonGpressed,richtung,IRbest,Icball,rotation,minEinerDa,irAutoCalibration,IRsave); //Die grünen Kontroll-LEDs leuchten lassen
  bluetooth(torwart,IRbest);                                           //empfangen und senden
  if (hatBall() && ( Icball == 0 || Icball == 15 || Icball == 1 )) {      //Ermitteln ob er den Ball hat  hatBall() && ( Icball == 0 || Icball == 15 || Icball == 1 )
    IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);   //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation);
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,piread,PixyG);
    if (bodenrichtung == -1) {
      PixyG=Pixy(pixy,piread);
      compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,true,PixyG);
      if (piread) {                                                         //sieht die pixy etwas
        motor(90, 120,rotation);                             //mit 100 aufs Tor zufahren (später mit Ausrichtung zum Tor -> Ausrichtung auf Pixywinkel ändern)
      }
      else {
        compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,false,PixyG);
        motor(90,90, rotation/3);                                               //nach vorne fahren
      }
    }else{                                                                //der Boden sieht etwas
      motor(bodenrichtung, 200,rotation);                                 //sehr schnell von der Linie wegfahren
    }
  }else if(torwart){
    torwartProgramm(LED,Schwellwerte,rotation,gyro,buttonGpressed,minus,alterWinkel,addRot,piread,PixyG2,PixyG,IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,WinkelBall,IRsave);
  }else{    //!torwart
    IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);   //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation);
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,piread,PixyG);
    piread=false;
    entfPID.Compute();
    wiPID.Compute();
    if (bodenrichtung == -1) {                                            //der Boden sieht nichts
      if (richtung != -1) {                                               //der IR sieht etwas
        if(WinkelBall<115&&WinkelBall>65){                                //Ball Vor dem Roboter
          int delay=50;
          if(addRot!=0){
            delay=1200;
          }
          if(WinkelBall>=90&&WinkelBall<270){
            if(addRotTime+delay<millis()){
              addRot=-19;
              addRotTime=millis();
            }
          }else{
            if(addRotTime+delay<millis()){
              addRot=19;
              addRotTime=millis();
            }
          }
          motor(90-0.7*addRot,95,rotation);
        }else{
          motor(richtung-addRot,((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo,rotation);
          //motor(richtung-addRot,entfVelo,rotation);
          int delay=50;
          if(addRot!=0){
            delay=700;
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
}