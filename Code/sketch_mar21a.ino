#include <Wire.h>
#include "GY521.h"
#include <Wire.h>
#include <IBusBM.h>
#include <math.h>
#include <MPU6050_light.h>
#include <LIDARLite.h>
//#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
#include "bmm150.h"
#include "bmm150_defs.h"



// Analog Input Channels
#define SCL A5 // SCL for Gyroscope
#define SDA A4 // SDA for Gyroscope


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
bmm150_mag_data value;
BMM150 bmm = BMM150();

MPU6050 mpu(Wire);
LIDARLite lidarLite;
//SFE_UBLOX_GPS myGPS;


void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  byte status = mpu.begin();
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
  Wire.begin();           // Why a second Wire.begin() 
  //myGPS.setNMEAOutputPort(Serial);


    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0) {}  // stop everything if could not connect to MPU6050
    Serial.println(("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(); // gyro and accelero
    Serial.println("Done!\n");

    // GPS Init
    //Wire.begin();
    //myGPS.setNMEAOutputPort(Serial);

    // Compass Init
    if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) 
    {
      Serial.println("Chip ID can not read!");
      while(1);
    } 
    else
      Serial.println("Initialize done!");
      

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

      //delay(1000); // delay by 5s
      dist = lidarLite.distance();      // With bias correction
      Serial.print("You have pushed the button, Here are your saved values: \n");

      // MPU6050 Angle Data
      AcXinit= mpu.getAngleX();
      AcYinit=mpu.getAngleY();
      AcZinit=mpu.getAngleZ();

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

   // Display MPU Angles
    Serial.print("Angles: ");
    Serial.print("X = "); Serial.print(Ax);
    Serial.print(" | Y = "); Serial.print(Ay);
    Serial.print(" | Z = "); Serial.println(Az);

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

  }

void calculate_target_coord(int dist, float heading, float lattitude, float longtitude){
r=6378.1*1000;//radius of the earth
// converting to radians
float latr=lattitude*pi/180
float longr=longtitude*pi/180
int16_t bearingR=heading*pi/180
//Finish converting to radians

int16_t east=dist*sin(bearingR)    //distance has to be in meters
int16_t north=dist*cos(bearingR)   //distance has to be in meters
int16_t deglat=r_earth*pi/180
int16_t deglong=r*pi/190*cos(latr)
int16_t dlat=north/deglat
int16_t dlon=east/deglong
int16_t lat2=latd+dlat
int16_t long2=longd+dlon



}
}
