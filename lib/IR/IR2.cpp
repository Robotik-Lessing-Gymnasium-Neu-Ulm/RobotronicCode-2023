#include<IR2.h>
#include<Arduino.h>
#include<PID_v1.h>
#include<SD.h>
#include <ArduinoJson.h>


#ifndef S0          //Multiplexer Unten
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

#ifndef AM1
#define AM1 A14     //analog Multiplexer Oben
#endif


int lesenMultiplexerOben(int s0, int s1, int s2, int s3) {           //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(AM1);
}
void IRsens(int* IR, double& IRbest, int& Icball, double& richtung,double &entfSet, double &wiIn, PID &wiPID, int* minWert, bool& irAutoCalibration, double& addRot, double& WinkelBall, unsigned long& addRotTime, bool& torwart) {
  if(!irAutoCalibration){
    static double AnfahrtsRadius=9;                                   //Achtung: auch bei der IR Kalibration ändern!
    static double BallWegRadius=195;
    entfSet=AnfahrtsRadius+3;
    // static int min=1023;
    // int gelesen=lesenMultiplexerOben(0,1,0,0);
    // if(min>gelesen){
    //   min=gelesen;
    //   Serial.println(gelesen);
    // }
    //static unsigned int count[16];
    IR[0] = map(lesenMultiplexerOben(0, 0, 0, 0), minWert[0] , 1023, 0, 200);   //alle IRs auslesen und mappen (auto-kalibration)
    IR[1] = map(lesenMultiplexerOben(0, 0, 0, 1), minWert[1] , 1023, 0, 200);
    IR[2] = map(lesenMultiplexerOben(0, 0, 1, 0), minWert[2] , 1023, 0, 200);
    IR[3] = map(lesenMultiplexerOben(0, 0, 1, 1), minWert[3] , 1023, 0, 200);
    IR[4] = map(lesenMultiplexerOben(0, 1, 0, 0), minWert[4] , 1023, 0, 200);
    IR[5] = map(lesenMultiplexerOben(0, 1, 0, 1), minWert[5] , 1023, 0, 200);
    IR[6] = map(lesenMultiplexerOben(0, 1, 1, 0), minWert[6] , 1023, 0, 200);
    IR[7] = map(lesenMultiplexerOben(0, 1, 1, 1), minWert[7] , 1023, 0, 200);
    IR[8] = map(lesenMultiplexerOben(1, 0, 0, 0), minWert[8] , 1023, 0, 200);
    IR[9] = map(lesenMultiplexerOben(1, 0, 0, 1), minWert[9] , 1023, 0, 200);
    IR[10] = map(lesenMultiplexerOben(1, 0, 1, 0),minWert[10], 1023, 0, 200);
    IR[11] = map(lesenMultiplexerOben(1, 0, 1, 1),minWert[11], 1023, 0, 200);
    IR[12] = map(lesenMultiplexerOben(1, 1, 0, 0),minWert[12], 1023, 0, 200);
    IR[13] = map(lesenMultiplexerOben(1, 1, 0, 1),minWert[13], 1023, 0, 200);
    IR[14] = map(lesenMultiplexerOben(1, 1, 1, 0),minWert[14], 1023, 0, 200);
    IR[15] = map(lesenMultiplexerOben(1, 1, 1, 1),minWert[15], 1023, 0, 200);
    IRbest = 200;                                                        //bestimmen des niedrigsten, gemessenen Wertes und Speichern des Index in Icball
    for (int i = 0; i < 16; i++) {
      if (IR[i] < IRbest) {
        IRbest = IR[i];
        Icball = i;
      }
    }
    for(int i=0;i<16;i++){
      if(IR[i]<0){
        minWert[i]--;
      }
    }
    Serial.println(IRbest);
    // if(!torwart){
      for(int i=0;i<16;i++){
        
        //   if(IR[i]>10){
        //     count[i]++;
        //   }
        //   if(count[i]>=2.1*minWert[i]){
        //     minWert[i]+=2;
        //     count[i]=0;
        //   }
      }
    // }
    WinkelBall=90-22.5*Icball+addRot;                                   //Berechnen des Winkels zum Ball
    if(WinkelBall<=0){                                                  //auf Wertebereich 0-360 verschieben
      WinkelBall+=360;
    }
    wiIn=WinkelBall-90;                                                 //Winkel berechnen, sodass der Vorzeichen-Wechsel hinten beim Roboter liegt
    if(wiIn>180){
      wiIn-=360;
    }
    // static unsigned long lastSave=0;                                      //Nur alle 5 Sek speichern
    // //alle 5sec speichern
    // if(millis()>=lastSave+5'000){
    //   Serial.println("Speichern(IR)");
    //   Serial.println(minWert[0]);
    //   File myFile_=SD.open("minWerte.json",FILE_READ);                    //Datei öffnen, lesen
    //     char buf_[myFile_.size()];                                        //Zwischenspeicher des Inhalts der geöffneten *.json-Datei
    //     myFile_.read(buf_,myFile_.size());                                //Buffer mit Dateiinhalt befüllen
    //     StaticJsonDocument<1000> doc_;                                    //Json aus geöffneter Datei
    //     deserializeJson(doc_, buf_);
    //   myFile_.close();
    //   File s = SD.open("minWerte.json", FILE_WRITE);                      //Datei öffnen, schreiben
    //     s.truncate();                                                     //Datei leeren
    //     for(int i=0;i<16;i++){                                            //Json-Datei mit neuen Daten füllen
    //       doc_["IR"][i]=minWert[i];
    //     }
    //     char b[500];                                                      //Buffer, der in die geöffnete *.json-Datei geschrieben wird
    //     for(int i{0};i<500;i++){                                          //Vordefinieren des buffers mit Leerzeichen
    //       b[i]=' ';
    //     }
    //     serializeJsonPretty(doc_,b);                                      //Json in Text übersetzen
    //     for(auto elem:b){                                                 //Datei mit Buffer befüllen
    //       s.write(elem);
    //     }
    //   s.close();
    //   lastSave=millis();
    // }
    if(IRbest>BallWegRadius){                                           //Wenn er den Ball nicht sieht
      richtung=-1;
      addRot=0;
      WinkelBall=90;
      return;
    }
    if(WinkelBall<0){
      WinkelBall+=360;
    }
    if(IRbest<1.5*AnfahrtsRadius){
      int delay=80;
      if(addRot!=0){
        delay=1300;
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
    }
    // Serial.print(addRot);Serial.print("|");
    if(IRbest<AnfahrtsRadius){                                          //Wenn der Roboter im Anfahrtskreis steht
      richtung=270-addRot;                                                     //nach hinten fahren
      return;
    }
    if(WinkelBall<=90||WinkelBall>=270){                                //Ball rechts vom Roboter
      richtung=WinkelBall-(asin((double)AnfahrtsRadius/IRbest))*180/PI; //auf der unteren Tangente fahren
      wiPID.SetControllerDirection(DIRECT);
    }else{                                                              //Ball links vom Roboter
      richtung=WinkelBall+(asin((double)AnfahrtsRadius/IRbest))*180/PI; //auf der unteren Tangente fahren
      wiPID.SetControllerDirection(REVERSE);
    }
    if(richtung<0){                                                     //auf Wertebereich 0-360 verscheiben
      richtung+=360;
    }
    if(richtung>360){
      richtung-=360;
    }
  }else{
    irAutoCal(minWert);
    for(int i=0;i<16;i++){
      Serial.print(minWert[i]);
      Serial.print("|");
    }
    Serial.println(";");
  }
}

void irAutoCal(int* minWert){
  if(minWert[0]>lesenMultiplexerOben(0, 0, 0, 0)){minWert[0]=lesenMultiplexerOben(0, 0, 0, 0);}
  if(minWert[1]>lesenMultiplexerOben(0, 0, 0, 1)){minWert[1]=lesenMultiplexerOben(0, 0, 0, 1);}
  if(minWert[2]>lesenMultiplexerOben(0, 0, 1, 0)){minWert[2]=lesenMultiplexerOben(0, 0, 1, 0);}
  if(minWert[3]>lesenMultiplexerOben(0, 0, 1, 1)){minWert[3]=lesenMultiplexerOben(0, 0, 1, 1);}
  if(minWert[4]>lesenMultiplexerOben(0, 1, 0, 0)){minWert[4]=lesenMultiplexerOben(0, 1, 0, 0);}
  if(minWert[5]>lesenMultiplexerOben(0, 1, 0, 1)){minWert[5]=lesenMultiplexerOben(0, 1, 0, 1);}
  if(minWert[6]>lesenMultiplexerOben(0, 1, 1, 0)){minWert[6]=lesenMultiplexerOben(0, 1, 1, 0);}
  if(minWert[7]>lesenMultiplexerOben(0, 1, 1, 1)){minWert[7]=lesenMultiplexerOben(0, 1, 1, 1);}
  if(minWert[8]>lesenMultiplexerOben(1, 0, 0, 0)){minWert[8]=lesenMultiplexerOben(1, 0, 0, 0);}
  if(minWert[9]>lesenMultiplexerOben(1, 0, 0, 1)){minWert[9]=lesenMultiplexerOben(1, 0, 0, 1);}
  if(minWert[10]>lesenMultiplexerOben(1, 0, 1, 0)){minWert[10]=lesenMultiplexerOben(1, 0, 1, 0);}
  if(minWert[11]>lesenMultiplexerOben(1, 0, 1, 1)){minWert[11]=lesenMultiplexerOben(1, 0, 1, 1);}
  if(minWert[12]>lesenMultiplexerOben(1, 1, 0, 0)){minWert[12]=lesenMultiplexerOben(1, 1, 0, 0);}
  if(minWert[13]>lesenMultiplexerOben(1, 1, 0, 1)){minWert[13]=lesenMultiplexerOben(1, 1, 0, 1);}
  if(minWert[14]>lesenMultiplexerOben(1, 1, 1, 0)){minWert[14]=lesenMultiplexerOben(1, 1, 1, 0);}
  if(minWert[15]>lesenMultiplexerOben(1, 1, 1, 1)){minWert[15]=lesenMultiplexerOben(1, 1, 1, 1);}
}