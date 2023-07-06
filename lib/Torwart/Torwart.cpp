#include<Torwart.h>
#include<Motoren.h>
#include<Kompass.h>
#include<IR2.h>
#include<Boden.h>
#include<myPixy.h>
#include<Ballverfolgung.h>
#include<Defines.h>

#define ballUndLinie false  //Solle er, wenn er die linie sieht immernoch auf den Ball reagiern?
#define Abweichung 10       //Wie schräg soll er fahren, wenn er auf den Ball und die Linie reagieren soll

// void torwartProgramm(Pixy2I2C& pixy2,int* LED,int* Schwellwerte, double rotation,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot, bool piread2,bool piread, int pixyG2, int& PixyG, int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave, bool hatBall, bool& torwart, PID& entfPID,PID& wiPID,double& offsetVorne, double& entfVelo,double& wiVelo,bool &minEinerDa, bool* Photo,bool* gesehenSensor,double& bodenrichtung,long& alteZeit,double &entfSet,unsigned long& addRotTime,bool& surface,double& accel){
//     constexpr int Grenze{46};                   //Schwellwert um auf den Ball zuzufahren
//     Serial.print("Boden(inTorwart): ");
//     pixyG2 = Pixy2(pixy2,piread2);              //aktualisieren des Winkels zum Tor
//     double winkel=compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,piread2,PixyG,pixyG2,hatBall,true,accel); //drehung ausrechnen
//     IRsensTW(IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,rotation,addRot,WinkelBall,IRsave);
//     if (LED[12] > Schwellwerte[12] || LED[13] > Schwellwerte[13]) { //wenn Linie links erkannt nach rechts fahren
//         motor(0, 150,rotation);
//     } else if (LED[20] > Schwellwerte[20] || LED[21] > Schwellwerte[21]) {  //wenn Linie rechts nach links
//         motor(180, 150,rotation);
//     } else {                   
//         if (onLine(LED,Schwellwerte)) { //falls Roboter auf der Linie
//             if(IRbest<Grenze){          //falls nah verfolge den Ball; Noch TESTEN weil boden (vllt nur wenn der Ball noch in Kegel)
//                 if(piread2&&abs(wiIn)<40&&IRbest<30){
//                     // verfolgeBall(IRbest,entfPID,wiPID,offsetVorne,entfVelo,wiVelo,minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,pixyG2,hatBall,torwart,IR,Icball,richtung,entfSet,wiIn,minWert,irAutoCalibration,WinkelBall,addRotTime,IRsave,surface,accel);
//                     motor(wiIn*2.6+90,90,rotation);
//                 }
//             }
//             else if (wiIn == 0) {  //wenn vorne und auf der Linie nach vorne fahren
//                 Serial.println("vorne");
//                 motor(90, 50,rotation);
//             }  else if (richtung == -1) {  //kein Ball da, sttehen bleiben
//                 motor(0, 0,rotation);
//                 Serial.println("nicht");
//             } else if (wiIn > 0) {       //Ball rechts, also nach rechts fahren
//                 Serial.println("rechts");
//                 motor(5, 2*wiIn,rotation);
//             } else {                     //Ball links, nach links fahren
//                 Serial.println("links");
//                 motor(175, -2*wiIn,rotation);
//             }
//             Serial.println("onli");
//         }
//         else {
//             if(IRbest<Grenze&&abs(winkel)<35){          //falls zu sehr am Rand nich auf Ball zufahren
//                 //verfolgeBall(IRbest,entfPID,wiPID,offsetVorne,entfVelo,wiVelo,minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,pixyG2,hatBall,torwart,IR,Icball,richtung,entfSet,wiIn,minWert,irAutoCalibration,WinkelBall,addRotTime,IRsave,surface,accel);
//             }
//             else{
//                 if(piread2== false){ //kein Tor, suchen
//                     //compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,pireads,PixyG,pixyG2,hatBall,torwart);
//                     motor(0,0,11);      //drehen
//                 }else{
//                     if (LED[44] > Schwellwerte[44] || LED[45] > Schwellwerte[45] || LED[34] > Schwellwerte[34] || LED[35] > Schwellwerte[35]||LED[40]>Schwellwerte[40]||LED[41]>Schwellwerte[41]) { //zu weit auf der Linie
//                         if(ballUndLinie){                           //nach vorne fahren in Richtung Ball
//                             if (wiIn == 0) {
//                                 //Serial.println("yes vorne");
//                                 motor(90, 100,rotation);
//                             }  else if (richtung == -1) {
//                                 //Serial.println("yes nicht");
//                                 motor(90, 100,rotation);
//                             } else if (wiIn < 0) {
//                                 // Serial.println("yes rechts");
//                                 motor(90-Abweichung, 70,rotation);
//                             } else {
//                                 // Serial.println("yes links");
//                                 motor(90+Abweichung, 70,rotation);
//                             }
//                         }else{
//                             motor(90, 250,rotation);
//                         }
//                     }
//                     else {                               //keine Linie, nach hinten fahren und auf den Ball zu
//                         if(ballUndLinie){
//                             if (wiIn == 0) {
//                                 // Serial.println("no vorne");
//                                 motor(270, 60,rotation);
//                             }  else if (richtung == -1) {
//                                 // Serial.println("no nicht");
//                                 motor(270, 60,rotation);
//                             } else if (wiIn < 0) {
//                                 // Serial.println("no rechts");
//                                 motor(270+Abweichung, 60,rotation);
//                             } else {
//                                 // Serial.println("no links");
//                                 motor(270-Abweichung, 60,rotation);
//                             }
//                         }else{
//                             motor(270, 40,rotation);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

constexpr bool Notfall{true};
void torwartProgramm(Pixy2I2C& pixy2,bool piread2,Adafruit_BNO055 &gyro, bool &buttonGpressed, double &minus, int &alterWinkel, double &addRot,double rotation,int& PixyG,bool hatBall,double& accel,bool &minEinerDa, int *LED, int *Schwellwerte, bool *Photo, bool *gesehenSensor, double &bodenrichtung,
long &alteZeit,bool piread,int PixyG2, bool hBall, bool torwart, bool &surface,int *IR, double &IRbest, int &Icball, double &richtung, double &wiIn, int *minWert, bool &irAutoCalibration, double &WinkelBall, bool &IRsave,
PID &entfPID, PID &wiPID, double &offsetVorne, double &entfVelo, double &wiVelo,double &entfSet,unsigned long &addRotTime){
    static int naheAnfahrt=0.75;     //erster Wert: wenn die Maus aktiviert ist, zweiter wenn nicht; für den Winkel, wenn der Ball vorne fährt (gewichtung des Abstands)
    static int naheOffAnf=18;                      //für den Winkel, wenn der Ball vorne fährt
    static double GeschNahOff=55;        //die Geschw., wenn der Ball vorne liegt (Sensor 15 oder 1), aber nicht exakt (nicht 0)
    static double GeschNahFakt=0.5;                //Geschwindigkeit wenn der Ball bei Sensor 0 ist
    static bool driveBack=true;
    static unsigned long lastBackTimeRefresh=millis();
    double winkel=compass(gyro,buttonGpressed,minus,rotation,alterWinkel,addRot,piread2,PixyG,PixyG2,hatBall,true,accel); //drehung ausrechnen
    if(millis()>lastBackTimeRefresh+5'000){
        driveBack=true;
        lastBackTimeRefresh=millis();
    }
    PixyG2 = Pixy2(pixy2,piread2);              //aktualisieren des Winkels zum Tor
    bodenlesen(minEinerDa,LED,Schwellwerte,Photo);
    if(Photo[0]||Photo[1]||Photo[2]||Photo[3]||Photo[30]||Photo[31]){
        driveBack=false;
    }
    if(bodenrichtung==-1){
        if(piread2){
            Serial.println(wiIn);
            IRsensTW(IR,IRbest,Icball,richtung,wiIn,minWert,irAutoCalibration,rotation,addRot,WinkelBall,IRsave);
            wiIn+=winkel;           //Drehung des Roboters rausrechnen aus Richtung zu Ball
            if(Icball == 0){
                motor(90,GeschNahFakt*IRbest+GeschNahOff,rotation);
            }else if(Icball == 15){
                motor(90-min(IRbest*naheAnfahrt+naheOffAnf,89),GeschNahFakt*IRbest+GeschNahOff,rotation);
            }else if(Icball == 1){
                motor(90+min(IRbest*naheAnfahrt+naheOffAnf,89),GeschNahFakt*IRbest+GeschNahOff,rotation);
            }else if(abs(wiIn)<=120){     //ball liegt "vor" ihm (im erweiterten Kegel)
                if(driveBack){
                    motor(270,80,rotation);
                }else if(richtung!=-1){     //überprüfe, ob du Ball siehst
                    if (wiIn == 0) {        //wenn vorne und auf der Linie nach vorne fahren
                        Serial.println("vorne");
                        // motor(90, 50,rotation);
                        motor(0,0,rotation);
                    }  else if (richtung == -1) {  //kein Ball da, sttehen bleiben
                        motor(0, 0,rotation);
                        Serial.println("nicht");
                    } else if (wiIn > 0) {       //Ball rechts, also nach rechts fahren
                        Serial.println("rechts");
                        motor(350, max(1.4*wiIn,50),rotation);
                    } else {                     //Ball links, nach links fahren
                        Serial.println("links");
                        motor(190, max(1.4*wiIn,50),rotation);
                    }
                }else{ //kein Ball gesehen
                    motor(0,0,rotation);
                }
            }else{      //Ball liegt hinter ihm
                verfolgeBall(IRbest,entfPID,wiPID,offsetVorne,entfVelo,wiVelo,minEinerDa,LED,Schwellwerte,Photo,gesehenSensor,bodenrichtung,gyro,buttonGpressed,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,PixyG2,hBall,torwart,IR,Icball,richtung,entfSet,wiIn,minWert,irAutoCalibration,WinkelBall,addRotTime,IRsave,surface,accel);
                return;
            }
        }else{
            motor(0,0,11);
        }
    }else{                                                                                                                          //der Boden sieht etwas
        motor(bodenrichtung, 200,rotation);                                                                                           //sehr schnell von der Linie wegfahren
    }
    bodenverarbeiten(gesehenSensor,minEinerDa,Photo,bodenrichtung,LED,gyro,buttonGpressed,Schwellwerte,minus,alteZeit,alterWinkel,rotation,addRot,piread,PixyG,PixyG2,hatBall,torwart,hBall,surface,accel);
}