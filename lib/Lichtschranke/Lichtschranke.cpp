#include<Lichtschranke.h>
#include<Arduino.h>

boolean hatBall(uint8_t Lichtschranke) {
  // buffern der letzten 5 Werte
  static int MittelwertLS[5];
  MittelwertLS[4] = MittelwertLS[3];
  MittelwertLS[3] = MittelwertLS[2];
  MittelwertLS[2] = MittelwertLS[1];
  MittelwertLS[1] = MittelwertLS[0];
  MittelwertLS[0] = analogRead(Lichtschranke);
  // Das arithmetische Mittel mit einer Schwelle überprüfen
  if (((MittelwertLS[0] +  MittelwertLS[1] + MittelwertLS[2] + MittelwertLS[3]+MittelwertLS[4]) / 5)  < 400) {
    return true;
  }
  else {
    return false;
  }
  // return false;
}