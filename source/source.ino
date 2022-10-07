#include <WiFi.h>
#include <Adafruit_NeoPixel.h>


//-----------------Definicion neopixel
int PIN = 23;
int NUMPIXELS = 6;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t rojo = pixels.Color(250, 0, 0);
uint32_t verde = pixels.Color(0, 150, 0);
uint32_t azul = pixels.Color(0, 0, 150);
uint32_t apagado = pixels.Color(0, 0, 0);

int red, green, blue;

uint32_t* colorA[3];

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "Eslait";
const char* password = "Anadieseleda";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

String header; // Variable para guardar el HTTP request

String estadoSalida = "off";

const int salida = 2;

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE html>"
"<html lang=''en'>"
"    <head>"
"        <meta charset='UTF-8'/>"
"        <meta http-equiv='X-UA-Compatible' content='IE=edge' />"
"        <meta name='viewport' content='width=device-width, initial-scale=1.0' />"
"        <title>Home-Cloud IoT</title>"
"        <!-----Bootstrap-->"
"        <link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.0/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-gH2yIJqKdNHPEq0n4Mqa/HGKIhSkIHeL5AyhkYV8i59U5AR6csBvApHHNl/vI1Bx' crossorigin='anonymous'>"
"    </head>"
"    <body>"
"        <nav class='navbar navbar-light navbar-expand bg-light navigation-clean'>"
"            <div class='container'>"
"                <a class='navbar-brand' href='#'>HOME</a>"
"                <button class='navbar-toggler' data-bs-tooggle='collapse' data-bs-target='#navcol-1'></button>"
"                <div id='navcol-1' class='collapse navbar-collapse'> </div>"
"                <div class='btn-toolbar'>"
"                    <div class='btn-group' role='group'>"
"                        <button class='btn btn-light' type='button'>Cocina</button>"
"                        <button class='btn btn-light' type='button'>Baño</button>"
"                        <button class='btn btn-light' type='button'>Garaje</button>"
"                        <button class='btn btn-light' type='button'>Habitacion</button>"
"                        <button class='btn btn-light' type='button'>Sala</button>"
"                    </div>"
"                </div>"
"            </div>"
"        </nav>"
"        <div class='container my-5'>"
"            <center><h1 class='display-5'>Control de sensores</h1></center>"
"            <div class='row'>"
"                <div class='col-sm-3'>"
"                  <div class='card'>"
"                    <div class='card-body'>"
"                      <center><h5 class='card-title'>Gas</h5>"
"                        <div class='alert alert-danger' role='alert'>"
"                            Peligro"
"                        </div>"
"                      </center>"
"                    </div>"
"                  </div>"
"                </div>"
"                <div class='col-sm-3'>"
"                  <div class='card'>"
"                    <div class='card-body'>"
"                      <center><h5 class='card-title'>Garaje</h5> "
"                        <div class='alert alert-warning' role='alert'>"
"                           Abierto"
"                        </div>"
"                      </center>"
"                    </div>"
"                  </div>"
"                </div>"
"                <div class='col-sm-3'>"
"                    <div class='card'>"
"                      <div class='card-body'>"
"                        <center><h5 class='card-title'>Puerta principal</h5>"
"                        <div class='alert alert-success' role='alert'>"
"                            Seguro"
"                        </div>"
"                        </center>"
"                      </div>"
"                    </div>"
"                </div>"
"                <div class='col-sm-3'>"
"                    <div class='card'>"
"                      <div class='card-body'>"
"                        <center><h5 class='card-title'>Sensor de presencia</h5>"
"                        <div class='alert alert-success' role='alert'>"
"                            Seguro"
"                        </div>"
"                        </center>"
"                      </div>"
"                    </div>"
"                </div>"
"            </div>"
"        </div>"
"        <div class='container my-5'>"
"            <center><h1 class='display-5'>Control de iluminacion</h1></center>"
"            <div class='row'>"
"                <form class='row g-3' action='' id='homeform'>                    "
"                        <div class='col-sm-4'>"
"                            <select name='home' form = 'homeform' class='form-select form-select-lg mb-3' aria-label='.form-select-lg example'>                                "
"                                <option selected>Seleccione el espacio que desea iluminar</option>"
"                                <option value='1'>Habitacion</option>"
"                                <option value='2'>Garaje</option>"
"                                <option value='3'>Baño</option>"
"                                <option value='4'>Cocina</option>"
"                                <option value='5'>Sala</option>"
"                            </select>"
"                        </div>"
"                        <div class='col-sm-3'>"
"                            <input name='range' form = 'homeform' type='range' class='form-range' min='0' max='100' id='customRange'>"
"                        </div>"
"                        <div class='col-sm-1'>                           "
"                            <input name='Color' form = 'homeform' type='color' class='form-control form-control-color' id='ColorInput' value='#04087c' title='Seleccione tono'>"
"                        </div> "
"                        <div class='col-sm-2'>  "
"                            <div class='form-check form-switch'>"
"                                <input name='set' form='homeform' class='form-check-input' type='checkbox' role='switch' id='flexSwitchCheckDefault'>"
"                                <label class='form-check-label' for='flexSwitchCheckDefault'>OFF/ON</label>"
"                            </div>"
"                        </div>"
"                    <div class='col-sm-2'>"
"                      <input type='submit' class='btn btn-primary mb-3'>"
"                    </div>"
"                </form>  "
"            </div>                     "
"        </div>"
""
"        <div class='container my-5'>"
"            <center><h1 class='display-5'>Control de tomacorrientes</h1></center>"
"            <div class='row'>"
"                <form class='row g-3' action='' id='tomaform'> "
"                    <div class='col-sm-10'>"
"                        <select name='toma' form= 'tomaform' class='form-select form-select-lg mb-3' aria-label='.form-select-lg example'>"
"                            <option selected>Seleccione el tomacorriente que desea encender</option>"
"                            <option value='Habitacion'>Habitacion</option>"
"                            <option value='Garaje'>Garaje</option>"
"                            <option value='Baño'>Baño</option>"
"                            <option value='Cocina'>Cocina</option>"
"                            <option value='Sala'>Sala</option>"
"                        </select>"
"                    </div>"
"                    <div class='col-sm-1'>"
"                        <input type='submit' class='btn btn-primary'> "
"                    </div>       "
"                </form>"
"                                  "
"            </div>"
"        </div>"
""
"        "
"    </body>"
"</html>";




//---------------------------SETUP--------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("");

  pinMode(salida, OUTPUT);
  digitalWrite(salida, LOW);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    // para usar con ip fija
    IPAddress ip(192, 168, 1, 180);
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

  pixels.begin();
  pixels.clear();

}

//----------------------------LOOP----------------------------------

void loop() {
  peticion();

  //  int delayval = 50;
  //
  //  for(int i=0;i<NUMPIXELS;i++){
  //    pixels.setPixelColor(i,rojo);
  //    pixels.show();
  //    delay(delayval);
  //  }
  //
  //  for(int i=0;i<NUMPIXELS;i++){
  //    pixels.setPixelColor(i,verde);
  //    pixels.show();
  //    delay(delayval);
  //  }
  //
  //  for(int i=0;i<NUMPIXELS;i++){
  //    pixels.setPixelColor(i,azul);
  //    pixels.show();
  //    delay(delayval);
  //  }


}

void peticion() {
  WiFiClient client = server.available();   // Escucha a los clientes entrantes
  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.\n");          //
    String currentLine = "";                //
    String peticion;
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //            if (header.indexOf("GET /?home=Habitacion&color=R") >= 0) {
            //              Serial.println("GPIO on");
            //              estadoSalida = "on";
            //              digitalWrite(salida, HIGH);
            //              pixels.setPixelColor(1,rojo);
            //              pixels.show();
            //            } else if (header.indexOf("GET /off") >= 0) {
            //              Serial.println("GPIO off");
            //              estadoSalida = "off";
            //              digitalWrite(salida, LOW);
            //              pixels.setPixelColor(1, pixels.Color(0, 0, 0));
            //              pixels.show();
            //            }else if (header.indexOf("GET /?home=Habitacion&color=A") >= 0) {
            //              pixels.setPixelColor(1,azul);
            //              pixels.show();
            //            }else if (header.indexOf("GET /?home=Habitacion&color=V") >= 0) {
            //              pixels.setPixelColor(1,verde);
            //              pixels.show();
            //            }
            //
            //            // Muestra la página web
            client.println(pagina);

            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }

    peticion = header.substring(header.indexOf("GET /?") + 6, header.indexOf(" HTTP/1.1"));
    Serial.println(peticion);

    luces(peticion);

    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void luces(String peticion) {
  //  int str_len = peticion.length() + 1;
  //  char peticionC[str_len];
  //  peticion.toCharArray(peticionC,str_len);
  //  char* homee = NULL;
  //  char* range = NULL;
  //  char* color = NULL;
  //  char* sett = NULL;
  //
  //  homee = strtok(peticionC, "&");
  //  if (homee != NULL) {range = strtok(NULL, "&");}
  //  if (range != NULL) {color = strtok(NULL, "&");}
  //  if (color != NULL) {sett = strtok(NULL, "&");}
  //
  //  Serial.println(homee);
  //  Serial.println(range);
  //  Serial.println(color);
  //  Serial.println(sett);

  String estadoS = peticion.substring(peticion.indexOf("set=") + 4, peticion.length());
  int estado;
  if (estadoS == "on"){
    estado = 1;
  }else if (estadoS =="off"){
    estado = 0;
  }
  //int estado = estadoS.toInt();
  String espacioS = peticion.substring(peticion.indexOf("home=") + 5, peticion.indexOf("&"));
  int espacio = espacioS.toInt();
  String brilloS = peticion.substring(peticion.indexOf("range=") + 6, peticion.indexOf("&Color="));
  int brillo = brilloS.toInt();
  String colorS = peticion.substring(peticion.indexOf("Color=") + 6, peticion.indexOf("&set="));
  int color = colorS.toInt();

  Serial.println(colorS);
  //colorConverter(colorS);
  //getRGB(colorS);
  getRGB(colorS);

  switch (estado) {
    case 0:
      pixels.setPixelColor(espacio, apagado);
      pixels.show();
      break;

    case 1:
//      pixels.setPixelColor(espacio, brillo * 255 / 100, 0, 0);
      pixels.setPixelColor(espacio, red*brillo/100, green*brillo/100, blue*brillo/100);
      pixels.show();
      break;
  }
}

void getRGB(String hexvalue) {
  hexvalue.toUpperCase();

  char c[7];
  hexvalue.toCharArray(c, 8);
  red = hexcolorToInt(c[1], c[2]);
  green = hexcolorToInt(c[3], c[4]);
  blue = hexcolorToInt(c[5], c[6]);
  Serial.print("REED: "); Serial.println(red);
  Serial.print("green: "); Serial.println(green);
  Serial.print("blue: "); Serial.println(blue);
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
