/*
 * Bluetooth program, to comunicate cloud with Android app
 * Reciving data via bluetooth 
*/

//TODO
// zobaczyc czy zamiana z hex na dec zajmuje duzo wiecej czasu od wysyłąnia w dec


#include <PololuLedStrip.h>
/* Create an ledStrip object and specify the pin it will use. */
PololuLedStrip<6> ledStrip;

/* Create a buffer for holding the colors (3 bytes per color). example: [255,255,255] */
#define LED_COUNT 20 // numbers of leds 

rgb_color colors[LED_COUNT]; // holds structures of color
rgb_color color;  // structure of 3 variables: red, green, blue


void setup(){
    Serial.begin(9600);                                
    pinMode(13, OUTPUT); 
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
char SEPARATOR = '#';
char RAINBOW[] = "rbw"; // sprawdzic czy z 3 będzie ok TODO
char COLOR[] = "col";
char RAINBOW_MODE[] = "mod";
char BRIGHTNESS[] = "bgh";

char commandChars[4];
byte gotCommandChars = 0;

char colorChars[7];
byte gotColorChars = 0;

char commandCharsIn[4];
byte gotCommandCharsIn = 0;
 
char rainbowModeChar;
char brightnessArray[4]; 
byte gotBrightnessChars = 0;

boolean gotSTART = false;
boolean gotSEPARATOR = false;


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

// polecenie <rbw> zrąbało program TODO moze gotStart dawac na false jak else

void loop(){
  if ( Serial.available() > 0 ) { /* Read data only when you receive data */  
      char c = Serial.read();        
//      Serial.println(c);
      if (gotSTART){
//        Serial.println("Start");
        if (gotSEPARATOR){ 
//          Serial.println("Separator");
//          Serial.println(commandChars); // to jest prawie caly czas col
//          Serial.println("st");
//          Serial.println(strcmp(commandChars,COLOR));
//          Serial.println("en");
//          Serial.println(COLOR);
          if (strcmp(commandChars,COLOR) == 0){
//            Serial.println("COLOR");
              if ( c == END ){
//                Serial.println("COLOR_END");
                gotSTART = false;
                gotColorChars = 0;
                gotSEPARATOR = false;
                sendToDiodes( hexToRgb(colorChars) );
              } else {
                colorChars[gotColorChars] = c;
                gotColorChars++;
              }
          } else if (strcmp(commandChars, RAINBOW) == 0) {
//              Serial.println("RAINBOW");
              if ( c == END ){
                gotSTART = false;
                gotColorChars = 0;
                gotSEPARATOR = false;
                gotBrightnessChars = 0;
                gotCommandCharsIn = 0;
                gotCommandChars = 0;
                
                int rainbowMode = (int)rainbowModeChar - 48;
                if (rainbowMode == 0 || rainbowMode == 1){
                  int brightness = atoi(brightnessArray);
                  while(Serial.available() == 0){
                    rainbow(brightness, rainbowMode);
                  }
                }
              } else {
                if(gotCommandCharsIn < 3){ 
                /* reads commands after main command */
                  commandCharsIn[gotCommandCharsIn] = c;
                  gotCommandCharsIn++; 
                } else {
                  if (strcmp(commandCharsIn, RAINBOW_MODE) == 0){
                    if (c != SEPARATOR){
                      rainbowModeChar = c;
                    } else {
                      gotCommandCharsIn = 0;
                    }
                  } else if (strcmp(commandCharsIn, BRIGHTNESS) == 0){
                    if(c != SEPARATOR){ 
                      /* reads brightness command */
                      brightnessArray[gotBrightnessChars] = c;
                      gotBrightnessChars++; 
                    } else {
                      gotBrightnessChars = 0;
                    }  
                  } else {
                    /* something failed and we didn't get correct message*/
                    gotSTART = false;
                    gotColorChars = 0;
                    gotSEPARATOR = false;
                    gotBrightnessChars = 0;
                    gotCommandCharsIn = 0;
                    gotCommandChars = 0;
                    // zrąbało się 
                  }
                }  
              }
          } else {
            /* something failed and we didn't get correct message*/
            gotSTART = false;
            gotColorChars = 0;
            // czyścić tablice TODO
            gotSEPARATOR = false;
            gotCommandChars = 0;
          }
        } else {
          if(c != SEPARATOR){ 
            /* reads first command */
            commandChars[gotCommandChars] = c;
//            Serial.println(commandChars);
            gotCommandChars++; 
          } else {
            gotSEPARATOR = true;
            gotCommandChars = 0;
          }  
        }   
      } else {
        if(c == START){
          gotSTART = true;
        }
      }
   }
}
