#include <Wire.h>
int data = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin(5);
  Wire.onRequest(requestedData);
}

void requestedData()
{
  Wire.write(data);
}

void loop()
{
  data++;
  delay(1000);
}
