#include <Bluetooth.h>
#include<Wire.h>

void bluetooth(bool& torwart, double& IRbest) {
  static int blueCount;
  static int torwartarr[10];
  for(int i=9;i>0;i--){
    torwartarr[i]=torwartarr[i-1];
  }
  if (blueCount == 30) {
    if (Serial5.availableForWrite()) {
      if (IRbest > 0) {
        Serial5.write((int)IRbest);
      } else {
        Serial5.write(0);
      }
    }
    blueCount = 0;
  }
  else {
    blueCount++;
  }
  if (Serial5.available()) {
    if (Serial5.read() <= IRbest) {
      Serial.println("Torwart");
      torwartarr[0] = 1;
    } else {
      Serial.println("Stuermer");
      torwartarr[0] = 0;
    }
    Serial5.read();
  }
  int t=0;
  for(int i=0;i<10;i++){
    t+=torwartarr[i];
  }
  torwart=(t>3);
}