#include <myPixy.h>
#include <Pixy2I2C.h>

// [[deprecated]] double Pixy(Pixy2I2C& pixy,bool& piread){
//   constexpr size_t s=500;
//   static double pixywinkel = -1;
//   static unsigned long time=0;
//   static double buf[s];     //das Glätten hier ist falschrum… (in der Forschleife fürs Verschieben)
//   pixy.ccc.getBlocks();       //pixy auslesen

//   if (pixy.ccc.numBlocks){
//     if(millis()>=time){
//       piread = true;          //Kamera sieht was

//       //int16_t farbe = pixy.ccc.blocks[0].m_signature;
//       int16_t TorX = (pixy.ccc.blocks[0].m_x) - 158 ;
//       int16_t TorY = 208 - (pixy.ccc.blocks[0].m_y);
//       /*uint16_t TorHoehe = pixy.ccc.blocks[0].m_height;
//         uint16_t TorBreite = pixy.ccc.blocks[0].m_width;*/

//       const int UrX = 0;
//       const int UrY = 0;

//       int VekTorX = UrX - TorX;
//       int VekTorY = UrY - TorY;
//       pixywinkel = atan2(VekTorY,- VekTorX) * 180/PI; //berechnen des Winkels
//       pixywinkel = abs(pixywinkel);         //Betrag
//       for(int i=1;i<s;i++){               //diese Forschleife muss andersrum gemacht werden
//         buf[i]=buf[i-1];
//       }
//       buf[0]=pixywinkel;
//       pixywinkel=0;
//       for(int i=0;i<s;i++){
//         pixywinkel+=buf[i]/s;
//       }
//       // Serial.println(farbe);
//       time=millis()+50;
//     }
//   }
//   else {
//     time=millis();
//     for(int i=0;i<s;i++){               //resetten der Buffer-Werte
//       buf[i]=0;
//     }
//     piread = false;
//   }
//   return 90-pixywinkel;
// }

double Pixy(Pixy2I2C& pixy,bool& piread,int&  TorHoehe) {
  static double pixywinkel = -1;
  pixy.ccc.getBlocks();       //pixy auslesen

  if (pixy.ccc.numBlocks){
    piread = true;          //Kamera sieht was

    //int16_t farbe = pixy.ccc.blocks[0].m_signature;
    int16_t TorX = (pixy.ccc.blocks[0].m_x) - 158 ;
    int16_t TorY = 208 - (pixy.ccc.blocks[0].m_y);
    TorHoehe = pixy.ccc.blocks[0].m_height;
    /*uint16_t TorBreite = pixy.ccc.blocks[0].m_width;*/

    const int UrX = 0;
    const int UrY = 0;

    int VekTorX = UrX - TorX;
    int VekTorY = UrY - TorY;
    pixywinkel = atan2(VekTorY,- VekTorX) * 180/PI; //berechnen des Winkels
    pixywinkel = abs(pixywinkel);         //Betrag
    //Serial.println(TorHoehe);
    //Serial.println(90 - pixywinkel);
  }
  else {
    piread = false;
  }
  return 90-pixywinkel;
}
double Pixy2(Pixy2I2C& pixy2,bool& piread2,int&  TorHoehe2) {
  static double pixywinkel = -1;
  pixy2.ccc.getBlocks();       //pixy auslesen

  if (pixy2.ccc.numBlocks){
    piread2 = true;          //Kamera sieht was

    //int16_t farbe = pixy.ccc.blocks[0].m_signature;
    int16_t TorX = (pixy2.ccc.blocks[0].m_x) - 158 ;
    int16_t TorY = 208 - (pixy2.ccc.blocks[0].m_y);
    TorHoehe2 = pixy2.ccc.blocks[0].m_height;
    /*uint16_t TorBreite = pixy.ccc.blocks[0].m_width;*/

    const int UrX = 0;
    const int UrY = 0;

    int VekTorX = UrX - TorX;
    int VekTorY = UrY - TorY;
    pixywinkel = atan2(VekTorY,- VekTorX) * 180/PI; //berechnen des Winkels
    pixywinkel = abs(pixywinkel);         //Betrag
    //Serial.println(TorHoehe2);
    //Serial.println(90 - pixywinkel);
  }
  else {
    piread2 = false;
  }
  return 90-pixywinkel;
}
/*void position(double WinkelToreGes,double AbstandX,double AbstandY,Pixy2I2C& pixy2,bool& piread2,Pixy2I2C& pixy,bool& piread,int&  TorHoehe,int&  TorHoehe2){
  Pixy(pixy,piread,TorHoehe);
  Pixy2(pixy2,piread2,TorHoehe2);
  if(piread&&piread2){
  WinkelToreGes = abs(Pixy(pixy,piread,TorHoehe)) + abs(Pixy2(pixy2,piread2,TorHoehe2));          //Beträge der Winkel zu den beiden Toren werden addiert
  AbstandX = WinkelToreGes * 1;                                                                   //Ausrechnen des Abstands zum Mittelpunkt in X-Richtung mit durch ausbrobieren bestimmten Faktor
  AbstandY = double(double(TorHoehe)/double(TorHoehe2))*100;                                      //Ausrechnen des Abstands zum Mittelpunkt in Y-Richtung mit durch ausbrobieren bestimmten Faktor
  }else{
    //Serial.println("Lachs");
  }
  if(Pixy2(pixy2,piread2,TorHoehe2) >0){                                                          //Untrscheidung Rechts-links
    AbstandX = -AbstandX;
  }
  //Serial.println(AbstandX);                                                                     //Ausgabe der ermittelten Werte
  //Serial.println(AbstandY);
}*/


void position(double WinkelToreGes,double AbstandX,double AbstandY,Pixy2I2C& pixy2,bool& piread2,Pixy2I2C& pixy,bool& piread,int&  TorHoehe,int&  TorHoehe2){
  int a = 220*sin(abs(Pixy(pixy,piread,TorHoehe))*PI/180)/sin((180-abs(Pixy2(pixy2,piread2,TorHoehe2))-abs(Pixy(pixy,piread,TorHoehe)))*PI/180);
  AbstandX = sin(abs(Pixy2(pixy2,piread2,TorHoehe2))*PI/180)*a;
  AbstandY = sqrt((a*a)-(AbstandX*AbstandX));
  if(Pixy2(pixy2,piread2,TorHoehe2)<0){
    AbstandX = -AbstandX;
  }
}