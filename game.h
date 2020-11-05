#ifndef GAME_H
#define GAME_H

#include "gba.h"

/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
*
*
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

// ease of utility feature -- lol
typedef signed char i8;

// the enemy type (enemy_t)
/**
 * diagonal - the enemy moves diagonally across the screen
 * horizontal - the enemy moves horizontally across the screen
 * vertical - the enemy moves vertically across the screen
 * user - this is the player :)
 * rebound - the enemy rebounds against the sides of vDraw/hDraw
 */
typedef enum enemy_t {
    diagonal,
    horizontal,
    vertical,
    user,
    rebound
} enemy_t;

// a "player" in the game -- includes enemies
/**
 * row - the current row on the video buffer
 * col - the current column on the video buffer
 * type - the current enemy type
 * engaged - whether the player is currently active in the game
 * rebounds - the number of rebounds the player has made
 * vy - the velocity in the y direction
 * vx - the velocity in the x direction
 */
typedef struct player {
    int row;
    int col;
    enemy_t type;
    u8 engaged;
    u16 rebounds;
    i8 vy;
    i8 vx;
} Player;

typedef struct portal {
    int row;
    int col;
    u16 lifetime;
    u16 color;
} Portal;

void loadGame(void);
void playGameIteration(Player *enemy, int speed, char *level, u32 *currentButtons);

#endif
