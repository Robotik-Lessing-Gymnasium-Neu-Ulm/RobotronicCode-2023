#include<Lichtschranke.h>
#include<Arduino.h>
#include<Defines.h>

boolean hatBall(int minWertLS) {
  // buffern der letzten 5 Werte
  constexpr size_t s =10;
  static int MittelwertLS[s];
  for(int i=s-1;i>0;i--){
    MittelwertLS[i]=MittelwertLS[i-1];
  }
  MittelwertLS[0] = analogRead(LichtSchranke);
  double buf=0;
  for(int i=0;i<s;i++){
    buf+=(double)MittelwertLS[i]/s;
  }
  // Serial.println(buf);
  // Das arithmetische Mittel mit einer Schwelle überprüfen
  //LILA: 29; black: 45
  if (buf  < minWertLS) {
    return true;
  }
  else {
    return false;
  }
  return false;
}