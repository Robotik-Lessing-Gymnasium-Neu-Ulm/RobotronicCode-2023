#include <Bluetooth.h>
#include<Wire.h>

void bluetooth(bool& torwart, double& IRbest) {
  static int blueCount;             //nur jeden 30. Aufruf senden
  static int torwartarr[10];        //(bool)torwart glätten
  for(int i=9;i>0;i--){
    torwartarr[i]=torwartarr[i-1];
  }
  if (blueCount == 30) {
    if (Serial5.availableForWrite()) {
      if (IRbest > 0) {             //senden
        Serial5.write((int)IRbest);
      } else {
        Serial5.write(0);           //ich glaube das ist nicht mit Absicht, besser wäre einen großen Wert zu senden, weil dieser Roboter sieht den Ball nicht (das ist aber eine Taktikfrage)
      }
    }
    blueCount = 0;
  }else {
    blueCount++;
  }
  if (Serial5.available()) {
    if (Serial5.read() <= IRbest) { //empfangen und mit eigenem Abstand zum Ball vergleichen
      Serial.println("Torwart");
      torwartarr[0] = 1;
    } else {
      Serial.println("Stuermer");
      torwartarr[0] = 0;
    }
    Serial5.read();
  }
  int t=0;
  for(int i=0;i<10;i++){            //glätten
    t+=torwartarr[i];
  }
  torwart=(t>3);                    //das kann noch genauer eingestellt werden
}