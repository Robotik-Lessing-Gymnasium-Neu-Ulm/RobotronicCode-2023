#include<Ballverfolgung.h>
#include<Defines.h>
#include<PID_v1.h>
#include<Arduino.h>
#include<Boden.h>
#include<Kompass.h>
#include<IR2.h>
#include<Motoren.h>

void verfolgeBall(double& IRbest, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation,double &addRot, bool piread, int PixyG, int PixyG2, bool hBall, bool torwart, int* IR, int& Icball, double& richtung,double &entfSet, double &wiIn, int* minWert, bool& irAutoCalibration, double& WinkelBall, unsigned long& addRotTime, bool& IRsave, bool& RetSurface,double& accel){
    static bool setup{true};
    if(setup){
        entfPID.SetMode(AUTOMATIC);
        wiPID.SetMode(AUTOMATIC);

        if(Roboter==LILA){
            entfPID.SetTunings(1,0,0);
            wiPID.SetTunings(1,0,0);
            // entfPID.SetTunings(3.9,0,0.8);
            // wiPID.SetTunings(0.74,0,0.38);
            offsetVorne=22; //26
            Serial.print("LILA:    ");Serial.println(wiPID.GetKd());
        }else{ //black
            // entfPID.SetTunings(0.271,0.001,0.02);
            // wiPID.SetTunings(0.102,0.001,0.03); //0.102,0.001,0.04
            entfPID.SetTunings(0.47,0,0.16);   //0.4,0,0.23;0.5,0,0.39;0.47,0,0.16
            entfPID.SetOutputLimits(0,11);
            wiPID.SetTunings(0.21,0,0.05);  //0.1247,0,0.08;0.126,0,0.09;0.19,0,0.1;0.21,0.07,0
            wiPID.SetOutputLimits(0,9);
            offsetVorne=24;
            Serial.println("BLACK:    ");Serial.println(wiPID.GetKd());
        }
    }
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,PixyG2,hBall,torwart,accel);
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel, addRot,piread,PixyG,PixyG2,hBall,false,accel); 
    IRsens(IR,IRbest,Icball,richtung,entfSet,wiIn,wiPID,minWert,irAutoCalibration, addRot,WinkelBall, addRotTime, torwart,IRsave);  //die IR/Boden/Kompass-Sensoren messen und abspeichern lassen                                                                                                                 //Pixy soll nicht beachtet werden
    //Serial.println(WinkelBall);
    entfPID.Compute();                                                                                                              //die PIDs (für Ballanfahrt) berechnen
    wiPID.Compute();
    if (bodenrichtung == -1) {                                                                                                      //der Boden sieht nichts
      if (richtung != -1) {                                                                                                         //der IR sieht etwas
        if(Icball == 0){                                                                                                            //Ball Vor dem Roboter
          int delay=50;                                                                                                             //Verzögerung der temporären Ballausrichtung, wenn er gerade ist
          if(addRot!=0){                                                                                                            //wenn er schon gedreht ist, soll er sich die temporäre später ändern
            delay=1200;
          }
          if(WinkelBall>=90&&WinkelBall<270){
            if(addRotTime+delay<millis()){
              addRot=0;
              addRotTime=millis();
            }
          }else{
            if(addRotTime+delay<millis()){
              addRot=0;
              addRotTime=millis();
            }
          }
          // motor(90,95,rotation);
          fahren(90,5.5,0,gyro,buttonGpressed,RetSurface);
          // Serial.println("front");
        }else if(Icball == 15){
          fahren(90-35,4.7,0,gyro,buttonGpressed,RetSurface);
          // motor(90-offsetVorne,90,rotation);
          // Serial.println("slightly right");
        }else if(Icball == 1){
          fahren(90+35,4.7,0,gyro,buttonGpressed,RetSurface);
          // motor(90+offsetVorne,90,rotation);
          // Serial.println("slightly left");
        }else{
          // Serial.print(entfVelo);Serial.print(" | ");
          // Serial.print(wiVelo);Serial.print(" | ");
          // Serial.println(((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo);
          // fahren(richtung-addRot,entfVelo,0,gyro,buttonGpressed);        //aus Testzwecken nur Entfernung/Winkel reglen
          fahren(richtung-addRot,((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo,0,gyro,buttonGpressed,RetSurface);      //Konvexkombination über die beiden PID-Geschwindigkeiten; t ist normierte Entf. zum Ball
          // motor(richtung-addRot,((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo,rotation);      //Konvexkombination über die beiden PID-Geschwindigkeiten; t ist normierte Entf. zum Ball
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

    setup=false;
}