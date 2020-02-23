/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
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
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  initLEDs();
  setStrip(1,255,255,255);
  leds.show();
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    if(inputString.equals("3")){
      setStrip(1, 255,0,0);
    } else if(inputString.equals("6")){
      setStrip(1, 0,255,0);
    } else if(inputString.equals("8")){
      setStrip(1, 0,0,255);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  leds.show();
  delay(100);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:e
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
