#include <Wire.h>
#include <IBusBM.h>
#include <math.h>
#include <MPU6050_light.h>
#include <LIDARLite.h>
//#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
#include "bmm150.h"
#include "bmm150_defs.h"

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>  // Arduino SPI Library used for TFT



// Analog Input Channels
#define SCL A5 // SCL for Gyroscope
#define SDA A4 // SDA for Gyroscope

// Used for TFT Display
#define cs   10 // Control Select
#define dc   9  // Data Command Line
#define rst  8  // Reset


const int MPU = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t AcXinit, AcYinit, AcZinit, GyXinit, GyYinit, GyZinit;
float xyHeading, zxHeading, heading, headingDegrees, xyHeadingDegrees, zxHeadingDegrees;
unsigned long pulseWidth;
double pitch,roll;
const int buttonPin=2;
const int TriggerPin =4;
const int MonitorPin = 5;
int buttonState =0;
float pi =3.14;
bmm150_mag_data value;

// char arrays to print to the TFT screen (Can be adjusted for move levels of precision)
char CharLat[7];
char CharLong[9];
char CharLat2[7];
char CharLong2[9];
char CharX[5];
char CharY[5];
char CharZ[5];

BMM150 bmm = BMM150();            // Creating an instance of the compass
TFT TFTscreen = TFT(cs, dc, rst); // Creating an instance of the TFT display

MPU6050 mpu(Wire);
LIDARLite lidarLite;
//SFE_UBLOX_GPS myGPS;


void setup() 
{

    TFTscreen.begin();
    TFTscreen.background(0, 0, 0);    // Set the Background to black    
    TFTscreen.stroke(255, 255, 255);  // Set the font color to White
    TFTscreen.setTextSize(1);         // Set the text size to 1
    
    Wire.begin();
    Serial.begin(9600);
    byte status = mpu.begin();
    lidarLite.begin(0, true);         // Set configuration to default and I2C to 400 kHz
    lidarLite.configure(0);           // Change this number to try out alternate configurations
    Wire.begin();                     // Why a second Wire.begin() 
    //myGPS.setNMEAOutputPort(Serial);


    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0) {}  // stop everything if could not connect to MPU6050
    Serial.println(("Calculating offsets, do not move MPU6050"));
    
    TFTscreen.text("Calculating offsets, do not move MPU6050 ", 0, 0);  // Print this onto the TFT screen  
    
    delay(1000);
    mpu.calcOffsets(); // gyro and accelero

    TFTscreen.stroke(0,0,0);                                            // Set the font to black
    TFTscreen.text("Calculating offsets, do not move MPU6050 ", 0, 0);  // Print this onto the TFT screen (Basically erasing the text)
    
    Serial.println("Done!\n");

    TFTscreen.stroke(255,255,255);                                      // Set the font back to white
    TFTscreen.text("Done", 0, 0);                                       // Print this onto TFT screen
    delay(1000);
    TFTscreen.stroke(0,0,0);                                            // Set the font to black
    TFTscreen.text("Done", 0, 0);                                       // Print this onto TFT screen (Basically erasing the text)
    TFTscreen.stroke(255,255,255);                                      // Set the font back to white
    TFTscreen.text("Intializing Compass...", 0, 0);                     // Print this onto TFT screen
    

    // GPS Init
    //Wire.begin();
    //myGPS.setNMEAOutputPort(Serial);

    // Compass Init
    if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) 
      {
        TFTscreen.stroke(0,0,0);
        TFTscreen.text("Intializing Compass...", 0, 0); 
        TFTscreen.stroke(255,255,255);
        TFTscreen.text("Chip ID can not read!", 0, 0);  
        Serial.println("Chip ID can not read!");
        while(1);
      } 
    else
      {
        TFTscreen.stroke(0,0,0);
        TFTscreen.text("Intializing Compass...", 0, 0); 
        TFTscreen.stroke(255,255,255);
        TFTscreen.text("Initialize done!", 0, 0);
        Serial.println("Initialize done!");
        delay(500);
        TFTscreen.stroke(0,0,0);
        TFTscreen.text("Initialize done!", 0, 0); 
      }

    TFTscreen.stroke(255,255,255);                                      
    TFTscreen.text("GPS Coordinates and Angles:\n ", 0, 0);  

    pinMode(buttonPin, INPUT);    // used to get the input of the button
    pinMode(TriggerPin, OUTPUT);
    pinMode(MonitorPin, INPUT);
    digitalWrite(TriggerPin, LOW);


}

void loop()
{
  int dist;

   mpu.update();
   
   // Read MPU Data
   AcX= mpu.getAngleX();
   AcY=mpu.getAngleY();
   AcZ=mpu.getAngleZ();

   // Read Compass Data
   bmm.read_mag_data();

   // Compass Raw Data
   value.x = bmm.raw_mag_data.raw_datax;
   value.y = bmm.raw_mag_data.raw_datay;
   value.z = bmm.raw_mag_data.raw_dataz;
   xyHeading = atan2(value.x, value.y);
   zxHeading = atan2(value.z, value.x);
   heading = xyHeading;

   // Reading Button State
   buttonState = digitalRead(buttonPin);
   pulseWidth = pulseIn(MonitorPin, HIGH);


if(buttonState==HIGH)
{

      dist = lidarLite.distance();      // With bias correction
      Serial.print("You have pushed the button, Here are your saved values: \n");

      // MPU6050 Angle Data
      AcXinit= mpu.getAngleX();
      AcYinit= mpu.getAngleY();
      AcZinit= mpu.getAngleZ();

      // Compass Raw Data
      value.x = bmm.raw_mag_data.raw_datax;
      value.y = bmm.raw_mag_data.raw_datay;
      value.z = bmm.raw_mag_data.raw_dataz;
      xyHeading = atan2(value.x, value.y);
      zxHeading = atan2(value.z, value.x);
      heading = xyHeading;

      // Reading GPS Data
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
      
      print_data(AcXinit,AcYinit,AcZinit,dist, heading, num1, num2);
  }
else 
{
      dist=lidarLite.distance();
      // Reading GPS Data
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
      print_data(AcX,AcY,AcZ, dist, heading, num1,num2);
}

}

void print_data(int16_t Ax, int16_t Ay, int16_t Az, int dist, float heading, float num1, float num2)
  {
    
   // Compass Arithmetic
    if(heading < 0)
       heading += 2*PI;
    if(heading > 2*PI)
       heading -= 2*PI;
      
    headingDegrees = heading * 180/M_PI;
    xyHeadingDegrees = xyHeading * 180 / M_PI;
    zxHeadingDegrees = zxHeading * 180 / M_PI;

  // Display MPU Angles on Serial Montitor
    Serial.print("Angles: ");
    Serial.print("X = "); Serial.print(Ax);
    Serial.print(" | Y = "); Serial.print(Ay);
    Serial.print(" | Z = "); Serial.println(Az);
    
  // Converting values to strings
    String Latitude = String(num1,7);
    String Longitude= String(num2,8);
    String AngleX = String(Ax,5);
    String AngleY=String(Ay,5);
    String AngleZ = String(Az,5);
    
  // Converting Strings to Char Array
    Latitude.toCharArray(CharLat, 7);
    Longitude.toCharArray(CharLong,9);
    AngleX.toCharArray(CharX,5);
    AngleY.toCharArray(CharY,5);
    AngleZ.toCharArray(CharZ,5);

  // Print the values
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.text("Your Coordinates:", 0, 10);
    TFTscreen.text(CharLat, 0, 20);
    TFTscreen.text(", ", 35, 20);
    TFTscreen.text(CharLong, 45, 20);
    
    calculate_target_coord(dist, heading, num1, num2);

    TFTscreen.text("Angles:", 0, 70);
    TFTscreen.text("X: ",0,80);
    TFTscreen.text(CharX, 20, 80);
    TFTscreen.text("Y: ",0,90);
    TFTscreen.text(CharY, 20, 90);
    TFTscreen.text("Z: ",0,100);
    TFTscreen.text(CharZ, 20, 100);
     
  // Display LIDAR Distance  
    Serial.print("dist = "); Serial.println(dist);
    Serial.print("\n\n");
    
  // Display Heading of Compass
    Serial.print("Heading: ");
    Serial.println(headingDegrees);

  // Display GPS Data
    Serial.print("\n");
    Serial.print("Float Conversion 1: ");
    Serial.println(num1, 6);
    Serial.print("Float Conversion 2: ");
    Serial.println(num2, 6);
    Serial.print("\n");
  
    delay(300);
    
  // Erase the data on the TFT display for updating
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(CharLat, 0, 20);
    TFTscreen.text(CharLong, 45, 20);
    TFTscreen.text(CharLat2, 0, 50);
    TFTscreen.text(CharLong2, 45, 50);
    TFTscreen.text(CharX, 20, 80);
    TFTscreen.text(CharY, 20, 90);
    TFTscreen.text(CharZ, 20, 100);

  }

void calculate_target_coord(int dist, float heading, float lattitude, float longtitude)

{
    float r_earth=6378.1*1000;//radius of the earth
    // converting to radians
    float latr=lattitude*pi/180;
    float longr=longtitude*pi/180;
    int16_t bearingR=heading*pi/180;
    //Finish converting to radians
    
    int16_t east=dist*sin(bearingR);    //distance has to be in meters
    int16_t north=dist*cos(bearingR);   //distance has to be in meters
    int16_t deglat=r_earth*pi/180;
    int16_t deglong=r_earth*pi/180*cos(latr);
    int16_t dlat=north/deglat;
    int16_t dlon=east/deglong;
    int16_t lat2=deglat+dlat;
    int16_t long2=deglong+dlon;

    String Latitude = String(lat2,7);
    String Longitude= String(long2,8);
    Latitude.toCharArray(CharLat2, 7);
    Longitude.toCharArray(CharLong2,9);
    
    TFTscreen.text("Object Coordinates:", 0, 40);
    TFTscreen.text(CharLat2, 0, 50);
    TFTscreen.text(", ", 35, 50);
    TFTscreen.text(CharLong2, 45, 50);

}


//#include <Wire.h>
//#include "GY521.h"
//#include <Wire.h>
//#include <IBusBM.h>
//#include <math.h>
//#include <MPU6050_light.h>
//#include <LIDARLite.h>
////#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
//#include "bmm150.h"
//#include "bmm150_defs.h"
//
//
//
//// Analog Input Channels
//#define SCL A5 // SCL for Gyroscope
//#define SDA A4 // SDA for Gyroscope
//
//
//const int MPU = 0x68;
//int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
//int16_t AcXinit, AcYinit, AcZinit, GyXinit, GyYinit, GyZinit;
//float xyHeading, zxHeading, heading, headingDegrees, xyHeadingDegrees, zxHeadingDegrees;
//unsigned long pulseWidth;
//double pitch,roll;
//const int buttonPin=2;
//const int TriggerPin =4;
//const int MonitorPin = 5;
//int buttonState =0;
//bmm150_mag_data value;
//BMM150 bmm = BMM150();
//
//MPU6050 mpu(Wire);
//LIDARLite lidarLite;
////SFE_UBLOX_GPS myGPS;
//
//
//void setup() 
//{
//  Wire.begin();
//  Serial.begin(9600);
//  byte status = mpu.begin();
//  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
//  lidarLite.configure(0); // Change this number to try out alternate configurations
//  Wire.begin();           // Why a second Wire.begin() 
//  //myGPS.setNMEAOutputPort(Serial);
//
//
//    Serial.print(F("MPU6050 status: "));
//    Serial.println(status);
//    while (status != 0) {}  // stop everything if could not connect to MPU6050
//    Serial.println(("Calculating offsets, do not move MPU6050"));
//    delay(1000);
//    mpu.calcOffsets(); // gyro and accelero
//    Serial.println("Done!\n");
//
//    // GPS Init
//    //Wire.begin();
//    //myGPS.setNMEAOutputPort(Serial);
//
//    // Compass Init
//    if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) 
//    {
//      Serial.println("Chip ID can not read!");
//      while(1);
//    } 
//    else
//      Serial.println("Initialize done!");
//      
//
//    pinMode(buttonPin, INPUT);    // used to get the input of the button
//    pinMode(TriggerPin, OUTPUT);
//    pinMode(MonitorPin, INPUT);
//    digitalWrite(TriggerPin, LOW);
//}
//
//void loop()
//{
//  int dist;
//
//   mpu.update();
//   
//   // Read MPU Data
//   AcX= mpu.getAngleX();
//   AcY=mpu.getAngleY();
//   AcZ=mpu.getAngleZ();
//
//   // Read Compass Data
//   bmm.read_mag_data();
//
//   // Compass Raw Data
//   value.x = bmm.raw_mag_data.raw_datax;
//   value.y = bmm.raw_mag_data.raw_datay;
//   value.z = bmm.raw_mag_data.raw_dataz;
//   xyHeading = atan2(value.x, value.y);
//   zxHeading = atan2(value.z, value.x);
//   heading = xyHeading;
//
//   // Reading Button State
//   buttonState = digitalRead(buttonPin);
//   pulseWidth = pulseIn(MonitorPin, HIGH);
//
//
//if(buttonState==HIGH)
//{
//
//      //delay(1000); // delay by 5s
//      dist = lidarLite.distance();      // With bias correction
//      Serial.print("You have pushed the button, Here are your saved values: \n");
//
//      // MPU6050 Angle Data
//      AcXinit= mpu.getAngleX();
//      AcYinit=mpu.getAngleY();
//      AcZinit=mpu.getAngleZ();
//
//      // Compass Raw Data
//      value.x = bmm.raw_mag_data.raw_datax;
//      value.y = bmm.raw_mag_data.raw_datay;
//      value.z = bmm.raw_mag_data.raw_dataz;
//      xyHeading = atan2(value.x, value.y);
//      zxHeading = atan2(value.z, value.x);
//      heading = xyHeading;
//
//      // Reading GPS Data
//      Wire.requestFrom(2, 20);
//      String string, string1, string2;
//
//      do
//      {
//        char c = Wire.read();
//
//        string =string +c;
//        string1 = string.substring(0, 8);
//        string2 = string.substring(9);
//    
//      } while(Wire.available());
//
//      float num1= string1.toFloat();
//      float num2 =string2.toFloat();
//      
//      print_data(AcXinit,AcYinit,AcZinit,dist, heading, num1, num2);
//  }
//else 
//{
//    dist=lidarLite.distance();
//      // Reading GPS Data
//      Wire.requestFrom(2, 20);
//      String string, string1, string2;
//
//      do
//      {
//        char c = Wire.read();
//
//        string =string +c;
//        string1 = string.substring(0, 8);
//        string2 = string.substring(9);
//    
//      } while(Wire.available());
//
//      float num1= string1.toFloat();
//      float num2 =string2.toFloat();
//    print_data(AcX,AcY,AcZ, dist, heading, num1,num2);
//}
//
//}
//
//void print_data(int16_t Ax, int16_t Ay, int16_t Az, int dist, float heading, float num1, float num2)
//  {
//    
//   // Compass Arithmetic
//    if(heading < 0)
//       heading += 2*PI;
//    if(heading > 2*PI)
//       heading -= 2*PI;
//      
//    headingDegrees = heading * 180/M_PI;
//    xyHeadingDegrees = xyHeading * 180 / M_PI;
//    zxHeadingDegrees = zxHeading * 180 / M_PI;
//
//   // Display MPU Angles
//    Serial.print("Angles: ");
//    Serial.print("X = "); Serial.print(Ax);
//    Serial.print(" | Y = "); Serial.print(Ay);
//    Serial.print(" | Z = "); Serial.println(Az);
//
//  // Display LIDAR Distance  
//    Serial.print("dist = "); Serial.println(dist);
//    Serial.print("\n\n");
//    
//  // Display Heading of Compass
//    Serial.print("Heading: ");
//    Serial.println(headingDegrees);
//
//  // Display GPS Data
//  Serial.print("\n");
//  Serial.print("Float Conversion 1: ");
//  Serial.println(num1, 6);
//  Serial.print("Float Conversion 2: ");
//  Serial.println(num2, 6);
//  Serial.print("\n");
//
//    delay(300);
//
//  }
