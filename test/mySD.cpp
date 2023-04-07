
#include <SPI.h>
#include <SD.h>

//create an integer named "pot" This integer will be used for the value of the potentiometer.
int pot; 

void loop(){
  String dataString = "";
        if (digitalRead(7) == HIGH) {
          int pot = analogRead(A0);
          dataString = String(pot);


      // open the file named datalog.txt on the sd card
          File dataFile = SD.open("datalog.txt", FILE_WRITE);

          // if the file is available, write the contents of datastring to it
          if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
          }  
          // if the file isn't open, pop up an error:
          else {
          Serial.println("error opening datalog.txt");
        }   
  }

//================================================================================================================================

//if button wired to digital pin 8 is pressed, print "button 8 pressed - load" to serial


        if (digitalRead(8) == HIGH) {
        Serial.println("button 8 pressed - load");

      //open up datalog2.txt and then print all of its contents  

        File dataFile = SD.open("datalog.txt");
        if(dataFile) {
          Serial.println("datalog:");
          while (dataFile.available()) {
              Serial.write(dataFile.read());
           }
      // close the file:
         dataFile.close();


//Delete the file so it can be created again at the begining of the loop
SD.remove("datalog.txt");
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datalog.txt");
  }
  
}



  delay(150);
  }