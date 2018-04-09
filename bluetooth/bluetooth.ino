/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth 
 * Data is read by 6 chars 
 * 
*/

//TODO
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec
// jak nie jest 6 to nic nie robić  


#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<6> ledStrip;

// Create a buffer for holding the colors (3 bytes per color). example: [255,255,255]
#define LED_COUNT 20 // numbers of leds 

rgb_color colors[LED_COUNT]; // holds structures of color
rgb_color color;  // structure of 3 variables: red, green, blue

//char receivedData[6]; // received data from bluetooth
//byte gotHexNumbers = 0; // how many chars program have read; App sends 6-chars-long data
//boolean gotSTARTchar = false; // saves data only when true ( between # and > )


void setup(){
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
    digitalWrite(10, HIGH);
}

rgb_color hexToRgb(char *hexColor){
    uint8_t r, g, b;
    char rgbHex[2]; 
    strncpy(rgbHex, hexColor, 2); // copy 2 characters from hexColor
    r = hexToDec(rgbHex);
    strncpy(rgbHex, hexColor+2, 2);
    g = hexToDec(rgbHex);
    strncpy(rgbHex, hexColor+4, 2);
    b = hexToDec(rgbHex);
      
    return (rgb_color){r, g, b};
}


byte hexToDec(char *s) {
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
 return (uint8_t)x;
}

void sendToDiodes(rgb_color color){
  for ( uint16_t i = 0; i < LED_COUNT; i++ ){
    colors[i] = color;
  }
  ledStrip.write(colors,LED_COUNT);
  delay(10);
}

char START = '<';
char END = '>';
char RAINBOW[4] = "rbw";
char COLOR[4] = "col";
char SEPARATOR = '#';

char commandChars[3];
boolean gotSEPARATOR = false;
byte gotCommandChars = 0;
char colorChars[6];
byte gotColorChars = 0;
boolean gotSTART = false;

rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}

void rainbow(byte brightness, boolean allTheSame=false){ 
  uint16_t time = millis() >> 5; //5
  for(uint16_t i = 0; i < LED_COUNT; i++){
    byte x = (time >> 2);
    if (!allTheSame){
      x -= (i << 4);
    }
    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, brightness);
  }
  ledStrip.write(colors, LED_COUNT);
  delay(10);
}

void loop(){
  if ( Serial.available() > 0 ) {     // Get data only when you receive data     
      char c = Serial.read();        //Read the incoming data & store into c
      Serial.println(c);
      if (gotSTART){
        /* reads command */
        if (!gotSEPARATOR){
          if(c != SEPARATOR){ // moze sie da z while i tu cztac 
//            Serial.println("NO SEPARATOR");
              commandChars[gotCommandChars] = c;
              gotCommandChars++; // moze trzeba tak jak receivedData[ gotHexNumbers ] = c;
          } else {
//            Serial.println("GOT SEPARATOR ELSE");
            gotSEPARATOR = true;
            gotCommandChars = 0;
          }    
        } else {
//          Serial.println("GOT SEPARATOR");
          // got whole command 
//          Serial.println(commandChars);
//          Serial.println(COLOR);
          if (strcmp(commandChars,COLOR) == 0){
//              Serial.println("COLOR");
              if ( c == END ){
                gotSTART = false;
                gotColorChars = 0;
                gotSEPARATOR = false;
//                Serial.println("WYSYLAM");
                sendToDiodes( hexToRgb(colorChars) );
//                colorChars = {};
//                commandChars = {};
              } else {
                colorChars[gotColorChars] = c;
                gotColorChars++;
              }
              // czytać kolor do END
          } else if (strcmp(commandChars, RAINBOW) == 0) {
            Serial.println("RAINBOW");
              // rainbow puścić 
              gotSTART = false;
              gotColorChars = 0;
              gotSEPARATOR = false;
//              colorChars = {};
//              commandChars = {};
              while(Serial.available() == 0){
                rainbow(255);
              }
//                sendToDiodes( hexToRgb(receivedData) ); // rainbow dać TODO
          } else {
            Serial.println(commandChars);
            Serial.println(RAINBOW);
            // jak sie spiepszyło coś 
            gotSTART = false;
            gotColorChars = 0;
            gotSEPARATOR = false;
          }
        }
      } else {
        if(c == START){
        gotSTART = true;
        }
      }
  }
}
