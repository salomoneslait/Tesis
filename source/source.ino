#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

int PIN = 23;
int NUMPIXELS = 5;
int contconexion = 0;
uint16_t valor_puerta=0, valor_presencia=0;


#define hall 35      //Pin controlador de efecto hall
#define piro 34      //Pin controlador de sensor de presencia
#define Relay1 33     //Pin controlador Relay 1
#define Relay2 25     //Pin controlador Relay 2
#define Relay3 26     //Pin controlador Relay 3
#define Relay4 27     //Pin controlador Relay 4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "Sebastian";
const char* password =  "63275999v";

String peticion;
int red, green, blue;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pixels.begin();

  //tomacorrientes
  pinMode(piro, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    IPAddress ip(192, 168, 20, 78);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    server.begin(); // iniciamos el servidor
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
  }

  server.on(
    "/luces",
    HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    for (size_t i = 0; i < len; i++) {
      //Serial.write(data[i]);
    }
    Serial.println("peticion recibida");
    peticion = converter(data);
    peticion = peticion.substring(peticion.indexOf("{") + 1, peticion.indexOf("}"));
    Serial.println(peticion);

    // partir la peticion en variables

    String estadoS = peticion.substring(peticion.indexOf("encendido") + 12, peticion.indexOf("brillo") - 3);
    int estado;
    if (estadoS == "ON") {
      estado = 1;
    } else if (estadoS == "OFF") {
      estado = 0;
    }
    //"habitacion":"1","encendido":"ON","brillo":"50","color":"#04087c"

    String espacioS = peticion.substring(peticion.indexOf("habitacion") + 13, peticion.indexOf("habitacion") + 14);
    int espacio = espacioS.toInt();
    String brilloS = peticion.substring(peticion.indexOf("brillo") + 9, peticion.indexOf("color") - 3);
    int brillo = brilloS.toInt();
    String colorS = peticion.substring(peticion.indexOf("color") + 8, peticion.length() - 1);
    getRGB(colorS);

    switch (estado) {
      case 0:
        pixels.setPixelColor(espacio, 0, 0, 0);
        pixels.show();
        break;

      case 1:
        //      pixels.setPixelColor(espacio, brillo * 255 / 100, 0, 0);
        pixels.setPixelColor(espacio, red * brillo / 100, green * brillo / 100, blue * brillo / 100);
        pixels.show();
        break;
    }

    request->send(200);
  });

  server.on(
    "/tomacorrientes",
    HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    for (size_t i = 0; i < len; i++) {
      //Serial.write(data[i]);
    }
    Serial.println("peticion recibida tomas");
    peticion = converter(data);
    peticion = peticion.substring(peticion.indexOf("{") + 1, peticion.indexOf("}"));
    Serial.println(peticion);

    // partir la peticion en variables
    //"habitacion":"1","encendido":"OFF"

    String espacioS = peticion.substring(peticion.indexOf("habitacion") + 13, peticion.indexOf("habitacion") + 14);
    int espacio = espacioS.toInt();
    String estadoS = peticion.substring(peticion.indexOf("encendido") + 12, peticion.length() - 1);
    bool estado;
    if (estadoS == "ON") {
      estado = LOW;
    } else if (estadoS == "OFF") {
      estado = HIGH;
    }

    switch (espacio){
      case 1:
        digitalWrite(Relay1,estado);
        Serial.println("Relay1");
        break;
      case 2:
        digitalWrite(Relay2,estado);
        Serial.println("Relay2");
        break;
      case 3:
        digitalWrite(Relay3,estado);
        Serial.println("Relay3");
        break;
      case 4:
        digitalWrite(Relay4,estado);
        Serial.println("Relay4");
        break;
    }

    request->send(200);
  });

  server.begin();
}

void loop() {
  leerSensorMag();
    Serial.println(valor_puerta);
    delay(1000);
  if(valor_puerta<500){
    Serial.println("Puerta cerrada");
  }else{
    Serial.println("Puerta abierta");
  }
  leerSensorPiro();
    Serial.println(valor_presencia);
  if(valor_presencia== 1){
    Serial.println("Presencia detectada");
    valor_presencia= 0;
  }else{
    Serial.println("Sin presencia");
  }
}

void leerSensorMag(){
  valor_puerta = analogRead(hall);
}

void leerSensorPiro(){
  valor_presencia = digitalRead(piro);
}

String converter(uint8_t *str) {
  return String((char *)str);
}

void getRGB(String hexvalue) {
  hexvalue.toUpperCase();

  char c[7];
  hexvalue.toCharArray(c, 8);
  red = hexcolorToInt(c[1], c[2]);
  green = hexcolorToInt(c[3], c[4]);
  blue = hexcolorToInt(c[5], c[6]);
  //  Serial.print("REED: "); Serial.println(red);
  //  Serial.print("green: "); Serial.println(green);
  //  Serial.print("blue: "); Serial.println(blue);
}

int hexcolorToInt(char upper, char lower)
{
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  // Serial.println(uVal+lVal);
  return uVal + lVal;
}
