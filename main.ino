#include "BluetoothSerial.h"
#include "math.h"
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <ESP32_Servo.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
TinyGPSPlus gps;
HMC5883L compass;


//  MOTOR
Servo motor1;
Servo motor2;




#define RXD2 16
#define TXD2 17

float R = 6372.795477598; //Earth's radius
float pi = PI;

float currLat = 0.0000000;
float currLong = 0.0000000;
float distanceToTarget = 0.0;
float targetHeading = 0.0;
float currentHeading = 0.0;
int current_target = 0;
bool servoAtAngle; //Tracking servo 360 rotation status



float targerList[5][2] = {
  { 23.7982775, 90.4491337 },
  { 23.7982775, 90.4491337 },
  { 23.7982775, 90.4491337 },
  { 23.7982775, 90.4491337 },
  { 23.7982775, 90.4491337 },
};

int size = sizeof(targerList) / sizeof(targerList[0]);
int i = 0;

float targetLat = targerList[current_target][0];
float targetLong = targerList[current_target][1];

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);       //GPS

  motor1.attach(12, 1000, 2000);
  motor2.attach(14, 1000, 2000);


  SerialBT.begin("Autonomus System"); //Bluetooth device name
  Serial.println("Start. Pair it with bluetooth!");

  while (!compass.begin())
  {
    delay(500);
  }

  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  compass.setSamples(HMC5883L_SAMPLES_8);
  compass.setOffset(0, 0);
}
void loop() {
  while (i < size) {
    currentHeading = readCompass();
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read()))
      {
        readGPS();
      }
    }

    if (targetLat == 0 && targetLong == 0)
    {
      stopp();
      delay(2000);
    }
    else
    {
      targetHeading = getAngle(currLat, currLong, targetLat, targetLong);
      LeftRight();

      distanceToTarget = getDistance(currLat, currLong, targetLat, targetLong);
      if (distanceToTarget >= 5)
      {
        goo();
      }
      else if (getDistance(currLat, currLong, targetLat, targetLong) < 5) {
        SerialBT.println("NEXT POINT");
        delay(2000);
        i = i + 1;

        if (i == size) {
          stopp();
          delay(100000);
        }
        nextTarget();
      }
    }
  }
}
float getAngle( float lat1, float long1, float lat2, float long2 )
{
  float dlon = radians(long2 - long1);
  float cLat = radians(lat1);
  float tLat = radians(lat2);
  float a1 = sin(dlon) * cos(tLat);
  float a2 = sin(cLat) * cos(tLat) * cos(dlon);
  a2 = cos(cLat) * sin(tLat) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  int targetHeading = degrees(a2);

  return targetHeading;
}
float getDistance( float lat1, float long1, float lat2, float long2 )
{
  float latA = radians(lat1);
  float longA = radians(long1);
  float latB = radians(lat2);
  float longB = radians(long2);

  return ( 1000 * R * acos(  (sin(latA) * sin(latB)) + ( cos(latA) * cos(latB) * cos(longA - longB) ) ) );
}
void readGPS()
{
  if (gps.location.isValid())
  {
    currLat = gps.location.lat();
    currLong = gps.location.lng();

    Serial.print("Lat: ");
    Serial.print(currLat, 7);
    Serial.print(",");
    Serial.print("Lng: ");
    Serial.print(currLong, 7);
    Serial.println();
  }
}
int readCompass()
{
  float heading = atan2(compass.readNormalize().YAxis, compass.readNormalize().XAxis);  // Calculate heading
  //float declinationAngle = ((+10) + (6.0 / 60.0)) / (180 / M_PI);   // For Dhaka Magnetic Declination: -0° 29'
  float declinationAngle = ((-0) + (29 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  if (heading < 0)  heading += 2 * PI;
  if (heading > 2 * PI) heading -= 2 * PI;
  float headingDegrees = heading * 180 / M_PI;

  return ((int)headingDegrees);
}
void LeftRight() {
  while (true)
  {
    currentHeading = readCompass();
    while (Serial2.available() > 0)
      if (gps.encode(Serial2.read()))
      {
        readGPS();
      }
    targetHeading = getAngle(currLat, currLong, targetLat, targetLong);
    // distanceToTarget = getDistance(currLat, currLong, targetLat, targetLong);


    if (abs(currentHeading - targetHeading) <= 8)
    {
      break;
    }

    if (currentHeading > targetHeading) {
      if ((currentHeading - targetHeading) > 180)
      {
        rightward();
        SerialBT.println("u-Right...........");
      }
      else {
        leftward();
        SerialBT.println("u-Left.............");
      }
    }

    else
    {
      if ((targetHeading - currentHeading) > 180)
      {
        leftward();
        SerialBT.println("d-Left..............");
      }
      else
      {
        rightward();
        SerialBT.println("d-Right........");
      }
    }
  }
}
void goo()
{
  distanceToTarget = getDistance(currLat, currLong, targetLat, targetLong);

  if (distanceToTarget >= 5)
  {
    forward();
    SerialBT.print(distanceToTarget);
    SerialBT.println("      Gooooooooooooo");
    delay(400);
  }
}
void nextTarget() {
  current_target++;
  targetLat = targerList[current_target][0];
  targetLong = targerList[current_target][1];
  delay(7000);
}


void forward()
{
  motor1.write(1650);
  motor2.write(1650);
}


void backward()
{
  motor1.write(1250);
  motor2.write(1250);
}

//1250 r 1750 l 1700 f
void rightward()
{
  motor1.write(1600);
  motor2.write(1500);
}


void leftward()
{
  motor1.write(1500);
  motor2.write(1600);
}

void stopp()
{
  motor1.write(1500);
  motor2.write(1500);
}
