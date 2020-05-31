//for GPS
#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <SoftwareSerial.h>      // Include software serial library
 
TinyGPSPlus gps;
 
#define S_RX    4                // Define software serial RX pin
#define S_TX    3                // Define software serial TX pin
 
SoftwareSerial SoftSerial(S_RX, S_TX);    // Configure SoftSerial library

const int LDR=A0;
const int sensorled=13 ;
const int redled=12 ;
const int bluled=11;
const int grnled=10;
const int button=9;


void setup()
{
  Serial.begin(9600);
  SoftSerial.begin(9600);
  Serial.print("screen on");
  pinMode(sensorled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  pinMode(bluled, OUTPUT);
  pinMode(button, INPUT);
  pinMode (LDR, INPUT);

}

int counter =1 , x=20;
bool buttonRead = digitalRead(button);
int LDRvalue =0;

void loop() {
  

 if (!buttonRead)
 {
  buttonRead= digitalRead(button);
 }
 
 else
 {
   
digitalWrite(sensorled,HIGH);
 LDRvalue = analogRead(LDR);
 digitalWrite (redled, LOW);
 digitalWrite (grnled, LOW);
 
  //check if it's a Positive case
  if (LDRvalue>600&&counter<=x) // by assuming that the color change start when the sensor read is 600
  {
  digitalWrite (redled, HIGH);

  //Start GPS signal
 while (SoftSerial.available() > 0) {
 
    if (gps.encode(SoftSerial.read())) {
 
      if (gps.location.isValid()) {
        Serial.print("Latitude   = ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude  = ");
        Serial.println(gps.location.lng(), 6);
      }
      else
        Serial.println("Location Invalid");
 
      if (gps.altitude.isValid()) {
        Serial.print("Altitude   = ");
        Serial.print(gps.altitude.meters());
        Serial.println(" meters");
      }
      else
        Serial.println("Altitude Invalid");
 
      if (gps.speed.isValid()) {
        Serial.print("Speed      = ");
        Serial.print(gps.speed.kmph());
        Serial.println(" kmph");
      }
      else
        Serial.println("Speed Invalid");
 
      if (gps.time.isValid()) {
        Serial.print("Time (GMT) : ");
        if(gps.time.hour() < 10)     Serial.print("0");
        Serial.print(gps.time.hour());
        Serial.print(":");
        if(gps.time.minute() < 10)   Serial.print("0");
        Serial.print(gps.time.minute());
        Serial.print(":");
        if(gps.time.second() < 10)   Serial.print("0");
        Serial.println(gps.time.second());
      }
      else
        Serial.println("Time Invalid");
 
      if (gps.date.isValid()) {
        Serial.print("Date       : ");
        if(gps.date.day() < 10)      Serial.print("0");
        Serial.print(gps.date.day());
        Serial.print("/");
        if(gps.date.month() < 10)    Serial.print("0");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.println(gps.date.year());
      }
      else
        Serial.println("Date Invalid");
 
      if (gps.satellites.isValid()) {
        Serial.print("Satellites = ");
        Serial.println(gps.satellites.value());
      }
      else
        Serial.println("Satellites Invalid");
 
    }
 
  }  
  buttonRead=0;
  } 
  //check for the maximum time limit
  else if (LDRvalue<=600 && counter <=x)   //Still Processing
  {
    digitalWrite(bluled,HIGH);
    delay (100);
    digitalWrite(bluled,LOW);
    delay(100);
    counter++;
     
  }
  else  //negative case detection
  {
    digitalWrite (sensorled,LOW);
    digitalWrite (grnled,HIGH);
    counter=0;
    buttonRead=0;
  }
  }
}


