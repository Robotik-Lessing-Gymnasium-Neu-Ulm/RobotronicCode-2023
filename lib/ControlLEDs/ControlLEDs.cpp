#include<ControlLEDs.h>
#include<Arduino.h>
#include<Lichtschranke.h>
#include<Defines.h>

#ifndef ButtonII
#define ButtonII 4
#endif

#ifndef ButtonI
#define ButtonI 5
#endif

//Control LEDs
#ifndef LEDboden
#define LEDboden 13
#endif
#ifndef LEDir
#define LEDir    17
#endif
#ifndef LEDgyro
#define LEDgyro  20
#endif
#ifndef LEDballcaught
#define LEDballcaught    37
#endif

void ControlLEDs(bool& buttonGpressed, double &richtung,double& IRbest, int& Icball, double& rotation,bool& minEinerDa, bool& irAutoCalibration) {
  // Einzelne Variablen überprüfen und dann die Pins schreiben
  if (digitalRead(ButtonI) == LOW) {
    buttonGpressed = true;
  }
  if (digitalRead(ButtonIV) == LOW) {
    irAutoCalibration=!irAutoCalibration;
    Serial.println("IR-KALIBRATION");
  }
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
  if (hatBall() && IRbest <10&&( Icball == 0 || Icball == 15 || Icball == 1 )) {
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