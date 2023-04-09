#include<ControlLEDs.h>
#include<Arduino.h>
#include<Lichtschranke.h>
#include<Boden.h>

void blink(uint8_t pin,bool direction){           //synchronisiert
  static long nextTrigger=0;
  static bool nextZustand=true;
  if(millis()>=nextTrigger){                      //Wenn die Zeit gekommen ist
    digitalWrite(pin,nextZustand==direction);     //LED schreiben, Invertieren,falls direction==false
    nextZustand=!nextZustand;
    nextTrigger=millis()+500;
  }
}

void ControlLEDs(bool& buttonGpressed, double &richtung,double& IRbest, int& Icball, double& rotation,bool& minEinerDa, bool& irAutoCalibration, int* LED, int* Schwellwerte,  uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t UAM1, uint8_t UAM2,uint8_t UAM3, uint8_t LEDboden, uint8_t ButtonI, uint8_t ButtonII, uint8_t ButtonIII, uint8_t ButtonIV, uint8_t LEDir, uint8_t LEDballcaught, uint8_t LEDgyro, uint8_t Lichtschranke, char& PhaseLSKalibration, int minWertLS) {
  // Einzelne Variablen überprüfen und dann die Pins schreiben (Pullups)
  if (digitalRead(ButtonI) == LOW) {
    buttonGpressed = true;
  }
  if (digitalRead(ButtonIV) == LOW) {
    static long lastTrigger=0;                                  //um Ungenauigkeiten beim Drücken zu glätten
    if(lastTrigger+100<millis()){
      lastTrigger=millis();
      irAutoCalibration=!irAutoCalibration;                     //der Wert wird invertiert beim Drücken
    }
  }
  if(digitalRead(ButtonII)==LOW){                               //wie Button IV, wobei die Phase eins weiter gestellt wird
    static long LastTriggerLS=0;
    if(LastTriggerLS+100<millis()){
      LastTriggerLS=millis();
      PhaseLSKalibration++;
      if(PhaseLSKalibration>3){
        PhaseLSKalibration=0;
      }
    }
  }
  if(digitalRead(ButtonIII)==LOW){                              //direktes Aufrufen von 
    AutoCalibration(LED,Schwellwerte,LEDboden,S0,S1,S2,S3,UAM1,UAM2,UAM3);
  }
  if(irAutoCalibration){                                        //eigentlich unnötig, da nur minWert[] resettet wird
    blink(LEDir,1);
    blink(LEDboden,1);
    blink(LEDballcaught,1);
    blink(LEDgyro,1);
  }else if(PhaseLSKalibration){                                 //Andere Blinkzeichen je nach Phase der LS-Kalibration
    switch (PhaseLSKalibration){
      case 1:
        blink(LEDir,0);
        blink(LEDboden,0);
        blink(LEDballcaught,1);
        blink(LEDgyro,0);
        break;
      case 2:
        blink(LEDir,1);
        blink(LEDboden,0);
        blink(LEDballcaught,1);
        blink(LEDgyro,0);
        break;
    }
  }else{                                                          //Es wird nirgends kalibriert -> einfach nur Anzeigen, ob gewisse Werte erreicht sind
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
    if (hatBall(Lichtschranke,minWertLS,PhaseLSKalibration) && IRbest <10&&( Icball == 0 || Icball == 15 || Icball == 1 )) {
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