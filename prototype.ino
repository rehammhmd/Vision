#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//---------------------- OLED display -------------------------------
#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

void setOled() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.print("Connecting");
}

//---------------------- WiFi  -------------------------------
// wifi & location settings , will be set via web in next release  
const char* ssid = "Mav";
const char* password = "BlackBird";
const String latitude="29.966436";
const String longitude="32.553503";
const String name="Hamdy Pharmacy";
const String id="5";
//
const char* host = "script.google.com";
const int httpsPort = 443;



String GAS_ID = "AKfycbxc4uqSHUYNCtLPFEpbZFX9YAFi-0EOaIJrYkeDkQEy7JZ5tsQ";

WiFiClientSecure client;
//String readString;

void sendData(String data)
{
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
/*
  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  */
  String url = "/macros/s/" + GAS_ID + "/exec?"+data;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
    Serial.println("headers received");
    break;
  }
  }
 
  String line = client.readStringUntil('\n');
 /*Error check
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  */
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}

void setWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWifi Conneted"); 
}

int posCount=1; 
//const uint8_t led=7;    //sensor led pin

void setup() {

  Serial.begin(9600); 
  Serial.println("VISION 0.1");
  //pinMode(led,1);
  //digitalWrite(led,1);  //sensor led on
  setOled();
  setWifi();
}

//----------- Sensor --------------------------------------
const int maxCount=5;
const int blue=880;
const int red=700;
int state=0;                   // 0: idle , 1: Check, 2: Positive, 3: Negative
int count=maxCount;
String statStr[]={"Idle","Check - ","Positive","Negative"};
//-----------------------------------------------
void loop() {
  int ldr=analogRead(A0);         // read sensor
  Serial.println(ldr);                    // output sensor value for debug
  if(state==0&&ldr>=blue) state=1;      //if idle and ldr sees blue go to check
  else if(state==1)                               //if check
  {
    if(--count==0) state=3;                   //if counter=0 go to Negative
    else if(ldr<=red) {                           // else if ldr sees red
      state=2;                                       // go to Positive 
      String data="tag=" + id + "&value=1"+"&long="+longitude+"&lat="+latitude+"&name="+name; 
      sendData(data);      // send positive count to google sheet        
    }
  }
  // update display
  display.clearDisplay();
  display.setCursor(0,8);
  display.print(statStr[state]);
  if(state==1) display.print(count);
  display.display();
  // reset state when card removed
  if((state>1)&&(ldr>red)&&(ldr<blue)) 
  {
    count=maxCount;
    state=0;    
  }
  delay(1000);  
}
