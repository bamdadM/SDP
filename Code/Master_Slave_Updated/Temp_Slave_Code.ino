#include <Wire.h>
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS


char wy[7];
char ix[8];
float latitude;
float longitude;

SFE_UBLOX_GPS myGPS;

void setup() {
 Serial.begin(9600);
 Wire.begin(2);
 Wire.onRequest(Request);
 myGPS.setNMEAOutputPort(Serial);
}

void loop() { 
     if(myGPS.begin() == true){
       myGPS.setI2COutput(COM_TYPE_UBX);
       myGPS.saveConfiguration();
       latitude = myGPS.getLatitude();
       latitude = latitude / 10000000.0;
//
       longitude = myGPS.getLongitude();
       longitude = longitude / 10000000;
//
       Serial.println(latitude, 7);
       Serial.println(longitude,7);
       Serial.println("True");
     }
     else {
       Serial.println("False");
   }
   delay(300);
}

void Request()
{
  if(latitude > longitude)
  {
    dtostrf(latitude,8,5,ix);
    Wire.write(ix);
    Wire.write(",");
    dtostrf(longitude,11,5,wy);
    delay(1000);
    Wire.write(wy);
    Wire.write("\n");
  }

  else
  {}
}
