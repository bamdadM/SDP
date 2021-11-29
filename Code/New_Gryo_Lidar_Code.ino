#include "GY521.h"
#include <Wire.h>
#include <IBusBM.h>
#include <math.h>
#include <MPU6050_light.h>


// Analog Input Channels
#define SCL A5 // SCL for Gyroscope
#define SDA A4 // SDA for Gyroscope


/*
void setup() {
    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    pinMode(CH3, INPUT);
    pinMode(CH4, INPUT);
    pinMode(CH5, INPUT);
}
void loop() {
    
}
*/

const int MPU = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t AcXinit, AcYinit, AcZinit, GyXinit, GyYinit, GyZinit;
unsigned long pulseWidth;
double pitch,roll;
const int buttonPin=2;
const int TriggerPin =4;
const int MonitorPin = 5;
int buttonState =0;
 MPU6050 mpu(Wire);
void setup() 
{
    Serial.begin(9600);
    Wire.begin();
    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0) { } // stop everything if could not connect to MPU6050
    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(); // gyro and accelero
    Serial.println("Done!\n");
   
    pinMode(buttonPin, INPUT);    // used to get the input of the button
    pinMode(TriggerPin, OUTPUT);
    pinMode(MonitorPin, INPUT);
    digitalWrite(TriggerPin, LOW);
}

void loop()
{

   mpu.update();

   AcX= mpu.getAngleX();
   AcY=mpu.getAngleY();
   AcZ=mpu.getAngleZ();
   buttonState = digitalRead(buttonPin);
   
if( buttonState==HIGH)
{
    
      //delay(1000); // delay by 5s
      pulseWidth = pulseIn(MonitorPin, HIGH);
      Serial.print("You have pushed the button, Here are your saved values: \n");

      AcXinit= mpu.getAngleX();
      AcYinit=mpu.getAngleY();
      AcZinit=mpu.getAngleZ();

      print_data(AcXinit,AcYinit,AcZinit,pulseWidth);
  }
else 
    print_data(AcX,AcY,AcZ, pulseWidth);
  
}

void print_data(int16_t Ax, int16_t Ay, int16_t Az, unsigned long pulseWidth)
  {
   if(pulseWidth!= 0)
    {
      pulseWidth = pulseWidth/10;
      Serial.print("The distance in cm is: ");
      Serial.print(pulseWidth);
      Serial.print("\n");
    }

     
    Serial.print("Angles: ");
    Serial.print("X = "); Serial.print(Ax);
    Serial.print(" | Y = "); Serial.print(Ay);
    Serial.print(" | Z = "); Serial.println(Az);
  }
