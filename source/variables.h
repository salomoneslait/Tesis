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
String url;
String frontUrl;
float temperatura;
String var;

//homekit
bool received_sat = false;
bool received_hue = false;

bool is_on = false;
float current_brightness =  100.0;
float current_sat = 0.0;
float current_hue = 0.0;

//garaje
bool received_sat_2 = false;
bool received_hue_2 = false;
bool is_2_on = false;
float current_brightness_2 =  100.0;
float current_sat_2 = 0.0;
float current_hue_2 = 0.0;

int rgb_colors[3];
