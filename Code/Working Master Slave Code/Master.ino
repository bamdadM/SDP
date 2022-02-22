#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  Wire.requestFrom(2, 20);
  String string, string1, string2;

  do
  {
    char c = Wire.read();

    string =string +c;
    string1 = string.substring(0, 8);
    string2 = string.substring(9);
    
  } while(Wire.available());

float num1= string1.toFloat();
float num2 =string2.toFloat();



Serial.print ("Recieving float as String...");
Serial.print("\n");
Serial.print("\n");
Serial.print("Full string: ");
Serial.print(string);

Serial.print("\n");
Serial.print("Float Conversion 1: ");
Serial.println(num1, 6);
Serial.print("Float Conversion 2: ");
Serial.println(num2, 6);
Serial.print("\n");

delay(500);

}
