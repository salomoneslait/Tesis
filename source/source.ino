#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>

int PIN = 23;
int NUMPIXELS = 6;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "Eslait";
const char* password =  "Anadieseleda";

String peticion;
int red, green, blue;

AsyncWebServer server(80);
 
void setup() {
  Serial.begin(115200);
  pixels.begin();
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
  server.on(
    "/luces",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
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

      String estadoS = peticion.substring(peticion.indexOf("encendido") + 12, peticion.length()-1);
      int estado;
      if (estadoS == "on"){
        estado = 1;
      }else if (estadoS =="off"){
        estado = 0;
      }
      //"habitacion":"1","brillo":"50","color":"#04087c","encendido":"on"
      
      String espacioS = peticion.substring(peticion.indexOf("habitacion") + 13, peticion.indexOf("habitacion") + 14);
      int espacio = espacioS.toInt();
      String brilloS = peticion.substring(peticion.indexOf("brillo") + 9, peticion.indexOf("color") - 3);
      int brillo = brilloS.toInt();
      String colorS = peticion.substring(peticion.indexOf("color") + 8, peticion.indexOf("encendido")-3);
      getRGB(colorS);

      switch (estado) {
      case 0:
        pixels.setPixelColor(espacio, 0,0,0);
        pixels.show();
        break;
  
      case 1:
  //      pixels.setPixelColor(espacio, brillo * 255 / 100, 0, 0);
        pixels.setPixelColor(espacio, red*brillo/100, green*brillo/100, blue*brillo/100);
        pixels.show();
        break;
    }
            
      request->send(200);
  });
 
  server.begin();
}

String converter(uint8_t *str){
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
 
void loop() {
  }