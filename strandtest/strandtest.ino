
#include <Adafruit_NeoPixel.h>

#define LED_PIN 8
#define STRIP_LENGTH 7
#define NUM_STRIPS 8
#define NUM_PIXELS (NUM_STRIPS * STRIP_LENGTH)

Adafruit_NeoPixel leds((STRIP_LENGTH*NUM_STRIPS), LED_PIN, NEO_GRB + NEO_KHZ800);

void initLEDs(){
  leds.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  leds.show();            // Turn OFF all pixels ASAP
  leds.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void setStrip(int strip, int red, int green, int blue){
  int offset = strip*STRIP_LENGTH;
  for(int i=0; i<STRIP_LENGTH; i++) { 
    leds.setPixelColor(offset + i, red, green, blue);
  }
}

void setPixel(int strip, int pixel, int red, int green, int blue){
  int index = strip*STRIP_LENGTH + pixel;
  leds.setPixelColor(index, red, green, blue);
}

void setup() {
  initLEDs();
  Serial.begin(9600);
}

int px = 0;
int pix = 0;
void loop() {
  Serial.print("inc: ");
  Serial.println(px);
  setStrip(px, 255, 255, 255); // turn on current strip
  setStrip( (px+NUM_STRIPS-1)%NUM_STRIPS, 0, 0, 0); // turn off next strip
  setPixel(pix / NUM_STRIPS, pix % STRIP_LENGTH , 255, 0, 55); // turn on current strip
  setPixel( (px+NUM_PIXELS-1) / NUM_STRIPS, (px+NUM_PIXELS-1)%STRIP_LENGTH, 0, 0, 0); // turn off next strip
  leds.show(); // update display
  px = (px+1)%NUM_STRIPS; // increment
  pix = (pix+1)%NUM_PIXELS; // increment
  delay(500);
}
