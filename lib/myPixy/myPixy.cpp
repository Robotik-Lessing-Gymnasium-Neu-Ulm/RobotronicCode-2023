#include <myPixy.h>
#include <Pixy2I2C.h>

double Pixy(Pixy2I2C& pixy,bool& piread) {
  static double pixywinkel = -1;
  static unsigned long time=0;
  static double buf[500];
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks){
    if(millis()>=time){
      piread = true;

      //int16_t farbe = pixy.ccc.blocks[0].m_signature;
      int16_t TorX = (pixy.ccc.blocks[0].m_x) - 158 ;
      int16_t TorY = 208 - (pixy.ccc.blocks[0].m_y);
      /*uint16_t TorHoehe = pixy.ccc.blocks[0].m_height;
        uint16_t TorBreite = pixy.ccc.blocks[0].m_width;*/

      const int UrX = 0;
      const int UrY = 0;

      int VekTorX = UrX - TorX;
      int VekTorY = UrY - TorY;
      pixywinkel = atan2(VekTorY,- VekTorX) * 180/PI;
      pixywinkel = abs(pixywinkel);
      for(int i=1;i<500;i++){
        buf[i]=buf[i-1];
      }
      buf[0]=pixywinkel;
      pixywinkel=0;
      for(int i=0;i<500;i++){
        pixywinkel+=buf[i]/500;
      }
      // Serial.println(farbe);
      time=millis()+50;
    }
  }
  else {
    time=millis();
    for(int i=0;i<500;i++){
      buf[i]=0;
    }
    piread = false;
  }
  return 90-pixywinkel;
}