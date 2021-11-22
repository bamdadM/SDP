#include <Wire.h>
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS

SFE_UBLOX_GPS myGPS;

void setup() {
 Serial.begin(115200);
 Wire.begin();
 myGPS.setNMEAOutputPort(Serial);
}

void loop() { 
     if(myGPS.begin() == true){
       myGPS.setI2COutput(COM_TYPE_UBX);
       myGPS.saveConfiguration();
       float latitude = myGPS.getLatitude();
       latitude = latitude / 10000000.0;
//
      float longitude = myGPS.getLongitude();
       longitude = longitude / 10000000;
//
       Serial.println(latitude);
       Serial.println(longitude);
       Serial.println("True");
     }
     else {
       Serial.println("False");
   }
   delay(300);
}
