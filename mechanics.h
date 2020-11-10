#ifndef MECHANICS_H
#define MECHANICS_H

#include "game.h"
#include <stdlib.h>

#define MAX_ENEMIES 5
#define MAX_SPEED 6
#define PORTAL_SIZE 8
#define PORTAL_LIFETIME 50
#define SPAWN_TIME 10
#define MAX_PORTALS 4
#define ENEMY_SIZE 5
#define PLAYER_THICCNESS 16

void clearPlayer(int row, int col, int size);
void updateEnemyType(Player *en);
void updateLocation(Player *pl, int speed);
void createPortal(Portal *portal, Portal *nextPortal, u8 id);
void enablePortalPair(Portal *portal);
u8 isCollision(Player *pl, Player *en);
int atPortal(Portal *portal, Player *pl);

#endif