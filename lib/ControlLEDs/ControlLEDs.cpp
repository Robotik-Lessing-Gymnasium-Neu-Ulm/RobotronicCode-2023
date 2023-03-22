#include<ControlLEDs.h>
#include<Arduino.h>
#include<Lichtschranke.h>

#ifndef gyroButton
#define gyroButton 31
#endif

#ifndef calibrationButton
#define calibrationButton 32
#endif

//Control LEDs
#ifndef LEDboden
#define LEDboden 27
#endif
#ifndef LEDir
#define LEDir    28
#endif
#ifndef LEDgyro
#define LEDgyro  29
#endif
#ifndef LEDballcaught
#define LEDballcaught    30
#endif

void ControlLEDs(bool& buttonGpressed, double &richtung,int& IRbest, int& Icball, double& rotation,bool& minEinerDa) {
  if (digitalRead(gyroButton) == LOW) {
    buttonGpressed = true;
  }
  if (digitalRead(calibrationButton) == LOW) {
    /*digitalWrite(LEDir, HIGH);
    digitalWrite(LEDboden, HIGH);
    digitalWrite(LEDballcaught, HIGH);
    digitalWrite(LEDgyro, HIGH);
    counterBoost = 0;
    while (counterBoost < 400) {
      compass();
      motor(90, 200);
      counterBoost++;
    }*/
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