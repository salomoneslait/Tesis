#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#include <HTTPClient.h>
#include "variables.h"
#include "funciones.h"
#include "homekit.h"



//--------------------------








//--------------------------

void setup() {
  
  frontUrl = "34.74.122.46:3000";

  Serial.begin(115200);
  pixels.begin();

  //tomacorrientes
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

void loop() {
  var = "true";
  Enviar();
  delay(500);
  var= "false";
  Enviar();
  delay(500);

  my_homekit_loop();
  delay(10);
}
