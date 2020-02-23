#ifndef GAME_STATE_MACHINE_H_
#define GAME_STATE_MACHINE_H_

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

typedef enum {
    sStart,
    sIdle,
    sPlayerAction,
    sEnemyResponse,
    sWin,
    sLose,
    sEnd,
} state_id_t;

typedef enum {
    eNoEvent,
    eStart,
    eEnd,
    eWin,
    eLose,
    ePlayerActionLeft,
    ePlayerActionRight,
    ePlayerActionShoot,
    eEnemyResponseProgress,
    eEnemyResponseTouch,
    eEnemyResponseOneDead,
} event_id_t;

typedef struct {
    state_id_t state;
    event_id_t event;
} status_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} position;

typedef enum {
    NoProgress,
    Progress,
} progress_t;

extern status_t status;
extern uint8_t enemy_health;
extern uint8_t enemy_touch;
extern uint8_t player_action_variable;
extern progress_t enemy_progress_by_one;
extern position player;
extern position bullet;

// extern Adafruit_Neopixel strip[7];

void event_handler(void);
void change_state(state_id_t);
void change_event(event_id_t);

#endif /* GAME_STATE_MACHINE_H_ */