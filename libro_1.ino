/***************************************************
  DFPlayer - A Mini MP3 Player For Arduino
  <https://www.dfrobot.com/product-1121.html>

 ***************************************************
  This example shows the basic function of library for DFPlayer.

  Created 2016-12-07
  By [Angelo qiao](Angelo.qiao@dfrobot.com)

  GNU Lesser General Public License.
  See <http://www.gnu.org/licenses/> for details.
  All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
  1.Connection and Diagram can be found here
  <https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
  2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>


SoftwareSerial mySoftwareSerial(12, 13); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
int flag;
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

}
void  avvio(int Volume, int brano)
{
 Serial.print("brano-->");Serial.println(brano+1);

  if (!flag)
  {

    //Serial.print("Avvio   ");
    myDFPlayer.volume(Volume);  //Set volume value. From 0 to 30

    myDFPlayer.play(brano+1);  //Play the first mp3
    // myDFPlayer.next();  //Play next mp3 every 20 second.
  }
}
void loop()
{
  sensorValue = analogRead(sensorPin); //carica il valore dell'analogica
  delay(100);
  int Volume = sensorValue / 22; //calcola il valore del volume
  for (int i = 0 ; i < 10; i++) {
    if (digitalRead(pin[i]) == HIGH)
    {
      key[i] = !key[i];
    }
    delay(20);
  }

  for (int i = 0 ; i < 10; i++) {
    if (digitalRead(pin[i]) == HIGH) {
      Serial.print("PIN");
      Serial.print(pin[i]);
      Serial.println(" ON ");
      brano = i;
      avvio(Volume, brano);
    }
    else
    {

      //Serial.print("PIN");
      //Serial.print(pin[i]);
      //Serial.println(" OFF   ");
    }

  }
}


void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number: "));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      Serial.print(F("tipo: "));
      Serial.print(type);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError: "));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
