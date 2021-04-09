#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "mechanics.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/garbage.h"
#include "images/gamer.h"
#include "images/player.h"
#include "images/epic.h"
#include "images/reddit.h"

static Player enemies[MAX_ENEMIES];
static Player p;
static volatile u32 iterations = 0;
static Portal portals[MAX_PORTALS];
static volatile size_t num_portals = 2;

/* TODO: */
// Add any additional states you need for your app.
typedef enum {
    START,
    LOAD,
    PLAY,
    PAUSE,
    WIN,
    LOSE,
    RESET
} GBAState;

int main(void) {
    /* TODO: */
    // Manipulate REG_DISPCNT here to set Mode 3. //
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    // Load initial game state
    GBAState state = START;
    GBAState previousState = state;

    size_t num_enemies = 2;
    int speed = 0;
    u32 high_score = 0;
    u8 lost, prevlost;
    u32 frames;

    // start of the number of frames -- basic initialization
    frames = 0;
    iterations = 0;

    while (1) {
        currentButtons = BUTTONS;  // Load the current state of the buttons

        /* TODO: */
        // Manipulate the state machine below as needed //
        // NOTE: Call waitForVBlank() before you draw
        switch (state) {
        case START:
            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = LOAD;
            }
            p.col = 112;
            p.row = 88;
            high_score = 0;
            waitForVBlank();
            if (frames % 30 == 0) {
                drawFullScreenImageDMA(epic);
                frames++;
            }
            else if (frames++ % 15 == 0) {
                drawFullScreenImageDMA(gamer);
            }
            drawImageDMA(p.row, p.col, PLAYER_WIDTH, PLAYER_HEIGHT, player);
            break;

        case LOAD:
            waitForVBlank();
            fillScreenDMA(BLACK);
            speed = 1;
            num_enemies = 2;
            num_portals = 2;
            lost = 0;
            frames = 0;
            p.col = 112;
            p.row = 88;
            prevlost = 0;
            iterations = 0;
            loadGame();
            state = PLAY;
            break;

        case PLAY:
            previousState = state;
            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = PAUSE;
            } else if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                state = RESET;
            }
            char str[2];    // char buffer to display the current game level
            sprintf(str, "%d", (speed+num_enemies-2));
            for (size_t i = 0; i < num_enemies; i++) {
                // play a single iteration of the game - updates a single enemy
                playGameIteration(&enemies[i], speed, str, &currentButtons);

                // collision
                if (isCollision(&p, &enemies[i])) {
                    state = LOSE;
                    break;
                }

                // state changes mid-loop
                if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                    state = PAUSE;
                    break;
                } else if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                    state = RESET;
                    break;
                }
            }

            // increase the score (# of iterations)
            iterations++;

            if (iterations > high_score) {
                high_score = iterations;
            }

            // determine level increase or if the character has won
            if (iterations % 1000 == 0 && speed < MAX_SPEED) {
                drawRectDMA(1, 1, 10, 10, BLACK);
                speed++;
            } else if (iterations % 750 == 0 && num_enemies < MAX_ENEMIES) {
                drawRectDMA(1, 1, 10, 10, BLACK);
                num_enemies++;
            } else if (iterations % 2500 == 0 && num_portals < 4) {
                num_portals += 2;
            }
            if (iterations == 7500) {
                state = WIN;
                waitForVBlank();
                fillScreenDMA(BLACK);
            }
            break;
            // state = ?

        case PAUSE:
            previousState = PLAY;
            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = PLAY;
                waitForVBlank();
                drawRectDMA(1, 120 - 18, 36, 8, BLACK);
                break;
            } else if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                state = RESET;
            }
            waitForVBlank();
            drawString(1, 120 - 18, "PAUSED", WHITE);
            break;

        case WIN:
            previousState = state;
            if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                state = RESET;
            } else if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = LOAD;
            }
            waitForVBlank();
            drawString(88, 120 - 63, "YOU ARE AN EPIC GAMER", GREEN);
            drawImageDMA(80 - 32, 120 - 16, REDDIT_WIDTH, REDDIT_HEIGHT, reddit);
            waitForVBlank();
            if (frames % 30 == 0) {
                drawString(120, 120 - 33, "PLAY AGAIN?", WHITE);
                frames++;
            }
            else if (frames++ % 15 == 0) {
                drawRectDMA(120, 120 - 33, 66, 8, BLACK);
            }
            // state = ?
            break;

        case LOSE:
            previousState = state;
            if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                state = RESET;
            } else if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = LOAD;
            }
            lost = 1;
            waitForVBlank();
            if (lost != prevlost) {
                fillScreenDMA(BLACK);
                prevlost = 1;
            }
            char highScore[32];
            sprintf(highScore, "High Score: %d", high_score);
            drawString(151, 1, highScore, YELLOW);
            drawString(80 - 4, 120 - 54,  "NOT AN EPIC GAMER?", RED);
            char score[100];
            sprintf(score, "Score: %d", iterations);
            drawString(3, 3, score, GREEN);
            // state = ?
            break;
        
        case RESET:
            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                state = START;
            } else if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                state = previousState;
                waitForVBlank();
                fillScreenDMA(BLACK);
                break;
            }
            waitForVBlank();
            fillScreenDMA(BLACK);
            drawString(76, 120 - 93, "ARE YOU SURE YOU WANT TO RESET?", WHITE);
            frames = 0;
            break;
        }

        previousButtons = currentButtons;  // Store the current state of the buttons
    }

    return 0;
}

void loadGame(void) {
    p.vx = 0;
    p.vy = 1;
    for (size_t i = 0; i < MAX_ENEMIES; i++) {
        updateEnemyType(&enemies[i]);
    }
    for (size_t i = 0; i < MAX_PORTALS - 1; i+=2) {
        createPortal(&portals[i], &portals[i+1], i);
    }
}

// a simple function to perform one enemy movement iteration
// ...all in a single vBlank period ;D

void playGameIteration(Player *enemy, int speed, char *level, u32 *currentButtons) {
    waitForVBlank();

    // cover the player and current enemy locations
    // (instead of filling the buffer)
    clearPlayer(p.row, p.col, 16);
    
    for (size_t i = 0; i < num_portals; i++) {
        clearPlayer(portals[i].row, portals[i].col, PORTAL_SIZE);
    }

    clearPlayer(enemy->row, enemy->col, 5);

    // player movement button bindings
    if (KEY_DOWN(BUTTON_RIGHT, *currentButtons)) {
        p.vx = 1;
        p.vy = 0;
    } else if (KEY_DOWN(BUTTON_LEFT, *currentButtons)) {
        p.vx = -1;
        p.vy = 0;
    } else if (KEY_DOWN(BUTTON_DOWN, *currentButtons)) {
        p.vy = 2;
        p.vx = 0;
    } else if (KEY_DOWN(BUTTON_UP, *currentButtons)) {
        p.vx = 0;
        p.vy = -1;
    } else {
        p.vx = 0;
        p.vy = 1;
    }
    
    // portal time!
    if (iterations != 0 && iterations % 600 == 0) {
        for (size_t i = 0; i < num_portals - 1; i+=2) {
            enablePortalPair(&portals[i]);
        }
    }
    for (size_t i = 0; i < num_portals; i++) {
        drawPortal(&portals[i]);

        // player movement button bindings
        if (KEY_DOWN(BUTTON_RIGHT, *currentButtons)) {
            p.vx = 1;
            p.vy = 0;
        } else if (KEY_DOWN(BUTTON_LEFT, *currentButtons)) {
            p.vx = -1;
            p.vy = 0;
        } else if (KEY_DOWN(BUTTON_DOWN, *currentButtons)) {
            p.vy = 2;
            p.vx = 0;
        } else if (KEY_DOWN(BUTTON_UP, *currentButtons)) {
            p.vx = 0;
            p.vy = -1;
        } else {
            p.vx = 0;
            p.vy = 1;
        }
        if ((portals[i].lifetime > 0) && (atPortal(&portals[i], &p) == 1)) {
            portals[i].lifetime = 0;
            portals[i].pair->lifetime = 0;
            p.col = portals[i].pair->col;
            p.row = portals[i].pair->row;
        }
    }
    
    updateLocation(&p, 1);
    // prevent player from leaving the screen
    if (!((p.col < 240-16 && p.row < 160-16) && (p.col > 8 && p.row > 8))) {
        updateLocation(&p, -1);
    }

    drawImageDMA(p.row, p.col, 16, 16, player);

    // update and/or draw the enemy
    if (enemy->engaged < SPAWN_TIME) {
        drawRectDMA(enemy->row, enemy->col, 5, 5, RED);
        enemy->engaged++;
    } else {
        updateLocation(enemy, speed);
        drawRectDMA(enemy->row, enemy->col, 5, 5, WHITE);
    }

    // draw the current level in the top left corner
    drawString(1, 1, level, WHITE);
}

void drawPortal(Portal *portal) {
    if (portal->lifetime > 0) {
        drawRectDMA(portal->row, portal->col, PORTAL_SIZE, PORTAL_SIZE, portal->color);
        portal->lifetime = portal->lifetime - 1;
    }
} 
