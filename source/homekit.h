#include <Arduino.h>
#include <arduino_homekit_server.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

//#define NEOPIN          23
//#define NUMPIXELS       6
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);


//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c

extern "C" homekit_server_config_t accessory_config;
//habitaicion
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

//garaje
extern "C" homekit_characteristic_t cha_2_on;
extern "C" homekit_characteristic_t cha_2_bright;
extern "C" homekit_characteristic_t cha_2_sat;
extern "C" homekit_characteristic_t cha_2_hue;

extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;

//habitacion
void updateColor()
{
  if(is_on)
  {
   
      if(received_hue && received_sat)
      {
        HSV2RGB(current_hue, current_sat, current_brightness);
        received_hue = false;
        received_sat = false;
      }
      
      int b = map(current_brightness,0, 100,75, 255);
      Serial.println(b);
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(1, pixels.Color(rgb_colors[0]*b/100,rgb_colors[1]*b/100,rgb_colors[2]*b/100));
      pixels.show();

    }
  else if(!is_on) //lamp - switch to off
  {
      Serial.println("is_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(1,0,0,0);
      pixels.show();
  }
}

//garaje
void updateColor2()
{
  if(is_2_on)
  {
   
      if(received_hue_2 && received_sat_2)
      {
        HSV2RGB(current_hue_2, current_sat_2, current_brightness_2);
        received_hue_2 = false;
        received_sat_2 = false;
      }
      
      int b = map(current_brightness,0, 100,75, 255);
      Serial.println(b);
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(2, pixels.Color(rgb_colors[0]*b/100,rgb_colors[1]*b/100,rgb_colors[2]*b/100));
      pixels.show();

    }
  else if(!is_2_on) //lamp - switch to off
  {
      Serial.println("is_2_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(2,0,0,0);
      pixels.show();
  }
}

//habitacion
void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_on.value.bool_value = on; //sync the value

    if(on) {
        is_on = true;
        Serial.println("is_1_on On");
    } else  {
        is_on = false;
        Serial.println("is_1_on Off");
    }

    updateColor();
}
//garaje
void set_on_2(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_2_on.value.bool_value = on; //sync the value

    if(on) {
        is_2_on = true;
        Serial.println("is_2_on On");
    } else  {
        is_2_on = false;
        Serial.println("is_2_on Off");
    }

    updateColor2();
}
//habitacion
void set_hue(const homekit_value_t v) {
    Serial.println("set_hue");
    float hue = v.float_value;
    cha_hue.value.float_value = hue; //sync the value

    current_hue = hue;
    received_hue = true;
    
    updateColor();
}
//garaje
void set_hue_2(const homekit_value_t v) {
    Serial.println("set_hue");
    float hue = v.float_value;
    cha_2_hue.value.float_value = hue; //sync the value

    current_hue_2 = hue;
    received_hue_2 = true;
    
    updateColor2();
}
//habitacion
void set_sat(const homekit_value_t v) {
    Serial.println("set_sat");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat = sat;
    received_sat = true;
    
    updateColor();
}
//garaje
void set_sat_2(const homekit_value_t v) {
    Serial.println("set_sat");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat_2 = sat;
    received_sat_2 = true;
    
    updateColor2();
}
//habitacion
void set_bright(const homekit_value_t v) {
    Serial.println("set_bright");
    int bright = v.int_value;
    cha_bright.value.int_value = bright; //sync the value

    current_brightness = bright;

    updateColor();
}
//garaje
void set_bright_2(const homekit_value_t v) {
    Serial.println("set_bright");
    int bright = v.int_value;
    cha_2_bright.value.int_value = bright; //sync the value

    current_brightness_2 = bright;

    updateColor2();
}


void my_homekit_setup() {
  
//  pixels.begin(); 
//  for(int i = 0; i < NUMPIXELS; i++)
//  {
//    pixels.setPixelColor(i, 0, 0, 0);
//  }
//  pixels.show();
//  delay(1000);
  rgb_colors[0] = 255;
  rgb_colors[1] = 255;
  rgb_colors[2] = 255;

  
  //habitacion
  cha_on.setter = set_on;
  cha_bright.setter = set_bright;
  cha_sat.setter = set_sat;
  cha_hue.setter = set_hue;
  
  //garaje
  cha_2_on.setter = set_on_2;
  cha_2_bright.setter = set_bright_2;
  cha_2_sat.setter = set_sat_2;
  cha_2_hue.setter = set_hue_2;
  
  arduino_homekit_setup(&accessory_config);

}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}
