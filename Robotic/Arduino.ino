                                                                                   // Robotic System
                                                                                
#define BLYNK_TEMPLATE_ID " "
#define BLYNK_TEMPLATE_NAME " "
#define BLYNK_AUTH_TOKEN " " 

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
//Motor PINs
#define ENA 16
#define IN1 5
#define IN2 4
#define IN3 0
#define IN4 2
#define ENB 15

#define switch 13

#define S1 12
#define S2 14 
 
bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed;

char auth[] = " ";         //Enter your Blynk auth token
char ssid[] = " ";                                     //Enter your WIFI name
char pass[] = " ";                                 //Enter your WIFI passowrd

 
void setup() 
{
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  Blynk.begin(auth, ssid, pass);
}
 
BLYNK_WRITE(V0)
{
  forward = param.asInt();
}
 
BLYNK_WRITE(V1) 
{
  backward = param.asInt();
}
 
BLYNK_WRITE(V2) 
{
  left = param.asInt();
}
 
BLYNK_WRITE(V3) 
{
  right = param.asInt();
}
 
BLYNK_WRITE(V4) 
{
  Speed = param.asInt();
}
 
void smartcar() 
{
  if (forward == 1) 
  {
    carforward();
    Serial.println("carforward");
  } 
  else if (backward == 1) 
  {
    carbackward();
    Serial.println("carbackward");
  } 
  else if (left == 1) 
  {
    carturnleft();
    Serial.println("carfleft");
  } 
  else if (right == 1) 
  {
    carturnright();
    Serial.println("carright");
  }
  else if (forward == 0 && backward == 0 && left == 0 && right == 0) 
  {
    carStop();
    Serial.println("carstop");
  }
}

void automode()
{
  if(digitalRead(S1)==HIGH  && digitalRead(S2)==HIGH)
    {
      carforward();
    }
    if(digitalRead(S1)==HIGH  && digitalRead(S2)==LOW)
    {
      carturnright();
    }
    if(digitalRead(S1)==LOW  && digitalRead(S2)==HIGH)
    {
      carturnleft();
    }
    if(digitalRead(S1)==LOW  && digitalRead(S2)==LOW)
    {
     carStop();
    }
}

void loop() 
{
  if(digitalRead(switch)== 1)
  {
   Serial.println("Auto Mode");
   automode();
  }
  else if(digitalRead(switch)== 0)
  {
    Serial.println("Manual Mode");
    smartcar();
  }
   Blynk.run();
}
 
 
void carforward() 
{
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() 
{
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() 
{
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() 
{
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}