#include<Ballverfolgung.h>
#include<Defines.h>
#include<PID_v1.h>
#include<Arduino.h>
#include<Boden.h>
#include<Kompass.h>
#include<IR2.h>
#include<Motoren.h>

void verfolgeBall(double& IRbest, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa,int* LED,int* Schwellwerte, bool* Photo,bool* gesehenSensor,double& bodenrichtung,Adafruit_BNO055& gyro,bool& buttonGpressed, double& minus, long& alteZeit, int& alterWinkel, double& rotation,double &addRot, bool piread, int PixyG, int PixyG2, bool hBall, bool torwart, int* IR, int& Icball, double& richtung,double &entfSet, double &wiIn, int* minWert, bool& irAutoCalibration, double& WinkelBall, unsigned long& addRotTime, bool& IRsave, bool& RetSurface,double& accel){
  static const bool useMaus=(Roboter!=LILA);
  // static const bool useMaus=false;
  static double naheAnfahrt=useMaus?0.8:0.82;     //erster Wert: wenn die Maus aktiviert ist, zweiter wenn nicht; für den Winkel, wenn der Ball vorne fährt (gewichtung des Abstands)
  static double naheOffAnf=17;                      //für den Winkel, wenn der Ball vorne fährt
  static double GeschNahOff=useMaus?8.5:55;        //die Geschw., wenn der Ball vorne liegt (Sensor 15 oder 1), aber nicht exakt (nicht 0)
  static double GeschNahFakt=useMaus?0.1:0.5;                //Geschwindigkeit wenn der Ball bei Sensor 0 ist
  constexpr double torTeiler{1.12};
    // Serial.print(Icball);Serial.print(" ");
    // Serial.println(IRbest);
    static bool setup{true};
    if(setup){
        entfPID.SetMode(AUTOMATIC);
        wiPID.SetMode(AUTOMATIC);

        if(useMaus){
          // entfPID.SetTunings(0.3,0,0);
          // entfPID.SetOutputLimits(-14,19);
          // wiPID.SetTunings(0.1,0,0);
          // wiPID.SetOutputLimits(-24,24);

          // entfPID.SetTunings(0.4,0,0);
          // entfPID.SetOutputLimits(-14,19);
          // wiPID.SetTunings(0.5,0,0);
          // wiPID.SetOutputLimits(-24,24);

          // entfPID.SetTunings(0.4,0,0.09);
          // entfPID.SetOutputLimits(-14,19);
          // wiPID.SetTunings(1,0,0.095);
          // wiPID.SetOutputLimits(-24,24);

          entfPID.SetTunings(2.5,0,2);
          wiPID.SetTunings(2.5,0,0.98);
          entfPID.SetOutputLimits(-200,300);
          wiPID.SetOutputLimits(-200,300);

          naheAnfahrt=0.27;
          naheOffAnf=21;
          GeschNahOff=9;
          GeschNahFakt=0.08;

          // naheAnfahrt=0.2;
          // naheOffAnf=15;
          // GeschNahOff=8.5;
          // GeschNahFakt=0.1;
        }else{//!useMaus
          entfPID.SetTunings(2.5,0,2);
          wiPID.SetTunings(2.5,0,0.98);
          entfPID.SetOutputLimits(-200,300);
          wiPID.SetOutputLimits(-200,300);
          // naheAnfahrt=0.74;
          // naheOffAnf=17;
          // GeschNahOff=64;
          // GeschNahFakt=0.7;
          naheAnfahrt=0.58;
          naheOffAnf=10;
          GeschNahOff=70;
          GeschNahFakt=0.4;  //0.65
        }

        // if(Roboter==LILA){
        //     if(!useMaus){
        //       // entfPID.SetTunings(3.9,0,0.8);
        //       // wiPID.SetTunings(0.74,0,0.38);
        //       entfPID.SetTunings(2.5,0,2);
        //       wiPID.SetTunings(2.5,0,0.98);
        //       entfPID.SetOutputLimits(-200,300);
        //       wiPID.SetOutputLimits(-200,300);
        //       naheAnfahrt=0.7;
        //       naheOffAnf=16;
        //     }else{
        //       entfPID.SetTunings(0.47,0,0.16);   //0.4,0,0.23;0.5,0,0.39;0.47,0,0.16
        //       entfPID.SetOutputLimits(0,11);
        //       wiPID.SetTunings(0.21,0,0.09);  //0.1247,0,0.08;0.126,0,0.09;0.19,0,0.1;0.21,0.07,0
        //       wiPID.SetOutputLimits(0,9);
        //     }
        //     Serial.print("LILA:    ");Serial.println(wiPID.GetKd());
        // }else{ //black
        //     if(!useMaus){
        //       // entfPID.SetTunings(3.9,0,0.8);
        //       // wiPID.SetTunings(0.74,0,0.38);//2.5,0,1.5

        //       // entfPID.SetTunings(2.1,0,2);
        //       // wiPID.SetTunings(1.7,0,0.32);
        //       // entfPID.SetOutputLimits(-200,300);
        //       // wiPID.SetOutputLimits(-200,300);
        //       // naheAnfahrt=0.82;
        //       // naheOffAnf=17;
        //       entfPID.SetTunings(2.5,0,2);
        //       wiPID.SetTunings(2.5,0,0.98);
        //       entfPID.SetOutputLimits(-200,300);
        //       wiPID.SetOutputLimits(-200,300);
        //       naheAnfahrt=0.7;
        //       naheOffAnf=16;
        //     }else{
        //       // entfPID.SetTunings(0.47,0,0.16);   //0.4,0,0.23;0.5,0,0.39;0.47,0,0.16
        //       // entfPID.SetOutputLimits(0,11);
        //       // wiPID.SetTunings(0.21,0,0.05);  //0.1247,0,0.08;0.126,0,0.09;0.19,0,0.1;0.21,0.07,0
        //       // wiPID.SetOutputLimits(0,9);
        //       // naheAnfahrt=0.63;
        //       // naheOffAnf=24;

        //       // entfPID.SetTunings(0.25,0,0.0372);
        //       // entfPID.SetOutputLimits(-14,16);
        //       // wiPID.SetTunings(2.8,0,0.6);//0.285,0,0.06;0.38,0,0.088;0.455,0,0.1;0.84,0,0.25;2.8,0,0.593
        //       // wiPID.SetOutputLimits(-14,19);
        //       // naheAnfahrt=1;
        //       // naheOffAnf=23.5;

        //       entfPID.SetTunings(0.4,0,0.09);
        //       entfPID.SetOutputLimits(-14,19);
        //       wiPID.SetTunings(1,0,0.095);
        //       wiPID.SetOutputLimits(-24,24);
        //       naheAnfahrt=0.2;
        //       naheOffAnf=15;
        //     }
        //     offsetVorne=24;
        //     Serial.println("BLACK:    ");Serial.println(wiPID.GetKd());
        // }
    }
    Boden(minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,PixyG2,hBall,torwart,hBall,RetSurface,accel); //HIER
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
          if(useMaus){
            fahren(90,torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),0,gyro,buttonGpressed,RetSurface);
          }else{
            motor(90,torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),rotation);
          }
          Serial.println("front");
        }else if(Icball == 15){
          if(useMaus){
            fahren(90-min(IRbest*naheAnfahrt+naheOffAnf,89),torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),0,gyro,buttonGpressed,RetSurface); //90-35
            Serial.print(GeschNahFakt*IRbest+GeschNahOff);Serial.print("   ");
            Serial.println(90-min(IRbest*naheAnfahrt+naheOffAnf,89));
          }else{
            motor(90-min(IRbest*naheAnfahrt+naheOffAnf,89),torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),rotation);
          }
          Serial.println("slightly right");
        }else if(Icball == 1){
          if(useMaus){
            fahren(90+min(IRbest*naheAnfahrt+naheOffAnf,89),torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),0,gyro,buttonGpressed,RetSurface); //90+35
          }else{
            motor(90+min(IRbest*naheAnfahrt+naheOffAnf,89),torwart?(GeschNahFakt*IRbest+GeschNahOff)/torTeiler:(GeschNahFakt*IRbest+GeschNahOff),rotation);
          }
          Serial.println("slightly left");
        }else{
          // Serial.println(((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo);
          motor(richtung-addRot,((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo,rotation);      //Konvexkombination über die beiden PID-Geschwindigkeiten; t ist normierte Entf. zum Ball
          // Serial.println(((IRbest-entfSet)/(195-entfSet))*entfVelo+(1-(IRbest-entfSet)/(195-entfSet))*wiVelo);
          // motor(richtung-addRot,entfVelo,rotation);Serial.println(entfVelo);
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