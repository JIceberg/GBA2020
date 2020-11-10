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

void createPortal(Portal *portal, Portal *nextPortal, u8 id) {
    portal->pair = nextPortal;
    nextPortal->pair = portal;
    
    if (id == 0) {
        portal->color = BLUE;
        nextPortal->color = BLUE;
    } else {
        portal->color = YELLOW;
        nextPortal->color = YELLOW;
    }

    portal->lifetime = 0;
    nextPortal->lifetime = 0;
}

void enablePortalPair(Portal *portal) {
    portal->row = 80 - randint(PLAYER_THICCNESS, 80);
    portal->col = 120 - randint(PLAYER_THICCNESS, 120);
    portal->pair->row = 160 - PLAYER_THICCNESS - portal->row;
    portal->pair->col = 240 - PLAYER_THICCNESS - portal->col;
    portal->lifetime = PORTAL_LIFETIME;
    portal->pair->lifetime = PORTAL_LIFETIME;
}

u8 isCollision(Player *pl, Player *en) {
    coord_pair l1, r1, l2, r2;
    l1.x = pl->col;
    l1.y = pl->row;
    l2.x = en->col;
    l2.y = en->row;
    r1.x = pl->col + PLAYER_THICCNESS;
    r1.y = pl->row + PLAYER_THICCNESS;
    r2.x = en->col + ENEMY_SIZE;
    r2.y = en->row + ENEMY_SIZE;

    if ((l1.y > r2.y) || (l2.y > r1.y)) 
        return 0;

    if ((l1.x > r2.x) || (l2.x > r1.x))
        return 0;
    
    return 1;
}

int atPortal(Portal *portal, Player *pl) {
    // time for an ugly list of coordinate setting!
    coord_pair l1, r1, l2, r2;
    l1.x = pl->col;
    l1.y = pl->row;
    l2.x = portal->col;
    l2.y = portal->row;
    r1.x = pl->col + PLAYER_THICCNESS;
    r1.y = pl->row + PLAYER_THICCNESS;
    r2.x = portal->col + PORTAL_SIZE;
    r2.y = portal->row + PORTAL_SIZE;

    if ((l1.y > r2.y) || (l2.y > r1.y)) 
        return 0;

    if ((l1.x > r2.x) || (l2.x > r1.x))
        return 0;
    
    return 1;
}
