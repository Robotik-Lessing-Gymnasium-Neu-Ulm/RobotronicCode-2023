#include<Torwart.h>
#include<Motoren.h>
#include<Kompass.h>
#include<IR2.h>
#include<Boden.h>

#define ballUndLinie false //Solle er, wenn er die linie sieht immernoch auf den Ball reagiern?
#define Abweichung 10 //Wie schräg soll er fahren, wenn er auf den Ball und die Linie reagieren soll

void torwartProgramm(int* LED,int* Schwellwerte, double rotation,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot, bool pireads, int pixyG2, int& PixyG, int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave, bool hatBall, bool& torwart){
    //Serial.println(LED[1]);
    static int k=0;
    Serial3.write(0);
    if (Serial3.available()) {
        k = (int)Serial3.read();
    }
    if (k > 128) {
        k -= 256;
    }
    //Serial.println(k);
    pixyG2 = k;
    compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,pireads,PixyG,pixyG2,hatBall,true);
    IRsensTW(IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,rotation,addRot,WinkelBall,IRsave);
    //Serial.println(wiIn);
    if (LED[12] > Schwellwerte[12] || LED[13] > Schwellwerte[13]) {
        motor(0, 150,rotation);
        //Serial.println("salkd1");
    } else if (LED[20] > Schwellwerte[20] || LED[21] > Schwellwerte[21]) {
        motor(180, 150,rotation);
        //Serial.println("salkd2");
    } else {
        richtung=0;
        if (onLine(LED,Schwellwerte)) {
            if (wiIn == 0) {
                Serial.println("vorne");
                motor(90, 50,rotation);
            }  else if (richtung == -1) {
                motor(0, 0,rotation);
                Serial.println("nicht");
            } else if (wiIn < 0) {
                Serial.println("rechts");
                motor(0, 70,rotation);
            } else {
                Serial.println("links");
                motor(180, 70,rotation);
            }
            Serial.println("onli");
        }
        else {
            if(k==127){
                //compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,pireads,PixyG,pixyG2,hatBall,torwart);
                motor(90,10,0);
            }else{
                if (LED[44] > Schwellwerte[44] || LED[45] > Schwellwerte[45] || LED[34] > Schwellwerte[34] || LED[35] > Schwellwerte[35]) {
                    if(ballUndLinie){
                        if (wiIn == 0) {
                            //Serial.println("yes vorne");
                            motor(90, 100,rotation);
                        }  else if (richtung == -1) {
                            //Serial.println("yes nicht");
                            motor(90, 100,rotation);
                        } else if (wiIn < 0) {
                            // Serial.println("yes rechts");
                            motor(90-Abweichung, 70,rotation);
                        } else {
                            // Serial.println("yes links");
                            motor(90+Abweichung, 70,rotation);
                        }
                    }else{
                        motor(90, 250,rotation);
                    }
                }
                else {
                    if(ballUndLinie){
                        if (wiIn == 0) {
                            // Serial.println("no vorne");
                            motor(270, 60,rotation);
                        }  else if (richtung == -1) {
                            // Serial.println("no nicht");
                            motor(270, 60,rotation);
                        } else if (wiIn < 0) {
                            // Serial.println("no rechts");
                            motor(270+Abweichung, 60,rotation);
                        } else {
                            // Serial.println("no links");
                            motor(270-Abweichung, 60,rotation);
                        }
                    }else{
                        motor(270, 60,rotation);
                    }
                }
            }
        }
    }
    // Serial.println(LED[17]);
    Serial3.flush();
}