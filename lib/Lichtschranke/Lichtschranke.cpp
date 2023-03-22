#include<Lichtschranke.h>
#include<Arduino.h>

boolean hatBall() {
  static int MittelwertLS[5];                                                                                  //buffering of the last 5 readings
  MittelwertLS[4] = MittelwertLS[3];
  MittelwertLS[3] = MittelwertLS[2];
  MittelwertLS[2] = MittelwertLS[1];
  MittelwertLS[1] = MittelwertLS[0];
  MittelwertLS[0] = analogRead(A9);
  if (((MittelwertLS[0] +  MittelwertLS[1] + MittelwertLS[2] + MittelwertLS[3]+MittelwertLS[4]) / 5)  < 400) { //arithmetic mean smaller than 400
    return true;
  }
  else {
    return false;
  }

}