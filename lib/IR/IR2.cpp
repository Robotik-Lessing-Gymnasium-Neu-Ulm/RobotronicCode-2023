#include<IR2.h>
#include<Arduino.h>
#include<PID_v1.h>

//Multiplexer Unten
#ifndef S0
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
#define AM1 A17 //analog Multiplexer Oben
#endif

#ifndef RoboRadius
#define RoboRadius 20
#endif
#ifndef AnfahrtsRadius
#define AnfahrtsRadius 27
#endif

int lesenMultiplexerOben(int s0, int s1, int s2, int s3) {  //Verkürzung Auslesen
  digitalWrite(S0, s3);
  digitalWrite(S1, s2);
  digitalWrite(S2, s1);
  digitalWrite(S3, s0);
  return analogRead(AM1);
}
void IRsens(int* IR, int& IRbest, int& Icball, double& richtung, double &veloAnf) {
  //alle IRs auslesen und mappen
  IR[0] = map(lesenMultiplexerOben(0, 0, 0, 0), 300, 1023, 0, 100);
  IR[1] = map(lesenMultiplexerOben(0, 0, 0, 1), 300, 1023, 0, 100);
  IR[2] = map(lesenMultiplexerOben(0, 0, 1, 0), 305, 1023, 0, 100);
  IR[3] = map(lesenMultiplexerOben(0, 0, 1, 1), 315, 1023, 0, 100);
  IR[4] = map(lesenMultiplexerOben(0, 1, 0, 0), 295, 1023, 0, 100);
  IR[5] = map(lesenMultiplexerOben(0, 1, 0, 1), 300, 1023, 0, 100);
  IR[6] = map(lesenMultiplexerOben(0, 1, 1, 0), 315, 1023, 0, 100);
  IR[7] = map(lesenMultiplexerOben(0, 1, 1, 1), 300, 1023, 0, 100);
  IR[8] = map(lesenMultiplexerOben(1, 0, 0, 0), 295, 1023, 0, 100);
  IR[9] = map(lesenMultiplexerOben(1, 0, 0, 1), 310, 1023, 0, 100);
  IR[10] = map(lesenMultiplexerOben(1, 0, 1, 0), 300, 1023, 0, 100);
  IR[11] = map(lesenMultiplexerOben(1, 0, 1, 1), 310, 1023, 0, 100);
  IR[12] = map(lesenMultiplexerOben(1, 1, 0, 0), 295, 1023, 0, 100);
  IR[13] = map(lesenMultiplexerOben(1, 1, 0, 1), 295, 1023, 0, 100);
  IR[14] = map(lesenMultiplexerOben(1, 1, 1, 0), 305, 1023, 0, 100);
  IR[15] = map(lesenMultiplexerOben(1, 1, 1, 1), 305, 1023, 0, 100);
  //bestimmen des niedrigsten, gemessenen Wertes und Speichern des Index in Icball
  IRbest = 90;
  for (int i = 0; i < 16; i++) {
    if (IR[i] < IRbest) {
      IRbest = IR[i];
      Icball = i;
    }
  }
  //Der Winkel zum Ball in Grad (Hier gehe ich davon aus, dass 0 vorne ist und dann gegen den Uhrzeigersinn gezählt wird)
  double WinkelBall=22.5*Icball+90;
  if(WinkelBall>=360){
    WinkelBall-=360;
  }
  //PID-Regler, der nur die Geschwindigkeit regelt (veloAnf – velocityAnfahrt)
  static double Ziel=0;
  static double BetrWinkel=WinkelBall-90; //Winkel zum Ball neu ausrichten
  PID veloPID(&BetrWinkel,&veloAnf,&Ziel,0.5,0,0,P_ON_M,DIRECT); //noch nicht eingestellt
  veloPID.SetMode(AUTOMATIC);
  //die Verschiebung nach unten ist noch nicht implementiert
  //richtung berechnen (Die "untere" Tangente mit dem Anfahrtskreis um den Ball)
  Serial.println(IRbest);
  if(IRbest>=90){
    richtung=-1;
  }
  else if(Icball<=8){//der ball liegt links vom Roboter
    //Richtung berechnen
    richtung =WinkelBall+asin(AnfahrtsRadius/(IRbest+RoboRadius));
    //verschiebe den neu ausgerichteten Winkel teilweise ins Negative
    if(BetrWinkel>300){
        BetrWinkel-=360;
    }
    veloPID.SetControllerDirection(DIRECT);
    veloPID.Compute();
  }
  else{//der ball liegt rechts vom Roboter
    //Richtung berechnen
    richtung =WinkelBall-asin(AnfahrtsRadius/(IRbest+RoboRadius));
    //verschiebe den neu ausgerichteten Winkel teilweise ins Negative
    if(BetrWinkel<60){ //überprüfe das noch mal
        BetrWinkel+=360;
    }
    veloPID.SetControllerDirection(REVERSE);
    veloPID.Compute(); //jetzt ist der als Output: veloAnf
  }
  if(Icball==0){//wenn der Ball vor dem Roboter liegt
    veloAnf=100;
    richtung=90;
  }
}