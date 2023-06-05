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


Pixy2I2C pixy, pixy2;                                      //pixi im i2c-kommunikations-modus initialisieren

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
double AbstandX;
double AbstandY;
double WinkelToreGes;
int TorHoehe;
int TorHoehe2;

bool piread2;

long alteZeit;                                        //Variablen für Gyro
int alterWinkel;
double minus;
double rotation;
double accel;
int counter;

int IR[16];                                           //Variablen für IR
double IRbest = 100;    //0
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
PID entfPID(&IRbest,&entfVelo,&entfSet,4.1,0,0.8,REVERSE);        //LILA:4.1,0,0.8; Schwarz: 3.9,0,0.7
double wiSet=0;                                                   //Setpoint des Winkelpids (vorne)
double wiIn;                                                      //Inpunt des Winkelpids
PID wiPID(&wiIn,&wiVelo,&wiSet,0.8,0,0.28,REVERSE);               //LILA:0.8,0,0.28; Schwarz: …
double offsetVorne=11;                                  //LILA: 10; Schwarz: 10

bool buttonGpressed = true;                           //other

void setup() {
  /*while (!SD.begin(BUILTIN_SDCARD)) {                     //SD-Karte initialisieren
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
  myFile.close();*/
  
  Serial.begin(115200);                         //Seriellen Monitor initialisieren
  Serial3.begin(115200);                        
  Serial1.begin(115200);                        //Bluetooth initialisieren
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
  pinMode(LEDI, OUTPUT);                        //Die Button-Pins an einen Pullup-Widerstand hängen
  pinMode(ButtonI, INPUT_PULLUP);
  pinMode(ButtonII, INPUT_PULLUP);
  pinMode(ButtonIII, INPUT_PULLUP);
  pinMode(ButtonIV, INPUT_PULLUP);              //IR-Kalibration
  pinMode(LichtSchranke,INPUT);
  AutoCalibration(LED,Schwellwerte);
  gyro.begin(/*8*/);                            //den gyro losmessen lassen (ich musste die 8 auskommentieren, es funktioniert trotzdem)
  entfPID.SetMode(AUTOMATIC);
  wiPID.SetMode(AUTOMATIC);

  if(Roboter==LILA){
    torwart=true;
    entfPID.SetTunings(3.9,0,0.8);
    wiPID.SetTunings(0.74,0,0.38);  //0.365
    offsetVorne=22; //26
    Serial.print("LILA:    ");Serial.println(wiPID.GetKd());
  }else{ //black
    torwart=false;
    entfPID.SetTunings(3.9,0,0.8);
    wiPID.SetTunings(0.74,0,0.38); //25
    offsetVorne=22;
    Serial.println("BLACK:    ");Serial.println(wiPID.GetKd());
  }
  minWertLS=analogRead(LichtSchranke)-5;
  delay(100);
  pixy.init(0x54);
  pixy2.init(0x53);
}

#define bt true
#define Schusswinkel 8

void loop() {
  //torwart=false;
  position(WinkelToreGes,AbstandX,AbstandY,pixy2,piread2,pixy,piread,TorHoehe,TorHoehe2); //aufrufen der Postionsbestimmungsfunktion
  digitalWrite(Schuss_FW,HIGH);
  digitalWrite(Schuss_RW,LOW);
  analogWrite(Schuss_PWM,255);
  bool hBall= hatBall(minWertLS) && ( Icball == 0 || Icball == 15 || Icball == 1);
  IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);//
   if(bt){
     int bufBest=(int)IRbest;
     bluetooth(torwart,bufBest);                                                                                                        //empfangen und senden
   }
  ControlLEDs(buttonGpressed,richtung,IRbest,Icball,rotation,minEinerDa,irAutoCalibration,IRsave, hBall, torwart);                                  //Die grünen Kontroll-LEDs leuchten lassen& Knöpfe überprüfen

  if (hBall) {                                                                //Ermitteln ob er den Ball hat  hBall
    IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);  //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,rotation,alterWinkel, addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
    if((accel >=-10)&&(accel<=5)&&counter==75){
      if (bodenrichtung == -1) {
        PixyG=Pixy(pixy,piread,TorHoehe);
        PixyG2 = Pixy2(pixy2,piread2,TorHoehe2);                                                                                                    //Pixy auslesen
        compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,true,PixyG,PixyG2,hBall,torwart,accel);                                                   //Ausrichtungs-Funktion aufrufen, wobei die Kamera beachtet werden soll
        Serial.println("toranfahrt");
        if((abs(PixyG)+abs(PixyG2))>30&&PixyG<0){
          addRot = 90;
          compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
          motor(0,180,rotation);
          Serial.println("links");
        }else if((abs(PixyG)+abs(PixyG2))>30&&PixyG>0){
          addRot = -90;
          compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
          Serial.println("rechts");
          motor(180,180,(rotation));
        }else{
          Serial.println("Mitte");
          Serial.println(PixyG);
          Serial.println(PixyG2);
        
        }
        counter = 0;
      }
      else{                                                                                                                          //der Boden sieht etwas
      motor(bodenrichtung, 200,rotation);                                                                                           //sehr schnell von der Linie wegfahren
      }
    }
    else if((accel >=-10)&&(accel<=5)){
      counter++;
    }else{
      counter=0;
    }
      if (piread) {                                                                                                                 //sieht die pixy etwas
        if(PixyG<Schusswinkel&&PixyG>-Schusswinkel){
          digitalWrite(Schuss_FW,LOW);
          digitalWrite(Schuss_RW,HIGH);
          analogWrite(Schuss_PWM,255);
          delay(20);

          digitalWrite(Schuss_FW,HIGH);
          digitalWrite(Schuss_RW,LOW);
          analogWrite(Schuss_PWM,0);
        }
        motor(90-PixyG, 100,rotation);                                                                                                    //aufs Tor zufahren, mit Ausrichtung aufs Tor
        // motor(0,0,rotation);
      }else {
        compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,false,PixyG,PixyG2,hBall,torwart,accel);                                                //Ausrichtungs-Funktion aufrufen, wobei die Kamera NICHT beachtet werden soll
        motor(90,90, rotation/3);                                                                                                   //nach vorne fahren (abgeschwächte Ausrichtung)
        // motor(0,0,rotation);
      }
  
    }
  else if(torwart){    //torwart                                                                                                          //Als Torwart verhalten
    //Serial.println(IRbest);
    bodenlesen(minEinerDa,LED,Schwellwerte,Photo);
    // motor(90,0,10);
    torwartProgramm(pixy2,LED,Schwellwerte,rotation,gyro,buttonGpressed,minus,alterWinkel,addRot,piread,PixyG2,PixyG,IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,WinkelBall,IRsave,hBall,torwart,accel,TorHoehe,TorHoehe2,entfSet,wiPID,addRotTime);
  }else{    //!torwart
    piread=false;
    //Serial.println(IRbest);
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,rotation,alterWinkel, addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,piread,PixyG,PixyG2,hBall,false,accel); 
    IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);  //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen                                                                                                                 //Pixy soll nicht beachtet werden
    //Serial.println(WinkelBall);
    entfPID.Compute();                                                                                                              //die PIDs (für Ballanfahrt) berechnen
    wiPID.Compute();
    if (bodenrichtung == -1) {                                                                                                      //der Boden sieht nichts
      if (richtung != -1) {                                                                                                         //der IR sieht etwas
      //Serial.println(Icball);
        if(Icball == 0){                                                                                          //Ball Vor dem Roboter
          int delay=50;                                                                                                             //Verzögerung der temporären Ballausrichtung, wenn er gerade ist
          if(addRot!=0){                                                                                                            //wenn er schon gedreht ist, soll er sich die temporäre später ändern
            delay=1200;
          }
          if(WinkelBall>=90&&WinkelBall<270){
            if(addRotTime+delay<millis()){
              addRot=0; //-
              addRotTime=millis();
            }
          }else{
            if(addRotTime+delay<millis()){
              addRot=0;
              addRotTime=millis();
            }
          }
          motor(90,95,rotation);
          //Serial.println("fornt");
        }else if(Icball == 1){
          motor(90-offsetVorne,90,rotation);
          //Serial.println("slightly right");
        }else if(Icball == 15){
          motor(90+offsetVorne,90,rotation);
          //Serial.println("slightly left");
        }else{
          if(((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo>170){
            motor(richtung,170,rotation);
          }
          motor(richtung-addRot,((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo,rotation);      //Konvexkombination über die beiden PID-Geschwindigkeiten; t ist normierte Entf. zum Ball
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
      else {                                                              //der IR sieht nichts
        motor(0, 0,rotation);                                             //nur ausrichten
      }
    }
    else {                                                                //der Boden sieht etwas
      motor(bodenrichtung, 200,rotation);                                 //sehr schnell von der Linie wegfahren
    }
  }
}