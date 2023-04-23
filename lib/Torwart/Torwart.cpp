#include<Torwart.h>
#include<Motoren.h>
#include<Kompass.h>
#include<IR2.h>
#include<Boden.h>

void torwartProgramm(int* LED,int* Schwellwerte, double rotation,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot, bool pireads, int pixyG2, int& PixyG, int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave){
    if (LED[12] > Schwellwerte[12] || LED[13] > Schwellwerte[13]) {
        motor(0, 250,rotation);
    } else if (LED[20] > Schwellwerte[20] || LED[21] > Schwellwerte[21]) {
        motor(180, 250,rotation);
    } else {
        static int k=0;
        Serial3.write(0);
        if (Serial3.available()) {
        k = (int)Serial3.read();
        }
        if (k > 128) {
        k -= 256;
        }
        compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,pireads,PixyG);
        IRsensTW(IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,rotation,addRot,WinkelBall,IRsave);
        pixyG2 = k;
        if (onLine(LED,Schwellwerte)) {
        if (wiIn == 0) {
            motor(0, 0,rotation);
        }  else if (richtung == -1) {
            motor(0, 0,rotation);
        } else if (wiIn < 180) {
            Serial.println("rechts");
            motor(0, 110,rotation);
        } else {
            Serial.println("links");
            motor(180, 110,rotation);
        }
        }
        else {
        if (LED[44] > Schwellwerte[44] || LED[45] > Schwellwerte[45] || LED[34] > Schwellwerte[34] || LED[35] > Schwellwerte[35]) {
            motor(90, 15,rotation);
        }
        else {
            motor(270, 50,rotation);
        }
        }
        Serial.println(LED[17]);
        Serial3.flush();
    }
}