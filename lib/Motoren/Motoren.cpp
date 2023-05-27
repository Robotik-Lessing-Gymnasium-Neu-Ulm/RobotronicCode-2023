#include <Motoren.h>
#include <Arduino.h>
#include<vector>
#include<string>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include<PID_v1.h>
#include<SD.h>

#ifndef M1_FW
#define M1_FW 26 //richtig
#endif
#ifndef M1_RW
#define M1_RW 28
#endif
#ifndef M1_PWM
#define M1_PWM 29
#endif

#ifndef M2_FW
#define M2_FW 12 //richtig
#endif
#ifndef M2_RW
#define M2_RW 25
#endif
#ifndef M2_PWM
#define M2_PWM 24
#endif

#ifndef M3_FW
#define M3_FW  9  //richtig!
#endif
#ifndef M3_RW
#define M3_RW 11
#endif
#ifndef M3_PWM
#define M3_PWM 10
#endif

#ifndef M4_FW
#define M4_FW  6  //richtig!
#endif
#ifndef M4_RW
#define M4_RW 8
#endif
#ifndef M4_PWM
#define M4_PWM 7
#endif

constexpr bool speichern=false; //else: offsetten
constexpr size_t STELLEN=2;               //Anzahl der Nachkommastellen, die gespeichert werdem sollen

void motor (double dir, double velocity, double rotation) {
  // dir Umrechnen ins Bogenmaß
  double phi = (dir / 180) * PI;
  // Aufteilen des damit aufgespannten Vektors auf die 4 Motoren
  double m4 = sin(phi - 5 * PI / 4) * velocity + rotation; //VR
  double m1 = sin(phi - 7 * PI / 4) * velocity + rotation; //VL
  double m2 = sin(phi - PI / 4) * velocity + rotation;     //HL
  double m3 = sin(phi - 3 * PI / 4) * velocity + rotation; //HR
  //Aufteilen in Drehrichtungen
  //VL
  if (m1 < 0) {
    digitalWrite(M1_FW, HIGH);
    digitalWrite(M1_RW, LOW);
    analogWrite(M1_PWM, abs(m1));
  }
  if (m1 > 0) {
    digitalWrite(M1_FW, LOW);
    digitalWrite(M1_RW, HIGH);
    analogWrite(M1_PWM, abs(m1));
  }
  if (m1 == 0) {
    digitalWrite(M1_FW, HIGH);
    digitalWrite(M1_RW, HIGH);
    analogWrite(M1_PWM, 255);
  }
  // VR
  if (m2 < 0) {
    digitalWrite(M2_FW, HIGH);
    digitalWrite(M2_RW, LOW);
    analogWrite(M2_PWM, abs(m2));
  }
  if (m2 > 0) {
    digitalWrite(M2_FW, LOW);
    digitalWrite(M2_RW, HIGH);
    analogWrite(M2_PWM, abs(m2));
  }
  if (m2 == 0) {
    digitalWrite(M2_FW, HIGH);
    digitalWrite(M2_RW, HIGH);
    analogWrite(M2_PWM, 255);
  }
  // HR
  if (m3 < 0) {
    digitalWrite(M3_FW, HIGH);
    digitalWrite(M3_RW, LOW);
    analogWrite(M3_PWM, abs(m3));
  }
  if (m3 > 0) {
    digitalWrite(M3_FW, LOW);
    digitalWrite(M3_RW, HIGH);
    analogWrite(M3_PWM, abs(m3));
  }
  if (m3 == 0) {
    digitalWrite(M3_FW, HIGH);
    digitalWrite(M3_RW, HIGH);
    analogWrite(M3_PWM, 255);
  }
  // HL
  if (m4 < 0) {
    digitalWrite(M4_FW, HIGH);
    digitalWrite(M4_RW, LOW);
    analogWrite(M4_PWM, abs(m4));
  }
  if (m4 > 0) {
    digitalWrite(M4_FW, LOW);
    digitalWrite(M4_RW, HIGH);
    analogWrite(M4_PWM, abs(m4));
  }
  if (m4 == 0) {
    digitalWrite(M4_FW, HIGH);
    digitalWrite(M4_RW, HIGH);
    analogWrite(M4_PWM, 255);
  }
}

double getRotation(Adafruit_BNO055& gyro){
  sensors_event_t orientationData;                                          //momentane Aufnahme der der Sensorwerte (eigener Sensor: BNO055)
  gyro.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;
}
double getRotationSpeed(Adafruit_BNO055& gyro){
  sensors_event_t angVelocityData;
  gyro.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  return angVelocityData.orientation.z;
}

void fahren(double direction, double velocity, double rotation, Adafruit_BNO055& gyro,bool& buttonGpressed){
  static bool setup=true;
  static double InpidWi;
  static double SetpidWi;
  static double OutpidWi;
  static PID pidWi(&InpidWi,&OutpidWi,&SetpidWi,3,0,0.15,DIRECT);  //35,12,14
  if(setup){
    pidWi.SetOutputLimits(-40,40);                              //Notlösung, denn er gleicht sich auch der Unstetigkeitsstelle an
    pidWi.SetMode(AUTOMATIC);
  }
  static std::vector<double> v_buf (0);                         //um die Geschwindigkeit zu speichern und später anzuzeigen
  static std::vector<double> ri_buf (0);                        //um die Bewegungsrichtung zu speichern und später anzuzeigen
  static std::vector<double> wi_buf (0);                        //um den Drehwinkel zu speichern und später anzuzeigen
  static std::vector<unsigned long> measurementTime_buf (0);    //Um die x-Skalierung beim Speicher und Anzeigen zu beachten
  static double minus{0};                                          //Offset des BNO055
  static uint32_t lastMeasurement{0};
  static double winkel{0},rotationSpeed{0};
  winkel=getRotation(gyro)-minus;
  while(winkel>rotation+180){                                  //unstetige Stelle genau auf der Anderen Seite des Rotation-Setpoints
    winkel-=360;
  }
  while(winkel<rotation-180){
    winkel+=360;
  }
  rotationSpeed=getRotationSpeed(gyro);
  static int16_t x{0},y{0};
  static bool surface{false},motion{false};
  static char x_{0},y_{0},o_{0};
  static char* currentWrite=&x_;
  char readBuf{0};
  for(int i=0;i<6;i++){
    while(!Serial3.available());
    readBuf=Serial3.read();
    switch(readBuf){                                            //Auslesen des Teensy 4.0
      case 255:
        currentWrite=&x_;break;
      case 254:
        currentWrite=&y_;break;
      case 253:
        currentWrite=&o_;break;
      default:
        *currentWrite=readBuf;break;
    }
  }
  char o{o_};
  if(o>=8){
    o-=8;
    y=y_;
  }else{
    y=-y_;
  }
  if(o>=4){
    o-=4;
    x=x_;
  }else{
    x=-x_;
  }
  if(o>=2){
    o-=2;
    motion=true;
  }else{
    motion=false;
  }
  surface=(o_>=1);
  x+=1*rotationSpeed;
  y+=3.4*rotationSpeed;
  double v=hypot((double)x,(double)y)/(millis()-lastMeasurement);
  double ri=atan2(x,y)*180/PI+8;
//speichern
  static char count{0};
  if(count>=10){
    v_buf.push_back(v);
    wi_buf.push_back(winkel);
    ri_buf.push_back(ri);
    measurementTime_buf.push_back(millis());
    count=0;
    // Serial.print(".");
    Serial.println(OutpidWi);
  }else{
    count++;
  }
  lastMeasurement=millis();

  InpidWi=ri-winkel+90;
  while(InpidWi<direction-180){
    InpidWi+=360;
  }while(InpidWi>direction+180){
    InpidWi-=360;
  }
  SetpidWi=direction;
  if(motion){
    pidWi.Compute();
  }
  if (buttonGpressed) {
    if(!speichern){   //offsetten
      minus = getRotation(gyro);                                                           //offsetten
      Serial.print("MINUS:");
      Serial.println(minus);
    }else{            //speichern
      Serial.println("Messwerte abspeichern.");
      File myF=SD.open("Rotation.txt",FILE_WRITE); /* myF.seek(EOF); */
        myF.truncate();
        myF.println("Winkel: ");
        Serial.println("Winkel: ");
        for(auto elem:wi_buf){
          myF.println(elem,STELLEN);
          Serial.println(elem,STELLEN);
        }
      myF.close();
      myF=SD.open("Direction.txt",FILE_WRITE); /* myF.seek(EOF); */
        myF.truncate();
        myF.println("Richtung: ");
        Serial.println("Richtung: ");
        for(auto elem:ri_buf){
          myF.println(elem,STELLEN);
          Serial.println(elem,STELLEN);
        }
      myF.close();
      myF=SD.open("Velocity.txt",FILE_WRITE); /* myF.seek(EOF); */
        myF.truncate();
        myF.println("Geschwindigkeit: ");
        Serial.println("Geschwindigket: ");
        for(auto elem:v_buf){
          myF.println(elem,STELLEN);
          Serial.println(elem,STELLEN);
        }
      myF.close();
      myF=SD.open("Time.txt",FILE_WRITE); /* myF.seek(EOF); */
        myF.truncate();
        myF.println("Zeit: ");
        Serial.println("Zeit: ");
        for(auto elem:measurementTime_buf){
          myF.println(elem,STELLEN);
          Serial.println(elem,STELLEN);
        }
      myF.close();
      Serial.println("abgeschlossen");
      delay(500);
    }
    buttonGpressed = false;                                                 //automatisch terminieren
  }
  int p{11},d{50};                                                                 //p(i)d Werte des Rotations-Reglers
  double ro = -((p * (winkel-rotation)) - d * rotationSpeed)/4.5;                   //skalierter pd Regler
  static bool lastSurface{true};
  if(surface||lastSurface){       //minimale Glättung
    motor(OutpidWi+direction,velocity,ro);
  }else{
    motor(0,0,0);
  }
  lastSurface=surface;
  setup=false;
}