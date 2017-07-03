/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth 
 * Data is read by 6 chars 
 * 
*/

//TODO
// dostac caly hex string i dopiero wtedy coś robic (end marker)
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec
// sprawdzic czy lepiej inkrementwac zmianna itam pod index tablicy dawac znak czy robic na stringu i +=
// inkcementowac i jak jest 6, to koniec ( bez koncowego znaku

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

byte gotChars = 0; // how many chars program have read; App sends 6 chars long data

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
}
void loop()
{
  
    if ( Serial.available() > 0 ) {     // Get data only when you receive data
        gotChars += 1;
        char c = Serial.read();        //Read the incoming data & store into c
        
        if ( gotChars ==  7 ){
          Serial.print( receivedData );          
          Serial.print("\n");
          gotChars = 0;
          //digitalWrite(13, HIGH);
            
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
