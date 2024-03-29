#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include "variables.h"
#include "funciones.h"
#include "homekit.h"



void setup() {
  
  frontUrl = "34.148.42.100:3000";

  Serial.begin(115200);
  pixels.begin();

  //tomacorrientes
  pinMode(piro, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  
  // Conexión WIFI
  conexionWifi();

  //API
  rutaLuces();
  rutaTomacorrientes();
  rutaGaraje();
  server.begin();

  //homekit
  my_homekit_setup();

  //Puerta
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400);
}

int espera = 1000;
unsigned long tiempoActual = 0;
String magg,piroo,gass;

void loop() {

  my_homekit_loop();
  delay(10);
  
  tiempoActual = millis();
  while(millis() < tiempoActual+espera){}
  
  //sensor 
  SensorMag();
  SensorPiro();
  SensorHumo();

  Enviar();

  
}
