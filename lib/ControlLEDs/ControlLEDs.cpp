#include<ControlLEDs.h>
#include<Arduino.h>
#include<Lichtschranke.h>
#include<Boden.h>

void blink(uint8_t pin,bool direction){  //synchronisiert
  static long nextTrigger=0;
  static bool nextZustand=true;
  if(millis()>=nextTrigger){
    digitalWrite(pin,nextZustand==direction);
    nextZustand=!nextZustand;
    nextTrigger=millis()+500;
  }
}

void ControlLEDs(bool& buttonGpressed, double &richtung,double& IRbest, int& Icball, double& rotation,bool& minEinerDa, bool& irAutoCalibration, int* LED, int* Schwellwerte,  uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t ButtonII, uint8_t ButtonIII, uint8_t ButtonIV, uint8_t LEDir, uint8_t LEDballcaught, uint8_t LEDgyro, uint8_t Lichtschranke) {
  // Einzelne Variablen überprüfen und dann die Pins schreiben
  if (digitalRead(ButtonI) == LOW) {
    buttonGpressed = true;
  }
  if (digitalRead(ButtonIV) == LOW) {
    static long lastTrigger=0;
    if(lastTrigger+100<millis()){
      lastTrigger=millis();
      irAutoCalibration=!irAutoCalibration;
    }
  }
  if(digitalRead(ButtonIII)){
    AutoCalibration(LED,Schwellwerte,LEDboden,S0,S1,S2,S3,UAM1,UAM2,UAM3);
  }
  if(irAutoCalibration){
    blink(LEDir,1);
    blink(LEDboden,1);
    blink(LEDballcaught,1);
    blink(LEDgyro,1);
  }else{
    if (richtung >= 0) {
      digitalWrite(LEDir, HIGH);
    } else {
      digitalWrite(LEDir, LOW);
    }
    if (minEinerDa) {
      digitalWrite(LEDboden, HIGH);
    } else {
      digitalWrite(LEDboden, LOW);
    }
    if (hatBall(Lichtschranke) && IRbest <10&&( Icball == 0 || Icball == 15 || Icball == 1 )) {
      digitalWrite(LEDballcaught, HIGH);
    } else {
      digitalWrite(LEDballcaught, LOW);
    }
    if (rotation <= 2.5 && rotation >= -2.5) {
      digitalWrite(LEDgyro, HIGH);
    } else {
      digitalWrite(LEDgyro, LOW);
    }
  }
}