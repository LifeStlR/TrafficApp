#define BLYNK_TEMPLATE_ID           "TMPL6L-_hoO7Z"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "JsrB-2tt2H1kc980A_7NDSEr5Otbw_xa"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 21
#define DIO 22
TM1637Display display(CLK, DIO);
const int button = 35;
int cp,test;

char ssid[] = "Galaxy_izza";
char pass[] = "MupengKao";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

// create a server with port 80 (default web port)
WiFiServer server(80);

// set the servo
Servo servo;
int pos = 00;
#define crossbar 15

// set the sensor & led
int MotionSensor = 5;
int MotionLed = 23;

//Timer
#define timeSeconds 10
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  digitalWrite(MotionLed, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

//Ultrasonic Sensor
int trigPin = 18;
int echoPin = 19;

//Traffic 1
int RedLed1 = 12;
int YellowLed1 = 13;
int GreenLed1 = 14;

//Traffic 2
int RedLed2 = 25;
int YellowLed2 = 26;
int GreenLed2 = 27;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
timer.setInterval(1000L, myTimerEvent);

display.setBrightness(0x0f);

//Traffic 1
pinMode(RedLed1,OUTPUT);
pinMode(YellowLed1,OUTPUT);
pinMode(GreenLed1,OUTPUT);
digitalWrite(RedLed1,LOW);
digitalWrite(YellowLed1,LOW);
digitalWrite(GreenLed1,LOW);
//Traffic 2
pinMode(RedLed2,OUTPUT);
pinMode(YellowLed2,OUTPUT);
pinMode(GreenLed2,OUTPUT);
digitalWrite(RedLed2,LOW);
digitalWrite(YellowLed2,LOW);
digitalWrite(GreenLed2,LOW);
//Servo
servo.setPeriodHertz(50); // PWM frequency for servo (sg90)
servo.attach(crossbar, 500, 2400); // Minimum and maximum pulse width (in µs) to go from 0° to 180°
//MotionLED
pinMode(MotionSensor, INPUT_PULLUP);
pinMode(MotionLed,OUTPUT);
digitalWrite(MotionLed,LOW);
attachInterrupt(digitalPinToInterrupt(MotionSensor), detectsMovement, RISING);
//Ultrasonic Sensor
pinMode(echoPin, INPUT);
pinMode(trigPin, OUTPUT);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void loop() {
  // put your main code here, to run repeatedly:
Blynk.run();
timer.run();
}

BLYNK_WRITE(V1){ // this command is listening when something is written to V1
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  while (true){
   // do something when button is pressed;
   Serial.println("Pressed");
  test=1;
  cp=6; // initialize the timer to 0
    while(test==1 && cp>0) {
      digitalWrite(RedLed1,LOW);
      digitalWrite(GreenLed1,HIGH);
      digitalWrite(RedLed2,HIGH);
      digitalWrite(YellowLed1,LOW);
      digitalWrite(YellowLed2,LOW);
      cp--;// decrement the counter
      display.showNumberDec(cp);
      delay(1000);
    }
      if(cp==0) {
        digitalWrite(GreenLed1,LOW);
        digitalWrite(YellowLed1,HIGH);
        digitalWrite(YellowLed2,HIGH);
        digitalWrite(RedLed2,LOW);
        delay(3000);
        test=0;
        cp=6;
      }
    while(test==0 && cp>0) {
      digitalWrite(YellowLed1,LOW);
      digitalWrite(YellowLed2,LOW);
      digitalWrite(GreenLed2,HIGH);
      digitalWrite(RedLed1,HIGH);
      cp--;// decrement the counter
      display.showNumberDec(cp);
      delay(1000);
    }
      if(cp==0) {
        digitalWrite(GreenLed2,LOW);
        digitalWrite(YellowLed1,HIGH);
        digitalWrite(YellowLed2,HIGH);
        digitalWrite(RedLed1,LOW);
        delay(2000);
        test=1;
        cp=6;
      }
}
}

BLYNK_WRITE(V4){ // this command is listening when something is written to V1
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  if (pinValue == 1){
   // do something when button is pressed;
   Serial.println("V4");
   while(true){
     long duration,cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(250);
   test=3;
   cp=6;
   // Current time
  now = millis();
  if((digitalRead(MotionLed) == HIGH) && (motion == false)) {
    Serial.println("MOTION DETECTED!!!");
    motion = true;
  }
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*50))) {
    Serial.println("Motion stopped...");
    digitalWrite(MotionLed, LOW);
    startTimer = false;
    motion = false;
  }
   while (test==3 && cp>0 && motion==true){
    digitalWrite(RedLed1,HIGH);
    digitalWrite(RedLed2,HIGH);
    digitalWrite(YellowLed1,LOW);
    digitalWrite(YellowLed2,LOW);
    cp--;
    display.showNumberDec(cp);
    delay(1000);
    Serial.println(test);
   }
   if(cp==0) {
        digitalWrite(GreenLed2,LOW);
        digitalWrite(YellowLed1,HIGH);
        digitalWrite(YellowLed2,HIGH);
        digitalWrite(RedLed1,LOW);
        digitalWrite(RedLed2,LOW);
        delay(2000);
        test=1;
        cp=5;
        display.showNumberDec(cp);
        Serial.println(test);
      }
    while(test==1&&cp>0){
      digitalWrite(RedLed1,LOW);
      digitalWrite(GreenLed1,HIGH);
      digitalWrite(RedLed2,HIGH);
      digitalWrite(YellowLed1,LOW);
      digitalWrite(YellowLed2,LOW);
      cp--;// decrement the counter
      display.showNumberDec(cp);
      delay(1000);
      Serial.println(test);
    }
    if(cp==0) {
        digitalWrite(GreenLed1,LOW);
        digitalWrite(YellowLed1,HIGH);
        digitalWrite(YellowLed2,HIGH);
        digitalWrite(RedLed2,LOW);
        delay(3000);
        motion=false;
        test=0;
        cp=6;
        Serial.println(test);
      }
    while(test==0 && cp>0) {
      digitalWrite(YellowLed1,LOW);
      digitalWrite(YellowLed2,LOW);
      digitalWrite(GreenLed2,HIGH);
      digitalWrite(RedLed1,HIGH);
      cp--;// decrement the counter
      display.showNumberDec(cp);
      delay(1000);
      Serial.println(test);
    }
  if(cp==0) {
        digitalWrite(GreenLed2,LOW);
        digitalWrite(YellowLed1,HIGH);
        digitalWrite(YellowLed2,HIGH);
        digitalWrite(RedLed1,LOW);
        delay(2000);
        test=2;
        cp=6;
        Serial.println(test);
      }
    while(test==2&&cm<=60){
      digitalWrite(RedLed1,HIGH);
      digitalWrite(RedLed2,HIGH);
      digitalWrite(YellowLed1,LOW);
      digitalWrite(YellowLed2,LOW);
      test=3;
      //Servo
      for (int pos = 00; pos <= 90; pos += 10) { //rotation from 0 to 90°
      servo.write(pos);
      delay(5);
      }
      while (test==3&&cm<=20){
      // Rotation from 180° to 0
      for (int pos = 180; pos >= 0; pos -= 10) {
      servo.write(pos);
      delay(5);
      digitalWrite(RedLed1,LOW);
      digitalWrite(RedLed2,LOW);
      digitalWrite(YellowLed1,HIGH);
      digitalWrite(YellowLed2,HIGH);
      delay(3000);
      test=1;
      cp=5;
    }
      }
    }
  }
  }
}