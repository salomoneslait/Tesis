#include <Adafruit_NeoPixel.h>

int PIN = 23;
int NUMPIXELS = 5;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  pixels.begin();
}

void loop(){
  uint32_t rojo = pixels.Color(150,0,0);
  uint32_t verde = pixels.Color(0,150,0);
  uint32_t azul = pixels.Color(0,0,150);

  int delayval = 50;

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,rojo);
    pixels.show();
    delay(delayval);
  }

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,verde);
    pixels.show();
    delay(delayval);
  }

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,azul);
    pixels.show();
    delay(delayval);
  }
}
