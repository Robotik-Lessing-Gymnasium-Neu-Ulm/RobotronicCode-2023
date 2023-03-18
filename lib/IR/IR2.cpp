#include<IR2.h>
#include<Arduino.h>
#include<PID_v1.h>

#ifndef S0          //Multiplexer Unten
#define S0 17
#endif
#ifndef S1
#define S1 16
#endif
#ifndef S2
#define S2 15
#endif
#ifndef S3
#define S3 14
#endif

#ifndef AM1
#define AM1 A17     //analog Multiplexer Oben
#endif

#ifndef AnfahrtsRadius
#define AnfahrtsRadius 15
#endif

int lesenMultiplexerOben(int s0, int s1, int s2, int s3) {           //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(AM1);
}
void IRsens(int* IR, int& IRbest, int& Icball, double& richtung, double &veloAnf) {
  IR[0] = map(lesenMultiplexerOben(0, 0, 0, 0), 409, 1023, 0, 100);   //alle IRs auslesen und mappen
  IR[1] = map(lesenMultiplexerOben(0, 0, 0, 1), 413, 1023, 0, 100);
  IR[2] = map(lesenMultiplexerOben(0, 0, 1, 0), 416, 1023, 0, 100);
  IR[3] = map(lesenMultiplexerOben(0, 0, 1, 1), 410, 1023, 0, 100);
  IR[4] = map(lesenMultiplexerOben(0, 1, 0, 0), 408, 1023, 0, 100);
  IR[5] = map(lesenMultiplexerOben(0, 1, 0, 1), 410, 1023, 0, 100);
  IR[6] = map(lesenMultiplexerOben(0, 1, 1, 0), 420, 1023, 0, 100);
  IR[7] = map(lesenMultiplexerOben(0, 1, 1, 1), 415, 1023, 0, 100);
  IR[8] = map(lesenMultiplexerOben(1, 0, 0, 0), 409, 1023, 0, 100);
  IR[9] = map(lesenMultiplexerOben(1, 0, 0, 1), 411, 1023, 0, 100);
  IR[10] = map(lesenMultiplexerOben(1, 0, 1, 0), 409, 1023, 0, 100);
  IR[11] = map(lesenMultiplexerOben(1, 0, 1, 1), 412, 1023, 0, 100);
  IR[12] = map(lesenMultiplexerOben(1, 1, 0, 0), 412, 1023, 0, 100);
  IR[13] = map(lesenMultiplexerOben(1, 1, 0, 1), 407, 1023, 0, 100);
  IR[14] = map(lesenMultiplexerOben(1, 1, 1, 0), 410, 1023, 0, 100);
  IR[15] = map(lesenMultiplexerOben(1, 1, 1, 1), 240, 1023, 0, 100);
  IRbest = 90;                                                        //bestimmen des niedrigsten, gemessenen Wertes und Speichern des Index in Icball
  for (int i = 0; i < 16; i++) {
    if (IR[i] < IRbest) {
      IRbest = IR[i];
      Icball = i;
    }
  }
  double WinkelBall=90-22.5*Icball;                                   //Berechnen des Winkels zum Ball
  if(WinkelBall>=360){                                                //auf Wertebereich 0-360 verschieben
    WinkelBall-=360; 
  }
  if(WinkelBall<0){
    WinkelBall+=360;
  }
  if(IRbest<AnfahrtsRadius){                                          //Wenn der Roboter im Anfahrtskreis steht
    richtung=270;                                                     //nach hinten fahren
    veloAnf=100;
  }else if(Icball==0){                                                //Ball vor dem Roboter
    richtung=90;                                                      //nach vorne fahren
    veloAnf=150;
  }else if(Icball<=8){                                                //Ball rechts vom Roboter; Der RoboRadius von 4 wurde noch miteinbezogen
    richtung=WinkelBall-(asin((double)AnfahrtsRadius/IRbest))*180/PI;
  }else{                                                              //Ball links vom Roboter
    richtung=WinkelBall+(asin((double)AnfahrtsRadius/IRbest))*180/PI;
  }
  if(richtung<0){                                                     //auf Wertebereich 0-360 verscheiben
    richtung+=360;
  }
  if(richtung>360){
    richtung-=360;
  }
  Serial.println(richtung);
}