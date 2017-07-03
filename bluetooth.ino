/*
 * Bluetooth controled program, to comunicate cloud with Android app
 * Reciving data via bluetooth with end-marker '>'
*/

//TODO
// dostac caly hex string i dopiero wtedy coś robic

String recivedData;            //Variable for storing received data
const byte numChars = 32;
char receivedChars[numChars]; // array of data

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
}
void loop()
{
      if ( Serial.available() > 0 ) {     // Get data only when you receive data:
          char c = Serial.read();        //Read the incoming data & store into c
          recivedData += c;
          Serial.print( recivedData );          
          Serial.print("\n");
          Serial.print("\n");     
          if( recivedData == '1')              // Checks whether value of data is equal to 1
             digitalWrite(13, HIGH);   //If value is 1 then LED turns ON
          else if( recivedData == '0')         //  Checks whether value of data is equal to 0
             digitalWrite(13, LOW);    //If value is 0 then LED turns OFF
   }
  }
}
