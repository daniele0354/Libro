/***************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif
// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

SoftwareSerial mySoftwareSerial(12, 13); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

long t1 = 0;
int sensorPin = A1;
int sensorValue = 0;
int volValue = 0  ;
int distValue = 0;
int brano;
int  key[10] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int pin[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  for (int i = 0 ; i < 10; i++) {
    pinMode(pin[i], INPUT);
  }
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}
void  avvio(int Volume, int brano)
{
  Serial.print("brano-->"); Serial.println(brano+1);

  
    //Serial.print("Avvio   ");
    myDFPlayer.volume(Volume);  //Set volume value. From 0 to 30

    myDFPlayer.play(brano + 1); //Play the first mp3
    // myDFPlayer.next();  //Play next mp3 every 20 second.
  
  lasttouched=currtouched; 
}
void loop()
{
  sensorValue = analogRead(sensorPin); //carica il valore dell'analogica
  delay(100);
  int Volume = sensorValue / 22; //calcola il valore del volume

  currtouched = cap.touched();

  if ((currtouched != lasttouched) && (currtouched != 0)) {
     myDFPlayer.stop(); 
    for (uint8_t i = 0; i < 12; i++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
        //Serial.print(i); Serial.println(" touched");
        int brano  = i;
        Volume = 30;
        avvio(Volume, brano);
      }
    }
    lasttouched = currtouched;
  }
  delay(100);
}
