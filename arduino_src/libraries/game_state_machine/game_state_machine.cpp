#include "game_state_machine.h"

void change_event(event_id_t event)
{
    status.event = event;
}

void change_state(state_id_t state)
{
    status.state = state;
}

void setStrip(int strip, int red, int green, int blue)
{
    ;
}

void setPixel(int strip, int pixel, int red, int green, int blue)
{
    ;
}

void Execute_Start(void)
{
    Serial.println("Start!");
}

void Check_For_Touch(void)
{
    ;
}

void Check_For_Progress(void)
{
    ;
}

void Count_Time(void)
{
    ;
}

void Move_Right(void)
{
    ;
}

void Move_Left(void)
{
    ;
}

void Shoot(void)
{
    ;
}

void Progress(void)
{
    ;
}

void Touch(void)
{
    ;
}

void Die(void)
{
    ;
}

void Win(void)
{
    ;
}

void Lose(void)
{
    ;
}

void End_Game(void)
{
    ;
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