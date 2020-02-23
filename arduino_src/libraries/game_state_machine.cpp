#include "game_state_machine.h"

#define LED_PIN 8
#define STRIP_LENGTH 7 // width
#define NUM_STRIPS 8 // height
#define NUM_PIXELS (NUM_STRIPS * STRIP_LENGTH)

#define WHITE   255, 255, 255
#define RED     255, 0, 0
#define GREEN   0, 255, 0
#define NOCOLOR 0, 0, 0

#define INTERVAL    10000

Adafruit_NeoPixel leds((STRIP_LENGTH*NUM_STRIPS), LED_PIN, NEO_GRB + NEO_KHZ800);

/*-------------------------------------------------------------------*/
// LED Code
/*-------------------------------------------------------------------*/

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

void setScreen(int red, int green, int blue){
    for(int i = 0; i < NUM_STRIPS; i++) {
        leds.setStrip(i, red, green, blue);
    }
    leds.show();
}
/*-------------------------------------------------------------------*/
// State Machine Code
/*-------------------------------------------------------------------*/
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
    
    // Flash the whole screen with white once
    setScreen(WHITE);
    delay(100);

    // and go blank
    setScreen(NOCOLOR);
    delay(100);

    // Put starting positions
    // player position according to player.x and player.y
    player.x = 0;
    player.y = 2;
    enemy_position = 6;
}

void Check_For_Touch(void)
{
    Serial.println("Check for Touch!");
    // See if the alien has reached position 1
    // Next progress will be a touch toggling progress
    if(enemy_position == 1)
    {
        change_event(eEnemyResponseTouch);
    }
}

void Check_For_Progress(void)
{
    Serial.println("Check for Progress!");
    // See if it's time to progress by one
    if(enemy_progress_by_one)
    {
        change_event(eEnemyResponseProgress);
    }
}

void Count_Time(void)
{
    // Using this as a global so that we can track time regardless of what state we are in
    Serial.println("Time Up!");
    // Do we need this? or shall we calculate time in check for progress?
    // Putting it in check_for_progress gives with faster response
    currentMillis = millis();
 
    if(currentMillis - previousMillis > INTERVAL) 
    {
    // save the last time you blinked the LED 
        previousMillis = currentMillis;   
        enemy_progress_by_one = 1;
    }
}

void Move_Right(void)
{
    Serial.println("Move Right!");
    // Change LEDs in a way that we change right
    if(player.x < (NUM_PIXELS-1) || player.y < 4)
    {
        setPixel(player.y, player.x, NOCOLOR);
        player.x += 1;
        player.y += 0;
        setPixel(player.y, player.x, GREEN);
        leds.show();
    }
}

void Move_Left(void)
{
    Serial.println("Move Left!");
    // Change LEDs in a way that we change left
    if(player.x > 0 || player.y < 4)
    {
        setPixel(player.y, player.x, NOCOLOR);
        player.x -= 1;
        player.y += 0;
        setPixel(player.y, player.x, GREEN);
        leds.show();
    }
}

void Shoot(void)
{
    Serial.println("Shoot!");
    // Change LEDs in a way that we emulate a shooting action
}

void Progress(void)
{
    Serial.println("Progress!");
    // Change LEDs in a way that the alien comes near
    // When you do change it, flush enemy_progress_by_one
}

void Touch(void)
{
    Serial.println("Touch!");
    // Show touch and some weird ass color
    // I choose rebeccapurple!
}

void Die(void)
{
    Serial.println("Die!");
    // Turn off an LED where shoot hits
}

void Win(void)
{
    Serial.println("Win!");
    // Flash the screen green twice
    setScreen(GREEN);
    leds.show();
    delay(200);
    setScreen(NOCOLOR);
    leds.show();
    delay(200);
    setScreen(GREEN);
    leds.show();
    delay(200);
    setScreen(NOCOLOR);
    leds.show();
    delay(200);
}

void Lose(void)
{
    Serial.println("Lose!");
    // Flash the screen red twice
}

void End_Game(void)
{
    Serial.println("End Game!");
    // Turn the screen white
    setScreen(WHITE);
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