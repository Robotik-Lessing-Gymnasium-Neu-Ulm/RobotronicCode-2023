#include<Kompass.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

void compass(Adafruit_BNO055& gyro, bool& buttonGpressed, double& minus, double& rotation, int& alterWinkel) {
  sensors_event_t orientationData;                                                                              //getting reading from gyro
  sensors_event_t angVelocityData;
  gyro.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);                                           //receiving angular velocity
  gyro.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);                                               //receiving angle in degrees
  double winkel = orientationData.orientation.x;                            
  double rotationSpeed = angVelocityData.orientation.z;
  if (buttonGpressed) {                                                                                         //if button was pressed saves the offset
    minus = winkel;
    Serial.print("MINUS:");
    Serial.println(minus);
    buttonGpressed = false;
  }
  winkel = winkel - minus;
  if (winkel > 180) {                                                                                           //to be usable angle needs to be between 180 and -180
    winkel = winkel - 360;
  }
  if (winkel < -180) {
    winkel = winkel + 360;
  }
  double p = 11;                                                          
  double d = 50;                                                          
  rotation = (p * winkel) - d * rotationSpeed;                                                                  //calculate rotation needed
  alterWinkel = winkel;
  rotation = -rotation / 4.5;
}