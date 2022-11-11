// neopixel
int PIN = 23;
int NUMPIXELS = 5;
int contconexion = 0;
int red, green, blue;

//tomacorrientes
#define Relay1 33     //Pin controlador Relay 1
#define Relay2 25     //Pin controlador Relay 2
#define Relay3 26     //Pin controlador Relay 3
#define Relay4 27     //Pin controlador Relay 4

//credenciales wifi
const char* ssid = "Eslait";
const char* password =  "Anadieseleda";

//http
String peticion;
short int httpResponseCode;
String httpRequestData,payload;
String url;
String frontUrl;
float temperatura;
String var;

//homekit

//habitacion
bool received_sat = false;
bool received_hue = false;
bool is_on = false;
float current_brightness =  100.0;
float current_sat = 0.0;
float current_hue = 0.0;
int rgb_colors[3];

//garaje
bool received_sat_2 = false;
bool received_hue_2 = false;
bool is_2_on = false;
float current_brightness_2 =  100.0;
float current_sat_2 = 0.0;
float current_hue_2 = 0.0;
int rgb_colors_2[3];

//baño
bool received_sat_3 = false;
bool received_hue_3 = false;
bool is_3_on = false;
float current_brightness_3 =  100.0;
float current_sat_3 = 0.0;
float current_hue_3 = 0.0;
int rgb_colors_3[3];

//cocina
bool received_sat_4 = false;
bool received_hue_4 = false;
bool is_4_on = false;
float current_brightness_4 =  100.0;
float current_sat_4 = 0.0;
float current_hue_4 = 0.0;
int rgb_colors_4[3];

//sala 
bool received_sat_5 = false;
bool received_hue_5 = false;
bool is_5_on = false;
float current_brightness_5 =  100.0;
float current_sat_5 = 0.0;
float current_hue_5 = 0.0;
int rgb_colors_5[3];




//sensores 

uint16_t valor_puerta=0, valor_presencia=0, valor_humo=0;
String presencia, sensormag, sensorHumo;

#define s_mq2 32      //Sensor de humo y gas MQ2
#define hall 35      //Pin controlador de efecto hall
#define piro 34      //Pin controlador de sensor de presencia
