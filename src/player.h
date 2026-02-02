#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct {
    Vector2 pos;
    Vector2 dir;
    Vector2 plane;
} Player;

// Move
void MovePlayer(Player *player, float dt, int world_map[16][16], float cellwidth, float cellheight, float moveSpeed, float rotSpeed);

#endif
