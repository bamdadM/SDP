#include "GY521.h"
#include <Wire.h>
#include <IBusBM.h>
#include <math.h>

/************************************ For MPU **************************************/
#include <MPU6050_light.h> // Import this library
MPU6050 mpu(Wire);
 ////////////////////////////////////////////////////////////////////////////////////////

/******************************For GPS Module*********************************************/
#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
SFE_UBLOX_GPS myGPS;
//////////////////////////////////////////////////////////////////////////////////////////

// Analog Input Channels
#define SDA A4 // SDA for Gyroscope
#define SCL A5 // SCL for Gyroscope


// Gloabl variables used for:
const int MPU = 0x68;         // MPU
const int buttonPin = 2;      // Button
const int TriggerPin = 4;     // Lidar
const int MonitorPin = 5;     // Lidar
int buttonState = 1;          // Button
 unsigned long timer = 0;     // MPU

// Global variables for 1st press of button
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t AcXinit, AcYinit, AcZinit, GyXinit, GyYinit, GyZinit;
unsigned long pulseWidth;
double pitch,roll;

// Global variables for 2nd press of button
int16_t AcX_2,AcY_2,AcZ_2,Tmp_2,GyX_2,GyY_2,GyZ_2;
int16_t AcXinit_2, AcYinit_2, AcZinit_2, GyXinit_2, GyYinit_2, GyZinit_2;
double pitch_2,roll_2;

// GPS Global variables
float user_latitude, user_longitude;

void setup() 
{
    Wire.begin(); 
    Wire.beginTransmission(MPU);    // Begin I2C transmission to the MPU (slave) with the given address (0x68)
    Wire.write(0x6B);               // Queue bytes for transmisssion
    Wire.write(0);                  // Queue bytes for transmission
    Wire.endTransmission(true);     // Transmit bytes that were queued by Wire.write(num)
    Serial.begin(9600);             // sets data rate for bps (baud) for serial data transmission
    pinMode(buttonPin, INPUT);      // used to get the input of the button
    pinMode(TriggerPin, OUTPUT);
    pinMode(MonitorPin, INPUT);
    digitalWrite(TriggerPin, LOW);

    myGPS.setNMEAOutputPort(Serial); // GPS initialization

    Serial.begin(9600);
    Wire.begin();
    byte status = mpu.begin();
    mpu.calcOffsets(); // gyro and accelero
}

void loop()
{
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);

    int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
    //pulseWidth= pulseIn(MonitorPin,HIGH);

    //Acceleration data correction. NEED TO GET ADJUSTED FOR NEW MPU
    AcXoff = 0;
    AcYoff = 0;
    AcZoff = 0;

    //Gyro correction. NEED TO GET ADJUSTED FOR NEW MPU
    GyXoff = 0;
    GyYoff = 0;
    GyZoff = 0;

   //buttonState = digitalRead(buttonPin);
   if(digitalRead(buttonPin) == HIGH)
   {
    
      //delay(1000); // delay by 5s
      pulseWidth = pulseIn(MonitorPin, HIGH);
      Serial.print("You have pushed the button, Here are your saved values: \n");
      //read accel data
      AcXinit=(Wire.read()<<8|Wire.read()) + AcXoff;
      AcYinit=(Wire.read()<<8|Wire.read()) + AcYoff;
      AcZinit=(Wire.read()<<8|Wire.read()) + AcZoff;
    
    
      //read gyro data
      GyXinit=(Wire.read()<<8|Wire.read()) + GyXoff;
      GyYinit=(Wire.read()<<8|Wire.read()) + GyYoff;
      GyZinit=(Wire.read()<<8|Wire.read()) + GyZoff;

    
      //get pitch/roll
      getAngle(AcXinit,AcYinit,AcZinit);

      // get GPS coordinates
      user_latitude = getLatitude();
      user_longitude = getLongitude();
    
      //send the data out the serial port
      print_data(AcXinit,AcYinit,AcZinit,GyXinit,GyYinit,GyZinit,pulseWidth,user_latitude, user_longitude);
  }
//else 
//  {
//    getdata(AcXoff,AcYoff,AcZoff, GyXoff,GyYoff,GyZoff, pulseWidth, user_latitude, user_longitude);
//  }

}

//void getdata(int AcXoff, int AcYoff, int AcZoff, int GyXoff, int GyYoff, int GyZoff, unsigned long pulseWidth, float latitude, float longitude)
//  {
//    
//    //read accel data
//    AcX=(Wire.read()<<8|Wire.read()) + AcXoff;      // Get the X axis Accelerometer data
//    AcY=(Wire.read()<<8|Wire.read()) + AcYoff;      // Get the Y axis Accelerometer data
//    AcZ=(Wire.read()<<8|Wire.read()) + AcZoff;      // Get the Z axis Accelerometer data
//    
//    //read gyro data
//    GyX=(Wire.read()<<8|Wire.read()) + GyXoff;      // Get the X axis Gyroscope data
//    GyY=(Wire.read()<<8|Wire.read()) + GyYoff;      // Get the Y axis Gyroscope data
//    GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;      // Get the Z axis Gyroscope data
//    
//    //get pitch/roll
//    getAngle(AcX,AcY,AcZ);                          // This function is used to get pitch and roll
//    
//    //send the data out the serial port
//    print_data(AcX,AcY,AcZ,GyX,GyY,GyZ, pulseWidth, latitude, longitude);
//  }

void getAngle(int Vx,int Vy,int Vz) {
    double x = Vx;
    double y = Vy;
    double z = Vz;
    pitch = atan(x/sqrt((y*y) + (z*z)));
    roll = atan(y/sqrt((x*x) + (z*z)));
    //convert radians into degrees
    pitch = pitch * (180.0/3.14);
    roll = roll * (180.0/3.14) ;
}


void print_data(int16_t Ax, int16_t Ay, int16_t Az, int16_t Gx, int16_t Gy, int16_t Gz, unsigned long pulseWidth, float latitude, float longitude)
  {
   if(pulseWidth!= 0)
    {
      pulseWidth = pulseWidth/10;
      Serial.print("The distance in cm is: ");
      Serial.print(pulseWidth);
      Serial.print("\n");
    }

    //send the data out the serial port
    Serial.print("Angle: ");
    Serial.print("Pitch = "); Serial.print(pitch);
    Serial.print(" | Roll = "); Serial.println(roll);
     
    Serial.print("Accelerometer: ");
    Serial.print("X = "); Serial.print(Ax);
    Serial.print(" | Y = "); Serial.print(Ay);
    Serial.print(" | Z = "); Serial.println(Az);
    
    Serial.print("Gyroscope: ");
    Serial.print("X = "); Serial.print(Gx);
    Serial.print(" | Y = "); Serial.print(Gy);
    Serial.print(" | Z = "); Serial.println(Gz);
    Serial.println(" ");

    Serial.print("GPS Coordinates: ");
    Serial.print("Latitude: "); Serial.println(latitude);
    Serial.print("Longitude: "); Serial.println(longitude);

  }

float getLatitude(){
       float latitude;
       
       if(myGPS.begin() == true){
       myGPS.setI2COutput(COM_TYPE_UBX);
       myGPS.saveConfiguration();
       latitude = myGPS.getLatitude();
       latitude = latitude / 10000000.0;
       }
       else{
        Serial.println("False");
       }
       
       return latitude;
}

float getLongitude(){
       float longitude;
       
       if(myGPS.begin() == true){
       myGPS.setI2COutput(COM_TYPE_UBX);
       myGPS.saveConfiguration();
       longitude = myGPS.getLongitude();
       longitude = longitude / 10000000.0;
       }
       else{
        Serial.println("False");
       }
       
       return longitude;
}
