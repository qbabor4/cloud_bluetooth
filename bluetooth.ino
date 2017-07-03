/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth with end-marker '>' like a2b1b4>
 *
 * 
*/

//TODO
// dostac caly hex string i dopiero wtedy coś robic (end marker)
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec

#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<6> ledStrip;

const int LED_COUNT = 21;
rgb_color colors[LED_COUNT];
//rgb_color color;

//color variables
byte red, green, blue;

// Create a buffer for holding the colors (3 bytes per color). example: [255,255,255]
#define LED_COUNT 21 // numbers of leds + 1

String receivedData = "";
const char endMarker = '>'; 
//const byte numChars = 32;
//char receivedData[numChars]; // array of data

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
}
void loop()
{
      if ( Serial.available() > 0 ) {     // Get data only when you receive data:
          char c = Serial.read();        //Read the incoming data & store into c
                    
          if ( c ==  endMarker ){
            Serial.print( receivedData );          
            Serial.print("\n");
            Serial.print("\n");
            digitalWrite(13, HIGH);
            
            // tu wszystko zmianiać
            // podłączyć pasek rgb
            // zobaczyc czy działa
            // zamiana hex na rgb
            // progres
             
            receivedData = "";
          } 
          else {
            receivedData += c;
          }
   }
  
}
