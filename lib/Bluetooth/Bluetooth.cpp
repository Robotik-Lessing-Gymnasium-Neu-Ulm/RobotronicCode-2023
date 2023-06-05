#include <Bluetooth.h>
#include<Wire.h>
#include<Defines.h>

void bluetooth(bool& torwart, int IRbest) {
   static int blueCount;             //nur jeden 30. Aufruf senden
   static int torwartarr[10];        //(bool)torwart glätten
   if (blueCount == 25) {
    for(int i=9;i>0;i--){
     torwartarr[i]=torwartarr[i-1];
    }
     if (Serial1.availableForWrite()) {
       if (IRbest > 0) {             //senden
         Serial1.write(IRbest);
       } else {
         Serial1.write(200);           //ich glaube das ist nicht mit Absicht, besser wäre einen großen Wert zu senden, weil dieser Roboter sieht den Ball nicht (das ist aber eine Taktikfrage)
       }
     }
     if (Serial1.available()) {
       if (Serial1.read() < IRbest) { //empfangen und mit eigenem Abstand zum Ball vergleichen
         Serial.println("Torwart");
         torwartarr[0] = 1;
       } else {
         Serial.println("Stuermer");
         torwartarr[0] = 0;
       }
       Serial1.flush();
     }
     int t=0;
     for(int i=0;i<10;i++){            //glätten
       t+=torwartarr[i];
     }
     torwart=(t>5);                    //das kann noch genauer eingestellt werden
     blueCount = 0;
   }else {
     blueCount++;
   }
   }
    

/*#define Schwellwert 5  //Erst TW/Stürmer wechseln, wenn der andere um 'Schwellwert' besser ist

void bluetooth(bool& torwart, int IRbest) {
  constexpr size_t si=10;
  static bool torbufs[si];
  static unsigned long t=0;
  if(millis()>=t+200){
    if(Roboter==LILA){ //-> aktiv!
      static int bufl;
      if(Serial1.available()){
        bufl=Serial1.read();
      }
      Serial.print("Receive: ");Serial.print(bufl);Serial.print("</>"); Serial.print(IRbest);
      for(int i=si-1;i>0;i--){
        torbufs[i]=torbufs[i-1];
      }
      torbufs[0]=!(bufl>IRbest);
      int count=0;
      for(int i=0;i<si;i++){
        if(torbufs[i]){
          count++;
        }
      }
      torwart=(count>si/2);
      Serial.print("   |"); Serial.println(torwart);
      if(Serial1.availableForWrite()){
        if(torwart){
          Serial1.write(234);
        }else{
          Serial1.write(235);
        }
      }
    }else{ //black -> passiv!
      if(Serial1.availableForWrite()){
        int buWri=IRbest;
        if(buWri<0||buWri>200){
          buWri=200;
        }
        Serial1.write(buWri);
        Serial.print("Send: ");Serial.print(buWri);
      }
      if(Serial1.available()){
        int buf=Serial1.read();
        Serial.print("Receive: ");Serial.print(buf);
        if(buf==234){
          torwart=false;
        }else if(buf=235){
          torwart=true;
        }
      }
      Serial.print("   |"); Serial.println(torwart);
    }
    t=millis();
  }
}*/
/*
void bluetooth(bool& torwart,bool& hBall){
  if(torwart && hBall){
    Serial1.write(1);
    torwart = false;
  }if(!torwart){
    if(Serial1.available()){
      if(Serial1.read() == 1){
        torwart = true;
      }
    }
  }
}
*/