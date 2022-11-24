AsyncWebServer server(80);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void conexionWifi(){
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    IPAddress ip(192, 168, 1, 180);
    IPAddress gateway(192, 168, 1, 254);
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
}

String converter(uint8_t *str) {
  return String((char *)str);
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

void getRGB(String hexvalue) {
  hexvalue.toUpperCase();

  char c[7];
  hexvalue.toCharArray(c, 8);
  red = hexcolorToInt(c[1], c[2]);
  green = hexcolorToInt(c[3], c[4]);
  blue = hexcolorToInt(c[5], c[6]);
}

void rutaLuces(){
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
}

void rutaTomacorrientes(){
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
}

void rutaGaraje(){
  server.on(
    "/garaje",
    HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  NULL,
  [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    for (size_t i = 0; i < len; i++) {
      //Serial.write(data[i]);
    }
    Serial.println("peticion recibida puerta");
    peticion = converter(data);
    // encendido: 'ON'
    peticion = peticion.substring(peticion.indexOf("{encendido: ") + 15, peticion.indexOf("}")-1);

    if (peticion == "ON"){
      myservo.write(0);
      Serial.println("Abriendo garaje");
    }
    if (peticion == "OFF"){
      myservo.write(75);
      Serial.println("Cerrando garaje");
    }
       
    request->send(200);
  });

}

void Enviar(){
  HTTPClient http;
  http.addHeader("Content-Type", "text/plain");
  //url = "http://192.168.15.32/Enviar?Temperatura="+(String)temperatura+"&Senha=admin";//IP NodeJS
  url = "http://"+frontUrl+"/Enviar?mag="+sensormag+"&piro="+presencia+"&gas="+sensorHumo;//IP NodeJS

  if (url == auxURL){
    Serial.println(" estado de los sensores sin cambio");
  }
  else{ 
    auxURL = url;
    Serial.println(url);
    http.begin(url.c_str());
    httpResponseCode = http.POST("");
    delay(300);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }else {
      Serial.println("sin respuesta");
    }
    http.end();
  }
  
}

//---Homekit
void HSV2RGB(int R, float h,float s,float v) {

  int i;
  float m, n, f;

  s/=100;
  v/=100;

  if(s==0){
    switch(R){
      case 0: //habitacion
        rgb_colors[0]=rgb_colors[1]=rgb_colors[2]=round(v*255);
      break;

      case 1: //garaje
        rgb_colors_2[0]=rgb_colors_2[1]=rgb_colors_2[2]=round(v*255);
      break;

      case 2: //baño
        rgb_colors_3[0]=rgb_colors_3[1]=rgb_colors_3[2]=round(v*255);
      break;

      case 3: //cocina
        rgb_colors_4[0]=rgb_colors_4[1]=rgb_colors_4[2]=round(v*255);
      break;

      case 4: //sala
        rgb_colors_5[0]=rgb_colors_5[1]=rgb_colors_5[2]=round(v*255);
      break;
    }
    return;
  }

  h/=60;
  i=floor(h);
  f=h-i;

  if(!(i&1)){
    f=1-f;
  }

  m=v*(1-s);
  n=v*(1-s*f);

  switch (i) {

    case 0: case 6:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(v*255);
          rgb_colors[1]=round(n*255);
          rgb_colors[2]=round(m*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(v*255);
          rgb_colors_2[1]=round(n*255);
          rgb_colors_2[2]=round(m*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(v*255);
          rgb_colors_3[1]=round(n*255);
          rgb_colors_3[2]=round(m*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(v*255);
          rgb_colors_4[1]=round(n*255);
          rgb_colors_4[2]=round(m*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(v*255);
          rgb_colors_5[1]=round(n*255);
          rgb_colors_5[2]=round(m*255);
        break;
    }
      
    break;

    case 1:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(n*255);
          rgb_colors[1]=round(v*255);
          rgb_colors[2]=round(m*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(n*255);
          rgb_colors_2[1]=round(v*255);
          rgb_colors_2[2]=round(m*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(n*255);
          rgb_colors_3[1]=round(v*255);
          rgb_colors_3[2]=round(m*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(n*255);
          rgb_colors_4[1]=round(v*255);
          rgb_colors_4[2]=round(m*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(n*255);
          rgb_colors_5[1]=round(v*255);
          rgb_colors_5[2]=round(m*255);
        break;
    }
      
    break;

    case 2:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(m*255);
          rgb_colors[1]=round(v*255);
          rgb_colors[2]=round(n*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(m*255);
          rgb_colors_2[1]=round(v*255);
          rgb_colors_2[2]=round(n*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(m*255);
          rgb_colors_3[1]=round(v*255);
          rgb_colors_3[2]=round(n*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(m*255);
          rgb_colors_4[1]=round(v*255);
          rgb_colors_4[2]=round(n*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(m*255);
          rgb_colors_5[1]=round(v*255);
          rgb_colors_5[2]=round(n*255);
        break;
    }
          
    break;

    case 3:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(m*255);
          rgb_colors[1]=round(n*255);
          rgb_colors[2]=round(v*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(m*255);
          rgb_colors_2[1]=round(n*255);
          rgb_colors_2[2]=round(v*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(m*255);
          rgb_colors_3[1]=round(n*255);
          rgb_colors_3[2]=round(v*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(m*255);
          rgb_colors_4[1]=round(n*255);
          rgb_colors_4[2]=round(v*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(m*255);
          rgb_colors_5[1]=round(n*255);
          rgb_colors_5[2]=round(v*255);
        break;
    }
      
    break;

    case 4:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(n*255);
          rgb_colors[1]=round(m*255);
          rgb_colors[2]=round(v*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(n*255);
          rgb_colors_2[1]=round(m*255);
          rgb_colors_2[2]=round(v*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(n*255);
          rgb_colors_3[1]=round(m*255);
          rgb_colors_3[2]=round(v*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(n*255);
          rgb_colors_4[1]=round(m*255);
          rgb_colors_4[2]=round(v*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(n*255);
          rgb_colors_5[1]=round(m*255);
          rgb_colors_5[2]=round(v*255);
        break;
    }
      
    break;

    case 5:
      switch(R){
        case 0: //habitacion
          rgb_colors[0]=round(v*255);
          rgb_colors[1]=round(m*255);
          rgb_colors[2]=round(n*255);
        break;
  
        case 1: //garaje
          rgb_colors_2[0]=round(v*255);
          rgb_colors_2[1]=round(m*255);
          rgb_colors_2[2]=round(n*255);
        break;
  
        case 2: //baño
          rgb_colors_3[0]=round(v*255);
          rgb_colors_3[1]=round(m*255);
          rgb_colors_3[2]=round(n*255);
        break;
  
        case 3: //cocina
          rgb_colors_4[0]=round(v*255);
          rgb_colors_4[1]=round(m*255);
          rgb_colors_4[2]=round(n*255);
        break;
  
        case 4: //sala
          rgb_colors_5[0]=round(v*255);
          rgb_colors_5[1]=round(m*255);
          rgb_colors_5[2]=round(n*255);
        break;
    }
      
    break;
  }
}

void SensorMag(){
  valor_puerta = analogRead(hall);
  Serial.print("sensor magnetico:");
  Serial.println(valor_puerta);
  if(valor_puerta<500){
    Serial.println("Puerta cerrada");
    sensormag = "true";
  }else{
    Serial.println("Puerta abierta");
    sensormag = "false";
  }
}

void SensorPiro(){
  valor_presencia = digitalRead(piro);
  if(valor_presencia== 1){
    Serial.println("Presencia detectada");
    valor_presencia= 0;
    presencia = "true";
  }else{
    Serial.println("Sin presencia");
    presencia = "false";
  }
}

void SensorHumo(){
   valor_humo = analogRead(s_mq2);
  if(valor_humo>=1950){
    Serial.println("Hay una fuga de gas");
    sensorHumo = "true";
  }
  else{
    sensorHumo = "false";
  }
 
}
