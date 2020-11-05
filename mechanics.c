#include "mechanics.h"
#include "gba.h"
#include "game.h"
#include <stdlib.h>

void clearPlayer(int row, int col, int size) {
    drawRectDMA(row, col, size, size, BLACK);
}

void updateEnemyType(Player *en) {
    int type = randint(0, 3);
    switch (type) {
        case 0:
        // the enemy moves horizontally (can rebound)
            en->col = randint(0, 2) == 0 ? randint(0, 20) : randint(215, 235);
            en->row = randint(10, 155);
            en->vx = en->col <= 20 ? 1 : -1;
            en->type = randint(0, 6) == 0 ? rebound : horizontal;
            en->vy = 0;
            break;
        case 1:
        // the enemy moves vertically (can rebound)
            en->row = randint(0, 2) == 0 ? randint(10, 30) : randint(135, 155);
            en->col = randint(0, 235);
            en->type = randint(0, 6) == 0 ? rebound : vertical;
            en->vx = 0;
            en->vy = en->row <= 30 ? 1 : -1;
            break;
        default:
        // the enemy moves diagonally (can rebound)
            en->col = randint(0, 235);
            en->row = randint(120, 155);
            en->type = randint(0, 6) == 0 ? rebound : diagonal;
            en->vy = -1;
            en->vx = en->col > 120 ? -1 : 1;
    }
    // disengage the enemy
    en->engaged = 0;
    en->rebounds = 0;
}

void updateLocation(Player *pl, int speed) {
    // we use the speed here because it allows us to automatically update the speeds
    // of every enemy at the next level
    pl->col += speed * pl->vx;
    pl->row += speed * pl->vy;

    // if the player is not the user (i.e. an enemy)
    if (pl->type != user) {
        // if the enemy is at the edge of the video buffer...
        if (!(pl->row > 0 && pl->row < 156) || !(pl->col > 0 && pl->col < 236)) {
            // if the enemy is not a rebounder or has performed enough rebounds...
            if (pl->type != rebound || pl->rebounds >= speed) {
                // ...update its type and location
                updateEnemyType(pl);
            // otherwise...
            } else {
                // ...perform the rebound
                pl->vx *= -1;
                pl->vy *= -1;
                pl->rebounds++;
            }
        }
    }
}

// lol this is so bad but somehow it works?
// basically it manually checks to see if there is any white pixel
// where the user's player is -- which means there is an intersection with an enemy
u8 isCollision(Player *pl, int playerSize) {
    for (int r = pl->row; r < pl->row + playerSize; r++) {
        for (int c = pl->col; c < pl->col + playerSize; c++) {
            if (videoBuffer[OFFSET(r, c, WIDTH)] == WHITE) {
                return 1;
            }
        }
    }
    return 0;
}
