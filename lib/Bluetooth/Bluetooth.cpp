#include <Bluetooth.h>
#include<Wire.h>
#include<Defines.h>

void bluetooth(bool& torwart,bool& hBall,bool& surface){
  if(torwart && hBall){
    Serial1.write(1);
    torwart = false;
  }
  if(!torwart){
    if(Serial1.available()){
      bool wechsel = bool(Serial1.read());
      if(wechsel == 1){
        torwart = true;
      }else{
        torwart = false;
      }
    }
  }
  if(!surface){
    torwart = true;
    Serial1.write(0);
  }
}
