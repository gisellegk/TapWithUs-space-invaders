#include "game_state_machine.h"

#define LED_PIN 8
#define STRIP_LENGTH 7
#define NUM_STRIPS 8
#define NUM_PIXELS (NUM_STRIPS * STRIP_LENGTH)

Adafruit_NeoPixel leds((STRIP_LENGTH*NUM_STRIPS), LED_PIN, NEO_GRB + NEO_KHZ800);

void initLEDs(void)
{
  leds.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  leds.show();            // Turn OFF all pixels ASAP
  leds.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void setStrip(int strip, int red, int green, int blue)
{
  int offset = strip*STRIP_LENGTH;
  for(int i=0; i<STRIP_LENGTH; i++) 
  {
    leds.setPixelColor(offset + i, red, green, blue);
  }
}

void setPixel(int strip, int pixel, int red, int green, int blue)
{
  int index = strip*STRIP_LENGTH + pixel;
  leds.setPixelColor(index, red, green, blue);
}

void change_event(event_id_t event)
{
    status.event = event;
}

void change_state(state_id_t state)
{
    status.state = state;
}

void Execute_Start(void)
{
    Serial.println("Start!");
    initLEDs();
}

void Check_For_Touch(void)
{
    Serial.println("Check for Touch!");
}

void Check_For_Progress(void)
{
    Serial.println("Check for Progress!");
}

void Count_Time(void)
{
    Serial.println("Time Up!");
}

void Move_Right(void)
{
    Serial.println("Move Right!");
}

void Move_Left(void)
{
    Serial.println("Move Left!");
}

void Shoot(void)
{
    Serial.println("Shoot!");
}

void Progress(void)
{
    Serial.println("Progress!");
}

void Touch(void)
{
    Serial.println("Touch!");
}

void Die(void)
{
    Serial.println("Die!");
}

void Win(void)
{
    Serial.println("Win!");
}

void Lose(void)
{
    Serial.println("Lose!");
}

void End_Game(void)
{
    Serial.println("End Game!");
}

static inline void Start_State(void)
{
    switch(status.event)
    {
    case eStart:
        Execute_Start();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    case eNoEvent:
        break;
    }
}

static inline void Idle_State(void)
{
    switch(status.event)
    {
    case eNoEvent:
        Check_For_Touch();
        Check_For_Progress();
        Count_Time();
        break;
    case ePlayerActionLeft:
    case ePlayerActionRight:
    case ePlayerActionShoot:
        change_state(sPlayerAction);
        break;
    case eEnemyResponseProgress:
    case eEnemyResponseTouch:
        change_state(sEnemyResponse);
        break;
    }
}

static inline void PlayerAction_State(void)
{
    switch(status.event)
    {
    case ePlayerActionLeft:
        Move_Right();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    case ePlayerActionRight:
        Move_Left();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    case ePlayerActionShoot:
        Shoot();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    }
}

static inline void EnemyResponse_State(void)
{
    switch(status.event)
    {
    case eEnemyResponseProgress:
        Progress();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    case eEnemyResponseTouch:
        Touch();
        change_state(sLose);
        change_event(eLose);
        break;
    case eEnemyResponseOneDead:
        Die();
        change_state(sIdle);
        change_event(eNoEvent);
        break;
    }
}

static inline void Win_State(void)
{
    switch(status.event)
    {
    case eWin:
        Win();
        change_state(sEnd);
        change_event(eEnd);
        break;
    }
}

static inline void Lose_State(void)
{
    switch(status.event)
    {
    case eLose:
        Lose();
        change_state(sEnd);
        change_event(eEnd);
        break;
    }
}

static inline void End_State(void)
{
    switch(status.event)
    {
    case eEnd:
        End_Game();
        change_state(sStart);
        change_event(eNoEvent);
        break;
    }
}

void event_handler(void)
{
    switch(status.state)
    {
    case sStart:
        Start_State();
        break;
    case sIdle:
        Idle_State();
        break;
    case sPlayerAction:
        PlayerAction_State();
    case sEnemyResponse:
        EnemyResponse_State();
    case sWin:
        Win_State();
    case sLose:
        Lose_State();
    case sEnd:
        End_State();
    default:
        Serial.println("Error State");
        while(1);
        break;
    }
}