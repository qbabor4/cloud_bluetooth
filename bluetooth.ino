/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth 
 * Data is read by 6 chars 
 * 
*/

//TODO
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec
// zamiana z hex na 3 bajty

#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<6> ledStrip;

// Create a buffer for holding the colors (3 bytes per color). example: [255,255,255]
#define LED_COUNT 21 // numbers of leds + 1

rgb_color colors[LED_COUNT]; // holds structures of color
rgb_color color;  // structure of 3 variables: red, green, blue

char receivedData[6]; // received data from bluetooth
byte gotChars = 0; // how many chars program have read; App sends 6-chars-long data

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
}

rgb_color hexToRgb(char *hexColor){
  byte r, g, b;
  char rHex[2], gHex[2], bHex[2];
  //strncpy(rHex, hexColor, 2); // copy 2 characters from hexColor
  strncpy(gHex, hexColor+2, 0);
  strncpy(bHex, hexColor+4, 0);
  //char gHex[2] = {hexColor[2], hexColor[3]};
  //char bHex[2] = {hexColor[4], hexColor[5]};
  //r = hexrToDec(rHex);
  //g = hexrToDec(gHex);
  //b = hexrToDec(bHex);
  
  //Serial.println(rHex);
  Serial.println(gHex);
  Serial.println(bHex);
  //r = strtol(hexColor[0,1], 0, 16);
 
  return (rgb_color){r, g, b};
}


byte hexToDec(char *s) // opt
{
  int x = 0;
  for(;;) {
   char c = *s;
   if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
   }
   else if (c >= 'a' && c <= 'f') {
      x *= 16;
      x += (c - 'a') + 10;
   }
   else break;
   s++;
 }
 return (byte)x;
}

void loop()
{
  
    if ( Serial.available() > 0 ) {     // Get data only when you receive data     
        char c = Serial.read();        //Read the incoming data & store into c
        
        if ( gotChars ==  5 ){ 
          receivedData[gotChars] = c;
          Serial.println( receivedData );          
          //Serial.print("\n");
          gotChars = 0;

          // zamianic tego strigna na 3 kolory w strukturze i w petli wpisaca wszystko 
          // do tablicy kolorów
          
          colors[0] = hexToRgb(receivedData);
          //digitalWrite(13, HIGH);
            
            // tu wszystko zmianiać
            // podłączyć pasek rgb
            // zobaczyc czy działa
            // zamiana hex na rgb
            // progres      
          
        } 
        else {
            receivedData[gotChars] = c;
            gotChars += 1;
        }

         
    }
}
