#include "game_state_machine.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define LED_PIN_1
#define LED_PIN_2
#define LED_PIN_3
#define LED_PIN_4
#define LED_PIN_5
#define LED_PIN_6
#define LED_PIN_7

#define LED_COUNT     7
#define STRIP_COUNT   8

//Adafruit_NeoPixel strip[0](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[1](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[2](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[3](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[4](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[5](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip[6](LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//
//Adafruit_NeoPixel strip0(LED_COUNT, 0, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip1(LED_COUNT, 1, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2(LED_COUNT, 2, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip3(LED_COUNT, 3, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip4(LED_COUNT, 4, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip5(LED_COUNT, 5, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip6(LED_COUNT, 6, NEO_GRB + NEO_KHZ800);

status_t status = {
  .state = sStart,
  .event = eNoEvent,
};

#define USART_BAUDRATE 9600
void setup() {
  // put your setup code here, to run once:
   pinMode(13, OUTPUT);
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  event_handler();
  SerialEvent();
}

void SerialEvent() {
  if(Serial.available()){
    char inChar = (char) Serial.read();
    if (inChar == '1')
    {
      change_event(eStart);
    }
  }
}
