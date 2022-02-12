#include<Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
 }

void loop()
{   
      Wire.requestFrom(5,4);
        int c = Wire.read();
        Serial.print(c);
        Serial.print('\n'); 
        delay(1000);
  }
