#include "libraries/game_state_machine/game_state_machine.h"
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

uint8_t enemy_health = 14;
uint8_t enemy_touch = 0;
uint8_t enemy_position;
uint8_t player_action_variable;
progress_t enemy_progress_by_one = 0;

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
  Count_Time();
}

void SerialEvent() {
  if(Serial.available()){
    char inChar = (char) Serial.read();
    String inputString = "";
    inputString += inChar;
    // switch between bunch of events based 
    // what character is received
    if(inputString.equals("3") || inputString.equals("2")){
      //UP
      if(status.state == sStart) change_event(eStart); //status.event = eStart;
      else change_event(ePlayerActionShoot); //status.event = ePlayerActionShoot;
    } else if(inputString.equals("6") || inputString.equals("7")){
      //LEFT
      change_event(ePlayerActionLeft); //status.event = ePlayerActionLeft;
      
    } else if(inputString.equals("8") || inputString.equals("9")){
      //RIGHT
      change_event(ePlayerActionRight); // status.event = ePlayerActionRight;      
    }
  }
}
