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
      Serial.println("Kalibrierung(LS): Ball außerhalb der Kuhle!");
      if(analogRead(Lichtschranke)<outMin){
        outMin=analogRead(Lichtschranke);
      }
      break;
    case 2:                     //Ball in der Kuhle drehen
      Serial.println("Kalibrierung(LS): Ball in der Kuhle!");
      if(analogRead(Lichtschranke)>inMax){
        inMax=analogRead(Lichtschranke);
      }
      break;
    case 3:               //Speicherphase, die sich automatisch schließt
      minWertLS=(inMax+outMin)/2;
      Serial.print("Kalibrierung(LS): speichern: ");
      Serial.println(minWertLS);
      File myFile_=SD.open("minWerte.json",FILE_READ); //Datei öffnen, lesen
        char buf_[myFile_.size()];                                  //Zwischenspeicher des Inhalts der geöffneten *.json-Datei
        myFile_.read(buf_,myFile_.size());
        StaticJsonDocument<1000> doc_;
        deserializeJson(doc_, buf_);
      myFile_.close();
      File s = SD.open("minWerte.json", FILE_WRITE); //Datei öffnen, schreiben
        s.truncate();
        doc_["Lichtschranke"]=minWertLS;
        char b[500];                                                     //Buffer, der in die geöffnete *.json-Datei geschrieben wird
        for(int i{0};i<500;i++){
          b[i]=' ';
        }
        serializeJsonPretty(doc_,b);
        for(auto elem:b){
          s.write(elem);
        }
      s.close();
      PhaseLSKalibration=0;
      break;
  }
}