#include<Lichtschranke.h>
#include<Arduino.h>
#include<SD.h>
#include <ArduinoJson.h>

boolean hatBall(uint8_t Lichtschranke,int& minWertLS, char& PhaseLSKalibration) {
  static int outMin=1023, inMax=0;
  switch(PhaseLSKalibration){
    case 0:
      static int MittelwertLS[5];                     // buffern der letzten 5 Werte
      MittelwertLS[4] = MittelwertLS[3];
      MittelwertLS[3] = MittelwertLS[2];
      MittelwertLS[2] = MittelwertLS[1];
      MittelwertLS[1] = MittelwertLS[0];
      MittelwertLS[0] = analogRead(Lichtschranke);
      if (((MittelwertLS[0] +  MittelwertLS[1] + MittelwertLS[2] + MittelwertLS[3]+MittelwertLS[4]) / 5)  < minWertLS) {      // Das arithmetische Mittel mit einer Schwelle überprüfen
        return true;
      }
      else {
        return false;
      }
      break;
    case 1:                     //Ball draußen halten!!!
      if(analogRead(Lichtschranke)<outMin){
        outMin=analogRead(Lichtschranke);
      }
      break;
    case 2:                     //Ball in der Kuhle drehen
      if(analogRead(Lichtschranke)>inMax){
        inMax=analogRead(Lichtschranke);
      }
      break;
    case 3:               //Speicherphase, die sich automatisch schließt
      minWertLS=(inMax+outMin)/2;
      File file = SD.open("minWerte.json", FILE_WRITE | O_TRUNC  | O_CREAT); //Datei öffnen, schreiben|leeren|neu erstellen, falls nicht existent
        StaticJsonDocument<500> doc;
        doc["Lichtschranke"]=minWertLS;
        String buf{""};                                                     //Buffer, der in die geöffnete *.json-Datei geschrieben wird
        serializeJsonPretty(doc,buf);
        for(auto elem:buf){
          file.write(elem);
        }
      file.close();
      PhaseLSKalibration=0;
      break;
  }
}