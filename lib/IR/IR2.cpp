#include<IR2.h>
#include<Arduino.h>
#include<PID_v1.h>
#include<SD.h>
#include <ArduinoJson.h>


int lesenMultiplexerOben(int s0, int s1, int s2, int s3, uint8_t S0, uint8_t S1,uint8_t S2,uint8_t S3,uint8_t AM1) {           //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(AM1);
}
void IRsens(int* IR, double& IRbest, int& Icball, double& richtung,double &entfSet, double &wiIn, PID &wiPID, int* minWert, bool& irAutoCalibration, double& addRot, double& WinkelBall, unsigned long& addRotTime, bool& torwart, uint8_t S0, uint8_t S1,uint8_t S2,uint8_t S3,uint8_t AM1) {
  if(!irAutoCalibration){
    static double AnfahrtsRadius=7;                                   //Achtung: auch bei der IR Kalibration ändern!
    static double BallWegRadius=75;
    entfSet=AnfahrtsRadius;
    // static int min=1023;
    // int gelesen=lesenMultiplexerOben(0,1,0,0);
    // if(min>gelesen){
    //   min=gelesen;
    //   Serial.println(gelesen);
    // }
    //static unsigned int count[16];
    IR[0] = map(lesenMultiplexerOben(0, 0, 0, 0,S0,S1,S2,S3,AM1), minWert[0] , 1023, 0, 200);   //alle IRs auslesen und mappen (auto-kalibration)
    IR[1] = map(lesenMultiplexerOben(0, 0, 0, 1,S0,S1,S2,S3,AM1), minWert[1] , 1023, 0, 200);
    IR[2] = map(lesenMultiplexerOben(0, 0, 1, 0,S0,S1,S2,S3,AM1), minWert[2] , 1023, 0, 200);
    IR[3] = map(lesenMultiplexerOben(0, 0, 1, 1,S0,S1,S2,S3,AM1), minWert[3] , 1023, 0, 200);
    IR[4] = map(lesenMultiplexerOben(0, 1, 0, 0,S0,S1,S2,S3,AM1), minWert[4] , 1023, 0, 200);
    IR[5] = map(lesenMultiplexerOben(0, 1, 0, 1,S0,S1,S2,S3,AM1), minWert[5] , 1023, 0, 200);
    IR[6] = map(lesenMultiplexerOben(0, 1, 1, 0,S0,S1,S2,S3,AM1), minWert[6] , 1023, 0, 200);
    IR[7] = map(lesenMultiplexerOben(0, 1, 1, 1,S0,S1,S2,S3,AM1), minWert[7] , 1023, 0, 200);
    IR[8] = map(lesenMultiplexerOben(1, 0, 0, 0,S0,S1,S2,S3,AM1), minWert[8] , 1023, 0, 200);
    IR[9] = map(lesenMultiplexerOben(1, 0, 0, 1,S0,S1,S2,S3,AM1), minWert[9] , 1023, 0, 200);
    IR[10] = map(lesenMultiplexerOben(1, 0, 1, 0,S0,S1,S2,S3,AM1),minWert[10], 1023, 0, 200);
    IR[11] = map(lesenMultiplexerOben(1, 0, 1, 1,S0,S1,S2,S3,AM1),minWert[11], 1023, 0, 200);
    IR[12] = map(lesenMultiplexerOben(1, 1, 0, 0,S0,S1,S2,S3,AM1),minWert[12], 1023, 0, 200);
    IR[13] = map(lesenMultiplexerOben(1, 1, 0, 1,S0,S1,S2,S3,AM1),minWert[13], 1023, 0, 200);
    IR[14] = map(lesenMultiplexerOben(1, 1, 1, 0,S0,S1,S2,S3,AM1),minWert[14], 1023, 0, 200);
    IR[15] = map(lesenMultiplexerOben(1, 1, 1, 1,S0,S1,S2,S3,AM1),minWert[15], 1023, 0, 200);
    IRbest = 90;                                                        //bestimmen des niedrigsten, gemessenen Wertes und Speichern des Index in Icball
    for (int i = 0; i < 16; i++) {
      if (IR[i] < IRbest) {
        IRbest = IR[i];
        Icball = i;
      }
    }
    // if(!torwart){
      for(int i=0;i<16;i++){
        if(IR[i]<0){
          minWert[i]--;
        }
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
    if(IRbest>BallWegRadius){                                           //Wenn er den Ball nicht sieht
      richtung=-1;
      addRot=0;
      WinkelBall=90;
      return;
    }
    if(WinkelBall<0){
      WinkelBall+=360;
    }
    if(IRbest<3*AnfahrtsRadius/2){
      int delay=50;
      if(addRot!=0){
        delay=1200;
      }
      if(WinkelBall>=90&&WinkelBall<270){
        if(addRotTime+delay<millis()){
          addRot=-20;
          addRotTime=millis();
        }
      }else{
        if(addRotTime+delay<millis()){
          addRot=20;
          addRotTime=millis();
        }
      }
    }
    if(IRbest<AnfahrtsRadius){                                          //Wenn der Roboter im Anfahrtskreis steht
      int delay=50;
      if(addRot!=0){
        delay=1200;
      }
      if(WinkelBall>=90&&WinkelBall<270){
        if(addRotTime+delay<millis()){
          addRot=-30;
          addRotTime=millis();
        }
      }else{
        if(addRotTime+delay<millis()){
          addRot=30;
          addRotTime=millis();
        }
      }
      richtung=270-addRot;                                              //nach hinten fahren
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

    static unsigned long lastSave=0;                                     //Nur alle 5 Sek speichern
    //speichern
    if(millis()>=lastSave+5'000){
      File file = SD.open("minWerte.json", FILE_WRITE | O_TRUNC  | O_CREAT); //Datei öffnen, schreiben|leeren|neu erstellen, falls nicht existent
        StaticJsonDocument<500> doc;
        int values[16];                                                     //ein echter C-Array für minWert
        for(int i=0;i<16;i++){
          values[i]=minWert[i];
        }
        JsonArray Jarr;                                                     //Json-Array von minWert
        copyArray(values, Jarr);
        doc["IR"]=Jarr;

        String buf{""};                                                     //Buffer, der in die geöffnete *.json-Datei geschrieben wird
        serializeJsonPretty(doc,buf);
        for(auto elem:buf){
          file.write(elem);
        }
      file.close();
      lastSave=millis();
    }
  }else{
    for(int i=0;i<16;i++){
      Serial.print(minWert[i]);
      Serial.print("|");
    }
    Serial.println(";");
  }
}