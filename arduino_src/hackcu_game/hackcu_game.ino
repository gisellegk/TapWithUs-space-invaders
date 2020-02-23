#include "game_state_machine.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>


uint32_t currentMillis = 0;
uint32_t previousMillis = 0;

status_t status = {
  .state = sStart,
  .event = eNoEvent,
};

position player;
position bullet;

uint8_t game_start = 0; 
uint8_t enemy_count = 7;
uint8_t enemy_health[7];
uint8_t enemy_touch = 0;
uint8_t enemy_position;

uint8_t player_action_variable;
progress_t enemy_progress_by_one;

#define USART_BAUDRATE 9600
void setup() {
  // put your setup code here, to run once:
   pinMode(13, OUTPUT);
   Serial.begin(9600);
   initLEDs();
}

void loop() {
  // put your main code here, to run repeatedly:
  event_handler();
  SerialEvent();
  // Count_Time();
}

void SerialEvent() {
  if(Serial.available()){
    char inChar = (char) Serial.read();
    Serial.println(inChar);
    String inputString = "";
    inputString += inChar;
    // switch between bunch of events based 
    // what character is received
    Serial.println(inputString);

    if(game_start){
      if(inputString.equals("3") || inputString.equals("2")){
        //UP
        change_event(ePlayerActionShoot); //status.event = ePlayerActionShoot;
      }
      else if(inputString.equals("6") || inputString.equals("7")){
        //LEFT
        change_event(ePlayerActionRight); //status.event = ePlayerActionLeft;  
      } else if(inputString.equals("8") || inputString.equals("9")){
        //RIGHT
        change_event(ePlayerActionLeft); // status.event = ePlayerActionRight;      
      }
    }

    if(game_start == 0 && (inputString.equals("3") || inputString.equals("2")) )
    {
      Serial.println("start!");
      change_event(eStart);
      game_start = 1;
    }
  }
}
