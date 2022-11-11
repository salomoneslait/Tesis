#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <HTTPClient.h>
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

  // Conexión WIFI
  conexionWifi();

  //API
  rutaLuces();
  rutaTomacorrientes();
  server.begin();

  //homekit
  my_homekit_setup();
}

int espera = 5000;
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
