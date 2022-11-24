#include <Arduino.h>
#include <arduino_homekit_server.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

//#define NEOPIN          23
//#define NUMPIXELS       6
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);


// access your HomeKit characteristics defined in my_accessory.c

extern "C" homekit_server_config_t accessory_config;

//----- Luces ------
// habitaicion
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;
extern "C" homekit_characteristic_t cha_sat;
extern "C" homekit_characteristic_t cha_hue;

// garaje
extern "C" homekit_characteristic_t cha_2_on;
extern "C" homekit_characteristic_t cha_2_bright;
extern "C" homekit_characteristic_t cha_2_sat;
extern "C" homekit_characteristic_t cha_2_hue;

// baño 
extern "C" homekit_characteristic_t cha_3_on;
extern "C" homekit_characteristic_t cha_3_bright;
extern "C" homekit_characteristic_t cha_3_sat;
extern "C" homekit_characteristic_t cha_3_hue;
// cocina 
extern "C" homekit_characteristic_t cha_4_on;
extern "C" homekit_characteristic_t cha_4_bright;
extern "C" homekit_characteristic_t cha_4_sat;
extern "C" homekit_characteristic_t cha_4_hue;
// sala
extern "C" homekit_characteristic_t cha_5_on;
extern "C" homekit_characteristic_t cha_5_bright;
extern "C" homekit_characteristic_t cha_5_sat;
extern "C" homekit_characteristic_t cha_5_hue;

extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;

//habitacion
void updateColor()
{
  if(is_on)
  {
   
      if(received_hue && received_sat)
      {
        HSV2RGB(0,current_hue, current_sat, current_brightness);
        received_hue = false;
        received_sat = false;
      }
      
      int b = map(current_brightness,0, 100,75, 255);
      Serial.println("update color");
      Serial.println(b);
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(3, pixels.Color(rgb_colors[0]*b/100,rgb_colors[1]*b/100,rgb_colors[2]*b/100));
      pixels.show();

    }
  else if(!is_on) //lamp - switch to off
  {
      Serial.println("is_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(3,0,0,0);
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
        HSV2RGB(1,current_hue_2, current_sat_2, current_brightness_2);
        received_hue_2 = false;
        received_sat_2 = false;
      }
      
      int b = map(current_brightness_2,0, 100,75, 255);
      Serial.println(b);
      Serial.println("update color 2");
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(1, pixels.Color(rgb_colors_2[0]*b/100,rgb_colors_2[1]*b/100,rgb_colors_2[2]*b/100));
      pixels.show();

    }
  else if(!is_2_on) //lamp - switch to off
  {
      Serial.println("is_2_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(1,0,0,0);
      pixels.show();
  }
}

//baño
void updateColor3()
{
  if(is_3_on)
  {
   
      if(received_hue_3 && received_sat_3)
      {
        HSV2RGB(2,current_hue_3, current_sat_3, current_brightness_3);
        received_hue_3 = false;
        received_sat_3 = false;
      }
      
      int b = map(current_brightness_3,0, 100,75, 255);
      Serial.println(b);
      Serial.println("update color 3");
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(2, pixels.Color(rgb_colors_3[0]*b/100,rgb_colors_3[1]*b/100,rgb_colors_3[2]*b/100));
      pixels.show();

    }
  else if(!is_3_on) //lamp - switch to off
  {
      Serial.println("is_3_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(2,0,0,0);
      pixels.show();
  }
}

//cocina
void updateColor4()
{
  if(is_4_on)
  {
   
      if(received_hue_4 && received_sat_4)
      {
        HSV2RGB(3,current_hue_4, current_sat_4, current_brightness_4);
        received_hue_4 = false;
        received_sat_4 = false;
      }
      
      int b = map(current_brightness_4,0, 100,75, 255);
      Serial.println(b);
      Serial.println("update color 4");
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(3, pixels.Color(rgb_colors_4[0]*b/100,rgb_colors_4[1]*b/100,rgb_colors_4[2]*b/100));
      pixels.show();

    }
  else if(!is_4_on) //lamp - switch to off
  {
      Serial.println("is_4_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(3,0,0,0);
      pixels.show();
  }
}

//sala
void updateColor5()
{
  if(is_5_on)
  {
   
      if(received_hue_5 && received_sat_5)
      {
        HSV2RGB(4,current_hue_5, current_sat_5, current_brightness_5);
        received_hue_5 = false;
        received_sat_5 = false;
      }
      
      int b = map(current_brightness_5,0, 100,75, 255);
      Serial.println(b);
      Serial.println("update color 5");
  
      //pixels.setBrightness(b);
      //pixels.setPixelColor(1, pixels.Color(rgb_colors[0],rgb_colors[1],rgb_colors[2]));
      pixels.setPixelColor(4, pixels.Color(rgb_colors_5[0]*b/100,rgb_colors_5[1]*b/100,rgb_colors_5[2]*b/100));
      pixels.show();

    }
  else if(!is_5_on) //lamp - switch to off
  {
      Serial.println("is_5_on == false");
      //pixels.setBrightness(0);
      pixels.setPixelColor(4,0,0,0);
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

//baño
void set_on_3(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_3_on.value.bool_value = on; //sync the value

    if(on) {
        is_3_on = true;
        Serial.println("is_3_on On");
    } else  {
        is_3_on = false;
        Serial.println("is_3_on Off");
    }

    updateColor3();
}

//cocina 
void set_on_4(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_4_on.value.bool_value = on; //sync the value

    if(on) {
        is_4_on = true;
        Serial.println("is_4_on On");
    } else  {
        is_4_on = false;
        Serial.println("is_4_on Off");
    }

    updateColor4();
}

//sala
void set_on_5(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_5_on.value.bool_value = on; //sync the value

    if(on) {
        is_5_on = true;
        Serial.println("is_5_on On");
    } else  {
        is_5_on = false;
        Serial.println("is_5_on Off");
    }

    updateColor5();
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
    Serial.println("set_hue 2");
    float hue = v.float_value;
    cha_2_hue.value.float_value = hue; //sync the value

    current_hue_2 = hue;
    received_hue_2 = true;
    
    updateColor2();
}

//baño
void set_hue_3(const homekit_value_t v) {
    Serial.println("set_hue 3");
    float hue = v.float_value;
    cha_3_hue.value.float_value = hue; //sync the value

    current_hue_3 = hue;
    received_hue_3 = true;
    
    updateColor3();
}

//cocina 
void set_hue_4(const homekit_value_t v) {
    Serial.println("set_hue 4");
    float hue = v.float_value;
    cha_4_hue.value.float_value = hue; //sync the value

    current_hue_4 = hue;
    received_hue_4 = true;
    
    updateColor4();
}

//sala
void set_hue_5(const homekit_value_t v) {
    Serial.println("set_hue 5");
    float hue = v.float_value;
    cha_5_hue.value.float_value = hue; //sync the value

    current_hue_5 = hue;
    received_hue_5 = true;
    
    updateColor5();
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
    Serial.println("set_sat 2");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat_2 = sat;
    received_sat_2 = true;
    
    updateColor2();
}

//baño
void set_sat_3(const homekit_value_t v) {
    Serial.println("set_sat 3");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat_3 = sat;
    received_sat_3 = true;
    
    updateColor3();
}

//cocina
void set_sat_4(const homekit_value_t v) {
    Serial.println("set_sat 4");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat_4 = sat;
    received_sat_4 = true;
    
    updateColor4();
}

//sala
void set_sat_5(const homekit_value_t v) {
    Serial.println("set_sat 5");
    float sat = v.float_value;
    cha_sat.value.float_value = sat; //sync the value

    current_sat_5 = sat;
    received_sat_5 = true;
    
    updateColor5();
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
    Serial.println("set_bright 2");
    int bright = v.int_value;
    cha_2_bright.value.int_value = bright; //sync the value

    current_brightness_2 = bright;

    updateColor2();
}

//baño
void set_bright_3(const homekit_value_t v) {
    Serial.println("set_bright 3");
    int bright = v.int_value;
    cha_3_bright.value.int_value = bright; //sync the value

    current_brightness_3 = bright;

    updateColor3();
}

//cocina
void set_bright_4(const homekit_value_t v) {
    Serial.println("set_bright 4");
    int bright = v.int_value;
    cha_4_bright.value.int_value = bright; //sync the value

    current_brightness_4 = bright;

    updateColor4();
}

//sala
void set_bright_5(const homekit_value_t v) {
    Serial.println("set_bright 5");
    int bright = v.int_value;
    cha_5_bright.value.int_value = bright; //sync the value

    current_brightness_5 = bright;

    updateColor5();
}
void my_homekit_setup() {
  //habitacion
  rgb_colors[0] = 255;
  rgb_colors[1] = 255;
  rgb_colors[2] = 255;

  //garaje
  rgb_colors_2[0] = 255;
  rgb_colors_2[1] = 255;
  rgb_colors_2[2] = 255;

  //baño
  rgb_colors_3[0] = 255;
  rgb_colors_3[1] = 255;
  rgb_colors_3[2] = 255;

  //cocina
  rgb_colors_4[0] = 255;
  rgb_colors_4[1] = 255;
  rgb_colors_4[2] = 255;

  //sala
  rgb_colors_5[0] = 255;
  rgb_colors_5[1] = 255;
  rgb_colors_5[2] = 255;
  
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

  //baño 
  cha_3_on.setter = set_on_3;
  cha_3_bright.setter = set_bright_3;
  cha_3_sat.setter = set_sat_3;
  cha_3_hue.setter = set_hue_3;

  //cocina
  cha_4_on.setter = set_on_4;
  cha_4_bright.setter = set_bright_4;
  cha_4_sat.setter = set_sat_4;
  cha_4_hue.setter = set_hue_4;
  
  //sala
  cha_5_on.setter = set_on_5;
  cha_5_bright.setter = set_bright_5;
  cha_5_sat.setter = set_sat_5;
  cha_5_hue.setter = set_hue_5;
  
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
