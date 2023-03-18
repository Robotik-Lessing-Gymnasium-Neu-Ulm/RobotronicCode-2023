#include <myPixy.h>
#include <Pixy2I2C.h>

double Pixy(Pixy2I2C& pixy,bool& piread) {
  double pixywinkel = -1;
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks){
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

    pixywinkel = atan2(VekTorY,- VekTorX) * 57.2957795;
    pixywinkel = abs(pixywinkel);
    return pixywinkel;
    //Serial.println(farbe);

  }
  else {
    piread = false;
  }
  return pixywinkel;
}