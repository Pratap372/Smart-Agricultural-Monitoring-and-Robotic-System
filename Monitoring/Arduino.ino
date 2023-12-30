                                                               // IOT Based Smart Agriculture Monitoring System
#define BLYNK_TEMPLATE_ID " "
#define BLYNK_TEMPLATE_NAME " "
#define BLYNK_AUTH_TOKEN " "

#define BLYNK_PRINT Serial
#include <SPI.h>                                       //Including libraries                                    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
BlynkTimer timer;

char auth[] = " ";       //Authentication code sent by Blynk
char ssid[] = " ";                             //WiFi SSID
char pass[] = " ";                               //WiFi Password

#define SCREEN_WIDTH 128                                // OLED display width, in pixels
#define SCREEN_HEIGHT 64                                // OLED display height, in pixels
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
//Pin defining for dht22 (D4)
DHT dht(DHTPIN, DHT22);
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define buzzer 12
#define rainPin 13                                 //Pin defining for Rain sensore (D7)
int rainState = 0;
int lastRainState = 0;
const int AirValue = 1024;   
const int WaterValue = 365; 
const int SensorPin = A0;                               //Analog pin(A0) for Soil Moisture
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

#define IN1 14
#define IN2 16
#define IN3 15

bool push = 0;
bool pull = 0;
bool Switch = 0;
int crankoff;


#define pirPin 0
int pirValue;
int pinValue;

BLYNK_WRITE(V4)
{
  pinValue = param.asInt();
}

BLYNK_WRITE(V1) 
{
  push = param.asInt();

}

BLYNK_WRITE(V2) 
{
  pull = param.asInt();
}

BLYNK_WRITE(V6) 
{
  Switch = param.asInt();
}


void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);            //initializing OLED(128x64)
  display.clearDisplay();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  sensors.begin();
  dht.begin();
}

void getPirValue(void)        //Get PIR Data
{
  pirValue = digitalRead(pirPin);
  if (pirValue == HIGH)
  {
    digitalWrite(buzzer, HIGH);
    Serial.println("Motion detected");
    Blynk.logEvent("motion_detection","Motion detected in your farm");
  }
  else if (pirValue == LOW)
  {
    digitalWrite(buzzer, LOW);
    Serial.println("No Motion");
   
  }
}

void getPushValue( )        
{
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     Serial.println("Crank is pushed");
     Blynk.logEvent("Motion detected in your farm") ;
 }

void getPullValue( )        
{
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     Serial.println("Crank is pulled");
    
}

void CrankOff()
{
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
}

void MotorOn()
{
  if(Switch == 1) 
 {
  digitalWrite(IN3, HIGH);
  WidgetLED PumpLed(V0);
  PumpLed.on();
 }
 else if(Switch == 0)
 {
  digitalWrite(IN3, LOW);
  WidgetLED PumpLed(V0);
  PumpLed.off();
 }
}

void MotorOff()
{
  if(Switch == 0)
 {
  digitalWrite(IN3, LOW);
  WidgetLED PumpLed(V0);
  PumpLed.off();
 }
  else if(Switch == 1)
 {
  digitalWrite(IN3, LOW);
  WidgetLED PumpLed(V0);
  PumpLed.off();
 }
 
}

void loop() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  
  Serial.print("Soil Temperature: ");
  Serial.println(temp);
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity: ");
  Serial.println(h);

  Blynk.virtualWrite(V7, t);                              //V7 is for Temperature
  Blynk.virtualWrite(V5, h);                              //V5 is for Humidity 
  Blynk.virtualWrite(V3, soilmoisturepercent);            //V3 Soil Moisture sensor
  

  if(push == 1)
  { 
  getPushValue();
  Serial.println("Crank is pushed");
  }
  else if(pull == 1)
  {
  getPullValue();
  Serial.println("Crank is pulled");
  }
  else if(push == 0 && pull == 0)
  {
  CrankOff();
  Serial.println("Crank is off");
  }


  soilMoistureValue = analogRead(SensorPin);              
  Serial.println(soilMoistureValue);

  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  
  if (soilmoisturepercent > 100)
  {
    Serial.println("100 %");
    display.clearDisplay();

    // Display Water precentage of Soil
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("WP of Soil: ");
    display.print("100");
    display.print(" %");

    // Display relative Temperature of Air
    display.setCursor(0, 20);
    display.print("Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // Display relative Humidity of Air
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.print(h);
    display.print(" %");

    display.display();
    
  }
  else if (soilmoisturepercent < 0)
  {
    Serial.println("0 %");
    display.clearDisplay();

    // Display Water precentage of Soil
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("WP of Soil: ");
    display.print("0");
    display.print(" %");

    // Display relative Temperature of Air
    display.setCursor(0, 20);
    display.print("Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // Display relative Humidity of Air
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.print(h);
    display.print(" %");

    display.display();
  }
  else if (soilmoisturepercent >= 0 && soilmoisturepercent <= 100)
  {
    Serial.print(soilmoisturepercent);
    Serial.println("%");
    display.clearDisplay();

    // Display Water precentage of Soil
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.print("WP of Soil: ");
    display.print(soilmoisturepercent);
    display.print(" %");

    // Display relative Temperature of Air
    display.setCursor(0, 20);
    display.print("Temp: ");
    display.print(t);
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");

    // Display relative Humidity of Air
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.print(h);
    display.print(" %");

    display.display();
  }

  if (soilmoisturepercent >= 0 && soilmoisturepercent < 30)
  {
    MotorOn();
    Serial.println("needs water, send notification");
    //Blynk.logEvent("Plants need water...") ;                                       //Send notification
    Serial.println("Motor is ON");
  }
  else if (soilmoisturepercent > 30 && soilmoisturepercent <= 100)
  {
    MotorOff();
    Serial.println("Soil Moisture level looks good...");
    Serial.println("Motor is OFF");
  }


 
  rainState = digitalRead(rainPin);
  Serial.println(rainState);
  if (rainState == 0 && lastRainState == 0)
  {
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.clearDisplay();
    display.setCursor(0, 50);
    display.print("Raining");
    Serial.println("It's Raining outside!");
    Blynk.logEvent("raining","It's Raining Outside..."); 
    lastRainState = 1;
    delay(1000);
    //send notification
  }
  else if (rainState == 0 && lastRainState == 1) 
  {
    delay(1000);
  }
  else 
  {
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.clearDisplay();
    display.setCursor(0, 50);
    display.print("No Rains");
    Serial.println("No Rains...");
    lastRainState = 0;
    delay(1000);
  }
  
 
   if (pinValue == 1)
  {
    getPirValue();
  }
   else if(pinValue == 0)
  {
   Serial.println("PIR is Off");
  }

  Blynk.run();
  timer.run();
}



 