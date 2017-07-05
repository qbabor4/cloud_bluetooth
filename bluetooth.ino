/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth 
 * Data is read by 6 chars 
 * 
*/

//TODO
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec
// zamiana z hex na 3 bajty
// mryganie przy wiekszej ilości danych na raz ( jak sie jeździ palcem po kółku )
// najwyzej poczatkowy i koncowy znacznik

// dostawanie danych ze znacznikiem poczatkowym i koncowym
// jak nie jest 6 to nic nie robić  
///!!!!!!!!!!!!!!!!!!!!


#include <PololuLedStrip.h>
// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<6> ledStrip;

// Create a buffer for holding the colors (3 bytes per color). example: [255,255,255]
#define LED_COUNT 20 // numbers of leds 

rgb_color colors[LED_COUNT]; // holds structures of color
rgb_color color;  // structure of 3 variables: red, green, blue

char receivedData[6]; // received data from bluetooth
byte gotHexNumbers = 0; // how many chars program have read; App sends 6-chars-long data
boolean startSavingData = false; // saves data only when true ( between # and > )


void setup()
{
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
  //strncpy(gHex, hexColor+2, 0);
  //strncpy(bHex, hexColor+4, 0);
  //char gHex[2] = {hexColor[2], hexColor[3]};
  //char bHex[2] = {hexColor[4], hexColor[5]};
  //r = hexrToDec(rHex);
  //g = hexrToDec(gHex);
  //b = hexrToDec(bHex);
  
  //Serial.println(r);
  //Serial.println(g);
  //Serial.println(b);
  //r = strtol(hexColor[0,1], 0, 16);
 
  return (rgb_color){r, g, b};
}


byte hexToDec(char *s) // opt // źle zamienia. // to zrobić
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
 return (uint8_t)x;
}

void sendToDiodes(rgb_color color){
  for ( uint16_t i = 0; i < LED_COUNT; i++ ){
    colors[i] = color;
  }
  ledStrip.write(colors,LED_COUNT);
  delay(10);
}

void loop()
{
  
    if ( Serial.available() > 0 ) {     // Get data only when you receive data     
        char c = Serial.read();        //Read the incoming data & store into c
        if (c == '#'){
          startSavingData = true;
        } else if ( c == '>' ){
          // moze wywalić nadpisywac znowu pierwsze kolory i łączyc poprzedni kolor z tym teraz
          // długosc musiałaby byc 6 ( String i konkatenacja? )
          startSavingData = false;
          gotHexNumbers = 0;
          sendToDiodes( hexToRgb(receivedData) );
        } else {
          if (startSavingData){
             receivedData[ gotHexNumbers ] = c;
             gotHexNumbers += 1;
          }
        }
        /*if ( gotChars ==  5 ){ 
          receivedData[gotChars] = c;
          Serial.println( receivedData );          
          //Serial.print("\n");
          gotChars = 0;
          sendToDiodes( hexToRgb(receivedData) );          
        } 
        else {
            receivedData[gotChars] = c;
            gotChars += 1;
        } */

         
    }
}
