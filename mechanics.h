#ifndef MECHANICS_H
#define MECHANICS_H

#include "game.h"
#include <stdlib.h>

#define MAX_ENEMIES 5
#define MAX_SPEED 6
#define SPAWN_TIME 10

void clearPlayer(int row, int col, int size);
void updateEnemyType(Player *en);
void updateLocation(Player *pl, int speed);
u8 isCollision(Player *pl, int playerSize);

#endif